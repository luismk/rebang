// Original: d:\build\custom\temp\code\client\projectg\gamedatadb.h
#ifndef PG_CLIENT_PROJECTG_GAMEDATADB_H
#define PG_CLIENT_PROJECTG_GAMEDATADB_H

#include <map>
#include <string>

#include "..\..\defines.h"
#include "singleton.h"

class IGameData;
class TiXmlDocument;

// Kinds of game data the database holds; each has its own XML tag
// (RegisterGameData: "textbox", "bitmapbox", "eventnotice", "voiceitem").
enum eGameDataDB {
	eGAMEDATADB_TEXTBOX = 0,
	eGAMEDATADB_BITMAPBOX = 1,
	eGAMEDATADB_EVENTNOTICE = 2,
	eGAMEDATADB_VOICEITEM = 3,
	eGAMEDATADB_MAX = 4
};

typedef IGameData* (*GAMEDATA_CREATE_FUNC)(void);
typedef void (*GAMEDATA_DESTORY_FUNC)(IGameData*);

// One kind of game data: the instances by name, the factory pair of the
// kind's class and the XML tag naming its entries. Size 0x30 (the `eh
// vector constructor iterator` of CGameDataDB's constructor).
struct sGAMEDATA {
	std::map<std::string, IGameData*> mapGameData; // 0x0
	GAMEDATA_CREATE_FUNC pCreateFunc;              // 0xc
	GAMEDATA_DESTORY_FUNC pDestoryFunc;            // 0x10
	std::string strGameID;                         // 0x14
};
CHECK_SIZE(sGAMEDATA, 0x30);
CHECK_FIELD(sGAMEDATA, pCreateFunc, 0xc);
CHECK_FIELD(sGAMEDATA, pDestoryFunc, 0x10);
CHECK_FIELD(sGAMEDATA, strGameID, 0x14);

// Database of the XML-defined game data ("gamedata.xml"), loaded by the
// constructor. Members are transcribed as far as the decompiled functions
// need them (the size is not proven past the last array).
class CGameDataDB : public WSingleton<CGameDataDB> {
	LAYOUT_CHECK_FRIEND;
public:
	CGameDataDB(void);
	virtual ~CGameDataDB(void);

	void RegisterGameDataToDB(eGameDataDB eType, std::string& strName, IGameData* pGameData);
	void RegisterCreateFunction(eGameDataDB eType, GAMEDATA_CREATE_FUNC pFunc);
	void RegisterDestoryFunction(eGameDataDB eType, GAMEDATA_DESTORY_FUNC pFunc);
	void RegisterGameID(eGameDataDB eType, const char* szGameID);
	IGameData* GetGameData(eGameDataDB eType, const char* szName);

private:
	void LoadGameData(const char* szFileName);
	void RegisterGameData(void);
	void ParseGameData(TiXmlDocument& doc);
	void Destory(void);

	sGAMEDATA m_GameData[eGAMEDATADB_MAX]; // 0x28
};
BEGIN_LAYOUT_CHECK(CGameDataDB)
	CHECK_FIELD(CGameDataDB, m_GameData, 0x28);
END_LAYOUT_CHECK

#endif
