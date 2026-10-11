// Original: d:\build\custom\temp\code\client\projectg\ingameguildlobbydlg.h
#ifndef PG_CLIENT_PROJECTG_INGAMEGUILDLOBBYDLG_H
#define PG_CLIENT_PROJECTG_INGAMEGUILDLOBBYDLG_H

#include "..\..\defines.h"
#include "frform.h"
#include "guildformcontroller.h"
#include "contentsdoc.h"
#include <list>
#include <string>

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;
class FrListBox;
class FrWnd;
class WFont;
class WTitleFont;
struct FrListItem;

namespace _guild {

class IGuildAuthController;
class CGuildManager;
class CGuildMaster;
class CGuildSubMaster;
class CGuildMember;
class CGuildNull;

// The guild lobby form (members, search, guild information).
class CGuildMainForm : public FrForm {
	LAYOUT_CHECK_FRIEND;
	// The rank controllers read the form's controls (IGuildAuthController::m_linkForm).
	friend class IGuildAuthController;
	friend class CGuildManager;
	friend class CGuildMaster;
	friend class CGuildSubMaster;
	friend class CGuildMember;
	friend class CGuildNull;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	// One row of the member list (the list item's data).
	struct sMemberItem {
		// ingameguildlobbydlg.h:119
		sMemberItem(void) {
			pUserInfo = 0;
			bEditing = false;
		}

		const GUILD_USER_LIST *pUserInfo;   // 0x0
		bool bEditing;                      // 0x4: the talk edit is open on this row
	};

	CGuildMainForm(void);
	virtual ~CGuildMainForm(void);

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void Refresh(void);
	void RefreshMyGuildInfo(void);
	void RefreshMemberPageList(void);
	void RefreshSearchPageList(void);
	void RefreshMyGuildHistory(void);
	void RefreshGuildMember(void);
	void RefreshGuildSearch(void);
	void DisableJoinButton(void);
	void InitSearchMode(void);
	void LockScrollMemberList(void);
	void UnLockScrollMemberList(void);
	void RequestMember_Page(int page);
	void RequestSearch_Page(int page);
	void ClearForm(void);

	// ingameguildlobbydlg.h:139
	void SetLinkGuildInfo(CGuildInfo *pGuildInfo) { m_pLinkGuildInfo = pGuildInfo; }
	// ingameguildlobbydlg.h:140
	void SetCommandUserInfo(const GUILD_USER_LIST *pUserInfo) { m_pCommandUserInfo = pUserInfo; }
	// ingameguildlobbydlg.h:141
	void ClearCommandUserInfo(void) { m_pCommandUserInfo = 0; }

	void RequestChangeClass(GUILD_CLASS_IDX classIndex);

private:
	// ingameguildlobbydlg.h:184
	CGuildInfo *GuildInfo(void) { return m_pLinkGuildInfo; }
	// ingameguildlobbydlg.h:185
	CGuildFormController *Controller(void) { return m_pController; }

	void InitControl(void);
	void LoadResource(void);
	void CreateAuthController(void);
	eAuthority ConvertGuildClass(void);
	bool CanExecuteClass(const GUILD_USER_LIST *pUserInfo);
	void EnableAuthCommand(GUILD_CLASS_IDX classIndex);
	void PopupMemberCommand(float x, float y);
	void SelectMemberItem(FrListBox *pList, FrListItem *pItem);

