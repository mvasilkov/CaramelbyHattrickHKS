
#include "SDK.hpp"
#include "Platform.hpp"
#include "Hooks.hpp"
#include "nSkinz.hpp"

enum ESequence : INT {
	SEQUENCE_DEFAULT_DRAW = 0,
	SEQUENCE_DEFAULT_IDLE1 = 1,
	SEQUENCE_DEFAULT_IDLE2 = 2,
	SEQUENCE_DEFAULT_LIGHT_MISS1 = 3,
	SEQUENCE_DEFAULT_LIGHT_MISS2 = 4,
	SEQUENCE_DEFAULT_HEAVY_MISS1 = 9,
	SEQUENCE_DEFAULT_HEAVY_HIT1 = 10,
	SEQUENCE_DEFAULT_HEAVY_BACKSTAB = 11,
	SEQUENCE_DEFAULT_LOOKAT01 = 12,
	SEQUENCE_BUTTERFLY_DRAW = 0,
	SEQUENCE_BUTTERFLY_DRAW2 = 1,
	SEQUENCE_BUTTERFLY_LOOKAT01 = 13,
	SEQUENCE_BUTTERFLY_LOOKAT03 = 15,
	SEQUENCE_FALCHION_IDLE1 = 1,
	SEQUENCE_FALCHION_HEAVY_MISS1 = 8,
	SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP = 9,
	SEQUENCE_FALCHION_LOOKAT01 = 12,
	SEQUENCE_FALCHION_LOOKAT02 = 13,
	SEQUENCE_DAGGERS_IDLE1 = 1,
	SEQUENCE_DAGGERS_LIGHT_MISS1 = 2,
	SEQUENCE_DAGGERS_LIGHT_MISS5 = 6,
	SEQUENCE_DAGGERS_HEAVY_MISS2 = 11,
	SEQUENCE_DAGGERS_HEAVY_MISS1 = 12,
	SEQUENCE_BOWIE_IDLE1 = 1,
};

FORCEINLINE CONST INT CONST RandomSequence(int low, int high)
{
	return rand() % (high - low + 1) + low;
};

static CONST unordered_map < string, int(*) (int) > CONST animation_fix_map
{
	{
		XorString("models/weapons/v_knife_butterfly.mdl"), [](int sequence) -> int
{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_DRAW: return RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
	case SEQUENCE_DEFAULT_LOOKAT01: return RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
	default: return sequence + 1;
	};
}
	},
{
	XorString("models/weapons/v_knife_falchion_advanced.mdl"), [](int sequence) -> int
{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_IDLE2: return SEQUENCE_FALCHION_IDLE1;
	case SEQUENCE_DEFAULT_HEAVY_MISS1: return RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
	case SEQUENCE_DEFAULT_LOOKAT01: return RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
	case SEQUENCE_DEFAULT_DRAW: case SEQUENCE_DEFAULT_IDLE1: return sequence;
	default: return sequence - 1;
	};
}
},
{
	XorString("models/weapons/v_knife_push.mdl"), [](int sequence) -> int
{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_IDLE2: return SEQUENCE_DAGGERS_IDLE1;
	case SEQUENCE_DEFAULT_LIGHT_MISS1: case SEQUENCE_DEFAULT_LIGHT_MISS2: return RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
	case SEQUENCE_DEFAULT_HEAVY_MISS1: return RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
	case SEQUENCE_DEFAULT_HEAVY_HIT1: case SEQUENCE_DEFAULT_HEAVY_BACKSTAB: case SEQUENCE_DEFAULT_LOOKAT01: return sequence + 3;
	case SEQUENCE_DEFAULT_DRAW: case SEQUENCE_DEFAULT_IDLE1: return sequence;
	default: return sequence + 2;
	};
}
},
{
	XorString("models/weapons/v_knife_survival_bowie.mdl"), [](int sequence) -> int
{
	switch (sequence)
	{
	case SEQUENCE_DEFAULT_DRAW: case SEQUENCE_DEFAULT_IDLE1: return sequence;
	case SEQUENCE_DEFAULT_IDLE2: return SEQUENCE_BOWIE_IDLE1;
	default: return sequence - 1;
	};
}
}
};

VOID __cdecl SequenceProxyFn(CONST CRecvProxyData * proxy_data_const, VOID * entity, VOID * output)
{
	static auto orig = g_sequence_hook->GetOriginalFunction();
	static C_BasePlayer * local = 0;
	static CRecvProxyData * proxy_data = 0;
	static C_BaseViewModel * view_model = 0;
	static CBaseHandle owner_handle = 0;
	static int local_index = 0;
	static const char * name = 0;
	local_index = g_engine->GetLocalPlayer(), local = static_cast < C_BasePlayer * > (g_entity_list->GetClientEntity(local_index));
	if (!local || local->GetLifeState() != LifeState::ALIVE) return orig(proxy_data_const, entity, output);
	proxy_data = const_cast < CRecvProxyData * > (proxy_data_const), view_model = static_cast < C_BaseViewModel * > (entity);
	if (view_model)
	{
		owner_handle = view_model->GetOwner();
		if (owner_handle && owner_handle != INVALID_EHANDLE_INDEX && static_cast < C_BasePlayer * > (g_entity_list->GetClientEntityFromHandle(owner_handle)) == local)
		{
			name = g_model_info->GetModelName(g_model_info->GetModel(view_model->GetModelIndex()));
			if (animation_fix_map.count(name)) proxy_data->m_Value.m_Int = animation_fix_map.at(name) (proxy_data->m_Value.m_Int);
		};
	};
	orig(proxy_data, entity, output);
};
