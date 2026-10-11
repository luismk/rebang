// Original: d:\build\custom\temp\code\client\projectg\replaymodedlg.h
#ifndef PG_CLIENT_PROJECTG_REPLAYMODEDLG_H
#define PG_CLIENT_PROJECTG_REPLAYMODEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;

// Replay confirmation form ("ok" / "cancel"). The mode set by its opener
// decides what OnOkBtnUp does (1: play the replay, 2: delete the replay
// file). Its size (0x120) is the `new` of FrReplayModeDlgMakeInstance.
class FrReplayModeDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // replaymodedlg.h:17

	FrReplayModeDlg(void);

	void SetReplayMode(int mode);
	int GetReplayMode(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnOkInit(intptr_t param);
	void OnOkBtnUp(void);
	void OnCancelInit(intptr_t param);
	void OnCancelBtnUp(void);

	FrButton* m_pOkBtn;       // 0x110
	FrButton* m_pCancelBtn;   // 0x114
	int m_replayMode;         // 0x118, 1 = play, 2 = delete
	PADDING(0x11c, 0x120);    // FrReplayModeDlg members
};

CHECK_SIZE(FrReplayModeDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrReplayModeDlg)
	CHECK_FIELD(FrReplayModeDlg, m_pOkBtn, 0x110);
	CHECK_FIELD(FrReplayModeDlg, m_pCancelBtn, 0x114);
	CHECK_FIELD(FrReplayModeDlg, m_replayMode, 0x118);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_REPLAYMODEDLG_H
