
#pragma once

#include "XorString.h"

#include <string.h>

#define IM_ARRAYSIZE(_ARR) ((int) (sizeof(_ARR) / sizeof(*_ARR)))
#define IM_OFFSETOF(_TYPE, _MEMBER) ((size_t) & (((_TYPE *) 0)->_MEMBER))

struct ImDrawChannel;
struct ImDrawCmd;
struct ImDrawData;
struct ImDrawList;
struct ImDrawListSharedData;
struct ImDrawVert;
struct ImFont;
struct ImFontAtlas;
struct ImFontConfig;
struct ImGuiIO;
struct ImGuiStorage;
struct ImGuiStyle;
struct ImGuiTextEditCallbackData;
struct ImGuiSizeCallbackData;
struct ImGuiListClipper;
struct ImGuiContext;

typedef unsigned int ImU32;
typedef unsigned int ImGuiID;
typedef unsigned short ImWchar;
typedef void * ImTextureID;
typedef int ImGuiCol;
typedef int ImGuiCond;
typedef int ImGuiKey;
typedef int ImGuiMouseCursor;
typedef int ImGuiStyleVar;
typedef int ImDrawCornerFlags;
typedef int ImDrawListFlags;
typedef int ImGuiColumnsFlags;
typedef int ImGuiComboFlags;
typedef int ImGuiFocusedFlags;
typedef int ImGuiHoveredFlags;
typedef int ImGuiInputTextFlags;
typedef int ImGuiSelectableFlags;
typedef int ImGuiWindowFlags;
typedef int(*ImGuiTextEditCallback) (ImGuiTextEditCallbackData * data);
typedef void(*ImGuiSizeCallback) (ImGuiSizeCallbackData * data);

struct ImVec2
{
	float x, y;
	ImVec2() { x = y = 0.0f; };
	ImVec2(float _x, float _y) { x = _x; y = _y; };
};

struct ImVec4
{
	float x, y, z, w;
	ImVec4() { x = y = z = w = 0.0f; };
	ImVec4(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; };
};

