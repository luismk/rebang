// Original: d:\build\custom\temp\code\client\projectg\giftdlg.cpp
#include "NetworkSystem.h"
#include "GiftDlg.h"
#include "ObjectFactory.h"
#include "EmoticonDlg.h"
#include "FriendDlg.h"
#include "Packet.h"
#include "buddymanager.h"
#include "../shared/globalgamedefine.h"
#include "commonutil.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frlistbox.h"
#include "bitmap.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "fredit.h"
#include "frstatic.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "binstr.h"
#include "Browser.h"
#include "logininfo.h"
#include "TaskManager.h"
#include "actor.h"
#include "InputManager.h"
#include "SharedDoc.h"
#include "commonutil.h"

// The project's SDK has no <wininet.h>; the function is declared locally.
extern "C" __declspec(dllimport) BOOL __stdcall InternetSetCookieA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData);

// The default rental periods (defined with the checkout dialog)
struct days_t {
	const char* m_name;   // 0x0
	int m_days;           // 0x4
};

extern days_t days[];

// giftdlg.cpp:26 (run-time type and factory registration of FrGiftDlg)
const WRTTI FrGiftDlg::m_RTTI("FrGiftDlg", &FrForm::m_RTTI);

IObject* FrGiftDlgMakeInstance(void) {
	return new FrGiftDlg;
}

struct __sFrGiftDlg {
	__sFrGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGiftDlgMakeInstance, "FrGiftDlg");
	}
};
static __sFrGiftDlg s_registerFrGiftDlg;

// giftdlg.cpp:28
const sFRESH_MSGMAP* FrGiftDlg::GetMessageMap(void) const {
	return &FrGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrGiftDlg::_MsgEntries[0] };

