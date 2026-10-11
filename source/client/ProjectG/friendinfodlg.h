// Original: d:\build\custom\temp\code\client\projectg\friendinfodlg.h
#ifndef PG_CLIENT_PROJECTG_FRIENDINFODLG_H
#define PG_CLIENT_PROJECTG_FRIENDINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;
struct sFriend;

// Friend information dialog: the friend's id, nickname, sex, server and
// channel, a whisper button and an ignore toggle. Its size (0x128) is the
// `new` of FrFriendInfoDlgMakeInstance.
class FrFriendInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrFriendInfoDlg(void) {                // friendinfodlg.h:13
		m_pID = 0;
		m_pNick = 0;
		m_pSex = 0;
		m_pServer = 0;
		m_pChannel = 0;
		m_pIgnore = 0;
	}

	void SetInfo(sFriend *info);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnIDInit(intptr_t param);
	void OnNickInit(intptr_t param);
	void OnSexInit(intptr_t param);
	void OnServerInit(intptr_t param);
	void OnChannelInit(intptr_t param);
	void OnWhisperBtnUp(void);
	void OnIgnoreInit(intptr_t param);
	void OnIgnoreBtnUp(void);
	void OnCloseBtnUp(void);

	virtual bool OnInit(void);

private:
	FrEdit* m_pID;                     // 0x110 (OnIDInit)
	FrEdit* m_pNick;                   // 0x114 (OnNickInit)
	FrEdit* m_pSex;                    // 0x118 (OnSexInit)
	FrEdit* m_pServer;                 // 0x11c (OnServerInit)
	FrEdit* m_pChannel;                // 0x120 (OnChannelInit)
	FrButton* m_pIgnore;               // 0x124 (OnIgnoreInit)
};
BEGIN_LAYOUT_CHECK(FrFriendInfoDlg)
	CHECK_FIELD(FrFriendInfoDlg, m_pID, 0x110);
	CHECK_FIELD(FrFriendInfoDlg, m_pNick, 0x114);
	CHECK_FIELD(FrFriendInfoDlg, m_pSex, 0x118);
	CHECK_FIELD(FrFriendInfoDlg, m_pServer, 0x11c);
	CHECK_FIELD(FrFriendInfoDlg, m_pChannel, 0x120);
	CHECK_FIELD(FrFriendInfoDlg, m_pIgnore, 0x124);
END_LAYOUT_CHECK
CHECK_SIZE(FrFriendInfoDlg, 0x128);

#endif // PG_CLIENT_PROJECTG_FRIENDINFODLG_H
