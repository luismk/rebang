// Original: d:\build\custom\temp\code\client\projectg\frienddlg.cpp
#include "FriendDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "buddymanager.h"
#include "GiftDlg.h"
#include "Post_SendDlg.h"
#include "fredit.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "bitmap.h"
#include "user_info.h"
#include "binstr.h"

#include <map>
#include <string.h>

// frienddlg.cpp:24 (run-time type and factory registration of FrFriendDlg)
const WRTTI FrFriendDlg::m_RTTI("FrFriendDlg", &FrForm::m_RTTI);

IObject* FrFriendDlgMakeInstance(void) {
	return new FrFriendDlg;
}

struct __sFrFriendDlg {
	__sFrFriendDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFriendDlgMakeInstance, "FrFriendDlg");
	}
};
static __sFrFriendDlg s_registerFrFriendDlg;

// frienddlg.cpp:26
const sFRESH_MSGMAP* FrFriendDlg::GetMessageMap(void) const {
	return &FrFriendDlg::_MsgMap;
}

sFRESH_MSGMAP FrFriendDlg::_MsgMap = { &FrForm::_MsgMap, &FrFriendDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFriendDlg::_MsgEntries[] = {
	// frienddlg.cpp:28
	{ "friend_listbox", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendDlg::OnFriendListInit },
	{ "friend_listbox", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFriendDlg::OnFriendListOwnerDraw },
	{ "friend_listbox", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendDlg::OnFriendListLBtnUp },
	{ "friend_listbox", FRCMD_RBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendDlg::OnFriendListRBtnUp },
	{ "friend_listbox", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrFriendDlg::OnFriendListDblClick },
	{ "cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrFriendDlg::OnCancelBtnUp },

	// frienddlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frienddlg.cpp:39
FrFriendDlg::FrFriendDlg(void) {
	m_pFriendList = 0;
	m_pGiftDlg = 0;
	m_uid = -1;
}

// frienddlg.cpp:46
void FrFriendDlg::SetParent(FrGiftDlg* pParent) {
	m_pGiftDlg = pParent;
}

// frienddlg.cpp:51
void FrFriendDlg::SetParent2(CPost_SendDlg* pParent) {
	m_pPostSendDlg = pParent;
}

// frienddlg.cpp:57
bool FrFriendDlg::IsSelectPerson(void) {
	return m_uid != -1 ? true : false;
}

// frienddlg.cpp:62
bool FrFriendDlg::HaveFriends(void) {
	return m_pFriendList->GetCurrentItemSize(false) > 0 ? true : false;
}

// frienddlg.cpp:71
void FrFriendDlg::UpdateFriendList(void) {
	if (!m_pFriendList) {
		return;
	}

	std::map<unsigned long, sFriend>* pBuddyList = BuddyMGR->GetBuddyList();

	m_pFriendList->ClearItem();

	for (std::map<unsigned long, sFriend>::iterator it = pBuddyList->begin(); it != pBuddyList->end(); ++it) {
		sFriend* pFriend = &it->second;

		if (pFriend->flags & 0x0C) {
			m_pFriendList->AddItem(pFriend);
		}
	}
}

// frienddlg.cpp:95
void FrFriendDlg::OnFriendListInit(intptr_t param) {
	m_pFriendList = DYNAMIC_CAST(FrListBox, param);
	if (m_pFriendList) {
		m_pSexIcon[0] = m_pFriendList->GetBitmap("i_male");
		m_pSexIcon[1] = m_pFriendList->GetBitmap("i_female");

		m_pFriendList->UseRightButton(true);

		UpdateFriendList();
	}
}

// frienddlg.cpp:110
void FrFriendDlg::OnFriendListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem) {
		sFriend* pFriend = (sFriend*)pItem->pData;
		if (pFriend) {
			FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
			if (pGDI) {
				const Bitmap* pIcon = m_pSexIcon[pFriend->flags & 1];
				WRect rect(pItem->pos.x - 8.0f, pItem->pos.y, m_pSexIcon[0]->Width(), pIcon->Height());

				if (pFriend->uid == m_uid) {
					pGDI->Box(WRect(rect.x, rect.y, m_pFriendList->GetItemWidth() + 8.0f, (float)m_pFriendList->GetItemHeight()), 0x999B9DFF, 0, 0.0f);
				}

				pGDI->DrawTexture(pIcon, rect, (((pFriend->flags & 4) && (pFriend->flags & 2)) ? 0xFF : 0x50) << 24 | 0xFFFFFF, 0);

				if (pItem->underCursor) {
					pGDI->SetTextColor(0xFFFFFFFF, 0xFF808080);
					pGDI->SetTextStyle(2);

				} else {
					pGDI->SetTextColor(0xFF4C4C4C, 0xFFFFFFFF);
					pGDI->SetTextStyle(0);
				}

				char szNick[22] = { 0 };
				int nickShown = 0;
				if (strcmp(pFriend->group, "Friend") == 0) {
					if (!pItem->underCursor) {
						pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
						pGDI->SetTextStyle(0);
					}

					if (strlen(pFriend->nick) > 18) {
						mbscut(szNick, pFriend->nick, 18);
						strcat(szNick, "..");
						g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 12.0f, pItem->pos.y + 7.0f), 0, szNick, -1.0f, 0xFFFFFFFF);
					} else {
						g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 12.0f, pItem->pos.y + 7.0f), 0, pFriend->nick, -1.0f, 0xFFFFFFFF);
					}

					nickShown = 1;

				} else {
					g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 12.0f, pItem->pos.y + 7.0f), 0, pFriend->group, -1.0f, 0xFFFFFFFF);
				}

				if (!pItem->underCursor) {
					pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
					pGDI->SetTextStyle(0);
				}

				if (!nickShown) {
					if (strlen(pFriend->nick) > 9) {
						mbscut(szNick, pFriend->nick, 9);
						strcat(szNick, "..");
						g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 82.0f, pItem->pos.y + 7.0f), 0, szNick, -1.0f, 0xFFFFFFFF);

					} else {
						g_pFresh->GetManager()->PrintText(WPoint(pItem->pos.x + 82.0f, pItem->pos.y + 7.0f), 0, pFriend->nick, -1.0f, 0xFFFFFFFF);
					}
				}
			}
		}
	}
}

