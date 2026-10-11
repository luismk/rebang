// Original: d:\build\custom\temp\code\client\projectg\resultdlg.h
#ifndef PG_CLIENT_PROJECTG_RESULTDLG_H
#define PG_CLIENT_PROJECTG_RESULTDLG_H

#include <windows.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "SharedDoc.h"

class Bitmap;
class CExhibition;
class CPartTidList;
class FrArea;
class FrAwardInfoDlg;
class FrButton;
class FrEdit;
class FrListBox;
class FrScoreDlg;
class FrGaugeBar;
class FrStatic;
class WOverlay;
class WTitleFont;

// Result dialog of a stroke game; its size (0x2f8) is the `new` of
// FrResultDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // resultdlg.h:29

	FrResultDlg(void);
	virtual ~FrResultDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;
	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void ResetVars(void);

	void OnMatchStaticInit(int param);
	void OnStageOwnerDraw(void);
	void OnScoreStaticInit(int param);
	void OnBonusInit(int param);
	void OnIdInit(intptr_t param);
	void OnIdOwnerDraw(int param);
	void OnBonusStaticInit(int param);
	void OnGetItemInit(int param);
	void OnGetItemOwnerDraw(void);
	void OnLevelInit(intptr_t param);
	void OnExpInit(int param);
	void OnExpGaugeBarInit(int param);
	void OnLevelUpInit(int param);
	void OnTrophyInit(int param);
	void OnScoreInit(int param);
	void OnPangInit(int param);
	void OnStageInit(int param);

	void FindWinner(void);
	void ShowPet(void);

	sRivalData m_winnerData;        // 0x110, the first-ranked player's result (FindWinner)
	int m_startLevel;               // 0x2a0 (OnProc shows m_pLevelUpArea when m_level passed it)
	int m_level;                    // 0x2a4 (OnProc: index into CSharedDoc::m_levelExp, raised on level up)
	int m_exp;                      // 0x2a8 (OnProc: the gauge's experience, in tenths)
	int m_expGain;                  // 0x2ac (OnProc: experience still to add to m_exp, signed)
	float m_elapsed;                // 0x2b0 (OnProc accumulates the frame time)
	bool m_bExpDone;                // 0x2b4 (OnProc: the gauge animation finished)
	PADDING(0x2b5, 0x2b8);
	FrArea* m_pStageArea;           // 0x2b8 (OnStageInit)
	FrArea* m_pLevelArea;           // 0x2bc (OnLevelInit)
	FrStatic* m_pExpStatic;         // 0x2c0 (OnExpInit)
	FrGaugeBar* m_pExpGaugeBar;     // 0x2c4 (OnExpGaugeBarInit)
	FrArea* m_pLevelUpArea;         // 0x2c8 (OnLevelUpInit)
	FrArea* m_pGetItemArea;         // 0x2cc (OnGetItemInit)
	FrArea* m_pIdArea;              // 0x2d0 (OnIdInit)
	CExhibition* m_pExhibition;     // 0x2d4 (the pet shown by ShowPet)
	CPartTidList* m_pPartTidList;   // 0x2d8 (its parts)
	WRect m_stageRect;              // 0x2dc (OnProc: m_pStageArea's rectangle the pet was last centred on)
	WTitleFont* m_pTitleFont;       // 0x2ec (created by the constructor)
	WOverlay* m_pHalloweenOverlay;  // 0x2f0 (z_Halloween_Male.tga, loaded by the constructor for local content 0x17)
	PADDING(0x2f4, 0x2f8);
};

CHECK_SIZE(FrResultDlg, 0x2f8);
BEGIN_LAYOUT_CHECK(FrResultDlg)
	CHECK_FIELD(FrResultDlg, m_winnerData, 0x110);
	CHECK_FIELD(FrResultDlg, m_pStageArea, 0x2b8);
	CHECK_FIELD(FrResultDlg, m_pLevelArea, 0x2bc);
	CHECK_FIELD(FrResultDlg, m_pExpStatic, 0x2c0);
	CHECK_FIELD(FrResultDlg, m_pExpGaugeBar, 0x2c4);
	CHECK_FIELD(FrResultDlg, m_pLevelUpArea, 0x2c8);
	CHECK_FIELD(FrResultDlg, m_pGetItemArea, 0x2cc);
	CHECK_FIELD(FrResultDlg, m_pIdArea, 0x2d0);
	CHECK_FIELD(FrResultDlg, m_pExhibition, 0x2d4);
	CHECK_FIELD(FrResultDlg, m_pPartTidList, 0x2d8);
	CHECK_FIELD(FrResultDlg, m_elapsed, 0x2b0);
	CHECK_FIELD(FrResultDlg, m_startLevel, 0x2a0);
	CHECK_FIELD(FrResultDlg, m_level, 0x2a4);
	CHECK_FIELD(FrResultDlg, m_exp, 0x2a8);
	CHECK_FIELD(FrResultDlg, m_expGain, 0x2ac);
	CHECK_FIELD(FrResultDlg, m_bExpDone, 0x2b4);
	CHECK_FIELD(FrResultDlg, m_stageRect, 0x2dc);
	CHECK_FIELD(FrResultDlg, m_pTitleFont, 0x2ec);
	CHECK_FIELD(FrResultDlg, m_pHalloweenOverlay, 0x2f0);
