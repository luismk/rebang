// Original: d:\build\custom\temp\code\client\projectg\ghostplayer.cpp

#include "GhostPlayer.h"

#include <algorithm>
#include <iterator>
#include <vector>

#include "GolfDoc.h"
#include "ObjectFactory.h"
#include "ProjectG.h"
#include "Packet.h"
#include "GhostGameDlg.h"
#include "SequenceStateMachine.h"
#include "SharedDoc.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "actor.h"
#include "club.h"
#include "commonutil.h"
#include "fresh.h"
#include "golfball.h"
#include "ghosthandler.h"
#include "ghostmanager.h"
#include "ghostdocument.h"
#include "s5/Utilities.h"
#include "type_convert.h"
#include "wind.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"

namespace {

// ghostplayer.cpp:17
bool IsGhostTurn(void) {
	return GOLFDOC()->m_myPlayerIndex != 0;
}

// A state of the ghost replay: runs OnStateRun once per frame until it
// reports done, and stays current until m_timer runs out.
class CGhostStateUnit : public CStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:26
	virtual char const* GetClassNameA(void) const { return "CGhostStateUnit"; }
	// ghostplayer.cpp:28
	CGhostStateUnit(void) : m_elapsed(0.0f), m_timer(2.0f), m_timeLimit(2.0f), m_done(false) {}
	// ghostplayer.cpp:30
	virtual ~CGhostStateUnit(void) {}

	void SetTimer(float fTime);

	virtual void OnRender(void);

	virtual bool OnStateRun(void) = 0;

private:
	virtual void OnProcess(float fDelta);
	virtual bool OnFinished(void);

protected:
	float m_elapsed;   // 0xc
	float m_timer;     // 0x10
	float m_timeLimit; // 0x14
	bool m_done;       // 0x18
	float m_delta;     // 0x1c
};
CHECK_SIZE(CGhostStateUnit, 0x20);
BEGIN_LAYOUT_CHECK(CGhostStateUnit)
	CHECK_FIELD(CGhostStateUnit, m_elapsed, 0xc);
	CHECK_FIELD(CGhostStateUnit, m_timer, 0x10);
	CHECK_FIELD(CGhostStateUnit, m_timeLimit, 0x14);
	CHECK_FIELD(CGhostStateUnit, m_done, 0x18);
	CHECK_FIELD(CGhostStateUnit, m_delta, 0x1c);
END_LAYOUT_CHECK

// ghostplayer.cpp:32
void CGhostStateUnit::SetTimer(float fTime) {
	m_timer = m_timeLimit = fTime;
}

// ghostplayer.cpp:38
void CGhostStateUnit::OnRender(void) {}

// ghostplayer.cpp:46
void CGhostStateUnit::OnProcess(float fDelta) {
	m_delta = fDelta;
	if (!OnFinished() && !m_done) {
		m_elapsed += fDelta;
		m_done = OnStateRun();

		if (m_done) {
			OnPrintLog(MakeStr("Runned time: %.1f sec, Finish remain: %.1f sec",
				m_elapsed, m_timeLimit > 0.0f ? m_timeLimit - m_elapsed : 0.0f));
		}
	}

	m_timer -= fDelta;
}

// ghostplayer.cpp:64
bool CGhostStateUnit::OnFinished(void) {
	return m_timer <= 0.0f;
}

class CWindChangeState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:83
	virtual char const* GetClassNameA(void) const { return "CWindChangeState"; }
	// ghostplayer.cpp:85
	CWindChangeState(unsigned char power, unsigned char direction, unsigned char type, unsigned char special)
		: m_power(power), m_direction(direction), m_type(type), m_special(special) {}

	// ghostplayer.cpp:89
	virtual bool OnStateRun(void) {
		SetTimer(1.0f);
		Wind().Init(m_power, m_direction, m_type, m_special);

		return true;
	}

private:
	unsigned char m_power;     // 0x20
	unsigned char m_direction; // 0x21
	unsigned char m_type;      // 0x22
	unsigned char m_special;   // 0x23
};
CHECK_SIZE(CWindChangeState, 0x24);
BEGIN_LAYOUT_CHECK(CWindChangeState)
	CHECK_FIELD(CWindChangeState, m_power, 0x20);
	CHECK_FIELD(CWindChangeState, m_direction, 0x21);
	CHECK_FIELD(CWindChangeState, m_type, 0x22);
	CHECK_FIELD(CWindChangeState, m_special, 0x23);
END_LAYOUT_CHECK

class CWeatherChangeState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:110
	virtual char const* GetClassNameA(void) const { return "CWeatherChangeState"; }
	// ghostplayer.cpp:112
	CWeatherChangeState(unsigned char weather) : m_weather(weather) {}

	virtual bool OnStateRun(void);

