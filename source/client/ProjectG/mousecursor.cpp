// Original: d:\build\custom\temp\code\client\projectg\mousecursor.cpp
#include "ClientSetting.h"
#include "mousecursor.h"
#include "InputManager.h"
#include "SimpleOverlay.h"
#include "QuitWindow.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frdesktop.h"
#include "frgroupbox.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "SharedDoc.h"
#include "golfdoc.h"
#include "quadtree.h"
#include "TaskManager.h"
#include "RealMyRoomTask.h"
#include "messengerdlg.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "frwnd.h"
#include <string>

// clientsetting.h:444
bool COption::GetQuitWindowed(void) {
	return m_quitWindowed;
}

// mousecursor.cpp:41
CMouseCursor::CMouseCursor(void) : m_pImages(0), m_pBoundArea(0), m_pTextTip(0) {
	m_pImages = new sCursorImage[12];

	const char* names[12] = {
		"1", "2", "3", "4",
		"_up", "_down", "_zoom", "_both",
		"_move", "_move_no", "_rotate", "_rotate_no",
	};

	for (int i = 0; i < 12; i++) {
		std::string fileName;
		fileName = "[s_pointer";
		fileName += names[i];
		fileName += ".png";

		m_pImages[i].m_pOverlay = g_resrcmng->GetOverlay(fileName.c_str(), 0);

		m_pImages[i].m_width = (float)m_pImages[i].m_pOverlay->GetWidth();
		m_pImages[i].m_height = (float)m_pImages[i].m_pOverlay->GetHeight();
		m_pImages[i].m_frameSize = 32.0f;
		m_pImages[i].m_frameCount = (int)(m_pImages[i].m_width / 32.0f);
	}

	m_frameTime = 0.35f;
	m_frame = 0;
	m_holeCupX = 0.0f; m_holeCupY = 0.0f;
	m_autoHide = false;
	m_toggle = true;

	if (g_view) {
		m_pointer.x = g_view->GetWidth();
		m_pointer.y = g_view->GetHeight();
	} else {
		m_pointer.x = 800.0f;
		m_pointer.y = 600.0f;
	}

	if (m_pBoundArea) { delete m_pBoundArea; m_pBoundArea = 0; }
	Reset(false);

	m_mode = SCREEN_LOBBY;
	m_pointer.x = 500.0f;
	m_pointer.y = 300.0f;

	m_pGroundPropt = new CGroundPropt("[ground_2.jpg");

	m_pGroundPropt->GetGroundTip()->Add("Fairway", WRect(0.0f, 0.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Rough", WRect(0.0f, 25.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Green", WRect(0.0f, 50.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Bunker", WRect(0.0f, 75.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("OB", WRect(0.0f, 100.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Ice", WRect(0.0f, 125.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Sand", WRect(0.0f, 150.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("Water", WRect(0.0f, 175.0f, 90.0f, 25.0f), false);
	m_pGroundPropt->GetGroundTip()->Add("WaterHazard", WRect(0.0f, 200.0f, 90.0f, 25.0f), false);

	m_pGroundPropt->GetGroundTip()->Add("Vector", WRect(0.0f, 225.0f, 90.0f, 25.0f), false);

	m_pGroundPropt->SetGroundType("");
	m_terrainToolTip = COption::Instance()->gGetTerrainTooltip() != 0;
}

// mousecursor.cpp:117
CMouseCursor::~CMouseCursor(void) {
	for (int i = 0; i < 12; i++) {
		if (m_pImages[i].m_pOverlay) {
			g_resrcmng->Release(m_pImages[i].m_pOverlay);
			m_pImages[i].m_pOverlay = 0;
		}
	}

	if (m_pImages) { delete[] m_pImages; m_pImages = 0; }
	if (m_pBoundArea) { delete m_pBoundArea; m_pBoundArea = 0; }

	if (m_pGroundPropt) { delete m_pGroundPropt; m_pGroundPropt = 0; }
}

// mousecursor.cpp:135
void CMouseCursor::SetBoundArea(WRect* rect) {
	if (rect == 0) {
		if (m_pBoundArea) { delete m_pBoundArea; m_pBoundArea = 0; }
		return;
	}

	if (m_pBoundArea == 0)
		m_pBoundArea = new WRect;

	m_pBoundArea->x = rect->x;
	m_pBoundArea->y = rect->y;
	m_pBoundArea->y = rect->y;
	m_pBoundArea->y = rect->y;
}

// mousecursor.cpp:152
void CMouseCursor::Reset(bool all) {
	m_storedPointer = m_pointer;

	m_hoverTime = 0;
	g_input->SetMousePoint(m_pointer.x, m_pointer.y);

	m_active = all;
	m_tempActive = false;
	m_visible = true;
	m_dragged = false;

	m_animTime = 0.0f;
	m_groundTipTime = 0.0f;

	m_area = 0;
	m_cursor = 0;

	m_camMode = false;
	m_moveRate = 0.0f;
	m_resetState = 0;

	m_itemWindow = false;
	m_cursorOverride = false;

	m_points.clear();

	for (int i = 0; i < 3; i++) m_pressArea[i] = 0;

	memset(m_enable, 1, sizeof(m_enable));
}

// mousecursor.cpp:188
void CMouseCursor::Process(float dt) {
	if (!m_toggle)
		return;

	if (!m_active && !m_tempActive)
		return;

	if (m_moveRate > 0.0f) {
		m_moveRate -= dt * m_moveSpeed;
		if (m_moveRate < 0.0f) {
			m_moveRate = 0.0f;
			m_storedPointer = m_moveTarget;
		}

		WVector2D delta = m_moveDelta * m_moveRate; m_pointer = WVector2D(delta.x + m_moveTarget.x, delta.y + m_moveTarget.y);

		g_input->SetMousePoint(m_pointer.x, m_pointer.y);
		return;
	}

	if (m_autoHide && WisEqual(g_input->GetMouseDelta(), WVector::ZERO, 1e-5f) && !g_input->GetButton(LEFT_BUTTON) && !g_input->GetButton(RIGHT_BUTTON) && !g_input->GetButton(MIDDLE_BUTTON)) {
		m_hoverTime += dt;
		if (m_hoverTime > 3.0f)
			m_visible = false;
	} else {
		m_hoverTime = 0.0f;
		m_visible = true;
	}

	if (m_pImages[m_cursor].m_frameCount > 0) {
		m_animTime += dt;

		if (m_animTime > m_frameTime / m_pImages[m_cursor].m_frameCount) {
			m_animTime -= m_frameTime / m_pImages[m_cursor].m_frameCount;
			m_frame = (m_frame + 1) % m_pImages[m_cursor].m_frameCount;
		}
	}

	m_pointer.x = g_input->GetMousePoint().x;
	m_pointer.y = g_input->GetMousePoint().y;

	if (m_mode == SCREEN_TEST)
		return;

	if (m_visible)
		m_cursor = CheckArea();

	if (!m_enable[m_area]) {
		m_area = 0;
		m_cursor = 0;
	}

	for (int i = 0; i < 3; i++) {
		if (g_input->GetButton((eButton)i) == 1)
			m_pressArea[i] = m_area;
	}

	if (GOLFDOC() && CSharedDoc::Instance()->m_roomType != 2 && GOLFDOC()->GameMode() == 0x200 && m_terrainToolTip) {
		int area = m_area;
		if (area == 0 || area == 0x22) {
			m_groundTipTime += dt;

			if (m_groundTipTime > 0.2f) {
				m_groundTipTime = 0.0f;
				if (m_pGroundPropt)
					m_pGroundPropt->SelectGroundTip();
			}
		}
	}

	switch (m_cursor) {
	case 0:
		if (g_input->GetButton(LEFT_BUTTON) == 3) {
			float dx = g_input->GetButtonDownPos(LEFT_BUTTON).x - m_pointer.x; dx = dx > 0.0f ? dx : -dx;
			float dy = g_input->GetButtonDownPos(LEFT_BUTTON).y - m_pointer.y; dy = dy > 0.0f ? dy : -dy;

			if (dx + dy > 0.0f)
				m_cursor = 3;
		}
		break;
	case 1:
		if (g_input->GetButton(LEFT_BUTTON)) {
			if (m_pressArea[LEFT_BUTTON] == m_area || m_dragged)
				m_cursor = m_checkGame ? 2 : 3;
			else
				m_cursor = 0;
		} else if (g_input->GetButton(RIGHT_BUTTON)) {
			if (m_pressArea[RIGHT_BUTTON] == m_area || m_dragged)
				m_cursor = m_checkChat ? 2 : 3;
			else
				m_cursor = 0;
		}
		break;
	}
}

// mousecursor.cpp:360
int CMouseCursor::CheckArea(void) {
	m_checkGame = true;
	m_checkChat = false;

	switch (m_mode) {
	case SCREEN_GAME:
		return GameCheck();
	case SCREEN_CHAT:
		return ChatCheck();
	case SCREEN_TEST:
		return TestCheck();
	}
	return 0;
}

// mousecursor.cpp:379
int CMouseCursor::TestCheck(void) {
	m_checkGame = true;
	m_checkChat = true;

	m_area = 0;
	return 0;
}

// mousecursor.cpp:388
bool CMouseCursor::ChildWindowCallback(FrWnd* pWnd, void* pParam) {
	std::string name;
	int* pFound = (int*)pParam;

	if (pWnd->IsVisible()) {
		if ((__rtti_obj = (void*)pWnd, ((IObject*)__rtti_obj)->IsKindOf(&FrGroupBox::m_RTTI))) {
			pWnd->EnumerateChildWindow(ChildWindowCallback, pParam);
			if (*pFound == 1) {
				return false;
			}
			return true;
		}

		WRect rect;
		rect = pWnd->GetRect();
		if (Instance()->m_pointer.x >= rect.x && rect.x + rect.w >= Instance()->m_pointer.x && Instance()->m_pointer.y >= rect.y && rect.y + rect.h >= Instance()->m_pointer.y) {
			pWnd->GetWindowName(name);

			if (name == "chatview") {
				FrWnd* pScroll = pWnd->FindChild("scrollbar");
				if (pScroll) {
					rect = pScroll->GetRect();
					if (Instance()->m_pointer.x >= rect.x && rect.x + rect.w >= Instance()->m_pointer.x && Instance()->m_pointer.y >= rect.y && rect.y + rect.h >= Instance()->m_pointer.y) {
						Instance()->m_area = 0x40;
						*pFound = 1;
						return false;
					}
				}
			} else {
				Instance()->m_area = 0x40;
				*pFound = 1;
				return false;
			}
		}
	}

	Instance()->m_area = 0;
	*pFound = 0;
	return true;
}

// mousecursor.cpp:445
int CMouseCursor::ChatCheck(void) {
	// The quit window covers everything.
	if (CQuitWindow::Instance() && CQuitWindow::Instance()->IsActive()) {
		m_area = 0x24;
		return 0;
	}

	std::list<FrWnd*>& topWnds = g_pFresh->GetManager()->GetTopWndList();
	for (std::list<FrWnd*>::iterator it = topWnds.begin(); it != topWnds.end(); ++it) {
		FrWnd* wnd = *it;

		if (wnd->IsVisible()) {
			WRect rect = wnd->GetRect();
			if (m_pointer.x >= rect.x && m_pointer.x <= rect.x + rect.w && m_pointer.y >= rect.y && m_pointer.y <= rect.y + rect.h) {
				m_area = 0x40;
				return 1;
			}
		}
	}

	int area = 0;

	g_pFresh->GetDesktop()->EnumerateChildWindow(ChildWindowCallback, &area);
	if (area != 0)
		return area;

	m_area = 0;
	return 0;
}

extern WMatrix g_camera;

// mousecursor.cpp:481
float CMouseCursor::RayTrace4Ground(void) {
	WVector origin;
	g_view->GetScreen2World(m_pointer.x, m_pointer.y, origin, m_rayDir);
	m_rayDir.Normalize();
	m_rayDir *= 300.0f;

	if (!(WAbsBySign(m_rayDir.y) < WMATH_EPSILON)) {
		float t = g_camera.pivot.y / m_rayDir.y * -1.0f;
		m_rayDist = t;
		float x = g_camera.pivot.x + m_rayDir.x * t;
		float z = g_camera.pivot.z + m_rayDir.z * t;

		if (x > m_groundMinX && x < m_groundMaxX && z > m_groundMinZ && z < m_groundMaxZ)
			return t;
	}

	return 1.0f;
}

// mousecursor.cpp:506
bool CMouseCursor::IsInclude(float x, float y, float w, float h, bool sizeOnly) {
	if (m_pointer.x >= x && m_pointer.x <= (sizeOnly ? 0.0f : x) + w && m_pointer.y >= y && m_pointer.y <= (sizeOnly ? 0.0f : y) + h)
		return true;

	return false;
}

// mousecursor.cpp:962
bool CMouseCursor::SetArea(int area) {
	if (m_enable[area]) {
		m_area = area;
		return true;
	}

	return false;
}

// mousecursor.cpp:973
void CMouseCursor::Display(void) {
	if (!m_toggle)
		return;

	if (IsLocalContent((localContentType_t)0x4F) && CSharedDoc::Instance()->m_playingMode == 2 && CSharedDoc::Instance()->m_replayState == 7)
		return;

	if ((!m_active && !m_tempActive) || !m_visible || (m_mode == SCREEN_GAME && GOLFDOC() && GOLFDOC()->GameMode() == 0x400))
		return;

	WRect dest(m_pointer.x, m_pointer.y, m_pImages[m_cursor].m_frameSize, m_pImages[m_cursor].m_frameSize);

	switch (m_cursor) {
	case 4:
		dest.y -= 4.0f;
		break;
	case 5:
		dest.y -= 28.0f;
		break;
	}

	if (GOLFDOC() && CSharedDoc::Instance()->m_roomType != 2 && GOLFDOC()->GameMode() == 0x200 && m_terrainToolTip && !((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false)) {
		int area = m_area;
		if (area == 0 || area == 0x22) {
			if (m_pGroundPropt && !COption::Instance()->GetQuitWindowed())
				m_pGroundPropt->DrawGroundTip();
		}
	}

	m_pImages[m_cursor].m_pOverlay->Render(g_view, WRect(m_frame * (1.0f / m_pImages[m_cursor].m_width) * dest.w, 0.0f, dest.w * (1.0f / m_pImages[m_cursor].m_width), dest.h / m_pImages[m_cursor].m_height), dest, 0x2080000, 0xFFFFFFFF, 0.0f, 0);

	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	if (m_cursorOverride && CMessengerInfo::Instance()->IsVisible() && CMessengerInfo::Instance()->m_rcMessengerDlg.IsInRect(m_pointer.x, m_pointer.y)) {
		const Bitmap* bitmap = g_pFresh->GetManager()->GetBitmap("MESSENGER", "mouse_right_btn");
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(m_pointer.x + 2.0f, m_pointer.y + 12.0f, (float)bitmap->Width(), (float)bitmap->Height()), 0xFFFFFFFF, 0);
	} else if (m_cursorOverride) {
		const Bitmap* bitmap = g_pFresh->GetManager()->GetBitmap("NEW_ICON", "mouse_right_button");
		if (bitmap)
			gdi->DrawTexture(bitmap, WRect(m_pointer.x + 2.0f, m_pointer.y + 12.0f, (float)bitmap->Width(), (float)bitmap->Height()), 0xFFFFFFFF, 0);
	}
}

// mousecursor.cpp:1085
void CMouseCursor::SetActive(bool active) {
	if (active) {
		m_pointer = m_storedPointer;
		m_hoverTime = 0;
		m_cursor = 0;
		g_input->SetMousePoint(m_pointer.x, m_pointer.y);
		m_visible = true;
	} else {
		m_storedPointer = m_pointer;
		m_area = 0;
		m_cursor = 0;
	}

	m_active = active;
}

// mousecursor.cpp:1113
void CMouseCursor::Store(void) {
	m_storedPointer = m_pointer;
}

// mousecursor.cpp:1118
bool CMouseCursor::InArea(const WRect& rect) {
	if (CQuitWindow::Instance() && CQuitWindow::Instance()->IsActive())
		return false;

	if (m_pointer.x > rect.x && m_pointer.x < rect.x + rect.w && m_pointer.y > rect.y && m_pointer.y < rect.y + rect.h) {
		m_cursor = 1;
		return true;
	}

	return false;
}

// mousecursor.cpp:1132
void CMouseCursor::SetCamMode(bool camMode) {
	m_camMode = camMode;
	if (camMode) {
		ForceMove(g_view->GetWidth() * 0.65f, g_view->GetHeight() - 150.0f, 0.13f);
		m_visible = true;
		m_hoverTime = 0;
		m_cursor = 0;
	}
}

// mousecursor.cpp:1145
void CMouseCursor::ForceMove(float x, float y, float factor) {
	m_visible = true;
	m_hoverTime = 0;

	if (factor < 0.0f) {
		g_input->SetMousePoint(x, y);
		m_pointer.x = x;
		m_pointer.y = y;
	} else {
		m_moveTarget = WVector2D(x, y);
		m_moveDelta = WVector2D(m_pointer.x - m_moveTarget.x, m_pointer.y - m_moveTarget.y);
		m_moveRate = 1.0f;
		m_moveSpeed = 1.0f / factor;
	}

	m_cursor = 0;
	m_area = 0;
}

// mousecursor.cpp:1176
int CMouseCursor::GetCursor(void) {
	if (m_cursorOverride)
		return 7;

	return m_cursor;
}

// mousecursor.cpp:1184
void CMouseCursor::SetCursor(int cursor) {
	if (cursor < 12) {
		m_cursorOverride = false;
	} else {
		cursor = 1;
		m_cursorOverride = true;
	}
	m_cursor = cursor;
}

// mousecursor.cpp:1201
CGroundPropt::CGroundPropt(const char* name) {
	m_pGroundTip = new CSimpleUI(name);
}

// mousecursor.cpp:1206
CGroundPropt::~CGroundPropt(void) {
	if (m_pGroundTip) { delete m_pGroundTip; m_pGroundTip = 0; }
}

// mousecursor.cpp:1211
void CGroundPropt::SelectGroundTip(void) {
	WVector mouse = g_input->GetMousePoint();
	WVector from, to;
	g_view->GetScreen2World(mouse.x, mouse.y, from, to);
	WVector point;
	point = GetPVS().GetRayIntersection(from, to);

	if (GOLFDOC()->m_pPolySoup->IsOutOfBound(point)) {
		strcpy(m_groundType, "OB");
		return;
	}

	CGolfBall::eWater water = (CGolfBall::eWater)0;
	switch (GetPVS().GetGroundType(point, false, &water)) {
	case 1:
		strcpy(m_groundType, "Fairway");
		break;
	case 4:
		strcpy(m_groundType, "Rough");
		break;
	case 6:
		strcpy(m_groundType, "Rough");
		break;
	case 2:
		strcpy(m_groundType, "Green");
		break;
	case 3:
		strcpy(m_groundType, "Bunker");
		break;
	case 13:
		strcpy(m_groundType, "Rough");
		break;
	case 7:
		strcpy(m_groundType, "Ice");
		break;
	case 8:
		strcpy(m_groundType, "Vector");
		break;
	case 9:
		strcpy(m_groundType, "Water");
		break;
	case 10:
		if (water == 0 || water == 1)
			strcpy(m_groundType, "OB");
		else
			strcpy(m_groundType, "WaterHazard");
		break;
	case 11:
		strcpy(m_groundType, "Sand");
		break;
	case 12:
		strcpy(m_groundType, "Rough");
		break;
	case 14:
		strcpy(m_groundType, "Water");
		break;
	default:
		strcpy(m_groundType, "Rough");
		break;
	}
}

// mousecursor.cpp:1300
void CGroundPropt::DrawGroundTip(void) {
	if (m_pGroundTip) {
		m_pGroundTip->SetColor(0xFFFFFFFF);

		float x = g_input->GetMousePoint().x + 3.0f;
		float y = g_input->GetMousePoint().y + 10.0f;

		if (x >= g_view->GetWidth() - 90.0f)
			x = g_view->GetWidth() - 90.0f;
		if (y >= g_view->GetHeight() - 25.0f)
			y = g_view->GetHeight() - 25.0f;

		m_pGroundTip->Render(m_groundType, x, y, 1.0f, 0);
	}
}
