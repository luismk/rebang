// Original: d:\build\custom\temp\code\client\fresh\source\frtextbutton.cpp
#include <stdio.h>
#include <string.h>

#include "frtextbutton.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "../client/ProjectG/binstr.h"

// frtextbutton.cpp:16 (run-time type and factory registration of FrTextButton)
const WRTTI FrTextButton::m_RTTI("FrTextButton", &FrWnd::m_RTTI);

IObject* FrTextButtonMakeInstance(void) {
	return new FrTextButton;
}

struct __sFrTextButton {
	__sFrTextButton(void) {
		ObjectFactory().AddObjectFunctor(FrTextButtonMakeInstance, "FrTextButton");
	}
};
static __sFrTextButton s_registerFrTextButton;

// frtextbutton.cpp:19
FrTextButton::FrTextButton(void) {
	m_style = 1;
	m_color = 0xffffffff;
	m_outlineColor = 0xff000000;
	m_center = true;
	m_selStyle = 1;
	m_selColor = 0xffffcc00;
	m_selOutlineColor = 0xff000000;
	m_clip = 0;
	m_hlAlpha = 0.0f;
	m_hlAlphaD = 1;
	m_szPushSound = "ui_icon_click";
	m_sepWidth = 5.0f;
	m_underCursor = m_selected = m_tBtnList.end();
}

// frtextbutton.cpp:40
FrTextButton::~FrTextButton(void) {
}

// frtextbutton.cpp:45
void FrTextButton::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	pManager->GetDocument();

	std::map<std::string, std::string>& param = item.m_param;

	if (param.find("style") != param.end())
		sscanf(param["style"].c_str(), "%x", &m_style);

	if (param.find("center") != param.end())
		m_center = strcmpi("true", param["center"].c_str()) == 0;

	if (param.find("color") != param.end())
		sscanf(param["color"].c_str(), "%x", &m_color);

	if (param.find("color2") != param.end())
		sscanf(param["color2"].c_str(), "%x", &m_outlineColor);

	if (param.find("sel_style") != param.end())
		sscanf(param["sel_style"].c_str(), "%x", &m_selStyle);

	if (param.find("select_color") != param.end())
		sscanf(param["select_color"].c_str(), "%x", &m_selColor);

	if (param.find("select_color2") != param.end())
		sscanf(param["select_color2"].c_str(), "%x", &m_selOutlineColor);

	_RectangleSHORT r = item.m_rect;
	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 1, WRect(r.left, r.top, r.Width(), r.Height()), pParent);

	SetCaption(item.m_caption.c_str());
}

// frtextbutton.cpp:83
void FrTextButton::SetCaption(const char* caption) {
	m_caption = caption;

	if (m_center) {
		float extent = GDI()->GetTextExtend(caption);
		m_rect.x += (int)(((m_rect.w - extent) + 1.0f) * 0.5f);
		m_rect.w = extent;
	}

	m_tBtnList.clear();

	m_underCursor = m_selected = m_tBtnList.end();

	cTokenV token;
	token.Init(caption, strlen(caption));

	float offset = 0.0f;
	static const char* pSep = (0, "|");
	m_sepWidth = GDI()->GetTextExtend(pSep);

	for (;;) {
		char* text;
		token.GetToken(&text, pSep, 1);

		if (*text == 0)
			break;

		TButtonItem item;
		item.selected = false;
		item.underCursor = false;
		item.text = text;
		item.left = offset;
		float right = GDI()->GetTextExtend(text) + offset;
		m_rect.w = right;
		item.right = right;
		offset = right + m_sepWidth;
		m_tBtnList.push_back(item);
	}
}

// frtextbutton.cpp:131
const char* FrTextButton::GetCaption(void) {
	return m_caption.c_str();
}

// frtextbutton.cpp:137
void FrTextButton::SetClippingArea(WRect* clip) {
	if (clip) {
		if (!m_clip) {
			m_clip = new WRect(*clip);
		} else {
			m_clip->x = clip->x;
			m_clip->y = clip->y;
			m_clip->w = clip->w;
			m_clip->h = clip->h;
		}
	} else {
		if (m_clip) { delete m_clip; m_clip = 0; }
	}
}

// frtextbutton.cpp:156
const char* FrTextButton::GetUnderCursor(void) {
	if (m_underCursor == m_tBtnList.end())
		return 0;

	return m_underCursor->text.c_str();
}

// frtextbutton.cpp:165
const char* FrTextButton::GetSelected(void) {
	if (m_selected == m_tBtnList.end())
		return 0;

	return m_selected->text.c_str();
}

