// Original: d:\build\custom\temp\code\client\projectg\lobbymain.h
#ifndef PG_CLIENT_PROJECTG_LOBBYMAIN_H
#define PG_CLIENT_PROJECTG_LOBBYMAIN_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskMain.h"
#include "../shared/globalgamedefine.h"
#include "RTTI.h"
#include "frcmdtarget.h"
#include "../client/Wangreal/include/wtypes.h"
#include <vector>
#include <list>

class FrCreateNickDlg;
class FrLoginDlg;
class FrEquipDlg;
class FrUserListDlg;
class FrParanNickDlg;
class FrMakeRoomDlg;
class FrWorldTourEventDlg;
class FrArea;
class FrButton;
class FrListBox;
class FrNoticeDlg;
class FrComboCtlEx;
class FrComboBox;
class FrViewer;
class FrEdit;
class FrGaugeBar;
class FrStatic;
class FrFrame;
class FrGuildNoticeDlg;
class FrScoreDlg;
class FrGaugeBarImage;
class NewsReader;
class Bitmap;
struct sSlotInfo;

// An entry of the guild match ranking (CSharedDoc 0x4528, a vector of 9-byte
// packed entries; OnGameRoomExtRes_RoomGuildRankInit adds one list item
// each). The original name and declaring header are not known.
#pragma pack(push, 1)
struct sGuildRankEntry {
	PADDING(0x0, 0x1);
	unsigned long m_uid;                // 0x1, the guild player shown by FrScoreDlg::SetGuildPlayer
	PADDING(0x5, 0x9);                  // dword 0x5: the second player UID compared in the owner draw
};
#pragma pack(pop)
CHECK_SIZE(sGuildRankEntry, 0x9);
CHECK_FIELD(sGuildRankEntry, m_uid, 0x1);

// The body of client packet 0x08 (character creation, sent to the login
// server by OnCreate_CreateBtnUp). The original name is not known.
#pragma pack(push, 1)
struct sCreateCharacter {
	unsigned long m_typeId;             // 0x0, the character's item type id (0x4000000 | index)
	unsigned char m_colors;             // 0x4, hair color index (low nibble) | shirts color index (high nibble)
};
#pragma pack(pop)
CHECK_SIZE(sCreateCharacter, 0x5);
CHECK_FIELD(sCreateCharacter, m_colors, 0x4);

// The lobby task: server list, room list, game room and the lobby's
// dialogs. Its size (0x954) is the `new` of CLobbyMainMakeInstance. Only
// the members the transcribed handlers touch are named; the dialog
// pointers are cleared by the result callbacks of their dialogs.
class CLobbyMain : public CTaskMain {
	LAYOUT_CHECK_FRIEND;
public:
	// Room list order (RoomList_SortRoomList) and filter mode; the category
	// tabs pass ROOMSORT_CATEGORY with their category.
	enum roomSort_t {
		ROOMSORT_NO = 0,
		ROOMSORT_CATEGORY = 1,
		ROOMSORT_TITLE = 2,
		ROOMSORT_GAMETYPE = 3,
		ROOMSORT_HOLE = 4,
		ROOMSORT_COURSE = 5,
		ROOMSORT_STATE = 6
	};

	// User list order (RoomList_SortUserList); the other values are not
	// transcribed.
	enum userSort_t {
		USERSORT_DEFAULT = 3
	};

	// Room list category, the int of RoomList_FilterRoomList (VS, Mass and
	// Battle also index m_cateNewRoom).
	enum {
		ROOMCATE_VS = 0,
		ROOMCATE_MASS = 1,
		ROOMCATE_BATTLE = 2,
		ROOMCATE_CHAT = 3,
		ROOMCATE_ALL = 4
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // lobbymain.h:29

	CLobbyMain(void);
	virtual ~CLobbyMain(void);

	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnDestroy(void);
	virtual void OnDisplay(void);
	virtual void OnInit(void);
	virtual void OnProcess(float);
	virtual void HandleMsg(const MsgObject& msg);

private:
	void CheckTikiReport(const char* layout);

	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;
	virtual void CloseAllDialogs(void);
	// Message-map handlers (_MsgEntries) not transcribed yet.
	void ProcessLobbyBgCamera(float);
	void ProcessLobbyPetSync(float);
	void ProcessAdministrator(float);
	void ProcessDialogKeyboard(float);
	void ProcessLobbyUI(float);
	void ProcessAutoStart(float);
	void ResetControlsRoomListSort(float);
	void AnimateWebEventButton(float);
	void RefreshAllUccClothes(void);
	void SendChangedUserInfo(void);
	void _RefreshUccClothes(sSlotInfo* slot, unsigned long typeId, const char* uccKey);
	void OnLogin_Finish(void);
	void OnToppage_Finish(void);
	void OnToppage_FamilyUp(void);
	void OnToppage_RentalExpiredAlramInit(intptr_t);
	void OnToppage_RentalExpiredAlramUp(void);
	void OnToppage_Event1Up(void);
	void OnToppage_NewOwnerDraw(int);
	void OnToppage_PointEventInit(intptr_t);
	void OnToppage_WorldTourEventInit(intptr_t);
	void OnToppage_WorldTourEventDown(void);
	void OnToppage_FindEbortEventInit(intptr_t);
	void OnToppage_FindEbortEventDown(void);
	void OnToppage_Halloween2007EvenDown(void);
	void OnToppage_Christmas2007EventDown(void);
	void OnToppage_TicketExchangeInit(intptr_t);
	void OnToppage_TicketExchangeDown(void);
	void OnToppage_KoohBirthdayEventInit(intptr_t);
	void OnFirstConnectOwnerDraw(int);
	void OnRoomList_Init(int);
	void OnRoomList_RoomListInit(intptr_t);
	void OnRoomList_RoomListOwnerDraw(int);
	void OnRoomList_RoomListRUp(void);
	void OnRoomList_RoomListDClick(void);
	void OnRoomList_RoomSortNoDown(void);
	void OnRoomList_RoomSortNoOwnerDraw(int);
	void OnRoomList_RoomSortHoleDown(void);
	void OnRoomList_RoomSortHoleOwnerDraw(int);
	void OnRoomList_RoomSortCourseDown(void);
	void OnRoomList_RoomSortCourseOwnerDraw(int);
	void OnRoomList_RoomSortStateDown(void);
	void OnRoomList_RoomSortStateOwnerDraw(int);
	void OnRoomList_RoomSortCateVS_ListLUp(void);
	void OnRoomList_RoomSortCateVS_ListOwnerDraw(int);
	void OnRoomList_RoomSortCateMass_ListLUp(void);
	void OnRoomList_RoomSortCateMass_ListOwnerDraw(int);
	void OnRoomList_RoomSortCateBattle_ListLUp(void);
	void OnRoomList_RoomSortCateBattle_ListOwnerDraw(int);
	void OnEmoticonBtnUp(void);
	bool OnChatInputEnterKey(intptr_t);
	void OnReportBtnUp(void);
	void OnRoomList_MakeRoomInit(intptr_t);
	void OnRoomList_MakeRoomUp(void);
	void OnRoomList_GuildInfoInit(intptr_t);
	void OnRoomList_GuildInfoLButtonUp(void);
	void OnRoomList_UserListInit(intptr_t);
	void OnRoomList_UserListRBtnUp(void);
	void OnRoomList_UserListOwnerDraw(int);
	void OnRoomList_UserSortGenderUp(void);
	void OnRoomList_UserSortGenderOwnerDraw(int);
	void OnRoomList_UserSortLevelUp(void);
	void OnRoomList_UserSortLevelOwnerDraw(int);
	void OnRoomList_UserSortGuildUp(void);
	void OnRoomList_UserSortGuildOwnerDraw(int);
	void OnRoomList_UserSortNickUp(void);
	void OnRoomList_UserSortNickOwnerDraw(int);
	void OnGameRoom_Finish(void);
	void OnGameRoom_TitleInit(intptr_t);
	void OnGameRoom_RoomUserInit(int);
	void OnGameRoom_RoomUserOwnerDraw(int);
	void OnGameRoom_MapInit(int);
	void OnGameRoom_MapBtnUp(void);
	void OnGameRoom_MapPrevBtnUp(void);
	void OnGameRoom_MapNextBtnUp(void);
	void OnGameRoom_MapHoleComboInit(int);
	void OnGameRoom_MapHoleComboDown(void);
	void OnGameRoom_MapGaugeInit(int);
	void OnGameRoom_StartBtnUp(void);
	void OnGameRoom_ReadyBtnUp(void);
	void OnGameRoom_ChangeTeamBtnUp(void);
	void OnGameRoom_UserListBtnUp(void);
	void OnGameRoom_RoomOptionBtnUp(void);
	void OnGameRoom_CharDown(void);
	void OnGameRoom_CharOwnerDraw(int);
	void OnGameRoom_CaddieDown(void);
	void OnGameRoom_CaddieOwnerDraw(int);
	void OnGameRoom_ClubDown(void);
	void OnGameRoom_ClubOwnerDraw(int);
	void OnGameRoom_AztecDown(void);
	void OnGameRoom_AztecOwnerDraw(int);
	void OnGameRoom_MascotDown(void);
	void OnGameRoom_MascotOwnerDraw(int);
	void OnGameRoom_EquipItemDown(void);
	void OnGameRoom_EquipItemOwnerDraw(int);
	void OnGameRoom_CharSelOwnerDraw(int);
	void OnGameRoom_CaddieSelOwnerDraw(int);
	void OnGameRoom_ClubSelOwnerDraw(int);
	void OnGameRoom_AztecSelOwnerDraw(int);
	void OnGameRoom_MascotSelOwnerDraw(int);
	void OnGameRoomExt_Finish(void);
	void OnGameRoom_EventOwnerDraw(int);
	void OnGameRoomExt_RoomUserInit(int);
	void OnGameRoomExt_RoomUserOwnerDraw(int);
	void OnGameRoomExt_TeamNumOwnerDraw(int);
	void OnGameRoomExt_GuildNumOwnerDraw(int);
	void OnGameRoomExt_GuildInviteLBtnUp(void);
	void OnGameRoomExtRes_Init(int);
	void OnGameRoomExtRes_RoomUserRankInit(int);
	void OnGameRoomExtRes_RoomUserRankOwnerDraw(int);
	void OnGameRoomExtRes_RoomUserRankRBtnUp(void);
	void OnGameRoomExtRes_RoomGuildRankInit(int);
	void OnGameRoomExtRes_RoomGuildRankOwnerDraw(int);
	void OnGameRoomExtRes_RoomGuildRankRBtnUp(void);
	void OnGameRoom_ScoreBtnUp(void);
	void OnGameRoomExt_UserInfoLBtnUp(void);
	void OnGameRoomExt_ResCloseBtnUp(void);
	void OnCreatePetInit(int);
	void OnCreate_HairPrevBtnUp(void);
	void OnCreate_HairNextBtnUp(void);
	void OnCreate_ShirtsPrevBtnUp(void);
	void OnCreate_ShirtsNextBtnUp(void);
	void OnCreate_CreateBtnUp(void);

