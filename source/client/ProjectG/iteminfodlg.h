// Original: d:\build\custom\temp\code\client\projectg\iteminfodlg.h
#ifndef PG_CLIENT_PROJECTG_ITEMINFODLG_H
#define PG_CLIENT_PROJECTG_ITEMINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;
class FrEdit;
class FrGaugeBar;
class FrStatic;
class Bitmap;
struct sTradeItem;

// Item information dialog of the trade screen (portrait, name, level and
// the five ability rows); its size (0x178) is the `new` of
// FrItemInfoDlgMakeInstance.
class FrItemInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // iteminfodlg.h:8

	FrItemInfoDlg(void);
	virtual ~FrItemInfoDlg(void);

	void SetControlsAbility(bool bVisible);
	void SetLevelNumber(int level);
	void SetEnchantNumber(int ability);
	void DrawEnchantArrow(void);
	void SetLevelBar(void);
	void SetItemInfo(sTradeItem* pItem);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // iteminfodlg.cpp:21

	virtual bool OnInit(void);
	void OnPortraitInit(intptr_t param);
	void OnPortraitOwnerDraw(void);
	void OnNameInit(intptr_t param);
	void OnLevelInit(intptr_t param);
	void OnPowerStaticInit(intptr_t param);
	void OnControlStaticInit(intptr_t param);
	void OnAccuracyStaticInit(intptr_t param);
	void OnSpinStaticInit(intptr_t param);
	void OnCurveStaticInit(intptr_t param);
	void OnPowerEdit1Init(intptr_t param);
	void OnControlEdit1Init(intptr_t param);
	void OnAccuracyEdit1Init(intptr_t param);
	void OnSpinEdit1Init(intptr_t param);
	void OnCurveEdit1Init(intptr_t param);
	void OnPowerEdit2Init(intptr_t param);
	void OnControlEdit2Init(intptr_t param);
	void OnAccuracyEdit2Init(intptr_t param);
	void OnSpinEdit2Init(intptr_t param);
	void OnCurveEdit2Init(intptr_t param);
	void OnPowerBarInit(intptr_t param);
	void OnControlBarInit(intptr_t param);
	void OnAccuracyBarInit(intptr_t param);
	void OnSpinBarInit(intptr_t param);
	void OnCurveBarInit(intptr_t param);

	sTradeItem* m_pTradeItem;               // 0x110
	FrArea* m_pPortrait;                    // 0x114
	FrEdit* m_pName;                        // 0x118
	FrArea* m_pLevel;                       // 0x11c
	FrGaugeBar* m_pAbilityBar[5];           // 0x120, power/control/accuracy/spin/curve
	FrEdit* m_pAbilityEdit1[5];             // 0x134
	FrEdit* m_pAbilityEdit2[5];             // 0x148
	FrStatic* m_pAbilityStatic[5];          // 0x15c
	const Bitmap* m_pEnchantBitmap[2];      // 0x170, "trade_enchant_nothing" / "trade_enchant_upgrade"
};

CHECK_SIZE(FrItemInfoDlg, 0x178);
BEGIN_LAYOUT_CHECK(FrItemInfoDlg)
	CHECK_FIELD(FrItemInfoDlg, m_pTradeItem, 0x110);
	CHECK_FIELD(FrItemInfoDlg, m_pPortrait, 0x114);
	CHECK_FIELD(FrItemInfoDlg, m_pName, 0x118);
	CHECK_FIELD(FrItemInfoDlg, m_pLevel, 0x11c);
	CHECK_FIELD(FrItemInfoDlg, m_pAbilityBar, 0x120);
	CHECK_FIELD(FrItemInfoDlg, m_pAbilityEdit1, 0x134);
	CHECK_FIELD(FrItemInfoDlg, m_pAbilityEdit2, 0x148);
	CHECK_FIELD(FrItemInfoDlg, m_pAbilityStatic, 0x15c);
	CHECK_FIELD(FrItemInfoDlg, m_pEnchantBitmap, 0x170);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_ITEMINFODLG_H