private:
	unsigned char m_weather; // 0x20
};
CHECK_SIZE(CWeatherChangeState, 0x24);
BEGIN_LAYOUT_CHECK(CWeatherChangeState)
	CHECK_FIELD(CWeatherChangeState, m_weather, 0x20);
END_LAYOUT_CHECK

// ghostplayer.cpp:115
bool CWeatherChangeState::OnStateRun(void) {
	AfxGetTask()->PassMsgToActor("GolfBg", MsgObject(0, 0x142, m_weather, 1, 0, 0, 0));

	return true;
}

class CDirectionChangeState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:132
	virtual char const* GetClassNameA(void) const { return "CDirectionChangeState"; }
	// ghostplayer.cpp:134
	CDirectionChangeState(float direction) : m_direction(direction) {}

	virtual bool OnStateRun(void);

private:
	float m_direction; // 0x20
};
CHECK_SIZE(CDirectionChangeState, 0x24);
BEGIN_LAYOUT_CHECK(CDirectionChangeState)
	CHECK_FIELD(CDirectionChangeState, m_direction, 0x20);
END_LAYOUT_CHECK

// ghostplayer.cpp:136
bool CDirectionChangeState::OnStateRun(void) {
	SetTimer(3.0f);
	// The camera reads the direction through the address in param1.
	AfxGetTask()->PassMsgToActor("Camera", MsgObject(0, 0xF2, (intptr_t)&m_direction, 0, 0, 0, 0));

	return true;
}

class CSpecialWindState : public CGhostStateUnit {
public:
	// ghostplayer.cpp:154
	virtual char const* GetClassNameA(void) const { return "CSpecialWindState"; }
	// ghostplayer.cpp:156
	CSpecialWindState(void) {}

	// ghostplayer.cpp:158
	virtual bool OnStateRun(void) {
		return true;
	}
};
CHECK_SIZE(CSpecialWindState, 0x20);

class CClubChangeState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:173
	virtual char const* GetClassNameA(void) const { return "CClubChangeState"; }
	// ghostplayer.cpp:175
	CClubChangeState(unsigned char club) : m_club(club) {}

	// ghostplayer.cpp:177
	virtual bool OnStateRun(void);

private:
	unsigned char m_club; // 0x20
};
CHECK_SIZE(CClubChangeState, 0x24);
BEGIN_LAYOUT_CHECK(CClubChangeState)
	CHECK_FIELD(CClubChangeState, m_club, 0x20);
END_LAYOUT_CHECK

class CUseItemState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:197
	virtual char const* GetClassNameA(void) const { return "CUseItemState"; }
	// ghostplayer.cpp:199
	CUseItemState(unsigned long itemTypeId, unsigned long itemId) : m_itemTypeId(itemTypeId), m_itemId(itemId) {}

	virtual bool OnStateRun(void);

private:
	unsigned long m_itemTypeId; // 0x20
	unsigned long m_itemId;     // 0x24
};
CHECK_SIZE(CUseItemState, 0x28);
BEGIN_LAYOUT_CHECK(CUseItemState)
	CHECK_FIELD(CUseItemState, m_itemTypeId, 0x20);
	CHECK_FIELD(CUseItemState, m_itemId, 0x24);
END_LAYOUT_CHECK

// ghostplayer.cpp:177
bool CClubChangeState::OnStateRun(void) {
	if (GolfClub().GetIndex() != m_club) {
		GolfClub().SetClub(m_club);
		g_audio->PlaySfx("\xc5\xac\xb7\xb4\xb1\xb3\xc3\xbc", 0, 0, 0, 0, 0.5f, 200.0f);
	}

	return true;
}

// ghostplayer.cpp:201
bool CUseItemState::OnStateRun(void) {
	AfxGetTask()->PassMsgToActor("Item", MsgObject(0, 0x19F, -2, m_itemTypeId, m_itemId, 0, 0));

	return true;
}

class CPoweShotMotionState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:218
	virtual char const* GetClassNameA(void) const { return "CPoweShotMotionState"; }
	// ghostplayer.cpp:220
	CPoweShotMotionState(unsigned char powerShot) : m_powerShot(powerShot) {}

	virtual bool OnStateRun(void);

private:
	unsigned char m_powerShot; // 0x20
};
CHECK_SIZE(CPoweShotMotionState, 0x24);
BEGIN_LAYOUT_CHECK(CPoweShotMotionState)
	CHECK_FIELD(CPoweShotMotionState, m_powerShot, 0x20);
END_LAYOUT_CHECK

// ghostplayer.cpp:222
bool CPoweShotMotionState::OnStateRun(void) {
	if (m_powerShot) {
		AfxGetTask()->PassMsgToActor("Player", MsgObject(0, 0x11F, m_powerShot, 0, 0, 0, 0));
	}

	return true;
}

