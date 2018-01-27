
#include "SDK.hpp"
#include "ItemDefinitions.hpp"

map <size_t, Item_t> k_weapon_info =
{
	{ WEAPON_KNIFE,{ XorString("models/weapons/v_knife_default_ct.mdl"), XorString("knife_default_ct") } },
{ WEAPON_KNIFE_T,{ XorString("models/weapons/v_knife_default_t.mdl"), XorString("knife_t") } },
{ WEAPON_KNIFE_BAYONET,{ XorString("models/weapons/v_knife_bayonet.mdl"), XorString("bayonet") } },
{ WEAPON_KNIFE_FLIP,{ XorString("models/weapons/v_knife_flip.mdl"), XorString("knife_flip") } },
{ WEAPON_KNIFE_GUT,{ XorString("models/weapons/v_knife_gut.mdl"), XorString("knife_gut") } },
{ WEAPON_KNIFE_KARAMBIT,{ XorString("models/weapons/v_knife_karam.mdl"), XorString("knife_karambit") } },
{ WEAPON_KNIFE_M9_BAYONET,{ XorString("models/weapons/v_knife_m9_bay.mdl"), XorString("knife_m9_bayonet") } },
{ WEAPON_KNIFE_TACTICAL,{ XorString("models/weapons/v_knife_tactical.mdl"), XorString("knife_tactical") } },
{ WEAPON_KNIFE_FALCHION,{ XorString("models/weapons/v_knife_falchion_advanced.mdl"), XorString("knife_falchion") } },
{ WEAPON_KNIFE_SURVIVAL_BOWIE,{ XorString("models/weapons/v_knife_survival_bowie.mdl"), XorString("knife_survival_bowie") } },
{ WEAPON_KNIFE_BUTTERFLY,{ XorString("models/weapons/v_knife_butterfly.mdl"), XorString("knife_butterfly") } },
{ WEAPON_KNIFE_PUSH,{ XorString("models/weapons/v_knife_push.mdl"), XorString("knife_push") } },
{ GLOVE_STUDDED_BLOODHOUND,{ XorString("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl") } },
{ GLOVE_T_SIDE,{ XorString("models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl") } },
{ GLOVE_CT_SIDE,{ XorString("models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl") } },
{ GLOVE_SPORTY,{ XorString("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl") } },
{ GLOVE_SLICK,{ XorString("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl") } },
{ GLOVE_LEATHER_WRAP,{ XorString("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl") } },
{ GLOVE_MOTORCYCLE,{ XorString("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl") } },
{ GLOVE_SPECIALIST,{ XorString("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl") } },
};

vector <WeaponName_t> k_knife_names =
{
	{ 0, XorString("DEFAULT") },
{ WEAPON_KNIFE_BAYONET, XorString("BAYONET") },
{ WEAPON_KNIFE_FLIP, XorString("FLIP") },
{ WEAPON_KNIFE_GUT, XorString("GUT") },
{ WEAPON_KNIFE_KARAMBIT, XorString("KARAMBIT") },
{ WEAPON_KNIFE_M9_BAYONET, XorString("M9") },
{ WEAPON_KNIFE_TACTICAL, XorString("HUNTSMAN") },
{ WEAPON_KNIFE_FALCHION, XorString("FALCHION") },
{ WEAPON_KNIFE_SURVIVAL_BOWIE, XorString("BOWIE") },
{ WEAPON_KNIFE_BUTTERFLY, XorString("BUTTERFLY") },
{ WEAPON_KNIFE_PUSH, XorString("DAGGERS") },
};

vector <WeaponName_t> k_glove_names =
{
	{ 0, XorString("DEFAULT") },
{ GLOVE_STUDDED_BLOODHOUND, XorString("BLOODHOUND") },
{ GLOVE_T_SIDE, XorString("DEFAULT (TE)") },
{ GLOVE_CT_SIDE, XorString("DEFAULT (CT)") },
{ GLOVE_SPORTY, XorString("SPORT") },
{ GLOVE_SLICK, XorString("DRIVER") },
{ GLOVE_LEATHER_WRAP, XorString("WRAPS") },
{ GLOVE_MOTORCYCLE, XorString("MOTO") },
{ GLOVE_SPECIALIST, XorString("SPECIALIST") },
};

vector <WeaponName_t> k_weapon_names =
{
	{ WEAPON_KNIFE, XorString("KNIFE") },
{ GLOVE_T_SIDE, XorString("GLOVES") },
{ 7, XorString("AK") },
{ 8, XorString("AUG") },
{ 9, XorString("AWP") },
{ 63, XorString("CZ") },
{ 1, XorString("DEAGLE") },
{ 2, XorString("ELITES") },
{ 10, XorString("FAMAS") },
{ 3, XorString("FN57") },
{ 11, XorString("G3") },
{ 13, XorString("GALIL") },
{ 4, XorString("GLOCK") },
{ 14, XorString("M249") },
{ 60, XorString("M4-S") },
{ 16, XorString("M4") },
{ 17, XorString("MAC") },
{ 27, XorString("MAG") },
{ 33, XorString("MP7") },
{ 34, XorString("MP9") },
{ 28, XorString("NEGEV") },
{ 35, XorString("NOVA") },
{ 32, XorString("P2K") },
{ 36, XorString("P250") },
{ 19, XorString("P90") },
{ 26, XorString("BIZON") },
{ 64, XorString("R8") },
{ 29, XorString("SAWED") },
{ 38, XorString("SCAR") },
{ 40, XorString("SSG") },
{ 39, XorString("SG") },
{ 30, XorString("TEC") },
{ 24, XorString("UMP") },
{ 61, XorString("USP") },
{ 25, XorString("XM") },
};

vector <QualityName_t> k_quality_names =
{
	{ 0, XorString("NORMAL WEAPON") },
{ 3, XorString("KNIFE OR GLOVES") },
{ 12, XorString("SOUVENIR WEAPON") },
};
