// Original: d:\build\custom\temp\code\client\projectg\msnunit.h
#ifndef PG_CLIENT_PROJECTG_MSNUNIT_H
#define PG_CLIENT_PROJECTG_MSNUNIT_H

#include <string>

#include "..\..\defines.h"
#include "NetworkUnit.h"

class WReceivedPacket;

// The connection to the messenger (MSN) server.
class MSNUnit : public NetworkUnit {
public:
	MSNUnit(void);
	virtual ~MSNUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void);
	virtual bool OnLine(void);
	virtual bool OffLine(void);
	virtual bool Connect(void);
	virtual bool Connecting(void);
	virtual bool Disconnecting(void);
	virtual bool ResetConnect(void);

protected:
	bool GetConnectServerInfo(std::string *address, int *port);
	void RequestMSNServerList(void);
	void ConnectToMSNServer(void);

private:
	int OnConnect(WReceivedPacket &packet);

	unsigned long m_connectingTick;     // 0x88, GetTickCount when Connecting first ran; it gives up after 10000 ms
	unsigned long m_serverListTick;     // 0x8C, GetTickCount of the last server list request (one per 30000 ms)
	unsigned long m_connectTick;        // 0x90, GetTickCount when ConnectToMSNServer started; OffLine reconnects after 60000 ms
	bool m_connectStarted;              // 0x94, set by ConnectToMSNServer, cleared once Connecting has started its timer
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(MSNUnit, 0x98);
BEGIN_LAYOUT_CHECK(MSNUnit)
	CHECK_FIELD(MSNUnit, m_connectingTick, 0x88);
	CHECK_FIELD(MSNUnit, m_serverListTick, 0x8C);
	CHECK_FIELD(MSNUnit, m_connectTick, 0x90);
	CHECK_FIELD(MSNUnit, m_connectStarted, 0x94);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MSNUNIT_H
