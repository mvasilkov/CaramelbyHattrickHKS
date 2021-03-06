
#pragma once

#include "SDK.hpp"
#include "IClientEntity.hpp"
#include "NetVarManager.hpp"
#include "Virtuals.hpp"

enum class LifeState : INT {
	ALIVE = 0,
	DYING,
	DEAD,
	RESPAWNABLE,
	DISCARDBODY,
};

class C_BaseEntity : public IClientEntity
{
public:

	NETVAR(GetModelIndex, unsigned, "CBaseEntity->m_nModelIndex")

		FORCEINLINE CONST VOID CONST SetModelIndex(INT X)
	{
		GetVirtualFunction < VOID(__thiscall *) (C_BaseEntity *, int) >(this, 75) (this, X);
	};
};

class C_BaseCombatCharacter : public C_BaseEntity
{
public:

	PNETVAR(GetWeapons, CBaseHandle, "CBaseCombatCharacter->m_hMyWeapons")
		PNETVAR(GetWearables, CBaseHandle, "CBaseCombatCharacter->m_hMyWearables")
};

class C_BasePlayer : public C_BaseCombatCharacter
{
public:

	NETVAR(GetLifeState, LifeState, "CBasePlayer->m_lifeState")
		NETVAR(GetViewModel, CBaseHandle, "CBasePlayer->m_hViewModel[0]")
};

class C_BaseCombatWeapon : public C_BaseEntity
{
public:

	NETVAR(GetViewModelIndex, int, "CBaseCombatWeapon->m_iViewModelIndex")
};

class C_BaseAttributableItem : public C_BaseCombatWeapon
{
private:

	using STR32 = char[32];

public:

	NETVAR(GetAccountID, int, "CBaseAttributableItem->m_iAccountID")
		NETVAR(GetItemDefinitionIndex, int, "CBaseAttributableItem->m_iItemDefinitionIndex")
		NETVAR(GetItemIDHigh, int, "CBaseAttributableItem->m_iItemIDHigh")
		NETVAR(GetEntityQuality, int, "CBaseAttributableItem->m_iEntityQuality")
		NETVAR(GetCustomName, STR32, "CBaseAttributableItem->m_szCustomName")
		NETVAR(GetFallbackPaintKit, unsigned, "CBaseAttributableItem->m_nFallbackPaintKit")
		NETVAR(GetFallbackSeed, unsigned, "CBaseAttributableItem->m_nFallbackSeed")
		NETVAR(GetFallbackWear, float, "CBaseAttributableItem->m_flFallbackWear")
		NETVAR(GetFallbackStatTrak, unsigned, "CBaseAttributableItem->m_nFallbackStatTrak")
};

class C_BaseViewModel : public C_BaseEntity
{
public:

	NETVAR(GetOwner, CBaseHandle, "CBaseViewModel->m_hOwner")
		NETVAR(GetWeapon, CBaseHandle, "CBaseViewModel->m_hWeapon")
		NETPROP(GetSequenceProp, "CBaseViewModel->m_nSequence")
};
