
#include "Misc.h"

Hattrick g_Hattrick;
CreateMoveFn oCreateMove = NULL;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

PlaySoundFn oPlaySound = NULL;
VTHook * H::Surf = NULL;

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

OverrideViewFn oOverrideView = NULL;
GetViewModelFoVFn oGetViewModelFoV = NULL;

#endif

bool g_bFrozen = false;
time_t g_roundStartTime = 0;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

CRBOT * g_pRageBOT = new CRBOT;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

bool g_bShouldMask = false;
TraceRayFn oTraceRay;

#endif

#endif

Color g_Red = Color::Red();
Color g_DarkRed = Color::DarkRed();
Color g_CandyRed = Color::CandyRed();
Color g_DarkBlue = Color::DarkBlue();
Color g_Blue = Color::Blue();
Color g_CandyBlue = Color::CandyBlue();
Color g_DarkGreen = Color::DarkGreen();
Color g_Cyan = Color::Cyan();
Color g_Green = Color::Green();
Color g_CandyGreen = Color::CandyGreen();
Color g_White = Color::White();
Color g_WhiteSmoke = Color::WhiteSmoke();
Color g_Grey = Color::Grey();
Color g_DarkGrey = Color::DarkGrey();
Color g_Black = Color::Black();
Color g_Magenta = Color::Magenta();
Color g_Violet = Color::Violet();
Color g_Yellow = Color::Yellow();
Color g_Orange = Color::Orange();
bool g_bLoaded = false;
time_t g_lastDoPlantMsg = 0;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

CHAR g_szFormattedClanTag[XS_RANDOM_SIZE] = {};

#endif

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

CHAR g_Phrases[SIZE_T(XS_RANDOM_SIZE * .333333f)][SIZE_T(XS_RANDOM_SIZE * .75f)];
INT g_PhrasesSize = 0;

#endif

bool g_bSmokeExcluded[XS_RANDOM_SIZE / 2] = { false };
time_t g_bombDropTime = 0;
PaintTraverseFn oPaintTraverse = NULL;
UINT g_uiDrawPanel = NULL;
time_t g_timeLastEventStamp = NULL;
CONST CHAR * g_pszPanelName = NULL;
json g_Json;
bool m_bVis[XS_RANDOM_SIZE / 2] = { false };
bool g_bRoundEnded = true;
clock_t g_saveTime = 0;
clock_t g_loadTime = 0;
ResetFn oReset;
EndSceneFn oEndScene;
WNDPROC g_pOldWndProc = NULL;
BOOL g_bCheckForLastEnemy = FALSE;
CNetVarMan * g_pNetVarsMan = new CNetVarMan;
INT g_nStyle = -1;
INT g_iCTScore = 0;
INT g_iTEScore = 0;
FLOAT G::fSelfVelLen = 0.f;
bool g_bBombPlanted = false;
CGrenTraj g_nadeTraj;
CGrenTraj * g_pNadeTraj = &g_nadeTraj;
COffsets g_Offsets;
CConfig * Config = new CConfig;
Variables Vars;
ServerRankRevealAllFn ServerRankRevealAllEx = NULL;
FireEventClientSide fnOriginalFireEventClientSide = NULL;
bool g_bOffline = true;
CLBOT * g_pLegitBOT = new CLBOT;
CVisuals * g_pVisuals = new CVisuals;
SIZE_T g_uTraces = 0;
Vector G::vecSelfEyePos;
Vector G::vecSelfVel;
FLOAT G::fSelfVelRealLen = 0.f;
QAngle g_angSelfView;
QAngle G::angSelfPunch;
bool G::bAlive = false;
Entity_T * G::pSelf = NULL;
CUserCmd * G::pCmd = NULL;
FLOAT g_fMaxPunch = 0.f;
INT G::iTeam = 0;
INT G::iFlags = 0;
bool G::Keys[XS_RANDOM_SIZE * 2] = { false };
bool G::bScoped = false;
bool g_bD3DOK = false;
HWND g_pWnd = NULL;
INT G::iWeapId = 0;
bool G::bGun = false;
clock_t g_zoomTime = NULL;
clock_t g_switchTime = NULL;
INT g_iLastItem = NULL;
bool g_bWasScoped = false;
bool G::bEmpty = false;
Entity_T * G::pWeap = NULL;
VTHook * H::VPanel = NULL;
VTHook * H::CLMode = NULL;
VTHook * H::CL = NULL;
VTHook * H::D3D = NULL;
IGameEventManager * I::EvntsMgr = NULL;
IBaseClientDll * I::Client = NULL;
IClientModeShared * I::CLMode = NULL;
IEnts * I::Ents = NULL;
IEngineClient * I::Engine = NULL;
IEngineTrace * I::EngTrace = NULL;
IGlobalVarsBase * I::Globals = NULL;
ISurface * I::Surf = NULL;
IVPanel * I::VPanel = NULL;
IVModelInfo * I::MDLInfo = NULL;
IInputSystem * I::InputSys = NULL;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

IPhysProps * I::PhysProps = NULL;
VTHook * H::Traces = NULL;

#endif

#endif

