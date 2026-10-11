// Original: d:\build\custom\temp\code\client\projectg\reportdlg.h
#ifndef PG_CLIENT_PROJECTG_REPORTDLG_H
#define PG_CLIENT_PROJECTG_REPORTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrComboBox;
class FrEdit;
class FrEmoticonDlg;

// Player report dialog: the reported player's id (with an emoticon picker),
// a reason combo box and an OK button that writes the chat history and files
// the report. Its size (0x120) is the `new` of FrReportDlgMakeInstance.
class FrReportDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // reportdlg.h:18

	FrReportDlg(void);
	virtual ~FrReportDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnOK(void);
	virtual void OnProc(const float dt);

	void OnIdInit(intptr_t param);
	bool OnIdEnterKey(int param);
	void OnEmoticonBtnUp(void);
	bool OnEmoticonResult(int ret, FrForm* pForm);
	void OnReasonInit(intptr_t param);
	void OnOKInit(intptr_t param);

private:
	FrEdit* m_pId;                   // 0x110 (OnIdInit, the reported player's id)
	FrComboBox* m_pReason;           // 0x114 (OnReasonInit)
	FrButton* m_pOK;                 // 0x118 (OnOKInit, enabled while an id is typed)
	FrEmoticonDlg* m_pEmoticonDlg;   // 0x11c (open emoticon picker)
};
BEGIN_LAYOUT_CHECK(FrReportDlg)
	CHECK_FIELD(FrReportDlg, m_pId, 0x110);
	CHECK_FIELD(FrReportDlg, m_pReason, 0x114);
	CHECK_FIELD(FrReportDlg, m_pOK, 0x118);
	CHECK_FIELD(FrReportDlg, m_pEmoticonDlg, 0x11c);
END_LAYOUT_CHECK
CHECK_SIZE(FrReportDlg, 0x120);

#endif // PG_CLIENT_PROJECTG_REPORTDLG_H
