
#include "XorString.h"

#include "imgui.h"
#include "imgui_internal.h"

#include <stdlib.h>
#include <stdio.h>

bool IsKeyPressedMap(ImGuiKey key, bool repeat = true);
ImFont* GetDefaultFont();
void SetCurrentFont(ImFont* font);
void SetCurrentWindow(ImGuiWindow* window);
void SetWindowScrollY(ImGuiWindow* window, float new_scroll_y);
void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond);
void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond);
void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond);
ImGuiWindow* FindHoveredWindow(ImVec2 pos);
ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags);
void CheckStacksSize(ImGuiWindow* window, bool write);
ImVec2 CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window);
void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list);
void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window);
void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window);
ImRect GetVisibleRect();
void CloseInactivePopups(ImGuiWindow* ref_window);
void ClosePopupToLevel(int remaining);
ImGuiWindow* GetFrontMostModalRootWindow();
int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end);
ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining = NULL, ImVec2* out_offset = NULL, bool stop_on_new_line = false);
inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size);
inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size);
void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2);
bool DataTypeApplyOpFromText(const char * buf, void * data_ptr, const char * scalar_format);

namespace ImGui
{
	void FocusFrontMostActiveWindow(ImGuiWindow* ignore_window);
};

ImFontAtlas GImDefaultFontAtlas;
ImGuiContext GImDefaultContext;
ImGuiContext * GImGui = &GImDefaultContext;

ImGuiStyle::ImGuiStyle()
{
	Alpha = 1.0f;
	WindowPadding = ImVec2(8, 8);
	WindowRounding = 7.0f;
	WindowBorderSize = 0.0f;
	WindowMinSize = ImVec2(32, 32);
	WindowTitleAlign = ImVec2(0.0f, 0.5f);
	ChildRounding = 0.0f;
	ChildBorderSize = 1.0f;
	PopupRounding = 0.0f;
	PopupBorderSize = 1.0f;
	FramePadding = ImVec2(4, 3);
	FrameRounding = 0.0f;
	FrameBorderSize = 0.0f;
	ItemSpacing = ImVec2(8, 4);
	ItemInnerSpacing = ImVec2(4, 4);
	TouchExtraPadding = ImVec2(0, 0);
	IndentSpacing = 21.0f;
	ColumnsMinSpacing = 6.0f;
	ScrollbarSize = 16.0f;
	ScrollbarRounding = 9.0f;
	GrabMinSize = 10.0f;
	GrabRounding = 0.0f;
	ButtonTextAlign = ImVec2(0.5f, 0.5f);
	DisplayWindowPadding = ImVec2(22, 22);
	DisplaySafeAreaPadding = ImVec2(4, 4);
	AntiAliasedLines = true;
	AntiAliasedFill = true;
	CurveTessellationTol = 1.25f;
};

void ImGuiStyle::ScaleAllSizes(float scale_factor)
{
	WindowPadding = ImFloor(WindowPadding * scale_factor);
	WindowRounding = ImFloor(WindowRounding * scale_factor);
	WindowMinSize = ImFloor(WindowMinSize * scale_factor);
	ChildRounding = ImFloor(ChildRounding * scale_factor);
	PopupRounding = ImFloor(PopupRounding * scale_factor);
	FramePadding = ImFloor(FramePadding * scale_factor);
	FrameRounding = ImFloor(FrameRounding * scale_factor);
	ItemSpacing = ImFloor(ItemSpacing * scale_factor);
	ItemInnerSpacing = ImFloor(ItemInnerSpacing * scale_factor);
	TouchExtraPadding = ImFloor(TouchExtraPadding * scale_factor);
	IndentSpacing = ImFloor(IndentSpacing * scale_factor);
	ColumnsMinSpacing = ImFloor(ColumnsMinSpacing * scale_factor);
	ScrollbarSize = ImFloor(ScrollbarSize * scale_factor);
	ScrollbarRounding = ImFloor(ScrollbarRounding * scale_factor);
	GrabMinSize = ImFloor(GrabMinSize * scale_factor);
	GrabRounding = ImFloor(GrabRounding * scale_factor);
	DisplayWindowPadding = ImFloor(DisplayWindowPadding * scale_factor);
	DisplaySafeAreaPadding = ImFloor(DisplaySafeAreaPadding * scale_factor);
};

ImGuiIO::ImGuiIO()
{
	memset(this, 0, sizeof(*this));
	DisplaySize = ImVec2(-1.0f, -1.0f);
	DeltaTime = 1.0f / 60.0f;
	MouseDoubleClickTime = 0.30f;
	MouseDoubleClickMaxDist = 6.0f;
	for (int i = 0; i < ImGuiKey_COUNT; i++) KeyMap[i] = -1;
	KeyRepeatDelay = 0.250f;
	KeyRepeatRate = 0.050f;
	UserData = NULL;
	Fonts = &GImDefaultFontAtlas;
	FontGlobalScale = 1.0f;
	FontDefault = NULL;
	FontAllowUserScaling = false;
	DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	DisplayVisibleMin = DisplayVisibleMax = ImVec2(0.0f, 0.0f);
	OptMacOSXBehaviors = false;
	OptCursorBlink = true;
	RenderDrawListsFn = NULL;
	ImeWindowHandle = NULL;
	MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	MousePosPrev = ImVec2(-FLT_MAX, -FLT_MAX);
	MouseDragThreshold = 6.f;
	for (int i = 0; i < IM_ARRAYSIZE(MouseDownDuration); i++) MouseDownDuration[i] = MouseDownDurationPrev[i] = -1.f;
	for (int i = 0; i < IM_ARRAYSIZE(KeysDownDuration); i++) KeysDownDuration[i] = KeysDownDurationPrev[i] = -1.f;
};

void ImGuiIO::AddInputCharacter(ImWchar c)
{
	const int n = ImStrlenW(InputCharacters);
	if (n + 1 < IM_ARRAYSIZE(InputCharacters)) InputCharacters[n] = c, InputCharacters[n + 1] = '\0';
};

#define IM_F32_TO_INT8_UNBOUND(_VAL) ((int) ((_VAL) * 255.f + ((_VAL) >= 0 ? .5f : -0.5f)))
#define IM_F32_TO_INT8_SAT(_VAL) ((int) (ImSaturate(_VAL) * 255.f + .5f))

ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p)
{
	ImVec2 ap = p - a;
	ImVec2 ab_dir = b - a;
	float ab_len = sqrtf(ab_dir.x * ab_dir.x + ab_dir.y * ab_dir.y);
	ab_dir *= 1.f / ab_len;
	float dot = ap.x * ab_dir.x + ap.y * ab_dir.y;
	if (dot < 0.f) return a;
	if (dot > ab_len) return b;
	return a + ab_dir * dot;
}

bool ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
	bool b1 = ((p.x - b.x) * (a.y - b.y) - (p.y - b.y) * (a.x - b.x)) < 0.f;
	bool b2 = ((p.x - c.x) * (b.y - c.y) - (p.y - c.y) * (b.x - c.x)) < 0.f;
	bool b3 = ((p.x - a.x) * (c.y - a.y) - (p.y - a.y) * (c.x - a.x)) < 0.f;
	return ((b1 == b2) && (b2 == b3));
}

void ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w)
{
	ImVec2 v0 = b - a;
	ImVec2 v1 = c - a;
	ImVec2 v2 = p - a;
	const float denom = v0.x * v1.y - v1.x * v0.y;
	out_v = (v2.x * v1.y - v1.x * v2.y) / denom;
	out_w = (v0.x * v2.y - v2.x * v0.y) / denom;
	out_u = 1.f - out_v - out_w;
}

ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p)
{
	ImVec2 proj_ab = ImLineClosestPoint(a, b, p);
	ImVec2 proj_bc = ImLineClosestPoint(b, c, p);
	ImVec2 proj_ca = ImLineClosestPoint(c, a, p);
	float dist2_ab = ImLengthSqr(p - proj_ab);
	float dist2_bc = ImLengthSqr(p - proj_bc);
	float dist2_ca = ImLengthSqr(p - proj_ca);
	float m = ImMin(dist2_ab, ImMin(dist2_bc, dist2_ca));
	if (m == dist2_ab) return proj_ab;
	if (m == dist2_bc) return proj_bc;
	return proj_ca;
}

int ImStricmp(const char* str1, const char* str2)
{
	int d;
	while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; }
	return d;
}

int ImStrnicmp(const char* str1, const char* str2, size_t count)
{
	int d = 0;
	while (count > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; count--; }
	return d;
}

void ImStrncpy(char* dst, const char* src, size_t count)
{
	if (count < 1) return;
	strncpy(dst, src, count);
	dst[count - 1] = 0;
}

char* ImStrdup(const char *str)
{
	size_t len = strlen(str) + 1;
	void * buf = malloc(len);
	return (char *)memcpy(buf, (const void *)str, len);
}

int ImStrlenW(const ImWchar* str)
{
	int n = 0;
	while (*str++) n++;
	return n;
}

const ImWchar* ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin)
{
	while (buf_mid_line > buf_begin && buf_mid_line[-1] != '\n') buf_mid_line--;
	return buf_mid_line;
}

const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end)
{
	if (!needle_end)
		needle_end = needle + strlen(needle);

	const char un0 = (char)toupper(*needle);
	while ((!haystack_end && *haystack) || (haystack_end && haystack < haystack_end))
	{
		if (toupper(*haystack) == un0)
		{
			const char* b = needle + 1;
			for (const char* a = haystack + 1; b < needle_end; a++, b++)
				if (toupper(*a) != toupper(*b))
					break;
			if (b == needle_end)
				return haystack;
		}
		haystack++;
	}
	return NULL;
}

const char* ImAtoi(const char* src, int* output)
{
	int negative = 0;
	if (*src == '-') { negative = 1; src++; }
	if (*src == '+') { src++; }
	int v = 0;
	while (*src >= '0' && *src <= '9') v = (v * 10) + (*src++ - '0');
	*output = negative ? -v : v;
	return src;
}

int ImFormatString(char* buf, size_t buf_size, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int w = vsnprintf(buf, buf_size, fmt, args);
	va_end(args);
	if (buf == NULL) return w;
	if (w == -1 || w >= (int)buf_size) w = (int)buf_size - 1;
	buf[w] = 0;
	return w;
};

int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args)
{
	int w = vsnprintf(buf, buf_size, fmt, args);
	if (buf == NULL) return w;
	if (w == -1 || w >= (int)buf_size) w = (int)buf_size - 1;
	buf[w] = 0;
	return w;
};

ImU32 ImHash(const void* data, int data_size, ImU32 seed)
{
	static ImU32 crc32_lut[XS_RANDOM_SIZE * 2] = { 0 };
	if (!crc32_lut[1])
	{
		const ImU32 polynomial = 0xEDB88320;
		for (ImU32 i = 0; i < 256; i++)
		{
			ImU32 crc = i;
			for (ImU32 j = 0; j < 8; j++) crc = (crc >> 1) ^ (ImU32(-int(crc & 1)) & polynomial);
			crc32_lut[i] = crc;
		}
	}

	seed = ~seed;
	ImU32 crc = seed;
	const unsigned char * current = (const unsigned char *)data;

	if (data_size > 0)
	{
		while (data_size--)
			crc = (crc >> 8) ^ crc32_lut[(crc & 0x000000FF) ^ *current++];
	}
	else
	{
		while (unsigned char c = *current++)
		{
			if (c == '#' && current[0] == '#' && current[1] == '#')
				crc = seed;
			crc = (crc >> 8) ^ crc32_lut[(crc & 0x000000FF) ^ c];
		}
	}
	return ~crc;
}

int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end)
{
	unsigned int c = (unsigned int)-1;
	const unsigned char* str = (const unsigned char*)in_text;
	if (!(*str & 0x00000080))
	{
		c = (unsigned int)(*str++);
		*out_char = c;
		return 1;
	}
	if ((*str & 0x000000e0) == 0x000000c0)
	{
		*out_char = 0x0000FFFD;
		if (in_text_end && in_text_end - (const char*)str < 2) return 1;
		if (*str < 0x000000c2) return 2;
		c = (unsigned int)((*str++ & 0x0000001f) << 6);
		if ((*str & 0x000000c0) != 0x00000080) return 2;
		c += (*str++ & 0x0000003f);
		*out_char = c;
		return 2;
	}
	if ((*str & 0xf0) == 0xe0)
	{
		*out_char = 0xFFFD;
		if (in_text_end && in_text_end - (const char*)str < 3) return 1;
		if (*str == 0xe0 && (str[1] < 0xa0 || str[1] > 0xbf)) return 3;
		if (*str == 0xed && str[1] > 0x9f) return 3;
		c = (unsigned int)((*str++ & 0x0f) << 12);
		if ((*str & 0xc0) != 0x80) return 3;
		c += (unsigned int)((*str++ & 0x3f) << 6);
		if ((*str & 0xc0) != 0x80) return 3;
		c += (*str++ & 0x3f);
		*out_char = c;
		return 3;
	}
	if ((*str & 0xf8) == 0xf0)
	{
		*out_char = 0xFFFD;
		if (in_text_end && in_text_end - (const char*)str < 4) return 1;
		if (*str > 0xf4) return 4;
		if (*str == 0xf0 && (str[1] < 0x90 || str[1] > 0xbf)) return 4;
		if (*str == 0xf4 && str[1] > 0x8f) return 4;
		c = (unsigned int)((*str++ & 0x07) << 18);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (unsigned int)((*str++ & 0x3f) << 12);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (unsigned int)((*str++ & 0x3f) << 6);
		if ((*str & 0xc0) != 0x80) return 4;
		c += (*str++ & 0x3f);
		if ((c & 0xFFFFF800) == 0xD800) return 4;
		*out_char = c;
		return 4;
	}
	*out_char = 0;
	return 0;
}

int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_text_remaining)
{
	ImWchar* buf_out = buf;
	ImWchar* buf_end = buf + buf_size;
	while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c;
		in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
		if (c == 0)
			break;
		if (c < 0x10000)
			*buf_out++ = (ImWchar)c;
	}
	*buf_out = 0;
	if (in_text_remaining)
		*in_text_remaining = in_text;
	return (int)(buf_out - buf);
}

int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end)
{
	int char_count = 0;
	while ((!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c;
		in_text += ImTextCharFromUtf8(&c, in_text, in_text_end);
		if (c == 0)
			break;
		if (c < 0x10000)
			char_count++;
	}
	return char_count;
}

inline int ImTextCharToUtf8(char* buf, int buf_size, unsigned int c)
{
	if (c < 0x80)
	{
		buf[0] = (char)c;
		return 1;
	}
	if (c < 0x800)
	{
		if (buf_size < 2) return 0;
		buf[0] = (char)(0xc0 + (c >> 6));
		buf[1] = (char)(0x80 + (c & 0x3f));
		return 2;
	}
	if (c >= 0xdc00 && c < 0xe000)
	{
		return 0;
	}
	if (c >= 0xd800 && c < 0xdc00)
	{
		if (buf_size < 4) return 0;
		buf[0] = (char)(0xf0 + (c >> 18));
		buf[1] = (char)(0x80 + ((c >> 12) & 0x3f));
		buf[2] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[3] = (char)(0x80 + ((c) & 0x3f));
		return 4;
	}
	{
		if (buf_size < 3) return 0;
		buf[0] = (char)(0xe0 + (c >> 12));
		buf[1] = (char)(0x80 + ((c >> 6) & 0x3f));
		buf[2] = (char)(0x80 + ((c) & 0x3f));
		return 3;
	}
}

inline int ImTextCountUtf8BytesFromChar(unsigned int c)
{
	if (c < 0x80) return 1;
	if (c < 0x800) return 2;
	if (c >= 0xdc00 && c < 0xe000) return 0;
	if (c >= 0xd800 && c < 0xdc00) return 4;
	return 3;
}

int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end)
{
	char* buf_out = buf;
	const char* buf_end = buf + buf_size;
	while (buf_out < buf_end - 1 && (!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c = (unsigned int)(*in_text++);
		if (c < 0x80)
			*buf_out++ = (char)c;
		else
			buf_out += ImTextCharToUtf8(buf_out, (int)(buf_end - buf_out - 1), c);
	}
	*buf_out = 0;
	return (int)(buf_out - buf);
}

int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end)
{
	int bytes_count = 0;
	while ((!in_text_end || in_text < in_text_end) && *in_text)
	{
		unsigned int c = (unsigned int)(*in_text++);
		if (c < 0x80)
			bytes_count++;
		else
			bytes_count += ImTextCountUtf8BytesFromChar(c);
	}
	return bytes_count;
}

ImVec4 ImGui::ColorConvertU32ToFloat4(ImU32 in)
{
	float s = 1.0f / 255.0f;
	return ImVec4(
		((in >> IM_COL32_R_SHIFT) & 0xFF) * s,
		((in >> IM_COL32_G_SHIFT) & 0xFF) * s,
		((in >> IM_COL32_B_SHIFT) & 0xFF) * s,
		((in >> IM_COL32_A_SHIFT) & 0xFF) * s);
}

ImU32 ImGui::ColorConvertFloat4ToU32(const ImVec4& in)
{
	ImU32 out;
	out = ((ImU32)IM_F32_TO_INT8_SAT(in.x)) << IM_COL32_R_SHIFT;
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.y)) << IM_COL32_G_SHIFT;
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.z)) << IM_COL32_B_SHIFT;
	out |= ((ImU32)IM_F32_TO_INT8_SAT(in.w)) << IM_COL32_A_SHIFT;
	return out;
}

ImU32 ImGui::GetColorU32(ImGuiCol idx, float alpha_mul)
{
	ImGuiStyle& style = GImGui->Style;
	ImVec4 c = style.Colors[idx];
	c.w *= style.Alpha * alpha_mul;
	return ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(const ImVec4& col)
{
	ImGuiStyle& style = GImGui->Style;
	ImVec4 c = col;
	c.w *= style.Alpha;
	return ColorConvertFloat4ToU32(c);
}

ImU32 ImGui::GetColorU32(ImU32 col)
{
	float style_alpha = GImGui->Style.Alpha;
	if (style_alpha >= 1.0f)
		return col;
	int a = (col & IM_COL32_A_MASK) >> IM_COL32_A_SHIFT;
	a = (int)(a * style_alpha);
	return (col & ~IM_COL32_A_MASK) | (a << IM_COL32_A_SHIFT);
}

void ImGui::ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
	float K = 0.f;
	if (g < b)
	{
		ImSwap(g, b);
		K = -1.f;
	}
	if (r < g)
	{
		ImSwap(r, g);
		K = -2.f / 6.f - K;
	}

	const float chroma = r - (g < b ? g : b);
	out_h = fabsf(K + (g - b) / (6.f * chroma + 1e-20f));
	out_s = chroma / (r + 1e-20f);
	out_v = r;
}

void ImGui::ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
	if (s == 0.0f)
	{
		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i)
	{
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	};
};

void* ImFileLoadToMemory(const char* filename, const char* file_open_mode, int* out_file_size, int padding_bytes)
{
	if (out_file_size) *out_file_size = 0;

	FILE * f;
	if ((f = fopen(filename, file_open_mode)) == NULL)
		return NULL;

	long file_size_signed;
	if (fseek(f, 0, SEEK_END) || (file_size_signed = ftell(f)) == -1 || fseek(f, 0, SEEK_SET))
	{
		fclose(f);
		return NULL;
	}

	int file_size = (int)file_size_signed;
	void * file_data = malloc(file_size + padding_bytes);
	if (file_data == NULL)
	{
		fclose(f);
		return NULL;
	}
	if (fread(file_data, 1, (size_t)file_size, f) != (size_t)file_size)
	{
		fclose(f);
		free(file_data);
		return NULL;
	}
	if (padding_bytes > 0)
		memset((void *)(((char*)file_data) + file_size), 0, padding_bytes);

	fclose(f);
	if (out_file_size)
		*out_file_size = file_size;

	return file_data;
}

void SetCursorPosYAndSetupDummyPrevLine(float pos_y, float line_height)
{
	ImGui::SetCursorPosY(pos_y);
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	window->DC.CursorPosPrevLine.y = window->DC.CursorPos.y - line_height;
	window->DC.PrevLineHeight = (line_height - GImGui->Style.ItemSpacing.y);
	if (window->DC.ColumnsSet)
		window->DC.ColumnsSet->CellMinY = window->DC.CursorPos.y;
}

void ImGuiListClipper::Begin(int count, float items_height)
{
	StartPosY = ImGui::GetCursorPosY();
	ItemsHeight = items_height;
	ItemsCount = count;
	StepNo = 0;
	DisplayEnd = DisplayStart = -1;
	if (ItemsHeight > 0.0f)
	{
		ImGui::CalcListClipping(ItemsCount, ItemsHeight, &DisplayStart, &DisplayEnd);
		if (DisplayStart > 0)
			SetCursorPosYAndSetupDummyPrevLine(StartPosY + DisplayStart * ItemsHeight, ItemsHeight);
		StepNo = 2;
	}
}

void ImGuiListClipper::End()
{
	if (ItemsCount < 0)
		return;
	if (ItemsCount < INT_MAX)
		SetCursorPosYAndSetupDummyPrevLine(StartPosY + ItemsCount * ItemsHeight, ItemsHeight);
	ItemsCount = -1;
	StepNo = 3;
}

bool ImGuiListClipper::Step()
{
	if (ItemsCount == 0 || ImGui::GetCurrentWindowRead()->SkipItems)
	{
		ItemsCount = -1;
		return false;
	}
	if (StepNo == 0)
	{
		DisplayStart = 0;
		DisplayEnd = 1;
		StartPosY = ImGui::GetCursorPosY();
		StepNo = 1;
		return true;
	}
	if (StepNo == 1)
	{
		if (ItemsCount == 1) { ItemsCount = -1; return false; }
		float items_height = ImGui::GetCursorPosY() - StartPosY;
		Begin(ItemsCount - 1, items_height);
		DisplayStart++;
		DisplayEnd++;
		StepNo = 3;
		return true;
	}
	if (StepNo == 2)
	{
		StepNo = 3;
		return true;
	}
	if (StepNo == 3)
		End();
	return false;
}

ImGuiWindow::ImGuiWindow(ImGuiContext* context, const char* name)
{
	Name = ImStrdup(name);
	ID = ImHash(name, 0);
	IDStack.push_back(ID);
	Flags = 0;
	PosFloat = Pos = ImVec2(0.0f, 0.0f);
	Size = SizeFull = ImVec2(0.0f, 0.0f);
	SizeContents = SizeContentsExplicit = ImVec2(0.0f, 0.0f);
	WindowPadding = ImVec2(0.0f, 0.0f);
	WindowRounding = 0.0f;
	WindowBorderSize = 0.0f;
	MoveId = GetID(XorString("#MOVE"));
	Scroll = ImVec2(0.0f, 0.0f);
	ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
	ScrollTargetCenterRatio = ImVec2(0.5f, 0.5f);
	ScrollbarX = ScrollbarY = false;
	ScrollbarSizes = ImVec2(0.0f, 0.0f);
	Active = WasActive = false;
	WriteAccessed = false;
	Collapsed = false;
	SkipItems = false;
	Appearing = false;
	BeginOrderWithinParent = -1;
	BeginOrderWithinContext = -1;
	BeginCount = 0;
	PopupId = 0;
	AutoFitFramesX = AutoFitFramesY = -1;
	AutoFitOnlyGrows = false;
	AutoFitChildAxises = 0x00;
	AutoPosLastDirection = ImGuiDir_None;
	HiddenFrames = 0;
	SetWindowPosAllowFlags = SetWindowSizeAllowFlags = SetWindowCollapsedAllowFlags = ImGuiCond_Always | ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing;
	SetWindowPosVal = SetWindowPosPivot = ImVec2(FLT_MAX, FLT_MAX);
	LastFrameActive = -1;
	ItemWidthDefault = 0.0f;
	FontWindowScale = 1.0f;
	DrawList = new ImDrawList(&context->DrawListSharedData);
	ParentWindow = NULL;
	RootWindow = NULL;
	RootNonPopupWindow = NULL;
	FocusIdxAllCounter = FocusIdxTabCounter = -1;
	FocusIdxAllRequestCurrent = FocusIdxTabRequestCurrent = INT_MAX;
	FocusIdxAllRequestNext = FocusIdxTabRequestNext = INT_MAX;
}

ImGuiWindow::~ImGuiWindow()
{
	if (DrawList)
		delete DrawList, DrawList = NULL;

	if (Name)
	{
		free(Name);
		Name = NULL;
	};

	for (int i = 0; i != ColumnsStorage.Size; i++)
		ColumnsStorage[i].~ImGuiColumnsSet();
}

ImGuiID ImGuiWindow::GetID(const char* str, const char* str_end)
{
	ImGuiID seed = IDStack.back();
	ImGuiID id = ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
	ImGui::KeepAliveID(id);
	return id;
}

ImGuiID ImGuiWindow::GetID(const void* ptr)
{
	ImGuiID seed = IDStack.back();
	ImGuiID id = ImHash(&ptr, sizeof(void*), seed);
	ImGui::KeepAliveID(id);
	return id;
}