namespace ImGui
{
	ImGuiIO & GetIO();
	ImGuiStyle & GetStyle();
	ImDrawData * GetDrawData();
	void NewFrame();
	void Render();
	void EndFrame();
	bool Begin(const char* name, ImGuiWindowFlags flags = 0);
	void End();
	bool BeginChild(const char* str_id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags extra_flags = 0);
	bool BeginChild(ImGuiID id, const ImVec2& size = ImVec2(0, 0), bool border = false, ImGuiWindowFlags extra_flags = 0);
	void EndChild();
	ImVec2 GetContentRegionMax();
	ImVec2 GetContentRegionAvail();
	float GetContentRegionAvailWidth();
	ImVec2 GetWindowContentRegionMin();
	ImVec2 GetWindowContentRegionMax();
	float GetWindowContentRegionWidth();
	ImDrawList * GetWindowDrawList();
	ImVec2 GetWindowPos();
	ImVec2 GetWindowSize();
	float GetWindowWidth();
	float GetWindowHeight();
	bool IsWindowCollapsed();
	bool IsWindowAppearing();
	void SetWindowFontScale(float scale);
	void SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));
	void SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);
	void SetNextWindowSizeConstraints(const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL);
	void SetNextWindowContentSize(const ImVec2& size);
	void SetNextWindowCollapsed(bool collapsed, ImGuiCond cond = 0);
	void SetNextWindowFocus();
	void SetWindowPos(const ImVec2& pos, ImGuiCond cond = 0);
	void SetWindowSize(const ImVec2& size, ImGuiCond cond = 0);
	void SetWindowCollapsed(bool collapsed, ImGuiCond cond = 0);
	void SetWindowFocus();
	void SetWindowPos(const char* name, const ImVec2& pos, ImGuiCond cond = 0);
	void SetWindowSize(const char* name, const ImVec2& size, ImGuiCond cond = 0);
	void SetWindowCollapsed(const char* name, bool collapsed, ImGuiCond cond = 0);
	void SetWindowFocus(const char* name);
	float GetScrollX();
	float GetScrollY();
	float GetScrollMaxX();
	float GetScrollMaxY();
	void SetScrollX(float scroll_x);
	void SetScrollY(float scroll_y);
	void SetScrollHere(float center_y_ratio = 0.5f);
	void SetScrollFromPosY(float pos_y, float center_y_ratio = 0.5f);
	void PushFont(ImFont* font);
	void PopFont();
	ImFont * GetFont();
	float GetFontSize();
	ImVec2 GetFontTexUvWhitePixel();
	ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f);
	ImU32 GetColorU32(const ImVec4& col);
	ImU32 GetColorU32(ImU32 col);
	void PushItemWidth(float item_width);
	void PopItemWidth();
	float CalcItemWidth();
	void PushTextWrapPos(float wrap_pos_x = 0.0f);
	void PopTextWrapPos();
	void Separator();
	void SameLine(float pos_x = 0.0f, float spacing_w = -1.0f);
	void NewLine();
	void Spacing();
	void Indent(float indent_w = 0.0f);
	void Unindent(float indent_w = 0.0f);
	void BeginGroup();
	void EndGroup();
	ImVec2 GetCursorPos();
	float GetCursorPosX();
	float GetCursorPosY();
	void SetCursorPos(const ImVec2& local_pos);
	void SetCursorPosX(float x);
	void SetCursorPosY(float y);
	ImVec2 GetCursorStartPos();
	ImVec2 GetCursorScreenPos();
	void SetCursorScreenPos(const ImVec2& pos);
	float GetTextLineHeight();
	float GetTextLineHeightWithSpacing();
	float GetFrameHeight();
	float GetFrameHeightWithSpacing();
	void Columns(int count = 1);
	void NextColumn();
	int GetColumnIndex();
	float GetColumnWidth(int column_index = -1);
	void SetColumnWidth(int column_index, float width);
	float GetColumnOffset(int column_index = -1);
	void SetColumnOffset(int column_index, float offset_x);
	int GetColumnsCount();
	void PushID(const char* str_id);
	void PushID(const void* ptr_id);
	void PushID(int int_id);
	void PopID();
	ImGuiID GetID(const char* str_id);
	ImGuiID GetID(const char* str_id_begin, const char* str_id_end);
	ImGuiID GetID(const void* ptr_id);
	void TextUnformatted(const char* text, const char* text_end = NULL);
	void Text(const char* fmt, ...);
	void TextV(const char* fmt, va_list args);
	bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
	bool Checkbox(const char* label, bool* v);
	bool CheckboxFlags(const char* label, unsigned int* flags, unsigned int flags_value);
	bool BeginCombo(const char* label, const char* preview_value, ImGuiComboFlags flags = 0);
	void EndCombo();
	bool Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
	bool Combo(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int popup_max_height_in_items = -1);
	bool InputText(const char* label, char* buf, size_t buf_size, ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL);
	bool InputInt(const char* label, int* v, int step = 1, int step_fast = 100, ImGuiInputTextFlags extra_flags = 0);
	bool SliderFloat(const char* label, float* v, float v_min, float v_max, const char* display_format, float power = 1.f);
	bool SliderInt(const char* label, int* v, int v_min, int v_max, const char* display_format);
	bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool Selectable(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
	bool ListBox(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items = -1);
	bool ListBox(const char* label, int* current_item, bool(*items_getter)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
	bool ListBoxHeader(const char* label, const ImVec2& size = ImVec2(0, 0));
	bool ListBoxHeader(const char* label, int items_count, int height_in_items = -1);
	void ListBoxFooter();
	void EndPopup();
	bool IsPopupOpen(const char* str_id);
	void CloseCurrentPopup();
	void PushClipRect(const ImVec2& clip_rect_min, const ImVec2& clip_rect_max, bool intersect_with_current_clip_rect);
	void PopClipRect();
	void SetItemDefaultFocus();
	void SetKeyboardFocusHere(int offset = 0);
	bool IsItemHovered(ImGuiHoveredFlags flags = 0);
	bool IsItemActive();
	bool IsItemClicked(int mouse_button = 0);
	bool IsItemVisible();
	bool IsAnyItemHovered();
	bool IsAnyItemActive();
	ImVec2 GetItemRectMin();
	ImVec2 GetItemRectMax();
	ImVec2 GetItemRectSize();
	void SetItemAllowOverlap();
	bool IsWindowFocused(ImGuiFocusedFlags flags = 0);
	bool IsWindowHovered(ImGuiHoveredFlags flags = 0);
	bool IsAnyWindowFocused();
	bool IsAnyWindowHovered();
	bool IsRectVisible(const ImVec2& size);
	bool IsRectVisible(const ImVec2& rect_min, const ImVec2& rect_max);
	float GetTime();
	int GetFrameCount();
	ImDrawList * GetOverlayDrawList();
	ImDrawListSharedData * GetDrawListSharedData();
	ImVec2 CalcTextSize(const char* text, const char* text_end = NULL, bool hide_text_after_double_hash = false, float wrap_width = -1.0f);
	void CalcListClipping(int items_count, float items_height, int* out_items_display_start, int* out_items_display_end);
	bool BeginChildFrame(ImGuiID id, const ImVec2& size, ImGuiWindowFlags extra_flags = 0);
	void EndChildFrame();
	ImVec4 ColorConvertU32ToFloat4(ImU32 in);
	ImU32 ColorConvertFloat4ToU32(const ImVec4& in);
	void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
	void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);
	int GetKeyIndex(ImGuiKey imgui_key);
	bool IsKeyDown(int user_key_index);
	bool IsKeyPressed(int user_key_index, bool repeat = true);
	bool IsKeyReleased(int user_key_index);
	int GetKeyPressedAmount(int key_index, float repeat_delay, float rate);
	bool IsMouseDown(int button);
	bool IsMouseClicked(int button, bool repeat = false);
	bool IsMouseDoubleClicked(int button);
	bool IsMouseReleased(int button);
	bool IsMouseDragging(int button = 0, float lock_threshold = -1.0f);
	bool IsMouseHoveringRect(const ImVec2& r_min, const ImVec2& r_max, bool clip = true);
	bool IsMousePosValid(const ImVec2* mouse_pos = NULL);
	ImVec2 GetMousePos();
	ImVec2 GetMousePosOnOpeningCurrentPopup();
	ImVec2 GetMouseDragDelta(int button = 0, float lock_threshold = -1.0f);
	void ResetMouseDragDelta(int button = 0);
	ImGuiMouseCursor GetMouseCursor();
	void SetMouseCursor(ImGuiMouseCursor type);
	ImGuiContext * CreateContext();
	void DestroyContext(ImGuiContext* ctx);
	ImGuiContext * GetCurrentContext();
	void SetCurrentContext(ImGuiContext* ctx);
};

