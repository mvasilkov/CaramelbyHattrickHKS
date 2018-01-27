
#include "XorString.h"

#include "imgui.h"
#include "imgui_internal.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

ImDrawListSharedData::ImDrawListSharedData()
{
	Font = NULL;
	FontSize = 0.0f;
	CurveTessellationTol = 0.0f;
	ClipRectFullscreen = ImVec4(-8192.f, -8192.f, 8192.f, 8192.f);

	for (int i = 0; i < IM_ARRAYSIZE(CircleVtx12); i++)
	{
		const float a = ((float)i * 2 * IM_PI) / (float)IM_ARRAYSIZE(CircleVtx12);
		CircleVtx12[i] = ImVec2(cosf(a), sinf(a));
	};
};

void ImDrawList::Clear()
{
	CmdBuffer.resize(0);
	IdxBuffer.resize(0);
	VtxBuffer.resize(0);
	Flags = ImDrawListFlags_AntiAliasedLines | ImDrawListFlags_AntiAliasedFill;
	_VtxCurrentIdx = 0;
	_VtxWritePtr = NULL;
	_IdxWritePtr = NULL;
	_ClipRectStack.resize(0);
	_TextureIdStack.resize(0);
	_Path.resize(0);
	_ChannelsCurrent = 0;
	_ChannelsCount = 1;
};

void ImDrawList::ClearFreeMemory()
{
	CmdBuffer.clear();
	IdxBuffer.clear();
	VtxBuffer.clear();
	_VtxCurrentIdx = 0;
	_VtxWritePtr = NULL;
	_IdxWritePtr = NULL;
	_ClipRectStack.clear();
	_TextureIdStack.clear();
	_Path.clear();
	_ChannelsCurrent = 0;
	_ChannelsCount = 1;
	for (int i = 0; i < _Channels.Size; i++)
	{
		if (i == 0) memset(&_Channels[0], 0, sizeof(_Channels[0]));
		_Channels[i].CmdBuffer.clear();
		_Channels[i].IdxBuffer.clear();
	};
	_Channels.clear();
};

#define GetCurrentClipRect() (_ClipRectStack.Size ? _ClipRectStack.Data[_ClipRectStack.Size-1] : _Data->ClipRectFullscreen)
#define GetCurrentTextureId() (_TextureIdStack.Size ? _TextureIdStack.Data[_TextureIdStack.Size-1] : NULL)

void ImDrawList::AddDrawCmd()
{
	ImDrawCmd draw_cmd;
	draw_cmd.ClipRect = GetCurrentClipRect();
	draw_cmd.TextureId = GetCurrentTextureId();
	CmdBuffer.push_back(draw_cmd);
};

void ImDrawList::AddCallback(ImDrawCallback callback, void* callback_data)
{
	ImDrawCmd* current_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
	if (!current_cmd || current_cmd->ElemCount != 0 || current_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		current_cmd = &CmdBuffer.back();
	};
	current_cmd->UserCallback = callback;
	current_cmd->UserCallbackData = callback_data;
	AddDrawCmd();
};

void ImDrawList::UpdateClipRect()
{
	const ImVec4 curr_clip_rect = GetCurrentClipRect();
	ImDrawCmd* curr_cmd = CmdBuffer.Size > 0 ? &CmdBuffer.Data[CmdBuffer.Size - 1] : NULL;
	if (!curr_cmd || (curr_cmd->ElemCount != 0 && memcmp(&curr_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) != 0) || curr_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		return;
	}

	ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
	if (curr_cmd->ElemCount == 0 && prev_cmd && memcmp(&prev_cmd->ClipRect, &curr_clip_rect, sizeof(ImVec4)) == 0 && prev_cmd->TextureId == GetCurrentTextureId() && prev_cmd->UserCallback == NULL)
		CmdBuffer.pop_back();
	else
		curr_cmd->ClipRect = curr_clip_rect;
}

void ImDrawList::UpdateTextureID()
{
	const ImTextureID curr_texture_id = GetCurrentTextureId();
	ImDrawCmd* curr_cmd = CmdBuffer.Size ? &CmdBuffer.back() : NULL;
	if (!curr_cmd || (curr_cmd->ElemCount != 0 && curr_cmd->TextureId != curr_texture_id) || curr_cmd->UserCallback != NULL)
	{
		AddDrawCmd();
		return;
	}

	ImDrawCmd* prev_cmd = CmdBuffer.Size > 1 ? curr_cmd - 1 : NULL;
	if (curr_cmd->ElemCount == 0 && prev_cmd && prev_cmd->TextureId == curr_texture_id && memcmp(&prev_cmd->ClipRect, &GetCurrentClipRect(), sizeof(ImVec4)) == 0 && prev_cmd->UserCallback == NULL)
		CmdBuffer.pop_back();
	else
		curr_cmd->TextureId = curr_texture_id;
}

#undef GetCurrentClipRect
#undef GetCurrentTextureId

void ImDrawList::PushClipRect(ImVec2 cr_min, ImVec2 cr_max, bool intersect_with_current_clip_rect)
{
	ImVec4 cr(cr_min.x, cr_min.y, cr_max.x, cr_max.y);
	if (intersect_with_current_clip_rect && _ClipRectStack.Size)
	{
		ImVec4 current = _ClipRectStack.Data[_ClipRectStack.Size - 1];
		if (cr.x < current.x) cr.x = current.x;
		if (cr.y < current.y) cr.y = current.y;
		if (cr.z > current.z) cr.z = current.z;
		if (cr.w > current.w) cr.w = current.w;
	}
	cr.z = ImMax(cr.x, cr.z);
	cr.w = ImMax(cr.y, cr.w);

	_ClipRectStack.push_back(cr);
	UpdateClipRect();
}

void ImDrawList::PushClipRectFullScreen()
{
	PushClipRect(ImVec2(_Data->ClipRectFullscreen.x, _Data->ClipRectFullscreen.y), ImVec2(_Data->ClipRectFullscreen.z, _Data->ClipRectFullscreen.w));
}

void ImDrawList::PopClipRect()
{
	_ClipRectStack.pop_back();
	UpdateClipRect();
}

void ImDrawList::PushTextureID(const ImTextureID& texture_id)
{
	_TextureIdStack.push_back(texture_id);
	UpdateTextureID();
}

void ImDrawList::PopTextureID()
{
	_TextureIdStack.pop_back();
	UpdateTextureID();
}

void ImDrawList::ChannelsSplit(int channels_count)
{
	int old_channels_count = _Channels.Size;
	if (old_channels_count < channels_count)
		_Channels.resize(channels_count);
	_ChannelsCount = channels_count;

	memset(&_Channels[0], 0, sizeof(ImDrawChannel));
	for (int i = 1; i < channels_count; i++)
	{
		if (i >= old_channels_count)
			_Channels[i] = ImDrawChannel();
		else
		{
			_Channels[i].CmdBuffer.resize(0);
			_Channels[i].IdxBuffer.resize(0);
		}
		if (_Channels[i].CmdBuffer.Size == 0)
		{
			ImDrawCmd draw_cmd;
			draw_cmd.ClipRect = _ClipRectStack.back();
			draw_cmd.TextureId = _TextureIdStack.back();
			_Channels[i].CmdBuffer.push_back(draw_cmd);
		}
	}
}

void ImDrawList::ChannelsMerge()
{
	if (_ChannelsCount <= 1)
		return;

	ChannelsSetCurrent(0);
	if (CmdBuffer.Size && CmdBuffer.back().ElemCount == 0)
		CmdBuffer.pop_back();

	int new_cmd_buffer_count = 0, new_idx_buffer_count = 0;
	for (int i = 1; i < _ChannelsCount; i++)
	{
		ImDrawChannel& ch = _Channels[i];
		if (ch.CmdBuffer.Size && ch.CmdBuffer.back().ElemCount == 0)
			ch.CmdBuffer.pop_back();
		new_cmd_buffer_count += ch.CmdBuffer.Size;
		new_idx_buffer_count += ch.IdxBuffer.Size;
	}
	CmdBuffer.resize(CmdBuffer.Size + new_cmd_buffer_count);
	IdxBuffer.resize(IdxBuffer.Size + new_idx_buffer_count);

	ImDrawCmd* cmd_write = CmdBuffer.Data + CmdBuffer.Size - new_cmd_buffer_count;
	_IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size - new_idx_buffer_count;
	for (int i = 1; i < _ChannelsCount; i++)
	{
		ImDrawChannel& ch = _Channels[i];
		if (int sz = ch.CmdBuffer.Size) { memcpy(cmd_write, ch.CmdBuffer.Data, sz * sizeof(ImDrawCmd)); cmd_write += sz; }
		if (int sz = ch.IdxBuffer.Size) { memcpy(_IdxWritePtr, ch.IdxBuffer.Data, sz * sizeof(unsigned short)); _IdxWritePtr += sz; }
	}
	UpdateClipRect();
	_ChannelsCount = 1;
}