ImGuiID ImGuiWindow::GetIDNoKeepAlive(const char* str, const char* str_end)
{
	ImGuiID seed = IDStack.back();
	return ImHash(str, str_end ? (int)(str_end - str) : 0, seed);
}

ImGuiID ImGuiWindow::GetIDFromRectangle(const ImRect& r_abs)
{
	ImGuiID seed = IDStack.back();
	const int r_rel[4] = { (int)(r_abs.Min.x - Pos.x), (int)(r_abs.Min.y - Pos.y), (int)(r_abs.Max.x - Pos.x), (int)(r_abs.Max.y - Pos.y) };
	ImGuiID id = ImHash(&r_rel, sizeof(r_rel), seed);
	ImGui::KeepAliveID(id);
	return id;
}

void SetCurrentWindow(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	g.CurrentWindow = window;
	if (window)
		g.FontSize = g.DrawListSharedData.FontSize = window->CalcFontSize();
}

void ImGui::SetActiveID(ImGuiID id, ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	g.ActiveIdIsJustActivated = (g.ActiveId != id);
	if (g.ActiveIdIsJustActivated)
		g.ActiveIdTimer = 0.0f;
	g.ActiveId = id;
	g.ActiveIdAllowOverlap = false;
	g.ActiveIdIsAlive |= (id != 0);
	g.ActiveIdWindow = window;
}

void ImGui::ClearActiveID()
{
	SetActiveID(0, NULL);
}

void ImGui::SetHoveredID(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	g.HoveredId = id;
	g.HoveredIdAllowOverlap = false;
	g.HoveredIdTimer = (id != 0 && g.HoveredIdPreviousFrame == id) ? (g.HoveredIdTimer + g.IO.DeltaTime) : 0.0f;
}

void ImGui::KeepAliveID(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	if (g.ActiveId == id)
		g.ActiveIdIsAlive = true;
}

inline bool IsWindowContentHoverable(ImGuiWindow* window, ImGuiHoveredFlags flags)
{
	ImGuiContext& g = *GImGui;
	if (g.NavWindow)
		if (ImGuiWindow* focused_root_window = g.NavWindow->RootWindow)
			if (focused_root_window->WasActive && focused_root_window != window->RootWindow)
			{
				if (focused_root_window->Flags & ImGuiWindowFlags_Modal)
					return false;
				if ((focused_root_window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiHoveredFlags_AllowWhenBlockedByPopup))
					return false;
			}

	return true;
}

void ImGui::ItemSize(const ImVec2& size, float text_offset_y)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (window->SkipItems) return;
	const float line_height = ImMax(window->DC.CurrentLineHeight, size.y);
	const float text_base_offset = ImMax(window->DC.CurrentLineTextBaseOffset, text_offset_y);
	window->DC.CursorPosPrevLine = ImVec2(window->DC.CursorPos.x + size.x, window->DC.CursorPos.y);
	window->DC.CursorPos = ImVec2((float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX), (float)(int)(window->DC.CursorPos.y + line_height + g.Style.ItemSpacing.y));
	window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPosPrevLine.x);
	window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y - g.Style.ItemSpacing.y);
	window->DC.PrevLineHeight = line_height;
	window->DC.PrevLineTextBaseOffset = text_base_offset;
	window->DC.CurrentLineHeight = window->DC.CurrentLineTextBaseOffset = 0.0f;
	if (window->DC.LayoutType == ImGuiLayoutType_Horizontal) SameLine();
}

void ImGui::ItemSize(const ImRect& bb, float text_offset_y)
{
	ItemSize(bb.GetSize(), text_offset_y);
}

bool ImGui::ItemAdd(const ImRect& bb, ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const bool is_clipped = IsClippedEx(bb, id, false);
	window->DC.LastItemId = id;
	window->DC.LastItemRect = bb;
	window->DC.LastItemRectHoveredRect = false;
	if (is_clipped) return false;
	window->DC.LastItemRectHoveredRect = IsMouseHoveringRect(bb.Min, bb.Max);
	return true;
}

bool ImGui::IsItemHovered(ImGuiHoveredFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if (!window->DC.LastItemRectHoveredRect)
		return false;

	if (g.HoveredRootWindow != window->RootWindow && !(flags & ImGuiHoveredFlags_AllowWhenOverlapped))
		return false;

	if (!(flags & ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		if (g.ActiveId != 0 && g.ActiveId != window->DC.LastItemId && !g.ActiveIdAllowOverlap && g.ActiveId != window->MoveId)
			return false;

	if (!IsWindowContentHoverable(window, flags))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_Disabled)
		return false;

	if (window->DC.LastItemId == window->MoveId && window->WriteAccessed)
		return false;
	return true;
}

bool ImGui::ItemHoverable(const ImRect& bb, ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	if (g.HoveredId != 0 && g.HoveredId != id && !g.HoveredIdAllowOverlap)
		return false;

	ImGuiWindow* window = g.CurrentWindow;
	if (g.HoveredWindow != window)
		return false;
	if (g.ActiveId != 0 && g.ActiveId != id && !g.ActiveIdAllowOverlap)
		return false;
	if (!IsMouseHoveringRect(bb.Min, bb.Max))
		return false;
	if (!IsWindowContentHoverable(window, ImGuiHoveredFlags_Default))
		return false;
	if (window->DC.ItemFlags & ImGuiItemFlags_Disabled)
		return false;

	SetHoveredID(id);
	return true;
}

bool ImGui::IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (!bb.Overlaps(window->ClipRect))
		if (id == 0 || id != g.ActiveId)
			if (clip_even_when_logged)
				return true;
	return false;
}

bool ImGui::FocusableItemRegister(ImGuiWindow* window, ImGuiID id, bool tab_stop)
{
	ImGuiContext& g = *GImGui;

	const bool allow_keyboard_focus = (window->DC.ItemFlags & (ImGuiItemFlags_AllowKeyboardFocus | ImGuiItemFlags_Disabled)) == ImGuiItemFlags_AllowKeyboardFocus;
	window->FocusIdxAllCounter++;
	if (allow_keyboard_focus)
		window->FocusIdxTabCounter++;

	if (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent)
		return true;

	if (allow_keyboard_focus)
		if (window->FocusIdxTabCounter == window->FocusIdxTabRequestCurrent)
			return true;

	return false;
}

void ImGui::FocusableItemUnregister(ImGuiWindow* window)
{
	window->FocusIdxAllCounter--;
	window->FocusIdxTabCounter--;
}

ImVec2 ImGui::CalcItemSize(ImVec2 size, float default_x, float default_y)
{
	ImGuiContext& g = *GImGui;
	ImVec2 content_max;
	if (size.x < 0.0f || size.y < 0.0f)
		content_max = g.CurrentWindow->Pos + GetContentRegionMax();
	if (size.x <= 0.0f)
		size.x = (size.x == 0.0f) ? default_x : ImMax(content_max.x - g.CurrentWindow->DC.CursorPos.x, 4.0f) + size.x;
	if (size.y <= 0.0f)
		size.y = (size.y == 0.0f) ? default_y : ImMax(content_max.y - g.CurrentWindow->DC.CursorPos.y, 4.0f) + size.y;
	return size;
}

float ImGui::CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x)
{
	if (wrap_pos_x < 0.0f)
		return 0.0f;

	ImGuiWindow* window = GetCurrentWindowRead();
	if (wrap_pos_x == 0.0f)
		wrap_pos_x = GetContentRegionMax().x + window->Pos.x;
	else if (wrap_pos_x > 0.0f)
		wrap_pos_x += window->Pos.x - window->Scroll.x;

	return ImMax(wrap_pos_x - pos.x, 1.0f);
}

ImGuiContext* ImGui::GetCurrentContext()
{
	return GImGui;
}

void ImGui::SetCurrentContext(ImGuiContext* ctx)
{
	GImGui = ctx;
}

ImGuiContext* ImGui::CreateContext()
{
	ImGuiContext * ctx = new ImGuiContext;
	return ctx;
};

void ImGui::DestroyContext(ImGuiContext* ctx)
{
	delete ctx;
	if (GImGui == ctx) SetCurrentContext(NULL);
};

ImGuiIO& ImGui::GetIO()
{
	return GImGui->IO;
}

ImGuiStyle& ImGui::GetStyle()
{
	return GImGui->Style;
}

ImDrawData* ImGui::GetDrawData()
{
	return GImGui->RenderDrawData.Valid ? &GImGui->RenderDrawData : NULL;
}

float ImGui::GetTime()
{
	return GImGui->Time;
}

int ImGui::GetFrameCount()
{
	return GImGui->FrameCount;
}

ImDrawList* ImGui::GetOverlayDrawList()
{
	return &GImGui->OverlayDrawList;
}

ImDrawListSharedData* ImGui::GetDrawListSharedData()
{
	return &GImGui->DrawListSharedData;
}

void ImGui::NewFrame()
{
	ImGuiContext& g = *GImGui;
	if (!g.Initialized) Initialize();

	g.Time += g.IO.DeltaTime;
	g.FrameCount += 1;
	g.WindowsActiveCount = 0;

	SetCurrentFont(GetDefaultFont());
	g.DrawListSharedData.ClipRectFullscreen = ImVec4(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
	g.DrawListSharedData.CurveTessellationTol = g.Style.CurveTessellationTol;

	g.OverlayDrawList.Clear();
	g.OverlayDrawList.PushTextureID(g.IO.Fonts->TexID);
	g.OverlayDrawList.PushClipRectFullScreen();
	g.OverlayDrawList.Flags = (g.Style.AntiAliasedLines ? ImDrawListFlags_AntiAliasedLines : 0) | (g.Style.AntiAliasedFill ? ImDrawListFlags_AntiAliasedFill : 0);
	g.RenderDrawData.Valid = false;
	g.RenderDrawData.CmdLists = NULL;
	g.RenderDrawData.CmdListsCount = g.RenderDrawData.TotalVtxCount = g.RenderDrawData.TotalIdxCount = 0;

	if (!g.HoveredIdPreviousFrame)
		g.HoveredIdTimer = 0.0f;
	g.HoveredIdPreviousFrame = g.HoveredId;
	g.HoveredId = 0;
	g.HoveredIdAllowOverlap = false;
	if (!g.ActiveIdIsAlive && g.ActiveIdPreviousFrame == g.ActiveId && g.ActiveId != 0)
		ClearActiveID();
	if (g.ActiveId)
		g.ActiveIdTimer += g.IO.DeltaTime;
	g.ActiveIdPreviousFrame = g.ActiveId;
	g.ActiveIdIsAlive = false;
	g.ActiveIdIsJustActivated = false;
	if (g.ScalarAsInputTextId && g.ActiveId != g.ScalarAsInputTextId)
		g.ScalarAsInputTextId = 0;

	memcpy(g.IO.KeysDownDurationPrev, g.IO.KeysDownDuration, sizeof(g.IO.KeysDownDuration));
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.KeysDown); i++)
		g.IO.KeysDownDuration[i] = g.IO.KeysDown[i] ? (g.IO.KeysDownDuration[i] < 0.0f ? 0.0f : g.IO.KeysDownDuration[i] + g.IO.DeltaTime) : -1.0f;

	if (IsMousePosValid(&g.IO.MousePos) && IsMousePosValid(&g.IO.MousePosPrev))
		g.IO.MouseDelta = g.IO.MousePos - g.IO.MousePosPrev;
	else
		g.IO.MouseDelta = ImVec2(0.0f, 0.0f);
	g.IO.MousePosPrev = g.IO.MousePos;
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
	{
		g.IO.MouseClicked[i] = g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] < 0.0f;
		g.IO.MouseReleased[i] = !g.IO.MouseDown[i] && g.IO.MouseDownDuration[i] >= 0.0f;
		g.IO.MouseDownDurationPrev[i] = g.IO.MouseDownDuration[i];
		g.IO.MouseDownDuration[i] = g.IO.MouseDown[i] ? (g.IO.MouseDownDuration[i] < 0.0f ? 0.0f : g.IO.MouseDownDuration[i] + g.IO.DeltaTime) : -1.0f;
		g.IO.MouseDoubleClicked[i] = false;
		if (g.IO.MouseClicked[i])
		{
			if (g.Time - g.IO.MouseClickedTime[i] < g.IO.MouseDoubleClickTime)
			{
				if (ImLengthSqr(g.IO.MousePos - g.IO.MouseClickedPos[i]) < g.IO.MouseDoubleClickMaxDist * g.IO.MouseDoubleClickMaxDist)
					g.IO.MouseDoubleClicked[i] = true;
				g.IO.MouseClickedTime[i] = -FLT_MAX;
			}
			else
			{
				g.IO.MouseClickedTime[i] = g.Time;
			}
			g.IO.MouseClickedPos[i] = g.IO.MousePos;
			g.IO.MouseDragMaxDistanceAbs[i] = ImVec2(0.0f, 0.0f);
			g.IO.MouseDragMaxDistanceSqr[i] = 0.0f;
		}
		else if (g.IO.MouseDown[i])
		{
			ImVec2 mouse_delta = g.IO.MousePos - g.IO.MouseClickedPos[i];
			g.IO.MouseDragMaxDistanceAbs[i].x = ImMax(g.IO.MouseDragMaxDistanceAbs[i].x, mouse_delta.x < 0.0f ? -mouse_delta.x : mouse_delta.x);
			g.IO.MouseDragMaxDistanceAbs[i].y = ImMax(g.IO.MouseDragMaxDistanceAbs[i].y, mouse_delta.y < 0.0f ? -mouse_delta.y : mouse_delta.y);
			g.IO.MouseDragMaxDistanceSqr[i] = ImMax(g.IO.MouseDragMaxDistanceSqr[i], ImLengthSqr(mouse_delta));
		}
	}

	if (g.MovingWindowMoveId && g.MovingWindowMoveId == g.ActiveId)
	{
		KeepAliveID(g.MovingWindowMoveId);
		if (g.IO.MouseDown[0])
		{
			ImVec2 pos = g.IO.MousePos - g.ActiveIdClickOffset;
			g.MovingWindow->RootWindow->PosFloat = pos;
			FocusWindow(g.MovingWindow);
		}
		else
		{
			ClearActiveID();
			g.MovingWindow = NULL;
			g.MovingWindowMoveId = 0;
		}
	}
	else
	{
		g.MovingWindow = NULL;
		g.MovingWindowMoveId = 0;
	}

	g.HoveredWindow = (g.MovingWindow && !(g.MovingWindow->Flags & ImGuiWindowFlags_NoInputs)) ? g.MovingWindow : FindHoveredWindow(g.IO.MousePos);
	g.HoveredRootWindow = g.HoveredWindow ? g.HoveredWindow->RootWindow : NULL;

	ImGuiWindow* modal_window = GetFrontMostModalRootWindow();
	if (modal_window != NULL)
	{
		g.ModalWindowDarkeningRatio = ImMin(g.ModalWindowDarkeningRatio + g.IO.DeltaTime * 6.0f, 1.0f);
		if (g.HoveredRootWindow && !IsWindowChildOf(g.HoveredRootWindow, modal_window))
			g.HoveredRootWindow = g.HoveredWindow = NULL;
	}
	else
	{
		g.ModalWindowDarkeningRatio = 0.0f;
	}

	int mouse_earliest_button_down = -1;
	bool mouse_any_down = false;
	for (int i = 0; i < IM_ARRAYSIZE(g.IO.MouseDown); i++)
	{
		if (g.IO.MouseClicked[i])
			g.IO.MouseDownOwned[i] = (g.HoveredWindow != NULL) || (!g.OpenPopupStack.empty());
		mouse_any_down |= g.IO.MouseDown[i];
		if (g.IO.MouseDown[i])
			if (mouse_earliest_button_down == -1 || g.IO.MouseClickedTime[i] < g.IO.MouseClickedTime[mouse_earliest_button_down])
				mouse_earliest_button_down = i;
	}
	bool mouse_avail_to_imgui = (mouse_earliest_button_down == -1) || g.IO.MouseDownOwned[mouse_earliest_button_down];
	if (g.WantCaptureMouseNextFrame != -1)
		g.IO.WantCaptureMouse = (g.WantCaptureMouseNextFrame != 0);
	else
		g.IO.WantCaptureMouse = (mouse_avail_to_imgui && (g.HoveredWindow != NULL || mouse_any_down)) || (!g.OpenPopupStack.empty());
	if (g.WantCaptureKeyboardNextFrame != -1)
		g.IO.WantCaptureKeyboard = (g.WantCaptureKeyboardNextFrame != 0);
	else
		g.IO.WantCaptureKeyboard = (g.ActiveId != 0) || (modal_window != NULL);
	g.IO.WantTextInput = (g.WantTextInputNextFrame != -1) ? (g.WantTextInputNextFrame != 0) : 0;
	g.MouseCursor = ImGuiMouseCursor_Arrow;
	g.WantCaptureMouseNextFrame = g.WantCaptureKeyboardNextFrame = g.WantTextInputNextFrame = -1;
	g.OsImePosRequest = ImVec2(1.0f, 1.0f);
	if (g.HoveredWindow && g.IO.MouseWheel != 0.0f && !g.HoveredWindow->Collapsed)
	{
		ImGuiWindow* window = g.HoveredWindow;
		ImGuiWindow* scroll_window = window;
		while ((scroll_window->Flags & ImGuiWindowFlags_ChildWindow) && (scroll_window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(scroll_window->Flags & ImGuiWindowFlags_NoScrollbar) && !(scroll_window->Flags & ImGuiWindowFlags_NoInputs) && scroll_window->ParentWindow)
			scroll_window = scroll_window->ParentWindow;

		if (!(scroll_window->Flags & ImGuiWindowFlags_NoScrollWithMouse) && !(scroll_window->Flags & ImGuiWindowFlags_NoInputs))
		{
			float scroll_amount = 5 * scroll_window->CalcFontSize();
			scroll_amount = (float)(int)ImMin(scroll_amount, (scroll_window->ContentsRegionRect.GetHeight() + scroll_window->WindowPadding.y * 2.0f) * 0.67f);
			SetWindowScrollY(scroll_window, scroll_window->Scroll.y - g.IO.MouseWheel * scroll_amount);
		}
	}

	for (int i = 0; i != g.Windows.Size; i++)
	{
		ImGuiWindow* window = g.Windows[i];
		window->WasActive = window->Active;
		window->Active = false;
		window->WriteAccessed = false;
	}

	if (g.NavWindow && !g.NavWindow->WasActive)
		FocusFrontMostActiveWindow(NULL);

	g.CurrentWindowStack.resize(0);
	g.CurrentPopupStack.resize(0);
	CloseInactivePopups(g.NavWindow);
}

void ImGui::Initialize()
{
	ImGuiContext& g = *GImGui;
	g.Initialized = true;
}

int ChildWindowComparer(const void* lhs, const void* rhs)
{
	const ImGuiWindow* a = *(const ImGuiWindow**)lhs;
	const ImGuiWindow* b = *(const ImGuiWindow**)rhs;
	if (int d = (a->Flags & ImGuiWindowFlags_Popup) - (b->Flags & ImGuiWindowFlags_Popup))
		return d;
	return (a->BeginOrderWithinParent - b->BeginOrderWithinParent);
}

void AddWindowToSortedBuffer(ImVector<ImGuiWindow*>& out_sorted_windows, ImGuiWindow* window)
{
	out_sorted_windows.push_back(window);
	if (window->Active)
	{
		int count = window->DC.ChildWindows.Size;
		if (count > 1)
			qsort(window->DC.ChildWindows.begin(), (size_t)count, sizeof(ImGuiWindow*), ChildWindowComparer);
		for (int i = 0; i < count; i++)
		{
			ImGuiWindow* child = window->DC.ChildWindows[i];
			if (child->Active)
				AddWindowToSortedBuffer(out_sorted_windows, child);
		}
	}
}

void AddDrawListToRenderList(ImVector<ImDrawList*>& out_render_list, ImDrawList* draw_list)
{
	if (draw_list->CmdBuffer.empty())
		return;

	ImDrawCmd& last_cmd = draw_list->CmdBuffer.back();
	if (last_cmd.ElemCount == 0 && last_cmd.UserCallback == NULL)
	{
		draw_list->CmdBuffer.pop_back();
		if (draw_list->CmdBuffer.empty())
			return;
	}

	out_render_list.push_back(draw_list);
	GImGui->IO.MetricsRenderVertices += draw_list->VtxBuffer.Size;
	GImGui->IO.MetricsRenderIndices += draw_list->IdxBuffer.Size;
}

void AddWindowToRenderList(ImVector<ImDrawList*>& out_render_list, ImGuiWindow* window)
{
	AddDrawListToRenderList(out_render_list, window->DrawList);
	for (int i = 0; i < window->DC.ChildWindows.Size; i++)
	{
		ImGuiWindow* child = window->DC.ChildWindows[i];
		if (!child->Active)
			continue;
		if (child->HiddenFrames > 0)
			continue;
		AddWindowToRenderList(out_render_list, child);
	}
}

void AddWindowToRenderListSelectLayer(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	g.IO.MetricsActiveWindows++;
	if (window->Flags & ImGuiWindowFlags_Popup)
		AddWindowToRenderList(g.RenderDrawLists[1], window);
	else
		AddWindowToRenderList(g.RenderDrawLists[0], window);
}

void ImGui::PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DrawList->PushClipRect(clip_rect_min, clip_rect_max, intersect_with_current_clip_rect);
	window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::PopClipRect()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DrawList->PopClipRect();
	window->ClipRect = window->DrawList->_ClipRectStack.back();
}

void ImGui::EndFrame()
{
	ImGuiContext& g = *GImGui;
	if (g.FrameCountEnded == g.FrameCount)
		return;

	if (g.IO.ImeSetInputScreenPosFn && ImLengthSqr(g.OsImePosRequest - g.OsImePosSet) > 0.0001f)
	{
		g.IO.ImeSetInputScreenPosFn((int)g.OsImePosRequest.x, (int)g.OsImePosRequest.y);
		g.OsImePosSet = g.OsImePosRequest;
	}

	if (g.CurrentWindow && !g.CurrentWindow->WriteAccessed)
		g.CurrentWindow->Active = false;

	if (g.ActiveId == 0 && g.HoveredId == 0)
	{
		if (!g.NavWindow || !g.NavWindow->Appearing)
		{
			if (g.IO.MouseClicked[0])
			{
				if (g.HoveredRootWindow != NULL)
				{
					FocusWindow(g.HoveredWindow);
					if (!(g.HoveredWindow->Flags & ImGuiWindowFlags_NoMove) && !(g.HoveredRootWindow->Flags & ImGuiWindowFlags_NoMove))
					{
						g.MovingWindow = g.HoveredWindow;
						g.MovingWindowMoveId = g.MovingWindow->MoveId;
						SetActiveID(g.MovingWindowMoveId, g.HoveredRootWindow);
						g.ActiveIdClickOffset = g.IO.MousePos - g.MovingWindow->RootWindow->Pos;
					}
				}
				else if (g.NavWindow != NULL && GetFrontMostModalRootWindow() == NULL)
				{
					FocusWindow(NULL);
				}
			}

			if (g.IO.MouseClicked[1])
			{
				ImGuiWindow* modal = GetFrontMostModalRootWindow();
				bool hovered_window_above_modal = false;
				if (modal == NULL)
					hovered_window_above_modal = true;
				for (int i = g.Windows.Size - 1; i >= 0 && hovered_window_above_modal == false; i--)
				{
					ImGuiWindow* window = g.Windows[i];
					if (window == modal)
						break;
					if (window == g.HoveredWindow)
						hovered_window_above_modal = true;
				}
				CloseInactivePopups(hovered_window_above_modal ? g.HoveredWindow : modal);
			}
		}
	}

	g.WindowsSortBuffer.resize(0);
	g.WindowsSortBuffer.reserve(g.Windows.Size);
	for (int i = 0; i != g.Windows.Size; i++)
	{
		ImGuiWindow* window = g.Windows[i];
		if (window->Active && (window->Flags & ImGuiWindowFlags_ChildWindow))
			continue;
		AddWindowToSortedBuffer(g.WindowsSortBuffer, window);
	}

	g.Windows.swap(g.WindowsSortBuffer);
	g.IO.MouseWheel = 0.0f;
	memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));

	g.FrameCountEnded = g.FrameCount;
}