bool __fastcall FireEventClientSideThink(PVOID ECX, PVOID, IGameEvent * pEvent)
{
	static CONST CHAR * pszName = NULL, *pszWeapon = NULL;
	static CHAR szData[XS_RANDOM_SIZE] = {}, szWeapon[XS_RANDOM_SIZE] = {};
	static time_t tNow = 0, tStamp = 0;
	static INT iVictim = 0, iAttacker = 0, iPlayer = 0, iTeam = 0, iTerroristsAlive = 0, iWinner = 0;
	static bool bFlip = false, bHS = false;
	static Vector vecOrigin;
	static Entity_T * pPlayer = NULL;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	static PlrInfo_T PlrInfo, SelfInfo;

#endif

	static string Weapon = {}, Name = {};

	tNow = time(NULL);

	if (!(G::pSelf = GetSelf()) || !pEvent || !(pszName = pEvent->GetName()) || *pszName == NULL)
		return fnOriginalFireEventClientSide(ECX, pEvent);

	else if (stricmp(pszName, XorString("CS_Game_Disconnected")) == NULL)
	{
		sendConCmd(XorString("bind tab +showscores; bind y messagemode")), \
			sendConCmd(XorString("bind u messagemode2; cl_mouseenable 1"));

		g_bFrozen = G::bAlive = G::bGun = g_bBombPlanted = false, g_bOffline = G::bEmpty = g_bRoundEnded = true, G::iTeam = G::iFlags = NULL, g_pNadeTraj->Clear(), g_iCTScore = g_iTEScore = 0;
		return fnOriginalFireEventClientSide(ECX, pEvent);
	}

	else if (stricmp(pszName, XorString("Player_Connect_Full")) == NULL || stricmp(pszName, XorString("Player_Full_Connect")) == NULL || \
		stricmp(pszName, XorString("Player_Connected_Full")) == NULL || stricmp(pszName, XorString("Player_Full_Connected")) == NULL)
	{
		sendConCmd(XorString("bind tab +showscores; bind y messagemode")), \
			sendConCmd(XorString("bind u messagemode2"));

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

		g_bOffline = g_bFrozen = g_bBombPlanted = false, g_bRoundEnded = true, g_pNadeTraj->Clear(), g_iCTScore = g_iTEScore = g_PhrasesSize = 0;

#else

		g_bOffline = g_bFrozen = g_bBombPlanted = false, g_bRoundEnded = true, g_pNadeTraj->Clear(), g_iCTScore = g_iTEScore = 0;

#endif

		return fnOriginalFireEventClientSide(ECX, pEvent);
	}

	else if (stricmp(pszName, XorString("Bomb_Defuse")) == 0 || stricmp(pszName, XorString("Bomb_Explode")) == 0 || \
		stricmp(pszName, XorString("Bomb_Defused")) == 0 || stricmp(pszName, XorString("Bomb_Exploded")) == 0 || \
		stricmp(pszName, XorString("Round_End")) == 0 || stricmp(pszName, XorString("Round_End_Post")) == 0)
	{
		g_bRoundEnded = true;

		if (stricmp(pszName, XorString("Round_End")) == 0)
		{
			iWinner = pEvent->GetInt(XorString("winner"));

			if (iWinner > 1 && iWinner < 4)
			{
				if (iWinner == 2) g_iTEScore++;
				else g_iCTScore++;
			};
		};

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

		if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk)
		{
			if (stricmp(pszName, XorString("Round_End")) == 0)
			{
				if (RandomINT(0, 1) == 0 && iWinner > 1 && iWinner < 4)
				{
					if (iWinner == G::iTeam)
					{
						switch (RandomINT(0, 9))
						{
						case 0: xprintf(szData, XorString("nS+")); break;
						case 1: xprintf(szData, XorString("very good round")); break;
						case 2: xprintf(szData, XorString("splendid round+")); break;
						case 3: xprintf(szData, XorString("good round+")); break;
						case 4: xprintf(szData, XorString("nice Round man")); break;
						case 5: xprintf(szData, XorString("amazing play+")); break;
						case 6: xprintf(szData, XorString("stronk team ❤")); break;
						case 7: xprintf(szData, XorString("nice teamplay :*")); break;
						case 8: xprintf(szData, XorString("hard play man+")); break;
						case 9: xprintf(szData, XorString("high skill team ❤")); break;
						};
					}

					else
					{
						switch (RandomINT(0, 9))
						{
						case 0: xprintf(szData, XorString("i fuck all ur family")); break;
						case 1: xprintf(szData, XorString("u all 5 hack man?")); break;
						case 2: xprintf(szData, XorString("full hack pre-mates?")); break;
						case 3: xprintf(szData, XorString("why hack?")); break;
						case 4: xprintf(szData, XorString("go away guys..")); break;
						case 5: xprintf(szData, XorString("mby turn off pls?")); break;
						case 6: xprintf(szData, XorString("Reported all 5")); break;
						case 7: xprintf(szData, XorString("reportbotted all :*")); break;
						case 8: xprintf(szData, XorString("wait VAC guys!")); break;
						case 9: xprintf(szData, XorString("u will get OW np :@")); break;
						};
					};

					sprintf(g_Phrases[g_PhrasesSize++], szData);
					g_timeLastEventStamp = tNow;
				};
			};
		};

#endif

		g_bBombPlanted = false;
	}

	else if (stricmp(pszName, XorString("Bomb_Plant")) == 0 || stricmp(pszName, XorString("Bomb_Planted")) == 0)
		g_bBombPlanted = true;

	else if (stricmp(pszName, XorString("Round_Start")) == NULL || stricmp(pszName, XorString("Round_Pre_Start")) == NULL || \
		stricmp(pszName, XorString("Round_Start_Pre")) == NULL || stricmp(pszName, XorString("Pre_Round_Start")) == NULL || \
		stricmp(pszName, XorString("Player_Spawn")) == NULL || stricmp(pszName, XorString("Player_Spawned")) == NULL)
	{
		g_pNadeTraj->Clear();

		if (stricmp(pszName, XorString("Round_Start")) == NULL || stricmp(pszName, XorString("Round_Pre_Start")) == NULL || \
			stricmp(pszName, XorString("Round_Start_Pre")) == NULL || stricmp(pszName, XorString("Pre_Round_Start")) == NULL)
		{

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

			if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk)
			{
				if ((stricmp(pszName, XorString("Round_Start")) == 0 || stricmp(pszName, XorString("Round_Start_Pre")) == 0 || \
					stricmp(pszName, XorString("Pre_Round_Start")) == 0) && RandomINT(0, 1) == 0)
				{
					if (g_iCTScore > g_iTEScore && G::iTeam == 3)
					{
						switch (RandomINT(0, 16))
						{
						case 0: xprintf(szData, XorString("we still win+")); break;
						case 1: xprintf(szData, XorString("you will lose this game I promise+")); break;
						case 2: xprintf(szData, XorString("splendid game+")); break;
						case 3: xprintf(szData, XorString("so good team, definitely win")); break;
						case 4: xprintf(szData, XorString("no disguise we winning")); break;
						case 5: xprintf(szData, XorString("EZ game?")); break;
						case 6: xprintf(szData, XorString("we will win ❤")); break;
						case 7: xprintf(szData, XorString("so fkn EZ")); break;
						case 8: xprintf(szData, XorString("dont be mad guys, its only a game")); break;
						case 9: xprintf(szData, XorString("dont worry man we will win ❤")); break;
						case 10: xprintf(szData, XorString("drop me AWP ❤")); break;
						case 11: xprintf(szData, XorString("go eco 1")); break;
						case 12: xprintf(szData, XorString("man go eco 1 to hav money")); break;
						case 13: xprintf(szData, XorString("now i dont have money")); break;
						case 14: xprintf(szData, XorString("if we dont eco we cant win")); break;
						case 15: xprintf(szData, XorString("EZ+")); break;
						case 16: xprintf(szData, XorString("gg, we win it")); break;
						};

						g_timeLastEventStamp = tNow;
						sprintf(g_Phrases[g_PhrasesSize++], szData);
					}

					else if (g_iTEScore > g_iCTScore && G::iTeam == 2)
					{
						switch (RandomINT(0, 19))
						{
						case 0: xprintf(szData, XorString("i reported all 5")); break;
						case 1: xprintf(szData, XorString("please report first 2 for WH")); break;
						case 2: xprintf(szData, XorString("first 2 cheat for sure+")); break;
						case 3: xprintf(szData, XorString("kick your cheater pls")); break;
						case 4: xprintf(szData, XorString("u boosted cunts")); break;
						case 5: xprintf(szData, XorString("nice man, boosted by hacker+")); break;
						case 6: xprintf(szData, XorString("why we lose ?")); break;
						case 7: xprintf(szData, XorString("are we losing?")); break;
						case 8: xprintf(szData, XorString("guys, let's go win mby?")); break;
						case 9: xprintf(szData, XorString("we win or we lose?")); break;
						case 10: xprintf(szData, XorString("do we lose?")); break;
						case 11: xprintf(szData, XorString("guys, lets play?")); break;
						case 12: xprintf(szData, XorString("guys, pls focus on fkn game")); break;
						case 13: xprintf(szData, XorString("i dont have money now")); break;
						case 14: xprintf(szData, XorString("eco 1")); break;
						case 15: xprintf(szData, XorString("now i dont have money")); break;
						case 16: xprintf(szData, XorString("if we dont eco we cant win")); break;
						case 17: xprintf(szData, XorString("man go eco 1 to hav money")); break;
						case 18: xprintf(szData, XorString("man if u have $$ go drop to ur team")); break;
						case 19: xprintf(szData, XorString("man if u wanna win, go drop to ur team")); break;
						};

						g_timeLastEventStamp = tNow;
						sprintf(g_Phrases[g_PhrasesSize++], szData);
					};
				};
			};

#endif

			g_roundStartTime = tNow, g_bBombPlanted = g_bRoundEnded = false;
		};

		if (tNow - tStamp > 0)
		{
			if (bFlip)
			{
				if (!Vars.Misc.bDoNotEditRadarScale)
				{
					sendConCmd(XorString("cl_radar_scale .4"));
					sendConCmd(XorString("cl_radar_always_centered 0"));
				};

				if (!Vars.Misc.bDoNotEnforceTopSettings)
					sendConCmd(XorString("cl_interp_ratio 0; cl_interp 0"));
			}

			else
			{
				if (!Vars.Misc.bDoNotEnforceTopSettings)
				{
					{
						sendConCmd(XorString("rate 786432; cl_cmdrate 128")), \
							sendConCmd(XorString("mm_dedicated_search_maxping 350"));
					};

					{
						sendConCmd(XorString("fps_max 8192; fps_max_menu 8192")), \
							sendConCmd(XorString("cl_updaterate 128"));
					};
				};
			};

			tStamp = tNow, bFlip = !bFlip;
		};
	}

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	else if (Vars.Misc.bSpam && NULL == stricmp(pszName, XorString("Player_Death")))
	{
		iVictim = pEvent->GetInt(XorString("userid")), iAttacker = pEvent->GetInt(XorString("attacker")), \
			pszWeapon = pEvent->GetString(XorString("weapon")), bHS = pEvent->GetInt(XorString("headshot")) ? true : false;

		g_bCheckForLastEnemy = TRUE;

		if (iAttacker > 0 && iVictim > 0 && iAttacker != iVictim && I::Engine->GetPlayerInfo(I::Engine->GetSelf(), &SelfInfo))
		{
			Weapon = pszWeapon, replaceAll(Weapon, XorString("weapon_"), XorString("")), strcpy(szWeapon, Weapon.c_str()), pszWeapon = &szWeapon[0];

			if (iAttacker == SelfInfo.userid && Vars.Misc.bSpamInsultVictim)
			{
				for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
				{
					pPlayer = I::Ents->GetClEntity(iPlayer);

					if (!pPlayer || pPlayer == G::pSelf || (iTeam = pPlayer->GetTeam()) < 2 || iTeam > 3 || (I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo) && PlrInfo.userid != iVictim))
						continue;

					g_timeLastEventStamp = tNow;

					if (bHS && RandomINT(0, 8) == 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("n1 HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("dat HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("%s, ns HS"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("nize HS %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("fkn HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'm' && pszWeapon[1] == '4' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 3))
						{
						case 0: xprintf(szData, XorString("n1 m4 kill, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("m4 skills, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("M4 expert, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns M4, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'a' && pszWeapon[1] == 'k' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 3))
						{
						case 0: xprintf(szData, XorString("n1 AK kill, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("ak47 skills, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("Ak47 expert, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns Ak47, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'a' && pszWeapon[1] == 'w' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("ofc AWP, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("ns AWP %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("u go AWP, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("n1 AWP trigger, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("1 hit 1 kill, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 't' && pszWeapon[1] == 'a')
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("got rekt, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("REKT, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("enjoyed dat x27, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns x27 %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("WTF %s tased ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'k' && pszWeapon[1] == 'n')
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("got rekt, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("REKT, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("enjoyed dat Kniv, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns Knif %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("WTF %s sliced ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					PrepareSpamString(szData, &PlrInfo);
					sprintf(g_Phrases[g_PhrasesSize++], szData);

					break;
				};
			}

			else if (iVictim == SelfInfo.userid && Vars.Misc.bSpamInsultEnemy)
			{
				for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
				{
					pPlayer = I::Ents->GetClEntity(iPlayer);

					if (!pPlayer || pPlayer == G::pSelf || (iTeam = pPlayer->GetTeam()) < 2 || iTeam > 3 || (I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo) && PlrInfo.userid != iAttacker))
						continue;

					g_timeLastEventStamp = tNow;

					if (pPlayer->GetHealth() <= 25 && RandomINT(0, 1) == 0 && pPlayer->GetHealth() > 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("%s%d HP"), XorString("-"), 100 - pPlayer->GetHealth()); break;
						case 1: xprintf(szData, XorString("%s%d"), XorString("-"), 100 - pPlayer->GetHealth()); break;
						case 2: xprintf(szData, XorString("%s is %d HP"), ShortenName(string(PlrInfo.name)).c_str(), pPlayer->GetHealth()); break;
						case 3: xprintf(szData, XorString("%s is low (%d)"), ShortenName(string(PlrInfo.name)).c_str(), pPlayer->GetHealth()); break;
						case 4: xprintf(szData, XorString("fuck u %s, %s%d"), ShortenName(string(PlrInfo.name)).c_str(), XorString("-"), 100 - pPlayer->GetHealth()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (bHS && RandomINT(0, 8) == 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("n1 HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("dat HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("%s, ns HS"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("nize HS %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("fkn HS, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'm' && pszWeapon[1] == '4' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("n1 m4 kill, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("m4 skills, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("who teached u M4, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("M4 expert, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("ns M4, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'a' && pszWeapon[1] == 'k' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("n1 AK kill, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("ak47 skills, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("who teached u AK47, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("Ak47 expert, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("ns Ak47, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'a' && pszWeapon[1] == 'w' && RandomINT(0, 5) == 0)
					{
						switch (RandomINT(0, 5))
						{
						case 0: xprintf(szData, XorString("ofc AWP, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("how $$ for AWP, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("ns AWP %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("u go AWP, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("n1 AWP trigger, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 5: xprintf(szData, XorString("1 hit 1 kill, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 't' && pszWeapon[1] == 'a')
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("i got rekt, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("REKT, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("i enjoyed dat x27, %s !"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns x27 %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("WTF %s tased me ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					if (pszWeapon[0] == 'k' && pszWeapon[1] == 'n')
					{
						switch (RandomINT(0, 4))
						{
						case 0: xprintf(szData, XorString("i got rekt, %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 1: xprintf(szData, XorString("REKT, %s ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 2: xprintf(szData, XorString("enjoyed dat Kniv, %s !"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 3: xprintf(szData, XorString("ns Knif %s"), ShortenName(string(PlrInfo.name)).c_str()); break;
						case 4: xprintf(szData, XorString("WTF %s sliced me ?"), ShortenName(string(PlrInfo.name)).c_str()); break;
						};

						sprintf(g_Phrases[g_PhrasesSize++], szData);
						break;
					};

					PrepareSpamString(szData, &PlrInfo);
					sprintf(g_Phrases[g_PhrasesSize++], szData);

					break;
				};
			};
		};
	};

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && RandomINT(0, 1) == 0 && (NULL == stricmp(pszName, XorString("Bomb_Plant")) || NULL == stricmp(pszName, XorString("Bomb_Planted"))))
	{
		switch (RandomINT(0, 5))
		{
		case 0: xprintf(szData, XorString("u planted bomb already man?")); break;
		case 1: xprintf(szData, XorString("why u plant+?")); break;
		case 2: xprintf(szData, XorString("whos gonna defuse now?")); break;
		case 3: xprintf(szData, XorString("man go defus dat bomb ?")); break;
		case 4: xprintf(szData, XorString("n1 plant+")); break;
		case 5: xprintf(szData, XorString("finally planted..")); break;
		};

		sprintf(g_Phrases[g_PhrasesSize++], szData);
		g_timeLastEventStamp = tNow;
	}

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && RandomINT(0, 2) == 0 && (NULL == stricmp(pszName, XorString("Bomb_Drop")) || NULL == stricmp(pszName, XorString("Bomb_Dropped"))))
	{
		if (tNow - g_bombDropTime >= 16 && tNow - g_roundStartTime >= 24)
		{
			switch (RandomINT(0, 5))
			{
			case 0: xprintf(szData, XorString("man why u drop dat bomb?")); break;
			case 1: xprintf(szData, XorString("stop dropping the bomb, LOL")); break;
			case 2: xprintf(szData, XorString("man dont drop the bomb go plant it")); break;
			case 3: xprintf(szData, XorString("stop dropping that bomb")); break;
			case 4: xprintf(szData, XorString("go plant the bomb dont drop it")); break;
			case 5: xprintf(szData, XorString("why drop bomb?")); break;
			};

			sprintf(g_Phrases[g_PhrasesSize++], szData);
			g_timeLastEventStamp = tNow, g_bombDropTime = tNow;
		};
	}

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && G::iTeam > 0 && G::iTeam < 4 && (NULL == stricmp(pszName, XorString("Player_Disconnect"))))
	{
		iVictim = pEvent->GetInt(XorString("userid"));

		if (iVictim >= 0)
		{
			for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
			{
				pPlayer = I::Ents->GetClEntity(iPlayer);

				if (pPlayer && pPlayer != G::pSelf && I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo) && iVictim == PlrInfo.userid)
				{
					switch (RandomINT(0, 6))
					{
					case 0: xprintf(szData, XorString("%s, n1 rage quit"), ShortenName(PlrInfo.name).c_str()); break;
					case 1: xprintf(szData, XorString("LOL, %s rage quitted"), ShortenName(PlrInfo.name).c_str()); break;
					case 2: xprintf(szData, XorString("why %s rage quitted?"), ShortenName(PlrInfo.name).c_str()); break;
					case 3: xprintf(szData, XorString("%s rage quit?"), ShortenName(PlrInfo.name).c_str()); break;
					case 4: xprintf(szData, XorString("nice, %s rage quitted"), ShortenName(PlrInfo.name).c_str()); break;
					case 5: xprintf(szData, XorString("%s surrendered?"), ShortenName(PlrInfo.name).c_str()); break;
					case 6: xprintf(szData, XorString("%s crashed?"), ShortenName(PlrInfo.name).c_str()); break;
					};

					sprintf(g_Phrases[g_PhrasesSize++], szData);
					g_timeLastEventStamp = tNow;

					break;
				};
			};
		};
	};

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && G::iTeam > 0 && G::iTeam < 4 && (NULL == stricmp(pszName, XorString("Player_Connect"))))
	{
		iVictim = pEvent->GetInt(XorString("userid"));

		if (iVictim >= 0)
		{
			for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
			{
				pPlayer = I::Ents->GetClEntity(iPlayer);

				if (pPlayer && pPlayer != G::pSelf && I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo) && iVictim == PlrInfo.userid)
				{
					switch (RandomINT(0, 5))
					{
					case 0: xprintf(szData, XorString("hi, %s"), ShortenName(PlrInfo.name).c_str()); break;
					case 1: xprintf(szData, XorString("welcome back, %s"), ShortenName(PlrInfo.name).c_str()); break;
					case 2: xprintf(szData, XorString("WB, %s"), ShortenName(PlrInfo.name).c_str()); break;
					case 3: xprintf(szData, XorString("hello, %s"), ShortenName(PlrInfo.name).c_str()); break;
					case 4: xprintf(szData, XorString("hey %s"), ShortenName(PlrInfo.name).c_str()); break;
					case 5: xprintf(szData, XorString("ПРИВЕТ, %s"), ShortenName(PlrInfo.name).c_str()); break;
					};

					sprintf(g_Phrases[g_PhrasesSize++], szData);
					g_timeLastEventStamp = tNow;

					break;
				};
			};
		};
	};

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && RandomINT(0, 1) == 0 && (NULL == stricmp(pszName, XorString("Bomb_Defuse")) || NULL == stricmp(pszName, XorString("Bomb_Defused"))))
	{
		g_bRoundEnded = true;

		switch (RandomINT(0, 7))
		{
		case 0: xprintf(szData, XorString("n1 defuse+")); break;
		case 1: xprintf(szData, XorString("splendid defuse man i ❤ it")); break;
		case 2: xprintf(szData, XorString("this defuse will go on YT™")); break;
		case 3: xprintf(szData, XorString("dat1 was Ninja?")); break;
		case 4: xprintf(szData, XorString("ninja defused+?")); break;
		case 5: xprintf(szData, XorString("so good defuse")); break;
		case 6: xprintf(szData, XorString("so good defuse+, never seen befoar")); break;
		case 7: xprintf(szData, XorString("waiting applauses for defuse+")); break;
		};

		sprintf(g_Phrases[g_PhrasesSize++], szData);
		g_timeLastEventStamp = tNow;
	};

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && (NULL == stricmp(pszName, XorString("CS_Win_Panel_Match")) || NULL == stricmp(pszName, XorString("Announce_Phase_End")) || NULL == stricmp(pszName, XorString("CS_Intermission")) || NULL == stricmp(pszName, XorString("Start_HalfTime"))))
	{
		g_bRoundEnded = true;

		switch (RandomINT(0, 7))
		{
		case 0: xprintf(szData, XorString("n1 game++")); break;
		case 1: xprintf(szData, XorString("i ❤ it, GG")); break;
		case 2: xprintf(szData, XorString("dis game will go on YT™")); break;
		case 3: xprintf(szData, XorString("you will all get OW np")); break;
		case 4: xprintf(szData, XorString("u all get VAC dont worry")); break;
		case 5: xprintf(szData, XorString("hadi kazı mezarını")); break;
		case 6: xprintf(szData, XorString("check ur acc in 2 hr :*")); break;
		case 7: xprintf(szData, XorString("reportbotted first 2, paid bot, just wait guys")); break;
		};

		sprintf(g_Phrases[g_PhrasesSize++], szData);
		g_timeLastEventStamp = tNow;
	}

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && tNow - g_lastDoPlantMsg >= 150 && !g_bBombPlanted && !g_bRoundEnded && tNow - g_roundStartTime >= 80 && RandomINT(0, 2) == 0)
	{
		iTerroristsAlive = 0;

		for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
		{
			pPlayer = I::Ents->GetClEntity(iPlayer);

			if (pPlayer && pPlayer != G::pSelf && pPlayer->GetHealth() > 0 && pPlayer->GetTeam() == 2)
				iTerroristsAlive++;

			if (iTerroristsAlive > 0)
				break;
		};

		if (iTerroristsAlive > 0)
		{
			switch (RandomINT(0, 6))
			{
			case 0: xprintf(szData, XorString("mby go plant that bomb?")); break;
			case 1: xprintf(szData, XorString("no bomb plant yet?")); break;
			case 2: xprintf(szData, XorString("man stop camping, go plant")); break;
			case 3: xprintf(szData, XorString("when u plant?+")); break;
			case 4: xprintf(szData, XorString("i wait for plant+")); break;
			case 5: xprintf(szData, XorString("man time? plant pls")); break;
			case 6: xprintf(szData, XorString("can u plant pls?")); break;
			};

			sprintf(g_Phrases[g_PhrasesSize++], szData);
			g_timeLastEventStamp = tNow, g_lastDoPlantMsg = tNow;
		};
	};

	if (Vars.Misc.bSpam && Vars.Misc.bSpamRandomTalk && NULL == stricmp(pszName, XorString("HEGrenade_Detonate")))
	{
		vecOrigin = Vector(pEvent->GetInt(XorString("x")), pEvent->GetInt(XorString("y")), pEvent->GetInt(XorString("z")));

		for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
		{
			if ((pPlayer = I::Ents->GetClEntity(iPlayer)) && pPlayer != G::pSelf && (iTeam = pPlayer->GetTeam()) > 1 && iTeam < 3 && iTeam != G::iTeam && pPlayer->GetHealth() > 0 && \
				(vecOrigin - pPlayer->GetOrigin()).Length() < 320.f && I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo))
			{
				Name = ShortenName(PlrInfo.name);

				if (RandomINT(0, 1) == 0 && pPlayer->GetHealth() <= 25)
				{
					switch (RandomINT(0, 2))
					{
					case 0: xprintf(szData, XorString("%s is low, %d HP"), Name.c_str(), pPlayer->GetHealth()); break;
					case 1: xprintf(szData, XorString("%s is %d HP"), Name.c_str(), pPlayer->GetHealth()); break;
					case 2: xprintf(szData, XorString("%s, -%d HP"), Name.c_str(), 100 - pPlayer->GetHealth()); break;
					};
				}

				else
				{
					switch (RandomINT(0, 6))
					{
					case 0: xprintf(szData, XorString("u enjoyed dat, %s ?"), Name.c_str()); break;
					case 1: xprintf(szData, XorString("u ate it, %s ?"), Name.c_str()); break;
					case 2: xprintf(szData, XorString("u got destructed, %s ?"), Name.c_str()); break;
					case 3: xprintf(szData, XorString("%s, enjoyed dat grenade?"), Name.c_str()); break;
					case 4: xprintf(szData, XorString("u liked it, %s ?"), Name.c_str()); break;
					case 5: xprintf(szData, XorString("u liked the grenade, %s ?"), Name.c_str()); break;
					case 6: xprintf(szData, XorString("good grenade, %s ?"), Name.c_str()); break;
					};
				};

				sprintf(g_Phrases[g_PhrasesSize++], szData);
				g_timeLastEventStamp = tNow;
			};
		};
	};

#endif

	return fnOriginalFireEventClientSide(ECX, pEvent);
};

VOID SetupInterfaces(VOID)
{
	PDWORD pOriginalGameEventMgrVMT = NULL, pNewGameEventMgrVMT = NULL, *pGameEventMgrVMT = NULL;
	SIZE_T dwGameEventMgrVMTSize = NULL;

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pClient = g_Hattrick.ProcessModule(XorString("client.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pClient) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pEngine = g_Hattrick.ProcessModule(XorString("engine.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pEngine) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pVGUI2 = g_Hattrick.ProcessModule(XorString("vgui2.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pVGUI2) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pInputSystem = g_Hattrick.ProcessModule(XorString("inputsystem.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pInputSystem) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pVGUIMatSurface = g_Hattrick.ProcessModule(XorString("vguimatsurface.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pVGUIMatSurface) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	while (true)
	{
		CHAR Buffer_A[XS_RANDOM_SIZE] = {};
		Hattrick::ModuleTuple * pVPhysics = g_Hattrick.ProcessModule(XorString("vphysics.dll"));
		Buffer_A[0] += __TIME__[4];
		if (pVPhysics) break;
		Sleep(XS_RANDOM_SIZE / 16);
	};

	CHAR Buffer_A[XS_RANDOM_SIZE] = {};
	I::EvntsMgr = CaptureInterface < IGameEventManager >(XorString("engine.dll"), XorString("GAMEEVENTSMANAGER002")), Buffer_A[0] += __TIME__[4];

	CHAR Buffer_B[XS_RANDOM_SIZE] = {};
	I::Client = CaptureInterface < IBaseClientDll >(XorString("client.dll"), XorString("VClient018")), Buffer_B[0] += __TIME__[4];

	CHAR Buffer_C[XS_RANDOM_SIZE] = {};
	I::CLMode = **(IClientModeShared ***)((*(PDWORD *)I::Client)[10] + 0x00000005), Buffer_C[0] += __TIME__[4];

	CHAR Buffer_D[XS_RANDOM_SIZE] = {};
	I::Ents = CaptureInterface < IEnts >(XorString("client.dll"), XorString("VClientEntityList003")), Buffer_D[0] += __TIME__[4];

	CHAR Buffer_E[XS_RANDOM_SIZE] = {};
	I::Engine = CaptureInterface < IEngineClient >(XorString("engine.dll"), XorString("VEngineClient014")), Buffer_E[0] += __TIME__[4];

	CHAR Buffer_F[XS_RANDOM_SIZE] = {};
	I::EngTrace = CaptureInterface < IEngineTrace >(XorString("engine.dll"), XorString("EngineTraceClient004")), Buffer_F[0] += __TIME__[4];

	CHAR Buffer_G[XS_RANDOM_SIZE] = {};
	I::Globals = ** (IGlobalVarsBase ***)((*(PDWORD *)I::Client)[0] + 0x0000001B), Buffer_G[0] += __TIME__[4];

	CHAR Buffer_H[XS_RANDOM_SIZE] = {};
	I::Surf = CaptureInterface < ISurface >(XorString("vguimatsurface.dll"), XorString("VGUI_Surface031")), Buffer_H[0] += __TIME__[4];

	CHAR Buffer_I[XS_RANDOM_SIZE] = {};
	I::VPanel = CaptureInterface < IVPanel >(XorString("vgui2.dll"), XorString("VGUI_Panel009")), Buffer_I[0] += __TIME__[4];

	CHAR Buffer_J[XS_RANDOM_SIZE] = {};
	I::MDLInfo = CaptureInterface < IVModelInfo >(XorString("engine.dll"), XorString("VModelInfoClient004")), Buffer_J[0] += __TIME__[4];

	CHAR Buffer_K[XS_RANDOM_SIZE] = {};
	I::InputSys = CaptureInterface < IInputSystem >(XorString("inputsystem.dll"), XorString("InputSystemVersion001")), Buffer_K[0] += __TIME__[4];

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	CHAR Buffer_L[XS_RANDOM_SIZE] = {};
	I::PhysProps = CaptureInterface < IPhysProps >(XorString("vphysics.dll"), XorString("VPhysicsSurfaceProps001")), Buffer_L[0] += __TIME__[4];

#endif

#endif

	CHAR Buffer_M[XS_RANDOM_SIZE] = {};
	pGameEventMgrVMT = (PDWORD *)I::EvntsMgr, pOriginalGameEventMgrVMT = *pGameEventMgrVMT;
	while ((PDWORD)(*pGameEventMgrVMT)[dwGameEventMgrVMTSize]) dwGameEventMgrVMTSize++;
	pNewGameEventMgrVMT = new DWORD[dwGameEventMgrVMTSize], \
		CopyMemory(pNewGameEventMgrVMT, pOriginalGameEventMgrVMT, sizeof DWORD * dwGameEventMgrVMTSize), \
		pNewGameEventMgrVMT[9] = (DWORD)FireEventClientSideThink, \
		fnOriginalFireEventClientSide = (FireEventClientSide)pOriginalGameEventMgrVMT[9], \
		*pGameEventMgrVMT = pNewGameEventMgrVMT, Buffer_M[ARRAYSIZE(Buffer_A) / 2] += __TIME__[4] + ARRAYSIZE(Buffer_B) / 2;
};

bool CanAim(VOID)
{
	return (fabsf(G::angSelfPunch.x) > g_fMaxPunch * Vars.LEGITBOT.AimHelpers.fAimLimit || fabsf(G::angSelfPunch.y) > g_fMaxPunch * Vars.LEGITBOT.AimHelpers.fAimLimit) ? false : true;
};

Entity_T * GetSelf(VOID)
{
	static Entity_T * pEntity = NULL;

	if (!I::Engine->IsInGame() || !I::Engine->IsConnected() || !(pEntity = I::Ents->GetClEntity(I::Engine->GetSelf())))
	{
		G::angSelfPunch = QAngle(0.f, 0.f, 0.f), g_pNadeTraj->Clear(), g_bOffline = G::bEmpty = true, G::bAlive = G::bGun = false, \
			G::iTeam = G::iFlags = G::iWeapId = NULL, G::pWeap = NULL, g_iCTScore = g_iTEScore = 0;

		return NULL;
	};

	g_bOffline = false, G::bAlive = pEntity->GetHealth() > 0 ? true : false, G::iTeam = pEntity->GetTeam(), G::iFlags = pEntity->GetFlags(), G::vecSelfEyePos = pEntity->GetEyePos();

	if (G::bAlive)
	{
		if ((G::pWeap = pEntity->GetWeapon()))
			G::iWeapId = G::pWeap->GetItemDefinitionIndex(), G::bEmpty = G::pWeap->IsEmpty(), G::bGun = G::pWeap->IsGun(G::iWeapId), G::bScoped = pEntity->IsScoped();

		else
			G::iWeapId = NULL, G::bEmpty = true, G::bGun = G::bScoped = false;

		G::angSelfPunch = pEntity->GetPunch(), G::vecSelfVel = pEntity->GetVelocity(), G::fSelfVelRealLen = G::fSelfVelLen = G::vecSelfVel.Length();

		if (G::angSelfPunch.x || G::angSelfPunch.y)
		{
			if (G::iWeapId != WEAPON_SCAR20 && G::iWeapId != WEAPON_G3SG1)
				G::fSelfVelLen = 8192.f;

			if (fabsf(G::angSelfPunch.x) > g_fMaxPunch)
				g_fMaxPunch = fabsf(G::angSelfPunch.x);

			if (fabsf(G::angSelfPunch.y) > g_fMaxPunch)
				g_fMaxPunch = fabsf(G::angSelfPunch.y);
		};
	}

	else
		G::bScoped = G::bGun = false, G::bEmpty = true, G::iWeapId = NULL, G::pWeap = NULL;

	return pEntity;
};

VOID TraceLine(CONST Vector vecStart, CONST Vector vecEnd, CONST UINT uiMask, Entity_T * pIgnore, GameTrace_T * pTrace)
{
	static Ray_T _Ray;
	static TraceFilter_T _Filter;

	_Ray.Run(vecStart, vecEnd), _Filter.pSkip = pIgnore, I::EngTrace->TraceRay(_Ray, uiMask, &_Filter, pTrace);
};

VOID ServerRankRevealAll(VOID)
{
	static FLOAT pArray[3] = { 0.f, 0.f, 0.f, };

	if (g_Offsets.m_dwServerRankRevealAllEx)
	{
		ServerRankRevealAllEx = (ServerRankRevealAllFn)(g_Offsets.m_dwServerRankRevealAllEx);

		if (ServerRankRevealAllEx)
			ServerRankRevealAllEx(pArray);
	};
};

VOID SetupHooks(VOID)
{
	H::VPanel = new VTHook((PDWORD *)I::VPanel);
	H::CLMode = new VTHook((PDWORD *)I::CLMode);

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	H::Surf = new VTHook((PDWORD *)I::Surf);

#endif

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	H::Traces = new VTHook((PDWORD *)I::EngTrace);

#endif

#endif

	H::CL = new VTHook((PDWORD *)I::Client);
	H::D3D = new VTHook((PDWORD *)g_Offsets.m_dwD3DDevice);

	oPaintTraverse = (PaintTraverseFn)H::VPanel->HookFunction((DWORD)PaintTraverse, 41);
	oCreateMove = (CreateMoveFn)H::CLMode->HookFunction((DWORD)CreateMove, 24);
	oReset = (ResetFn)H::D3D->HookFunction((DWORD)Reset, 16);
	oEndScene = (EndSceneFn)H::D3D->HookFunction((DWORD)EndScene, 42);

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	oTraceRay = (TraceRayFn)H::Traces->HookFunction((DWORD)OnTraceRay, 5);

#endif

#endif

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	oPlaySound = (PlaySoundFn)H::Surf->HookFunction((DWORD)OnPlaySound, 82);

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

	oOverrideView = (OverrideViewFn)H::CLMode->HookFunction((DWORD)OnOverrideView, 18);
	oGetViewModelFoV = (GetViewModelFoVFn)H::CLMode->HookFunction((DWORD)OnGetViewModelFoV, 35);

#endif

};

VOID SetupOffsets(VOID)
{
	g_pNetVarsMan->Run(), GrabOffsets();
};

FLOAT GetDisp(VOID)
{
	if ((G::pCmd->buttons & IN_ATTACK) && (G::pCmd->buttons & IN_ATTACK2)) return -6.f;
	else if ((G::pCmd->buttons & IN_ATTACK)) return 0.f;
	return -12.f;
};

FLOAT GetFactor(VOID)
{
	if ((G::pCmd->buttons & IN_ATTACK) && (G::pCmd->buttons & IN_ATTACK2)) return .6f;
	else if ((G::pCmd->buttons & IN_ATTACK)) return .9f;
	return .27f;
};

FLOAT GetDetonationTime(VOID)
{
	return (G::iWeapId == WEAPON_HEGRENADE || G::iWeapId == WEAPON_FLASHBANG) ? 1.5f : 3.f;
};

VOID GrabOffsets(VOID)
{
	g_Offsets.m_bDormant = 0x000000E9;

	g_Offsets.m_iObserverMode = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_iObserverMode"));
	g_Offsets.m_hObserverTarget = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_hObserverTarget"));
	g_Offsets.m_iHealth = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_iHealth"));
	g_Offsets.m_bSpotted = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_bSpotted"));
	g_Offsets.m_iTeamNum = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_iTeamNum"));
	g_Offsets.m_bIsDefusing = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_bIsDefusing"));
	g_Offsets.m_bHasDefuser = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_bHasDefuser"));
	g_Offsets.m_bIsScoped = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_bIsScoped"));
	g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_flElasticity"));
	g_Offsets.m_bGunGameImmunity = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_bGunGameImmunity"));
	g_Offsets.m_fFlags = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_fFlags"));
	g_Offsets.m_nTickBase = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_nTickBase"));
	g_Offsets.m_angEyeAngles = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_angEyeAngles"));
	g_Offsets.m_flFlashMaxAlpha = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_flFlashMaxAlpha"));
	g_Offsets.m_flFlashDuration = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_flFlashDuration"));
	g_Offsets.m_vecViewOffset = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_vecViewOffset[0]"));
	g_Offsets.m_vecVelocity = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_vecVelocity[0]"));
	g_Offsets.m_hActiveWeapon = g_pNetVarsMan->GetOffset(XorString("DT_CSPlayer"), XorString("m_hActiveWeapon"));

	g_Offsets.m_Collision = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_Collision"));
	g_Offsets.m_aimPunchAngle = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_aimPunchAngle"));
	g_Offsets.m_vecOrigin = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_vecOrigin"));
	g_Offsets.m_nHitboxSet = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_nHitboxSet"));

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	g_Offsets.m_fAccuracyPenalty = g_pNetVarsMan->GetOffset(XorString("DT_WeaponCSBase"), XorString("m_fAccuracyPenalty"));

#endif

#endif

	g_Offsets.m_flNextPrimaryAttack = g_pNetVarsMan->GetOffset(XorString("DT_BaseCombatWeapon"), XorString("m_flNextPrimaryAttack"));
	g_Offsets.m_iClip1 = g_pNetVarsMan->GetOffset(XorString("DT_BaseCombatWeapon"), XorString("m_iClip1"));

	g_Offsets.m_flC4Blow = g_pNetVarsMan->GetOffset(XorString("DT_PlantedC4"), XorString("m_flC4Blow"));

	g_Offsets.m_iItemDefinitionIndex = g_pNetVarsMan->GetOffset(XorString("DT_BaseAttributableItem"), XorString("m_iItemDefinitionIndex"));

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

	DWORD dwGlowAddr = FindPattern(XorString("client.dll"), XorString("75 4B 0F 57 C0")) + 18;

	if (dwGlowAddr && dwGlowAddr != 18)
		g_Offsets.m_dwGlowManager = *(PDWORD)dwGlowAddr;

	else
		g_Offsets.m_dwGlowManager = NULL;

#endif

	DWORD dwD3DDevAddr = FindPattern(XorString("shaderapidx9.dll"), XorString("A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C")) + 0x00000001;

	if (dwD3DDevAddr && dwD3DDevAddr != 0x00000001)
		g_Offsets.m_dwD3DDevice = **(DWORD **)dwD3DDevAddr;

	g_Offsets.m_dwServerRankRevealAllEx = FindPattern(XorString("client.dll"), XorString("55 8B EC 8B 0D ? ? ? ? 68 ? ? ? ? "));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_BaseEntity"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_BaseGrenade"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_CSGrenade"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_BaseAttributableItem"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_BaseCombatWeapon"), XorString("m_flElasticity"));

	if (!g_Offsets.m_flElasticity)
		g_Offsets.m_flElasticity = g_pNetVarsMan->GetOffset(XorString("DT_WeaponCSBase"), XorString("m_flElasticity"));

	if (!g_Offsets.m_bHasDefuser)
		g_Offsets.m_bHasDefuser = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_bHasDefuser"));

	if (!g_Offsets.m_bHasDefuser)
		g_Offsets.m_bHasDefuser = g_pNetVarsMan->GetOffset(XorString("DT_BaseEntity"), XorString("m_bHasDefuser"));

	if (!g_Offsets.m_bIsDefusing)
		g_Offsets.m_bIsDefusing = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_bIsDefusing"));

	if (!g_Offsets.m_bIsDefusing)
		g_Offsets.m_bIsDefusing = g_pNetVarsMan->GetOffset(XorString("DT_BaseEntity"), XorString("m_bIsDefusing"));

	if (!g_Offsets.m_iObserverMode)
		g_Offsets.m_iObserverMode = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_iObserverMode"));

	if (!g_Offsets.m_iObserverMode)
		g_Offsets.m_iObserverMode = g_pNetVarsMan->GetOffset(XorString("DT_BaseEntity"), XorString("m_iObserverMode"));

	if (!g_Offsets.m_hObserverTarget)
		g_Offsets.m_hObserverTarget = g_pNetVarsMan->GetOffset(XorString("DT_BasePlayer"), XorString("m_hObserverTarget"));

	if (!g_Offsets.m_hObserverTarget)
		g_Offsets.m_hObserverTarget = g_pNetVarsMan->GetOffset(XorString("DT_BaseEntity"), XorString("m_hObserverTarget"));
};

string g_pFoVCircles[3] =
{
	XorString("STATIC"),
	XorString("DYNAMIC"),
	XorString("EXPANDABLE"),
};

string g_pCustomColorOptions[19] =
{
	XorString("WHITE"),
	XorString("WHITE SMOKE"),
	XorString("GREY"),
	XorString("DARK GREY"),
	XorString("BLACK"),
	XorString("RED"),
	XorString("DARK RED"),
	XorString("CANDY RED"),
	XorString("GREEN"),
	XorString("DARK GREEN"),
	XorString("CANDY GREEN"),
	XorString("BLUE"),
	XorString("DARK BLUE"),
	XorString("CANDY BLUE"),
	XorString("VIOLET"),
	XorString("YELLOW"),
	XorString("MAGENTA"),
	XorString("CYAN"),
	XorString("ORANGE"),
};

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

string g_pCustomGlowColorOptions[ARRAYSIZE(g_pCustomColorOptions) + 1] =
{
	XorString("DEFAULT"),
	XorString("WHITE"),
	XorString("WHITE SMOKE"),
	XorString("GREY"),
	XorString("DARK GREY"),
	XorString("BLACK"),
	XorString("RED"),
	XorString("DARK RED"),
	XorString("CANDY RED"),
	XorString("GREEN"),
	XorString("DARK GREEN"),
	XorString("CANDY GREEN"),
	XorString("BLUE"),
	XorString("DARK BLUE"),
	XorString("CANDY BLUE"),
	XorString("VIOLET"),
	XorString("YELLOW"),
	XorString("MAGENTA"),
	XorString("CYAN"),
	XorString("ORANGE"),
};

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

string g_pTriggerKeys[91] =
{
	XorString("N/ A"),
	XorString("L. MOUSE BUTTON"),
	XorString("R. MOUSE BUTTON"),
	XorString("CANCEL"),
	XorString("MID. MOUSE BUTTON"),
	XorString("X1 MOUSE BUTTON"),
	XorString("X2 MOUSE BUTTON"),
	XorString("N/ A"),
	XorString("BACKSPACE"),
	XorString("TAB"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("CLEAR"),
	XorString("ENTER") /* 13 */,
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("SHIFT"),
	XorString("CTRL"),
	XorString("ALT"),
	XorString("PAUSE"),
	XorString("CAPS LOCK"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("ESC") /* 27 */,
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("SPACE BAR") /* 32 */,
	XorString("PAGE UP"),
	XorString("PAGE DOWN"),
	XorString("END"),
	XorString("HOME"),
	XorString("LEFT ARROW"),
	XorString("UP ARROW"),
	XorString("RIGHT ARROW"),
	XorString("DOWN ARROW") /* 40 */,
	XorString("SELECT"),
	XorString("PRINT"),
	XorString("EXECUTE"),
	XorString("PRINT SCREEN"),
	XorString("INSERT (INS)"),
	XorString("DELETE (DEL)"),
	XorString("HELP"),
	XorString("0"),
	XorString("1"),
	XorString("2"),
	XorString("3"),
	XorString("4"),
	XorString("5"),
	XorString("6"),
	XorString("7"),
	XorString("8"),
	XorString("9") /* 57 */,
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("N/ A"),
	XorString("A") /* 65 */,
	XorString("B"),
	XorString("C"),
	XorString("D"),
	XorString("E"),
	XorString("F"),
	XorString("G"),
	XorString("H"),
	XorString("I"),
	XorString("J"),
	XorString("K"),
	XorString("L"),
	XorString("M"),
	XorString("N"),
	XorString("O"),
	XorString("P"),
	XorString("Q"),
	XorString("R"),
	XorString("S"),
	XorString("T"),
	XorString("U"),
	XorString("V"),
	XorString("W"),
	XorString("X"),
	XorString("Y"),
	XorString("Z") /* 90 */,
};

#endif

VOID WINAPI PaintTraverse(UINT uiPanel, bool bForceRePaint, bool bAllowForce)
{
	oPaintTraverse(uiPanel, bForceRePaint, bAllowForce);
	if (!g_uiDrawPanel)
	{
		g_pszPanelName = I::VPanel->GetName(uiPanel);
		if (g_pszPanelName[0] == 'M' && g_pszPanelName[2] == 't') g_uiDrawPanel = uiPanel;
	};
	if (uiPanel == g_uiDrawPanel && !g_bOffline) g_pVisuals->Run();
};

INT g_nHitBoxes[20 - 4] =
{
	HITBOX_HEAD,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	// HITBOX_RIGHT_FOOT,
	// HITBOX_LEFT_FOOT,
	// HITBOX_RIGHT_HAND,
	// HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
};

VOID CreateGUI(IDirect3DDevice9 * pDevice, string File)
{
	ImGui_ImplDX9_Init(g_pWnd, pDevice);
	ImGuiStyle & style = ImGui::GetStyle();

	style.Colors[ImGuiCol_Text] = ImVec4(.98f, .98f, .98f, 1.f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(.26f, .26f, .26f, .95f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(.21f, .21f, .21f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(.78f, .78f, .78f, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(.74f, .74f, .74f, 1.f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(.74f, .74f, .74f, 1.f);
	style.Colors[ImGuiCol_Button] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.43f, .43f, .43f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(.11f, .11f, .11f, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(.26f, .59f, .98f, 1.f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(.26f, .59f, .98f, 1.f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(.32f, .52f, .65f, 1.f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(.2f, .2f, .2f, .5f);

	style.GrabRounding = 0.f;
	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 0.f;
	style.FrameRounding = 0.f;
	style.WindowTitleAlign = ImVec2(.5f, .5f);

	ImGui::GetIO().Fonts->AddFontFromFileTTF(File.c_str(), 12.f);

	g_bD3DOK = true;
};

VOID IInputSystem::GetCursorPosition(PINT m_pX, PINT m_pY)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, PINT, PINT);
	return GetVFunc < OriginalFn >(this, 56) (this, m_pX, m_pY);
};

bool WINAPI CreateMove(FLOAT, CUserCmd * pCmd)
{

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	static INT iEnemy = 0, iWeapon = 0, iTeam = 0, iTick = 0;

#else

	static INT iEnemy = 0, iWeapon = 0, iTeam = 0;

#endif

	static Entity_T * pEnemy = NULL, *pWeapon = NULL;
	static Vector vecZone;
	static QAngle angAim, angCurPunch, angOldPunch;
	static bool bLastJumped = false, bShouldFake = false;

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	static PDWORD pPack = NULL;

#endif

	G::pSelf = GetSelf(), G::pCmd = pCmd;

	if (!pCmd || pCmd->cmdnum == 0 || !G::pSelf || G::iTeam < 1)
	{
		if (pCmd) pCmd->viewang.Clamp();
		return false;
	};

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

	if (Vars.Visuals.bON && Vars.Visuals.bGlow)
		g_pVisuals->Glow();

#endif

	if (!G::bAlive)
	{
		pCmd->viewang.Clamp();
		return false;
	};

	if (G::bScoped)
	{
		if (!g_bWasScoped) g_bWasScoped = true, g_zoomTime = clock();
	}

	else
	{
		if (g_bWasScoped) g_bWasScoped = false;
	};

	if (G::iWeapId > 0 && g_iLastItem != G::iWeapId)
		g_iLastItem = G::iWeapId, g_switchTime = clock();

	if (G::iFlags & FL_FROZEN)
	{
		if (!g_bFrozen)
		{
			sendConCmd(XorString("cl_mouseenable 0; unbind tab"));
			sendConCmd(XorString("unbind y; unbind u"));

			g_bFrozen = true;
		};

		G::pCmd->buttons &= ~IN_SCORE, G::Keys[VK_TAB] = false;
	}

	else
	{
		if (g_bFrozen)
		{
			sendConCmd(XorString("cl_mouseenable 1; bind tab +showscores"));
			sendConCmd(XorString("bind y messagemode; bind u messagemode2"));

			g_bFrozen = false;
		};
	};

	if (Vars.Menu.bOpened)
	{
		pCmd->viewang.Clamp();
		return false;
	};

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

	if (Vars.Misc.bFakeLag)
	{
		switch (Vars.Misc.iFakeLagType)
		{
		case NULL:
			__asm mov pPack, ebp;
			*(bool *)(*pPack - 0x0000001C) = RandomINT(0, 1) ? true : false;
			break;

		default:
			__asm mov pPack, ebp;
			*(bool *)(*pPack - 0x0000001C) = iTick % 2 ? true : false;
			if (++iTick == 128) iTick = 0;
			break;
		};
	};

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	if (!(G::iFlags & FL_FROZEN) && (Vars.Misc.bAutoLeftRight || Vars.Misc.bSpecialFlickShots) && \
		!(G::pCmd->buttons & IN_WALK) && !(G::pCmd->buttons & IN_DUCK) && !(G::pCmd->buttons & IN_ATTACK) && \
		!(G::iFlags & FL_DUCKING) && G::iFlags & FL_ONGROUND && !(G::pCmd->buttons & IN_JUMP))
	{
		for (iEnemy = 0; iEnemy <= I::Globals->m_iMaxClients; iEnemy++)
		{
			if (g_bSmokeExcluded[iEnemy] || !(pEnemy = I::Ents->GetClEntity(iEnemy)) || pEnemy == G::pSelf || pEnemy->IsDormant() || pEnemy->GetHealth() < 1 || !(pWeapon = pEnemy->GetWeapon()) || (iWeapon = pWeapon->GetItemDefinitionIndex()) < 1)
				continue;

			switch (iWeapon)
			{
			case WEAPON_AWP: case WEAPON_SSG08: case WEAPON_SCAR20: case WEAPON_G3SG1:
			{
				if (pEnemy->IsSpottingMe())
				{
					if (Vars.Misc.bSpecialFlickShots && FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && \
						(G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER || G::iWeapId == WEAPON_AWP || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_DEAGLE) && \
						pEnemy->GetTeam() != G::iTeam && ((G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER) || G::bScoped) && !pEnemy->GetImmune() && TopZone((G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER) ? true : false, pEnemy, vecZone, 1.f, NULL))
					{
						if (G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_AWP)
						{
							if (!G::fSelfVelLen)
							{
								if (!IS_SNIPER(G::iWeapId))
									angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
									G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;

								else
								{
									if (FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f))
										angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
										G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;
								};
							};
						}

						else
						{
							angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
								G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;
						};
					}

					else if (Vars.Misc.bAutoLeftRight && !TopZone(false, pEnemy, vecZone, 3.f, NULL))
					{
						switch (time(NULL) % 2 == 0)
						{
						case true:
						{
							G::pCmd->sidemove = -450.f;
							break;
						};

						case false:
						{
							G::pCmd->sidemove = 450.f;
							break;
						};
						};

						if (G::pCmd->fwdmove != 0.f)
							G::pCmd->fwdmove = -450.f;
					};
				};

				break;
			};

			default:
			{
				if (pEnemy->IsSpottingMyHead())
				{
					if (Vars.Misc.bSpecialFlickShots && FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && \
						(G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER || G::iWeapId == WEAPON_AWP || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_DEAGLE) && \
						pEnemy->GetTeam() != G::iTeam && ((G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER) || G::bScoped) && !pEnemy->GetImmune() && TopZone((G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_HKP2000 || G::iWeapId == WEAPON_USP_SILENCER) ? true : false, pEnemy, vecZone, 1.f, NULL))
					{
						if (G::iWeapId == WEAPON_DEAGLE || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_AWP)
						{
							if (!G::fSelfVelLen)
							{
								if (!IS_SNIPER(G::iWeapId))
									angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
									G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;

								else
								{
									if (FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f))
										angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
										G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;
								};
							};
						}

						else
						{
							angAim = CalcAng(G::vecSelfEyePos, vecZone) - G::angSelfPunch * 2.f, \
								G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim), G::pCmd->buttons |= IN_ATTACK;
						};
					}

					else if (Vars.Misc.bAutoLeftRight && !TopZone(false, pEnemy, vecZone, 3.f, NULL))
					{
						switch (time(NULL) % 2 == 0)
						{
						case false:
						{
							G::pCmd->sidemove = -450.f;
							break;
						};

						case true:
						{
							G::pCmd->sidemove = 450.f;
							break;
						};
						};
					};
				};

				break;
			};
			};
		};
	};

#else

	if (!(G::iFlags & FL_FROZEN) && Vars.Misc.bAutoLeftRight && \
		!(G::pCmd->buttons & IN_WALK) && !(G::pCmd->buttons & IN_DUCK) && !(G::pCmd->buttons & IN_ATTACK) && \
		!(G::iFlags & FL_DUCKING) && G::iFlags & FL_ONGROUND && !(G::pCmd->buttons & IN_JUMP))
	{
		for (iEnemy = 0; iEnemy <= I::Globals->m_iMaxClients; iEnemy++)
		{
			if (g_bSmokeExcluded[iEnemy] || !(pEnemy = I::Ents->GetClEntity(iEnemy)) || pEnemy == G::pSelf || pEnemy->IsDormant() || pEnemy->GetHealth() < 1 || !(pWeapon = pEnemy->GetWeapon()) || (iWeapon = pWeapon->GetItemDefinitionIndex()) < 1)
				continue;

			switch (iWeapon)
			{
			case WEAPON_AWP: case WEAPON_SSG08: case WEAPON_SCAR20: case WEAPON_G3SG1:
			{
				if (pEnemy->IsSpottingMe())
				{
					if (Vars.Misc.bAutoLeftRight && !TopZone(false, pEnemy, vecZone, 3.f, NULL))
					{
						switch (time(NULL) % 2 == 0)
						{
						case true:
						{
							G::pCmd->sidemove = -450.f;
							break;
						};

						case false:
						{
							G::pCmd->sidemove = 450.f;
							break;
						};
						};

						if (G::pCmd->fwdmove != 0.f)
							G::pCmd->fwdmove = -450.f;
					};
				};

				break;
			};

			default:
			{
				if (pEnemy->IsSpottingMyHead())
				{
					if (Vars.Misc.bAutoLeftRight && !TopZone(false, pEnemy, vecZone, 3.f, NULL))
					{
						switch (time(NULL) % 2 == 0)
						{
						case false:
						{
							G::pCmd->sidemove = -450.f;
							break;
						};

						case true:
						{
							G::pCmd->sidemove = 450.f;
							break;
						};
						};
					};
				};

				break;
			};
			};
		};
	};

#endif

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

	if (Vars.Misc.bAutoStrafe && !(G::iFlags & FL_ONGROUND) && (G::pCmd->mousedx > 1 || G::pCmd->mousedx < -1))
		G::pCmd->sidemove = G::pCmd->mousedx < 0 ? -450.f : 450.f;

#endif

	if (!G::bEmpty && G::bGun && G::iWeapId != WEAPON_REVOLVER && G::iWeapId != WEAPON_NEGEV && !(G::iFlags & FL_FROZEN))
	{

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

		if (Vars.RageBOT.bON)
			g_pRageBOT->DoAim();

		else

#endif

			g_pLegitBOT->Run();
	};

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

	if (Vars.Misc.bBunnyHop)
	{
		if (!bLastJumped && bShouldFake)
		{
			bShouldFake = false;
			G::pCmd->buttons |= IN_JUMP;
		}

		else if (G::pCmd->buttons & IN_JUMP)
		{
			if (G::pSelf->GetFlags() & FL_ONGROUND)
				bLastJumped = bShouldFake = true;

			else
			{
				G::pCmd->buttons &= ~IN_JUMP;
				bLastJumped = false;
			};
		}

		else
			bLastJumped = bShouldFake = false;
	};

#endif

	pCmd->viewang.Clamp();
	return false;
};

LRESULT WINAPI OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static UINT uiButton = 0;

	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		G::Keys[VK_LBUTTON] = true;
		break;

	case WM_LBUTTONUP:
		G::Keys[VK_LBUTTON] = false;
		break;

	case WM_RBUTTONDOWN:
		G::Keys[VK_RBUTTON] = true;
		break;

	case WM_RBUTTONUP:
		G::Keys[VK_RBUTTON] = false;
		break;

	case WM_MBUTTONDOWN:
		G::Keys[VK_MBUTTON] = true;
		break;

	case WM_MBUTTONUP:
		G::Keys[VK_MBUTTON] = false;
		break;

	case WM_XBUTTONDOWN:
	{
		uiButton = GET_XBUTTON_WPARAM(wParam);

		if (uiButton == XBUTTON1)
			G::Keys[VK_XBUTTON1] = true;

		else if (uiButton == XBUTTON2)
			G::Keys[VK_XBUTTON2] = true;

		break;
	};

	case WM_XBUTTONUP:
	{
		uiButton = GET_XBUTTON_WPARAM(wParam);

		if (uiButton == XBUTTON1)
			G::Keys[VK_XBUTTON1] = false;

		else if (uiButton == XBUTTON2)
			G::Keys[VK_XBUTTON2] = false;

		break;
	};

	case WM_KEYDOWN:
		G::Keys[wParam] = true;
		break;

	case WM_KEYUP:
		G::Keys[wParam] = false;
		break;

	case WM_SYSKEYDOWN:
		if (wParam == VK_MENU)
			G::Keys[VK_MENU] = true;

		break;

	case WM_SYSKEYUP:
		if (wParam == VK_MENU)
			G::Keys[VK_MENU] = false;

		break;
	};

	if (g_bD3DOK && Vars.Menu.bOpened && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return LRESULT(true);

	return (LRESULT)CallWindowProc(g_pOldWndProc, hWnd, uMsg, wParam, lParam);
};

INT RandomINT(INT iMin, INT iMax)
{
	return (rand() % (iMax + 1 - iMin)) + iMin;
};

VOID replaceAll(string & Str, string From, string To)
{
	static INT s = 0, e = 0;

	s = e = 0;

	while ((s = Str.find(From, s)) != -1)
		e = s + From.length(), Str.replace(s, e, To), s += To.length();
};

BOOL xIsCharUpper(CHAR x)
{
	if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
		return IsCharUpper(x) ? TRUE : FALSE;

	return FALSE;
};

BOOL xIsCharLower(CHAR x)
{
	if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
		return IsCharLower(x) ? TRUE : FALSE;

	return FALSE;
};

SIZE_T countChars(string Buffer, CHAR Char)
{
	static INT iIter = 0;
	static SIZE_T iCount = 0;

	iCount = 0;

	for (iIter = 0; iIter < Buffer.length(); iIter++)
		if (Buffer.at(iIter) == Char)
			iCount++;

	return iCount;
};

string ShortenNameNoRandom(string Name)
{
	static INT iPos = 0, iSize = 0, iLen = 0, iMaxLen = 0, iMany = 0;
	static string xRes = {};
	static vector < string > xArray = {};
	static PCHAR pToken = {};
	static CHAR szBuffer[XS_RANDOM_SIZE] = {};

	xRes.assign(Name);

	if (xRes.length() < 1)
	{
		xRes.assign(XorString("???"));
		return xRes;
	};

	replaceAll(xRes, XorString("%"), XorString("*"));
	replaceAll(xRes, XorString(";"), XorString("*"));
	replaceAll(xRes, XorString(":"), XorString("*"));
	replaceAll(xRes, XorString("\""), XorString("*"));

	if (xRes.find_last_of(XorString(" ")) < 0 || xRes.length() < 8)
		return xRes;

	strcpy(szBuffer, xRes.c_str());
	pToken = strtok(szBuffer, XorString(" "));

	xArray.clear();
	iMany = 0;

	while (pToken)
	{
		xArray.push_back(pToken);
		pToken = strtok(NULL, XorString(" "));

		iMany++;
	};

	iSize = xArray.size();

	if (iSize < 1)
		return xRes;

	iMaxLen = 0;

	for (iPos = 0; iPos < iSize; iPos++)
	{
		iLen = xArray.at(iPos).length();

		if (iLen > iMaxLen)
		{
			if (xArray.at(iPos).length() < 3 && iMany > 1)
				continue;

			if (iMany > 1 && (stristr(xArray.at(iPos).c_str(), XorString(".COM")) || stristr(xArray.at(iPos).c_str(), XorString(".NET"))))
				continue;

			iMaxLen = iLen;
			xRes = xArray.at(iPos);
		};
	};

	if ((xRes.at(0) == '-' || xRes.at(xRes.length() - 1) == '-') && countChars(xRes, '-') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("-"), XorString(""));

	if ((xRes.at(0) == '!' || xRes.at(xRes.length() - 1) == '!') && countChars(xRes, '!') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("!"), XorString(""));

	if ((xRes.at(0) == '?' || xRes.at(xRes.length() - 1) == '?') && countChars(xRes, '?') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("!"), XorString(""));

	if ((xRes.at(0) == '.' || xRes.at(xRes.length() - 1) == '.') && countChars(xRes, '.') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("."), XorString(""));

	if ((xRes.at(0) == '#' || xRes.at(xRes.length() - 1) == '#') && countChars(xRes, '#') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("#"), XorString(""));

	if ((xRes.at(0) == '$' || xRes.at(xRes.length() - 1) == '$') && countChars(xRes, '$') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("$"), XorString(""));

	if ((xRes.at(0) == '@' || xRes.at(xRes.length() - 1) == '@') && countChars(xRes, '@') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("@"), XorString(""));

	if ((xRes.at(0) == '^' || xRes.at(xRes.length() - 1) == '^') && countChars(xRes, '^') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("^"), XorString(""));

	if ((xRes.at(0) == '[' || xRes.at(xRes.length() - 1) == '[') && countChars(xRes, '[') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("["), XorString(""));

	if ((xRes.at(0) == ']' || xRes.at(xRes.length() - 1) == ']') && countChars(xRes, ']') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("]"), XorString(""));

	if ((xRes.at(0) == '<' || xRes.at(xRes.length() - 1) == '<') && countChars(xRes, '<') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("<"), XorString(""));

	if ((xRes.at(0) == '>' || xRes.at(xRes.length() - 1) == '>') && countChars(xRes, '>') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString(">"), XorString(""));

	if ((xRes.at(0) == ')' || xRes.at(xRes.length() - 1) == ')') && countChars(xRes, ')') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString(")"), XorString(""));

	if ((xRes.at(0) == '(' || xRes.at(xRes.length() - 1) == '(') && countChars(xRes, '(') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("("), XorString(""));

	return xRes;
};

string ShortenName(string Name)
{
	static INT iPos = 0, iSize = 0, iLen = 0, iMaxLen = 0, iMany = 0;
	static string xRes = {};
	static vector < string > xArray = {};
	static PCHAR pToken = {};
	static CHAR szBuffer[XS_RANDOM_SIZE] = {};

	xRes.assign(Name);

	if (xRes.length() < 1)
	{
		xRes.assign(XorString("???"));
		return xRes;
	};

	replaceAll(xRes, XorString("%"), XorString("*"));
	replaceAll(xRes, XorString(";"), XorString("*"));
	replaceAll(xRes, XorString(":"), XorString("*"));
	replaceAll(xRes, XorString("\""), XorString("*"));

	if (xRes.find_last_of(XorString(" ")) < 0 || xRes.length() < 8)
	{
		if ((xRes.at(0) >= 'a' && xRes.at(0) <= 'z') || (xRes.at(0) >= 'A' && xRes.at(0) <= 'Z'))
		{
			if (RandomINT(0, 1) == 0 && xIsCharLower(xRes.at(1)))
				xRes.at(0) = tolower(xRes.at(0));

			else
				xRes.at(0) = toupper(xRes.at(0));
		};

		return xRes;
	};

	strcpy(szBuffer, xRes.c_str());
	pToken = strtok(szBuffer, XorString(" "));

	xArray.clear();
	iMany = 0;

	while (pToken)
	{
		xArray.push_back(pToken);
		pToken = strtok(NULL, XorString(" "));

		iMany++;
	};

	iSize = xArray.size();

	if (iSize < 1)
	{
		if ((xRes.at(0) >= 'a' && xRes.at(0) <= 'z') || (xRes.at(0) >= 'A' && xRes.at(0) <= 'Z'))
		{
			if (RandomINT(0, 1) == 0 && xIsCharLower(xRes.at(1)))
				xRes.at(0) = tolower(xRes.at(0));

			else
				xRes.at(0) = toupper(xRes.at(0));
		};

		return xRes;
	};

	iMaxLen = 0;

	for (iPos = 0; iPos < iSize; iPos++)
	{
		iLen = xArray.at(iPos).length();

		if (iLen > iMaxLen)
		{
			if (xArray.at(iPos).length() < 3 && iMany > 1)
				continue;

			if (iMany > 1 && (stristr(xArray.at(iPos).c_str(), XorString(".COM")) || stristr(xArray.at(iPos).c_str(), XorString(".NET"))))
				continue;

			iMaxLen = iLen;
			xRes = xArray.at(iPos);
		};
	};

	if ((xRes.at(0) >= 'a' && xRes.at(0) <= 'z') || (xRes.at(0) >= 'A' && xRes.at(0) <= 'Z'))
	{
		if (RandomINT(0, 1) == 0 && xIsCharLower(xRes.at(1)))
			xRes.at(0) = tolower(xRes.at(0));

		else
			xRes.at(0) = toupper(xRes.at(0));
	};

	if ((xRes.at(0) == '-' || xRes.at(xRes.length() - 1) == '-') && countChars(xRes, '-') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("-"), XorString(""));

	if ((xRes.at(0) == '!' || xRes.at(xRes.length() - 1) == '!') && countChars(xRes, '!') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("!"), XorString(""));

	if ((xRes.at(0) == '?' || xRes.at(xRes.length() - 1) == '?') && countChars(xRes, '?') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("!"), XorString(""));

	if ((xRes.at(0) == '.' || xRes.at(xRes.length() - 1) == '.') && countChars(xRes, '.') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("."), XorString(""));

	if ((xRes.at(0) == '#' || xRes.at(xRes.length() - 1) == '#') && countChars(xRes, '#') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("#"), XorString(""));

	if ((xRes.at(0) == '$' || xRes.at(xRes.length() - 1) == '$') && countChars(xRes, '$') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("$"), XorString(""));

	if ((xRes.at(0) == '@' || xRes.at(xRes.length() - 1) == '@') && countChars(xRes, '@') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("@"), XorString(""));

	if ((xRes.at(0) == '^' || xRes.at(xRes.length() - 1) == '^') && countChars(xRes, '^') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("^"), XorString(""));

	if ((xRes.at(0) == '[' || xRes.at(xRes.length() - 1) == '[') && countChars(xRes, '[') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("["), XorString(""));

	if ((xRes.at(0) == ']' || xRes.at(xRes.length() - 1) == ']') && countChars(xRes, ']') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("]"), XorString(""));

	if ((xRes.at(0) == '<' || xRes.at(xRes.length() - 1) == '<') && countChars(xRes, '<') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("<"), XorString(""));

	if ((xRes.at(0) == '>' || xRes.at(xRes.length() - 1) == '>') && countChars(xRes, '>') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString(">"), XorString(""));

	if ((xRes.at(0) == ')' || xRes.at(xRes.length() - 1) == ')') && countChars(xRes, ')') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString(")"), XorString(""));

	if ((xRes.at(0) == '(' || xRes.at(xRes.length() - 1) == '(') && countChars(xRes, '(') < 3 && xRes.length() > 3)
		replaceAll(xRes, XorString("("), XorString(""));

	return xRes;
};

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

VOID PrepareSpamString(PCHAR pData, PlrInfo_T * pPlayer)
{
	static string Shorten;

	*pData = NULL;

	Shorten = ShortenName(pPlayer->name);

	switch (RandomINT(0, 545))
	{
	case 0: xprintf(pData, XorString("%s, i fuck your mom"), Shorten.c_str()); break;
	case 1: xprintf(pData, XorString("geh weg, %s"), Shorten.c_str()); break;
	case 2: xprintf(pData, XorString("aufgeben, %s"), Shorten.c_str()); break;
	case 3: xprintf(pData, XorString("%s, du dumme schlampe.."), Shorten.c_str()); break;
	case 4: xprintf(pData, XorString("siktir git, %s"), Shorten.c_str()); break;
	case 5: xprintf(pData, XorString("hay siktir, %s"), Shorten.c_str()); break;
	case 6: xprintf(pData, XorString("%s, how much $ for hack?"), Shorten.c_str()); break;
	case 7: xprintf(pData, XorString("u using hack, %s ?"), Shorten.c_str()); break;
	case 8: xprintf(pData, XorString("cumback israel, %s ?"), Shorten.c_str()); break;
	case 9: xprintf(pData, XorString("i fuck your mom, %s"), Shorten.c_str()); break;
	case 10: xprintf(pData, XorString("nice engrish, %s"), Shorten.c_str()); break;
	case 11: xprintf(pData, XorString("nice frenglish, %s"), Shorten.c_str()); break;
	case 12: xprintf(pData, XorString("u cute, %s ?"), Shorten.c_str()); break;
	case 13: xprintf(pData, XorString("%s, I FUCK YOUR SISTER"), Shorten.c_str()); break;
	case 14: xprintf(pData, XorString("u so fucked, %s"), Shorten.c_str()); break;
	case 15: xprintf(pData, XorString("u horny, %s ?"), Shorten.c_str()); break;
	case 16: xprintf(pData, XorString("fick dich, %s"), Shorten.c_str()); break;
	case 17: xprintf(pData, XorString("%s, u fucking CUNT"), Shorten.c_str()); break;
	case 18: xprintf(pData, XorString("%s, n1 hack"), Shorten.c_str()); break;
	case 19: xprintf(pData, XorString("nice hack, %s"), Shorten.c_str()); break;
	case 20: xprintf(pData, XorString("eat my cock, %s"), Shorten.c_str()); break;
	case 21: xprintf(pData, XorString("să-mi bei boașele, %s"), Shorten.c_str()); break;
	case 22: xprintf(pData, XorString("să moară mă-ta, %s ?"), Shorten.c_str()); break;
	case 23: xprintf(pData, XorString("te fut, %s"), Shorten.c_str()); break;
	case 24: xprintf(pData, XorString("АНАЛЬНЫЙ СЕКС, %s ?"), Shorten.c_str()); break;
	case 25: xprintf(pData, XorString("ХУЙ ТЕБЯ, %s !"), Shorten.c_str()); break;
	case 26: xprintf(pData, XorString("ТЫ СОСАТЬ ХУЙ, %s ?"), Shorten.c_str()); break;
	case 27: xprintf(pData, XorString("%s, ТЫ КУРИТЕ ??"), Shorten.c_str()); break;
	case 28: xprintf(pData, XorString("%s, u like BBC?"), Shorten.c_str()); break;
	case 29: xprintf(pData, XorString("go surrender, %s .."), Shorten.c_str()); break;
	case 30: xprintf(pData, XorString("how mush paid for hax, %s ??"), Shorten.c_str()); break;
	case 31: xprintf(pData, XorString("temiz dişler, %s ?"), Shorten.c_str()); break;
	case 32: xprintf(pData, XorString("fahişe anne, %s ??"), Shorten.c_str()); break;
	case 33: xprintf(pData, XorString("orospu anne, %s ?"), Shorten.c_str()); break;
	case 34: xprintf(pData, XorString("büyük çük, %s ??"), Shorten.c_str()); break;
	case 35: xprintf(pData, XorString("parles-tu français, %s ?"), Shorten.c_str()); break;
	case 36: xprintf(pData, XorString("i hate u, %s"), Shorten.c_str()); break;
	case 37: xprintf(pData, XorString("%s, how mush money for hack ??"), Shorten.c_str()); break;
	case 38: xprintf(pData, XorString("%s, go fuck yourself.."), Shorten.c_str()); break;
	case 39: xprintf(pData, XorString("i bet you have hard cock, %s"), Shorten.c_str()); break;
	case 40: xprintf(pData, XorString("i like hard anal sex, %s"), Shorten.c_str()); break;
	case 41: xprintf(pData, XorString("%s, u suck for $$ ?"), Shorten.c_str()); break;
	case 42: xprintf(pData, XorString("%s, ur mom knows ur retard?"), Shorten.c_str()); break;
	case 43: xprintf(pData, XorString("hi %s"), Shorten.c_str()); break;
	case 44: xprintf(pData, XorString("%s, ur dick 24cm+ ?"), Shorten.c_str()); break;
	case 45: xprintf(pData, XorString("%s, how long your dick=?"), Shorten.c_str()); break;
	case 46: xprintf(pData, XorString("%s, last time u masturbate =?"), Shorten.c_str()); break;
	case 47: xprintf(pData, XorString("ns, %s"), Shorten.c_str()); break;
	case 48: xprintf(pData, XorString("u such a SWAG, %s"), Shorten.c_str()); break;
	case 49: xprintf(pData, XorString("ПРИВЕТ, %s"), Shorten.c_str()); break;
	case 50: xprintf(pData, XorString("ТЫ ЗНАЕШЬ КАРМЕЛИТА ЦЫГАНСКАЯ СТРАСТЬ, %s ?"), Shorten.c_str()); break;
	case 51: xprintf(pData, XorString("ТОЛЬКО ТЫ МЕНЯ КОЛЫШЕШЬ, %s ?"), Shorten.c_str()); break;
	case 52: xprintf(pData, XorString("ПОСТОЙ ПАРОВОЗ, %s ?"), Shorten.c_str()); break;
	case 53: xprintf(pData, XorString("НЕТ ТАКИХ УЖЕ ЛИЧНОСТЕЙ, %s .."), Shorten.c_str()); break;
	case 54: xprintf(pData, XorString("Я ВАС ЛЮБЛЮ, %s !!"), Shorten.c_str()); break;
	case 55: xprintf(pData, XorString("СПАСИБО ЧТО ТЫ БЫЛ, %s"), Shorten.c_str()); break;
	case 56: xprintf(pData, XorString("СПАСИБО, %s !"), Shorten.c_str()); break;
	case 57: xprintf(pData, XorString("ХОРОШИЙ АРТИСТ, %s"), Shorten.c_str()); break;
	case 58: xprintf(pData, XorString("КЛАСС, %s !!"), Shorten.c_str()); break;
	case 59: xprintf(pData, XorString("КТО СУКА, %s ??"), Shorten.c_str()); break;
	case 60: xprintf(pData, XorString("%s, ИДИОТ ПЕДОФИЛ .."), Shorten.c_str()); break;
	case 61: xprintf(pData, XorString("%s, îmi sugi pula !!"), Shorten.c_str()); break;
	case 62: xprintf(pData, XorString("u got nice butt, %s ?"), Shorten.c_str()); break;
	case 63: xprintf(pData, XorString("enjoying gay porn, %s ?"), Shorten.c_str()); break;
	case 64: xprintf(pData, XorString("enjoying blowjobs, %s ?"), Shorten.c_str()); break;
	case 65: xprintf(pData, XorString("%s, u from =?"), Shorten.c_str()); break;
	case 66: xprintf(pData, XorString("%s, u arab?"), Shorten.c_str()); break;
	case 67: xprintf(pData, XorString("%s, u arabs?"), Shorten.c_str()); break;
	case 68: xprintf(pData, XorString("%s, u arabiqué?"), Shorten.c_str()); break;
	case 69: xprintf(pData, XorString("%s, u like anal Cum?"), Shorten.c_str()); break;
	case 70: xprintf(pData, XorString("%s, u hav RedTube.com Acc?"), Shorten.c_str()); break;
	case 71: xprintf(pData, XorString("%s, u hav XNXX.com acc?"), Shorten.c_str()); break;
	case 72: xprintf(pData, XorString("u watchin xHamster.com, %s ?"), Shorten.c_str()); break;
	case 73: xprintf(pData, XorString("u got big Dick, %s ?"), Shorten.c_str()); break;
	case 74: xprintf(pData, XorString("i ❤ u, %s"), Shorten.c_str()); break;
	case 75: xprintf(pData, XorString("u fkn slave, %s"), Shorten.c_str()); break;
	case 76: xprintf(pData, XorString("this will go on YT™, %s"), Shorten.c_str()); break;
	case 77: xprintf(pData, XorString("vaffanculo, %s"), Shorten.c_str()); break;
	case 78: xprintf(pData, XorString("¡ven a mi cama, %s!"), Shorten.c_str()); break;
	case 79: xprintf(pData, XorString("¿de donde eres, %s?"), Shorten.c_str()); break;
	case 80: xprintf(pData, XorString("dass der Tod sie Scheidet, %s ?"), Shorten.c_str()); break;
	case 81: xprintf(pData, XorString("u speak Hitler, %s ?"), Shorten.c_str()); break;
	case 82: xprintf(pData, XorString("u NAZI, %s ?"), Shorten.c_str()); break;
	case 83: xprintf(pData, XorString("how mutch $ to fuck ur mom, %s ?"), Shorten.c_str()); break;
	case 84: xprintf(pData, XorString("how many dildos u own, %s ?"), Shorten.c_str()); break;
	case 85: xprintf(pData, XorString("u using fleshlight, %s ?"), Shorten.c_str()); break;
	case 86: xprintf(pData, XorString("%s, u havin Dildos collection?"), Shorten.c_str()); break;
	case 87: xprintf(pData, XorString("%s, u hav 24cm+ dildo?"), Shorten.c_str()); break;
	case 88: xprintf(pData, XorString("%s, i think i luv ur mom?"), Shorten.c_str()); break;
	case 89: xprintf(pData, XorString("%s, u incest with your sis?"), Shorten.c_str()); break;
	case 90: xprintf(pData, XorString("%s, u incest with your mum?"), Shorten.c_str()); break;
	case 91: xprintf(pData, XorString("%s, can u self-suck for me?"), Shorten.c_str()); break;
	case 92: xprintf(pData, XorString("%s, last time u washd your Teeth?"), Shorten.c_str()); break;
	case 93: xprintf(pData, XorString("u having AIDS, %s ?"), Shorten.c_str()); break;
	case 94: xprintf(pData, XorString("u on Ebola, %s ?"), Shorten.c_str()); break;
	case 95: xprintf(pData, XorString("ЧЕЧНИ ВОДКА, %s ?"), Shorten.c_str()); break;
	case 96: xprintf(pData, XorString("u hav good AimBOT, %s"), Shorten.c_str()); break;
	case 97: xprintf(pData, XorString("ur mom has good Tits, %s ?"), Shorten.c_str()); break;
	case 98: xprintf(pData, XorString("how much KG u have, %s ?"), Shorten.c_str()); break;
	case 99: xprintf(pData, XorString("%s, how tall r you 2m?"), Shorten.c_str()); break;
	case 100: xprintf(pData, XorString("ur mom enjoys Anal, %s ?"), Shorten.c_str()); break;
	case 101: xprintf(pData, XorString("u like hentai sex, %s ?"), Shorten.c_str()); break;
	case 102: xprintf(pData, XorString("u doing 69 with your sis, %s ?"), Shorten.c_str()); break;
	case 103: xprintf(pData, XorString("Bazd meg, %s !"), Shorten.c_str()); break;
	case 104: xprintf(pData, XorString("Utállak, %s !"), Shorten.c_str()); break;
	case 105: xprintf(pData, XorString("van nagy faszod, %s ?"), Shorten.c_str()); break;
	case 106: xprintf(pData, XorString("Menj el, %s !"), Shorten.c_str()); break;
	case 107: xprintf(pData, XorString("add meg magad, %s !"), Shorten.c_str()); break;
	case 108: xprintf(pData, XorString("meg baszom az anyádat, %s !"), Shorten.c_str()); break;
	case 109: xprintf(pData, XorString("БОЛЬШОЙ ПЕНИС, %s ?"), Shorten.c_str()); break;
	case 110: xprintf(pData, XorString("ur having fun, %s ?"), Shorten.c_str()); break;
	case 111: xprintf(pData, XorString("u liek Porn films, %s ?"), Shorten.c_str()); break;
	case 112: xprintf(pData, XorString("i like ur sister, %s"), Shorten.c_str()); break;
	case 113: xprintf(pData, XorString("can i have ur sister, %s ?"), Shorten.c_str()); break;
	case 114: xprintf(pData, XorString("%s, how much $ for your mom?"), Shorten.c_str()); break;
	case 115: xprintf(pData, XorString("%s, is ur mom MILF?"), Shorten.c_str()); break;
	case 116: xprintf(pData, XorString("%s, ХУЙ ЗНАЕШЬ ?"), Shorten.c_str()); break;
	case 117: xprintf(pData, XorString("%s, КАК ТЕБЯ ЗОВУТ?"), Shorten.c_str()); break;
	case 118: xprintf(pData, XorString("КАК ДЕЛА, %s ?"), Shorten.c_str()); break;
	case 119: xprintf(pData, XorString("Anal seks, %s ?"), Shorten.c_str()); break;
	case 120: xprintf(pData, XorString("Kocaman çük, %s ?"), Shorten.c_str()); break;
	case 121: xprintf(pData, XorString("nize hack, %s"), Shorten.c_str()); break;
	case 122: xprintf(pData, XorString("%s, u cummed today?"), Shorten.c_str()); break;
	case 123: xprintf(pData, XorString("%s, u crying?"), Shorten.c_str()); break;
	case 124: xprintf(pData, XorString("cry moar, %s"), Shorten.c_str()); break;
	case 125: xprintf(pData, XorString("%s, cry me rivers"), Shorten.c_str()); break;
	case 126: xprintf(pData, XorString("cry me a river, %s"), Shorten.c_str()); break;
	case 127: xprintf(pData, XorString("excellent, %s"), Shorten.c_str()); break;
	case 128: xprintf(pData, XorString("%s, u from plastic?"), Shorten.c_str()); break;
	case 129: xprintf(pData, XorString("far from plastic, %s ?"), Shorten.c_str()); break;
	case 130: xprintf(pData, XorString("what hack u have, %s ?"), Shorten.c_str()); break;
	case 131: xprintf(pData, XorString("u hacking hard, %s ?"), Shorten.c_str()); break;
	case 132: xprintf(pData, XorString("u think im hacking, %s ?"), Shorten.c_str()); break;
	case 133: xprintf(pData, XorString("how can i hack with 3 karambits, %s ?"), Shorten.c_str()); break;
	case 134: xprintf(pData, XorString("how can i hack with 2 bayonets, %s ?"), Shorten.c_str()); break;
	case 135: xprintf(pData, XorString("%s, why u hack with $3000 inv?"), Shorten.c_str()); break;
	case 136: xprintf(pData, XorString("%s, u fan of Lisa Ann?"), Shorten.c_str()); break;
	case 137: xprintf(pData, XorString("%s, am pl de 1m și o să te cam FUT"), Shorten.c_str()); break;
	case 138: xprintf(pData, XorString("%s, care ești mă ?"), Shorten.c_str()); break;
	case 139: xprintf(pData, XorString("%s, când aduci banii pe HACK?"), Shorten.c_str()); break;
	case 140: xprintf(pData, XorString("%s, u fkn CUNT"), Shorten.c_str()); break;
	case 141: xprintf(pData, XorString("%s, turn off man"), Shorten.c_str()); break;
	case 142: xprintf(pData, XorString("%s, why u hack?"), Shorten.c_str()); break;
	case 143: xprintf(pData, XorString("%s, how mush $ your hack?"), Shorten.c_str()); break;
	case 144: xprintf(pData, XorString("%s, how mutch $ your hack?"), Shorten.c_str()); break;
	case 145: xprintf(pData, XorString("%s, whats your hack name ?"), Shorten.c_str()); break;
	case 146: xprintf(pData, XorString("Nerelisin, %s ?"), Shorten.c_str()); break;
	case 147: xprintf(pData, XorString("where u from, %s ?"), Shorten.c_str()); break;
	case 148: xprintf(pData, XorString("ОТКУДА ТЫ, %s ?"), Shorten.c_str()); break;
	case 149: xprintf(pData, XorString("u like gangBang, %s ?"), Shorten.c_str()); break;
	case 150: xprintf(pData, XorString("u liek group seks, %s ?"), Shorten.c_str()); break;
	case 151: xprintf(pData, XorString("%s, u like ponies?"), Shorten.c_str()); break;
	case 152: xprintf(pData, XorString("%s, can u suck my dick?"), Shorten.c_str()); break;
	case 153: xprintf(pData, XorString("buy Caramel hack, %s .."), Shorten.c_str()); break;
	case 154: xprintf(pData, XorString("u need Caramel hack, %s"), Shorten.c_str()); break;
	case 155: xprintf(pData, XorString("why u so low, %s ?"), Shorten.c_str()); break;
	case 156: xprintf(pData, XorString("can i suck ur balls, %s ?"), Shorten.c_str()); break;
	case 157: xprintf(pData, XorString("u need hack, %s .."), Shorten.c_str()); break;
	case 158: xprintf(pData, XorString("%s, u need better hack"), Shorten.c_str()); break;
	case 159: xprintf(pData, XorString("surprise, %s"), Shorten.c_str()); break;
	case 160: xprintf(pData, XorString("go die, %s .."), Shorten.c_str()); break;
	case 161: xprintf(pData, XorString("%s, go die in inferno"), Shorten.c_str()); break;
	case 162: xprintf(pData, XorString("die in hell, %s"), Shorten.c_str()); break;
	case 163: xprintf(pData, XorString("%s, u obvious fuck.."), Shorten.c_str()); break;
	case 164: xprintf(pData, XorString("%s, u such a dickhead"), Shorten.c_str()); break;
	case 165: xprintf(pData, XorString("why use cheat, %s ?"), Shorten.c_str()); break;
	case 166: xprintf(pData, XorString("u using siktir hook, %s ?"), Shorten.c_str()); break;
	case 167: xprintf(pData, XorString("%s, go die in Dust 2"), Shorten.c_str()); break;
	case 168: xprintf(pData, XorString("i hope ur mom dies in Dust 2, %s"), Shorten.c_str()); break;
	case 169: xprintf(pData, XorString("go die in inferno, %s"), Shorten.c_str()); break;
	case 170: xprintf(pData, XorString("u left-handed, %s ?"), Shorten.c_str()); break;
	case 171: xprintf(pData, XorString("%s, u traded ur mum for Hack?"), Shorten.c_str()); break;
	case 172: xprintf(pData, XorString("%s, trade ur sister for my hack?"), Shorten.c_str()); break;
	case 173: xprintf(pData, XorString("can i touch ur sister, %s ?"), Shorten.c_str()); break;
	case 174: xprintf(pData, XorString("i want ur sister, %s"), Shorten.c_str()); break;
	case 175: xprintf(pData, XorString("what u ate today, %s ?"), Shorten.c_str()); break;
	case 176: xprintf(pData, XorString("du svart, %s ?"), Shorten.c_str()); break;
	case 177: xprintf(pData, XorString("har du svart kuk, %s ?"), Shorten.c_str()); break;
	case 178: xprintf(pData, XorString("skrika som en gris, %s !"), Shorten.c_str()); break;
	case 179: xprintf(pData, XorString("du fett gris, %s .."), Shorten.c_str()); break;
	case 180: xprintf(pData, XorString("u tried anal sex, %s ?"), Shorten.c_str()); break;
	case 181: xprintf(pData, XorString("Är du vit/ svart, %s ?"), Shorten.c_str()); break;
	case 182: xprintf(pData, XorString("har du vit rumpa, %s ?"), Shorten.c_str()); break;
	case 183: xprintf(pData, XorString("u like juicy cocks, %s ?"), Shorten.c_str()); break;
	case 184: xprintf(pData, XorString("%s, u horny yet?"), Shorten.c_str()); break;
	case 185: xprintf(pData, XorString("i piss on ur family, %s"), Shorten.c_str()); break;
	case 186: xprintf(pData, XorString("%s, i piss in ur mouth"), Shorten.c_str()); break;
	case 187: xprintf(pData, XorString("%s, u fkn slave"), Shorten.c_str()); break;
	case 188: xprintf(pData, XorString("u were in Egypt, %s ?"), Shorten.c_str()); break;
	case 189: xprintf(pData, XorString("u using condom, %s ?"), Shorten.c_str()); break;
	case 190: xprintf(pData, XorString("u like to lick, %s ?"), Shorten.c_str()); break;
	case 191: xprintf(pData, XorString("how long ur Vein, %s ?"), Shorten.c_str()); break;
	case 192: xprintf(pData, XorString("ai vână lungă, %s ?"), Shorten.c_str()); break;
	case 193: xprintf(pData, XorString("u got acne, %s ?"), Shorten.c_str()); break;
	case 194: xprintf(pData, XorString("%s, is ur mom pregnant ?"), Shorten.c_str()); break;
	case 195: xprintf(pData, XorString("%s, ur mom takes it Anal?"), Shorten.c_str()); break;
	case 196: xprintf(pData, XorString("u swallow it, %s ?"), Shorten.c_str()); break;
	case 197: xprintf(pData, XorString("u swallow or spit the cum, %s ?"), Shorten.c_str()); break;
	case 198: xprintf(pData, XorString("can i see ur dick, %s ?"), Shorten.c_str()); break;
	case 199: xprintf(pData, XorString("u know Kendama, %s ?"), Shorten.c_str()); break;
	case 200: xprintf(pData, XorString("te îneci când sugi pula, %s ?"), Shorten.c_str()); break;
	case 201: xprintf(pData, XorString("muști pula cu dinții, %s ?"), Shorten.c_str()); break;
	case 202: xprintf(pData, XorString("har du kissat idag, %s ?"), Shorten.c_str()); break;
	case 203: xprintf(pData, XorString("u brushed ur teeth today, %s ?"), Shorten.c_str()); break;
	case 204: xprintf(pData, XorString("is ur mom horny, %s ?"), Shorten.c_str()); break;
	case 205: xprintf(pData, XorString("Kuruyordu nehir, %s ?"), Shorten.c_str()); break;
	case 206: xprintf(pData, XorString("kasvetli kutlama, %s ?"), Shorten.c_str()); break;
	case 207: xprintf(pData, XorString("jag vill spräcker ditt anus, %s"), Shorten.c_str()); break;
	case 208: xprintf(pData, XorString("%s, u take it anal for $5?"), Shorten.c_str()); break;
	case 209: xprintf(pData, XorString("u sad, %s ?"), Shorten.c_str()); break;
	case 210: xprintf(pData, XorString("now im dead meat, %s"), Shorten.c_str()); break;
	case 211: xprintf(pData, XorString("+rep for +rep, %s"), Shorten.c_str()); break;
	case 212: xprintf(pData, XorString("%s, +rep for +rep ?"), Shorten.c_str()); break;
	case 213: xprintf(pData, XorString("commend for commend, %s ?"), Shorten.c_str()); break;
	case 214: xprintf(pData, XorString("%s, commend for commend ?"), Shorten.c_str()); break;
	case 215: xprintf(pData, XorString("%s, u like anal Sex?"), Shorten.c_str()); break;
	case 216: xprintf(pData, XorString("%s, is ur ass cute?"), Shorten.c_str()); break;
	case 217: xprintf(pData, XorString("u got cute ass, %s ?"), Shorten.c_str()); break;
	case 218: xprintf(pData, XorString("u got bubble butt, %s ?"), Shorten.c_str()); break;
	case 219: xprintf(pData, XorString("how long is ur dick, %s ?"), Shorten.c_str()); break;
	case 220: xprintf(pData, XorString("enjoying hard porn, %s ?"), Shorten.c_str()); break;
	case 221: xprintf(pData, XorString("%s, stop downloading gay porn"), Shorten.c_str()); break;
	case 222: xprintf(pData, XorString("%s, nice ping man"), Shorten.c_str()); break;
	case 223: xprintf(pData, XorString("%s, can u lag moar pls?"), Shorten.c_str()); break;
	case 224: xprintf(pData, XorString("why u lagging, %s ?"), Shorten.c_str()); break;
	case 225: xprintf(pData, XorString("%s, u like creampie?"), Shorten.c_str()); break;
	case 226: xprintf(pData, XorString("%s, u downloading porn?"), Shorten.c_str()); break;
	case 227: xprintf(pData, XorString("u like porn films, %s ?"), Shorten.c_str()); break;
	case 228: xprintf(pData, XorString("last time u fucked yourself, %s ?"), Shorten.c_str()); break;
	case 229: xprintf(pData, XorString("bel cazzo, %s"), Shorten.c_str()); break;
	case 230: xprintf(pData, XorString("%s, hai un bel cazzo man"), Shorten.c_str()); break;
	case 231: xprintf(pData, XorString("scheiße, %s"), Shorten.c_str()); break;
	case 232: xprintf(pData, XorString("%s, ist dein schwanz lang?"), Shorten.c_str()); break;
	case 233: xprintf(pData, XorString("ich fick deine mutter, %s"), Shorten.c_str()); break;
	case 234: xprintf(pData, XorString("ЦЫГАНСКАЯ СТРАСТЬ, %s ?"), Shorten.c_str()); break;
	case 235: xprintf(pData, XorString("ТЫ ЦЫГАН, %s ?"), Shorten.c_str()); break;
	case 236: xprintf(pData, XorString("%s, ЦЫГАН ?"), Shorten.c_str()); break;
	case 237: xprintf(pData, XorString("%s, ТЫ РОГАТЫЙ?"), Shorten.c_str()); break;
	case 238: xprintf(pData, XorString("ТЫ РОГАТЫЙ, %s ?"), Shorten.c_str()); break;
	case 239: xprintf(pData, XorString("ТЫ ЕШЬ СПЕРМУ, %s ?"), Shorten.c_str()); break;
	case 240: xprintf(pData, XorString("%s, ТЫ ПЬЕШЬ СПЕРМУ?"), Shorten.c_str()); break;
	case 241: xprintf(pData, XorString("reported %s"), Shorten.c_str()); break;
	case 242: xprintf(pData, XorString("all report %s"), Shorten.c_str()); break;
	case 243: xprintf(pData, XorString("5 reports for %s"), Shorten.c_str()); break;
	case 244: xprintf(pData, XorString("%s, u reported"), Shorten.c_str()); break;
	case 245: xprintf(pData, XorString("%s, u got reported m8"), Shorten.c_str()); break;
	case 246: xprintf(pData, XorString("%s, -acc"), Shorten.c_str()); break;
	case 247: xprintf(pData, XorString("%s, check ur acc in 2 hr"), Shorten.c_str()); break;
	case 248: xprintf(pData, XorString("%s, bb acc"), Shorten.c_str()); break;
	case 249: xprintf(pData, XorString("bb acc, %s"), Shorten.c_str()); break;
	case 250: xprintf(pData, XorString("RIP %s"), Shorten.c_str()); break;
	case 251: xprintf(pData, XorString("sooner or later u get banned, %s"), Shorten.c_str()); break;
	case 252: xprintf(pData, XorString("w8 VAC, %s"), Shorten.c_str()); break;
	case 253: xprintf(pData, XorString("u will get OW, %s"), Shorten.c_str()); break;
	case 254: xprintf(pData, XorString("OW will ban u %s"), Shorten.c_str()); break;
	case 255: xprintf(pData, XorString("w8 OW ban, %s"), Shorten.c_str()); break;
	case 256: xprintf(pData, XorString("n1 hack %s"), Shorten.c_str()); break;
	case 257: xprintf(pData, XorString("%s is hacking"), Shorten.c_str()); break;
	case 258: xprintf(pData, XorString("Robertto: %s is hacking"), Shorten.c_str()); break;
	case 259: xprintf(pData, XorString("Mike: %s has obv WH"), Shorten.c_str()); break;
	case 260: xprintf(pData, XorString("ur last match, %s"), Shorten.c_str()); break;
	case 261: xprintf(pData, XorString("pis off, %s"), Shorten.c_str()); break;
	case 262: xprintf(pData, XorString("futu-ți glijania mă-tii, %s"), Shorten.c_str()); break;
	case 263: xprintf(pData, XorString("îmi bei coaiele, %s"), Shorten.c_str()); break;
	case 264: xprintf(pData, XorString("îmi bag pula în mă-ta, %s"), Shorten.c_str()); break;
	case 265: xprintf(pData, XorString("futu-ți pizda mă-tii, %s"), Shorten.c_str()); break;
	case 266: xprintf(pData, XorString("îmi bei pula, %s"), Shorten.c_str()); break;
	case 267: xprintf(pData, XorString("%s, îmi fac schiuri din crucea mă-tii"), Shorten.c_str()); break;
	case 268: xprintf(pData, XorString("îmi fac skiuri din crucia mă-tii, %s"), Shorten.c_str()); break;
	case 269: xprintf(pData, XorString("să moară tac-tu pe scară, %s"), Shorten.c_str()); break;
	case 270: xprintf(pData, XorString("să-ți fut mama în aripă, %s"), Shorten.c_str()); break;
	case 271: xprintf(pData, XorString("să mă lingi sub coadă, %s"), Shorten.c_str()); break;
	case 272: xprintf(pData, XorString("%s, îmi sugi pula cu mă-ta?"), Shorten.c_str()); break;
	case 273: xprintf(pData, XorString("te bag de unde ai ieșit, %s"), Shorten.c_str()); break;
	case 274: xprintf(pData, XorString("să moară familia ta, %s"), Shorten.c_str()); break;
	case 275: xprintf(pData, XorString("%s, das kleine krokodil?"), Shorten.c_str()); break;
	case 276: xprintf(pData, XorString("ți-am dat viol, %s?"), Shorten.c_str()); break;
	case 277: xprintf(pData, XorString("ești curat la cur, %s ?"), Shorten.c_str()); break;
	case 278: xprintf(pData, XorString("câte pule au spart-o pe mă-ta, %s ?"), Shorten.c_str()); break;
	case 279: xprintf(pData, XorString("%s, u play poker?"), Shorten.c_str()); break;
	case 280: xprintf(pData, XorString("Panda: %s is hacking hard.."), Shorten.c_str()); break;
	case 281: xprintf(pData, XorString("Pasha: %s has trigger"), Shorten.c_str()); break;
	case 282: xprintf(pData, XorString("%s is triggering hard"), Shorten.c_str()); break;
	case 283: xprintf(pData, XorString("%s, HS Only?"), Shorten.c_str()); break;
	case 284: xprintf(pData, XorString("only 1 tap, %s ?"), Shorten.c_str()); break;
	case 285: xprintf(pData, XorString("u like 1 taps, %s ?"), Shorten.c_str()); break;
	case 286: xprintf(pData, XorString("%s, îmi bag pula în soacră-ta"), Shorten.c_str()); break;
	case 287: xprintf(pData, XorString("n1 trigger, %s"), Shorten.c_str()); break;
	case 288: xprintf(pData, XorString("%s, u using wallhack?"), Shorten.c_str()); break;
	case 289: xprintf(pData, XorString("nice radarhack, %s"), Shorten.c_str()); break;
	case 290: xprintf(pData, XorString("using EZfrags, %s ?"), Shorten.c_str()); break;
	case 291: xprintf(pData, XorString("u know Aimware, %s ?"), Shorten.c_str()); break;
	case 292: xprintf(pData, XorString("u sell hek, %s ?"), Shorten.c_str()); break;
	case 293: xprintf(pData, XorString("%s, add me for buy hack"), Shorten.c_str()); break;
	case 294: xprintf(pData, XorString("why u added me, %s ?"), Shorten.c_str()); break;
	case 295: xprintf(pData, XorString("%s, why added me on Steam?"), Shorten.c_str()); break;
	case 296: xprintf(pData, XorString("%s, stop adding me.."), Shorten.c_str()); break;
	case 297: xprintf(pData, XorString("%s, i hate u"), Shorten.c_str()); break;
	case 298: xprintf(pData, XorString("du König, %s ?"), Shorten.c_str()); break;
	case 299: xprintf(pData, XorString("w2lekum salam, %s ?"), Shorten.c_str()); break;
	case 300: xprintf(pData, XorString("SA, %s"), Shorten.c_str()); break;
	case 301: xprintf(pData, XorString("salam-alaikum, %s"), Shorten.c_str()); break;
	case 302: xprintf(pData, XorString("u pray to buddha, %s ?"), Shorten.c_str()); break;
	case 303: xprintf(pData, XorString("u like Buddha, %s ?"), Shorten.c_str()); break;
	case 304: xprintf(pData, XorString("whats ur religion, %s ?"), Shorten.c_str()); break;
	case 305: xprintf(pData, XorString("where u work, %s ?"), Shorten.c_str()); break;
	case 306: xprintf(pData, XorString("u work at brazzers, %s ?"), Shorten.c_str()); break;
	case 307: xprintf(pData, XorString("u porn actor, %s ?"), Shorten.c_str()); break;
	case 308: xprintf(pData, XorString("%s, ur mom = porn actress?"), Shorten.c_str()); break;
	case 309: xprintf(pData, XorString("%s, u like to watch Porn?"), Shorten.c_str()); break;
	case 310: xprintf(pData, XorString("%s, u lesbian or gay?"), Shorten.c_str()); break;
	case 311: xprintf(pData, XorString("%s, how many dicks u tasted?"), Shorten.c_str()); break;
	case 312: xprintf(pData, XorString("fuck off.. %s"), Shorten.c_str()); break;
	case 313: xprintf(pData, XorString("ПОКОЙСЯ С МИРОМ, %s"), Shorten.c_str()); break;
	case 314: xprintf(pData, XorString("%s, ПОКОЙСЯ С МИРОМ!"), Shorten.c_str()); break;
	case 315: xprintf(pData, XorString("%s, ЧЕЛОВЕК КОТОРЫЙ РОЖДАЕТСЯ РАЗ В ЖИЗНИ.."), Shorten.c_str()); break;
	case 316: xprintf(pData, XorString("ПЕРЕДАЮЩАЯ РЕАЛИЮ ЧЕРЕЗ ЭМОЦИЮ, %s ?"), Shorten.c_str()); break;
	case 317: xprintf(pData, XorString("НИКТО НЕ ВЕЧЕН, %s.."), Shorten.c_str()); break;
	case 318: xprintf(pData, XorString("%s, НИКТО НЕ ВЕЧЕН.."), Shorten.c_str()); break;
	case 319: xprintf(pData, XorString("УНИКАЛЕН И УНИВЕРСАЛЕН, %s?"), Shorten.c_str()); break;
	case 320: xprintf(pData, XorString("%s, УНИКАЛЕН И УНИВЕРСАЛЕН.."), Shorten.c_str()); break;
	case 321: xprintf(pData, XorString("%s ЖЖЁТ !!"), Shorten.c_str()); break;
	case 322: xprintf(pData, XorString("БОЛЬШЕ И БОЛЬШЕ, %s.."), Shorten.c_str()); break;
	case 323: xprintf(pData, XorString("ДЕБИЛЫ ОЧЕВИДНО, %s ?"), Shorten.c_str()); break;
	case 324: xprintf(pData, XorString("A ЧЕЛОВЕК ТО КАКОЙ БЫЛ, %s.."), Shorten.c_str()); break;
	case 325: xprintf(pData, XorString("A %s МОЙ КУМИР!"), Shorten.c_str()); break;
	case 326: xprintf(pData, XorString("ШИПОВАННЫЕ КОЛЕСА, %s ?"), Shorten.c_str()); break;
	case 327: xprintf(pData, XorString("%s, НАСТРОЕНИЕ ПОДНИМАЕТ!"), Shorten.c_str()); break;
	case 328: xprintf(pData, XorString("НАСТРОЕНИЕ ПОДНИМАЕТ, %s"), Shorten.c_str()); break;
	case 329: xprintf(pData, XorString("ОТЛИЧНО, %s!"), Shorten.c_str()); break;
	case 330: xprintf(pData, XorString("%s, ОТЛИЧНО!"), Shorten.c_str()); break;
	case 331: xprintf(pData, XorString("МОЛОДЕЦ, %s !"), Shorten.c_str()); break;
	case 332: xprintf(pData, XorString("%s, МОЛОДЕЦ!"), Shorten.c_str()); break;
	case 333: xprintf(pData, XorString("ВЕСЁЛАЯ РАБОТА %s, ПОНРАВИЛОСЬ!"), Shorten.c_str()); break;
	case 334: xprintf(pData, XorString("ВЕСЁЛАЯ РАБОТА %s!"), Shorten.c_str()); break;
	case 335: xprintf(pData, XorString("%s, ВЕСЁЛАЯ РАБОТА!"), Shorten.c_str()); break;
	case 336: xprintf(pData, XorString("КОНЕЧНО МНЕ ПОНРАВИЛОСЬ, %s"), Shorten.c_str()); break;
	case 337: xprintf(pData, XorString("%s, КОНЕЧНО МНЕ ПОНРАВИЛОСЬ!"), Shorten.c_str()); break;
	case 338: xprintf(pData, XorString("%s, НУ ДА, КОНЕЧНО"), Shorten.c_str()); break;
	case 339: xprintf(pData, XorString("НУ ДА, КОНЕЧНО, %s"), Shorten.c_str()); break;
	case 340: xprintf(pData, XorString("%s, ВНЕЗАПНО, ДА?"), Shorten.c_str()); break;
	case 341: xprintf(pData, XorString("КНИГИ ЧИТАТЬ НУЖНО, %s.."), Shorten.c_str()); break;
	case 342: xprintf(pData, XorString("%s, КНИГИ ЧИТАТЬ НУЖНО.."), Shorten.c_str()); break;
	case 343: xprintf(pData, XorString("Yarı melek, yarı canavar, %s ?"), Shorten.c_str()); break;
	case 344: xprintf(pData, XorString("%s, yarı melek, yarı canavar?"), Shorten.c_str()); break;
	case 345: xprintf(pData, XorString("bir anda, %s ?"), Shorten.c_str()); break;
	case 346: xprintf(pData, XorString("%s, bir anda.. ?"), Shorten.c_str()); break;
	case 347: xprintf(pData, XorString("ve yine %s.."), Shorten.c_str()); break;
	case 348: xprintf(pData, XorString("Hadi kazı mezarını, %s"), Shorten.c_str()); break;
	case 349: xprintf(pData, XorString("%s, hadi kazı mezarını"), Shorten.c_str()); break;
	case 350: xprintf(pData, XorString("Ahmaklar uluyor, %s.."), Shorten.c_str()); break;
	case 351: xprintf(pData, XorString("%s, ahmaklar uluyor"), Shorten.c_str()); break;
	case 352: xprintf(pData, XorString("Acıyı hisset, %s!"), Shorten.c_str()); break;
	case 353: xprintf(pData, XorString("%s, acıyı hisset!"), Shorten.c_str()); break;
	case 354: xprintf(pData, XorString("reportbotted %s"), Shorten.c_str()); break;
	case 355: xprintf(pData, XorString("reportbot for %s"), Shorten.c_str()); break;
	case 356: xprintf(pData, XorString("report for %s submitted"), Shorten.c_str()); break;
	case 357: xprintf(pData, XorString("pls report %s for hack"), Shorten.c_str()); break;
	case 358: xprintf(pData, XorString("stop griefing %s"), Shorten.c_str()); break;
	case 359: xprintf(pData, XorString("go kick %s pls"), Shorten.c_str()); break;
	case 360: xprintf(pData, XorString("kick %s"), Shorten.c_str()); break;
	case 361: xprintf(pData, XorString("!ban %s"), Shorten.c_str()); break;
	case 362: xprintf(pData, XorString("!kick %s Hack"), Shorten.c_str()); break;
	case 363: xprintf(pData, XorString("!ban %s Hacks"), Shorten.c_str()); break;
	case 364: xprintf(pData, XorString("!slay %s"), Shorten.c_str()); break;
	case 365: xprintf(pData, XorString("%s, tanz mit mir?"), Shorten.c_str()); break;
	case 366: xprintf(pData, XorString("tanz mit mir, %s ?"), Shorten.c_str()); break;
	case 367: xprintf(pData, XorString("%s, tanz mit mir den galgentanz?"), Shorten.c_str()); break;
	case 368: xprintf(pData, XorString("%s, ТЫ СУКА?"), Shorten.c_str()); break;
	case 369: xprintf(pData, XorString("ТЫ СУКА, %s ?"), Shorten.c_str()); break;
	case 370: xprintf(pData, XorString("%s, КТО СУКА?"), Shorten.c_str()); break;
	case 371: xprintf(pData, XorString("КТО СУКА, %s ?"), Shorten.c_str()); break;
	case 372: xprintf(pData, XorString("КТО ШЛЮХА, %s ?"), Shorten.c_str()); break;
	case 373: xprintf(pData, XorString("ТЫ ШЛЮХА, %s ?"), Shorten.c_str()); break;
	case 374: xprintf(pData, XorString("%s, КТО ШЛЮХА?"), Shorten.c_str()); break;
	case 375: xprintf(pData, XorString("%s, ТЫ ШЛЮХА ?"), Shorten.c_str()); break;
	case 376: xprintf(pData, XorString("%s, КАК ШЛЮХА.."), Shorten.c_str()); break;
	case 377: xprintf(pData, XorString("%s, u manager @brazzers?"), Shorten.c_str()); break;
	case 378: xprintf(pData, XorString("%s, u boss at brazzers?"), Shorten.c_str()); break;
	case 379: xprintf(pData, XorString("%s, u moderator at brazzers?"), Shorten.c_str()); break;
	case 380: xprintf(pData, XorString("%s, i saw ur mom @livejasmin man"), Shorten.c_str()); break;
	case 381: xprintf(pData, XorString("%s, mby add me for buy hack?"), Shorten.c_str()); break;
	case 382: xprintf(pData, XorString("%s, ХУЙ ЗНАЕШЬ?"), Shorten.c_str()); break;
	case 383: xprintf(pData, XorString("ХУЙ ЗНАЕШЬ, %s ?"), Shorten.c_str()); break;
	case 384: xprintf(pData, XorString("%s, u like horses?"), Shorten.c_str()); break;
	case 385: xprintf(pData, XorString("%s, ur mom fucks hard?"), Shorten.c_str()); break;
	case 386: xprintf(pData, XorString("ur mom fucks hard, %s ?"), Shorten.c_str()); break;
	case 387: xprintf(pData, XorString("LOL, %s"), Shorten.c_str()); break;
	case 388: xprintf(pData, XorString("%s, LOL"), Shorten.c_str()); break;
	case 389: xprintf(pData, XorString("why hack, %s ?"), Shorten.c_str()); break;
	case 390: xprintf(pData, XorString("%s, БОЛЬШОЙ ПЕНИС?"), Shorten.c_str()); break;
	case 391: xprintf(pData, XorString("hay allah, %s"), Shorten.c_str()); break;
	case 392: xprintf(pData, XorString("%, hay allah.."), Shorten.c_str()); break;
	case 393: xprintf(pData, XorString("%s, i hate u man"), Shorten.c_str()); break;
	case 394: xprintf(pData, XorString("%s, u hacking since 2013?"), Shorten.c_str()); break;
	case 395: xprintf(pData, XorString("u hacking since 2013, %s ?"), Shorten.c_str()); break;
	case 396: xprintf(pData, XorString("%s, idiot cunt?"), Shorten.c_str()); break;
	case 397: xprintf(pData, XorString("ТЫ ИДИОТ, %s ?"), Shorten.c_str()); break;
	case 398: xprintf(pData, XorString("%s, ТЫ ИДИОТ!"), Shorten.c_str()); break;
	case 399: xprintf(pData, XorString("%s, u fucking WORM"), Shorten.c_str()); break;
	case 400: xprintf(pData, XorString("%s, u fkn RAT"), Shorten.c_str()); break;
	case 401: xprintf(pData, XorString("%s, how are you?"), Shorten.c_str()); break;
	case 402: xprintf(pData, XorString("how are you, %s ?"), Shorten.c_str()); break;
	case 403: xprintf(pData, XorString("u from Sweden, %s ?"), Shorten.c_str()); break;
	case 404: xprintf(pData, XorString("u know Japanese, %s ?"), Shorten.c_str()); break;
	case 405: xprintf(pData, XorString("using hentaiware, %s ?"), Shorten.c_str()); break;
	case 406: xprintf(pData, XorString("%s, u using HookTronic?"), Shorten.c_str()); break;
	case 407: xprintf(pData, XorString("%s, u on Interwebz?"), Shorten.c_str()); break;
	case 408: xprintf(pData, XorString("using x22, %s ?"), Shorten.c_str()); break;
	case 409: xprintf(pData, XorString("%s, u using skins changer?"), Shorten.c_str()); break;
	case 410: xprintf(pData, XorString("%s, add me for send Hack"), Shorten.c_str()); break;
	case 411: xprintf(pData, XorString("%s, give me link for Hack pls?"), Shorten.c_str()); break;
	case 412: xprintf(pData, XorString("Är du svart, %s ?"), Shorten.c_str()); break;
	case 413: xprintf(pData, XorString("Är din kuk svart, %s ?"), Shorten.c_str()); break;
	case 414: xprintf(pData, XorString("Är din kuk fett, %s ?"), Shorten.c_str()); break;
	case 415: xprintf(pData, XorString("Är din kuk söt, %s ?"), Shorten.c_str()); break;
	case 416: xprintf(pData, XorString("Är din kuk stor, %s ?"), Shorten.c_str()); break;
	case 417: xprintf(pData, XorString("Gillar du stora svarta rumpor, %s ?"), Shorten.c_str()); break;
	case 418: xprintf(pData, XorString("Gillar du stora vita rumpor, %s ?"), Shorten.c_str()); break;
	case 419: xprintf(pData, XorString("jag vill ditt anus, %s"), Shorten.c_str()); break;
	case 420: xprintf(pData, XorString("%s, jag vill spräcker ditt anus för $5"), Shorten.c_str()); break;
	case 421: xprintf(pData, XorString("%s, jag vill spräcker ditt anus på Skype™"), Shorten.c_str()); break;
	case 422: xprintf(pData, XorString("%s, jag vill spräcker ditt anus på Steam™"), Shorten.c_str()); break;
	case 423: xprintf(pData, XorString("JAG ❤ DIG, %s"), Shorten.c_str()); break;
	case 424: xprintf(pData, XorString("%s, add me to send u SkinChanger?"), Shorten.c_str()); break;
	case 425: xprintf(pData, XorString("%s, u legit man?"), Shorten.c_str()); break;
	case 426: xprintf(pData, XorString("%s, u visited Norway?"), Shorten.c_str()); break;
	case 427: xprintf(pData, XorString("u from Moscow, %s ?"), Shorten.c_str()); break;
	case 428: xprintf(pData, XorString("u like Shakira, %s ?"), Shorten.c_str()); break;
	case 429: xprintf(pData, XorString("u wanna fuck Rihanna, %s ?"), Shorten.c_str()); break;
	case 430: xprintf(pData, XorString("god natt, %s"), Shorten.c_str()); break;
	case 431: xprintf(pData, XorString("god Jul, %s"), Shorten.c_str()); break;
	case 432: xprintf(pData, XorString("%s, remember me?"), Shorten.c_str()); break;
	case 433: xprintf(pData, XorString("%s, i think i fucked ur mom last week?"), Shorten.c_str()); break;
	case 434: xprintf(pData, XorString("%s, i think i know ur sister.."), Shorten.c_str()); break;
	case 435: xprintf(pData, XorString("u was at WC today, %s ?"), Shorten.c_str()); break;
	case 436: xprintf(pData, XorString("ești șters bine la cur, %s ?"), Shorten.c_str()); break;
	case 437: xprintf(pData, XorString("te-ai șters bine la cur, %s ?"), Shorten.c_str()); break;
	case 438: xprintf(pData, XorString("mă fut pe cripta mă-tii, %s"), Shorten.c_str()); break;
	case 439: xprintf(pData, XorString("mă cac pe mormântu' mă-tii, %s"), Shorten.c_str()); break;
	case 440: xprintf(pData, XorString("să-ți iau altaru' mă-tii în pulă, %s"), Shorten.c_str()); break;
	case 441: xprintf(pData, XorString("a tras mă-ta apa la WC azi, %s ?"), Shorten.c_str()); break;
	case 442: xprintf(pData, XorString("%s, tragi apa la WC când te caci ?"), Shorten.c_str()); break;
	case 443: xprintf(pData, XorString("bei pula des, %s ?"), Shorten.c_str()); break;
	case 444: xprintf(pData, XorString("%s, preferi labă sau deget ?"), Shorten.c_str()); break;
	case 445: xprintf(pData, XorString("faci des inserții anale, %s ?"), Shorten.c_str()); break;
	case 446: xprintf(pData, XorString("e mă-ta pe ciclu azi, %s ?"), Shorten.c_str()); break;
	case 447: xprintf(pData, XorString("când e pe ciclu mă-ta, %s ?"), Shorten.c_str()); break;
	case 448: xprintf(pData, XorString("îmi bag pl în rapița mă-tii, %s"), Shorten.c_str()); break;
	case 449: xprintf(pData, XorString("sugi pl cu mă-ta, %s!"), Shorten.c_str()); break;
	case 450: xprintf(pData, XorString("să-mi belești pl la Cluj, %s"), Shorten.c_str()); break;
	case 451: xprintf(pData, XorString("is ur mom pregnant, %s ?"), Shorten.c_str()); break;
	case 452: xprintf(pData, XorString("%s, is ur mom pregnant ?"), Shorten.c_str()); break;
	case 453: xprintf(pData, XorString("ur mom fucked while pregnant, %s ?"), Shorten.c_str()); break;
	case 454: xprintf(pData, XorString("%s, ur mum fucked while pregnant ?"), Shorten.c_str()); break;
	case 455: xprintf(pData, XorString("have u got big dick, %s ?"), Shorten.c_str()); break;
	case 456: xprintf(pData, XorString("%s, nice man so brave!"), Shorten.c_str()); break;
	case 457: xprintf(pData, XorString("so brave %s, i like it"), Shorten.c_str()); break;
	case 458: xprintf(pData, XorString("u will get ban, %s"), Shorten.c_str()); break;
	case 459: xprintf(pData, XorString("își face mă-ta des clisme, %s ?"), Shorten.c_str()); break;
	case 460: xprintf(pData, XorString("te-a futut Pomohaci, %s ?"), Shorten.c_str()); break;
	case 461: xprintf(pData, XorString("am auzit că dă tare Pomohaci, așa %s ?"), Shorten.c_str()); break;
	case 462: xprintf(pData, XorString("îmi bag pl în sarcofagu' mă-tii, %s"), Shorten.c_str()); break;
	case 463: xprintf(pData, XorString("u ever fucked ur mom, %s ?"), Shorten.c_str()); break;
	case 464: xprintf(pData, XorString("%s, u tried sex with ur mum ?"), Shorten.c_str()); break;
	case 465: xprintf(pData, XorString("o padrinho, %s ?"), Shorten.c_str()); break;
	case 466: xprintf(pData, XorString("%s, o padrinho man ?"), Shorten.c_str()); break;
	case 467: xprintf(pData, XorString("îți iau în pl vezica mă-tii, %s"), Shorten.c_str()); break;
	case 468: xprintf(pData, XorString("are coaie mari tac-tu, %s ?"), Shorten.c_str()); break;
	case 469: xprintf(pData, XorString("futu-ți suspinu' mă-tii să-ți fut, %s"), Shorten.c_str()); break;
	case 470: xprintf(pData, XorString("%s, u can't cheat in secured servers"), Shorten.c_str()); break;
	case 471: xprintf(pData, XorString("%s, u can't cheat on secured servers"), Shorten.c_str()); break;
	case 472: xprintf(pData, XorString("%s, cry here -___-"), Shorten.c_str()); break;
	case 473: xprintf(pData, XorString("cry here -____-, %s"), Shorten.c_str()); break;
	case 474: xprintf(pData, XorString("%s, u crying ?"), Shorten.c_str()); break;
	case 475: xprintf(pData, XorString("%s, it's only a game.."), Shorten.c_str()); break;
	case 476: xprintf(pData, XorString("%s, why u have to be mad ?"), Shorten.c_str()); break;
	case 477: xprintf(pData, XorString("what is ur mom doing right now, %s ?"), Shorten.c_str()); break;
	case 478: xprintf(pData, XorString("%s, ОК ОК.."), Shorten.c_str()); break;
	case 479: xprintf(pData, XorString("ОК ОК, %s.."), Shorten.c_str()); break;
	case 480: xprintf(pData, XorString("%s, ДАЙ АК?"), Shorten.c_str()); break;
	case 481: xprintf(pData, XorString("ДАЙ АК, %s?"), Shorten.c_str()); break;
	case 482: xprintf(pData, XorString("%s, ДАЙ АВП?"), Shorten.c_str()); break;
	case 483: xprintf(pData, XorString("ДАЙ АВП, %s?"), Shorten.c_str()); break;
	case 484: xprintf(pData, XorString("stop griefing, %s"), Shorten.c_str()); break;
	case 485: xprintf(pData, XorString("kick %s for griefing"), Shorten.c_str()); break;
	case 486: xprintf(pData, XorString("%s, reported for griefing man"), Shorten.c_str()); break;
	case 487: xprintf(pData, XorString("%s, cry will not help you win"), Shorten.c_str()); break;
	case 488: xprintf(pData, XorString("%s, may the God be with u man"), Shorten.c_str()); break;
	case 489: xprintf(pData, XorString("may the god be with u, %s"), Shorten.c_str()); break;
	case 490: xprintf(pData, XorString("%s, illegal scheißegal?"), Shorten.c_str()); break;
	case 491: xprintf(pData, XorString("scheißegal, %s?"), Shorten.c_str()); break;
	case 492: xprintf(pData, XorString("EZ, %s?"), Shorten.c_str()); break;
	case 493: xprintf(pData, XorString("%s, so EZ.."), Shorten.c_str()); break;
	case 494: xprintf(pData, XorString("%s, EZ WIN?"), Shorten.c_str()); break;
	case 495: xprintf(pData, XorString("EZ taps, %s?"), Shorten.c_str()); break;
	case 496: xprintf(pData, XorString("u like alcohol, %s?"), Shorten.c_str()); break;
	case 497: xprintf(pData, XorString("u alcoholic, %s?"), Shorten.c_str()); break;
	case 498: xprintf(pData, XorString("%s, u alcoholic?"), Shorten.c_str()); break;
	case 499: xprintf(pData, XorString("%s, u like CAMPARİ®?"), Shorten.c_str()); break;
	case 500: xprintf(pData, XorString("u drinking CAMPARİ®, %s ?"), Shorten.c_str()); break;
	case 501: xprintf(pData, XorString("u like NEMİROFF® VODKA, %s ?"), Shorten.c_str()); break;
	case 502: xprintf(pData, XorString("%s, u drink NEMİROFF® VODKA?"), Shorten.c_str()); break;
	case 503: xprintf(pData, XorString("u like RedBull®, %s ?"), Shorten.c_str()); break;
	case 504: xprintf(pData, XorString("%s, u drinking RedBull®?"), Shorten.c_str()); break;
	case 505: xprintf(pData, XorString("one-tapped, %s ?"), Shorten.c_str()); break;
	case 506: xprintf(pData, XorString("%s, u like Marlboro® cigs?"), Shorten.c_str()); break;
	case 507: xprintf(pData, XorString("u like Marlboro®, %s ?"), Shorten.c_str()); break;
	case 508: xprintf(pData, XorString("u from USA, %s ?"), Shorten.c_str()); break;
	case 509: xprintf(pData, XorString("u like Serbia, %s ?"), Shorten.c_str()); break;
	case 510: xprintf(pData, XorString("no way, %s"), Shorten.c_str()); break;
	case 511: xprintf(pData, XorString("%s, np man.."), Shorten.c_str()); break;
	case 512: xprintf(pData, XorString("%s, give me your kik ID"), Shorten.c_str()); break;
	case 513: xprintf(pData, XorString("what's your kik id, %s ?"), Shorten.c_str()); break;
	case 514: xprintf(pData, XorString("nice hack %s, where you found it?"), Shorten.c_str()); break;
	case 515: xprintf(pData, XorString("from where you have ur hack, %s?"), Shorten.c_str()); break;
	case 516: xprintf(pData, XorString("nice hack %s, where you found?"), Shorten.c_str()); break;
	case 517: xprintf(pData, XorString("i think %s is cheating"), Shorten.c_str()); break;
	case 518: xprintf(pData, XorString("%s, are you cheating man?"), Shorten.c_str()); break;
	case 519: xprintf(pData, XorString("%s, u cheat?"), Shorten.c_str()); break;
	case 520: xprintf(pData, XorString("ЧИТ ИМЯ, %s?"), Shorten.c_str()); break;
	case 521: xprintf(pData, XorString("%s, ЧИТ ИМЯ?"), Shorten.c_str()); break;
	case 522: xprintf(pData, XorString("%s, ДАЙ ЧИТ ИМЯ?"), Shorten.c_str()); break;
	case 523: xprintf(pData, XorString("ДАЙ ЧИТ ИМЯ, %s ?"), Shorten.c_str()); break;
	case 524: xprintf(pData, XorString("ТВОЙ АНУС, %s?"), Shorten.c_str()); break;
	case 525: xprintf(pData, XorString("%s, ТВОЙ АНУС?"), Shorten.c_str()); break;
	case 526: xprintf(pData, XorString("ДАЙ ТВОЙ АНУС, %s?"), Shorten.c_str()); break;
	case 527: xprintf(pData, XorString("%s, ДАЙ ТВОЙ АНУС?"), Shorten.c_str()); break;
	case 528: xprintf(pData, XorString("ПРОСТЕР АНУС, %s?"), Shorten.c_str()); break;
	case 529: xprintf(pData, XorString("%s, ПРОСТЕР АНУС?"), Shorten.c_str()); break;
	case 530: xprintf(pData, XorString("ТВОЯ МАМА, %s"), Shorten.c_str()); break;
	case 531: xprintf(pData, XorString("%s, ТВОЯ МАМА"), Shorten.c_str()); break;
	case 532: xprintf(pData, XorString("ДАЙ ТВОЯ МАМА, %s?"), Shorten.c_str()); break;
	case 533: xprintf(pData, XorString("%s, ДАЙ ТВОЯ МАМА?"), Shorten.c_str()); break;
	case 534: xprintf(pData, XorString("%s, i like ur cheat"), Shorten.c_str()); break;
	case 535: xprintf(pData, XorString("%s, i add u for buy hack"), Shorten.c_str()); break;
	case 536: xprintf(pData, XorString("%s, accept me on Steam™"), Shorten.c_str()); break;
	case 537: xprintf(pData, XorString("%s, add me on Skype™ pls"), Shorten.c_str()); break;
	case 538: xprintf(pData, XorString("%s, send me nudes pls?"), Shorten.c_str()); break;
	case 539: xprintf(pData, XorString("%s, what's your Skype™"), Shorten.c_str()); break;
	case 540: xprintf(pData, XorString("what's ur Skype™, %s?"), Shorten.c_str()); break;
	case 541: xprintf(pData, XorString("%s, go add for trade?"), Shorten.c_str()); break;
	case 542: xprintf(pData, XorString("%s, u trade ur sister pls?"), Shorten.c_str()); break;
	case 543: xprintf(pData, XorString("%s, u wh man"), Shorten.c_str()); break;
	case 544: xprintf(pData, XorString("Skeeter: %s has good hack"), Shorten.c_str()); break;
	case 545: xprintf(pData, XorString("eddy: %s has trigger man"), Shorten.c_str()); break;
	};
};

#endif

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

INT FilterClanTag(ImGuiTextEditCallbackData * pData)
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

	if (strlen(Vars.Misc.szClanTag) >= 12)
	{
		pData->EventChar = '\0';
		return 1;
	};

	return 0;
};

VOID SetClanTag(VOID)
{
	static INT iBaseLen = 0, iCurLen = 0, iIter = 0;
	static time_t tStamp = 0, tNow = 0;
	static DWORD dwAddr = {};
	static bool bSignature = false;
	static auto pSetClanTag = reinterpret_cast <VOID(__fastcall *) (CONST CHAR *, CONST CHAR *)> (NULL);

	if (!bSignature)
	{
		dwAddr = FindPattern(XorString("engine.dll"), XorString("53 56 57 8B DA 8B F9 FF 15"));
		if (dwAddr) pSetClanTag = reinterpret_cast <VOID(__fastcall *) (CONST CHAR *, CONST CHAR *)> (dwAddr);
		bSignature = true;
	};

	if (!pSetClanTag)
		return;

	tNow = time(NULL);

	if (tNow - tStamp > 0)
	{
		iBaseLen = strlen(Vars.Misc.szClanTag), \
			iCurLen = strlen(g_szFormattedClanTag);

		if (iBaseLen < 1)
		{
			tStamp = tNow;
			return;
		};

		if (iBaseLen > 12)
			Vars.Misc.szClanTag[12] = '\0', iBaseLen = strlen(Vars.Misc.szClanTag);

		if (iCurLen >= iBaseLen || !iCurLen)
		{
			g_szFormattedClanTag[0] = Vars.Misc.szClanTag[0], \
				g_szFormattedClanTag[1] = '\0';

			pSetClanTag(Vars.Misc.bClanTagRainbow ? g_szFormattedClanTag : Vars.Misc.szClanTag, XorString(CLAN_TAG)), \
				tStamp = tNow;

			return;
		};

		g_szFormattedClanTag[iCurLen] = Vars.Misc.szClanTag[iCurLen], \
			g_szFormattedClanTag[iCurLen + 1] = '\0';

		pSetClanTag(Vars.Misc.bClanTagRainbow ? g_szFormattedClanTag : Vars.Misc.szClanTag, XorString(CLAN_TAG)), \
			tStamp = tNow;
	};
};

#endif

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

VOID WINAPI OnPlaySound(CONST CHAR * pszSoundName)
{
	static bool bReceived = false;
	static DWORD dwAddr = NULL;
	static IsReadyCallBackFn Fn = NULL;

	oPlaySound(pszSoundName);

	if (g_bOffline && Vars.Misc.bAutoAccept && pszSoundName && *pszSoundName && !strcmp(pszSoundName, XorString("!UI/competitive_accept_beep.wav")))
	{
		if (!bReceived)
		{
			dwAddr = FindPattern(XorString("client.dll"), XorString("55 8B EC 83 E4 F8 83 EC 08 56 8B 35 ? ? ? ? 57 83 BE"));
			if (dwAddr) Fn = (IsReadyCallBackFn)dwAddr;
			bReceived = true;
		};

		if (Fn)
		{
			if (GetForegroundWindow() != g_pWnd)
				ShowWindow(g_pWnd, SW_RESTORE), Sleep(XS_RANDOM_SIZE * .5f), SetActiveWindow(g_pWnd);

			Fn();
		};
	};
};

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

VOID WINAPI OnOverrideView(CViewSetup * pViewSetup)
{
	static bool bPistol = false;

	oOverrideView(pViewSetup);

	if (G::bAlive && G::bGun && !G::bScoped)
	{
		bPistol = IS_PISTOL(G::iWeapId);

		if (!bPistol && Vars.Misc.bFOV) pViewSetup->fov = (FLOAT)Vars.Misc.iFOV;
		else if (bPistol && Vars.Misc.bFOVPistols) pViewSetup->fov = (FLOAT)Vars.Misc.iFOVPistols;
	};
};

FLOAT WINAPI OnGetViewModelFoV(VOID)
{
	static bool bPistol = false;
	static FLOAT fFoV = 0.f;

	fFoV = oGetViewModelFoV();

	if (G::bAlive && G::bGun && !G::bScoped)
	{
		bPistol = IS_PISTOL(G::iWeapId);

		if (!bPistol && Vars.Misc.bFOV) fFoV = (FLOAT)Vars.Misc.iViewModelFOV;
		else if (bPistol && Vars.Misc.bFOVPistols) fFoV = (FLOAT)Vars.Misc.iViewModelFOVPistols;
	};

	return fFoV;
};

#endif

bool TopZone(bool bRage, Entity_T * pEntity, Vector & vecHBox, CONST FLOAT fMaxFoV, PFLOAT pFoV)
{
	static INT iID = 0, iIter = 0, iHBoxSet = 0;
	static FLOAT fFoV = 0.f, fTopFoV = 0.f;
	static GameTrace_T Trace;
	static QAngle angAim;
	static Matrix34_T mMatrix[128];
	static Vector vecMin, vecMax, vecLocations[9], vecFwd, vecPositions[9], vecTopHBox;
	static StdHdr_T * pHdr = NULL;
	static StdioBBox_T * pUntransfBox = NULL;
	static Model_T * pModel = NULL;
	static StdHBoxSet_T * pHBoxSet = NULL;

	fTopFoV = 8192.f;

	if (pEntity->SetupBones(mMatrix) && (pModel = pEntity->GetModel()) && (pHdr = I::MDLInfo->GetStudioModel(pModel)) && (iHBoxSet = pEntity->GetHBoxSet()) >= 0 && (pHBoxSet = pHdr->pHBoxSet(iHBoxSet)))
	{
		if (bRage)
		{
			if (G::iWeapId != WEAPON_AWP && G::iWeapId != WEAPON_SSG08)
			{
				if ((pUntransfBox = pHBoxSet->pHBox(HITBOX_HEAD)))
				{
					vecMin = pUntransfBox->m_vecMin, vecMax = pUntransfBox->m_vecMax;

					if (pUntransfBox->m_fRadius != -1.f && pUntransfBox->m_fRadius != 0.f)
						vecMin -= Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale), \
						vecMax += Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale);

					vecLocations[0] = (vecMin + vecMax) * .5f, \
						vecLocations[1] = Vector(vecMin.x, vecMin.y, vecMin.z), \
						vecLocations[2] = Vector(vecMin.x, vecMax.y, vecMin.z), \
						vecLocations[3] = Vector(vecMax.x, vecMax.y, vecMin.z), \
						vecLocations[4] = Vector(vecMax.x, vecMin.y, vecMin.z), \
						vecLocations[5] = Vector(vecMax.x, vecMax.y, vecMax.z), \
						vecLocations[6] = Vector(vecMin.x, vecMax.y, vecMax.z), \
						vecLocations[7] = Vector(vecMin.x, vecMin.y, vecMax.z), \
						vecLocations[8] = Vector(vecMax.x, vecMin.y, vecMax.z);

					for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
					{
						if (iID) vecLocations[iID] = (((vecLocations[iID] + vecLocations[0]) * .5f) + vecLocations[iID]) * .5f;
						VecTransf(vecLocations[iID], mMatrix[pUntransfBox->m_iBone], vecPositions[iID]);
						angAim = CalcAng(G::vecSelfEyePos, vecPositions[iID]), fFoV = FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim);
						if (fFoV > fMaxFoV) continue;
						AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
						if (!Trace.ent || !Trace.hitgroup) continue;
						if (fFoV < fTopFoV) fTopFoV = fFoV, vecTopHBox = vecPositions[iID];
					};

					if (fTopFoV <= fMaxFoV)
					{
						vecHBox = vecTopHBox;
						if (pFoV) *pFoV = fTopFoV;
						return true;
					};
				};
			}

			else
			{
				for (iIter = 0; iIter < ARRAYSIZE(g_nHitBoxes); iIter++)
				{
					if ((pUntransfBox = pHBoxSet->pHBox(g_nHitBoxes[iIter])))
					{
						vecMin = pUntransfBox->m_vecMin, vecMax = pUntransfBox->m_vecMax;

						if (pUntransfBox->m_fRadius != -1.f && pUntransfBox->m_fRadius != 0.f)
							vecMin -= Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale), \
							vecMax += Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale);

						vecLocations[0] = (vecMin + vecMax) * .5f, \
							vecLocations[1] = Vector(vecMin.x, vecMin.y, vecMin.z), \
							vecLocations[2] = Vector(vecMin.x, vecMax.y, vecMin.z), \
							vecLocations[3] = Vector(vecMax.x, vecMax.y, vecMin.z), \
							vecLocations[4] = Vector(vecMax.x, vecMin.y, vecMin.z), \
							vecLocations[5] = Vector(vecMax.x, vecMax.y, vecMax.z), \
							vecLocations[6] = Vector(vecMin.x, vecMax.y, vecMax.z), \
							vecLocations[7] = Vector(vecMin.x, vecMin.y, vecMax.z), \
							vecLocations[8] = Vector(vecMax.x, vecMin.y, vecMax.z);

						for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
						{
							if (iID) vecLocations[iID] = (((vecLocations[iID] + vecLocations[0]) * .5f) + vecLocations[iID]) * .5f;
							VecTransf(vecLocations[iID], mMatrix[pUntransfBox->m_iBone], vecPositions[iID]);
							angAim = CalcAng(G::vecSelfEyePos, vecPositions[iID]), fFoV = FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim);
							if (fFoV > fMaxFoV) continue;
							AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
							if (!Trace.ent || !Trace.hitgroup) continue;
							if (fFoV < fTopFoV) fTopFoV = fFoV, vecTopHBox = vecPositions[iID];
						};
					};
				};

				if (fTopFoV <= fMaxFoV)
				{
					vecHBox = vecTopHBox;
					if (pFoV) *pFoV = fTopFoV;
					return true;
				};
			};
		}

		else
		{
			for (iIter = 0; iIter < ARRAYSIZE(g_nHitBoxes); iIter++)
			{
				if ((pUntransfBox = pHBoxSet->pHBox(g_nHitBoxes[iIter])))
				{
					vecMin = pUntransfBox->m_vecMin, vecMax = pUntransfBox->m_vecMax;

					if (pUntransfBox->m_fRadius != -1.f && pUntransfBox->m_fRadius != 0.f)
						vecMin -= Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale), \
						vecMax += Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale);

					vecLocations[0] = (vecMin + vecMax) * .5f, \
						vecLocations[1] = Vector(vecMin.x, vecMin.y, vecMin.z), \
						vecLocations[2] = Vector(vecMin.x, vecMax.y, vecMin.z), \
						vecLocations[3] = Vector(vecMax.x, vecMax.y, vecMin.z), \
						vecLocations[4] = Vector(vecMax.x, vecMin.y, vecMin.z), \
						vecLocations[5] = Vector(vecMax.x, vecMax.y, vecMax.z), \
						vecLocations[6] = Vector(vecMin.x, vecMax.y, vecMax.z), \
						vecLocations[7] = Vector(vecMin.x, vecMin.y, vecMax.z), \
						vecLocations[8] = Vector(vecMax.x, vecMin.y, vecMax.z);

					for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
					{
						if (iID) vecLocations[iID] = (((vecLocations[iID] + vecLocations[0]) * .5f) + vecLocations[iID]) * .5f;
						VecTransf(vecLocations[iID], mMatrix[pUntransfBox->m_iBone], vecPositions[iID]);
						angAim = CalcAng(G::vecSelfEyePos, vecPositions[iID]), fFoV = FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim);
						if (fFoV > fMaxFoV) continue;
						AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
						if (!Trace.ent || !Trace.hitgroup) continue;
						if (fFoV < fTopFoV) fTopFoV = fFoV, vecTopHBox = vecPositions[iID];
					};
				};
			};

			if (fTopFoV <= fMaxFoV)
			{
				vecHBox = vecTopHBox;
				if (pFoV) *pFoV = fTopFoV;
				return true;
			};
		};
	};

	return false;
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

FLOAT CanAutoWall(Vector vecStart, Vector vecEnd, WeaponInfo_T * pData)
{
	static GameTrace_T Trace;
	static INT iDummy = 0, iHitsLeft = 0, iMaterial = 0;
	static Vector vecResult, vecNormal;
	static FLOAT fDamage = 0.f;
	static SurfData_T * pSurf = NULL;
	static DWORD dwAddr = 0;
	static bool bChecked = false;
	static bool(__thiscall * HandleBulletsFn) (Entity_T *, FLOAT &, INT &, INT *, GameTrace_T *, Vector *, FLOAT, FLOAT, FLOAT, INT, INT, FLOAT, INT *, Vector *, FLOAT, FLOAT, FLOAT *) = NULL;

	if (!bChecked)
	{
		dwAddr = FindPattern(XorString("client.dll"), XorString("53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 78 8B 53 14"));
		if (dwAddr) HandleBulletsFn = (bool(__thiscall *) (Entity_T *, FLOAT &, INT &, INT *, GameTrace_T *, Vector *, FLOAT, FLOAT, FLOAT, INT, INT, FLOAT, INT *, Vector *, FLOAT, FLOAT, FLOAT *)) dwAddr;
		bChecked = true;
	};

	if (!HandleBulletsFn) return 0.f;
	vecResult = vecStart, vecNormal = vecEnd, iHitsLeft = 4, iDummy = 0, fDamage = pData->iDamage, vecNormal.Normalize();
	do
	{
		TraceLine(vecResult, vecEnd, MASK_SHOT, G::pSelf, &Trace);
		if (Trace.ent && Trace.hitgroup) break;
		pSurf = I::PhysProps->GetSurfaceData(Trace.surface.surfaceProps), iMaterial = pSurf->game.material, g_bShouldMask = true;
		if (HandleBulletsFn(G::pSelf, pData->flPenetration, iMaterial, &iDummy, &Trace, &vecNormal, 0.f, pSurf->game.flPenetrationModifier, pSurf->game.flDamageModifier, 0, 0x00001002, pData->flPenetration, &iHitsLeft, &vecResult, 0.f, 0.f, &fDamage))
		{
			g_bShouldMask = false; break;
		}
		else g_bShouldMask = false;
	} while (iHitsLeft > 0);

	fDamage *= powf(pData->flRangeModifier, (vecStart - vecResult).Length() * .002f);
	if (IS_PISTOL(G::iWeapId) && fDamage >= Vars.RageBOT.iMinDamagePistols && Trace.ent && Trace.hitgroup && Trace.ent->GetTeam() != G::iTeam) return fDamage;
	else if (IS_SNIPER(G::iWeapId) && fDamage >= Vars.RageBOT.iMinDamageSnipers && Trace.ent && Trace.hitgroup && Trace.ent->GetTeam() != G::iTeam) return fDamage;
	return fDamage >= Vars.RageBOT.iMinDamageRifles && Trace.ent && Trace.hitgroup && Trace.ent->GetTeam() != G::iTeam ? fDamage : 0.f;
};

bool TopZoneHeavyRage(Entity_T * pEntity, Vector & vecHBox, CONST FLOAT fMaxFoV, PFLOAT pDamage, WeaponInfo_T * pData)
{
	static CONST Vector vecZ = Vector(0.f, 0.f, 1.f);
	static INT iID = 0, iHBoxSet = 0;
	static FLOAT fDamage = 0.f, fTopDamage = 0.f, fPitch = 0.f, fDistance = 0.f;
	static GameTrace_T Trace;
	static QAngle angAim;
	static Matrix34_T mMatrix[128];
	static Vector vecMin, vecMax, vecLocations[9], vecFwd, vecPositions[5], vecTopHBox, vecPos, vecA, vecB;
	static StdHdr_T * pHdr = NULL;
	static StdioBBox_T * pUntransfBox = NULL;
	static Model_T * pModel = NULL;
	static StdHBoxSet_T * pHBoxSet = NULL;
	fTopDamage = 0.f;
	if (pEntity->SetupBones(mMatrix) && (pModel = pEntity->GetModel()) && (pHdr = I::MDLInfo->GetStudioModel(pModel)) && (iHBoxSet = pEntity->GetHBoxSet()) >= 0 && (pHBoxSet = pHdr->pHBoxSet(iHBoxSet)))
	{
		if ((pUntransfBox = pHBoxSet->pHBox(HITBOX_HEAD)))
		{
			fDistance = (pEntity->GetOrigin() - G::vecSelfEyePos).Length(), fPitch = pEntity->GetEyeAng().x, vecMin = pUntransfBox->m_vecMin, vecMax = pUntransfBox->m_vecMax;
			if (pUntransfBox->m_fRadius != -1.f && pUntransfBox->m_fRadius != 0.f)
				vecMin -= Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale), \
				vecMax += Vector(pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale, pUntransfBox->m_fRadius * Vars.LEGITBOT.AimHelpers.fHitPointsScale);
			vecLocations[0] = (vecMin + vecMax) * .5f, \
				vecLocations[1] = Vector(vecMin.x, vecMin.y, vecMin.z), \
				vecLocations[2] = Vector(vecMin.x, vecMax.y, vecMin.z), \
				vecLocations[3] = Vector(vecMax.x, vecMax.y, vecMin.z), \
				vecLocations[4] = Vector(vecMax.x, vecMin.y, vecMin.z), \
				vecLocations[5] = Vector(vecMax.x, vecMax.y, vecMax.z), \
				vecLocations[6] = Vector(vecMin.x, vecMax.y, vecMax.z), \
				vecLocations[7] = Vector(vecMin.x, vecMin.y, vecMax.z), \
				vecLocations[8] = Vector(vecMax.x, vecMin.y, vecMax.z);
			for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
			{
				if (iID) vecLocations[iID] = (((vecLocations[iID] + vecLocations[0]) * .5f) + vecLocations[iID]) * .5f;
				VecTransf(vecLocations[iID], mMatrix[pUntransfBox->m_iBone], vecPositions[iID]);
			};
			for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
			{
				angAim = CalcAng(G::vecSelfEyePos, vecPositions[iID]);
				if (FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim) > fMaxFoV) continue;
				AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), vecFwd *= pData->flRange, TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd, MASK_SHOT, G::pSelf, &Trace);
				if (Trace.ent && Trace.hitgroup)
				{
					vecHBox = vecPositions[iID];
					if (pDamage) *pDamage = 8192.f - fDistance;
					return true;
				};
			};
			if ((fPitch > 0.f && fPitch <= 89.f) || (fPitch < 292.5f && fPitch >= 271.f))
			{
				if (fPitch > 0.f && fPitch <= 89.f) vecA = (vecPositions[3] + vecPositions[5]) * .5f, vecB = ((vecA - vecPositions[0]) / 3.f) * 4.f, vecPos = vecPositions[0] + (vecB * (fPitch / 89.f));
				else vecPos = vecPositions[0] - vecZ;
				angAim = CalcAng(G::vecSelfEyePos, vecPos);
				if (FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim) <= fMaxFoV)
				{
					AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), vecFwd *= pData->flRange, TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd, MASK_SHOT, G::pSelf, &Trace);
					if (Trace.ent && Trace.hitgroup)
					{
						vecHBox = vecPos;
						if (pDamage) *pDamage = 8192.f - fDistance;
						return true;
					}
					if ((fDamage = CanAutoWall(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd, pData)) > 0.f)
					{
						vecHBox = vecPos;
						if (pDamage) *pDamage = (fDamage * 2.f) / fDistance;
						return true;
					};
				};
			};
			for (iID = 0; iID < ARRAYSIZE(vecLocations); iID++)
			{
				angAim = CalcAng(G::vecSelfEyePos, vecPositions[iID]);
				if (FoV(G::pCmd->viewang + G::angSelfPunch * 2.f, angAim) > fMaxFoV) continue;
				AngVec(angAim + G::angSelfPunch * 2.f, &vecFwd), vecFwd *= pData->flRange, TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd, MASK_SHOT, G::pSelf, &Trace);
				if (Trace.ent && Trace.hitgroup)
				{
					vecHBox = vecPositions[iID];
					if (pDamage) *pDamage = 8192.f - fDistance;
					return true;
				}
				if (vecPositions[iID].z >= vecPositions[0].z && (fDamage = CanAutoWall(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd, pData)) > fTopDamage)
				{
					fTopDamage = fDamage, vecTopHBox = vecPositions[iID];
					if (vecPositions[iID].z > vecPositions[0].z) fTopDamage *= 2.f;
				};
			};
			if (fTopDamage > 0.f)
			{
				vecHBox = vecTopHBox;
				if (pDamage) *pDamage = fTopDamage / fDistance;
				return true;
			};
		};
	};
	return false;
};

#endif

#endif

Color ClrByOption(INT Option)
{
	switch (Option)
	{
	case 1: return g_WhiteSmoke;
	case 2: return g_Grey;
	case 3: return g_DarkGrey;
	case 4: return g_Black;
	case 5: return g_Red;
	case 6: return g_DarkRed;
	case 7: return g_CandyRed;
	case 8: return g_Green;
	case 9: return g_DarkGreen;
	case 10: return g_CandyGreen;
	case 11: return g_Blue;
	case 12: return g_DarkBlue;
	case 13: return g_CandyBlue;
	case 14: return g_Violet;
	case 15: return g_Yellow;
	case 16: return g_Magenta;
	case 17: return g_Cyan;
	case 18: return g_Orange;
	};

	return g_White;
};

VOID MakeFonts(VOID)
{
	static INT iLastTall = 0;
	if (iLastTall != Vars.Misc.iFontHeight) g_nStyle = I::Surf->AddFont(), I::Surf->SetFontGlyphSet(g_nStyle, XorString("MS Sans Serif"), Vars.Misc.iFontHeight, 500, NULL, NULL, 0x00000200 | 0x00000010, NULL, NULL), iLastTall = Vars.Misc.iFontHeight;
};

VOID DrawStr(INT X, INT Y, Color _Clr, bool bCentered, CONST string Data)
{
	static WCHAR szWideBuffer[XS_RANDOM_SIZE];
	static INT iW = 0, iH = 0;

	MultiByteToWideChar(CP_UTF8, NULL, Data.c_str(), ARRAYSIZE(szWideBuffer) / 2, szWideBuffer, ARRAYSIZE(szWideBuffer) / 2);
	I::Surf->GetTextSize(g_nStyle, szWideBuffer, iW, iH);
	I::Surf->DrawSetTextFont(g_nStyle);
	I::Surf->DrawSetTextColor(_Clr);
	I::Surf->DrawSetTextPos(bCentered ? (X - iW / 2) : (X), Y - iH / 2);
	I::Surf->DrawPrintText(szWideBuffer, wcslen(szWideBuffer));
};

VOID DrawLine(INT X0, INT Y0, INT X1, INT Y1, Color _Clr)
{
	I::Surf->DrawSetColor(_Clr), I::Surf->DrawLine(X0, Y0, X1, Y1);
};

VOID DrawCircle(FLOAT fX, FLOAT fY, FLOAT fRadius, FLOAT fSegments, Color _Clr)
{
	static FLOAT fStep = 0.f, fIter = 0.f;

	fStep = M_PI_F * 2.f / fSegments;

	for (fIter = 0.f; fIter < M_PI_F * 2.f; fIter += fStep)
		DrawLine(fRadius * cosf(fIter) + fX, fRadius * sinf(fIter) + fY, fRadius * cosf(fIter + fStep) + fX, fRadius * sinf(fIter + fStep) + fY, _Clr);
};

bool WTS(CONST Vector vecOrigin, Vector & vecScreen)
{
	static Matrix44_T Matrix;
	static INT SWidth = 0, SHeight = 0;
	static FLOAT fX = 0.f, fY = 0.f, fRes = 0.f, fTemp = 0.f;

	Matrix = I::Engine->WorldToScreenMatrix();

	vecScreen.x = Matrix[0][0] * vecOrigin.x + Matrix[0][1] * vecOrigin.y + Matrix[0][2] * vecOrigin.z + Matrix[0][3], \
		vecScreen.y = Matrix[1][0] * vecOrigin.x + Matrix[1][1] * vecOrigin.y + Matrix[1][2] * vecOrigin.z + Matrix[1][3], \
		fRes = Matrix[3][0] * vecOrigin.x + Matrix[3][1] * vecOrigin.y + Matrix[3][2] * vecOrigin.z + Matrix[3][3], vecScreen.z = 0.f;

	if (fRes >= .001f)
	{
		fTemp = 1.f / fRes, vecScreen.x *= fTemp, vecScreen.y *= fTemp, I::Engine->GetScreenSize(SWidth, SHeight), fX = (FLOAT)(SWidth / 2), \
			fY = (FLOAT)(SHeight / 2), fX += .5f * vecScreen.x * FLOAT(SWidth) + .5f, fY -= .5f * vecScreen.y * FLOAT(SHeight) + .5f, vecScreen.x = fX, vecScreen.y = fY;

		return true;
	};

	return false;
};

VOID SinCos(FLOAT fRad, PFLOAT pSin, PFLOAT pCos)
{
	*pSin = sinf(fRad), *pCos = cosf(fRad);
};

VOID VecAng(CONST Vector vecFwd, QAngle & angAng)
{
	if (vecFwd.y == 0.f && vecFwd.x == 0.f) angAng.x = (vecFwd.z > 0.f) ? 270.f : 90.f, angAng.y = 0.f;
	else
	{
		angAng.x = atan2f(-vecFwd.z, vecFwd.Length2D()) * -180.f / M_PI_F, angAng.y = atan2f(vecFwd.y, vecFwd.x) * 180.f / M_PI_F;
		if (angAng.y > 90.f) angAng.y -= 180.f;
		else if (angAng.y < 90.f) angAng.y += 180.f;
		else if (angAng.y == 90.f) angAng.y = 0.f;
	};
	angAng.z = 0.f;
};

VOID AngVec(CONST QAngle angAng, Vector * vecFwd)
{
	static FLOAT fSp, fSy, fCp, fCy;
	SinCos(DEG2RAD(angAng.y), &fSy, &fCy), SinCos(DEG2RAD(angAng.x), &fSp, &fCp);
	vecFwd->x = fCp * fCy, vecFwd->y = fCp * fSy, vecFwd->z = -fSp;
};

FLOAT FoV(CONST QAngle angViewAng, CONST QAngle angAimAng)
{
	static Vector vecAng, vecAim;
	AngVec(angViewAng, &vecAim), AngVec(angAimAng, &vecAng);
	return RAD2DEG(acosf(vecAim.Dot(vecAng) / vecAim.LengthSqr()));
};

QAngle CalcAng(CONST Vector Src, CONST Vector Dst)
{
	static QAngle angRes;
	VecAng(Src - Dst, angRes);
	return angRes;
};

VOID VecTransf(CONST Vector A, CONST Matrix34_T B, Vector & Output)
{
	Output.x = A.Dot(B.m_pArray[0]) + B.m_pArray[0][3], Output.y = A.Dot(B.m_pArray[1]) + B.m_pArray[1][3], Output.z = A.Dot(B.m_pArray[2]) + B.m_pArray[2][3];
};

ClientClass * IBaseClientDll::GetAllClasses(VOID)
{
	typedef ClientClass * (__thiscall * OriginalFn) (PVOID);
	return GetVFunc < OriginalFn >(this, 8) (this);
};

CONST CHAR * IVPanel::GetName(INT iIndex)
{
	typedef CONST CHAR * (__thiscall * __GetNameFn) (PVOID, INT);
	return GetVFunc < __GetNameFn >(this, 36) (this, iIndex);
};

VOID ISurface::DrawSetColor(Color Col)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, Color);
	return GetVFunc < OriginalFn >(this, 14) (this, Col);
};

VOID ISurface::DrawSetTextFont(ULONG nStyle)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, ULONG);
	return GetVFunc < OriginalFn >(this, 23) (this, nStyle);
};

VOID ISurface::DrawSetTextColor(Color Col)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, Color);
	return GetVFunc < OriginalFn >(this, 24) (this, Col);
};

VOID ISurface::DrawSetTextPos(INT X, INT Y)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, INT, INT);
	return GetVFunc < OriginalFn >(this, 26) (this, X, Y);
};

VOID ISurface::DrawLine(INT X0, INT Y0, INT X1, INT Y1)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, INT, INT, INT, INT);
	return GetVFunc < OriginalFn >(this, 19) (this, X0, Y0, X1, Y1);
};

VOID ISurface::DrawPrintText(CONST WCHAR * pBuffer, INT iLen)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, CONST WCHAR *, INT, INT);
	return GetVFunc < OriginalFn >(this, 28) (this, pBuffer, iLen, NULL);
};

VOID ISurface::DrawSetTexture(INT iId)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, INT);
	return GetVFunc < OriginalFn >(this, 38) (this, iId);
};

