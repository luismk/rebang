// Original: d:\build\custom\temp\code\client\fresh\source\frcombobox.cpp
#include <stdio.h>
#include <string.h>

#include "frcombobox.h"
#include "ObjectFactory.h"
#include "bitmap.h"
#include "frbutton.h"
#include "frdesktop.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "frwndmanager.h"
#include "../client/ProjectG/ChatMsg.h"
#include "../client/ProjectG/commonutil.h"

// wtypes.h:174: WRect::IsInRect is inline in the original; local copy until wtypes.h defines it.
inline bool WRect::IsInRect(const WPoint& pt) {
	if (pt.x >= x && pt.x <= x + w && pt.y >= y && pt.y <= y + h)
		return true;
	return false;
}

// frcombobox.cpp:16 (run-time type and factory registration of FrComboBox)
const WRTTI FrComboBox::m_RTTI("FrComboBox", &FrWnd::m_RTTI);

IObject* FrComboBoxMakeInstance(void) {
	return new FrComboBox;
}

struct __sFrComboBox {
	__sFrComboBox(void) {
		ObjectFactory().AddObjectFunctor(FrComboBoxMakeInstance, "FrComboBox");
	}
};
static __sFrComboBox s_registerFrComboBox;

// frcombobox.cpp:19
const sFRESH_MSGMAP* FrComboBox::GetMessageMap(void) const {
	return &FrComboBox::_MsgMap;
}

sFRESH_MSGMAP FrComboBox::_MsgMap = { &FrCmdTarget::_MsgMap, &FrComboBox::_MsgEntries[0] };

sFRESH_ENTRY FrComboBox::_MsgEntries[] = {
	// frcombobox.cpp:21
	{ "unfold", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrComboBox::OnUnfold },
	// frcombobox.cpp:22
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrComboBox::OnListOwnerDraw },
	// frcombobox.cpp:23
	{ "list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrComboBox::OnListBtnDown },
	// frcombobox.cpp:24
	{ "list", FRCMD_LOSTKEYFOCUS, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrComboBox::OnListLostKeyFocus },

	// frcombobox.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frcombobox.cpp:33
FrComboBox::FrComboBox(void) {
	m_maxListNum = 10;
	m_selected = m_comboList.end();
	m_pButton = 0;
	m_pListBox = 0;
	m_selectColor = 0;
}

// frcombobox.cpp:43
FrComboBox::~FrComboBox(void) {
	ClearList();
}

// frcombobox.cpp:48
void FrComboBox::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrEdit::Init(item, pManager, pParent);

	FrElementDoc* pDoc = pManager->GetDocument();

	m_multiLine = false;

	m_pButton = new FrButton;
	if (m_pButton) {
		FrGuiItem& info = m_buttonInfo;
		info = item;
		info.m_caption = "unfold";
		info.m_name = "unfold";
		info.m_rect.Reset();

		m_pButton->SetOwner(this);
		m_pButton->Init(info, pManager, this, true);
		m_pButton->UseDblClick(false);
		m_pButton->SetPushDelay(0.0f);

		WRect rect = m_pButton->GetRect();
		rect.x += m_rect.w - rect.w;
		rect.y += (int)(float(m_rect.h - rect.h + 1.0f) * 0.5f);
		m_pButton->SetRect(rect);
	}

	if (item.m_param.find("selectcolor") != item.m_param.end())
		sscanf(item.m_param["selectcolor"].c_str(), "%x", &m_selectColor);

	m_pListBox = new FrListBox;
	if (m_pListBox) {
		FrGuiItem& info = m_listInfo;
		info = item;
		info.m_caption = "list";
		info.m_name = "list";
		info.m_rect.right -= info.m_rect.left;
		info.m_rect.bottom -= info.m_rect.top;
		info.m_rect.left = 0;
		float height = pManager->GetDesktop()->GetRect().h;
		info.m_rect.top = m_rect.y > height * 0.5f ? -5 : 5;

		m_pListBox->SetItemSize(info.m_rect.right, m_lineHeight);
		m_pListBox->SetOwner(this);
		m_pListBox->Init(info, pManager, this);
		m_pListBox->SetTopmost(true);

		if (item.m_param.find("maxlistnum") != item.m_param.end())
			sscanf(item.m_param["maxlistnum"].c_str(), "%d", &m_maxListNum);

		ResizeListRect();

		m_pListBox->SetVisible(false);
		m_pListBox->SetKeyEvent(true);
		m_pListBox->EnableHover(true, 0.0f);
	}
}

