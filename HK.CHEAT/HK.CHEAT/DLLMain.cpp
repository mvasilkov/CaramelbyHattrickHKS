
#include "Misc.h"

DWORD WINAPI runAll(HANDLE)
{
	clock_t menuStamp = clock_t(NULL);
	time_t theTime = NULL, timeStamp = NULL;
	CHAR szData[XS_RANDOM_SIZE] = {};
	INT iPlayer = 0, Players[XS_RANDOM_SIZE / 2] = {}, iSize = 0, iHealth = 0, iEnemiesSize = 0, iEnemy = 0, iTeam = 0;
	Entity_T * pPlayer = NULL, *pEnemy = NULL;
	bool bLineGoesThroughSmokeChecked = false;
	LineGoesThroughSmokeTypeFn fnLGTS = NULL;
	DWORD dwAddr = NULL;
	Vector vecPos;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

	PlrInfo_T PlrInfo;

#endif

	SetupInterfaces(), SetupOffsets(), SetupHooks(), MakeFonts();

	while (true)
	{

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

		if (g_PhrasesSize > ARRAYSIZE(g_Phrases) * .9)
			g_PhrasesSize = ARRAYSIZE(g_Phrases) * .9;

#endif

		if ((G::Keys[VK_HOME] || G::Keys[VK_INSERT] || G::Keys[0x000000FF]) && FLOAT(((DOUBLE)clock() - menuStamp) / CLOCKS_PER_SEC) > .1f)
		{
			Vars.Menu.bOpened = !Vars.Menu.bOpened;
			if (!g_bOffline && G::iFlags & FL_CLIENT && !G::pSelf->GetOrigin().IsZero()) sendConCmd(XorString("toggleconsole"));
			if (!Vars.Menu.bOpened && g_bD3DOK) ImGui::GetIO().MouseDrawCursor = false;
			menuStamp = clock();
		};

		if (!g_bOffline)
		{
			if (G::Keys[VK_END])
				Config->Panic();

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

			if (G::iTeam > 1 && G::iTeam < 4)
				SetClanTag();

#endif

			if (G::bAlive)
			{
				if (!bLineGoesThroughSmokeChecked)
				{
					dwAddr = FindPattern(XorString("client.dll"), XorString("55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0"));
					if (dwAddr) fnLGTS = (LineGoesThroughSmokeTypeFn)dwAddr;
					bLineGoesThroughSmokeChecked = true;
				};

				if (fnLGTS)
				{
					for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
					{
						if (!(pPlayer = I::Ents->GetClEntity(iPlayer)) || pPlayer == G::pSelf || (iTeam = pPlayer->GetTeam()) < 2 || iTeam == G::iTeam || iTeam > 3 || pPlayer->GetHealth() < 1) continue;
						if ((G::iWeapId == WEAPON_AWP || G::iWeapId == WEAPON_SSG08) && G::bScoped) g_bSmokeExcluded[iPlayer] = false;
						else
						{
							vecPos = pPlayer->GetOrigin();
							if (fnLGTS(G::vecSelfEyePos.x, G::vecSelfEyePos.y, G::vecSelfEyePos.z, vecPos.x, vecPos.y, vecPos.z, 1)) g_bSmokeExcluded[iPlayer] = true;
							else g_bSmokeExcluded[iPlayer] = false;
						};
					};
				};
			};

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

			if (Vars.Misc.bSpam && G::iTeam > 1 && G::iTeam < 4)
			{
				if (g_PhrasesSize > 0)
				{
					theTime = time(NULL);

					if (theTime - timeStamp > RandomINT(Vars.Misc.iSpamDelaySeconds, Vars.Misc.iSpamDelaySeconds + Vars.Misc.iSpamDelaySeconds / 2) && theTime - g_timeLastEventStamp > RandomINT(Vars.Misc.iSpamDelaySeconds, Vars.Misc.iSpamDelaySeconds + Vars.Misc.iSpamDelaySeconds / 2))
					{
						if ((g_Phrases[g_PhrasesSize - 1][0] >= 'a' && g_Phrases[g_PhrasesSize - 1][0] <= 'z') || (g_Phrases[g_PhrasesSize - 1][0] >= 'A' && g_Phrases[g_PhrasesSize - 1][0] <= 'Z'))
						{
							if (RandomINT(0, 1) == 0) g_Phrases[g_PhrasesSize - 1][0] = toupper(g_Phrases[g_PhrasesSize - 1][0]);
							else if (xIsCharLower(g_Phrases[g_PhrasesSize - 1][1])) g_Phrases[g_PhrasesSize - 1][0] = tolower(g_Phrases[g_PhrasesSize - 1][0]);
						};

						xprintf(szData, XorString("Say \"%s\";"), g_Phrases[g_PhrasesSize - 1]);
						I::Engine->ClientCmd_Unrestricted(szData, NULL);
						g_PhrasesSize--, timeStamp = theTime;
					};
				}

				else
				{
					theTime = time(NULL);

					if (g_bCheckForLastEnemy && Vars.Misc.bSpamRandomEnemies)
					{
						iEnemiesSize = 0;

						for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
						{
							pPlayer = I::Ents->GetClEntity(iPlayer);
							if (!pPlayer || pPlayer == G::pSelf || (iTeam = pPlayer->GetTeam()) < 2 || iTeam == G::iTeam || iTeam > 3 || pPlayer->GetHealth() < 1) continue;
							iEnemiesSize++, iEnemy = iPlayer;
						};

						if (iEnemiesSize == 1)
						{
							if ((pEnemy = I::Ents->GetClEntity(iEnemy)) && (iHealth = pEnemy->GetHealth()) <= 25 && I::Engine->GetPlayerInfo(pEnemy->m_iId, &PlrInfo))
							{
								switch (RandomINT(0, 4))
								{
								case 0: xprintf(szData, XorString("%s is low, %d HP"), ShortenName(string(PlrInfo.name)).c_str(), iHealth); break;
								case 1: xprintf(szData, XorString("%s is LOW (%d)"), ShortenName(string(PlrInfo.name)).c_str(), iHealth); break;
								case 2: xprintf(szData, XorString("-%d last"), 100 - iHealth); break;
								case 3: xprintf(szData, XorString("-%d"), 100 - iHealth); break;
								case 4: xprintf(szData, XorString("he's %d HP"), iHealth); break;
								};

								sprintf(g_Phrases[g_PhrasesSize++], szData), g_timeLastEventStamp = theTime;
							};
						};

						g_bCheckForLastEnemy = FALSE;
					}

					else
					{
						if (Vars.Misc.bSpamRandomEnemies && theTime - timeStamp > RandomINT(32, 64) && theTime - g_timeLastEventStamp > RandomINT(Vars.Misc.iSpamDelaySeconds, Vars.Misc.iSpamDelaySeconds + Vars.Misc.iSpamDelaySeconds / 2))
						{
							iSize = 0;

							for (iPlayer = 0; iPlayer <= I::Globals->m_iMaxClients; iPlayer++)
							{
								pPlayer = I::Ents->GetClEntity(iPlayer);
								if (!pPlayer || pPlayer == G::pSelf || (iTeam = pPlayer->GetTeam()) == G::iTeam || iTeam < 2 || iTeam > 3) continue;
								Players[iSize++] = iPlayer;
							};

							if (iSize > 0 && (pPlayer = I::Ents->GetClEntity(Players[RandomINT(0, iSize - 1)])) && I::Engine->GetPlayerInfo(pPlayer->m_iId, &PlrInfo))
							{
								PrepareSpamString(szData, &PlrInfo);
								sprintf(g_Phrases[g_PhrasesSize++], szData);

								if ((g_Phrases[g_PhrasesSize - 1][0] >= 'a' && g_Phrases[g_PhrasesSize - 1][0] <= 'z') || (g_Phrases[g_PhrasesSize - 1][0] >= 'A' && g_Phrases[g_PhrasesSize - 1][0] <= 'Z'))
								{
									if (RandomINT(0, 1) == 0) g_Phrases[g_PhrasesSize - 1][0] = toupper(g_Phrases[g_PhrasesSize - 1][0]);
									else if (xIsCharLower(g_Phrases[g_PhrasesSize - 1][1])) g_Phrases[g_PhrasesSize - 1][0] = tolower(g_Phrases[g_PhrasesSize - 1][0]);
								};

								xprintf(szData, XorString("Say \"%s\";"), g_Phrases[g_PhrasesSize - 1]);
								I::Engine->ClientCmd_Unrestricted(szData, NULL);
								g_PhrasesSize--, g_timeLastEventStamp = theTime, timeStamp = theTime;
							};
						};
					};
				};
			};

#endif

			if (G::pSelf && G::iTeam > 0)
			{
				if (Vars.Misc.bFlashbangReducer && G::bAlive) G::pSelf->SetFlashMaxAlpha((FLOAT)Vars.Misc.iFlashbangAmount);
				if (Vars.Misc.bRevealRanks && (G::Keys[VK_TAB] || G::pCmd->buttons & IN_SCORE) && !G::pSelf->GetOrigin().IsZero()) ServerRankRevealAll();
			};
		};

		Sleep(XS_RANDOM_SIZE / 6);
	};

	return EXIT_SUCCESS;
};

DWORD WINAPI runModule(HANDLE)
{
	Vars.Menu.bOpened = false;
	while (!(g_pWnd = FindWindow(XorString("Valve001"), NULL))) Sleep(XS_RANDOM_SIZE / 8);
	g_pOldWndProc = (WNDPROC)SetWindowLongPtr(g_pWnd, GWL_WNDPROC, (LONG_PTR)OnWndProc);
	CreateThread(NULL, NULL, runAll, NULL, NULL, NULL);
	return EXIT_SUCCESS;
};

DWORD WINAPI DllMain(HANDLE, DWORD dwReason, HANDLE)
{
	if (dwReason == DLL_PROCESS_ATTACH && !g_bLoaded) CreateThread(NULL, NULL, runModule, NULL, NULL, NULL), g_bLoaded = true;
	return BOOL(true);
};
