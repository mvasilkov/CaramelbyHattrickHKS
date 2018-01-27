
#pragma once

#include "SDK.hpp"
#include "Platform.hpp"

enum InitReturnVal_t : INT {
	INIT_FAILED = 0,
	INIT_OK,
	INIT_LAST_VAL,
};

enum AppSystemTier_t : INT {
	APP_SYSTEM_TIER0 = 0,
	APP_SYSTEM_TIER1,
	APP_SYSTEM_TIER2,
	APP_SYSTEM_TIER3,
	APP_SYSTEM_TIER_OTHER,
};

struct AppSystemInfo_t;

class IAppSystem {
public:

	virtual bool Connect(CreateInterfaceFn) = 0;
	virtual void Disconnect() = 0;
	virtual void * QueryInterface(const char *) = 0;
	virtual InitReturnVal_t Init() = 0;
	virtual void Shutdown() = 0;
	virtual const AppSystemInfo_t * GetDependencies() = 0;
	virtual AppSystemTier_t GetTier() = 0;
	virtual void Reconnect(CreateInterfaceFn, const char *) = 0;
	virtual bool IsSingleton() = 0;
};
