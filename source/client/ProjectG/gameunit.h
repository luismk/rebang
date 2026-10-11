// Original: d:\build\custom\temp\code\client\projectg\gameunit.h
#ifndef PG_CLIENT_PROJECTG_GAMEUNIT_H
#define PG_CLIENT_PROJECTG_GAMEUNIT_H

#include "..\..\defines.h"
#include "NetworkUnit.h"

class WReceivedPacket;

// The connection to the game server (size 0x8C from WNetworkSystem's new).
class GameUnit : public NetworkUnit {
public:
	GameUnit(void);
	virtual ~GameUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void);
	virtual bool OnLine(void);
	virtual bool OffLine(void);
	virtual bool Connect(void);
	virtual bool Connecting(void);
	virtual bool Disconnecting(void);

private:
	int OnConnect(WReceivedPacket &packet);

	unsigned long m_connectTime;    // 0x88, GetTickCount when Connect started; Connecting gives up after 30000 ms
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(GameUnit, 0x8C);
BEGIN_LAYOUT_CHECK(GameUnit)
	CHECK_FIELD(GameUnit, m_connectTime, 0x88);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GAMEUNIT_H
