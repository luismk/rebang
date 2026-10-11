// Original: d:\build\custom\temp\code\client\projectg\s5\supplypack.cpp
#include "SupplyPack.h"
#include "ObjectFactory.h"
#include "exhibition.h"
#include "InputManager.h"
#include "../client/Wangreal/include/wmath.h"
#include "TitleFontEx.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "../shared/ItemManager.h"
#include "commonutil.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "bitmap.h"
#include "frgraphicinterface.h"
#include "Packet.h"
#include "actor.h"
#include "TaskManager.h"
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>

// Animated text drawn with a font (local to this unit; size 0x18).
class CFontAnimation {
public:
	struct FONTFXPIECE {
		std::string text;   // 0x0
		float fadeTime;     // 0x1c, seconds until the text has faded out
		float alpha;        // 0x20
		WVector2D pos;      // 0x24
		WVector2D dir;      // 0x2c
		float speed;        // 0x34
	};

	CFontAnimation(WTitleFontEx* pFont, const WVector2D* pStartPoint);

	void SetStartPoint(const WVector2D& point);
	void AddFontFx(const char* text, const WVector2D& dir, float speed, float fadeTime);
	void Process(float dt);
	void Display(void);

	WTitleFontEx* m_pFont;                 // 0x0
	std::list<FONTFXPIECE> m_pieces;     // 0x4
	WVector2D m_startPoint;              // 0x10
};

// supplypack.cpp:32
CFontAnimation::CFontAnimation(WTitleFontEx* pFont, const WVector2D* pStartPoint) : m_pFont(pFont) {
	if (pStartPoint) {
		m_startPoint = *pStartPoint;
	} else {
		m_startPoint = WVector2D(0.0f, 0.0f);
	}
}

// supplypack.cpp:40
void CFontAnimation::SetStartPoint(const WVector2D& point) {
	m_startPoint = point;
}

// supplypack.cpp:49
void CFontAnimation::AddFontFx(const char* text, const WVector2D& dir, float speed, float fadeTime) {
	FONTFXPIECE piece;

	piece.fadeTime = fadeTime;
	piece.alpha = 255.0f;
	piece.dir = dir;
	piece.pos = m_startPoint;
	piece.speed = speed;
	piece.text = text;

	m_pieces.push_back(piece);
}

// supplypack.cpp:64
void CFontAnimation::Process(float dt) {
	std::list<FONTFXPIECE>::iterator it = m_pieces.begin();
	while (it != m_pieces.end()) {
		if (it->fadeTime > 0.0f) {
			it->alpha -= dt * (1.0f / it->fadeTime) * 255.0f;
			it->pos += it->dir * it->speed * dt;
		} else {
			it->alpha = 0.0f;
		}

		if (it->alpha <= 0.0f) {
			it = m_pieces.erase(it);
		} else {
			++it;
		}
	}
}

// supplypack.cpp:90
void CFontAnimation::Display(void) {
	std::list<FONTFXPIECE>::iterator it = m_pieces.begin();
	while (it != m_pieces.end()) {
		m_pFont->SetColor(((unsigned long)(unsigned char)it->alpha << 24) | 0xFFFFFF);
		m_pFont->Print(it->pos.x, it->pos.y, it->text.c_str(), TITLEFONT_ALIGN_CENTER, 1.0f, 1.0f);
		++it;
	}
}

namespace {

// supplypack.cpp:111
int GetSupplyGroup(unsigned long typeId) {
	switch (typeId) {
	case 0x1A000105:
	case 0x1A00014B:
		return 5;
	}

	return -1;
}

// supplypack.cpp:128
bool IsSuppliableItem(unsigned long typeId) {
	unsigned long excluded[] = { 0x14000004 };
	for (int i = 0; i < 1; ++i) {
		if (excluded[i] == typeId) {
			return false;
		}
	}

	if (!ItemManager()->IsCanOverlapped(typeId, 0, 0)) {
		return false;
	}

	if ((typeId & 0xFC000000) == 0x14000000) {
		if (ItemManager()->IsBasicAztecFamily(typeId)) {
			return false;
		}
		if (ItemManager()->FindBall(typeId) == NULL) {
			return false;
		}
	}

	return true;
}

} // namespace

