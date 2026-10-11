// Original: d:\build\custom\temp\code\client\fresh\source\frgaugebar.cpp
#include "frgaugebar.h"
#include "frbutton.h"
#include "frwndmanager.h"
#include "frelement.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/wutil.h"

// frgaugebar.cpp:16 (run-time type and factory registration of FrGaugeBar)
const WRTTI FrGaugeBar::m_RTTI("FrGaugeBar", &FrWnd::m_RTTI);

IObject* FrGaugeBarMakeInstance(void) {
	return new FrGaugeBar;
}

struct __sFrGaugeBar {
	__sFrGaugeBar(void) {
		ObjectFactory().AddObjectFunctor(FrGaugeBarMakeInstance, "FrGaugeBar");
	}
};
static __sFrGaugeBar s_registerFrGaugeBar;

// frgaugebar.cpp:23
FrGaugeBar::FrGaugeBar(void) {
	m_color = 0xffffffff;
	m_bgColor = 0xff404040;
	m_exColor[0] = 0xff707070;
	m_exColor[1] = 0xff707070;
	m_thickness = 2;
	m_lower = 0;
	m_upper = 100;
	m_origin = 0;
	m_pos = m_expand = 0;
	m_destPos = m_srcPos = 0;
	m_speed = 65.0f;

	CalcBarRect(0);
}

// frgaugebar.cpp:41
FrGaugeBar::~FrGaugeBar(void) {
}

// frgaugebar.cpp:46
void FrGaugeBar::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	pManager->GetDocument();

	m_pItem = &item;

	std::map<std::string, std::string>& param = item.m_param;
	if (param.find("color") != param.end())
		sscanf(param["color"].c_str(), "%x", &m_color);

	if (param.find("bgcolor") != param.end())
		sscanf(param["bgcolor"].c_str(), "%x", &m_bgColor);

	if (param.find("excolor1") != param.end())
		sscanf(param["excolor1"].c_str(), "%x", &m_exColor[0]);

	if (param.find("excolor2") != param.end())
		sscanf(param["excolor2"].c_str(), "%x", &m_exColor[1]);

	if (param.find("border_thickness") != param.end())
		sscanf(param["border_thickness"].c_str(), "%d", &m_thickness);

	WRect rect((float)item.m_rect.left, (float)item.m_rect.top,
		(short)(item.m_rect.right - item.m_rect.left),
		(short)(item.m_rect.bottom - item.m_rect.top));

	Create(item.m_caption.c_str(), item.m_name.c_str(), pManager, 1, rect, pParent);
}

// frgaugebar.cpp:76
void FrGaugeBar::SetRange(int lower, int upper, int origin) {
	m_lower = lower;
	m_upper = upper;

	if (origin < lower) {
		m_origin = lower;
	} else if (origin > upper) {
		m_origin = upper;
	} else {
		m_origin = origin;
	}

	if (m_pos > upper) {
		m_pos = (float)upper;
	} else if (m_pos < lower) {
		m_pos = (float)lower;
	}

	if (m_destPos > upper) {
		m_destPos = (float)upper;
	} else if (m_destPos < lower) {
		m_destPos = (float)lower;
	}

	CalcBarRect(0);
}

// frgaugebar.cpp:109
void FrGaugeBar::SetPos(int pos) {
	if (pos > m_upper) {
		m_pos = (float)m_upper;
	} else if (pos < m_lower) {
		m_pos = (float)m_lower;
	} else {
		m_pos = (float)pos;
	}

	m_destPos = m_pos;
	CalcBarRect(0);
}

// frgaugebar.cpp:124
void FrGaugeBar::SetExpand(int expand) {
	m_expand = (float)expand;
}

// frgaugebar.cpp:129
void FrGaugeBar::SetDestPos(int pos, bool achillesRun) {
	m_achillesRun = achillesRun;

	if (pos > m_upper) {
		m_destPos = (float)m_upper;
	} else if (pos < m_lower) {
		m_destPos = (float)m_lower;
	} else {
		m_destPos = (float)pos;
	}
}

