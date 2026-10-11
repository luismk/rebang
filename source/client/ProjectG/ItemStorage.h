// Original: d:\build\custom\temp\code\client\projectg\itemstorage.h
#ifndef PG_CLIENT_PROJECTG_ITEMSTORAGE_H
#define PG_CLIENT_PROJECTG_ITEMSTORAGE_H

#include <list>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "singleton.h"
#include "globalnetworkdefine.h"
#include "../client/Wangreal/include/wtypes.h"

// The item storage notice dialog; its size (0x110, no members of its own) is
// the `new` of CreateForm<FrItemStorageNotice>. The constructor and destructor
// are inline at itemstorage.h:14-15; their emitted copies are defined out of
// line in realmyroommainui.cpp.
class FrItemStorageNotice : public FrForm {
public:
	FrItemStorageNotice(void);            // itemstorage.h:14
	virtual ~FrItemStorageNotice(void);   // itemstorage.h:15

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // itemstorage.h:23

	virtual void OnProc(const float dt);
	virtual void OnDraw(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // itemstorage.cpp:26

	void OnLBDownConfirm(void);          // itemstorage.cpp:34
	void OnOwnerDrawPrintOut(intptr_t id);    // itemstorage.cpp:39
};

CHECK_SIZE(FrItemStorageNotice, 0x110);

class CIndexControl;
class FrListBox;
class WTitleFont;
struct FrListItem;

class FrArea;
class FrButton;
class FrEdit;

// The password panel of the item storage; its size (0x1b8) is the `new` of
// FrItemStorageAccessPanelMakeInstance.
class FrItemStorageAccessPanel : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	enum eFormType {
	};

	FrItemStorageAccessPanel(void);            // itemstorage.cpp:94
	virtual ~FrItemStorageAccessPanel(void);   // itemstorage.cpp:132

	// itemstorage.h:83, emitted out of line by RealMyRoomMainUI
	void SetFormType(eFormType formType) {
		m_formType = formType;

		switch (formType) {
		case 0:
			SetCaption("\xc3\xd6\xc3\xca \xc6\xd0\xbd\xba\xbf\xf6\xb5\xe5\xb8\xa6 \xbc\xb3\xc1\xa4\xc7\xd8\xc1\xd6\xbc\xbc\xbf\xe4.");
			SetDesc("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 4\xc0\xda\xb8\xae \xc0\xd4\xb7\xc2.");
			m_inputStep = 1;
			break;
		case 1:
			SetCaption("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 \xc0\xd4\xb7\xc2");
			SetDesc("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 4\xc0\xda\xb8\xae \xc0\xd4\xb7\xc2.");
			m_inputStep = 1;
			break;
		case 2:
			SetCaption("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 \xba\xaf\xb0\xe6");
			SetDesc("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 4\xc0\xda\xb8\xae \xc0\xd4\xb7\xc2.");
			m_inputStep = 0;
			break;
		case 3:
			SetCaption("\xc0\xe1\xb1\xdd\xbc\xb3\xc1\xa4");
			SetDesc("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 4\xc0\xda\xb8\xae \xc0\xd4\xb7\xc2.");
			m_inputStep = 1;
			break;
		case 4:
			SetCaption("\xc0\xe1\xb1\xdd\xc7\xd8\xc1\xa6");
			SetDesc("\xba\xf1\xb9\xd0\xb9\xf8\xc8\xa3 4\xc0\xda\xb8\xae \xc0\xd4\xb7\xc2.");
			m_inputStep = 1;
			break;
		}
	}
	eFormType GetFormType(void) const;  // itemstorage.h:130
	void SendPasswordToServer(void);

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // itemstorage.h:138

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // itemstorage.cpp:399

