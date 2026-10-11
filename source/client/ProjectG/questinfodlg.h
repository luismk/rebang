// Original: d:\build\custom\temp\code\client\projectg\questinfodlg.h
#ifndef PG_CLIENT_PROJECTG_QUESTINFODLG_H
#define PG_CLIENT_PROJECTG_QUESTINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class WTitleFont;

// Quest information dialog; its size (0x130) is the `new` of
// FrQuestInfoDlgMakeInstance.
class FrQuestInfoDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // questinfodlg.h:22

	FrQuestInfoDlg(void);
	virtual ~FrQuestInfoDlg(void);

	void SetQuestExplane(int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnQuestInfoInit(int);
	void OnRewardItemListInit(intptr_t param);
	void OnRewardItemListOwnerDraw(int param);
	void OnQuestNameEditInit(intptr_t param);
	void OnQuestMessageEditInit(intptr_t param);
	void OnQuestExplaneEditInit(intptr_t param);

private:
	FrListBox* m_pRewardItemList;     // 0x110 ("reward_item")
	const Bitmap* m_pItemFrame;       // 0x114 ("re_itemframe_n")
	FrEdit* m_pQuestName;             // 0x118 ("name")
	FrEdit* m_pQuestMessage;          // 0x11c ("message")
	FrEdit* m_pQuestExplane;          // 0x120 ("explane")
	bool m_bRewardItemAdded;          // 0x124
	unsigned char m_questIndex;       // 0x125
	WTitleFont* m_pTitleFont;         // 0x128
	PADDING(0x12c, 0x130);
};

CHECK_SIZE(FrQuestInfoDlg, 0x130);
BEGIN_LAYOUT_CHECK(FrQuestInfoDlg)
	CHECK_FIELD(FrQuestInfoDlg, m_pRewardItemList, 0x110);
	CHECK_FIELD(FrQuestInfoDlg, m_pItemFrame, 0x114);
	CHECK_FIELD(FrQuestInfoDlg, m_pQuestName, 0x118);
	CHECK_FIELD(FrQuestInfoDlg, m_pQuestMessage, 0x11c);
	CHECK_FIELD(FrQuestInfoDlg, m_pQuestExplane, 0x120);
	CHECK_FIELD(FrQuestInfoDlg, m_bRewardItemAdded, 0x124);
	CHECK_FIELD(FrQuestInfoDlg, m_questIndex, 0x125);
	CHECK_FIELD(FrQuestInfoDlg, m_pTitleFont, 0x128);
END_LAYOUT_CHECK

// Quest reward (gift) selection dialog; its size (0x148) is the `new` of
// FrQuestGiftMakeInstance.
class FrQuestGift : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // questinfodlg.h:68

	FrQuestGift(void);
	virtual ~FrQuestGift(void);

	bool OnNotifyResult(int ret, FrForm* pForm);
	void SetSize(void);
	void SetQuestExplane(int index);
	void SetData(int index);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnGiftListInit(intptr_t param);
	void OnGiftListOwnerDraw(int param);
	void OnGiftListLBtnDown(void);
	void OnSelectInit(intptr_t param);
	void OnItemBack4(intptr_t param);
	void OnItemBack5(intptr_t param);
	void OnItemBack6(intptr_t param);
	void OnItemBack7(intptr_t param);
	void OnItemBack8(intptr_t param);
	void OnItemBack9(intptr_t param);
	void OnOkInit(intptr_t param);

private:
	FrForm* m_pNotifyForm;            // 0x110 ("notify", OnGiftListLBtnDown)
	FrListBox* m_pGiftList;           // 0x114 ("reward_item")
	FrArea* m_pSelect;                // 0x118 ("select")
	FrArea* m_pItemBack[6];           // 0x11c ("itemback4".."itemback9")
	FrButton* m_pOk;                  // 0x134 ("ok")
	WTitleFont* m_pTitleFont;         // 0x138
	unsigned char m_questIndex;       // 0x13c, set by SetQuestExplane
	const Bitmap* m_pItemFrame;       // 0x140 ("re_itemframe_n")
	bool m_bGiftAdded;                // 0x144
	PADDING(0x145, 0x148);
};

CHECK_SIZE(FrQuestGift, 0x148);
BEGIN_LAYOUT_CHECK(FrQuestGift)
	CHECK_FIELD(FrQuestGift, m_pNotifyForm, 0x110);
	CHECK_FIELD(FrQuestGift, m_pGiftList, 0x114);
	CHECK_FIELD(FrQuestGift, m_pSelect, 0x118);
	CHECK_FIELD(FrQuestGift, m_pItemBack, 0x11c);
	CHECK_FIELD(FrQuestGift, m_pOk, 0x134);
	CHECK_FIELD(FrQuestGift, m_pTitleFont, 0x138);
	CHECK_FIELD(FrQuestGift, m_questIndex, 0x13c);
	CHECK_FIELD(FrQuestGift, m_pItemFrame, 0x140);
	CHECK_FIELD(FrQuestGift, m_bGiftAdded, 0x144);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_QUESTINFODLG_H
