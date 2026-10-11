// Original: d:\build\custom\temp\code\client\projectg\post_senddlg.cpp
// This unit emits the out-of-line copy of FrFriendDlg::GetUid (frienddlg.h).
#define POST_SENDDLG_FRIENDDLG_INLINE
#include "NetworkSystem.h"
#include "Packet.h"
#include "post_senddlg.h"
#include "FriendDlg.h"
#include "EmoticonDlg.h"
#include "ObjectFactory.h"
#include "StandingInLine.h"
#include "SharedDoc.h"
#include "TransferableItem.h"
#include "bitmap.h"
#include "binstr.h"
#include "TaskManager.h"
#include "actor.h"
#include "buddymanager.h"
#include "commonutil.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "fresh.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "wview.h"
#include "../shared/ItemManager.h"
#include "../shared/Localize.h"
#include "../shared/classdefine.h"
#include "UccLibrary.h"
#include "UccManager.h"

#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <string.h>

// File names of the title fonts (nameless data).
extern "C" char* data_a8f630[];
extern "C" char* data_a8f634[];
extern "C" char* data_a8f638[];
// Names of the array numbers of the item list (nameless data).
extern "C" char* data_af6414[];

// UccLibrary.cpp overload missing from UccLibrary.h (belongs next to
// GetUccItemIcon(sItemInfo*), uccLibrary.h).
const Bitmap* GetUccItemIcon(unsigned long uid);

// Name of the gateway actor (file-local data).
static const char* s_gatewayActorName = "Gateway";

// post_senddlg.cpp:39
void CallErrorMsg(const char* msg) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
	pForm->SetMessage(msg, false);
	pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
}

// post_senddlg.cpp:47 (run-time type and factory registration of CPost_SendDlg)
const WRTTI CPost_SendDlg::m_RTTI("CPost_SendDlg", &FrForm::m_RTTI);

IObject* CPost_SendDlgMakeInstance(void) {
	return new CPost_SendDlg;
}

struct __sCPost_SendDlg {
	__sCPost_SendDlg(void) {
		ObjectFactory().AddObjectFunctor(CPost_SendDlgMakeInstance, "CPost_SendDlg");
	}
};
static __sCPost_SendDlg s_registerCPost_SendDlg;

// post_senddlg.cpp:49
const sFRESH_MSGMAP* CPost_SendDlg::GetMessageMap(void) const {
	return &CPost_SendDlg::_MsgMap;
}

sFRESH_MSGMAP CPost_SendDlg::_MsgMap = { &FrForm::_MsgMap, &CPost_SendDlg::_MsgEntries[0] };

