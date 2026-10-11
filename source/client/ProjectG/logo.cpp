// Original: d:\build\custom\temp\code\client\projectg\logo.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "logo.h"

#include <new>

#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "commonutil.h"
#include "wresrcmng.h"
#include "woverlay.h"
#include "wview.h"

// logo.cpp:15 (run-time type and factory registration of CLogo)
const WRTTI CLogo::m_RTTI("CLogo", &IActor::m_RTTI);

IObject* CLogoMakeInstance(void) {
	return new CLogo;
}

struct __impCLogo {
	__impCLogo(void) {
		ObjectFactory().AddObjectFunctor(CLogoMakeInstance, "CLogo");
		ActorFactory().AddActorClass("CLogo");
	}
};
static __impCLogo s_impCLogo;

// logo.cpp:18
CLogo::CLogo(void) : m_pOverlay(0) {
	m_priority = 5;
}

// logo.cpp:23
void CLogo::OnDestroy(void) {
	if (m_pOverlay) {
		g_resrcmng->Release(m_pOverlay);
		m_pOverlay = 0;
	}
}

// logo.cpp:32
void CLogo::OnLoad(void) {
	if (GOLFDOC()->m_gameState < 15)
		return;

	m_pOverlay = g_resrcmng->GetOverlay(MakeStr("[logo_%d.png", GetCurMap()), 0);
}

// logo.cpp:40
void CLogo::OnInit(void) {
	m_alpha = 0.0f;
}

// logo.cpp:45
void CLogo::OnProcess(float elapsed) {
	if (m_pOverlay) {
		float alpha = m_alpha + elapsed * (1.0f / 3.0f);
		m_alpha = 1.0f > alpha ? alpha : 1.0f;
	}
}

// logo.cpp:53
void CLogo::OnDisplay(void) {
	if (m_pOverlay) {
		int x = (int)((g_view->GetWidth() - m_pOverlay->GetWidth()) * 0.5f);

		m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect((float)x, 0.0f, m_pOverlay->GetWidth(), m_pOverlay->GetHeight()), 0, ((unsigned char)(m_alpha * 255.0f) << 24) | 0xFFFFFF, 0.0f, 0);
	}
}

// logo.cpp:68
void CLogo::HandleMsg(const MsgObject& msg) {
	if (msg.m_msg == 0x18B) {
		OnDestroy();
	}
}
