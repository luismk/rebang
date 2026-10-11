// Original: d:\build\custom\temp\code\client\projectg\pointeventdlg.h
#ifndef PG_CLIENT_PROJECTG_POINTEVENTDLG_H
#define PG_CLIENT_PROJECTG_POINTEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;

// One item of a commodity (size 0x10, pointeventdlg.cpp:38-86).
class CCommodityItem {
	LAYOUT_CHECK_FRIEND;
public:
	enum eItemType {
	};

	CCommodityItem(void);
	CCommodityItem(const CCommodityItem&);
	CCommodityItem(unsigned long, unsigned long, eItemType);
	virtual ~CCommodityItem(void) {
	}

	void SetTypeID(unsigned long);
	void SetValue(unsigned long);
	void SetItemType(eItemType);
	unsigned long GetTypeID(void) const;
	unsigned long GetValue(void) const;
	eItemType GetItemType(void) const;
	const CCommodityItem& operator=(const CCommodityItem&);

private:
	eItemType m_itemType;  // 0x4
	unsigned long m_typeID; // 0x8
	unsigned long m_value;  // 0xc
};

CHECK_SIZE(CCommodityItem, 0x10);
BEGIN_LAYOUT_CHECK(CCommodityItem)
	CHECK_FIELD(CCommodityItem, m_itemType, 0x4);
	CHECK_FIELD(CCommodityItem, m_typeID, 0x8);
	CHECK_FIELD(CCommodityItem, m_value, 0xc);
END_LAYOUT_CHECK

// A commodity of the point event: up to four items and a cost (size 0x48,
// pointeventdlg.cpp:101-170, destructor pointeventdlg.h:79).
class CCommodity {
	LAYOUT_CHECK_FRIEND;
public:
	CCommodity(void);
	CCommodity(const CCommodity&);
	CCommodity(const CCommodityItem&, unsigned short);
	virtual ~CCommodity(void) {
	}

	void SetCommodityCost(unsigned short);
	unsigned short GetCommodityItemCount(void) const;
	unsigned short GetCommodityCost(void) const;
	bool InsertCommodityItem(const CCommodityItem&);
	const CCommodityItem& GetCommodityItem(unsigned short) const;
	const CCommodity& operator=(const CCommodity&);

private:
	CCommodityItem m_items[4];      // 0x4
	unsigned short m_itemCount;     // 0x44
	unsigned short m_commodityCost; // 0x46
};

CHECK_SIZE(CCommodity, 0x48);
BEGIN_LAYOUT_CHECK(CCommodity)
	CHECK_FIELD(CCommodity, m_items, 0x4);
	CHECK_FIELD(CCommodity, m_itemCount, 0x44);
	CHECK_FIELD(CCommodity, m_commodityCost, 0x46);
END_LAYOUT_CHECK

// The ten commodities of the point event (size 0x2d4, pointeventdlg.cpp:176-308,
// destructor pointeventdlg.h:107).
class CCommodityTable {
	LAYOUT_CHECK_FRIEND;
public:
	CCommodityTable(void);
	virtual ~CCommodityTable(void) {
	}

	const CCommodity& GetCommodity(unsigned short) const;

protected:
	bool InitializeTable(void);

private:
	CCommodity m_commodities[10]; // 0x4
};

CHECK_SIZE(CCommodityTable, 0x2d4);
BEGIN_LAYOUT_CHECK(CCommodityTable)
	CHECK_FIELD(CCommodityTable, m_commodities, 0x4);
END_LAYOUT_CHECK

// Status, own count and total count of one commodity (size 0xc, the
// element of CCommodityStatusManager's new[10] array).
struct sCommodityStatus {
	unsigned long status;     // 0x0
	unsigned long myCount;    // 0x4
	unsigned long totalCount; // 0x8
};

