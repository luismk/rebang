// Original: d:\build\custom\temp\code\client\projectg\privatetradedlg.cpp
#include "PrivateTradeDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "frdesktop.h"
#include "fredit.h"
#include "fresh.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "wtypes.h"
#include "InputManager.h"
#include "SharedDoc.h"
#include "classdefine.h"
#include "TaskManager.h"
#include "actor.h"
#include "UccManager.h"
#include "globalgamedefine.h"
#include "bitmap.h"
#include "commonutil.h"
#include "wtemplate.h"
#include "wfont.h"
#include "woverlay.h"

#include <stdlib.h>
#include <string.h>

#include <algorithm>

// A warehouse item shown in a list row (FrListItem::pData). Only the members
// the unit's functions so far use are named.
struct sWarpItem {
	sWarpItem(const sWarpItem* pItem);
	sWarpItem(const sItemInfo& item);
	sWarpItem(const sCards& card);
	void SetIcon(unsigned long typeId);

	unsigned long typeId;     // 0x0 (FindListItem compares it)
	unsigned long guid;       // 0x4 (less_guid_ptr, _FindByGuid_Ptr)
	const Bitmap* pIcon;      // 0x8 (SetIcon)
	std::string name;         // 0xc (SetIcon)
	WRect iconRect;           // 0x28 (SetIcon: the icon's size)
	int count;                // 0x38 (Add/SubListItemCount)
};
CHECK_FIELD(sWarpItem, typeId, 0x0);
CHECK_FIELD(sWarpItem, guid, 0x4);
CHECK_FIELD(sWarpItem, pIcon, 0x8);
CHECK_FIELD(sWarpItem, name, 0xc);
CHECK_FIELD(sWarpItem, iconRect, 0x28);
CHECK_FIELD(sWarpItem, count, 0x38);
CHECK_SIZE(sWarpItem, 0x3c);

// privatetradedlg.cpp:28
sWarpItem::sWarpItem(const sWarpItem* pItem)
	: typeId(pItem->typeId), guid(pItem->guid), pIcon(pItem->pIcon), name(pItem->name),
	  iconRect(pItem->iconRect), count(pItem->count) {
}

// privatetradedlg.cpp:33
sWarpItem::sWarpItem(const sItemInfo& item)
	: typeId(item.m_typeId), guid(item.m_serial), pIcon(0) {
	if (ItemManager()->IsDisplayItemNumber(typeId)) {
		count = item.m_count;
	} else {
		count = 1;
	}
	SetIcon(typeId);
}

// privatetradedlg.cpp:44
sWarpItem::sWarpItem(const sCards& card)
	: typeId(card.m_typeId), guid(card.m_guid), pIcon(0) {
	count = card.m_count; SetIcon(typeId);
}

// privatetradedlg.cpp:49
void sWarpItem::SetIcon(unsigned long typeId) {
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(typeId);
	if (pCommon != 0) {
		pIcon = g_pFresh->GetBitmap(pCommon->m_icon);
		name = pCommon->m_name;

		if (pIcon == 0) {
			pIcon = g_pFresh->GetBitmap("caddie_no");
			name = MakeStr("INVAID(%d)", typeId);
		}
	}

	if (pIcon != 0) {
		iconRect.x = iconRect.y = 0.0f;
		iconRect.w = (float)pIcon->Width(); iconRect.h = (float)pIcon->Height();
	}
}

// wtemplate.h:409-414 and :460-465 (not yet in src/client/ProjectG/wtemplate.h,
// which this unit does not own): list finders by the element's first member.
struct _FindByGuid {
	unsigned long m_guid;

	_FindByGuid(unsigned long guid) : m_guid(guid) {}
	template <class T>
	bool operator()(const T& item) const { return m_guid == item.m_serial; }
};

struct _FindByUid {
	unsigned long m_uid;

	_FindByUid(unsigned long uid) : m_uid(uid) {}
	template <class T>
	bool operator()(const T& item) const { return m_uid == item.m_guid; }
};

namespace {

void Formator(const sTradeDropItem& item, _sPrivateTradeItemSingle& single);
unsigned long GetMyItemCount(unsigned long itemId, unsigned long typeId);
void SetListParts(std::vector<sWarpItem*>& items);
void SetListClubSet(std::vector<sWarpItem*>& items);
void SetListCard(std::vector<sWarpItem*>& items);
void DoSortItem(std::vector<sWarpItem*>& items, eSetItemButton button, int sortType);
void GetAblilityText(std::vector<std::string>& lines, short* pStats, short* pSlotStats);
void DrawPartsCardSlot(_sPrivateTradeItemSingle* pItem, const WRect& rect, float alpha, int bSmall);

// privatetradedlg.cpp:78
template <class T>
T* FindPrivateTradeChildForm(const char* name) {
	FrWnd* pTrade = g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
	if (pTrade != 0) {
		FrWnd* pChild = pTrade->FindChildForm(name);
		if (pChild != 0) {
			return DYNAMIC_CAST(T, pChild);
		}
	}
	return 0;
}

// privatetradedlg.cpp:98
// The period cards put on the part (typeId, id); false when there are none
// or the item is not a part.
int GetCardsAttachedToPart(std::vector<sSCardAvilityPeriodInfo*>& cards, unsigned long typeId, unsigned long id) {
	if ((typeId & 0xfc000000) != 0x8000000) {
		return 0;
	}

	std::list<sSCardAvilityPeriodInfo>& cardList = WSingleton<CSharedDoc>::Instance()->m_cardPeriodList[0];
	for (std::list<sSCardAvilityPeriodInfo>::iterator it = cardList.begin(); it != cardList.end(); ++it) {
		if (typeId == it->partsTypeId && id == it->partsId) {
			cards.push_back(&*it);
		}
	}

	return !cards.empty();
}

// privatetradedlg.cpp:118
// How many of the item (itemId, typeId) the player holds; 1 for an item
// that is not counted.
unsigned long GetMyItemCount(unsigned long itemId, unsigned long typeId) {
	if (ItemManager()->IsDisplayItemNumber(typeId)) {
		switch (typeId >> 26) {
		case 6: {
			CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
			std::list<sItemInfo>::iterator it = std::find_if(pDoc->m_itemInfoList.begin(), pDoc->m_itemInfoList.end(), _FindByGuid(itemId));
			if (it != pDoc->m_itemInfoList.end()) {
				return it->m_count;
			}
			break;
		}
		case 0x1f: {
			CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
			std::list<sCards>::iterator it = std::find_if(pDoc->m_myCardList.begin(), pDoc->m_myCardList.end(), _FindByUid(itemId));
			if (it != pDoc->m_myCardList.end()) {
				return it->m_count;
			}
			break;
		}
		case 0x1c: {
			CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
			std::list<sItemInfo>::iterator it = std::find_if(pDoc->m_myExtraItemList3.begin(), pDoc->m_myExtraItemList3.end(), _FindByGuid(itemId));
			if (it != pDoc->m_myExtraItemList3.end()) {
				return it->m_count;
			}
			break;
		}
		}
	}

	return 1;
}

// privatetradedlg.cpp:151
// The card's name and the first line of its description.
std::string GetCardInfoText(unsigned long typeId) {
	std::string text;
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(typeId);
	if (pCommon == 0) {
		text += "unknown card";
	} else {
		text += pCommon->m_name;
	}

	IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(typeId);
	if (pDesc != 0) {
		text += " : ";
		std::string desc(pDesc->m_text);
		std::string::size_type pos = desc.find('\n');
		if (pos != std::string::npos) {
			text += std::string(desc.begin(), desc.begin() + pos);
		} else {
			text += desc;
		}
	}

	return text;
}

// privatetradedlg.cpp:292
void Formator(const sTradeDropItem& item, _sPrivateTradeItemSingle& single) {
	single.typeId = item.typeId;
	single.itemId = item.itemId;
	single.count = item.count;

	switch (single.typeId >> 26) {
	case 2: {
		std::vector<sSCardAvilityPeriodInfo*> cards;
		if (GetCardsAttachedToPart(cards, single.typeId, single.itemId)) {
			int i = 0;
			for (std::vector<sSCardAvilityPeriodInfo*>::iterator it = cards.begin(); it != cards.end(); ++it) {
				if (i >= 8) {
					break;
				}

				sSCardAvilityPeriodInfo* pCard = *it;
				single.cards[i].id = pCard->id;
				single.cards[i].typeId = pCard->typeId;
				single.cards[i].effect = pCard->effect;
				single.cards[i].effectValue = pCard->effectValue;
				single.cards[i].kind = pCard->kind;
				single.cards[i].slot = pCard->slot;
				i++;
			}
		}
		break;
	}
	case 4: {
		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
		std::map<unsigned int, sItemInfo>::iterator it = pDoc->m_myItemMap.find(single.itemId);
		if (it != pDoc->m_myItemMap.end()) {
			for (int i = 0; i < 5; i++) single.slotStats[i] = it->second.m_clubStatLevel[i];
		}
		break;
	}
	}
}

// privatetradedlg.cpp:504
bool IsSalable(const sItemInfo& item) {
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(item.m_typeId);
	if (pCommon != 0) {
		return (pCommon->m_flags0 & 0x1e) != 0;
	}

	return false;
}

// privatetradedlg.cpp:559
void SetListAztec(std::vector<sWarpItem*>& items) {
	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	std::list<sItemInfo>* pGearList = &pDoc->m_myGearList;
	unsigned long ballId = pDoc->m_myBallId;

	items.reserve(pGearList->size());

	for (std::list<sItemInfo>::iterator it = pGearList->begin(); it != pGearList->end(); ++it) {
		if (it->m_typeId != ballId) {
			if (IsSalable(*it)) {
				sWarpItem* pItem = new sWarpItem(*it);
				items.push_back(pItem);
			}
		}
	}
}

// privatetradedlg.cpp:602
void SetListHItem(std::vector<sWarpItem*>& items) {
	std::list<sItemInfo>& itemList = WSingleton<CSharedDoc>::Instance()->m_itemInfoList;
	for (std::list<sItemInfo>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
		if ((it->m_typeId & 0x2000000) != 0) {
			if (IsSalable(*it)) {
				sWarpItem* pItem = new sWarpItem(*it);
				items.push_back(pItem);
			}
		}
	}
}

// privatetradedlg.cpp:616
void SetListUitem(std::vector<sWarpItem*>& items) {
	std::list<sItemInfo>& itemList = WSingleton<CSharedDoc>::Instance()->m_itemInfoList;
	for (std::list<sItemInfo>::iterator it = itemList.begin(); it != itemList.end(); ++it) {
		if ((it->m_typeId & 0x2000000) == 0) {
			if (IsSalable(*it)) {
				sWarpItem* pItem = new sWarpItem(*it);
				items.push_back(pItem);
			}
		}
	}
}

// privatetradedlg.cpp:635
void SetListCard(std::vector<sWarpItem*>& items) {
	std::list<sCards>& cardList = WSingleton<CSharedDoc>::Instance()->m_myCardList;

	items.reserve(cardList.size());

	for (std::list<sCards>::iterator it = cardList.begin(); it != cardList.end(); ++it) {
		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(it->m_typeId);
		if (pCommon != 0 && (pCommon->m_flags0 & 0x1e) != 0) {
			sWarpItem* pItem = new sWarpItem(*it);
			items.push_back(pItem);
		}
	}
}

// privatetradedlg.cpp:790
template <class T>
void GetIconNameDesc(const Bitmap*& pIcon, std::string& name, std::string& desc, const T* pItem) {
	if (pItem != 0) {
		pIcon = g_pFresh->GetBitmap(pItem->m_icon);
		name = pItem->m_name;
		IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(pItem->m_typeId);
		if (pDesc != 0) {
			desc = pDesc->m_text;
		}
	}
}

// privatetradedlg.cpp:803
// One line per stat (power, control, accuracy, spin, curve): the base value
// and the slot upgrades.
void GetAblilityText(std::vector<std::string>& lines, short* pStats, short* pSlotStats) {
	const char* names[5] = { "\xc6\xc4\xbf\xf6", "\xc4\xc1\xc6\xae\xb7\xd1", "\xc1\xa4\xc8\xae\xb5\xb5", "\xbd\xba\xc7\xc9", "\xc4\xbf\xba\xea" };

	for (int i = 0; i < 5; i++) {
		std::string text;
		if (pStats[i] != 0) {
			text = MakeStr("%s +%d", names[i], pStats[i]);
		}

		if (pSlotStats[i] != 0) {
			if (text.empty()) {
				text = MakeStr("%s \xbd\xbd\xb7\xd4 +%d", names[i], pSlotStats[i]);
			} else {
				text += MakeStr(" / %s \xbd\xbd\xb7\xd4 +%d", names[i], pSlotStats[i]);
			}
		}

		if (!text.empty()) {
			lines.push_back(text);
		}
	}
}

}

