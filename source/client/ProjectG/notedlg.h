// Original: d:\build\custom\temp\code\client\projectg\notedlg.h
#ifndef PG_CLIENT_PROJECTG_NOTEDLG_H
#define PG_CLIENT_PROJECTG_NOTEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrEdit;
class FrStatic;

// Note (mail) writing dialog: the receiver's nick, an optional "received"
// line, the note text and the "paid" area. Its size (0x138) is the `new` of
// FrNoteDlgMakeInstance; the inline constructor is emitted where the dialog
// is created (Avatar_MainUI).
class FrNoteDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // notedlg.h:8

	FrNoteDlg(void)                                                // notedlg.h:11
		: m_pNick(0), m_pNote(0), m_pNoteStatic(0), m_pNickStatic(0), m_pReceived(0),
		  m_pReceivedStatic(0), m_pPaid(0), m_elapsed(0), m_bFocused(false) {
	}

	void SetNick(const char* nick, const char* received);
	void SetID(const char* nick, const char* received);
	const char* GetNote(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnPaidInit(intptr_t param);
	void OnNoteInit(intptr_t param);
	void OnNoteLBtnDown(void);
	bool OnNoteEnterKey(int param);
	void OnNoteStaticInit(intptr_t param);
	void OnNickInit(intptr_t param);
	void OnNickStaticInit(intptr_t param);
	void OnReceivedInit(intptr_t param);
	void OnReceivedStaticInit(intptr_t param);
	void OnOKBtnUp(void);

private:
	FrEdit* m_pNick;                   // 0x110 (OnNickInit)
	FrEdit* m_pNote;                   // 0x114 (OnNoteInit, the note text)
	FrStatic* m_pNoteStatic;           // 0x118 (OnNoteStaticInit)
	FrStatic* m_pNickStatic;           // 0x11c (OnNickStaticInit)
	FrEdit* m_pReceived;               // 0x120 (OnReceivedInit)
	FrStatic* m_pReceivedStatic;       // 0x124 (OnReceivedStaticInit)
	FrArea* m_pPaid;                   // 0x128 (OnPaidInit)
	float m_elapsed;                   // 0x12c (OnProc)
	bool m_bFocused;                   // 0x130 (OnProc, the note got the key focus)
	UNKNOWN(0x131, 0x138);
};
BEGIN_LAYOUT_CHECK(FrNoteDlg)
	CHECK_FIELD(FrNoteDlg, m_pNick, 0x110);
	CHECK_FIELD(FrNoteDlg, m_pNote, 0x114);
	CHECK_FIELD(FrNoteDlg, m_pNoteStatic, 0x118);
	CHECK_FIELD(FrNoteDlg, m_pNickStatic, 0x11c);
	CHECK_FIELD(FrNoteDlg, m_pReceived, 0x120);
	CHECK_FIELD(FrNoteDlg, m_pReceivedStatic, 0x124);
	CHECK_FIELD(FrNoteDlg, m_pPaid, 0x128);
	CHECK_FIELD(FrNoteDlg, m_elapsed, 0x12c);
	CHECK_FIELD(FrNoteDlg, m_bFocused, 0x130);
END_LAYOUT_CHECK
CHECK_SIZE(FrNoteDlg, 0x138);

#endif // PG_CLIENT_PROJECTG_NOTEDLG_H
