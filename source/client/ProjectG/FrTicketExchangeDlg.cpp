// Original: d:\build\custom\temp\code\client\projectg\frticketexchangedlg.cpp
#include <stdio.h>
#include <string.h>

#include "FrTicketExchangeDlg.h"
#include "fredit.h"
#include "frbutton.h"
#include "frviewer.h"
#include "fresh.h"
#include "ObjectFactory.h"
#include "singleton.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"

// frticketexchangedlg.h:44
TicketInfo::sTicketItem TicketInfo::FindCount(unsigned long tid) {
	sTicketItem item;
	item.m_count = 0;
	item.m_maxCount = 0;

	std::map<unsigned long, sTicketItem>::iterator it = m_counts.find(tid);
	if (it != m_counts.end()) {
		item = it->second;
	}

	return item;
}

// frticketexchangedlg.h:60
TicketItemObject::TicketItemObject(void) {
	m_TID = 0;
	m_Quantity = 0;
	m_pButton = 0;
	m_pViewer = 0;
	m_pViewerOver = 0;

	for (int i = 0; i < 2; ++i) {
		m_pEdit[i] = 0;
	}
}

// frticketexchangedlg.h:71
TicketItemObject::~TicketItemObject(void) {
}

// frticketexchangedlg.h:82
void TicketItemObject::set_TID(unsigned long tid) {
	m_TID = tid;
}

// frticketexchangedlg.h:83
unsigned long TicketItemObject::get_TID(void) {
	return m_TID;
}

// frticketexchangedlg.h:85
void TicketItemObject::set_Quantity(unsigned long quantity) {
	m_Quantity = quantity;
}

// frticketexchangedlg.h:86
unsigned long TicketItemObject::get_Quantity(void) {
	return m_Quantity;
}

// frticketexchangedlg.h:89
FrButton* TicketItemObject::get_Button(int wnd) {
	if (m_pButton == 0) {
		m_pButton = DYNAMIC_CAST(FrButton, wnd);
	}

	return m_pButton;
}

// frticketexchangedlg.h:108
FrViewer* TicketItemObject::get_Viewer(int wnd, bool over) {
	FrViewer* pViewer = m_pViewer;
	if (over) {
		pViewer = m_pViewerOver;
	}

	if (pViewer == 0) {
		pViewer = DYNAMIC_CAST(FrViewer, wnd);
	}

	if (over) {
		m_pViewerOver = pViewer;
	} else {
		m_pViewer = pViewer;
	}

	return pViewer;
}

// frticketexchangedlg.cpp:10
unsigned long nsTicketHelper::Combine(unsigned long group, unsigned long index) {
	return group | index;
}

// frticketexchangedlg.cpp:15
void nsTicketHelper::Analyze(unsigned long& tid, unsigned long& group, unsigned long& index) {
	group = tid & 0xffff0000;
	index = tid & 0xffff;
}

// frticketexchangedlg.cpp:37 (run-time type and factory registration of FrTicketExchangeDescDlg)
const WRTTI FrTicketExchangeDescDlg::m_RTTI("FrTicketExchangeDescDlg", &FrForm::m_RTTI);

IObject* FrTicketExchangeDescDlgMakeInstance(void) {
	return new FrTicketExchangeDescDlg;
}

struct __sFrTicketExchangeDescDlg {
	__sFrTicketExchangeDescDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTicketExchangeDescDlgMakeInstance, "FrTicketExchangeDescDlg");
	}
};
static __sFrTicketExchangeDescDlg s_registerFrTicketExchangeDescDlg;

// frticketexchangedlg.cpp:39
const sFRESH_MSGMAP* FrTicketExchangeDescDlg::GetMessageMap(void) const {
	return &FrTicketExchangeDescDlg::_MsgMap;
}

sFRESH_MSGMAP FrTicketExchangeDescDlg::_MsgMap = { &FrForm::_MsgMap, &FrTicketExchangeDescDlg::_MsgEntries[0] };

sFRESH_ENTRY FrTicketExchangeDescDlg::_MsgEntries[] = {
	// frticketexchangedlg.cpp:41
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTicketExchangeDescDlg::OnInitDescViewer },

	// frticketexchangedlg.cpp:43
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frticketexchangedlg.cpp:46
FrTicketExchangeDescDlg::FrTicketExchangeDescDlg(void) {
	m_pDescViewer = 0;
	memset(m_descImage, 0, sizeof(m_descImage));
}