END_LAYOUT_CHECK

// Result dialog of a team game; its size (0x360) is the `new` of
// FrResultTeamDlgMakeInstance.
class FrResultTeamDlg : public FrResultDlg {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // resultdlg.h:103

	FrResultTeamDlg(void) {}            // resultdlg.h:106
	virtual ~FrResultTeamDlg(void) {}   // resultdlg.h:107

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;
	virtual bool OnInit(void);

	void OnIdInit(intptr_t param);
	void OnIdOwnerDraw(int param);
	void OnGetItemInit(int param);
	void OnGetItemOwnerDraw(void);
	void OnLevelInit(intptr_t param);
	void OnExpInit(int param);
	void OnExpGaugeBarInit(int param);
	void OnLevelUpInit(int param);
	void OnTrophyInit(int param);
	void OnScoreInit(int param);
	void OnPangInit(int param);

	void OnMatchTypeInit(int param);
	void OnBonusStaticInit(int param);
	void OnRankStaticInit(int param);
	void OnResultStaticInit(int param);
	void OnRedScoreInit(int param);
	void OnBlueScoreInit(int param);
	void OnRedPangInit(int param);
	void OnBluePangInit(int param);
	void OnStageInit(int param);
	void OnRedInit(int param);
	void OnBlueInit(int param);
	void OnRedOwnerDraw(void);
	void OnBlueOwnerDraw(void);

private:
	FrArea* m_pTeamIdArea;          // 0x2f8 (OnIdInit)
	FrArea* m_pRedArea;             // 0x2fc (OnRedInit)
	FrArea* m_pBlueArea;            // 0x300 (OnBlueInit)
	const Bitmap* m_pRedEmblem;     // 0x304 (OnRedInit: the red guild's emblem in a guild match)
	const Bitmap* m_pBlueEmblem;    // 0x308 (OnBlueInit: the blue guild's emblem in a guild match)
	const Bitmap* m_pTrophyBitmap;  // 0x30c (OnStageInit: the guild trophy when the guild won)
	sGuildMatchInfo m_guildMatch;   // 0x310, a copy of CSharedDoc::m_roomInfo's guild block (OnInit); the owner draws print its guild names
	PADDING(0x35a, 0x360);
};

CHECK_SIZE(FrResultTeamDlg, 0x360);
BEGIN_LAYOUT_CHECK(FrResultTeamDlg)
	CHECK_FIELD(FrResultTeamDlg, m_pTeamIdArea, 0x2f8);
	CHECK_FIELD(FrResultTeamDlg, m_pRedArea, 0x2fc);
	CHECK_FIELD(FrResultTeamDlg, m_pBlueArea, 0x300);
	CHECK_FIELD(FrResultTeamDlg, m_pRedEmblem, 0x304);
	CHECK_FIELD(FrResultTeamDlg, m_pBlueEmblem, 0x308);
	CHECK_FIELD(FrResultTeamDlg, m_pTrophyBitmap, 0x30c);
	CHECK_FIELD(FrResultTeamDlg, m_guildMatch, 0x310);
END_LAYOUT_CHECK

