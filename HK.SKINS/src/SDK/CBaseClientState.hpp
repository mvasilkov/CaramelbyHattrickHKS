
#pragma once

#include "SDK.hpp"

class CBaseClientState
{
public:

	VOID ForceFullUpdate(VOID)
	{
		*reinterpret_cast < PINT > (reinterpret_cast < uintptr_t > (this) + 0x00000174) = -1;
	};
};

extern CBaseClientState ** g_client_state;
