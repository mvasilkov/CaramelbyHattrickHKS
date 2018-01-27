
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


//
// =============================================== OPTIONS BELOW ===============================================
//


#define ENABLE_RAGE_BOT			1									// 0 = NO, 1 = YES
#define ENABLE_HEAVY_RAGE		0									// 0 = NO, 1 = YES
#define ENABLE_GLOW_ESP_WH		1									// 0 = NO, 1 = YES
#define ENABLE_CHAT_SPAM		0									// 0 = NO, 1 = YES
#define ENABLE_TRIGGER_BOT		1									// 0 = NO, 1 = YES
#define ENABLE_AIM_BOT			1									// 0 = NO, 1 = YES
#define ENABLE_BUNNY_HOP		1									// 0 = NO, 1 = YES
#define ENABLE_AUTO_STRAFE		1									// 0 = NO, 1 = YES
#define ENABLE_CLAN_TAG			1									// 0 = NO, 1 = YES
#define ENABLE_AUTO_ACCEPT		1									// 0 = NO, 1 = YES
#define ENABLE_CUSTOM_FOV		1									// 0 = NO, 1 = YES
#define ENABLE_FAKE_LAG			1									// 0 = NO, 1 = YES

#define CLAN_TAG				"CARAMEL X"							// CAN BE CHANGED, BUT TO BE AT MOST 12 CHARACTERS! IT HAS NOTHING TO DO WITH THE CLAN TAG, IT'S JUST A SMALL VARIABLE.. IF YOU PLAY WITH BOTS, IT WILL SHOW "CARAMEL" WINS THE ROUND.. (BOTS ONLY!)
#define FONT_HEIGHT				10									// DEFAULT PIXELS HEIGHT (HOW TALL) [BETWEEN 8 AND 16]
#define CFG_FILE				"CARAMEL.PRIVATE.INI"				// FILE NAME, MUST BE UNIQUE
#define CFG_SEPARATOR			"///"								// CAN BE ., _, >, <, :, @, #, $, *, -, =, +, ?, ! AND /. CAN BE EVEN @@, ##, !!, :: AND __.
#define MENU_FONT_FILE			"Platform\\VGUI\\Fonts\\Times.TTF"	// MENU FONT FILE (ARIAL.TTF, COUR.TTF, LUCON.TTF, MARLETT.TTF, TAHOMA.TTF, TIMES.TTF, TREBUC.TTF)
#define MENU_TITLE				"CARAMEL by Hattrick HKS"			// MENU TITLE
#define WATERMARK_NAME			"CARAMEL by Hattrick HKS"			// WATERMARK NAME
#define WATERMARK_INFO			"http://hattrick.go.ro/"			// WATERMARK INFORMATION


//
// =============================================== OPTIONS ABOVE ===============================================
//


#define ISINRANGE(X, A, B) (X >= A && X <= B)

#define GETBITS(X) (ISINRANGE((X & (~0x00000020)), 'A', 'F') ? ((X & (~0x00000020)) - 'A' + 0x0000000A) : (ISINRANGE(X, '0', '9') ? X - '0' : 0x00000000))
#define GETBYTE(X) (GETBITS(X[0]) << 4 | GETBITS(X[1]))

#include <Windows.h>
#include <Psapi.h>
#include <d3d9.h>
#include <time.h>

#include <map>
#include <memory>
#include <functional>
#include <algorithm>
#include <fstream>
#include <array>
#include <string>
#include <vector>

using namespace std;

#include "XorString.h"

#include "Hattrick.H"
extern Hattrick g_Hattrick;

#include "json.hpp"
using json = nlohmann::json;

#include "imgui.h"

#include "LegitBot.h"
#include "RageBot.h"

template < typename Type > constexpr FORCEINLINE CONST Type CallVFunc(PVOID pClassBase, INT nFnID)
{
	return (Type)(*(PDWORD *)pClassBase)[nFnID];
};

template < typename Type > constexpr FORCEINLINE CONST Type GetVFunc(PVOID pTable, INT nID)
{
	return (Type)(*(Type **)pTable)[nID];
};

class ICollideable;
class CUtlBuffer;
class Entity_T;
class CVisuals;
class IGameEvent;
class IEnts;
class IVPanel;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

class SurfSoundHandles_T;
class SurfSoundNames_T;
class SurfGameProps_T;
class SurfAudioParams_T;
class SurfPhysParams_T;
class SurfData_T;
class IPhysProps;

#endif

#endif

class Surface_T;
class IInputSystem;
class IEngineClient;
class Variables;
class Quaternion;
class IVModelInfo;
class IGlobalVarsBase;
class ITraceFilter;
class ISurface;
class IEngineTrace;
class IBaseClientDll;
class Vector;
class QAngle;
class CUserCmd;
class CGrenTrajObj;
class CGrenTraj;
class Color;
class CNetVarMan;
class CLBOT;
class AudioState_T;
class GameTrace_T;

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

class CViewSetup;

#endif

class pfnDemoCustomDataCallback;
class ClientClass;
class VTHook;
class Plane_T;
class Matrix44_T;
class COffsets;
class IGameEventManager;
class IClientModeShared;
class CConfig;
class Matrix34_T;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

class CRBOT;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

class WeaponInfo_T;

#endif

#endif

struct IDirect3DDevice9;
struct PlrInfo_T;
struct RecvProp_T;
struct Model_T;
struct RecvTable_T;
struct StdBone_T;
struct StdHdr_T;
struct Ray_T;

class IGameEventManager { };
class IClientModeShared { };

class Matrix34_T
{
public:

	PFLOAT operator [] (INT iId)
	{
		return m_pArray[iId];
	};

	FLOAT m_pArray[3][4];
};

class Matrix44_T
{
public:

	FLOAT m_pArray[4][4];

	PFLOAT operator [] (INT iId)
	{
		return m_pArray[iId];
	};
};

class Variables
{
public:

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

	struct
	{
		bool					bON;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

		bool					bHeavy;

#endif

		FLOAT					fFoV;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

		INT						iMinDamageRifles;
		INT						iMinDamageSnipers;
		INT						iMinDamagePistols;

#endif

	} RageBOT;

#endif

	struct
	{

		struct
		{
			FLOAT				fHitPointsScale;
			bool				bLimitAim;
			FLOAT				fAimLimit;

		} AimHelpers;

#if defined(ENABLE_AIM_BOT) && ENABLE_AIM_BOT != 0

		struct
		{
			bool				bON;
			FLOAT				fFoV;

		} AimBOT;

#endif

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

		struct
		{
			bool				bON;
			bool				bAuto;
			INT					iKey;
			FLOAT				fDelay;

		} TriggerBOT;

#endif

	} LEGITBOT;

	struct
	{
		bool					bON;
		bool					bDrawSkeleton;
		bool					bRadar;
		bool					bDrawHitPoints;
		INT						iHitPointsColor;
		bool					bDrawHitPointsOnlyHead;
		bool					bDrawEnemySight;
		INT						iEnemySightColor;

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

		bool					bGlow;
		INT						iGlowAlphaIntensity;
		FLOAT					fGlowBloomAmount;
		INT						iVisibleGlowColor;
		INT						iOccludedGlowColor;

#endif

		struct
		{
			bool				bWeapons;
			bool				bDecoy;
			bool				bC4Bomb;
			bool				bHostages;
			bool				bEnemyWeapon;
			bool				bEnemyName;
			bool				bEnemyHealth;
			bool				bEnemyReloading;
			bool				bEnemyFlashed;
			bool				bEnemyDefusing;

		} Filter;

	} Visuals;

	struct
	{
		bool					bRevealRanks;
		bool					bAutoLeftRight;

#if defined(ENABLE_TRIGGER_BOT) && ENABLE_TRIGGER_BOT != 0

		bool					bSpecialFlickShots;

#endif

#if defined(ENABLE_AUTO_STRAFE) && ENABLE_AUTO_STRAFE != 0

		bool					bAutoStrafe;

#endif

		bool					bDroppedWeaponsName;
		bool					bAutoSniperCrosshair;
		bool					bBombSiteSkeletons;
		INT						iSniperCrosshairColor;
		bool					bGrenadesPrediction;

#if defined(ENABLE_BUNNY_HOP) && ENABLE_BUNNY_HOP != 0

		bool					bBunnyHop;

#endif

		bool					bGrenadeTraces;
		bool					bDoNotEditRadarScale;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

		bool					bSpam;
		bool					bSpamRandomEnemies;
		bool					bSpamInsultEnemy;
		bool					bSpamInsultVictim;
		bool					bSpamRandomTalk;
		INT						iSpamDelaySeconds;

#endif

		bool					bDoNotEnforceTopSettings;
		bool					bRecoilCrosshair;
		INT						iRecoilCrosshairType;
		bool					bC4BombTimer;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

		bool					bAutoAccept;

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

		bool					bFOV;
		INT						iFOV;
		INT						iViewModelFOV;

		bool					bFOVPistols;
		INT						iFOVPistols;
		INT						iViewModelFOVPistols;

#endif

		bool					bFlashbangReducer;
		INT						iFlashbangAmount;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

		bool					bClanTagRainbow;
		CHAR					szClanTag[XS_RANDOM_SIZE];

#endif

		bool					bDrawFoVCircle;
		INT						iFoVCircleColor;
		INT						iSkeletonColor;
		bool					bEnableEnemyDetails;
		bool					bEnableHackTradeMark;
		INT						iFoVCircleType;

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

		bool					bFakeLag;
		INT						iFakeLagType;

#endif

		bool					bSpecTable;
		FLOAT					fSpecTableX;
		FLOAT					fSpecTableY;
		INT						iSpecTableColor;
		INT						iFontHeight;

	} Misc;

	struct
	{
		bool					bOpened;

	} Menu;
};

class QAngle
{
public:

	FLOAT x, y, z;

	FORCEINLINE QAngle(VOID)
	{
		x = y = z = 0.f;
	};

	FORCEINLINE QAngle(FLOAT X, FLOAT Y, FLOAT Z)
	{
		x = X, y = Y, z = Z;
	};

	FORCEINLINE CONST bool CONST IsZero(VOID) CONST
	{
		return x == 0.f && y == 0.f;
	};

	FORCEINLINE CONST FLOAT CONST Length(VOID) CONST
	{
		return sqrtf(x * x + y * y);
	};

	FORCEINLINE CONST VOID CONST Clamp(VOID)
	{
		if (x < -89.f) x = -89.f;
		if (x > 89.f) x = 89.f;
		while (y < -180.f) y += 360.f;
		while (y > 180.f) y -= 360.f;
		z = 0.f;
	};

	FORCEINLINE QAngle & operator = (CONST QAngle & Other)
	{
		x = Other.x, y = Other.y, z = Other.z;
		return *this;
	};

	FORCEINLINE QAngle & operator += (CONST QAngle & Other)
	{
		x += Other.x, y += Other.y, z += Other.z;
		return *this;
	};

	FORCEINLINE QAngle & operator + (CONST QAngle & Other)
	{
		static QAngle Res;
		Res.x = x + Other.x, Res.y = y + Other.y, Res.z = z + Other.z;
		return Res;
	};

	FORCEINLINE QAngle & operator -= (CONST QAngle & Other)
	{
		x -= Other.x, y -= Other.y, z -= Other.z;
		return *this;
	};

	FORCEINLINE QAngle & operator - (CONST QAngle & Other)
	{
		static QAngle Res;
		Res.x = x - Other.x, Res.y = y - Other.y, Res.z = z - Other.z;
		return Res;
	};

