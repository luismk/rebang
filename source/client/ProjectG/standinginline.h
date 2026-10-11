// Original: d:\build\custom\temp\code\client\projectg\standinginline.h
#ifndef PG_CLIENT_PROJECTG_STANDINGINLINE_H
#define PG_CLIENT_PROJECTG_STANDINGINLINE_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;
class FrViewer;

// Number picker form: a "total" viewer and a list of the numbers 1-13; the
// chosen number is read back with GetSelectNum (0 = "total"). Its size
// (0x120) is the `new` of CStandingInLineMakeInstance.
class CStandingInLine : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // standinginline.h:53

	CStandingInLine(void);
	virtual ~CStandingInLine(void);

	int GetSelectNum(void) { return m_selectNum; }   // standinginline.h:82

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnView_TotalInit(intptr_t param);
	void OnView_TotalOwnerDraw(int param);
	void OnView_TotalButtonUp(void);
	void OnArrayListInit(intptr_t param);
	void OnArrayListOwnerDraw(intptr_t param);
	void OnArrayListBtnDown(void);
	void OnArrayListRBtnDown(void);

	FrViewer* m_pViewTotal;    // 0x110, the "total" viewer
	FrListBox* m_pArrayList;   // 0x114
	int m_selectNum;           // 0x118, chosen number (0 = total)
	PADDING(0x11c, 0x120);     // CStandingInLine members
};

CHECK_SIZE(CStandingInLine, 0x120);
BEGIN_LAYOUT_CHECK(CStandingInLine)
	CHECK_FIELD(CStandingInLine, m_pViewTotal, 0x110);
	CHECK_FIELD(CStandingInLine, m_pArrayList, 0x114);
	CHECK_FIELD(CStandingInLine, m_selectNum, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_STANDINGINLINE_H
