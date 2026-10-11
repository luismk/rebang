// Original: d:\build\custom\temp\code\client\projectg\ghosttask.cpp
#include "GhostTask.h"

#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "Packet.h"

extern int g_iRecvPacketType;

// ghosttask.cpp:9 (run-time type and factory registration of CGhostTask)
const WRTTI CGhostTask::m_RTTI("CGhostTask", &CTask::m_RTTI);

IObject* CGhostTaskMakeInstance(void) {
	return new CGhostTask;
}

struct __sCGhostTask {
	__sCGhostTask(void) {
		ObjectFactory().AddObjectFunctor(CGhostTaskMakeInstance, "CGhostTask");
	}
};
static __sCGhostTask s_registerCGhostTask;

// ghosttask.cpp:15
CGhostTask::CGhostTask(void) {
	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
}

// ghosttask.cpp:21
void CGhostTask::Load(void) {
	SetProgressBar(false);
	SetTip(false);
	CTask::Load();
}

// ghosttask.cpp:33
void CGhostTask::Init(const char *backGround) {
	CTask::Init("ghost_main_bg.jpg");
	SetWhisper(false);
}

// ghosttask.cpp:44
void CGhostTask::Register(void) {
	SetMainActor(AddActor("CGhostMain", "Ghost", 0, ""));
}

// ghosttask.cpp:49
void CGhostTask::Process(float dt) {
	CTask::Process(dt);
	g_pFresh->OnProcess(dt);
}

// ghosttask.cpp:55
void CGhostTask::Display(void) {
	CTask::Display();
	g_pFresh->OnDisplay(true);
}

// ghosttask.cpp:61
int CGhostTask::OnPacket(WReceivedPacket &packet) {
	unsigned short type = packet.Decode2();

	g_iRecvPacketType = type;

	switch (g_iRecvPacketType) {
	case 0x132:
		packet.DecodeStr();
		packet.Decode1();
		break;
	default:
		return OnPacketCommon(packet);
	}
	return 1;
}
