// Original: d:\build\custom\temp\code\client\projectg\mousecursor.h
#ifndef PG_CLIENT_PROJECTG_MOUSECURSOR_H
#define PG_CLIENT_PROJECTG_MOUSECURSOR_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wmath.h"
#include <vector>

class WRect;
class CSimpleUI;
class FrWnd;
class WOverlay;

// Ground-type tooltip of the cursor (mousecursor.h, mousecursor.cpp:1201).
class CGroundPropt {
public:
	CGroundPropt(const char* name);
	~CGroundPropt(void);

	// mousecursor.h:23
	void SetGroundType(const char* type) { strcpy(m_groundType, type); }

	// mousecursor.h:25
	CSimpleUI* GetGroundTip(void) { return m_pGroundTip; }

	void DrawGroundTip(void);
	void SelectGroundTip(void);

protected:
	char m_groundType[0x10];    // 0x0
	CSimpleUI* m_pGroundTip;    // 0x10

	LAYOUT_CHECK_FRIEND;
};
BEGIN_LAYOUT_CHECK(CGroundPropt)
	CHECK_FIELD(CGroundPropt, m_groundType, 0x0);
	CHECK_FIELD(CGroundPropt, m_pGroundTip, 0x10);
END_LAYOUT_CHECK

// One cursor image of CMouseCursor (array of 12, 0x14 bytes each).
struct sCursorImage {
	WOverlay* m_pOverlay;    // 0x0, released through g_resrcmng
	float m_width;           // 0x4, texture width (all frames side by side)
	float m_height;          // 0x8, texture height
	float m_frameSize;       // 0xC, drawn size of one frame (32)
	int m_frameCount;        // 0x10, frames in the strip (m_width / 32)
};
CHECK_SIZE(sCursorImage, 0x14);
CHECK_FIELD(sCursorImage, m_width, 0x4);
CHECK_FIELD(sCursorImage, m_height, 0x8);
CHECK_FIELD(sCursorImage, m_frameSize, 0xC);
CHECK_FIELD(sCursorImage, m_frameCount, 0x10);

class CMouseCursor : public WSingleton<CMouseCursor> {
	friend class CCalipers;
public:
	CMouseCursor(void);
	virtual ~CMouseCursor(void);

	enum eMode {
		SCREEN_LOBBY = 0,
		SCREEN_GAME = 1,
		SCREEN_CHAT = 2,
		SCREEN_TEST = 3,
	};

	void SetBoundArea(WRect* rect);
	int GetCursor();
	void SetCursor(int cursor);
	void ForceMove(float x, float y, float factor);
	void Reset(bool all);
	void SetActive(bool active);
	void Store(void);
	void SetCamMode(bool camMode);
	void Display(void);
	void Process(float dt);

	bool IsActive(void);                    // mousecursor.h:44

	void Toggle(bool on);                   // mousecursor.h:47

	void SetTempActive(bool active);        // mousecursor.h:50

	bool IsVisible(void);                   // mousecursor.h:59

	void SetAutoHide(bool autoHide);        // mousecursor.h:63

	bool InArea(const WRect& rect);
	float RayTrace4Ground(void);

	int GetArea(void);                      // mousecursor.h:187

	bool IsCamMode(void);                   // mousecursor.h:195

	// mousecursor.h:203
	void SetItemWindow(bool itemWindow) { m_itemWindow = itemWindow; }

	void SetHoleCup(float x, float y);      // mousecursor.h:204

	void Enable(int mode, bool enable);     // mousecursor.h:206

	// mousecursor.h:198
	void SetMode(int mode) { m_mode = mode; }