// frcombobox.cpp:118
void FrComboBox::IncreaseMaxListNum(int num) {
	if (num >= m_maxListNum) {
		m_maxListNum = num;
		ResizeListRect();
	}
}

// frcombobox.cpp:132
void FrComboBox::ResizeListRect(void) {
	if (m_pListBox) {
		int num = (int)m_comboList.size();
		if (num > m_maxListNum)
			num = m_maxListNum;
		else if (num < 1)
			num = 1;

		WRect rect = m_pListBox->GetRect();
		rect.h = (float)(m_lineHeight * num + m_topMargin * 2);
		rect.y = rect.y > m_rect.y ? m_rect.y + m_rect.h + 2.0f
			: m_rect.y - rect.h - 2.0f;
		m_pListBox->SetRect(rect);
	}
}

// frcombobox.cpp:155
void FrComboBox::AddString(const char* str) {
	FrLine* pLine = new FrLine;

	pLine->text = str;

	LimitText(pLine->text);

	m_comboList.push_back(pLine);

	if (m_pListBox)
		m_pListBox->AddItem(pLine);

	ResizeListRect();
}

// frcombobox.cpp:174
void FrComboBox::DelString(int index) {
	for (std::list<FrLine*>::iterator it = m_comboList.begin(); it != m_comboList.end(); ++it) {
		if (index < 1) {
			if (m_pListBox)
				m_pListBox->DelItem(*it);

			if (m_selected == it)
				m_selected = m_comboList.end();

			m_comboList.erase(it);
			ResizeListRect();
			return;
		}
		--index;
	}
}

// frcombobox.cpp:193
int FrComboBox::FindString(const char* str) {
	int index = 1;
	for (std::list<FrLine*>::iterator it = m_comboList.begin(); it != m_comboList.end(); ++it, ++index) {
		if ((*it)->text == str)
			return index;
	}

	return 0;
}

// frcombobox.cpp:205
void FrComboBox::ClearList(void) {
	m_selected = m_comboList.end();
	sequence_delete(m_comboList.begin(), m_comboList.end());
	m_comboList.clear();

	if (m_pListBox)
		m_pListBox->ClearItem();

	ResizeListRect();
}

// frcombobox.cpp:217
bool FrComboBox::IsSelectString(const std::string& str) {
	if (m_selected == m_comboList.end())
		return false;

	return (*m_selected)->text == str ? true : false;
}

// frcombobox.cpp:246
void FrComboBox::OnKeyFocus(CChatMsg* im) {
	FrEdit::OnKeyFocus(im);

	if (im->GetConsolKeyCode() == 6) {
		if (--m_selected == m_comboList.end())
			--m_selected;

		if (m_selected != m_comboList.end()) {
			FrLine* pLine = *m_selected;
			SetLine(1, pLine->text.c_str(), 0, false, 0);
			im->SetChatText(pLine->text.c_str(), false);
		}
	}

	if (im->GetConsolKeyCode() == 7) {
		if (++m_selected == m_comboList.end())
			m_selected = m_comboList.begin();

		if (m_selected != m_comboList.end()) {
			FrLine* pLine = *m_selected;
			SetLine(1, pLine->text.c_str(), 0, false, 0);
			im->SetChatText(pLine->text.c_str(), false);
		}
	}
}

