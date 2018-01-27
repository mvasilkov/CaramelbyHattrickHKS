
#pragma once

#include "SDK.hpp"
#include <Psapi.h>
#include "Hattrick.h"

#define ISINRANGE(X, A, B) (X >= A && X <= B)

#define GETBITS(X) (ISINRANGE((X & (~0x00000020)), 'A', 'F') ? ((X & (~0x00000020)) - 'A' + 0x0000000A) : (ISINRANGE(X, '0', '9') ? X - '0' : 0x00000000))
#define GETBYTE(X) (GETBITS(X[0]) << 4 | GETBITS(X[1]))

using CreateInterfaceFn = void * (*) (const char *, int *);

extern Hattrick g_Hattrick;

FORCEINLINE DWORD FindPattern(string mod, string pat)
{
	static Hattrick::ModuleTuple * pModule = NULL;
	static CONST CHAR * pszPattern = NULL;
	static DWORD dwFirst = (DWORD)0, dwStart = (DWORD)0, dwEnd = (DWORD)0, dwCur = (DWORD)0;
	if (!(pModule = g_Hattrick.ProcessModule(mod))) return DWORD(0);
	pszPattern = pat.c_str(), dwFirst = (DWORD)0, dwStart = (DWORD)pModule->m_pHandle, dwEnd = dwStart + pModule->m_Info.SizeOfImage;
	for (dwCur = dwStart; dwCur < dwEnd; dwCur++)
	{
		if (*pszPattern == '\0') return dwFirst;
		if (*(PBYTE)pszPattern == '\?' || *(PBYTE)dwCur == GETBYTE(pszPattern))
		{
			if (dwFirst == DWORD(0)) dwFirst = dwCur;
			if (pszPattern[2] == '\0') return dwFirst;
			if (*(PWORD)pszPattern == '\?\?' || *(PBYTE)pszPattern != '\?') pszPattern += 3;
			else pszPattern += 2;
		}
		else pszPattern = pat.c_str(), dwFirst = (DWORD)0;
	};
	return NULL;
};

bool IsCodePtr(void *);
