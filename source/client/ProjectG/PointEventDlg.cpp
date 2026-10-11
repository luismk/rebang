// Original: d:\build\custom\temp\code\client\projectg\pointeventdlg.cpp
#include <string.h>

#include "PointEventDlg.h"
#include "ObjectFactory.h"
#include "frviewer.h"
#include "../shared/Localize.h"
#include "frarea.h"
#include "frbutton.h"
#include "commonutil.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "Packet.h"
#include "SharedDoc.h"

// pointeventdlg.cpp:28
CCommodityTable g_CommodityTable;
// pointeventdlg.cpp:29
CCommodityStatusManager g_CommodityStatusManager;

// Written by Confirm only, no symbol.
static unsigned short data_aba218;

// pointeventdlg.cpp:38
CCommodityItem::CCommodityItem(void) {
	m_typeID = 0;
	m_value = 0;
	m_itemType = (eItemType)0;
}

// pointeventdlg.cpp:45
CCommodityItem::CCommodityItem(const CCommodityItem& rhs) {
	m_typeID = rhs.m_typeID;
	m_value = rhs.m_value;
	m_itemType = rhs.m_itemType;
}

// pointeventdlg.cpp:52
CCommodityItem::CCommodityItem(unsigned long typeID, unsigned long value, eItemType itemType) {
	m_typeID = typeID;
	m_value = value;
	m_itemType = itemType;
}

// pointeventdlg.cpp:59
void CCommodityItem::SetTypeID(unsigned long typeID) {
	m_typeID = typeID;
}

// pointeventdlg.cpp:64
void CCommodityItem::SetValue(unsigned long value) {
	m_value = value;
}

// pointeventdlg.cpp:69
void CCommodityItem::SetItemType(eItemType itemType) {
	m_itemType = itemType;
}

// pointeventdlg.cpp:74
unsigned long CCommodityItem::GetTypeID(void) const {
	return m_typeID;
}

// pointeventdlg.cpp:79
unsigned long CCommodityItem::GetValue(void) const {
	return m_value;
}

// pointeventdlg.cpp:84
CCommodityItem::eItemType CCommodityItem::GetItemType(void) const {
	return m_itemType;
}

// pointeventdlg.cpp:89
const CCommodityItem& CCommodityItem::operator=(const CCommodityItem& rhs) {
	m_typeID = rhs.m_typeID;
	m_value = rhs.m_value;
	m_itemType = rhs.m_itemType;
	return *this;
}

// pointeventdlg.cpp:101
CCommodity::CCommodity(void) {
	m_itemCount = 0;
	m_commodityCost = 0;
}

// pointeventdlg.cpp:107
CCommodity::CCommodity(const CCommodity& rhs) {
	m_itemCount = rhs.m_itemCount;

	for (int i = 0; i < m_itemCount; i++) {
		m_items[i] = rhs.GetCommodityItem(i);
	}

	m_commodityCost = rhs.m_commodityCost;
}

// pointeventdlg.cpp:119
CCommodity::CCommodity(const CCommodityItem& item, unsigned short cost) {
	m_itemCount = 1; m_items[0] = item;
	m_commodityCost = cost;
}

// pointeventdlg.cpp:126
void CCommodity::SetCommodityCost(unsigned short cost) {
	m_commodityCost = cost;
}

// pointeventdlg.cpp:131
unsigned short CCommodity::GetCommodityItemCount(void) const {
	return m_itemCount;
}

// pointeventdlg.cpp:136
unsigned short CCommodity::GetCommodityCost(void) const {
	return m_commodityCost;
}

// pointeventdlg.cpp:141
bool CCommodity::InsertCommodityItem(const CCommodityItem& item) {
	if (m_itemCount >= 4) {
		return false;
	}

	m_items[m_itemCount] = item;
	m_itemCount++;
	return true;
}

// pointeventdlg.cpp:154
const CCommodityItem& CCommodity::GetCommodityItem(unsigned short index) const {
	return m_items[index];
}

// pointeventdlg.cpp:159
const CCommodity& CCommodity::operator=(const CCommodity& rhs) {
	m_itemCount = rhs.GetCommodityItemCount();

	for (int i = 0; i < m_itemCount; i++) {
		m_items[i] = rhs.GetCommodityItem(i);
	}

	m_commodityCost = rhs.GetCommodityCost();
	return *this;
}

// pointeventdlg.cpp:176
CCommodityTable::CCommodityTable(void) {
	InitializeTable();
}

// pointeventdlg.cpp:183
bool CCommodityTable::InitializeTable(void) {
	CCommodity table[10];

	table[0].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[0].SetCommodityCost(600);

	table[1].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[1].SetCommodityCost(600);

	table[2].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[2].SetCommodityCost(600);

	table[3].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[3].SetCommodityCost(600);

	table[4].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[4].SetCommodityCost(600);

	table[5].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[5].SetCommodityCost(600);

	table[6].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[6].SetCommodityCost(600);

	table[7].InsertCommodityItem(CCommodityItem(0, 0, (CCommodityItem::eItemType)4));
	table[7].SetCommodityCost(300);

	table[8].InsertCommodityItem(CCommodityItem(0x1a000010, 3000, (CCommodityItem::eItemType)1));
	table[8].SetCommodityCost(150);

	table[9].InsertCommodityItem(CCommodityItem(0x1a00003f, 1, (CCommodityItem::eItemType)5));
	table[9].SetCommodityCost(100);

	for (int i = 0; i < 10; i++) {
		m_commodities[i] = table[i];
	}

	return true;
}

// pointeventdlg.cpp:301
const CCommodity& CCommodityTable::GetCommodity(unsigned short index) const {
	if (index > 10) {
		index = 0;
	}

	return m_commodities[index];
}

// pointeventdlg.cpp:314
CCommodityStatusManager::CCommodityStatusManager(void) {
	m_status = NULL;
	InitializeStatus();
}

// pointeventdlg.cpp:320
CCommodityStatusManager::~CCommodityStatusManager(void) {
	if (m_status != NULL) {
		delete[] m_status;
		m_status = NULL;
	}
}

