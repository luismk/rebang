// Original: d:\build\custom\temp\code\client\projectg\chatmsg.cpp
#include <string.h>

#include "NetworkSystem.h"
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "ClientSetting.h"
#include "ChatMsg.h"
#include "GolfDoc.h"
#include "GolfTask.h"
#include "RealMyRoomTask.h"
#include "commonutil.h"
#include "InputManager.h"
#include "LobbyTask.h"
#include "SharedDoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "avatar_task.h"
#include "../shared/Localize.h"
#include "winput.h"
#include "wresrcmng.h"
#include "woverlay.h"
#include "wfont.h"
#include "wview.h"
#include "../client/Fresh/include/fresh.h"
#include "../client/Fresh/include/frdesktop.h"
#include "../client/Fresh/include/frwnd.h"

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev *g_ime;
extern unsigned long g_CurrentTime;

// clientsetting.h:417
const char* COption::mGetMacroText(int index) {
	return m_option.m_macroText[index];
}

// chatmsg.cpp:69
CChatMsg::CChatMsg(void) : m_printed(false), m_caretMoveEnabled(true), m_bufLen(0), m_bufWidth(1000.0f), m_pMaskedFont(0), m_toggle(true) {
	for (int i = 0; i < 3; ++i)
		m_pOverlays[i] = 0;

	m_openState = 0;
	Init();
}

// chatmsg.cpp:88
CChatMsg::~CChatMsg(void) {
	for (int i = 0; i < 3; ++i) {
		if (m_pOverlays[i]) {
			g_resrcmng->Release(m_pOverlays[i]);
			m_pOverlays[i] = 0;
		}
	}

	if (g_resrcmng && m_pMaskedFont) {
		g_resrcmng->Release(m_pMaskedFont);
		m_pMaskedFont = 0;
	}
}

// chatmsg.cpp:124
void CChatMsg::Init(void) {
	InitFont();

	m_editFlag = false;
	m_active = false;
	m_insertMode = true;
	m_editState = 0;
	m_inputLength = m_cursorPos = m_inputWidth = m_cursorX = 0;
	m_copyLength = m_copyWidth = 0;
	m_selectedLength = 0;
	m_inputText[0] = '\0';
	m_compText[0] = '\0';

	m_pOverlays[0] = g_resrcmng->GetOverlay("{chat_l.png", 0);
	m_pOverlays[1] = g_resrcmng->GetOverlay("{chat_m.png", 0);
	m_pOverlays[2] = g_resrcmng->GetOverlay("{chat_r.png", 0);

	m_overlayDst[0] = WRect(0.0f, 0.0f, 0.54296875f, 1.0f);
	m_overlayDst[1] = WRect(0.0f, 0.0f, 1.0f, 1.0f);
	m_overlayDst[2] = WRect(0.0f, 0.0f, 1.0f, 1.0f);

	m_openSpeed = 3000.0f;
	m_backupUiAlpha = 0xFF;
	m_caretVisible = true;
	m_caretBlinkTime = 0.0f;

	ResetUiPosition();
}

// chatmsg.cpp:161
void CChatMsg::ResetUiPosition(void) {
	WRect clip;
	float x;
	float y;

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) {
		y = g_view->GetHeight() - 106.0f;
		x = 70.0f;
		clip.x = 70.0f;
		clip.y = g_view->GetHeight() - 106.0f;
	} else {
		x = 91.0f;
		y = COption::Instance()->gGetBar_Y() + 9.0f;
		clip.x = 91.0f;
		clip.y = COption::Instance()->gGetBar_Y() + 9.0f;
	}
	clip.w = 482.0f;
	clip.h = 32.0f;

	for (int i = 0; i < 3; ++i) {
		if (m_pOverlays[i])
			m_pOverlays[i]->SetClippingArea(&clip);
	}

	m_overlaySrc[0] = WRect(m_openState == 3 ? x : x - 462.0f, y, 139.0f, 32.0f);
	m_overlaySrc[2] = WRect(0.0f, y, 64.0f, 32.0f);
	m_overlaySrc[1] = WRect(m_overlaySrc[0].x + m_overlaySrc[0].w, y, 462.0f - m_overlaySrc[0].w - m_overlaySrc[2].w, 32.0f);
	m_overlaySrc[2].x = m_overlaySrc[1].x + m_overlaySrc[1].w;
}

