// Original: d:\build\custom\temp\code\client\projectg\user_info.h
#ifndef PG_CLIENT_PROJECTG_USER_INFO_H
#define PG_CLIENT_PROJECTG_USER_INFO_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "frform.h"
#include "frbutton.h"
#include "frcmdtarget.h"
#include "AnotherHand.h"
#include "SharedDoc.h"
#include "Packet.h"
#include <string>
#include <list>

class Bitmap;
class CExhibition;
class CPartTidList;
class FrAddFriendDlg;
class FrArea;
class FrGaugeBar;
class WTitleFont;
class FrEdit;
class FrListBox;
struct FrListItem;
class FrStatic;

// The user information form; its size (0x2568) is the `new` of
// FrUserInfoFormMakeInstance. CHandOwner (0x110) receives the pet model loaded
// on the worker thread (OnAnotherHand adjusts `this` by -0x110). Only the
// members this project's code uses so far are declared. Byte-packed: the
// match-history records (4-byte aligned on their own) sit at 0x244D.
#pragma pack(push, 1)
class FrUserInfoForm : public FrForm, public CHandOwner {
	LAYOUT_CHECK_FRIEND;
	friend class CTaskMain;   // HandleMsgCommon reads m_addFriendDlg

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // user_info.h:35

	// The two tabs of the form (SetFormA).
	enum eTabType {
		TAB_GENERAL_INFO = 0,
		TAB_SPECIAL_INFO = 1,
		TAB_RECORD_INFO = 2,
		TAB_TROPHY_INFO = 3,
		TAB_GUILD_INFO = 4,
		TAB_NONE = 5          // no tab shown yet (OnInit)
	};

	// The season whose statistics a page shows.
	enum eSeasonType {
		SEASON_CURRENT = 0,
		SEASON_TOTAL = 1
	};

	// One season's statistics of the shown player (0x4B0 bytes; the form
	// holds two, at 0xE90 and 0x1340; SelectContents hands out the second
	// one's statistics, trophies and lists for SEASON_TOTAL).
	struct sSeasonStat {
		// user_info.h:361
		void Clear(void) {
			memset(&m_statistics, 0, sizeof(m_statistics));
			memset(m_mapStats, 0, sizeof(m_mapStats));
			for (int i = 0; i < 20; i++) {
				m_mapStats[i].m_bestScore = 0x7F;
			}
			memset(&m_trophy, 0, sizeof(m_trophy));
			m_specialTrophies.clear();
			m_guildTrophies.clear();
		}

		sPangYaUserStatistics m_statistics;            // 0x0
		sMapStatistics m_mapStats[20];                 // 0xEB, one record per course
		sTrophyStatistics m_trophy;                    // 0x447
		UNKNOWN(0x495, 0x498);
		std::list<sSpecialTrophy> m_specialTrophies;   // 0x498
		std::list<sGuildTrophy> m_guildTrophies;       // 0x4A4
	};

	FrUserInfoForm(void);
	virtual ~FrUserInfoForm(void);

	void SetMsnControl(bool b) { m_bMsnControl = b; }       // user_info.h:378
	void SetControlOff(bool b) { m_bControlOff = b; }       // user_info.h:379
	bool IsOpenAddFriendDlg(void) const;   // user_info.h:386
	int IsRecvSeason(unsigned long season) { return (m_recvSeasonFlags & season) ? 1 : 0; }   // user_info.h:569

	void Clear(void);
	void SetContent(bool b);
	void SetInfo(sUserInfoTime *pInfo);
	void RecvedSeasonData(unsigned long season);
	int IsTotalTab(void);
	void RequestSeasonData(unsigned char season);
	void OnTitleSeasonVisible(void);
	void SetRequestFriendData(const char *nick, unsigned long uid);
	void OnCustomProc(float elapsed);

	virtual bool Close(eFormRet ret, bool bDestroy);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnAnotherHand(int nWork, void *pData);
	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void RefreshAllUccClothes(void);