sFRESH_ENTRY CPost_SendDlg::_MsgEntries[] = {
	// post_senddlg.cpp:52
	{ "recive_user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostReciveUserInit },
	// post_senddlg.cpp:53
	{ "recive_user", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CPost_SendDlg::OnPostReceiveUserEnterKey },
	// post_senddlg.cpp:54
	{ "user_Nick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostUserNickInit },
	// post_senddlg.cpp:56
	{ "recive_msg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostReciveMsgInit },
	// post_senddlg.cpp:57
	{ "recive_msg", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CPost_SendDlg::OnPostReciveMsgEnterKey },
	// post_senddlg.cpp:59
	{ "search_user", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostSearchUserInit },
	// post_senddlg.cpp:60
	{ "search_user", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_SendDlg::OnPostSearchUserBtnUp },
	// post_senddlg.cpp:61
	{ "friend_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostFriendListInit },
	// post_senddlg.cpp:62
	{ "friend_list", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_SendDlg::OnPostFriendListBtnUp },
	// post_senddlg.cpp:63
	{ "item_appending", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostItemAddInit },
	// post_senddlg.cpp:64
	{ "item_appending", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_SendDlg::OnPostItemAddBtnUp },
	// post_senddlg.cpp:66
	{ "post_slot", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostListInit },
	// post_senddlg.cpp:67
	{ "post_slot", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostListOwnerDraw },
	// post_senddlg.cpp:69
	{ "Btnemoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnEmoticonBtnInit },
	// post_senddlg.cpp:70
	{ "Btnemoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_SendDlg::OnEmoticonBtnUp },
	// post_senddlg.cpp:72
	{ "edit_servicecharge", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_SendDlg::OnPostServiceChargeInit },
	// post_senddlg.cpp:74
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// post_senddlg.cpp:78
CPost_SendDlg::CPost_SendDlg(void) {
	m_pFriendDlg = 0;
	m_pReciveUserEdit = 0;
	m_pReciveMsgEdit = 0;
	m_pSearchUserBtn = 0;
	m_pFriendListBtn = 0;
	m_pEmoticonBtn = 0;
	m_pEmoticonDlg = 0;
	m_pItemAddBtn = 0;
	m_pMyItemList = 0;
	m_sendUID = (unsigned long)-1;
	m_receiverNick = "";
	m_receiverName = "";

	m_bReceiverSet = false;

	CTransferableItem::Instance()->Reset();
	CTransferableItem::Instance()->SetTransferableItemList();
	CTransferableItem::Instance()->Set_RegistrationListSize(4);

	m_pUccStateBitmap[0] = g_pFresh->RegisterBitmap("ucc_state_original");
	m_pUccStateBitmap[1] = g_pFresh->RegisterBitmap("ucc_state_copied");
	m_pUccStateBitmap[2] = g_pFresh->RegisterBitmap("ucc_state_temp_saved");

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = data_a8f638;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,";

	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);
}

// post_senddlg.cpp:128
CPost_SendDlg::~CPost_SendDlg(void) {
	Reset();
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// post_senddlg.cpp:138
void CPost_SendDlg::Reset(void) {
	m_pFriendDlg = 0;
	m_pReciveUserEdit = 0;
	m_pReciveMsgEdit = 0;
	m_pSearchUserBtn = 0;
	m_pFriendListBtn = 0;
	m_pEmoticonBtn = 0;
	m_pEmoticonDlg = 0;
	m_pItemAddBtn = 0;
	m_pMyItemList = 0;
	m_sendUID = (unsigned long)-1;
	m_receiverNick = "";
	m_receiverName = "";

	m_bReceiverSet = false;

	CTransferableItem::Instance()->Reset();
	CTransferableItem::Instance()->SetTransferableItemList();
	CTransferableItem::Instance()->Set_RegistrationListSize(4);
}

// post_senddlg.cpp:164
void CPost_SendDlg::OnPostReciveUserInit(intptr_t param) {
	m_pReciveUserEdit = DYNAMIC_CAST(FrEdit, param);
}

// post_senddlg.cpp:172
void CPost_SendDlg::OnPostReciveMsgInit(intptr_t param) {
	m_pReciveMsgEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pReciveMsgEdit) {
		m_pReciveMsgEdit->SetCharLimit(800, true);
	}
}

// post_senddlg.cpp:182
bool CPost_SendDlg::OnPostReciveMsgEnterKey(int) {
	return true;
}

// post_senddlg.cpp:187
void CPost_SendDlg::OnPostUserNickInit(intptr_t param) {
	m_pUserNickEdit = DYNAMIC_CAST(FrEdit, param);
}

// post_senddlg.cpp:191
void CPost_SendDlg::OnPostSearchUserInit(intptr_t param) {
	m_pSearchUserBtn = DYNAMIC_CAST(FrButton, param);
}

// post_senddlg.cpp:196
bool CPost_SendDlg::OnPostReceiveUserEnterKey(int) {
	OnPostSearchUserBtnUp();
	return false;
}

// post_senddlg.cpp:202
void CPost_SendDlg::OnPostSearchUserBtnUp(void) {
	if (m_pReciveUserEdit) {
		const char* text = m_pReciveUserEdit->GetLine(1, false);

		if (!stricmp(text, WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname)) {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			pForm->SetMessage("\xba\xbb\xc0\xce\xbf\xa1\xb0\xd4\xb4\xc2 \xbf\xec\xc6\xed\xb9\xb0\xc0\xbb \xba\xb8\xb3\xbe \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9. ", false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 3);
			return;
		}

		int len = strlen(text);
		if (text && len > 0 && len < 0x16) {
			WSendPacket packet((enumClientPacket)7);

			packet.Encode1(0);

			packet.EncodeStr(std::string(text));
			packet.Send(SEND_TO_DEFAULT);

			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 1, 0, 0, 0, 0, 0);
		} else {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xb9\xde\xb4\xc2 \xc0\xcc\xc0\xc7 \xb4\xd0\xb3\xd7\xc0\xd3\xc0\xbb \xb3\xd6\xbe\xee\xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
		}
	}
}

// post_senddlg.cpp:235
void CPost_SendDlg::OnPostFriendListInit(intptr_t param) {
	m_pFriendListBtn = DYNAMIC_CAST(FrButton, param);
}

// post_senddlg.cpp:240
void CPost_SendDlg::OnPostFriendListBtnUp(void) {
	if (BuddyMGR->GetBuddySize() == 0) {
		if (!WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)1)) {
			WSendPacket packet((enumClientPacket)0x3c);
			packet.Encode2(0x11f);
			packet.Send(SEND_TO_DEFAULT);
		} else {
			WSendPacket packet(0x13);
			packet.Encode2(0x11f);
			packet.Send((eSendTo)1);
		}
	}

	if (!m_pFriendDlg) {
		m_pFriendDlg = CreateForm<FrFriendDlg>(g_pFresh->GetManager(), this, "post_friend_list_form", 0);
		if (m_pFriendDlg) {
			m_pFriendDlg->SetParent2(this);
			m_pFriendDlg->EnableDrag(false);
			m_pFriendDlg->Open((FRESH_RESULT_CALLBACK)&CPost_SendDlg::OnFriendListResult, WPoint(602.0f, m_rect.y), 1);
		}
	}
}

// post_senddlg.cpp:270
void CPost_SendDlg::OnPostItemAddInit(intptr_t param) {
	m_pItemAddBtn = DYNAMIC_CAST(FrButton, param);
}

// post_senddlg.cpp:274
void CPost_SendDlg::OnPostItemAddBtnUp(void) {
	bool bResult = false;

	IActor* pActor = AfxGetTask()->GetActor(s_gatewayActorName);
	pActor << MsgObject(0, 0x26d, 0x10, (intptr_t)&bResult, 0, 0, 0);

	if (bResult == true) {
		if (!m_pMyItemList) {
			m_pMyItemList = CreateForm<CPost_MyItemList>(g_pFresh->GetManager(), this, "myitem_choice", 0);

			FrForm::EnableTail(false);
			m_pMyItemList->Open((FRESH_RESULT_CALLBACK)&CPost_SendDlg::OnPostItemListResutl, 0x81);
			m_pMyItemList->SetFadeout(false);
		}
	} else {
		pActor << MsgObject(0, 0x26d, 0x11, 0, 0, 0, 0);
	}
}

// post_senddlg.cpp:306
void CPost_SendDlg::OnPostListInit(intptr_t param) {
	m_pPostList = DYNAMIC_CAST(FrListBox, param);
}

// post_senddlg.cpp:315
void CPost_SendDlg::OnPostListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	sTradeItem* pTrade = (sTradeItem*)pItem->pData;
	if (pTrade == 0)
		return;

	unsigned long typeId = pTrade->typeId;
	int count = pTrade->count;

	if (pGDI == 0)
		return;
	if (typeId == 0)
		return;

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(typeId);
	if (pCommon == 0)
		return;

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);

	if (IsLocalContent((localContentType_t)0x57) && (pTrade->uccStatus & 1) && IsUccClothes(pTrade->typeId)) {
		const Bitmap* pUccIcon = GetUccItemIcon(pTrade->guid);
		if (pUccIcon)
			pIcon = pUccIcon;
	}

	if ((pTrade->typeId & 0xFC000000) == 0x8000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pTrade->typeId >> 18) & 0xFF) | 0x4000000);
		if (pChar) {
			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));
			if (pBitmap) {
				WRect rc(pItem->pos.x + 3.0f, pItem->pos.y + 3.0f, (float)pBitmap->Width(), (float)pBitmap->Height());
				pGDI->DrawTexture(pBitmap, rc, 0xFFFFFFFF, 0);
			}
		}
	}

	if ((typeId & 0xFC000000) == 0x8000000) {
		offsetX = -12.0f;
		offsetY = -20.0f;
	}

	if (pIcon) {
		WRect rc(offsetX + pItem->pos.x + 14.0f, offsetY + pItem->pos.y + 11.0f, (float)pIcon->Width(), (float)pIcon->Height());
		pGDI->DrawTexture(pIcon, rc, 0xFFFFFFFF, 0);
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
		pGDI->SetTextStyle(4);
	}

	if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pTrade->typeId)) {
		if ((pTrade->uccStatus & 1) && pTrade->uccSeq == 1 && m_pUccStateBitmap[0]) {
			pGDI->DrawTexture(m_pUccStateBitmap[0], WRect(pItem->pos.x + 3.0f, pItem->pos.y + 15.0f, (float)m_pUccStateBitmap[0]->Width(), (float)m_pUccStateBitmap[0]->Height()), 0xFFFFFFFF, 0);
		} else if ((pTrade->uccStatus & 1) && pTrade->uccSeq > 1 && m_pUccStateBitmap[1]) {
			pGDI->DrawTexture(m_pUccStateBitmap[1], WRect(pItem->pos.x + 3.0f, pItem->pos.y + 15.0f, (float)m_pUccStateBitmap[1]->Width(), (float)m_pUccStateBitmap[1]->Height()), 0xFFFFFFFF, 0);
		} else if ((pTrade->uccStatus & 2) && m_pUccStateBitmap[2]) {
			pGDI->DrawTexture(m_pUccStateBitmap[2], WRect(pItem->pos.x + 3.0f, pItem->pos.y + 15.0f, (float)m_pUccStateBitmap[2]->Width(), (float)m_pUccStateBitmap[2]->Height()), 0xFFFFFFFF, 0);
		}
	}

	if (IsLocalContent((localContentType_t)0x51)) {
		if ((pTrade->typeId & 0xFC000000) == 0x8000000)
			DrawPartsSlot(pGDI, pItem);
	}

	if (count > 1) {
		const char* text = MakeStr("%d", count);
		float x = pItem->pos.x + 54.0f;

		for (int i = 0; i < strlen(text); i++)
			x -= m_pTitleFont->GetCharWidthA(g_view, text[i] - '0');

		m_pTitleFont->Print(g_view, x, pItem->pos.y + 42.0f, text, 0, 0xFFFFFFFF, 0);
	}
}

