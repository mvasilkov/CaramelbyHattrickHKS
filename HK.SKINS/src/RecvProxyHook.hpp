
#pragma once

#include "SDK.hpp"

class RecvPropHook
{
public:

	FORCEINLINE RecvPropHook(RecvProp * prop, RecvVarProxyFn proxy_fn) : m_property(prop), m_original_proxy_fn(prop->m_ProxyFn)
	{
		SetProxyFunction(proxy_fn);
	};

	FORCEINLINE ~RecvPropHook(void)
	{
		m_property->m_ProxyFn = m_original_proxy_fn;
	};

	FORCEINLINE constexpr RecvVarProxyFn GetOriginalFunction()
	{
		return m_original_proxy_fn;
	};

	FORCEINLINE void SetProxyFunction(RecvVarProxyFn proxy_fn)
	{
		m_property->m_ProxyFn = proxy_fn;
	};

private:

	RecvProp * m_property;
	RecvVarProxyFn m_original_proxy_fn;
};
