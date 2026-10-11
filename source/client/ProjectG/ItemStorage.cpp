// Original: d:\build\custom\temp\code\client\projectg\itemstorage.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "ItemStorage.h"
#include "globalnetworkdefine.h"
#include "IndexControl.h"
#include "Packet.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "frlistbox.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "InputManager.h"
#include "SharedDoc.h"
#include "wresrcmng.h"
#include "wfont.h"
#include "wview.h"
#include "wutil.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "Localize.h"
#include "UccManager.h"
#include "UccLibrary.h"
#include "TaskManager.h"
#include "binstr.h"

template <> CItemStorage* WSingleton<CItemStorage>::m_pInstance = NULL;

// itemstorage.h:202
void FrItemCounter::SetStoredItemInfo(const sStoredItemInfo& info) {
	m_storedItemInfo.slot = info.slot;
	m_storedItemInfo.item = info.item;
}

// itemstorage.h:206
const sStoredItemInfo& FrItemCounter::GetStoredItemInfo(void) const {
	return m_storedItemInfo;
}

// itemstorage.h:211
void FrItemCounter::SetType(unsigned long type) {
	m_type = type;

	switch (type) {
	case 0:
		SetCaption("¾ÆÀÌÅÛ º¸°üÇÏ±â");
		SetDesc("º¸°üÇÒ ¾ÆÀÌÅÛÀÇ ¼ö·®À» ÀÔ·ÂÇÏ¼¼¿ä.");
		break;
	case 1:
		SetCaption("¾ÆÀÌÅÛ °¡Á®¿À±â");
		SetDesc("°¡Á®¿Ã ¾ÆÀÌÅÛÀÇ ¼ö·®À» ÀÔ·ÂÇÏ¼¼¿ä.");
		break;
	}
}

// itemstorage.h:232
unsigned long FrItemCounter::GetType(void) const {
	return m_type;
}

// itemstorage.h:275
void FrItemConfirm::SetStoredItemInfo(const sStoredItemInfo& info) {
	m_storedItemInfo.slot = info.slot;
	m_storedItemInfo.item = info.item;
}

// itemstorage.h:279
const sStoredItemInfo& FrItemConfirm::GetStoredItemInfo(void) const {
	return m_storedItemInfo;
}

// itemstorage.h:283
void FrItemConfirm::SetType(unsigned long type) {
	m_type = type;

	switch (type) {
	case 0:
		SetCaption("¾ÆÀÌÅÛ º¸°üÇÏ±â");
		SetDesc("¾ÆÀÌÅÛÀ» Ã¢°í¿¡ º¸°üÇÕ´Ï´Ù.");
		break;
	case 1:
		SetCaption("¾ÆÀÌÅÛ °¡Á®¿À±â");
		SetDesc("¾ÆÀÌÅÛÀ» Ã¢°í¿¡¼­ °¡Á®¿É´Ï´Ù.");
		break;
	}
}

// itemstorage.h:304
unsigned long FrItemConfirm::GetType(void) const {
	return m_type;
}

// itemstorage.h:348
__int64 FrItemStoragePangBank::GetStoredPang(void) const {
	return m_storedPang;
}

// itemstorage.h:356
__int64 FrItemStoragePangBank::GetMyPang(void) const {
	return m_myPang;
}

// itemstorage.h:361
void FrItemStoragePangBank::SetType(unsigned long type) {
	m_type = type;

	switch (type) {
	case TYPE_DEPOSIT:
		SetCaption("\xC6\xCE \xBA\xB8\xB0\xFC\xC7\xCF\xB1\xE2");
		SetDesc("\xBA\xB8\xB0\xFC\xC7\xD2 \xC6\xCE\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xCF\xBC\xBC\xBF\xE4.");
		break;
	case TYPE_WITHDRAW:
		SetCaption("\xC6\xCE \xB0\xA1\xC1\xAE\xBF\xC0\xB1\xE2");
		SetDesc("\xB0\xA1\xC1\xAE\xBF\xC3 \xC6\xCE\xC0\xBB \xC0\xD4\xB7\xC2\xC7\xCF\xBC\xBC\xBF\xE4.");
		break;
	}
}

// itemstorage.h:382
unsigned long FrItemStoragePangBank::GetType(void) const {
	return m_type;
}

// itemstorage.h:518
__int64 CItemStorage::GetStoredPang(void) const {
	return m_storedPang;
}

// itemstorage.cpp:15
void FrItemStorageNotice::OnProc(float) {
}

// itemstorage.cpp:17
void FrItemStorageNotice::OnDraw(void) {
}

// itemstorage.cpp:23 (run-time type and factory registration of FrItemStorageNotice)
const WRTTI FrItemStorageNotice::m_RTTI("FrItemStorageNotice", &FrForm::m_RTTI);

IObject* FrItemStorageNoticeMakeInstance(void) {
	return new FrItemStorageNotice;
}

struct __sFrItemStorageNotice {
	__sFrItemStorageNotice(void) {
		ObjectFactory().AddObjectFunctor(FrItemStorageNoticeMakeInstance, "FrItemStorageNotice");
	}
};
static __sFrItemStorageNotice s_registerFrItemStorageNotice;

// itemstorage.cpp:26
const sFRESH_MSGMAP* FrItemStorageNotice::GetMessageMap(void) const {
	return &FrItemStorageNotice::_MsgMap;
}

sFRESH_MSGMAP FrItemStorageNotice::_MsgMap = { &FrForm::_MsgMap, &FrItemStorageNotice::_MsgEntries[0] };