// chatmsg.cpp:318
void CChatMsg::Reset(void) {
	g_ime->Reset();

	m_editFlag = false;
	m_editState = 0;
	m_inputLength = m_cursorPos = m_inputWidth = m_cursorX = 0;
	m_inputText[0] = m_compText[0] = '\0';
}

// chatmsg.cpp:339
void CChatMsg::InitFont(void) {
	m_pMaskedFont = g_resrcmng->GetFixedFont("fnt_\xb5\xb8\xbf\xf2\xc3\xbc_12.wft");
	if (m_pMaskedFont)
		m_pMaskedFont->SetMode(WFont::MASKED);
}

// chatmsg.cpp:389
bool CChatMsg::HasBeenPrinted(void) {
	return !m_toggle || m_printed;
}

// chatmsg.cpp:394
void CChatMsg::ReplaceBackupUiAlpha(void) {
	if (GOLFDOC()) {
		m_backupUiAlpha = GOLFDOC()->GetUIAlpha();
	}
}

// chatmsg.cpp:400
void CChatMsg::SetActive(bool active, bool reset, bool) {
	if (m_active == active)
		return;

	m_active = active;

	g_ime->SetActive(active);

	WSingleton<CInputManager>::Instance()->SetInputMode(active ? CInputManager::INPUT_MODE_CHAT : CInputManager::INPUT_MODE_NORMAL);

	if (!m_active && reset)
		Reset();
}

// chatmsg.cpp:416
void CChatMsg::ToggleInsertMode(void) {
	m_insertMode = !m_insertMode;
}

// chatmsg.cpp:421
void CChatMsg::Process(float) {
	if (!m_active)
		return;

	ProcessResultString();
	ProcessCompString();

	if (m_openState == 3) {
		m_caretBlinkTime += 0.01f;

		if (m_caretBlinkTime > (m_caretVisible ? 0.4f : 0.1f)) {
			m_caretVisible = !m_caretVisible;
			m_caretBlinkTime = 0.0f;
		}
	}
}

// chatmsg.cpp:573
// The chat macros: FUNC_1 .. FUNC_8 send the option's macro texts.
void CChatMsg::ProcessMacro(float) {
	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false)
		return;

	if (g_input->Get("LALT", true) || g_input->Get("RALT", true) || !OnlinePlay())
		return;

	std::string msg;

	CTask *pTask = AfxGetTask();

	for (int i = 0; i < 8; i++) {
		if (g_input->GetDown(MakeStr("FUNC_%d", i + 1), true)) {
			if (((__rtti_obj = pTask) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) : false)
				|| ((__rtti_obj = pTask) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
				WSingleton<CSharedDoc>::Instance()->m_chatManager.SendChatMessage(COption::Instance()->mGetMacroText(i), 0, false);

				if (!WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
					msg = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_name;
					msg += " : ";
					msg += COption::Instance()->mGetMacroText(i);
					AddChatMsg(msg, 0xFFFFCC00, false, false);
				}
			} else {
				WSingleton<CSharedDoc>::Instance()->m_chatManager.SendChatMessage2(COption::Instance()->mGetMacroText(i), 0, false);

				if (!WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
					msg = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname;
					msg += " : ";
					msg += COption::Instance()->mGetMacroText(i);

					AfxGetTask()->GetActor("Lobby") << MsgObject(0, 3, (intptr_t)msg.c_str(), 0xFFFFCC00, 0, 0, 0);
				}
			}
		}
	}
}

