// Original: d:\build\custom\temp\code\client\fresh\include\frcombobox.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRCOMBOBOX_H
#define PG_CLIENT_FRESH_INCLUDE_FRCOMBOBOX_H
#include "..\..\defines.h"

#include <list>
#include <string>
#include <vector>

#include "fredit.h"
#include "frelement.h"

class Bitmap;
class FrButton;
class FrListBox;

// An edit with a drop-down list. Layout, access and methods from the PDB
// (size 0x2bc, base FrEdit; OnListBtnDown is the new virtual of slot 23).
class FrComboBox : public FrEdit {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frcombobox.h:24

	FrComboBox(void);
	virtual ~FrComboBox(void);

	void Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent);
	void ResizeListRect(void);
	void AddString(const char* str);
	void DelString(int index);
	int FindString(const char* str);
	void ClearList(void);
	bool IsSelectString(const std::string& str);
	void IncreaseMaxListNum(int num);
	// Never emitted as a COMDAT; the bodies are inferred from their members.
	int GetMaxComboListNum(void) { return m_maxListNum; }
	int GetComboListNum(void) { return (int)m_comboList.size(); }
	virtual void Enable(bool bEnable);
	const FrButton* GetButton(void) { return m_pButton; }
	const FrListBox* GetListBox(void) { return m_pListBox; }

protected:
	virtual void OnKeyFocus(CChatMsg* im);
	void OnUnfold(void);
	void OnListOwnerDraw(int param);
	void OnListLostKeyFocus(int param);
	// vtable slot 23
	virtual void OnListBtnDown(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	int m_maxListNum;                         // 0x1b4
	std::list<FrLine*> m_comboList;           // 0x1b8
	std::list<FrLine*>::iterator m_selected;  // 0x1c4
	FrButton* m_pButton;                      // 0x1c8
	FrGuiItem m_buttonInfo;                   // 0x1cc
	FrListBox* m_pListBox;                    // 0x240
	FrGuiItem m_listInfo;                     // 0x244
	unsigned long m_selectColor;              // 0x2b8
};

CHECK_SIZE(FrComboBox, 0x2bc);
BEGIN_LAYOUT_CHECK(FrComboBox)
	CHECK_FIELD(FrComboBox, m_maxListNum, 0x1b4);
	CHECK_FIELD(FrComboBox, m_comboList, 0x1b8);
	CHECK_FIELD(FrComboBox, m_selected, 0x1c4);
	CHECK_FIELD(FrComboBox, m_pButton, 0x1c8);
	CHECK_FIELD(FrComboBox, m_buttonInfo, 0x1cc);
	CHECK_FIELD(FrComboBox, m_pListBox, 0x240);
	CHECK_FIELD(FrComboBox, m_listInfo, 0x244);
	CHECK_FIELD(FrComboBox, m_selectColor, 0x2b8);
END_LAYOUT_CHECK

// A combo box drawing its items as text or images (PDB: size 0x2e8, base
// FrComboBox).
class FrComboCtlEx : public FrComboBox {
	LAYOUT_CHECK_FRIEND;
public:
	enum {
		NORMAL = 0,
		OVER = 1,
		MAX = 2
	};

	enum eItemDrawStyle {
		IDS_TEXT = 0,
		IDS_IMG = 1
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frcombobox.h:81

	FrComboCtlEx(void);
	virtual ~FrComboCtlEx(void);

	void Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent);
	void AddString(const char* str);
	void AddTexItem(const char* name);
	void SelectItem(int index);
	void ClearAllListItem(void);
	unsigned char GetCurrentIdx(void) { return m_byCurrentIdx; }   // frcombobox.h:105

private:
	void AlignListItem(void);

protected:
	virtual void OnKeyFocus(CChatMsg* im);
	void OnUnfold(void);
	void OnUnfoldOwnerDraw(int param);
	void OnListOwnerDraw(int param);
	virtual void OnListBtnDown(void);
	void OnListLostKeyFocus(int param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	const Bitmap* m_BitmapSkin[MAX];              // 0x2bc
	const Bitmap* m_pSelectItem;                  // 0x2c4
	std::vector<const Bitmap*> m_TexItemList;     // 0x2c8
	eItemDrawStyle m_eDrawStyle;                  // 0x2d8
	WPoint m_ItemOffset;                          // 0x2dc
	bool m_bFold;                                 // 0x2e4
	unsigned char m_byCurrentIdx;                 // 0x2e5
};

CHECK_SIZE(FrComboCtlEx, 0x2e8);
BEGIN_LAYOUT_CHECK(FrComboCtlEx)
	CHECK_FIELD(FrComboCtlEx, m_BitmapSkin, 0x2bc);
	CHECK_FIELD(FrComboCtlEx, m_pSelectItem, 0x2c4);
	CHECK_FIELD(FrComboCtlEx, m_TexItemList, 0x2c8);
	CHECK_FIELD(FrComboCtlEx, m_eDrawStyle, 0x2d8);
	CHECK_FIELD(FrComboCtlEx, m_ItemOffset, 0x2dc);
	CHECK_FIELD(FrComboCtlEx, m_bFold, 0x2e4);
	CHECK_FIELD(FrComboCtlEx, m_byCurrentIdx, 0x2e5);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRCOMBOBOX_H