	void ShowPet(void);
	void HidePet(bool bWait);
	void OnGeneralInfoInit(int param);
	void OnSpecialInfoInit(int param);
	void OnRecordInfoInit(int param);
	void OnTrophyInfoInit(int param);
	void OnGuildInfoInit(int param);
	void OnGeneralBackInit(int param);
	void OnRecCourseBackInit(int param);
	void OnTroNormalBackInit(int param);
	void OnTroSpecialBackInit(int param);
	void OnGuildBackInit(int param);
	void OnListBoxInit_TRI_SPECIALBOX(int param);
	void OnListBoxInit_TRI_NORMALBOX(int param);
	void OnWhisperInit(int param);
	void OnFriendInit(int param);
	void OnIgnoreInit(intptr_t param);
	void OnPetInit(int param);
	void OnRecAwardBackInit(int param);
	void OnBtn_RmrVisitorInit(int param);
	void OnMatchHistoryBtn1Init(int param);
	void OnMatchHistoryBtn2Init(int param);
	void OnMatchHistoryBtn3Init(int param);
	void OnMatchHistoryBtn4Init(int param);
	void OnMatchHistoryBtn5Init(int param);
	void OnAreaInit_GI_Level(int param);
	void OnAreaInit_GI_ClUB(int param);
	void OnStaticInit_TRI_NOTROPHY(int param);
	void OnStaticInit_GUILD_NOTROPHY(int param);
	void OnTrophyNormalNumInit(int param);
	void OnTrophySpecialNumInit(int param);
	void OnEditInit_GI_ID(int param);
	void OnEditInit_GI_SCHOOL(int param);
	void OnEditInit_GI_CLUB(int param);
	void OnEditInit_GI_EXP(int param);
	void OnEditInit_GI_CONNSTATUS(int param);
	void OnEditInit_GI_DISCONN(int param);
	void OnEditInit_GI_COMBO(int param);
	void OnEditInit_GI_QUIZLEVEL(int param);
	void OnEditInit_SI_TROPHY_GOLD(int param);
	void OnEditInit_SI_TROPHY_SILVER(int param);
	void OnEditInit_SI_TROPHY_BRONZE(int param);
	void OnEditInit_SI_MATCH_WIN_PERCENTAGE(int param);
	void OnEditInit_SI_AVG_SCORE(int param);
	void OnEditInit_SI_MEAN_SHOTTIME(int param);
	void OnEditInit_SI_PANGYA_PERCENTAGE(int param);
	void OnEditInit_SI_FAIRWAY_PERCENTAGE(int param);
	void OnEditInit_SI_PUTT_PERCENTAGE(int param);
	void OnEditInit_SI_OB_PERCENTAGE(int param);
	void OnEditInit_SI_TOTAL_SKINS_PANG(int param);
	void OnEditInit_RI_TOT_PLAYTIME(int param);
	void OnEditInit_RI_TOT_SHOTS(int param);
	void OnEditInit_RI_TOT_HOLES(int param);
	void OnEditInit_RI_TOT_DRIVE(int param);
	void OnEditInit_RI_EARTH_ROUND(int param);
	void OnEditInit_RI_BEST_SCORE_5(int param);
	void OnEditInit_RI_BEST_SCORE_4(int param);
	void OnEditInit_RI_BEST_SCORE_3(int param);
	void OnEditInit_RI_BEST_SCORE_2(int param);
	void OnEditInit_RI_BEST_SCORE_1(int param);
	void OnEditInit_RI_TOT_HOLEINONES(int param);
	void OnEditInit_RI_TOT_ALBATROSS(int param);
	void OnEditInit_RI_LONGEST_DRIVE(int param);
	void OnEditInit_RI_LONGEST_PUTT(int param);
	void OnEditInit_RI_LONGEST_CHIPIN(int param);
	void OnEditInit_GUILD_NAME(int param);
	void OnEditInit_GUILD_PANG(int param);
	void OnEditInit_GUILD_MYPERCENT(int param);
	void OnEditInit_SI_MATCH_HISTORY1(int param);
	void OnEditInit_SI_MATCH_HISTORY2(int param);
	void OnEditInit_SI_MATCH_HISTORY3(int param);
	void OnEditInit_SI_MATCH_HISTORY4(int param);
	void OnEditInit_SI_MATCH_HISTORY5(int param);
	void OnTitleSeasonBtnInit(int param);
	void OnTitleTotalBtnInit(int param);
	void OnRecordNormalBtnInit(int param);
	void OnRecordCourseBtnInit(int param);
	void OnRecordClassicBtnInit(int param);
	void OnTrophyNormalBtnInit(int param);
	void OnTrophySpecialBtnInit(int param);
	void OnGhostdUp(void);
	void OnTitleTotalBtnUp(void);