// frticketexchangedlg.cpp:52
FrTicketExchangeDescDlg::~FrTicketExchangeDescDlg(void) {
}

// frticketexchangedlg.cpp:56
void FrTicketExchangeDescDlg::SetDescImage(const char* name) {
	if (m_pDescViewer) {
		memset(m_descImage, 0, sizeof(m_descImage));
		strcpy(m_descImage, name);
		m_pDescViewer->ShowScrollBar(true);
		m_pDescViewer->Open(m_descImage);
	}
}

// frticketexchangedlg.cpp:67
void FrTicketExchangeDescDlg::OnInitDescViewer(intptr_t wnd) {
	m_pDescViewer = DYNAMIC_CAST(FrViewer, wnd);
}

// frticketexchangedlg.cpp:74 (run-time type and factory registration of FrTicketExchangeDlg)
const WRTTI FrTicketExchangeDlg::m_RTTI("FrTicketExchangeDlg", &FrForm::m_RTTI);

IObject* FrTicketExchangeDlgMakeInstance(void) {
	return new FrTicketExchangeDlg;
}

struct __sFrTicketExchangeDlg {
	__sFrTicketExchangeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTicketExchangeDlgMakeInstance, "FrTicketExchangeDlg");
	}
};
static __sFrTicketExchangeDlg s_registerFrTicketExchangeDlg;

// frticketexchangedlg.cpp:76
const sFRESH_MSGMAP* FrTicketExchangeDlg::GetMessageMap(void) const {
	return &FrTicketExchangeDlg::_MsgMap;
}

// frticketexchangedlg.cpp:143
FrTicketExchangeDlg::FrTicketExchangeDlg(void) {
	m_pDescDlg = 0;
	memset(m_pNumber, 0, sizeof(m_pNumber));

	new TicketInfo;
}

// frticketexchangedlg.cpp:153
FrTicketExchangeDlg::~FrTicketExchangeDlg(void) {
	std::map<unsigned long, TicketItemObject*>::iterator it = m_items.begin();
	for (; it != m_items.end(); ++it) {
		if (it->second) {
			delete it->second;
			it->second = 0;
		}
	}

	m_items.clear();
	memset(m_pNumber, 0, sizeof(m_pNumber));

	if (TicketInfo::Instance()) {
		delete TicketInfo::Instance();
	}
}

// frticketexchangedlg.cpp:173
bool FrTicketExchangeDlg::OnTicketExchangeDescResult(int ret, FrForm* pForm) {
	m_pDescDlg = 0;

	return true;
}

// frticketexchangedlg.cpp:182
TicketItemObject* FrTicketExchangeDlg::FindItem(unsigned long tid) {
	TicketItemObject* pItem = 0;

	std::map<unsigned long, TicketItemObject*>::iterator it = m_items.find(tid);
	if (it != m_items.end()) {
		pItem = it->second;
	}

	if (pItem == 0) {
		pItem = new TicketItemObject;
		m_items[tid] = pItem;
	}

	return pItem;
}

// frticketexchangedlg.cpp:203
bool FrTicketExchangeDlg::Request(unsigned int group, unsigned long tid, unsigned int quantity) {
	WSendPacket packet(0xe1);
	packet.Encode4(group);

	switch (group) {
	case 0x10000:
	case 0x100000:
	case 0x1000000:
		packet.Encode4(tid);
		packet.Encode4(quantity);
		break;
	}

	packet.Send((eSendTo)0);
	AfxGetTask()->GetActor("Lobby") << MsgObject(0, 1, 0, 0, 0, 0, 0);

	return true;
}