	FORCEINLINE QAngle & operator *= (FLOAT fVal)
	{
		x *= fVal, y *= fVal, z *= fVal;
		return *this;
	};

	FORCEINLINE QAngle & operator * (CONST QAngle & Other)
	{
		static QAngle Res;
		Res.x = x * Other.x, Res.y = y * Other.y, Res.z = z * Other.z;
		return Res;
	};

	FORCEINLINE QAngle & operator * (FLOAT fVal)
	{
		static QAngle Other;
		Other.x = x * fVal, Other.y = y * fVal, Other.z = z * fVal;
		return Other;
	};

	FORCEINLINE QAngle & operator /= (FLOAT fVal)
	{
		x /= fVal, y /= fVal, z /= fVal;
		return *this;
	};

	FORCEINLINE QAngle & operator / (CONST QAngle & Other)
	{
		static QAngle Res;
		Res.x = x / Other.x, Res.y = y / Other.y, Res.z = z / Other.z;
		return Res;
	};

	FORCEINLINE QAngle & operator / (FLOAT fVal)
	{
		static QAngle Other;
		Other.x = x / fVal, Other.y = y / fVal, Other.z = z / fVal;
		return Other;
	};
};

class Vector
{
public:

	FLOAT x, y, z;

	FORCEINLINE Vector(VOID) { x = y = z = 0.f; };
	FORCEINLINE Vector(FLOAT X, FLOAT Y, FLOAT Z) { x = X, y = Y, z = Z; };

	FORCEINLINE CONST VOID CONST Zero(VOID) { x = y = z = 0.f; };
	FORCEINLINE CONST FLOAT CONST Length(VOID) CONST { return sqrtf(x * x + y * y + z * z); };
	FORCEINLINE CONST FLOAT CONST LengthSqr(VOID) CONST { return x * x + y * y + z * z; };
	FORCEINLINE CONST FLOAT CONST Length2D(VOID) CONST { return sqrtf(x * x + y * y); };
	FORCEINLINE CONST bool CONST IsZero(VOID) CONST { return x == 0.f && y == 0.f && z == 0.f; };
	FORCEINLINE CONST FLOAT CONST Dot(CONST Vector & Other) CONST { return x * Other.x + y * Other.y + z * Other.z; };
	FORCEINLINE CONST FLOAT CONST Dot(FLOAT * pData) CONST { return x * pData[0] + y * pData[1] + z * pData[2]; };
	FORCEINLINE CONST FLOAT CONST Dot(Vector & Other) CONST { return x * Other.x + y * Other.y + z * Other.z; };
	FORCEINLINE CONST FLOAT CONST Dot(CONST FLOAT * pData) CONST { return x * pData[0] + y * pData[1] + z * pData[2]; };

	FORCEINLINE CONST VOID CONST Normalize(VOID)
	{
		static FLOAT fLen = 0.f;
		fLen = Length();
		if (fLen != 0.f) *this /= fLen;
		else x = y = 0.f, z = 1.f;
	};

	FORCEINLINE Vector & operator = (CONST Vector & Other)
	{
		x = Other.x, y = Other.y, z = Other.z;
		return *this;
	};

	FORCEINLINE Vector & operator += (CONST Vector & Other)
	{
		x += Other.x, y += Other.y, z += Other.z;
		return *this;
	};

	FORCEINLINE Vector & operator + (CONST Vector & Other)
	{
		static Vector Res;
		Res.x = x + Other.x, Res.y = y + Other.y, Res.z = z + Other.z;
		return Res;
	};

	FORCEINLINE CONST Vector & operator + (CONST Vector & Other) CONST
	{
		static Vector Res;
		Res.x = x + Other.x, Res.y = y + Other.y, Res.z = z + Other.z;
		return Res;
	};

	FORCEINLINE Vector & operator -= (CONST Vector & Other)
	{
		x -= Other.x, y -= Other.y, z -= Other.z;
		return *this;
	};

	FORCEINLINE Vector & operator - (CONST Vector & Other)
	{
		static Vector Res;
		Res.x = x - Other.x, Res.y = y - Other.y, Res.z = z - Other.z;
		return Res;
	};

	FORCEINLINE CONST Vector & operator - (CONST Vector & Other) CONST
	{
		static Vector Res;
		Res.x = x - Other.x, Res.y = y - Other.y, Res.z = z - Other.z;
		return Res;
	};

	FORCEINLINE Vector & operator *= (FLOAT fVal)
	{
		x *= fVal, y *= fVal, z *= fVal;
		return *this;
	};

	FORCEINLINE Vector & operator * (CONST Vector & Other)
	{
		static Vector Res;
		Res.x = x * Other.x, Res.y = y * Other.y, Res.z = z * Other.z;
		return Res;
	};

	FORCEINLINE Vector & operator * (FLOAT fVal)
	{
		static Vector Other;
		Other.x = x * fVal, Other.y = y * fVal, Other.z = z * fVal;
		return Other;
	};

	FORCEINLINE Vector & operator /= (FLOAT fVal)
	{
		x /= fVal, y /= fVal, z /= fVal;
		return *this;
	};

	FORCEINLINE Vector & operator / (CONST Vector & Other)
	{
		static Vector Res;
		Res.x = x / Other.x, Res.y = y / Other.y, Res.z = z / Other.z;
		return Res;
	};

	FORCEINLINE Vector & operator / (FLOAT fVal)
	{
		static Vector Other;
		Other.x = x / fVal, Other.y = y / fVal, Other.z = z / fVal;
		return Other;
	};
};

class Color
{
public:

	FORCEINLINE Color(VOID)
	{
		Set((BYTE)255, (BYTE)255, (BYTE)255, (BYTE)255);
	};

	FORCEINLINE Color(BYTE R, BYTE G, BYTE B, BYTE A = (BYTE)255)
	{
		Set(R, G, B, A);
	};

	FORCEINLINE VOID Set(BYTE R, BYTE G, BYTE B, BYTE A = 255)
	{
		m_Col[0] = (BYTE)R, m_Col[1] = (BYTE)G, m_Col[2] = (BYTE)B, m_Col[3] = (BYTE)A;
	};

	FORCEINLINE Vector ToVec3DBase(VOID)
	{
		return Vector(m_Col[0] / 255.f, m_Col[1] / 255.f, m_Col[2] / 255.f);
	};

	FORCEINLINE static Color Red(VOID)
	{
		return Color((BYTE)255, (BYTE)0, (BYTE)0);
	};

	FORCEINLINE static Color DarkRed(VOID)
	{
		return Color((BYTE)255 / 2, (BYTE)0, (BYTE)0);
	};

	FORCEINLINE static Color Green(VOID)
	{
		return Color((BYTE)0, (BYTE)255, (BYTE)0);
	};

	FORCEINLINE static Color DarkGreen(VOID)
	{
		return Color((BYTE)0, (BYTE)255 / 2, (BYTE)0);
	};

	FORCEINLINE static Color Blue(VOID)
	{
		return Color((BYTE)0, (BYTE)0, (BYTE)255);
	};

	FORCEINLINE static Color DarkBlue(VOID)
	{
		return Color((BYTE)0, (BYTE)0, (BYTE)255 / 2);
	};

	FORCEINLINE static Color White(VOID)
	{
		return Color((BYTE)255, (BYTE)255, (BYTE)255);
	};

	FORCEINLINE static Color WhiteSmoke(VOID)
	{
		return Color((BYTE)255 * .75f, (BYTE)255 * .75f, (BYTE)255 * .75f);
	};

	FORCEINLINE static Color Black(VOID)
	{
		return Color((BYTE)0, (BYTE)0, (BYTE)0);
	};

	FORCEINLINE static Color Grey(VOID)
	{
		return Color((BYTE)255 / 2, (BYTE)255 / 2, (BYTE)255 / 2);
	};

	FORCEINLINE static Color DarkGrey(VOID)
	{
		return Color((BYTE)255 / 4, (BYTE)255 / 4, (BYTE)255 / 4);
	};

	FORCEINLINE static Color Yellow(VOID)
	{
		return Color((BYTE)255, (BYTE)255, (BYTE)0);
	};

	FORCEINLINE static Color Violet(VOID)
	{
		return Color((BYTE)255 / 2, (BYTE)0, (BYTE)255);
	};

	FORCEINLINE static Color Magenta(VOID)
	{
		return Color((BYTE)255, (BYTE)0, (BYTE)255);
	};

	FORCEINLINE static Color Orange(VOID)
	{
		return Color((BYTE)255, (BYTE)255 / 2, (BYTE)0);
	};

	FORCEINLINE static Color Cyan(VOID)
	{
		return Color((BYTE)0, (BYTE)255, (BYTE)255);
	};

	FORCEINLINE static Color CandyRed(VOID)
	{
		return Color((BYTE)255, (BYTE)0, (BYTE)255 / 2);
	};

	FORCEINLINE static Color CandyBlue(VOID)
	{
		return Color((BYTE)0, (BYTE)255 / 2, (BYTE)255);
	};

	FORCEINLINE static Color CandyGreen(VOID)
	{
		return Color((BYTE)0, (BYTE)255, (BYTE)255 / 2);
	};

	BYTE m_Col[4];
};

class Plane_T
{
public:

	Vector normal;
	FLOAT dist;
	BYTE type;
	BYTE signbits;
	BYTE __UNUSED__[2];
};

class Surface_T
{
public:

	CONST CHAR * name;
	SHORT surfaceProps;
	USHORT flags;
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

class SurfSoundHandles_T
{
public:

	SHORT walkStepLeft;
	SHORT walkStepRight;
	SHORT runStepLeft;
	SHORT runStepRight;
	SHORT impactSoft;
	SHORT impactHard;
	SHORT scrapeSmooth;
	SHORT scrapeRough;
	SHORT bulletImpact;
	SHORT rolling;
	SHORT breakSound;
	SHORT strainSound;
};

class SurfSoundNames_T
{
public:

	USHORT walkStepLeft;
	USHORT walkStepRight;
	USHORT runStepLeft;
	USHORT runStepRight;
	USHORT impactSoft;
	USHORT impactHard;
	USHORT scrapeSmooth;
	USHORT scrapeRough;
	USHORT bulletImpact;
	USHORT rolling;
	USHORT breakSound;
	USHORT strainSound;
};

class SurfGameProps_T
{
public:

	FLOAT maxSpeedFactor;
	FLOAT jumpFactor;
	CHAR pad00[0x00000004];
	FLOAT flPenetrationModifier;
	FLOAT flDamageModifier;
	USHORT material;
	FLOAT pad01[0x00000003];
};

class SurfAudioParams_T
{
public:

	FLOAT reflectivity;
	FLOAT hardnessFactor;
	FLOAT roughnessFactor;
	FLOAT roughThreshold;
	FLOAT hardThreshold;
	FLOAT hardVelocityThreshold;
};

class SurfPhysParams_T
{
public:

	FLOAT friction;
	FLOAT elasticity;
	FLOAT density;
	FLOAT thickness;
	FLOAT dampening;
};

class SurfData_T
{
public:

	SurfPhysParams_T physics;
	SurfAudioParams_T audio;
	SurfSoundNames_T sounds;
	SurfGameProps_T game;
	SurfSoundHandles_T soundhandles;
};

#endif

#endif

class IEnts
{
public:

	virtual VOID Fn_A(VOID) = NULL, Fn_B(VOID) = NULL, Fn_C(VOID) = NULL;
	virtual Entity_T * GetClEntity(INT) = NULL, GetEntityFromHndl(DWORD) = NULL;
	virtual INT NumberOfEntities(bool) = NULL, GetHighestEntityIndex(VOID) = NULL;
};

class ICollideable
{
public:

