// Original: d:\build\custom\temp\code\client\projectg\mascotinfodlg.h
#ifndef PG_CLIENT_PROJECTG_MASCOTINFODLG_H
#define PG_CLIENT_PROJECTG_MASCOTINFODLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "MascotInfo.h"

class FrArea;
class FrButton;
class FrEdit;
class FrStatic;
class WPuppet;


// Mascot information dialog of the "my room": portrait, vacation / limit
// marks, name and an edit box to change the mascot's message. Its size
// (0x198) is the `new` of FrMascotInfoDlgMakeInstance.
class FrMascotInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // mascotinfodlg.h:12

	FrMascotInfoDlg(void);

	void SetMascotInfo(const sMascotInfo& info, WPuppet* pPuppet);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnPortraitInit(intptr_t param);
	void OnVacationInit(intptr_t param);
	void OnLimitInit(intptr_t param);
	void OnNameInit(intptr_t param);
	void OnTextMessageInit(intptr_t param);
	void OnMascotMsgInit(intptr_t param);
	void OnOkInit(intptr_t param);
	void OnCancelInit(intptr_t param);
	void OnCancelBtnUp(void);
	bool OnChangeMsgConfirmDlgResult(int result, FrForm* pForm);
	void OnOkBtnUp(void);

private:
	sMascotInfo m_info;          // 0x110
	UNKNOWN(0x14f, 0x150);
	FrArea* m_pPortrait;         // 0x150 ("portrait")
	FrArea* m_pVacation;         // 0x154 ("vacation")
	FrArea* m_pLimit;            // 0x158 ("limit")
	FrEdit* m_pName;             // 0x15c ("name")
	FrStatic* m_pTextMessage;    // 0x160 ("text_message")
	FrEdit* m_pMascotMsg;        // 0x164 ("mascot_msg")
	FrButton* m_pOk;             // 0x168 ("ok")
	FrButton* m_pCancel;         // 0x16c ("cancel")
	FrForm* m_pConfirmDlg;       // 0x170 (cleared by OnChangeMsgConfirmDlgResult)
	std::string m_msg;           // 0x174 (message being edited)
	WPuppet* m_pPuppet;          // 0x190 (the mascot's model, for ChangeMascotMsgInTex)
	UNKNOWN(0x194, 0x198);
};

CHECK_SIZE(sMascotInfo, 0x3f);
CHECK_SIZE(FrMascotInfoDlg, 0x198);
BEGIN_LAYOUT_CHECK(FrMascotInfoDlg)
	CHECK_FIELD(sMascotInfo, typeId, 0x4);
	CHECK_FIELD(sMascotInfo, message, 0xd);
	CHECK_FIELD(sMascotInfo, remainHours, 0x2b);
	CHECK_FIELD(sMascotInfo, pcBang, 0x3e);
	CHECK_FIELD(FrMascotInfoDlg, m_info, 0x110);
	CHECK_FIELD(FrMascotInfoDlg, m_pPortrait, 0x150);
	CHECK_FIELD(FrMascotInfoDlg, m_pTextMessage, 0x160);
	CHECK_FIELD(FrMascotInfoDlg, m_pConfirmDlg, 0x170);
	CHECK_FIELD(FrMascotInfoDlg, m_msg, 0x174);
	CHECK_FIELD(FrMascotInfoDlg, m_pPuppet, 0x190);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MASCOTINFODLG_H