sFRESH_ENTRY FrItemStorageNotice::_MsgEntries[] = {
	// itemstorage.cpp:28
	{ "confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageNotice::OnLBDownConfirm },

	// itemstorage.cpp:29
	{ "print_out", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageNotice::OnOwnerDrawPrintOut },

	// itemstorage.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// itemstorage.cpp:34
void FrItemStorageNotice::OnLBDownConfirm(void) {
	Close(true);
}

// itemstorage.cpp:39
void FrItemStorageNotice::OnOwnerDrawPrintOut(intptr_t id) {
	const char* lines[7] = {
		// "Hello, PangYa users!"
		"\xbe\xc8\xb3\xe7\xc7\xcf\xbc\xbc\xbf\xe4, \xc6\xce\xbe\xdf \xc0\xaf\xc0\xfa \xbf\xa9\xb7\xaf\xba\xd0!",
		"",
		// "To prevent the account theft hacks seen often lately"
		"\xc3\xd6\xb1\xd9 \xba\xf3\xb9\xf8\xc7\xcf\xb0\xd4 \xb9\xdf\xbb\xfd\xc7\xcf\xb4\xc2 \xb0\xe8\xc1\xa4\xb5\xb5\xbf\xeb \xc7\xd8\xc5\xb7 \xbb\xe7\xb7\xca\xb8\xa6 \xb9\xe6\xc1\xf6\xc7\xcf\xbf\xa9",
		// "and let users play PangYa more safely,"
		"\xc0\xaf\xc0\xfa \xbf\xa9\xb7\xaf\xba\xd0\xb2\xb2\xbc\xad \xb4\xf5\xbf\xed \xbe\xc8\xbd\xc9\xc7\xcf\xb0\xed \xc6\xce\xbe\xdf\xb8\xa6 \xc0\xcc\xbf\xeb\xc7\xcf\xbd\xc7 \xbc\xf6 \xc0\xd6\xb5\xb5\xb7\xcf",
		"",
		// "we opened the 'Dolfini's secret safe' system!"
		"'\xb5\xb9\xc7\xc7\xb4\xcf\xc0\xc7 \xba\xf1\xb9\xd0\xb1\xdd\xb0\xed' \xbd\xc3\xbd\xba\xc5\xdb\xc0\xbb \xbf\xc0\xc7\xc2\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9!",
		// "To use Dolfini's secret safe safely,"
		"\xb5\xb9\xc7\xc7\xb4\xcf\xc0\xc7 \xba\xf1\xb9\xd0\xb1\xdd\xb0\xed\xb8\xa6 \xbe\xc8\xc0\xfc\xc7\xcf\xb0\xd4 \xc0\xcc\xbf\xeb\xc7\xcf\xbd\xc7 \xbc\xf6 \xc0\xd6\xb5\xb5\xb7\xcf,"
	};

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	WPoint pt(GetRect().x, GetRect().y);

	if (pGDI) {
		float x = pt.x + 10.0f;

		for (int i = 0; i < 7; i++) {
			pGDI->Print(WPoint(x, (float)i * 15.0f + pt.y + 45.0f), 0, lines[i]);
		}
	}

	// "on first use"
	pGDI->Print(WPoint(pt.x + 10.0f, pt.y + 150.0f), 0, "\xc3\xd6\xc3\xca \xbb\xe7\xbf\xeb\xbd\xc3");
	pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
	pGDI->SetTextStyle(1);
	// "set a 4-digit number code"
	pGDI->Print(WPoint(pt.x + 80.0f, pt.y + 150.0f), 0, "4\xc0\xda\xb8\xae\xc0\xc7 \xbc\xfd\xc0\xda \xbe\xcf\xc8\xa3\xb8\xa6 \xbc\xb3\xc1\xa4");
	pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
	pGDI->SetTextStyle(0);
	// "you must"
	pGDI->Print(WPoint(pt.x + 220.0f, pt.y + 150.0f), 0, "\xc7\xd8 \xc1\xd6\xbc\xc5\xbe\xdf \xc7\xd5\xb4\xcf\xb4\xd9.");
}

// itemstorage.cpp:94
FrItemStorageAccessPanel::FrItemStorageAccessPanel(void) {
	m_formType = (eFormType)0;
	m_inputStep = 1;

	memset(m_password, 0, sizeof(m_password));
	memset(m_newPassword, 0, sizeof(m_newPassword));
	memset(m_confirmPassword, 0, sizeof(m_confirmPassword));

	memset(m_numberSlot, 0, sizeof(m_numberSlot));

	m_pPrintOut = 0;

	memset(m_pNumberSlotBn, 0, sizeof(m_pNumberSlotBn));

	m_pConfirmBn = 0;
	m_pCancelBn = 0;

	// Shuffle the digits of the number buttons.
	srand(rand());

	int i;
	for (i = 0; i < 10; i++) {
		m_numberSlot[i] = (unsigned char)i;
	}

	for (i = 0; i < 10; i++) {
		int other = rand() % 10;

		unsigned char digit = m_numberSlot[i];
		m_numberSlot[i] = m_numberSlot[other];
		m_numberSlot[other] = digit;
	}
}

// itemstorage.cpp:132
FrItemStorageAccessPanel::~FrItemStorageAccessPanel(void) {
}

// itemstorage.cpp:311
bool FrItemStorageAccessPanel::DiffPassword(void) {
	bool ret = false;

	if (strcmp(m_newPassword, m_confirmPassword) == 0) {
		ret = true;
	}

	return ret;
}

// itemstorage.cpp:324
void FrItemStorageAccessPanel::SetButtonImage(FrButton* pButton, unsigned char number) {
	if (pButton) {
		char normalImage[0x80] = { 0 };
		char overImage[0x80] = { 0 };

		unsigned char digit = m_numberSlot[number];
		sprintf(normalImage, "%d_n", digit);
		sprintf(overImage, "%d_o", digit);

		pButton->SetStyle(FrButton::BT_3TYPES);
		pButton->SetButtonImg(normalImage, FrButton::NORMAL);
		pButton->SetButtonImg(overImage, FrButton::OVER);
		pButton->SetButtonImg(overImage, FrButton::PRESSED);
	}
}

// itemstorage.cpp:396 (run-time type and factory registration of FrItemStorageAccessPanel)
const WRTTI FrItemStorageAccessPanel::m_RTTI("FrItemStorageAccessPanel", &FrForm::m_RTTI);

IObject* FrItemStorageAccessPanelMakeInstance(void) {
	return new FrItemStorageAccessPanel;
}

struct __sFrItemStorageAccessPanel {
	__sFrItemStorageAccessPanel(void) {
		ObjectFactory().AddObjectFunctor(FrItemStorageAccessPanelMakeInstance, "FrItemStorageAccessPanel");
	}
};
static __sFrItemStorageAccessPanel s_registerFrItemStorageAccessPanel;

// itemstorage.cpp:345
void FrItemStorageAccessPanel::SendPasswordToServer(void) {
	switch (m_formType) {
	case 0: {
		WSendPacket packet((enumClientPacket)0xD2);
		packet.EncodeStr(m_newPassword);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 1: {
		WSendPacket packet((enumClientPacket)0xCE);
		packet.EncodeStr(m_newPassword);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 2: {
		WSendPacket packet((enumClientPacket)0xD3);
		packet.EncodeStr(m_password);
		packet.EncodeStr(m_newPassword);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 3: {
		WSendPacket packet((enumClientPacket)0xD4);
		packet.Encode1(1);
		packet.EncodeStr(m_newPassword);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	case 4: {
		WSendPacket packet((enumClientPacket)0xD4);
		packet.Encode1(0);
		packet.EncodeStr(m_newPassword);
		packet.Send(SEND_TO_DEFAULT);
		break;
	}
	}
}

// itemstorage.cpp:399
const sFRESH_MSGMAP* FrItemStorageAccessPanel::GetMessageMap(void) const {
	return &FrItemStorageAccessPanel::_MsgMap;
}

sFRESH_MSGMAP FrItemStorageAccessPanel::_MsgMap = { &FrForm::_MsgMap, &FrItemStorageAccessPanel::_MsgEntries[0] };

sFRESH_ENTRY FrItemStorageAccessPanel::_MsgEntries[] = {
	// itemstorage.cpp:401
	{ "print_out", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitPrintOut },

	// itemstorage.cpp:402
	{ "print_out", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnOwnerDrawPrintOut },

	// itemstorage.cpp:404
	{ "btn_slot_0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot0 },

	// itemstorage.cpp:405
	{ "btn_slot_1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot1 },

	// itemstorage.cpp:406
	{ "btn_slot_2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot2 },

	// itemstorage.cpp:407
	{ "btn_slot_3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot3 },

	// itemstorage.cpp:408
	{ "btn_slot_4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot4 },

	// itemstorage.cpp:409
	{ "btn_slot_5", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot5 },

	// itemstorage.cpp:410
	{ "btn_slot_6", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot6 },

	// itemstorage.cpp:411
	{ "btn_slot_7", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot7 },

	// itemstorage.cpp:412
	{ "btn_slot_8", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot8 },

	// itemstorage.cpp:413
	{ "btn_slot_9", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitNumberSlot9 },

	// itemstorage.cpp:415
	{ "btn_slot_0", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot0 },

	// itemstorage.cpp:416
	{ "btn_slot_1", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot1 },

	// itemstorage.cpp:417
	{ "btn_slot_2", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot2 },

	// itemstorage.cpp:418
	{ "btn_slot_3", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot3 },

	// itemstorage.cpp:419
	{ "btn_slot_4", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot4 },

	// itemstorage.cpp:420
	{ "btn_slot_5", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot5 },

	// itemstorage.cpp:421
	{ "btn_slot_6", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot6 },

	// itemstorage.cpp:422
	{ "btn_slot_7", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot7 },

	// itemstorage.cpp:423
	{ "btn_slot_8", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot8 },

	// itemstorage.cpp:424
	{ "btn_slot_9", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownNumberSlot9 },

	// itemstorage.cpp:426
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitConfirm },

	// itemstorage.cpp:427
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownConfirm },

	// itemstorage.cpp:428
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStorageAccessPanel::OnInitCancel },

	// itemstorage.cpp:429
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStorageAccessPanel::OnLBDownCancel },

	// itemstorage.cpp:431
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// itemstorage.cpp:436
void FrItemStorageAccessPanel::OnInitPrintOut(intptr_t id) {
	TBindControl(m_pPrintOut, id);
}

// itemstorage.cpp:607
void FrItemStorageAccessPanel::OnInitNumberSlot0(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[0], id)) {
		m_pNumberSlotBn[0]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[0], 0);
	}
}

// itemstorage.cpp:616
void FrItemStorageAccessPanel::OnInitNumberSlot1(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[1], id)) {
		m_pNumberSlotBn[1]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[1], 1);
	}
}

// itemstorage.cpp:625
void FrItemStorageAccessPanel::OnInitNumberSlot2(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[2], id)) {
		m_pNumberSlotBn[2]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[2], 2);
	}
}