	virtual VOID Dummy(VOID);

	virtual CONST Vector & OBBMins(VOID) CONST;
	virtual CONST Vector & OBBMaxs(VOID) CONST;
};

class IGameEvent
{
public:

	CONST CHAR * GetName(VOID)
	{
		return CallVFunc < CONST CHAR * (__thiscall *) (PVOID) >(this, 1) (this);
	};

	INT GetInt(CONST CHAR * pszKey, INT iDefVal = NULL)
	{
		return CallVFunc < INT(__thiscall *) (PVOID, CONST CHAR *, INT) >(this, 6) (this, pszKey, iDefVal);
	};

	CONST CHAR * GetString(CONST CHAR * pszKey)
	{
		return CallVFunc < CONST CHAR * (__thiscall *) (PVOID, CONST CHAR *, INT) >(this, 9) (this, pszKey, NULL);
	};

	VOID SetString(CONST CHAR * pszKey, CONST CHAR * pszVal)
	{
		return CallVFunc < VOID(__thiscall *) (PVOID, CONST CHAR *, CONST CHAR *) >(this, 16) (this, pszKey, pszVal);
	};
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

class IPhysProps
{
public:

	virtual ~IPhysProps(VOID) {};

	virtual INT ParseSurfaceData(CONST CHAR *, CONST CHAR *) = NULL;
	virtual INT SurfacePropCount(VOID) CONST = NULL;
	virtual INT GetSurfaceIndex(CONST CHAR *) CONST = NULL;
	virtual VOID GetPhysicsProperties(INT, FLOAT *, FLOAT *, FLOAT *, FLOAT *) CONST = NULL;
	virtual SurfData_T * GetSurfaceData(INT) = NULL;
	virtual CONST CHAR * GetString(USHORT) CONST = NULL;
	virtual CONST CHAR * GetPropName(INT) CONST = NULL;
	virtual VOID SetWorldMaterialIndexTable(INT *, INT) = NULL;
	virtual VOID GetPhysicsParameters(INT, SurfPhysParams_T *) CONST = NULL;
};

#endif

#endif

class IInputSystem
{
public:

	VOID GetCursorPosition(PINT, PINT);
};

class VTHook
{
public:

	FORCEINLINE VTHook(VOID)
	{
		ZeroMemory(this, sizeof VTHook);
	};

	FORCEINLINE VTHook(PDWORD * ppdwClassBase)
	{
		Startup(ppdwClassBase);
	};

	FORCEINLINE ~VTHook(VOID)
	{
		UnHook();
	};

	FORCEINLINE VOID Startup(PDWORD * ppdwClassBase)
	{
		m_ppdwClassBase = ppdwClassBase;
		m_pdwOldVT = *ppdwClassBase;
		m_dwVTSize = GetVTCount(*ppdwClassBase);
		m_pdwNewVT = new DWORD[m_dwVTSize];
		CopyMemory(m_pdwNewVT, m_pdwOldVT, sizeof(DWORD) * m_dwVTSize);
		*ppdwClassBase = m_pdwNewVT;
	};

	FORCEINLINE VOID UnHook(VOID)
	{
		if (m_ppdwClassBase) *m_ppdwClassBase = m_pdwOldVT;
	};

	FORCEINLINE VOID ReHook(VOID)
	{
		if (m_ppdwClassBase) *m_ppdwClassBase = m_pdwNewVT;
	};

	FORCEINLINE DWORD HookFunction(DWORD dwNewFunc, INT nID)
	{
		if (m_pdwNewVT && m_pdwOldVT && nID <= m_dwVTSize && nID >= NULL)
		{
			m_pdwNewVT[nID] = dwNewFunc;
			return m_pdwOldVT[nID];
		};
		return NULL;
	};

	FORCEINLINE DWORD GetVTCount(PDWORD pdwVMT)
	{
		DWORD dwID = 0;
		for (dwID; pdwVMT[dwID]; dwID++)
		{
			if (IsBadCodePtr((FARPROC)pdwVMT[dwID])) break;
		};
		return dwID;
	};

	PDWORD * m_ppdwClassBase;
	PDWORD m_pdwNewVT, m_pdwOldVT;
	DWORD m_dwVTSize;
};

class CVisuals
{
public:

	VOID Run(VOID), Glow(VOID), Player(Entity_T *, Vector, Vector), Entity(Entity_T *, Vector, Vector), Skeleton(Entity_T *);
};

class COffsets
{
public:

	DWORD m_bSpotted;
	DWORD m_iHealth;
	DWORD m_iTeamNum;
	DWORD m_bDormant;
	DWORD m_bGunGameImmunity;
	DWORD m_flElasticity;
	DWORD m_bIsDefusing;
	DWORD m_flFlashDuration;
	DWORD m_iObserverMode;
	DWORD m_hObserverTarget;
	DWORD m_fFlags;
	DWORD m_nTickBase;
	DWORD m_aimPunchAngle;
	DWORD m_bHasDefuser;
	DWORD m_vecOrigin;
	DWORD m_vecViewOffset;
	DWORD m_Collision;
	DWORD m_vecVelocity;
	DWORD m_flNextPrimaryAttack;
	DWORD m_hActiveWeapon;
	DWORD m_bIsScoped;
	DWORD m_nHitboxSet;
	DWORD m_angEyeAngles;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	DWORD m_fAccuracyPenalty;

#endif

#endif

	DWORD m_flC4Blow;
	DWORD m_flFlashMaxAlpha;
	DWORD m_iItemDefinitionIndex;
	DWORD m_iClip1;

	DWORD m_dwD3DDevice;

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

	DWORD m_dwGlowManager;

#endif

	DWORD m_dwServerRankRevealAllEx;
};

#if defined(ENABLE_GLOW_ESP_WH) && ENABLE_GLOW_ESP_WH != 0

extern Variables Vars;

class CGlowObjMan
{
public:

	class GlowObjDef_T
	{
	public:

		FORCEINLINE VOID Set(Color Col)
		{
			m_vecGlowCol = Col.ToVec3DBase(), m_flGlowAlpha = (((BYTE)(Vars.Visuals.iGlowAlphaIntensity)) / 255.f), m_bRenderWhenOccl = true, m_bRenderWhenUnoccl = false, \
				m_flBloomAmt = Vars.Visuals.fGlowBloomAmount;
		};

		FORCEINLINE VOID UnSet(VOID)
		{
			m_flGlowAlpha = m_flBloomAmt = 0.f, m_bRenderWhenOccl = m_bRenderWhenUnoccl = false;
		};

		Entity_T * m_pEntity;
		Vector m_vecGlowCol;
		FLOAT m_flGlowAlpha;
		CHAR m_szUnkA[4];
		FLOAT m_fUnkA;
		FLOAT m_flBloomAmt;
		FLOAT m_fUnkB;
		bool m_bRenderWhenOccl;
		bool m_bRenderWhenUnoccl;
		bool m_bFullBloomRender;
		CHAR m_szUnkB[1];
		INT m_nFullBloomStencilTestVal;
		INT m_iUnkA;
		INT m_nSplitScreenSlot;
		INT m_nNextFreeSlot;
	};

	GlowObjDef_T * m_pGlowObjDefs;

	INT m_nMaxSize;
	INT m_nPad;
	INT m_nSize;
};

#endif

class IGlobalVarsBase
{
public:

	FLOAT m_fRealTime;
	INT m_iFrameCount;
	FLOAT m_fAbsFrameTime;
	FLOAT m_fAbsFrameStartTime;
	FLOAT m_fCurTime;
	FLOAT m_fFrameTime;
	INT m_iMaxClients;
	INT m_iTickCount;
	FLOAT m_fIntervPerTick;
	FLOAT m_fInterpAmt;
	INT m_iSimTicks;
	INT m_iNetworkProto;
	PVOID m_pSaveData;
	bool m_bClient;
	bool m_bRemClient;
	INT m_iTStampNetworkBase;
	INT m_iTStampRandomWnd;
};

class ITraceFilter
{
public:

	virtual bool ShouldHitEntity(Entity_T * pEntityHandle, INT)
	{
		return pEntityHandle != pSkip;
	};

	virtual INT	GetTraceType(VOID)
	{
		return NULL;
	};

	PVOID pSkip;
};

class TraceFilter_T : public ITraceFilter
{
public:

	virtual bool ShouldHitEntity(Entity_T * pEntityHandle, INT)
	{
		return pEntityHandle != pSkip;
	};

	virtual INT	GetTraceType(VOID) CONST
	{
		return NULL;
	};

	PVOID pSkip;
};

class BaseTrace_T
{
public:

	Vector startpos;
	Vector endpos;
	Plane_T plane;
	FLOAT fraction;
	INT contents;
	USHORT dispFlags;
	bool allsolid;
	bool startsolid;
};

class GameTrace_T : public BaseTrace_T
{
public:

	FLOAT fractionleftsolid;
	Surface_T surface;
	INT hitgroup;
	SHORT physicsbone;
	USHORT worldSurfaceIndex;
	Entity_T * ent;
	INT hitbox;
};

class CUserCmd
{
public:

	virtual ~CUserCmd(VOID) { };
	INT cmdnum;
	INT tickcnt;
	QAngle viewang;
	Vector aimdir;
	FLOAT fwdmove;
	FLOAT sidemove;
	FLOAT upmove;
	INT buttons;
	BYTE impulse;
	INT weaponselect;
	INT weaponsubtype;
	INT randseed;
	SHORT mousedx;
	SHORT mousedy;
};

class Entity_T
{
public:

	CHAR m_Dummy[0x00000064];
	INT m_iId;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	WeaponInfo_T * GetSpecialData(VOID);
	FLOAT GetAccuracyPenalty(VOID);

#endif

#endif

	ICollideable * GetCollideable(VOID);
	VOID SetFlashMaxAlpha(FLOAT);
	VOID SetSpotted(VOID);
	INT GetHealth(VOID);
	INT GetTeam(VOID);
	INT GetFlags(VOID);
	FLOAT GetElasticity(VOID);
	INT GetTickBase(VOID);
	INT GetHBoxSet(VOID);
	bool IsDormant(VOID);
	bool GetImmune(VOID);
	bool IsVisible(VOID);
	Entity_T * GetTarget(VOID);
	FLOAT GetBombTimer(VOID);
	QAngle GetPunch(VOID);
	QAngle GetEyeAng(VOID);
	Vector GetOrigin(VOID);
	Vector GetEyePos(VOID);
	bool IsScoped(VOID);
	bool SetupBones(Matrix34_T *);
	Vector GetVelocity(VOID);
	Model_T * GetModel(VOID);
	Entity_T * GetWeapon(VOID);
	INT GetMode(VOID);
	ClientClass * GetClientClass(VOID);
	bool IsSpottingMe(VOID);
	bool IsSpottingMyHead(VOID);
	FLOAT GetNextPrimaryAttack(VOID);
	INT GetItemDefinitionIndex(VOID);
	bool IsEmpty(VOID);
	bool IsGun(INT);
	bool IsDefusing(VOID);
	bool HasDefuser(VOID);
	FLOAT GetFlashDuration(VOID);
	string GetWeaponName(INT);
	string GetPlayerName(VOID);
};

class CConfig
{
public:

	VOID SetupVal(INT &, INT, string, string), SetupVal(bool &, bool, string, string), SetupVal(FLOAT &, FLOAT, string, string);

	FORCEINLINE CConfig(VOID)
	{
		LoadStartup(), Setup();
	};