class CBallDropState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:240
	virtual char const* GetClassNameA(void) const { return "CBallDropState"; }
	// ghostplayer.cpp:242
	CBallDropState(float x, float y, float z) {
		m_x = x;
		m_y = y;
		m_z = z;
	}

	virtual bool OnStateRun(void);

private:
	float m_x; // 0x20
	float m_y; // 0x24
	float m_z; // 0x28
};
CHECK_SIZE(CBallDropState, 0x2C);
BEGIN_LAYOUT_CHECK(CBallDropState)
	CHECK_FIELD(CBallDropState, m_x, 0x20);
	CHECK_FIELD(CBallDropState, m_y, 0x24);
	CHECK_FIELD(CBallDropState, m_z, 0x28);
END_LAYOUT_CHECK

// ghostplayer.cpp:248
bool CBallDropState::OnStateRun(void) {
	SetTimer(2.5f);
	// The rule reads the position through the address in param1.
	AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x8D, (intptr_t)&m_x, 0, 0, 0, 0));

	return true;
}

class CTimeOutState : public CGhostStateUnit {
public:
	// ghostplayer.cpp:266
	virtual char const* GetClassNameA(void) const { return "CTimeOutState"; }
	// ghostplayer.cpp:268
	CTimeOutState(void) {}

	virtual bool OnStateRun(void);
};
CHECK_SIZE(CTimeOutState, 0x20);

// ghostplayer.cpp:270
bool CTimeOutState::OnStateRun(void) {
	SetTimer(3.0f);
	AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x7C, -2, 0, 0, 0, 0));

	return true;
}

class CReadyToShotState : public CGhostStateUnit {
public:
	// ghostplayer.cpp:285
	virtual char const* GetClassNameA(void) const { return "CReadyToShotState"; }
	// ghostplayer.cpp:287
	CReadyToShotState(void) {}

	// ghostplayer.cpp:289
	virtual bool OnStateRun(void) {
		SetTimer(0.0f);
		return true;
	}
};
CHECK_SIZE(CReadyToShotState, 0x20);

// Moves the ball to the recorded start of the shot when it lies elsewhere.
class CWarpEventState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:302
	virtual char const* GetClassNameA(void) const { return "CWarpEventState"; }
	// ghostplayer.cpp:306
	CWarpEventState(GHOST_SHOT_DATA const* pShot) : m_warp(false) {
		if (!(WAbsBySign(GolfBall().m_pos.x - pShot->m_ballPos[0]) < WMATH_EPSILON
			&& WAbsBySign(GolfBall().m_pos.y - pShot->m_ballPos[1]) < WMATH_EPSILON
			&& WAbsBySign(GolfBall().m_pos.z - pShot->m_ballPos[2]) < WMATH_EPSILON)) {
			m_warp = true;
			m_pos[0] = pShot->m_ballPos[0];
			m_pos[1] = pShot->m_ballPos[1];
			m_pos[2] = pShot->m_ballPos[2];
		}
	}

	virtual bool OnStateRun(void);

private:
	bool m_warp;      // 0x20
	float m_pos[3];   // 0x24
};
CHECK_SIZE(CWarpEventState, 0x30);
BEGIN_LAYOUT_CHECK(CWarpEventState)
	CHECK_FIELD(CWarpEventState, m_warp, 0x20);
	CHECK_FIELD(CWarpEventState, m_pos, 0x24);
END_LAYOUT_CHECK

// ghostplayer.cpp:318
bool CWarpEventState::OnStateRun(void) {
	SetTimer(0.0f);
	if (m_warp) {
		// The rule reads the position through the address in param1.
		AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x8E, (intptr_t)m_pos, 0, 0, 0, 0));
	}

	return true;
}

// The masks closing in at the start of the ghost's turn.
class CGhostTurnBeginAnimationState : public CGhostStateUnit {
	LAYOUT_CHECK_FRIEND;
public:
	// ghostplayer.cpp:340
	virtual char const* GetClassNameA(void) const { return "CGhostTurnBeginAnimationState"; }

	CGhostTurnBeginAnimationState(void);
	virtual ~CGhostTurnBeginAnimationState(void);

	virtual bool OnStateRun(void);
	virtual void OnRender(void);

protected:
	void SelectMaskType(void);
	bool RunAnimate(float fDelta);

private:
	int m_maskType;                  // 0x20 (chosen by SelectMaskType)
	WOverlay* m_pOverlay;            // 0x24
	std::vector<WRect> m_maskRects;  // 0x28
	std::vector<float> m_maskSpeeds; // 0x38
	float m_z;                       // 0x48
	float m_alpha;                   // 0x4c
	float m_animTime;                // 0x50
};
CHECK_SIZE(CGhostTurnBeginAnimationState, 0x54);
BEGIN_LAYOUT_CHECK(CGhostTurnBeginAnimationState)
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_maskType, 0x20);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_pOverlay, 0x24);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_maskRects, 0x28);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_maskSpeeds, 0x38);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_z, 0x48);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_alpha, 0x4c);
	CHECK_FIELD(CGhostTurnBeginAnimationState, m_animTime, 0x50);
