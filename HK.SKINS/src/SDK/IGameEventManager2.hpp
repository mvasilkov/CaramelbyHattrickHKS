
#pragma once

#include "SDK.hpp"

struct bf_read;
struct bf_write;

class IGameEvent;
class KeyValues;

class IGameEventListener2
{
public:

	virtual ~IGameEventListener2() { };
	virtual void FireGameEvent(IGameEvent *) = 0;
	virtual int GetEventDebugID() = 0;
};

class IGameEventManager2
{
public:

	virtual ~IGameEventManager2() { };

	virtual int LoadEventsFromFile(const char *) = 0;
	virtual void Reset() = 0;
	virtual bool AddListener(IGameEventListener2 *, const char *, bool) = 0;
	virtual bool FindListener(IGameEventListener2 *, const char *) = 0;
	virtual void RemoveListener(IGameEventListener2 *) = 0;
	virtual void AddListenerGlobal(IGameEventListener2 *, bool) = 0;
	virtual IGameEvent * CreateEvent(const char *, bool, int *) = 0;
	virtual bool FireEvent(IGameEvent *, bool) = 0;
	virtual bool FireEventClientSide(IGameEvent *) = 0;
	virtual IGameEvent * DuplicateEvent(IGameEvent *) = 0;
	virtual void FreeEvent(IGameEvent *) = 0;
	virtual bool SerializeEvent(IGameEvent *, bf_write *) = 0;
	virtual IGameEvent * UnserializeEvent(bf_read *) = 0;
	virtual KeyValues * GetEventDataTypes(IGameEvent *) = 0;
};

#define INTERFACEVERSION_GAMEEVENTSMANAGER2 "GAMEEVENTSMANAGER002"

extern IGameEventManager2 * g_game_event_manager;
