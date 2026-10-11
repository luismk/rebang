// Original: d:\build\custom\temp\code\client\fresh\source\frlistbox.cpp
#include <stdio.h>
#include <string.h>

#include "frlistbox.h"
#include "ObjectFactory.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frscrollbar.h"
#include "frwndmanager.h"
#include "../client/ProjectG/ChatMsg.h"
#include "../client/ProjectG/commonutil.h"

// frlistbox.cpp:16 (run-time type and factory registration of FrListBox)
const WRTTI FrListBox::m_RTTI("FrListBox", &FrWnd::m_RTTI);

IObject* FrListBoxMakeInstance(void) {
	return new FrListBox;
}

struct __sFrListBox {
	__sFrListBox(void) {
		ObjectFactory().AddObjectFunctor(FrListBoxMakeInstance, "FrListBox");
	}
};
static __sFrListBox s_registerFrListBox;

// frlistbox.cpp:18
const sFRESH_MSGMAP* FrListBox::GetMessageMap(void) const {
	return &FrListBox::_MsgMap;
}

sFRESH_MSGMAP FrListBox::_MsgMap = { &FrCmdTarget::_MsgMap, &FrListBox::_MsgEntries[0] };

sFRESH_ENTRY FrListBox::_MsgEntries[] = {
	// frlistbox.cpp:21
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frlistbox.cpp:29
FrListBox::FrListBox(void) {
	m_pItem = 0;
	m_pItemUnderCursor = 0;
	m_pSelected = 0;
	m_multiSelect = false;
	m_useRightButton = false;
	m_dummyCount = 0;
	m_useDummy = false;
	m_pPreviousSelected = 0;
	m_listBgImg = 0;
	m_rollOver = true;
	m_szPushSound = "ui_icon_click";
}

// frlistbox.cpp:48
FrListBox::~FrListBox(void) {
	ClearItem();

	m_noRButtonMap.clear();
}

// frlistbox.cpp:55
void FrListBox::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrElementDoc* pDoc = pManager->GetDocument();
	std::map<std::string, std::string>& param = item.m_param;
	if (param.find("itemsize") != param.end())
		sscanf(param["itemsize"].c_str(), "%d %d", &m_itemSize.w, &m_itemSize.h);

	if (param.find("column") != param.end())
		sscanf(param["column"].c_str(), "%d", &m_colCapacity);

	bool barOn = true;
	if (param.find("show_scrollbar") != param.end()) {
		int show;
		sscanf(param["show_scrollbar"].c_str(), "%d", &show);
		barOn = show != 0;
	}

	if (item.m_param.find("listbg") != item.m_param.end())
		m_listBgImg = pDoc->GetBitmap(item.m_param["listbg"]);

	bool useKey = false;
	if (param.find("use_key") != param.end())
		useKey = strcmpi(param["use_key"].c_str(), "true") == 0;

	m_pItem = &item;

	unsigned long keyFlag = useKey ? 0x80 : 0;

	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 0x1 | keyFlag,
		WRect((float)item.m_rect.left, (float)item.m_rect.top, (short)(item.m_rect.right - item.m_rect.left),
			(short)(item.m_rect.bottom - item.m_rect.top)),
		pParent);

	m_prevTopRow = 0;
	int tf = m_itemSize.h ? (int)m_rect.h / m_itemSize.h : 0;

	if (m_pScrBar == 0) {
		m_pScrBar = new FrScrollBar;
		m_pScrBar->Init(pManager, this, 0, tf, m_colCapacity, false);
		EnableScrollBar(barOn);
		m_pScrBar->Resize(tf, m_colCapacity, false);
		m_pScrBar->FollowBottom(false);
	}

	RecalcTopBottom();
}

// frlistbox.cpp:128
void FrListBox::Resize(int item_w, int item_h) {
	m_itemSize.w = item_w;
	m_itemSize.h = item_h;

	int scrollBarW = (m_pScrBar && m_pScrBar->IsVisible()) ? (int)m_pScrBar->GetRect().w : 0;
	int colCapacity = m_itemSize.w ? ((int)m_rect.w - scrollBarW) / m_itemSize.w : 1;
	m_colCapacity = colCapacity < 1 ? 1 : colCapacity;
	RecalcTopBottom();

	OnResize();
}

