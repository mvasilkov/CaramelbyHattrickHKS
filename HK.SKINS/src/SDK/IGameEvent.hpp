
#pragma once

#include <SDK.hpp>

class IGameEvent
{
public:

	virtual ~IGameEvent() { };
	virtual const char * GetName() const = 0;
	virtual bool IsReliable() const = 0;
	virtual bool IsLocal() const = 0;
	virtual bool IsEmpty(const char *) = 0;
	virtual bool GetBool(const char *, bool) = 0;
	virtual int GetInt(const char *, int) = 0;
	virtual uint64_t GetUint64(const char *, uint64_t) = 0;
	virtual float GetFloat(const char *, float) = 0;
	virtual const char * GetString(const char *, const char *) = 0;
	virtual const wchar_t * GetWString(const char *, const wchar_t *) = 0;
	virtual const void * GetPtr(const char *, const void *) = 0;
	virtual void SetBool(const char *, bool) = 0;
	virtual void SetInt(const char *, int) = 0;
	virtual void SetUint64(const char *, uint64_t) = 0;
	virtual void SetFloat(const char *, float) = 0;
	virtual void SetString(const char *, const char *) = 0;
	virtual void SetWString(const char *, const wchar_t *) = 0;
	virtual void SetPtr(const char *, const void *) = 0;
};
