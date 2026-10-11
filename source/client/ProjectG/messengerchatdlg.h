// Original: d:\build\custom\temp\code\client\projectg\messengerchatdlg.h
#ifndef PG_CLIENT_PROJECTG_MESSENGERCHATDLG_H
#define PG_CLIENT_PROJECTG_MESSENGERCHATDLG_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;
class FrEmoticonDlg;
class FrStatic;
class WPoint;
class WRect;
struct sChatSlot;

// One-to-one messenger chat window: a chat view, an input edit, an emoticon
// button opening an FrEmoticonDlg, a caption with the buddy's name, and a
// "vibration" that shakes the window; its size (0x160) is the `new` of
// FrMessengerChatDlgMakeInstance.
class FrMessengerChatDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // messengerchatdlg.h:18

	FrMessengerChatDlg(void);
	virtual ~FrMessengerChatDlg(void);

	virtual bool Close(bool bDestroy);

	void Vibration(float time);
	void SaveTempChat(sChatSlot& slot);
	void RestoreChat(sChatSlot& slot);
	void SetUID(unsigned long uid);
	bool GetEmoticonClientRect(WRect& rect) const;
	void AddChatLine(std::string nick, std::string text);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);
	virtual bool OnLButtonDown(const WPoint& pt);

	void OnMessenger_ChatInputLBtnDown(void);
	void OnMessenger_ChatViewInit(intptr_t param);
	void OnMessenger_EmoticonInit(intptr_t param);
	void OnMessenger_CaptionInit(intptr_t param);
	void OnMessenger_ChatInputInit(intptr_t param);
	bool OnEmoticonResult(int result, FrForm* pForm);
	void OnMessenger_EmoticonLBtnDown(void);
	bool OnChatInputEnterKey(intptr_t param);
	void OnMessenger_CloseLBtnDown(void);

private:
	FrEmoticonDlg* m_pEmoticonDlg;   // 0x110 (opened by OnMessenger_EmoticonLBtnDown)
	int m_reserved114;               // 0x114 (only cleared by the constructor so far)
	FrButton* m_pEmoticonBtn;        // 0x118 (OnMessenger_EmoticonInit)
	FrEdit* m_pChatInput;            // 0x11c (OnMessenger_ChatInputInit)
	FrEdit* m_pChatView;             // 0x120 (OnMessenger_ChatViewInit)
	FrStatic* m_pCaption;            // 0x124 (OnMessenger_CaptionInit; the buddy's name, SetUID)
	unsigned long m_uid;             // 0x128 (SetUID)
	std::string m_nick;              // 0x12c (read by OnEmoticonResult, presumably the buddy's nick)
	float m_vibrationTime;           // 0x148 (OnProc)
	float m_vibrationElapsed;        // 0x14c (OnProc)
	bool m_bVibrating;               // 0x150 (OnProc)
	float m_shakeOffset;             // 0x154 (2.0, swapped with m_shakeOffsetNext by OnProc)
	float m_shakeOffsetNext;         // 0x158 (-2.0)
	UNKNOWN(0x15c, 0x160);
};

CHECK_SIZE(FrMessengerChatDlg, 0x160);
BEGIN_LAYOUT_CHECK(FrMessengerChatDlg)
	CHECK_FIELD(FrMessengerChatDlg, m_pEmoticonDlg, 0x110);
	CHECK_FIELD(FrMessengerChatDlg, m_reserved114, 0x114);
	CHECK_FIELD(FrMessengerChatDlg, m_pEmoticonBtn, 0x118);
	CHECK_FIELD(FrMessengerChatDlg, m_pChatInput, 0x11c);
	CHECK_FIELD(FrMessengerChatDlg, m_pChatView, 0x120);
	CHECK_FIELD(FrMessengerChatDlg, m_pCaption, 0x124);
	CHECK_FIELD(FrMessengerChatDlg, m_uid, 0x128);
	CHECK_FIELD(FrMessengerChatDlg, m_nick, 0x12c);
	CHECK_FIELD(FrMessengerChatDlg, m_vibrationTime, 0x148);
	CHECK_FIELD(FrMessengerChatDlg, m_vibrationElapsed, 0x14c);
	CHECK_FIELD(FrMessengerChatDlg, m_bVibrating, 0x150);
	CHECK_FIELD(FrMessengerChatDlg, m_shakeOffset, 0x154);
	CHECK_FIELD(FrMessengerChatDlg, m_shakeOffsetNext, 0x158);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MESSENGERCHATDLG_H
