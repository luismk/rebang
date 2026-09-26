#pragma once
#include <list>
#include <map>
#include "rtti.h"
#include "frwnd.h"
#include "../../Wangreal/include/wtypes.h"

class Bitmap;
class CChatMsg;
class FrGuiItem;
class FrWndManager;
struct FrInputState;

struct FrListItem
{
	FrListItem();
	~FrListItem();
	void AddNoRBtnIdx(int idx);

	void* pData;
	WPoint pos;
	bool underCursor;
	bool selected;
	int no;
	int idx;
	std::list<int> noRBtnIdxList;
};

class FrListBox : public FrWnd
{
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI() const { return &m_RTTI; }

	typedef std::list<FrListItem*> ITEM_LIST;
	ITEM_LIST m_itemList;

	FrListBox();
	virtual ~FrListBox();

	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	void RecalcColCapacity(bool b);
	void SetMultiSelect(bool multi);
	void UseRightButton(bool use);
	const Bitmap* GetBitmap(const char* name);
	FrListItem* GetItemUnderCursor() { return m_pItemUnderCursor; }
	FrListItem* GetSelected();
	FrListItem* GetPreviousSelected();
	void SelectItem(FrListItem* item, bool b);
	void UnselectItem(FrListItem* item);
	void UnselectAllItem();
	void ToggleItem(FrListItem* item);
	int GetCurrentItemSize(bool b);
	int GetItemWidth() { return m_itemSize.w; }
	int GetItemHeight() { return m_itemSize.h; }
	void SetItemSize(int w, int h)
	{
		m_itemSize.w = w;
		m_itemSize.h = h;
	}
	FrListItem* AddItem(void* data);
	FrListItem* FindItem(void* data, int no);
	FrListItem* FindItem(void* data);
	FrListItem* FindItem(FrListItem* item);
	FrListItem* GetItem(int no);
	void DelItem(void* data, int no);
	void DelItem(void* data);
	void DelItem(FrListItem* item);
	ITEM_LIST::iterator _DelItem(ITEM_LIST::iterator it);
	void ClearItem();
	void SortItem(bool (*compare)(const void*, const void*));
	void RecalcTopBottom();
	void EnableScrollBar(bool enable);
	virtual void SetClientRect(const WRect& rect);
	void EnableRollOver(bool enable);
	bool MoveTopItem(int n);
	void Resize(int w, int h);
	void UseDummy(bool use);
	void AddNoRButtonRect(const WRect& rect, int idx);
	void ResetTopRow();
	void SetMouseEvent(bool active);
	int GetColCapacity() { return m_colCapacity; }

protected:
	virtual void OnDraw();
	virtual void OnProc(const float deltaTime);
	virtual void OnResize();
	virtual void OnMouseMove(const WPoint& point);
	virtual void OnSetCursor(bool active, const WPoint& point);
	virtual bool OnLButtonUp(const WPoint& point);
	virtual bool OnLButtonDown(const WPoint& point);
	virtual bool OnRButtonUp(const WPoint& point);
	virtual bool OnRButtonDown(const WPoint& point);
	virtual void OnDblClick(const WPoint& point);
	virtual void EnableKeyFocus(FrInputState& input);
	virtual void OnKeyFocus(CChatMsg* im);
	void SetCursorToFirstItem();
	void SetCursorToLastItem();

	FrGuiItem* m_pItem;

	struct sSize
	{
		int w;
		int h;
	};

	sSize m_itemSize;
	int m_colCapacity;
	int m_prevTopRow;
	ITEM_LIST::iterator m_itemTop;
	ITEM_LIST::iterator m_itemBottom;
	bool m_multiSelect;
	bool m_useRightButton;
	bool m_rollOver;
	FrListItem* m_pItemUnderCursor;
	FrListItem* m_pSelected;
	FrListItem* m_pPreviousSelected;
	int m_dummyCount;
	bool m_useDummy;
	std::map<int, WRect> m_noRButtonMap;
	const Bitmap* m_listBgImg;
};
