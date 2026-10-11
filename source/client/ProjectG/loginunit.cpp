// Original: d:\build\custom\temp\code\client\projectg\loginunit.cpp
#include "LoginUnit.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "logininfo.h"
#include "ProjectG.h"

#include <stdlib.h>
#include <string>

#ifdef PG_OFFLINE
#include "OfflineServer.h"
#endif

// The login server's addresses and ports (header unknown).
extern const char *LSIP_ADDRESS[];
extern int LOGIN_PORT_LIST[];

// loginunit.cpp:100
LoginUnit::LoginUnit(void) {
}

// loginunit.cpp:110
LoginUnit::~LoginUnit(void) {
}

// loginunit.cpp:117
bool LoginUnit::Init(void) {
	NetworkUnit::Init();
	return true;
}

// loginunit.cpp:128
void LoginUnit::ShutDown(void) {
	Init();
}

// loginunit.cpp:136
bool LoginUnit::OnLine(void) {
	WReceivedPacket packet;

	if (m_socket.GetPacket(packet) == 3) {
		WSingleton<CProjectG>::Instance()->OnPacket(packet);
	}
	return true;
}

// loginunit.cpp:161
bool LoginUnit::OffLine(void) {
	return true;
}

// loginunit.cpp:170
bool LoginUnit::Connect(void) {
	if (m_socket.m_bConnected) {
		setForceTransition(0);
	} else {
		std::string address;
#ifdef PG_OFFLINE
		// The offline server of the CMake build (CMake PG_OFFLINE) stands in
		// for Ntreev's login servers.
		address = "127.0.0.1";
		int port = PgOfflineLoginPort();
#else
		address = LSIP_ADDRESS[rand() % 1];
		int port = LOGIN_PORT_LIST[rand() % 1];
#endif
		char ip[21] = "\0";
		m_socket.m_asyncMessage = 0x40D;
		hostent *host = gethostbyname(address.c_str());
		if (host != NULL) {
			wsprintf(ip, "%s", inet_ntoa(*(in_addr *)host->h_addr_list[0]));
		}
		if (host == NULL || !m_socket.Connect(inet_addr(ip), htons(port))) {
			setForceTransition(1);
			m_connectResult = 1;
			return false;
		}
		m_connectTime = GetTickCount();
		setTransition(0);
	}
	return true;
}

// loginunit.cpp:261
bool LoginUnit::Connecting(void) {
	if (GetTickCount() - m_connectTime > 3000 || m_socket.TryRead() == 0) {
		setTransition(1);
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

// loginunit.cpp:292
bool LoginUnit::Disconnecting(void) {
	if (m_socket.m_bConnected == 0) {
		setForceTransition(1);
	} else {
		m_socket.Close();
	}
	return true;
}

// loginunit.cpp:307
int LoginUnit::OnConnect(WReceivedPacket &packet) {
	packet.Decode1();

	unsigned short result = packet.Decode2();
	if (result != 0) {
		return 0;
	}

	int parseKey = packet.Decode4();
	unsigned long connectValue = packet.Decode4();
	WSingleton<CSharedDoc>::Instance()->m_loginConnectValue = connectValue;
	m_socket.SetParseKey(parseKey);

	WSendPacket sendPacket(1);
	sendPacket.EncodeStr(CLoginInfo::Instance()->Id());

	unsigned long provType = CLoginInfo::Instance()->ProvType();

	sendPacket.EncodeStr(CLoginInfo::Instance()->Passwd());

	sendPacket.Encode4(provType);
	sendPacket.Encode1(CLoginInfo::Instance()->IsWebLogin() != 0);
	sendPacket.Encode4(CLoginInfo::Instance()->AuthUid());
	switch (provType) {
	case 2:
		sendPacket.Encode8(0x7FFFFFFFFFFFFFFFi64);
		break;
	case 4: {
		NhnLogin *nhnLogin = (NhnLogin *)CLoginInfo::Instance()->QueryInterface();
		sendPacket.Encode8(nhnLogin->GetNHNMemberNumber());
		break;
	}
	default:
		return 0;
	}

	sendPacket.Encode1(CLoginInfo::Instance()->IsPcBang());

	sendPacket.Send((eSendTo)3);

	return 1;
}