ULONG ISurface::AddFont(VOID)
{
	typedef ULONG(__thiscall * OriginalFn) (PVOID);
	return GetVFunc < OriginalFn >(this, 71) (this);
};

bool ISurface::SetFontGlyphSet(ULONG nStyle, CONST CHAR * pszName, INT iTall, INT iWeight, INT iBlur, INT iScanLines, INT iFlags, INT nRangeMin, INT nRangeMax)
{
	typedef bool(__thiscall * OriginalFn) (PVOID, ULONG, CONST CHAR *, INT, INT, INT, INT, INT, INT, INT);
	return GetVFunc < OriginalFn >(this, 72) (this, nStyle, pszName, iTall, iWeight, iBlur, iScanLines, iFlags, nRangeMin, nRangeMax);
};

VOID ISurface::GetTextSize(ULONG nStyle, CONST WCHAR * pszBuffer, INT & iWide, INT & iTall)
{
	typedef VOID(__thiscall * OriginalFn) (PVOID, ULONG, CONST WCHAR *, INT &, INT &);
	return GetVFunc < OriginalFn >(this, 79) (this, nStyle, pszBuffer, iWide, iTall);
};

VOID CNetVarMan::Run(VOID)
{
	m_vecTables.clear();
	ClientClass * pClass = I::Client->GetAllClasses();
	while (pClass)
	{
		RecvTable_T * pTable = pClass->m_pRecvTable;
		m_vecTables.push_back(pTable), pClass = pClass->m_pNext;
	};
};