	VOID Setup(VOID), Save(VOID), Load(VOID), LoadStartup(VOID), LoadSpecialBase(VOID), LoadLegitBase(VOID), LoadRageBase(VOID), Panic(VOID);

	vector < pair < PINT, string > > g_pInts;
	vector < pair < PFLOAT, string > > g_pFloats;
	vector < pair < bool *, string > > g_pBools;
};

class ISurface
{
public:

	VOID DrawSetColor(Color), DrawSetTextFont(ULONG), DrawSetTextColor(Color), DrawSetTextPos(INT, INT), DrawLine(INT, INT, INT, INT), DrawPrintText(CONST WCHAR *, INT), DrawSetTexture(INT);
	ULONG AddFont(VOID);
	bool SetFontGlyphSet(ULONG, CONST CHAR *, INT, INT, INT, INT, INT, INT, INT);
	VOID GetTextSize(ULONG, CONST WCHAR *, INT &, INT &);
};

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

class CViewSetup
{
public:

	INT x, x_old;
	INT y, y_old;
	INT width, width_old;
	INT height, height_old;
	bool m_bOrtho;
	FLOAT m_OrthoLeft;
	FLOAT m_OrthoTop;
	FLOAT m_OrthoRight;
	FLOAT m_OrthoBottom;
	bool m_bCustomViewMatrix;
	Matrix34_T m_matCustomViewMatrix;
	CHAR pad_0x68[0x00000048];
	FLOAT fov;
	FLOAT fovViewmodel;
	Vector origin;
	QAngle angles;
	FLOAT zNear;
	FLOAT zFar;
	FLOAT zNearViewmodel;
	FLOAT zFarViewmodel;
	FLOAT m_flAspectRatio;
	FLOAT m_flNearBlurDepth;
	FLOAT m_flNearFocusDepth;
	FLOAT m_flFarFocusDepth;
	FLOAT m_flFarBlurDepth;
	FLOAT m_flNearBlurRadius;
	FLOAT m_flFarBlurRadius;
	INT m_nDoFQuality;
	INT m_nMotionBlurMode;
	FLOAT m_flShutterTime;
	Vector m_vShutterOpenPosition;
	QAngle m_shutterOpenAngles;
	Vector m_vShutterClosePosition;
	QAngle m_shutterCloseAngles;
	FLOAT m_flOffCenterTop;
	FLOAT m_flOffCenterBottom;
	FLOAT m_flOffCenterLeft;
	FLOAT m_flOffCenterRight;
	INT m_EdgeBlur;
};

#endif

class IVPanel
{
public:

	CONST CHAR * GetName(INT);
};

class Quaternion
{
public:

	FLOAT m_pUnused[4];
};

class IBaseClientDll
{
public:

	ClientClass * GetAllClasses(VOID);
};

class ClientClass
{
public:

	PVOID(*m_fnCreate) (INT, INT);
	PVOID(*m_fnCreateEvent) (VOID);
	PCHAR m_pNetworkName;
	RecvTable_T * m_pRecvTable;
	ClientClass * m_pNext;
	INT m_ClassID;
};

class CNetVarMan
{
public:

	VOID Run(VOID);
	INT GetOffset(CONST CHAR *, CONST CHAR *);
	INT Get_Prop(CONST CHAR *, CONST CHAR *, RecvProp_T **);
	INT Get_Prop(RecvTable_T *, CONST CHAR *, RecvProp_T **);
	RecvTable_T * GetTable(CONST CHAR *);
	vector < RecvTable_T * > m_vecTables;
};

class IVModelInfo
{
public:

	virtual ~IVModelInfo(VOID) { };
	virtual CONST Model_T * GetModel(INT) CONST = NULL;
	virtual INT GetModelIndex(CONST CHAR *) CONST = NULL;
	virtual CONST CHAR * GetModelName(CONST Model_T *) CONST = NULL;
	virtual PVOID GetVCollide(CONST Model_T *) CONST = NULL;
	virtual PVOID GetVCollide(INT) CONST = NULL;
	virtual VOID GetModelBounds(CONST Model_T *, Vector &, Vector &) CONST = NULL;
	virtual	VOID GetModelRenderBounds(CONST Model_T *, Vector &, Vector &) CONST = NULL;
	virtual INT GetModelFrameCount(CONST Model_T *) CONST = NULL;
	virtual INT GetModelType(CONST Model_T *) CONST = NULL;
	virtual PVOID GetModelExtraData(CONST Model_T *) = NULL;
	virtual bool ModelHasMaterialProxy(CONST Model_T *) CONST = NULL;
	virtual bool IsTranslucent(Model_T CONST *) CONST = NULL;
	virtual bool IsTranslucentTwoPass(CONST Model_T *) CONST = NULL;
	virtual VOID Dummy(VOID) { };
	virtual INT ComputeTranslucencyType(CONST Model_T *, INT, INT) = NULL;
	virtual INT GetModelMaterialCount(CONST Model_T *) CONST = NULL;
	virtual VOID GetModelMaterials(CONST Model_T *, INT, VOID **) = NULL;
	virtual bool IsModelVertexLit(CONST Model_T *) CONST = NULL;
	virtual CONST CHAR * GetModelKeyValueText(CONST Model_T *) = NULL;
	virtual bool GetModelKeyValue(CONST Model_T *, CUtlBuffer &) = NULL;
	virtual FLOAT GetModelRadius(CONST Model_T *) = NULL;
	virtual CONST StdHdr_T * FindModel(CONST StdHdr_T *, VOID **, CONST CHAR *) CONST = NULL;
	virtual CONST StdHdr_T * FindModel(PVOID) CONST = NULL;
	virtual	PVOID GetVirtualModel(CONST StdHdr_T *) CONST = NULL;
	virtual PBYTE GetAnimBlock(CONST StdHdr_T *, INT) CONST = NULL;
	virtual bool HasAnimBlockBeenPreloaded(StdHdr_T CONST *, INT) CONST = NULL;
	virtual VOID GetModelMaterialColorAndLighting(CONST Model_T *, Vector CONST &, QAngle CONST &, GameTrace_T *, Vector &, Vector &) = NULL;
	virtual VOID GetIlluminationPoint(CONST Model_T *, VOID *, Vector CONST &, QAngle CONST &, Vector *) = NULL;
	virtual INT GetModelContents(INT) CONST = NULL;
	virtual StdHdr_T * GetStudioModel(CONST Model_T *) = NULL;
	virtual INT GetModelSpriteWidth(CONST Model_T *) CONST = NULL;
	virtual INT GetModelSpriteHeight(CONST Model_T *) CONST = NULL;
	virtual VOID SetLevelScreenFadeRange(FLOAT, FLOAT) = NULL;
	virtual VOID GetLevelScreenFadeRange(PFLOAT, PFLOAT) CONST = NULL;
	virtual VOID SetViewScreenFadeRange(FLOAT, FLOAT) = NULL;
	virtual UCHAR ComputeLevelScreenFade(CONST Vector &, FLOAT, FLOAT) CONST = NULL;
	virtual UCHAR ComputeViewScreenFade(CONST Vector &, FLOAT, FLOAT) CONST = NULL;
	virtual INT GetAutoplayList(CONST StdHdr_T *, USHORT **) CONST = NULL;
	virtual PVOID GetCollideForVirtualTerrain(INT) = NULL;
	virtual bool IsUsingFBTexture(CONST Model_T *, INT, INT, PVOID) CONST = NULL;
	virtual CONST Model_T *FindOrLoadModel(CONST CHAR *) CONST = NULL;
	virtual USHORT GetCacheHandle(CONST Model_T *) CONST = NULL;
	virtual INT GetBrushModelPlaneCount(CONST Model_T *) CONST = NULL;
	virtual VOID GetBrushModelPlane(CONST Model_T *, INT, Plane_T &, Vector *) CONST = NULL;
	virtual INT GetSurfacepropsForVirtualTerrain(INT) = NULL;
	virtual bool UsesEnvCubemap(CONST Model_T *) CONST = NULL;
	virtual bool UsesStaticLighting(CONST Model_T *) CONST = NULL;
};

class IEngineClient
{
public:

