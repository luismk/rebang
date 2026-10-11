// Original: d:\build\custom\temp\code\client\projectg\gui_control.h
#ifndef PG_CLIENT_PROJECTG_GUI_CONTROL_H
#define PG_CLIENT_PROJECTG_GUI_CONTROL_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "TaskMain.h"
#include "wlist.h"
#include "woverlay.h"
#include "wresrcmng.h"

class Bitmap;
class CShotCountInfo;
class CSimpleUI;
class CWindInfo;
class FrButton;
class FrChatFuncDlg;
class FrReportDlg;
class IObject;
class WFont;
class WOverlay;
class WRect;
class WPuppet;
class WTitleFont;

// The round screen (HUD) task; its size (0x40C) is the `new` of
// CScreenMakeInstance, a friend (the constructor is protected). Only the
// members this unit defines so far are named.
class CScreen : public CTaskMain {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CScreenMakeInstance(void);
public:
	virtual ~CScreen(void) {}   // gui_control.h:29

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // gui_control.h:31

	// A named overlay of the screen's overlay list (AddOverlay).
	struct sOverlay {
		sOverlay(void) {   // gui_control.h:104
			m_pOverlay = NULL;
		}

		~sOverlay(void) {   // gui_control.h:109
			if (g_resrcmng && m_pOverlay) { g_resrcmng->Release(m_pOverlay); m_pOverlay = 0; }
		}

		char m_name[0x20];      // 0x00
		WOverlay* m_pOverlay;   // 0x20
	};

	virtual void OnPreLoadInit(void);
	virtual void OnInit(void);

protected:
	CScreen(void);

	void AddOverlay(const char* name, const char* file, int flags);
	void DrawOverlay(const char* name, const WRect& rect, unsigned char alpha, int flags);
	void DrawOverlay2(const char* name, const WRect& rect, int color, int flags);

private:
	void DrawCammodeInfo(void);
	void DrawWindInfo(void);
	void DrawRemainTime(void);
	void DrawCommandArrow(void);
	bool OnTrainingOptionDlgResult(int ret, FrForm* pForm);

protected:

private:
	PADDING(0x1E0, 0x1E8);
	CWindInfo* m_pWindInfo;        // 0x1E8
	PADDING(0x1EC, 0x1FC);
	WPuppet* m_pWindPuppet;        // 0x1FC, drawn by CWind::Display
	PADDING(0x200, 0x218);
	CSimpleUI* m_pCammodeUI;       // 0x218, DrawCammodeInfo
	PADDING(0x21C, 0x228);
	CSimpleUI* m_pCommandArrowUI;  // 0x228, DrawCommandArrow
	PADDING(0x22C, 0x260);
	WTitleFont* m_pTitleFont;      // 0x260, [font_wind.jpg digits (DrawApproachInfo)
	CSimpleUI* m_pMissionUI;       // 0x264, text_mission.png (DrawApproachInfo)
	PADDING(0x268, 0x26C);
	float m_accumValue;            // 0x26C, HandleMsg sets or adds a message's float and copies it on
	float m_fadeValue;             // 0x270, HandleMsg sets 1, OnProcess runs it down
	WList<sOverlay*> m_overlays;   // 0x274
	PADDING(0x294, 0x298);
	float m_remainTimeArcShown;    // 0x298, DrawRemainTime copies m_remainTimeArc while not captured
	float m_remainTimeArc;         // 0x29C, start angle of the remaining-time arc
	unsigned long m_remainTimeColor; // 0x2A0
	bool m_hudVisible;             // 0x2A4, OnDisplay draws the HUD while set
	PADDING(0x2A5, 0x2AC);
	int m_msgColorTime;            // 0x2AC, set by HandleMsg
	unsigned long m_msgColor;      // 0x2B0, alpha-faded colour set by HandleMsg
	PADDING(0x2B4, 0x2B8);
	int m_hitBarFrom;              // 0x2B8, DrawHitBarInfo range start
	int m_hitBarCount;             // 0x2BC, DrawHitBarInfo scale count (10)
	PADDING(0x2C0, 0x2C4);
	float m_hitBarTime;            // 0x2C4, DrawHitBarInfo / OnProcess
	float m_remainTimeY;           // 0x2C8, the remaining-time circle's y
	PADDING(0x2CC, 0x374);
	float m_blinkTime;             // 0x374, OnProcess adds the elapsed time (0.25 s period)
	unsigned long m_drawFlags;     // 0x378, OnInit sets bit 2
	float m_cammodeAlpha;          // 0x37C, alpha of the camera mode text
	PADDING(0x380, 0x3AC);
	int m_selectedIndex;           // 0x3AC, -1 after OnInit (HandleMsg)
	float m_bannerTime;            // 0x3B0, OnDisplay shows a banner while positive
	PADDING(0x3B4, 0x3E0);
	bool m_turnStarted;            // 0x3E0, HandleMsg sets it in game state 0xC
	bool m_unknown3E1;             // 0x3E1, set by OnPreLoadInit
	bool m_unknown3E2;             // 0x3E2, cleared by OnPreLoadInit
	bool m_unknown3E3;             // 0x3E3, cleared by OnPreLoadInit
	float m_turnTime;              // 0x3E4, OnProcess
	PADDING(0x3E8, 0x3F4);
	float m_clubAngle;             // 0x3F4, the club preview's rotation (2 pi at OnInit; DrawClubInfo)
	int m_clubSpinDir;             // 0x3F8, OnProcess: 1, -1 or 0
	float m_clubSwing;             // 0x3FC, the club preview's swing phase (DrawClubInfo)
	PADDING(0x400, 0x404);
	int m_clubBoundingBox;         // 0x404, the club puppet's bounding-box script handle
	unsigned char m_playerCount;   // 0x408, set by HandleMsg, looped over by OnDisplay
	PADDING(0x409, 0x40C);
};