// post_senddlg.cpp:563
void CPost_SendDlg::OnPostServiceChargeInit(intptr_t param) {
	m_pServiceChargeEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pServiceChargeEdit) {
		m_pServiceChargeEdit->AddText(MakeStr("%6d", 100), false, true);
	}
}

// post_senddlg.cpp:573
void CPost_SendDlg::OnEmoticonBtnInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pEmoticonBtn) {
		m_pEmoticonBtn->SetVisible(true);
	}
}

// post_senddlg.cpp:583
void CPost_SendDlg::OnEmoticonBtnUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (m_pEmoticonDlg) {
			WPoint pt(0, 0);
			const WRect& rect = m_pEmoticonBtn->GetRect();
			pt.x = rect.x - 200.0f;
			pt.y = rect.y + 24.0f;

			m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&CPost_SendDlg::OnEmoticonResult, pt, 3);
		}
	}
}

// post_senddlg.cpp:600
void CPost_SendDlg::OnOK(void) {
	int listSize = CTransferableItem::Instance()->Get_RegistrationListSize();

	std::string msg = GetLetterMsg();
	int msgLen = msg.size();

	if (!m_bReceiverSet
		|| (strlen(m_pReciveUserEdit->GetLine(1, false)) == 0 && strlen(m_pUserNickEdit->GetLine(1, false)) == 0)) {
		CallErrorMsg("\xbc\xb1\xb9\xb0\xc7\xd2 \xb4\xeb\xbb\xf3\xc0\xc7 \xc0\xcc\xb8\xa7\xc0\xcc \xc8\xae\xc0\xce\xb5\xc7\xc1\xf6 \xbe\xca\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9. \xb0\xcb\xbb\xf6\xc0\xb8\xb7\xce \xc8\xae\xc0\xce\xc7\xcf\xbd\xc3\xb0\xc5\xb3\xaa \xc4\xa3\xb1\xb8\xb8\xae\xbd\xba\xc6\xae\xbf\xa1\xbc\xad \xc3\xdf\xb0\xa1\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4. ");
		return;
	} else if (strlen(m_pReciveMsgEdit->GetLine(1, false)) == 0 && CTransferableItem::Instance()->Get_RegistrationListSize() == 0) {
		CallErrorMsg("\xb8\xde\xbd\xc3\xc1\xf6\xb3\xaa \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc3\xb7\xba\xce\xc7\xcf\xbc\xc5\xbe\xdf \xbf\xec\xc6\xed\xb9\xb0\xc0\xbb \xba\xb8\xb3\xbe \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	} else if (listSize == 0 && WSingleton<CSharedDoc>::Instance()->m_myPang < 100) {
		CallErrorMsg("\xbc\xf6\xbc\xf6\xb7\xe1\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.");
		return;
	} else if ((__int64)(listSize * 500) > WSingleton<CSharedDoc>::Instance()->m_myPang) {
		CallErrorMsg("\xbc\xf6\xbc\xf6\xb7\xe1\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.");
		CTransferableItem::Instance()->ResetRegistrationItemList();
		m_pPostList->ClearItem();
		return;
	} else if (msgLen > 800) {
		CallErrorMsg("\xc6\xed\xc1\xf6 \xb3\xbb\xbf\xeb\xc0\xba 800\xc0\xda\xb8\xa6 \xc3\xca\xb0\xfa\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	}

	if (!VerifySendItemList()) {
		CallErrorMsg("\xc3\xb7\xba\xce\xc7\xd1 \xbe\xc6\xc0\xcc\xc5\xdb\xb5\xe9 \xc1\xdf\xbf\xa1 \xb5\xa5\xc0\xcc\xc5\xcd\xb0\xa1 \xc0\xdf\xb8\xf8\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
		CTransferableItem::Instance()->ResetRegistrationItemList();
		m_pPostList->ClearItem();
		return;
	}

	if (!VerifyIncludeSameItem()) {
		CallErrorMsg("\xb0\xb0\xc0\xba \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb5\xce \xb0\xa1\xc1\xf6 \xc0\xcc\xbb\xf3 \xc3\xb7\xba\xce\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		CTransferableItem::Instance()->ResetRegistrationItemList();
		m_pPostList->ClearItem();
		return;
	}

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "rmr_notify", 0);
	pForm->SetMessage(MakeStr("\xbc\xf6\xbc\xf6\xb7\xe1\xb0\xa1 %d \xc0\xd4\xb4\xcf\xb4\xd9. \n\xbf\xec\xc6\xed\xb9\xb0\xc0\xbb \xba\xb8\xb3\xbb\xbd\xc3\xb0\xda\xbd\xc0\xb4\xcf\xb1\xee?", GetSendServiceCharge()), false);
	pForm->Open((FRESH_RESULT_CALLBACK)&CPost_SendDlg::OnSendQuestionResult, 2);
}

// post_senddlg.cpp:662
bool CPost_SendDlg::OnSendQuestionResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		SetFadeout(true);
		FrForm::OnOK();
	}
	return true;
}

// post_senddlg.cpp:672
void CPost_SendDlg::OnCancel(void) {
	CTransferableItem::Instance()->ResetRegistrationItemList();
	FrForm::OnCancel();
}

// post_senddlg.cpp:681
bool CPost_SendDlg::OnFriendListResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_bReceiverSet = true;

		if (m_pFriendDlg->IsSelectPerson() || !m_pFriendDlg->HaveFriends()) {
			m_sendUID = m_pFriendDlg->GetUid();

			m_receiverName = m_pReciveUserEdit->GetLine(1, false);
			m_receiverNick = m_pReciveUserEdit->GetLine(1, false);
		} else {
			AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)"\xc4\xa3\xb1\xb8\xb8\xa6 \xbc\xb1\xc5\xc3\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
		}
	} else {
		m_pReciveUserEdit->ClearLine();
		m_pUserNickEdit->ClearLine();

		m_sendUID = (unsigned long)-1;
		m_receiverName.clear();
		m_receiverNick.clear();
	}

	m_pFriendDlg = 0;
	return true;
}

