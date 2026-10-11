// Original: d:\build\custom\temp\code\client\projectg\guildactor.cpp
#include "guildactor.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "contentsdoc.h"
#include "ingameguildlobbydlg.h"
#include "ingameguildmyroomdlg.h"
#include "TaskManager.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frform.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "commonutil.h"

#include <string.h>

namespace _guild {

// guildactor.cpp:25 (run-time type and factory registration of CGuildActor)
const WRTTI CGuildActor::m_RTTI("CGuildActor", &IActor::m_RTTI);

IObject* CGuildActorMakeInstance(void) {
	return new CGuildActor;
}

struct __impCGuildActor {
	__impCGuildActor(void) {
		ObjectFactory().AddObjectFunctor(CGuildActorMakeInstance, "CGuildActor");
		ActorFactory().AddActorClass("CGuildActor");
	}
};
static __impCGuildActor s_registerCGuildActor;

// guildactor.cpp:28
CGuildActor::CGuildActor(void) {
	InitControl();
}

// guildactor.cpp:37
CGuildActor::~CGuildActor(void) {
}

// guildactor.cpp:44
void CGuildActor::InitControl(void) {
	m_pCreateGuildKit = 0;
	m_pRegisterGuildMark = 0;
	m_pChangeGuildName = 0;
	m_pGuildJoin = 0;
	m_pMainForm = 0;
	m_pGuildInfo = 0;
}

// guildactor.cpp:74
void CGuildActor::OnInit(void) {
	WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x97, m_pGuildInfo);
}

// guildactor.cpp:112
void CGuildActor::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x263:
		ExecGuildMessage(msg);
		break;
	}
}

// guildactor.cpp:145
void CGuildActor::RequestGuildUserList(int page) {
	if (page < 1) {
		return;
	}

	if (!m_pGuildInfo->IsGuildMember()) {
		return;
	}

	WSendPacket packet((enumClientPacket)0x10F);
	packet.Encode4(m_pGuildInfo->GetMyGuildInfo().guildNumber);
	packet.Encode4(page);
	packet.Send(SEND_TO_DEFAULT);
}

