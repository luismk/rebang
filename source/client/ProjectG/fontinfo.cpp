// Original: d:\build\custom\temp\code\client\projectg\fontinfo.cpp
#include "fontinfo.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "cardsystem.h"
#include "club.h"
#include "golfball.h"
#include "wind.h"
#include "mousecursor.h"
#include "wview.h"
#include "quadtree.h"
#include "wresrcmng.h"
#include "SoundManager.h"
#include "SimpleOverlay.h"
#include "ClientSetting.h"
#include "InputManager.h"
#include "wutil.h"
#include "s5/Utilities.h"
#include "s5/classicserver/ClassicServer.h"
#include "../../shared/Localize.h"
#include "logininfo.h"
#include "commonutil.h"
#include "GroundItemMan.h"
#include "TaskManager.h"
#include "UccManager.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

// Calipers tuning floats without a symbol: [20] = 1.0f (CDistToHoleInfo::Display), [21] = 15.0f (speed limit),
// [26] = 500.0f (right end of the bar), [28] = left margin past the club's
// narrow accuracy.
extern "C" const float data_9eb5b8[29];
// A float with no symbol (a screen width, set elsewhere).
extern "C" float data_af23d0;
// The camera's forward vector, next to data_00b7bfac (its position).
extern WVector data_00b7bfa0;
// Calipers key-repeat timer.
extern "C" float data_af23d8;
extern WVector data_00b7bfac;

int WisZero(const float &f, float eps);

// fontinfo.h:33
void CFontInfo::Process(float fDelta) {
}

// fontinfo.h:34
void CFontInfo::Display(void) {
}

// fontinfo.h:41
void CFontInfo::SetColor(unsigned long color) {
	m_color = color;
}

// fontinfo.h:42
void CFontInfo::GetScreenPos(float *pfX, float *pfY) {
}

// fontinfo.h:46
void CFontInfo::Reset(void) {
}

// fontinfo.h:49
char *&CFontInfo::Buffer(void) {
	return m_pBuffer;
}

// fontinfo.h:458
char *&CAccuracyInfo::Buffer2(void) {
	return m_pBuffer2;
}

// fontinfo.h:493
char *&CGuildInfo::Buffer2(void) {
	return m_pBuffer2;
}

// fontinfo.h:795
bool CCalipers::IsVisiblePowerBar(void) const {
	return m_nPowerBarState == 1 ? true : false;
}

// fontinfo.h:796
void CCalipers::VisiblePowerBar(int nState) {
	m_nPowerBarState = nState;
}

// quadtree.h:541
const std::vector<CQuadTree::sColData> &CQuadTree::GetColList(void) {
	return m_colList;
}

// cardsystem.h:63
unsigned char CCardManager::GetWindColor(void) {
	return m_windColor;
}

// mousecursor.h:204
void CMouseCursor::SetHoleCup(float x, float y) {
	m_holeCupX = x;
	m_holeCupY = y;
}

// shareddoc.h:642
unsigned long CSharedDoc::GetFlagAbilityItem(void) {
	return m_flagAbilityItem;
}

// shareddoc.h:644
unsigned char CSharedDoc::GetNumBonusPangItem(void) {
	return m_numBonusPangItem;
}

// shareddoc.h:1012
bool CSharedDoc::IsWearHalloweenItem(void) {
	return m_wearHalloweenItem;
}

// shareddoc.inl:174
bool IsGameType(unsigned char type) {
	return WSingleton<CSharedDoc>::Instance()->m_roomType == type ? true : false;
}

bool CFontInfo::m_loaded;

// fontinfo.cpp:40
CFontInfo::CFontInfo(void) : m_pBuffer(NULL) {
	if (!m_loaded) {
		m_loaded = true;

		AddFont("[font_hole.jpg", 16, 16, 1, 128, 32, "1234567890ym%-.,+>");
		AddFont("[font_bar.jpg", 16, 16, 1, 128, 32, "1234567890ym%-. ");
		AddFont("[font_wind.jpg", 16, 16, 1, 128, 64, "1234567890ym%-.?/:");
		AddFont("[font_club.jpg", 32, 32, 1, 128, 128, "0123456789abcdef");
		AddFont("[bigfont.jpg", 32, 32, 1, 256, 128, "1234567890ym%-./x?() ");

		if (IsGameType(7)) {
			AddFont("[font_hole_skins_red.jpg", 16, 16, 1, 128, 32, "1234567890-,");
			AddFont("[font_hole_skins_blue.jpg", 16, 16, 1, 128, 32, "1234567890+,");
			AddFont("[font_hole_skins_grey.jpg", 16, 16, 1, 128, 32, "1234567890>,");
		}

		if (IsGameType(6)) {
			AddFont("[font_hole_skins_red.jpg", 16, 16, 1, 128, 32, "1234567890-,");
			AddFont("[font_hole_skins_blue.jpg", 16, 16, 1, 128, 32, "1234567890+,");
		}
	}
}

// fontinfo.cpp:67
WList<CFontInfo::sFontInfo *> CFontInfo::m_fontList(16, 4);

// fontinfo.cpp:70
CFontInfo::~CFontInfo() {
	Clear();
}

// The font textures AddFont accepts; a registered font points at its entry.
static char *s_fontFiles[8] = {
	"[font_hole.jpg",
	"[font_bar.jpg",
	"[font_wind.jpg",
	"[font_club.jpg",
	"[bigfont.jpg",
	"[font_hole_skins_red.jpg",
	"[font_hole_skins_blue.jpg",
	"[font_hole_skins_grey.jpg",
};

// fontinfo.cpp:76
void CFontInfo::AddFont(const char *name, int fontw, int fonth, int numPages, int texw, int texh, char *charSet) {
	sFontInfo *pInfo = new sFontInfo;

	memset(pInfo, 0, sizeof(tagWTITLEFONT));

	for (int i = 0; i < sizeof(s_fontFiles) / sizeof(s_fontFiles[0]); i++) {
		if (strcmp(s_fontFiles[i], name) == 0) {
			pInfo->filename = &s_fontFiles[i];
			break;
		}
	}

	if (pInfo->filename != NULL) {
		pInfo->fontw = fontw;
		pInfo->fonth = fonth;
		pInfo->numPages = numPages;
		pInfo->texw = texw;
		pInfo->texh = texh;
		pInfo->pCharSet = charSet;
		pInfo->m_index = -1;

		strcpy(pInfo->m_name, name);

		m_fontList.AddItem(pInfo, pInfo->m_name, false);
	} else {
		delete pInfo;
	}
}

// fontinfo.cpp:126
void CFontInfo::Init(char *pszName, sFont *pFont) {
	if (pFont == NULL) {
		pFont = &m_font;
	}
	pFont->m_pFontInfo = m_fontList.Find(pszName);

	pFont->m_pTitleFont = g_resrcmng->GetTitleFont();
	pFont->m_pTitleFont->Create(pFont->m_pFontInfo);

	m_color = 0xFFFFFFFF;
	m_nFlag = 0;
}

// fontinfo.cpp:141
void CFontInfo::Clear(void) {
	if (g_resrcmng && m_font.m_pTitleFont) {
		g_resrcmng->Release(m_font.m_pTitleFont);
		m_font.m_pTitleFont = NULL;
	}
	if (m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	for (sFontInfo *pInfo = m_fontList.Start(); pInfo != NULL; pInfo = m_fontList.Next()) delete pInfo; m_fontList.Reset();

	m_loaded = false;
}

// fontinfo.cpp:151
float CFontInfo::GetTextWidth(const char *text, float space, float scale) {
	int len = strlen(text);
	return (float)(len * m_font.m_pFontInfo->fontw) * scale * 0.5f + len * space * 2;
}

// fontinfo.cpp:157
float CFontInfo::GetExactTextWidth(const char *text, float space, float scale) {
	if (text == NULL) {
		return 0.0f;
	}

	int len = strlen(text);
	if (len == 0) {
		return 0.0f;
	}

	float oldScale = m_font.m_pTitleFont->GetScale();

	m_font.m_pTitleFont->SetScale(scale);

	char *charSet = m_font.m_pFontInfo->pCharSet;

	float width = 0;
	while (*text) {
		char c = *text++;
		char *pFound = strchr(charSet, c);

		width += m_font.m_pTitleFont->GetCharWidthA(g_view, pFound - charSet) + space * scale;
	}

	float result = width - space;

	m_font.m_pTitleFont->SetScale(oldScale);

	return result;
}

// fontinfo.cpp:194
float CFontInfo::Print(float x, float y, const char *text, float space, int flag, float scale, sFont *pFont) {
	if (text == NULL) {
		return 0.0f;
	}

	int len = strlen(text);
	if (len == 0) {
		return 0.0f;
	}

	if (pFont == NULL) {
		pFont = &m_font;
	}

	char *charSet = pFont->m_pFontInfo->pCharSet;

	float oldScale = pFont->m_pTitleFont->GetScale();
	pFont->m_pTitleFont->SetScale(scale);

	float width = 0.0f;
	space *= scale;
	switch (m_nFlag) {
	case 1: {
		const char *p = text;
		while (*p) {
			char *pFound = strchr(charSet, *p++);
			width += pFont->m_pTitleFont->GetCharWidthA(g_view, pFound - charSet) + space;
		}
		width = (width - space) * 0.5f;
		break;
	}
	case 2: {
		const char *p = text;
		while (*p) {
			char *pFound = strchr(charSet, *p++);
			width += pFont->m_pTitleFont->GetCharWidthA(g_view, pFound - charSet) + space;
		}
		width = width - space;
		break;
	}
	}

	x -= width;
	while (*text) {
		char *pFound = strchr(charSet, *text++);
		x += pFont->m_pTitleFont->PutChar(g_view, x, y, pFound - charSet, flag, m_color) + space;
	}

	pFont->m_pTitleFont->SetScale(oldScale);
	return width;
}

// fontinfo.cpp:271
CDistToHoleInfo::CDistToHoleInfo(void) {
	ResetUiPosition();
	m_fScale = 0.77f;
	m_curPos.y = 63.0f;
	m_curPos.z = 0.5f;
	m_nState = 5;
	m_fBlend = 0;

	Init("[bigfont.jpg", NULL);

	m_pBuffer = new char[0x20];
}

// fontinfo.cpp:290
void CDistToHoleInfo::Process(float fDelta) {
	if (GOLFDOC()->GameMode() & 2) {
		return;
	}

	if ((GOLFDOC()->GameMode() & 0x8210) || !IsMyTurn(true)
		|| (GOLFDOC()->m_gameState < 15 && GOLFDOC()->m_gameState != 8)
		|| (GOLFDOC()->m_gameState == 8 && PLAYER(GOLFDOC()->m_myPlayerIndex)->m_holes[0] == 1)) {
		m_nState = 0;
	}

	if (CSharedDoc::Instance()->m_roomType == 10) {
		m_nState = 0;
	}

	switch (m_nState) {
	case 1:
		m_fTime += fDelta;
		if (m_fTime > 0.5f) {
			m_fTime = 0.5f;
			m_nState = 0;
		}

		m_fBlend = m_fTime * 2;

		if (g_input->GetList() && GOLFDOC()->GameMode() != 4) {
			m_nState = 0;
		}
		break;
	case 2:
		m_fAlpha += fDelta * 510.0f;
		if (m_fAlpha > 255.0f) {
			m_fTime = 0;
			m_nState = 4;
			m_fAlpha = 255.0f;
		}
		break;
	case 4:
		m_fTime += fDelta;
		if (m_fTime > 2.0f) {
			m_fTime = 0;
			m_nState = 1;
		}

		if (g_input->GetList() && GOLFDOC()->GameMode() != 4) {
			m_nState = 3;
		} else {
			WVector toCup = GetHoleCupPos() - data_00b7bfac;
			if (!(data_00b7bfa0.x * toCup.x + data_00b7bfa0.y * toCup.y + data_00b7bfa0.z * toCup.z > 0.0f)) {
				m_nState = 3;
			}
		}
		break;
	case 3:
		m_fAlpha -= fDelta * 510.0f;
		if (m_fAlpha > 0.0f) {
			break;
		}
		m_nState = 0;
	case 0:
		m_fBlend = 1.0f;
		m_fAlpha = 255.0f;
		break;
	}
}

// fontinfo.cpp:366
void CDistToHoleInfo::Display(void) {
	if (m_nState == 5 || (CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_identity & 1) && m_nState != 0 && GOLFDOC()->GameMode() != 4) {
		return;
	}

	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	WVector screen = g_view->Projection(cup);

	if (screen.z <= 0.0f || data_9eb5b8[20] <= screen.z) {
		return;
	}

	float fInv = data_9eb5b8[20] - m_fBlend;
	m_fixPos = WVector((screen.x + m_fOffsetX) * m_fBlend + fInv * m_uiX,
		fInv * m_uiY + m_curPos.y * m_fBlend,
		fInv * m_fScale + m_curPos.z * m_fBlend);

	m_color = ((int)m_fAlpha << 24) | 0xFF0000;

	Print(m_fixPos.x, GOLFDOC()->GameMode() == 0x200 ? screen.y : m_fixPos.y, m_pBuffer, m_fixPos.z * 6.0f, 0, m_fixPos.z, NULL);
}

// fontinfo.cpp:406
void CDistToHoleInfo::Fix(void) {
	m_fixPos.x = m_curPos.x;
	m_fAlpha = 255.0f;
	m_nState = 0;
	m_fixPos.y = m_curPos.y;
	m_fixPos.z = m_curPos.z;
}

// fontinfo.cpp:419
void CDistToHoleInfo::Reset(void) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	WVector delta;
	delta.x = cup.x - PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos.x;
	delta.z = cup.z - PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos.z;

	if (IsLocalContent((localContentType_t)0x4F) && CSharedDoc::Instance()->m_playingMode == 2) {
		delta = GOLFDOC()->GetHoleData().m_cupPos - GolfBall().m_pos;
	}

	m_fDistance = sqrtf(delta.x * delta.x + delta.z * delta.z) * 0.3125f;
	if (m_fDistance <= 100.0f) {
		sprintf(m_pBuffer, "%.1fy", m_fDistance);
		if (m_fDistance <= 10.0f) {
			m_fOffsetX = -14.0f;
		} else {
			m_fOffsetX = -20.0f;
		}
	} else {
		sprintf(m_pBuffer, "%.0fy", m_fDistance);
		m_fOffsetX = -20.0f;
	}

	m_fTime = 0;
	m_fBlend = 0;

	if (IsMyTurn(true) && !(IsLocalContent((localContentType_t)0x4F) && CSharedDoc::Instance()->m_playingMode == 2)) {
		m_fixPos = WVector(m_uiX, m_uiY, m_fScale);
		m_fAlpha = 0;
		m_nState = 2;
	} else {
		m_nState = 0;
		m_fAlpha = 255.0f;
	}
}

// fontinfo.cpp:467
void CDistToHoleInfo::ResetUiPosition(void) {
	m_uiX = g_view->GetWidth() * 0.5f + 20.0f;
	m_uiY = g_view->GetHeight() * 0.25f + 25.0f;
}

// fontinfo.cpp:479
CGuildInfo::CGuildInfo(void) {
	Init("[font_hole_skins_red.jpg", &m_leftFont);
	Init("[font_hole_skins_blue.jpg", &m_rightFont);

	m_pBuffer = new char[0x20];
	m_pBuffer2 = new char[0x20];
	m_fTime = 0;
	m_fScale = 1.5f;
	m_pszSound = "pang_down";

	memset(m_targetScore, 0, sizeof(m_targetScore));

	memset(m_displayScore, 0, sizeof(m_displayScore));
}

// fontinfo.cpp:496
CGuildInfo::~CGuildInfo() {
	if (m_pBuffer2) {
		delete m_pBuffer2;
		m_pBuffer2 = NULL;
	}
}