// chatmsg.cpp:653
void CChatMsg::ProcessResultString(void) {
	m_consolKeyCode = 0;

	int ch = g_ime->GetState(3, 0);

	if (ch == '\\')
		return;

	if (ch & 0x80) {
		if (!(g_ime->GetState(3, -2) & 0x80))
			m_editFlag = false;

		InsertWCharacter(ch);

		m_selectedLength = m_selectedWidth = 0;

		m_caretBlinkTime = 0.0f;
	} else {
		if (ch == 0)
			return;

		switch (ch) {
		case 8:
			m_consolKeyCode = ch;
			OnBackSpace();
			break;

		case 4:
			m_consolKeyCode = ch;
			if (m_inputText[0])
				OnMoveLeft();
			else if (m_openState == 3 && !g_pFresh->GetDesktop()->FindChildForm("emoticon"))
				Close(true, true);
			break;

		case 5:
			m_consolKeyCode = ch;
			if (m_inputText[0])
				OnMoveRight();
			else if (m_openState == 3 && !g_pFresh->GetDesktop()->FindChildForm("emoticon"))
				Close(true, true);
			break;

		case 6:
			m_consolKeyCode = ch;
			if (!m_inputText[0] && m_openState == 3 && !g_pFresh->GetDesktop()->FindChildForm("emoticon"))
				Close(true, true);
			break;

		case 7:
			m_consolKeyCode = ch;
			if (!m_inputText[0] && m_openState == 3 && !g_pFresh->GetDesktop()->FindChildForm("emoticon"))
				Close(true, true);
			break;

		case 26:
			m_cursorPos = m_inputLength;
			m_cursorX = m_inputWidth;
			break;

		case 12:
			m_cursorPos = 0;
			m_cursorX = 0;
			break;

		case 1:
			m_consolKeyCode = ch;
			OnDelete();
			break;

		default:
			if (ch >= 0x20 && ch <= 0x7F) {
				if (m_selectedLength)
					DeleteSelected();

				WriteCharacter(ch);
			}
			break;
		}
	}

	m_caretBlinkTime = 0.0f;
	m_caretVisible = true;
}

// chatmsg.cpp:876
void CChatMsg::ProcessCompString(void) {
	m_compChar = g_ime->GetState(3, -2);

	if (m_compChar == 0x5C) {
		m_compChar = 0;
		return;
	}

	if (m_compChar != 0) {
		float width = m_pMaskedFont->GetTextWidth(g_view, m_inputText);
		if (width > m_bufWidth || m_inputLength - m_selectedLength > m_bufLen - 2) {
			g_ime->Reset();

			if (m_selectedLength)
				DeleteSelected();

			m_caretBlinkTime = 0.0f;
			m_caretVisible = true;
			return;
		}

		if (m_compChar == m_editState)
			return;

		m_editState = m_compChar;
		m_compText[0] = (char)m_compChar;
		m_compText[1] = (char)(m_compChar >> 8);
		m_compText[2] = '\0';

		if (!m_editFlag) {
			m_editFlag = true;
			if (m_selectedLength)
				DeleteSelected();

			if (!m_insertMode && m_cursorX < m_inputWidth)
				DeleteCharacter();
		}

		m_caretBlinkTime = 0.0f;
		m_caretVisible = true;
	} else {
		if (m_editFlag) {
			memset(m_compText, 0, 2);
			m_editFlag = false;
		}

		m_editState = 0;
	}
}

// chatmsg.cpp:937
void CChatMsg::ProcessCompString_JP(void) {
	const char* comp = (const char*)g_ime->GetState(3, -2);

	if (!comp)
		return;

	strcpy(m_compText, comp);

	if (m_compText[0]) {
		const char* attr = (const char*)g_ime->GetState(3, -3);
		const char* clause = (const char*)g_ime->GetState(3, -4);

		memcpy(m_compAttr, attr, sizeof(m_compAttr));
		memcpy(m_compClause, clause, sizeof(m_compClause));
		m_compCursor = g_ime->GetState(3, -5);
		m_compDeltaStart = g_ime->GetState(3, -6);

		m_pMaskedFont->GetTextWidth(g_view, m_inputText);
		m_pMaskedFont->GetTextWidth(g_view, m_compText);

		if (!m_editFlag) {
			m_editFlag = true;

			if (m_selectedLength)
				DeleteSelected();

			if (!m_insertMode && m_cursorX < m_inputWidth)
				DeleteCharacter();
		}

		m_caretVisible = true;
		m_caretBlinkTime = 0.0f;
	} else {
		if (m_editFlag)
			m_editFlag = false;
	}
}