// frgaugebar.cpp:143
void FrGaugeBar::MoveWindow(const WPoint& pos) {
	FrWnd::MoveWindow(pos);

	m_barRect.x = m_rect.x + m_thickness;
	m_barRect.y = m_rect.y + m_thickness;
}

// frgaugebar.cpp:151
void FrGaugeBar::CalcBarRect(const WRect* pRect) {
	if (pRect == 0)
		m_barRect = m_rect;
	else
		m_barRect = *pRect;

	float ratio = (m_barRect.w - m_thickness * 2) / (m_upper - m_lower);

	if (m_origin < m_pos) {
		m_barRect.x += (m_origin - m_lower) * ratio;
		m_barRect.w = (m_pos - m_origin) * ratio;
	} else {
		m_barRect.x += (m_pos - m_lower) * ratio;
		m_barRect.w = (m_origin - m_pos) * ratio;
	}

	m_barRect.x += m_thickness;
	m_barRect.y += m_thickness;
	m_barRect.h -= m_thickness * 2;
}

// frgaugebar.cpp:177
void FrGaugeBar::OnProc(const float delta) {
	float step;
	if (m_achillesRun && m_destPos != m_srcPos)
		step = (m_destPos - m_pos) * m_speed * delta / (m_destPos - m_srcPos);
	else
		step = delta * m_speed;

	if (m_pos > m_destPos) {
		float pos = m_pos - step;
		m_pos = pos > m_destPos ? pos : m_destPos;
	} else if (m_pos < m_destPos) {
		float pos = m_pos + step;
		m_pos = pos < m_destPos ? pos : m_destPos;
	}

	CalcBarRect(0);
}

// frgaugebar.cpp:202
void FrGaugeBar::OnDraw(void) {
	FrWnd::OnDraw();

	GDI()->Box(m_rect, m_bgColor, 0, 0);

	float expand = Abs(m_expand * m_rect.w / (m_upper - m_lower));

	if (m_expand > 0) {
		expand = Between(0.0f, expand, m_rect.x + m_rect.w - m_thickness - (m_barRect.x + m_barRect.w));

		GDI()->Box(m_barRect, m_color, 0, 0);
		GDI()->Box(WRect(m_barRect.x + m_barRect.w, m_barRect.y, expand, m_barRect.h), m_exColor[0], 0, 0);
	} else if (m_expand < 0) {
		expand = Min(m_barRect.w, expand);

		GDI()->Box(WRect(m_barRect.x, m_barRect.y, m_barRect.w - expand, m_barRect.h), m_color, 0, 0);
		GDI()->Box(WRect(m_barRect.x + m_barRect.w - expand, m_barRect.y, expand, m_barRect.h), m_exColor[1], 0, 0);
	} else {
		GDI()->Box(m_barRect, m_color, 0, 0);
	}
}

// frgaugebar.cpp:231 (run-time type and factory registration of FrGaugeBarEx)
const WRTTI FrGaugeBarEx::m_RTTI("FrGaugeBarEx", &FrGaugeBar::m_RTTI);

IObject* FrGaugeBarExMakeInstance(void) {
	return new FrGaugeBarEx;
}

struct __sFrGaugeBarEx {
	__sFrGaugeBarEx(void) {
		ObjectFactory().AddObjectFunctor(FrGaugeBarExMakeInstance, "FrGaugeBarEx");
	}
};
static __sFrGaugeBarEx s_registerFrGaugeBarEx;

// frgaugebar.cpp:234
const sFRESH_MSGMAP* FrGaugeBarEx::GetMessageMap(void) const {
	return &FrGaugeBarEx::_MsgMap;
}

sFRESH_MSGMAP FrGaugeBarEx::_MsgMap = { &FrCmdTarget::_MsgMap, &FrGaugeBarEx::_MsgEntries[0] };

