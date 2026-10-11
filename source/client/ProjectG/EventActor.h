// Original: d:\build\custom\temp\code\client\projectg\eventactor.h
#ifndef PG_CLIENT_PROJECTG_EVENTACTOR_H
#define PG_CLIENT_PROJECTG_EVENTACTOR_H

#include <list>
#include <map>
#include <string>

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"
#include "wtemplate.h"

class IObject;

namespace _gateway {
class CGatewayHandler;
}

// Message id of a task (a MsgObject's m_msg). Its header and enumerators are
// not recovered yet.
enum taskMsg_t {
};

namespace _event {

// Enumerators not recovered yet.
enum eEventIdentifier {
};

// Actor of the event contents: creates the handler of each task and routes
// the task messages to them. Size 0x3c (the `new` of CEventActorMakeInstance).
class CEventActor : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CEventActorMakeInstance(void);   // the factory creates it
public:
	static const WRTTI m_RTTI;

	// eventactor.h:28
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	CEventActor(void);
public:
	virtual ~CEventActor(void);

	virtual void OnPreLoadInit(void);
	virtual void HandleMsg(const MsgObject& msg);

private:
	void CreateLobbyHandler(void);
	void CreateAvatarChatHandler(void);
	void CreateMyRoomHandler(void);
	void CreateShopHandler(void);
	void CreateGolfHandler(void);

	typedef std::map<taskMsg_t, _gateway::CGatewayHandler*> HandlerMap;
	typedef std::list<eEventIdentifier> EventList;
	typedef std::map<std::string, _util::WMemFun<CEventActor, void> > CreatorMap;

	HandlerMap m_handlers;      // 0x18
	EventList m_events;         // 0x24
	CreatorMap m_creators;      // 0x30, handler creator of each task class name
};

CHECK_SIZE(CEventActor, 0x3c);
BEGIN_LAYOUT_CHECK(CEventActor)
	CHECK_FIELD(CEventActor, m_handlers, 0x18);
	CHECK_FIELD(CEventActor, m_events, 0x24);
	CHECK_FIELD(CEventActor, m_creators, 0x30);
END_LAYOUT_CHECK

IObject* CEventActorMakeInstance(void);

}

#endif
