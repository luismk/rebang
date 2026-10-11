// Original: d:\build\custom\temp\code\client\projectg\gdeventnotice.h
#ifndef PG_CLIENT_PROJECTG_GDEVENTNOTICE_H
#define PG_CLIENT_PROJECTG_GDEVENTNOTICE_H

#include <map>
#include <string>

#include "igamedata.h"

// "eventnotice" game data: notice texts by numeric key. Size 0x10.
class CGDEventNotice : public IGameData {
	LAYOUT_CHECK_FRIEND;
public:
	DECLARE_GAMEDATA(CGDEventNotice, eGAMEDATADB_EVENTNOTICE, "eventnotice") // gdeventnotice.h:11

	CGDEventNotice(void);
	virtual ~CGDEventNotice(void);

	virtual void Initialize(TiXmlNode* pNode);
	const char* GetText(int nKey);

private:
	std::map<int, std::string> m_mapText; // 0x4
};
CHECK_SIZE(CGDEventNotice, 0x10);
BEGIN_LAYOUT_CHECK(CGDEventNotice)
	CHECK_FIELD(CGDEventNotice, m_mapText, 0x4);
END_LAYOUT_CHECK

#endif