// frlistbox.cpp:139
void FrListBox::RecalcColCapacity(bool update) {
	int scrollBarW = (m_pScrBar && m_pScrBar->IsVisible()) ? (int)m_pScrBar->GetRect().w : 0;
	int colCapacity = m_itemSize.w ? ((int)m_rect.w - scrollBarW) / m_itemSize.w : 1;

	if (!update) {
		colCapacity = colCapacity > m_colCapacity ? m_colCapacity : colCapacity;
	}
	m_colCapacity = colCapacity < 1 ? 1 : colCapacity;
}

// frlistbox.cpp:151
const Bitmap* FrListBox::GetBitmap(const char* name) {
	FrElementDoc* pDoc = m_pWndManager->GetDocument();
	return pDoc->GetBitmap(name);
}

// frlistbox.cpp:159
void FrListBox::SelectItem(FrListItem* pItem, bool setSelected) {
	if (pItem) {
		pItem->selected = true;
	}
	if (!m_multiSelect && m_pSelected && m_pSelected != pItem) {
		m_pSelected->selected = false;
	}
	if (setSelected) {
		m_pPreviousSelected = m_pSelected;
		m_pSelected = pItem;

		int pos = -1;
		int no = 0;
		for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i, ++no) {
			if (i == m_itemTop) {
				pos = 0;
			}
			if (i == m_itemBottom) {
				pos = 1;
			}
			if (*i == pItem) {
				if (pos == -1) {
					int colCapacity = m_pScrBar->GetColCapacity();
					int rowCapacity = m_pScrBar->GetRowCapacity();

					int pageSize = rowCapacity * colCapacity;
					if (pageSize) {
						int halfPage = pageSize / 2;
						if (halfPage) {
							int count = (m_pScrBar->GetCurTopRow_Int() * colCapacity - no + halfPage - 1) / halfPage;
							for (int j = 0; j < count; ++j) {
								m_pScrBar->ScrollUp(-1);
							}
						}
					}
				} else if (pos == 1) {
					int colCapacity = m_pScrBar->GetColCapacity();
					int rowCapacity = m_pScrBar->GetRowCapacity();

					int pageSize = rowCapacity * colCapacity;
					if (pageSize) {
						int halfPage = pageSize / 2;
						if (halfPage) {
							int count = (no - m_pScrBar->GetCurTopRow_Int() * colCapacity - halfPage) / halfPage;
							for (int j = 0; j < count; ++j) {
								m_pScrBar->ScrollDown(-1);
							}
						}
					}
				}
				break;
			}
		}
	}
}

// frlistbox.cpp:228
void FrListBox::UnselectItem(FrListItem* pItem) {
	if (pItem) {
		pItem->selected = false;
		if (m_pSelected == pItem) {
			m_pSelected = 0;
		}
	}
}

// frlistbox.cpp:239
void FrListBox::UnselectAllItem(void) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		UnselectItem(*i);
	}
}

// frlistbox.cpp:247
void FrListBox::ToggleItem(FrListItem* pItem) {
	if (pItem->selected) {
		UnselectItem(pItem);
	} else {
		SelectItem(pItem, true);
	}
}

// frlistbox.cpp:255
FrListItem* FrListBox::AddItem(void* pData) {
	FrListItem* pItem = new FrListItem;
	pItem->pData = pData;
	pItem->idx = m_itemList.size();

	// An item without data is a dummy row.
	if (!pData)
		m_dummyCount++;

	m_itemList.push_back(pItem);

	if (m_pScrBar)
		m_pScrBar->AddItem();

	RecalcTopBottom();

	return pItem;
}

// frlistbox.cpp:273
FrListItem* FrListBox::FindItem(FrListItem* pSelected) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		if (*i == pSelected) {
			return *i;
		}
	}
	return 0;
}

// frlistbox.cpp:284
FrListItem* FrListBox::FindItem(void* pData) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		if ((*i)->pData == pData) {
			return *i;
		}
	}
	return 0;
}

// frlistbox.cpp:295
FrListItem* FrListBox::FindItem(void* pData, int cmpLen) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		if ((*i)->pData) {
			if (memcmp((*i)->pData, pData, cmpLen) == 0) {
				return *i;
			}
		}
	}
	return 0;
}

// frlistbox.cpp:309
FrListItem* FrListBox::GetItem(int iLineNum) {
	int lineNum = 0;
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i, ++lineNum) {
		if ((*i)->pData) {
			if (iLineNum == lineNum) {
				return *i;
			}
		}
	}
	return 0;
}

