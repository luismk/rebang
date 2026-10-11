// Original: d:\build\custom\temp\code\client\projectg\ani_overlay.cpp
#include "ani_overlay.h"

#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"
#include "ClientSetting.h"

// ani_overlay.cpp:17
CAniOverlay::CAniOverlay(const char* name, float frameWidth, float frameHeight) : m_pOverlay(NULL) {
	m_pOverlay = g_resrcmng->GetOverlay(name, 0);
	m_frameWidth = frameWidth;
	m_frameHeight = frameHeight;

	m_columns = (int)(m_pOverlay->GetWidth() / frameWidth);



	m_rows = (int)(m_pOverlay->GetHeight() / frameHeight);
	m_frameCount = m_columns * m_rows;
}

// ani_overlay.cpp:29
CAniOverlay::~CAniOverlay(void) {
	if (m_pOverlay != NULL) {

		g_resrcmng->Release(m_pOverlay);
		m_pOverlay = NULL;
	}

}

// ani_overlay.cpp:38
void CAniOverlay::Render(float x, float y, int frame, int flags, int color, float scale) {


	float width = m_frameWidth * scale;

	m_dstRect.x = x - 0.5f * width;
	float height = m_frameHeight * scale;
	m_dstRect.y = y - 0.5f * height;
	m_dstRect.w = width;
	m_dstRect.h = height;
	if (m_pOverlay != NULL) {
		m_pOverlay->SetClippingArea(&m_dstRect);
	}
	int column = frame % m_columns;
	int row = frame / m_columns;

	m_srcRect.x = m_dstRect.x - column * scale * m_frameWidth;
	m_srcRect.y = m_dstRect.y - row * scale * m_frameHeight;
	if (m_pOverlay != NULL) {
		m_srcRect.w = m_pOverlay->GetWidth() * scale;
		m_srcRect.h = m_pOverlay->GetHeight() * scale;




		m_pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), m_srcRect, flags, color, 0.0f, 0);
	}
}

// File-local accumulator of CEffectOverlay::Process (no symbol in the PDB).
static float s_effectElapsed = 0.0f;

// ani_overlay.cpp:64
CEffectOverlay::CEffectOverlay(void) : m_pAniOverlay(NULL), m_active(false) {
	m_pAniOverlay = new CAniOverlay("bar-impact.jpg", 32.0f, 32.0f);
}

// ani_overlay.cpp:69
CEffectOverlay::~CEffectOverlay(void) {
	if (m_pAniOverlay != NULL) {
		delete m_pAniOverlay;
		m_pAniOverlay = NULL;
	}
}

// ani_overlay.cpp:74
void CEffectOverlay::Process(float elapsed) {
	if (m_active) {




		s_effectElapsed += elapsed;

		if (s_effectElapsed > m_frameTime) {

			s_effectElapsed -= m_frameTime;
			m_frame++;

			if (m_frame == 8) {

				s_effectElapsed = 0.0f;
				m_active = false;
			}
		}
	}
}

// ani_overlay.cpp:96
void CEffectOverlay::SetActive(void) {
	m_active = true;
	m_frame = 0;
	m_frameTime = 0.0625f;
}

// ani_overlay.cpp:103
void CEffectOverlay::Display(void) {
	if (m_active) {


		m_pAniOverlay->Render(140.0f, COption::Instance()->gGetBar_Y() - 8.0f, m_frame, 0x800000, -1, 1.0f);
	}
}
