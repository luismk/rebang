// Original: d:\build\custom\temp\code\client\projectg\treasuredlg.h
#ifndef PG_CLIENT_PROJECTG_TREASUREDLG_H
#define PG_CLIENT_PROJECTG_TREASUREDLG_H

#include <vector>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"
#include "../client/Wangreal/include/wtypes.h"

struct FrListItem;
class Bitmap;
class CSimpleUI;
class FrArea;
class FrButton;
class FrComboCtlEx;
class FrGraphicInterface;
class FrGaugeBar;
class FrGaugeBarImage;
class FrViewer;
class WOverlay;

namespace nsTHunter {
	bool IsWndSizeMinimum(void);
	unsigned long CalcGauge(unsigned long value);
}

class FrListBox;
class WTitleFont;

// Treasure-box opening sequence embedded in the dialogs that hand out gifts
// (size 0x7c, the extent its constructor writes).
class CBoxOpener {
	LAYOUT_CHECK_FRIEND;
	friend class FrTreasureGift;
	friend class FrDemonFinalScore;   // OnInit_Gift_List (s5/DemonLandDlg)

public:
	// A gift shown in the gift list (size 0x14).
	struct sTGift {
		sTGift(void) { itemIdx = -1; m_unknown4 = 0; m_unknown8 = 0; m_unknownc = 0; m_unknowne = 0; m_unknownf = 0; m_unknown10 = 0; }   // treasuredlg.h:482
		void Reset(void) { itemIdx = -1; m_unknown4 = 0; m_unknown8 = 0; m_unknownc = 0; m_unknowne = 0; m_unknownf = 0; m_unknown10 = 0; }   // treasuredlg.h:483

		int itemIdx;                 // 0x0, -1 when empty
		int m_unknown4;              // 0x4
		int m_unknown8;              // 0x8
		short m_unknownc;            // 0xc
		unsigned char m_unknowne;    // 0xe
		unsigned char m_unknownf;    // 0xf
		int m_unknown10;             // 0x10, 1 once ProcInit ran
	};

	// Opening sequence steps (ChangeSequence); names from their Proc* handlers.
	enum eSeqProc {
		SEQ_NONE = 0,
		SEQ_INIT = 1,
		SEQ_CREATE = 2,
		SEQ_MYITEM_OPEN = 3,
		SEQ_COMPLETE = 4,
		SEQ_UNKNOWN5 = 5,
		SEQ_END = 6,
		SEQ_CLOSE = 7
	};

	CBoxOpener(void);
	~CBoxOpener(void);

	bool OnInit(void);
	void OnProc(float dt);
	void DrawGiftList(FrListItem* pItem);
	void DrawBoxArea(void);

private:
	void ChangeSequence(eSeqProc seq);
	void ProcInit(void);
	void ProcCreate(void);
	void ProcMyItemOpen(void);
	void ProcComplete(void);
	void ProcBoxNone(void);
	void ProcEnd(void);
	sTGift* GetGiftItem(int index);
	void GetItemOffset(WPoint& pt, unsigned long typeId, bool bOpened);

	std::vector<sTGift> m_gifts;         // 0x0
	FrListBox* m_pGiftList;              // 0x10
	FrArea* m_pBoxArea;                  // 0x14, the box drawing area (FrTreasureGift::OnInit_Box_Area)
	void (CBoxOpener::*m_pProcFunc)(void);   // 0x18, the current step's Proc* handler
	CSimpleUI* m_pSimpleUI;              // 0x1c, deleted by the destructor
	PADDING(0x20, 0x48);
	int m_unknown48;                     // 0x48
	eSeqProc m_nextSeq;                  // 0x4c, step to switch to on the next OnProc
	eSeqProc m_curSeq;                   // 0x50
	unsigned char m_unknown54;           // 0x54
	PADDING(0x55, 0x5c);
	int m_openGiftIdx;                   // 0x5c, next gift ProcCreate/ProcMyItemOpen opens
	int m_giftCount;                     // 0x60, gifts in the list
	int m_unknown64;                     // 0x64
	unsigned long m_seqDuration;         // 0x68, ms the current step lasts
	unsigned long m_seqStartTime;        // 0x6c, timeGetTime() at the step's start
	float m_dt;                          // 0x70, last OnProc delta
	unsigned char m_unknown74;           // 0x74
	PADDING(0x75, 0x78);
	WTitleFont* m_pTitleFont;            // 0x78
};