// post_senddlg.cpp:847
void CPost_SendDlg::SetReceiverInfo(unsigned long uid, const sPangYaUserInfo& info) {
	m_sendUID = uid;

	m_receiverNick = info.nickname;
	m_receiverName = info.id;

	if (IsLocalContent((localContentType_t)16)
		&& info.nameByEdit == 1) {
		m_receiverName = m_pReciveUserEdit->GetLine(1, false);
	} else {
		m_receiverName = info.nickname;
	}

	if (m_pReciveUserEdit) {
		m_bReceiverSet = true;
		m_pReciveUserEdit->SetLine(1, m_receiverName.c_str(), 0, false, 0);
		m_pUserNickEdit->SetVisible(true);
	}

	if (m_pUserNickEdit) {
		m_pUserNickEdit->SetLine(1, m_receiverNick.c_str(), 0, false, 0);
	}
}

// post_senddlg.cpp:891
bool CPost_SendDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == FrOK) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = DYNAMIC_CAST(FrEdit, FindChildByName("id"));
				if (m_pReciveUserEdit) {
					const char* front = m_pReciveUserEdit->GetEditText_Front();
					const char* comp = m_pReciveUserEdit->GetEditText_Comp();
					const char* end = m_pReciveUserEdit->GetEditText_End();
					m_pReciveUserEdit->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					m_pReciveUserEdit->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// post_senddlg.cpp:921
std::string CPost_SendDlg::GetLetterMsg(void) {
	static std::string msg;

	msg.clear();

	int lineNum = m_pReciveMsgEdit->GetLineNum();

	for (int i = 1; i <= lineNum; i++) {
		msg += m_pReciveMsgEdit->GetLine(i, false);

		if (i != lineNum)
			msg += "\n";
	}

	return msg;
}

// post_senddlg.cpp:940
int CPost_SendDlg::GetSendListSize(void) {
	return CTransferableItem::Instance()->Get_RegistrationListSize();
}

// post_senddlg.cpp:945
std::list<sMailIncludeItem>* CPost_SendDlg::GetSendList(void) {
	m_sendList.clear();

	sMailIncludeItem item;
	memset(&item, 0, sizeof(item));

	std::list<sTradeItem>* pList = CTransferableItem::Instance()->GetList_RegistrationItem();

	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		item.guid = it->guid;
		item.typeId = it->typeId;
		item.count = it->count;
		item.kind = it->kind;
		item.flag = it->flag;

		m_sendList.push_back(item);
	}

	return &m_sendList;
}

// post_senddlg.cpp:971
bool CPost_SendDlg::VerifySendItemList(void) {
	std::list<sTradeItem>* pList = CTransferableItem::Instance()->GetList_RegistrationItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		unsigned long guid = it->guid;
		unsigned long typeId = it->typeId;
		sItemInfo* pItem = CSharedDoc::Instance()->FindMyItemByGuid(guid);
		if (pItem) {
			if (pItem->m_typeId != typeId) {
				return false;
			}
		}
		int count = it->count;
		if (count > 99) {
			return false;
		}
	}
	return true;
}

// post_senddlg.cpp:999
bool CPost_SendDlg::VerifyIncludeSameItem(void) {
	std::vector<unsigned long> typeIds;

	std::list<sTradeItem>* pList = CTransferableItem::Instance()->GetList_RegistrationItem();
	for (std::list<sTradeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
		typeIds.push_back((*it).typeId);
	}

	unsigned int size = typeIds.size();
	for (unsigned int i = 0; i < size - 1; i++) {
		if (std::count(typeIds.begin(), typeIds.end(), typeIds[i]) > 1) {
			return false;
		}
	}

	return true;
}

// post_senddlg.cpp:1022
int CPost_SendDlg::GetSendServiceCharge(void) {
	if (CTransferableItem::Instance()->Get_RegistrationListSize() == 0) {
		return 100;
	}
	return CTransferableItem::Instance()->Get_RegistrationListSize() * 500;
}

