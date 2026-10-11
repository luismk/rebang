// Original: d:\build\custom\temp\code\client\projectg\familytask.cpp
#include "FamilyTask.h"

#include <stdio.h>

#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "SharedDoc.h"
#include "Packet.h"

extern int g_iRecvPacketType;

// A level's name and the experience it takes (unnamed .rdata table, one
// record per level, 20 bytes each).
struct sLevelExpInfo {
	char name[16];   // 0x0
	long exp;        // 0x10
};
extern const sLevelExpInfo data_a291c0[];

// Minimal placeholder for the level experience table used by familytask.
// The real table is large and game-specific; provide an empty table to
// satisfy the linker during progressive stubbing. This should be replaced
// with the correct data later.
const sLevelExpInfo data_a291c0[] = { { "", 0 } };

// familytask.cpp:20 (run-time type and factory registration of CFamilyTask)
const WRTTI CFamilyTask::m_RTTI("CFamilyTask", &CTask::m_RTTI);

IObject* CFamilyTaskMakeInstance(void) {
	return new CFamilyTask;
}

struct __sCFamilyTask {
	__sCFamilyTask(void) {
		ObjectFactory().AddObjectFunctor(CFamilyTaskMakeInstance, "CFamilyTask");
	}
};
static __sCFamilyTask s_registerCFamilyTask;

// familytask.cpp:24
CFamilyTask::CFamilyTask(void) {
	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
}

// familytask.cpp:30
CFamilyTask::~CFamilyTask(void) {
}

// familytask.cpp:38
void CFamilyTask::Init(const char *backGround) {
	CTask::Init("family_background.jpg");
	SetWhisper(false);
}

// familytask.cpp:48
void CFamilyTask::Register(void) {
	SetMainActor(AddActor("CFamilyMain", "Family", 0, ""));
}

// familytask.cpp:53
void CFamilyTask::Load(void) {
	SetProgressBar(false);
	SetTip(false);
	CTask::Load();
}

// familytask.cpp:61
void CFamilyTask::Process(float dt) {
	CTask::Process(dt);
	g_pFresh->OnProcess(dt);
}

// familytask.cpp:67
void CFamilyTask::Display(void) {
	CTask::Display();
	g_pFresh->OnDisplay(true);
}

// familytask.cpp:73
int CFamilyTask::OnPacket(WReceivedPacket &packet) {
	unsigned short type = packet.Decode2();

	g_iRecvPacketType = type;

	switch (g_iRecvPacketType) {
	case 0xAD: {
		char msg[256];
		unsigned char level = packet.Decode1();
		unsigned long exp = packet.Decode4();

		CSharedDoc::Instance()->m_myExp += exp;

		if (level > CSharedDoc::Instance()->m_myLevel) {
			CSharedDoc::Instance()->m_myExp -= data_a291c0[CSharedDoc::Instance()->m_myLevel].exp;
			sprintf(msg, "%d\xC0\xC7 \xB0\xE6\xC7\xE8\xC4\xA1\xB8\xA6 \xBE\xF2\xBE\xEE\xBC\xAD, %s\xB7\xCE \xB7\xB9\xBA\xA7\xBE\xF7 \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", exp, data_a291c0[level].name);
		} else {
			sprintf(msg, "%d\xC0\xC7 \xB0\xE6\xC7\xE8\xC4\xA1\xB8\xA6 \xBE\xF2\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", exp);
		}

		CSharedDoc::Instance()->m_myLevel = level;

		PassMsgToActor("Family", MsgObject(0, 0x23, (intptr_t)msg, 0, 0, 0, 0));
		break;
	}
	default:
		return OnPacketCommon(packet);
	}
	return 1;
}
