// Original: d:\build\custom\temp\code\client\fresh\source\frgraphicinterface.cpp
#include <stdarg.h>
#include <stdio.h>
#include <vector>
#include "../client/Fresh/include/frgraphicinterface.h"
#include "../client/Fresh/include/frtext.h"
#include "../client/Fresh/texturecache/tcachemanager.h"
#include "../client/Wangreal/include/wfont.h"
#include "../client/Wangreal/include/wresrcmng.h"

// defined in frtooltip.cpp
extern const float TEXT_TOOLTIP_LINE_INTERVAL;

// frgraphicinterface.cpp:26
FrGraphicInterface::FrGraphicInterface(WView* pView)
	: m_pView(pView)
	, m_pCacheManager(NULL)
	, m_pFont12(NULL)
	, m_maxTexCache(32)
	, m_cacheTexSize(256) {
	m_pCacheManager = new TexCacheManager;
	if (m_pCacheManager) {
		m_pCacheManager->Init(m_maxTexCache, m_cacheTexSize, m_cacheTexSize, 4, 4);
	}

	m_pFont12 = g_resrcmng->GetFixedFont("fnt_\xb5\xb8\xbf\xf2\xc3\xbc_12.wft");
	if (m_pFont12) {
		m_pFont12->SetMode(WFont::MASKED);
	}

	m_pFont11 = g_resrcmng->GetFixedFont("fnt_\xb5\xb8\xbf\xf2\xc3\xbc_11.wft");
	if (m_pFont11) {
		m_pFont11->SetMode(WFont::MASKED);
	}

	m_textStyle = 0; m_textColor = 0xff000000; m_textOutlineColor = 0xffffffff; m_alpha = 1.0f;
}

// frgraphicinterface.cpp:77
FrGraphicInterface::~FrGraphicInterface(void) {
	if (m_pCacheManager) { delete m_pCacheManager; m_pCacheManager = NULL; }
	if (g_resrcmng && m_pFont12) { g_resrcmng->Release(m_pFont12); m_pFont12 = NULL; }
	if (g_resrcmng && m_pFont11) { g_resrcmng->Release(m_pFont11); m_pFont11 = NULL; }
}

// frgraphicinterface.cpp:86
void FrGraphicInterface::DrawTexture(const Bitmap* pBitmap, const WRect& dest, unsigned long diffuse, int type) const {
	if (pBitmap == NULL) {
		return;
	}

	const sTexCacheInfo* pInfo = m_pCacheManager->Draw(*pBitmap);
	if (pInfo == NULL) {
		return;
	}

	WRect r(pInfo->rcPixel.left / (float)m_cacheTexSize, pInfo->rcPixel.top / (float)m_cacheTexSize, pInfo->rcPixel.Width() / (float)m_cacheTexSize, pInfo->rcPixel.Height() / (float)m_cacheTexSize);
	m_pView->Draw2DTexture(r, dest, pInfo->texHandle, FrALPHA(diffuse, m_alpha), type);
}

// frgraphicinterface.cpp:104
void FrGraphicInterface::DrawTexture(const Bitmap* pBitmap, const WRect& src, const WRect& dest, unsigned long diffuse, int type) const {
	if (pBitmap == NULL) {
		return;
	}

	const sTexCacheInfo* pInfo = m_pCacheManager->Draw(*pBitmap);
	if (pInfo == NULL) {
		return;
	}

	WRect r((pInfo->rcPixel.left + src.x) / m_cacheTexSize, (pInfo->rcPixel.top + src.y) / m_cacheTexSize, src.w * (1.0f / m_cacheTexSize), src.h * (1.0f / m_cacheTexSize));
	m_pView->Draw2DTexture(r, dest, pInfo->texHandle, FrALPHA(diffuse, m_alpha), type);
}

// frgraphicinterface.cpp:124
void FrGraphicInterface::UpdateTextureCacheInfo(const Bitmap* pBitmap) {
	const sTexCacheInfo* pInfo = m_pCacheManager->Draw(*pBitmap);

	if (pInfo) {
		m_pCacheManager->UpdateTextureCacheInfo(*pInfo);
	}
}

