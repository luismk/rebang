// Original: d:\build\custom\temp\code\client\projectg\checkoutdlg.cpp
#include <stdlib.h>
#include <string.h>

#include "CheckoutDlg.h"
#include "ObjectFactory.h"
#include "CaddieInfoDlg.h"
#include "bitmap.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "frdesktop.h"
#include "fredit.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "Browser.h"
#include "logininfo.h"
#include "TaskManager.h"
#include "actor.h"
#include "InputManager.h"
#include "SharedDoc.h"
#include "commonutil.h"

// The project's SDK has no <wininet.h>; the function is declared locally.
extern "C" __declspec(dllimport) BOOL __stdcall InternetSetCookieA(LPCSTR lpszUrl, LPCSTR lpszCookieName, LPCSTR lpszCookieData);

// checkoutdlg.h:77
bool FrCheckoutDlg::IsUnderLevel(void) {
	return m_bUnderLevel;
}

// checkoutdlg.h:78
bool FrCheckoutDlg::IsOverLevel(void) {
	return m_bOverLevel;
}

// checkoutdlg.h: no line record (empty constructor of a nested helper class)
FrCheckoutDlg::MiniBtn::MiniBtn(void) {
}

// checkoutdlg.cpp:32 (run-time type and factory registration of FrCheckoutDlg)
const WRTTI FrCheckoutDlg::m_RTTI("FrCheckoutDlg", &FrForm::m_RTTI);

IObject* FrCheckoutDlgMakeInstance(void) {
	return new FrCheckoutDlg;
}

struct __sFrCheckoutDlg {
	__sFrCheckoutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCheckoutDlgMakeInstance, "FrCheckoutDlg");
	}
};
static __sFrCheckoutDlg s_registerFrCheckoutDlg;

// checkoutdlg.cpp:34
const sFRESH_MSGMAP* FrCheckoutDlg::GetMessageMap(void) const {
	return &FrCheckoutDlg::_MsgMap;
}

// checkoutdlg.cpp:84
// A rental period offered by the shop (8 bytes).
struct days_t {
	const char* const m_name;   // 0x0
	const int m_days;           // 0x4
};

days_t days[] = {
	{ "1\xc0\xcf", 1 },
	{ "7\xc0\xcf", 7 },
	{ "15\xc0\xcf", 15 },
	{ "30\xc0\xcf", 30 },
	{ "365\xc0\xcf", 365 },
	{ "\xb9\xab\xc1\xa6\xc7\xd1", 0 },
};

// checkoutdlg.cpp:96
FrCheckoutDlg::FrCheckoutDlg(void) {
	m_pCheckout = 0;
	m_pCart = 0;
	m_pPrice = 0;
	m_pMyMoney = 0;
	m_pMoneyLeft = 0;
	m_pPrice_Cookie = 0;
	m_pMyMoney_Cookie = 0;
	m_pMoneyLeft_Cookie = 0;
	m_pNumStatic = 0;

	m_pExplaneStatic = 0;

	m_pNum = 0;
	m_pNumPrev = 0;
	m_pNumNext = 0;

	m_flag16a = false;
	m_bUnderLevel = false;
	m_bOverLevel = false;
	m_flag180 = false;
	m_flag181 = false;
	m_bCheckoutFlag = false;
	m_value184 = 0;

	m_num = 1;

	m_pang = 0;
	m_cookie = 0;

	m_value1a0 = 0;
	m_flag1a8 = false;

	m_coupons.clear();
	m_bCouponFlag = true;
	m_selectedCoupon = -1;
	m_value1c0 = 0;

	m_value1c8 = 0;
	m_value1d0 = 0;

	m_value1d8 = 0;

	m_dayNameCount = 0;
	memset(m_dayNames, 0, sizeof(m_dayNames));

	m_miniBtn[0].m_pCartItem = 0;
	m_miniBtn[1].m_pCartItem = 0;
	m_flag1dc = false;
}

// checkoutdlg.cpp:167
std::list<FrCheckoutDlg::CartItem>& FrCheckoutDlg::Cart(void) {
	return m_cart;
}

// checkoutdlg.cpp:172
void FrCheckoutDlg::Process(void) {
	if (m_pNum) {
		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num != m_num) {
			SetPrice(num);
		}
	}
}

