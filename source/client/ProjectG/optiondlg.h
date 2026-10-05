#pragma once

#include "clientsetting.h"

class FrSchoolDlg;
class FrOptionDlg;

class FrChangeNickDlg : public FrForm
{
	DECLARE_OBJECT(FrChangeNickDlg)

	FrChangeNickDlg();

	virtual ~FrChangeNickDlg() { }

	virtual bool OnInit();

	void SetOptionDlg(FrOptionDlg* pDlg) { m_pOptionDlg = pDlg; }
	void SetNick(const char* nick) { m_nick = nick; }

protected:
	void OnPriceStaticInit(int param);
	void OnPriceInit(int param);
	void OnMyCookieStaticInit(int param);
	void OnMyCookieInit(int param);
	void OnRemainCookieStaticInit(int param);
	void OnRemainCookieInit(int param);
	void OnCheckoutInit(int param);
	void OnCheckoutBtnUp();

	bool OnChangeNickConfirmDlgResult(int result, FrForm* form);

	FrOptionDlg* m_pOptionDlg;
	const char* m_nick;
	bool m_bHaveItem;

	DECLARE_FRESH_MSGMAP()
};

class FrOptionDlg : public FrForm
{
	DECLARE_OBJECT(FrOptionDlg)

	enum eTabType
	{
		TAB_GAME,
		TAB_VIDEO,
		TAB_AUDIO,
		TAB_MACRO,
		TAB_NUM,
	};

	FrOptionDlg();
	virtual ~FrOptionDlg();

	void SetApplyScrSize(bool bApply);
	void OpenTab(eTabType tab);
	void CloseDlg();
	void EnableChangeNickControls(bool bEnable);

protected:
	virtual bool OnInit();
	virtual void OnProc(const float delta);
	virtual bool OnLButtonDown(const WPoint& pt);
	virtual void OnOK();
	virtual void OnCancel();

	void OnOkBtnInit(int param);
	void OnGameTabInit(int param);
	void OnVideoTabInit(int param);
	void OnAudioTabInit(int param);
	void OnMacroTabInit(int param);
	void OnGameTabUp();
	void OnVideoTabUp();
	void OnAudioTabUp();
	void OnMacroTabUp();

	void gOnMsSensiStaticInit(int param);
	void gOnMsSensiGaugeInit(int param);
	void gOnWhisperStaticInit(int param);
	void gOnWhisperBtnInit(int param);
	void gOnWhisperBtnUp();
	void gOnInvitationStaticInit(int param);
	void gOnInvitationBtnInit(int param);
	void gOnInvitationBtnUp();
	void gOnFriendConfirmStaticInit(int param);
	void gOnFriendConfirmBtnInit(int param);
	void gOnFriendConfirmBtnUp();
	void gOnTransChatWinStaticInit(int param);
	void gOnTransChatWinBtnInit(int param);
	void gOnTransChatWinBtnUp();
	void gOnUiEffectStaticInit(int param);
	void gOnTerrainTooltipStaticInit(int param);
	void gOnUiEffectBtnInit(int param);
	void gOnUiEffectBtnUp();
	void gOnNickStaticInit(int param);
	void gOnNickEditInit(int param);
	bool gOnNickEditEnterKey(int param);
	void gOnChangeNickBtnInit(int param);
	void gOnChangeNickBtnUp();
	void gOnSchoolStaticInit(int param);
	void gOnSchoolEditInit(int param);
	bool gOnSchoolEditEnterKey(int param);
	void gOnChangeSchoolBtnInit(int param);
	void gOnChangeSchoolBtnUp();
	void gOnPowerGaugeStaticInit(int param);
	void gOnPowerGaugeBtnInit(int param);
	void gOnPowerGaugeBtnUp();
	void gOnHideGUIStaticInit(int param);
	void gOnHideGUIBtnInit(int param);
	void gOnHideGUIBtnUp();
	void gOnHidePIStaticInit(int param);
	void gOnHidePIBtnInit(int param);
	void gOnHidePIBtnUp();
	void gOnTerrainBtnInit(int param);
	void gOnTerrainBtnUp();
	void gOnCutinDisplayStaticInit(int param);
	void gOnCutinDisplayBtnInit(int param);
	void gOnCutinDisplayBtnUp();

	void vOnFullScreenStaticInit(int param);
	void vOnFullScreenBtnInit(int param);
	void vOnFullScreenBtnUp();
	void vOnFillScreenStaticInit(int param);
	void vOnFillScreenBtnInit(int param);
	void vOnFillScreenBtnUp();
	void vOnWideModeStaticInit(int param);
	void vOnWideModeBtnInit(int param);
	void vOnWideModeBtnUp();
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
	void vOnShadowBtnUp();
	void vOnShadowmapStaticInit(int param);
	void vOnShadowmapBtnInit(int param);
	void vOnShadowmapBtnUp();
	void vOnReflectionStaticInit(int param);
	void vOnReflectionBtnInit(int param);
	void vOnReflectionBtnUp();
	void vOnLodStaticInit(int param);
	void vOnLodBtnInit(int param);
	void vOnLodBtnUp();
	void vOnNpcStaticInit(int param);
	void vOnNpcBtnInit(int param);
	void vOnNpcBtnUp();
	void vOnParticleStaticInit(int param);
	void vOnParticleCmbInit(int param);