void ImDrawList::ChannelsSetCurrent(int idx)
{
	if (_ChannelsCurrent == idx) return;
	memcpy(&_Channels.Data[_ChannelsCurrent].CmdBuffer, &CmdBuffer, sizeof(CmdBuffer));
	memcpy(&_Channels.Data[_ChannelsCurrent].IdxBuffer, &IdxBuffer, sizeof(IdxBuffer));
	_ChannelsCurrent = idx;
	memcpy(&CmdBuffer, &_Channels.Data[_ChannelsCurrent].CmdBuffer, sizeof(CmdBuffer));
	memcpy(&IdxBuffer, &_Channels.Data[_ChannelsCurrent].IdxBuffer, sizeof(IdxBuffer));
	_IdxWritePtr = IdxBuffer.Data + IdxBuffer.Size;
}

void ImDrawList::PrimReserve(int idx_count, int vtx_count)
{
	ImDrawCmd& draw_cmd = CmdBuffer.Data[CmdBuffer.Size - 1];
	draw_cmd.ElemCount += idx_count;

	int vtx_buffer_old_size = VtxBuffer.Size;
	VtxBuffer.resize(vtx_buffer_old_size + vtx_count);
	_VtxWritePtr = VtxBuffer.Data + vtx_buffer_old_size;

	int idx_buffer_old_size = IdxBuffer.Size;
	IdxBuffer.resize(idx_buffer_old_size + idx_count);
	_IdxWritePtr = IdxBuffer.Data + idx_buffer_old_size;
}

