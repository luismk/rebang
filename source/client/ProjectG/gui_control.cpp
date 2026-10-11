// Original: d:\build\custom\temp\code\client\projectg\gui_control.cpp
// The original holds the out-of-line copy of CMouseCursor::IsVisible and
// IsCamMode (mousecursor.h) in this object.
#define GUI_CONTROL_MOUSECURSOR_INLINE

#include "gui_control.h"
#include "fredit.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "fontinfo.h"
#include "SimpleOverlay.h"
#include "wresrcmng.h"
#include "wview.h"
#include "mousecursor.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "ChatMsg.h"
#include "frbutton.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "ReportDlg.h"
#include "../shared/classdefine.h"
#include "Packet.h"
#include "TaskManager.h"
#include "CapturedBg.h"
#include "InputManager.h"
#include "QuitWindow.h"
#include "autotarget.h"
#include "golfball.h"
#include "club.h"
#include "wind.h"
#include "s5/Utilities.h"
#include "s5/classicserver/ClassicServer.h"
#include "commonutil.h"
#include "fxinterpreter.h"
#include "wfont.h"
#include "ClientSetting.h"

// The wind font's page file (tagWTITLEFONT::filename of CScreen's title font).
static char* s_windFontFile = "[font_wind.jpg";

// gui_control.cpp:45 (run-time type and factory registration of CScreen)
const WRTTI CScreen::m_RTTI("CScreen", &IActor::m_RTTI);

IObject* CScreenMakeInstance(void) {
	return new CScreen;
}

struct __impCScreen {
	__impCScreen(void) {
		ObjectFactory().AddObjectFunctor(CScreenMakeInstance, "CScreen");
		ActorFactory().AddActorClass("CScreen");
	}
};
static __impCScreen s_impCScreen;

// gui_control.cpp:154
void CScreen::OnPreLoadInit(void) {

	m_unknown3E1 = true; m_unknown3E3 = false;
	m_unknown3E2 = false;
}

// gui_control.cpp:610
void CScreen::OnInit(void) {
	m_bannerTime = 0;


	m_hitBarCount = 10;
	m_hitBarFrom = 0;
	m_hitBarTime = 0;
	m_selectedIndex = -1;
	m_turnStarted = false;
	m_turnTime = 0;

	m_accumValue = 0;
	m_fadeValue = 0;
	m_blinkTime = 0;
	m_hudVisible = true;
	m_clubAngle = 6.2831855f;
	m_clubSpinDir = 0;
	m_clubSwing = 0;
	m_playerCount = 0;

	m_msgColorTime = 0;
	m_msgColor = 0;

	m_cammodeAlpha = 0;

	m_drawFlags |= 2;

	unsigned long clubSetTypeId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_clubSetTypeId;
	sClubSet* pClubSet = GolfClub().FindClubSet(clubSetTypeId);
	WPuppet* pPuppet = NULL;
	if (pClubSet) {

		pPuppet = pClubSet->m_puppet[1][GolfClub().GetType()];
	}
	m_clubBoundingBox = GetFxInterpreter()->ParseBoundingBoxScript_fx(pPuppet, FxInterpreter::SCRIPT_FORMAT_TEXT);


	if (m_pTitleFont == NULL) {
		tagWTITLEFONT info;
		memset(&info, 0, sizeof(info));
		info.filename = &s_windFontFile;
		info.fontw = 16;
		info.fonth = 16;
		info.numPages = 1;
		info.texw = 128;
		info.texh = 64;
		info.pCharSet = "1234567890ym%-.?/:";
		m_pTitleFont = g_resrcmng->GetTitleFont();
		m_pTitleFont->Create(&info);
	}

	if (m_pMissionUI == NULL) {
		m_pMissionUI = new CSimpleUI("text_mission.png");
		m_pMissionUI->Add("\xb9\xcc\xbc\xc7\xbe\xf8\xc0\xbd", WRect(91.0f, 10.0f, 232.0f, 100.0f), false);
		m_pMissionUI->Add("\xb9\xcc\xbc\xc7\xc0\xd6\xc0\xbd", WRect(50.0f, 130.0f, 256.0f, 130.0f), false);
	}
}

// gui_control.cpp:1139
bool CScreen::OnTrainingOptionDlgResult(int ret, FrForm* pForm) {
	CMouseCursor::Instance()->m_trainingOptionEnable = true;
	return true;
}

// gui_control.cpp:1405
void CScreen::AddOverlay(const char* name, const char* file, int flags) {
	sOverlay* overlay = new sOverlay;
	overlay->m_pOverlay = g_resrcmng->GetOverlay(file, flags);
	if (overlay->m_pOverlay) {
		strcpy(overlay->m_name, name);
		m_overlays.AddItem(overlay, overlay->m_name);
	} else {
		delete overlay;
	}
}

