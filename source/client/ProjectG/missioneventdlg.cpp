// Original: d:\build\custom\temp\code\client\projectg\missioneventdlg.cpp
#include <stdio.h>
#include <string.h>

#include "MissionEventDlg.h"
#include "ObjectFactory.h"
#include "contentsdoc.h"
#include "TaskManager.h"
#include "actor.h"
#include "Packet.h"
#include "fresh.h"
#include "frarea.h"
#include "frbutton.h"
#include "frviewer.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"

// missioneventdlg.cpp:20 (run-time type and factory registration of FrMissionEventDlg)
const WRTTI FrMissionEventDlg::m_RTTI("FrMissionEventDlg", &FrForm::m_RTTI);

IObject* FrMissionEventDlgMakeInstance(void) {
	return new FrMissionEventDlg;
}

struct __sFrMissionEventDlg {
	__sFrMissionEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMissionEventDlgMakeInstance, "FrMissionEventDlg");
	}
};
static __sFrMissionEventDlg s_registerFrMissionEventDlg;

// missioneventdlg.cpp:22
const sFRESH_MSGMAP* FrMissionEventDlg::GetMessageMap(void) const {
	return &FrMissionEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrMissionEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrMissionEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMissionEventDlg::_MsgEntries[] = {
	// missioneventdlg.cpp:24
	{ "eventdetail", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnInitDetailButton },
	// missioneventdlg.cpp:25
	{ "eventdetail", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMissionEventDlg::OnLButtonDownDetailButton },

	// missioneventdlg.cpp:27
	{ "daygift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnInitDayGiftButton },
	// missioneventdlg.cpp:28
	{ "daygift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMissionEventDlg::OnLButtonDownDayGiftButton },

	// missioneventdlg.cpp:30
	{ "termgift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnInitTermGiftButton },
	// missioneventdlg.cpp:31
	{ "termgift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrMissionEventDlg::OnLButtonDownTermGiftButton },

	// missioneventdlg.cpp:33
	{ "clear_mission", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnInitMissionClearArea },
	// missioneventdlg.cpp:34
	{ "clear_mission", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnOwnerDrawMissionClearArea },

	// missioneventdlg.cpp:36
	{ "clear_course", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnInitCourseClearArea },
	// missioneventdlg.cpp:37
	{ "clear_course", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnOwnerDrawCourseClearArea },
	// missioneventdlg.cpp:38
	{ "clear_course", FRCMD_HOVERON, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnHoverOnCourseClearArea },
	// missioneventdlg.cpp:39
	{ "clear_course", FRCMD_HOVEROFF, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionEventDlg::OnHoverOffCourseClearArea },

	// missioneventdlg.cpp:42
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// missioneventdlg.cpp:45
FrMissionEventDlg::FrMissionEventDlg(void) {
	memset(m_pButton, 0, sizeof(m_pButton));
	memset(m_pBitmap, 0, sizeof(m_pBitmap));

	m_pCourseClearArea = 0;
	m_pMissionClearArea = 0;
	m_pDetailDlg = 0;
	m_pMissionEvent = 0;
	memset(m_missionComplete, 0, sizeof(m_missionComplete));
	m_bHoverCourse = false;
	m_completeCourseCount = 0;
}

// missioneventdlg.cpp:59
FrMissionEventDlg::~FrMissionEventDlg(void) {

}

// missioneventdlg.cpp:64
bool FrMissionEventDlg::OnInit(void) {
	m_pBitmap[BITMAP_CLEAR_MARK] = g_pFresh->RegisterBitmap("missionevent_clear_mark");
	m_pBitmap[BITMAP_COURSE_BG] = g_pFresh->RegisterBitmap("course_bg1");
	m_pBitmap[BITMAP_COURSE_BG_CLEAR] = g_pFresh->RegisterBitmap("course_bg_c1");

	IContentsDataContainer* pContainer = WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x83); if (pContainer) m_pMissionEvent = (CMissionEvent*)pContainer;

	if (m_pMissionEvent == 0) {


		return false;
	}

	m_missionPos[0] = WPoint(215.0f, 250.0f);
	m_missionPos[1] = WPoint(350.0f, 250.0f);
	m_missionPos[2] = WPoint(215.0f, 370.0f);
	m_missionPos[3] = WPoint(215.0f, 450.0f);
	m_missionPos[4] = WPoint(350.0f, 370.0f);
	m_missionPos[5] = WPoint(350.0f, 450.0f);
	m_missionPos[6] = WPoint(485.0f, 370.0f);

	m_termMarkPos[0] = WPoint(256.0f, 301.0f);
	m_termMarkPos[1] = WPoint(388.0f, 301.0f);
	m_termMarkPos[2] = WPoint(253.0f, 423.0f);
	m_termMarkPos[3] = WPoint(392.0f, 505.0f);
	m_termMarkPos[4] = WPoint(546.0f, 359.0f);

	CheckMissionComplete();
	CheckCourseComplete();

	return FrForm::OnInit();
}

// missioneventdlg.cpp:98
bool FrMissionEventDlg::OnDetailDlgResult(int ret, FrForm* pForm) {
	m_pDetailDlg = 0;

	return true;
}

// missioneventdlg.cpp:105
void FrMissionEventDlg::CheckMissionComplete(void) {
	if (m_pMissionEvent->IsCompleteAllDayMission() == true) {

		m_pButton[BUTTON_DAY_GIFT]->Enable(true);
	}

	if (m_pMissionEvent->IsCompleteAllTermMission() == true) {

		m_pButton[BUTTON_TERM_GIFT]->Enable(true);
	}

	int i;
	for (i = 0; i < 2; ++i) {

		if (m_pMissionEvent->IsCompleteMission(i)) {

			m_missionComplete[i] = true;
		}
	}

	for (i = 2; i < 7; ++i) {

		if (m_pMissionEvent->IsCompleteMission(i)) {

			m_missionComplete[i] = true;
		}
	}
}

// missioneventdlg.cpp:134
void FrMissionEventDlg::CheckCourseComplete(void) {
	memset(m_courseComplete, 0, sizeof(m_courseComplete));

	for (int i = 0; i < 15; ++i) {

		if (m_pMissionEvent->IsCompleteCourse(i)) {

			m_courseComplete[i] = 1;
			m_completeCourseCount += 1;
		}
	}
}

// missioneventdlg.cpp:148
void FrMissionEventDlg::RequestGift(int type) {
	WSendPacket packet(0xE4);
	packet.Encode4(type);
	packet.Send(SEND_TO_DEFAULT);
}

// missioneventdlg.cpp:155
void FrMissionEventDlg::CalcOffset(int index, float& x, float& y) {
	int column = index > 7;
	if (column == 0) {


		x = 0.0f; y = index * 12.0f;
	}
	else if (column == 1) {

		index -= 8;
		x = 65.0f; y = index * 12.0f;
	}
}

// missioneventdlg.cpp:170
void FrMissionEventDlg::OnInitDetailButton(intptr_t param) {
	m_pButton[BUTTON_DETAIL] = DYNAMIC_CAST(FrButton, param);
}

// missioneventdlg.cpp:175
void FrMissionEventDlg::OnLButtonDownDetailButton(void) {
	if (m_pDetailDlg == 0) {




		m_pDetailDlg = CreateForm<FrMissionDetail>(g_pFresh->GetManager(), this, "mission_detail", 0);
		if (m_pDetailDlg) {

			m_pDetailDlg->Open((FRESH_RESULT_CALLBACK)&FrMissionEventDlg::OnDetailDlgResult, 1);
		}
	}
}

// missioneventdlg.cpp:189
void FrMissionEventDlg::OnInitDayGiftButton(intptr_t param) {
	m_pButton[BUTTON_DAY_GIFT] = DYNAMIC_CAST(FrButton, param);
	if (m_pButton[BUTTON_DAY_GIFT]) {

		m_pButton[BUTTON_DAY_GIFT]->Enable(false);
	}
}

// missioneventdlg.cpp:198
void FrMissionEventDlg::OnLButtonDownDayGiftButton(void) {
	if (m_pMissionEvent->GetDayGiftFlag() == true) {

		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xc0\xcc\xb9\xcc \xc0\xcf\xc0\xcf\xb9\xcc\xbc\xc7 \xbc\xb1\xb9\xb0\xc0\xbb \xb9\xde\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
		return;
	}

	RequestGift(1);
}

// missioneventdlg.cpp:210
void FrMissionEventDlg::OnInitTermGiftButton(intptr_t param) {
	m_pButton[BUTTON_TERM_GIFT] = DYNAMIC_CAST(FrButton, param);
	if (m_pButton[BUTTON_TERM_GIFT]) {

		m_pButton[BUTTON_TERM_GIFT]->Enable(false);
	}
}

// missioneventdlg.cpp:219
void FrMissionEventDlg::OnLButtonDownTermGiftButton(void) {
	if (m_pMissionEvent->GetTermGiftFlag() == true) {

		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xc0\xcc\xb9\xcc \xb1\xe2\xb0\xa3\xb9\xcc\xbc\xc7 \xbc\xb1\xb9\xb0\xc0\xbb \xb9\xde\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
		return;
	}

	RequestGift(2);
}

// missioneventdlg.cpp:231
void FrMissionEventDlg::OnInitMissionClearArea(intptr_t param) {
	m_pMissionClearArea = DYNAMIC_CAST(FrArea, param);
}

// missioneventdlg.cpp:236
void FrMissionEventDlg::OnOwnerDrawMissionClearArea(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {




		unsigned long oldColor = pGDI->GetTextColor();


		char buf[4] = "";
		unsigned char conditionIndex[4] = { 0, 1, 2, 5 };
		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0xffffffff, 0xffffffff);


		int i;
		for (i = 0; i < 4; ++i) {

			sprintf(buf, "%d", m_pMissionEvent->GetCondition(conditionIndex[i]));
			pGDI->Print(WPoint(m_termMarkPos[i].x, m_termMarkPos[i].y), 0, "%s", buf);
		}
		pGDI->SetTextColor(oldColor, 0xffffffff);


		unsigned int width = m_pBitmap[BITMAP_CLEAR_MARK]->Width();
		unsigned int height = m_pBitmap[BITMAP_CLEAR_MARK]->Height();
		for (i = 0; i < 7; ++i) {

			if (m_missionComplete[i]) {

				pGDI->DrawTexture(m_pBitmap[BITMAP_CLEAR_MARK], WRect(m_missionPos[i].x, m_missionPos[i].y, width, height), 0xffffffff, 0);
			}
		}
	}
}

// missioneventdlg.cpp:272
void FrMissionEventDlg::OnInitCourseClearArea(intptr_t param) {
	m_pCourseClearArea = DYNAMIC_CAST(FrArea, param);
	if (m_pCourseClearArea) {

		m_pCourseClearArea->SetMouseEvent(true);
		m_pCourseClearArea->EnableHover(true, 0.0f);
	}
}

// missioneventdlg.cpp:282
void FrMissionEventDlg::OnOwnerDrawCourseClearArea(int param) {
	if (m_bHoverCourse) {




		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
		if (pGDI) {




			const WRect& rcArea = m_pCourseClearArea->GetRect();


			pGDI->DrawTexture(m_pBitmap[BITMAP_COURSE_BG], WRect(rcArea.x, rcArea.y, (float)m_pBitmap[BITMAP_COURSE_BG]->Width(), (float)m_pBitmap[BITMAP_COURSE_BG]->Height()), 0xffffffff, 0);



			float x = 0.0f;
			float y = 0.0f;
			for (int i = 0; i < m_pMissionEvent->GetMapCount(); ++i) {

				if (m_courseComplete[i]) {

					CalcOffset(i, x, y);
					WRect src(x, y, 65.0f, 11.0f);
					m_courseClearSrc = src;













					pGDI->DrawTexture(m_pBitmap[BITMAP_COURSE_BG_CLEAR], m_courseClearSrc, WRect(x + rcArea.x + 15.0f, y + rcArea.y + 30.0f, 65.0f, 11.0f), 0xffffffff, 0);
				}
			}

			unsigned long oldColor = pGDI->GetTextColor();
			char buf[4] = "";
			pGDI->SetTextStyle(0);
			pGDI->SetTextColor(0xffffffff, 0xffffffff);
			sprintf(buf, "%d", m_completeCourseCount);
			pGDI->Print(WPoint(m_termMarkPos[4]), 0, "%s", buf);
			pGDI->SetTextColor(oldColor, 0xffffffff);
		}
	}
}

// missioneventdlg.cpp:327
void FrMissionEventDlg::OnHoverOnCourseClearArea(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {




		m_bHoverCourse = true;
	}
}

// missioneventdlg.cpp:338
void FrMissionEventDlg::OnHoverOffCourseClearArea(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {




		m_bHoverCourse = false;
	}
}

// missioneventdlg.cpp:360 (run-time type and factory registration of FrMissionDetail)
const WRTTI FrMissionDetail::m_RTTI("FrMissionDetail", &FrForm::m_RTTI);

IObject* FrMissionDetailMakeInstance(void) {
	return new FrMissionDetail;
}

struct __sFrMissionDetail {
	__sFrMissionDetail(void) {
		ObjectFactory().AddObjectFunctor(FrMissionDetailMakeInstance, "FrMissionDetail");
	}
};
static __sFrMissionDetail s_registerFrMissionDetail;

// missioneventdlg.cpp:362
const sFRESH_MSGMAP* FrMissionDetail::GetMessageMap(void) const {
	return &FrMissionDetail::_MsgMap;
}

sFRESH_MSGMAP FrMissionDetail::_MsgMap = { &FrForm::_MsgMap, &FrMissionDetail::_MsgEntries[0] };

sFRESH_ENTRY FrMissionDetail::_MsgEntries[] = {
	// missioneventdlg.cpp:364
	{ "detailview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMissionDetail::OnInitExplaneViewer },

	// missioneventdlg.cpp:366
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// missioneventdlg.cpp:370
FrMissionDetail::FrMissionDetail(void) {
	m_pExplaneViewer = 0;
}

// missioneventdlg.cpp:375
FrMissionDetail::~FrMissionDetail(void) {

}

// missioneventdlg.cpp:380
void FrMissionDetail::OnInitExplaneViewer(intptr_t param) {
	m_pExplaneViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_pExplaneViewer) {

		m_pExplaneViewer->Open("missionevent_explanation.jpg");
	}
}
