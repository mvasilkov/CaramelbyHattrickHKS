
#include "SDK.hpp"
#include "Platform.hpp"
#include "Configuration.hpp"
#include "ItemDefinitions.hpp"
#include "KitParser.hpp"
#include <imgui.h>

namespace ImGui
{
	FORCEINLINE CONST bool CONST ListBox(CONST CHAR * label, PINT current_item, function < CONST CHAR * (INT) > lambda, INT items_count, INT height_in_items)
	{
		return ImGui::ListBox(label, current_item, [](PVOID data, INT idx, CONST CHAR ** out_text)
		{
			*out_text = (*reinterpret_cast < function < CONST CHAR * (INT) > * > (data)) (idx);
			return true;
		}, &lambda, items_count, height_in_items);
	};
};

FORCEINLINE INT FilterChars(ImGuiTextEditCallbackData * pData)
{
	if (pData->EventChar >= 'A' && pData->EventChar <= 'Z')
	{

	}

	else if (pData->EventChar >= 'a' && pData->EventChar <= 'z')
	{

	}

	else if (pData->EventChar == '0' || (pData->EventChar >= '1' && pData->EventChar <= '9'))
	{

	}

	else if (pData->EventChar == '!' || pData->EventChar == '@' || pData->EventChar == '#' || pData->EventChar == '$' || pData->EventChar == '^' \
		|| pData->EventChar == '&' || pData->EventChar == '*' || pData->EventChar == '(' || pData->EventChar == ')' || pData->EventChar == '-' \
		|| pData->EventChar == '_' || pData->EventChar == ' ' || pData->EventChar == '=' || pData->EventChar == '+' || pData->EventChar == '[' \
		|| pData->EventChar == ']' || pData->EventChar == '|' || pData->EventChar == ',' || pData->EventChar == '.' || pData->EventChar == '?' \
		|| pData->EventChar == '<' || pData->EventChar == '>' || pData->EventChar == '/')
	{

	}

	else
	{
		pData->EventChar = '\0';
		return 1;
	};

	return 0;
};

