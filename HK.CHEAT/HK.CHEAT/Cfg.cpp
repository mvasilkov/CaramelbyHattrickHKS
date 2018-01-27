
#include "Misc.h"

VOID CConfig::Setup(VOID)
{

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	static string Name;

#endif

	SetupVal(Vars.LEGITBOT.AimHelpers.fHitPointsScale, .55f, XorString("AimHelpers"), XorString("fHitPointsScale"));
	SetupVal(Vars.LEGITBOT.AimHelpers.bLimitAim, false, XorString("AimHelpers"), XorString("bLimitAim"));
	SetupVal(Vars.LEGITBOT.AimHelpers.fAimLimit, 0.f, XorString("AimHelpers"), XorString("fAimLimit"));

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	SetupVal(Vars.RageBOT.bON, false, XorString("RageBOT"), XorString("bON"));
	SetupVal(Vars.RageBOT.fFoV, .8f, XorString("RageBOT"), XorString("fFoV"));

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	SetupVal(Vars.RageBOT.bHeavy, false, XorString("RageBOT"), XorString("bHeavy"));
	SetupVal(Vars.RageBOT.iMinDamagePistols, 16, XorString("RageBOT"), XorString("iMinDamagePistols"));
	SetupVal(Vars.RageBOT.iMinDamageSnipers, 32, XorString("RageBOT"), XorString("iMinDamageSnipers"));
	SetupVal(Vars.RageBOT.iMinDamageRifles, 16, XorString("RageBOT"), XorString("iMinDamageRifles"));

#endif

#endif

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	SetupVal(Vars.LEGITBOT.AimBOT.bON, false, XorString("AimBOT"), XorString("bON"));
	SetupVal(Vars.LEGITBOT.AimBOT.fFoV, .6f, XorString("AimBOT"), XorString("fFoV"));

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	SetupVal(Vars.LEGITBOT.TriggerBOT.bON, false, XorString("TriggerBOT"), XorString("bON"));
	SetupVal(Vars.LEGITBOT.TriggerBOT.bAuto, false, XorString("TriggerBOT"), XorString("bAuto"));
	SetupVal(Vars.LEGITBOT.TriggerBOT.fDelay, .3f, XorString("TriggerBOT"), XorString("fDelay"));
	SetupVal(Vars.LEGITBOT.TriggerBOT.iKey, 0, XorString("TriggerBOT"), XorString("iKey"));

#endif

	SetupVal(Vars.Visuals.bON, false, XorString("Visuals"), XorString("bON"));
	SetupVal(Vars.Visuals.bDrawSkeleton, false, XorString("Visuals"), XorString("bDrawSkeleton"));

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

	SetupVal(Vars.Visuals.iVisibleGlowColor, 0, XorString("Visuals"), XorString("iVisibleGlowColor"));
	SetupVal(Vars.Visuals.iOccludedGlowColor, 0, XorString("Visuals"), XorString("iOccludedGlowColor"));
	SetupVal(Vars.Visuals.bGlow, false, XorString("Visuals"), XorString("bGlow"));
	SetupVal(Vars.Visuals.fGlowBloomAmount, .5f, XorString("Visuals"), XorString("fGlowBloomAmount"));
	SetupVal(Vars.Visuals.iGlowAlphaIntensity, 255, XorString("Visuals"), XorString("iGlowAlphaIntensity"));

#endif

	SetupVal(Vars.Visuals.bRadar, false, XorString("Visuals"), XorString("bRadar"));
	SetupVal(Vars.Visuals.bDrawHitPoints, false, XorString("Visuals"), XorString("bDrawHitPoints"));
	SetupVal(Vars.Visuals.bDrawHitPointsOnlyHead, false, XorString("Visuals"), XorString("bDrawHitPointsOnlyHead"));
	SetupVal(Vars.Visuals.iHitPointsColor, 0, XorString("Visuals"), XorString("iHitPointsColor"));
	SetupVal(Vars.Visuals.iEnemySightColor, 0, XorString("Visuals"), XorString("iEnemySightColor"));
	SetupVal(Vars.Visuals.bDrawEnemySight, false, XorString("Visuals"), XorString("bDrawEnemySight"));

	SetupVal(Vars.Visuals.Filter.bWeapons, true, XorString("Visuals"), XorString("bFilterWeapons"));
	SetupVal(Vars.Visuals.Filter.bDecoy, true, XorString("Visuals"), XorString("bFilterDecoy"));
	SetupVal(Vars.Visuals.Filter.bC4Bomb, true, XorString("Visuals"), XorString("bFilterC4Bomb"));
	SetupVal(Vars.Visuals.Filter.bHostages, true, XorString("Visuals"), XorString("bFilterHostages"));
	SetupVal(Vars.Visuals.Filter.bEnemyHealth, true, XorString("Visuals"), XorString("bFilterEnemyHealth"));
	SetupVal(Vars.Visuals.Filter.bEnemyName, true, XorString("Visuals"), XorString("bFilterEnemyName"));
	SetupVal(Vars.Visuals.Filter.bEnemyWeapon, true, XorString("Visuals"), XorString("bFilterEnemyWeapon"));
	SetupVal(Vars.Visuals.Filter.bEnemyReloading, true, XorString("Visuals"), XorString("bFilterEnemyReloading"));
	SetupVal(Vars.Visuals.Filter.bEnemyFlashed, true, XorString("Visuals"), XorString("bFilterEnemyFlashed"));
	SetupVal(Vars.Visuals.Filter.bEnemyDefusing, true, XorString("Visuals"), XorString("bFilterEnemyDefusing"));

	SetupVal(Vars.Misc.bRevealRanks, true, XorString("Misc"), XorString("bRevealRanks"));
	SetupVal(Vars.Misc.bGrenadeTraces, true, XorString("Misc"), XorString("bGrenadeTraces"));
	SetupVal(Vars.Misc.bDoNotEnforceTopSettings, false, XorString("Misc"), XorString("bDoNotEnforceTopSettings"));
	SetupVal(Vars.Misc.bAutoSniperCrosshair, true, XorString("Misc"), XorString("bAutoSniperCrosshair"));
	SetupVal(Vars.Misc.iSniperCrosshairColor, 0, XorString("Misc"), XorString("iSniperCrosshairColor"));
	SetupVal(Vars.Misc.bGrenadesPrediction, true, XorString("Misc"), XorString("bGrenadesPrediction"));
	SetupVal(Vars.Misc.bDroppedWeaponsName, false, XorString("Misc"), XorString("bDroppedWeaponsName"));
	SetupVal(Vars.Misc.bRecoilCrosshair, false, XorString("Misc"), XorString("bRecoilCrosshair"));
	SetupVal(Vars.Misc.iRecoilCrosshairType, 0, XorString("Misc"), XorString("iRecoilCrosshairType"));
	SetupVal(Vars.Misc.bBombSiteSkeletons, false, XorString("Misc"), XorString("bBombSiteSkeletons"));
	SetupVal(Vars.Misc.bDoNotEditRadarScale, false, XorString("Misc"), XorString("bDoNotEditRadarScale"));

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	SetupVal(Vars.Misc.bSpam, false, XorString("Misc"), XorString("bSpam"));
	SetupVal(Vars.Misc.bSpamRandomEnemies, true, XorString("Misc"), XorString("bSpamRandomEnemies"));
	SetupVal(Vars.Misc.bSpamInsultEnemy, true, XorString("Misc"), XorString("bSpamInsultEnemy"));
	SetupVal(Vars.Misc.bSpamInsultVictim, true, XorString("Misc"), XorString("bSpamInsultVictim"));
	SetupVal(Vars.Misc.bSpamRandomTalk, true, XorString("Misc"), XorString("bSpamRandomTalk"));
	SetupVal(Vars.Misc.iSpamDelaySeconds, 3, XorString("Misc"), XorString("iSpamDelaySeconds"));

#endif

	SetupVal(Vars.Misc.bC4BombTimer, false, XorString("Misc"), XorString("bC4BombTimer"));

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	SetupVal(Vars.Misc.bBunnyHop, false, XorString("Misc"), XorString("bBunnyHop"));

#endif

	SetupVal(Vars.Misc.bAutoLeftRight, false, XorString("Misc"), XorString("bAutoLeftRight"));

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

	SetupVal(Vars.Misc.bAutoStrafe, false, XorString("Misc"), XorString("bAutoStrafe"));

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	SetupVal(Vars.Misc.bSpecialFlickShots, false, XorString("Misc"), XorString("bSpecialFlickShots"));

#endif

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	SetupVal(Vars.Misc.bAutoAccept, false, XorString("Misc"), XorString("bAutoAccept"));

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

	SetupVal(Vars.Misc.bFOV, false, XorString("Misc"), XorString("bFOV"));
	SetupVal(Vars.Misc.iFOV, 80, XorString("Misc"), XorString("iFOV"));
	SetupVal(Vars.Misc.iViewModelFOV, 120, XorString("Misc"), XorString("iViewModelFOV"));

	SetupVal(Vars.Misc.bFOVPistols, false, XorString("Misc"), XorString("bFOVPistols"));
	SetupVal(Vars.Misc.iFOVPistols, 60, XorString("Misc"), XorString("iFOVPistols"));
	SetupVal(Vars.Misc.iViewModelFOVPistols, 120, XorString("Misc"), XorString("iViewModelFOVPistols"));

#endif

	SetupVal(Vars.Misc.bFlashbangReducer, false, XorString("Misc"), XorString("bFlashbangReducer"));
	SetupVal(Vars.Misc.iFlashbangAmount, 199, XorString("Misc"), XorString("iFlashbangAmount"));
	SetupVal(Vars.Misc.bDrawFoVCircle, false, XorString("Misc"), XorString("bDrawFoVCircle"));
	SetupVal(Vars.Misc.iFoVCircleColor, 0, XorString("Misc"), XorString("iFoVCircleColor"));
	SetupVal(Vars.Misc.iSkeletonColor, 0, XorString("Misc"), XorString("iSkeletonColor"));
	SetupVal(Vars.Misc.bEnableEnemyDetails, true, XorString("Misc"), XorString("bEnableEnemyDetails"));
	SetupVal(Vars.Misc.bEnableHackTradeMark, false, XorString("Misc"), XorString("bEnableHackTradeMark"));
	SetupVal(Vars.Misc.iFoVCircleType, 0, XorString("Misc"), XorString("iFoVCircleType"));

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	SetupVal(Vars.Misc.bFakeLag, false, XorString("Misc"), XorString("bFakeLag"));
	SetupVal(Vars.Misc.iFakeLagType, 0, XorString("Misc"), XorString("iFakeLagType"));

#endif

	SetupVal(Vars.Misc.bSpecTable, false, XorString("Misc"), XorString("bSpecTable"));
	SetupVal(Vars.Misc.fSpecTableX, .9f, XorString("Misc"), XorString("fSpecTableX"));
	SetupVal(Vars.Misc.fSpecTableY, .26f, XorString("Misc"), XorString("fSpecTableY"));
	SetupVal(Vars.Misc.iSpecTableColor, 0, XorString("Misc"), XorString("iSpecTableColor"));
	SetupVal(Vars.Misc.iFontHeight, FONT_HEIGHT, XorString("Misc"), XorString("iFontHeight"));

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	SetupVal(Vars.Misc.bClanTagRainbow, true, XorString("Misc"), XorString("bClanTagRainbow"));
	Name = XorString("Misc") + string(XorString(CFG_SEPARATOR)) + XorString("szClanTag");
	if (g_Json[Name].empty()) strcpy(Vars.Misc.szClanTag, XorString(CLAN_TAG)), g_Json[Name] = Vars.Misc.szClanTag;
	else strcpy(Vars.Misc.szClanTag, g_Json[Name].get < string >().c_str());
	Vars.Misc.szClanTag[12] = '\0';

#endif

};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

