// Original: d:\build\custom\temp\code\client\projectg\fontinfo.h
#ifndef PG_CLIENT_PROJECTG_FONTINFO_H
#define PG_CLIENT_PROJECTG_FONTINFO_H

#include <map>

#include "..\..\defines.h"
#include "wmath.h"
#include "FiniteStateMachine.h"
#include "wfont.h"
#include "wlist.h"
#include "woverlay.h"
#include "contentsdoc.h"
#include "CaddieBonus.h"

class WTitleFont;
class CSimpleUI;

class CFontInfo {
public:
	// A registered font: its title font description and its name.
	struct sFontInfo : public tagWTITLEFONT {
		int m_index;                // 0x20, -1
		char m_name[0x20];          // 0x24
	};

	// A title font and the font description it was created from.
	struct sFont {
		WTitleFont *m_pTitleFont;   // 0x0
		sFontInfo *m_pFontInfo;     // 0x4
	};

	CFontInfo(void);
	virtual ~CFontInfo();
	virtual void Process(float fDelta);                 // fontinfo.h:33
	virtual void Display(void);                         // fontinfo.h:34
	virtual void Init(char *pszName, sFont *pFont);
	virtual void GetScreenPos(float *pfX, float *pfY);  // fontinfo.h:42
	virtual void Reset(void);                           // fontinfo.h:46

	void SetColor(unsigned long color);                 // fontinfo.h:41
	void Clear(void);
	float GetTextWidth(const char *text, float space, float scale);
	float GetExactTextWidth(const char *text, float space, float scale);
	float Print(float x, float y, const char *text, float space, int flag, float scale, sFont *pFont);

protected:
	char *&Buffer(void);                                // fontinfo.h:49

	static bool m_loaded;   // the shared fonts are registered
	static WList<sFontInfo *> m_fontList;   // the registered fonts, by name

	sFont m_font;           // 0x4
	unsigned long m_color;  // 0xc
	int m_nFlag;            // 0x10
	char *m_pBuffer;        // 0x14

private:
	void AddFont(const char *name, int fontw, int fonth, int numPages, int texw, int texh, char *charSet);

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CFontInfo, 0x18);
CHECK_SIZE(CFontInfo::sFont, 0x8);
CHECK_SIZE(CFontInfo::sFontInfo, 0x44);
CHECK_FIELD(CFontInfo::sFontInfo, m_index, 0x20);
CHECK_FIELD(CFontInfo::sFontInfo, m_name, 0x24);
BEGIN_LAYOUT_CHECK(CFontInfo)
	CHECK_FIELD(CFontInfo, m_font, 0x4);
	CHECK_FIELD(CFontInfo, m_color, 0xC);
	CHECK_FIELD(CFontInfo, m_nFlag, 0x10);
	CHECK_FIELD(CFontInfo, m_pBuffer, 0x14);
END_LAYOUT_CHECK

class CDistToHoleInfo : public CFontInfo {
public:
	CDistToHoleInfo(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void Reset(void);
	virtual void ResetUiPosition(void);

	void Fix(void);

private:
	float m_fDistance;        // 0x18, the distance to the cup in yards (Reset)
	int m_nState;           // 0x1c, 5 at construction
	float m_fTime;          // 0x20, seconds spent in the current state (Process, Reset)
	float m_fBlend;         // 0x24, 0 = at the UI position, 1 = at the cup (Display), cleared at construction
	float m_uiX;            // 0x28
	float m_uiY;            // 0x2c
	float m_fScale;         // 0x30, 0.77 at construction
	WVector m_curPos;       // 0x34
	WVector m_fixPos;       // 0x40
	float m_fAlpha;         // 0x4c
	float m_fOffsetX;       // 0x50, added to the cup's screen x (Display)
	PADDING(0x54, 0x58);

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CDistToHoleInfo, 0x58);
BEGIN_LAYOUT_CHECK(CDistToHoleInfo)
	CHECK_FIELD(CDistToHoleInfo, m_fDistance, 0x18);
	CHECK_FIELD(CDistToHoleInfo, m_fTime, 0x20);
	CHECK_FIELD(CDistToHoleInfo, m_nState, 0x1C);
	CHECK_FIELD(CDistToHoleInfo, m_fBlend, 0x24);
	CHECK_FIELD(CDistToHoleInfo, m_fOffsetX, 0x50);
	CHECK_FIELD(CDistToHoleInfo, m_fScale, 0x30);
	CHECK_FIELD(CDistToHoleInfo, m_uiX, 0x28);
	CHECK_FIELD(CDistToHoleInfo, m_uiY, 0x2C);
	CHECK_FIELD(CDistToHoleInfo, m_curPos, 0x34);
	CHECK_FIELD(CDistToHoleInfo, m_fixPos, 0x40);
	CHECK_FIELD(CDistToHoleInfo, m_fAlpha, 0x4C);
END_LAYOUT_CHECK

class CHoleCupInfo : public CFontInfo {
public:
	CHoleCupInfo(void);
	virtual ~CHoleCupInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void GetScreenPos(float *pfX, float *pfY);
	virtual void Reset(void);

