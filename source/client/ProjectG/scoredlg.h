// Original: d:\build\custom\temp\code\client\projectg\scoredlg.h
#ifndef PG_CLIENT_PROJECTG_SCOREDLG_H
#define PG_CLIENT_PROJECTG_SCOREDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "frgraphicinterface.h"

class Bitmap;
class FrArea;
class FrButton;
class FrListBox;
struct FrListItem;
struct sRivalData;

// Score card dialog: front/back nine, shot/graph/pang/versus pages, hole, par,
// golfer, record and total lists. Its size (0x1d0) is the `new` of
// FrScoreDlgMakeInstance.
class FrScoreDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // scoredlg.h:8

	FrScoreDlg(void);

	void SetVisibleCloseBtn(bool bVisible);
	void SetVisibleCancelBtn(bool bVisible);
	void SetPlayer(unsigned long uid);
	void SetGuildPlayer(unsigned long uid);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);
	virtual void OnDraw(void);

	void SetFrontBackBtn(void);
	void SetTypeBtn(void);
	int GetGuildVersus(sRivalData* pMine, sRivalData* pRival, int hole);
	unsigned long GetScoreColor(int score);
	void LoadBitmaps(void);
	void Resize(void);
	int GetMyCurHoleInGuildMatch(void);
	void DrawDigit(FrGraphicInterface* pGDI, char* text, WPoint pos, unsigned long color, FrGDI::eTextAlign align, bool bShadow);
	void DrawShot(FrListItem* pItem);
	void DrawGraph(FrListItem* pItem);
	void DrawPang(FrListItem* pItem);
	void DrawVersus(FrListItem* pItem);

	void OnBar2Init(intptr_t param);
	void OnBar3Init(intptr_t param);
	void OnFront9BtnInit(intptr_t param);
	void OnFront9BtnUp(void);
	void OnBack9BtnInit(intptr_t param);
	void OnBack9BtnUp(void);
	void OnShotBtnInit(intptr_t param);
	void OnShotBtnUp(void);
	void OnGraphBtnInit(intptr_t param);
	void OnGraphBtnUp(void);
	void OnPangBtnInit(intptr_t param);
	void OnPangBtnUp(void);
	void OnVersusBtnInit(intptr_t param);
	void OnVersusBtnUp(void);
	void OnPuttBtnInit(intptr_t param);
	void OnHoleListInit(intptr_t param);
	void OnHoleListOwnerDraw(intptr_t param);
	void OnParListInit(intptr_t param);
	void OnParListOwnerDraw(intptr_t param);
	void OnGolferListInit(intptr_t param);
	void OnGolferListOwnerDraw(intptr_t param);
	void OnRecordListInit(intptr_t param);
	void OnRecordListOwnerDraw(intptr_t param);
	void OnTotalListInit(intptr_t param);
	void OnTotalListOwnerDraw(int param);
	void OnCancelBtnInit(intptr_t param);
	void OnCancelBtnUp(void);
	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnBlankPanel01Init(intptr_t param);
	void OnBlankPanel01OwnerDraw(int param);
	void OnBlankPanel02Init(intptr_t param);
	void OnBlankPanel02OwnerDraw(int param);
	void OnBlankPanel03Init(intptr_t param);
	void OnBlankPanel03OwnerDraw(int param);

