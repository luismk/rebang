// Original: d:\build\custom\temp\code\client\projectg\onelinereqdlg.cpp
#include "OnelineReqDlg.h"
#include "EmoticonDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "fresh.h"
#include "winput.h"
#include "commonutil.h"
#include "ChatMsg.h"
#include "wfont.h"
#include "wview.h"
#include "SharedDoc.h"

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev* g_ime;

// onelinereqdlg.cpp:23 (run-time type and factory registration of FrOnelineReqDlg)
const WRTTI FrOnelineReqDlg::m_RTTI("FrOnelineReqDlg", &FrForm::m_RTTI);

IObject* FrOnelineReqDlgMakeInstance(void) {
	return new FrOnelineReqDlg;
}

struct __sFrOnelineReqDlg {
	__sFrOnelineReqDlg(void) {
		ObjectFactory().AddObjectFunctor(FrOnelineReqDlgMakeInstance, "FrOnelineReqDlg");
	}
};
static __sFrOnelineReqDlg s_registerFrOnelineReqDlg;

// onelinereqdlg.cpp:25
const sFRESH_MSGMAP* FrOnelineReqDlg::GetMessageMap(void) const {
	return &FrOnelineReqDlg::_MsgMap;
}

sFRESH_MSGMAP FrOnelineReqDlg::_MsgMap = { &FrForm::_MsgMap, &FrOnelineReqDlg::_MsgEntries[0] };

