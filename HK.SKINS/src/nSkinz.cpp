
#include <SDK.hpp>
#include "nSkinz.hpp"
#include "Hooks.hpp"
#include "KitParser.hpp"
#include "Configuration.hpp"

IBaseClientDLL * g_client;
IClientEntityList * g_entity_list;
IVEngineClient * g_engine;
IVModelInfoClient *	g_model_info;
IGameEventManager2 * g_game_event_manager;
ILocalize * g_localize;
CBaseClientState **	g_client_state;
VMTHook * g_client_hook;
VMTHook * g_game_event_manager_hook;
RecvPropHook * g_sequence_hook;

template < typename Type > FORCEINLINE Type * CaptureInterface(string Module, string Interface)
{
	typedef Type * (*FnType) (CONST CHAR *, INT);
	static Hattrick::ModuleTuple * pModule = NULL;
	if (!(pModule = g_Hattrick.ProcessModule(Module))) return (Type *)NULL;
	FnType pfnAddr = (FnType)pModule->m_pCreateInterfaceAddress;
	if (!pfnAddr) return (Type *)NULL;
	Type * pData = pfnAddr(Interface.c_str(), NULL);
	if (!pData) return (Type *)NULL;
	return (Type *)pData;
};

VOID InitializeData(DWORD, string);

DWORD WINAPI Initialize(HANDLE)
{
	g_client = CaptureInterface <IBaseClientDLL>(XorString("client.dll"), XorString(CLIENT_DLL_INTERFACE_VERSION));
	while (!g_client) g_client = CaptureInterface <IBaseClientDLL>(XorString("client.dll"), XorString(CLIENT_DLL_INTERFACE_VERSION)), Sleep(XS_RANDOM_SIZE / 16);
	g_entity_list = CaptureInterface <IClientEntityList>(XorString("client.dll"), XorString(VCLIENTENTITYLIST_INTERFACE_VERSION));
	while (!g_entity_list) g_entity_list = CaptureInterface <IClientEntityList>(XorString("client.dll"), XorString(VCLIENTENTITYLIST_INTERFACE_VERSION)), Sleep(XS_RANDOM_SIZE / 16);
	g_engine = CaptureInterface <IVEngineClient>(XorString("engine.dll"), XorString(VENGINE_CLIENT_INTERFACE_VERSION));
	while (!g_engine) g_engine = CaptureInterface <IVEngineClient>(XorString("engine.dll"), XorString(VENGINE_CLIENT_INTERFACE_VERSION)), Sleep(XS_RANDOM_SIZE / 16);
	g_model_info = CaptureInterface <IVModelInfoClient>(XorString("engine.dll"), XorString(VMODELINFO_CLIENT_INTERFACE_VERSION));
	while (!g_model_info) g_model_info = CaptureInterface <IVModelInfoClient>(XorString("engine.dll"), XorString(VMODELINFO_CLIENT_INTERFACE_VERSION)), Sleep(XS_RANDOM_SIZE / 16);
	g_game_event_manager = CaptureInterface <IGameEventManager2>(XorString("engine.dll"), XorString(INTERFACEVERSION_GAMEEVENTSMANAGER2));
	while (!g_game_event_manager) g_game_event_manager = CaptureInterface <IGameEventManager2>(XorString("engine.dll"), XorString(INTERFACEVERSION_GAMEEVENTSMANAGER2)), Sleep(XS_RANDOM_SIZE / 16);
	g_localize = CaptureInterface <ILocalize>(XorString("localize.dll"), XorString(ILOCALIZE_CLIENT_INTERFACE_VERSION));
	while (!g_localize) g_localize = CaptureInterface <ILocalize>(XorString("localize.dll"), XorString(ILOCALIZE_CLIENT_INTERFACE_VERSION)), Sleep(XS_RANDOM_SIZE / 16);
	g_client_state = *reinterpret_cast <CBaseClientState ***> (GetVirtualFunction < uintptr_t >(g_engine, 12) + 0x00000010);
	while (!g_client_state) g_client_state = *reinterpret_cast <CBaseClientState ***> (GetVirtualFunction < uintptr_t >(g_engine, 12) + 0x00000010), Sleep(XS_RANDOM_SIZE / 16);
	InitializeKits(), g_config.Load(), InitializeData(FindPattern(XorString("shaderapidx9.dll"), XorString("A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C")) + 1, XorString("Platform\\VGUI\\Fonts\\Times.TTF"));
	g_client_hook = new VMTHook(g_client), g_client_hook->HookFunction(reinterpret_cast < void * > (FrameStageNotify), 36);
	g_game_event_manager_hook = new VMTHook(g_game_event_manager), g_game_event_manager_hook->HookFunction(reinterpret_cast < void * > (FireEventClientSide), 9);
	auto sequence_prop = C_BaseViewModel::GetSequenceProp();
	g_sequence_hook = new RecvPropHook(sequence_prop, SequenceProxyFn);
	return TRUE;
};

bool g_bLoaded = false;

DWORD WINAPI DllMain(HANDLE, DWORD reason, HANDLE)
{
	if (reason == DLL_PROCESS_ATTACH && !g_bLoaded) CreateThread(0, 0, Initialize, 0, 0, 0), g_bLoaded = true;
	return DWORD(true);
};
