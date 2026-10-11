// Original: d:\build\custom\temp\code\client\projectg\actor.cpp
#include "actor.h"

#include "TaskManager.h"
#include "frwnd.h"

#include <string.h>

extern unsigned long g_CurrentTime;

// Placeholder actor (constructor, destructor, GetRTTI and HandleMsg at
// actor.cpp:181-191, registered with the factories by __impCDummy). Its
// declaration is not in actor.h's line info, so it is kept in this file.
class CDummy : public IActor {
public:
	static const WRTTI m_RTTI;

	CDummy(void);
	virtual const WRTTI* GetRTTI(void) const;
	virtual ~CDummy(void);
	virtual void HandleMsg(const MsgObject &msg);
};

// actor.cpp:23
CActorFactory& ActorFactory(void)
{
	static CActorFactory s_actorFactory;
	return s_actorFactory;
}

// actor.cpp:28
CActorFactory::CActorFactory(void)
{
}

// actor.cpp:32
CActorFactory::~CActorFactory(void)
{
}

// actor.cpp:37
IActor* CActorFactory::CreateActor(const char* name)
{
	std::vector<const char*>::iterator it;
	for (it = m_actorClassNames.begin(); it != m_actorClassNames.end(); ++it) {
		if (strcmp(*it, name) == 0) {
			IObject* pObject = ObjectFactory().CreateObject(*it);
			return DYNAMIC_CAST(IActor, pObject);
		}
	}
	return 0;
}

// actor.cpp:77
void CActorFactory::AddActorClass(const char* name)
{
	m_actorClassNames.push_back(name);
}

// actor.cpp:86 (run-time type and factory registration of IActor)
const WRTTI IActor::m_RTTI("IActor", &IObject::m_RTTI);

IObject* IActorMakeInstance(void)
{
	return new IActor;
}

struct __sIActor {
	__sIActor(void) {
		ObjectFactory().AddObjectFunctor(IActorMakeInstance, "IActor");
	}
};
static __sIActor s_registerIActor;

// actor.cpp:93
IActor::IActor(void) : m_flag(0), m_priority(2), m_skipLayer(0), m_pTask(0), m_pDocument(0)
{
	m_skipLayer = m_defaultSkipLayer;
}

// actor.cpp:98
IActor::~IActor(void)
{
}

// actor.cpp:102
CTaskDoc* IActor::GetDocument(void) const
{
	return m_pDocument;
}

// actor.cpp:108
CTask* IActor::GetTask(void) const
{
	return m_pTask;
}

// actor.cpp:114
void IActor::HandleMsg(const MsgObject &msg)
{
	if ((unsigned long)msg.m_reserved > g_CurrentTime) {
		StoreDelayedMsg(msg);
	}
}

// actor.cpp:122
int IActor::PostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4)
{
	return CTaskManager::Instance()->PostMsg(pSender, pTarget, msg, param1, param2, param3, param4);
}

// actor.cpp:127
int IActor::SendMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4)
{
	MsgObject msgObj;
	msgObj.m_msg = msg;
	msgObj.m_pSender = pSender;
	msgObj.m_param1 = param1;
	msgObj.m_param2 = param2;
	msgObj.m_param3 = param3;
	msgObj.m_reserved = param4;

	if (pTarget) {
		m_pTask->GetActor(pTarget) << msgObj;
	} else {
		m_pTask->SendGlobalMsg(msgObj);
	}

	return 0;
}

// actor.cpp:145
void IActor::AddActor(const char *className, const char *name, bool flag, const std::string &str)
{
	m_pTask->AddActor(className, name, flag, str);
}

// actor.cpp:150
IActor* IActor::GetActor(const char *name) const
{
	return m_pTask->GetActor(name);
}

// actor.cpp:155
void IActor::StoreDelayedMsg(const MsgObject &msg) const
{
}

// actor.cpp:160
bool IActor::Skipped(void) const
{
	if (m_pLayer && (*m_pLayer & m_skipLayer)) {
		return true;
	}
	return false;
}

// actor.cpp:168
void IActor::AddSkipList(unsigned long layer)
{
	m_skipLayer |= layer;
}

// actor.cpp:173
void IActor::RemoveSkipList(unsigned long layer)
{
	m_skipLayer &= ~layer;
}

// actor.cpp:181
CDummy::CDummy(void) {}
// actor.cpp:182
CDummy::~CDummy(void)
{
}

// actor.cpp:186
const WRTTI* CDummy::GetRTTI(void) const { return &m_RTTI; }

// actor.cpp:188
void CDummy::HandleMsg(const MsgObject &msg)
{
}

// actor.cpp:194 (run-time type and factory registration of CDummy)
const WRTTI CDummy::m_RTTI("CDummy", &IActor::m_RTTI);

IObject* CDummyMakeInstance(void)
{
	return new CDummy;
}

struct __impCDummy {
	__impCDummy(void) {
		ObjectFactory().AddObjectFunctor(CDummyMakeInstance, "CDummy");
		ActorFactory().AddActorClass("CDummy");
	}
};
static __impCDummy s_impCDummy;
