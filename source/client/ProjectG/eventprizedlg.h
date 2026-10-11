// Original: d:\build\custom\temp\code\client\projectg\eventprizedlg.h
#ifndef PG_CLIENT_PROJECTG_EVENTPRIZEDLG_H
#define PG_CLIENT_PROJECTG_EVENTPRIZEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrViewer;

// Event prize dialog; its size (0x120) is the `new` of
// FrEventPrizeDlgMakeInstance. The constructor is inline at
// eventprizedlg.h:11; its emitted copy (with the event accessors) is defined
// out of line in lobbymain.cpp.
class FrEventPrizeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // eventprizedlg.h:8

	FrEventPrizeDlg(void);                  // eventprizedlg.h:11

	void SetEvent(unsigned char event);     // eventprizedlg.h:13
	unsigned char GetEvent(void) const;     // eventprizedlg.h:14

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	void OnViewBtnUp(void);
	void OnViewInit(intptr_t);
	void OnCancelInit(intptr_t);

private:
	FrButton* m_cancelBtn;      // 0x110, OnCancelInit ("cancel")
	FrViewer* m_viewBtn;        // 0x114, OnViewInit ("view", the event image)
	unsigned char m_event;      // 0x118
	PADDING(0x119, 0x120);
};

CHECK_SIZE(FrEventPrizeDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrEventPrizeDlg)
	CHECK_FIELD(FrEventPrizeDlg, m_cancelBtn, 0x110);
	CHECK_FIELD(FrEventPrizeDlg, m_viewBtn, 0x114);
	CHECK_FIELD(FrEventPrizeDlg, m_event, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EVENTPRIZEDLG_H