// chatmsg.cpp:1035
void CChatMsg::OnMoveLeft(void) {
	if (m_caretMoveEnabled) {
		if (m_cursorX > 0) {
			m_cursorPos -= m_charExtraBytes[m_cursorX-- - 1] + 1;
		}
	}
}

// chatmsg.cpp:1133
void CChatMsg::OnMoveRight(void) {
	if (m_caretMoveEnabled) {
		if (m_cursorX < m_inputWidth) {
			m_cursorPos += m_charExtraBytes[m_cursorX++] + 1;
		}
	}
}

// chatmsg.cpp:1224
void CChatMsg::OnMoveEnd(void) {
	m_cursorPos = m_inputLength;
	m_cursorX = m_inputWidth;
}

// chatmsg.cpp:1237
void CChatMsg::OnMoveHome(void) {
	m_cursorPos = 0;
	m_cursorX = 0;
}

// chatmsg.cpp:1250
void CChatMsg::OnDelete(void) {
	if (m_selectedLength) {
		DeleteSelected();
	} else if (m_cursorX < m_inputWidth) {
		DeleteCharacter();
	}
}

// chatmsg.cpp:1261
void CChatMsg::OnBackSpace(void) {
	if (m_selectedLength) {
		DeleteSelected();
		return;
	}

	if (m_cursorX == 0)
		return;

	int bytes = m_charExtraBytes[m_cursorX - 1] + 1;
	memmove(&m_inputText[m_cursorPos - bytes], &m_inputText[m_cursorPos], m_inputLength - m_cursorPos);
	memmove(&m_inputTextAttr[m_cursorPos - bytes], &m_inputTextAttr[m_cursorPos], m_inputLength - m_cursorPos);

	m_inputLength -= bytes;
	m_cursorPos -= bytes;
	memmove(&m_charExtraBytes[m_cursorX - 1], &m_charExtraBytes[m_cursorX], m_inputWidth - m_cursorX);
	m_inputWidth--;
	m_cursorX--;
}

// chatmsg.cpp:1286
void CChatMsg::DeleteCharacter(void) {
	int bytes = m_charExtraBytes[m_cursorX] + 1;

	memmove(&m_inputText[m_cursorPos], &m_inputText[m_cursorPos + bytes], m_inputLength - m_cursorPos - bytes);
	memmove(&m_inputTextAttr[m_cursorPos], &m_inputTextAttr[m_cursorPos + bytes], m_inputLength - m_cursorPos - bytes);

	m_inputLength -= bytes;
	memmove(&m_charExtraBytes[m_cursorX], &m_charExtraBytes[m_cursorX + 1], m_inputWidth - m_cursorX - 1);
	m_inputWidth--;
}

// chatmsg.cpp:1355
void CChatMsg::DeleteSelected(void) {
	m_inputWidth -= m_selectedWidth;
	m_inputLength -= m_selectedLength;

	if (m_selBeginPos < m_selEndPos) {
		memmove(&m_inputText[m_selBeginPos], &m_inputText[m_selEndPos], m_selectedLength);
		memmove(&m_inputTextAttr[m_selBeginPos], &m_inputTextAttr[m_selEndPos], m_selectedLength);

		memmove(&m_charExtraBytes[m_selBeginX], &m_charExtraBytes[m_selEndX], m_selectedWidth);

		m_cursorPos = m_selBeginPos;
		m_cursorX = m_selBeginX;
	} else {
		memmove(&m_inputText[m_selEndPos], &m_inputText[m_selBeginPos], m_selectedLength);
		memmove(&m_inputTextAttr[m_selEndPos], &m_inputTextAttr[m_selBeginPos], m_selectedLength);

		memmove(&m_charExtraBytes[m_selEndX], &m_charExtraBytes[m_selBeginX], m_selectedWidth);
		m_cursorPos = m_selEndPos;
		m_cursorX = m_selEndX;
	}

	m_selectedLength = m_selectedWidth = 0;
}

