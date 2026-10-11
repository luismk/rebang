// Original: d:\build\custom\temp\code\client\fresh\include\frcontextmenuctrl.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRCONTEXTMENUCTRL_H
#define PG_CLIENT_FRESH_INCLUDE_FRCONTEXTMENUCTRL_H

#include <list>
#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrElementFrame;
class FrGuiItem;

// A popup menu of text items. Layout, access and methods from the PDB (size
// 0x188, base FrWnd). The class has no run-time type of its own (no m_RTTI,
// no GetRTTI): FrWndManager creates it through DoCreateItem.
class FrContextMenuCtrl : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	enum eMenuType {
		SEPARATOR = 0,
		MENU = 1
	};

	struct sMenuItem {
		int nType;              // 0x0
		char szText[32];        // 0x4
		bool underCursor;       // 0x24
		bool selected;          // 0x25
		bool Enable;            // 0x26
		float top;              // 0x28
		float bottom;           // 0x2c
		const Bitmap* bmp;      // 0x30
	};

	typedef std::list<sMenuItem> MENU_LIST;

	FrContextMenuCtrl(void);
	virtual ~FrContextMenuCtrl(void);

	virtual void SetVisible(bool visible);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void UnableMenuItem(const char* szMenu);
	void ToggleMenuItem(const char* szSrc, const char* szDest);
	const char* GetSelectMenuText(void);
	void SetSelectMenuText(const char* szBuff);
	void SetMenuList(const char* caption);

protected:
	virtual void OnDraw(void);
	virtual void OnProc(const float deltaTime);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);

	void Select(MENU_LIST::iterator i);
	void Unselect(MENU_LIST::iterator i);
	void UpdateRectInfo(int layer, const Bitmap** bmp);
	void DrawFrame(int layer, float x, float y, const Bitmap** bmp);
	void DrawIcon(WPoint& pt, const Bitmap* bmp, float fHeight, bool bEnable);

	MENU_LIST m_MenuList;                   // 0xd4
	MENU_LIST::iterator m_underCursor;      // 0xe0
	MENU_LIST::iterator m_selected;         // 0xe4
	unsigned long m_color;                  // 0xe8
	unsigned long m_outlineColor;           // 0xec
	unsigned long m_style;                  // 0xf0
	unsigned long m_selColor;               // 0xf4
	unsigned long m_selOutlineColor;        // 0xf8
	unsigned long m_unableColor;            // 0xfc
	unsigned long m_selStyle;               // 0x100
	bool m_center;                          // 0x104
	WRect* m_clip;                          // 0x108
	float m_hlAlpha;                        // 0x10c
	char m_hlAlphaD;                        // 0x110
	bool m_bIsIcon;                         // 0x111
	FrElementFrame* m_pFrame;               // 0x114
	WSize m_a;                              // 0x118
	WSize m_b;                              // 0x120
	WSize m_c;                              // 0x128
	WSize m_min;                            // 0x130
	float m_width;                          // 0x138
	float m_height;                         // 0x13c
	const Bitmap* m_pBaseBmp[9];            // 0x140
	const Bitmap* m_pSubBmp[9];             // 0x164
};

CHECK_SIZE(FrContextMenuCtrl::sMenuItem, 0x34);
CHECK_SIZE(FrContextMenuCtrl, 0x188);
BEGIN_LAYOUT_CHECK(FrContextMenuCtrl)
	CHECK_FIELD(FrContextMenuCtrl, m_MenuList, 0xd4);
	CHECK_FIELD(FrContextMenuCtrl, m_underCursor, 0xe0);
	CHECK_FIELD(FrContextMenuCtrl, m_selected, 0xe4);
	CHECK_FIELD(FrContextMenuCtrl, m_color, 0xe8);
	CHECK_FIELD(FrContextMenuCtrl, m_outlineColor, 0xec);
	CHECK_FIELD(FrContextMenuCtrl, m_style, 0xf0);
	CHECK_FIELD(FrContextMenuCtrl, m_selColor, 0xf4);
	CHECK_FIELD(FrContextMenuCtrl, m_selOutlineColor, 0xf8);
	CHECK_FIELD(FrContextMenuCtrl, m_unableColor, 0xfc);
	CHECK_FIELD(FrContextMenuCtrl, m_selStyle, 0x100);
	CHECK_FIELD(FrContextMenuCtrl, m_center, 0x104);
	CHECK_FIELD(FrContextMenuCtrl, m_clip, 0x108);
	CHECK_FIELD(FrContextMenuCtrl, m_hlAlpha, 0x10c);
	CHECK_FIELD(FrContextMenuCtrl, m_hlAlphaD, 0x110);
	CHECK_FIELD(FrContextMenuCtrl, m_bIsIcon, 0x111);
	CHECK_FIELD(FrContextMenuCtrl, m_pFrame, 0x114);
	CHECK_FIELD(FrContextMenuCtrl, m_a, 0x118);
	CHECK_FIELD(FrContextMenuCtrl, m_b, 0x120);
	CHECK_FIELD(FrContextMenuCtrl, m_c, 0x128);
	CHECK_FIELD(FrContextMenuCtrl, m_min, 0x130);
	CHECK_FIELD(FrContextMenuCtrl, m_width, 0x138);
	CHECK_FIELD(FrContextMenuCtrl, m_height, 0x13c);
	CHECK_FIELD(FrContextMenuCtrl, m_pBaseBmp, 0x140);
	CHECK_FIELD(FrContextMenuCtrl, m_pSubBmp, 0x164);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRCONTEXTMENUCTRL_H
