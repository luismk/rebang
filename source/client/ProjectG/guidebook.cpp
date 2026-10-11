// Original: d:\build\custom\temp\code\client\projectg\guidebook.cpp
#include "guidebook.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "frtextbutton.h"
#include "mousecursor.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"

// guidebook.cpp:19 (run-time type and factory registration of FrGuideBookDlg)
const WRTTI FrGuideBookDlg::m_RTTI("FrGuideBookDlg", &FrForm::m_RTTI);

IObject* FrGuideBookDlgMakeInstance(void) {
	return new FrGuideBookDlg;
}

struct __sFrGuideBookDlg {
	__sFrGuideBookDlg(void) {
		ObjectFactory().AddObjectFunctor(FrGuideBookDlgMakeInstance, "FrGuideBookDlg");
	}
};
static __sFrGuideBookDlg s_registerFrGuideBookDlg;

// guidebook.cpp:21
const sFRESH_MSGMAP* FrGuideBookDlg::GetMessageMap(void) const {
	return &FrGuideBookDlg::_MsgMap;
}

sFRESH_MSGMAP FrGuideBookDlg::_MsgMap = { &FrForm::_MsgMap, &FrGuideBookDlg::_MsgEntries[0] };

sFRESH_ENTRY FrGuideBookDlg::_MsgEntries[] = {
	// guidebook.cpp:23
	{ "content", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnContentInit },
	// guidebook.cpp:24
	{ "content", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnContentOwnerDraw },
	// guidebook.cpp:25
	{ "prev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnPrevInit },
	// guidebook.cpp:26
	{ "prev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnPrevBtnUp },
	// guidebook.cpp:27
	{ "next", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnNextInit },
	// guidebook.cpp:28
	{ "next", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnNextBtnUp },
	// guidebook.cpp:29
	{ "tip1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip1Init },
	// guidebook.cpp:30
	{ "tip2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip2Init },
	// guidebook.cpp:31
	{ "tip3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip3Init },
	// guidebook.cpp:32
	{ "tip4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip4Init },
	// guidebook.cpp:33
	{ "tip5", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip5Init },
	// guidebook.cpp:34
	{ "tip6", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip6Init },
	// guidebook.cpp:35
	{ "tip7", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip7Init },
	// guidebook.cpp:36
	{ "tip8", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip8Init },
	// guidebook.cpp:37
	{ "tip9", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrGuideBookDlg::OnTip9Init },
	// guidebook.cpp:38
	{ "tip1", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip1BtnUp },
	// guidebook.cpp:39
	{ "tip2", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip2BtnUp },
	// guidebook.cpp:40
	{ "tip3", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip3BtnUp },
	// guidebook.cpp:41
	{ "tip4", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip4BtnUp },
	// guidebook.cpp:42
	{ "tip5", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip5BtnUp },
	// guidebook.cpp:43
	{ "tip6", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip6BtnUp },
	// guidebook.cpp:44
	{ "tip7", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip7BtnUp },
	// guidebook.cpp:45
	{ "tip8", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip8BtnUp },
	// guidebook.cpp:46
	{ "tip9", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrGuideBookDlg::OnTip9BtnUp },

	// guidebook.cpp:48
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// guidebook.cpp:54
FrGuideBookDlg::FrGuideBookDlg(void)
	: m_pPrev(0),
	  m_pNext(0) {
	for (int i = 0; i < 9; i++) {
		m_pTip[i] = 0;
	}

	m_pPage = 0;
}

// guidebook.cpp:67
FrGuideBookDlg::~FrGuideBookDlg(void) {
	if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }
}

// guidebook.cpp:73
void FrGuideBookDlg::OpenChapter(int chapter) {
	if (m_pPrev)
		m_pPrev->Enable(chapter > 1);

	for (unsigned int i = 0; i < 9; i++) {
		if (i + 1 != chapter)
			m_pTip[i]->Unselect(0);
	}

	if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }
	m_pPage = g_resrcmng->GetOverlay(MakeStr("%d_%d.tga", chapter, 1), 0x80000000);
	if (m_pPage) {
		m_chapter = chapter;
		m_page = 1;
	}
}

// guidebook.cpp:98
void FrGuideBookDlg::OnContentInit(intptr_t param) {
	m_pContent = DYNAMIC_CAST(FrArea, param);
}

// guidebook.cpp:104
void FrGuideBookDlg::OnContentOwnerDraw(int param) {
	if (m_pPage) {
		WRect src(0.0f, 0.0f, 1.0f, 1.0f);
		WRect dst = m_pContent->GetRect();
		dst.x += (int)((dst.w - m_pPage->GetWidth() + 1.0f) * 0.5f);
		dst.y += (int)((dst.h - m_pPage->GetHeight() + 1.0f) * 0.5f);
		dst.w = (float)m_pPage->GetWidth();
		dst.h = (float)m_pPage->GetHeight();

		m_pPage->Render(g_view, src, dst, 0, 0xffffffff, 0.0f, 0);
	}
}

// guidebook.cpp:120
void FrGuideBookDlg::OnPrevInit(intptr_t param) {
	m_pPrev = DYNAMIC_CAST(FrButton, param);
	m_pPrev->Enable(false);

	m_chapter = 1;
	m_page = 1;

	if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }
	m_pPage = g_resrcmng->GetOverlay(MakeStr("%d_%d.tga", m_chapter, m_page), 0x80000000);
}

// guidebook.cpp:133
void FrGuideBookDlg::OnPrevBtnUp(void) {
	int chapter = m_chapter;
	int page = m_page - 1;
	while (chapter > 0) {
		for (; page > 0; page--) {
			if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }
			m_pPage = g_resrcmng->GetOverlay(MakeStr("%d_%d.tga", chapter, page), 0x80000000);
			if (m_pPage) {
				m_pNext->Enable(true);
				m_chapter = chapter;
				m_page = page;

				for (unsigned int i = 0; i < 9; i++) {
					if (i + 1 == chapter)
						m_pTip[i]->Select(0);
					else
						m_pTip[i]->Unselect(0);
				}

				if (chapter == 1 && page == 1) {
					if (m_pPrev)
						m_pPrev->Enable(false);
				}
				return;
			}
		}

		chapter--;

		page = 1;
	}
}

// guidebook.cpp:178
void FrGuideBookDlg::OnNextInit(intptr_t param) {
	m_pNext = DYNAMIC_CAST(FrButton, param);
}

// guidebook.cpp:184
void FrGuideBookDlg::OnNextBtnUp(void) {
	WOverlay* pPage = g_resrcmng->GetOverlay(MakeStr("%d_%d.tga", m_chapter, m_page + 1), 0x80000000);
	if (pPage) {
		if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }

		m_pPage = pPage;
		m_pPrev->Enable(true);
		m_page++;
		return;
	}

	pPage = g_resrcmng->GetOverlay(MakeStr("%d_%d.tga", m_chapter + 1, 1), 0x80000000);
	if (pPage) {
		if (g_resrcmng && m_pPage) { g_resrcmng->Release(m_pPage); m_pPage = 0; }

		m_pPage = pPage;
		m_pPrev->Enable(true);
		m_chapter += 1;
		m_page = 1;

		for (unsigned int i = 0; i < 9; i++) {
			if (i + 1 == m_chapter)
				m_pTip[i]->Select(0);
			else
				m_pTip[i]->Unselect(0);
		}
		return;
	}

	m_pNext->Enable(false);
}

// guidebook.cpp:225
void FrGuideBookDlg::OnTip1Init(intptr_t param) {
	m_pTip[0] = DYNAMIC_CAST(FrTextButton, param);
	m_pTip[0]->Select(0);
}

// guidebook.cpp:231
void FrGuideBookDlg::OnTip2Init(intptr_t param) {
	m_pTip[1] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:236
void FrGuideBookDlg::OnTip3Init(intptr_t param) {
	m_pTip[2] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:241
void FrGuideBookDlg::OnTip4Init(intptr_t param) {
	m_pTip[3] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:246
void FrGuideBookDlg::OnTip5Init(intptr_t param) {
	m_pTip[4] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:251
void FrGuideBookDlg::OnTip6Init(intptr_t param) {
	m_pTip[5] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:256
void FrGuideBookDlg::OnTip7Init(intptr_t param) {
	m_pTip[6] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:261
void FrGuideBookDlg::OnTip8Init(intptr_t param) {
	m_pTip[7] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:266
void FrGuideBookDlg::OnTip9Init(intptr_t param) {
	m_pTip[8] = DYNAMIC_CAST(FrTextButton, param);
}

// guidebook.cpp:272
void FrGuideBookDlg::OnTip1BtnUp(void) {
	OpenChapter(1);
}

// guidebook.cpp:277
void FrGuideBookDlg::OnTip2BtnUp(void) {
	OpenChapter(2);
}

// guidebook.cpp:282
void FrGuideBookDlg::OnTip3BtnUp(void) {
	OpenChapter(3);
}

// guidebook.cpp:287
void FrGuideBookDlg::OnTip4BtnUp(void) {
	OpenChapter(4);
}

// guidebook.cpp:292
void FrGuideBookDlg::OnTip5BtnUp(void) {
	OpenChapter(5);
}

// guidebook.cpp:297
void FrGuideBookDlg::OnTip6BtnUp(void) {
	OpenChapter(6);
}

// guidebook.cpp:302
void FrGuideBookDlg::OnTip7BtnUp(void) {
	OpenChapter(7);
}

// guidebook.cpp:307
void FrGuideBookDlg::OnTip8BtnUp(void) {
	OpenChapter(8);
}

// guidebook.cpp:312
void FrGuideBookDlg::OnTip9BtnUp(void) {
	OpenChapter(9);
}

// guidebook.cpp:319 (run-time type and factory registration of CGuideBook)
const WRTTI CGuideBook::m_RTTI("CGuideBook", &IActor::m_RTTI);

IObject* CGuideBookMakeInstance(void) {
	return new CGuideBook;
}

struct __impCGuideBook {
	__impCGuideBook(void) {
		ObjectFactory().AddObjectFunctor(CGuideBookMakeInstance, "CGuideBook");
		ActorFactory().AddActorClass("CGuideBook");
	}
};
static __impCGuideBook s_registerCGuideBook;

// guidebook.cpp:323
CGuideBook::CGuideBook(void) {
	m_pDlg = 0;
	m_priority = 4;
}

// guidebook.cpp:329
void CGuideBook::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x12a:
		if (m_pDlg == 0) {
			m_pDlg = CreateForm<FrGuideBookDlg>(g_pFresh->GetManager(), this, "guide_book", 0);
			if (m_pDlg) {
				m_pDlg->Open((FRESH_RESULT_CALLBACK)&CGuideBook::OnDlgResult, 1);
				m_pDlg->EnableDrag(false);
			}

			CMouseCursor::Instance()->m_mode = 1;
			CMouseCursor::Instance()->m_uiEnable[1] = false;
		}
		break;

	case 0x12b:
		if (m_pDlg)
			m_pDlg->Close(FrOK, true);
		break;

	case 0x12c:
		*(bool*)msg.m_param1 = m_pDlg != 0;
		break;
	}
}

// guidebook.cpp:363
bool CGuideBook::OnDlgResult(int ret, FrForm* pForm) {
	m_pDlg = 0;
	CMouseCursor::Instance()->m_uiEnable[1] = true;

	return true;
}