// guildactor.cpp:166
void CGuildActor::ExecGuildMessage(const MsgObject& msg) {
	switch (msg.m_param1) {
	case 1:
		OpenGuileMainForm();
		break;

	case 27:
		OpenGuildItemForm(msg.m_param2);
		break;

	case 29:
		OpenGuildJoinForm(msg.m_param2);
		break;

	case 30:
		OpenGuildQuestionForm((const char*)msg.m_param2, msg.m_param3);
		break;

	case 3: {
		std::string* pName = (std::string*)msg.m_param2;
		if (m_pCreateGuildKit)
			m_pCreateGuildKit->SetConfirmGuildName(pName->c_str());

		if (m_pChangeGuildName)
			m_pChangeGuildName->SetConfirmedName(pName->c_str());
		break;
	}

	case 4: {
		m_pGuildInfo->ClearAllData();

		GUILD_INFO* pInfo = (GUILD_INFO*)msg.m_param2;
		m_pGuildInfo->SetMyGuildInfo(*pInfo);
		ReInitMyGuildInfo_SharedDoc(pInfo);
		if (m_pMainForm) {
			{
				WSendPacket packet((enumClientPacket)0x107);
				packet.Send(SEND_TO_DEFAULT);
			}

			m_pMainForm->Refresh();
			m_pMainForm->RequestMember_Page(0);
		}
		break;
	}

	case 5: {
		GUILD_USER_INFO* pInfo = (GUILD_USER_INFO*)msg.m_param2;

		std::pair<bool, bool> state = CheckMyGuildState(pInfo);
		if (state.first) {
			m_pGuildInfo->ClearAllData();
			WSingleton<CSharedDoc>::Instance()->ClearMyGuildInfo();
			if (m_pMainForm)
				m_pMainForm->ClearForm();
		}

		if (state.second) {
			m_pGuildInfo->ClearMemberPage();
			m_pGuildInfo->SetMyGuildUserInfo(*pInfo);
			ReInitMyGuildInfo_SharedDoc(pInfo);
			if (m_pMainForm) {
				m_pMainForm->Refresh();
				m_pMainForm->RequestMember_Page(0);
			}
		}
		break;
	}

	case 11:
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x401), 0, 0, 0, 0));
		break;

	case 12:
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x3FD), 0, 0, 0, 0));
		break;

	case 6: {
		m_pGuildInfo->ClearAllData();

		GUILD_USER_INFO* pInfo = (GUILD_USER_INFO*)msg.m_param2;

		m_pGuildInfo->SetMyGuildUserInfo(*pInfo);
		ReInitMyGuildInfo_SharedDoc(pInfo);
		break;
	}

	case 7: {
		int allMemberCount = msg.m_param4;
		m_pGuildInfo->SetMemberPage(msg.m_param3, (std::list<GUILD_USER_LIST>*)msg.m_param2);
		m_pGuildInfo->SetAllMemberCount(allMemberCount);
		if (m_pMainForm)
			m_pMainForm->RefreshGuildMember();
		break;
	}

	case 8: {
		std::list<GUILD_LIST>* pList = (std::list<GUILD_LIST>*)msg.m_param2;
		int page = msg.m_param3;
		int allGuildCount = msg.m_param4;

		if (pList->size() == 0) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x40B), 0, 0, 0, 0));
			if (m_pMainForm)
				m_pMainForm->InitSearchMode();
			break;
		}

		m_pGuildInfo->SetSearchList(page, pList);
		m_pGuildInfo->SetAllGuildCount(allGuildCount);
		if (m_pMainForm)
			m_pMainForm->RefreshGuildSearch();
		break;
	}

	case 9:
		m_pGuildInfo->SetHistoryList((std::list<GUILD_HISTORY>*)msg.m_param2);

		if (m_pMainForm)
			m_pMainForm->RefreshMyGuildHistory();
		break;

	case 10:
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x3F8), 0, 0, 0, 0));
		break;

	case 14:
		if (m_pMainForm)
			m_pMainForm->DisableJoinButton();

		if (m_pGuildJoin)
			m_pGuildJoin->SetSuccessRequest();
		break;

	case 15:
		if (m_pMainForm)
			m_pMainForm->RequestMember_Page(0);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x3FB), 0, 0, 0, 0));
		break;

	case 16:
		if (m_pMainForm)
			m_pMainForm->RequestMember_Page(0);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x3FA), 0, 0, 0, 0));
		break;

	case 17: {
		if (!m_pMainForm)
			break;

		if (!m_pGuildJoin)
			break;

		m_pGuildJoin->Close(FrCANCEL, true);

		int index = msg.m_param2;
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(index), 0, 0, 0, 0));
		break;
	}

	case 13:
		if (m_pMainForm)
			m_pMainForm->RequestMember_Page(0);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x3F9), 0, 0, 0, 0));
		break;

	case 18:
		if (m_pMainForm) {
			m_pMainForm->RequestMember_Page(0);
			m_pMainForm->UnLockScrollMemberList();
		}

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0x406), 0, 0, 0, 0));
		break;

	case 19: {
		if (m_pMainForm)
			m_pMainForm->RequestMember_Page(0);

		const char* text;
		switch (msg.m_param2) {
		case 1:
			text = MakeStr(SystemMsg(0x409), SystemMsg(0x435), SystemMsg(0x437));
			break;

		case 2:
			text = MakeStr(SystemMsg(0x40A), SystemMsg(0x436));
			break;

		case 3:
			text = MakeStr(SystemMsg(0x40A), SystemMsg(0x437));
			break;

		default:
			return;
		}

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)text, 0, 0, 0, 0));
		break;
	}

	case 32: {
		int* pGuildNumber = (int*)msg.m_param2;
		if (m_pGuildInfo->IsGuildMember())
			*pGuildNumber = m_pGuildInfo->GetMyGuildInfo().guildNumber;
		else
			*pGuildNumber = 0;
		break;
	}

	case 33: {
		const GUILD_INFO** ppInfo = (const GUILD_INFO**)msg.m_param2;
		*ppInfo = &m_pGuildInfo->GetMyGuildInfo();
		break;
	}

	case 34:
		*(GUILD_CLASS_IDX*)msg.m_param2 = m_pGuildInfo->GetClassIndex();
		break;

	case 21:
		if (m_pRegisterGuildMark)
			m_pRegisterGuildMark->RequestUpload(msg.m_param2, *(const std::string*)msg.m_param3);
		break;

	case 22:
		ChangeGuildEmblem((GUILD_USER_INFO*)msg.m_param2);
		break;

	case 23:
		ChangeGuildName((GUILD_INFO*)msg.m_param2);
		break;

	case 24: {
		WSendPacket packet(0x2A);
		packet.Encode4(m_pGuildInfo->GetGuildNumber());
		packet.Send(SEND_TO_MSN);
		break;
	}

	case 25: {
		WSendPacket packet(0x2B);
		packet.Encode4(m_pGuildInfo->GetGuildNumber());
		packet.Send(SEND_TO_MSN);
		break;
	}

	case 26: {
		int index = msg.m_param2;

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(index), 0, 0, 0, 0));
		break;
	}
	}
}

