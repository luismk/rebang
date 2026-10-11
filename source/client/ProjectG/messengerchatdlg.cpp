// Original: d:\build\custom\temp\code\client\projectg\messengerchatdlg.cpp
#include "messengerchatdlg.h"
#include "ObjectFactory.h"
#include "ChatMsg.h"
#include "GolfTask.h"
#include "TaskManager.h"
#include "commonutil.h"
#include "wfont.h"
#include "wview.h"
#include "EmoticonDlg.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frframe.h"
#include "frstatic.h"
#include "frwndmanager.h"
#include "InputManager.h"
#include "buddymanager.h"
#include "messengerdlg.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "wutil.h"

// messengerchatdlg.cpp:21 (run-time type and factory registration of FrMessengerChatDlg)
const WRTTI FrMessengerChatDlg::m_RTTI("FrMessengerChatDlg", &FrForm::m_RTTI);

IObject* FrMessengerChatDlgMakeInstance(void) {
	return new FrMessengerChatDlg;
}

struct __sFrMessengerChatDlg {
	__sFrMessengerChatDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMessengerChatDlgMakeInstance, "FrMessengerChatDlg");
	}
};
static __sFrMessengerChatDlg s_registerFrMessengerChatDlg;

// messengerchatdlg.cpp:23
const sFRESH_MSGMAP* FrMessengerChatDlg::GetMessageMap(void) const {
	return &FrMessengerChatDlg::_MsgMap;
}

sFRESH_MSGMAP FrMessengerChatDlg::_MsgMap = { &FrForm::_MsgMap, &FrMessengerChatDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMessengerChatDlg::_MsgEntries[] = {
	// messengerchatdlg.cpp:25
	{ "cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMessengerChatDlg::OnMessenger_CloseLBtnDown },
	// messengerchatdlg.cpp:26
	{ "send", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMessengerChatDlg::OnMessenger_ChatInputInit },
	// messengerchatdlg.cpp:27
	{ "send", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMessengerChatDlg::OnMessenger_ChatInputLBtnDown },
	// messengerchatdlg.cpp:28
	{ "send", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrMessengerChatDlg::OnChatInputEnterKey },
	// messengerchatdlg.cpp:29
	{ "receive", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMessengerChatDlg::OnMessenger_ChatViewInit },
	// messengerchatdlg.cpp:30
	{ "emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMessengerChatDlg::OnMessenger_EmoticonInit },
	// messengerchatdlg.cpp:31
	{ "emoticon", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMessengerChatDlg::OnMessenger_EmoticonLBtnDown },
	// messengerchatdlg.cpp:32
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMessengerChatDlg::OnMessenger_CaptionInit },

	// messengerchatdlg.cpp:34
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// messengerchatdlg.cpp:38
FrMessengerChatDlg::FrMessengerChatDlg(void) {
	m_pEmoticonDlg = 0;
	m_pEmoticonBtn = 0;
	m_reserved114 = 0;
	m_pChatInput = 0;
	m_pChatView = 0;
	m_uid = 0;
	m_nick = "";

	m_vibrationTime = 0;
	m_vibrationElapsed = 0;
	m_bVibrating = false;
	m_shakeOffset = 2.0f;
	m_shakeOffsetNext = -2.0f;
}

// messengerchatdlg.cpp:56
FrMessengerChatDlg::~FrMessengerChatDlg(void) {
}

// messengerchatdlg.cpp:71
void FrMessengerChatDlg::OnMessenger_CloseLBtnDown(void) {
	std::list<sChatSlot>& chatDlgList = CMessengerInfo::Instance()->m_chatDlgList;

	std::list<sChatSlot>::iterator it;
	for (it = chatDlgList.begin(); it != chatDlgList.end(); ++it) {
		if (it->uid == m_uid) {
			it->pChatDlg = 0;
			chatDlgList.erase(it);
			break;
		}
	}

	Close(true);
}

// messengerchatdlg.cpp:104
void FrMessengerChatDlg::OnMessenger_ChatInputInit(intptr_t param) {
	m_pChatInput = DYNAMIC_CAST(FrEdit, param);

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		g_pFresh->GetManager()->SetExclusiveKey(true);
	}

	if (m_pChatInput) {
		m_pChatInput->SetCharLimit(0x24, true);
	}
}

// messengerchatdlg.cpp:134
void FrMessengerChatDlg::OnMessenger_ChatInputLBtnDown(void) {
	OnLButtonDown(g_pFresh->GetManager()->GetMousePos());
}

// messengerchatdlg.cpp:149
bool FrMessengerChatDlg::OnLButtonDown(const WPoint& pt) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		if (g_input->GetButton(LEFT_BUTTON) == 1) {
			g_pFresh->GetManager()->SetExclusiveKey(true);
			CMessengerInfo::Instance()->SetGameFocus(false);
		}
	}

	m_pBaseFrm->SetBlink(false);

	return FrForm::OnLButtonDown(pt);
}