private:
	FrArea* m_pBar2;                      // 0x110 (OnBar2Init)
	FrArea* m_pBar3;                      // 0x114 (OnBar3Init)
	FrButton* m_pFront9Btn;               // 0x118 (OnFront9BtnInit)
	FrButton* m_pBack9Btn;                // 0x11c (OnBack9BtnInit)
	unsigned long m_values120[4];         // 0x120 cleared by the constructor
	FrButton* m_pTypeBtn[4];              // 0x130 shot, graph, pang, versus (SetTypeBtn)
	FrListBox* m_pList[5];                // 0x140 golfer, record, total, par, hole (OnXxxListInit)
	FrButton* m_pCancelBtn;               // 0x154 (OnCancelBtnInit)
	FrButton* m_pCloseBtn;                // 0x158 (OnCloseBtnInit)
	bool m_bFront9;                       // 0x15c true: the front nine is shown
	UNKNOWN(0x15d, 0x160);
	int m_type;                           // 0x160 page: 0 shot, 1 graph, 2 pang, 3 versus
	float m_time;                         // 0x164 (OnProc, reset by the page buttons)
	const Bitmap* m_pDigit[10];           // 0x168 SCORECARD "0".."9" (LoadBitmaps)
	const Bitmap* m_pPlus;                // 0x190 SCORECARD "+"
	const Bitmap* m_pMinus;               // 0x194 SCORECARD "-"
	const Bitmap* m_pPoint;               // 0x198 SCORECARD "point"
	const Bitmap* m_pWin;                 // 0x19c SCORECARD "win"
	const Bitmap* m_pLose;                // 0x1a0 SCORECARD "lose"
	const Bitmap* m_pDraw;                // 0x1a4 SCORECARD "draw"
	const Bitmap* m_pVersusMinus;         // 0x1a8 SCORECARD "-"
	unsigned long m_guildPlayerUID[2];    // 0x1ac the two guild match players compared on the versus page (SetGuildPlayer)
	const Bitmap* m_pMassIcon;            // 0x1b4 golf "pc_icon_mass"
	sRivalData* m_pGuildRival;            // 0x1b8 the player whose holes GetMyCurHoleInGuildMatch checks, cleared by the constructor
	bool m_bShowBlankPanel;               // 0x1bc (room mode 14)
	UNKNOWN(0x1bd, 0x1c0);
	FrArea* m_pBlankPanel[3];             // 0x1c0 (OnBlankPanel0xInit)
	UNKNOWN(0x1cc, 0x1d0);
};
BEGIN_LAYOUT_CHECK(FrScoreDlg)
	CHECK_FIELD(FrScoreDlg, m_pBar2, 0x110);
	CHECK_FIELD(FrScoreDlg, m_pBar3, 0x114);
	CHECK_FIELD(FrScoreDlg, m_pFront9Btn, 0x118);
	CHECK_FIELD(FrScoreDlg, m_pBack9Btn, 0x11c);
	CHECK_FIELD(FrScoreDlg, m_values120, 0x120);
	CHECK_FIELD(FrScoreDlg, m_pTypeBtn, 0x130);
	CHECK_FIELD(FrScoreDlg, m_pList, 0x140);
	CHECK_FIELD(FrScoreDlg, m_pCancelBtn, 0x154);
	CHECK_FIELD(FrScoreDlg, m_pCloseBtn, 0x158);
	CHECK_FIELD(FrScoreDlg, m_bFront9, 0x15c);
	CHECK_FIELD(FrScoreDlg, m_type, 0x160);
	CHECK_FIELD(FrScoreDlg, m_time, 0x164);
	CHECK_FIELD(FrScoreDlg, m_pDigit, 0x168);
	CHECK_FIELD(FrScoreDlg, m_pPlus, 0x190);
	CHECK_FIELD(FrScoreDlg, m_pMinus, 0x194);
	CHECK_FIELD(FrScoreDlg, m_pPoint, 0x198);
	CHECK_FIELD(FrScoreDlg, m_pWin, 0x19c);
	CHECK_FIELD(FrScoreDlg, m_pLose, 0x1a0);
	CHECK_FIELD(FrScoreDlg, m_pDraw, 0x1a4);
	CHECK_FIELD(FrScoreDlg, m_pVersusMinus, 0x1a8);
	CHECK_FIELD(FrScoreDlg, m_guildPlayerUID, 0x1ac);
	CHECK_FIELD(FrScoreDlg, m_pMassIcon, 0x1b4);
	CHECK_FIELD(FrScoreDlg, m_pGuildRival, 0x1b8);
	CHECK_FIELD(FrScoreDlg, m_bShowBlankPanel, 0x1bc);
	CHECK_FIELD(FrScoreDlg, m_pBlankPanel, 0x1c0);
END_LAYOUT_CHECK
CHECK_SIZE(FrScoreDlg, 0x1d0);

#endif // PG_CLIENT_PROJECTG_SCOREDLG_H
