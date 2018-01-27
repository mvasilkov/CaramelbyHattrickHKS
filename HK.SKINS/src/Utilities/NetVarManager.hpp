
#pragma once

#include "SDK.hpp"
#include "DataTable.hpp"
#include "FnvHash.hpp"

class NetVarManager
{
public:

	struct StoredPropData
	{
		RecvProp * prop_ptr;
		uint16_t class_relative_offset;
	};

	FORCEINLINE static NetVarManager & Get(VOID)
	{
		static NetVarManager instance;
		return instance;
	};

	FORCEINLINE USHORT GetOffset(UINT hash) { return m_props[hash].class_relative_offset; };
	FORCEINLINE RecvProp * GetPropPtr(UINT hash) { return m_props[hash].prop_ptr; };

	FORCEINLINE __declspec (noinline) static uint16_t GetOffsetByHash(fnv_t hash)
	{
		return Get().GetOffset(hash);
	};

	NetVarManager();
	VOID DumpRecursive(const char * base_class, RecvTable * table, uint16_t offset);
	map <fnv_t, StoredPropData> m_props;
};

#define PNETVAR(funcname, type, netvarname) type * funcname() \
{ \
	static constexpr CONST fnv_t CONST hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if (!offset) offset = NetVarManager::GetOffsetByHash(hash); \
	return reinterpret_cast < type * > (uintptr_t(this) + offset); \
}

#define NETVAR(funcname, type, netvarname) type & funcname() \
{ \
	static constexpr CONST fnv_t CONST hash = FnvHash(netvarname); \
	static uint16_t offset = 0; \
	if (!offset) offset = NetVarManager::GetOffsetByHash(hash); \
	return *reinterpret_cast < type * > (uintptr_t(this) + offset); \
}

#define NETPROP(funcname, netvarname) static RecvProp * funcname() \
{ \
	static constexpr CONST fnv_t CONST hash = FnvHash(netvarname); \
	static auto prop_ptr = NetVarManager::Get().GetPropPtr(hash); \
	return prop_ptr; \
}
