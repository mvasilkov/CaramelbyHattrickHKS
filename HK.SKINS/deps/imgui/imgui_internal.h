
#pragma once

#include <stdio.h>

struct ImRect;
struct ImGuiColMod;
struct ImGuiStyleMod;
struct ImGuiGroupData;
struct ImGuiDrawContext;
struct ImGuiTextEditState;
struct ImGuiMouseCursorData;
struct ImGuiPopupRef;
struct ImGuiWindow;

typedef int ImGuiLayoutType;
typedef int ImGuiButtonFlags;
typedef int ImGuiItemFlags;
typedef int ImGuiSeparatorFlags;
typedef int ImGuiSliderFlags;

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE

#define STB_TEXTEDIT_STRING ImGuiTextEditState
#define STB_TEXTEDIT_CHARTYPE ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE -1.f

#include "stb_textedit.h"

extern ImGuiContext * GImGui;

#define IM_PI 3.141592f

int ImTextStrToUtf8(char* buf, int buf_size, const ImWchar* in_text, const ImWchar* in_text_end);
int ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);
int ImTextStrFromUtf8(ImWchar* buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);
int ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);
int ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);
ImU32 ImHash(const void* data, int data_size, ImU32 seed = 0);
void * ImFileLoadToMemory(const char* filename, const char* file_open_mode, int* out_file_size = NULL, int padding_bytes = 0);
inline bool ImCharIsSpace(int c) { return c == ' ' || c == '\t' || c == 0x3000; };
inline int ImUpperPowerOfTwo(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; };
ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
bool ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
void ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
int ImStricmp(const char* str1, const char* str2);
int ImStrnicmp(const char* str1, const char* str2, size_t count);
void ImStrncpy(char* dst, const char* src, size_t count);
char * ImStrdup(const char* str);
int ImStrlenW(const ImWchar* str);
const ImWchar * ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin);
const char * ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
int ImFormatString(char* buf, size_t buf_size, const char* fmt, ...);
int ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args);
inline ImVec2 operator * (const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x*rhs, lhs.y*rhs); }
inline ImVec2 operator / (const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
inline ImVec2 operator + (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline ImVec2 operator - (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline ImVec2 operator * (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x*rhs.x, lhs.y*rhs.y); }
inline ImVec2 operator / (const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
inline ImVec2& operator += (ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
inline ImVec2& operator -= (ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
inline ImVec2& operator *= (ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
inline ImVec2& operator /= (ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
inline ImVec4 operator + (const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
inline ImVec4 operator - (const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
inline ImVec4 operator * (const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z, lhs.w*rhs.w); }
inline int ImMin(int lhs, int rhs) { return lhs < rhs ? lhs : rhs; }
inline int ImMax(int lhs, int rhs) { return lhs >= rhs ? lhs : rhs; }
inline float ImMin(float lhs, float rhs) { return lhs < rhs ? lhs : rhs; }
inline float ImMax(float lhs, float rhs) { return lhs >= rhs ? lhs : rhs; }
inline ImVec2 ImMin(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(ImMin(lhs.x, rhs.x), ImMin(lhs.y, rhs.y)); }
inline ImVec2 ImMax(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(ImMax(lhs.x, rhs.x), ImMax(lhs.y, rhs.y)); }
inline int ImClamp(int v, int mn, int mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
inline float ImClamp(float v, float mn, float mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
inline ImVec2 ImClamp(const ImVec2& f, const ImVec2& mn, ImVec2 mx) { return ImVec2(ImClamp(f.x, mn.x, mx.x), ImClamp(f.y, mn.y, mx.y)); }
inline float ImSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
inline void ImSwap(int& a, int& b) { int tmp = a; a = b; b = tmp; }
inline void ImSwap(float& a, float& b) { float tmp = a; a = b; b = tmp; }
inline int ImLerp(int a, int b, float t) { return (int)(a + (b - a) * t); }
inline float ImLerp(float a, float b, float t) { return a + (b - a) * t; }
inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t) { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
inline ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t) { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
inline ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t) { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
inline float ImLengthSqr(const ImVec2& lhs) { return lhs.x*lhs.x + lhs.y*lhs.y; }
inline float ImLengthSqr(const ImVec4& lhs) { return lhs.x*lhs.x + lhs.y*lhs.y + lhs.z*lhs.z + lhs.w*lhs.w; }
inline float ImInvLength(const ImVec2& lhs, float fail_value) { float d = lhs.x*lhs.x + lhs.y*lhs.y; if (d > 0.0f) return 1.0f / sqrtf(d); return fail_value; }
inline float ImFloor(float f) { return (float)(int)f; }
inline ImVec2 ImFloor(const ImVec2& v) { return ImVec2((float)(int)v.x, (float)(int)v.y); }
inline float ImDot(const ImVec2& a, const ImVec2& b) { return a.x * b.x + a.y * b.y; }
inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a) { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
inline float ImLinearSweep(float current, float target, float speed) { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

enum ImGuiButtonFlags_
{
	ImGuiButtonFlags_Repeat = 1 << 0,
	ImGuiButtonFlags_PressedOnClickRelease = 1 << 1,
	ImGuiButtonFlags_PressedOnClick = 1 << 2,
	ImGuiButtonFlags_PressedOnRelease = 1 << 3,
	ImGuiButtonFlags_PressedOnDoubleClick = 1 << 4,
	ImGuiButtonFlags_FlattenChildren = 1 << 5,
	ImGuiButtonFlags_AllowItemOverlap = 1 << 6,
	ImGuiButtonFlags_DontClosePopups = 1 << 7,
	ImGuiButtonFlags_Disabled = 1 << 8,
	ImGuiButtonFlags_AlignTextBaseLine = 1 << 9,
	ImGuiButtonFlags_NoKeyModifiers = 1 << 10,
	ImGuiButtonFlags_NoHoldingActiveID = 1 << 11,
};

enum ImGuiSliderFlags_
{
	ImGuiSliderFlags_Vertical = 1 << 0,
};

enum ImGuiColumnsFlags_
{
	ImGuiColumnsFlags_NoBorder = 1 << 0,
	ImGuiColumnsFlags_NoResize = 1 << 1,
	ImGuiColumnsFlags_NoPreserveWidths = 1 << 2,
	ImGuiColumnsFlags_NoForceWithinWindow = 1 << 3,
	ImGuiColumnsFlags_GrowParentContentsSize = 1 << 4,
};

enum ImGuiSelectableFlagsPrivate_
{
	ImGuiSelectableFlags_Menu = 1 << 3,
	ImGuiSelectableFlags_MenuItem = 1 << 4,
	ImGuiSelectableFlags_Disabled = 1 << 5,
	ImGuiSelectableFlags_DrawFillAvailWidth = 1 << 6,
};

enum ImGuiSeparatorFlags_
{
	ImGuiSeparatorFlags_Horizontal = 1 << 0,
	ImGuiSeparatorFlags_Vertical = 1 << 1,
};

enum ImGuiLayoutType_
{
	ImGuiLayoutType_Vertical,
	ImGuiLayoutType_Horizontal,
};

enum ImGuiAxis
{
	ImGuiAxis_None = -1,
	ImGuiAxis_X = 0,
	ImGuiAxis_Y = 1,
};

enum ImGuiDataType
{
	ImGuiDataType_Int,
	ImGuiDataType_Float,
};

enum ImGuiDir
{
	ImGuiDir_None = -1,
	ImGuiDir_Left = 0,
	ImGuiDir_Right = 1,
	ImGuiDir_Up = 2,
	ImGuiDir_Down = 3,
	ImGuiDir_Count_,
};

struct ImRect
{
	ImVec2 Min;
	ImVec2 Max;
	ImRect() : Min(FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX) {}
	ImRect(const ImVec2& min, const ImVec2& max) : Min(min), Max(max) {}
	ImRect(const ImVec4& v) : Min(v.x, v.y), Max(v.z, v.w) {}
	ImRect(float x1, float y1, float x2, float y2) : Min(x1, y1), Max(x2, y2) {}
	ImVec2 GetCenter() const { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
	ImVec2 GetSize() const { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
	float GetWidth() const { return Max.x - Min.x; }
	float GetHeight() const { return Max.y - Min.y; }
	ImVec2 GetTL() const { return Min; }
	ImVec2 GetTR() const { return ImVec2(Max.x, Min.y); }
	ImVec2 GetBL() const { return ImVec2(Min.x, Max.y); }
	ImVec2 GetBR() const { return Max; }
	bool Contains(const ImVec2& p) const { return p.x >= Min.x && p.y >= Min.y && p.x < Max.x && p.y < Max.y; }
	bool Contains(const ImRect& r) const { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x < Max.x && r.Max.y < Max.y; }
	bool Overlaps(const ImRect& r) const { return r.Min.y < Max.y && r.Max.y > Min.y && r.Min.x < Max.x && r.Max.x > Min.x; }
	void Add(const ImVec2& p) { if (Min.x > p.x) Min.x = p.x; if (Min.y > p.y) Min.y = p.y; if (Max.x < p.x) Max.x = p.x; if (Max.y < p.y) Max.y = p.y; }
	void Add(const ImRect& r) { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
	void Expand(const float amount) { Min.x -= amount; Min.y -= amount; Max.x += amount; Max.y += amount; }
	void Expand(const ImVec2& amount) { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
	void Translate(const ImVec2& v) { Min.x += v.x; Min.y += v.y; Max.x += v.x; Max.y += v.y; }
	void ClipWith(const ImRect& r) { if (Min.x < r.Min.x) Min.x = r.Min.x; if (Min.y < r.Min.y) Min.y = r.Min.y; if (Max.x > r.Max.x) Max.x = r.Max.x; if (Max.y > r.Max.y) Max.y = r.Max.y; }
	void Floor() { Min.x = (float)(int)Min.x; Min.y = (float)(int)Min.y; Max.x = (float)(int)Max.x; Max.y = (float)(int)Max.y; }
	void FixInverted() { if (Min.x > Max.x) ImSwap(Min.x, Max.x); if (Min.y > Max.y) ImSwap(Min.y, Max.y); }
	bool IsFinite() const { return Min.x != FLT_MAX; }
};

struct ImGuiColMod
{
	ImGuiCol Col;
	ImVec4 BackupValue;
};

struct ImGuiStyleMod
{
	ImGuiStyleVar VarIdx;
	union { int BackupInt[2]; float BackupFloat[2]; };
	ImGuiStyleMod(ImGuiStyleVar idx, int v) { VarIdx = idx; BackupInt[0] = v; }
	ImGuiStyleMod(ImGuiStyleVar idx, float v) { VarIdx = idx; BackupFloat[0] = v; }
	ImGuiStyleMod(ImGuiStyleVar idx, ImVec2 v) { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

struct ImGuiGroupData
{
	ImVec2 BackupCursorPos;
	ImVec2 BackupCursorMaxPos;
	float BackupIndentX;
	float BackupGroupOffsetX;
	float BackupCurrentLineHeight;
	float BackupCurrentLineTextBaseOffset;
	bool BackupActiveIdIsAlive;
	bool AdvanceCursor;
};

struct ImGuiTextEditState
{
	ImGuiID Id;
	ImVector<ImWchar> Text;
	ImVector<char> InitialText;
	ImVector<char> TempTextBuffer;
	int CurLenA, CurLenW;
	int BufSizeA;
	float ScrollX;
	STB_TexteditState StbState;
	float CursorAnim;
	bool CursorFollow;
	bool SelectedAllMouseLock;
	ImGuiTextEditState() { memset(this, 0, sizeof(*this)); }
	void CursorAnimReset() { CursorAnim = -0.30f; }
	void CursorClamp() { StbState.cursor = ImMin(StbState.cursor, CurLenW); StbState.select_start = ImMin(StbState.select_start, CurLenW); StbState.select_end = ImMin(StbState.select_end, CurLenW); }
	bool HasSelection() const { return StbState.select_start != StbState.select_end; }
	void ClearSelection() { StbState.select_start = StbState.select_end = StbState.cursor; }
	void SelectAll() { StbState.select_start = 0; StbState.select_end = CurLenW; StbState.cursor = StbState.select_end; StbState.has_preferred_x = false; }
	void OnKeyPressed(int key);
};

struct ImGuiMouseCursorData
{
	ImGuiMouseCursor Type;
	ImVec2 HotOffset;
	ImVec2 Size;
	ImVec2 TexUvMin[2];
	ImVec2 TexUvMax[2];
};

struct ImGuiPopupRef
{
	ImGuiID PopupId;
	ImGuiWindow* Window;
	ImGuiWindow* ParentWindow;
	int OpenFrameCount;
	ImGuiID OpenParentId;
	ImVec2 OpenPopupPos;
	ImVec2 OpenMousePos;
};

struct ImGuiColumnData
{
	float OffsetNorm;
	float OffsetNormBeforeResize;
	ImGuiColumnsFlags Flags;
	ImRect ClipRect;
	ImGuiColumnData() { OffsetNorm = OffsetNormBeforeResize = 0.0f; Flags = 0; };
};

struct ImGuiColumnsSet
{
	ImGuiID ID;
	ImGuiColumnsFlags Flags;
	bool IsFirstFrame;
	bool IsBeingResized;
	int Current;
	int Count;
	float MinX, MaxX;
	float StartPosY;
	float StartMaxPosX;
	float CellMinY, CellMaxY;
	ImVector<ImGuiColumnData> Columns;
	ImGuiColumnsSet() { Clear(); };
	void Clear()
	{
		ID = 0;
		Flags = 0;
		IsFirstFrame = false;
		IsBeingResized = false;
		Current = 0;
		Count = 1;
		MinX = MaxX = 0.0f;
		StartPosY = 0.0f;
		StartMaxPosX = 0.0f;
		CellMinY = CellMaxY = 0.0f;
		Columns.clear();
	};
};

struct ImDrawListSharedData
{
	ImVec2 TexUvWhitePixel;
	ImFont* Font;
	float FontSize;
	float CurveTessellationTol;
	ImVec4 ClipRectFullscreen;
	ImVec2 CircleVtx12[12];
	ImDrawListSharedData();
};

struct ImGuiNextWindowData
{
	ImGuiCond PosCond;
	ImGuiCond SizeCond;
	ImGuiCond ContentSizeCond;
	ImGuiCond CollapsedCond;
	ImGuiCond SizeConstraintCond;
	ImGuiCond FocusCond;
	ImVec2 PosVal;
	ImVec2 PosPivotVal;
	ImVec2 SizeVal;
	ImVec2 ContentSizeVal;
	bool CollapsedVal;
	ImRect SizeConstraintRect;
	ImGuiSizeCallback SizeCallback;
	void* SizeCallbackUserData;

	ImGuiNextWindowData()
	{
		PosCond = SizeCond = ContentSizeCond = CollapsedCond = SizeConstraintCond = FocusCond = 0;
		PosVal = PosPivotVal = SizeVal = ImVec2(0.0f, 0.0f);
		ContentSizeVal = ImVec2(0.0f, 0.0f);
		CollapsedVal = false;
		SizeConstraintRect = ImRect();
		SizeCallback = NULL;
		SizeCallbackUserData = NULL;
	};

	void Clear()
	{
		PosCond = SizeCond = ContentSizeCond = CollapsedCond = SizeConstraintCond = FocusCond = 0;
	};
};

struct ImGuiContext
{
	bool Initialized;
	ImGuiIO IO;
	ImGuiStyle Style;
	ImFont* Font;
	float FontSize;
	float FontBaseSize;
	ImDrawListSharedData DrawListSharedData;
	float Time;
	int FrameCount;
	int FrameCountEnded;
	int FrameCountRendered;
	ImVector<ImGuiWindow*> Windows;
	ImVector<ImGuiWindow*> WindowsSortBuffer;
	ImVector<ImGuiWindow*> CurrentWindowStack;
	ImGuiStorage WindowsById;
	int WindowsActiveCount;
	ImGuiWindow* CurrentWindow;
	ImGuiWindow* NavWindow;
	ImGuiWindow* HoveredWindow;
	ImGuiWindow* HoveredRootWindow;
	ImGuiID HoveredId;
	bool HoveredIdAllowOverlap;
	ImGuiID HoveredIdPreviousFrame;
	float HoveredIdTimer;
	ImGuiID ActiveId;
	ImGuiID ActiveIdPreviousFrame;
	float ActiveIdTimer;
	bool ActiveIdIsAlive;
	bool ActiveIdIsJustActivated;
	bool ActiveIdAllowOverlap;
	ImVec2 ActiveIdClickOffset;
	ImGuiWindow* ActiveIdWindow;
	ImGuiWindow* MovingWindow;
	ImGuiID MovingWindowMoveId;
	ImVector<ImGuiColMod> ColorModifiers;
	ImVector<ImGuiStyleMod> StyleModifiers;
	ImVector<ImFont*> FontStack;
	ImVector<ImGuiPopupRef> OpenPopupStack;
	ImVector<ImGuiPopupRef> CurrentPopupStack;
	ImGuiNextWindowData NextWindowData;
	ImDrawData RenderDrawData;
	ImVector<ImDrawList*> RenderDrawLists[3];
	float ModalWindowDarkeningRatio;
	ImDrawList OverlayDrawList;
	ImGuiMouseCursor MouseCursor;
	ImGuiMouseCursorData MouseCursorData[ImGuiMouseCursor_Count_];
	ImGuiTextEditState InputTextState;
	ImGuiID ScalarAsInputTextId;
	ImVec4 ColorPickerRef;
	float DragCurrentValue;
	ImVec2 DragLastMouseDelta;
	float DragSpeedDefaultRatio;
	float DragSpeedScaleSlow;
	float DragSpeedScaleFast;
	ImVec2 ScrollbarClickDeltaToGrabCenter;
	ImVec2 OsImePosRequest, OsImePosSet;
	int WantCaptureMouseNextFrame;
	int WantCaptureKeyboardNextFrame;
	int WantTextInputNextFrame;
	char TempBuffer[XS_RANDOM_SIZE * 2 + 1];

	ImGuiContext() : OverlayDrawList(NULL)
	{
		Initialized = false;
		Font = NULL;
		FontSize = FontBaseSize = 0.0f;
		Time = 0.0f;
		FrameCount = 0;
		FrameCountEnded = FrameCountRendered = -1;
		WindowsActiveCount = 0;
		CurrentWindow = NULL;
		NavWindow = NULL;
		HoveredWindow = NULL;
		HoveredRootWindow = NULL;
		HoveredId = 0;
		HoveredIdAllowOverlap = false;
		HoveredIdPreviousFrame = 0;
		HoveredIdTimer = 0.0f;
		ActiveId = 0;
		ActiveIdPreviousFrame = 0;
		ActiveIdTimer = 0.0f;
		ActiveIdIsAlive = false;
		ActiveIdIsJustActivated = false;
		ActiveIdAllowOverlap = false;
		ActiveIdClickOffset = ImVec2(-1, -1);
		ActiveIdWindow = NULL;
		MovingWindow = NULL;
		MovingWindowMoveId = 0;
		ScalarAsInputTextId = 0;
		DragCurrentValue = 0.0f;
		DragLastMouseDelta = ImVec2(0.0f, 0.0f);
		DragSpeedDefaultRatio = 1.0f / 100.0f;
		DragSpeedScaleSlow = 1.0f / 100.0f;
		DragSpeedScaleFast = 10.0f;
		ScrollbarClickDeltaToGrabCenter = ImVec2(0.0f, 0.0f);
		OsImePosRequest = OsImePosSet = ImVec2(-1.0f, -1.0f);
		ModalWindowDarkeningRatio = 0.0f;
		OverlayDrawList._Data = &DrawListSharedData;
		MouseCursor = ImGuiMouseCursor_Arrow;
		memset(MouseCursorData, 0, sizeof(MouseCursorData));
		WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame = WantTextInputNextFrame = -1;
		memset(TempBuffer, 0, sizeof(TempBuffer));
	};
};

enum ImGuiItemFlags_
{
	ImGuiItemFlags_AllowKeyboardFocus = 1 << 0,
	ImGuiItemFlags_ButtonRepeat = 1 << 1,
	ImGuiItemFlags_Disabled = 1 << 2,
	ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,
	ImGuiItemFlags_Default_ = ImGuiItemFlags_AllowKeyboardFocus,
};

struct ImGuiDrawContext
{
	ImVec2 CursorPos;
	ImVec2 CursorPosPrevLine;
	ImVec2 CursorStartPos;
	ImVec2 CursorMaxPos;
	float CurrentLineHeight;
	float CurrentLineTextBaseOffset;
	float PrevLineHeight;
	float PrevLineTextBaseOffset;
	int TreeDepth;
	ImGuiID LastItemId;
	ImRect LastItemRect;
	bool LastItemRectHoveredRect;
	bool MenuBarAppending;
	float MenuBarOffsetX;
	ImVector<ImGuiWindow*> ChildWindows;
	ImGuiStorage* StateStorage;
	ImGuiLayoutType LayoutType;
	ImGuiItemFlags ItemFlags;
	float ItemWidth;
	float TextWrapPos;
	ImVector<ImGuiItemFlags>ItemFlagsStack;
	ImVector<float> ItemWidthStack;
	ImVector<float> TextWrapPosStack;
	ImVector<ImGuiGroupData>GroupStack;
	int StackSizesBackup[6];
	float IndentX;
	float GroupOffsetX;
	float ColumnsOffsetX;
	ImGuiColumnsSet* ColumnsSet;
	ImGuiDrawContext()
	{
		CursorPos = CursorPosPrevLine = CursorStartPos = CursorMaxPos = ImVec2(0.0f, 0.0f);
		CurrentLineHeight = PrevLineHeight = 0.0f;
		CurrentLineTextBaseOffset = PrevLineTextBaseOffset = 0.0f;
		TreeDepth = 0;
		LastItemId = 0;
		LastItemRect = ImRect();
		LastItemRectHoveredRect = false;
		MenuBarAppending = false;
		MenuBarOffsetX = 0.0f;
		StateStorage = NULL;
		LayoutType = ImGuiLayoutType_Vertical;
		ItemWidth = 0.0f;
		ItemFlags = ImGuiItemFlags_Default_;
		TextWrapPos = -1.0f;
		memset(StackSizesBackup, 0, sizeof(StackSizesBackup));
		IndentX = 0.0f;
		GroupOffsetX = 0.0f;
		ColumnsOffsetX = 0.0f;
		ColumnsSet = NULL;
	};
};

struct ImGuiWindow
{
	char* Name;
	ImGuiID ID;
	ImGuiWindowFlags Flags;
	ImVec2 PosFloat;
	ImVec2 Pos;
	ImVec2 Size;
	ImVec2 SizeFull;
	ImVec2 SizeFullAtLastBegin;
	ImVec2 SizeContents;
	ImVec2 SizeContentsExplicit;
	ImRect ContentsRegionRect;
	ImVec2 WindowPadding;
	float WindowRounding;
	float WindowBorderSize;
	ImGuiID MoveId;
	ImVec2 Scroll;
	ImVec2 ScrollTarget;
	ImVec2 ScrollTargetCenterRatio;
	bool ScrollbarX, ScrollbarY;
	ImVec2 ScrollbarSizes;
	bool Active;
	bool WasActive;
	bool WriteAccessed;
	bool Collapsed;
	bool SkipItems;
	bool Appearing;
	int BeginOrderWithinParent;
	int BeginOrderWithinContext;
	int BeginCount;
	ImGuiID PopupId;
	int AutoFitFramesX, AutoFitFramesY;
	bool AutoFitOnlyGrows;
	int AutoFitChildAxises;
	ImGuiDir AutoPosLastDirection;
	int HiddenFrames;
	ImGuiCond SetWindowPosAllowFlags;
	ImGuiCond SetWindowSizeAllowFlags;
	ImGuiCond SetWindowCollapsedAllowFlags;
	ImVec2 SetWindowPosVal;
	ImVec2 SetWindowPosPivot;
	ImGuiDrawContext DC;
	ImVector<ImGuiID> IDStack;
	ImRect ClipRect;
	ImRect WindowRectClipped;
	ImRect InnerRect;
	int LastFrameActive;
	float ItemWidthDefault;
	ImGuiStorage StateStorage;
	ImVector<ImGuiColumnsSet> ColumnsStorage;
	float FontWindowScale;
	ImDrawList* DrawList;
	ImGuiWindow* ParentWindow;
	ImGuiWindow* RootWindow;
	ImGuiWindow* RootNonPopupWindow;
	int FocusIdxAllCounter;
	int FocusIdxTabCounter;
	int FocusIdxAllRequestCurrent;
	int FocusIdxTabRequestCurrent;
	int FocusIdxAllRequestNext;
	int FocusIdxTabRequestNext;

public:

	ImGuiWindow(ImGuiContext* context, const char* name);
	~ImGuiWindow();
	ImGuiID GetID(const char* str, const char* str_end = NULL);
	ImGuiID GetID(const void* ptr);
	ImGuiID GetIDNoKeepAlive(const char* str, const char* str_end = NULL);
	ImGuiID GetIDFromRectangle(const ImRect& r_abs);
	ImRect Rect() const { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
	float CalcFontSize() const { return GImGui->FontBaseSize * FontWindowScale; }
	float TitleBarHeight() const { return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f; }
	ImRect TitleBarRect() const { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
	float MenuBarHeight() const { return (Flags & ImGuiWindowFlags_MenuBar) ? CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f : 0.0f; }
	ImRect MenuBarRect() const { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};

struct ImGuiItemHoveredDataBackup
{
	ImGuiID LastItemId;
	ImRect LastItemRect;
	bool LastItemRectHoveredRect;
	ImGuiItemHoveredDataBackup() { Backup(); }
	void Backup() { ImGuiWindow* window = GImGui->CurrentWindow; LastItemId = window->DC.LastItemId; LastItemRect = window->DC.LastItemRect; LastItemRectHoveredRect = window->DC.LastItemRectHoveredRect; }
	void Restore() const { ImGuiWindow* window = GImGui->CurrentWindow; window->DC.LastItemId = LastItemId; window->DC.LastItemRect = LastItemRect; window->DC.LastItemRectHoveredRect = LastItemRectHoveredRect; }
};

namespace ImGui
{
	inline ImGuiWindow * GetCurrentWindowRead() { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
	inline ImGuiWindow * GetCurrentWindow() { ImGuiContext& g = *GImGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
	ImGuiWindow * FindWindowByName(const char* name);
	void FocusWindow(ImGuiWindow* window);
	void BringWindowToFront(ImGuiWindow* window);
	void BringWindowToBack(ImGuiWindow* window);
	bool IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent);
	void Initialize();
	void SetActiveID(ImGuiID id, ImGuiWindow* window);
	void ClearActiveID();
	void SetHoveredID(ImGuiID id);
	void KeepAliveID(ImGuiID id);
	void ItemSize(const ImVec2& size, float text_offset_y = 0.0f);
	void ItemSize(const ImRect& bb, float text_offset_y = 0.0f);
	bool ItemAdd(const ImRect& bb, ImGuiID id);
	bool ItemHoverable(const ImRect& bb, ImGuiID id);
	bool IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged);
	bool FocusableItemRegister(ImGuiWindow* window, ImGuiID id, bool tab_stop = true);
	void FocusableItemUnregister(ImGuiWindow* window);
	ImVec2 CalcItemSize(ImVec2 size, float default_x, float default_y);
	float CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
	void PushMultiItemsWidths(int components, float width_full = 0.0f);
	void OpenPopupEx(ImGuiID id);
	void ClosePopup(ImGuiID id);
	bool IsPopupOpen(ImGuiID id);
	bool BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags);
	int CalcTypematicPressedRepeatAmount(float t, float t_prev, float repeat_delay, float repeat_rate);
	void Scrollbar(ImGuiLayoutType direction);
	void VerticalSeparator();
	void BeginColumns(int count);
	void EndColumns();
	void PushColumnClipRect(int column_index = -1);
	void RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
	void RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
	void RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
	void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
	void RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
	void RenderTriangle(ImVec2 pos, ImGuiDir dir, float scale = 1.0f);
	void RenderCheckMark(ImVec2 pos, ImU32 col, float sz);
	const char * FindRenderedTextEnd(const char* text, const char* text_end = NULL);
	void OpenPopupEx(ImGuiID id);
	bool ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
	bool ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
	bool SliderBehavior(const ImRect& frame_bb, ImGuiID id, float* v, float v_min, float v_max, float power, int decimal_precision, ImGuiSliderFlags flags = 0);
	bool InputTextEx(const char* label, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback = NULL);
	bool InputScalarEx(const char* label, ImGuiDataType data_type, void* data_ptr, void* step_ptr, void* step_fast_ptr, const char* scalar_format, ImGuiInputTextFlags extra_flags);
	int ParseFormatPrecision(const char* fmt, int default_value);
	float RoundScalar(float value, int decimal_precision);
	void ShadeVertsLinearColorGradientKeepAlpha(ImDrawVert* vert_start, ImDrawVert* vert_end, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
	void ShadeVertsLinearAlphaGradientForLeftToRightText(ImDrawVert* vert_start, ImDrawVert* vert_end, float gradient_p0_x, float gradient_p1_x);
	void ShadeVertsLinearUV(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp);
};

bool ImFontAtlasBuildWithStbTruetype(ImFontAtlas* atlas);
void ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas* atlas);
void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent);
void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* spc);
void ImFontAtlasBuildFinish(ImFontAtlas* atlas);
void ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char *, short, float);
void ImFontAtlasBuildMultiplyRectAlpha8(unsigned char *, unsigned char *, int, int, int, int, int);
