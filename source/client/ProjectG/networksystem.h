// Original: d:\build\custom\temp\code\client\projectg\networksystem.h
#ifndef PG_CLIENT_PROJECTG_NETWORKSYSTEM_H
#define PG_CLIENT_PROJECTG_NETWORKSYSTEM_H

#include "..\..\defines.h"
#include "singleton.h"
#include "NetworkUnit.h"

// The client's connection manager singleton: owns one NetworkUnit per
// server connection (created by the constructor, in eNetUnit order).
class WNetworkSystem : public WSingleton<WNetworkSystem> {
public:
	// The connection a request is about; the index into m_units.
	enum eNetUnit {
		NET_UNIT_DEFAULT = 0,           // GameUnit
		NET_UNIT_MSN = 1,               // MSNUnit
		NET_UNIT_LOGIN = 2,             // LoginUnit
		NET_UNIT_RANK = 3,              // RankUnit
		NET_UNIT_ALL = 4                // Init: every unit; also the unit count
	};

	WNetworkSystem(void);
	virtual ~WNetworkSystem(void);

	int Init(eNetUnit unit);
	int Process(void);
	int SendMessage(eNetUnit unit, NetworkUnit::eEvent event);
	void ForceShutDown(eNetUnit unit);
	void WriteSendTime(void);
	int IsConnected(eNetUnit unit);
	int IsConnectionComplete(eNetUnit unit);
	void SendTTL(void);

	// networksystem.h:71
	WClientSocket *GetSocket(eNetUnit unit) {
		return m_units[unit]->GetSocket();
	}

	// networksystem.h:74
	bool IsGetParseKey(eNetUnit unit) {
		if (unit < NET_UNIT_ALL) {
			return m_parseKey[unit];
		}
		return false;
	}

	// networksystem.h:83
	void SetParseKey(eNetUnit unit, bool set) {
		if (unit < NET_UNIT_ALL) {
			m_parseKey[unit] = set;
		}
	}

	int GetConnectionType(void) const;  // networksystem.h:90, emitted by lobbymain.cpp

private:
	unsigned long m_sendTime;           // 0x28, GetTickCount of the last send (SendTTL)
	bool m_parseKey[NET_UNIT_ALL];      // 0x2C, per unit: the server sent its parse key
	NetworkUnit *m_units[NET_UNIT_ALL]; // 0x30
	int m_connectionType;               // 0x40

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(WNetworkSystem, 0x44);
BEGIN_LAYOUT_CHECK(WNetworkSystem)
	CHECK_FIELD(WNetworkSystem, m_sendTime, 0x28);
	CHECK_FIELD(WNetworkSystem, m_parseKey, 0x2C);
	CHECK_FIELD(WNetworkSystem, m_units, 0x30);
	CHECK_FIELD(WNetworkSystem, m_connectionType, 0x40);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NETWORKSYSTEM_H