sFRESH_ENTRY FrOnelineReqDlg::_MsgEntries[] = {
	// onelinereqdlg.cpp:27
	{ "explain", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqDlg::OnExplainInit },
	{ "chatinput", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqDlg::OnChatInputInit },
	{ "chatinput", FRCMD_ENTERKEY, FrFuncBI, (FRESH_PMSG)(FRESH_PMSGBI)&FrOnelineReqDlg::OnChatEnterKey },
	{ "emoticon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqDlg::OnEmoticonInit },
	{ "emoticon", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrOnelineReqDlg::OnEmoticonBtnUp },
	{ "language", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqDlg::OnLanguageInit },
	{ "yes", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqDlg::OnYesInit },

	// onelinereqdlg.cpp:35
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// onelinereqdlg.cpp:39
FrOnelineReqDlg::FrOnelineReqDlg(void) {
	m_pChatInput = 0;
	m_pEmoticonBtn = 0;
	m_pLanguage = 0;
	m_pEmoticonDlg = 0;
}

// onelinereqdlg.cpp:47
FrOnelineReqDlg::~FrOnelineReqDlg(void) {
}

// onelinereqdlg.cpp:51
void FrOnelineReqDlg::OnChatInputInit(intptr_t param) {
	m_pChatInput = DYNAMIC_CAST(FrEdit, param);
	if (m_pChatInput)
		m_pChatInput->SetKeyFocus(true);
}

// onelinereqdlg.cpp:59
bool FrOnelineReqDlg::OnChatEnterKey(int param) {
	OnOK();
	return false;
}

// onelinereqdlg.cpp:66
void FrOnelineReqDlg::OnEmoticonInit(intptr_t param) {
	m_pEmoticonBtn = DYNAMIC_CAST(FrButton, param);
}

// onelinereqdlg.cpp:71
bool FrOnelineReqDlg::OnEmoticonResult(int ret, FrForm* pForm) {
	if (m_pEmoticonDlg && ret == 1) {
		FrEmoticonDlg* pDlg = DYNAMIC_CAST(FrEmoticonDlg, pForm);
		if (pDlg) {
			const char* icon = pDlg->GetSelectedIcon();
			if (icon) {
				FrEdit* pEdit = m_pChatInput;

				if (pEdit) {
					const char* front = pEdit->GetEditText_Front();
					const char* comp = pEdit->GetEditText_Comp();
					const char* end = pEdit->GetEditText_End();

					if (CChatMsg::Instance()->GetMaskedFont()->GetTextWidth(g_view, MakeStr("%s%s%s%s", front, comp, icon, end)) < pEdit->GetWidthLimit())
						pEdit->SetLine(1, MakeStr("%s%s%s%s", front, comp, icon, end), 0, false, 0);
					pEdit->SetKeyFocus(true);
				}
			}
		}
	}

	m_pEmoticonDlg = 0;
	return true;
}

// onelinereqdlg.cpp:101
void FrOnelineReqDlg::OnEmoticonBtnUp(void) {
	if (m_pEmoticonBtn && !m_pEmoticonDlg) {
		m_pEmoticonDlg = CreateForm<FrEmoticonDlg>(g_pFresh->GetManager(), this, "emoticon", 0);
		if (!m_pEmoticonDlg)
			return;

		WPoint pt(0, 0);
		if (m_pEmoticonBtn) {
			pt.x = m_pEmoticonBtn->GetRect().x - 200.0f;
			pt.y = m_pEmoticonBtn->GetRect().y - 260.0f;
		}

		m_pEmoticonDlg->Open((FRESH_RESULT_CALLBACK)&FrOnelineReqDlg::OnEmoticonResult, pt, 3);
	}
}

// onelinereqdlg.cpp:122
void FrOnelineReqDlg::OnLanguageInit(intptr_t param) {
	m_pLanguage = DYNAMIC_CAST(FrArea, param);
	if (m_pLanguage) {
		if (g_ime->IsAlphaNumericMode())
			m_pLanguage->SetBgImg("chat_eng");
		else
			m_pLanguage->SetBgImg("chat_kor");
	}
}

// onelinereqdlg.cpp:134
void FrOnelineReqDlg::SetLanguage(bool bEnglish) {
	if (bEnglish)
		m_pLanguage->SetBgImg("chat_eng");
	else
		m_pLanguage->SetBgImg("chat_kor");
}

// onelinereqdlg.cpp:142
void FrOnelineReqDlg::OnExplainInit(intptr_t param) {
	FrEdit* pExplain = DYNAMIC_CAST(FrEdit, param);

	pExplain->AddLine("\\c0xff000000,t\\c\xc0\xfc\xb1\xa4\xc6\xc7 \xbc\xd2\xb0\xb3\\c0xff000000,n\\c", 0, false);
	pExplain->AddLine("- \xb0\xd4\xc0\xd3 \xb3\xbb\xbf\xa1\xbc\xad \xc0\xaf\xc0\xfa\xb0\xa3 \xbc\xd2\xbd\xc4\xc0\xbb \xc0\xfc\xc7\xd2 \xbc\xf6 \xc0\xd6\xb4\xc2 \xb1\xa4\xb0\xed\xc0\xd4\xb4\xcf\xb4\xd9.", 0, false);
	pExplain->AddLine(MakeStr("- \xc0\xfc\xb1\xa4\xc6\xc7\xbf\xa1 \xb1\xdb \xb5\xee\xb7\xcf \xbd\xc3 %d\xc4\xed\xc5\xb0\xb0\xa1 \xbc\xd2\xba\xf1\xb5\xcb\xb4\xcf\xb4\xd9.", 1), 0, false);
	pExplain->AddLine("- \xc7\xd1\xb1\xdb \xb1\xe2\xba\xbb\xc0\xb8\xb7\xce \xc3\xd6\xb4\xeb 20\xc0\xda\xb1\xee\xc1\xf6 \xbe\xb2\xb1\xe2\xb0\xa1 \xb0\xa1\xb4\xc9\xc7\xd5\xb4\xcf\xb4\xd9.", 0, false);
	pExplain->AddLine("  (\xc0\xcc\xb8\xf0\xc6\xbc\xc4\xdc \xc1\xf6\xbf\xf8)", 0, false);

	pExplain->AddLine("\\c0xff000000,t\\c\xc0\xfc\xb1\xa4\xc6\xc7 \xbd\xc5\xc3\xbb\\c0xff000000,n\\c", 0, false);
	pExplain->AddLine("\\c0xffff0000\\c*\xc1\xd6\xc0\xc7", 0, false);

	pExplain->AddLine("- \xc0\xfc\xb1\xa4\xc6\xc7 \xbd\xc5\xc3\xbb\xc0\xcc \xbf\xcf\xb7\xe1\xb5\xc8 \xb1\xdb\xc0\xba \xbc\xf6\xc1\xa4, \xc3\xeb\xbc\xd2\xb0\xa1 \xba\xd2\xb0\xa1\xb4\xc9", 0, false);
	pExplain->AddLine("  \xc7\xd5\xb4\xcf\xb4\xd9.", 0, false);

	pExplain->AddLine("- \xc0\xbd\xb6\xf5\xb1\xdb, \xbf\xe5\xbc\xb3, \xbb\xf3\xbe\xf7\xc0\xfb \xb1\xa4\xb0\xed\xb8\xa6 \xbf\xc3\xb8\xb1 \xbd\xc3 \xb0\xd4\xc0\xd3\xb3\xbb \xc1\xa6\xc0\xe7\xb8\xa6 ", 0, false);

	pExplain->AddLine("  \xb9\xde\xc0\xbb \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, false);
	pExplain->AddLine("- \xc0\xcc \xbb\xf3\xc7\xb0\xc0\xba \xc3\xbb\xbe\xe0\xc3\xb6\xc8\xb8(\xb1\xb8\xb8\xc5\xc3\xeb\xbc\xd2)\xb0\xa1 \xba\xd2\xb0\xa1\xb4\xc9\xc7\xd1 \xbb\xf3\xc7\xb0\xc0\xd4\xb4\xcf\xb4\xd9.\\c0xff000000\\c", 0, false);
}

// onelinereqdlg.cpp:170
void FrOnelineReqDlg::OnYesInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// onelinereqdlg.cpp:176 (run-time type and factory registration of FrOnelineReqChkOutDlg)
const WRTTI FrOnelineReqChkOutDlg::m_RTTI("FrOnelineReqChkOutDlg", &FrForm::m_RTTI);

IObject* FrOnelineReqChkOutDlgMakeInstance(void) {
	return new FrOnelineReqChkOutDlg;
}

struct __sFrOnelineReqChkOutDlg {
	__sFrOnelineReqChkOutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrOnelineReqChkOutDlgMakeInstance, "FrOnelineReqChkOutDlg");
	}
};
static __sFrOnelineReqChkOutDlg s_registerFrOnelineReqChkOutDlg;

// onelinereqdlg.cpp:178
const sFRESH_MSGMAP* FrOnelineReqChkOutDlg::GetMessageMap(void) const {
	return &FrOnelineReqChkOutDlg::_MsgMap;
}

sFRESH_MSGMAP FrOnelineReqChkOutDlg::_MsgMap = { &FrForm::_MsgMap, &FrOnelineReqChkOutDlg::_MsgEntries[0] };

sFRESH_ENTRY FrOnelineReqChkOutDlg::_MsgEntries[] = {
	// onelinereqdlg.cpp:180
	{ "explain", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqChkOutDlg::OnExplainInit },
	{ "yes", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrOnelineReqChkOutDlg::OnYesInit },

	// onelinereqdlg.cpp:183
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// onelinereqdlg.cpp:187
FrOnelineReqChkOutDlg::FrOnelineReqChkOutDlg(void) {
}

// onelinereqdlg.cpp:191
FrOnelineReqChkOutDlg::~FrOnelineReqChkOutDlg(void) {
}

// onelinereqdlg.cpp:195
void FrOnelineReqChkOutDlg::OnYesInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
	if (pBtn) {
		if (WSingleton<CSharedDoc>::Instance()->m_myCash < 1) {
			pBtn->Enable(false);
		}
	}
}
