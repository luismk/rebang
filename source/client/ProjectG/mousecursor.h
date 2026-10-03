#pragma once
#include <vector>
#include "baseobject.h"
#include "singleton.h"

class CGroundPropt;
class CTextToolTip;

class CMouseCursor : public WSingleton<CMouseCursor>
{
public:
	enum
	{
		UNSELECT,
		ROLL_OVER,
		SELECT,
		WRONG,
		UP,
		DOWN,
		ZOOM
	};

	enum
	{
		AREA_NORMAL = 0,
		AREA_CLUB = 1,
		AREA_CLUB_CIRCLE = 2,
		AREA_SPIN = 3,
		AREA_CHAT_WINDOW = 4,
		AREA_BAR = 5,
		AREA_ITEM = 6,
		AREA_CHAT = 7,
		AREA_TOP = 8,
		AREA_BOTTOM = 9,
		AREA_USERINFO1 = 10,
		AREA_USERINFO2 = 11,
		AREA_USERINFO3 = 12,
		AREA_USERINFO4 = 13,
		AREA_COMBO1 = 14,
		AREA_COMBO2 = 15,
		AREA_COMBO3 = 16,
		AREA_COMBO4 = 17,
		AREA_ITEMSLOT = 18,
		AREA_WIND = 19,
		AREA_TIMER = 20,
		AREA_HOLEINFO = 21,
		AREA_CAMRESET = 22,
		AREA_CAMLEFT = 23,
		AREA_CAMRIGHT = 24,
		AREA_CAMUP = 25,
		AREA_CAMDOWN = 26,
		AREA_CAMFORWARD = 27,
		AREA_CAMBACKWARD = 28,
		AREA_EXIT_YES = 29,
		AREA_EXIT_NO = 30,
		AREA_TUTORIAL_NEXT = 31,
		AREA_TUTORIAL_REVIEW = 32,
		AREA_CHECKPOINT = 33,
		AREA_RANK = 34,
		AREA_CHAT_SCROLLBAR = 35,
		AREA_TRAINING_OPTION = 36,
		AREA_AUXMENU_SHOW = 37,
		AREA_AUXMENU_OPEN = 38,
		AREA_AUXMENU_OPTION = 39,
		AREA_AUXMENU_TIP = 40,
		AREA_AUXMENU_UNPLAYABLE = 41,
		AREA_AUXMENU_PAUSE = 42,
		AREA_AUXMENU_REPORT = 43,
		AREA_AUXMENU_USERLIST = 44,
		AREA_EMOTUI_WEATHER = 45,
		AREA_EMOTUI_ITEM = 46,
		AREA_EMOTUI_ACTION = 47,
		AREA_EMOTUI_EXPRESSION0 = 48,
		AREA_EMOTUI_EXPRESSION1 = 49,
		AREA_EMOTUI_CLOSE = 50,
		AREA_EMOTUI_EMOTICON_00 = 51,
		AREA_EMOTUI_EMOTICON_01 = 52,
		AREA_EMOTUI_EMOTICON_02 = 53,
		AREA_EMOTUI_EMOTICON_03 = 54,
		AREA_EMOTUI_EMOTICON_04 = 55,
		AREA_EMOTUI_EMOTICON_05 = 56,
		AREA_EMOTUI_EMOTICON_06 = 57,
		AREA_EMOTUI_EMOTICON_07 = 58,
		AREA_EMOTUI_EMOTICON_08 = 59,
		AREA_EMOTUI_EMOTICON_09 = 60,
		AREA_EMOTUI_EMOTICON_10 = 61,
		AREA_EMOTUI_EMOTICON_11 = 62,
		AREA_RAY_CHECK = 63,
		AREA_RESERVE_GAME = 64,
		AREA_MESSANGER = 65,
		AREA_TRY_ON = 66,
		AREA_OPTION = 67,
		AREA_CANCEL_RESERVE = 68,
		AREA_GAME_START = 69,
		AREA_LEFT = 70,
		AREA_RIGHT = 71,
		AREA_EXIT_TO_LOBBY = 72,
		AREA_MAX = 73
	};

	bool IsActive() { return m_active; }
	void Toggle(bool show) { m_bShow = show; }
	bool IsVisible() { return m_show; }
	void SetAutoHide(bool autoHide) { m_autoHide = autoHide; }

	void Reset(bool active);
	void SetActive(bool active);
	void SetCursor(int cursor);
	int GetCursor();
	bool InArea(const WRect& rect);
	void ForceMove(float x, float y, float z);

	int GetArea() { return m_area; }
	bool IsInButtonDownArea(eButton button)
	{
		return m_buttonDownArea[button] == m_area;
	}
	bool IsCamMode() { return m_camMode; }
	void SetMode(int mode) { m_mode = mode; }
	void SetItemWindow(bool itemWindow) { m_itemWindow = itemWindow; }
	void SetTerrainToolTip(bool tooltip) { m_bTerrainTooltip = tooltip; }

protected:
	WVector2D m_pointer;
	WVector2D m_oldPointer;
	WVector2D m_mouse;
	WVector2D m_target;
	WVector2D m_delta;
	int m_mode;

public:
	struct sPointer
	{
		WOverlay* overlay;
		float w;
		float h;
		float len;
		int num;
	};

protected:
	sPointer* m_overlay;
	float m_stopDuration;
	float m_frequency;
	int m_frame;
	bool m_show;
	bool m_active;
	int m_area;
	int m_cursor;
	bool m_validButton[3];
	float m_dt;
	WRect* m_bound;
	bool m_camMode;
	float m_forceMoving;
	float m_forceFactor;
	int m_layer;
	std::vector<int> m_activeArea;
	bool m_itemWindow;
	int m_buttonDownArea[3];
	bool m_ignoreButtonDownArea;
	bool m_tempActive;
	WVector2D m_holecup;
	WRect m_groundArea;
	WVector m_ray;
	float m_rayRatio;
	bool m_autoHide;
	bool m_bArea[73];
	bool m_bShow;
	bool m_bRightButton;
	CGroundPropt* m_pGroundPt;
	bool m_bTerrainTooltip;
	float m_td4Tooltip;
	CTextToolTip* m_pTextTip;
};
