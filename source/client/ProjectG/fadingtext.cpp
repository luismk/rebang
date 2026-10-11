// Original: d:\build\custom\temp\code\client\projectg\fadingtext.cpp
#include "FadingText.h"

#include "../client/Fresh/include/fresh.h"
#include "../client/Fresh/include/frgraphicinterface.h"
#include "../client/Fresh/include/FrWndManager.h"
#include "../client/Fresh/include/FrWnd.h"

#include <windows.h>
#include <string.h>

// fadingtext.cpp:10
CFadingText::CFadingText() {
	m_duration = 0;
	m_pWnd = 0;
	m_x = 0;
	m_y = 0;
	m_color = 0xff000000;
	m_type = eFADE_OUT;

	m_active = false;
}

// fadingtext.cpp:22
CFadingText::~CFadingText() {
}

// fadingtext.cpp:26
void CFadingText::Print(const char* text, float duration, FrWnd* pWnd, float x, float y, eFadeType type,
	unsigned long color) {
	strncpy(m_text, text, sizeof(m_text));

	m_duration = duration;
	m_pWnd = pWnd;
	m_x = x;
	m_y = y;
	m_color = color;
	m_type = type;
	m_elapsed = 0;

	if (type == eFADE_OUT) {
		m_alpha = 0xff;
		m_direction = -1;
	}
	else if (type == eFADE_IN) {
		m_direction = 1;
		m_alpha = 0;
	}
	else if (type == eFADE_IN_OUT) {
		m_alpha = 0;
		m_direction = 2;
	}

	m_active = true;
}

// Tick of CFadingText::Process's last update (no symbol).
extern "C" {
unsigned long data_af5030;
}

// fadingtext.cpp:56
void CFadingText::Process(void) {
	float elapsed = 0;


	unsigned long now = GetTickCount();
	if (data_af5030 == 0) data_af5030 = now;
	unsigned long delta = now - data_af5030;

	if (delta > 50) {
		// milliseconds to seconds
		elapsed = delta * 0.001f;
		data_af5030 = now;
	}

	if (m_type == eFADE_OUT || m_type == eFADE_IN_OUT) {
		if (m_duration < m_elapsed) {
			m_active = false;
		}
	}
	if (!m_active) {
		return;
	}

	float x = 0, y = 0;
	if (m_pWnd) {
		x = m_pWnd->GetRect().x;
		y = m_pWnd->GetRect().y;
	}

	x += m_x; y += m_y; int direction = m_direction; int alpha = direction > 0 ? 0 : 0xff;

	switch (m_type) {
	case eFADE_OUT:
	case eFADE_IN:
		alpha += (int)(direction * (m_elapsed / m_duration * 255.0f));
		break;
	case eFADE_IN_OUT:
		alpha += (int)(direction * (m_elapsed / m_duration * 255.0f));
		break;
	}

	if (alpha < 0) alpha = 0;
	else if (alpha > 0xff) alpha = 0xff;

	if (alpha == 0xff && m_type == eFADE_IN_OUT) {
		// faded in: now fade out
		m_direction = -2;
		m_elapsed = 0;
	}

	FrGraphicInterface* pDevice = g_pFresh->GetManager()->GetGDI();
	if (pDevice) {
		pDevice->SetTextColor((m_color & 0x00ffffff) + (alpha << 24), 0xffffffff);
		pDevice->Print(WPoint(x, y), 0, m_text);

		m_elapsed += elapsed;
	}
}
