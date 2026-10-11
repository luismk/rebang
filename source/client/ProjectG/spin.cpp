// Original: d:\build\custom\temp\code\client\projectg\spin.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "spin.h"

#include <new>
#include <math.h>

#include "ObjectFactory.h"
#include "ClientSetting.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "club.h"
#include "golfball.h"
#include "QuitWindow.h"
#include "ItemWindow.h"
#include "mousecursor.h"
#include "InputManager.h"
#include "Localize.h"
#include "wutil.h"

// clientsetting (0x4020e0); its header is not known yet (as in PetBody.cpp).
int WisZero(const float &f, float eps);

// spin.cpp:33 (run-time type and factory registration of CSpin)
const WRTTI CSpin::m_RTTI("CSpin", &IActor::m_RTTI);

IObject* CSpinMakeInstance(void) {
	return new CSpin;
}

struct __impCSpin {
	__impCSpin(void) {
		ObjectFactory().AddObjectFunctor(CSpinMakeInstance, "CSpin");
		ActorFactory().AddActorClass("CSpin");
	}
};
static __impCSpin s_impCSpin;

// spin.cpp:36
CSpin::CSpin(void) {
	m_priority = 4;
	AddSkipList(0x400);
}

// spin.cpp:43
void CSpin::OnPreLoadInit(void) {
	float barY = COption::Instance()->gGetBar_Y();

	m_baseX = 58.0f;
	m_baseY = barY + 4.0f;
}

// spin.cpp:51
void CSpin::Reset(void) {
	m_worldX = m_baseX;
	m_worldY = m_baseY;
	m_spinX = 0.0f;
	m_spinY = 0.0f;

	m_autoWorldX = m_baseX;
	m_autoWorldY = m_baseY;
	m_autoX = 0.0f;
	m_autoY = 0.0f;
	m_bAutoPoint = false;
	m_bFlag56 = false;
	m_bToggle57 = false;
	m_direction = 2;
	m_nextDirection = 2;
	m_loopMode = 0;
	m_bFlag64 = false;

	m_color = 0xFFFFFFFF;
	m_bActive = false;
	m_bFlag65 = false;
}

// spin.cpp:77
void CSpin::OnInit(void) {
	Reset();
}

// spin.cpp:83
void CSpin::ProcessAutoPoint(float elapsed) {
	if (m_bAutoPoint) {
		float x = m_autoX;
		float y = m_autoY;

		ProcessLoopHitPoint(elapsed, x, y, elapsed);

		m_autoX = x;
		m_autoY = y;
		m_autoWorldX = x + m_baseX;
		m_autoWorldY = y + m_baseY;
	}
}