CHECK_SIZE(sCommodityStatus, 0xc);
BEGIN_LAYOUT_CHECK(sCommodityStatus)
	CHECK_FIELD(sCommodityStatus, status, 0x0);
	CHECK_FIELD(sCommodityStatus, myCount, 0x4);
	CHECK_FIELD(sCommodityStatus, totalCount, 0x8);
END_LAYOUT_CHECK

// Status of the ten commodities (size 0x30, pointeventdlg.cpp:314-464).
class CCommodityStatusManager {
	LAYOUT_CHECK_FRIEND;
public:
	// How UpdateTotalCount treats a new total count.
	enum eUpdateType {
		UPDATE_TYPE_NONE = 0,
		UPDATE_TYPE_RAISE = 1,     // only if larger
		UPDATE_TYPE_LOWER = 2,     // only if smaller, or if unset
		UPDATE_TYPE_OVERWRITE = 3, // always
	};

	CCommodityStatusManager(void);
	virtual ~CCommodityStatusManager(void);

	unsigned long GetStatus(unsigned short) const;
	unsigned long GetMyCount(unsigned short) const;
	unsigned long GetTotalCount(unsigned short) const;
	eUpdateType GetUpdateType(unsigned short) const;
	void SetStatus(unsigned short, unsigned long);
	void SetMyCount(unsigned short, unsigned long);
	void SetTotalCount(unsigned short, unsigned long);
	void SetData(unsigned short, unsigned long, unsigned long, unsigned long);
	void UpdateTotalCount(unsigned short, unsigned long);
	void UpdateStatus(unsigned short, unsigned long);
	void UpdateMyCount(unsigned short, unsigned long);

protected:
	void InitializeStatus(void);

private:
	sCommodityStatus* m_status;   // 0x4, new[10]
	eUpdateType m_updateTypes[10]; // 0x8
};

CHECK_SIZE(CCommodityStatusManager, 0x30);
BEGIN_LAYOUT_CHECK(CCommodityStatusManager)
	CHECK_FIELD(CCommodityStatusManager, m_status, 0x4);
	CHECK_FIELD(CCommodityStatusManager, m_updateTypes, 0x8);
END_LAYOUT_CHECK

// A button of the point event board bound to a FrButton (size 0x18,
// pointeventdlg.cpp:472-597).
class CCommodityButton {
	LAYOUT_CHECK_FRIEND;
public:
	enum eCommodityButtonType {
		COMMODITY_BUTTON_GIFT1 = 0, // btn_pointevent_gift1_*
		COMMODITY_BUTTON_GIFT2 = 1, // btn_pointevent_gift2_*
		COMMODITY_BUTTON_PLAIN = 2, // images left as they are
		COMMODITY_BUTTON_NONE = 3,  // set by the constructor
	};

	CCommodityButton(void);
	virtual ~CCommodityButton(void);

	eCommodityButtonType GetButtonType(void) const;
	void ChangeButtonType(eCommodityButtonType);
	void ButtonSwitch(bool);
	bool IsVisible(void) const;
	bool BindButton(FrButton*);
	void SetCover(const char*);
	void DrawCover(void);

protected:
	eCommodityButtonType m_buttonType; // 0x4
	FrButton* m_button;                // 0x8, BindButton
	const Bitmap* m_cover;             // 0xc, SetCover
	WPoint m_coverPos;                 // 0x10, the button's position
};

CHECK_SIZE(CCommodityButton, 0x18);
BEGIN_LAYOUT_CHECK(CCommodityButton)
	CHECK_FIELD(CCommodityButton, m_buttonType, 0x4);
	CHECK_FIELD(CCommodityButton, m_button, 0x8);
	CHECK_FIELD(CCommodityButton, m_cover, 0xc);
	CHECK_FIELD(CCommodityButton, m_coverPos, 0x10);
END_LAYOUT_CHECK