	void Fix(void);
	void ResetUiPosition(void);

private:
	float m_screenX;                    // 0x18
	float m_screenY;                    // 0x1c
	float m_fCupHeight;                 // 0x20, the cup's height over the player's ball, in metres (Reset)
	WOverlay *m_pTopHoleOverlay[3];     // 0x24, top_hole_l/m/h
	int m_nDisplayMode;                 // 0x30, 1 at construction
	CDistToHoleInfo *m_pDistToHole;     // 0x34

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CHoleCupInfo)
	CHECK_FIELD(CHoleCupInfo, m_screenX, 0x18);
	CHECK_FIELD(CHoleCupInfo, m_screenY, 0x1C);
	CHECK_FIELD(CHoleCupInfo, m_fCupHeight, 0x20);
	CHECK_FIELD(CHoleCupInfo, m_pTopHoleOverlay, 0x24);
	CHECK_FIELD(CHoleCupInfo, m_nDisplayMode, 0x30);
	CHECK_FIELD(CHoleCupInfo, m_pDistToHole, 0x34);
END_LAYOUT_CHECK

class CTimeOutInfo : public CFontInfo {
public:
	CTimeOutInfo(void);

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void Reset(void);

private:
	int m_nState;           // 0x18
	float m_fAlpha;         // 0x1c
	float m_fTime;          // 0x20

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CTimeOutInfo)
	CHECK_FIELD(CTimeOutInfo, m_nState, 0x18);
	CHECK_FIELD(CTimeOutInfo, m_fAlpha, 0x1C);
	CHECK_FIELD(CTimeOutInfo, m_fTime, 0x20);
END_LAYOUT_CHECK

class CShotCountInfo : public CFontInfo {
public:
	CShotCountInfo(void);
	virtual ~CShotCountInfo();

	virtual void Process(float fDelta);

	void StartFloating(void);

private:
	WOverlay *m_pCoinOverlay;   // 0x18, skins games only
	unsigned char m_alpha;      // 0x1c
	PADDING(0x1D, 0x20);
	float m_fFloatTime;         // 0x20
	int m_nFloatCount;          // 0x24
	sFont m_redFont;            // 0x28, skins games only
	sFont m_greyFont;           // 0x30
	sFont m_blueFont;           // 0x38

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CShotCountInfo)
	CHECK_FIELD(CShotCountInfo, m_pCoinOverlay, 0x18);
	CHECK_FIELD(CShotCountInfo, m_redFont, 0x28);
	CHECK_FIELD(CShotCountInfo, m_greyFont, 0x30);
	CHECK_FIELD(CShotCountInfo, m_blueFont, 0x38);
	CHECK_FIELD(CShotCountInfo, m_alpha, 0x1C);
	CHECK_FIELD(CShotCountInfo, m_fFloatTime, 0x20);
	CHECK_FIELD(CShotCountInfo, m_nFloatCount, 0x24);
END_LAYOUT_CHECK

class CHoleInfo : public CFontInfo {
public:
	CHoleInfo(void);
	virtual ~CHoleInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void Reset(void);

	void Flicker(void);
	void Fix(void);
	void ResetUiPosition(void);

private:
	struct sItem {
		float baseX;        // 0x0
		float baseY;        // 0x4
		float curX;         // 0x8, offset from the base
		float curY;         // 0xc
		float fixX;         // 0x10
		float fixY;         // 0x14
		float scale;        // 0x18
	};