void ImDrawList::PrimRect(const ImVec2& a, const ImVec2& c, ImU32 col)
{
	ImVec2 b(c.x, a.y), d(a.x, c.y), uv(_Data->TexUvWhitePixel);
	unsigned short idx = (unsigned short)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (unsigned short)(idx + 1); _IdxWritePtr[2] = (unsigned short)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (unsigned short)(idx + 2); _IdxWritePtr[5] = (unsigned short)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

void ImDrawList::PrimRectUV(const ImVec2& a, const ImVec2& c, const ImVec2& uv_a, const ImVec2& uv_c, ImU32 col)
{
	ImVec2 b(c.x, a.y), d(a.x, c.y), uv_b(uv_c.x, uv_a.y), uv_d(uv_a.x, uv_c.y);
	unsigned short idx = (unsigned short)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (unsigned short)(idx + 1); _IdxWritePtr[2] = (unsigned short)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (unsigned short)(idx + 2); _IdxWritePtr[5] = (unsigned short)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

void ImDrawList::PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
	unsigned short idx = (unsigned short)_VtxCurrentIdx;
	_IdxWritePtr[0] = idx; _IdxWritePtr[1] = (unsigned short)(idx + 1); _IdxWritePtr[2] = (unsigned short)(idx + 2);
	_IdxWritePtr[3] = idx; _IdxWritePtr[4] = (unsigned short)(idx + 2); _IdxWritePtr[5] = (unsigned short)(idx + 3);
	_VtxWritePtr[0].pos = a; _VtxWritePtr[0].uv = uv_a; _VtxWritePtr[0].col = col;
	_VtxWritePtr[1].pos = b; _VtxWritePtr[1].uv = uv_b; _VtxWritePtr[1].col = col;
	_VtxWritePtr[2].pos = c; _VtxWritePtr[2].uv = uv_c; _VtxWritePtr[2].col = col;
	_VtxWritePtr[3].pos = d; _VtxWritePtr[3].uv = uv_d; _VtxWritePtr[3].col = col;
	_VtxWritePtr += 4;
	_VtxCurrentIdx += 4;
	_IdxWritePtr += 6;
}

void ImDrawList::AddPolyline(const ImVec2* points, const int points_count, ImU32 col, bool closed, float thickness)
{
	if (points_count < 2)
		return;

	const ImVec2 uv = _Data->TexUvWhitePixel;

	int count = points_count;
	if (!closed)
		count = points_count - 1;

	const bool thick_line = thickness > 1.0f;
	if (Flags & ImDrawListFlags_AntiAliasedLines)
	{
		const float AA_SIZE = 1.0f;
		const ImU32 col_trans = col & ~IM_COL32_A_MASK;

		const int idx_count = thick_line ? count * 18 : count * 12;
		const int vtx_count = thick_line ? points_count * 4 : points_count * 3;
		PrimReserve(idx_count, vtx_count);

		ImVec2* temp_normals = (ImVec2*)malloc(points_count * (thick_line ? 5 : 3) * sizeof(ImVec2));
		ImVec2* temp_points = temp_normals + points_count;

		for (int i1 = 0; i1 < count; i1++)
		{
			const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
			ImVec2 diff = points[i2] - points[i1];
			diff *= ImInvLength(diff, 1.0f);
			temp_normals[i1].x = diff.y;
			temp_normals[i1].y = -diff.x;
		}
		if (!closed)
			temp_normals[points_count - 1] = temp_normals[points_count - 2];

		if (!thick_line)
		{
			if (!closed)
			{
				temp_points[0] = points[0] + temp_normals[0] * AA_SIZE;
				temp_points[1] = points[0] - temp_normals[0] * AA_SIZE;
				temp_points[(points_count - 1) * 2 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * AA_SIZE;
				temp_points[(points_count - 1) * 2 + 1] = points[points_count - 1] - temp_normals[points_count - 1] * AA_SIZE;
			}

			unsigned int idx1 = _VtxCurrentIdx;
			for (int i1 = 0; i1 < count; i1++)
			{
				const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
				unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 3;

				ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
				float dmr2 = dm.x*dm.x + dm.y*dm.y;
				if (dmr2 > 0.000001f)
				{
					float scale = 1.0f / dmr2;
					if (scale > 100.0f) scale = 100.0f;
					dm *= scale;
				}
				dm *= AA_SIZE;
				temp_points[i2 * 2 + 0] = points[i2] + dm;
				temp_points[i2 * 2 + 1] = points[i2] - dm;

				_IdxWritePtr[0] = (unsigned short)(idx2 + 0); _IdxWritePtr[1] = (unsigned short)(idx1 + 0); _IdxWritePtr[2] = (unsigned short)(idx1 + 2);
				_IdxWritePtr[3] = (unsigned short)(idx1 + 2); _IdxWritePtr[4] = (unsigned short)(idx2 + 2); _IdxWritePtr[5] = (unsigned short)(idx2 + 0);
				_IdxWritePtr[6] = (unsigned short)(idx2 + 1); _IdxWritePtr[7] = (unsigned short)(idx1 + 1); _IdxWritePtr[8] = (unsigned short)(idx1 + 0);
				_IdxWritePtr[9] = (unsigned short)(idx1 + 0); _IdxWritePtr[10] = (unsigned short)(idx2 + 0); _IdxWritePtr[11] = (unsigned short)(idx2 + 1);
				_IdxWritePtr += 12;

				idx1 = idx2;
			}

			for (int i = 0; i < points_count; i++)
			{
				_VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
				_VtxWritePtr[1].pos = temp_points[i * 2 + 0]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;
				_VtxWritePtr[2].pos = temp_points[i * 2 + 1]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col_trans;
				_VtxWritePtr += 3;
			}
		}
		else
		{
			const float half_inner_thickness = (thickness - AA_SIZE) * 0.5f;
			if (!closed)
			{
				temp_points[0] = points[0] + temp_normals[0] * (half_inner_thickness + AA_SIZE);
				temp_points[1] = points[0] + temp_normals[0] * (half_inner_thickness);
				temp_points[2] = points[0] - temp_normals[0] * (half_inner_thickness);
				temp_points[3] = points[0] - temp_normals[0] * (half_inner_thickness + AA_SIZE);
				temp_points[(points_count - 1) * 4 + 0] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
				temp_points[(points_count - 1) * 4 + 1] = points[points_count - 1] + temp_normals[points_count - 1] * (half_inner_thickness);
				temp_points[(points_count - 1) * 4 + 2] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness);
				temp_points[(points_count - 1) * 4 + 3] = points[points_count - 1] - temp_normals[points_count - 1] * (half_inner_thickness + AA_SIZE);
			}

			unsigned int idx1 = _VtxCurrentIdx;
			for (int i1 = 0; i1 < count; i1++)
			{
				const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
				unsigned int idx2 = (i1 + 1) == points_count ? _VtxCurrentIdx : idx1 + 4;

				ImVec2 dm = (temp_normals[i1] + temp_normals[i2]) * 0.5f;
				float dmr2 = dm.x*dm.x + dm.y*dm.y;
				if (dmr2 > 0.000001f)
				{
					float scale = 1.0f / dmr2;
					if (scale > 100.0f) scale = 100.0f;
					dm *= scale;
				}
				ImVec2 dm_out = dm * (half_inner_thickness + AA_SIZE);
				ImVec2 dm_in = dm * half_inner_thickness;
				temp_points[i2 * 4 + 0] = points[i2] + dm_out;
				temp_points[i2 * 4 + 1] = points[i2] + dm_in;
				temp_points[i2 * 4 + 2] = points[i2] - dm_in;
				temp_points[i2 * 4 + 3] = points[i2] - dm_out;

				_IdxWritePtr[0] = (unsigned short)(idx2 + 1); _IdxWritePtr[1] = (unsigned short)(idx1 + 1); _IdxWritePtr[2] = (unsigned short)(idx1 + 2);
				_IdxWritePtr[3] = (unsigned short)(idx1 + 2); _IdxWritePtr[4] = (unsigned short)(idx2 + 2); _IdxWritePtr[5] = (unsigned short)(idx2 + 1);
				_IdxWritePtr[6] = (unsigned short)(idx2 + 1); _IdxWritePtr[7] = (unsigned short)(idx1 + 1); _IdxWritePtr[8] = (unsigned short)(idx1 + 0);
				_IdxWritePtr[9] = (unsigned short)(idx1 + 0); _IdxWritePtr[10] = (unsigned short)(idx2 + 0); _IdxWritePtr[11] = (unsigned short)(idx2 + 1);
				_IdxWritePtr[12] = (unsigned short)(idx2 + 2); _IdxWritePtr[13] = (unsigned short)(idx1 + 2); _IdxWritePtr[14] = (unsigned short)(idx1 + 3);
				_IdxWritePtr[15] = (unsigned short)(idx1 + 3); _IdxWritePtr[16] = (unsigned short)(idx2 + 3); _IdxWritePtr[17] = (unsigned short)(idx2 + 2);
				_IdxWritePtr += 18;

				idx1 = idx2;
			}

			for (int i = 0; i < points_count; i++)
			{
				_VtxWritePtr[0].pos = temp_points[i * 4 + 0]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col_trans;
				_VtxWritePtr[1].pos = temp_points[i * 4 + 1]; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
				_VtxWritePtr[2].pos = temp_points[i * 4 + 2]; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
				_VtxWritePtr[3].pos = temp_points[i * 4 + 3]; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col_trans;
				_VtxWritePtr += 4;
			}
		}
		_VtxCurrentIdx += (unsigned short)vtx_count;
	}
	else
	{
		const int idx_count = count * 6;
		const int vtx_count = count * 4;
		PrimReserve(idx_count, vtx_count);

		for (int i1 = 0; i1 < count; i1++)
		{
			const int i2 = (i1 + 1) == points_count ? 0 : i1 + 1;
			const ImVec2& p1 = points[i1];
			const ImVec2& p2 = points[i2];
			ImVec2 diff = p2 - p1;
			diff *= ImInvLength(diff, 1.0f);

			const float dx = diff.x * (thickness * 0.5f);
			const float dy = diff.y * (thickness * 0.5f);
			_VtxWritePtr[0].pos.x = p1.x + dy; _VtxWritePtr[0].pos.y = p1.y - dx; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
			_VtxWritePtr[1].pos.x = p2.x + dy; _VtxWritePtr[1].pos.y = p2.y - dx; _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col;
			_VtxWritePtr[2].pos.x = p2.x - dy; _VtxWritePtr[2].pos.y = p2.y + dx; _VtxWritePtr[2].uv = uv; _VtxWritePtr[2].col = col;
			_VtxWritePtr[3].pos.x = p1.x - dy; _VtxWritePtr[3].pos.y = p1.y + dx; _VtxWritePtr[3].uv = uv; _VtxWritePtr[3].col = col;
			_VtxWritePtr += 4;

			_IdxWritePtr[0] = (unsigned short)(_VtxCurrentIdx); _IdxWritePtr[1] = (unsigned short)(_VtxCurrentIdx + 1); _IdxWritePtr[2] = (unsigned short)(_VtxCurrentIdx + 2);
			_IdxWritePtr[3] = (unsigned short)(_VtxCurrentIdx); _IdxWritePtr[4] = (unsigned short)(_VtxCurrentIdx + 2); _IdxWritePtr[5] = (unsigned short)(_VtxCurrentIdx + 3);
			_IdxWritePtr += 6;
			_VtxCurrentIdx += 4;
		}
	}
}

void ImDrawList::AddConvexPolyFilled(const ImVec2* points, const int points_count, ImU32 col)
{
	const ImVec2 uv = _Data->TexUvWhitePixel;

	if (Flags & ImDrawListFlags_AntiAliasedFill)
	{
		const float AA_SIZE = 1.0f;
		const ImU32 col_trans = col & ~IM_COL32_A_MASK;
		const int idx_count = (points_count - 2) * 3 + points_count * 6;
		const int vtx_count = (points_count * 2);
		PrimReserve(idx_count, vtx_count);

		unsigned int vtx_inner_idx = _VtxCurrentIdx;
		unsigned int vtx_outer_idx = _VtxCurrentIdx + 1;
		for (int i = 2; i < points_count; i++)
		{
			_IdxWritePtr[0] = (unsigned short)(vtx_inner_idx); _IdxWritePtr[1] = (unsigned short)(vtx_inner_idx + ((i - 1) << 1)); _IdxWritePtr[2] = (unsigned short)(vtx_inner_idx + (i << 1));
			_IdxWritePtr += 3;
		}

		ImVec2* temp_normals = (ImVec2 *)malloc(points_count * sizeof(ImVec2));
		for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
		{
			const ImVec2& p0 = points[i0];
			const ImVec2& p1 = points[i1];
			ImVec2 diff = p1 - p0;
			diff *= ImInvLength(diff, 1.0f);
			temp_normals[i0].x = diff.y;
			temp_normals[i0].y = -diff.x;
		}

		for (int i0 = points_count - 1, i1 = 0; i1 < points_count; i0 = i1++)
		{
			const ImVec2& n0 = temp_normals[i0];
			const ImVec2& n1 = temp_normals[i1];
			ImVec2 dm = (n0 + n1) * 0.5f;
			float dmr2 = dm.x*dm.x + dm.y*dm.y;
			if (dmr2 > 0.000001f)
			{
				float scale = 1.0f / dmr2;
				if (scale > 100.0f) scale = 100.0f;
				dm *= scale;
			}
			dm *= AA_SIZE * 0.5f;

			_VtxWritePtr[0].pos = (points[i1] - dm); _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
			_VtxWritePtr[1].pos = (points[i1] + dm); _VtxWritePtr[1].uv = uv; _VtxWritePtr[1].col = col_trans;
			_VtxWritePtr += 2;

			_IdxWritePtr[0] = (unsigned short)(vtx_inner_idx + (i1 << 1)); _IdxWritePtr[1] = (unsigned short)(vtx_inner_idx + (i0 << 1)); _IdxWritePtr[2] = (unsigned short)(vtx_outer_idx + (i0 << 1));
			_IdxWritePtr[3] = (unsigned short)(vtx_outer_idx + (i0 << 1)); _IdxWritePtr[4] = (unsigned short)(vtx_outer_idx + (i1 << 1)); _IdxWritePtr[5] = (unsigned short)(vtx_inner_idx + (i1 << 1));
			_IdxWritePtr += 6;
		}
		_VtxCurrentIdx += (unsigned short)vtx_count;
	}
	else
	{
		const int idx_count = (points_count - 2) * 3;
		const int vtx_count = points_count;
		PrimReserve(idx_count, vtx_count);
		for (int i = 0; i < vtx_count; i++)
		{
			_VtxWritePtr[0].pos = points[i]; _VtxWritePtr[0].uv = uv; _VtxWritePtr[0].col = col;
			_VtxWritePtr++;
		}
		for (int i = 2; i < points_count; i++)
		{
			_IdxWritePtr[0] = (unsigned short)(_VtxCurrentIdx); _IdxWritePtr[1] = (unsigned short)(_VtxCurrentIdx + i - 1); _IdxWritePtr[2] = (unsigned short)(_VtxCurrentIdx + i);
			_IdxWritePtr += 3;
		}
		_VtxCurrentIdx += (unsigned short)vtx_count;
	}
}

void ImDrawList::PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12)
{
	if (radius == 0.0f || a_min_of_12 > a_max_of_12)
	{
		_Path.push_back(centre);
		return;
	}
	_Path.reserve(_Path.Size + (a_max_of_12 - a_min_of_12 + 1));
	for (int a = a_min_of_12; a <= a_max_of_12; a++)
	{
		const ImVec2& c = _Data->CircleVtx12[a % IM_ARRAYSIZE(_Data->CircleVtx12)];
		_Path.push_back(ImVec2(centre.x + c.x * radius, centre.y + c.y * radius));
	}
}

void ImDrawList::PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments)
{
	if (radius == 0.0f)
	{
		_Path.push_back(centre);
		return;
	}
	_Path.reserve(_Path.Size + (num_segments + 1));
	for (int i = 0; i <= num_segments; i++)
	{
		const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
		_Path.push_back(ImVec2(centre.x + cosf(a) * radius, centre.y + sinf(a) * radius));
	}
}