// A slot of the point event board (size 0x7c, pointeventdlg.cpp:631-776).
class CCommoditySlot : public CCommodityButton {
	LAYOUT_CHECK_FRIEND;
public:
	// Names guessed from the uses: SetSlotStatus shows the button only for
	// SLOT_STATUS_OPEN, Process locks a slot linked to a completed one.
	enum eSlotStatus {
		SLOT_STATUS_OPEN = 0,
		SLOT_STATUS_CLOSED = 1,
		SLOT_STATUS_COMPLETED = 2,
		SLOT_STATUS_LOCKED = 3,
	};

	// Description of a slot handed to InitSlot (size 0x38).
	struct sCommoditySlotData {
		unsigned short slotIndex;         // 0x0
		unsigned long countDisplay;       // 0x4, 1 own count, 2 total count, 3 both
		unsigned long markFlags;          // 0x8, 1 open mark, 2 completed mark
		const char* openMarkName;         // 0xc
		const char* completedMarkName;    // 0x10
		WPoint myCountPos;                // 0x14
		WPoint totalCountPos;             // 0x1c
		WPoint markPos;                   // 0x24
		FrButton* button;                 // 0x2c
		const char* coverName;            // 0x30
		eCommodityButtonType buttonType;  // 0x34
	};

	CCommoditySlot(void);
	virtual ~CCommoditySlot(void);

	void SetMyCount(unsigned long);
	void SetTotalCount(unsigned long);
	void SetSlotStatus(eSlotStatus);
	unsigned short GetSlotIndex(void) const;
	eSlotStatus GetSlotStatus(void) const;
	void AddSlotLink(const CCommoditySlot*);
	void Process(void);
	void InitSlot(sCommoditySlotData&);
	WRect GetButtonRect(void) const;
	void DrawSlot(WPoint);

private:
	unsigned short m_slotIndex;          // 0x18
	eSlotStatus m_slotStatus;            // 0x1c
	unsigned long m_countDisplay;        // 0x20, InitSlot (sCommoditySlotData::countDisplay)
	unsigned long m_markFlags;           // 0x24, InitSlot (sCommoditySlotData::markFlags)
	const Bitmap* m_openMark;            // 0x28, InitSlot
	const Bitmap* m_completedMark;       // 0x2c, InitSlot
	unsigned long m_myCount;             // 0x30, SetMyCount
	unsigned long m_totalCount;          // 0x34, SetTotalCount
	WPoint m_myCountPos;                 // 0x38, InitSlot
	WPoint m_totalCountPos;              // 0x40, InitSlot
	WPoint m_markPos;                    // 0x48, InitSlot
	unsigned short m_linkCount;          // 0x50, AddSlotLink
	const CCommoditySlot* m_links[10];   // 0x54, AddSlotLink
};

CHECK_SIZE(CCommoditySlot, 0x7c);
CHECK_SIZE(CCommoditySlot::sCommoditySlotData, 0x38);
BEGIN_LAYOUT_CHECK(CCommoditySlot)
	CHECK_FIELD(CCommoditySlot, m_slotIndex, 0x18);
	CHECK_FIELD(CCommoditySlot, m_slotStatus, 0x1c);
	CHECK_FIELD(CCommoditySlot, m_countDisplay, 0x20);
	CHECK_FIELD(CCommoditySlot, m_markFlags, 0x24);
	CHECK_FIELD(CCommoditySlot, m_openMark, 0x28);
	CHECK_FIELD(CCommoditySlot, m_completedMark, 0x2c);
	CHECK_FIELD(CCommoditySlot, m_myCountPos, 0x38);
	CHECK_FIELD(CCommoditySlot, m_totalCountPos, 0x40);
	CHECK_FIELD(CCommoditySlot, m_markPos, 0x48);
	CHECK_FIELD(CCommoditySlot, m_myCount, 0x30);
	CHECK_FIELD(CCommoditySlot, m_totalCount, 0x34);
	CHECK_FIELD(CCommoditySlot, m_linkCount, 0x50);
	CHECK_FIELD(CCommoditySlot, m_links, 0x54);