// frlistbox.cpp:324
FrListBox::ITEM_LIST::iterator FrListBox::_DelItem(ITEM_LIST::iterator i) {
	FrListItem* pItem = *i;

	if (!pItem->pData) {
		--m_dummyCount;
	}
	if (pItem == m_pItemUnderCursor) {
		m_pItemUnderCursor = 0;
	}
	if (pItem == m_pSelected) {
		if (!m_multiSelect) {
			ITEM_LIST::iterator next = i;
			++next;
			if (next != m_itemList.end()) {
				SelectItem(*next, true);
			} else {
				--next;
				if (next != m_itemList.begin()) {
					--next;
					SelectItem(*next, true);
				} else {
					m_pSelected = 0;
				}
			}
		} else {
			m_pSelected = 0;
		}
	}

	delete pItem;
	i = m_itemList.erase(i);

	if (m_pScrBar) {
		m_pScrBar->DelItem();
	}
	RecalcTopBottom();
	return i;
}

// frlistbox.cpp:377
void FrListBox::DelItem(FrListItem* pSelected) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		if (*i == pSelected) {
			_DelItem(i);
			return;
		}
	}
}

// frlistbox.cpp:389
void FrListBox::DelItem(void* pData) {
	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		if ((*i)->pData == pData) {
			_DelItem(i);
			return;
		}
	}
}

// frlistbox.cpp:401
void FrListBox::DelItem(void* pData, int cmpLen) {
	if (!pData) {
		return;
	}

	for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
		FrListItem* pItem = *i;

		if (pItem->pData) {
			if (memcmp(pItem->pData, pData, cmpLen) == 0) {
				_DelItem(i);
				return;
			}
		}
	}
}

// frlistbox.cpp:421
void FrListBox::ClearItem(void) {
	if (m_itemList.size()) {
		// Deletes every item, then empties the list.
		sequence_delete(m_itemList.begin(), m_itemList.end());
		m_itemList.clear();
	}

	m_pItemUnderCursor = 0;
	m_pSelected = 0;

	if (m_pScrBar) {
		m_pScrBar->ClearItem();
	}

	RecalcTopBottom();
}

// frlistbox.cpp:439
void FrListBox::SortItem(bool (*compare)(const void*, const void*)) {
	if (compare) {
		m_itemList.sort(FrListBoxGreater(compare));
		RecalcTopBottom();
	}
}

// frlistbox.cpp:448
void FrListBox::RecalcTopBottom(void) {
	int skip = m_pScrBar ? m_pScrBar->GetCurTopRow_Int() * m_colCapacity : 0;

	ITEM_LIST::iterator i = m_itemList.begin();
	while (skip > 0 && i != m_itemList.end()) {
		++i;
		--skip;
	}

	m_itemTop = i;
	int x = 0;
	int y = 0;
	for (; i != m_itemList.end(); ++i) {
		if (x / m_itemSize.w >= m_colCapacity) {
			y += m_itemSize.h;
			x = 0;

			if (y + m_itemSize.h > m_rect.h) {
				break;
			}
		}
		x += m_itemSize.w;
	}

	m_itemBottom = i;
}

// frlistbox.cpp:482
void FrListBox::EnableScrollBar(bool enable) {
	if (m_pScrBar) {
		m_pScrBar->SetVisible(enable);
		m_pScrBar->Enable(enable);
	}
	RecalcColCapacity(false);
}

// frlistbox.cpp:492
void FrListBox::SetCursorToFirstItem(void) {
	ITEM_LIST::iterator i = m_itemList.begin();
	if (i != m_itemList.end()) {
		FrListItem* pItem = *i;
		pItem->underCursor = true;
		m_pItemUnderCursor = pItem;
	}
}

// frlistbox.cpp:504
void FrListBox::SetCursorToLastItem(void) {
	ITEM_LIST::iterator i = m_itemList.end();
	if (--i != m_itemList.end()) {
		FrListItem* pItem = *i;
		pItem->underCursor = true;
		m_pItemUnderCursor = pItem;
	}
}

// frlistbox.cpp:516
void FrListBox::OnDraw(void) {
	FrWnd::OnDraw();

	if (!m_pItem) {
		return;
	}

	if (m_listBgImg) {
		float fOldAlpha = GDI()->GetAlpha();
		GDI()->SetAlpha(GetAlpha());

		GDI()->DrawTexture(m_listBgImg, m_rect, 0xffffffff, 0);

		GDI()->SetAlpha(fOldAlpha);
	}

	SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);

	int xPos = 0;
	int yPos = 0;
	int count = 0;
	for (ITEM_LIST::iterator i = m_itemTop; i != m_itemBottom && i != m_itemList.end(); ++i) {
		if (!m_itemSize.w || xPos / m_itemSize.w >= m_colCapacity) {
			xPos = 0;
			yPos += m_itemSize.h;
		}
		(*i)->no = count++;
		(*i)->pos = WPoint(m_rect.x, m_rect.y) + WPoint((float)xPos, (float)yPos);
		xPos += m_itemSize.w;
		SendCmdToOwnerTarget(FRCMD_OWNERDRAW, (intptr_t)*i, 0);
	}
}