CTradeDragDrop* CTradeDragDrop::m_pCaller;
sTradeDropItem* CTradeDragDrop::m_pItem;

// privatetradedlg.cpp:833
CTradeDragDrop::CTradeDragDrop(void) {
	if (m_pItem == 0) {
		m_pItem = new sTradeDropItem;
	}
}

// privatetradedlg.cpp:839
CTradeDragDrop::~CTradeDragDrop(void) {
	if (m_pItem != 0) {
		delete m_pItem;
		m_pItem = 0;
	}
}

// privatetradedlg.cpp:848
const sTradeDropItem* CTradeDragDrop::GetDropItem(void) {
	if (m_pCaller != 0 && m_pItem->typeId != 0 && m_pItem->itemId != 0) {
		return m_pItem;
	}
	return 0;
}

// privatetradedlg.cpp:855
bool CTradeDragDrop::IsPoped(void) {
	return m_pCaller != 0 && m_pItem->typeId != 0 && m_pItem->itemId != 0;
}

// privatetradedlg.cpp:860
void CTradeDragDrop::Cancel(void) {
	m_pCaller = 0;
}

// privatetradedlg.cpp:865
bool CTradeDragDrop::Pop(void) {
	m_pCaller = this;
	return true;
}

// privatetradedlg.cpp:871
void CTradeDragDrop::SetPopItem(const sTradeDropItem& item) {
	*m_pItem = item;
}

// privatetradedlg.cpp:876
void CTradeDragDrop::Drop(CTradeDragDrop* pTarget) {
	if (m_pCaller != 0 && pTarget != 0) {
		if (m_pCaller != pTarget) {
			if (pTarget->DropItemDown()) {
				m_pCaller->DropItemMoved();
			}
		}
	}
	m_pCaller = 0;
	memset(m_pItem, 0, sizeof(sTradeDropItem));
}

// privatetradedlg.cpp:895 (run-time type and factory registration of FrNumEdit)
const WRTTI FrNumEdit::m_RTTI("FrNumEdit", &FrForm::m_RTTI);

IObject* FrNumEditMakeInstance(void) {
	return new FrNumEdit;
}

struct __sFrNumEdit {
	__sFrNumEdit(void) {
		ObjectFactory().AddObjectFunctor(FrNumEditMakeInstance, "FrNumEdit");
	}
};
static __sFrNumEdit s_registerFrNumEdit;

// privatetradedlg.cpp:897
const sFRESH_MSGMAP* FrNumEdit::GetMessageMap(void) const {
	return &FrNumEdit::_MsgMap;
}

sFRESH_MSGMAP FrNumEdit::_MsgMap = { &FrForm::_MsgMap, &FrNumEdit::_MsgEntries[0] };

sFRESH_ENTRY FrNumEdit::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNumEdit::OnViewDraw },
	{ "NumEdit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNumEdit::OnEditInit },
	{ "NumEdit", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrNumEdit::OnEditEnter },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:906
FrNumEdit::FrNumEdit(void)
	: m_pEdit(0), m_value(0), m_pReturnParam(0), m_minValue(0), m_maxValue(0x7fffffff) {
}

// privatetradedlg.cpp:911
FrNumEdit::~FrNumEdit(void) {
}

// privatetradedlg.cpp:916
void FrNumEdit::SetValue(unsigned long value) {
	if (m_value >= m_minValue && m_value <= m_maxValue) {
		m_value = value;
	} else {
		m_value = m_minValue;
	}

	char buf[32] = { 0 };
	m_pEdit->SetLine(1, _itoa(m_value, buf, 10), 0, false, 0);
}

// privatetradedlg.cpp:925
void FrNumEdit::SetEditRange(unsigned long minValue, unsigned long maxValue) {
	char buf[32] = { 0 };
	_itoa(maxValue, buf, 10);
	m_pEdit->SetCharLimit(strlen(buf) + 1, false);

	m_minValue = minValue;
	m_maxValue = maxValue;
}

// privatetradedlg.cpp:936
void FrNumEdit::OnProc(const float dt) {
	if (m_pEdit->m_nFlags.GetFlag(0x10)) {
		unsigned long value = 0;
		const char* text = m_pEdit->GetLine(1, false);
		std::string digits;
		for (unsigned int i = 0; i < strlen(text); i++) {
			char c = text[i];
			if (c < 0) {
				continue;
			}

			for (unsigned int j = 0; j < strlen("0123456789"); j++) {
				if (c == "0123456789"[j]) {
					digits += c;
					break;
				}
			}
		}

		if (digits.length() > 0) {
			value = atoi(digits.c_str());
			if (value < m_minValue) {
				value = m_minValue;
			}
			if (value > m_maxValue) {
				value = m_maxValue;
			}
		}

		char buf[32] = { 0 };
		_itoa(value, buf, 10);
		m_pEdit->ForcedEdit(buf, 0);
	}
}

// privatetradedlg.cpp:979
void FrNumEdit::OnViewDraw(void) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();

	const Bitmap* pBitmap = g_pFresh->GetBitmap("cfrm00");
	WRect src(0.0f, 0.0f, pBitmap->Width(), pBitmap->Height());
	pDevice->DrawTexture(pBitmap, src, m_rect, 0xffffffff, 0);
}

// privatetradedlg.cpp:990
void FrNumEdit::OnEditInit(intptr_t param) {
	m_pEdit = DYNAMIC_CAST(FrEdit, param);

	char buf[32] = { 0 }; _itoa(0x7fffffff, buf, 10);
	m_pEdit->SetCharLimit(strlen(buf) + 1, false);

	m_pEdit->SetWidthLimit(m_pEdit->GetRect().w);
	if (m_value != 0) {
		char text[32] = { 0 };
		m_pEdit->SetLine(1, _itoa(m_value, text, 10), 0, false, 0);
	}
}