// spin.cpp:119
void CSpin::OnProcess(float elapsed) {
	if (GOLFDOC()->m_gameState < 12)
		return;

	if (CQuitWindow::Instance()->IsActive() || !CItemWindow::IsClosed())
		return;

	if (GolfClub().GetType() == 3 || !m_bActive)
		return;

	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000000
		|| PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000011
		|| GOLFDOC()->m_gameState == 12) {
		m_radiusY = 30.0f;
	} else {
		m_radiusY = (float)(int)PLAYER(GOLFDOC()->m_myPlayerIndex)->m_spin;
	}

	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000001 || GOLFDOC()->m_gameState == 12) {
		m_radiusX = 30.0f;
	} else {
		m_radiusX = (float)(int)PLAYER(GOLFDOC()->m_myPlayerIndex)->m_curve;
	}

	if (m_bToggle57 == true) {
		ProcessAutoPoint(elapsed);
	}

	if (m_bFlag65)
		return;

	float x = g_input->GetMousePoint().x - m_baseX;
	float y = g_input->GetMousePoint().y - m_baseY;

	// The original also requires an int at CMouseCursor +0xA0 to be 3 here
	// (mousecursor.h does not have that member yet).
	if (CMouseCursor::Instance()->GetArea() == 3 && g_input->GetButton(LEFT_BUTTON)) {
		if (IsInclude(x, y)) {
			m_spinX = x;
			m_spinY = y;
			m_color = 0xFFFFFFFF;
		} else {
			if (WisZero(x, 1e-5f)) {
				m_spinX = 0.0f;
				m_spinY = (y > 0.0f ? 1.0f : -1.0f) * m_radiusY;
			} else {
				float slope = y / x;
				float edgeX = m_radiusY / sqrtf(m_radiusX * m_radiusX * slope * slope + m_radiusY * m_radiusY) * m_radiusX;

				if ((float)tan(-20.0 * 0.017453292f) < slope && (float)tan(20.0 * 0.017453292f) > slope) {
					m_spinX = (x > 0.0f ? 1.0f : -1.0f) * m_radiusX;
					m_spinY = 0.0f;
				} else if ((float)tan(1.5707964f - 20.0 * 0.017453292f) < slope || (float)tan(1.5707964f + 20.0 * 0.017453292f) > slope) {
					m_spinX = 0.0f;
					m_spinY = y > 0.0f ? m_radiusY : m_radiusY * -1.0f;
				} else {
					m_spinX = x > 0.0f ? edgeX : edgeX * -1.0f;
					m_spinY = m_spinX * slope;
				}
			}

			m_color = 0xA0FFFFFF;
		}
	}

	if (GOLFDOC()->m_gameState == 12 && m_bFlag56) {
		if (IsPointInAutoPoint()) {
			m_bFlag56 = false;
			IActor *tutorial = GetActor("Tutorial");
			if (tutorial)
				tutorial << MsgObject(0, 0x149, 0, 0, 0, 0, 0);
		}
	}

	if (!(g_input->Get("LCONTROL", true) || g_input->Get("RCONTROL", true))
		|| g_input->Get("LSHIFT", true) || g_input->Get("RSHIFT", true)) {
		ConvertToWorld(m_spinX, m_spinY);
		return;
	}

	float step = elapsed * 3.1415927f;
	x = m_spinX;
	y = m_spinY;

	// up
	if (g_input->Get("\xBB\xF3", true)) {
		float limit = sqrtf(1.0f - x * x / (m_radiusX * m_radiusX)) * m_radiusY * -1.0f;

		if (y == limit) {
			float angle = acos(Between(-1.0f, x / m_radiusX, 1.0f));

			if (x > 0.0f)
				m_angle = Max(angle * -1.0f - step, -1.5707964f);
			else
				m_angle = Min(step - angle, -1.5707964f);

			x = cos(m_angle) * m_radiusX;
			y = sin(m_angle) * m_radiusY;
		} else {
			y -= elapsed * 50.0f;

			if (y < limit) {
				y = limit;
				m_color = 0xA0FFFFFF;
			} else {
				m_color = 0xFFFFFFFF;
			}
		}
	}

	// down
	if (g_input->Get("\xC7\xCF", true)) {
		float limit = sqrtf(1.0f - x * x / (m_radiusX * m_radiusX)) * m_radiusY;

		if (y == limit) {
			float angle = acos(Between(-1.0f, x / m_radiusX, 1.0f));

			if (x < 0.0f)
				m_angle = Max(angle - step, 1.5707964f);
			else
				m_angle = Min(angle + step, 1.5707964f);

			x = cos(m_angle) * m_radiusX;
			y = sin(m_angle) * m_radiusY;
		} else {
			y += elapsed * 50.0f;

			if (y > limit) {
				y = limit;
				m_color = 0xA0FFFFFF;
			} else {
				m_color = 0xFFFFFFFF;
			}
		}
	}

	// left
	if (g_input->Get("\xC1\xC2", true)) {
		float limit = sqrtf(1.0f - y * y / (m_radiusY * m_radiusY)) * m_radiusX * -1.0f;

		if (x == limit) {
			float angle = acos(Between(-1.0f, x / m_radiusX, 1.0f));

			if (y > 0.0f)
				m_angle = Min(angle + step, 3.1415927f);
			else
				m_angle = Max(angle * -1.0f - step, -3.1415927f);

			x = cos(m_angle) * m_radiusX;
			y = sin(m_angle) * m_radiusY;
		} else {
			x -= elapsed * 50.0f;

			if (x < limit) {
				x = limit;
				m_color = 0xA0FFFFFF;
			} else {
				m_color = 0xFFFFFFFF;
			}
		}
	}

	// right
	if (g_input->Get("\xBF\xEC", true)) {
		float limit = sqrtf(1.0f - y * y / (m_radiusY * m_radiusY)) * m_radiusX;

		if (x == limit) {
			float angle = acos(Between(-1.0f, x / m_radiusX, 1.0f));

			if (y < 0.0f)
				m_angle = Min(step - angle, 0.0f);
			else
				m_angle = Max(angle - step, 0.0f);

			x = cos(m_angle) * m_radiusX;
			y = sin(m_angle) * m_radiusY;
		} else {
			x += elapsed * 50.0f;

			if (x > limit) {
				x = limit;
				m_color = 0xA0FFFFFF;
			} else {
				m_color = 0xFFFFFFFF;
			}
		}
	}

	m_spinX = x;
	m_spinY = y;
	ConvertToWorld(x, y);
}

