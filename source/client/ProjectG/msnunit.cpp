// Original: d:\build\custom\temp\code\client\projectg\msnunit.cpp
#include "MSNUnit.h"
#include "Packet.h"
#include "ProjectG.h"
#include "NetworkSystem.h"
#include "messengerdlg.h"
#include "SharedDoc.h"

// msnunit.cpp:29
MSNUnit::MSNUnit(void) {
	m_connectTick = 0;
	m_serverListTick = 0;
}

// msnunit.cpp:36
MSNUnit::~MSNUnit(void) {
}

// msnunit.cpp:43
bool MSNUnit::Init(void) {
	m_connectStarted = false;

	WSingleton<CMessengerInfo>::Instance()->m_serverListRequest = 0;
	WSingleton<WNetworkSystem>::Instance()->SetParseKey(WNetworkSystem::NET_UNIT_MSN, false);

	NetworkUnit::Init();

	return true;
}

// msnunit.cpp:58
void MSNUnit::ShutDown(void) {
	m_socket.Close();

	Init();

	WSingleton<CMessengerInfo>::Instance()->m_serverList.clear();
}

// msnunit.cpp:73
bool MSNUnit::OnLine(void) {
	WReceivedPacket packet;

	if (m_socket.GetPacket(packet) == 3) {
		WSingleton<CProjectG>::Instance()->OnPacket(packet);
	}
	return true;
}

// msnunit.cpp:96
bool MSNUnit::OffLine(void) {
	if (WSingleton<CProjectG>::Instance()->m_loginDlgFlag != true) {
		m_connectTick = 0;
		return false;
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT) && WSingleton<WNetworkSystem>::Instance()->IsConnectionComplete(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (IsConnectionComplete() == 0 || GetTickCount() - m_connectTick > 60000) {
			Init();
			setTransition(0);
		}
	}

	return true;
}

// msnunit.cpp:141
bool MSNUnit::Connect(void) {
	if (m_socket.m_bConnected) {
		setForceTransition(0);
		m_connectResult = 1;
	} else if (!m_connectStarted) {
		ConnectToMSNServer();
	}
	return true;
}

// msnunit.cpp:186
bool MSNUnit::Connecting(void) {
	if (m_connectStarted) {
		m_connectingTick = GetTickCount();
		m_connectStarted = false;
	}

	if (GetTickCount() - m_connectingTick > 10000 || m_socket.TryRead() == 0) {
		setForceTransition(5);
		GetSocket()->Close();
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

// msnunit.cpp:239
int MSNUnit::OnConnect(WReceivedPacket &packet) {
	packet.Decode1();

	if (packet.Decode2() != 0x2C) {
		return 0;
	}

	WSingleton<CMessengerInfo>::Instance()->m_serverListRequest = 1;

	packet.Decode1();
	packet.Decode1();

	int parseKey = packet.Decode4();

	m_socket.SetParseKey(parseKey);

	WSingleton<WNetworkSystem>::Instance()->SetParseKey(WNetworkSystem::NET_UNIT_MSN, true);

	std::string id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
	std::string server = WSingleton<CSharedDoc>::Instance()->m_loginServerAddress;

	WSendPacket send(0x12);
	send.Encode4(WSingleton<CSharedDoc>::Instance()->m_myUID);
	send.EncodeStr(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname);
	send.Send((eSendTo)1);

	return 1;
}

// msnunit.cpp:301
bool MSNUnit::Disconnecting(void) {
	if (m_socket.m_bConnected == 0) {
		setForceTransition(1);
	} else {
		GetSocket()->Close();
	}
	return true;
}

// msnunit.cpp:314
void MSNUnit::ConnectToMSNServer(void) {
	std::string address = "";
	int port = 0;

	if (!GetConnectServerInfo(&address, &port)) {
		RequestMSNServerList();
		setForceTransition(1);
		m_connectResult = 1;
		return;
	}

	m_socket.m_asyncMessage = 0x40C;
	m_socket.AsyncSelect(0x10);

	hostent *host = gethostbyname(address.c_str());
	char ip[21] = "\0";
	wsprintfA(ip, "%s", inet_ntoa(*(in_addr *)host->h_addr_list[0]));

	m_socket.Connect(inet_addr(ip), htons(port));

	m_connectStarted = true;
	m_connectTick = GetTickCount();
}

// msnunit.cpp:379
bool MSNUnit::GetConnectServerInfo(std::string *address, int *port) {
	int serverCount = WSingleton<CMessengerInfo>::Instance()->m_serverList.size();
	int minUsers = 100000;
	if (serverCount < 1) {
		return false;
	}

	int found = -1;
	int index = 0;
	for (std::list<MSNServerInfo>::iterator it = WSingleton<CMessengerInfo>::Instance()->m_serverList.begin();
			it != WSingleton<CMessengerInfo>::Instance()->m_serverList.end(); ++it, ++index) {
		if (it->m_tried) {
			continue;
		}

		sGameServerInfo info = *it;

		// A nearly full server is tried only one time in three.
		if (info.m_curUsers >= info.m_maxUsers - 150 && rand() % 3 != 0) {
			it->m_tried = true;
			continue;
		}

		if (minUsers > info.m_curUsers) {
			*address = std::string(info.m_address);
			*port = info.m_port;
			minUsers = info.m_curUsers;
			found = index;
		}
	}

	if (found == -1) {
		return false;
	}

	WSingleton<CMessengerInfo>::Instance()->SetCurrentServerIndex(found);

	return true;
}

// msnunit.cpp:463
bool MSNUnit::ResetConnect(void) {
	m_connectStarted = false;

	WSingleton<CMessengerInfo>::Instance()->InvalidateCurrentServer();

	setTransition(NetworkUnit::EVENT_NEXT);
	return true;
}

// msnunit.cpp:473
void MSNUnit::RequestMSNServerList(void) {
	if (WSingleton<WNetworkSystem>::Instance()->IsGetParseKey(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (!WSingleton<CMessengerInfo>::Instance()->IsWaitingForServerList()) {
			unsigned long tick = GetTickCount();

			if (tick - m_serverListTick > 30000) {
				WSingleton<CMessengerInfo>::Instance()->m_serverList.clear();

				WSendPacket packet((enumClientPacket)0x88);
				packet.Send(SEND_TO_DEFAULT);

				WSingleton<CMessengerInfo>::Instance()->m_waitingForServerList = true;

				m_serverListTick = tick;
			}
		}
	}
}
