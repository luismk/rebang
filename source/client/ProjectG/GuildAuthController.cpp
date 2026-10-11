// Original: d:\build\custom\temp\code\client\projectg\guildauthcontroller.cpp
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer

#include "guilddefine.h"

#include "GuildAuthController.h"
#include "ingameguildlobbydlg.h"
#include "Packet.h"
#include "TaskManager.h"
#include "actor.h"
#include "SharedDoc.h"
#include "frwnd.h"
#include "frcontextmenuctrl.h"
#include "frlistbox.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "fredit.h"
#include "commonutil.h"
#include "hatManager.h"
#include "user_info.h"
#include <string>

namespace _guild {

CGuildMainForm* IGuildAuthController::m_linkForm = 0;
CGuildInfo* IGuildAuthController::m_linkInfo = 0;

// Name of the guild actor the controllers message (a pointer in this unit's
// data).
static const char* s_guildActorName = "GuildActor";

// Line separator appended between the lines of the introduction and notice
// edits (a pointer in this unit's data).
static const char* s_newLine = "\n";

// guildauthcontroller.cpp:29
IGuildAuthController::IGuildAuthController(void) {
}

// guildauthcontroller.cpp:34
IGuildAuthController::~IGuildAuthController(void) {
}

// guildauthcontroller.cpp:39
void IGuildAuthController::SetLinkDepends(CGuildMainForm* pForm, CGuildInfo* pInfo) {
	m_linkForm = pForm;
	m_linkInfo = pInfo;
}

// guildauthcontroller.cpp:45
void IGuildAuthController::PopupCommandMenu(float x, float y) {
	m_linkForm->m_pManagerCmd->MoveWindow(WPoint(x, y));
	m_linkForm->m_pManagerCmd->SetVisible(true);
}

// guildauthcontroller.cpp:53
void IGuildAuthController::OnGuildJoin_LButtonUp(void) {
	if (m_linkInfo->IsGuildMember() == true) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x3F5), 0, 0, 0, 0));
	}
}

// guildauthcontroller.cpp:67
void CGuildManager::DrawGuildTalk(FrListItem* pItem) {
	CGuildMainForm::sMemberItem* pMember = (CGuildMainForm::sMemberItem*)pItem->pData;

	if (pMember->bEditing == false) {
		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		float x = m_linkForm->m_talkTextX + pItem->pos.x;

		pGDI->Print(WPoint(x, pItem->pos.y + 10.0f), 0, pMember->pUserInfo->text);

		const Bitmap* pIcon = m_linkForm->m_pBitmaps[15];
		if (pIcon) {
			pGDI->DrawTexture(pIcon, WRect(m_linkForm->m_talkIconX + pItem->pos.x, pItem->pos.y + 5.0f, pIcon->Width(), pIcon->Height()), 0xFFFFFFFF, 0);
		}
	}
}

