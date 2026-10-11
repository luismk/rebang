// Original: d:\build\custom\temp\code\client\projectg\valentineeventdlg.h
#ifndef PG_CLIENT_PROJECTG_VALENTINEEVENTDLG_H
#define PG_CLIENT_PROJECTG_VALENTINEEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "frbutton.h"
#include "frarea.h"
#include "wfont.h"

// Valentine event heart dialog; its size (0x148) is the `new` of
// FrValEventDlgMakeInstance. Only the members the unit defines so far are
// declared.
class FrValEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // valentineeventdlg.h:20

	FrValEventDlg(void);
	virtual ~FrValEventDlg(void);

	virtual bool OnInit(void);
	void BuildHeartCount(void);
	void DrawInDlg(const char* name, WPoint& pt);
	void DrawGauge(WPoint& pt, unsigned char count);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void BindButton(FrButton*& button, int id);
	void OnBackImgInit(intptr_t id);
	void OnBackImgOwnerDraw(int param);
	void OnGaugeImgOwnerDraw(int param);
	void OnNuriBtnInit(intptr_t id);
	void OnNuriBtnLBDown(void);
	void OnAzerBtnInit(intptr_t id);
	void OnAzerBtnLBDown(void);
	void OnMaxBtnInit(intptr_t id);
	void OnMaxBtnLBDown(void);
	void OnKazBtnInit(intptr_t id);
	void OnKazBtnLBDown(void);
	void OnHanaBtnInit(intptr_t id);
	void OnHanaBtnLBDown(void);
	void OnArinBtnInit(intptr_t id);
	void OnArinBtnLBDown(void);
	void OnCecilBtnInit(intptr_t id);
	void OnCecilBtnLBDown(void);
	void OnKoohBtnInit(intptr_t id);
	void OnKoohBtnLBDown(void);
	void OnLuciaBtnInit(intptr_t id);
	void OnLuciaBtnLBDown(void);
	void OnNellBtnInit(int id);
	void OnNellBtnLBDown(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	void CheckButtonClick(void);

	unsigned long m_giftFlag;        // 0x110
	unsigned char m_hanaCount;       // 0x114
	unsigned char m_cecilCount;      // 0x115
	unsigned char m_koohCount;       // 0x116
	unsigned char m_arinCount;       // 0x117
	unsigned char m_luciaCount;      // 0x118
	unsigned char m_nellCount;       // 0x119
	unsigned char m_nuriCount;       // 0x11A (BuildHeartCount, capped at 10)
	unsigned char m_azerCount;       // 0x11B
	unsigned char m_maxCount;        // 0x11C
	unsigned char m_kazCount;        // 0x11D
	PADDING(0x11E, 0x120);
	FrButton* m_pHanaBtn;            // 0x120
	FrButton* m_pCecilBtn;           // 0x124
	FrButton* m_pKoohBtn;            // 0x128
	FrButton* m_pArinBtn;            // 0x12C
	FrButton* m_pLuciaBtn;           // 0x130
	FrButton* m_pNellBtn;            // 0x134
	FrButton* m_pNuriBtn;            // 0x138
	FrButton* m_pAzerBtn;            // 0x13C
	FrButton* m_pMaxBtn;             // 0x140
	FrButton* m_pKazBtn;             // 0x144
};

CHECK_SIZE(FrValEventDlg, 0x148);
BEGIN_LAYOUT_CHECK(FrValEventDlg)
	CHECK_FIELD(FrValEventDlg, m_giftFlag, 0x110);
	CHECK_FIELD(FrValEventDlg, m_hanaCount, 0x114);
	CHECK_FIELD(FrValEventDlg, m_nellCount, 0x119);
	CHECK_FIELD(FrValEventDlg, m_nuriCount, 0x11A);
	CHECK_FIELD(FrValEventDlg, m_kazCount, 0x11D);
	CHECK_FIELD(FrValEventDlg, m_pHanaBtn, 0x120);
	CHECK_FIELD(FrValEventDlg, m_pNellBtn, 0x134);
	CHECK_FIELD(FrValEventDlg, m_pKazBtn, 0x144);
END_LAYOUT_CHECK