	template <class T> bool TBindControl(T*& pControl, int id) {   // itemstorage.h:72
		pControl = DYNAMIC_CAST(T, id);

		return pControl ? true : false;
	}
	bool DiffPassword(void);   // itemstorage.cpp:311
	bool InputPassword(unsigned char number);
	void OnInitPrintOut(intptr_t id);   // itemstorage.cpp:436
	void OnLBDownNumberSlot0(void);   // itemstorage.cpp:697
	void OnLBDownNumberSlot1(void);   // itemstorage.cpp:698
	void OnLBDownNumberSlot2(void);   // itemstorage.cpp:699
	void OnLBDownNumberSlot3(void);   // itemstorage.cpp:700
	void OnLBDownNumberSlot4(void);   // itemstorage.cpp:701
	void OnLBDownNumberSlot5(void);   // itemstorage.cpp:702
	void OnLBDownNumberSlot6(void);   // itemstorage.cpp:703
	void OnLBDownNumberSlot7(void);   // itemstorage.cpp:704
	void OnLBDownNumberSlot8(void);   // itemstorage.cpp:705
	void OnLBDownNumberSlot9(void);   // itemstorage.cpp:706
	void SetButtonImage(FrButton* pButton, unsigned char number);
	void OnOwnerDrawPrintOut(intptr_t id);
	void OnInitNumberSlot0(intptr_t id);   // itemstorage.cpp:607
	void OnInitNumberSlot1(intptr_t id);
	void OnInitNumberSlot2(intptr_t id);
	void OnInitNumberSlot3(intptr_t id);
	void OnInitNumberSlot4(intptr_t id);
	void OnInitNumberSlot5(intptr_t id);
	void OnInitNumberSlot6(intptr_t id);
	void OnInitNumberSlot7(intptr_t id);
	void OnInitNumberSlot8(intptr_t id);
	void OnInitNumberSlot9(intptr_t id);
	void OnInitConfirm(intptr_t id);       // itemstorage.cpp:711
	void OnLBDownConfirm(void);
	void OnInitCancel(intptr_t id);        // itemstorage.cpp:882
	void OnLBDownCancel(void);        // itemstorage.cpp:890

private:
	eFormType m_formType;              // 0x110
	int m_inputStep;                   // 0x114, 1-3 (OnLBDownCancel)
	char m_password[0x20];             // 0x118
	char m_newPassword[0x20];          // 0x138
	char m_confirmPassword[0x20];      // 0x158, compared with m_newPassword
	unsigned char m_numberSlot[10];    // 0x178, digit of each number button (shuffled)
	FrArea* m_pPrintOut;               // 0x184
	FrButton* m_pNumberSlotBn[10];     // 0x188
	FrButton* m_pConfirmBn;            // 0x1b0
	FrButton* m_pCancelBn;             // 0x1b4
};

CHECK_SIZE(FrItemStorageAccessPanel, 0x1b8);
BEGIN_LAYOUT_CHECK(FrItemStorageAccessPanel)
	CHECK_FIELD(FrItemStorageAccessPanel, m_formType, 0x110);
	CHECK_FIELD(FrItemStorageAccessPanel, m_inputStep, 0x114);
	CHECK_FIELD(FrItemStorageAccessPanel, m_password, 0x118);
	CHECK_FIELD(FrItemStorageAccessPanel, m_newPassword, 0x138);
	CHECK_FIELD(FrItemStorageAccessPanel, m_confirmPassword, 0x158);
	CHECK_FIELD(FrItemStorageAccessPanel, m_numberSlot, 0x178);
	CHECK_FIELD(FrItemStorageAccessPanel, m_pPrintOut, 0x184);
	CHECK_FIELD(FrItemStorageAccessPanel, m_pNumberSlotBn, 0x188);
	CHECK_FIELD(FrItemStorageAccessPanel, m_pConfirmBn, 0x1b0);
	CHECK_FIELD(FrItemStorageAccessPanel, m_pCancelBn, 0x1b4);
END_LAYOUT_CHECK