	virtual INT GetIntersectingSurfaces(CONST Model_T *, CONST Vector &, CONST FLOAT, CONST bool, VOID *, CONST INT) = NULL;
	virtual Vector GetLightForPoint(CONST Vector &, bool) = NULL;
	virtual PVOID TraceLineMaterialAndLighting(CONST Vector &, CONST Vector &, Vector &, Vector &) = NULL;
	virtual CONST CHAR * ParseFile(CONST CHAR *, CHAR *, INT) = NULL;
	virtual bool CopyFile(CONST CHAR *, CONST CHAR *) = NULL;
	virtual VOID GetScreenSize(INT &, INT &) = NULL;
	virtual VOID ServerCmd(CONST CHAR *, bool) = NULL;
	virtual VOID ClientCmd(CONST CHAR *) = NULL;
	virtual bool GetPlayerInfo(INT, VOID *) = NULL;
	virtual INT GetPlayerForUserID(INT) = NULL;
	virtual PVOID TextMessageGet(CONST CHAR *) = NULL;
	virtual bool Con_IsVisible(VOID) = NULL;
	virtual INT GetSelf(VOID) = NULL;
	virtual CONST Model_T * LoadModel(CONST CHAR *, bool) = NULL;
	virtual FLOAT GetLastTimeStamp(VOID) = NULL;
	virtual PVOID GetSentence(VOID *) = NULL;
	virtual FLOAT GetSentenceLength(VOID *) = NULL;
	virtual bool IsStreaming(VOID *) CONST = NULL;
	virtual VOID GetViewAngles(QAngle &) = NULL;
	virtual VOID SetViewAngles(QAngle &) = NULL;
	virtual INT GetMaxClients(VOID) = NULL;
	virtual CONST CHAR * Key_LookupBinding(CONST CHAR *) = NULL;
	virtual CONST CHAR * Key_BindingForKey(INT &) = NULL;
	virtual VOID Key_SetBinding(INT, CHAR CONST *) = NULL;
	virtual VOID StartKeyTrapMode(VOID) = NULL;
	virtual bool CheckDoneKeyTrapping(INT &) = NULL;
	virtual bool IsInGame(VOID) = NULL;
	virtual bool IsConnected(VOID) = NULL;
	virtual bool IsDrawingLoadingImage(VOID) = NULL;
	virtual VOID HideLoadingPlaque(VOID) = NULL;
	virtual VOID Con_NPrintf(INT, CONST CHAR *, ...) = NULL;
	virtual VOID Con_NXPrintf(CONST VOID *, CONST CHAR *, ...) = NULL;
	virtual INT IsBoxVisible(CONST Vector &, CONST Vector &) = NULL;
	virtual INT IsBoxInViewCluster(CONST Vector &, CONST Vector &) = NULL;
	virtual bool CullBox(CONST Vector &, CONST Vector &) = NULL;
	virtual VOID Sound_ExtraUpdate(VOID) = NULL;
	virtual CONST CHAR * GetGameDirectory(VOID) = NULL;
	virtual CONST Matrix44_T & WorldToScreenMatrix(VOID) = NULL;
	virtual CONST Matrix44_T & WorldToViewMatrix(VOID) = NULL;
	virtual INT GameLumpVersion(INT) CONST = NULL;
	virtual INT GameLumpSize(INT) CONST = NULL;
	virtual bool LoadGameLump(INT, VOID *, INT) = NULL;
	virtual INT LevelLeafCount(VOID) CONST = NULL;
	virtual PVOID GetBSPTreeQuery(VOID) = NULL;
	virtual VOID LinearToGamma(FLOAT *, FLOAT *) = NULL;
	virtual FLOAT LightStyleValue(INT) = NULL;
	virtual VOID ComputeDynamicLighting(CONST Vector &, CONST Vector *, Vector &) = NULL;
	virtual VOID GetAmbientLightColor(Vector &) = NULL;
	virtual INT GetDXSupportLevel(VOID) = NULL;
	virtual bool SupportsHDR(VOID) = NULL;
	virtual VOID Mat_Stub(VOID *) = NULL;
	virtual VOID GetChapterName(CHAR *, INT) = NULL;
	virtual CHAR CONST * GetLevelName(VOID) = NULL;
	virtual CHAR CONST * GetLevelNameShort(VOID) = NULL;
	virtual CHAR CONST * GetMapGroupName(VOID) = NULL;
	virtual PVOID GetVoiceTweakAPI(VOID) = NULL;
	virtual VOID SetVoiceCasterID(UINT) = NULL;
	virtual VOID EngineStats_BeginFrame(VOID) = NULL;
	virtual VOID EngineStats_EndFrame(VOID) = NULL;
	virtual VOID FireEvents(VOID) = NULL;
	virtual INT GetLeavesArea(PUSHORT, INT) = NULL;
	virtual bool DoesBoxTouchAreaFrustum(CONST Vector &, CONST Vector &, INT) = NULL;
	virtual INT GetFrustumList(VOID **, INT) = NULL;
	virtual bool ShouldUseAreaFrustum(INT) = NULL;
	virtual VOID SetAudioState(CONST AudioState_T &) = NULL;
	virtual INT SentenceGroupPick(INT, PCHAR, INT) = NULL;
	virtual INT SentenceGroupPickSequential(INT, CHAR *, INT, INT, INT) = NULL;
	virtual INT SentenceIndexFromName(CONST CHAR *) = NULL;
	virtual CONST CHAR * SentenceNameFromIndex(INT) = NULL;
	virtual INT SentenceGroupIndexFromName(CONST CHAR *) = NULL;
	virtual CONST CHAR * SentenceGroupNameFromIndex(INT) = NULL;
	virtual FLOAT SentenceLength(INT) = NULL;
	virtual VOID ComputeLighting(CONST Vector &, CONST Vector *, bool, Vector &, Vector * = NULL) = NULL;
	virtual VOID ActivateOccluder(INT, bool) = NULL;
	virtual bool IsOccluded(CONST Vector &, CONST Vector &) = NULL;
	virtual INT GetOcclusionViewId(VOID) = NULL;
	virtual VOID * SaveAllocMemory(size_t, size_t) = NULL;
	virtual VOID SaveFreeMemory(VOID *) = NULL;
	virtual VOID * GetNetChannelInfo(VOID) = NULL;
	virtual VOID DebugDrawPhysCollide(CONST VOID *, VOID *, CONST Matrix34_T &, CONST Color &) = NULL;
	virtual VOID CheckPoint(CONST CHAR *) = NULL;
	virtual VOID DrawPortals(VOID) = NULL;
	virtual bool IsPlayingDemo(VOID) = NULL;
	virtual bool IsRecordingDemo(VOID) = NULL;
	virtual bool IsPlayingTimeDemo(VOID) = NULL;
	virtual INT GetDemoRecordingTick(VOID) = NULL;
	virtual INT GetDemoPlaybackTick(VOID) = NULL;
	virtual INT GetDemoPlaybackStartTick(VOID) = NULL;
	virtual FLOAT GetDemoPlaybackTimeScale(VOID) = NULL;
	virtual INT GetDemoPlaybackTotalTicks(VOID) = NULL;
	virtual bool IsPaused(VOID) = NULL;
	virtual FLOAT GetTimescale(VOID) CONST = NULL;
	virtual bool IsTakingScreenshot(VOID) = NULL;
	virtual bool IsHLTV(VOID) = NULL;
	virtual bool IsLevelMainMenuBackground(VOID) = NULL;
	virtual VOID GetMainMenuBackgroundName(CHAR *, INT) = NULL;
	virtual VOID SetOcclusionParameters(CONST INT &) = NULL;
	virtual VOID GetUILanguage(CHAR *, INT) = NULL;
	virtual INT IsSkyboxVisibleFromPoint(CONST Vector &) = NULL;
	virtual CONST CHAR * GetMapEntitiesString(VOID) = NULL;
	virtual bool IsInEditMode(VOID) = NULL;
	virtual FLOAT GetScreenAspectRatio(INT, INT) = NULL;
	virtual bool REMOVED_SteamRefreshLogin(CONST CHAR *, bool) = NULL;
	virtual bool REMOVED_SteamProcessCall(bool &) = NULL;
	virtual UINT GetEngineBuildNumber(VOID) = NULL;
	virtual CONST CHAR * GetProductVersionString(VOID) = NULL;
	virtual VOID GrabPreColorCorrectedFrame(INT, INT, INT, INT) = NULL;
	virtual bool IsHammerRunning(VOID) CONST = NULL;
	virtual VOID ExecuteClientCmd(CONST CHAR *) = NULL;
	virtual bool MapHasHDRLighting(VOID) = NULL;
	virtual bool MapHasLightMapAlphaData(VOID) = NULL;
	virtual INT GetAppID(VOID) = NULL;
	virtual Vector GetLightForPointFast(CONST Vector &, bool) = NULL;
	virtual VOID ClientCmd_Unrestricted1(CHAR CONST *, INT, bool);
	virtual VOID ClientCmd_Unrestricted(CONST CHAR *, CONST CHAR *) = NULL;
	virtual VOID SetRestrictServerCommands(bool) = NULL;
	virtual VOID SetRestrictClientCommands(bool) = NULL;
	virtual VOID SetOverlayBindProxy(INT, VOID *) = NULL;
	virtual bool CopyFrameBufferToMaterial(CONST CHAR *) = NULL;
	virtual VOID ReadConfiguration(CONST INT, CONST bool) = NULL;
	virtual VOID SetAchievementMgr(VOID *) = NULL;
	virtual VOID * GetAchievementMgr(VOID) = NULL;
	virtual bool MapLoadFailed(VOID) = NULL;
	virtual VOID SetMapLoadFailed(bool) = NULL;
	virtual bool IsLowViolence(VOID) = NULL;
	virtual CONST CHAR * GetMostRecentSaveGame(VOID) = NULL;
	virtual VOID SetMostRecentSaveGame(CONST CHAR *) = NULL;
	virtual VOID StartXboxExitingProcess(VOID) = NULL;
	virtual bool IsSaveInProgress(VOID) = NULL;
	virtual bool IsAutoSaveDangerousInProgress(VOID) = NULL;
	virtual UINT OnStorageDeviceAttached(INT) = NULL;
	virtual VOID OnStorageDeviceDetached(INT) = NULL;
	virtual CHAR CONST * GetSaveDirName(VOID) = NULL;
	virtual VOID WriteScreenshot(CONST CHAR *) = NULL;
	virtual VOID ResetDemoInterpolation(VOID) = NULL;
	virtual INT GetActiveSplitScreenPlayerSlot(VOID) = NULL;
	virtual INT SetActiveSplitScreenPlayerSlot(INT) = NULL;
	virtual bool SetLocalPlayerIsResolvable(CHAR CONST *, INT, bool) = NULL;
	virtual bool IsLocalPlayerResolvable(VOID) = NULL;
	virtual INT GetSplitScreenPlayer(INT) = NULL;
	virtual bool IsSplitScreenActive(VOID) = NULL;
	virtual bool IsValidSplitScreenSlot(INT) = NULL;
	virtual INT FirstValidSplitScreenSlot(VOID) = NULL;
	virtual INT NextValidSplitScreenSlot(INT) = NULL;
	virtual VOID * GetSinglePlayerSharedMemorySpace(CONST CHAR *, INT) = NULL;
	virtual VOID ComputeLightingCube(CONST Vector &, bool, Vector *) = NULL;
	virtual VOID RegisterDemoCustomDataCallback(CONST CHAR *, pfnDemoCustomDataCallback) = NULL;
	virtual VOID RecordDemoCustomData(pfnDemoCustomDataCallback, CONST VOID *, size_t) = NULL;
	virtual VOID SetPitchScale(FLOAT) = NULL;
	virtual FLOAT GetPitchScale(VOID) = NULL;
	virtual bool LoadFilmmaker(VOID) = NULL;
	virtual VOID UnloadFilmmaker(VOID) = NULL;
	virtual VOID SetLeafFlag(INT, INT) = NULL;
	virtual VOID RecalculateBSPLeafFlags(VOID) = NULL;
	virtual bool DSPGetCurrentDASRoomNew(VOID) = NULL;
	virtual bool DSPGetCurrentDASRoomChanged(VOID) = NULL;
	virtual bool DSPGetCurrentDASRoomSkyAbove(VOID) = NULL;
	virtual FLOAT DSPGetCurrentDASRoomSkyPercent(VOID) = NULL;
	virtual VOID SetMixGroupOfCurrentMixer(CONST CHAR *, CONST CHAR *, FLOAT, INT) = NULL;
	virtual INT GetMixLayerIndex(CONST CHAR *) = NULL;
	virtual VOID SetMixLayerLevel(INT, FLOAT) = NULL;
	virtual INT GetMixGroupIndex(CHAR CONST *) = NULL;
	virtual VOID SetMixLayerTriggerFactor(INT, INT, FLOAT) = NULL;
	virtual VOID SetMixLayerTriggerFactor(CHAR CONST *, CHAR CONST *, FLOAT) = NULL;
	virtual bool IsCreatingReslist(VOID) = NULL;
	virtual bool IsCreatingXboxReslist(VOID) = NULL;
	virtual VOID SetTimescale(FLOAT) = NULL;
	virtual VOID SetGamestatsData(VOID *) = NULL;
	virtual VOID * GetGamestatsData(VOID) = NULL;
	virtual VOID GetMouseDelta(INT &, INT &, bool) = NULL;
	virtual CONST CHAR * Key_LookupBindingEx(CONST CHAR *, INT, INT, INT) = NULL;
	virtual INT Key_CodeForBinding(CHAR CONST *, INT, INT, INT) = NULL;
	virtual VOID UpdateDAndELights(VOID) = NULL;
	virtual INT GetBugSubmissionCount(VOID) CONST = NULL;
	virtual VOID ClearBugSubmissionCount(VOID) = NULL;
	virtual bool DoesLevelContainWater(VOID) CONST = NULL;
	virtual FLOAT GetServerSimulationFrameTime(VOID) CONST = NULL;
	virtual VOID SolidMoved(VOID *, VOID *, CONST Vector *, bool) = NULL;
	virtual VOID TriggerMoved(VOID *, bool) = NULL;
	virtual VOID ComputeLeavesConnected(CONST Vector &, INT, CONST INT *, bool *) = NULL;
	virtual bool IsInCommentaryMode(VOID) = NULL;
	virtual VOID SetBlurFade(FLOAT) = NULL;
	virtual bool IsTransitioningToLoad(VOID) = NULL;
	virtual VOID SearchPathsChangedAfterInstall(VOID) = NULL;
	virtual VOID ConfigureSystemLevel(INT, INT) = NULL;
	virtual VOID SetConnectionPassword(CHAR CONST *) = NULL;
	virtual PVOID GetSteamAPIContext(VOID) = NULL;
	virtual VOID SubmitStatRecord(CHAR CONST *, UINT, UINT, CONST VOID *) = NULL;
	virtual VOID ServerCmdKeyValues(VOID *) = NULL;
	virtual VOID SpherePaintSurface(CONST Model_T *, CONST Vector &, UCHAR *, FLOAT, FLOAT) = NULL;
	virtual bool HasPaintmap(VOID) = NULL;
	virtual VOID EnablePaintmapRender(VOID) = NULL;
	virtual VOID SphereTracePaintSurface(CONST Model_T *, CONST Vector &, CONST Vector &, FLOAT, INT &) = NULL;
	virtual VOID RemoveAllPaint(VOID) = NULL;
	virtual VOID PaintAllSurfaces(UCHAR *) = NULL;
	virtual VOID RemovePaint(CONST Model_T *) = NULL;
	virtual bool IsActiveApp(VOID) = NULL;
	virtual bool IsClientLocalToActiveServer(VOID) = NULL;
	virtual VOID TickProgressBar(VOID) = NULL;
	virtual PVOID GetInputContext(INT) = NULL;
	virtual VOID GetStartupImage(CHAR *, INT) = NULL;
	virtual bool IsUsingLocalNetworkBackdoor(VOID) = NULL;
	virtual VOID SaveGame(CONST CHAR *, bool, CHAR *, INT, CHAR *, INT) = NULL;
	virtual VOID GetGenericMemoryStats(VOID **) = NULL;
	virtual bool GameHasShutdownAndFlushedMemory(VOID) = NULL;
	virtual INT GetLastAcknowledgedCommand(VOID) = NULL;
	virtual VOID FinishContainerWrites(INT) = NULL;
	virtual VOID FinishAsyncSave(VOID) = NULL;
	virtual INT GetServerTick(VOID) = NULL;
	virtual CONST CHAR * GetModDirectory(VOID) = NULL;
	virtual bool AudioLanguageChanged(VOID) = NULL;
	virtual bool IsAutoSaveInProgress(VOID) = NULL;
	virtual VOID StartLoadingScreenForCommand(CONST CHAR *) = NULL;
	virtual VOID StartLoadingScreenForKeyValues(VOID *) = NULL;
	virtual VOID SOSSetOpvarFloat(CONST CHAR *, FLOAT) = NULL;
	virtual VOID SOSGetOpvarFloat(CONST CHAR *, FLOAT &) = NULL;
	virtual bool IsSubscribedMap(CONST CHAR *, bool) = NULL;
	virtual bool IsFeaturedMap(CONST CHAR *, bool) = NULL;
	virtual VOID GetDemoPlaybackParameters(VOID) = NULL;
	virtual INT GetClientVersion(VOID) = NULL;
	virtual bool IsDemoSkipping(VOID) = NULL;
	virtual VOID SetDemoImportantEventData(CONST VOID *) = NULL;
	virtual VOID ClearEvents(VOID) = NULL;
	virtual INT GetSafeZoneXMin(VOID) = NULL;
	virtual bool IsVoiceRecording(VOID) = NULL;
	virtual VOID ForceVoiceRecordOn(VOID) = NULL;
	virtual bool IsReplay(VOID) = NULL;
};

class IEngineTrace
{
public:

	virtual INT GetPointContents(CONST Vector &, INT, VOID **) = NULL;
	virtual INT GetPointContents_WorldOnly(CONST Vector &, INT) = NULL;
	virtual INT GetPointContents_Collideable(VOID *, CONST Vector &) = NULL;
	virtual VOID ClipRayToEntity(CONST Ray_T &, UINT, VOID *, GameTrace_T *) = NULL;
	virtual VOID ClipRayToCollideable(CONST Ray_T &, UINT, VOID *, GameTrace_T *) = NULL;
	virtual VOID TraceRay(CONST Ray_T &, UINT, ITraceFilter *, GameTrace_T *) = NULL;
	virtual VOID SetupLeafAndEntityListRay(CONST Ray_T &, VOID *) = NULL;
	virtual VOID SetupLeafAndEntityListBox(CONST Vector &, CONST Vector &, VOID *) = NULL;
	virtual VOID TraceRayAgainstLeafAndEntityList(CONST Ray_T &, VOID *, UINT, ITraceFilter *, GameTrace_T *) = NULL;
	virtual VOID SweepCollideable(VOID *, CONST Vector &, CONST Vector &, CONST QAngle &, UINT, ITraceFilter *, GameTrace_T *) = NULL;
	virtual VOID EnumerateEntities(CONST Ray_T &, bool, VOID *) = NULL;
	virtual VOID EnumerateEntities(CONST Vector &, CONST Vector &, VOID *) = NULL;
	virtual VOID * GetCollideable(VOID *) = NULL;
	virtual INT GetStatByIndex(INT, bool) = NULL;
	virtual VOID lolignorethisaswellrifk(VOID) = NULL;
	virtual VOID GetCollidableFromDisplacementsInAABB(VOID) = NULL;
	virtual INT GetNumDisplacements(VOID) = NULL;
	virtual VOID GetDisplacementMesh(INT, PVOID) = NULL;
	virtual bool lolignorethis(VOID) = NULL;
	virtual bool PointOutsideWorld(CONST Vector &) = NULL;
	virtual INT GetLeafContainingPoint(CONST Vector &) = NULL;
	virtual VOID *AllocTraceListData(VOID) = NULL;
	virtual VOID FreeTraceListData(VOID *) = NULL;
	virtual INT GetSetDebugTraceCounter(INT, INT) = NULL;
};

class CGrenTrajObj
{
public:

	FORCEINLINE CGrenTrajObj(VOID)
	{
		m_vecPositions.clear();
		m_Grenade = -1;
		m_lastThinkTime = 0;
		m_pClientClass = NULL;
	};

	FORCEINLINE ~CGrenTrajObj(VOID)
	{
		m_vecPositions.clear();
		m_Grenade = -1;
		m_lastThinkTime = 0;
		m_pClientClass = NULL;
	};

	vector < Vector > m_vecPositions;
	clock_t m_lastThinkTime;
	INT m_Grenade;
	ClientClass * m_pClientClass;
};

class CGrenTraj
{
public:

	CGrenTraj(VOID);
	~CGrenTraj(VOID);
	VOID AddGrenade(INT);
	bool IsEntityGrenade(Entity_T *, ClientClass *);
	VOID Work(VOID);
	bool IsGrenadeQueued(INT);
	VOID Clear(VOID);

	vector < CGrenTrajObj > m_vecGrenades;
};

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

class WeaponInfo_T
{
public:

	CHAR pad_0x0000[4];
	PCHAR consoleName;
	CHAR pad_0008[12];
	INT iMaxClip1;
	INT iMaxClip2;
	INT iDefaultClip1;
	INT iDefaultClip2;
	CHAR pad_0024[8];
	PCHAR szWorldModel;
	PCHAR szViewModel;
	PCHAR szDroppedModel;
	CHAR pad_0038[4];
	PCHAR N0000023E;
	CHAR pad_0040[56];
	PCHAR szEmptySound;
	CHAR pad_007C[4];
	PCHAR szBulletType;
	CHAR pad_0084[4];
	PCHAR szHudName;
	PCHAR szWeaponName;
	CHAR pad_0090[56];
	INT WeaponType;
	INT iWeaponPrice;
	INT iKillAward;
	PCHAR szAnimationPrefix;
	FLOAT flCycleTime;
	FLOAT flCycleTimeAlt;
	FLOAT flTimeToIdle;
	FLOAT flIdleInterval;
	bool bFullAuto;
	CHAR pad_0x00E5[3];
	INT iDamage;
	FLOAT flArmorRatio;
	INT iBullets;
	FLOAT flPenetration;
	FLOAT flFlinchVelocityModifierLarge;
	FLOAT flFlinchVelocityModifierSmall;
	FLOAT flRange;
	FLOAT flRangeModifier;
	FLOAT flThrowVelocity;
	CHAR pad_0x010C[12];
	bool bHasSilencer;
	CHAR pad_0x0119[3];
	PCHAR pSilencerModel;
	INT iCrosshairMinDistance;
	INT iCrosshairDeltaDistance;
	FLOAT flMaxPlayerSpeed;
	FLOAT flMaxPlayerSpeedAlt;
	FLOAT flSpread;
	FLOAT flSpreadAlt;
	FLOAT flInaccuracyCrouch;
	FLOAT flInaccuracyCrouchAlt;
	FLOAT flInaccuracyStand;
	FLOAT flInaccuracyStandAlt;
	FLOAT flInaccuracyJumpInitial;
	FLOAT flInaccuracyJump;
	FLOAT flInaccuracyJumpAlt;
	FLOAT flInaccuracyLand;
	FLOAT flInaccuracyLandAlt;
	FLOAT flInaccuracyLadder;
	FLOAT flInaccuracyLadderAlt;
	FLOAT flInaccuracyFire;
	FLOAT flInaccuracyFireAlt;
	FLOAT flInaccuracyMove;
	FLOAT flInaccuracyMoveAlt;
	FLOAT flInaccuracyReload;
	INT iRecoilSeed;
	FLOAT flRecoilAngle;
	FLOAT flRecoilAngleAlt;
	FLOAT flRecoilAngleVariance;
	FLOAT flRecoilAngleVarianceAlt;
	FLOAT flRecoilMagnitude;
	FLOAT flRecoilMagnitudeAlt;
	FLOAT flRecoilMagnitudeVariance;
	FLOAT flRecoilMagnitudeVarianceAlt;
	FLOAT flRecoveryTimeCrouch;
	FLOAT flRecoveryTimeStand;
	FLOAT flRecoveryTimeCrouchFinal;
	FLOAT flRecoveryTimeStandFinal;
	INT iRecoveryTransitionStartBullet;
	INT iRecoveryTransitionEndBullet;
	bool bUnzoomAfterShot;
	bool bHideViewModelZoomed;
	CHAR pad_0x01B5[2];
	CHAR iZoomLevels[3];
	INT iZoomFOV[2];
	FLOAT fZoomTime[3];
	PCHAR szWeaponClass;
	FLOAT flAddonScale;
	CHAR pad_0x01DC[4];
	PCHAR szEjectBrassEffect;
	PCHAR szTracerEffect;
	INT iTracerFrequency;
	INT iTracerFrequencyAlt;
	PCHAR szMuzzleFlashEffect_1stPerson;
	CHAR pad_0x01F4[4];
	PCHAR szMuzzleFlashEffect_3rdPerson;
	CHAR pad_0x01FC[4];
	PCHAR szMuzzleSmokeEffect;
	FLOAT flHeatPerShot;
	PCHAR szZoomInSound;
	PCHAR szZoomOutSound;
	FLOAT flInaccuracyPitchShift;
	FLOAT flInaccuracySoundThreshold;
	FLOAT flBotAudibleRange;
	BYTE pad_0x0218[8];
	PCHAR pWrongTeamMsg;
	bool bHasBurstMode;
	BYTE pad_0x0225[3];
	bool bIsRevolver;
	bool bCannotShootUnderwater;
};

#endif

#endif

struct StdioBBox_T
{
	INT m_iBone;
	INT m_iGroup;
	Vector m_vecMin;
	Vector m_vecMax;
	INT iHBoxNameID;
	INT m_pUnusedA[0x00000003];
	FLOAT m_fRadius;
	INT m_pUnusedB[0x00000004];
};

struct Model_T
{
	CHAR szName[255];
};

struct StdHBoxSet_T
{
	INT iNameID;
	INT iHBoxes;
	INT iHBoxID;