// post_senddlg.cpp:1038 (run-time type and factory registration of CPost_MyItemList)
const WRTTI CPost_MyItemList::m_RTTI("CPost_MyItemList", &FrForm::m_RTTI);

IObject* CPost_MyItemListMakeInstance(void) {
	return new CPost_MyItemList;
}

struct __sCPost_MyItemList {
	__sCPost_MyItemList(void) {
		ObjectFactory().AddObjectFunctor(CPost_MyItemListMakeInstance, "CPost_MyItemList");
	}
};
static __sCPost_MyItemList s_registerCPost_MyItemList;

// post_senddlg.cpp:1040
const sFRESH_MSGMAP* CPost_MyItemList::GetMessageMap(void) const {
	return &CPost_MyItemList::_MsgMap;
}

sFRESH_MSGMAP CPost_MyItemList::_MsgMap = { &FrForm::_MsgMap, &CPost_MyItemList::_MsgEntries[0] };

sFRESH_ENTRY CPost_MyItemList::_MsgEntries[] = {
	// post_senddlg.cpp:1043
	{ "myitem_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_ItemListInit },
	// post_senddlg.cpp:1044
	{ "myitem_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_ItemListOwnerDraw },
	// post_senddlg.cpp:1045
	{ "myitem_list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_ItemListBtnDown },
	// post_senddlg.cpp:1046
	{ "myitem_list", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_ItemListRBtnDown },
	// post_senddlg.cpp:1047
	{ "myitem_list", FRCMD_MOUSEMOVE, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_ItemListMouseMove },
	// post_senddlg.cpp:1048
	{ "myitem_list", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_ItemListDClick },
	// post_senddlg.cpp:1050
	{ "select_itemlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_SelectListInit },
	// post_senddlg.cpp:1051
	{ "select_itemlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_SelectListOwnerDraw },
	// post_senddlg.cpp:1052
	{ "select_itemlist", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_SelectListBtnDown },
	// post_senddlg.cpp:1053
	{ "select_itemlist", FRCMD_MOUSEMOVE, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_SelectListMouseMove },
	// post_senddlg.cpp:1054
	{ "select_itemlist", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPost_SelectListDClick },
	// post_senddlg.cpp:1056
	{ "select_item_img", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_ImgSelect },
	// post_senddlg.cpp:1057
	{ "Edit_array", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPost_EditArrayInit },
	// post_senddlg.cpp:1059
	{ "Btn_Array", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_MyItemList::OnPostBtnArrayInit },
	// post_senddlg.cpp:1060
	{ "Btn_Array", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_MyItemList::OnPostBtnArrayButtonUp },
	// post_senddlg.cpp:1062
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// post_senddlg.cpp:1068
CPost_MyItemList::CPost_MyItemList(void) {
	m_pParent = 0;
	m_pItemCountDlg = 0;
	m_pCountItem = 0;
	m_pBtnArray = 0;
	m_pEditArray = 0;

	m_pArrayDlg = 0;

	m_arrayIndex = -1;

	m_pSelectNBitmap = g_pFresh->GetBitmap("rmr_select_n");
	m_pSelectDBitmap = g_pFresh->GetBitmap("rmr_select_d");

	m_pLineBitmap = g_pFresh->GetBitmap("rmr_line");

	tagWTITLEFONT countInfo;
	memset(&countInfo, 0, sizeof(countInfo));
	countInfo.filename = data_a8f634;
	countInfo.fontw = 16;
	countInfo.fonth = 16;
	countInfo.numPages = 1;
	countInfo.texw = 128;
	countInfo.texh = 64;
	countInfo.pCharSet = "1234567890ym%-.?/:,";

	m_pCountFont = g_resrcmng->GetTitleFont();
	m_pCountFont->Create(&countInfo);

	tagWTITLEFONT pangInfo;
	memset(&pangInfo, 0, sizeof(pangInfo));
	pangInfo.filename = data_a8f630;
	pangInfo.fontw = 9;
	pangInfo.fonth = 16;
	pangInfo.numPages = 1;
	pangInfo.texw = 128;
	pangInfo.texh = 32;
	pangInfo.pCharSet = "1234567890,-";

	m_pPangFont = g_resrcmng->GetTitleFont();
	m_pPangFont->Create(&pangInfo);

	m_pUccStateBitmap[0] = g_pFresh->RegisterBitmap("ucc_state_original");
	m_pUccStateBitmap[1] = g_pFresh->RegisterBitmap("ucc_state_copied");
	m_pUccStateBitmap[2] = g_pFresh->RegisterBitmap("ucc_state_temp_saved");

	m_miniBtn.m_pBitmapN = g_pFresh->RegisterBitmap("rmr_visitor_book_cancel_n");
	m_miniBtn.m_pBitmapO = g_pFresh->RegisterBitmap("rmr_visitor_book_cancel_o");
	m_miniBtn.m_rect = WRect(52.0f, 60.0f, (float)m_miniBtn.m_pBitmapN->Width(), (float)m_miniBtn.m_pBitmapN->Height());
}

// post_senddlg.cpp:1145
CPost_MyItemList::~CPost_MyItemList(void) {
	if (m_pCountFont) {
		m_pCountFont->Erase();
	}

	if (m_pPangFont) {
		m_pPangFont->Erase();
	}

	if (g_resrcmng && m_pCountFont) { g_resrcmng->Release(m_pCountFont); m_pCountFont = 0; }
	if (g_resrcmng && m_pPangFont) { g_resrcmng->Release(m_pPangFont); m_pPangFont = 0; }
}

// post_senddlg.h: no line record (empty constructor of a nested helper struct)
CPost_MyItemList::miniBtn_t::miniBtn_t(void) {
}

// post_senddlg.cpp:1169
void CPost_MyItemList::OnPost_ItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
	if (m_pItemList) {
		m_pItemList->SetMultiSelect(true);
	}
}

// post_senddlg.cpp:1202
void CPost_MyItemList::OnPost_ItemListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pItem->pData == 0)
		return;

	const Bitmap* pSelect;
	if (pItem->selected) {
		pSelect = m_pSelectDBitmap;
		pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
	} else if (pItem->underCursor) {
		pSelect = m_pSelectNBitmap;
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	} else {
		pSelect = 0;
		pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	}

	sTradeItem* pTrade = (sTradeItem*)pItem->pData;

	WRect rc;
	WRect rcLine;
	GetClientRect(rc);
	for (int row = 0; row < 2; row++) {
		float y = row * 110.0f + 87.0f;
		for (int col = 0; col < 3; col++) {
			rcLine = WRect(col * 92.0f + rc.x + 126.0f, y + rc.y, m_pLineBitmap->Width(), m_pLineBitmap->Height());

			pGDI->DrawTexture(m_pLineBitmap, rcLine, 0xFFFFFFFF, 0);
		}
	}

	if (pSelect) {
		pItem = (FrListItem*)param;
		pGDI->DrawTexture(pSelect, WRect(pItem->pos.x - 2.0f, pItem->pos.y, pSelect->Width(), pSelect->Height()), 0xFFFFFFFF, 0);
	}

	if ((pTrade->typeId & 0xFC000000) == 0x8000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pTrade->typeId >> 18) & 0xFF) | 0x4000000);
		if (pChar) {
			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));
			if (pBitmap) {
				pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x + 7.0f, pItem->pos.y + 5.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
			}
		}
	}

	DrawItem(WPoint(pItem->pos.x + 13.0f, pItem->pos.y + 20.0f), pTrade, 0);

	if (IsLocalContent((localContentType_t)0x51)) {
		if ((pTrade->typeId & 0xFC000000) == 0x8000000)
			DrawPartsSlot(pGDI, pItem);
	}
}