sFRESH_ENTRY FrGaugeBarEx::_MsgEntries[] = {
	// frgaugebar.cpp:236
	{ "left", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGaugeBarEx::OnLLButtonDown },
	// frgaugebar.cpp:237
	{ "right", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrGaugeBarEx::OnRLButtonDown },

	// frgaugebar.cpp:239
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// frgaugebar.cpp:246
FrGaugeBarEx::FrGaugeBarEx(void) {
	m_borderColor = 0xff808080;
	m_color2 = 0xffffffff;
	m_thickness = 3;
	m_fPos = 0;
	m_steps = 1;
	m_pLButton = 0;
	m_pRButton = 0;

	CalcBarRect(0);
}

// frgaugebar.cpp:259
FrGaugeBarEx::~FrGaugeBarEx(void) {
}

// frgaugebar.cpp:263
void FrGaugeBarEx::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrGaugeBar::Init(item, pManager, pParent);

	FrElementDoc* pDoc = pManager->GetDocument();

	std::map<std::string, std::string>& param = m_pItem->m_param;

	if (param.find("bordercolor") != param.end())
		sscanf(param["bordercolor"].c_str(), "%x", &m_borderColor);

	if (param.find("color2") != param.end())
		sscanf(param["color2"].c_str(), "%x", &m_color2);
	else
		m_color2 = m_color;

	if (param.find("steps") != param.end())
		sscanf(param["steps"].c_str(), "%d", &m_steps);

	m_gaugeRect = m_rect;

	if (param.find("left") != param.end() && param["left"] == "true") {
		m_pLButton = new FrButton;
		if (m_pLButton) {
			FrGuiItem& info = m_LButtonInfo;
			info = item;
			info.m_caption = "left";
			info.m_name = "left";
			info.m_rect.left = info.m_rect.top = info.m_rect.right = info.m_rect.bottom = 0;

			m_pLButton->SetOwner(this);
			m_pLButton->Init(info, pManager, this, true);

			m_pLButton->UseDblClick(false);
			m_pLButton->SetPushDelay(0.0f);
			WRect rect = m_pLButton->GetRect();
			m_gaugeRect.x = rect.w + m_gaugeRect.x + 2.0f;
			m_gaugeRect.w = m_gaugeRect.w - (rect.w + 2.0f);
			rect.x = rect.x + 2.0f;
			rect.y = (float)(int)((m_rect.h - rect.h + 1.0f) * 0.5f) + rect.y;
			m_pLButton->SetRect(rect);
		}
	}

	if (param.find("right") != param.end() && param["right"] == "true") {
		m_pRButton = new FrButton;
		if (m_pRButton) {
			FrGuiItem& info = m_RButtonInfo;
			info = item;
			info.m_caption = "right";
			info.m_name = "right";
			info.m_rect.left = info.m_rect.top = info.m_rect.right = info.m_rect.bottom = 0;

			m_pRButton->SetOwner(this);
			m_pRButton->Init(info, pManager, this, false);
			m_pRButton->UseDblClick(false);
			m_pRButton->SetPushDelay(0.0f);

			std::map<std::string, std::string>& skin = m_pItem->m_param;
			FrButton::eButStyle style = FrButton::BT_NORMAL;
			FrButton::eButPushStyle pushStyle = FrButton::BP_PUSHBUTTON;
			const Bitmap* pBmp[3];

			pBmp[0] = pDoc->GetBitmap(skin["rnormal"]);
			pBmp[1] = pDoc->GetBitmap(skin["rover"]);
			pBmp[2] = pDoc->GetBitmap(skin["rselected"]);

			if (skin.find("style") != skin.end())
				style = (FrButton::eButStyle)atoi(skin["style"].c_str());
			if (skin.find("pushstyle") != skin.end())
				pushStyle = (FrButton::eButPushStyle)atoi(skin["pushstyle"].c_str());

			m_pRButton->InitExtern(pBmp[0], pBmp[1], pBmp[2], style, pushStyle);

			WRect rect = m_pRButton->GetRect();
			rect.x = (m_rect.w - rect.w - 2.0f) + rect.x;
			rect.y = (float)(int)((m_rect.h - rect.h + 1.0f) * 0.5f) + rect.y;

			m_gaugeRect.w = m_gaugeRect.w - (rect.w + 2.0f);
			m_pRButton->SetRect(rect);
		}
	}
}

