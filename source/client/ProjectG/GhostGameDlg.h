// Original: d:\build\custom\temp\code\client\projectg\ghostgamedlg.h
#ifndef PG_CLIENT_PROJECTG_GHOSTGAMEDLG_H
#define PG_CLIENT_PROJECTG_GHOSTGAMEDLG_H

#include <windows.h>

#include <list>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "formbar.h"
#include "ghosttypes.h"

class Bitmap;
class FrArea;
class FrButton;
class FrGaugeBar;
class FrGraphicInterface;
class FrListBox;
class FrStatic;
class WTitleFont;

// List of the player's recent ghost matches; its size (0x160) is the `new` of
// FrRecentMatchMakeInstance. Only the members used so far are declared.
class FrRecentMatch : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	// One row of the history list (copied by AddHistory).
	struct sRecentMatchForm {
		unsigned char m_byte00;   // 0x00
		std::string m_name;       // 0x04
		int m_int20;              // 0x20
		unsigned char m_byte24;   // 0x24
		SYSTEMTIME m_time;        // 0x26
		int m_int38;              // 0x38
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ghostgamedlg.h:12

	FrRecentMatch(void);
	virtual ~FrRecentMatch(void);

	void Init(const std::string& ghostNick, int courseId, int itemTypeId, int number1, int number2);   // ghostgamedlg.cpp:70
	void AddHistory(const sRecentMatchForm& form);   // ghostgamedlg.cpp:90
	void AddFinish(void);   // ghostgamedlg.cpp:99
	void Exception(void);   // ghostgamedlg.cpp:109

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitBgArea(intptr_t);
	void OnDrawBgArea(void);
	void OnInitHistoryList(intptr_t);
	void OnDrawHistoryList(intptr_t);
	void OnInitStaticText1(intptr_t);

private:
	const Bitmap* m_pCourseImg; // 0x110 ("ghost_mid_<course icon>", Init)
	const Bitmap* m_pItemImg;   // 0x114 (the item's icon, Init)
	const Bitmap* m_pPangImg;   // 0x118 ("ghost_pang", Init)
	int m_number1;              // 0x11c (Init; OnDrawBgArea prints it)
	int m_number2;              // 0x120 (Init; OnDrawBgArea prints it)
	FrListBox* m_pHistoryList;  // 0x124 ("match_list")
	std::list<sRecentMatchForm*> m_history;   // 0x128
	FrArea* m_pBgArea;          // 0x134 ("bg_area")
	FrStatic* m_pStaticText1;   // 0x138 ("text1")
	WTitleFont* m_pTitleFont;   // 0x13c
	std::string m_string140;    // 0x140
	PADDING(0x15c, 0x160);      // FrRecentMatch members
};

CHECK_SIZE(FrRecentMatch::sRecentMatchForm, 0x3c);
CHECK_FIELD(FrRecentMatch::sRecentMatchForm, m_name, 0x04);
CHECK_FIELD(FrRecentMatch::sRecentMatchForm, m_int20, 0x20);
CHECK_FIELD(FrRecentMatch::sRecentMatchForm, m_byte24, 0x24);
CHECK_FIELD(FrRecentMatch::sRecentMatchForm, m_time, 0x26);
CHECK_FIELD(FrRecentMatch::sRecentMatchForm, m_int38, 0x38);
CHECK_SIZE(FrRecentMatch, 0x160);
BEGIN_LAYOUT_CHECK(FrRecentMatch)
	CHECK_FIELD(FrRecentMatch, m_pCourseImg, 0x110);
	CHECK_FIELD(FrRecentMatch, m_pItemImg, 0x114);
	CHECK_FIELD(FrRecentMatch, m_pPangImg, 0x118);
	CHECK_FIELD(FrRecentMatch, m_number1, 0x11c);
	CHECK_FIELD(FrRecentMatch, m_number2, 0x120);
	CHECK_FIELD(FrRecentMatch, m_pHistoryList, 0x124);
	CHECK_FIELD(FrRecentMatch, m_pBgArea, 0x134);
	CHECK_FIELD(FrRecentMatch, m_history, 0x128);
	CHECK_FIELD(FrRecentMatch, m_pStaticText1, 0x138);
	CHECK_FIELD(FrRecentMatch, m_pTitleFont, 0x13c);
	CHECK_FIELD(FrRecentMatch, m_string140, 0x140);
END_LAYOUT_CHECK