	sItem m_items[4];       // 0x18, [3] is the pang count (base = screen width, 110; cur = left edge, 5)
	float m_fAlpha;         // 0x88
	float m_fTargetAlpha;   // 0x8c
	float m_fTime;          // 0x90, the state's elapsed time (0..1 while sliding)
	int m_nFlickerTime;     // 0x94
	int m_nState;           // 0x98, 4 = flickering
	CSimpleUI *m_pCourseUI;             // 0x9c, [start_name.jpg, one piece per course
	CSimpleUI *m_pHoleUI;               // 0xa0, [start_hole.png
	CSimpleUI *m_pStartPosiUI;          // 0xa4, [start_posi.jpg
	CSimpleUI *m_pPlayerNumUI;          // 0xa8, [nplayer.jpg
	CSimpleUI *m_pEmotiUI;              // 0xac, [emoti_object.png, skins games only
	WOverlay *m_pShuffleBonusOverlay;   // 0xb0, text_shufflebonus.png
	WOverlay *m_pCompleteOverlay;       // 0xb4, [complete.png
	WOverlay *m_pScoreEventOverlay;     // 0xb8, [score_event.png
	WOverlay *m_pExtraStageOverlay;     // 0xbc, extrastage.png, skins games only
	WOverlay *m_pExtraStageSmallOverlay;// 0xc0, extrastagesmall.png, skins games only
	WOverlay *m_pCoin1Overlay;          // 0xc4, P_coin01.png, skins games only
	WOverlay *m_pCoin2Overlay;          // 0xc8, P_coin02.png
	WOverlay *m_pCoin3Overlay;          // 0xcc, P_coin03.png
	WOverlay *m_pMapBonusOverlay;       // 0xd0, text_mapbonus.png
	WOverlay *m_pSkinsPang1Overlay;     // 0xd4, [skins_pang01.png, skins games only
	WOverlay *m_pSkinsPang3Overlay;     // 0xd8, [skins_pang03.png
	WOverlay *m_pApproachOverlay;       // 0xdc, text_approach.png, approach games only
	unsigned char m_firstPlayer;    // 0xe0, the first playing player not known yet (Reset)
	int m_nEmoti;                       // 0xe4, -1

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CHoleInfo)
	CHECK_FIELD(CHoleInfo, m_firstPlayer, 0xE0);
	CHECK_FIELD(CHoleInfo, m_items, 0x18);
	CHECK_FIELD(CHoleInfo, m_fAlpha, 0x88);
	CHECK_FIELD(CHoleInfo, m_fTime, 0x90);
	CHECK_FIELD(CHoleInfo, m_nFlickerTime, 0x94);
	CHECK_FIELD(CHoleInfo, m_nState, 0x98);
	CHECK_FIELD(CHoleInfo, m_pCourseUI, 0x9C);
	CHECK_FIELD(CHoleInfo, m_pApproachOverlay, 0xDC);
	CHECK_FIELD(CHoleInfo, m_nEmoti, 0xE4);
END_LAYOUT_CHECK

class CAccuracyInfo : public CFontInfo {
public:
	CAccuracyInfo(void);
	virtual ~CAccuracyInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);

	void SetPower(void);
	void SetAccuracy(void);

private:
	char *&Buffer2(void);                               // fontinfo.h:458

	bool m_bFlicker;        // 0x18
	PADDING(0x19, 0x1C);
	float m_fAlpha;         // 0x1c
	CSimpleUI *m_pImpactUI; // 0x20
	float m_fTextX;         // 0x24
	char *m_pBuffer2;       // 0x28

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CAccuracyInfo)
	CHECK_FIELD(CAccuracyInfo, m_bFlicker, 0x18);
	CHECK_FIELD(CAccuracyInfo, m_fAlpha, 0x1C);
	CHECK_FIELD(CAccuracyInfo, m_pImpactUI, 0x20);
	CHECK_FIELD(CAccuracyInfo, m_fTextX, 0x24);
	CHECK_FIELD(CAccuracyInfo, m_pBuffer2, 0x28);
END_LAYOUT_CHECK

// The guild match score board: both guilds' scores, counting up to the target.
class CGuildInfo : public CFontInfo {
public:
	CGuildInfo(void);
	virtual ~CGuildInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);

	void SetScore(int nScore1, int nScore2);

protected:
	char *&Buffer2(void);                               // fontinfo.h:493

	PADDING(0x18, 0x1C);
	float m_fTime;              // 0x1c, since the last score change
	float m_fScale;             // 0x20, of the score digits
	sFont m_leftFont;           // 0x24
	sFont m_rightFont;          // 0x2c
	char *m_pBuffer2;           // 0x34
	int m_targetScore[2];       // 0x38
	int m_displayScore[2];      // 0x40, counts up to m_targetScore
	const char *m_pszSound;     // 0x48, played on a score change

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CGuildInfo)
	CHECK_FIELD(CGuildInfo, m_fTime, 0x1C);
	CHECK_FIELD(CGuildInfo, m_fScale, 0x20);
	CHECK_FIELD(CGuildInfo, m_leftFont, 0x24);
	CHECK_FIELD(CGuildInfo, m_rightFont, 0x2C);
	CHECK_FIELD(CGuildInfo, m_pBuffer2, 0x34);
	CHECK_FIELD(CGuildInfo, m_targetScore, 0x38);
	CHECK_FIELD(CGuildInfo, m_displayScore, 0x40);
	CHECK_FIELD(CGuildInfo, m_pszSound, 0x48);