VOID CConfig::LoadRageBase(VOID)
{
	Vars.RageBOT.bON = true;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	Vars.RageBOT.bHeavy = false;

#endif

	Vars.RageBOT.fFoV = 1.f;

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	Vars.LEGITBOT.AimBOT.bON = false;

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.LEGITBOT.TriggerBOT.bON = false;

#endif

	Vars.Visuals.bRadar = true;
	Vars.Visuals.bON = true;
	Vars.Visuals.bDrawSkeleton = false;

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

	Vars.Visuals.bGlow = true;

#endif

	Vars.Visuals.Filter.bWeapons = true;
	Vars.Visuals.Filter.bDecoy = true;
	Vars.Visuals.Filter.bC4Bomb = true;
	Vars.Visuals.Filter.bHostages = true;
	Vars.Visuals.Filter.bEnemyHealth = true;
	Vars.Visuals.Filter.bEnemyName = true;
	Vars.Visuals.Filter.bEnemyWeapon = true;
	Vars.Visuals.Filter.bEnemyReloading = true;
	Vars.Visuals.Filter.bEnemyFlashed = true;
	Vars.Visuals.Filter.bEnemyDefusing = true;

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	Vars.Misc.bBunnyHop = true;

#endif

	Vars.Misc.bRevealRanks = true;
	Vars.Misc.bAutoSniperCrosshair = true;
	Vars.Misc.bGrenadeTraces = true;
	Vars.Misc.bGrenadesPrediction = true;

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.Misc.bSpecialFlickShots = true;

#endif

};