// itemstorage.cpp:634
void FrItemStorageAccessPanel::OnInitNumberSlot3(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[3], id)) {
		m_pNumberSlotBn[3]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[3], 3);
	}
}

// itemstorage.cpp:643
void FrItemStorageAccessPanel::OnInitNumberSlot4(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[4], id)) {
		m_pNumberSlotBn[4]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[4], 4);
	}
}

// itemstorage.cpp:652
void FrItemStorageAccessPanel::OnInitNumberSlot5(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[5], id)) {
		m_pNumberSlotBn[5]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[5], 5);
	}
}

// itemstorage.cpp:661
void FrItemStorageAccessPanel::OnInitNumberSlot6(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[6], id)) {
		m_pNumberSlotBn[6]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[6], 6);
	}
}

// itemstorage.cpp:670
void FrItemStorageAccessPanel::OnInitNumberSlot7(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[7], id)) {
		m_pNumberSlotBn[7]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[7], 7);
	}
}

// itemstorage.cpp:679
void FrItemStorageAccessPanel::OnInitNumberSlot8(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[8], id)) {
		m_pNumberSlotBn[8]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[8], 8);
	}
}

// itemstorage.cpp:688
void FrItemStorageAccessPanel::OnInitNumberSlot9(intptr_t id) {
	if (TBindControl(m_pNumberSlotBn[9], id)) {
		m_pNumberSlotBn[9]->SetPushDelay(0.1f);
		SetButtonImage(m_pNumberSlotBn[9], 9);
	}
}

// itemstorage.cpp:697
void FrItemStorageAccessPanel::OnLBDownNumberSlot0(void) { InputPassword(0); }
// itemstorage.cpp:698
void FrItemStorageAccessPanel::OnLBDownNumberSlot1(void) { InputPassword(1); }
// itemstorage.cpp:699
void FrItemStorageAccessPanel::OnLBDownNumberSlot2(void) { InputPassword(2); }
// itemstorage.cpp:700
void FrItemStorageAccessPanel::OnLBDownNumberSlot3(void) { InputPassword(3); }
// itemstorage.cpp:701
void FrItemStorageAccessPanel::OnLBDownNumberSlot4(void) { InputPassword(4); }
// itemstorage.cpp:702
void FrItemStorageAccessPanel::OnLBDownNumberSlot5(void) { InputPassword(5); }
// itemstorage.cpp:703
void FrItemStorageAccessPanel::OnLBDownNumberSlot6(void) { InputPassword(6); }
// itemstorage.cpp:704
void FrItemStorageAccessPanel::OnLBDownNumberSlot7(void) { InputPassword(7); }
// itemstorage.cpp:705
void FrItemStorageAccessPanel::OnLBDownNumberSlot8(void) { InputPassword(8); }
// itemstorage.cpp:706
void FrItemStorageAccessPanel::OnLBDownNumberSlot9(void) { InputPassword(9); }

// itemstorage.cpp:711
void FrItemStorageAccessPanel::OnInitConfirm(intptr_t id) {
	if (TBindControl(m_pConfirmBn, id)) {
		m_pConfirmBn->SetPushDelay(0.1f);
	}
}

// itemstorage.cpp:882
void FrItemStorageAccessPanel::OnInitCancel(intptr_t id) {
	if (TBindControl(m_pCancelBn, id)) {
		m_pCancelBn->SetPushDelay(0.1f);
	}
}

// itemstorage.cpp:890
void FrItemStorageAccessPanel::OnLBDownCancel(void) {
	switch (m_formType) {
	case 0:
		switch (m_inputStep) {
		case 1:
			if (strlen(m_newPassword) != 0) {
				memset(m_newPassword, 0, sizeof(m_newPassword));
			} else {
				Close(FrCANCEL, true);
			}
			break;
		case 2:
			if (strlen(m_confirmPassword) != 0) {
				memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
			} else {
				memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
				m_inputStep = 1;
			}
			break;
		case 3:
			memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
			m_inputStep = 2;
			break;
		}
		break;
	case 1:
	case 3:
	case 4:
		m_inputStep = 1;
		if (strlen(m_newPassword) != 0) {
			memset(m_newPassword, 0, sizeof(m_newPassword));
			memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
		} else {
			Close(FrCANCEL, true);
		}
		break;
	case 2:
		switch (m_inputStep) {
		case 0:
			if (strlen(m_password) != 0) {
				memset(m_password, 0, sizeof(m_password));
			} else {
				Close(FrCANCEL, true);
			}
			break;
		case 1:
			if (strlen(m_newPassword) == 0) {
				memset(m_newPassword, 0, sizeof(m_newPassword));
				m_inputStep = 0;
			} else {
				memset(m_newPassword, 0, sizeof(m_newPassword));
			}
			break;
		case 2:
			if (strlen(m_confirmPassword) != 0) {
				memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
			} else {
				memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
				m_inputStep = 1;
			}
			break;
		case 3:
			memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
			m_inputStep = 2;
			break;
		}
		break;
	}
}

// itemstorage.cpp:1015
FrItemCounter::FrItemCounter(void) {
	m_pDrawArea = 0;
	m_pInput = 0;
	m_pConfirmBn = 0;
	m_pCancelBn = 0;
	memset(&m_storedItemInfo, 0, sizeof(m_storedItemInfo));
}

// itemstorage.cpp:1024
FrItemCounter::~FrItemCounter(void) {
}

// itemstorage.cpp:1030 (run-time type and factory registration of FrItemCounter)
const WRTTI FrItemCounter::m_RTTI("FrItemCounter", &FrForm::m_RTTI);

IObject* FrItemCounterMakeInstance(void) {
	return new FrItemCounter;
}

struct __sFrItemCounter {
	__sFrItemCounter(void) {
		ObjectFactory().AddObjectFunctor(FrItemCounterMakeInstance, "FrItemCounter");
	}
};
static __sFrItemCounter s_registerFrItemCounter;

// itemstorage.cpp:1033
const sFRESH_MSGMAP* FrItemCounter::GetMessageMap(void) const {
	return &FrItemCounter::_MsgMap;
}

sFRESH_MSGMAP FrItemCounter::_MsgMap = { &FrForm::_MsgMap, &FrItemCounter::_MsgEntries[0] };

sFRESH_ENTRY FrItemCounter::_MsgEntries[] = {
	// itemstorage.cpp:1035
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemCounter::OnInitDrawArea },

	// itemstorage.cpp:1036
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemCounter::OnOwnerDrawDrawArea },

	// itemstorage.cpp:1037
	{ "edit_count", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemCounter::OnInitInput },

	// itemstorage.cpp:1038
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemCounter::OnInitConfirm },

	// itemstorage.cpp:1039
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemCounter::OnLBDownConfirm },

	// itemstorage.cpp:1040
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemCounter::OnInitCancel },

	// itemstorage.cpp:1041
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemCounter::OnLBDownCancel },

	// itemstorage.cpp:1043
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// itemstorage.cpp:1046
void FrItemCounter::OnInitDrawArea(intptr_t param) {
	m_pDrawArea = DYNAMIC_CAST(FrArea, param);
}

// itemstorage.cpp:1050
void FrItemCounter::OnOwnerDrawDrawArea(intptr_t id) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		WPoint pt(GetRect().x, GetRect().y);

		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(m_storedItemInfo.item.typeId);

		if (pCommon) {
			const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);
			if (pIcon) {
				float scale = 100.0f;   // percent
				pGDI->DrawTexture(pIcon, WRect(pt.x + 35.0f, pt.y + 40.0f, (float)pIcon->Width() * 0.01f * scale, (float)pIcon->Height() * 0.01f * scale), 0xFFFFFFFF, 0);
			}

			pGDI->Print(WPoint(pt.x + 100.0f, pt.y + 55.0f), 0, pCommon->m_name);
		}

		// "enter the quantity"
		pGDI->Print(WPoint(pt.x + 15.0f, pt.y + 135.0f), 0, "\xbc\xf6\xb7\xae\xc0\xd4\xb7\xc2");
	}
}