// gui_control.cpp:1418
void CScreen::DrawOverlay(const char* name, const WRect& rect, unsigned char alpha, int flags) {
	if (alpha) {
		sOverlay* overlay = m_overlays.Find(name);
		if (overlay && overlay->m_pOverlay)
			overlay->m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), rect, flags, (alpha << 24) | 0xFFFFFF, 0.0f, 0);
	}
}

// gui_control.cpp:1429
void CScreen::DrawOverlay2(const char* name, const WRect& rect, int color, int flags) {
	if (color & 0xFF000000) {
		sOverlay* overlay = m_overlays.Find(name);
		if (overlay && overlay->m_pOverlay)
			overlay->m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), rect, flags, color, 0.0f, 0);
	}
}

// gui_control.cpp:2044
CMiniScore::CMiniScore(void)
	: m_state(1)
	, m_alpha(0) {
	m_pOverlay04 = NULL; m_pOverlay1C = NULL; m_pOverlay20 = NULL; m_stateTime = 0; memset(m_pCharBitmap, 0, sizeof(m_pCharBitmap));

	for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (PLAYER(i)->m_state == 3)
			continue;

		CPartTidList& tidList = GOLFDOC()->GetPartTidList(i);

		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(tidList.m_tidChar);

		if (pChar)
			m_pCharBitmap[i] = g_pFresh->GetManager()->GetBitmap("ITEMS", pChar->m_icon);
	}

	m_pVsIcon = g_pFresh->GetManager()->GetBitmap("golf", "pc_icon_vs");

	m_pOverlay20 = g_resrcmng->GetOverlay("[nplayer.jpg", 0);
	m_pOverlay1C = g_resrcmng->GetOverlay("info_hp.jpg", 0);

	m_pOverlay24 = g_resrcmng->GetOverlay("[best_record_l.png", 0);
	m_pOverlay28 = g_resrcmng->GetOverlay("[best_record_r.png", 0);

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1)
		m_pOverlay04 = g_resrcmng->GetOverlay("[info_2p.jpg", 0);
	else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7)
		m_pOverlay04 = g_resrcmng->GetOverlay("[skins_info_1p.jpg", 0);
	else
		m_pOverlay04 = g_resrcmng->GetOverlay("[info_1p.jpg", 0);

	m_pFont = WSingleton<CChatMsg>::Instance()->GetMaskedFont();

	m_pShotCountInfo = new CShotCountInfo;

	m_pSimpleUI = new CSimpleUI("[hole_out.jpg");
	m_pSimpleUI->Add("HoleOut", WRect(0.0f, 0.0f, 128.0f, 32.0f), false);
	m_pSimpleUI->Add("GiveUp", WRect(0.0f, 32.0f, 128.0f, 32.0f), false);
	m_pSimpleUI->SetAngle(-0.52359879f);

	m_iconUV = WRect(0.0f, 0.0f, 0.46875f, 1.0f);
	m_iconWidth = 120.0f;
	m_iconHeight = (WSingleton<CSharedDoc>::Instance()->m_roomType == 7 ? 128.0f : 64.0f) * (WSingleton<CSharedDoc>::Instance()->m_roomType == 1 ? 2.0f : 1.0f);

	m_offsetY = 0;
	m_posX = g_view->GetWidth() - 120.0f;
	m_gameMode = 0;
}

// gui_control.cpp:2105
CMiniScore::~CMiniScore(void) {
	if (m_pOverlay04 != NULL)
	{
		g_resrcmng->Release(m_pOverlay04);
		m_pOverlay04 = NULL;
	}

	if (m_pOverlay1C != NULL)
	{
		g_resrcmng->Release(m_pOverlay1C);
		m_pOverlay1C = NULL;
	}

	if (m_pOverlay20 != NULL)
	{
		g_resrcmng->Release(m_pOverlay20);
		m_pOverlay20 = NULL;
	}

	if (m_pOverlay24 != NULL)
	{
		g_resrcmng->Release(m_pOverlay24);
		m_pOverlay24 = NULL;
	}

	if (m_pOverlay28 != NULL)
	{
		g_resrcmng->Release(m_pOverlay28);
		m_pOverlay24 = NULL;   // the original clears 0x24 here
	}

	if (m_pShotCountInfo) { delete m_pShotCountInfo; m_pShotCountInfo = NULL; }
	if (m_pSimpleUI) { delete m_pSimpleUI; m_pSimpleUI = NULL; }
}

// gui_control.cpp:2141
void CMiniScore::ChangeCharacter(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4: case 5: case 6: case 9: case 10: {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_characterInfo.typeId);
		if (pChar)
			m_pCharBitmap[0] = g_pFresh->GetManager()->GetBitmap("ITEMS", pChar->m_icon);
		else
			m_pCharBitmap[0] = NULL;
		break;
	}
	default:
		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_characterInfo.typeId);

			if (pChar)
				m_pCharBitmap[i] = g_pFresh->GetManager()->GetBitmap("ITEMS", pChar->m_icon);
			else
				m_pCharBitmap[i] = NULL;
		}
		break;
	}
}

