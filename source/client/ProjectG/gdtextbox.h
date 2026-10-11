// Original: d:\build\custom\temp\code\client\projectg\gdtextbox.h
#ifndef PG_CLIENT_PROJECTG_GDTEXTBOX_H
#define PG_CLIENT_PROJECTG_GDTEXTBOX_H

#include <map>
#include <string>

#include "igamedata.h"

// "textbox" game data: texts by key. Size 0x10 (CreateInstance).
class CGDTextBox : public IGameData {
	LAYOUT_CHECK_FRIEND;
public:
	DECLARE_GAMEDATA(CGDTextBox, eGAMEDATADB_TEXTBOX, "textbox") // gdtextbox.h:11

	CGDTextBox(void);
	virtual ~CGDTextBox(void);

	virtual void Initialize(TiXmlNode* pNode);
	const char* GetText(const char* szKey);
	const char* GetText(std::string& strKey);

private:
	std::map<std::string, std::string> m_mapText; // 0x4
};
CHECK_SIZE(CGDTextBox, 0x10);
BEGIN_LAYOUT_CHECK(CGDTextBox)
	CHECK_FIELD(CGDTextBox, m_mapText, 0x4);
END_LAYOUT_CHECK

#endif