// privatetradedlg.cpp:1008
bool FrNumEdit::OnEditEnter(intptr_t param) {
	const char* text = (const char*)param;
	unsigned long value = 0;
	if (text != 0 && strlen(text) != 0) {
		value = atoi(text);
	}

	if (m_value >= m_minValue && m_value <= m_maxValue) {
		m_value = value;
	} else {
		m_value = m_minValue;
	}

	return FrForm::Close(FrOK, true);
}

// privatetradedlg.cpp:1021 (run-time type and factory registration of FrPangEdit)
const WRTTI FrPangEdit::m_RTTI("FrPangEdit", &FrForm::m_RTTI);

IObject* FrPangEditMakeInstance(void) {
	return new FrPangEdit;
}

struct __sFrPangEdit {
	__sFrPangEdit(void) {
		ObjectFactory().AddObjectFunctor(FrPangEditMakeInstance, "FrPangEdit");
	}
};
static __sFrPangEdit s_registerFrPangEdit;

// privatetradedlg.cpp:1023
const sFRESH_MSGMAP* FrPangEdit::GetMessageMap(void) const {
	return &FrPangEdit::_MsgMap;
}

sFRESH_MSGMAP FrPangEdit::_MsgMap = { &FrForm::_MsgMap, &FrPangEdit::_MsgEntries[0] };

sFRESH_ENTRY FrPangEdit::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrPangEdit::OnViewDraw },
	{ "EditPang", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPangEdit::OnEditInit },
	{ "EditPang", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrPangEdit::OnEditEnter },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:1032
FrPangEdit::FrPangEdit(void)
	: m_pEdit(0), m_pangValue(0), m_texRate(0) {
}

// privatetradedlg.cpp:1037
FrPangEdit::~FrPangEdit(void) {
}

// privatetradedlg.cpp:1042
void FrPangEdit::SetPangValue(__int64 value) {
	m_pangValue = value;
	if (value != 0) {
		char buf[32] = { 0 };
		m_pEdit->SetLine(1, _i64toa(value, buf, 10), 0, false, 0);
	}
}

// privatetradedlg.cpp:1052
void FrPangEdit::OnProc(const float dt) {
	if (m_pEdit->m_nFlags.GetFlag(0x10)) {
		const char* text = m_pEdit->GetLine(1, false);
		std::string digits;
		bool bFiltered = false;
		for (unsigned int i = 0; i < strlen(text); i++) {
			char c = text[i];
			if (c < 0) {
				continue;
			}

			unsigned int j;
			for (j = 0; j < strlen("0123456789"); j++) {
				if (c == "0123456789"[j]) {
					digits += c;
					break;
				}
			}

			if (j == strlen("0123456789")) {
				bFiltered = true;
			}
		}

		if (bFiltered) {
			m_pEdit->ForcedEdit(digits.c_str(), 0);
		}

		if (digits.length() > 0) {
			__int64 value = _atoi64(digits.c_str());
			if (value > 0) {
				__int64 tex = m_texRate * value / 100;

				CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
				if (pDoc->m_myPang <= value + tex) {
					// The most pang the player can put on with the tax.
					value = pDoc->m_myPang * 100 / (m_texRate + 100);

					char buf[32] = { 0 };
					_i64toa(value, buf, 10);
					m_pEdit->ForcedEdit(buf, 0);
				}
			}
		}
	}
}

// privatetradedlg.cpp:1114
void FrPangEdit::OnViewDraw(void) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();

	const Bitmap* pBitmap = g_pFresh->GetBitmap("cfrm00");
	WRect src(0.0f, 0.0f, pBitmap->Width(), pBitmap->Height());
	pDevice->DrawTexture(pBitmap, src, m_rect, 0xffffffff, 0);

	const Bitmap* pPang = g_pFresh->GetBitmap("pang_bar_pang");

	if (pPang != 0) {
		pDevice->DrawTexture(pPang, WRect(m_rect.x + 10.0f, m_rect.y + 5.0f, pPang->Width(), pPang->Height()), 0xffffffff, 0);
	}
}

// privatetradedlg.cpp:1130
void FrPangEdit::OnEditInit(intptr_t param) {
	m_pEdit = DYNAMIC_CAST(FrEdit, param);

	char buf[32] = { 0 };
	_i64toa(WSingleton<CSharedDoc>::Instance()->m_myPang, buf, 10);

	m_pEdit->SetCharLimit(strlen(buf) + 1, false);
	m_pEdit->SetWidthLimit(m_pEdit->GetRect().w);

	if (m_pangValue != 0) {
		char text[32] = { 0 };
		m_pEdit->SetLine(1, _i64toa(m_pangValue, text, 10), 0, false, 0);
	}
}

// privatetradedlg.cpp:1150
bool FrPangEdit::OnEditEnter(intptr_t param) {
	const char* text = (const char*)param;
	if (text != 0 && strlen(text) != 0) {
		m_pangValue = _atoi64(text);
	} else {
		m_pangValue = 0;
	}

	return FrForm::Close(FrOK, true);
}

// privatetradedlg.cpp:1165
EncapsulatedPang::EncapsulatedPang(void)
	: m_pang(0), m_pRect(0), m_style(6) {
}

// privatetradedlg.cpp:1170
EncapsulatedPang::~EncapsulatedPang(void) {
}

// privatetradedlg.cpp:1181
void EncapsulatedPang::SetRect(const WRect& rect) {
	if (m_pRect == 0) {
		m_pRect = new WRect(rect);
	} else {
		*m_pRect = rect;
	}
}

// privatetradedlg.cpp:1191
int EncapsulatedPang::IsStyle(eStyle style) {
	return m_style & style;
}

// privatetradedlg.cpp:1201
int EncapsulatedPang::SubPang(__int64 pang) {
	if (m_pang - pang < 0) {
		return FALSE;
	}

	m_pang -= pang;
	return TRUE;
}

// privatetradedlg.cpp:1210
void EncapsulatedPang::AddPangBarStyle(eStyle style) {
	m_style |= style;
}

// privatetradedlg.cpp:1290 (run-time type and factory registration of FrMyWareHouse)
const WRTTI FrMyWareHouse::m_RTTI("FrMyWareHouse", &FrForm::m_RTTI);

IObject* FrMyWareHouseMakeInstance(void) {
	return new FrMyWareHouse;
}

struct __sFrMyWareHouse {
	__sFrMyWareHouse(void) {
		ObjectFactory().AddObjectFunctor(FrMyWareHouseMakeInstance, "FrMyWareHouse");
	}
};
static __sFrMyWareHouse s_registerFrMyWareHouse;

// privatetradedlg.cpp:1292
const sFRESH_MSGMAP* FrMyWareHouse::GetMessageMap(void) const {
	return &FrMyWareHouse::_MsgMap;
}

sFRESH_MSGMAP FrMyWareHouse::_MsgMap = { &FrForm::_MsgMap, &FrMyWareHouse::_MsgEntries[0] };

