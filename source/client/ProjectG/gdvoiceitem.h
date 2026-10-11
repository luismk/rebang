// Original: d:\build\custom\temp\code\client\projectg\gdvoiceitem.h
#ifndef PG_CLIENT_PROJECTG_GDVOICEITEM_H
#define PG_CLIENT_PROJECTG_GDVOICEITEM_H

#include <list>
#include <string>

#include "igamedata.h"

// One "voiceitem" row. Size 0x28.
struct sGD_VOICEITEM {
	sGD_VOICEITEM(void) {                        // gdvoiceitem.h:11
		strName = "";                              // gdvoiceitem.h:13
		byNumOfSounds = 0; nState = 0; nTypeID = 0; // gdvoiceitem.h:15
	}

	int nTypeID;                 // 0x0
	std::string strName;         // 0x4
	int nState;                  // 0x20
	unsigned char byNumOfSounds; // 0x24
};
CHECK_SIZE(sGD_VOICEITEM, 0x28);
CHECK_FIELD(sGD_VOICEITEM, strName, 0x4);
CHECK_FIELD(sGD_VOICEITEM, nState, 0x20);
CHECK_FIELD(sGD_VOICEITEM, byNumOfSounds, 0x24);

// "voiceitem" game data: the list of voice items. Size 0x10.
class CGDVoiceItem : public IGameData {
	LAYOUT_CHECK_FRIEND;
public:
	DECLARE_GAMEDATA(CGDVoiceItem, eGAMEDATADB_VOICEITEM, "voiceitem") // gdvoiceitem.h:24

	CGDVoiceItem(void);
	virtual ~CGDVoiceItem(void);

	virtual void Initialize(TiXmlNode* pNode);

	std::list<sGD_VOICEITEM>& GetVoiceList(void) { return m_listVoiceItem; } // gdvoiceitem.h:32

private:
	std::list<sGD_VOICEITEM> m_listVoiceItem; // 0x4
};
CHECK_SIZE(CGDVoiceItem, 0x10);
BEGIN_LAYOUT_CHECK(CGDVoiceItem)
	CHECK_FIELD(CGDVoiceItem, m_listVoiceItem, 0x4);
END_LAYOUT_CHECK

#endif
