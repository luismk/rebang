// Original: d:\build\custom\temp\code\client\projectg\ingameguildlobbydlg.cpp
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "ingameguildlobbydlg.h"
#include "guilddefine.h"
#include "GuildAuthController.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "frcontextmenuctrl.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "fredit.h"
#include "fresh.h"
#include "bitmap.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "InputManager.h"
#include "mousecursor.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "wtemplate.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "wview.h"
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// The page number fonts of CGuildMainForm::OnInit.
extern "C" char* data_abf940 = "guild_over_font.tga";
extern "C" char* data_abf944 = "guild_normal_font.tga";

namespace _guild {

// Name of the guild actor the forms message (the first pointer of this
// unit's data; its header of origin is not recovered yet).
static const char* s_guildActorName = "GuildActor";

// ingameguildlobbydlg.cpp:24 (run-time type and factory registration of CGuildMainForm)
const WRTTI CGuildMainForm::m_RTTI("CGuildMainForm", &FrForm::m_RTTI);

IObject* CGuildMainFormMakeInstance(void) {
	return new CGuildMainForm;
}

struct __sCGuildMainForm {
	__sCGuildMainForm(void) {
		ObjectFactory().AddObjectFunctor(CGuildMainFormMakeInstance, "CGuildMainForm");
	}
};
static __sCGuildMainForm s_registerCGuildMainForm;

// ingameguildlobbydlg.cpp:25
const sFRESH_MSGMAP* CGuildMainForm::GetMessageMap(void) const {
	return &CGuildMainForm::_MsgMap;
}

// ingameguildlobbydlg.cpp:110
CGuildMainForm::CGuildMainForm(void) {
	InitControl();
}

// ingameguildlobbydlg.cpp:116
CGuildMainForm::~CGuildMainForm(void) {
	if (g_resrcmng && m_pCurrentPageFont) { g_resrcmng->Release(m_pCurrentPageFont); m_pCurrentPageFont = 0; }
	if (g_resrcmng && m_pPageFont) { g_resrcmng->Release(m_pPageFont); m_pPageFont = 0; }
	std::for_each(m_authControllers.begin(), m_authControllers.end(), Delete_SecondObject());
	if (m_pSearchArrow) { delete m_pSearchArrow; m_pSearchArrow = 0; }
	if (m_pController) { delete m_pController; m_pController = 0; }
}

// ingameguildlobbydlg.cpp:126
bool CGuildMainForm::OnInit(void) {
	LoadResource();

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));
	info.filename = &data_abf944;
	info.fontw = 12;
	info.fonth = 14;
	info.numPages = 1;
	info.texw = 120;
	info.texh = 14;
	info.pCharSet = "0123456789";
	m_pPageFont = g_resrcmng->GetTitleFont();
	m_pPageFont->Create(&info);

	memset(&info, 0, sizeof(info));
	info.filename = &data_abf940;
	info.fontw = 13;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 130;
	info.texh = 16;
	info.pCharSet = "0123456789";
	m_pCurrentPageFont = g_resrcmng->GetTitleFont();
	m_pCurrentPageFont->Create(&info);

	m_formStyle = FORM_GUILD_INFO;
	Controller()->ControllForm(m_formStyle, ConvertGuildClass());
	m_pTabs[0]->SetStatus(FrButton::PRESSED);

	CreateAuthController();

	// The talk text and icon offsets in a member row.
	const WRect &listRect = m_pListBoxes[0]->GetRect();
	m_talkTextX = listRect.w * 0.5f + 6.0f;
	m_talkIconX = listRect.w - m_pButtons[8]->GetRect().w * 1.4f - 1.0f;

	int guildNumber = GuildInfo()->GetMyGuildInfo().guildNumber;
	if (guildNumber != 0) {
		WSendPacket packet((enumClientPacket)0x101);
		packet.Encode4(guildNumber);
		packet.Send(SEND_TO_DEFAULT);
	}

	{
		WSendPacket packet(0x105);
		packet.Encode4(1);
		packet.Send(SEND_TO_DEFAULT);
	}

	return FrForm::OnInit();
}

// ingameguildlobbydlg.cpp:190
void CGuildMainForm::OnProc(const float dt) {
	if (m_formStyle != FORM_GUILD_MEMBER) {
		return;
	}

	if (g_input->GetButton(LEFT_BUTTON) || g_input->GetButton(RIGHT_BUTTON)) {
		// A click outside the member list closes the manager command menu.
		if (CMouseCursor::Instance()->InArea(m_pListBoxes[0]->GetRect())) {
			return;
		}

		if (m_pManagerCmd != 0) {
			m_pManagerCmd->SetVisible(false);
		}
	}
}

// ingameguildlobbydlg.cpp:213
void CGuildMainForm::Refresh(void) {
	switch (m_formStyle) {
	case 1:
		RefreshMyGuildInfo();
		RefreshMyGuildHistory();
		break;
	case 2:
		RefreshGuildMember();
		break;
	case 4:
		RefreshGuildSearch();
		break;
	}
}

// ingameguildlobbydlg.cpp:232
void CGuildMainForm::RefreshMyGuildInfo(void) {
	const GUILD_INFO &info = GuildInfo()->GetMyGuildInfo();

	if (info.classIndex == 9) {
		// The waiting member's form.
		Controller()->ControllForm(FORM_GUILD_INFO, AUTHORITY_CLASS9);
	} else if (info.classIndex == 3) {
		Controller()->ControllForm(FORM_GUILD_INFO, AUTHORITY_CLASS3);
	}

	m_pEdits[0]->ClearLine();
	m_pEdits[0]->AddText(info.introduce, false, true);
	m_pEdits[0]->ResetKeyFocus();

	m_pEdits[0]->GetScrollBar()->ScrollToFirst();

	m_pEdits[1]->ClearLine();
	m_pEdits[1]->AddText(info.notice, false, true);

	m_pEdits[1]->GetScrollBar()->ScrollToFirst();

	m_pEdits[2]->ClearLine();
}

// ingameguildlobbydlg.cpp:264
void CGuildMainForm::RefreshMyGuildHistory(void) {
	CGuildInfo *pGuildInfo = GuildInfo();

	std::string history;

	std::list<GUILD_HISTORY>::const_iterator it = pGuildInfo->GetHistoryList().begin();
	for (; it != pGuildInfo->GetHistoryList().end(); ++it) {
		char line[256];
		memset(line, 0, sizeof(line));

		// " - <year> <month> <day> <state message>"
		sprintf(line, GUILD_DATE_FORMAT, it->date.wYear, it->date.wMonth, it->date.wDay,
			GuildInfo()->GuildMsg(ConvertGuildHistory((GUILD_STATE_IDX)it->state)));

		history += line;
	}

	m_pEdits[2]->AddText(history.c_str(), false, true);
}