END_LAYOUT_CHECK

class CClubRangeInfo : public CFontInfo {
public:
	CClubRangeInfo(void);

	virtual void Display(void);

private:
	WVector m_vPos;         // 0x18, projected to the screen

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CClubRangeInfo)
	CHECK_FIELD(CClubRangeInfo, m_vPos, 0x18);
END_LAYOUT_CHECK

class CPowerInfo : public CFontInfo {
public:
	CPowerInfo(void);

	virtual void Display(void);

private:
	WVector m_vPos;         // 0x18, the point whose distance from the ball is shown

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CPowerInfo)
	CHECK_FIELD(CPowerInfo, m_vPos, 0x18);
END_LAYOUT_CHECK

class CWindInfo : public CFontInfo {
public:
	CWindInfo(void);

	virtual void Display(void);
};

// The pang bonuses of the local player's round (add_pang.png): award,
// extra, aux-part and item pangs and their total, computed by Process.
// Size 0x60 assumed from the highest member its methods touch (0x5c).
class CBonusInfo : public CFontInfo {
public:
	CBonusInfo(void);
	virtual ~CBonusInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);

private:
	WOverlay *m_pPangOverlay;       // 0x18, add_pang.png (add_pang_skins.png in room type 7)
	WOverlay *m_pPcBangOverlay;     // 0x1c, pc_eventmark.tga, PC bang only
	float m_fTime;                  // 0x20, Process adds the frame time
	PADDING(0x24, 0x28);
	__int64 m_awardPang;            // 0x28, sPlayerData::m_ghostAward (Process)
	__int64 m_extraPang;            // 0x30, sPlayerData::m_cryptValue4F8 (Process)
	__int64 m_auxPartPang;          // 0x38, aux part property 3 applied to the two above (Process)
	__int64 m_itemPang;             // 0x40, sPlayerData 0xb78 (Process)
	__int64 m_totalPang;            // 0x48, the sum of the four above (Process)
	std::map<unsigned long, WOverlay *> m_overlays; // 0x50
	WOverlay *m_pEventOverlay;      // 0x5c, z_Halloween_Male.tga (local content 0x17); not set otherwise

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(CBonusInfo, 0x60);
BEGIN_LAYOUT_CHECK(CBonusInfo)
	CHECK_FIELD(CBonusInfo, m_pPangOverlay, 0x18);
	CHECK_FIELD(CBonusInfo, m_pPcBangOverlay, 0x1C);
	CHECK_FIELD(CBonusInfo, m_fTime, 0x20);
	CHECK_FIELD(CBonusInfo, m_awardPang, 0x28);
	CHECK_FIELD(CBonusInfo, m_extraPang, 0x30);
	CHECK_FIELD(CBonusInfo, m_auxPartPang, 0x38);
	CHECK_FIELD(CBonusInfo, m_itemPang, 0x40);
	CHECK_FIELD(CBonusInfo, m_totalPang, 0x48);
	CHECK_FIELD(CBonusInfo, m_overlays, 0x50);
	CHECK_FIELD(CBonusInfo, m_pEventOverlay, 0x5C);
END_LAYOUT_CHECK

class CRemainTimeInfo : public CFontInfo {
public:
	CRemainTimeInfo(void);

	virtual void Display(void);
};

class CTotalDistInfo : public CFontInfo {
public:
	CTotalDistInfo(void);

	virtual void Display(void);

private:
	float m_fDistance;      // 0x18, ball to cup, in yards

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CTotalDistInfo)
	CHECK_FIELD(CTotalDistInfo, m_fDistance, 0x18);
END_LAYOUT_CHECK

class CClubTypeInfo : public CFontInfo {
public:
	CClubTypeInfo(void);

	virtual void Display(void);
};

class CCalipers : public CFontInfo {
public:
	CCalipers(void);
	virtual ~CCalipers();

	virtual void Process(float fDelta);
	virtual void Display(void);
	virtual void Reset(void);