// frticketexchangedlg.cpp:226
void FrTicketExchangeDlg::Reponse(unsigned long group, unsigned char result) {
	char message[256] = { 0 };

	switch (result) {
	case 0:
		switch (group) {
		case 0x100000:
			TicketInfo::Instance()->m_groupGState = 0;

		case 0x1000000:
			{
				const char* text = "\xC1\xF6\xB1\xDE \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.\n\xBF\xEC\xC6\xED\xC7\xD4\xC0\xBB \xC8\xAE\xC0\xCE\xC7\xD8 \xBA\xB8\xBD\xCA\xBD\xC3\xBF\xC0.";
				strcpy(message, text);
			}
			break;

		case 0x10000:
			{
				const char* text = "\xC0\xC0\xB8\xF0 \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.\n\xC7\xE0\xBF\xEE\xC0\xBB \xB1\xE2\xBF\xF8\xC7\xD5\xB4\xCF\xB4\xD9.";
				strcpy(message, text);
			}
			break;

		case 0x11110000:
			ApplyInformation();
			return;
		}
		break;

	case 4:
		{
			const char* text = "\xB1\xB3\xC8\xAF \xB0\xA1\xB4\xC9\xC7\xD1 \xC6\xBC\xC4\xCF\xC0\xCC \xC0\xFB\xBD\xC0\xB4\xCF\xB4\xD9.\n\xC1\xB6\xB1\xDD \xB4\xF5 \xB8\xF0\xBE\xC6\xC1\xD6\xBD\xC3\xB1\xE2 \xB9\xD9\xB6\xF8\xB4\xCF\xB4\xD9.";
			strcpy(message, text);
		}
		break;

	case 2:
		{
			const char* text = "\xC7\xD8\xB4\xE7 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xC7 \xC0\xE7\xB0\xED\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.\n\xB4\xD9\xBD\xC3 \xB5\xE9\xBE\xEE\xBF\xC3\xB6\xA7\xB1\xEE\xC1\xF6 \xB1\xE2\xB4\xD9\xB7\xC1 \xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0.";
			strcpy(message, text);
		}
		break;
	}

	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
	pForm->SetTimeLimit(3.0f);
	pForm->SetMessage(message, true);
	pForm->Open(0, 0);

	if (result == 0) {
		ApplyInformation();
	}
}

// frticketexchangedlg.cpp:392
void FrTicketExchangeDlg::ChangeItemState(TicketItemObject* pItem, int state) {
	if (pItem) {
		FrViewer* pViewer = pItem->get_Viewer(0, false);

		FrViewer* pViewerOver = pItem->get_Viewer(0, true);

		char name[64] = { 0 };
		switch (state) {
		case -1:
			pViewer->Enable(false);
			pViewer->SetVisible(false);

			if (pViewerOver) {
				pViewerOver->Enable(false);
				pViewerOver->SetVisible(false);
			}
			break;

		case 2:
			if (strlen(name) == 0) {
				strcpy(name, "soldout.tga");
			}

		case 1:
			if (strlen(name) == 0) {
				strcpy(name, "payment.tga");
			}

			pViewer->Enable(true);
			pViewer->SetVisible(true);

			pViewer->Open(name);

			if (pViewerOver) {
				pViewerOver->Enable(true);
				pViewerOver->SetVisible(true);
			}
			break;
		}
	}
}

// frticketexchangedlg.cpp:443
void FrTicketExchangeDlg::SetCountImage(unsigned int count) {
	if (count > 9999) {
		count = 9999;
	}

	unsigned int pos = 0;
	char num[5] = { 0 };
	sprintf(num, "%d", count);
	unsigned int len = strlen(num);

	WRect rect;
	rect = WRect(457.0f, 69.0f, 14.0f, 19.0f);

	rect.x += (4.0f - len) * 0.5f * rect.w;

	const WRect& wndRect = GetRect();

	rect.x += wndRect.x;
	rect.y += wndRect.y;

	for (int i = 0; i < 4; i++) {
		char name[16] = { 0 };
		bool show = false;
		if (pos < len) {
			sprintf(name, "ticket_%c.tga", num[pos++]);
			show = true;
		}

		m_pNumber[i]->Enable(show);
		m_pNumber[i]->SetVisible(show);
		m_pNumber[i]->Open(name);

		m_pNumber[i]->SetRect(rect);
		rect.x += rect.w;
	}
}