namespace S5 {

// supplypack.cpp:159 (run-time type and factory registration of FrSupplyPackDlg)
const WRTTI FrSupplyPackDlg::m_RTTI("FrSupplyPackDlg", &FrForm::m_RTTI);

IObject* FrSupplyPackDlgMakeInstance(void) {
	return new FrSupplyPackDlg;
}

struct __sFrSupplyPackDlg {
	__sFrSupplyPackDlg(void) {
		ObjectFactory().AddObjectFunctor(FrSupplyPackDlgMakeInstance, "FrSupplyPackDlg");
	}
};
static __sFrSupplyPackDlg s_registerFrSupplyPackDlg;

// supplypack.cpp:162
const sFRESH_MSGMAP* FrSupplyPackDlg::GetMessageMap(void) const {
	return &FrSupplyPackDlg::_MsgMap;
}

sFRESH_MSGMAP FrSupplyPackDlg::_MsgMap = { &FrForm::_MsgMap, &FrSupplyPackDlg::_MsgEntries[0] };

sFRESH_ENTRY FrSupplyPackDlg::_MsgEntries[] = {
	// supplypack.cpp:164
	{ "lt_item", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSupplyPackDlg::OnItemListInit },
	// supplypack.cpp:165
	{ "bt_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSupplyPackDlg::OnCancelBtnInit },
	// supplypack.cpp:166
	{ "bt_ok", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSupplyPackDlg::OnOkayBtnInit },
	// supplypack.cpp:167
	{ "bt_cancel", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSupplyPackDlg::OnCancelBtnUp },
	// supplypack.cpp:168
	{ "bt_ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSupplyPackDlg::OnOkayBtnUp },
	// supplypack.cpp:169
	{ "bg_area", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrSupplyPackDlg::OnBgAreaDraw },
	// supplypack.cpp:170
	{ "lt_item", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSupplyPackDlg::OnItemListDraw },
	// supplypack.cpp:171
	{ "lt_item", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSupplyPackDlg::OnItemListUp },

	// supplypack.cpp:173
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// supplypack.cpp:189
FrSupplyPackDlg::FrSupplyPackDlg(void) : m_typeId(0), m_pItemList(NULL), m_pCancelBtn(NULL), m_pExhibition(NULL), m_exhibitionAlpha(0), m_pSelectedItem(NULL), m_pWindFont(NULL), m_slotDelay(5.0f), m_animating(0), m_supplyCount(0), m_slotTimer(1.0f), m_arrowOffset(0), m_heldCount(0), m_displayCount(0), m_slotIndex(0) {
	m_pWindFont = new CWindFont;
	m_pBlueFont = new CBlueWindFontBig;
	m_pFontAnimation = new CFontAnimation(m_pBlueFont, NULL);
	memset(m_slotCounts, 0, sizeof(m_slotCounts));
	g_input->BlockKey("ESCAPE");
}

// supplypack.cpp:200
FrSupplyPackDlg::~FrSupplyPackDlg(void) {
	g_input->FreeKey("ESCAPE");
	if (m_pFontAnimation) {
		delete m_pFontAnimation;
		m_pFontAnimation = NULL;
	}
	if (m_pBlueFont) {
		delete m_pBlueFont;
		m_pBlueFont = NULL;
	}
	if (m_pWindFont) {
		delete m_pWindFont;
		m_pWindFont = NULL;
	}
	if (m_pExhibition) {
		delete m_pExhibition;
		m_pExhibition = NULL;
	}
}

// supplypack.cpp:210
void FrSupplyPackDlg::Initialize(unsigned long typeId) {
	m_typeId = typeId;
	m_pItemList->m_nFlags.Enable(1);

	// the animated text starts at the dialog's centre
	WVector2D center(m_rect.x + m_rect.w * 0.5f, m_rect.y + m_rect.h * 0.5f);

	m_pFontAnimation->SetStartPoint(center);

	switch (m_typeId) {
	case 0x1A000105:
	case 0x1A00014B:
		FrForm::SetDesc("\xbc\xb1\xc5\xc3\xb5\xc8 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xbc\xf6\xb7\xae\xc0\xbb \xba\xb8\xc3\xe6 \xc7\xd5\xb4\xcf\xb4\xd9.");
		FrForm::SetCaption("\xbe\xc6\xc1\xee\xc5\xd8 \xbc\xf6\xb7\xae \xba\xb8\xc3\xe6 \xc6\xd1");

		{
			std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myGearList.begin();
			while (it != WSingleton<CSharedDoc>::Instance()->m_myGearList.end()) {
				if (IsSuppliableItem((*it).m_typeId) && (*it).m_count > 0) {
					m_itemList.push_back(*it);
					m_pItemList->AddItem(&m_itemList.back());
				}
				++it;
			}
		}

		while (m_pItemList->GetCurrentItemSize(true) % 3 != 0) {
			m_pItemList->AddItem(NULL);
		}
		break;

	default:
		m_typeId = 0;
		break;
	}
}

// supplypack.cpp:255
void FrSupplyPackDlg::UpdateData(unsigned int supplyCount) {
	if (m_animating == 0 && supplyCount >= 10) {
		m_slotTimer = 1.0f;
		m_animating = 1;
		m_supplyCount = supplyCount;

		for (int i = 0; i < 10; ++i) {
			++m_slotCounts[i];
		}

		unsigned int rest = supplyCount - 10;
		while (rest) {
			int slot = rand() % 10;
			++m_slotCounts[slot];
			--rest;
		}
	} else {
		FrForm::Close(FrCANCEL, true);

		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), 0, "notify", 0);
		// "A problem occurred in the update animation."
		pForm->SetMessage("\xBE\xF7\xB5\xA5\xC0\xCC\xC6\xAE \xBF\xA1\xB4\xCF\xB8\xDE\xC0\xCC\xBC\xC7\xBF\xA1 \xB9\xAE\xC1\xA6\xB0\xA1 \xB9\xDF\xBB\xFD\xC7\xCF\xBF\xB4\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		pForm->Open(0, 3);
	}
}

// supplypack.cpp:290
void FrSupplyPackDlg::OnProc(const float dt) {
	static bool bArrowUp = true;

	if (m_typeId == 0) {
		FrForm::Close(FrCANCEL, true);
		return;
	}

	if (m_pSelectedItem && m_pExhibition) {
		if (m_exhibitionAlpha < 255.0f) {
			m_exhibitionAlpha += dt * 120.0f;
			if (m_exhibitionAlpha > 255.0f) {
				m_exhibitionAlpha = 255.0f;
			}
		}

		m_pExhibition->Process(dt, false);
	}

	if (m_animating && m_exhibitionAlpha >= 250.0f) {
		if (bArrowUp) {
			if (m_arrowOffset < 3.0f) {
				m_arrowOffset += dt * 12.0f;
			}
			if (m_arrowOffset >= 3.0f) {
				bArrowUp = false;
			}
		} else {
			if (m_arrowOffset > -3.0f) {
				m_arrowOffset -= dt * 12.0f;
			}
			if (m_arrowOffset <= -3.0f) {
				bArrowUp = true;
			}
		}

		if (m_slotDelay > 0.0f || m_slotIndex < 10) {
			m_slotTimer += dt;
			m_slotDelay -= dt;
			if (m_slotTimer > 0.5f) {
				m_slotTimer = 0.0f;
				int count = m_slotCounts[m_slotIndex < 0 ? 0 : (m_slotIndex > 9 ? 9 : m_slotIndex)];

				int spread = rand() % 201;

				WVector2D dir((spread - 100) * 0.01f, -1.0f);
				dir.Normalize();
				m_pFontAnimation->AddFontFx(MakeStr("+%d", count), dir, 80.0f, 1.5f);

				m_displayCount += count;
				++m_slotIndex;
			}
		} else if (m_slotIndex == 10) {
			m_pOkayBtn->Enable(true);
			++m_slotIndex;
		}

		m_pFontAnimation->Process(dt);
	}
}

// supplypack.cpp:368
bool FrSupplyPackDlg::OnResultConfirm(int result, FrForm* pForm) {
	m_pItemList->EnableToolTip(true);
	if (result == 1) {

		sItemInfo* pInfo = (sItemInfo*)m_pSelectedItem->pData;
		if (pInfo == NULL) {
			return true;
		}

		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pInfo->m_typeId);
		if (pCommon == NULL) {
			return true;
		}

		m_pItemList->EnableToolTip(false);

		WRect rect = m_pItemList->GetRect();
		rect.x -= 7.0f;

		m_pExhibition = new CExhibition(rect.x + rect.w * 0.5f, rect.y + rect.h * 0.33333334f, 0.0f, false);
		m_pExhibition->SetArea(rect.w, rect.h);
		m_pExhibition->SetDistance(1.2f, true, false);
		m_pExhibition->SetModel(pInfo->m_typeId);
		m_pExhibition->SetEnableRotate(false);
		m_pExhibition->SetEnableZoom(false);
		m_pExhibition->SetAlpha((unsigned char)m_exhibitionAlpha);

		m_pOkayBtn->SetVisible(true);
		m_pOkayBtn->Enable(false);

		m_pCancelBtn->Enable(false);
		m_pCancelBtn->SetVisible(false);

		m_slotDelay = 5.0f;


		m_displayCount = m_heldCount = pInfo->m_count;

		WSendPacket packet((enumClientPacket)0xEE);
		packet.Encode4(m_typeId);
		packet.Encode4(pInfo->m_typeId);
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->PassMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
		return true;
	}

	m_pSelectedItem = NULL;
	return true;
}

// supplypack.cpp:419
void FrSupplyPackDlg::OnItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
}

// supplypack.cpp:424
void FrSupplyPackDlg::OnOkayBtnInit(intptr_t param) {
	m_pOkayBtn = DYNAMIC_CAST(FrButton, param);

	m_pOkayBtn->SetVisible(false);
}

// supplypack.cpp:430
void FrSupplyPackDlg::OnCancelBtnInit(intptr_t param) {
	m_pCancelBtn = DYNAMIC_CAST(FrButton, param);
}

// supplypack.cpp:435
void FrSupplyPackDlg::OnCancelBtnUp(void) {
	FrForm::Close(FrCANCEL, true);
}

// supplypack.cpp:439
void FrSupplyPackDlg::OnOkayBtnUp(void) {
	FrForm::Close(FrOK, true);
}

// supplypack.cpp:443
void FrSupplyPackDlg::OnBgAreaDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (m_pExhibition) {
		const Bitmap* pBmp = g_pFresh->GetBitmap("supplypack_bg_2");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += 9.0f; rect.y += 53.0f;

			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
		}