// Count input of a stored item; its size (0x1d8) is the `new` of
// FrItemCounterMakeInstance.
class FrItemCounter : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CItemStorage;   // ReqPopStoredItemInfo fills the form
public:
	FrItemCounter(void);            // itemstorage.cpp:1015
	virtual ~FrItemCounter(void);   // itemstorage.cpp:1024

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // itemstorage.h:200

	void SetStoredItemInfo(const sStoredItemInfo& info);
	const sStoredItemInfo& GetStoredItemInfo(void) const;
	void SetType(unsigned long type);
	unsigned long GetType(void) const;

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // itemstorage.cpp:1033

	void OnInitDrawArea(intptr_t param);   // itemstorage.cpp:1046
	void OnOwnerDrawDrawArea(intptr_t id); // itemstorage.cpp:1050
	void OnInitInput(intptr_t param);      // itemstorage.cpp:1095
	void OnInitConfirm(intptr_t param);    // itemstorage.cpp:1101
	void OnLBDownConfirm(void);  // itemstorage.cpp:1111
	void OnInitCancel(intptr_t param);     // itemstorage.cpp:1142
	void OnLBDownCancel(void);   // itemstorage.cpp:1152

private:
	FrArea* m_pDrawArea;              // 0x110
	FrEdit* m_pInput;                 // 0x114
	FrButton* m_pConfirmBn;           // 0x118
	FrButton* m_pCancelBn;            // 0x11c
	unsigned long m_type;             // 0x120
	sStoredItemInfo m_storedItemInfo; // 0x124
};

CHECK_SIZE(FrItemCounter, 0x1d8);
BEGIN_LAYOUT_CHECK(FrItemCounter)
	CHECK_FIELD(FrItemCounter, m_pDrawArea, 0x110);
	CHECK_FIELD(FrItemCounter, m_pInput, 0x114);
	CHECK_FIELD(FrItemCounter, m_pConfirmBn, 0x118);
	CHECK_FIELD(FrItemCounter, m_pCancelBn, 0x11c);
	CHECK_FIELD(FrItemCounter, m_type, 0x120);
	CHECK_FIELD(FrItemCounter, m_storedItemInfo, 0x124);
END_LAYOUT_CHECK

// Confirmation of a stored item move; its size (0x1d0) is the `new` of
// FrItemConfirmMakeInstance.
class FrItemConfirm : public FrForm {
	LAYOUT_CHECK_FRIEND;
	friend class CItemStorage;   // ReqPopStoredItemInfo fills the form
public:
	FrItemConfirm(void);            // itemstorage.cpp:1428
	virtual ~FrItemConfirm(void);   // itemstorage.cpp:1436

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // itemstorage.h:273

	void SetStoredItemInfo(const sStoredItemInfo& info);
	const sStoredItemInfo& GetStoredItemInfo(void) const;
	void SetType(unsigned long type);
	unsigned long GetType(void) const;

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // itemstorage.cpp:1445

	void OnInitDrawArea(intptr_t param);   // itemstorage.cpp:1457
	void OnOwnerDrawDrawArea(intptr_t id); // itemstorage.cpp:1461
	void OnInitConfirm(intptr_t param);    // itemstorage.cpp:1524
	void OnLBDownConfirm(void);   // itemstorage.cpp:1534
	void OnInitCancel(intptr_t param);     // itemstorage.cpp:1542
	void OnLBDownCancel(void);    // itemstorage.cpp:1552

private:
	FrArea* m_pDrawArea;              // 0x110
	FrButton* m_pConfirmBn;           // 0x114
	FrButton* m_pCancelBn;            // 0x118
	unsigned long m_type;             // 0x11C
	sStoredItemInfo m_storedItemInfo; // 0x120
};

