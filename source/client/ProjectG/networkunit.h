// Original: d:\build\custom\temp\code\client\projectg\networkunit.h
#ifndef PG_CLIENT_PROJECTG_NETWORKUNIT_H
#define PG_CLIENT_PROJECTG_NETWORKUNIT_H

#include "..\..\defines.h"
#include "ClientSocket.h"
#include "FiniteStateMachine.h"

// The base of the client's connections (size 0x88 from its constructor).
// Vtable order from ??_7NetworkUnit@@6B@ (0xA15FE4) and RankUnit's.
class NetworkUnit {
public:
	// The inputs of the connection state machine (the transitions added by
	// the constructor; states: 0 online, 1 offline, 2 connect, 3 connecting,
	// 4 disconnecting, 5 reset).
	enum eEvent {
		EVENT_NEXT = 0,         // offline->connect->connecting->online, reset->connect
		EVENT_RECONNECT = 1,    // online->connect
		EVENT_DISCONNECT = 2,   // online->disconnecting->offline
		EVENT_RESET = 3         // connect->reset
	};

	NetworkUnit(void);
	virtual ~NetworkUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void) = 0;
	virtual bool OnLine(void) = 0;
	virtual bool OffLine(void) = 0;
	virtual bool Connect(void) = 0;
	virtual bool Connecting(void) = 0;
	virtual bool Disconnecting(void) = 0;
	virtual bool ResetConnect(void);
	virtual bool Process(void);

	void setTransition(unsigned long input);
	void setForceTransition(unsigned long input);
	unsigned long getState(void);
	int IsConnected(void);
	int IsConnectionComplete(void);

	// networkunit.h:52
	WClientSocket *GetSocket(void) { return &m_socket; }

protected:
	WClientSocket m_socket;         // 0x4
	FiniteStateMachine m_fsm;       // 0x70
	int m_connectResult;            // 0x84, 1 once a connection attempt ended
	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(NetworkUnit, 0x88);
BEGIN_LAYOUT_CHECK(NetworkUnit)
	CHECK_FIELD(NetworkUnit, m_socket, 0x4);
	CHECK_FIELD(NetworkUnit, m_fsm, 0x70);
	CHECK_FIELD(NetworkUnit, m_connectResult, 0x84);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NETWORKUNIT_H