	void SetFormA(eTabType tab);
	void SetVisible(eTabType tab, bool bVisible);
	void OnMyGuildInit(int param);
	void OnCommentInit(int param);
	void OnGuildTrophyListInit(int param);
	void OnNickInit(intptr_t param);

	void SetVisible(FrWnd* pWnds[], int count, bool bVisible);

	void SetTrophyStatic(void);

	void OnTitleSeasonBtnUp(void);
	void OnCloseUp(void);
	void OnEditInit_HoverOff(int param);
	void OnGeneralInfoDown(void);
	void OnSpecialInfoDown(void);
	void OnRecordInfoDown(void);
	void OnTrophyInfoDown(void);
	void OnGuildInfoDown(void);
	void ChangeTrophyType(void);
	void OnTrophyNormalBtnUp(void);
	void ChangeRecordType(void);
	void OnRecordNormalBtnUp(void);
	void OnRecordCourseBtnUp(void);
	void OnRecordClassicBtnUp(void);
	void OnTrophySpecialBtnUp(void);
	void OnMatchHistoryBtn1Up(void);
	void OnMatchHistoryBtn2Up(void);
	void OnMatchHistoryBtn3Up(void);
	void OnMatchHistoryBtn4Up(void);
	void OnMatchHistoryBtn5Up(void);
	void OnBtn_RmrVisitorBtnUp(void);
	void OnPetLoadInit(int param);
	void OnEditInit_HoverOn(int param);
	void OnIgnoreUp(void);
	void OnSpecListboxInit(int param);
	void OnScoreListInit(int param);
	void OnRecTotalListboxInit(int param);
	void OnAreaInit_Question_mark(int param);
	unsigned long GetBestScoreColor(unsigned char course);
	unsigned long GetMaxPangColor(unsigned char course);
	unsigned long Get30sRecordColor(int medal);

private:
	void SelectContents(eSeasonType season, sPangYaUserStatistics **ppStatistics,
			sTrophyStatistics **ppTrophy, std::list<sSpecialTrophy> **ppSpecialTrophies,
			std::list<sGuildTrophy> **ppGuildTrophies);
	unsigned long GetSeasonTextColor(eSeasonType season);
	void UpdateLevelTextImg(unsigned char level);
	void MatchHistoryUserBtnUp(int index);
	void SetMatchHistoryContents(void);
	void MsnDisconExeception(float elapsed, FrListItem *pItem);
	void SetSpecListBoxItem(eSeasonType season);
	void UpdateEditMapStatistics(eSeasonType season, bool bClassic);
	void SetDisconntextAlign(void);
	void SetDisconnQuestionMark(bool bVisible);
	void SetIgnoreButtonImg(bool bIgnored);

