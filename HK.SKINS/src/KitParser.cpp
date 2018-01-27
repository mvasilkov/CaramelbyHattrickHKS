
#include "SDK.hpp"
#include "Platform.hpp"
#include "KitParser.hpp"
#include "nSkinz.hpp"

vector <Kit_t> k_skins;
vector <Kit_t> k_gloves;
vector <Kit_t> k_stickers;

class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value> struct Node_t {
	int previous_id;
	int next_id;
	void * _unknown_ptr;
	int _unknown;
	Key key;
	Value value;
};

template <typename Key, typename Value> struct Head_t {
	Node_t <Key, Value> * memory;
	int allocation_count;
	int grow_size;
	int start_element;
	int next_available;
	int _unknown;
	int last_element;
};

struct String_t {
	char * buffer;
	int capacity;
	int grow_size;
	int length;
};

struct CPaintKit {
	int id;
	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;
	char pad_0x0054[0x0000008C];
};

struct CStickerKit {
	int id;
	int item_rarity;
	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;
	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;
	float rotate_end;
	float rotate_start;
	float scale_min;
	float scale_max;
	float wear_min;
	float wear_max;
	String_t image_inventory2;
	String_t image_inventory_large;
	uint32_t pad0[4];
};

CONST VOID CONST InitializeKits(VOID)
{
	static INT i = 0, iter = 0;
	static wstring_convert < codecvt_utf8_utf16 < WCHAR > > converter;
	auto sig_address = FindPattern(XorString("client.dll"), XorString("E8 ?? ?? ?? ?? FF 76 0C 8D 48 04 E8"));
	auto item_system_offset = *reinterpret_cast < int32_t * > (sig_address + 1);
	auto item_system_fn = reinterpret_cast < CCStrike15ItemSystem * (*) () > (sig_address + 5 + item_system_offset);
	auto item_schema = reinterpret_cast < CCStrike15ItemSchema * > (uintptr_t(item_system_fn()) + sizeof(void *));
	{
		auto get_paint_kit_definition_offset = *reinterpret_cast < int32_t * > (sig_address + 12);
		auto get_paint_kit_definition_fn = reinterpret_cast < CPaintKit * (__thiscall *) (CCStrike15ItemSchema *, int) > (sig_address + 16 + get_paint_kit_definition_offset);
		auto start_element_offset = *reinterpret_cast  <intptr_t * > (uintptr_t(get_paint_kit_definition_fn) + 10);
		auto head_offset = start_element_offset - 12;
		auto map_head = reinterpret_cast < Head_t < int, CPaintKit * > * > (uintptr_t(item_schema) + head_offset);
		for (i = 0; i <= map_head->last_element; i++)
		{
			auto paint_kit = map_head->memory[i].value;
			if (paint_kit->id == 9001) continue;
			CONST PWCHAR CONST wide_name = g_localize->Find(paint_kit->item_name.buffer + 1);
			auto name = converter.to_bytes(wide_name);
			for (iter = 0; iter < name.length(); iter++)
				if ((name.at(iter) >= 'a' && name.at(iter) <= 'z') || (name.at(iter) >= 'A' && name.at(iter) <= 'Z'))
					name.at(iter) = toupper(name.at(iter));
			if (paint_kit->id < 10000) k_skins.push_back({ paint_kit->id, name });
			else k_gloves.push_back({ paint_kit->id, name });
		}
		sort(k_skins.begin(), k_skins.end()), sort(k_gloves.begin(), k_gloves.end());
	};
	{
		auto sticker_sig = FindPattern(XorString("client.dll"), XorString("53 8D 48 04 E8 ? ? ? ? 8B 4D 10")) + 4;
		auto get_sticker_kit_definition_offset = *reinterpret_cast < intptr_t * > (sticker_sig + 1);
		auto get_sticker_kit_definition_fn = reinterpret_cast < CPaintKit * (__thiscall *) (CCStrike15ItemSchema *, int) > (sticker_sig + 5 + get_sticker_kit_definition_offset);
		auto start_element_offset = *reinterpret_cast < intptr_t * > (uintptr_t(get_sticker_kit_definition_fn) + 10);
		auto head_offset = start_element_offset - 12;
		auto map_head = reinterpret_cast < Head_t < int, CStickerKit * > * > (uintptr_t(item_schema) + head_offset);
		for (i = 0; i <= map_head->last_element; i++)
		{
			auto sticker_kit = map_head->memory[i].value;
			char sticker_name_if_valve_fucked_up_their_translations[XS_RANDOM_SIZE / 2] = {};
			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;
			if (strstr(sticker_name_ptr, XorString("StickerKit_dhw2014_dignitas")))
			{
				strcpy(sticker_name_if_valve_fucked_up_their_translations, XorString("StickerKit_dhw2014_teamdignitas"));
				strcat(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			};
			CONST PWCHAR CONST wide_name = g_localize->Find(sticker_name_ptr);
			auto name = converter.to_bytes(wide_name);
			for (iter = 0; iter < name.length(); iter++)
				if ((name.at(iter) >= 'a' && name.at(iter) <= 'z') || (name.at(iter) >= 'A' && name.at(iter) <= 'Z'))
					name.at(iter) = toupper(name.at(iter));
			k_stickers.push_back({ sticker_kit->id, name });
		};
		sort(k_stickers.begin(), k_stickers.end());
		k_stickers.insert(k_stickers.begin(), { 0, XorString("NONE") });
	};
};