enum ImGuiWindowFlags_
{
	ImGuiWindowFlags_NoTitleBar = 1 << 0,
	ImGuiWindowFlags_NoResize = 1 << 1,
	ImGuiWindowFlags_NoMove = 1 << 2,
	ImGuiWindowFlags_NoScrollbar = 1 << 3,
	ImGuiWindowFlags_NoScrollWithMouse = 1 << 4,
	ImGuiWindowFlags_NoCollapse = 1 << 5,
	ImGuiWindowFlags_AlwaysAutoResize = 1 << 6,
	ImGuiWindowFlags_NoSavedSettings = 1 << 8,
	ImGuiWindowFlags_NoInputs = 1 << 9,
	ImGuiWindowFlags_MenuBar = 1 << 10,
	ImGuiWindowFlags_HorizontalScrollbar = 1 << 11,
	ImGuiWindowFlags_NoFocusOnAppearing = 1 << 12,
	ImGuiWindowFlags_NoBringToFrontOnFocus = 1 << 13,
	ImGuiWindowFlags_AlwaysVerticalScrollbar = 1 << 14,
	ImGuiWindowFlags_AlwaysHorizontalScrollbar = 1 << 15,
	ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,
	ImGuiWindowFlags_ResizeFromAnySide = 1 << 17,
	ImGuiWindowFlags_ChildWindow = 1 << 24,
	ImGuiWindowFlags_Popup = 1 << 26,
	ImGuiWindowFlags_Modal = 1 << 27,
	ImGuiWindowFlags_ChildMenu = 1 << 28,
};

enum ImGuiInputTextFlags_
{
	ImGuiInputTextFlags_CharsDecimal = 1 << 0,
	ImGuiInputTextFlags_CharsHexadecimal = 1 << 1,
	ImGuiInputTextFlags_CharsUppercase = 1 << 2,
	ImGuiInputTextFlags_CharsNoBlank = 1 << 3,
	ImGuiInputTextFlags_AutoSelectAll = 1 << 4,
	ImGuiInputTextFlags_EnterReturnsTrue = 1 << 5,
	ImGuiInputTextFlags_CallbackCharFilter = 1 << 9,
	ImGuiInputTextFlags_AllowTabInput = 1 << 10,
	ImGuiInputTextFlags_CtrlEnterForNewLine = 1 << 11,
	ImGuiInputTextFlags_NoHorizontalScroll = 1 << 12,
	ImGuiInputTextFlags_AlwaysInsertMode = 1 << 13,
};

enum ImGuiSelectableFlags_
{
	ImGuiSelectableFlags_DontClosePopups = 1 << 0,
	ImGuiSelectableFlags_SpanAllColumns = 1 << 1,
	ImGuiSelectableFlags_AllowDoubleClick = 1 << 2,
};

enum ImGuiComboFlags_
{
	ImGuiComboFlags_PopupAlignLeft = 1 << 0,
	ImGuiComboFlags_HeightSmall = 1 << 1,
	ImGuiComboFlags_HeightRegular = 1 << 2,
	ImGuiComboFlags_HeightLarge = 1 << 3,
	ImGuiComboFlags_HeightLargest = 1 << 4,
	ImGuiComboFlags_HeightMask_ = ImGuiComboFlags_HeightSmall | ImGuiComboFlags_HeightRegular | ImGuiComboFlags_HeightLarge | ImGuiComboFlags_HeightLargest,
};

enum ImGuiFocusedFlags_
{
	ImGuiFocusedFlags_ChildWindows = 1 << 0,
	ImGuiFocusedFlags_RootWindow = 1 << 1,
	ImGuiFocusedFlags_RootAndChildWindows = ImGuiFocusedFlags_RootWindow | ImGuiFocusedFlags_ChildWindows,
};

enum ImGuiHoveredFlags_
{
	ImGuiHoveredFlags_Default = 0,
	ImGuiHoveredFlags_ChildWindows = 1 << 0,
	ImGuiHoveredFlags_RootWindow = 1 << 1,
	ImGuiHoveredFlags_AllowWhenBlockedByPopup = 1 << 2,
	ImGuiHoveredFlags_AllowWhenBlockedByActiveItem = 1 << 4,
	ImGuiHoveredFlags_AllowWhenOverlapped = 1 << 5,
	ImGuiHoveredFlags_RectOnly = ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem | ImGuiHoveredFlags_AllowWhenOverlapped,
	ImGuiHoveredFlags_RootAndChildWindows = ImGuiHoveredFlags_RootWindow | ImGuiHoveredFlags_ChildWindows,
};