CONST VOID CONST DrawGUI(VOID)
{
	static INT selected_id = 0, selected_sticker_slot = 0;

	ImGui::SetNextWindowSize(ImVec2(500, 380));

	if (ImGui::Begin(XorString("CARAMEL's Skins by Hattrick HKS"), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings))
	{
		auto & entries = g_config.GetItems();
		if (entries.size() == 0)
			entries.push_back(EconomyItem_t());

		ImGui::Columns(2);
		ImGui::PushItemWidth(-1);

		CHAR element_name[XS_RANDOM_SIZE] = {};
		ImGui::ListBox(XorString("##Skins"), &selected_id, [&element_name, &entries](int idx)
		{
			xprintf(element_name, XorString("%s (%s)"), entries.at(idx).name, k_weapon_names.at(entries.at(idx).definition_vector_index).name.c_str());
			return element_name;
		}, entries.size(), 12);

		if (ImGui::Button(XorString("Add")))
			entries.push_back(EconomyItem_t()), selected_id = entries.size() - 1;
		ImGui::SameLine();

		if (ImGui::Button(XorString("Remove")) && entries.size() > 1)
			entries.erase(entries.begin() + selected_id);

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		selected_id = selected_id < int(entries.size()) ? selected_id : entries.size() - 1;
		auto & selected_entry = entries[selected_id];

		ImGui::InputText(XorString("Entry"), selected_entry.name, XS_RANDOM_SIZE / 10, ImGuiInputTextFlags_CallbackCharFilter, FilterChars);
		ImGui::Combo(XorString("Item"), &selected_entry.definition_vector_index, [](void *, int idx, const char ** out_text)
		{
			*out_text = k_weapon_names[idx].name.c_str();
			return true;
		}, nullptr, k_weapon_names.size());

		ImGui::Checkbox(XorString("Enabled"), &selected_entry.enabled);

		if (selected_entry.definition_index != GLOVE_T_SIDE)
		{
			ImGui::InputInt(XorString("Seed"), &selected_entry.seed);
			ImGui::InputInt(XorString("StatTrak"), &selected_entry.stat_trak);
		};

		ImGui::SliderFloat(XorString("Wear"), &selected_entry.wear, .00015f, 1.f, XorString("%.10f"), 5.f);

		if (selected_entry.definition_index != GLOVE_T_SIDE)
		{
			ImGui::Combo(XorString("Skin"), &selected_entry.paint_kit_vector_index, [](void *, int idx, const char ** out_text)
			{
				*out_text = k_skins[idx].name.c_str();
				return true;
			}, nullptr, k_skins.size());
		}
		else
		{
			ImGui::Combo(XorString("Skin"), &selected_entry.paint_kit_vector_index, [](void *, int idx, const char ** out_text)
			{
				*out_text = k_gloves[idx].name.c_str();
				return true;
			}, nullptr, k_gloves.size());
		}

		ImGui::Combo(XorString("Quality"), &selected_entry.entity_quality_vector_index, [](void *, int idx, const char ** out_text)
		{
			*out_text = k_quality_names[idx].name.c_str();
			return true;
		}, nullptr, k_quality_names.size());

		selected_entry.UpdateValues();

		if (selected_entry.definition_index == WEAPON_KNIFE)
		{
			ImGui::Combo(XorString("Knife Type"), &selected_entry.definition_override_vector_index, [](void *, int idx, const char ** out_text)
			{
				*out_text = k_knife_names.at(idx).name.c_str();
				return true;
			}, nullptr, k_knife_names.size());
		}
		else if (selected_entry.definition_index == GLOVE_T_SIDE)
		{
			ImGui::Combo(XorString("Gloves Type"), &selected_entry.definition_override_vector_index, [](void *, int idx, const char ** out_text)
			{
				*out_text = k_glove_names.at(idx).name.c_str();
				return true;
			}, nullptr, k_glove_names.size());
		}
		else selected_entry.definition_override_vector_index = 0;

		selected_entry.UpdateValues();

		if (selected_entry.definition_index != GLOVE_T_SIDE) ImGui::InputText(XorString("Tag"), selected_entry.custom_name, XS_RANDOM_SIZE / 10, ImGuiInputTextFlags_CallbackCharFilter, FilterChars);
		else selected_entry.custom_name[0] = '\0';

		ImGui::Columns(1), \
			ImGui::Columns(2);

		if (selected_entry.definition_index != WEAPON_KNIFE && selected_entry.definition_index != GLOVE_T_SIDE)
		{
			auto & selected_sticker = selected_entry.stickers[selected_sticker_slot];
			ImGui::PushItemWidth(-1);

			if (selected_entry.definition_index == WEAPON_REVOLVER || selected_entry.definition_index == WEAPON_G3SG1)
			{
				ImGui::ListBox(XorString("##Stickers"), &selected_sticker_slot, [&selected_entry, &element_name](int idx)
				{
					auto kit_vector_index = selected_entry.stickers[idx].kit_vector_index;
					xprintf(element_name, XorString("%d. %s"), idx + 1, k_stickers.at(kit_vector_index).name.c_str());
					return element_name;
				}, 5, 5);
			}
			else
			{
				ImGui::ListBox(XorString("##Stickers"), &selected_sticker_slot, [&selected_entry, &element_name](int idx)
				{
					auto kit_vector_index = selected_entry.stickers[idx].kit_vector_index;
					xprintf(element_name, XorString("%d. %s"), idx + 1, k_stickers.at(kit_vector_index).name.c_str());
					return element_name;
				}, 4, 4);
			};

			ImGui::PopItemWidth();
			ImGui::NextColumn();

			ImGui::Combo(XorString("Sticker"), &selected_sticker.kit_vector_index, [](void *, int idx, const char ** out_text)
			{
				*out_text = k_stickers.at(idx).name.c_str();
				return true;
			}, nullptr, k_stickers.size());

			ImGui::SliderFloat(XorString("Scratch"), &selected_sticker.wear, .00015f, 1.f, XorString("%.10f"), 5.f);
			ImGui::SliderFloat(XorString("Scale"), &selected_sticker.scale, .1f, 5.f, XorString("%.1f"));
			ImGui::SliderFloat(XorString("Rotation"), &selected_sticker.rotation, 0.f, 360.f, XorString("%.0f"));
		};

		ImGui::Columns(1);
		ImGui::PushItemWidth(-1);

		if (ImGui::Button(XorString("Update"))) (*g_client_state)->ForceFullUpdate();
		ImGui::SameLine();

		if (ImGui::Button(XorString("Save"))) g_config.Save();
		ImGui::SameLine();

		if (ImGui::Button(XorString("Load"))) g_config.Load();

		ImGui::PopItemWidth();
		ImGui::End();
	};
};
