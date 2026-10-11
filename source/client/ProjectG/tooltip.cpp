// Original: d:\build\custom\temp\code\client\projectg\tooltip.cpp
#include "ToolTip.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "InputManager.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "../client/Wangreal/include/wview.h"

// Tool tip bitmap of each bar button.
struct sToolTip {
	int button;              // 0x0, eBarButton
	const char* bitmapName;  // 0x4
};

CHECK_SIZE(sToolTip, 0x8);
CHECK_FIELD(sToolTip, button, 0x0);
CHECK_FIELD(sToolTip, bitmapName, 0x4);

sToolTip ToolTips[] = {
	{ 0, "tooltip_back" },
	{ 1, "tooltip_front" },
	{ 2, "tooltip_shop" },
	{ 3, "tooltip_myroom" },
	{ 4, "tooltip_dol" },
	{ 5, "rmr_rm_postbox" },
	{ 6, "tooltip_cookie" },
	{ 7, "tooltip_game" },
	{ 8, "tooltip_server" },
	{ 9, "tooltip_exit" },
	{ 10, "tooltip_special_card" },
	{ 11, "tooltip_msn" },
	{ 12, "tooltip_myinfo" },
	{ 13, "tooltip_option" },
	{ 14, "tooltip_magicbox" },
	{ 15, "tooltip_bong" },
	{ 16, "tooltip_ranking" },
	{ 17, "tooltip_guild" },
	{ 18, "tooltip_notice" },
	{ 19, "rmr_tooltip_market" },
	{ 20, "tooltip_scratch" },
	{ 21, "tooltip_play" },
	{ 22, "tooltip_buy" },
	{ 23, "tooltip_gift" },
	{ 24, "tooltip_buyall" },
	{ 25, "tooltip_buyreset" },
	{ 26, "tooltip_equip" },
	{ 27, "tooltip_movegift" },
	{ 28, "tooltip_delete" },
	{ 29, "tooltip_reset" },
	{ 30, "tooltip_reemploy" },
	{ 31, "tooltip_reply" },
	{ 32, "tooltip_handsel" },
	{ 33, "tooltip_opengift" },
	{ 34, "tooltip_report" },
	{ 35, "tooltip_upgrade" },
	{ 36, "tooltip_downgrade" },
	{ 37, "tooltip_equipcancel" },
	{ 38, "tooltip_equipreset" },
	{ 39, "tooltip_card" },
	{ 40, "tooltip_character" },
	{ 41, "tooltip_caddie" },
	{ 42, "tooltip_club" },
	{ 43, "tooltip_aztec" },
	{ 44, "tooltip_mascot" },
	{ 45, "ucc_tooltip_draw" },
	{ 46, "ucc_tooltip_copy" },
	{ 47, "ucc_tooltip_drag" },
	{ 48, "ucc_tooltip_grid" },
	{ 49, "ucc_tooltip_paint" },
	{ 50, "ucc_tooltip_redo" },
	{ 51, "ucc_tooltip_undo" },
	{ 52, "ucc_tooltip_reset" },
	{ 53, "ucc_tooltip_zoomin" },
	{ 54, "ucc_tooltip_zoomout" },
	{ 55, "ucc_tooltip_spoid" },
	{ 56, "ucc_tooltip_tab_draw" },
	{ 57, "ucc_tooltip_tab_pattern" },
	{ 58, "ucc_tooltip_tab_emoticon" },
	{ 59, "ucc_tooltip_tab_text" },
	{ 60, "ucc_tooltip_redraw" },
	{ 61, "ucc_tooltip_tempsave" },
	{ 62, "rmr_rm_deco" },
	{ 63, "rmr_rm_letter" },
	{ 64, "rmr_delete" },
	{ 65, "rmr_rm_withdrawal" },
	{ 66, "rmr_tooltip_cancel" },
	{ 67, "rmr_tooltip_extend" },
	{ 68, "rmr_item_upload" },
	{ 69, "rmr_tooltip_return" },
	{ 70, "tooltip_use" },
	{ 71, "tooltip_package" }
};

// tooltip.cpp:126
CIconToolTip::CIconToolTip(void) {
	m_curTipIndex = -1;
	m_curTipBitmap = 0;

	for (int i = 0; i < (int)(sizeof(ToolTips) / sizeof(ToolTips[0])); ++i)
	{
		m_tipBitmaps[i] = g_pFresh->GetBitmap(ToolTips[i].bitmapName);
	}
}

// tooltip.cpp:138
CIconToolTip::~CIconToolTip(void) {
	for (int i = 0; i < sizeof(m_tipBitmaps) / sizeof(m_tipBitmaps[0]); ++i)
	{
		m_tipBitmaps[i] = 0;
	}
}

// tooltip.cpp:146
void CIconToolTip::Render(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (m_curTipBitmap == 0 || pGDI == 0)
		return;

	if (GOLFDOC() && GOLFDOC()->GameMode() == 0x200) {
		ClearToolTip();
		return;
	}

	float x = g_input->GetMousePoint().x;
	float y = g_input->GetMousePoint().y;
	float width = (float)m_curTipBitmap->Width();
	float height = (float)m_curTipBitmap->Height();

	WRect rect(x + 12.0f, y + 23.0f, width, height);

	if (rect.x >= g_view->GetWidth() - width)
		rect.x = g_view->GetWidth() - width;

	if (rect.y >= g_view->GetHeight() - height)
		rect.y = g_view->GetHeight() - height;

	pGDI->DrawTexture(m_curTipBitmap, rect, 0xFFFFFFFF, 0);
}

// tooltip.cpp:174
void CIconToolTip::SetToolTip(eBarButton button) {
	if (m_tipBitmaps[button] == 0) {
		m_tipBitmaps[button] = g_pFresh->GetBitmap(ToolTips[button].bitmapName);
	}

	m_curTipBitmap = m_tipBitmaps[button];
	m_curTipIndex = button;
}

// tooltip.cpp:187
void CIconToolTip::ClearToolTip(void) {
	m_curTipBitmap = 0;
	m_curTipIndex = -1;
}

// tooltip.cpp:193
bool CIconToolTip::IsCurToolTip(eBarButton button) {
	if (m_curTipBitmap == m_tipBitmaps[button])
		return true;

	return false;
}