END_LAYOUT_CHECK

// ghostplayer.cpp:361
CGhostTurnBeginAnimationState::CGhostTurnBeginAnimationState(void)
	: m_maskType(0), m_pOverlay(0), m_z(0.0f), m_alpha(255.0f), m_animTime(0.0f) {
	SetTimer(30.0f);
	m_pOverlay = g_resrcmng->GetOverlay("black_mask.jpg", 0);
	SelectMaskType();
}

// ghostplayer.cpp:370
CGhostTurnBeginAnimationState::~CGhostTurnBeginAnimationState(void) {
	if (g_resrcmng && m_pOverlay) { g_resrcmng->Release(m_pOverlay); m_pOverlay = 0; }
}

// ghostplayer.cpp:375
bool CGhostTurnBeginAnimationState::OnStateRun(void) {
	float fDelta = m_delta;
	if (RunAnimate(fDelta)) {
		SetTimer(0.0f);
		return true;
	}

	return false;
}

// ghostplayer.cpp:385
void CGhostTurnBeginAnimationState::OnRender(void) {
	std::vector<WRect>::iterator it = m_maskRects.begin();
	while (it != m_maskRects.end()) {
		m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), *it, 0, (unsigned char)m_alpha << 24, m_z, 0);
		++it;
	}
}

// ghostplayer.cpp:422
void CGhostTurnBeginAnimationState::SelectMaskType(void) {
	float width = g_view->GetWidth();
	float height = g_view->GetHeight();

	m_z = 0.0f;
	m_alpha = 255.0f;
	m_animTime = 0.0f;

	m_maskType = rand() % 7;
	if (m_maskType >= 7) {
		m_maskType = 0;
	}

	switch (m_maskType) {
	case 0:
		// Vertical bars with their own speed.
		m_maskSpeeds.reserve(20);
		for (int i = 0; i < 20; ++i) {
			m_maskSpeeds.push_back((rand() % 30 + 10) * 0.1f);
		}
	case 4:
	case 5:
		m_maskRects.reserve(20);
		for (int i = 0; i < 20; ++i) { float barWidth = width * 0.05f; m_maskRects.push_back(WRect(i * barWidth, 0.0f, barWidth, height)); }
		break;
	case 1:
		// Horizontal bars with their own speed.
		m_maskSpeeds.reserve(20);
		for (int i = 0; i < 20; ++i) {
			m_maskSpeeds.push_back((rand() % 30 + 10) * 0.1f);
		}
	case 3:
	case 6:
		m_maskRects.reserve(20);
		for (int i = 0; i < 20; ++i) { float barHeight = height * 0.05f; m_maskRects.push_back(WRect(0.0f, i * barHeight, width, barHeight)); }
		break;
	case 2:
		m_maskRects.push_back(WRect(0.0f, (width - height) * -0.5f, width, width));
		break;
	}
}

// The closing speed of the masks (nameless constant of the original).
extern "C" const float data_a04e60;