// frtextbutton.cpp:174
void FrTextButton::Select(int index) {
	TBUTTON_LIST::iterator it;
	for (it = m_tBtnList.begin(); it != m_tBtnList.end() && index > 0; ++it, --index);

	if (it == m_tBtnList.end())
		--it;

	Select(it);
}

// frtextbutton.cpp:204
void FrTextButton::Unselect(int index) {
	TBUTTON_LIST::iterator it;
	for (it = m_tBtnList.begin(); it != m_tBtnList.end() && index > 0; ++it, --index);

	if (it == m_tBtnList.end())
		--it;

	Unselect(it);
}

// frtextbutton.cpp:231
void FrTextButton::Select(TBUTTON_LIST::iterator it) {
	if (m_selected != m_tBtnList.end())
		m_selected->selected = false;

	m_selected = it;

	if (m_selected != m_tBtnList.end())
		m_selected->selected = true;
}

// frtextbutton.cpp:243
void FrTextButton::Unselect(TBUTTON_LIST::iterator it) {
	if (it != m_tBtnList.end())
		it->selected = false;

	if (m_selected == it)
		m_selected = m_tBtnList.end();
}

// frtextbutton.cpp:253
void FrTextButton::OnDraw(void) {
	FrWnd::OnDraw();

	if (m_clip)
		GDI()->SetClippingArea(m_clip);

	TBUTTON_LIST::iterator it = m_tBtnList.begin();
	while (it != m_tBtnList.end()) {
		TButtonItem& item = *it;
		WPoint left = WPoint(m_rect.x, m_rect.y) + WPoint(item.left, 0.0f);
		WPoint right = WPoint(m_rect.x, m_rect.y) + WPoint(item.right, 0.0f);

		if (item.selected && !item.underCursor) {
			GDI()->SetTextStyle(m_selStyle);
			GDI()->SetTextColor(FrALPHA(m_selColor, m_hlAlpha < 1.0f ? m_hlAlpha : 1.0f),
				FrALPHA(m_selOutlineColor, m_hlAlpha < 1.0f ? m_hlAlpha : 1.0f));
		} else if (item.underCursor) {
			GDI()->SetTextStyle(m_selStyle);
			GDI()->SetTextColor(FrALPHA(m_selColor, m_wndAlpha), FrALPHA(m_selOutlineColor, m_wndAlpha));
		} else {
			GDI()->SetTextStyle(m_style);
			GDI()->SetTextColor(FrALPHA(m_color, m_wndAlpha), FrALPHA(m_outlineColor, m_wndAlpha));
		}

		GDI()->Print(left, 0, item.text.c_str());

		GDI()->SetTextStyle(m_style);
		GDI()->SetTextColor(FrALPHA(m_color, m_wndAlpha), FrALPHA(m_outlineColor, m_wndAlpha));

		++it;
		if (it == m_tBtnList.end())
			break;

		static const char* pSep = (0, "|");
		GDI()->Print(right, 0, pSep);
	}

	if (m_clip)
		GDI()->SetClippingArea(0);
}

// frtextbutton.cpp:303
void FrTextButton::OnProc(const float deltaTime) {
	m_hlAlpha += m_hlAlphaD * deltaTime * 2.0f;

	if (m_hlAlpha < 0.0f) {
		m_hlAlpha = 0.0f;
		m_hlAlphaD = 1;
	} else if (m_hlAlpha > 3.0f) {
		m_hlAlpha = 3.0f;
		m_hlAlphaD = -1;
	}
}

// frtextbutton.cpp:320
void FrTextButton::OnMouseMove(const WPoint& mousePos) {
	m_underCursor = m_tBtnList.end();

	for (TBUTTON_LIST::iterator it = m_tBtnList.begin(); it != m_tBtnList.end(); ++it) {
		if (it->left + m_rect.x <= mousePos.x && m_sepWidth + it->right + m_rect.x > mousePos.x) {
			m_underCursor = it;
			it->underCursor = true;
		} else {
			it->underCursor = false;
		}
	}
}

// frtextbutton.cpp:341
void FrTextButton::OnSetCursor(bool bInClient, const WPoint& mousePos) {
	if (bInClient) {
		if (m_underCursor != m_tBtnList.end())
			SetCursor(1);
	} else {
		if (m_underCursor != m_tBtnList.end()) {
			m_underCursor->underCursor = false;
			m_underCursor = m_tBtnList.end();
		}
	}
}

// frtextbutton.cpp:361
bool FrTextButton::OnLButtonUp(const WPoint& mousePos) {
	if (m_underCursor->selected)
		return false;

	PlayPushSound();

	OnMouseMove(mousePos);
	Select(m_underCursor);
	SendCmdToOwnerTarget(FRCMD_LBUTTONUP, 0, 0);

	return true;
}
