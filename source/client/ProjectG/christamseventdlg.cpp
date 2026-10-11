// Original: d:\build\custom\temp\code\client\projectg\christamseventdlg.cpp
#include <algorithm>
#include <vector>

#include "ChristamsEventDlg.h"
#include "ObjectFactory.h"
#include "frbutton.h"
#include "frarea.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/wtypes.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../shared/globalgamedefine.h"

// christamseventdlg.cpp:26 (run-time type and factory registration of FrChristmasEventDlg)
const WRTTI FrChristmasEventDlg::m_RTTI("FrChristmasEventDlg", &FrForm::m_RTTI);

IObject* FrChristmasEventDlgMakeInstance(void) {
	return new FrChristmasEventDlg;
}

struct __sFrChristmasEventDlg {
	__sFrChristmasEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrChristmasEventDlgMakeInstance, "FrChristmasEventDlg");
	}
};
static __sFrChristmasEventDlg s_registerFrChristmasEventDlg;

// christamseventdlg.cpp:28
const sFRESH_MSGMAP* FrChristmasEventDlg::GetMessageMap(void) const {
	return &FrChristmasEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrChristmasEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrChristmasEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrChristmasEventDlg::_MsgEntries[] = {
	// christamseventdlg.cpp:30
	{ "close", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnCloseBtnInit },
	// christamseventdlg.cpp:31
	{ "close", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrChristmasEventDlg::OnCloseBtnUp },
	// christamseventdlg.cpp:32
	{ "caption", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnCaptionInit },

	// christamseventdlg.cpp:34
	{ "xmas_btn1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasBtn1Init },
	// christamseventdlg.cpp:35
	{ "xmas_btn2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasBtn2Init },
	// christamseventdlg.cpp:36
	{ "xmas_btn3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasBtn3Init },

	// christamseventdlg.cpp:38
	{ "xmas_ex1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasEx1Init },
	// christamseventdlg.cpp:39
	{ "xmas_ex2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasEx2Init },
	// christamseventdlg.cpp:40
	{ "xmas_ex3", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnXmasEx3Init },

	// christamseventdlg.cpp:42
	{ "xmas_socks1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnSocks1AreaInit },
	// christamseventdlg.cpp:43
	{ "xmas_socks2", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrChristmasEventDlg::OnSocks2AreaInit },

	// christamseventdlg.cpp:45
	{ "snow_falling", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrChristmasEventDlg::OnSnowFallDraw },

	// christamseventdlg.cpp:47
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

namespace {

// One falling snowflake of the dialog.
class cSnow {
public:
	cSnow(void) : m_bAlive(true) {}                                   // christamseventdlg.cpp:71
	~cSnow(void) {}                                                   // christamseventdlg.cpp:72

	static void SetArea(const WRect& rc) { rcArea = rc; }             // christamseventdlg.cpp:75
	const Bitmap* Img(void) const { return m_pImg; }                  // christamseventdlg.cpp:78
	WRect GetRect(void) const {                                       // christamseventdlg.cpp:81
		return WRect(m_pos.x, m_pos.y, m_pImg->Width(), m_pImg->Height());
	}
	unsigned long Color(void) const {                                 // christamseventdlg.cpp:84
		return ((unsigned char)(int)m_alpha << 24) | 0xffffff;
	}

	void Create(void);
	void InitialCreate(void);
	void Process(float dt);
	bool IsAlive(void);

	static WRect rcArea;

	WVector2D m_pos;         // 0x0
	WVector2D m_dir;         // 0x8
	const Bitmap* m_pImg;    // 0x10
	float m_alpha;           // 0x14
	bool m_bFadeIn;          // 0x18
	float m_speed;           // 0x1c
	bool m_bAlive;           // 0x20
};

WRect cSnow::rcArea;

// christamseventdlg.cpp:88
void cSnow::Create(void) {
	int left = (int)rcArea.x; int right = (int)(rcArea.w + rcArea.x);

	int x = rand() % right;

	m_pos.x = (float)(x + left); m_pos.y = rcArea.y + 30.0f; m_dir.x = (float)(rand() % 10 - 5);
	m_dir.y = (float)(rand() % 50 + 1);
	m_dir.Normalize();

	m_speed = (float)(rand() % 100 + 10);

	int r = rand() % 100;
	if (r < 20)
		m_pImg = g_pFresh->GetBitmap("snow1");
	else if (r < 60)
		m_pImg = g_pFresh->GetBitmap("snow2");
	else
		m_pImg = g_pFresh->GetBitmap("snow3");

	m_alpha = (float)(rand() % 255 + 128);
	m_bFadeIn = rand() % 2 != 0;
}

// christamseventdlg.cpp:117
void cSnow::InitialCreate(void) {
	Create();
	int top = (int)rcArea.y + 30;
	int bottom = (int)(rcArea.h + rcArea.y);

	m_pos.y = (float)(rand() % bottom + top);
}

// christamseventdlg.cpp:127
void cSnow::Process(float dt) {
	m_pos += m_dir * m_speed * dt;
	if (m_bFadeIn) {
		m_alpha += dt * 64.0f;
		if (m_alpha > 255.0f) {
			m_alpha = 255.0f;
			m_bFadeIn = false;
		}
	} else {
		m_alpha -= dt * 64.0f;
		if (m_alpha < 128.0f) {
			m_alpha = 128.0f;
			m_bFadeIn = true;
		}
	}
}

// christamseventdlg.cpp:151
bool cSnow::IsAlive(void) {
	if (m_pos.x - m_pImg->Width() < rcArea.x || m_pos.x + m_pImg->Width() > rcArea.x + rcArea.w
		|| m_pos.y + m_pImg->Height() + 30.0f > rcArea.y + rcArea.h) {
		return false;
	}
	return true;
}

// christamseventdlg.cpp:163, the falling snowflakes, owned by the dialog
std::vector<cSnow*> snow;

} // namespace

// christamseventdlg.cpp:169
FrChristmasEventDlg::FrChristmasEventDlg(void) {
	char* pFontName = "[font_hole_skins_red.jpg";
	m_pTitleFont = g_resrcmng->GetTitleFont();

	m_titleFontInfo.filename = &pFontName;

	m_titleFontInfo.flag = 0;
	m_titleFontInfo.fonth = 16;
	m_titleFontInfo.fontw = 16;
	m_titleFontInfo.numPages = 1;
	m_titleFontInfo.texw = 128;
	m_titleFontInfo.texh = 32;
	m_titleFontInfo.pCharSet = "1234567890-,";
	m_pTitleFont->Create(&m_titleFontInfo);

	m_pXmasBtn1 = NULL;
	m_pXmasEx1 = NULL; m_pXmasBtn2 = NULL; m_pXmasEx2 = NULL; m_pXmasBtn3 = NULL; m_pXmasEx3 = NULL;

	snow.clear();
}

// christamseventdlg.cpp:195
FrChristmasEventDlg::~FrChristmasEventDlg(void) {
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
	std::for_each(snow.begin(), snow.end(), Delete_Object());
	snow.clear();
}

// christamseventdlg.cpp:202
bool FrChristmasEventDlg::OnInit(void) {
	FrForm::OnInit();

	EnableDrag(false);

	m_bXmasEx1Shown = false;
	m_bXmasEx2Shown = false;
	m_bXmasEx3Shown = false;

	cSnow::SetArea(m_rect);
	for (int i = 0; i < 100; ++i) {
		cSnow* pSnow = new cSnow;
		pSnow->InitialCreate();
		snow.push_back(pSnow);
	}

	return true;
}

// christamseventdlg.cpp:222
void FrChristmasEventDlg::OnCloseBtnInit(intptr_t param) {
	FrButton* pBtn = DYNAMIC_CAST(FrButton, param);
}

// christamseventdlg.cpp:228
void FrChristmasEventDlg::OnCloseBtnUp(void) {
	OnFreshOkay();
}

// christamseventdlg.cpp:233
void FrChristmasEventDlg::OnCaptionInit(intptr_t param) {
	FrArea* pCaption = DYNAMIC_CAST(FrArea, param);
}

// christamseventdlg.cpp:241
void FrChristmasEventDlg::OnXmasBtn1Init(intptr_t param) {
	m_pXmasBtn1 = DYNAMIC_CAST(FrButton, param);
}

// christamseventdlg.cpp:246
void FrChristmasEventDlg::OnXmasBtn2Init(intptr_t param) {
	m_pXmasBtn2 = DYNAMIC_CAST(FrButton, param);
}

// christamseventdlg.cpp:251
void FrChristmasEventDlg::OnXmasBtn3Init(intptr_t param) {
	m_pXmasBtn3 = DYNAMIC_CAST(FrButton, param);
}

// christamseventdlg.cpp:256
void FrChristmasEventDlg::OnXmasEx1Init(intptr_t param) {
	m_pXmasEx1 = DYNAMIC_CAST(FrArea, param);
	if (m_pXmasEx1) {
		// shown while xmas_btn1 is hovered (OnProc)
		m_pXmasEx1->SetVisible(false);
	}
}

// christamseventdlg.cpp:265
void FrChristmasEventDlg::OnXmasEx2Init(intptr_t param) {
	m_pXmasEx2 = DYNAMIC_CAST(FrArea, param);
	if (m_pXmasEx2) {
		// shown while xmas_btn2 is hovered (OnProc)
		m_pXmasEx2->SetVisible(false);
	}
}

// christamseventdlg.cpp:274
void FrChristmasEventDlg::OnXmasEx3Init(intptr_t param) {
	m_pXmasEx3 = DYNAMIC_CAST(FrArea, param);
	if (m_pXmasEx3) {
		// shown while xmas_btn3 is hovered (OnProc)
		m_pXmasEx3->SetVisible(false);
	}
}

// christamseventdlg.cpp:284
void FrChristmasEventDlg::OnSocks1AreaInit(intptr_t param) {

	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea)
		pArea->SetVisible(false);
}

// christamseventdlg.cpp:293
void FrChristmasEventDlg::OnSocks2AreaInit(intptr_t param) {

	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea)
		pArea->SetVisible(false);
}

// christamseventdlg.cpp:301
void FrChristmasEventDlg::OnProc(const float dt) {
	if (m_pXmasBtn1->GetStatus() == FrButton::OVER) {
		if (!m_bXmasEx1Shown) {
			m_bXmasEx1Shown = true;
			m_pXmasEx1->SetVisible(true);
		}
	} else if (m_bXmasEx1Shown == true) {
		m_bXmasEx1Shown = false;
		m_pXmasEx1->SetVisible(false);
	}

	if (m_pXmasBtn2->GetStatus() == FrButton::OVER) {
		if (!m_bXmasEx2Shown) {
			m_bXmasEx2Shown = true;
			m_pXmasEx2->SetVisible(true);
		}
	} else if (m_bXmasEx2Shown == true) {
		m_bXmasEx2Shown = false;
		m_pXmasEx2->SetVisible(false);
	}

	if (m_pXmasBtn3->GetStatus() == FrButton::OVER) {
		if (!m_bXmasEx3Shown) {
			m_bXmasEx3Shown = true;
			m_pXmasEx3->SetVisible(true);
		}
	} else if (m_bXmasEx3Shown == true) {
		m_bXmasEx3Shown = false;
		m_pXmasEx3->SetVisible(false);
	}

	if (snow.size() < 100) {
		cSnow* pSnow = new cSnow;
		pSnow->Create();
		snow.push_back(pSnow);
	}

	std::vector<cSnow*>::iterator it = snow.begin();
	while (it != snow.end()) {
		cSnow* pSnow = *it;
		pSnow->Process(dt);
		if (!pSnow->IsAlive()) {
			delete pSnow;
			it = snow.erase(it);
		} else {
			++it;
		}
	}
}

// christamseventdlg.cpp:370
void FrChristmasEventDlg::OnDraw(void) {
}

// christamseventdlg.cpp:382
void FrChristmasEventDlg::OnSnowFallDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	std::vector<cSnow*>::iterator it = snow.begin();
	for (; it != snow.end(); ++it) {
		cSnow* pSnow = *it;
		pGDI->DrawTexture(pSnow->Img(), pSnow->GetRect(), pSnow->Color(), 0);
	}
}