// gui_control.cpp:2168
WRect CMiniScore::GetIconRect(int flags) {
	float x = (flags & 2) ? g_view->GetWidth() - 120.0f : 0.0f;
	float y = (flags & 1) ? 124.0f : 30.0f;
	WSize size(m_iconWidth, m_iconHeight);

	return WRect(WPoint(x, y), size);
}

// gui_control.cpp:2179
void CMiniScore::Process(float elapsed) {
	if (m_gameMode != GOLFDOC()->GameMode()) {
		if ((m_gameMode & ~0x38) && ((GOLFDOC()->GameMode() & 8) || (WSingleton<CSharedDoc>::Instance()->m_roomType == 7 && (GOLFDOC()->GameMode() & 0x1000)))) {
			m_state = 2; m_stateTime = 0; m_offsetY = 0; m_posX = g_view->GetWidth() - 120.0f; m_alpha = 0;
		}
		else if ((GOLFDOC()->GameMode() & ~0x3A) && (m_gameMode & 0x2A)) {
			m_state = 3;
			m_stateTime = 0;
			m_alpha = 255;
		}

		m_gameMode = GOLFDOC()->GameMode();
	}

	if (m_state == 0) {
		if (m_pShotCountInfo)
			m_pShotCountInfo->Process(elapsed);
	}

	if (m_state == 0 || m_state == 1)
		return;

	if (m_state == 2) {
		m_stateTime += elapsed * 2.0f;
		if (m_stateTime > 1.0f) {
			m_state = 0;
			m_stateTime = 1.0f;
		}

		m_alpha = (unsigned char)(m_stateTime * 255.0f);
	}
	else {
		m_stateTime += elapsed;
		if (m_stateTime > 1.0f) {
			m_state = 1;
			m_stateTime = 1.0f;
		}

		m_offsetY -= m_stateTime * 30.0f;
		m_posX += m_stateTime * 30.0f;
	}
}

// gui_control.cpp:2235
void CMiniScore::Flicker(void) {
	m_state = 0;
	m_stateTime = 1.0f;
	m_alpha = 255;
}

// gui_control.cpp:2845
void CMiniScore::ResetUiPosition(void) {
	if (m_state == 0 || m_state == 2)
		m_posX = g_view->GetWidth() - 120.0f;
}

// gui_control.cpp:2851
void CMiniScore::Show(void) {
	m_state = 2; m_stateTime = 0;
	m_offsetY = 0;
	m_posX = g_view->GetWidth() - 120.0f;
	m_alpha = 0;
}

// gui_control.cpp:2860
void CMiniScore::Hide(void) {
	m_state = 1;
	m_stateTime = 1.0f;
	m_offsetY = -120.0f;
	m_posX = g_view->GetWidth() - 120.0f + 120.0f;
	m_alpha = 255;
}

// gui_control.cpp:2869
void CMiniScore::StartFloating(void) {
	if (m_pShotCountInfo)
		m_pShotCountInfo->StartFloating();
}

// gui_control.cpp:2881
CAuxMenuUI::CAuxMenuUI(void)
	: m_pBase(NULL)
	, m_pDropDlg(NULL)
	, m_pDropFailDlg(NULL)
	, m_pChatFuncDlg(NULL) {
	m_pBase = g_resrcmng->GetOverlay("[option_base.png", 0x80000000);
	m_pItemOverlay[0] = g_resrcmng->GetOverlay("[option_black.png", 0x80000000);
	m_pItemOverlay[1] = g_resrcmng->GetOverlay("[option_white.png", 0x80000000);

	m_active = 0; m_fade = 0; m_unknown28 = 2;
}

// gui_control.cpp:2893
CAuxMenuUI::~CAuxMenuUI(void) {
	if (g_resrcmng && m_pBase) { g_resrcmng->Release(m_pBase); m_pBase = 0; }
	if (g_resrcmng && m_pItemOverlay[0]) { g_resrcmng->Release(m_pItemOverlay[0]); m_pItemOverlay[0] = 0; }
	if (g_resrcmng && m_pItemOverlay[1]) { g_resrcmng->Release(m_pItemOverlay[1]); m_pItemOverlay[1] = 0; }
}

