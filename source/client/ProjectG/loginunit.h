// Original: d:\build\custom\temp\code\client\projectg\loginunit.h
#ifndef PG_CLIENT_PROJECTG_LOGINUNIT_H
#define PG_CLIENT_PROJECTG_LOGINUNIT_H

#include "..\..\defines.h"
#include "NetworkUnit.h"

class WReceivedPacket;

// The connection to the login server.
class LoginUnit : public NetworkUnit {
public:
	LoginUnit(void);
	virtual ~LoginUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void);
	virtual bool OnLine(void);
	virtual bool OffLine(void);
	virtual bool Connect(void);
	virtual bool Connecting(void);
	virtual bool Disconnecting(void);

private:
	int OnConnect(WReceivedPacket &packet);

	unsigned long m_connectTime;    // 0x88, GetTickCount() when Connect started
	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(LoginUnit)
	CHECK_FIELD(LoginUnit, m_connectTime, 0x88);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LOGINUNIT_H
