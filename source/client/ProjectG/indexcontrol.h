// Original: d:\build\custom\temp\code\client\projectg\indexcontrol.h
#ifndef PG_CLIENT_PROJECTG_INDEXCONTROL_H
#define PG_CLIENT_PROJECTG_INDEXCONTROL_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "../client/Wangreal/include/wtypes.h"

// Paged index (page number) control (indexcontrol.cpp). Size 0x5c: the
// `??_M` element size in CIndexControl's vector deleting destructor
// (itemstorage). The vtable holds only the deleting destructor.
class CIndexControl {
	LAYOUT_CHECK_FRIEND;
public:
	CIndexControl(void);            // indexcontrol.cpp:27
	virtual ~CIndexControl(void);   // indexcontrol.cpp:35

	bool Init(unsigned long totalCount, unsigned long countPerPage);   // indexcontrol.cpp:68
	bool BuildPage(unsigned long page);
	void Draw(WPoint& pos);   // indexcontrol.cpp:164
	bool Click(void);
	bool NextPage(void);
	bool PrevPage(void);
	bool SelectIndex(unsigned long index);
	void SetSelectedNumber(unsigned long number);
	unsigned long GetSelectedNumber(void) const;   // indexcontrol.cpp:383
	unsigned long GetSelectedIndex(void) const;    // indexcontrol.cpp:390
	unsigned long GetCurIndexCount(void) const;    // indexcontrol.cpp:397
	unsigned long GetTotalPage(void) const;        // indexcontrol.cpp:404

protected:
	void ClearVariables(void);   // indexcontrol.cpp:47

	WRect m_indexRect;                // 0x04 one index's cell (0, 0, 30, 10)
	bool m_enabled;                   // 0x14 set once Init built the first page
	unsigned char m_align;            // 0x15 1 centre, 2 right (Draw)
	PADDING(0x16, 0x18);
	unsigned long m_color;            // 0x18 0xffff0000
	unsigned long m_selectedColor;    // 0x1c 0xffaa1111
	unsigned long m_disabledColor;    // 0x20 0xffaaaaaa
	unsigned long m_totalCount;       // 0x24
	unsigned long m_totalPage;        // 0x28
	unsigned long m_curIndexCount;    // 0x2c indices on the current page
	unsigned long m_countPerPage;     // 0x30
	unsigned long m_curPage;          // 0x34
	unsigned long m_selectedIndex;    // 0x38 within the current page
	unsigned long m_hoverIndex;       // 0x3c index under the mouse (Draw), else the selected one
	WRect m_drawRect;                 // 0x40
	WRect* m_indexRects;              // 0x50 new[]'d, one per index of the page
	WPoint m_pos;                     // 0x54 last Draw position
};
CHECK_SIZE(CIndexControl, 0x5c);
BEGIN_LAYOUT_CHECK(CIndexControl)
	CHECK_FIELD(CIndexControl, m_indexRect, 0x04);
	CHECK_FIELD(CIndexControl, m_enabled, 0x14);
	CHECK_FIELD(CIndexControl, m_align, 0x15);
	CHECK_FIELD(CIndexControl, m_color, 0x18);
	CHECK_FIELD(CIndexControl, m_selectedColor, 0x1c);
	CHECK_FIELD(CIndexControl, m_disabledColor, 0x20);
	CHECK_FIELD(CIndexControl, m_totalCount, 0x24);
	CHECK_FIELD(CIndexControl, m_totalPage, 0x28);
	CHECK_FIELD(CIndexControl, m_curIndexCount, 0x2c);
	CHECK_FIELD(CIndexControl, m_countPerPage, 0x30);
	CHECK_FIELD(CIndexControl, m_curPage, 0x34);
	CHECK_FIELD(CIndexControl, m_selectedIndex, 0x38);
	CHECK_FIELD(CIndexControl, m_hoverIndex, 0x3c);
	CHECK_FIELD(CIndexControl, m_drawRect, 0x40);
	CHECK_FIELD(CIndexControl, m_indexRects, 0x50);
	CHECK_FIELD(CIndexControl, m_pos, 0x54);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_INDEXCONTROL_H