	void ProcManagerCommand(const char *pCommand);
	void CheckTalkState(void);
	void ResetEditState(void *pData);
	void UnSelectMember(void);
	const char *GuildMsg(int index);
	GUILD_CLASS_IDX MyGuildClassIndex(void);

	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnGuildForm_CloseLButtonUp(void);
	void OnGuildInfo_ButtonInit(int param);
	void OnGuildMember_ButtonInit(int param);
	void OnGuildInfo_LButtonUp(void);
	void OnGuildMember_LButtonUp(void);
	void OnGuildSearch_ButtonInit(int param);
	void OnGuildSearch_LButtonUp(void);
	void OnGuildForm_CloseInit(int param);
	void OnGuildMemberTalk_EditInit(int param);
	void OnGuildMemberTalk_ButtonInit(int param);
	void OnGuildManagerCmd_ContextInit(int param);
	void OnMemberPage_ListOwnerDraw(int param);
	void OnGuildSearchPage_OwnerDraw(int param);
	void OnGuildManagerCmd_LButtonUp(void);
	void OnGuildManagerCmd_LButtonDown(void);
	void OnGuildSearchInfo_LButtonUp(void);
	void OnGuildSearchInfo_OwnerDraw(int param);
	void OnGuildSearchInfo_LButtonDown(void);
	void OnGuildSearchPrev_LButtonUp(void);
	void OnGuildSearchNext_LButtonUp(void);
	void OnGuildSearch_EnterKey(int param);
	void OnGuildIntroConfirm_LButtonUp(void);
	void OnGuildNoticeConfirm_LButtonUp(void);
	void OnGuildJoin_LButtonUp(void);
	void OnGuildJoinAbort_LButtonUp(void);
	void OnGuildShutDown_LButtonUp(void);
	void OnGuildOut_LButtonUp(void);
	void OnMemberPage_ListLButtonDown(void);
	void OnGuildSearchPage_LButtonDown(void);
	void OnGuildMemberInfo_LButtonDown(void);
	void OnGuildDetail_AreaInit(int param);
	void OnGuildIntroduce_AreaInit(int param);
	void OnGuildIntroduce_EditInit(int param);
	void OnGuildNotice_EditInit(int param);
	void OnGuildNotice_AreaInit(int param);
	void OnGuildAct_AreaInit(int param);
	void OnEmptyGuild_AreaInit(int param);
	void OnEmptyGuild_AreaOwnerDraw(int param);
	void OnGuildMemberInfoBg_AreaInit(int param);
	void OnGuildMemberInfoBg_OwnerDraw(int param);
	void OnSearchBg1_AreaInit(int param);
	void OnSearchBg2_AreaInit(int param);
	void OnGuildAct_EditInit(int param);
	void OnGuildSearch_EditInit(int param);
	void OnGuildIntroConfirm_ButtonInit(int param);
	void OnGuildNoticeConfirm_ButtonInit(int param);
	void OnGuildJoin_ButtonInit(int param);
	void OnGuildSearchInfo_ButtonInit(int param);
	void OnGuildJoinAbort_ButtonInit(int param);
	void OnGuildSearchPrev_ButtonInit(int param);
	void OnGuildSearchNext_ButtonInit(int param);
	void OnGuildShutDown_ButtonInit(int param);
	void OnGuildOut_ButtonInit(int param);
	void OnGuildMemberInfo_ListBoxInit(int param);
	void OnMemberPage_ListBoxInit(int param);
	void OnGuildSearchInfo_ListBoxInit(int param);
	void OnGuildSearchPage_ListBoxInit(int param);

private:
	FrButton *m_pTabs[3];          // 0x110: the page tabs (0 info, 1 member, 2 search; SetStatus by page)
	FrButton *m_pButtons[11];       // 0x11C: 0 close (OnGuildForm_CloseInit), 3 join (DisableJoinButton), 8 member talk
	FrArea *m_pAreas[8];            // 0x148: 2 guild notice (OnGuildNotice_AreaInit)
	FrEdit *m_pEdits[5];            // 0x168: 0 introduction, 1 notice, 2 activity, 3 search, 4 member talk
	FrListBox *m_pListBoxes[4];     // 0x17C: 0 members (LockScrollMemberList), 1 guild search results, 2 member pages, 3 search pages
	FrWnd *m_pManagerCmd;           // 0x18C: the manager command context (OnGuildManagerCmd_ContextInit)
	FrListItem *m_pSelectedMember;  // 0x190: the member list item the command menu acts on
	const Bitmap *m_pBitmaps[17];   // 0x194: LoadResource; [14] the selected search row, [15] the talk icon (DrawGuildTalk)
	WTitleFont *m_pPageFont;          // 0x1D8: the page numbers
	WTitleFont *m_pCurrentPageFont;   // 0x1DC: the current page number
	eFormStyle m_formStyle;         // 0x1E0: the page shown
	CGuildFormController *m_pController;   // 0x1E4 (Controller)
	CGuildInfo *m_pLinkGuildInfo;   // 0x1E8: the guild content (SetLinkGuildInfo)
	GUILD_LIST m_joinGuild;         // 0x1EC: the selected search guild a join request goes to (CGuildNull::OnGuildJoin_LButtonUp)
	std::map<GUILD_CLASS_IDX, IGuildAuthController *> m_authControllers;   // 0x2B0: one per guild rank
	float m_talkTextX;              // 0x2BC: x offset of the talk text in a member row (DrawGuildTalk)
	float m_talkIconX;              // 0x2C0: x offset of the talk icon in a member row (DrawGuildTalk)
	std::list<sMemberItem> m_memberItems;  // 0x2C4: the rows of the member list box
	CPageArrow *m_pSearchArrow;     // 0x2D0: the guild search page arrows
	const GUILD_USER_LIST *m_pCommandUserInfo;   // 0x2D4: the member the command menu acts on
	std::list<const GUILD_LIST *> m_searchItems;   // 0x2D8: the rows of the search list box
	int m_memberPage;               // 0x2E4: the current member page (1-based)
	unsigned int m_searchPage;      // 0x2E8: the current guild search page (1-based)
	int m_searchMode;               // 0x2EC: reset by InitSearchMode
	std::string m_searchText;       // 0x2F0
	PADDING(0x30C, 0x310);
};

CHECK_SIZE(CGuildMainForm, 0x310);
BEGIN_LAYOUT_CHECK(CGuildMainForm)
	CHECK_FIELD(CGuildMainForm, m_pTabs, 0x110);
	CHECK_FIELD(CGuildMainForm, m_pAreas, 0x148);
	CHECK_FIELD(CGuildMainForm, m_pButtons, 0x11C);
	CHECK_FIELD(CGuildMainForm, m_pSearchArrow, 0x2D0);
	CHECK_FIELD(CGuildMainForm, m_pEdits, 0x168);
	CHECK_FIELD(CGuildMainForm, m_pSelectedMember, 0x190);
	CHECK_FIELD(CGuildMainForm, m_pPageFont, 0x1D8);
	CHECK_FIELD(CGuildMainForm, m_pCurrentPageFont, 0x1DC);
	CHECK_FIELD(CGuildMainForm, m_pController, 0x1E4);
	CHECK_FIELD(CGuildMainForm, m_pCommandUserInfo, 0x2D4);
	CHECK_FIELD(CGuildMainForm, m_pListBoxes, 0x17C);
	CHECK_FIELD(CGuildMainForm, m_pManagerCmd, 0x18C);
	CHECK_FIELD(CGuildMainForm, m_authControllers, 0x2B0);
	CHECK_FIELD(CGuildMainForm, m_memberPage, 0x2E4);
	CHECK_FIELD(CGuildMainForm, m_formStyle, 0x1E0);
	CHECK_FIELD(CGuildMainForm, m_pLinkGuildInfo, 0x1E8);
	CHECK_FIELD(CGuildMainForm, m_searchPage, 0x2E8);
	CHECK_FIELD(CGuildMainForm, m_searchMode, 0x2EC);
	CHECK_FIELD(CGuildMainForm, m_memberItems, 0x2C4);
	CHECK_FIELD(CGuildMainForm, m_pBitmaps, 0x194);
	CHECK_FIELD(CGuildMainForm, m_joinGuild, 0x1EC);
	CHECK_FIELD(CGuildMainForm, m_searchItems, 0x2D8);
	CHECK_FIELD(CGuildMainForm, m_searchText, 0x2F0);
	CHECK_FIELD(CGuildMainForm, m_talkTextX, 0x2BC);
	CHECK_FIELD(CGuildMainForm, m_talkIconX, 0x2C0);
END_LAYOUT_CHECK

// The "request to join a guild" popup form; its size (0x130) is the `new` of
// FrGuildJoinMakeInstance.
class FrGuildJoin : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // ingameguildlobbydlg.h:333

