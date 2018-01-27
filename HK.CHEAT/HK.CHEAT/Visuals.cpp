
#include "Misc.h"

VOID CVisuals::Run(VOID)
{
	static CONST Vector vecZ = Vector(0.f, 0.f, 1.f);

	static Color xColor;
	static Entity_T * pEntity = NULL, *pWeapon = NULL, *pOther = NULL;
	static ClientClass * pClass = NULL;
	static INT iOtherId = 0, iId = 0, iTeam = 0, iWidth = 0, iHeight = 0, iCrX = 0, iCrY = 0, iWeapon = 0, iHealth = 0, iIter = 0, iHBoxSet = 0;
	static Vector vecMin, vecMax, vecLocations[9], vecPositions[9], vecPos, vecScreen, vecGrenVel, vecThrow, vecStart, vecEnd, vecScreenEnd, vecSelfVel, vecPlaneNormal;
	static FLOAT fStep = 0.f, fHeight = 0.f, fTimer = 0.f, fDTime = 0.f, fElasticity = 0.f, fSurfElasticity = 0.f, fImpulse = 0.f, fNewVel = 0.f, fDuration = 0.f, fThrowVel = 0.f, fForce = 0.f, fAvgVel = 0.f;
	static GameTrace_T Trace;
	static QAngle angThrow;
	static CHAR szBuffer[XS_RANDOM_SIZE] = {};
	static ICollideable * pCollideable = NULL;
	static bool bShouldDivideFoV = false;
	static Matrix34_T mMatrix[128];
	static StdHdr_T * pHdr = NULL;
	static StdioBBox_T * pUntransfBox = NULL;
	static Model_T * pModel = NULL;
	static StdHBoxSet_T * pHBoxSet = NULL;

	if (g_bOffline || g_nStyle < 0 || !(G::pSelf = GetSelf()))
		return;

	I::Engine->GetScreenSize(iWidth, iHeight);

	if (Vars.Misc.bEnableHackTradeMark)
		DrawStr(iWidth * .24f, 16.f, g_White, false, XorString(WATERMARK_NAME)), DrawStr(iWidth * .24f, 32.f, g_White, false, XorString(WATERMARK_INFO));

	g_pNadeTraj->Work();

	if (G::bAlive)
	{
		pOther = nullptr;

		if (Vars.Misc.bGrenadesPrediction && !G::bEmpty && !G::bGun && IS_GRENADE(G::iWeapId) && ((G::pCmd->buttons & IN_ATTACK) || (G::pCmd->buttons & IN_ATTACK2)))
		{
			iId = 0, fDTime = GetDetonationTime(), angThrow = G::pCmd->viewang + G::angSelfPunch * 2.f, angThrow.x = -10.f + angThrow.x + fabsf(angThrow.x) * 10.f / 90.f, \
				AngVec(angThrow, &vecThrow), vecThrow.Normalize(), vecStart = G::vecSelfEyePos, vecStart.x += vecThrow.x * 16.f, vecStart.y += vecThrow.y * 16.f, \
				vecStart.z += vecThrow.z * 16.f, vecStart.z += GetDisp(), vecSelfVel = G::vecSelfVel, fThrowVel = 750.f * GetFactor(), vecSelfVel *= 1.25f, \
				vecGrenVel.x = vecThrow.x * fThrowVel + vecSelfVel.x, vecGrenVel.y = vecThrow.y * fThrowVel + vecSelfVel.y, vecGrenVel.z = vecThrow.z * fThrowVel + vecSelfVel.z;

			for (fStep = 0.f; fStep <= fDTime; fStep += .05f)
			{
				vecEnd.x = vecStart.x + vecGrenVel.x * .05f, vecEnd.y = vecStart.y + vecGrenVel.y * .05f, fForce = .4f * 800.f, fNewVel = vecGrenVel.z - fForce * .05f, \
					fAvgVel = (vecGrenVel.z + fNewVel) / 2.f, vecEnd.z = vecStart.z + fAvgVel * .05f, vecGrenVel.z = fNewVel, TraceLine(vecStart, vecEnd, MASK_SHOT_HULL, G::pSelf, &Trace);

				if (Trace.fraction != 1.f)
				{
					if (Trace.ent == G::pSelf && fStep == 0.f)
					{
						vecStart = vecEnd;
						continue;
					};

					vecEnd = Trace.endpos, vecPlaneNormal = Trace.plane.normal, fImpulse = 2.f * vecPlaneNormal.Dot(vecGrenVel);

					vecGrenVel.x -= fImpulse * vecPlaneNormal.x;
					if (fabsf(vecGrenVel.x) < .1f) vecGrenVel.x = 0.f;

					vecGrenVel.y -= fImpulse * vecPlaneNormal.y;
					if (fabsf(vecGrenVel.y) < .1f) vecGrenVel.y = 0.f;

					vecGrenVel.z -= fImpulse * vecPlaneNormal.z;
					if (fabsf(vecGrenVel.z) < .1f) vecGrenVel.z = 0.f;

					fSurfElasticity = Trace.ent != NULL ? Trace.ent->GetElasticity() : 0.f, fElasticity = .45f * fSurfElasticity, vecGrenVel *= fElasticity;

					if (vecPlaneNormal.Dot(vecZ) > .7f && (G::iWeapId == WEAPON_MOLOTOV || G::iWeapId == WEAPON_INCGRENADE))
						fDTime = 0.f;

					if (++iId >= 1 && WTS(vecStart, vecScreen) && WTS(vecEnd, vecScreenEnd))
						DrawLine(vecScreenEnd.x - 7.5f, vecScreenEnd.y, vecScreenEnd.x + 7.5f, vecScreenEnd.y, g_Red), DrawLine(vecScreenEnd.x, vecScreenEnd.y - 7.5f, vecScreenEnd.x, vecScreenEnd.y + 7.5f, g_Red);
				};

				if (iId >= 1 && WTS(vecStart, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawLine((INT)vecScreen.x, (INT)vecScreen.y, (INT)vecScreenEnd.x, (INT)vecScreenEnd.y, g_White);

				vecStart = vecEnd;
			};
		};

		if (Vars.Misc.bAutoSniperCrosshair && IS_SNIPER(G::iWeapId) && !G::bScoped)
			xColor = ClrByOption(Vars.Misc.iSniperCrosshairColor), iCrX = iWidth / 2, iCrY = iHeight / 2, DrawLine(iCrX - 5, iCrY, iCrX + 5, iCrY, xColor), DrawLine(iCrX, iCrY - 5, iCrX, iCrY + 5, xColor);

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0 && defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

		if (Vars.Misc.bDrawFoVCircle && (Vars.LEGITBOT.AimBOT.bON || Vars.RageBOT.bON))
		{
			bShouldDivideFoV = ((G::iWeapId == WEAPON_AWP || G::iWeapId == WEAPON_SSG08 || G::iWeapId == WEAPON_SCAR20 || G::iWeapId == WEAPON_G3SG1) && G::bScoped);

			if (!Vars.Misc.iFoVCircleType)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (Vars.RageBOT.bON ? (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV)) : (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV))), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else if (Vars.Misc.iFoVCircleType == 1)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (Vars.RageBOT.bON ? (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV)) : (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV))), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(vecScreen.x, vecScreen.y, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else
			{
				AngVec(G::pCmd->viewang, &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (Vars.RageBOT.bON ? (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.RageBOT.fFoV)) : (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV))), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			};
		};

