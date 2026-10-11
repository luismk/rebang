// Original: d:\build\custom\temp\code\client\projectg\giftalarmdlg.cpp
#include <windows.h>
#include <string.h>
#include "GiftAlarmDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "commonutil.h"
#include "fresh.h"
#include "SharedDoc.h"

// Item data of a gift-list row: the received gift and the sender message
// split over two lines (new char[0x50] each).
struct sGiftAlarmItem {
	void* pGift;       // 0x0, element of the shared doc's gift list
	char* pLines[2];   // 0x4
};
CHECK_SIZE(sGiftAlarmItem, 0xC);

// Item data of a treasure-gift row (new 0xC8).
struct sTreasureAlarmItem {
	unsigned long typeId;    // 0x0
	unsigned short count;    // 0x4
	char name[0x40];         // 0x6
	char desc[0x80];         // 0x46
};
CHECK_SIZE(sTreasureAlarmItem, 0xC8);

// Blink state of a dialog's caption (OnProc): set when the caption must be
// rewritten, and whether it is shown.
struct sCaptionBlink {
	bool update;   // 0x0
	bool show;     // 0x1
};

// The OnProc blink states (no symbol): FrGiftAlarmDlg's, CTreasureAlarmDlg's.
extern "C" {
sCaptionBlink data_af7531;
sCaptionBlink data_af7533;
}

// giftalarmdlg.cpp:19 (run-time type and factory registration of FrGiftAlarmDlg)
const WRTTI FrGiftAlarmDlg::m_RTTI("FrGiftAlarmDlg", &FrForm::m_RTTI);

IObject* FrGiftAlarmDlgMakeInstance(void) {
	return new FrGiftAlarmDlg;
}

struct __sFrGiftAlarmDlg {
	__sFrGiftAlarmDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGiftAlarmDlgMakeInstance, "FrGiftAlarmDlg");
	}
};
static __sFrGiftAlarmDlg s_registerFrGiftAlarmDlg;

// giftalarmdlg.cpp:21
const sFRESH_MSGMAP* FrGiftAlarmDlg::GetMessageMap(void) const {
	return &FrGiftAlarmDlg::_MsgMap;
}

sFRESH_MSGMAP FrGiftAlarmDlg::_MsgMap = { &FrForm::_MsgMap, &FrGiftAlarmDlg::_MsgEntries[0] };

