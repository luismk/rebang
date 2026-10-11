// Original: d:\build\custom\temp\code\client\projectg\frticketexchangedlg.h
#ifndef PG_CLIENT_PROJECTG_FRTICKETEXCHANGEDLG_H
#define PG_CLIENT_PROJECTG_FRTICKETEXCHANGEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include <map>

#include "frform.h"
#include "singleton.h"

class FrButton;
class FrEdit;
class FrViewer;

// Ticket id helpers: a TID is a 16-bit group (high word) and index (low word).
namespace nsTicketHelper {
	unsigned long Combine(unsigned long group, unsigned long index);
	void Analyze(unsigned long& tid, unsigned long& group, unsigned long& index);
}

// Exchange counts received from the server (size 0x3c, the `new` of the
// FrTicketExchangeDlg constructor), a singleton.
class TicketInfo : public WSingleton<TicketInfo> {
	LAYOUT_CHECK_FRIEND;
public:
	// Exchange count of one ticket (the value type of the map, 8 bytes).
	struct sTicketItem {
		unsigned long m_count;      // 0x0
		unsigned long m_maxCount;   // 0x4
	};

	TicketInfo(void);
	virtual ~TicketInfo(void);

	sTicketItem FindCount(unsigned long tid);

	unsigned long m_ticketCount;                        // 0x28
	char m_groupGState;                                 // 0x2c, -1 until the server answers
	std::map<unsigned long, sTicketItem> m_counts;      // 0x30, by TID
};

CHECK_SIZE(TicketInfo, 0x3c);
BEGIN_LAYOUT_CHECK(TicketInfo)
	CHECK_FIELD(TicketInfo, m_ticketCount, 0x28);
	CHECK_FIELD(TicketInfo, m_groupGState, 0x2c);
	CHECK_FIELD(TicketInfo, m_counts, 0x30);
END_LAYOUT_CHECK

// frticketexchangedlg.h:21
inline TicketInfo::TicketInfo(void) {
	m_ticketCount = 0;
	m_groupGState = -1;
}

// frticketexchangedlg.h:27
inline TicketInfo::~TicketInfo(void) {
	m_counts.clear();
}

// One exchangeable item of the dialog with its controls (0x1c bytes).
class TicketItemObject {
	LAYOUT_CHECK_FRIEND;
public:
	TicketItemObject(void);
	~TicketItemObject(void);

	void set_TID(unsigned long tid);
	unsigned long get_TID(void);
	void set_Quantity(unsigned long quantity);
	unsigned long get_Quantity(void);
	FrButton* get_Button(int wnd);
	FrEdit* get_Edit(unsigned int index, int wnd);
	FrViewer* get_Viewer(int wnd, bool over);

private:
	unsigned long m_TID;          // 0x0
	unsigned long m_Quantity;     // 0x4
	FrButton* m_pButton;          // 0x8
	FrEdit* m_pEdit[2];           // 0xc
	FrViewer* m_pViewer;          // 0x14
	FrViewer* m_pViewerOver;      // 0x18
};

CHECK_SIZE(TicketItemObject, 0x1c);
BEGIN_LAYOUT_CHECK(TicketItemObject)
	CHECK_FIELD(TicketItemObject, m_Quantity, 0x4);
	CHECK_FIELD(TicketItemObject, m_pButton, 0x8);
	CHECK_FIELD(TicketItemObject, m_pEdit, 0xc);
	CHECK_FIELD(TicketItemObject, m_pViewer, 0x14);
	CHECK_FIELD(TicketItemObject, m_pViewerOver, 0x18);
END_LAYOUT_CHECK

// Description pop-up of the ticket exchange dialog (size 0x158, the `new` of
// FrTicketExchangeDescDlgMakeInstance).
class FrTicketExchangeDescDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frticketexchangedlg.h:126

	FrTicketExchangeDescDlg(void);
	virtual ~FrTicketExchangeDescDlg(void);

	void SetDescImage(const char* name);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescViewer(intptr_t wnd);

	FrViewer* m_pDescViewer;          // 0x110
	char m_descImage[0x40];           // 0x114
	PADDING(0x154, 0x158);            // FrTicketExchangeDescDlg members
};

