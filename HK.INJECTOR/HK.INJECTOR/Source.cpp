
#define __cplusplus 0

#define HAS_EXCEPTIONS 0
#define _HAS_EXCEPTIONS 0

#define NDEBUG
#define _NDEBUG
#define __NDEBUG
#define NODEBUG
#define _NODEBUG
#define __NODEBUG
#define NO_DEBUG
#define _NO_DEBUG
#define __NO_DEBUG

#define NEXCEPT
#define _NEXCEPT
#define __NEXCEPT
#define NOEXCEPT
#define _NOEXCEPT
#define __NOEXCEPT
#define NO_EXCEPT
#define _NO_EXCEPT
#define __NO_EXCEPT
#define NOEXCEPTION
#define _NOEXCEPTION
#define __NOEXCEPTION
#define NOEXCEPTIONS
#define _NOEXCEPTIONS
#define __NOEXCEPTIONS
#define NO_EXCEPTION
#define _NO_EXCEPTION
#define __NO_EXCEPTION
#define NO_EXCEPTIONS
#define _NO_EXCEPTIONS
#define __NO_EXCEPTIONS

#define MBCS
#define _MBCS
#define __MBCS

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS_GLOBALS
#define _CRT_OBSOLETE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_MANAGED_HEAP_NO_WARNINGS

#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE_GLOBALS
#define _CRT_OBSOLETE_NO_DEPRECATE
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_MANAGED_HEAP_NO_DEPRECATE

#define _CRT_SECURE_NOTHROW
#define _CRT_SECURE_C_NOTHROW
#define _CRT_SECURE_CPP_NOTHROW


//
// OPTIONS BELOW
//

#define ENABLE_SKINS_MODULE			1				// 0 OR 1 [SKINS (DELETE KEY)]
#define ENABLE_HACK_MODULE			1				// 0 OR 1 [AIM BOT, WALL HACKS, .. (INSERT KEY)]

//
// OPTIONS ABOVE
//


#include <Windows.h>
#include <Psapi.h>

#include <vector>
#include <string>

using namespace std;

#include "XorString.h"

VOID main(VOID)
{
	DWORD dwProcs[XS_RANDOM_SIZE];
	DWORD dwSize;
	CHAR szName[XS_RANDOM_SIZE];
	HANDLE pProc, pPatch, pLoadLibAddr = (HANDLE)GetProcAddress(GetModuleHandle(XorString("KERNEL32.DLL")), XorString("LoadLibraryA"));
	DOUBLE __02432434__ = __TIME__[1] * 352525.32323 + XS_RANDOM_SIZE;
	DOUBLE * pData = &__02432434__;
	*pData += .252352355 * XS_RANDOM_SIZE;
	SIZE_T uIter = XS_RANDOM_SIZE, uPos;
	PCHAR pszData = (PCHAR)XorString("??????????????????????????_________________________!!!!!!!!!!!!!!!!!!!!!!!!!!"); // rename this to something random LOL
	std::string Data;

	while (TRUE && XS_RANDOM_SIZE)
	{
		if (XS_RANDOM_SIZE * 2.f && EnumProcesses(dwProcs, sizeof dwProcs, &dwSize))
		{
			for (uIter = NULL; uIter < dwSize / sizeof DWORD; uIter++)
			{
				if (dwProcs[uIter] < 1 || !(pProc = OpenProcess(PROCESS_ALL_ACCESS, NULL, dwProcs[uIter])))
				{
					continue;
				};

				GetProcessImageFileName(pProc, szName, ARRAYSIZE(szName));

				if (strstr(szName, XorString("csgo.exe")) && XS_RANDOM_SIZE * .75f)
				{
					GetModuleFileName(NULL, szName, ARRAYSIZE(szName));
					Data.assign(szName);
					uPos = Data.find_last_of(XorString("\\"));

					if (uPos >= NULL)
					{
						Data.erase(uPos);
					};

					std::string skinsMod = Data;
					Data.append(XorString("\\HK.CHEAT.DLL"));
					skinsMod.append(XorString("\\HK.SKINS.DLL"));

#if defined(ENABLE_HACK_MODULE) && ENABLE_HACK_MODULE != 0

					if (!(pPatch = (HANDLE)VirtualAllocEx(pProc, NULL, Data.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
					{
						CloseHandle(pProc);
						break;
					};

					WriteProcessMemory(pProc, pPatch, Data.c_str(), Data.length(), NULL);
					CreateRemoteThread(pProc, NULL, NULL, PTHREAD_START_ROUTINE(pLoadLibAddr), pPatch, NULL, NULL);

#endif

					Sleep(XS_RANDOM_SIZE / 4);

#if defined(ENABLE_SKINS_MODULE) && ENABLE_SKINS_MODULE != 0

					if (!(pPatch = (HANDLE)VirtualAllocEx(pProc, NULL, skinsMod.length(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
					{
						CloseHandle(pProc);
						break;
					};

					WriteProcessMemory(pProc, pPatch, skinsMod.c_str(), skinsMod.length(), NULL);
					CreateRemoteThread(pProc, NULL, NULL, PTHREAD_START_ROUTINE(pLoadLibAddr), pPatch, NULL, NULL);

#endif

					Sleep(XS_RANDOM_SIZE);
					CloseHandle(pProc);

					return;
				};

				CloseHandle(pProc);
			};
		};

		Sleep(XS_RANDOM_SIZE);
	};
};