sFRESH_ENTRY FrMyWareHouse::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnViewDraw },
	{ "ItemList", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnListInit },
	{ "ItemList", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnListDraw },
	{ "ItemList", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnListDblClick },
	{ "SortItem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnSortInit },
	{ "SortItem", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnSortDraw },
	{ "SortItem", FRCMD_LBUTTONUP, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnSortDown },
	{ "PartItem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnPartBtnInit },
	{ "ClubItem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnClubBtnInit },
	{ "WareItem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnItemBtnInit },
	{ "CardItem", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMyWareHouse::OnCardBtnInit },
	{ "PartItem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnPartBtnDown },
	{ "ClubItem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnClubBtnDown },
	{ "WareItem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnItemBtnDown },
	{ "CardItem", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMyWareHouse::OnCardBtnDown },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:1316
FrMyWareHouse::FrMyWareHouse(void)
	: m_pItemList(0), m_pSortList(0), m_curButton(SET_ITEM_BUTTON_PART) {
	back_insert_null(m_buttons, 4);
}

// privatetradedlg.cpp:1321
FrMyWareHouse::~FrMyWareHouse(void) {
	std::for_each(m_warpItems.begin(), m_warpItems.end(), Delete_Object());
}

// privatetradedlg.cpp:1326
bool FrMyWareHouse::OnInit(void) {
	WRect rect = m_rect;
	rect.x += 15.0f;
	rect.y += 80.0f;
	rect.w -= 30.0f;
	rect.h = 20.0f;
	EncapsulatedPang::SetRect(rect);

	SetPang(WSingleton<CSharedDoc>::Instance()->m_myPang);

	return FrForm::OnInit();
}

// privatetradedlg.cpp:1340
void FrMyWareHouse::OnProc(const float dt) {
	FrForm::OnProc(dt);

	WRect rect = m_pItemList->GetScrollBar()->GetRect();
	if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {   // WRect::IsInRect is inline in the original (wtypes.h:174)
		if (IsPoped()) {
			Cancel();
		}
	}
}

// privatetradedlg.cpp:1352
bool FrMyWareHouse::DropItemUp(void) {
	return true;
}

// privatetradedlg.cpp:1416
void FrMyWareHouse::DropItemMoved(void) {
	const sTradeDropItem* pItem = GetDropItem();
	if (ItemManager()->IsDisplayItemNumber(pItem->typeId)) {
		SubListItemCount(pItem->itemId, pItem->typeId, 1);
	} else {
		EraseListItem(pItem->itemId, pItem->typeId);
	}
}

// privatetradedlg.cpp:1430
FrListItem* FrMyWareHouse::FindListItem(unsigned long guid, unsigned long typeId) {
	int size = m_pItemList->GetCurrentItemSize(true);
	for (int i = 0; i < size; i++) {
		FrListItem* pItem = m_pItemList->GetItem(i);
		if (pItem == 0 || pItem->pData == 0) {
			continue;
		}

		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		if (pWarp->guid == guid && pWarp->typeId == typeId) {
			return pItem;
		}
	}

	return 0;
}

// privatetradedlg.cpp:1446
bool FrMyWareHouse::SubListItemCount(unsigned long guid, unsigned long typeId, int count) {
	FrListItem* pItem = FindListItem(guid, typeId);
	if (pItem != 0) {
		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		int rest = pWarp->count - count;
		if (rest >= 0) {
			pWarp->count = rest;
			if (rest == 0) {
				m_pItemList->DelItem(pItem);
			}
			return true;
		}
	}

	return false;
}

// privatetradedlg.cpp:1464
bool FrMyWareHouse::AddListItemCount(unsigned long guid, unsigned long typeId, int count) {
	FrListItem* pItem = FindListItem(guid, typeId);
	if (pItem != 0) {
		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		pWarp->count += count;
		return true;
	}

	return false;
}

// privatetradedlg.cpp:1476
int FrMyWareHouse::GetListItemCount(unsigned long guid, unsigned long typeId) {
	FrListItem* pItem = FindListItem(guid, typeId);
	if (pItem != 0) {
		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		return pWarp->count;
	}

	return 0;
}

// privatetradedlg.cpp:1490
bool FrMyWareHouse::SetListItem(unsigned long guid, unsigned long typeId, int count) {
	FrListItem* pItem = FindListItem(guid, typeId);
	if (pItem != 0) {
		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		if (pWarp != 0) {
			if (count == 0) {
				EraseListItem(guid, typeId);
			} else {
				pWarp->count = count;
			}

			return true;
		}
	} else if (count > 0) {
		return AddListItem(guid, typeId, count);
	}

	return false;
}

// privatetradedlg.cpp:1515
bool FrMyWareHouse::AddListItem(unsigned long guid, unsigned long typeId, int count) {
	if (FindListItem(guid, typeId) != 0) {
		return false;
	}

	for (std::vector<sWarpItem*>::iterator it = m_warpItems.begin(); it != m_warpItems.end(); ++it) {
		for (int i = 0; i < 4; i++) {
			if ((*it)->typeId == typeId && (*it)->guid == guid) {
				(*it)->count = count;
				m_pItemList->AddItem(*it);
				m_pItemList->GetScrollBar()->ScrollToBottom();
				break;
			}
		}
	}

	return true;
}

// privatetradedlg.cpp:1539
bool FrMyWareHouse::EraseListItem(unsigned long guid, unsigned long typeId) {
	FrListItem* pItem = FindListItem(guid, typeId);
	if (pItem != 0) {
		m_pItemList->DelItem(pItem);
		return true;
	}

	return false;
}

// privatetradedlg.cpp:1550
void FrMyWareHouse::InitSortList(eSetItemButton button) {
	if (m_pSortList != 0) {
		m_pSortList->ClearItem();

		switch (button) {
		case SET_ITEM_BUTTON_PART:
			for (int i = 0; i < 10; i++) {
				IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(i | 0x4000000);
				if (pCommon != 0) {
					m_pSortList->AddItem((void*)g_pFresh->GetBitmap(MakeStr("s_%s", pCommon->m_icon)));
				}
			}
			break;
		}
	}
}

// privatetradedlg.cpp:1585
void FrMyWareHouse::OnPartBtnInit(int param) {
	SetButtonStyle(param, SET_ITEM_BUTTON_PART);
}

// privatetradedlg.cpp:1586
void FrMyWareHouse::OnClubBtnInit(int param) {
	SetButtonStyle(param, SET_ITEM_BUTTON_CLUB);
}

// privatetradedlg.cpp:1587
void FrMyWareHouse::OnItemBtnInit(int param) {
	SetButtonStyle(param, SET_ITEM_BUTTON_ITEM);
}

// privatetradedlg.cpp:1588
void FrMyWareHouse::OnCardBtnInit(int param) {
	SetButtonStyle(param, SET_ITEM_BUTTON_CARD);
}

// privatetradedlg.cpp:1593
void FrMyWareHouse::OnSortInit(intptr_t param) {
	m_pSortList = DYNAMIC_CAST(FrListBox, param);

	InitSortList(m_curButton);
}

// privatetradedlg.cpp:1601
void FrMyWareHouse::OnSortDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	const Bitmap* pBitmap = (const Bitmap*)pItem->pData;
	if (pBitmap == 0) {
		return;
	}

	WRect rect(pItem->pos.x, pItem->pos.y,
		pBitmap->Width(), pBitmap->Height());

	g_pFresh->GetManager()->GetGDI()->DrawTexture(pBitmap, rect, 0xffffffff, 0);
}

// privatetradedlg.cpp:1619
void FrMyWareHouse::OnSortDown(int param) {
	FrListItem* pItem = m_pSortList->GetSelected();
	if (pItem == 0) {
		return;
	}

	switch (m_curButton) {
	case SET_ITEM_BUTTON_PART:
		SetItemList(m_curButton, (eSetSortType)pItem->idx);
		break;
	}
}

// privatetradedlg.cpp:1643
void FrMyWareHouse::OnPartBtnDown(void) {
	UnPushedButtonAll();
	m_buttons[SET_ITEM_BUTTON_PART]->SetStatus(FrButton::PRESSED);
	SetItemList(SET_ITEM_BUTTON_PART, SET_SORT_TYPE_DEFAULT);
}

// privatetradedlg.cpp:1651
void FrMyWareHouse::OnClubBtnDown(void) {
	UnPushedButtonAll();
	m_buttons[SET_ITEM_BUTTON_CLUB]->SetStatus(FrButton::PRESSED);
	SetItemList(SET_ITEM_BUTTON_CLUB, SET_SORT_TYPE_DEFAULT);
}

// privatetradedlg.cpp:1659
void FrMyWareHouse::OnItemBtnDown(void) {
	UnPushedButtonAll();
	m_buttons[SET_ITEM_BUTTON_ITEM]->SetStatus(FrButton::PRESSED);
	SetItemList(SET_ITEM_BUTTON_ITEM, SET_SORT_TYPE_DEFAULT);
}

// privatetradedlg.cpp:1675
void FrMyWareHouse::OnViewDraw(void) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();

	const Bitmap* pBitmap = g_pFresh->GetBitmap("cfrm00");
	WRect src(0.0f, 0.0f, pBitmap->Width(), pBitmap->Height());
	pDevice->DrawTexture(pBitmap, src, m_rect, 0xa0800000, 0);

	DrawPang();
}

// privatetradedlg.cpp:1687
void FrMyWareHouse::OnListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);

	SetItemList(m_curButton, SET_SORT_TYPE_DEFAULT);
}

// privatetradedlg.cpp:1695
void FrMyWareHouse::OnListDblClick(void) {
	FrListItem* pItem = m_pItemList->GetSelected();
	if (pItem != 0) {
		sWarpItem* pWarp = (sWarpItem*)pItem->pData;
		if (pWarp != 0) {
			FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
			if (pTrade != 0) {
				pTrade->MoveItem("FrMyWareHouse", pWarp);
			}
		}
	}
}

// privatetradedlg.cpp:1804
void FrMyWareHouse::SetItemList(eSetItemButton button, eSetSortType sortType) {
	InitSortList(button);

	m_pItemList->ClearItem();
	std::for_each(m_warpItems.begin(), m_warpItems.end(), Delete_Object());
	std::vector<sWarpItem*>().swap(m_warpItems);

	switch (button) {
	case SET_ITEM_BUTTON_PART:
		SetListParts(m_warpItems);
		break;
	case SET_ITEM_BUTTON_CLUB:
		SetListClubSet(m_warpItems);
		break;
	case SET_ITEM_BUTTON_ITEM:
		SetListHItem(m_warpItems);
		SetListUitem(m_warpItems);
		break;
	case SET_ITEM_BUTTON_CARD:
		SetListCard(m_warpItems);
		break;
	}

	_sPrivateTradeItem items;
	memset(&items, 0, sizeof(items));
	FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");

	if (pTrade != 0) {
		pTrade->m_pMyTradeWnd->GetTradeItems(items);
	}

	DoSortItem(m_warpItems, button, sortType);
	for (std::vector<sWarpItem*>::iterator it = m_warpItems.begin(); it != m_warpItems.end(); ++it) {
		sWarpItem* pItem = *it;
		bool bOnTrade = false;
		for (int i = 0; i < 4; i++) {
			if (items.items[i].typeId == pItem->typeId && items.items[i].itemId == pItem->guid) {
				if (items.items[i].count == pItem->count) {
					bOnTrade = true;
					break;
				}
			}
		}

		if (!bOnTrade) {
			m_pItemList->AddItem(pItem);
		}
	}

	m_curButton = button;
	m_pItemList->GetScrollBar()->ScrollToFirst();
}

// privatetradedlg.cpp:1875
void FrMyWareHouse::SetButtonStyle(int param, eSetItemButton button) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != 0) {
		pButton->SetPushDelay(0.0f);
		pButton->SetStyle(FrButton::BT_3TYPES);

		m_buttons[button] = pButton;
	}
}

// privatetradedlg.cpp:1893
void FrMyWareHouse::UnPushedButtonAll(void) {
	for (std::vector<FrButton*>::iterator it = m_buttons.begin(); it != m_buttons.end(); ++it) {
		(*it)->SetStatus(FrButton::NORMAL);
	}
}

// privatetradedlg.cpp:1900 (run-time type and factory registration of FrTradeToolTip)
const WRTTI FrTradeToolTip::m_RTTI("FrTradeToolTip", &FrForm::m_RTTI);

IObject* FrTradeToolTipMakeInstance(void) {
	return new FrTradeToolTip;
}

struct __sFrTradeToolTip {
	__sFrTradeToolTip(void) {
		ObjectFactory().AddObjectFunctor(FrTradeToolTipMakeInstance, "FrTradeToolTip");
	}
};
static __sFrTradeToolTip s_registerFrTradeToolTip;

// privatetradedlg.cpp:1902
const sFRESH_MSGMAP* FrTradeToolTip::GetMessageMap(void) const {
	return &FrTradeToolTip::_MsgMap;
}

sFRESH_MSGMAP FrTradeToolTip::_MsgMap = { &FrForm::_MsgMap, &FrTradeToolTip::_MsgEntries[0] };

sFRESH_ENTRY FrTradeToolTip::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrTradeToolTip::OnViewDraw },
	{ "TextList", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTradeToolTip::OnTextListInit },
	{ "TextList", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTradeToolTip::OnTextListDraw },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:1915