enum ImGuiKey_
{
	ImGuiKey_LeftArrow,
	ImGuiKey_RightArrow,
	ImGuiKey_Backspace,
	ImGuiKey_COUNT,
};

enum ImGuiCol_
{
	ImGuiCol_Text,
	ImGuiCol_WindowBg,
	ImGuiCol_ChildBg,
	ImGuiCol_PopupBg,
	ImGuiCol_Border,
	ImGuiCol_BorderShadow,
	ImGuiCol_FrameBg,
	ImGuiCol_FrameBgHovered,
	ImGuiCol_FrameBgActive,
	ImGuiCol_TitleBg,
	ImGuiCol_TitleBgActive,
	ImGuiCol_TitleBgCollapsed,
	ImGuiCol_MenuBarBg,
	ImGuiCol_ScrollbarBg,
	ImGuiCol_ScrollbarGrab,
	ImGuiCol_ScrollbarGrabHovered,
	ImGuiCol_ScrollbarGrabActive,
	ImGuiCol_CheckMark,
	ImGuiCol_SliderGrab,
	ImGuiCol_SliderGrabActive,
	ImGuiCol_Button,
	ImGuiCol_ButtonHovered,
	ImGuiCol_ButtonActive,
	ImGuiCol_Header,
	ImGuiCol_HeaderHovered,
	ImGuiCol_HeaderActive,
	ImGuiCol_Separator,
	ImGuiCol_SeparatorHovered,
	ImGuiCol_SeparatorActive,
	ImGuiCol_ResizeGrip,
	ImGuiCol_ResizeGripHovered,
	ImGuiCol_ResizeGripActive,
	ImGuiCol_TextSelectedBg,
	ImGuiCol_ModalWindowDarkening,
	ImGuiCol_COUNT,
};

enum ImGuiStyleVar_
{
	ImGuiStyleVar_Alpha,
	ImGuiStyleVar_WindowPadding,
	ImGuiStyleVar_WindowRounding,
	ImGuiStyleVar_WindowBorderSize,
	ImGuiStyleVar_WindowMinSize,
	ImGuiStyleVar_ChildRounding,
	ImGuiStyleVar_ChildBorderSize,
	ImGuiStyleVar_PopupRounding,
	ImGuiStyleVar_PopupBorderSize,
	ImGuiStyleVar_FramePadding,
	ImGuiStyleVar_FrameRounding,
	ImGuiStyleVar_FrameBorderSize,
	ImGuiStyleVar_ItemSpacing,
	ImGuiStyleVar_ItemInnerSpacing,
	ImGuiStyleVar_IndentSpacing,
	ImGuiStyleVar_GrabMinSize,
	ImGuiStyleVar_ButtonTextAlign,
	ImGuiStyleVar_Count_,
};

enum ImGuiMouseCursor_
{
	ImGuiMouseCursor_None = -1,
	ImGuiMouseCursor_Arrow = 0,
	ImGuiMouseCursor_TextInput,
	ImGuiMouseCursor_Move,
	ImGuiMouseCursor_ResizeNS,
	ImGuiMouseCursor_ResizeEW,
	ImGuiMouseCursor_ResizeNESW,
	ImGuiMouseCursor_ResizeNWSE,
	ImGuiMouseCursor_Count_,
};

enum ImGuiCond_
{
	ImGuiCond_Always = 1 << 0,
	ImGuiCond_Once = 1 << 1,
	ImGuiCond_FirstUseEver = 1 << 2,
	ImGuiCond_Appearing = 1 << 3,
};

struct ImGuiStyle
{
	float Alpha;
	ImVec2 WindowPadding;
	float WindowRounding;
	float WindowBorderSize;
	ImVec2 WindowMinSize;
	ImVec2 WindowTitleAlign;
	float ChildRounding;
	float ChildBorderSize;
	float PopupRounding;
	float PopupBorderSize;
	ImVec2 FramePadding;
	float FrameRounding;
	float FrameBorderSize;
	ImVec2 ItemSpacing;
	ImVec2 ItemInnerSpacing;
	ImVec2 TouchExtraPadding;
	float IndentSpacing;
	float ColumnsMinSpacing;
	float ScrollbarSize;
	float ScrollbarRounding;
	float GrabMinSize;
	float GrabRounding;
	ImVec2 ButtonTextAlign;
	ImVec2 DisplayWindowPadding;
	ImVec2 DisplaySafeAreaPadding;
	bool AntiAliasedLines;
	bool AntiAliasedFill;
	float CurveTessellationTol;
	ImVec4 Colors[ImGuiCol_COUNT];
	ImGuiStyle();
	void ScaleAllSizes(float scale_factor);
};

