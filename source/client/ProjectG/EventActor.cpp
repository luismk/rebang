// Original: d:\build\custom\temp\code\client\projectg\eventactor.cpp
#include "EventActor.h"
#include "ObjectFactory.h"
#include "gatewayinterface.h"

// Task class names, "CLobbyTask", "CShopTask", "CRealMyRoomTask",
// "ntAvatarChatTask", "CGolfTask" (unnamed data, not cut yet).
extern "C" const char* data_a9df84[5];

// eventactor.cpp:10 (run-time type and factory registration of CEventActor)
const WRTTI _event::CEventActor::m_RTTI("CEventActor", &IActor::m_RTTI);

namespace _event {

IObject* CEventActorMakeInstance(void) {
	return new CEventActor;
}

struct __impCEventActor {
	__impCEventActor(void) {
		ObjectFactory().AddObjectFunctor(CEventActorMakeInstance, "CEventActor");
		ActorFactory().AddActorClass("CEventActor");
	}
};
static __impCEventActor s_impCEventActor;

}

// eventactor.cpp:13
_event::CEventActor::CEventActor(void) {
	m_creators.insert(std::make_pair(data_a9df84[0], _util::MakeMemFun(*this, &CEventActor::CreateLobbyHandler)));
	m_creators.insert(std::make_pair(data_a9df84[1], _util::MakeMemFun(*this, &CEventActor::CreateShopHandler)));
	m_creators.insert(std::make_pair(data_a9df84[2], _util::MakeMemFun(*this, &CEventActor::CreateMyRoomHandler)));
	m_creators.insert(std::make_pair(data_a9df84[3], _util::MakeMemFun(*this, &CEventActor::CreateAvatarChatHandler)));
	m_creators.insert(std::make_pair(data_a9df84[4], _util::MakeMemFun(*this, &CEventActor::CreateGolfHandler)));
}

// eventactor.cpp:22
_event::CEventActor::~CEventActor(void) {
}

// eventactor.cpp:27
void _event::CEventActor::OnPreLoadInit(void) {
	m_events.push_back((eEventIdentifier)0);
}

// eventactor.cpp:37
void _event::CEventActor::HandleMsg(const MsgObject& msg) {
	taskMsg_t msgId = (taskMsg_t)msg.m_msg;
	if (msgId == 0x274 || msgId == 0x2712) {
		return;
	}

	HandlerMap::iterator it;
	it = m_handlers.find(msgId);
	if (it == m_handlers.end()) {
		return;
	}

	it->second->Execute(msg);
}

// eventactor.cpp:69
void _event::CEventActor::CreateLobbyHandler(void) {
}

// eventactor.cpp:74
void _event::CEventActor::CreateAvatarChatHandler(void) {
}

// eventactor.cpp:79
void _event::CEventActor::CreateMyRoomHandler(void) {
}

// eventactor.cpp:84
void _event::CEventActor::CreateShopHandler(void) {
}

// eventactor.cpp:89
void _event::CEventActor::CreateGolfHandler(void) {
}