FrTradeToolTip::FrTradeToolTip(void)
	: m_alpha(0), m_pTextList(0), m_pIcon(0), m_pItem(0), m_bDragDraw(0), m_pDragItem(0) {
	m_pItem = new _sPrivateTradeItemSingle;
	memset(m_pItem, 0, sizeof(_sPrivateTradeItemSingle));
}

// privatetradedlg.cpp:1921
FrTradeToolTip::~FrTradeToolTip(void) {
	if (m_pItem != 0) { delete m_pItem; m_pItem = 0; }
}

// privatetradedlg.cpp:1926
void FrTradeToolTip::SetDragItem(const sTradeDropItem* pItem) {
	m_pDragItem = pItem;
}

// privatetradedlg.cpp:1931
void FrTradeToolTip::OnProc(const float dt) {
	FrForm::OnProc(dt);

	if (CTradeDragDrop::IsPoped()) {
		m_alpha = 255.0f;
		return;
	}

	WRect rect = m_rect;
	if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {   // WRect::IsInRect is inline in the original (wtypes.h:174)
		m_alpha = 255.0f; m_pTextList->GetScrollBar()->SetVisible(true);
		return;
	}

	if (m_alpha > 0.0f) {
		m_pTextList->GetScrollBar()->SetVisible(false);
		m_alpha -= dt * 120.0f;
		if (m_alpha < 0.0f) {
			m_alpha = 0.0f;
		}
	}
}

// privatetradedlg.cpp:1960
void FrTradeToolTip::SetToolTip(_sPrivateTradeItemSingle* pItem) {
	m_alpha = 255.0f; m_pTextList->GetScrollBar()->SetVisible(true);

	if (m_pItem->itemId == pItem->itemId && m_pItem->typeId == pItem->typeId) {
		return;
	}

	if (CTradeDragDrop::IsPoped()) {
		return;
	}

	*m_pItem = *pItem;

	m_pTextList->ClearItem();

	m_pTextList->GetScrollBar()->ScrollToFirst();

	m_pIcon = 0; m_textLines.clear();

	std::string name, desc, text;
	switch (m_pItem->typeId >> 26) {
	case 28:
		GetIconNameDesc(m_pIcon, name, desc, ItemManager()->FindAuxPart(m_pItem->typeId));
		break;
	case 2:
		GetIconNameDesc(m_pIcon, name, desc, ItemManager()->FindPart(m_pItem->typeId));
		break;
	case 4:
		GetIconNameDesc(m_pIcon, name, desc, ItemManager()->FindClubSet(m_pItem->typeId));
		break;
	case 6:
		GetIconNameDesc(m_pIcon, name, desc, ItemManager()->FindItem(m_pItem->typeId));
		break;
	case 31:
		GetIconNameDesc(m_pIcon, name, desc, ItemManager()->FindCard(m_pItem->typeId));
		break;
	}

	m_textLines.push_back("");
	m_textLines.push_back(name);

	if ((m_pItem->typeId & 0xfc000000) == 0x8000000) {
		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(m_pItem->typeId);
		if (pPart != 0) {
			m_textLines.push_back("");
			GetAblilityText(m_textLines, pPart->m_baseStat, pPart->m_slotStat);
		}
	}

	if ((m_pItem->typeId & 0xfc000000) == 0x10000000) {
		IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_pItem->typeId);
		if (pClubSet != 0) {
			m_textLines.push_back("");
			GetAblilityText(m_textLines, pClubSet->m_baseStat, m_pItem->slotStats);
		}
	}

	for (int i = 0; i < 8; i++) {
		unsigned long cardId = m_pItem->cards[i].typeId;
		if (cardId != 0) {
			std::string cardText(" + ");
			cardText += GetCardInfoText(cardId);

			m_textLines.push_back(cardText.c_str());
		}
	}

	m_textLines.push_back("");
	MakeFormatText(desc, m_textLines, 0);

	for (std::vector<std::string>::iterator it = m_textLines.begin(); it != m_textLines.end(); ++it) {
		m_pTextList->AddItem(&*it);
	}
}

// privatetradedlg.cpp:2094
void FrTradeToolTip::OnTextListInit(intptr_t param) {
	m_pTextList = DYNAMIC_CAST(FrListBox, param);
}

// privatetradedlg.cpp:2101
void FrTradeToolTip::OnTextListDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	std::string* pText = (std::string*)pItem->pData;

	pDevice->SetTextColor(((unsigned long)(unsigned char)m_alpha << 24) | 0x00ffffff, 0xffffffff);
	pDevice->PrintText11(WPoint(pItem->pos.x, pItem->pos.y + 2.0f), 0, pText->c_str(), 0);
	pDevice->SetTextColor(0xff000000, 0xffffffff);
}

// privatetradedlg.cpp:2176 (run-time type and factory registration of FrPrivateTradeWnd)
const WRTTI FrPrivateTradeWnd::m_RTTI("FrPrivateTradeWnd", &FrForm::m_RTTI);

IObject* FrPrivateTradeWndMakeInstance(void) {
	return new FrPrivateTradeWnd;
}

struct __sFrPrivateTradeWnd {
	__sFrPrivateTradeWnd(void) {
		ObjectFactory().AddObjectFunctor(FrPrivateTradeWndMakeInstance, "FrPrivateTradeWnd");
	}
};
static __sFrPrivateTradeWnd s_registerFrPrivateTradeWnd;

// privatetradedlg.cpp:2178
const sFRESH_MSGMAP* FrPrivateTradeWnd::GetMessageMap(void) const {
	return &FrPrivateTradeWnd::_MsgMap;
}

sFRESH_MSGMAP FrPrivateTradeWnd::_MsgMap = { &FrForm::_MsgMap, &FrPrivateTradeWnd::_MsgEntries[0] };

sFRESH_ENTRY FrPrivateTradeWnd::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnViewDraw },
	{ "ItemList", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnListInit },
	{ "ItemList", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnListDraw },
	{ "ItemList", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnListDown },
	{ "ItemList", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnListDblClick },
	{ "NumEditBtn01", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnNumEditBtn01Init },
	{ "NumEditBtn01", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnNumEditBtn01Down },
	{ "NumEditBtn02", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnNumEditBtn02Init },
	{ "NumEditBtn02", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnNumEditBtn02Down },
	{ "NumEditBtn03", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnNumEditBtn03Init },
	{ "NumEditBtn03", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnNumEditBtn03Down },
	{ "NumEditBtn04", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTradeWnd::OnNumEditBtn04Init },
	{ "NumEditBtn04", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTradeWnd::OnNumEditBtn04Down },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:2201
FrPrivateTradeWnd::FrPrivateTradeWnd(void)
	: m_nick("unknown_nick"), m_avatarIndex(0), m_pPangEditWnd(0), m_pNumEditWnd(0),
	  m_bActiveControl(true), m_tradeState(0), m_pTradeItem(0) {
	back_insert_null(m_buttons, 4);
	m_pTradeItem = new _sPrivateTradeItem;
	memset(m_pTradeItem, 0, sizeof(_sPrivateTradeItem));
}

// privatetradedlg.cpp:2208
FrPrivateTradeWnd::~FrPrivateTradeWnd(void) {
	if (m_pTradeItem != 0) {
		delete m_pTradeItem;
		m_pTradeItem = 0;
	}
	m_pItemList->ClearItem();
	m_buttons.clear();
}

// privatetradedlg.cpp:2215
bool FrPrivateTradeWnd::OnInit(void) {
	WRect rect = m_rect;
	rect.x += 145.0f;
	rect.y += 5.0f;
	rect.w -= 150.0f;
	rect.h = 20.0f;
	EncapsulatedPang::SetRect(rect);

	return FrForm::OnInit();
}