// Result dialog of the united (tournament/guild) games; its size (0x400) is
// the `new` of FrUniteResultDlgMakeInstance.
class FrUniteResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // resultdlg.h:176

	FrUniteResultDlg(void);
	virtual ~FrUniteResultDlg(void);

	virtual bool Close(bool bDestroy);

	void SetTikiReportState(SYSTEMTIME& time);
	bool CheckChildDlgVisible(void);   // resultdlg.cpp:3311

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	bool OnAwardInfoDlgResult(int ret, FrForm* pForm);
	bool OnScoreDlgResult(int ret, FrForm* pForm);
	bool OnTreasureScoreDlgResult(int ret, FrForm* pForm);
	bool OnTreasureGiftDlgResult(int ret, FrForm* pForm);
	void OnAwardInfoClick(void);
	void SetResultDlgType(void);
	bool SetTooltip(WRect rect, int index);

	void OnCancelButtonInit(int param);
	void OnAwardInfoInit(int param);
	void OnMyInfoAreaInit(int param);
	void OnExpGaugeBarInit(int param);
	void OnExpAreaInit(int param);
	void OnExpAreaOwnerDraw(int param);
	void OnRankListBoxInit(int param);
	void OnRankListLBtnUp(void);
	void OnRankListRBtnUp(void);
	void OnDescAreaInit(int param);
	void OnDescAreaOwnerDraw(int param);
	void OnAwardAreaInit(int param);
	void OnItemAreaInit(int param);
	void OnTeamResultInit(int param);
	void OnNextLevelTextInit(int param);
	void OnTooltipOwnerDraw(int param);

	void FindWinner(void);
	void SetPrizeResult(void);

private:
	std::map<unsigned long, int> m_prizeCountMap;   // 0x110, how many of each won item type (SetPrizeResult)
	FrButton* m_pCancelButton;      // 0x11c (OnCancelButtonInit)
	FrButton* m_pAwardInfoButton;   // 0x120 (OnAwardInfoInit)
	FrListBox* m_pRankListBox;      // 0x124 (OnRankListBoxInit)
	FrArea* m_pMyInfoArea;          // 0x128 (OnMyInfoAreaInit)
	FrAwardInfoDlg* m_pAwardInfoDlg;   // 0x12c (opened by OnAwardInfoClick, cleared by OnAwardInfoDlgResult)
	FrScoreDlg* m_pScoreDlg;        // 0x130 (cleared by OnScoreDlgResult; OnRankListRBtnUp shows the selected player)
	FrForm* m_pTreasureScoreDlg;    // 0x134 (cleared by OnTreasureScoreDlgResult; closed with the dialog)
	FrForm* m_pTreasureGiftDlg;     // 0x138 (cleared by OnTreasureGiftDlgResult; closed with the dialog)
	FrGaugeBar* m_pExpGaugeBar;     // 0x13c (OnExpGaugeBarInit)
	FrArea* m_pExpArea;             // 0x140 (OnExpAreaInit)
	FrArea* m_pDescArea;            // 0x144 (OnDescAreaInit)
	FrArea* m_pAwardArea;           // 0x148 (OnAwardAreaInit)
	FrArea* m_pItemArea;            // 0x14c (OnItemAreaInit)
	FrArea* m_pTeamResultArea;      // 0x150 (OnTeamResultInit)
	FrEdit* m_pNextLevelText;       // 0x154 (OnNextLevelTextInit)
	PADDING(0x158, 0x178);
	WTitleFont* m_pTitleFont1;      // 0x178 (from WResourceManager::GetTitleFont in the constructor; released by the destructor)
	PADDING(0x17c, 0x19c);
	WTitleFont* m_pTitleFont2;      // 0x19c (likewise)
	PADDING(0x1a0, 0x204);
	const Bitmap* m_pCurTooltip;    // 0x204 (SetTooltip: the entry of m_pTooltips under the mouse; OnTooltipOwnerDraw draws it)
	const Bitmap* m_pTooltips[7];   // 0x208 (SetTooltip's entries by index)
	int m_resultDlgType;            // 0x224 (SetResultDlgType: 0, 1 or 2 by the room type)
	PADDING(0x228, 0x22c);
	bool m_bLevelUp;                // 0x22c (OnExpAreaOwnerDraw draws font_levelup instead of the experience)
	PADDING(0x22d, 0x234);
	int m_exp;                      // 0x234 (OnExpAreaOwnerDraw prints it divided by 10)
	int m_nextLevelExp;             // 0x238 (likewise, after the slash)
	PADDING(0x23c, 0x240);
public:
	bool m_escClosable;             // 0x240 (CLobbyMain::ProcessLobbyUI: Escape closes the dialog only when set and no child dialog is shown)