// spin.cpp:364
void CSpin::GetNextDirection(void) {
	switch (m_loopMode) {
	case 0:
		m_direction = 0;
		break;
	case 1:
		if (m_nextDirection == 5)
			m_nextDirection = 2;
		else
			m_nextDirection++;
		break;
	case 2:
		if (m_direction == 3) {
			m_nextDirection = 5;
		} else if (m_direction == 5) {
			m_nextDirection = 3;
		}
		break;
	}
}

// spin.cpp:404
void CSpin::ProcessLoopHitPoint(float elapsed, float &x, float &y, float &unused) {
	switch (m_direction) {
	case 1:
		if (y < 0.0f) {
			y += elapsed * 50.0f;
			if (y >= 0.0f) {
				GetNextDirection();
				m_direction = m_nextDirection;
				y = 0.0f;
			}
		} else if (x < 0.0f) {
			x += elapsed * 50.0f;
			if (x >= 0.0f) {
				GetNextDirection();
				m_direction = m_nextDirection;
				x = 0.0f;
			}
		} else if (y > 0.0f) {
			y -= elapsed * 50.0f;
			if (y <= 0.0f) {
				GetNextDirection();
				m_direction = m_nextDirection;
				y = 0.0f;
			}
		} else {
			x -= elapsed * 50.0f;
			if (x <= 0.0f) {
				GetNextDirection();
				m_direction = m_nextDirection;
				x = 0.0f;
			}
		}
		break;
	case 2: {
		float a = x;
		float limit = sqrtf(1.0f - a * a * 0.0025f) * -20.0f;
		if (y == limit) {
			m_direction = (m_nextDirection != 0);
		} else {
			y -= elapsed * 50.0f;
			if (y < limit)
				y = limit;
		}
		break;
	}
	case 3: {
		float a = y;
		float limit = sqrtf(1.0f - a * a * 0.0025f) * -20.0f;
		if (x == limit) {
			m_direction = (m_nextDirection != 0);
		} else {
			x -= elapsed * 50.0f;
			if (x < limit)
				x = limit;
		}
		break;
	}
	case 4: {
		float a = x;
		float limit = sqrtf(1.0f - a * a * 0.0025f) * 20.0f;
		if (y == limit) {
			m_direction = (m_nextDirection != 0);
		} else {
			y += elapsed * 50.0f;
			if (y > limit)
				y = limit;
		}
		break;
	}
	case 5: {
		float a = y;
		float limit = sqrtf(1.0f - a * a * 0.0025f) * 20.0f;
		if (x == limit) {
			m_direction = (m_nextDirection != 0);
		} else {
			x += elapsed * 50.0f;
			if (x > limit)
				x = limit;
		}
		break;
	}
	}
}

