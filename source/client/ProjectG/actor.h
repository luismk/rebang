// Original: d:\build\custom\temp\code\client\projectg\actor.h
#ifndef PG_CLIENT_PROJECTG_ACTOR_H
#define PG_CLIENT_PROJECTG_ACTOR_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "ObjectFactory.h"

class CTask;
class IActor;
class CTaskDoc;
class WRTTI;
class MsgObject;

// Message passed between actors: the sender, the message id and five generic
// parameters. A parameter carries either an integer or a pointer (the handlers
// cast it back: `(const char *)msg.m_param1`, `*(int *)msg.m_param1`), so it is
// pointer-sized: `int` on the 32-bit match, 8 bytes on a 64-bit target
// (docs/PORTABILITY.md). m_reserved holds the delivery time of a delayed
// message (compared with g_CurrentTime by IActor::HandleMsg; cleared here, so
// the message is delivered at once).
class MsgObject {
public:
	// actor.h:27 (defined below the class)
	MsgObject(void);
	// actor.h:32
	MsgObject(const IActor *pSender, int msg, intptr_t param1, intptr_t param2, intptr_t param3, intptr_t param4, intptr_t param5) : m_pSender(pSender), m_msg(msg), m_param1(param1), m_param2(param2), m_param3(param3), m_param4(param4), m_param5(param5) {
		m_reserved = 0;
	}

	const IActor *m_pSender; // 0x00
	int m_msg;               // 0x04
	intptr_t m_param1;       // 0x08
	intptr_t m_param2;       // 0x0C
	intptr_t m_param3;       // 0x10
	intptr_t m_param4;       // 0x14
	intptr_t m_param5;       // 0x18
	int m_reserved;          // 0x1C (delivery time, against g_CurrentTime)
};

// CLIENTSETTING_ACTOR_INLINE is defined empty, before its first include, by
// ClientSetting.cpp, the one unit whose object kept the out-of-line copy of
// MsgObject's default constructor in the original; every other unit inlines it.
#ifndef CLIENTSETTING_ACTOR_INLINE
#define CLIENTSETTING_ACTOR_INLINE inline
#endif

// actor.h:27
CLIENTSETTING_ACTOR_INLINE MsgObject::MsgObject(void) {
}

CHECK_SIZE(MsgObject, 0x20);
CHECK_FIELD(MsgObject, m_pSender, 0x00);
CHECK_FIELD(MsgObject, m_msg, 0x04);
CHECK_FIELD(MsgObject, m_param5, 0x18);
CHECK_FIELD(MsgObject, m_reserved, 0x1C);

// IActor derives from IObject, which has only a vtable (sizeof 4); the members
// below therefore start at offset 4, giving IActor its original size 0x18.
// Virtuals in vtable order (from ??_7NetSyncActor@@6BIActor@@@): GetRTTI and
// the destructor (IObject's slots), then the ones declared below.
class IActor : public IObject {
	LAYOUT_CHECK_FRIEND;
	friend class CTask;
	// Delivers msg through the protected HandleMsg (actor.h:163).
	friend IActor* operator<<(IActor* pActor, const MsgObject& msg);
public:
	// Display priority (values not known yet).
	enum eDispPriority {
		DISP_PRIORITY_DEFAULT = 0
	};

	static const WRTTI m_RTTI;

	IActor(void);
	// actor.h:93
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }
	virtual ~IActor(void);
	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	// actor.h:122
	virtual void OnProcess(float) {}
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);
	virtual void OnPreserveBack(char const *);
	virtual void OnRestorePreserved(void);

	// Layer mask of the skipped actors (see Skipped).
	static unsigned long *m_pLayer;
	// Skip layer given to new actors (cleared by the tutorial task).
	static unsigned long m_defaultSkipLayer;

	IActor* GetActor(const char *name) const;

	int PostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4);
	int SendMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4);
	void AddActor(const char *className, const char *name, bool flag, const std::string &str);

protected:
	virtual void HandleMsg(const MsgObject &msg);
	virtual void StoreDelayedMsg(const MsgObject &msg) const;
	// actor.h:143
	virtual eDispPriority GetDisplayPriority(void) { return (eDispPriority)m_priority; }
	virtual bool Skipped(void) const;

	CTaskDoc* GetDocument(void) const;
	CTask* GetTask(void) const;
	void AddSkipList(unsigned long layer);
	void RemoveSkipList(unsigned long layer);

protected:
	int m_flag;                 // 0x4
	int m_priority;             // 0x8
	unsigned long m_skipLayer;  // 0xc
	CTask *m_pTask;             // 0x10
	CTaskDoc *m_pDocument;      // 0x14
};

CHECK_SIZE(IActor, 0x18);
BEGIN_LAYOUT_CHECK(IActor)
	CHECK_FIELD(IActor, m_skipLayer, 0xc);
	CHECK_FIELD(IActor, m_pTask, 0x10);
	CHECK_FIELD(IActor, m_pDocument, 0x14);
END_LAYOUT_CHECK

// Name -> actor class registry. The map's role is read from CreateActor
// (not transcribed yet); AddActorClass appends to the name list.
class CActorFactory {
	LAYOUT_CHECK_FRIEND;
public:
	CActorFactory(void);
	~CActorFactory(void);

	IActor* CreateActor(const char* name);
	void AddActorClass(const char* name);

private:
	std::map<std::string, std::string> m_actorMap; // 0x0
	std::vector<const char*> m_actorClassNames;    // 0xc
};

CHECK_SIZE(CActorFactory, 0x1c);
BEGIN_LAYOUT_CHECK(CActorFactory)
	CHECK_FIELD(CActorFactory, m_actorClassNames, 0xc);
END_LAYOUT_CHECK

CActorFactory& ActorFactory(void);

// actor.h:163 (out of line in SharedDoc, 0x40bd70; inlined elsewhere):
// has pActor handle msg at once.
inline IActor* operator<<(IActor* pActor, const MsgObject& msg) {
	if (!pActor) {
		return 0;
	}
	pActor->HandleMsg(msg);
	return pActor;
}

#endif // PG_CLIENT_PROJECTG_ACTOR_H