// guildactor.cpp:561
void CGuildActor::OpenGuileMainForm(void) {
	if (m_pMainForm)
		return;

	m_pMainForm = CreateForm<CGuildMainForm>(g_pFresh->GetManager(), this, "guildmain", 0);

	m_pMainForm->EnableDrag(false);
	m_pMainForm->SetLinkGuildInfo(m_pGuildInfo);
	m_pMainForm->Open((FRESH_RESULT_CALLBACK)&CGuildActor::OnMainFormCloseResult, 1);
}

// guildactor.cpp:574
void CGuildActor::OpenGuildItemForm(unsigned long itemTypeId) {
	switch (itemTypeId) {
	case 0x1A00012F:
		if (m_pCreateGuildKit)
			return;

		m_pCreateGuildKit = CreateForm<FrCreateGuildKit>(g_pFresh->GetManager(), this, "create_guild", 0);

		m_pCreateGuildKit->EnableDrag(false);
		m_pCreateGuildKit->SetCaption(SystemMsg(0x3ED));
		m_pCreateGuildKit->Open((FRESH_RESULT_CALLBACK)&CGuildActor::OnCreateGuildFormResult, 1);
		break;

	case 0x1A000130:
		if (m_pRegisterGuildMark)
			return;

		if (!m_pGuildInfo->IsManager()) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0xD4EB), 0, 0, 0, 0));
			return;
		}

		m_pRegisterGuildMark = CreateForm<FrRegisterGuildMark>(g_pFresh->GetManager(), this, "register_guild_mark", 0);

		m_pRegisterGuildMark->EnableDrag(false);
		m_pRegisterGuildMark->SetCaption(SystemMsg(0x3EE));
		m_pRegisterGuildMark->Open((FRESH_RESULT_CALLBACK)&CGuildActor::OnRegisterGuildMarkResult, 1);
		break;

	case 0x1A000131:
		if (m_pChangeGuildName)
			return;

		if (!m_pGuildInfo->IsManager()) {
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)SystemMsg(0xD4E9), 0, 0, 0, 0));
			return;
		}

		m_pChangeGuildName = CreateForm<FrChangeGuildName>(g_pFresh->GetManager(), this, "change_guild_name", 0);

		if (m_pChangeGuildName->IsHaveGuild()) {
			m_pChangeGuildName->EnableDrag(false);
			m_pChangeGuildName->SetCaption(SystemMsg(0x3EF));
			m_pChangeGuildName->Open((FRESH_RESULT_CALLBACK)&CGuildActor::OnChangeGuildNameResult, 1);
		}
		break;
	}
}

// guildactor.cpp:643
void CGuildActor::OpenGuildJoinForm(int guildNumber) {
	if (m_pGuildJoin)
		return;

	m_pGuildJoin = CreateForm<FrGuildJoin>(g_pFresh->GetManager(), this, "requestjoinform", 0);

	m_pGuildJoin->SetFixed(true);
	m_pGuildJoin->EnableDrag(false);
	m_pGuildJoin->SetGuildNumber(guildNumber);
	m_pGuildJoin->Open((FRESH_RESULT_CALLBACK)&CGuildActor::OnGuildJoinFormResult, 1);
}

// guildactor.cpp:659
void CGuildActor::OpenGuildQuestionForm(const char* text, int type) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);

	pForm->SetMessage(text, false);
	pForm->SetTopmost(true);
	pForm->FindNextTopFocus(false);

	FRESH_RESULT_CALLBACK pfnResult = 0;
	switch (type) {
	case 0:
		pfnResult = (FRESH_RESULT_CALLBACK)&CGuildActor::OnGuildShutDownResult;
		break;

	case 1:
		pfnResult = (FRESH_RESULT_CALLBACK)&CGuildActor::OnGuildMasterDelegate;
		break;

	case 2:
		pfnResult = (FRESH_RESULT_CALLBACK)&CGuildActor::OnGuildOutResult;
		break;
	}

	if (pfnResult)
		pForm->Open(pfnResult, 1);
}

// guildactor.cpp:689
void CGuildActor::ReInitMyGuildInfo_SharedDoc(GUILD_USER_INFO* pInfo) {
	sPangYaUserInfo& userInfo = WSingleton<CSharedDoc>::Instance()->m_userInfo;

	userInfo.guildUID = pInfo->guildNumber;
	userInfo.guildPosition = pInfo->value19;
	strncpy(userInfo.guildName, pInfo->name, 0x14);
	strncpy(userInfo.guildMark, pInfo->emblem, 0xB);
}