void ImDrawList::PathRect(const ImVec2& a, const ImVec2& b, float rounding, int rounding_corners)
{
	rounding = ImMin(rounding, fabsf(b.x - a.x) * (((rounding_corners & ImDrawCornerFlags_Top) == ImDrawCornerFlags_Top) || ((rounding_corners & ImDrawCornerFlags_Bot) == ImDrawCornerFlags_Bot) ? 0.5f : 1.0f) - 1.0f);
	rounding = ImMin(rounding, fabsf(b.y - a.y) * (((rounding_corners & ImDrawCornerFlags_Left) == ImDrawCornerFlags_Left) || ((rounding_corners & ImDrawCornerFlags_Right) == ImDrawCornerFlags_Right) ? 0.5f : 1.0f) - 1.0f);

	if (rounding <= 0.0f || rounding_corners == 0)
	{
		PathLineTo(a);
		PathLineTo(ImVec2(b.x, a.y));
		PathLineTo(b);
		PathLineTo(ImVec2(a.x, b.y));
	}
	else
	{
		const float rounding_tl = (rounding_corners & ImDrawCornerFlags_TopLeft) ? rounding : 0.0f;
		const float rounding_tr = (rounding_corners & ImDrawCornerFlags_TopRight) ? rounding : 0.0f;
		const float rounding_br = (rounding_corners & ImDrawCornerFlags_BotRight) ? rounding : 0.0f;
		const float rounding_bl = (rounding_corners & ImDrawCornerFlags_BotLeft) ? rounding : 0.0f;
		PathArcToFast(ImVec2(a.x + rounding_tl, a.y + rounding_tl), rounding_tl, 6, 9);
		PathArcToFast(ImVec2(b.x - rounding_tr, a.y + rounding_tr), rounding_tr, 9, 12);
		PathArcToFast(ImVec2(b.x - rounding_br, b.y - rounding_br), rounding_br, 0, 3);
		PathArcToFast(ImVec2(a.x + rounding_bl, b.y - rounding_bl), rounding_bl, 3, 6);
	}
}

void ImDrawList::AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	PathLineTo(a + ImVec2(0.5f, 0.5f));
	PathLineTo(b + ImVec2(0.5f, 0.5f));
	PathStroke(col, false, thickness);
}

void ImDrawList::AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	PathRect(a + ImVec2(0.5f, 0.5f), b - ImVec2(0.5f, 0.5f), rounding, rounding_corners_flags);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding, int rounding_corners_flags)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;
	if (rounding > 0.0f)
	{
		PathRect(a, b, rounding, rounding_corners_flags);
		PathFillConvex(col);
	}
	else
	{
		PrimReserve(6, 4);
		PrimRect(a, b, col);
	}
}

void ImDrawList::AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathLineTo(d);
	PathFillConvex(col);
}

void ImDrawList::AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	PathLineTo(a);
	PathLineTo(b);
	PathLineTo(c);
	PathFillConvex(col);
}

void ImDrawList::AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments, float thickness)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
	PathArcTo(centre, radius - 0.5f, 0.0f, a_max, num_segments);
	PathStroke(col, true, thickness);
}

void ImDrawList::AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const float a_max = IM_PI * 2.0f * ((float)num_segments - 1.0f) / (float)num_segments;
	PathArcTo(centre, radius, 0.0f, a_max, num_segments);
	PathFillConvex(col);
}

void ImDrawList::AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	if (text_end == NULL)
		text_end = text_begin + strlen(text_begin);
	if (text_begin == text_end)
		return;
	if (font == NULL)
		font = _Data->Font;
	if (font_size == 0.0f)
		font_size = _Data->FontSize;

	ImVec4 clip_rect = _ClipRectStack.back();
	if (cpu_fine_clip_rect)
	{
		clip_rect.x = ImMax(clip_rect.x, cpu_fine_clip_rect->x);
		clip_rect.y = ImMax(clip_rect.y, cpu_fine_clip_rect->y);
		clip_rect.z = ImMin(clip_rect.z, cpu_fine_clip_rect->z);
		clip_rect.w = ImMin(clip_rect.w, cpu_fine_clip_rect->w);
	}
	font->RenderText(this, font_size, pos, col, clip_rect, text_begin, text_end, wrap_width, cpu_fine_clip_rect != NULL);
}

void ImDrawList::AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end)
{
	AddText(NULL, 0.0f, pos, col, text_begin, text_end);
}

void ImDrawList::AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
	if (push_texture_id)
		PushTextureID(user_texture_id);

	PrimReserve(6, 4);
	PrimRectUV(a, b, uv_a, uv_b, col);

	if (push_texture_id)
		PopTextureID();
}

void ImDrawList::AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
	if (push_texture_id)
		PushTextureID(user_texture_id);

	PrimReserve(6, 4);
	PrimQuadUV(a, b, c, d, uv_a, uv_b, uv_c, uv_d, col);

	if (push_texture_id)
		PopTextureID();
}

void ImDrawList::AddImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners)
{
	if ((col & IM_COL32_A_MASK) == 0)
		return;

	if (rounding <= 0.0f || (rounding_corners & ImDrawCornerFlags_All) == 0)
	{
		AddImage(user_texture_id, a, b, uv_a, uv_b, col);
		return;
	}

	const bool push_texture_id = _TextureIdStack.empty() || user_texture_id != _TextureIdStack.back();
	if (push_texture_id)
		PushTextureID(user_texture_id);

	int vert_start_idx = VtxBuffer.Size;
	PathRect(a, b, rounding, rounding_corners);
	PathFillConvex(col);
	int vert_end_idx = VtxBuffer.Size;
	ImGui::ShadeVertsLinearUV(VtxBuffer.Data + vert_start_idx, VtxBuffer.Data + vert_end_idx, a, b, uv_a, uv_b, true);

	if (push_texture_id)
		PopTextureID();
}

void ImDrawData::DeIndexAllBuffers()
{
	ImVector<ImDrawVert> new_vtx_buffer;
	TotalVtxCount = TotalIdxCount = 0;
	for (int i = 0; i < CmdListsCount; i++)
	{
		ImDrawList* cmd_list = CmdLists[i];
		if (cmd_list->IdxBuffer.empty())
			continue;
		new_vtx_buffer.resize(cmd_list->IdxBuffer.Size);
		for (int j = 0; j < cmd_list->IdxBuffer.Size; j++)
			new_vtx_buffer[j] = cmd_list->VtxBuffer[cmd_list->IdxBuffer[j]];
		cmd_list->VtxBuffer.swap(new_vtx_buffer);
		cmd_list->IdxBuffer.resize(0);
		TotalVtxCount += cmd_list->VtxBuffer.Size;
	}
}

void ImDrawData::ScaleClipRects(const ImVec2& scale)
{
	for (int i = 0; i < CmdListsCount; i++)
	{
		ImDrawList* cmd_list = CmdLists[i];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			ImDrawCmd* cmd = &cmd_list->CmdBuffer[cmd_i];
			cmd->ClipRect = ImVec4(cmd->ClipRect.x * scale.x, cmd->ClipRect.y * scale.y, cmd->ClipRect.z * scale.x, cmd->ClipRect.w * scale.y);
		}
	}
}

void ImGui::ShadeVertsLinearColorGradientKeepAlpha(ImDrawVert* vert_start, ImDrawVert* vert_end, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
	ImVec2 gradient_extent = gradient_p1 - gradient_p0;
	float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
	for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
	{
		float d = ImDot(vert->pos - gradient_p0, gradient_extent);
		float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
		int r = ImLerp((int)(col0 >> IM_COL32_R_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_R_SHIFT) & 0xFF, t);
		int g = ImLerp((int)(col0 >> IM_COL32_G_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_G_SHIFT) & 0xFF, t);
		int b = ImLerp((int)(col0 >> IM_COL32_B_SHIFT) & 0xFF, (int)(col1 >> IM_COL32_B_SHIFT) & 0xFF, t);
		vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
	}
}

void ImGui::ShadeVertsLinearAlphaGradientForLeftToRightText(ImDrawVert* vert_start, ImDrawVert* vert_end, float gradient_p0_x, float gradient_p1_x)
{
	float gradient_extent_x = gradient_p1_x - gradient_p0_x;
	float gradient_inv_length2 = 1.0f / (gradient_extent_x * gradient_extent_x);
	int full_alpha_count = 0;
	for (ImDrawVert* vert = vert_end - 1; vert >= vert_start; vert--)
	{
		float d = (vert->pos.x - gradient_p0_x) * (gradient_extent_x);
		float alpha_mul = 1.0f - ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
		if (alpha_mul >= 1.0f && ++full_alpha_count > 2) return;
		int a = (int)(((vert->col >> IM_COL32_A_SHIFT) & 0xFF) * alpha_mul);
		vert->col = (vert->col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
	}
}

void ImGui::ShadeVertsLinearUV(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp)
{
	const ImVec2 size = b - a;
	const ImVec2 uv_size = uv_b - uv_a;
	const ImVec2 scale = ImVec2(
		size.x != 0.0f ? (uv_size.x / size.x) : 0.0f,
		size.y != 0.0f ? (uv_size.y / size.y) : 0.0f);

	if (clamp)
	{
		const ImVec2 min = ImMin(uv_a, uv_b);
		const ImVec2 max = ImMax(uv_a, uv_b);

		for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
			vertex->uv = ImClamp(uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale), min, max);
	}
	else
	{
		for (ImDrawVert* vertex = vert_start; vertex < vert_end; ++vertex)
			vertex->uv = uv_a + ImMul(ImVec2(vertex->pos.x, vertex->pos.y) - a, scale);
	}
}