// frgaugebar.cpp:361
void FrGaugeBarEx::SetPos(int pos) {
	FrGaugeBar::SetPos(pos);

	m_fPos = m_pos;
}

// frgaugebar.cpp:368
void FrGaugeBarEx::SetSteps(int steps) {
	m_steps = steps > 1 ? steps : 1;
}

// frgaugebar.cpp:373
int FrGaugeBarEx::GetSteps(void) {
	return m_steps;
}

// frgaugebar.cpp:378
void FrGaugeBarEx::OnDraw(void) {
	unsigned long aDiffuse[4];
	aDiffuse[0] = m_color; aDiffuse[1] = m_colorP;
	aDiffuse[2] = m_color; aDiffuse[3] = m_colorP;

	FrWnd::OnDraw();

	if (m_dwStyle & 0x2) {
		for (int i = 0; i < 4; i++)
			aDiffuse[i] = Modulate(aDiffuse[i], 0xb0b0b0) | 0x50000000;
	}

	GDI()->Box(m_rect, m_bgColor, 0, 0);

	WRect rect(m_rect.x, m_rect.y, m_rect.w - 1.0f, m_rect.h - 1.0f);
	GDI()->LineBox(rect, m_borderColor, 0, 0);

	GDI()->RainbowBox(m_barRect, aDiffuse, 0, 0);

	float fStep = (m_gaugeRect.w - m_thickness * 2.0f - (m_steps - 1.0f) * 2.0f) / m_steps;
	rect.x = (float)m_thickness + m_gaugeRect.x + fStep;
	rect.y = (float)m_thickness + m_gaugeRect.y;
	rect.h = m_gaugeRect.h - m_thickness * 2.0f;
	rect.w = 2.0f;

	for (int step = 0; step < m_steps - 1; step++) {
		GDI()->Box(rect, m_bgColor, 0, 0);
		rect.x += fStep + 2.0f;
	}
}

// frgaugebar.cpp:428
void FrGaugeBarEx::CalcBarRect(const WRect* pRect) {
	FrGaugeBar::CalcBarRect(&m_gaugeRect);

	float inv = 1.0f - m_fPos * 0.01f;
	unsigned long color;
	float value;

	value = (float)((m_color >> 24) & 0xff) * inv;
	value += (float)((m_color2 >> 24) & 0xff) * m_fPos * 0.01f;
	color = (unsigned char)value;
	color <<= 8;
	value = (float)((m_color >> 16) & 0xff) * inv;
	value += (float)((m_color2 >> 16) & 0xff) * m_fPos * 0.01f;
	color |= (unsigned char)value;
	color <<= 8;
	value = (float)((m_color >> 8) & 0xff) * inv;
	value += (float)((m_color2 >> 8) & 0xff) * m_fPos * 0.01f;
	color |= (unsigned char)value;
	color <<= 8;
	value = (float)(m_color & 0xff) * inv;
	value += (float)(m_color2 & 0xff) * m_fPos * 0.01f;
	color |= (unsigned char)value;
	m_colorP = color;
}

// frgaugebar.cpp:446
void FrGaugeBarEx::OnLLButtonDown(void) {
	int lower = m_lower;
	int range = m_upper - lower;

	m_fPos -= (float)range / m_steps;

	if (m_fPos < (float)lower)
		m_fPos = (float)lower;

	m_destPos = m_pos = m_fPos;

	CalcBarRect(0);
}

// frgaugebar.cpp:462
void FrGaugeBarEx::OnRLButtonDown(void) {
	int upper = m_upper;
	int range = upper - m_lower;

	m_fPos += (float)range / m_steps;

	if (m_fPos > (float)upper)
		m_fPos = (float)upper;

	m_destPos = m_pos = m_fPos;

	CalcBarRect(0);
}

// frgaugebar.cpp:478
void FrGaugeBarEx::Enable(bool enable) {
	FrGaugeBar::Enable(enable);
	if (m_pLButton)
		m_pLButton->Enable(enable);
	if (m_pRButton)
		m_pRButton->Enable(enable);
}

