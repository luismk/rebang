// Original: d:\build\custom\temp\code\client\fresh\include\frlistbox.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRLISTBOX_H
#define PG_CLIENT_FRESH_INCLUDE_FRLISTBOX_H

#include <list>
#include <map>

#include "..\..\defines.h"
#include "frwnd.h"

class Bitmap;
class FrGuiItem;

// One row of a list box (PDB layout, size 0x24). Constructor, destructor and
// AddNoRBtnIdx are the inline ones of frlistbox.h:26-29.
struct FrListItem {
	FrListItem(void) : pData(0), underCursor(false), selected(false) {   // frlistbox.h:26
		noRBtnIdxList.clear();
	}
	~FrListItem(void) { noRBtnIdxList.clear(); }                          // frlistbox.h:27

	void AddNoRBtnIdx(int idx) { noRBtnIdxList.push_back(idx); }         // frlistbox.h:29

	void* pData;                      // 0x0
	WPoint pos;                       // 0x4
	bool underCursor;                 // 0xc
	bool selected;                    // 0xd
	int no;                           // 0x10
	int idx;                          // 0x14
	std::list<int> noRBtnIdxList;     // 0x18
};

CHECK_SIZE(FrListItem, 0x24);
CHECK_FIELD(FrListItem, pData, 0x0);
CHECK_FIELD(FrListItem, pos, 0x4);
CHECK_FIELD(FrListItem, underCursor, 0xc);
CHECK_FIELD(FrListItem, selected, 0xd);
CHECK_FIELD(FrListItem, no, 0x10);
CHECK_FIELD(FrListItem, idx, 0x14);
CHECK_FIELD(FrListItem, noRBtnIdxList, 0x18);

// Sort predicate of FrListBox::SortItem: compares the items' data (PDB
// layout, size 0x4; frlistbox.h:38-41).
class FrListBoxGreater {
	LAYOUT_CHECK_FRIEND;
public:
	FrListBoxGreater(bool (*compare)(const void*, const void*)) : m_compare(compare) {}   // frlistbox.h:38
	bool operator()(FrListItem* const& a, FrListItem* const& b) const {                  // frlistbox.h:41
		return m_compare(a->pData, b->pData);
	}

protected:
	bool (*m_compare)(const void*, const void*);   // 0x0
};

CHECK_SIZE(FrListBoxGreater, 0x4);

