// Original: d:\build\custom\temp\code\client\projectg\gameunit.cpp
#include "GameUnit.h"
#include "Packet.h"
#include "ProjectG.h"
#include "NetworkSystem.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "logininfo.h"
#include "encryption.h"

// gameunit.cpp:35
GameUnit::GameUnit(void) {
}

// gameunit.cpp:39
GameUnit::~GameUnit(void) {
}

// gameunit.cpp:47
bool GameUnit::Init(void) {
	NetworkUnit::Init();

	WSingleton<WNetworkSystem>::Instance()->SetParseKey(WNetworkSystem::NET_UNIT_DEFAULT, false);

	return true;
}

// gameunit.cpp:62
void GameUnit::ShutDown(void) {
	WSingleton<CTaskManager>::Instance()->ReleaseWork();

	Init();
}

// gameunit.cpp:72
bool GameUnit::OnLine(void) {
	WReceivedPacket packet;

	DWORD start = timeGetTime();

	if (m_socket.GetPacket(packet) == 3) {
		do {
			WSingleton<CProjectG>::Instance()->OnPacket(packet);
		} while (timeGetTime() - start <= 1000 && m_socket.GetPacket(packet) == 3);
	}
	return true;
}

// gameunit.cpp:100
bool GameUnit::OffLine(void) {
	return true;
}

// gameunit.cpp:109
bool GameUnit::Connect(void) {
	if (m_socket.m_bConnected) {
		setForceTransition(0);
		return true;
	}

	std::string address = WSingleton<CSharedDoc>::Instance()->m_gameServerAddress;
	int port = WSingleton<CSharedDoc>::Instance()->m_gameServerPort;

	m_socket.SetEventMsg(0x40A);

	if (!m_socket.Connect(inet_addr(address.c_str()), htons(port))) {
		setForceTransition(1);
		m_connectResult = 1;

		WSingleton<WNetworkSystem>::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);

		if (IsLocalContent((localContentType_t)0x5A)) {
			WSingleton<CProjectG>::Instance()->SetMoveLoginServer(
				WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id,
				WSingleton<CSharedDoc>::Instance()->m_loginServerAddress.c_str());

			WSingleton<CTaskManager>::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"LOGIN", 0, 0, 0);
		}
		return false;
	}

	m_connectTime = GetTickCount();
	setTransition(0);
	return true;
}

// gameunit.cpp:154
bool GameUnit::Connecting(void) {
	if (GetTickCount() - m_connectTime > 30000 || m_socket.TryRead() == 0) {
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

// gameunit.cpp:193
bool GameUnit::Disconnecting(void) {
	if (m_socket.m_bConnected == 0) {
		setForceTransition(1);
	} else {
		m_socket.Close();
	}
	return true;
}

// gameunit.cpp:210
int GameUnit::OnConnect(WReceivedPacket &packet) {
	packet.Decode1();

	if (packet.Decode2() != 0x3D) {
		return 0;
	}

	WSingleton<CTaskManager>::Instance()->ReleaseWork();

	packet.Decode1();
	packet.Decode1();
	int parseKey = packet.Decode1();

	m_socket.SetParseKey(parseKey);

	WSingleton<WNetworkSystem>::Instance()->SetParseKey(WNetworkSystem::NET_UNIT_DEFAULT, true);

	std::string id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
	std::string server = WSingleton<CSharedDoc>::Instance()->m_loginServerAddress;

	unsigned long key = 0x77DEF61B;
	XorWithKey((char *)&key, sizeof(key));

	WSendPacket send((enumClientPacket)2);
	send.EncodeStr(id);
	send.Encode4(WSingleton<CSharedDoc>::Instance()->m_myUID);
	send.Encode4(CLoginInfo::Instance()->AuthUid());
	send.Encode2(0x6696);
	send.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetLoginAuthKey());
	send.EncodeStr(std::string("645.00"));
	send.Encode4(key);
	send.Encode4(CLoginInfo::Instance()->IsPcBang());
	send.Send(SEND_TO_DEFAULT);

	return 1;
}
