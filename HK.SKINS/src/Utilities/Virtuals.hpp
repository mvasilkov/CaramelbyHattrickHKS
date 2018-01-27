
#pragma once

#include "SDK.hpp"

template < typename Fn = void * > FORCEINLINE Fn GetVirtualFunction(void * class_base, size_t index)
{
	return (*reinterpret_cast < Fn ** > (class_base))[index];
};
