// Original: d:\build\custom\temp\code\client\projectg\reportdlg.cpp
#include <stdio.h>
#include <string>

#include "ReportDlg.h"
#include "EmoticonDlg.h"
#include "ObjectFactory.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "avatar_task.h"
#include "actor.h"
#include "PangFBI.h"
#include "SharedDoc.h"
#include "ChatMsg.h"
#include "commonutil.h"
#include "frbutton.h"
#include "frcombobox.h"
#include "fredit.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "wfont.h"
#include "wview.h"

extern char g_executeDirectory[];

// reportdlg.cpp:27 (run-time type and factory registration of FrReportDlg)
const WRTTI FrReportDlg::m_RTTI("FrReportDlg", &FrForm::m_RTTI);

IObject* FrReportDlgMakeInstance(void) {
	return new FrReportDlg;
}

struct __sFrReportDlg {
	__sFrReportDlg(void) {
		ObjectFactory().AddObjectFunctor(FrReportDlgMakeInstance, "FrReportDlg");
	}
};
static __sFrReportDlg s_registerFrReportDlg;

// reportdlg.cpp:29
const sFRESH_MSGMAP* FrReportDlg::GetMessageMap(void) const {
	return &FrReportDlg::_MsgMap;
}

sFRESH_MSGMAP FrReportDlg::_MsgMap = { &FrForm::_MsgMap, &FrReportDlg::_MsgEntries[0] };

sFRESH_ENTRY FrReportDlg::_MsgEntries[] = {
	// reportdlg.cpp:31
	{ "id", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrReportDlg::OnIdInit },
	// reportdlg.cpp:32
	{ "id", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrReportDlg::OnIdEnterKey },
	// reportdlg.cpp:33
	{ "emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrReportDlg::OnEmoticonBtnUp },
	// reportdlg.cpp:34
	{ "reason", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrReportDlg::OnReasonInit },
	// reportdlg.cpp:35
	{ "ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrReportDlg::OnOKInit },

	// reportdlg.cpp:37
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// reportdlg.cpp:41
FrReportDlg::FrReportDlg(void) {
	m_pId = 0;
	m_pReason = 0;
	m_pOK = 0;

	g_pFresh->GetManager()->SetExclusiveKey(true);
}

// reportdlg.cpp:50
FrReportDlg::~FrReportDlg(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) && !CChatMsg::Instance()->IsOpened())
		g_pFresh->GetManager()->SetExclusiveKey(false);
}

// reportdlg.cpp:56
void FrReportDlg::OnIdInit(intptr_t param) {
	m_pId = DYNAMIC_CAST(FrEdit, param);


	m_pId->SetKeyFocus(true);
}

// reportdlg.cpp:64
bool FrReportDlg::OnIdEnterKey(int param) {
	OnOK();

	return false;
}

// reportdlg.cpp:71
bool FrReportDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == 1) {

		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {

			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {

				FrEdit* pEdit = DYNAMIC_CAST(FrEdit, FindChildByName("id"));
				if (m_pId) {

					const char* front = m_pId->GetEditText_Front();
					const char* comp = m_pId->GetEditText_Comp();
					const char* end = m_pId->GetEditText_End();


					if (CChatMsg::Instance()->GetMaskedFont()->GetTextWidth(g_view, MakeStr("%s%s%s%s", front, comp, icon, end)) < m_pId->GetWidthLimit())
						m_pId->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					m_pId->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// reportdlg.cpp:101
void FrReportDlg::OnEmoticonBtnUp(void) {
	m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
	if (m_pEmoticonDlg) {

		m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrReportDlg::OnEmoticonResult, 3);
	}
}

// reportdlg.cpp:110
void FrReportDlg::OnReasonInit(intptr_t param) {
	m_pReason = DYNAMIC_CAST(FrComboBox, param);
	if (m_pReason) {
		m_pReason->AddString("1.\xbf\xe5\xbc\xb3,\xc5\xb8\xc0\xce\xba\xf1\xb9\xe6");
		m_pReason->AddString("2.\xba\xce\xb8\xf0/\xbc\xba\xc0\xfb\xbf\xe5\xbc\xb3");
		m_pReason->AddString("3.\xb1\xe2\xc5\xb8\xbb\xe7\xc7\xd7");

		m_pReason->SetLine(1, "1.\xbf\xe5\xbc\xb3,\xc5\xb8\xc0\xce\xba\xf1\xb9\xe6", 0, false, 0);
	}
}

// reportdlg.cpp:135
void FrReportDlg::OnProc(const float dt) {
	std::string id = m_pId->GetLine(1, false);
	if (id.empty())
		m_pOK->Enable(false);
	else
		m_pOK->Enable(true);
}

// reportdlg.cpp:144
void FrReportDlg::OnOKInit(intptr_t param) {
	m_pOK = DYNAMIC_CAST(FrButton, param);
	m_pOK->Enable(false);
}

// reportdlg.cpp:150
void FrReportDlg::OnOK(void) {
	char path[0x200];

	strcpy(path, g_executeDirectory);

	char buffer[0x400] = { 0 };
	FILE* fp = fopen(MakeStr("%s\\chat_history.txt", path), "wt");
	if (fp == 0) {
		return;
	}

	SYSTEMTIME st;
	GetLocalTime(&st);

	sprintf(buffer, "\xBD\xC5\xB0\xED\xBD\xC3\xB0\xA3 : %d:%02d:%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	fwrite(buffer, strlen(buffer), 1, fp);

	sprintf(buffer, "\xBD\xC5\xB0\xED\xB4\xEB\xBB\xF3 \xC0\xAF\xC0\xFA : %s\n", m_pId->GetLine(1, false));
	fwrite(buffer, strlen(buffer), 1, fp);

	sprintf(buffer, "------------------------------------------------------------\n");
	fwrite(buffer, strlen(buffer), 1, fp);

	std::list<sChatLine>::iterator it;

	for (it = CSharedDoc::Instance()->m_chatLineList.begin(); it != CSharedDoc::Instance()->m_chatLineList.end(); ++it) {

		fwrite(it->m_text.c_str(), strlen(it->m_text.c_str()), 1, fp);
		fwrite("\n", 1, 1, fp);
	}

	fclose(fp);

	// The report goes to the server through CPangFBI: the log file name with
	// the reporter's nickname.
	_snprintf(buffer, 0x3FF, "%s %d %s\\chat_history.txt", CSharedDoc::Instance()->GetPangyaUserInfo().nickname, 4, path);

	CPangFBI::Instance()->ProcessReport("crime_log", buffer);

	FrForm::OnOK();

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {

		AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0x23, (intptr_t)"\xBD\xC5\xB0\xED\xB0\xA1 \xC1\xA4\xBB\xF3\xC0\xFB\xC0\xB8\xB7\xCE \xC1\xA2\xBC\xF6\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
	} else if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI)) {

		AfxGetTask()->GetActor("AvatarChat") << MsgObject(0, 0x23, (intptr_t)"\xBD\xC5\xB0\xED\xB0\xA1 \xC1\xA4\xBB\xF3\xC0\xFB\xC0\xB8\xB7\xCE \xC1\xA2\xBC\xF6\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
	} else {

		AfxGetTask()->GetActor("Lobby") << MsgObject(0, 0x23, (intptr_t)"\xBD\xC5\xB0\xED\xB0\xA1 \xC1\xA4\xBB\xF3\xC0\xFB\xC0\xB8\xB7\xCE \xC1\xA2\xBC\xF6\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0);
	}
}