// itemstorage.cpp:1095
void FrItemCounter::OnInitInput(intptr_t param) {
	m_pInput = DYNAMIC_CAST(FrEdit, param);
}

// itemstorage.cpp:1101
void FrItemCounter::OnInitConfirm(intptr_t param) {
	m_pConfirmBn = DYNAMIC_CAST(FrButton, param);

	if (m_pConfirmBn) {
		m_pConfirmBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1111
void FrItemCounter::OnLBDownConfirm(void) {
	char text[0x20] = { 0 };

	if (m_pInput) {
		m_pInput->GetWindowTextA(text, sizeof(text));
		if (strlen(text) >= 1) {
			unsigned long count = atoi(text);

			if (count > 0 && count <= m_storedItemInfo.item.count) {
				m_storedItemInfo.item.count = count;
				Close(FrOK, true);
			}
		}
	}
}

// itemstorage.cpp:1142
void FrItemCounter::OnInitCancel(intptr_t param) {
	m_pCancelBn = DYNAMIC_CAST(FrButton, param);

	if (m_pCancelBn) {
		m_pCancelBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1152
void FrItemCounter::OnLBDownCancel(void) {
	Close(FrCANCEL, true);
}

// itemstorage.cpp:1163
FrItemStoragePangBank::FrItemStoragePangBank(void) {
	m_pDrawArea = 0;
	m_pInput = 0;
	m_pConfirmBn = 0;
	m_pCancelBn = 0;
	m_storedPang = 0;
	m_myPang = 0;
}

// itemstorage.cpp:1173
FrItemStoragePangBank::~FrItemStoragePangBank(void) {
}

// itemstorage.cpp:1179 (run-time type and factory registration of FrItemStoragePangBank)
const WRTTI FrItemStoragePangBank::m_RTTI("FrItemStoragePangBank", &FrForm::m_RTTI);

IObject* FrItemStoragePangBankMakeInstance(void) {
	return new FrItemStoragePangBank;
}

struct __sFrItemStoragePangBank {
	__sFrItemStoragePangBank(void) {
		ObjectFactory().AddObjectFunctor(FrItemStoragePangBankMakeInstance, "FrItemStoragePangBank");
	}
};
static __sFrItemStoragePangBank s_registerFrItemStoragePangBank;

// itemstorage.cpp:1182
const sFRESH_MSGMAP* FrItemStoragePangBank::GetMessageMap(void) const {
	return &FrItemStoragePangBank::_MsgMap;
}

sFRESH_MSGMAP FrItemStoragePangBank::_MsgMap = { &FrForm::_MsgMap, &FrItemStoragePangBank::_MsgEntries[0] };

sFRESH_ENTRY FrItemStoragePangBank::_MsgEntries[] = {
	// itemstorage.cpp:1184
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStoragePangBank::OnInitDrawArea },

	// itemstorage.cpp:1185
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStoragePangBank::OnOwnerDrawDrawArea },

	// itemstorage.cpp:1186
	{ "edit_count", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStoragePangBank::OnInitInput },

	// itemstorage.cpp:1187
	{ "edit_count", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrItemStoragePangBank::OnInputEnterKey },

	// itemstorage.cpp:1188
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStoragePangBank::OnInitConfirm },

	// itemstorage.cpp:1189
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStoragePangBank::OnLBDownConfirm },

	// itemstorage.cpp:1190
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemStoragePangBank::OnInitCancel },

	// itemstorage.cpp:1191
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemStoragePangBank::OnLBDownCancel },

	// itemstorage.cpp:1193
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// itemstorage.cpp:1196
void FrItemStoragePangBank::OnInitDrawArea(intptr_t param) {
	m_pDrawArea = DYNAMIC_CAST(FrArea, param);
}

// itemstorage.cpp:1321
void FrItemStoragePangBank::OnInitInput(intptr_t param) {
	m_pInput = DYNAMIC_CAST(FrEdit, param);
}

// itemstorage.cpp:1327
bool FrItemStoragePangBank::OnInputEnterKey(int param) {
	OnLBDownConfirm();

	return true;
}

// itemstorage.cpp:1334
void FrItemStoragePangBank::OnInitConfirm(intptr_t param) {
	m_pConfirmBn = DYNAMIC_CAST(FrButton, param);

	if (m_pConfirmBn) {
		m_pConfirmBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1344
void FrItemStoragePangBank::OnLBDownConfirm(void) {
	char text[32] = "\0";
	int pang = 0;
	bool bValid = false;

	if (m_pInput) {
		m_pInput->GetWindowTextA(text, 32);
		if (strlen(text) >= 1) {
			pang = atoi(text);

			if (m_type == TYPE_WITHDRAW) {
				if (pang > 0 && m_storedPang >= pang) {
					bValid = true;
				}
			} else {
				if (pang > 0 && m_myPang >= pang) {
					bValid = true;
				}
			}

			if (bValid) {
				m_storedPang = pang;
				Close(FrOK, true);
				return;
			}
		}
	}

	__int64 limit = m_type == TYPE_WITHDRAW ? m_storedPang : m_myPang;
	if (pang > limit) {
		// "Not enough pang."
		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xb1\xdd\xbe\xd7\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
	} else if (pang <= 0) {
		// "You entered an invalid amount."
		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xc0\xdf\xb8\xf8\xb5\xc8 \xb1\xdd\xbe\xd7\xc0\xbb \xc0\xd4\xb7\xc2\xc7\xcf\xbc\xcc\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
	}

	m_pInput->ClearLine();
}

// itemstorage.cpp:1406
void FrItemStoragePangBank::OnInitCancel(intptr_t param) {
	m_pCancelBn = DYNAMIC_CAST(FrButton, param);

	if (m_pCancelBn) {
		m_pCancelBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1416
void FrItemStoragePangBank::OnLBDownCancel(void) {
	Close(FrCANCEL, true);
}

// itemstorage.cpp:1428
FrItemConfirm::FrItemConfirm(void) {
	m_pDrawArea = 0;
	m_pConfirmBn = 0;
	m_pCancelBn = 0;
	memset(&m_storedItemInfo, 0, sizeof(m_storedItemInfo));
}

// itemstorage.cpp:1436
FrItemConfirm::~FrItemConfirm(void) {
}

// itemstorage.cpp:1442 (run-time type and factory registration of FrItemConfirm)
const WRTTI FrItemConfirm::m_RTTI("FrItemConfirm", &FrForm::m_RTTI);

IObject* FrItemConfirmMakeInstance(void) {
	return new FrItemConfirm;
}

struct __sFrItemConfirm {
	__sFrItemConfirm(void) {
		ObjectFactory().AddObjectFunctor(FrItemConfirmMakeInstance, "FrItemConfirm");
	}
};
static __sFrItemConfirm s_registerFrItemConfirm;

// itemstorage.cpp:1445
const sFRESH_MSGMAP* FrItemConfirm::GetMessageMap(void) const {
	return &FrItemConfirm::_MsgMap;
}

sFRESH_MSGMAP FrItemConfirm::_MsgMap = { &FrForm::_MsgMap, &FrItemConfirm::_MsgEntries[0] };

sFRESH_ENTRY FrItemConfirm::_MsgEntries[] = {
	// itemstorage.cpp:1447
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemConfirm::OnInitDrawArea },

	// itemstorage.cpp:1448
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemConfirm::OnOwnerDrawDrawArea },

	// itemstorage.cpp:1449
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemConfirm::OnInitConfirm },

	// itemstorage.cpp:1450
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemConfirm::OnLBDownConfirm },

	// itemstorage.cpp:1451
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemConfirm::OnInitCancel },

	// itemstorage.cpp:1452
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrItemConfirm::OnLBDownCancel },

	// itemstorage.cpp:1454
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// itemstorage.cpp:1457
void FrItemConfirm::OnInitDrawArea(intptr_t param) {
	m_pDrawArea = DYNAMIC_CAST(FrArea, param);
}