	void VisiblePowerBar(int nState);                   // fontinfo.h:796
	void SetEvent(int nEvent);
	void MoveNone(void);
	void MoveLeft(void);
	void MoveRight(void);
	void KeyInputEnd(void);
	void ResetBar(void);
	void ExecuteKeyAction(int nKey);
	void ChangeKeyState(void);
	void SetCalipersLimit(float fLimit);
	void DrawShotPowerRange(float &fPos, bool bImpact);
	void DrawLimitLine(float &fPos, bool bImpact);
	void ProcKeyState(float fDelta);
	bool IsVisiblePowerBar(void) const;                 // fontinfo.h:795

private:
	FiniteStateMachine m_fsm;           // 0x18
	float m_fCursorPos;                 // 0x2c
	float m_fDisplayPos;                // 0x30
	unsigned long m_prevKeyFlags;       // 0x34, m_keyFlags of the last frame
	unsigned long m_keyFlags;           // 0x38, 1 = left key, 2 = right key
	unsigned long m_prevState;          // 0x3c
	unsigned long m_curState;           // 0x40
	bool m_bPowerFixed;                 // 0x44
	bool m_bAccuracyFixed;              // 0x45
	PADDING(0x46, 0x48);
	float m_fBarScale;                  // 0x48
	float m_fMoveSpeed;                 // 0x4c
	int m_nPowerBarState;               // 0x50, 1 = power bar visible
	WOverlay *m_pIndicatorOverlay;      // 0x54, indicator.bmp
	WOverlay *m_pCalipersOverlay;       // 0x58, callipers_o.png
	bool m_bKeyMoving;                  // 0x5c
	PADDING(0x5D, 0x60);
	float m_fMoveDelta;                 // 0x60, the cursor's pending move (Process); Display shows the arrows from it, cleared by Reset
	float m_fSoundTime;                 // 0x64, time since the bar sound played (Process clears m_bSoundPlayed past 0.4s)
	bool m_bSoundPlayed;                // 0x68, the bar sound played (Process)
	bool m_bResetFlag;                  // 0x69, set by Reset and at construction

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CCalipers)
	CHECK_FIELD(CCalipers, m_fMoveDelta, 0x60);
	CHECK_FIELD(CCalipers, m_bResetFlag, 0x69);
	CHECK_FIELD(CCalipers, m_fSoundTime, 0x64);
	CHECK_FIELD(CCalipers, m_bSoundPlayed, 0x68);
	CHECK_FIELD(CCalipers, m_fsm, 0x18);
	CHECK_FIELD(CCalipers, m_fCursorPos, 0x2C);
	CHECK_FIELD(CCalipers, m_prevKeyFlags, 0x34);
	CHECK_FIELD(CCalipers, m_keyFlags, 0x38);
	CHECK_FIELD(CCalipers, m_prevState, 0x3C);
	CHECK_FIELD(CCalipers, m_curState, 0x40);
	CHECK_FIELD(CCalipers, m_bPowerFixed, 0x44);
	CHECK_FIELD(CCalipers, m_fBarScale, 0x48);
	CHECK_FIELD(CCalipers, m_fMoveSpeed, 0x4C);
	CHECK_FIELD(CCalipers, m_nPowerBarState, 0x50);
	CHECK_FIELD(CCalipers, m_pIndicatorOverlay, 0x54);
	CHECK_FIELD(CCalipers, m_pCalipersOverlay, 0x58);
	CHECK_FIELD(CCalipers, m_bKeyMoving, 0x5C);
END_LAYOUT_CHECK

// The overlay of a pang result picture and its size.
struct PangOverlayInfo {
	unsigned int m_overlay;     // 0x0
	unsigned int m_height;      // 0x4
	unsigned int m_width;       // 0x8
};
CHECK_SIZE(PangOverlayInfo, 0xC);

extern PangOverlayInfo g_holeInPangOverlayInfo[];
extern PangOverlayInfo g_specialShotPangOverlayInfo[][4];

class CPangInfo : public CFontInfo {
public:
	struct sPangCase {
		sPangCase(void);

		void SetState(unsigned long state) {
			m_flags = (m_flags & 0xFFFFF000) | state;
			m_fTime = 0;
		}

		union {
			unsigned long m_flags;          // 0x0, 0x100 = case closed
			struct {
				unsigned int m_state : 12;  // 0x0
			};
		};
		PADDING(0x4, 0x8);
		__int64 m_pang;                     // 0x8, the pangs the case shows
		int m_nIndex;                       // 0x10, -1 = none (ResetVars)
		float m_fTime;                      // 0x14, seconds since the case was set (Process adds the frame time while it runs)
		float m_fY;                         // 0x18, screen row (ResetUiPosition)
		float m_fTargetY;                   // 0x1c, the row the case slides to (Insert)
		PADDING(0x20, 0x34);
		unsigned char m_byDone;             // 0x34, cleared by ResetVars
		PADDING(0x35, 0x38);
	};