// gui_control.cpp:2900
void CAuxMenuUI::Process(float elapsed) {
	m_hoverIndex = WSingleton<CMouseCursor>::Instance()->GetArea() - 0x27;

	switch (m_active) {
	case 0:
		if (WSingleton<CMouseCursor>::Instance()->GetArea() == 0x26 && g_input->GetButton(LEFT_BUTTON) == 2) {

			m_active = 1;
		}
		break;
	case 1:
		m_fade += elapsed * 5.0f;
		if (m_fade > 1.0f) {
			m_fade = 1.0f;
			m_active = 2;

			WSingleton<CMouseCursor>::Instance()->m_uiEnable[0] = true;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[1] = true;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[2] = true;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[3] = true;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[4] = true;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[5] = true;
		}
		break;
	case 2:
		if (m_hoverIndex < 0 || m_hoverIndex > 5) {

			WSingleton<CMouseCursor>::Instance()->m_uiEnable[0] = false;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[1] = false;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[2] = false;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[3] = false;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[4] = false;
			WSingleton<CMouseCursor>::Instance()->m_uiEnable[5] = false;
			m_active = 3;
		}
		else if (g_input->GetButton(LEFT_BUTTON) == 2) {
			switch (m_hoverIndex) {
			case 0:
				if (WSingleton<CQuitWindow>::Instance())
					WSingleton<CQuitWindow>::Instance()->CreateOptionWindow();
				break;
			case 1:
				AfxGetTask()->GetActor("GuideBook") << MsgObject(0, 0x12A, 0, 0, 0, 0, 0);
				break;
			case 2:
				SetUnplayable();
				break;
			case 3:
				SetPause();
				break;
			case 4:
				OpenChatRecord();
				break;
			}
		}
		break;
	case 3:
		m_fade -= elapsed * 5.0f;
		if (m_fade < 0.0f) {
			m_fade = 0;
			m_active = 0;
		}
		break;
	}
}

// gui_control.cpp:3032
void CAuxMenuUI::SetPause(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) { case 4: case 5: case 6: case 9: case 10: return; } if (WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit == 0) return;


	WSingleton<CMouseCursor>::Instance()->m_uiEnable[3] = false;

	if (m_unknown28 == 0) {
		// "Pausing the game can be used only twice per game."
		AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x23, (intptr_t)"\xB0\xD4\xC0\xD3\xC1\xA4\xC1\xF6\xB4\xC2 \xC7\xD1 \xB0\xD4\xC0\xD3\xBF\xA1 2\xB9\xF8\xB8\xB8 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}

	m_unknown28--;

	if (OnlinePlay()) {
		WSendPacket packet((enumClientPacket)0x30);
		packet.Encode1(1);
		packet.Send((eSendTo)0);
	}
	else {
		// "The game is paused.\nPress OK to continue the game."
		AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x84, 1, (intptr_t)"\xC7\xF6\xC0\xE7 \xB0\xD4\xC0\xD3\xC0\xCC \xC0\xE1\xBD\xC3 \xC1\xDF\xB4\xDC\xB5\xC8 \xBB\xF3\xC5\xC2\xC0\xD4\xB4\xCF\xB4\xD9.\n\xB0\xD4\xC0\xD3\xC0\xBB \xB0\xE8\xBC\xD3\xC7\xCF\xB1\xE2 \xC0\xA7\xC7\xD8\xBC\xAD\xB4\xC2 \xC8\xAE\xC0\xCE\xC0\xBB \xB4\xAD\xB7\xAF\xC1\xD6\xBC\xBC\xBF\xE4.", 0, 0, 0));
	}
}

// gui_control.cpp:3059
void CAuxMenuUI::OpenChatRecord(void) {
	if (OnlinePlay()) {
		if (m_pChatFuncDlg == NULL) {
			m_pChatFuncDlg = CreateForm<FrChatFuncDlg>(g_pFresh->GetManager(), this, "chat_func", 0);
			m_pChatFuncDlg->Open((FRESH_RESULT_CALLBACK)&CAuxMenuUI::OnChatFuncDlgResult, 5);
		}
	}
}

// gui_control.cpp:3071
bool CAuxMenuUI::OnDropDlgResult(int ret, FrForm* pForm) {
	if (ret == 1) {

		if (OnlinePlay()) {
			WSendPacket packet((enumClientPacket)0x19);
			WVector pos = GOLFDOC()->m_pAutoTarget->GetDropPos();
			packet.EncodeBuffer(&pos, sizeof(WVector));
			packet.Send((eSendTo)0);
		}
		else {
			GolfBall().m_pos = GOLFDOC()->m_pAutoTarget->GetDropPos();
			AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x8D, 0, 0, 0, 0, 0));
		}
	}

	WSingleton<CMouseCursor>::Instance()->m_uiEnable[2] = true;

	m_pDropDlg = NULL;
	return true;
}

// gui_control.cpp:3094
bool CAuxMenuUI::OnDropFailDlgResult(int ret, FrForm* pForm) {
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[2] = true;

	m_pDropFailDlg = NULL;
	return true;
}