CHECK_SIZE(FrItemConfirm, 0x1d0);
BEGIN_LAYOUT_CHECK(FrItemConfirm)
	CHECK_FIELD(FrItemConfirm, m_pDrawArea, 0x110);
	CHECK_FIELD(FrItemConfirm, m_pConfirmBn, 0x114);
	CHECK_FIELD(FrItemConfirm, m_pCancelBn, 0x118);
	CHECK_FIELD(FrItemConfirm, m_type, 0x11C);
	CHECK_FIELD(FrItemConfirm, m_storedItemInfo, 0x120);
END_LAYOUT_CHECK

// Pang deposit and withdrawal of the item storage; its size (0x138) is the
// `new` of FrItemStoragePangBankMakeInstance.
class FrItemStoragePangBank : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	// What the pang bank form does; the names are inferred from the buttons
	// opening it (the storage's pang "plus" opens 0, "minus" 1).
	enum eType {
		TYPE_DEPOSIT = 0,
		TYPE_WITHDRAW = 1
	};

	FrItemStoragePangBank(void);            // itemstorage.cpp:1163
	virtual ~FrItemStoragePangBank(void);   // itemstorage.cpp:1173

	// itemstorage.h:344
	void SetStoredPang(__int64 pang) {
		m_storedPang = pang;
	}
	__int64 GetStoredPang(void) const;
	// itemstorage.h:352
	void SetMyPang(__int64 pang) {
		m_myPang = pang;
	}
	__int64 GetMyPang(void) const;
	void SetType(unsigned long type);
	unsigned long GetType(void) const;

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // itemstorage.h:388

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // itemstorage.cpp:1182

	void OnInitDrawArea(intptr_t param);        // itemstorage.cpp:1196
	void OnOwnerDrawDrawArea(intptr_t id);      // itemstorage.cpp:1200
	void OnInitInput(intptr_t param);           // itemstorage.cpp:1321
	bool OnInputEnterKey(int param);       // itemstorage.cpp:1327
	void OnInitConfirm(intptr_t param);         // itemstorage.cpp:1334
	void OnLBDownConfirm(void);            // itemstorage.cpp:1342
	void OnInitCancel(intptr_t param);          // itemstorage.cpp:1406
	void OnLBDownCancel(void);             // itemstorage.cpp:1416

private:
	FrArea* m_pDrawArea;     // 0x110
	FrEdit* m_pInput;        // 0x114
	FrButton* m_pConfirmBn;  // 0x118
	FrButton* m_pCancelBn;   // 0x11c
	unsigned long m_type;    // 0x120
	UNKNOWN(0x124, 0x128);
	__int64 m_storedPang;    // 0x128
	__int64 m_myPang;        // 0x130
};

CHECK_SIZE(FrItemStoragePangBank, 0x138);
BEGIN_LAYOUT_CHECK(FrItemStoragePangBank)
	CHECK_FIELD(FrItemStoragePangBank, m_pDrawArea, 0x110);
	CHECK_FIELD(FrItemStoragePangBank, m_pInput, 0x114);
	CHECK_FIELD(FrItemStoragePangBank, m_pConfirmBn, 0x118);
	CHECK_FIELD(FrItemStoragePangBank, m_pCancelBn, 0x11c);
	CHECK_FIELD(FrItemStoragePangBank, m_type, 0x120);
	CHECK_FIELD(FrItemStoragePangBank, m_storedPang, 0x128);
	CHECK_FIELD(FrItemStoragePangBank, m_myPang, 0x130);
END_LAYOUT_CHECK

// The drag-and-drop state of a stored item; its constructor is
// compiler-generated (sTradeItem at 0x18 has one). The copy in
// SetDragDropItem is memberwise, so 0x14-0xC4 is not an sStoredItemInfo
// (whose operator= is user-declared) here.
struct sStoredItemDragDrop {
	// The drag modes; the names are inferred from CheckDrop (an item of the
	// inventory list dropped on the storage is pushed, a stored item dropped
	// on the list is popped).
	enum MODE {
		MODE_NONE = 0,
		MODE_PUSH = 1,
		MODE_POP = 2
	};

