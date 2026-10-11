// Original: d:\build\custom\temp\code\client\projectg\upgradecaddiedlg.h
#ifndef PG_CLIENT_PROJECTG_UPGRADECADDIEDLG_H
#define PG_CLIENT_PROJECTG_UPGRADECADDIEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrEdit;

// Caddie upgrade confirmation dialog: a message edit listing the pang cost and
// the five stat changes, "Yes" sends the upgrade request (packet 0xEC) for the
// caddie; its size (0x138) is the `new` of FrUpgradeCaddieDlgMakeInstance.
class FrUpgradeCaddieDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // upgradecaddiedlg.h:5

	FrUpgradeCaddieDlg(void);

	void SetUpgradeInfo(unsigned long caddieId, int* stats, int beforePang, int afterPang);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnMessageInit(intptr_t param);
	void OnYesBtnUp(void);
	void OnNoBtnUp(void);

private:
	FrEdit* m_pMessage;      // 0x110
	int m_stats[5];          // 0x114 (power, control, accuracy, spin, curve changes)
	unsigned long m_caddieId;   // 0x128 (sent by OnYesBtnUp)
	int m_value12c;          // 0x12c (only cleared by the constructor here)
	int m_value130;          // 0x130 (only cleared by the constructor here)
	UNKNOWN(0x134, 0x138);
};

CHECK_SIZE(FrUpgradeCaddieDlg, 0x138);
BEGIN_LAYOUT_CHECK(FrUpgradeCaddieDlg)
	CHECK_FIELD(FrUpgradeCaddieDlg, m_pMessage, 0x110);
	CHECK_FIELD(FrUpgradeCaddieDlg, m_stats, 0x114);
	CHECK_FIELD(FrUpgradeCaddieDlg, m_caddieId, 0x128);
	CHECK_FIELD(FrUpgradeCaddieDlg, m_value12c, 0x12c);
	CHECK_FIELD(FrUpgradeCaddieDlg, m_value130, 0x130);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_UPGRADECADDIEDLG_H
