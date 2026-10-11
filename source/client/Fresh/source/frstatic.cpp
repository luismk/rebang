// Original: d:\build\custom\temp\code\client\fresh\source\frstatic.cpp
#include <stdlib.h>

#include "frstatic.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"

// commonutil.cpp (client/ProjectG/commonutil); no header of ours declares it.
unsigned long htoi(const char* str);

// frstatic.cpp:18 (run-time type and factory registration of FrStatic)
const WRTTI FrStatic::m_RTTI("FrStatic", &FrWnd::m_RTTI);

IObject* FrStaticMakeInstance(void) {
	return new FrStatic;
}

struct __sFrStatic {
	__sFrStatic(void) {
		ObjectFactory().AddObjectFunctor(FrStaticMakeInstance, "FrStatic");
	}
};
static __sFrStatic s_registerFrStatic;

// frstatic.cpp:21
FrStatic::FrStatic(void) {
	m_color = 0xff808080;
	m_outlineColor = 0xffffffff;
	m_style = 0;
	m_align = 0;
	m_clip = 0;
	m_smallFont = false;
}

// frstatic.cpp:31
FrStatic::~FrStatic(void) {
}

// frstatic.cpp:35
void FrStatic::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrElementDoc* pDoc = pManager->GetDocument();

	m_caption = item.m_caption;

	std::map<std::string, std::string>& param = item.m_param;
	if (param.find("style") != param.end())
		m_style = atoi(param["style"].c_str());

	if (param.find("align") != param.end())
		m_align = atoi(param["align"].c_str());

	if (param.find("color") != param.end())
		m_color = htoi(param["color"].c_str());

	if (m_style & 0x6)
		m_outlineColor = htoi(param["color2"].c_str());

	_RectangleSHORT r = item.m_rect;
	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 1, WRect(r.left, r.top, r.Width(), r.Height()), pParent);
}

// frstatic.cpp:61
void FrStatic::SetClippingArea(WRect* clip) {
	if (clip) {
		if (m_clip == 0) {
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

// frstatic.cpp:79
void FrStatic::SetTextStyle(unsigned long style) {
	m_style = style;
}

// frstatic.cpp:84
void FrStatic::SetTextColor(unsigned long color, unsigned long outline_color) {
	m_color = color;
	m_outlineColor = outline_color;
}

// frstatic.cpp:90
void FrStatic::OnDraw(void) {
	FrWnd::OnDraw();

	if (m_pWndManager->HidePrivacy() && m_nFlags.GetFlag(0x1000))
		return;

	if (m_clip)
		GDI()->SetClippingArea(m_clip);

	if (IsEnabled())
		GDI()->SetTextColor(FrALPHA(m_color, m_wndAlpha2 * m_wndAlpha), FrALPHA(m_outlineColor, m_wndAlpha2 * m_wndAlpha));
	else
		GDI()->SetTextColor(FrALPHA(m_color, 0.4f), 0xffffffff);

	GDI()->SetTextStyle(m_style);
	FrWndManager* pWndManager = m_pWndManager;
	if (m_smallFont)
		pWndManager->PrintText11(WPoint(m_rect.x, m_rect.y), m_align, m_caption.c_str(), -1.0f, 0xffffffff);
	else
		pWndManager->PrintText(WPoint(m_rect.x, m_rect.y), m_align, m_caption.c_str(), -1.0f, 0xffffffff);

	if (m_clip)
		GDI()->SetClippingArea(0);
}

// frstatic.cpp:115
void FrStatic::Enable(bool enable) {
	if (!enable)
		m_dwStyle.Enable(0x2);
	else
		m_dwStyle.Disable(0x2);
}