		pBmp = g_pFresh->GetBitmap("supplypack_tex_2");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += (rect.w - pBmp->Width()) * 0.5f;
			rect.y += 28.0f;
			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
		}

		m_pExhibition->SetAlpha((unsigned char)m_exhibitionAlpha);
		m_pExhibition->Display(0.0f, 0.0f);

		pBmp = g_pFresh->GetBitmap("supplypack_num");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += (rect.w - pBmp->Width()) * 0.5f;
			rect.y += 192.0f;
			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
		}

		// the count held before the supply, then the count shown so far
		{
			m_pWindFont->SetColor(((unsigned long)(unsigned char)m_exhibitionAlpha << 24) | 0xFFFFFF);
			float x = m_rect.x;
			float y = m_rect.y + 212.0f;
			m_pWindFont->Print(x + 71.0f, y, MakeStr("%d", m_heldCount), TITLEFONT_ALIGN_CENTER, 1.0f, 1.0f);

			m_pWindFont->SetColor(((unsigned long)(unsigned char)m_exhibitionAlpha << 24) | 0xFF80FF);
			m_pWindFont->Print(x + 177.0f, y, MakeStr("%d", m_displayCount), TITLEFONT_ALIGN_CENTER, 1.0f, 1.0f);
		}

		pBmp = g_pFresh->GetBitmap("supplypack_arrow");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += (rect.w - pBmp->Width()) * 0.5f
				+ m_arrowOffset;
			rect.y += 205.0f;
			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, ((unsigned long)(unsigned char)m_exhibitionAlpha << 24) | 0xFFFFFF, 0);
		}
	} else {
		const Bitmap* pBmp = g_pFresh->GetBitmap("supplypack_bg_1");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += 13.0f; rect.y += 53.0f;

			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
		}

		pBmp = g_pFresh->GetBitmap("supplypack_tex_1");
		if (pBmp) {
			WRect rect = m_rect;
			rect.x += (rect.w - pBmp->Width()) * 0.5f;
			rect.y += 28.0f;
			rect.w = pBmp->Width();
			rect.h = pBmp->Height();
			pGDI->DrawTexture(pBmp, rect, 0xFFFFFFFF, 0);
		}
	}

	if (m_animating) {
		m_pFontAnimation->Display();
	}
}