INT CNetVarMan::GetOffset(CONST CHAR * pszTable, CONST CHAR * pszProp)
{
	return Get_Prop(pszTable, pszProp, NULL);
};

INT CNetVarMan::Get_Prop(CONST CHAR * pszTable, CONST CHAR * pszProp, RecvProp_T ** ppProp)
{
	RecvTable_T * pTable = GetTable(pszTable);
	if (!pTable) return 0;
	return Get_Prop(pTable, pszProp, ppProp);
};

INT CNetVarMan::Get_Prop(RecvTable_T * pTable, CONST CHAR * pszProp, RecvProp_T ** ppProp)
{
	INT iExtraOffs = 0;
	for (INT iIter = 0; iIter < pTable->m_nProps; iIter++)
	{
		RecvProp_T * pProp = &pTable->m_pProps[iIter];
		RecvTable_T * pChildTable = pProp->m_pDataTable;
		if (pChildTable && pChildTable->m_nProps > 0)
		{
			INT iTemp = Get_Prop(pChildTable, pszProp, ppProp);
			if (iTemp) iExtraOffs += (pProp->m_Offset + iTemp);
		};
		if (stricmp(pProp->m_pVarName, pszProp)) continue;
		if (ppProp) *ppProp = pProp;
		return pProp->m_Offset + iExtraOffs;
	};
	return iExtraOffs;
};