// itemstorage.cpp:1463
void FrItemConfirm::OnOwnerDrawDrawArea(intptr_t id) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		WPoint pt(GetRect().x, GetRect().y);

		// The parentheses test the bool of the whole condition, not the type id.
		if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(m_storedItemInfo.item.typeId && (m_storedItemInfo.item.uccStatus & 1))) {
			const Bitmap* pUccIcon = GetUccItemIcon(m_storedItemInfo.item.typeId, m_storedItemInfo.item.uccIdx);

			if (pUccIcon) {
				pGDI->DrawTexture(pUccIcon, WRect(pt.x + 35.0f, pt.y + 40.0f, pUccIcon->Width() * 0.01f * 100.0f, pUccIcon->Height() * 0.01f * 100.0f), 0xFFFFFFFF, 0);
			}

			pGDI->Print(WPoint(pt.x + 110.0f, pt.y + 55.0f), 0, GetUccItemName(m_storedItemInfo.item.guid));
			return;
		}

		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(m_storedItemInfo.item.typeId);

		if (pCommon) {
			const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);
			if (pIcon) {
				pGDI->DrawTexture(pIcon, WRect(pt.x + 35.0f, pt.y + 40.0f, pIcon->Width() * 0.01f * 100.0f, pIcon->Height() * 0.01f * 100.0f), 0xFFFFFFFF, 0);
			}

			pGDI->Print(WPoint(pt.x + 110.0f, pt.y + 55.0f), 0, pCommon->m_name);
		}
	}
}

// itemstorage.cpp:1524
void FrItemConfirm::OnInitConfirm(intptr_t param) {
	m_pConfirmBn = DYNAMIC_CAST(FrButton, param);

	if (m_pConfirmBn) {
		m_pConfirmBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1534
void FrItemConfirm::OnLBDownConfirm(void) {
	if (m_storedItemInfo.item.count > 0) {
		Close(FrOK, true);
	}
}

// itemstorage.cpp:1542
void FrItemConfirm::OnInitCancel(intptr_t param) {
	m_pCancelBn = DYNAMIC_CAST(FrButton, param);

	if (m_pCancelBn) {
		m_pCancelBn->SetPushDelay(0.5f);
	}
}

// itemstorage.cpp:1552
void FrItemConfirm::OnLBDownCancel(void) {
	Close(FrCANCEL, true);
}

// The title font file of the storage's numbers.
extern "C" char* data_a8df20 = "[font_wind.jpg";

// itemstorage.cpp:1564
CItemStorage::CItemStorage(void) {
	m_visible = false;
	m_lock = false;

	m_frontUI.clear();
	m_backGroundUI.clear();
	m_backGroundVisibleUI.clear();

	m_pTitleFont = 0;
	m_storedPang = 0;
	m_totalPage = 0;
	m_curPage = 0;
	m_indexControl = 0;

	TSafeNew<CIndexControl>(m_indexControl, 1);
	m_storedItemInfo.clear();

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	m_pPangBank = 0;
	m_pItemCounter = 0;
	m_pItemConfirm = 0;

	info.filename = &data_a8df20;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:,";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	memset(&m_dragDrop, 0, sizeof(m_dragDrop));
}

// itemstorage.cpp:1604
CItemStorage::~CItemStorage(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
	if (m_indexControl) { delete[] m_indexControl; m_indexControl = 0; }
}

// itemstorage.cpp:1610
void CItemStorage::Lock(void) {
	m_lock = true;

	std::list<FrWnd*>::iterator it;
	for (it = m_frontUI.begin(); it != m_frontUI.end(); ++it) {
		if (*it) {
			(*it)->Enable(false);
		}
	}
}

// itemstorage.cpp:1624
void CItemStorage::UnLock(void) {
	m_lock = false;

	std::list<FrWnd*>::iterator it;
	for (it = m_frontUI.begin(); it != m_frontUI.end(); ++it) {
		if (*it) {
			(*it)->Enable(true);
		}
	}
}

// itemstorage.cpp:1638
void CItemStorage::AddFrontUI(FrWnd* pWnd) {
	if (pWnd) {
		pWnd->SetVisible(false);
		m_frontUI.push_back(pWnd);
	}
}

// itemstorage.cpp:1647
void CItemStorage::AddBackGroundUI(FrWnd* pWnd, bool bVisibleGroup) {
	if (pWnd) {
		if (!bVisibleGroup) {
			m_backGroundUI.push_back(pWnd);
		} else {
			m_backGroundVisibleUI.push_back(pWnd);
		}
	}
}

// itemstorage.cpp:1662
void CItemStorage::SetVisibleFrontUI(bool bVisible) {
	std::list<FrWnd*>::iterator it;
	for (it = m_frontUI.begin(); it != m_frontUI.end(); ++it) {
		m_visible = bVisible;

		FrWnd* pWnd = *it;
		if (pWnd) {
			pWnd->SetVisible(bVisible);
		}
	}
}

// itemstorage.cpp:1677
void CItemStorage::SetVisibleBackGroundUI(bool bVisible) {
	std::list<FrWnd*>::iterator it;
	for (it = m_backGroundVisibleUI.begin(); it != m_backGroundVisibleUI.end(); ++it) {
		FrWnd* pWnd = *it;
		if (pWnd) {
			pWnd->SetVisible(bVisible);
		}
	}
}

// itemstorage.cpp:1690
void CItemStorage::SetEnableFrontUI(bool bEnable) {
	std::list<FrWnd*>::iterator it;
	for (it = m_frontUI.begin(); it != m_frontUI.end(); ++it) {
		FrWnd* pWnd = *it;
		if (pWnd) {
			pWnd->Enable(bEnable);
		}
	}
}

// itemstorage.cpp:1703
void CItemStorage::SetEnableBackGroundUI(bool bEnable) {
	std::list<FrWnd*>::iterator it;
	for (it = m_backGroundUI.begin(); it != m_backGroundUI.end(); ++it) {
		FrWnd* pWnd = *it;
		if (pWnd) {
			pWnd->Enable(bEnable);
		}
	}
}

// itemstorage.cpp:1717
// Prints text with the title font; align 1 centres it on x, 2 ends it at x.
void CItemStorage::Print(float x, float y, const char* text, float spacing, int align, float scale, unsigned long color) {
	if (!text) {
		return;
	}

	if (strlen(text) == 0) {
		return;
	}

	const char* charSet = m_pTitleFont->GetFontInfo()->pCharSet;
	float oldScale = m_pTitleFont->GetScale();

	m_pTitleFont->SetScale(scale);
	float width = 0.0f;

	switch (align) {
	case 1:
		for (const char* p = text; *p; ) {
			int ch = *p;

			int index = strchr(charSet, ch) - charSet;
			p++;

			width += m_pTitleFont->GetCharWidthA(g_view, index) + spacing * scale;
		}
		width = (width - spacing) * 0.5f;
		break;
	case 2:
		for (const char* p = text; *p; ) {
			int ch = *p;

			int index = strchr(charSet, ch) - charSet;
			p++;

			width += m_pTitleFont->GetCharWidthA(g_view, index) + spacing * scale;
		}
		width -= spacing;
		break;
	}

	x -= width;

	while (*text) {
		int ch = *text;

		int index = strchr(charSet, ch) - charSet;
		text++;

		x += m_pTitleFont->PutChar(g_view, x, y, index, 0, color) + spacing;
	}

	m_pTitleFont->SetScale(oldScale);
	m_pTitleFont->Flush(g_view);
}

// itemstorage.cpp:1796
void CItemStorage::ClearStoredItemInfo(void) {
	std::vector<sStoredItemInfo*>::iterator it;
	for (it = m_storedItemInfo.begin(); it != m_storedItemInfo.end(); ++it) {
		if (*it) {
			delete[] *it;
			*it = 0;
		}
	}

	std::vector<sItemInfo*>::iterator itItem;
	for (itItem = m_itemInfo.begin(); itItem != m_itemInfo.end(); ++itItem) {
		if (*itItem) {
			delete[] *itItem;
			*itItem = 0;
		}
	}

	std::vector<sStoredItemInfo*>().swap(m_storedItemInfo);
}

// itemstorage.cpp:1814
void CItemStorage::AddStoredItemInfo(const sStoredItemInfo& info) {
	sStoredItemInfo* pStored = 0;
	TSafeNew<sStoredItemInfo>(pStored, 1);

	if (pStored) {
		*pStored = info;
		m_storedItemInfo.push_back(pStored);

		if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pStored->item.typeId)) {
			sItemInfo* pItem = new sItemInfo;
			memset(pItem, 0, sizeof(sItemInfo));

			if (pItem) {
				pItem->m_typeId = pStored->item.typeId;
				pItem->m_serial = pStored->item.guid;
				pItem->m_uccSeq = pStored->item.uccSeq;
				pItem->m_uccStatus = pStored->item.uccStatus;
				memcpy(pItem->m_uccName, pStored->item.uccName, sizeof(pItem->m_uccName));
				memcpy(pItem->m_uccKey, pStored->item.uccIdx, sizeof(pItem->m_uccKey));

				m_itemInfo.push_back(pItem);

				sUccClothes clothes;
				clothes.m_serial = pItem->m_serial;
				clothes.m_typeId = pItem->m_typeId;
				strncpy(clothes.m_uccKey, pItem->m_uccKey, 9);
				strncpy(clothes.m_uccName, pItem->m_uccName, 0x29);
				clothes.m_downloadCount = !(pStored->item.uccStatus & 1);

				UccManager()->AddClothes(0, clothes, true, false);
			}
		}
	}
}