	CPangInfo(void);
	virtual ~CPangInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);

	void Skip(void);
	void Restore(void);

protected:
	void ResetVars(int nFlags, int nCaseState);
	void ProcessHappyBonus(float fDelta);
	void ProcessAztecBonus(float fDelta);
	void ProcessAngelBonus(float fDelta);
	void ProcessDevilBonus(float fDelta);
	void Insert(int nCase);
	void UpdateBallState(void);
	void Approach(void);
	void LongPutt(void);
	void Multiply(int nCase, int nMultiply);
	void SuperPangya(void);
	void BeamImpact(void);
	void SpecialShot(void);
	void NiceRecovery(void);
	void HoleIn(void);
	void DormieBonus(void);
	void HappyBonus(void);
	void AztecBonus(void);
	void ClearBonus(void);
	void AngelWingBonus(void);
	void DevilWingBonus(void);
	void MascotBonus(void);
	void AbilityItemBonus(void);
	void RecoveryBonus(void);
	void MakeDecision(int nCase, bool bSuccess);
	void GetHoleInOverlayInfo(int nScore, int nPar, unsigned int &overlay, float &fWidth, float &fHeight);
	void GetSpecialShotOverlayInfo(unsigned long flags, unsigned char index, unsigned int &overlay, float &fWidth, float &fHeight);
	__int64 GetHoleInPang(int nScore, int nPar);
	void LongDrive(void);
	void ProcessTotal(float fDelta);
	void ProcessCases(float fDelta);
	void CheckReadyToMakeDecision(void);
	void CheckReadyToFinalize(float fDelta);
	void LongChipIn(void);
	void ClashCombo(void);
	void AztecBonusWater(void);
	void GimmickItem(GIMMICK_ITEM_TYPE itemType);

public:
	void ResetUiPosition(void);

private:
	// The pangs of a hole-in per course, by score against par.
	struct sHoleInPang {
		__int64 m_holeInOne;            // 0x0
		__int64 m_albatross;            // 0x8, par - 3
		__int64 m_eagle;                // 0x10, par - 2
		__int64 m_birdie;               // 0x18, par - 1
		__int64 m_par;                  // 0x20
		__int64 m_perStroke;            // 0x28, times the score against par otherwise
	};

	WOverlay *m_pPangOverlay[2];        // 0x18, g_pangOverlayFilenames
	unsigned long m_flags;              // 0x20, 0x4 = deciding the cases, 0x8 = bonus timers running
	const char *m_pszSfx;               // 0x24, alias of the sound playing (Skip stops it)
	__int64 m_pangs[4];                 // 0x28, cleared by ResetVars
	__int64 m_bonusPangs[4];            // 0x48, added to m_pangs by Skip
	__int64 m_shownPangs[4];           // 0x68, set from m_bonusPangs by Restore
	__int64 m_extraPangs[4];            // 0x88, cleared by ResetVars
	sPangCase m_totalCases[4];          // 0xa8, per player, the totals Skip jumps to
	sPangCase m_cases[24];              // 0x188, indexed by case id
	WVector m_aimDir;                   // 0x6c8, LongPutt compares the putt's direction with it
	int m_nPangRate;                    // 0x6d4, Approach's pang per yard; Multiply's hole-in decision needs it <= 0
	PADDING(0x6D8, 0x6E0);
	float m_fLongestDrive;              // 0x6e0
	float m_fStartToPin;                // 0x6e4, Approach halves its pang below 60
	float m_fDriveDistance;             // 0x6e8, LongDrive pays the excess over m_fLongestDrive
	float m_fToPin;                     // 0x6ec, ball to cup (UpdateBallState), Approach pays 10 minus it
	int m_nState;                       // 0x6f0, cleared by ResetVars
	int m_nSubState;                    // 0x6f4, cleared by ResetVars
	float m_fPopupX;                    // 0x6f8, set by Insert for cases 12, 16 and 18
	float m_fPopupY;                    // 0x6fc
	float m_uiPos[8][2];                // 0x700, (x, y) of the result items (ResetUiPosition)
	float m_fHappyBonusTime;            // 0x740
	float m_fAztecBonusTime;            // 0x744
	float m_fAngelBonusTime;            // 0x748
	float m_fDevilBonusTime;            // 0x74c
	int m_nBonusCount;                  // 0x750, cleared by ResetVars
	PADDING(0x754, 0x780);
	int m_nShowCount;                   // 0x780, cleared by ResetVars
	int m_nShowCase;                    // 0x784, -1 = none (ResetVars)
	PADDING(0x788, 0x79C);
	CCaddieBonus m_caddieBonus;         // 0x79c
	sHoleInPang m_holeInPangs[20];      // 0x7a8, indexed by course

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CPangInfo)
	CHECK_FIELD(CPangInfo, m_pPangOverlay, 0x18);
	CHECK_FIELD(CPangInfo, m_fLongestDrive, 0x6E0);
	CHECK_FIELD(CPangInfo, m_fDriveDistance, 0x6E8);
	CHECK_FIELD(CPangInfo, m_uiPos, 0x700);
	CHECK_FIELD(CPangInfo, m_caddieBonus, 0x79C);
	CHECK_FIELD(CPangInfo, m_holeInPangs, 0x7A8);
	CHECK_FIELD(CPangInfo, m_flags, 0x20);
	CHECK_FIELD(CPangInfo, m_pangs, 0x28);
	CHECK_FIELD(CPangInfo, m_pszSfx, 0x24);
	CHECK_FIELD(CPangInfo, m_bonusPangs, 0x48);
	CHECK_FIELD(CPangInfo, m_extraPangs, 0x88);
	CHECK_FIELD(CPangInfo, m_totalCases, 0xA8);
	CHECK_FIELD(CPangInfo, m_cases, 0x188);
	CHECK_FIELD(CPangInfo, m_nState, 0x6F0);
	CHECK_FIELD(CPangInfo, m_shownPangs, 0x68);
	CHECK_FIELD(CPangInfo, m_aimDir, 0x6C8);
	CHECK_FIELD(CPangInfo, m_nPangRate, 0x6D4);
	CHECK_FIELD(CPangInfo, m_fStartToPin, 0x6E4);
	CHECK_FIELD(CPangInfo, m_fToPin, 0x6EC);
	CHECK_FIELD(CPangInfo, m_nSubState, 0x6F4);
	CHECK_FIELD(CPangInfo, m_fPopupX, 0x6F8);
	CHECK_FIELD(CPangInfo, m_fPopupY, 0x6FC);
	CHECK_FIELD(CPangInfo, m_nBonusCount, 0x750);
	CHECK_FIELD(CPangInfo, m_nShowCase, 0x784);
	CHECK_FIELD(CPangInfo, m_fHappyBonusTime, 0x740);
	CHECK_FIELD(CPangInfo, m_fDevilBonusTime, 0x74C);
