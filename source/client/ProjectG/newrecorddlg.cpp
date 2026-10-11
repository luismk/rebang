// Original: d:\build\custom\temp\code\client\projectg\newrecorddlg.cpp
#include "NewRecordDlg.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "../shared/classdefine.h"
#include "frlistbox.h"

// Course order of the list (low byte of the course type ids, 0x28000000 | n).
static const unsigned char data_00a244fc[18] = {
	19, 16, 15, 14, 13, 11, 8, 10, 0, 1, 2, 3, 4, 5, 6, 7, 9, 127
};

// newrecorddlg.cpp:18 (run-time type and factory registration of FrNewRecordDlg)
const WRTTI FrNewRecordDlg::m_RTTI("FrNewRecordDlg", &FrForm::m_RTTI);

IObject* FrNewRecordDlgMakeInstance(void) {
	return new FrNewRecordDlg;
}

struct __sFrNewRecordDlg {
	__sFrNewRecordDlg(void) {
		ObjectFactory().AddObjectFunctor(FrNewRecordDlgMakeInstance, "FrNewRecordDlg");
	}
};
static __sFrNewRecordDlg s_registerFrNewRecordDlg;

// newrecorddlg.cpp:26
const sFRESH_MSGMAP* FrNewRecordDlg::GetMessageMap(void) const {
	return &FrNewRecordDlg::_MsgMap;
}

sFRESH_MSGMAP FrNewRecordDlg::_MsgMap = { &FrForm::_MsgMap, &FrNewRecordDlg::_MsgEntries[0] };

sFRESH_ENTRY FrNewRecordDlg::_MsgEntries[] = {
	// newrecorddlg.cpp:28
	{ "courselist", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewRecordDlg::OnCourseListInit },
	// newrecorddlg.cpp:29
	{ "courselist", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewRecordDlg::OnCourseListOwnerDraw },

	// newrecorddlg.cpp:31
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// Owner-draw placeholder: no custom drawing required for the course list in
// this stubbed build. Provide an empty implementation to satisfy the linker.
void FrNewRecordDlg::OnCourseListOwnerDraw(int) { }

// newrecorddlg.cpp:34
void FrNewRecordDlg::OnCourseListInit(intptr_t param) {
	m_pCourseList = DYNAMIC_CAST(FrListBox, param);

	if (m_pCourseList == 0) {
		return;
	}

	std::map<unsigned int, IFF_STRUCT::sCourse>& courseMap = ItemManager()->m_courseMap;

	for (int i = 0; i < 18; i++) {
		std::map<unsigned int, IFF_STRUCT::sCourse>::iterator it = courseMap.find(data_00a244fc[i] | 0x28000000);
		if (it == courseMap.end()) {
			continue;
		}

		IFF_STRUCT::sCourse* pCourse = &it->second;

		if (!pCourse->m_active) {
			continue;
		}

		m_pCourseList->AddItem(pCourse);
	}
}