RecvTable_T * CNetVarMan::GetTable(CONST CHAR * pszTable)
{
	if (m_vecTables.empty()) return 0;
	for each (RecvTable_T * pTable in m_vecTables)
	{
		if (pTable && stricmp(pTable->m_pNetTableName, pszTable) == 0) return pTable;
	};
	return 0;
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

WeaponInfo_T * Entity_T::GetSpecialData(VOID)
{
	typedef WeaponInfo_T * (__thiscall * OriginalFn) (PVOID);
	return GetVFunc < OriginalFn >(this, 446) (this);
};

FLOAT Entity_T::GetAccuracyPenalty(VOID)
{
	return *(PFLOAT)((DWORD)this + g_Offsets.m_fAccuracyPenalty);
};

#endif

#endif

ICollideable * Entity_T::GetCollideable(VOID)
{
	return (ICollideable *)((DWORD)this + g_Offsets.m_Collision);
};

bool Entity_T::IsSpottingMe(VOID)
{
	static Vector S, D, F;
	static GameTrace_T T;
	static QAngle angEyeAng;
	AngVec((angEyeAng = GetEyeAng()), &F), F *= 8192.f, S = GetEyePos(), TraceLine(S, S + F, MASK_SHOT, this, &T);
	if (T.ent == G::pSelf) return true;
	AngVec(angEyeAng + GetPunch() * 2.f, &F), F *= 8192.f, TraceLine(S, S + F, MASK_SHOT, this, &T);
	return T.ent == G::pSelf;
};

bool Entity_T::IsSpottingMyHead(VOID)
{
	static Vector S, D, F;
	static GameTrace_T T;
	static QAngle angEyeAng;
	AngVec((angEyeAng = GetEyeAng()), &F), F *= 8192.f, S = GetEyePos(), TraceLine(S, S + F, MASK_SHOT, this, &T);
	if (T.ent == G::pSelf && T.hitgroup == HITGROUP_HEAD) return true;
	AngVec(angEyeAng + GetPunch() * 2.f, &F), F *= 8192.f, TraceLine(S, S + F, MASK_SHOT, this, &T);
	return T.ent == G::pSelf && T.hitgroup == HITGROUP_HEAD;
};

VOID Entity_T::SetSpotted(VOID)
{
	*(bool *)((DWORD)this + g_Offsets.m_bSpotted) = true;
};

INT	Entity_T::GetHealth(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_iHealth);
};

