// Original: d:\build\custom\temp\code\client\projectg\itemwindow.h
#ifndef PG_CLIENT_PROJECTG_ITEMWINDOW_H
#define PG_CLIENT_PROJECTG_ITEMWINDOW_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "../client/Wangreal/include/wtypes.h"

class WOverlay;
class CSimpleUI;

// The in-game item bar that slides in from the left of the screen.
// Size 0x94 (the `new` of CItemWindowMakeInstance).
class CItemWindow : public IActor {
	LAYOUT_CHECK_FRIEND;
public:
	// Slide state of the window (values from ResetVars/OnLeft/OnRight).
	enum eState {
		STATE_OPENED = 0,
		STATE_CLOSING = 1,
		STATE_OPENING = 2,
		STATE_CLOSED = 3
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	// itemwindow.h:26
	virtual ~CItemWindow(void) {}

	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnProcess(float delta);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);

	void ResetUiPosition(void);
	static bool IsClosed(void);

protected:
	CItemWindow(void);

	void ResetVars(void);
	void OnLeft(float delta);
	void OnRight(float delta);
	void OnClosed(float delta);
	void OnOpened(float delta);
	void CheckInput(void);
	void Draw(void);
	void ExecuteNow(bool now);
	unsigned long* GetTidItemSlot(unsigned char index);
	bool ProcessItem(unsigned char slot, unsigned long typeId, bool now, unsigned long param);
	bool IsUsable(unsigned long typeId);
	bool UseItem(int slot);
	void NetUseItem(unsigned long typeId, unsigned long param1, unsigned long param2);

	static eState ms_state;

	friend IObject* CItemWindowMakeInstance(void);

	std::map<unsigned long, WOverlay*> m_overlays; // 0x18
	WOverlay* m_pOverlay;                          // 0x24, the bar's background
	WRect m_rect;                                  // 0x28, drawn area
	WRect m_clipRect;                              // 0x38, clipping area of m_pOverlay
	float m_pos;                                   // 0x48, slide offset
	float m_targetPos;                             // 0x4c
	float m_speed;                                 // 0x50
	float m_mouseX;                                // 0x54, cursor position read each frame
	float m_mouseY;                                // 0x58
	char m_burstFrame;                             // 0x5C, frame of the burst animation (0..7)
	PADDING(0x5D, 0x60);
	CSimpleUI* m_pSimpleUI;                        // 0x60
	int m_selected;                                // 0x64
	int m_used;                                    // 0x68
	int m_over;                                    // 0x6c
	int m_unknown70;                               // 0x70
	float m_unknown74;                             // 0x74
	float m_unknown78;                             // 0x78
	float m_flyTime;                               // 0x7C, time since the used item's icon started flying
	float m_flyX;                                  // 0x80, the flying icon's screen position
	float m_flyY;                                  // 0x84
	float m_flyVelX;                               // 0x88, the flying icon's velocity
	float m_flyVelY;                               // 0x8C
	bool m_unknown90;                              // 0x90
	bool m_bursting;                               // 0x91, the icon reached the head and bursts
	bool m_unknown92;                              // 0x92
	bool m_unknown93;                              // 0x93
};

CHECK_SIZE(CItemWindow, 0x94);
BEGIN_LAYOUT_CHECK(CItemWindow)
	CHECK_FIELD(CItemWindow, m_overlays, 0x18);
	CHECK_FIELD(CItemWindow, m_pOverlay, 0x24);
	CHECK_FIELD(CItemWindow, m_rect, 0x28);
	CHECK_FIELD(CItemWindow, m_clipRect, 0x38);
	CHECK_FIELD(CItemWindow, m_pos, 0x48);
	CHECK_FIELD(CItemWindow, m_targetPos, 0x4c);
	CHECK_FIELD(CItemWindow, m_speed, 0x50);
	CHECK_FIELD(CItemWindow, m_pSimpleUI, 0x60);
	CHECK_FIELD(CItemWindow, m_selected, 0x64);
	CHECK_FIELD(CItemWindow, m_used, 0x68);
	CHECK_FIELD(CItemWindow, m_over, 0x6c);
	CHECK_FIELD(CItemWindow, m_unknown70, 0x70);
	CHECK_FIELD(CItemWindow, m_unknown74, 0x74);
	CHECK_FIELD(CItemWindow, m_unknown78, 0x78);
	CHECK_FIELD(CItemWindow, m_mouseX, 0x54);
	CHECK_FIELD(CItemWindow, m_mouseY, 0x58);
	CHECK_FIELD(CItemWindow, m_burstFrame, 0x5c);
	CHECK_FIELD(CItemWindow, m_flyTime, 0x7c);
	CHECK_FIELD(CItemWindow, m_flyX, 0x80);
	CHECK_FIELD(CItemWindow, m_flyY, 0x84);
	CHECK_FIELD(CItemWindow, m_flyVelX, 0x88);
	CHECK_FIELD(CItemWindow, m_flyVelY, 0x8c);
	CHECK_FIELD(CItemWindow, m_unknown90, 0x90);
	CHECK_FIELD(CItemWindow, m_bursting, 0x91);
	CHECK_FIELD(CItemWindow, m_unknown92, 0x92);
	CHECK_FIELD(CItemWindow, m_unknown93, 0x93);
END_LAYOUT_CHECK

IObject* CItemWindowMakeInstance(void);

#endif // PG_CLIENT_PROJECTG_ITEMWINDOW_H
