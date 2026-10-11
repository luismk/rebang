// Original: d:\build\custom\temp\code\client\projectg\dlgquickstartsetting.h
#ifndef PG_CLIENT_PROJECTG_DLGQUICKSTARTSETTING_H
#define PG_CLIENT_PROJECTG_DLGQUICKSTARTSETTING_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "singleton.h"
#include <string>
#include <vector>

class FrArea;
class FrButton;
class FrComboBox;

// Result of a quick start request: a message and an "ok1" button; its size
// (0x118) is the `new` of FrQuickResultDlgMakeInstance.
class FrQuickResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // dlgquickstartsetting.h:31

	FrQuickResultDlg(void);
	virtual ~FrQuickResultDlg(void);

	void SetContents(const char* contents);
	void SetOkbtnEnable(bool bEnable);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnOkBtnInit(intptr_t param);
	void OnOkBtnUp(void);

private:
	FrButton* m_pOkBtn;                // 0x110 (OnOkBtnInit)
	UNKNOWN(0x114, 0x118);
};
BEGIN_LAYOUT_CHECK(FrQuickResultDlg)
	CHECK_FIELD(FrQuickResultDlg, m_pOkBtn, 0x110);
END_LAYOUT_CHECK
CHECK_SIZE(FrQuickResultDlg, 0x118);

class FrQuickStartSettingDlg;

// Quick start dialog: one button per game mode (stroke, tournament, battle,
// chat), each with a "stop" area, and the option button opening the
// settings dialog; its size (0x138) is the `new` of FrQuickStartDlgMakeInstance.
class FrQuickStartDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // dlgquickstartsetting.h:56

	FrQuickStartDlg(void);
	virtual ~FrQuickStartDlg(void);

	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void SendRequestMatching(unsigned char code);

	void OnStrokeBtnInit(intptr_t param);
	void OnStrokeBtnUp(void);
	void OnStrokeStopInit(intptr_t param);
	void OnTournamentBtnInit(intptr_t param);
	void OnTournamentBtnUp(void);
	void OnTournamenStopInit(intptr_t param);
	void OnBattleBtnInit(intptr_t param);
	void OnBattleBtnUp(void);
	void OnBattleStopInit(intptr_t param);
	void OnChatBtnInit(intptr_t param);
	void OnChatBtnUp(void);
	void OnChatStopInit(intptr_t param);
	void OnOptionBtnInit(intptr_t param);
	void OnOptionBtnUp(void);
	bool OnQuickStartSettingDlgResult(int ret, FrForm* pForm);

private:
	FrButton* m_pStrokeBtn;                    // 0x110 (OnStrokeBtnInit)
	FrButton* m_pTournamentBtn;                // 0x114 (OnTournamentBtnInit)
	FrButton* m_pBattleBtn;                    // 0x118 (OnBattleBtnInit)
	FrButton* m_pChatBtn;                      // 0x11c (OnChatBtnInit)
	FrButton* m_pOptionBtn;                    // 0x120 (OnOptionBtnInit)
	FrArea* m_pStrokeStop;                     // 0x124 (OnStrokeStopInit)
	FrArea* m_pTournamentStop;                 // 0x128 (OnTournamenStopInit)
	FrArea* m_pBattleStop;                     // 0x12c (OnBattleStopInit)
	FrArea* m_pChatStop;                       // 0x130 (OnChatStopInit)
	FrQuickStartSettingDlg* m_pSettingDlg;     // 0x134 (OnOptionBtnUp)
};
BEGIN_LAYOUT_CHECK(FrQuickStartDlg)
	CHECK_FIELD(FrQuickStartDlg, m_pStrokeBtn, 0x110);
	CHECK_FIELD(FrQuickStartDlg, m_pTournamentBtn, 0x114);
	CHECK_FIELD(FrQuickStartDlg, m_pBattleBtn, 0x118);
	CHECK_FIELD(FrQuickStartDlg, m_pChatBtn, 0x11c);
	CHECK_FIELD(FrQuickStartDlg, m_pOptionBtn, 0x120);
	CHECK_FIELD(FrQuickStartDlg, m_pStrokeStop, 0x124);
	CHECK_FIELD(FrQuickStartDlg, m_pTournamentStop, 0x128);
	CHECK_FIELD(FrQuickStartDlg, m_pBattleStop, 0x12c);
	CHECK_FIELD(FrQuickStartDlg, m_pChatStop, 0x130);
	CHECK_FIELD(FrQuickStartDlg, m_pSettingDlg, 0x134);
END_LAYOUT_CHECK
CHECK_SIZE(FrQuickStartDlg, 0x138);