// privatetradedlg.cpp:2228
bool FrPrivateTradeWnd::DropItemUp(void) {
	return false;
}

// privatetradedlg.cpp:2236
bool FrPrivateTradeWnd::DropItemDown(void) {
	if (!m_bActiveControl) {
		return false;
	}

	const sTradeDropItem* pItem = GetDropItem();
	if (pItem != 0) {
		int count = 0;
		for (int i = 0; i < 4; i++) {
			FrListItem* pListItem = m_pItemList->GetItem(i);
			if (pListItem != 0 && pListItem->pData != 0) {
				_sPrivateTradeItemSingle* pSingle = (_sPrivateTradeItemSingle*)pListItem->pData;

				if (pSingle->itemId != 0 && pSingle->typeId != 0) {
					count++;
				}

				if (pItem->itemId == pSingle->itemId && pItem->typeId == pSingle->typeId) {
					pSingle->count++;
					OnSendItemUpdate();
					return true;
				}
			}
		}

		if (count < 4) {
			AddItem(pItem);
			return true;
		}
	}

	return false;
}

// privatetradedlg.cpp:2274
void FrPrivateTradeWnd::DropItemMoved(void) {
	if (m_bActiveControl) {
		SubItem(GetDropItem());
		DoTradeUnLock();
	}
}

// privatetradedlg.cpp:2283
void FrPrivateTradeWnd::SetAvatarIndex(int index) {
	m_avatarIndex = index;
}

// privatetradedlg.cpp:2294
void FrPrivateTradeWnd::GetTradeItems(_sPrivateTradeItem& items) {
	items.pang = GetPang();
	for (int i = 0; i < 4; i++) {
		FrListItem* pItem = m_pItemList->GetItem(i);
		if (pItem != 0 && pItem->pData != 0) {
			items.items[i] = *(const _sPrivateTradeItemSingle*)pItem->pData;
		}
	}
}

// privatetradedlg.cpp:2313
void FrPrivateTradeWnd::DoConfirm(void) {
	m_tradeState = 1;
}

// privatetradedlg.cpp:2323
void FrPrivateTradeWnd::DoPermit(void) {
	m_tradeState = 2;
	m_bActiveControl = false;
}

// privatetradedlg.cpp:2334
void FrPrivateTradeWnd::DoTradeUnLock(void) {
	if (m_tradeState != 0) {
		FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
		if (pTrade != 0) {
			pTrade->ButtonContolByConfirmCancel();
		}

		m_tradeState = 0;
	}
}

// privatetradedlg.cpp:2347
void FrPrivateTradeWnd::OpenPangEditWnd(void) {
	if (m_bActiveControl) {
		WRect rect = *GetRectPtr();
		if (m_pPangEditWnd == 0 && rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
			OnSendItemUpdate();

			m_pPangEditWnd = CreateForm<FrPangEdit>(g_pFresh->GetManager(), this, "FrPangEdit", 0);

			m_pPangEditWnd->EnableDrag(false);
			m_pPangEditWnd->SetFixed(true);
			m_pPangEditWnd->SetPangValue(GetPang());

			m_pPangEditWnd->SetTexRate(5); m_pPangEditWnd->Open((FRESH_RESULT_CALLBACK)&FrPrivateTradeWnd::OnResultEditPang, WPoint(rect.x + 10.0f, rect.y + 10.0f), 1);
		}
	}
}

// privatetradedlg.cpp:2369
void FrPrivateTradeWnd::OpenNumEditWnd(int index) {
	if (!m_bActiveControl || index < 0 || index > 3) {
		return;
	}

	FrButton* pButton = m_buttons[index];
	if (pButton != 0 && !pButton->IsVisible()) {
		return;
	}

	FrListItem* pListItem = m_pItemList->GetItem(index);
	if (pListItem == 0) {
		return;
	}

	sTradeDropItem* pItem = (sTradeDropItem*)pListItem->pData;
	if (pItem->itemId == 0 && pItem->typeId == 0) {
		return;
	}

	WPoint pos(pButton->GetRect().x, pButton->GetRect().y);
	if (m_pNumEditWnd == 0) {
		OnSendItemUpdate();

		m_pNumEditWnd = CreateForm<FrNumEdit>(g_pFresh->GetManager(), this, "FrNumEdit", 0);

		m_pNumEditWnd->EnableDrag(false);
		m_pNumEditWnd->SetFixed(true);
		m_pNumEditWnd->SetValue(pItem->count);
		m_pNumEditWnd->SetReturnParam(pListItem);
		m_pNumEditWnd->SetEditRange(0, GetMyItemCount(pItem->itemId, pItem->typeId));
		m_pNumEditWnd->Open((FRESH_RESULT_CALLBACK)&FrPrivateTradeWnd::OnResultEditNum, WPoint(pos.x + 10.0f, pos.y + 10.0f), 1);
	}
}

// privatetradedlg.cpp:2404
void FrPrivateTradeWnd::OnSendItemUpdate(void) {
	if (m_bActiveControl) {
		AfxGetTask()->GetActor("PrivateTrade") << MsgObject(0, 0x247, 1, (intptr_t)m_pTradeItem, 0, 0, 0);

		FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
		if (pTrade != 0) {
			pTrade->m_pCompanionTradeWnd->DoTradeUnLock(); pTrade->m_pMyTradeWnd->DoTradeUnLock();
		}
	}
}

// privatetradedlg.cpp:2419
void FrPrivateTradeWnd::OnProc(const float dt) {
	if (EncapsulatedPang::IsStyle(EncapsulatedPang::STYLE_EDITABLE) && g_input->GetButton(LEFT_BUTTON) == 1) {
		OpenPangEditWnd();
	}
}

// privatetradedlg.cpp:2429
bool FrPrivateTradeWnd::OnResultEditPang(int result, FrForm* pForm) {
	if (m_bActiveControl && result == FrOK) {
		FrPangEdit* pEdit = DYNAMIC_CAST(FrPangEdit, pForm);
		if (pEdit != 0) {
			__int64 pang = pEdit->GetPangValue();
			__int64 tex = pEdit->GetTex();

			if (WSingleton<CSharedDoc>::Instance()->m_myPang >= pang + tex) {
				SetPang(pang);
				m_pTradeItem->pang = pang;

				FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
				if (pTrade != 0) {
					pTrade->SetPang(tex);
				}

				FrMyWareHouse* pWareHouse = FindPrivateTradeChildForm<FrMyWareHouse>("FrMyWareHouse");
				if (pWareHouse != 0) {
					pWareHouse->SetPang(WSingleton<CSharedDoc>::Instance()->m_myPang - tex - pang);
				}
				OnSendItemUpdate();
			}
		}
	}

	m_pPangEditWnd = 0;
	return true;
}

// privatetradedlg.cpp:2465
bool FrPrivateTradeWnd::OnResultEditNum(int result, FrForm* pForm) {
	if (m_bActiveControl && result == FrOK) {
		FrNumEdit* pEdit = DYNAMIC_CAST(FrNumEdit, pForm);
		if (pEdit != 0) {
			unsigned long value = pEdit->GetValue();
			FrListItem* pListItem = (FrListItem*)pEdit->GetReturnParam();
			if (pListItem != 0 && pListItem->pData != 0) {
				_sPrivateTradeItemSingle* pSingle = (_sPrivateTradeItemSingle*)pListItem->pData;
				unsigned long count = GetMyItemCount(pSingle->itemId, pSingle->typeId);
				if (value <= count) {
					pSingle->count = (short)value;
					FrMyWareHouse* pWareHouse = FindPrivateTradeChildForm<FrMyWareHouse>("FrMyWareHouse");
					if (pWareHouse != 0) {
						pWareHouse->SetListItem(pSingle->itemId, pSingle->typeId, count - value);
					}
					OnSendItemUpdate();
				}
			}
		}
	}

	m_pNumEditWnd = 0;
	return true;
}

// privatetradedlg.cpp:2498
void FrPrivateTradeWnd::AddItem(const sTradeDropItem* pItem) {
	if (m_bActiveControl) {
		for (int i = 0; i < 4; i++) {
			FrListItem* pListItem = m_pItemList->GetItem(i);
			if (pListItem != 0) {
				_sPrivateTradeItemSingle* pSingle = (_sPrivateTradeItemSingle*)pListItem->pData;
				if (pSingle->itemId == 0 && pSingle->typeId == 0) {
					Formator(*pItem, *pSingle);
					OnSendItemUpdate();
					break;
				}
			}
		}
	}
}

// privatetradedlg.cpp:2537
void FrPrivateTradeWnd::OnUpdateTradeItem(const _sPrivateTradeItem& items) {
	if (!m_bActiveControl) {
		*m_pTradeItem = items;
		SetPang(m_pTradeItem->pang);
	}
}

// privatetradedlg.cpp:2546
void FrPrivateTradeWnd::SubItem(const sTradeDropItem* pItem) {
	for (int i = 0; i < 4; i++) {
		FrListItem* pListItem = m_pItemList->GetItem(i);
		if (pListItem != 0 && pListItem->pData != 0) {
			_sPrivateTradeItemSingle* pSingle = (_sPrivateTradeItemSingle*)pListItem->pData;
			if (pSingle->itemId == pItem->itemId && pSingle->typeId == pItem->typeId) {
				memset(pSingle, 0, sizeof(_sPrivateTradeItemSingle));
				OnSendItemUpdate();
				break;
			}
		}
	}
}