// messengerchatdlg.cpp:176
void FrMessengerChatDlg::OnProc(const float dt) {
	if (m_bVibrating) {
		if (m_vibrationTime > m_vibrationElapsed) {
			WRect rect = GetRect();

			MoveWindow(WPoint(rect.x + m_shakeOffset, rect.y + m_shakeOffsetNext));

			Swap(m_shakeOffset, m_shakeOffsetNext);

			m_vibrationElapsed += dt;
		} else {
			m_bVibrating = false;
			m_vibrationTime = 0;
			m_vibrationElapsed = 0;
		}
	}

	if ((g_input->Get("LCONTROL", false) || g_input->Get("RCONTROL", false)) && g_input->GetDown("EMOTICON", false)) {
		OnMessenger_EmoticonLBtnDown();
	}
}

// messengerchatdlg.cpp:215
void FrMessengerChatDlg::Vibration(float time) {
	m_pBaseFrm->SetBlink(true);
}

// messengerchatdlg.cpp:238
void FrMessengerChatDlg::OnMessenger_ChatViewInit(intptr_t param) {
	m_pChatView = DYNAMIC_CAST(FrEdit, param);
}

// messengerchatdlg.cpp:253
bool FrMessengerChatDlg::OnChatInputEnterKey(intptr_t param) {
	char* text = (char*)param;

	if (strlen(text)) {
		CMessengerInfo* pInfo = CMessengerInfo::Instance();
		std::list<sChatSlot>::iterator it;
		for (it = pInfo->m_chatDlgList.begin(); it != pInfo->m_chatDlgList.end(); ++it) {
			if (it->uid == m_uid) {
				sFriend* pFriend = BuddyMGR->GetBuddy(m_uid);
				if (!pFriend)
					return false;
				if (pFriend->flags & 0x10) {
					AddChatLine(pFriend->nick, "\xc2\xf7\xb4\xdc\xbb\xf3\xb4\xeb\xbf\xcd\xb4\xc2 \xb4\xeb\xc8\xad\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9");
					return false;
				}
				if (!(pFriend->flags & 2)) {
					AddChatLine(pFriend->nick, "\xb8\xde\xbd\xc3\xc1\xf6\xb8\xa6 \xc0\xfc\xb4\xde\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9");
					return false;
				}

				CSharedDoc::Instance()->m_chatManager.Filtering(text);

				WSendPacket packet(0x1E);
				packet.Encode4(pFriend->uid);
				packet.EncodeStr(std::string(text));
				packet.Send((eSendTo)1);

				CSharedDoc::Instance()->m_chatManager.FilteringHack(text, false);

				sMsnChat chat;
				chat.nick = CSharedDoc::Instance()->GetPangyaUserInfo().nickname;
				chat.text = text;

				it->chatList.push_back(chat);

				AddChatLine(chat.nick, chat.text);
				break;
			}
		}
	}

	return true;
}

// messengerchatdlg.cpp:332
void FrMessengerChatDlg::AddChatLine(std::string nick, std::string text) {
	if (m_pChatView) {
		if (m_nick != nick) {
			m_pChatView->AddLine(MakeStr("%s : ", nick.c_str()), 0xFF000000, false);
			m_nick = nick;
		}

		CSharedDoc::Instance()->m_chatManager.FilteringHack(text.c_str(), false);

		wchar_t wideText[0x104];
		wchar_t wideLine[0x104];
		char line[0x104];
		int len = MultiByteToWideChar(0, 0, text.c_str(), strlen(text.c_str()) + 1, wideText, 0x104);

		int i = 0;
		int shift = 0;
		int lineCount;
		if (len % 16 == 0) {
			lineCount = len / 16 - 1;
			lineCount = lineCount < 0 ? 0 : lineCount;
		} else {
			lineCount = len / 16;
		}

		do {
			memset(wideLine, 0, sizeof(wideLine));
			memset(line, 0, sizeof(line));

			int start = i * 16 + shift;
			int end = i * 16 + 16;
			if (!(len > end))
				end = len - 1;

			wchar_t* p = &wideText[end];
			bool bOpen = true;
			for (int j = 0; j < 4; j++, p--) {
				if (*p == ')')
					bOpen = false;

				if (*p == '(' && bOpen)
					break;
			}

			for (int k = 0; k < 4; k++) {
				if (wideText[end + k] == ')') {
					shift = k + 1;
					break;
				}
			}

			end += shift;

			wcsncpy(wideLine, &wideText[start], end - start);

			if (WideCharToMultiByte(0, 0, wideLine, -1, line, 0x104, 0, 0) > 1) {
				if (i)
					m_pChatView->AddLine(MakeStr("   \x08%s", line), 0xFF464646, false);
				else
					m_pChatView->AddLine(MakeStr(" > \x08%s", line), 0xFF464646, false);
			}
		} while (i++ < lineCount);
	}
}