// frgraphicinterface.cpp:134
void FrGraphicInterface::RefreshTextureCacheInfo(const Bitmap* pBitmap) {
	m_pCacheManager->RefreshTexCache(*pBitmap);
}

// frgraphicinterface.cpp:139
float __cdecl FrGraphicInterface::Print(const WPoint& pos, unsigned long align, const char* format, ...) {
	char buf[2048];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buf, sizeof(buf) - 1, format, args) == -1)
		buf[sizeof(buf) - 1] = '\0';

	return PrintText(pos, align, buf, NULL);
}

// frgraphicinterface.cpp:149
float __cdecl FrGraphicInterface::Print11(const WPoint& pos, unsigned long align, const char* format, ...) {
	char buf[2048];
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buf, sizeof(buf) - 1, format, args) == -1)
		buf[sizeof(buf) - 1] = '\0';

	return PrintText11(pos, align, buf, NULL);
}

// frgraphicinterface.cpp:159
float FrGraphicInterface::PrintText(const WPoint& pos, unsigned long align, const char* text, Bitmap* bmp) {
	WPoint p;
	float w;

	switch (align) {
	case 0:
		p = pos;
		break;
	case 1:
		p.x = pos.x - m_pFont12->GetTextWidth(m_pView, text) * 0.5f;
		p.y = pos.y;
		break;
	case 2:
		w = m_pFont12->GetTextWidth(m_pView, text); p.x = pos.x - w; p.y = pos.y;
		break;
	}

	switch (m_textStyle) {
	case 1:
		m_pFont12->Print(m_pView, p.x + 1.0f, p.y, text, 0, FrALPHA(m_textColor, m_alpha), bmp);
		break;
	case 2: {
		// outline offsets, one pixel around the text
		static int opix[8][2] = { { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };
		for (int i = 0; i < 8; i++) {
			m_pFont12->Print(m_pView, p.x + opix[i][0], p.y + opix[i][1], text, 0, FrALPHA(m_textOutlineColor, m_alpha), bmp);
		}
		break;
	}
	case 4:
		m_pFont12->Print(m_pView, p.x + 1.0f, p.y + 1.0f, text, 0, FrALPHA(m_textOutlineColor, m_alpha), bmp);
		break;
	}

	w = m_pFont12->Print(m_pView, p.x, p.y, text, 0x80000, FrALPHA(m_textColor, m_alpha), bmp);
	if (bmp == NULL) {
		m_pFont12->Flush(m_pView);
	}

	return w;
}

// frgraphicinterface.cpp:220
void FrGraphicInterface::PrintText(const WPoint& pos, const FrTEXT& text) {
	std::list<FrTextToken> txToken;
	text.GetTokenList(txToken);

	WPoint p;
	p = pos;
	for (std::list<FrTextToken>::iterator itr = txToken.begin(); itr != txToken.end(); ++itr) {
		FrTextToken& token = *itr;
		if (token.IsTag()) {
			token.RunTag(this);
			continue;
		}

		if (token.Text() == "\n") {
			p.x = pos.x;
			p.y += m_pFont12->GetFontHeight();
			continue;
		}
		p.x += PrintText(p, 0, token.Text().c_str(), 0);
	}
}

// frgraphicinterface.cpp:245
float FrGraphicInterface::PrintText11(const WPoint& pos, unsigned long align, const char* text, Bitmap* bmp) {
	WPoint p;
	float w;

	switch (align) {
	case 0:
		p = pos;
		break;
	case 1:
		p.x = pos.x - m_pFont11->GetTextWidth(m_pView, text) * 0.5f;
		p.y = pos.y;
		break;
	case 2:
		w = m_pFont11->GetTextWidth(m_pView, text); p.x = pos.x - w; p.y = pos.y;
		break;
	}

	switch (m_textStyle) {
	case 1:
		m_pFont11->Print(m_pView, p.x + 1.0f, p.y, text, 0, FrALPHA(m_textColor, m_alpha), bmp);
		break;
	case 2: {
		// outline offsets, one pixel around the text
		static int opix[8][2] = { { 0, -1 }, { -1, 0 }, { 1, 0 }, { 0, 1 }, { -1, -1 }, { 1, -1 }, { -1, 1 }, { 1, 1 } };
		for (int i = 0; i < 8; i++) {
			m_pFont11->Print(m_pView, p.x + opix[i][0], p.y + opix[i][1], text, 0, FrALPHA(m_textOutlineColor, m_alpha), bmp);
		}
		break;
	}
	case 4:
		m_pFont11->Print(m_pView, p.x + 1.0f, p.y + 1.0f, text, 0, FrALPHA(m_textOutlineColor, m_alpha), bmp);
		break;
	}

	w = m_pFont11->Print(m_pView, p.x, p.y, text, 0x80000, FrALPHA(m_textColor, m_alpha), bmp);
	if (bmp == NULL) {
		m_pFont11->Flush(m_pView);
	}

	return w;
}

// frgraphicinterface.cpp:306
void FrGraphicInterface::PrintText11(const WPoint& pos, const FrTEXT& text) {
	std::list<FrTextToken> txToken;
	text.GetTokenList(txToken);

	WPoint p;
	p = pos;
	for (std::list<FrTextToken>::iterator itr = txToken.begin(); itr != txToken.end(); ++itr) {
		FrTextToken& token = *itr;
		if (token.IsTag()) {
			token.RunTag(this);
			continue;
		}

		if (token.Text() == "\n") {
			p.x = pos.x;
			p.y += m_pFont11->GetFontHeight() + TEXT_TOOLTIP_LINE_INTERVAL;
			continue;
		}
		p.x += PrintText11(p, 0, token.Text().c_str(), 0);
	}
}

// frgraphicinterface.cpp:332
void FrGraphicInterface::PrintText(const WPoint& pos, unsigned long align, const FrTEXT& text) {
	std::list<FrTextToken> txToken;
	text.GetTokenList(txToken);

	std::vector<float> vecLineWidth;

	float lenth = 0.0f;
	for (std::list<FrTextToken>::iterator itr = txToken.begin(); itr != txToken.end(); ++itr) {
		FrTextToken& token = *itr;
		if (token.IsTag())
			continue;

		if (token.Text() == "\n") {
			vecLineWidth.push_back(lenth);
			lenth = 0.0f;
			continue;
		}

		lenth += m_pFont12->GetTextWidth(m_pView, token.Text().c_str());
	}
	vecLineWidth.push_back(lenth);

	std::vector<float>::iterator itLine = vecLineWidth.begin();
	WPoint p;
	p = pos;

	switch (align) {
	case 1:
		p.x -= *itLine * 0.5f;
		break;
	case 2:
		p.x -= *itLine;
		break;
	}

	std::list<FrTextToken>::iterator itToken = txToken.begin();
	if (itToken != txToken.end()) {
		++itLine;
		for (; itToken != txToken.end(); ++itToken) {
			FrTextToken& token = *itToken;
			if (token.IsTag()) {
				token.RunTag(this);
				continue;
			}

			if (token.Text() == "\n") {
				p.x = pos.x;

				switch (align) {
				case 1:
					p.x -= *itLine * 0.5f;
					break;
				case 2:
					p.x -= *itLine;
					break;
				}

				p.y += m_pFont12->GetFontHeight();
				++itLine;
				continue;
			}
			p.x += PrintText(p, 0, token.Text().c_str(), 0);
		}
	}
}

// frgraphicinterface.cpp:412
void FrGraphicInterface::PrintText11(const WPoint& pos, unsigned long align, const FrTEXT& text) {
	std::list<FrTextToken> txToken;
	text.GetTokenList(txToken);

	std::vector<float> vecLineWidth;

	float lenth = 0.0f;
	for (std::list<FrTextToken>::iterator itr = txToken.begin(); itr != txToken.end(); ++itr) {
		FrTextToken& token = *itr;
		if (token.IsTag())
			continue;

		if (token.Text() == "\n") {
			vecLineWidth.push_back(lenth);
			lenth = 0.0f;
			continue;
		}

		lenth += m_pFont12->GetTextWidth(m_pView, token.Text().c_str());
	}
	vecLineWidth.push_back(lenth);

	std::vector<float>::iterator itLine = vecLineWidth.begin();
	WPoint p;
	p = pos;

	switch (align) {
	case 1:
		p.x -= *itLine * 0.5f;
		break;
	case 2:
		p.x -= *itLine;
		break;
	}

	std::list<FrTextToken>::iterator itToken = txToken.begin();
	if (itToken != txToken.end()) {
		++itLine;
		for (; itToken != txToken.end(); ++itToken) {
			FrTextToken& token = *itToken;
			if (token.IsTag()) {
				token.RunTag(this);
				continue;
			}

			if (token.Text() == "\n") {
				p.x = pos.x;

				switch (align) {
				case 1:
					p.x -= *itLine * 0.5f;
					break;
				case 2:
					p.x -= *itLine;
					break;
				}

				p.y += m_pFont12->GetFontHeight();
				++itLine;
				continue;
			}
			p.x += PrintText11(p, 0, token.Text().c_str(), 0);
		}
	}
}

// frgraphicinterface.cpp:492
float FrGraphicInterface::GetTextExtend(const char* lpszText) {
	return m_pFont12->GetTextWidth(m_pView, lpszText);
}

// frgraphicinterface.cpp:497
float FrGraphicInterface::GetTextExtend11(const char* lpszText) {
	return m_pFont11->GetTextWidth(m_pView, lpszText);
}

// frgraphicinterface.cpp:502
int FrGraphicInterface::GetFontHeight(void) {
	return m_pFont12->GetFontHeight();
}

// frgraphicinterface.cpp:507
int FrGraphicInterface::GetFontHeight11(void) {
	return m_pFont11->GetFontHeight();
}

// frgraphicinterface.cpp:512
void FrGraphicInterface::SetTextColor(unsigned long color, unsigned long outlineColor) {
	m_textColor = color;
	m_textOutlineColor = outlineColor;
}

// frgraphicinterface.cpp:518
void FrGraphicInterface::SetTextStyle(unsigned long textStyle) {
	m_textStyle = textStyle;
}

// frgraphicinterface.cpp:523
int FrGraphicInterface::SetSpace(int space) {
	int oldSpace = m_pFont12->GetSpace();
	m_pFont12->SetSpace(space);
	return oldSpace;
}

// frgraphicinterface.cpp:530
int FrGraphicInterface::SetSpace11(int space) {
	int oldSpace = m_pFont11->GetSpace();
	m_pFont11->SetSpace(space);
	return oldSpace;
}

// frgraphicinterface.cpp:537
void FrGraphicInterface::SetClippingArea(WRect* clip) {
	m_pFont12->SetClippingArea(clip);
}

// frgraphicinterface.cpp:542
void FrGraphicInterface::SetClippingArea11(WRect* clip) {
	m_pFont11->SetClippingArea(clip);
}

// frgraphicinterface.cpp:547
void FrGraphicInterface::SetAlpha(float alpha) {
	m_alpha = alpha;
}

// frgraphicinterface.cpp:552
float FrGraphicInterface::GetAlpha(void) {
	return m_alpha;
}

// frgraphicinterface.cpp:557
void FrGraphicInterface::Reset(void) {
	if (m_pCacheManager) {
		m_pCacheManager->ClearAll();
	}

	m_alpha = 1.0f;
}

// frgraphicinterface.cpp:565
void FrGraphicInterface::SetScale(float scale) {
	m_pFont12->SetScale(scale);
}

// frgraphicinterface.cpp:570
void FrGraphicInterface::SetScale11(float scale) {
	m_pFont11->SetScale(scale);
}

// frgraphicinterface.cpp:602
void FrGraphicInterface::RefreshTexCache(const Bitmap& bitmap) {
	if (m_pCacheManager) {
		m_pCacheManager->RefreshTexCache(bitmap);
	}
}

// frgraphicinterface.cpp:610
void FrGraphicInterface::InvalidateCache(const Bitmap& bitmap) {
	if (m_pCacheManager) {
		m_pCacheManager->InvalidateCache(bitmap);
	}
}