void ImGui::Render()
{
	ImGuiContext& g = *GImGui;

	if (g.FrameCountEnded != g.FrameCount)
		ImGui::EndFrame();
	g.FrameCountRendered = g.FrameCount;

	if (g.Style.Alpha > 0.0f)
	{
		g.IO.MetricsRenderVertices = g.IO.MetricsRenderIndices = g.IO.MetricsActiveWindows = 0;
		for (int i = 0; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
			g.RenderDrawLists[i].resize(0);
		for (int i = 0; i != g.Windows.Size; i++)
		{
			ImGuiWindow* window = g.Windows[i];
			if (window->Active && window->HiddenFrames <= 0 && (window->Flags & (ImGuiWindowFlags_ChildWindow)) == 0)
				AddWindowToRenderListSelectLayer(window);
		}

		int n = g.RenderDrawLists[0].Size;
		int flattened_size = n;
		for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
			flattened_size += g.RenderDrawLists[i].Size;
		g.RenderDrawLists[0].resize(flattened_size);
		for (int i = 1; i < IM_ARRAYSIZE(g.RenderDrawLists); i++)
		{
			ImVector<ImDrawList*>& layer = g.RenderDrawLists[i];
			if (layer.empty())
				continue;
			memcpy(&g.RenderDrawLists[0][n], &layer[0], layer.Size * sizeof(ImDrawList*));
			n += layer.Size;
		}

		if (g.IO.MouseDrawCursor)
		{
			const ImGuiMouseCursorData& cursor_data = g.MouseCursorData[g.MouseCursor];
			const ImVec2 pos = g.IO.MousePos - cursor_data.HotOffset;
			const ImVec2 size = cursor_data.Size;
			const ImTextureID tex_id = g.IO.Fonts->TexID;
			g.OverlayDrawList.PushTextureID(tex_id);
			g.OverlayDrawList.AddImage(tex_id, pos + ImVec2(1, 0), pos + ImVec2(1, 0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 48));
			g.OverlayDrawList.AddImage(tex_id, pos + ImVec2(2, 0), pos + ImVec2(2, 0) + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 48));
			g.OverlayDrawList.AddImage(tex_id, pos, pos + size, cursor_data.TexUvMin[1], cursor_data.TexUvMax[1], IM_COL32(0, 0, 0, 255));
			g.OverlayDrawList.AddImage(tex_id, pos, pos + size, cursor_data.TexUvMin[0], cursor_data.TexUvMax[0], IM_COL32(255, 255, 255, 255));
			g.OverlayDrawList.PopTextureID();
		}
		if (!g.OverlayDrawList.VtxBuffer.empty())
			AddDrawListToRenderList(g.RenderDrawLists[0], &g.OverlayDrawList);

		g.RenderDrawData.Valid = true;
		g.RenderDrawData.CmdLists = (g.RenderDrawLists[0].Size > 0) ? &g.RenderDrawLists[0][0] : NULL;
		g.RenderDrawData.CmdListsCount = g.RenderDrawLists[0].Size;
		g.RenderDrawData.TotalVtxCount = g.IO.MetricsRenderVertices;
		g.RenderDrawData.TotalIdxCount = g.IO.MetricsRenderIndices;

		if (g.RenderDrawData.CmdListsCount > 0 && g.IO.RenderDrawListsFn != NULL)
			g.IO.RenderDrawListsFn(&g.RenderDrawData);
	}
}

const char* ImGui::FindRenderedTextEnd(const char* text, const char* text_end)
{
	const char* text_display_end = text;
	if (!text_end)
		text_end = (const char*)-1;

	while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		text_display_end++;
	return text_display_end;
}

void ImGui::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	const char* text_display_end;
	if (hide_text_after_hash)
	{
		text_display_end = FindRenderedTextEnd(text, text_end);
	}
	else
	{
		if (!text_end) text_end = text + strlen(text);
		text_display_end = text_end;
	}

	const int text_len = (int)(text_display_end - text);
	if (text_len > 0)
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end);
}

void ImGui::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if (!text_end)
		text_end = text + strlen(text);

	const int text_len = (int)(text_end - text);
	if (text_len > 0)
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_end, wrap_width);
}

void ImGui::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
	const char* text_display_end = FindRenderedTextEnd(text, text_end);
	const int text_len = (int)(text_display_end - text);
	if (text_len == 0)
		return;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	ImVec2 pos = pos_min;
	const ImVec2 text_size = text_size_if_known ? *text_size_if_known : CalcTextSize(text, text_display_end, false, 0.0f);

	const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
	const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
	bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
	if (clip_rect)
		need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

	if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
	if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

	if (need_clipping)
	{
		ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
	}
	else
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColorU32(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
}

void ImGui::RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
	const float border_size = g.Style.FrameBorderSize;
	if (border && border_size > 0.0f)
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, ImDrawCornerFlags_All, border_size);
		window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, ImDrawCornerFlags_All, border_size);
	}
}

void ImGui::RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	const float border_size = g.Style.FrameBorderSize;
	if (border_size > 0.0f)
	{
		window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, ImDrawCornerFlags_All, border_size);
		window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, ImDrawCornerFlags_All, border_size);
	}
}

void ImGui::RenderTriangle(ImVec2 p_min, ImGuiDir dir, float scale)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	const float h = g.FontSize * 1.00f;
	float r = h * 0.40f * scale;
	ImVec2 center = p_min + ImVec2(h * 0.50f, h * 0.50f * scale);

	ImVec2 a, b, c;
	switch (dir)
	{
	case ImGuiDir_Up:
	case ImGuiDir_Down:
		if (dir == ImGuiDir_Up) r = -r;
		center.y -= r * 0.25f;
		a = ImVec2(0, 1) * r;
		b = ImVec2(-0.866f, -0.5f) * r;
		c = ImVec2(+0.866f, -0.5f) * r;
		break;
	case ImGuiDir_Left:
	case ImGuiDir_Right:
		if (dir == ImGuiDir_Left) r = -r;
		center.x -= r * 0.25f;
		a = ImVec2(1, 0) * r;
		b = ImVec2(-0.500f, +0.866f) * r;
		c = ImVec2(-0.500f, -0.866f) * r;
		break;
	case ImGuiDir_None:
	case ImGuiDir_Count_:
		break;
	}

	window->DrawList->AddTriangleFilled(center + a, center + b, center + c, GetColorU32(ImGuiCol_Text));
}

void ImGui::RenderCheckMark(ImVec2 pos, ImU32 col, float sz)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness * 0.5f;
	pos += ImVec2(thickness*0.25f, thickness*0.25f);

	float third = sz / 3.0f;
	float bx = pos.x + third;
	float by = pos.y + sz - third * 0.5f;
	window->DrawList->PathLineTo(ImVec2(bx - third, by - third));
	window->DrawList->PathLineTo(ImVec2(bx, by));
	window->DrawList->PathLineTo(ImVec2(bx + third * 2, by - third * 2));
	window->DrawList->PathStroke(col, false, thickness);
}

ImVec2 ImGui::CalcTextSize(const char* text, const char* text_end, bool hide_text_after_double_hash, float wrap_width)
{
	ImGuiContext& g = *GImGui;

	const char* text_display_end;
	if (hide_text_after_double_hash)
		text_display_end = FindRenderedTextEnd(text, text_end);
	else
		text_display_end = text_end;

	ImFont* font = g.Font;
	const float font_size = g.FontSize;
	if (text == text_display_end)
		return ImVec2(0.0f, font_size);
	ImVec2 text_size = font->CalcTextSizeA(font_size, FLT_MAX, wrap_width, text, text_display_end, NULL);

	const float font_scale = font_size / font->FontSize;
	const float character_spacing_x = 1.0f * font_scale;
	if (text_size.x > 0.0f)
		text_size.x -= character_spacing_x;
	text_size.x = (float)(int)(text_size.x + 0.95f);

	return text_size;
}

void ImGui::CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (window->SkipItems)
	{
		*out_items_display_start = *out_items_display_end = 0;
		return;
	}

	const ImVec2 pos = window->DC.CursorPos;
	int start = (int)((window->ClipRect.Min.y - pos.y) / items_height);
	int end = (int)((window->ClipRect.Max.y - pos.y) / items_height);
	start = ImClamp(start, 0, items_count);
	end = ImClamp(end + 1, start, items_count);
	*out_items_display_start = start;
	*out_items_display_end = end;
}

ImGuiWindow* FindHoveredWindow(ImVec2 pos)
{
	ImGuiContext& g = *GImGui;
	for (int i = g.Windows.Size - 1; i >= 0; i--)
	{
		ImGuiWindow* window = g.Windows[i];
		if (!window->Active)
			continue;
		if (window->Flags & ImGuiWindowFlags_NoInputs)
			continue;
		ImRect bb(window->WindowRectClipped.Min - g.Style.TouchExtraPadding, window->WindowRectClipped.Max + g.Style.TouchExtraPadding);
		if (bb.Contains(pos))
			return window;
	}
	return NULL;
}

bool ImGui::IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImRect rect_clipped(r_min, r_max);
	if (clip)
		rect_clipped.ClipWith(window->ClipRect);
	const ImRect rect_for_touch(rect_clipped.Min - g.Style.TouchExtraPadding, rect_clipped.Max + g.Style.TouchExtraPadding);
	return rect_for_touch.Contains(g.IO.MousePos);
}

bool ImGui::IsAnyWindowHovered()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredWindow != NULL;
}

bool ImGui::IsAnyWindowFocused()
{
	ImGuiContext& g = *GImGui;
	return g.NavWindow != NULL;
}

bool IsKeyPressedMap(ImGuiKey key, bool repeat)
{
	const int key_index = GImGui->IO.KeyMap[key];
	return (key_index >= 0) ? ImGui::IsKeyPressed(key_index, repeat) : false;
}

int ImGui::GetKeyIndex(ImGuiKey imgui_key)
{
	return GImGui->IO.KeyMap[imgui_key];
}

bool ImGui::IsKeyDown(int user_key_index)
{
	if (user_key_index < 0) return false;
	return GImGui->IO.KeysDown[user_key_index];
}

int ImGui::CalcTypematicPressedRepeatAmount(float t, float t_prev, float repeat_delay, float repeat_rate)
{
	if (t == 0.0f)
		return 1;
	if (t <= repeat_delay || repeat_rate <= 0.0f)
		return 0;
	const int count = (int)((t - repeat_delay) / repeat_rate) - (int)((t_prev - repeat_delay) / repeat_rate);
	return (count > 0) ? count : 0;
}

int ImGui::GetKeyPressedAmount(int key_index, float repeat_delay, float repeat_rate)
{
	ImGuiContext& g = *GImGui;
	if (key_index < 0) return false;
	const float t = g.IO.KeysDownDuration[key_index];
	return CalcTypematicPressedRepeatAmount(t, t - g.IO.DeltaTime, repeat_delay, repeat_rate);
}

bool ImGui::IsKeyPressed(int user_key_index, bool repeat)
{
	ImGuiContext& g = *GImGui;
	if (user_key_index < 0) return false;
	const float t = g.IO.KeysDownDuration[user_key_index];
	if (t == 0.0f)
		return true;
	if (repeat && t > g.IO.KeyRepeatDelay)
		return GetKeyPressedAmount(user_key_index, g.IO.KeyRepeatDelay, g.IO.KeyRepeatRate) > 0;
	return false;
}

bool ImGui::IsKeyReleased(int user_key_index)
{
	ImGuiContext& g = *GImGui;
	if (user_key_index < 0) return false;
	if (g.IO.KeysDownDurationPrev[user_key_index] >= 0.0f && !g.IO.KeysDown[user_key_index])
		return true;
	return false;
}

bool ImGui::IsMouseDown(int button)
{
	ImGuiContext& g = *GImGui;
	return g.IO.MouseDown[button];
}

bool ImGui::IsMouseClicked(int button, bool repeat)
{
	ImGuiContext& g = *GImGui;
	const float t = g.IO.MouseDownDuration[button];
	if (t == 0.0f)
		return true;

	if (repeat && t > g.IO.KeyRepeatDelay)
	{
		float delay = g.IO.KeyRepeatDelay, rate = g.IO.KeyRepeatRate;
		if ((fmodf(t - delay, rate) > rate*0.5f) != (fmodf(t - delay - g.IO.DeltaTime, rate) > rate*0.5f))
			return true;
	}

	return false;
}

bool ImGui::IsMouseReleased(int button)
{
	ImGuiContext& g = *GImGui;
	return g.IO.MouseReleased[button];
}

bool ImGui::IsMouseDoubleClicked(int button)
{
	ImGuiContext& g = *GImGui;
	return g.IO.MouseDoubleClicked[button];
}

bool ImGui::IsMouseDragging(int button, float lock_threshold)
{
	ImGuiContext& g = *GImGui;
	if (!g.IO.MouseDown[button])
		return false;
	if (lock_threshold < 0.0f)
		lock_threshold = g.IO.MouseDragThreshold;
	return g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold;
}

ImVec2 ImGui::GetMousePos()
{
	return GImGui->IO.MousePos;
}

ImVec2 ImGui::GetMousePosOnOpeningCurrentPopup()
{
	ImGuiContext& g = *GImGui;
	if (g.CurrentPopupStack.Size > 0)
		return g.OpenPopupStack[g.CurrentPopupStack.Size - 1].OpenMousePos;
	return g.IO.MousePos;
}

bool ImGui::IsMousePosValid(const ImVec2* mouse_pos)
{
	if (mouse_pos == NULL)
		mouse_pos = &GImGui->IO.MousePos;
	const float MOUSE_INVALID = -256000.0f;
	return mouse_pos->x >= MOUSE_INVALID && mouse_pos->y >= MOUSE_INVALID;
}

ImVec2 ImGui::GetMouseDragDelta(int button, float lock_threshold)
{
	ImGuiContext& g = *GImGui;
	if (lock_threshold < 0.0f)
		lock_threshold = g.IO.MouseDragThreshold;
	if (g.IO.MouseDown[button])
		if (g.IO.MouseDragMaxDistanceSqr[button] >= lock_threshold * lock_threshold)
			return g.IO.MousePos - g.IO.MouseClickedPos[button];
	return ImVec2(0.0f, 0.0f);
}

void ImGui::ResetMouseDragDelta(int button)
{
	ImGuiContext& g = *GImGui;
	g.IO.MouseClickedPos[button] = g.IO.MousePos;
}

ImGuiMouseCursor ImGui::GetMouseCursor()
{
	return GImGui->MouseCursor;
}

void ImGui::SetMouseCursor(ImGuiMouseCursor cursor_type)
{
	GImGui->MouseCursor = cursor_type;
}

bool ImGui::IsItemActive()
{
	ImGuiContext& g = *GImGui;
	if (g.ActiveId)
	{
		ImGuiWindow* window = g.CurrentWindow;
		return g.ActiveId == window->DC.LastItemId;
	}
	return false;
}

bool ImGui::IsItemClicked(int mouse_button)
{
	return IsMouseClicked(mouse_button) && IsItemHovered(ImGuiHoveredFlags_Default);
}

bool ImGui::IsAnyItemHovered()
{
	ImGuiContext& g = *GImGui;
	return g.HoveredId != 0 || g.HoveredIdPreviousFrame != 0;
}

bool ImGui::IsAnyItemActive()
{
	return GImGui->ActiveId != 0;
}

bool ImGui::IsItemVisible()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(window->DC.LastItemRect);
}

void ImGui::SetItemAllowOverlap()
{
	ImGuiContext& g = *GImGui;
	if (g.HoveredId == g.CurrentWindow->DC.LastItemId)
		g.HoveredIdAllowOverlap = true;
	if (g.ActiveId == g.CurrentWindow->DC.LastItemId)
		g.ActiveIdAllowOverlap = true;
}

ImVec2 ImGui::GetItemRectMin()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.Min;
}

ImVec2 ImGui::GetItemRectMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.Max;
}

ImVec2 ImGui::GetItemRectSize()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.LastItemRect.GetSize();
}

ImRect GetVisibleRect()
{
	ImGuiContext& g = *GImGui;
	if (g.IO.DisplayVisibleMin.x != g.IO.DisplayVisibleMax.x && g.IO.DisplayVisibleMin.y != g.IO.DisplayVisibleMax.y)
		return ImRect(g.IO.DisplayVisibleMin, g.IO.DisplayVisibleMax);
	return ImRect(0.0f, 0.0f, g.IO.DisplaySize.x, g.IO.DisplaySize.y);
}

void CloseInactivePopups(ImGuiWindow* ref_window)
{
	ImGuiContext& g = *GImGui;
	if (g.OpenPopupStack.empty())
		return;
	int n = 0;
	if (ref_window)
	{
		for (n = 0; n < g.OpenPopupStack.Size; n++)
		{
			ImGuiPopupRef& popup = g.OpenPopupStack[n];
			if (!popup.Window || popup.Window->Flags & ImGuiWindowFlags_ChildWindow)
				continue;
			bool has_focus = false;
			for (int m = n; m < g.OpenPopupStack.Size && !has_focus; m++)
				has_focus = (g.OpenPopupStack[m].Window && g.OpenPopupStack[m].Window->RootWindow == ref_window->RootWindow);
			if (!has_focus)
				break;
		}
	}
	if (n < g.OpenPopupStack.Size)
		ClosePopupToLevel(n);
}

ImGuiWindow* GetFrontMostModalRootWindow()
{
	ImGuiContext& g = *GImGui;
	for (int n = g.OpenPopupStack.Size - 1; n >= 0; n--)
		if (ImGuiWindow* popup = g.OpenPopupStack.Data[n].Window)
			if (popup->Flags & ImGuiWindowFlags_Modal)
				return popup;
	return NULL;
}

void ClosePopupToLevel(int remaining)
{
	ImGuiContext& g = *GImGui;
	if (remaining > 0)
		ImGui::FocusWindow(g.OpenPopupStack[remaining - 1].Window);
	else
		ImGui::FocusWindow(g.OpenPopupStack[0].ParentWindow);
	g.OpenPopupStack.resize(remaining);
}

void ImGui::ClosePopup(ImGuiID id)
{
	if (!IsPopupOpen(id))
		return;
	ImGuiContext& g = *GImGui;
	ClosePopupToLevel(g.OpenPopupStack.Size - 1);
}

void ImGui::CloseCurrentPopup()
{
	ImGuiContext& g = *GImGui;
	int popup_idx = g.CurrentPopupStack.Size - 1;
	if (popup_idx < 0 || popup_idx >= g.OpenPopupStack.Size || g.CurrentPopupStack[popup_idx].PopupId != g.OpenPopupStack[popup_idx].PopupId)
		return;
	while (popup_idx > 0 && g.OpenPopupStack[popup_idx].Window && (g.OpenPopupStack[popup_idx].Window->Flags & ImGuiWindowFlags_ChildMenu))
		popup_idx--;
	ClosePopupToLevel(popup_idx);
}

bool ImGui::BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	if (!IsPopupOpen(id))
	{
		g.NextWindowData.Clear();
		return false;
	}

	char name[XS_RANDOM_SIZE];
	if (extra_flags & ImGuiWindowFlags_ChildMenu)
		xprintf(name, IM_ARRAYSIZE(name), XorString("##Menu_%02d"), g.CurrentPopupStack.Size);
	else
		xprintf(name, IM_ARRAYSIZE(name), XorString("##PopUp_%08x"), id);

	bool is_open = Begin(name, extra_flags | ImGuiWindowFlags_Popup);
	if (!is_open)
		EndPopup();

	return is_open;
}

bool ImGui::IsPopupOpen(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == id;
}

bool ImGui::IsPopupOpen(const char* str_id)
{
	ImGuiContext& g = *GImGui;
	return g.OpenPopupStack.Size > g.CurrentPopupStack.Size && g.OpenPopupStack[g.CurrentPopupStack.Size].PopupId == g.CurrentWindow->GetID(str_id);
}

void ImGui::EndPopup()
{
	End();
}

bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow();
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_ChildWindow;
	flags |= (parent_window->Flags & ImGuiWindowFlags_NoMove);

	const ImVec2 content_avail = ImGui::GetContentRegionAvail();
	ImVec2 size = ImFloor(size_arg);
	const int auto_fit_axises = ((size.x == 0.0f) ? (1 << ImGuiAxis_X) : 0x00) | ((size.y == 0.0f) ? (1 << ImGuiAxis_Y) : 0x00);
	if (size.x <= 0.0f)
		size.x = ImMax(content_avail.x + size.x, 4.0f);
	if (size.y <= 0.0f)
		size.y = ImMax(content_avail.y + size.y, 4.0f);

	const float backup_border_size = g.Style.ChildBorderSize;
	if (!border)
		g.Style.ChildBorderSize = 0.0f;
	flags |= extra_flags;

	char title[XS_RANDOM_SIZE];
	if (name)
		xprintf(title, IM_ARRAYSIZE(title), XorString("%s/%s_%08X"), parent_window->Name, name, id);
	else
		xprintf(title, IM_ARRAYSIZE(title), XorString("%s/%08X"), parent_window->Name, id);

	ImGui::SetNextWindowSize(size);
	bool ret = ImGui::Begin(title, flags);
	ImGuiWindow* child_window = ImGui::GetCurrentWindow();
	child_window->AutoFitChildAxises = auto_fit_axises;
	g.Style.ChildBorderSize = backup_border_size;

	return ret;
}

bool ImGui::BeginChild(const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	return BeginChildEx(str_id, window->GetID(str_id), size_arg, border, extra_flags);
}

bool ImGui::BeginChild(ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags)
{
	return BeginChildEx(NULL, id, size_arg, border, extra_flags);
}

void ImGui::EndChild()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->BeginCount > 1)
		End();
	else
	{
		ImVec2 sz = GetWindowSize();
		if (window->AutoFitChildAxises & (1 << ImGuiAxis_X))
			sz.x = ImMax(4.0f, sz.x);
		if (window->AutoFitChildAxises & (1 << ImGuiAxis_Y))
			sz.y = ImMax(4.0f, sz.y);
		End();

		ImGuiWindow* parent_window = GetCurrentWindow();
		ImRect bb(parent_window->DC.CursorPos, parent_window->DC.CursorPos + sz);
		ItemSize(sz);
		ItemAdd(bb, 0);
	}
}

bool ImGui::BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags)
{
	return BeginChild(id, size, true, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysUseWindowPadding | extra_flags);
}

void ImGui::EndChildFrame()
{
	EndChild();
}

void CheckStacksSize(ImGuiWindow* window, bool write)
{
	ImGuiContext& g = *GImGui;
	int* p_backup = &window->DC.StackSizesBackup[0];
	{ int current = window->IDStack.Size; if (write) *p_backup = current; p_backup++; };
	{ int current = window->DC.GroupStack.Size; if (write) *p_backup = current; p_backup++; };
	{ int current = g.CurrentPopupStack.Size; if (write) *p_backup = current; p_backup++; };
	{ int current = g.ColorModifiers.Size; if (write) *p_backup = current; p_backup++; };
	{ int current = g.StyleModifiers.Size; if (write) *p_backup = current; p_backup++; };
	{ int current = g.FontStack.Size; if (write) *p_backup = current; p_backup++; };
}

enum ImGuiPopupPositionPolicy
{
	ImGuiPopupPositionPolicy_Default,
	ImGuiPopupPositionPolicy_ComboBox
};

ImVec2 FindBestWindowPosForPopup(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy = ImGuiPopupPositionPolicy_Default)
{
	const ImGuiStyle& style = GImGui->Style;

	ImVec2 safe_padding = style.DisplaySafeAreaPadding;
	ImRect r_outer(GetVisibleRect());
	r_outer.Expand(ImVec2((size.x - r_outer.GetWidth() > safe_padding.x * 2) ? -safe_padding.x : 0.0f, (size.y - r_outer.GetHeight() > safe_padding.y * 2) ? -safe_padding.y : 0.0f));
	ImVec2 base_pos_clamped = ImClamp(ref_pos, r_outer.Min, r_outer.Max - size);

	if (policy == ImGuiPopupPositionPolicy_ComboBox)
	{
		const ImGuiDir dir_prefered_order[ImGuiDir_Count_] = { ImGuiDir_Down, ImGuiDir_Right, ImGuiDir_Left, ImGuiDir_Up };
		for (int n = (*last_dir != ImGuiDir_None) ? -1 : 0; n < ImGuiDir_Count_; n++)
		{
			const ImGuiDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
			if (n != -1 && dir == *last_dir)
				continue;
			ImVec2 pos;
			if (dir == ImGuiDir_Down) pos = ImVec2(r_avoid.Min.x, r_avoid.Max.y);
			if (dir == ImGuiDir_Right) pos = ImVec2(r_avoid.Min.x, r_avoid.Min.y - size.y);
			if (dir == ImGuiDir_Left) pos = ImVec2(r_avoid.Max.x - size.x, r_avoid.Max.y);
			if (dir == ImGuiDir_Up) pos = ImVec2(r_avoid.Max.x - size.x, r_avoid.Min.y - size.y);
			if (!r_outer.Contains(ImRect(pos, pos + size)))
				continue;
			*last_dir = dir;
			return pos;
		}
	}
	const ImGuiDir dir_prefered_order[ImGuiDir_Count_] = { ImGuiDir_Right, ImGuiDir_Down, ImGuiDir_Up, ImGuiDir_Left };
	for (int n = (*last_dir != ImGuiDir_None) ? -1 : 0; n < ImGuiDir_Count_; n++)
	{
		const ImGuiDir dir = (n == -1) ? *last_dir : dir_prefered_order[n];
		if (n != -1 && dir == *last_dir)
			continue;
		float avail_w = (dir == ImGuiDir_Left ? r_avoid.Min.x : r_outer.Max.x) - (dir == ImGuiDir_Right ? r_avoid.Max.x : r_outer.Min.x);
		float avail_h = (dir == ImGuiDir_Up ? r_avoid.Min.y : r_outer.Max.y) - (dir == ImGuiDir_Down ? r_avoid.Max.y : r_outer.Min.y);
		if (avail_w < size.x || avail_h < size.y)
			continue;
		ImVec2 pos;
		pos.x = (dir == ImGuiDir_Left) ? r_avoid.Min.x - size.x : (dir == ImGuiDir_Right) ? r_avoid.Max.x : base_pos_clamped.x;
		pos.y = (dir == ImGuiDir_Up) ? r_avoid.Min.y - size.y : (dir == ImGuiDir_Down) ? r_avoid.Max.y : base_pos_clamped.y;
		*last_dir = dir;
		return pos;
	}
	*last_dir = ImGuiDir_None;
	ImVec2 pos = ref_pos;
	pos.x = ImMax(ImMin(pos.x + size.x, r_outer.Max.x) - size.x, r_outer.Min.x);
	pos.y = ImMax(ImMin(pos.y + size.y, r_outer.Max.y) - size.y, r_outer.Min.y);
	return pos;
}

