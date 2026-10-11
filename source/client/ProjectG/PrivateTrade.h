// Original: d:\build\custom\temp\code\client\projectg\privatetrade.h
#ifndef PG_CLIENT_PROJECTG_PRIVATETRADE_H
#define PG_CLIENT_PROJECTG_PRIVATETRADE_H

#include "..\..\defines.h"
#include "actor.h"

class CPrivateTradeImpl;
class MsgObject;
class WReceivedPacket;

// The private-trade actor (size 0x1c, the `new` of CPrivateTradeMakeInstance):
// an IActor whose work is done by a CPrivateTradeImpl it owns.
class CPrivateTrade : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	CPrivateTrade(void);
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // privatetrade.h:27
	virtual ~CPrivateTrade(void);

	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnProcess(float);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);

	void OnPacket(WReceivedPacket* pPacket);

protected:
	virtual void HandleMsg(const MsgObject& msg);

private:
	CPrivateTradeImpl* m_impl;     // 0x18
};
CHECK_SIZE(CPrivateTrade, 0x1c);
BEGIN_LAYOUT_CHECK(CPrivateTrade)
	CHECK_FIELD(CPrivateTrade, m_impl, 0x18);
END_LAYOUT_CHECK

#endif
