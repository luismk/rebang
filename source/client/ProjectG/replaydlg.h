// Original: d:\build\custom\temp\code\client\projectg\replaydlg.h
#ifndef PG_CLIENT_PROJECTG_REPLAYDLG_H
#define PG_CLIENT_PROJECTG_REPLAYDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "singleton.h"

class FrButton;

// Control bar of the replay player; its size (0x140) is the `new` of
// FrReplayControlDlgMakeInstance. Only the members this unit's written
// functions use are named.
class FrReplayControlDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // replaydlg.h:47

	FrReplayControlDlg(void);
	virtual ~FrReplayControlDlg(void);

	static void Init(void);        // replaydlg.cpp:44
	void TogglePlayButton(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	void OnProc(const float dt);

	void OnPlayInit(intptr_t param);
	void OnPlayBtnUp(void);        // replaydlg.cpp:104
	void OnBeforeShotBtnInit(int param);
	void OnBeforeShotBtnUp(void);
	void OnNextShotBtnInit(int param);
	void OnNextShotBtnUp(void);
	void OnExitBtnInit(intptr_t param);
	void OnExitBtnUp(void);
	void OnMinimizeBtnInit(int param);
	void OnMinimizeBtnUp(void);
	void OnRecordBtnInit(int param);
	void OnRecordBtnUp(void);

	FrButton* m_pBeforeShotBtn;    // 0x110
	FrButton* m_pNextShotBtn;      // 0x114
	FrButton* m_pExitBtn;          // 0x118
	FrButton* m_pMinimizeBtn;      // 0x11c
	FrButton* m_pRecordBtn;        // 0x120
	FrButton* m_pPlay;             // 0x124
	PADDING(0x128, 0x134);         // FrReplayControlDlg members
	bool m_bPlaying;               // 0x134, TogglePlayButton flips it
	float m_alpha;                 // 0x138, fades in while the cursor is over the bar
	PADDING(0x13c, 0x140);         // FrReplayControlDlg members
};

CHECK_SIZE(FrReplayControlDlg, 0x140);
BEGIN_LAYOUT_CHECK(FrReplayControlDlg)
	CHECK_FIELD(FrReplayControlDlg, m_pBeforeShotBtn, 0x110);
	CHECK_FIELD(FrReplayControlDlg, m_pNextShotBtn, 0x114);
	CHECK_FIELD(FrReplayControlDlg, m_pExitBtn, 0x118);
	CHECK_FIELD(FrReplayControlDlg, m_pMinimizeBtn, 0x11c);
	CHECK_FIELD(FrReplayControlDlg, m_pRecordBtn, 0x120);
	CHECK_FIELD(FrReplayControlDlg, m_pPlay, 0x124);
	CHECK_FIELD(FrReplayControlDlg, m_bPlaying, 0x134);
	CHECK_FIELD(FrReplayControlDlg, m_alpha, 0x138);
END_LAYOUT_CHECK

// Places the replay control bar on screen (the
// WSingleton<CReplayControlManager> instance). Members past BaseObject from
// the constructor and Init; the full size is not known.
class CReplayControlManager : public WSingleton<CReplayControlManager> {
	LAYOUT_CHECK_FRIEND;
public:
	CReplayControlManager(void);
	virtual ~CReplayControlManager(void);

	void Init(void);
	bool CalcReplayControlPos(void);

	int GetFadeType(void);         // replaydlg.h:109
	void SetFadeType(int);         // replaydlg.h:110
	WRect GetControlRect(void);    // replaydlg.h:112
	void SetControlRect(WRect rc); // replaydlg.h:113

protected:
	float m_fadeAlpha;             // 0x28
	float m_fadeTime;              // 0x2c
	int m_nFadeType;               // 0x30
	WRect m_rcControl;             // 0x34, screen rectangle of the control bar
};
BEGIN_LAYOUT_CHECK(CReplayControlManager)
	CHECK_FIELD(CReplayControlManager, m_fadeAlpha, 0x28);
	CHECK_FIELD(CReplayControlManager, m_fadeTime, 0x2c);
	CHECK_FIELD(CReplayControlManager, m_nFadeType, 0x30);
	CHECK_FIELD(CReplayControlManager, m_rcControl, 0x34);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_REPLAYDLG_H