// pointeventdlg.cpp:329
void CCommodityStatusManager::InitializeStatus(void) {
	if (m_status == NULL) {
		m_status = new sCommodityStatus[10];

		for (int i = 0; i < 10; i++) {
			m_status[i].myCount = 0;
			m_status[i].totalCount = 0;
			m_status[i].status = 0;
		}
	}

	for (int i = 0; i < 10; i++) {
		m_updateTypes[i] = (i == 8) ? UPDATE_TYPE_NONE : UPDATE_TYPE_RAISE;
	}
}

// pointeventdlg.cpp:376
unsigned long CCommodityStatusManager::GetStatus(unsigned short index) const {
	return m_status[index].status;
}

// pointeventdlg.cpp:381
unsigned long CCommodityStatusManager::GetMyCount(unsigned short index) const {
	return m_status[index].myCount;
}

// pointeventdlg.cpp:386
unsigned long CCommodityStatusManager::GetTotalCount(unsigned short index) const {
	return m_status[index].totalCount;
}

// pointeventdlg.cpp:391
CCommodityStatusManager::eUpdateType CCommodityStatusManager::GetUpdateType(unsigned short index) const {
	return m_updateTypes[index];
}

// pointeventdlg.cpp:396
void CCommodityStatusManager::SetStatus(unsigned short index, unsigned long status) {
	m_status[index].status = status;
}

// pointeventdlg.cpp:401
void CCommodityStatusManager::SetMyCount(unsigned short index, unsigned long count) {
	m_status[index].myCount = count;
}

// pointeventdlg.cpp:406
void CCommodityStatusManager::SetTotalCount(unsigned short index, unsigned long count) {
	m_status[index].totalCount = count;
}

// pointeventdlg.cpp:411
void CCommodityStatusManager::SetData(unsigned short index, unsigned long status, unsigned long myCount, unsigned long totalCount) {
	m_status[index].status = status;
	m_status[index].myCount = myCount;
	m_status[index].totalCount = totalCount;
}

// pointeventdlg.cpp:419
void CCommodityStatusManager::UpdateTotalCount(unsigned short index, unsigned long count) {
	switch (m_updateTypes[index]) {
	case UPDATE_TYPE_RAISE:
		if (m_status[index].totalCount <= count) {
			m_status[index].totalCount = count;
		}
		break;
	case UPDATE_TYPE_LOWER:
		if (m_status[index].totalCount >= count || m_status[index].totalCount == 0) {
			m_status[index].totalCount = count;
		}
		break;
	case UPDATE_TYPE_OVERWRITE:
		m_status[index].totalCount = count;
		break;
	}
}

// pointeventdlg.cpp:457
void CCommodityStatusManager::UpdateStatus(unsigned short index, unsigned long status) {
	m_status[index].status = status;
}

// pointeventdlg.cpp:462
void CCommodityStatusManager::UpdateMyCount(unsigned short index, unsigned long count) {
	m_status[index].myCount = count;
}

// pointeventdlg.cpp:472
CCommodityButton::CCommodityButton(void) {
	m_buttonType = COMMODITY_BUTTON_NONE;
	m_button = NULL;
	m_cover = NULL;
	m_coverPos = WPoint(0.0f, 0.0f);
}

// pointeventdlg.cpp:480
CCommodityButton::~CCommodityButton(void) {
}

// pointeventdlg.cpp:484
CCommodityButton::eCommodityButtonType CCommodityButton::GetButtonType(void) const {
	return m_buttonType;
}

// pointeventdlg.cpp:489
bool CCommodityButton::IsVisible(void) const {
	return m_button->IsVisible() ? true : false;
}

// pointeventdlg.cpp:496
bool CCommodityButton::BindButton(FrButton* button) {
	if (button == NULL) {
		return false;
	}

	m_button = button;
	m_button->SetPushDelay(0.1f);
	return true;
}

// pointeventdlg.cpp:514
void CCommodityButton::SetCover(const char* name) {
	if (m_button != NULL) {
		m_cover = NULL;
		m_cover = g_pFresh->GetBitmap(name);

		m_coverPos.x = m_button->GetRect().x;
		m_coverPos.y = m_button->GetRect().y;
	}
}

// pointeventdlg.cpp:529
void CCommodityButton::ChangeButtonType(eCommodityButtonType type) {
	if (m_button == NULL) {
		return;
	}

	switch (type) {
	case COMMODITY_BUTTON_GIFT1:
		m_button->SetButtonImg("btn_pointevent_gift1_n", FrButton::NORMAL);
		m_button->SetButtonImg("btn_pointevent_gift1_o", FrButton::OVER);
		m_button->SetButtonImg("btn_pointevent_gift1_d", FrButton::PRESSED);
		m_buttonType = COMMODITY_BUTTON_GIFT1;
		break;
	case COMMODITY_BUTTON_GIFT2:
		m_button->SetButtonImg("btn_pointevent_gift2_n", FrButton::NORMAL);
		m_button->SetButtonImg("btn_pointevent_gift2_o", FrButton::OVER);
		m_button->SetButtonImg("btn_pointevent_gift2_d", FrButton::PRESSED);
		m_buttonType = COMMODITY_BUTTON_GIFT2;
		break;
	case COMMODITY_BUTTON_PLAIN:
		m_buttonType = COMMODITY_BUTTON_PLAIN;
		break;
	}
}

// pointeventdlg.cpp:591
void CCommodityButton::ButtonSwitch(bool on) {
	if (m_button != NULL) {
		m_button->SetVisible(on);
	}
}

// pointeventdlg.cpp:600
void CCommodityButton::DrawCover(void) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi == NULL) {
		return;
	}

	if (!m_button->IsVisible()) {
		return;
	}

	if (m_cover == NULL) {
		return;
	}

	m_coverPos.x = m_button->GetRect().x;
	m_coverPos.y = m_button->GetRect().y;

	int space = gdi->SetSpace(0);

	gdi->DrawTexture(m_cover, WRect(m_coverPos.x, m_coverPos.y, m_cover->Width(), m_cover->Height()), 0xffffffff, 0);

	gdi->SetSpace(space);
}