sFRESH_ENTRY FrGiftAlarmDlg::_MsgEntries[] = {
	// giftalarmdlg.cpp:23
	{ "giftlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftAlarmDlg::OnGiftListInit },
	{ "giftlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftAlarmDlg::OnGiftListOwnerDraw },
	{ "infomsg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGiftAlarmDlg::OnInfoMsgInit },

	// giftalarmdlg.cpp:27
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// giftalarmdlg.cpp:32
FrGiftAlarmDlg::FrGiftAlarmDlg(void) {
	m_pGiftList = 0;
	m_curGift = 0;
	m_giftCount = 0;
	m_blinkTime = 0.0f;

	WSingleton<CSharedDoc>::Instance()->m_tutorialFinish = 0;
	m_pArrivalMailImage = g_pFresh->RegisterBitmap("rmr_Arrival_mail");
	m_pOpenMailImage = g_pFresh->RegisterBitmap("rmr_Open_mail");
}

// giftalarmdlg.cpp:44
FrGiftAlarmDlg::~FrGiftAlarmDlg(void) {
	for (FrListBox::ITEM_LIST::iterator it = m_pGiftList->m_itemList.begin();
			it != m_pGiftList->m_itemList.end(); ++it) {
		FrListItem* pItem = *it;
		sGiftAlarmItem* pData = (sGiftAlarmItem*)pItem->pData;

		if (pData) {

			delete[] pData->pLines[0];
			delete[] pData->pLines[1];
		}

		delete pData;
	}
}

// giftalarmdlg.cpp:64
void FrGiftAlarmDlg::OnInfoMsgInit(intptr_t param) {
	m_pInfoMsg = DYNAMIC_CAST(FrStatic, param);


}

// giftalarmdlg.cpp:72
void FrGiftAlarmDlg::MakeSenderMsg(const char* pSender, char* pLines[]) {
	int len = (int)strlen(pSender);
	if (len > 20) {

		int dbcsCount = 0, sbcsCount = 0;
		for (int i = len - 1; i >= 20; i--) {

			if (IsDBCSLeadByte(pSender[i])) {
				dbcsCount++;
			} else {
				sbcsCount++;
			}
		}

		if (dbcsCount % 2) {
			dbcsCount++;
		}
		int cut = len - sbcsCount - dbcsCount;

		memcpy(pLines[0], pSender, cut);
		pLines[0][cut] = 0;


		for (int j = 0; j < 20; j++) {

			if (pSender[cut] != ' ') {
				break;
			}
			cut++;
		}


		strcpy(pLines[1], pSender + cut);
	} else {


		strcpy(pLines[0], pSender);
		pLines[1][0] = 0;
	}
}

// giftalarmdlg.cpp:446
void FrGiftAlarmDlg::OnProc(const float dt) {
	m_blinkTime += dt;

	if (m_blinkTime > 1.0f) {

		data_af7531.show ^= true;

		m_blinkTime = 0; data_af7531.update = true;
	}

	if (data_af7531.show) {

		if (data_af7531.update) {

			m_pInfoMsg->SetCaption(MakeStr("\xbf\xec\xc6\xed\xc7\xd4\xc0\xbb \xc8\xae\xc0\xce\xc7\xcf\xbc\xbc\xbf\xe4   (%d/%d)", m_curGift + 1, m_giftCount));

			data_af7531.update = false;
		}
	} else {

		if (data_af7531.update) {

			m_pInfoMsg->SetCaption("");
			data_af7531.update = false;
		}
	}
}

// giftalarmdlg.cpp:485 (run-time type and factory registration of CTreasureAlarmDlg)
const WRTTI CTreasureAlarmDlg::m_RTTI("CTreasureAlarmDlg", &FrForm::m_RTTI);

IObject* CTreasureAlarmDlgMakeInstance(void) {
	return new CTreasureAlarmDlg;
}

struct __sCTreasureAlarmDlg {
	__sCTreasureAlarmDlg(void) {
		ObjectFactory().AddObjectFunctor(CTreasureAlarmDlgMakeInstance, "CTreasureAlarmDlg");
	}
};
static __sCTreasureAlarmDlg s_registerCTreasureAlarmDlg;

// giftalarmdlg.cpp:487
const sFRESH_MSGMAP* CTreasureAlarmDlg::GetMessageMap(void) const {
	return &CTreasureAlarmDlg::_MsgMap;
}

sFRESH_MSGMAP CTreasureAlarmDlg::_MsgMap = { &FrForm::_MsgMap, &CTreasureAlarmDlg::_MsgEntries[0] };

sFRESH_ENTRY CTreasureAlarmDlg::_MsgEntries[] = {
	// giftalarmdlg.cpp:489
	{ "giftlist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTreasureAlarmDlg::OnListBox_Gift_Init },
	{ "giftlist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTreasureAlarmDlg::OnListBox_Gift_OwnerDraw },
	{ "infomsg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&CTreasureAlarmDlg::OnStatic_Msg_Init },

	// giftalarmdlg.cpp:493
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// giftalarmdlg.cpp:498
CTreasureAlarmDlg::CTreasureAlarmDlg(void) {
	m_pGiftList = 0;
	m_pMsg = 0;
}

// giftalarmdlg.cpp:504
CTreasureAlarmDlg::~CTreasureAlarmDlg(void) {


	for (FrListBox::ITEM_LIST::iterator it = m_pGiftList->m_itemList.begin();
			it != m_pGiftList->m_itemList.end(); ++it) {
		FrListItem* pItem = *it;


		delete (sTreasureAlarmItem*)pItem->pData;
	}
}

// giftalarmdlg.cpp:676
void CTreasureAlarmDlg::OnStatic_Msg_Init(intptr_t param) {
	m_pMsg = DYNAMIC_CAST(FrStatic, param);


}

// giftalarmdlg.cpp:683
void CTreasureAlarmDlg::OnProc(const float dt) {
	m_blinkTime += dt;

	if (m_blinkTime > 1.0f) {

		data_af7533.show ^= true;
		m_blinkTime = 0;
		data_af7533.update = true;
	}

	if (data_af7533.show) {

		if (data_af7533.update) {

			m_pMsg->SetCaption("\xb8\xb6\xc0\xcc\xb7\xeb\xbf\xa1\xbc\xad \xc8\xae\xc0\xce\xc7\xcf\xbc\xbc\xbf\xe4");

			data_af7533.update = false;
		}
	} else {

		if (data_af7533.update) {

			m_pMsg->SetCaption("");
			data_af7533.update = false;
		}
	}
}