// post_senddlg.cpp:1433
void CPost_MyItemList::OnPost_ItemListBtnDown(void) {
	FrListItem* pItem = m_pItemList->GetItemUnderCursor();

	if (!pItem)
		return;

	sTradeItem* pTrade = (sTradeItem*)pItem->pData;

	if (!pTrade) {
		CallErrorMsg("\xbc\xb1\xc5\xc3\xc7\xd1 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xc0\xdf\xb8\xf8\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xd4\xb4\xcf\xb4\xd9.");
		return;
	}

	if (!pItem->selected) {
		pItem->selected = true;
		CallErrorMsg("\xc0\xcc\xb9\xcc \xc3\xdf\xb0\xa1\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xd4\xb4\xcf\xb4\xd9.");
		return;
	}

	if (m_pSelectList->FindItem(pItem)) {
		pItem->selected = false;
		CallErrorMsg("\xb0\xb0\xc0\xba \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc3\xdf\xb0\xa1\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	}

	if (WSingleton<CTransferableItem>::Instance()->Get_RegistrationListSize() >= 4) {
		pItem->selected = false;
		CallErrorMsg("\xc0\xcc\xb9\xcc 4\xb0\xb3\xc0\xc7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb8\xf0\xb5\xce \xbc\xb1\xc5\xc3\xc7\xcf\xbc\xcc\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	}

	if (!WSingleton<CTransferableItem>::Instance()->IsCheckCardParts(pTrade->guid, pTrade->typeId)) {
		pItem->selected = false;
		CallErrorMsg("\xc4\xab\xb5\xe5\xb0\xa1 \xc0\xce\xc3\xa6\xc6\xae \xb5\xc8 \xc0\xc7\xbb\xf3\xc0\xba \xc7\xf6\xc0\xe7 \xbc\xb1\xb9\xb0 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	}

	if ((pTrade->typeId & 0xfc000000) == 0x7c000000) {
		pItem->selected = false;
		CallErrorMsg("\xc4\xab\xb5\xe5\xb4\xc2 \xc7\xf6\xc0\xe7 \xbc\xb1\xb9\xb0 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.");
		return;
	}

	if (pTrade->count > 1) {
		if (!m_pItemCountDlg) {
			m_pCountItem = pItem;
			m_pItemCountDlg = CreateForm<CPost_ItemCountDlg>(g_pFresh->GetManager(), this, "post_itemcount", 0);
			m_pItemCountDlg->SetRect(WRect(pItem->pos.x - 20.0f, pItem->pos.y + 80.0f, m_pItemCountDlg->GetRect().w, m_pItemCountDlg->GetRect().h));
			m_pItemCountDlg->SetSelectItem(pTrade->guid, pTrade->count);
			m_pItemCountDlg->Open((FRESH_RESULT_CALLBACK)&CPost_MyItemList::OnPostItemCountResult, 0x81);
		}
	} else {
		if (WSingleton<CTransferableItem>::Instance()->SetRegistrationItem(pTrade->guid, 1)) {
			if (!m_pImgSelect->IsVisible())
				m_pImgSelect->SetVisible(true);
			if (!m_pSelectList->IsVisible())
				m_pSelectList->SetVisible(true);

			SetList();
		} else {
			pItem->selected = false;
			CallErrorMsg("\xbf\xec\xc6\xed\xbf\xa1 \xc6\xf7\xc7\xd4\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xbf\xc5\xb1\xe2\xc1\xf6 \xb8\xf8\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.");
		}
	}
}

// post_senddlg.cpp:1519
void CPost_MyItemList::OnPost_ItemListMouseMove(void) {
}

// post_senddlg.cpp:1523
void CPost_MyItemList::OnPost_ItemListRBtnDown(void) {
}

// post_senddlg.cpp:1569
void CPost_MyItemList::OnPost_ItemListDClick(void) {
}

// post_senddlg.cpp:1573
void CPost_MyItemList::OnPost_ImgSelect(intptr_t param) {
	m_pImgSelect = DYNAMIC_CAST(FrArea, param);
	if (m_pImgSelect) {
		m_pImgSelect->SetVisible(false);
	}
}

// post_senddlg.cpp:1582
void CPost_MyItemList::OnPost_SelectListInit(intptr_t param) {
	m_pSelectList = DYNAMIC_CAST(FrListBox, param);
	if (m_pSelectList) {
		m_pSelectList->SetVisible(false);
	}
}

// post_senddlg.cpp:1591
void CPost_MyItemList::OnPost_EditArrayInit(intptr_t param) {
	m_pEditArray = DYNAMIC_CAST(FrEdit, param);
	if (m_pEditArray) {
		m_pEditArray->ClearLine();
		// "All"
		m_pEditArray->AddText("\xC0\xFC\x20\xC3\xBC", false, true);
	}
}

// post_senddlg.cpp:1602
void CPost_MyItemList::OnPost_SelectListOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0)
		return;

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	sTradeItem* pTrade = (sTradeItem*)pItem->pData;
	if (pTrade == 0)
		return;

	if ((pTrade->typeId & 0xFC000000) == 0x8000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pTrade->typeId >> 18) & 0xFF) | 0x4000000);
		if (pChar) {
			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));
			if (pBitmap) {
				pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x + 3.0f, pItem->pos.y + 3.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
			}
		}
	}

	WPoint pos;
	if (pItem->underCursor)
		pos = WPoint(pItem->pos.x + 3.0f, pItem->pos.y + 5.0f);
	else
		pos = WPoint(pItem->pos.x + 5.0f, pItem->pos.y + 7.0f);

	DrawItem(pos, pTrade, 0);

	const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
	if ((pItem->underCursor || pItem->selected) && WRect(m_miniBtn.m_rect.x + pItem->pos.x, m_miniBtn.m_rect.y + pItem->pos.y, m_miniBtn.m_rect.w, m_miniBtn.m_rect.h).IsInRect(mousePos)) {
		pGDI->DrawTexture(m_miniBtn.m_pBitmapO, WRect(m_miniBtn.m_rect.x + pItem->pos.x, m_miniBtn.m_rect.y + pItem->pos.y, m_miniBtn.m_rect.w, m_miniBtn.m_rect.h), 0xFFFFFFFF, 0);
	} else {
		pGDI->DrawTexture(m_miniBtn.m_pBitmapN, WRect(m_miniBtn.m_rect.x + pItem->pos.x, m_miniBtn.m_rect.y + pItem->pos.y, m_miniBtn.m_rect.w, m_miniBtn.m_rect.h), 0xFFFFFFFF, 0);
	}

	if (IsLocalContent((localContentType_t)0x51)) {
		if ((pTrade->typeId & 0xFC000000) == 0x8000000)
			DrawPartsSlot(pGDI, pItem);
	}
}

