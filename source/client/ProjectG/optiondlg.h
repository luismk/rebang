// Original: d:\build\custom\temp\code\client\projectg\optiondlg.h
#ifndef PG_CLIENT_PROJECTG_OPTIONDLG_H
#define PG_CLIENT_PROJECTG_OPTIONDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "ClientSetting.h"

class FrButton;
class FrStatic;
class FrEdit;
class FrComboBox;
class FrGaugeBarEx;
class FrSchoolDlg;

class FrOptionDlg;

// Nickname change dialog; its size (0x120) is the `new` of
// FrChangeNickDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrChangeNickDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // optiondlg.h:22

	FrChangeNickDlg(void);                // optiondlg.cpp:398
	virtual ~FrChangeNickDlg(void) {}     // optiondlg.h:26

	void SetOptionDlg(FrOptionDlg* pOptionDlg) { m_pOptionDlg = pOptionDlg; }   // optiondlg.h:30
	void SetNick(const char* szNick) { m_szNick = szNick; }                      // optiondlg.h:31

	virtual bool OnInit(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // optiondlg.cpp:383

	void OnPriceStaticInit(int param);          // optiondlg.cpp:414
	void OnPriceInit(intptr_t param);                // optiondlg.cpp:430
	void OnMyCookieStaticInit(int param);       // optiondlg.cpp:454
	void OnMyCookieInit(int param);             // optiondlg.cpp:470
	void OnRemainCookieStaticInit(int param);   // optiondlg.cpp:495
	void OnRemainCookieInit(int param);         // optiondlg.cpp:511
	void OnCheckoutInit(intptr_t param);             // optiondlg.cpp:545
	void OnCheckoutBtnUp(void);                 // optiondlg.cpp:561
	bool OnChangeNickConfirmDlgResult(int result, FrForm* pForm);

	FrOptionDlg* m_pOptionDlg;          // 0x110 (SetOptionDlg)
	const char* m_szNick;               // 0x114, the requested nickname (SetNick)
	bool m_bHasTicket;                  // 0x118, the player owns a nickname change ticket (item 0x1A000003)

private:
	PADDING(0x119, 0x120);
};

CHECK_SIZE(FrChangeNickDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrChangeNickDlg)
	CHECK_FIELD(FrChangeNickDlg, m_pOptionDlg, 0x110);
	CHECK_FIELD(FrChangeNickDlg, m_szNick, 0x114);
	CHECK_FIELD(FrChangeNickDlg, m_bHasTicket, 0x118);
END_LAYOUT_CHECK

