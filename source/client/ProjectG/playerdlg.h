// Original: d:\build\custom\temp\code\client\projectg\playerdlg.h
#ifndef PG_CLIENT_PROJECTG_PLAYERDLG_H
#define PG_CLIENT_PROJECTG_PLAYERDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "fresh.h"
#include <list>
#include <vector>

struct IFF_ITEM_COMMON;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class FrGaugeBar;
class Bitmap;
class CExhibition;
class CPartTidList;
enum eLvlType;
enum PANGYA_ITEM_GROUP;

// Status bar of the player dialog (one bar per level type). Only the members
// this unit defines so far are declared.
class CStatBar {
	LAYOUT_CHECK_FRIEND;
public:
	// One bar of the status bar.
	struct sBar {
		sBar(const char* name) {   // playerdlg.h:174
			m_curPos = 0.0f;
			m_destPos = 0.0f; m_rangeLimit = 0.0f;
			m_pBitmap = g_pFresh->GetBitmap(name);
		}

		const Bitmap* m_pBitmap;   // 0x0, set from the bitmap name by the constructor
		float m_curPos;            // 0x4, moves towards m_destPos (Process)
		float m_destPos;           // 0x8, never below 0 (SetPos, SetRange)
		float m_rangePos;          // 0xC, clamped to m_rangeLimit by SetRange
		float m_rangeLimit;        // 0x10
		int m_maxValue;            // 0x14, the third value of SetPos/SetRange
	};

	void SetRange(eLvlType type, int pos, int range, int maxValue);   // playerdlg.cpp:64
	void SetPos(eLvlType type, int pos, int range, int maxValue);     // playerdlg.cpp:113
	void Render(const WPoint& pos);     // playerdlg.cpp:120

	CStatBar(float barGap, float barHeight);   // playerdlg.cpp:40
	~CStatBar(void);                    // playerdlg.cpp:58
	void SetDestTime(float destTime);   // playerdlg.cpp:72
	void Process(float dt);

private:
	std::vector<sBar> m_bars;   // 0x0
	float m_barGap;     // 0x10, vertical gap between two bars (Render)
	float m_destTime;   // 0x14
	float m_barHeight;  // 0x18, height of one bar (Render)
};
CHECK_SIZE(CStatBar, 0x1C);
CHECK_SIZE(CStatBar::sBar, 0x18);
CHECK_FIELD(CStatBar::sBar, m_curPos, 0x4);
CHECK_FIELD(CStatBar::sBar, m_destPos, 0x8);
CHECK_FIELD(CStatBar::sBar, m_rangePos, 0xC);
CHECK_FIELD(CStatBar::sBar, m_rangeLimit, 0x10);
CHECK_FIELD(CStatBar::sBar, m_maxValue, 0x14);
BEGIN_LAYOUT_CHECK(CStatBar)
	CHECK_FIELD(CStatBar, m_bars, 0x0);
	CHECK_FIELD(CStatBar, m_barGap, 0x10);
	CHECK_FIELD(CStatBar, m_destTime, 0x14);
	CHECK_FIELD(CStatBar, m_barHeight, 0x18);
END_LAYOUT_CHECK

// Equipment selection bar of the player dialog. Only the members this
// project's code uses so far are declared.
class FrSubEquipBar : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	struct _sortitem {
		_sortitem(IFF_ITEM_COMMON* pItem, unsigned long guid) : m_pItem(pItem), m_guid(guid) {}   // playerdlg.h:102

		IFF_ITEM_COMMON* m_pItem;   // 0x0
		unsigned long m_guid;       // 0x4
	};

	// Item category shown by the bar; enumerators not recovered yet.
	enum eCategory {
	};

	// Sub type of the bar; enumerators not recovered yet.
	enum eSubType {
	};

	void Open(FRESH_RESULT_CALLBACK pfnResult, eCategory category, const WPoint& pt, eSubType subType);   // playerdlg.cpp:657

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrSubEquipBar(void);            // playerdlg.cpp:156
	virtual ~FrSubEquipBar(void);   // playerdlg.cpp:161

	IFF_ITEM_COMMON *GetResult(void);          // playerdlg.h:120
	unsigned long GetLastSelectedGuid(void);   // playerdlg.h:121
	eCategory GetCategory(void);               // playerdlg.h:122
	bool SetClientEquipInfo(PANGYA_ITEM_GROUP group, unsigned long guid);

