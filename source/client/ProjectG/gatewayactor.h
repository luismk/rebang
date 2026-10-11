// Original: d:\build\custom\temp\code\client\projectg\gatewayactor.h
#ifndef PG_CLIENT_PROJECTG_GATEWAYACTOR_H
#define PG_CLIENT_PROJECTG_GATEWAYACTOR_H

#include <map>
#include <string>

#include "..\..\defines.h"
#include "actor.h"
#include "RTTI.h"
#include "Localize.h"
#include "EventActor.h"

class IObject;

namespace _gateway {

class CGatewayHandler;

IObject* CGatewayActorMakeInstance(void);

// Actor of the gateway contents: creates the handler of each task (from the
// task's class name) and routes the task messages to them. Size 0x30 (the
// `new` of CGatewayActorMakeInstance).
class CGatewayActor : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CGatewayActorMakeInstance(void);
public:
	static const WRTTI m_RTTI;

	// gatewayactor.h:21
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	CGatewayActor(void);
public:
	virtual ~CGatewayActor(void);

	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);

private:
	bool CreateHandler(intptr_t name);
	bool InsertHandler(taskMsg_t msg, CGatewayHandler* pHandler);
	void QueryHandler(localContentType_t type, taskMsg_t msg);

	void CreateLobbyHandler(void);
	void CreateAvatarChatHandler(void);
	void CreateMyRoomHandler(void);
	void CreateShopHandler(void);
	void CreateGolfHandler(void);

	typedef std::map<taskMsg_t, CGatewayHandler*> HandlerMap;
	typedef void (CGatewayActor::*CreatorFunc)(void);
	typedef std::map<std::string, CreatorFunc> CreatorMap;

	HandlerMap m_handlers;      // 0x18
	CreatorMap m_creators;      // 0x24, handler creator of each task class name
};

CHECK_SIZE(CGatewayActor, 0x30);
BEGIN_LAYOUT_CHECK(CGatewayActor)
	CHECK_FIELD(CGatewayActor, m_handlers, 0x18);
	CHECK_FIELD(CGatewayActor, m_creators, 0x24);
END_LAYOUT_CHECK

}

#endif // PG_CLIENT_PROJECTG_GATEWAYACTOR_H