	// mousecursor.h:210
	void SetTerrainToolTip(bool show) { m_terrainToolTip = show; }

private:
	int CheckArea(void);
	int ChatCheck(void);
	int GameCheck(void);
	int TestCheck(void);
	bool IsInclude(float x, float y, float w, float h, bool sizeOnly);
	bool SetArea(int area);
	static bool ChildWindowCallback(FrWnd* pWnd, void* pParam);

protected:
	WVector2D m_pointer;           // 0x28, current cursor position
	WVector2D m_storedPointer;     // 0x30, saved by Store / SetActive(false)
	PADDING(0x38, 0x40);
	WVector2D m_moveTarget;        // 0x40, ForceMove destination
	WVector2D m_moveDelta;         // 0x48, m_pointer - m_moveTarget
public:
	int m_mode;             // 0x50, eMode (written directly by CGuideBook::HandleMsg)
protected:
	sCursorImage* m_pImages;    // 0x54, new[] of 12
	float m_hoverTime;      // 0x58, idle time, cleared on every move
	float m_frameTime;      // 0x5C, time of one full cursor animation (0.35 s)
	int m_frame;            // 0x60, current frame of the cursor animation
	bool m_visible;         // 0x64, set on every move, cleared after 3 s idle; Display draws only while set
	bool m_active;          // 0x65
	PADDING(0x66, 0x68);
	int m_area;             // 0x68
	int m_cursor;           // 0x6C
	bool m_checkGame;       // 0x70
	bool m_checkChat;       // 0x71
	PADDING(0x72, 0x74);
	float m_animTime;       // 0x74, clock of the current cursor's frame animation (Process)
	WRect* m_pBoundArea;    // 0x78
	bool m_camMode;         // 0x7C
	PADDING(0x7D, 0x80);
	float m_moveRate;       // 0x80
	float m_moveSpeed;      // 0x84, 1 / ForceMove factor
	int m_resetState;       // 0x88, only cleared by Reset so far
	std::vector<int> m_points;  // 0x8C, emptied by Reset (element type not known yet)
	bool m_itemWindow;      // 0x9C
	PADDING(0x9D, 0xA0);
	int m_pressArea[3];     // 0xA0, m_area when each mouse button went down (Process)
	bool m_dragged;         // 0xAC, set by GameCheck once a drag passes 42 pixels
	bool m_tempActive;      // 0xAD, Process and Display run while m_active or this is set
	PADDING(0xAE, 0xB0);
	float m_holeCupX;       // 0xB0
	float m_holeCupY;       // 0xB4
	float m_groundMinX;     // 0xB8, ground bounds RayTrace4Ground accepts
	float m_groundMinZ;     // 0xBC
	float m_groundMaxX;     // 0xC0
	float m_groundMaxZ;     // 0xC4
	WVector m_rayDir;       // 0xC8, picking ray of the cursor (scaled to 300)
	float m_rayDist;        // 0xD4, ray factor down to the ground plane
	bool m_autoHide;        // 0xD8
public:
	// 0xD9: enable flag of each cursor area, indexed by area (Reset sets
	// all 0x49); the entries other units write by name overlay it.
	union {
		bool m_enable[0x49];    // 0xD9
		struct {
			PADDING(0xD9, 0xDE);
			// 0xDE: cleared by CTutorial::BlockInput, set again by CTutorial::FreeInput.
			bool m_inputEnable;     // 0xDE
			PADDING(0xDF, 0xFD);
			// 0xFD: enable flag of cursor area 0x24, the training option button
			// (CScreen clears it before opening FrTrainingOptionDlg and sets it again
			// in OnTrainingOptionDlgResult).
			bool m_trainingOptionEnable;    // 0xFD
			PADDING(0xFE, 0x100);
			// 0x100: six flags written one by one from outside (CAuxMenuUI::Process
			// sets or clears all six; CGuideBook clears [1] while its dialog is open
			// and sets it again on the dialog result).
			bool m_uiEnable[6];     // 0x100
		};
	};
protected:
	bool m_toggle;          // 0x122
	bool m_cursorOverride;  // 0x123, SetCursor past the last cursor
	CGroundPropt* m_pGroundPropt;   // 0x124
	bool m_terrainToolTip;  // 0x128
	PADDING(0x129, 0x12C);
	float m_groundTipTime;  // 0x12C, Process selects the ground tip after 0.2 s
	void* m_pTextTip;       // 0x130, nulled by the constructor (type not known yet)

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CMouseCursor, 0x134);
BEGIN_LAYOUT_CHECK(CMouseCursor)
	CHECK_FIELD(CMouseCursor, m_pointer, 0x28);
	CHECK_FIELD(CMouseCursor, m_storedPointer, 0x30);
	CHECK_FIELD(CMouseCursor, m_moveTarget, 0x40);
	CHECK_FIELD(CMouseCursor, m_moveDelta, 0x48);
	CHECK_FIELD(CMouseCursor, m_mode, 0x50);
	CHECK_FIELD(CMouseCursor, m_pImages, 0x54);
	CHECK_FIELD(CMouseCursor, m_hoverTime, 0x58);
	CHECK_FIELD(CMouseCursor, m_frameTime, 0x5C);
	CHECK_FIELD(CMouseCursor, m_frame, 0x60);
	CHECK_FIELD(CMouseCursor, m_visible, 0x64);
	CHECK_FIELD(CMouseCursor, m_active, 0x65);
	CHECK_FIELD(CMouseCursor, m_area, 0x68);
	CHECK_FIELD(CMouseCursor, m_cursor, 0x6C);
	CHECK_FIELD(CMouseCursor, m_checkGame, 0x70);
	CHECK_FIELD(CMouseCursor, m_checkChat, 0x71);
	CHECK_FIELD(CMouseCursor, m_animTime, 0x74);
	CHECK_FIELD(CMouseCursor, m_pBoundArea, 0x78);
	CHECK_FIELD(CMouseCursor, m_camMode, 0x7C);
	CHECK_FIELD(CMouseCursor, m_moveRate, 0x80);
	CHECK_FIELD(CMouseCursor, m_moveSpeed, 0x84);
	CHECK_FIELD(CMouseCursor, m_resetState, 0x88);
	CHECK_FIELD(CMouseCursor, m_points, 0x8C);
	CHECK_FIELD(CMouseCursor, m_itemWindow, 0x9C);
	CHECK_FIELD(CMouseCursor, m_pressArea, 0xA0);
	CHECK_FIELD(CMouseCursor, m_dragged, 0xAC);
	CHECK_FIELD(CMouseCursor, m_tempActive, 0xAD);
	CHECK_FIELD(CMouseCursor, m_holeCupX, 0xB0);
	CHECK_FIELD(CMouseCursor, m_holeCupY, 0xB4);
	CHECK_FIELD(CMouseCursor, m_groundMinX, 0xB8);
	CHECK_FIELD(CMouseCursor, m_groundMinZ, 0xBC);
	CHECK_FIELD(CMouseCursor, m_groundMaxX, 0xC0);
	CHECK_FIELD(CMouseCursor, m_groundMaxZ, 0xC4);
	CHECK_FIELD(CMouseCursor, m_rayDir, 0xC8);
	CHECK_FIELD(CMouseCursor, m_rayDist, 0xD4);
	CHECK_FIELD(CMouseCursor, m_autoHide, 0xD8);
	CHECK_FIELD(CMouseCursor, m_enable, 0xD9);
	CHECK_FIELD(CMouseCursor, m_inputEnable, 0xDE);
	CHECK_FIELD(CMouseCursor, m_trainingOptionEnable, 0xFD);
	CHECK_FIELD(CMouseCursor, m_uiEnable, 0x100);
	CHECK_FIELD(CMouseCursor, m_toggle, 0x122);
	CHECK_FIELD(CMouseCursor, m_cursorOverride, 0x123);
	CHECK_FIELD(CMouseCursor, m_pGroundPropt, 0x124);
	CHECK_FIELD(CMouseCursor, m_terrainToolTip, 0x128);
	CHECK_FIELD(CMouseCursor, m_groundTipTime, 0x12C);
	CHECK_FIELD(CMouseCursor, m_pTextTip, 0x130);
