
#pragma once

#include "SDK.hpp"

using FrameStageNotify_t = VOID(__thiscall *) (IBaseClientDLL *, ClientFrameStage_t);
using FireEventClientSide_t = bool(__thiscall *) (IGameEventManager2 *, IGameEvent *);

VOID __fastcall FrameStageNotify(IBaseClientDLL *, VOID *, ClientFrameStage_t);
bool __fastcall FireEventClientSide(IGameEventManager2 *, VOID *, IGameEvent *);
VOID __cdecl SequenceProxyFn(CONST CRecvProxyData *, VOID *, VOID *);