// pointeventdlg.cpp:631
CCommoditySlot::CCommoditySlot(void) {
	m_slotIndex = 0;

	m_slotStatus = SLOT_STATUS_OPEN;

	m_openMark = NULL;
	m_completedMark = NULL;

	m_myCount = 0;
	m_totalCount = 0;

	m_myCountPos = WPoint(0.0f, 0.0f);
	m_totalCountPos = WPoint(0.0f, 0.0f);
	m_markPos = WPoint(0.0f, 0.0f);

	m_linkCount = 0;



	for (int i = 0; i < 10; i++) {
		m_links[i] = NULL;
	}
}

// pointeventdlg.cpp:655
CCommoditySlot::~CCommoditySlot(void) {
}

// pointeventdlg.cpp:660
void CCommoditySlot::InitSlot(sCommoditySlotData& data) {
	m_slotIndex = data.slotIndex;
	m_slotStatus = SLOT_STATUS_OPEN;
	m_openMark = g_pFresh->GetBitmap(data.openMarkName);
	m_completedMark = g_pFresh->GetBitmap(data.completedMarkName);
	m_countDisplay = data.countDisplay;
	m_markFlags = data.markFlags;
	m_myCountPos = data.myCountPos;
	m_totalCountPos = data.totalCountPos;
	m_markPos = data.markPos;


	BindButton(data.button);
	SetCover(data.coverName);
	ChangeButtonType(data.buttonType);


	m_linkCount = 0;


	for (int i = 0; i < 10; i++) {
		m_links[i] = NULL;
	}
}

// pointeventdlg.cpp:685
void CCommoditySlot::SetMyCount(unsigned long count) {
	m_myCount = count;
}

// pointeventdlg.cpp:690
void CCommoditySlot::SetTotalCount(unsigned long count) {
	m_totalCount = count;
}

// pointeventdlg.cpp:695
void CCommoditySlot::SetSlotStatus(eSlotStatus status) {
	m_slotStatus = status;

	switch (status) {
	case SLOT_STATUS_OPEN:
		ButtonSwitch(true);
		break;
	case SLOT_STATUS_CLOSED:
	case SLOT_STATUS_LOCKED:
		ButtonSwitch(false);
		break;
	case SLOT_STATUS_COMPLETED:
		ButtonSwitch(false);
		break;
	}
}

// pointeventdlg.cpp:750
unsigned short CCommoditySlot::GetSlotIndex(void) const {
	return m_slotIndex;
}

// pointeventdlg.cpp:755
CCommoditySlot::eSlotStatus CCommoditySlot::GetSlotStatus(void) const {
	return m_slotStatus;
}

// pointeventdlg.cpp:760
WRect CCommoditySlot::GetButtonRect(void) const {
	WRect rect = m_button->GetRect();
	return rect;
}

// pointeventdlg.cpp:766
void CCommoditySlot::AddSlotLink(const CCommoditySlot* link) {
	if (link != NULL) {
		if (m_linkCount < 10) {
			m_links[m_linkCount] = link;
			m_linkCount++;
		}
	}
}

// pointeventdlg.cpp:779
void CCommoditySlot::Process(void) {
	for (int i = 0; i < m_linkCount; i++) {
		if (m_links[i] != NULL) {
			if (m_links[i]->GetSlotStatus() == SLOT_STATUS_COMPLETED) {
				SetSlotStatus(SLOT_STATUS_LOCKED);
			}
		}
	}
}

// pointeventdlg.cpp:801
void CCommoditySlot::DrawSlot(WPoint pos) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi == NULL) {
		return;
	}

	int space = gdi->SetSpace(0);

	gdi->SetTextStyle(1);

	switch (m_countDisplay) {
	case 1:
		gdi->Print(pos + m_myCountPos, 2, "%d", m_myCount);
		break;
	case 2:
		gdi->Print(pos + m_totalCountPos, 2, "%d", m_totalCount);
		break;
	case 3:
		gdi->Print(pos + m_myCountPos, 2, "%d", m_myCount);
		gdi->Print(pos + m_totalCountPos, 2, "%d", m_totalCount);
		break;
	}

	gdi->SetTextStyle(0);

	switch (m_slotStatus) {
	case SLOT_STATUS_CLOSED:
		if (m_openMark != NULL) {
			if (m_markFlags & 1) {
				gdi->DrawTexture(m_openMark, WRect(pos.x + m_markPos.x, pos.y + m_markPos.y, (float)m_openMark->Width(), (float)m_openMark->Height()), 0xffffffff, 0);
			}
		}
		break;
	case SLOT_STATUS_COMPLETED:
		if (m_completedMark != NULL) {
			if (m_markFlags & 2) {
				gdi->DrawTexture(m_completedMark, WRect(pos.x + m_markPos.x, pos.y + m_markPos.y, (float)m_completedMark->Width(), (float)m_completedMark->Height()), 0xffffffff, 0);
			}
		}
		break;
	}

	gdi->SetSpace(space);
}

// pointeventdlg.cpp:913 (run-time type and factory registration of FrPointEventNoticeDlg)
const WRTTI FrPointEventNoticeDlg::m_RTTI("FrPointEventNoticeDlg", &FrForm::m_RTTI);

IObject* FrPointEventNoticeDlgMakeInstance(void) {
	return new FrPointEventNoticeDlg;
}

struct __sFrPointEventNoticeDlg {
	__sFrPointEventNoticeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPointEventNoticeDlgMakeInstance, "FrPointEventNoticeDlg");
	}
};
static __sFrPointEventNoticeDlg s_registerFrPointEventNoticeDlg;

// pointeventdlg.cpp:915
const sFRESH_MSGMAP* FrPointEventNoticeDlg::GetMessageMap(void) const {
	return &FrPointEventNoticeDlg::_MsgMap;
}

