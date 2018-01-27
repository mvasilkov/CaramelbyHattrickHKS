
#include "SDK.hpp"
#include "Platform.hpp"
#include "Configuration.hpp"

enum class EStickerAttributeType : INT { Index, Wear, Scale, Rotation, };

uint16_t s_econ_item_interface_wrapper_offset = 0;
void * oGetStickerAttributeBySlotIndexFloat;

float __fastcall GetStickerAttributeBySlotIndexFloat(void * thisptr, void *, int slot, EStickerAttributeType attribute, float unknown)
{
	static C_BaseAttributableItem * item = NULL;
	static EconomyItem_t * pX = NULL;
	item = reinterpret_cast < C_BaseAttributableItem * > (uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);
	if (item)
	{
		pX = g_config.GetByDefinitionIndex(item->GetItemDefinitionIndex());
		if (pX)
		{
			switch (attribute)
			{
			case EStickerAttributeType::Wear: return pX->stickers[slot].wear;
			case EStickerAttributeType::Scale: return pX->stickers[slot].scale;
			case EStickerAttributeType::Rotation: return pX->stickers[slot].rotation;
			};
		};
	};
	return reinterpret_cast < decltype (GetStickerAttributeBySlotIndexFloat) * > (oGetStickerAttributeBySlotIndexFloat) (thisptr, nullptr, slot, attribute, unknown);
};

void * oGetStickerAttributeBySlotIndexInt;

unsigned int __fastcall GetStickerAttributeBySlotIndexInt(void * thisptr, void *, int slot, EStickerAttributeType attribute, unsigned unknown)
{
	static C_BaseAttributableItem * item = NULL;
	static EconomyItem_t * pX = NULL;
	if (attribute == EStickerAttributeType::Index)
	{
		item = reinterpret_cast < C_BaseAttributableItem * > (uintptr_t(thisptr) - s_econ_item_interface_wrapper_offset);
		if (item)
		{
			pX = g_config.GetByDefinitionIndex(item->GetItemDefinitionIndex());
			if (pX) return pX->stickers[slot].kit_index;
		};
	};
	return reinterpret_cast < decltype (GetStickerAttributeBySlotIndexInt) * > (oGetStickerAttributeBySlotIndexInt) (thisptr, nullptr, slot, attribute, unknown);
};

void ApplyStickerHooks(C_BaseAttributableItem * item)
{
	if (!s_econ_item_interface_wrapper_offset) s_econ_item_interface_wrapper_offset = NetVarManager::Get().GetOffset(FnvHash(XorString("CEconEntity->m_Item"))) + 0x0000000C;
	void ** & vmt = *reinterpret_cast < void *** > (uintptr_t(item) + s_econ_item_interface_wrapper_offset);
	static void ** hooked_vmt = nullptr;
	if (!hooked_vmt)
	{
		size_t size = 0;
		while (IsCodePtr(vmt[size])) size++;
		hooked_vmt = new void *[size];
		memcpy(hooked_vmt, vmt, size * sizeof(void *));
		oGetStickerAttributeBySlotIndexFloat = hooked_vmt[4];
		hooked_vmt[4] = reinterpret_cast <void *> (&GetStickerAttributeBySlotIndexFloat);
		oGetStickerAttributeBySlotIndexInt = hooked_vmt[5];
		hooked_vmt[5] = reinterpret_cast <void *> (&GetStickerAttributeBySlotIndexInt);
	};
	vmt = hooked_vmt;
};