// ghostplayer.cpp:490
bool CGhostTurnBeginAnimationState::RunAnimate(float fDelta) {
	float width = g_view->GetWidth();
	float height = g_view->GetHeight();
	bool finished = false;

	switch (m_maskType) {
	case 5: {
		float move = width * 0.05f * fDelta * data_a04e60;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (it->w > 0.0f) { if ((it->w -= move) <= 0.0f) it->w = 0.0f; }
			else finished = true;
		}
		break;
	}
	case 4: {
		float move = width * 0.05f * fDelta * data_a04e60;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (it->x < width) { if ((it->x += move) >= width) it->x = width; }
			if (it->w > 0.0f) { if ((it->w -= move) <= 0.0f) it->w = 0.0f; }
			else finished = true;
		}
		break;
	}
	case 1: {
		if (m_alpha > 0.0f) { if ((m_alpha -= fDelta * 127.5f) <= 0.0f) m_alpha = 0.0f; }

		float move = width * fDelta * 0.5f;
		int closed = 0;
		int i = 0;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (i % 2) {
				if (it->x < width) { if ((it->x += move * m_maskSpeeds[i]) >= width) it->x = width; }
			}

			if (it->w > 0.0f) { if ((it->w -= move * m_maskSpeeds[i]) <= 0.0f) it->w = 0.0f; }
			else ++closed;

			++i;
		}

		if (closed == m_maskRects.size()) {
			finished = true;
		}
		break;
	}
	case 0: {
		if (m_alpha > 0.0f) { if ((m_alpha -= fDelta * 127.5f) <= 0.0f) m_alpha = 0.0f; }

		float move = height * fDelta * 0.5f;
		int closed = 0;
		int i = 0;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (i % 2) {
				if (it->y < height) { if ((it->y += move * m_maskSpeeds[i]) >= height) it->y = height; }
			}

			if (it->h > 0.0f) { if ((it->h -= move * m_maskSpeeds[i]) <= 0.0f) it->h = 0.0f; }
			else ++closed;

			++i;
		}

		if (closed == m_maskRects.size()) {
			finished = true;
		}
		break;
	}
	case 6: {
		float move = height * 0.05f * fDelta * data_a04e60;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (it->h > 0.0f) { if ((it->h -= move) <= 0.0f) it->h = 0.0f; }
			else finished = true;
		}
		break;
	}
	case 3: {
		float move = height * 0.05f * fDelta * data_a04e60;
		for (std::vector<WRect>::iterator it = m_maskRects.begin(); it != m_maskRects.end(); ++it) {
			if (it->y < height) { if ((it->y += move) >= height) it->y = height; }
			if (it->h > 0.0f) { if ((it->h -= move) <= 0.0f) it->h = 0.0f; }
			else finished = true;
		}
		break;
	}
	case 2: {
		if (m_alpha > 0.0f) {
			m_alpha -= fDelta * m_alpha * data_a04e60;
			if (m_alpha <= 0.0f) {
				m_alpha = 0.0f;
			}
		}

		m_z += fDelta * 12.566371f;

		std::vector<WRect>::iterator it = m_maskRects.begin();
		float move = width * fDelta * data_a04e60;
		if (it != m_maskRects.end()) {
			if (it->w > 0.0f) {
				it->x += move * 0.5f;
				it->y += move * 0.5f;
				it->w -= move;
				it->h -= move;
				if (it->w <= 0.0f) {
					finished = true;
				}
			}
		}
		break;
	}
	}

	return finished;
}

} // namespace

// ghostplayer.cpp:652
CGhostPlayer::cMovieStyleDisplay::cMovieStyleDisplay(void) {
	m_height = 0.0f;
	m_pOverlay = g_resrcmng->GetOverlay("black_mask.jpg", 0);
}

// ghostplayer.cpp:657
CGhostPlayer::cMovieStyleDisplay::~cMovieStyleDisplay(void) {
	if (g_resrcmng && m_pOverlay) { g_resrcmng->Release(m_pOverlay); m_pOverlay = 0; }
}

// ghostplayer.cpp:662
void CGhostPlayer::cMovieStyleDisplay::ResetHeight(void) {
	m_height = 0.0f;
}

// ghostplayer.cpp:667
void CGhostPlayer::cMovieStyleDisplay::Process(float fDelta) {
	float maxHeight = g_view->GetHeight() * (1.0f / 7.0f);
	if (m_height < maxHeight) {
		m_height += fDelta * maxHeight;
	}

	if (m_height > maxHeight) {
		m_height = maxHeight;
	}
}

// ghostplayer.cpp:677
void CGhostPlayer::cMovieStyleDisplay::Render(void) {
	float maxHeight = g_view->GetHeight() * (1.0f / 7.0f);

	unsigned long color = ((unsigned char)(m_height / maxHeight * 255.0f) << 24) | 0xFFFFFF;
	m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(0.0f, 0.0f, g_view->GetWidth(), m_height), 0, color, 0.0f, 0);

	m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(0.0f, g_view->GetHeight() - m_height, g_view->GetWidth(), m_height), 0, color, 0.0f, 0);
}

// ghostplayer.cpp:702
IObject* CGhostPlayerMakeInstance(void) {
	return new CGhostPlayer;
}

// ghostplayer.cpp:702 (run-time type and factory registration of CGhostPlayer)
const WRTTI CGhostPlayer::m_RTTI("CGhostPlayer", &IActor::m_RTTI);

struct __impCGhostPlayer {
	__impCGhostPlayer(void) {
		ObjectFactory().AddObjectFunctor(CGhostPlayerMakeInstance, "CGhostPlayer");
		ActorFactory().AddActorClass("CGhostPlayer");
	}
};
static __impCGhostPlayer s_impCGhostPlayer;

// ghostplayer.cpp:715
CGhostPlayer::CGhostPlayer(void)
	: m_pGhostDoc(0), m_pHoleData(0), m_pStateMachine(0), m_pAward(0), m_pMovieStyle(0),
	  m_count3c(0), m_count40(0), m_phase(0), m_alarmState(0), m_serverProperty(0) {
	m_priority = 5;
}

