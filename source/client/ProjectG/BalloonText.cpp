// Original: d:\build\custom\temp\code\client\projectg\balloontext.cpp
#include "balloontext.h"

#include <string.h>

#include <algorithm>

#include "avatar_avatar.h"
#include "commonutil.h"
#include "fremoticon.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "wfont.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"

// The emoticon closing character (0xA84394, a writable int in .data).
static int s_emoticonClose = ')';

// balloontext.cpp:30
ntBalloonText::ntBalloonText(unsigned long guid, const char *text, const float * const offset, float duration) {
	m_guid = guid;
	m_textChanged = false;
	m_textExtent = 0;
	m_offset[0] = offset[0];
	m_offset[1] = offset[1];
	m_offset[2] = offset[2];
	m_transparency = offset[3];
	m_duration = duration;
	m_elapsed = 0.0f;
	SetText(text);
}

// balloontext.cpp:44
ntBalloonText::~ntBalloonText(void) {
}

// balloontext.cpp:48
void ntBalloonText::SetText(const char *text) {
	if (text == NULL || *text == '\0') {
		return;
	}

	if (*text == '!') {
		text++;
		m_isThink = true;
	} else {
		m_isThink = false;
	}

	const char *stripped = StripSpecialText(text);
	if (*stripped == '\0') {
		return;
	}

	m_textChanged = true;
	m_text = stripped;

	m_elapsed = 0.0f;
}

// balloontext.cpp:99
const char *ntBalloonText::StripSpecialText(const char *text) const {
	// Size not established (the original's buffer at 0xAF41A0 has no symbol).
	static char s_buffer[1024];

	int length = strlen(text);

	int src = 0;
	int dst = 0;

	while (src < length) {
		char c = text[src++];

		if (c == '/') {
			src++;
			while ((unsigned char)text[src] > ' ') {
				src++;
			}
			src++;
		} else {
			s_buffer[dst++] = c;
		}
	}

	s_buffer[dst] = '\0';

	return s_buffer;
}

// balloontext.cpp:147
bool ntBalloonText::Process(float elapsed) {
	m_elapsed += elapsed;

	if (m_elapsed < m_duration) {
		return true;
	}
	return false;
}