#elif (!defined(ENABLE_RAGE_BOT) || ENABLE_RAGE_BOT != 1) && defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

		if (Vars.Misc.bDrawFoVCircle && Vars.LEGITBOT.AimBOT.bON)
		{
			if (!Vars.Misc.iFoVCircleType)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else if (Vars.Misc.iFoVCircleType == 1)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(vecScreen.x, vecScreen.y, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else
			{
				AngVec(G::pCmd->viewang, &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.LEGITBOT.AimBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			};
		};

#elif (!defined(ENABLE_AIM_BOT) || ENABLE_AIM_BOT != 0) && defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

		if (Vars.Misc.bDrawFoVCircle && Vars.RageBOT.bON)
		{
			if (!Vars.Misc.iFoVCircleType)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else if (Vars.Misc.iFoVCircleType == 1)
			{
				AngVec(G::pCmd->viewang + (G::angSelfPunch * 2.f), &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(vecScreen.x, vecScreen.y, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			}

			else
			{
				AngVec(G::pCmd->viewang, &vecStart), vecPos = vecStart * 8192.f + G::vecSelfEyePos, \
					AngVec(QAngle(G::pCmd->viewang.x + (G::angSelfPunch.x * 2.f) + (bShouldDivideFoV ? (Vars.RageBOT.fFoV / 2) : (Vars.LEGITBOT.AimBOT.fFoV)), G::pCmd->viewang.y + (G::angSelfPunch.y * 2.f), 0.f), &vecStart), vecEnd = vecStart * 8192.f + G::vecSelfEyePos;

				if (WTS(vecPos, vecScreen) && WTS(vecEnd, vecScreenEnd))
					DrawCircle(iWidth / 2, iHeight / 2, vecScreenEnd.y - vecScreen.y, 128.f, ClrByOption(Vars.Misc.iFoVCircleColor));
			};
		};

#endif

		if (G::bGun && !G::bEmpty && Vars.Misc.bRecoilCrosshair && (!G::angSelfPunch.IsZero() || G::bScoped))
		{
			AngVec(G::pCmd->viewang + G::angSelfPunch * 2.f, &vecEnd), TraceLine(G::vecSelfEyePos, G::vecSelfEyePos + vecEnd, MASK_SHOT, G::pSelf, &Trace);

			if (WTS(Trace.endpos, vecScreen))
				DrawLine((INT)(vecScreen.x - 5.f), (INT)(vecScreen.y), (INT)(vecScreen.x + 5.f), (INT)(vecScreen.y), (xColor = ClrByOption(Vars.Misc.iRecoilCrosshairType))), \
				DrawLine((INT)(vecScreen.x), (INT)(vecScreen.y - 5.f), (INT)(vecScreen.x), (INT)(vecScreen.y + 5.f), xColor);
		};

		if (Vars.Visuals.bDrawHitPoints || Vars.Visuals.bDrawEnemySight)
		{
			for (iId = 0; iId <= I::Globals->m_iMaxClients; iId++)
			{
				if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity == G::pSelf || pEntity->IsDormant() || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || pEntity->GetHealth() < 1)
					continue;

				if (Vars.Visuals.bDrawEnemySight)
				{
					vecStart = pEntity->GetEyePos(), AngVec(pEntity->GetEyeAng() + pEntity->GetPunch() * 2.f, &vecEnd), vecEnd *= 8192.f, TraceLine(vecStart, vecStart + vecEnd, MASK_SHOT, pEntity, &Trace);
					if (WTS(vecStart, vecScreen) && WTS(Trace.endpos, vecScreenEnd)) DrawLine(vecScreen.x, vecScreen.y, vecScreenEnd.x, vecScreenEnd.y, ClrByOption(Vars.Visuals.iEnemySightColor)), DrawLine(vecScreenEnd.x - 2.5f, vecScreenEnd.y, vecScreenEnd.x + 2.5f, vecScreenEnd.y, g_Red), DrawLine(vecScreenEnd.x, vecScreenEnd.y - 2.5f, vecScreenEnd.x, vecScreenEnd.y + 2.5f, g_Red);
				};

				if (Vars.Visuals.bDrawHitPoints)
				{
					if (pEntity->SetupBones(mMatrix) && (pModel = pEntity->GetModel()) && (pHdr = I::MDLInfo->GetStudioModel(pModel)) && (iHBoxSet = pEntity->GetHBoxSet()) >= 0 && (pHBoxSet = pHdr->pHBoxSet(iHBoxSet)))
					{
						if (!Vars.Visuals.bDrawHitPointsOnlyHead)
						{
							for (iIter = 0; iIter < ARRAYSIZE(g_nHitBoxes); iIter++)
							{
								if (pUntransfBox = pHBoxSet->pHBox(g_nHitBoxes[iIter]))
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

									for (iOtherId = 0; iOtherId < ARRAYSIZE(vecLocations); iOtherId++)
									{
										if (iOtherId) vecLocations[iOtherId] = (((vecLocations[iOtherId] + vecLocations[0]) * .5f) + vecLocations[iOtherId]) * .5f;
										VecTransf(vecLocations[iOtherId], mMatrix[pUntransfBox->m_iBone], vecPositions[iOtherId]);
										if (WTS(vecPositions[iOtherId], vecScreen)) DrawLine(vecScreen.x - 1.f, vecScreen.y, vecScreen.x + 1.f, vecScreen.y, ClrByOption(Vars.Visuals.iHitPointsColor)), DrawLine(vecScreen.x, vecScreen.y - 1.f, vecScreen.x, vecScreen.y + 1.f, ClrByOption(Vars.Visuals.iHitPointsColor));
									};
								};
							};
						}

						else
						{
							if (pUntransfBox = pHBoxSet->pHBox(HITBOX_HEAD))
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

								for (iOtherId = 0; iOtherId < ARRAYSIZE(vecLocations); iOtherId++)
								{
									if (iOtherId) vecLocations[iOtherId] = (((vecLocations[iOtherId] + vecLocations[0]) * .5f) + vecLocations[iOtherId]) * .5f;
									VecTransf(vecLocations[iOtherId], mMatrix[pUntransfBox->m_iBone], vecPositions[iOtherId]);
									if (WTS(vecPositions[iOtherId], vecScreen)) DrawLine(vecScreen.x - 1.f, vecScreen.y, vecScreen.x + 1.f, vecScreen.y, ClrByOption(Vars.Visuals.iHitPointsColor)), DrawLine(vecScreen.x, vecScreen.y - 1.f, vecScreen.x, vecScreen.y + 1.f, ClrByOption(Vars.Visuals.iHitPointsColor));
								};
							};
						};
					};
				};
			};
		};

		if (Vars.Misc.bSpecTable)
		{
			fStep = 4.f, DrawStr(iWidth * Vars.Misc.fSpecTableX, iHeight * Vars.Misc.fSpecTableY, ClrByOption(Vars.Misc.iSpecTableColor), false, XorString("Spectators Table")), fStep += Vars.Misc.iFontHeight + 2.f;

			for (iId = 0; iId <= I::Globals->m_iMaxClients; iId++)
			{
				if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity == G::pSelf || pEntity->GetHealth() > 0 || pEntity->GetTarget() != G::pSelf)
					continue;

				DrawStr(iWidth * Vars.Misc.fSpecTableX, iHeight * Vars.Misc.fSpecTableY + fStep, ClrByOption(Vars.Misc.iSpecTableColor), false, pEntity->GetPlayerName().c_str()), fStep += Vars.Misc.iFontHeight + 2.f;
			};
		};
	}

	else
	{
		if ((pOther = G::pSelf->GetTarget()) && pOther->GetHealth() < 1)
			pOther = nullptr;

		if (Vars.Misc.bAutoSniperCrosshair && pOther && (pWeapon = pOther->GetWeapon()) && IS_SNIPER(pWeapon->GetItemDefinitionIndex()) && !pOther->IsScoped())
			xColor = ClrByOption(Vars.Misc.iSniperCrosshairColor), iCrX = iWidth / 2, iCrY = iHeight / 2, DrawLine(iCrX - 5, iCrY, iCrX + 5, iCrY, xColor), DrawLine(iCrX, iCrY - 5, iCrX, iCrY + 5, xColor);

		if (Vars.Misc.bSpecTable && pOther)
		{
			fStep = 4.f, DrawStr(iWidth * Vars.Misc.fSpecTableX, iHeight * Vars.Misc.fSpecTableY, ClrByOption(Vars.Misc.iSpecTableColor), false, XorString("Spectators Table")), fStep += Vars.Misc.iFontHeight + 2.f;

			for (iId = 0; iId <= I::Globals->m_iMaxClients; iId++)
			{
				if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity->GetHealth() > 0 || pEntity->GetTarget() != pOther)
					continue;

				DrawStr(iWidth * Vars.Misc.fSpecTableX, iHeight * Vars.Misc.fSpecTableY + fStep, ClrByOption(Vars.Misc.iSpecTableColor), false, pEntity->GetPlayerName().c_str()), fStep += Vars.Misc.iFontHeight + 2.f;
			};
		};

		if (Vars.Visuals.bDrawHitPoints || Vars.Visuals.bDrawEnemySight)
		{
			for (iId = 0; iId <= I::Globals->m_iMaxClients; iId++)
			{
				if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity == pOther || pEntity == G::pSelf || pEntity->IsDormant() || (iTeam = pEntity->GetTeam()) < 2 || iTeam > 3 || iTeam == G::iTeam || pEntity->GetHealth() < 1)
					continue;

				if (Vars.Visuals.bDrawEnemySight)
				{
					vecStart = pEntity->GetEyePos(), AngVec(pEntity->GetEyeAng() + pEntity->GetPunch() * 2.f, &vecEnd), vecEnd *= 8192.f, TraceLine(vecStart, vecStart + vecEnd, MASK_SHOT, pEntity, &Trace);
					if (WTS(vecStart, vecScreen) && WTS(Trace.endpos, vecScreenEnd)) DrawLine(vecScreen.x, vecScreen.y, vecScreenEnd.x, vecScreenEnd.y, ClrByOption(Vars.Visuals.iEnemySightColor)), DrawLine(vecScreenEnd.x - 2.5f, vecScreenEnd.y, vecScreenEnd.x + 2.5f, vecScreenEnd.y, g_Red), DrawLine(vecScreenEnd.x, vecScreenEnd.y - 2.5f, vecScreenEnd.x, vecScreenEnd.y + 2.5f, g_Red);
				};

				if (Vars.Visuals.bDrawHitPoints)
				{
					if (pEntity->SetupBones(mMatrix) && (pModel = pEntity->GetModel()) && (pHdr = I::MDLInfo->GetStudioModel(pModel)) && (iHBoxSet = pEntity->GetHBoxSet()) >= 0 && (pHBoxSet = pHdr->pHBoxSet(iHBoxSet)))
					{
						if (!Vars.Visuals.bDrawHitPointsOnlyHead)
						{
							for (iIter = 0; iIter < ARRAYSIZE(g_nHitBoxes); iIter++)
							{
								if (pUntransfBox = pHBoxSet->pHBox(g_nHitBoxes[iIter]))
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

									for (iOtherId = 0; iOtherId < ARRAYSIZE(vecLocations); iOtherId++)
									{
										if (iOtherId) vecLocations[iOtherId] = (((vecLocations[iOtherId] + vecLocations[0]) * .5f) + vecLocations[iOtherId]) * .5f;
										VecTransf(vecLocations[iOtherId], mMatrix[pUntransfBox->m_iBone], vecPositions[iOtherId]);
										if (WTS(vecPositions[iOtherId], vecScreen)) DrawLine(vecScreen.x - 1.f, vecScreen.y, vecScreen.x + 1.f, vecScreen.y, ClrByOption(Vars.Visuals.iHitPointsColor)), DrawLine(vecScreen.x, vecScreen.y - 1.f, vecScreen.x, vecScreen.y + 1.f, ClrByOption(Vars.Visuals.iHitPointsColor));
									};
								};
							};
						}

						else
						{
							if (pUntransfBox = pHBoxSet->pHBox(HITBOX_HEAD))
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

								for (iOtherId = 0; iOtherId < ARRAYSIZE(vecLocations); iOtherId++)
								{
									if (iOtherId) vecLocations[iOtherId] = (((vecLocations[iOtherId] + vecLocations[0]) * .5f) + vecLocations[iOtherId]) * .5f;
									VecTransf(vecLocations[iOtherId], mMatrix[pUntransfBox->m_iBone], vecPositions[iOtherId]);
									if (WTS(vecPositions[iOtherId], vecScreen)) DrawLine(vecScreen.x - 1.f, vecScreen.y, vecScreen.x + 1.f, vecScreen.y, ClrByOption(Vars.Visuals.iHitPointsColor)), DrawLine(vecScreen.x, vecScreen.y - 1.f, vecScreen.x, vecScreen.y + 1.f, ClrByOption(Vars.Visuals.iHitPointsColor));
								};
							};
						};
					};
				};
			};
		};
	};

	if (!Vars.Visuals.bON)
	{
		for (iId = 0; iId < I::Ents->GetHighestEntityIndex(); iId++)
		{
			if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity == pOther || pEntity == G::pSelf || pEntity->IsDormant() || (vecPos = pEntity->GetOrigin()).IsZero() || !(pClass = pEntity->GetClientClass()))
				continue;

			if (pClass->m_ClassID == CCSPlayer)
			{
				if ((iHealth = pEntity->GetHealth()) < 1 || (iTeam = pEntity->GetTeam()) < 2 || iTeam == G::iTeam || iTeam > 3 || !(pWeapon = pEntity->GetWeapon()) || (iWeapon = pWeapon->GetItemDefinitionIndex()) < 1)
					continue;

				if (Vars.Visuals.bRadar)
					pEntity->SetSpotted();

				if ((pCollideable = pEntity->GetCollideable()) && WTS(vecPos, vecScreen) && WTS(vecPos + Vector(NULL, NULL, pCollideable->OBBMaxs().z), vecScreenEnd))
				{
					if (Vars.Misc.bBombSiteSkeletons && g_bBombPlanted && !pEntity->IsVisible())
						Skeleton(pEntity);

					if (Vars.Misc.bEnableEnemyDetails)
					{
						fHeight = vecScreen.y - vecScreenEnd.y, fStep = 8.f;

						if (iHealth < 20)
							xprintf(szBuffer, XorString("%d HP"), iHealth), DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_CandyRed, true, szBuffer), fStep += Vars.Misc.iFontHeight + 2.f;

						if (pWeapon->IsGun(iWeapon))
						{
							if (pWeapon->IsEmpty())
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("RELOADING")), fStep += Vars.Misc.iFontHeight + 2.f;
						}

						else
						{
							if (IS_KNIFE(iWeapon))
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("KNIFE")), fStep += Vars.Misc.iFontHeight + 2.f;

							else if (IS_GRENADE(iWeapon))
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("GRENADE")), fStep += Vars.Misc.iFontHeight + 2.f;

							else if (iWeapon == WEAPON_C4)
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("BOMB")), fStep += Vars.Misc.iFontHeight + 2.f;
						};

						if ((fDuration = pEntity->GetFlashDuration()) > .33f)
						{
							if (fDuration > 2.f)
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Red, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;

							else if (fDuration > 1.f)
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;

							else
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_White, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;
						};

						if (pEntity->IsDefusing())
						{
							DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_CandyBlue, true, XorString("DEFUSING")), fStep += Vars.Misc.iFontHeight + 2.f;

							if (pEntity->HasDefuser())
								DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Cyan, true, XorString("DEFUSER")), fStep += Vars.Misc.iFontHeight + 2.f;
						};
					};
				};
			}

			else
			{
				if (Vars.Misc.bGrenadeTraces && g_pNadeTraj->IsEntityGrenade(pEntity, pClass))
					g_pNadeTraj->AddGrenade(pEntity->m_iId);

				if (Vars.Misc.bDroppedWeaponsName)
				{
					if (pClass->m_ClassID == CAK47 || pClass->m_ClassID == CDEagle || pClass->m_ClassID == CBaseCSGrenade || pClass->m_ClassID == CBaseGrenade || pClass->m_ClassID == CDecoyGrenade || \
						pClass->m_ClassID == CSensorGrenade || pClass->m_ClassID == CFlashbang || pClass->m_ClassID == CSmokeGrenade || pClass->m_ClassID == CHEGrenade || pClass->m_ClassID == CMolotovGrenade || \
						pClass->m_ClassID == CIncendiaryGrenade || !strncmp(pClass->m_pNetworkName, XorString("CWeapon"), 7))
					{
						if (WTS(vecPos, vecScreen))
							DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_White, true, pEntity->GetWeaponName(pEntity->GetItemDefinitionIndex()).c_str());
					}

					else if (pClass->m_ClassID == CC4)
					{
						if (WTS(vecPos, vecScreen))
							DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Orange, true, XorString("C4"));
					}

					else if (pClass->m_ClassID == CHostage)
					{
						if (WTS(vecPos, vecScreen) && (pCollideable = pEntity->GetCollideable()) && WTS(vecPos + Vector(NULL, NULL, pCollideable->OBBMaxs().z), vecScreenEnd))
							DrawStr(INT(vecScreenEnd.x), INT(vecScreenEnd.y - 8.f), g_Green, true, XorString("HOSTAGE"));
					}

					else if (pClass->m_ClassID == CDecoyProjectile)
					{
						if (WTS(vecPos, vecScreen))
							DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Violet, true, XorString("DECOY"));
					};
				};

				if (pClass->m_ClassID == CPlantedC4 && Vars.Misc.bC4BombTimer && WTS(vecPos, vecScreen))
				{
					fTimer = pEntity->GetBombTimer(), xprintf(szBuffer, XorString("C4 [ %.1f ]"), fTimer);

					if (fTimer > 30.f)
						DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_White, true, szBuffer);

					else if (fTimer > 20.f)
						DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Yellow, true, szBuffer);

					else if (fTimer > 10.f)
						DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Orange, true, szBuffer);

					else if (fTimer > 5.f)
						DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Magenta, true, szBuffer);

					else if (fTimer > 0.f)
						DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Red, true, szBuffer);
				};
			};
		};
	}

	else
	{

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

		if (Vars.Visuals.bGlow)
			Glow();

#endif

		for (iId = 0; iId < I::Ents->GetHighestEntityIndex(); iId++)
		{
			if (!(pEntity = I::Ents->GetClEntity(iId)) || pEntity == pOther || pEntity == G::pSelf || pEntity->IsDormant() || (vecPos = pEntity->GetOrigin()).IsZero() || !WTS(vecPos, vecScreen))
				continue;

			if (iId >= 0 && iId <= I::Globals->m_iMaxClients)
				Player(pEntity, vecPos, vecScreen);

			else
				Entity(pEntity, vecPos, vecScreen);
		};
	};
};

