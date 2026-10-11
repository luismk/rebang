// Original: d:\build\custom\temp\code\client\projectg\scoreboard.h
#ifndef PG_CLIENT_PROJECTG_SCOREBOARD_H
#define PG_CLIENT_PROJECTG_SCOREBOARD_H

#include <map>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "frgraphicinterface.h"
#include "actor.h"
#include "NoticeImage.h"

class Bitmap;
class CExhibition;
class CSimpleUI;
class FrFrame;
class FrLevelupItemForm;
class FrNextHole;
class FrScoreDlg;
class FrTreasureGauge;
class FrTreasureGift;
class FrTreasureScore;
class FrArea;
class FrButton;
class FrEdit;
class FrGaugeBar;
class FrListBox;
class FrStatic;
class WOverlay;
class WVector;
struct FrListItem;
struct sRivalData;

// Ranking dialog of the result screen; its size (0x168) is the `new` of
// FrRankFormMakeInstance. Only the members the transcribed functions use are
// declared.
class FrRankForm : public FrForm {
	friend class CScore;   // CScore::HandleMsg writes the board messages
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:30

	FrRankForm(void);

	void Align(void);
	void Resize(void);
	void SetCaptionType(unsigned char type);
	void SortRank(void);

protected:
	virtual void OnProc(const float dt);

protected:
	void OnRankLBtnDown(void);
	void OnRankRBtnUp(void);
	void OnRankDClick(void);
	void OnCenterBtnUp(void);
	void OnInfoBtnUp(void);
	void OnSizeBtnUp(void);

protected:
	void OnRankInit(intptr_t param);
	void OnRankOwnerDraw(intptr_t param);
	void OnBoardInit(intptr_t param);
	void OnBar01Init(intptr_t param);
	void OnBar02Init(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	FrListBox* m_pRankList;      // 0x110
	FrStatic* m_pBoard;          // 0x114 ("billboard")
	PADDING(0x118, 0x11c);
	unsigned char m_sizeType;    // 0x11c (cycles 0..2)
	unsigned char m_captionType; // 0x11d (SetCaptionType)
	PADDING(0x11e, 0x120);
	int m_rankIndex;             // 0x120
	int m_rankPage;              // 0x124
	unsigned long m_selectedUID; // 0x128
	unsigned char m_rankCount;   // 0x12c (CScore::HandleMsg 0x133)
	unsigned char m_boardMessage; // 0x12d (4 once OnProc has shown its message)
	unsigned char m_announcedHole; // 0x12e, the hole of the last "most played hole" message (CScore::HandleMsg 0x136)
	unsigned char m_scrollCount; // 0x12f, scrolls of caption 2 before caption 0 returns (OnProc)
	float m_captionTime;         // 0x130 (OnProc)
	char m_rankText[0x13];       // 0x134
	PADDING(0x147, 0x168);

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(FrRankForm, 0x168);
BEGIN_LAYOUT_CHECK(FrRankForm)
	CHECK_FIELD(FrRankForm, m_pRankList, 0x110);
	CHECK_FIELD(FrRankForm, m_pBoard, 0x114);
	CHECK_FIELD(FrRankForm, m_sizeType, 0x11c);
	CHECK_FIELD(FrRankForm, m_captionType, 0x11d);
	CHECK_FIELD(FrRankForm, m_boardMessage, 0x12d);
	CHECK_FIELD(FrRankForm, m_captionTime, 0x130);
	CHECK_FIELD(FrRankForm, m_rankIndex, 0x120);
	CHECK_FIELD(FrRankForm, m_rankPage, 0x124);
	CHECK_FIELD(FrRankForm, m_selectedUID, 0x128);
	CHECK_FIELD(FrRankForm, m_announcedHole, 0x12e);
	CHECK_FIELD(FrRankForm, m_scrollCount, 0x12f);
	CHECK_FIELD(FrRankForm, m_rankText, 0x134);
END_LAYOUT_CHECK

// Second ranking dialog (size 0x138, the `new` of FrRankForm2MakeInstance).
class FrRankForm2 : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:114

	FrRankForm2(void);

	void SortRank(void);

protected:
	void OnRankInit(intptr_t param);
	void OnRankOwnerDraw(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	void* m_pRankData;       // 0x110 (cleared by the constructor)
	PADDING(0x114, 0x138);   // FrRankForm2 members
};

CHECK_SIZE(FrRankForm2, 0x138);
BEGIN_LAYOUT_CHECK(FrRankForm2)
	CHECK_FIELD(FrRankForm2, m_pRankData, 0x110);
END_LAYOUT_CHECK

// Third ranking dialog (size 0x178, the `new` of FrRankForm3MakeInstance).
class FrRankForm3 : public FrForm {
	friend class CScore;   // CScore::HandleMsg writes the board message
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:151

	FrRankForm3(void);

	void SetScore(int score, int remain);
	void SetCaptionType(unsigned char type);

protected:
	void LoadBitmaps(void);

protected:
	void OnRankInit(intptr_t param);
	void OnRankOwnerDraw(intptr_t param);
	void OnSizeBtnUp(void);
	void OnBoardInit(intptr_t param);
	void OnScoreGaugeInit(intptr_t param);
	void OnScoreInfoInit(intptr_t param);
	void OnScoreInfoOwnerDraw(void);
	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	FrListBox* m_pRankList;      // 0x110 (OnRankInit)
	FrStatic* m_pBoard;          // 0x114 ("billboard")
	FrGaugeBar* m_pScoreGauge;   // 0x118 (OnScoreGaugeInit)
	FrArea* m_pScoreInfo;        // 0x11c (OnScoreInfoInit)
	unsigned char m_sizeType;    // 0x120 (OnSizeBtnUp toggles 0..1)
	unsigned char m_captionType; // 0x121 (SetCaptionType)
	int m_index124;          // 0x124
	unsigned char m_playerCount;   // 0x128, the players in the game (OnProc caption 0)
	bool m_bFlag129;         // 0x129
	unsigned char m_captionCycle;  // 0x12a, scrolls of caption 2 before caption 0 returns (OnProc)
	PADDING(0x12b, 0x12c);
	float m_captionTime;          // 0x12c (OnProc)
	const Bitmap* m_pRedEmblem;   // 0x130, the red guild's emblem
	const Bitmap* m_pBlueEmblem;  // 0x134, the blue guild's emblem
	PADDING(0x138, 0x13c);
	const Bitmap* m_pDigit[10];   // 0x13c, "SCORECARD" "0".."9"
	const Bitmap* m_pPlus;        // 0x164
	const Bitmap* m_pMinus;       // 0x168
	PADDING(0x16c, 0x170);
	int m_score;                  // 0x170 (SetScore)
	int m_scoreRemain;            // 0x174 (SetScore)

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(FrRankForm3, 0x178);
BEGIN_LAYOUT_CHECK(FrRankForm3)
	CHECK_FIELD(FrRankForm3, m_pRankList, 0x110);
	CHECK_FIELD(FrRankForm3, m_pBoard, 0x114);
	CHECK_FIELD(FrRankForm3, m_pScoreInfo, 0x11c);
	CHECK_FIELD(FrRankForm3, m_sizeType, 0x120);
	CHECK_FIELD(FrRankForm3, m_captionType, 0x121);
	CHECK_FIELD(FrRankForm3, m_captionTime, 0x12c);
	CHECK_FIELD(FrRankForm3, m_pRedEmblem, 0x130);
	CHECK_FIELD(FrRankForm3, m_pBlueEmblem, 0x134);
	CHECK_FIELD(FrRankForm3, m_index124, 0x124);
	CHECK_FIELD(FrRankForm3, m_bFlag129, 0x129);
	CHECK_FIELD(FrRankForm3, m_captionCycle, 0x12a);
	CHECK_FIELD(FrRankForm3, m_playerCount, 0x128);
	CHECK_FIELD(FrRankForm3, m_pDigit, 0x13c);
	CHECK_FIELD(FrRankForm3, m_pPlus, 0x164);
	CHECK_FIELD(FrRankForm3, m_pMinus, 0x168);
	CHECK_FIELD(FrRankForm3, m_pScoreGauge, 0x118);
	CHECK_FIELD(FrRankForm3, m_score, 0x170);
	CHECK_FIELD(FrRankForm3, m_scoreRemain, 0x174);
END_LAYOUT_CHECK

// Leader stage of FrRankFormEx: the rival list, the leader's exhibition and
// its stage area. The size is inferred from the constructor and from
// FrRankFormEx's layout.
class CRankExData {
	LAYOUT_CHECK_FRIEND;
public:
	CRankExData(void);
	~CRankExData(void);

	void InitAreaStage(FrArea* pArea);
	void Clear(void);
	void OnProc(float dt);
	void SetActive(bool bActive);
	bool IsMotionFinished(void);
	void ResetStage(void);
	void ClearRival(void);
	void DrawRankItem(FrListItem* pItem);
	void SetLeader(void);
	void ShowLeader(void);
	void DrawStageArea(void);
	void InitRivalData(void);
	void InitMedalData(void);
	void SetRivalData(void);
	void CreateLeaderPet(const WRect& rect);

	sRivalData* m_pRival;          // 0x0 (array)
	float m_motionLength;          // 0x4
	float m_motionTime;            // 0x8
	int m_leader;                  // 0xc
	int m_showLeader;              // 0x10
	CExhibition* m_pExhibition;    // 0x14
	FrListBox* m_pRankList;        // 0x18 (FrRankFormEx::OnRankInit)
	FrArea* m_pAreaStage;          // 0x1c
	FrStatic* m_pBoard;            // 0x20 (FrRankFormEx::OnBoardInit)
	const Bitmap* m_pRankGold;     // 0x24, InitMedalData: "rank_gold"
	const Bitmap* m_pRankSilver;   // 0x28, "rank_silver"
	const Bitmap* m_pRankBronze;   // 0x2c, "rank_bronze"
	const Bitmap* m_pRankNoAward;  // 0x30, "rank_noaward"
	const Bitmap* m_pRedTeamRank;  // 0x34, "30in_team_redteam_rank"
	const Bitmap* m_pBlueTeamRank; // 0x38, "30in_team_blueteam_rank"
	const Bitmap* m_pRank;         // 0x3c, "30in_rank"
	const Bitmap* m_pMassListIcon; // 0x40, "pc_icon_mass_list"
	const Bitmap* m_pZeroIcon;     // 0x44, "icon_zero"
};

CHECK_SIZE(CRankExData, 0x48);
BEGIN_LAYOUT_CHECK(CRankExData)
	CHECK_FIELD(CRankExData, m_pRival, 0x0);
	CHECK_FIELD(CRankExData, m_motionLength, 0x4);
	CHECK_FIELD(CRankExData, m_motionTime, 0x8);
	CHECK_FIELD(CRankExData, m_leader, 0xc);
	CHECK_FIELD(CRankExData, m_showLeader, 0x10);
	CHECK_FIELD(CRankExData, m_pExhibition, 0x14);
	CHECK_FIELD(CRankExData, m_pRankList, 0x18);
	CHECK_FIELD(CRankExData, m_pAreaStage, 0x1c);
	CHECK_FIELD(CRankExData, m_pBoard, 0x20);
	CHECK_FIELD(CRankExData, m_pRankGold, 0x24);
	CHECK_FIELD(CRankExData, m_pZeroIcon, 0x44);
END_LAYOUT_CHECK

// Ranking dialog with the leader stage (size 0x158, the `new` of
// FrRankFormExMakeInstance).
class FrRankFormEx : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:276

	FrRankFormEx(void);
	virtual ~FrRankFormEx(void);

	void Clear(void);
	void SortRank(void);
	sRivalData* GetRival(void);
	void SetActive(bool bActive);
	bool IsMotionFinished(void);
	void ResetStage(void);

protected:
	void OnRankOwnerDraw(intptr_t param);
	void SetLeader(void);
	void ShowLeader(void);
	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

protected:
	void OnStageAreaInit(intptr_t param);
	void OnStageAreaOwnerDraw(void);
	void OnRankInit(intptr_t param);
	void OnBoardInit(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	friend class CScore;   // CScore::UpdateRivals refills the rival data
	CRankExData m_rankEx;    // 0x110
};

CHECK_SIZE(FrRankFormEx, 0x158);
BEGIN_LAYOUT_CHECK(FrRankFormEx)
	CHECK_FIELD(FrRankFormEx, m_rankEx, 0x110);
END_LAYOUT_CHECK

// Award dialog of the result screen (size 0x208, the `new` of
// FrAwardFormMakeInstance).
class FrAwardForm : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:383

	FrAwardForm(void);
	virtual ~FrAwardForm(void);

	virtual bool Close(bool bDestroy);

protected:
	void CalcResult(void);
	void CalcResultMatch(void);
	virtual bool OnInit(void);
	virtual void OnProc(const float deltaTime);

protected:
	void OnTab0Init(int param);
	void OnTab1Init(int param);
	void OnNextLevelTextInit(int param);

protected:
	void OnUserListInit(intptr_t param);
	void OnUserListOwnerDraw(int param);
	void OnExpBar0Init(intptr_t param);
	void OnExpBar1Init(intptr_t param);
	void OnExpBar2Init(intptr_t param);
	void OnExpBar3Init(intptr_t param);
	void OnExp0Init(intptr_t param);
	void OnExp1Init(intptr_t param);
	void OnExp2Init(intptr_t param);
	void OnExp3Init(intptr_t param);
	void OnExpEnd0Init(intptr_t param);
	void OnExpEnd1Init(intptr_t param);
	void OnExpEnd2Init(intptr_t param);
	void OnExpEnd3Init(intptr_t param);
	void OnLevel0Init(intptr_t param);
	void OnLevel1Init(intptr_t param);
	void OnLevel2Init(intptr_t param);
	void OnLevel3Init(intptr_t param);
	void OnLevelUp0Init(intptr_t param);
	void OnLevelUp1Init(intptr_t param);
	void OnLevelUp2Init(intptr_t param);
	void OnLevelUp3Init(intptr_t param);
	void OnMatchRes0Init(intptr_t param);
	void OnMatchRes1Init(intptr_t param);
	void OnGainedLadderPoint0Init(intptr_t param);
	void OnGainedLadderPoint1Init(intptr_t param);
	void OnLadderPoint0Init(intptr_t param);
	void OnLadderPoint1Init(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	const char* m_sfx;                  // 0x110, the exp sound playing (OnProc; stopped by Close)
	float m_expTick;                    // 0x114, time banked for the exp count-up (OnProc)
	int m_slotPlayer[4];                // 0x118, the player index shown by each row (OnInit)
	FrListBox* m_pUserList;             // 0x128 (OnInit)
	FrGaugeBar* m_pExpBar[4];           // 0x12c, by row
	FrStatic* m_pExp[4];                // 0x13c, by row
	FrStatic* m_pExpEnd[4];             // 0x14c, by row
	FrArea* m_pLevel[4];                // 0x15c, by row
	FrArea* m_pLevelUp[4];              // 0x16c, by row
	FrArea* m_pMatchRes[2];             // 0x17c (OnMatchRes0Init/OnMatchRes1Init)
	int m_level[4];                     // 0x184, by player: the level shown, raised by OnProc
	int m_gainExp[4];                   // 0x194, by player: exp still to count up (negative: down)
	int m_exp[4];                       // 0x1a4, by player: exp shown, in tenths
	unsigned char m_expDone[4];         // 0x1b4, by player: the count-up is over
	int m_numRows;                      // 0x1b8, the rows of the user list
	FrStatic* m_pGainedLadderPoint[2];  // 0x1bc
	FrStatic* m_pLadderPoint[2];        // 0x1c4
	FrEdit* m_pNextLevelText;           // 0x1cc
	PADDING(0x1d0, 0x1d4);
	float m_elapsed;                    // 0x1d4, time since the form opened (OnProc)
	const Bitmap* m_pVsIcon;            // 0x1d8, golf/pc_icon_vs (OnInit)
	const Bitmap* m_pBonus20Icon;       // 0x1dc, golf/pc_icon_bonus_20
	const Bitmap* m_pMascotBonus01;     // 0x1e0, golf/bonus_mascot_01
	const Bitmap* m_pMascotBonus02;     // 0x1e4, golf/bonus_mascot_02
	const Bitmap* m_pMascotBonus09;     // 0x1e8, golf/bonus_mascot_09
	const Bitmap* m_pMascotBonus10;     // 0x1ec, golf/bonus_mascot_10
	const Bitmap* m_pMascotBonus11;     // 0x1f0, golf/bonus_mascot_11
	const Bitmap* m_pMascotBonus12;     // 0x1f4, golf/bonus_mascot_12
	const Bitmap* m_pBonusMa;           // 0x1f8, golf/bonus_ma
	const Bitmap* m_pBonusMa2;          // 0x1fc, golf/bonus_ma2
	const Bitmap* m_pZeroVsIcon;        // 0x200, golf/icon_zero_vs
	PADDING(0x204, 0x208);

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(FrAwardForm, 0x208);
BEGIN_LAYOUT_CHECK(FrAwardForm)
	CHECK_FIELD(FrAwardForm, m_sfx, 0x110);
	CHECK_FIELD(FrAwardForm, m_expTick, 0x114);
	CHECK_FIELD(FrAwardForm, m_slotPlayer, 0x118);
	CHECK_FIELD(FrAwardForm, m_pUserList, 0x128);
	CHECK_FIELD(FrAwardForm, m_pExpBar, 0x12c);
	CHECK_FIELD(FrAwardForm, m_pExp, 0x13c);
	CHECK_FIELD(FrAwardForm, m_pExpEnd, 0x14c);
	CHECK_FIELD(FrAwardForm, m_pLevel, 0x15c);
	CHECK_FIELD(FrAwardForm, m_pLevelUp, 0x16c);
	CHECK_FIELD(FrAwardForm, m_pMatchRes, 0x17c);
	CHECK_FIELD(FrAwardForm, m_level, 0x184);
	CHECK_FIELD(FrAwardForm, m_gainExp, 0x194);
	CHECK_FIELD(FrAwardForm, m_exp, 0x1a4);
	CHECK_FIELD(FrAwardForm, m_expDone, 0x1b4);
	CHECK_FIELD(FrAwardForm, m_numRows, 0x1b8);
	CHECK_FIELD(FrAwardForm, m_pGainedLadderPoint, 0x1bc);
	CHECK_FIELD(FrAwardForm, m_pLadderPoint, 0x1c4);
	CHECK_FIELD(FrAwardForm, m_pNextLevelText, 0x1cc);
	CHECK_FIELD(FrAwardForm, m_elapsed, 0x1d4);
	CHECK_FIELD(FrAwardForm, m_pVsIcon, 0x1d8);
	CHECK_FIELD(FrAwardForm, m_pBonus20Icon, 0x1dc);
	CHECK_FIELD(FrAwardForm, m_pMascotBonus01, 0x1e0);
	CHECK_FIELD(FrAwardForm, m_pMascotBonus12, 0x1f4);
	CHECK_FIELD(FrAwardForm, m_pBonusMa, 0x1f8);
	CHECK_FIELD(FrAwardForm, m_pBonusMa2, 0x1fc);
	CHECK_FIELD(FrAwardForm, m_pZeroVsIcon, 0x200);
END_LAYOUT_CHECK

// Item acquired during a round, shown on the score board.
class cAquireItem {
	LAYOUT_CHECK_FRIEND;
	friend class CScore;   // CScore::OnProcess resets m_value
public:
	cAquireItem(void);
	~cAquireItem(void);

	void Reset(void);
	void OnProc(float dt);
	void DisplayPrizeList(int player);

protected:
	WVector GetPlayer2DPos(int player);

private:
	UNKNOWN(0x0, 0x80);
	float m_value;      // 0x80
	float m_maxValue;   // 0x84
};

BEGIN_LAYOUT_CHECK(cAquireItem)
	CHECK_FIELD(cAquireItem, m_value, 0x80);
	CHECK_FIELD(cAquireItem, m_maxValue, 0x84);
END_LAYOUT_CHECK

// Score board dialog (size 0x200, the `new` of FrScoreFormMakeInstance).
class FrScoreForm : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:481

	// Only the hidden state is known (SetState(1) disables and hides the form).
	enum eState {
		STATE_NORMAL = 0,
		STATE_HIDDEN = 1
	};

	FrScoreForm(void);

	void SetState(eState state);
	void SetTypeBtn(void);
	void Resize(void);
	void SetDesc(void);
	FrFrame* GetBaseFrame(void) { return m_pBaseFrm; }
	void SetPPL(const char* name);
	void SortLists(bool bSort);
	void UpdatePang(void);
	void ChangeTurn(void);

protected:
	unsigned long GetScoreColor(int score);
	void DrawDigit(FrGraphicInterface* pGDI, char* text, WPoint pos, unsigned long color, FrGDI::eTextAlign align, bool bShadow);
	void SetFrontBackBtn(void);
	void OnFront9BtnUp(void);
	void OnBack9BtnUp(void);
	void OnShotBtnUp(void);
	void LoadBitmaps(void);
	virtual bool OnInit(void);
	virtual void OnDraw(void);
	void OnBlankPanel01OwnerDraw(int param);
	void OnBlankPanel02OwnerDraw(int param);
	void OnBlankPanel03OwnerDraw(int param);
	void OnCancelBtnUp(void);
	void OnCloseBtnInit(intptr_t param);
	void OnPangListInit(int param);
	void OnCloseBtnUp(void);
	virtual void OnProc(const float dt);
	void OnGraphBtnUp(void);
	void OnVersusBtnUp(void);
	void OnPuttBtnUp(void);
	void OnPangBtnUp(void);

protected:
	void OnBar2Init(intptr_t param);
	void OnBar3Init(intptr_t param);
	void OnFront9BtnInit(intptr_t param);
	void OnBack9BtnInit(intptr_t param);
	void OnShotBtnInit(intptr_t param);
	void OnGraphBtnInit(intptr_t param);
	void OnVersusBtnInit(intptr_t param);
	void OnPuttBtnInit(intptr_t param);
	void OnPangBtnInit(intptr_t param);
	void OnHoleListInit(intptr_t param);
	void ConditionalStrCat(char* dst, const char* src);
	void OnHoleListOwnerDraw(intptr_t param);
	void OnParListInit(intptr_t param);
	void OnParListOwnerDraw(intptr_t param);
	void OnPangListOwnerDraw(int param);
	void OnGolferListInit(intptr_t param);
	void OnGolferListOwnerDraw(intptr_t param);
	void OnRecordListInit(intptr_t param);
	void OnRecordListOwnerDraw(intptr_t param);
	void OnTotalListInit(intptr_t param);
	void OnTotalListOwnerDraw(int param);
	void OnTotalInit(intptr_t param);
	void OnPPLInit(intptr_t param);
	void OnBoard1Init(intptr_t param);
	void OnBoard2Init(intptr_t param);
	void OnDrawLinesInit(intptr_t param);
	void OnDrawLinesOwnerDraw(int param);
	void OnCancelBtnInit(intptr_t param);
	void OnBlankPanel01Init(intptr_t param);
	void OnBlankPanel02Init(intptr_t param);
	void OnBlankPanel03Init(intptr_t param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	eState m_state;               // 0x110
	FrButton* m_pFront9Btn;       // 0x114
	FrButton* m_pBack9Btn;        // 0x118
	FrButton* m_pTypeBtn[6];      // 0x11c (pressed for the current m_scoreType)
	FrListBox* m_pHoleList;       // 0x134 (OnHoleListInit)
	FrListBox* m_pParList;        // 0x138 (OnParListInit)
	FrListBox* m_pPangList;       // 0x13c (OnPangListInit)
	FrListBox* m_pScoreList[3];   // 0x140, the golfer, record and total lists (OnGolferListInit, OnRecordListInit, OnTotalListInit)
	FrButton* m_pCancelBtn;       // 0x14c (OnCancelBtnInit)
	FrButton* m_pCloseBtn;        // 0x150 (OnCloseBtnInit)
	FrArea* m_pBar2;              // 0x154 (OnBar2Init)
	FrArea* m_pBar3;              // 0x158 (OnBar3Init)
	FrArea* m_pPPLArea;           // 0x15c
	bool m_bPPL;                  // 0x160 (the PPL image loaded)
	void* m_rowData[4];           // 0x164, the item data of the score lists' rows (m_rowCount used)
	bool m_bFront9;               // 0x174 (front nine shown, else back nine)
	int m_scoreType;              // 0x178 (0 shot, 1 graph, 2 putt, 3 pang, 4, 5 versus)
	float m_typeTime;             // 0x17c
	int m_unknown180;             // 0x180
	int m_unknown184;             // 0x184
	int m_unknown188;             // 0x188
	int m_unknown18c;             // 0x18c
	int m_rowCount;               // 0x190, the rows of the score lists
	std::map<unsigned long, const Bitmap*> m_bitmaps;   // 0x194
	int m_unknown1a0[10];         // 0x1a0
	PADDING(0x1c8, 0x1d0);
	int m_unknown1d0;             // 0x1d0
	PADDING(0x1d4, 0x1f0);
	bool m_bShowBlankPanel;       // 0x1f0
	FrWnd* m_pBlankPanel01;  // 0x1f4
	FrWnd* m_pBlankPanel02;  // 0x1f8
	FrWnd* m_pBlankPanel03;  // 0x1fc

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(FrScoreForm, 0x200);
BEGIN_LAYOUT_CHECK(FrScoreForm)
	CHECK_FIELD(FrScoreForm, m_state, 0x110);
	CHECK_FIELD(FrScoreForm, m_pFront9Btn, 0x114);
	CHECK_FIELD(FrScoreForm, m_pBack9Btn, 0x118);
	CHECK_FIELD(FrScoreForm, m_pTypeBtn, 0x11c);
	CHECK_FIELD(FrScoreForm, m_pHoleList, 0x134);
	CHECK_FIELD(FrScoreForm, m_pParList, 0x138);
	CHECK_FIELD(FrScoreForm, m_pScoreList, 0x140);
	CHECK_FIELD(FrScoreForm, m_pCancelBtn, 0x14c);
	CHECK_FIELD(FrScoreForm, m_pPangList, 0x13c);
	CHECK_FIELD(FrScoreForm, m_pBar2, 0x154);
	CHECK_FIELD(FrScoreForm, m_pBar3, 0x158);
	CHECK_FIELD(FrScoreForm, m_pPPLArea, 0x15c);
	CHECK_FIELD(FrScoreForm, m_bPPL, 0x160);
	CHECK_FIELD(FrScoreForm, m_rowData, 0x164);
	CHECK_FIELD(FrScoreForm, m_bFront9, 0x174);
	CHECK_FIELD(FrScoreForm, m_scoreType, 0x178);
	CHECK_FIELD(FrScoreForm, m_typeTime, 0x17c);
	CHECK_FIELD(FrScoreForm, m_unknown180, 0x180);
	CHECK_FIELD(FrScoreForm, m_rowCount, 0x190);
	CHECK_FIELD(FrScoreForm, m_bitmaps, 0x194);
	CHECK_FIELD(FrScoreForm, m_unknown1a0, 0x1a0);
	CHECK_FIELD(FrScoreForm, m_unknown1d0, 0x1d0);
	CHECK_FIELD(FrScoreForm, m_bShowBlankPanel, 0x1f0);
	CHECK_FIELD(FrScoreForm, m_pBlankPanel01, 0x1f4);
	CHECK_FIELD(FrScoreForm, m_pBlankPanel02, 0x1f8);
	CHECK_FIELD(FrScoreForm, m_pBlankPanel03, 0x1fc);
END_LAYOUT_CHECK

// Match items dialog (size 0x158, the `new` of FrMatchItemsMakeInstance).
class FrMatchItems : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:664

	FrMatchItems(void);
	virtual ~FrMatchItems(void);

	void Reset(void);   // scoreboard.h:670
	void Refresh(void) { m_bRefresh = true; m_bRedraw = true; }

protected:
	virtual bool OnInit(void);
	void OnViewInit(intptr_t param);
	void OnViewOwnerDraw(int param);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	std::map<unsigned long, WOverlay*> m_overlays;   // 0x110
	PADDING(0x11c, 0x120);   // FrMatchItems members
	WPoint m_pos;            // 0x120, centred above the bottom bar (OnInit)
	PADDING(0x128, 0x140);
	FrArea* m_pView;         // 0x140 (OnViewInit)
	CSimpleUI* m_pSimpleUI;  // 0x144
	bool m_bRefresh;         // 0x148
	bool m_bRedraw;          // 0x149
	PADDING(0x14a, 0x14c);
	int m_unknown14c;        // 0x14c (OnInit)
	int m_unknown150;        // 0x150 (OnInit)
	float m_unknown154;      // 0x154 (OnInit: -3)
};

CHECK_SIZE(FrMatchItems, 0x158);
BEGIN_LAYOUT_CHECK(FrMatchItems)
	CHECK_FIELD(FrMatchItems, m_overlays, 0x110);
	CHECK_FIELD(FrMatchItems, m_pView, 0x140);
	CHECK_FIELD(FrMatchItems, m_pSimpleUI, 0x144);
	CHECK_FIELD(FrMatchItems, m_bRefresh, 0x148);
	CHECK_FIELD(FrMatchItems, m_bRedraw, 0x149);
	CHECK_FIELD(FrMatchItems, m_pos, 0x120);
	CHECK_FIELD(FrMatchItems, m_unknown154, 0x154);
END_LAYOUT_CHECK

// Score actor of the round (size 0x9c, the `new` of CScoreMakeInstance):
// an IActor with a Fresh command target at 0x18.
class CScore : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
public:
	// Background of the score board.
	class CScoreBg {
		LAYOUT_CHECK_FRIEND;
	public:
		CScoreBg(void);
		~CScoreBg(void);

		void ScoreBgCommand(int command);

	protected:
		void SetDesktopWallPaper(bool bShow, bool bDefault);

	private:
		Bitmap* m_pImage; // 0x0
		bool m_bShow;     // 0x4
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoreboard.h:719

protected:
	CScore(void);
	friend IObject* CScoreMakeInstance(void);   // the factory creates it

public:
	virtual ~CScore(void);

	virtual void OnLoad(void);
	virtual void OnDestroy(void);
	virtual void OnDisplay(void);
	virtual void OnReload(void);
	virtual void OnProcess(float dt);
	virtual void HandleMsg(const MsgObject& msg);

protected:
	void OnCloseBtnUp(void);
	bool OnLevelupFormResult(int ret, FrForm* pForm);
	bool OnRankFormExResult(int ret, FrForm* pForm);
	bool OnGuildScoreFormResult(int ret, FrForm* pForm);
	bool UpdateRivals(void);

private:
	void CheckOpenedDlg(void);
	void ImageSortByCharacterIndex(std::vector<sNoticeImage>& images);

	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

private:
	int m_unknown1c;               // 0x1c
	float m_awardTime;             // 0x20, time since the round ended (OnProcess)
	int m_bOpen;                   // 0x24
	int m_unknown28;               // 0x28
	int m_unknown2c;               // 0x2c
	int m_unknown30;               // 0x30
	FrScoreForm* m_pScoreForm;     // 0x34 ("score" / "score_skins", OnLoad)
	FrRankForm* m_pRankForm;       // 0x38 ("rank", OnLoad)
	FrRankForm2* m_pRankForm2;     // 0x3c ("rank2", GM view, OnLoad)
	FrRankForm3* m_pRankForm3;     // 0x40 ("rank3", guild match, OnLoad)
	FrRankFormEx* m_pRankFormEx;   // 0x44
	FrAwardForm* m_pAwardForm;     // 0x48 ("award", OnProcess)
	FrMatchItems* m_pMatchItems;   // 0x4c ("items", OnLoad)
	FrScoreDlg* m_pGuildScoreForm; // 0x50 ("score_guild", OnLoad)
	std::vector<FrLevelupItemForm*> m_levelupForms;   // 0x54
	FrTreasureGauge* m_pTreasureGauge;   // 0x64 ("tPointdlg", OnLoad)
	FrNextHole* m_pNextHole;       // 0x68 ("nexthole", OnLoad)
	FrTreasureScore* m_pTreasureScore;   // 0x6c ("trscore", OnProcess)
	FrTreasureGift* m_pTreasureGift;     // 0x70 ("trgift", OnProcess)
	float m_fadeTime;              // 0x74, desktop fade while m_bOpen is 2 or 3 (OnProcess)
	WPoint m_rankPos;              // 0x78, where the rank form opens (OnLoad)
	cAquireItem* m_pAquireItem;    // 0x80
	std::vector<sNoticeImage> m_noticeImages;   // 0x84
	CScoreBg m_scoreBg;            // 0x94
};

CHECK_SIZE(CScore, 0x9c);
BEGIN_LAYOUT_CHECK(CScore)
	CHECK_FIELD(CScore, m_unknown1c, 0x1c);
	CHECK_FIELD(CScore, m_bOpen, 0x24);
	CHECK_FIELD(CScore, m_unknown28, 0x28);
	CHECK_FIELD(CScore, m_pRankFormEx, 0x44);
	CHECK_FIELD(CScore, m_pGuildScoreForm, 0x50);
	CHECK_FIELD(CScore, m_levelupForms, 0x54);
	CHECK_FIELD(CScore, m_pScoreForm, 0x34);
	CHECK_FIELD(CScore, m_pMatchItems, 0x4c);
	CHECK_FIELD(CScore, m_pTreasureGauge, 0x64);
	CHECK_FIELD(CScore, m_rankPos, 0x78);
	CHECK_FIELD(CScore, m_pAquireItem, 0x80);
	CHECK_FIELD(CScore, m_noticeImages, 0x84);
	CHECK_FIELD(CScore, m_scoreBg, 0x94);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SCOREBOARD_H
