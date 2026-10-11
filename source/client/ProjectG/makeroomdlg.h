// Original: d:\build\custom\temp\code\client\projectg\makeroomdlg.h
#ifndef PG_CLIENT_PROJECTG_MAKEROOMDLG_H
#define PG_CLIENT_PROJECTG_MAKEROOMDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrTabButton;
class FrViewer;
class FrGaugeBarImage;
class FrTreasureCourse;

class FrGraphicInterface;
class WRect;

// Hole type of a room (PDB enum name; values not transcribed yet).
enum eHoleType {
	eHoleType_None = 0
};

// Room creation form. Its size (0x1d0) is the `new` of
// FrMakeRoomDlgMakeInstance.
class FrMakeRoomDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // makeroomdlg.h:27

	FrMakeRoomDlg(void);
	virtual ~FrMakeRoomDlg(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	bool MakeRoom(bool b);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	bool OnTitleEnterKey(int param);
	void ChangeUserLimit(unsigned char userLimit);
	void ChangeHole(unsigned char hole);
	void ChangeTimeLimit(unsigned long timeLimit);
	void ChangeHoleType(eHoleType holeType);
	void ChangeCourse(unsigned char course);
	void ChangeGameType(unsigned char gameType, bool b);
	void SetDefaultTitle(void);
	bool IsBtnEnable(void);

	void OnHoleMinUp(void);
	void OnHolePrevUp(void);
	void OnHoleNextUp(void);
	void OnHoleMaxUp(void);
	void OnCourseUp(void);
	void OnCourseChatUp(void);
	void OnMapPrevUp(void);
	void OnMapNextUp(void);
	void OnGameTypeButtonUp(void);
	void OnUserLimitButtonUp(void);
	void OnHoleButtonUp(void);
	void OnTimeLimitButtonUp(void);
	void OnMakeVsDown(void);
	void OnMakeMassDown(void);
	void OnMakeBattleDown(void);
	void OnMakeChatDown(void);
	void OnMakeChaosDown(void);
	bool OnMapSelectDlgResult(int result, FrForm* pForm);
	void DrawHoleTypeButton(FrGraphicInterface* pDevice, eHoleType holeType, WRect rect, bool b);

	void OnMakeVsInit(int param);
	void OnMakeMassInit(int param);
	void OnMakeBattleInit(int param);
	void OnMakeChatInit(int param);
	void OnPictureInit(int param);
	void OnTextInit(intptr_t param);
	void OnTitleInit(intptr_t param);
	void OnPasswordInit(intptr_t param);
	void OnMapPrevInit(intptr_t param);
	void OnMapNextInit(intptr_t param);
	void OnMapPrevChatInit(int param);
	void OnMapNextChatInit(int param);
	void OnGameTypeInit(intptr_t param);
	void OnUserLimitInit(intptr_t param);
	void OnHoleInit(intptr_t param);
	void OnTimeLimitInit(intptr_t param);
	void OnCourseInit(intptr_t param);
	void OnHoleTypeInit(intptr_t param);
	void OnCourseChatInit(int param);
	void OnHoleChatInit(int param);
	void OnHoleMinInit(int param);
	void OnHolePrevInit(int param);
	void OnHoleNextInit(int param);
	void OnHoleMaxInit(int param);
	void OnMakeChaosInit(int param);
	void OnOverGauge_Init(int param);
	void OnGameTypeOwnerDraw(intptr_t param);
	void OnUserLimitOwnerDraw(intptr_t param);
	void OnHoleOwnerDraw(intptr_t param);
	void OnTimeLimitOwnerDraw(intptr_t param);
	void OnHoleChatOwnerDraw(int param);
	void OnCourseOwnerDraw(int param);
	void OnHoleTypeOwnerDraw(int param);

	FrButton* m_pMakeBtn[5];       // 0x110, vs, mass, battle, chat, chaos (OnMakeXxxInit)
	FrViewer* m_pPicture;          // 0x124
	FrEdit* m_pTitleEdit;          // 0x128, the room title (SetDefaultTitle)
	FrArea* m_pText;               // 0x12c
	PADDING(0x130, 0x138);
	FrButton* m_pMapPrev;          // 0x138
	FrButton* m_pMapNext;          // 0x13c
	FrButton* m_pMapPrevChat;      // 0x140
	FrButton* m_pMapNextChat;      // 0x144
	FrTabButton* m_pGameType;      // 0x148
	FrTabButton* m_pUserLimitTab;  // 0x14c
	FrTabButton* m_pHoleTab;       // 0x150
	FrTabButton* m_pTimeLimitTab;  // 0x154
	FrViewer* m_pCourse;           // 0x158
	FrArea* m_pHoleType;           // 0x15c
	FrViewer* m_pCourseChat;       // 0x160
	FrArea* m_pHoleChat;           // 0x164
	FrButton* m_pHoleMin;          // 0x168
	FrButton* m_pHolePrev;         // 0x16c
	FrButton* m_pHoleNext;         // 0x170
	FrButton* m_pHoleMax;          // 0x174
	FrGaugeBarImage* m_pOverGauge; // 0x178 (OnOverGauge_Init)
	const Bitmap* m_pHoleTypeImg[8];   // 0x17c, hole_front/back/random/shuffle, _n and _d each
	const Bitmap* m_pMapSelectImg; // 0x19c, MAKEROOM/map_select_o
	PADDING(0x1a0, 0x1a4);     // FrMakeRoomDlg members
	FrTreasureCourse* m_pMapSelectDlg; // 0x1a4, the open course picker (OnCourseUp); blocks the button delay
	int m_makeBtnIndex;            // 0x1a8, which m_pMakeBtn shows pressed (OnInit)
	unsigned char m_gameType;      // 0x1ac, 3 or 7 from the channel flags, else 0; GetDefaultTitle's argument
	unsigned char m_userLimit;     // 0x1ad
	PADDING(0x1ae, 0x1b0);
	unsigned long m_timeLimit;     // 0x1b0
	unsigned char m_hole;          // 0x1b4, hole count (18 limits the hole types)
	unsigned char m_course;        // 0x1b5, the selected course (ChangeCourse)
	unsigned char m_holeType;      // 0x1b6, an eHoleType
	PADDING(0x1b7, 0x1b8);
	int m_startHole;           // 0x1b8, hole picked by the min/prev/next/max buttons (1-18)
	unsigned long m_gameTime;      // 0x1bc, game time limit in ms sent by MakeRoom (mass games), cleared by OnInit
	unsigned long m_turnTime;      // 0x1c0, shot time limit in ms sent by MakeRoom (other games), cleared by OnInit
	float m_courseX;               // 0x1c4, m_pCourse's x at OnInit
	float m_btnDelay;              // 0x1c8, 0.7 s countdown before the buttons enable
	PADDING(0x1cc, 0x1d0);
};

