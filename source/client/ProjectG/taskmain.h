// Original: d:\build\custom\temp\code\client\projectg\taskmain.h
#ifndef PG_CLIENT_PROJECTG_TASKMAIN_H
#define PG_CLIENT_PROJECTG_TASKMAIN_H

#include "..\..\defines.h"
#include "ToolTip.h"
#include "actor.h"
#include "frcmdtarget.h"
#include <vector>
#include <string>

class FrForm;
class FrSPCardBuffDlg;
class FrGiftDlg;
class FrServerDlg;
class FrNoticeDlg;
class FrButton;
class WTitleFont;
class FrWnd;
class FrArea;
class FrStatic;
class FrOptionDlg;
class FrViewer;
class FrSubEquipBar;
class WPoint;
class WRect;
class FrGraphicInterface;
class MsgObject;
enum PANGYA_ITEM_GROUP;
class Bitmap;
class WRTTI;
struct sFRESH_ENTRY;
struct sFRESH_MSGMAP;

// Base of the main tasks' screens: IActor at 0x0, FrCmdTarget at 0x18 (the
// constructor calls both). The virtuals CTaskMain adds follow IActor's in
// ??_7CTaskMain@@6BIActor@@@: CloseAllDialogs, OnResultSubEquipBar,
// OnUnderBar_ShopUp, OnUnderBar_MyRoomUp, OnUnderBar_CookieUp,
// OnUnderBar_GuildUp.
class CTaskMain : public IActor, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;

