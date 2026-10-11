// Original: d:\build\custom\temp\code\client\projectg\mailboxdlg.h
#ifndef PG_CLIENT_PROJECTG_MAILBOXDLG_H
#define PG_CLIENT_PROJECTG_MAILBOXDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrListBox;
class FrNoteDlg;

struct sNoteInfo; // SharedDoc.h

// Mail box dialog: lists the received notes (CSharedDoc's note list) in the
// "note" list box; its size (0x120) is the `new` of
// FrMailBoxDlgMakeInstance. The constructor is inline (emitted by lobbymain).
class FrMailBoxDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // mailboxdlg.h:8

	FrMailBoxDlg(void) : m_noteList(0), m_pSelNote(0), m_pNoteDlg(0) {}   // mailboxdlg.h:11

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnNoteOwnerDraw(int);
	void OnNoteLBtnUp(void);
	bool OnNoteDlgResult(int, FrForm*);
	void OnNoteRBtnUp(void);
	void OnNoteInit(intptr_t);
	bool OnCloseDlgResult(int, FrForm*);
	void OnCancelBtnUp(void);

private:
	FrListBox* m_noteList;      // 0x110, OnNoteInit
	sNoteInfo* m_pSelNote;      // 0x114, the note last clicked (OnNoteLBtnUp)
	FrNoteDlg* m_pNoteDlg;      // 0x118, the reply dialog (OnNoteRBtnUp)
	PADDING(0x11C, 0x120);
};

CHECK_SIZE(FrMailBoxDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrMailBoxDlg)
	CHECK_FIELD(FrMailBoxDlg, m_noteList, 0x110);
	CHECK_FIELD(FrMailBoxDlg, m_pSelNote, 0x114);
	CHECK_FIELD(FrMailBoxDlg, m_pNoteDlg, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MAILBOXDLG_H
