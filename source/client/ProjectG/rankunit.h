// Original: d:\build\custom\temp\code\client\projectg\rankunit.h
#ifndef PG_CLIENT_PROJECTG_RANKUNIT_H
#define PG_CLIENT_PROJECTG_RANKUNIT_H

#include "..\..\defines.h"
#include "NetworkUnit.h"

class WReceivedPacket;

// The connection to the ranking server.
class RankUnit : public NetworkUnit {
public:
	RankUnit(void);
	virtual ~RankUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void);
	virtual bool OnLine(void);
	virtual bool OffLine(void);
	virtual bool Connect(void);
	virtual bool Connecting(void);
	virtual bool Disconnecting(void);

private:
	int OnConnect(WReceivedPacket &packet);

	unsigned long m_connectTick;    // 0x88, GetTickCount when Connect started; Connecting gives up after 3000 ms
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(RankUnit, 0x8C);
BEGIN_LAYOUT_CHECK(RankUnit)
	CHECK_FIELD(RankUnit, m_connectTick, 0x88);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RANKUNIT_H
