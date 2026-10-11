// Original: d:\build\custom\temp\code\client\projectg\helpdlg.h
#ifndef PG_CLIENT_PROJECTG_HELPDLG_H
#define PG_CLIENT_PROJECTG_HELPDLG_H

#include "..\..\defines.h"
#include "frform.h"

class FrArea;
class FrViewer;

// Help dialog: a title area and an image viewer whose picture depends on
// m_helpType; its size (0x120) is the `new` of FrHelpDlgMakeInstance.
class FrHelpDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // helpdlg.h:8

	FrHelpDlg(void) : m_pView(0), m_pTitle(0), m_helpType(0) {}   // helpdlg.h:11

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnTitleInit(intptr_t param);
	void OnViewInit(intptr_t param);
	void OnViewBtnUp(void);

private:
	FrViewer* m_pView;   // 0x110 ("view")
	FrArea* m_pTitle;    // 0x114 ("title")
	int m_helpType;      // 0x118 (0: caddie upgrade, 2: quest, ...); 0x11c: tail padding to FrForm's 8-byte alignment

};

CHECK_SIZE(FrHelpDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrHelpDlg)
	CHECK_FIELD(FrHelpDlg, m_pView, 0x110);
	CHECK_FIELD(FrHelpDlg, m_pTitle, 0x114);
	CHECK_FIELD(FrHelpDlg, m_helpType, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_HELPDLG_H
