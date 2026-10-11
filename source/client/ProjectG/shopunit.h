// Original: d:\build\custom\temp\code\client\projectg\shopunit.h
#ifndef PG_CLIENT_PROJECTG_SHOPUNIT_H
#define PG_CLIENT_PROJECTG_SHOPUNIT_H

#include "..\..\defines.h"
#include "NetworkUnit.h"

// The connection to the shop server (no members of its own).
class ShopUnit : public NetworkUnit {
public:
	ShopUnit(void);
	virtual ~ShopUnit(void);

	virtual bool Init(void);
	virtual void ShutDown(void);
	virtual bool OnLine(void);
	virtual bool OffLine(void);
	virtual bool Connect(void);
	virtual bool Connecting(void);
	virtual bool Disconnecting(void);

private:
	int OnConnect(void);
};

CHECK_SIZE(ShopUnit, 0x88);

#endif // PG_CLIENT_PROJECTG_SHOPUNIT_H