// ghostplayer.cpp:720
CGhostPlayer::~CGhostPlayer(void) {
}

// ghostplayer.cpp:726
CStateUnit* CGhostPlayer::CreateStateUnit(int type, GHOST_ACTION const* pAction) {
	switch (type) {
	case 0: {
		unsigned char power = (unsigned char)pAction->m_param[0].m_dw;
		unsigned char direction = (unsigned char)pAction->m_param[1].m_dw;
		unsigned char windType = (unsigned char)pAction->m_param[2].m_dw;
		unsigned char special = (unsigned char)pAction->m_param[3].m_dw;

		return new CWindChangeState(power, direction, windType, special);
	}
	case 1: {
		unsigned char weather = (unsigned char)pAction->m_param[0].m_dw;
		return new CWeatherChangeState(weather);
	}
	case 2: {
		float direction = pAction->m_param[0].m_f;
		return new CDirectionChangeState(direction);
	}
	case 3:
		return new CSpecialWindState;
	case 4: {
		unsigned char club = (unsigned char)pAction->m_param[0].m_dw;
		return new CClubChangeState(club);
	}
	case 5: {
		unsigned long itemTypeId = pAction->m_param[0].m_dw;
		unsigned long itemId = pAction->m_param[1].m_dw;
		return new CUseItemState(itemTypeId, itemId);
	}
	case 6: {
		unsigned char powerShot = (unsigned char)pAction->m_param[0].m_dw;
		return new CPoweShotMotionState(powerShot);
	}
	case 7: {
		float x = pAction->m_param[0].m_f;
		float y = pAction->m_param[1].m_f;
		float z = pAction->m_param[2].m_f;
		return new CBallDropState(x, y, z);
	}
	case 8:
		return new CTimeOutState;
	}

	return 0;
}

// ghostplayer.cpp:800
void CGhostPlayer::OnLoad(void) {
	m_pGhostDoc = WSingleton<CGhostManager>::Instance()->GetDocument();

	// The replay runs under the server property the ghost was recorded with.
	m_serverProperty = S5::UTIL::GetServerProperty();
	if (m_pGhostDoc->m_pGameData->m_serverProperty & 0x80) {
		S5::UTIL::SetServerProperty(m_serverProperty | 0x80);
	} else {
		S5::UTIL::SetServerProperty(m_serverProperty & ~0x80);
	}

	m_pStateMachine = new CSequenceStateMachine(this, "GhostSequenceMachine");
	m_pMovieStyle = new cMovieStyleDisplay;
}

// ghostplayer.cpp:817
void CGhostPlayer::OnReload(void) {
}

// ghostplayer.cpp:825
void CGhostPlayer::OnInit(void) {
	m_pStateMachine->Clear();
	m_shots.clear();
	m_count40 = 0;

	m_pHoleData = m_pGhostDoc->GetHoleData(GOLFDOC()->m_currentHole);

	if (m_pHoleData) {
		std::list<GHOST_SHOT_DATA*> const& shotDataList = m_pHoleData->GetShotDataList();
		std::copy(shotDataList.begin(), shotDataList.end(), std::back_inserter(m_shots));

		int shotNum = m_count3c;
		std::list<GHOST_SHOT_DATA*>::iterator it = m_shots.begin();
		while (it != m_shots.end()) {
			GHOST_SHOT_DATA* pShot = *it;

			m_pStateMachine->BackInsert(new CWarpEventState(pShot));
			m_pStateMachine->BackInsert(new CGhostTurnBeginAnimationState);

			for (int i = 0; i < 9; ++i) {
				if (pShot->m_actions[i].m_changed) {
					m_pStateMachine->BackInsert(CreateStateUnit(i, &pShot->m_actions[i]));
				}
			}

			if (pShot->m_shotNum != ++shotNum) {
				m_phase = 1;
				break;
			}

			m_pStateMachine->BackInsert(new CReadyToShotState);
			++it;
		}

		if (m_shots.size()) {
			SetWindAndWeather(GOLFDOC()->m_currentHole);
			return;
		}
	}

	m_phase = 3;
}

