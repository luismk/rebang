// Original: d:\build\custom\temp\code\client\projectg\iteminfodlg.cpp
#include <string.h>

#include "ItemInfoDlg.h"
#include "ObjectFactory.h"
#include "globalnetworkdefine.h"
#include "fresh.h"
#include "frarea.h"
#include "fredit.h"
#include "frgaugebar.h"
#include "frstatic.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "../client/Wangreal/include/bitmap.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "../shared/classdefine.h"

// iteminfodlg.cpp:19 (run-time type and factory registration of FrItemInfoDlg)
const WRTTI FrItemInfoDlg::m_RTTI("FrItemInfoDlg", &FrForm::m_RTTI);

IObject* FrItemInfoDlgMakeInstance(void) {
	return new FrItemInfoDlg;
}

struct __sFrItemInfoDlg {
	__sFrItemInfoDlg(void) {
		ObjectFactory().AddObjectFunctor(FrItemInfoDlgMakeInstance, "FrItemInfoDlg");
	}
};
static __sFrItemInfoDlg s_registerFrItemInfoDlg;

// iteminfodlg.cpp:21
const sFRESH_MSGMAP* FrItemInfoDlg::GetMessageMap(void) const {
	return &FrItemInfoDlg::_MsgMap;
}

sFRESH_MSGMAP FrItemInfoDlg::_MsgMap = { &FrForm::_MsgMap, &FrItemInfoDlg::_MsgEntries[0] };

