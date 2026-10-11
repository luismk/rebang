// Original: d:\build\custom\temp\code\client\projectg\post_senddlg.h
#ifndef PG_CLIENT_PROJECTG_POST_SENDDLG_H
#define PG_CLIENT_PROJECTG_POST_SENDDLG_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "../shared/globalgamedefine.h"
#include "../shared/globalnetworkdefine.h"

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrEmoticonDlg;
class FrFriendDlg;
class FrListBox;
class CPost_ItemCountDlg;
class CPost_MyItemList;
class CStandingInLine;
class FrGraphicInterface;
class WTitleFont;
struct FrListItem;

// Mail writing dialog; its size (0x1a8) is the `new` of
// CPost_SendDlgMakeInstance. Only the members the transcribed functions touch
// are named.
class CPost_SendDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
	// FrFriendDlg fills the receiver edit boxes of its parent dialog
	friend class FrFriendDlg;
	friend class CRealMyRoomMain;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_senddlg.h:28

	CPost_SendDlg(void);
	virtual ~CPost_SendDlg(void);

	void UpdateFriendList(void);          // post_senddlg.h:35
	unsigned long GetSendUID(void);       // post_senddlg.h:42

	void Reset(void);
	bool OnSendQuestionResult(int ret, FrForm* pForm);
	bool OnFriendListResult(int ret, FrForm* pForm);
	bool OnPostItemListResutl(int ret, FrForm* pForm);
	void SetReceiverInfo(unsigned long uid, const sPangYaUserInfo& info);
	int GetSendListSize(void);
	bool VerifySendItemList(void);
	bool VerifyIncludeSameItem(void);
	int GetSendServiceCharge(void);
	bool OnEmoticonResult(int ret, FrForm* pForm);
	std::string GetLetterMsg(void);
	std::list<sMailIncludeItem>* GetSendList(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_senddlg.cpp:49

	void OnPostReciveUserInit(intptr_t param);       // post_senddlg.cpp:164
	void OnPostReciveMsgInit(intptr_t param);        // post_senddlg.cpp:172
	void OnPostUserNickInit(intptr_t param);         // post_senddlg.cpp:187
	void OnPostSearchUserInit(intptr_t param);       // post_senddlg.cpp:191
	void OnPostFriendListInit(intptr_t param);       // post_senddlg.cpp:235
	void OnPostFriendListBtnUp(void);           // post_senddlg.cpp:240
	void OnPostItemAddInit(intptr_t param);          // post_senddlg.cpp:270
	void OnPostItemAddBtnUp(void);              // post_senddlg.cpp:274
	void OnPostListInit(intptr_t param);             // post_senddlg.cpp:306
	void OnPostServiceChargeInit(intptr_t param);    // post_senddlg.cpp:563
	void OnEmoticonBtnInit(intptr_t param);          // post_senddlg.cpp:573
	void OnEmoticonBtnUp(void);                 // post_senddlg.cpp:583
	bool OnPostReciveMsgEnterKey(int);    // post_senddlg.cpp:182
	bool OnPostReceiveUserEnterKey(int);  // post_senddlg.cpp:196
	void OnPostSearchUserBtnUp(void);
	void OnPostListOwnerDraw(intptr_t param);  // post_senddlg.cpp:315
	void DrawPartsSlot(FrGraphicInterface* pGDI, FrListItem* pItem);   // post_senddlg.cpp:432
	virtual void OnOK(void);              // post_senddlg.cpp:600
	virtual void OnCancel(void);          // post_senddlg.cpp:672

private:
	FrFriendDlg* m_pFriendDlg;            // 0x110
	FrButton* m_pFriendListBtn;           // 0x114
	FrButton* m_pEmoticonBtn;             // 0x118
	FrButton* m_pItemAddBtn;              // 0x11c
	FrEmoticonDlg* m_pEmoticonDlg;        // 0x120
	CPost_MyItemList* m_pMyItemList;      // 0x124, item picker opened by the item add button
	FrListBox* m_pPostList;               // 0x128
	FrEdit* m_pServiceChargeEdit;         // 0x12c
	std::string m_receiverNick;           // 0x130
	std::string m_receiverName;           // 0x14c, the name shown in the receiver edit
	unsigned long m_sendUID;              // 0x168
	const Bitmap* m_pUccStateBitmap[3];   // 0x16c: original, copied, temp saved
	WTitleFont* m_pTitleFont;             // 0x178
	FrEdit* m_pUserNickEdit;              // 0x17c
	FrEdit* m_pReciveUserEdit;            // 0x180
	FrEdit* m_pReciveMsgEdit;             // 0x184
	FrButton* m_pSearchUserBtn;           // 0x188
	bool m_bReceiverSet;                  // 0x18c
	std::list<sTradeItem> m_tradeList;    // 0x190
	std::list<sMailIncludeItem> m_sendList;   // 0x19c
};