CHECK_SIZE(CScreen::sOverlay, 0x24);
CHECK_SIZE(CScreen, 0x40C);
BEGIN_LAYOUT_CHECK(CScreen)
	CHECK_FIELD(CScreen, m_pWindInfo, 0x1E8);
	CHECK_FIELD(CScreen, m_pWindPuppet, 0x1FC);
	CHECK_FIELD(CScreen, m_pCammodeUI, 0x218);
	CHECK_FIELD(CScreen, m_pCommandArrowUI, 0x228);
	CHECK_FIELD(CScreen, m_pTitleFont, 0x260);
	CHECK_FIELD(CScreen, m_pMissionUI, 0x264);
	CHECK_FIELD(CScreen, m_accumValue, 0x26C);
	CHECK_FIELD(CScreen, m_fadeValue, 0x270);
	CHECK_FIELD(CScreen, m_overlays, 0x274);
	CHECK_FIELD(CScreen, m_remainTimeArcShown, 0x298);
	CHECK_FIELD(CScreen, m_remainTimeArc, 0x29C);
	CHECK_FIELD(CScreen, m_remainTimeColor, 0x2A0);
	CHECK_FIELD(CScreen, m_hudVisible, 0x2A4);
	CHECK_FIELD(CScreen, m_msgColorTime, 0x2AC);
	CHECK_FIELD(CScreen, m_msgColor, 0x2B0);
	CHECK_FIELD(CScreen, m_hitBarFrom, 0x2B8);
	CHECK_FIELD(CScreen, m_hitBarCount, 0x2BC);
	CHECK_FIELD(CScreen, m_hitBarTime, 0x2C4);
	CHECK_FIELD(CScreen, m_remainTimeY, 0x2C8);
	CHECK_FIELD(CScreen, m_blinkTime, 0x374);
	CHECK_FIELD(CScreen, m_drawFlags, 0x378);
	CHECK_FIELD(CScreen, m_cammodeAlpha, 0x37C);
	CHECK_FIELD(CScreen, m_selectedIndex, 0x3AC);
	CHECK_FIELD(CScreen, m_bannerTime, 0x3B0);
	CHECK_FIELD(CScreen, m_turnStarted, 0x3E0);
	CHECK_FIELD(CScreen, m_unknown3E1, 0x3E1);
	CHECK_FIELD(CScreen, m_unknown3E2, 0x3E2);
	CHECK_FIELD(CScreen, m_unknown3E3, 0x3E3);
	CHECK_FIELD(CScreen, m_turnTime, 0x3E4);
	CHECK_FIELD(CScreen, m_clubAngle, 0x3F4);
	CHECK_FIELD(CScreen, m_clubSpinDir, 0x3F8);
	CHECK_FIELD(CScreen, m_clubSwing, 0x3FC);
	CHECK_FIELD(CScreen, m_clubBoundingBox, 0x404);
	CHECK_FIELD(CScreen, m_playerCount, 0x408);