INT Entity_T::GetTeam(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_iTeamNum);
};

bool Entity_T::IsScoped(VOID)
{
	return *(bool *)((DWORD)this + g_Offsets.m_bIsScoped);
};

INT Entity_T::GetFlags(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_fFlags);
};

VOID Entity_T::SetFlashMaxAlpha(FLOAT fAmount)
{
	*(PFLOAT)((DWORD)this + g_Offsets.m_flFlashMaxAlpha) = fAmount;
};

INT Entity_T::GetTickBase(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_nTickBase);
};

INT Entity_T::GetHBoxSet(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_nHitboxSet);
};

FLOAT Entity_T::GetBombTimer(VOID)
{
	static FLOAT fBombTime = 0.f, fVal = 0.f;
	fBombTime = *(PFLOAT)((DWORD)this + g_Offsets.m_flC4Blow), fVal = fBombTime - I::Globals->m_fCurTime;
	return fVal < 0.f ? 0.f : fVal;
};

FLOAT Entity_T::GetElasticity(VOID)
{
	return *(PFLOAT)((DWORD)this + g_Offsets.m_flElasticity);
};

bool Entity_T::IsDormant(VOID)
{
	return *(bool *)((DWORD)this + g_Offsets.m_bDormant);
};

bool Entity_T::GetImmune(VOID)
{
	return *(bool *)((DWORD)this + g_Offsets.m_bGunGameImmunity);
};

