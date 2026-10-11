// Original: d:\build\custom\temp\code\client\projectg\clock.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "clock.h"

#include <new>

#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "SoundManager.h"
#include "Packet.h"
#include "wview.h"

extern unsigned long g_CurrentTime;

// The tick sound's name and alias.
#define CLOCK_TICK_SFX "\xC5\xB8\xC0\xD3"

// clock.cpp:15 (run-time type and factory registration of CClock)
const WRTTI CClock::m_RTTI("CClock", &IActor::m_RTTI);

IObject* CClockMakeInstance(void) {
	return new CClock;
}

struct __impCClock {
	__impCClock(void) {
		ObjectFactory().AddObjectFunctor(CClockMakeInstance, "CClock");
		ActorFactory().AddActorClass("CClock");
	}
};
static __impCClock s_impCClock;

// clock.cpp:22
CClock::CClock(void) {
	AddSkipList(0x84400);
}

// clock.cpp:27
void CClock::OnInit(void) {
	Reset();
}

// clock.cpp:32
void CClock::OnProcess(float elapsed) {
	if (GOLFDOC()->m_pause || GOLFDOC()->m_clockStopped)
		return;

	if (GOLFDOC()->m_gameState < 15)
		return;

	GOLFDOC()->m_shotElapsed += (int)(elapsed * 1000.0f);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
		unsigned long startTime = WSingleton<CSharedDoc>::Instance()->m_gameStartTime;
		if (startTime > 0) {
			unsigned long endTime = startTime + GetGameTimeLimit();
			if (g_CurrentTime > endTime) {
				if (m_bTicking) {
					m_bTicking = false;
					g_audio->PlaySfx(CLOCK_TICK_SFX, 0, 1, CLOCK_TICK_SFX, 0, 0.5f, 200.0f);

					g_audio->PlaySfx("approach_bell", 0, 0, 0, 0, 0.5f, 200.0f);
				}
			} else if (g_CurrentTime > endTime - 10000) {
				if (!m_bTicking) {
					m_bTicking = true;
					g_audio->PlaySfx(CLOCK_TICK_SFX, 0, 1, CLOCK_TICK_SFX, 0, 0.5f, 200.0f);
				}
			}
		}
	} else if (m_bTicking && GetShotTimeLimit()) {
		unsigned long elapsedMs = (unsigned long)(elapsed * 1000.0f);
		CGolfDoc *golfDoc = GOLFDOC();
		if (golfDoc->m_remainShotTime > elapsedMs)
			golfDoc->m_remainShotTime -= elapsedMs;
		else
			golfDoc->m_remainShotTime = 0;

		m_angle = ((1.0f - (float)GOLFDOC()->m_remainShotTime / (float)GetShotTimeLimit()) * 278.0f + 12.0f) * 0.017453292f;

		if (GOLFDOC()->m_remainShotTime < 10000) {
			if (!GOLFDOC()->m_clockHold && !g_audio->IsPlaying(CLOCK_TICK_SFX))
				g_audio->PlaySfx(CLOCK_TICK_SFX, 0, 1, CLOCK_TICK_SFX, 0, 0.5f, 200.0f);
		}

		unsigned long remain = GOLFDOC()->m_remainShotTime;
		if (remain > 20000) {
			m_color = 0xFF147EFF;
		} else if (remain > 15000) {
			m_color = ULblend(0xFF147EFF, 0xFFF014FF, (unsigned char)((20000 - remain) * 255 / 5000));
		} else if (remain > 10000) {
			m_color = 0xFFF014FF;
		} else if (remain > 5000) {
			m_color = ULblend(0xFFF014FF, 0xFFFF1450, (unsigned char)((10000 - remain) * 255 / 5000));
		} else {
			m_color = 0xFFFF1450;
		}

		SendMsg(this, "Screen", 0x1A9, m_color, (intptr_t)&m_angle, 0, 0);

		if (GOLFDOC()->m_clockHold <= 0) {
			if (GOLFDOC()->m_remainShotTime == 0) {
				m_bTicking = false;

				if (!OnlinePlay()) {
					Reset();
					SendMsg(this, "GolfRule", 0x78, 0x4000, 0, 0, 0);
				}
			}
		}
	}
}

// clock.cpp:129
void CClock::Reset(void) {
	GOLFDOC()->m_remainShotTime = GetShotTimeLimit();
	GOLFDOC()->m_shotElapsed = 0;
	m_angle = 12.0f * 0.017453292f;
	m_color = 0xFF147EFF;
	m_bTicking = false;
	SendMsg(this, "Screen", 0x1A9, 0xFF147EFF, (intptr_t)&m_angle, 0, 0);

	g_audio->StopSfx(CLOCK_TICK_SFX);
}

// clock.cpp:142
void CClock::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x271A:
		Reset();
		break;

	case 0x1E9:
		m_bTicking = true;
		if (OnlinePlay() && GetShotTimeLimit() && !WSingleton<CSharedDoc>::Instance()->m_userInfo.IsIdentity(2) && IsMyTurn(true)) {
			WSendPacket packet((enumClientPacket)0x22);
			packet.Send(SEND_TO_DEFAULT);
		}
		break;

	case 0x1EA:
		m_bTicking = false;
		break;
	}
}