	eSeasonType m_seasonTab;       // 0x114, the season tab shown
	eTabType m_curTab;             // 0x118, the tab shown (SetFormA)
	FrButton *m_pCurSeasonBtn;     // 0x11C, pressed by OnTitleSeasonVisible
	FrButton *m_pTotalSeasonBtn;   // 0x120, released by OnTitleSeasonVisible
	FrButton *m_pTabBtn[5];        // 0x124, the five tab buttons, indexed by eTabType
	FrArea *m_pGeneralBack;        // 0x138, the tab pages' backgrounds
	UNKNOWN(0x13C, 0x140);
	FrArea *m_pRecAwardBack;       // 0x140
	FrArea *m_pRecCourseBack;      // 0x144
	FrArea *m_pTroNormalBack;      // 0x148
	FrArea *m_pTroSpecialBack;     // 0x14C
	FrArea *m_pGuildBack;          // 0x150
	FrEdit *m_pIdEdit;             // 0x154, general info edits
	FrEdit *m_pSchoolEdit;         // 0x158
	FrEdit *m_pClubEdit;           // 0x15C
	FrEdit *m_pExpEdit;            // 0x160
	FrEdit *m_pConnStatusEdit;     // 0x164
	FrEdit *m_pDisconnEdit;        // 0x168
	FrEdit *m_pComboEdit;          // 0x16C
	FrEdit *m_pQuizLevelEdit;      // 0x170
	FrArea *m_pLevelArea;          // 0x174, the level image (UpdateLevelTextImg)
	FrArea *m_pClubArea;           // 0x178
	FrArea *m_pDisconnQuestionMark; // 0x17C, SetDisconnQuestionMark
	WRect m_disconnEditRect;       // 0x180, the disconnect edit's rect without the question mark (OnInit, SetDisconntextAlign)
	union {
	struct {
	FrEdit *m_pTrophyGoldEdit;     // 0x190, special info edits
	FrEdit *m_pTrophySilverEdit;   // 0x194
	FrEdit *m_pTrophyBronzeEdit;   // 0x198
	FrEdit *m_pMatchWinPercentEdit; // 0x19C
	FrEdit *m_pAvgScoreEdit;       // 0x1A0
	FrEdit *m_pMeanShotTimeEdit;   // 0x1A4
	FrEdit *m_pPangyaPercentEdit;  // 0x1A8
	FrEdit *m_pFairwayPercentEdit; // 0x1AC
	FrEdit *m_pPuttPercentEdit;    // 0x1B0
	FrEdit *m_pObPercentEdit;      // 0x1B4
	FrEdit *m_pTotalSkinsPangEdit; // 0x1B8
	FrEdit *m_pMatchHistoryEdit[5]; // 0x1BC
	};
	FrWnd *m_pSpecialInfoWnd[16];  // 0x190, the sixteen above, hidden as one (OnInit, SetVisible)
	};
	FrListBox *m_pSpecList;        // 0x1D0, the special info backgrounds (OnSpecListboxInit)
	const Bitmap *m_pSpecBackBmp;  // 0x1D4, special_back
	const Bitmap *m_pSpecBack00Bmp; // 0x1D8, special_back00
	FrButton *m_pMatchHistoryBtn[5]; // 0x1DC
	const Bitmap *m_pRecTotalBack01Bmp; // 0x1F0, special_back01 (OnRecTotalListboxInit)
	const Bitmap *m_pRecTotalBack02Bmp; // 0x1F4, special_back02
	const Bitmap *m_pRecTotalBack03Bmp; // 0x1F8, special_back03
	const Bitmap *m_pRecTotalBack02NdBmp; // 0x1FC, special_back02_nd
	FrListBox *m_pRecTotalList;    // 0x200, the record totals' backgrounds
	FrButton *m_pRecordNormalBtn;  // 0x204
	UNKNOWN(0x208, 0x20C);
	FrButton *m_pRecordCourseBtn;  // 0x20C
	FrButton *m_pRecordClassicBtn; // 0x210
	int m_recordType;              // 0x214, 0 normal, 2 course, 3 classic
	// 0x218, record info edits, in order: total play time, shots, holes,
	// earth rounds, drive, best scores 5..1, hole-in-ones, albatrosses,
	// longest drive, putt and chip-in (ChangeRecordType hides them as one)
	FrEdit *m_pRecordEdit[15];
	FrWnd *m_pRecAwardWnd[6];      // 0x254, shown with the award page (ChangeRecordType)
	int m_trophyType;              // 0x26C, 0 = normal trophies, 1 = special
	int m_trophyListState;         // 0x270, cleared by the constructor (name provisional, use not identified)
	FrButton *m_pTrophyNormalBtn;  // 0x274
	FrButton *m_pTrophySpecialBtn; // 0x278
	FrListBox *m_pTrophySpecialList; // 0x27C
	FrListBox *m_pTrophyNormalList;  // 0x280
	FrStatic *m_pNoTrophyStatic;   // 0x284
	FrEdit *m_pTrophyNormalNum;    // 0x288
	FrEdit *m_pTrophySpecialNum;   // 0x28C
	union {
	struct {
	FrEdit *m_pGuildNameEdit;      // 0x290
	FrEdit *m_pGuildPangEdit;      // 0x294
	FrEdit *m_pGuildMyPercentEdit; // 0x298
	};
	FrWnd *m_pGuildInfoEdit[3];    // 0x290, the three above, hidden as one (OnInit, SetVisible)
	};
	FrStatic *m_pGuildNoTrophyStatic; // 0x29C
	FrListBox *m_pGuildTrophyList; // 0x2A0, OnGuildTrophyListInit
	FrArea *m_pMyGuildArea;        // 0x2A4, OnMyGuildInit
	FrArea *m_pGuildMarkArea;      // 0x2A8, cleared by the constructor (name provisional, use not identified)
	const Bitmap *m_pGuildTrophyBmp; // 0x2AC, TROPHY/guild
	UNKNOWN(0x2B0, 0x2E0);
	FrButton *m_pRmrVisitorBtn;    // 0x2E0
	sUserInfo m_userInfo;          // 0x2E4, the shown player's information (SetInfo)
	UNKNOWN(0xE76, 0xE78);
	std::list<sSpecialTrophy> m_specialTrophies;   // 0xE78, the shown player's trophy lists
	std::list<sGuildTrophy> m_guildTrophies;       // 0xE84
	sSeasonStat m_seasonRecord;    // 0xE90, a season's statistics (which season is not identified yet)
	sSeasonStat m_totalRecord;     // 0x1340, the total season's statistics (SelectContents)
	sFriendExtra m_friendExtra;    // 0x17F0, reset by the constructor (use not identified yet)
	sMapStatistics m_classicMapStats[20];       // 0x183B, the current season's classic course records (UpdateEditMapStatistics)
	sMapStatistics m_seasonClassicMapStats[20]; // 0x1B97, constructed and cleared with the other two (use not identified yet)
	sMapStatistics m_totalClassicMapStats[20];  // 0x1EF3, the total season's classic course records
	UNKNOWN(0x224F, 0x2250);
	unsigned long m_matchHistoryState[5]; // 0x2250, cleared by the constructor (name provisional, use not identified)
	FrWnd *m_pGhostWnd;            // 0x2264, cleared by the constructor (name provisional, use not identified)
	FrWnd *m_pGhostListWnd;        // 0x2268, cleared by the constructor (name provisional, use not identified)
	FrButton *m_pFriendBtn;        // 0x226C
	FrButton *m_pWhisperBtn;       // 0x2270
	FrButton *m_pIgnoreBtn;        // 0x2274
	FrArea *m_pPetArea;            // 0x2278
	FrEdit *m_pCommentEdit;        // 0x227C, OnCommentInit, hidden by OnEditInit_HoverOff
	FrArea *m_pNickArea;           // 0x2280, OnNickInit
	const Bitmap *m_pMaleBmp;      // 0x2284, i_male
	const Bitmap *m_pFemaleBmp;    // 0x2288, i_female
	FrListBox *m_pScoreList;       // 0x228C, OnScoreListInit
	WTitleFont *m_pTitleFont;      // 0x2290, released by Clear
	FrWnd *m_pTitleWnd;            // 0x2294, cleared by the constructor (name provisional, use not identified)
	FrWnd *m_pTitleBackWnd;        // 0x2298, cleared by the constructor (name provisional, use not identified)
	FrWnd *m_pTitleIconWnd;        // 0x229C, cleared by the constructor (name provisional, use not identified)
	FrWnd *m_pTitleTextWnd;        // 0x22A0, cleared by the constructor (name provisional, use not identified)
	bool m_bOption;                // 0x22A4, cleared by the constructor (name provisional, use not identified)
	bool m_bMsnControl;            // 0x22A5, SetMsnControl
	bool m_bControlOff;            // 0x22A6, SetControlOff
	UNKNOWN(0x22A7, 0x22A8);
	float m_petLoadTime;           // 0x22A8, time the pet gauge has run (OnProc hides it at 3 s)
	bool m_bPetShown;              // 0x22AC, cleared by the constructor (name provisional, use not identified)
	UNKNOWN(0x22AD, 0x22B0);
	CExhibition *m_pPetExhibition; // 0x22B0, the pet's display (ShowPet/HidePet)
	CPartTidList *m_pPetTids;      // 0x22B4, the shown character's part ids (ShowPet)
	FrGaugeBar *m_pPetLoadGauge;   // 0x22B8, the pet loading gauge (OnPetLoadInit)
	CAnotherHand m_anotherHand;    // 0x22BC, loads the pet on its worker thread
	bool m_bMatchHistoryRequested; // 0x2444, set when a match-history player's information is requested (MatchHistoryUserBtnUp)
	bool m_bFriendRequested;       // 0x2445, cleared by the constructor (name provisional, use not identified)
	UNKNOWN(0x2446, 0x2448);
	float m_lastElapsed;           // 0x2448, the last frame time given to OnProc
	bool m_bCheckTime;             // 0x244C, cleared by the constructor (name provisional, use not identified)
	sUserMatchHistory m_matchHistory[5]; // 0x244D, the last games' players (SetMatchHistoryContents)
	UNKNOWN(0x2551, 0x2554);
	FrAddFriendDlg *m_addFriendDlg; // 0x2554, the add-friend dialog while it is open
	unsigned long m_requestedSeason; // 0x2558, cleared by the constructor (name provisional, use not identified)
	unsigned long m_recvSeasonFlags;   // 0x255C, seasons whose data was received
	unsigned long m_seasonPending;     // 0x2560, cleared when a season's data arrives
	UNKNOWN(0x2564, 0x2568);
};
#pragma pack(pop)
BEGIN_LAYOUT_CHECK(FrUserInfoForm)
	CHECK_FIELD(FrUserInfoForm, m_seasonTab, 0x114);
	CHECK_FIELD(FrUserInfoForm, m_classicMapStats, 0x183B);
	CHECK_FIELD(FrUserInfoForm, m_seasonClassicMapStats, 0x1B97);
	CHECK_FIELD(FrUserInfoForm, m_totalClassicMapStats, 0x1EF3);
	CHECK_FIELD(FrUserInfoForm, m_pCurSeasonBtn, 0x11C);
	CHECK_FIELD(FrUserInfoForm, m_pTotalSeasonBtn, 0x120);
	CHECK_FIELD(FrUserInfoForm, m_curTab, 0x118);
	CHECK_FIELD(FrUserInfoForm, m_pTabBtn, 0x124);
	CHECK_FIELD(FrUserInfoForm, m_pGuildBack, 0x150);
	CHECK_FIELD(FrUserInfoForm, m_pQuizLevelEdit, 0x170);
	CHECK_FIELD(FrUserInfoForm, m_pLevelArea, 0x174);
	CHECK_FIELD(FrUserInfoForm, m_pClubArea, 0x178);
	CHECK_FIELD(FrUserInfoForm, m_pDisconnQuestionMark, 0x17C);
	CHECK_FIELD(FrUserInfoForm, m_disconnEditRect, 0x180);
	CHECK_FIELD(FrUserInfoForm, m_pSpecialInfoWnd, 0x190);
	CHECK_FIELD(FrUserInfoForm, m_pGuildInfoEdit, 0x290);
	CHECK_FIELD(FrUserInfoForm, m_pTrophyGoldEdit, 0x190);
	CHECK_FIELD(FrUserInfoForm, m_pMatchHistoryEdit, 0x1BC);
	CHECK_FIELD(FrUserInfoForm, m_pSpecList, 0x1D0);
	CHECK_FIELD(FrUserInfoForm, m_pSpecBackBmp, 0x1D4);
	CHECK_FIELD(FrUserInfoForm, m_pSpecBack00Bmp, 0x1D8);
	CHECK_FIELD(FrUserInfoForm, m_pMatchHistoryBtn, 0x1DC);
	CHECK_FIELD(FrUserInfoForm, m_pRecTotalBack01Bmp, 0x1F0);
	CHECK_FIELD(FrUserInfoForm, m_pRecTotalBack02NdBmp, 0x1FC);
	CHECK_FIELD(FrUserInfoForm, m_pRecTotalList, 0x200);
	CHECK_FIELD(FrUserInfoForm, m_pRecordNormalBtn, 0x204);
	CHECK_FIELD(FrUserInfoForm, m_pRecordCourseBtn, 0x20C);
	CHECK_FIELD(FrUserInfoForm, m_pRecordClassicBtn, 0x210);
	CHECK_FIELD(FrUserInfoForm, m_recordType, 0x214);
	CHECK_FIELD(FrUserInfoForm, m_pRecordEdit, 0x218);
	CHECK_FIELD(FrUserInfoForm, m_pRecAwardWnd, 0x254);
	CHECK_FIELD(FrUserInfoForm, m_trophyType, 0x26C);
	CHECK_FIELD(FrUserInfoForm, m_pTrophyNormalBtn, 0x274);
	CHECK_FIELD(FrUserInfoForm, m_pTrophySpecialBtn, 0x278);
	CHECK_FIELD(FrUserInfoForm, m_pTrophySpecialList, 0x27C);
	CHECK_FIELD(FrUserInfoForm, m_pGuildNoTrophyStatic, 0x29C);
	CHECK_FIELD(FrUserInfoForm, m_pRmrVisitorBtn, 0x2E0);
	CHECK_FIELD(FrUserInfoForm, m_userInfo, 0x2E4);
	CHECK_FIELD(FrUserInfoForm, m_seasonRecord, 0xE90);
	CHECK_FIELD(FrUserInfoForm, m_totalRecord, 0x1340);
	CHECK_FIELD(FrUserInfoForm, m_pFriendBtn, 0x226C);
	CHECK_FIELD(FrUserInfoForm, m_pPetArea, 0x2278);
	CHECK_FIELD(FrUserInfoForm, m_pGuildTrophyList, 0x2A0);
	CHECK_FIELD(FrUserInfoForm, m_pMyGuildArea, 0x2A4);
	CHECK_FIELD(FrUserInfoForm, m_pNickArea, 0x2280);
	CHECK_FIELD(FrUserInfoForm, m_pFemaleBmp, 0x2288);
	CHECK_FIELD(FrUserInfoForm, m_pGuildTrophyBmp, 0x2AC);
	CHECK_FIELD(FrUserInfoForm, m_pCommentEdit, 0x227C);
	CHECK_FIELD(FrUserInfoForm, m_bMsnControl, 0x22A5);
	CHECK_FIELD(FrUserInfoForm, m_bControlOff, 0x22A6);
	CHECK_FIELD(FrUserInfoForm, m_pPetExhibition, 0x22B0);
	CHECK_FIELD(FrUserInfoForm, m_pPetTids, 0x22B4);
	CHECK_FIELD(FrUserInfoForm, m_pScoreList, 0x228C);
	CHECK_FIELD(FrUserInfoForm, m_pTitleFont, 0x2290);
	CHECK_FIELD(FrUserInfoForm, m_petLoadTime, 0x22A8);
	CHECK_FIELD(FrUserInfoForm, m_pPetLoadGauge, 0x22B8);
	CHECK_FIELD(FrUserInfoForm, m_bMatchHistoryRequested, 0x2444);
	CHECK_FIELD(FrUserInfoForm, m_lastElapsed, 0x2448);
	CHECK_FIELD(FrUserInfoForm, m_matchHistory, 0x244D);
	CHECK_FIELD(FrUserInfoForm, m_anotherHand, 0x22BC);
	CHECK_FIELD(FrUserInfoForm, m_addFriendDlg, 0x2554);
	CHECK_FIELD(FrUserInfoForm, m_recvSeasonFlags, 0x255C);
	CHECK_FIELD(FrUserInfoForm, m_seasonPending, 0x2560);
