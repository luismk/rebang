// Original: d:\build\custom\temp\code\client\projectg\gamedatadb.cpp
#include "gamedatadb.h"
#include "gdtextbox.h"
#include "gdbitmapbox.h"
#include "gdeventnotice.h"
#include "gdvoiceitem.h"
#include "tinyxml.h"

template <> CGameDataDB* WSingleton<CGameDataDB>::m_pInstance = NULL;

// gamedatadb.cpp:11
CGameDataDB::CGameDataDB(void) {
	RegisterGameData();
	LoadGameData("gamedata.xml");
}

// gamedatadb.cpp:17
CGameDataDB::~CGameDataDB(void) {
	Destory();
}

// gamedatadb.cpp:22
void CGameDataDB::RegisterGameDataToDB(eGameDataDB eType, std::string& strName, IGameData* pGameData) {
	std::map<std::string, IGameData*>::iterator it = m_GameData[eType].mapGameData.find(strName);
	if (it == m_GameData[eType].mapGameData.end()) {
		m_GameData[eType].mapGameData.insert(std::map<std::string, IGameData*>::value_type(strName, pGameData));
	}
}

// gamedatadb.cpp:34
void CGameDataDB::LoadGameData(const char* szFileName) {
	TiXmlDocument doc;

	if (doc.LoadFileEx(szFileName) == false) {

		return;
	}
	ParseGameData(doc);
}

// gamedatadb.cpp:48
void CGameDataDB::RegisterCreateFunction(eGameDataDB eType, GAMEDATA_CREATE_FUNC pFunc) {
	m_GameData[eType].pCreateFunc = pFunc;
}

// gamedatadb.cpp:55
void CGameDataDB::RegisterDestoryFunction(eGameDataDB eType, GAMEDATA_DESTORY_FUNC pFunc) {
	m_GameData[eType].pDestoryFunc = pFunc;
}

// gamedatadb.cpp:61
void CGameDataDB::RegisterGameID(eGameDataDB eType, const char* szGameID) {
	m_GameData[eType].strGameID = szGameID;
}

// gamedatadb.cpp:68
void CGameDataDB::RegisterGameData(void) {
	CGDTextBox::RegisterToGameDB();
	CGDBitmapBox::RegisterToGameDB();

	CGDEventNotice::RegisterToGameDB();

	CGDVoiceItem::RegisterToGameDB();
}

// gamedatadb.cpp:80
void CGameDataDB::ParseGameData(TiXmlDocument& doc) {
	for (int i = 0; i < eGAMEDATADB_MAX; i++) {

		TiXmlNode* pNode = doc.FirstChild(m_GameData[i].strGameID.c_str());
		while (pNode) {
			IGameData* pGameData = m_GameData[i].pCreateFunc();
			std::string strName = pNode->ToElement()->Attribute("name");
			pGameData->Initialize(pNode);
			RegisterGameDataToDB((eGameDataDB)i, strName, pGameData);
			pNode = pNode->NextSibling(m_GameData[i].strGameID.c_str());
		}
	}
}

// gamedatadb.cpp:96
void CGameDataDB::Destory(void) {
	for (int i = 0; i < eGAMEDATADB_MAX; i++) {

		std::map<std::string, IGameData*>::iterator it = m_GameData[i].mapGameData.begin();
		while (it != m_GameData[i].mapGameData.end()) {

			m_GameData[i].pDestoryFunc(it->second);
			++it;
		}
	}
}

// gamedatadb.cpp:109
IGameData* CGameDataDB::GetGameData(eGameDataDB eType, const char* szName) {
	std::map<std::string, IGameData*>::iterator it = m_GameData[eType].mapGameData.find(szName);
	if (it != m_GameData[eType].mapGameData.end()) {

		return it->second;
	}
	return NULL;
}