CHECK_SIZE(CPost_SendDlg, 0x1a8);
BEGIN_LAYOUT_CHECK(CPost_SendDlg)
	CHECK_FIELD(CPost_SendDlg, m_pFriendDlg, 0x110);
	CHECK_FIELD(CPost_SendDlg, m_pFriendListBtn, 0x114);
	CHECK_FIELD(CPost_SendDlg, m_pEmoticonBtn, 0x118);
	CHECK_FIELD(CPost_SendDlg, m_pItemAddBtn, 0x11c);
	CHECK_FIELD(CPost_SendDlg, m_pEmoticonDlg, 0x120);
	CHECK_FIELD(CPost_SendDlg, m_pMyItemList, 0x124);
	CHECK_FIELD(CPost_SendDlg, m_pPostList, 0x128);
	CHECK_FIELD(CPost_SendDlg, m_pServiceChargeEdit, 0x12c);
	CHECK_FIELD(CPost_SendDlg, m_receiverNick, 0x130);
	CHECK_FIELD(CPost_SendDlg, m_receiverName, 0x14c);
	CHECK_FIELD(CPost_SendDlg, m_sendUID, 0x168);
	CHECK_FIELD(CPost_SendDlg, m_pUccStateBitmap, 0x16c);
	CHECK_FIELD(CPost_SendDlg, m_pTitleFont, 0x178);
	CHECK_FIELD(CPost_SendDlg, m_pUserNickEdit, 0x17c);
	CHECK_FIELD(CPost_SendDlg, m_pReciveUserEdit, 0x180);
	CHECK_FIELD(CPost_SendDlg, m_pReciveMsgEdit, 0x184);
	CHECK_FIELD(CPost_SendDlg, m_pSearchUserBtn, 0x188);
	CHECK_FIELD(CPost_SendDlg, m_bReceiverSet, 0x18c);
	CHECK_FIELD(CPost_SendDlg, m_tradeList, 0x190);
	CHECK_FIELD(CPost_SendDlg, m_sendList, 0x19c);
END_LAYOUT_CHECK

// Item list of the mail dialog; its size (0x178) is the `new` of
// CPost_MyItemListMakeInstance.
class CPost_MyItemList : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_senddlg.h:123

	// Nested helper struct; members not recovered yet.
	struct miniBtn_t {
		miniBtn_t(void);

		WRect m_rect;               // 0x00
		const Bitmap* m_pBitmapN;   // 0x10, normal state
		const Bitmap* m_pBitmapO;   // 0x14, cursor over
		UNKNOWN(0x18, 0x1c);
	};

	CPost_MyItemList(void);
	virtual ~CPost_MyItemList(void);

	void SetParent(CPost_SendDlg* pParent);

	void OnPost_ItemListMouseMove(void);    // post_senddlg.cpp:1519
	void OnPost_ItemListRBtnDown(void);     // post_senddlg.cpp:1523
	void OnPost_ItemListDClick(void);       // post_senddlg.cpp:1569
	void OnPost_SelectListMouseMove(void);  // post_senddlg.cpp:1701
	void OnPost_SelectListDClick(void);     // post_senddlg.cpp:1705
	void OnPost_ItemListOwnerDraw(intptr_t param);
	void OnPost_ItemListBtnDown(void);
	void OnPost_SelectListOwnerDraw(intptr_t param);
	void OnPost_SelectListBtnDown(void);
	void DrawPang(void);                    // post_senddlg.cpp:1841
	void DrawItem(WPoint pos, sTradeItem* pTrade, unsigned char flag);

	void OnPost_ItemListInit(intptr_t param);    // post_senddlg.cpp:1169
	void OnPost_ImgSelect(intptr_t param);       // post_senddlg.cpp:1573
	void OnPost_SelectListInit(intptr_t param);  // post_senddlg.cpp:1582
	void OnPost_EditArrayInit(intptr_t param);   // post_senddlg.cpp:1591
	void OnPostBtnArrayInit(intptr_t param);     // post_senddlg.cpp:1711
	void OnPostBtnArrayButtonUp(void);      // post_senddlg.cpp:1719
	bool OnPostItemCountResult(int ret, FrForm* pForm);   // post_senddlg.cpp:2068
	bool OnPostItemArrayResult(int ret, FrForm* pForm);   // post_senddlg.cpp:2103

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_senddlg.cpp:1040

	virtual void OnOK(void);                // post_senddlg.cpp:1859
	virtual bool OnInit(void);              // post_senddlg.cpp:1864
	void SetList(void);
	void DrawPartsSlot(FrGraphicInterface* pGDI, FrListItem* pItem);

