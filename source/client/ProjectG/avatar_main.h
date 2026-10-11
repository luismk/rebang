// Original: d:\build\custom\temp\code\client\projectg\avatar_main.h
#ifndef PG_CLIENT_PROJECTG_AVATAR_MAIN_H
#define PG_CLIENT_PROJECTG_AVATAR_MAIN_H

#include <windows.h>
#include <atlbase.h>
#include <atlapp.h>
#include <atlmisc.h>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "frcmdtarget.h"
#include "scenemanager.h"
#include "wmath.h"
#include "wscene.h"
#include "GolfDoc.h"

class Bitmap;
class WOverlay;
class WTitleFont;
class CQuadTree;
class FrArea;
class FrButton;
class FrComboBox;
class FrEdit;
class FrForm;
class FrFrame;
class FrAvatarUserListDlg;
class FrNoteDlg;
class FrWnd;
class IObject;
class ntAvatar;
enum eSPAVILITYTYPE;

// The avatar (lounge) chat main screen, a CTaskMain (its constructor calls
// CTaskMain's). Its size (0x4EC) is the `new` of ntAvatarChatMainMakeInstance.
// GetRTTI's line (avatar_main.h:43) places the class in this header. Only the
// members the transcribed code uses are declared; the dialog pointers are
// cleared by the result callbacks of their dialogs.
class ntAvatarChatMain : public CTaskMain, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	friend class ntAvatar;         // IsClickable/Render/RenderOverlay read the icons and the shadow texture

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; } // avatar_main.h:43

	ntAvatarChatMain(void);
	virtual ~ntAvatarChatMain(void);

	virtual void OnInit(void);
	virtual void OnPreLoadInit(void);
	virtual void OnDestroy(void);
	virtual void Display(void);
	virtual void DoReLoad(void);
	virtual void OnPreserveBack(const char *nextTask);
	virtual void CloseAllDialogs(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

	bool CanUseSPItem(eSPAVILITYTYPE type);
	bool SpCommand(const char *command);
	void StartSpEffect(ntAvatar *pAvatar, eSPAVILITYTYPE type);

protected:
	void CheckUIPosition(void);
	void ToggleTopViewMode(void);
	void ProcessCamera(float elapsed);
	void PrintText(float x, float y, const char *text, float spacing, int align, float scale, unsigned long color);
	void RefreshAllUccClothes(void);
	CQuadTree &GetPVS(void);
	void SetLocalPVS(CQuadTree &pvs);

	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void InitUI(void);
	void ResetFrWndPointers(void);
	void OnAvatarChatMainInit(int);
	void OnAvatarChatMainFinish(void);
	bool OnAvatarTipDlgResult(int, FrForm*);
	bool OnJukeBoxDlgResult(int, FrForm*);
	bool OnAvatarChatLogDlgResult(int, FrForm*);
	bool OnAvatarUserListDlgResult(int, FrForm*);
	void OnChatTargetBtnDown(int);
	bool OnChatTargetEnterKey(int);
	bool OnReportResult(int, FrForm*);
	bool OnAddFriendDlgResult(int, FrForm*);
	bool OnTradeDlgResult(int, FrForm*);
	void OnRightMenuRBtnDown(void);
	void ChangePortraitImage(int);
	void UpdateRightMenu(void);
	void PopupRightMenu(int, int);
	void ToggleJukeBox(void);
	void ToggleAvatarChatLog(void);
	void ToggleAvatarUserList(void);
	void OnPortraitJukeBoxButtonBtnUp(void);
	void OnPortraitChatLogButtonBtnUp(void);
	void OnAvatarUserListButtonBtnUp(void);
	void OnPortraitInit(intptr_t);
	void OnPortraitMaskAreaInit(int);
	void OnPortraitMaskInit(int);
	void OnPortraitMaskBtnUp(void);
	void OnPortraitJukeBoxButtonAreaInit(int);
	void OnPortraitJukeBoxButtonInit(int);
	void OnPortraitChatLogButtonInit(int);
	void OnAvatarUserListButtonInit(int);
	void OnChatFrameInit(int);
	void OnChatViewInit(intptr_t);
	void OnChatInputInit(intptr_t);
	void OnEmoticonInit(intptr_t);
	void OnLanguageInit(intptr_t);
	void OnReportInit(int);
	void OnRightMenuInit(int);
	bool OnNoteDlgResult(int, FrForm*);
	void OnAddFriend(void);
	void OnFollowAvatar(void);
	void OnKickAvatar(void);
	void OnPortraitTipButtonBtnUp(void);
	void OnChatTargetInit(intptr_t);
	bool OnChatInputEnterKey(intptr_t);
	bool OnEmoticonResult(int, FrForm*);
	void OnReportBtnUp(void);
	void OnNoteToFriend(void);
	void OnOpenTradeShop(void);
	void OnEnterTradeShop(void);
	bool OnPasswordDlgResult(int, FrForm*);
	void OnPortraitTipButtonInit(int);
	void OnEmoticonBtnUp(void);
	void ProcessUI(float);
	void OnAvatarInfo(void);
	void OnRightMenuLBtnDown(void);

	virtual void OnUnderBar_ShopUp(void);
	virtual void OnUnderBar_MyRoomUp(void);
	virtual void OnUnderBar_CookieUp(void);
	virtual void OnUnderBar_RankingUp(void);

	PADDING(0x1E4, 0x1EC);      // not transcribed yet (CRenderFuncPtr base at 0x1E0)
	CWaveInfo m_waveInfo;       // 0x1EC, built and destroyed with the screen
	CRiverInfo m_riverInfo;     // 0x1F8
	float m_decalTime;          // 0x204, pulses the decal Display draws under the target, cleared by the constructor
	bool m_showTargetDecal;     // 0x208, Display draws the target decal
	int m_glowBallTexture;      // 0x20C, "GlowBall.jpg", released by the destructor
	PADDING(0x210, 0x226);
	char m_targetNickName[0x16];    // 0x226, nickname of the avatar the right menu acts on
	unsigned long m_targetUid;      // 0x23C, its uid, sent with a note
	WTL::CString m_bgName;      // 0x240, LoadBg's map name
	WVector m_cameraAngle;      // 0x244, rotation of the camera (RotMat), x -28 degrees at construction
	WVector m_cameraOffset;     // 0x250, WVector::ZERO at construction
	float m_cameraDistance;     // 0x25C, eased toward m_cameraDistanceGoal by ProcessCamera
	float m_cameraDistanceGoal; // 0x260
	float m_cameraClipDistance; // 0x264, the camera distance shortened by the ground ray (ProcessCamera)
	float m_cameraRayHit;       // 0x268, RayIntersectAll's result that set m_cameraClipDistance
	float m_cameraHeight;       // 0x26C, eased toward m_cameraHeightGoal by ProcessCamera
	float m_cameraHeightGoal;   // 0x270
	float m_idleTime;           // 0x274, OnProcess adds the elapsed time, cleared on input
	float m_idleMark;           // 0x278, stored by OnProcess
	PADDING(0x27C, 0x288);
	bool m_topViewMode;         // 0x288, toggled by ToggleTopViewMode
	float m_topViewHeight;      // 0x28C, 256 when the top view starts
	PADDING(0x290, 0x294);
	WVector m_topViewCenter;    // 0x294, the own avatar's position, or the base model's box center
	bool m_initFlag;            // 0x2A0, only cleared by the constructor (no reader found in this class)
	PADDING(0x2A1, 0x2A4);
	WTitleFont *m_pTitleFont;   // 0x2A4, PrintText's font, released by the destructor
	Bitmap* m_portraitBitmaps[10];    // 0x2A8, indexed by ChangePortraitImage
	WOverlay *m_genderIcons[6];     // 0x2D0, i_male, i_female, i_male_02, i_female_02, i_male_03, i_female_03
	WOverlay *m_mannerIcons[2];     // 0x2E8, i_male_manner, i_female_manner
	WOverlay *m_angelIcons[2];      // 0x2F0, i_male_angel, i_female_angel
	WOverlay *m_diveIcon;           // 0x2F8, dive
	WOverlay *m_stateIcon01;        // 0x2FC, state_icon_01
	WOverlay *m_stateIcon02;        // 0x300, state_icon_02
	WOverlay *m_stateIcon03;        // 0x304, state_icon_03
	WOverlay *m_stateIcon04;        // 0x308, state_icon_04
	WOverlay *m_stateIcon05;        // 0x30C, state_icon_05
	WOverlay *m_stateIcon06;        // 0x310, state_icon_06
	WOverlay *m_stateIcon01N;       // 0x314, stateicon_01_n
	WOverlay *m_stateIcon02N;       // 0x318, stateicon_02_n
	WOverlay *m_stateIcon03N;       // 0x31C, stateicon_03_n
	WOverlay *m_stateIcon04N;       // 0x320, stateicon_04_n
	WOverlay *m_stateBars[9];       // 0x324, statebar_%02d
	WOverlay *m_emotiObject;        // 0x348, [emoti_object
	int m_shadowTexture;            // 0x34C, [shadow.jpg
	PADDING(0x350, 0x458);
	FrArea* m_portrait;         // 0x458
	FrButton* m_portraitMask;   // 0x45C
	FrButton* m_jukeBoxButton;      // 0x460
	FrButton* m_tipButton;          // 0x464
	FrButton* m_chatLogButton;      // 0x468
	FrButton* m_userListButton;     // 0x46C
	FrFrame* m_chatFrame;       // 0x470
	FrComboBox* m_chatTarget;   // 0x474
	FrEdit* m_chatInput;        // 0x478, gets the key focus on a chat target click
	FrButton* m_emoticonButton; // 0x47C
	FrArea* m_language;         // 0x480
	FrButton* m_reportButton;   // 0x484
	FrEdit* m_chatView;         // 0x488
	FrForm* m_emoticonDlg;      // 0x48C
	FrForm* m_reportDlg;        // 0x490
	FrNoteDlg* m_noteDlg;       // 0x494
	FrForm* m_addFriendDlg;     // 0x498
	FrForm* m_dlg49C;           // 0x49C, only cleared by ResetFrWndPointers so far
	FrForm* m_passwordDlg;      // 0x4A0
	FrForm* m_jukeBoxDlg;       // 0x4A4
	FrForm* m_tradeDlg;         // 0x4A8
	FrForm* m_avatarTipDlg;     // 0x4AC
	FrForm* m_chatLogDlg;       // 0x4B0
	FrAvatarUserListDlg* m_userListDlg; // 0x4B4
	FrWnd* m_rightMenu;         // 0x4B8, the avatar popup menu
	PADDING(0x4BC, 0x4C0);
	int m_uiCycle;              // 0x4C0, cleared by InitUI, cycled by ProcessUI
	PADDING(0x4C4, 0x4C8);
	unsigned char m_preservedMap;   // 0x4C8, GetCurMap() saved by OnPreserveBack
	unsigned char m_preservedHole;  // 0x4C9, the golf doc's current hole saved by OnPreserveBack
	bool m_spItemChanged;       // 0x4CA, set by HandleMsg, ProcessCamera re-reads the special prize items
	PADDING(0x4CB, 0x4CC);
	LightSet m_savedLightSet;   // 0x4CC, g_lightset copied by LoadBg, restored by OnRestorePreserved
	CQuadTree *m_pPVS;          // 0x4E8, set by SetLocalPVS
};
CHECK_SIZE(ntAvatarChatMain, 0x4EC);
BEGIN_LAYOUT_CHECK(ntAvatarChatMain)
	CHECK_FIELD(ntAvatarChatMain, m_waveInfo, 0x1EC);
	CHECK_FIELD(ntAvatarChatMain, m_riverInfo, 0x1F8);
	CHECK_FIELD(ntAvatarChatMain, m_bgName, 0x240);
	CHECK_FIELD(ntAvatarChatMain, m_decalTime, 0x204);
	CHECK_FIELD(ntAvatarChatMain, m_cameraAngle, 0x244);
	CHECK_FIELD(ntAvatarChatMain, m_cameraOffset, 0x250);
	CHECK_FIELD(ntAvatarChatMain, m_cameraDistance, 0x25C);
	CHECK_FIELD(ntAvatarChatMain, m_cameraRayHit, 0x268);
	CHECK_FIELD(ntAvatarChatMain, m_cameraHeight, 0x26C);
	CHECK_FIELD(ntAvatarChatMain, m_idleMark, 0x278);
	CHECK_FIELD(ntAvatarChatMain, m_spItemChanged, 0x4CA);
	CHECK_FIELD(ntAvatarChatMain, m_showTargetDecal, 0x208);
	CHECK_FIELD(ntAvatarChatMain, m_glowBallTexture, 0x20C);
	CHECK_FIELD(ntAvatarChatMain, m_targetNickName, 0x226);
	CHECK_FIELD(ntAvatarChatMain, m_topViewMode, 0x288);
	CHECK_FIELD(ntAvatarChatMain, m_topViewHeight, 0x28C);
	CHECK_FIELD(ntAvatarChatMain, m_topViewCenter, 0x294);
	CHECK_FIELD(ntAvatarChatMain, m_pTitleFont, 0x2A4);
	CHECK_FIELD(ntAvatarChatMain, m_savedLightSet, 0x4CC);
	CHECK_FIELD(ntAvatarChatMain, m_targetUid, 0x23C);
	CHECK_FIELD(ntAvatarChatMain, m_portraitBitmaps, 0x2A8);
	CHECK_FIELD(ntAvatarChatMain, m_genderIcons, 0x2D0);
	CHECK_FIELD(ntAvatarChatMain, m_mannerIcons, 0x2E8);
	CHECK_FIELD(ntAvatarChatMain, m_angelIcons, 0x2F0);
	CHECK_FIELD(ntAvatarChatMain, m_diveIcon, 0x2F8);
	CHECK_FIELD(ntAvatarChatMain, m_stateIcon01, 0x2FC);
	CHECK_FIELD(ntAvatarChatMain, m_stateIcon06, 0x310);
	CHECK_FIELD(ntAvatarChatMain, m_stateIcon01N, 0x314);
	CHECK_FIELD(ntAvatarChatMain, m_stateIcon04N, 0x320);
	CHECK_FIELD(ntAvatarChatMain, m_stateBars, 0x324);
	CHECK_FIELD(ntAvatarChatMain, m_emotiObject, 0x348);
	CHECK_FIELD(ntAvatarChatMain, m_shadowTexture, 0x34C);
	CHECK_FIELD(ntAvatarChatMain, m_portrait, 0x458);
	CHECK_FIELD(ntAvatarChatMain, m_jukeBoxButton, 0x460);
	CHECK_FIELD(ntAvatarChatMain, m_tipButton, 0x464);
	CHECK_FIELD(ntAvatarChatMain, m_chatLogButton, 0x468);
	CHECK_FIELD(ntAvatarChatMain, m_userListButton, 0x46C);
	CHECK_FIELD(ntAvatarChatMain, m_portraitMask, 0x45C);
	CHECK_FIELD(ntAvatarChatMain, m_chatFrame, 0x470);
	CHECK_FIELD(ntAvatarChatMain, m_chatTarget, 0x474);
	CHECK_FIELD(ntAvatarChatMain, m_chatInput, 0x478);
	CHECK_FIELD(ntAvatarChatMain, m_emoticonButton, 0x47C);
	CHECK_FIELD(ntAvatarChatMain, m_language, 0x480);
	CHECK_FIELD(ntAvatarChatMain, m_reportButton, 0x484);
	CHECK_FIELD(ntAvatarChatMain, m_chatView, 0x488);
	CHECK_FIELD(ntAvatarChatMain, m_emoticonDlg, 0x48C);
	CHECK_FIELD(ntAvatarChatMain, m_reportDlg, 0x490);
	CHECK_FIELD(ntAvatarChatMain, m_noteDlg, 0x494);
	CHECK_FIELD(ntAvatarChatMain, m_addFriendDlg, 0x498);
	CHECK_FIELD(ntAvatarChatMain, m_dlg49C, 0x49C);
	CHECK_FIELD(ntAvatarChatMain, m_passwordDlg, 0x4A0);
	CHECK_FIELD(ntAvatarChatMain, m_jukeBoxDlg, 0x4A4);
	CHECK_FIELD(ntAvatarChatMain, m_tradeDlg, 0x4A8);
	CHECK_FIELD(ntAvatarChatMain, m_avatarTipDlg, 0x4AC);
	CHECK_FIELD(ntAvatarChatMain, m_chatLogDlg, 0x4B0);
	CHECK_FIELD(ntAvatarChatMain, m_userListDlg, 0x4B4);
	CHECK_FIELD(ntAvatarChatMain, m_rightMenu, 0x4B8);
	CHECK_FIELD(ntAvatarChatMain, m_uiCycle, 0x4C0);
	CHECK_FIELD(ntAvatarChatMain, m_preservedMap, 0x4C8);
	CHECK_FIELD(ntAvatarChatMain, m_preservedHole, 0x4C9);
	CHECK_FIELD(ntAvatarChatMain, m_pPVS, 0x4E8);
END_LAYOUT_CHECK

IObject *ntAvatarChatMainMakeInstance(void);

#endif // PG_CLIENT_PROJECTG_AVATAR_MAIN_H
