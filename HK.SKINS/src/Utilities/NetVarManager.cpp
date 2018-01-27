
#include "SDK.hpp"
#include "NetVarManager.hpp"

NetVarManager::NetVarManager(void)
{
	for (auto pClass = g_client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		if (pClass->m_pRecvTable)
			DumpRecursive(pClass->m_pNetworkName, pClass->m_pRecvTable, 0);
};

void NetVarManager::DumpRecursive(const char * base_class, RecvTable * table, uint16_t offset)
{
	for (auto i = 0; i < table->m_nProps; ++i)
	{
		auto prop_ptr = &table->m_pProps[i];
		if (!prop_ptr || isdigit(prop_ptr->m_pVarName[0]) || !strcmp(prop_ptr->m_pVarName, XorString("baseclass")))
			continue;

		if (prop_ptr->m_RecvType == DPT_DataTable && prop_ptr->m_pDataTable && prop_ptr->m_pDataTable->m_pNetTableName[0] == 'D')
			DumpRecursive(base_class, prop_ptr->m_pDataTable, offset + prop_ptr->m_Offset);

		m_props[FnvHash(static_cast < const char * > (((string(base_class + string(XorString("->")) + prop_ptr->m_pVarName)).c_str())))] = { prop_ptr, uint16_t(offset + prop_ptr->m_Offset) };
	};
};