// frlistbox.cpp:560
void FrListBox::OnProc(const float deltaTime) {
	if (m_pScrBar && m_prevTopRow != m_pScrBar->GetCurTopRow_Int()) {
		m_prevTopRow = m_pScrBar->GetCurTopRow_Int();
		RecalcTopBottom();
	}
}

// frlistbox.cpp:569
void FrListBox::SetClientRect(const WRect& rect) {
	FrWnd::SetClientRect(rect);
	OnResize();
}

// frlistbox.cpp:575
void FrListBox::OnResize(void) {
	RecalcTopBottom();

	if (m_pScrBar) {
		int rowCapacity = m_itemSize.h ? (int)m_rect.h / m_itemSize.h : 0;
		m_pScrBar->Resize(rowCapacity, m_colCapacity, false);
	}
}

// frlistbox.cpp:586
int FrListBox::GetCurrentItemSize(bool bDummy) {
	if (bDummy) {
		return m_itemList.size();
	} else {
		return m_itemList.size() - m_dummyCount;
	}
}

// frlistbox.cpp:594
void FrListBox::OnMouseMove(const WPoint& mousePos) {
	bool found = false;

	for (ITEM_LIST::iterator i = m_itemTop; i != m_itemBottom && i != m_itemList.end(); ++i) {
		if ((*i)->pos.x <= mousePos.x && mousePos.x < (*i)->pos.x + m_itemSize.w &&
			(*i)->pos.y <= mousePos.y && mousePos.y < (*i)->pos.y + m_itemSize.h) {
			(*i)->underCursor = true;
			m_pItemUnderCursor = *i;

			if (!m_useDummy || (*i)->pData) {
				found = true;
			}
		} else {
			(*i)->underCursor = false;
		}
	}

	SendCmdToOwnerTarget(FRCMD_MOUSEMOVE, 0, 0);

	if (!found) {
		m_pItemUnderCursor = 0;
	}
}

// frlistbox.cpp:629
void FrListBox::AddNoRButtonRect(const WRect& rect, int index) {
	m_noRButtonMap[index] = rect;
}

// frlistbox.cpp:634
void FrListBox::OnSetCursor(bool bInClient, const WPoint& mousePos) {
	if (bInClient) {
		if (m_pItemUnderCursor && m_rollOver) {
			if (m_useRightButton) {
				std::list<int>::iterator i;
				for (i = m_pItemUnderCursor->noRBtnIdxList.begin();
					i != m_pItemUnderCursor->noRBtnIdxList.end(); ++i) {
					WRect rect;
					rect = m_noRButtonMap[*i];

					if (mousePos.x > rect.x + m_pItemUnderCursor->pos.x &&
						mousePos.x < rect.w + m_pItemUnderCursor->pos.x + rect.x &&
						mousePos.y > rect.y + m_pItemUnderCursor->pos.y &&
						mousePos.y < rect.h + m_pItemUnderCursor->pos.y + rect.y) {
						SetCursor(1);
						return;
					}
				}
				SetCursor(7);
			} else {
				SetCursor(1);
			}
		}
	} else if (!m_dwStyle.GetFlag(0x80) && m_pItemUnderCursor) {
		m_pItemUnderCursor->underCursor = false;
		m_pItemUnderCursor = 0;
	}
}

// frlistbox.cpp:671
bool FrListBox::OnLButtonDown(const WPoint& mousePos) {
	OnMouseMove(mousePos);

	if (m_pItemUnderCursor) {
		ToggleItem(m_pItemUnderCursor);
		PlayPushSound();
	}

	SendCmdToOwnerTarget(FRCMD_LBUTTONDOWN, 0, 0);
	return false;
}

// frlistbox.cpp:685
bool FrListBox::OnLButtonUp(const WPoint& mousePos) {
	OnMouseMove(mousePos);
	if (m_pItemUnderCursor) {
		SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);
	}
	return false;
}