// The option dialog: four tabs (game, video, audio, macro) editing a copy of
// the client options. Its size (0x6c8) is the `new` of FrOptionDlgMakeInstance.
class FrOptionDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // optiondlg.h:69

	// One tab: its button and the windows it shows (constructor at
	// optiondlg.cpp:125). The tabs have no destructor.
	struct sTab {
		sTab(void);

		virtual void SetSelected(bool);     // optiondlg.cpp:130
		virtual void Init(void);            // optiondlg.h:261

		bool m_bSelected;                   // 0x4 (true after construction)
		FrButton* m_pTabBtn;                // 0x8
		int m_numWnds;                      // 0xc
		FrWnd** m_ppWnds;                   // 0x10
		FrWnd*** m_pppHoverWnds;            // 0x14, the hoverable controls, per window
		bool* m_pbDisabled;                 // 0x18, per window, tested by SetSelected
	};

	// The game tab: 13 labels, then its controls; m_ppWnds is m_pStatics and
	// m_pppHoverWnds is m_pHoverWnds, which point at the hoverable controls.
	struct sGameTab : public sTab {
		sGameTab(void);
		virtual void SetSelected(bool);
		virtual void Init(void);            // optiondlg.cpp:242
		bool IsEnableChangeNickname(void);   // optiondlg.cpp:250

		FrStatic* m_pStatics[13];           // 0x1c (mouse sensitivity, whisper, invitation,
		                                    // friend confirm, chat window, UI effect, nick,
		                                    // school, power gauge, hide GUI, hide PI,
		                                    // terrain tooltip, cut-in display)
		FrWnd** m_pHoverWnds[13];           // 0x50
		FrGaugeBarEx* m_pMsSensiGauge;      // 0x84
		FrButton* m_pWhisperBtn;            // 0x88
		FrButton* m_pInvitationBtn;         // 0x8c
		FrButton* m_pFriendConfirmBtn;      // 0x90
		FrButton* m_pTransChatWinBtn;       // 0x94
		FrButton* m_pUiEffectBtn;           // 0x98
		FrEdit* m_pNickEdit;                // 0x9c
		FrButton* m_pChangeNickBtn;         // 0xa0
		FrEdit* m_pSchoolEdit;              // 0xa4
		FrButton* m_pChangeSchoolBtn;       // 0xa8
		FrButton* m_pPowerGaugeBtn;         // 0xac
		FrButton* m_pHideGUIBtn;            // 0xb0
		FrButton* m_pHidePIBtn;             // 0xb4
		FrButton* m_pTerrainBtn;            // 0xb8
		FrButton* m_pCutinDisplayBtn;       // 0xbc
		bool m_bEnableChangeNick;           // 0xc0
	};

	// The video tab: 13 labels, their hover pointers and disabled flags,
	// then its controls.
	struct sVideoTab : public sTab {
		sVideoTab(void);

		FrStatic* m_pStatics[13];           // 0x1c (full screen, fill screen, wide mode,
		                                    // overall, resolution, T&L, texture, shadow,
		                                    // shadow map, reflection, LOD, NPC, particle)
		FrWnd** m_pHoverWnds[13];           // 0x50
		bool m_bDisabled[13];               // 0x84
		FrButton* m_pWideModeBtn;           // 0x94
		FrButton* m_pFullScreenBtn;         // 0x98
		FrButton* m_pFillScreenBtn;         // 0x9c
		FrComboBox* m_pOverallCmb;          // 0xa0
		FrComboBox* m_pScrResCmb;           // 0xa4
		FrComboBox* m_pTnlCmb;              // 0xa8
		FrComboBox* m_pTexResCmb;           // 0xac
		FrButton* m_pShadowBtn;             // 0xb0
		FrButton* m_pShadowmapBtn;          // 0xb4
		FrButton* m_pReflectionBtn;         // 0xb8
		FrButton* m_pLodBtn;                // 0xbc
		FrButton* m_pNpcBtn;                // 0xc0
		FrComboBox* m_pParticleCmb;         // 0xc4
	};

	// The audio tab: 5 labels (enable, overall, SFX, BGM, speaker).
	struct sAudioTab : public sTab {
		sAudioTab(void);

		FrStatic* m_pStatics[5];            // 0x1c
		FrWnd** m_pHoverWnds[5];            // 0x30
		bool m_bDisabled[5];                // 0x44
		FrButton* m_pEnableBtn;             // 0x4c
		FrComboBox* m_pOverallCmb;          // 0x50
		FrGaugeBarEx* m_pSfxGauge;          // 0x54
		FrGaugeBarEx* m_pBgmGauge;          // 0x58
		FrComboBox* m_pSpeakerCmb;          // 0x5c
	};

	// The macro tab: 8 labels and 8 macro edits.
	struct sMacroTab : public sTab {
		sMacroTab(void);

		FrStatic* m_pStatics[8];            // 0x1c
		FrWnd** m_pHoverWnds[8];            // 0x3c
		bool m_bDisabled[8];                // 0x5c
		FrEdit* m_pMacroEdits[8];           // 0x64
	};

	FrOptionDlg(void);                  // optiondlg.cpp:917
	virtual ~FrOptionDlg(void);         // optiondlg.cpp:938

	void SetApplyScrSize(bool bApply);  // optiondlg.cpp:949
	void EnableChangeNickControls(bool bEnable);   // optiondlg.cpp:2398
	void CloseDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // optiondlg.cpp:747

	enum eTabType {
		TAB_GAME = 0,
		TAB_VIDEO = 1,
		TAB_AUDIO = 2,
		TAB_MACRO = 3
	};

	static eTabType ms_eCurTab;            // the tab OnInit opens

public:
	void OpenTab(eTabType type);           // optiondlg.cpp:954

