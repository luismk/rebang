// Original: d:\build\custom\temp\code\client\projectg\s5\demonlanddlg.h
#ifndef PG_CLIENT_PROJECTG_S5_DEMONLANDDLG_H
#define PG_CLIENT_PROJECTG_S5_DEMONLANDDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "TreasureDlg.h"

class Bitmap;
class FrArea;
class FrGaugeBarImage;
class FrListBox;
class FrStatic;
class WRect;
class WOverlay;
enum eDemonDlg;

// Next-hole panel of the Demon Land mode; its size (0x148) is the `new` of
// FrDemonNextHoleMakeInstance. Only the members this unit defines so far are
// declared.
class FrDemonNextHole : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // demonlanddlg.h:39

	FrDemonNextHole(void);
	virtual ~FrDemonNextHole(void);

	virtual bool OnInit(void);   // demonlanddlg.cpp:59
	void SetNextHole(int hole);
	void SetState(unsigned char state);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnAreaPattern_Init(intptr_t param);
	void OnAreaPattern_OwnerDraw(int param);
	void OnAreaNextHole_Init(intptr_t param);
	void OnAreaNextHole_OwnerDraw(int param);

	FrArea* m_pPatternArea;    // 0x110
	FrArea* m_pNextHoleArea;   // 0x114
	WOverlay* m_pHoleOverlay[2];   // 0x118, the hole number textures (holes 1-10, then 11-19)
	WRect m_holeUV;            // 0x120, the UV of the hole number (S5::UTIL::CalculateUVPos)
	const Bitmap* m_pPatternBitmap;   // 0x130
	int m_texWidth;            // 0x134, m_pHoleOverlay[0]'s texture size
	int m_texHeight;           // 0x138
	int m_state;               // 0x13c
	int m_holeOverlayIndex;    // 0x140, which of m_pHoleOverlay draws the hole
	PADDING(0x144, 0x148);     // FrDemonNextHole members
};

CHECK_SIZE(FrDemonNextHole, 0x148);
BEGIN_LAYOUT_CHECK(FrDemonNextHole)
	CHECK_FIELD(FrDemonNextHole, m_pPatternArea, 0x110);
	CHECK_FIELD(FrDemonNextHole, m_pNextHoleArea, 0x114);
	CHECK_FIELD(FrDemonNextHole, m_pPatternBitmap, 0x130);
	CHECK_FIELD(FrDemonNextHole, m_state, 0x13c);
	CHECK_FIELD(FrDemonNextHole, m_pHoleOverlay, 0x118);
	CHECK_FIELD(FrDemonNextHole, m_holeUV, 0x120);
	CHECK_FIELD(FrDemonNextHole, m_holeOverlayIndex, 0x140);
	CHECK_FIELD(FrDemonNextHole, m_texWidth, 0x134);
	CHECK_FIELD(FrDemonNextHole, m_texHeight, 0x138);
END_LAYOUT_CHECK

// Attack gauge of the Demon Land mode (size 0x128).
class FrDemonAttackGauge : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // demonlanddlg.h:93

	FrDemonAttackGauge(void);
	virtual ~FrDemonAttackGauge(void);

	virtual bool OnInit(void);
	void SetState(unsigned char state);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnImageBarAttackPoint_Init(intptr_t param);
	void OnAreaAttackMarker_Init(intptr_t param);
	void OnAreaAttackMarker_OwnerDraw(int param);
	void OnAreaBlingBar_Init(intptr_t param);
	void OnAreaBlingBar_OwnerDraw(int param);

	FrGaugeBarImage* m_pAttackPointBar;      // 0x110
	FrArea* m_pAttackMarkerArea;             // 0x114
	FrArea* m_pBlingBarArea;                 // 0x118
	const Bitmap* m_pAttackPointBitmap;      // 0x11c
	const Bitmap* m_pGaugeLightBitmap;       // 0x120
	int m_state;                             // 0x124
};