void SetWindowConditionAllowFlags(ImGuiWindow* window, ImGuiCond flags, bool enabled)
{
	window->SetWindowPosAllowFlags = enabled ? (window->SetWindowPosAllowFlags | flags) : (window->SetWindowPosAllowFlags & ~flags);
	window->SetWindowSizeAllowFlags = enabled ? (window->SetWindowSizeAllowFlags | flags) : (window->SetWindowSizeAllowFlags & ~flags);
	window->SetWindowCollapsedAllowFlags = enabled ? (window->SetWindowCollapsedAllowFlags | flags) : (window->SetWindowCollapsedAllowFlags & ~flags);
}

ImGuiWindow* ImGui::FindWindowByName(const char* name)
{
	ImGuiContext& g = *GImGui;
	ImGuiID id = ImHash(name, 0);
	return (ImGuiWindow *)g.WindowsById.GetVoidPtr(id);
}

ImVector < ImGuiStorage::Pair > ::iterator LowerBound(ImVector < ImGuiStorage::Pair > & data, ImGuiID key)
{
	ImVector < ImGuiStorage::Pair > ::iterator first = data.begin();
	ImVector < ImGuiStorage::Pair > ::iterator last = data.end();
	size_t count = (size_t)(last - first);
	while (count > 0)
	{
		size_t count2 = count >> 1;
		ImVector < ImGuiStorage::Pair > ::iterator mid = first + count2;
		if (mid->key < key) first = ++mid, count -= count2 + 1;
		else count = count2;
	};
	return first;
};

void * ImGuiStorage::GetVoidPtr(ImGuiID key) const
{
	ImVector <Pair> ::iterator it = LowerBound(const_cast < ImVector < ImGuiStorage::Pair > & > (Data), key);
	if (it == Data.end() || it->key != key) return NULL;
	return it->val_p;
};

void ImGuiStorage::SetVoidPtr(ImGuiID key, void * val)
{
	ImVector <Pair> ::iterator it = LowerBound(Data, key);
	if (it == Data.end() || it->key != key)
	{
		Data.insert(it, Pair(key, val));
		return;
	};
	it->val_p = val;
};

ImGuiWindow* CreateNewWindow(const char* name, ImVec2 size, ImGuiWindowFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow * window = new ImGuiWindow(&g, name);
	window->Flags = flags;
	g.WindowsById.SetVoidPtr(window->ID, window);

	if (!(flags & ImGuiWindowFlags_NoSavedSettings))
	{
		window->PosFloat = ImVec2(60, 60);
		window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
	}
	window->Size = window->SizeFull = window->SizeFullAtLastBegin = size;

	if ((flags & ImGuiWindowFlags_AlwaysAutoResize) != 0)
	{
		window->AutoFitFramesX = window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = false;
	}
	else
	{
		if (window->Size.x <= 0.0f)
			window->AutoFitFramesX = 2;
		if (window->Size.y <= 0.0f)
			window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = (window->AutoFitFramesX > 0) || (window->AutoFitFramesY > 0);
	}

	if (flags & ImGuiWindowFlags_NoBringToFrontOnFocus)
		g.Windows.insert(g.Windows.begin(), window);
	else
		g.Windows.push_back(window);
	return window;
}

ImVec2 CalcSizeAfterConstraint(ImGuiWindow* window, ImVec2 new_size)
{
	ImGuiContext& g = *GImGui;
	if (g.NextWindowData.SizeConstraintCond != 0)
	{
		ImRect cr = g.NextWindowData.SizeConstraintRect;
		new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
		new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
		if (g.NextWindowData.SizeCallback)
		{
			ImGuiSizeCallbackData data;
			data.UserData = g.NextWindowData.SizeCallbackUserData;
			data.Pos = window->Pos;
			data.CurrentSize = window->SizeFull;
			data.DesiredSize = new_size;
			g.NextWindowData.SizeCallback(&data);
			new_size = data.DesiredSize;
		}
	}

	if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
	{
		new_size = ImMax(new_size, g.Style.WindowMinSize);
		new_size.y = ImMax(new_size.y, window->TitleBarHeight() + window->MenuBarHeight() + ImMax(0.0f, g.Style.WindowRounding - 1.0f));
	}
	return new_size;
}

ImVec2 CalcSizeContents(ImGuiWindow* window)
{
	ImVec2 sz;
	sz.x = (float)(int)((window->SizeContentsExplicit.x != 0.0f) ? window->SizeContentsExplicit.x : (window->DC.CursorMaxPos.x - window->Pos.x + window->Scroll.x));
	sz.y = (float)(int)((window->SizeContentsExplicit.y != 0.0f) ? window->SizeContentsExplicit.y : (window->DC.CursorMaxPos.y - window->Pos.y + window->Scroll.y));
	return sz + window->WindowPadding;
}

ImVec2 CalcSizeAutoFit(ImGuiWindow* window, const ImVec2& size_contents)
{
	ImGuiContext& g = *GImGui;
	ImGuiStyle& style = g.Style;
	ImGuiWindowFlags flags = window->Flags;
	ImVec2 size_auto_fit;
	size_auto_fit = ImClamp(size_contents, style.WindowMinSize, ImMax(style.WindowMinSize, g.IO.DisplaySize - g.Style.DisplaySafeAreaPadding));
	ImVec2 size_auto_fit_after_constraint = CalcSizeAfterConstraint(window, size_auto_fit);
	if (size_auto_fit_after_constraint.x < size_contents.x && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar))
		size_auto_fit.y += style.ScrollbarSize;
	if (size_auto_fit_after_constraint.y < size_contents.y && !(flags & ImGuiWindowFlags_NoScrollbar))
		size_auto_fit.x += style.ScrollbarSize;
	return size_auto_fit;
}

float GetScrollMaxX(ImGuiWindow* window)
{
	return ImMax(0.0f, window->SizeContents.x - (window->SizeFull.x - window->ScrollbarSizes.x));
}

float GetScrollMaxY(ImGuiWindow* window)
{
	return ImMax(0.0f, window->SizeContents.y - (window->SizeFull.y - window->ScrollbarSizes.y));
}

ImVec2 CalcNextScrollFromScrollTargetAndClamp(ImGuiWindow* window)
{
	ImVec2 scroll = window->Scroll;
	float cr_x = window->ScrollTargetCenterRatio.x;
	float cr_y = window->ScrollTargetCenterRatio.y;
	if (window->ScrollTarget.x < FLT_MAX)
		scroll.x = window->ScrollTarget.x - cr_x * (window->SizeFull.x - window->ScrollbarSizes.x);
	if (window->ScrollTarget.y < FLT_MAX)
		scroll.y = window->ScrollTarget.y - (1.0f - cr_y) * (window->TitleBarHeight() + window->MenuBarHeight()) - cr_y * (window->SizeFull.y - window->ScrollbarSizes.y);
	scroll = ImMax(scroll, ImVec2(0.0f, 0.0f));
	if (!window->Collapsed && !window->SkipItems)
	{
		scroll.x = ImMin(scroll.x, GetScrollMaxX(window));
		scroll.y = ImMin(scroll.y, GetScrollMaxY(window));
	}
	return scroll;
}

ImGuiCol GetWindowBgColorIdxFromFlags(ImGuiWindowFlags flags)
{
	if (flags & (ImGuiWindowFlags_Popup))
		return ImGuiCol_PopupBg;
	if (flags & ImGuiWindowFlags_ChildWindow)
		return ImGuiCol_ChildBg;
	return ImGuiCol_WindowBg;
}

void CalcResizePosSizeFromAnyCorner(ImGuiWindow* window, const ImVec2& corner_target, const ImVec2& corner_norm, ImVec2* out_pos, ImVec2* out_size)
{
	ImVec2 pos_min = ImLerp(corner_target, window->Pos, corner_norm);
	ImVec2 pos_max = ImLerp(window->Pos + window->Size, corner_target, corner_norm);
	ImVec2 size_expected = pos_max - pos_min;
	ImVec2 size_constrained = CalcSizeAfterConstraint(window, size_expected);
	*out_pos = pos_min;
	if (corner_norm.x == 0.0f)
		out_pos->x -= (size_constrained.x - size_expected.x);
	if (corner_norm.y == 0.0f)
		out_pos->y -= (size_constrained.y - size_expected.y);
	*out_size = size_constrained;
}

struct ImGuiResizeGripDef
{
	ImVec2 CornerPos;
	ImVec2 InnerDir;
	int AngleMin12, AngleMax12;
};

const ImGuiResizeGripDef resize_grip_def[4] =
{
	{ ImVec2(1,1), ImVec2(-1,-1), 0, 3 },
{ ImVec2(0,1), ImVec2(+1,-1), 3, 6 },
{ ImVec2(0,0), ImVec2(+1,+1), 6, 9 },
{ ImVec2(1,0), ImVec2(-1,+1), 9,12 },
};

ImRect GetBorderRect(ImGuiWindow* window, int border_n, float perp_padding, float thickness)
{
	ImRect rect = window->Rect();
	if (thickness == 0.0f) rect.Max -= ImVec2(1, 1);
	if (border_n == 0) return ImRect(rect.Min.x + perp_padding, rect.Min.y, rect.Max.x - perp_padding, rect.Min.y + thickness);
	if (border_n == 1) return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x, rect.Max.y - perp_padding);
	if (border_n == 2) return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y);
	if (border_n == 3) return ImRect(rect.Min.x, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding);
	return ImRect();
}

bool ImGui::Begin(const char* name, ImGuiWindowFlags flags)
{
	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	ImGuiWindow* window = FindWindowByName(name);
	if (!window)
	{
		ImVec2 size_on_first_use = (g.NextWindowData.SizeCond != 0) ? g.NextWindowData.SizeVal : ImVec2(0.0f, 0.0f);
		window = CreateNewWindow(name, size_on_first_use, flags);
	}

	if (flags & ImGuiWindowFlags_NoInputs)
		flags |= ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

	const int current_frame = g.FrameCount;
	const bool first_begin_of_the_frame = (window->LastFrameActive != current_frame);
	if (first_begin_of_the_frame)
		window->Flags = (ImGuiWindowFlags)flags;
	else
		flags = window->Flags;

	bool window_just_activated_by_user = (window->LastFrameActive < current_frame - 1);
	const bool window_just_appearing_after_hidden_for_resize = (window->HiddenFrames == 1);
	if (flags & ImGuiWindowFlags_Popup)
	{
		ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
		window_just_activated_by_user |= (window->PopupId != popup_ref.PopupId);
		window_just_activated_by_user |= (window != popup_ref.Window);
	}
	window->Appearing = (window_just_activated_by_user || window_just_appearing_after_hidden_for_resize);
	if (window->Appearing)
		SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, true);
	ImGuiWindow* parent_window_in_stack = g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back();
	ImGuiWindow* parent_window = first_begin_of_the_frame ? ((flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)) ? parent_window_in_stack : NULL) : window->ParentWindow;

	g.CurrentWindowStack.push_back(window);
	SetCurrentWindow(window);
	CheckStacksSize(window, true);
	if (flags & ImGuiWindowFlags_Popup)
	{
		ImGuiPopupRef& popup_ref = g.OpenPopupStack[g.CurrentPopupStack.Size];
		popup_ref.Window = window;
		g.CurrentPopupStack.push_back(popup_ref);
		window->PopupId = popup_ref.PopupId;
	}
	bool window_pos_set_by_api = false;
	bool window_size_x_set_by_api = false, window_size_y_set_by_api = false;
	if (g.NextWindowData.PosCond)
	{
		window_pos_set_by_api = (window->SetWindowPosAllowFlags & g.NextWindowData.PosCond) != 0;
		if (window_pos_set_by_api && ImLengthSqr(g.NextWindowData.PosPivotVal) > 0.00001f)
		{
			window->SetWindowPosVal = g.NextWindowData.PosVal;
			window->SetWindowPosPivot = g.NextWindowData.PosPivotVal;
			window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
		}
		else
		{
			SetWindowPos(window, g.NextWindowData.PosVal, g.NextWindowData.PosCond);
		}
		g.NextWindowData.PosCond = 0;
	}
	if (g.NextWindowData.SizeCond)
	{
		window_size_x_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.x > 0.0f);
		window_size_y_set_by_api = (window->SetWindowSizeAllowFlags & g.NextWindowData.SizeCond) != 0 && (g.NextWindowData.SizeVal.y > 0.0f);
		SetWindowSize(window, g.NextWindowData.SizeVal, g.NextWindowData.SizeCond);
		g.NextWindowData.SizeCond = 0;
	}
	if (g.NextWindowData.ContentSizeCond)
	{
		window->SizeContentsExplicit = g.NextWindowData.ContentSizeVal;
		window->SizeContentsExplicit.y += window->TitleBarHeight() + window->MenuBarHeight();
		g.NextWindowData.ContentSizeCond = 0;
	}
	else if (first_begin_of_the_frame)
	{
		window->SizeContentsExplicit = ImVec2(0.0f, 0.0f);
	}
	if (g.NextWindowData.CollapsedCond)
	{
		SetWindowCollapsed(window, g.NextWindowData.CollapsedVal, g.NextWindowData.CollapsedCond);
		g.NextWindowData.CollapsedCond = 0;
	}
	if (g.NextWindowData.FocusCond)
	{
		SetWindowFocus();
		g.NextWindowData.FocusCond = 0;
	}
	if (window->Appearing)
		SetWindowConditionAllowFlags(window, ImGuiCond_Appearing, false);

	if (first_begin_of_the_frame)
	{
		window->ParentWindow = parent_window;
		window->RootWindow = window->RootNonPopupWindow = window;
		if (parent_window && (flags & ImGuiWindowFlags_ChildWindow))
			window->RootWindow = parent_window->RootWindow;
		if (parent_window && !(flags & ImGuiWindowFlags_Modal) && (flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_Popup)))
			window->RootNonPopupWindow = parent_window->RootNonPopupWindow;

		window->Active = true;
		window->BeginOrderWithinParent = 0;
		window->BeginOrderWithinContext = g.WindowsActiveCount++;
		window->BeginCount = 0;
		window->ClipRect = ImVec4(-FLT_MAX, -FLT_MAX, +FLT_MAX, +FLT_MAX);
		window->LastFrameActive = current_frame;
		window->IDStack.resize(1);
		window->DrawList->Clear();
		window->DrawList->Flags = (g.Style.AntiAliasedLines ? ImDrawListFlags_AntiAliasedLines : 0) | (g.Style.AntiAliasedFill ? ImDrawListFlags_AntiAliasedFill : 0);
		window->DrawList->PushTextureID(g.Font->ContainerAtlas->TexID);
		ImRect fullscreen_rect(GetVisibleRect());
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup))
			PushClipRect(parent_window->ClipRect.Min, parent_window->ClipRect.Max, true);
		else
			PushClipRect(fullscreen_rect.Min, fullscreen_rect.Max, true);

		if (window_just_activated_by_user)
		{
			window->AutoPosLastDirection = ImGuiDir_None;
			if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api)
				window->PosFloat = g.CurrentPopupStack.back().OpenPopupPos;
		}
		if (!(flags & ImGuiWindowFlags_NoTitleBar) && !(flags & ImGuiWindowFlags_NoCollapse))
		{
			ImRect title_bar_rect = window->TitleBarRect();
			if (g.HoveredWindow == window && IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max) && g.IO.MouseDoubleClicked[0])
			{
				window->Collapsed = !window->Collapsed;
				FocusWindow(window);
			}
		}
		else
			window->Collapsed = false;

		window->SizeContents = CalcSizeContents(window);
		if (window->HiddenFrames > 0)
			window->HiddenFrames--;
		if ((flags & (ImGuiWindowFlags_Popup)) != 0 && window_just_activated_by_user)
		{
			window->HiddenFrames = 1;
			if (flags & ImGuiWindowFlags_AlwaysAutoResize)
			{
				if (!window_size_x_set_by_api)
					window->Size.x = window->SizeFull.x = 0.f;
				if (!window_size_y_set_by_api)
					window->Size.y = window->SizeFull.y = 0.f;
				window->SizeContents = ImVec2(0.f, 0.f);
			}
		}
		window->WindowRounding = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildRounding : ((flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupRounding : style.WindowRounding;
		window->WindowBorderSize = (flags & ImGuiWindowFlags_ChildWindow) ? style.ChildBorderSize : ((flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiWindowFlags_Modal)) ? style.PopupBorderSize : style.WindowBorderSize;
		window->WindowPadding = style.WindowPadding;
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & (ImGuiWindowFlags_AlwaysUseWindowPadding | ImGuiWindowFlags_Popup)) && window->WindowBorderSize == 0.0f)
			window->WindowPadding = ImVec2(0.0f, (flags & ImGuiWindowFlags_MenuBar) ? style.WindowPadding.y : 0.0f);
		const float window_rounding = window->WindowRounding;
		const float window_border_size = window->WindowBorderSize;
		const ImVec2 size_auto_fit = CalcSizeAutoFit(window, window->SizeContents);
		ImVec2 size_full_modified(FLT_MAX, FLT_MAX);
		if (flags & ImGuiWindowFlags_AlwaysAutoResize && !window->Collapsed)
		{
			if (!window_size_x_set_by_api)
				window->SizeFull.x = size_full_modified.x = size_auto_fit.x;
			if (!window_size_y_set_by_api)
				window->SizeFull.y = size_full_modified.y = size_auto_fit.y;
		}
		else if (window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
		{
			if (!window_size_x_set_by_api && window->AutoFitFramesX > 0)
				window->SizeFull.x = size_full_modified.x = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.x, size_auto_fit.x) : size_auto_fit.x;
			if (!window_size_y_set_by_api && window->AutoFitFramesY > 0)
				window->SizeFull.y = size_full_modified.y = window->AutoFitOnlyGrows ? ImMax(window->SizeFull.y, size_auto_fit.y) : size_auto_fit.y;
		}
		window->SizeFull = CalcSizeAfterConstraint(window, window->SizeFull);
		window->Size = window->Collapsed ? window->TitleBarRect().GetSize() : window->SizeFull;
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup))
			window->Size = window->SizeFull;
		if (!window->Collapsed)
		{
			float size_x_for_scrollbars = size_full_modified.x != FLT_MAX ? window->SizeFull.x : window->SizeFullAtLastBegin.x;
			float size_y_for_scrollbars = size_full_modified.y != FLT_MAX ? window->SizeFull.y : window->SizeFullAtLastBegin.y;
			window->ScrollbarY = (flags & ImGuiWindowFlags_AlwaysVerticalScrollbar) || ((window->SizeContents.y > size_y_for_scrollbars) && !(flags & ImGuiWindowFlags_NoScrollbar));
			window->ScrollbarX = (flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar) || ((window->SizeContents.x > size_x_for_scrollbars - (window->ScrollbarY ? style.ScrollbarSize : 0.0f) - window->WindowPadding.x) && !(flags & ImGuiWindowFlags_NoScrollbar) && (flags & ImGuiWindowFlags_HorizontalScrollbar));
			if (window->ScrollbarX && !window->ScrollbarY)
				window->ScrollbarY = (window->SizeContents.y > size_y_for_scrollbars + style.ScrollbarSize) && !(flags & ImGuiWindowFlags_NoScrollbar);
			window->ScrollbarSizes = ImVec2(window->ScrollbarY ? style.ScrollbarSize : 0.0f, window->ScrollbarX ? style.ScrollbarSize : 0.0f);
		}
		if (flags & ImGuiWindowFlags_ChildWindow)
		{
			window->BeginOrderWithinParent = parent_window->DC.ChildWindows.Size;
			parent_window->DC.ChildWindows.push_back(window);
		}
		if ((flags & ImGuiWindowFlags_ChildWindow) && !(flags & ImGuiWindowFlags_Popup) && !window_pos_set_by_api)
			window->Pos = window->PosFloat = parent_window->DC.CursorPos;

		const bool window_pos_with_pivot = (window->SetWindowPosVal.x != FLT_MAX && window->HiddenFrames == 0);
		if (window_pos_with_pivot)
		{
			SetWindowPos(window, ImMax(style.DisplaySafeAreaPadding, window->SetWindowPosVal - window->SizeFull * window->SetWindowPosPivot), 0);
		}
		else if (flags & ImGuiWindowFlags_ChildMenu)
		{
			float horizontal_overlap = style.ItemSpacing.x;
			ImGuiWindow* parent_menu = parent_window_in_stack;
			ImRect rect_to_avoid;
			if (parent_menu->DC.MenuBarAppending)
				rect_to_avoid = ImRect(-FLT_MAX, parent_menu->Pos.y + parent_menu->TitleBarHeight(), FLT_MAX, parent_menu->Pos.y + parent_menu->TitleBarHeight() + parent_menu->MenuBarHeight());
			else
				rect_to_avoid = ImRect(parent_menu->Pos.x + horizontal_overlap, -FLT_MAX, parent_menu->Pos.x + parent_menu->Size.x - horizontal_overlap - parent_menu->ScrollbarSizes.x, FLT_MAX);
			window->PosFloat = FindBestWindowPosForPopup(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
		}
		else if ((flags & ImGuiWindowFlags_Popup) != 0 && !window_pos_set_by_api && window_just_appearing_after_hidden_for_resize)
		{
			ImRect rect_to_avoid(window->PosFloat.x - 1, window->PosFloat.y - 1, window->PosFloat.x + 1, window->PosFloat.y + 1);
			window->PosFloat = FindBestWindowPosForPopup(window->PosFloat, window->Size, &window->AutoPosLastDirection, rect_to_avoid);
		}
		if (!(flags & ImGuiWindowFlags_ChildWindow))
		{
			if (!window_pos_set_by_api && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && g.IO.DisplaySize.x > 0.0f && g.IO.DisplaySize.y > 0.0f)
			{
				ImVec2 padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
				window->PosFloat = ImMax(window->PosFloat + window->Size, padding) - window->Size;
				window->PosFloat = ImMin(window->PosFloat, g.IO.DisplaySize - padding);
			}
		}
		window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
		if (window->Size.x > 0.0f && !(flags & ImGuiWindowFlags_AlwaysAutoResize))
			window->ItemWidthDefault = (float)(int)(window->Size.x * 0.65f);
		else
			window->ItemWidthDefault = (float)(int)(g.FontSize * 16.0f);
		window->FocusIdxAllRequestCurrent = (window->FocusIdxAllRequestNext == INT_MAX || window->FocusIdxAllCounter == -1) ? INT_MAX : (window->FocusIdxAllRequestNext + (window->FocusIdxAllCounter + 1)) % (window->FocusIdxAllCounter + 1);
		window->FocusIdxTabRequestCurrent = (window->FocusIdxTabRequestNext == INT_MAX || window->FocusIdxTabCounter == -1) ? INT_MAX : (window->FocusIdxTabRequestNext + (window->FocusIdxTabCounter + 1)) % (window->FocusIdxTabCounter + 1);
		window->FocusIdxAllCounter = window->FocusIdxTabCounter = -1;
		window->FocusIdxAllRequestNext = window->FocusIdxTabRequestNext = INT_MAX;
		window->Scroll = CalcNextScrollFromScrollTargetAndClamp(window);
		window->ScrollTarget = ImVec2(FLT_MAX, FLT_MAX);
		bool want_focus = false;
		if (window_just_activated_by_user && !(flags & ImGuiWindowFlags_NoFocusOnAppearing))
			if (!(flags & (ImGuiWindowFlags_ChildWindow)) || (flags & ImGuiWindowFlags_Popup))
				want_focus = true;
		if ((flags & ImGuiWindowFlags_Modal) != 0 && window == GetFrontMostModalRootWindow())
			window->DrawList->AddRectFilled(fullscreen_rect.Min, fullscreen_rect.Max, GetColorU32(ImGuiCol_ModalWindowDarkening, g.ModalWindowDarkeningRatio));

		ImRect title_bar_rect = window->TitleBarRect();
		if (window->Collapsed)
		{
			float backup_border_size = style.FrameBorderSize;
			g.Style.FrameBorderSize = window->WindowBorderSize;
			RenderFrame(title_bar_rect.Min, title_bar_rect.Max, GetColorU32(ImGuiCol_TitleBgCollapsed), true, window_rounding);
			g.Style.FrameBorderSize = backup_border_size;
		}
		else
		{
			int border_held = -1;
			ImU32 resize_grip_col[4] = { 0 };
			const int resize_grip_count = (flags & ImGuiWindowFlags_ResizeFromAnySide) ? 2 : 1;
			const int resize_border_count = (flags & ImGuiWindowFlags_ResizeFromAnySide) ? 4 : 0;
			const float grip_draw_size = (float)(int)ImMax(g.FontSize * 1.35f, window_rounding + 1.0f + g.FontSize * 0.2f);
			const float grip_hover_size = (float)(int)(grip_draw_size * 0.75f);
			if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0 && !(flags & ImGuiWindowFlags_NoResize))
			{
				ImVec2 pos_target(FLT_MAX, FLT_MAX);
				ImVec2 size_target(FLT_MAX, FLT_MAX);

				for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
				{
					const ImGuiResizeGripDef& grip = resize_grip_def[resize_grip_n];
					const ImVec2 corner = ImLerp(window->Pos, window->Pos + window->Size, grip.CornerPos);

					ImRect resize_rect(corner, corner + grip.InnerDir * grip_hover_size);
					resize_rect.FixInverted();
					bool hovered, held;
					ButtonBehavior(resize_rect, window->GetID((void *)(intptr_t)resize_grip_n), &hovered, &held, ImGuiButtonFlags_FlattenChildren);
					if (hovered || held)
						g.MouseCursor = (resize_grip_n & 1) ? ImGuiMouseCursor_ResizeNESW : ImGuiMouseCursor_ResizeNWSE;

					if (g.HoveredWindow == window && held && g.IO.MouseDoubleClicked[0] && resize_grip_n == 0)
					{
						size_target = CalcSizeAfterConstraint(window, size_auto_fit);
						ClearActiveID();
					}
					else if (held)
					{
						ImVec2 corner_target = g.IO.MousePos - g.ActiveIdClickOffset + resize_rect.GetSize() * grip.CornerPos;
						CalcResizePosSizeFromAnyCorner(window, corner_target, grip.CornerPos, &pos_target, &size_target);
					}
					if (resize_grip_n == 0 || held || hovered)
						resize_grip_col[resize_grip_n] = GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);
				}
				for (int border_n = 0; border_n < resize_border_count; border_n++)
				{
					const float BORDER_SIZE = 5.0f;
					const float BORDER_APPEAR_TIMER = 0.05f;
					bool hovered, held;
					ImRect border_rect = GetBorderRect(window, border_n, grip_hover_size, BORDER_SIZE);
					ButtonBehavior(border_rect, window->GetID((void *)(intptr_t)(border_n + 4)), &hovered, &held, ImGuiButtonFlags_FlattenChildren);
					if ((hovered && g.HoveredIdTimer > BORDER_APPEAR_TIMER) || held)
					{
						g.MouseCursor = (border_n & 1) ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
						if (held) border_held = border_n;
					}
					if (held)
					{
						ImVec2 border_target = window->Pos;
						ImVec2 border_posn;
						if (border_n == 0) { border_posn = ImVec2(0, 0); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y); }
						if (border_n == 1) { border_posn = ImVec2(1, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x + BORDER_SIZE); }
						if (border_n == 2) { border_posn = ImVec2(0, 1); border_target.y = (g.IO.MousePos.y - g.ActiveIdClickOffset.y + BORDER_SIZE); }
						if (border_n == 3) { border_posn = ImVec2(0, 0); border_target.x = (g.IO.MousePos.x - g.ActiveIdClickOffset.x); }
						CalcResizePosSizeFromAnyCorner(window, border_target, border_posn, &pos_target, &size_target);
					}
				}

				if (size_target.x != FLT_MAX)
					window->SizeFull = size_target;

				if (pos_target.x != FLT_MAX)
					window->Pos = window->PosFloat = ImVec2((float)(int)pos_target.x, (float)(int)pos_target.y);

				window->Size = window->SizeFull;
				title_bar_rect = window->TitleBarRect();
			}

			ImU32 bg_col = GetColorU32(GetWindowBgColorIdxFromFlags(flags));
			window->DrawList->AddRectFilled(window->Pos + ImVec2(0, window->TitleBarHeight()), window->Pos + window->Size, bg_col, window_rounding, (flags & ImGuiWindowFlags_NoTitleBar) ? ImDrawCornerFlags_All : ImDrawCornerFlags_Bot);

			const bool window_is_focused = want_focus || (g.NavWindow && window->RootNonPopupWindow == g.NavWindow->RootNonPopupWindow);
			if (!(flags & ImGuiWindowFlags_NoTitleBar))
				window->DrawList->AddRectFilled(title_bar_rect.Min, title_bar_rect.Max, GetColorU32(window_is_focused ? ImGuiCol_TitleBgActive : ImGuiCol_TitleBg), window_rounding, ImDrawCornerFlags_Top);

			if (flags & ImGuiWindowFlags_MenuBar)
			{
				ImRect menu_bar_rect = window->MenuBarRect();
				menu_bar_rect.ClipWith(window->Rect());
				window->DrawList->AddRectFilled(menu_bar_rect.Min, menu_bar_rect.Max, GetColorU32(ImGuiCol_MenuBarBg), (flags & ImGuiWindowFlags_NoTitleBar) ? window_rounding : 0.0f, ImDrawCornerFlags_Top);
				if (style.FrameBorderSize > 0.0f && menu_bar_rect.Max.y < window->Pos.y + window->Size.y)
					window->DrawList->AddLine(menu_bar_rect.GetBL(), menu_bar_rect.GetBR(), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);
			}

			if (window->ScrollbarX)
				Scrollbar(ImGuiLayoutType_Horizontal);
			if (window->ScrollbarY)
				Scrollbar(ImGuiLayoutType_Vertical);

			if (!(flags & ImGuiWindowFlags_NoResize))
			{
				for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
				{
					const ImGuiResizeGripDef& grip = resize_grip_def[resize_grip_n];
					const ImVec2 corner = ImLerp(window->Pos, window->Pos + window->Size, grip.CornerPos);
					window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(window_border_size, grip_draw_size) : ImVec2(grip_draw_size, window_border_size)));
					window->DrawList->PathLineTo(corner + grip.InnerDir * ((resize_grip_n & 1) ? ImVec2(grip_draw_size, window_border_size) : ImVec2(window_border_size, grip_draw_size)));
					window->DrawList->PathArcToFast(ImVec2(corner.x + grip.InnerDir.x * (window_rounding + window_border_size), corner.y + grip.InnerDir.y * (window_rounding + window_border_size)), window_rounding, grip.AngleMin12, grip.AngleMax12);
					window->DrawList->PathFillConvex(resize_grip_col[resize_grip_n]);
				}
			}

			if (window_border_size > 0.0f)
				window->DrawList->AddRect(window->Pos, window->Pos + window->Size, GetColorU32(ImGuiCol_Border), window_rounding, ImDrawCornerFlags_All, window_border_size);
			if (border_held != -1)
			{
				ImRect border = GetBorderRect(window, border_held, grip_draw_size, 0.0f);
				window->DrawList->AddLine(border.Min, border.Max, GetColorU32(ImGuiCol_SeparatorActive), ImMax(1.0f, window_border_size));
			}
			if (style.FrameBorderSize > 0 && !(flags & ImGuiWindowFlags_NoTitleBar))
				window->DrawList->AddLine(title_bar_rect.GetBL() + ImVec2(style.WindowBorderSize, -1), title_bar_rect.GetBR() + ImVec2(-style.WindowBorderSize, -1), GetColorU32(ImGuiCol_Border), style.FrameBorderSize);
		}
		window->SizeFullAtLastBegin = window->SizeFull;
		window->ContentsRegionRect.Min.x = -window->Scroll.x + window->WindowPadding.x;
		window->ContentsRegionRect.Min.y = -window->Scroll.y + window->WindowPadding.y + window->TitleBarHeight() + window->MenuBarHeight();
		window->ContentsRegionRect.Max.x = -window->Scroll.x - window->WindowPadding.x + (window->SizeContentsExplicit.x != 0.0f ? window->SizeContentsExplicit.x : (window->Size.x - window->ScrollbarSizes.x));
		window->ContentsRegionRect.Max.y = -window->Scroll.y - window->WindowPadding.y + (window->SizeContentsExplicit.y != 0.0f ? window->SizeContentsExplicit.y : (window->Size.y - window->ScrollbarSizes.y));
		window->DC.IndentX = 0.0f + window->WindowPadding.x - window->Scroll.x;
		window->DC.GroupOffsetX = 0.0f;
		window->DC.ColumnsOffsetX = 0.0f;
		window->DC.CursorStartPos = window->Pos + ImVec2(window->DC.IndentX + window->DC.ColumnsOffsetX, window->TitleBarHeight() + window->MenuBarHeight() + window->WindowPadding.y - window->Scroll.y);
		window->DC.CursorPos = window->DC.CursorStartPos;
		window->DC.CursorPosPrevLine = window->DC.CursorPos;
		window->DC.CursorMaxPos = window->DC.CursorStartPos;
		window->DC.CurrentLineHeight = window->DC.PrevLineHeight = 0.0f;
		window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset = 0.0f;
		window->DC.MenuBarAppending = false;
		window->DC.MenuBarOffsetX = ImMax(window->WindowPadding.x, style.ItemSpacing.x);
		window->DC.ChildWindows.resize(0);
		window->DC.LayoutType = ImGuiLayoutType_Vertical;
		window->DC.ItemFlags = ImGuiItemFlags_Default_;
		window->DC.ItemWidth = window->ItemWidthDefault;
		window->DC.TextWrapPos = -1.0f;
		window->DC.ItemFlagsStack.resize(0);
		window->DC.ItemWidthStack.resize(0);
		window->DC.TextWrapPosStack.resize(0);
		window->DC.ColumnsSet = NULL;
		window->DC.TreeDepth = 0;
		window->DC.StateStorage = &window->StateStorage;
		window->DC.GroupStack.resize(0);

		if ((flags & ImGuiWindowFlags_ChildWindow) && (window->DC.ItemFlags != parent_window->DC.ItemFlags))
		{
			window->DC.ItemFlags = parent_window->DC.ItemFlags;
			window->DC.ItemFlagsStack.push_back(window->DC.ItemFlags);
		}

		if (window->AutoFitFramesX > 0)
			window->AutoFitFramesX--;
		if (window->AutoFitFramesY > 0)
			window->AutoFitFramesY--;

		if (want_focus)
			FocusWindow(window);

		if (!(flags & ImGuiWindowFlags_NoTitleBar))
		{
			if (!(flags & ImGuiWindowFlags_NoCollapse))
			{
				RenderTriangle(window->Pos + style.FramePadding, window->Collapsed ? ImGuiDir_Right : ImGuiDir_Down, 1.0f);
			}

			ImVec2 text_size = CalcTextSize(name, NULL, true);
			ImRect text_r = title_bar_rect;
			float pad_left = (flags & ImGuiWindowFlags_NoCollapse) == 0 ? (style.FramePadding.x + g.FontSize + style.ItemInnerSpacing.x) : style.FramePadding.x;
			float pad_right = style.FramePadding.x;
			if (style.WindowTitleAlign.x > 0.0f) pad_right = ImLerp(pad_right, pad_left, style.WindowTitleAlign.x);
			text_r.Min.x += pad_left;
			text_r.Max.x -= pad_right;
			ImRect clip_rect = text_r;
			clip_rect.Max.x = window->Pos.x + window->Size.x - style.FramePadding.x;
			RenderTextClipped(text_r.Min, text_r.Max, name, NULL, &text_size, style.WindowTitleAlign, &clip_rect);
		}

		window->WindowRectClipped = window->Rect();
		window->WindowRectClipped.ClipWith(window->ClipRect);
		window->InnerRect.Min.x = title_bar_rect.Min.x + window->WindowBorderSize;
		window->InnerRect.Min.y = title_bar_rect.Max.y + window->MenuBarHeight() + (((flags & ImGuiWindowFlags_MenuBar) || !(flags & ImGuiWindowFlags_NoTitleBar)) ? style.FrameBorderSize : window->WindowBorderSize);
		window->InnerRect.Max.x = window->Pos.x + window->Size.x - window->ScrollbarSizes.x - window->WindowBorderSize;
		window->InnerRect.Max.y = window->Pos.y + window->Size.y - window->ScrollbarSizes.y - window->WindowBorderSize;
		window->DC.LastItemId = window->MoveId;
		window->DC.LastItemRect = title_bar_rect;
		window->DC.LastItemRectHoveredRect = IsMouseHoveringRect(title_bar_rect.Min, title_bar_rect.Max, false);
	}
	const float border_size = window->WindowBorderSize;
	ImRect clip_rect;
	clip_rect.Min.x = ImFloor(0.5f + window->InnerRect.Min.x + ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - border_size)));
	clip_rect.Min.y = ImFloor(0.5f + window->InnerRect.Min.y);
	clip_rect.Max.x = ImFloor(0.5f + window->InnerRect.Max.x - ImMax(0.0f, ImFloor(window->WindowPadding.x*0.5f - border_size)));
	clip_rect.Max.y = ImFloor(0.5f + window->InnerRect.Max.y);
	PushClipRect(clip_rect.Min, clip_rect.Max, true);

	if (first_begin_of_the_frame)
		window->WriteAccessed = false;

	window->BeginCount++;
	g.NextWindowData.SizeConstraintCond = 0;

	if (flags & ImGuiWindowFlags_ChildWindow)
	{
		window->Collapsed = parent_window && parent_window->Collapsed;

		if (!(flags & ImGuiWindowFlags_AlwaysAutoResize) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0)
			window->Collapsed |= (window->WindowRectClipped.Min.x >= window->WindowRectClipped.Max.x || window->WindowRectClipped.Min.y >= window->WindowRectClipped.Max.y);

		if (window->Collapsed)
			window->Active = false;
	}
	if (style.Alpha <= 0.0f)
		window->Active = false;

	window->SkipItems = (window->Collapsed || !window->Active) && window->AutoFitFramesX <= 0 && window->AutoFitFramesY <= 0;
	return !window->SkipItems;
}