	StdioBBox_T * pHBox(INT iHBoxNum) CONST
	{
		return (StdioBBox_T *)(((PBYTE)this) + iHBoxID) + iHBoxNum;
	};
};

struct RecvTable_T
{
	RecvProp_T * m_pProps;
	INT m_nProps;
	PVOID m_pDecoder;
	PCHAR m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct PlrInfo_T
{
	INT64 dummy;

	union
	{
		INT64 xuid;

		struct
		{
			INT xuidlow, xuidhigh;
		};
	};

	CHAR name[128];
	INT userid;
	CHAR guid[33];
	UINT friendsid;
	CHAR friendsname[128];
	bool fakeplayer;
	bool ishltv;
	UINT customfiles[4];
	UCHAR filesdownloaded;
};

struct RecvProp_T
{
	PCHAR m_pVarName;
	INT m_RecvType;
	INT m_Flags;
	INT m_StringBufferSize;
	bool m_bInsideArray;
	CONST PVOID m_pExtraData;
	RecvProp_T * m_pArrayProp;
	PVOID m_ArrayLengthProxy;
	PVOID m_ProxyFn;
	PVOID m_DataTableProxyFn;
	RecvTable_T * m_pDataTable;
	INT m_Offset;
	INT m_ElementStride;
	INT m_nElements;
	CONST CHAR * m_pParentArrayPropName;
};

struct StdBone_T
{
	INT iNameID;
	INT iParent;
	INT pBoneController[6];
	Vector vecPos;
	Quaternion qUnusedA;
	Vector vecRot;
	Vector vecPosScale;
	Vector vecRotscale;
	Matrix34_T vecPoseToBone;
	Quaternion qUnusedB;
	INT iFlags;
	INT iProcType;
	INT iProcID;
	mutable INT iPhysBone;
	INT pUnused[10];
};

struct StdHdr_T
{
	INT iID;
	INT iVersion;
	LONG lChecksum;
	CHAR szName[64];
	INT iLen;
	Vector vecEyePos;
	Vector vecIllumPos;
	Vector vecHullMin;
	Vector vecHullMax;
	Vector vecViewMin;
	Vector vecViewMax;
	INT iFlags;
	INT iNumBones;
	INT iBoneID;

	StdBone_T * pBone(INT iId) CONST
	{
		return (StdBone_T *)(((PBYTE)this) + iBoneID) + iId;
	};

	INT iNumBoneControllers;
	INT iBoneControllerID;
	INT iNumHBoxSets;
	INT iHBoxSetID;

	StdHBoxSet_T * pHBoxSet(INT iId) CONST
	{
		return (StdHBoxSet_T *)(((PBYTE)this) + iHBoxSetID) + iId;
	};

	INT iNumLocalAnim;
	INT iLocalAnimID;
	INT iNumLocalSeq;
	INT iLocalSeqID;
	mutable INT iActivityListVersion;
	mutable INT iIndexedEvents;
	INT iNumTextures;
	INT iTextureID;
	INT iNumCDTextures;
	INT iCDTextureID;
	INT iNumSkinRef;
	INT iNumSkinFamilies;
	INT iSkinID;
	INT iNumBodyParts;
	INT iBodyPartID;
};

struct Ray_T
{
	Vector m_Start;
	FLOAT fUnusedA;
	Vector m_Delta;
	FLOAT fUnusedB;
	Vector m_StartOffset;
	FLOAT fUnusedC;
	Vector m_Extents;
	FLOAT fUnusedD;
	CONST Matrix34_T * m_pWorldAxisTransform;
	bool m_bIsRay;
	bool m_bIsSwept;