// ingameguildlobbydlg.cpp:289
void CGuildMainForm::RefreshGuildMember(void) {
	if (m_pListBoxes[0]->GetCurrentItemSize(true) > 0) {
		m_pListBoxes[0]->ClearItem();
	}

	if (!m_memberItems.empty()) {
		m_memberItems.clear();
	}

	m_pSelectedMember = 0;
	const std::list<GUILD_USER_LIST> *pMemberList = GuildInfo()->GetMemberList(m_memberPage);
	if (pMemberList == 0) {
		return;
	}

	std::list<GUILD_USER_LIST>::const_iterator it = pMemberList->begin();
	for (; it != pMemberList->end(); ++it) {
		sMemberItem memberItem;

		memberItem.pUserInfo = &*it;
		m_memberItems.push_back(memberItem);
	}

	std::list<sMemberItem>::iterator item = m_memberItems.begin();
	for (; item != m_memberItems.end(); ++item) {
		m_pListBoxes[0]->AddItem(&*item);
	}

	RefreshMemberPageList();

	ClearCommandUserInfo();
	if (m_pManagerCmd) {
		m_pManagerCmd->SetVisible(false);
	}
}

// ingameguildlobbydlg.cpp:337
void CGuildMainForm::RefreshGuildSearch(void) {
	if (m_pListBoxes[1]->GetCurrentItemSize(true) > 0) {
		m_pListBoxes[1]->ClearItem();
	}

	if (!m_searchItems.empty()) {
		m_searchItems.clear();
	}

	const std::list<GUILD_LIST> *pSearchList = GuildInfo()->GetSearchList(m_searchPage);
	if (pSearchList == 0) {
		return;
	}

	std::list<GUILD_LIST>::const_iterator it = pSearchList->begin();
	for (; it != pSearchList->end(); ++it) {
		m_searchItems.push_back(&*it);
	}

	std::list<const GUILD_LIST *>::iterator item = m_searchItems.begin();
	for (; item != m_searchItems.end(); ++item) {
		m_pListBoxes[1]->AddItem((void *)*item);
	}

	RefreshSearchPageList();
}

// ingameguildlobbydlg.cpp:372
void CGuildMainForm::RefreshMemberPageList(void) {
	m_pListBoxes[2]->ClearItem();
	int maxPage = GuildInfo()->GetMaxMemberPage();

	for (int i = 0; i < maxPage; i++) {
		m_pListBoxes[2]->AddItem(0);
	}

	FrListBox *pList = m_pListBoxes[2];
	int count = pList->GetCurrentItemSize(true);

	int width = m_pListBoxes[2]->GetItemWidth();

	WRect rect = pList->GetRect();

	rect.x = (m_rect.w + m_rect.x) * 0.5f - (float)width * (float)count + (float)count * 10.0f;
	m_pListBoxes[2]->SetRect(rect);
}

// ingameguildlobbydlg.cpp:399
void CGuildMainForm::RefreshSearchPageList(void) {
	m_pListBoxes[3]->ClearItem();
	int maxPage = GuildInfo()->GetMaxSearchPage();

	m_pListBoxes[1]->GetCurrentItemSize(true);

	unsigned int group = (m_searchPage - 1) / 5;

	int first = group * 5;

	int last = 0;
	if ((int)m_searchPage < maxPage) {
		if (group == (unsigned int)(maxPage - 1) / 5) {
			// The last group of pages.
			last = maxPage;
		} else {
			last = first + 5;
		}
	} else if (m_searchPage == maxPage) {
		last = m_searchPage;
	}

	for (int i = first; i < last; i++) {
		m_pListBoxes[3]->AddItem((void *)i);
	}

	m_pSearchArrow->RefreshArrowButton(maxPage, m_searchPage);

	int count = m_pListBoxes[3]->GetCurrentItemSize(true);
	FrWnd *pBg = m_pAreas[7];

	float center = pBg->GetRect().w * 0.5f + pBg->GetRect().x;

	WRect rect = m_pListBoxes[3]->GetRect();
	rect.x = center - (float)m_pListBoxes[3]->GetItemWidth() * count * 0.5f + 6.0f;
	m_pListBoxes[3]->SetRect(rect);
}

// ingameguildlobbydlg.cpp:449
void CGuildMainForm::RequestMember_Page(int page) {
	GUILD_CLASS_IDX classIndex = MyGuildClassIndex();
	if (classIndex == GUILD_CLASS_NONE || classIndex == (GUILD_CLASS_IDX)9) {
		// No guild, or a join request still waiting.
		return;
	}

	WSendPacket packet((enumClientPacket)0x10F);
	packet.Encode4(GuildInfo()->GetMyGuildInfo().guildNumber);
	packet.Encode4(page != 0 ? page : m_memberPage);
	packet.Send(SEND_TO_DEFAULT);
}

// ingameguildlobbydlg.cpp:463
void CGuildMainForm::RequestSearch_Page(int page) {
	if (m_searchMode == 0) {
		m_searchPage = page;

		WSendPacket packet((enumClientPacket)0x105);
		packet.Encode4(page != 0 ? page : 1);
		packet.Send(SEND_TO_DEFAULT);
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	} else if (m_searchMode == 1) {
		m_searchPage = page;

		WSendPacket packet((enumClientPacket)0x106);
		packet.Encode4(page);
		packet.EncodeStr(m_searchText);
		packet.Send(SEND_TO_DEFAULT);
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	}
}