void ImGui::End()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if (window->DC.ColumnsSet != NULL)
		EndColumns();
	PopClipRect();
	g.CurrentWindowStack.pop_back();
	if (window->Flags & ImGuiWindowFlags_Popup)
		g.CurrentPopupStack.pop_back();
	CheckStacksSize(window, false);
	SetCurrentWindow(g.CurrentWindowStack.empty() ? NULL : g.CurrentWindowStack.back());
}

void ImGui::Scrollbar(ImGuiLayoutType direction)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	const bool horizontal = (direction == ImGuiLayoutType_Horizontal);
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(horizontal ? XorString("#SCROLLX") : XorString("#SCROLLY"));
	bool other_scrollbar = (horizontal ? window->ScrollbarY : window->ScrollbarX);
	float other_scrollbar_size_w = other_scrollbar ? style.ScrollbarSize : 0.0f;
	const ImRect window_rect = window->Rect();
	const float border_size = window->WindowBorderSize;
	ImRect bb = horizontal
		? ImRect(window->Pos.x + border_size, window_rect.Max.y - style.ScrollbarSize, window_rect.Max.x - other_scrollbar_size_w - border_size, window_rect.Max.y - border_size)
		: ImRect(window_rect.Max.x - style.ScrollbarSize, window->Pos.y + border_size, window_rect.Max.x - border_size, window_rect.Max.y - other_scrollbar_size_w - border_size);
	if (!horizontal)
		bb.Min.y += window->TitleBarHeight() + ((window->Flags & ImGuiWindowFlags_MenuBar) ? window->MenuBarHeight() : 0.0f);
	if (bb.GetWidth() <= 0.0f || bb.GetHeight() <= 0.0f)
		return;

	int window_rounding_corners;
	if (horizontal)
		window_rounding_corners = ImDrawCornerFlags_BotLeft | (other_scrollbar ? 0 : ImDrawCornerFlags_BotRight);
	else
		window_rounding_corners = (((window->Flags & ImGuiWindowFlags_NoTitleBar) && !(window->Flags & ImGuiWindowFlags_MenuBar)) ? ImDrawCornerFlags_TopRight : 0) | (other_scrollbar ? 0 : ImDrawCornerFlags_BotRight);
	window->DrawList->AddRectFilled(bb.Min, bb.Max, GetColorU32(ImGuiCol_ScrollbarBg), window->WindowRounding, window_rounding_corners);
	bb.Expand(ImVec2(-ImClamp((float)(int)((bb.Max.x - bb.Min.x - 2.0f) * 0.5f), 0.0f, 3.0f), -ImClamp((float)(int)((bb.Max.y - bb.Min.y - 2.0f) * 0.5f), 0.0f, 3.0f)));

	float scrollbar_size_v = horizontal ? bb.GetWidth() : bb.GetHeight();
	float scroll_v = horizontal ? window->Scroll.x : window->Scroll.y;
	float win_size_avail_v = (horizontal ? window->SizeFull.x : window->SizeFull.y) - other_scrollbar_size_w;
	float win_size_contents_v = horizontal ? window->SizeContents.x : window->SizeContents.y;

	const float win_size_v = ImMax(ImMax(win_size_contents_v, win_size_avail_v), 1.0f);
	const float grab_h_pixels = ImClamp(scrollbar_size_v * (win_size_avail_v / win_size_v), style.GrabMinSize, scrollbar_size_v);
	const float grab_h_norm = grab_h_pixels / scrollbar_size_v;

	bool held = false;
	bool hovered = false;
	const bool previously_held = (g.ActiveId == id);
	ButtonBehavior(bb, id, &hovered, &held);

	float scroll_max = ImMax(1.0f, win_size_contents_v - win_size_avail_v);
	float scroll_ratio = ImSaturate(scroll_v / scroll_max);
	float grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;
	if (held && grab_h_norm < 1.0f)
	{
		float scrollbar_pos_v = horizontal ? bb.Min.x : bb.Min.y;
		float mouse_pos_v = horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
		float* click_delta_to_grab_center_v = horizontal ? &g.ScrollbarClickDeltaToGrabCenter.x : &g.ScrollbarClickDeltaToGrabCenter.y;
		const float clicked_v_norm = ImSaturate((mouse_pos_v - scrollbar_pos_v) / scrollbar_size_v);
		SetHoveredID(id);

		bool seek_absolute = false;
		if (!previously_held)
		{
			if (clicked_v_norm >= grab_v_norm && clicked_v_norm <= grab_v_norm + grab_h_norm)
			{
				*click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
			}
			else
			{
				seek_absolute = true;
				*click_delta_to_grab_center_v = 0.0f;
			}
		}
		const float scroll_v_norm = ImSaturate((clicked_v_norm - *click_delta_to_grab_center_v - grab_h_norm * 0.5f) / (1.0f - grab_h_norm));
		scroll_v = (float)(int)(0.5f + scroll_v_norm * scroll_max);
		if (horizontal)
			window->Scroll.x = scroll_v;
		else
			window->Scroll.y = scroll_v;

		scroll_ratio = ImSaturate(scroll_v / scroll_max);
		grab_v_norm = scroll_ratio * (scrollbar_size_v - grab_h_pixels) / scrollbar_size_v;

		if (seek_absolute)
			*click_delta_to_grab_center_v = clicked_v_norm - grab_v_norm - grab_h_norm * 0.5f;
	}

	const ImU32 grab_col = GetColorU32(held ? ImGuiCol_ScrollbarGrabActive : hovered ? ImGuiCol_ScrollbarGrabHovered : ImGuiCol_ScrollbarGrab);
	ImRect grab_rect;
	if (horizontal)
		grab_rect = ImRect(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm), bb.Min.y, ImMin(ImLerp(bb.Min.x, bb.Max.x, grab_v_norm) + grab_h_pixels, window_rect.Max.x), bb.Max.y);
	else
		grab_rect = ImRect(bb.Min.x, ImLerp(bb.Min.y, bb.Max.y, grab_v_norm), bb.Max.x, ImMin(ImLerp(bb.Min.y, bb.Max.y, grab_v_norm) + grab_h_pixels, window_rect.Max.y));
	window->DrawList->AddRectFilled(grab_rect.Min, grab_rect.Max, grab_col, style.ScrollbarRounding);
}

void ImGui::BringWindowToFront(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	if (g.Windows.back() == window)
		return;
	for (int i = g.Windows.Size - 2; i >= 0; i--)
		if (g.Windows[i] == window)
		{
			g.Windows.erase(g.Windows.Data + i);
			g.Windows.push_back(window);
			break;
		}
}

void ImGui::BringWindowToBack(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	if (g.Windows[0] == window)
		return;
	for (int i = 0; i < g.Windows.Size; i++)
		if (g.Windows[i] == window)
		{
			memmove(&g.Windows[1], &g.Windows[0], (size_t)i * sizeof(ImGuiWindow*));
			g.Windows[0] = window;
			break;
		}
}

void ImGui::FocusWindow(ImGuiWindow* window)
{
	ImGuiContext& g = *GImGui;
	g.NavWindow = window;
	if (!window)
		return;
	if (window->RootWindow)
		window = window->RootWindow;
	if (window->Flags & ImGuiWindowFlags_Popup)
		if (g.ActiveId != 0 && g.ActiveIdWindow && g.ActiveIdWindow->RootWindow != window)
			ClearActiveID();
	if (!(window->Flags & ImGuiWindowFlags_NoBringToFrontOnFocus))
		BringWindowToFront(window);
}

void ImGui::FocusFrontMostActiveWindow(ImGuiWindow* ignore_window)
{
	ImGuiContext& g = *GImGui;
	for (int i = g.Windows.Size - 1; i >= 0; i--)
		if (g.Windows[i] != ignore_window && g.Windows[i]->WasActive && !(g.Windows[i]->Flags & ImGuiWindowFlags_ChildWindow))
		{
			FocusWindow(g.Windows[i]);
			return;
		}
}

void ImGui::PushItemWidth(float item_width)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ItemWidth = (item_width == 0.0f ? window->ItemWidthDefault : item_width);
	window->DC.ItemWidthStack.push_back(window->DC.ItemWidth);
}

void ImGui::PushMultiItemsWidths(int components, float w_full)
{
	ImGuiWindow* window = GetCurrentWindow();
	const ImGuiStyle& style = GImGui->Style;
	if (w_full <= 0.0f)
		w_full = CalcItemWidth();
	const float w_item_one = ImMax(1.0f, (float)(int)((w_full - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
	const float w_item_last = ImMax(1.0f, (float)(int)(w_full - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));
	window->DC.ItemWidthStack.push_back(w_item_last);
	for (int i = 0; i < components - 1; i++)
		window->DC.ItemWidthStack.push_back(w_item_one);
	window->DC.ItemWidth = window->DC.ItemWidthStack.back();
}

void ImGui::PopItemWidth()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.ItemWidthStack.pop_back();
	window->DC.ItemWidth = window->DC.ItemWidthStack.empty() ? window->ItemWidthDefault : window->DC.ItemWidthStack.back();
}

float ImGui::CalcItemWidth()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	float w = window->DC.ItemWidth;
	if (w < 0.0f)
	{
		float width_to_right_edge = GetContentRegionAvail().x;
		w = ImMax(1.0f, width_to_right_edge + w);
	}
	w = (float)(int)w;
	return w;
}

ImFont* GetDefaultFont()
{
	ImGuiContext& g = *GImGui;
	return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0];
}

void SetCurrentFont(ImFont* font)
{
	ImGuiContext& g = *GImGui;
	g.Font = font;
	g.FontBaseSize = g.IO.FontGlobalScale * g.Font->FontSize * g.Font->Scale;
	g.FontSize = g.CurrentWindow ? g.CurrentWindow->CalcFontSize() : 0.0f;

	ImFontAtlas* atlas = g.Font->ContainerAtlas;
	g.DrawListSharedData.TexUvWhitePixel = atlas->TexUvWhitePixel;
	g.DrawListSharedData.Font = g.Font;
	g.DrawListSharedData.FontSize = g.FontSize;
}

void ImGui::PushFont(ImFont* font)
{
	ImGuiContext& g = *GImGui;
	if (!font)
		font = GetDefaultFont();
	SetCurrentFont(font);
	g.FontStack.push_back(font);
	g.CurrentWindow->DrawList->PushTextureID(font->ContainerAtlas->TexID);
}

void ImGui::PopFont()
{
	ImGuiContext& g = *GImGui;
	g.CurrentWindow->DrawList->PopTextureID();
	g.FontStack.pop_back();
	SetCurrentFont(g.FontStack.empty() ? GetDefaultFont() : g.FontStack.back());
}