END_LAYOUT_CHECK
CHECK_SIZE(FrUserInfoForm, 0x2568);
CHECK_SIZE(FrUserInfoForm::sSeasonStat, 0x4B0);
CHECK_FIELD(FrUserInfoForm::sSeasonStat, m_mapStats, 0xEB);
CHECK_FIELD(FrUserInfoForm::sSeasonStat, m_trophy, 0x447);
CHECK_FIELD(FrUserInfoForm::sSeasonStat, m_specialTrophies, 0x498);
CHECK_FIELD(FrUserInfoForm::sSeasonStat, m_guildTrophies, 0x4A4);


// Only the members this project's code uses so far are declared.
class CUserInfo : public WSingleton<CUserInfo>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
	friend class FrUserInfoForm;
	friend class CLobbyTask;  // OnPacket 0xBF reads m_guid

public:
	CUserInfo(void);
	virtual ~CUserInfo(void);

	unsigned long GetGuid(void);           // user_info.h:597
	FrUserInfoForm *GetDlg(void);          // user_info.h:601

	bool IsVisible(void);
	bool OnUserInfoFormResult(int ret, FrForm *pForm);
	void Close(void);
	void Clear(void);
	void Refresh(void);
	bool IsRecvedData(unsigned char season);
	void Process(float elapsed);
	void SetInfo(unsigned long uid, unsigned long guid, bool bRequest, bool bSendPacket, bool bMsnControl,
		bool bControlOff, std::string nick);