// gui_control.cpp:3102
void CAuxMenuUI::Display(void) {
	if (!CMouseCursor::Instance()->IsVisible())
		return;

	unsigned long color;
	if (m_active == 0) {
		switch (CMouseCursor::Instance()->GetArea()) {
		case 0x25:
			color = 0x80FFFFFF;
			break;
		case 0x26:
			color = 0xFFFFFFFF;
			break;
		default:
			color = 0x80FFFFFF;
			break;
		}
	}
	else
		color = 0xFFFFFFFF;

	if (m_pBase)
		m_pBase->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(g_view->GetWidth() - 128.0f + (1.0f - m_fade) * 79.0f, g_view->GetHeight() * 0.5f - 66.0f, 128.0f, 256.0f), 0x2000000, color, 0.0f, 0);

	for (int i = 0; i < 6; i++) {
		if (m_pItemOverlay[i == m_hoverIndex || i == m_index])
			m_pItemOverlay[i == m_hoverIndex || i == m_index]->Render(g_view, WRect(0.0f, i * 0.0859375f, 1.0f, 0.0859375f), WRect(g_view->GetWidth() - m_fade * 79.0f, g_view->GetHeight() * 0.5f + (i - 3) * 22.0f, 128.0f, 22.0f), 0, 0xFFFFFFFF, 0.0f, 0);
	}
}

// gui_control.cpp:3139
bool CAuxMenuUI::OnChatFuncDlgResult(int ret, FrForm* pForm) {
	m_pChatFuncDlg = NULL;
	return true;
}

// gui_control.cpp:3145
void CAuxMenuUI::SetActive(bool active, int index) {
	if (active) {
		m_index = index; m_active = 1;

		WSingleton<CMouseCursor>::Instance()->m_uiEnable[index] = true;
		WSingleton<CMouseCursor>::Instance()->ForceMove(g_view->GetWidth() - 50.0f, ((index - 3) + 0.5f) * 22.0f + g_view->GetHeight() * 0.5f, -1.0f);
	} else {
		m_index = -1;
	}
}

// gui_control.cpp:3161
void CAuxMenuUI::ChangeTurn(void) {
	m_index = -1; m_active = 0; m_fade = 0;

	WSingleton<CMouseCursor>::Instance()->m_uiEnable[0] = false;
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[1] = false;
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[2] = false;
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[3] = false;
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[4] = false;
	WSingleton<CMouseCursor>::Instance()->m_uiEnable[5] = false;
}

// gui_control.cpp:3183
CDrivingDistInfo::CDrivingDistInfo(void)
	: m_pPlayerOverlay(NULL)
	, m_pTopOverlay(NULL)
	, m_unknown08(true)
	, m_unknown09(false)
	, m_bestPlayer(0xFF)
	, m_unknown0B(0xFF)
	, m_bestColor(0xFFFFFFFF)
	, m_myColor(0xFFFFFFFF)
	, m_shownDistance(0)
	, m_bestDistance(0)
	, m_myDistance(0) {
	m_pPlayerOverlay = g_resrcmng->GetOverlay("[nplayer.jpg", 0);
	m_pTopOverlay = g_resrcmng->GetOverlay("[top_drv.jpg", 0);
}

// gui_control.cpp:3189
CDrivingDistInfo::~CDrivingDistInfo(void) {
	if (g_resrcmng && m_pPlayerOverlay) { g_resrcmng->Release(m_pPlayerOverlay); m_pPlayerOverlay = 0; }
	if (g_resrcmng && m_pTopOverlay) { g_resrcmng->Release(m_pTopOverlay); m_pTopOverlay = 0; }
}

// gui_control.cpp:3309
void CDrivingDistInfo::Display(WFont* pFont) {
	char text[32];
	WRect rect;
	WRect uv;

	float y = WSingleton<COption>::Instance()->gGetBar_Y();

	WOverlay::DrawBox(g_view, WRect(27.0f, y + 4.0f, 62.0f, 1.0f), 0, 0xFFEEEEEE, 0.001f);

	if (GOLFDOC()->GameMode() != 0x80 && m_unknown09) {
		uv.y = 0.0f; uv.x = 0.0f; uv.h = 1.0f; uv.w = 1.0f;
		rect.x = 27.0f; rect.y = y - 28.0f; rect.h = 64.0f; rect.w = 64.0f;

		m_pTopOverlay->Render(g_view, uv, rect, 0, 0xFFFFFFFF, 0.0f, 0);
	}
	else {
		sprintf(text, "%.2fy", m_bestDistance);
		float width = pFont->GetTextWidth(g_view, text);
		float x;
		x = 57.0f - (int)(width * 0.5f);
		pFont->Print(g_view, x, y - 9.0f, text, 0, m_bestColor, 0);

		sprintf(text, "%.2fy", m_myDistance);
		width = pFont->GetTextWidth(g_view, text);
		x = 57.0f - (int)(width * 0.5f);
		pFont->Print(g_view, x, y + 7.0f, text, 0, m_myColor, 0);

		uv.w = 0.5f;
		uv.h = 0.25f;
		rect.x = 41.0f;
		rect.w = 32.0f;
		rect.h = 16.0f;
		if (m_bestPlayer != 0xFF) {
			uv.x = (m_bestPlayer & 1) ? 0.5f : 0.0f;
			uv.y = (m_bestPlayer & 2) ? 0.25f : 0.0f;
			rect.y = y - 25.0f;
			if (m_pPlayerOverlay) {
				m_pPlayerOverlay->SetClippingArea(&rect);
				m_pPlayerOverlay->Render(g_view, uv, rect, 0, PLAYER(m_bestPlayer)->m_color | 0xFF000000, 0.0f, 0);
			}
		}

		uv.x = (MyIndex() & 1) ? 0.5f : 0.0f;
		uv.y = (MyIndex() & 2) ? 0.25f : 0.0f;
		rect.y = y + 17.0f;
		if (m_pPlayerOverlay) {
			m_pPlayerOverlay->SetClippingArea(&rect);
			m_pPlayerOverlay->Render(g_view, uv, rect, 0, PLAYER(MyIndex())->m_color | 0xFF000000, 0.0f, 0);
		}

		pFont->Flush(g_view);
	}
}

