// Original: d:\build\custom\temp\code\client\projectg\rankunit.cpp
#include "RankUnit.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "ProjectG.h"

// rankunit.cpp:20
RankUnit::RankUnit(void) {
}

// rankunit.cpp:25
RankUnit::~RankUnit(void) {
}

// rankunit.cpp:30
bool RankUnit::Init(void) {
	NetworkUnit::Init();
	return true;
}

// rankunit.cpp:36
void RankUnit::ShutDown(void) {
	Init();
}

// rankunit.cpp:41
bool RankUnit::OnLine(void) {
	WReceivedPacket packet;

	if (m_socket.GetPacket(packet) == 3) {
		packet.Reset();
		WSingleton<CProjectG>::Instance()->OnPacket(packet);
	}
	return true;
}

// rankunit.cpp:61
bool RankUnit::OffLine(void) {
	return true;
}

// rankunit.cpp:66
bool RankUnit::Connect(void) {
	if (m_socket.m_bConnected) {
		setForceTransition(0);
	} else {
		m_socket.m_asyncMessage = 0x40E;
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		if (m_socket.Connect(inet_addr(doc->m_rankServerAddress.c_str()), htons(doc->m_rankServerPort)) == 0) {
			setForceTransition(1);
			m_connectResult = 1;
			return false;
		}
		m_connectTick = GetTickCount();
		setTransition(0);
	}
	return true;
}

// rankunit.cpp:104
bool RankUnit::Connecting(void) {
	if (GetTickCount() - m_connectTick > 3000 || m_socket.TryRead() == 0) {
		setForceTransition(1);
		GetSocket()->Close();
		m_connectResult = 1;
		return false;
	}

	WReceivedPacket packet;
	if (m_socket.GetPacket(packet) == 3) {
		if (OnConnect(packet)) {
			m_socket.AsyncSelect(0x23);
			setForceTransition(0);
			m_connectResult = 1;
			return true;
		}
		return false;
	}
	return true;
}

// rankunit.cpp:140
bool RankUnit::Disconnecting(void) {
	if (m_socket.m_bConnected == 0) {
		setForceTransition(1);
	} else {
		GetSocket()->Close();
	}
	return true;
}

// rankunit.cpp:153
int RankUnit::OnConnect(WReceivedPacket &packet) {
	packet.Decode1();
	unsigned short result = packet.Decode2();

	if (result != 500) {
		return 0;
	}

	m_socket.SetParseKey(packet.Decode4());
	if (packet.Decode1() != 5) {
		return 0;
	}

	std::string message = packet.DecodeStr();
	return 1;
}