// itemstorage.cpp:1868
bool CItemStorage::CheckEquiped(unsigned long id) {
	bool ret = false;

	std::map<unsigned int, sCharacterInfo>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_characterMap.begin(); it != WSingleton<CSharedDoc>::Instance()->m_characterMap.end(); ++it) {
		for (int i = 0; i < 24; i++) {
			if (it->second.partIds[i] != 0) {
				if (it->second.partIds[i] == id) {
					return true;
				}
			}
		}
	}

	return ret;
}

// itemstorage.cpp:1901
// Fills `stored` from an inventory item; only some item categories (the
// type id's top 6 bits) can be stored.
bool CItemStorage::CheckStoredItemInfo(sStoredItemInfo& stored, const sItemInfo& info) {
	bool ret = false;

	switch (info.m_typeId >> 26) {
	case 2:
	case 4:
	case 5:
	case 6:
	case 28:
		break;
	default:
		return false;
	}

	memset(&stored, 0, sizeof(stored));

	stored.item.typeId = info.m_typeId;
	stored.item.guid = info.m_serial;
	strcpy(stored.item.uccCopierNick, info.m_reserved5E);   // sItemInfo 0x5E holds the UCC copier's nick
	strcpy(stored.item.uccIdx, info.m_uccKey);

	switch (stored.item.typeId >> 26) {
	case 28:
		if (info.m_count > 1) {
			ret = false;
			break;
		}
		stored.item.count = 1;
		ret = true;
		break;
	case 2:
		stored.item.count = 1;
		ret = true;
		break;
	case 4:
		break;
	default:
		ret = false;
		break;
	}

	if (ret == false) {
		memset(&stored, 0, sizeof(stored));
	}

	return ret;
}

// itemstorage.cpp:2018
void CItemStorage::ReqPopStoredItemInfo(const FrListItem& item) {
	const sStoredItemInfo* pInfo = (const sStoredItemInfo*)item.pData;

	if (pInfo) {
		ReqPopStoredItemInfo(*pInfo);
	}
}

// itemstorage.cpp:2030
void CItemStorage::ReqPopStoredItemInfo(const sStoredItemInfo& info) {
	if (info.item.count > 1) {
		if (m_pItemCounter == 0) {
			m_pItemCounter = CreateForm<FrItemCounter>(g_pFresh->GetManager(), this, "item_storage_item_counter", 0);
		}

		if (m_pItemCounter) {
			FrItemCounter* pForm = m_pItemCounter;

			pForm->m_type = 1;
			pForm->SetCaption("\xbe\xc6\xc0\xcc\xc5\xdb \xb0\xa1\xc1\xae\xbf\xc0\xb1\xe2");
			pForm->SetDesc("\xb0\xa1\xc1\xae\xbf\xc3 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xbc\xf6\xb7\xae\xc0\xbb \xc0\xd4\xb7\xc2\xc7\xcf\xbc\xbc\xbf\xe4.");
			// sStoredItemInfo::operator= (globalnetworkdefine.h:2991) is
			// inline in the original; ours is declared out of line.
			sStoredItemInfo& stored = m_pItemCounter->m_storedItemInfo;
			stored.slot = info.slot;
			stored.item = info.item;
			m_pItemCounter->Open((FRESH_RESULT_CALLBACK)&CItemStorage::OnResultItemCounter, 9);
		}
	} else if (info.item.count == 1) {
		if (m_pItemConfirm == 0) {
			m_pItemConfirm = CreateForm<FrItemConfirm>(g_pFresh->GetManager(), this, "item_storage_item_confirm", 0);
		}

		if (m_pItemConfirm) {
			FrItemConfirm* pForm = m_pItemConfirm;

			pForm->m_type = 1;
			pForm->SetCaption("\xbe\xc6\xc0\xcc\xc5\xdb \xb0\xa1\xc1\xae\xbf\xc0\xb1\xe2");
			pForm->SetDesc("\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc3\xa2\xb0\xed\xbf\xa1\xbc\xad \xb0\xa1\xc1\xae\xbf\xc9\xb4\xcf\xb4\xd9.");
			sStoredItemInfo& stored = m_pItemConfirm->m_storedItemInfo;
			stored.slot = info.slot;
			stored.item = info.item;
			m_pItemConfirm->Open((FRESH_RESULT_CALLBACK)&CItemStorage::OnResultItemConfirm, 9);
		}
	}
}

// itemstorage.cpp:2070
void CItemStorage::ReqPushStoredItemInfo(const FrListItem& item) {
	const sItemInfo* pInfo = (const sItemInfo*)item.pData;

	if (pInfo) {
		ReqPushStoredItemInfo(*pInfo);
	}
}

// wtemplate.h:409-414 (not yet in src/client/ProjectG/wtemplate.h): list finder by the element's guid.
struct _FindByGuid {
	unsigned long m_guid;

	_FindByGuid(unsigned long guid) : m_guid(guid) {}
	template <class T>
	bool operator()(const T& item) const { return m_guid == item.m_serial; }
};

// itemstorage.cpp:2082
void CItemStorage::ReqPushStoredItemInfo(const sItemInfo& info) {
	sStoredItemInfo stored;

	if (CheckStoredItemInfo(stored, info) == true) {
		if (!CheckEquiped(stored.item.guid)) {
			if ((stored.item.typeId & 0xFC000000) == 0x08000000) {
				CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
				std::list<sItemInfo>::iterator it = std::find_if(pDoc->m_myPartsList.begin(), pDoc->m_myPartsList.end(), _FindByGuid(stored.item.guid));
				if (it != pDoc->m_myPartsList.end()) {
					if (it->m_stateBits.kind == 6) {
						FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
						pForm->SetMessage("\xb4\xeb\xbf\xa9 \xc0\xc7\xbb\xf3\xc0\xba \xc3\xa2\xb0\xed\xbf\xa1 \xba\xb8\xb0\xfc \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", true);
						pForm->Open((FRESH_RESULT_CALLBACK)0, 9);
						return;
					}
				}
		}

			if (stored.item.count > 1) {
				if (m_pItemCounter == 0) {
					m_pItemCounter = CreateForm<FrItemCounter>(g_pFresh->GetManager(), this, "item_storage_item_counter", 0);
				}

				if (m_pItemCounter) {
					FrItemCounter* pForm = m_pItemCounter;

					pForm->m_type = 0;
					pForm->SetCaption("\xbe\xc6\xc0\xcc\xc5\xdb \xba\xb8\xb0\xfc\xc7\xcf\xb1\xe2");
					pForm->SetDesc("\xba\xb8\xb0\xfc\xc7\xd2 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xbc\xf6\xb7\xae\xc0\xbb \xc0\xd4\xb7\xc2\xc7\xcf\xbc\xbc\xbf\xe4.");
					m_pItemCounter->SetStoredItemInfo(stored);
					m_pItemCounter->Open((FRESH_RESULT_CALLBACK)&CItemStorage::OnResultItemCounter, 9);
				}
			} else if (stored.item.count == 1) {
				if (m_pItemConfirm == 0) {
					m_pItemConfirm = CreateForm<FrItemConfirm>(g_pFresh->GetManager(), this, "item_storage_item_confirm", 0);
				}

				if (m_pItemConfirm) {
					FrItemConfirm* pForm = m_pItemConfirm;

					pForm->m_type = 0;
					pForm->SetCaption("\xbe\xc6\xc0\xcc\xc5\xdb \xba\xb8\xb0\xfc\xc7\xcf\xb1\xe2");
					pForm->SetDesc("\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc3\xa2\xb0\xed\xbf\xa1 \xba\xb8\xb0\xfc\xc7\xd5\xb4\xcf\xb4\xd9.");
					m_pItemConfirm->SetStoredItemInfo(stored);
					m_pItemConfirm->Open((FRESH_RESULT_CALLBACK)&CItemStorage::OnResultItemConfirm, 9);
				}
			} else if (stored.item.count == 0) {
				FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
				pForm->SetMessage("\xba\xb8\xb0\xfc \xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb \xc0\xd4\xb4\xcf\xb4\xd9.", true);
				pForm->Open((FRESH_RESULT_CALLBACK)0, 9);
		}
		} else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			pForm->SetMessage("\xc0\xe5\xc2\xf8\xc1\xdf\xc0\xce \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xba \xba\xb8\xb0\xfc \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", true);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 9);
		}
	} else {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		pForm->SetMessage("\xba\xb8\xb0\xfc \xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb \xc0\xd4\xb4\xcf\xb4\xd9.", true);
		pForm->Open((FRESH_RESULT_CALLBACK)0, 9);
	}
}