	MODE mode;                        // 0x00
	WPoint pos;                       // 0x04, mouse position (CheckDrag)
	unsigned long index;              // 0x0c, GetDragingItemIndex
	const sItemInfo* pItemInfo;       // 0x10, pushed by CheckDrop
	unsigned long slot;               // 0x14, storage slot of the popped item
	sTradeItem item;                  // 0x18, popped by CheckDrop
};
CHECK_SIZE(sStoredItemDragDrop, 0xc4);
CHECK_FIELD(sStoredItemDragDrop, mode, 0x00);
CHECK_FIELD(sStoredItemDragDrop, pos, 0x04);
CHECK_FIELD(sStoredItemDragDrop, index, 0x0c);
CHECK_FIELD(sStoredItemDragDrop, pItemInfo, 0x10);
CHECK_FIELD(sStoredItemDragDrop, slot, 0x14);
CHECK_FIELD(sStoredItemDragDrop, item, 0x18);

// The item storage (a WSingleton). Only the members this project's code uses
// so far are declared.
class CItemStorage : public WSingleton<CItemStorage>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	CItemStorage(void);                                              // itemstorage.cpp:1564
	virtual ~CItemStorage(void);                                     // itemstorage.cpp:1604
	void InitIndexControl(unsigned long totalCount, unsigned long countPerPage);   // itemstorage.cpp:2354
	bool NeedPageIndexer(void);                                      // itemstorage.cpp:2439
	void SetDragDropItem(sStoredItemDragDrop& dragDrop);             // itemstorage.cpp:2688
	unsigned long GetDragingItemIndex(sStoredItemDragDrop::MODE mode);   // itemstorage.cpp:2786
	void Lock(void);                                                 // itemstorage.cpp:1610
	void UnLock(void);                                               // itemstorage.cpp:1624
	void AddFrontUI(FrWnd* pWnd);                                    // itemstorage.cpp:1638
	void AddBackGroundUI(FrWnd* pWnd, bool bVisibleGroup);          // itemstorage.cpp:1647
	void SetVisibleFrontUI(bool bVisible);                           // itemstorage.cpp:1662
	void SetVisibleBackGroundUI(bool bVisible);                      // itemstorage.cpp:1677
	void SetEnableFrontUI(bool bEnable);                             // itemstorage.cpp:1690
	void SetEnableBackGroundUI(bool bEnable);                        // itemstorage.cpp:1703
	void Print(float x, float y, const char* text, float spacing, int align, float scale, unsigned long color);   // itemstorage.cpp:1717
	void DrawBackgroundImage(void);
	void SetDrag(sStoredItemDragDrop::MODE mode);
	void ClearStoredItemInfo(void);                                  // itemstorage.cpp:1796
	void AddStoredItemInfo(const sStoredItemInfo& info);             // itemstorage.cpp:1814
	bool OnResultPangBank(int ret, FrForm* pForm);                   // itemstorage.cpp:2476
	bool OnResultItemCounter(int ret, FrForm* pForm);                // itemstorage.cpp:2516
	bool OnResultItemConfirm(int ret, FrForm* pForm);                // itemstorage.cpp:2572
	bool CheckEquiped(unsigned long id);                             // itemstorage.cpp:1868
	void DragDropItemDraw(void);
	void OpenPangBank(FrItemStoragePangBank::eType type);
	void IndexControlDraw(WPoint& pt);
	void IndexControlClick(void);
	void IndexControlNext(void);
	void IndexControlPrev(void);
	void DrawStoredItemInfo(const FrListBox*& pListBox, const FrListItem& item);
	void ReqPopStoredItemInfo(const sStoredItemInfo& info);
	void ReqPopStoredItemInfo(const FrListItem& item);    // itemstorage.cpp:2018
	bool CheckStoredItemInfo(sStoredItemInfo& stored, const sItemInfo& info);   // itemstorage.cpp (not yet transcribed)
	void ReqPushStoredItemInfo(const sItemInfo& info);   // itemstorage.cpp:2082
	void ReqPushStoredItemInfo(const FrListItem& item);   // itemstorage.cpp:2070
	void MakeStoredItemInfo(FrListBox*& pListBox);        // itemstorage.cpp:2209
	bool IsVisible(void) const;
	bool IsLock(void) const;                 // itemstorage.h:451
	__int64 GetStoredPang(void) const;
	unsigned long GetTotalPage(void) const;  // itemstorage.h:489
	unsigned long GetCurPage(void) const;    // itemstorage.h:497
	CIndexControl* GetIndexControl(void);    // itemstorage.h:501
	void SetItemStorageArea(WRect rect);     // itemstorage.h:541
	void SetItemListArea(WRect rect);        // itemstorage.h:542

