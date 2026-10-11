// Original: d:\build\custom\temp\code\client\projectg\newrecorddlg.h
#ifndef PG_CLIENT_PROJECTG_NEWRECORDDLG_H
#define PG_CLIENT_PROJECTG_NEWRECORDDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;

// New course record dialog; its size (0x118) is the `new` of
// FrNewRecordDlgMakeInstance. Its constructor and destructor are the
// compiler's (no line marks; MakeInstance inlines the constructor).
class FrNewRecordDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // newrecorddlg.h:8

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCourseListOwnerDraw(int);
	void OnCourseListInit(intptr_t);

private:
	FrListBox* m_pCourseList;   // 0x110 ("courselist")
	PADDING(0x114, 0x118);
};

CHECK_SIZE(FrNewRecordDlg, 0x118);

BEGIN_LAYOUT_CHECK(FrNewRecordDlg)
	CHECK_FIELD(FrNewRecordDlg, m_pCourseList, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NEWRECORDDLG_H