// chatmsg.cpp:1382
void CChatMsg::CopySelected(void) {
	if (m_selBeginPos < m_selEndPos) {
		memmove(m_copyText, &m_inputText[m_selBeginPos], m_selectedLength);
		memmove(m_copyTextAttr, &m_inputTextAttr[m_selBeginPos], m_selectedLength);
		memmove(m_copyCharExtraBytes, &m_charExtraBytes[m_selBeginX], m_selectedWidth);
	} else {
		memmove(m_copyText, &m_inputText[m_selEndPos], m_selectedLength);
		memmove(m_copyTextAttr, &m_inputTextAttr[m_selEndPos], m_selectedLength);
		memmove(m_copyCharExtraBytes, &m_charExtraBytes[m_selEndX], m_selectedWidth);
	}

	m_copyLength = m_selectedLength;
	m_copyWidth = m_selectedWidth;
}

// chatmsg.cpp:1400
void CChatMsg::PasteCopied(void) {
	float inputWidth = m_pMaskedFont->GetTextWidth(g_view, m_inputText);
	float copyWidth = m_pMaskedFont->GetTextWidth(g_view, m_copyText);

	if (inputWidth + copyWidth > m_bufWidth || m_inputLength + m_copyLength >= m_bufLen)
		return;

	memmove(&m_inputText[m_cursorPos + m_copyLength], &m_inputText[m_cursorPos], m_inputLength - m_cursorPos);
	memmove(&m_inputTextAttr[m_cursorPos + m_copyLength], &m_inputTextAttr[m_cursorPos], m_inputLength - m_cursorPos);
	memmove(&m_charExtraBytes[m_cursorX + m_copyWidth], &m_charExtraBytes[m_cursorX], m_inputWidth - m_cursorX);

	memmove(&m_inputText[m_cursorPos], m_copyText, m_copyLength);
	memmove(&m_inputTextAttr[m_cursorPos], m_copyTextAttr, m_copyLength);
	memmove(&m_charExtraBytes[m_cursorX], m_copyCharExtraBytes, m_copyWidth);

	m_inputLength += m_copyLength;
	m_inputWidth += m_copyWidth;
	m_cursorPos += m_copyLength;
	m_cursorX += m_copyWidth;
}

// chatmsg.cpp:1427
void CChatMsg::InsertWCharacter(int ch) {
	float inputWidth = m_pMaskedFont->GetTextWidth(g_view, m_inputText);
	if (inputWidth > m_bufWidth || m_inputLength >= m_bufLen)
		return;

	memmove(&m_inputText[m_cursorPos + 2], &m_inputText[m_cursorPos], m_inputLength - m_cursorPos);
	memmove(&m_inputTextAttr[m_cursorPos + 2], &m_inputTextAttr[m_cursorPos], m_inputLength - m_cursorPos);

	memmove(&m_charExtraBytes[m_cursorX + 1], &m_charExtraBytes[m_cursorX], m_inputWidth - m_cursorX);

	m_inputText[m_cursorPos] = (char)ch;
	m_inputText[m_cursorPos + 1] = (char)(ch >> 8);
	m_inputTextAttr[m_cursorPos] = 1;
	m_inputTextAttr[m_cursorPos + 1] = 0;

	m_charExtraBytes[m_cursorX] = 1;

	m_inputWidth += 1;
	m_inputLength += 2;
	m_cursorPos += 2;
	m_cursorX += 1;
	m_inputText[m_inputLength] = '\0';
}