sFRESH_MSGMAP FrPointEventNoticeDlg::_MsgMap = { &FrForm::_MsgMap, &FrPointEventNoticeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPointEventNoticeDlg::_MsgEntries[] = {
	// pointeventdlg.cpp:917
	{ "yes_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventNoticeDlg::OnInitBtnYes },
	// pointeventdlg.cpp:918
	{ "yes_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventNoticeDlg::OnLBDownBtnYes },
	// pointeventdlg.cpp:919
	{ "no_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventNoticeDlg::OnInitBtnNo },
	// pointeventdlg.cpp:920
	{ "no_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventNoticeDlg::OnLBDownBtnNo },
	// pointeventdlg.cpp:921
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventNoticeDlg::OnInitDrawArea },
	// pointeventdlg.cpp:922
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventNoticeDlg::OnOwnerDrawDrawArea },

	// pointeventdlg.cpp:924
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// pointeventdlg.cpp:927
FrPointEventNoticeDlg::FrPointEventNoticeDlg(void) {
	m_yesBtn = NULL;
	m_noBtn = NULL;
	m_drawArea = NULL;
	m_commodityIndex = 0;
	m_itemBitmap = NULL;
	m_noticeType = 0;
}

// pointeventdlg.cpp:937
FrPointEventNoticeDlg::~FrPointEventNoticeDlg(void) {
}

// pointeventdlg.cpp:941
void FrPointEventNoticeDlg::OnInitBtnYes(intptr_t param) {
	m_yesBtn = DYNAMIC_CAST(FrButton, param);

	m_yesBtn->SetPushDelay(0.0f);
}

// pointeventdlg.cpp:948
void FrPointEventNoticeDlg::OnLBDownBtnYes(void) {
	Close(FrOK, true);
}

// pointeventdlg.cpp:954
void FrPointEventNoticeDlg::OnInitBtnNo(intptr_t param) {
	m_noBtn = DYNAMIC_CAST(FrButton, param);

	m_noBtn->SetPushDelay(0.0f);
}

// pointeventdlg.cpp:961
void FrPointEventNoticeDlg::OnLBDownBtnNo(void) {
	Close(FrCANCEL, true);
}

// pointeventdlg.cpp:967
void FrPointEventNoticeDlg::SetCommodity(int index, unsigned long noticeType) {
	m_commodityIndex = index;

	if (index + 1 < 10) {
		m_itemBitmap = g_pFresh->GetBitmap(MakeStr("pointevent_item_0%d", index + 1));
	} else {
		m_itemBitmap = g_pFresh->GetBitmap(MakeStr("pointevent_item_%d", index + 1));
	}

	m_noticeType = noticeType;
}

// pointeventdlg.cpp:995
void FrPointEventNoticeDlg::OnInitDrawArea(intptr_t param) {
	m_drawArea = DYNAMIC_CAST(FrArea, param);
}

// pointeventdlg.cpp:1001
void FrPointEventNoticeDlg::OnOwnerDrawDrawArea(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi) {

		int space = gdi->SetSpace(0);

		WPoint pos(GetRect().x, GetRect().y);

		if (m_itemBitmap) {

			gdi->DrawTexture(m_itemBitmap, WRect(pos.x + 48.0f, pos.y + 41.0f, m_itemBitmap->Width(), m_itemBitmap->Height()), 0xffffffff, 0);
		}

		gdi->SetTextStyle(1);

		switch (m_noticeType) {
		case 1:
			// "...: do you want to apply for it?"
			gdi->Print(WPoint(pos.x + 69.0f, pos.y + 153.0f), 0, "\xC0\xBB \xC0\xC0\xB8\xF0\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?");
			break;
		case 0:
			// "...: do you want to receive it?"
			gdi->Print(WPoint(pos.x + 69.0f, pos.y + 153.0f), 0, "\xC0\xBB \xC1\xF6\xB1\xDE\xB9\xDE\xC0\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?");
			break;
		case 2:
			// "...: you receive it and donate 100 points."
			gdi->Print(WPoint(pos.x + 53.0f, pos.y + 153.0f), 0, "\xC0\xBB \xC1\xF6\xB1\xDE\xB9\xDE\xB0\xED 100\xC6\xF7\xC0\xCE\xC6\xAE \xB1\xE2\xBA\xCE\xC7\xD5\xB4\xCF\xB4\xD9.");
			break;
		}

		gdi->SetTextStyle(0);

		gdi->SetSpace(space);
	}
}

// pointeventdlg.cpp:1083
bool FrPointEventNoticeDlg::OnInit(void) {
	return true;
}

// pointeventdlg.cpp:1089 (run-time type and factory registration of FrPointEventDescDlg)
const WRTTI FrPointEventDescDlg::m_RTTI("FrPointEventDescDlg", &FrForm::m_RTTI);

IObject* FrPointEventDescDlgMakeInstance(void) {
	return new FrPointEventDescDlg;
}

struct __sFrPointEventDescDlg {
	__sFrPointEventDescDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPointEventDescDlgMakeInstance, "FrPointEventDescDlg");
	}
};
static __sFrPointEventDescDlg s_registerFrPointEventDescDlg;

// pointeventdlg.cpp:1091
const sFRESH_MSGMAP* FrPointEventDescDlg::GetMessageMap(void) const {
	return &FrPointEventDescDlg::_MsgMap;
}

sFRESH_MSGMAP FrPointEventDescDlg::_MsgMap = { &FrForm::_MsgMap, &FrPointEventDescDlg::_MsgEntries[0] };

sFRESH_ENTRY FrPointEventDescDlg::_MsgEntries[] = {
	// pointeventdlg.cpp:1093
	{ "desc_view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDescDlg::OnInitDescViewer },

	// pointeventdlg.cpp:1095
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// pointeventdlg.cpp:1098
FrPointEventDescDlg::FrPointEventDescDlg(void) {
	m_descViewer = NULL;
	memset(m_descImage, 0, sizeof(m_descImage));
}

// pointeventdlg.cpp:1104
FrPointEventDescDlg::~FrPointEventDescDlg(void) {
}

// pointeventdlg.cpp:1108
void FrPointEventDescDlg::SetDescImage(const char* fileName) {
	if (m_descViewer) {

		memset(m_descImage, 0, sizeof(m_descImage));
		strcpy(m_descImage, fileName);
		m_descViewer->ShowScrollBar(true);
		m_descViewer->Open(m_descImage);
	}
}

// pointeventdlg.cpp:1120
void FrPointEventDescDlg::OnInitDescViewer(intptr_t param) {
	m_descViewer = DYNAMIC_CAST(FrViewer, param);
}

// pointeventdlg.cpp:1127 (run-time type and factory registration of FrPointEventDlg)
const WRTTI FrPointEventDlg::m_RTTI("FrPointEventDlg", &FrForm::m_RTTI);

IObject* FrPointEventDlgMakeInstance(void) {
	return new FrPointEventDlg;
}

struct __sFrPointEventDlg {
	__sFrPointEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrPointEventDlgMakeInstance, "FrPointEventDlg");
	}
};
static __sFrPointEventDlg s_registerFrPointEventDlg;

// pointeventdlg.cpp:1130
const sFRESH_MSGMAP* FrPointEventDlg::GetMessageMap(void) const {
	return &FrPointEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrPointEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrPointEventDlg::_MsgEntries[0] };

// pointeventdlg.cpp:1132-1174
sFRESH_ENTRY FrPointEventDlg::_MsgEntries[] = {
	{ "desc_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitDescButton },
	{ "desc_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownDescButton },
	{ "cover_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnOwnerDrawCoverArea },
	{ "commodity_button01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity01 },
	{ "commodity_button01", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity01 },
	{ "commodity_button02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity02 },
	{ "commodity_button02", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity02 },
	{ "commodity_button03", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity03 },
	{ "commodity_button03", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity03 },
	{ "commodity_button04", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity04 },
	{ "commodity_button04", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity04 },
	{ "commodity_button05", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity05 },
	{ "commodity_button05", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity05 },
	{ "commodity_button06", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity06 },
	{ "commodity_button06", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity06 },
	{ "commodity_button07", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity07 },
	{ "commodity_button07", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity07 },
	{ "commodity_button08", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity08 },
	{ "commodity_button08", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity08 },
	{ "commodity_button09", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnCommodity09 },
	{ "commodity_button09", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnCommodity09 },
	{ "data_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnOwnerDrawDataArea },
	{ "contribution_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnOwnerDrawContribution },
	{ "contribution_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnDescContribution },
	{ "contribution_desc", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnDescContribution },
	{ "contribution_button", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPointEventDlg::OnInitBtnContribution },
	{ "contribution_button", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPointEventDlg::OnLBDownBtnContribution },

	// pointeventdlg.cpp:1176
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

FrPointEventDlg* g_pPointEventDlg;

// pointeventdlg.cpp:1179
FrPointEventDlg::FrPointEventDlg(void) {
	m_updateTimer = 0.0f;
	m_descButton = NULL;
	m_descDlg = NULL;
	m_noticeDlg = NULL;
	m_myPoint = 0;

	for (int i = 0; i < 10; i++) {
		m_commodityButtons[i] = NULL;
	}

	g_pPointEventDlg = this;

	m_contributionPoint = 0;
	m_contributionDescButton = NULL;
	m_contributionGauge = NULL;
	m_contributionGaugeBack = NULL;
	memset(m_contributionFrames, 0, sizeof(m_contributionFrames));
	m_successIcon = NULL;
}

// pointeventdlg.cpp:1202
FrPointEventDlg::~FrPointEventDlg(void) {
	g_pPointEventDlg = NULL;
}

// pointeventdlg.cpp:1208
void FrPointEventDlg::OnInitDescButton(intptr_t param) {
	if (m_descButton == NULL) {

		m_descButton = DYNAMIC_CAST(FrButton, param);
	}
}

// pointeventdlg.cpp:1217
void FrPointEventDlg::OnLBDownDescButton(void) {
	if (m_descDlg == NULL) {

		m_descDlg = CreateForm<FrPointEventDescDlg>(g_pFresh->GetManager(), this, "point_event_desc", NULL);
	}
	if (m_descDlg) {

		m_descDlg->SetDescImage("point_event_explanation.jpg");
		m_descDlg->SetIconRect(m_descButton->GetRect());
		m_descDlg->Open((FRESH_RESULT_CALLBACK)&FrPointEventDlg::OnPointEventDescDlgResult, 0x11);
	}
}

// pointeventdlg.cpp:1245
void FrPointEventDlg::OnOwnerDrawCoverArea(int param) {
	for (int i = 0; i < 10; i++) {

		m_slots[i].DrawCover();
	}
}

// pointeventdlg.cpp:1254
void FrPointEventDlg::OnInitBtnCommodity01(intptr_t param) {
	m_commodityButtons[0] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[0]->SetVisible(false);
}

// pointeventdlg.cpp:1261
void FrPointEventDlg::OnInitBtnCommodity02(intptr_t param) {
	m_commodityButtons[1] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[1]->SetVisible(false);
}

// pointeventdlg.cpp:1268
void FrPointEventDlg::OnInitBtnCommodity03(intptr_t param) {
	m_commodityButtons[2] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[2]->SetVisible(false);
}

// pointeventdlg.cpp:1275
void FrPointEventDlg::OnInitBtnCommodity04(intptr_t param) {
	m_commodityButtons[3] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[3]->SetVisible(false);
}

// pointeventdlg.cpp:1282
void FrPointEventDlg::OnInitBtnCommodity05(intptr_t param) {
	m_commodityButtons[4] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[4]->SetVisible(false);
}

// pointeventdlg.cpp:1289
void FrPointEventDlg::OnInitBtnCommodity06(intptr_t param) {
	m_commodityButtons[5] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[5]->SetVisible(false);
}

// pointeventdlg.cpp:1296
void FrPointEventDlg::OnInitBtnCommodity07(intptr_t param) {
	m_commodityButtons[6] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[6]->SetVisible(false);
}

// pointeventdlg.cpp:1303
void FrPointEventDlg::OnInitBtnCommodity08(intptr_t param) {
	m_commodityButtons[7] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[7]->SetVisible(false);
}

// pointeventdlg.cpp:1310
void FrPointEventDlg::OnInitBtnCommodity09(intptr_t param) {
	m_commodityButtons[8] = DYNAMIC_CAST(FrButton, param);

	m_commodityButtons[8]->SetVisible(false);
}

// pointeventdlg.cpp:1317
void FrPointEventDlg::OnLBDownBtnCommodity01(void) {
	Confirm(0);
}

// pointeventdlg.cpp:1318
void FrPointEventDlg::OnLBDownBtnCommodity02(void) {
	Confirm(1);
}

// pointeventdlg.cpp:1319
void FrPointEventDlg::OnLBDownBtnCommodity03(void) {
	Confirm(2);
}

// pointeventdlg.cpp:1320
void FrPointEventDlg::OnLBDownBtnCommodity04(void) {
	Confirm(3);
}

// pointeventdlg.cpp:1321
void FrPointEventDlg::OnLBDownBtnCommodity05(void) {
	Confirm(4);
}

// pointeventdlg.cpp:1322
void FrPointEventDlg::OnLBDownBtnCommodity06(void) {
	Confirm(5);
}

// pointeventdlg.cpp:1323
void FrPointEventDlg::OnLBDownBtnCommodity07(void) {
	Confirm(6);
}

// pointeventdlg.cpp:1324
void FrPointEventDlg::OnLBDownBtnCommodity08(void) {
	Confirm(7);
}

// pointeventdlg.cpp:1325
void FrPointEventDlg::OnLBDownBtnCommodity09(void) {
	Confirm(8);
}

// pointeventdlg.cpp:1329
void FrPointEventDlg::OnOwnerDrawDataArea(int param) {
	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (gdi == NULL) {
		return;
	}

	WPoint pos(GetRect().x, GetRect().y);

	int space = gdi->SetSpace(0);

	gdi->SetTextStyle(1);

	gdi->Print(WPoint(pos.x + 507.0f, pos.y + 73.0f), 2, "%d", GetMyPoint());

	gdi->SetTextStyle(0);

	gdi->SetSpace(space);

	for (int i = 0; i < 10; i++) {
		m_slots[i].DrawSlot(pos);
	}
}

// pointeventdlg.cpp:1365
bool FrPointEventDlg::OnPointEventDescDlgResult(int result, FrForm* pForm) {
	m_descDlg = NULL;
	return true;
}

// pointeventdlg.cpp:1371
bool FrPointEventDlg::OnPointEventNoticeDlgResult(int result, FrForm* pForm) {
	m_noticeDlg = NULL;

	if (result == 1) {

		WSendPacket packet((enumClientPacket)0x8a);
		packet.Encode4(m_selectedCommodity);
		packet.Send(SEND_TO_DEFAULT);
	} else {

		m_selectedCommodity = 10;
	}

	return true;
}

// pointeventdlg.cpp:1396
bool FrPointEventDlg::OnInit(void) {
	SendUpdateDataPacket();

	CCommoditySlot::sCommoditySlotData slotData[10] = {
		{
			0,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(166.0f, 285.0f), WPoint(166.0f, 297.0f), WPoint(130.0f, 252.0f),
			m_commodityButtons[0],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			1,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(330.0f, 285.0f), WPoint(330.0f, 297.0f), WPoint(295.0f, 252.0f),
			m_commodityButtons[1],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			2,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(494.0f, 285.0f), WPoint(494.0f, 297.0f), WPoint(459.0f, 252.0f),
			m_commodityButtons[2],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			3,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(166.0f, 391.0f), WPoint(166.0f, 403.0f), WPoint(130.0f, 358.0f),
			m_commodityButtons[3],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			4,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(330.0f, 391.0f), WPoint(330.0f, 403.0f), WPoint(295.0f, 358.0f),
			m_commodityButtons[4],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			5,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(494.0f, 391.0f), WPoint(494.0f, 403.0f), WPoint(459.0f, 358.0f),
			m_commodityButtons[5],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			6,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(166.0f, 496.0f), WPoint(166.0f, 508.0f), WPoint(130.0f, 463.0f),
			m_commodityButtons[6],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			7,
			3, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(330.0f, 496.0f), WPoint(330.0f, 508.0f), WPoint(295.0f, 463.0f),
			m_commodityButtons[7],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT2
		},
		{
			8,
			0, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(494.0f, 496.0f), WPoint(494.0f, 508.0f), WPoint(459.0f, 463.0f),
			m_commodityButtons[8],
			"btn_pointevent_white",
			CCommodityButton::COMMODITY_BUTTON_GIFT1
		},
		{
			9,
			0, 0, "pointevent_soldout_mark", "pointevent_gift_mark",
			WPoint(494.0f, 496.0f), WPoint(494.0f, 508.0f), WPoint(459.0f, 463.0f),
			m_commodityButtons[9],
			"contribution_btn_white",
			CCommodityButton::COMMODITY_BUTTON_PLAIN
		}
	};

	for (int i = 0; i < 10; i++) {
		m_slots[i].InitSlot(slotData[i]);
	}

	m_selectedCommodity = 10;

	if (IsLocalContent((localContentType_t)0x16)) {
		WRect rect = GetRect();
		rect.x -= 50.0f;
		SetRect(rect);

		m_contributionGauge = g_pFresh->GetBitmap("contribution_gage");
		m_contributionGaugeBack = g_pFresh->GetBitmap("contribution_gage_background");
		m_contributionFrames[0] = g_pFresh->GetBitmap("contribution_frame_01");
		m_contributionFrames[1] = g_pFresh->GetBitmap("contribution_frame_02");
		m_successIcon = g_pFresh->GetBitmap("icon_success");
	}

	return true;
}

// pointeventdlg.cpp:1630
void FrPointEventDlg::OnProc(const float dt) {
	m_updateTimer += dt;

	if (m_updateTimer > 10.0f) {
		SendUpdateDataPacket();
		m_updateTimer = 0.0f;
	}

	SetMyPoint(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().pointEventPoints);

	int count = 10;
	if (!IsLocalContent((localContentType_t)0x16)) {
		count = 9;
	}

	for (int i = 0; i < count; i++) {
		CCommodity commodity = g_CommodityTable.GetCommodity(i);

		m_slots[i].SetMyCount(g_CommodityStatusManager.GetMyCount(i));
		m_slots[i].SetTotalCount(g_CommodityStatusManager.GetTotalCount(i));

		bool bLocked = false;
		if (GetMyPoint() >= commodity.GetCommodityCost()) {
			m_slots[i].SetSlotStatus(CCommoditySlot::SLOT_STATUS_OPEN);
		} else {
			m_slots[i].SetSlotStatus(CCommoditySlot::SLOT_STATUS_LOCKED);
			bLocked = true;
		}

		switch (g_CommodityStatusManager.GetStatus(i)) {
		case 0:
			if (!bLocked) {
				m_slots[i].SetSlotStatus(CCommoditySlot::SLOT_STATUS_OPEN);
			}
			break;
		case 2:
			m_slots[i].SetSlotStatus(CCommoditySlot::SLOT_STATUS_CLOSED);
			break;
		case 1:
			m_slots[i].SetSlotStatus(CCommoditySlot::SLOT_STATUS_COMPLETED);
			break;
		}

		m_slots[i].Process();
	}
}

// pointeventdlg.cpp:1752
void FrPointEventDlg::Confirm(unsigned long index) {
	data_aba218 = m_selectedCommodity;

	if (m_selectedCommodity != 10) {

		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		pForm->SetTimeLimit(3.0f);
		// " Please try again in a moment. "
		pForm->SetMessage(" \xC0\xE1\xBD\xC3\xC8\xC4\xBF\xA1 \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5 \xC7\xD8\xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0. ", false);
		pForm->Open(0, 0);
		return;
	}

	m_selectedCommodity = (unsigned short)index;
	if (m_noticeDlg == NULL) {

		m_noticeDlg = CreateForm<FrPointEventNoticeDlg>(g_pFresh->GetManager(), this, "point_event_notice", NULL);
	}

	if (m_noticeDlg) {

		unsigned long noticeType = 0;
		switch (m_slots[index].GetButtonType()) {
		case CCommodityButton::COMMODITY_BUTTON_GIFT2:

			noticeType = 1;
			break;
		case CCommodityButton::COMMODITY_BUTTON_PLAIN:

			noticeType = 2;
			break;
		case CCommodityButton::COMMODITY_BUTTON_GIFT1:
		case CCommodityButton::COMMODITY_BUTTON_NONE:

			noticeType = 0;
			break;
		}

		m_noticeDlg->SetCommodity(m_selectedCommodity, noticeType);
		m_noticeDlg->SetIconRect(m_slots[m_selectedCommodity].GetButtonRect());
		m_noticeDlg->Open((FRESH_RESULT_CALLBACK)&FrPointEventDlg::OnPointEventNoticeDlgResult, 0x11);
	}
}

// pointeventdlg.cpp:1863
void FrPointEventDlg::ResultMessage(unsigned long result) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
	WRect rect = pForm->GetRect();
	pForm->SetRect(WRect(rect));
	pForm->SetTimeLimit(12.0f);

	if (m_selectedCommodity < 10) {

		switch (result) {
		case 0:

			switch (m_slots[m_selectedCommodity].GetButtonType()) {
			case CCommodityButton::COMMODITY_BUTTON_GIFT1: {
				// "The prize was received."
				pForm->SetMessage("\xBC\xBA\xB0\xF8\xC0\xFB\xC0\xB8\xB7\xCE \xBB\xF3\xC7\xB0\xC0\xBB \xC1\xF6\xB1\xDE \xB9\xDE\xBE\xD2\xBD\xC0\xB4\xCF\xB4\xD9.", false);

				WSendPacket packet(0x21);

				packet.EncodeStr(std::string(CSharedDoc::Instance()->GetPangyaUserInfo().nickname));
				packet.Encode4(CSharedDoc::Instance()->m_myUID);

				packet.Send((eSendTo)1);
				break;
			}
			case CCommodityButton::COMMODITY_BUTTON_GIFT2:
				// "Applied for the prize."
				pForm->SetMessage("\xBC\xBA\xB0\xF8\xC0\xFB\xC0\xB8\xB7\xCE \xBB\xF3\xC7\xB0\xC0\xBB \xC0\xC0\xB8\xF0 \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
				break;
			case CCommodityButton::COMMODITY_BUTTON_PLAIN: {
				// "Donated."
				pForm->SetMessage("\xBC\xBA\xB0\xF8\xC0\xFB\xC0\xB8\xB7\xCE \xB1\xE2\xBA\xCE \xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);

				WSendPacket packet(0x21);

				packet.EncodeStr(std::string(CSharedDoc::Instance()->GetPangyaUserInfo().nickname));
				packet.Encode4(CSharedDoc::Instance()->m_myUID);

				packet.Send((eSendTo)1);
				break;
			}
			}

			LocalDataUpdate(m_selectedCommodity);
			break;
		case 1:
		case 4:

			switch (m_slots[m_selectedCommodity].GetButtonType()) {
			case CCommodityButton::COMMODITY_BUTTON_GIFT2:
				// "Not enough points."
				pForm->SetMessage("\xC6\xF7\xC0\xCE\xC6\xAE\xB0\xA1 \xBA\xCE\xC1\xB7\xC7\xD5\xB4\xCF\xB4\xD9.", false);
				break;
			case CCommodityButton::COMMODITY_BUTTON_GIFT1:
			case CCommodityButton::COMMODITY_BUTTON_PLAIN:
				// "Not enough points."
				pForm->SetMessage("\xC6\xF7\xC0\xCE\xC6\xAE\xB0\xA1 \xBA\xCE\xC1\xB7\xC7\xD5\xB4\xCF\xB4\xD9.", false);
				break;
			}
			break;
		case 2:
		case 3:

			switch (m_slots[m_selectedCommodity].GetButtonType()) {
			case CCommodityButton::COMMODITY_BUTTON_GIFT1:
				// "This prize was already received."
				pForm->SetMessage("\xC0\xCC\xB9\xCC \xC1\xF6\xB1\xDE\xB9\xDE\xC0\xBA \xBB\xF3\xC7\xB0\xC0\xD4\xB4\xCF\xB4\xD9.", false);
				break;
			case CCommodityButton::COMMODITY_BUTTON_GIFT2:
				// "This prize was already applied for."
				pForm->SetMessage("\xC0\xCC\xB9\xCC \xC0\xC0\xB8\xF0\xC7\xD1 \xBB\xF3\xC7\xB0\xC0\xD4\xB4\xCF\xB4\xD9.", false);
				break;
			case CCommodityButton::COMMODITY_BUTTON_PLAIN:
				// "Cannot donate. Please try again in a moment"
				pForm->SetMessage("\xB1\xE2\xBA\xCE \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \xC0\xE1\xBD\xC3 \xC8\xC4 \xB4\xD9\xBD\xC3 \xBD\xC3\xB5\xB5 \xC7\xD8 \xC1\xD6\xBD\xCA\xBD\xC3\xBF\xC0", false);
				break;
			}
			break;
		}

		pForm->Open(0, 0);
		m_selectedCommodity = 10;
	}
}

// pointeventdlg.cpp:2096
void FrPointEventDlg::UnSelectCommodity(void) {
	m_selectedCommodity = 10;
}

// pointeventdlg.cpp:2102
void FrPointEventDlg::SendUpdateDataPacket(void) {
	WSendPacket packet((enumClientPacket)0x8b);
	packet.Send(SEND_TO_DEFAULT);
}

// pointeventdlg.cpp:2109
void FrPointEventDlg::LocalDataUpdate(unsigned short index) {
	unsigned long myCount = g_CommodityStatusManager.GetMyCount(index);
	unsigned long totalCount = g_CommodityStatusManager.GetTotalCount(index);

	switch (m_slots[index].GetButtonType()) {
	case CCommodityButton::COMMODITY_BUTTON_GIFT1:

		myCount++;
		if (totalCount > 0) {

			totalCount--;
		}

		g_CommodityStatusManager.SetMyCount(index, myCount);
		g_CommodityStatusManager.SetTotalCount(index, totalCount);
		break;
	case CCommodityButton::COMMODITY_BUTTON_GIFT2:

		myCount++;

		g_CommodityStatusManager.SetMyCount(index, myCount);
		g_CommodityStatusManager.SetTotalCount(index, totalCount + 1);
		break;
	case CCommodityButton::COMMODITY_BUTTON_PLAIN:

		myCount += 1;
		totalCount += 100;

		g_CommodityStatusManager.SetMyCount(index, myCount);
		g_CommodityStatusManager.SetTotalCount(index, totalCount);
		break;
	}

	CCommodity commodity = g_CommodityTable.GetCommodity(index);

	m_myPoint -= commodity.GetCommodityCost();
}

// pointeventdlg.cpp:2161
void FrPointEventDlg::OnOwnerDrawContribution(int param) {
	if (IsLocalContent((localContentType_t)0x16)) {

		FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

		if (gdi != NULL) {

			int space = gdi->SetSpace(0);

			// Gauge height (0..146, four stages of 36.5) and the stage
			// whose success icon is shown.
			float gaugeHeight;
			unsigned long stage = 0;
			WRect rect = GetRect();

			m_contributionPoint = (float)g_CommodityStatusManager.GetTotalCount(9);
			unsigned long point = (unsigned long)(m_contributionPoint * 0.01f);

			if (point < 30000) {

				gaugeHeight = (float)point * (36.5f / 30000.0f);
			} else if (point < 70000) {

				point -= 30000;

				gaugeHeight = (float)point * (36.5f / 70000.0f) + 36.5f;
				if (point < 100) {

					stage = 2;
				}
			} else if (point < 120000) {

				point -= 70000;

				gaugeHeight = (float)point * (36.5f / 120000.0f) + 73.0f;
				if (point < 100) {

					stage = 3;
				}
			} else if (point < 200000) {

				point -= 120000;

				gaugeHeight = (float)point * (36.5f / 80000.0f) + 109.5f;
				if (point < 100) {

					stage = 4;
				}
			} else {

				gaugeHeight = 146.0f;
				stage = 5;
			}

			gdi->DrawTexture(m_contributionGaugeBack, WRect(rect.x + 604.0f, rect.y + 372.0f - 146.0f, (float)m_contributionGauge->Width(), 146.0f), 0xffffffff, 0);

			gdi->DrawTexture(m_contributionGauge, WRect(rect.x + 604.0f, rect.y + 373.0f - gaugeHeight, (float)m_contributionGauge->Width(), gaugeHeight + 1.0f), 0xffffffff, 0);

			gdi->DrawTexture(m_contributionFrames[0], WRect(rect.x + 522.0f, rect.y + 130.0f, (float)m_contributionFrames[0]->Width(), (float)m_contributionFrames[0]->Height()), 0xffffffff, 0);
			gdi->DrawTexture(m_contributionFrames[1], WRect(rect.x + 522.0f, rect.y + 386.0f, (float)m_contributionFrames[1]->Width(), (float)m_contributionFrames[1]->Height()), 0xffffffff, 0);

			if (stage != 0) {

				gdi->DrawTexture(m_successIcon, WRect(rect.x + 617.0f, rect.y + 366.0f - (float)stage * 36.5f, (float)m_successIcon->Width(), (float)m_successIcon->Height()), 0xffffffff, 0);
			}

			gdi->SetSpace(space);
		}
	}
}

// pointeventdlg.cpp:2257
void FrPointEventDlg::OnInitBtnDescContribution(intptr_t param) {
	m_contributionDescButton = DYNAMIC_CAST(FrButton, param);

	if (!IsLocalContent((localContentType_t)0x16)) {
		m_contributionDescButton->SetVisible(false);
		m_contributionDescButton->Enable(false);
	} else {

		m_contributionDescButton->SetPushDelay(0.0f);
	}
}

// pointeventdlg.cpp:2273
void FrPointEventDlg::OnLBDownBtnDescContribution(void) {
	if (IsLocalContent((localContentType_t)0x16)) {

		if (m_descDlg == NULL) {

			m_descDlg = CreateForm<FrPointEventDescDlg>(g_pFresh->GetManager(), this, "point_event_desc", NULL);
		}
		if (m_descDlg) {

			m_descDlg->SetDescImage("point_event_explanation_02.jpg");
			m_descDlg->SetIconRect(m_contributionDescButton->GetRect());
			m_descDlg->Open((FRESH_RESULT_CALLBACK)&FrPointEventDlg::OnPointEventDescDlgResult, 0x11);
		}
	}
}

// pointeventdlg.cpp:2291
void FrPointEventDlg::OnInitBtnContribution(intptr_t param) {
	m_commodityButtons[9] = DYNAMIC_CAST(FrButton, param);

	if (!IsLocalContent((localContentType_t)0x16)) {
		m_commodityButtons[9]->SetVisible(false);
	} else {

		m_commodityButtons[0]->SetVisible(false);
		m_commodityButtons[9]->SetPushDelay(0.0f);
	}
}

// pointeventdlg.cpp:2307
void FrPointEventDlg::OnLBDownBtnContribution(void) {
	if (IsLocalContent((localContentType_t)0x16)) {

		Confirm(9);
	}
}