#endif

VOID CConfig::LoadLegitBase(VOID)
{

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	Vars.RageBOT.bON = false;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	Vars.RageBOT.bHeavy = false;

#endif

#endif

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	Vars.LEGITBOT.AimBOT.bON = true;
	Vars.LEGITBOT.AimBOT.fFoV = .6f;

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.LEGITBOT.TriggerBOT.bON = true;
	Vars.LEGITBOT.TriggerBOT.bAuto = true;
	Vars.LEGITBOT.TriggerBOT.fDelay = .3f;

#endif

	Vars.Visuals.bON = false;
	Vars.Visuals.bRadar = true;

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	Vars.Misc.bBunnyHop = true;

#endif

	Vars.Misc.bC4BombTimer = true;

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.Misc.bSpecialFlickShots = true;

#endif

	Vars.Misc.bRevealRanks = true;
	Vars.Misc.bAutoSniperCrosshair = true;
	Vars.Misc.bGrenadeTraces = true;
	Vars.Misc.bGrenadesPrediction = true;
	Vars.Misc.bBombSiteSkeletons = true;
	Vars.Misc.bEnableEnemyDetails = true;
};

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

VOID CConfig::LoadSpecialBase(VOID)
{

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	Vars.RageBOT.bON = false;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	Vars.RageBOT.bHeavy = false;

#endif

#endif

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	Vars.LEGITBOT.AimBOT.bON = false;

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.LEGITBOT.TriggerBOT.bON = true;
	Vars.LEGITBOT.TriggerBOT.bAuto = true;
	Vars.LEGITBOT.TriggerBOT.fDelay = .3f;

#endif

	Vars.Visuals.bON = false;
	Vars.Visuals.bDrawHitPoints = false;
	Vars.Visuals.bDrawEnemySight = false;
	Vars.Visuals.bRadar = false;

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	Vars.Misc.bBunnyHop = false;

#endif

	Vars.Misc.bC4BombTimer = false;

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.Misc.bSpecialFlickShots = false;

#endif

	Vars.Misc.bRevealRanks = false;
	Vars.Misc.bAutoSniperCrosshair = false;
	Vars.Misc.bGrenadeTraces = false;
	Vars.Misc.bGrenadesPrediction = false;
	Vars.Misc.bEnableEnemyDetails = true;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	Vars.Misc.bAutoAccept = false;

#endif

	Vars.Misc.bAutoLeftRight = false;

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

	Vars.Misc.bAutoStrafe = false;

#endif

	Vars.Misc.bBombSiteSkeletons = false;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	Vars.Misc.bSpam = false;

#endif

	Vars.Misc.bRecoilCrosshair = false;

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

	Vars.Misc.bFOVPistols = false;
	Vars.Misc.bFOV = false;

#endif

	Vars.Misc.bDroppedWeaponsName = false;
	Vars.Misc.bFlashbangReducer = false;
	Vars.Misc.bEnableHackTradeMark = false;
	Vars.Misc.bDrawFoVCircle = false;

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	Vars.Misc.bFakeLag = false;

#endif

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	Vars.Misc.bClanTagRainbow = false, Vars.Misc.szClanTag[0] = '\0', g_szFormattedClanTag[0] = '\0';

#endif

	Vars.Misc.bSpecTable = false;
};

