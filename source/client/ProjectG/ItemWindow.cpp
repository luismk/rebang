// Original: d:\build\custom\temp\code\client\projectg\itemwindow.cpp
#include "ItemWindow.h"
#include "ObjectFactory.h"
#include "ClientSetting.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "SimpleOverlay.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "club.h"
#include "HelpMsg.h"
#include "mousecursor.h"
#include "ChatMsg.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "wview.h"
#include "wbone.h"
#include "golfball.h"
#include "quadtree.h"
#include "Packet.h"
#include "../shared/Localize.h"
#include <stdlib.h>

// Declared nowhere yet: s5/Utilities.h (S5::IsUsableCalipers) is another
// unit's header.
namespace S5 {
	bool IsUsableCalipers(void);
}

CItemWindow::eState CItemWindow::ms_state;

// itemwindow.cpp:36 (run-time type and factory registration of CItemWindow)
const WRTTI CItemWindow::m_RTTI("CItemWindow", &IActor::m_RTTI);

IObject* CItemWindowMakeInstance(void) {
	return new CItemWindow;
}

struct __impCItemWindow {
	__impCItemWindow(void) {
		ObjectFactory().AddObjectFunctor(CItemWindowMakeInstance, "CItemWindow");
		ActorFactory().AddActorClass("CItemWindow");
	}
};
static __impCItemWindow s_impCItemWindow;

// itemwindow.cpp:40
CItemWindow::CItemWindow(void) : m_pOverlay(0), m_pSimpleUI(0), m_unknown92(true) {
	m_priority = 4;
}

// itemwindow.cpp:45
void CItemWindow::OnDestroy(void) {
	for (std::map<unsigned long, WOverlay*>::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it) {
		if (g_resrcmng && it->second) { g_resrcmng->Release(it->second); it->second = 0; }
	}
	m_overlays.clear();

	if (g_resrcmng && m_pOverlay) { g_resrcmng->Release(m_pOverlay); m_pOverlay = 0; }
	if (m_pSimpleUI) { delete m_pSimpleUI; m_pSimpleUI = 0; }
}