END_LAYOUT_CHECK

IObject* CScreenMakeInstance(void);

// Mini score board of the round screen; its size (0x7C) is the `new` of
// CScreen's constructor. Only the members this unit defines so far are
// named.
class CMiniScore {
	LAYOUT_CHECK_FRIEND;
public:
	CMiniScore(void);
	~CMiniScore(void);

	void ChangeCharacter(void);
	WRect GetIconRect(int flags);
	void Flicker(void);
	void ResetUiPosition(void);
	void Show(void);
	void Hide(void);
	void StartFloating(void);
	void Process(float elapsed);
	void Display(void);

private:
	CShotCountInfo* m_pShotCountInfo;   // 0x00
	WOverlay* m_pOverlay04;             // 0x04
	const Bitmap* m_pCharBitmap[4];     // 0x08, ChangeCharacter
	const Bitmap* m_pVsIcon;            // 0x18, golf/pc_icon_vs
	WOverlay* m_pOverlay1C;             // 0x1C
	WOverlay* m_pOverlay20;             // 0x20
	WOverlay* m_pOverlay24;             // 0x24
	WOverlay* m_pOverlay28;             // 0x28
	CSimpleUI* m_pSimpleUI;             // 0x2C
	WFont* m_pFont;                     // 0x30, CChatMsg's masked font
	WRect m_iconUV;                     // 0x34, (0, 0, 0.46875, 1) after construction
	PADDING(0x44, 0x4C);
	float m_iconWidth;                  // 0x4C
	float m_iconHeight;                 // 0x50
	PADDING(0x54, 0x64);
	float m_offsetY;                    // 0x64
	float m_posX;                       // 0x68
	int m_state;                        // 0x6C, 0 flicker, 1 hide, 2 show
	float m_stateTime;                  // 0x70
	unsigned char m_alpha;              // 0x74
	PADDING(0x75, 0x78);
	unsigned long m_gameMode;           // 0x78, the CGolfDoc game mode Process last saw
};

CHECK_SIZE(CMiniScore, 0x7C);
BEGIN_LAYOUT_CHECK(CMiniScore)
	CHECK_FIELD(CMiniScore, m_pShotCountInfo, 0x00);
	CHECK_FIELD(CMiniScore, m_pOverlay04, 0x04);
	CHECK_FIELD(CMiniScore, m_pCharBitmap, 0x08);
	CHECK_FIELD(CMiniScore, m_pVsIcon, 0x18);
	CHECK_FIELD(CMiniScore, m_pOverlay1C, 0x1C);
	CHECK_FIELD(CMiniScore, m_pOverlay28, 0x28);
	CHECK_FIELD(CMiniScore, m_pSimpleUI, 0x2C);
	CHECK_FIELD(CMiniScore, m_pFont, 0x30);
	CHECK_FIELD(CMiniScore, m_iconUV, 0x34);
	CHECK_FIELD(CMiniScore, m_iconWidth, 0x4C);
	CHECK_FIELD(CMiniScore, m_iconHeight, 0x50);
	CHECK_FIELD(CMiniScore, m_offsetY, 0x64);
	CHECK_FIELD(CMiniScore, m_posX, 0x68);
	CHECK_FIELD(CMiniScore, m_state, 0x6C);
	CHECK_FIELD(CMiniScore, m_stateTime, 0x70);
	CHECK_FIELD(CMiniScore, m_alpha, 0x74);
	CHECK_FIELD(CMiniScore, m_gameMode, 0x78);
