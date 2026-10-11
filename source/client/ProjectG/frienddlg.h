// Original: d:\build\custom\temp\code\client\projectg\frienddlg.h
#ifndef PG_CLIENT_PROJECTG_FRIENDDLG_H
#define PG_CLIENT_PROJECTG_FRIENDDLG_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class CPost_SendDlg;
class FrGiftDlg;
class FrListBox;
struct sFriend;

// The friend list dialog opened by the gift and mail dialogs; picking a
// friend fills the parent's nick edit boxes. Size 0x170 (the `new` of
// FrFriendDlgMakeInstance).
class FrFriendDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frienddlg.h:26

	FrFriendDlg(void);

	void SetParent(FrGiftDlg* pParent);
	void SetParent2(CPost_SendDlg* pParent);
	bool IsSelectPerson(void);
	bool HaveFriends(void);
	void UpdateFriendList(void);
	unsigned long GetUid(void) const;   // frienddlg.h:32, defined below

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnFriendListInit(intptr_t param);
	void OnFriendListOwnerDraw(intptr_t param);
	void OnFriendListLBtnUp(void);
	void OnFriendListRBtnUp(void);
	void OnFriendListDblClick(void);
	void OnCancelBtnUp(void);

private:
	CPost_SendDlg* m_pPostSendDlg;   // 0x110, parent when opened by the mail dialog
	FrGiftDlg* m_pGiftDlg;           // 0x114, parent when opened by the gift dialog
	FrListBox* m_pFriendList;        // 0x118
	std::list<sFriend> m_friends;    // 0x11c
	unsigned long m_uid;             // 0x128, the selected friend (-1: none)
	const Bitmap* m_pSexIcon[2];     // 0x12c: male, female (indexed by sFriend::flags bit 0)
	std::string m_savedNick;         // 0x134, the parent's nick before the pick, restored by cancel
	std::string m_savedConfirm;      // 0x150, the parent's confirm/receiver line before the pick
	PADDING(0x16c, 0x170);
};

CHECK_SIZE(FrFriendDlg, 0x170);
BEGIN_LAYOUT_CHECK(FrFriendDlg)
	CHECK_FIELD(FrFriendDlg, m_pPostSendDlg, 0x110);
	CHECK_FIELD(FrFriendDlg, m_pGiftDlg, 0x114);
	CHECK_FIELD(FrFriendDlg, m_pFriendList, 0x118);
	CHECK_FIELD(FrFriendDlg, m_friends, 0x11c);
	CHECK_FIELD(FrFriendDlg, m_uid, 0x128);
	CHECK_FIELD(FrFriendDlg, m_pSexIcon, 0x12c);
	CHECK_FIELD(FrFriendDlg, m_savedNick, 0x134);
	CHECK_FIELD(FrFriendDlg, m_savedConfirm, 0x150);
END_LAYOUT_CHECK

// Post_SendDlg defines POST_SENDDLG_FRIENDDLG_INLINE empty before including
// this header: the original's out-of-line copy of this accessor is in its
// object; every other unit inlines it.
#ifndef POST_SENDDLG_FRIENDDLG_INLINE
#define POST_SENDDLG_FRIENDDLG_INLINE inline
#endif

// frienddlg.h:32
POST_SENDDLG_FRIENDDLG_INLINE unsigned long FrFriendDlg::GetUid(void) const {
	return m_uid;
}

#endif // PG_CLIENT_PROJECTG_FRIENDDLG_H
