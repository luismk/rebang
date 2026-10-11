// Original: d:\build\custom\temp\code\client\projectg\userlistdlg.h
#ifndef PG_CLIENT_PROJECTG_USERLISTDLG_H
#define PG_CLIENT_PROJECTG_USERLISTDLG_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class Bitmap;
class FrButton;
class FrListBox;
class FrAddFriendDlg;
struct sBriefUserInfo;

// Lobby user list dialog: the list box of the lobby's players (sorted by
// sex, level, nickname or guild), with invite, whisper and add-friend
// buttons. Its size (0x168) is the `new` of FrUserListDlgMakeInstance.
class FrUserListDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	FrUserListDlg(void);

	void MakeUserList(std::map<unsigned long, sBriefUserInfo>& userMap);
	void SortUserList(void);
	void EnableRightButton(bool enable);
	void AddUser(void* data);
	void DelUser(void* data, int cmpLen);
	void SetAddFriendDlgNick(const char* nick, const char* unused);
	bool OnAddFriendDlgResult(int result, FrForm* pForm);
	void CloseInviteDelayDlg(void);
	void SetRequestFriendData(const char* nick, unsigned long uid);
	bool IsOpenAddFriendDlg(void) const {   // userlistdlg.h:34
		return m_pAddFriendDlg != 0;
	}

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnInviteBtnInit(intptr_t param);
	void OnInviteBtnUp(void);
	void OnWhisperBtnInit(intptr_t param);
	void OnWhisperBtnUp(void);
	void OnFriendBtnUp(void);
	void OnSortBtnInit(intptr_t param);
	void OnSexBtnUp(void);
	void OnLevelBtnUp(void);
	void OnGuildBtnUp(void);
	void OnIdBtnUp(void);
	void OnUserListInit(intptr_t param);
	void OnUserListOwnerDraw(int param);
	void OnUserListLBtnDown(void);
	void OnUserListRBtnUp(void);
	void OnUserListDClick(void);

private:
	float m_inviteDelayTime;            // 0x110 (OnProc counts it down)
	FrForm* m_pInviteDelayDlg;          // 0x114 (closed when the delay runs out)
	FrAddFriendDlg* m_pAddFriendDlg;    // 0x118 (OnFriendBtnUp)
	FrButton* m_pWhisperBtn;            // 0x11c (OnWhisperBtnInit)
	FrButton* m_pInviteBtn;             // 0x120 (OnInviteBtnInit)
	FrListBox* m_pUserList;             // 0x124 (OnUserListInit)
	unsigned long m_selectedUID;        // 0x128, dword 4 of the clicked sBriefUserInfo
	unsigned long m_selectedID;         // 0x12c, dword 0 of the clicked sBriefUserInfo
	const Bitmap* m_pMaleBmp;           // 0x130 "i_male"
	const Bitmap* m_pFemaleBmp;         // 0x134 "i_female"
	const Bitmap* m_pMale02Bmp;         // 0x138 "i_male_02"
	const Bitmap* m_pFemale02Bmp;       // 0x13c "i_female_02"
	const Bitmap* m_pMale03Bmp;         // 0x140 "i_male_03"
	const Bitmap* m_pFemale03Bmp;       // 0x144 "i_female_03"
	const Bitmap* m_pMaleMannerBmp;     // 0x148 "i_male_manner"
	const Bitmap* m_pFemaleMannerBmp;   // 0x14c "i_female_manner"
	const Bitmap* m_pMaleAngelBmp;      // 0x150 "i_male_angel"
	const Bitmap* m_pFemaleAngelBmp;    // 0x154 "i_female_angel"
	const Bitmap* m_pInGameBmp;         // 0x158 "ingame"
	int m_sortType;                     // 0x15c, SortUserList's switch (0..3)
	bool m_sortReverse;                 // 0x160
	PADDING(0x161, 0x168);
};
BEGIN_LAYOUT_CHECK(FrUserListDlg)
	CHECK_FIELD(FrUserListDlg, m_inviteDelayTime, 0x110);
	CHECK_FIELD(FrUserListDlg, m_pInviteDelayDlg, 0x114);
	CHECK_FIELD(FrUserListDlg, m_pAddFriendDlg, 0x118);
	CHECK_FIELD(FrUserListDlg, m_pWhisperBtn, 0x11c);
	CHECK_FIELD(FrUserListDlg, m_pInviteBtn, 0x120);
	CHECK_FIELD(FrUserListDlg, m_pUserList, 0x124);
	CHECK_FIELD(FrUserListDlg, m_selectedUID, 0x128);
	CHECK_FIELD(FrUserListDlg, m_selectedID, 0x12c);
	CHECK_FIELD(FrUserListDlg, m_pMaleBmp, 0x130);
	CHECK_FIELD(FrUserListDlg, m_pInGameBmp, 0x158);
	CHECK_FIELD(FrUserListDlg, m_sortType, 0x15c);
	CHECK_FIELD(FrUserListDlg, m_sortReverse, 0x160);
END_LAYOUT_CHECK
CHECK_SIZE(FrUserListDlg, 0x168);

#endif // PG_CLIENT_PROJECTG_USERLISTDLG_H