END_LAYOUT_CHECK

// Option menu of the round screen (pause, drop, chat record); its size
// (0x30) is the `new` of CScreen's constructor.
class CAuxMenuUI : public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
public:
	CAuxMenuUI(void);
	virtual ~CAuxMenuUI(void);

	void Display(void);
	void SetActive(bool active, int index);
	void ChangeTurn(void);
	void SetPause(void);
	void OpenChatRecord(void);
	void SetUnplayable(void);
	void Process(float elapsed);

private:
	bool OnChatFuncDlgResult(int ret, FrForm* pForm);
	bool OnDropFailDlgResult(int ret, FrForm* pForm);
	bool OnDropDlgResult(int ret, FrForm* pForm);

	WOverlay* m_pBase;                  // 0x04, [option_base.png
	// 0x08: menu item background, [0] [option_black.png, [1]
	// [option_white.png for the item under the cursor or the current one.
	WOverlay* m_pItemOverlay[2];        // 0x08
	int m_hoverIndex;                   // 0x10, the menu item under the cursor (cursor area - 0x27)
	int m_active;                       // 0x14, 0 closed, 1 opening, 2 open, 3 closing
	int m_index;                        // 0x18
	float m_fade;                       // 0x1C, 0..1 while opening/closing
	FrForm* m_pDropDlg;                 // 0x20
	FrForm* m_pDropFailDlg;             // 0x24
	unsigned char m_unknown28;          // 0x28, 2 after construction
	FrChatFuncDlg* m_pChatFuncDlg;      // 0x2C
};

CHECK_SIZE(CAuxMenuUI, 0x30);
BEGIN_LAYOUT_CHECK(CAuxMenuUI)
	CHECK_FIELD(CAuxMenuUI, m_pBase, 0x04);
	CHECK_FIELD(CAuxMenuUI, m_pItemOverlay, 0x08);
	CHECK_FIELD(CAuxMenuUI, m_hoverIndex, 0x10);
	CHECK_FIELD(CAuxMenuUI, m_active, 0x14);
	CHECK_FIELD(CAuxMenuUI, m_index, 0x18);
	CHECK_FIELD(CAuxMenuUI, m_fade, 0x1C);
	CHECK_FIELD(CAuxMenuUI, m_pDropDlg, 0x20);
	CHECK_FIELD(CAuxMenuUI, m_pDropFailDlg, 0x24);
	CHECK_FIELD(CAuxMenuUI, m_unknown28, 0x28);
	CHECK_FIELD(CAuxMenuUI, m_pChatFuncDlg, 0x2C);
END_LAYOUT_CHECK

// Driving distance board of the round screen. Its size is not known from
// a `new`; 0x20 covers every member its methods touch.
class CDrivingDistInfo {
	LAYOUT_CHECK_FRIEND;
public:
	CDrivingDistInfo(void);
	~CDrivingDistInfo(void);

