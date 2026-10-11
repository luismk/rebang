// Original: d:\build\custom\temp\code\client\projectg\compounddlg.h
#ifndef PG_CLIENT_PROJECTG_COMPOUNDDLG_H
#define PG_CLIENT_PROJECTG_COMPOUNDDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"
#include "../shared/classdefine.h"

class FrArea;
class FrButton;
class FrEdit;
class FrGaugeBar;
class WTitleFont;

// Compound (item synthesis) dialog: shows the quest's requirements, runs a
// gauge bar while the server answers, then the resulting item. Its size
// (0x2b8) is the `new` of FrCompoundDlgMakeInstance.
class FrCompoundDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // compounddlg.h:8

	FrCompoundDlg(void);
	virtual ~FrCompoundDlg(void);

	void SetCompound(IFF_STRUCT::sQuest& quest);
	void SetCompoundRes(unsigned char res, sItemInfo& itemInfo);
	virtual void OnProc(const float dt);
	void SetDlgText(const char* text);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnYesInit(intptr_t param);
	void OnYesBtnUp(void);
	void OnNoInit(intptr_t param);
	void OnNoBtnUp(void);
	void OnOKInit(intptr_t param);
	void OnOKBtnUp(void);
	void OnGaugeBarInit(intptr_t param);
	void OnTextEditInit(intptr_t param);
	void OnCompResInit(intptr_t param);
	void OnCompResOwnerDraw(void);

private:
	enum eCompoundBtn {
		BTN_YES = 0,
		BTN_NO = 1,
		BTN_OK = 2
	};

	IFF_STRUCT::sQuest m_quest;     // 0x110, copy of the quest given to SetCompound
	sItemInfo m_resItem;            // 0x1e8 (item given by SetCompoundRes)
	FrArea* m_pCompRes;             // 0x290 ("compres" area, shows the result item)
	bool m_bShowRes;                // 0x294 (set once the result is shown)
	FrButton* m_pBtn[3];            // 0x298 (indexed by eCompoundBtn)
	FrGaugeBar* m_pGaugeBar;        // 0x2a4
	FrEdit* m_pTextEdit;            // 0x2a8
	unsigned char m_compoundRes;    // 0x2ac (result of SetCompoundRes, 0xff = none yet)
	UNKNOWN(0x2ad, 0x2b4);
	WTitleFont* m_pTitleFont;       // 0x2b4
};

CHECK_SIZE(FrCompoundDlg, 0x2b8);
BEGIN_LAYOUT_CHECK(FrCompoundDlg)
	CHECK_FIELD(FrCompoundDlg, m_quest, 0x110);
	CHECK_FIELD(FrCompoundDlg, m_resItem, 0x1e8);
	CHECK_FIELD(FrCompoundDlg, m_pCompRes, 0x290);
	CHECK_FIELD(FrCompoundDlg, m_bShowRes, 0x294);
	CHECK_FIELD(FrCompoundDlg, m_pBtn, 0x298);
	CHECK_FIELD(FrCompoundDlg, m_pGaugeBar, 0x2a4);
	CHECK_FIELD(FrCompoundDlg, m_pTextEdit, 0x2a8);
	CHECK_FIELD(FrCompoundDlg, m_compoundRes, 0x2ac);
	CHECK_FIELD(FrCompoundDlg, m_pTitleFont, 0x2b4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_COMPOUNDDLG_H