sFRESH_ENTRY FrItemInfoDlg::_MsgEntries[] = {
	// iteminfodlg.cpp:23
	{ "portrait", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnPortraitInit },
	// iteminfodlg.cpp:24
	{ "portrait", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrItemInfoDlg::OnPortraitOwnerDraw },
	// iteminfodlg.cpp:25
	{ "name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnNameInit },
	// iteminfodlg.cpp:26
	{ "level", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnLevelInit },
	// iteminfodlg.cpp:27
	{ "static_power", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnPowerStaticInit },
	// iteminfodlg.cpp:28
	{ "static_control", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnControlStaticInit },
	// iteminfodlg.cpp:29
	{ "static_accuracy", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnAccuracyStaticInit },
	// iteminfodlg.cpp:30
	{ "static_spin", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnSpinStaticInit },
	// iteminfodlg.cpp:31
	{ "static_curve", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnCurveStaticInit },
	// iteminfodlg.cpp:32
	{ "epower1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnPowerEdit1Init },
	// iteminfodlg.cpp:33
	{ "econtrol1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnControlEdit1Init },
	// iteminfodlg.cpp:34
	{ "eaccuracy1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnAccuracyEdit1Init },
	// iteminfodlg.cpp:35
	{ "espin1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnSpinEdit1Init },
	// iteminfodlg.cpp:36
	{ "ecurve1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnCurveEdit1Init },
	// iteminfodlg.cpp:37
	{ "epower2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnPowerEdit2Init },
	// iteminfodlg.cpp:38
	{ "econtrol2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnControlEdit2Init },
	// iteminfodlg.cpp:39
	{ "eaccuracy2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnAccuracyEdit2Init },
	// iteminfodlg.cpp:40
	{ "espin2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnSpinEdit2Init },
	// iteminfodlg.cpp:41
	{ "ecurve2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnCurveEdit2Init },
	// iteminfodlg.cpp:42
	{ "powerbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnPowerBarInit },
	// iteminfodlg.cpp:43
	{ "controlbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnControlBarInit },
	// iteminfodlg.cpp:44
	{ "accuracybar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnAccuracyBarInit },
	// iteminfodlg.cpp:45
	{ "spinbar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnSpinBarInit },
	// iteminfodlg.cpp:46
	{ "curvebar", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrItemInfoDlg::OnCurveBarInit },

	// iteminfodlg.cpp:47
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// iteminfodlg.cpp:51
FrItemInfoDlg::FrItemInfoDlg(void)
	: m_pTradeItem(0),

	m_pPortrait(0),
	m_pName(0),
	m_pLevel(0) {

	memset(m_pAbilityBar, 0, sizeof(m_pAbilityBar));
	memset(m_pAbilityEdit1, 0, sizeof(m_pAbilityEdit1));
	memset(m_pAbilityEdit2, 0, sizeof(m_pAbilityEdit2));
	memset(m_pAbilityStatic, 0, sizeof(m_pAbilityStatic));

	m_pEnchantBitmap[0] = g_pFresh->GetBitmap("trade_enchant_nothing");
	m_pEnchantBitmap[1] = g_pFresh->GetBitmap("trade_enchant_upgrade");
}

// iteminfodlg.cpp:68
FrItemInfoDlg::~FrItemInfoDlg(void) {
}

// iteminfodlg.cpp:72
void FrItemInfoDlg::OnPortraitInit(intptr_t param) {
	m_pPortrait = DYNAMIC_CAST(FrArea, param);
}

// iteminfodlg.cpp:77
void FrItemInfoDlg::OnPortraitOwnerDraw(void) {
	DrawEnchantArrow();
}

// iteminfodlg.cpp:82
void FrItemInfoDlg::OnNameInit(intptr_t param) {
	m_pName = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:87
void FrItemInfoDlg::OnLevelInit(intptr_t param) {
	m_pLevel = DYNAMIC_CAST(FrArea, param);
}

// iteminfodlg.cpp:92
void FrItemInfoDlg::OnPowerStaticInit(intptr_t param) {
	m_pAbilityStatic[0] = DYNAMIC_CAST(FrStatic, param);
}

// iteminfodlg.cpp:97
void FrItemInfoDlg::OnControlStaticInit(intptr_t param) {
	m_pAbilityStatic[1] = DYNAMIC_CAST(FrStatic, param);
}

// iteminfodlg.cpp:102
void FrItemInfoDlg::OnAccuracyStaticInit(intptr_t param) {
	m_pAbilityStatic[2] = DYNAMIC_CAST(FrStatic, param);
}

// iteminfodlg.cpp:107
void FrItemInfoDlg::OnSpinStaticInit(intptr_t param) {
	m_pAbilityStatic[3] = DYNAMIC_CAST(FrStatic, param);
}

// iteminfodlg.cpp:112
void FrItemInfoDlg::OnCurveStaticInit(intptr_t param) {
	m_pAbilityStatic[4] = DYNAMIC_CAST(FrStatic, param);
}

// iteminfodlg.cpp:117
void FrItemInfoDlg::OnPowerEdit1Init(intptr_t param) {
	m_pAbilityEdit1[0] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:122
void FrItemInfoDlg::OnControlEdit1Init(intptr_t param) {
	m_pAbilityEdit1[1] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:127
void FrItemInfoDlg::OnAccuracyEdit1Init(intptr_t param) {
	m_pAbilityEdit1[2] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:132
void FrItemInfoDlg::OnSpinEdit1Init(intptr_t param) {
	m_pAbilityEdit1[3] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:137
void FrItemInfoDlg::OnCurveEdit1Init(intptr_t param) {
	m_pAbilityEdit1[4] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:142
void FrItemInfoDlg::OnPowerEdit2Init(intptr_t param) {
	m_pAbilityEdit2[0] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:147
void FrItemInfoDlg::OnControlEdit2Init(intptr_t param) {
	m_pAbilityEdit2[1] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:152
void FrItemInfoDlg::OnAccuracyEdit2Init(intptr_t param) {
	m_pAbilityEdit2[2] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:157
void FrItemInfoDlg::OnSpinEdit2Init(intptr_t param) {
	m_pAbilityEdit2[3] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:162
void FrItemInfoDlg::OnCurveEdit2Init(intptr_t param) {
	m_pAbilityEdit2[4] = DYNAMIC_CAST(FrEdit, param);
}

// iteminfodlg.cpp:167
void FrItemInfoDlg::OnPowerBarInit(intptr_t param) {
	m_pAbilityBar[0] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pAbilityBar[0]->SetRange(0, 50, 0);
}

// iteminfodlg.cpp:174
void FrItemInfoDlg::OnControlBarInit(intptr_t param) {
	m_pAbilityBar[1] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pAbilityBar[1]->SetRange(0, 30, 0);
}

// iteminfodlg.cpp:181
void FrItemInfoDlg::OnAccuracyBarInit(intptr_t param) {
	m_pAbilityBar[2] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pAbilityBar[2]->SetRange(0, 30, 0);
}

// iteminfodlg.cpp:187
void FrItemInfoDlg::OnSpinBarInit(intptr_t param) {
	m_pAbilityBar[3] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pAbilityBar[3]->SetRange(0, 30, 0);
}

// iteminfodlg.cpp:194
void FrItemInfoDlg::OnCurveBarInit(intptr_t param) {
	m_pAbilityBar[4] = DYNAMIC_CAST(FrGaugeBar, param);
	m_pAbilityBar[4]->SetRange(0, 30, 0);
}

// iteminfodlg.cpp:201
bool FrItemInfoDlg::OnInit(void) {
	SetControlsAbility(false);

	return true;
}

// iteminfodlg.cpp:359
void FrItemInfoDlg::SetLevelBar(void) {
	if (m_pTradeItem == 0) {
		return;
	}

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_pTradeItem->typeId);

	if (pClubSet == 0) {
		return;
	}

	for (int i = 0; i < 5; i++) {
		int maxValue = pClubSet->m_maxStat[i];
		int value = (short)m_pTradeItem->stats[i] + pClubSet->m_baseStat[i];

		if (m_pAbilityBar[i]) {
			m_pAbilityBar[i]->SetDestPos(value, false);
			m_pAbilityBar[i]->SetExpand(maxValue - value);
		}

		SetLevelNumber(i);
		SetEnchantNumber(i);
	}
}

// iteminfodlg.cpp:387
void FrItemInfoDlg::SetLevelNumber(int level) {
	if (m_pTradeItem == 0) {
		return;
	}

	if (m_pAbilityEdit1[level] == 0) {
		return;
	}

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_pTradeItem->typeId);

	if (pClubSet == 0) {
		return;
	}

	unsigned char value = (unsigned char)(pClubSet->m_baseStat[level] + m_pTradeItem->stats[level]);
	unsigned char maxValue = (unsigned char)pClubSet->m_maxStat[level];

	m_pAbilityEdit1[level]->SetLine(1, MakeStr("%d", value > maxValue ? maxValue : value), 0xFFFF0000, false, 0);
}

// iteminfodlg.cpp:408
void FrItemInfoDlg::SetEnchantNumber(int ability) {
	if (m_pTradeItem == 0) {
		return;
	}

	if (m_pAbilityEdit2[ability] == 0) {
		return;
	}

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_pTradeItem->typeId);

	if (pClubSet == 0) {
		return;
	}

	unsigned short maxValue = pClubSet->m_maxStat[ability];
	m_pAbilityEdit2[ability]->SetLine(1, MakeStr("%d/%d", (short)m_pTradeItem->stats[ability], maxValue - pClubSet->m_baseStat[ability]), 0xFF000000, false, 0);
}

// iteminfodlg.cpp:427
void FrItemInfoDlg::SetControlsAbility(bool bVisible) {
	// Show or hide the five ability rows.
	for (int i = 0; i < 5; i++) {
		if (m_pAbilityBar[i]) m_pAbilityBar[i]->SetVisible(bVisible);
		if (m_pAbilityEdit1[i]) m_pAbilityEdit1[i]->SetVisible(bVisible);
		if (m_pAbilityEdit2[i]) m_pAbilityEdit2[i]->SetVisible(bVisible);
		if (m_pAbilityStatic[i]) m_pAbilityStatic[i]->SetVisible(bVisible);
	}
}

// iteminfodlg.cpp:441
void FrItemInfoDlg::DrawEnchantArrow(void) {
	if (m_pTradeItem == 0) {
		return;
	}

	// Only a club set carries enchant levels.
	if ((m_pTradeItem->typeId & 0xFC000000) != 0x10000000) {
		return;
	}

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();

	if (pDevice == 0) {
		return;
	}

	float x = m_rect.x + 248.0f, y = m_rect.y + 229.0f;

	// One arrow per ability row, 17 pixels apart: the upgrade arrow for an
	// enchanted one.
	for (int i = 0, offsetY = 0; i < 5; i++, offsetY += 17) {
		int bUpgraded = m_pTradeItem->stats[i] != 0;

		if (m_pEnchantBitmap[bUpgraded] != 0) {
			pDevice->DrawTexture(m_pEnchantBitmap[bUpgraded], WRect(x, y + offsetY, m_pEnchantBitmap[bUpgraded]->Width(), m_pEnchantBitmap[bUpgraded]->Height()), 0xFFFFFFFF, 0);
		}
	}
}
