
#pragma once

#include "SDK.hpp"

struct model_t;

class IVModelInfoClient
{
public:

	virtual ~IVModelInfoClient() { };

	virtual const model_t * GetModel(int) const = 0;
	virtual int GetModelIndex(const char *) const = 0;
	virtual const char * GetModelName(const model_t *) const = 0;
};

#define VMODELINFO_CLIENT_INTERFACE_VERSION "VModelInfoClient004"

extern IVModelInfoClient * g_model_info;