VOID CVisuals::Player(Entity_T * pEntity, Vector vecPos, Vector vecScreen)
{
	static INT iTeam = 0, iHealth = 0, iWeapon = 0;
	static Entity_T * pWeapon = NULL;
	static Vector vecScreenEnd;
	static FLOAT fDuration = 0.f, fHeight = 0.f, fStep = 0.f;
	static ICollideable * pCollideable = NULL;
	static CHAR szBuffer[XS_RANDOM_SIZE] = {};

	if ((iHealth = pEntity->GetHealth()) > 0 && (iTeam = pEntity->GetTeam()) > 1 && iTeam < 4 && iTeam != G::iTeam && (pWeapon = pEntity->GetWeapon()) && (iWeapon = pWeapon->GetItemDefinitionIndex()) > 0)
	{
		if (Vars.Visuals.bRadar)
			pEntity->SetSpotted();

		if ((pCollideable = pEntity->GetCollideable()) && WTS(vecPos + Vector(NULL, NULL, pCollideable->OBBMaxs().z), vecScreenEnd))
		{
			fStep = 8.f, fHeight = vecScreen.y - vecScreenEnd.y;

			if (Vars.Visuals.bDrawSkeleton)
				Skeleton(pEntity);

			if (Vars.Visuals.Filter.bEnemyName)
				DrawStr(vecScreenEnd.x, vecScreenEnd.y - fStep, g_White, true, pEntity->GetPlayerName().c_str());

			if (Vars.Visuals.Filter.bEnemyWeapon)
				DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_White, true, pWeapon->GetWeaponName(iWeapon).c_str()), fStep += Vars.Misc.iFontHeight + 2.f;

			if (Vars.Visuals.Filter.bEnemyHealth)
			{
				xprintf(szBuffer, XorString("%d HP"), iHealth);

				if (iHealth > 32)
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_CandyGreen, true, szBuffer), fStep += Vars.Misc.iFontHeight + 2.f;

				else if (iHealth > 16)
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, szBuffer), fStep += Vars.Misc.iFontHeight + 2.f;

				else
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_CandyRed, true, szBuffer), fStep += Vars.Misc.iFontHeight + 2.f;
			};

			if (Vars.Visuals.Filter.bEnemyReloading)
			{
				if (pWeapon->IsGun(iWeapon))
				{
					if (pWeapon->IsEmpty())
						DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("RELOADING")), fStep += Vars.Misc.iFontHeight + 2.f;
				}

				else
				{
					if (IS_KNIFE(iWeapon))
						DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("KNIFE")), fStep += Vars.Misc.iFontHeight + 2.f;

					else if (IS_GRENADE(iWeapon))
						DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("GRENADE")), fStep += Vars.Misc.iFontHeight + 2.f;

					else if (iWeapon == WEAPON_C4)
						DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("BOMB")), fStep += Vars.Misc.iFontHeight + 2.f;
				};
			};

			if (Vars.Visuals.Filter.bEnemyFlashed && (fDuration = pEntity->GetFlashDuration()) > .33f)
			{
				if (fDuration > 2.f)
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Red, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;

				else if (fDuration > 1.f)
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Orange, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;

				else
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_White, true, XorString("FLASHED")), fStep += Vars.Misc.iFontHeight + 2.f;
			};

			if (Vars.Visuals.Filter.bEnemyDefusing && pEntity->IsDefusing())
			{
				DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_CandyBlue, true, XorString("DEFUSING")), fStep += Vars.Misc.iFontHeight + 2.f;

				if (pEntity->HasDefuser())
					DrawStr(vecScreenEnd.x, vecScreenEnd.y + fHeight + fStep, g_Cyan, true, XorString("DEFUSER")), fStep += Vars.Misc.iFontHeight + 2.f;
			};
		};
	};
};

