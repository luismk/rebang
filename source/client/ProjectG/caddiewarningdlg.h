// Original: d:\build\custom\temp\code\client\projectg\caddiewarningdlg.h
#ifndef PG_CLIENT_PROJECTG_CADDIEWARNINGDLG_H
#define PG_CLIENT_PROJECTG_CADDIEWARNINGDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"

class FrArea;
class FrEdit;
class FrButton;
class FrStatic;

// Caddie warning dialog: warns that the player's caddie is on vacation (its
// contract ran out) and lets the player turn the pay-day warning off. Its
// size (0x150) is the `new` of FrCaddieWarningDlgMakeInstance.
class FrCaddieWarningDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // caddiewarningdlg.h:15

	FrCaddieWarningDlg(void);

	void SetCaddieInfo(const sCaddieInfo& info);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnPortraitInit(intptr_t param);
	void OnVacationInit(intptr_t param);
	void OnLimitInit(intptr_t param);
	void OnNameInit(intptr_t param);
	void OnMessageInit(intptr_t param);
	void OnOkInit(intptr_t param);
	void OnWarningButtonInit(intptr_t param);
	void OnWarningTextInit(intptr_t param);
	void OnOkBtnUp(void);

private:
	sCaddieInfo m_caddieInfo;          // 0x110 (SetCaddieInfo)
	UNKNOWN(0x129, 0x12c);
	FrArea* m_pPortrait;               // 0x12c (OnPortraitInit)
	FrArea* m_pVacation;               // 0x130 (OnVacationInit)
	FrArea* m_pLimit;                  // 0x134 (OnLimitInit)
	FrEdit* m_pName;                   // 0x138 (OnNameInit)
	FrEdit* m_pMessage;                // 0x13c (OnMessageInit)
	FrButton* m_pOk;                   // 0x140 (OnOkInit)
	FrButton* m_pWarningButton;        // 0x144 (OnWarningButtonInit)
	FrStatic* m_pWarningText;          // 0x148 (OnWarningTextInit)
	UNKNOWN(0x14c, 0x150);
};
BEGIN_LAYOUT_CHECK(FrCaddieWarningDlg)
	CHECK_FIELD(FrCaddieWarningDlg, m_caddieInfo, 0x110);
	CHECK_FIELD(FrCaddieWarningDlg, m_pPortrait, 0x12c);
	CHECK_FIELD(FrCaddieWarningDlg, m_pVacation, 0x130);
	CHECK_FIELD(FrCaddieWarningDlg, m_pLimit, 0x134);
	CHECK_FIELD(FrCaddieWarningDlg, m_pName, 0x138);
	CHECK_FIELD(FrCaddieWarningDlg, m_pMessage, 0x13c);
	CHECK_FIELD(FrCaddieWarningDlg, m_pOk, 0x140);
	CHECK_FIELD(FrCaddieWarningDlg, m_pWarningButton, 0x144);
	CHECK_FIELD(FrCaddieWarningDlg, m_pWarningText, 0x148);
END_LAYOUT_CHECK
CHECK_SIZE(FrCaddieWarningDlg, 0x150);

#endif // PG_CLIENT_PROJECTG_CADDIEWARNINGDLG_H