private:
	FrUserInfoForm *m_dlg;         // 0x2C, the form while it is open
	unsigned long m_guid;          // 0x30, second argument of SetInfo
	unsigned long m_uid;           // 0x34, first argument of SetInfo
	bool m_bRequest;               // 0x38, third argument of SetInfo
	bool m_bHasInfo;               // 0x39, set by SetInfo, cleared when the form closes
	std::string m_nick;            // 0x3C, last argument of SetInfo when the form opens
};
BEGIN_LAYOUT_CHECK(CUserInfo)
	CHECK_FIELD(CUserInfo, m_dlg, 0x2C);
	CHECK_FIELD(CUserInfo, m_guid, 0x30);
	CHECK_FIELD(CUserInfo, m_uid, 0x34);
	CHECK_FIELD(CUserInfo, m_bRequest, 0x38);
	CHECK_FIELD(CUserInfo, m_bHasInfo, 0x39);
	CHECK_FIELD(CUserInfo, m_nick, 0x3C);
END_LAYOUT_CHECK
CHECK_SIZE(CUserInfo, 0x58);

// TaskMain.cpp defines TASKMAIN_USER_INFO_INLINE empty before its includes:
// the original holds the out-of-line copies of these inlines in TaskMain's
// object; every other unit inlines them.
#ifndef TASKMAIN_USER_INFO_INLINE
#define TASKMAIN_USER_INFO_INLINE inline
#endif

