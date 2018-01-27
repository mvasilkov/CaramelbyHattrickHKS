
#include "SDK.hpp"
#include "Platform.hpp"
#include "Hooks.hpp"
#include "Configuration.hpp"
#include "nSkinz.hpp"

bool __fastcall FireEventClientSide(IGameEventManager2 * thisptr, VOID *, IGameEvent * pevent)
{
	static auto orig = g_game_event_manager_hook->GetOriginalFunction < FireEventClientSide_t >(9);

	if (!strcmp(pevent->GetName(), XorString("player_death")) && g_engine->GetPlayerForUserID(pevent->GetInt(XorString("attacker"), 0)) == g_engine->GetLocalPlayer())
		if (auto icon_override = g_config.GetIconOverride(pevent->GetString(XorString("weapon"), NULL)))
			pevent->SetString(XorString("weapon"), icon_override);

	return orig(thisptr, pevent);
};
