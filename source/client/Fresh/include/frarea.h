// Original: d:\build\custom\temp\code\client\fresh\include\frarea.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRAREA_H
#define PG_CLIENT_FRESH_INCLUDE_FRAREA_H

#include <string>

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrGuiItem;

// A window drawing a background image. Layout and access from the PDB (size
// 0x114, base FrWnd); ClippingArea is the one new virtual (slot 23).
class FrArea : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frarea.h:16

	FrArea(void);
	virtual ~FrArea(void);

	void Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent);
	bool SetBgImg(const Bitmap* pBitmap);
	bool SetBgImg(const char* name);
	const Bitmap* GetBgImg(void) { return m_pBitmap; }   // frarea.h:24
	void SetMouseEvent(bool mouseEvent);

protected:
	virtual void OnDraw(void);
	virtual void OnSetCursor(bool b, const WPoint& pt);
	virtual bool OnLButtonUp(const WPoint& pt);
	virtual bool OnLButtonDown(const WPoint& pt);
	virtual bool ClippingArea(WRect& src, WRect& dst);

	FrGuiItem* m_pItem;          // 0xd4
	const Bitmap* m_pBitmap;     // 0xd8
	bool m_center;               // 0xdc
	bool m_stretch;              // 0xdd
	bool m_mouseEvent;           // 0xde
	unsigned long m_color;       // 0xe0
	std::string m_imgName;       // 0xe4
	bool m_isClipping;           // 0x100
	WRect m_rcClipArea;          // 0x104
};

CHECK_SIZE(FrArea, 0x114);
BEGIN_LAYOUT_CHECK(FrArea)
	CHECK_FIELD(FrArea, m_pItem, 0xd4);
	CHECK_FIELD(FrArea, m_pBitmap, 0xd8);
	CHECK_FIELD(FrArea, m_center, 0xdc);
	CHECK_FIELD(FrArea, m_stretch, 0xdd);
	CHECK_FIELD(FrArea, m_mouseEvent, 0xde);
	CHECK_FIELD(FrArea, m_color, 0xe0);
	CHECK_FIELD(FrArea, m_imgName, 0xe4);
	CHECK_FIELD(FrArea, m_isClipping, 0x100);
	CHECK_FIELD(FrArea, m_rcClipArea, 0x104);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRAREA_H