Model_T * Entity_T::GetModel(VOID)
{
	return *(Model_T **)((DWORD)this + 0x0000006C);
};

bool Entity_T::IsVisible(VOID)
{
	static INT _Id = 0;
	static GameTrace_T _Trace;
	static clock_t _Stamp[XS_RANDOM_SIZE / 2] = { 0 }, _Now = 0;
	static Vector vecPos;
	static Matrix34_T Data[128];

	if (m_iId >= 0 && m_iId < XS_RANDOM_SIZE / 2)
	{
		_Now = clock();

		if (FLOAT(((DOUBLE)_Now - _Stamp[m_iId]) / CLOCKS_PER_SEC) > FLOAT(.05F) && SetupBones(Data))
		{
			m_bVis[m_iId] = false;

			for (_Id = 0; _Id < ARRAYSIZE(Data); _Id++)
			{
				vecPos = Vector(Data[_Id].m_pArray[0][3], Data[_Id].m_pArray[1][3], Data[_Id].m_pArray[2][3]);
				if (vecPos.IsZero()) break;
				TraceLine(G::vecSelfEyePos, vecPos, MASK_SHOT, G::pSelf, &_Trace);
				if (_Trace.ent && _Trace.hitgroup)
				{
					m_bVis[m_iId] = true;
					break;
				};
			};

			_Stamp[m_iId] = _Now;
		};

		return m_bVis[m_iId];
	};

	return false;
};