	void OnToppage_Destroy(void);
	void OnToppage_GameUp(void);
	bool OnValEventDlgResult(int, FrForm*);
	void OnToppage_TreasureIslandLBDown(void);
	void OnServer_Init(int);
	bool OnNewRecordDlgResult(int, FrForm*);
	void OnBlank_Init(int);
	void OnSecond_Pwd_Init(int);
	void OnSecond_Pwd_Finish(void);
	bool OnEventDlgResult(int, FrForm* form);
	bool OnMailBoxDlgResult(int, FrForm*);
	bool OnScoreDlgResult(int, FrForm*);
	bool OnItemEquipResult(int, FrForm*);
	bool OnResultDlg_Report(int, FrForm*);
	bool OnUserListDlgResult(int, FrForm*);
	bool OnNotifyQuitDlgResult(int, FrForm*);
	bool OnCreateInfoResult(int, FrForm*);
	bool OnReportDlgResult(int, FrForm*);
	bool OnUIHackDlgResult(int, FrForm*);
	bool OnMapSelectDlgResult(int, FrForm*);
	bool OnHolesEventDlgResult(int, FrForm*);
	bool OnPointEventDlgResult(int, FrForm*);
	bool OnWorldTourEventDlgResult(int, FrForm*);
	bool OnFindEbortEventDlgResult(int, FrForm*);
	bool OnHalloweenEventDlgResult(int, FrForm*);
	bool OnChristmasEventDlgResult(int, FrForm*);
	bool OnChristmasSockEventDlgResult(int, FrForm*);
	bool OnTicketExchangeDlgResult(int, FrForm*);
	bool OnFirstTutorialDlgResult(int, FrForm*);
	bool OnQuickResultDlgResult(int, FrForm*);
	void OpenLayout(const char*);
	bool OnParanNickChangeResult(int, FrForm*);
	bool OnCaddieWarningDlgResult(int, FrForm*);
	bool OnDtRoomDlgResult(int ret, FrForm*);
	void OnRoomList_Finish(void);
	void OnRoomList_Destroy(void);
	void OnRoomList_RoomSortCateAll_BtnDown(void);
	void OnRoomList_RoomSortCateVS_BtnDown(void);
	void OnRoomList_RoomSortCateMass_BtnDown(void);
	void OnRoomList_RoomSortCateBattle_BtnDown(void);
	void OnRoomList_RoomSortCateChat_BtnDown(void);
	void RoomList_LoadRoomImages(void);
	void RoomList_FilterRoomList(roomSort_t sort, int cate);
	void RoomList_SortRoomList(roomSort_t sort, int);
	void RoomList_SortUserList(userSort_t sort);
	void MakeRoomUserList(void);
	void OpenCreateInfo(unsigned long id, const char* text);
	void RoomList_SetCateTabBtn(void);
	void OnChatWndBtnUp(void);
	void OnChatBgInit(intptr_t);
	void OnRoomList_RoomSortCateVS_ListInit(intptr_t);
	void OnRoomList_RoomSortCateMass_ListInit(intptr_t);
	void OnRoomList_RoomSortCateBattle_ListInit(intptr_t);
	void OnToppage_PcbangInit(intptr_t);
	void OnToppage_Event1Init(intptr_t);
	void OnToppage_ValEventInit(intptr_t);
	void OnToppage_Halloween2007Init(intptr_t);
	void OnToppage_Christmas2007EventInit(intptr_t);
	void OnToppage_MissionEventInit(intptr_t);
	void OnToppage_BingoEventInit(intptr_t);
	void OnToppage_TreasureIslandInit(intptr_t);
	void OnChatWndBtnInit(intptr_t);
	void OnLanguageInit(intptr_t);
	void OnChatTargetInit(intptr_t);
	void OnGameRoom_MapPrevInit(intptr_t);
	void OnGameRoom_MapNextInit(intptr_t);
	void OnCreate_WarningInit(intptr_t);
	void OnToppage_NewInit(intptr_t);
	void OnEmoticonInit(intptr_t);
	void OnCreate_HairInit(intptr_t);
	void OnCreate_ShirtsInit(intptr_t);
	void OnGameRoom_ReadyInit(intptr_t);
	void OnGameRoomExtRes_RoomUserRankTabInit(intptr_t);
	void OnRoomList_ChatViewInit(intptr_t);
	void OnToppage_GameInit(intptr_t);
	void OnToppage_FamilyInit(intptr_t);
	void OnToppage_PangysStudyInit(intptr_t);
	void OnToppage_NoticeInit(intptr_t);
	void OnToppage_ServerInit(intptr_t);
	void OnRoomList_RoomSortNoInit(intptr_t);
	void OnRoomList_RoomSortHoleInit(intptr_t);
	void OnRoomList_RoomSortCourseInit(intptr_t);
	void OnRoomList_RoomSortStateInit(intptr_t);
	void OnRoomList_UserSortGenderInit(intptr_t);
	void OnRoomList_UserSortLevelInit(intptr_t);
	void OnRoomList_UserSortGuildInit(intptr_t);
	void OnRoomList_UserSortNickInit(intptr_t);
	void OnRoomList_RoomSortCateAll_BtnInit(intptr_t);
	void OnRoomList_RoomSortCateVS_BtnInit(intptr_t);
	void OnRoomList_RoomSortCateMass_BtnInit(intptr_t);
	void OnRoomList_RoomSortCateBattle_BtnInit(intptr_t);
	void OnRoomList_RoomSortCateChat_BtnInit(intptr_t);
	void OnCreate_CharPrevInit(intptr_t);
	void OnCreate_CharNextInit(intptr_t);
	void OnCreate_HairPrevInit(intptr_t);
	void OnCreate_HairNextInit(intptr_t);
	void OnCreate_ShirtsPrevInit(intptr_t);
	void OnCreate_ShirtsNextInit(intptr_t);
	void OnGameRoom_ChangeTeamBtnInit(intptr_t);
	void OnChatInputInit(intptr_t);
	void OnGameRoom_BackGroundInit(intptr_t);
	void OnGameRoom_MapBgInit(intptr_t);
	void OnGameRoom_CharSelInit(intptr_t);
	void OnGameRoom_CaddieSelInit(intptr_t);
	void OnGameRoom_ClubSelInit(intptr_t);
	void OnGameRoom_AztecSelInit(intptr_t);
	void OnGameRoom_MascotSelInit(intptr_t);
	void OnGameRoom_StartTipInit(intptr_t);
	void OnGameRoom_ReadyTipInit(intptr_t);
	void OnGameRoom_StartInit(intptr_t);
	void OnGameRoom_RoomOptionInit(intptr_t);
	void OnGameRoom_MapLockInit(intptr_t);
	void OnGameRoom_PowerBarInit(intptr_t);
	void OnGameRoom_ControlBarInit(intptr_t);
	void OnGameRoom_AccuracyBarInit(intptr_t);
	void OnGameRoom_SpinBarInit(intptr_t);
	void OnGameRoom_CurveBarInit(intptr_t);
	void OnGameRoom_PowerEditInit(intptr_t);
	void OnGameRoom_ControlEditInit(intptr_t);
	void OnGameRoom_AccuracyEditInit(intptr_t);
	void OnGameRoom_SpinEditInit(intptr_t);
	void OnGameRoom_CurveEditInit(intptr_t);
	void OnGameRoom_ScoreInit(intptr_t);
	void OnGameRoomExt_CountdownInit(intptr_t);
	void OnGameRoomExt_FrameInit(intptr_t);
	void OnGameRoomExt_GuildTab1Init(intptr_t);
	void OnGameRoomExt_GuildTab2Init(intptr_t);
	void OnGameRoomExt_TeamTab1Init(intptr_t);
	void OnGameRoomExt_TeamTab2Init(intptr_t);
	void OnGameRoomExt_TeamNumInit(intptr_t);
	void OnGameRoomExt_GuildInviteInit(intptr_t);
	void OnGameRoomExt_UserInfoInit(intptr_t);
	void On_PowerBarInit(intptr_t);
	void On_ControlBarInit(intptr_t);
	void On_AccuracyBarInit(intptr_t);
	void On_SpinBarInit(intptr_t);
	void On_CurveBarInit(intptr_t);
	void OnGameRoomExt_GuildNumInit(intptr_t);
	void OnGameRoomExtRes_GuildScoreGaugeInit(intptr_t);
	void OnChannelRoomCloseBtnInit(intptr_t);
	void OnGameRoom_Pet0Init(intptr_t);
	void OnGameRoom_Pet1Init(intptr_t);
	void OnGameRoom_Pet2Init(intptr_t);
	void OnGameRoom_Pet3Init(intptr_t);
	void OnChatBgOwnerDraw(int);
	void OnToppage_ValEventDown(void);
	void ToggleChatBtn(bool);
	void OnChatTargetBtnDown(int);
	bool OnChatTargetEnterKey(int);
	void OnGameRoom_Init(int);
	void OnGameRoom_Destroy(void);
	void InitControlsSelectedUserInfo(void);
	void OnGameRoom_CharSelDown(void);
	void OnGameRoom_CaddieSelDown(void);
	void OnGameRoom_ClubSelDown(void);
	void OnGameRoom_AztecSelDown(void);
	void OnGameRoom_MascotSelDown(void);
	void OnGameRoom_UserListInit(int);
	void OnGameRoomExt_Init(int);
	void OnGameRoomExt_Destroy(void);
	void OnGameRoomExtRes_Destroy(void);
	void OnCreate_Init(int);
	void OnCreate_Finish(void);
	void OnCreate_Destroy(void);
	bool OnMissionEventResult(int, FrForm*);
	bool OnBingoEventResult(int, FrForm*);
	bool OnKoohBirthdayEventDlg(int, FrForm*);
	void JoinOtherServerRoom(void);
	void OpenNoteForm(void);
	void OpenEventPrizeForm(unsigned char);
	void OpenCaddieWarningForm(void);
	void EnableRoomEnterControls(bool);
	void OnToppage_TopIcon1Init(int);
	void OnToppage_TopIcon2Init(int);
	void OnToppage_TopIcon3Init(int);
	void OnToppage_TopIcon4Init(int);
	void OnToppage_TopIcon5Init(int);
	void OnToppage_TopIcon6Init(int);
	void OnToppage_TopIcon1Up(void);
	void OnToppage_TopIcon2Up(void);
	void OnToppage_TopIcon3Up(void);
	void OnToppage_TopIcon4Up(void);
	void OnToppage_TopIcon5Up(void);
	void OnToppage_TopIcon6Up(void);
	void OnToppage_ServerUp(void);
	void OnToppage_PangysStudyUp(void);
	void OpenNewRecordForm(void);
	void OnGameRoomExtRes_Finish(void);
	void OnToppage_UCCShopBtnUp(void);
	void OnToppage_CardShopBtnUp(void);
	float GetTopPageBtnRect(void);
	bool OnRemoveFriendDlgResult(int ret, FrForm*);
	bool OnLoginDlgResult(int ret, FrForm*);
	void OnRoomList_RoomListLDown(void);
	void Banish(void);
	bool OnMakeRoomDlgResult(int ret, FrForm*);
	bool LatestVersion(void);
	void HidePet(unsigned long uid);
	void ShowPet(unsigned long uid);
	void RemakeTeam(bool);
	void OnToppage_Init(int);
	void OnToppage_NoticeUp(void);
	void OnToppage_PcbangUp(void);
	void OnLogin_Init(int);
	bool OnPCBangDlgResult(int, FrForm*);
	void OnRoomList_UserListLBtnUp(void);
	bool OnRoomExtResQuitDlgResult(int, FrForm*);
	void OpenGameExtResQuitForm(void);
	bool OnRoomUseReportDlgResult(int, FrForm*);
	void OpenUseReportForm(void);
	bool OnApproachGiftDlgResult(int, FrForm*);
	void OnBlank_Finish(void);
	void OnGameRoom_RoomUserLBtnUp(void);
	void OnGameRoom_RoomUserRBtnUp(void);
	void ReLoadHoleItem(void);
	void OpenPCBangNoticeForm(void);
	void GameExtResQuit(void);
	void OnGameRoom_CharInit(intptr_t);
	void OnGameRoom_CaddieInit(intptr_t);
	void OnGameRoom_ClubInit(intptr_t);
	void OnGameRoom_AztecInit(intptr_t);
	void OnGameRoom_MascotInit(intptr_t);
	void OnGameRoom_EquipItemInit(intptr_t);
	void OnCreate_CharInit(intptr_t);
	void OnCreate_DescInit(intptr_t);
	void OnToppage_KoohBirthdayEventDown(void);
	void OnServer_Finish(void);
	bool OnGuildNoticeDlgResult(int, FrForm*);
	bool OnVoteDlgResult(int, FrForm*);
	bool OnLevelupFormResult(int, FrForm*);
	void OnGameRoom_RoomUserDClick(void);
	void OpenParanNickChange(void);
	bool NeedsChangeNick(void);
	void OnToppage_MissionEventDown(void);
	void OnToppage_BingoEventDown(void);
	void OnGameRoomExtRes_RoomUserRankLBtnUp(void);
	void OnGameRoomExtRes_RoomGuildRankLBtnUp(void);
	void OnToppage_PointEventDown(void);
	void OnCreatePetOwnerDraw(void);
	void ResetControl(void) throw();

private:
	int m_topPageBtnIndex;              // 0x1E0, advanced by GetTopPageBtnRect
	FrButton* m_toppageExtraBtn;        // 0x1E4, a top page button cleared by the constructor (role not known)
	FrButton* m_toppageGameBtn;         // 0x1E8, OnToppage_GameInit
	FrButton* m_toppageFamilyBtn;       // 0x1EC, OnToppage_FamilyInit
	FrButton* m_toppagePangysStudyBtn;  // 0x1F0, OnToppage_PangysStudyInit
	FrButton* m_toppageMsgBtn;          // 0x1F4, a top page button HandleMsg uses (role not known)
	FrButton* m_toppageNoticeBtn;       // 0x1F8, OnToppage_NoticeInit; the notice dialog's icon rect
	FrButton* m_toppageServerBtn;       // 0x1FC, OnToppage_ServerInit
	FrButton* m_toppagePcbangBtn;       // 0x200, OnToppage_PcbangInit; the PC bang notice's icon rect
	FrButton* m_toppageEvent1Btn;       // 0x204, OnToppage_Event1Init
	FrButton* m_toppagePointEventBtn;   // 0x208, OnToppage_PointEventInit
	FrButton* m_toppageWorldTourBtn;    // 0x20C, OnToppage_WorldTourEventInit
	FrButton* m_toppageFindEbortBtn;    // 0x210, OnToppage_FindEbortEventInit
	FrButton* m_toppageHalloweenBtn;    // 0x214, OnToppage_Halloween2007Init
	FrButton* m_toppageChristmasBtn;    // 0x218, OnToppage_Christmas2007EventInit
	FrButton* m_toppageTicketExchangeBtn; // 0x21C, cleared with m_ticketExchangeDlg
	FrButton* m_toppageMissionBtn;      // 0x220, OnToppage_MissionEventInit
	FrButton* m_toppageBingoBtn;        // 0x224, OnToppage_BingoEventInit
	FrButton* m_toppageKoohBirthdayBtn; // 0x228, OnToppage_KoohBirthdayEventInit; cleared by OnKoohBirthdayEventDlg
	PADDING(0x22C, 0x230);
	FrButton* m_toppageValEventBtn;     // 0x230, OnToppage_ValEventInit
	FrButton* m_toppageTreasureBtn;     // 0x234, OnToppage_TreasureIslandInit
	FrButton* m_toppageChristmasSockBtn; // 0x238, cleared with m_christmasSockEventDlg
	FrArea* m_newArea;                  // 0x23C, OnToppage_NewInit
	const Bitmap* m_loginBitmaps[3];    // 0x240, "login_00".."login_02", loaded by OnInit
	const Bitmap* m_newTabBitmap;       // 0x24C, "tap_new2", loaded by OnInit
	float m_tipBlinkTime;               // 0x250, ProcessLobbyUI: time of the current blink of m_blinkTipArea
	int m_tipBlinkCount;                // 0x254, ProcessLobbyUI: blinks done (stops at 3)
	bool m_tipBlinking;                 // 0x258, SetStartBtn starts the tip blink
	PADDING(0x259, 0x25C);
	NewsReader* m_guildNewsReader;      // 0x25C, deleted when the guild notice dialog closes
	FrGuildNoticeDlg* m_guildNoticeDlg; // 0x260, OnGuildNoticeDlgResult
	bool m_caddieWarningPending;        // 0x264, OpenCaddieWarningForm shows one warning and clears it
	sCaddieInfo m_warningCaddies[12];   // 0x265, caddies to warn about (id 0: empty); OpenCaddieWarningForm takes the first
	PADDING(0x391, 0x394);
	float m_bgCameraDirection;          // 0x394, +1/-1: the side the lobby background camera pans to (ProcessLobbyBgCamera)
	bool m_bgCameraPaused;              // 0x398, the camera waits at the end of a pan
	PADDING(0x399, 0x39C);
	unsigned long m_bgCameraPauseTime;  // 0x39C, g_CurrentTime when the pause began
	unsigned long m_bgCameraMoveTime;   // 0x3A0, g_CurrentTime when the pan began
	bool m_scrollActive;                // 0x3A4, cleared by the constructor (role not known)
	float m_autoStartCountdown;         // 0x3A8, ProcessAutoStart: seconds left before the game room starts by itself (15, 6 in a guild room)
	bool m_autoStartArmed;              // 0x3AC, ProcessAutoStart runs the countdown while set (or m_roomOptionD); cleared at 1 second and by OnGameRoom_Init
	PADDING(0x3AD, 0x3B0);
	FrForm* m_pendingDlg;               // 0x3B0, cleared by the constructor
	bool m_useReportEnabled;            // 0x3B4, GameExtResQuit offers the use report form (cleared without local content 0x14)
	PADDING(0x3B5, 0x3B8);
	WRect m_event1IconRect;             // 0x3B8, the Event1 button's rect (OnToppage_Event1Init)
	float m_toppageBtnScale;            // 0x3C8, 1 at construction (with m_topPageBtnIndex)
	FrForm* m_secondPwdDlg;             // 0x3CC, cleared by the constructor
	PADDING(0x3D0, 0x3D4);
	FrLoginDlg* m_loginDlg;             // 0x3D4, OnLoginDlgResult
	FrForm* m_matchingDlg;              // 0x3D8, OnMatchingDlgResult, OnInviteDlgResult
	FrUserListDlg* m_userListDlg;       // 0x3DC, OnUserListDlgResult
	unsigned long m_equipItem[10];      // 0x3E0, the edited equipped items (OnItemEquipResult sends them)
	FrForm* m_dtRoomDlg;                // 0x408, OnDtRoomDlgResult
	FrScoreDlg* m_scoreDlg;             // 0x40C, OnScoreDlgResult
	FrForm* m_passwordDlg;              // 0x410, OnPasswordDlgResult
	FrNoticeDlg* m_pcbangNoticeDlg;     // 0x414, OnToppage_PcbangUp, OnPCBangDlgResult
	PADDING(0x418, 0x41C);
	FrForm* m_mapSelectDlg;             // 0x41C, OnMapSelectDlgResult
	FrForm* m_emoticonDlg;              // 0x420, OnEmoticonResult
	FrForm* m_reportDlg;                // 0x424, OnReportDlgResult
	FrForm* m_unusedDlg;                // 0x428, only cleared by ResetControl
	FrForm* m_mailBoxDlg;               // 0x42C, OnMailBoxDlgResult
	FrForm* m_eventDlg[6];              // 0x430, by FrEventPrizeDlg::GetEvent
	FrForm* m_newRecordDlg;             // 0x448, OnNewRecordDlgResult
	FrParanNickDlg* m_paranNickChangeDlg; // 0x44C, OnParanNickChangeResult
	FrForm* m_caddieWarningDlg;         // 0x450, OnCaddieWarningDlgResult
	FrForm* m_roomExtResQuitDlg;        // 0x454, OpenGameExtResQuitForm
	FrForm* m_roomUseReportDlg;         // 0x458, OpenUseReportForm
	FrEquipDlg* m_itemEquipDlg;            // 0x45C, OnItemEquipResult
	FrForm* m_holesEventDlg;            // 0x460, OnHolesEventDlgResult
	FrForm* m_pointEventDlg;            // 0x464, OnPointEventDlgResult
	FrMakeRoomDlg* m_makeRoomDlg;       // 0x468, OnMakeRoomDlgResult
	PADDING(0x46C, 0x470);
	FrWorldTourEventDlg* m_worldTourEventDlg; // 0x470, OnWorldTourEventDlgResult
	FrForm* m_findEbortEventDlg;        // 0x474, OnFindEbortEventDlgResult
	FrForm* m_halloweenEventDlg;        // 0x478, OnHalloweenEventDlgResult
	FrForm* m_halloweenEvent2Dlg;       // 0x47C, cleared with m_halloweenEventDlg
	FrForm* m_christmasEventDlg;        // 0x480, OnChristmasEventDlgResult
	FrForm* m_ticketExchangeDlg;        // 0x484, OnTicketExchangeDlgResult
	FrForm* m_extraEventDlg;            // 0x488, cleared by the constructor
	FrForm* m_quickResultDlg;           // 0x48C, OnQuickResultDlgResult
	FrForm* m_quickResultDlg2;          // 0x490, cleared after m_quickResultDlg
	FrForm* m_valEventDlg;              // 0x494, OnValEventDlgResult
	FrCreateNickDlg* m_createNickDlg;   // 0x498, OnCreate_Destroy
	FrForm* m_missionEventDlg;          // 0x49C, OnMissionEventResult
	FrForm* m_bingoEventDlg;            // 0x4A0, OnBingoEventResult
	PADDING(0x4A4, 0x4A8);
	FrForm* m_christmasSockEventDlg;    // 0x4A8, OnChristmasSockEventDlgResult
	FrListBox* m_roomListBox;           // 0x4AC, OnRoomList_RoomListLDown
	FrViewer* m_roomSortNoViewer;       // 0x4B0, OnRoomList_RoomSortNoInit
	PADDING(0x4B4, 0x4B8);
	FrButton* m_cateAllBtn;             // 0x4B8, room list category tabs (RoomList_SetCateTabBtn)
	FrButton* m_cateVSBtn;              // 0x4BC
	FrButton* m_cateMassBtn;            // 0x4C0
	FrButton* m_cateBattleBtn;          // 0x4C4
	FrButton* m_cateChatBtn;            // 0x4C8
	FrViewer* m_roomSortHoleViewer;     // 0x4CC, OnRoomList_RoomSortHoleInit
	FrViewer* m_roomSortCourseViewer;   // 0x4D0, OnRoomList_RoomSortCourseInit
	FrViewer* m_roomSortStateViewer;    // 0x4D4, OnRoomList_RoomSortStateInit
	FrButton* m_makeRoomBtn;            // 0x4D8, OnRoomList_MakeRoomInit
	FrButton* m_guildInfoBtn;           // 0x4DC, OnRoomList_GuildInfoInit
	FrListBox* m_userListBox;           // 0x4E0, the room list screen's user list (sBriefUserInfo records)
	FrViewer* m_userSortGenderViewer;   // 0x4E4, OnRoomList_UserSortGenderInit
	FrViewer* m_userSortLevelViewer;    // 0x4E8, OnRoomList_UserSortLevelInit
	FrViewer* m_userSortGuildViewer;    // 0x4EC, OnRoomList_UserSortGuildInit
	FrViewer* m_userSortNickViewer;     // 0x4F0, OnRoomList_UserSortNickInit
	FrListBox* m_cateVSList;            // 0x4F4, OnRoomList_RoomSortCateVS_ListInit (g_strGameType[0..1])
	FrListBox* m_cateMassList;          // 0x4F8, OnRoomList_RoomSortCateMass_ListInit (g_strGameType[2..4])
	FrListBox* m_cateBattleList;        // 0x4FC, OnRoomList_RoomSortCateBattle_ListInit (g_strGameType[5..6])
	const Bitmap* m_roomListBoxBmp[3][4]; // 0x500, RoomList_LoadRoomImages: roomlistbox_<n|o|d|e>_0<row+1>
	const Bitmap* m_roomSortNoBmp[3];   // 0x530, OnRoomList_RoomSortNoOwnerDraw: normal, hover, pressed
	const Bitmap* m_roomSortCateBmp[3]; // 0x53C, RoomList_LoadRoomImages: room_sort_cate_n_down, _o, _d
	const Bitmap* m_roomSortHoleBmp[3]; // 0x548, OnRoomList_RoomSortHoleOwnerDraw
	const Bitmap* m_roomSortCourseBmp[3]; // 0x554, OnRoomList_RoomSortCourseOwnerDraw
	const Bitmap* m_roomSortStateBmp[6]; // 0x560, OnRoomList_RoomSortStateOwnerDraw: [m_roomSortPending * 3 + state]
	const Bitmap* m_genderBmp[3][2];   // 0x578, RoomList_LoadRoomImages: i_male/i_female, _02, _03
	const Bitmap* m_genderMannerBmp[2]; // 0x590, i_male_manner, i_female_manner
	const Bitmap* m_genderAngelBmp[2];  // 0x598, i_male_angel, i_female_angel
	const Bitmap* m_userSortGenderBmp[3]; // 0x5A0, OnRoomList_UserSortGenderOwnerDraw: normal, hover, pressed
	const Bitmap* m_userSortLevelBmp[3]; // 0x5AC, OnRoomList_UserSortLevelOwnerDraw
	const Bitmap* m_userSortGuildBmp[3]; // 0x5B8, OnRoomList_UserSortGuildOwnerDraw
	const Bitmap* m_userSortNickBmp[3]; // 0x5C4, OnRoomList_UserSortNickOwnerDraw
	PADDING(0x5D0, 0x5DC);
	const Bitmap* m_roomImage;          // 0x5DC, RoomList_LoadRoomImages ("ingame"); drawn by the room and user list owner draws
	const Bitmap* m_userSortArrowBmp[2]; // 0x5E0, the user sort owner draws' order arrow: [m_userSortPending == 0]
	void* m_roomListImage;              // 0x5E8, OnRoomList_RoomListOwnerDraw
	const Bitmap* m_eventImage;               // 0x5EC, OnGameRoom_EventOwnerDraw
	roomSort_t m_roomSort;              // 0x5F0, the room list order HandleMsg re-sorts with
	userSort_t m_userSort;              // 0x5F4, the user list order HandleMsg re-sorts with; 3 at construction
	bool m_roomSortPending;             // 0x5F8, cleared by the constructor
	bool m_userSortPending;             // 0x5F9, cleared by the constructor
	bool m_roomSortDescending[4];       // 0x5FA, room list sort order per column (No, Hole, Course, State), cleared by the sort headers' Init
	bool m_userSortDescending[4];       // 0x5FE, user list sort order per column (Gender, Level, Guild, Nick)
	bool m_roomListDirty;               // 0x602, cleared by the constructor
	PADDING(0x603, 0x604);
	float m_roomSortResetTime;          // 0x604, seconds until ResetControlsRoomListSort clears m_roomSortDescending (<= 0: idle)
	float m_userSortResetTime;          // 0x608, likewise for m_userSortDescending
	float m_cateNewRoom[3];             // 0x60C, VS/Mass/Battle: how long the tab has been held over (ResetControlsRoomListSort opens its list past 0.8 s); cleared by the tabs
	bool m_chatWndOpen;                 // 0x618, true at construction
	PADDING(0x619, 0x620);
	FrArea* m_chatBg;                   // 0x620, OnChatBgInit, OnChatBgOwnerDraw
	FrButton* m_chatWndBtn;             // 0x624, OnChatWndBtnInit
	FrArea* m_languageArea;             // 0x628, OnLanguageInit (chat_eng / chat_kor)
	FrEdit* m_chatEdit;                 // 0x62C, the chat input (OnChatTargetBtnDown); ProcessDialogKeyboard colours it by the chat target
	FrEdit* m_chatView;                 // 0x630, OnRoomList_ChatViewInit
	FrComboBox* m_chatTargetCombo;      // 0x634, OnChatTargetInit
	FrButton* m_emoticonBtn;            // 0x638, OnEmoticonInit
	FrArea* m_petArea[4];               // 0x63C, OnGameRoom_Pet0Init..Pet3Init
	FrViewer* m_backGroundViewer;       // 0x64C, OnGameRoom_BackGroundInit
	FrViewer* m_mapBgViewer;            // 0x650, OnGameRoom_MapBgInit
	FrButton* m_mapBtn;                 // 0x654, OnGameRoom_MapInit
	FrWnd* m_mapExtraWnd;               // 0x658, only cleared by ResetControl
	FrWnd* m_roomControlWnd;            // 0x65C, SetRoomControls
	FrButton* m_mapPrevBtn;             // 0x660, OnGameRoom_MapPrevInit
	FrButton* m_mapNextBtn;             // 0x664, OnGameRoom_MapNextInit
	FrComboCtlEx* m_holeCombo;          // 0x668, the game room's hole selection (ReLoadHoleItem)
	FrGaugeBarImage* m_mapGauge;        // 0x66C, OnGameRoom_MapGaugeInit
	FrArea* m_mapLockArea;              // 0x670, OnGameRoom_MapLockInit
	FrViewer* m_charViewer;             // 0x674, OnGameRoom_CharInit
	FrViewer* m_caddieViewer;           // 0x678, OnGameRoom_CaddieInit
	FrViewer* m_aztecViewer;            // 0x67C, OnGameRoom_AztecInit
	FrViewer* m_clubViewer;             // 0x680, OnGameRoom_ClubInit
	FrViewer* m_mascotViewer;           // 0x684, OnGameRoom_MascotInit
	FrViewer* m_equipItemViewer;        // 0x688, OnGameRoom_EquipItemInit
	FrViewer* m_charSelViewer;          // 0x68C, OnGameRoom_CharSelInit
	FrViewer* m_caddieSelViewer;        // 0x690, OnGameRoom_CaddieSelInit
	FrViewer* m_aztecSelViewer;         // 0x694, OnGameRoom_AztecSelInit
	FrViewer* m_clubSelViewer;          // 0x698, OnGameRoom_ClubSelInit
	FrViewer* m_mascotSelViewer;        // 0x69C, OnGameRoom_MascotSelInit
	FrButton* m_startBtn;               // 0x6A0, OnGameRoom_StartInit
	FrButton* m_readyBtn;               // 0x6A4, OnGameRoom_ReadyInit
	PADDING(0x6A8, 0x6AC);
	FrGaugeBar* m_powerBar;             // 0x6AC, OnGameRoom_PowerBarInit / On_PowerBarInit
	FrGaugeBar* m_controlBar;           // 0x6B0
	FrGaugeBar* m_accuracyBar;          // 0x6B4
	FrGaugeBar* m_spinBar;              // 0x6B8
	FrGaugeBar* m_curveBar;             // 0x6BC
	FrStatic* m_powerStatic;            // 0x6C0, OnGameRoom_PowerEditInit
	FrStatic* m_controlStatic;          // 0x6C4
	FrStatic* m_accuracyStatic;         // 0x6C8
	FrStatic* m_spinStatic;             // 0x6CC
	FrStatic* m_curveStatic;            // 0x6D0
	FrButton* m_guildInviteBtn;         // 0x6D4, OnGameRoomExt_GuildInviteInit
	FrButton* m_changeTeamBtn;          // 0x6D8, OnGameRoom_ChangeTeamBtnInit
	FrWnd* m_roomSettingWnd;            // 0x6DC, SetRoomControls
	FrButton* m_scoreBtn;               // 0x6E0, OnGameRoom_ScoreInit
	FrArea* m_teamNumArea;              // 0x6E4, OnGameRoomExt_TeamNumInit
	FrArea* m_guildNumArea;             // 0x6E8, OnGameRoomExt_GuildNumInit
	FrWnd* m_extReservedWnd;            // 0x6EC, only cleared by ResetControl
	FrWnd* m_extReservedWnd2;           // 0x6F0, only cleared by ResetControl
	FrFrame* m_extFrame;                // 0x6F4, OnGameRoomExt_FrameInit
	FrWnd* m_charControlWnd;            // 0x6F8, SetRoomCharControl
	PADDING(0x6FC, 0x708);
	FrArea* m_teamTab1Area;             // 0x708, OnGameRoomExt_TeamTab1Init
	FrArea* m_teamTab2Area;             // 0x70C
	FrArea* m_guildTab1Area;            // 0x710, OnGameRoomExt_GuildTab1Init
	FrArea* m_guildTab2Area;            // 0x714
	FrGaugeBar* m_guildScoreGauge;      // 0x718, OnGameRoomExtRes_GuildScoreGaugeInit
	PADDING(0x71C, 0x720);
	FrArea* m_startTipArea;             // 0x720, OnGameRoom_StartTipInit
	FrArea* m_readyTipArea;             // 0x724, OnGameRoom_ReadyTipInit
	FrArea* m_blinkTipArea;             // 0x728, the start/ready tip ProcessLobbyUI blinks
	FrArea* m_countdownArea;            // 0x72C, OnGameRoomExt_CountdownInit
	FrArea* m_userRankTabArea;          // 0x730, OnGameRoomExtRes_RoomUserRankTabInit
	FrWnd* m_userActionBtn;             // 0x734, enabled once a user of m_userListBox is clicked
	FrButton* m_channelRoomCloseBtn;    // 0x738, OnChannelRoomCloseBtnInit
	FrEdit* m_roomTitleEdit;            // 0x73C, OnGameRoom_TitleInit
	FrButton* m_roomOptionBtn;          // 0x740, OnGameRoom_RoomOptionInit
	FrListBox* m_roomUserListBox;       // 0x744, OnGameRoom_RoomUserInit / OnGameRoomExt_RoomUserInit
	FrListBox* m_userRankListBox;       // 0x748, OnGameRoomExtRes_RoomUserRankInit (sGameResultInfo records)
	FrListBox* m_guildRankListBox;      // 0x74C, OnGameRoomExtRes_RoomGuildRankInit
	void* m_selectedRoomData;           // 0x750, list data of the room clicked in the room list
	FrForm* m_resultReportDlg;          // 0x754, OnResultDlg_Report
	FrForm* m_approachGiftDlg;          // 0x758, OnApproachGiftDlgResult
	FrForm* m_resultDlg;                // 0x75C, cleared with m_approachGiftDlg
	float m_approachGiftTimer;          // 0x760, ProcessLobbyUI: negative while the approach gift dialog runs, counts up to 0 (then resets to 1); OnInit sets 1 without the dialog
	std::vector<FrForm*> m_levelupDlgs; // 0x764, the last one is dropped by OnLevelupFormResult
	PADDING(0x774, 0x788);
	void* m_roomUserImage[2];           // 0x788, OnGameRoom_RoomUserOwnerDraw (role not known)
	PADDING(0x790, 0x7AC);
	void* m_roomUserMarkImage[2];       // 0x7AC, OnGameRoom_RoomUserOwnerDraw (role not known)
	PADDING(0x7B4, 0x7D8);
	void* m_extRoomUserImage[4];        // 0x7D8, OnGameRoomExt_RoomUserOwnerDraw (role not known)
	void* m_roomUserSelImage;           // 0x7E8, the room user owner draws (role not known)
	void* m_roomUserReserved[3];        // 0x7EC, only cleared by ResetControl
	void* m_roomUserReserved2;          // 0x7F8, only cleared by ResetControl
	void* m_roomUserReserved3;          // 0x7FC, only cleared by ResetControl
	char m_storedText[0x104];           // 0x800, a string stored by message 0x27 and handed out by message 0x28 (role not known); emptied by the constructor
	bool m_storedFlag;                  // 0x904, set by message 0x16, read by message 0x17 (role not known)
	bool m_roomOptionA;                 // 0x905, OnGameRoom_Init: set for identity 2 (role not known)
	bool m_roomOptionB;                 // 0x906, cleared by OnGameRoom_Init (role not known)
	bool m_roomOptionC;                 // 0x907, OnGameRoom_Init: set for identity 2 (role not known)
	bool m_roomOptionD;                 // 0x908, cleared by OnGameRoom_Init (role not known)
	PADDING(0x909, 0x90A);
	bool m_away;                        // 0x90A, the local player is away from keyboard (ProcessLobbyUI: 60 s idle in the game room); cleared by OnGameRoom_Init
	PADDING(0x90B, 0x90C);
	std::list<sSlotInfo*> m_slotList1;  // 0x90C, constructed by the constructor
	std::list<sSlotInfo*> m_slotList2;  // 0x918
	unsigned long m_roomMasterUid;      // 0x924, the game room master's UID (ProcessAutoStart: only the master starts), -1 at construction
	unsigned long m_selectedUserId;     // 0x928, second word of the user list record (-1: none), Banish
	unsigned long m_removeFriendUid;    // 0x92C, OnRemoveFriendDlgResult (-1: none)
	PADDING(0x930, 0x938);
	FrEdit* m_createCharEdit;           // 0x938, character creation screen (OnCreate_CharInit)
	FrEdit* m_createDescEdit;           // 0x93C, character creation screen (OnCreate_DescInit)
	unsigned char m_createCharType;     // 0x940, the character chosen on the creation screen (OnCreate_CreateBtnUp sends 0x4000000 | it)
	PADDING(0x941, 0x944);
	FrEdit* m_createHairEdit;           // 0x944, OnCreate_HairInit
	unsigned char m_createHairColor;    // 0x948, hair color index shown in m_createHairEdit
	PADDING(0x949, 0x94C);
	FrEdit* m_createShirtsEdit;         // 0x94C, OnCreate_ShirtsInit
	unsigned char m_createShirtsColor;  // 0x950, shirts color index shown in m_createShirtsEdit
	PADDING(0x951, 0x954);
};

// Course order of the map selection (inline in shareddoc.inl, SharedDoc.h).
unsigned char PrevMap(unsigned char map);
unsigned char NextMap(unsigned char map, bool);

CHECK_SIZE(CLobbyMain, 0x954);
BEGIN_LAYOUT_CHECK(CLobbyMain)
	CHECK_FIELD(CLobbyMain, m_topPageBtnIndex, 0x1E0);
	CHECK_FIELD(CLobbyMain, m_toppageKoohBirthdayBtn, 0x228);
	CHECK_FIELD(CLobbyMain, m_newArea, 0x23C);
	CHECK_FIELD(CLobbyMain, m_loginDlg, 0x3D4);
	CHECK_FIELD(CLobbyMain, m_userListDlg, 0x3DC);
	CHECK_FIELD(CLobbyMain, m_dtRoomDlg, 0x408);
	CHECK_FIELD(CLobbyMain, m_scoreDlg, 0x40C);
	CHECK_FIELD(CLobbyMain, m_guildNewsReader, 0x25C);
	CHECK_FIELD(CLobbyMain, m_guildNoticeDlg, 0x260);
	CHECK_FIELD(CLobbyMain, m_levelupDlgs, 0x764);
	CHECK_FIELD(CLobbyMain, m_mapSelectDlg, 0x41C);
	CHECK_FIELD(CLobbyMain, m_reportDlg, 0x424);
	CHECK_FIELD(CLobbyMain, m_mailBoxDlg, 0x42C);
	CHECK_FIELD(CLobbyMain, m_eventDlg, 0x430);
	CHECK_FIELD(CLobbyMain, m_newRecordDlg, 0x448);
	CHECK_FIELD(CLobbyMain, m_paranNickChangeDlg, 0x44C);
	CHECK_FIELD(CLobbyMain, m_caddieWarningDlg, 0x450);
	CHECK_FIELD(CLobbyMain, m_holesEventDlg, 0x460);
	CHECK_FIELD(CLobbyMain, m_pointEventDlg, 0x464);
	CHECK_FIELD(CLobbyMain, m_worldTourEventDlg, 0x470);
	CHECK_FIELD(CLobbyMain, m_findEbortEventDlg, 0x474);
	CHECK_FIELD(CLobbyMain, m_halloweenEventDlg, 0x478);
	CHECK_FIELD(CLobbyMain, m_christmasEventDlg, 0x480);
	CHECK_FIELD(CLobbyMain, m_ticketExchangeDlg, 0x484);
	CHECK_FIELD(CLobbyMain, m_quickResultDlg, 0x48C);
	CHECK_FIELD(CLobbyMain, m_valEventDlg, 0x494);
	CHECK_FIELD(CLobbyMain, m_createNickDlg, 0x498);
	CHECK_FIELD(CLobbyMain, m_missionEventDlg, 0x49C);
	CHECK_FIELD(CLobbyMain, m_bingoEventDlg, 0x4A0);
	CHECK_FIELD(CLobbyMain, m_christmasSockEventDlg, 0x4A8);
	CHECK_FIELD(CLobbyMain, m_cateNewRoom, 0x60C);
	CHECK_FIELD(CLobbyMain, m_chatBg, 0x620);
	CHECK_FIELD(CLobbyMain, m_chatEdit, 0x62C);
	CHECK_FIELD(CLobbyMain, m_chatWndBtn, 0x624);
	CHECK_FIELD(CLobbyMain, m_cateVSList, 0x4F4);
	CHECK_FIELD(CLobbyMain, m_cateMassList, 0x4F8);
	CHECK_FIELD(CLobbyMain, m_cateBattleList, 0x4FC);
	CHECK_FIELD(CLobbyMain, m_roomListBoxBmp, 0x500);
	CHECK_FIELD(CLobbyMain, m_roomSortNoBmp, 0x530);
	CHECK_FIELD(CLobbyMain, m_roomSortCateBmp, 0x53C);
	CHECK_FIELD(CLobbyMain, m_genderBmp, 0x578);
	CHECK_FIELD(CLobbyMain, m_genderMannerBmp, 0x590);
	CHECK_FIELD(CLobbyMain, m_genderAngelBmp, 0x598);
	CHECK_FIELD(CLobbyMain, m_roomSortHoleBmp, 0x548);
	CHECK_FIELD(CLobbyMain, m_roomSortCourseBmp, 0x554);
	CHECK_FIELD(CLobbyMain, m_roomSortStateBmp, 0x560);
	CHECK_FIELD(CLobbyMain, m_languageArea, 0x628);
	CHECK_FIELD(CLobbyMain, m_chatTargetCombo, 0x634);
	CHECK_FIELD(CLobbyMain, m_mapPrevBtn, 0x660);
	CHECK_FIELD(CLobbyMain, m_mapNextBtn, 0x664);
	CHECK_FIELD(CLobbyMain, m_toppageEvent1Btn, 0x204);
	CHECK_FIELD(CLobbyMain, m_toppageHalloweenBtn, 0x214);
	CHECK_FIELD(CLobbyMain, m_toppageChristmasBtn, 0x218);
	CHECK_FIELD(CLobbyMain, m_toppageMissionBtn, 0x220);
	CHECK_FIELD(CLobbyMain, m_toppageBingoBtn, 0x224);
	CHECK_FIELD(CLobbyMain, m_toppageValEventBtn, 0x230);
	CHECK_FIELD(CLobbyMain, m_toppageTreasureBtn, 0x234);
	CHECK_FIELD(CLobbyMain, m_event1IconRect, 0x3B8);
	CHECK_FIELD(CLobbyMain, m_emoticonBtn, 0x638);
	CHECK_FIELD(CLobbyMain, m_petArea, 0x63C);
	CHECK_FIELD(CLobbyMain, m_channelRoomCloseBtn, 0x738);
	CHECK_FIELD(CLobbyMain, m_resultReportDlg, 0x754);
	CHECK_FIELD(CLobbyMain, m_makeRoomDlg, 0x468);
	CHECK_FIELD(CLobbyMain, m_roomListBox, 0x4AC);
	CHECK_FIELD(CLobbyMain, m_userRankListBox, 0x748);
	CHECK_FIELD(CLobbyMain, m_guildRankListBox, 0x74C);
	CHECK_FIELD(CLobbyMain, m_selectedRoomData, 0x750);
	CHECK_FIELD(CLobbyMain, m_selectedUserId, 0x928);
	CHECK_FIELD(CLobbyMain, m_autoStartArmed, 0x3AC);
	CHECK_FIELD(CLobbyMain, m_roomOptionA, 0x905);
	CHECK_FIELD(CLobbyMain, m_roomOptionB, 0x906);
	CHECK_FIELD(CLobbyMain, m_roomOptionC, 0x907);
	CHECK_FIELD(CLobbyMain, m_roomOptionD, 0x908);
	CHECK_FIELD(CLobbyMain, m_away, 0x90A);
	CHECK_FIELD(CLobbyMain, m_removeFriendUid, 0x92C);
	CHECK_FIELD(CLobbyMain, m_toppageNoticeBtn, 0x1F8);
	CHECK_FIELD(CLobbyMain, m_toppagePcbangBtn, 0x200);
	CHECK_FIELD(CLobbyMain, m_pcbangNoticeDlg, 0x414);
	CHECK_FIELD(CLobbyMain, m_roomExtResQuitDlg, 0x454);
	CHECK_FIELD(CLobbyMain, m_userListBox, 0x4E0);
	CHECK_FIELD(CLobbyMain, m_userActionBtn, 0x734);
	CHECK_FIELD(CLobbyMain, m_roomUseReportDlg, 0x458);
	CHECK_FIELD(CLobbyMain, m_approachGiftDlg, 0x758);
	CHECK_FIELD(CLobbyMain, m_cateAllBtn, 0x4B8);
	CHECK_FIELD(CLobbyMain, m_cateVSBtn, 0x4BC);
	CHECK_FIELD(CLobbyMain, m_cateMassBtn, 0x4C0);
	CHECK_FIELD(CLobbyMain, m_cateBattleBtn, 0x4C4);
	CHECK_FIELD(CLobbyMain, m_cateChatBtn, 0x4C8);
	CHECK_FIELD(CLobbyMain, m_holeCombo, 0x668);
	CHECK_FIELD(CLobbyMain, m_createCharEdit, 0x938);
	CHECK_FIELD(CLobbyMain, m_createDescEdit, 0x93C);
	CHECK_FIELD(CLobbyMain, m_createCharType, 0x940);
	CHECK_FIELD(CLobbyMain, m_createHairEdit, 0x944);
	CHECK_FIELD(CLobbyMain, m_createHairColor, 0x948);
	CHECK_FIELD(CLobbyMain, m_createShirtsEdit, 0x94C);
	CHECK_FIELD(CLobbyMain, m_createShirtsColor, 0x950);
	CHECK_FIELD(CLobbyMain, m_useReportEnabled, 0x3B4);
	CHECK_FIELD(CLobbyMain, m_charViewer, 0x674);
	CHECK_FIELD(CLobbyMain, m_caddieViewer, 0x678);
	CHECK_FIELD(CLobbyMain, m_aztecViewer, 0x67C);
	CHECK_FIELD(CLobbyMain, m_clubViewer, 0x680);
	CHECK_FIELD(CLobbyMain, m_mascotViewer, 0x684);
	CHECK_FIELD(CLobbyMain, m_equipItemViewer, 0x688);
	CHECK_FIELD(CLobbyMain, m_backGroundViewer, 0x64C);
	CHECK_FIELD(CLobbyMain, m_mapLockArea, 0x670);
	CHECK_FIELD(CLobbyMain, m_charSelViewer, 0x68C);
	CHECK_FIELD(CLobbyMain, m_startBtn, 0x6A0);
	CHECK_FIELD(CLobbyMain, m_powerBar, 0x6AC);
	CHECK_FIELD(CLobbyMain, m_powerStatic, 0x6C0);
	CHECK_FIELD(CLobbyMain, m_guildInviteBtn, 0x6D4);
	CHECK_FIELD(CLobbyMain, m_scoreBtn, 0x6E0);
	CHECK_FIELD(CLobbyMain, m_extFrame, 0x6F4);
	CHECK_FIELD(CLobbyMain, m_teamTab1Area, 0x708);
	CHECK_FIELD(CLobbyMain, m_guildScoreGauge, 0x718);
	CHECK_FIELD(CLobbyMain, m_startTipArea, 0x720);
	CHECK_FIELD(CLobbyMain, m_countdownArea, 0x72C);
	CHECK_FIELD(CLobbyMain, m_roomOptionBtn, 0x740);
	CHECK_FIELD(CLobbyMain, m_toppageGameBtn, 0x1E8);
	CHECK_FIELD(CLobbyMain, m_toppagePangysStudyBtn, 0x1F0);
	CHECK_FIELD(CLobbyMain, m_toppageServerBtn, 0x1FC);
	CHECK_FIELD(CLobbyMain, m_roomSortNoViewer, 0x4B0);
	CHECK_FIELD(CLobbyMain, m_roomSortHoleViewer, 0x4CC);
	CHECK_FIELD(CLobbyMain, m_userSortGenderViewer, 0x4E4);
	CHECK_FIELD(CLobbyMain, m_roomSortDescending, 0x5FA);
	CHECK_FIELD(CLobbyMain, m_userSortDescending, 0x5FE);
	CHECK_FIELD(CLobbyMain, m_userSortGenderBmp, 0x5A0);
	CHECK_FIELD(CLobbyMain, m_userSortLevelBmp, 0x5AC);
	CHECK_FIELD(CLobbyMain, m_userSortGuildBmp, 0x5B8);
	CHECK_FIELD(CLobbyMain, m_userSortNickBmp, 0x5C4);
	CHECK_FIELD(CLobbyMain, m_userSortArrowBmp, 0x5E0);
	CHECK_FIELD(CLobbyMain, m_changeTeamBtn, 0x6D8);
	CHECK_FIELD(CLobbyMain, m_readyBtn, 0x6A4);
	CHECK_FIELD(CLobbyMain, m_userRankTabArea, 0x730);
	CHECK_FIELD(CLobbyMain, m_chatView, 0x630);
	CHECK_FIELD(CLobbyMain, m_toppageExtraBtn, 0x1E4);
	CHECK_FIELD(CLobbyMain, m_toppagePointEventBtn, 0x208);
	CHECK_FIELD(CLobbyMain, m_toppageWorldTourBtn, 0x20C);
	CHECK_FIELD(CLobbyMain, m_toppageFindEbortBtn, 0x210);
	CHECK_FIELD(CLobbyMain, m_toppageTicketExchangeBtn, 0x21C);
	CHECK_FIELD(CLobbyMain, m_toppageChristmasSockBtn, 0x238);
	CHECK_FIELD(CLobbyMain, m_loginBitmaps, 0x240);
	CHECK_FIELD(CLobbyMain, m_bgCameraDirection, 0x394);
	CHECK_FIELD(CLobbyMain, m_bgCameraPaused, 0x398);
	CHECK_FIELD(CLobbyMain, m_bgCameraPauseTime, 0x39C);
	CHECK_FIELD(CLobbyMain, m_bgCameraMoveTime, 0x3A0);
	CHECK_FIELD(CLobbyMain, m_approachGiftTimer, 0x760);
	CHECK_FIELD(CLobbyMain, m_newTabBitmap, 0x24C);
	CHECK_FIELD(CLobbyMain, m_caddieWarningPending, 0x264);
	CHECK_FIELD(CLobbyMain, m_warningCaddies, 0x265);
	CHECK_FIELD(CLobbyMain, m_scrollActive, 0x3A4);
	CHECK_FIELD(CLobbyMain, m_autoStartCountdown, 0x3A8);
	CHECK_FIELD(CLobbyMain, m_pendingDlg, 0x3B0);
	CHECK_FIELD(CLobbyMain, m_toppageBtnScale, 0x3C8);
	CHECK_FIELD(CLobbyMain, m_secondPwdDlg, 0x3CC);
	CHECK_FIELD(CLobbyMain, m_halloweenEvent2Dlg, 0x47C);
	CHECK_FIELD(CLobbyMain, m_extraEventDlg, 0x488);
	CHECK_FIELD(CLobbyMain, m_quickResultDlg2, 0x490);
	CHECK_FIELD(CLobbyMain, m_roomSort, 0x5F0);
	CHECK_FIELD(CLobbyMain, m_userSort, 0x5F4);
	CHECK_FIELD(CLobbyMain, m_roomSortPending, 0x5F8);
	CHECK_FIELD(CLobbyMain, m_userSortPending, 0x5F9);
	CHECK_FIELD(CLobbyMain, m_roomListDirty, 0x602);
	CHECK_FIELD(CLobbyMain, m_chatWndOpen, 0x618);
	CHECK_FIELD(CLobbyMain, m_resultDlg, 0x75C);
	CHECK_FIELD(CLobbyMain, m_storedText, 0x800);
	CHECK_FIELD(CLobbyMain, m_storedFlag, 0x904);
	CHECK_FIELD(CLobbyMain, m_slotList1, 0x90C);
	CHECK_FIELD(CLobbyMain, m_slotList2, 0x918);
	CHECK_FIELD(CLobbyMain, m_roomMasterUid, 0x924);
	CHECK_FIELD(CLobbyMain, m_toppageMsgBtn, 0x1F4);
	CHECK_FIELD(CLobbyMain, m_tipBlinkTime, 0x250);
	CHECK_FIELD(CLobbyMain, m_tipBlinkCount, 0x254);
	CHECK_FIELD(CLobbyMain, m_tipBlinking, 0x258);
	CHECK_FIELD(CLobbyMain, m_matchingDlg, 0x3D8);
	CHECK_FIELD(CLobbyMain, m_passwordDlg, 0x410);
	CHECK_FIELD(CLobbyMain, m_emoticonDlg, 0x420);
	CHECK_FIELD(CLobbyMain, m_unusedDlg, 0x428);
	CHECK_FIELD(CLobbyMain, m_equipItem, 0x3E0);
	CHECK_FIELD(CLobbyMain, m_itemEquipDlg, 0x45C);
	CHECK_FIELD(CLobbyMain, m_makeRoomBtn, 0x4D8);
	CHECK_FIELD(CLobbyMain, m_guildInfoBtn, 0x4DC);
	CHECK_FIELD(CLobbyMain, m_roomImage, 0x5DC);
	CHECK_FIELD(CLobbyMain, m_roomListImage, 0x5E8);
	CHECK_FIELD(CLobbyMain, m_eventImage, 0x5EC);
	CHECK_FIELD(CLobbyMain, m_roomSortResetTime, 0x604);
	CHECK_FIELD(CLobbyMain, m_userSortResetTime, 0x608);
	CHECK_FIELD(CLobbyMain, m_mapBtn, 0x654);
	CHECK_FIELD(CLobbyMain, m_mapExtraWnd, 0x658);
	CHECK_FIELD(CLobbyMain, m_roomControlWnd, 0x65C);
	CHECK_FIELD(CLobbyMain, m_mapGauge, 0x66C);
	CHECK_FIELD(CLobbyMain, m_roomSettingWnd, 0x6DC);
	CHECK_FIELD(CLobbyMain, m_extReservedWnd, 0x6EC);
	CHECK_FIELD(CLobbyMain, m_extReservedWnd2, 0x6F0);
	CHECK_FIELD(CLobbyMain, m_charControlWnd, 0x6F8);
	CHECK_FIELD(CLobbyMain, m_blinkTipArea, 0x728);
	CHECK_FIELD(CLobbyMain, m_roomTitleEdit, 0x73C);
	CHECK_FIELD(CLobbyMain, m_roomUserListBox, 0x744);
	CHECK_FIELD(CLobbyMain, m_roomUserImage, 0x788);
	CHECK_FIELD(CLobbyMain, m_roomUserMarkImage, 0x7AC);
	CHECK_FIELD(CLobbyMain, m_extRoomUserImage, 0x7D8);
	CHECK_FIELD(CLobbyMain, m_roomUserSelImage, 0x7E8);
	CHECK_FIELD(CLobbyMain, m_roomUserReserved, 0x7EC);
	CHECK_FIELD(CLobbyMain, m_roomUserReserved3, 0x7FC);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_LOBBYMAIN_H