private:
	FrListBox* m_pItemList;                 // 0x110
	FrListBox* m_pSelectList;               // 0x114
	FrArea* m_pImgSelect;                   // 0x118
	FrListItem* m_pCountItem;               // 0x11c, item list entry the count dialog was opened for
	FrButton* m_pBtnArray;                  // 0x120
	FrEdit* m_pEditArray;                   // 0x124
	CPost_SendDlg* m_pParent;               // 0x128
	WTitleFont* m_pCountFont;               // 0x12c
	WTitleFont* m_pPangFont;                // 0x130
	const Bitmap* m_pSelectNBitmap;         // 0x134
	const Bitmap* m_pSelectDBitmap;         // 0x138
	const Bitmap* m_pLineBitmap;            // 0x13c
	int m_itemCount;                        // 0x140, count picked in the count dialog
	const Bitmap* m_pUccStateBitmap[3];     // 0x144: original, copied, temp saved
	CPost_ItemCountDlg* m_pItemCountDlg;    // 0x150
	CStandingInLine* m_pArrayDlg;           // 0x154, number picker opened by the array button
	miniBtn_t m_miniBtn;                    // 0x158, cancel button
	int m_arrayIndex;                       // 0x174, picked number - 1 (-1 = all)
};

CHECK_SIZE(CPost_MyItemList, 0x178);
CHECK_SIZE(CPost_MyItemList::miniBtn_t, 0x1c);
BEGIN_LAYOUT_CHECK(CPost_MyItemList)
	CHECK_FIELD(CPost_MyItemList, m_pItemList, 0x110);
	CHECK_FIELD(CPost_MyItemList, m_pSelectList, 0x114);
	CHECK_FIELD(CPost_MyItemList, m_pImgSelect, 0x118);
	CHECK_FIELD(CPost_MyItemList, m_pCountItem, 0x11c);
	CHECK_FIELD(CPost_MyItemList, m_pBtnArray, 0x120);
	CHECK_FIELD(CPost_MyItemList, m_pEditArray, 0x124);
	CHECK_FIELD(CPost_MyItemList, m_pParent, 0x128);
	CHECK_FIELD(CPost_MyItemList, m_pCountFont, 0x12c);
	CHECK_FIELD(CPost_MyItemList, m_pPangFont, 0x130);
	CHECK_FIELD(CPost_MyItemList, m_pSelectNBitmap, 0x134);
	CHECK_FIELD(CPost_MyItemList, m_pSelectDBitmap, 0x138);
	CHECK_FIELD(CPost_MyItemList, m_pLineBitmap, 0x13c);
	CHECK_FIELD(CPost_MyItemList, m_itemCount, 0x140);
	CHECK_FIELD(CPost_MyItemList, m_pUccStateBitmap, 0x144);
	CHECK_FIELD(CPost_MyItemList, m_pItemCountDlg, 0x150);
	CHECK_FIELD(CPost_MyItemList, m_pArrayDlg, 0x154);
	CHECK_FIELD(CPost_MyItemList, m_miniBtn, 0x158);
	CHECK_FIELD(CPost_MyItemList, m_arrayIndex, 0x174);
END_LAYOUT_CHECK

// Item count dialog of the mail dialog; its size (0x128) is the `new` of
// CPost_ItemCountDlgMakeInstance.
class CPost_ItemCountDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // post_senddlg.h:222

	CPost_ItemCountDlg(void);
	virtual ~CPost_ItemCountDlg(void);

	void SetSelectItem(unsigned long item, int count);   // post_senddlg.cpp:2228
	unsigned long GetSelectItem(void);                   // post_senddlg.cpp:2233
	int GetCount(void);
	void OnPostMinusBtnUp(void);
	void OnPostPluseBtnUp(void);
	bool OnPostEditItemCountEnterKey(int);
	void OnPostMinusInit(intptr_t param);   // post_senddlg.cpp:2160
	void OnPostPluseInit(intptr_t param);   // post_senddlg.cpp:2181
	void OnPostEditItemCountInit(intptr_t param);   // post_senddlg.cpp:2201

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // post_senddlg.cpp:2145

	void SetText(void);

private:
	FrButton* m_pMinusBtn;        // 0x110
	FrButton* m_pPluseBtn;        // 0x114
	FrEdit* m_pCountEdit;         // 0x118
	int m_count;                  // 0x11c, between 1 and m_selectCount
	int m_selectCount;            // 0x120, count of the selected item (the most that can be sent)
	unsigned long m_selectItem;   // 0x124
};

CHECK_SIZE(CPost_ItemCountDlg, 0x128);
BEGIN_LAYOUT_CHECK(CPost_ItemCountDlg)
	CHECK_FIELD(CPost_ItemCountDlg, m_pMinusBtn, 0x110);
	CHECK_FIELD(CPost_ItemCountDlg, m_pPluseBtn, 0x114);
	CHECK_FIELD(CPost_ItemCountDlg, m_pCountEdit, 0x118);
	CHECK_FIELD(CPost_ItemCountDlg, m_count, 0x11c);
	CHECK_FIELD(CPost_ItemCountDlg, m_selectCount, 0x120);
	CHECK_FIELD(CPost_ItemCountDlg, m_selectItem, 0x124);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_POST_SENDDLG_H
