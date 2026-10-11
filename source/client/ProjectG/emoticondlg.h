// Original: d:\build\custom\temp\code\client\projectg\emoticondlg.h
#ifndef PG_CLIENT_PROJECTG_EMOTICONDLG_H
#define PG_CLIENT_PROJECTG_EMOTICONDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;

// Emoticon picker: a list of every icon of the Fresh emoticon set; OK
// copies the chosen icon's code into m_szSelectedIcon, which the opener
// (FrOnelineReqDlg) reads through GetSelectedIcon. Size 0x158 (the `new`
// of FrEmoticonDlgMakeInstance).
class FrEmoticonDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // emoticondlg.h:16

	FrEmoticonDlg(void);
	virtual ~FrEmoticonDlg(void);

	// emoticondlg.h:22, emitted by golfrule.cpp
	const char* GetSelectedIcon(void) { return m_szSelectedIcon; }

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnOK(void);
	virtual bool OnInit(void);

	void OnListInit(intptr_t param);
	void OnListOwnerDraw(int param);
	void OnListBtnUp(void);

private:
	FrListBox* m_pList;              // 0x110, OnListInit
	char m_szSelectedIcon[0x40];     // 0x114, code of the chosen icon
	unsigned long m_dwOpenTime;      // 0x154, timeGetTime() at OnListInit
};

CHECK_SIZE(FrEmoticonDlg, 0x158);
BEGIN_LAYOUT_CHECK(FrEmoticonDlg)
	CHECK_FIELD(FrEmoticonDlg, m_pList, 0x110);
	CHECK_FIELD(FrEmoticonDlg, m_szSelectedIcon, 0x114);
	CHECK_FIELD(FrEmoticonDlg, m_dwOpenTime, 0x154);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_EMOTICONDLG_H
