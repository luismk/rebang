// Original: d:\build\custom\temp\code\client\fresh\include\frscrollbar.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRSCROLLBAR_H
#define PG_CLIENT_FRESH_INCLUDE_FRSCROLLBAR_H

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;

// The vertical scroll bar a window owns (FrWnd::GetScrollBar). Layout,
// access and methods from the PDB (size 0x134, base FrWnd, no new virtual).
// The bodies written here are the inline ones whose COMDATs carry
// frscrollbar.h line marks (lines 13-57).
class FrScrollBar : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	struct Frag {
		int width;              // 0x0
		int height;             // 0x4
		const Bitmap* n_img;    // 0x8
		const Bitmap* o_img;    // 0xc
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frscrollbar.h:13

	FrScrollBar(void);
	virtual ~FrScrollBar(void);

	void Init(FrWndManager* pManager, FrWnd* parentWnd, int row, int rowCapacity, int colCapacity, bool atLeft);
	void Resize(int rowCapacity, int colCapacity, bool atLeft);
	bool IsBarVisible(void) {                                      // frscrollbar.h:26
		return m_rowCapacity != 0 && m_rowCapacity < GetCurRowCount();
	}
	int GetRowCapacity(void) { return m_rowCapacity; }             // frscrollbar.h:27
	int GetColCapacity(void) { return m_colCapacity; }             // frscrollbar.h:28
	int GetItemNum(void) { return m_itemNum; }                     // frscrollbar.h:29
	float GetCurTopRow(void) { return m_curTopRow; }               // frscrollbar.h:30
	int GetCurTopRow_Int(void) { return (int)(m_curTopRow + 0.5f); }   // frscrollbar.h:31
	void SetCurTopRow(int row);
	void AddItem(void);
	void DelItem(void);
	void ClearItem(void);
	void ScrollToFirst(void);
	void ScrollToBottom(void);
	void ScrollUp(int delta);
	void ScrollDown(int delta);
	void FollowBottom(bool follow) { m_followBottom = follow; }    // frscrollbar.h:42
	void SetGuideVisible(bool show) { m_showGuide = show; }        // frscrollbar.h:43

protected:
	void SetBarHnY(void);
	virtual void OnDraw(void);
	virtual void OnResize(void);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual void OnWheel(FrInputState& istate);
	int GetCurRowCount(void) {                                     // frscrollbar.h:57
		return m_colCapacity ? (m_itemNum + m_colCapacity - 1) / m_colCapacity : 0;
	}

	float m_barH;                   // 0xd4
	float m_barY;                   // 0xd8
	int m_itemNum;                  // 0xdc
	float m_curTopRow;              // 0xe0
	int m_rowCapacity;              // 0xe4
	int m_colCapacity;              // 0xe8
	float m_dragPrevY;              // 0xec
	bool m_followBottom;            // 0xf0
	bool m_showGuide;               // 0xf1
	bool m_atLeft;                  // 0xf2
	const Bitmap* m_frames[3];      // 0xf4
	bool m_mouseDown;               // 0x100
	Frag m_frag[3];                 // 0x104
};

CHECK_SIZE(FrScrollBar::Frag, 0x10);
CHECK_SIZE(FrScrollBar, 0x134);
BEGIN_LAYOUT_CHECK(FrScrollBar)
	CHECK_FIELD(FrScrollBar, m_barH, 0xd4);
	CHECK_FIELD(FrScrollBar, m_barY, 0xd8);
	CHECK_FIELD(FrScrollBar, m_itemNum, 0xdc);
	CHECK_FIELD(FrScrollBar, m_curTopRow, 0xe0);
	CHECK_FIELD(FrScrollBar, m_rowCapacity, 0xe4);
	CHECK_FIELD(FrScrollBar, m_colCapacity, 0xe8);
	CHECK_FIELD(FrScrollBar, m_dragPrevY, 0xec);
	CHECK_FIELD(FrScrollBar, m_followBottom, 0xf0);
	CHECK_FIELD(FrScrollBar, m_showGuide, 0xf1);
	CHECK_FIELD(FrScrollBar, m_atLeft, 0xf2);
	CHECK_FIELD(FrScrollBar, m_frames, 0xf4);
	CHECK_FIELD(FrScrollBar, m_mouseDown, 0x100);
	CHECK_FIELD(FrScrollBar, m_frag, 0x104);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRSCROLLBAR_H