// messengerchatdlg.cpp:455
void FrMessengerChatDlg::OnMessenger_EmoticonInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// messengerchatdlg.cpp:470
void FrMessengerChatDlg::OnMessenger_EmoticonLBtnDown(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", this);
		if (m_pEmoticonDlg) {
			WPoint pt(0, 0);
			if (m_pEmoticonBtn) {
				pt.x = m_pEmoticonBtn->GetRect().x + 20.0f;
				pt.y = m_pEmoticonBtn->GetRect().y + 20.0f;
			}

			m_pEmoticonDlg->SetFixed(true);
			m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrMessengerChatDlg::OnEmoticonResult, pt, 3);

			WRect rect = m_pEmoticonDlg->GetRect();
			m_pEmoticonDlg->Adjust(rect);
			m_pEmoticonDlg->MoveWindow(WPoint(rect.x, rect.y));
		}
	}
}

// messengerchatdlg.cpp:507
bool FrMessengerChatDlg::OnEmoticonResult(int result, FrForm* pForm) {
	if (m_pEmoticonDlg && result == 1) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = m_pChatInput;

				if (pEdit) {
					if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
						g_pFresh->GetManager()->SetExclusiveKey(true);
					}

					const char* front = pEdit->GetEditText_Front();
					const char* comp = pEdit->GetEditText_Comp();
					const char* end = pEdit->GetEditText_End();

					if (CChatMsg::Instance()->GetMaskedFont()->GetTextWidth(g_view, MakeStr("%s%s%s%s", front, comp, icon, end)) < pEdit->GetWidthLimit())
						pEdit->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);

					pEdit->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// messengerchatdlg.cpp:555
void FrMessengerChatDlg::OnMessenger_CaptionInit(intptr_t param) {
	m_pCaption = DYNAMIC_CAST(FrStatic, param);
}

// messengerchatdlg.cpp:570
void FrMessengerChatDlg::SetUID(unsigned long uid) {
	m_uid = uid;

	sFriend* pFriend = BuddyMGR->GetBuddy(uid);
	if (pFriend) {
		m_pCaption->SetCaption(pFriend->nick);
	}
}

// messengerchatdlg.cpp:592
bool FrMessengerChatDlg::Close(bool bDestroy) {
	if (m_pEmoticonDlg && !bDestroy) {
		m_pEmoticonDlg->Close(true);
		return false;
	}

	std::list<sChatSlot>& chatDlgList = CMessengerInfo::Instance()->m_chatDlgList;

	std::list<sChatSlot>::iterator it;
	for (it = chatDlgList.begin(); it != chatDlgList.end(); ++it) {
		if (it->uid == m_uid) {
			it->pChatDlg = 0;
			break;
		}
	}

	for (it = chatDlgList.begin(); it != chatDlgList.end(); ++it) {
		if (it->pChatDlg) {
			it->pChatDlg->FindNextTopFocus(false);
			break;
		}
	}

	FrForm::Close(true);

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		if (!CChatMsg::Instance()->IsOpened() && chatDlgList.size() == 1) {
			g_pFresh->GetManager()->SetExclusiveKey(false);
			CMessengerInfo::Instance()->SetGameFocus(true);
		}
	}

	return true;
}

// messengerchatdlg.cpp:658
void FrMessengerChatDlg::RestoreChat(sChatSlot& slot) {
	sMsnChat chat;

	std::list<sMsnChat>::iterator it;
	for (it = slot.chatList.begin(); it != slot.chatList.end(); ++it) {
		chat = *it;
		AddChatLine(chat.nick, chat.text);
	}

	if (m_pChatInput)
		m_pChatInput->AddLine(slot.tempChat.c_str(), 0, false);
}

// messengerchatdlg.cpp:684
void FrMessengerChatDlg::SaveTempChat(sChatSlot& slot) {
	if (m_pChatInput)
		slot.tempChat = m_pChatInput->GetLine(1, false);
}

// messengerchatdlg.cpp:701
bool FrMessengerChatDlg::GetEmoticonClientRect(WRect& rect) const {
	if (m_pEmoticonDlg) {
		rect = m_pEmoticonDlg->GetRect();
		return true;
	}

	return false;
}

// wutil.h:222: the original holds the out-of-line Swap<float> in MessengerChatDlg's object.
template void Swap<float>(float& a, float& b);
