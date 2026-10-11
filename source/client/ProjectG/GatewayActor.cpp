// Original: d:\build\custom\temp\code\client\projectg\gatewayactor.cpp
#include "GatewayActor.h"
#include "ObjectFactory.h"
#include "gatewayinterface.h"
#include "icontentsdoc.h"
#include "Exception.h"

// Task class names, "CLobbyTask", "CShopTask", "CRealMyRoomTask",
// "ntAvatarChatTask", "CGolfTask" (unnamed data, not cut yet).
extern "C" const char* data_a9df60[5];

// gatewayactor.cpp:20 (run-time type and factory registration of CGatewayActor)
const WRTTI _gateway::CGatewayActor::m_RTTI("CGatewayActor", &IActor::m_RTTI);

namespace _gateway {

struct __impCGatewayActor {
	__impCGatewayActor(void) {
		ObjectFactory().AddObjectFunctor(CGatewayActorMakeInstance, "CGatewayActor");
		ActorFactory().AddActorClass("CGatewayActor");
	}
};
static __impCGatewayActor s_impCGatewayActor;

// gatewayactor.cpp:20
IObject* CGatewayActorMakeInstance(void) {
	return new CGatewayActor;
}

}

// gatewayactor.cpp:23
_gateway::CGatewayActor::CGatewayActor(void) {
	m_creators.insert(std::make_pair(data_a9df60[0], &CGatewayActor::CreateLobbyHandler));
	m_creators.insert(std::make_pair(data_a9df60[1], &CGatewayActor::CreateShopHandler));
	m_creators.insert(std::make_pair(data_a9df60[2], &CGatewayActor::CreateMyRoomHandler));
	m_creators.insert(std::make_pair(data_a9df60[3], &CGatewayActor::CreateAvatarChatHandler));
	m_creators.insert(std::make_pair(data_a9df60[4], &CGatewayActor::CreateGolfHandler));
}

// gatewayactor.cpp:32
_gateway::CGatewayActor::~CGatewayActor(void) {
}

// gatewayactor.cpp:37
void _gateway::CGatewayActor::OnDestroy(void) {
	m_creators.clear();
}

// gatewayactor.cpp:42
void _gateway::CGatewayActor::HandleMsg(const MsgObject& msg) {
	taskMsg_t msgId = (taskMsg_t)msg.m_msg;
	switch (msgId) {
	case 0x2712:
		CreateHandler(msg.m_param1);
		return;
	}

	HandlerMap::iterator it;
	it = m_handlers.find(msgId);
	if (it == m_handlers.end()) {
		return;
	}

	it->second->Execute(msg);
}

// gatewayactor.cpp:67
bool _gateway::CGatewayActor::CreateHandler(intptr_t name) {
	CreatorMap::iterator it;
	it = m_creators.find(std::string(reinterpret_cast<const char*>(name)));
	if (it == m_creators.end()) {
		return false;
	}

	(this->*it->second)();
	return true;
}

// gatewayactor.cpp:84
bool _gateway::CGatewayActor::InsertHandler(taskMsg_t msg, CGatewayHandler* pHandler) {
	m_handlers.insert(HandlerMap::value_type(msg, pHandler));
	return true;
}

// gatewayactor.cpp:92
void _gateway::CGatewayActor::QueryHandler(localContentType_t type, taskMsg_t msg) {
	if (type < 0x9D || type > 0xA0) {
		throw WAppException("Passed Unknown doctype!!\nCode[0]");
	}

	IContentsDataContainer* pDoc = WSingleton<CContentsDoc>::Instance()->GetContainer(type);
	if (!pDoc) {
		throw WAppException("Need Doc Object!!\nCode[1]");
	}

	CGatewayHandler* pHandler = pDoc->GetGatewayHandler();
	if (!pHandler) {
		throw WAppException("Need Gateway Handler!!\nCode[2]");
	}

	pHandler->SetDocument(pDoc);
	InsertHandler(msg, pHandler);
}

// gatewayactor.cpp:129
void _gateway::CGatewayActor::CreateLobbyHandler(void) {
	QueryHandler((localContentType_t)0x9D, (taskMsg_t)0x26D);
	QueryHandler((localContentType_t)0x9E, (taskMsg_t)0x26F);
	QueryHandler((localContentType_t)0xA0, (taskMsg_t)0x276);
}

// gatewayactor.cpp:141
void _gateway::CGatewayActor::CreateAvatarChatHandler(void) {
	QueryHandler((localContentType_t)0x9D, (taskMsg_t)0x26D);
	QueryHandler((localContentType_t)0x9F, (taskMsg_t)0x273);
}

// gatewayactor.cpp:149
void _gateway::CGatewayActor::CreateMyRoomHandler(void) {
	QueryHandler((localContentType_t)0x9D, (taskMsg_t)0x26D);
	QueryHandler((localContentType_t)0xA0, (taskMsg_t)0x276);
}

// gatewayactor.cpp:157
void _gateway::CGatewayActor::CreateShopHandler(void) {
	QueryHandler((localContentType_t)0x9D, (taskMsg_t)0x26D);
}

// gatewayactor.cpp:162
void _gateway::CGatewayActor::CreateGolfHandler(void) {
	QueryHandler((localContentType_t)0x9E, (taskMsg_t)0x26F);
}