CHECK_SIZE(FrTicketExchangeDescDlg, 0x158);
BEGIN_LAYOUT_CHECK(FrTicketExchangeDescDlg)
	CHECK_FIELD(FrTicketExchangeDescDlg, m_pDescViewer, 0x110);
	CHECK_FIELD(FrTicketExchangeDescDlg, m_descImage, 0x114);
END_LAYOUT_CHECK

// Ticket exchange dialog (size 0x130, the `new` of
// FrTicketExchangeDlgMakeInstance). Only the members the written functions
// use are named.
class FrTicketExchangeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frticketexchangedlg.h:147

	FrTicketExchangeDlg(void);
	virtual ~FrTicketExchangeDlg(void);

	bool OnTicketExchangeDescResult(int ret, FrForm* pForm);
	virtual bool OnInit(void);
	void Reponse(unsigned long group, unsigned char result);
	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void onInit_ItemE0(int wnd);
	void onLBtnUp_ItemE0(void);
	void onInit_Edit0E0(int wnd);
	void onInit_Edit1E0(int wnd);
	void onInit_ItemE1(int wnd);
	void onLBtnUp_ItemE1(void);
	void onInit_Edit0E1(int wnd);
	void onInit_Edit1E1(int wnd);
	void onInit_ItemG0(int wnd);
	void onLBtnUp_ItemG0(void);
	void onInit_Edit0G0(int wnd);
	void onInit_ItemG1(int wnd);
	void onLBtnUp_ItemG1(void);
	void onInit_Edit0G1(int wnd);
	void onInit_ItemG2(int wnd);
	void onLBtnUp_ItemG2(void);
	void onInit_Edit0G2(int wnd);
	void onInit_ItemG3(int wnd);
	void onLBtnUp_ItemG3(void);
	void onInit_Edit0G3(int wnd);
	void onInit_ItemI0(int wnd);
	void onLBtnUp_ItemI0(void);
	void onInit_Item_I1(int wnd);
	void onLBtnUp_ItemI1(void);
	void onInit_Number_0(int wnd);
	void onInit_Number_1(int wnd);
	void onInit_Number_2(int wnd);
	void onInit_Number_3(int wnd);
	void onLBtnUp_Help(void);
	void onInit_Icon_0(int wnd);
	void onInit_Icon_1(int wnd);
	void onInit_Icon_2(int wnd);
	void onInit_Icon_3(int wnd);
	void onInit_Icon_4(int wnd);
	void onInit_Icon_5(int wnd);
	void onInit_Icon_6(int wnd);
	void onInit_Icon_7(int wnd);
	void onInit_View0(int wnd);
	void onInit_View1(int wnd);
	void onInit_View2(int wnd);
	void onInit_View3(int wnd);

private:
	bool Request(unsigned int group, unsigned long tid, unsigned int quantity);
	TicketItemObject* FindItem(unsigned long tid);
	void ChangeItemState(TicketItemObject* pItem, int state);
	void SetCountImage(unsigned int count);
	void ApplyInformation(void);

protected:
	std::map<unsigned long, TicketItemObject*> m_items;   // 0x110, by TID
	FrTicketExchangeDescDlg* m_pDescDlg;   // 0x11c, open description pop-up
	FrViewer* m_pNumber[4];           // 0x120
};

CHECK_SIZE(FrTicketExchangeDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrTicketExchangeDlg)
	CHECK_FIELD(FrTicketExchangeDlg, m_items, 0x110);
	CHECK_FIELD(FrTicketExchangeDlg, m_pDescDlg, 0x11c);
	CHECK_FIELD(FrTicketExchangeDlg, m_pNumber, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FRTICKETEXCHANGEDLG_H