// guildactor.cpp:700
std::pair<bool, bool> CGuildActor::CheckMyGuildState(GUILD_USER_INFO* pInfo) {
	if (!m_pGuildInfo)
		return std::pair<bool, bool>(true, false);

	int myClass = m_pGuildInfo->GetClassIndex();
	switch (pInfo->classIndex) {
	case 0:
		if (myClass == 1 || myClass == 3 || myClass == 2)
			return std::pair<bool, bool>(true, false);

		if (myClass == 9)
			return std::pair<bool, bool>(false, true);
		break;

	case 9:
		if (myClass == 0)
			return std::pair<bool, bool>(false, true);
		break;

	case 3:
		if (myClass == 9) {
			ChangeGuildEmblem(pInfo);
			return std::pair<bool, bool>(false, true);
		} else if (myClass == 2) {
			return std::pair<bool, bool>(false, true);
		} else if (myClass == 1) {
			return std::pair<bool, bool>(false, true);
		}
		break;

	case 2:
		if (myClass == 3)
			return std::pair<bool, bool>(false, true);
		break;

	case 1:
		if (myClass == 2)
			return std::pair<bool, bool>(false, true);
		else if (myClass == 0)
			return std::pair<bool, bool>(false, true);
		break;
	}

	return std::pair<bool, bool>(false, false);
}

// guildactor.cpp:793
void CGuildActor::ChangeGuildEmblem(GUILD_USER_INFO* pInfo) {
	if (pInfo->guildNumber == 0)
		return;

	m_pGuildInfo->SetGuildEmblem(pInfo);

	strncpy(WSingleton<CSharedDoc>::Instance()->m_userInfo.guildMark, pInfo->emblem, 0xB);

	std::map<unsigned long, sBriefUserInfo>::iterator it;
	if (WSingleton<CSharedDoc>::Instance()->FindUserInfo(WSingleton<CSharedDoc>::Instance()->m_userInfo.guid, it) == 1) {
		strncpy(it->second.guildMark, pInfo->emblem, 0xB);
	}

	strncpy(WSingleton<CSharedDoc>::Instance()->m_userInfo.guildMark, pInfo->emblem, 0xB);
}

// guildactor.cpp:816
void CGuildActor::ChangeGuildName(GUILD_INFO* pInfo) {
	if (pInfo->guildNumber == 0)
		return;

	m_pGuildInfo->SetGuildName(pInfo);

	strncpy(WSingleton<CSharedDoc>::Instance()->m_userInfo.guildName, pInfo->name, 0x14);
}

// guildactor.cpp:830
const char* CGuildActor::SystemMsg(int index) {
	const char* msg = m_pGuildInfo->GuildMsg(index);
	if (!msg)
		msg = "\xc1\xa4\xc0\xc7 \xb5\xc7\xc1\xf6 \xbe\xca\xc0\xba \xbf\xa1\xb7\xaf \xb8\xde\xbd\xc3\xc1\xf6\xc0\xd4\xb4\xcf\xb4\xd9.";
	return msg;
}

// guildactor.cpp:837
bool CGuildActor::OnCreateGuildFormResult(int ret, FrForm* pForm) {
	m_pCreateGuildKit = 0;

	return true;
}

// guildactor.cpp:844
bool CGuildActor::OnRegisterGuildMarkResult(int ret, FrForm* pForm) {
	m_pRegisterGuildMark = 0;

	return true;
}

// guildactor.cpp:851
bool CGuildActor::OnChangeGuildNameResult(int ret, FrForm* pForm) {
	m_pChangeGuildName = 0;

	return true;
}

// guildactor.cpp:858
bool CGuildActor::OnGuildJoinFormResult(int ret, FrForm* pForm) {
	m_pGuildJoin = 0;

	return true;
}

// guildactor.cpp:865
bool CGuildActor::OnMainFormCloseResult(int ret, FrForm* pForm) {
	m_pMainForm = 0;

	return true;
}

// guildactor.cpp:872
bool CGuildActor::OnGuildShutDownResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		WSendPacket packet((enumClientPacket)0x104);
		packet.Encode4(m_pGuildInfo->GetMyGuildInfo().guildNumber);
		packet.Send(SEND_TO_DEFAULT);
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	}

	return true;
}

// guildactor.cpp:885
bool CGuildActor::OnGuildMasterDelegate(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		if (m_pMainForm) {
			m_pMainForm->RequestChangeClass(GUILD_CLASS_MASTER);
		}
	}

	return true;
}

// guildactor.cpp:898
bool CGuildActor::OnGuildOutResult(int ret, FrForm* pForm) {
	if (ret == FrOK) {
		WSendPacket packet((enumClientPacket)0x110);
		packet.Encode4(m_pGuildInfo->GetMyGuildInfo().guildNumber);
		packet.Send(SEND_TO_DEFAULT);
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	}

	return true;
}

} // namespace _guild