struct ImGuiIO
{
	ImVec2 DisplaySize;
	float DeltaTime;
	float MouseDoubleClickTime;
	float MouseDoubleClickMaxDist;
	float MouseDragThreshold;
	int KeyMap[ImGuiKey_COUNT];
	float KeyRepeatDelay;
	float KeyRepeatRate;
	void * UserData;
	ImFontAtlas * Fonts;
	float FontGlobalScale;
	bool FontAllowUserScaling;
	ImFont * FontDefault;
	ImVec2 DisplayFramebufferScale;
	ImVec2 DisplayVisibleMin;
	ImVec2 DisplayVisibleMax;
	bool OptMacOSXBehaviors;
	bool OptCursorBlink;
	void(*RenderDrawListsFn) (ImDrawData* data);
	void(*ImeSetInputScreenPosFn) (int x, int y);
	void* ImeWindowHandle;
	ImVec2 MousePos;
	bool MouseDown[5];
	float MouseWheel;
	bool MouseDrawCursor;
	bool KeyShift;
	bool KeySuper;
	bool KeysDown[XS_RANDOM_SIZE * 2];
	ImWchar InputCharacters[XS_RANDOM_SIZE / 16];
	void AddInputCharacter(ImWchar c);
	bool WantCaptureMouse;
	bool WantCaptureKeyboard;
	bool WantTextInput;
	bool WantMoveMouse;
	int MetricsRenderVertices;
	int MetricsRenderIndices;
	int MetricsActiveWindows;
	ImVec2 MouseDelta;
	ImVec2 MousePosPrev;
	ImVec2 MouseClickedPos[5];
	float MouseClickedTime[5];
	bool MouseClicked[5];
	bool MouseDoubleClicked[5];
	bool MouseReleased[5];
	bool MouseDownOwned[5];
	float MouseDownDuration[5];
	float MouseDownDurationPrev[5];
	ImVec2 MouseDragMaxDistanceAbs[5];
	float MouseDragMaxDistanceSqr[5];
	float KeysDownDuration[XS_RANDOM_SIZE * 2];
	float KeysDownDurationPrev[XS_RANDOM_SIZE * 2];
	ImGuiIO();
};

template<typename T> class ImVector
{
public:
	int Size;
	int Capacity;
	T* Data;
	typedef T value_type;
	typedef value_type* iterator;
	typedef const value_type* const_iterator;
	inline ImVector() { Size = Capacity = 0; Data = NULL; }
	inline ~ImVector() { if (Data) free(Data); }
	inline bool empty() const { return Size == 0; }
	inline int size() const { return Size; }
	inline int capacity() const { return Capacity; }
	inline value_type& operator[](int i) { return Data[i]; }
	inline const value_type& operator[](int i) const { return Data[i]; }
	inline void clear() { if (Data) { Size = Capacity = 0; free(Data); Data = NULL; } }
	inline iterator begin() { return Data; }
	inline const_iterator begin() const { return Data; }
	inline iterator end() { return Data + Size; }
	inline const_iterator end() const { return Data + Size; }
	inline value_type& front() { return Data[0]; }
	inline const value_type& front() const { return Data[0]; }
	inline value_type& back() { return Data[Size - 1]; }
	inline const value_type& back() const { return Data[Size - 1]; }
	inline void swap(ImVector<T>& rhs) { int rhs_size = rhs.Size; rhs.Size = Size; Size = rhs_size; int rhs_cap = rhs.Capacity; rhs.Capacity = Capacity; Capacity = rhs_cap; value_type* rhs_data = rhs.Data; rhs.Data = Data; Data = rhs_data; }
	inline int _grow_capacity(int sz) const { int new_capacity = Capacity ? (Capacity + Capacity / 2) : 8; return new_capacity > sz ? new_capacity : sz; }
	inline void resize(int new_size) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); Size = new_size; }
	inline void resize(int new_size, const T& v) { if (new_size > Capacity) reserve(_grow_capacity(new_size)); if (new_size > Size) for (int n = Size; n < new_size; n++) Data[n] = v; Size = new_size; }
	inline void reserve(int new_capacity)
	{
		if (new_capacity <= Capacity) return;
		T* new_data = (value_type *)malloc((size_t)new_capacity * sizeof(T));
		if (Data) memcpy(new_data, Data, (size_t)Size * sizeof(T));
		free(Data);
		Data = new_data;
		Capacity = new_capacity;
	};
	inline void push_back(const value_type& v) { if (Size == Capacity) reserve(_grow_capacity(Size + 1)); Data[Size++] = v; }
	inline void pop_back() { Size--; }
	inline void push_front(const value_type& v) { if (Size == 0) push_back(v); else insert(Data, v); }
	inline iterator erase(const_iterator it) { const ptrdiff_t off = it - Data; memmove(Data + off, Data + off + 1, ((size_t)Size - (size_t)off - 1) * sizeof(value_type)); Size--; return Data + off; }
	inline iterator insert(const_iterator it, const value_type& v) { const ptrdiff_t off = it - Data; if (Size == Capacity) reserve(_grow_capacity(Size + 1)); if (off < (int)Size) memmove(Data + off + 1, Data + off, ((size_t)Size - (size_t)off) * sizeof(value_type)); Data[off] = v; Size++; return Data + off; }
	inline bool contains(const value_type& v) const { const T* data = Data; const T* data_end = Data + Size; while (data < data_end) if (*data++ == v) return true; return false; }
};

