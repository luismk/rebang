// Original: d:\build\custom\temp\code\client\projectg\ggc_dlg.cpp
#include "GGC_Dlg.h"
#include "GGC_Helper.h"
#include "ObjectFactory.h"
#include "fredit.h"

// ggc_dlg.cpp:20 (run-time type and factory registration of GGC_Dlg)
const WRTTI GGC_Dlg::m_RTTI("GGC_Dlg", &FrForm::m_RTTI);

IObject* GGC_DlgMakeInstance(void) {
	return new GGC_Dlg;
}

struct __sGGC_Dlg {
	__sGGC_Dlg(void) {
		ObjectFactory().AddObjectFunctor(GGC_DlgMakeInstance, "GGC_Dlg");
	}
};
static __sGGC_Dlg s_registerGGC_Dlg;

// ggc_dlg.cpp:22
const sFRESH_MSGMAP* GGC_Dlg::GetMessageMap(void) const {
	return &GGC_Dlg::_MsgMap;
}

sFRESH_MSGMAP GGC_Dlg::_MsgMap = { &FrForm::_MsgMap, &GGC_Dlg::_MsgEntries[0] };

sFRESH_ENTRY GGC_Dlg::_MsgEntries[] = {
	// ggc_dlg.cpp:24
	{ "message", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&GGC_Dlg::OnMessage_Init },

	// ggc_dlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// ggc_dlg.cpp:29
void GGC_Dlg::OnMessage_Init(intptr_t param) {
	m_pMessageEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pMessageEdit == 0) {
		return;
	}

	std::string message;
	unsigned long code;
	GGC_GetHelper()->GetFirstCriticalErrorMessage(code, message);

	m_pMessageEdit->AddLine(message.c_str(), 0, false);
	m_pMessageEdit->AddLine("", 0, false);
	m_pMessageEdit->AddLine("", 0, false);
	m_pMessageEdit->AddLine("* [\xB0\xD4\xC0\xD3\xB0\xA1\xB5\xE5]\xB4\xC2 [INCA]\xC0\xC7 \xC7\xD8\xC5\xB7 \xC2\xF7\xB4\xDC \xBC\xD6\xB7\xE7\xBC\xC7\xC0\xD4\xB4\xCF\xB4\xD9.", 0, false);
	m_pMessageEdit->AddLine("", 0, false);
	m_pMessageEdit->AddLine("\xBA\xCE\xB5\xE6\xC0\xCC \xC6\xCE\xBE\xDF\xB8\xA6 \xC1\xBE\xB7\xE1\xC7\xD5\xB4\xCF\xB4\xD9!", 0, false);
	m_pMessageEdit->AddLine("", 0, false);
	m_pMessageEdit->AddLine("\xB9\xAE\xC0\xC7 : http://pangya.ntreev.com", 0, false);
}
