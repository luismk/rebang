// Original: d:\build\custom\temp\code\client\projectg\golfrule.h
#ifndef PG_CLIENT_PROJECTG_GOLFRULE_H
#define PG_CLIENT_PROJECTG_GOLFRULE_H

#include "..\..\defines.h"
#include "TaskMain.h"
#include "GolfRuleBase.h"
#include "encrypttypes.h"
#include <deque>
#include <vector>

class FrForm;
class FrWnd;
class FrButton;
class FrEdit;
class FrComboBox;
class FrEmoticonDlg;
class WMatrix;
class WVector;
class CSimpleUI;
class FrApproachResultDlg;
class FrApproachEndDlg;

// The round's task (its constructor calls CTaskMain's). The constructor
// (golfrule.cpp:1708) and the factory's `new` (0x5C0) fix the size.
class CGolfRule : public CTaskMain {
public:
	CGolfRule(void);
	virtual ~CGolfRule(void);

	static const WRTTI m_RTTI;
	// golfrule.h:33
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; }

	// golfrule.h:617
	void SetFlagCutin(bool flagCutin);
	// golfrule.h:618
	bool GetFlagCutin(void);

	void SaveShot(const char *fileName);
	virtual void OnLoad(void);
	virtual void OnReload(void);
	virtual void OnInit(void);
	virtual void OnDestroy(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP *GetMessageMap(void) const;

	void OnChatTargetBtnDown(int);
	void OnMyInfoInit(int);
	void OnOptionInit(int);
	void OnUnplayableInit(int);
	void OnPauseInit(int);
	void OnChatRecordInit(int);
	void OnSpecialCardBtnUp(void);
	void OnCaddieBookBtnUp(void);
	void OnChatRecordBtnUp(void);
	void OnOnelineViewOwnerDraw(int);
	void SetGrade(void);
	bool CheckGiveUp(void);
	bool CheckDropOut(void);
	void SendShotResult(void);
	bool OnForgetDlgResult(int result, FrForm *form);
	unsigned long GetHolePang(unsigned char hole);
	void OnEmoticonInit(intptr_t);
	void OnReportInit(int);
	void OnEmoticonBtnUp(void);
	bool OnEmoticonResult(int result, FrForm *form);
	void OnOnelineViewInit(int);
	void OnChatInputInit(intptr_t);
	void OnCaddieBookInit(int);
	void Replay(bool);
	void ReturnLobby(bool);
	bool OnReturnLobbyResult(int, FrForm *);
	bool OnPlayerInfoDlgResult(int, FrForm *);
	void OnMessengerBtnUp(void);
	void OnMessenger_AlarmBtnUp(void);
	void ChangeGameMode(unsigned long mode);
	void OnRadioPredictNormalShotInit(int);
	void OnRadioPredictTomahwakShotInit(int);
	void OnRadioPredictCobraShotInit(int);
	void OnRadioPredictSpikeShotInit(int);
	void OnRadioPredictNormalShotDown(void);
	void OnRadioPredictTomahwakShotDown(void);
	void OnRadioPredictCobraShotDown(void);
	void OnRadioPredictSpikeShotDown(void);
	void SetReady(void);
	void AddPrize(int);
	void AddExtPrize(void);
	void GetWinningPrize(int);
	void OnMyInfoBtnUp(void);
	void OnOptionBtnUp(void);
	bool CancelDropDlgResult(int, FrForm *);
	void CheckNetMsgWait_Send(char *);
	void CheckNetMsgWait_Recv(char *);
	bool CanCancelShot(void);
	void SetPlayer(void);
	WMatrix BankCamera(float);
	void RecordTeamData(unsigned char team);
	void DoFixedCamera(float elapsed);
	void DoSpotCamera(float elapsed);
	void DoWaitForStart(float elapsed);
	bool CheckAllHoleOut(void);
	bool OnOnelineReqChkOutDlgResult(int result, FrForm *form);
	void PlayBGM(void);
	void OnUnplayableBtnUp(void);
	void OnPauseBtnUp(void);
	void DoNavigateCamera(float elapsed);
	bool OnReportResult(int result, FrForm *form);
	void SetAngle(float angle);
	void DoGetReadyToCaptureBG(float elapsed);
	void SetNetMsgWait(bool send, int type, char *msg, float timeout);
	void RecordScoreByTeam(unsigned char team);
	void RecordScoreByPlayer(unsigned char player);
	void CancelShot(void);
	bool OnChatTargetEnterKey(int);
	void ToggleCommandBtns(bool show);
	void SetVisibleUpperBtns(bool visible);
	void Warp(const WVector &pos);
	// SetEnvironmentVariable under windows.h's macro: the PDB name ends in A.
	void SetEnvironmentVariableA(void);
	bool SetTurnChangeEvent(void);
	void DoCloseupCaddie(float elapsed);
	void ResetUiPos_Board(void);
	bool IsMaxWithHGParts(unsigned char index);
	bool PauseDlgResult(int result, FrForm *form);
	void RecordPlayerData(unsigned char player);
	void SendHoleStat(void);
	void ChangeTurn(void);
	void ReadyForNextTurn(void);
	bool GoStopDlgResult(int result, FrForm *form);
	void OnMessenger_GiftAlarmBtnUp(void);
	void OnTopBarHoverOff(int);
	void OnRadioPredictNormalShotDraw(int);
	bool ReturnToLobbyDlgResult(int, FrForm *);
	void OnReportBtnUp(void);
	void OnChatTargetInit(intptr_t);
	void OnSpecialCardInit(int);
	void OnRadioPredictTomahwakShotDraw(int);
	void OnRadioPredictCobraShotDraw(int);
	void OnRadioPredictSpikeShotDraw(int);
	void OnMessenger_GiftAlarmBtnInit(int);
	void OnReplayStatusInit(int);
	bool OnApproachDlgResult(int, FrForm *);
	bool OnApproachEndDlgResult(int, FrForm *);
	void OnTopBarHoverOn(int);

	// The rule strategies drive the game mode (GolfRuleTimeattack::SetReady).
	friend class CGolfRuleTimeattack;
	friend class CGolfRuleQuickrace;
	friend class CGolfRuleTutorial;
	friend class CGolfRuleBase;

private:
	bool StandViewClick(void);
	void AllReplay(void);
	int HoleReplay2(void);
	bool IsLastShotRelplay(void);
	void InitRadioPredictShotButton(FrButton **button, int wnd, float offset);
	void SetRadioPredictShotPress(FrButton *button);
	bool OnRecordAviResult(int result, FrForm *form);
	void ReplayBoxClose(void);
	bool TopViewClick(void);
	void FlyBy(float elapsed);
	int ShotDataSave(const char *fileName, bool flag);
	void _ImeReset(void);
	void CollectBallStopData(void);
	void ToggleGameUI(void);
	bool OnReplayModeDlgResult(int result, FrForm *form);
	void UpdateRadioPredictShotButtonStatus(void);
	void ReplayBoxOpen(void);
	void SaveErrorFile(void);
	char *GetPangyaInstallDic(void);
	void QuickSave(const char *name);
	void OnRecordAviBtnUp(void);

	// Zeroed by the constructor when IsLocalContent(0x50); roles not known.
	unsigned long m_value1E0;   // 0x1E0
	unsigned long m_value1E4;   // 0x1E4
	unsigned long m_value1E8;   // 0x1E8
	PADDING(0x1EC, 0x204);
	float m_modeTime;           // 0x204, time in the current game mode (ChangeGameMode zeroes it)
	PADDING(0x208, 0x265);
	bool m_goStopSent;          // 0x265, set once GoStopDlgResult sent the answer
	PADDING(0x266, 0x27C);
	// A saved shot state (0x60 bytes, copied whole by the deque).
	struct sBackUp {
		UNKNOWN(0x0, 0x60);
	};
	std::deque<sBackUp> m_backUps;  // 0x27C, emptied by OnDestroy
	PADDING(0x290, 0x36C);
	WCrypticValue<float> m_crypticValue36C; // 0x36C, an encrypted value; role not known
	PADDING(0x37C, 0x3A8);
	std::vector<unsigned long> m_list3A8; // 0x3A8, a vector of plain values (the destructor frees it); element type not known
	PADDING(0x3B8, 0x3CB);
	bool m_netMsgWaitSend;      // 0x3CB, SetNetMsgWait: waiting on a sent (true) or received packet
	int m_netMsgWaitType;       // 0x3CC
	char *m_netMsgWait;         // 0x3D0, the packet name CheckNetMsgWait_Send/_Recv compare with
	char *m_netMsgWaitRecv;     // 0x3D4, the packet name of a receive wait
	unsigned long m_netMsgSendTime; // 0x3D8, GetTickCount of the send wait
	unsigned long m_netMsgRecvTime; // 0x3DC, GetTickCount of the receive wait
	float m_netMsgWaitTimeout;  // 0x3E0
	PADDING(0x3E4, 0x3F8);
	unsigned char m_bgmIndex;   // 0x3F8, the m_bgmList entry PlayBGM plays off the green
	PADDING(0x3F9, 0x408);
	bool m_shotReady;           // 0x408, cleared by CancelShot
	PADDING(0x409, 0x414);
	FrForm *m_pauseDlg;         // 0x414, cleared by PauseDlgResult
	FrForm *m_goStopDlg;        // 0x418, cleared by GoStopDlgResult
	FrForm *m_dropDlg;          // 0x41C, cleared by CancelDropDlgResult
	FrForm *m_playerInfoDlg;    // 0x420, cleared by OnPlayerInfoDlgResult
	FrApproachResultDlg *m_approachResultDlg; // 0x424, cleared by OnApproachDlgResult
	FrApproachEndDlg *m_approachEndDlg; // 0x428, opened by OnApproachDlgResult once the approach ends
	bool m_flag42C;             // 0x42C, cleared by OnInit
	PADDING(0x42D, 0x444);
	bool m_forgetPending;       // 0x444, the "flower of forgetting" dialog runs (DoFlyBall_Stopped)
	PADDING(0x445, 0x448);
	float m_forgetElapsed;      // 0x448, seconds the forget dialog has been open; closed at 10 (DoFlyBall_Stopped)
	int m_cancelShotItemSlot;   // 0x44C, the equip slot of the shot cancel item (CanCancelShot)
	PADDING(0x450, 0x460);
	std::vector<std::string> m_bgmList; // 0x460, the round's background musics (PlayBGM)
	FrComboBox *m_chatEdit;     // 0x470, the chat target box (OnChatTargetInit) OnChatTargetEnterKey resets
	FrWnd *m_chatInput;         // 0x474, focused by OnChatTargetBtnDown
	FrWnd *m_emoticonBtn;       // 0x478, set by OnEmoticonInit
	FrWnd *m_wnd47C;            // 0x47C, cleared by the constructor

	FrWnd *m_reportBtn;         // 0x480, set by OnReportInit
	FrWnd *m_chatViewWnd;       // 0x484, set by OnChatViewInit
	FrEmoticonDlg *m_emoticonDlg; // 0x488, opened and closed by OnEmoticonBtnUp
	FrForm *m_reportDlg;        // 0x48C, cleared by OnReportResult
	FrForm *m_forgetDlg;        // 0x490, the "flowerofforgetting" form, cleared by OnForgetDlgResult
	FrForm *m_form494;          // 0x494, cleared by the constructor

	FrForm *m_replayBox;        // 0x498, opened by ReplayBoxOpen, closed by ReplayBoxClose
	FrForm *m_recordAviDlg;     // 0x49C, cleared by OnRecordAviResult
	FrWnd *m_replayBoxWnd;      // 0x4A0, shown by ReplayBoxClose
	FrWnd *m_myInfoBtn;         // 0x4A4, set by OnMyInfoInit
	FrWnd *m_optionBtn;         // 0x4A8, set by OnOptionInit
	FrWnd *m_commandBtns[4];    // 0x4AC, shown or hidden together by ToggleCommandBtns
	FrWnd *m_onelineBoardWnd;   // 0x4BC, the one-line board's window (ResetUiPos_Board)
	FrWnd *m_boardWnd;          // 0x4C0, kept 140 pixels from the right edge (ResetUiPos_Board)
	bool m_flag4C4;             // 0x4C4, cleared by the constructor
	char m_text4C5[100];        // 0x4C5, emptied by the constructor (one memset of 100 bytes)
	PADDING(0x529, 0x538);
	FrButton *m_predictNormalShotBtn;   // 0x538, radio button of the normal shot prediction
	FrButton *m_predictTomahwakShotBtn; // 0x53C
	FrButton *m_predictCobraShotBtn;   // 0x540
	FrButton *m_predictSpikeShotBtn;   // 0x544
	CSimpleUI *m_predictShotUI;        // 0x548, the "Pangya" label OnRadioPredictNormalShotDraw draws by the normal shot button
	PADDING(0x54C, 0x558);
	bool m_showGameUI;          // 0x558, ToggleGameUI shows or hides the round's UI with it
	char m_saveFileName[0x40]; // 0x559, the free save/... or error/..._NNN.rep name SaveGame and SaveErrorFile pick
	PADDING(0x599, 0x59C);
	unsigned long m_value59C;   // 0x59C, zeroed by the constructor
	unsigned long m_value5A0;   // 0x5A0, zeroed by the constructor
	PADDING(0x5A4, 0x5A8);
	unsigned long m_replayFlags; // 0x5A8, AllReplay clears bit 0x4
	int m_replayMode;           // 0x5AC, 2 = last shot replay
	PADDING(0x5B0, 0x5B2);
	bool m_flagCutin;           // 0x5B2
	PADDING(0x5B3, 0x5B4);
	CGolfRuleBase* m_pRule;     // 0x5B4
	bool m_flag5B8;             // 0x5B8, cleared by OnInit
	bool m_waitingForStart;     // 0x5B9, set by DoWaitForStart once the fly-by has run
	PADDING(0x5BA, 0x5BC);
	unsigned long m_value5BC;   // 0x5BC, zeroed by the constructor

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CGolfRule, 0x5C0);
BEGIN_LAYOUT_CHECK(CGolfRule)
	CHECK_FIELD(CGolfRule, m_modeTime, 0x204);
	CHECK_FIELD(CGolfRule, m_goStopSent, 0x265);
	CHECK_FIELD(CGolfRule, m_backUps, 0x27C);
	CHECK_FIELD(CGolfRule, m_netMsgWaitSend, 0x3CB);
	CHECK_FIELD(CGolfRule, m_netMsgWaitType, 0x3CC);
	CHECK_FIELD(CGolfRule, m_netMsgWait, 0x3D0);
	CHECK_FIELD(CGolfRule, m_netMsgWaitRecv, 0x3D4);
	CHECK_FIELD(CGolfRule, m_netMsgSendTime, 0x3D8);
	CHECK_FIELD(CGolfRule, m_netMsgRecvTime, 0x3DC);
	CHECK_FIELD(CGolfRule, m_netMsgWaitTimeout, 0x3E0);
	CHECK_FIELD(CGolfRule, m_bgmIndex, 0x3F8);
	CHECK_FIELD(CGolfRule, m_shotReady, 0x408);
	CHECK_FIELD(CGolfRule, m_pauseDlg, 0x414);
	CHECK_FIELD(CGolfRule, m_goStopDlg, 0x418);
	CHECK_FIELD(CGolfRule, m_dropDlg, 0x41C);
	CHECK_FIELD(CGolfRule, m_playerInfoDlg, 0x420);
	CHECK_FIELD(CGolfRule, m_approachResultDlg, 0x424);
	CHECK_FIELD(CGolfRule, m_approachEndDlg, 0x428);
	CHECK_FIELD(CGolfRule, m_forgetPending, 0x444);
	CHECK_FIELD(CGolfRule, m_emoticonBtn, 0x478);
	CHECK_FIELD(CGolfRule, m_reportBtn, 0x480);
	CHECK_FIELD(CGolfRule, m_emoticonDlg, 0x488);
	CHECK_FIELD(CGolfRule, m_forgetElapsed, 0x448);
	CHECK_FIELD(CGolfRule, m_cancelShotItemSlot, 0x44C);
	CHECK_FIELD(CGolfRule, m_forgetDlg, 0x490);
	CHECK_FIELD(CGolfRule, m_bgmList, 0x460);
	CHECK_FIELD(CGolfRule, m_chatEdit, 0x470);
	CHECK_FIELD(CGolfRule, m_chatInput, 0x474);
	CHECK_FIELD(CGolfRule, m_chatViewWnd, 0x484);
	CHECK_FIELD(CGolfRule, m_reportDlg, 0x48C);
	CHECK_FIELD(CGolfRule, m_replayBox, 0x498);
	CHECK_FIELD(CGolfRule, m_recordAviDlg, 0x49C);
	CHECK_FIELD(CGolfRule, m_replayBoxWnd, 0x4A0);
	CHECK_FIELD(CGolfRule, m_myInfoBtn, 0x4A4);
	CHECK_FIELD(CGolfRule, m_optionBtn, 0x4A8);
	CHECK_FIELD(CGolfRule, m_commandBtns, 0x4AC);
	CHECK_FIELD(CGolfRule, m_onelineBoardWnd, 0x4BC);
	CHECK_FIELD(CGolfRule, m_boardWnd, 0x4C0);
	CHECK_FIELD(CGolfRule, m_predictNormalShotBtn, 0x538);
	CHECK_FIELD(CGolfRule, m_predictShotUI, 0x548);
	CHECK_FIELD(CGolfRule, m_predictTomahwakShotBtn, 0x53C);
	CHECK_FIELD(CGolfRule, m_predictCobraShotBtn, 0x540);
	CHECK_FIELD(CGolfRule, m_predictSpikeShotBtn, 0x544);
	CHECK_FIELD(CGolfRule, m_showGameUI, 0x558);
	CHECK_FIELD(CGolfRule, m_saveFileName, 0x559);
	CHECK_FIELD(CGolfRule, m_replayFlags, 0x5A8);
	CHECK_FIELD(CGolfRule, m_replayMode, 0x5AC);
	CHECK_FIELD(CGolfRule, m_flagCutin, 0x5B2);
	CHECK_FIELD(CGolfRule, m_pRule, 0x5B4);
	CHECK_FIELD(CGolfRule, m_waitingForStart, 0x5B9);
	CHECK_FIELD(CGolfRule, m_value1E0, 0x1E0);
	CHECK_FIELD(CGolfRule, m_value1E8, 0x1E8);
	CHECK_FIELD(CGolfRule, m_crypticValue36C, 0x36C);
	CHECK_FIELD(CGolfRule, m_list3A8, 0x3A8);
	CHECK_FIELD(CGolfRule, m_flag42C, 0x42C);
	CHECK_FIELD(CGolfRule, m_wnd47C, 0x47C);
	CHECK_FIELD(CGolfRule, m_form494, 0x494);
	CHECK_FIELD(CGolfRule, m_flag4C4, 0x4C4);
	CHECK_FIELD(CGolfRule, m_text4C5, 0x4C5);
	CHECK_FIELD(CGolfRule, m_value59C, 0x59C);
	CHECK_FIELD(CGolfRule, m_value5A0, 0x5A0);
	CHECK_FIELD(CGolfRule, m_flag5B8, 0x5B8);
	CHECK_FIELD(CGolfRule, m_value5BC, 0x5BC);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_GOLFRULE_H