// ingameguildlobbydlg.cpp:490
void CGuildMainForm::RequestChangeClass(GUILD_CLASS_IDX classIndex) {
	if (m_pCommandUserInfo == 0) {
		return;
	}

	WSendPacket packet((enumClientPacket)0x10D);
	packet.Encode4(GuildInfo()->GetMyGuildInfo().guildNumber);
	packet.Encode4(m_pCommandUserInfo->uid);
	packet.Encode4(classIndex);
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildlobbydlg.cpp:506
void CGuildMainForm::ClearForm(void) {
	m_pEdits[0]->ClearLine();
	m_pEdits[1]->ClearLine();
	m_pEdits[2]->ClearLine();
	m_pListBoxes[0]->ClearItem();
	m_memberItems.clear();

	Controller()->ControllForm(FORM_GUILD_INFO, ConvertGuildClass());
	m_formStyle = FORM_GUILD_INFO;

	m_pTabs[0]->SetStatus(FrButton::PRESSED);
	m_pTabs[1]->SetStatus(FrButton::NORMAL);
	m_pTabs[2]->SetStatus(FrButton::NORMAL);
}

// ingameguildlobbydlg.cpp:522
void CGuildMainForm::DisableJoinButton(void) {
	FrButton *pButton = m_pButtons[3];
	if (pButton) {
		pButton->SetVisible(false);
		pButton->Enable(false);
	}
}

// ingameguildlobbydlg.cpp:530
void CGuildMainForm::InitSearchMode(void) {
	m_searchMode = 0;
}

// ingameguildlobbydlg.cpp:535
void CGuildMainForm::LockScrollMemberList(void) {
	if (m_pListBoxes[0]) {
		m_pListBoxes[0]->EnableScrollBar(false);
	}
}

// ingameguildlobbydlg.cpp:543
void CGuildMainForm::UnLockScrollMemberList(void) {
	FrListBox *pList = m_pListBoxes[0];
	if (pList) {
		if (pList->GetSelected()) {
			pList->EnableScrollBar(true);
		}
	}
}

// ingameguildlobbydlg.cpp:555
void CGuildMainForm::InitControl(void) {
	memset(m_pTabs, 0, sizeof(m_pTabs)); memset(m_pAreas, 0, sizeof(m_pAreas));
	memset(m_pEdits, 0, sizeof(m_pEdits));
	memset(m_pButtons, 0, sizeof(m_pButtons));
	memset(m_pListBoxes, 0, sizeof(m_pListBoxes));
	memset(m_pBitmaps, 0, sizeof(m_pBitmaps));
	memset(&m_joinGuild, 0, sizeof(m_joinGuild));

	m_pSelectedMember = 0;
	m_pController = _util::NewCreator<CGuildFormController>::CreateObject();
	m_pSearchArrow = _util::NewCreator<CPageArrow>::CreateObject();

	m_pPageFont = 0;
	m_pCurrentPageFont = 0;
	m_pLinkGuildInfo = 0;

	m_formStyle = (eFormStyle)0;

	m_talkTextX = 0;
	m_talkIconX = 0;
	m_pCommandUserInfo = 0;

	m_memberPage = 1;
	m_searchPage = 1;

	InitSearchMode();
}

// ingameguildlobbydlg.cpp:585
void CGuildMainForm::LoadResource(void) {
	const char *names[17] = {
		"guild_con_member_bg01",
		"guild_con_member_bg02",
		"guild_con_member_bg03",
		"guild_con_member_bg04",
		"guild_icon_manager",
		"guild_icon_assistant",
		"guild_icon_member",
		"guild_icon_waiting",
		"guild_not_join_bg1",
		"guild_not_join_bg2",
		"guild_wating_bg",
		"guild_member_on",
		"guild_member_off",
		"guild_select_line_o",
		"guild_select_line_d",
		"guild_btn_modification_n",
		"guild_btn_ok_n"
	};

	for (int i = 0; i < 17; i++) {
		m_pBitmaps[i] = g_pFresh->RegisterBitmap(names[i]);
	}
}

// ingameguildlobbydlg.cpp:618
void CGuildMainForm::CreateAuthController(void) {
	m_authControllers.insert(std::make_pair((GUILD_CLASS_IDX)1, (IGuildAuthController *)_util::NewCreator<CGuildMaster>::CreateObject()));
	m_authControllers.insert(std::make_pair((GUILD_CLASS_IDX)2, (IGuildAuthController *)_util::NewCreator<CGuildSubMaster>::CreateObject()));
	m_authControllers.insert(std::make_pair((GUILD_CLASS_IDX)3, (IGuildAuthController *)_util::NewCreator<CGuildMember>::CreateObject()));
	m_authControllers.insert(std::make_pair((GUILD_CLASS_IDX)9, (IGuildAuthController *)_util::NewCreator<CGuildWaitMember>::CreateObject()));
	m_authControllers.insert(std::make_pair((GUILD_CLASS_IDX)0, (IGuildAuthController *)_util::NewCreator<CGuildNull>::CreateObject()));

	// SetLinkDepends is static: the map lookup only creates the master's slot.
	m_authControllers[GUILD_CLASS_MASTER]->SetLinkDepends(this, m_pLinkGuildInfo);
}

// ingameguildlobbydlg.cpp:630
void CGuildMainForm::PopupMemberCommand(float x, float y) {
	if (m_pSelectedMember == 0) {
		return;
	}

	sMemberItem *pItem = (sMemberItem *)m_pSelectedMember->pData;

	ClearCommandUserInfo();
	SetCommandUserInfo(pItem->pUserInfo);
	m_authControllers[MyGuildClassIndex()]->PopupCommandMenu(x, y);
	EnableAuthCommand(MyGuildClassIndex());
}

// ingameguildlobbydlg.cpp:645
void CGuildMainForm::SelectMemberItem(FrListBox *pList, FrListItem *pItem) {
	if (m_pSelectedMember != 0) {
		// A new selection closes the talk edit opened on the previous row.
		if (((sMemberItem *)m_pSelectedMember->pData)->bEditing == true) {
			m_pEdits[4]->ClearLine();
			FrWnd *pEdit = m_pEdits[4]; pEdit->SetVisible(false); pEdit->Enable(false);
			FrWnd *pButton = m_pButtons[8]; pButton->SetVisible(false); pButton->Enable(false);
			pList->OrderItem(_util::MakeMemFun1(*this, &CGuildMainForm::ResetEditState));
		}
	}

	pList->UnselectItem(m_pSelectedMember);
	pList->SelectItem(pItem, true);
	m_pSelectedMember = pItem;
}

// ingameguildlobbydlg.cpp:663
void CGuildMainForm::ProcManagerCommand(const char *pCommand) {
	if (strlen(pCommand) < 1) {
		return;
	}

	m_authControllers[MyGuildClassIndex()]->ProcManagerCommand(pCommand);
}

// ingameguildlobbydlg.cpp:676
void CGuildMainForm::CheckTalkState(void) {
	m_pButtons[8]->SetVisible(true); m_pButtons[8]->Enable(true);
	m_pEdits[4]->SetVisible(true); m_pEdits[4]->Enable(true);

	if (m_pSelectedMember == 0) {
		m_pSelectedMember = m_pListBoxes[0]->GetSelected();
		if (m_pSelectedMember == 0) {
			return;
		}
	}

	FrListItem *pItem = m_pSelectedMember;

	// The talk edit and button sit on the selected member's row.
	WRect rect(0.0f, pItem->pos.y + 5.0f, 0, m_pListBoxes[0]->GetItemHeight());

	rect.x = m_talkTextX + pItem->pos.x; rect.w = m_pEdits[4]->GetRect().w;
	m_pEdits[4]->SetRect(rect);

	rect.x = m_talkIconX + pItem->pos.x;
	rect.w = m_pButtons[8]->GetRect().w;
	rect.h = m_pButtons[8]->GetRect().h;
	m_pButtons[8]->SetRect(rect);

	sMemberItem *pMember = (sMemberItem *)m_pSelectedMember->pData;

	int length = strlen(pMember->pUserInfo->text);
	if (length > 0) {
		if (length > 25) {
			return;
		}

		m_pEdits[4]->ClearLine();
		m_pEdits[4]->ForcedEdit(pMember->pUserInfo->text, 0);
	}

	LockScrollMemberList();
}

// ingameguildlobbydlg.cpp:728
void CGuildMainForm::EnableAuthCommand(GUILD_CLASS_IDX classIndex) {
	int targetClass = ((sMemberItem *)m_pSelectedMember->pData)->pUserInfo->m_classIdx;

	switch (classIndex) {
	case 1:
		if (targetClass == 1) {
			break;
		}

		switch (targetClass) {
		case 2:
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x439));
			break;

		case 3:
		case 9:
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x43A));
			break;
		}
		break;

	case 2:
		switch (targetClass) {
		case 1:
		case 2:
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x439));
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x43A));
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x43B));
			break;

		case 3:
		case 9:
			((FrContextMenuCtrl *)m_pManagerCmd)->UnableMenuItem(GuildInfo()->GuildMsg(0x43A));
			break;
		}
		break;
	}
}