void ImGui::PushTextWrapPos(float wrap_pos_x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.TextWrapPos = wrap_pos_x;
	window->DC.TextWrapPosStack.push_back(wrap_pos_x);
}

void ImGui::PopTextWrapPos()
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.TextWrapPosStack.pop_back();
	window->DC.TextWrapPos = window->DC.TextWrapPosStack.empty() ? -1.0f : window->DC.TextWrapPosStack.back();
}

bool ImGui::IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent)
{
	if (window->RootWindow == potential_parent)
		return true;
	while (window != NULL)
	{
		if (window == potential_parent)
			return true;
		window = window->ParentWindow;
	}
	return false;
}

bool ImGui::IsWindowHovered(ImGuiHoveredFlags flags)
{
	ImGuiContext& g = *GImGui;
	switch (flags & (ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows))
	{
	case ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows:
		if (g.HoveredRootWindow != g.CurrentWindow->RootWindow)
			return false;
		break;
	case ImGuiHoveredFlags_RootWindow:
		if (g.HoveredWindow != g.CurrentWindow->RootWindow)
			return false;
		break;
	case ImGuiHoveredFlags_ChildWindows:
		if (g.HoveredWindow == NULL || !IsWindowChildOf(g.HoveredWindow, g.CurrentWindow))
			return false;
		break;
	default:
		if (g.HoveredWindow != g.CurrentWindow)
			return false;
		break;
	}

	if (!IsWindowContentHoverable(g.HoveredRootWindow, flags))
		return false;
	if (!(flags & ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
		if (g.ActiveId != 0 && !g.ActiveIdAllowOverlap && g.ActiveId != g.HoveredWindow->MoveId)
			return false;
	return true;
}

bool ImGui::IsWindowFocused(ImGuiFocusedFlags flags)
{
	ImGuiContext& g = *GImGui;

	switch (flags & (ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows))
	{
	case ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows:
		return g.NavWindow && g.CurrentWindow->RootWindow == g.NavWindow->RootWindow;
	case ImGuiFocusedFlags_RootWindow:
		return g.CurrentWindow->RootWindow == g.NavWindow;
	case ImGuiFocusedFlags_ChildWindows:
		return g.NavWindow && IsWindowChildOf(g.NavWindow, g.CurrentWindow);
	default:
		return g.CurrentWindow == g.NavWindow;
	}
}

float ImGui::GetWindowWidth()
{
	ImGuiWindow* window = GImGui->CurrentWindow;
	return window->Size.x;
}

float ImGui::GetWindowHeight()
{
	ImGuiWindow* window = GImGui->CurrentWindow;
	return window->Size.y;
}

ImVec2 ImGui::GetWindowPos()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	return window->Pos;
}

void SetWindowScrollY(ImGuiWindow* window, float new_scroll_y)
{
	window->DC.CursorMaxPos.y += window->Scroll.y;
	window->Scroll.y = new_scroll_y;
	window->DC.CursorMaxPos.y -= window->Scroll.y;
}

void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond)
{
	if (cond && (window->SetWindowPosAllowFlags & cond) == 0)
		return;
	window->SetWindowPosAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
	window->SetWindowPosVal = ImVec2(FLT_MAX, FLT_MAX);
	const ImVec2 old_pos = window->Pos;
	window->PosFloat = pos;
	window->Pos = ImVec2((float)(int)window->PosFloat.x, (float)(int)window->PosFloat.y);
	window->DC.CursorPos += (window->Pos - old_pos);
	window->DC.CursorMaxPos += (window->Pos - old_pos);
}

void ImGui::SetWindowPos(const ImVec2& pos, ImGuiCond cond)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	SetWindowPos(window, pos, cond);
}

void ImGui::SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond)
{
	if (ImGuiWindow* window = FindWindowByName(name))
		SetWindowPos(window, pos, cond);
}

ImVec2 ImGui::GetWindowSize()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->Size;
}

void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond)
{
	if (cond && (window->SetWindowSizeAllowFlags & cond) == 0)
		return;
	window->SetWindowSizeAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
	if (size.x > 0.0f)
	{
		window->AutoFitFramesX = 0;
		window->SizeFull.x = size.x;
	}
	else
	{
		window->AutoFitFramesX = 2;
		window->AutoFitOnlyGrows = false;
	}
	if (size.y > 0.0f)
	{
		window->AutoFitFramesY = 0;
		window->SizeFull.y = size.y;
	}
	else
	{
		window->AutoFitFramesY = 2;
		window->AutoFitOnlyGrows = false;
	}
}

void ImGui::SetWindowSize(const ImVec2& size, ImGuiCond cond)
{
	SetWindowSize(GImGui->CurrentWindow, size, cond);
}

void ImGui::SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond)
{
	if (ImGuiWindow* window = FindWindowByName(name))
		SetWindowSize(window, size, cond);
}

void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond)
{
	if (cond && (window->SetWindowCollapsedAllowFlags & cond) == 0)
		return;
	window->SetWindowCollapsedAllowFlags &= ~(ImGuiCond_Once | ImGuiCond_FirstUseEver | ImGuiCond_Appearing);
	window->Collapsed = collapsed;
}

void ImGui::SetWindowCollapsed(bool collapsed, ImGuiCond cond)
{
	SetWindowCollapsed(GImGui->CurrentWindow, collapsed, cond);
}

bool ImGui::IsWindowCollapsed()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->Collapsed;
}

bool ImGui::IsWindowAppearing()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->Appearing;
}

void ImGui::SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond)
{
	if (ImGuiWindow* window = FindWindowByName(name))
		SetWindowCollapsed(window, collapsed, cond);
}

void ImGui::SetWindowFocus()
{
	FocusWindow(GImGui->CurrentWindow);
}

void ImGui::SetWindowFocus(const char* name)
{
	if (name)
	{
		if (ImGuiWindow* window = FindWindowByName(name))
			FocusWindow(window);
	}
	else
	{
		FocusWindow(NULL);
	}
}

void ImGui::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.PosVal = pos;
	g.NextWindowData.PosPivotVal = pivot;
	g.NextWindowData.PosCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.SizeVal = size;
	g.NextWindowData.SizeCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback, void* custom_callback_user_data)
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.SizeConstraintCond = ImGuiCond_Always;
	g.NextWindowData.SizeConstraintRect = ImRect(size_min, size_max);
	g.NextWindowData.SizeCallback = custom_callback;
	g.NextWindowData.SizeCallbackUserData = custom_callback_user_data;
}

void ImGui::SetNextWindowContentSize(const ImVec2& size)
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.ContentSizeVal = size;
	g.NextWindowData.ContentSizeCond = ImGuiCond_Always;
}

void ImGui::SetNextWindowCollapsed(bool collapsed, ImGuiCond cond)
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.CollapsedVal = collapsed;
	g.NextWindowData.CollapsedCond = cond ? cond : ImGuiCond_Always;
}

void ImGui::SetNextWindowFocus()
{
	ImGuiContext& g = *GImGui;
	g.NextWindowData.FocusCond = ImGuiCond_Always;
}

ImVec2 ImGui::GetContentRegionMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImVec2 mx = window->ContentsRegionRect.Max;
	if (window->DC.ColumnsSet)
		mx.x = GetColumnOffset(window->DC.ColumnsSet->Current + 1) - window->WindowPadding.x;
	return mx;
}

ImVec2 ImGui::GetContentRegionAvail()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return GetContentRegionMax() - (window->DC.CursorPos - window->Pos);
}

float ImGui::GetContentRegionAvailWidth()
{
	return GetContentRegionAvail().x;
}

ImVec2 ImGui::GetWindowContentRegionMin()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Min;
}

ImVec2 ImGui::GetWindowContentRegionMax()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Max;
}

float ImGui::GetWindowContentRegionWidth()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ContentsRegionRect.Max.x - window->ContentsRegionRect.Min.x;
}

float ImGui::GetTextLineHeight()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize;
}

float ImGui::GetTextLineHeightWithSpacing()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + g.Style.ItemSpacing.y;
}

float ImGui::GetFrameHeight()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + g.Style.FramePadding.y * 2.0f;
}

float ImGui::GetFrameHeightWithSpacing()
{
	ImGuiContext& g = *GImGui;
	return g.FontSize + g.Style.FramePadding.y * 2.0f + g.Style.ItemSpacing.y;
}

ImDrawList* ImGui::GetWindowDrawList()
{
	ImGuiWindow* window = GetCurrentWindow();
	return window->DrawList;
}

ImFont* ImGui::GetFont()
{
	return GImGui->Font;
}

float ImGui::GetFontSize()
{
	return GImGui->FontSize;
}

ImVec2 ImGui::GetFontTexUvWhitePixel()
{
	return GImGui->DrawListSharedData.TexUvWhitePixel;
}

void ImGui::SetWindowFontScale(float scale)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->FontWindowScale = scale;
	g.FontSize = g.DrawListSharedData.FontSize = window->CalcFontSize();
}

ImVec2 ImGui::GetCursorPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos - window->Pos + window->Scroll;
}

float ImGui::GetCursorPosX()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos.x - window->Pos.x + window->Scroll.x;
}

float ImGui::GetCursorPosY()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos.y - window->Pos.y + window->Scroll.y;
}

void ImGui::SetCursorPos(const ImVec2& local_pos)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos = window->Pos - window->Scroll + local_pos;
	window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

void ImGui::SetCursorPosX(float x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + x;
	window->DC.CursorMaxPos.x = ImMax(window->DC.CursorMaxPos.x, window->DC.CursorPos.x);
}

void ImGui::SetCursorPosY(float y)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos.y = window->Pos.y - window->Scroll.y + y;
	window->DC.CursorMaxPos.y = ImMax(window->DC.CursorMaxPos.y, window->DC.CursorPos.y);
}

ImVec2 ImGui::GetCursorStartPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorStartPos - window->Pos;
}

ImVec2 ImGui::GetCursorScreenPos()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.CursorPos;
}

void ImGui::SetCursorScreenPos(const ImVec2& screen_pos)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.CursorPos = screen_pos;
	window->DC.CursorMaxPos = ImMax(window->DC.CursorMaxPos, window->DC.CursorPos);
}

float ImGui::GetScrollX()
{
	return GImGui->CurrentWindow->Scroll.x;
}

float ImGui::GetScrollY()
{
	return GImGui->CurrentWindow->Scroll.y;
}

float ImGui::GetScrollMaxX()
{
	return GetScrollMaxX(GImGui->CurrentWindow);
}

float ImGui::GetScrollMaxY()
{
	return GetScrollMaxY(GImGui->CurrentWindow);
}

void ImGui::SetScrollX(float scroll_x)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->ScrollTarget.x = scroll_x;
	window->ScrollTargetCenterRatio.x = 0.0f;
}

void ImGui::SetScrollY(float scroll_y)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->ScrollTarget.y = scroll_y + window->TitleBarHeight() + window->MenuBarHeight();
	window->ScrollTargetCenterRatio.y = 0.0f;
}

void ImGui::SetScrollFromPosY(float pos_y, float center_y_ratio)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->ScrollTarget.y = (float)(int)(pos_y + window->Scroll.y);
	window->ScrollTargetCenterRatio.y = center_y_ratio;
	if (center_y_ratio <= 0.0f && window->ScrollTarget.y <= window->WindowPadding.y)
		window->ScrollTarget.y = 0.0f;
	else if (center_y_ratio >= 1.0f && window->ScrollTarget.y >= window->SizeContents.y - window->WindowPadding.y + GImGui->Style.ItemSpacing.y)
		window->ScrollTarget.y = window->SizeContents.y;
}

void ImGui::SetScrollHere(float center_y_ratio)
{
	ImGuiWindow* window = GetCurrentWindow();
	float target_y = window->DC.CursorPosPrevLine.y - window->Pos.y;
	target_y += (window->DC.PrevLineHeight * center_y_ratio) + (GImGui->Style.ItemSpacing.y * (center_y_ratio - 0.5f) * 2.0f);
	SetScrollFromPosY(target_y, center_y_ratio);
}

void ImGui::SetItemDefaultFocus()
{
	if (IsWindowAppearing())
		SetScrollHere();
}

void ImGui::SetKeyboardFocusHere(int offset)
{
	ImGuiWindow* window = GetCurrentWindow();
	window->FocusIdxAllRequestNext = window->FocusIdxAllCounter + 1 + offset;
	window->FocusIdxTabRequestNext = INT_MAX;
}

void ImGui::TextV(const char* fmt, va_list args)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	TextUnformatted(g.TempBuffer, text_end);
}

void ImGui::Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextV(fmt, args);
	va_end(args);
}

void ImGui::TextUnformatted(const char* text, const char* text_end)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const char* text_begin = text;
	if (text_end == NULL)
		text_end = text + strlen(text);

	const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);
	const float wrap_pos_x = window->DC.TextWrapPos;
	const bool wrap_enabled = wrap_pos_x >= 0.0f;
	if (text_end - text > 2000 && !wrap_enabled)
	{
		const char* line = text;
		const float line_height = GetTextLineHeight();
		const ImRect clip_rect = window->ClipRect;
		ImVec2 text_size(0, 0);

		if (text_pos.y <= clip_rect.Max.y)
		{
			ImVec2 pos = text_pos;

			int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
			if (lines_skippable > 0)
			{
				int lines_skipped = 0;
				while (line < text_end && lines_skipped < lines_skippable)
				{
					const char* line_end = strchr(line, '\n');
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					lines_skipped++;
				}
				pos.y += lines_skipped * line_height;
			}

			if (line < text_end)
			{
				ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (IsClippedEx(line_rect, 0, false))
						break;

					const ImVec2 line_size = CalcTextSize(line, line_end, false);
					text_size.x = ImMax(text_size.x, line_size.x);
					RenderText(pos, line, line_end, false);
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					line_rect.Min.y += line_height;
					line_rect.Max.y += line_height;
					pos.y += line_height;
				}

				int lines_skipped = 0;
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					lines_skipped++;
				}
				pos.y += lines_skipped * line_height;
			}

			text_size.y += (pos - text_pos).y;
		}

		ImRect bb(text_pos, text_pos + text_size);
		ItemSize(bb);
		ItemAdd(bb, 0);
	}
	else
	{
		const float wrap_width = wrap_enabled ? CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
		const ImVec2 text_size = CalcTextSize(text_begin, text_end, false, wrap_width);

		ImRect bb(text_pos, text_pos + text_size);
		ItemSize(text_size);
		if (!ItemAdd(bb, 0))
			return;

		RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
	}
}

bool ImGui::ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();

	if (flags & ImGuiButtonFlags_Disabled)
	{
		if (out_hovered) *out_hovered = false;
		if (out_held) *out_held = false;
		if (g.ActiveId == id) ClearActiveID();
		return false;
	}

	if ((flags & (ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick)) == 0)
		flags |= ImGuiButtonFlags_PressedOnClickRelease;

	ImGuiWindow* backup_hovered_window = g.HoveredWindow;
	if ((flags & ImGuiButtonFlags_FlattenChildren) && g.HoveredRootWindow == window)
		g.HoveredWindow = window;

	bool pressed = false;
	bool hovered = ItemHoverable(bb, id);

	if ((flags & ImGuiButtonFlags_FlattenChildren) && g.HoveredRootWindow == window)
		g.HoveredWindow = backup_hovered_window;

	if (hovered && (flags & ImGuiButtonFlags_AllowItemOverlap) && (g.HoveredIdPreviousFrame != id && g.HoveredIdPreviousFrame != 0))
		hovered = false;

	if (hovered)
	{
		if (!(flags & ImGuiButtonFlags_NoKeyModifiers) || !g.IO.KeyShift)
		{
			if ((flags & ImGuiButtonFlags_PressedOnClickRelease) && g.IO.MouseClicked[0])
			{
				SetActiveID(id, window);
				FocusWindow(window);
			}
			if (((flags & ImGuiButtonFlags_PressedOnClick) && g.IO.MouseClicked[0]) || ((flags & ImGuiButtonFlags_PressedOnDoubleClick) && g.IO.MouseDoubleClicked[0]))
			{
				pressed = true;
				if (flags & ImGuiButtonFlags_NoHoldingActiveID)
					ClearActiveID();
				else
					SetActiveID(id, window);
				FocusWindow(window);
			}
			if ((flags & ImGuiButtonFlags_PressedOnRelease) && g.IO.MouseReleased[0])
			{
				if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))
					pressed = true;
				ClearActiveID();
			}

			if ((flags & ImGuiButtonFlags_Repeat) && g.ActiveId == id && g.IO.MouseDownDuration[0] > 0.0f && IsMouseClicked(0, true))
				pressed = true;
		}
	}

	bool held = false;
	if (g.ActiveId == id)
	{
		if (g.ActiveIdIsJustActivated)
			g.ActiveIdClickOffset = g.IO.MousePos - bb.Min;
		if (g.IO.MouseDown[0])
		{
			held = true;
		}
		else
		{
			if (hovered && (flags & ImGuiButtonFlags_PressedOnClickRelease))
				if (!((flags & ImGuiButtonFlags_Repeat) && g.IO.MouseDownDurationPrev[0] >= g.IO.KeyRepeatDelay))
					pressed = true;
			ClearActiveID();
		}
	}

	if (out_hovered) *out_hovered = hovered;
	if (out_held) *out_held = held;

	return pressed;
}

bool ImGui::ButtonEx(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ItemSize(bb, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat) flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

	return pressed;
}

bool ImGui::Button(const char* label, const ImVec2& size_arg)
{
	return ButtonEx(label, size_arg, 0);
}

void ImGui::PushID(const char* str_id)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	window->IDStack.push_back(window->GetID(str_id));
}

void ImGui::PushID(const void* ptr_id)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PushID(int int_id)
{
	const void* ptr_id = (void*)(intptr_t)int_id;
	ImGuiWindow* window = GetCurrentWindowRead();
	window->IDStack.push_back(window->GetID(ptr_id));
}

void ImGui::PopID()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	window->IDStack.pop_back();
}

ImGuiID ImGui::GetID(const char* str_id)
{
	return GImGui->CurrentWindow->GetID(str_id);
}

ImGuiID ImGui::GetID(const char* str_id_begin, const char* str_id_end)
{
	return GImGui->CurrentWindow->GetID(str_id_begin, str_id_end);
}

ImGuiID ImGui::GetID(const void* ptr_id)
{
	return GImGui->CurrentWindow->GetID(ptr_id);
}

inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, const char* display_format, char* buf, int buf_size)
{
	if (data_type == ImGuiDataType_Int) ImFormatString(buf, buf_size, display_format, *(int *)data_ptr);
	else if (data_type == ImGuiDataType_Float) ImFormatString(buf, buf_size, display_format, *(float *)data_ptr);
};

inline void DataTypeFormatString(ImGuiDataType data_type, void* data_ptr, int decimal_precision, char* buf, int buf_size)
{
	if (data_type == ImGuiDataType_Int)
	{
		if (decimal_precision < 0) xprintf(buf, buf_size, XorString("%d"), *(int*)data_ptr);
		else xprintf(buf, buf_size, XorString("%.*d"), decimal_precision, *(int*)data_ptr);
	}
	else if (data_type == ImGuiDataType_Float)
	{
		if (decimal_precision < 0) xprintf(buf, buf_size, XorString("%f"), *(float*)data_ptr);
		else xprintf(buf, buf_size, XorString("%.*f"), decimal_precision, *(float*)data_ptr);
	}
}

void DataTypeApplyOp(ImGuiDataType data_type, int op, void* value1, const void* value2)
{
	if (data_type == ImGuiDataType_Int)
	{
		if (op == '+') *(int *)value1 = *(int *)value1 + *(const int *)value2;
		else if (op == '-') *(int *)value1 = *(int *)value1 - *(const int *)value2;
	}
	else if (data_type == ImGuiDataType_Float)
	{
		if (op == '+') *(float *)value1 = *(float *)value1 + *(const float *)value2;
		else if (op == '-') *(float *)value1 = *(float *)value1 - *(const float *)value2;
	};
};

bool DataTypeApplyOpFromText(const char * buf, void * data_ptr, const char * scalar_format)
{
	while (ImCharIsSpace(*buf)) buf++;
	if (!buf[0]) return false;
	int * v = (int *)data_ptr;
	const int old_v = *v;
	int arg0i = *v;
	if (sscanf(buf, scalar_format, &arg0i) == 1) *v = arg0i;
	return (old_v != *v);
};

int ImGui::ParseFormatPrecision(const char* fmt, int default_precision)
{
	int precision = default_precision;
	while ((fmt = strchr(fmt, '%')) != NULL)
	{
		fmt++;
		if (fmt[0] == '%') { fmt++; continue; }
		while (*fmt >= '0' && *fmt <= '9')
			fmt++;
		if (*fmt == '.')
		{
			fmt = ImAtoi(fmt + 1, &precision);
			if (precision < 0 || precision > 10)
				precision = default_precision;
		}
		if (*fmt == 'e' || *fmt == 'E')
			precision = -1;
		break;
	}
	return precision;
}

float GetMinimumStepAtDecimalPrecision(int decimal_precision)
{
	static const float min_steps[10] = { 1.0f, 0.1f, 0.01f, 0.001f, 0.0001f, 0.00001f, 0.000001f, 0.0000001f, 0.00000001f, 0.000000001f };
	return (decimal_precision >= 0 && decimal_precision < 10) ? min_steps[decimal_precision] : powf(10.0f, (float)-decimal_precision);
}

float ImGui::RoundScalar(float value, int decimal_precision)
{
	if (decimal_precision < 0)
		return value;
	const float min_step = GetMinimumStepAtDecimalPrecision(decimal_precision);
	bool negative = value < 0.0f;
	value = fabsf(value);
	float remainder = fmodf(value, min_step);
	if (remainder <= min_step * 0.5f)
		value -= remainder;
	else
		value += (min_step - remainder);
	return negative ? -value : value;
}

inline float SliderBehaviorCalcRatioFromValue(float v, float v_min, float v_max, float power, float linear_zero_pos)
{
	if (v_min == v_max)
		return 0.0f;

	const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
	const float v_clamped = (v_min < v_max) ? ImClamp(v, v_min, v_max) : ImClamp(v, v_max, v_min);
	if (is_non_linear)
	{
		if (v_clamped < 0.0f)
		{
			const float f = 1.0f - (v_clamped - v_min) / (ImMin(0.0f, v_max) - v_min);
			return (1.0f - powf(f, 1.0f / power)) * linear_zero_pos;
		}
		else
		{
			const float f = (v_clamped - ImMax(0.0f, v_min)) / (v_max - ImMax(0.0f, v_min));
			return linear_zero_pos + powf(f, 1.0f / power) * (1.0f - linear_zero_pos);
		}
	}

	return (v_clamped - v_min) / (v_max - v_min);
}

bool ImGui::SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	const ImGuiStyle& style = g.Style;

	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

	const bool is_non_linear = (power < 1.0f - 0.00001f) || (power > 1.0f + 0.00001f);
	const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;

	const float grab_padding = 2.0f;
	const float slider_sz = is_horizontal ? (frame_bb.GetWidth() - grab_padding * 2.0f) : (frame_bb.GetHeight() - grab_padding * 2.0f);
	float grab_sz;
	if (decimal_precision != 0)
		grab_sz = ImMin(style.GrabMinSize, slider_sz);
	else
		grab_sz = ImMin(ImMax(1.0f * (slider_sz / ((v_min < v_max ? v_max - v_min : v_min - v_max) + 1.0f)), style.GrabMinSize), slider_sz);
	const float slider_usable_sz = slider_sz - grab_sz;
	const float slider_usable_pos_min = (is_horizontal ? frame_bb.Min.x : frame_bb.Min.y) + grab_padding + grab_sz * 0.5f;
	const float slider_usable_pos_max = (is_horizontal ? frame_bb.Max.x : frame_bb.Max.y) - grab_padding - grab_sz * 0.5f;

	float linear_zero_pos = 0.0f;
	if (v_min * v_max < 0.0f)
	{
		const float linear_dist_min_to_0 = powf(fabsf(0.0f - v_min), 1.0f / power);
		const float linear_dist_max_to_0 = powf(fabsf(v_max - 0.0f), 1.0f / power);
		linear_zero_pos = linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0);
	}
	else
		linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;

	bool value_changed = false;
	if (g.ActiveId == id)
	{
		bool set_new_value = false;
		float clicked_t = 0.0f;
		if (g.IO.MouseDown[0])
		{
			const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
			clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
			if (!is_horizontal)
				clicked_t = 1.0f - clicked_t;
			set_new_value = true;
		}
		else
			ClearActiveID();

		if (set_new_value)
		{
			float new_value;
			if (is_non_linear)
			{
				if (clicked_t < linear_zero_pos)
				{
					float a = 1.0f - (clicked_t / linear_zero_pos);
					a = powf(a, power);
					new_value = ImLerp(ImMin(v_max, 0.0f), v_min, a);
				}
				else
				{
					float a;
					if (fabsf(linear_zero_pos - 1.0f) > 1.e-6f)
						a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
					else
						a = clicked_t;
					a = powf(a, power);
					new_value = ImLerp(ImMax(v_min, 0.0f), v_max, a);
				}
			}
			else
			{
				new_value = ImLerp(v_min, v_max, clicked_t);
			}

			new_value = RoundScalar(new_value, decimal_precision);
			if (*v != new_value)
			{
				*v = new_value;
				value_changed = true;
			}
		}
	}

	float grab_t = SliderBehaviorCalcRatioFromValue(*v, v_min, v_max, power, linear_zero_pos);
	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(ImVec2(grab_pos - grab_sz * 0.5f, frame_bb.Min.y + grab_padding), ImVec2(grab_pos + grab_sz * 0.5f, frame_bb.Max.y - grab_padding));
	else
		grab_bb = ImRect(ImVec2(frame_bb.Min.x + grab_padding, grab_pos - grab_sz * 0.5f), ImVec2(frame_bb.Max.x - grab_padding, grab_pos + grab_sz * 0.5f));
	window->DrawList->AddRectFilled(grab_bb.Min, grab_bb.Max, GetColorU32(g.ActiveId == id ? ImGuiCol_SliderGrabActive : ImGuiCol_SliderGrab), style.GrabRounding);

	return value_changed;
}