#endif

VOID CConfig::Panic(VOID)
{

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	Vars.RageBOT.bON = false;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	Vars.RageBOT.bHeavy = false;

#endif

#endif

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	Vars.LEGITBOT.AimBOT.bON = false;

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.LEGITBOT.TriggerBOT.bON = false;

#endif

	Vars.Visuals.bON = false;
	Vars.Visuals.bDrawHitPoints = false;
	Vars.Visuals.bDrawEnemySight = false;
	Vars.Visuals.bRadar = false;

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	Vars.Misc.bBunnyHop = false;

#endif

	Vars.Misc.bC4BombTimer = false;

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	Vars.Misc.bSpecialFlickShots = false;

#endif

	Vars.Misc.bRevealRanks = false;
	Vars.Misc.bAutoSniperCrosshair = false;
	Vars.Misc.bGrenadeTraces = false;
	Vars.Misc.bGrenadesPrediction = false;
	Vars.Misc.bEnableEnemyDetails = true;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	Vars.Misc.bAutoAccept = false;

#endif

	Vars.Misc.bAutoLeftRight = false;

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

	Vars.Misc.bAutoStrafe = false;

#endif

	Vars.Misc.bBombSiteSkeletons = false;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	Vars.Misc.bSpam = false;

#endif

	Vars.Misc.bRecoilCrosshair = false;

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

	Vars.Misc.bFOVPistols = false;
	Vars.Misc.bFOV = false;

#endif

	Vars.Misc.bDroppedWeaponsName = false;
	Vars.Misc.bFlashbangReducer = false;
	Vars.Misc.bEnableHackTradeMark = false;
	Vars.Misc.bDrawFoVCircle = false;

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	Vars.Misc.bFakeLag = false;

#endif

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	Vars.Misc.bClanTagRainbow = false, Vars.Misc.szClanTag[0] = '\0', g_szFormattedClanTag[0] = '\0';

#endif

	Vars.Misc.bSpecTable = false;
};

