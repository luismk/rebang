// Original: d:\build\custom\temp\code\client\projectg\detailedroominfodlg.h
#ifndef PG_CLIENT_PROJECTG_DETAILEDROOMINFODLG_H
#define PG_CLIENT_PROJECTG_DETAILEDROOMINFODLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include "globalgamedefine.h"
#include <list>

class FrButton;
class FrEdit;
class FrListBox;
class Bitmap;

// A player of the room the dialog details: the data of a user-list item
// (an element of the CSharedDoc list at 0x4AA8 that OnUserListInit walks).
// Byte-packed (the rank dword sits at 0xE). The original name, header and
// size are not known; only the members the transcribed functions read are
// named.
#pragma pack(push, 1)
struct sRoomUserInfo {
	unsigned long m_uid;            // 0x0, key of CSharedDoc::m_userMap
	unsigned char m_level;          // 0x4, the level sort key
	unsigned char m_status;         // 0x5, low 5 bits: the status sort key
	unsigned char m_flags;          // 0x6, 0x14 tested by OnUserListInit
	PADDING(0x7, 0xE);
	unsigned long m_rankValue;      // 0xE, sorted instead of the level in channels with flag 0x80
};
#pragma pack(pop)
CHECK_FIELD(sRoomUserInfo, m_uid, 0x0);
CHECK_FIELD(sRoomUserInfo, m_level, 0x4);
CHECK_FIELD(sRoomUserInfo, m_status, 0x5);
CHECK_FIELD(sRoomUserInfo, m_flags, 0x6);
CHECK_FIELD(sRoomUserInfo, m_rankValue, 0xE);

// Detailed room information dialog: the room's description, its player list
// (sortable by the level, id and status headers) and the join, user info and
// gallery buttons. Its size (0x220) is the `new` of FrDtRoomDlgMakeInstance.
class FrDtRoomDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // detailedroominfodlg.h:18

	// Sort order of the player list: a header, plus 3 when descending.
	enum eHeader {
		HEADER_LEVEL = 0,
		HEADER_ID = 1,
		HEADER_STATUS = 2
	};

	FrDtRoomDlg(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void SetRoomInfo(const sRoomInfo& roomInfo);
	void EnableJoinBtn(bool bEnable);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnGameDescInit(intptr_t param);
	void OnJoinBtnInit(intptr_t param);
	void OnJoinBtnUp(void);
	void OnUserInfoBtnInit(intptr_t param);
	void OnUserInfoBtnUp(void);
	void OnGalleryBtnInit(int param);
	void OnGalleryBtnUp(void);
	void OnLevelHdrInit(intptr_t param);
	void OnLevelHdrDown(void);
	void OnIdHdrInit(intptr_t param);
	void OnIdHdrDown(void);
	void OnStatusHdrInit(intptr_t param);
	void OnStatusHdrDown(void);
	void OnHeaderLBtnDown(eHeader header);
	void OnUserListInit(intptr_t param);
	void OnUserListOwnerDraw(int param);
	void OnUserListLBtnDown(void);
	void OnUserListRBtnUp(void);
	bool OnPasswordDlgResult(int result, FrForm* pForm);

private:
	std::list<void*> m_delList;        // 0x110 (list items OnProc removes from the user list)
	FrEdit* m_pGameDesc;               // 0x11c (OnGameDescInit)
	FrButton* m_pJoinBtn;              // 0x120 (OnJoinBtnInit)
	FrButton* m_pUserInfoBtn;          // 0x124 (OnUserInfoBtnInit)
	FrButton* m_pGalleryBtn;           // 0x128 (OnGalleryBtnInit)
	FrButton* m_pLevelHdr;             // 0x12c (OnLevelHdrInit)
	FrButton* m_pIdHdr;                // 0x130 (OnIdHdrInit)
	FrButton* m_pStatusHdr;            // 0x134 (OnStatusHdrInit)
	FrListBox* m_pUserList;            // 0x138 (OnUserListInit)
	sRoomInfo m_roomInfo;              // 0x13c (SetRoomInfo)
	sRoomUserInfo* m_pSelectedUser;    // 0x1f0 (data of the user-list item clicked)
	int m_sortState;                   // 0x1f4 (eHeader, +3 when descending)
	const Bitmap* m_pCharIcon[6];      // 0x1f8 (OnUserListInit: i_male, i_female, i_male_02, i_female_02, i_male_03, i_female_03; OnUserListOwnerDraw indexes it)
	const Bitmap* m_pMannerIcon[2];    // 0x210 (i_male_manner, i_female_manner)
	const Bitmap* m_pAngelIcon[2];     // 0x218 (i_male_angel, i_female_angel)
};
BEGIN_LAYOUT_CHECK(FrDtRoomDlg)
	CHECK_FIELD(FrDtRoomDlg, m_delList, 0x110);
	CHECK_FIELD(FrDtRoomDlg, m_pGameDesc, 0x11c);
	CHECK_FIELD(FrDtRoomDlg, m_pJoinBtn, 0x120);
	CHECK_FIELD(FrDtRoomDlg, m_pUserInfoBtn, 0x124);
	CHECK_FIELD(FrDtRoomDlg, m_pGalleryBtn, 0x128);
	CHECK_FIELD(FrDtRoomDlg, m_pLevelHdr, 0x12c);
	CHECK_FIELD(FrDtRoomDlg, m_pIdHdr, 0x130);
	CHECK_FIELD(FrDtRoomDlg, m_pStatusHdr, 0x134);
	CHECK_FIELD(FrDtRoomDlg, m_pUserList, 0x138);
	CHECK_FIELD(FrDtRoomDlg, m_roomInfo, 0x13c);
	CHECK_FIELD(FrDtRoomDlg, m_pSelectedUser, 0x1f0);
	CHECK_FIELD(FrDtRoomDlg, m_sortState, 0x1f4);
	CHECK_FIELD(FrDtRoomDlg, m_pCharIcon, 0x1f8);
	CHECK_FIELD(FrDtRoomDlg, m_pMannerIcon, 0x210);
	CHECK_FIELD(FrDtRoomDlg, m_pAngelIcon, 0x218);
END_LAYOUT_CHECK
CHECK_SIZE(FrDtRoomDlg, 0x220);

#endif // PG_CLIENT_PROJECTG_DETAILEDROOMINFODLG_H