END_LAYOUT_CHECK

// GolfRule.cpp defines GOLFRULE_MOUSECURSOR_INLINE empty before its includes:
// the original holds the out-of-line copy of these inline accessors in
// GolfRule's object; every other unit inlines them.
#ifndef GOLFRULE_MOUSECURSOR_INLINE
#define GOLFRULE_MOUSECURSOR_INLINE inline
#endif

// HelpMsg.cpp defines HELPMSG_MOUSECURSOR_INLINE empty before its includes:
// the original holds the out-of-line copy of IsActive in HelpMsg's object.
#ifndef HELPMSG_MOUSECURSOR_INLINE
#define HELPMSG_MOUSECURSOR_INLINE inline
#endif

// mousecursor.h:44
HELPMSG_MOUSECURSOR_INLINE bool CMouseCursor::IsActive(void) {
	return m_active;
}

// mousecursor.h:47
GOLFRULE_MOUSECURSOR_INLINE void CMouseCursor::Toggle(bool on) {
	m_toggle = on;
}

// GolfTask.cpp defines GOLFTASK_MOUSECURSOR_INLINE empty before its
// includes: the original holds the out-of-line copy of SetTempActive in
// GolfTask's object.
#ifndef GOLFTASK_MOUSECURSOR_INLINE
#define GOLFTASK_MOUSECURSOR_INLINE inline
#endif

// mousecursor.h:50
GOLFTASK_MOUSECURSOR_INLINE void CMouseCursor::SetTempActive(bool active) {
	m_tempActive = active;
	if (m_tempActive) {
		m_hoverTime = 0;
		m_visible = true;
	}
}

// gui_control.cpp defines GUI_CONTROL_MOUSECURSOR_INLINE empty before its
// includes: the original holds the out-of-line copy of IsVisible and
// IsCamMode in gui_control's object.
#ifndef GUI_CONTROL_MOUSECURSOR_INLINE
#define GUI_CONTROL_MOUSECURSOR_INLINE inline
#endif

// mousecursor.h:59
GUI_CONTROL_MOUSECURSOR_INLINE bool CMouseCursor::IsVisible(void) {
	return m_visible;
}

// mousecursor.h:63
GOLFRULE_MOUSECURSOR_INLINE void CMouseCursor::SetAutoHide(bool autoHide) {
	m_autoHide = autoHide;
}

// mousecursor.h:187
GOLFRULE_MOUSECURSOR_INLINE int CMouseCursor::GetArea(void) {
	return m_area;
}

// mousecursor.h:195
GUI_CONTROL_MOUSECURSOR_INLINE bool CMouseCursor::IsCamMode(void) {
	return m_camMode;
}

// mousecursor.h:206
GOLFRULE_MOUSECURSOR_INLINE void CMouseCursor::Enable(int mode, bool enable) {
	m_enable[mode] = enable;
}

#endif