// fontinfo.cpp:502
void CGuildInfo::Process(float fDelta) {
	m_fTime += fDelta;

	if (m_targetScore[0] > m_displayScore[0]) {

		m_displayScore[0]++;
	}

	if (m_targetScore[1] > m_displayScore[1]) {

		m_displayScore[1]++;
	}

	float fScale = 2.5f - m_fTime * 1.5f;
	m_fScale = fScale < 1.5f ? 1.5f : (fScale > 2.5f ? 2.5f : fScale);
}

// fontinfo.cpp:520
void CGuildInfo::SetScore(int nScore1, int nScore2) {
	m_targetScore[0] = nScore1;
	m_targetScore[1] = nScore2;
	m_fTime = 0;

	if (m_pszSound && !g_audio->IsPlaying(m_pszSound)) {
		g_audio->PlaySfx(m_pszSound, NULL, 0, m_pszSound, NULL, 0.5f, 200.0f);
	}
}

// fontinfo.cpp:540
void CGuildInfo::Display(void) {
	m_nFlag = 0;
	sprintf(m_pBuffer, "%d", m_displayScore[0]);
	sprintf(m_pBuffer2, "%d", m_displayScore[1]);

	Print(g_view->GetWidth() - 80.0f, 250.0f, m_pBuffer, 4.0f, 0, m_fScale, &m_leftFont);
	Print(g_view->GetWidth() - 30.0f, 250.0f, m_pBuffer2, 4.0f, 0, m_fScale, &m_rightFont);
}

// fontinfo.cpp:559
CHoleCupInfo::CHoleCupInfo(void) : m_pDistToHole(NULL) {
	Init("[font_bar.jpg", NULL);

	m_nDisplayMode = 1;

	m_pTopHoleOverlay[0] = g_resrcmng->GetOverlay("[top_hole_l.jpg", 0);
	m_pTopHoleOverlay[1] = g_resrcmng->GetOverlay("[top_hole_m.jpg", 0);
	m_pTopHoleOverlay[2] = g_resrcmng->GetOverlay("[top_hole_h.jpg", 0);

	m_pDistToHole = new CDistToHoleInfo;
}

// fontinfo.cpp:574
CHoleCupInfo::~CHoleCupInfo() {

	for (int i = 0; i < 3; i++) {
		if (m_pTopHoleOverlay[i]) {

			g_resrcmng->Release(m_pTopHoleOverlay[i]);
			m_pTopHoleOverlay[i] = NULL;
		}
	}

	if (m_pDistToHole) {
		delete m_pDistToHole;
		m_pDistToHole = NULL;
	}
}

// fontinfo.cpp:590
void CHoleCupInfo::Reset(void) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	m_fCupHeight = (cup.y - (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos.y - GolfBall().m_radius)) * 0.3125f * 0.9144f;
	if (m_pDistToHole)
		m_pDistToHole->Reset();
}

// fontinfo.cpp:597
void CHoleCupInfo::Fix(void) {
	if (m_pDistToHole) {
		m_pDistToHole->Fix();
	}
}

// fontinfo.cpp:606
void CHoleCupInfo::GetScreenPos(float *pfX, float *pfY) {
	*pfX = m_screenX;
	*pfY = m_screenY;
}

// fontinfo.cpp:614
void CHoleCupInfo::Process(float fDelta) {
	if (m_pDistToHole) {
		m_pDistToHole->Process(fDelta);
	}
}

// fontinfo.cpp:623
void CHoleCupInfo::Display(void) {
	char buf[0x20];

	if (m_pDistToHole) {
		m_pDistToHole->Display();
	}

	if (fabsf(m_fCupHeight) > 10.0f) {
		sprintf(buf, "%.1fm", m_fCupHeight);
	} else {
		sprintf(buf, "%.2fm", m_fCupHeight);
	}

	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	WVector proj = g_view->Projection(cup);

	if (proj.z > 0.0f && data_9eb5b8[20] > proj.z) {
		m_screenX = proj.x;
		m_screenY = GOLFDOC()->GameMode() == 0x200 ? proj.y : 62.0f;
		CMouseCursor::Instance()->SetHoleCup(proj.x, m_screenY);

		float fY;
		if (fabsf(m_fCupHeight) < 0.1f) {
			m_nDisplayMode = 1;
			fY = m_screenY - 26.0f;
		} else if (m_fCupHeight > 0.0f) {
			fY = m_screenY - 17.0f;
			m_nDisplayMode = 2;
		} else {
			fY = m_screenY - 37.0f;
			m_nDisplayMode = 0;
		}
		Print(m_screenX + 16.0f, fY, buf, 3.0f, 0, 1.0f, NULL);

		if (m_pTopHoleOverlay[m_nDisplayMode]) {
			WRect rc(m_screenX - 32.0f, m_screenY - 47.0f, 64.0f, 64.0f);
			WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
			m_pTopHoleOverlay[m_nDisplayMode]->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
		}
	}
}

// fontinfo.cpp:684
void CHoleCupInfo::ResetUiPosition(void) {
	if (m_pDistToHole) {
		m_pDistToHole->ResetUiPosition();
	}
}

// fontinfo.cpp:697
CClubRangeInfo::CClubRangeInfo(void) {
	Init("[font_wind.jpg", NULL);
	m_vPos.x = m_vPos.y = m_vPos.z = 0;

	m_nFlag = 1;
	m_pBuffer = new char[0x20];
}

