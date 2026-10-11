// Original: d:\build\custom\temp\code\client\projectg\quitwindow.h
#ifndef PG_CLIENT_PROJECTG_QUITWINDOW_H
#define PG_CLIENT_PROJECTG_QUITWINDOW_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "frform.h"

class FrButton;
class FrOptionDlg;

// The quit / exit menu form; its size (0x128) is the `new` of
// FrQuitFormMakeInstance. Only the members this unit defines so far are
// declared.
class FrQuitForm : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // quitwindow.h:8

	FrQuitForm(void);

	// quitwindow.h:13
	WRect GetOptionBtnRect(void) { return m_pOptionBtn->GetRect(); }

	void EnableOptionBtn(bool enable);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // quitwindow.cpp:26

	virtual void OnDraw(void);
	virtual void OnProc(float dt);

	void OnExitBtnInit(intptr_t param);
	bool OnExitConfirmResult(int ret, FrForm* pForm);
	void OnOptionBtnInit(intptr_t param);
	void OnNextBtnInit(intptr_t param);
	void OnToLobbyBtnInit(intptr_t param);
	void OnNextBtnUp(void);
	void OnExitBtnUp(void);
	void OnOptionBtnUp(void);
	bool OnToLobbyConfirmResult(int ret, FrForm* pForm);
	void OnToLobbyBtnUp(void);
	void OnCloseBtnUp(void);

	FrWnd* m_pOptionBtn;        // 0x110
	FrWnd* m_pNextBtn;          // 0x114
	FrButton* m_pToLobbyBtn;    // 0x118
	FrWnd* m_pExitBtn;          // 0x11c
	int m_descState;            // 0x120, which description OnProc last showed
	float m_elapsed;            // 0x124, seconds since opening, capped at 2 (backdrop alpha / 64)
};

CHECK_SIZE(FrQuitForm, 0x128);
BEGIN_LAYOUT_CHECK(FrQuitForm)
	CHECK_FIELD(FrQuitForm, m_pOptionBtn, 0x110);
	CHECK_FIELD(FrQuitForm, m_pNextBtn, 0x114);
	CHECK_FIELD(FrQuitForm, m_pToLobbyBtn, 0x118);
	CHECK_FIELD(FrQuitForm, m_pExitBtn, 0x11c);
	CHECK_FIELD(FrQuitForm, m_descState, 0x120);
	CHECK_FIELD(FrQuitForm, m_elapsed, 0x124);
END_LAYOUT_CHECK

// Owner of the quit form and of the option window opened from it.
class CQuitWindow : public WSingleton<CQuitWindow>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
public:
	CQuitWindow(void);
	virtual ~CQuitWindow(void);

	// quitwindow.h:60
	bool HasOptionWindow(void) { return m_pOptionWindow ? true : false; }

	bool IsActive(void);
	void CreateOptionWindow(void);
	void Process(void);

protected:
	bool OnQuitFormResult(int ret, FrForm* pForm);
	bool OnOptionResult(int ret, FrForm* pForm);

	FrQuitForm* m_pQuitForm;    // 0x2c
	FrOptionDlg* m_pOptionWindow;   // 0x30
	bool m_cursorActive;        // 0x34, restored on the mouse cursor when the form closes
};

BEGIN_LAYOUT_CHECK(CQuitWindow)
	CHECK_FIELD(CQuitWindow, m_pQuitForm, 0x2c);
	CHECK_FIELD(CQuitWindow, m_pOptionWindow, 0x30);
	CHECK_FIELD(CQuitWindow, m_cursorActive, 0x34);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_QUITWINDOW_H