// giftdlg.cpp:30
sFRESH_ENTRY FrGiftDlg::_MsgEntries[] = {
	{ "checkout", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnCheckoutInit },
	{ "checkout", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnCheckoutBtnUp },
	{ "friend", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnFriendBtnInit },
	{ "friend", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnFriendBtnUp },
	{ "cart", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnCartInit },
	{ "cart", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnCartOwnerDraw },
	{ "sxid", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnSxIdInit },
	{ "edid", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnEdIdInit },
	{ "edid", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrGiftDlg::OnEdIdEnterKey },
	{ "edid", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnEdIdDown },
	{ "id_check", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnBtIdCheckInit },
	{ "id_check", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnBtIdCheckUp },
	{ "sxnick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnSxNickInit },
	{ "ednick", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnEdNickInit },
	{ "ednick", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrGiftDlg::OnEdNickEnterKey },
	{ "ednick", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnEdNickDown },
	{ "emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnEmoticonBtnInit },
	{ "emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnEmoticonBtnUp },
	{ "nick_check", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnBtNickCheckInit },
	{ "nick_check", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnBtNickCheckUp },
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnMsgInit },
	{ "message", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrGiftDlg::OnMsgEnterKey },
	{ "confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnEdConfirmInit },
	{ "price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnPriceInit },
	{ "my_money", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnMyMoneyInit },
	{ "money_left", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnMoneyLeftInit },
	{ "price_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnPrice_CookieInit },
	{ "my_money_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnMyMoney_CookieInit },
	{ "money_left_cookie", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnMoneyLeft_CookieInit },
	{ "num_static", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnNumStaticInit },
	{ "num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnNumInit },
	{ "num_prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnNumPrevInit },
	{ "num_prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnNumPrevBtnUp },
	{ "num_next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnNumNextInit },
	{ "num_next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnNumNextBtnUp },
	{ "days", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnDaysInit },
	{ "days", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGiftDlg::OnDaysBtnDown },
	{ "warn", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnWarnInit },
	{ "explane", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftDlg::OnExplaneStaticInit },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// giftdlg.h: no line record (empty constructor of a nested helper class)
FrGiftDlg::MiniBtn::MiniBtn(void) {
}

// giftdlg.cpp:77
FrGiftDlg::FrGiftDlg(void) {
	memset(&m_item, 0, sizeof(m_item));
	m_receiverUID = 0xffffffff;
	m_receiverNick = "";
	m_bPayPang = false;
	m_bPayCookie = false;

	m_pFriendBtn = 0;
	m_pCheckoutBtn = 0;
	m_pCart = 0;
	m_pPrice = 0;
	m_pMyMoney = 0;
	m_pMoneyLeft = 0;
	m_pPriceCookie = 0;
	m_pMyMoneyCookie = 0;
	m_pMoneyLeftCookie = 0;
	m_pNumStatic = 0;
	m_pNum = 0;
	m_pNumPrev = 0;
	m_pNumNext = 0;
	m_pFriendListDlg = 0;
	m_pBtNickCheck = 0;
	m_pWarn = 0;

	m_pExplaneStatic = 0;

	m_pSxNick = 0;

	m_pEmoticonBtn = 0;
	m_pEmoticonDlg = 0;


	m_pangAmount = 0;
	m_cookieAmount = 0;


	m_totalCookie = 0;
	m_bTotalCookieSet = false;


	m_periodCount = 0;
	memset(m_periods, 0, sizeof(m_periods));
	m_receiverMode = -1;
}

// giftdlg.cpp:476
void FrGiftDlg::SetReceiverInfo(unsigned long uid, const sPangYaUserInfo& info) {
	char text[32];   // unused; its /GS cookie survives

	m_receiverUID = uid;

	if (m_pEdNick) {
		m_pEdNick->SetLine(1, info.nickname, 0, false, 0);
	}

	if (m_pEdConfirm) {
		switch (m_receiverMode) {
		case 1:
			m_pEdConfirm->SetLine(1, info.nickname, 0, false, 0);
			m_receiverNick = info.nickname;
			break;
		case 0: {
			if (m_pEdId) {
				m_pEdId->ClearLine();
			}

			std::string id(info.id);
			unsigned int len = id.length();
			id.erase(id.begin() + len / 2, id.end());
			len -= id.length();
			id.insert(id.end(), len, '*');

			if (m_pEdConfirm) {
				m_pEdConfirm->SetLine(1, id.c_str(), 0, false, 0);
			}

			m_receiverNick = info.id;
			break;
		}
		}
	}
}

// giftdlg.cpp:533
void FrGiftDlg::SetPrice(int num) {
	m_bPayPang = true;
	m_bPayCookie = true;

	if (m_pPrice && m_pMyMoney && m_pangPrice) {
		char buf[32] = { 0 }; MakePriceStrA(buf, num * m_pangPrice);

		strcat(buf, "\xc6\xce");
		m_pPrice->SetLine(1, buf, 0, false, 0);

		__int64 myPang = WSingleton<CSharedDoc>::Instance()->m_myPang;

		if (myPang - num * m_pangPrice < 0) {
			if (m_pMoneyLeft) {
				MakePriceStrA(buf, num * m_pangPrice - myPang);
				strcat(buf, "\xc6\xce \xba\xce\xc1\xb7");
				m_pMoneyLeft->SetLine(1, buf, 0xFFFF0000, false, 0);
			}

			m_bPayPang = false;
		}
		else {
			if (m_pMoneyLeft) {
				MakePriceStrA(buf, myPang - num * m_pangPrice);
				strcat(buf, "\xc6\xce");
				m_pMoneyLeft->SetLine(1, buf, 0, false, 0);
			}
		}
	}
	else if (m_pMoneyLeft) {
		m_pMoneyLeft->SetLine(1, m_pMyMoney->GetLine(1, false), 0, false, 0);
	}

	if (m_pPriceCookie && m_pMyMoneyCookie && m_cookiePrice) {
		char buf[32] = { 0 };
		sprintf(buf, "%d\xc4\xed\xc5\xb0", m_cookiePrice);

		m_pPriceCookie->SetLine(1, buf, 0, false, 0);

		__int64 myCash = WSingleton<CSharedDoc>::Instance()->m_myCash;

		if (myCash - m_cookiePrice < 0) {
			if (m_pMoneyLeftCookie) {
				MakePriceStrA(buf, m_cookiePrice - myCash);
				strcat(buf, "\xc4\xed\xc5\xb0 \xba\xce\xc1\xb7");
				m_pMoneyLeftCookie->SetLine(1, buf, 0xFFFF0000, false, 0);
			}

			m_bPayCookie = false;
		}
		else {
			if (m_pMoneyLeftCookie) {
				sprintf(buf, "%d\xc4\xed\xc5\xb0", myCash - m_cookiePrice);
				m_pMoneyLeftCookie->SetLine(1, buf, 0, false, 0);
			}
		}
	}
	else if (m_pMoneyLeftCookie) {
		m_pMoneyLeftCookie->SetLine(1, m_pMyMoneyCookie->GetLine(1, false), 0, false, 0);
	}

	if (m_pNum)
		m_pNum->SetLine(1, MakeStr("%d", num), 0, false, 0);
}

// giftdlg.cpp:618
bool FrGiftDlg::OnFriendListResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		m_receiverMode = 0; m_receiverUID = m_pFriendListDlg->GetUid();
		m_receiverNick = m_pEdConfirm->GetLine(1, false);
	}

	m_pFriendListDlg = 0;

	return true;
}

// giftdlg.cpp:653
bool FrGiftDlg::OnChargeResult(int ret, FrForm*) {
	if (ret == FrOK) {
		WSingleton<CBrowser>::Instance()->InitializePostArgument();

		switch (WSingleton<CSharedDoc>::Instance()->m_billingType) {
		case 2:
			WSingleton<CBrowser>::Instance()->AddPostArgument("sn", "9");
			WSingleton<CBrowser>::Instance()->AddPostArgument("mn", MakeStr("%d", CLoginInfo::Instance()->AuthUid()));
			WSingleton<CBrowser>::Instance()->OpenPost("https://qa.www.gametree.co.kr/InGame/Gateway.aspx", false, true);
			break;

		case 4:
			if (!InternetSetCookieA("https://alpha-bill.hangame.com/", NULL, ((NhnLogin*)CLoginInfo::Instance()->QueryInterface())->GetSiteCookie())) {
				GetLastError();
				MessageBoxA(NULL, "Cookie Setting Fail!", "Error", MB_OK);
				return false;
			}

			WSingleton<CBrowser>::Instance()->AddPostArgument("CHNL", "HG");
			WSingleton<CBrowser>::Instance()->OpenPost("https://alpha-bill.hangame.com/pay/charge.nhn", false, true);
			break;

		default:
			return false;
		}

		AfxGetTask()->GetActor("Shop") << MsgObject(0, 0xC2, 0, 0, 0, 0, 0);
		g_input->Reset();

		Close(FrCANCEL, true);
	}

	return true;
}

// giftdlg.cpp:763
bool FrGiftDlg::OnConfirmResult(int ret, FrForm*) {
	if (ret == FrOK) {
		GiftProgress();
	}

	return true;
}

// giftdlg.cpp:789
void FrGiftDlg::OnCheckoutInit(intptr_t param) {
	m_pCheckoutBtn = DYNAMIC_CAST(FrButton, param);
}

// giftdlg.cpp:863
void FrGiftDlg::OnFriendBtnInit(intptr_t param) {
	m_pFriendBtn = DYNAMIC_CAST(FrButton, param);
}

// giftdlg.cpp:869
void FrGiftDlg::OnFriendBtnUp(void) {
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

	if (!m_pFriendListDlg) {
		m_pFriendListDlg = CreateForm<FrFriendDlg>(g_pFresh->GetManager(), this, "friend_list_form", 0);
		if (m_pFriendListDlg) {
			m_pEdId->ClearLine();

			m_pFriendListDlg->SetParent(this);
			m_pFriendListDlg->EnableDrag(false);
			m_pFriendListDlg->Open((FRESH_RESULT_CALLBACK)&FrGiftDlg::OnFriendListResult, WPoint(582.0f, 35.0f), 1);
		}
	}
}

// giftdlg.cpp:901
void FrGiftDlg::OnCartInit(intptr_t param) {
	m_pCart = DYNAMIC_CAST(FrListBox, param);

	m_pClock = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_rcClock = WRect(40.0f, 35.0f, m_pClock->Width(), m_pClock->Height());
}

// giftdlg.cpp:1353
void FrGiftDlg::OnSxIdInit(intptr_t param) {
	m_pSxId = DYNAMIC_CAST(FrStatic, param);
	if (m_pSxId) {
		m_pSxId->SetVisible(false);
	}
}

// giftdlg.cpp:1365
void FrGiftDlg::OnEdIdInit(intptr_t param) {
	m_pEdId = DYNAMIC_CAST(FrEdit, param);
	if (m_pEdId) {
		m_pEdId->SetKeyFocus(true);
	}

	if (m_pEdId) {
		m_pEdId->SetVisible(false);
	}
}

// giftdlg.cpp:1380
bool FrGiftDlg::OnEdIdEnterKey(int) {
	return false;
}

// giftdlg.cpp:1387
void FrGiftDlg::OnEdIdDown(void) {
}

// giftdlg.cpp:1394
void FrGiftDlg::OnEdNickDown(void) {
	m_receiverMode = -1;
}

// giftdlg.cpp:1401
void FrGiftDlg::OnBtIdCheckInit(intptr_t param) {
	m_pBtIdCheck = DYNAMIC_CAST(FrButton, param);
	if (m_pBtIdCheck) {
		m_pBtIdCheck->SetVisible(false);
	}
}

// giftdlg.cpp:1413
void FrGiftDlg::OnBtIdCheckUp(void) {
}

// giftdlg.cpp:1446
void FrGiftDlg::OnSxNickInit(intptr_t param) {
	m_pSxNick = DYNAMIC_CAST(FrStatic, param);
}

// giftdlg.cpp:1451
void FrGiftDlg::OnEdNickInit(intptr_t param) {
	m_pEdNick = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1463
bool FrGiftDlg::OnEdNickEnterKey(int) {
	OnBtNickCheckUp();
	return false;
}

// giftdlg.cpp:1470
void FrGiftDlg::OnMsgInit(intptr_t param) {
	m_pMsg = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1476
bool FrGiftDlg::OnMsgEnterKey(int) {
	OnCheckoutBtnUp();
	return false;
}

// giftdlg.cpp:1483
void FrGiftDlg::OnEdConfirmInit(intptr_t param) {
	m_pEdConfirm = DYNAMIC_CAST(FrEdit, param);
	if (m_pEdConfirm) {
		m_pEdConfirm->SetLine(1, "<\xc8\xae\xc0\xce \xbe\xc8\xb5\xca>", 0, false, 0);
	}
}

// giftdlg.cpp:1492
void FrGiftDlg::OnBtNickCheckInit(intptr_t param) {
	m_pBtNickCheck = DYNAMIC_CAST(FrButton, param);
}

// giftdlg.cpp:1498
void FrGiftDlg::OnBtNickCheckUp(void) {
	if (m_pEdConfirm)
		m_pEdConfirm->ClearLine();

	if (m_pEdNick) {
		const char* nick = m_pEdNick->GetLine(1, false);
		int len = strlen(nick);
		if (len <= 0) {
			AfxGetTask()->GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb9\xde\xb4\xc2\xc0\xcc\xc0\xc7 \xb4\xeb\xc8\xad\xb8\xed\xc0\xbb \xb3\xd6\xbe\xee\xc1\xd6\xbc\xbc\xbf\xe4", 0, 0, 0, 0);
		} else if (len >= 0x16) {
			AfxGetTask()->GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)MakeStr("\xb4\xeb\xc8\xad\xb8\xed\xc0\xba %d\xc0\xda \xb1\xee\xc1\xf6 \xc0\xd4\xb7\xc2\xc7\xd2\xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0x16), 0, 0, 0, 0);
		} else if (strcmp(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().nickname, nick) != 0) {
			WSendPacket packet((enumClientPacket)7);

			packet.Encode1(0);

			packet.EncodeStr(std::string(nick));
			packet.Send(SEND_TO_DEFAULT);

			m_receiverMode = 0;

			AfxGetTask()->GetActor("Shop") << MsgObject(0, 1, 0, 0, 0, 0, 0);
		} else {
			AfxGetTask()->GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xda\xb1\xe2 \xc0\xda\xbd\xc5\xbf\xa1\xb0\xd4\xb4\xc2 \xbc\xb1\xb9\xb0\xc0\xbb \xba\xb8\xb3\xbe \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
		}
	}

	if (m_pMsg)
		m_pMsg->SetKeyFocus(true);
}

// giftdlg.cpp:1552
void FrGiftDlg::OnPriceInit(intptr_t param) {
	m_pPrice = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1558
void FrGiftDlg::OnMyMoneyInit(intptr_t param) {
	m_pMyMoney = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1564
void FrGiftDlg::OnMoneyLeftInit(intptr_t param) {
	m_pMoneyLeft = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1570
void FrGiftDlg::OnPrice_CookieInit(intptr_t param) {
	m_pPriceCookie = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1578
void FrGiftDlg::OnMyMoney_CookieInit(intptr_t param) {
	m_pMyMoneyCookie = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1586
void FrGiftDlg::OnMoneyLeft_CookieInit(intptr_t param) {
	m_pMoneyLeftCookie = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1594
void FrGiftDlg::OnNumStaticInit(intptr_t param) {
	m_pNumStatic = DYNAMIC_CAST(FrStatic, param);
}

// giftdlg.cpp:1600
void FrGiftDlg::OnNumInit(intptr_t param) {
	m_pNum = DYNAMIC_CAST(FrEdit, param);
}

// giftdlg.cpp:1606
void FrGiftDlg::OnNumPrevInit(intptr_t param) {
	m_pNumPrev = DYNAMIC_CAST(FrButton, param);
	if (m_pNumPrev) {
		m_pNumPrev->SetPushDelay(0.0f);
	}
}

// giftdlg.cpp:1614
void FrGiftDlg::OnNumPrevBtnUp(void) {
	if (m_pNum) {
		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num > 1) {
			SetPrice(num - 1);
		}
	}
}

// giftdlg.cpp:1627
void FrGiftDlg::OnNumNextInit(intptr_t param) {
	m_pNumNext = DYNAMIC_CAST(FrButton, param);
	if (m_pNumNext) {
		m_pNumNext->SetPushDelay(0.0f);
	}
}

// giftdlg.cpp:1635
void FrGiftDlg::OnNumNextBtnUp(void) {
	if (m_pNum) {
		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num < 10) {
			SetPrice(num + 1);
		}
	}
}

// giftdlg.cpp:1831
void FrGiftDlg::OnDaysInit(intptr_t param) {
	m_pDays = DYNAMIC_CAST(FrComboBox, param);
}

// giftdlg.cpp:1836
void FrGiftDlg::OnDaysBtnDown(void) {
	if (m_periodCount > 0) {
		const char* text = m_pDays->GetLine(1, false);
		for (int i = 0; i < m_periodCount; i++) {
			if (stricmp(m_periods[i].name, text) == 0) {
				m_daysIndex = i;
			}
		}
	} else {
		const char* text = m_pDays->GetLine(1, false);
		for (int i = 0; i < 5; i++) {
			if (stricmp(days[i].m_name, text) == 0) {
				m_daysIndex = i;
			}
		}
	}

	RecalculateTotalCookie(m_pangAmount, m_cookieAmount, true);
}

// giftdlg.cpp:1868
void FrGiftDlg::OnWarnInit(intptr_t param) {
	m_pWarn = DYNAMIC_CAST(FrStatic, param);
	if (m_pWarn) {
		m_pWarn->SetCaption("\xc6\xce \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xc4\xa3\xb1\xb8\xbf\xa1\xb0\xd4\xb8\xb8 \xbc\xb1\xb9\xb0\xc7\xd2 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.");
	}
}

// giftdlg.cpp:1878
void FrGiftDlg::OnEmoticonBtnInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// giftdlg.cpp:1883
void FrGiftDlg::OnEmoticonBtnUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (m_pEmoticonDlg) {
			WPoint pt(0, 0);
			const WRect& rect = m_pEmoticonBtn->GetRect();
			pt.x = rect.x - 200.0f;
			pt.y = rect.y + 24.0f;

			m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrGiftDlg::OnEmoticonResult, pt, 3);
		}
	}
}

// giftdlg.cpp:1901
void FrGiftDlg::OnExplaneStaticInit(intptr_t param) {
	m_pExplaneStatic = DYNAMIC_CAST(FrStatic, param);
}

// giftdlg.cpp:1907
bool FrGiftDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == FrOK) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = DYNAMIC_CAST(FrEdit, FindChildByName("id"));
				if (m_pEdNick) {
					const char* front = m_pEdNick->GetEditText_Front();
					const char* comp = m_pEdNick->GetEditText_Comp();
					const char* end = m_pEdNick->GetEditText_End();
					m_pEdNick->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					m_pEdNick->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}
