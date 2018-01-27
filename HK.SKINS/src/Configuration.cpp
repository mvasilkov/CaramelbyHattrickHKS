
#include "SDK.hpp"
#include "Platform.hpp"
#include "Configuration.hpp"
#include <json.hpp>

Config g_config;

using Json = nlohmann::json;

FORCEINLINE CONST VOID to_json(Json & X, CONST StickerSetting & Sticker)
{
	X = Json{
		{ XorString("kit"), Sticker.kit_index },
	{ XorString("wear"), Sticker.wear },
	{ XorString("scale"), Sticker.scale },
	{ XorString("rotation"), Sticker.rotation },
	};
};

FORCEINLINE CONST VOID from_json(CONST Json & X, StickerSetting & Sticker)
{
	Sticker.kit_index = X.at(XorString("kit")).get <INT>();
	Sticker.wear = X.at(XorString("wear")).get <FLOAT>();
	if (Sticker.wear == 0.f || Sticker.wear == FLT_MIN || Sticker.wear < .00015f) Sticker.wear = .00015f;
	Sticker.scale = X.at(XorString("scale")).get <FLOAT>();
	if (Sticker.scale == 0.f || Sticker.scale == FLT_MIN) Sticker.scale = 1.f;
	Sticker.rotation = X.at(XorString("rotation")).get <FLOAT>();
	if (Sticker.rotation == FLT_MIN) Sticker.rotation = 0.f;
	Sticker.UpdateIds();
};

FORCEINLINE CONST VOID to_json(Json & X, CONST EconomyItem_t & Item)
{
	X = Json{
		{ XorString("name"), Item.name },
	{ XorString("enabled"), Item.enabled },
	{ XorString("definition_index"), Item.definition_index },
	{ XorString("entity_quality_index"), Item.entity_quality_index },
	{ XorString("paint_kit_index"), Item.paint_kit_index },
	{ XorString("definition_override_index"), Item.definition_override_index },
	{ XorString("seed"), Item.seed },
	{ XorString("stat_trak"), Item.stat_trak },
	{ XorString("wear"), Item.wear },
	{ XorString("custom_name"), Item.custom_name },
	{ XorString("stickers"), Item.stickers },
	};
};

FORCEINLINE CONST VOID from_json(CONST Json & X, EconomyItem_t & Item)
{
	static vector < StickerSetting > Stickers = {};
	static SIZE_T Iter = {};

	strcpy(Item.name, X.at(XorString("name")).get <string>().c_str()), Item.name[XS_RANDOM_SIZE / 10] = '\0';
	Item.enabled = X.at(XorString("enabled")).get <bool>();
	Item.definition_index = X.at(XorString("definition_index")).get <INT>();
	Item.entity_quality_index = X.at(XorString("entity_quality_index")).get <INT>();
	Item.paint_kit_index = X.at(XorString("paint_kit_index")).get <INT>();
	Item.definition_override_index = X.at(XorString("definition_override_index")).get <INT>();
	Item.seed = X.at(XorString("seed")).get <INT>();
	Item.stat_trak = X.at(XorString("stat_trak")).get <INT>();
	Item.wear = X.at(XorString("wear")).get <FLOAT>();
	if (Item.wear == 0.f || Item.wear == FLT_MIN || Item.wear < .00015f) Item.wear = .00015f;
	strcpy(Item.custom_name, X.at(XorString("custom_name")).get <string>().c_str()), Item.custom_name[XS_RANDOM_SIZE / 10] = '\0';
	Stickers = X.at(XorString("stickers")).get < vector < StickerSetting > >();
	for (Iter = 0; Iter < ((Stickers.size() < Item.stickers.size()) ? Stickers.size() : Item.stickers.size()); Iter++) Item.stickers[Iter] = Stickers[Iter];
	Item.UpdateIds();
};

VOID Config::Save(VOID)
{
	static ofstream To;
	To.open(XorString("hSkins.JSON"));
	if (To.is_open()) To << Json(m_items), To.close();
};

VOID Config::Load(VOID)
{
	static ifstream From;
	static CHAR szBuffer[XS_RANDOM_SIZE / 4] = {};
	static FILE * pFile = NULL;
	static INT iIter = 0, iOther = 0;
	pFile = fopen(XorString("hSkins.JSON"), XorString("r"));
	if (pFile)
	{
		fgets(szBuffer, ARRAYSIZE(szBuffer), pFile);
		if (strchr(szBuffer, '['))
		{
			fclose(pFile);
			From.open(XorString("hSkins.json"));
			if (From.is_open()) m_items = Json::parse(From).get < vector < EconomyItem_t > >(), From.close(), (*g_client_state)->ForceFullUpdate();
		}
		else
		{
			fclose(pFile);
			pFile = fopen(XorString("hSkins.json"), XorString("a+"));
			if (pFile)
			{
				string Data = XorString("[]");
				fprintf(pFile, Data.c_str());
				fclose(pFile);
				From.open(XorString("hSkins.JSON"));
				if (From.is_open()) m_items = Json::parse(From).get < vector < EconomyItem_t > >(), From.close(), (*g_client_state)->ForceFullUpdate();
			};
		};
	}
	else
	{
		pFile = fopen(XorString("hSkins.json"), XorString("a+"));
		if (pFile)
		{
			string Data = XorString("[]");
			fprintf(pFile, Data.c_str());
			fclose(pFile);
			From.open(XorString("hSkins.JSON"));
			if (From.is_open()) m_items = Json::parse(From).get < vector < EconomyItem_t > >(), From.close(), (*g_client_state)->ForceFullUpdate();
		};
	};
	for (iIter = 0; iIter < m_items.size(); iIter++)
	{
		if (m_items.at(iIter).wear == 0.f || m_items.at(iIter).wear == FLT_MIN || m_items.at(iIter).wear < .00015f) m_items.at(iIter).wear = .00015f;
		m_items.at(iIter).name[XS_RANDOM_SIZE / 10] = '\0', m_items.at(iIter).custom_name[XS_RANDOM_SIZE / 10] = '\0';
		for (iOther = 0; iOther < m_items.at(iIter).stickers.size(); iOther++)
		{
			if (m_items.at(iIter).stickers.at(iOther).wear == 0.f || m_items.at(iIter).stickers.at(iOther).wear == FLT_MIN || m_items.at(iIter).stickers.at(iOther).wear < .00015f) m_items.at(iIter).stickers.at(iOther).wear = .00015f;
			if (m_items.at(iIter).stickers.at(iOther).rotation == FLT_MIN) m_items.at(iIter).stickers.at(iOther).rotation = 0.f;
			if (m_items.at(iIter).stickers.at(iOther).scale == 0.f || m_items.at(iIter).stickers.at(iOther).scale == FLT_MIN) m_items.at(iIter).stickers.at(iOther).scale = 1.f;
		};
	};
};

EconomyItem_t * Config::GetByDefinitionIndex(INT iId)
{
	for (auto X : m_items)
		if (X.enabled && X.definition_index == iId)
			return &X;
	return nullptr;
};