// frgaugebar.cpp:487
void FrGaugeBarEx::MoveWindow(const WPoint& pos) {
	float offset = m_gaugeRect.x - m_rect.x;
	FrWnd::MoveWindow(pos);

	m_gaugeRect.x = offset + m_rect.x;
	m_gaugeRect.y = m_rect.y;
	m_barRect.x = m_gaugeRect.x + m_thickness;
	m_barRect.y = m_gaugeRect.y + m_thickness;
}

// frgaugebar.cpp:500 (run-time type and factory registration of FrGaugeBarImage)
const WRTTI FrGaugeBarImage::m_RTTI("FrGaugeBarImage", &FrGaugeBar::m_RTTI);

IObject* FrGaugeBarImageMakeInstance(void) {
	return new FrGaugeBarImage;
}

struct __sFrGaugeBarImage {
	__sFrGaugeBarImage(void) {
		ObjectFactory().AddObjectFunctor(FrGaugeBarImageMakeInstance, "FrGaugeBarImage");
	}
};
static __sFrGaugeBarImage s_registerFrGaugeBarImage;

// frgaugebar.cpp:504
FrGaugeBarImage::FrGaugeBarImage(void) {
	m_thickness = 1;
	m_pBitImage[0] = 0;
	m_pBitImage[1] = 0;

	m_BitMask = 0;

	m_start = 0;
	m_iPixelCounter = 0;
	m_bOwnerDraw = false;
	m_complete = 0;
}

// frgaugebar.cpp:518
FrGaugeBarImage::~FrGaugeBarImage(void) {
	GDI()->InvalidateCache(*m_BitMask);
	if (m_BitMask) {
		delete m_BitMask;
		m_BitMask = 0;
	}
}

// frgaugebar.cpp:539
void FrGaugeBarImage::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrGaugeBar::Init(item, pManager, pParent);

	FrElementDoc* pDoc = pManager->GetDocument();

	std::map<std::string, std::string>& param = m_pItem->m_param;

	m_bOwnerDraw = atoi(param["drawstyle"].c_str()) == 1;

	m_pBitImage[GAUGE_NORMAL] = pDoc->GetBitmap(param["normal"]);
	m_pBitImage[GAUGE_MASK] = pDoc->GetBitmap(param["mask"]);

	if (m_pBitImage[GAUGE_NORMAL] == 0 || m_pBitImage[GAUGE_MASK] == 0) {
		// "Image file not found."
		MessageBoxA(0, "\300\314\271\314\301\366 \306\304\300\317\300\314 \276\370\275\300\264\317\264\331.", "\260\346\260\355", 0);
	}

	if (m_pBitImage[GAUGE_NORMAL]->Width() != m_pBitImage[GAUGE_MASK]->Width()) {
		// "The mask width differs."
		MessageBoxA(0, "\270\266\275\272\305\251\300\307 \263\320\300\314\260\241 \306\262\270\263\264\317\264\331.", "\260\346\260\355", 0);
	}

	m_gaugeRect.x = m_rect.x;
	m_gaugeRect.y = m_rect.y;
	m_gaugeRect.w = m_pBitImage[GAUGE_NORMAL]->Width();
	m_gaugeRect.h = m_pBitImage[GAUGE_NORMAL]->Height();

	m_BitMask = new Bitmap;
	m_BitMask->Create(m_pBitImage[GAUGE_MASK]->bi->bmiHeader.biWidth, m_pBitImage[GAUGE_MASK]->bi->bmiHeader.biHeight, m_pBitImage[GAUGE_MASK]->bi->bmiHeader.biBitCount);
	memcpy(m_BitMask->vram, m_pBitImage[GAUGE_MASK]->vram, m_pBitImage[GAUGE_MASK]->Size());

	m_start = 0;
	SetRange(0, (int)m_gaugeRect.w, 0);
}

// frgaugebar.cpp:583
const Bitmap* FrGaugeBarImage::GetGaugeSkin(int type) {
	const Bitmap* pSkin;
	if (type == GAUGE_MASK)
		pSkin = m_BitMask;
	else
		pSkin = m_pBitImage[GAUGE_NORMAL];
	return pSkin;
}