// fontinfo.cpp:708
void CClubRangeInfo::Display(void) {
	WVector proj = g_view->Projection(m_vPos);

	if (proj.z <= 0.0f || proj.z >= 1.0f)
		return;

	const WVector &ball = GolfBall().m_pos;
	float dx = m_vPos.x - ball.x;
	float dz = m_vPos.z - ball.z;

	sprintf(m_pBuffer, "%.0fy", sqrtf(dz * dz + dx * dx) * 0.3125f);
	Print(proj.x, proj.y - 25.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:737
CFlyInfo::CFlyInfo(void) : m_pGroundUI(NULL), m_pGroundTextUI(NULL), m_pApproachOutOverlay(NULL) {
	Init("[bigfont.jpg", NULL);
	m_pBuffer = new char[0x20];
	m_nFlag = 2;

	m_pGroundUI = new CSimpleUI("[ground.jpg");
	m_pGroundUI->Add("Fairway", WRect(0.0f, 0.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Rough", WRect(0.0f, 25.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Green", WRect(0.0f, 50.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Bunker", WRect(0.0f, 75.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Tee", WRect(0.0f, 100.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Ice", WRect(0.0f, 125.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Sand", WRect(0.0f, 150.0f, 128.0f, 25.0f), false);
	m_pGroundUI->Add("Water", WRect(0.0f, 175.0f, 128.0f, 25.0f), false);

	m_pGroundUI->Add("Vector", WRect(0.0f, 200.0f, 128.0f, 25.0f), false);

	m_pGroundTextUI = new CSimpleUI("[ground_text.jpg");

	m_pGroundTextUI->Add("ºñ°Å¸®", WRect(0.0f, 0.0f, 64.0f, 27.0f), false);
	m_pGroundTextUI->Add("³²Àº°Å¸®", WRect(0.0f, 27.0f, 64.0f, 27.0f), false);

	if (CSharedDoc::Instance()->m_roomType == 10) {
		m_pApproachOutOverlay = g_resrcmng->GetOverlay("approach_out.png", 0);
	}

	m_fTime = 0;
}

// fontinfo.cpp:776
CFlyInfo::~CFlyInfo() {
	if (m_pGroundUI) {
		delete m_pGroundUI;
		m_pGroundUI = NULL;
	}
	if (m_pGroundTextUI) {
		delete m_pGroundTextUI;
		m_pGroundTextUI = NULL;
	}

	if (g_resrcmng && m_pApproachOutOverlay) {
		g_resrcmng->Release(m_pApproachOutOverlay);
		m_pApproachOutOverlay = NULL;
	}
}

// fontinfo.cpp:786
void CFlyInfo::Process(float fDelta) {
	if (GOLFDOC()->GameMode() & 0x80) {
		CGolfBall &ball = GolfBall();

		if (ball.m_state != 0) {
			m_fTime = 0;
			m_fGroundAlpha = 0;
			m_fCarryAlpha = 0;
			m_fToPinAlpha = 0;
			CalcDistance();
			return;
		}

		if (!ball.m_landed && ball.m_lie == 0 && ball.IsHoleIn() != 1) {
			if (WisZero(m_fTime, 1e-5f)) {
				CalcDistance();

				switch (GetPVS().GetGroundType(ball.m_pos, true, 0)) {
				case 1:
					strcpy(m_szGround, "Fairway");
					break;
				case 2:
					strcpy(m_szGround, "Green");
					break;
				case 4:
					strcpy(m_szGround, "Rough");
					break;
				case 3:
					strcpy(m_szGround, "Bunker");
					break;
				case 0:
					strcpy(m_szGround, "Tee");
					break;
				case 7:
					strcpy(m_szGround, "Ice");
					break;
				case 8:
					strcpy(m_szGround, "Vector");
					break;
				case 11:
					strcpy(m_szGround, "Sand");
					break;
				case 14:
					strcpy(m_szGround, "Water");
					break;
				default:
					strcpy(m_szGround, "Rough");
					break;
				}
			}

			fDelta += m_fTime;
			m_fTime = fDelta;
			m_fGroundAlpha = Between(0.0f, fDelta * 510.0f + m_fGroundAlpha, 255.0f);
			m_fCarryAlpha = Between(0.0f, (fDelta - data_9eb5b8[20]) * 510.0f + m_fCarryAlpha, 255.0f);
			m_fToPinAlpha = Between(0.0f, (fDelta - 1.5f) * 510.0f + m_fToPinAlpha, 255.0f);
		}
	}
}

// fontinfo.cpp:866
void CFlyInfo::CalcDistance(void) {
	WVector diff;
	diff = GolfBall().m_pos - GolfBall().PreData(0)->m_pos;

	m_fCarry = sqrtf(diff.x * diff.x + diff.z * diff.z) * 0.3125f;

	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	diff = GolfBall().m_pos - cup;
	m_fToPin = sqrtf(diff.x * diff.x + diff.z * diff.z) * 0.3125f;
}

// fontinfo.cpp:878
void CFlyInfo::Display(void) {
	CGolfBall &ball = GolfBall();

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 10) {
		if (WSingleton<CSharedDoc>::Instance()->m_gameStartTime) {
			m_color = 0xFFFF3434;
			m_nFlag = 0;

			CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
			if (GOLFDOC()->GameMode() & 0x40) {
				sprintf(m_pBuffer, "-");
				Print(100.0f, 202.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
			} else if (GOLFDOC()->GameMode() & 0x80) {
				if (GolfBall().m_state == 0) {
					pDoc = WSingleton<CSharedDoc>::Instance();
					unsigned int dist = pDoc->m_gameResultList[MyIndex()].m_approachDistance;
					if (dist == 0xFFFFFFFF) {
						if (m_pApproachOutOverlay) {
							WRect rc(72.0f, 193.0f, 64.0f, 32.0f);
							WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
							m_pApproachOutOverlay->Render(g_view, uv, rc, 0, 0xFFFF3434, 0.0f, 0);
						}
					} else {
						sprintf(m_pBuffer, "%d.%d", dist / 10, dist % 10);
						Print(80.0f, 202.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
					}
				} else {
					int whole = float2int(m_fToPin);
					sprintf(m_pBuffer, "%d.%d", whole, float2int((m_fToPin - whole) * 10.0f));
					Print(80.0f, 202.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
				}
			} else if (GOLFDOC()->GameMode() & 0x800) {
				unsigned int dist = pDoc->m_gameResultList[MyIndex()].m_approachDistance;
				if (dist == 0xFFFFFFFF) {
					if (m_pApproachOutOverlay) {
						WRect rc(72.0f, 193.0f, 64.0f, 32.0f);
						WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
						m_pApproachOutOverlay->Render(g_view, uv, rc, 0, 0xFFFF3434, 0.0f, 0);
					}
				} else {
					sprintf(m_pBuffer, "%d.%d", dist / 10, dist % 10);
					Print(80.0f, 202.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
				}
			}

			pDoc = WSingleton<CSharedDoc>::Instance();
			if (GOLFDOC()->GameMode() & 0x8C0) {
				int point = pDoc->m_gameResultList[MyIndex()].m_approachPoint;
				if (point == -1) {
					sprintf(m_pBuffer, "-");
					Print(100.0f, 226.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
				} else {
					sprintf(m_pBuffer, "%d.%03d", point / 1000, point % 1000);
					Print(80.0f, 226.0f, m_pBuffer, 2.0f, 0, 0.64f, NULL);
				}
			}
		}
	} else if (GOLFDOC()->GameMode() & 0x80) {
		m_color = 0xFFFFFFFF;

		int carry = float2int(m_fCarry);
		int carryFrac = float2int((m_fCarry - carry) * 100.0f);
		float barY = COption::Instance()->gGetBar_Y();

		float left = g_view->GetWidth() * 0.5f - 20.0f;
		float top = barY - 33.0f;
		float right = g_view->GetWidth() * 0.5f + 35.0f;
		float bottom = g_view->GetHeight() * 0.5f - 37.0f;

		float dx = (right - left) * Between(0.0f, (m_fTime - 0.5f) * 2.0f, 1.0f);
		float dy = (bottom - top) * Between(0.0f, (m_fTime - 0.5f) * 2.0f, 1.0f);

		m_nFlag = 2;
		sprintf(m_pBuffer, "%d", carry);
		float y = top + dy;
		Print(left - 5.0f + dx, y, m_pBuffer, 4.0f, 0, 0.64f, NULL);

		float x = left + dx;
		m_nFlag = 1;
		Print(x, y, ".", 4.0f, 0, 0.64f, NULL);

		m_nFlag = 0;
		if (GOLFDOC()->m_approachMode) {
			sprintf(m_pBuffer, "%dy", carryFrac / 10);
		} else {
			sprintf(m_pBuffer, "%02dy", carryFrac);
		}
		Print(x + 5.0f, y, m_pBuffer, 4.0f, 0, 0.64f, NULL);

		m_color = 0xFFFF3434;
		carry = float2int(m_fToPin);
		carryFrac = float2int((m_fToPin - carry) * 100.0f);

		left = g_view->GetWidth() * 0.5f - 20.0f;
		top = barY - 12.0f;
		right = g_view->GetWidth() * 0.5f + 35.0f;
		bottom = g_view->GetHeight() * 0.5f - 7.0f;

		dx = (right - left) * Between(0.0f, (m_fTime - 1.0f) * 2.0f, 1.0f);
		dy = (bottom - top) * Between(0.0f, (m_fTime - 1.0f) * 2.0f, 1.0f);

		m_nFlag = 2;
		sprintf(m_pBuffer, "%d", carry);
		y = top + dy;
		Print(left - 5.0f + dx, y, m_pBuffer, 4.0f, 0, 0.64f, NULL);

		x = left + dx;
		m_nFlag = 1;
		Print(x, y, ".", 4.0f, 0, 0.64f, NULL);

		m_nFlag = 0;
		if (GOLFDOC()->m_approachMode) {
			if (ball.m_state == 0 && ball.IsHoleIn() == 1) {
				sprintf(m_pBuffer, "%dy", 0);
			} else {
				sprintf(m_pBuffer, "%dy", carryFrac / 10);
			}
		} else {
			sprintf(m_pBuffer, "%02dy", ball.m_state == 0 && ball.IsHoleIn() == 1 ? 0 : carryFrac);
		}
		Print(x + 5.0f, y, m_pBuffer, 4.0f, 0, 0.64f, NULL);

		if (ball.m_state == 0 && !ball.m_landed && ball.IsHoleIn() != 1 && ball.m_lie == 0) {
			if (m_pGroundUI) {
				m_pGroundUI->SetColor(((int)m_fGroundAlpha << 24) | 0xFFFFFF);
				m_pGroundUI->Render(m_szGround, g_view->GetWidth() * 0.5f - 64.0f, g_view->GetHeight() * 0.5f - 72.0f, 1.0f, 0);
			}

			if (m_pGroundTextUI) {
				if (IsLocalContent((localContentType_t)0x4F) && WSingleton<CSharedDoc>::Instance()->m_playingMode == 2) {
					m_fCarryAlpha = 0;
					m_fToPinAlpha = 0;
				}
				m_pGroundTextUI->SetColor(((int)m_fCarryAlpha << 24) | 0xFFFFFF);
				m_pGroundTextUI->Render("\xBA" "\xF1" "\xB0" "\xC5" "\xB8" "\xAE", g_view->GetWidth() * 0.5f - 80.0f, g_view->GetHeight() * 0.5f - 37.0f, 1.0f, 0);
				m_pGroundTextUI->SetColor(((int)m_fToPinAlpha << 24) | 0xFFFFFF);
				m_pGroundTextUI->Render("\xB3" "\xB2" "\xC0" "\xBA" "\xB0" "\xC5" "\xB8" "\xAE", g_view->GetWidth() * 0.5f - 80.0f, g_view->GetHeight() * 0.5f - 7.0f, 1.0f, 0);
			}
		}
	}
}

// fontinfo.cpp:1062
CWindInfo::CWindInfo(void) {
	Init("[font_wind.jpg", NULL);

	m_nFlag = 1;
	m_pBuffer = new char[0x20];
}

// fontinfo.cpp:1070
void CWindInfo::Display(void) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000006
		|| PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000020) {
		m_color = 0xFF00FF00;
		sprintf(m_pBuffer, "1m");
	} else {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000016) {
			m_color = 0xFF0000FF;
		} else if (CSharedDoc::Instance()->m_roomType == 3
			|| (GolfBall().m_preDataIndex >= 0 && Wind().IsInTurbulence(GolfBall().m_pos))
			|| (GolfBall().m_preDataIndex < 0 && Wind().IsInTurbulence(data_00b7bfac))) {
			m_color = 0xFFFFFFFF;
			sprintf(m_pBuffer, "?");
			Print(g_view->GetWidth() - 21.0f, COption::Instance()->gGetBar_Y() + 26.0f, m_pBuffer, 1.0f, 0, 1.0f, NULL);
			return;
		} else {
			m_color = 0xFFFFFFFF;

			switch (CSharedDoc::Instance()->m_roomType) {
			case 0:
			case 1:
			case 7:
				CCardManager::Instance()->SetPlayerIndex(GOLFDOC()->m_myPlayerIndex);
				break;
			case 4:
			case 5:
			case 6:
			case 9:
			case 10:
				CCardManager::Instance()->SetPlayerIndex(0xFF);
				break;
			}

			if (CCardManager::Instance()->GetWindColor() == 1) {
				m_color = 0xFF59D3FF;
			}
		}
		sprintf(m_pBuffer, "%.0fm", Wind().GetGlobalIntensity());
	}

	Print(g_view->GetWidth() - 21.0f, COption::Instance()->gGetBar_Y() + 26.0f, m_pBuffer, 1.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:1132
CBonusInfo::CBonusInfo(void) : m_pPangOverlay(NULL), m_pPcBangOverlay(NULL) {
	Init("[bigfont.jpg", NULL);

	m_nFlag = 2;
	m_pBuffer = new char[0x20];
	m_fTime = 0;

	m_awardPang = 0;
	m_extraPang = 0;
	m_auxPartPang = 0;
	m_itemPang = 0;
	m_totalPang = 0;

	CSharedDoc *pDoc = CSharedDoc::Instance();

	if (pDoc->m_roomType == 7) {
		m_pPangOverlay = g_resrcmng->GetOverlay("[add_pang_skins.png", 0);
	} else {
		m_pPangOverlay = g_resrcmng->GetOverlay("[add_pang.png", 0);
	}

	if (CLoginInfo::Instance()->IsPcBang()) {
		m_pPcBangOverlay = g_resrcmng->GetOverlay("[pc_eventmark.tga", 0);
	}

	if (IsLocalContent((localContentType_t)0x17)) {
		m_pEventOverlay = g_resrcmng->GetOverlay(MakeStr("%s.tga", "z_Halloween_Male"), 0);
	}
}

// fontinfo.cpp:1166
CBonusInfo::~CBonusInfo() {
	if (IsLocalContent((localContentType_t)0x17)) {
		if (g_resrcmng && m_pEventOverlay) {
			g_resrcmng->Release(m_pEventOverlay);
			m_pEventOverlay = NULL;
		}
	}
	if (g_resrcmng && m_pPangOverlay) {
		g_resrcmng->Release(m_pPangOverlay);
		m_pPangOverlay = NULL;
	}
	if (g_resrcmng && m_pPcBangOverlay) {
		g_resrcmng->Release(m_pPcBangOverlay);
		m_pPcBangOverlay = NULL;
	}

	for (std::map<unsigned long, WOverlay *>::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it) {
		if (g_resrcmng && it->second) {
			g_resrcmng->Release(it->second);
			it->second = NULL;
		}
	}
	m_overlays.clear();
}

// fontinfo.cpp:1180
void CBonusInfo::Process(float fDelta) {
	m_fTime += fDelta;

	m_awardPang = PLAYER(MyIndex())->m_ghostAward;
	m_extraPang = PLAYER(MyIndex())->m_extraPang;
	m_auxPartPang = (int)((float)(m_awardPang + m_extraPang) * CSharedDoc::Instance()->GetAuxPartProperty(MyIndex(), 3) * CSharedDoc::Instance()->m_pangPercent * CSharedDoc::Instance()->m_pangRate * 0.01f) - m_awardPang - m_extraPang;

	m_itemPang = PLAYER(MyIndex())->m_itemPang;
	m_totalPang = m_itemPang + m_extraPang + m_awardPang + m_auxPartPang;
}

// fontinfo.cpp:1194
void CBonusInfo::Display(void) {
	if (m_pPangOverlay) {
		WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
		WRect rc;
		rc.x = g_view->GetWidth() * 0.5f - 256.0f;
		if (CSharedDoc::Instance()->m_roomType == 7) {
			rc.w = 256.0f;
			rc.h = 256.0f;
			rc.y = (g_view->GetHeight() - 256.0f - 128.0f + 60.0f) * 0.5f;
		} else {
			rc.w = 256.0f;
			rc.h = 128.0f;
			rc.y = (g_view->GetHeight() - 128.0f) * 0.5f;
		}
		m_pPangOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
	}

	// fontinfo.cpp:1203
	if (CLoginInfo::Instance()->IsPcBang() && m_pPcBangOverlay) {
		WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
		WRect rc(g_view->GetWidth() * 0.5f + 150.0f, (g_view->GetHeight() + 25.0f) * 0.5f, 70.0f, 50.0f);
		m_pPcBangOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
	}

	// fontinfo.cpp:1208
	if (IsLocalContent((localContentType_t)0x17) && CSharedDoc::Instance()->IsWearHalloweenItem()) {
		WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
		WRect rc(g_view->GetWidth() * 0.5f + 170.0f, g_view->GetHeight() * 0.5f - 52.0f, (float)m_pEventOverlay->GetWidth(), (float)m_pEventOverlay->GetHeight());
		m_pEventOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);

		m_nFlag = 2;
		sprintf(m_pBuffer, "%%%d", 100);
		Print(g_view->GetWidth() * 0.5f + 220.0f, g_view->GetHeight() * 0.5f - 18.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);
	}

	// fontinfo.cpp:1220
	int yOff = 0;
	if (CSharedDoc::Instance()->m_roomType == 7) {
		m_nFlag = 2;
		sprintf(m_pBuffer, "%I64d", m_itemPang);
		Print(g_view->GetWidth() * 0.5f + 105.0f, g_view->GetHeight() * 0.5f - 88.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		sprintf(m_pBuffer, "%I64d", m_awardPang);
		Print(g_view->GetWidth() * 0.5f + 100.0f, g_view->GetHeight() * 0.5f - 53.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		sprintf(m_pBuffer, "%I64d", m_extraPang);
		Print(g_view->GetWidth() * 0.5f + 100.0f, g_view->GetHeight() * 0.5f - 18.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		m_nFlag = 0;
		sprintf(m_pBuffer, "%%%I64d", m_auxPartPang);
		Print(g_view->GetWidth() * 0.5f + 105.0f, g_view->GetHeight() * 0.5f - 18.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		m_nFlag = 2;
		sprintf(m_pBuffer, "%I64d", m_totalPang);
		Print(g_view->GetWidth() * 0.5f + 100.0f, g_view->GetHeight() * 0.5f + 25.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		yOff = 0x1C;
	} else {
		m_nFlag = 2;
		sprintf(m_pBuffer, "%I64d", m_awardPang);
		Print(g_view->GetWidth() * 0.5f + 40.0f, g_view->GetHeight() * 0.5f - 53.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		sprintf(m_pBuffer, "%I64d", m_extraPang);
		Print(g_view->GetWidth() * 0.5f + 40.0f, g_view->GetHeight() * 0.5f - 18.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		m_nFlag = 0;
		sprintf(m_pBuffer, "%%%I64d", m_auxPartPang);
		Print(g_view->GetWidth() * 0.5f + 45.0f, g_view->GetHeight() * 0.5f - 18.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);
	}

	// fontinfo.cpp:1266
	WOverlay::DrawBox(g_view, WRect((g_view->GetWidth() - 400.0f) * 0.5f, g_view->GetHeight() * 0.5f + data_9eb5b8[21], 400.0f, 3.0f), 0, 0xFF000000, 0.001f);
	WOverlay::DrawBox(g_view, WRect((g_view->GetWidth() - 400.0f) * 0.5f + data_9eb5b8[20], g_view->GetHeight() * 0.5f + 16.0f, 399.0f, 1.0f), 0, 0xFFFFFFFF, 0.001f);

	// fontinfo.cpp:1274
	CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
	std::vector<unsigned long> &list = pDoc->m_gainedItemList[0];
	int xCur = 0;
	int yCur = yOff;

	if (list.empty()) {
		Print(g_view->GetWidth() * 0.5f + 30.0f, g_view->GetHeight() * 0.5f + yOff + 28.0f, "-", 3.0f, 0, 1.0f, NULL);
		return;
	}

	// fontinfo.cpp:1280
	for (std::vector<unsigned long>::iterator it = list.begin(); it != list.end(); ++it) {
		unsigned long typeId = *it;
		WOverlay *pOverlay = NULL;

		std::map<unsigned long, WOverlay *>::iterator found = m_overlays.find(typeId);
		if (found != m_overlays.end()) {
			pOverlay = found->second;
		} else {
			IFF_ITEM_COMMON *pItem = ItemManager()->FindCommonItem(typeId);
			if (pItem) {
				switch (typeId >> 26) {
				case 2:
				case 5:
				case 6:
				case 0x1D:
					pOverlay = g_resrcmng->GetOverlay(MakeStr("%s.tga", pItem->m_icon), 0);
					if (pOverlay) {
						m_overlays.insert(std::map<unsigned long, WOverlay *>::value_type(typeId, pOverlay));
					}
					break;
				}
			}
		}

		if (pOverlay) {
			WRect rc(g_view->GetWidth() * 0.5f + xCur + data_9eb5b8[21], g_view->GetHeight() * 0.5f + yCur + 25.0f, (float)pOverlay->GetWidth(), (float)pOverlay->GetHeight());
			WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
			if ((typeId >> 26) == 2 || (typeId >> 26) == 0x1D) {
				WRect rcBig(rc.x - 11.0f, rc.y - 11.0f, rc.w, rc.h);
				pOverlay->Render(g_view, uv, rcBig, 0, 0xFFFFFFFF, 0.0f, 0);
			} else {
				pOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
			}

			int adv = pOverlay->GetWidth();
			if (adv <= 40) {
				adv = 40;
			}
			xCur += adv;
			if (xCur > 132) {
				yCur += 40;
				xCur = 0;
			}
		}
	}
}

// fontinfo.cpp:1347
CRemainTimeInfo::CRemainTimeInfo(void) {
	Init("[font_bar.jpg", NULL);
	m_nFlag = 1;
	m_color = 0xFF147EFF;

	m_pBuffer = new char[0x20];
}

// fontinfo.cpp:1356
void CRemainTimeInfo::Display(void) {
	if (CSharedDoc::Instance()->m_shotTimeLimit == 0) {
		sprintf(m_pBuffer, "-");
		Print(79.0f, COption::Instance()->gGetBar_Y() + 34.0f, m_pBuffer, 1.0f, 0, 1.0f, NULL);
	} else {
		unsigned long sec = GOLFDOC()->m_remainShotTime / 1000;
		if (IsLocalCountry(30)) {
			sprintf(m_pBuffer, "%d", Min(GOLFDOC()->m_remainShotTime > sec * 1000 ? sec + 1 : sec, CSharedDoc::Instance()->m_shotTimeLimit / 1000));
		} else {
			sprintf(m_pBuffer, "%d", Min(GOLFDOC()->m_remainShotTime > sec * 1000 ? sec + 1 : sec, CSharedDoc::Instance()->m_shotTimeLimit / 1000));
		}

		Print(79.0f, COption::Instance()->gGetBar_Y() + 34.0f, m_pBuffer, 1.0f, 0, 1.0f, NULL);
	}
}

// fontinfo.cpp:1381
CTimeOutInfo::CTimeOutInfo(void) {
	Init("[bigfont.jpg", NULL);
	m_pBuffer = new char[0x20];

	m_nFlag = 2;
	m_nState = 3;
	m_fAlpha = 0;
}

// fontinfo.cpp:1391
void CTimeOutInfo::Reset(void) {
	m_nState = 0;
	m_fAlpha = 0;
}

// fontinfo.cpp:1397
void CTimeOutInfo::Process(float fDelta) {
	switch (m_nState) {
	case 2:
		m_fTime += fDelta;
		if (m_fTime > 1.5f) {
			m_nState = 1;
		}
		break;

	case 0:
		m_fAlpha += fDelta * 510.0f;
		if (m_fAlpha > 255.0f) {
			m_fAlpha = 255.0f;
			m_nState = 2;
			m_fTime = 0;
		}
		break;

	case 1:
		m_fAlpha -= fDelta * 510.0f;
		if (m_fAlpha < 0) {
			m_fAlpha = 0;
			m_nState = 3;
		}
		break;

	case 3:
		m_fAlpha = 0;
		break;
	}
}

// fontinfo.cpp:1432
void CTimeOutInfo::Display(void) {
	if (m_nState == 3) {
		return;
	}

	sprintf(m_pBuffer, "%d", 3 - PLAYER(GOLFDOC()->m_myPlayerIndex)->m_numTimeout);

	SetColor(((int)m_fAlpha << 24) | 0xFF0000);
	Print(g_view->GetWidth() * 0.5f - 88.0f, g_view->GetHeight() * 0.25f + 3.0f, m_pBuffer, 1.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:1455
CTotalDistInfo::CTotalDistInfo(void) {
	Init("[font_hole.jpg", NULL);
	m_pBuffer = new char[0x20];

	m_nFlag = 1;
}

// fontinfo.cpp:1462
void CTotalDistInfo::Display(void) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	const WVector &ball = GolfBall().m_pos;

	float dx = cup.x - ball.x;
	float dz = cup.z - ball.z;
	m_fDistance = sqrtf(dz * dz + dx * dx + 0.0f) * 0.3125f;

	if (m_fDistance < 1.0f)
		sprintf(m_pBuffer, "%.1fy", m_fDistance);
	else
		sprintf(m_pBuffer, "%.0fy", m_fDistance);

	Print(34.0f, COption::Instance()->gGetBar_Y() + 8.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:1481
// Through a const variable: the original initializes it dynamically (docs/tough_problems/010).
static const float FLOAT_RATE = 30.0f;
static float s_fFloatInterval = 1.0f / FLOAT_RATE;

// fontinfo.cpp:1485
CShotCountInfo::CShotCountInfo(void) {
	Init("[font_hole.jpg", NULL);

	m_pBuffer = new char[0x20];
	m_nFlag = 1;

	if (IsGameType(7)) {

		Init("[font_hole_skins_red.jpg", &m_redFont);
		Init("[font_hole_skins_grey.jpg", &m_greyFont);
		Init("[font_hole_skins_blue.jpg", &m_blueFont);

		m_pCoinOverlay = g_resrcmng->GetOverlay("coin_pang.tga", 0);
	}
}

// fontinfo.cpp:1502
CShotCountInfo::~CShotCountInfo() {
	if (IsGameType(7)) {
		if (g_resrcmng && m_pCoinOverlay) {
			g_resrcmng->Release(m_pCoinOverlay);
			m_pCoinOverlay = NULL;
		}
	}
}

// fontinfo.cpp:1508
void CShotCountInfo::StartFloating(void) {
	m_fFloatTime = s_fFloatInterval;
	m_nFloatCount = 0;
	m_alpha = 255;
}

// fontinfo.cpp:1515
void CShotCountInfo::Process(float fDelta) {
	if (m_fFloatTime <= 0) {
		return;
	}

	m_fFloatTime -= fDelta;
	if (m_fFloatTime < 0) {
		m_nFloatCount++;
		if (m_nFloatCount == 60) {
			m_fFloatTime = 0;
		} else {
			m_alpha -= 4;
			m_fFloatTime = s_fFloatInterval;
		}
	}
}

// fontinfo.cpp:1622
CPowerInfo::CPowerInfo(void) {
	Init("[font_bar.jpg", NULL);
	m_pBuffer = new char[0x20];

	m_nFlag = 1;
}

// fontinfo.cpp:1629
void CPowerInfo::Display(void) {
	m_color = (GOLFDOC()->GetUIAlpha() << 24) | 0xFFFFFF;

	const WVector &ball = GolfBall().m_pos;

	float dx = m_vPos.x - ball.x;
	float dz = m_vPos.z - ball.z;
	float fDistance = sqrtf(dz * dz + dx * dx) * 0.3125f;

	sprintf(m_pBuffer, "%.0fy", fDistance);
	Print(data_af23d0 + data_9eb5b8[28] - 3.0f, COption::Instance()->gGetBar_Y() + data_9eb5b8[27], m_pBuffer, 1.0f, 0, 1.0f, NULL);
	sprintf(m_pBuffer, "%.0fy", fDistance * 0.5f);
	Print(data_af23d0 * 0.5f + 143.0f, COption::Instance()->gGetBar_Y() + data_9eb5b8[27], m_pBuffer, 1.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:1649
CClubTypeInfo::CClubTypeInfo(void) {
	Init("[font_club.jpg", NULL);
	m_pBuffer = new char[0x20];
}

// fontinfo.cpp:1655
void CClubTypeInfo::Display(void) {
	sprintf(m_pBuffer, "%x", GolfClub().GetID() > 15 ? 15 : GolfClub().GetID());
	Print(6.0f, COption::Instance()->gGetBar_Y() - 51.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:1667
CAccuracyInfo::CAccuracyInfo(void) : m_pImpactUI(NULL), m_pBuffer2(NULL) {
	Init("[font_bar.jpg", NULL);
	m_pImpactUI = new CSimpleUI("[impact.png");

	m_bFlicker = false;
	m_pImpactUI->Add("@Bad", WRect(0.0f, 0.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@Pangya", WRect(0.0f, 18.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@PowerSpin", WRect(0.0f, 36.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@PowerCurve", WRect(0.0f, 54.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@Tomahawk", WRect(0.0f, 72.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@Full", WRect(0.0f, 90.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@SuperPangya", WRect(0.0f, 108.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@Cobra", WRect(0.0f, 126.0f, 128.0f, 18.0f), false);

	m_pImpactUI->Add("@Spike", WRect(0.0f, 144.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@PowerTomahawk", WRect(0.0f, 162.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@PowerCobra", WRect(0.0f, 180.0f, 128.0f, 18.0f), false);
	m_pImpactUI->Add("@PowerSpike", WRect(0.0f, 198.0f, 128.0f, 18.0f), false);

	m_pBuffer2 = NULL;
	m_pBuffer = NULL;
	m_pBuffer = new char[0x20];
	m_pBuffer[0] = 0;
	m_pBuffer2 = new char[0x20];
	m_pBuffer2[0] = 0;
	m_fTextX = 0;
	m_nFlag = 1;
}

// fontinfo.cpp:1703
CAccuracyInfo::~CAccuracyInfo() {
	if (m_pImpactUI) {
		delete m_pImpactUI;
		m_pImpactUI = NULL;
	}
	if (m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = NULL;
	}
	if (m_pBuffer2) {
		delete[] m_pBuffer2;
		m_pBuffer2 = NULL;
	}
}

// fontinfo.cpp:1710
void CAccuracyInfo::Process(float fDelta) {
	if (m_bFlicker) {
		m_fAlpha -= fDelta * 128.0f;
		if (m_fAlpha < 0) {
			m_fAlpha = 0;
			m_bFlicker = false;
		}
	}
}

// fontinfo.cpp:1723
void CAccuracyInfo::SetPower(void) {
	m_bFlicker = true;
	m_fAlpha = 255.0f;
	sprintf(Buffer(), "@Full");

	m_fTextX = 436.0f;
}

// fontinfo.cpp:1818
void CAccuracyInfo::Display(void) {
	if (!m_bFlicker || !m_pImpactUI) {
		return;
	}

	if (strchr(m_pBuffer, '@')) {
		if (IsLocalContent((localContentType_t)0x50)) {
			if (stricmp(m_pBuffer, "@Full") != 0 && stricmp(m_pBuffer, "@Bad") != 0 && GOLFDOC()->GetBar().m_accuracyGrade != 4) {
				m_pImpactUI->SetColor(0xFF8A8A8A);
			} else {
				m_pImpactUI->SetColor(((int)m_fAlpha << 24) | 0xFFFFFF);
			}
		} else {
			m_pImpactUI->SetColor(((int)m_fAlpha << 24) | 0xFFFFFF);
		}
		m_pImpactUI->Render(m_pBuffer, m_fTextX, COption::Instance()->gGetBar_Y() - 22.0f, 1.0f, 0);
	}

	if (strchr(m_pBuffer2, '@')) {
		m_pImpactUI->SetColor(((int)m_fAlpha << 24) | 0xFFFFFF);
		m_pImpactUI->Render(m_pBuffer2, m_fTextX, COption::Instance()->gGetBar_Y() - 34.0f, 1.0f, 0);
	}
}

// fontinfo.cpp:1858
CHoleInfo::CHoleInfo(void) : m_pCourseUI(NULL), m_pHoleUI(NULL), m_pStartPosiUI(NULL), m_pPlayerNumUI(NULL), m_pEmotiUI(NULL),
	m_pShuffleBonusOverlay(NULL), m_pCompleteOverlay(NULL), m_pScoreEventOverlay(NULL), m_pExtraStageOverlay(NULL),
	m_pExtraStageSmallOverlay(NULL), m_pCoin1Overlay(NULL), m_pCoin2Overlay(NULL), m_pCoin3Overlay(NULL),
	m_pMapBonusOverlay(NULL), m_pSkinsPang1Overlay(NULL), m_pSkinsPang3Overlay(NULL), m_pApproachOverlay(NULL) {
	Init("[bigfont.jpg", NULL);
	m_pBuffer = new char[0x20];

	m_fTime = 0.0f;
	m_fAlpha = 0.0f;
	m_fTargetAlpha = 0.0f;

	m_items[0].baseX = g_view->GetWidth() * 0.5f - 116.0f;
	m_items[0].baseY = 97.0f;

	m_items[0].curX = -14.0f;
	m_items[0].curY = 8.0f;

	m_items[1].baseX = g_view->GetWidth() * 0.5f - 75.0f;
	m_items[1].baseY = 127.0f;
	m_items[1].curX = 100.0f;
	m_items[1].curY = 7.0f;

	m_items[2].baseX = g_view->GetWidth() * 0.5f - 144.0f;
	m_items[2].baseY = 159.0f;
	m_items[2].curX = 85.0f;
	m_items[2].curY = 30.0f;

	m_items[3].baseX = g_view->GetWidth();
	m_items[3].baseY = 110.0f;

	char szCount[8];
	sprintf(szCount, "%d", (int)CSharedDoc::Instance()->m_gameResultList.size() * 100);
	int len = (int)strlen(szCount);
	m_items[3].curX = g_view->GetWidth() * 0.5f - 50.0f - (m_font.m_pFontInfo->fontw * len * 0.5f + len * 4.0f);
	m_items[3].curY = 5.0f;

	std::map<unsigned int, IFF_STRUCT::sCourse> &courseMap = ItemManager()->m_courseMap;
	m_pCourseUI = new CSimpleUI("[start_name.jpg");

	std::map<unsigned int, IFF_STRUCT::sCourse>::const_iterator it;
	for (it = courseMap.begin(); it != courseMap.end(); ++it) {
		int index = it->second.m_typeId & 0x3FFFFFF;

		WRect rc;
		if (IsLocalContent((localContentType_t)0x26)) {
			if (index >= 14) {
				rc = WRect(256.0f, (float)(index - 14) * 36.0f, 256.0f, 36.0f);
			} else {
				rc = WRect(0.0f, (float)index * 36.0f, 256.0f, 36.0f);
			}
		} else {
			rc = WRect(0.0f, (float)index * 36.0f, 256.0f, 36.0f);
		}

		m_pCourseUI->Add(it->second.m_name, rc, false);
	}

	m_pHoleUI = new CSimpleUI("[start_hole.png");
	m_pHoleUI->Add("Hole", WRect(0.0f, 0.0f, 64.0f, 32.0f), false);
	m_pHoleUI->Add("Par", WRect(0.0f, 32.0f, 64.0f, 32.0f), false);

	if (CSharedDoc::Instance()->m_roomType == 7) {
		m_pHoleUI->Add("Pang", WRect(0.0f, 64.0f, 64.0f, 32.0f), false);
		m_pHoleUI->Add("x2", WRect(0.0f, 96.0f, 64.0f, 32.0f), false);
		m_pHoleUI->Add("x4", WRect(0.0f, 128.0f, 64.0f, 32.0f), false);
		m_pHoleUI->Add("x6", WRect(0.0f, 160.0f, 64.0f, 32.0f), false);
		m_pHoleUI->Add("x8", WRect(0.0f, 192.0f, 64.0f, 32.0f), false);
	}

	if (CSharedDoc::Instance()->m_roomType == 10) {
		m_pApproachOverlay = g_resrcmng->GetOverlay("text_approach.png", 0);
	} else {
		m_pStartPosiUI = new CSimpleUI("[start_posi.jpg");
		m_pPlayerNumUI = new CSimpleUI("[nplayer.jpg");

		if (CSharedDoc::Instance()->m_roomType == 0) {
			m_pStartPosiUI->Add("\xBC\xB1\xB5\xEE", WRect(0.0f, 0.0f, 64.0f, 32.0f), false);

			m_pPlayerNumUI->Add("1", WRect(0.0f, 0.0f, 32.0f, 16.0f), false);
			m_pPlayerNumUI->Add("2", WRect(32.0f, 0.0f, 32.0f, 16.0f), false);
			m_pPlayerNumUI->Add("3", WRect(0.0f, 16.0f, 32.0f, 16.0f), false);
			m_pPlayerNumUI->Add("4", WRect(32.0f, 16.0f, 32.0f, 16.0f), false);
		} else if (CSharedDoc::Instance()->m_roomType == 3) {
			m_pStartPosiUI->Add("vs", WRect(0.0f, 32.0f, 64.0f, 32.0f), false);

			m_pPlayerNumUI->Add("1", WRect(0.0f, 0.0f, 32.0f, 16.0f), false);
			m_pPlayerNumUI->Add("2", WRect(32.0f, 0.0f, 32.0f, 16.0f), false);
		} else {
			m_pStartPosiUI->Add("vs", WRect(0.0f, 32.0f, 64.0f, 32.0f), false);

			m_pPlayerNumUI->Add("red", WRect(0.0f, 32.0f, 32.0f, 16.0f), false);
			m_pPlayerNumUI->Add("blue", WRect(32.0f, 32.0f, 32.0f, 16.0f), false);
		}

		m_pCompleteOverlay = g_resrcmng->GetOverlay("[complete.png", 0);
		m_pScoreEventOverlay = g_resrcmng->GetOverlay("[score_event.png", 0);

		if (CSharedDoc::Instance()->m_roomType == 7) {
			m_pSkinsPang1Overlay = g_resrcmng->GetOverlay("[skins_pang01.png", 0);
			m_pSkinsPang3Overlay = g_resrcmng->GetOverlay("[skins_pang03.png", 0);
			m_pExtraStageOverlay = g_resrcmng->GetOverlay("extrastage.png", 0);
			m_pExtraStageSmallOverlay = g_resrcmng->GetOverlay("extrastagesmall.png", 0);
			m_pCoin1Overlay = g_resrcmng->GetOverlay("P_coin01.png", 0);
			m_pCoin2Overlay = g_resrcmng->GetOverlay("P_coin02.png", 0);
			m_pCoin3Overlay = g_resrcmng->GetOverlay("P_coin03.png", 0);

			m_pEmotiUI = new CSimpleUI("[emoti_object.png");
			m_pEmotiUI->Add("0", WRect(0.0f, 128.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("1", WRect(64.0f, 128.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("2", WRect(128.0f, 128.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("3", WRect(192.0f, 128.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("4", WRect(0.0f, 192.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("5", WRect(64.0f, 192.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("6", WRect(128.0f, 192.0f, 64.0f, 64.0f), false);
			m_pEmotiUI->Add("7", WRect(192.0f, 192.0f, 64.0f, 64.0f), false);

			m_nEmoti = -1;
		}

		m_pShuffleBonusOverlay = g_resrcmng->GetOverlay("text_shufflebonus.png", 0);
		m_pMapBonusOverlay = g_resrcmng->GetOverlay("text_mapbonus.png", 0);
	}
}

// fontinfo.cpp:1997
CHoleInfo::~CHoleInfo() {
	if (m_pCourseUI) { delete m_pCourseUI; m_pCourseUI = NULL; }
	if (m_pHoleUI) { delete m_pHoleUI; m_pHoleUI = NULL; }
	if (m_pStartPosiUI) { delete m_pStartPosiUI; m_pStartPosiUI = NULL; }
	if (m_pPlayerNumUI) { delete m_pPlayerNumUI; m_pPlayerNumUI = NULL; }

	if (g_resrcmng && m_pSkinsPang1Overlay) { g_resrcmng->Release(m_pSkinsPang1Overlay); m_pSkinsPang1Overlay = NULL; }
	if (g_resrcmng && m_pSkinsPang3Overlay) { g_resrcmng->Release(m_pSkinsPang3Overlay); m_pSkinsPang3Overlay = NULL; }
	if (g_resrcmng && m_pApproachOverlay) { g_resrcmng->Release(m_pApproachOverlay); m_pApproachOverlay = NULL; }
	if (g_resrcmng && m_pExtraStageOverlay) { g_resrcmng->Release(m_pExtraStageOverlay); m_pExtraStageOverlay = NULL; }
	if (g_resrcmng && m_pExtraStageSmallOverlay) { g_resrcmng->Release(m_pExtraStageSmallOverlay); m_pExtraStageSmallOverlay = NULL; }
	if (g_resrcmng && m_pCoin1Overlay) { g_resrcmng->Release(m_pCoin1Overlay); m_pCoin1Overlay = NULL; }
	if (g_resrcmng && m_pCoin2Overlay) { g_resrcmng->Release(m_pCoin2Overlay); m_pCoin2Overlay = NULL; }
	if (g_resrcmng && m_pCoin3Overlay) { g_resrcmng->Release(m_pCoin3Overlay); m_pCoin3Overlay = NULL; }
	if (g_resrcmng && m_pShuffleBonusOverlay) { g_resrcmng->Release(m_pShuffleBonusOverlay); m_pShuffleBonusOverlay = NULL; }

	if (m_pEmotiUI) { delete m_pEmotiUI; m_pEmotiUI = NULL; }

	if (g_resrcmng && m_pCompleteOverlay) { g_resrcmng->Release(m_pCompleteOverlay); m_pCompleteOverlay = NULL; }
	if (g_resrcmng && m_pScoreEventOverlay) { g_resrcmng->Release(m_pScoreEventOverlay); m_pScoreEventOverlay = NULL; }

	if (g_resrcmng && m_pMapBonusOverlay) { g_resrcmng->Release(m_pMapBonusOverlay); m_pMapBonusOverlay = NULL; }
}

// fontinfo.cpp:2022
void CHoleInfo::Process(float fDelta) {
	switch (m_nState) {
	case 0: {
		m_fTime += fDelta;

		m_fAlpha += fDelta * 255.0f;
		if (m_fAlpha > 255.0f)
			m_fAlpha = 255.0f;
		m_fTargetAlpha += fDelta * 255.0f;
		if (m_fTargetAlpha > 255.0f)
			m_fTargetAlpha = 255.0f;

		float fWait;
		CSharedDoc *pDoc = CSharedDoc::Instance();
		switch (pDoc->m_roomType) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10:
			if (GOLFDOC()->m_currentHole != pDoc->m_holeOrder[0]) {
				fWait = 1.0f;
				break;
			}
		default:
			fWait = 4.0f;
			break;
		}
		if (pDoc->m_roomType == 10)
			fWait = 2.0f;

		if (pDoc->m_roomType == 7 && !GOLFDOC()->m_approachMode
			&& GOLFDOC()->m_currentHole == pDoc->m_holeOrder[0]) {
			if (m_nEmoti == -1) {
				if (m_fTime > 1.0f) {
					m_nEmoti = 0;
					g_audio->PlaySfx("play_fail_item", NULL, 0, NULL, NULL, 0.5f, 200.0f);
				}
			} else if (m_nEmoti < 8) {
				m_nEmoti = (int)((m_fTime - 1.0f) * 8.0f);
			}
		}

		if (CSharedDoc::Instance()->m_roomType == 10) {
			float t = m_fTargetAlpha * (1.0f / 255.0f);
			m_items[3].fixY = m_items[3].baseY;
			m_items[3].scale = 1.0f;
			m_items[3].fixX = (1.0f - t) * m_items[3].baseX + t * m_items[3].curX;
		}

		if (m_fTime > fWait) {
			m_fTime = 0.0f;
			m_nState = 1;

			if (CSharedDoc::Instance()->m_roomType == 10) {
				m_items[3].baseX = m_items[3].curX;
				m_items[3].curX = m_items[1].curX + 60.0f;
			}
		}
		break;
	}
	case 1:
		m_fTargetAlpha -= fDelta * 510.0f;
		if (m_fTargetAlpha < 0.0f)
			m_fTargetAlpha = 0.0f;

		m_fTime += fDelta * 1.5f;
		if (m_fTime > 1.0f) {
			m_nState = 2;
			m_fTime = 1.0f;
		}

		m_items[0].fixX = (1.0f - m_fTime) * m_items[0].baseX + m_items[0].curX * m_fTime;
		m_items[0].fixY = (1.0f - m_fTime) * m_items[0].baseY + m_items[0].curY * m_fTime;
		m_items[0].scale = (1.0f - m_fTime) + m_fTime * 0.5f;
		m_items[1].fixX = (1.0f - m_fTime) * m_items[1].baseX + m_items[1].curX * m_fTime;
		m_items[1].fixY = (1.0f - m_fTime) * m_items[1].baseY + m_items[1].curY * m_fTime;
		m_items[1].scale = (1.0f - m_fTime) + m_fTime * 0.6f;
		m_items[2].fixX = (1.0f - m_fTime) * m_items[2].baseX + m_items[2].curX * m_fTime;
		m_items[2].fixY = (1.0f - m_fTime) * m_items[2].baseY + m_items[2].curY * m_fTime;
		m_items[2].scale = (1.0f - m_fTime) + m_fTime * 0.6f;
		m_items[3].fixX = (1.0f - m_fTime) * m_items[3].baseX + m_items[3].curX * m_fTime;
		m_items[3].fixY = (1.0f - m_fTime) * m_items[3].baseY + m_items[3].curY * m_fTime;
		m_items[3].scale = (1.0f - m_fTime) + m_fTime * 0.6f;
		break;

	case 2:
		m_fAlpha -= fDelta * 128.0f;
		if (m_fAlpha < 178.5f) {
			m_fAlpha = 178.5f;
			m_nState = 3;
		}
		break;

	case 4:
		m_fTime += fDelta;
		if (m_fTime > 0.25f) {
			m_fTime -= 0.25f;
			m_fAlpha = 255.0f - m_fAlpha;
			if (m_fAlpha > 0.0f) {
				m_nFlickerTime++;
				if (m_nFlickerTime == 3)
					m_nState = 3;
			}
		}
		break;
	}
}

// fontinfo.cpp:2137
void CHoleInfo::Flicker(void) {
	m_nState = 4;
	m_nFlickerTime = 0;
}

// fontinfo.cpp:2143
void CHoleInfo::Reset(void) {
	m_fTime = 0.0f;
	m_nState = 0;
	m_fAlpha = 0.0f;
	m_fTargetAlpha = 0.0f;

	m_items[0].fixX = m_items[0].baseX;
	m_items[0].fixY = m_items[0].baseY;
	m_items[0].scale = 1.0f;

	m_items[1].fixX = m_items[1].baseX;
	m_items[1].fixY = m_items[1].baseY;
	m_items[1].scale = 1.0f;

	m_items[2].fixX = m_items[2].baseX;
	m_items[2].fixY = m_items[2].baseY;
	m_items[2].scale = 1.0f;

	m_firstPlayer = 0;

	for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (GOLFDOC()->GetPlayer(i)->m_state == 3)
			continue;

		if ((GOLFDOC()->GetPlayer(i)->m_playerFlags & 0xF) == 1) {
			m_firstPlayer = i;
			break;
		}
	}
}

// fontinfo.cpp:2178
void CHoleInfo::Fix(void) {
	m_fAlpha = 255.0f;
	m_fTargetAlpha = 255.0f;
	m_items[0].fixY = m_items[0].curY;
	m_items[0].fixX = m_items[0].curX;
	m_items[1].fixY = m_items[1].curY;
	m_items[1].fixX = m_items[1].curX;
	m_items[2].fixX = m_items[2].curX;
	m_fTime = 0.0f;
	m_nState = 3;
	m_items[0].scale = 0.5f;
	m_items[1].scale = 0.6f;
	m_items[2].fixY = m_items[2].curY;
	m_items[2].scale = 0.6f;
}

// fontinfo.cpp:2542
void CHoleInfo::ResetUiPosition(void) {
	m_items[0].baseX = g_view->GetWidth() * 0.5f - 116.0f;
	m_items[0].baseY = 97.0f;
	m_items[0].curX = m_items[0].fixX = -14.0f;
	m_items[0].curY = m_items[0].fixY = 8.0f;

	m_items[1].baseX = g_view->GetWidth() * 0.5f - 75.0f;
	m_items[1].baseY = 127.0f;
	m_items[1].curX = m_items[1].fixX = 100.0f;
	m_items[1].curY = m_items[1].fixY = 6.0f;
}

// fontinfo.cpp:2882
CPangInfo::~CPangInfo() {
	for (int i = 0; i < 2; i++) {
		if (g_resrcmng && m_pPangOverlay[i]) {
			g_resrcmng->Release(m_pPangOverlay[i]);
			m_pPangOverlay[i] = NULL;
		}
	}
}

// fontinfo.cpp:2890
void CPangInfo::ResetUiPosition(void) {
	float fBarY = COption::Instance()->gGetBar_Y() - 108.0f;

	m_totalCases[0].m_fY = fBarY + 23.0f;
	m_totalCases[1].m_fY = fBarY + 23.0f;
	m_totalCases[2].m_fY = fBarY + 23.0f;
	m_totalCases[3].m_fY = fBarY + 23.0f;

	float fWidth = g_view->GetWidth();

	unsigned char roomType = CSharedDoc::Instance()->m_roomType;
	if (roomType == 0 || roomType == 3 || roomType == 7) {
		m_uiPos[0][0] = -25.0f;
		m_uiPos[0][1] = 48.0f;
		m_uiPos[1][0] = -25.0f;
		m_uiPos[1][1] = 141.0f;
		m_uiPos[2][0] = fWidth - 186.0f;
		m_uiPos[2][1] = 48.0f;
		m_uiPos[3][0] = fWidth - 186.0f;
		m_uiPos[3][1] = 141.0f;

		m_uiPos[4][0] = -25.0f;
		m_uiPos[4][1] = 68.0f;
		m_uiPos[5][0] = -25.0f;
		m_uiPos[5][1] = 161.0f;
		m_uiPos[6][0] = fWidth - 186.0f;
		m_uiPos[6][1] = 68.0f;
		m_uiPos[7][0] = fWidth - 186.0f;
		m_uiPos[7][1] = 161.0f;
	} else if (roomType == 1) {
		m_uiPos[0][0] = -25.0f;
		m_uiPos[0][1] = 51.0f;
		m_uiPos[1][0] = -25.0f;
		m_uiPos[1][1] = 100.0f;
		m_uiPos[2][0] = fWidth - 186.0f;
		m_uiPos[2][1] = 51.0f;
		m_uiPos[3][0] = fWidth - 186.0f;
		m_uiPos[3][1] = 100.0f;

		m_uiPos[4][0] = -25.0f;
		m_uiPos[4][1] = 68.0f;
		m_uiPos[5][0] = -25.0f;
		m_uiPos[5][1] = 161.0f;
		m_uiPos[6][0] = fWidth - 186.0f;
		m_uiPos[6][1] = 68.0f;
		m_uiPos[7][0] = fWidth - 186.0f;
		m_uiPos[7][1] = 161.0f;
	}
}

// fontinfo.cpp:3057
bool HaveAngelWing(int nPlayer) {
	sCharacterInfo &info = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[nPlayer].m_characterInfo;
	for (int i = 0; i < 24; i++) {
		switch (info.partTypeIds[i]) {
		case 0x8016800:
		case 0x8058800:
		case 0x8098800:
		case 0x80DC800:
		case 0x8118800:
		case 0x8160800:
		case 0x8190800:
		case 0x81E2800:
		case 0x8214800:
		case 0x8254800:
			return true;
		}
	}
	return false;
}

// fontinfo.cpp:3069
int GetAngelBonus(sPlayerData *player) {
	int grade = player->m_angelWingGrade;
	int bonus = 0;

	if (grade <= 5) {
		bonus = 7;
	} else if (grade >= 6 && grade <= 10) {
		bonus = 5;
	} else if (grade >= 11 && grade <= 20) {
		bonus = 3;
	} else if (grade >= 21) {
		bonus = 1;
	}

	return bonus;
}

// fontinfo.cpp:3179
bool IsEnabledOverDrive(unsigned char byCondition) {
	return byCondition != 19 ? true : false;
}

// fontinfo.cpp:2990
void CPangInfo::Process(float fDelta) {
	if (CSharedDoc::Instance()->m_roomType == 10) {
		return;
	}

	if (GOLFDOC()->m_gameState < 15 && GOLFDOC()->m_gameState != 8) {
		return;
	}

	if (!(GOLFDOC()->GameMode() & 0x81880) || !m_flags) {
		return;
	}

	if ((GOLFDOC()->GameMode() & 0x1000) && CSharedDoc::Instance()->m_roomType != 1 && CSharedDoc::Instance()->m_roomType != 3 && CSharedDoc::Instance()->m_roomType != 7) {
		return;
	}

	UpdateBallState();
	ProcessTotal(fDelta);
	ProcessCases(fDelta);
	CheckReadyToMakeDecision();
	CheckReadyToFinalize(fDelta);

	if (!(m_flags & 8)) {
		RecoveryBonus();
		SuperPangya();
		LongDrive();
		Multiply(1, 2);
		Approach();
		LongPutt();
		LongChipIn();
		BeamImpact();
		SpecialShot();
		NiceRecovery();
		ClashCombo();
		HoleIn();
		Multiply(9, 2);
		Multiply(10, 2);
		DormieBonus();
		HappyBonus();
		AztecBonus();
		AztecBonusWater();
		AngelWingBonus();
		DevilWingBonus();
		if (IsLocalContent((localContentType_t)3)) {
			MascotBonus();
		}
		AbilityItemBonus();
		GimmickItem(GIMMICK_ITEM_PANG_COIN);
		GimmickItem(GIMMICK_ITEM_RANDOM_BOX);
		ClearBonus();
	} else {
		ProcessHappyBonus(fDelta);
		ProcessAztecBonus(fDelta);
		ProcessAngelBonus(fDelta);
		ProcessDevilBonus(fDelta);
	}

	for (int i = 0; i < 24; i++) {
		if (m_cases[i].m_flags & 0x46) {
			m_cases[i].m_fTime += fDelta;
		}
	}

	m_nSubState = GolfBall().m_preDataIndex;
}

// fontinfo.cpp:4831
void CPangInfo::ResetVars(int nFlags, int nCaseState) {
	m_flags = nFlags;
	m_pszSfx = NULL;
	m_nState = 0;
	m_nSubState = 0;
	m_nShowCount = 0;
	m_nShowCase = -1;

	for (int i = 0; i < 4; i++) {
		m_totalCases[i].m_flags = 8;
		m_totalCases[i].m_fTime = 0;

		m_extraPangs[i] = 0;
		m_pangs[i] = 0;
	}

	m_nBonusCount = 0;

	for (int i = 0; i < 24; i++) {
		m_cases[i].m_flags = nCaseState;
		m_cases[i].m_pang = 0;
		m_cases[i].m_nIndex = -1;
		m_cases[i].m_fTime = 0;
		m_cases[i].m_byDone = 0;
	}
}

// fontinfo.cpp:4904
__int64 CPangInfo::GetHoleInPang(int nScore, int nPar) {
	unsigned char course = GetCurMap();

	if (GOLFDOC()->m_gameState < 15)
		course = 0;

	if (nScore == 1)
		return m_holeInPangs[course].m_holeInOne;

	switch (nScore - nPar) {
	case -3:
		return m_holeInPangs[course].m_albatross;
	case -2:
		return m_holeInPangs[course].m_eagle;
	case -1:
		return m_holeInPangs[course].m_birdie;
	case 0:
		return m_holeInPangs[course].m_par;
	default:
		return m_holeInPangs[course].m_perStroke * (nScore - nPar);
	}
}

// fontinfo.cpp:4933
void CPangInfo::GetHoleInOverlayInfo(int nScore, int nPar, unsigned int &overlay, float &fWidth, float &fHeight) {
	int index;
	if (nScore == 1) {
		index = 0;
	} else {
		switch (nScore - nPar) {
		case -3:
			index = 1;
			break;
		case -2:
			index = 2;
			break;
		case -1:
			index = 3;
			break;
		case 0:
			index = 4;
			break;
		default:
			index = 5;
			break;
		}
	}

	overlay = g_holeInPangOverlayInfo[index].m_overlay;

	fWidth = g_holeInPangOverlayInfo[index].m_width * 0.5f;
	fHeight = g_holeInPangOverlayInfo[index].m_height * 0.0625f;
}

// fontinfo.cpp:4967
void CPangInfo::GetSpecialShotOverlayInfo(unsigned long flags, unsigned char index, unsigned int &overlay, float &fWidth, float &fHeight) {
	int type;
	if (flags & 0x10) {
		type = 0;
	} else if (flags & 0x20) {
		type = 1;
	} else {
		type = 2;
	}

	overlay = g_specialShotPangOverlayInfo[type][index].m_overlay;

	fWidth = g_specialShotPangOverlayInfo[type][index].m_width * 0.5f;
	fHeight = g_specialShotPangOverlayInfo[type][index].m_height * 0.0625f;
}

// fontinfo.cpp:5292
void CPangInfo::ProcessHappyBonus(float fDelta) {
	if (m_flags & 8) {
		m_fHappyBonusTime += fDelta;
	}
}

// fontinfo.cpp:4987
void CPangInfo::UpdateBallState(void) {
	if (GolfBall().m_preDataIndex >= 0 && !(GOLFDOC()->GameMode() & 0x1000) && !(m_flags & 0x10)) {
		CGolfBall &ball = GolfBall();

		WVector diff, toCup;
		diff = ball.PreData(ball.m_preDataIndex)->m_pos - ball.PreData(0)->m_pos;
		toCup = GetHoleCupPos() - ball.PreData(ball.m_preDataIndex)->m_pos;

		m_fDriveDistance = sqrtf(diff.x * diff.x + diff.z * diff.z) * 0.3125f;
		m_fToPin = sqrtf(toCup.x * toCup.x + toCup.y * toCup.y + toCup.z * toCup.z) * 0.3125f;
	}
}

// fontinfo.cpp:5300
void CPangInfo::ProcessAztecBonus(float fDelta) {
	if (m_flags & 8) {
		m_fAztecBonusTime += fDelta;
	}
}

// fontinfo.cpp:5308
void CPangInfo::ProcessAngelBonus(float fDelta) {
	if (m_flags & 8) {
		m_fAngelBonusTime += fDelta;
	}
}

// fontinfo.cpp:5316
void CPangInfo::ProcessDevilBonus(float fDelta) {
	if (m_flags & 8) {
		m_fDevilBonusTime += fDelta;
	}
}

// fontinfo.cpp:5414
void CPangInfo::Insert(int nCase) {
	float fBarY = COption::Instance()->gGetBar_Y();

	int nIndex = -1;
	if (nCase == 0) {
		nIndex = 0;
		m_cases[0].m_nIndex = nIndex;
	} else {
		if (nCase == 8 && m_cases[11].m_flags != 0x100) {
			nIndex = m_cases[11].m_nIndex;
			m_cases[8].m_nIndex = nIndex;
		} else if (nCase == 9 || nCase == 10) {
			nIndex = m_cases[8].m_nIndex + 1;
			m_cases[nCase].m_nIndex = nIndex;
		} else if (nCase == 22 || nCase == 23) {
			m_cases[nCase].m_nIndex = m_nState;
		} else if (m_cases[14].m_flags == 0x100 || nCase == 13 || nCase == 14) {
			if (!(m_cases[22].m_flags & 0x101)) {
				nIndex = m_cases[22].m_nIndex;
			}
			if (!(m_cases[23].m_flags & 0x101)) {
				if (nIndex < 0) {
					nIndex = m_cases[23].m_nIndex;
				} else if (m_cases[23].m_nIndex <= nIndex) {
					nIndex = m_cases[23].m_nIndex;
				}
			}
			if (nIndex < 0) {
				m_cases[nCase].m_nIndex = m_nState;
			} else {
				m_cases[nCase].m_nIndex = nIndex;
			}
		} else {
			nIndex = m_cases[14].m_nIndex;
			m_cases[nCase].m_nIndex = nIndex;
		}
	}

	if (nIndex >= 0) {
		for (int i = 0; i < 24; i++) {
			if (i != nCase && !(m_cases[i].m_flags & 0x101) && m_cases[i].m_nIndex >= nIndex) {
				m_cases[i].m_flags |= 0x1000;
				m_cases[i].m_nIndex++;
				m_cases[i].m_fTargetY = fBarY - 108.0f - m_cases[i].m_nIndex * 20.0f;
			}
		}
	}

	m_nState++;
	m_cases[nCase].m_fY = fBarY - 108.0f - m_cases[nCase].m_nIndex * 20.0f;
	m_cases[nCase].m_flags = 2;

	if (nCase == 12 || nCase == 16 || nCase == 18) {
		m_fPopupX = (float)floor(g_view->GetWidth() - 10.0f - 192.6f);
		m_fPopupY = m_cases[nCase].m_fY - 13.44f;
	}
}

// fontinfo.cpp:5506
void CPangInfo::MakeDecision(int nCase, bool bSuccess) {
	m_cases[nCase].m_flags = (m_cases[nCase].m_flags & 0xFFFFF000) | (bSuccess ? 0x10 : 0x40);
	m_cases[nCase].m_fTime = 0;
}

// fontinfo.cpp:5512
void CPangInfo::SuperPangya(void) {
	if (m_cases[1].m_flags == 0x100) {
		return;
	}

	if (m_cases[1].m_flags == 1) {
		Insert(1);
	}
}

// fontinfo.cpp:5521
void CPangInfo::LongDrive(void) {
	if (m_cases[0].m_flags == 0x100) {
		return;
	}

	if (m_fDriveDistance > m_fLongestDrive) {

		if (m_cases[0].m_flags == 1)
			Insert(0);
		m_cases[0].m_pang = (int)(m_fDriveDistance - m_fLongestDrive);
	}

	if (m_flags & 4) {

		if (m_fDriveDistance > m_fLongestDrive && !GolfBall().m_landed)
			m_cases[0].SetState(0x10);
		else
			m_cases[0].SetState(0x40);
	}
}

// fontinfo.cpp:5542
void CPangInfo::Approach(void) {
	if (m_cases[2].m_flags == 0x100 || GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[2].m_flags == 1) {
		Insert(2);
		int nMultiplier = 1;
		if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty()))
			nMultiplier = 2;

		m_cases[2].m_pang = -((10 - (int)m_fToPin) * m_nPangRate * nMultiplier);

		if (m_fStartToPin < 60.0f)
			m_cases[2].m_pang = (m_cases[2].m_pang + 1) / 2;
	} else if (m_flags & 4) {
		m_cases[2].SetState(0x10);
	}
}

// fontinfo.cpp:5564
void CPangInfo::LongPutt(void) {
	if (m_cases[3].m_flags == 0x100) {
		return;
	}

	if (GolfBall().m_state != 0) {
		if (m_cases[3].m_flags == 1 && GolfBall().m_preDataIndex > 0) {
			WVector dir = GolfBall().m_pos - GolfBall().PreData(0)->m_pos;
			dir.y = 0.0f;
			dir.Normalize();

			if (WDotProduct(dir, m_aimDir) > (float)cos(0.017453292f * 20.0))
				Insert(3);
		} else {
			m_cases[3].m_pang = (int)m_fDriveDistance * 2;
		}
	} else if (m_flags & 4) {
		MakeDecision(3, GolfBall().IsHoleIn());
	}
}

// fontinfo.cpp:5650
void CPangInfo::BeamImpact(void) {
	if (m_cases[5].m_flags == 0x100 || GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[5].m_flags == 1) {
		Insert(5);
	} else if (m_flags & 4) {
		m_cases[5].SetState(0x10);
	}
}

// fontinfo.cpp:5661
void CPangInfo::SpecialShot(void) {
	if (m_cases[7].m_flags == 0x100 || GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[7].m_flags == 1) {
		Insert(7);
	} else if (m_flags & 4) {
		m_cases[7].SetState(0x10);
	}
}

// fontinfo.cpp:5672
void CPangInfo::NiceRecovery(void) {
	if (m_cases[0xF].m_flags == 0x100) {
		return;
	}

	if (m_cases[0xF].m_flags == 1) {
		if (GolfBall().m_preDataIndex > GolfBall().m_eventPreDataIndex[6]) {
			Insert(0xF);

			if (GOLFDOC()->m_pPolySoup->IsOutOfBound(GolfBall().PreData(GolfBall().m_eventPreDataIndex[6])->m_pos)) {
				m_cases[0xF].m_pang = 15;
			} else {
				m_cases[0xF].m_pang = 10;
			}
		}
	} else if (m_flags & 4) {
		if (GOLFDOC()->m_pPolySoup->IsOutOfBound(GolfBall().PreData(GolfBall().m_eventPreDataIndex[6])->m_pos)) {
			MakeDecision(0xF, !GOLFDOC()->m_pPolySoup->IsOutOfBound(GolfBall().PreData(GolfBall().m_preDataCount)->m_pos));
		} else {
			unsigned char ground = GetPVS().GetGroundType(GolfBall().PreData(GolfBall().m_preDataCount)->m_pos, true, NULL);
			MakeDecision(0xF, ground == 1 || ground == 2);
		}
	}
}

// fontinfo.cpp:5704
void CPangInfo::HoleIn(void) {
	if (m_cases[8].m_flags == 0x100 || GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[8].m_flags == 1) {
		Insert(8);
	} else if (m_flags & 4) {
		if (CSharedDoc::Instance()->m_roomType == 1) {
			int team = PLAYER(MyIndex())->m_attribute;

			if (TEAM(1 - team)->m_holes[GOLFDOC()->m_currentHole - 1].GetScore() >= TEAM(team)->m_holes[GOLFDOC()->m_currentHole - 1].GetScore()) {
				m_cases[8].SetState(0x10);
			} else {
				m_cases[8].SetState(0x40);
			}
		} else if (CSharedDoc::Instance()->m_roomType == 3) {
			if ((unsigned char)PLAYER(1 - MyIndex())->m_holes[GOLFDOC()->m_currentHole - 1] >= (unsigned char)PLAYER(MyIndex())->m_holes[GOLFDOC()->m_currentHole - 1]) {
				m_cases[8].SetState(0x10);
			} else {
				m_cases[8].SetState(0x40);
			}
		} else {
			m_cases[8].SetState(0x10);
		}
	}
}

// fontinfo.cpp:5734
void CPangInfo::Multiply(int nCase, int nMultiply) {
	if (m_cases[nCase].m_flags == 0x100 || GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[nCase].m_flags == 1) {
		Insert(nCase);
		m_cases[nCase].m_pang = nMultiply;
	} else if (m_flags & 4) {
		if (nCase == 1) {
			MakeDecision(nCase, (m_cases[0].m_flags >> 4) & 1);
		} else if (IsGameType(1) || IsGameType(3)) {
			MakeDecision(nCase, m_cases[8].m_flags != 0x100);
		} else {
			MakeDecision(nCase, GolfBall().IsHoleIn() && m_nPangRate <= 0);
		}
	}
}

// fontinfo.cpp:5788
void CPangInfo::DormieBonus(void) {
	if (!IsGameType(1) && !IsGameType(3) || m_cases[6].m_flags == 0x100) {
		return;
	}

	if (m_cases[6].m_flags == 1) {
		Insert(6);
	} else if (m_flags & 4) {
		m_cases[6].SetState(0x10);
	}
}

// fontinfo.cpp:5799
void CPangInfo::HappyBonus(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		return;
	}

	if (GolfBall().m_state != 0 || m_cases[0xC].m_flags == 0x100) {
		return;
	}

	if (m_cases[0xC].m_flags == 1) {
		Insert(0xC);
	} else if (m_flags & 4) {
		m_cases[0xC].m_pang = 20;
		m_cases[0xC].SetState(0x10);
	}
}

// fontinfo.cpp:5945
void CPangInfo::AngelWingBonus(void) {
	if (m_cases[0x12].m_flags == 0x100) {
		return;
	}

	if (m_cases[0x12].m_flags == 1) {
		Insert(0x12);
	} else if (m_flags & 4) {
		m_cases[0x12].m_pang = 55;
		m_cases[0x12].SetState(0x40);
	}
}

// fontinfo.cpp:5961
void CPangInfo::DevilWingBonus(void) {
	if (m_cases[0x13].m_flags == 0x100) {
		return;
	}

	if (m_cases[0x13].m_flags == 1) {
		Insert(0x13);
	} else if (m_flags & 4) {
		m_cases[0x13].m_pang = 1;
		m_cases[0x13].SetState(0x10);
	}
}

// fontinfo.cpp:5977
void CPangInfo::MascotBonus(void) {
	if (m_cases[0x14].m_flags == 0x100) {
		return;
	}

	if (m_cases[0x14].m_flags == 1) {
		Insert(0x14);
	} else if (m_flags & 4) {
		m_cases[0x14].SetState(0x10);
	}
}

// fontinfo.cpp:5988
void CPangInfo::AbilityItemBonus(void) {
	if (m_cases[0x15].m_flags == 0x100) {
		return;
	}

	if (m_cases[0x15].m_flags == 1) {
		Insert(0x15);
	} else if (m_flags & 4) {
		m_cases[0x15].SetState(0x10);
	}
}

// fontinfo.cpp:5999
void CPangInfo::GimmickItem(GIMMICK_ITEM_TYPE itemType) {
	int nCase;

	switch (itemType) {
	case 0:
		nCase = 0x16;
		break;
	case 1:
		nCase = 0x17;
		break;
	default:
		return;
	}

	if (GolfBall().m_state == 0) {
		if (m_cases[nCase].m_flags == 1) {
			m_cases[nCase].m_flags = 0x100;
		}
	}

	sPangCase &pangCase = m_cases[nCase];
	if (pangCase.m_flags != 0x100) {
		GroundItemMan *pMan = (GroundItemMan *)AfxGetTask()->GetActor("GroundItemMan");
		if (pMan) {
			int nCount = pMan->m_playerStats[GOLFDOC()->m_myPlayerIndex].m_shotCount[itemType];
			if (nCount > 0) {
				if (pangCase.m_flags == 1) {
					Insert(nCase);
				}
				m_cases[nCase].m_pang = nCount;
			}
		}

		if (m_flags & 4) {
			pangCase.SetState(0x10);
			m_cases[nCase].m_fTime = 0;
		}
	}
}

// fontinfo.cpp:6047
void CPangInfo::AztecBonus(void) {
	CSharedDoc *pDoc = CSharedDoc::Instance();

	if ((pDoc->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId & 0x3FFFFFF) == 0 || m_cases[0x10].m_flags == 0x100) {
		return;
	}

	if (m_cases[0x10].m_flags == 1) {
		Insert(0x10);
	} else if (m_flags & 4) {
		m_cases[0x10].m_pang = 30;
		m_cases[0x10].SetState(0x10);
	}
}

// fontinfo.cpp:6102
void CPangInfo::ClearBonus(void) {
	switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
	case 4:
	case 5:
	case 6:
	case 9:
	case 10:
		return;
	}

	if (m_cases[0xD].m_flags == 0x100 || (GOLFDOC()->GameMode() & 0x80) && GolfBall().m_state != 0) {
		return;
	}

	if (m_cases[0xD].m_flags == 1) {
		Insert(0xD);
	} else if (m_flags & 4) {
		m_cases[0xD].SetState(0x10);
	}
}

// fontinfo.cpp:6116
void CPangInfo::RecoveryBonus(void) {
	if (m_cases[0xE].m_flags == 0x100) {
		return;
	}

	if (m_cases[0xE].m_flags == 1) {
		Insert(0xE);
	} else if (m_flags & 4) {
		MakeDecision(0xE, m_pangs[0] > 0 || m_pangs[1] > 0);
	}
}

// fontinfo.cpp:6664
void CPangInfo::Restore(void) {
	m_flags = 2;
	m_nState = 0;
	m_pszSfx = NULL;
	m_nSubState = 0;
	m_nShowCount = 0;
	m_nShowCase = -1;

	for (int i = 0; i < 4; i++) {
		m_totalCases[i].m_flags = 8;
		m_totalCases[i].m_pang = m_bonusPangs[i];
		m_shownPangs[i] = m_totalCases[i].m_pang;
		m_totalCases[i].m_fTime = 0;
	}

	m_nBonusCount = 0;

	for (int j = 0; j < 24; j++) {
		if (m_cases[j].m_flags != 0x100)
			m_cases[j].m_flags = 1;
		m_cases[j].m_nIndex = -1;
		m_cases[j].m_fTime = 0;
		m_cases[j].m_byDone = 0;
	}

	m_cases[11].m_pang = 1;
}

// fontinfo.cpp:6698
void CPangInfo::Skip(void) {
	if (m_pszSfx) {
		g_audio->StopSfx(m_pszSfx);
	}

	for (int i = 0; i < 4; i++) {
		int nPang = (int)(m_bonusPangs[i] + m_pangs[i]);
		m_totalCases[i].m_pang = nPang > 0 ? nPang : 0;
	}
}

// fontinfo.cpp:6723
CCalipers::CCalipers(void) {
	Init("[font_bar.jpg", NULL);
	m_pBuffer = new char[0x20];
	m_nFlag = 1;

	m_pIndicatorOverlay = g_resrcmng->GetOverlay("{indicator.bmp", 0);

	m_pCalipersOverlay = g_resrcmng->GetOverlay("{callipers_o.png", 0);
	m_fSoundTime = 0;
	m_bKeyMoving = false;
	m_fDisplayPos = 0;
	m_fMoveDelta = 0;
	m_bSoundPlayed = false;
	m_bResetFlag = true;

	m_fsm.addStateTransition(0, 1, 1);
	m_fsm.addStateTransition(0, 2, 2);
	m_fsm.addStateTransition(1, 2, 0);
	m_fsm.addStateTransition(1, 0, 0);
	m_fsm.addStateTransition(2, 1, 0);
	m_fsm.addStateTransition(2, 0, 0);

	m_fsm.setCurrentState(0);

	ResetBar();
}

// fontinfo.cpp:6755
CCalipers::~CCalipers() {
	if (g_resrcmng && m_pIndicatorOverlay) {
		g_resrcmng->Release(m_pIndicatorOverlay);
		m_pIndicatorOverlay = NULL;
	}
	if (g_resrcmng && m_pCalipersOverlay) {
		g_resrcmng->Release(m_pCalipersOverlay);
		m_pCalipersOverlay = NULL;
	}
}

// fontinfo.cpp:6774
void CCalipers::SetCalipersLimit(float fLimit) {
	m_fCursorPos = m_fDisplayPos = fLimit;

	switch (GOLFDOC()->GameMode()) {
	case 8:
	case 0x100:
	case 0x200:
	case 0x8000:
	case 0x40000:
		if (m_nPowerBarState == 1) {
			float fMin = GolfClub().GetNarrowAccuracy() + data_af23d0 * 0.0f + data_9eb5b8[28];
			GOLFDOC()->GetBar().m_impact = fLimit < fMin ? fMin : (fLimit > data_9eb5b8[26] ? data_9eb5b8[26] : fLimit);
		}
		break;
	}
}

// fontinfo.cpp:6798
void CCalipers::ProcKeyState(float fDelta) {
	if (CMouseCursor::Instance()->m_area == 5) {
		m_bAccuracyFixed = true;
		return;
	}

	int nLeft = g_input->Get("CALIPERS_LEFT", true);
	int nRight = g_input->Get("CALIPERS_RIGHT", true);
	if (nLeft == 1) {
		m_keyFlags |= nLeft;
	}
	if (nRight == 1) {
		m_keyFlags |= 2;
	}

	unsigned long flags = m_keyFlags;
	int nAction = 0;
	if (flags == 0) {
		if (m_prevKeyFlags == 1 || m_prevKeyFlags == 2) {
			data_af23d8 = 0;
			m_fBarScale = 0.35f;
		}
	} else if (data_af23d8 > m_fBarScale && (flags == 1 || flags == 2)) {
		nAction = 2;
		data_af23d8 = data_af23d8 - m_fBarScale;
		m_fBarScale = 0.05f;
	} else if (!((flags & 1) && (flags & 2)) && flags != 0) {
		if (data_af23d8 > data_9eb5b8[7]) {
			nAction = 1;
			data_af23d8 = data_af23d8 + fDelta;
		} else if (flags != 0 && !(data_af23d8 < data_9eb5b8[7])) {
			data_af23d8 = data_af23d8 + fDelta;
		}
	}

	ChangeKeyState();

	if (nAction != 0) {
		unsigned long state = 0;
		switch (nAction) {
		case 1:
			if (!m_bPowerFixed) {
				m_bPowerFixed = true;
				GOLFDOC()->GetBar().m_impact = m_fDisplayPos;
				m_nPowerBarState = 1;
			} else {
				ExecuteKeyAction(m_curState);
			}
			return;
		case 2:
			state = m_curState;
			break;
		}

		switch (state) {
		case 0:
			break;
		case 1:
			MoveLeft();
			break;
		case 2:
			MoveRight();
			break;
		}
	}

	m_prevKeyFlags = m_keyFlags;
	m_keyFlags = 0;
}

// fontinfo.cpp:6882
void CCalipers::ExecuteKeyAction(int nKey) {
	switch (nKey) {
	case 0:
		MoveNone();
		break;

	case 1:
		MoveLeft();
		break;

	case 2:
		MoveRight();
		break;
	}
}

// fontinfo.cpp:6898
void CCalipers::ChangeKeyState(void) {
	switch (m_curState) {
	case 0:
		if (m_keyFlags & 1) {
			SetEvent(1);
		} else if (m_keyFlags & 2) {
			SetEvent(2);
		}

		m_fMoveSpeed = 1.0f;
		break;

	case 1:
		if (m_prevState != 1) {
			if (!(m_keyFlags & 1)) {
				KeyInputEnd();
			}
		}

		if (m_keyFlags & 2) {
			SetEvent(2);
		}
		break;

	case 2:
		if (m_prevState != 2) {
			if (!(m_keyFlags & 2)) {
				KeyInputEnd();
			}
		}

		if (m_keyFlags & 1) {
			SetEvent(1);
		}
		break;
	}
}

// fontinfo.cpp:6958
void CCalipers::SetEvent(int nEvent) {
	m_fsm.stateTransition(nEvent);
	m_prevState = m_curState;
	m_curState = m_fsm.getCurrentStateID();
}

// fontinfo.cpp:6965
void CCalipers::MoveNone(void) {
}

// fontinfo.cpp:6970
void CCalipers::MoveLeft(void) {
	m_bKeyMoving = true;

	if (GolfClub().GetType() != 3) {
		m_fCursorPos = GolfClub().GetNarrowAccuracy() + data_9eb5b8[28] > m_fCursorPos - m_fMoveSpeed ? GolfClub().GetNarrowAccuracy() + data_9eb5b8[28] : m_fCursorPos - m_fMoveSpeed;
	} else {
		m_fCursorPos = 144.0f > m_fCursorPos - m_fMoveSpeed ? 144.0f : m_fCursorPos - m_fMoveSpeed;
	}

	if (m_fMoveSpeed <= data_9eb5b8[21]) {
		m_fMoveSpeed += 0.5f;
	}

	m_fDisplayPos = m_fCursorPos;
}

// fontinfo.cpp:6988
void CCalipers::MoveRight(void) {
	m_bKeyMoving = true;

	float pos = m_fCursorPos - m_fMoveSpeed * -1.0f;
	pos = data_9eb5b8[26] < pos ? data_9eb5b8[26] : pos;
	m_fCursorPos = pos;

	if (m_fMoveSpeed <= data_9eb5b8[21]) {
		m_fMoveSpeed += 0.5f;
	}

	m_fDisplayPos = pos;
}

// fontinfo.cpp:7003
void CCalipers::KeyInputEnd(void) {
	m_fMoveSpeed = 1.0f;
	SetEvent(0);
	m_bKeyMoving = false;
}

// fontinfo.cpp:7101
void CCalipers::Reset(void) {
	m_fMoveDelta = 0;
	m_bResetFlag = true;
	m_bKeyMoving = false;

	if (IsLocalContent((localContentType_t)0x4F)) {
		if (WSingleton<CSharedDoc>::Instance()->m_replayPlayType != 1)
			PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId = 0;
	} else {
		PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId = 0;
	}

	float y = COption::Instance()->gGetBar_Y() - 23.0f;
	if (g_input->GetMousePoint().y > y)
		g_input->SetMousePoint(g_input->GetMousePoint().x, y);
}

// fontinfo.cpp:7120
void CCalipers::ResetBar(void) {
	m_fBarScale = 0.5f;
	m_prevKeyFlags = 0;
	m_keyFlags = 0;
	m_curState = 0;
	m_prevState = 0;
	m_fMoveSpeed = 1.0f;
	m_nPowerBarState = 0;
	m_bPowerFixed = false;
	m_bAccuracyFixed = false;
}

// fontinfo.cpp:7135
void CCalipers::DrawShotPowerRange(float &fPos, bool bImpact) {
	float fOffset = GolfClub().GetType() == 3 ? -8.0f : 0.0f;
	if (bImpact == true) {
		fPos = GOLFDOC()->GetBar().m_impact;
	} else {
		fPos = GOLFDOC()->GetBar().m_power;
	}

	if (fPos > data_9eb5b8[26]) {
		fPos = 500.0f;
		sprintf(m_pBuffer, "%.1fy", GolfClub().GetRange(NULL));
	} else {
		sprintf(m_pBuffer, "%.1fy", GolfClub().GetRange(NULL) * ((fPos - data_9eb5b8[28]) / data_af23d0));
	}

	SetColor(0xFF1AFD00);
	Print(fPos + 3.0f, COption::Instance()->gGetBar_Y() - 40.0f + fOffset, m_pBuffer, 1.0f, 0, 1.0f, NULL);
}

// fontinfo.cpp:7161
void CCalipers::DrawLimitLine(float &fPos, bool bImpact) {
	float fUvH = 1.0f;
	float fRcH = 32.0f;
	if (!bImpact) {
		fUvH = 0.375f;
		fRcH = 12.0f;
	}

	if (!GOLFDOC()->m_clockHold && GolfClub().GetType() != 3) {
		WRect uv(0.75f, 0.0f, 0.125f, fUvH);
		WRect rc(fPos - 7.0f, COption::Instance()->gGetBar_Y() - 21.0f, 16.0f, fRcH);
		m_pCalipersOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
		return;
	}

	if (m_bSoundPlayed) {
		WRect uv(0.75f, 0.0f, 0.125f, fUvH);
		WRect rc(fPos - 7.0f, COption::Instance()->gGetBar_Y() - 21.0f, 16.0f, fRcH);
		m_pCalipersOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);

		if (m_fSoundTime >= 0.1f && m_fSoundTime <= 0.2f) {
			WRect uv2(0.875f, 0.0f, 0.125f, fUvH);
			WRect rc2(fPos - 7.0f, COption::Instance()->gGetBar_Y() - 21.0f, 16.0f, fRcH);
			m_pCalipersOverlay->Render(g_view, uv2, rc2, 0, 0xFFFFFFFF, 0.0f, 0);
		}
	} else {
		WRect uv(0.75f, 0.0f, 0.125f, fUvH);
		WRect rc(fPos - 7.0f, COption::Instance()->gGetBar_Y() - 21.0f, 16.0f, fRcH);
		m_pCalipersOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);
	}
}

// fontinfo.cpp:7196
void CCalipers::Process(float fDelta) {
	ProcKeyState(fDelta);

	if (!GOLFDOC()->m_clockHold) {
		WVector mouse = g_input->GetMousePoint();
		float fMouseX = mouse.x;

		if (CMouseCursor::Instance()->m_area == 5) {
			float fLimit = GolfClub().GetType() == 3 ? data_9eb5b8[28] : GolfClub().GetNarrowAccuracy() + data_9eb5b8[28];

			if (mouse.x > fLimit) {
				if (mouse.x > data_9eb5b8[26]) {
					fMouseX = 500.0f;
				}

				float fPos = fMouseX;
				m_fDisplayPos = fPos;
				m_fCursorPos = fPos;
				m_fMoveDelta = fPos;
				GOLFDOC()->GetBar().m_impact = (float)floor(Between(GolfClub().GetNarrowAccuracy() + data_9eb5b8[28], fPos, 500.0f));

				CMouseCursor::Instance()->SetCursor(7);
				m_bKeyMoving = false;
			}
		} else {
			if (m_bAccuracyFixed == true) {
				GOLFDOC()->GetBar().m_impact = 0;
				m_bAccuracyFixed = false;
				m_bPowerFixed = false;
				m_nPowerBarState = 0;
			}

			if (m_bKeyMoving) {
				GOLFDOC()->GetBar().m_impact = (float)floor(Between(GolfClub().GetNarrowAccuracy() + data_af23d0 * 0.0f + data_9eb5b8[28], m_fDisplayPos, 500.0f));
			}
		}
	}

	if (m_pCalipersOverlay) {
		if (GOLFDOC()->GetBar().m_impact > data_9eb5b8[28]) {
			if (m_fDisplayPos > data_9eb5b8[28]) {
				if (m_fDisplayPos - 5.0f <= GOLFDOC()->GetBar().m_accuracy && m_fDisplayPos + 5.0f >= GOLFDOC()->GetBar().m_accuracy) {
					if (!m_bSoundPlayed && m_bResetFlag) {
						if ((GOLFDOC()->m_clockHold == 2 && GOLFDOC()->GetBar().m_state == -1) || GolfClub().GetType() == 3) {
							m_bSoundPlayed = true;
							m_fSoundTime = 0;
							m_bResetFlag = false;
							g_audio->PlaySfx("½ÃÄÀ°Ë¸»", NULL, 0, NULL, NULL, 0.5f, 200.0f);
						}
					}
				}
			}
		}

		if (m_bSoundPlayed) {
			m_fSoundTime = m_fSoundTime + fDelta;
			if (m_fSoundTime > 0.4f) {
				m_bSoundPlayed = false;
				m_fSoundTime = 0;
			}
		}
	}
}

// fontinfo.cpp:7271
void CCalipers::Display(void) {
	WRect uv(0.0f, 0.0f, 0.5f, 1.0f);
	WRect rc(150.0f, COption::Instance()->gGetBar_Y() + data_9eb5b8[21], 64.0f, 32.0f);
	m_pCalipersOverlay->Render(g_view, uv, rc, 0, 0xFFFFFFFF, 0.0f, 0);

	if (m_nPowerBarState == 1) {
		float fPos = 0;
		DrawShotPowerRange(fPos, true);

		if (m_pCalipersOverlay) {
			WVector mouse = g_input->GetMousePoint();
			float fLimit = GolfClub().GetType() == 3 ? data_9eb5b8[28] : GolfClub().GetNarrowAccuracy() + data_9eb5b8[28];

			if (mouse.x > fLimit) {
				if (g_input->Get("CALIPERS_LEFT", true) || m_fMoveDelta - m_fDisplayPos > 0) {
					WRect rcLeft(152.0f, COption::Instance()->gGetBar_Y() + 22.0f, 32.0f, 32.0f);
					WRect uvArrow(0.5f, 0.0f, 0.25f, 1.0f);
					m_pCalipersOverlay->Render(g_view, uvArrow, rcLeft, 0, 0xFFFFFFFF, 0.0f, 0);
				}

				if (g_input->Get("CALIPERS_RIGHT", true) || (m_fMoveDelta > 0 && m_fMoveDelta - m_fDisplayPos < 0)) {
					WRect rcRight(174.0f, COption::Instance()->gGetBar_Y() + 22.0f, 32.0f, 32.0f);
					WRect uvArrow(0.5f, 0.0f, 0.25f, 1.0f);
					m_pCalipersOverlay->Render(g_view, uvArrow, rcRight, 0, 0xFFFFFFFF, 0.0f, 0);
				}
			}

			DrawLimitLine(fPos, true);
		}
	}
}

// fontinfo.cpp:7307
CHolePoseSkins::CHolePoseSkins(void) {
	Init("[bigfont.jpg", NULL);

	m_pBuffer = new char[0x20];

	m_nValue = 0;
	m_nResult = 0;
	m_fAlpha = 0.0f;
	m_nState = 0;
	m_fScale = 2.0f;

	m_pWinOverlay[0] = g_resrcmng->GetOverlay("skins_win01.jpg", 0);
	m_pWinOverlay[1] = g_resrcmng->GetOverlay("skins_win02.jpg", 0);
	m_pWinOverlay[2] = g_resrcmng->GetOverlay("skins_win03.jpg", 0);

	m_pLoseOverlay[0] = g_resrcmng->GetOverlay("skins_lose01.jpg", 0);
	m_pLoseOverlay[1] = g_resrcmng->GetOverlay("skins_lose02.jpg", 0);

	m_pDrawOverlay[0] = g_resrcmng->GetOverlay("skins_draw01.jpg", 0);
	m_pDrawOverlay[1] = g_resrcmng->GetOverlay("skins_draw02.jpg", 0);
}

// fontinfo.cpp:7331
CHolePoseSkins::~CHolePoseSkins() {
	if (g_resrcmng && m_pWinOverlay[0]) {
		g_resrcmng->Release(m_pWinOverlay[0]);
		m_pWinOverlay[0] = NULL;
	}
	if (g_resrcmng && m_pWinOverlay[1]) {
		g_resrcmng->Release(m_pWinOverlay[1]);
		m_pWinOverlay[1] = NULL;
	}
	if (g_resrcmng && m_pWinOverlay[2]) {
		g_resrcmng->Release(m_pWinOverlay[2]);
		m_pWinOverlay[2] = NULL;
	}

	if (g_resrcmng && m_pLoseOverlay[0]) {
		g_resrcmng->Release(m_pLoseOverlay[0]);
		m_pLoseOverlay[0] = NULL;
	}
	if (g_resrcmng && m_pLoseOverlay[1]) {
		g_resrcmng->Release(m_pLoseOverlay[1]);
		m_pLoseOverlay[1] = NULL;
	}

	if (g_resrcmng && m_pDrawOverlay[0]) {
		g_resrcmng->Release(m_pDrawOverlay[0]);
		m_pDrawOverlay[0] = NULL;
	}
	if (g_resrcmng && m_pDrawOverlay[1]) {
		g_resrcmng->Release(m_pDrawOverlay[1]);
		m_pDrawOverlay[1] = NULL;
	}
}

// fontinfo.cpp:7344
void CHolePoseSkins::Process(float fDelta) {
	if (!(GOLFDOC()->GameMode() & 0x1000)) {
		m_nState = 0;
		return;
	}

	switch (m_nState) {
	case 1:
		if (m_fScale > 0.0f) {
			m_fScale -= fDelta;
			if (m_fScale < 0.0f) {
				m_nState = 2;
			}
		}
		break;
	case 2:
		m_fAlpha -= fDelta * 510.0f;
		if (m_fAlpha < 0.0f) {
			m_fAlpha = 0;
			m_nState = 0;
		}
		break;
	}
}

// fontinfo.cpp:7375
void CHolePoseSkins::SetPose(int nResult, int nValue) {
	if (m_nState != 0) {
		return;
	}

	m_nResult = nResult;
	m_nValue = nValue;
	m_fAlpha = 255.0f;
	m_nState = 1;
	m_fScale = 2.0f;

	if (nResult > 0) {
		m_pPoseOverlay[0] = m_pWinOverlay[0];
	} else if (nResult == 0) {
		m_pPoseOverlay[0] = m_pDrawOverlay[0];
		m_pPoseOverlay[1] = m_pDrawOverlay[1];
	} else if (nResult < 0) {
		m_pPoseOverlay[0] = m_pLoseOverlay[0];
		m_pPoseOverlay[1] = m_pLoseOverlay[1];
	}
}

// fontinfo.cpp:7402
void CHolePoseSkins::Display(void) {
	if (m_nState == 0) {
		return;
	}

	WRect uv(0.0f, 0.0f, 1.0f, 1.0f);
	WRect rc;
	unsigned long color = ((int)m_fAlpha << 24) + 0xFFFFFF;

	if (m_pPoseOverlay[0]) {
		float fW = (float)m_pPoseOverlay[0]->GetWidth();
		float fH = (float)m_pPoseOverlay[0]->GetHeight();
		rc.x = g_view->GetWidth() * 0.5f - fW * 0.5f;
		rc.y = g_view->GetHeight() * 0.25f - fH * 0.5f;
		rc.w = fW;
		rc.h = fH;
		m_pPoseOverlay[0]->Render(g_view, uv, rc, 0, color, 0.0f, 0);
	}

	if (m_nResult > 0) {
		sprintf(m_pBuffer, "%d", m_nValue);
		int len = (int)strlen(m_pBuffer);
		float fTextW = (float)(m_font.m_pFontInfo->fontw * len) * 0.5f + len * 6.0f;

		if (m_pWinOverlay[1]) {
			float fW = (float)m_pWinOverlay[1]->GetWidth();
			float fH = (float)m_pWinOverlay[1]->GetHeight();
			rc.x = g_view->GetWidth() * 0.5f - fTextW * 0.5f - fW;
			rc.y = rc.y + fH + 50.0f;
			rc.w = fW;
			rc.h = fH;
			m_pWinOverlay[1]->Render(g_view, uv, rc, 0, color, 0.0f, 0);
		}

		m_nFlag = 1;
		m_color = ((unsigned char)(int)m_fAlpha << 24) | 0xFFDE44;
		Print(g_view->GetWidth() * 0.5f + 5.0f, rc.y + 3.0f, m_pBuffer, 3.0f, 0, 1.0f, NULL);

		if (m_pWinOverlay[2]) {
			float fW = (float)m_pWinOverlay[2]->GetWidth();
			float fH = (float)m_pWinOverlay[2]->GetHeight();
			rc.x = (fTextW + g_view->GetWidth()) * 0.5f;
			rc.w = fW;
			rc.h = fH;
			m_pWinOverlay[2]->Render(g_view, uv, rc, 0, color, 0.0f, 0);
		}
	} else {
		if (m_pPoseOverlay[1]) {
			float fW = (float)m_pPoseOverlay[1]->GetWidth();
			float fH = (float)m_pPoseOverlay[1]->GetHeight();
			rc.x = g_view->GetWidth() * 0.5f - fW * 0.5f;
			rc.y = rc.y + fH + 50.0f;
			rc.w = fW;
			rc.h = fH;
			m_pPoseOverlay[1]->Render(g_view, uv, rc, 0, color, 0.0f, 0);
		}
	}
}

// fontinfo.cpp:7475
CExtraHoleInfo::CExtraHoleInfo(void) {
	Init("[font_hole.jpg", NULL);
	m_pBuffer = new char[0x20];
	m_nFlag = 2;

	for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		m_shotTime[i] = 0;
		m_cupDistance[i] = 0;
	}

	m_pPlayerNumUI = new CSimpleUI("[nplayer.jpg");
	m_pPlayerNumUI->Add("1", WRect(0.0f, 0.0f, 32.0f, 16.0f), false);
	m_pPlayerNumUI->Add("2", WRect(32.0f, 0.0f, 32.0f, 16.0f), false);
	m_pPlayerNumUI->Add("3", WRect(0.0f, 16.0f, 32.0f, 16.0f), false);
	m_pPlayerNumUI->Add("4", WRect(32.0f, 16.0f, 32.0f, 16.0f), false);

	m_leader = 0xFF;
}

// fontinfo.cpp:7497
CExtraHoleInfo::~CExtraHoleInfo() {
	if (m_pPlayerNumUI) {
		delete m_pPlayerNumUI;
		m_pPlayerNumUI = NULL;
	}
}

// fontinfo.cpp:7502
void CExtraHoleInfo::SetShotTime(unsigned long time) {
	m_shotTime[GOLFDOC()->m_myPlayerIndex] = time;
}

// fontinfo.cpp:7507
void CExtraHoleInfo::UpdateLeader(void) {
	if (GOLFDOC()->m_approachMode) {
		WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
		WVector *pBall = &PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos;

		float dx = pBall->x - cup.x;
		float dz = pBall->z - cup.z;
		float dist = sqrtf(dx * dx + dz * dz) * 0.3125f;
		m_cupDistance[GOLFDOC()->m_myPlayerIndex] = (unsigned long)floor(dist * 100.0f);

		if (m_leader == 0xFF) {
			m_leader = GOLFDOC()->m_myPlayerIndex;
		} else {
			unsigned char me = GOLFDOC()->m_myPlayerIndex;
			if (m_cupDistance[me] < m_cupDistance[m_leader] || m_cupDistance[me] == m_cupDistance[m_leader] && m_shotTime[me] < m_shotTime[m_leader]) {
				m_leader = me;
			}
		}
	}
}

// fontinfo.cpp:7527
void CExtraHoleInfo::Display(void) {
	if (GOLFDOC()->m_approachMode) {
		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (GOLFDOC()->GetPlayer(i)->m_state != 3) {
				if (m_pPlayerNumUI) {
					m_pPlayerNumUI->SetColor(GOLFDOC()->GetPlayer(i)->m_color | 0xFF000000);
					m_pPlayerNumUI->Render(MakeStr("%d", i + 1), g_view->GetWidth() - 161.0f, i * 23.0f + 213.0f, 1.0f, 0);
				}

				if (GOLFDOC()->GetPlayer(i)->m_state == 1) {
					m_color = 0xFFFF0A0A;
				} else {
					m_color = (i != m_leader ? 0x2DFF : 0) + 0xFFFFD200;
				}

				if (GOLFDOC()->GetPlayer(i)->m_holes[GOLFDOC()->m_currentHole - 1] == 0) {
					Print(g_view->GetWidth() - 85.0f, i * 23.0f + 213.0f, "-", 3.0f, 0, 1.0f, NULL);
					Print(g_view->GetWidth() - 25.0f, i * 23.0f + 213.0f, "-", 3.0f, 0, 1.0f, NULL);
				} else {
					if (m_cupDistance[i] > 0) {
						Print(g_view->GetWidth() - 65.0f, i * 23.0f + 213.0f, MakeStr("%d.%dy", m_cupDistance[i] / 100, m_cupDistance[i] % 100), 3.0f, 0, 1.0f, NULL);
					} else {
						Print(g_view->GetWidth() - 85.0f, i * 23.0f + 213.0f, "-", 3.0f, 0, 1.0f, NULL);
					}

					if (m_shotTime[i] > 0) {
						Print(g_view->GetWidth() - 5.0f, i * 23.0f + 213.0f, MakeStr("%d.%03d", m_shotTime[i] / 1000, m_shotTime[i] % 1000), 3.0f, 0, 1.0f, NULL);
					} else {
						Print(g_view->GetWidth() - 25.0f, i * 23.0f + 213.0f, "-", 3.0f, 0, 1.0f, NULL);
					}
				}
			}
		}
	}
}

// fontinfo.cpp:7577
CFontOverlay::CFontOverlay(void) {
	m_pHoleCupInfo = NULL;
	m_pFlyInfo = NULL;
	m_pHoleInfo = NULL;
	m_pAccuracyInfo = NULL;
	m_pTimeOutInfo = NULL;

	Init();
}

// fontinfo.cpp:7590
CFontOverlay::~CFontOverlay() {
	if (m_pHoleCupInfo) {
		delete m_pHoleCupInfo;
		m_pHoleCupInfo = NULL;
	}
	if (m_pFlyInfo) {
		delete m_pFlyInfo;
		m_pFlyInfo = NULL;
	}
	if (m_pHoleInfo) {
		delete m_pHoleInfo;
		m_pHoleInfo = NULL;
	}
	if (m_pAccuracyInfo) {
		delete m_pAccuracyInfo;
		m_pAccuracyInfo = NULL;
	}
	if (m_pTimeOutInfo) {
		delete m_pTimeOutInfo;
		m_pTimeOutInfo = NULL;
	}
}

// fontinfo.cpp:7602
void CFontOverlay::Init(void) {
	m_pHoleCupInfo = new CHoleCupInfo;
	m_pFlyInfo = new CFlyInfo;
	m_pHoleInfo = new CHoleInfo;
	m_pAccuracyInfo = new CAccuracyInfo;
	m_pTimeOutInfo = new CTimeOutInfo;

	m_bShowHoleCup = false;
	m_bShowFly = false;
	m_bShowHole = true;
	m_bShowAccuracy = true;
	m_bShowTimeOut = true;
}

// fontinfo.cpp:7620
void CFontOverlay::Process(float fDelta) {
	if (m_bShowHoleCup && GOLFDOC()->m_gameState >= 4) {
		if (m_pHoleCupInfo) {
			m_pHoleCupInfo->Process(fDelta);
		}
	}

	if (m_bShowFly) {
		if (m_pFlyInfo) {
			m_pFlyInfo->Process(fDelta);
		}
	}

	if (GOLFDOC()->m_gameState == 7 || GOLFDOC()->m_gameState >= 8) {
		if (m_pHoleInfo) {
			m_pHoleInfo->Process(fDelta);
		}
	}

	if (m_bShowAccuracy) {
		if (m_pAccuracyInfo) {
			m_pAccuracyInfo->Process(fDelta);
		}
	}

	if (m_bShowTimeOut) {
		if (m_pTimeOutInfo) {
			m_pTimeOutInfo->Process(fDelta);
		}
	}
}

// fontinfo.cpp:7653
void CFontOverlay::Display(void) {
	if (m_bShowHoleCup && GOLFDOC()->m_gameState >= 4 || GOLFDOC()->m_gameState == 1) {
		if (m_pHoleCupInfo) {
			m_pHoleCupInfo->Display();
		}
	}

	if (m_bShowFly) {
		if (m_pFlyInfo) {
			m_pFlyInfo->Display();
		}
	}

	if (GOLFDOC()->m_gameState == 7 || GOLFDOC()->m_gameState >= 8) {
		if (m_pHoleInfo) {
			m_pHoleInfo->Display();
		}
	}

	if (m_bShowAccuracy) {
		if (m_pAccuracyInfo) {
			m_pAccuracyInfo->Display();
		}
	}

	if (m_bShowTimeOut) {
		if (m_pTimeOutInfo) {
			m_pTimeOutInfo->Display();
		}
	}
}

// fontinfo.cpp:7694
void CFontOverlay::ResetUiPosition(void) {
	if (m_pHoleCupInfo) {
		m_pHoleCupInfo->ResetUiPosition();
	}

	if (m_pHoleInfo) {
		m_pHoleInfo->ResetUiPosition();
	}
}

CPangInfo::sPangCase::sPangCase(void) {
}
