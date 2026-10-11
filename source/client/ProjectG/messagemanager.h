// Original: d:\build\custom\temp\code\client\projectg\messagemanager.h
#ifndef PG_CLIENT_PROJECTG_MESSAGEMANAGER_H
#define PG_CLIENT_PROJECTG_MESSAGEMANAGER_H

#include <map>

#include "..\..\defines.h"
#include "singleton.h"

namespace _systemmsg {

// Identifier of a system message text (the enumerators are not known yet;
// callers cast their message number to it).
enum eMsgIdentifier {
};

// One entry of a message table registered with
// CMessageManager::InsertMessageGroup.
struct sMsgInfo {
	eMsgIdentifier id;                  // 0x0
	const char* text;                   // 0x4
};
CHECK_SIZE(sMsgInfo, 0x8);
CHECK_FIELD(sMsgInfo, id, 0x0);
CHECK_FIELD(sMsgInfo, text, 0x4);

// The system message table: message identifier -> text.
class CMessageManager : public WSingleton<CMessageManager> {
	LAYOUT_CHECK_FRIEND;
public:
	CMessageManager(void);
	virtual ~CMessageManager(void);

	bool InsertMessage(eMsgIdentifier id, const char* text);
	bool InsertMessageGroup(sMsgInfo* pInfo, int count);
	const char* GetMsg(eMsgIdentifier id);
	void NotifyNormalMessage(eMsgIdentifier id);
	void NotifyNormalMessage(const char* text);

private:
	typedef std::map<eMsgIdentifier, const char*> MessageMap;

	MessageMap m_messages;              // 0x28
};
CHECK_SIZE(CMessageManager, 0x34);
BEGIN_LAYOUT_CHECK(CMessageManager)
	CHECK_FIELD(CMessageManager, m_messages, 0x28);
END_LAYOUT_CHECK

} // namespace _systemmsg

#endif // PG_CLIENT_PROJECTG_MESSAGEMANAGER_H