// Quick start settings: one check button per game mode and three interest
// combo boxes; m_checked is indexed by the matching code (stroke 0, chat 2,
// tournament 4, battle 10). Its size (0x170) is the `new` of
// FrQuickStartSettingDlgMakeInstance.
class FrQuickStartSettingDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // dlgquickstartsetting.h:113

	FrQuickStartSettingDlg(void);
	virtual ~FrQuickStartSettingDlg(void);

	void UpdateData(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnChkStrokeBtnInit(intptr_t param);
	void OnChkStrokeBtnUp(void);
	void OnChkTournamentBtnInit(intptr_t param);
	void OnChkTournamentBtnUp(void);
	void OnChkBattleBtnInit(intptr_t param);
	void OnChkBattleBtnUp(void);
	void OnChkChatBtnInit(intptr_t param);
	void OnChkChatBtnUp(void);
	void OnCmbCondition1BoxInit(intptr_t param);
	void OnCmbCondition2BoxInit(intptr_t param);
	void OnCmbCondition3BoxInit(intptr_t param);

private:
	FrButton* m_pChkStrokeBtn;                 // 0x110 (OnChkStrokeBtnInit)
	FrButton* m_pChkTournamentBtn;             // 0x114 (OnChkTournamentBtnInit)
	FrButton* m_pChkBattleBtn;                 // 0x118 (OnChkBattleBtnInit)
	FrButton* m_pChkChatBtn;                   // 0x11c (OnChkChatBtnInit)
	FrComboBox* m_pCmbCondition1;              // 0x120 (OnCmbCondition1BoxInit)
	FrComboBox* m_pCmbCondition2;              // 0x124 (OnCmbCondition2BoxInit)
	FrComboBox* m_pCmbCondition3;              // 0x128 (OnCmbCondition3BoxInit)
	int m_checked[16];                         // 0x12c (by matching code)
	UNKNOWN(0x16c, 0x170);
};
BEGIN_LAYOUT_CHECK(FrQuickStartSettingDlg)
	CHECK_FIELD(FrQuickStartSettingDlg, m_pChkStrokeBtn, 0x110);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pChkTournamentBtn, 0x114);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pChkBattleBtn, 0x118);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pChkChatBtn, 0x11c);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pCmbCondition1, 0x120);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pCmbCondition2, 0x124);
	CHECK_FIELD(FrQuickStartSettingDlg, m_pCmbCondition3, 0x128);
	CHECK_FIELD(FrQuickStartSettingDlg, m_checked, 0x12c);
END_LAYOUT_CHECK
CHECK_SIZE(FrQuickStartSettingDlg, 0x170);

// An interest of the matching system: its group, code and display name
// (0x43 bytes, the stride of CMatchingSystem's display list; the code is
// compared at 0x21, the name at 0x22).
struct sIntrest {
	char group[0x21];                          // 0x0
	unsigned char code;                        // 0x21
	char name[0x21];                           // 0x22
};
CHECK_SIZE(sIntrest, 0x43);

// Quick start matching: the interests offered for display and the codes the
// local player chose (WSingleton, 0x48 bytes).
class CMatchingSystem : public WSingleton<CMatchingSystem> {
	LAYOUT_CHECK_FRIEND;
	friend class FrQuickStartDlg;   // OnInit reads m_myIntrests directly

public:
	CMatchingSystem(void);
	virtual ~CMatchingSystem(void);

	const char* GetGroup(unsigned char code);
	const sIntrest* GetIntrestInfo(unsigned char code);
	unsigned char GetDisplayIntrestCode(const std::string& group, const std::string& name);
	int CheckIntrestCode(unsigned char code);
	std::string GetNameChecked(const std::string& group);
	// dlgquickstartsetting.h:179
	void ClearMyIntrests(void) { m_myIntrests.clear(); }
	void Clear(void);
	int SetMyIntrest(unsigned char code);
	int SetDisplayIntrest(const sIntrest& intrest);
	int GetDisplayIntrests(const std::string& group, std::vector<sIntrest>* pIntrests);

private:
	std::vector<sIntrest> m_displayIntrests;   // 0x28 (SetDisplayIntrest)
	std::vector<unsigned char> m_myIntrests;   // 0x38 (SetMyIntrest)
};
BEGIN_LAYOUT_CHECK(CMatchingSystem)
	CHECK_FIELD(CMatchingSystem, m_displayIntrests, 0x28);
	CHECK_FIELD(CMatchingSystem, m_myIntrests, 0x38);
END_LAYOUT_CHECK
CHECK_SIZE(CMatchingSystem, 0x48);

#endif // PG_CLIENT_PROJECTG_DLGQUICKSTARTSETTING_H