// user_info.h:601
TASKMAIN_USER_INFO_INLINE FrUserInfoForm *CUserInfo::GetDlg(void) {
	return m_dlg;
}

// user_info.h:621
TASKMAIN_USER_INFO_INLINE FrUserInfoForm *USERINFODLG(void) {
	if (CUserInfo::Instance() == NULL)
		return NULL;

	return CUserInfo::Instance()->GetDlg();
}

// The local player's last five games' opponents (TaskManager fills them).
class CMatchHistory : public WSingleton<CMatchHistory> {
	LAYOUT_CHECK_FRIEND;

public:
	CMatchHistory(void);                   // user_info.h:682
	virtual ~CMatchHistory(void);

	// user_info.h:643
	bool IsUpdated(void) {
		return m_bUpdated;
	}

	// user_info.h:645
	void SetMatchHistory(sUserMatchHistory *history) {
		if (history == NULL)
			return;

		memcpy(m_history, history, sizeof(m_history));
		if (m_bUpdated == false)
			m_bUpdated = true;
	}

	// user_info.h:655
	void LoginRequest(std::string loginName) {
		if (m_loginName != loginName) {
			m_loginName = loginName;
			RequestHistory();
		}
	}

	// user_info.h:664
	void RequestHistory(void) {
		WSendPacket send((enumClientPacket)0x99);
		send.Send(SEND_TO_DEFAULT);
		m_bUpdated = false;
	}

	// user_info.h:671
	void GetMatchHistory(sUserMatchHistory *history) {
		memcpy(history, m_history, sizeof(m_history));
	}

private:
	sUserMatchHistory m_history[5];    // 0x28
	bool m_bUpdated;                   // 0x12C, IsUpdated
	std::string m_loginName;           // 0x130, the account LoginRequest last requested the history of
};
BEGIN_LAYOUT_CHECK(CMatchHistory)
	CHECK_FIELD(CMatchHistory, m_history, 0x28);
	CHECK_FIELD(CMatchHistory, m_bUpdated, 0x12C);
	CHECK_FIELD(CMatchHistory, m_loginName, 0x130);
END_LAYOUT_CHECK
CHECK_SIZE(CMatchHistory, 0x14C);

#endif // PG_CLIENT_PROJECTG_USER_INFO_H