// privatetradedlg.cpp:2566
void FrPrivateTradeWnd::OnListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);

	for (int i = 0; i < 4; i++) {
		m_pItemList->AddItem(&m_pTradeItem->items[i]);
	}
}

// privatetradedlg.cpp:2579
void FrPrivateTradeWnd::OnListDown(void) {
	if (m_bActiveControl) {
		FrListItem* pItem = m_pItemList->GetSelected();
		if (pItem != 0) {
			sTradeDropItem* pTradeItem = (sTradeDropItem*)pItem->pData;
			if (pTradeItem != 0) {
				if (pTradeItem->itemId != 0 || pTradeItem->typeId != 0) {
					CTradeDragDrop::SetPopItem(sTradeDropItem(pTradeItem->itemId, pTradeItem->typeId));
				}
			}
		}
	}
}

// privatetradedlg.cpp:2603
void FrPrivateTradeWnd::OnListDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	if (pItem->pData == 0) {
		return;
	}

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	WRect rect(pItem->pos, m_pItemList->GetItemWidth(), m_pItemList->GetItemHeight());

	const Bitmap* pBg = g_pFresh->GetBitmap("cfrm00");
	WRect src(0.0f, 0.0f, pBg->Width(), pBg->Height());

	_sPrivateTradeItemSingle* pSingle = (_sPrivateTradeItemSingle*)pItem->pData;
	if (pSingle->itemId == 0 && pSingle->typeId == 0) {
		// An empty slot: the frame, an outline, and no number-entry button.
		pDevice->DrawTexture(pBg, src, rect, 0xa0ffffff, 0);
		pDevice->LineBox(WRect(rect.x + 1.0f, rect.y + 1.0f, rect.w - 2.0f, rect.h - 2.0f), 0xff0000ff, 0, 0.0f);

		if (pItem->idx >= 0 && pItem->idx <= 3) {
			m_buttons[pItem->idx]->SetVisible(false);
		}
		return;
	}

	// "unknown item" (CP949)
	std::string name("\xbe\xcb\xbc\xf6 \xbe\xf8\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb");
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pSingle->typeId);
	const Bitmap* pIcon = 0;
	if (pCommon != 0) {
		pIcon = g_pFresh->GetBitmap(pCommon->m_icon);
		name = pCommon->m_name;
	}

	if (pIcon == 0) {
		pIcon = g_pFresh->GetBitmap("caddie_no");
	}

	WOverlay::DrawLineBox(g_view, WRect(rect.x + 1.0f, rect.y + 1.0f, rect.w - 2.0f, rect.h - 2.0f), 0, 0xffffff00);

	float x = (rect.w - pIcon->Width()) * 0.5f + pItem->pos.x;
	float y = (rect.h - pIcon->Height()) * 0.5f + pItem->pos.y;

	WRect iconRect(x, y, pIcon->Width(), pIcon->Height());

	if ((pSingle->typeId & 0xfc000000) == 0x18000000) {
		iconRect.x = x + 10.0f;
		iconRect.y = y + 10.0f;
	}
	pDevice->DrawTexture(pIcon, iconRect, 0xffffffff, 0);

	if ((pSingle->typeId & 0xfc000000) == 0x8000000) {
		DrawPartsCardSlot(pSingle, rect, 255.0f, 1);
	}

	unsigned long group = pSingle->typeId >> 26;
	if (group == 6 || group == 0x1f) {
		m_pFont->Print(g_view, rect.x + 10.0f, rect.y + rect.h - 25.0f, MakeStr("%d", (unsigned short)pSingle->count), 0, 0xffffffff, 0);
		if (pItem->idx >= 0 && pItem->idx <= 3 && m_bActiveControl) {
			m_buttons[pItem->idx]->SetVisible(true);
		}
	} else {
		if (pItem->idx >= 0 && pItem->idx <= 3) {
			m_buttons[pItem->idx]->SetVisible(false);
		}
	}

	if (m_tradeState == 1) {
		pDevice->DrawTexture(pBg, src, rect, 0x40ffff00, 0);
	}
	if (m_tradeState == 2) {
		pDevice->DrawTexture(pBg, src, rect, 0x4000ff00, 0);
	}

	if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
		FrTradeToolTip* pToolTip = FindPrivateTradeChildForm<FrTradeToolTip>("FrTradeToolTip");
		if (pToolTip != 0) {
			pToolTip->SetToolTip(pSingle);
		}
	}
}

// privatetradedlg.cpp:2698
void FrPrivateTradeWnd::OnListDblClick(void) {
	if (m_bActiveControl) {
		FrListItem* pItem = m_pItemList->GetSelected();
		if (pItem != 0 && pItem->pData != 0) {
			void* pData = pItem->pData;
			FrPrivateTrade* pTrade = (FrPrivateTrade*)g_pFresh->GetDesktop()->FindChildForm("FrPrivateTrade");
			if (pTrade != 0) {
				pTrade->MoveItem("FrPrivateTradeWnd", pData);
			}
		}
	}
}

// privatetradedlg.cpp:2763
void FrPrivateTradeWnd::OnNumEditBtn01Init(intptr_t param) {
	m_buttons[0] = DYNAMIC_CAST(FrButton, param);
	if (m_buttons[0] != 0) {
		m_buttons[0]->SetVisible(false);
	}
}

// privatetradedlg.cpp:2770
void FrPrivateTradeWnd::OnNumEditBtn02Init(intptr_t param) {
	m_buttons[1] = DYNAMIC_CAST(FrButton, param);
	if (m_buttons[1] != 0) {
		m_buttons[1]->SetVisible(false);
	}
}

// privatetradedlg.cpp:2777
void FrPrivateTradeWnd::OnNumEditBtn03Init(intptr_t param) {
	m_buttons[2] = DYNAMIC_CAST(FrButton, param);
	if (m_buttons[2] != 0) {
		m_buttons[2]->SetVisible(false);
	}
}

// privatetradedlg.cpp:2784
void FrPrivateTradeWnd::OnNumEditBtn04Init(intptr_t param) {
	m_buttons[3] = DYNAMIC_CAST(FrButton, param);
	if (m_buttons[3] != 0) {
		m_buttons[3]->SetVisible(false);
	}
}

// privatetradedlg.cpp:2790
void FrPrivateTradeWnd::OnNumEditBtn01Down(void) {
	OpenNumEditWnd(0);
}

// privatetradedlg.cpp:2791
void FrPrivateTradeWnd::OnNumEditBtn02Down(void) {
	OpenNumEditWnd(1);
}

// privatetradedlg.cpp:2792
void FrPrivateTradeWnd::OnNumEditBtn03Down(void) {
	OpenNumEditWnd(2);
}

// privatetradedlg.cpp:2793
void FrPrivateTradeWnd::OnNumEditBtn04Down(void) {
	OpenNumEditWnd(3);
}


// privatetradedlg.cpp:2797 (run-time type and factory registration of FrPrivateTrade)
const WRTTI FrPrivateTrade::m_RTTI("FrPrivateTrade", &FrForm::m_RTTI);

IObject* FrPrivateTradeMakeInstance(void) {
	return new FrPrivateTrade;
}

struct __sFrPrivateTrade {
	__sFrPrivateTrade(void) {
		ObjectFactory().AddObjectFunctor(FrPrivateTradeMakeInstance, "FrPrivateTrade");
	}
};
static __sFrPrivateTrade s_registerFrPrivateTrade;

// privatetradedlg.cpp:2799
const sFRESH_MSGMAP* FrPrivateTrade::GetMessageMap(void) const {
	return &FrPrivateTrade::_MsgMap;
}

sFRESH_MSGMAP FrPrivateTrade::_MsgMap = { &FrForm::_MsgMap, &FrPrivateTrade::_MsgEntries[0] };

