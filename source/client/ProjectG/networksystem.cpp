// Original: d:\build\custom\temp\code\client\projectg\networksystem.cpp
#include "GameUnit.h"
#include <string.h>
#include "NetworkSystem.h"
#include "MSNUnit.h"
#include "LoginUnit.h"
#include "RankUnit.h"
#include "Packet.h"

// networksystem.cpp:19
WNetworkSystem::WNetworkSystem(void) {
	m_units[NET_UNIT_DEFAULT] = new GameUnit;
	m_units[NET_UNIT_MSN] = new MSNUnit;

	m_units[NET_UNIT_LOGIN] = new LoginUnit;
	m_units[NET_UNIT_RANK] = new RankUnit;
	memset(m_parseKey, 0, sizeof(m_parseKey));
	m_connectionType = 0;
}

// networksystem.cpp:31
WNetworkSystem::~WNetworkSystem(void) {
	for (int i = 0; i < NET_UNIT_ALL; i++) {
		delete m_units[i];
	}
}

// networksystem.cpp:41
int WNetworkSystem::Init(eNetUnit unit) {
	if (unit == NET_UNIT_ALL) {
		for (int i = 0; i < NET_UNIT_ALL; i++) {
			m_units[i]->Init();
		}
	} else {
		m_units[unit]->Init();
	}
	return 1;
}

// networksystem.cpp:57
int WNetworkSystem::Process(void) {
	for (int i = 0; i < NET_UNIT_ALL; i++) {
		m_units[i]->Process();
	}
	return 1;
}

// networksystem.cpp:76
int WNetworkSystem::SendMessage(eNetUnit unit, NetworkUnit::eEvent event) {
	m_units[unit]->setTransition(event);
	return 1;
}

// networksystem.cpp:82
void WNetworkSystem::ForceShutDown(eNetUnit unit) {
	m_units[unit]->ShutDown();
}

// networksystem.cpp:88
void WNetworkSystem::WriteSendTime(void) {
	m_sendTime = GetTickCount();
}

// networksystem.cpp:96
int WNetworkSystem::IsConnected(eNetUnit unit) {
	return m_units[unit]->IsConnected();
}

// networksystem.cpp:105
int WNetworkSystem::IsConnectionComplete(eNetUnit unit) {
	return m_units[unit]->IsConnectionComplete();
}

// networksystem.cpp:111
void WNetworkSystem::SendTTL(void) {
	DWORD now = GetTickCount();

	if (now - m_sendTime > 5000) {
		WSendPacket packet((enumClientPacket)1);
		packet.Send(SEND_TO_DEFAULT);
	}
}