END_LAYOUT_CHECK

CHECK_SIZE(CPangInfo::sPangCase, 0x38);
CHECK_FIELD(CPangInfo::sPangCase, m_fY, 0x18);
CHECK_FIELD(CPangInfo::sPangCase, m_fTargetY, 0x1C);

class CFlyInfo : public CFontInfo {
public:
	CFlyInfo(void);
	virtual ~CFlyInfo();

	virtual void Process(float fDelta);
	virtual void Display(void);

private:
	void CalcDistance(void);

	float m_fTime;                      // 0x18, since the ball landed (Process)
	CSimpleUI *m_pGroundUI;             // 0x1c, [ground.jpg]
	CSimpleUI *m_pGroundTextUI;         // 0x20, [ground_text.jpg]
	WOverlay *m_pApproachOutOverlay;    // 0x24, approach_out.png
	float m_fGroundAlpha;               // 0x28, fades in the ground type picture
	float m_fCarryAlpha;                // 0x2c
	float m_fToPinAlpha;                // 0x30
	float m_fCarry;                     // 0x34, yards from the shot's start to the ball
	float m_fToPin;                     // 0x38, yards from the ball to the cup
	char m_szGround[8];                 // 0x3c, name of the ground type under the ball

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CFlyInfo)
	CHECK_FIELD(CFlyInfo, m_fTime, 0x18);
	CHECK_FIELD(CFlyInfo, m_fGroundAlpha, 0x28);
	CHECK_FIELD(CFlyInfo, m_fCarryAlpha, 0x2C);
	CHECK_FIELD(CFlyInfo, m_fToPinAlpha, 0x30);
	CHECK_FIELD(CFlyInfo, m_szGround, 0x3C);
	CHECK_FIELD(CFlyInfo, m_pGroundUI, 0x1C);
	CHECK_FIELD(CFlyInfo, m_pGroundTextUI, 0x20);
	CHECK_FIELD(CFlyInfo, m_pApproachOutOverlay, 0x24);
	CHECK_FIELD(CFlyInfo, m_fCarry, 0x34);
	CHECK_FIELD(CFlyInfo, m_fToPin, 0x38);
END_LAYOUT_CHECK