sFRESH_ENTRY FrPrivateTrade::_MsgEntries[] = {
	{ "RenderArea", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrPrivateTrade::OnViewDraw },
	{ "Confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTrade::OnInitConfirmBtn },
	{ "Permit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTrade::OnInitPermitBtn },
	{ "Cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrPrivateTrade::OnInitCancelBtn },
	{ "Confirm", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTrade::OnDownConfirmBtn },
	{ "Permit", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTrade::OnDownPermitBtn },
	{ "Cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrPrivateTrade::OnDownCancelBtn },
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// privatetradedlg.cpp:2816
FrPrivateTrade::FrPrivateTrade(void)
	: m_pDragItem(0), m_pConfirmBtn(0), m_pPermitBtn(0), m_pCancelBtn(0) {
}

// privatetradedlg.cpp:2821
FrPrivateTrade::~FrPrivateTrade(void) {
}

// privatetradedlg.cpp:2826
void FrPrivateTrade::SetCompanion(const std::string &nickName, unsigned long guid, unsigned long typeId) {
	m_companionNick = nickName;
	m_companionGuid = guid;
	m_companionTypeId = typeId;
}

// privatetradedlg.cpp:2833
bool FrPrivateTrade::OnInit(void) {
	FrForm::OnInit();

	FrPrivateTradeWnd* pCompanion = CreateForm<FrPrivateTradeWnd>(g_pFresh->GetManager(), this, "FrPrivateTradeWnd", 0);
	if (pCompanion == 0) {
		return false;
	}
	pCompanion->SetFixed(true);
	pCompanion->EnableDrag(false);

	pCompanion->Open((FRESH_RESULT_CALLBACK)0, WPoint(m_rect.x + 16.0f, m_rect.y + 16.0f), 12);
	m_pCompanionTradeWnd = pCompanion;
	pCompanion->EnableActiveControl(false);
	m_pCompanionTradeWnd->SetNick(m_companionNick);
	m_pCompanionTradeWnd->SetAvatarIndex(m_companionTypeId & 0x3ffffff);

	FrPrivateTradeWnd* pMine = CreateForm<FrPrivateTradeWnd>(g_pFresh->GetManager(), this, "FrPrivateTradeWnd", 0);
	if (pMine == 0) {
		return false;
	}
	pMine->SetFixed(true);
	pMine->EnableDrag(false);
	pMine->Open((FRESH_RESULT_CALLBACK)0, WPoint(m_rect.x + 16.0f, pCompanion->GetRect().y + pCompanion->GetRect().h + 16.0f), 12);
	m_pMyTradeWnd = pMine;
	m_pMyTradeWnd->SetNick(std::string(WSingleton<CSharedDoc>::Instance()->m_userInfo.nickname));
	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	std::map<unsigned int, sCharacterInfo>::iterator it = pDoc->m_characterMap.find(pDoc->m_myCharacterId);
	if (it != pDoc->m_characterMap.end()) {
		m_pMyTradeWnd->SetAvatarIndex(it->second.typeId & 0x3ffffff);
	}
	m_pMyTradeWnd->AddPangBarStyle(EncapsulatedPang::STYLE_EDITABLE);

	const WRect& myRect = pMine->GetRect();
	float right = myRect.x + myRect.w;
	EncapsulatedPang::SetRect(WRect(right - 150.0f, myRect.y + myRect.h + 5.0f, 150.0f, 20.0f));

	FrMyWareHouse* pWareHouse = CreateForm<FrMyWareHouse>(g_pFresh->GetManager(), this, "FrMyWareHouse", 0);
	if (pWareHouse == 0) {
		return false;
	}
	pWareHouse->SetFixed(true);
	pWareHouse->EnableDrag(false);
	pWareHouse->Open((FRESH_RESULT_CALLBACK)0, WPoint(m_rect.w + m_rect.x - pWareHouse->GetRect().w - 16.0f, (m_rect.h - pWareHouse->GetRect().h) * 0.5f + m_rect.y), 12);
	m_pWareHouse = pWareHouse;

	FrTradeToolTip* pToolTip = CreateForm<FrTradeToolTip>(g_pFresh->GetManager(), this, "FrTradeToolTip", 0);
	if (pToolTip == 0) {
		return false;
	}
	pToolTip->SetFixed(true);
	pToolTip->EnableDrag(false);
	pToolTip->Open((FRESH_RESULT_CALLBACK)0, WPoint(m_rect.x + 16.0f, m_rect.y + m_rect.h - pToolTip->GetRect().h - 16.0f), 12);
	m_pToolTip = pToolTip;
	return true;
}

// privatetradedlg.cpp:2903
void FrPrivateTrade::GetMyTradeItems(_sPrivateTradeItem& items) {
	m_pMyTradeWnd->GetTradeItems(items);
}

// privatetradedlg.cpp:2949
// The trade window the left button went down on (the drop target).
static CTradeDragDrop* s_pDropTarget;

void FrPrivateTrade::OnProc(const float dt) {
	FrForm::OnProc(dt);

	const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();

	if (g_input->GetButton(LEFT_BUTTON) == 1) {
		WRect myRect = m_pMyTradeWnd->GetRect();
		if (myRect.IsInRect(mousePos)) {
			s_pDropTarget = m_pMyTradeWnd;
		} else {
			WRect rect = m_pWareHouse->GetRect();
			if (rect.IsInRect(mousePos)) {
				s_pDropTarget = m_pWareHouse;
			} else {
				m_pDragItem = 0;
				s_pDropTarget = 0;
			}
		}

		if (s_pDropTarget != 0) {
			s_pDropTarget->Pop();
		}
	}

	if (g_input->GetButton(LEFT_BUTTON) == 3) {
		if (s_pDropTarget != 0) {
			m_pDragItem = s_pDropTarget->GetDropItem();
			if (m_pDragItem != 0) {
				m_pToolTip->SetDragDraw(1);
				m_pToolTip->SetDragItem(m_pDragItem);
			} else {
				m_pDragItem = 0;
				s_pDropTarget = 0;
			}
		}
	}

	if (g_input->GetButton(LEFT_BUTTON) == 2) {
		CTradeDragDrop* pTarget = 0;
		WRect myRect = m_pMyTradeWnd->GetRect();
		if (myRect.IsInRect(mousePos)) {
			pTarget = m_pMyTradeWnd;
		} else {
			WRect rect = m_pWareHouse->GetRect();
			if (rect.IsInRect(mousePos)) {
				pTarget = m_pWareHouse;
			}
		}

		CTradeDragDrop::Drop(pTarget);

		m_pDragItem = 0;
		s_pDropTarget = 0;
	}

	if (m_pDragItem == 0) {
		m_pToolTip->SetDragDraw(0);
	}
}

// privatetradedlg.cpp:3014
void FrPrivateTrade::UpdateFrTradeItem(const _sPrivateTradeItem& items) {
	m_pCompanionTradeWnd->OnUpdateTradeItem(items);
	SetTradeUnLockAll();
}

// privatetradedlg.cpp:3020
void FrPrivateTrade::FinalFrWait(const _sPrivateTradeItem& items) {
	_sPrivateTradeItem current;
	memset(&current, 0, sizeof(current));

	m_pCompanionTradeWnd->GetTradeItems(current);
	if (memcmp(&current, &items, sizeof(current)) != 0) {
		m_pCompanionTradeWnd->OnUpdateTradeItem(items);
		SetTradeUnLockAll();
	}

	m_pCompanionTradeWnd->DoConfirm();
}

// privatetradedlg.cpp:3038
void FrPrivateTrade::SetTradeUnLockAll(void) {
	m_pCompanionTradeWnd->DoTradeUnLock();
	m_pMyTradeWnd->DoTradeUnLock();
}

// privatetradedlg.cpp:3044
void FrPrivateTrade::FinalFrOK(const _sPrivateTradeItem& items) {
	_sPrivateTradeItem current;
	memset(&current, 0, sizeof(current));

	m_pCompanionTradeWnd->GetTradeItems(current);
	if (memcmp(&current, &items, sizeof(current)) != 0) {
		m_pCompanionTradeWnd->OnUpdateTradeItem(items);
	}

	m_pCompanionTradeWnd->DoPermit();
}

// privatetradedlg.cpp:3061
void FrPrivateTrade::ButtonContolByConfirmCancel(void) {
	m_pConfirmBtn->Enable(true);
	m_pPermitBtn->Enable(false);
	m_pCancelBtn->Enable(true);
}

// privatetradedlg.cpp:3069
void FrPrivateTrade::OnViewDraw(void) {
	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	const Bitmap* pBitmap = g_pFresh->GetBitmap("cfrm00");
	WRect src(0.0f, 0.0f, pBitmap->Width(), pBitmap->Height());
	pDevice->DrawTexture(pBitmap, src, m_rect, 0xffffffff, 0);

	DrawPang();
}

// privatetradedlg.cpp:3084
void FrPrivateTrade::OnInitConfirmBtn(intptr_t param) {
	m_pConfirmBtn = DYNAMIC_CAST(FrButton, param);
}

// privatetradedlg.cpp:3091
void FrPrivateTrade::OnInitPermitBtn(intptr_t param) {
	m_pPermitBtn = DYNAMIC_CAST(FrButton, param);
}

// privatetradedlg.cpp:3099
void FrPrivateTrade::OnInitCancelBtn(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// privatetradedlg.cpp:3106
void FrPrivateTrade::OnDownConfirmBtn(void) {
	m_pConfirmBtn->Enable(false);
	m_pPermitBtn->Enable(true);
	m_pMyTradeWnd->DoConfirm();

	_sPrivateTradeItem items;
	memset(&items, 0, sizeof(items));
	m_pMyTradeWnd->GetTradeItems(items);
	AfxGetTask()->GetActor("PrivateTrade") << MsgObject(0, 0x247, 2, (intptr_t)&items, 0, 0, 0);
}

// privatetradedlg.cpp:3120
void FrPrivateTrade::OnDownPermitBtn(void) {
	m_pConfirmBtn->Enable(false);
	m_pPermitBtn->Enable(false);

	m_pMyTradeWnd->DoPermit();

	_sPrivateTradeItem myItems;
	memset(&myItems, 0, sizeof(myItems));
	_sPrivateTradeItem companionItems;
	memset(&companionItems, 0, sizeof(companionItems));
	m_pMyTradeWnd->GetTradeItems(myItems);
	m_pCompanionTradeWnd->GetTradeItems(companionItems);

	AfxGetTask()->GetActor("PrivateTrade") << MsgObject(0, 0x247, 3, (intptr_t)&myItems, (intptr_t)&companionItems, 0, 0);
}

// privatetradedlg.cpp:3137
void FrPrivateTrade::OnDownCancelBtn(void) {
	FrForm::Close(FrCANCEL, true);
}