// frgaugebar.cpp:598
void FrGaugeBarImage::ResetGauge(void) {
	memcpy(m_BitMask->vram, m_pBitImage[GAUGE_MASK]->vram, m_pBitImage[GAUGE_MASK]->Size());

	GDI()->RefreshTextureCacheInfo(m_BitMask);

	m_start = 0;
	m_pos = 0;
	m_gaugeRect.x = m_rect.x;
	m_gaugeRect.y = m_rect.y;

	m_iPixelCounter = 0;
}

// frgaugebar.cpp:617
int FrGaugeBarImage::GetPixelPerHeight(void) {
	return m_iPixelCounter;
}

// frgaugebar.cpp:622
float FrGaugeBarImage::GetComplete(void) {
	return m_complete;
}

// frgaugebar.cpp:627
void FrGaugeBarImage::OnDraw(void) {
	if (m_pBitImage[GAUGE_NORMAL] == 0)
		return;
	if (m_pBitImage[GAUGE_MASK] == 0)
		return;

	FrWnd::OnDraw();

	if (!m_bOwnerDraw) {
		GDI()->DrawTexture(m_pBitImage[GAUGE_NORMAL], WRect(m_gaugeRect.x, m_gaugeRect.y, m_pBitImage[GAUGE_NORMAL]->Width(), m_pBitImage[GAUGE_NORMAL]->Height()), 0xffffffff, 0);
		GDI()->DrawTexture(m_BitMask, WRect(m_gaugeRect.x, m_gaugeRect.y, m_BitMask->Width(), m_BitMask->Height()), 0xffffffff, 0);
	} else {
		SendCmdToOwnerTarget(FRCMD_OWNERDRAW, 0, 0);
	}
}

// frgaugebar.cpp:646
void FrGaugeBarImage::CalcBarRect(const WRect* pRect) {
	m_barRect = m_gaugeRect;

	float ratio = (m_barRect.w - m_thickness * 2) / (m_upper - m_lower);

	if (m_origin < m_pos) {
		m_barRect.x += (m_origin - m_lower) * ratio;
		m_barRect.w = (m_pos - m_origin) * ratio;
	} else {
		m_barRect.x += (m_pos - m_lower) * ratio;
		m_barRect.w = (m_origin - m_pos) * ratio;
	}

	m_barRect.x += m_thickness;
	m_barRect.y += m_thickness;
	m_barRect.h -= m_thickness * 2;

	if (m_start != m_barRect.w) {
		if (m_pBitImage[GAUGE_MASK] != 0) {
			int i = (int)floor(m_start);
			while (i < (int)m_barRect.w) {
				for (unsigned int y = 0; y < m_pBitImage[GAUGE_MASK]->Height(); y++)
					m_BitMask->SetAlpha(i, y, 0);
				m_iPixelCounter++;
				i++;
			}

			m_start = m_barRect.w;
			m_complete = (m_start * 100.0f) / m_barRect.w;
			GDI()->RefreshTextureCacheInfo(m_BitMask);
		}
	}
}

// frgaugebar.cpp:688
void FrGaugeBarImage::OnProc(const float delta) {
	float step;
	if (m_achillesRun && m_destPos != m_srcPos)
		step = (m_destPos - m_pos) * m_speed * delta / (m_destPos - m_srcPos);
	else
		step = delta * m_speed;

	if (m_pos > m_destPos) {
		float pos = m_pos - step;
		m_pos = pos > m_destPos ? pos : m_destPos;
	} else if (m_pos < m_destPos) {
		float pos = m_pos + step;
		m_pos = pos < m_destPos ? pos : m_destPos;
	}

	CalcBarRect(0);
}

// frgaugebar.cpp:713
void FrGaugeBarImage::MoveWindow(const WPoint& pos) {
	FrWnd::MoveWindow(pos);

	m_gaugeRect.x = pos.x;
	m_gaugeRect.y = pos.y;
}
