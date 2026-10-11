// Original: d:\build\custom\temp\code\client\projectg\birthdayeventdlg.h
#ifndef PG_CLIENT_PROJECTG_BIRTHDAYEVENTDLG_H
#define PG_CLIENT_PROJECTG_BIRTHDAYEVENTDLG_H

#include <stdlib.h>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../client/Wangreal/include/wtypes.h"

class Bitmap;
class CGDBitmapBox;
class CGDTextBox;
class CSimpleUI;
class FrArea;
class FrButton;
class FrViewer;
class FrBirthdayEventHelpDlg;

// One mission of the birthday board: whether it is completed and its value
// (a course mask for the course missions, a count for the others).
struct MissionData {
	// birthdayeventdlg.h:20
	MissionData(void) {
		m_bCompleted = 0;
		m_value = 0;
	}

	unsigned long m_bCompleted;   // 0x0
	unsigned long m_value;        // 0x4
};
CHECK_SIZE(MissionData, 0x8);
CHECK_FIELD(MissionData, m_bCompleted, 0x0);
CHECK_FIELD(MissionData, m_value, 0x4);

// One square of the 4x4 board: its area, the area of its mission number and
// the number's image.
struct BoardInfo {
	WRect m_rect;                  // 0x0
	WRect m_numberRect;            // 0x10
	const Bitmap* m_pNumberImg;    // 0x20
};
CHECK_SIZE(BoardInfo, 0x24);
CHECK_FIELD(BoardInfo, m_rect, 0x0);
CHECK_FIELD(BoardInfo, m_numberRect, 0x10);
CHECK_FIELD(BoardInfo, m_pNumberImg, 0x20);

// The birthday event board dialog; its size (0x458) is the `new` of
// FrBirthdayEventDlgMakeInstance.
class FrBirthdayEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // birthdayeventdlg.h:34

	FrBirthdayEventDlg(void);
	virtual ~FrBirthdayEventDlg(void);

	virtual bool OnInit(void);
	virtual void OnMouseMove(const WPoint& pt);

	void SetRemainGiftCount(unsigned long count);
	void SetEventName(std::string& name);
	void SetMissionData(unsigned long completed, unsigned long remainGiftCount);
	void SetMissionInfo(std::vector<unsigned long>& missions, std::vector<unsigned long>& values);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnLButtonDownHelpButton(void);
	void OnInitGiftButton(intptr_t param);
	void OnLButtonDownGiftButton(void);
	void OnOwnerDrawBoardArea(int param);
	void OnInitBoardNumberArea(intptr_t param);
	void OnInitBoardBG1Area(intptr_t param);

private:
	void InitalizeCoordinates(void);
	void InitalizeImages(void);
	bool OnKoohBirthdayEventHelpDlg(int result, FrForm* pForm);
	void CheckToolTip(const WPoint& pt);
	void MakeToolTipText(int mission);
	char* MakeMapInfoText(int index, char* text);

	FrBirthdayEventHelpDlg* m_pHelpDlg;   // 0x110
	FrButton* m_pGiftButton;              // 0x114 ("Gift")
	FrArea* m_pBoardNumberArea;           // 0x118 ("boardNumber")
	FrArea* m_pBoardBG1Area;              // 0x11c ("BirthdayEvent_bg1")
	CGDTextBox* m_pTextBox;               // 0x120, game data "BirthdayEvent<name>" (texts)
	CGDBitmapBox* m_pBitmapBox;           // 0x124, game data "BirthdayEvent<name>" (bitmaps)
	div_t m_giftCount;                    // 0x128, m_remainGiftCount / 10
	float m_clickOffset;                  // 0x130, x offset of the bouncing "Click" image
	unsigned long m_remainGiftCount;      // 0x134
	unsigned long m_boardIndex;           // 0x138, row of the BIRTHDAYEVENT boards
	std::string m_eventName;              // 0x13c
	MissionData m_missions[16];           // 0x158
	const Bitmap* m_pDateImg;             // 0x1d8
	const Bitmap* m_pClickImg;            // 0x1dc
	CSimpleUI* m_pGiftNumberUI;           // 0x1e0
	CSimpleUI* m_pBoardUI;                // 0x1e4
	BoardInfo m_boards[16];               // 0x1e8
	WRect m_giftNumberRect;               // 0x428
	WRect m_dateRect;                     // 0x438
	WRect m_clickRect;                    // 0x448
};
CHECK_SIZE(FrBirthdayEventDlg, 0x458);
BEGIN_LAYOUT_CHECK(FrBirthdayEventDlg)
	CHECK_FIELD(FrBirthdayEventDlg, m_pHelpDlg, 0x110);
	CHECK_FIELD(FrBirthdayEventDlg, m_pGiftButton, 0x114);
	CHECK_FIELD(FrBirthdayEventDlg, m_pBoardNumberArea, 0x118);
	CHECK_FIELD(FrBirthdayEventDlg, m_pBoardBG1Area, 0x11c);
	CHECK_FIELD(FrBirthdayEventDlg, m_pTextBox, 0x120);
	CHECK_FIELD(FrBirthdayEventDlg, m_pBitmapBox, 0x124);
	CHECK_FIELD(FrBirthdayEventDlg, m_giftCount, 0x128);
	CHECK_FIELD(FrBirthdayEventDlg, m_clickOffset, 0x130);
	CHECK_FIELD(FrBirthdayEventDlg, m_remainGiftCount, 0x134);
	CHECK_FIELD(FrBirthdayEventDlg, m_boardIndex, 0x138);
	CHECK_FIELD(FrBirthdayEventDlg, m_eventName, 0x13c);
	CHECK_FIELD(FrBirthdayEventDlg, m_missions, 0x158);
	CHECK_FIELD(FrBirthdayEventDlg, m_pDateImg, 0x1d8);
	CHECK_FIELD(FrBirthdayEventDlg, m_pClickImg, 0x1dc);
	CHECK_FIELD(FrBirthdayEventDlg, m_pGiftNumberUI, 0x1e0);
	CHECK_FIELD(FrBirthdayEventDlg, m_pBoardUI, 0x1e4);
	CHECK_FIELD(FrBirthdayEventDlg, m_boards, 0x1e8);
	CHECK_FIELD(FrBirthdayEventDlg, m_giftNumberRect, 0x428);
	CHECK_FIELD(FrBirthdayEventDlg, m_dateRect, 0x438);
	CHECK_FIELD(FrBirthdayEventDlg, m_clickRect, 0x448);
