// Original: d:\build\custom\temp\code\client\projectg\netsyncactor.cpp
#include "netsyncactor.h"
#include "club.h"
#include "Packet.h"
#include "SharedDoc.h"

// netsyncactor.cpp:10
const WRTTI NetSyncActor::m_RTTI("NetSyncActor", &IActor::m_RTTI);

// netsyncactor.cpp:16
NetSyncActor::NetSyncActor(void)
	: m_wasActive(false), m_replay(false), m_initialRandomSeed(0) {
	m_syncTime = 0.0f;
	m_startTime = 0.0f;
	m_receivedStartTime = 0.0f;
	for (int i = 0; i < 2; ++i) {
		m_frame[i] = -1;
	}
	m_frame[0] = 999999999;
}

// netsyncactor.cpp:49
NetSyncActor::~NetSyncActor(void) {
}

// netsyncactor.cpp:55
void NetSyncActor::OnInit(void) {
}

// netsyncactor.cpp:61
void NetSyncActor::OnDestroy(void) {
}

// netsyncactor.cpp:66
void NetSyncActor::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0x278: {
		// The player answers with its time into the shot; the packet in
		// m_param1 carries the start time.
		float playerTime;
		SendMsg(this, "Player", 0x126, (intptr_t)&playerTime, 0, 0, 0);
		float startTime = playerTime + m_syncTime;

		if (GolfClub().PowerShotType() == 0) {
			startTime += 1.0f;
		}

		((WSendPacket *)msg.m_param1)->Encode4(*(unsigned long *)&startTime);

		if (!OnlinePlay()) {
			m_receivedStartTime = startTime;
		}
		break;
	}
	case 0x279: {
		unsigned long startTime = ((WReceivedPacket *)msg.m_param1)->Decode4();
		m_receivedStartTime = *(float *)&startTime;
		break;
	}
	case 0x27A:
		OnHoleOut();
		break;
	}
}

// netsyncactor.cpp:107
void NetSyncActor::NSA_Step(bool active, float elapsed, int frame) {
	if (m_frame[0] == 0 && frame == 0) {
		return;
	}

	if (!m_wasActive) {
		if (active) {
			if (frame == 0) {
				if (m_frame[0] == -1) {
					m_startTime = m_receivedStartTime;
					NSC_OnFirstNewShotFrame(m_startTime, 0);
				} else {
					m_replay = true;
					NSC_OnFirstReplayFrame(0);
				}
			} else {
				m_replay = true;
				NSC_OnFirstReplayFrame(frame);
			}
		}
	} else {
		if (!active) {
			m_replay = false;
		} else if (frame <= m_frame[1]) {
			m_replay = true;
			if (frame != m_frame[0] + 1) {
				NSC_OnFirstReplayFrame(frame);
			}
		} else {
			m_replay = false;
		}
	}

	if (active) {
		m_syncTime = frame * elapsed + m_startTime;
	} else {
		m_syncTime += elapsed;
	}

	NSC_OnStep(active, m_replay, elapsed, m_syncTime, frame);

	m_wasActive = active;
	m_frame[0] = frame;
	if (frame == -1 || m_frame[1] < frame) {
		m_frame[1] = frame;
	}
}

// netsyncactor.cpp:319
void NetSyncActor::OnHoleOut(void) {
}