END_LAYOUT_CHECK

// Notice dialog of the point event (size 0x128: FrForm plus the six
// members the constructor clears, pointeventdlg.cpp:928-933).
class FrPointEventNoticeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // pointeventdlg.h:285

	FrPointEventNoticeDlg(void);
	virtual ~FrPointEventNoticeDlg(void);

	void SetCommodity(int, unsigned long);
	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnLBDownBtnYes(void);
	void OnLBDownBtnNo(void);
	void OnInitBtnYes(intptr_t);
	void OnInitBtnNo(intptr_t);
	void OnInitDrawArea(intptr_t);
	void OnOwnerDrawDrawArea(int);

private:
	FrButton* m_yesBtn;         // 0x110, OnInitBtnYes
	FrButton* m_noBtn;          // 0x114, OnInitBtnNo
	FrArea* m_drawArea;         // 0x118, OnInitDrawArea
	int m_commodityIndex;       // 0x11c, SetCommodity
	const Bitmap* m_itemBitmap; // 0x120, SetCommodity ("pointevent_item_%d")
	unsigned long m_noticeType; // 0x124, SetCommodity: 0 receive, 1 apply, 2 donate
};

CHECK_SIZE(FrPointEventNoticeDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrPointEventNoticeDlg)
	CHECK_FIELD(FrPointEventNoticeDlg, m_yesBtn, 0x110);
	CHECK_FIELD(FrPointEventNoticeDlg, m_noBtn, 0x114);
	CHECK_FIELD(FrPointEventNoticeDlg, m_drawArea, 0x118);
	CHECK_FIELD(FrPointEventNoticeDlg, m_commodityIndex, 0x11c);
	CHECK_FIELD(FrPointEventNoticeDlg, m_itemBitmap, 0x120);
	CHECK_FIELD(FrPointEventNoticeDlg, m_noticeType, 0x124);
END_LAYOUT_CHECK

class FrViewer;

// Description dialog of the point event: a viewer showing an image file
// (size 0x158, pointeventdlg.cpp:1089-1123).
class FrPointEventDescDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // pointeventdlg.h:329

	FrPointEventDescDlg(void);
	virtual ~FrPointEventDescDlg(void);

	void SetDescImage(const char*);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescViewer(intptr_t);

private:
	FrViewer* m_descViewer; // 0x110, OnInitDescViewer
	char m_descImage[64];   // 0x114, SetDescImage
	UNKNOWN(0x154, 0x158);
};

CHECK_SIZE(FrPointEventDescDlg, 0x158);
BEGIN_LAYOUT_CHECK(FrPointEventDescDlg)
	CHECK_FIELD(FrPointEventDescDlg, m_descViewer, 0x110);
	CHECK_FIELD(FrPointEventDescDlg, m_descImage, 0x114);
END_LAYOUT_CHECK