VOID CVisuals::Entity(Entity_T * pEntity, Vector vecPos, Vector vecScreen)
{
	static ClientClass * pClass = NULL;
	static ICollideable * pCollideable = NULL;
	static FLOAT fTimer = 0.f;
	static Vector vecScreenEnd;
	static CHAR szBuffer[XS_RANDOM_SIZE] = {};

	if (!(pClass = pEntity->GetClientClass()))
		return;

	if (Vars.Misc.bGrenadeTraces && g_pNadeTraj->IsEntityGrenade(pEntity, pClass))
		g_pNadeTraj->AddGrenade(pEntity->m_iId);

	if (Vars.Visuals.Filter.bWeapons && (pClass->m_ClassID == CAK47 || pClass->m_ClassID == CDEagle || pClass->m_ClassID == CBaseCSGrenade || pClass->m_ClassID == CBaseGrenade || pClass->m_ClassID == CDecoyGrenade || \
		pClass->m_ClassID == CSensorGrenade || pClass->m_ClassID == CFlashbang || pClass->m_ClassID == CSmokeGrenade || pClass->m_ClassID == CHEGrenade || pClass->m_ClassID == CMolotovGrenade || \
		pClass->m_ClassID == CIncendiaryGrenade || !strncmp(pClass->m_pNetworkName, XorString("CWeapon"), 7)))
		DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_White, true, pEntity->GetWeaponName(pEntity->GetItemDefinitionIndex()).c_str());

	else if (pClass->m_ClassID == CC4 && Vars.Visuals.Filter.bC4Bomb)
		DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Orange, true, XorString("C4"));

	else if (pClass->m_ClassID == CHostage && Vars.Visuals.Filter.bHostages)
	{
		if ((pCollideable = pEntity->GetCollideable()) && WTS(vecPos + Vector(NULL, NULL, pCollideable->OBBMaxs().z), vecScreenEnd))
			DrawStr(INT(vecScreenEnd.x), INT(vecScreenEnd.y - 8.f), g_Green, true, XorString("HOSTAGE"));
	}

	else if (pClass->m_ClassID == CDecoyProjectile && Vars.Visuals.Filter.bDecoy)
		DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Violet, true, XorString("DECOY"));

	else if (pClass->m_ClassID == CPlantedC4 && Vars.Visuals.Filter.bC4Bomb)
	{
		fTimer = pEntity->GetBombTimer(), xprintf(szBuffer, XorString("C4 [ %.1f ]"), fTimer);

		if (fTimer > 30.f)
			DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_White, true, szBuffer);

		else if (fTimer > 20.f)
			DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Yellow, true, szBuffer);

		else if (fTimer > 10.f)
			DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Orange, true, szBuffer);

		else if (fTimer > 5.f)
			DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Magenta, true, szBuffer);

		else if (fTimer > 0.f)
			DrawStr(INT(vecScreen.x), INT(vecScreen.y), g_Red, true, szBuffer);
	};
};

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