	void OnChangeTurn(void);
	void Process(float elapsed);
	void Display(WFont* pFont);

private:
	WOverlay* m_pPlayerOverlay;   // 0x00, [nplayer.jpg
	WOverlay* m_pTopOverlay;      // 0x04, [top_drv.jpg
	bool m_unknown08;             // 0x08, true after construction
	bool m_unknown09;             // 0x09
	unsigned char m_bestPlayer;   // 0x0A, player index of the longest drive, 0xFF after construction
	unsigned char m_unknown0B;    // 0x0B, 0xFF after construction
	unsigned long m_bestColor;    // 0x0C, colour of the longest drive's text, -1 after construction
	unsigned long m_myColor;      // 0x10, colour of this player's drive text, -1 after construction
	float m_shownDistance;       // 0x14, copy of m_bestDistance (OnChangeTurn)
	float m_bestDistance;        // 0x18, the longest drive (yards)
	float m_myDistance;          // 0x1C, this player's drive (yards)
};

CHECK_SIZE(CDrivingDistInfo, 0x20);
BEGIN_LAYOUT_CHECK(CDrivingDistInfo)
	CHECK_FIELD(CDrivingDistInfo, m_pPlayerOverlay, 0x00);
	CHECK_FIELD(CDrivingDistInfo, m_pTopOverlay, 0x04);
	CHECK_FIELD(CDrivingDistInfo, m_unknown08, 0x08);
	CHECK_FIELD(CDrivingDistInfo, m_unknown09, 0x09);
	CHECK_FIELD(CDrivingDistInfo, m_bestPlayer, 0x0A);
	CHECK_FIELD(CDrivingDistInfo, m_unknown0B, 0x0B);
	CHECK_FIELD(CDrivingDistInfo, m_bestColor, 0x0C);
	CHECK_FIELD(CDrivingDistInfo, m_myColor, 0x10);
	CHECK_FIELD(CDrivingDistInfo, m_shownDistance, 0x14);
	CHECK_FIELD(CDrivingDistInfo, m_bestDistance, 0x18);
	CHECK_FIELD(CDrivingDistInfo, m_myDistance, 0x1C);
END_LAYOUT_CHECK

// Chat function dialog (show/hide chat, save the chat history, report a
// player); its size (0x130) is the `new` of FrChatFuncDlgMakeInstance.
// Only the members this unit defines so far are declared.
class FrChatFuncDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // gui_control.h:353

	FrChatFuncDlg(void);
	virtual ~FrChatFuncDlg(void);

	void AddChatMsg(const char* msg, int color);

protected:
	void OnBtnShowChatUp(void);
	void OnBtnHideChatUp(void);
	void OnBtnSaveChatUp(void);
	bool OnReportDlgResult(int ret, FrForm* pForm);
	void OnBtnAccuseUp(void);
	void OnBtnCloseDlgUp(void);
	void OnHideChatInit(intptr_t param);
	void OnShowChatInit(intptr_t param);
	void OnChatViewInit(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	void* m_pUnknown110;           // 0x110, cleared by the constructor
	FrButton* m_pBtnShowChat;      // 0x114, OnShowChatInit
	FrButton* m_pBtnHideChat;      // 0x118, OnHideChatInit
	void* m_pUnknown11C;           // 0x11C, cleared by the constructor
	void* m_pUnknown120;           // 0x120, cleared by the constructor
	FrEdit* m_pChatView;           // 0x124, OnChatViewInit
	FrReportDlg* m_pReportDlg;     // 0x128, OnBtnAccuseUp
	PADDING(0x12C, 0x130);
};

CHECK_SIZE(FrChatFuncDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrChatFuncDlg)
	CHECK_FIELD(FrChatFuncDlg, m_pUnknown110, 0x110);
	CHECK_FIELD(FrChatFuncDlg, m_pBtnShowChat, 0x114);
	CHECK_FIELD(FrChatFuncDlg, m_pBtnHideChat, 0x118);
	CHECK_FIELD(FrChatFuncDlg, m_pUnknown11C, 0x11C);
	CHECK_FIELD(FrChatFuncDlg, m_pUnknown120, 0x120);
	CHECK_FIELD(FrChatFuncDlg, m_pChatView, 0x124);
	CHECK_FIELD(FrChatFuncDlg, m_pReportDlg, 0x128);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GUI_CONTROL_H
