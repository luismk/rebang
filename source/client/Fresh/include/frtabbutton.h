// Original: d:\build\custom\temp\code\client\fresh\include\frtabbutton.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRTABBUTTON_H
#define PG_CLIENT_FRESH_INCLUDE_FRTABBUTTON_H

#include <list>

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrGuiItem;

// One tab of a FrTabButton (PDB layout, size 0x24).
struct TButtonItem {
	const Bitmap* bitmap[3];             // 0x0
	unsigned long index;                 // 0xc
	WRect rect;                          // 0x10
	bool underCursor;                    // 0x20
	bool selected;                       // 0x21
};

CHECK_SIZE(TButtonItem, 0x24);
CHECK_FIELD(TButtonItem, bitmap, 0x0);
CHECK_FIELD(TButtonItem, index, 0xc);
CHECK_FIELD(TButtonItem, rect, 0x10);
CHECK_FIELD(TButtonItem, underCursor, 0x20);
CHECK_FIELD(TButtonItem, selected, 0x21);

// A row of tabs. Layout, access and methods from the PDB (size 0x104, base
// FrWnd, no new virtual).
class FrTabButton : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	enum eBtnType {
		BT_NONE = 0,
		BT_DOWN = 1,
		BT_UP = 2
	};

	typedef std::list<TButtonItem*> TBUTTON_LIST;

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frtabbutton.h:18

	FrTabButton(void);
	virtual ~FrTabButton(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void ClearTab(void);
	void AddTab(const char** const names, unsigned long index, WSize* pSize);
	void AddTab(const char* name, unsigned long index, WSize* pSize);
	void Select(unsigned long index);
	void UnSelect(unsigned long index);
	void SetSepWidth(float width) { m_sepWidth = width; }   // frtabbutton.h:30
	TButtonItem* GetItemUnderCursor(void);
	TButtonItem* GetSelected(void) { return m_selected; }   // frtabbutton.h:33

protected:
	void Select(TButtonItem* pItem);
	void UnSelect(TButtonItem* pItem);
	void InitButtonItem(TButtonItem* pItem, unsigned long index, WSize* pSize);
	virtual void PreCreateWindow(FrWndManager* pManager, unsigned long dwStyle, const WRect& rect, FrWnd* pParentWnd);
	virtual void OnDraw(void);
	virtual void OnProc(const float deltaTime);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual void OnSetCursor(bool bInClient, const WPoint& mousePos);

	float m_sepWidth;                    // 0xd4
	TBUTTON_LIST m_tBtnList;             // 0xd8
	TButtonItem* m_underCursor;          // 0xe4
	TButtonItem* m_selected;             // 0xe8
	TButtonItem* m_pressed;              // 0xec
	const Bitmap* m_sepImg;              // 0xf0
	const Bitmap* m_pBtnBgImg[3];        // 0xf4
	eBtnType m_btnType;                  // 0x100
};

CHECK_SIZE(FrTabButton, 0x104);
BEGIN_LAYOUT_CHECK(FrTabButton)
	CHECK_FIELD(FrTabButton, m_sepWidth, 0xd4);
	CHECK_FIELD(FrTabButton, m_tBtnList, 0xd8);
	CHECK_FIELD(FrTabButton, m_underCursor, 0xe4);
	CHECK_FIELD(FrTabButton, m_selected, 0xe8);
	CHECK_FIELD(FrTabButton, m_pressed, 0xec);
	CHECK_FIELD(FrTabButton, m_sepImg, 0xf0);
	CHECK_FIELD(FrTabButton, m_pBtnBgImg, 0xf4);
	CHECK_FIELD(FrTabButton, m_btnType, 0x100);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRTABBUTTON_H
