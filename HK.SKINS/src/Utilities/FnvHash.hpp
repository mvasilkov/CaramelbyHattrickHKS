
#pragma once

#include "SDK.hpp"

using fnv_t = unsigned;

class FnvHash
{
	static CONST fnv_t CONST FNV_PRIME = 16777619u;
	static CONST fnv_t CONST OFFSET_BASIS = 2166136261u;

	template < UINT N > static constexpr FORCEINLINE CONST fnv_t CONST fnvHashConst(CONST CHAR(&str)[N], UINT I = N)
	{
		return static_cast < fnv_t > (1ULL * (I == 1 ? (OFFSET_BASIS ^ str[0]) : (fnvHashConst(str, I - 1) ^ str[I - 1])) * FNV_PRIME);
	};

	static FORCEINLINE CONST fnv_t CONST fnvHash(const char * str)
	{
		static int iter = 0, len = 0;
		static fnv_t hash = 0;
		len = strlen(str) + 1, hash = OFFSET_BASIS;
		for (iter = 0; iter < len; iter++) hash ^= *str++, hash *= FNV_PRIME;
		return hash;
	};

	struct Wrapper
	{
		FORCEINLINE constexpr Wrapper(CONST char * str) : str(str) { };
		CONST CHAR * str;
	};

	fnv_t hash_value;

public:

	FORCEINLINE FnvHash(Wrapper wrapper) : hash_value(fnvHash(wrapper.str)) { };
	template < UINT N > constexpr FORCEINLINE CONST FnvHash(CONST CHAR(&str)[N]) : hash_value(fnvHashConst(str)) { };
	constexpr FORCEINLINE operator CONST fnv_t(VOID) CONST { return this->hash_value; };
};