// itemstorage.cpp:2165
void CItemStorage::DrawBackgroundImage(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		const Bitmap* pBg1 = g_pFresh->GetBitmap("bg_01");
		const Bitmap* pBg2 = g_pFresh->GetBitmap("bg_02");
		const Bitmap* pBg3 = g_pFresh->GetBitmap("bg_03");
		const Bitmap* pBg4 = g_pFresh->GetBitmap("bg_04");

		if (pBg1 && pBg2 && pBg3 && pBg4) {
			pGDI->DrawTexture(pBg1, WRect(30.0f, 74.0f, (float)pBg1->Width(), (float)pBg1->Height()), 0xFFFFFFFF, 0);
			pGDI->DrawTexture(pBg2, WRect(pBg1->Width() + 30.0f, 74.0f, (float)pBg2->Width(), (float)pBg2->Height()), 0xFFFFFFFF, 0);
			pGDI->DrawTexture(pBg3, WRect(37.0f, pBg1->Height() + 74.0f, (float)pBg3->Width(), (float)pBg3->Height()), 0xFFFFFFFF, 0);
			pGDI->DrawTexture(pBg4, WRect(pBg3->Width() + 37.0f, pBg2->Height() + 74.0f, (float)pBg4->Width(), (float)pBg4->Height()), 0xFFFFFFFF, 0);
		}

		char price[32];
		memset(price, 0, sizeof(price));
		MakePriceStrA(price, m_storedPang);

		Print(329.0f, 131.0f, price, 2.0f, 2, 1.0f, 0xFFFFD18E);
	}
}

// itemstorage.cpp:2209
void CItemStorage::MakeStoredItemInfo(FrListBox*& pListBox) {
	if (pListBox) {
		pListBox->ClearItem();

		for (std::vector<sStoredItemInfo*>::iterator it = m_storedItemInfo.begin(); it != m_storedItemInfo.end(); ++it) {
			sStoredItemInfo* pInfo = *it;
			if (pInfo) {
				pListBox->AddItem(pInfo);
			}
		}
	}
}

// itemstorage.cpp:2226
void CItemStorage::DrawStoredItemInfo(const FrListBox*& pListBox, const FrListItem& item) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI && pListBox) {
		const Bitmap* pBg;

		if (item.selected == false && item.underCursor == false) {
			pBg = g_pFresh->GetBitmap("button_n");
		} else {
			pBg = g_pFresh->GetBitmap("button_o");
		}

		if (pBg) {
			pGDI->DrawTexture(pBg, WRect(item.pos.x, item.pos.y, (float)pBg->Width(), (float)pBg->Height()), 0xFFFFFFFF, 0);
		}

		const sStoredItemInfo* pInfo = (const sStoredItemInfo*)item.pData;

		if (pInfo) {
			IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pInfo->item.typeId);

			if (pCommon) {
				// The character the item belongs to.
				IFF_ITEM_COMMON* pChar = ItemManager()->FindCommonItem(((pInfo->item.typeId >> 18) & 0xFF) | 0x04000000);

				if (pChar) {
					const Bitmap* pCharIcon = g_pFresh->GetBitmap(MakeStr("s_%s", pChar->m_icon));

					if (pCharIcon) {
						pGDI->DrawTexture(pCharIcon, WRect(item.pos.x + 5.0f, item.pos.y + 3.0f, pCharIcon->Width() * 0.01f * 90.0f, pCharIcon->Height() * 0.01f * 90.0f), 0xFFFFFFFF, 0);
					}
				}

				const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);

				if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pInfo->item.typeId) && (pInfo->item.uccStatus & 1)) {
					pIcon = GetUccItemIcon(pInfo->item.typeId, pInfo->item.uccIdx);
				}

				unsigned long dragIndex = (m_dragDrop.mode == sStoredItemDragDrop::MODE_POP) ? m_dragDrop.index : (unsigned long)-1;

				if (dragIndex != (unsigned long)item.idx && pIcon) {
					pGDI->DrawTexture(pIcon, WRect(item.pos.x + 30.0f, item.pos.y + 1.0f, pIcon->Width() * 0.01f * 34.0f, pIcon->Height() * 0.01f * 34.0f), 0xFFFFFFFF, 0);
				}

				if (item.selected == true) {
					pGDI->SetTextStyle(1);
					pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
				} else {
					pGDI->SetTextStyle(0);
					pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
				}

				if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pInfo->item.typeId) && (pInfo->item.uccStatus & 1)) {
					pGDI->Print(WPoint(item.pos.x + 61.0f, item.pos.y + 10.0f), 0, GetUccItemName(pInfo->item.typeId, pInfo->item.uccIdx));
				} else {
					pGDI->Print(WPoint(item.pos.x + 61.0f, item.pos.y + 10.0f), 0, pCommon->m_name);
				}

				if ((pInfo->item.typeId >> 26) != 2) {
					pGDI->Print(WPoint(item.pos.x + 220.0f, item.pos.y + 10.0f), 0, "%d\xb0\xb3", pInfo->item.count);
				}

				pGDI->SetTextStyle(0);
				pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
			}
		}
	}
}

// itemstorage.cpp:2354
void CItemStorage::InitIndexControl(unsigned long totalCount, unsigned long countPerPage) {
	if (m_indexControl) {
		m_indexControl->Init(totalCount, countPerPage);
		m_indexControl->SetSelectedNumber(m_curPage);
	}
}

// itemstorage.cpp:2364
void CItemStorage::IndexControlClick(void) {
	if (m_indexControl) {
		if (m_indexControl->Click() == true) {
			unsigned long selectedNumber = m_indexControl->GetSelectedNumber();

			WSendPacket packet((enumClientPacket)0xCF);
			packet.Encode4(0x63);
			packet.Encode2((unsigned char)selectedNumber);
			packet.Send(SEND_TO_DEFAULT);
			Lock();

			m_curPage = selectedNumber;
		}
	}
}

// itemstorage.cpp:2386
void CItemStorage::IndexControlNext(void) {
	if (m_indexControl) {
		if (m_indexControl->NextPage() == true) {
			unsigned long selectedNumber = m_indexControl->GetSelectedNumber();

			WSendPacket packet((enumClientPacket)0xCF);
			packet.Encode4(0x63);
			packet.Encode2((unsigned char)selectedNumber);
			packet.Send(SEND_TO_DEFAULT);
			Lock();

			m_curPage = selectedNumber;
		}
	}
}

