// Original: d:\build\custom\temp\code\client\projectg\cardboxdlg.h
#ifndef PG_CLIENT_PROJECTG_CARDBOXDLG_H
#define PG_CLIENT_PROJECTG_CARDBOXDLG_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class CWindFont;
class CExhibition;
class CParticle2D;
class FrButton;
class WReceivedPacket;

// Card pack opening dialog: the 3D box and packs (a CExhibition), the
// buttons that open a pack, go to the next one and show the result. Its
// size (0x168) is the `new` of FrCardBoxDlgMakeInstance.
class FrCardBoxDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // cardboxdlg.h:18

	FrCardBoxDlg(void);                                              // cardboxdlg.cpp:74
	virtual ~FrCardBoxDlg(void);                                     // cardboxdlg.cpp:79

	virtual void Init(unsigned long, unsigned long, int);
	void RecievedPacket(WReceivedPacket* pPacket);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // cardboxdlg.cpp:56

	virtual bool OnInit(void);                                       // cardboxdlg.cpp:210
	virtual void OnProc(const float dt);
	virtual void OnOK(void);        // cardboxdlg.cpp:381
	virtual void OnCancel(void);    // cardboxdlg.cpp:386
	void OnResultBtnInit(intptr_t param);                                 // cardboxdlg.cpp:393
	void OnCloseBtnInit(intptr_t param);                                  // cardboxdlg.cpp:430
	void OnCloseBtnUp(void);        // cardboxdlg.cpp:437
	void OnAreaBgInit(intptr_t param);                                    // cardboxdlg.cpp:443
	void OnPackOpenBtnInit(intptr_t param);                               // cardboxdlg.cpp:556
	void OnOpenNextBtnInit(intptr_t param);                               // cardboxdlg.cpp:597
	void OnOpenNextBtnUp(void);                                      // cardboxdlg.cpp:610
	void OnPackOpenBtnUp(void);
	void OnAreaBgOwnerDraw(void);
	void OnResultBtnUp(void);

private:
	void SetAnimation(unsigned long);
	void Display2DBg(void);
	void LoadCardBoxPet(unsigned long, int);
	void LoadCardPackPet(unsigned long);
	void SetPackVisibleInBox(void);

	CWindFont* m_pFont;                          // 0x110 (OnInit)
	CExhibition* m_pExhibition;                  // 0x114 (the 3D box and packs)
	WRect m_rcBg;                                // 0x118 (OnAreaBgInit)
	int m_state;                                 // 0x128 (animation state, 7 after OnOpenNextBtnUp)
	FrButton* m_pPackOpenBtn;                    // 0x12c (OnPackOpenBtnInit)
	FrButton* m_pOpenNextBtn;                    // 0x130 (OnOpenNextBtnInit)
	FrButton* m_pCloseBtn;                       // 0x134 (OnCloseBtnInit)
	FrButton* m_pResultBtn;                      // 0x138 (OnResultBtnInit)
	unsigned long m_unknown13c;                  // 0x13c (an item type id: sent with the pack-open packet, its kind bits pick the open sound)
	unsigned long m_packId;                      // 0x140 (SetPackVisibleInBox looks it up)
	WRect m_rcPackOpen;                          // 0x144 (the pack-open button's rect)
	float m_packOpenShake;                       // 0x154 (the pack-open button's swing to the right, 0..15, OnProc)
	std::map<unsigned long, unsigned int> m_map; // 0x158
	CParticle2D* m_pParticle;                    // 0x164
};
BEGIN_LAYOUT_CHECK(FrCardBoxDlg)
	CHECK_FIELD(FrCardBoxDlg, m_pFont, 0x110);
	CHECK_FIELD(FrCardBoxDlg, m_pExhibition, 0x114);
	CHECK_FIELD(FrCardBoxDlg, m_rcBg, 0x118);
	CHECK_FIELD(FrCardBoxDlg, m_state, 0x128);
	CHECK_FIELD(FrCardBoxDlg, m_pPackOpenBtn, 0x12c);
	CHECK_FIELD(FrCardBoxDlg, m_pOpenNextBtn, 0x130);
	CHECK_FIELD(FrCardBoxDlg, m_pCloseBtn, 0x134);
	CHECK_FIELD(FrCardBoxDlg, m_pResultBtn, 0x138);
	CHECK_FIELD(FrCardBoxDlg, m_unknown13c, 0x13c);
	CHECK_FIELD(FrCardBoxDlg, m_packId, 0x140);
	CHECK_FIELD(FrCardBoxDlg, m_rcPackOpen, 0x144);
	CHECK_FIELD(FrCardBoxDlg, m_packOpenShake, 0x154);
	CHECK_FIELD(FrCardBoxDlg, m_map, 0x158);
	CHECK_FIELD(FrCardBoxDlg, m_pParticle, 0x164);
END_LAYOUT_CHECK
CHECK_SIZE(FrCardBoxDlg, 0x168);

#endif // PG_CLIENT_PROJECTG_CARDBOXDLG_H