// chatmsg.cpp:1452
void CChatMsg::WriteCharacter(int ch) {
	float inputWidth = m_pMaskedFont->GetTextWidth(g_view, m_inputText);
	if (inputWidth > m_bufWidth || m_inputLength >= m_bufLen)
		return;

	if (m_insertMode) {
		memmove(&m_inputText[m_cursorPos + 1], &m_inputText[m_cursorPos], m_inputLength - m_cursorPos);
		memmove(&m_inputTextAttr[m_cursorPos + 1], &m_inputTextAttr[m_cursorPos], m_inputLength - m_cursorPos);
		memmove(&m_charExtraBytes[m_cursorX + 1], &m_charExtraBytes[m_cursorX], m_inputWidth - m_cursorX);

		m_inputLength += 1;
		m_inputWidth += 1;
	} else {
		// Overwrite mode: a selection still makes room first.
		if (m_selectedLength) {
			memmove(&m_inputText[m_cursorPos + 1], &m_inputText[m_cursorPos], m_inputLength - m_cursorPos);
			memmove(&m_inputTextAttr[m_cursorPos + 1], &m_inputTextAttr[m_cursorPos], m_inputLength - m_cursorPos);
			memmove(&m_charExtraBytes[m_cursorX + 1], &m_charExtraBytes[m_cursorX], m_inputWidth - m_cursorX);

			m_inputLength += 1;
			m_inputWidth += 1;
		}

		if (m_cursorX == m_inputWidth) {
			m_inputLength += 1;
			m_inputWidth += 1;
		} else if (m_charExtraBytes[m_cursorX]) {
			// Overwriting a double-byte character: drop its second byte.
			memmove(&m_inputText[m_cursorPos + 1], &m_inputText[m_cursorPos + 2], m_inputLength - m_cursorPos - 2);
			memmove(&m_inputTextAttr[m_cursorPos + 1], &m_inputTextAttr[m_cursorPos + 2], m_inputLength - m_cursorPos - 2);
			m_inputLength--;
		}
	}

	m_inputText[m_cursorPos] = (char)ch;
	m_inputTextAttr[m_cursorPos] = 0;
	m_charExtraBytes[m_cursorX] = 0;

	m_cursorPos += 1;
	m_cursorX += 1;
	m_inputText[m_inputLength] = '\0';
}

// chatmsg.cpp:1541
void CChatMsg::PrintInChatWindow(void) {
}

// chatmsg.cpp:1641
// Unreferenced by the original; its dynamic initializer stores 10.0f (built
// from a const variable after the ATL initializer, docs/tough_problems/010).
const float CHAT_DEFAULT_SPAN = 10.0f;
float s_chatDefaultSpan = CHAT_DEFAULT_SPAN;

// chatmsg.cpp:1654
unsigned long CChatMsg::ConvertToUIColor(unsigned long color) {
	switch (color) {
	case 0xFFFFFFFF:
		return 0xFF000000;
	case 0xFFFFCC00:
		return 0xFFBF5100;
	case 0xFFD2A2FF:
		return 0xFF8400FF;
	case 0xFFFF7878:
		return 0xFFFF0000;
	case 0xFF00FF00:
		return 0xFF00A117;
	case 0xFF00C6FF:
		return 0xFF0054FF;
	case 0xFF5BFFB0:
		return 0xFF009900;
	}
	return color;
}

// chatmsg.cpp:1709
void CChatMsg::AddChatMsg(const std::string& msg, unsigned long color, bool alwaysShow, bool removeColor) {
	m_lastPrintTime = g_CurrentTime;

	std::string text;

	if (removeColor)
		RemoveColor(msg, text);
	else
		text = msg;

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI) : false) {
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 3, (intptr_t)text.c_str(), color, 0, 0, 0);
		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x60, (intptr_t)m_chatFacialUser.c_str(), (intptr_t)text.c_str(), 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) : false) {
		if (GOLFDOC()->IsShowChat() || alwaysShow)
			AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 100, (intptr_t)text.c_str(), color, 0, 0, 0);

		AfxGetTask()->GetActor("Screen") << MsgObject(0, 100, (intptr_t)text.c_str(), ConvertToUIColor(color), 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI) : false) {
		if (GOLFDOC()->IsShowChat() || alwaysShow)
			AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 100, (intptr_t)text.c_str(), color, alwaysShow, 0, 0);
	}

	sChatLine line;
	line.m_text = text;
	line.m_color = color;
	WSingleton<CSharedDoc>::Instance()->m_chatLineList.push_back(line);
}