public:
	CTaskMain(void) {}   // taskmain.h:93
	virtual ~CTaskMain(void) {}   // taskmain.h:94

	static const WRTTI m_RTTI;
	virtual const WRTTI *GetRTTI(void) const;   // taskmain.h:37

	virtual void CloseAllDialogs(void);
	virtual void OnProcess(float);
	virtual void OnPreserveBack(const char *);

	void CheckNaviButtons(void);
	void ChangeRealMyRoom(void);
	void CreateMaketDlg(void);
	void MakeRealMyRoom(void);
	bool IsActiveRecycleDlg(void);
	bool IsActiveScratchDlg(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnRestorePreserved(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP *GetMessageMap(void) const;

	void CalcUIOffset(bool fixed);
	void SetCenterOffset(FrWnd *wnd, float x, float y);
	void SetLeftOffset(FrWnd *wnd, float x);
	void SetBGMNone(void);
	void OpenSubEquipBar(const WPoint &pt, PANGYA_ITEM_GROUP group, int subType);
	void OnUnderBar_FrontUp(void);
	void OnOverBar_MessengerUp(void);
	bool OnNoticeDlgResult(int, FrForm *);
	bool OnOnelineReqChkOutDlgResult(int, FrForm *);
	bool OnOnelineReqDlgResult(int, FrForm *);
	void InitMessenger(void);
	void InitBongdariShop(void);
	void InitUnderBar(void);
	void OnUnderBar_TikiMagicBoxInit(int);
	void EnableUnderBar(bool enable);
	void EnableOverAndUnderBar(bool enable);
	void ShowAllSubSelection(void);
	void CloseAllSubSelection(void);
	void OnOverBar_TabDown(void);
	void OnOverBar_CharDown(void);
	void OnOverBar_CaddieDown(void);
	void OnOverBar_ClubDown(void);
	void OnOverBar_AztecDown(void);
	void OnOverBar_MascotDown(void);
	void OnOverBar_OneLineViewOwnerDraw(int);
	void OnBarTooltipOwnerDraw(int);
	void OnMessengerAlarmBtnUp(void);
	bool OnQuitDlgResult(int, FrForm *);
	bool OnOptionDlgResult(int, FrForm *);
	bool OnNotifyOneDlgResult(int, FrForm *);
	bool OnLoginRsDlgResult(int, FrForm *);
	bool OnMaketListDlgResult(int, FrForm *);
	bool OnKbdManualDlgResult(int, FrForm *);
	bool OnSpecialCardFormResult(int, FrForm *);
	const char *MakeStatusString(int slot, int value, int slotValue);
	const char *MakeStatusString2(int current, int total, int penalty);
	const char *MakeStatusString3(int, int, int);
	const char *MakeCardStatusString(int slot, int value);
	bool OnRMR_ResultSetting(int result, FrForm *form);
	void TurnOn3DBackground(void);
	void TurnOff3DBackground(void);
	bool CompletedMission_Result(int, FrForm *);
	void SetRandomBGM(void);
	void SetConstantBGM(const char* name, bool bOnce, bool bForce);
	void OnUnderBar_GameUp(void);
	void OnUnderBar_ServerUp(void);
	void OnUnderBar_RankingUp(void);
	void OnUnderBar_NoticeUp(void);
	void OnOverBar_SpecialCardUp(void);
	void OnBarHoverOff(intptr_t);
	void SetTimeVariableBGM(void);
	void HandleMsgCommon(const MsgObject& msg);
	void OpenInformation(unsigned long);
	bool OnBongdariDlgResult(int, FrForm *);
	void OnUnderBar_BackUp(void);
	void InitControls(void);
	void InitOverBar(void);
	void ProcessOverBar(void);
	void SendSubSelectionPacket(PANGYA_ITEM_GROUP group);
	void OnOverBar_MyInfoUp(void);
	void OnUnderBar_ItemStorageUp(void);
	void OnMessengerGiftAlarmBtnUp(void);
	bool OnNotifyDlgResult(int, FrForm *);
	bool OnServerDlgResult(int, FrForm *);
	bool OnRecycleDlgResult(int, FrForm *);
	bool OnTikiMagicBoxDlgResult(int, FrForm *);
	bool OnLevelupFormResult(int, FrForm *);
	void OnTreasureAlarmUp(void);
	bool OnScratchDlgResult(int, FrForm *);
	void OnOverBar_TitleInit(intptr_t);
	bool GetBtnState(unsigned char frame, eBarButton button);
	void TurnOnUnderTabBar(void);
	void TurnOffUnderTabBar(void);
	void OnUnderBar_TabBtnUp(void);
	void OpenWaitedLayout(void);
	void ShowOverAndUnderBar(bool show);
	void OnOverBar_Init(intptr_t);
	void OnUnderBar_BackInit(intptr_t);
	void OnUnderBar_FrontInit(intptr_t);
	void OnUnderBar_CookieInit(intptr_t);
	void OnUnderBar_ExitInit(intptr_t);
	void OnUnderBar_TabToolTipInit(intptr_t);
	void OnUnderBar_MaketInit(intptr_t);
	void OnUnderBar_GuildInit(intptr_t);
	void SendRoomListExitPacket(void);
	void OnUnderBarInit(intptr_t);
	void OnUnderTab_Init(intptr_t);
	void OnUnderBar_MagicBoxInit(intptr_t);
	void OnUnderBar_ScratchInit(intptr_t);
	void OnUnderBar_RankingInit(intptr_t);
	void OnUnderBar_NoticeInit(intptr_t);
	void OnOverBar_Bar1Init(int);
	void OnOverBar_Bar3Init(int);
	void OnOverBar_Bar2Init(int);
	void OnOverBar_OptionInit(intptr_t);
	void OnOverBar_MyInfoInit(intptr_t);
	void OnUnderBar_TabBtnInit(intptr_t);
	void OnUnderBar_BongdariShopInit(intptr_t);
	void OnTreasureAlarmInit(intptr_t);
	void SendRoomExitPacket(void);
	void OnOverBar_SiteInit(intptr_t);
	void OnMessengerBubbleEditInit(intptr_t);
	void OnMessengerGiftAlarmBtnInit(intptr_t);
	void UpdateSiteInfo(void);
	void OnOverBar_OptionUp(void);
	unsigned long GetCategoryFlag(int category);
	bool OnWaitDlgResult(int, FrForm *);
	void OnBarHoverOn(intptr_t);
	void OnOverBar_SpecialCardInit(intptr_t);
	void OnMessengerBubbleInit(intptr_t);
	void OnOverBar_CharOwnerDraw(int);
	void OnOverBar_AztecOwnerDraw(int);
	void OnOverBar_ClubOwnerDraw(int);
	void OnOverBar_Frame1Init(int);
	void OnOverBar_Frame2Init(int);
	void OnOverBar_Frame3Init(int);
	void OnOverBar_TabInit(int);
	void OnOverBar_OneLineReqBtnInit(intptr_t);
	// Message-map handlers named by CLobbyMain::_MsgEntries.
	void OnUnderBar_ShopInit(intptr_t);
	void OnUnderBar_MyRoomInit(intptr_t);
	void OnUnderBar_GiftInit(intptr_t);
	void OnUnderBar_GiftUp(void);
	void OnUnderBar_GameInit(intptr_t);
	void OnUnderBar_ServerInit(intptr_t);
	void OnUnderBar_ExitUp(void);
	void OnUnderBar_MagicBoxUp(void);
	void OnUnderBar_TikiMagicBoxUp(void);
	void OnUnderBar_ScratchUp(void);
	void OnUnderBar_BongdariShopUp(void);
	void OnUnderBar_MaketUp(void);
	void OnOverBar_CharInit(intptr_t);
	void OnOverBar_CaddieInit(intptr_t);
	void OnOverBar_CaddieOwnerDraw(int);
	void OnOverBar_ClubInit(intptr_t);
	void OnOverBar_AztecInit(intptr_t);
	void OnOverBar_MascotInit(intptr_t);
	void OnOverBar_MascotOwnerDraw(int);
	void OnOverBar_CharSelInit(intptr_t);
	void OnOverBar_CharSelOwnerDraw(int);
	void OnOverBar_CaddieSelInit(intptr_t);
	void OnOverBar_CaddieSelOwnerDraw(int);
	void OnOverBar_ClubSelInit(intptr_t);
	void OnOverBar_ClubSelOwnerDraw(int);
	void OnOverBar_AztecSelInit(intptr_t);
	void OnOverBar_AztecSelOwnerDraw(int);
	void OnOverBar_MascotSelInit(intptr_t);
	void OnOverBar_MascotSelOwnerDraw(int);
	void OnOverBar_PangCookieInit(intptr_t);
	void OnOverBar_PangCookieOwnerDraw(int);
	float Print(float x, float y, const char *text, float space, int align, float scale, unsigned long color);
	void OnOverBar_OneLineReqBtnUp(void);
	void OnOverBar_OneLineViewInit(intptr_t);
	void OnOverBar_MessengerInit(intptr_t);
	void OnMessengerBubbleOwnerDraw(int);
	void OnMessengerAlarmBtnInit(intptr_t);
	void OnUnderBar_ItemStorageInit(intptr_t);

	virtual bool OnResultSubEquipBar(int, FrForm *);
	virtual void OnUnderBar_ShopUp(void);
	virtual void OnUnderBar_MyRoomUp(void);
	virtual void OnUnderBar_CookieUp(void);
	virtual void OnUnderBar_GuildUp(void);

private:
	void NotifyServerControlMsg(int property);
	void _DrawSelection(FrViewer *viewer, WRect &rect, eBarButton button, FrGraphicInterface *gdi);

public:
	virtual void OnPreLoadInit(void);
	virtual void OnDestroy(void);

private:
	FrForm *m_rmrSettingDlg;    // 0x1C, cleared by OnRMR_ResultSetting

protected:
	FrButton *m_barButton[0x15]; // 0x20, indexed by eBarButton (10: the special card button)

	// 0x74: [0] the over bar's group box (OnOverBar_Init), [1] the under
	// tab (OnUnderTab_Init), [2] the under bar (OnUnderBarInit).
	FrWnd *m_barFrame[3];

private:
	FrButton *m_underBar;       // 0x80, cleared by InitUnderBar; blinks while the under tab is shown
	FrWnd *m_underTab;          // 0x84, cleared by InitUnderBar

protected:
	FrButton *m_oneLineReqBtn;  // 0x88, OnOverBar_OneLineReqBtnInit

private:
	FrWnd *m_oneLineView;       // 0x8C, OnOverBar_OneLineViewInit
	FrArea *m_overBarArea[3];   // 0x90, OnOverBar_Bar1Init..Bar3Init
	FrArea *m_overBarFrame[3];  // 0x9C, OnOverBar_Frame1Init..Frame3Init (moved left by 140 below 800 wide)

protected:
	FrArea *m_titleArea;        // 0xA8, the over bar's title (OnOverBar_TitleInit)

private:
	FrWnd *m_overBarTab;        // 0xAC, OnOverBar_TabInit

protected:
	FrViewer *m_charViewer;     // 0xB0, OnOverBar_CharInit
	FrViewer *m_caddieViewer;   // 0xB4, OnOverBar_CaddieInit
	FrViewer *m_aztecViewer;    // 0xB8, OnOverBar_AztecInit
	FrViewer *m_clubViewer;     // 0xBC, OnOverBar_ClubInit
	FrViewer *m_mascotViewer;   // 0xC0, OnOverBar_MascotInit, cleared by InitUnderBar

private:
	FrWnd *m_subSelectionWnd[5];        // 0xC4, hidden by CloseAllSubSelection
	FrStatic *m_siteWnd;                // 0xD8, OnOverBar_SiteInit; UpdateSiteInfo sets its caption
	FrWnd *m_pangCookieWnd;             // 0xDC, OnOverBar_PangCookieInit
	const Bitmap *m_treeCashBitmap;     // 0xE0, "pang_bar_treecash"
	const Bitmap *m_treeFrameBitmap;    // 0xE4, "pang_bar_frame_tree"
	const Bitmap *m_hanCoinBitmap;      // 0xE8, "pang_bar_hancoin"
	const Bitmap *m_hanFrameBitmap;     // 0xEC, "pang_bar_frame_han"

protected:
	bool m_overBarButtonDown;           // 0xF0, set while ProcessOverBar sees button 0 pressed; cleared by CLobbyMain's constructor
	bool m_subSelectionPending;         // 0xF1, ProcessOverBar sends the sub selection on release

private:
	PADDING(0xF2, 0xF4);

protected:
	FrWnd *m_messengerBubbleEdit;       // 0xF4, OnMessengerBubbleEditInit (CGolfRule's too)

private:
	FrArea *m_messengerBubble;          // 0xF8, OnMessengerBubbleInit
	const Bitmap *m_bubbleBitmap[3];    // 0xFC, "login_%02d"

protected:
	FrWnd *m_messengerWnd;              // 0x108, cleared by InitMessenger; CGolfRule::SetVisibleUpperBtns shows/hides it

	FrWnd *m_messengerAlarmBtn;         // 0x10C, OnMessengerAlarmBtnInit (CGolfRule's too)
	FrWnd *m_messengerGiftAlarmBtn;     // 0x110, OnMessengerGiftAlarmBtnInit

private:
	FrWnd *m_treasureAlarm;             // 0x114, OnTreasureAlarmInit
	PADDING(0x118, 0x120);

protected:
	bool m_bongdariShop[3];     // 0x120, cleared by InitBongdariShop

private:
	PADDING(0x123, 0x124);

protected:
	FrGiftDlg *m_giftDlg;       // 0x124, CShopMain::SendGift creates it
	FrOptionDlg *m_optionDlg;   // 0x128, OnOverBar_OptionUp; CShopMain::InitShopDialogs clears it
	FrForm *m_kbdManualDlg;     // 0x12C, CShopMain::InitShopDialogs clears it
	FrServerDlg *m_serverDlg;   // 0x130, the server list (CLobbyMain::OnUnderBar_ServerUp); OnProcess waits while it is open

private:
	FrForm *m_loginRsDlg;       // 0x134
	FrForm *m_recycleDlg;       // 0x138
	FrForm *m_tikiMagicBoxDlg;  // 0x13C, cleared by OnTikiMagicBoxDlgResult
	FrForm *m_scratchDlg;       // 0x140

protected:
	FrForm *m_onelineReqDlg;    // 0x144, opened with OnOnelineReqDlgResult; CLobbyMain::ResetControl clears it

protected:
	FrForm *m_waitDlg;          // 0x148, the "wait" form OnProcess opens
	FrNoticeDlg *m_noticeDlg;   // 0x14C, created by OnUnderBar_NoticeUp

	FrForm *m_popupDlg;         // 0x150, opened by a derived task; CloseAllDialogs closes it, CLobbyMain::ResetControl clears it

protected:
	FrForm *m_notifyOneDlg;     // 0x154, CShopMain::InitShopDialogs clears it

private:
	FrForm *m_quitDlg;          // 0x158
	FrSPCardBuffDlg *m_specialCardForm;  // 0x15C
	FrForm *m_maketListDlg;     // 0x160
	FrForm *m_completedMissionDlgs[5];  // 0x164
	std::vector<FrForm *> m_levelupForms;   // 0x178, CloseAllDialogs closes them, OnLevelupFormResult drops the last

protected:
	float m_waitDlgTime;        // 0x188, OnProcess opens m_waitDlg past 0.5s, -10 when idle

	FrButton *m_hoverButton;    // 0x18C, the hovered bar button (OnBarHoverOn sets it, OnBarHoverOff clears it)

private:
	bool m_showSubSelection;    // 0x190
	PADDING(0x191, 0x194);

protected:
	int m_subSelection;         // 0x194, CLobbyMain::InitControlsSelectedUserInfo clears it
	int m_subSelectionTab;      // 0x198

private:
	bool m_underBarEnabled;     // 0x19C, cleared by InitUnderBar
	PADDING(0x19D, 0x1A0);

protected:
	WTitleFont *m_titleFont;    // 0x1A0, released by OnDestroy/OnPreserveBack

	float m_uiOffsetX;          // 0x1A4, CalcUIOffset: centres the 800x600 UI
	float m_uiOffsetY;          // 0x1A8, CalcUIOffset: bottom-aligns it

private:
	float m_screenWidth;        // 0x1AC
	float m_screenHeight;       // 0x1B0
	PADDING(0x1B4, 0x1B8);      // members not transcribed yet
	std::string m_waitedLayout; // 0x1B8, posted to "Lobby" by OpenWaitedLayout
	bool m_itemStorageOpen;     // 0x1D4, cleared by OnUnderBar_ItemStorageInit
	PADDING(0x1D5, 0x1DC);

protected:
	FrSubEquipBar *m_subEquipBar;       // 0x1DC, OpenSubEquipBar creates it ("EQUIPBAR_SUB")
};
CHECK_SIZE(CTaskMain, 0x1E0);
BEGIN_LAYOUT_CHECK(CTaskMain)
	CHECK_FIELD(CTaskMain, m_rmrSettingDlg, 0x1C);
	CHECK_FIELD(CTaskMain, m_barButton, 0x20);
	CHECK_FIELD(CTaskMain, m_barFrame, 0x74);
	CHECK_FIELD(CTaskMain, m_underBar, 0x80);
	CHECK_FIELD(CTaskMain, m_underTab, 0x84);
	CHECK_FIELD(CTaskMain, m_oneLineView, 0x8C);
	CHECK_FIELD(CTaskMain, m_overBarArea, 0x90);
	CHECK_FIELD(CTaskMain, m_overBarFrame, 0x9C);
	CHECK_FIELD(CTaskMain, m_titleArea, 0xA8);
	CHECK_FIELD(CTaskMain, m_overBarTab, 0xAC);
	CHECK_FIELD(CTaskMain, m_oneLineReqBtn, 0x88);
	CHECK_FIELD(CTaskMain, m_charViewer, 0xB0);
	CHECK_FIELD(CTaskMain, m_caddieViewer, 0xB4);
	CHECK_FIELD(CTaskMain, m_aztecViewer, 0xB8);
	CHECK_FIELD(CTaskMain, m_clubViewer, 0xBC);
	CHECK_FIELD(CTaskMain, m_mascotViewer, 0xC0);
	CHECK_FIELD(CTaskMain, m_subSelectionWnd, 0xC4);
	CHECK_FIELD(CTaskMain, m_siteWnd, 0xD8);
	CHECK_FIELD(CTaskMain, m_pangCookieWnd, 0xDC);
	CHECK_FIELD(CTaskMain, m_treeCashBitmap, 0xE0);
	CHECK_FIELD(CTaskMain, m_treeFrameBitmap, 0xE4);
	CHECK_FIELD(CTaskMain, m_hanCoinBitmap, 0xE8);
	CHECK_FIELD(CTaskMain, m_hanFrameBitmap, 0xEC);
	CHECK_FIELD(CTaskMain, m_overBarButtonDown, 0xF0);
	CHECK_FIELD(CTaskMain, m_subSelectionPending, 0xF1);
	CHECK_FIELD(CTaskMain, m_messengerBubbleEdit, 0xF4);
	CHECK_FIELD(CTaskMain, m_messengerBubble, 0xF8);
	CHECK_FIELD(CTaskMain, m_bubbleBitmap, 0xFC);
	CHECK_FIELD(CTaskMain, m_messengerWnd, 0x108);
	CHECK_FIELD(CTaskMain, m_messengerAlarmBtn, 0x10C);
	CHECK_FIELD(CTaskMain, m_messengerGiftAlarmBtn, 0x110);
	CHECK_FIELD(CTaskMain, m_treasureAlarm, 0x114);
	CHECK_FIELD(CTaskMain, m_bongdariShop, 0x120);
	CHECK_FIELD(CTaskMain, m_giftDlg, 0x124);
	CHECK_FIELD(CTaskMain, m_optionDlg, 0x128);
	CHECK_FIELD(CTaskMain, m_kbdManualDlg, 0x12C);
	CHECK_FIELD(CTaskMain, m_serverDlg, 0x130);
	CHECK_FIELD(CTaskMain, m_loginRsDlg, 0x134);
	CHECK_FIELD(CTaskMain, m_recycleDlg, 0x138);
	CHECK_FIELD(CTaskMain, m_tikiMagicBoxDlg, 0x13C);
	CHECK_FIELD(CTaskMain, m_scratchDlg, 0x140);
	CHECK_FIELD(CTaskMain, m_onelineReqDlg, 0x144);
	CHECK_FIELD(CTaskMain, m_waitDlg, 0x148);
	CHECK_FIELD(CTaskMain, m_noticeDlg, 0x14C);
	CHECK_FIELD(CTaskMain, m_notifyOneDlg, 0x154);
	CHECK_FIELD(CTaskMain, m_popupDlg, 0x150);
	CHECK_FIELD(CTaskMain, m_quitDlg, 0x158);
	CHECK_FIELD(CTaskMain, m_specialCardForm, 0x15C);
	CHECK_FIELD(CTaskMain, m_maketListDlg, 0x160);
	CHECK_FIELD(CTaskMain, m_completedMissionDlgs, 0x164);
	CHECK_FIELD(CTaskMain, m_levelupForms, 0x178);
	CHECK_FIELD(CTaskMain, m_waitDlgTime, 0x188);
	CHECK_FIELD(CTaskMain, m_hoverButton, 0x18C);
	CHECK_FIELD(CTaskMain, m_showSubSelection, 0x190);
	CHECK_FIELD(CTaskMain, m_subSelection, 0x194);
	CHECK_FIELD(CTaskMain, m_subSelectionTab, 0x198);
	CHECK_FIELD(CTaskMain, m_underBarEnabled, 0x19C);
	CHECK_FIELD(CTaskMain, m_titleFont, 0x1A0);
	CHECK_FIELD(CTaskMain, m_uiOffsetX, 0x1A4);
	CHECK_FIELD(CTaskMain, m_uiOffsetY, 0x1A8);
	CHECK_FIELD(CTaskMain, m_screenWidth, 0x1AC);
	CHECK_FIELD(CTaskMain, m_screenHeight, 0x1B0);
	CHECK_FIELD(CTaskMain, m_waitedLayout, 0x1B8);
	CHECK_FIELD(CTaskMain, m_itemStorageOpen, 0x1D4);
	CHECK_FIELD(CTaskMain, m_subEquipBar, 0x1DC);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TASKMAIN_H