protected:
	static sFRESH_MSGMAP _MsgMap;
	static sFRESH_ENTRY _MsgEntries[];
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // playerdlg.cpp:147

	void OnInitSubEquipList(intptr_t param);
	void OnDownSubEquipList(int param);
	void OnDrawSubEquipList(int param);

	virtual void OnDraw(void);     // playerdlg.cpp:629
	virtual void OnProc(const float dt);   // playerdlg.cpp:634
	virtual void OnOK(void);       // playerdlg.cpp:647
	virtual void OnCancel(void);   // playerdlg.cpp:652

private:
	std::list<_sortitem> m_sortList;   // 0x110
	IFF_ITEM_COMMON *m_result;     // 0x11C
	FrListBox* m_pSubEquipList;    // 0x120, "sub_equip"
	void Initialize(void);

	eCategory m_category;          // 0x124
	eSubType m_subType;            // 0x128
	unsigned long m_lastSelectedGuid;  // 0x12C
};
BEGIN_LAYOUT_CHECK(FrSubEquipBar)
	CHECK_FIELD(FrSubEquipBar, m_sortList, 0x110);
	CHECK_FIELD(FrSubEquipBar, m_result, 0x11C);
	CHECK_FIELD(FrSubEquipBar, m_pSubEquipList, 0x120);
	CHECK_FIELD(FrSubEquipBar, m_category, 0x124);
	CHECK_FIELD(FrSubEquipBar, m_subType, 0x128);
	CHECK_FIELD(FrSubEquipBar, m_lastSelectedGuid, 0x12C);
END_LAYOUT_CHECK

// Singleton helper that picks the equipment of the player dialog.
class CEquipHelper {
private:
	CEquipHelper(void);    // playerdlg.cpp:741
	~CEquipHelper(void);   // playerdlg.cpp:746
};

// Player (character selection) dialog. Only the members this unit defines so
// far are declared.
class FrPlayerDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // playerdlg.h:207

	FrPlayerDlg(void) : m_pExhibition(0), m_pPartTidList(0), m_team(2), m_playerIndex(-1), m_pLinkedWnd(0) {}   // playerdlg.h:210

	void Init(int index);          // playerdlg.cpp:1104
	void OnCustomProc(float dt);   // playerdlg.cpp:1173
	void SetTeam(unsigned char team);   // playerdlg.cpp:1185
	void Select(void);     // playerdlg.cpp:1224
	void UnSelect(void);   // playerdlg.cpp:1230
	virtual ~FrPlayerDlg(void);   // playerdlg.cpp:1099
	void HidePet(void);           // playerdlg.cpp:1142
	const char* GetName(void);   // playerdlg.cpp:1378
	void SetName(const char* name);   // playerdlg.cpp:1386
	void SetState(void);         // playerdlg.cpp:1148
	void SetRandomState(void);   // playerdlg.cpp:1162
	void ShowPet(bool show);