// A list of selectable items laid out in rows and columns. Layout, access
// and vtable from the PDB (size 0x124, base FrWnd; SetClientRect is the new
// virtual of slot 23). The bodies written here are the inline ones whose
// COMDATs carry frlistbox.h line marks (lines 49-111).
class FrListBox : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	typedef std::list<FrListItem*> ITEM_LIST;

	struct sSize {
		int w;   // 0x0
		int h;   // 0x4
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frlistbox.h:49

	FrListBox(void);
	virtual ~FrListBox(void);

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void RecalcColCapacity(bool update);
	void SetMultiSelect(bool multiSelect) { m_multiSelect = multiSelect; }          // frlistbox.h:61
	void UseRightButton(bool useRightButton) { m_useRightButton = useRightButton; }   // frlistbox.h:62
	const Bitmap* GetBitmap(const char* name);
	FrListItem* GetItemUnderCursor(void) { return m_pItemUnderCursor; }            // frlistbox.h:64
	FrListItem* GetSelected(void) { return m_pSelected; }                          // frlistbox.h:65
	FrListItem* GetPreviousSelected(void) { return m_pPreviousSelected; }          // frlistbox.h:67
	void SelectItem(FrListItem* pItem, bool setSelected);
	void UnselectItem(FrListItem* pItem);
	void UnselectAllItem(void);
	void ToggleItem(FrListItem* pItem);
	int GetCurrentItemSize(bool bDummy);
	int GetItemWidth(void) { return m_itemSize.w; }                                // frlistbox.h:73
	int GetItemHeight(void) { return m_itemSize.h; }                               // frlistbox.h:74
	void SetItemSize(int w, int h) {                                               // frlistbox.h:75
		m_itemSize.w = w;
		m_itemSize.h = h;
	}
	FrListItem* AddItem(void* pData);
	FrListItem* FindItem(void* pData, int cmpLen);
	FrListItem* FindItem(void* pData);
	FrListItem* FindItem(FrListItem* pSelected);
	FrListItem* GetItem(int iLineNum);
	void DelItem(void* pData, int cmpLen);
	void DelItem(void* pData);
	void DelItem(FrListItem* pSelected);
	ITEM_LIST::iterator _DelItem(ITEM_LIST::iterator i);
	void ClearItem(void);
	void SortItem(bool (*compare)(const void*, const void*));
	void RecalcTopBottom(void);
	void EnableScrollBar(bool enable);
	// Never emitted as a COMDAT; the body is inferred from its member.
	void EnableRollOver(bool rollOver) { m_rollOver = rollOver; }
	bool MoveTopItem(int dir);
	void Resize(int item_w, int item_h);
	void UseDummy(bool useDummy) { m_useDummy = useDummy; }                        // frlistbox.h:93
	void AddNoRButtonRect(const WRect& rect, int index);
	void ResetTopRow(void) { m_prevTopRow = -1; }                                  // frlistbox.h:95
	void SetMouseEvent(bool active);
	int GetColCapacity(void) { return m_colCapacity; }                             // frlistbox.h:97

	// frlistbox.h:106; calls f with the data of every item, in order.
	template <class F>
	void OrderItem(F f) {
		for (ITEM_LIST::iterator i = m_itemList.begin(); i != m_itemList.end(); ++i) {
			f((*i)->pData);
		}
	}

	// vtable slot 23
	virtual void SetClientRect(const WRect& rect);

protected:
	virtual void OnDraw(void);
	virtual void OnProc(const float deltaTime);
	virtual void OnResize(void);
	virtual void OnMouseMove(const WPoint& mousePos);
	virtual void OnSetCursor(bool bInClient, const WPoint& mousePos);
	virtual bool OnLButtonUp(const WPoint& mousePos);
	virtual bool OnLButtonDown(const WPoint& mousePos);
	virtual bool OnRButtonUp(const WPoint& mousePos);
	virtual bool OnRButtonDown(const WPoint& mousePos);
	virtual void OnDblClick(const WPoint& mousePos);
	virtual void EnableKeyFocus(FrInputState& istate);
	virtual void OnKeyFocus(CChatMsg* im);

	void SetCursorToFirstItem(void);
	void SetCursorToLastItem(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

public:
	ITEM_LIST m_itemList;                     // 0xd4

protected:
	FrGuiItem* m_pItem;                       // 0xe0
	sSize m_itemSize;                         // 0xe4
	int m_colCapacity;                        // 0xec
	int m_prevTopRow;                         // 0xf0
	ITEM_LIST::iterator m_itemTop;            // 0xf4
	ITEM_LIST::iterator m_itemBottom;         // 0xf8
	bool m_multiSelect;                       // 0xfc
	bool m_useRightButton;                    // 0xfd
	bool m_rollOver;                          // 0xfe
	FrListItem* m_pItemUnderCursor;           // 0x100
	FrListItem* m_pSelected;                  // 0x104
	FrListItem* m_pPreviousSelected;          // 0x108
	int m_dummyCount;                         // 0x10c
	bool m_useDummy;                          // 0x110
	std::map<int, WRect> m_noRButtonMap;      // 0x114
	const Bitmap* m_listBgImg;                // 0x120
};

CHECK_SIZE(FrListBox::sSize, 0x8);
CHECK_SIZE(FrListBox, 0x124);
BEGIN_LAYOUT_CHECK(FrListBox)
	CHECK_FIELD(FrListBox, m_itemList, 0xd4);
	CHECK_FIELD(FrListBox, m_pItem, 0xe0);
	CHECK_FIELD(FrListBox, m_itemSize, 0xe4);
	CHECK_FIELD(FrListBox, m_colCapacity, 0xec);
	CHECK_FIELD(FrListBox, m_prevTopRow, 0xf0);
	CHECK_FIELD(FrListBox, m_itemTop, 0xf4);
	CHECK_FIELD(FrListBox, m_itemBottom, 0xf8);
	CHECK_FIELD(FrListBox, m_multiSelect, 0xfc);
	CHECK_FIELD(FrListBox, m_useRightButton, 0xfd);
	CHECK_FIELD(FrListBox, m_rollOver, 0xfe);
	CHECK_FIELD(FrListBox, m_pItemUnderCursor, 0x100);
	CHECK_FIELD(FrListBox, m_pSelected, 0x104);
	CHECK_FIELD(FrListBox, m_pPreviousSelected, 0x108);
	CHECK_FIELD(FrListBox, m_dummyCount, 0x10c);
	CHECK_FIELD(FrListBox, m_useDummy, 0x110);
	CHECK_FIELD(FrListBox, m_noRButtonMap, 0x114);
	CHECK_FIELD(FrListBox, m_listBgImg, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRLISTBOX_H