// itemstorage.cpp:2408
void CItemStorage::IndexControlPrev(void) {
	if (m_indexControl) {
		if (m_indexControl->PrevPage() == true) {
			unsigned long selectedNumber = m_indexControl->GetSelectedNumber();

			WSendPacket packet((enumClientPacket)0xCF);
			packet.Encode4(0x63);
			packet.Encode2((unsigned char)selectedNumber);
			packet.Send(SEND_TO_DEFAULT);
			Lock();

			m_curPage = selectedNumber;
		}
	}
}

// itemstorage.cpp:2430
void CItemStorage::IndexControlDraw(WPoint& pt) {
	if (m_indexControl) {
		m_indexControl->Draw(pt);
	}
}

// itemstorage.cpp:2439
bool CItemStorage::NeedPageIndexer(void) {
	bool ret = false;

	if (m_indexControl) {
		if (m_indexControl->GetTotalPage() > 1) {
			return true;
		}
	}

	return ret;
}

// itemstorage.cpp:2459
void CItemStorage::OpenPangBank(FrItemStoragePangBank::eType type) {
	if (m_pPangBank == 0) {
		m_pPangBank = CreateForm<FrItemStoragePangBank>(g_pFresh->GetManager(), this, "item_storage_pang_bank", 0);
	}

	if (m_pPangBank) {
		m_pPangBank->SetStoredPang(m_storedPang);
		unsigned long myPang = (unsigned long)WSingleton<CSharedDoc>::Instance()->m_myPang;
		m_pPangBank->SetMyPang(myPang);
		m_pPangBank->SetType(type);
		m_pPangBank->Open((FRESH_RESULT_CALLBACK)&CItemStorage::OnResultPangBank, 9);
	}
}

// itemstorage.cpp:2476
bool CItemStorage::OnResultPangBank(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		FrItemStoragePangBank* pPangBank = (FrItemStoragePangBank*)pForm;

		if (pPangBank) {
			WSendPacket packet((enumClientPacket)0xD6);

			if (pPangBank->GetType() == FrItemStoragePangBank::TYPE_WITHDRAW) {
				packet.Encode1(0);
			} else {
				packet.Encode1(1);
			}

			packet.Encode8(pPangBank->GetStoredPang());
			packet.Send(SEND_TO_DEFAULT);
			Lock();
		}
	}

	m_pPangBank = 0;

	return true;
}

// itemstorage.cpp:2516
bool CItemStorage::OnResultItemCounter(int ret, FrForm* pForm) {
	bool result = false;

	if (ret == FrOK) {
		FrItemCounter* pCounter = (FrItemCounter*)pForm;

		if (pCounter) {
			sStoredItemInfo info = pCounter->GetStoredItemInfo();

			switch (pCounter->GetType()) {
			case 1: {
				WSendPacket packet((enumClientPacket)0xD1);
				packet.Encode1(1);
				packet.EncodeBuffer(&info, sizeof(info));
				packet.Send(SEND_TO_DEFAULT);
				Lock();

				result = true;
				break;
			}
			case 0: {
				WSendPacket packet((enumClientPacket)0xD0);
				packet.Encode1(1);
				packet.EncodeBuffer(&info, sizeof(info));
				packet.Send(SEND_TO_DEFAULT);
				Lock();

				result = true;
				break;
			}
			}
		}
	} else {
		result = true;
	}

	m_pItemCounter = 0;

	return result;
}

// itemstorage.cpp:2572
bool CItemStorage::OnResultItemConfirm(int ret, FrForm* pForm) {
	bool result = false;

	if (ret == FrOK) {
		FrItemConfirm* pConfirm = (FrItemConfirm*)pForm;

		if (pConfirm) {
			sStoredItemInfo info = pConfirm->GetStoredItemInfo();

			switch (pConfirm->GetType()) {
			case 1: {
				WSendPacket packet((enumClientPacket)0xD1);
				packet.Encode1(1);
				packet.EncodeBuffer(&info, sizeof(info));
				packet.Send(SEND_TO_DEFAULT);
				Lock();

				result = true;
				break;
			}
			case 0: {
				WSendPacket packet((enumClientPacket)0xD0);
				packet.Encode1(1);
				packet.EncodeBuffer(&info, sizeof(info));
				packet.Send(SEND_TO_DEFAULT);
				Lock();

				result = true;
				break;
			}
			}
		}
	} else {
		result = true;
	}

	m_pItemConfirm = 0;

	return result;
}

// itemstorage.cpp:2627
bool CItemStorage::CheckDrag(void) {
	bool ret = false;

	if (g_input->GetButton(LEFT_BUTTON) == 3) {
		const WPoint& pt = g_pFresh->GetManager()->GetMousePos();

		m_dragDrop.pos.x = pt.x;
		m_dragDrop.pos.y = pt.y;

		return true;
	}

	return ret;
}

// itemstorage.cpp:2644
bool CItemStorage::CheckDrop(void) {
	const WPoint& pt = g_pFresh->GetManager()->GetMousePos();

	if (g_input->GetButton(LEFT_BUTTON) == 2) {
		switch (m_dragDrop.mode) {
		case sStoredItemDragDrop::MODE_POP:
			if (m_itemListArea.IsInRect(pt) == true) {
				// slot and item lie as an sStoredItemInfo at 0x14
				ReqPopStoredItemInfo(*reinterpret_cast<const sStoredItemInfo*>(&m_dragDrop.slot));
			}
			break;
		case sStoredItemDragDrop::MODE_PUSH:
			if (m_itemStorageArea.IsInRect(pt) == true) {
				if (m_dragDrop.pItemInfo) {
					ReqPushStoredItemInfo(*m_dragDrop.pItemInfo);
				}
			}
			break;
		}

		memset(&m_dragDrop, 0, sizeof(m_dragDrop));
	}

	return false;
}

// itemstorage.cpp:2688
void CItemStorage::SetDragDropItem(sStoredItemDragDrop& dragDrop) {
	m_dragDrop = dragDrop;
}

// itemstorage.cpp:2694
void CItemStorage::SetDrag(sStoredItemDragDrop::MODE mode) {
	if (m_dragDrop.item.typeId) {
		if (m_dragDrop.mode == sStoredItemDragDrop::MODE_NONE) {
			m_dragDrop.mode = mode;
		}
	}
}

// itemstorage.cpp:2706
void CItemStorage::DragDropItemDraw(void) {
	if (CheckDrag()) {
		if (m_dragDrop.mode != sStoredItemDragDrop::MODE_NONE) {
			FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
			IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(m_dragDrop.item.typeId);

			if (pGDI && pCommon) {
				const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);

				if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pCommon->m_typeId) && (m_dragDrop.item.uccStatus & 1)) {
					const Bitmap* pUccIcon = GetUccItemIcon(m_dragDrop.item.typeId, m_dragDrop.item.uccIdx);

					if (pUccIcon) {
						pIcon = pUccIcon;
					}
				}

				if (pIcon) {
					float w = (float)pIcon->Width();
					float h = (float)pIcon->Height();

					pGDI->DrawTexture(pIcon, WRect(m_dragDrop.pos.x - w * 0.5f, m_dragDrop.pos.y - h * 0.5f, w, h), 0xFFFFFFFF, 0);
				}
			}
		}
	} else {
		CheckDrop();
	}
}

// itemstorage.cpp:2786
unsigned long CItemStorage::GetDragingItemIndex(sStoredItemDragDrop::MODE mode) {
	unsigned long index = (unsigned long)-1;

	if (m_dragDrop.mode == mode) {
		index = m_dragDrop.index;
	}

	return index;
}

// globalnetworkdefine.h:2982
void sStoredItemInfo::SetTypeID(unsigned long typeId) {
	item.typeId = typeId;
}

// globalnetworkdefine.h:2985
void sStoredItemInfo::SetGuid(unsigned long guid) {
	item.guid = guid;
}

// globalnetworkdefine.h:2987
unsigned long sStoredItemInfo::GetCount(void) const {
	return item.count;
}

// globalnetworkdefine.h:2988
void sStoredItemInfo::SetCount(unsigned long count) {
	item.count = count;
}
