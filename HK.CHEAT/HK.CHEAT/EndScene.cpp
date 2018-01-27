
#include "Misc.h"

LONG WINAPI EndScene(IDirect3DDevice9 * pDevice)
{
	static INT iX = 0, iY = 0;

	if (!Vars.Menu.bOpened)
		return oEndScene(pDevice);

	if (!g_bD3DOK)
		CreateGUI(pDevice, XorString(MENU_FONT_FILE));

	ImGuiIO & X = ImGui::GetIO();
	ImGui_ImplDX9_NewFrame();

	I::InputSys->GetCursorPosition(&iX, &iY), X.MousePos.x = (FLOAT)(iX), X.MousePos.y = (FLOAT)(iY), X.MouseDrawCursor = true;

	ImGui::SetNextWindowSize(ImVec2(800, 500));
	ImGui::Begin(XorString(MENU_TITLE), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);

	ImGui::Columns(4);
	ImGui::Text(XorString("EZ Set-Up"));

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	ImGui::Text(XorString("Very Safe Modes (Match-Making, ..)"));

	if (ImGui::Button(XorString("Mode Competitive 0.4"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.fFoV = .4f;
	if (ImGui::Button(XorString("Mode Competitive 0.6"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.fFoV = .6f;
	if (ImGui::Button(XorString("Mode Competitive 0.8"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.fFoV = .8f;
	if (ImGui::Button(XorString("Mode Competitive 1.0"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.fFoV = 1.f;

#endif

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	ImGui::Text(XorString("Safe Rage Modes (Are Risky)"));

	if (ImGui::Button(XorString("Mode Rage 0.6"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = .6f, Vars.Visuals.bON = false;
	if (ImGui::Button(XorString("Mode Rage 0.8"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = .8f, Vars.Visuals.bON = false;
	if (ImGui::Button(XorString("Mode Rage 1.0"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 1.f, Vars.Visuals.bON = false;

	ImGui::Text(XorString("High Rage Modes (Not For Match-Making)"));

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	if (ImGui::Button(XorString("Mode Rage 1.5"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 1.5f, Vars.RageBOT.bHeavy = false;
	if (ImGui::Button(XorString("Mode Rage 2.0"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 2.f, Vars.RageBOT.bHeavy = false;
	if (ImGui::Button(XorString("Mode Rage 3.0"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 3.f, Vars.RageBOT.bHeavy = false;
	if (ImGui::Button(XorString("Mode Rage+ 10"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 10.f, Vars.RageBOT.bHeavy = true;

#else

	if (ImGui::Button(XorString("Mode Rage 1.5"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 1.5f;
	if (ImGui::Button(XorString("Mode Rage 2.0"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 2.f;
	if (ImGui::Button(XorString("Mode Rage 3.0"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 3.f;
	if (ImGui::Button(XorString("Mode Rage+ 10"))) Config->LoadRageBase(), Vars.RageBOT.fFoV = 10.f;

#endif

#endif

	ImGui::Text(XorString("Non-Aim Modes (Match-Making, ..)"));

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (ImGui::Button(XorString("Mode Radar Only"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.bON = false, Vars.LEGITBOT.TriggerBOT.bON = false;

#else

	if (ImGui::Button(XorString("Mode Radar Only"))) Config->LoadLegitBase(), Vars.LEGITBOT.TriggerBOT.bON = false;

#endif

#else

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (ImGui::Button(XorString("Mode Radar Only"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.bON = false;

#else

	if (ImGui::Button(XorString("Mode Radar Only"))) Config->LoadLegitBase();

#endif

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (ImGui::Button(XorString("Mode Radar + Trigger"))) Config->LoadLegitBase(), Vars.LEGITBOT.AimBOT.bON = false;

#else

	if (ImGui::Button(XorString("Mode Radar + Trigger"))) Config->LoadLegitBase();

#endif

	if (ImGui::Button(XorString("Mode FaceIT (BETA)"))) Config->LoadSpecialBase();

#endif

	ImGui::Text(XorString("Save/ Load Configuration Files"));

	if (FLOAT(((DOUBLE)clock() - g_saveTime) / CLOCKS_PER_SEC) < FLOAT(1.F))
		ImGui::Button(XorString("SAVED"));

	else
	{
		if (ImGui::Button(XorString("SAVE")))
		{
			Config->Save(), g_saveTime = clock();

			if (g_nStyle >= 0)
				MakeFonts();
		};
	};

	if (FLOAT(((DOUBLE)clock() - g_loadTime) / CLOCKS_PER_SEC) < FLOAT(1.F))
		ImGui::Button(XorString("LOADED"));

	else
	{
		if (ImGui::Button(XorString("LOAD")))
		{
			Config->Load(), g_loadTime = clock();

			if (g_nStyle >= 0)
				MakeFonts();
		};
	};

	ImGui::Text(XorString("Hack's PANIC Button Is [END]"));
	ImGui::Text(XorString("Press [LOAD] For Restore"));
	ImGui::NextColumn();

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	if (!Vars.RageBOT.bON)
	{
		ImGui::Text(XorString("Legit Set-Up"));

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

		ImGui::Checkbox(XorString("Enables Aim BOT"), &Vars.LEGITBOT.AimBOT.bON);

		if (Vars.LEGITBOT.AimBOT.bON)
			ImGui::SliderFloat(XorString("Aim's FoV"), &Vars.LEGITBOT.AimBOT.fFoV, .1f, 3.f, XorString("%.1f"));

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

		ImGui::Checkbox(XorString("Enables Trigger BOT"), &Vars.LEGITBOT.TriggerBOT.bON);

		if (Vars.LEGITBOT.TriggerBOT.bON)
		{
			ImGui::Checkbox(XorString("Auto Hits When Needed"), &Vars.LEGITBOT.TriggerBOT.bAuto);

			if (!Vars.LEGITBOT.TriggerBOT.bAuto)
				ImGui::Combo(XorString("Hold Key"), &Vars.LEGITBOT.TriggerBOT.iKey, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pTriggerKeys[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pTriggerKeys));

			ImGui::SliderFloat(XorString("Seconds"), &Vars.LEGITBOT.TriggerBOT.fDelay, .1f, 1.f, XorString("%.1f"));
		};

#endif

		ImGui::Text(XorString("Hit Boxes (Points) Expansion"));
		ImGui::SliderFloat(XorString("Scale"), &Vars.LEGITBOT.AimHelpers.fHitPointsScale, .55f, .8f, XorString("%.2f"));

#if (defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0) || (defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0)

		if (Vars.LEGITBOT.AimBOT.bON)
		{
			ImGui::Checkbox(XorString("Limits Aim"), &Vars.LEGITBOT.AimHelpers.bLimitAim);

			if (Vars.LEGITBOT.AimHelpers.bLimitAim)
				ImGui::SliderFloat(XorString("Maximum"), &Vars.LEGITBOT.AimHelpers.fAimLimit, 0.f, 1.f, XorString("%.10f"), 5);
		};

#endif

	}

	else
	{
		ImGui::Text(XorString("Rage Set-Up"));

		ImGui::Checkbox(XorString("Enables Rage's Aim BOT"), &Vars.RageBOT.bON);

		if (Vars.RageBOT.bON)
			ImGui::SliderFloat(XorString("Rage's FoV"), &Vars.RageBOT.fFoV, .1f, 10.f, XorString("%.1f"));

		ImGui::Text(XorString("Hit Boxes (Points) Expansion"));
		ImGui::SliderFloat(XorString("Scale"), &Vars.LEGITBOT.AimHelpers.fHitPointsScale, .55f, .8f, XorString("%.2f"));

#if (defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0) || (defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0)

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

		if (!Vars.RageBOT.bHeavy)
		{

#endif

			if (Vars.RageBOT.bON)
			{
				ImGui::Checkbox(XorString("Limits Aim"), &Vars.LEGITBOT.AimHelpers.bLimitAim);

				if (Vars.LEGITBOT.AimHelpers.bLimitAim)
					ImGui::SliderFloat(XorString("Maximum"), &Vars.LEGITBOT.AimHelpers.fAimLimit, 0.f, 1.f, XorString("%.10f"), 5);
			};

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

		};

#endif

#endif

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

		ImGui::Checkbox(XorString("Enables Heavy Rage Module"), &Vars.RageBOT.bHeavy);

		if (Vars.RageBOT.bHeavy)
		{
			ImGui::Text(XorString("Sets Minimum Wall Damage"));

			ImGui::SliderInt(XorString("Pistols"), &Vars.RageBOT.iMinDamagePistols, 1, 200, XorString("%.0f"));
			ImGui::SliderInt(XorString("Rifles"), &Vars.RageBOT.iMinDamageRifles, 1, 200, XorString("%.0f"));
			ImGui::SliderInt(XorString("Snipers"), &Vars.RageBOT.iMinDamageSnipers, 1, 200, XorString("%.0f"));
		};

#endif

	};

#else

	ImGui::Text(XorString("Legit Set-Up"));

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	ImGui::Checkbox(XorString("Enables Aim BOT"), &Vars.LEGITBOT.AimBOT.bON);

	if (Vars.LEGITBOT.AimBOT.bON)
		ImGui::SliderFloat(XorString("Aim's FoV"), &Vars.LEGITBOT.AimBOT.fFoV, .1f, 3.f, XorString("%.1f"));

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	ImGui::Checkbox(XorString("Enables Trigger BOT"), &Vars.LEGITBOT.TriggerBOT.bON);

	if (Vars.LEGITBOT.TriggerBOT.bON)
	{
		ImGui::Checkbox(XorString("Auto Hits When Needed"), &Vars.LEGITBOT.TriggerBOT.bAuto);

		if (!Vars.LEGITBOT.TriggerBOT.bAuto)
			ImGui::Combo(XorString("Hold Key"), &Vars.LEGITBOT.TriggerBOT.iKey, [](PVOID, INT iId, CONST CHAR ** pOutput) {
			*pOutput = g_pTriggerKeys[iId].c_str();
			return true;
		}, nullptr, ARRAYSIZE(g_pTriggerKeys));

		ImGui::SliderFloat(XorString("Seconds"), &Vars.LEGITBOT.TriggerBOT.fDelay, .1f, 1.f, XorString("%.1f"));
	};

#endif

	ImGui::Text(XorString("Hit Boxes (Points) Expansion"));
	ImGui::SliderFloat(XorString("Scale"), &Vars.LEGITBOT.AimHelpers.fHitPointsScale, .55f, .8f, XorString("%.2f"));

#if (defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0) || (defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0)

	if (Vars.LEGITBOT.AimBOT.bON)
	{
		ImGui::Checkbox(XorString("Limits Aim"), &Vars.LEGITBOT.AimHelpers.bLimitAim);

		if (Vars.LEGITBOT.AimHelpers.bLimitAim)
			ImGui::SliderFloat(XorString("Maximum"), &Vars.LEGITBOT.AimHelpers.fAimLimit, 0.f, 1.f, XorString("%.10f"), 5);
	};

#endif

#endif

	ImGui::NextColumn();
	ImGui::Text(XorString("Wall Hacks Set-Up"));

	ImGui::Checkbox(XorString("Draws Hit Boxes (Points)"), &Vars.Visuals.bDrawHitPoints);

	if (Vars.Visuals.bDrawHitPoints)
	{
		ImGui::Checkbox(XorString("Only Head"), &Vars.Visuals.bDrawHitPointsOnlyHead);

		ImGui::Combo(XorString("Points"), &Vars.Visuals.iHitPointsColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
			*pOutput = g_pCustomColorOptions[iId].c_str();
			return true;
		}, nullptr, ARRAYSIZE(g_pCustomColorOptions));
	};

	ImGui::Checkbox(XorString("Draws Enemies' Sight"), &Vars.Visuals.bDrawEnemySight);

	if (Vars.Visuals.bDrawEnemySight)
		ImGui::Combo(XorString("Sight"), &Vars.Visuals.iEnemySightColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
		*pOutput = g_pCustomColorOptions[iId].c_str();
		return true;
	}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

	ImGui::Checkbox(XorString("Enables Radar Hack"), &Vars.Visuals.bRadar);
	ImGui::Checkbox(XorString("Enables Wall Hacks"), &Vars.Visuals.bON);

	if (Vars.Visuals.bON)
	{
		ImGui::Checkbox(XorString("Enables Skeleton"), &Vars.Visuals.bDrawSkeleton);

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

		ImGui::Checkbox(XorString("Enables Glow"), &Vars.Visuals.bGlow);

		if (Vars.Visuals.bGlow)
		{
			ImGui::Combo(XorString("Occluded"), &Vars.Visuals.iOccludedGlowColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pCustomGlowColorOptions[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pCustomGlowColorOptions));

			ImGui::Combo(XorString("Visible"), &Vars.Visuals.iVisibleGlowColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pCustomGlowColorOptions[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pCustomGlowColorOptions));

			ImGui::SliderInt(XorString("Alpha"), &Vars.Visuals.iGlowAlphaIntensity, 1, 255, XorString("%.0f"));
			ImGui::SliderFloat(XorString("Bloom"), &Vars.Visuals.fGlowBloomAmount, .1f, 1.f, XorString("%.1f"));
		};

#endif

		ImGui::Text(XorString("Wall Hacks' Features To Include"));
		ImGui::Checkbox(XorString("World Weapons"), &Vars.Visuals.Filter.bWeapons);
		ImGui::Checkbox(XorString("Hostages"), &Vars.Visuals.Filter.bHostages);
		ImGui::Checkbox(XorString("Decoy"), &Vars.Visuals.Filter.bDecoy);
		ImGui::Checkbox(XorString("C4 Bomb"), &Vars.Visuals.Filter.bC4Bomb);
		ImGui::Checkbox(XorString("Flashed Enemies"), &Vars.Visuals.Filter.bEnemyFlashed);
		ImGui::Checkbox(XorString("Defusing Enemies"), &Vars.Visuals.Filter.bEnemyDefusing);
		ImGui::Checkbox(XorString("Enemies' Health"), &Vars.Visuals.Filter.bEnemyHealth);
		ImGui::Checkbox(XorString("Enemies' Name"), &Vars.Visuals.Filter.bEnemyName);
		ImGui::Checkbox(XorString("Enemies' Weapon"), &Vars.Visuals.Filter.bEnemyWeapon);
		ImGui::Checkbox(XorString("Reloading Enemies"), &Vars.Visuals.Filter.bEnemyReloading);
	};

	ImGui::NextColumn();
	ImGui::Text(XorString("Miscellaneous Set-Up"));
	ImGui::Checkbox(XorString("Does Not Edit Radar's Scale"), &Vars.Misc.bDoNotEditRadarScale);
	ImGui::Checkbox(XorString("Does Not Enforce Ping Settings"), &Vars.Misc.bDoNotEnforceTopSettings);

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0 && defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (Vars.RageBOT.bON || Vars.LEGITBOT.AimBOT.bON)
	{
		ImGui::Checkbox(XorString("Draws FoV's Circle"), &Vars.Misc.bDrawFoVCircle);

		if (Vars.Misc.bDrawFoVCircle)
		{
			ImGui::Combo(XorString("Detail"), &Vars.Misc.iFoVCircleColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pCustomColorOptions[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

			ImGui::Combo(XorString("Style"), &Vars.Misc.iFoVCircleType, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pFoVCircles[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pFoVCircles));
		};
	};

#elif (!defined(ENABLE_RAGE_BOT) || ENABLE_RAGE_BOT != 1) && defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (Vars.LEGITBOT.AimBOT.bON)
	{
		ImGui::Checkbox(XorString("Draws FoV's Circle"), &Vars.Misc.bDrawFoVCircle);

		if (Vars.Misc.bDrawFoVCircle)
		{
			ImGui::Combo(XorString("Detail"), &Vars.Misc.iFoVCircleColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pCustomColorOptions[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

			ImGui::Combo(XorString("Style"), &Vars.Misc.iFoVCircleType, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pFoVCircles[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pFoVCircles));
		};
	};

#elif (!defined(ENABLE_AIM_BOT) || ENABLE_AIM_BOT != 1) && defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

	if (Vars.RageBOT.bON)
	{
		ImGui::Checkbox(XorString("Draws FoV's Circle"), &Vars.Misc.bDrawFoVCircle);

		if (Vars.Misc.bDrawFoVCircle)
		{
			ImGui::Combo(XorString("Detail"), &Vars.Misc.iFoVCircleColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pCustomColorOptions[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

			ImGui::Combo(XorString("Style"), &Vars.Misc.iFoVCircleType, [](PVOID, INT iId, CONST CHAR ** pOutput) {
				*pOutput = g_pFoVCircles[iId].c_str();
				return true;
			}, nullptr, ARRAYSIZE(g_pFoVCircles));
		};
	};

#endif

	ImGui::Checkbox(XorString("Enables Grenades' Trajectory"), &Vars.Misc.bGrenadeTraces);
	ImGui::Checkbox(XorString("Enables Grenades' Prediction"), &Vars.Misc.bGrenadesPrediction);
	ImGui::Checkbox(XorString("Enables Fake Snipers' Crosshair"), &Vars.Misc.bAutoSniperCrosshair);

	if (Vars.Misc.bAutoSniperCrosshair)
		ImGui::Combo(XorString("Color"), &Vars.Misc.iSniperCrosshairColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
		*pOutput = g_pCustomColorOptions[iId].c_str();
		return true;
	}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

	ImGui::Checkbox(XorString("Enables Recoil Crosshair"), &Vars.Misc.bRecoilCrosshair);

	if (Vars.Misc.bRecoilCrosshair)
		ImGui::Combo(XorString("Paint"), &Vars.Misc.iRecoilCrosshairType, [](PVOID, INT iId, CONST CHAR ** pOutput) {
		*pOutput = g_pCustomColorOptions[iId].c_str();
		return true;
	}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	ImGui::Checkbox(XorString("Enables Chat's Spamming Module"), &Vars.Misc.bSpam);

	if (Vars.Misc.bSpam)
	{
		ImGui::Checkbox(XorString("Random Insults Enemies"), &Vars.Misc.bSpamRandomEnemies);
		ImGui::Checkbox(XorString("Insults My Enemy"), &Vars.Misc.bSpamInsultEnemy);
		ImGui::Checkbox(XorString("Insults My Victim"), &Vars.Misc.bSpamInsultVictim);
		ImGui::Checkbox(XorString("Random Talks On Game Events"), &Vars.Misc.bSpamRandomTalk);
		ImGui::SliderInt(XorString("Delay"), &Vars.Misc.iSpamDelaySeconds, 2, 8, XorString("%.0f"));
	};

#endif

	if (!Vars.Visuals.bON)
	{
		ImGui::Checkbox(XorString("Dropped Weapons' Name"), &Vars.Misc.bDroppedWeaponsName);
		ImGui::Checkbox(XorString("Bomb Site Enemies' Skeleton"), &Vars.Misc.bBombSiteSkeletons);
		ImGui::Checkbox(XorString("C4's Timer"), &Vars.Misc.bC4BombTimer);
	};

	ImGui::Checkbox(XorString("Reveals Match-Making Ranks"), &Vars.Misc.bRevealRanks);

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	ImGui::Checkbox(XorString("Enables Bunny Hop"), &Vars.Misc.bBunnyHop);

#endif

	ImGui::Checkbox(XorString("Auto Left/ Right When Aimed"), &Vars.Misc.bAutoLeftRight);

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

	ImGui::Checkbox(XorString("Auto Strafe While In Air"), &Vars.Misc.bAutoStrafe);

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	ImGui::Checkbox(XorString("Special Flick Hits For Snipers"), &Vars.Misc.bSpecialFlickShots);

#endif

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	ImGui::Checkbox(XorString("Auto Accepts Match-Making Game"), &Vars.Misc.bAutoAccept);

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

	ImGui::Checkbox(XorString("Enables Big Guns FoV"), &Vars.Misc.bFOV);

	if (Vars.Misc.bFOV)
	{
		ImGui::SliderInt(XorString("W. View"), &Vars.Misc.iFOV, 60, 120, XorString("%.0f"));
		ImGui::SliderInt(XorString("W. Model"), &Vars.Misc.iViewModelFOV, 60, 120, XorString("%.0f"));
	};

	ImGui::Checkbox(XorString("Enables Hand Guns FoV"), &Vars.Misc.bFOVPistols);

	if (Vars.Misc.bFOVPistols)
	{
		ImGui::SliderInt(XorString("P. View"), &Vars.Misc.iFOVPistols, 60, 120, XorString("%.0f"));
		ImGui::SliderInt(XorString("P. Model"), &Vars.Misc.iViewModelFOVPistols, 60, 120, XorString("%.0f"));
	};

#endif

	ImGui::Checkbox(XorString("Enables Flashbang Reducer"), &Vars.Misc.bFlashbangReducer);

	if (Vars.Misc.bFlashbangReducer)
		ImGui::SliderInt(XorString("Amount"), &Vars.Misc.iFlashbangAmount, 1, 255, XorString("%.0f"));

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	ImGui::InputText(XorString("Clan Tag"), Vars.Misc.szClanTag, ARRAYSIZE(Vars.Misc.szClanTag), ImGuiInputTextFlags_CallbackCharFilter, FilterClanTag);

	if (strlen(Vars.Misc.szClanTag) > 1)
		ImGui::Checkbox(XorString("Clan Tag's Rainbow Effect"), &Vars.Misc.bClanTagRainbow);

	Vars.Misc.szClanTag[12] = '\0';

#endif

	if ((Vars.Visuals.bON && Vars.Visuals.bDrawSkeleton) || Vars.Misc.bBombSiteSkeletons)
		ImGui::Combo(XorString("Skeleton"), &Vars.Misc.iSkeletonColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
		*pOutput = g_pCustomColorOptions[iId].c_str();
		return true;
	}, nullptr, ARRAYSIZE(g_pCustomColorOptions));

	if (!Vars.Visuals.bON)
		ImGui::Checkbox(XorString("Enables On-Screen Enemies' Info."), &Vars.Misc.bEnableEnemyDetails);

	ImGui::Checkbox(XorString("Enables Hack's Trade Mark"), &Vars.Misc.bEnableHackTradeMark);

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	ImGui::Checkbox(XorString("Enables Fake Lag"), &Vars.Misc.bFakeLag);

	if (Vars.Misc.bFakeLag)
		ImGui::Combo(XorString("Fake Lag"), &Vars.Misc.iFakeLagType, [](PVOID, INT iId, CONST CHAR ** pOutput) {
		*pOutput = g_pFakeLagOptions[iId].c_str();
		return true;
	}, nullptr, ARRAYSIZE(g_pFakeLagOptions));

#endif

	ImGui::Checkbox(XorString("Enables Spectators Table"), &Vars.Misc.bSpecTable);

	if (Vars.Misc.bSpecTable)
	{
		ImGui::SliderFloat(XorString("X (W)"), &Vars.Misc.fSpecTableX, .72f, .92f, XorString("%.2f"));
		ImGui::SliderFloat(XorString("Y (H)"), &Vars.Misc.fSpecTableY, .2f, .72f, XorString("%.2f"));

		ImGui::Combo(XorString("Tone"), &Vars.Misc.iSpecTableColor, [](PVOID, INT iId, CONST CHAR ** pOutput) {
			*pOutput = g_pCustomColorOptions[iId].c_str();
			return true;
		}, nullptr, ARRAYSIZE(g_pCustomColorOptions));
	};

	ImGui::Text(XorString("Sets On-Screen Font's Height"));
	ImGui::SliderInt(XorString("Tall"), &Vars.Misc.iFontHeight, 8, 16, XorString("%.0f"));

	ImGui::Columns();
	ImGui::End();
	ImGui::Render();

	return oEndScene(pDevice);
};

LONG WINAPI Reset(IDirect3DDevice9 * pDevice, D3DPRESENT_PARAMETERS * pPresentationParameters)
{
	static LONG lRes = 0;

	ImGui_ImplDX9_InvalidateDeviceObjects();
	lRes = oReset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();

	if (g_nStyle >= 0)
		MakeFonts();

	return lRes;
};