END_LAYOUT_CHECK

// The help window of the birthday event; its size (0x118) is the `new` of
// FrBirthdayEventHelpDlgMakeInstance.
class FrBirthdayEventHelpDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // birthdayeventdlg.h:105

	FrBirthdayEventHelpDlg(void);
	virtual ~FrBirthdayEventHelpDlg(void);

	void OpenHelpDlg(const char* file);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitHelpView(intptr_t param);

private:
	FrViewer* m_pHelpView;   // 0x110 ("HelpView")
	UNKNOWN(0x114, 0x118);
};
CHECK_SIZE(FrBirthdayEventHelpDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrBirthdayEventHelpDlg)
	CHECK_FIELD(FrBirthdayEventHelpDlg, m_pHelpView, 0x110);
END_LAYOUT_CHECK

// The window showing the number of the mission just completed, faded out a
// few seconds after it is shown; its size (0x160) is the `new` of
// FrBirthdayEventMissionDlgMakeInstance.
class FrBirthdayEventMissionDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CTaskMain;   // HandleMsgCommon sets m_fadeOut
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // birthdayeventdlg.h:124

	FrBirthdayEventMissionDlg(void);
	virtual ~FrBirthdayEventMissionDlg(void);

	virtual bool OnInit(void);

	void SetCompletedMissionInfo(unsigned long mission);
	void CalculatePos(unsigned long index, unsigned long count);
	// birthdayeventdlg.h:147; its body is still out of line in
	// client/ProjectG/TaskMain.cpp
	void SetFadeOutFlag(bool fadeOut);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnInitMissionNumberArea(intptr_t param);

private:
	FrArea* m_pMissionNumberArea;          // 0x110 ("MissionNumber")
	const Bitmap* m_pNumberImg[15];        // 0x114, "BirthdayEvent_num2_%d" for missions 1-15
	UNKNOWN(0x150, 0x154);
	unsigned long m_completedMission;      // 0x154
	float m_fadeTime;                      // 0x158
	bool m_fadeOut;                        // 0x15c
	UNKNOWN(0x15d, 0x160);
};
CHECK_SIZE(FrBirthdayEventMissionDlg, 0x160);
BEGIN_LAYOUT_CHECK(FrBirthdayEventMissionDlg)
	CHECK_FIELD(FrBirthdayEventMissionDlg, m_pMissionNumberArea, 0x110);
	CHECK_FIELD(FrBirthdayEventMissionDlg, m_pNumberImg, 0x114);
	CHECK_FIELD(FrBirthdayEventMissionDlg, m_completedMission, 0x154);
	CHECK_FIELD(FrBirthdayEventMissionDlg, m_fadeTime, 0x158);
	CHECK_FIELD(FrBirthdayEventMissionDlg, m_fadeOut, 0x15c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BIRTHDAYEVENTDLG_H
