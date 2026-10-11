// Original: d:\build\custom\temp\code\client\projectg\frfindeborteventdlg.cpp
#include "FrFindEbortEventDlg.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// frfindeborteventdlg.cpp:14 (run-time type and factory registration of FrFindEbortEventDlg)
const WRTTI FrFindEbortEventDlg::m_RTTI("FrFindEbortEventDlg", &FrForm::m_RTTI);

IObject* FrFindEbortEventDlgMakeInstance(void) {
	return new FrFindEbortEventDlg;
}

struct __sFrFindEbortEventDlg {
	__sFrFindEbortEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFindEbortEventDlgMakeInstance, "FrFindEbortEventDlg");
	}
};
static __sFrFindEbortEventDlg s_registerFrFindEbortEventDlg;

// frfindeborteventdlg.cpp:16
const sFRESH_MSGMAP* FrFindEbortEventDlg::GetMessageMap(void) const {
	return &FrFindEbortEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrFindEbortEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrFindEbortEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFindEbortEventDlg::_MsgEntries[] = {
	// frfindeborteventdlg.cpp:18
	{ "current_ebort", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFindEbortEventDlg::OnCurrentItemOwnerDraw },

	// frfindeborteventdlg.cpp:20
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frfindeborteventdlg.cpp:27
FrFindEbortEventDlg::FrFindEbortEventDlg(void) : m_reserved140(0) {
	for (int i = 0; i < 4; ++i) { m_itemCounts[i] = 0; m_countPositions[i] = WPoint(0.0f, 0.0f); }
}

// frfindeborteventdlg.cpp:36
FrFindEbortEventDlg::~FrFindEbortEventDlg(void) {
}

// Minimal OnInit implementation to satisfy the virtual override expected by the
// linker. Full initialization logic can be implemented later.
bool FrFindEbortEventDlg::OnInit(void) {
	// default initialization: zero counts already set in ctor
	return true;
}

// frfindeborteventdlg.cpp:40
void FrFindEbortEventDlg::OnCurrentItemOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		int oldSpace = pGDI->SetSpace(0);

		WPoint origin(m_rect.x, m_rect.y);

		for (int i = 0; i < 4; ++i) {
			WPoint pos = m_countPositions[i] + origin;

			pGDI->PrintText(pos, 2, MakeStr("%d", m_itemCounts[i]), 0);
		}

		pGDI->SetSpace(oldSpace);
	}
}
