// Original: d:\build\custom\temp\code\client\projectg\uccsetinfodlg.h
#ifndef PG_CLIENT_PROJECTG_UCCSETINFODLG_H
#define PG_CLIENT_PROJECTG_UCCSETINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "bitmap.h"

class FrArea;
class FrButton;
class FrEdit;
class FrUccDrawDlg;

// Names a finished UCC (self-designed item) drawing: a name edit box and the
// item's thumbnail. Size 0x138 (the `new` of FrUccSetInfoDlgMakeInstance).
class FrUccSetInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // uccsetinfodlg.h:10

	FrUccSetInfoDlg(void);

	void SetParent(FrUccDrawDlg* pParent);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnNameInit(intptr_t param);
	bool OnNameEnterKey(int param);
	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);
	void OnIconAreaInit(intptr_t param);
	void OnIconAreaOwnerDraw(int param);

	FrEdit* m_pName;              // 0x110
	FrButton* m_pOkBtn;           // 0x114
	FrButton* m_pCancelBtn;       // 0x118
	FrArea* m_pIconArea;          // 0x11c
	FrUccDrawDlg* m_pParent;      // 0x120
	Bitmap m_icon;                // 0x124, 64x84 thumbnail of the parent's canvas
};

CHECK_SIZE(FrUccSetInfoDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrUccSetInfoDlg)
	CHECK_FIELD(FrUccSetInfoDlg, m_pName, 0x110);
	CHECK_FIELD(FrUccSetInfoDlg, m_pOkBtn, 0x114);
	CHECK_FIELD(FrUccSetInfoDlg, m_pCancelBtn, 0x118);
	CHECK_FIELD(FrUccSetInfoDlg, m_pIconArea, 0x11c);
	CHECK_FIELD(FrUccSetInfoDlg, m_pParent, 0x120);
	CHECK_FIELD(FrUccSetInfoDlg, m_icon, 0x124);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UCCSETINFODLG_H