protected:
	// Direction of the caddie browse buttons.
	enum eButtonDir {
		eButtonDirPrev = 0,
		eButtonDirNext = 1,
		eButtonDirCurrent = 2,   // SetState: show the player's own selection
		eButtonDirRandom = 3     // SetRandomState
	};

	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // playerdlg.cpp:1053

	void SetCharControl(eButtonDir dir);
	void SetCaddieControl(eButtonDir dir);
	void SetClubControl(eButtonDir dir);
	void SetBallControl(eButtonDir dir);
	void OnBaseInit(intptr_t param);         // playerdlg.cpp:1208
	void OnBaseBtnUp(void);             // playerdlg.cpp:1213
	void OnSelectInit(intptr_t param);       // playerdlg.cpp:1218
	void OnIndexInit(intptr_t param);        // playerdlg.cpp:1395
	void OnPowerBarInit(intptr_t param);     // playerdlg.cpp:1956
	void OnControlBarInit(intptr_t param);   // playerdlg.cpp:1961
	void OnImpactBarInit(intptr_t param);    // playerdlg.cpp:1967
	void OnSpinBarInit(intptr_t param);      // playerdlg.cpp:1973
	void OnCurveBarInit(intptr_t param);     // playerdlg.cpp:1979
	void OnPowerEditInit(int param);    // playerdlg.cpp:1987
	void OnControlEditInit(int param);  // playerdlg.cpp:1992
	void OnImpactEditInit(int param);   // playerdlg.cpp:1997
	void OnSpinEditInit(int param);     // playerdlg.cpp:2002
	void OnCurveEditInit(int param);    // playerdlg.cpp:2007
	void OnPetInit(int param);          // playerdlg.cpp:1236
	void OnCharPrevInit(int param);     // playerdlg.cpp:1241
	void OnCharNextInit(int param);     // playerdlg.cpp:1252
	void OnCaddiePrevInit(int param);   // playerdlg.cpp:1273
	void OnCaddieNextInit(int param);   // playerdlg.cpp:1284
	void OnClubPrevInit(int param);     // playerdlg.cpp:1305
	void OnClubNextInit(int param);     // playerdlg.cpp:1316
	void OnBallPrevInit(int param);     // playerdlg.cpp:1332
	void OnBallNextInit(int param);     // playerdlg.cpp:1343
	void OnRemoveInit(int param);       // playerdlg.cpp:1354
	void OnRemoveBtnUp(void);           // playerdlg.cpp:1360
	void SetClub(unsigned long typeId); // playerdlg.cpp:1926
	void OnCaddieInit(int param);       // playerdlg.cpp:1263
	void OnCaddieLimitInit(int param);  // playerdlg.cpp:1268
	void OnClubInit(int param);         // playerdlg.cpp:1295
	void OnClubLimitInit(int param);    // playerdlg.cpp:1300
	void OnBallInit(int param);         // playerdlg.cpp:1327
	void OnNameInit(intptr_t param);         // playerdlg.cpp:1373
	void SetCharacter(void);            // playerdlg.cpp:1849
	void SetBall(unsigned long typeId); // playerdlg.cpp:1942
	void SetLevelBar(void);
	void OnCharPrevBtnUp(void);     // playerdlg.cpp:1247
	void OnCharNextBtnUp(void);     // playerdlg.cpp:1258
	void OnCaddiePrevBtnUp(void);   // playerdlg.cpp:1279
	void OnCaddieNextBtnUp(void);   // playerdlg.cpp:1290
	void OnBallPrevBtnUp(void);     // playerdlg.cpp:1338
	void OnBallNextBtnUp(void);     // playerdlg.cpp:1349
	void OnPetOwnerDraw(void);      // playerdlg.cpp:1179
	void SetLevelNum(unsigned char type, unsigned char level, unsigned char maxLevel);   // playerdlg.cpp:1484

