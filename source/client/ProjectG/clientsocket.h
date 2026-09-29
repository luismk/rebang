#pragma once

#include "packet.h"
#include "socket.h"

class WClientSocket : public WSocket
{
public:
	WClientSocket(unsigned int sock, sockaddr_in addr);
	virtual ~WClientSocket();

	virtual void Close();

	int TryRead();
	int GetPacket(WReceivedPacket& packet);
	int PutPacket(WSendPacket& packet);
	int SendPacket(WSendPacket& packet);
	void Flush();
	void AsyncSelect(long events);
	void SetEventMsg(unsigned int msg) { m_eventMsg = msg; }
	void SetPacketHeaderType(int type);
	void SetParseKey(int key) { m_parseKey = key; }
	int GetRcvCnt() const { return m_rcvCnt; }

protected:
	WMemBlock m_rcvBuf;
	WMemBlock m_sndBuf;
	int m_rcvCnt;
	int m_sndCnt;
	int m_packetHeaderType;
	int m_sendSeq;
	int m_parseKey;
	unsigned int m_eventMsg;
	CRITICAL_SECTION m_cs;
};