VOID CVisuals::Glow(VOID)
{
	static bool bObjManRetrieved = false;
	static INT iIter = 0, iTeam = 0;
	static CGlowObjMan::GlowObjDef_T * pGlowEntity = NULL;
	static CGlowObjMan * pGlowObjMan = NULL;

	if (g_Offsets.m_dwGlowManager)
	{
		if (!bObjManRetrieved)
			pGlowObjMan = (CGlowObjMan *)g_Offsets.m_dwGlowManager, bObjManRetrieved = true;

		if (pGlowObjMan)
		{
			for (iIter = 0; iIter < pGlowObjMan->m_nSize; iIter++)
			{
				if (!(pGlowEntity = &pGlowObjMan->m_pGlowObjDefs[iIter]) || !pGlowEntity->m_pEntity || pGlowEntity->m_nNextFreeSlot != -2 || pGlowEntity->m_pEntity->m_iId < 0 || \
					pGlowEntity->m_pEntity->m_iId > I::Globals->m_iMaxClients || pGlowEntity->m_pEntity->IsDormant() || (iTeam = pGlowEntity->m_pEntity->GetTeam()) < 2 || iTeam > 3 || \
					iTeam == G::iTeam || pGlowEntity->m_pEntity->GetHealth() < 1)
					continue;

				if (pGlowEntity->m_pEntity->IsVisible())
				{
					if (!Vars.Visuals.iVisibleGlowColor)
						pGlowEntity->UnSet();

					else
						pGlowEntity->Set(ClrByOption(Vars.Visuals.iVisibleGlowColor - 1));
				}

				else
				{
					if (!Vars.Visuals.iOccludedGlowColor)
						pGlowEntity->UnSet();

					else
						pGlowEntity->Set(ClrByOption(Vars.Visuals.iOccludedGlowColor - 1));
				};
			};
		};
	};
};

#endif

VOID CVisuals::Skeleton(Entity_T * pEntity)
{
	static Matrix34_T Matrix[128];
	static Vector vecA, vecB;
	static INT iIter = 0;
	static StdHdr_T * pStdio = NULL;
	static StdBone_T * pBone = NULL;
	static Model_T * pModel = NULL;

	if ((pModel = pEntity->GetModel()) && (pStdio = I::MDLInfo->GetStudioModel(pModel)) && pEntity->SetupBones(Matrix))
	{
		for (iIter = 0; iIter < pStdio->iNumBones; iIter++)
		{
			if (!(pBone = pStdio->pBone(iIter)) || !(pBone->iFlags & 256) || pBone->iParent < 0 || \
				!WTS(Vector(Matrix[iIter][0][3], Matrix[iIter][1][3], Matrix[iIter][2][3]), vecA) || \
				!WTS(Vector(Matrix[pBone->iParent][0][3], Matrix[pBone->iParent][1][3], Matrix[pBone->iParent][2][3]), vecB))
				continue;

			DrawLine((INT)vecA.x, (INT)vecA.y, (INT)vecB.x, (INT)vecB.y, ClrByOption(Vars.Misc.iSkeletonColor));
		};
	};
};