// frlistbox.cpp:693
bool FrListBox::OnRButtonDown(const WPoint& mousePos) {
	if (m_useRightButton) {
		OnMouseMove(mousePos);

		if (m_pItemUnderCursor) {
			ToggleItem(m_pItemUnderCursor);
			PlayPushSound();
		}

		SendCmdToOwnerTarget(FRCMD_RBUTTONDOWN, 0, 0);
	}
	return false;
}

// frlistbox.cpp:710
bool FrListBox::OnRButtonUp(const WPoint& mousePos) {
	if (m_useRightButton) {
		OnMouseMove(mousePos);
		if (m_pItemUnderCursor) {
			SendCmdToOwnerTarget(FRCMD_RBUTTONUP, 0, 0);
		}
	}
	return false;
}

// frlistbox.cpp:723
void FrListBox::OnDblClick(const WPoint& mousePos) {
	OnMouseMove(mousePos);
	SendCmdToOwnerTarget(FRCMD_DBLCLICK, 0, 0);
}

// frlistbox.cpp:729
void FrListBox::EnableKeyFocus(FrInputState& istate) {
	FrWnd::EnableKeyFocus(istate);
	if (!m_pItemUnderCursor) {
		SetCursorToFirstItem();
	}
}

// frlistbox.cpp:737
void FrListBox::OnKeyFocus(CChatMsg* im) {
	if (!im->IsActive()) {
		return;
	}

	im->Process(0.01f);

	if (im->GetConsolKeyCode() == 4) {
		if (!m_pItemUnderCursor) {
			SetCursorToFirstItem();
		} else {
			for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
				if (*i == m_pItemUnderCursor) {
					--i;
					if (i != m_itemList.end()) {
						FrListItem* pItem = *i;
						m_pItemUnderCursor->underCursor = false;
						pItem->underCursor = true;
						m_pItemUnderCursor = pItem;
					}
					break;
				}
			}
		}
	}

	if (im->GetConsolKeyCode() == 5) {
		if (!m_pItemUnderCursor) {
			SetCursorToFirstItem();
		} else {
			for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
				if (*i == m_pItemUnderCursor) {
					++i;
					if (i != m_itemList.end()) {
						FrListItem* pItem = *i;
						m_pItemUnderCursor->underCursor = false;
						pItem->underCursor = true;
						m_pItemUnderCursor = pItem;
					}
					break;
				}
			}
		}
	}

	if (im->GetConsolKeyCode() == 6) {
		if (!m_pItemUnderCursor) {
			SetCursorToFirstItem();
		} else {
			for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
				if (*i == m_pItemUnderCursor) {
					for (int n = 0; i != m_itemList.end() && n < m_colCapacity; --i, ++n) {
					}
					if (i != m_itemList.end()) {
						FrListItem* pItem = *i;
						m_pItemUnderCursor->underCursor = false;
						pItem->underCursor = true;
						m_pItemUnderCursor = pItem;
					}
					break;
				}
			}
		}
	}

	if (im->GetConsolKeyCode() == 7) {
		if (!m_pItemUnderCursor) {
			SetCursorToFirstItem();
		} else {
			for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
				if (*i == m_pItemUnderCursor) {
					for (int n = 0; i != m_itemList.end() && n < m_colCapacity; ++i, ++n) {
					}
					if (i != m_itemList.end()) {
						FrListItem* pItem = *i;
						m_pItemUnderCursor->underCursor = false;
						pItem->underCursor = true;
						m_pItemUnderCursor = pItem;
					}
					break;
				}
			}
		}
	}
}

// frlistbox.cpp:857
bool FrListBox::MoveTopItem(int dir) {
	bool moved = true;
	ITEM_LIST::iterator i = m_itemTop;

	if (dir > 0) {
		if (++i == m_itemList.end()) {
			return false;
		}
	} else {
		if (i == m_itemList.begin()) {
			return false;
		}
		--i;
		if (i == m_itemList.begin()) {
			moved = false;
		}
	}

	m_itemTop = i;
	int x = 0;
	int y = 0;
	for (; i != m_itemList.end(); ++i) {
		if (x / m_itemSize.w >= m_colCapacity) {
			y += m_itemSize.h;
			x = 0;

			if (y + m_itemSize.h > m_rect.h) {
				break;
			}
		}
		x += m_itemSize.w;
	}
	m_itemBottom = i;

	if (dir > 0 && i == m_itemList.end()) {
		moved = false;
	}
	return moved;
}

// frlistbox.cpp:911
void FrListBox::SetMouseEvent(bool active) {
	if (active) {
		m_dwStyle.Disable(0x40);
	} else {
		m_dwStyle.Enable(0x40);
	}
}
