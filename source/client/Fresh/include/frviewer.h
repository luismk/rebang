// Original: d:\build\custom\temp\code\client\fresh\include\frviewer.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRVIEWER_H
#define PG_CLIENT_FRESH_INCLUDE_FRVIEWER_H

#include <string>

#include "..\..\defines.h"
#include "frscrollbar.h"
#include "frwnd.h"
#include "../client/ProjectG/background.h"

class FrGuiItem;

// A window showing an image file. Layout, access and methods from the PDB
// (size 0x100, base FrWnd, no new virtual). The bodies written here are the
// inline ones whose COMDATs carry frviewer.h line marks.
class FrViewer : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frviewer.h:18

	FrViewer(void);
	virtual ~FrViewer(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	bool Open(const char* fileName);
	int GetSrcWidth(void) { return m_pView ? m_pView->GetWidth() : 0; }      // frviewer.h:27
	int GetSrcHeight(void) { return m_pView ? m_pView->GetHeight() : 0; }    // frviewer.h:28
	float GetViewWidth(void) { return m_rect.w; }                            // frviewer.h:29
	float GetViewHeight(void) { return m_rect.h; }                           // frviewer.h:30
	float GetScrollBarOffset(void) { return m_pScrBar ? m_pScrBar->GetRect().w : 0.0f; }   // frviewer.h:31
	void ShowScrollBar(bool show) {                                          // frviewer.h:32
		if (m_pScrBar)
			m_pScrBar->SetVisible(show);
	}
	void SetMouseEvent(bool mouseEvent);

protected:
	virtual void OnDraw(void);
	virtual void OnResize(void);
	virtual void OnSetCursor(bool bInClient, const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& pt);
	virtual bool OnLButtonDown(const WPoint& pt);
	virtual bool OnRButtonUp(const WPoint& pt);
	virtual bool OnRButtonDown(const WPoint& pt);
	virtual void OnDblClick(const WPoint& pt);

	FrGuiItem* m_pItem;        // 0xd4
	std::string m_fileName;    // 0xd8
	unsigned long m_bgColor;   // 0xf4
	CBackGround* m_pView;      // 0xf8
	bool m_mouseEvent;         // 0xfc
};

CHECK_SIZE(FrViewer, 0x100);
BEGIN_LAYOUT_CHECK(FrViewer)
	CHECK_FIELD(FrViewer, m_pItem, 0xd4);
	CHECK_FIELD(FrViewer, m_fileName, 0xd8);
	CHECK_FIELD(FrViewer, m_bgColor, 0xf4);
	CHECK_FIELD(FrViewer, m_pView, 0xf8);
	CHECK_FIELD(FrViewer, m_mouseEvent, 0xfc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRVIEWER_H