	VOID Run(Vector S, Vector D)
	{
		m_Delta = D - S, m_bIsSwept = m_Delta.LengthSqr() != 0.f, m_Extents.Zero(), m_pWorldAxisTransform = NULL, m_bIsRay = true, m_StartOffset.Zero(), m_Start = S;
	};
};

#define IS_SNIPER(X) (X == WEAPON_SCAR20 || X == WEAPON_G3SG1 || X == WEAPON_AWP || X == WEAPON_SSG08)
#define IS_PISTOL(X) (X == WEAPON_USP_SILENCER || X == WEAPON_HKP2000 || X == WEAPON_CZ75A || X == WEAPON_TEC9 || X == WEAPON_DEAGLE || X == WEAPON_GLOCK || X == WEAPON_P250 || X == WEAPON_ELITE || X == WEAPON_FIVESEVEN || X == WEAPON_REVOLVER)
#define IS_GRENADE(X) (X == WEAPON_DECOY || X == WEAPON_FLASHBANG || X == WEAPON_INCGRENADE || X == WEAPON_MOLOTOV || X == WEAPON_HEGRENADE || X == WEAPON_SMOKEGRENADE)
#define IS_KNIFE(X) (X == WEAPON_KNIFE_M9_BAYONET || X == WEAPON_KNIFE_FALCHION || X == WEAPON_KNIFE_SURVIVAL_BOWIE || X == WEAPON_KNIFE_BAYONET || X == WEAPON_KNIFE || X == WEAPON_KNIFE_T || X == WEAPON_KNIFE_KARAMBIT || X == WEAPON_KNIFE_BUTTERFLY || X == WEAPON_KNIFE_PUSH || X == WEAPON_KNIFE_TACTICAL || X == WEAPON_KNIFE_FLIP || X == WEAPON_KNIFE_GUT)

#define HITGROUP_HEAD 1
#define HITGROUP_CHEST 2
#define HITGROUP_STOMACH 3

#define M_PI_F ((FLOAT)(3.14159265359))

#define RAD2DEG(X) ((FLOAT)(X) * (FLOAT)(57.2957795131))
#define DEG2RAD(X) ((FLOAT)(X) * (FLOAT)(0.01745329251))

#define IN_ATTACK (1 << 0)
#define IN_JUMP (1 << 1)
#define IN_DUCK (1 << 2)
#define IN_FORWARD (1 << 3)
#define IN_BACK (1 << 4)
#define IN_MOVELEFT (1 << 9)
#define IN_MOVERIGHT (1 << 10)
#define IN_ATTACK2 (1 << 11)
#define IN_SCORE (1 << 16)
#define IN_WALK (1 << 18)

#define FL_ONGROUND (1 << 0) 
#define FL_DUCKING (1 << 1) 
#define FL_FROZEN (1 << 6) 
#define FL_CLIENT (1 << 8) 

#define CONTENTS_SOLID 0x00000001
#define CONTENTS_WINDOW 0x00000002
#define CONTENTS_GRATE 0x00000008
#define CONTENTS_MOVEABLE 0x00004000
#define CONTENTS_MONSTER 0x2000000
#define CONTENTS_DEBRIS 0x4000000
#define CONTENTS_HITBOX 0x40000000

#define MASK_SHOT_HULL (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE)
#define MASK_SHOT (CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX)

enum HBox : INT
{
	HITBOX_HEAD = 0,
	HITBOX_NECK,
	HITBOX_LOWER_NECK,
	HITBOX_PELVIS,
	HITBOX_BODY,
	HITBOX_THORAX,
	HITBOX_CHEST,
	HITBOX_UPPER_CHEST,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_CALF,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_HAND,
	HITBOX_LEFT_HAND,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_RIGHT_FOREARM,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_LEFT_FOREARM,
};

enum EClass : INT
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CItemDogtags = 92,
	CKnife = 93,
	CKnifeGG = 94,
	CLightGlow = 95,
	CMaterialModifyControl = 96,
	CMolotovGrenade = 97,
	CMolotovProjectile = 98,
	CMovieDisplay = 99,
	CParticleFire = 100,
	CParticlePerformanceMonitor = 101,
	CParticleSystem = 102,
	CPhysBox = 103,
	CPhysBoxMultiplayer = 104,
	CPhysicsProp = 105,
	CPhysicsPropMultiplayer = 106,
	CPhysMagnet = 107,
	CPlantedC4 = 108,
	CPlasma = 109,
	CPlayerResource = 110,
	CPointCamera = 111,
	CPointCommentaryNode = 112,
	CPointWorldText = 113,
	CPoseController = 114,
	CPostProcessController = 115,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CPredictedViewModel = 118,
	CProp_Hallucination = 119,
	CPropDoorRotating = 120,
	CPropJeep = 121,
	CPropVehicleDriveable = 122,
	CRagdollManager = 123,
	CRagdollProp = 124,
	CRagdollPropAttached = 125,
	CRopeKeyframe = 126,
	CSCAR17 = 127,
	CSceneEntity = 128,
	CSensorGrenade = 129,
	CSensorGrenadeProjectile = 130,
	CShadowControl = 131,
	CSlideshowDisplay = 132,
	CSmokeGrenade = 133,
	CSmokeGrenadeProjectile = 134,
	CSmokeStack = 135,
	CSpatialEntity = 136,
	CSpotlightEnd = 137,
	CSprite = 138,
	CSpriteOriented = 139,
	CSpriteTrail = 140,
	CStatueProp = 141,
	CSteamJet = 142,
	CSun = 143,
	CSunlightShadowControl = 144,
	CTeam = 145,
	CTeamplayRoundBasedRulesProxy = 146,
	CTEArmorRicochet = 147,
	CTEBaseBeam = 148,
	CTEBeamEntPoint = 149,
	CTEBeamEnts = 150,
	CTEBeamFollow = 151,
	CTEBeamLaser = 152,
	CTEBeamPoints = 153,
	CTEBeamRing = 154,
	CTEBeamRingPoint = 155,
	CTEBeamSpline = 156,
	CTEBloodSprite = 157,
	CTEBloodStream = 158,
	CTEBreakModel = 159,
	CTEBSPDecal = 160,
	CTEBubbles = 161,
	CTEBubbleTrail = 162,
	CTEClientProjectile = 163,
	CTEDecal = 164,
	CTEDust = 165,
	CTEDynamicLight = 166,
	CTEEffectDispatch = 167,
	CTEEnergySplash = 168,
	CTEExplosion = 169,
	CTEFireBullets = 170,
	CTEFizz = 171,
	CTEFootprintDecal = 172,
	CTEFoundryHelpers = 173,
	CTEGaussExplosion = 174,
	CTEGlowSprite = 175,
	CTEImpact = 176,
	CTEKillPlayerAttachments = 177,
	CTELargeFunnel = 178,
	CTEMetalSparks = 179,
	CTEMuzzleFlash = 180,
	CTEParticleSystem = 181,
	CTEPhysicsProp = 182,
	CTEPlantBomb = 183,
	CTEPlayerAnimEvent = 184,
	CTEPlayerDecal = 185,
	CTEProjectedDecal = 186,
	CTERadioIcon = 187,
	CTEShatterSurface = 188,
	CTEShowLine = 189,
	CTesla = 190,
	CTESmoke = 191,
	CTESparks = 192,
	CTESprite = 193,
	CTESpriteSpray = 194,
	CTest_ProxyToggle_Networkable = 194,
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTriggerPlayerMovement = 198,
	CTriggerSoundOperator = 199,
	CVGuiScreen = 200,
	CVoteController = 201,
	CWaterBullet = 202,
	CWaterLODControl = 203,
	CWeaponAug = 204,
	CWeaponAWP = 205,
	CWeaponBaseItem = 206,
	CWeaponBizon = 207,
	CWeaponCSBase = 208,
	CWeaponCSBaseGun = 209,
	CWeaponCycler = 210,
	CWeaponElite = 211,
	CWeaponFamas = 212,
	CWeaponFiveSeven = 213,
	CWeaponG3SG1 = 214,
	CWeaponGalil = 215,
	CWeaponGalilAR = 216,
	CWeaponGlock = 217,
	CWeaponHKP2000 = 218,
	CWeaponM249 = 219,
	CWeaponM3 = 220,
	CWeaponM4A1 = 221,
	CWeaponMAC10 = 222,
	CWeaponMag7 = 223,
	CWeaponMP5Navy = 224,
	CWeaponMP7 = 225,
	CWeaponMP9 = 226,
	CWeaponNegev = 227,
	CWeaponNOVA = 228,
	CWeaponP228 = 229,
	CWeaponP250 = 230,
	CWeaponP90 = 231,
	CWeaponSawedoff = 232,
	CWeaponSCAR20 = 233,
	CWeaponScout = 234,
	CWeaponSG550 = 235,
	CWeaponSG552 = 236,
	CWeaponSG556 = 237,
	CWeaponSSG08 = 238,
	CWeaponTaser = 239,
	CWeaponTec9 = 240,
	CWeaponTMP = 241,
	CWeaponUMP45 = 242,
	CWeaponUSP = 243,
	CWeaponXM1014 = 244,
	CWorld = 245,
	DustTrail = 246,
	MovieExplosion = 247,
	ParticleSmokeGrenade = 248,
	RocketTrail = 249,
	SmokeTrail = 250,
	SporeExplosion = 251,
	SporeTrail = 252,
};

enum ItemDefinitionIndex : INT
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516,
};

typedef bool(__thiscall * FireEventClientSide) (PVOID, IGameEvent *);
typedef bool (WINAPI * CreateMoveFn) (FLOAT, CUserCmd *);

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

typedef VOID(*IsReadyCallBackFn) (VOID);
typedef VOID(WINAPI * PlaySoundFn) (CONST CHAR *);

#endif

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

typedef VOID(__thiscall * TraceRayFn) (PVOID, CONST Ray_T &, UINT, TraceFilter_T *, GameTrace_T *);

#endif

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

typedef VOID(WINAPI * OverrideViewFn) (CViewSetup *);
typedef FLOAT(WINAPI * GetViewModelFoVFn) (VOID);

#endif

typedef VOID(WINAPI * PaintTraverseFn) (UINT, bool, bool);
typedef LONG(WINAPI * EndSceneFn) (IDirect3DDevice9 *);
typedef LONG(WINAPI * ResetFn) (IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
typedef bool(WINAPIV * ServerRankRevealAllFn) (PFLOAT);
typedef ClientClass * (__thiscall * GetClientClassFn) (PVOID);
typedef bool(*LineGoesThroughSmokeTypeFn) (FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, FLOAT, SHORT);

namespace G
{
	extern bool bAlive;
	extern Entity_T * pSelf;
	extern INT iWeapId;
	extern INT iFlags;
	extern INT iTeam;
	extern CUserCmd * pCmd;
	extern QAngle angSelfPunch;
	extern Vector vecSelfVel;
	extern bool bScoped;
	extern FLOAT fSelfVelRealLen;
	extern bool Keys[XS_RANDOM_SIZE * 2];
	extern Vector vecSelfEyePos;
	extern FLOAT fSelfVelLen;
	extern bool bGun;
	extern bool bEmpty;
	extern Entity_T * pWeap;
};

namespace H
{
	extern VTHook * VPanel;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

	extern VTHook * Surf;

#endif

	extern VTHook * CLMode;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	extern VTHook * Traces;

#endif

#endif

	extern VTHook * CL;
	extern VTHook * D3D;
};

namespace I
{
	extern IBaseClientDll * Client;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

	extern IPhysProps * PhysProps;

#endif

#endif

	extern IClientModeShared * CLMode;
	extern IEnts * Ents;
	extern IEngineClient * Engine;
	extern IEngineTrace * EngTrace;
	extern IGlobalVarsBase * Globals;
	extern IInputSystem * InputSys;
	extern IGameEventManager * EvntsMgr;
	extern ISurface * Surf;
	extern IVPanel * VPanel;
	extern IVModelInfo * MDLInfo;
};

extern bool g_bD3DOK;
extern HWND g_pWnd;
extern CGrenTraj g_nadeTraj;
extern bool g_bWasScoped;
extern bool g_bBombPlanted;
extern CGrenTraj * g_pNadeTraj;
extern UINT g_uiDrawPanel;
extern CONST CHAR * g_pszPanelName;
extern PaintTraverseFn oPaintTraverse;
extern bool g_bFrozen;
extern Variables Vars;
extern bool m_bVis[XS_RANDOM_SIZE / 2];
extern ServerRankRevealAllFn ServerRankRevealAllEx;
extern INT g_iCTScore;
extern bool g_bSmokeExcluded[XS_RANDOM_SIZE / 2];
extern INT g_iTEScore;
extern string g_CFGFile;
extern COffsets g_Offsets;
extern Color g_Red;
extern Color g_DarkRed;
extern Color g_CandyRed;
extern Color g_DarkBlue;
extern Color g_Blue;
extern Color g_CandyBlue;
extern Color g_DarkGreen;
extern Color g_Green;
extern Color g_CandyGreen;
extern Color g_White;
extern Color g_WhiteSmoke;
extern Color g_Grey;
extern Color g_DarkGrey;
extern Color g_Black;
extern Color g_Magenta;
extern Color g_Violet;
extern Color g_Yellow;
extern Color g_Orange;
extern Color g_Cyan;
extern clock_t g_switchTime;
extern INT g_iLastItem;
extern clock_t g_zoomTime;

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

extern CHAR g_szFormattedClanTag[XS_RANDOM_SIZE];

#endif

extern time_t g_timeLastEventStamp;
extern bool g_bLoaded;

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT == 1

extern CRBOT * g_pRageBOT;

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

extern TraceRayFn oTraceRay;
extern bool g_bShouldMask;

#endif

#endif

extern CNetVarMan * g_pNetVarsMan;
extern CConfig * Config;
extern WNDPROC g_pOldWndProc;
extern bool g_bOffline;
extern time_t g_bombDropTime;
extern json g_Json;
extern CreateMoveFn oCreateMove;
extern time_t g_roundStartTime;
extern clock_t g_saveTime;

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

extern PlaySoundFn oPlaySound;

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

extern OverrideViewFn oOverrideView;
extern GetViewModelFoVFn oGetViewModelFoV;

#endif

extern clock_t g_loadTime;
extern ResetFn oReset;
extern EndSceneFn oEndScene;
extern INT g_nStyle;

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

extern CHAR g_Phrases[SIZE_T(XS_RANDOM_SIZE * .333333f)][SIZE_T(XS_RANDOM_SIZE * .75f)];
extern INT g_PhrasesSize;

#endif

extern FireEventClientSide fnOriginalFireEventClientSide;
extern BOOL g_bCheckForLastEnemy;
extern CLBOT * g_pLegitBOT;
extern CVisuals * g_pVisuals;
extern SIZE_T g_uTraces;
extern FLOAT g_fMaxPunch;
extern bool g_bRoundEnded;
extern time_t g_lastDoPlantMsg;
extern QAngle g_angSelfView;
extern string g_pFoVCircles[3];
extern string g_pCustomColorOptions[19];
extern string g_pCustomGlowColorOptions[ARRAYSIZE(g_pCustomColorOptions) + 1];
extern string g_pTriggerKeys[91];
extern INT g_nHitBoxes[20 - 4];

#if defined(ENABLE_FAKE_LAG) && ENABLE_FAKE_LAG != 0

extern string g_pFakeLagOptions[2];

#endif

LRESULT WINAPI OnWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);
VOID ImGui_ImplDX9_NewFrame(VOID);
VOID ImGui_ImplDX9_InvalidateDeviceObjects(VOID);
bool ImGui_ImplDX9_CreateDeviceObjects(VOID);
bool ImGui_ImplDX9_Init(PVOID, IDirect3DDevice9 *);

#if defined(ENABLE_CLAN_TAG) && ENABLE_CLAN_TAG != 0

VOID SetClanTag(VOID);
INT FilterClanTag(ImGuiTextEditCallbackData *);

#endif

#if defined(ENABLE_CUSTOM_FOV) && ENABLE_CUSTOM_FOV != 0

VOID WINAPI OnOverrideView(CViewSetup *);
FLOAT WINAPI OnGetViewModelFoV(VOID);

#endif

bool TopZone(bool, Entity_T *, Vector &, CONST FLOAT, PFLOAT);
VOID CreateGUI(IDirect3DDevice9 *, string);

#if defined(ENABLE_AUTO_ACCEPT) && ENABLE_AUTO_ACCEPT != 0

VOID WINAPI OnPlaySound(CONST CHAR *);

#endif

VOID GrabOffsets(VOID);
bool WINAPI CreateMove(FLOAT, CUserCmd *);
VOID WINAPI PaintTraverse(UINT, bool, bool);
LONG WINAPI EndScene(IDirect3DDevice9 *);
LONG WINAPI Reset(IDirect3DDevice9 *, D3DPRESENT_PARAMETERS *);
bool __fastcall FireEventClientSideThink(PVOID, PVOID, IGameEvent *);
FLOAT GetDisp(VOID);
Color ClrByOption(INT);
FLOAT GetFactor(VOID);
FLOAT GetDetonationTime(VOID);

#if defined(ENABLE_RAGE_BOT) && ENABLE_RAGE_BOT != 0

#if defined(ENABLE_HEAVY_RAGE) && ENABLE_HEAVY_RAGE != 0

bool TopZoneHeavyRage(Entity_T *, Vector &, CONST FLOAT, PFLOAT, WeaponInfo_T *);
FLOAT CanAutoWall(Vector, Vector, WeaponInfo_T *);
VOID __fastcall OnTraceRay(PVOID, PVOID, CONST Ray_T &, UINT, TraceFilter_T *, GameTrace_T *);

#endif

#endif

Entity_T * GetSelf(VOID);
VOID TraceLine(CONST Vector, CONST Vector, CONST UINT, Entity_T *, GameTrace_T *);
VOID ServerRankRevealAll(VOID);
VOID SetupInterfaces(VOID);
VOID SetupHooks(VOID);
VOID SetupOffsets(VOID);
VOID sendConCmd(string);
INT RandomINT(INT, INT);
VOID replaceAll(string &, string, string);
BOOL xIsCharUpper(CHAR);
BOOL xIsCharLower(CHAR);
SIZE_T countChars(string, CHAR);
string ShortenName(string);
string ShortenNameNoRandom(string);
VOID MakeFonts(VOID);
VOID DrawStr(INT, INT, Color, bool, CONST string Data);
VOID DrawLine(INT, INT, INT, INT, Color);
VOID DrawCircle(FLOAT, FLOAT, FLOAT, FLOAT, Color);
bool WTS(CONST Vector, Vector &);
VOID SinCos(FLOAT, PFLOAT, PFLOAT);
VOID VecAng(CONST Vector, QAngle &);
VOID AngVec(CONST QAngle, Vector *);
QAngle CalcAng(CONST Vector, CONST Vector);
FLOAT FoV(CONST QAngle, CONST QAngle);
VOID VecTransf(CONST Vector, CONST Matrix34_T, Vector &);
bool stristr(CONST CHAR *, CONST CHAR *);
DWORD FindPattern(CONST string, CONST string);
template < typename Type > Type * CaptureInterface(CONST string, CONST string);
bool CanAim(VOID);

#if defined(ENABLE_CHAT_SPAM) && ENABLE_CHAT_SPAM != 0

VOID PrepareSpamString(PCHAR, PlrInfo_T *);

#endif
