// Original: d:\build\custom\temp\code\client\projectg\noticedlg.cpp
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "noticedlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frviewer.h"
#include "frwndmanager.h"

// Page numbers the "num" list box shows (statically initialized, no
// dynamic initializer).
static char* num1[10] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

// noticedlg.cpp:20 (run-time type and factory registration of FrNoticeDlg)
const WRTTI FrNoticeDlg::m_RTTI("FrNoticeDlg", &FrForm::m_RTTI);

IObject* FrNoticeDlgMakeInstance(void) {
	return new FrNoticeDlg;
}

struct __sFrNoticeDlg {
	__sFrNoticeDlg(void) {
		ObjectFactory().AddObjectFunctor(FrNoticeDlgMakeInstance, "FrNoticeDlg");
	}
};
static __sFrNoticeDlg s_registerFrNoticeDlg;

// noticedlg.cpp:22
const sFRESH_MSGMAP* FrNoticeDlg::GetMessageMap(void) const {
	return &FrNoticeDlg::_MsgMap;
}

sFRESH_MSGMAP FrNoticeDlg::_MsgMap = { &FrForm::_MsgMap, &FrNoticeDlg::_MsgEntries[0] };

sFRESH_ENTRY FrNoticeDlg::_MsgEntries[] = {
	// noticedlg.cpp:24
	{ "cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnCancelInit },
	// noticedlg.cpp:25
	{ "view", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnViewInit },

	// noticedlg.cpp:27
	{ "view", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNoticeDlg::OnViewBtnDown },
	// noticedlg.cpp:28
	{ "num", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnNumInit },
	// noticedlg.cpp:29
	{ "num", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNoticeDlg::OnNumLBtnUp },
	// noticedlg.cpp:30
	{ "num", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnNumOwnerDraw },
	// noticedlg.cpp:31
	{ "prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnPrevInit },
	// noticedlg.cpp:32
	{ "prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNoticeDlg::OnPrevBtnUp },
	// noticedlg.cpp:33
	{ "next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNoticeDlg::OnNextInit },
	// noticedlg.cpp:34
	{ "next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrNoticeDlg::OnNextBtnUp },

	// noticedlg.cpp:36
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// noticedlg.cpp:39
FrNoticeDlg::FrNoticeDlg(void) {
	m_pCancel = 0;
	m_pView = 0;
	m_page = 0;
	m_pPrev = 0;
	m_pNext = 0;
	m_pNum = 0;
	m_typeId = 0;
	m_openedPage = 0;
	m_bScroll = false;
	m_startPage = -1;
}

// noticedlg.cpp:62
FrNoticeDlg::~FrNoticeDlg(void) {
	std::vector<sNoticeImage>::iterator it;
	for (it = m_images.begin(); it != m_images.end(); ++it) {
		it->buttons.clear();
	}

	m_images.clear();
}

// noticedlg.cpp:78
void FrNoticeDlg::LoadOnly(const char* fileName) {
	if (fileName) {
		m_images.clear();

		sNoticeImage image;
		strcpy(image.name, fileName);
		m_images.push_back(image);
	}
}

// noticedlg.cpp:92
void FrNoticeDlg::LoadInitFile(const char* fileName) {
	if (fileName) {
		LoadNoticeImageFile(fileName, m_images, 10, "hg");
	}

	if (m_startPage >= 0) {
		m_page = (unsigned char)m_startPage;
		if (m_pPrev) {
			m_pPrev->SetVisible(false);
		}
		if (m_pNext) {
			m_pNext->SetVisible(false);
		}
	} else if (m_images.size() != 0) {
		m_page = 0;
	} else {
		if (m_pPrev) {
			m_pPrev->SetVisible(false);
		}
		if (m_pNext) {
			m_pNext->SetVisible(false);
		}
	}

	if (m_pPrev) {
		float x = m_pPrev->GetRect().x, y = m_pPrev->GetRect().y;
		m_pPrev->MoveWindow(WPoint(x - (m_images.size() * 15.0f + 20.0f), y));

		m_pPrev->Enable(m_page > 0);
	}

	if (m_pNext && m_images.size() != 0) {
		m_pNext->Enable(m_page < m_images.size() - 1);
	}

	if (m_pNum && m_startPage == -1) {
		for (int i = 0; i < 10; i++) {
			if (i >= 10 - m_images.size()) {
				m_pNum->AddItem(num1[i + m_images.size() - 10]);
			} else {
				m_pNum->AddItem(0);
			}
		}
	}
}

// noticedlg.cpp:149
void FrNoticeDlg::Prev(void) {
	OnPrevBtnUp();
}

// noticedlg.cpp:154
void FrNoticeDlg::Next(void) {
	OnNextBtnUp();
}

// noticedlg.cpp:159
void FrNoticeDlg::OnPrevInit(intptr_t wnd) {
	m_pPrev = DYNAMIC_CAST(FrButton, wnd);
	if (m_pPrev) {
		m_pPrev->SetPushDelay(0.0f);
	}
}

// noticedlg.cpp:168
void FrNoticeDlg::OnNextInit(intptr_t wnd) {
	m_pNext = DYNAMIC_CAST(FrButton, wnd);
	if (m_pNext) {
		m_pNext->SetPushDelay(0.0f);
	}
}

// noticedlg.cpp:177
void FrNoticeDlg::OnPrevBtnUp(void) {
	if (m_page == 0) {
		return;
	}

	m_page -= 1;
	m_openedPage = m_page;
	OpenImage();
	SetScrollBar();
}

// noticedlg.cpp:188
void FrNoticeDlg::OnNextBtnUp(void) {
	if (m_page == m_images.size() - 1) {
		return;
	}

	m_page += 1;
	m_openedPage = m_page;
	OpenImage();
	SetScrollBar();
}

// noticedlg.cpp:199
bool FrNoticeDlg::OnInit(void) {
	if (m_pView) {
		bool bScroll = false;
		float offset = (float)floor(m_pView->GetScrollBarOffset() * 0.5f);

		OpenImage();

		int width = m_pView->GetSrcWidth();
		if (width > 550) {
			width = 550;
		}
		int dx = width - (int)m_pView->GetViewWidth();

		int height = m_pView->GetSrcHeight();
		if (height > 400) {
			m_bScroll = true;
			height = 400;
			bScroll = true;
			m_pView->ShowScrollBar(true);
		} else {
			m_bScroll = false;
			m_pView->ShowScrollBar(false);
		}
		int dy = height - (int)m_pView->GetViewHeight();

		WRect rect = m_pView->GetRect();
		rect.w += dx;
		rect.h += dy;
		if (bScroll) {
			rect.w += offset * 2;
		}
		m_pView->SetRect(rect);

		if (m_pCancel) {
			rect = m_pCancel->GetRect();
			rect.x += dx;
			if (bScroll) {
				rect.x += offset * 2;
			} else {
				rect.x -= offset;
			}
			rect.x -= 1.0f;
			m_pCancel->MoveWindow(WPoint(rect.x, rect.y));
		}

		rect = GetRect();
		rect.x -= dx >> 1;
		rect.y -= dy >> 1;
		rect.w += dx;
		rect.h += dy;
		if (bScroll) {
			rect.w += offset * 2;
		} else {
			rect.w -= offset;
		}
		SetRect(rect);

		m_openedPage = m_page;
	}

	return FrForm::OnInit();
}

// noticedlg.cpp:288
void FrNoticeDlg::SetScrollBar(void) {
	bool bScroll = false;
	WRect rect;
	float offset = (float)floor(m_pView->GetScrollBarOffset() * 0.5f);

	int width = m_pView->GetSrcWidth();

	int height = m_pView->GetSrcHeight();
	if (height > 400) {
		bScroll = true;
	}

	if (m_pCancel) {
		rect = m_pCancel->GetRect();
		if (bScroll) {
			rect.x += offset * 3;
		} else if (m_bScroll) {
			rect.x -= offset * 3;
		}
		m_pCancel->MoveWindow(WPoint(rect.x, rect.y));
	}

	rect = GetRect();
	if (bScroll) {
		rect.w += offset * 3;
	} else if (m_bScroll) {
		rect.w -= offset * 3;
	}
	SetRect(rect);

	rect = m_pView->GetRect();
	if (bScroll) {
		rect.w += offset * 2;
	} else if (m_bScroll) {
		rect.w -= offset * 2;
	}
	m_pView->SetRect(rect);

	if (bScroll) {
		m_pView->ShowScrollBar(true);
		m_bScroll = true;
	} else {
		m_pView->ShowScrollBar(false);
		m_bScroll = false;
	}
}

// noticedlg.cpp:362
void FrNoticeDlg::OnCancelInit(intptr_t wnd) {
	m_pCancel = DYNAMIC_CAST(FrButton, wnd);
}

// noticedlg.cpp:368
void FrNoticeDlg::OnViewInit(intptr_t wnd) {
	m_pView = DYNAMIC_CAST(FrViewer, wnd);
}

// noticedlg.cpp:378
void FrNoticeDlg::OnViewBtnDown(void) {
	const WPoint& mousePos = g_pFresh->GetManager()->GetMousePos();
	WPoint mouse(mousePos.x, mousePos.y);

	std::list<sNoticeImage::sBn>::iterator it;
	for (it = m_images[m_page].buttons.begin(); it != m_images[m_page].buttons.end(); ++it) {
		WPoint pos = m_pView->GetRect().TopLeft();
		WRect rect(pos.x + it->x, pos.y + it->y, it->w, it->h);

		if (mouse.x >= rect.x && mouse.x <= rect.x + rect.w && mouse.y >= rect.y && mouse.y <= rect.y + rect.h) {
			m_typeId = it->index;
			Close(FrOK, true);
			return;
		}
	}

	Close(FrNONE, true);
}

// noticedlg.cpp:403
void FrNoticeDlg::OnNumInit(intptr_t wnd) {
	m_pNum = DYNAMIC_CAST(FrListBox, wnd);
}

// noticedlg.cpp:408
void FrNoticeDlg::OpenImage(void) {
	if (m_images.size() != 0 && m_pView->Open(m_images[m_page].name)) {
		if (m_pPrev) {
			m_pPrev->Enable(m_page > 0);
		}
		if (m_pNext) {
			m_pNext->Enable(m_page < m_images.size() - 1);
		}
		m_pView->SetKeyFocus(true);
	} else {
		Close(true);
	}
}

// noticedlg.cpp:426
void FrNoticeDlg::OnNumLBtnUp(void) {
	FrListItem* pItem = m_pNum->GetItemUnderCursor();
	if (pItem == 0) {
		return;
	}

	const char* text = (const char*)pItem->pData;
	if (text == 0) {
		return;
	}

	m_page = atoi(text) - 1;
	if (m_openedPage == m_page) {
		return;
	}

	m_openedPage = m_page;
	OpenImage();
	SetScrollBar();
}

// noticedlg.cpp:448
void FrNoticeDlg::OnNumOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;
	if (pItem == 0) {
		return;
	}

	const char* text = (const char*)pItem->pData;
	if (text == 0) {
		return;
	}

	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI == 0) {
		return;
	}

	int page = atoi(text);

	if (m_page == page - 1) {
		pGI->SetTextColor(0xffff0000, 0xffffffff);
		pGI->SetTextStyle(1);
	} else {
		pGI->SetTextColor(0xff000000, 0xffffffff);
		pGI->SetTextStyle(0);
	}

	pGI->Print(pItem->pos, 0, text);
}