// Map selection dialog of the ghost game; its size (0x1a0) is the `new` of
// FrGhostGameMapDlgMakeInstance. Only the members used so far are declared.
class FrGhostGameMapDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ghostgamedlg.h:69

	FrGhostGameMapDlg(void);
	virtual ~FrGhostGameMapDlg(void);

	struct sImgTag {
		const Bitmap* m_pBitmap;     // 0x00 (OnDrawChoiceMap)
		const Bitmap* m_pEndBitmap;  // 0x04 (revealed from the left by OnDrawChoiceMap)
		WRect m_rect;      // 0x08 (copied member-wise by the generated copy constructor)
		float m_base;      // 0x18
		float m_endY;      // 0x1c
		float m_grow;      // 0x20
		float m_endH;      // 0x24
		float m_current;   // 0x28
		float m_alpha;     // 0x2c
		float m_alphaEnd;  // 0x30

		sImgTag(void) {}   // ghostgamedlg.h
		void Animate(float);   // ghostgamedlg.cpp:272
		void Reset(void);      // ghostgamedlg.cpp:296
	};

	struct sImgInfo {
		float m_x;               // 0x00
		float m_y;               // 0x04
		float m_unknown08;       // 0x08
		float m_lockOffset;      // 0x0c
		bool m_flag10;           // 0x10
		bool m_flag11;           // 0x11
		bool m_flag12;           // 0x12 (0x13: alignment)
		int m_mapId;             // 0x14 (20 = no map: OnProc skips it)
		bool m_flag18;           // 0x18

		// ghostgamedlg.h:82
		sImgInfo(float x, float y)
			: m_x(x), m_y(y), m_unknown08(0), m_lockOffset(0), m_flag10(false), m_flag11(false), m_flag12(false), m_flag18(false) {
		}
		void AnimateLock(float dt);   // ghostgamedlg.cpp:303
	};

	void Init(unsigned long, const std::string&, const std::list<GHOST_GAME_BRIEF>&);
	void* GetSelectItem(void);   // ghostgamedlg.cpp:341
	virtual void OnProc(const float deltaTime);   // ghostgamedlg.cpp:450
	// ghostgamedlg.h:122
	std::string GetGhostNick(void) { return m_ghostNick; }

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDownCancel(void);
	void OnDownOK(void);     // ghostgamedlg.cpp:494
	void OnInitRecentMatch(intptr_t);
	void OnDownRecentMatch(void);
	void OnInitAreaBg(intptr_t);
	void OnDrawAreaBg(int);
	void OnInitSelMapList(intptr_t);
	void OnDrawSelMapList(intptr_t);
	void OnDownSelMapList(void);
	void OnInitAreaSelecText(intptr_t);
	void OnDrawSelecText(void);
	void OnInitAreaMap(intptr_t);
	void OnDrawChoiceMap(int);

private:
	std::vector<sImgInfo> m_imgInfos;            // 0x110 (one per map of "map_select")
	std::vector<sImgTag> m_imgTags;              // 0x120 (one per map of "map_select")
	std::list<GHOST_GAME_BRIEF> m_gameBriefs;    // 0x130
	unsigned long m_ghostUserId;                 // 0x13c (Init's first argument; OnDownRecentMatch sends it)
	std::string m_string140;                     // 0x140
	std::string m_ghostNick;                     // 0x15c (GetGhostNick)
	WRect m_mapRect;              // 0x178 (rect of "choice_map")
	FrListBox* m_pSelMapList;     // 0x188 ("map_select")
	int m_selectId;               // 0x18c
	FrArea* m_pSelecTextArea;     // 0x190 ("choice_text")
	float m_selecTextAlpha;       // 0x194 (alpha of the "choice_text" tip)
	FrButton* m_pRecentBtn;       // 0x198 ("recent")
	PADDING(0x19c, 0x1a0);        // FrGhostGameMapDlg members
};

CHECK_SIZE(FrGhostGameMapDlg::sImgTag, 0x34);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_pBitmap, 0x00);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_pEndBitmap, 0x04);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_rect, 0x08);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_base, 0x18);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_endY, 0x1c);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_grow, 0x20);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_endH, 0x24);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_current, 0x28);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_alpha, 0x2c);
CHECK_FIELD(FrGhostGameMapDlg::sImgTag, m_alphaEnd, 0x30);
CHECK_SIZE(FrGhostGameMapDlg::sImgInfo, 0x1c);
CHECK_FIELD(FrGhostGameMapDlg::sImgInfo, m_y, 0x04);
CHECK_FIELD(FrGhostGameMapDlg::sImgInfo, m_flag10, 0x10);
CHECK_FIELD(FrGhostGameMapDlg::sImgInfo, m_mapId, 0x14);
CHECK_FIELD(FrGhostGameMapDlg::sImgInfo, m_flag18, 0x18);
CHECK_SIZE(FrGhostGameMapDlg, 0x1a0);
BEGIN_LAYOUT_CHECK(FrGhostGameMapDlg)
	CHECK_FIELD(FrGhostGameMapDlg, m_imgInfos, 0x110);
	CHECK_FIELD(FrGhostGameMapDlg, m_imgTags, 0x120);
	CHECK_FIELD(FrGhostGameMapDlg, m_gameBriefs, 0x130);
	CHECK_FIELD(FrGhostGameMapDlg, m_ghostUserId, 0x13c);
	CHECK_FIELD(FrGhostGameMapDlg, m_string140, 0x140);
	CHECK_FIELD(FrGhostGameMapDlg, m_ghostNick, 0x15c);
	CHECK_FIELD(FrGhostGameMapDlg, m_mapRect, 0x178);
	CHECK_FIELD(FrGhostGameMapDlg, m_pSelMapList, 0x188);
	CHECK_FIELD(FrGhostGameMapDlg, m_selectId, 0x18c);
	CHECK_FIELD(FrGhostGameMapDlg, m_pSelecTextArea, 0x190);
	CHECK_FIELD(FrGhostGameMapDlg, m_selecTextAlpha, 0x194);
	CHECK_FIELD(FrGhostGameMapDlg, m_pRecentBtn, 0x198);