// The point event board (size 0x648, pointeventdlg.cpp:1127-2312).
class FrPointEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // pointeventdlg.h:355

	FrPointEventDlg(void);
	virtual ~FrPointEventDlg(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	bool OnPointEventDescDlgResult(int, FrForm*);
	bool OnPointEventNoticeDlgResult(int, FrForm*);
	void UnSelectCommodity(void);
	void SendUpdateDataPacket(void);
	void LocalDataUpdate(unsigned short);
	void Confirm(unsigned long);
	void ResultMessage(unsigned long);
	unsigned long GetMyPoint(void) const { return m_myPoint; }   // pointeventdlg.h:446
	void SetMyPoint(unsigned long point) { m_myPoint = point; }   // pointeventdlg.h:447

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescButton(intptr_t);
	void OnLBDownDescButton(void);
	void OnOwnerDrawCoverArea(int);
	void OnInitBtnCommodity01(intptr_t);
	void OnInitBtnCommodity02(intptr_t);
	void OnInitBtnCommodity03(intptr_t);
	void OnInitBtnCommodity04(intptr_t);
	void OnInitBtnCommodity05(intptr_t);
	void OnInitBtnCommodity06(intptr_t);
	void OnInitBtnCommodity07(intptr_t);
	void OnInitBtnCommodity08(intptr_t);
	void OnInitBtnCommodity09(intptr_t);
	void OnLBDownBtnCommodity01(void);
	void OnLBDownBtnCommodity02(void);
	void OnLBDownBtnCommodity03(void);
	void OnLBDownBtnCommodity04(void);
	void OnLBDownBtnCommodity05(void);
	void OnLBDownBtnCommodity06(void);
	void OnLBDownBtnCommodity07(void);
	void OnLBDownBtnCommodity08(void);
	void OnLBDownBtnCommodity09(void);
	void OnOwnerDrawDataArea(int);
	void OnOwnerDrawContribution(int);
	void OnInitBtnDescContribution(intptr_t);
	void OnLBDownBtnDescContribution(void);
	void OnInitBtnContribution(intptr_t);
	void OnLBDownBtnContribution(void);

private:
	float m_updateTimer;                   // 0x110, OnProc: seconds since the last update request
	FrButton* m_descButton;                // 0x114, OnInitDescButton
	FrPointEventDescDlg* m_descDlg;        // 0x118, OnPointEventDescDlgResult
	FrPointEventNoticeDlg* m_noticeDlg;    // 0x11c, Confirm
	unsigned long m_myPoint;               // 0x120, GetMyPoint/SetMyPoint
	unsigned short m_selectedCommodity;    // 0x124, 10 = none (UnSelectCommodity)
	FrButton* m_commodityButtons[10];      // 0x128, OnInitBtnCommodityNN ([9]: OnInitBtnContribution)
	CCommoditySlot m_slots[10];            // 0x150
	float m_contributionPoint;             // 0x628, OnOwnerDrawContribution: the contribution total
	FrButton* m_contributionDescButton;    // 0x62c, OnInitBtnDescContribution
	const Bitmap* m_contributionGauge;     // 0x630, "contribution_gage"
	const Bitmap* m_contributionGaugeBack; // 0x634, "contribution_gage_background"
	const Bitmap* m_contributionFrames[2]; // 0x638, "contribution_frame_01"/"_02"
	const Bitmap* m_successIcon;           // 0x640, "icon_success"
	UNKNOWN(0x644, 0x648);
};

CHECK_SIZE(FrPointEventDlg, 0x648);
BEGIN_LAYOUT_CHECK(FrPointEventDlg)
	CHECK_FIELD(FrPointEventDlg, m_updateTimer, 0x110);
	CHECK_FIELD(FrPointEventDlg, m_descButton, 0x114);
	CHECK_FIELD(FrPointEventDlg, m_descDlg, 0x118);
	CHECK_FIELD(FrPointEventDlg, m_noticeDlg, 0x11c);
	CHECK_FIELD(FrPointEventDlg, m_myPoint, 0x120);
	CHECK_FIELD(FrPointEventDlg, m_selectedCommodity, 0x124);
	CHECK_FIELD(FrPointEventDlg, m_commodityButtons, 0x128);
	CHECK_FIELD(FrPointEventDlg, m_slots, 0x150);
	CHECK_FIELD(FrPointEventDlg, m_contributionPoint, 0x628);
	CHECK_FIELD(FrPointEventDlg, m_contributionDescButton, 0x62c);
	CHECK_FIELD(FrPointEventDlg, m_contributionGauge, 0x630);
	CHECK_FIELD(FrPointEventDlg, m_contributionGaugeBack, 0x634);
	CHECK_FIELD(FrPointEventDlg, m_contributionFrames, 0x638);
	CHECK_FIELD(FrPointEventDlg, m_successIcon, 0x640);
END_LAYOUT_CHECK

extern FrPointEventDlg* g_pPointEventDlg;

#endif // PG_CLIENT_PROJECTG_POINTEVENTDLG_H
