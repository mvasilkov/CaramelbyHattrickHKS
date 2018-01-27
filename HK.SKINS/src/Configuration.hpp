
#pragma once

#include "SDK.hpp"
#include "Platform.hpp"
#include "ItemDefinitions.hpp"
#include "KitParser.hpp"

class StickerSetting
{
public:

	FORCEINLINE VOID UpdateValues(VOID)
	{
		kit_vector_index = kit_vector_index < k_stickers.size() ? kit_vector_index : k_stickers.size() - 1;
		kit_index = k_stickers.at(kit_vector_index).id;
	};

	FORCEINLINE VOID UpdateIds(VOID)
	{
		kit_vector_index = find_if(k_stickers.begin(), k_stickers.end(), [this](CONST Kit_t & X)
		{
			return kit_index == X.id;
		}) - k_stickers.begin();
	};

	INT kit_index = 0, kit_vector_index = 0;
	FLOAT wear = .00015f, scale = 1.f, rotation = 0.f;
};

class EconomyItem_t
{
public:

	FORCEINLINE VOID UpdateValues(VOID)
	{
		definition_vector_index = definition_vector_index < k_weapon_names.size() ? definition_vector_index : k_weapon_names.size() - 1;
		definition_index = k_weapon_names.at(definition_vector_index).definition_index;
		entity_quality_vector_index = entity_quality_vector_index < k_quality_names.size() ? entity_quality_vector_index : k_quality_names.size() - 1;
		entity_quality_index = k_quality_names.at(entity_quality_vector_index).index;

		if (definition_index == GLOVE_T_SIDE)
		{
			paint_kit_vector_index = paint_kit_vector_index < k_gloves.size() ? paint_kit_vector_index : k_gloves.size() - 1;
			paint_kit_index = k_gloves.at(paint_kit_vector_index).id;
			definition_override_vector_index = definition_override_vector_index < k_glove_names.size() ? definition_override_vector_index : k_glove_names.size() - 1;
			definition_override_index = k_glove_names.at(definition_override_vector_index).definition_index;
		}

		else
		{
			paint_kit_vector_index = paint_kit_vector_index < k_skins.size() ? paint_kit_vector_index : k_skins.size() - 1;
			paint_kit_index = k_skins.at(paint_kit_vector_index).id;
			definition_override_vector_index = definition_override_vector_index < k_knife_names.size() ? definition_override_vector_index : k_knife_names.size() - 1;
			definition_override_index = k_knife_names.at(definition_override_vector_index).definition_index;
		};

		for (auto & Sticker : stickers)
			Sticker.UpdateValues();
	};

	FORCEINLINE VOID UpdateIds(VOID)
	{
		definition_vector_index = find_if(k_weapon_names.begin(), k_weapon_names.end(), [this](CONST WeaponName_t & X)
		{
			return definition_index == X.definition_index;
		}) - k_weapon_names.begin();

		entity_quality_vector_index = find_if(k_quality_names.begin(), k_quality_names.end(), [this](CONST QualityName_t & X)
		{
			return this->entity_quality_index == X.index;
		}) - k_quality_names.begin();

		const auto & skin_set = definition_index == GLOVE_T_SIDE ? k_gloves : k_skins;

		paint_kit_vector_index = find_if(skin_set.begin(), skin_set.end(), [this](CONST Kit_t & X)
		{
			return paint_kit_index == X.id;
		}) - skin_set.begin();

		const auto & override_set = definition_index == GLOVE_T_SIDE ? k_glove_names : k_knife_names;

		definition_override_vector_index = find_if(override_set.begin(), override_set.end(), [this](CONST WeaponName_t & X)
		{
			return definition_override_index == X.definition_index;
		}) - override_set.begin();

		for (auto & Sticker : stickers)
			Sticker.UpdateIds();
	};

	FORCEINLINE EconomyItem_t(VOID) {
		xprintf(name, XorString("FRESH NEW ENTRY"));
	};

	CHAR name[XS_RANDOM_SIZE] = {};
	bool enabled = false;
	INT definition_vector_index = 0, definition_index = 1, entity_quality_vector_index = 0, entity_quality_index = 0, \
		paint_kit_vector_index = 0, paint_kit_index = 0, definition_override_vector_index = 0, definition_override_index = 0, \
		seed = 0, stat_trak = 0;
	FLOAT wear = .00015f;
	CHAR custom_name[XS_RANDOM_SIZE] = {};
	array < StickerSetting, 5 > stickers;
};

class Config
{
public:

	FORCEINLINE Config(VOID) {
		m_items.reserve(XS_RANDOM_SIZE / 2);
	};

	VOID Save(VOID), Load(VOID);
	EconomyItem_t * GetByDefinitionIndex(INT);
	FORCEINLINE vector <EconomyItem_t> & GetItems(VOID) { return m_items; }
	FORCEINLINE unordered_map <string, string> & GetIconOverrideMap(VOID) { return m_icon_overrides; }
	FORCEINLINE CONST CHAR * GetIconOverride(CONST string & Orig) { return m_icon_overrides.count(Orig) ? m_icon_overrides.at(Orig).c_str() : nullptr; };
	vector < EconomyItem_t > m_items;
	unordered_map < string, string > m_icon_overrides;
};

extern Config g_config;