// post_senddlg.cpp:1661
void CPost_MyItemList::OnPost_SelectListBtnDown(void) {
	FrListItem* pItem = m_pSelectList->GetItemUnderCursor();
	if (pItem) {
		WRect rc(m_miniBtn.m_rect.x + pItem->pos.x, m_miniBtn.m_rect.y + pItem->pos.y, m_miniBtn.m_rect.w, m_miniBtn.m_rect.h);
		const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
		if (mousePos.x >= rc.x && mousePos.x <= rc.x + rc.w && mousePos.y >= rc.y && mousePos.y <= rc.y + rc.h) {
			sTradeItem* pTrade = (sTradeItem*)pItem->pData;
			if (!pTrade) {
				CallErrorMsg("\xbc\xb1\xc5\xc3\xc7\xd1 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xc0\xdf\xb8\xf8\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xd4\xb4\xcf\xb4\xd9.");
				return;
			}

			if (WSingleton<CTransferableItem>::Instance()->DelRegistrationItem(pTrade->guid)) {
				if (WSingleton<CTransferableItem>::Instance()->Get_RegistrationListSize() == 0) {
					m_pSelectList->ClearItem();
					m_pSelectList->SetVisible(false);
					m_pImgSelect->SetVisible(false);
				}

				SetList();
			} else {
				pItem->selected = true;
				CallErrorMsg("\xbe\xc6\xc0\xcc\xc5\xdb \xbc\xb1\xc5\xc3 \xc3\xeb\xbc\xd2 \xbd\xc7\xc6\xd0");
			}
		}
	}
}

// post_senddlg.cpp:1701
void CPost_MyItemList::OnPost_SelectListMouseMove(void) {
}

// post_senddlg.cpp:1705
void CPost_MyItemList::OnPost_SelectListDClick(void) {
}

// post_senddlg.cpp:1711
void CPost_MyItemList::OnPostBtnArrayInit(intptr_t param) {
	m_pBtnArray = DYNAMIC_CAST(FrButton, param);
}

// post_senddlg.cpp:1719
void CPost_MyItemList::OnPostBtnArrayButtonUp(void) {
	WRect rect = m_pBtnArray->GetRect();

	if (!m_pArrayDlg) {
		m_pArrayDlg = CreateForm<CStandingInLine>(g_pFresh->GetManager(), this, "arraydlg", 0);
		if (m_pArrayDlg) {
			m_pArrayDlg->Open((FRESH_RESULT_CALLBACK)&CPost_MyItemList::OnPostItemArrayResult,
				WPoint(rect.x, rect.y + rect.h), 1);
		}
	}
}

// post_senddlg.cpp:1841
void CPost_MyItemList::DrawPang(void) {
	if (!g_pFresh->GetManager()->GetGDI())
		return;
	if (!m_pPangFont)
		return;

	char buf[32] = ""; WPoint pos(m_rect.x, m_rect.y);
	static int s_fontWidth = m_pPangFont->GetFontInfo()->fontw;
	static float s_textWidth;

	MakePriceStrA(buf, WSingleton<CSharedDoc>::Instance()->m_myPang);
	s_textWidth = (s_fontWidth - 1.0f) * (strlen(buf) - 1);
	m_pPangFont->Print(g_view, pos.x + 130.0f - s_textWidth, pos.y + 483.0f, buf, 0, 0xffffffff, 0);
}

// post_senddlg.cpp:1859
void CPost_MyItemList::OnOK(void) {
	FrForm::OnOK();
}

// post_senddlg.cpp:1864
bool CPost_MyItemList::OnInit(void) {
	SetList();
	return true;
}

// post_senddlg.cpp:2062
void CPost_MyItemList::SetParent(CPost_SendDlg* pParent) {
	m_pParent = pParent;
}