ImFontConfig::ImFontConfig()
{
	FontData = NULL;
	FontDataSize = 0;
	FontDataOwnedByAtlas = true;
	FontNo = 0;
	SizePixels = 0.0f;
	OversampleH = 3;
	OversampleV = 1;
	PixelSnapH = false;
	GlyphExtraSpacing = ImVec2(0.0f, 0.0f);
	GlyphOffset = ImVec2(0.0f, 0.0f);
	GlyphRanges = NULL;
	MergeMode = false;
	RasterizerFlags = 0x00;
	RasterizerMultiply = 1.0f;
	DstFont = NULL;
};

const int FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF = 90;
const int FONT_ATLAS_DEFAULT_TEX_DATA_H = 27;
const unsigned int FONT_ATLAS_DEFAULT_TEX_DATA_ID = 0x80000000;

const ImVec2 FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[ImGuiMouseCursor_Count_][3] =
{
	{ ImVec2(0, 3), ImVec2(12, 19), ImVec2(0, 0) },
{ ImVec2(13, 0), ImVec2(7, 16), ImVec2(4, 8) },
{ ImVec2(31, 0), ImVec2(23, 23), ImVec2(11, 11) },
{ ImVec2(21, 0), ImVec2(9, 23), ImVec2(5, 11) },
{ ImVec2(55, 18),ImVec2(23, 9), ImVec2(11, 5) },
{ ImVec2(73, 0), ImVec2(17, 17), ImVec2(9, 9) },
{ ImVec2(55, 0), ImVec2(17, 17), ImVec2(9, 9) },
};

ImFontAtlas::ImFontAtlas()
{
	TexID = NULL;
	TexDesiredWidth = 0;
	TexGlyphPadding = 1;
	TexPixelsAlpha8 = NULL;
	TexPixelsRGBA32 = NULL;
	TexWidth = TexHeight = 0;
	TexUvWhitePixel = ImVec2(0, 0);
	for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
		CustomRectIds[n] = -1;
}

ImFontAtlas::~ImFontAtlas()
{
	Clear();
}

void ImFontAtlas::ClearInputData()
{
	for (int i = 0; i < ConfigData.Size; i++)
		if (ConfigData[i].FontData && ConfigData[i].FontDataOwnedByAtlas)
		{
			free(ConfigData[i].FontData);
			ConfigData[i].FontData = NULL;
		}

	for (int i = 0; i < Fonts.Size; i++)
		if (Fonts[i]->ConfigData >= ConfigData.Data && Fonts[i]->ConfigData < ConfigData.Data + ConfigData.Size)
		{
			Fonts[i]->ConfigData = NULL;
			Fonts[i]->ConfigDataCount = 0;
		}
	ConfigData.clear();
	CustomRects.clear();
	for (int n = 0; n < IM_ARRAYSIZE(CustomRectIds); n++)
		CustomRectIds[n] = -1;
}

void ImFontAtlas::ClearTexData()
{
	if (TexPixelsAlpha8) free(TexPixelsAlpha8);
	if (TexPixelsRGBA32) free(TexPixelsRGBA32);
	TexPixelsAlpha8 = NULL;
	TexPixelsRGBA32 = NULL;
}

void ImFontAtlas::ClearFonts()
{
	for (int i = 0; i < Fonts.Size; i++)
	{
		if (Fonts[i])
			delete Fonts[i], Fonts[i] = NULL;
	};
	Fonts.clear();
}

void ImFontAtlas::Clear()
{
	ClearInputData();
	ClearTexData();
	ClearFonts();
}

void ImFontAtlas::GetTexDataAsAlpha8(unsigned char ** out_pixels, int * out_width, int * out_height, int * out_bytes_per_pixel)
{
	if (TexPixelsAlpha8 == NULL) Build();
	*out_pixels = TexPixelsAlpha8;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 1;
};

void ImFontAtlas::GetTexDataAsRGBA32(unsigned char ** out_pixels, int * out_width, int * out_height, int * out_bytes_per_pixel)
{
	static int n = 0;
	if (!TexPixelsRGBA32)
	{
		unsigned char * pixels = NULL;
		GetTexDataAsAlpha8(&pixels, NULL, NULL);
		if (pixels)
		{
			TexPixelsRGBA32 = (unsigned int *)malloc((size_t)(TexWidth * TexHeight * 4));
			const unsigned char * src = pixels;
			unsigned int * dst = TexPixelsRGBA32;
			for (n = TexWidth * TexHeight; n > 0; n--) *dst++ = IM_COL32(255, 255, 255, (unsigned int)(*src++));
		};
	};
	*out_pixels = (unsigned char *)TexPixelsRGBA32;
	if (out_width) *out_width = TexWidth;
	if (out_height) *out_height = TexHeight;
	if (out_bytes_per_pixel) *out_bytes_per_pixel = 4;
};

ImFont* ImFontAtlas::AddFont(const ImFontConfig* font_cfg)
{
	if (!font_cfg->MergeMode)
	{
		ImFont * Font = new ImFont; Fonts.push_back(Font);
	};

	ConfigData.push_back(*font_cfg);
	ImFontConfig& new_font_cfg = ConfigData.back();
	if (!new_font_cfg.DstFont)
		new_font_cfg.DstFont = Fonts.back();
	if (!new_font_cfg.FontDataOwnedByAtlas)
	{
		new_font_cfg.FontData = malloc(new_font_cfg.FontDataSize);
		new_font_cfg.FontDataOwnedByAtlas = true;
		memcpy(new_font_cfg.FontData, font_cfg->FontData, (size_t)new_font_cfg.FontDataSize);
	}

	ClearTexData();
	return new_font_cfg.DstFont;
}

ImFont* ImFontAtlas::AddFontFromFileTTF(const char * filename, float size_pixels)
{
	int data_size = 0;
	void * data = ImFileLoadToMemory(filename, XorString("rb"), &data_size);
	return AddFontFromMemoryTTF(data, data_size, size_pixels);
}

ImFont* ImFontAtlas::AddFontFromMemoryTTF(void * ttf_data, int ttf_size, float size_pixels)
{
	ImFontConfig font_cfg = ImFontConfig();
	font_cfg.FontData = ttf_data;
	font_cfg.FontDataSize = ttf_size;
	font_cfg.SizePixels = size_pixels;
	return AddFont(&font_cfg);
}

int ImFontAtlas::AddCustomRectRegular(unsigned int id, int width, int height)
{
	CustomRect r;
	r.ID = id;
	r.Width = (unsigned short)width;
	r.Height = (unsigned short)height;
	CustomRects.push_back(r);
	return CustomRects.Size - 1;
}

int ImFontAtlas::AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset)
{
	CustomRect r;
	r.ID = id;
	r.Width = (unsigned short)width;
	r.Height = (unsigned short)height;
	r.GlyphAdvanceX = advance_x;
	r.GlyphOffset = offset;
	r.Font = font;
	CustomRects.push_back(r);
	return CustomRects.Size - 1;
}

void ImFontAtlas::CalcCustomRectUV(const CustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max)
{
	*out_uv_min = ImVec2((float)rect->X / TexWidth, (float)rect->Y / TexHeight);
	*out_uv_max = ImVec2((float)(rect->X + rect->Width) / TexWidth, (float)(rect->Y + rect->Height) / TexHeight);
}

bool ImFontAtlas::Build()
{
	return ImFontAtlasBuildWithStbTruetype(this);
};

void ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char * out_table, short size, float in_brighten_factor)
{
	for (unsigned int i = 0, value = 0; i < size; i++)
		value = (unsigned int)(i * in_brighten_factor), out_table[i] = value > 255 ? 255 : (value & 0x000000FF);
};

void ImFontAtlasBuildMultiplyRectAlpha8(unsigned char * table, unsigned char * pixels, int x, int y, int w, int h, int stride)
{
	unsigned char * data = pixels + x + y * stride;
	for (int j = h; j > 0; j--, data += stride)
		for (int i = 0; i < w; i++)
			data[i] = table[data[i]];
};

