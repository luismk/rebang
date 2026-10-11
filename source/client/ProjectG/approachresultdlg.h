// Original: d:\build\custom\temp\code\client\projectg\approachresultdlg.h
#ifndef PG_CLIENT_PROJECTG_APPROACHRESULTDLG_H
#define PG_CLIENT_PROJECTG_APPROACHRESULTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class CExhibition;
class CPartTidList;
class FrArea;
class FrListBox;
class WTitleFont;

// Result dialog of an approach hole: the hole's players on the left and
// right lists; its size (0x160) is the `new` of
// FrApproachResultDlgMakeInstance. Only the members this unit defines so far
// are declared.
class FrApproachResultDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // approachresultdlg.h:9

	FrApproachResultDlg(void);
	virtual ~FrApproachResultDlg(void);

	void ClearVariables(void);
	void SetApproachEnd(bool bEnd);
	bool GetApproachEnd(void) const;
	void ShowPet(void);

	virtual void OnProc(const float dt);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);

	void OnLeftAreaInit(intptr_t param);
	void OnLeftAreaOwnerDraw(int param);
	void OnLeftListInit(intptr_t param);
	void OnLeftListOwnerDraw(int param);
	void OnRightAreaInit(intptr_t param);
	void OnRightAreaOwnerDraw(int param);
	void OnRightListInit(intptr_t param);
	void OnRightListOwnerDraw(int param);

private:
	FrArea* m_pLeftArea;                // 0x110 ("left_area")
	FrListBox* m_pLeftList;             // 0x114 ("left_list")
	const Bitmap* m_pLeftAreaBitmap;    // 0x118 (name built by OnLeftAreaInit)
	const Bitmap* m_pLeftTab1;          // 0x11c
	const Bitmap* m_pLeftTab2;          // 0x120
	CExhibition* m_pExhibition;         // 0x124 (the pet shown by ShowPet)
	CPartTidList* m_pPartTidList;       // 0x128 (its parts, set by ShowPet)
	FrListBox* m_pRightList;            // 0x12c ("right_list")
	FrArea* m_pRightArea;               // 0x130 ("right_area")
	const Bitmap* m_pRightTab1;         // 0x134 ("approach_tab3")
	const Bitmap* m_pRightTab2;         // 0x138 ("approach_tab4")
	WTitleFont* m_pTitleFont1;          // 0x13c ("[bigfont.jpg", 32x32)
	WTitleFont* m_pTitleFont2;          // 0x140 ("[font_wind.jpg", 16x16)
	const Bitmap* m_pBoxBitmap;         // 0x144 ("th_box_<n>" of the course)
	const Bitmap* m_pRankGold;          // 0x148 ("rank_gold")
	const Bitmap* m_pRankSilver;        // 0x14c ("rank_silver")
	const Bitmap* m_pRankBronze;        // 0x150 ("rank_bronze")
	const Bitmap* m_pTreasureBox;       // 0x154 ("treasurebox_<x>" of the course)
	const Bitmap* m_pPrizeGoodLuck;     // 0x158 ("prize_goodluck")
	bool m_bApproachEnd;                // 0x15c
	PADDING(0x15d, 0x160);
};

CHECK_SIZE(FrApproachResultDlg, 0x160);
BEGIN_LAYOUT_CHECK(FrApproachResultDlg)
	CHECK_FIELD(FrApproachResultDlg, m_pLeftArea, 0x110);
	CHECK_FIELD(FrApproachResultDlg, m_pLeftList, 0x114);
	CHECK_FIELD(FrApproachResultDlg, m_pLeftAreaBitmap, 0x118);
	CHECK_FIELD(FrApproachResultDlg, m_pLeftTab2, 0x120);
	CHECK_FIELD(FrApproachResultDlg, m_pExhibition, 0x124);
	CHECK_FIELD(FrApproachResultDlg, m_pPartTidList, 0x128);
	CHECK_FIELD(FrApproachResultDlg, m_pRightList, 0x12c);
	CHECK_FIELD(FrApproachResultDlg, m_pRightArea, 0x130);
	CHECK_FIELD(FrApproachResultDlg, m_pRightTab2, 0x138);
	CHECK_FIELD(FrApproachResultDlg, m_pTitleFont1, 0x13c);
	CHECK_FIELD(FrApproachResultDlg, m_pTitleFont2, 0x140);
	CHECK_FIELD(FrApproachResultDlg, m_pBoxBitmap, 0x144);
	CHECK_FIELD(FrApproachResultDlg, m_pRankGold, 0x148);
	CHECK_FIELD(FrApproachResultDlg, m_pTreasureBox, 0x154);
	CHECK_FIELD(FrApproachResultDlg, m_pPrizeGoodLuck, 0x158);
	CHECK_FIELD(FrApproachResultDlg, m_bApproachEnd, 0x15c);
END_LAYOUT_CHECK

// Final result dialog of an approach game: a drawing area with four
// bitmaps and the ranking list; its size (0x128) is the `new` of
// FrApproachEndDlgMakeInstance.
class FrApproachEndDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // approachresultdlg.h:79

	FrApproachEndDlg(void);
	virtual ~FrApproachEndDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnDrawAreaInit(intptr_t param);
	void OnDrawAreaOwnerDraw(int param);
	void OnListInit(intptr_t param);
	void OnListOwnerDraw(int param);

private:
	FrArea* m_pDrawArea;         // 0x110 ("draw_area")
	FrListBox* m_pList;          // 0x114 ("app_list")
	const Bitmap* m_pBitmap[4];  // 0x118 (drawn by OnDrawAreaOwnerDraw)
};

CHECK_SIZE(FrApproachEndDlg, 0x128);
BEGIN_LAYOUT_CHECK(FrApproachEndDlg)
	CHECK_FIELD(FrApproachEndDlg, m_pDrawArea, 0x110);
	CHECK_FIELD(FrApproachEndDlg, m_pList, 0x114);
	CHECK_FIELD(FrApproachEndDlg, m_pBitmap, 0x118);
END_LAYOUT_CHECK

// Treasure gift dialog of an approach game: no members and no handlers of
// its own; its size (0x110) is the `new` of FrAppTreasureGiftDlgMakeInstance.
class FrAppTreasureGiftDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // approachresultdlg.h:117

	FrAppTreasureGiftDlg(void);
	virtual ~FrAppTreasureGiftDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;
};

CHECK_SIZE(FrAppTreasureGiftDlg, 0x110);

#endif // PG_CLIENT_PROJECTG_APPROACHRESULTDLG_H