END_LAYOUT_CHECK

// Time bar of the ghost game: a gauge bar form (FrFormBar) counting down the
// click-bonus time; size 0x1a8, the `new` of FrGhostTimeBarMakeInstance.
class FrGhostTimeBar : public FrFormBar {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ghostgamedlg.h:159

	FrGhostTimeBar(void);
	virtual ~FrGhostTimeBar(void);

	virtual bool Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnInitMouseArea(intptr_t);
	void OnDrawMouseArea(void);

private:
	float m_time;               // 0x190 (remaining click time, starts at 4)
	float m_closeDelay;         // 0x194 (delay before closing once the time is out)
	float m_gaugePos;           // 0x198
	bool m_timeOut;             // 0x19c
	FrArea* m_pMouseArea;       // 0x1a0 ("mouse_area")
	const Bitmap* m_pClickImg;  // 0x1a4 (mouse image of the button being clicked)
};

CHECK_SIZE(FrGhostTimeBar, 0x1a8);
BEGIN_LAYOUT_CHECK(FrGhostTimeBar)
	CHECK_FIELD(FrGhostTimeBar, m_time, 0x190);
	CHECK_FIELD(FrGhostTimeBar, m_closeDelay, 0x194);
	CHECK_FIELD(FrGhostTimeBar, m_gaugePos, 0x198);
	CHECK_FIELD(FrGhostTimeBar, m_timeOut, 0x19c);
	CHECK_FIELD(FrGhostTimeBar, m_pMouseArea, 0x1a0);
	CHECK_FIELD(FrGhostTimeBar, m_pClickImg, 0x1a4);
END_LAYOUT_CHECK

// Award screen of the ghost game; its size (0x1f8) is the `new` of
// FrGhostAwardMakeInstance. Only the members used so far are declared.
class FrGhostAward : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ghostgamedlg.h:193

	FrGhostAward(void);
	virtual ~FrGhostAward(void);

	// A pang coin flying into the pouch.
	class cPangFx {
	public:
		cPangFx(void);     // ghostgamedlg.cpp:910
		~cPangFx(void);    // ghostgamedlg.cpp:911
		void Create(const WRect& rect);
		bool IsAlive(void);       // ghostgamedlg.cpp:913
		void Proc(float dt);      // ghostgamedlg.cpp:926
		void Render(FrGraphicInterface* pGI);   // ghostgamedlg.cpp:939

		const Bitmap* m_pBitmap;  // 0x00
		WRect m_rect;             // 0x04
		float m_speed;            // 0x14
		float m_alpha;            // 0x18
		float m_targetY;          // 0x1c
	};

	// One side's result (the player's or the ghost's), filled by SetResultInfo.
	struct sResultInfo {
		std::string m_name;          // 0x00
		WPoint m_pos;                // 0x1c (OnDrawScore: where the score is printed)
		std::string m_levelImage;    // 0x24 ("level_%03d")
		unsigned long m_gameResult;  // 0x40, sPlayerData::m_ghostResult (SetResultInfo)
		unsigned long m_exp;         // 0x44
		bool m_isGhost;              // 0x48 (player index 1)
	};

	virtual void OnProc(const float deltaTime);   // ghostgamedlg.cpp:1099
	void Init(bool);
	bool IsFinishCalcBonus(void);   // ghostgamedlg.cpp:1094
	bool IsLevelUpState(void);      // ghostgamedlg.cpp:1212

	// ghostgamedlg.h:206
	int GetAutoBonus(void) { return m_autoBonus; }
	// ghostgamedlg.h:207
	int GetClickBonus(void) { return m_clickBonus; }
	// ghostgamedlg.h:208
	unsigned char GetClickCount(void) { return m_clickCount; }
	// ghostgamedlg.h:209
	int GetBasePang(void) { return m_basePang; }

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitDescText(intptr_t);
	void OnInitGaugeBar(intptr_t);
	void OnInitGaugeText(intptr_t);
	void OnInitPouchArea(intptr_t);
	void OnDrawScore(int);
	void OnDrawPouch(int);

	void SetResultInfo(sResultInfo& info, int index);
	void AwardProc(float dt);