// gui_control.cpp:3370 (run-time type and factory registration of FrChatFuncDlg)
const WRTTI FrChatFuncDlg::m_RTTI("FrChatFuncDlg", &FrForm::m_RTTI);

IObject* FrChatFuncDlgMakeInstance(void) {
	return new FrChatFuncDlg;
}

struct __sFrChatFuncDlg {
	__sFrChatFuncDlg(void) {
		ObjectFactory().AddObjectFunctor(FrChatFuncDlgMakeInstance, "FrChatFuncDlg");
	}
};
static __sFrChatFuncDlg s_registerFrChatFuncDlg;

// gui_control.cpp:3372
const sFRESH_MSGMAP* FrChatFuncDlg::GetMessageMap(void) const {
	return &FrChatFuncDlg::_MsgMap;
}

sFRESH_MSGMAP FrChatFuncDlg::_MsgMap = { &FrForm::_MsgMap, &FrChatFuncDlg::_MsgEntries[0] };

sFRESH_ENTRY FrChatFuncDlg::_MsgEntries[] = {
	// gui_control.cpp:3374
	{ "chatview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChatFuncDlg::OnChatViewInit },
	{ "hidechat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChatFuncDlg::OnHideChatInit },
	{ "showchat", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChatFuncDlg::OnShowChatInit },

	// gui_control.cpp:3378
	{ "hidechat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChatFuncDlg::OnBtnHideChatUp },
	{ "showchat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChatFuncDlg::OnBtnShowChatUp },
	{ "savechat", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChatFuncDlg::OnBtnSaveChatUp },
	{ "accuse", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChatFuncDlg::OnBtnAccuseUp },
	{ "closedlg", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChatFuncDlg::OnBtnCloseDlgUp },

	// gui_control.cpp:3384
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// gui_control.cpp:3393
FrChatFuncDlg::FrChatFuncDlg(void)
	: m_pUnknown110(NULL)
	, m_pBtnShowChat(NULL)
	, m_pBtnHideChat(NULL)
	, m_pUnknown11C(NULL)
	, m_pUnknown120(NULL)
	, m_pChatView(NULL) {
}

// gui_control.cpp:3397
FrChatFuncDlg::~FrChatFuncDlg(void) {
}

// gui_control.cpp:3401
void FrChatFuncDlg::OnBtnShowChatUp(void) {
	if (m_pBtnHideChat) {
		m_pBtnHideChat->SetVisible(true);
	}
	if (m_pBtnShowChat) {
		m_pBtnShowChat->SetVisible(false);
	}

	GOLFDOC()->ShowChat(true);

	// "Turns the in-game chat window on."
	WSingleton<CChatMsg>::Instance()->AddChatMsg("\xB0\xD4\xC0\xD3\xB3\xBB \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xBB \xC8\xB0\xBC\xBA\xC8\xAD\xC7\xD5\xB4\xCF\xB4\xD9.", 0xFFFF7878, true, false);
}

// gui_control.cpp:3413
void FrChatFuncDlg::OnBtnHideChatUp(void) {
	if (m_pBtnHideChat) {
		m_pBtnHideChat->SetVisible(false);
	}
	if (m_pBtnShowChat) {
		m_pBtnShowChat->SetVisible(true);
	}

	GOLFDOC()->ShowChat(false);

	// "Hides the in-game chat window."
	WSingleton<CChatMsg>::Instance()->AddChatMsg("\xB0\xD4\xC0\xD3\xB3\xBB \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xBB \xBC\xFB\xB1\xE9\xB4\xCF\xB4\xD9.", 0xFFFF7878, true, false);
}

// gui_control.cpp:3425
void FrChatFuncDlg::OnBtnSaveChatUp(void) {
	int index = WSingleton<CSharedDoc>::Instance()->m_chatManager.SaveChatHistory(GOLFDOC()->m_chatHistoryIndex, 1);
	GOLFDOC()->m_chatHistoryIndex = index;
}

// gui_control.cpp:3431
bool FrChatFuncDlg::OnReportDlgResult(int ret, FrForm* pForm) {
	Close(FrOK, true);

	m_pReportDlg = NULL;
	return true;
}

// gui_control.cpp:3439
void FrChatFuncDlg::OnBtnAccuseUp(void) {
	m_pReportDlg = CreateForm<FrReportDlg>(g_pFresh->GetManager(), this, "report", 0);
	if (m_pReportDlg) {
		std::string msg;
		// "1. Earlier contents of the chat window are recorded on the server too."
		msg = "1. \xB4\xEB\xC8\xAD\xC3\xA2\xC0\xC7 \xC1\xF6\xB3\xAD \xB3\xBB\xBF\xEB\xB5\xB5 \xB8\xF0\xB5\xCE \xBC\xAD\xB9\xF6\xBF\xA1 \xB1\xE2\xB7\xCF\xB5\xCB\xB4\xCF\xB4\xD9.\n";
		// "2. Manipulated, false or repeated reports are punished."
		msg += "2. \xC1\xB6\xC0\xDB, \xC7\xE3\xC0\xA7, \xB9\xDD\xBA\xB9\xBD\xC5\xB0\xED\xB4\xC2 \xC3\xB3\xB9\xFA\xC0\xBB \xB9\xDE\xBD\xC0\xB4\xCF\xB4\xD9.\n";
		// "3. Reports are handled within 48 hours."
		msg += "3. \xBD\xC5\xB0\xED\xC7\xCF\xBD\xC5 \xB3\xBB\xBF\xEB\xC0\xBA 48\xBD\xC3\xB0\xA3 \xC0\xCC\xB3\xBB\xBF\xA1 \xC3\xB3\xB8\xAE\xB5\xCB\xB4\xCF\xB4\xD9.";

		m_pReportDlg->SetMessage(msg.c_str(), false);
		m_pReportDlg->Open((FRESH_RESULT_CALLBACK)&FrChatFuncDlg::OnReportDlgResult, 3);
	}
}

// gui_control.cpp:3455
void FrChatFuncDlg::OnBtnCloseDlgUp(void) {
	Close(FrOK, true);
}

// gui_control.cpp:3460
void FrChatFuncDlg::OnHideChatInit(intptr_t param) {
	m_pBtnHideChat = DYNAMIC_CAST(FrButton, param);
	if (m_pBtnHideChat) {


		if (GOLFDOC()->IsShowChat()) {

			m_pBtnHideChat->SetVisible(true);
		}
		else {

			m_pBtnHideChat->SetVisible(false);
		}
	}
}

// gui_control.cpp:3476
void FrChatFuncDlg::OnShowChatInit(intptr_t param) {
	m_pBtnShowChat = DYNAMIC_CAST(FrButton, param);
	if (m_pBtnShowChat) {


		if (GOLFDOC()->IsShowChat()) {

			m_pBtnShowChat->SetVisible(false);
		}
		else {

			m_pBtnShowChat->SetVisible(true);
		}
	}
}

// gui_control.cpp:3492
void FrChatFuncDlg::OnChatViewInit(intptr_t param) {
	m_pChatView = DYNAMIC_CAST(FrEdit, param);
	if (m_pChatView) {
		std::list<sChatLine>::iterator it;
		for (it = CSharedDoc::Instance()->m_chatLineList.begin(); it != CSharedDoc::Instance()->m_chatLineList.end(); ++it) {
			m_pChatView->AddLine((*it).m_text.c_str(), WSingleton<CChatMsg>::Instance()->ConvertToUIColor((*it).m_color), false);
		}
	}
}

// gui_control.cpp:3505
void FrChatFuncDlg::AddChatMsg(const char* msg, int color) {
	if (m_pChatView) {
		m_pChatView->AddLine(msg, color, false);
	}
}

// gui_control.cpp:3598
void CScreen::DrawCammodeInfo(void) {
	if (!WSingleton<CMouseCursor>::Instance()->IsCamMode())
		return;

	char text[16];
	char state[8];

	if (g_input->GetButton(LEFT_BUTTON))
		strcpy(state, "CLICK");
	else
		strcpy(state, "OVER");

	switch (WSingleton<CMouseCursor>::Instance()->GetArea()) {
	case 0x17:
		strcpy(text, MakeStr("LEFT_%s", state));
		break;
	case 0x18:
		strcpy(text, MakeStr("RIGHT_%s", state));
		break;
	case 0x19:
		strcpy(text, MakeStr("UP_%s", state));
		break;
	case 0x1A:
		strcpy(text, MakeStr("DOWN_%s", state));
		break;
	case 0x1B:
		if (GOLFDOC()->GameMode() & 0x8000)
			strcpy(text, "INACTIVE");
		else
			strcpy(text, MakeStr("FORWARD_%s", state));
		break;
	case 0x1C:
		if (GOLFDOC()->GameMode() & 0x8000)
			strcpy(text, "INACTIVE");
		else
			strcpy(text, MakeStr("BACKWARD_%s", state));
		break;
	default:
		if (GOLFDOC()->GameMode() == 0x8000)
			strcpy(text, "INACTIVE");
		else
			strcpy(text, "NORMAL");
		break;
	}

	if (m_pCammodeUI) {
		m_pCammodeUI->SetColor((WSingleton<CMouseCursor>::Instance()->IsVisible() || m_cammodeAlpha < 1.0f) ? 0xFFFFFFFF : 0x80FFFFFF);
		m_pCammodeUI->Render(text, g_view->GetWidth() * 0.65f, g_view->GetHeight() - 150.0f, m_cammodeAlpha, 0x22);
	}
}

// gui_control.cpp:3653
void CScreen::DrawWindInfo(void) {
	bool visible;

	SendMsg(NULL, "ShotReview", -1, (intptr_t)"GET_VISIBLE_STATE", (intptr_t)&visible, 0, 0);

	if (GolfClub().GetType() == 3) {
		if (!visible)
			return;
	}

	if (GOLFDOC()->m_gameState == 15 || (GOLFDOC()->m_gameState == 11 && m_unknown3E2)) {
		DrawOverlay2("\xb9\xd9\xb6\xf7\xc5\xab\xc3\xa2", WRect(g_view->GetWidth() - 114.0f, m_remainTimeY - 60.0f, 128.0f, 128.0f), 0xFFFFFFFF, 0);
		Wind().Display(m_pWindPuppet, g_view->GetWidth() - 50.0f, m_remainTimeY + 5.0f);
		DrawOverlay2("\xb9\xd9\xb6\xf7\xc0\xdb\xc0\xba\xc3\xa2", WRect(g_view->GetWidth() - 52.0f, m_remainTimeY + 1.0f, 64.0f, 64.0f), 0xFFFFFFFF, 0);

		if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty())) {
			S5::CLASSICSRV::eWINDPOWERSTEP step = S5::CLASSICSRV::GetWindPowerStep(Wind().GetGlobalIntensity());

			WRect rect(g_view->GetWidth() - 34.0f, m_remainTimeY + 19.0f, 32.0f, 32.0f);

			switch (step) {
			case S5::CLASSICSRV::WINDPOWERSTEP_WEAK:
				DrawOverlay("\xb9\xd9\xb6\xf7\xbc\xbc\xb1\xe2\xbe\xe0", rect, 0xFF, 0);
				break;
			case S5::CLASSICSRV::WINDPOWERSTEP_MEDIUM:
				DrawOverlay("\xb9\xd9\xb6\xf7\xbc\xbc\xb1\xe2\xc1\xdf", rect, 0xFF, 0);
				break;
			case S5::CLASSICSRV::WINDPOWERSTEP_STRONG:
				DrawOverlay("\xb9\xd9\xb6\xf7\xbc\xbc\xb1\xe2\xb0\xad", rect, 0xFF, 0);
				break;
			}
		}
		else {
			if (m_pWindInfo)
				m_pWindInfo->Display();
		}
	}
}

// gui_control.cpp:4143
void CScreen::DrawRemainTime(void) {
	// "remaining-time circle"
	sOverlay* overlay = m_overlays.Find("\xB3\xB2\xC0\xBA\xBD\xC3\xB0\xA3\xBF\xF8");

	if (WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		if (overlay && overlay->m_pOverlay)
			overlay->m_pOverlay->ArcClipRender(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(11.5f, m_remainTimeY + 4.0f - 46.5f, 93.0f, 93.0f), m_remainTimeArc, 5.0614548f, 0, m_remainTimeColor);
	}
	else {
		m_remainTimeArcShown = m_remainTimeArc;
		if (overlay && overlay->m_pOverlay)
			overlay->m_pOverlay->ArcClipRender(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(11.5f, m_remainTimeY + 4.0f - 46.5f, 93.0f, 93.0f), m_remainTimeArc, 5.0614548f, 0, m_remainTimeColor);
	}
}

// gui_control.cpp:4279
void CScreen::DrawCommandArrow(void) {
	if (GOLFDOC()->GameMode() & 0x60) {
		float y = 200.0f;
		std::string name;

		std::list<unsigned long>::iterator it = GOLFDOC()->m_commandArrows.begin();
		while (it != GOLFDOC()->m_commandArrows.end()) {
			if (*it == 0x10) {
				++it;
				continue;
			}

			switch (*it) {
			case 1: name = "UP"; break;
			case 2: name = "DOWN"; break;
			case 4: name = "LEFT"; break;
			case 8: name = "RIGHT"; break;
			case 0x40: name = "INVALID"; break;
			case 0x20: name = "INVALID"; y -= 22.0f; break;
			}

			++it;
			if (it != GOLFDOC()->m_commandArrows.end() && *it == 0x10)
				name += "_S";

			if (m_pCommandArrowUI)
				m_pCommandArrowUI->Render(name.c_str(), y, m_remainTimeY - 30.0f, 1.0f, 0);
			y += 22.0f;
		}
	}
}