// balloontext.cpp:157
void ntBalloonText::Render(WView *view) {
	std::string lines[32];

	ntAvatar *avatar = ntAvatarManager::Instance()->FindAvatarByGuid(m_guid);
	if (avatar == NULL) {
		return;
	}

	int x;
	int y;
	if (!avatar->GetBoneScreenCoord("Bip01 HeadNub", &x, &y)) {
		return;
	}

	FrGraphicInterface *device = g_pFresh->GetManager()->GetGDI();
	FrEmoticon *emoticon = g_pFresh->GetManager()->GetEmoticon();

	emoticon->SetAnim(true);

	if (!IsVisible()) {
		if (device != NULL) {
			device->SetScale(1.0f);
		}
		return;
	}

	float scale;
	if (m_elapsed < 0.1f) {
		scale = m_elapsed * 10.0f;
	} else if (m_duration - m_elapsed < 0.1f) {
		scale = (m_duration - m_elapsed) * 10.0f;
	} else {
		scale = 1.0f;
	}

	int lineCount = 0;
	int maxWidth = 0;
	int src = 0;
	int dst = 0;
	bool isLeadByte = false;
	bool hasEmoticon = false;
	int emoticonLength = 0;
	char lineBuffer[1024];
	lineBuffer[0] = '\0';

	int length = strlen(m_text.c_str());

	while (src < length) {
		char c = m_text.c_str()[src++];

		if (c == '\\') {
			char escaped = m_text.c_str()[src++];

			if (escaped == 'n') {
				lineCount++;
				continue;
			}
			if (escaped == '\0') {
				break;
			}
			continue;
		}

		if (c == '(') {
			const char *close = strchr(m_text.c_str() + src, s_emoticonClose);
			unsigned int keyLength = close - src - m_text.c_str();

			if (close != NULL && keyLength < 32) {
				char key[32] = { 0 };
				key[31] = '\0';
				memcpy(key, m_text.c_str() + src, keyLength);

				if (emoticon->GetIconIndex(key) != -1) {
					hasEmoticon = true;
					emoticonLength = keyLength + 2;
				}
			}
		}

		lineBuffer[dst++] = c;

		if ((signed char)c < 0 && !isLeadByte) {
			isLeadByte = true;
			continue;
		}

		lineBuffer[dst] = '\0';
		if (hasEmoticon) {
			if (emoticonLength + dst > 20) {
				int back = (isLeadByte ? 1 : 0) + 1;
				dst -= back;
				src -= back;
				lineBuffer[dst] = '\0';
				lines[lineCount++] = lineBuffer;
				dst = 0;
			}
			hasEmoticon = false;
		} else if (dst > 20) {
			int back = (isLeadByte ? 1 : 0) + 1;
			dst -= back;
			src -= back;
			lineBuffer[dst] = '\0';
			lines[lineCount++] = lineBuffer;
			dst = 0;
		}

		ntBalloonTextManager::Instance()->m_font->SetScale(scale);
		int width = (int)ntBalloonTextManager::Instance()->m_font->GetTextWidth(g_view, lineBuffer);
		if (width > maxWidth) {
			maxWidth = width;
		}

		if (isLeadByte) {
			isLeadByte = false;
		}
	}

	if (lineBuffer[0] != '\0') {
		lines[lineCount++] = lineBuffer;
	}

	int height = (int)(lineCount * scale * 15.0f);

	if (m_textChanged) {
		m_textExtent = x;
		m_textChanged = false;
	}

	if (x < m_textExtent) {
		m_textExtent = x;
	} else if (x > m_textExtent + maxWidth) {
		m_textExtent = x - maxWidth;
	}

	y += (int)((1.0f - scale) * 10.0f);

	unsigned long color = (((((unsigned char)(m_transparency * 255.0f) << 8) | (unsigned char)(m_offset[0] * 255.0f)) << 8 | (unsigned char)(m_offset[1] * 255.0f)) << 8) | (unsigned char)(m_offset[2] * 255.0f);

	int textX = m_textExtent;
	int textY;
	ntBalloonTextManager *manager = ntBalloonTextManager::Instance();
	if (!m_isThink) {
		int frameX = textX - manager->m_leftWidth;
		textY = y - manager->m_bottomHeight - manager->m_frameCenter->GetHeight() - height + 5;

		tagRECT rect;
		rect.left = (int)((frameX - frameX) * scale + frameX);
		rect.top = y - manager->m_bottomHeight - manager->m_frameCenter->GetHeight() - height - manager->m_topHeight + 5;
		rect.right = manager->m_rightWidth + manager->m_leftWidth + maxWidth;
		rect.bottom = manager->m_bottomHeight + height + manager->m_topHeight;
		WOverlay::DrawFrameOverlay9(view, manager->m_frame, rect, color);

		manager = ntBalloonTextManager::Instance();
		int w = manager->m_frameCenter->GetWidth();
		int h = manager->m_frameCenter->GetHeight();
		_WRECT uv;
		uv.x = 0.0f;
		uv.y = 0.0f;
		uv.w = 1.0f;
		uv.h = 1.0f;
		_WRECT pos;
		pos.x = (float)(x - w / 2);
		pos.y = y - h;
		pos.w = (float)w;
		pos.h = (float)h;
		manager->m_frameCenter->RenderWithShear(view, uv, pos, 0.0f, 0x80000, color);
	} else {
		int frameX = textX - manager->m_leftWidth;
		textY = y - manager->m_thinkFrame1->GetHeight() - height - 19;

		tagRECT rect;
		rect.left = (int)((textX - frameX - 24) * scale + frameX);
		rect.top = y - manager->m_thinkFrame1->GetHeight() - height - 43;
		rect.right = maxWidth + 48;
		rect.bottom = height + 48;
		WOverlay::DrawFrameOverlay1(view, manager->m_thinkFrame2, 24.0f, 24.0f, 24.0f, 24.0f, rect, color);

		manager = ntBalloonTextManager::Instance();
		int w = manager->m_thinkFrame1->GetWidth();
		int h = manager->m_thinkFrame1->GetHeight();
		_WRECT uv;
		uv.x = 0.0f;
		uv.y = 0.0f;
		uv.w = 1.0f;
		uv.h = 1.0f;
		_WRECT pos;
		pos.x = (float)(x - w / 2);
		pos.y = y - h;
		pos.w = (float)w;
		pos.h = (float)h;
		manager->m_thinkFrame1->RenderWithShear(view, uv, pos, 0.0f, 0x80000, color);
	}

	for (int i = 0; i < lineCount; i++) {
		if (device != NULL) {
			device->SetScale(scale);
			unsigned long oldColor = device->GetTextColor();
			device->SetTextStyle(0);
			device->SetTextColor((unsigned char)(m_transparency * 255.0f) << 24, 0xFFFFFFFF);
			WPoint pt;
			pt.x = (float)textX;
			pt.y = i * scale * 15.0f + (float)textY;
			emoticon->PrintText(pt, 0, lines[i].c_str(), ((unsigned char)(m_transparency * 255.0f) << 24) | 0xFFFFFF);
			device->SetScale(1.0f);
			device->SetTextColor(oldColor, 0xFFFFFFFF);
		} else {
			ntBalloonTextManager::Instance()->m_font->SetScale(scale);
			ntBalloonTextManager::Instance()->m_font->Print(view, (float)textX, i * scale * 15.0f + (float)textY, lines[i].c_str(), 0, 0xFF000000, NULL);
		}
	}

	ntBalloonTextManager::Instance()->m_font->Flush(view);
}

