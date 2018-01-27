
#pragma once

struct IDirect3DDevice9;

bool ImGui_ImplDX9_Init(VOID *, IDirect3DDevice9 *);
VOID ImGui_ImplDX9_NewFrame(VOID);
VOID ImGui_ImplDX9_InvalidateDeviceObjects(VOID);
bool ImGui_ImplDX9_CreateDeviceObjects(VOID);
