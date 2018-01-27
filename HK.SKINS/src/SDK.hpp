
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

#include "XorString.h"

#include <memory>
#include <map>
#include <algorithm>
#include <array>
#include <unordered_map>
#include <fstream>
#include <codecvt>
#include <functional>

#include "DataTable.hpp"
#include "CBaseClientState.hpp"
#include "ClientClass.hpp"
#include "IGameEvent.hpp"
#include "IClientEntity.hpp"
#include "CBaseEntity.hpp"

#include "IBaseClientDLL.hpp"
#include "IClientEntityList.hpp"
#include "IGameEventManager2.hpp"
#include "IVEngineClient.hpp"
#include "IVModelInfoClient.hpp"
#include "ILocalize.hpp"

#include "VMTHook.hpp"
#include "Platform.hpp"
#include "Virtuals.hpp"
#include "NetVarManager.hpp"