// balloontext.cpp:472
void ntBalloonTextManager::Init(void) {
	m_font = g_resrcmng->GetFixedFont("fnt_\xb5\xb8\xbf\xf2\xc3\xbc_12.wft");
	if (m_font != NULL) {
		m_font->SetMode(WFont::MASKED);
	}

	for (int i = 0; i < 9; i++) {
		m_frame[i] = g_resrcmng->GetOverlay(MakeStr("chat_frame_%02d.tga", i + 1), 0);
	}

	m_thinkFrame2 = g_resrcmng->GetOverlay("framethink_02.tga", 0);
	m_thinkFrame1 = g_resrcmng->GetOverlay("framethink_01.tga", 0);

	m_leftWidth = m_frame[3]->GetWidth();
	m_topHeight = m_frame[1]->GetHeight();
	m_rightWidth = m_frame[5]->GetWidth();
	m_bottomHeight = m_frame[7]->GetHeight();

	m_frameCenter = g_resrcmng->GetOverlay("chat_frame_00.tga", 0);
}

// balloontext.cpp:500
void ntBalloonTextManager::Shutdown(void) {
	DeleteAll();

	if (g_resrcmng && m_font) { g_resrcmng->Release(m_font); m_font = NULL; }

	for (int i = 0; i < 9; i++) {
		if (g_resrcmng && m_frame[i]) { g_resrcmng->Release(m_frame[i]); m_frame[i] = NULL; }
	}
	if (g_resrcmng && m_frameCenter) { g_resrcmng->Release(m_frameCenter); m_frameCenter = NULL; }

	if (g_resrcmng && m_thinkFrame2) { g_resrcmng->Release(m_thinkFrame2); m_thinkFrame2 = NULL; }
	if (g_resrcmng && m_thinkFrame1) { g_resrcmng->Release(m_thinkFrame1); m_thinkFrame1 = NULL; }
}

// balloontext.cpp:516
void ntBalloonTextManager::DeleteAll(void) {
	for (std::list<ntBalloonText*>::iterator it = m_balloons.begin(); it != m_balloons.end(); ) {
		delete *it;
		it = m_balloons.erase(it);
	}
	m_balloons.clear();
}

// balloontext.cpp:529
void ntBalloonTextManager::Process(float elapsed) {
	for (std::list<ntBalloonText*>::iterator it = m_balloons.begin(); it != m_balloons.end(); ++it) {
		(*it)->Process(elapsed);
	}
}

// balloontext.cpp:539
void ntBalloonTextManager::Render(void) {
	for (std::list<ntBalloonText*>::iterator it = m_balloons.begin(); it != m_balloons.end(); ++it) {
		(*it)->Render(g_view);
	}
}

// balloontext.cpp:549
ntBalloonText *ntBalloonTextManager::CreateBalloonText(unsigned long guid, const char *text, const float * const offset, float duration) {
	ntBalloonText *balloon = new ntBalloonText(guid, text, offset, duration);
	m_balloons.push_back(balloon);

	return balloon;
}

// balloontext.cpp:556
void ntBalloonTextManager::DeleteBalloonText(ntBalloonText *balloon) {
	std::list<ntBalloonText*>::iterator it = std::find(m_balloons.begin(), m_balloons.end(), balloon);
	if (it != m_balloons.end()) {
		m_balloons.erase(it);
		delete balloon;
	}
}