// spin.cpp:545
void CSpin::ConvertToWorld(float x, float y) {
	m_worldX = x + m_baseX;
	m_worldY = y + m_baseY;
}

// spin.cpp:552
bool CSpin::IsPointInAutoPoint(void) {
	if (!IsLocalContent((localContentType_t)0x38))
		return false;

	if (fabs(m_autoWorldX - m_baseX) < 0.6 && fabs(m_autoWorldY - m_baseY) < 0.6)
		return false;

	if (fabs(m_autoWorldX - m_worldX) < 2.0f && fabs(m_autoWorldY - m_worldY) < 2.0f)
		return true;

	return false;
}

// spin.cpp:566
void CSpin::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x161:
		if (GolfClub().GetType() == 3) {
			*(float*)msg.m_param1 = 0.0f;
			*(float*)msg.m_param2 = 0.0f;
		} else {
			*(float*)msg.m_param1 = m_spinX * (1.0f / 30.0f);
			*(float*)msg.m_param2 = m_spinY * (1.0f / 30.0f);
		}
		break;
	case 0x163:
		m_bActive = true;
		break;
	case 0x164:
		m_bActive = false;
		break;
	case 0x271A:
		Reset();
		break;
	case 0x165:
		switch (msg.m_param1) {
		case 0:
			m_spinX = Between(-1.0f, GolfBall().m_lieSlope, 1.0f) * 30.0f;
			m_spinY = Between(-1.0f, GolfBall().m_lieAngle, 1.0f) * 30.0f;
			break;
		case 1:
			m_spinX = (float)msg.m_param2;
			m_spinY = (float)msg.m_param3;
			break;
		case 2:
			m_autoX = (float)msg.m_param2;
			m_autoY = (float)msg.m_param3;

			m_autoWorldX = m_baseX + m_autoX;
			m_autoWorldY = m_baseY + m_autoY;
			return;
		}
		{
			bool bInclude = IsInclude(m_spinX, m_spinY);

			ConvertToWorld(m_spinX, m_spinY);
			m_bActive = true;
			m_color = bInclude ? 0xFFFFFFFF : 0xA0FFFFFF;
		}
		break;
	case 0x167:
		*(unsigned long*)msg.m_param3 = m_bActive ? m_color : 0;
	case 0x166:
		*(float*)msg.m_param1 = m_worldX;
		*(float*)msg.m_param2 = m_worldY;
		break;
	case 0x168:
		*(float*)msg.m_param1 = m_autoWorldX;
		*(float*)msg.m_param2 = m_autoWorldY;
		*(unsigned long*)msg.m_param3 = m_color;
		break;
	case 0x271B: {
		float barY = COption::Instance()->gGetBar_Y();

		m_baseY = barY + 4.0f;
		ConvertToWorld(m_spinX, m_spinY);
		break;
	}
	case 0x162:
		*(float*)msg.m_param1 = m_spinX / m_radiusX;
		*(float*)msg.m_param2 = m_spinY / m_radiusY;
		break;
	case 0x169: {
		int mode = msg.m_param1;

		if (mode < 2) {
			m_bAutoPoint = (mode == 1);
		} else if (mode == 3) {
			m_bFlag65 = true;
		} else if (mode == 4) {
			m_bFlag65 = false;
		} else {
			m_bFlag56 = true;
		}
		break;
	}
	case 0x16A:
		m_bToggle57 = !m_bToggle57;
		break;
	case 0x16B:
		m_direction = msg.m_param1;
		m_nextDirection = msg.m_param2;
		m_loopMode = msg.m_param3;
		break;
	}
}

// spin.cpp:703
bool CSpin::IsInclude(float x, float y) {
	if (x * x / (m_radiusX * m_radiusX) + y * y / (m_radiusY * m_radiusY) < 1.0f)
		return true;

	return false;
}