private:
	FrGhostTimeBar* m_pTimeBar; // 0x110 ("FORM_BAR_GHOST", Init)
	FrArea* m_pPouchArea;    // 0x114 ("bg_pouch")
	FrGaugeBar* m_pExpGauge; // 0x118 ("gauge_exp")
	FrStatic* m_pDescText;   // 0x11c ("text_desc")
	FrStatic* m_pGaugeText;  // 0x120 ("gauge_text")
	PADDING(0x124, 0x12c);   // FrGhostAward members
	std::list<cPangFx*> m_pangFxList;   // 0x12c (deleted by the destructor)
	float m_calcBonusTime;   // 0x138
	unsigned char m_clickCount; // 0x13c (GetClickCount)
	WTitleFont* m_pTitleFont; // 0x140
	int m_autoBonus;         // 0x144 (GetAutoBonus)
	int m_clickBonus;        // 0x148 (GetClickBonus)
	int m_basePang;          // 0x14c (GetBasePang)
	float m_shownBasePang;   // 0x150, counts up to m_basePang (OnProc, OnDrawPouch)
	int m_gainExp;           // 0x154, the experience the game earned (AwardProc stops at 0; IsLevelUpState)
	int m_remainExp;         // 0x158, the experience AwardProc has still to add, one per call
	sResultInfo m_myResult;      // 0x15c
	sResultInfo m_ghostResult;   // 0x1a8
	PADDING(0x1f4, 0x1f8);   // FrGhostAward members
};

// cPangFx: 0x20 covers the members used so far (its allocation is not found yet).
CHECK_SIZE(FrGhostAward::cPangFx, 0x20);
CHECK_FIELD(FrGhostAward::cPangFx, m_rect, 0x04);
CHECK_FIELD(FrGhostAward::cPangFx, m_speed, 0x14);
CHECK_FIELD(FrGhostAward::cPangFx, m_alpha, 0x18);
CHECK_FIELD(FrGhostAward::cPangFx, m_targetY, 0x1c);
CHECK_SIZE(FrGhostAward::sResultInfo, 0x4c);
CHECK_FIELD(FrGhostAward::sResultInfo, m_pos, 0x1c);
CHECK_FIELD(FrGhostAward::sResultInfo, m_levelImage, 0x24);
CHECK_FIELD(FrGhostAward::sResultInfo, m_gameResult, 0x40);
CHECK_FIELD(FrGhostAward::sResultInfo, m_exp, 0x44);
CHECK_FIELD(FrGhostAward::sResultInfo, m_isGhost, 0x48);
CHECK_SIZE(FrGhostAward, 0x1f8);
BEGIN_LAYOUT_CHECK(FrGhostAward)
	CHECK_FIELD(FrGhostAward, m_pTimeBar, 0x110);
	CHECK_FIELD(FrGhostAward, m_pPouchArea, 0x114);
	CHECK_FIELD(FrGhostAward, m_pExpGauge, 0x118);
	CHECK_FIELD(FrGhostAward, m_pDescText, 0x11c);
	CHECK_FIELD(FrGhostAward, m_pGaugeText, 0x120);
	CHECK_FIELD(FrGhostAward, m_pangFxList, 0x12c);
	CHECK_FIELD(FrGhostAward, m_calcBonusTime, 0x138);
	CHECK_FIELD(FrGhostAward, m_clickCount, 0x13c);
	CHECK_FIELD(FrGhostAward, m_pTitleFont, 0x140);
	CHECK_FIELD(FrGhostAward, m_autoBonus, 0x144);
	CHECK_FIELD(FrGhostAward, m_clickBonus, 0x148);
	CHECK_FIELD(FrGhostAward, m_basePang, 0x14c);
	CHECK_FIELD(FrGhostAward, m_shownBasePang, 0x150);
	CHECK_FIELD(FrGhostAward, m_gainExp, 0x154);
	CHECK_FIELD(FrGhostAward, m_remainExp, 0x158);
	CHECK_FIELD(FrGhostAward, m_myResult, 0x15c);
	CHECK_FIELD(FrGhostAward, m_ghostResult, 0x1a8);
END_LAYOUT_CHECK

#endif