// ingameguildlobbydlg.cpp:785
void CGuildMainForm::ResetEditState(void *pData) {
	sMemberItem *pItem = (sMemberItem *)pData;

	pItem->bEditing = false;
}

// ingameguildlobbydlg.cpp:792
void CGuildMainForm::UnSelectMember(void) {
	FrListItem *pItem = m_pListBoxes[0]->GetSelected();
	if (pItem == 0) {
		return;
	}

	((sMemberItem *)pItem->pData)->bEditing = false;
	UnLockScrollMemberList();
	m_pListBoxes[0]->UnselectItem(pItem);
}

// ingameguildlobbydlg.cpp:806
eAuthority CGuildMainForm::ConvertGuildClass(void) {
	eAuthority authority = AUTHORITY_NONE;

	switch (GuildInfo()->GetMyGuildInfo().classIndex) {
	case 0: authority = AUTHORITY_CLASS0; break;
	case 1: authority = AUTHORITY_MASTER; break;
	case 2: authority = AUTHORITY_CLASS2; break;
	case 3: authority = AUTHORITY_CLASS3; break;
	case 9: authority = AUTHORITY_CLASS9; break;
	}

	return authority;
}

// ingameguildlobbydlg.cpp:824
bool CGuildMainForm::CanExecuteClass(const GUILD_USER_LIST *pUserInfo) {
	switch (MyGuildClassIndex()) {
	case 1:
	case 2:
		return true;
	case 3:
		if (pUserInfo->uid == MyUID()) {
			return true;
		}
		break;
	}

	return false;
}

// ingameguildlobbydlg.cpp:847
const char *CGuildMainForm::GuildMsg(int index) {
	return m_pLinkGuildInfo->GuildMsg(index);
}

// ingameguildlobbydlg.cpp:852
GUILD_CLASS_IDX CGuildMainForm::MyGuildClassIndex(void) {
	return m_pLinkGuildInfo->GetClassIndex();
}

