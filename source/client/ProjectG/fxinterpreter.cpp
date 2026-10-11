// Original: d:\build\custom\temp\code\client\projectg\fxinterpreter.cpp
#include <string.h>

#include "fxinterpreter.h"
#include "fx.h"

// commonutil.cpp; not declared in commonutil.h yet.
const char* GetStrConst(const char* pSrc, char* pDest);
const char* GetCharsTillSpace(const char* pSrc, char* pDest);

const char FxInterpreter::TOKEN[2][8] = { "*fx", "*playif" };

// fxinterpreter.cpp:20
FxInterpreter* GetFxInterpreter(void) {
	static FxInterpreter s_interpreter;
	return &s_interpreter;
}

// fxinterpreter.cpp:29
FxInterpreter::sFxInfo::sFxInfo(int handle, eEffectType type, void* pFx) {
	m_handle = handle;
	m_type = type; m_pFx = pFx;
}

// fxinterpreter.cpp:36
void FxInterpreter::sFxInfo::DetachSingleFx(void) {
	if (CFx::Instance()) {
		if (m_type == EFFECT_SPRAY) {
			CFx::Instance()->CloseSpray((CFxSpray*)m_pFx);
		} else if (m_type == EFFECT_SEQUENCE) {
			CFx::Instance()->CloseSequence((CFxSequence*)m_pFx, true);
		}
	}
}

// fxinterpreter.cpp:56
FxInterpreter::FxInterpreter(void) {
	m_handle = 0;
}

// fxinterpreter.cpp:61
FxInterpreter::~FxInterpreter(void) {
	if (!m_fxList.empty()) {
		DetachFxAll();
	}
}

// fxinterpreter.cpp:70
bool FxInterpreter::ParseToken(const char** ppScript, eScripFormat format) const {
	*ppScript = strstr(*ppScript, TOKEN[format]);
	return *ppScript != NULL;
}

// fxinterpreter.cpp:76
bool FxInterpreter::ParseFxName(const char** ppScript, char* name, eScripFormat format, eEffectType& type) const {
	switch (format) {
	case SCRIPT_FORMAT_TEXT:
		*ppScript = GetCharsTillSpace(*ppScript + 3, name);
		break;
	case SCRIPT_FORMAT_STRING:
		*ppScript = GetStrConst(*ppScript + 3, name);
		break;
	default:
		return false;
	}

	int len = strlen(name);
	if (len == 0 || len >= 64) {
		return false;
	}

	const char* ext = strrchr(name, '.');
	if (ext == NULL || strlen(ext) < 4) {
		return false;
	}

	if (stricmp(ext + 1, "seq") == 0) {
		type = EFFECT_SEQUENCE;
	}
	else if (stricmp(ext + 1, "spr") == 0) {
		type = EFFECT_SPRAY;
	}
	else {
		type = EFFECT_NONE;
		return false;
	}

	return true;
}

// fxinterpreter.cpp:160
int FxInterpreter::ParseCharBoundingBoxScript_fx(WPuppet* pPuppet, eScripFormat format) {
	return -1;
}

// fxinterpreter.cpp:165
int FxInterpreter::ParseCharFrameDataScrip_fx(WPuppet* pPuppet, eScripFormat format) {
	return -1;
}

// fxinterpreter.cpp:170
bool FxInterpreter::AttachFx(const char* name, const Attacher* pAttacher, eEffectType type) {
	if (name == NULL || pAttacher == NULL) {
		return false;
	}

	if (type == EFFECT_SPRAY) {
		void* pFx = CFx::Instance()->Attach(*pAttacher, name, false, NULL);
		m_fxList.push_back(sFxInfo(m_handle, EFFECT_SPRAY, pFx));
	} else if (type == EFFECT_SEQUENCE) {
		void* pFx = CFx::Instance()->Attach(*pAttacher, name, false, NULL);
		m_fxList.push_back(sFxInfo(m_handle, EFFECT_SEQUENCE, pFx));
	} else {
		return false;
	}

	return true;
}

// fxinterpreter.cpp:193
void FxInterpreter::DetachFx(int handle) {
	if (handle < 0) {
		return;
	}

	std::list<sFxInfo>::iterator it = m_fxList.begin();
	while (it != m_fxList.end()) {
		if (it->m_handle == handle) {
			std::list<sFxInfo>::iterator cur = it++;
			cur->DetachSingleFx();
			m_fxList.erase(cur);
		} else if (it->m_handle > handle) {
			break;
		} else {
			++it;
		}
	}
}

// fxinterpreter.cpp:217
void FxInterpreter::DetachFxAll(void) {
	for (std::list<sFxInfo>::iterator it = m_fxList.begin(); it != m_fxList.end(); ++it) {
		it->DetachSingleFx();
	}

	m_fxList.clear();
}