private:
	PADDING(0x241, 0x248);
	sRivalData m_winnerData;        // 0x248, the first-ranked player's result (FindWinner; OnDescAreaOwnerDraw names the winner with it)
	unsigned long m_selectedUid;    // 0x3d8 (OnRankListRBtnUp: the right-clicked player)
	PADDING(0x3dc, 0x3e8);
	bool m_bTikiReport;             // 0x3e8 (SetTikiReportState, Tiki report content only)
	PADDING(0x3e9, 0x3ea);
	SYSTEMTIME m_tikiReportTime;    // 0x3ea (SetTikiReportState's time)
	unsigned char m_tikiReportRank; // 0x3fa (SetTikiReportState: the local player's index in the game results)
	bool m_bKeepTooltip[2];         // 0x3fb (SetTooltip keeps m_pCurTooltip while either is set)
	PADDING(0x3fd, 0x400);
};

CHECK_SIZE(FrUniteResultDlg, 0x400);
BEGIN_LAYOUT_CHECK(FrUniteResultDlg)
	CHECK_FIELD(FrUniteResultDlg, m_prizeCountMap, 0x110);
	CHECK_FIELD(FrUniteResultDlg, m_pCancelButton, 0x11c);
	CHECK_FIELD(FrUniteResultDlg, m_escClosable, 0x240);
	CHECK_FIELD(FrUniteResultDlg, m_pAwardInfoButton, 0x120);
	CHECK_FIELD(FrUniteResultDlg, m_pRankListBox, 0x124);
	CHECK_FIELD(FrUniteResultDlg, m_pMyInfoArea, 0x128);
	CHECK_FIELD(FrUniteResultDlg, m_pAwardInfoDlg, 0x12c);
	CHECK_FIELD(FrUniteResultDlg, m_pScoreDlg, 0x130);
	CHECK_FIELD(FrUniteResultDlg, m_pTreasureScoreDlg, 0x134);
	CHECK_FIELD(FrUniteResultDlg, m_pTreasureGiftDlg, 0x138);
	CHECK_FIELD(FrUniteResultDlg, m_pExpGaugeBar, 0x13c);
	CHECK_FIELD(FrUniteResultDlg, m_pExpArea, 0x140);
	CHECK_FIELD(FrUniteResultDlg, m_pDescArea, 0x144);
	CHECK_FIELD(FrUniteResultDlg, m_pAwardArea, 0x148);
	CHECK_FIELD(FrUniteResultDlg, m_pItemArea, 0x14c);
	CHECK_FIELD(FrUniteResultDlg, m_pTeamResultArea, 0x150);
	CHECK_FIELD(FrUniteResultDlg, m_pNextLevelText, 0x154);
	CHECK_FIELD(FrUniteResultDlg, m_pTitleFont1, 0x178);
	CHECK_FIELD(FrUniteResultDlg, m_pTitleFont2, 0x19c);
	CHECK_FIELD(FrUniteResultDlg, m_pCurTooltip, 0x204);
	CHECK_FIELD(FrUniteResultDlg, m_pTooltips, 0x208);
	CHECK_FIELD(FrUniteResultDlg, m_resultDlgType, 0x224);
	CHECK_FIELD(FrUniteResultDlg, m_bLevelUp, 0x22c);
	CHECK_FIELD(FrUniteResultDlg, m_exp, 0x234);
	CHECK_FIELD(FrUniteResultDlg, m_nextLevelExp, 0x238);
	CHECK_FIELD(FrUniteResultDlg, m_winnerData, 0x248);
	CHECK_FIELD(FrUniteResultDlg, m_selectedUid, 0x3d8);
	CHECK_FIELD(FrUniteResultDlg, m_bKeepTooltip, 0x3fb);
	CHECK_FIELD(FrUniteResultDlg, m_bTikiReport, 0x3e8);
	CHECK_FIELD(FrUniteResultDlg, m_tikiReportTime, 0x3ea);
	CHECK_FIELD(FrUniteResultDlg, m_tikiReportRank, 0x3fa);
END_LAYOUT_CHECK

// Award information dialog opened from FrUniteResultDlg; its size (0x118) is
// the `new` of FrAwardInfoDlgMakeInstance.
class FrAwardInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // resultdlg.h:359

	FrAwardInfoDlg(void);
	virtual ~FrAwardInfoDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

private:
	float m_elapsed;   // 0x110 (accumulated by OnProc)
	PADDING(0x114, 0x118);
};

CHECK_SIZE(FrAwardInfoDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrAwardInfoDlg)
	CHECK_FIELD(FrAwardInfoDlg, m_elapsed, 0x110);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_RESULTDLG_H