// ingameguildlobbydlg.cpp:858
void CGuildMainForm::OnGuildInfo_ButtonInit(int param) {
	m_pTabs[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:863
void CGuildMainForm::OnGuildInfo_LButtonUp(void) {
	if (m_formStyle == FORM_GUILD_INFO) {
		return;
	}

	m_pEdits[0]->ClearLine();
	m_pEdits[0]->AddText(GuildInfo()->GetMyGuildInfo().introduce, false, true);

	m_pEdits[0]->GetScrollBar()->ScrollToFirst();

	Controller()->ControllForm(FORM_GUILD_INFO, ConvertGuildClass());
	m_formStyle = FORM_GUILD_INFO;

	if (m_pTabs[1]->GetStatus() == FrButton::PRESSED) {
		UnSelectMember();
	}

	m_pTabs[0]->SetStatus(FrButton::PRESSED);
	m_pTabs[1]->SetStatus(FrButton::NORMAL);
	m_pTabs[2]->SetStatus(FrButton::NORMAL);
}

// ingameguildlobbydlg.cpp:892
void CGuildMainForm::OnGuildMember_ButtonInit(int param) {
	m_pTabs[1] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:897
void CGuildMainForm::OnGuildMember_LButtonUp(void) {
	if (m_formStyle == FORM_GUILD_MEMBER) {
		return;
	}

	Controller()->ControllForm(FORM_GUILD_MEMBER, ConvertGuildClass());
	m_formStyle = FORM_GUILD_MEMBER;

	m_pTabs[0]->SetStatus(FrButton::NORMAL);
	m_pTabs[1]->SetStatus(FrButton::PRESSED);
	m_pTabs[2]->SetStatus(FrButton::NORMAL);
}

// ingameguildlobbydlg.cpp:912
void CGuildMainForm::OnGuildSearch_ButtonInit(int param) {
	m_pTabs[2] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:917
void CGuildMainForm::OnGuildSearch_LButtonUp(void) {
	if (m_formStyle == FORM_GUILD_SEARCH) {
		return;
	}

	Controller()->ControllForm(FORM_GUILD_SEARCH, ConvertGuildClass());
	m_formStyle = FORM_GUILD_SEARCH;

	int maxPage = GuildInfo()->GetMaxSearchPage();
	m_pSearchArrow->RefreshArrowButton(maxPage, m_searchPage);

	if (m_pTabs[1]->GetStatus() == FrButton::PRESSED) {
		UnSelectMember();
	}

	m_pTabs[0]->SetStatus(FrButton::NORMAL);
	m_pTabs[1]->SetStatus(FrButton::NORMAL);
	m_pTabs[2]->SetStatus(FrButton::PRESSED);
}

// ingameguildlobbydlg.cpp:941
void CGuildMainForm::OnGuildDetail_AreaInit(int param) {
	m_pAreas[0] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[0]);
	Controller()->InsertPostAction(m_pAreas[0], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pAreas[0], FORM_GUILD_INFO, AUTHORITY_ALL, &CGuildFormController::MoveLeftSide);
	Controller()->InsertPostAction(m_pAreas[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pAreas[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::MoveRightSide);
}

// ingameguildlobbydlg.cpp:1034
void CGuildMainForm::OnGuildIntroduce_AreaInit(int param) {
	m_pAreas[1] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[1]);
	Controller()->InsertPostAction(m_pAreas[1], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pAreas[1], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::MoveLeftSide);
	Controller()->InsertPostAction(m_pAreas[1], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pAreas[1], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::MoveRightSide);
}

// ingameguildlobbydlg.cpp:1044
void CGuildMainForm::OnGuildNotice_AreaInit(int param) {
	m_pAreas[2] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[2]);
	Controller()->InsertPostAction(m_pAreas[2], FORM_GUILD_INFO, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1051
void CGuildMainForm::OnGuildAct_AreaInit(int param) {
	m_pAreas[3] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[3]);
	Controller()->InsertPostAction(m_pAreas[3], FORM_GUILD_INFO, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1058
void CGuildMainForm::OnEmptyGuild_AreaInit(int param) {
	m_pAreas[4] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[4]);
	Controller()->InsertPostAction(m_pAreas[4], FORM_GUILD_INFO, AUTHORITY_CLASS0 | AUTHORITY_CLASS9, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1065
void CGuildMainForm::OnEmptyGuild_AreaOwnerDraw(int param) {
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();

	// TODO: contentsdoc.h:360 CGuildInfo::IsWaitingJoin() (classIndex == 9) once the header has it.
	if (GuildInfo()->GetClassIndex() == 9) {
		// Waiting for the join request to be accepted: the guild's name.
		const WRect &rect = m_pAreas[4]->GetRect();

		pGDI->DrawTexture(m_pBitmaps[10], WRect(rect.x + 15.0f, rect.y + 57.0f, (float)m_pBitmaps[10]->Width(), (float)m_pBitmaps[10]->Height()), 0xFFFFFFFF, 0);

		pGDI->SetTextStyle(1);
		pGDI->SetTextColor(0xFF33365F, 0xFFFFFFFF);

		pGDI->Print(WPoint(rect.x + 107.0f, rect.y + 89.0f), 1, GuildInfo()->GetMyGuildInfo().name);

		pGDI->SetTextStyle(0);
		pGDI->SetTextColor(0, 0xFFFFFFFF);
	} else if (!GuildInfo()->IsGuildMember()) {
		const WRect &rect = m_pAreas[4]->GetRect();

		pGDI->DrawTexture(m_pBitmaps[8], WRect(rect.x, rect.y, (float)m_pBitmaps[8]->Width(), (float)m_pBitmaps[8]->Height()), 0xFFFFFFFF, 0);
		pGDI->DrawTexture(m_pBitmaps[9], WRect(rect.x, rect.y + 256.0f, (float)m_pBitmaps[8]->Width(), (float)m_pBitmaps[9]->Height()), 0xFFFFFFFF, 0);
	}
}

// ingameguildlobbydlg.cpp:1100
void CGuildMainForm::OnGuildMemberInfoBg_AreaInit(int param) {
	m_pAreas[5] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[5]);
	Controller()->InsertPostAction(m_pAreas[5], FORM_GUILD_MEMBER, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1107
void CGuildMainForm::OnGuildMemberInfoBg_OwnerDraw(int param) {
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();

	// The four corners of the member information background.
	pGDI->DrawTexture(m_pBitmaps[0], WRect(m_rect.x + 68.0f, m_rect.y + 83.0f, (float)m_pBitmaps[0]->Width(), (float)m_pBitmaps[0]->Height()), 0xFFFFFFFF, 0);
	pGDI->DrawTexture(m_pBitmaps[1], WRect(m_rect.x + 324.0f, m_rect.y + 83.0f, (float)m_pBitmaps[1]->Width(), (float)m_pBitmaps[1]->Height()), 0xFFFFFFFF, 0);
	pGDI->DrawTexture(m_pBitmaps[2], WRect(m_rect.x + 68.0f, m_rect.y + 339.0f, (float)m_pBitmaps[2]->Width(), (float)m_pBitmaps[2]->Height()), 0xFFFFFFFF, 0);
	pGDI->DrawTexture(m_pBitmaps[3], WRect(m_rect.x + 324.0f, m_rect.y + 339.0f, (float)m_pBitmaps[3]->Width(), (float)m_pBitmaps[3]->Height()), 0xFFFFFFFF, 0);
}

// ingameguildlobbydlg.cpp:1120
void CGuildMainForm::OnSearchBg1_AreaInit(int param) {
	m_pAreas[6] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[6]);
	Controller()->InsertPostAction(m_pAreas[6], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1127
void CGuildMainForm::OnSearchBg2_AreaInit(int param) {
	m_pAreas[7] = DYNAMIC_CAST(FrArea, param);
	Controller()->InsertControl(m_pAreas[7]);
	Controller()->InsertPostAction(m_pAreas[7], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1136
void CGuildMainForm::OnGuildIntroduce_EditInit(int param) {
	m_pEdits[0] = DYNAMIC_CAST(FrEdit, param);
	Controller()->InsertControl(m_pEdits[0]);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::MoveLeftSide);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2, &CGuildFormController::WritableWindow);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_INFO, AUTHORITY_CLASS3, &CGuildFormController::ReadOnlywindow);

	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::MoveRightSide);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::ClearEdit);
	Controller()->InsertPostAction(m_pEdits[0], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::ReadOnlywindow);
}

// ingameguildlobbydlg.cpp:1151
void CGuildMainForm::OnGuildNotice_EditInit(int param) {
	m_pEdits[1] = DYNAMIC_CAST(FrEdit, param);
	Controller()->InsertControl(m_pEdits[1]);
	Controller()->InsertPostAction(m_pEdits[1], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pEdits[1], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2, &CGuildFormController::WritableWindow);
	Controller()->InsertPostAction(m_pEdits[1], FORM_GUILD_INFO, AUTHORITY_CLASS0 | AUTHORITY_CLASS3 | AUTHORITY_CLASS9, &CGuildFormController::ReadOnlywindow);
}

// ingameguildlobbydlg.cpp:1160
void CGuildMainForm::OnGuildAct_EditInit(int param) {
	m_pEdits[2] = DYNAMIC_CAST(FrEdit, param);
	Controller()->InsertControl(m_pEdits[2]);
	Controller()->InsertPostAction(m_pEdits[2], FORM_GUILD_INFO, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
	Controller()->InsertPostAction(m_pEdits[2], FORM_GUILD_INFO, AUTHORITY_ALL, &CGuildFormController::ReadOnlywindow);
}

// ingameguildlobbydlg.cpp:1168
void CGuildMainForm::OnGuildSearch_EditInit(int param) {
	m_pEdits[3] = DYNAMIC_CAST(FrEdit, param);
	Controller()->InsertControl(m_pEdits[3]);
	Controller()->InsertPostAction(m_pEdits[3], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1175
void CGuildMainForm::OnGuildSearch_EnterKey(int param) {
	OnGuildSearchInfo_LButtonUp();
}

// ingameguildlobbydlg.cpp:1180
void CGuildMainForm::OnGuildMemberTalk_EditInit(int param) {
	m_pEdits[4] = DYNAMIC_CAST(FrEdit, param);
	Controller()->InsertControl(m_pEdits[4]);
}

// ingameguildlobbydlg.cpp:1186
void CGuildMainForm::OnGuildForm_CloseInit(int param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:1191
void CGuildMainForm::OnGuildForm_CloseLButtonUp(void) {
	Close(true);
}

// ingameguildlobbydlg.cpp:1196
void CGuildMainForm::OnGuildIntroConfirm_ButtonInit(int param) {
	m_pButtons[1] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[1]);
	Controller()->InsertPostAction(m_pButtons[1], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1203
void CGuildMainForm::OnGuildIntroConfirm_LButtonUp(void) {
	m_authControllers[MyGuildClassIndex()]->OnGuildIntroConfirm_LButtonUp();
}

// ingameguildlobbydlg.cpp:1208
void CGuildMainForm::OnGuildNoticeConfirm_ButtonInit(int param) {
	m_pButtons[2] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[2]);
	Controller()->InsertPostAction(m_pButtons[2], FORM_GUILD_INFO, AUTHORITY_MASTER | AUTHORITY_CLASS2, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1215
void CGuildMainForm::OnGuildNoticeConfirm_LButtonUp(void) {
	m_authControllers[MyGuildClassIndex()]->OnGuildNoticeConfirm_LButtonUp();
}

// ingameguildlobbydlg.cpp:1220
void CGuildMainForm::OnGuildJoin_ButtonInit(int param) {
	m_pButtons[3] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[3]);
	Controller()->InsertPostAction(m_pButtons[3], FORM_GUILD_SEARCH, AUTHORITY_CLASS0, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1227
void CGuildMainForm::OnGuildJoin_LButtonUp(void) {
	if (m_joinGuild.guildNumber == 0) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)GuildInfo()->GuildMsg(0x40E), 0, 0, 0, 0));
		return;
	}

	m_authControllers[MyGuildClassIndex()]->OnGuildJoin_LButtonUp();
}

// ingameguildlobbydlg.cpp:1238
void CGuildMainForm::OnGuildSearchInfo_ButtonInit(int param) {
	m_pButtons[4] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[4]);
	Controller()->InsertPostAction(m_pButtons[4], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1245
void CGuildMainForm::OnGuildSearchInfo_LButtonUp(void) {
	m_searchText = m_pEdits[3]->GetLine(1, false);
	if (m_searchText.empty()) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)GuildInfo()->GuildMsg(0x40D), 0, 0, 0, 0));
		return;
	}

	if (m_searchText.length() < 4) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)MakeStr("\\c0xffff0000\\c\xBF\xB5\xB9\xAE %d\xB1\xDB\xC0\xDA\\c0xff000000\\c \xB6\xC7\xB4\xC2 \\c0xffff0000\\c\xC7\xD1\xB1\xDB %d\xB1\xDB\xC0\xDA\\c0xff000000\\c\xC0\xCC\xBB\xF3 \xC0\xD4\xB7\xC2\xC7\xCF\xBF\xA9\xBE\xDF \xC7\xD5\xB4\xCF\xB4\xD9.", 4, 2), 0, 0, 0, 0));
		return;
	}

	if (m_searchText.find("%%") == std::string::npos) {
		m_searchMode = 1; RequestSearch_Page(1);
	}
}

// ingameguildlobbydlg.cpp:1278
void CGuildMainForm::OnGuildJoinAbort_ButtonInit(int param) {
	m_pButtons[6] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[6]);
	Controller()->InsertPostAction(m_pButtons[6], FORM_GUILD_INFO | FORM_GUILD_MEMBER | FORM_GUILD_SEARCH, AUTHORITY_CLASS9, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1285
void CGuildMainForm::OnGuildJoinAbort_LButtonUp(void) {
	m_authControllers[MyGuildClassIndex()]->OnGuildJoinAbort_LButtonUp();
}

// ingameguildlobbydlg.cpp:1290
void CGuildMainForm::OnGuildMemberTalk_ButtonInit(int param) {
	m_pButtons[8] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[8]);
}

// ingameguildlobbydlg.cpp:1344
void CGuildMainForm::OnGuildSearchPrev_ButtonInit(int param) {
	m_pButtons[9] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[9]);
	Controller()->InsertPostAction(m_pButtons[9], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::VisibleWindow);

	m_pSearchArrow->SetLeftArrow(m_pButtons[9]);
}

// ingameguildlobbydlg.cpp:1353
void CGuildMainForm::OnGuildSearchPrev_LButtonUp(void) {
	unsigned int group = (m_searchPage - 1) / 5;
	int page = group * 5;
	RequestSearch_Page(page);
}

// ingameguildlobbydlg.cpp:1360
void CGuildMainForm::OnGuildSearchNext_ButtonInit(int param) {
	m_pButtons[10] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[10]);
	Controller()->InsertPostAction(m_pButtons[10], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::VisibleWindow);

	m_pSearchArrow->SetRightArrow(m_pButtons[10]);
}

