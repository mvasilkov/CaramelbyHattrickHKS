
#include "SDK.hpp"
#include "Platform.hpp"
#include "Hattrick.h"

Hattrick g_Hattrick;

bool IsCodePtr(void * ptr)
{
	static constexpr const DWORD const protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
	static MEMORY_BASIC_INFORMATION X;
	VirtualQuery(ptr, &X, sizeof X);
	return X.Type && !(X.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && X.Protect & protect_flags;
};