QAngle Entity_T::GetPunch(VOID)
{
	return *(QAngle *)((DWORD)this + g_Offsets.m_aimPunchAngle);
};

QAngle Entity_T::GetEyeAng(VOID)
{
	return *(QAngle *)((DWORD)this + g_Offsets.m_angEyeAngles);
};

Vector Entity_T::GetOrigin(VOID)
{
	return *(Vector *)((DWORD)this + g_Offsets.m_vecOrigin);
};

Vector Entity_T::GetEyePos(VOID)
{
	return GetOrigin() + *(Vector *)((DWORD)this + g_Offsets.m_vecViewOffset);
};

bool Entity_T::SetupBones(Matrix34_T * pBones)
{
	static FLOAT varTime = 0.f;
	static INT varBones = 0, varMask = 0;

	varTime = I::Globals->m_fCurTime, varBones = 128, varMask = 256;

	__asm
	{
		mov edi, this
		lea ecx, dword ptr ds : [edi + 0x4]
		mov edx, dword ptr ds : [ecx]
		push varTime
		push varMask
		push varBones
		push pBones
		call dword ptr ds : [edx + 0x34]
	};
};

Vector Entity_T::GetVelocity(VOID)
{
	return *(Vector *)((DWORD)this + g_Offsets.m_vecVelocity);
};

Entity_T * Entity_T::GetWeapon(VOID)
{
	return I::Ents->GetClEntity((*(PDWORD)((DWORD)this + g_Offsets.m_hActiveWeapon)) & 0x00000FFF);
};

INT Entity_T::GetMode(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_iObserverMode);
};

Entity_T * Entity_T::GetTarget(VOID)
{
	return GetMode() == 4 ? I::Ents->GetClEntity((*(PDWORD)((DWORD)this + g_Offsets.m_hObserverTarget)) & 0x00000FFF) : nullptr;
};

ClientClass * Entity_T::GetClientClass(VOID)
{
	static PVOID pNetworkable = NULL;
	pNetworkable = (PVOID)((DWORD)this + 0x00000008);
	return GetVFunc < GetClientClassFn >(pNetworkable, 2) (pNetworkable);
};

FLOAT Entity_T::GetNextPrimaryAttack(VOID)
{
	return *(PFLOAT)((DWORD)this + g_Offsets.m_flNextPrimaryAttack);
};

INT Entity_T::GetItemDefinitionIndex(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_iItemDefinitionIndex);
};

bool Entity_T::IsEmpty(VOID)
{
	return *(PINT)((DWORD)this + g_Offsets.m_iClip1) == 0;
};

bool Entity_T::IsDefusing(VOID)
{
	return *(bool *)((DWORD)this + g_Offsets.m_bIsDefusing);
};

bool Entity_T::HasDefuser(VOID)
{
	return *(bool *)((DWORD)this + g_Offsets.m_bHasDefuser);
};

FLOAT Entity_T::GetFlashDuration(VOID)
{
	static FLOAT fDuration = 0.f;
	fDuration = *(PFLOAT)((DWORD)this + g_Offsets.m_flFlashDuration);
	return fDuration < 0.f ? 0.f : fDuration;
};

bool Entity_T::IsGun(INT iId)
{
	switch (iId)
	{
	case WEAPON_DEAGLE: case WEAPON_ELITE: case WEAPON_FIVESEVEN: case WEAPON_GLOCK: case WEAPON_AK47: case WEAPON_AUG: case WEAPON_AWP: case WEAPON_FAMAS:
	case WEAPON_G3SG1: case WEAPON_GALILAR: case WEAPON_M249: case WEAPON_M4A1: case WEAPON_MAC10: case WEAPON_P90: case WEAPON_UMP45: case WEAPON_XM1014:
	case WEAPON_BIZON: case WEAPON_MAG7: case WEAPON_NEGEV: case WEAPON_SAWEDOFF: case WEAPON_TEC9: case WEAPON_HKP2000: case WEAPON_MP7: case WEAPON_MP9:
	case WEAPON_NOVA: case WEAPON_P250: case WEAPON_SCAR20: case WEAPON_SG556: case WEAPON_SSG08: case WEAPON_M4A1_SILENCER: case WEAPON_USP_SILENCER:
	case WEAPON_CZ75A: case WEAPON_REVOLVER: return true;
	};

	return false;
};

string Entity_T::GetPlayerName(VOID)
{
	static PlrInfo_T PlrInfo;
	if (m_iId >= 0 && I::Engine->GetPlayerInfo(m_iId, &PlrInfo) && PlrInfo.name && *PlrInfo.name) return ShortenNameNoRandom(PlrInfo.name);
	return XorString("???");
};

string Entity_T::GetWeaponName(INT iId)
{
	switch (iId)
	{
	case WEAPON_DEAGLE: return XorString("DEAGLE");
	case WEAPON_ELITE: return XorString("ELITES");
	case WEAPON_FIVESEVEN: return XorString("FN57");
	case WEAPON_GLOCK: return XorString("GLOCK");
	case WEAPON_AK47: return XorString("AK47");
	case WEAPON_AUG: return XorString("AUG");
	case WEAPON_AWP: return XorString("AWP");
	case WEAPON_FAMAS: return XorString("FAMAS");
	case WEAPON_G3SG1: return XorString("G3");
	case WEAPON_GALILAR: return XorString("GALIL");
	case WEAPON_M249: return XorString("M249");
	case WEAPON_M4A1: return XorString("M4");
	case WEAPON_MAC10: return XorString("MAC");
	case WEAPON_P90: return XorString("P90");
	case WEAPON_UMP45: return XorString("UMP");
	case WEAPON_XM1014: return XorString("XM");
	case WEAPON_BIZON: return XorString("BIZON");
	case WEAPON_MAG7: return XorString("MAG");
	case WEAPON_NEGEV: return XorString("NEGEV");
	case WEAPON_SAWEDOFF: return XorString("SAWED");
	case WEAPON_TEC9: return XorString("TEC");
	case WEAPON_TASER: return XorString("TASER");
	case WEAPON_HKP2000: return XorString("P2K");
	case WEAPON_MP7: return XorString("MP7");
	case WEAPON_MP9: return XorString("MP9");
	case WEAPON_NOVA: return XorString("NOVA");
	case WEAPON_P250: return XorString("P250");
	case WEAPON_SCAR20: return XorString("SCAR");
	case WEAPON_SG556: return XorString("SG");
	case WEAPON_SSG08: return XorString("SSG");
	case WEAPON_FLASHBANG: return XorString("FLASH");
	case WEAPON_HEGRENADE: return XorString("GRENADE");
	case WEAPON_SMOKEGRENADE: return XorString("SMOKE");
	case WEAPON_MOLOTOV: return XorString("MOLOTOV");
	case WEAPON_DECOY: return XorString("DECOY");
	case WEAPON_INCGRENADE: return XorString("INCENDIARY");
	case WEAPON_C4: return XorString("C4");
	case WEAPON_M4A1_SILENCER: return XorString("M4-S");
	case WEAPON_USP_SILENCER: return XorString("USP-S");
	case WEAPON_CZ75A: return XorString("CZ");
	case WEAPON_REVOLVER: return XorString("R8");
	case WEAPON_KNIFE_BAYONET: return XorString("BAYONET");
	case WEAPON_KNIFE_BUTTERFLY: return XorString("BUTTERFLY");
	case WEAPON_KNIFE_FALCHION: return XorString("FALCHION");
	case WEAPON_KNIFE_FLIP: return XorString("FLIP");
	case WEAPON_KNIFE_GUT: return XorString("GUT");
	case WEAPON_KNIFE_KARAMBIT: return XorString("KARAMBIT");
	case WEAPON_KNIFE_M9_BAYONET: return XorString("M9");
	case WEAPON_KNIFE_PUSH: return XorString("DAGGERS");
	case WEAPON_KNIFE_SURVIVAL_BOWIE: return XorString("BOWIE");
	case WEAPON_KNIFE_TACTICAL: return XorString("HUNTSMAN");
	};

	return XorString("KNIFE");
};

CGrenTraj::CGrenTraj(VOID)
{
	Clear();
};

CGrenTraj::~CGrenTraj(VOID)
{
	Clear();
};

VOID CGrenTraj::AddGrenade(INT grenadeId)
{
	static Vector vecPos;
	static Entity_T * pGrenade = 0;
	static CGrenTrajObj Grenade;
	static ClientClass * pClass = 0;

	if (grenadeId < 0 || IsGrenadeQueued(grenadeId) || !(pGrenade = I::Ents->GetClEntity(grenadeId)) || pGrenade->m_iId < 0 || pGrenade->GetVelocity().Length() < 4.f || !(pClass = pGrenade->GetClientClass()))
		return;

	vecPos = pGrenade->GetOrigin();
	if (vecPos.Length() < 4.f) return;

	Grenade.m_vecPositions.clear();

	Grenade.m_Grenade = grenadeId;
	Grenade.m_lastThinkTime = clock();
	Grenade.m_vecPositions.push_back(vecPos);
	Grenade.m_pClientClass = pClass;

	m_vecGrenades.push_back(Grenade);
};

bool CGrenTraj::IsEntityGrenade(Entity_T * pEntity, ClientClass * pClientClass)
{
	if (!pEntity || pEntity->m_iId < 0 || !pClientClass || (pClientClass->m_ClassID != CBaseCSGrenadeProjectile && pClientClass->m_ClassID != CDecoyProjectile && pClientClass->m_ClassID != CMolotovProjectile && pClientClass->m_ClassID != CSmokeGrenadeProjectile && pClientClass->m_ClassID != CSensorGrenadeProjectile))
		return false;

	return true;
};

VOID CGrenTraj::Work(VOID)
{
	static SIZE_T mIterator = 0, mPosIterator = 0, mPosCount = 0;
	static Entity_T * mGrenadeHandle = 0;
	static Vector mActualPos, mOldPos, mOldScreenPos, mOlderPos, mOlderScreenPos;

	for (mIterator = 0; mIterator < m_vecGrenades.size(); mIterator++)
	{
		mGrenadeHandle = I::Ents->GetClEntity(m_vecGrenades.at(mIterator).m_Grenade);

		if (!mGrenadeHandle || mGrenadeHandle->m_iId < 0 || (mGrenadeHandle->GetVelocity().Length() < 4.f && m_vecGrenades.at(mIterator).m_vecPositions.size() >= 32))
		{
			m_vecGrenades.at(mIterator).m_vecPositions.clear();
			continue;
		};

		mActualPos = mGrenadeHandle->GetOrigin();
		if (mActualPos.Length() < 4.f && m_vecGrenades.at(mIterator).m_vecPositions.size() >= 32)
		{
			m_vecGrenades.at(mIterator).m_vecPositions.clear();
			continue;
		};

		m_vecGrenades.at(mIterator).m_pClientClass = mGrenadeHandle->GetClientClass();
		if (!IsEntityGrenade(mGrenadeHandle, mGrenadeHandle->GetClientClass()))
		{
			m_vecGrenades.at(mIterator).m_vecPositions.clear();
			continue;
		};

		if (FLOAT(((DOUBLE)clock() - m_vecGrenades.at(mIterator).m_lastThinkTime) / CLOCKS_PER_SEC) >= FLOAT(.05f))
		{
			m_vecGrenades.at(mIterator).m_vecPositions.push_back(mActualPos);
			m_vecGrenades.at(mIterator).m_lastThinkTime = clock();
		};

		mPosCount = m_vecGrenades.at(mIterator).m_vecPositions.size();

		for (mPosIterator = 0; mPosIterator < mPosCount; mPosIterator++)
		{
			if (mPosIterator >= (mPosCount - 1)) break;

			mOlderPos = m_vecGrenades.at(mIterator).m_vecPositions.at(mPosIterator);
			mOldPos = m_vecGrenades.at(mIterator).m_vecPositions.at(mPosIterator + 1);

			if (WTS(mOlderPos, mOlderScreenPos) && WTS(mOldPos, mOldScreenPos))
				DrawLine(INT(mOlderScreenPos.x), INT(mOlderScreenPos.y), INT(mOldScreenPos.x), INT(mOldScreenPos.y), g_White);
		};
	};
};

bool CGrenTraj::IsGrenadeQueued(INT grenadeId)
{
	static INT mGrenadesCount = 0, mIterator = 0;

	mGrenadesCount = m_vecGrenades.size();

	if (mGrenadesCount == 0)
		return false;

	for (mIterator = 0; mIterator < mGrenadesCount; mIterator++)
	{
		if (m_vecGrenades.at(mIterator).m_Grenade == grenadeId)
			return true;
	};

	return false;
};

VOID CGrenTraj::Clear(VOID)
{
	m_vecGrenades.clear();
};

bool stristr(CONST CHAR * A, CONST CHAR * B)
{
	static string _A, _B;
	_A = A, _B = B, transform(_A.begin(), _A.end(), _A.begin(), ::tolower), transform(_B.begin(), _B.end(), _B.begin(), ::tolower);
	return strstr(_A.c_str(), _B.c_str()) ? true : false;
};

DWORD FindPattern(CONST string Module, CONST string Pattern)
{
	static Hattrick::ModuleTuple * pModule = NULL;
	static CONST CHAR * pszPattern = NULL;
	static DWORD dwFirst = (DWORD)0, dwStart = (DWORD)0, dwEnd = (DWORD)0, dwCur = (DWORD)0;
	if (!(pModule = g_Hattrick.ProcessModule(Module))) return DWORD(0);
	pszPattern = Pattern.c_str(), dwFirst = (DWORD)0, dwStart = (DWORD)pModule->m_pHandle, dwEnd = dwStart + pModule->m_Info.SizeOfImage;
	for (dwCur = dwStart; dwCur < dwEnd; dwCur++)
	{
		if (*pszPattern == '\0') return dwFirst;
		if (*(PBYTE)pszPattern == '\?' || *(PBYTE)dwCur == GETBYTE(pszPattern))
		{
			if (dwFirst == DWORD(0)) dwFirst = dwCur;
			if (pszPattern[2] == '\0') return dwFirst;
			if (*(PWORD)pszPattern == '\?\?' || *(PBYTE)pszPattern != '\?') pszPattern += 3;
			else pszPattern += 2;
		}
		else pszPattern = Pattern.c_str(), dwFirst = (DWORD)0;
	};
	return NULL;
};

template < typename Type > Type * CaptureInterface(CONST string Module, CONST string Interface)
{
	typedef Type * (*FnType) (CONST CHAR *, INT);
	static Hattrick::ModuleTuple * pModule = NULL;
	if (!(pModule = g_Hattrick.ProcessModule(Module))) return (Type *)NULL;
	FnType pfnAddr = (FnType)pModule->m_pCreateInterfaceAddress;
	if (!pfnAddr) return (Type *)NULL;
	Type * pData = pfnAddr(Interface.c_str(), NULL);
	if (!pData) return (Type *)NULL;
	return (Type *)pData;
};

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

string g_pFakeLagOptions[2] =
{
	XorString("FULL RANDOM"),
	XorString("ASYNCHRONOUS"),
};

#endif

VOID sendConCmd(string Buffer)
{
	I::Engine->ClientCmd_Unrestricted(Buffer.c_str(), NULL);
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

VOID __fastcall OnTraceRay(PVOID pThis, PVOID, CONST Ray_T & Ray, UINT uMask, TraceFilter_T * pFilter, GameTrace_T * pTrace)
{
	oTraceRay(pThis, Ray, uMask, pFilter, pTrace);
	if (g_bShouldMask) pTrace->surface.flags |= 0x00000004;
};

#endif

#endif