bool ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas)
{
	ImFontAtlasBuildRegisterDefaultCustomRects(atlas);

	atlas->TexID = NULL;
	atlas->TexWidth = atlas->TexHeight = 0;
	atlas->TexUvWhitePixel = ImVec2(0, 0);
	atlas->ClearTexData();

	int total_glyphs_count = 0;
	int total_ranges_count = 0;
	for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = atlas->ConfigData[input_i];
		if (!cfg.GlyphRanges) cfg.GlyphRanges = atlas->GetGlyphRangesDefault();
		for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2, total_ranges_count++)
			total_glyphs_count += (in_range[1] - in_range[0]) + 1;
	}

	atlas->TexWidth = (atlas->TexDesiredWidth > 0) ? atlas->TexDesiredWidth : (total_glyphs_count > 4000) ? 4096 : (total_glyphs_count > 2000) ? 2048 : (total_glyphs_count > 1000) ? 1024 : 512;
	atlas->TexHeight = 0;

	const int max_tex_height = XS_RANDOM_SIZE * 32;
	stbtt_pack_context spc = {};
	stbtt_PackBegin(&spc, NULL, atlas->TexWidth, max_tex_height, 0, atlas->TexGlyphPadding, NULL);
	stbtt_PackSetOversampling(&spc, 1, 1);

	ImFontAtlasBuildPackCustomRects(atlas, spc.pack_info);

	struct ImFontTempBuildData
	{
		stbtt_fontinfo FontInfo;
		stbrp_rect* Rects;
		int RectsCount;
		stbtt_pack_range* Ranges;
		int RangesCount;
	};
	ImFontTempBuildData* tmp_array = (ImFontTempBuildData *)malloc((size_t)atlas->ConfigData.Size * sizeof(ImFontTempBuildData));
	for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = atlas->ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];
		const int font_offset = stbtt_GetFontOffsetForIndex(cfg.FontNo);
		if (!stbtt_InitFont(&tmp.FontInfo, (unsigned char*)cfg.FontData, font_offset))
		{
			atlas->TexWidth = atlas->TexHeight = 0;
			free(tmp_array);
			return false;
		}
	}

	int buf_packedchars_n = 0, buf_rects_n = 0, buf_ranges_n = 0;
	stbtt_packedchar* buf_packedchars = (stbtt_packedchar *)malloc(total_glyphs_count * sizeof(stbtt_packedchar));
	stbrp_rect* buf_rects = (stbrp_rect *)malloc(total_glyphs_count * sizeof(stbrp_rect));
	stbtt_pack_range* buf_ranges = (stbtt_pack_range *)malloc(total_ranges_count * sizeof(stbtt_pack_range));
	memset(buf_packedchars, 0, total_glyphs_count * sizeof(stbtt_packedchar));
	memset(buf_rects, 0, total_glyphs_count * sizeof(stbrp_rect));
	memset(buf_ranges, 0, total_ranges_count * sizeof(stbtt_pack_range));

	for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = atlas->ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];

		int font_glyphs_count = 0;
		int font_ranges_count = 0;
		for (const ImWchar* in_range = cfg.GlyphRanges; in_range[0] && in_range[1]; in_range += 2, font_ranges_count++)
			font_glyphs_count += (in_range[1] - in_range[0]) + 1;
		tmp.Ranges = buf_ranges + buf_ranges_n;
		tmp.RangesCount = font_ranges_count;
		buf_ranges_n += font_ranges_count;
		for (int i = 0; i < font_ranges_count; i++)
		{
			const ImWchar* in_range = &cfg.GlyphRanges[i * 2];
			stbtt_pack_range& range = tmp.Ranges[i];
			range.font_size = cfg.SizePixels;
			range.first_unicode_codepoint_in_range = in_range[0];
			range.num_chars = (in_range[1] - in_range[0]) + 1;
			range.chardata_for_range = buf_packedchars + buf_packedchars_n;
			buf_packedchars_n += range.num_chars;
		}

		tmp.Rects = buf_rects + buf_rects_n;
		tmp.RectsCount = font_glyphs_count;
		buf_rects_n += font_glyphs_count;
		stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
		int n = stbtt_PackFontRangesGatherRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
		stbrp_pack_rects((stbrp_context*)spc.pack_info, tmp.Rects, n);

		for (int i = 0; i < n; i++)
			if (tmp.Rects[i].was_packed)
				atlas->TexHeight = ImMax(atlas->TexHeight, tmp.Rects[i].y + tmp.Rects[i].h);
	}

	atlas->TexHeight = ImUpperPowerOfTwo(atlas->TexHeight);
	atlas->TexPixelsAlpha8 = (unsigned char *)malloc(atlas->TexWidth * atlas->TexHeight);
	memset(atlas->TexPixelsAlpha8, 0, atlas->TexWidth * atlas->TexHeight);
	spc.pixels = atlas->TexPixelsAlpha8;
	spc.height = atlas->TexHeight;

	for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = atlas->ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];
		stbtt_PackSetOversampling(&spc, cfg.OversampleH, cfg.OversampleV);
		stbtt_PackFontRangesRenderIntoRects(&spc, &tmp.FontInfo, tmp.Ranges, tmp.RangesCount, tmp.Rects);
		if (cfg.RasterizerMultiply != 1.0f)
		{
			unsigned char multiply_table[SIZE_T(XS_RANDOM_SIZE * 1.075f)] = {};
			ImFontAtlasBuildMultiplyCalcLookupTable(multiply_table, ARRAYSIZE(multiply_table), cfg.RasterizerMultiply);
			for (const stbrp_rect* r = tmp.Rects; r != tmp.Rects + tmp.RectsCount; r++)
				if (r->was_packed)
					ImFontAtlasBuildMultiplyRectAlpha8(multiply_table, spc.pixels, r->x, r->y, r->w, r->h, spc.stride_in_bytes);
		}
		tmp.Rects = NULL;
	}

	stbtt_PackEnd(&spc), free(buf_rects), buf_rects = NULL;

	for (int input_i = 0; input_i < atlas->ConfigData.Size; input_i++)
	{
		ImFontConfig& cfg = atlas->ConfigData[input_i];
		ImFontTempBuildData& tmp = tmp_array[input_i];
		ImFont* dst_font = cfg.DstFont;

		const float font_scale = stbtt_ScaleForPixelHeight(&tmp.FontInfo, cfg.SizePixels);
		int unscaled_ascent, unscaled_descent, unscaled_line_gap;
		stbtt_GetFontVMetrics(&tmp.FontInfo, &unscaled_ascent, &unscaled_descent, &unscaled_line_gap);

		const float ascent = unscaled_ascent * font_scale;
		const float descent = unscaled_descent * font_scale;
		ImFontAtlasBuildSetupFont(atlas, dst_font, &cfg, ascent, descent);
		const float off_x = cfg.GlyphOffset.x;
		const float off_y = cfg.GlyphOffset.y + (float)(int)(dst_font->Ascent + 0.5f);

		for (int i = 0; i < tmp.RangesCount; i++)
		{
			stbtt_pack_range& range = tmp.Ranges[i];
			for (int char_idx = 0; char_idx < range.num_chars; char_idx += 1)
			{
				const stbtt_packedchar& pc = range.chardata_for_range[char_idx];
				if (!pc.x0 && !pc.x1 && !pc.y0 && !pc.y1)
					continue;

				const int codepoint = range.first_unicode_codepoint_in_range + char_idx;
				if (cfg.MergeMode && dst_font->FindGlyph((unsigned short)codepoint))
					continue;

				stbtt_aligned_quad q;
				float dummy_x = 0.0f, dummy_y = 0.0f;
				stbtt_GetPackedQuad(range.chardata_for_range, atlas->TexWidth, atlas->TexHeight, char_idx, &dummy_x, &dummy_y, &q, 0);
				dst_font->AddGlyph((ImWchar)codepoint, q.x0 + off_x, q.y0 + off_y, q.x1 + off_x, q.y1 + off_y, q.s0, q.t0, q.s1, q.t1, pc.xadvance);
			}
		}
	}

	free(buf_packedchars);
	free(buf_ranges);
	free(tmp_array);

	ImFontAtlasBuildFinish(atlas);

	return true;
}

void ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas* atlas)
{
	if (atlas->CustomRectIds[0] < 0)
		atlas->CustomRectIds[0] = atlas->AddCustomRectRegular(FONT_ATLAS_DEFAULT_TEX_DATA_ID, FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF * 2 + 1, FONT_ATLAS_DEFAULT_TEX_DATA_H);
}

void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent)
{
	if (!font_config->MergeMode)
	{
		font->ClearOutputData();
		font->FontSize = font_config->SizePixels;
		font->ConfigData = font_config;
		font->ContainerAtlas = atlas;
		font->Ascent = ascent;
		font->Descent = descent;
	}
	font->ConfigDataCount++;
}

void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* pack_context_opaque)
{
	stbrp_context* pack_context = (stbrp_context*)pack_context_opaque;

	ImVector<ImFontAtlas::CustomRect>& user_rects = atlas->CustomRects;
	ImVector<stbrp_rect> pack_rects;
	pack_rects.resize(user_rects.Size);
	memset(pack_rects.Data, 0, sizeof(stbrp_rect) * user_rects.Size);
	for (int i = 0; i < user_rects.Size; i++)
	{
		pack_rects[i].w = user_rects[i].Width;
		pack_rects[i].h = user_rects[i].Height;
	}
	stbrp_pack_rects(pack_context, &pack_rects[0], pack_rects.Size);
	for (int i = 0; i < pack_rects.Size; i++)
		if (pack_rects[i].was_packed)
		{
			user_rects[i].X = pack_rects[i].x;
			user_rects[i].Y = pack_rects[i].y;
			atlas->TexHeight = ImMax(atlas->TexHeight, pack_rects[i].y + pack_rects[i].h);
		}
}