CHECK_SIZE(CBoxOpener::sTGift, 0x14);
CHECK_SIZE(CBoxOpener, 0x7c);
BEGIN_LAYOUT_CHECK(CBoxOpener)
	CHECK_FIELD(CBoxOpener, m_pGiftList, 0x10);
	CHECK_FIELD(CBoxOpener, m_pBoxArea, 0x14);
	CHECK_FIELD(CBoxOpener, m_pProcFunc, 0x18);
	CHECK_FIELD(CBoxOpener, m_pSimpleUI, 0x1c);
	CHECK_FIELD(CBoxOpener, m_unknown48, 0x48);
	CHECK_FIELD(CBoxOpener, m_nextSeq, 0x4c);
	CHECK_FIELD(CBoxOpener, m_unknown54, 0x54);
	CHECK_FIELD(CBoxOpener, m_unknown64, 0x64);
	CHECK_FIELD(CBoxOpener, m_seqStartTime, 0x6c);
	CHECK_FIELD(CBoxOpener, m_dt, 0x70);
	CHECK_FIELD(CBoxOpener, m_pTitleFont, 0x78);
END_LAYOUT_CHECK

// Test dialog with a combo box (size 0x118, the `new` of
// FrTestDlg_comboMakeInstance). Only the members defined so far are declared.
class FrTestDlg_combo : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:599

	FrTestDlg_combo(void);
	virtual ~FrTestDlg_combo(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	void OnInit_combo(intptr_t param);
	void ONLBTN_DOWN(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	FrComboCtlEx* m_pCombo;   // 0x110
	PADDING(0x114, 0x118);
};

CHECK_SIZE(FrTestDlg_combo, 0x118);
BEGIN_LAYOUT_CHECK(FrTestDlg_combo)
	CHECK_FIELD(FrTestDlg_combo, m_pCombo, 0x110);
END_LAYOUT_CHECK

// Next-hole indicator of the treasure-hunter mode (size 0x150, the `new` of
// FrNextHoleMakeInstance). Only the members defined so far are declared.
class FrNextHole : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	// Display state (SetState); its values are not known yet.
	enum eState {
		STATE_NONE = 0,
		STATE_HIDE = 1    // SetState disables and hides the form
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:33

	FrNextHole(void);
	virtual ~FrNextHole(void);

	virtual bool OnInit(void);

	WPoint GetCalcPoint(void);
	void SetOutFrameVisible(bool bVisible);
	void SetNextHole(int hole);
	void SetState(eState state);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnAreaNextHole_Init(intptr_t param);
	void OnAreaNextHole_OwnerDraw(int param);
	void OnArea_OutFrame_Init(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	WOverlay* m_pOverlay[3];             // 0x110, next-hole images (%s_1.png, %s_2.png, next_chaos.tga)
	WRect m_drawRect;                    // 0x11c
	FrArea* m_pNextHoleArea;             // 0x12c
	std::vector<FrArea*> m_outFrames;    // 0x130
	int m_overlayIdx;                    // 0x140, index into m_pOverlay
	eState m_state;                      // 0x144
	int m_overlayWidth;                  // 0x148, from the first overlay
	int m_overlayHeight;                 // 0x14c
};

CHECK_SIZE(FrNextHole, 0x150);
BEGIN_LAYOUT_CHECK(FrNextHole)
	CHECK_FIELD(FrNextHole, m_pOverlay, 0x110);
	CHECK_FIELD(FrNextHole, m_drawRect, 0x11c);
	CHECK_FIELD(FrNextHole, m_pNextHoleArea, 0x12c);
	CHECK_FIELD(FrNextHole, m_outFrames, 0x130);
	CHECK_FIELD(FrNextHole, m_overlayIdx, 0x140);
	CHECK_FIELD(FrNextHole, m_state, 0x144);
	CHECK_FIELD(FrNextHole, m_overlayWidth, 0x148);
	CHECK_FIELD(FrNextHole, m_overlayHeight, 0x14c);
END_LAYOUT_CHECK

// Treasure-point gauge of the treasure-hunter mode (size 0x130, the `new` of
// FrTreasureGaugeMakeInstance). Only the members defined so far are declared.
class FrTreasureGauge : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	// Display state (SetState); its values are not known yet.
	enum eState {
		STATE_NONE = 0
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:93

	FrTreasureGauge(void);
	virtual ~FrTreasureGauge(void);

	virtual bool OnInit(void);
	void SetTPoint(void);
	WPoint GetCalcPoint(void);
	void SetOutFrameVisible(bool bVisible);
	void SetState(eState state);   // treasuredlg.cpp:362

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnImageBarInit(int param);
	void OnArea_Arrow_Init(int param);
	void OnArea_Arrow_OwnerDraw(int param);
	void OnArea_OutFrame_Init(intptr_t param);

private:
	FrGaugeBarImage* m_pGauge;           // 0x110, the treasure-point gauge
	FrArea* m_pArrowArea;                // 0x114
	std::vector<FrArea*> m_outFrames;    // 0x118
	eState m_state;                      // 0x128
	const Bitmap* m_pArrowBitmap;        // 0x12c, th_gage_arrow
};

CHECK_SIZE(FrTreasureGauge, 0x130);
BEGIN_LAYOUT_CHECK(FrTreasureGauge)
	CHECK_FIELD(FrTreasureGauge, m_pGauge, 0x110);
	CHECK_FIELD(FrTreasureGauge, m_pArrowArea, 0x114);
	CHECK_FIELD(FrTreasureGauge, m_outFrames, 0x118);
	CHECK_FIELD(FrTreasureGauge, m_state, 0x128);
	CHECK_FIELD(FrTreasureGauge, m_pArrowBitmap, 0x12c);
END_LAYOUT_CHECK

// Treasure-hunter course selection dialog. Only the members defined or used
// from other units so far are declared; its full size is not known yet.
class FrTreasureCourse : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	// A drifting cloud of the course map (size 0x20).
	struct sCloud {
		sCloud(void) { srcRect.x = 0; srcRect.y = 0; srcRect.w = 1.0f; srcRect.h = 1.0f; m_unknown10 = 0; m_unknown14 = 0; m_unknown18 = 0; alpha = 0; m_unknown1d = false; }   // treasuredlg.h:165

		WRect srcRect;          // 0x0, source rect in m_pCloudOverlay (scrolled along x by ProcCloud)
		float m_unknown10;      // 0x10
		float m_unknown14;      // 0x14
		float m_unknown18;      // 0x18
		unsigned char alpha;    // 0x1c, ShiftAlpha fades it
		bool m_unknown1d;       // 0x1d
	};

	// Cloud fade direction (ShiftAlpha).
	enum eALPHA {
		ALPHA_IN = 0,
		ALPHA_OUT = 1
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:147

	FrTreasureCourse(void);
	virtual ~FrTreasureCourse(void);

	void SelectMapBtn(unsigned char mapIdx);
	void SetSelectMap(unsigned char mapType);
	void SetCurMapIdx(unsigned char mapIdx);
	unsigned char GetCurMapIdx(void);
	void SetMapGauge(void);
	bool GetMapIndexChanged(void) const;
	void SetMapIndexChanged(bool changed);

	// treasuredlg.h:188
	void EnableRandomBtn(bool enable) {
		m_randomBtn->Enable(enable);
	}

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnBtnMap_Lagoon_Down(void);
	void OnBtnMap_Water_Down(void);
	void OnBtnMap_Sepia_Down(void);
	void OnBtnMap_Hill_Down(void);
	void OnBtnMap_WizWiz_Down(void);
	void OnBtnMap_West_Down(void);
	void OnBtnMap_Moon_Down(void);
	void OnBtnMap_Silvia_Down(void);
	void OnBtnMap_Cannon_Down(void);
	void OnBtnMap_White_Down(void);
	void OnBtnMap_Shining_Down(void);
	void OnBtnMap_Pink_Down(void);
	void OnBtnMap_Inferno_Down(void);
	void OnBtnMap_IceSpa_Down(void);
	void OnBtnMap_SeaWay_Down(void);
	void OnBtnMap_Panda_Down(void);
	void OnBtnMap_Wizcity_Down(void);
	void OnBtnMap_Random_Down(void);

	void OnAreaSelectMapInit(int param);
	void OnBtnMap_ALL_Init(int param);
	void OnBtnMap_Random_Init(int param);
	void OnViewerF1(int param);
	void OnViewerF2(int param);
	void OnViewer_CurrMap_Init(int param);
	void OnArea_Explane_Init(int param);
	void OnArea_Explane_OwnerDraw(int param);
	void OnImageBar(int param);
	void OnGauge_SmallBar(int param);
	void OnGauge_SmallBar_OwnerDraw(int param);
	void OnInit_Cloud_Area(int param);
	void OnOwnerDraw_Cloud_Area(int param);

	void OnBtnMap_Lagoon_OwnerDraw(int);
	void OnBtnMap_Water_OwnerDraw(int);
	void OnBtnMap_Sepia_OwnerDraw(int);
	void OnBtnMap_Hill_OwnerDraw(int);
	void OnBtnMap_WizWiz_OwnerDraw(int);
	void OnBtnMap_West_OwnerDraw(int);
	void OnBtnMap_Moon_OwnerDraw(int);
	void OnBtnMap_Silvia_OwnerDraw(int);
	void OnBtnMap_Cannon_OwnerDraw(int);
	void OnBtnMap_White_OwnerDraw(int);
	void OnBtnMap_Shining_OwnerDraw(int);
	void OnBtnMap_Pink_OwnerDraw(int);
	void OnBtnMap_Inferno_OwnerDraw(int);
	void OnBtnMap_IceSpa_OwnerDraw(int);
	void OnBtnMap_SeaWay_OwnerDraw(int);
	void OnBtnMap_Panda_OwnerDraw(int);
	void OnBtnMap_Wizcity_OwnerDraw(int);

private:
	bool OpenGauge(WPoint& pt, int mapIdx);
	void OpenSelectedMap(unsigned char mapIdx);
	void OpenCurrentMap(int mapType);
	void DrawMapBtn(eMapType type);
	void DrawCurrentMap(FrGraphicInterface* pGI, int mapIdx);
	void ShiftAlpha(int cloudIdx, float dt, eALPHA type);
	void InitCloud(void);
	void ReInitCloud(int cloudIdx);
	void CheckArea(int cloudIdx, float dt);
	void ProcCloud(float dt);

	std::vector<FrButton*> m_mapBtns;    // 0x110, one per course (20)
	FrWnd* m_randomBtn;                  // 0x120, the random-map button (a FrButton)
	FrArea* m_pSelectMapArea;            // 0x124
	FrViewer* m_pViewerF1;               // 0x128
	FrViewer* m_pViewerF2;               // 0x12c
	FrGaugeBarImage* m_pImageBar;        // 0x130
	FrGaugeBarImage* m_pSmallBar;        // 0x134
	WPoint m_gaugePos[20];               // 0x138, position of each course's small gauge (OnGauge_SmallBar_OwnerDraw)
	FrViewer* m_pCurMapViewer;           // 0x1d8
	FrArea* m_pExplaneArea;              // 0x1dc
	FrArea* m_pCloudArea;                // 0x1e0
	PADDING(0x1e4, 0x1e8);
	unsigned char m_curMapIdx;           // 0x1e8, the selected course
	unsigned char m_unknown1e9;          // 0x1e9
	bool m_mapIndexChanged;              // 0x1ea, set when the selection changed
	PADDING(0x1eb, 0x1ec);
	unsigned char m_unknown1ec;          // 0x1ec
	PADDING(0x1ed, 0x1f4);
	int m_openedMapIdx;                  // 0x1f4, course whose gauge is open, -1 at construction
	int m_cloudCount;                    // 0x1f8, clouds in m_clouds
	int m_mapBtnInitIdx;                 // 0x1fc, next m_mapBtns slot filled by OnBtnMap_ALL_Init
	const Bitmap* m_mapBitmaps[20];      // 0x200, image of each course (DrawCurrentMap)
	const Bitmap* m_randomMapBitmap;     // 0x250, image drawn for the random course (index 0x7f)
	int m_unknown254;                    // 0x254
	PADDING(0x258, 0x25c);
	const Bitmap* m_pExplaneBitmap;      // 0x25c, drawn while the mouse is over m_pExplaneArea
	PADDING(0x260, 0x26c);
	int m_unknown26c;                    // 0x26c
	int m_unknown270;                    // 0x270
	PADDING(0x274, 0x284);
	unsigned long m_mapGauges[20];       // 0x284, treasure gauge of each course (SetMapGauge)
	WOverlay* m_pCloudOverlay;           // 0x2d4
	std::vector<sCloud> m_clouds;        // 0x2d8
};

CHECK_SIZE(FrTreasureCourse::sCloud, 0x20);
CHECK_SIZE(FrTreasureCourse, 0x2e8);

BEGIN_LAYOUT_CHECK(FrTreasureCourse)
	CHECK_FIELD(FrTreasureCourse, m_mapBtns, 0x110);
	CHECK_FIELD(FrTreasureCourse, m_pSmallBar, 0x134);
	CHECK_FIELD(FrTreasureCourse, m_gaugePos, 0x138);
	CHECK_FIELD(FrTreasureCourse, m_pCurMapViewer, 0x1d8);
	CHECK_FIELD(FrTreasureCourse, m_pCloudArea, 0x1e0);
	CHECK_FIELD(FrTreasureCourse, m_unknown1ec, 0x1ec);
	CHECK_FIELD(FrTreasureCourse, m_openedMapIdx, 0x1f4);
	CHECK_FIELD(FrTreasureCourse, m_mapBitmaps, 0x200);
	CHECK_FIELD(FrTreasureCourse, m_randomMapBitmap, 0x250);
	CHECK_FIELD(FrTreasureCourse, m_pExplaneBitmap, 0x25c);
	CHECK_FIELD(FrTreasureCourse, m_unknown26c, 0x26c);
	CHECK_FIELD(FrTreasureCourse, m_mapGauges, 0x284);
	CHECK_FIELD(FrTreasureCourse, m_pCloudOverlay, 0x2d4);
	CHECK_FIELD(FrTreasureCourse, m_clouds, 0x2d8);
	CHECK_FIELD(FrTreasureCourse, m_randomBtn, 0x120);
	CHECK_FIELD(FrTreasureCourse, m_curMapIdx, 0x1e8);
	CHECK_FIELD(FrTreasureCourse, m_mapIndexChanged, 0x1ea);
END_LAYOUT_CHECK

// Score board of the treasure-hunter mode (size 0x128, the `new` of
// FrTreasureScoreMakeInstance).
class FrTreasureScore : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:364

	FrTreasureScore(void);
	virtual ~FrTreasureScore(void);

	void SetTPoint(void);
	void SetState(bool bState);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnGBarImage_ImageBar_Init(intptr_t param);
	void OnView_Backgound_Init(intptr_t param);
	void OnArea_Team_Init(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

	WPoint GetCalcPoint(void);
	void IsTeamMode(void);   // treasuredlg.cpp:1621

	FrGaugeBarImage* m_pGauge;   // 0x110, the treasure-point gauge
	FrViewer* m_pBackground;     // 0x114
	FrArea* m_pTeamArea;         // 0x118
	bool m_bState;               // 0x11c, shown state (SetState)
	PADDING(0x11d, 0x120);
	const Bitmap* m_pTeam0Bitmap;   // 0x120, team badge of a player of team 0 (IsTeamMode)
	const Bitmap* m_pTeam1Bitmap;   // 0x124, team badge of the other team
};

CHECK_SIZE(FrTreasureScore, 0x128);
BEGIN_LAYOUT_CHECK(FrTreasureScore)
	CHECK_FIELD(FrTreasureScore, m_pGauge, 0x110);
	CHECK_FIELD(FrTreasureScore, m_pBackground, 0x114);
	CHECK_FIELD(FrTreasureScore, m_pTeamArea, 0x118);
	CHECK_FIELD(FrTreasureScore, m_bState, 0x11c);
	CHECK_FIELD(FrTreasureScore, m_pTeam0Bitmap, 0x120);
	CHECK_FIELD(FrTreasureScore, m_pTeam1Bitmap, 0x124);
END_LAYOUT_CHECK

// A row of frame areas sized together (size 0x14).
class CTFrame {
	LAYOUT_CHECK_FRIEND;

public:
	CTFrame(void);
	CTFrame(unsigned char count);
	~CTFrame(void);

	void AddFrame(FrArea* pArea);
	bool SetFrameWidth(float width);
	bool SetFrameHeight(float height);

private:
	unsigned char m_count;               // 0x0, frames reserved
	std::vector<FrArea*> m_frames;       // 0x4
};

CHECK_SIZE(CTFrame, 0x14);
BEGIN_LAYOUT_CHECK(CTFrame)
	CHECK_FIELD(CTFrame, m_count, 0x0);
	CHECK_FIELD(CTFrame, m_frames, 0x4);
END_LAYOUT_CHECK

// Gift list shown at the end of a treasure-hunter round (size 0x1d0, the
// `new` of FrTreasureGiftMakeInstance). Only the members defined so far are
// declared.
class FrTreasureGift : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // treasuredlg.h:543

	FrTreasureGift(void);
	virtual ~FrTreasureGift(void);

	WPoint GetCalcPoint(void);
	void SetPointByFrameHeight(void);
	void SetState(bool bState);
	void SetApproachMode(void) { m_boxOpener.m_unknown74 = 1; }   // treasuredlg.h:554

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnInit_RangeArea_Out(int param);
	void OnInit_RangeArea_In(int param);
	void OnInit_Gift_List(intptr_t param);
	void OnInit_GridArea(int param);
	void OnOwnerDraw_GridArea(int param);
	void OnInit_Box_Area(int param);
	void OnOwnerDraw_Gift_List(intptr_t param);
	void OnOwnerDraw_Box_Area(int param);

private:
	CTFrame m_frame0;               // 0x110
	CTFrame m_frame1;               // 0x124
	CTFrame m_frame2;               // 0x138
	FrArea* m_pGridArea;            // 0x14c
	unsigned char m_unknown150;     // 0x150, 8 at construction
	unsigned char m_unknown151;     // 0x151, 1 at construction
	bool m_bState;                  // 0x152, shown state (SetState)
	PADDING(0x153, 0x154);
	CBoxOpener m_boxOpener;         // 0x154
};

CHECK_SIZE(FrTreasureGift, 0x1d0);
BEGIN_LAYOUT_CHECK(FrTreasureGift)
	CHECK_FIELD(FrTreasureGift, m_frame0, 0x110);
	CHECK_FIELD(FrTreasureGift, m_frame1, 0x124);
	CHECK_FIELD(FrTreasureGift, m_frame2, 0x138);
	CHECK_FIELD(FrTreasureGift, m_pGridArea, 0x14c);
	CHECK_FIELD(FrTreasureGift, m_unknown150, 0x150);
	CHECK_FIELD(FrTreasureGift, m_unknown151, 0x151);
	CHECK_FIELD(FrTreasureGift, m_bState, 0x152);
	CHECK_FIELD(FrTreasureGift, m_boxOpener, 0x154);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TREASUREDLG_H
