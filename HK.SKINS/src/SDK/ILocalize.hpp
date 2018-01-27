
#pragma once

#include "SDK.hpp"
#include "IAppSystem.hpp"

using LocalizeStringIndex_t = unsigned;

class ILocalizeTextQuery
{
public:

	virtual int ComputeTextWidth(const wchar_t *) = 0;
};

class ILocalizationChangeCallback
{
public:

	virtual void OnLocalizationChanged() = 0;
};

class ILocalize : public IAppSystem
{
public:

	virtual bool AddFile(const char *, const char *, bool) = 0;
	virtual void RemoveAll(VOID) = 0;
	virtual wchar_t * Find(const char *) = 0;
	virtual const wchar_t * FindSafe(const char *) = 0;
	virtual int ConvertANSIToUnicode(const char *, wchar_t *, int) = 0;
	virtual int ConvertUnicodeToANSI(const wchar_t *, char *, int) = 0;
	virtual LocalizeStringIndex_t FindIndex(const char *) = 0;
	virtual void ConstructString(wchar_t *, int, const wchar_t *, int, ...) = 0;
	virtual const char * GetNameByIndex(LocalizeStringIndex_t) = 0;
	virtual wchar_t * GetValueByIndex(LocalizeStringIndex_t) = 0;
};

#define ILOCALIZE_CLIENT_INTERFACE_VERSION "Localize_001"

extern ILocalize * g_localize;