struct ImGuiStorage
{
	struct Pair
	{
		ImGuiID key;
		union { int val_i; float val_f; void * val_p; };
		Pair(ImGuiID _key, int _val_i) { key = _key; val_i = _val_i; };
		Pair(ImGuiID _key, float _val_f) { key = _key; val_f = _val_f; };
		Pair(ImGuiID _key, void * _val_p) { key = _key; val_p = _val_p; };
	};
	ImVector <Pair> Data;
	void Clear() { Data.clear(); };
	void * GetVoidPtr(ImGuiID key) const;
	void SetVoidPtr(ImGuiID key, void* val);
};

struct ImGuiTextEditCallbackData
{
	ImGuiInputTextFlags EventFlag;
	ImGuiInputTextFlags Flags;
	ImWchar EventChar;
};

struct ImGuiSizeCallbackData
{
	void* UserData;
	ImVec2 Pos;
	ImVec2 CurrentSize;
	ImVec2 DesiredSize;
};

#define IM_COL32_R_SHIFT 0
#define IM_COL32_G_SHIFT 8
#define IM_COL32_B_SHIFT 16
#define IM_COL32_A_SHIFT 24
#define IM_COL32_A_MASK 0xFF000000

#define IM_COL32(R, G, B, A) (((ImU32)(A)<<IM_COL32_A_SHIFT) | ((ImU32)(B)<<IM_COL32_B_SHIFT) | ((ImU32)(G)<<IM_COL32_G_SHIFT) | ((ImU32)(R)<<IM_COL32_R_SHIFT))
#define IM_COL32_WHITE IM_COL32(255, 255, 255, 255)
#define IM_COL32_BLACK IM_COL32(0, 0, 0, 255)

struct ImGuiListClipper
{
	float StartPosY;
	float ItemsHeight;
	int ItemsCount, StepNo, DisplayStart, DisplayEnd;
	ImGuiListClipper(int items_count = -1, float items_height = -1.0f) { Begin(items_count, items_height); }
	~ImGuiListClipper() { };
	bool Step();
	void Begin(int items_count, float items_height = -1.0f);
	void End();
};

typedef void(*ImDrawCallback)(const ImDrawList* parent_list, const ImDrawCmd* cmd);

struct ImDrawCmd
{
	unsigned int ElemCount;
	ImVec4 ClipRect;
	ImTextureID TextureId;
	ImDrawCallback UserCallback;
	void* UserCallbackData;
	ImDrawCmd() { ElemCount = 0; ClipRect.x = ClipRect.y = ClipRect.z = ClipRect.w = 0.0f; TextureId = NULL; UserCallback = NULL; UserCallbackData = NULL; }
};

struct ImDrawVert
{
	ImVec2 pos;
	ImVec2 uv;
	ImU32 col;
};

struct ImDrawChannel
{
	ImVector <ImDrawCmd> CmdBuffer;
	ImVector <unsigned short> IdxBuffer;
};

enum ImDrawCornerFlags_
{
	ImDrawCornerFlags_TopLeft = 1 << 0,
	ImDrawCornerFlags_TopRight = 1 << 1,
	ImDrawCornerFlags_BotLeft = 1 << 2,
	ImDrawCornerFlags_BotRight = 1 << 3,
	ImDrawCornerFlags_Top = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_TopRight,
	ImDrawCornerFlags_Bot = ImDrawCornerFlags_BotLeft | ImDrawCornerFlags_BotRight,
	ImDrawCornerFlags_Left = ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft,
	ImDrawCornerFlags_Right = ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight,
	ImDrawCornerFlags_All = 0xF,
};

enum ImDrawListFlags_
{
	ImDrawListFlags_AntiAliasedLines = 1 << 0,
	ImDrawListFlags_AntiAliasedFill = 1 << 1,
};