// The overtime holes' board.
class CExtraHoleInfo : public CFontInfo {
public:
	CExtraHoleInfo(void);
	virtual ~CExtraHoleInfo();

	virtual void Display(void);

	void SetShotTime(unsigned long time);
	void UpdateLeader(void);

private:
	unsigned long m_shotTime[4];        // 0x18, per player
	CSimpleUI *m_pPlayerNumUI;          // 0x28, [nplayer.jpg]
	unsigned char m_leader;             // 0x2c, the player nearest the cup, 0xff = none yet
	unsigned long m_cupDistance[4];     // 0x30, per player: distance to the cup in 1/100 yard

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CExtraHoleInfo)
	CHECK_FIELD(CExtraHoleInfo, m_shotTime, 0x18);
	CHECK_FIELD(CExtraHoleInfo, m_pPlayerNumUI, 0x28);
	CHECK_FIELD(CExtraHoleInfo, m_leader, 0x2C);
	CHECK_FIELD(CExtraHoleInfo, m_cupDistance, 0x30);
END_LAYOUT_CHECK

// The skins game's hole result picture (win, lose or draw).
class CHolePoseSkins : public CFontInfo {
public:
	CHolePoseSkins(void);
	virtual ~CHolePoseSkins();

	virtual void Process(float fDelta);
	virtual void Display(void);

	void SetPose(int nResult, int nValue);

private:
	WOverlay *m_pWinOverlay[3];     // 0x18, skins_win01..03.jpg
	WOverlay *m_pLoseOverlay[2];    // 0x24, skins_lose01..02.jpg
	WOverlay *m_pDrawOverlay[2];    // 0x2c, skins_draw01..02.jpg
	WOverlay *m_pPoseOverlay[2];    // 0x34, the pictures shown
	int m_nValue;                   // 0x3c
	int m_nResult;                  // 0x40, > 0 win, 0 draw, < 0 lose
	float m_fAlpha;                 // 0x44
	float m_fScale;                 // 0x48
	int m_nState;                   // 0x4c, 0 = idle, 1 = showing

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CHolePoseSkins)
	CHECK_FIELD(CHolePoseSkins, m_pWinOverlay, 0x18);
	CHECK_FIELD(CHolePoseSkins, m_pLoseOverlay, 0x24);
	CHECK_FIELD(CHolePoseSkins, m_pDrawOverlay, 0x2C);
	CHECK_FIELD(CHolePoseSkins, m_pPoseOverlay, 0x34);
	CHECK_FIELD(CHolePoseSkins, m_nValue, 0x3C);
	CHECK_FIELD(CHolePoseSkins, m_nResult, 0x40);
	CHECK_FIELD(CHolePoseSkins, m_fAlpha, 0x44);
	CHECK_FIELD(CHolePoseSkins, m_fScale, 0x48);
	CHECK_FIELD(CHolePoseSkins, m_nState, 0x4C);
END_LAYOUT_CHECK

struct sPlayerData;

int GetAngelBonus(sPlayerData *player);

class CFontOverlay {
public:
	CFontOverlay(void);
	~CFontOverlay();

	void Init(void);
	void Process(float fDelta);
	void Display(void);
	void ResetUiPosition(void);

private:
	CHoleCupInfo *m_pHoleCupInfo;       // 0x0
	CFlyInfo *m_pFlyInfo;               // 0x4
	CHoleInfo *m_pHoleInfo;             // 0x8
	CAccuracyInfo *m_pAccuracyInfo;     // 0xc
	CTimeOutInfo *m_pTimeOutInfo;       // 0x10
	bool m_bShowHoleCup;                // 0x14
	bool m_bShowFly;                    // 0x15
	bool m_bShowHole;                   // 0x16
	bool m_bShowAccuracy;               // 0x17
	bool m_bShowTimeOut;                // 0x18

	LAYOUT_CHECK_FRIEND;
};

BEGIN_LAYOUT_CHECK(CFontOverlay)
	CHECK_FIELD(CFontOverlay, m_pHoleCupInfo, 0x0);
	CHECK_FIELD(CFontOverlay, m_pFlyInfo, 0x4);
	CHECK_FIELD(CFontOverlay, m_pHoleInfo, 0x8);
	CHECK_FIELD(CFontOverlay, m_pAccuracyInfo, 0xC);
	CHECK_FIELD(CFontOverlay, m_pTimeOutInfo, 0x10);
	CHECK_FIELD(CFontOverlay, m_bShowHoleCup, 0x14);
	CHECK_FIELD(CFontOverlay, m_bShowTimeOut, 0x18);
END_LAYOUT_CHECK

#endif