// ingameguildlobbydlg.cpp:1369
void CGuildMainForm::OnGuildSearchNext_LButtonUp(void) {
	unsigned int group = (m_searchPage - 1) / 5;
	int page = group * 5 + 6;
	RequestSearch_Page(page);
}

// ingameguildlobbydlg.cpp:1376
void CGuildMainForm::OnGuildShutDown_ButtonInit(int param) {
	m_pButtons[5] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[5]);
	Controller()->InsertPostAction(m_pButtons[5], FORM_GUILD_INFO | FORM_GUILD_MEMBER | FORM_GUILD_SEARCH, AUTHORITY_MASTER, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1383
void CGuildMainForm::OnGuildShutDown_LButtonUp(void) {
	m_authControllers[MyGuildClassIndex()]->OnGuildShutDown_LButtonUp();
}

// ingameguildlobbydlg.cpp:1388
void CGuildMainForm::OnGuildOut_ButtonInit(int param) {
	m_pButtons[7] = DYNAMIC_CAST(FrButton, param);
	Controller()->InsertControl(m_pButtons[7]);
	Controller()->InsertPostAction(m_pButtons[7], FORM_GUILD_INFO | FORM_GUILD_MEMBER | FORM_GUILD_SEARCH, AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1395
void CGuildMainForm::OnGuildOut_LButtonUp(void) {
	m_authControllers[MyGuildClassIndex()]->OnGuildOut_LButtonUp();
}

// ingameguildlobbydlg.cpp:1400
void CGuildMainForm::OnGuildMemberInfo_ListBoxInit(int param) {
	m_pListBoxes[0] = DYNAMIC_CAST(FrListBox, param);
	Controller()->InsertControl(m_pListBoxes[0]);
	Controller()->InsertPostAction(m_pListBoxes[0], FORM_GUILD_MEMBER, AUTHORITY_MASTER | AUTHORITY_CLASS2 | AUTHORITY_CLASS3, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1481
void CGuildMainForm::OnGuildMemberInfo_LButtonDown(void) {
	FrListBox *pList = m_pListBoxes[0];

	FrListItem *pItem = pList->GetItemUnderCursor();
	if (pItem == 0) {
		// A click below the last member closes the command menu.
		if (m_pManagerCmd) {
			m_pManagerCmd->SetVisible(false);
		}
		return;
	}

	SelectMemberItem(pList, pItem);

	// The left half of the row opens the command menu at the mouse.
	WRect rect(pItem->pos, 0, 0);
	rect.w = pList->GetRect().w * 0.5f; rect.h = (float)pList->GetItemHeight();
	if (CMouseCursor::Instance()->InArea(rect)) {
		WVector pt = g_input->GetMousePoint();
		PopupMemberCommand(pt.x, pt.y);
		return;
	}

	// The talk icon opens the talk edit on the row.
	rect.x = m_talkIconX + pItem->pos.x;
	if (CMouseCursor::Instance()->InArea(rect)) {
		sMemberItem *pMember = (sMemberItem *)pItem->pData;
		if (CanExecuteClass(pMember->pUserInfo) && pMember->bEditing == false) {
			pMember->bEditing = true;
			CheckTalkState();
			return;
		}

		if (m_pManagerCmd->IsVisible()) {
			m_pManagerCmd->SetVisible(false);
		}
		return;
	}

	m_pManagerCmd->SetVisible(false);
}

// ingameguildlobbydlg.cpp:1537
void CGuildMainForm::OnMemberPage_ListBoxInit(int param) {
	m_pListBoxes[2] = DYNAMIC_CAST(FrListBox, param);
	Controller()->InsertControl(m_pListBoxes[2]);
	Controller()->InsertPostAction(m_pListBoxes[2], FORM_GUILD_MEMBER, AUTHORITY_ALL, &CGuildFormController::EnableWindow);

	m_pListBoxes[2]->EnableScrollBar(false);
}

// ingameguildlobbydlg.cpp:1546
void CGuildMainForm::OnMemberPage_ListOwnerDraw(int param) {
	FrListItem *pItem = (FrListItem *)param;
	if (pItem == 0) {
		return;
	}

	static char szPage[8];
	itoa(pItem->idx + 1, szPage, 10);

	int page = pItem->idx + 1;

	if (m_memberPage == page) {
		m_pCurrentPageFont->Print(g_view, pItem->pos.x, pItem->pos.y, szPage, 0, 0xFFFFFFFF, 0);
	} else {
		m_pPageFont->Print(g_view, pItem->pos.x, pItem->pos.y, szPage, 0, 0xFFFFFFFF, 0);
	}
}

// ingameguildlobbydlg.cpp:1568
void CGuildMainForm::OnMemberPage_ListLButtonDown(void) {
	FrListItem *pItem = m_pListBoxes[2]->GetSelected();
	if (pItem == 0) {
		return;
	}

	int page = pItem->idx + 1;
	if (page == m_memberPage) {
		return;
	}

	m_memberPage = page;
	RequestMember_Page(page);
}

// ingameguildlobbydlg.cpp:1588
void CGuildMainForm::OnGuildSearchInfo_ListBoxInit(int param) {
	m_pListBoxes[1] = DYNAMIC_CAST(FrListBox, param);
	Controller()->InsertControl(m_pListBoxes[1]);
	Controller()->InsertPostAction(m_pListBoxes[1], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1595
void CGuildMainForm::OnGuildSearchInfo_OwnerDraw(int param) {
	FrListItem *pItem = (FrListItem *)param;
	if (pItem == 0) {
		return;
	}

	const GUILD_LIST *pGuild = (const GUILD_LIST *)pItem->pData;
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();

	if (pItem->selected == true) {
		pGDI->DrawTexture(m_pBitmaps[14], WRect(pItem->pos, m_pListBoxes[1]->GetItemWidth(), m_pListBoxes[1]->GetItemHeight()), 0xFFFFFFFF, 0);
	}

	pGDI->SetTextStyle(1);
	pGDI->SetTextColor(0xFF333333, 0xFFFFFFFF);

	pGDI->Print(WPoint(pItem->pos.x + 38.0f, pItem->pos.y + 10.0f), 0, pGuild->name);

	pGDI->SetTextStyle(0);
	pGDI->SetTextColor(0, 0xFFFFFFFF);
}

// ingameguildlobbydlg.cpp:1635
void CGuildMainForm::OnGuildSearchInfo_LButtonDown(void) {
	FrListItem *pItem = m_pListBoxes[1]->GetSelected();

	if (pItem == 0) {
		memset(&m_joinGuild, 0, sizeof(m_joinGuild));
		m_pEdits[0]->ClearLine();
		return;
	}

	const GUILD_LIST *pGuild = (const GUILD_LIST *)pItem->pData;
	if (m_joinGuild.guildNumber != pGuild->guildNumber) {
		m_joinGuild = *pGuild;

		m_pEdits[0]->ClearLine();
		m_pEdits[0]->AddText(m_joinGuild.intro, false, true);

		m_pEdits[0]->GetScrollBar()->ScrollToFirst();
	}
}

// ingameguildlobbydlg.cpp:1659
void CGuildMainForm::OnGuildSearchPage_ListBoxInit(int param) {
	m_pListBoxes[3] = DYNAMIC_CAST(FrListBox, param);
	Controller()->InsertControl(m_pListBoxes[3]);
	Controller()->InsertPostAction(m_pListBoxes[3], FORM_GUILD_SEARCH, AUTHORITY_ALL, &CGuildFormController::EnableWindow);
}

// ingameguildlobbydlg.cpp:1666
void CGuildMainForm::OnGuildSearchPage_OwnerDraw(int param) {
	FrListItem *pItem = (FrListItem *)param;
	if (pItem == 0) {
		return;
	}

	static char szPage[8];
	memset(szPage, 0, sizeof(szPage));

	// The page list keeps each page's index as its item data.
	unsigned int page = (intptr_t)pItem->pData + 1;
	itoa(page, szPage, 10);

	if (m_searchPage == page) {
		m_pCurrentPageFont->Print(g_view, pItem->pos.x, pItem->pos.y, szPage, 0, 0xFFFFFFFF, 0);
	} else {
		m_pPageFont->Print(g_view, pItem->pos.x, pItem->pos.y, szPage, 0, 0xFFFFFFFF, 0);
	}
}

// ingameguildlobbydlg.cpp:1692
void CGuildMainForm::OnGuildSearchPage_LButtonDown(void) {
	FrListItem *pItem = m_pListBoxes[3]->GetSelected();
	if (pItem == 0) {
		return;
	}

	// The page list keeps each page's index as its item data.
	int page = (intptr_t)pItem->pData + 1;
	if (page == m_searchPage) {
		return;
	}

	m_searchPage = page;
	RequestSearch_Page(page);
}

// ingameguildlobbydlg.cpp:1712
void CGuildMainForm::OnGuildManagerCmd_ContextInit(int param) {
	m_pManagerCmd = DYNAMIC_CAST(FrWnd, param);
	if (m_pManagerCmd == 0) {
		return;
	}

	m_pManagerCmd->SetVisible(false);
	m_pManagerCmd->SetFixed(true);
}

// ingameguildlobbydlg.cpp:1726
void CGuildMainForm::OnGuildManagerCmd_LButtonDown(void) {
	const char *pCommand = ((FrContextMenuCtrl *)m_pManagerCmd)->GetSelectMenuText();
	m_pManagerCmd->SetVisible(false);

	ProcManagerCommand(pCommand);
}

// ingameguildlobbydlg.cpp:1736
void CGuildMainForm::OnGuildManagerCmd_LButtonUp(void) {
	m_pManagerCmd->SetVisible(false);
}

// ingameguildlobbydlg.cpp:1743 (run-time type and factory registration of FrGuildJoin)
const WRTTI FrGuildJoin::m_RTTI("FrGuildJoin", &FrForm::m_RTTI);

IObject* FrGuildJoinMakeInstance(void) {
	return new FrGuildJoin;
}

struct __sFrGuildJoin {
	__sFrGuildJoin(void) {
		ObjectFactory().AddObjectFunctor(FrGuildJoinMakeInstance, "FrGuildJoin");
	}
};
static __sFrGuildJoin s_registerFrGuildJoin;

// ingameguildlobbydlg.cpp:1744
const sFRESH_MSGMAP* FrGuildJoin::GetMessageMap(void) const {
	return &FrGuildJoin::_MsgMap;
}

// ingameguildlobbydlg.cpp:1766
FrGuildJoin::FrGuildJoin(void)
	: m_pJoinArea(0),
	  m_pJoinEdit(0),
	  m_pBackground(0),
	  m_guildNumber(0),
	  m_bRequested(false) {
	memset(m_pButtons, 0, sizeof(m_pButtons));
}

// ingameguildlobbydlg.cpp:1771
FrGuildJoin::~FrGuildJoin(void) {
}

// ingameguildlobbydlg.cpp:1776
bool FrGuildJoin::OnInit(void) {
	m_pBackground = g_pFresh->RegisterBitmap("guild_popup_write_bg");

	return FrForm::OnInit();
}

// ingameguildlobbydlg.cpp:1784
void FrGuildJoin::SetGuildNumber(int guildNumber) {
	m_guildNumber = guildNumber;
}

// ingameguildlobbydlg.cpp:1791
void FrGuildJoin::SetSuccessRequest(void) {
	m_bRequested = true;
	m_pJoinEdit->SetReadOnly(true);
	m_pJoinEdit->ClearLine();
	m_pJoinEdit->SetFontColor(0xFF33365F);
	m_pJoinEdit->AddText("\xB0\xA1\xC0\xD4 \xBD\xC5\xC3\xBB\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false, true);

	FrButton *pRequestJoin = m_pButtons[0];
	pRequestJoin->SetVisible(false);
	pRequestJoin->Enable(false);
	FrButton *pRequestOK = m_pButtons[1];
	pRequestOK->SetVisible(true);
	pRequestOK->Enable(true);
}

// ingameguildlobbydlg.cpp:1804
void FrGuildJoin::OnGuildJoin_AreaInit(int param) {
	m_pJoinArea = DYNAMIC_CAST(FrArea, param);
}

// ingameguildlobbydlg.cpp:1809
void FrGuildJoin::OnGuildJoin_AreaOwnerDraw(int param) {
	if (m_bRequested == true) {
		return;
	}

	g_pFresh->GetManager()->GetGDI()->DrawTexture(m_pBackground,
		WRect(m_rect.x + 33.0f, m_rect.y - 20.0f, m_pBackground->Width(), m_pBackground->Height()), 0xFFFFFFFF, 0);
}

// ingameguildlobbydlg.cpp:1823
void FrGuildJoin::OnJoin_EditInit(int param) {
	m_pJoinEdit = DYNAMIC_CAST(FrEdit, param);
}

// ingameguildlobbydlg.cpp:1828
void FrGuildJoin::OnRequestJoin_ButtonInit(int param) {
	m_pButtons[0] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:1833
void FrGuildJoin::OnRequestJoin_LButtonUp(void) {
	if (m_pJoinEdit == 0) {
		return;
	}

	std::string text;

	int i = 1, lineNum = m_pJoinEdit->GetLineNum();
	for (; i <= lineNum; i++) {
		text += m_pJoinEdit->GetLine(i, false);
	}

	if (text.empty()) {
		// The request message is empty.
		AfxGetTask()->GetActor(s_guildActorName) << MsgObject(0, 0x263, 0x1A, 0x3F6, 0, 0, 0);
		return;
	}

	WSendPacket packet(0x109);
	packet.Encode4(m_guildNumber);
	packet.EncodeStr(text);
	packet.Send(SEND_TO_DEFAULT);

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// ingameguildlobbydlg.cpp:1862
void FrGuildJoin::OnRequestOK_ButtonInit(int param) {
	FrButton *pButton = DYNAMIC_CAST(FrButton, param);
	m_pButtons[1] = pButton;
	if (pButton) {
		// Hidden until the join request succeeds (SetSuccessRequest).
		pButton->SetVisible(false);
		pButton->Enable(false);
	}
}

// ingameguildlobbydlg.cpp:1871
void FrGuildJoin::OnRequestOK_LButtonUp(void) {
	Close(FrOK, true);
}

// ingameguildlobbydlg.cpp:1876
void FrGuildJoin::OnFormClose_ButtonInit(int param) {
	m_pButtons[2] = DYNAMIC_CAST(FrButton, param);
}

// ingameguildlobbydlg.cpp:1881
void FrGuildJoin::OnFormClose_LButtonUp(void) {
	Close(FrNO, true);
}

} // namespace _guild