// Valentine combination dialog; its size (0x188) is the `new` of
// NtValentineEventDlgMakeInstance.
class NtValentineEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // valentineeventdlg.h:111

	NtValentineEventDlg(void);
	virtual ~NtValentineEventDlg(void);

	void SetUserData(int valen0, int valen1, int valen2, int valen3);
	void SetUserDataWhite(int white0, int white1, int white2, int white3);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnCloseBtnInit(intptr_t param);
	void OnCloseBtnUp(void);
	void OnDetailBtnInit(intptr_t param);
	void OnDetailBtnUp(void);
	void OnCaptionInit(intptr_t param);
	void OnBg1Init(intptr_t param);
	void OnBg2Init(intptr_t param);
	void OnBg3Init(intptr_t param);
	void OnBg4Init(intptr_t param);
	void OnValen0Init(intptr_t param);
	void OnValen1Init(intptr_t param);
	void OnValen2Init(intptr_t param);
	void OnValen3Init(intptr_t param);
	void OnValen0OwnerDraw(void);
	void OnValen1OwnerDraw(void);
	void OnValen2OwnerDraw(void);
	void OnValen3OwnerDraw(void);
	void OnWhite0Init(intptr_t param);
	void OnWhite1Init(intptr_t param);
	void OnWhite2Init(intptr_t param);
	void OnWhite3Init(intptr_t param);
	void OnWhite0OwnerDraw(void);
	void OnWhite1OwnerDraw(void);
	void OnWhite2OwnerDraw(void);
	void OnWhite3OwnerDraw(void);
	void OnCombine0BtnInit(intptr_t param);
	void OnCombine0BtnUp(void);
	void OnCombine1BtnInit(intptr_t param);
	void OnCombine1BtnUp(void);
	void OnCombine2BtnInit(intptr_t param);
	void OnCombine2BtnUp(void);
	void OnCombine3BtnInit(intptr_t param);
	void OnCombine3BtnUp(void);
	bool OnDetailDlgResult(int result, FrForm* form);
	bool OnNotifyDlgResult(int result, FrForm* form);

private:
	static sFRESH_ENTRY _MsgEntries[];

	void UpdateCombinerState(void);

	tagWTITLEFONT m_titleFontInfo;   // 0x110
	WTitleFont* m_pTitleFont;        // 0x130
	FrArea* m_pValen[4];             // 0x134 (OnValen<n>Init)
	FrArea* m_pWhite[4];             // 0x144 (OnWhite<n>Init)
	int m_valenCount[4];             // 0x154
	int m_whiteCount[4];             // 0x164
	FrButton* m_pCombineBtn[4];      // 0x174
	UNKNOWN(0x184, 0x188);
};

CHECK_SIZE(NtValentineEventDlg, 0x188);
BEGIN_LAYOUT_CHECK(NtValentineEventDlg)
	CHECK_FIELD(NtValentineEventDlg, m_titleFontInfo, 0x110);
	CHECK_FIELD(NtValentineEventDlg, m_pTitleFont, 0x130);
	CHECK_FIELD(NtValentineEventDlg, m_pValen, 0x134);
	CHECK_FIELD(NtValentineEventDlg, m_pWhite, 0x144);
	CHECK_FIELD(NtValentineEventDlg, m_valenCount, 0x154);
	CHECK_FIELD(NtValentineEventDlg, m_whiteCount, 0x164);
	CHECK_FIELD(NtValentineEventDlg, m_pCombineBtn, 0x174);
END_LAYOUT_CHECK

// Holes event dialog (five prize windows and a gauge); its size (0x138) is
// the `new` of ntHolesEventDlgMakeInstance.
class ntHolesEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // valentineeventdlg.h:184

	ntHolesEventDlg(void);
	virtual ~ntHolesEventDlg(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnPrize0Init(intptr_t param);
	void OnPrize1Init(intptr_t param);
	void OnPrize2Init(intptr_t param);
	void OnPrize3Init(intptr_t param);
	void OnPrize4Init(intptr_t param);
	void OnGaugeOwnerDraw(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	UNKNOWN(0x110, 0x130);
	WTitleFont* m_pTitleFont;        // 0x130
	UNKNOWN(0x134, 0x138);
};

CHECK_SIZE(ntHolesEventDlg, 0x138);
BEGIN_LAYOUT_CHECK(ntHolesEventDlg)
	CHECK_FIELD(ntHolesEventDlg, m_pTitleFont, 0x130);
END_LAYOUT_CHECK

// Mighty MAC event dialog; only its constructor and destructor are in the
// unit (valentineeventdlg.cpp:1428).
class ntMightyMACEventDlg : public FrForm {
public:
	ntMightyMACEventDlg(void);
	virtual ~ntMightyMACEventDlg(void);
};

#endif // PG_CLIENT_PROJECTG_VALENTINEEVENTDLG_H