// ghostplayer.cpp:893
void CGhostPlayer::OnProcess(float fDelta) {
	if (!m_pAward && m_phase) {
		char text[64] = { 0 };
		sprintf(text, "GHOST FILE BREAK! (CODE:%d)(HOLE:%d)(SHOT:%d)", m_phase, GOLFDOC()->m_currentHole, m_count3c);
		GHOST_GAME_BRIEF const* pBrief = WSingleton<CGhostManager>::Instance()->GetLatestGameBrief();
		WSingleton<CGhostHandler>::Instance()->SendErrorReport(text, MakeStr("%d_%d_%d_%d_%d_%d.GST", pBrief->m_userId, pBrief->m_gameId,
			pBrief->m_fileNameParts[0], pBrief->m_fileNameParts[1], pBrief->m_fileNameParts[2], pBrief->m_fileNameParts[3]));

		HandleMsg(MsgObject(0, 0xE, 0, 0, 0, 0, 0));
		return;
	}

	if (m_pAward) {
		if (m_pAward->IsFinishCalcBonus() && !m_awardSent) {
			m_awardSent = 1;
			SendAwardResult();
		}
		return;
	}

	if (!IsGhostTurn()) {
		if (m_alarmState) {
			m_alarmState = 0;
		}
		return;
	}

	if (m_alarmState == 1) {
		m_pMovieStyle->Process(fDelta);
	}

	if (!(GOLFDOC()->GameMode() & 0x308)) {
		return;
	}

	if (!m_pStateMachine->Process(fDelta)) {
		m_phase = 4;
		return;
	}

	if (m_pStateMachine->GetCurrentState()
		&& strcmp("CReadyToShotState", m_pStateMachine->GetCurrentState()->GetClassNameA()) == 0) {
		if (m_count40 < m_shots.size()) {
			std::list<GHOST_SHOT_DATA*>::iterator it = m_shots.begin();
			std::advance(it, m_count40);
			if (it != m_shots.end()) {
				GHOST_SHOT_DATA* pShot = *it;
				if (pShot->m_type == 0) {
					if (DoShot(pShot)) {
						m_alarmState = 1;
						m_pMovieStyle->ResetHeight();
					} else {
						m_phase = 2;
						return;
					}
				}
			}

			++m_count40;
			++m_count3c;
		}
	}
}

// ghostplayer.cpp:975
bool CGhostPlayer::DoShot(GHOST_SHOT_DATA const* pShot) {
	if (pShot->m_holeNum != GOLFDOC()->m_currentHole) {
		return false;
	}

	memset(&GOLFDOC()->m_shotProtocol, 0, sizeof(sShotProtocol));
	type_convert(&pShot->m_protocol, &GOLFDOC()->m_shotProtocol);
	AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x7A, 0, 0, 0, 0, 0));

	return true;
}

// ghostplayer.cpp:988
void CGhostPlayer::OnDisplay(void) {
	if (!WSingleton<CSharedDoc>::Instance()->m_gameOver && IsGhostTurn()) {
		m_pStateMachine->Render();
	}
}

// ghostplayer.cpp:994
void CGhostPlayer::OnDestroy(void) {
	if (m_pStateMachine) { delete m_pStateMachine; m_pStateMachine = 0; }
	if (m_pMovieStyle) { delete m_pMovieStyle; m_pMovieStyle = 0; }

	S5::UTIL::SetServerProperty(m_serverProperty);
}

// ghostplayer.cpp:1002
void CGhostPlayer::HandleMsg(const MsgObject &msg) {
	if (WSingleton<CSharedDoc>::Instance()->m_gameMode != 5)
		return;

	switch (msg.m_msg) {
	case 13:
		*(int *)msg.m_param1 = m_alarmState;
		break;

	case 14:
		if (m_pAward == 0) {
			m_pAward = CreateForm<FrGhostAward>(g_pFresh->GetManager(), 0, "GHOST_AWARD", 0);
			m_pAward->Init((GOLFDOC()->GetPlayer(0)->m_playerFlags & 0xF) == 1);

			if (msg.m_param1 != 0) {
				WSingleton<CSharedDoc>::Instance()->m_gameOver = true;
				WSingleton<CTaskManager>::Instance()->PostMsg(this, "GolfRule", 0x78, 0x20000, 0, 0, 0);
				m_pAward->Open(0, 0);
			} else {
				FrForm *pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
				pForm->SetTopmost(true);
				pForm->FindNextTopFocus(false);
				pForm->SetTimeLimit(5.0f);
				// "A corrupted ghost was found; the game is stopped and the result screen follows."
				pForm->SetMessage("\xBC\xD5\xBB\xF3\xB5\xC8\x20\xB5\xA5\xC0\xCC\xC5\xCD\xB0\xA1\x20\xB9\xDF\xB0\xDF\xB5\xC7\xBE\xEE\x20\xB0\xD4\xC0\xD3\xC0\xBB\x20\xC1\xDF\xB4\xDC\xC7\xCF\xB0\xED\x20\xC0\xE1\xBD\xC3\x20\xC8\xC4\x20\xBD\xC3\xBB\xF3\xBD\xC4\xC0\xBB\x20\xC1\xF8\xC7\xE0\xC7\xD5\xB4\xCF\xB4\xD9\x2E\x0A", false);
				pForm->Open((FRESH_RESULT_CALLBACK)&CGhostPlayer::ExceptionAlarmWaitCallBack, 0);
			}
		}
		break;

	case 15: {
		CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
		std::list<sItemInfo>::iterator it = pDoc->m_itemInfoList.begin();
		while (it != pDoc->m_itemInfoList.end() && (*it).m_typeId != 0x1A000040)
			++it;
		if (it != pDoc->m_itemInfoList.end()) {
			WSendPacket packet((enumClientPacket)0xBA);
			packet.Encode1(3);
			packet.Encode4(0x1A000040);
			packet.Send(SEND_TO_DEFAULT);
		}
		break;
	}

	case 16:
		m_alarmState = 0;
		break;

	case 17:
		if (m_alarmState == 1)
			m_pMovieStyle->Render();
		break;

	case 18: {
		WSendPacket packet((enumClientPacket)0xBA);
		packet.Encode1(3);
		packet.Encode4(msg.m_param1);
		packet.Send(SEND_TO_DEFAULT);

		if (msg.m_param1 == 0x1A000011) {
			WSingleton<CProjectG>::Instance()->SetGameSpeed(3.0f);
			AfxGetTask()->GetActor("Letter") << MsgObject(0, 0x193, 0, 0, 0, 0, 0);
		}
		break;
	}
	}
}

