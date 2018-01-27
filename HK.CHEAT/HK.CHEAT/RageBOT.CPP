
#include "Misc.H"

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

VOID CRBOT::DoAim(VOID)
{
	static Entity_T * pEntity = NULL, *pEnemy = NULL;
	static INT iIter = 0, iTeam = 0;
	static FLOAT fBestFoV = 0.f, fFoV = 0.f;
	static QAngle angAim;
	static Vector vecTopHBox, vecHBox, vecFwd;
	static GameTrace_T Trace;
	static bool bSniper = false;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	static FLOAT fBestDamage = 0.f, fDamage = 0.f;
	static WeaponInfo_T * pData = NULL;

	if (!Vars.RageBOT.bHeavy)
	{
		if (FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && (!Vars.LEGITBOT.AimHelpers.bLimitAim || CanAim()))
		{
			pEnemy = NULL, fBestFoV = Vars.RageBOT.fFoV;

			for (iIter = 0; iIter <= I::Globals->m_iMaxClients; iIter++)
			{
				if (g_bSmokeExcluded[iIter] || !(pEntity = I::Ents->GetClEntity(iIter)) || pEntity == G::pSelf || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || pEntity->GetImmune() || pEntity->GetHealth() < 1 || (!IS_SNIPER(G::iWeapId) && !pEntity->IsVisible()) || !TopZone(true, pEntity, vecHBox, (IS_SNIPER(G::iWeapId) && G::bScoped) ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV), &fFoV))
					continue;

				if (fFoV < fBestFoV)
					pEnemy = pEntity, fBestFoV = fFoV, vecTopHBox = vecHBox;
			};

			if (pEnemy && G::pWeap->GetNextPrimaryAttack() <= G::pSelf->GetTickBase() * I::Globals->m_fIntervPerTick)
			{
				bSniper = IS_SNIPER(G::iWeapId);

				if (bSniper && !G::bScoped)
					G::pCmd->buttons |= IN_ATTACK2;

				else
				{
					AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);

					if (bSniper)
					{
						if ((!Trace.ent || !Trace.hitgroup) && !G::fSelfVelLen && FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f))
							angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
					}

					else
					{
						if (!Trace.ent || Trace.hitgroup != HITGROUP_HEAD)
						{
							if (G::iWeapId == WEAPON_DEAGLE)
							{
								if (!G::fSelfVelLen)
									angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
							}

							else
								angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
						};
					};

					if (G::iWeapId != WEAPON_AWP && G::iWeapId != WEAPON_SSG08)
					{
						if (G::iWeapId == WEAPON_DEAGLE)
						{
							if (!G::fSelfVelLen)
							{
								AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
								if (Trace.ent && (Trace.hitgroup == HITGROUP_CHEST || Trace.hitgroup == HITGROUP_HEAD || Trace.hitgroup == HITGROUP_STOMACH)) G::pCmd->buttons |= IN_ATTACK;
							};
						}

						else
						{
							AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
							if (Trace.ent && (Trace.hitgroup == HITGROUP_CHEST || Trace.hitgroup == HITGROUP_HEAD || Trace.hitgroup == HITGROUP_STOMACH)) G::pCmd->buttons |= IN_ATTACK;
						};
					}

					else if (FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f) && !G::fSelfVelLen && G::bScoped)
					{
						AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
						if (Trace.ent && Trace.hitgroup) G::pCmd->buttons |= IN_ATTACK;
					};
				};
			};
		};
	}

	else
	{
		if (FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && (pData = G::pWeap->GetSpecialData()) && ((1.f - G::pWeap->GetAccuracyPenalty()) * 100.f >= 75.f))
		{
			pEnemy = NULL, fBestDamage = 0.f;

			for (iIter = 0; iIter <= I::Globals->m_iMaxClients; iIter++)
			{
				if (!(pEntity = I::Ents->GetClEntity(iIter)) || pEntity == G::pSelf || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || pEntity->GetImmune() || pEntity->GetHealth() < 1)
					continue;

				if (TopZoneHeavyRage(pEntity, vecHBox, (IS_SNIPER(G::iWeapId) && G::bScoped) ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV), &fDamage, pData) && fDamage > fBestDamage)
					pEnemy = pEntity, fBestDamage = fDamage, vecTopHBox = vecHBox;

				else if (TopZone(true, pEntity, vecHBox, (IS_SNIPER(G::iWeapId) && G::bScoped) ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV), NULL))
					pEnemy = pEntity, vecTopHBox = vecHBox;
			};

			if (pEnemy && G::pWeap->GetNextPrimaryAttack() <= G::pSelf->GetTickBase() * I::Globals->m_fIntervPerTick)
			{
				bSniper = IS_SNIPER(G::iWeapId);

				if (bSniper && !G::bScoped)
					G::pCmd->buttons |= IN_ATTACK2;

				else
				{
					if (bSniper)
					{
						if (!G::fSelfVelLen && FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f))
							angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
					}

					else
					{
						if (G::iWeapId == WEAPON_DEAGLE)
						{
							if (!G::fSelfVelLen) angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
						}

						else angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
					};

					if (G::iWeapId != WEAPON_AWP && G::iWeapId != WEAPON_SSG08)
					{
						if (G::iWeapId == WEAPON_DEAGLE)
						{
							if (!G::fSelfVelLen) G::pCmd->buttons |= IN_ATTACK;
						}

						else G::pCmd->buttons |= IN_ATTACK;
					}

					else if (FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f) && !G::fSelfVelLen && G::bScoped) G::pCmd->buttons |= IN_ATTACK;
				};
			};
		};
	};