// chatmsg.cpp:1851
void CChatMsg::GetChatText(std::string& front, std::string& comp, std::string& end) {
	m_inputText[m_inputLength] = '\0';

	if (m_cursorPos < m_inputLength) {
		int pos = m_cursorPos;

		char c = m_inputText[pos];
		m_inputText[pos] = '\0';
		front = m_inputText;

		m_inputText[m_cursorPos] = c;
		end = &m_inputText[m_cursorPos];
	} else {
		front = m_inputText;
		end = "";
	}

	if (m_editFlag)
		comp = m_compText;
	else
		comp = "";
}

// chatmsg.cpp:1877
void CChatMsg::GetCompositionString(sCompStr *compStr) {
	compStr->pCompStr = m_compText;
	compStr->pCompAttr = m_compAttr;
	compStr->pCompClause = m_compClause;
	compStr->compCursor = m_compCursor;
	compStr->compDeltaStart = m_compDeltaStart;
}

// chatmsg.cpp:1886
void CChatMsg::SetChatText(const char* text, bool keepCursor) {
	int len = strlen(text);

	if (len > m_bufLen) {
		strcpy(m_inputText, text);
		len = strlen(text);
		m_bufLen = len;
	} else {
		strcpy(m_inputText, text);
		len = strlen(m_inputText);
	}

	int cursorPos = m_cursorPos;
	int cursorX = m_cursorX;
	int x = 0;
	m_inputLength = len;

	for (int i = 0; i < m_inputLength; ++i) {
		if (m_inputText[i] & 0x80) {
			m_charExtraBytes[x] = 1;
			++i;
		} else {
			m_charExtraBytes[x] = 0;
		}
		++x;

		if (keepCursor) {
			if (m_cursorPos > i) {
				cursorPos = i;
				cursorX = x;
			}
		}
	}

	m_inputWidth = x;

	if (keepCursor) {
		m_cursorPos = m_cursorPos ? cursorPos + 1 : 0;
		m_cursorPos = m_cursorPos > m_inputLength ? m_inputLength : m_cursorPos;
		m_cursorX = cursorX;
	} else {
		m_cursorPos = m_inputLength;
		m_cursorX = x;
	}

	m_cursorX = m_cursorX > m_inputWidth ? m_inputWidth : m_cursorX;
}

// chatmsg.cpp:1953
void CChatMsg::DrawChatWindow(void) {
	if (m_openState == 0)
		return;

	if (!g_pFresh->IsVisible())
		return;

	for (int i = 0; i < 3; ++i) {
		if (m_pOverlays[i])
			m_pOverlays[i]->Render(g_view, m_overlayDst[i], m_overlaySrc[i], 0, 0xFFFFFFFF, 0.0f, 0);
	}
}

// chatmsg.cpp:1980
void CChatMsg::PrintInScreen(void) {
	m_printed = true;

	if (g_CurrentTime - m_lastPrintTime > 10000) {
		if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
			AfxGetTask()->PassMsgToActor("GolfRule", MsgObject(0, 0x96, 0, 0, 0, 0, 0));
		}

		m_lastPrintTime = g_CurrentTime;
	}
}

// chatmsg.cpp:2029
void CChatMsg::PrintOnChatRoom(void) {
}

// chatmsg.cpp:2061
std::string CChatMsg::GetIncomingChatMsg(void) const {
	std::string msg(m_inputText);

	if (m_compChar)
		msg += m_compText;

	for (unsigned int i = 0; i < msg.length(); ++i) {
		if (msg[i] != ' ')
			return msg;
	}

	return msg = "";
}