	void aOnEnableStaticInit(int param);
	void aOnEnableBtnInit(int param);
	void aOnEnableBtnUp();
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
	bool mOnMacroEnterKey(int param);
	void mOnMacroEnterKeyLBtnDown();

	void tOnHintInit(int param);
	void OnHoverOn(int param);
	void OnHoverOff(int param);
	const char* MakeHint(eTabType tab, int index) const;
	void ProcessHoverMessage(FrWnd* pWnd);

	void InitVideoControls();
	void SetGameColtrols();
	void SetVideoColtrols(bool bReset);
	void SetAudioColtrols();
	void SetMacroColtrols();
	void SetTab(eTabType tab);

	bool OnChangeNickConfirmDlgResult(int result, FrForm* form);
	bool OnChangeSchoolDlgResult(int result, FrForm* form);

	static eTabType ms_eCurTab;

public:
	struct sTab
	{
		sTab();

		virtual void SetSelected(bool bSelect);

		virtual void Init() { }

		bool m_bSelected;
		FrButton* m_pTabBtn;
		int m_num;
		FrStatic** m_ppStatic;
		FrWnd*** m_pppCtrl;
		bool* m_pbDisable;
	};

	struct sGameTab : public sTab
	{
		sGameTab();

		virtual void SetSelected(bool bSelect);
		virtual void Init();

		bool IsEnableChangeNickname();

		FrStatic* m_pStatic[13];
		FrWnd** m_ppCtrl[13];
		FrGaugeBarEx* m_pMsSensiGauge;
		FrButton* m_pWhisperBtn;
		FrButton* m_pInvitationBtn;
		FrButton* m_pFriendConfirmBtn;
		FrButton* m_pTransChatWinBtn;
		FrButton* m_pUiEffectBtn;
		FrEdit* m_pNickEdit;
		FrButton* m_pChangeNickBtn;
		FrEdit* m_pSchoolEdit;
		FrButton* m_pChangeSchoolBtn;
		FrButton* m_pPowerGaugeBtn;
		FrButton* m_pHideGUIBtn;
		FrButton* m_pHidePIBtn;
		FrButton* m_pTerrainBtn;
		FrButton* m_pCutinDisplayBtn;
		bool m_bEnableChangeNick;
	};

	struct sVideoTab : public sTab
	{
		sVideoTab();

		FrStatic* m_pStatic[13];
		FrWnd** m_ppCtrl[13];
		bool m_bDisable[13];
		FrButton* m_pWideModeBtn;
		FrButton* m_pFullScreenBtn;
		FrButton* m_pFillScreenBtn;
		FrComboBox* m_pOverallCmb;
		FrComboBox* m_pScrResCmb;
		FrComboBox* m_pTnlCmb;
		FrComboBox* m_pTexResCmb;
		FrButton* m_pShadowBtn;
		FrButton* m_pShadowmapBtn;
		FrButton* m_pReflectionBtn;
		FrButton* m_pLodBtn;
		FrButton* m_pNpcBtn;
		FrComboBox* m_pParticleCmb;
	};

	struct sAudioTab : public sTab
	{
		sAudioTab();

		FrStatic* m_pStatic[5];
		FrWnd** m_ppCtrl[5];
		bool m_bDisable[5];
		FrButton* m_pEnableBtn;
		FrComboBox* m_pOverallCmb;
		FrGaugeBarEx* m_pSfxGauge;
		FrGaugeBarEx* m_pBgmGauge;
		FrComboBox* m_pSpeakerCmb;
	};

	struct sMacroTab : public sTab
	{
		sMacroTab();

		FrStatic* m_pStatic[8];
		FrWnd** m_ppCtrl[8];
		bool m_bDisable[8];
		FrEdit* m_pMacro[8];
	};

protected:
	bool m_bApplyScrSize;
	sOption m_option;
	bool m_bApply;
	bool m_bOK;
	FrButton* m_pOkBtn;
	sGameTab m_gameTab;
	sVideoTab m_videoTab;
	sAudioTab m_audioTab;
	sMacroTab m_macroTab;
	sTab* m_pTab[4];
	FrChangeNickDlg* m_pChangeNickDlg;
	FrSchoolDlg* m_pSchoolDlg;
	FrEdit* m_pHint;
	int m_hoverCount;

	DECLARE_FRESH_MSGMAP()
};