// guildauthcontroller.cpp:90
void CGuildManager::ProcManagerCommand(const char* command) {
	if (m_linkForm->m_pSelectedMember == 0) {
		return;
	}

	const GUILD_USER_LIST* pUserInfo = m_linkForm->m_pCommandUserInfo;
	if (strcmp("\xbd\xc2\xc0\xce(\xbd\xc2\xb1\xde)", command) == 0) {   // "approve (promote)"
		if (pUserInfo->uid == MyUID()) {
			return;
		}
		if (m_linkInfo->IsGuildMember(pUserInfo->m_classIdx)) {
			if (pUserInfo->m_classIdx == 1 || pUserInfo->m_classIdx == 2) {
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x3F3), 0, 0, 0, 0));
			} else if (pUserInfo->m_classIdx == 3) {
				WSendPacket packet((enumClientPacket)0x10D);
				packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
				packet.Encode4(pUserInfo->uid);
				packet.Encode4(2);
				packet.Send(SEND_TO_DEFAULT);
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
			}
		} else {
			WSendPacket packet((enumClientPacket)0x10B);
			packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
			packet.Encode4(pUserInfo->uid);
			packet.Send(SEND_TO_DEFAULT);
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		}
	} else if (strcmp("\xb0\xad\xb5\xee", command) == 0) {   // "demote"
		if (pUserInfo->uid == MyUID()) {
			return;
		}
		if (m_linkInfo->IsGuildMember(pUserInfo->m_classIdx)) {
			if (pUserInfo->m_classIdx == 2) {
				WSendPacket packet((enumClientPacket)0x10D);
				packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
				packet.Encode4(pUserInfo->uid);
				packet.Encode4(3);
				packet.Send(SEND_TO_DEFAULT);
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
			} else {
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x3F4), 0, 0, 0, 0));
			}
		} else if (pUserInfo->m_classIdx == 9) {
			WSendPacket packet((enumClientPacket)0x10C);
			packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
			packet.Encode4(pUserInfo->uid);
			packet.Send(SEND_TO_DEFAULT);
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		}
	} else if (strcmp("\xb1\xe6\xb5\xe5\xc0\xe5 \xc0\xa7\xc0\xd3", command) == 0) {   // "delegate the master"
		if (pUserInfo->uid == MyUID()) {
			return;
		}
		if (pUserInfo->m_classIdx > 2) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x40C), 0, 0, 0, 0));
		} else if (pUserInfo->m_classIdx == 2) {
			IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
			pActor << MsgObject(0, 0x263, 0x1E, (intptr_t)m_linkInfo->GuildMsg(0x408), 1, 0, 0);
		}
	} else if (strcmp("\xb1\xe6\xb5\xe5\xbf\xf8 \xc3\xdf\xb9\xe6", command) == 0) {   // "expel the member"
		if (pUserInfo->uid == MyUID()) {
			return;
		}
		WSendPacket packet((enumClientPacket)0x111);
		packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
		packet.Encode4(pUserInfo->uid);
		packet.Send(SEND_TO_DEFAULT);
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	} else if (strcmp("\xc1\xa4\xba\xb8\xba\xb8\xb1\xe2", command) == 0) {   // "show info"
		WSingleton<CUserInfo>::Instance()->SetInfo(pUserInfo->uid, 0xFFFFFFFF, false, true, false, true, pUserInfo->nickname);
	}
}

// guildauthcontroller.cpp:198
void CGuildManager::OnGuildIntroConfirm_LButtonUp(void) {
	std::string introduce;
	int lineCount = m_linkForm->m_pEdits[0]->GetLineNum();
	for (int i = 1; i <= lineCount; i++) {
		introduce += m_linkForm->m_pEdits[0]->GetLine(i, false);
		if (i != lineCount) {
			introduce += s_newLine;
		}
	}

	if (introduce == m_linkInfo->GetMyGuildInfo().introduce) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x424), 0, 0, 0, 0));
		return;
	}

	str_Replace(introduce, std::string("'"), std::string("`"));
	int result = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringGuildString(introduce.c_str(), 2, 100, false);
	if (result != 1) {
		if (result > 0) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(result), 0, 0, 0, 0));
		}
		return;
	}

	std::string stored(introduce);
	str_Replace(stored, std::string("`"), std::string("'"));
	m_linkInfo->SetGuildIntroduce(stored);

	WSendPacket packet((enumClientPacket)0x103);
	packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
	packet.Encode4(MyUID());
	packet.EncodeStr(introduce);
	packet.Send(SEND_TO_DEFAULT);
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// guildauthcontroller.cpp:247
void CGuildManager::OnGuildNoticeConfirm_LButtonUp(void) {
	std::string notice;
	int lineCount = m_linkForm->m_pEdits[1]->GetLineNum();
	for (int i = 1; i <= lineCount; i++) {
		notice += m_linkForm->m_pEdits[1]->GetLine(i, false);
		notice += s_newLine;
	}

	if (notice == m_linkInfo->GetMyGuildInfo().notice) {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x424), 0, 0, 0, 0));
		return;
	}

	str_Replace(notice, std::string("'"), std::string("`"));
	int result = WSingleton<CSharedDoc>::Instance()->m_chatManager.FilteringGuildString(notice.c_str(), 2, 100, false);
	if (result != 1) {
		if (result > 0) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(result), 0, 0, 0, 0));
		}
		return;
	}

	std::string stored(notice);
	str_Replace(stored, std::string("`"), std::string("'"));
	m_linkInfo->SetGuildNotice(stored);

	WSendPacket packet((enumClientPacket)0x102);
	packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
	packet.Encode4(MyUID());
	packet.EncodeStr(notice);
	packet.Send(SEND_TO_DEFAULT);
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// guildauthcontroller.cpp:295
void CGuildMaster::PopupCommandMenu(float x, float y) {
	m_linkForm->m_pManagerCmd->MoveWindow(WPoint(x, y)); m_linkForm->m_pManagerCmd->SetVisible(true);
	((FrContextMenuCtrl*)m_linkForm->m_pManagerCmd)->SetMenuList(m_linkInfo->GuildMsg(0x3F0));
}