struct ImDrawList
{
	ImVector<ImDrawCmd> CmdBuffer;
	ImVector<unsigned short> IdxBuffer;
	ImVector<ImDrawVert> VtxBuffer;
	ImDrawListFlags Flags;
	const ImDrawListSharedData* _Data;
	unsigned int _VtxCurrentIdx;
	ImDrawVert* _VtxWritePtr;
	unsigned short* _IdxWritePtr;
	ImVector<ImVec4> _ClipRectStack;
	ImVector<ImTextureID> _TextureIdStack;
	ImVector<ImVec2> _Path;
	int _ChannelsCurrent;
	int _ChannelsCount;
	ImVector<ImDrawChannel> _Channels;
	ImDrawList(const ImDrawListSharedData* shared_data) { _Data = shared_data; Clear(); }
	~ImDrawList() { ClearFreeMemory(); }
	void PushClipRect(ImVec2 clip_rect_min, ImVec2 clip_rect_max, bool intersect_with_current_clip_rect = false);
	void PushClipRectFullScreen();
	void PopClipRect();
	void PushTextureID(const ImTextureID& texture_id);
	void PopTextureID();
	inline ImVec2 GetClipRectMin() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.x, cr.y); }
	inline ImVec2 GetClipRectMax() const { const ImVec4& cr = _ClipRectStack.back(); return ImVec2(cr.z, cr.w); }
	void AddLine(const ImVec2& a, const ImVec2& b, ImU32 col, float thickness = 1.0f);
	void AddRect(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All, float thickness = 1.0f);
	void AddRectFilled(const ImVec2& a, const ImVec2& b, ImU32 col, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All);
	void AddQuad(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col, float thickness = 1.0f);
	void AddQuadFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, ImU32 col);
	void AddTriangle(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col, float thickness = 1.0f);
	void AddTriangleFilled(const ImVec2& a, const ImVec2& b, const ImVec2& c, ImU32 col);
	void AddCircle(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12, float thickness = 1.0f);
	void AddCircleFilled(const ImVec2& centre, float radius, ImU32 col, int num_segments = 12);
	void AddText(const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL);
	void AddText(const ImFont* font, float font_size, const ImVec2& pos, ImU32 col, const char* text_begin, const char* text_end = NULL, float wrap_width = 0.0f, const ImVec4* cpu_fine_clip_rect = NULL);
	void AddImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 1), ImU32 col = 0xFFFFFFFF);
	void AddImageQuad(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 0), const ImVec2& uv_c = ImVec2(1, 1), const ImVec2& uv_d = ImVec2(0, 1), ImU32 col = 0xFFFFFFFF);
	void AddImageRounded(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col, float rounding, int rounding_corners = ImDrawCornerFlags_All);
	void AddPolyline(const ImVec2* points, const int num_points, ImU32 col, bool closed, float thickness);
	void AddConvexPolyFilled(const ImVec2* points, const int num_points, ImU32 col);
	inline void PathClear() { _Path.resize(0); }
	inline void PathLineTo(const ImVec2& pos) { _Path.push_back(pos); }
	inline void PathLineToMergeDuplicate(const ImVec2& pos) { if (_Path.Size == 0 || memcmp(&_Path[_Path.Size - 1], &pos, 8) != 0) _Path.push_back(pos); }
	inline void PathFillConvex(ImU32 col) { AddConvexPolyFilled(_Path.Data, _Path.Size, col); PathClear(); }
	inline void PathStroke(ImU32 col, bool closed, float thickness = 1.0f) { AddPolyline(_Path.Data, _Path.Size, col, closed, thickness); PathClear(); }
	void PathArcTo(const ImVec2& centre, float radius, float a_min, float a_max, int num_segments = 10);
	void PathArcToFast(const ImVec2& centre, float radius, int a_min_of_12, int a_max_of_12);
	void PathRect(const ImVec2& rect_min, const ImVec2& rect_max, float rounding = 0.0f, int rounding_corners_flags = ImDrawCornerFlags_All);
	void ChannelsSplit(int channels_count);
	void ChannelsMerge();
	void ChannelsSetCurrent(int channel_index);
	void AddCallback(ImDrawCallback callback, void* callback_data);
	void AddDrawCmd();
	void Clear();
	void ClearFreeMemory();
	void PrimReserve(int idx_count, int vtx_count);
	void PrimRect(const ImVec2& a, const ImVec2& b, ImU32 col);
	void PrimRectUV(const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, ImU32 col);
	void PrimQuadUV(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& d, const ImVec2& uv_a, const ImVec2& uv_b, const ImVec2& uv_c, const ImVec2& uv_d, ImU32 col);
	inline void PrimWriteVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col) { _VtxWritePtr->pos = pos; _VtxWritePtr->uv = uv; _VtxWritePtr->col = col; _VtxWritePtr++; _VtxCurrentIdx++; }
	inline void PrimWriteIdx(unsigned short idx) { *_IdxWritePtr = idx; _IdxWritePtr++; }
	inline void PrimVtx(const ImVec2& pos, const ImVec2& uv, ImU32 col) { PrimWriteIdx((unsigned short)_VtxCurrentIdx); PrimWriteVtx(pos, uv, col); }
	void UpdateClipRect();
	void UpdateTextureID();
};

struct ImDrawData
{
	bool Valid;
	ImDrawList** CmdLists;
	int CmdListsCount;
	int TotalVtxCount;
	int TotalIdxCount;
	ImDrawData() { Valid = false; CmdLists = NULL; CmdListsCount = TotalVtxCount = TotalIdxCount = 0; }
	void DeIndexAllBuffers();
	void ScaleClipRects(const ImVec2& sc);
};

