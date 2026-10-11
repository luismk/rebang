// Original: d:\build\custom\temp\code\client\projectg\onelinereqdlg.h
#ifndef PG_CLIENT_PROJECTG_ONELINEREQDLG_H
#define PG_CLIENT_PROJECTG_ONELINEREQDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "fredit.h"

class FrArea;
class FrButton;
class FrEmoticonDlg;

// One-line request dialog: a chat input with an emoticon picker and a
// language indicator. Size 0x120 (the `new` of FrOnelineReqDlgMakeInstance).
class FrOnelineReqDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // onelinereqdlg.h:8

	FrOnelineReqDlg(void);
	virtual ~FrOnelineReqDlg(void);

	// onelinereqdlg.h:13, emitted by taskmain.cpp
	const char* GetInputString(void);

	void SetLanguage(bool bEnglish);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnChatInputInit(intptr_t param);
	bool OnChatEnterKey(int param);
	void OnEmoticonInit(intptr_t param);
	void OnEmoticonBtnUp(void);
	bool OnEmoticonResult(int ret, FrForm* pForm);
	void OnLanguageInit(intptr_t param);
	void OnExplainInit(intptr_t param);
	void OnYesInit(intptr_t param);

private:
	FrEdit* m_pChatInput;           // 0x110, OnChatInputInit
	FrArea* m_pLanguage;            // 0x114, chat_eng / chat_kor background
	FrButton* m_pEmoticonBtn;       // 0x118
	FrEmoticonDlg* m_pEmoticonDlg;  // 0x11c, open emoticon picker
};

CHECK_SIZE(FrOnelineReqDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrOnelineReqDlg)
	CHECK_FIELD(FrOnelineReqDlg, m_pChatInput, 0x110);
	CHECK_FIELD(FrOnelineReqDlg, m_pLanguage, 0x114);
	CHECK_FIELD(FrOnelineReqDlg, m_pEmoticonBtn, 0x118);
	CHECK_FIELD(FrOnelineReqDlg, m_pEmoticonDlg, 0x11c);
END_LAYOUT_CHECK

// Check-out confirmation variant (size 0x110, no members of its own).
class FrOnelineReqChkOutDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // onelinereqdlg.h:40

	FrOnelineReqChkOutDlg(void);
	virtual ~FrOnelineReqChkOutDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnExplainInit(intptr_t param);
	void OnYesInit(intptr_t param);
};

CHECK_SIZE(FrOnelineReqChkOutDlg, 0x110);

#endif // PG_CLIENT_PROJECTG_ONELINEREQDLG_H