bool ImGui::SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems) return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));

	if (!ItemAdd(total_bb, id))
	{
		ItemSize(total_bb, style.FramePadding.y);
		return false;
	}
	const bool hovered = ItemHoverable(frame_bb, id);
	int decimal_precision = ParseFormatPrecision(display_format, 3);
	const bool tab_focus_requested = FocusableItemRegister(window, id);
	if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]))
	{
		SetActiveID(id, window);
		FocusWindow(window);
		if (tab_focus_requested) g.ScalarAsInputTextId = 0;
	}

	ItemSize(total_bb, style.FramePadding.y);
	const bool value_changed = SliderBehavior(frame_bb, id, v, v_min, v_max, power, decimal_precision);
	char value_buf[XS_RANDOM_SIZE];
	const char* value_buf_end = value_buf + ImFormatString(value_buf, IM_ARRAYSIZE(value_buf), display_format, *v);
	RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

	if (label_size.x > 0.0f)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	return value_changed;
}

bool ImGui::SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format)
{
	float v_f = (float)*v;
	bool value_changed = SliderFloat(label, &v_f, (float)v_min, (float)v_max, display_format, 1.0f);
	*v = (int)v_f;
	return value_changed;
}

bool ImGui::Checkbox(const char* label, bool* v)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(label_size.y + style.FramePadding.y * 2, label_size.y + style.FramePadding.y * 2));
	ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(0, style.FramePadding.y), window->DC.CursorPos + ImVec2(0, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight()), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
		*v = !(*v);

	RenderFrame(check_bb.Min, check_bb.Max, GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), true, style.FrameRounding);
	if (*v)
	{
		const float check_sz = ImMin(check_bb.GetWidth(), check_bb.GetHeight());
		const float pad = ImMax(1.0f, (float)(int)(check_sz / 6.0f));
		RenderCheckMark(check_bb.Min + ImVec2(pad, pad), GetColorU32(ImGuiCol_CheckMark), check_bb.GetWidth() - pad * 2.0f);
	}

	if (label_size.x > 0.0f)
		RenderText(text_bb.Min, label);

	return pressed;
}

bool ImGui::CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value)
{
	bool v = ((*flags & flags_value) == flags_value);
	bool pressed = Checkbox(label, &v);
	if (pressed)
	{
		if (v) *flags |= flags_value;
		else *flags &= ~flags_value;
	}

	return pressed;
}

int InputTextCalcTextLenAndLineCount(const char* text_begin, const char** out_text_end)
{
	int line_count = 0;
	const char* s = text_begin;
	while (char c = *s++)
		if (c == '\n') line_count++;
	s--;
	if (s[0] != '\n' && s[0] != '\r')
		line_count++;
	*out_text_end = s;
	return line_count;
}

ImVec2 InputTextCalcTextSizeW(const ImWchar* text_begin, const ImWchar* text_end, const ImWchar** remaining, ImVec2* out_offset, bool stop_on_new_line)
{
	ImFont* font = GImGui->Font;
	const float line_height = GImGui->FontSize;
	const float scale = line_height / font->FontSize;
	ImVec2 text_size = ImVec2(0, 0);
	float line_width = 0.0f;
	const ImWchar* s = text_begin;
	while (s < text_end)
	{
		unsigned int c = (unsigned int)(*s++);
		if (c == '\n')
		{
			text_size.x = ImMax(text_size.x, line_width);
			text_size.y += line_height;
			line_width = 0.0f;
			if (stop_on_new_line) break;
			continue;
		}
		if (c == '\r') continue;
		const float char_width = font->GetCharAdvance((unsigned short)c) * scale;
		line_width += char_width;
	}
	if (text_size.x < line_width)
		text_size.x = line_width;
	if (out_offset)
		*out_offset = ImVec2(line_width, text_size.y + line_height);
	if (line_width > 0 || text_size.y == 0.0f)
		text_size.y += line_height;
	if (remaining)
		*remaining = s;
	return text_size;
}

int STB_TEXTEDIT_STRINGLEN(const STB_TEXTEDIT_STRING* obj) { return obj->CurLenW; };
ImWchar STB_TEXTEDIT_GETCHAR(const STB_TEXTEDIT_STRING* obj, int idx) { return obj->Text[idx]; };
float STB_TEXTEDIT_GETWIDTH(STB_TEXTEDIT_STRING* obj, int line_start_idx, int char_idx) { ImWchar c = obj->Text[line_start_idx + char_idx]; if (c == '\n') return STB_TEXTEDIT_GETWIDTH_NEWLINE; return GImGui->Font->GetCharAdvance(c) * (GImGui->FontSize / GImGui->Font->FontSize); };
int STB_TEXTEDIT_KEYTOTEXT(int key) { return key >= 0x00010000 ? 0 : key; };

void STB_TEXTEDIT_LAYOUTROW(StbTexteditRow* r, STB_TEXTEDIT_STRING* obj, int line_start_idx)
{
	const ImWchar* text = obj->Text.Data;
	const ImWchar* text_remaining = NULL;
	const ImVec2 size = InputTextCalcTextSizeW(text + line_start_idx, text + obj->CurLenW, &text_remaining, NULL, true);
	r->x0 = 0.0f;
	r->x1 = size.x;
	r->baseline_y_delta = size.y;
	r->ymin = 0.0f;
	r->ymax = size.y;
	r->num_chars = (int)(text_remaining - (text + line_start_idx));
};

bool is_separator(unsigned int c) { return ImCharIsSpace(c) || c == ',' || c == ';' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' || c == '|'; };
int is_word_boundary_from_right(STB_TEXTEDIT_STRING * obj, int idx) { return idx > 0 ? (is_separator(obj->Text[idx - 1]) && !is_separator(obj->Text[idx])) : 1; };
int STB_TEXTEDIT_MOVEWORDLEFT_IMPL(STB_TEXTEDIT_STRING * obj, int idx) { idx--; while (idx >= 0 && !is_word_boundary_from_right(obj, idx)) idx--; return idx < 0 ? 0 : idx; };
int STB_TEXTEDIT_MOVEWORDRIGHT_IMPL(STB_TEXTEDIT_STRING * obj, int idx) { idx++; int len = obj->CurLenW; while (idx < len && !is_word_boundary_from_right(obj, idx)) idx++; return idx > len ? len : idx; };

#define STB_TEXTEDIT_MOVEWORDLEFT STB_TEXTEDIT_MOVEWORDLEFT_IMPL
#define STB_TEXTEDIT_MOVEWORDRIGHT STB_TEXTEDIT_MOVEWORDRIGHT_IMPL

void STB_TEXTEDIT_DELETECHARS(STB_TEXTEDIT_STRING* obj, int pos, int n)
{
	ImWchar* dst = obj->Text.Data + pos;
	obj->CurLenA -= ImTextCountUtf8BytesFromStr(dst, dst + n);
	obj->CurLenW -= n;
	const ImWchar* src = obj->Text.Data + pos + n;
	while (ImWchar c = *src++) *dst++ = c;
	*dst = '\0';
};

bool STB_TEXTEDIT_INSERTCHARS(STB_TEXTEDIT_STRING* obj, int pos, const ImWchar* new_text, int new_text_len)
{
	const int text_len = obj->CurLenW;
	if (new_text_len + text_len + 1 > obj->Text.Size) return false;
	const int new_text_len_utf8 = ImTextCountUtf8BytesFromStr(new_text, new_text + new_text_len);
	if (new_text_len_utf8 + obj->CurLenA + 1 > obj->BufSizeA) return false;
	ImWchar* text = obj->Text.Data;
	if (pos != text_len) memmove(text + pos + new_text_len, text + pos, (size_t)(text_len - pos) * sizeof(ImWchar));
	memcpy(text + pos, new_text, (size_t)new_text_len * sizeof(ImWchar));
	obj->CurLenW += new_text_len, obj->CurLenA += new_text_len_utf8;
	obj->Text[obj->CurLenW] = '\0';
	return true;
};

#define STB_TEXTEDIT_K_LEFT 0x10000
#define STB_TEXTEDIT_K_RIGHT 0x10001
#define STB_TEXTEDIT_K_UP 0x10002
#define STB_TEXTEDIT_K_DOWN 0x10003
#define STB_TEXTEDIT_K_LINESTART 0x10004
#define STB_TEXTEDIT_K_LINEEND 0x10005
#define STB_TEXTEDIT_K_TEXTSTART 0x10006
#define STB_TEXTEDIT_K_TEXTEND 0x10007
#define STB_TEXTEDIT_K_DELETE 0x10008
#define STB_TEXTEDIT_K_BACKSPACE 0x10009
#define STB_TEXTEDIT_K_UNDO 0x1000A
#define STB_TEXTEDIT_K_REDO 0x1000B
#define STB_TEXTEDIT_K_WORDLEFT 0x1000C
#define STB_TEXTEDIT_K_WORDRIGHT 0x1000D
#define STB_TEXTEDIT_K_SHIFT 0x20000

#define STB_TEXTEDIT_IMPLEMENTATION
#include "stb_textedit.h"

void ImGuiTextEditState::OnKeyPressed(int key)
{
	stb_textedit_key(this, &StbState, key);
	CursorFollow = true;
	CursorAnimReset();
}

bool InputTextFilterCharacter(unsigned int* p_char, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback)
{
	unsigned int c = *p_char;

	if (c >= 0x0000E000 && c <= 0x0000F8FF)
		return false;

	if (flags & ImGuiInputTextFlags_CallbackCharFilter)
	{
		ImGuiTextEditCallbackData callback_data;
		memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
		callback_data.EventFlag = ImGuiInputTextFlags_CallbackCharFilter;
		callback_data.EventChar = (ImWchar)c;
		callback_data.Flags = flags;
		if (callback(&callback_data) != 0) return false;
		*p_char = callback_data.EventChar;
		if (!callback_data.EventChar) return false;
	}

	return true;
}

bool ImGui::InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems) return false;
	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), label_size.y + style.FramePadding.y*2.0f);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));
	ImGuiWindow* draw_window = window;
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id)) return false;
	const bool hovered = ItemHoverable(frame_bb, id);
	if (hovered) g.MouseCursor = ImGuiMouseCursor_TextInput;
	ImGuiTextEditState& edit_state = g.InputTextState;
	const bool focus_requested = FocusableItemRegister(window, id);
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;
	const bool user_clicked = hovered && io.MouseClicked[0];
	bool clear_active_id = false;
	bool select_all = (g.ActiveId != id) && (flags & ImGuiInputTextFlags_AutoSelectAll) != 0;
	if (focus_requested || user_clicked)
	{
		if (g.ActiveId != id)
		{
			const int prev_len_w = edit_state.CurLenW;
			edit_state.Text.resize(buf_size + 1);
			edit_state.InitialText.resize(buf_size + 1);
			ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);
			edit_state.CursorAnimReset();
			const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
			if (recycle_state) edit_state.CursorClamp();
			else
			{
				edit_state.Id = id;
				edit_state.ScrollX = 0.0f;
				stb_textedit_initialize_state(&edit_state.StbState, true);
				if (focus_requested_by_code) select_all = true;
			}
			if (flags & ImGuiInputTextFlags_AlwaysInsertMode) edit_state.StbState.insert_mode = true;
			if (focus_requested_by_tab) select_all = true;
		}
		SetActiveID(id, window);
		FocusWindow(window);
	}
	else if (io.MouseClicked[0]) clear_active_id = true;
	bool value_changed = false, enter_pressed = false;
	if (g.ActiveId == id)
	{
		edit_state.BufSizeA = buf_size;
		g.ActiveIdAllowOverlap = !io.MouseDown[0];
		g.WantTextInputNextFrame = 1;
		const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
		const float mouse_y = g.FontSize*0.5f;
		const bool osx_double_click_selects_words = io.OptMacOSXBehaviors;
		if (select_all || (hovered && !osx_double_click_selects_words && io.MouseDoubleClicked[0]))
		{
			edit_state.SelectAll();
			edit_state.SelectedAllMouseLock = true;
		}
		else if (hovered && osx_double_click_selects_words && io.MouseDoubleClicked[0])
		{
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
		}
		else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
		{
			stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
		}
		else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
		{
			stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
			edit_state.CursorFollow = true;
		}
		if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
			edit_state.SelectedAllMouseLock = false;
		if (io.InputCharacters[0])
		{
			for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
				if (unsigned int c = (unsigned int)io.InputCharacters[n])
				{
					if (!InputTextFilterCharacter(&c, flags, callback)) continue;
					edit_state.OnKeyPressed((int)c);
				}
			memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
		};
	}
	if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
	{
		const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
		const bool is_wordmove_key_down = false;
		const bool is_startend_key_down = io.OptMacOSXBehaviors && io.KeySuper;
		if (IsKeyPressedMap(ImGuiKey_LeftArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT : STB_TEXTEDIT_K_LEFT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_RightArrow)) { edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT : STB_TEXTEDIT_K_RIGHT) | k_mask); }
		else if (IsKeyPressedMap(ImGuiKey_Backspace))
		{
			if (!edit_state.HasSelection())
			{
				if (is_wordmove_key_down) edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
				else if (io.OptMacOSXBehaviors && io.KeySuper) edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
			}
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
		}
	}
	if (g.ActiveId == id)
	{
		edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
		ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
		if (strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
		{
			ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
			value_changed = true;
		}
	}
	if (clear_active_id && g.ActiveId == id)
		ClearActiveID();
	const char* buf_display = (g.ActiveId == id) ? edit_state.TempTextBuffer.Data : buf; buf = NULL;
	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y);
	ImVec2 render_pos = frame_bb.Min + style.FramePadding;
	ImVec2 text_size(0.f, 0.f);
	const bool is_currently_scrolling = false;
	if (g.ActiveId == id || is_currently_scrolling)
	{
		edit_state.CursorAnim += io.DeltaTime;
		const ImWchar* text_begin = edit_state.Text.Data;
		ImVec2 cursor_offset, select_start_offset;
		{
			const ImWchar* searches_input_ptr[2];
			searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
			searches_input_ptr[1] = NULL;
			int searches_remaining = 1;
			int searches_result_line_number[2] = { -1, -999 };
			if (edit_state.StbState.select_start != edit_state.StbState.select_end)
			{
				searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
				searches_result_line_number[1] = -1;
				searches_remaining++;
			}
			searches_remaining += 0;
			int line_count = 0;
			for (const ImWchar* s = text_begin; *s != 0; s++)
				if (*s == '\n')
				{
					line_count++;
					if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0]) { searches_result_line_number[0] = line_count; if (--searches_remaining <= 0) break; }
					if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1]) { searches_result_line_number[1] = line_count; if (--searches_remaining <= 0) break; }
				}
			line_count++;
			if (searches_result_line_number[0] == -1) searches_result_line_number[0] = line_count;
			if (searches_result_line_number[1] == -1) searches_result_line_number[1] = line_count;
			cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
			cursor_offset.y = searches_result_line_number[0] * g.FontSize;
			if (searches_result_line_number[1] >= 0)
			{
				select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
				select_start_offset.y = searches_result_line_number[1] * g.FontSize;
			}
		}
		if (edit_state.CursorFollow)
		{
			if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
			{
				const float scroll_increment_x = size.x * 0.25f;
				if (cursor_offset.x < edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
				else if (cursor_offset.x - size.x >= edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
			}
			else
				edit_state.ScrollX = 0.0f;
		}
		edit_state.CursorFollow = false;
		const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);
		if (edit_state.StbState.select_start != edit_state.StbState.select_end)
		{
			const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
			const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);
			float bg_offy_up = -1.0f;
			float bg_offy_dn = 2.0f;
			ImU32 bg_color = GetColorU32(ImGuiCol_TextSelectedBg);
			ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
			for (const ImWchar* p = text_selected_begin; p < text_selected_end; )
			{
				if (rect_pos.y > clip_rect.w + g.FontSize)
					break;
				if (rect_pos.y < clip_rect.y)
				{
					while (p < text_selected_end)
						if (*p++ == '\n')
							break;
				}
				else
				{
					ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
					if (rect_size.x <= 0.0f) rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f);
					ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
					rect.ClipWith(clip_rect);
					if (rect.Overlaps(clip_rect))
						draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
				}
				rect_pos.x = render_pos.x - render_scroll.x;
				rect_pos.y += g.FontSize;
			}
		}
		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, GetColorU32(ImGuiCol_Text), buf_display, buf_display + edit_state.CurLenA, 0.0f, &clip_rect);
		bool cursor_is_visible = (!g.IO.OptCursorBlink) || (g.InputTextState.CursorAnim <= 0.0f) || fmodf(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
		ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
		ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
		if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
			draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), GetColorU32(ImGuiCol_Text));
		g.OsImePosRequest = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
	}
	else
	{
		const char* buf_end = NULL;
		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, GetColorU32(ImGuiCol_Text), buf_display, buf_end, 0.0f, &clip_rect);
	}
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);
	if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		return enter_pressed;
	return value_changed;
}

bool ImGui::InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback)
{
	return InputTextEx(label, buf, (int)buf_size, ImVec2(0, 0), flags, callback);
}

bool ImGui::InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	BeginGroup();
	PushID(label);
	const ImVec2 button_sz = ImVec2(GetFrameHeight(), GetFrameHeight());
	if (step_ptr) PushItemWidth(ImMax(1.0f, CalcItemWidth() - (button_sz.x + style.ItemInnerSpacing.x) * 2));

	char buf[XS_RANDOM_SIZE];
	DataTypeFormatString(data_type, data_ptr, scalar_format, buf, IM_ARRAYSIZE(buf));

	bool value_changed = false;
	if (!(extra_flags & ImGuiInputTextFlags_CharsHexadecimal)) extra_flags |= ImGuiInputTextFlags_CharsDecimal;
	extra_flags |= ImGuiInputTextFlags_AutoSelectAll;
	if (InputText(XorString("##Number"), buf, IM_ARRAYSIZE(buf) / 16, extra_flags))
		value_changed = DataTypeApplyOpFromText(buf, data_ptr, scalar_format);

	if (step_ptr)
	{
		PopItemWidth();
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx(XorString("-"), button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, '-', data_ptr, step_ptr);
			value_changed = true;
		}
		SameLine(0, style.ItemInnerSpacing.x);
		if (ButtonEx(XorString("+"), button_sz, ImGuiButtonFlags_Repeat | ImGuiButtonFlags_DontClosePopups))
		{
			DataTypeApplyOp(data_type, '+', data_ptr, step_ptr);
			value_changed = true;
		}
	}
	PopID();
	if (label_size.x > 0)
	{
		SameLine(0, style.ItemInnerSpacing.x);
		RenderText(ImVec2(window->DC.CursorPos.x, window->DC.CursorPos.y + style.FramePadding.y), label);
		ItemSize(label_size, style.FramePadding.y);
	}
	EndGroup();

	return value_changed;
}

bool ImGui::InputInt(const char* label, int* v, int step, int step_fast, ImGuiInputTextFlags extra_flags)
{
	return InputScalarEx(label, ImGuiDataType_Int, (void*)v, (void*)(step>0.0f ? &step : NULL), (void*)(step_fast>0.0f ? &step_fast : NULL), XorString("%d"), extra_flags);
}

float CalcMaxPopupHeightFromItemCount(int items_count)
{
	ImGuiContext& g = *GImGui;
	if (items_count <= 0) return FLT_MAX;
	return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

bool ImGui::BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiCond backup_next_window_size_constraint = g.NextWindowData.SizeConstraintCond;
	g.NextWindowData.SizeConstraintCond = 0;

	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w = CalcItemWidth();

	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y*2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	ItemSize(total_bb, style.FramePadding.y);
	if (!ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);
	bool popup_open = IsPopupOpen(id);

	const float arrow_size = GetFrameHeight();
	const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
	RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
	RenderFrame(ImVec2(frame_bb.Max.x - arrow_size, frame_bb.Min.y), frame_bb.Max, GetColorU32(popup_open || hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button), true, style.FrameRounding);
	RenderTriangle(ImVec2(frame_bb.Max.x - arrow_size + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y), ImGuiDir_Down);
	if (preview_value != NULL)
		RenderTextClipped(frame_bb.Min + style.FramePadding, value_bb.Max, preview_value, NULL, NULL, ImVec2(0.0f, 0.0f));
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if (pressed && !popup_open)
	{
		OpenPopupEx(id);
		popup_open = true;
	}

	if (!popup_open)
		return false;

	if (backup_next_window_size_constraint)
	{
		g.NextWindowData.SizeConstraintCond = backup_next_window_size_constraint;
		g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
	}
	else
	{
		if ((flags & ImGuiComboFlags_HeightMask_) == 0)
			flags |= ImGuiComboFlags_HeightRegular;

		int popup_max_height_in_items = -1;
		if (flags & ImGuiComboFlags_HeightRegular) popup_max_height_in_items = 8;
		else if (flags & ImGuiComboFlags_HeightSmall) popup_max_height_in_items = 4;
		else if (flags & ImGuiComboFlags_HeightLarge) popup_max_height_in_items = 20;
		SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
	}

	char name[XS_RANDOM_SIZE];
	xprintf(name, IM_ARRAYSIZE(name), XorString("##Combo_%02d"), g.CurrentPopupStack.Size);

	if (ImGuiWindow* popup_window = FindWindowByName(name))
		if (popup_window->WasActive)
		{
			ImVec2 size_contents = CalcSizeContents(popup_window);
			ImVec2 size_expected = CalcSizeAfterConstraint(popup_window, CalcSizeAutoFit(popup_window, size_contents));
			if (flags & ImGuiComboFlags_PopupAlignLeft)
				popup_window->AutoPosLastDirection = ImGuiDir_Left;
			ImVec2 pos = FindBestWindowPosForPopup(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
			SetNextWindowPos(pos);
		}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	if (!Begin(name, window_flags))
	{
		EndPopup();
		return false;
	}

	if (style.FramePadding.x != style.WindowPadding.x)
		Indent(style.FramePadding.x - style.WindowPadding.x);

	return true;
}

void ImGui::OpenPopupEx(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = g.CurrentWindow;
	int current_stack_size = g.CurrentPopupStack.Size;
	ImGuiPopupRef popup_ref;
	popup_ref.PopupId = id;
	popup_ref.Window = NULL;
	popup_ref.ParentWindow = parent_window;
	popup_ref.OpenFrameCount = g.FrameCount;
	popup_ref.OpenParentId = parent_window->IDStack.back();
	popup_ref.OpenMousePos = g.IO.MousePos;
	popup_ref.OpenPopupPos = g.IO.MousePos;

	if (g.OpenPopupStack.Size < current_stack_size + 1)
		g.OpenPopupStack.push_back(popup_ref);
	else
	{
		g.OpenPopupStack.resize(current_stack_size + 1);

		if (g.OpenPopupStack[current_stack_size].PopupId == id && g.OpenPopupStack[current_stack_size].OpenFrameCount == g.FrameCount - 1)
			g.OpenPopupStack[current_stack_size].OpenFrameCount = popup_ref.OpenFrameCount;
		else
			g.OpenPopupStack[current_stack_size] = popup_ref;
	}
}

void ImGui::EndCombo()
{
	const ImGuiStyle& style = GImGui->Style;
	if (style.FramePadding.x != style.WindowPadding.x)
		Unindent(style.FramePadding.x - style.WindowPadding.x);
	EndPopup();
}

bool ImGui::Combo(const char* label, int* current_item, bool(*items_getter) (void *, int, const char **), void * data, int items_count, int popup_max_height_in_items)
{
	ImGuiContext& g = *GImGui;

	const char* preview_text = NULL;
	if (*current_item >= 0 && *current_item < items_count)
		items_getter(data, *current_item, &preview_text);

	if (popup_max_height_in_items != -1 && !g.NextWindowData.SizeConstraintCond)
	{
		float popup_max_height = CalcMaxPopupHeightFromItemCount(popup_max_height_in_items);
		SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, popup_max_height));
	}

	if (!BeginCombo(label, preview_text, 0))
		return false;

	bool value_changed = false;
	for (int i = 0; i < items_count; i++)
	{
		PushID((void *)(intptr_t)i);
		const bool item_selected = (i == *current_item);
		const char* item_text;
		items_getter(data, i, &item_text);

		if (Selectable(item_text, item_selected))
		{
			value_changed = true;
			*current_item = i;
		}
		if (item_selected)
			SetItemDefaultFocus();
		PopID();
	}

	EndCombo();
	return value_changed;
}