// frcombobox.cpp:277
void FrComboBox::OnUnfold(void) {
	if (m_pListBox) {
		if (m_pListBox->IsVisible())
			m_pListBox->SetVisible(false);
		else {
			m_pListBox->SetVisible(true);
			m_pListBox->SetKeyFocus(false);
		}
	}
}

// frcombobox.cpp:291
void FrComboBox::OnListOwnerDraw(int param) {
	if (param == 0) {
		const WRect& listRect = m_pListBox->GetRect();
		GDI()->Box(listRect, m_bgColor, 0, 0.0f);
		GDI()->LineBox(listRect, m_borderColor, 0, 0.0f);
		return;
	}

	FrListItem* pItem = (FrListItem*)param;
	FrLine* pLine = (FrLine*)pItem->pData;

	if (pItem->underCursor) {
		WRect rect = m_pListBox->GetRect();
		rect.x = pItem->pos.x;
		rect.y = pItem->pos.y + m_topMargin / 2;
		rect.w -= m_pScrBar->GetRect().w;
		rect.h = (float)m_lineHeight;

		unsigned long color = ~m_bgColor | 0xff000000;
		if (m_selectColor)
			color = m_selectColor;

		GDI()->Box(rect, color, 0, 0.0f);
		GDI()->SetTextColor(m_bgColor, m_fontColor2);
		GDI()->SetTextStyle(2);
		GDI()->Print(WPoint(m_leftMargin - 2.0f, m_topMargin - 2.0f) + pItem->pos, 0, pLine->text.c_str());
	} else {
		GDI()->SetTextColor(m_fontColor, m_fontColor2);
		GDI()->SetTextStyle(m_font);
		GDI()->Print(WPoint((float)m_leftMargin, (float)m_topMargin) + pItem->pos, 0, pLine->text.c_str());
	}
}

// frcombobox.cpp:343
void FrComboBox::OnListBtnDown(void) {
	FrListItem* pItem = m_pListBox->GetItemUnderCursor();
	if (pItem) {
		FrLine* pLine = (FrLine*)pItem->pData;

		for (std::list<FrLine*>::iterator it = m_comboList.begin(); it != m_comboList.end(); ++it) {
			if (pLine == *it)
				m_selected = it;
		}

		SetLine(1, pLine->text.c_str(), 0, false, 0);
		SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, (intptr_t)pItem, 0);
	}

	m_pListBox->SetVisible(false);
}

// frcombobox.cpp:363
void FrComboBox::OnListLostKeyFocus(int param) {
	WRect listRect = m_pListBox->GetRect();
	WRect buttonRect = m_pButton->GetRect();

	if (!listRect.IsInRect(((FrInputState*)param)->mousePos) && !buttonRect.IsInRect(((FrInputState*)param)->mousePos))
		m_pListBox->SetVisible(false);
}

// frcombobox.cpp:376
void FrComboBox::Enable(bool bEnable) {
	FrWnd::Enable(bEnable);
	if (m_pButton)
		m_pButton->Enable(bEnable);
}

// frcombobox.cpp:384 (run-time type and factory registration of FrComboCtlEx)
const WRTTI FrComboCtlEx::m_RTTI("FrComboCtlEx", &FrWnd::m_RTTI);

IObject* FrComboCtlExMakeInstance(void) {
	return new FrComboCtlEx;
}

struct __sFrComboCtlEx {
	__sFrComboCtlEx(void) {
		ObjectFactory().AddObjectFunctor(FrComboCtlExMakeInstance, "FrComboCtlEx");
	}
};
static __sFrComboCtlEx s_registerFrComboCtlEx;

// frcombobox.cpp:387
const sFRESH_MSGMAP* FrComboCtlEx::GetMessageMap(void) const {
	return &FrComboCtlEx::_MsgMap;
}

sFRESH_MSGMAP FrComboCtlEx::_MsgMap = { &FrCmdTarget::_MsgMap, &FrComboCtlEx::_MsgEntries[0] };