CHECK_SIZE(FrDemonAttackGauge, 0x128);
BEGIN_LAYOUT_CHECK(FrDemonAttackGauge)
	CHECK_FIELD(FrDemonAttackGauge, m_pAttackPointBar, 0x110);
	CHECK_FIELD(FrDemonAttackGauge, m_pAttackMarkerArea, 0x114);
	CHECK_FIELD(FrDemonAttackGauge, m_pBlingBarArea, 0x118);
	CHECK_FIELD(FrDemonAttackGauge, m_pAttackPointBitmap, 0x11c);
	CHECK_FIELD(FrDemonAttackGauge, m_pGaugeLightBitmap, 0x120);
	CHECK_FIELD(FrDemonAttackGauge, m_state, 0x124);
END_LAYOUT_CHECK

// Live ranking of the Demon Land mode (size 0x120).
class FrDemonProcessRank : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // demonlanddlg.h:136

	FrDemonProcessRank(void);
	virtual ~FrDemonProcessRank(void);

	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnAreaLeaderPet_Init(intptr_t param);
	void OnAreaLeaderPet_OwnerDraw(int);
	void OnListBoxUserInfoList_Init(intptr_t param);
	void OnListBoxUserInfoList_OwnerDraw(int param);

	FrArea* m_pLeaderPetArea;       // 0x110
	FrListBox* m_pUserInfoList;     // 0x114
	PADDING(0x118, 0x120);          // FrDemonProcessRank members
};

CHECK_SIZE(FrDemonProcessRank, 0x120);
BEGIN_LAYOUT_CHECK(FrDemonProcessRank)
	CHECK_FIELD(FrDemonProcessRank, m_pLeaderPetArea, 0x110);
	CHECK_FIELD(FrDemonProcessRank, m_pUserInfoList, 0x114);
END_LAYOUT_CHECK

// Final score and gift screen of the Demon Land mode (size 0x190).
class FrDemonFinalScore : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // demonlanddlg.h:172

	FrDemonFinalScore(void);
	virtual ~FrDemonFinalScore(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnImageBarAttackPoint_Init(intptr_t param);
	void OnInit_Gift_List(intptr_t param);
	void OnOwnerDraw_Gift_List(intptr_t param);

	FrGaugeBarImage* m_pAttackPointBar;   // 0x110
	CBoxOpener m_boxOpener;          // 0x114
};

CHECK_SIZE(FrDemonFinalScore, 0x190);
BEGIN_LAYOUT_CHECK(FrDemonFinalScore)
	CHECK_FIELD(FrDemonFinalScore, m_pAttackPointBar, 0x110);
	CHECK_FIELD(FrDemonFinalScore, m_boxOpener, 0x114);
END_LAYOUT_CHECK

// Entry-ticket notice of the Demon Land mode (size 0x118).
class FrDemonLandTicket : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // demonlanddlg.h:197

	FrDemonLandTicket(void);
	virtual ~FrDemonLandTicket(void);

	void SetTicketNumber(int ticketNumber);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInit_TicketMsg_Static(intptr_t param);

	FrStatic* m_pTicketMsgStatic;   // 0x110
	int m_ticketNumber;             // 0x114
};

CHECK_SIZE(FrDemonLandTicket, 0x118);
BEGIN_LAYOUT_CHECK(FrDemonLandTicket)
	CHECK_FIELD(FrDemonLandTicket, m_pTicketMsgStatic, 0x110);
	CHECK_FIELD(FrDemonLandTicket, m_ticketNumber, 0x114);
END_LAYOUT_CHECK

class CDemonLandForm {
public:
	CDemonLandForm();
	~CDemonLandForm();

	void InitializeForm();
	FrDemonNextHole* GetDemonNextHole();
	FrDemonAttackGauge* GetDemonAttackGauge();
	FrDemonProcessRank* GetDemonProcessRank();
	FrDemonFinalScore* GetDemonFinalScore();
	void AdjustWindowPos();
	bool CreateDemonLandForm(eDemonDlg dlg);

private:
	void ReleaseForm();
	void AdjustNextHolePos(WRect& rect);
	void AdjustAttackGaugePos(WRect& rect);

	FrDemonNextHole* m_pNextHole;       // 0x0
	FrDemonAttackGauge* m_pAttackGauge; // 0x4
	FrDemonProcessRank* m_pProcessRank; // 0x8
	FrDemonFinalScore* m_pFinalScore;   // 0xC
};
CHECK_SIZE(CDemonLandForm, 0x10);

#endif
