// Original: d:\build\custom\temp\code\client\projectg\igamedata.h
#ifndef PG_CLIENT_PROJECTG_IGAMEDATA_H
#define PG_CLIENT_PROJECTG_IGAMEDATA_H

#include "gamedatadb.h"

class TiXmlNode;

// Factory pair and registration of a game data class with CGameDataDB.
// Used on one line of each class (gdtextbox.h:11, gdbitmapbox.h:20,
// gdeventnotice.h:11, gdvoiceitem.h:24): the three functions carry that line.
#define DECLARE_GAMEDATA(className, eType, szGameID) \
	static IGameData* CreateInstance(void) { return new className; } \
	static void DestoryInstance(IGameData* pGameData) { if (pGameData) delete pGameData; } \
	static void RegisterToGameDB(void) { \
		CGameDataDB::Instance()->RegisterGameID(eType, szGameID); \
		CGameDataDB::Instance()->RegisterCreateFunction(eType, CreateInstance); \
		CGameDataDB::Instance()->RegisterDestoryFunction(eType, DestoryInstance); \
	}

// Base of the XML-defined game data kinds held by CGameDataDB. Size 0x4.
class IGameData {
public:
	IGameData(void) {}          // igamedata.h:35
	virtual ~IGameData(void) {} // igamedata.h:36

	virtual void Initialize(TiXmlNode* pNode) = 0;
};
CHECK_SIZE(IGameData, 0x4);

#endif
