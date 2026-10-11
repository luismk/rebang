// Original: d:\build\custom\temp\code\client\projectg\enchantdlg.h
#ifndef PG_CLIENT_PROJECTG_ENCHANTDLG_H
#define PG_CLIENT_PROJECTG_ENCHANTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrButton;
class FrEdit;
class FrGaugeBar;

// Enchant dialog; its size (0x150) is the `new` of FrEnchantDlgMakeInstance.
class FrEnchantDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrEnchantDlg(void);

	virtual void OnProc(const float dt);

	void SetType(unsigned char type, unsigned char grade);
	void SetResultCode(unsigned char code);   // enchantdlg.h:24

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnTitleInit(intptr_t param);
	void OnBaseInit(intptr_t param);
	void OnYesInit(intptr_t param);
	void OnYesBtnUp(void);
	void OnNoInit(intptr_t param);
	void OnNoBtnUp(void);
	void OnOKInit(intptr_t param);
	void OnOKBtnUp(void);
	void OnGaugeBarInit(intptr_t param);
	void OnTextEditInit(intptr_t param);

private:
	FrButton* m_pYesBtn;              // 0x110 ("yes")
	FrButton* m_pNoBtn;               // 0x114 ("no")
	FrButton* m_pOKBtn;               // 0x118 ("ok")
	FrGaugeBar* m_pGaugeBar;          // 0x11c ("gauge")
	FrEdit* m_pTextEdit;              // 0x120 ("etext")
	FrArea* m_pTitle;                 // 0x124 ("title")
	FrArea* m_pBase;                  // 0x128 ("base")
	unsigned char m_type;             // 0x12c (SetType's first argument)
	unsigned char m_grade;            // 0x12d (SetType's second argument)
	UNKNOWN(0x12E, 0x130);
	int m_itemValue;                  // 0x130
	unsigned long m_gradeColor;       // 0x134
	const char* m_gradeName;          // 0x138
	UNKNOWN(0x13C, 0x140);
	__int64 m_price;                  // 0x140, the pang the enchant costs (0 for a downgrade)
	bool m_bStarted;                  // 0x148
	unsigned char m_resultCode;       // 0x149
	UNKNOWN(0x14A, 0x150);
};
BEGIN_LAYOUT_CHECK(FrEnchantDlg)
	CHECK_FIELD(FrEnchantDlg, m_pYesBtn, 0x110);
	CHECK_FIELD(FrEnchantDlg, m_pNoBtn, 0x114);
	CHECK_FIELD(FrEnchantDlg, m_pOKBtn, 0x118);
	CHECK_FIELD(FrEnchantDlg, m_pGaugeBar, 0x11c);
	CHECK_FIELD(FrEnchantDlg, m_pTextEdit, 0x120);
	CHECK_FIELD(FrEnchantDlg, m_pTitle, 0x124);
	CHECK_FIELD(FrEnchantDlg, m_pBase, 0x128);
	CHECK_FIELD(FrEnchantDlg, m_type, 0x12c);
	CHECK_FIELD(FrEnchantDlg, m_grade, 0x12d);
	CHECK_FIELD(FrEnchantDlg, m_itemValue, 0x130);
	CHECK_FIELD(FrEnchantDlg, m_gradeColor, 0x134);
	CHECK_FIELD(FrEnchantDlg, m_gradeName, 0x138);
	CHECK_FIELD(FrEnchantDlg, m_price, 0x140);
	CHECK_FIELD(FrEnchantDlg, m_bStarted, 0x148);
	CHECK_FIELD(FrEnchantDlg, m_resultCode, 0x149);
END_LAYOUT_CHECK
CHECK_SIZE(FrEnchantDlg, 0x150);

#endif // PG_CLIENT_PROJECTG_ENCHANTDLG_H