protected:
	virtual bool OnInit(void);             // optiondlg.cpp:1797
	virtual bool OnLButtonDown(const WPoint& pt);   // optiondlg.cpp:1824
	virtual void OnProc(const float deltaTime);     // optiondlg.cpp:1838
	virtual void OnCancel(void);           // optiondlg.cpp:2302
	virtual void OnOK(void);               // optiondlg.cpp:2326
	void SetGameColtrols(void);
	void InitVideoControls(void);
	void SetVideoColtrols(bool);
	void SetAudioColtrols(void);           // optiondlg.cpp:2213
	void SetMacroColtrols(void);           // optiondlg.cpp:2243
	bool OnChangeNickConfirmDlgResult(int result, FrForm* pForm);   // optiondlg.cpp:2579
	bool OnChangeSchoolDlgResult(int result, FrForm* pForm);   // optiondlg.cpp:2588

	void SetTab(eTabType type);
	void OnGameTabUp(void);                // optiondlg.cpp:973
	void OnVideoTabUp(void);               // optiondlg.cpp:984
	void OnAudioTabUp(void);               // optiondlg.cpp:995
	void OnMacroTabUp(void);               // optiondlg.cpp:1006
	bool gOnNickEditEnterKey(int key);    // optiondlg.cpp:1237
	bool gOnSchoolEditEnterKey(int key);  // optiondlg.cpp:1276
	void gOnTerrainBtnUp(void);            // optiondlg.cpp:1335
	bool mOnMacroEnterKey(int key);        // optiondlg.cpp:1715

	void OnOkBtnInit(intptr_t param);
	void OnGameTabInit(int param);
	void OnVideoTabInit(int param);
	void OnAudioTabInit(int param);
	void OnMacroTabInit(int param);
	void gOnMsSensiStaticInit(int param);
	void gOnMsSensiGaugeInit(int param);
	void gOnWhisperStaticInit(int param);
	void gOnWhisperBtnInit(intptr_t param);
	void gOnWhisperBtnUp(void);
	void gOnInvitationStaticInit(int param);
	void gOnInvitationBtnInit(int param);
	void gOnInvitationBtnUp(void);
	void gOnFriendConfirmStaticInit(int param);
	void gOnFriendConfirmBtnInit(int param);
	void gOnFriendConfirmBtnUp(void);
	void gOnTransChatWinStaticInit(int param);
	void gOnTransChatWinBtnInit(int param);
	void gOnTransChatWinBtnUp(void);
	void gOnUiEffectStaticInit(int param);
	void gOnTerrainTooltipStaticInit(int param);
	void gOnUiEffectBtnInit(int param);
	void gOnUiEffectBtnUp(void);
	void gOnNickStaticInit(intptr_t param);
	void gOnNickEditInit(int param);
	void gOnChangeNickBtnInit(int param);
	void gOnSchoolStaticInit(int param);
	void gOnSchoolEditInit(int param);
	void gOnChangeSchoolBtnInit(int param);
	void gOnChangeSchoolBtnUp(void);
	void gOnPowerGaugeStaticInit(int param);
	void gOnPowerGaugeBtnInit(int param);
	void gOnPowerGaugeBtnUp(void);
	void gOnHideGUIStaticInit(int param);
	void gOnHideGUIBtnInit(int param);
	void gOnHideGUIBtnUp(void);
	void gOnHidePIStaticInit(int param);
	void gOnHidePIBtnInit(int param);
	void gOnHidePIBtnUp(void);
	void gOnTerrainBtnInit(int param);
	void gOnCutinDisplayStaticInit(int param);
	void gOnCutinDisplayBtnInit(int param);
	void gOnCutinDisplayBtnUp(void);
	void vOnFullScreenStaticInit(int param);
	void vOnFullScreenBtnInit(int param);
	void vOnFullScreenBtnUp(void);
	void vOnFillScreenStaticInit(int param);
	void vOnFillScreenBtnInit(int param);
	void vOnFillScreenBtnUp(void);
	void vOnWideModeStaticInit(int param);
	void vOnWideModeBtnInit(int param);
	void vOnWideModeBtnUp(void);
	void vOnOverallStaticInit(int param);
	void vOnOverallCmbInit(int param);
	void vOnScrResStaticInit(int param);
	void vOnScrResCmbInit(int param);
	void vOnTnlStaticInit(int param);
	void vOnTnlCmbInit(int param);
	void vOnTexResStaticInit(int param);
	void vOnTexResCmbInit(int param);
	void vOnShadowStaticInit(int param);
	void vOnShadowBtnInit(int param);
	void vOnShadowBtnUp(void);
	void vOnShadowmapStaticInit(int param);
	void vOnShadowmapBtnInit(int param);
	void vOnShadowmapBtnUp(void);
	void vOnReflectionStaticInit(int param);
	void vOnReflectionBtnInit(int param);
	void vOnReflectionBtnUp(void);
	void vOnLodStaticInit(int param);
	void vOnLodBtnInit(int param);
	void vOnLodBtnUp(void);
	void vOnNpcStaticInit(int param);
	void vOnNpcBtnInit(int param);
	void vOnNpcBtnUp(void);
	void vOnParticleStaticInit(int param);
	void vOnParticleCmbInit(int param);
	void aOnEnableStaticInit(int param);
	void aOnEnableBtnInit(int param);
	void aOnEnableBtnUp(void);
	void aOnOverallStaticInit(int param);
	void aOnOverallCmbInit(int param);
	void aOnSfxStaticInit(int param);
	void aOnSfxGaugeInit(int param);
	void aOnBgmStaticInit(int param);
	void aOnBgmGaugeInit(int param);
	void aOnSpeakerStaticInit(int param);
	void aOnSpeakerCmbInit(int param);
	void mOnMacroStatic1Init(int param);
	void mOnMacroStatic2Init(int param);
	void mOnMacroEnterKeyLBtnDown(void);
	void mOnMacroStatic3Init(int param);
	void mOnMacroStatic4Init(int param);
	void mOnMacroStatic5Init(int param);
	void mOnMacroStatic6Init(int param);
	void mOnMacroStatic7Init(int param);
	void mOnMacroStatic8Init(int param);
	void mOnMacro1Init(int param);
	void mOnMacro2Init(int param);
	void mOnMacro3Init(int param);
	void mOnMacro4Init(int param);
	void mOnMacro5Init(int param);
	void mOnMacro6Init(int param);
	void mOnMacro7Init(int param);
	void mOnMacro8Init(int param);
	void tOnHintInit(int param);
	const char* MakeHint(eTabType type, int index) const;
	void ProcessHoverMessage(FrWnd* pWnd);   // optiondlg.cpp:2532
	void OnHoverOn(int param);
	void OnHoverOff(int param);

	bool m_bApplyScrSize;               // 0x110
	sOption m_option;                   // 0x114, edited copy of COption's block
	bool m_bChanged1;                   // 0x42c
	bool m_bChanged2;                   // 0x42d
	FrButton* m_pOkBtn;                 // 0x430
	sGameTab m_gameTab;                 // 0x434
	sVideoTab m_videoTab;               // 0x4f8
	sAudioTab m_audioTab;               // 0x5c0
	sMacroTab m_macroTab;               // 0x620
	sTab* m_pTabs[4];                   // 0x6a4, indexed by eTabType
	int m_state1;                       // 0x6b4
	FrSchoolDlg* m_pSchoolDlg;          // 0x6b8, open school search dialog
	FrEdit* m_pHintEdit;                // 0x6bc
	int m_hoverCount;                   // 0x6c0, OnHoverOn/OnHoverOff
	PADDING(0x6c4, 0x6c8);
};

