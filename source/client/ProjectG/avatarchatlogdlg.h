// Original: d:\build\custom\temp\code\client\projectg\avatarchatlogdlg.h
#ifndef PG_CLIENT_PROJECTG_AVATARCHATLOGDLG_H
#define PG_CLIENT_PROJECTG_AVATARCHATLOGDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;
class FrReportDlg;
class FrWnd;

// Chat log of the avatar lobby; its size (0x130) is the `new` of
// FrAvatarChatLogDlgMakeInstance.
class FrAvatarChatLogDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // avatarchatlogdlg.h:8

	FrAvatarChatLogDlg(void);
	virtual ~FrAvatarChatLogDlg(void);

	void AddChatMsg(const char* msg, int color);

private:
	static sFRESH_ENTRY _MsgEntries[];

	bool OnReportDlgResult(int ret, FrForm* pForm);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnBtnShowChatUp(void);
	void OnBtnHideChatUp(void);
	void OnBtnSaveChatUp(void);
	void OnBtnAccuseUp(void);
	void OnBtnCloseDlgUp(void);
	void OnHideChatInit(intptr_t param);
	void OnShowChatInit(intptr_t param);
	void OnChatViewInit(intptr_t param);

	// 0x110, 0x11c and 0x120 are zeroed by the constructor and read by no
	// function of this unit.
	FrWnd* m_pUnusedWnd1;         // 0x110
	FrButton* m_pShowChatBtn;     // 0x114, "showchat"
	FrButton* m_pHideChatBtn;     // 0x118, "hidechat"
	FrWnd* m_pUnusedWnd2;         // 0x11c
	FrWnd* m_pUnusedWnd3;         // 0x120
	FrEdit* m_pChatView;          // 0x124, "chatview"
	FrReportDlg* m_pReportDlg;    // 0x128, opened by OnBtnAccuseUp
	PADDING(0x12C, 0x130);
};

CHECK_SIZE(FrAvatarChatLogDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrAvatarChatLogDlg)
	CHECK_FIELD(FrAvatarChatLogDlg, m_pUnusedWnd1, 0x110);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pShowChatBtn, 0x114);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pHideChatBtn, 0x118);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pUnusedWnd2, 0x11c);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pUnusedWnd3, 0x120);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pChatView, 0x124);
	CHECK_FIELD(FrAvatarChatLogDlg, m_pReportDlg, 0x128);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AVATARCHATLOGDLG_H