// supplypack.cpp:548
void FrSupplyPackDlg::OnItemListDraw(int param) {
	if (m_pItemList->IsVisible()) {
		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		FrListItem* pItem = (FrListItem*)param;
		if (pItem) {
			// the list fades out while the exhibited item fades in
			float alpha = 255.0f - m_exhibitionAlpha;
			if (WAbsBySign(alpha) < 0.00001f) {
				m_pItemList->SetVisible(false);
				return;
			}

			sItemInfo* pInfo = (sItemInfo*)pItem->pData;
			if (!pInfo) {
				if (pItem == m_pItemList->GetItemUnderCursor() && m_pItemList->IsEnableToolTip()) {
					m_pItemList->SetToolTipText("");
				}
				return;
			}

			IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pInfo->m_typeId);
			if (pCommon) {
				const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);
				if (pIcon) {
					float offsetX = (m_pItemList->GetItemWidth() - pIcon->Width()) * 0.5f;
					float offsetY = (m_pItemList->GetItemHeight() - pIcon->Height()) * 0.5f;
					WRect rect(pItem->pos.x + offsetX, pItem->pos.y + offsetY, pIcon->Width(), pIcon->Height());

					unsigned long color = ((unsigned long)(unsigned char)alpha << 24) | 0xFFFFFF;
					pGDI->DrawTexture(pIcon, rect, color, 0);

					m_pWindFont->SetColor(color);

					m_pWindFont->Print(m_pItemList->GetItemWidth() + pItem->pos.x, m_pItemList->GetItemHeight() + pItem->pos.y - m_pWindFont->GetFontH(),
						MakeStr("%d", pInfo->m_count), TITLEFONT_ALIGN_RIGHT, 1.0f, 1.0f);

					if (m_pSelectedItem == NULL) {
						if (pItem != m_pItemList->GetItemUnderCursor()) {
							return;
						}
					} else {
						if (pItem != m_pSelectedItem) {
							return;
						}
					}

					const Bitmap* pCheck = g_pFresh->GetBitmap("supplypack_btn_ch_o");

					pGDI->DrawTexture(pCheck, WRect(pItem->pos.x, pItem->pos.y, m_pItemList->GetItemWidth(), m_pItemList->GetItemHeight()), color, 0);

					if (m_pItemList->IsEnableToolTip()) {
						m_pItemList->SetToolTipText(MakeStr("%s", pCommon->m_name));
					}
				}
			}
		}
	}
}

// supplypack.cpp:616
void FrSupplyPackDlg::OnItemListUp(void) {
	FrListItem* pItem = m_pItemList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}

	sItemInfo* pInfo = (sItemInfo*)pItem->pData;
	if (pInfo == NULL) {
		return;
	}

	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pInfo->m_typeId);
	if (pCommon == NULL) {
		return;
	}

	if (m_pExhibition == NULL && m_pSelectedItem == NULL) {
		m_pSelectedItem = pItem;
		m_pItemList->EnableToolTip(false);

		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		// "Do you want to replenish the quantity of the selected item '%s'?"
		pForm->SetMessage(MakeStr("\xBC\xB1\xC5\xC3\xC7\xCF\xBD\xC5 \xBE\xC6\xC0\xCC\xC5\xDB '%s'\xC0\xC7 \xBC\xF6\xB7\xAE\xC0\xBB \xBA\xB8\xC3\xE6 \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", ItemManager()->GetItemName(pInfo->m_typeId)), false);
		pForm->Open((FRESH_RESULT_CALLBACK)&FrSupplyPackDlg::OnResultConfirm, 1);
	}
}

} // namespace S5
