// Original: d:\build\custom\temp\code\client\projectg\changeroominfodlg.h
#ifndef PG_CLIENT_PROJECTG_CHANGEROOMINFODLG_H
#define PG_CLIENT_PROJECTG_CHANGEROOMINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "globalgamedefine.h"
#include "MakeRoomDlg.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrViewer;
class FrTreasureCourse;

class FrGraphicInterface;
class WRect;

// Room settings form of the room's owner: edits a copy of the current room
// (title, password, game type, players, holes, time, course, hole type).
// Its size (0x250) is the `new` of FrChangeRoomInfoDlgMakeInstance.
class FrChangeRoomInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // changeroominfodlg.h:57

	FrChangeRoomInfoDlg(void);
	virtual ~FrChangeRoomInfoDlg(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void ChangeUserLimit(int userLimit);
	void ChangeHole(int hole);
	void ChangeTimeLimit(int timeLimit);
	void ChangeHoleType(eHoleType holeType);
	void ChangeCourse(int course);
	void ChangeGameType(int gameType, bool b1, bool b2);
	bool IsBtnEnable(void);

	void OnTextInit(intptr_t param);
	void OnTitleInit(intptr_t param);
	void OnPasswordInit(intptr_t param);
	void OnMapPrevInit(intptr_t param);
	void OnMapNextInit(intptr_t param);
	void OnGameTypeInit(intptr_t param);
	void OnUserLimitInit(intptr_t param);
	void OnHoleInit(intptr_t param);
	void OnTimeLimitInit(intptr_t param);
	void OnCourseInit(intptr_t param);
	void OnHoleTypeInit(intptr_t param);

	void OnGameTypeOwnerDraw(intptr_t param);
	void OnUserLimitOwnerDraw(intptr_t param);
	void OnHoleOwnerDraw(intptr_t param);
	void OnTimeLimitOwnerDraw(intptr_t param);
	void OnCourseOwnerDraw(int param);
	void OnHoleTypeOwnerDraw(int param);

	void DrawGameTypeButton(FrGraphicInterface* pDevice, int gameType, WRect rect, bool b);
	void DrawUserLimitButton(FrGraphicInterface* pDevice, int userLimit, WRect rect);
	void DrawHoleButton(FrGraphicInterface* pDevice, int hole, WRect rect);
	void DrawTimeLimitButton(FrGraphicInterface* pDevice, int timeLimit, WRect rect);
	void DrawHoleTypeButton(FrGraphicInterface* pDevice, eHoleType holeType, WRect rect, bool b);

	void OnOkBtnUp(void);
	void OnMapPrevUp(void);
	void OnMapNextUp(void);
	void OnCourseUp(void);
	bool OnMapSelectDlgResult(int result, FrForm* pForm);

	FrEdit* m_pTitle;                  // 0x110 (OnTitleInit)
	FrEdit* m_pPassword;               // 0x114 (OnPasswordInit)
	FrArea* m_pText;                   // 0x118 (OnTextInit)
	PADDING(0x11c, 0x120);         // window pointer cleared by the ctor
	FrButton* m_pMapPrev;              // 0x120 (OnMapPrevInit)
	FrButton* m_pMapNext;              // 0x124 (OnMapNextInit)
	FrArea* m_pGameType;               // 0x128 (OnGameTypeInit)
	FrArea* m_pUserLimit;              // 0x12c (OnUserLimitInit)
	FrArea* m_pHole;                   // 0x130 (OnHoleInit)
	FrArea* m_pTimeLimit;              // 0x134 (OnTimeLimitInit)
	FrViewer* m_pCourse;               // 0x138 (OnCourseInit)
	FrArea* m_pHoleType;               // 0x13c (OnHoleTypeInit)
	const Bitmap* m_pSelectButtonImg[3];   // 0x140, ITEMS select_button_n/_o/_d
	const Bitmap* m_pHoleTypeImg[8];       // 0x14c, hole_front/back/random/shuffle, _n and _d each
	const Bitmap* m_pMapSelectImg;         // 0x16c, MAKEROOM map_select_o
	FrTreasureCourse* m_pMapSelectDlg; // 0x170, open course selection (OnCourseUp); blocks the button delay
	PADDING(0x174, 0x178);         // cleared by the ctor
	int m_gameType;                    // 0x178
	int m_userLimit;                   // 0x17c
	int m_timeLimit;                   // 0x180
	int m_hole;                        // 0x184
	int m_course;                      // 0x188
	unsigned char m_holeType;          // 0x18c, an eHoleType
	PADDING(0x18d, 0x198);         // room values copied by the ctor
	float m_btnDelay;                  // 0x198, 0.7 s countdown before the buttons enable
	sRoomInfo m_roomInfo;              // 0x19c, copy of the current room
};

CHECK_SIZE(FrChangeRoomInfoDlg, 0x250);
BEGIN_LAYOUT_CHECK(FrChangeRoomInfoDlg)
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pTitle, 0x110);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pPassword, 0x114);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pText, 0x118);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pMapPrev, 0x120);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pMapNext, 0x124);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pGameType, 0x128);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pUserLimit, 0x12c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pHole, 0x130);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pTimeLimit, 0x134);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pCourse, 0x138);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pHoleType, 0x13c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pSelectButtonImg, 0x140);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pHoleTypeImg, 0x14c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pMapSelectImg, 0x16c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_pMapSelectDlg, 0x170);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_gameType, 0x178);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_userLimit, 0x17c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_timeLimit, 0x180);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_hole, 0x184);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_course, 0x188);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_holeType, 0x18c);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_btnDelay, 0x198);
	CHECK_FIELD(FrChangeRoomInfoDlg, m_roomInfo, 0x19c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CHANGEROOMINFODLG_H