// frticketexchangedlg.cpp:523
bool FrTicketExchangeDlg::OnInit(void) {
	unsigned long ticketIds[8] = { 0 };
	ticketIds[0] = 0x3B9AC9F7;
	ticketIds[1] = 0x3B9AC9F8;
	ticketIds[2] = 0x1A00000E;
	ticketIds[3] = 0x18000006;
	ticketIds[4] = 0x1A000011;
	ticketIds[5] = 0x1A000040;
	ticketIds[6] = 0x1A0000BC;
	ticketIds[7] = 0x1A000030;

	unsigned long quantities[8] = { 0 };
	quantities[0] = 1;
	quantities[1] = 1;
	quantities[2] = 5;
	quantities[3] = 5;
	quantities[4] = 5;
	quantities[5] = 5;
	quantities[6] = 3;
	quantities[7] = 3;

	unsigned long tids[8] = { 0 };
	tids[0] = nsTicketHelper::Combine(0x10000, 1);
	tids[1] = nsTicketHelper::Combine(0x10000, 2);
	tids[2] = nsTicketHelper::Combine(0x100000, 1);
	tids[3] = nsTicketHelper::Combine(0x100000, 2);
	tids[4] = nsTicketHelper::Combine(0x100000, 3);
	tids[5] = nsTicketHelper::Combine(0x100000, 4);
	tids[6] = nsTicketHelper::Combine(0x1000000, 1);
	tids[7] = nsTicketHelper::Combine(0x1000000, 2);

	for (unsigned int i = 0; i < 8; i++) {
		TicketItemObject* pItem = FindItem(tids[i]);
		if (pItem) {
			pItem->set_TID(ticketIds[i]);
			pItem->set_Quantity(quantities[i]);
		}
	}

	for (int j = 0; j < 6; j++) {
		std::map<unsigned long, TicketInfo::sTicketItem>& counts = TicketInfo::Instance()->m_counts;
		std::map<unsigned long, TicketInfo::sTicketItem>::iterator it = counts.find(ticketIds[j]);
		if (it == TicketInfo::Instance()->m_counts.end()) {
			counts[ticketIds[j]] = TicketInfo::sTicketItem();
		}
	}

	Request(0x11110000, 0, 0);

	return true;
}

// frticketexchangedlg.cpp:589
void FrTicketExchangeDlg::OnProc(const float dt) {
}

// frticketexchangedlg.cpp:596
void FrTicketExchangeDlg::onLBtnUp_Help(void) {
	if (m_pDescDlg == 0) {
		m_pDescDlg = CreateForm<FrTicketExchangeDescDlg>(g_pFresh->GetManager(), this, "ticketexchange_desc", 0);
	}

	if (m_pDescDlg) {
		m_pDescDlg->SetDescImage("tiketexchange_event_explanation.jpg");
		m_pDescDlg->SetIconRect(m_pDescDlg->GetRect());
		m_pDescDlg->Open((FRESH_RESULT_CALLBACK)&FrTicketExchangeDlg::OnTicketExchangeDescResult, 0x11);
	}
}

// frticketexchangedlg.cpp:613
void FrTicketExchangeDlg::onInit_Icon_0(int wnd) {
	TicketItemObject* pItem = FindItem(0x10001); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 1); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:618
void FrTicketExchangeDlg::onInit_Icon_1(int wnd) {
	TicketItemObject* pItem = FindItem(0x10002); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 2); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:623
void FrTicketExchangeDlg::onInit_Icon_2(int wnd) {
	TicketItemObject* pItem = FindItem(0x100001); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 3); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:628
void FrTicketExchangeDlg::onInit_Icon_3(int wnd) {
	TicketItemObject* pItem = FindItem(0x100002); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 4); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:633
void FrTicketExchangeDlg::onInit_Icon_4(int wnd) {
	TicketItemObject* pItem = FindItem(0x100003); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 5); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:638
void FrTicketExchangeDlg::onInit_Icon_5(int wnd) {
	TicketItemObject* pItem = FindItem(0x100004); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 6); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:643
void FrTicketExchangeDlg::onInit_Icon_6(int wnd) {
	TicketItemObject* pItem = FindItem(0x1000001); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 7); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:648
void FrTicketExchangeDlg::onInit_Icon_7(int wnd) {
	TicketItemObject* pItem = FindItem(0x1000002); if (pItem) { char name[32] = { 0 }; sprintf(name, "ticket_frame%d.tga", 8); FrViewer* pViewer = pItem->get_Viewer(wnd, true); pViewer->Open(name); pViewer->Enable(false); pViewer->SetVisible(false); }
}