sFRESH_ENTRY FrComboCtlEx::_MsgEntries[] = {
	// frcombobox.cpp:389
	{ "unfold", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrComboCtlEx::OnUnfold },
	// frcombobox.cpp:390
	{ "unfold", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrComboCtlEx::OnUnfoldOwnerDraw },
	// frcombobox.cpp:391
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrComboCtlEx::OnListOwnerDraw },
	// frcombobox.cpp:392
	{ "list", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrComboCtlEx::OnListBtnDown },
	// frcombobox.cpp:393
	{ "list", FRCMD_LOSTKEYFOCUS, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrComboCtlEx::OnListLostKeyFocus },

	// frcombobox.cpp:395
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frcombobox.cpp:399
FrComboCtlEx::FrComboCtlEx(void) {
	memset(m_BitmapSkin, 0, sizeof(m_BitmapSkin));
	m_pSelectItem = 0;
	m_eDrawStyle = IDS_TEXT;
	m_ItemOffset.x = 0.0f;
	m_ItemOffset.y = 0.0f;
	m_byCurrentIdx = 0;
	m_bFold = true;
}

// frcombobox.cpp:410
FrComboCtlEx::~FrComboCtlEx(void) {
}

// frcombobox.cpp:415
void FrComboCtlEx::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrEdit::Init(item, pManager, pParent);

	FrElementDoc* pDoc = pManager->GetDocument();

	m_multiLine = false;

	if (item.m_param.find("liststyle") != item.m_param.end())
		sscanf(item.m_param["liststyle"].c_str(), "%d", &m_eDrawStyle);

	if (item.m_param.find("align") != item.m_param.end())
		sscanf(item.m_param["align"].c_str(), "%d", &m_align);

	m_BitmapSkin[NORMAL] = pDoc->GetBitmap(item.m_param["normal"]);
	m_BitmapSkin[OVER] = pDoc->GetBitmap(item.m_param["over"]);

	m_pButton = new FrButton;
	if (m_pButton) {
		FrGuiItem& info = m_buttonInfo;
		info = item;
		info.m_caption = "unfold";
		info.m_name = "unfold";
		info.m_rect.left = info.m_rect.top = info.m_rect.right = info.m_rect.bottom = 0;

		m_pButton->SetOwner(this);
		m_pButton->Init(info, pManager, this, true);
		m_pButton->UseDblClick(false);
		m_pButton->SetPushDelay(0.0f);
	}

	m_pListBox = new FrListBox;
	if (m_pListBox) {
		FrGuiItem& info = m_listInfo;
		info = item;
		info.m_caption = "list";
		info.m_name = "list";
		info.m_rect.right -= info.m_rect.left;
		info.m_rect.bottom -= info.m_rect.top;
		info.m_rect.left = 0;
		float height = pManager->GetDesktop()->GetRect().h;
		info.m_rect.top = m_rect.y > height * 0.5f ? -5 : 5;

		if (m_BitmapSkin[NORMAL])
			m_pListBox->SetItemSize(m_BitmapSkin[NORMAL]->Width(), m_topMargin + m_BitmapSkin[NORMAL]->Height());
		else
			m_pListBox->SetItemSize(info.m_rect.right, m_lineHeight);

		AlignListItem();

		m_pListBox->SetOwner(this);
		m_pListBox->Init(info, pManager, this);
		m_pListBox->SetTopmost(true);

		if (item.m_param.find("maxlistnum") != item.m_param.end())
			sscanf(item.m_param["maxlistnum"].c_str(), "%d", &m_maxListNum);

		if (m_eDrawStyle == IDS_IMG)
			m_TexItemList.reserve(m_maxListNum);

		m_pListBox->SetVisible(false);
		m_pListBox->SetKeyEvent(true);
		m_pListBox->EnableHover(true, 0.0f);

		WRect rt = m_pButton->GetRect();
		rt.x = m_BitmapSkin[NORMAL]->Width() + m_rect.x - rt.w;
		rt.y += (int)(float(m_rect.h - rt.h + 1.0f) * 0.5f);
		m_pButton->SetRect(rt);

		rt = m_pListBox->GetRect();
		rt.x = m_rect.x;
		rt.y = m_rect.y + m_topMargin + m_BitmapSkin[NORMAL]->Height();
		rt.w = (float)m_BitmapSkin[NORMAL]->Width();
		rt.h = (float)m_BitmapSkin[NORMAL]->Height() * m_maxListNum + m_maxListNum * m_topMargin;
		m_pListBox->SetRect(rt);

		m_rect.w = rt.w;
		m_rect.h = (float)m_BitmapSkin[NORMAL]->Height();
		SetRect(m_rect);
	}
}