	FrGuildJoin(void);
	virtual ~FrGuildJoin(void);

	virtual bool OnInit(void);

	void SetGuildNumber(int guildNumber);
	void SetSuccessRequest(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnGuildJoin_AreaInit(int param);
	void OnGuildJoin_AreaOwnerDraw(int param);
	void OnJoin_EditInit(int param);
	void OnRequestJoin_ButtonInit(int param);
	void OnRequestJoin_LButtonUp(void);
	void OnRequestOK_ButtonInit(int param);
	void OnRequestOK_LButtonUp(void);
	void OnFormClose_ButtonInit(int param);
	void OnFormClose_LButtonUp(void);

private:
	FrArea* m_pJoinArea;           // 0x110 (OnGuildJoin_AreaInit)
	FrEdit* m_pJoinEdit;           // 0x114
	FrButton* m_pButtons[3];       // 0x118 (request join, request OK, form close)
	const Bitmap* m_pBackground;   // 0x124 ("guild_popup_write_bg")
	int m_guildNumber;             // 0x128
	bool m_bRequested;             // 0x12c
};

CHECK_SIZE(FrGuildJoin, 0x130);
BEGIN_LAYOUT_CHECK(FrGuildJoin)
	CHECK_FIELD(FrGuildJoin, m_pJoinArea, 0x110);
	CHECK_FIELD(FrGuildJoin, m_pJoinEdit, 0x114);
	CHECK_FIELD(FrGuildJoin, m_pButtons, 0x118);
	CHECK_FIELD(FrGuildJoin, m_pBackground, 0x124);
	CHECK_FIELD(FrGuildJoin, m_guildNumber, 0x128);
	CHECK_FIELD(FrGuildJoin, m_bRequested, 0x12c);
END_LAYOUT_CHECK

} // namespace _guild

#endif // PG_CLIENT_PROJECTG_INGAMEGUILDLOBBYDLG_H