CHECK_SIZE(FrOptionDlg::sTab, 0x1c);
CHECK_SIZE(FrOptionDlg::sGameTab, 0xc4);
CHECK_SIZE(FrOptionDlg::sVideoTab, 0xc8);
CHECK_SIZE(FrOptionDlg::sAudioTab, 0x60);
CHECK_SIZE(FrOptionDlg::sMacroTab, 0x84);
CHECK_SIZE(FrOptionDlg, 0x6c8);
BEGIN_LAYOUT_CHECK(FrOptionDlg)
	CHECK_FIELD(FrOptionDlg, m_bApplyScrSize, 0x110);
	CHECK_FIELD(FrOptionDlg, m_option, 0x114);
	CHECK_FIELD(FrOptionDlg, m_bChanged1, 0x42c);
	CHECK_FIELD(FrOptionDlg, m_bChanged2, 0x42d);
	CHECK_FIELD(FrOptionDlg, m_pOkBtn, 0x430);
	CHECK_FIELD(FrOptionDlg, m_gameTab, 0x434);
	CHECK_FIELD(FrOptionDlg, m_videoTab, 0x4f8);
	CHECK_FIELD(FrOptionDlg, m_audioTab, 0x5c0);
	CHECK_FIELD(FrOptionDlg, m_macroTab, 0x620);
	CHECK_FIELD(FrOptionDlg, m_pTabs, 0x6a4);
	CHECK_FIELD(FrOptionDlg, m_state1, 0x6b4);
	CHECK_FIELD(FrOptionDlg, m_pHintEdit, 0x6bc);
	CHECK_FIELD(FrOptionDlg, m_hoverCount, 0x6c0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_OPTIONDLG_H
