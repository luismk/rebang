// Original: d:\build\custom\temp\code\client\projectg\noticeboard.cpp
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#include "NoticeBoard.h"
#include "ChatMsg.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "wresrcmng.h"
#include "wfont.h"
#include "wview.h"
#include "wutil.h"
#include "SoundManager.h"
#include "binstr.h"
#include "frwnd.h"
#include "ProjectG.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "onelineboard.h"

// 1e-5f in .rdata, read from storage rather than a __real literal (a named
// const float in the original; only CNoticeBoard::Display references it).
extern "C" const float data_a08d80;

// noticeboard.cpp:28
CNoticeBoard::CNoticeBoard(void) {
	m_pOverlayLeft = g_resrcmng->GetOverlay("notice_l.tga", 0);
	m_pOverlayMid = g_resrcmng->GetOverlay("notice_m.tga", 0);
	m_pOverlayRight = g_resrcmng->GetOverlay("notice_r.tga", 0);

	m_pFont = CChatMsg::Instance()->GetMaskedFont();
	m_current = m_noticeList.end();
	m_posX = -5555.0f;
	m_posY = -50.0f;
}

// noticeboard.cpp:41
CNoticeBoard::~CNoticeBoard(void) {
	m_current = m_noticeList.end();
	sequence_delete(m_noticeList.begin(), m_noticeList.end());

	if (g_resrcmng != NULL && m_pOverlayLeft != NULL) { g_resrcmng->Release(m_pOverlayLeft); m_pOverlayLeft = NULL; }
	if (g_resrcmng != NULL && m_pOverlayMid != NULL) { g_resrcmng->Release(m_pOverlayMid); m_pOverlayMid = NULL; }
	if (g_resrcmng != NULL && m_pOverlayRight != NULL) { g_resrcmng->Release(m_pOverlayRight); m_pOverlayRight = NULL; }
}

// noticeboard.cpp:52
void CNoticeBoard::Process(float elapsed) {
	elapsed /= WSingleton<CProjectG>::Instance()->GetGameSpeed();

	if (m_current == m_noticeList.end()) {
		m_current = m_noticeList.begin();
		if (m_current == m_noticeList.end()) {
			if (m_posY > -50.0f) {
				m_posY -= elapsed * 30.0f;
				if (m_posY < -50.0f) {
					m_posY = -50.0f;
				}
			}
			if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
				WSingleton<COneLineBoard>::Instance()->FadeIn();
			}
			return;
		}
	}

	if (m_posY < -0.1f) {
		m_posY += elapsed * 30.0f;
		if (!(m_posY > 0.0f)) {
			return;
		}
	}

	m_posY = 0.0f;
	sNotice* notice = *m_current;
	if (m_posX < -5000.0f) {
		m_posX = g_view->GetWidth() * 0.5f + 150.0f;
	}
	m_posX -= elapsed * 30.0f;

	float center = g_view->GetWidth() * 0.5f;
	if (m_posX + notice->m_width < center - 150.0f) {
		m_posX = center + 150.0f;
		if (--notice->m_repeatCount < 1) {
			if (*m_current != NULL) {
				delete *m_current;
			}
			m_current = m_noticeList.erase(m_current);
		} else {
			++m_current;
		}
	}
}

// noticeboard.cpp:120
void CNoticeBoard::Display(void) {
	if (m_pOverlayLeft != NULL && m_pOverlayMid != NULL && m_pOverlayRight != NULL) {
		float alpha = (m_posY + 50.0f) * 0.02f;
		if (!(WAbsBySign(alpha) < data_a08d80)) {
			WRect src(0.0f, 0.0f, 1.0f, 1.0f);
			WRect dst(g_view->GetWidth() * 0.5f - 150.0f - 20.0f, m_posY, m_pOverlayLeft->GetWidth(), m_pOverlayLeft->GetHeight());
			m_pOverlayLeft->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);

			dst.x += dst.w;
			dst.w = 300.0f - m_pOverlayRight->GetWidth() + 20.0f;
			dst.h = (float)m_pOverlayMid->GetHeight();
			m_pOverlayMid->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);

			dst.x += dst.w;
			dst.w = (float)m_pOverlayRight->GetWidth();
			dst.h = (float)m_pOverlayRight->GetHeight();
			m_pOverlayRight->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);
		}
	}

	if (m_current != m_noticeList.end()) {
		sNotice* notice = *m_current;
		if (m_pFont != NULL) {
			WRect clip(g_view->GetWidth() * 0.5f - 150.0f, 0.0f, 300.0f, 40.0f);
			m_pFont->SetClippingArea(&clip);

			m_pFont->Print(g_view, (float)(int)m_posX, 7.0f, notice->m_text.c_str(), 0x80000, notice->m_color, NULL);

			m_pFont->Flush(g_view);
			m_pFont->SetClippingArea(NULL);
		}
	}
}

// noticeboard.cpp:169
bool CNoticeBoard::GetColornText(const char* pszText, std::string& text, unsigned long& color) {
	color = 0xFFFFFFFF;
	cTokenV token;
	if (token.Init(pszText, strlen(pszText))) {
		char* pToken;
		bool ret = token.GetTokenFullSep(&pToken, "\\c", 2);
		if (pToken == 0 || *pToken == 0) {
			if (!ret) {
				return false;
			}
		}
		if (ret) {
			token.GetTokenFullSep(&pToken, "\\c", 2);

			unsigned long value = 0xFFFFFFFF;
			cTokenV colorToken;
			bool more = colorToken.Init(pToken, strlen(pToken));
			while (more) {
				more = colorToken.GetToken(&pToken, ",", 1);
				if (pToken != 0 && *pToken != 0) {
					if (strlen(pToken) > 1) {
						sscanf(pToken, "%x", &value);
					}
				}
			}
			color = value;

			std::string str(pszText);
			text = str.substr(str.rfind("\\c") + 2);
		}
		else {
			text = pszText;
		}
		return true;
	}
	return false;
}

// noticeboard.cpp:232
void CNoticeBoard::AddNotice(const char* text, int repeatCount, bool immediately) {
	sNotice* notice = new sNotice;

	notice->m_color = 0xFFFFFFFF;
	if (!GetColornText(text, notice->m_text, notice->m_color)) {
		notice->m_text = text;
	}

	notice->m_repeatCount = repeatCount;
	notice->m_width = m_pFont->GetTextWidth(g_view, text);

	size_t prevCount = m_noticeList.size();
	m_noticeList.push_back(notice);

	if (prevCount == 0 && m_noticeList.size() == 1 && m_posY < -0.1f) {
		g_audio->PlaySfx("notice_dingdong", NULL, 0, NULL, NULL, 0.5f, 200.0f);
	}

	if (immediately) {
		m_current = m_noticeList.end();
		--m_current;
		m_posX = g_view->GetWidth() * 0.5f + 150.0f;
	}

	if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
		WSingleton<COneLineBoard>::Instance()->FadeOut();
	}
}
