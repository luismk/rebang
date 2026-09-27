#pragma once
#include "frwnd.h"

class FrScrollBar : public FrWnd
{
public:
	FrScrollBar();
	virtual ~FrScrollBar();
	void Init(FrWndManager* manager, FrWnd* parent, int items, int rows,
		int columns, bool atLeft);
	void ScrollUp(int rows);
	void ScrollDown(int rows);

	void Resize(int rows, int columns, bool reset);
	void AddItem();
	void DelItem();
	void ClearItem();
	void ScrollToFirst();
	void ScrollToBottom();
	float GetCurTopRow() { return m_curTopRow; }
	int GetCurTopRow_Int() { return (int)(m_curTopRow + 0.5f); }
	void SetCurTopRow(int row);
	void FollowBottom(bool follow) { m_followBottom = follow; }

protected:
	float m_barH;
	float m_barY;
	int m_itemNum;
	float m_curTopRow;
	int m_rowCapacity;
	int m_colCapacity;
	float m_dragPrevY;
	bool m_followBottom;
	bool m_showGuide;
	bool m_atLeft;
	const Bitmap* m_frames[3];
	bool m_mouseDown;
	struct Frag
	{
		int width;
		int height;
		const Bitmap* n_img;
		const Bitmap* o_img;
	};
	Frag m_frag[3];
};