// chatmsg.cpp:2084
void CChatMsg::Open(bool reset, bool) {
	if (IsLocalContent((localContentType_t)0x4F) && WSingleton<CSharedDoc>::Instance()->m_playingMode == 2)
		return;

	SetActive(true, reset, false);

	m_caretVisible = true;
	m_caretBlinkTime = 0.0f;
	m_openState = 1;

	ReplaceBackupUiAlpha();

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (GOLFDOC() && IsMyTurn(true))
			GOLFDOC()->GetUIAlpha() = 0x80;
	} else {
		if (GOLFDOC())
			GOLFDOC()->GetUIAlpha() = 0x80;
	}
}

// chatmsg.cpp:2121
void CChatMsg::Close(bool reset, bool) {
	SetActive(false, reset, false);

	if (m_openState != 0) {
		m_openState = 2;
		m_openSpeed = 3000.0f;
	}

	if (GOLFDOC())
		GOLFDOC()->GetUIAlpha() = m_backupUiAlpha;

	IActor *pActor = AfxGetTask()->GetActor("GolfRule");
	if (pActor)
		pActor << MsgObject(0, 0x62, 0, 0, 0, 0, 0);

	pActor = AfxGetTask()->GetActor("AvatarChat");
	if (pActor) {
		pActor << MsgObject(0, 0x62, 0, 0, 0, 0, 0);
	}
}

// chatmsg.cpp:2145
const char* CChatMsg::MakeShortID(const char* id, float width) {
	static char shortID[0x100];

	float ellipsisWidth = m_pMaskedFont->GetTextWidth(g_view, "\xA1\xA6");

	if (m_pMaskedFont->GetTextWidth(g_view, id) > width) {
		float idWidth = 0.0f;
		int i = 0;

		for (;;) {
			if (id[i] & 0x80) {
				static float wideCharWidth = m_pMaskedFont->GetTextWidth(g_view, "\xB0\xA1");

				idWidth += wideCharWidth;

				if (width - ellipsisWidth < idWidth)
					break;

				shortID[i] = id[i];
				shortID[i + 1] = id[i + 1];
				i += 2;
			} else {
				idWidth += m_pMaskedFont->GetTextWidth(g_view, "A");

				if (width - ellipsisWidth < idWidth)
					break;

				shortID[i] = id[i];
				i += 1;
			}
		}

		shortID[i] = '\0';
		strcat(shortID, "\xA1\xA6");
	} else {
		strcpy(shortID, id);
	}

	return shortID;
}

// chatmsg.cpp:2195
bool CChatMsg::IME_IsCandWinOpened(void) {
	return g_ime->GetState(5, 0) ? true : false;
}

// chatmsg.cpp:2200
bool CChatMsg::IME_IsCompDone(void) {
	return g_ime->GetState(6, 0) ? true : false;
}

// chatmsg.cpp:2205
bool CChatMsg::IME_IsImeOpened(void) {
	return g_ime->GetState(7, 0) ? true : false;
}

// chatmsg.cpp:2210
bool CChatMsg::IME_IsCompInProgress(void) {
	return g_ime->GetState(4, 0) ? true : false;
}

// chatmsg.cpp:2215
bool CChatMsg::IME_IsAlphaNumericMode(void) {
	return g_ime->GetState(10, 0) ? false : true;
}

// chatmsg.cpp:2220
void CChatMsg::IME_SetAlphaNumericMode(void) {
	g_ime->SetAlphaNumericMode();
}

// chatmsg.cpp:2225
void CChatMsg::IME_SetOpenStatus(bool open) {
	g_ime->SetOpenStatus(open);
}

// chatmsg.cpp:2230
tagCANDIDATELIST *const CChatMsg::IME_GetCandList(void) {
	return (tagCANDIDATELIST *)g_ime->GetState(9, 0);
}