void ImFontAtlasBuildRenderDefaultTexData(ImFontAtlas* atlas)
{
	ImFontAtlas::CustomRect& r = atlas->CustomRects[atlas->CustomRectIds[0]];

	for (int y = 0, n = 0; y < FONT_ATLAS_DEFAULT_TEX_DATA_H; y++)
		for (int x = 0; x < FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF; x++, n++)
		{
			const int offset0 = (int)(r.X + x) + (int)(r.Y + y) * atlas->TexWidth;
			const int offset1 = offset0 + FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
			atlas->TexPixelsAlpha8[offset0] = 0xFF, atlas->TexPixelsAlpha8[offset1] = 0xFF;
		}
	const ImVec2 tex_uv_scale(1.0f / atlas->TexWidth, 1.0f / atlas->TexHeight);
	atlas->TexUvWhitePixel = ImVec2((r.X + 0.5f) * tex_uv_scale.x, (r.Y + 0.5f) * tex_uv_scale.y);

	for (int type = 0; type < ImGuiMouseCursor_Count_; type++)
	{
		ImGuiMouseCursorData& cursor_data = GImGui->MouseCursorData[type];
		ImVec2 pos = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[type][0] + ImVec2((float)r.X, (float)r.Y);
		const ImVec2 size = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[type][1];
		cursor_data.Type = type;
		cursor_data.Size = size;
		cursor_data.HotOffset = FONT_ATLAS_DEFAULT_TEX_CURSOR_DATA[type][2];
		cursor_data.TexUvMin[0] = (pos)* tex_uv_scale;
		cursor_data.TexUvMax[0] = (pos + size) * tex_uv_scale;
		pos.x += FONT_ATLAS_DEFAULT_TEX_DATA_W_HALF + 1;
		cursor_data.TexUvMin[1] = (pos)* tex_uv_scale;
		cursor_data.TexUvMax[1] = (pos + size) * tex_uv_scale;
	}
}

void ImFontAtlasBuildFinish(ImFontAtlas* atlas)
{
	ImFontAtlasBuildRenderDefaultTexData(atlas);

	for (int i = 0; i < atlas->CustomRects.Size; i++)
	{
		const ImFontAtlas::CustomRect& r = atlas->CustomRects[i];
		if (r.Font == NULL || r.ID > 0x10000)
			continue;

		ImVec2 uv0, uv1;
		atlas->CalcCustomRectUV(&r, &uv0, &uv1);
		r.Font->AddGlyph((ImWchar)r.ID, r.GlyphOffset.x, r.GlyphOffset.y, r.GlyphOffset.x + r.Width, r.GlyphOffset.y + r.Height, uv0.x, uv0.y, uv1.x, uv1.y, r.GlyphAdvanceX);
	}

	for (int i = 0; i < atlas->Fonts.Size; i++)
		atlas->Fonts[i]->BuildLookupTable();
}

const ImWchar* ImFontAtlas::GetGlyphRangesDefault()
{
	static const ImWchar ranges[] = { 0x00000020, 0x000000FF, 0, };
	return &ranges[0];
};

void ImFontAtlas::GlyphRangesBuilder::AddText(const char* text, const char* text_end)
{
	while (text_end ? (text < text_end) : *text)
	{
		unsigned int c = 0;
		int c_len = ImTextCharFromUtf8(&c, text, text_end);
		text += c_len;
		if (c_len == 0) break;
		if (c < 0x00010000) AddChar((ImWchar)c);
	};
};

void ImFontAtlas::GlyphRangesBuilder::AddRanges(const ImWchar* ranges)
{
	for (; ranges[0]; ranges += 2)
		for (ImWchar c = ranges[0]; c <= ranges[1]; c++)
			AddChar(c);
}

ImFont::ImFont()
{
	Scale = 1.0f;
	FallbackChar = (ImWchar) '?';
	DisplayOffset = ImVec2(0.0f, 1.0f);
	ClearOutputData();
};

ImFont::~ImFont()
{
	ClearOutputData();
};

void ImFont::ClearOutputData()
{
	FontSize = 0.0f;
	Glyphs.clear();
	IndexAdvanceX.clear();
	IndexLookup.clear();
	FallbackGlyph = NULL;
	FallbackAdvanceX = 0.0f;
	ConfigDataCount = 0;
	ConfigData = NULL;
	ContainerAtlas = NULL;
	Ascent = Descent = 0.0f;
	MetricsTotalSurface = 0;
}

void ImFont::BuildLookupTable()
{
	int max_codepoint = 0;
	for (int i = 0; i != Glyphs.Size; i++)
		max_codepoint = ImMax(max_codepoint, (int)Glyphs[i].Codepoint);

	IndexAdvanceX.clear();
	IndexLookup.clear();
	GrowIndex(max_codepoint + 1);
	for (int i = 0; i < Glyphs.Size; i++)
	{
		int codepoint = (int)Glyphs[i].Codepoint;
		IndexAdvanceX[codepoint] = Glyphs[i].AdvanceX;
		IndexLookup[codepoint] = (unsigned short)i;
	}
	if (FindGlyph((unsigned short)' '))
	{
		if (Glyphs.back().Codepoint != '\t')
			Glyphs.resize(Glyphs.Size + 1);
		ImFontGlyph& tab_glyph = Glyphs.back();
		tab_glyph = *FindGlyph((unsigned short)' ');
		tab_glyph.Codepoint = '\t';
		tab_glyph.AdvanceX *= 4;
		IndexAdvanceX[(int)tab_glyph.Codepoint] = (float)tab_glyph.AdvanceX;
		IndexLookup[(int)tab_glyph.Codepoint] = (unsigned short)(Glyphs.Size - 1);
	}

	FallbackGlyph = NULL;
	FallbackGlyph = FindGlyph(FallbackChar);
	FallbackAdvanceX = FallbackGlyph ? FallbackGlyph->AdvanceX : 0.0f;
	for (int i = 0; i < max_codepoint + 1; i++)
		if (IndexAdvanceX[i] < 0.0f)
			IndexAdvanceX[i] = FallbackAdvanceX;
}

void ImFont::SetFallbackChar(ImWchar c)
{
	FallbackChar = c;
	BuildLookupTable();
}

void ImFont::GrowIndex(int new_size)
{
	if (new_size <= IndexLookup.Size)
		return;
	IndexAdvanceX.resize(new_size, -1.0f);
	IndexLookup.resize(new_size, (unsigned short)-1);
}

void ImFont::AddGlyph(ImWchar codepoint, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x)
{
	Glyphs.resize(Glyphs.Size + 1);
	ImFontGlyph& glyph = Glyphs.back();
	glyph.Codepoint = (ImWchar)codepoint;
	glyph.X0 = x0;
	glyph.Y0 = y0;
	glyph.X1 = x1;
	glyph.Y1 = y1;
	glyph.U0 = u0;
	glyph.V0 = v0;
	glyph.U1 = u1;
	glyph.V1 = v1;
	glyph.AdvanceX = advance_x + ConfigData->GlyphExtraSpacing.x;

	if (ConfigData->PixelSnapH)
		glyph.AdvanceX = (float)(int)(glyph.AdvanceX + 0.5f);

	MetricsTotalSurface += (int)((glyph.U1 - glyph.U0) * ContainerAtlas->TexWidth + 1.99f) * (int)((glyph.V1 - glyph.V0) * ContainerAtlas->TexHeight + 1.99f);
}

const ImFontGlyph* ImFont::FindGlyph(ImWchar c) const
{
	if (c < IndexLookup.Size)
	{
		const unsigned short i = IndexLookup[c];
		if (i != (unsigned short)-1)
			return &Glyphs.Data[i];
	}
	return FallbackGlyph;
};

const char* ImFont::CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const
{
	float line_width = 0.0f;
	float word_width = 0.0f;
	float blank_width = 0.0f;
	wrap_width /= scale;

	const char* word_end = text;
	const char* prev_word_end = NULL;
	bool inside_word = true;

	const char* s = text;
	while (s < text_end)
	{
		unsigned int c = (unsigned int)*s;
		const char* next_s;
		if (c < 0x00000080)
			next_s = s + 1;
		else
			next_s = s + ImTextCharFromUtf8(&c, s, text_end);
		if (c == 0)
			break;

		if (c < 32)
		{
			if (c == '\n')
			{
				line_width = word_width = blank_width = 0.0f;
				inside_word = true;
				s = next_s;
				continue;
			}
			if (c == '\r')
			{
				s = next_s;
				continue;
			}
		}

		const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX[(int)c] : FallbackAdvanceX);
		if (ImCharIsSpace(c))
		{
			if (inside_word)
			{
				line_width += blank_width;
				blank_width = 0.0f;
				word_end = s;
			}
			blank_width += char_width;
			inside_word = false;
		}
		else
		{
			word_width += char_width;
			if (inside_word)
			{
				word_end = next_s;
			}
			else
			{
				prev_word_end = word_end;
				line_width += word_width + blank_width;
				word_width = blank_width = 0.0f;
			}

			inside_word = !(c == '.' || c == ',' || c == ';' || c == '!' || c == '?' || c == '\"');
		}

		if (line_width + word_width >= wrap_width)
		{
			if (word_width < wrap_width)
				s = prev_word_end ? prev_word_end : word_end;
			break;
		}

		s = next_s;
	}

	return s;
}