// frcombobox.cpp:519
void FrComboCtlEx::AddString(const char* str) {
	if (m_eDrawStyle == IDS_IMG)
		return;

	FrLine* pLine = new FrLine;

	pLine->text = str;

	LimitText(pLine->text);

	m_comboList.push_back(pLine);

	if (m_pListBox)
		m_pListBox->AddItem(pLine);
}

// frcombobox.cpp:541
void FrComboCtlEx::AddTexItem(const char* fileName) {
	if (m_eDrawStyle == IDS_TEXT)
		return;

	FrElementDoc* pDoc = WndManager()->GetDocument();
	const Bitmap* pTex = pDoc->GetBitmap(fileName);
	if (pTex)
		m_TexItemList.push_back(pTex);

	FrLine* pItem = new FrLine;

	char buf[8];
	sprintf(buf, "%d", m_comboList.size() + 1);
	pItem->text = buf;

	m_comboList.push_back(pItem);
	if (m_pListBox)
		m_pListBox->AddItem(pItem);
}

// frcombobox.cpp:573
void FrComboCtlEx::SelectItem(int index) {
	std::vector<const Bitmap*>::iterator it = m_TexItemList.begin();
	for (int i = 0; it != m_TexItemList.end(); ++it) {
		if (i++ == index) {
			m_pSelectItem = *it;
			m_byCurrentIdx = (unsigned char)index;
			break;
		}
	}

	if (it == m_TexItemList.end()) {
		m_pSelectItem = 0;
		m_byCurrentIdx = 0;
	}
}

// frcombobox.cpp:594
void FrComboCtlEx::ClearAllListItem(void) {
	m_pListBox->ClearItem();
	typedef std::vector<const Bitmap*> TEX_ITEM_LIST;
	m_TexItemList.~TEX_ITEM_LIST();
}

// frcombobox.cpp:600
void FrComboCtlEx::AlignListItem(void) {
	switch (m_align) {
	case leftAlign:
		m_ItemOffset.x = (float)m_leftMargin;
		m_ItemOffset.y = (float)m_topMargin;
		break;

	case centerAlign:
		m_ItemOffset.x = m_pListBox->GetItemWidth() * 0.5f + m_leftMargin;
		m_ItemOffset.y = (float)m_topMargin;
		break;

	case rightAlign:
		m_ItemOffset.x = (float)(m_pListBox->GetItemWidth() - m_leftMargin);
		m_ItemOffset.y = (float)m_topMargin;
		break;
	}
}

