
#pragma once

#include "SDK.hpp"
#include "Platform.hpp"

class Kit_t {
public:
	int id;
	string name;
	FORCEINLINE CONST bool CONST operator < (CONST Kit_t & Other) CONST { return (name < Other.name); }
};

extern vector <Kit_t> k_skins;
extern vector <Kit_t> k_gloves;
extern vector <Kit_t> k_stickers;

CONST VOID CONST InitializeKits(VOID);
