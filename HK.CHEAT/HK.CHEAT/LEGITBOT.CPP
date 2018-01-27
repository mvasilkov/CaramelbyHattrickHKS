
#include "Misc.h"

VOID CLBOT::Run(VOID)
{

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

	if (Vars.LEGITBOT.AimBOT.bON && (G::Keys[VK_LBUTTON] || G::pCmd->buttons & IN_ATTACK))
		DoAim();

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

	if ((G::Keys[Vars.LEGITBOT.TriggerBOT.iKey] || Vars.LEGITBOT.TriggerBOT.bAuto) && Vars.LEGITBOT.TriggerBOT.bON)
		TriggerBOT();

#endif

};

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

VOID CLBOT::DoAim(VOID)
{
	static INT iIter = 0, iTeam = 0;
	static Entity_T * pEntity = NULL, *pEnemy = NULL;
	static FLOAT fFoV = 0.f, fTopFoV = 0.f;
	static Vector vecHBox, vecTopHBox, vecFwd;
	static GameTrace_T Trace;

	if (FLOAT(((DOUBLE)clock() - g_switchTime) / CLOCKS_PER_SEC) >= FLOAT(1.333333f) && (!Vars.LEGITBOT.AimHelpers.bLimitAim || CanAim()))
	{
		AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecFwd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecFwd * 8192.f, MASK_SHOT, G::pSelf, &Trace);

		if ((!Trace.ent || !Trace.hitgroup) && (!IS_SNIPER(G::iWeapId) || G::bScoped))
		{
			fTopFoV = Vars.LEGITBOT.AimBOT.fFoV, pEnemy = NULL;

			for (iIter = 0; iIter <= I::Globals->m_iMaxClients; iIter++)
			{
				if (g_bSmokeExcluded[iIter] || !(pEntity = I::Ents->GetClEntity(iIter)) || pEntity == G::pSelf || pEntity->GetHealth() < 1 || pEntity->GetImmune() || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || (!IS_SNIPER(G::iWeapId) && !pEntity->IsVisible()) || !TopZone(false, pEntity, vecHBox, ((G::iWeapId == WEAPON_AWP || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_SCAR20 || G::iWeapId == WEAPON_G3SG1) && G::bScoped) ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV), &fFoV))
					continue;

				if (fFoV < fTopFoV)
					fTopFoV = fFoV, pEnemy = pEntity, vecTopHBox = vecHBox;
			};

			if (pEnemy && G::pWeap->GetNextPrimaryAttack() <= G::pSelf->GetTickBase() * I::Globals->m_fIntervPerTick)
				G::pCmd->viewang = CalcAng(G::vecSelfEyePos, vecTopHBox) - G::angSelfPunch * 2.f, I::Engine->SetViewAngles(G::pCmd->viewang);
		};
	};
};

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

VOID CLBOT::TriggerBOT(VOID)
{
	static Vector fwdVec;
	static bool bDidHit = false, bSniper = false, bInMove = false, bPistol = false;
	static INT iTeam = 0;
	static clock_t Old = 0, Now = 0;
	static GameTrace_T Trace;

	AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &fwdVec), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + fwdVec * 8192.f, MASK_SHOT, G::pSelf, &Trace);

	if (!Trace.ent || g_bSmokeExcluded[Trace.ent->m_iId] || (iTeam = Trace.ent->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || Trace.ent->GetHealth() < 1 || Trace.ent->GetImmune())
	{
		if (g_uTraces > 0) g_uTraces--;
		return;
	};

	bSniper = IS_SNIPER(G::iWeapId), bPistol = IS_PISTOL(G::iWeapId), bDidHit = false;

	if (bSniper || bPistol)
	{
		if (G::iWeapId == WEAPON_DEAGLE)
		{
			if (Trace.hitgroup == HITGROUP_HEAD)
				bDidHit = true;
		}

		else
		{
			if (Trace.hitgroup)
				bDidHit = true;
		};
	}

	else
	{
		if (Trace.hitgroup == HITGROUP_HEAD)
			bDidHit = true;
	};

	if (bDidHit)
	{
		bInMove = ((G::pCmd->buttons & IN_BACK) || (G::pCmd->buttons & IN_FORWARD) || (G::pCmd->buttons & IN_MOVELEFT) || (G::pCmd->buttons & IN_MOVERIGHT) || (G::pCmd->buttons & IN_JUMP)) ? true : false;

		if (bSniper && (!G::bScoped || G::fSelfVelLen || bInMove || FLOAT(((DOUBLE)clock() - g_zoomTime) / CLOCKS_PER_SEC) < FLOAT(.5f)))
		{
			if (g_uTraces > 0) g_uTraces--;
			return;
		}

		else if (G::iWeapId == WEAPON_DEAGLE && (G::fSelfVelLen || bInMove))
		{
			if (g_uTraces > 0) g_uTraces--;
			return;
		};

		if (++g_uTraces >= 2)
		{
			if (bPistol)
			{
				if (G::pWeap->GetNextPrimaryAttack() <= G::pSelf->GetTickBase() * I::Globals->m_fIntervPerTick)
				{
					Now = clock();

					if (FLOAT(((DOUBLE)Now - Old) / CLOCKS_PER_SEC) < FLOAT(Vars.LEGITBOT.TriggerBOT.fDelay))
					{
						if (g_uTraces > 0) g_uTraces--;
					}

					else G::pCmd->buttons |= IN_ATTACK, Old = clock();
				};
			}

			else
			{
				Now = clock();

				if (FLOAT(((DOUBLE)Now - Old) / CLOCKS_PER_SEC) < FLOAT(Vars.LEGITBOT.TriggerBOT.fDelay))
				{
					if (g_uTraces > 0) g_uTraces--;
				}

				else G::pCmd->buttons |= IN_ATTACK, Old = clock();
			};

			g_uTraces = 1;
		};
	}

	else
	{
		if (g_uTraces > 0) g_uTraces--;
	};
};

#endif
