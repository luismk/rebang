// Original: d:\build\custom\temp\code\client\projectg\clientsocket.cpp
#include "ClientSocket.h"
#include "networkmonitor.h"
#include "Exception.h"

// The main window (?g_hwnd@@3PAUHWND__@@A); its header is not known yet.
extern HWND g_hwnd;

// The type of the last packet put (PutPacket) and of the one whose send
// failed (Flush); their header is not known yet.
extern int g_iSendPacketType;
extern int g_iSendFailType;

// clientsocket.cpp:26
WClientSocket::WClientSocket(unsigned int socket, sockaddr_in address)
	: WSocket(socket, address), m_recvBuffer(0x3FFFC), m_sendBuffer(0xC000) {
	m_recvLength = 0;
	m_sendLength = 0;
	m_packetHeaderType = 0;
	m_packetCounter = 0x28;
	m_parseKey = -1;
}

// clientsocket.cpp:35
WClientSocket::~WClientSocket(void) {
}

// clientsocket.cpp:39
void WClientSocket::Close(void) {
	WSocket::Close();
	WSocket::CreateSocket();

	m_recvLength = 0;
	m_sendLength = 0;
	m_packetHeaderType = 0;
	m_packetCounter = 0x28;
	m_parseKey = -1;

	m_recvBuffer.ResetBuffer();
}

// clientsocket.cpp:126
int WClientSocket::TryRead(void) {
	if (Poll(0)) {
		if (m_recvLength >= 0x3FFFC)
			throw WAppException("! socket receive buffer overflow, close connection");

		int read = Read(m_recvBuffer.LockBuffer(m_recvLength), 0x3FFFC - m_recvLength);

		if (read == -1) {
			if (WSAGetLastError() != WSAEWOULDBLOCK) {
				m_bConnected = 0;
				return 0;
			}
		} else if (read == 0) {
			m_bConnected = 0;
			return 0;
		} else {
			m_recvLength += read;
			if (m_recvLength < 0 || m_recvLength > 0x3FFFC)
				throw WAppException("! socket receive buffer overflow, close connection");
			return 2;
		}
	}

	return 1;
}

// clientsocket.cpp:165
int WClientSocket::GetPacket(WReceivedPacket &packet) {
	if (m_recvLength >= 4) {
		const unsigned char* buffer = m_recvBuffer.GetBuffer();

		// The header: the option byte, then the body length.
		int length = MAKEWORD(buffer[1], buffer[2]);

		int total = length + 3;
		if (total > m_recvLength)
			return 4;

		packet.SetRcvPacket(buffer + 3, length, (unsigned char)m_parseKey, buffer[0]);
		m_recvLength -= length + 3;

		if (m_recvLength > 0)
			memmove(m_recvBuffer.LockBuffer(0), m_recvBuffer.LockBuffer(length + 3), m_recvLength);

		CNetworkMonitor::Instance()->TraceInBytes(total);

		bool noParseKey = false;
		if (m_parseKey == -1)
			noParseKey = true;

		if (packet.IsValid(noParseKey))
			return 3;

		packet.Reset();
		m_recvLength = 0;
		return 0;
	}

	return 4;
}

// clientsocket.cpp:217
int WClientSocket::SendPacket(WSendPacket &packet) {
	m_lock.Enter();
	if (PutPacket(packet) == 0) {
		m_lock.Leave();
		return 0;
	}

	Flush();
	m_lock.Leave();
	return 1;
}

// clientsocket.cpp:231
int WClientSocket::PutPacket(WSendPacket &packet) {
	if (packet.GetPacketType() != 0xF6)
		g_iSendPacketType = packet.GetPacketType();

	packet.MakePacketComplete(m_parseKey);

	int length = (packet.GetBuffer()[2] << 8) + packet.GetBuffer()[1] + 4;
	if (m_sendLength + length > 0xC000)
		throw WAppException("! socket send buffer overflow");

	memmove(m_sendBuffer.LockBuffer(m_sendLength), packet.GetBuffer(), length);
	m_packetCounter += 3;
	m_sendLength += length;
	return 1;
}

// clientsocket.cpp:251
void WClientSocket::Flush(void) {
	int sent = 0;

	while (m_sendLength > 0) {
		int written = Write(m_sendBuffer.LockBuffer(sent), m_sendLength);

		if (written == -1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK) {
				memmove(m_sendBuffer.LockBuffer(0), m_sendBuffer.LockBuffer(sent), m_sendLength);
				return;
			}

			g_iSendFailType = g_iSendPacketType;
			m_sendLength = 0;
			return;
		}

		m_sendLength -= written;
		sent += written;
	}
}

// clientsocket.cpp:284
void WClientSocket::SetPacketHeaderType(int type) {
	m_packetHeaderType = type;
}

// clientsocket.cpp:289
void WClientSocket::AsyncSelect(long events) {
	WSAAsyncSelect(m_socket, g_hwnd, m_asyncMessage, events);
}