VOID CConfig::SetupVal(INT & Val, INT Def, string Cat, string Key)
{
	static string Name;
	Name = Cat + XorString(CFG_SEPARATOR) + Key;
	if (!g_Json[Name].empty()) Val = atoi(g_Json[Name].get < string >().c_str());
	else Val = Def, g_Json[Name] = to_string(Def);
	g_pInts.push_back(make_pair(&Val, Name));
};

VOID CConfig::SetupVal(FLOAT & Val, FLOAT Def, string Cat, string Key)
{
	static string Name;
	Name = Cat + XorString(CFG_SEPARATOR) + Key;
	if (!g_Json[Name].empty()) Val = atof(g_Json[Name].get < string >().c_str());
	else Val = Def, g_Json[Name] = to_string(Def);
	g_pFloats.push_back(make_pair(&Val, Name));
};

VOID CConfig::SetupVal(bool & Val, bool Def, string Cat, string Key)
{
	static string Name;
	Name = Cat + XorString(CFG_SEPARATOR) + Key;
	if (!g_Json[Name].empty()) Val = !stricmp(g_Json[Name].get < string >().c_str(), XorString("True")) ? true : false;
	else Val = Def, g_Json[Name] = Def ? XorString("true") : XorString("false");
	g_pBools.push_back(make_pair(&Val, Name));
};

VOID CConfig::Save(VOID)
{
	static INT Iter = 0;
	static ofstream To;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	Vars.Misc.szClanTag[12] = '\0', g_Json[XorString("Misc") + string(XorString(CFG_SEPARATOR)) + XorString("szClanTag")] = Vars.Misc.szClanTag;

#endif

	for (Iter = 0; Iter < g_pInts.size(); Iter++) g_Json[g_pInts.at(Iter).second] = to_string(*g_pInts.at(Iter).first);
	for (Iter = 0; Iter < g_pFloats.size(); Iter++) g_Json[g_pFloats.at(Iter).second] = to_string(*g_pFloats.at(Iter).first);
	for (Iter = 0; Iter < g_pBools.size(); Iter++) g_Json[g_pBools.at(Iter).second] = *g_pBools.at(Iter).first ? XorString("true") : XorString("false");

	To.open(XorString(CFG_FILE));
	if (To.is_open()) To << g_Json, To.close();
};

VOID CConfig::Load(VOID)
{
	static INT Iter = 0;
	static ifstream From;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	static string Name;

#endif

	From.open(XorString(CFG_FILE));
	if (From.is_open()) From >> g_Json, From.close();

	for (Iter = 0; Iter < g_pInts.size(); Iter++)
		if (!g_Json[g_pInts.at(Iter).second].empty()) *g_pInts.at(Iter).first = atoi(g_Json[g_pInts.at(Iter).second].get < string >().c_str());

	for (Iter = 0; Iter < g_pFloats.size(); Iter++)
		if (!g_Json[g_pFloats.at(Iter).second].empty()) *g_pFloats.at(Iter).first = atof(g_Json[g_pFloats.at(Iter).second].get < string >().c_str());

	for (Iter = 0; Iter < g_pBools.size(); Iter++)
		if (!g_Json[g_pBools.at(Iter).second].empty()) *g_pBools.at(Iter).first = !stricmp(g_Json[g_pBools.at(Iter).second].get < string >().c_str(), XorString("True")) ? true : false;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

	Name = XorString("Misc") + string(XorString(CFG_SEPARATOR)) + XorString("szClanTag");
	if (!g_Json[Name].empty()) strcpy(Vars.Misc.szClanTag, g_Json[Name].get < string >().c_str());
	Vars.Misc.szClanTag[12] = '\0';

#endif

};

VOID CConfig::LoadStartup(VOID)
{
	static ifstream From;
	From.open(XorString(CFG_FILE));
	if (From.is_open()) From >> g_Json, From.close();
};