// checkoutdlg.cpp:744
bool FrCheckoutDlg::OnChargeResult(int ret, FrForm*) {
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

// checkoutdlg.cpp:1085
bool CompareUsableCoupon(const FrCheckoutDlg::sCoupon& a, const FrCheckoutDlg::sCoupon& b) {
	return a.m_bUsable > b.m_bUsable;
}

// checkoutdlg.cpp:1091
bool ComparePeriodCoupon(const FrCheckoutDlg::sCoupon& a, const FrCheckoutDlg::sCoupon& b) {
	return a.m_period < b.m_period;
}

// checkoutdlg.cpp:1775
unsigned long FrCheckoutDlg::GetGuidSelectedCoupon(void) {
	m_bCouponFlag = false;

	return 0;
}

// checkoutdlg.cpp:1802
void FrCheckoutDlg::OnCheckoutInit(intptr_t param) {
	m_pCheckout = DYNAMIC_CAST(FrButton, param);

	m_pClockIcon = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockRect = WRect(40.0f, 35.0f, m_pClockIcon->Width(), m_pClockIcon->Height());
}

// checkoutdlg.cpp:2192
void FrCheckoutDlg::OpenCaddieInfo(const sCaddieInfo& info) {
	FrWnd* pWnd = g_pFresh->GetManager()->GetDesktop()->FindChildByName("caddie_info");
	if (pWnd == 0) {
		// The parent goes through a type cast of a null window: its __rtti_obj store remains.
		FrCaddieInfoDlg* pDlg = CreateForm<FrCaddieInfoDlg>(g_pFresh->GetManager(), this, "caddie_info", DYNAMIC_CAST(FrWnd, 0));
		if (pDlg) {
			pDlg->SetCaddieInfo(info);
			pDlg->Open(0, 3);
		}
	}
}

// checkoutdlg.cpp:2206
void FrCheckoutDlg::OnDaysInit(intptr_t param) {
	m_miniBtn[0].m_pDays = DYNAMIC_CAST(FrComboBox, param);
}

// checkoutdlg.cpp:2211
void FrCheckoutDlg::OnDays2Init(int param) {
	m_miniBtn[1].m_pDays = DYNAMIC_CAST(FrComboBox, param);
}

// checkoutdlg.cpp:2216
void FrCheckoutDlg::OnDaysBtnDown(void) {
	if (m_dayNameCount > 0) {
		const char* text = m_miniBtn[0].m_pDays->GetLine(1, false);

		for (int i = 0; i < m_dayNameCount; ++i) {
			if (_stricmp(m_dayNames[i], text) == 0) {
				m_miniBtn[0].m_pCartItem->m_days = i;
			}
		}
	} else {
		const char* text = m_miniBtn[0].m_pDays->GetLine(1, false);

		for (int i = 0; i < 5; ++i) {
			if (_stricmp(days[i].m_name, text) == 0) {
				m_miniBtn[0].m_pCartItem->m_days = i;
			}
		}
	}

	RecalculateTotalCookie(m_pang, m_cookie, true);
}

// checkoutdlg.cpp:2247
void FrCheckoutDlg::OnDays2BtnDown(void) {
	if (m_dayNameCount > 0) {
		const char* text = m_miniBtn[1].m_pDays->GetLine(1, false);

		for (int i = 0; i < m_dayNameCount; ++i) {
			if (_stricmp(m_dayNames[i], text) == 0) {
				m_miniBtn[1].m_pCartItem->m_days = i;
			}
		}
	} else {
		const char* text = m_miniBtn[1].m_pDays->GetLine(1, false);

		for (int i = 0; i < 5; ++i) {
			if (_stricmp(days[i].m_name, text) == 0) {
				m_miniBtn[1].m_pCartItem->m_days = i;
			}
		}
	}

	RecalculateTotalCookie(m_pang, m_cookie, true);
}

// checkoutdlg.cpp:2278
void FrCheckoutDlg::OnCartInit(intptr_t param) {
	m_pCart = DYNAMIC_CAST(FrListBox, param);
}

// checkoutdlg.cpp:2872
void FrCheckoutDlg::OnPriceInit(intptr_t param) {
	m_pPrice = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2878
void FrCheckoutDlg::OnMyMoneyInit(intptr_t param) {
	m_pMyMoney = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2884
void FrCheckoutDlg::OnMoneyLeftInit(intptr_t param) {
	m_pMoneyLeft = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2890
void FrCheckoutDlg::OnPrice_CookieInit(intptr_t param) {
	m_pPrice_Cookie = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2898
void FrCheckoutDlg::OnMyMoney_CookieInit(intptr_t param) {
	m_pMyMoney_Cookie = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2906
void FrCheckoutDlg::OnMoneyLeft_CookieInit(intptr_t param) {
	m_pMoneyLeft_Cookie = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2914
void FrCheckoutDlg::OnNumStaticInit(intptr_t param) {
	m_pNumStatic = DYNAMIC_CAST(FrStatic, param);
}

// checkoutdlg.cpp:2920
void FrCheckoutDlg::OnExplaneStaticInit(intptr_t param) {
	m_pExplaneStatic = DYNAMIC_CAST(FrStatic, param);
}

// checkoutdlg.cpp:2926
void FrCheckoutDlg::OnNumInit(intptr_t param) {
	m_pNum = DYNAMIC_CAST(FrEdit, param);
}

// checkoutdlg.cpp:2931
bool FrCheckoutDlg::OnNumEnterKey(intptr_t key) {
	return false;
}

// checkoutdlg.cpp:2937
void FrCheckoutDlg::OnNumPrevInit(intptr_t param) {
	m_pNumPrev = DYNAMIC_CAST(FrButton, param);
	if (m_pNumPrev) {
		m_pNumPrev->SetPushDelay(0.0f);
	}
}

// checkoutdlg.cpp:2945
void FrCheckoutDlg::OnNumPrevLBtnUp(void) {
	if (m_pNum) {
		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num > 1) {
			SetPrice(num - 1);
		}
	}
}

// checkoutdlg.cpp:2963
void FrCheckoutDlg::OnNumNextInit(intptr_t param) {
	m_pNumNext = DYNAMIC_CAST(FrButton, param);
	if (m_pNumNext) {
		m_pNumNext->SetPushDelay(0.0f);
	}
}

// checkoutdlg.cpp:2971
void FrCheckoutDlg::OnNumNextLBtnUp(void) {
	if (m_pNum) {
		const char* text = m_pNum->GetLine(1, false);
		int num = atoi(text);

		if (num < 99) {
			SetPrice(num + 1);
		}
	}
}
