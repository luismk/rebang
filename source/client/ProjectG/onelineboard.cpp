// Original: d:\build\custom\temp\code\client\projectg\onelineboard.cpp
#include <windows.h>

#include "onelineboard.h"

#include "CapturedBg.h"
#include "GolfTask.h"
#include "TaskManager.h"
#include "commonutil.h"
#include "bitmap.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwnd.h"
#include "frwndmanager.h"
#include "fremoticon.h"
#include <mmsystem.h>

// onelineboard.cpp:23
COneLineBoard::COneLineBoard(void) {
	m_fadeState = 0;
	m_waitMsgNum = 0;
	m_waitMsgTime = 0;
	m_bg = 0;
	m_showBg = true;
	m_fadeFactor = 1.0f;
	m_visible = true;
}

// onelineboard.cpp:35
COneLineBoard::~COneLineBoard(void) {
	Clear();
}

// onelineboard.cpp:40
void COneLineBoard::Clear(void) {
	sequence_delete(m_msgList.begin(), m_msgList.end());
	m_msgList.clear();
}

// onelineboard.cpp:47
void COneLineBoard::Process(float elapsed) {
	static DWORD s_lastTick = 0;

	if (m_fadeState == 1) {
		m_fadeFactor -= elapsed;

		if (m_fadeFactor < 0.0f) {
			m_fadeFactor = 0.0f;
		}
	} else if (m_fadeState == 2) {
		m_fadeFactor += elapsed;

		if (m_fadeFactor > 1.0f) {
			m_fadeFactor = 1.0f;
			m_fadeState = 0;
		}
	}

	if (m_fadeState != 0) {
		return;
	}

	DWORD tick = GetTickCount();
	if (tick - s_lastTick < 25) {
		return;
	}

	s_lastTick = tick;

	m_visible = true;

	CCapturedBg *capturedBg = WSingleton<CCapturedBg>::Instance();
	if (capturedBg && capturedBg->IsCaptured()) {
		m_visible = false;
		return;
	}

	for (m_iter = m_msgList.begin(); m_iter != m_msgList.end(); ) {
		sOnelineMsg *msg = *m_iter;

		msg->x -= 0.5f;

		if (msg->x + msg->width < m_rect.x) {
			delete msg;
			m_msgList.erase(m_iter++);
		} else {
			++m_iter;
		}
	}
}

// onelineboard.cpp:109
void COneLineBoard::Display(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		DrawBg();
	}

	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();

	float alpha = gdi->GetAlpha();

	gdi->SetAlpha(m_fadeFactor);

	if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGolfTask::m_RTTI) : false) {
		gdi->SetTextStyle(4);
		gdi->SetTextColor(0xFFFFFFFF, 0xFF000000);
	} else {
		gdi->SetTextStyle(0);
		gdi->SetTextColor(0xFF000000, 0xFFFFFFFF);
	}

	FrEmoticon *emoticon = g_pFresh->GetManager()->GetEmoticon();
	WRect clip(m_rect.x, m_rect.y - 10.0f, m_rect.w, m_rect.h + 20.0f);
	emoticon->SetClippingArea(&clip);

	for (m_iter = m_msgList.begin(); m_iter != m_msgList.end(); ++m_iter) {
		sOnelineMsg *msg = *m_iter;

		if (m_visible) {
			emoticon->SetAnim(true);
		}

		emoticon->SetAnimTime(msg->time);
		emoticon->PrintText(WPoint((float)(int)msg->x, m_rect.y + 5.0f), 0, msg->text.c_str(), 0xFFFFFFFF);
		emoticon->SetAnim(false);
	}

	emoticon->SetClippingArea(0);

	gdi->SetAlpha(alpha);
	gdi->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);
	gdi->SetTextStyle(0);
}

// onelineboard.cpp:151
void COneLineBoard::AddOnelineMsg(const std::string &name, const std::string &message) {
	FrEmoticon *emoticon = g_pFresh->GetManager()->GetEmoticon();

	sOnelineMsg *msg = new sOnelineMsg;
	msg->text = message + "  <" + name + ">";
	msg->width = emoticon->GetTextWidth(msg->text.c_str());

	float x = 0.0f;
	if (m_msgList.size() != 0) {
		sOnelineMsg *last = m_msgList.back();

		x = last->x + last->width + 50.0f;
	}

	msg->x = x > m_rect.x + m_rect.w ? x : m_rect.x + m_rect.w;

	msg->time = timeGetTime();

	m_msgList.push_back(msg);
}

// onelineboard.cpp:177
void COneLineBoard::LoadBg(void) {
	if (!m_bg) {
		m_bg = g_pFresh->GetBitmap("oneline_back");
	}
}

// onelineboard.cpp:183
void COneLineBoard::FadeOut(void) {
	m_fadeState = 1;
}

// onelineboard.cpp:189
void COneLineBoard::FadeIn(void) {
	m_fadeState = 2;
}

// onelineboard.cpp:195
float COneLineBoard::GetFadeFactor(void) {
	if (m_fadeState == 0) {
		return 1.0f;
	}
	return m_fadeFactor;
}

// onelineboard.cpp:204
void COneLineBoard::DrawBg(void) {
	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		if (!m_showBg) {
			return;
		}

		if (WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
			return;
		}
	}

	if (!m_bg) {
		return;
	}

	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (!gdi) {
		return;
	}

	gdi->DrawTexture(m_bg, WRect(0, 0, m_bg->Width(), m_bg->Height()), WRect(m_rect.x - 2.0f, m_rect.y, m_bg->Width(), m_bg->Height()), 0xFFFFFFFF, 0);
}
