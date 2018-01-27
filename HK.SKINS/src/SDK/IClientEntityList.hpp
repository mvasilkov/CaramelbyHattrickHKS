
#pragma once

#include "SDK.hpp"

class IClientEntity;
class IClientUnknown;
class IClientNetworkable;

class IClientEntityList
{
public:

	virtual IClientNetworkable * GetClientNetworkable(int) = 0;
	virtual IClientNetworkable * GetClientNetworkableFromHandle(CBaseHandle) = 0;
	virtual IClientUnknown * GetClientUnknownFromHandle(CBaseHandle) = 0;
	virtual IClientEntity * GetClientEntity(int) = 0;
	virtual IClientEntity * GetClientEntityFromHandle(CBaseHandle) = 0;
	virtual int NumberOfEntities(bool) = 0;
	virtual int GetHighestEntityIndex() = 0;
	virtual void SetMaxEntities(int) = 0;
	virtual int GetMaxEntities() = 0;
};

#define VCLIENTENTITYLIST_INTERFACE_VERSION "VClientEntityList003"

extern IClientEntityList * g_entity_list;
