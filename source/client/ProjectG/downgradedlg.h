// Original: d:\build\custom\temp\code\client\projectg\downgradedlg.h
#ifndef PG_CLIENT_PROJECTG_DOWNGRADEDLG_H
#define PG_CLIENT_PROJECTG_DOWNGRADEDLG_H

#include "..\..\defines.h"
#include "frform.h"

class FrArea;
class FrButton;
class FrEdit;
class FrGaugeBar;
class FrHelpDlg;

// The ability upgrade / downgrade dialog of a character or a club: one row
// per stat (power, control, impact, spin, curve) with a gauge bar, two
// number edits and an up/down button, a title area and two help texts. Its
// size (0x190) is the `new` of FrDowngradeDlgMakeInstance.
class FrDowngradeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // downgradedlg.h:14

	FrDowngradeDlg(void);   // no destructor of its own: the implicit one only chains to FrForm

	void SetInfo(unsigned long typeId, char* name, unsigned long* pLevel, unsigned long* pEnchant, int bDowngrade);
	void SetInfo(unsigned long itemId, int bDowngrade);
	void SetLevelBar(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnTitleInit(intptr_t param);
	void OnHelpBtnInit(intptr_t param);
	void OnHelpBtnUp(void);
	bool OnHelpDlgResult(int ret, FrForm* pForm);
	void OnHelpEdit1Init(intptr_t param);
	void OnHelpEdit2Init(intptr_t param);
	void OnPowerBarInit(intptr_t param);
	void OnControlBarInit(intptr_t param);
	void OnImpactBarInit(intptr_t param);
	void OnSpinBarInit(intptr_t param);
	void OnCurveBarInit(intptr_t param);
	void OnPowerInit(intptr_t param);
	void OnControlInit(intptr_t param);
	void OnImpactInit(intptr_t param);
	void OnSpinInit(intptr_t param);
	void OnCurveInit(intptr_t param);
	void OnPowerBtnUp(void);
	void OnControlBtnUp(void);
	void OnImpactBtnUp(void);
	void OnSpinBtnUp(void);
	void OnCurveBtnUp(void);
	void OpenDowngrade(unsigned char stat);
	void SetLevelNumber(int stat);
	void SetEnchantNumber(int stat);
	void OnPowerEdit1Init(intptr_t param);
	void OnControlEdit1Init(intptr_t param);
	void OnImpactEdit1Init(intptr_t param);
	void OnSpinEdit1Init(intptr_t param);
	void OnCurveEdit1Init(intptr_t param);
	void OnPowerEdit2Init(intptr_t param);
	void OnControlEdit2Init(intptr_t param);
	void OnImpactEdit2Init(intptr_t param);
	void OnSpinEdit2Init(intptr_t param);
	void OnCurveEdit2Init(intptr_t param);

private:
	FrGaugeBar* m_pBar[5];          // 0x110 (powerbar, controlbar, impactbar, spinbar, curvebar)
	FrEdit* m_pEdit1[5];            // 0x124 (epower1 ... ecurve1)
	FrEdit* m_pEdit2[5];            // 0x138 (epower2 ... ecurve2)
	FrButton* m_pButton[5];         // 0x14c (power, control, impact, spin, curve)
	FrEdit* m_pHelp1;               // 0x160 ("help1")
	FrEdit* m_pHelp2;               // 0x164 ("help2")
	FrArea* m_pTitle;               // 0x168 ("title")
	FrButton* m_pHelpBtn;           // 0x16c ("help")
	FrHelpDlg* m_pHelpDlg;          // 0x170
	unsigned long m_itemId;         // 0x174 (club SetInfo)
	int m_bClub;                    // 0x178 (0: character, 1: club)
	int m_bDowngrade;               // 0x17c
	char* m_pName;                  // 0x180
	unsigned long* m_pLevel;        // 0x184
	unsigned long* m_pEnchant;      // 0x188
	unsigned long m_typeId;         // 0x18c
};

CHECK_SIZE(FrDowngradeDlg, 0x190);
BEGIN_LAYOUT_CHECK(FrDowngradeDlg)
	CHECK_FIELD(FrDowngradeDlg, m_pBar, 0x110);
	CHECK_FIELD(FrDowngradeDlg, m_pEdit1, 0x124);
	CHECK_FIELD(FrDowngradeDlg, m_pEdit2, 0x138);
	CHECK_FIELD(FrDowngradeDlg, m_pButton, 0x14c);
	CHECK_FIELD(FrDowngradeDlg, m_pHelp1, 0x160);
	CHECK_FIELD(FrDowngradeDlg, m_pHelp2, 0x164);
	CHECK_FIELD(FrDowngradeDlg, m_pTitle, 0x168);
	CHECK_FIELD(FrDowngradeDlg, m_pHelpBtn, 0x16c);
	CHECK_FIELD(FrDowngradeDlg, m_pHelpDlg, 0x170);
	CHECK_FIELD(FrDowngradeDlg, m_itemId, 0x174);
	CHECK_FIELD(FrDowngradeDlg, m_bClub, 0x178);
	CHECK_FIELD(FrDowngradeDlg, m_bDowngrade, 0x17c);
	CHECK_FIELD(FrDowngradeDlg, m_pName, 0x180);
	CHECK_FIELD(FrDowngradeDlg, m_pLevel, 0x184);
	CHECK_FIELD(FrDowngradeDlg, m_pEnchant, 0x188);
	CHECK_FIELD(FrDowngradeDlg, m_typeId, 0x18c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_DOWNGRADEDLG_H