// frienddlg.cpp:202
void FrFriendDlg::OnFriendListLBtnUp(void) {
	FrListItem* pItem = m_pFriendList->GetItemUnderCursor();
	if (!pItem) {
		return;
	}

	sFriend* pFriend = (sFriend*)pItem->pData;
	if (!pFriend) {
		return;
	}

	if (pFriend->uid == -1) {
		return;
	}

	if (m_uid == -1 && m_pGiftDlg) {
		if (m_pGiftDlg->m_pEdNick->GetLine(1, false)) {
			m_savedNick = m_pGiftDlg->m_pEdNick->GetLine(1, false);
		}
		if (m_pGiftDlg->m_pEdConfirm->GetLine(1, false)) {
			m_savedConfirm = m_pGiftDlg->m_pEdConfirm->GetLine(1, false);
		}
	} else if (m_uid == -1 && m_pPostSendDlg) {
		if (m_pPostSendDlg->m_pReciveUserEdit->GetLine(1, false)) {
			m_savedConfirm = m_pPostSendDlg->m_pReciveUserEdit->GetLine(1, false);
		}
	}

	m_uid = pFriend->uid;
	if (m_pGiftDlg) {
		m_pGiftDlg->m_pEdNick->SetLine(1, pFriend->nick, 0, false, 0);
		m_pGiftDlg->m_pEdConfirm->SetLine(1, pFriend->nick, 0, false, 0);
	} else if (m_pPostSendDlg) {
		m_pPostSendDlg->m_pReciveUserEdit->SetLine(1, pFriend->nick, 0, false, 0);
		m_pPostSendDlg->m_pUserNickEdit->SetLine(1, pFriend->nick, 0, false, 0);
	}
}

// frienddlg.cpp:254
void FrFriendDlg::OnFriendListRBtnUp(void) {
	if (!CUserInfo::Instance()->IsVisible()) {
		if (CUserInfo::Instance()) {
			OnFriendListLBtnUp();

			if (m_uid != -1) {
				sFriend* pFriend = BuddyMGR->GetBuddy(m_uid);
				if (pFriend) {
					CUserInfo::Instance()->SetInfo(m_uid, 0, true, true, true, true, std::string(pFriend->nick));
				}
			}
		}
	}
}

// frienddlg.cpp:273
void FrFriendDlg::OnFriendListDblClick(void) {
	if (IsSelectPerson()) {
		OnOK();
	}
}

// frienddlg.cpp:281
void FrFriendDlg::OnCancelBtnUp(void) {
	if (m_uid != -1 && m_pGiftDlg) {
		m_pGiftDlg->m_pEdNick->SetLine(1, m_savedNick.c_str(), 0, false, 0);
		m_pGiftDlg->m_pEdConfirm->SetLine(1, m_savedConfirm.c_str(), 0, false, 0);
	}

	OnCancel();
}
