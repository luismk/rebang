// Original: d:\build\custom\temp\code\client\projectg\memodlg.h
#ifndef PG_CLIENT_PROJECTG_MEMODLG_H
#define PG_CLIENT_PROJECTG_MEMODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;

// Memo dialog: a one-line "memo" edit (defaulting to "Friend") and an "ok"
// button; its size (0x120) is the `new` of FrMemoDlgMakeInstance.
class FrMemoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // memodlg.h:8

	FrMemoDlg(void)                                                // memodlg.h:11
		: m_pMemo(0), m_elapsed(0), m_bFocused(false) {
	}

	const char* GetMemo(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnMemoInit(intptr_t param);
	void OnMemoLButtonDown(void);
	bool OnMemoEnterKey(int param);
	void OnOkBtnUp(void);
	virtual void OnProc(const float dt);

private:
	FrEdit* m_pMemo;                     // 0x110 ("memo")
	float m_elapsed;                     // 0x114 (time since creation)
	bool m_bFocused;                     // 0x118 (key focus given to "memo")
	UNKNOWN(0x119, 0x120);
};

CHECK_SIZE(FrMemoDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrMemoDlg)
	CHECK_FIELD(FrMemoDlg, m_pMemo, 0x110);
	CHECK_FIELD(FrMemoDlg, m_elapsed, 0x114);
	CHECK_FIELD(FrMemoDlg, m_bFocused, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MEMODLG_H