ImVec2 ImFont::CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end, const char** remaining) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin);

	const float line_height = size;
	const float scale = size / FontSize;

	ImVec2 text_size = ImVec2(0, 0);
	float line_width = 0.0f;

	const bool word_wrap_enabled = (wrap_width > 0.0f);
	const char* word_wrap_eol = NULL;

	const char* s = text_begin;
	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			if (!word_wrap_eol)
			{
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - line_width);
				if (word_wrap_eol == s)
					word_wrap_eol++;
			}

			if (s >= word_wrap_eol)
			{
				if (text_size.x < line_width)
					text_size.x = line_width;
				text_size.y += line_height;
				line_width = 0.0f;
				word_wrap_eol = NULL;

				while (s < text_end)
				{
					const char c = *s;
					if (ImCharIsSpace(c)) { s++; }
					else if (c == '\n') { s++; break; }
					else { break; }
				}
				continue;
			}
		}

		const char* prev_s = s;
		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
		{
			s += 1;
		}
		else
		{
			s += ImTextCharFromUtf8(&c, s, text_end);
			if (c == 0) break;
		}

		if (c < 32)
		{
			if (c == '\n')
			{
				text_size.x = ImMax(text_size.x, line_width);
				text_size.y += line_height;
				line_width = 0.0f;
				continue;
			}
			if (c == '\r') continue;
		}

		const float char_width = ((int)c < IndexAdvanceX.Size ? IndexAdvanceX[(int)c] : FallbackAdvanceX) * scale;
		if (line_width + char_width >= max_width)
		{
			s = prev_s;
			break;
		}

		line_width += char_width;
	}

	if (text_size.x < line_width)
		text_size.x = line_width;

	if (line_width > 0 || text_size.y == 0.0f)
		text_size.y += line_height;

	if (remaining)
		*remaining = s;

	return text_size;
}

void ImFont::RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r')
		return;
	if (const ImFontGlyph* glyph = FindGlyph(c))
	{
		float scale = (size >= 0.0f) ? (size / FontSize) : 1.0f;
		pos.x = (float)(int)pos.x + DisplayOffset.x;
		pos.y = (float)(int)pos.y + DisplayOffset.y;
		draw_list->PrimReserve(6, 4);
		draw_list->PrimRectUV(ImVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale), ImVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale), ImVec2(glyph->U0, glyph->V0), ImVec2(glyph->U1, glyph->V1), col);
	}
}

void ImFont::RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width, bool cpu_fine_clip) const
{
	if (!text_end)
		text_end = text_begin + strlen(text_begin);

	pos.x = (float)(int)pos.x + DisplayOffset.x;
	pos.y = (float)(int)pos.y + DisplayOffset.y;
	float x = pos.x;
	float y = pos.y;
	if (y > clip_rect.w)
		return;

	const float scale = size / FontSize;
	const float line_height = FontSize * scale;
	const bool word_wrap_enabled = (wrap_width > 0.0f);
	const char* word_wrap_eol = NULL;

	const char* s = text_begin;
	if (!word_wrap_enabled && y + line_height < clip_rect.y)
		while (s < text_end && *s != '\n')
			s++;

	const int vtx_count_max = (int)(text_end - s) * 4;
	const int idx_count_max = (int)(text_end - s) * 6;
	const int idx_expected_size = draw_list->IdxBuffer.Size + idx_count_max;
	draw_list->PrimReserve(idx_count_max, vtx_count_max);

	ImDrawVert* vtx_write = draw_list->_VtxWritePtr;
	unsigned short* idx_write = draw_list->_IdxWritePtr;
	unsigned int vtx_current_idx = draw_list->_VtxCurrentIdx;

	while (s < text_end)
	{
		if (word_wrap_enabled)
		{
			if (!word_wrap_eol)
			{
				word_wrap_eol = CalcWordWrapPositionA(scale, s, text_end, wrap_width - (x - pos.x));
				if (word_wrap_eol == s)
					word_wrap_eol++;
			}

			if (s >= word_wrap_eol)
			{
				x = pos.x;
				y += line_height;
				word_wrap_eol = NULL;

				while (s < text_end)
				{
					const char c = *s;
					if (ImCharIsSpace(c)) { s++; }
					else if (c == '\n') { s++; break; }
					else { break; }
				}
				continue;
			}
		}

		unsigned int c = (unsigned int)*s;
		if (c < 0x80)
		{
			s += 1;
		}
		else
		{
			s += ImTextCharFromUtf8(&c, s, text_end);
			if (c == 0) break;
		}

		if (c < 32)
		{
			if (c == '\n')
			{
				x = pos.x;
				y += line_height;

				if (y > clip_rect.w)
					break;
				if (!word_wrap_enabled && y + line_height < clip_rect.y)
					while (s < text_end && *s != '\n')
						s++;
				continue;
			}
			if (c == '\r') continue;
		}

		float char_width = 0.0f;
		if (const ImFontGlyph* glyph = FindGlyph((unsigned short)c))
		{
			char_width = glyph->AdvanceX * scale;

			if (c != ' ' && c != '\t')
			{
				float x1 = x + glyph->X0 * scale;
				float x2 = x + glyph->X1 * scale;
				float y1 = y + glyph->Y0 * scale;
				float y2 = y + glyph->Y1 * scale;
				if (x1 <= clip_rect.z && x2 >= clip_rect.x)
				{
					float u1 = glyph->U0;
					float v1 = glyph->V0;
					float u2 = glyph->U1;
					float v2 = glyph->V1;

					if (cpu_fine_clip)
					{
						if (x1 < clip_rect.x)
						{
							u1 = u1 + (1.0f - (x2 - clip_rect.x) / (x2 - x1)) * (u2 - u1);
							x1 = clip_rect.x;
						}
						if (y1 < clip_rect.y)
						{
							v1 = v1 + (1.0f - (y2 - clip_rect.y) / (y2 - y1)) * (v2 - v1);
							y1 = clip_rect.y;
						}
						if (x2 > clip_rect.z)
						{
							u2 = u1 + ((clip_rect.z - x1) / (x2 - x1)) * (u2 - u1);
							x2 = clip_rect.z;
						}
						if (y2 > clip_rect.w)
						{
							v2 = v1 + ((clip_rect.w - y1) / (y2 - y1)) * (v2 - v1);
							y2 = clip_rect.w;
						}
						if (y1 >= y2)
						{
							x += char_width;
							continue;
						}
					}

					{
						idx_write[0] = (unsigned short)(vtx_current_idx); idx_write[1] = (unsigned short)(vtx_current_idx + 1); idx_write[2] = (unsigned short)(vtx_current_idx + 2);
						idx_write[3] = (unsigned short)(vtx_current_idx); idx_write[4] = (unsigned short)(vtx_current_idx + 2); idx_write[5] = (unsigned short)(vtx_current_idx + 3);
						vtx_write[0].pos.x = x1; vtx_write[0].pos.y = y1; vtx_write[0].col = col; vtx_write[0].uv.x = u1; vtx_write[0].uv.y = v1;
						vtx_write[1].pos.x = x2; vtx_write[1].pos.y = y1; vtx_write[1].col = col; vtx_write[1].uv.x = u2; vtx_write[1].uv.y = v1;
						vtx_write[2].pos.x = x2; vtx_write[2].pos.y = y2; vtx_write[2].col = col; vtx_write[2].uv.x = u2; vtx_write[2].uv.y = v2;
						vtx_write[3].pos.x = x1; vtx_write[3].pos.y = y2; vtx_write[3].col = col; vtx_write[3].uv.x = u1; vtx_write[3].uv.y = v2;
						vtx_write += 4;
						vtx_current_idx += 4;
						idx_write += 6;
					}
				}
			}
		}

		x += char_width;
	}

	draw_list->VtxBuffer.resize((int)(vtx_write - draw_list->VtxBuffer.Data));
	draw_list->IdxBuffer.resize((int)(idx_write - draw_list->IdxBuffer.Data));
	draw_list->CmdBuffer[draw_list->CmdBuffer.Size - 1].ElemCount -= (idx_expected_size - draw_list->IdxBuffer.Size);
	draw_list->_VtxWritePtr = vtx_write;
	draw_list->_IdxWritePtr = idx_write;
	draw_list->_VtxCurrentIdx = (unsigned int)draw_list->VtxBuffer.Size;
}

inline float ImAcos01(float x)
{
	if (x <= 0.0f) return IM_PI * 0.5f;
	if (x >= 1.0f) return 0.0f;
	return acosf(x);
}