// frcombobox.cpp:631
void FrComboCtlEx::OnListOwnerDraw(int param) {
	if (!param)
		return;

	FrListItem* pItem = (FrListItem*)param;
	FrLine* pLine = (FrLine*)pItem->pData;

	GDI()->DrawTexture(m_BitmapSkin[NORMAL], WRect(pItem->pos.x, m_ItemOffset.y + pItem->pos.y,
		m_BitmapSkin[NORMAL]->Width(), m_BitmapSkin[NORMAL]->Height()), 0xffffffff, 0);

	if (pItem->underCursor) {
		GDI()->DrawTexture(m_BitmapSkin[OVER], WRect(pItem->pos.x, m_ItemOffset.y + pItem->pos.y,
			m_BitmapSkin[OVER]->Width(), m_BitmapSkin[OVER]->Height()), 0xffffffff, 0);
	}

	GDI()->SetTextColor(m_fontColor, m_fontColor2);
	GDI()->SetTextStyle(m_font);

	if (m_eDrawStyle == IDS_TEXT) {
		pItem->pos.x += 0.5f;
		pItem->pos.y += 0.5f;
		GDI()->Print(WPoint(pItem->pos.x, pItem->pos.y), 0, pLine->text.c_str());
		return;
	}

	const Bitmap* pTex = 0;
	if (m_TexItemList.size())
		pTex = m_TexItemList[pItem->idx];

	if (pTex) {
		pItem->pos.x -= pTex->Width() / 2;
		GDI()->DrawTexture(pTex, WRect(pItem->pos.x + m_ItemOffset.x, m_ItemOffset.y + pItem->pos.y + 2.0f,
			pTex->Width(), pTex->Height()), 0xffffffff, 0);
	}
}

// frcombobox.cpp:674
void FrComboCtlEx::OnListBtnDown(void) {
	FrListItem* pItem = m_pListBox->GetItemUnderCursor();
	if (pItem) {
		if (m_eDrawStyle == IDS_IMG)
			SelectItem(pItem->idx);

		FrLine* pLine = (FrLine*)pItem->pData;

		for (std::list<FrLine*>::iterator it = m_comboList.begin(); it != m_comboList.end(); ++it) {
			if (pLine == *it)
				m_selected = it;
		}

		SetLine(1, pLine->text.c_str(), 0, false, 0);
		SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, (intptr_t)pItem, 0);

		m_bFold = true;
	}

	m_pListBox->SetVisible(false);
}

// frcombobox.cpp:699
void FrComboCtlEx::OnKeyFocus(CChatMsg* im) {
	FrEdit::OnKeyFocus(im);

	if (im->GetConsolKeyCode() == 6) {
		if (--m_selected == m_comboList.end())
			--m_selected;

		if (m_selected != m_comboList.end()) {
			FrLine* pLine = *m_selected;
			SetLine(1, pLine->text.c_str(), 0, false, 0);
			im->SetChatText(pLine->text.c_str(), false);
		}
	}

	if (im->GetConsolKeyCode() == 7) {
		if (++m_selected == m_comboList.end())
			m_selected = m_comboList.begin();

		if (m_selected != m_comboList.end()) {
			FrLine* pLine = *m_selected;
			SetLine(1, pLine->text.c_str(), 0, false, 0);
			im->SetChatText(pLine->text.c_str(), false);
		}
	}
}

// frcombobox.cpp:730
void FrComboCtlEx::OnListLostKeyFocus(int param) {
	WRect listRect = m_pListBox->GetRect();
	WRect buttonRect = m_pButton->GetRect();

	if (!listRect.IsInRect(((FrInputState*)param)->mousePos) && !buttonRect.IsInRect(((FrInputState*)param)->mousePos)) {
		m_pListBox->SetVisible(false);
		m_bFold = true;
	}
}

// frcombobox.cpp:744
void FrComboCtlEx::OnUnfold(void) {
	if (m_pListBox) {
		if (m_pListBox->IsVisible()) {
			m_pListBox->SetVisible(false);
			m_bFold = true;
		} else {
			m_pListBox->SetVisible(true);
			m_pListBox->SetKeyFocus(false);
			m_bFold = false;
		}
	}
}

// frcombobox.cpp:762
void FrComboCtlEx::OnUnfoldOwnerDraw(int param) {
	if (m_eDrawStyle == IDS_IMG) {
		if (m_pSelectItem) {
			WRect rc = m_pButton->GetRect();
			rc.x -= m_pSelectItem->Width() / 2;
			GDI()->DrawTexture(m_pSelectItem, WRect(rc.x + m_ItemOffset.x, rc.y + 2.0f, m_pSelectItem->Width(), m_pSelectItem->Height()), 0xffffffff, 0);
		}
	}
}