struct ImFontConfig
{
	void * FontData;
	int FontDataSize;
	bool FontDataOwnedByAtlas;
	int FontNo;
	float SizePixels;
	int OversampleH, OversampleV;
	bool PixelSnapH;
	ImVec2 GlyphExtraSpacing;
	ImVec2 GlyphOffset;
	const ImWchar * GlyphRanges;
	bool MergeMode;
	unsigned int RasterizerFlags;
	float RasterizerMultiply;
	ImFont * DstFont;
	ImFontConfig();
};

struct ImFontGlyph
{
	ImWchar Codepoint;
	float AdvanceX;
	float X0, Y0, X1, Y1;
	float U0, V0, U1, V1;
};

struct ImFontAtlas
{
	ImFontAtlas();
	~ImFontAtlas();
	ImFont * AddFont(const ImFontConfig* font_cfg);
	ImFont * AddFontFromFileTTF(const char * filename, float size_pixels);
	ImFont * AddFontFromMemoryTTF(void * font_data, int font_size, float size_pixels);
	void ClearTexData();
	void ClearInputData();
	void ClearFonts();
	void Clear();
	bool Build();
	void GetTexDataAsAlpha8(unsigned char ** out_pixels, int * out_width, int * out_height, int * out_bytes_per_pixel = NULL);
	void GetTexDataAsRGBA32(unsigned char ** out_pixels, int * out_width, int * out_height, int * out_bytes_per_pixel = NULL);
	void SetTexID(ImTextureID id) { TexID = id; };
	const ImWchar* GetGlyphRangesDefault();
	struct GlyphRangesBuilder
	{
		ImVector<unsigned char> UsedChars;
		GlyphRangesBuilder() { UsedChars.resize(0x10000 / 8); memset(UsedChars.Data, 0, 0x10000 / 8); }
		bool GetBit(int n) { return (UsedChars[n >> 3] & (1 << (n & 7))) != 0; }
		void SetBit(int n) { UsedChars[n >> 3] |= 1 << (n & 7); }
		void AddChar(ImWchar c) { SetBit(c); }
		void AddText(const char* text, const char* text_end = NULL);
		void AddRanges(const ImWchar* ranges);
	};
	struct CustomRect
	{
		unsigned int ID;
		unsigned short Width, Height;
		unsigned short X, Y;
		float GlyphAdvanceX;
		ImVec2 GlyphOffset;
		ImFont* Font;
		CustomRect() { ID = 0xFFFFFFFF; Width = Height = 0; X = Y = 0xFFFF; GlyphAdvanceX = 0.0f; GlyphOffset = ImVec2(0, 0); Font = NULL; }
		bool IsPacked() const { return X != 0xFFFF; }
	};
	int AddCustomRectRegular(unsigned int id, int width, int height);
	int AddCustomRectFontGlyph(ImFont* font, ImWchar id, int width, int height, float advance_x, const ImVec2& offset = ImVec2(0, 0));
	void CalcCustomRectUV(const CustomRect* rect, ImVec2* out_uv_min, ImVec2* out_uv_max);
	const CustomRect * GetCustomRectByIndex(int index) const { if (index < 0) return NULL; return &CustomRects[index]; }
	ImTextureID TexID;
	int TexDesiredWidth;
	int TexGlyphPadding;
	unsigned char * TexPixelsAlpha8;
	unsigned int * TexPixelsRGBA32;
	int TexWidth;
	int TexHeight;
	ImVec2 TexUvWhitePixel;
	ImVector<ImFont*> Fonts;
	ImVector<CustomRect> CustomRects;
	ImVector<ImFontConfig> ConfigData;
	int CustomRectIds[1];
};

struct ImFont
{
	float FontSize;
	float Scale;
	ImVec2 DisplayOffset;
	ImVector <ImFontGlyph> Glyphs;
	ImVector <float> IndexAdvanceX;
	ImVector <unsigned short> IndexLookup;
	const ImFontGlyph* FallbackGlyph;
	float FallbackAdvanceX;
	ImWchar FallbackChar;
	short ConfigDataCount;
	ImFontConfig * ConfigData;
	ImFontAtlas * ContainerAtlas;
	float Ascent, Descent;
	int MetricsTotalSurface;
	ImFont();
	~ImFont();
	void ClearOutputData();
	void BuildLookupTable();
	const ImFontGlyph * FindGlyph(ImWchar c) const;
	void SetFallbackChar(ImWchar c);
	float GetCharAdvance(ImWchar c) const { return ((int)c < IndexAdvanceX.Size) ? IndexAdvanceX[(int)c] : FallbackAdvanceX; }
	bool IsLoaded() const { return ContainerAtlas != NULL; };
	ImVec2 CalcTextSizeA(float size, float max_width, float wrap_width, const char* text_begin, const char* text_end = NULL, const char** remaining = NULL) const;
	const char * CalcWordWrapPositionA(float scale, const char* text, const char* text_end, float wrap_width) const;
	void RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, unsigned short c) const;
	void RenderText(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, const ImVec4& clip_rect, const char* text_begin, const char* text_end, float wrap_width = 0.0f, bool cpu_fine_clip = false) const;
	void GrowIndex(int new_size);
	void AddGlyph(ImWchar c, float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, float advance_x);
};