CHECK_SIZE(FrMakeRoomDlg, 0x1d0);
BEGIN_LAYOUT_CHECK(FrMakeRoomDlg)
	CHECK_FIELD(FrMakeRoomDlg, m_pMakeBtn, 0x110);
	CHECK_FIELD(FrMakeRoomDlg, m_pTitleEdit, 0x128);
	CHECK_FIELD(FrMakeRoomDlg, m_pPicture, 0x124);
	CHECK_FIELD(FrMakeRoomDlg, m_pText, 0x12c);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapPrev, 0x138);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapNext, 0x13c);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapPrevChat, 0x140);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapNextChat, 0x144);
	CHECK_FIELD(FrMakeRoomDlg, m_pGameType, 0x148);
	CHECK_FIELD(FrMakeRoomDlg, m_pUserLimitTab, 0x14c);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleTab, 0x150);
	CHECK_FIELD(FrMakeRoomDlg, m_pTimeLimitTab, 0x154);
	CHECK_FIELD(FrMakeRoomDlg, m_pCourse, 0x158);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleType, 0x15c);
	CHECK_FIELD(FrMakeRoomDlg, m_pCourseChat, 0x160);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleChat, 0x164);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleMin, 0x168);
	CHECK_FIELD(FrMakeRoomDlg, m_pHolePrev, 0x16c);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleNext, 0x170);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleMax, 0x174);
	CHECK_FIELD(FrMakeRoomDlg, m_pOverGauge, 0x178);
	CHECK_FIELD(FrMakeRoomDlg, m_pHoleTypeImg, 0x17c);
	CHECK_FIELD(FrMakeRoomDlg, m_gameType, 0x1ac);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapSelectDlg, 0x1a4);
	CHECK_FIELD(FrMakeRoomDlg, m_pMapSelectImg, 0x19c);
	CHECK_FIELD(FrMakeRoomDlg, m_makeBtnIndex, 0x1a8);
	CHECK_FIELD(FrMakeRoomDlg, m_hole, 0x1b4);
	CHECK_FIELD(FrMakeRoomDlg, m_course, 0x1b5);
	CHECK_FIELD(FrMakeRoomDlg, m_gameTime, 0x1bc);
	CHECK_FIELD(FrMakeRoomDlg, m_turnTime, 0x1c0);
	CHECK_FIELD(FrMakeRoomDlg, m_courseX, 0x1c4);
	CHECK_FIELD(FrMakeRoomDlg, m_userLimit, 0x1ad);
	CHECK_FIELD(FrMakeRoomDlg, m_timeLimit, 0x1b0);
	CHECK_FIELD(FrMakeRoomDlg, m_holeType, 0x1b6);
	CHECK_FIELD(FrMakeRoomDlg, m_startHole, 0x1b8);
	CHECK_FIELD(FrMakeRoomDlg, m_btnDelay, 0x1c8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MAKEROOMDLG_H