bool Items_ArrayGetter(void * data, int idx, const char ** out_text)
{
	const char * const * items = (const char * const *)data;
	if (out_text)
		*out_text = items[idx];
	return true;
};

bool Items_SingleStringGetter(void* data, int idx, const char** out_text)
{
	const char* items_separated_by_zeros = (const char*)data;
	int items_count = 0;
	const char* p = items_separated_by_zeros;
	while (*p)
	{
		if (idx == items_count)
			break;
		p += strlen(p) + 1;
		items_count++;
	}
	if (!*p)
		return false;
	if (out_text)
		*out_text = p;
	return true;
}

bool ImGui::Combo(const char * label, int * current_item, const char * const items[], int items_count, int height_in_items)
{
	const bool value_changed = Combo(label, current_item, Items_ArrayGetter, (void *)items, items_count, height_in_items);
	return value_changed;
};

bool ImGui::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
		PopClipRect();

	ImGuiID id = window->GetID(label);
	ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
	ImVec2 pos = window->DC.CursorPos;
	pos.y += window->DC.CurrentLineTextBaseOffset;
	ImRect bb(pos, pos + size);
	ItemSize(bb);

	ImVec2 window_padding = window->WindowPadding;
	float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? GetWindowContentRegionMax().x : GetContentRegionMax().x;
	float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
	ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
	ImRect bb_with_spacing(pos, pos + size_draw);
	if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
		bb_with_spacing.Max.x += window_padding.x;

	float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
	float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
	float spacing_R = style.ItemSpacing.x - spacing_L;
	float spacing_D = style.ItemSpacing.y - spacing_U;
	bb_with_spacing.Min.x -= spacing_L;
	bb_with_spacing.Min.y -= spacing_U;
	bb_with_spacing.Max.x += spacing_R;
	bb_with_spacing.Max.y += spacing_D;
	if (!ItemAdd(bb_with_spacing, id))
	{
		if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
			PushColumnClipRect();
		return false;
	}

	ImGuiButtonFlags button_flags = 0;
	if (flags & ImGuiSelectableFlags_Menu) button_flags |= ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_NoHoldingActiveID;
	if (flags & ImGuiSelectableFlags_MenuItem) button_flags |= ImGuiButtonFlags_PressedOnRelease;
	if (flags & ImGuiSelectableFlags_Disabled) button_flags |= ImGuiButtonFlags_Disabled;
	if (flags & ImGuiSelectableFlags_AllowDoubleClick) button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
	bool hovered, held;
	bool pressed = ButtonBehavior(bb_with_spacing, id, &hovered, &held, button_flags);
	if (flags & ImGuiSelectableFlags_Disabled)
		selected = false;

	if (hovered || selected)
	{
		const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_HeaderActive : hovered ? ImGuiCol_HeaderHovered : ImGuiCol_Header);
		RenderFrame(bb_with_spacing.Min, bb_with_spacing.Max, col, false, 0.0f);
	}

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
	{
		PushColumnClipRect();
		bb_with_spacing.Max.x -= (GetContentRegionMax().x - max_x);
	}
	RenderTextClipped(bb.Min, bb_with_spacing.Max, label, NULL, &label_size, ImVec2(0.0f, 0.0f));
	if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
		CloseCurrentPopup();
	return pressed;
}

bool ImGui::Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	if (Selectable(label, *p_selected, flags, size_arg))
	{
		*p_selected = !*p_selected;
		return true;
	}
	return false;
}

bool ImGui::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = GetStyle();
	const ImGuiID id = GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 size = CalcItemSize(size_arg, CalcItemWidth(), GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
	ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	window->DC.LastItemRect = bb;

	BeginGroup();
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	BeginChildFrame(id, frame_bb.GetSize());
	return true;
}

bool ImGui::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
	if (height_in_items < 0)
		height_in_items = ImMin(items_count, 7);
	float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

	ImVec2 size;
	size.x = 0.0f;
	size.y = GetTextLineHeightWithSpacing() * height_in_items_f + GetStyle().ItemSpacing.y;
	return ListBoxHeader(label, size);
}

void ImGui::ListBoxFooter()
{
	ImGuiWindow* parent_window = GetCurrentWindow()->ParentWindow;
	const ImRect bb = parent_window->DC.LastItemRect;
	const ImGuiStyle& style = GetStyle();

	EndChildFrame();
	SameLine();
	parent_window->DC.CursorPos = bb.Min;
	ItemSize(bb, style.FramePadding.y);
	EndGroup();
}

bool ImGui::ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_items)
{
	const bool value_changed = ListBox(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_items);
	return value_changed;
}

bool ImGui::ListBox(const char* label, int* current_item, bool(*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
	if (!ListBoxHeader(label, items_count, height_in_items))
		return false;

	bool value_changed = false;
	ImGuiListClipper clipper(items_count, GetTextLineHeightWithSpacing());
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);
			const char* item_text;
			items_getter(data, i, &item_text);
			PushID(i);
			if (Selectable(item_text, item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			PopID();
		}
	ListBoxFooter();
	return value_changed;
}

inline ImU32 ImAlphaBlendColor(ImU32 col_a, ImU32 col_b)
{
	float t = ((col_b >> IM_COL32_A_SHIFT) & 0xFF) / 255.f;
	int r = ImLerp((int)(col_a >> IM_COL32_R_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_R_SHIFT) & 0xFF, t);
	int g = ImLerp((int)(col_a >> IM_COL32_G_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_G_SHIFT) & 0xFF, t);
	int b = ImLerp((int)(col_a >> IM_COL32_B_SHIFT) & 0xFF, (int)(col_b >> IM_COL32_B_SHIFT) & 0xFF, t);
	return IM_COL32(r, g, b, 0xFF);
}

void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col)
{
	switch (direction)
	{
	case ImGuiDir_Left: draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), pos, col); return;
	case ImGuiDir_Right: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), pos, col); return;
	case ImGuiDir_Up: draw_list->AddTriangleFilled(ImVec2(pos.x + half_sz.x, pos.y + half_sz.y), ImVec2(pos.x - half_sz.x, pos.y + half_sz.y), pos, col); return;
	case ImGuiDir_Down: draw_list->AddTriangleFilled(ImVec2(pos.x - half_sz.x, pos.y - half_sz.y), ImVec2(pos.x + half_sz.x, pos.y - half_sz.y), pos, col); return;
	case ImGuiDir_None: case ImGuiDir_Count_: break;
	}
}

void RenderArrowsForVerticalBar(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, float bar_w)
{
	RenderArrow(draw_list, ImVec2(pos.x + half_sz.x + 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Right, IM_COL32_BLACK);
	RenderArrow(draw_list, ImVec2(pos.x + half_sz.x, pos.y), half_sz, ImGuiDir_Right, IM_COL32_WHITE);
	RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x - 1, pos.y), ImVec2(half_sz.x + 2, half_sz.y + 1), ImGuiDir_Left, IM_COL32_BLACK);
	RenderArrow(draw_list, ImVec2(pos.x + bar_w - half_sz.x, pos.y), half_sz, ImGuiDir_Left, IM_COL32_WHITE);
}

void ImGui::Separator()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;

	ImGuiWindowFlags flags = 0;
	if ((flags & (ImGuiSeparatorFlags_Horizontal | ImGuiSeparatorFlags_Vertical)) == 0)
		flags |= (window->DC.LayoutType == ImGuiLayoutType_Horizontal) ? ImGuiSeparatorFlags_Vertical : ImGuiSeparatorFlags_Horizontal;

	if (flags & ImGuiSeparatorFlags_Vertical)
	{
		VerticalSeparator();
		return;
	}

	if (window->DC.ColumnsSet)
		PopClipRect();

	float x1 = window->Pos.x;
	float x2 = window->Pos.x + window->Size.x;
	if (!window->DC.GroupStack.empty())
		x1 += window->DC.IndentX;

	const ImRect bb(ImVec2(x1, window->DC.CursorPos.y), ImVec2(x2, window->DC.CursorPos.y + 1.0f));
	ItemSize(ImVec2(0.0f, 0.0f));
	if (!ItemAdd(bb, 0))
	{
		if (window->DC.ColumnsSet)
			PushColumnClipRect();
		return;
	}

	window->DrawList->AddLine(bb.Min, ImVec2(bb.Max.x, bb.Min.y), GetColorU32(ImGuiCol_Separator));

	if (window->DC.ColumnsSet)
	{
		PushColumnClipRect();
		window->DC.ColumnsSet->CellMinY = window->DC.CursorPos.y;
	}
}

void ImGui::VerticalSeparator()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ImGuiContext& g = *GImGui;

	float y1 = window->DC.CursorPos.y;
	float y2 = window->DC.CursorPos.y + window->DC.CurrentLineHeight;
	const ImRect bb(ImVec2(window->DC.CursorPos.x, y1), ImVec2(window->DC.CursorPos.x + 1.0f, y2));
	ItemSize(ImVec2(bb.GetWidth(), 0.0f));
	if (!ItemAdd(bb, 0))
		return;

	window->DrawList->AddLine(ImVec2(bb.Min.x, bb.Min.y), ImVec2(bb.Min.x, bb.Max.y), GetColorU32(ImGuiCol_Separator));
}

void ImGui::Spacing()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;
	ItemSize(ImVec2(0, 0));
}

bool ImGui::IsRectVisible(const ImVec2& size)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(window->DC.CursorPos, window->DC.CursorPos + size));
}

bool ImGui::IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->ClipRect.Overlaps(ImRect(rect_min, rect_max));
}

void ImGui::BeginGroup()
{
	ImGuiWindow* window = GetCurrentWindow();

	window->DC.GroupStack.resize(window->DC.GroupStack.Size + 1);
	ImGuiGroupData& group_data = window->DC.GroupStack.back();
	group_data.BackupCursorPos = window->DC.CursorPos;
	group_data.BackupCursorMaxPos = window->DC.CursorMaxPos;
	group_data.BackupIndentX = window->DC.IndentX;
	group_data.BackupGroupOffsetX = window->DC.GroupOffsetX;
	group_data.BackupCurrentLineHeight = window->DC.CurrentLineHeight;
	group_data.BackupCurrentLineTextBaseOffset = window->DC.CurrentLineTextBaseOffset;
	group_data.BackupActiveIdIsAlive = GImGui->ActiveIdIsAlive;
	group_data.AdvanceCursor = true;

	window->DC.GroupOffsetX = window->DC.CursorPos.x - window->Pos.x - window->DC.ColumnsOffsetX;
	window->DC.IndentX = window->DC.GroupOffsetX;
	window->DC.CursorMaxPos = window->DC.CursorPos;
	window->DC.CurrentLineHeight = 0.0f;
}

void ImGui::EndGroup()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	ImGuiGroupData& group_data = window->DC.GroupStack.back();

	ImRect group_bb(group_data.BackupCursorPos, window->DC.CursorMaxPos);
	group_bb.Max = ImMax(group_bb.Min, group_bb.Max);

	window->DC.CursorPos = group_data.BackupCursorPos;
	window->DC.CursorMaxPos = ImMax(group_data.BackupCursorMaxPos, window->DC.CursorMaxPos);
	window->DC.CurrentLineHeight = group_data.BackupCurrentLineHeight;
	window->DC.CurrentLineTextBaseOffset = group_data.BackupCurrentLineTextBaseOffset;
	window->DC.IndentX = group_data.BackupIndentX;
	window->DC.GroupOffsetX = group_data.BackupGroupOffsetX;

	if (group_data.AdvanceCursor)
	{
		window->DC.CurrentLineTextBaseOffset = ImMax(window->DC.PrevLineTextBaseOffset, group_data.BackupCurrentLineTextBaseOffset);
		ItemSize(group_bb.GetSize(), group_data.BackupCurrentLineTextBaseOffset);
		ItemAdd(group_bb, 0);
	}

	const bool active_id_within_group = (!group_data.BackupActiveIdIsAlive && g.ActiveIdIsAlive && g.ActiveId && g.ActiveIdWindow->RootWindow == window->RootWindow);
	if (active_id_within_group)
		window->DC.LastItemId = g.ActiveId;
	window->DC.LastItemRect = group_bb;
	window->DC.GroupStack.pop_back();
}

void ImGui::SameLine(float pos_x, float spacing_w)
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	if (pos_x != 0.0f)
	{
		if (spacing_w < 0.0f) spacing_w = 0.0f;
		window->DC.CursorPos.x = window->Pos.x - window->Scroll.x + pos_x + spacing_w + window->DC.GroupOffsetX + window->DC.ColumnsOffsetX;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	else
	{
		if (spacing_w < 0.0f) spacing_w = g.Style.ItemSpacing.x;
		window->DC.CursorPos.x = window->DC.CursorPosPrevLine.x + spacing_w;
		window->DC.CursorPos.y = window->DC.CursorPosPrevLine.y;
	}
	window->DC.CurrentLineHeight = window->DC.PrevLineHeight;
	window->DC.CurrentLineTextBaseOffset = window->DC.PrevLineTextBaseOffset;
}

void ImGui::NewLine()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const ImGuiLayoutType backup_layout_type = window->DC.LayoutType;
	window->DC.LayoutType = ImGuiLayoutType_Vertical;
	if (window->DC.CurrentLineHeight > 0.0f)
		ItemSize(ImVec2(0, 0));
	else
		ItemSize(ImVec2(0.0f, g.FontSize));
	window->DC.LayoutType = backup_layout_type;
}

void ImGui::NextColumn()
{
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems || window->DC.ColumnsSet == NULL)
		return;

	ImGuiContext& g = *GImGui;
	PopItemWidth();
	PopClipRect();

	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	columns->CellMaxY = ImMax(columns->CellMaxY, window->DC.CursorPos.y);
	if (++columns->Current < columns->Count)
	{
		window->DC.ColumnsOffsetX = GetColumnOffset(columns->Current) - window->DC.IndentX + g.Style.ItemSpacing.x;
		window->DrawList->ChannelsSetCurrent(columns->Current);
	}
	else
	{
		window->DC.ColumnsOffsetX = 0.0f;
		window->DrawList->ChannelsSetCurrent(0);
		columns->Current = 0;
		columns->CellMinY = columns->CellMaxY;
	}
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
	window->DC.CursorPos.y = columns->CellMinY;
	window->DC.CurrentLineHeight = 0.0f;
	window->DC.CurrentLineTextBaseOffset = 0.0f;

	PushColumnClipRect();
	PushItemWidth(GetColumnWidth() * 0.65f);
}

int ImGui::GetColumnIndex()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsSet ? window->DC.ColumnsSet->Current : 0;
}

int ImGui::GetColumnsCount()
{
	ImGuiWindow* window = GetCurrentWindowRead();
	return window->DC.ColumnsSet ? window->DC.ColumnsSet->Count : 1;
}

float OffsetNormToPixels(const ImGuiColumnsSet* columns, float offset_norm)
{
	return offset_norm * (columns->MaxX - columns->MinX);
}

float PixelsToOffsetNorm(const ImGuiColumnsSet* columns, float offset)
{
	return offset / (columns->MaxX - columns->MinX);
}

inline float GetColumnsRectHalfWidth() { return 4.0f; }

float GetDraggedColumnOffset(ImGuiColumnsSet* columns, int column_index)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + GetColumnsRectHalfWidth() - window->Pos.x;
	x = ImMax(x, ImGui::GetColumnOffset(column_index - 1) + g.Style.ColumnsMinSpacing);
	if ((columns->Flags & ImGuiColumnsFlags_NoPreserveWidths))
		x = ImMin(x, ImGui::GetColumnOffset(column_index + 1) - g.Style.ColumnsMinSpacing);

	return x;
}

float ImGui::GetColumnOffset(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;

	if (column_index < 0)
		column_index = columns->Current;

	const float t = columns->Columns[column_index].OffsetNorm;
	const float x_offset = ImLerp(columns->MinX, columns->MaxX, t);
	return x_offset;
}

float GetColumnWidthEx(ImGuiColumnsSet* columns, int column_index, bool before_resize = false)
{
	if (column_index < 0)
		column_index = columns->Current;

	float offset_norm;
	if (before_resize)
		offset_norm = columns->Columns[column_index + 1].OffsetNormBeforeResize - columns->Columns[column_index].OffsetNormBeforeResize;
	else
		offset_norm = columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm;
	return OffsetNormToPixels(columns, offset_norm);
}

float ImGui::GetColumnWidth(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;

	if (column_index < 0)
		column_index = columns->Current;
	return OffsetNormToPixels(columns, columns->Columns[column_index + 1].OffsetNorm - columns->Columns[column_index].OffsetNorm);
}

void ImGui::SetColumnOffset(int column_index, float offset)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;

	if (column_index < 0)
		column_index = columns->Current;

	const bool preserve_width = !(columns->Flags & ImGuiColumnsFlags_NoPreserveWidths) && (column_index < columns->Count - 1);
	const float width = preserve_width ? GetColumnWidthEx(columns, column_index, columns->IsBeingResized) : 0.0f;

	if (!(columns->Flags & ImGuiColumnsFlags_NoForceWithinWindow))
		offset = ImMin(offset, columns->MaxX - g.Style.ColumnsMinSpacing * (columns->Count - column_index));
	columns->Columns[column_index].OffsetNorm = PixelsToOffsetNorm(columns, offset - columns->MinX);

	if (preserve_width)
		SetColumnOffset(column_index + 1, offset + ImMax(g.Style.ColumnsMinSpacing, width));
}

void ImGui::SetColumnWidth(int column_index, float width)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;

	if (column_index < 0)
		column_index = columns->Current;
	SetColumnOffset(column_index + 1, GetColumnOffset(column_index) + width);
}

void ImGui::PushColumnClipRect(int column_index)
{
	ImGuiWindow* window = GetCurrentWindowRead();
	ImGuiColumnsSet* columns = window->DC.ColumnsSet;
	if (column_index < 0)
		column_index = columns->Current;

	PushClipRect(columns->Columns[column_index].ClipRect.Min, columns->Columns[column_index].ClipRect.Max, false);
}

ImGuiColumnsSet* FindOrAddColumnsSet(ImGuiWindow* window, ImGuiID id)
{
	for (int n = 0; n < window->ColumnsStorage.Size; n++)
		if (window->ColumnsStorage[n].ID == id)
			return &window->ColumnsStorage[n];

	window->ColumnsStorage.push_back(ImGuiColumnsSet());
	ImGuiColumnsSet* columns = &window->ColumnsStorage.back();
	columns->ID = id;
	return columns;
}

void ImGui::BeginColumns(int columns_count)
{
	ImGuiContext & g = *GImGui;
	ImGuiWindow * window = GetCurrentWindow();
	ImGuiID id = window->GetID(XorString("Columns"));
	ImGuiColumnsSet* columns = FindOrAddColumnsSet(window, id);
	columns->Current = 0;
	columns->Count = columns_count;
	columns->Flags = ImGuiColumnsFlags_NoBorder;
	window->DC.ColumnsSet = columns;
	const float content_region_width = (window->SizeContentsExplicit.x != 0.0f) ? (window->SizeContentsExplicit.x) : (window->Size.x - window->ScrollbarSizes.x);
	columns->MinX = window->DC.IndentX - g.Style.ItemSpacing.x;
	columns->MaxX = content_region_width - window->Scroll.x;
	columns->StartPosY = window->DC.CursorPos.y;
	columns->StartMaxPosX = window->DC.CursorMaxPos.x;
	columns->CellMinY = columns->CellMaxY = window->DC.CursorPos.y;
	window->DC.ColumnsOffsetX = 0.0f;
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
	if (columns->Columns.Size != 0 && columns->Columns.Size != columns_count + 1)
		columns->Columns.resize(0);

	columns->IsFirstFrame = (columns->Columns.Size == 0);
	if (columns->Columns.Size == 0)
	{
		columns->Columns.reserve(columns_count + 1);
		for (int n = 0; n < columns_count + 1; n++)
		{
			ImGuiColumnData column;
			column.OffsetNorm = n / (float)columns_count;
			columns->Columns.push_back(column);
		}
	}

	for (int n = 0; n < columns_count + 1; n++)
	{
		ImGuiColumnData* column = &columns->Columns[n];
		float t = column->OffsetNorm;
		if (!(columns->Flags & ImGuiColumnsFlags_NoForceWithinWindow))
			t = ImMin(t, PixelsToOffsetNorm(columns, (columns->MaxX - columns->MinX) - g.Style.ColumnsMinSpacing * (columns->Count - n)));
		column->OffsetNorm = t;

		if (n == columns_count)
			continue;

		float clip_x1 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n) - 1.0f);
		float clip_x2 = ImFloor(0.5f + window->Pos.x + GetColumnOffset(n + 1) - 1.0f);
		column->ClipRect = ImRect(clip_x1, -FLT_MAX, clip_x2, +FLT_MAX);
		column->ClipRect.ClipWith(window->ClipRect);
	}

	window->DrawList->ChannelsSplit(columns->Count);
	PushColumnClipRect();
	PushItemWidth(GetColumnWidth() * 0.65f);
}

void ImGui::EndColumns()
{
	ImGuiContext & g = *GImGui;
	ImGuiWindow * window = GetCurrentWindow();
	ImGuiColumnsSet * columns = window->DC.ColumnsSet;

	PopItemWidth();
	PopClipRect();
	window->DrawList->ChannelsMerge();

	columns->CellMaxY = ImMax(columns->CellMaxY, window->DC.CursorPos.y);
	window->DC.CursorPos.y = columns->CellMaxY;
	if (!(columns->Flags & ImGuiColumnsFlags_GrowParentContentsSize))
		window->DC.CursorMaxPos.x = ImMax(columns->StartMaxPosX, columns->MaxX);

	bool is_being_resized = false;
	if (!(columns->Flags & ImGuiColumnsFlags_NoBorder) && !window->SkipItems)
	{
		const float y1 = columns->StartPosY;
		const float y2 = window->DC.CursorPos.y;
		int dragging_column = -1;
		for (int n = 1; n < columns->Count; n++)
		{
			float x = window->Pos.x + GetColumnOffset(n);
			const ImGuiID column_id = columns->ID + ImGuiID(n);
			const float column_hw = GetColumnsRectHalfWidth();
			const ImRect column_rect(ImVec2(x - column_hw, y1), ImVec2(x + column_hw, y2));
			KeepAliveID(column_id);
			if (IsClippedEx(column_rect, column_id, false))
				continue;

			bool hovered = false, held = false;
			if (!(columns->Flags & ImGuiColumnsFlags_NoResize))
			{
				ButtonBehavior(column_rect, column_id, &hovered, &held);
				if (hovered || held)
					g.MouseCursor = ImGuiMouseCursor_ResizeEW;
				if (held && !(columns->Columns[n].Flags & ImGuiColumnsFlags_NoResize))
					dragging_column = n;
			}

			const ImU32 col = GetColorU32(held ? ImGuiCol_SeparatorActive : hovered ? ImGuiCol_SeparatorHovered : ImGuiCol_Separator);
			const float xi = (float)(int)x;
			window->DrawList->AddLine(ImVec2(xi, ImMax(y1 + 1.0f, window->ClipRect.Min.y)), ImVec2(xi, ImMin(y2, window->ClipRect.Max.y)), col);
		}

		if (dragging_column != -1)
		{
			if (!columns->IsBeingResized)
				for (int n = 0; n < columns->Count + 1; n++)
					columns->Columns[n].OffsetNormBeforeResize = columns->Columns[n].OffsetNorm;
			columns->IsBeingResized = is_being_resized = true;
			float x = GetDraggedColumnOffset(columns, dragging_column);
			SetColumnOffset(dragging_column, x);
		}
	}
	columns->IsBeingResized = is_being_resized;

	window->DC.ColumnsSet = NULL;
	window->DC.ColumnsOffsetX = 0.0f;
	window->DC.CursorPos.x = (float)(int)(window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX);
}

void ImGui::Columns(int columns_count)
{
	ImGuiWindow * window = GetCurrentWindow();
	if (window->DC.ColumnsSet != NULL && window->DC.ColumnsSet->Count != columns_count) EndColumns();
	if (columns_count != 1) BeginColumns(columns_count);
};

void ImGui::Indent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX += (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
};

void ImGui::Unindent(float indent_w)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = GetCurrentWindow();
	window->DC.IndentX -= (indent_w != 0.0f) ? indent_w : g.Style.IndentSpacing;
	window->DC.CursorPos.x = window->Pos.x + window->DC.IndentX + window->DC.ColumnsOffsetX;
};