// guildauthcontroller.cpp:301
void CGuildMaster::OnGuildShutDown_LButtonUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	pActor << MsgObject(0, 0x263, 0x1E, (intptr_t)m_linkInfo->GuildMsg(0x400), 0, 0, 0);
}

// guildauthcontroller.cpp:307
void CGuildMaster::OnGuildOut_LButtonUp(void) {
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x3FE), 0, 0, 0, 0));
}

// guildauthcontroller.cpp:315
void CGuildSubMaster::PopupCommandMenu(float x, float y) {
	m_linkForm->m_pManagerCmd->MoveWindow(WPoint(x, y)); m_linkForm->m_pManagerCmd->SetVisible(true);
	((FrContextMenuCtrl*)m_linkForm->m_pManagerCmd)->SetMenuList(m_linkInfo->GuildMsg(0x3F1));
}

// guildauthcontroller.cpp:322
void CGuildSubMaster::OnGuildOut_LButtonUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	pActor << MsgObject(0, 0x263, 0x1E, (intptr_t)m_linkInfo->GuildMsg(0x417), 2, 0, 0);
}

// guildauthcontroller.cpp:332
void CGuildMember::DrawGuildTalk(FrListItem* pItem) {
	CGuildMainForm::sMemberItem* pMember = (CGuildMainForm::sMemberItem*)pItem->pData;

	if (pMember->bEditing == false) {
		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		float x = m_linkForm->m_talkTextX + pItem->pos.x;

		pGDI->Print(WPoint(x, pItem->pos.y + 10.0f), 0, pMember->pUserInfo->text);

		if (pMember->pUserInfo->uid == MyUID()) {
			const Bitmap* pIcon = m_linkForm->m_pBitmaps[15];
			if (pIcon) {
				pGDI->DrawTexture(pIcon, WRect(m_linkForm->m_talkIconX + pItem->pos.x, pItem->pos.y + 5.0f, pIcon->Width(), pIcon->Height()), 0xFFFFFFFF, 0);
			}
		}
	}
}

// guildauthcontroller.cpp:361
void CGuildMember::PopupCommandMenu(float x, float y) {
	m_linkForm->m_pManagerCmd->MoveWindow(WPoint(x, y)); m_linkForm->m_pManagerCmd->SetVisible(true);
	((FrContextMenuCtrl*)m_linkForm->m_pManagerCmd)->SetMenuList(m_linkInfo->GuildMsg(0x3F2));
}

// guildauthcontroller.cpp:370
void CGuildMember::ProcManagerCommand(const char* command) {
	if (m_linkForm->m_pSelectedMember == 0) {
		return;
	}

	CGuildMainForm::sMemberItem* pMember = (CGuildMainForm::sMemberItem*)m_linkForm->m_pSelectedMember->pData;
	if (strcmp("\xc1\xa4\xba\xb8\xba\xb8\xb1\xe2", command) == 0) {   // "show info"
		WSingleton<CUserInfo>::Instance()->SetInfo(pMember->pUserInfo->uid, 0xFFFFFFFF, false, true, false, true, pMember->pUserInfo->nickname);
	}
}

// guildauthcontroller.cpp:386
void CGuildMember::OnGuildOut_LButtonUp(void) {
	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	pActor << MsgObject(0, 0x263, 0x1E, (intptr_t)m_linkInfo->GuildMsg(0x417), 2, 0, 0);
}

// guildauthcontroller.cpp:394
void CGuildWaitMember::OnGuildJoinAbort_LButtonUp(void) {
	WSendPacket packet((enumClientPacket)0x10A);
	packet.Encode4(m_linkInfo->GetMyGuildInfo().guildNumber);
	packet.Send(SEND_TO_DEFAULT);
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// guildauthcontroller.cpp:402
void CGuildWaitMember::OnGuildOut_LButtonUp(void) {
	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)m_linkInfo->GuildMsg(0x3FE), 0, 0, 0, 0));
}

// guildauthcontroller.cpp:410
void CGuildNull::OnGuildJoin_LButtonUp(void) {
	IGuildAuthController::OnGuildJoin_LButtonUp();

	IActor* pActor = AfxGetTask()->GetActor(s_guildActorName);
	if (pActor) {
		pActor << MsgObject(0, 0x263, 0x1D, m_linkForm->m_joinGuild.guildNumber, 0, 0, 0);
	}
}

} // namespace _guild
