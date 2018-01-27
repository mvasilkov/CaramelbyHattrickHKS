
#include "SDK.hpp"
#include "Platform.hpp"
#include "Hooks.hpp"
#include "ItemDefinitions.hpp"
#include "nSkinz.hpp"
#include "Configuration.hpp"

VOID ApplyStickerHooks(C_BaseAttributableItem *);

VOID EraseOverrideIfExistsByIndex(INT definition_index)
{
	if (k_weapon_info.count(definition_index))
	{
		auto & icon_override_map = g_config.GetIconOverrideMap();
		CONST auto & original_item = k_weapon_info.at(definition_index);
		if (original_item.icon.length() > 0 && icon_override_map.count(original_item.icon))
			icon_override_map.erase(icon_override_map.at(original_item.icon));
	};
};

VOID ApplyConfigOnAttributableItem(C_BaseAttributableItem * item, CONST EconomyItem_t * config, unsigned xuid_low)
{
	static unsigned old_definition_index = {};
	item->GetItemIDHigh() = -1, item->GetAccountID() = xuid_low;
	if (config->entity_quality_index) item->GetEntityQuality() = config->entity_quality_index;
	if (config->custom_name[0]) strcpy(item->GetCustomName(), config->custom_name);
	if (config->paint_kit_index) item->GetFallbackPaintKit() = config->paint_kit_index;
	if (config->seed) item->GetFallbackSeed() = config->seed;
	if (config->stat_trak) item->GetFallbackStatTrak() = config->stat_trak;
	item->GetFallbackWear() = config->wear;
	auto & definition_index = item->GetItemDefinitionIndex();
	if (config->definition_override_index && config->definition_override_index != definition_index && k_weapon_info.count(config->definition_override_index))
	{
		old_definition_index = definition_index;
		definition_index = config->definition_override_index;
		CONST auto & replacement_item = k_weapon_info.at(config->definition_override_index);
		item->SetModelIndex(g_model_info->GetModelIndex(replacement_item.model.c_str())), item->GetClientNetworkable()->PreDataUpdate(0);
		if (old_definition_index && k_weapon_info.count(old_definition_index))
		{
			CONST auto & original_item = k_weapon_info.at(old_definition_index);
			if (original_item.icon.length() > 0 && replacement_item.icon.length() > 0)
			{
				auto & icon_override_map = g_config.GetIconOverrideMap();
				icon_override_map[original_item.icon] = replacement_item.icon;
			};
		};
	}
	else EraseOverrideIfExistsByIndex(definition_index);
	ApplyStickerHooks(item);
};

CreateClientClassFn GetWearableCreateFn(VOID)
{
	auto pClass = g_client->GetAllClasses();
	while (strcmp(pClass->m_pNetworkName, XorString("CEconWearable"))) pClass = pClass->m_pNext;
	return pClass->m_pCreateFn;
};

VOID PostDataUpdateStart(VOID)
{
	static player_info_t player_info = {};
	static size_t iter = 0;
	static INT local_index = 0, entry = 0, serial = 0, definition_index = 0;
	static CBaseHandle glove_handle = 0, *wearables = 0, *weapons = 0, view_model_handle = 0, view_model_weapon_handle = 0;
	static EconomyItem_t * glove_config = 0;
	static C_BaseAttributableItem * glove = 0, *our_glove = 0, *weapon = 0, *view_model_weapon = 0;
	static C_BasePlayer * local = 0;
	static C_BaseViewModel * view_model = 0;
	local_index = g_engine->GetLocalPlayer(), local = static_cast <C_BasePlayer *> (g_entity_list->GetClientEntity(local_index));
	if (!local || !g_engine->GetPlayerInfo(local_index, &player_info)) return;
	wearables = local->GetWearables(), glove_config = g_config.GetByDefinitionIndex(GLOVE_T_SIDE), glove = reinterpret_cast <C_BaseAttributableItem *> (g_entity_list->GetClientEntityFromHandle(wearables[0]));
	if (!glove)
	{
		our_glove = reinterpret_cast <C_BaseAttributableItem *> (g_entity_list->GetClientEntityFromHandle(glove_handle));
		if (our_glove) wearables[0] = glove_handle, glove = our_glove;
	};
	if (local->GetLifeState() != LifeState::ALIVE)
	{
		if (glove) glove->GetClientNetworkable()->SetDestroyedOnRecreateEntities(), glove->GetClientNetworkable()->Release();
		return;
	};
	if (glove_config && glove_config->definition_override_index)
	{
		if (!glove)
		{
			static auto cw = GetWearableCreateFn();
			entry = g_entity_list->GetHighestEntityIndex() + 1, serial = rand() % 0x00001000, cw(entry, serial), glove = reinterpret_cast <C_BaseAttributableItem *> (g_entity_list->GetClientEntity(entry));
			wearables[0] = entry | serial << 16, glove_handle = wearables[0];
		};
		ApplyConfigOnAttributableItem(glove, glove_config, player_info.xuid_low);
	};
	weapons = local->GetWeapons();
	for (iter = 0; weapons[iter] != INVALID_EHANDLE_INDEX; iter++)
	{
		weapon = static_cast <C_BaseAttributableItem *> (g_entity_list->GetClientEntityFromHandle(weapons[iter]));
		if (!weapon) continue;
		auto & definition_index = weapon->GetItemDefinitionIndex();
		if (auto active_conf = g_config.GetByDefinitionIndex(IsKnife(definition_index) ? WEAPON_KNIFE : definition_index))
			ApplyConfigOnAttributableItem(weapon, active_conf, player_info.xuid_low);
		else EraseOverrideIfExistsByIndex(definition_index);
	};
	view_model_handle = local->GetViewModel();
	if (view_model_handle == INVALID_EHANDLE_INDEX) return;
	view_model = static_cast <C_BaseViewModel *> (g_entity_list->GetClientEntityFromHandle(view_model_handle));
	if (!view_model) return;
	view_model_weapon_handle = view_model->GetWeapon();
	if (view_model_weapon_handle == INVALID_EHANDLE_INDEX) return;
	view_model_weapon = static_cast <C_BaseAttributableItem *> (g_entity_list->GetClientEntityFromHandle(view_model_weapon_handle));
	if (!view_model_weapon || !k_weapon_info.count((definition_index = view_model_weapon->GetItemDefinitionIndex()))) return;
	view_model->GetModelIndex() = (local_index = g_model_info->GetModelIndex(k_weapon_info.at(definition_index).model.c_str()));
};

VOID __fastcall FrameStageNotify(IBaseClientDLL * thisptr, VOID *, ClientFrameStage_t stage)
{
	static auto original_fn = g_client_hook->GetOriginalFunction <FrameStageNotify_t>(36);
	if (stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) PostDataUpdateStart();
	return original_fn(thisptr, stage);
};
