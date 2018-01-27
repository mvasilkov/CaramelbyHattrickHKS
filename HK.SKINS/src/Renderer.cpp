
#include "SDK.hpp"
#include <d3d9.h>
#include <imgui.h>
#include <imgui_impl_dx9.h>

CONST VOID CONST DrawGUI(VOID);
using EndScene_t = HRESULT(__stdcall *) (IDirect3DDevice9 *);
using Reset_t = HRESULT(__stdcall *) (IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
HWND s_hwnd;
WNDPROC s_original_wnd_proc = nullptr;
VMTHook * s_d3d_hook;
bool s_ready = false, s_active = false;

HRESULT __stdcall hkReset(IDirect3DDevice9 * thisptr, D3DPRESENT_PARAMETERS * params)
{
	static auto original_fn = s_d3d_hook->GetOriginalFunction<Reset_t>(16);
	if (!s_ready) return original_fn(thisptr, params);
	ImGui_ImplDX9_InvalidateDeviceObjects();
	auto result = original_fn(thisptr, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
};

HRESULT __stdcall hkEndScene(IDirect3DDevice9 * thisptr)
{
	static auto original_fn = s_d3d_hook->GetOriginalFunction<EndScene_t>(42);
	static auto mouse_enabled = true;
	if (s_active)
	{
		if (mouse_enabled) g_engine->ClientCmd_Unrestricted(XorString("cl_mouseenable 0")), mouse_enabled = false;
		ImGui::GetIO().MouseDrawCursor = true;
		ImGui_ImplDX9_NewFrame();
		DrawGUI();
		ImGui::Render();
	}
	else
	{
		if (!mouse_enabled) g_engine->ClientCmd_Unrestricted(XorString("cl_mouseenable 1")), mouse_enabled = true;
	};
	return original_fn(thisptr);
};

bool HandleInputMessage(UINT message_type, WPARAM w_param, LPARAM l_param)
{
	if (!s_ready) return false;
	if (message_type == WM_KEYUP && w_param == VK_DELETE) s_active = !s_active;
	if (s_active) return ImGui_ImplWin32_WndProcHandler(s_hwnd, message_type, w_param, l_param) != 0;
	return false;
};

LRESULT __stdcall hkWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (HandleInputMessage(message, wParam, lParam)) return true;
	return CallWindowProc(s_original_wnd_proc, hWnd, message, wParam, lParam);
};

VOID InitializeData(DWORD dwAddr, string Font)
{
	if (s_ready) return;
	s_hwnd = FindWindow(XorString("Valve001"), nullptr);
	while (!s_hwnd) s_hwnd = FindWindow(XorString("Valve001"), nullptr), Sleep(XS_RANDOM_SIZE / 16);
	s_original_wnd_proc = reinterpret_cast <WNDPROC> (SetWindowLongPtr(s_hwnd, GWLP_WNDPROC, LONG_PTR(hkWndProc)));
	auto d3d_device = **reinterpret_cast <IDirect3DDevice9 ***> ((dwAddr));
	s_d3d_hook = new VMTHook(d3d_device);
	s_d3d_hook->HookFunction(reinterpret_cast <void *> (hkReset), 16);
	s_d3d_hook->HookFunction(reinterpret_cast <void *> (hkEndScene), 42);
	if (ImGui_ImplDX9_Init(s_hwnd, d3d_device)) s_ready = true;
	auto & style = ImGui::GetStyle();
	style.GrabRounding = 0.f;
	style.WindowRounding = 0.f;
	style.ScrollbarRounding = 0.f;
	style.FrameRounding = 0.f;
	style.WindowTitleAlign = ImVec2(.5f, .5f);
	style.Colors[ImGuiCol_Text] = ImVec4(.98f, .98f, .98f, 1.f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(.26f, .26f, .26f, .95f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_Border] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(.16f, .16f, .16f, 1.f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(.98f, .49f, 0.f, 1.f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(.26f, .26f, .26f, 1.f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(.21f, .21f, .21f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(.78f, .78f, .78f, 1.f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(.74f, .74f, .74f, 1.f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(.74f, .74f, .74f, 1.f);
	style.Colors[ImGuiCol_Button] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.43f, .43f, .43f, 1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(.11f, .11f, .11f, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(.36f, .36f, .36f, 1.f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(.26f, .59f, .98f, 1.f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(.26f, .59f, .98f, 1.f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(.32f, .52f, .65f, 1.f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(.2f, .2f, .2f, .5f);
	ImGui::GetIO().Fonts->AddFontFromFileTTF(Font.c_str(), 12.f);
};