protected:
	bool CheckDrag(void);                    // itemstorage.cpp:2627
	bool CheckDrop(void);                    // itemstorage.cpp:2644

private:
	bool m_visible;       // 0x2c
	bool m_lock;          // 0x2d
	UNKNOWN(0x2E, 0x30);
	std::list<FrWnd*> m_frontUI;              // 0x30
	std::list<FrWnd*> m_backGroundUI;         // 0x3c, SetEnableBackGroundUI
	std::list<FrWnd*> m_backGroundVisibleUI;  // 0x48, SetVisibleBackGroundUI
	WTitleFont* m_pTitleFont;       // 0x54, released by the destructor
	__int64 m_storedPang; // 0x58
	unsigned long m_totalPage;      // 0x60
	unsigned long m_curPage;        // 0x64
	CIndexControl* m_indexControl;  // 0x68
	std::vector<sStoredItemInfo*> m_storedItemInfo;  // 0x6c, MakeStoredItemInfo
	std::vector<sItemInfo*> m_itemInfo;              // 0x7c, ClearStoredItemInfo
	FrItemStoragePangBank* m_pPangBank;              // 0x8c, OpenPangBank
	FrItemCounter* m_pItemCounter;                   // 0x90, ReqPopStoredItemInfo
	FrItemConfirm* m_pItemConfirm;                   // 0x94, ReqPopStoredItemInfo
	WRect m_itemStorageArea;        // 0x98
	WRect m_itemListArea;           // 0xa8
	sStoredItemDragDrop m_dragDrop; // 0xb8
};
BEGIN_LAYOUT_CHECK(CItemStorage)
	CHECK_FIELD(CItemStorage, m_visible, 0x2C);
	CHECK_FIELD(CItemStorage, m_lock, 0x2D);
	CHECK_FIELD(CItemStorage, m_frontUI, 0x30);
	CHECK_FIELD(CItemStorage, m_backGroundUI, 0x3C);
	CHECK_FIELD(CItemStorage, m_backGroundVisibleUI, 0x48);
	CHECK_FIELD(CItemStorage, m_pTitleFont, 0x54);
	CHECK_FIELD(CItemStorage, m_storedPang, 0x58);
	CHECK_FIELD(CItemStorage, m_totalPage, 0x60);
	CHECK_FIELD(CItemStorage, m_curPage, 0x64);
	CHECK_FIELD(CItemStorage, m_indexControl, 0x68);
	CHECK_FIELD(CItemStorage, m_storedItemInfo, 0x6C);
	CHECK_FIELD(CItemStorage, m_itemInfo, 0x7C);
	CHECK_FIELD(CItemStorage, m_pPangBank, 0x8C);
	CHECK_FIELD(CItemStorage, m_pItemCounter, 0x90);
	CHECK_FIELD(CItemStorage, m_pItemConfirm, 0x94);
	CHECK_FIELD(CItemStorage, m_itemStorageArea, 0x98);
	CHECK_FIELD(CItemStorage, m_itemListArea, 0xA8);
	CHECK_FIELD(CItemStorage, m_dragDrop, 0xB8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_ITEMSTORAGE_H
