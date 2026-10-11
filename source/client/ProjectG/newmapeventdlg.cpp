// Original: d:\build\custom\temp\code\client\projectg\newmapeventdlg.cpp
#include "NewMapEventDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "SharedDoc.h"
#include "bitmap.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"

// newmapeventdlg.cpp:18 (run-time type and factory registration of FrNewMapEventDlg)
const WRTTI FrNewMapEventDlg::m_RTTI("FrNewMapEventDlg", &FrForm::m_RTTI);

IObject* FrNewMapEventDlgMakeInstance(void) {
	return new FrNewMapEventDlg;
}

struct __sFrNewMapEventDlg {
	__sFrNewMapEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrNewMapEventDlgMakeInstance, "FrNewMapEventDlg");
	}
};
static __sFrNewMapEventDlg s_registerFrNewMapEventDlg;

// newmapeventdlg.cpp:20
const sFRESH_MSGMAP* FrNewMapEventDlg::GetMessageMap(void) const {
	return &FrNewMapEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrNewMapEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrNewMapEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrNewMapEventDlg::_MsgEntries[] = {
	// newmapeventdlg.cpp:22
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnCaptionInit },
	// newmapeventdlg.cpp:23
	{ "info1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnInfo1Init },
	// newmapeventdlg.cpp:24
	{ "score0", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore0Init },
	// newmapeventdlg.cpp:25
	{ "score1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore1Init },
	// newmapeventdlg.cpp:26
	{ "score2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore2Init },
	// newmapeventdlg.cpp:27
	{ "score3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore3Init },
	// newmapeventdlg.cpp:28
	{ "score4", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore4Init },
	// newmapeventdlg.cpp:29
	{ "score5", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewMapEventDlg::OnScore5Init },
	// newmapeventdlg.cpp:30
	{ "score0", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore0OwnerDraw },
	// newmapeventdlg.cpp:31
	{ "score1", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore1OwnerDraw },
	// newmapeventdlg.cpp:32
	{ "score2", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore2OwnerDraw },
	// newmapeventdlg.cpp:33
	{ "score3", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore3OwnerDraw },
	// newmapeventdlg.cpp:34
	{ "score4", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore4OwnerDraw },
	// newmapeventdlg.cpp:35
	{ "score5", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrNewMapEventDlg::OnScore5OwnerDraw },

	// newmapeventdlg.cpp:37
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// newmapeventdlg.cpp:40
FrNewMapEventDlg::FrNewMapEventDlg(void) {
}

// newmapeventdlg.cpp:44
FrNewMapEventDlg::~FrNewMapEventDlg(void) {
}

// newmapeventdlg.cpp:49
void FrNewMapEventDlg::OnCaptionInit(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:56
void FrNewMapEventDlg::OnInfo1Init(intptr_t param) {
	DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:64
void FrNewMapEventDlg::OnScore0Init(intptr_t param) {
	m_pScore[0] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:69
void FrNewMapEventDlg::OnScore1Init(intptr_t param) {
	m_pScore[1] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:74
void FrNewMapEventDlg::OnScore2Init(intptr_t param) {
	m_pScore[2] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:79
void FrNewMapEventDlg::OnScore3Init(intptr_t param) {
	m_pScore[3] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:84
void FrNewMapEventDlg::OnScore4Init(intptr_t param) {
	m_pScore[4] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:89
void FrNewMapEventDlg::OnScore5Init(intptr_t param) {
	m_pScore[5] = DYNAMIC_CAST(FrArea, param);
}

// newmapeventdlg.cpp:94
void FrNewMapEventDlg::OnScore0OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x1) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "5under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "5under_00");
	}

	const WRect& rc = m_pScore[0]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}

// newmapeventdlg.cpp:122
void FrNewMapEventDlg::OnScore1OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x2) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "10under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "10under_00");
	}

	const WRect& rc = m_pScore[1]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}

// newmapeventdlg.cpp:150
void FrNewMapEventDlg::OnScore2OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x4) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "15under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "15under_00");
	}

	const WRect& rc = m_pScore[2]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}

// newmapeventdlg.cpp:178
void FrNewMapEventDlg::OnScore3OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x8) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "20under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "20under_00");
	}

	const WRect& rc = m_pScore[3]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}

// newmapeventdlg.cpp:206
void FrNewMapEventDlg::OnScore4OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x10) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "25under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "25under_00");
	}

	const WRect& rc = m_pScore[4]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}

// newmapeventdlg.cpp:234
void FrNewMapEventDlg::OnScore5OwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI == NULL) {
		return;
	}

	const Bitmap* pBmp;
	if (WSingleton<CSharedDoc>::Instance()->m_newMapEventFlags & 0x20) {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "30under_01");
	} else {
		pBmp = g_pFresh->GetManager()->GetBitmap("NEWMAP_EVENT", "30under_00");
	}

	const WRect& rc = m_pScore[5]->GetRect();
	WRect src(0, 0, pBmp->Width(), pBmp->Height());
	pGDI->DrawTexture(pBmp, src, WRect(rc.x, rc.y, src.w, src.h), 0xffffffff, 0);
}
