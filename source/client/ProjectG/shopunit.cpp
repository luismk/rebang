// Original: d:\build\custom\temp\code\client\projectg\shopunit.cpp
#include "ShopUnit.h"
#include "Packet.h"
#include "SharedDoc.h"

// shopunit.cpp:24
ShopUnit::ShopUnit(void) {
}

// shopunit.cpp:29
ShopUnit::~ShopUnit(void) {
}

// shopunit.cpp:37
bool ShopUnit::Init(void) {
	NetworkUnit::Init();
	return true;
}

// shopunit.cpp:45
void ShopUnit::ShutDown(void) {
}

// shopunit.cpp:52
bool ShopUnit::OnLine(void) {
	return true;
}

// shopunit.cpp:59
bool ShopUnit::OffLine(void) {
	return true;
}

// shopunit.cpp:70
bool ShopUnit::Connect(void) {
	if (m_socket.m_bConnected) {
		setTransition(0);
	} else {
		m_socket.m_asyncMessage = 0x40B;
		if (m_socket.Connect(inet_addr("127.0.0.1"), htons(7777)) == 0) {
			m_connectResult = 1;
		} else if (OnConnect()) {
			m_socket.AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
			setTransition(0);
			return true;
		}
		return false;
	}
	return true;
}

// shopunit.cpp:117
bool ShopUnit::Connecting(void) {
	setTransition(0);
	m_connectResult = 1;
	return true;
}

// shopunit.cpp:127
bool ShopUnit::Disconnecting(void) {
	if (m_socket.m_bConnected) {
		GetSocket()->Close();
		setTransition(2);
	} else {
		setForceTransition(2);
	}
	return true;
}

// shopunit.cpp:147
int ShopUnit::OnConnect(void) {
	std::string id = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;

	WSendPacket packet(2);

	packet.Encode1(0);
	packet.EncodeStr(id);
	packet.EncodeStr(std::string("hohoho"));
	packet.Send((eSendTo)2);

	return 1;
}
