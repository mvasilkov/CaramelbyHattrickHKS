
#pragma once

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

#include <Windows.h>

#include <vector>
#include <string>

using namespace std;

#include "XorString.h"

class Hattrick
{
public:

	FORCEINLINE Hattrick(VOID)
	{
		Clear();
	};

	FORCEINLINE ~Hattrick(VOID)
	{
		Clear();
	};

	FORCEINLINE CONST VOID Clear(VOID)
	{
		m_vecParsedModules.clear();
	};

	class ModuleTuple
	{
	public:

		HMODULE m_pHandle;
		MODULEINFO m_Info;
		PVOID m_pCreateInterfaceAddress;
		string m_InsensitiveName;

		FORCEINLINE ModuleTuple(VOID)
		{
			Clear();
		};

		FORCEINLINE ~ModuleTuple(VOID)
		{
			Clear();
		};

		FORCEINLINE CONST VOID Clear(VOID)
		{
			ZeroMemory(&m_Info, sizeof m_Info), m_InsensitiveName.clear(), m_pHandle = NULL, m_pCreateInterfaceAddress = NULL;
		};
	};

	vector < ModuleTuple > m_vecParsedModules;

	FORCEINLINE ModuleTuple * ProcessModule(string ModuleName)
	{
		static ModuleTuple m_Module;
		static vector < ModuleTuple > ::iterator m_Object;

		for (m_Object = m_vecParsedModules.begin(); m_Object != m_vecParsedModules.end(); m_Object++)
		{
			if (_stricmp((*m_Object).m_InsensitiveName.c_str(), ModuleName.c_str()) == 0)
				return &(*m_Object);
		};

		m_Module.m_InsensitiveName.assign(ModuleName);
		if (!(m_Module.m_pHandle = GetModuleHandle(ModuleName.c_str())))
			return NULL;

		ZeroMemory(&m_Module.m_Info, sizeof m_Module.m_Info);
		if (!GetModuleInformation(GetCurrentProcess(), m_Module.m_pHandle, &m_Module.m_Info, sizeof m_Module.m_Info))
			return NULL;

		m_Module.m_pCreateInterfaceAddress = GetProcAddress(m_Module.m_pHandle, XorString("CreateInterface")), m_vecParsedModules.push_back(m_Module);
		return &m_Module;
	};
};
