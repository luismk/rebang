// Original: d:\build\custom\temp\code\client\projectg\netsyncactor.h
#ifndef PG_CLIENT_PROJECTG_NETSYNCACTOR_H
#define PG_CLIENT_PROJECTG_NETSYNCACTOR_H

#include "..\..\defines.h"
#include "actor.h"
#include "mtrand1p1.h"

class MsgObject;

// Receiver of the network-synchronised step (vtable ??_7NetSyncable@@6B@:
// three pure virtuals, no destructor). The names are those of GroundItemMan's
// overrides.
class NetSyncable {
public:
	virtual void NSC_OnStep(bool active, bool replay, float elapsed, float syncTime, int frame) = 0;
	virtual void NSC_OnFirstNewShotFrame(float startTime, int frame) = 0;
	virtual void NSC_OnFirstReplayFrame(int frame) = 0;
};

CHECK_SIZE(NetSyncable, 0x4);

// An actor whose time is driven by NetSyncActor::NSA_Step and that owns a
// random generator shared with the other clients through its seed.
class NetSyncActor : public IActor, public NetSyncable {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;

	NetSyncActor(void);
	virtual ~NetSyncActor(void);
	// netsyncactor.h:29
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	virtual void OnInit(void);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject &msg);
	virtual void NSA_Step(bool active, float elapsed, int frame);
	virtual void OnHoleOut(void);

	// netsyncactor.h:53
	MTRand1p1& NSA_GetRandomGenerator(void) { return m_random; }

protected:
	// netsyncactor.h:62
	void NSA_SetInitialRandomSeed(unsigned long seed)
	{
		m_initialRandomSeed = seed;
	}

	float m_syncTime;                   // 0x1c, time of the current step
	float m_startTime;                  // 0x20, time of frame 0 of the shot
	float m_receivedStartTime;          // 0x24, start time agreed over the network
	// 0x28: [0] the frame of the previous step, [1] the highest frame
	// stepped so far (an array: the constructor fills it in a loop).
	int m_frame[2];
	MTRand1p1 m_random;                 // 0x30
	bool m_wasActive;                   // 0x9f8, active flag of the previous step
	bool m_replay;                      // 0x9f9, the current step replays known frames
	unsigned long m_initialRandomSeed;  // 0x9fc
};

CHECK_SIZE(NetSyncActor, 0xa00);
BEGIN_LAYOUT_CHECK(NetSyncActor)
	CHECK_FIELD(NetSyncActor, m_syncTime, 0x1c);
	CHECK_FIELD(NetSyncActor, m_startTime, 0x20);
	CHECK_FIELD(NetSyncActor, m_receivedStartTime, 0x24);
	CHECK_FIELD(NetSyncActor, m_frame, 0x28);
	CHECK_FIELD(NetSyncActor, m_random, 0x30);
	CHECK_FIELD(NetSyncActor, m_wasActive, 0x9f8);
	CHECK_FIELD(NetSyncActor, m_replay, 0x9f9);
	CHECK_FIELD(NetSyncActor, m_initialRandomSeed, 0x9fc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NETSYNCACTOR_H