// itemwindow.cpp:62
void CItemWindow::OnLoad(void) {
	if (GOLFDOC()->m_gameState < 15 && GOLFDOC()->m_gameState != 5) {
		return;
	}

	m_pOverlay = g_resrcmng->GetOverlay("{Item_win.png", 0);

	m_pSimpleUI = new CSimpleUI("[emoti_object.png");
	m_pSimpleUI->Add("0", WRect(0.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("1", WRect(64.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("2", WRect(128.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("3", WRect(192.0f, 128.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("4", WRect(0.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("5", WRect(64.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("6", WRect(128.0f, 192.0f, 64.0f, 64.0f), false);
	m_pSimpleUI->Add("7", WRect(192.0f, 192.0f, 64.0f, 64.0f), false);

	ResetUiPosition();
}

// itemwindow.cpp:86
void CItemWindow::OnInit(void) {
	ResetVars();
}

// itemwindow.cpp:94
void CItemWindow::ResetVars(void) {
	m_pos = 0.0f;
	m_speed = 0.0f;
	ms_state = STATE_CLOSED;
	m_selected = -1;
	m_over = -1;
	m_used = -1;

	m_unknown70 = 0;
	m_unknown74 = 200.0f;
	m_unknown78 = 200.0f;
	m_unknown90 = false;
	m_unknown93 = true;
}

// itemwindow.cpp:109
void CItemWindow::OnProcess(float delta) {
	if (GOLFDOC()->m_gameState < 15 && GOLFDOC()->m_gameState != 5) {
		return;
	}

	if (m_unknown90) {
		if (!(GOLFDOC()->GameMode() & 8)) {
			m_unknown90 = false;
		}

		m_flyTime += delta;
		if (m_bursting) {
			m_burstFrame = (char)(m_flyTime * 10.0f);

			if (m_burstFrame == 8) {
				m_burstFrame = 0;
				m_unknown90 = false;
			}
		} else if (m_flyTime < 0.8f) {
			m_flyVelY += delta * 6.0f;
			m_flyX += m_flyVelX;
			m_flyY += m_flyVelY;
		} else {
			WBone* pHead;
			SendMsg(this, "Player", 0x111, (intptr_t)&pHead, (intptr_t)"Bip01 Head", 0, 0);

			WVector headPos(pHead->GetMatrix().pivot.x, pHead->GetMatrix().pivot.y - 0.5f, pHead->GetMatrix().pivot.z);
			WVector screen = g_view->Projection(headPos);
			float dx = screen.x - m_flyX;
			m_flyVelX += dx * delta * 0.5f;
			m_flyVelY += (screen.y - m_flyY) * delta * 0.5f;

			float x = m_flyX + m_flyVelX;
			float y = m_flyY + m_flyVelY;

			if ((screen.x - x) * dx < 0.0f) {
				m_unknown90 = false;
			}

			m_flyX = x;
			m_flyY = y;
		}
	}

	if (m_unknown92 && IsMyTurn(true)) {
		m_mouseX = g_input->GetMousePoint().x;
		m_mouseY = g_input->GetMousePoint().y;

		CheckInput();

		switch (ms_state) {
		case STATE_OPENED:
			OnOpened(delta);
			break;

		case STATE_CLOSING:
			OnRight(delta);
			break;

		case STATE_OPENING:
			OnLeft(delta);
			break;

		case STATE_CLOSED:
			OnClosed(delta);
			break;
		}

		if (ms_state != STATE_CLOSED) {
			m_rect.x = m_pos - 444.0f;
		}
	}
}

// itemwindow.cpp:231
void CItemWindow::ExecuteNow(bool now) {
	switch (ms_state) {
	case STATE_OPENED:
		if (now) {
			g_audio->PlaySfx("play_menu_close", 0, 0, 0, 0, 0.5f, 200.0f);
		}
		// fall through
	case STATE_CLOSING:
		ms_state = STATE_OPENING;
		m_selected = m_over = -1;
		CMouseCursor::Instance()->SetItemWindow(false);
		CMouseCursor::Instance()->SetAutoHide(true);
		SendMsg(this, "Screen", 0x1b0, 0, 0, 0, 0);

		if (CHelpMsg::Instance()) {
			CHelpMsg::Instance()->SetItemHelpMsg(0);
		}
		break;

	case STATE_CLOSED:
		if (now) {
			g_audio->PlaySfx("play_menu_open", 0, 0, 0, 0, 0.5f, 200.0f);
		}
		GolfClub().SetItemWindow(true);
		if (GolfClub().BarIs(0)) {
			GolfClub().SetBar(1);
		}
		// fall through
	case STATE_OPENING:
		ms_state = STATE_CLOSING;
		{
			int numItems = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_numItems;
			m_speed = 1500.0f;
			m_targetPos = (float)numItems * 45.0f + 64.0f;
		}
		SendMsg(this, "Screen", 0x1af, 0, 0, 0, 0);
		CMouseCursor::Instance()->SetAutoHide(false);
		break;
	}
}

// itemwindow.cpp:265
void CItemWindow::OnClosed(float delta) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId) {
		CHelpMsg::Instance()->SetItemHelpMsg(PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId);
	}
}

// itemwindow.cpp:271
void CItemWindow::OnRight(float delta) {
	if (CChatMsg::Instance()->IsActive()) {
		ms_state = STATE_OPENING;
		CMouseCursor::Instance()->SetItemWindow(false);
	}

	m_pos += delta * m_speed;

	if (m_pos > m_targetPos) {
		m_speed -= delta * 52500.0f;

		if (m_pos > m_targetPos + 20.0f) {
			m_pos = m_targetPos + 20.0f;
		}
	} else if (m_speed < 0.0f) {
		m_pos = m_targetPos;
		ms_state = STATE_OPENED;
		m_over = 0;
		CMouseCursor::Instance()->SetItemWindow(true);
	}
}

// itemwindow.cpp:297
void CItemWindow::OnLeft(float delta) {
	m_pos -= delta * 1500.0f;

	if (m_pos < 0.0f) {
		m_pos = 0.0f;
		ms_state = STATE_CLOSED;
		SendMsg(this, "Screen", 0x1af, 0, 0, 0, 0);
		GolfClub().SetItemWindow(false);
	}
}

// itemwindow.cpp:694
bool CItemWindow::IsUsable(unsigned long typeId) {
	if (typeId == 0 || (typeId & 0xFC000000) != 0x18000000) {
		return false;
	}

	switch (typeId & 0x1FFFFFF) {
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x07:
	case 0x08:
	case 0x0A:
	case 0x0B:
	case 0x0F:
	case 0x11:
	case 0x12:
	case 0x13:
	case 0x1F:
	case 0x21:
		return GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 2;

	case 0x0C:
		if ((int)(float)PLAYER(GOLFDOC()->m_myPlayerIndex)->m_comboGauge < 33) {
			return false;
		}
		// fall through
	case 0x04:
	case 0x06:
	case 0x16:
	case 0x20:
	case 0x22:
	case 0x25:
	case 0x27:
		return GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 2 && GolfClub().GetType() != 3;

	case 0x05:
		return GetPVS().GetGroundType(GolfBall().m_pos, true, 0) == 2;

	case 0x09:
		return true;

	case 0x0D:
		return false;

	case 0x0E:
		return GolfBall().m_landed;

	case 0x10:
		return GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 2 && GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 2 && GolfClub().GetType() != 3;

	case 0x28:
		if (!IsLocalContent((localContentType_t)0x2F)) {
			return false;
		}

		return GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 2 && GetPVS().GetGroundType(GolfBall().m_pos, true, 0) != 0;
	}

	return false;
}

// itemwindow.cpp:766
bool CItemWindow::UseItem(int slot) {
	if (GalleryMode()) {
		return false;
	}

	unsigned long typeId = GetTidItemSlot(GOLFDOC()->m_myPlayerIndex)[slot];

	if (!IsUsable(typeId)) {
		if (CHelpMsg::Instance()) {
			CHelpMsg::Instance()->SetHelpMsg(0x11, 0);
		}

		return false;
	}

	if (typeId == 0x18000009) {
		if (WSingleton<CSharedDoc>::Instance()->m_gameMode != 2 && WSingleton<CSharedDoc>::Instance()->m_gameMode != 1) {
			if (S5::IsUsableCalipers() == true) {
				if (CHelpMsg::Instance()) {
					CHelpMsg::Instance()->SetHelpMsg(0x1B, 0);
				}

				return false;
			}
		}
	}

	CMouseCursor::Instance()->SetItemWindow(false);

	m_used = slot;
	ms_state = STATE_OPENING;

	if (OnlinePlay()) {
		WSendPacket packet((enumClientPacket)0x17);
		packet.Encode4(typeId);
		packet.Send(SEND_TO_DEFAULT);
	} else if (WSingleton<CSharedDoc>::Instance()->m_gameMode != 5) {
		SendMsg(this, "GolfRule", 0x92, 0, 0, 0, 0);
		ProcessItem(GOLFDOC()->m_myPlayerIndex, typeId, true, rand());
	}

	return true;
}

// itemwindow.cpp:835
void CItemWindow::NetUseItem(unsigned long uid, unsigned long typeId, unsigned long param) {
	if ((WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().identity & 0xE) == 0xE && ms_state != STATE_CLOSED) {
		CMouseCursor::Instance()->SetItemWindow(false);
		CHelpMsg::Instance()->SetItemHelpMsg(typeId);

		ms_state = STATE_OPENING;
		m_selected = m_over = -1;
	}

	if (OnlinePlay()) {
		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (PLAYER(i)->m_state == 3) {
				continue;
			}

			if (PLAYER(i)->m_uid == uid) {
				ProcessItem(i, typeId, uid == MyGuid(false), param);
				return;
			}
		}
	} else {
		ProcessItem(GOLFDOC()->m_myPlayerIndex, typeId, true, param);
	}
}

// itemwindow.cpp:993
void CItemWindow::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x194:
		CheckInput();
		break;

	case 0x195:
		ms_state = STATE_CLOSING;
		ExecuteNow(false);
		m_unknown93 = true;
		break;

	case 0x196:
		ms_state = STATE_OPENING;
		ExecuteNow(false);
		m_unknown93 = false;
		break;

	case 0x199:
		*(int*)msg.m_param1 = ms_state;
		break;

	case 0x197:
		if (ms_state != STATE_CLOSED) {
			*(int*)msg.m_param1 = 1;
		} else {
			*(int*)msg.m_param1 = 0;
		}
		break;

	case 0x198:
		Draw();
		break;

	case 0x19a:
		m_unknown92 = true;
		break;

	case 0x19b:
		m_unknown92 = false;
		break;

	case 0x19e:
		UseItem(msg.m_param1);
		break;

	case 0x19f:
		NetUseItem(msg.m_param1, msg.m_param2, msg.m_param3);
		break;

	case 0x1a0:
		ResetVars();
		break;

	case 0x271b:
		ResetUiPosition();
		break;
	}
}

// itemwindow.cpp:1047
void CItemWindow::ResetUiPosition(void) {
	float barY = COption::Instance()->gGetBar_Y();

	if (m_pOverlay) {
		m_clipRect.x = 65.0f;
		m_clipRect.y = barY - 25.0f;
		m_clipRect.w = (float)m_pOverlay->GetWidth();
		m_clipRect.h = (float)m_pOverlay->GetHeight();
		m_pOverlay->SetClippingArea(&m_clipRect);
	}

	m_rect.x = -444.0f;
	m_rect.y = barY - 25.0f;
	m_rect.w = m_clipRect.w;
	m_rect.h = m_clipRect.h;
}

// itemwindow.cpp:1077
bool CItemWindow::IsClosed(void) {
	return ms_state == STATE_CLOSED ? true : false;
}

// itemwindow.cpp:1087
unsigned long* CItemWindow::GetTidItemSlot(unsigned char index) {
	IActor* pTutorial = GetActor("Tutorial");
	if (pTutorial && index == 0) {
		unsigned long* pSlot = 0;
		pTutorial << MsgObject(0, 0x152, (intptr_t)&pSlot, 0, 0, 0, 0);

		if (pSlot) {
			return pSlot;
		}
	}

	return WSingleton<CSharedDoc>::Instance()->m_roomPlayers[index].m_equipItem;
}
