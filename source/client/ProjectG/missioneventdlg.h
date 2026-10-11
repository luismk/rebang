// Original: d:\build\custom\temp\code\client\projectg\missioneventdlg.h
#ifndef PG_CLIENT_PROJECTG_MISSIONEVENTDLG_H
#define PG_CLIENT_PROJECTG_MISSIONEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../client/Wangreal/include/wtypes.h"

class Bitmap;
class FrArea;
class FrButton;
class FrViewer;
class CMissionEvent;
class FrMissionDetail;

// The mission event window: the day and term mission clear marks, the
// course clear board, and the buttons asking for the day and term gifts.
// Its size (0x1f8) is the `new` of FrMissionEventDlgMakeInstance.
class FrMissionEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // missioneventdlg.h:13

	FrMissionEventDlg(void);
	virtual ~FrMissionEventDlg(void);

	virtual bool OnInit(void);

	bool OnDetailDlgResult(int ret, FrForm* pForm);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDetailButton(intptr_t param);
	void OnLButtonDownDetailButton(void);
	void OnInitDayGiftButton(intptr_t param);
	void OnLButtonDownDayGiftButton(void);
	void OnInitTermGiftButton(intptr_t param);
	void OnLButtonDownTermGiftButton(void);
	void OnInitMissionClearArea(intptr_t param);
	void OnOwnerDrawMissionClearArea(int param);
	void OnInitCourseClearArea(intptr_t param);
	void OnOwnerDrawCourseClearArea(int param);
	void OnHoverOnCourseClearArea(intptr_t param);
	void OnHoverOffCourseClearArea(intptr_t param);

private:
	enum {
		BUTTON_DETAIL = 0,
		BUTTON_DAY_GIFT = 1,
		BUTTON_TERM_GIFT = 2
	};
	enum {
		BITMAP_CLEAR_MARK = 0,
		BITMAP_COURSE_BG = 1,
		BITMAP_COURSE_BG_CLEAR = 2
	};

	static sFRESH_ENTRY _MsgEntries[];

	void CheckMissionComplete(void);
	void CheckCourseComplete(void);
	void RequestGift(int type);
	void CalcOffset(int index, float& x, float& y);

	// The detail, day gift and term gift buttons (BUTTON_*), cleared together
	// by the constructor.
	FrButton* m_pButton[3];              // 0x110 (OnInitDetailButton, OnInitDayGiftButton, OnInitTermGiftButton)
	FrArea* m_pCourseClearArea;          // 0x11c (OnInitCourseClearArea)
	FrArea* m_pMissionClearArea;         // 0x120 (OnInitMissionClearArea)
	// missionevent_clear_mark, course_bg1, course_bg_c1 (BITMAP_*), cleared
	// together by the constructor.
	const Bitmap* m_pBitmap[3];          // 0x124 (OnInit)
	FrMissionDetail* m_pDetailDlg;       // 0x130 (OnLButtonDownDetailButton)
	CMissionEvent* m_pMissionEvent;      // 0x134 (OnInit)
	UNKNOWN(0x138, 0x13c);
	WRect m_courseClearSrc;              // 0x13c (OnOwnerDrawCourseClearArea: source rect of a course clear mark)
	WPoint m_missionPos[7];              // 0x14c (OnInit: clear mark of each mission)
	WPoint m_termMarkPos[5];             // 0x184 (OnInit)
	bool m_missionComplete[7];           // 0x1ac (CheckMissionComplete: 2 day, 5 term missions)
	bool m_bHoverCourse;                 // 0x1b3 (OnHoverOnCourseClearArea)
	int m_courseComplete[15];            // 0x1b4 (CheckCourseComplete)
	int m_completeCourseCount;           // 0x1f0 (CheckCourseComplete)
	UNKNOWN(0x1f4, 0x1f8);
};

CHECK_SIZE(FrMissionEventDlg, 0x1f8);
BEGIN_LAYOUT_CHECK(FrMissionEventDlg)
	CHECK_FIELD(FrMissionEventDlg, m_pButton, 0x110);
	CHECK_FIELD(FrMissionEventDlg, m_pCourseClearArea, 0x11c);
	CHECK_FIELD(FrMissionEventDlg, m_pMissionClearArea, 0x120);
	CHECK_FIELD(FrMissionEventDlg, m_pBitmap, 0x124);
	CHECK_FIELD(FrMissionEventDlg, m_pDetailDlg, 0x130);
	CHECK_FIELD(FrMissionEventDlg, m_pMissionEvent, 0x134);
	CHECK_FIELD(FrMissionEventDlg, m_courseClearSrc, 0x13c);
	CHECK_FIELD(FrMissionEventDlg, m_missionPos, 0x14c);
	CHECK_FIELD(FrMissionEventDlg, m_termMarkPos, 0x184);
	CHECK_FIELD(FrMissionEventDlg, m_missionComplete, 0x1ac);
	CHECK_FIELD(FrMissionEventDlg, m_bHoverCourse, 0x1b3);
	CHECK_FIELD(FrMissionEventDlg, m_courseComplete, 0x1b4);
	CHECK_FIELD(FrMissionEventDlg, m_completeCourseCount, 0x1f0);
END_LAYOUT_CHECK

// The mission event explanation window: one image viewer. Its size (0x118)
// is the `new` of FrMissionDetailMakeInstance.
class FrMissionDetail : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // missioneventdlg.h:83

	FrMissionDetail(void);
	virtual ~FrMissionDetail(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitExplaneViewer(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrViewer* m_pExplaneViewer;          // 0x110 (OnInitExplaneViewer)
	UNKNOWN(0x114, 0x118);
};

CHECK_SIZE(FrMissionDetail, 0x118);
BEGIN_LAYOUT_CHECK(FrMissionDetail)
	CHECK_FIELD(FrMissionDetail, m_pExplaneViewer, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MISSIONEVENTDLG_H
