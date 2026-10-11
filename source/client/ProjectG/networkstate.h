// Original: d:\build\custom\temp\code\client\projectg\networkstate.h
#ifndef PG_CLIENT_PROJECTG_NETWORKSTATE_H
#define PG_CLIENT_PROJECTG_NETWORKSTATE_H

#include "..\..\defines.h"

// Abstract base of a network state: only a vtable, whose second slot is
// pure (_purecall in ??_7NetworkState@@6B@). No derived class is
// constructed in the exe, so the pure method's name is unknown.
class NetworkState {
public:
	NetworkState(void);
	virtual ~NetworkState(void);

	virtual void Process(void) = 0;
};

CHECK_SIZE(NetworkState, 0x4);

#endif // PG_CLIENT_PROJECTG_NETWORKSTATE_H
