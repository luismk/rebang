// Original: d:\build\custom\temp\code\client\projectg\clientsocket.h
#ifndef PG_CLIENT_PROJECTG_CLIENTSOCKET_H
#define PG_CLIENT_PROJECTG_CLIENTSOCKET_H

#include "..\..\defines.h"
#include "Socket.h"
#include "Packet.h"
#include "CriticalSection.h"

// The client side of a game connection (size 0x6C from NetworkUnit's
// layout; members from the constructor and Close).
class WClientSocket : public WSocket {
public:
	WClientSocket(unsigned int socket = INVALID_SOCKET, sockaddr_in address = ms_addrEmpty);
	virtual ~WClientSocket(void);
	virtual void Close(void);

	// clientsocket.h:40
	void SetEventMsg(unsigned int message) { m_asyncMessage = message; }
	// clientsocket.h:50
	void SetParseKey(int key) { m_parseKey = key; }
	// clientsocket.h:60
	int GetRcvCnt(void) const { return m_recvLength; }

	void SetPacketHeaderType(int type);
	void AsyncSelect(long events);
	int TryRead(void);
	int GetPacket(WReceivedPacket &packet);
	int SendPacket(WSendPacket &packet);
	int PutPacket(WSendPacket &packet);
	void Flush(void);

	WMemBlock m_recvBuffer;         // 0x1C, 0x3FFFC bytes
	WMemBlock m_sendBuffer;         // 0x2C, 0xC000 bytes
	int m_recvLength;               // 0x3C, bytes waiting in m_recvBuffer (GetRcvCnt)
	int m_sendLength;               // 0x40, 0 on Close
	int m_packetHeaderType;         // 0x44, SetPacketHeaderType
	int m_packetCounter;            // 0x48, 0x28 on Close, +3 per PutPacket
	int m_parseKey;                 // 0x4C, SetParseKey; -1 until the server sends one
	unsigned int m_asyncMessage;    // 0x50, window message of WSAAsyncSelect
	CCriticalSection m_lock;        // 0x54, held by SendPacket
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(WClientSocket, 0x6C);
BEGIN_LAYOUT_CHECK(WClientSocket)
	CHECK_FIELD(WClientSocket, m_recvBuffer, 0x1C);
	CHECK_FIELD(WClientSocket, m_sendBuffer, 0x2C);
	CHECK_FIELD(WClientSocket, m_recvLength, 0x3C);
	CHECK_FIELD(WClientSocket, m_sendLength, 0x40);
	CHECK_FIELD(WClientSocket, m_packetHeaderType, 0x44);
	CHECK_FIELD(WClientSocket, m_packetCounter, 0x48);
	CHECK_FIELD(WClientSocket, m_parseKey, 0x4C);
	CHECK_FIELD(WClientSocket, m_asyncMessage, 0x50);
	CHECK_FIELD(WClientSocket, m_lock, 0x54);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CLIENTSOCKET_H