// frticketexchangedlg.cpp:656
void FrTicketExchangeDlg::onInit_ItemE0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10001); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:661
void FrTicketExchangeDlg::onLBtnUp_ItemE0(void) {
	TicketItemObject* pItem = FindItem(0x10001);
	if (pItem) {
		Request(0x10000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:671
void FrTicketExchangeDlg::onInit_Edit0E0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10001); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:676
void FrTicketExchangeDlg::onInit_Edit1E0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10001); if (pItem) pItem->get_Edit(1, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:683
void FrTicketExchangeDlg::onInit_ItemE1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10002); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:688
void FrTicketExchangeDlg::onLBtnUp_ItemE1(void) {
	TicketItemObject* pItem = FindItem(0x10002);
	if (pItem) {
		Request(0x10000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:698
void FrTicketExchangeDlg::onInit_Edit0E1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10002); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:703
void FrTicketExchangeDlg::onInit_Edit1E1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x10002); if (pItem) pItem->get_Edit(1, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:711
void FrTicketExchangeDlg::onInit_ItemG0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100001); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:716
void FrTicketExchangeDlg::onLBtnUp_ItemG0(void) {
	TicketItemObject* pItem = FindItem(0x100001);
	if (pItem) {
		Request(0x100000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:726
void FrTicketExchangeDlg::onInit_Edit0G0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100001); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:733
void FrTicketExchangeDlg::onInit_ItemG1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100002); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:738
void FrTicketExchangeDlg::onLBtnUp_ItemG1(void) {
	TicketItemObject* pItem = FindItem(0x100002);
	if (pItem) {
		Request(0x100000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:748
void FrTicketExchangeDlg::onInit_Edit0G1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100002); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:755
void FrTicketExchangeDlg::onInit_ItemG2(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100003); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:760
void FrTicketExchangeDlg::onLBtnUp_ItemG2(void) {
	TicketItemObject* pItem = FindItem(0x100003);
	if (pItem) {
		Request(0x100000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:770
void FrTicketExchangeDlg::onInit_Edit0G2(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100003); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:777
void FrTicketExchangeDlg::onInit_ItemG3(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100004); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:782
void FrTicketExchangeDlg::onLBtnUp_ItemG3(void) {
	TicketItemObject* pItem = FindItem(0x100004);
	if (pItem) {
		Request(0x100000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:792
void FrTicketExchangeDlg::onInit_Edit0G3(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100004); if (pItem) pItem->get_Edit(0, wnd)->AddText("12345678", false, true);
}

// frticketexchangedlg.cpp:800
void FrTicketExchangeDlg::onInit_ItemI0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x1000001); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:805
void FrTicketExchangeDlg::onLBtnUp_ItemI0(void) {
	TicketItemObject* pItem = FindItem(0x1000001);
	if (pItem) {
		Request(0x1000000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:817
void FrTicketExchangeDlg::onInit_Item_I1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x1000002); if (pItem) pItem->get_Button(wnd);
}

// frticketexchangedlg.cpp:822
void FrTicketExchangeDlg::onLBtnUp_ItemI1(void) {
	TicketItemObject* pItem = FindItem(0x1000002);
	if (pItem) {
		Request(0x1000000, pItem->get_TID(), pItem->get_Quantity());
	}
}

// frticketexchangedlg.cpp:834
void FrTicketExchangeDlg::onInit_Number_0(int wnd) {
	if (m_pNumber[0] == 0) {
		m_pNumber[0] = DYNAMIC_CAST(FrViewer, wnd);
	}
}

// frticketexchangedlg.cpp:840
void FrTicketExchangeDlg::onInit_Number_1(int wnd) {
	if (m_pNumber[1] == 0) {
		m_pNumber[1] = DYNAMIC_CAST(FrViewer, wnd);
	}
}

// frticketexchangedlg.cpp:846
void FrTicketExchangeDlg::onInit_Number_2(int wnd) {
	if (m_pNumber[2] == 0) {
		m_pNumber[2] = DYNAMIC_CAST(FrViewer, wnd);
	}
}

// frticketexchangedlg.cpp:852
void FrTicketExchangeDlg::onInit_Number_3(int wnd) {
	if (m_pNumber[3] == 0) {
		m_pNumber[3] = DYNAMIC_CAST(FrViewer, wnd);
	}
}

// frticketexchangedlg.cpp:860
void FrTicketExchangeDlg::onInit_View0(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100001); if (pItem) { pItem->get_Viewer(wnd, false); ChangeItemState(pItem, -1); }
}

// frticketexchangedlg.cpp:865
void FrTicketExchangeDlg::onInit_View1(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100002); if (pItem) { pItem->get_Viewer(wnd, false); ChangeItemState(pItem, -1); }
}

// frticketexchangedlg.cpp:870
void FrTicketExchangeDlg::onInit_View2(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100003); if (pItem) { pItem->get_Viewer(wnd, false); ChangeItemState(pItem, -1); }
}

// frticketexchangedlg.cpp:875
void FrTicketExchangeDlg::onInit_View3(int wnd) {
	char text[32];   // unused: only its /GS cookie remains in the original
	TicketItemObject* pItem = FindItem(0x100004); if (pItem) { pItem->get_Viewer(wnd, false); ChangeItemState(pItem, -1); }
}

// Instantiates the singleton destructor this unit emits.
template WSingleton<TicketInfo>::~WSingleton();