// ghostplayer.cpp:1081
void CGhostPlayer::SetWindAndWeather(unsigned char holeNum) {
	CGhostHoleData const* pHoleData = m_pGhostDoc->GetHoleData(holeNum);

	if (pHoleData) {
		GHOST_SHOT_DATA const* pShot = pHoleData->GetShotDataList().front();
		GHOST_ACTION const& windAction = pShot->m_actions[0];
		if (windAction.m_changed) {
			CWindChangeState wind((unsigned char)windAction.m_param[0].m_dw, (unsigned char)windAction.m_param[1].m_dw,
				(unsigned char)windAction.m_param[2].m_dw, (unsigned char)windAction.m_param[3].m_dw);
			wind.OnStateRun();
		}

		GHOST_ACTION const& weatherAction = pShot->m_actions[1];
		if (weatherAction.m_changed) {
			CWeatherChangeState weather((unsigned char)weatherAction.m_param[0].m_dw);
			weather.OnStateRun();
		}
	}
}

// ghostplayer.cpp:1104
void CGhostPlayer::SendAwardResult(void) {
	if (m_pAward) {

		GHOST_USER_DATA* pUserData = m_pGhostDoc->m_pUserData;
		WSendPacket packet((enumClientPacket)0xBA);
		packet.Encode1(6);
		packet.Encode4(pUserData->uid);
		packet.Encode4(WSingleton<CGhostManager>::Instance()->GetLatestGameBrief()->m_fileNameParts[3]);
		packet.Encode1(GetCurMap());
		packet.Encode4(m_pAward->GetBasePang());
		packet.Encode4(m_pAward->GetAutoBonus());
		packet.Encode4(m_pAward->GetClickBonus());
		packet.Encode4(m_pAward->GetClickCount());
		packet.Encode4(GOLFDOC()->GetPlayer(0)->m_ghostResult);
		packet.Encode4(GOLFDOC()->GetPlayer(1)->m_ghostResult);
		packet.Encode4(GOLFDOC()->GetPlayer(0)->m_ghostAward);
		packet.Encode4(GOLFDOC()->GetPlayer(1)->m_ghostAward);
		packet.Encode1(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[1].m_level);
		packet.Encode1(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_level);
		packet.Encode1(GetHoleIndex(GOLFDOC()->m_currentHole));
		packet.Send(SEND_TO_DEFAULT);

		if (m_phase)
			m_pGhostDoc->SetInvalidate();
	}
}

// ghostplayer.cpp:1134
void CGhostPlayer::UseItem(unsigned long itemTypeId, unsigned long itemId) {
	if (itemTypeId == 0x1A000040) {
		return;
	}

	AfxGetTask()->PassMsgToActor("Item", MsgObject(0, 0x19F, -1, itemTypeId, itemId, 0, 0));
}

// ghostplayer.cpp:1145
bool CGhostPlayer::ExceptionAlarmWaitCallBack(int result, FrForm* pForm) {
	WSingleton<CSharedDoc>::Instance()->m_gameOver = true;
	WSingleton<CTaskManager>::Instance()->PostMsg(this, "GolfRule", 0x78, 0x20000, 0, 0, 0);

	if (m_pAward) {
		m_pAward->Open(0, 0);
	} else {
		m_pAward = CreateForm<FrGhostAward>(g_pFresh->GetManager(), 0, "GHOST_AWARD", 0);
		m_pAward->Init((GOLFDOC()->GetPlayer(0)->m_playerFlags & 0xF) == 1);
		m_pAward->Open(0, 0);
	}

	return true;
}
