// Original: d:\build\custom\temp\code\client\projectg\fortunedlg.cpp
#include <windows.h>
#include <atlbase.h>
#include "FortuneDlg.h"
#include "ObjectFactory.h"
#include "UccManager.h"
#include "commonutil.h"
#include "frarea.h"
#include "frstatic.h"

// fortunedlg.cpp:19 (run-time type and factory registration of FrFortuneDlg)
const WRTTI FrFortuneDlg::m_RTTI("FrFortuneDlg", &FrForm::m_RTTI);

IObject* FrFortuneDlgMakeInstance(void) {
	return new FrFortuneDlg;
}

struct __sFrFortuneDlg {
	__sFrFortuneDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFortuneDlgMakeInstance, "FrFortuneDlg");
	}
};
static __sFrFortuneDlg s_registerFrFortuneDlg;

// fortunedlg.cpp:21
const sFRESH_MSGMAP* FrFortuneDlg::GetMessageMap(void) const {
	return &FrFortuneDlg::_MsgMap;
}

sFRESH_MSGMAP FrFortuneDlg::_MsgMap = { &FrForm::_MsgMap, &FrFortuneDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFortuneDlg::_MsgEntries[] = {
	// fortunedlg.cpp:23
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneDlg::OnPortraitInit },
	{ "desc_name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneDlg::OnDescNameInit },
	{ "desc_level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneDlg::OnDescLevelInit },
	{ "desc_desc", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneDlg::OnDescDescInit },

	// fortunedlg.cpp:28
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// fortunedlg.cpp:31
void FrFortuneDlg::OnPortraitInit(intptr_t param) {
	m_pPortrait = DYNAMIC_CAST(FrArea, param);
}

// fortunedlg.cpp:36
void FrFortuneDlg::OnDescNameInit(intptr_t param) {
	m_pDescName = DYNAMIC_CAST(FrStatic, param);
}

// fortunedlg.cpp:41
void FrFortuneDlg::OnDescLevelInit(intptr_t param) {
	m_pDescLevel = DYNAMIC_CAST(FrStatic, param);
}

// fortunedlg.cpp:46
void FrFortuneDlg::OnDescDescInit(intptr_t param) {
	m_pDescDesc = DYNAMIC_CAST(FrStatic, param);
}

// fortunedlg.cpp:51
FrFortuneDlg::FrFortuneDlg(void) {
	m_typeId = 0;

	m_pPortrait = 0;
	m_pDescName = 0;
	m_pDescLevel = 0;
	m_pDescDesc = 0;
}

// fortunedlg.cpp:61
FrFortuneDlg::~FrFortuneDlg(void) {
}

// fortunedlg.cpp:65
void FrFortuneDlg::SetTypeId(unsigned long typeId, IFF_ITEM_COMMON* pItem) {
	m_typeId = typeId;

	if (pItem == 0) {
		return;
	}

	if (m_pPortrait) {
		m_pPortrait->SetBgImg(pItem->m_icon);
	}

	if (m_pDescName) {
		// "Name : %s"
		m_pDescName->SetCaption(MakeStr("\xC0\xCC\xB8\xA7 : %s", pItem->m_name));
	}

	if (m_pDescLevel) {
		// "Level : usable by anyone"
		m_pDescLevel->SetCaption("\xB7\xB9\xBA\xA7 : \xB4\xA9\xB1\xB8\xB3\xAA \xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9");
	}

	if (m_pDescDesc) {
		// "Do you want to open the %s you own?"
		m_pDescDesc->SetCaption(MakeStr("\xBC\xD2\xC0\xAF\xC7\xCF\xB0\xED \xC0\xD6\xB4\xC2 %s\xC0\xBB \xBF\xAD\xBE\xEE\xBA\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", pItem->m_name));
	}
}