// post_senddlg.cpp:2068
bool CPost_MyItemList::OnPostItemCountResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_itemCount = 0;
		m_itemCount = m_pItemCountDlg->GetCount();

		if (CTransferableItem::Instance()->SetRegistrationItem(m_pItemCountDlg->GetSelectItem(), m_itemCount)) {
			if (!m_pImgSelect->IsVisible()) m_pImgSelect->SetVisible(true);
			if (!m_pSelectList->IsVisible()) m_pSelectList->SetVisible(true);

			SetList();
		} else {
			m_pCountItem->selected = false;
			CallErrorMsg("\xbe\xc6\xc0\xcc\xc5\xdb \xbc\xb1\xc5\xc3 \xc3\xeb\xbc\xd2 \xbd\xc7\xc6\xd0");
		}
	} else {
		m_pCountItem->selected = false;
		CallErrorMsg("\xbe\xc6\xc0\xcc\xc5\xdb \xbc\xb1\xc5\xc3 \xc3\xeb\xbc\xd2");
	}

	m_pItemCountDlg = 0;
	return true;
}

// post_senddlg.cpp:2103
bool CPost_MyItemList::OnPostItemArrayResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_arrayIndex = m_pArrayDlg->GetSelectNum() - 1;

		m_pEditArray->ClearLine();

		if (m_arrayIndex == -1) {
			// "All"
			m_pEditArray->AddText("\xC0\xFC\x20\xC3\xBC", false, true);
		} else {
			m_pEditArray->AddText(MakeStr("%s", data_af6414[m_arrayIndex]), false, true);
		}
		SetList();
	}

	m_pArrayDlg = 0;

	return true;
}

// post_senddlg.cpp:2129
CPost_ItemCountDlg::CPost_ItemCountDlg(void) {
	m_pMinusBtn = 0;
	m_pPluseBtn = 0;
	m_pCountEdit = 0;

	m_count = 1;
}

// post_senddlg.cpp:2137
CPost_ItemCountDlg::~CPost_ItemCountDlg(void) {
}

// post_senddlg.cpp:2143 (run-time type and factory registration of CPost_ItemCountDlg)
const WRTTI CPost_ItemCountDlg::m_RTTI("CPost_ItemCountDlg", &FrForm::m_RTTI);

IObject* CPost_ItemCountDlgMakeInstance(void) {
	return new CPost_ItemCountDlg;
}

struct __sCPost_ItemCountDlg {
	__sCPost_ItemCountDlg(void) {
		ObjectFactory().AddObjectFunctor(CPost_ItemCountDlgMakeInstance, "CPost_ItemCountDlg");
	}
};
static __sCPost_ItemCountDlg s_registerCPost_ItemCountDlg;

// post_senddlg.cpp:2145
const sFRESH_MSGMAP* CPost_ItemCountDlg::GetMessageMap(void) const {
	return &CPost_ItemCountDlg::_MsgMap;
}

sFRESH_MSGMAP CPost_ItemCountDlg::_MsgMap = { &FrForm::_MsgMap, &CPost_ItemCountDlg::_MsgEntries[0] };

sFRESH_ENTRY CPost_ItemCountDlg::_MsgEntries[] = {
	// post_senddlg.cpp:2148
	{ "btn_minus", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_ItemCountDlg::OnPostMinusInit },
	// post_senddlg.cpp:2149
	{ "btn_minus", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_ItemCountDlg::OnPostMinusBtnUp },
	// post_senddlg.cpp:2150
	{ "btn_pluse", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_ItemCountDlg::OnPostPluseInit },
	// post_senddlg.cpp:2151
	{ "btn_pluse", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&CPost_ItemCountDlg::OnPostPluseBtnUp },

	// post_senddlg.cpp:2153
	{ "input_count", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CPost_ItemCountDlg::OnPostEditItemCountInit },
	// post_senddlg.cpp:2154
	{ "input_count", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&CPost_ItemCountDlg::OnPostEditItemCountEnterKey },

	// post_senddlg.cpp:2156
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// post_senddlg.cpp:2160
void CPost_ItemCountDlg::OnPostMinusInit(intptr_t param) {
	m_pMinusBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pMinusBtn) {
		m_pMinusBtn->SetPushDelay(0.0f);
	}
}

// post_senddlg.cpp:2169
void CPost_ItemCountDlg::OnPostMinusBtnUp(void) {
	m_count = atoi(m_pCountEdit->GetLine(1, false));
	m_count--;

	if (m_count < 1)
		m_count = 1;
	SetText();
}

// post_senddlg.cpp:2181
void CPost_ItemCountDlg::OnPostPluseInit(intptr_t param) {
	m_pPluseBtn = DYNAMIC_CAST(FrButton, param);
	if (m_pPluseBtn) {
		m_pPluseBtn->SetPushDelay(0.0f);
	}
}

// post_senddlg.cpp:2190
void CPost_ItemCountDlg::OnPostPluseBtnUp(void) {
	m_count = atoi(m_pCountEdit->GetLine(1, false));
	m_count++;

	if (m_count > m_selectCount)
		m_count = m_selectCount;
	SetText();
}

// post_senddlg.cpp:2201
void CPost_ItemCountDlg::OnPostEditItemCountInit(intptr_t param) {
	m_pCountEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pCountEdit) {
		m_pCountEdit->SetCharLimit(2, true);
	}

	SetText();
}

// post_senddlg.cpp:2211
int CPost_ItemCountDlg::GetCount(void) {
	m_count = atoi(m_pCountEdit->GetLine(1, false));
	int maxCount = m_selectCount;
	m_count = m_count < 1 ? 1 : (m_count > maxCount ? maxCount : m_count);
	return m_count;
}

// post_senddlg.cpp:2219
void CPost_ItemCountDlg::SetText(void) {
	if (m_pCountEdit) {
		m_pCountEdit->ClearLine();
		m_pCountEdit->AddText(MakeStr("%d", m_count), false, true);
	}
}

// post_senddlg.cpp:2228
void CPost_ItemCountDlg::SetSelectItem(unsigned long item, int count) {
	m_selectItem = item;
	m_selectCount = count;
}

// post_senddlg.cpp:2233
unsigned long CPost_ItemCountDlg::GetSelectItem(void) {
	return m_selectItem;
}

// post_senddlg.cpp:2238
bool CPost_ItemCountDlg::OnPostEditItemCountEnterKey(int) {
	OnOK();
	return true;
}