#else

if (FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && (!Vars.LEGITBOT.AimHelpers.bLimitAim || CanAim()))
{
	pEnemy = NULL, fBestFoV = Vars.RageBOT.fFoV;

	for (iIter = 0; iIter <= I::Globals->m_iMaxClients; iIter++)
	{
		if (g_bSmokeExcluded[iIter] || !(pEntity = I::Ents->GetClEntity(iIter)) || pEntity == G::pSelf || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || pEntity->GetImmune() || pEntity->GetHealth() < 1 || (!IS_SNIPER(G::iWeapId) && !pEntity->IsVisible()) || !TopZone(true, pEntity, vecHBox, (IS_SNIPER(G::iWeapId) && G::bScoped) ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV), &fFoV))
			continue;

		if (fFoV < fBestFoV)
			pEnemy = pEntity, fBestFoV = fFoV, vecTopHBox = vecHBox;
	};

	if (pEnemy && G::pWeap->GetNextPrimaryAttack() <= G::pSelf->GetTickBase() * I::Globals->m_fIntervPerTick)
	{
		bSniper = IS_SNIPER(G::iWeapId);

		if (bSniper && !G::bScoped)
			G::pCmd->buttons |= IN_ATTACK2;

		else
		{
			AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);

			if (bSniper)
			{
				if ((!Trace.ent || !Trace.hitgroup) && !G::fSelfVelLen && FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f))
					angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
			}

			else
			{
				if (!Trace.ent || Trace.hitgroup != HITGROUP_HEAD)
				{
					if (G::iWeapId == WEAPON_DEAGLE)
					{
						if (!G::fSelfVelLen)
							angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
					}

					else
						angAim = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, G::pCmd->viewang = angAim, I::Engine->SetViewAngles(angAim);
				};
			};

			if (G::iWeapId != WEAPON_AWP && G::iWeapId != WEAPON_SSG08)
			{
				if (G::iWeapId == WEAPON_DEAGLE)
				{
					if (!G::fSelfVelLen)
					{
						AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
						if (Trace.ent && (Trace.hitgroup == HITGROUP_CHEST || Trace.hitgroup == HITGROUP_HEAD || Trace.hitgroup == HITGROUP_STOMACH)) G::pCmd->buttons |= IN_ATTACK;
					};
				}

				else
				{
					AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
					if (Trace.ent && (Trace.hitgroup == HITGROUP_CHEST || Trace.hitgroup == HITGROUP_HEAD || Trace.hitgroup == HITGROUP_STOMACH)) G::pCmd->buttons |= IN_ATTACK;
				};
			}

			else if (FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) >= FLOAT(.5f) && !G::fSelfVelLen && G::bScoped)
			{
				AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);
				if (Trace.ent && Trace.hitgroup) G::pCmd->buttons |= IN_ATTACK;
			};
		};
	};
};

#endif

};

#endif