private:
	FrArea* m_pPetArea;           // 0x110, "pet"
	FrButton* m_pCharPrevBtn;     // 0x114
	FrButton* m_pCharNextBtn;     // 0x118
	FrArea* m_pCaddieArea;        // 0x11C, "caddie"
	FrArea* m_pCaddieLimitArea;   // 0x120, "caddie_limit"
	FrButton* m_pCaddiePrevBtn;   // 0x124
	FrButton* m_pCaddieNextBtn;   // 0x128
	FrArea* m_pClubArea;          // 0x12C, "club"
	FrArea* m_pClubLimitArea;     // 0x130, "club_limit", shown while the club set's level is above the player's
	FrButton* m_pClubPrevBtn;     // 0x134
	FrButton* m_pClubNextBtn;     // 0x138
	FrArea* m_pBallArea;          // 0x13C, "ball"
	FrButton* m_pBallPrevBtn;     // 0x140
	FrButton* m_pBallNextBtn;     // 0x144
	FrButton* m_pRemoveBtn;       // 0x148, shown for the players from index 2 on
	FrEdit* m_pNameEdit;          // 0x14C
	FrArea* m_pIndexArea;         // 0x150, "index"
	FrButton* m_pTeamBtn;         // 0x154, team colour button
	FrWnd* m_pSelectWnd;          // 0x158, shown while the dialog is selected
	FrGaugeBar* m_pStatGauge[5];  // 0x15C, gauge per level type (power, control, impact, spin, curve)
	FrEdit* m_pLevelEdit[5];      // 0x170, level text per level type
	CExhibition* m_pExhibition;   // 0x184, character model
	CPartTidList* m_pPartTidList; // 0x188, parts of the character model
	unsigned char m_team;         // 0x18C
	int m_playerIndex;            // 0x190, index into CSharedDoc::m_roomPlayers, -1 after the constructor
	FrWnd* m_pLinkedWnd;          // 0x194, shown again when the player is removed (OnRemoveBtnUp); cleared by the constructor
};
CHECK_SIZE(FrPlayerDlg, 0x198);
BEGIN_LAYOUT_CHECK(FrPlayerDlg)
	CHECK_FIELD(FrPlayerDlg, m_pPetArea, 0x110);
	CHECK_FIELD(FrPlayerDlg, m_pCharPrevBtn, 0x114);
	CHECK_FIELD(FrPlayerDlg, m_pCharNextBtn, 0x118);
	CHECK_FIELD(FrPlayerDlg, m_pCaddiePrevBtn, 0x124);
	CHECK_FIELD(FrPlayerDlg, m_pCaddieNextBtn, 0x128);
	CHECK_FIELD(FrPlayerDlg, m_pClubPrevBtn, 0x134);
	CHECK_FIELD(FrPlayerDlg, m_pClubNextBtn, 0x138);
	CHECK_FIELD(FrPlayerDlg, m_pBallPrevBtn, 0x140);
	CHECK_FIELD(FrPlayerDlg, m_pBallNextBtn, 0x144);
	CHECK_FIELD(FrPlayerDlg, m_pRemoveBtn, 0x148);
	CHECK_FIELD(FrPlayerDlg, m_pCaddieArea, 0x11C);
	CHECK_FIELD(FrPlayerDlg, m_pCaddieLimitArea, 0x120);
	CHECK_FIELD(FrPlayerDlg, m_pClubArea, 0x12C);
	CHECK_FIELD(FrPlayerDlg, m_pClubLimitArea, 0x130);
	CHECK_FIELD(FrPlayerDlg, m_pBallArea, 0x13C);
	CHECK_FIELD(FrPlayerDlg, m_pNameEdit, 0x14C);
	CHECK_FIELD(FrPlayerDlg, m_pPartTidList, 0x188);
	CHECK_FIELD(FrPlayerDlg, m_pIndexArea, 0x150);
	CHECK_FIELD(FrPlayerDlg, m_pStatGauge, 0x15C);
	CHECK_FIELD(FrPlayerDlg, m_pTeamBtn, 0x154);
	CHECK_FIELD(FrPlayerDlg, m_pSelectWnd, 0x158);
	CHECK_FIELD(FrPlayerDlg, m_pLevelEdit, 0x170);
	CHECK_FIELD(FrPlayerDlg, m_pExhibition, 0x184);
	CHECK_FIELD(FrPlayerDlg, m_team, 0x18C);
	CHECK_FIELD(FrPlayerDlg, m_playerIndex, 0x190);
	CHECK_FIELD(FrPlayerDlg, m_pLinkedWnd, 0x194);
END_LAYOUT_CHECK

// Empty player slot dialog; its size (0x128) is the `new` of
// FrPlayerEmptyDlgMakeInstance.
class FrPlayerEmptyDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // playerdlg.h:331

	FrPlayerEmptyDlg(void) : m_pPlayerDlg(0), m_index(-1) {}   // playerdlg.h:334

	void Init(int index);   // playerdlg.cpp:2026

protected:
	static sFRESH_MSGMAP _MsgMap;
	static sFRESH_ENTRY _MsgEntries[];
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // playerdlg.cpp:2016

	void OnBaseInit(intptr_t param);
	void OnCreateInit(intptr_t param);
	void OnCreateBtnUp(void);
	void OnIndexInit(intptr_t param);

private:
	FrButton* m_pCreateBtn; // 0x110, "create"
	FrArea* m_pBgArea;      // 0x114, "index", background image of the slot
	FrArea* m_pBaseArea;    // 0x118, "base"
	FrPlayerDlg* m_pPlayerDlg;   // 0x11C, the player dialog shown in place of the slot (OnCreateBtnUp); cleared by the constructor
	int m_index;            // 0x120, slot index set by Init
	UNKNOWN(0x124, 0x128);
};
CHECK_SIZE(FrPlayerEmptyDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrPlayerEmptyDlg)
	CHECK_FIELD(FrPlayerEmptyDlg, m_pCreateBtn, 0x110);
	CHECK_FIELD(FrPlayerEmptyDlg, m_pBgArea, 0x114);
	CHECK_FIELD(FrPlayerEmptyDlg, m_pBaseArea, 0x118);
	CHECK_FIELD(FrPlayerEmptyDlg, m_pPlayerDlg, 0x11C);
	CHECK_FIELD(FrPlayerEmptyDlg, m_index, 0x120);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PLAYERDLG_H
