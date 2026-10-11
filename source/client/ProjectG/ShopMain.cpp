// Original: d:\build\custom\temp\code\client\projectg\shopmain.cpp
#include <stdio.h>

#include "shopmain.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "frscrollbar.h"
#include "UccManager.h"
#include "SharedDoc.h"
#include "frlistbox.h"
#include "Packet.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "frarea.h"
#include "frform.h"
#include "RepayDlg.h"
#include "partsinfo.h"
#include "fredit.h"
#include "frcombobox.h"
#include "frtextbutton.h"
#include "frdesktop.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"
#include "CaddieInfoDlg.h"
#include "commonutil.h"
#include "ItemManager.h"
#include "RankingDlg.h"
#include "user_info.h"
#include "s5/Utilities.h"
#include "messengerdlg.h"
#include "CheckoutDlg.h"
#include "InputManager.h"
#include "TaskManager.h"
#include "globalgamedefine.h"
#include "MascotInfo.h"
#include "UccLibrary.h"
#include "Localize.h"
#include "SoundManager.h"
#include <math.h>

// shopmain.h: no line record (empty constructor of a nested helper struct)
CShopMain::miniBtn_t::miniBtn_t(void) {
}

bool SpecialHitNewCompare(const void *lhs, const void *rhs);
bool PartCompare(const void *lhs, const void *rhs);
bool IsAngelWing(unsigned long typeId);
bool IsMannerPlayer(void);

// shopmain.cpp:62
void AddSellMiniButton(IFF_ITEM_COMMON &item, FrListItem *&listItem) {
	if ((item.m_flags0 >> 5) == 1 || (item.m_flags0 >> 5) == 3)
		listItem->noRBtnIdxList.push_back(1);
	if ((item.m_flags0 >> 5) == 1 || (item.m_flags0 >> 5) == 2)
		listItem->noRBtnIdxList.push_back(2);
}

// shopmain.cpp:170 (run-time type of CShopMain)
const WRTTI CShopMain::m_RTTI("CShopMain", &CTaskMain::m_RTTI);

IObject* CShopMainMakeInstance(void) {
	return new CShopMain;
}

struct __impCShopMain {
	__impCShopMain(void) {
		ObjectFactory().AddObjectFunctor(CShopMainMakeInstance, "CShopMain");
		ActorFactory().AddActorClass("CShopMain");
	}
};
static __impCShopMain s_impCShopMain;

// shopmain.cpp:172
const sFRESH_MSGMAP *CShopMain::GetMessageMap(void) const {
	return &_MsgMap;
}

// shopmain.cpp:477
void CShopMain::OnShopMainInit(int) {
	SetTimeVariableBGM();
	TurnOff3DBackground();
}

// shopmain.cpp:624
void CShopMain::OnSortOptionComboInit(int param) {
	FrComboBox *wnd = DYNAMIC_CAST(FrComboBox, param);
	m_sortOptionCombo = wnd;
	if (wnd) {
		wnd->SetVisible(false); wnd->Enable(false);
	}
}

// shopmain.cpp:633
void CShopMain::OnSortOptionComboDown(void) {
}

// shopmain.cpp:638
void CShopMain::OnSearchGoodsButtonInit(int param) {
	FrTextButton *wnd = DYNAMIC_CAST(FrTextButton, param);
	m_searchGoodsButton = wnd;
	if (wnd) {
		wnd->SetVisible(false); wnd->Enable(false);
	}
}

// shopmain.cpp:647
void CShopMain::OnSearchGoodsLButtonDown(void) {
}

// shopmain.cpp:699
void CShopMain::InitShopDialogs(void) {
	m_buyDlg = NULL;
	m_optionDlg = NULL;
	m_kbdManualDlg = NULL;
	m_serverDlg = NULL;
	m_giftDlg = NULL;
	m_waitDlg = NULL;
	m_notifyOneDlg = NULL;
	m_waitDlgTime = -10.0f;
}

// shopmain.cpp:761
void CShopMain::CloseAllDialogs(void) {
	FrWnd *desktop = g_pFresh->GetManager()->GetDesktop();
	FrWnd *shop = DYNAMIC_CAST(FrWnd, desktop->FindChildByName("bongdari_shop"));
	if (shop)
		shop->Close(true);
}

// shopmain.cpp:770
void CShopMain::TurnOnItemStatus(void) {
	m_statBox->SetVisible(true);
	SetLevelBar();
}

// shopmain.cpp:776
void CShopMain::TurnOffItemStatus(void) {
	m_statBox->SetVisible(false);
	SetLevelBar();
}

// shopmain.cpp:912
bool SpecialHitNewCompare(const void *lhs, const void *rhs) {
	const IFF_ITEM_COMMON *left = (const IFF_ITEM_COMMON *)lhs;
	const IFF_ITEM_COMMON *right = (const IFF_ITEM_COMMON *)rhs;

	_SYSTEMTIME *leftBegin = NULL, *leftEnd = NULL, *rightBegin = NULL, *rightEnd = NULL;
	bool leftHasPeriod = ItemManager()->HasSalePeriod(left->m_typeId, &leftBegin, &leftEnd);
	bool rightHasPeriod = ItemManager()->HasSalePeriod(right->m_typeId, &rightBegin, &rightEnd);

	if (!leftHasPeriod) {
		if (rightHasPeriod) {
			return false;
		}
	} else {
		if (!rightHasPeriod) {
			return true;
		}

		if ((leftBegin->wYear != 0 || leftEnd->wYear != 0) && rightBegin->wYear == 0 && rightEnd->wYear == 0) {
			return true;
		}
		if (leftBegin->wYear == 0 && leftEnd->wYear == 0 && (rightBegin->wYear != 0 || rightEnd->wYear != 0)) {
			return false;
		}
	}

	if ((left->m_flags1 & 1) && !(right->m_flags1 & 1)) {
		return true;
	}
	if ((right->m_flags1 & 1) && !(left->m_flags1 & 1)) {
		return false;
	}

	if (((left->m_flags1 >> 1) & 1) && !(right->m_flags1 & 2)) {
		return true;
	}
	if (!((left->m_flags1 >> 1) & 1) && (right->m_flags1 & 2)) {
		return false;
	}

	return right->m_typeId < left->m_typeId;
}

// shopmain.cpp:957
// Sort of the skin list: by type id, empty slots last.
bool SkinCompare(const void *lhs, const void *rhs) {
	const IFF_ITEM_COMMON *left = (const IFF_ITEM_COMMON *)lhs;
	const IFF_ITEM_COMMON *right = (const IFF_ITEM_COMMON *)rhs;

	if (left == NULL) {
		return false;
	}
	if (right == NULL) {
		return true;
	}

	return left->m_typeId < right->m_typeId;
}

// shopmain.cpp:974
bool ShopEmptyCompare(const void *lhs, const void *rhs) {
	if (lhs) {
		if (rhs == NULL) {
			return true;
		}
	}
	return false;
}

// shopmain.cpp:985
bool CaddieCompare(const void *lhs, const void *rhs) {
	const IFF_ITEM_COMMON *left = (const IFF_ITEM_COMMON *)lhs;
	const IFF_ITEM_COMMON *right = (const IFF_ITEM_COMMON *)rhs;

	if (left == NULL || right == NULL) {
		return false;
	}

	return left->m_typeId < right->m_typeId;
}

// shopmain.cpp:996
void CShopMain::OnPetInit(int param) {
	m_pet = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1001
void CShopMain::OnSkinPetInit(int param) {
	m_skinPet = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1006
void CShopMain::OnSkinPet2Init(int param) {
	m_skinPet2 = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1192
void CShopMain::DrawCutinFrame(void) {
	if (m_subTab == 33) {
		FrListItem *listItem = m_itemListBox->GetSelected();
		if (listItem == NULL)
			return;
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)listItem->pData;
		if (item == NULL)
			return;

		WOverlay *cutin = GetCutinImg(item->m_typeId);
		if (cutin == NULL)
			return;

		cutin->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), WRect(72.0f, 67.0f, cutin->GetWidth(), cutin->GetHeight()), 0, 0xFFFFFFFF, 0.0f, 0);
	}
}

// shopmain.cpp:1230
WOverlay *CShopMain::GetCutinImg(unsigned long typeId) {
	WOverlay *overlay;
	std::map<unsigned int, WOverlay *>::iterator it = m_cutinImgs.find(typeId);
	if (it == m_cutinImgs.end()) {

		IFF_STRUCT::sSkin *skin = ItemManager()->FindSkin(typeId);

		if (skin) {
			char path[64];
			sprintf(path, "%s.jpg", skin->m_cutinImage);
			overlay = g_resrcmng->GetOverlay(path, 0);
			if (overlay) {
				m_cutinImgs.insert(std::map<unsigned int, WOverlay *>::value_type(skin->m_typeId, overlay));
			}
		} else {
			return NULL;
		}
	} else {
		overlay = it->second;
	}

	return overlay;
}

// shopmain.cpp:1277
void CShopMain::OnPetOwnerDraw(int) {
	if (m_shopTab == 4) {
		switch (m_subTab) {
		case 13:
		case 14:
		case 15:
			DrawSkinFrame();
			break;
		case 16:
		case 34:
			DrawSkinSlot();
			break;
		case 33:
			DrawCutinFrame();
			break;
		}
	} else {
		if (!m_anotherHand.IsWorking(0)) {
			if (!m_bongdariShop[2] && !WSingleton<CRankingInfo>::Instance()->GetDlg() && !USERINFODLG()) {
				m_anotherHand.Enter();
				if (m_petExhibition)
					m_petExhibition->Display(0.0f, 0.0f);
				m_anotherHand.Leave();
			}
		}
	}
}

// shopmain.cpp:1309
void CShopMain::OnItemDescBgInit(int param) {
	m_itemDescBg = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1341
void CShopMain::OnItemDescInit(int param) {
	m_itemDesc = DYNAMIC_CAST(FrEdit, param);
	if (m_itemDesc) {
		m_itemDesc->SetVisible(false);
		m_itemDescPos = WPoint(m_itemDesc->GetRect().x, m_itemDesc->GetRect().y + m_itemDesc->GetRect().h);
	}
}

// shopmain.cpp:1351
void CShopMain::SetItemDescRect(void) {
	m_itemDesc->ClearLine();

	switch (m_subTab) {
	case SUB_TAB_CLUBSET:
	case 10:
		m_itemDescOffset = WPoint(0.0f, 30.0f);
		break;
	case SUB_TAB_AZTEC:
	case SUB_TAB_MASCOT:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		m_itemDescOffset = WPoint(0.0f, 120.0f);
		break;
	default:
		m_itemDescOffset = WPoint(0.0f, 0.0f);
		break;
	}
}

// shopmain.cpp:1377
void CShopMain::ShowItemDesc(unsigned long typeId) {
	IFF_STRUCT::sDesc *desc = ItemManager()->FindDesc(typeId);
	if (desc) {
		m_itemDesc->ClearLine();
		m_itemDesc->AddText(desc->m_text, false, true);

		int lineNum = m_itemDesc->GetLineNum();

		// Keep at most two lines.
		if (lineNum > 2) {
			int extra = lineNum - 2;
			lineNum -= extra;
			do {
				m_itemDesc->DeleteEndLine();
			} while (--extra);
		}

		float height = (float)m_itemDesc->GetLineHeight() * lineNum;
		m_itemDesc->SetRect(WRect(m_itemDescPos.x + m_itemDescOffset.x, m_itemDescPos.y + m_itemDescOffset.y - height, m_itemDesc->GetRect().w, height));

		m_itemDesc->GetScrollBar()->ScrollToFirst();
	}
}

// shopmain.cpp:1401
void CShopMain::OnPowerBarInit(intptr_t param) {
	m_statBar[0] = DYNAMIC_CAST(FrGaugeBar, param);
	m_statBar[0]->SetRange(0, 50, 0);
}

// shopmain.cpp:1407
void CShopMain::OnControlBarInit(intptr_t param) {
	m_statBar[1] = DYNAMIC_CAST(FrGaugeBar, param);
	m_statBar[1]->SetRange(0, 30, 0);
}

// shopmain.cpp:1413
void CShopMain::OnImpactBarInit(intptr_t param) {
	m_statBar[2] = DYNAMIC_CAST(FrGaugeBar, param);
	m_statBar[2]->SetRange(0, 30, 0);
}

// shopmain.cpp:1419
void CShopMain::OnSpinBarInit(intptr_t param) {
	m_statBar[3] = DYNAMIC_CAST(FrGaugeBar, param);
	m_statBar[3]->SetRange(0, 30, 0);
}

// shopmain.cpp:1425
void CShopMain::OnCurveBarInit(intptr_t param) {
	m_statBar[4] = DYNAMIC_CAST(FrGaugeBar, param);
	m_statBar[4]->SetRange(0, 30, 0);
}

// shopmain.cpp:1515
void CShopMain::OnSelFrameInit(int param) {
	m_selFrame = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1522
void CShopMain::OnStatBoxInit(int param) {
	m_statBox = DYNAMIC_CAST(FrGroupBox, param);
}

// shopmain.cpp:1529
void CShopMain::OnSelItemIconInit(int param) {
	m_selItemIcon = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1536
void CShopMain::OnSelItemIconOwnerDraw(int) {
	FrGraphicInterface *graphic = g_pFresh->GetManager()->GetGDI();
	if (graphic == NULL)
		return;

	switch (m_subTab) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 26:
		DrawSelItem(graphic, m_standPetTypeId);
		DrawCharacterList(graphic, m_standPetTypeId);
		break;
	case SUB_TAB_CLUBSET:
		DrawSelItem(graphic, m_clubsetTypeId);
		break;
	case SUB_TAB_AZTEC:
		DrawSelItem(graphic, m_aztecTypeId);
		break;
	case 10:
		DrawSelItem(graphic, m_caddieTypeId);
		DrawCaddieList(graphic, m_caddieTypeId);
		break;
	case SUB_TAB_MASCOT:
		DrawSelItem(graphic, m_mascotTypeId);
		break;
	}
}

// shopmain.cpp:1577
void CShopMain::OnSelItemInit(int param) {
	m_selItem = DYNAMIC_CAST(FrButton, param);
	if (m_selItem) {
		m_selItem->EnableBlink(true);
	}
}

// shopmain.cpp:1586
void CShopMain::OnSelItemOwnerDraw(int itemIndex) {
}

// shopmain.cpp:1591
void CShopMain::OnTabTxt1Init(int param) {
	m_tabTxt1 = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1598
void CShopMain::OnTabTxt2Init(int param) {
	m_tabTxt2 = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1605
void CShopMain::OnTabTxt3Init(int param) {
	m_tabTxt3 = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1612
void CShopMain::OnSelectedNameInit(int param) {
	m_selectedName = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:1619
void CShopMain::OnSelItemUp(void) {
	m_selItemExpanded = !m_selItemExpanded;
}

// shopmain.cpp:1629
void CShopMain::OnSelItemPrevInit(int param) {
	m_selItemPrev = DYNAMIC_CAST(FrButton, param);
	if (m_selItemPrev) {
		m_selItemPrev->SetPushDelay(0.0f);
	}
}

// shopmain.cpp:1680
void CShopMain::OnSelItemNextInit(int param) {
	m_selItemNext = DYNAMIC_CAST(FrButton, param);
	if (m_selItemNext) {
		m_selItemNext->SetPushDelay(0.0f);
	}
}

// shopmain.cpp:1737
void CShopMain::OnStatFrameInit(int param) {
	m_statFrame = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1742
void CShopMain::OnStatTextInit(int param) {
	m_statText = DYNAMIC_CAST(FrArea, param);
}

// shopmain.cpp:1747
void CShopMain::OnStatPartInit(int param) {
	m_statPart = DYNAMIC_CAST(FrViewer, param);
}

// shopmain.cpp:1754
void CShopMain::OnStatPartUp(void) {
	m_statTab = STAT_TAB_PART;
}

// shopmain.cpp:1759
void CShopMain::OnStatPartOwnerDraw(int) {
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		pGDI->DrawTexture(m_statPartBitmap[m_statPartKind * 3], m_statPart->GetRect(), 0xFFFFFFFF, 0);

		WRect rect = m_statAll->GetRect();
		pGDI->DrawTexture(m_statAllBitmap[0], rect, 0xFFFFFFFF, 0);
	}
}

// shopmain.cpp:1772
void CShopMain::OnStatAllInit(int param) {
	m_statAll = DYNAMIC_CAST(FrViewer, param);
	if (m_statAll) {
		m_statAll->SetVisible(false);
	}
}

// shopmain.cpp:1782
void CShopMain::OnStatAllUp(void) {
	m_statTab = STAT_TAB_ALL;
}

// shopmain.cpp:1787
void CShopMain::OnStatAllOwnerDraw(int) {
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		WRect rect = m_statAll->GetRect();
		int state = 0;
		if (m_statAll->IsVisible()) {
			if (m_statTab == STAT_TAB_ALL) {
				state = 2;
			} else if (rect.IsInRect(g_pFresh->GetManager()->GetMousePos())) {
				state = 1;
			}
			pGDI->DrawTexture(m_statAllBitmap[state], rect, 0xFFFFFFFF, 0);
		}
	}
}

// shopmain.cpp:1898
void CShopMain::OnTabSubInit(int param) {
	m_tabSubButton = DYNAMIC_CAST(FrTabButton, param);
}

// shopmain.cpp:1906
void CShopMain::OnTabSubButtonUp(void) {
	TButtonItem *selected = m_tabSubButton->GetSelected();
	if (selected) {
		ChangeSubTab((subTab_t)selected->index, false);
	}
}

// shopmain.cpp:1915
void CShopMain::OnItemListInit(intptr_t param) {
	m_itemListBox = DYNAMIC_CAST(FrListBox, param);
	if (m_itemListBox == 0)
		return;

	m_itemListBox->UseRightButton(true);
	m_itemListBox->UseDummy(true);
	m_itemListBox->SetPushSound(0);
	m_itemListBox->AddNoRButtonRect(m_itemListNoRButtonRect1, 1);
	m_itemListBox->AddNoRButtonRect(m_itemListNoRButtonRect2, 2);
}

// shopmain.cpp:1962
void CShopMain::DrawStatus2(FrGraphicInterface *graphic, FrListItem *listItem, short *const status, short *const slotStatus, float x) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {
		if (status[i] != 0 || slotStatus[i] != 0) {
			graphic->SetTextColor(0xFF0000FF, 0xFFFFFFFF);
			graphic->Print11(WPoint(x + 182.0f, offsetY + x + 36.0f) + listItem->pos, 2, MakeStatusString(i, status[i], slotStatus[i]));
			offsetY += 14.0f;
		}
	}
}

// shopmain.cpp:1986
void CShopMain::DrawStatusCard(FrGraphicInterface *graphic, FrListItem *listItem, short *const cardStatus, float x) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {
		if (cardStatus[i] != 0) {
			graphic->SetTextColor(0xFF0000FF, 0xFFFFFFFF);
			graphic->Print11(WPoint(x + 182.0f, offsetY + x + 36.0f) + listItem->pos, 2, MakeCardStatusString(i, cardStatus[i]));
			offsetY += 14.0f;
		}
	}
}

// shopmain.cpp:2886
void CShopMain::OnBuyAllInit(int param) {
	m_buyAll = DYNAMIC_CAST(FrButton, param);
	if (m_buyAll) {
		m_buyAll->EnableHover(true, 0.0f);
	}
}

// shopmain.cpp:2958
void CShopMain::OnBuyResetInit(int param) {
	m_buyReset = DYNAMIC_CAST(FrButton, param);
	if (m_buyReset) {
		m_buyReset->EnableHover(true, 0.0f);
	}
}

// shopmain.cpp:3059
void CShopMain::OnBuyHoverOn(int param) {
	FrButton *button = DYNAMIC_CAST(FrButton, param);
	if (button == m_buyAll) {
		CIconToolTip::GetInstance()->SetToolTip((eBarButton)0x18);
	} else if (button == m_buyReset) {
		CIconToolTip::GetInstance()->SetToolTip((eBarButton)0x19);
	}
}

// shopmain.cpp:3107
void CShopMain::OnSideTabBuyInit(int param) {
	m_sideTabBuy = DYNAMIC_CAST(FrButton, param);
	if (m_sideTabBuy) {
		m_sideTabBuy->SetPushDelay(0.2f);
		m_sideTabBuy->SetStatus(FrButton::PRESSED);
	}
}

// shopmain.cpp:3118
void CShopMain::OnSideTabBuyUp(void) {
	if (m_sideTabBuy && m_sideTabRental) {
		if (m_sideTabBuy->GetStatus() != FrButton::PRESSED) {
			m_sideTabBuy->SetStatus(FrButton::PRESSED);
			m_sideTabRental->SetStatus(FrButton::NORMAL);

			m_buySideTab = true;

			ClearCartList(true, &WSingleton<CSharedDoc>::Instance()->GetMyPartTidList());
			SetLevelBar();
			BuildDisplayStand(m_subTab);
			m_anotherHand.Work(this, 0, &m_standPetTypeId, true, -1);
		}
	}
}

// shopmain.cpp:3137
void CShopMain::OnSideTabRentalInit(int param) {
	m_sideTabRental = DYNAMIC_CAST(FrButton, param);
	if (m_sideTabRental) {
		m_sideTabRental->SetPushDelay(0.2f);
		m_sideTabRental->SetStatus(FrButton::NORMAL);
	}
}

// shopmain.cpp:3147
void CShopMain::OnSideTabRentalUp(void) {
	if (m_sideTabBuy && m_sideTabRental) {
		if (m_sideTabRental->GetStatus() != FrButton::PRESSED) {
			m_sideTabRental->SetStatus(FrButton::PRESSED);
			m_sideTabBuy->SetStatus(FrButton::NORMAL);

			m_buySideTab = false;

			ClearCartList(false, &WSingleton<CSharedDoc>::Instance()->GetMyPartTidList());
			SetLevelBar();
			BuildDisplayRental(m_subTab);
			m_anotherHand.Work(this, 0, &m_standPetTypeId, true, -1);
		}
	}
}

// shopmain.cpp:3177
void CShopMain::SetVisibleSideTab(bool bVisible) {
	if (m_sideTabBuy) m_sideTabBuy->SetVisible(bVisible);
	if (m_sideTabRental) m_sideTabRental->SetVisible(bVisible);
}

// shopmain.cpp:3182
void CShopMain::BuildDisplayRental(subTab_t tab) {
	m_itemListBox->ClearItem();

	if (tab > 0 && tab <= 4) {
		unsigned long categoryFlag = GetCategoryFlag(tab);

		for (std::map<unsigned int, IFF_STRUCT::sPart>::iterator it = ItemManager()->m_partMap.begin();
			it != ItemManager()->m_partMap.end(); ++it) {
			IFF_STRUCT::sPart &part = it->second;
			if (part.m_rentalPrice > 0
				&& S5::UnderCategory(m_standPetTypeId, part.m_typeId, categoryFlag)
				&& (part.m_flags & 0xE0) != 0
				&& ItemManager()->IsInSalePeriod(part.m_typeId, WSingleton<CSharedDoc>::Instance()->GetServerTime())) {
				if (!IsAngelWing(part.m_typeId) || (IsMannerPlayer() && WSingleton<CSharedDoc>::Instance()->m_myLevel >= 0x15)) {
					FrListItem *listItem = m_itemListBox->AddItem(&part);
					if (listItem && IsItemInCart(listItem)) {
						m_itemListBox->SelectItem(listItem, true);
					}

					unsigned char shopFlag = part.m_flags >> 5;
					if (shopFlag == 1 || shopFlag == 3) {
						listItem->AddNoRBtnIdx(1);
					}
				}
			}
		}

		m_itemListBox->SortItem(PartCompare);
	}

	int count = m_itemListBox->GetCurrentItemSize(true);
	if (count < 8) {
		for (int missing = 8 - count; missing != 0; --missing) {
			m_itemListBox->AddItem(NULL);
		}
	}
}

// shopmain.cpp:3231
void CShopMain::DrawRentalPrice(float x, float y, IFF_ITEM_COMMON &item, FrListItem *listItem) {
	if (listItem == NULL)
		return;

	if ((item.m_flags0 & 0xE0) == 0x80)
		return;

	if ((item.m_typeId & 0xFC000000) != 0x08000000)
		return;

	IFF_STRUCT::sPart *part = ItemManager()->FindPart(item.m_typeId);
	if (part == NULL)
		return;

	g_pFresh->GetManager()->GetGDI()->Print11(WPoint(x, y) + listItem->pos, 2, MakeStr("%d\xc6\xce(%d\xc0\xcf)", part->m_rentalPrice, 7));
}

// shopmain.cpp:3260
bool CShopMain::IsSelectedRentalTab(void) {
	if (m_shopTab == 0 && m_sideTabRental) {
		if (m_sideTabRental->GetStatus() == FrButton::PRESSED) {
			return true;
		}
	}
	return false;
}

// shopmain.cpp:3271
void CShopMain::OnPartCartListInit(int param) {
	m_cartList = DYNAMIC_CAST(FrListBox, param);
}

// shopmain.cpp:3471
void CShopMain::DrawStatus3(FrGraphicInterface *graphic, float x, float y, short *const status, short *const slotStatus) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {
		if (status[i] != 0 || slotStatus[i] != 0) {
			graphic->Print11(WPoint(x, offsetY + y), 0, MakeStatusString(i, status[i], slotStatus[i]));
			offsetY += 12.0f;
		}
	}
}

// shopmain.cpp:3484
void CShopMain::OnAuxPartCartListInit(int param) {
	m_auxCartList = DYNAMIC_CAST(FrListBox, param);
}

// shopmain.cpp:3575
void CShopMain::CheckAuxCartScroll(void) {
	FrScrollBar *scrollBar = m_auxCartList->GetScrollBar();
	int top = scrollBar->GetCurTopRow_Int();

	if (scrollBar->GetRowCapacity() < scrollBar->GetItemNum()) {
		m_scrollUp->SetVisible(true);
		m_scrollDown->SetVisible(true);

		if (top == 0) {
			m_scrollUp->Enable(false);
			m_scrollDown->Enable(true);
		} else {
			m_scrollUp->Enable(true);
			m_scrollDown->Enable(false);
		}

		if (m_buyAll) {
			WRect buyRect, downRect, upRect;
			m_buyAll->GetClientRect(buyRect);
			m_scrollDown->GetClientRect(downRect);
			m_scrollUp->GetClientRect(upRect);

			downRect.x = buyRect.x - downRect.w;
			upRect.x = downRect.x - upRect.w - 2.0f;
			upRect.y = (buyRect.h - downRect.h) * 0.5f + buyRect.y;
			downRect.y = upRect.y;

			m_scrollUp->SetClientRect(upRect);
			m_scrollDown->SetClientRect(downRect);
		}
	} else {
		m_scrollUp->SetVisible(false);
		m_scrollDown->SetVisible(false);
	}
}

// shopmain.cpp:3619
void CShopMain::TurnOffAuxCartScroll(void) {
	m_scrollUp->SetVisible(false);
	m_scrollDown->SetVisible(false);
}

// shopmain.cpp:3625
void CShopMain::TurnOnAuxCartScroll(void) {
	m_scrollUp->SetVisible(true);
	m_scrollDown->SetVisible(true);
}

// shopmain.cpp:3631
void CShopMain::OnScrollUpInit(int param) {
	m_scrollUp = DYNAMIC_CAST(FrButton, param);
	if (m_scrollUp) {
		m_scrollUp->SetVisible(false);
	}
}

// shopmain.cpp:3640
void CShopMain::OnScrollUpUp(void) {
	m_auxCartList->GetScrollBar()->ScrollUp(-1);
	CheckAuxCartScroll();
}

// shopmain.cpp:3646
void CShopMain::OnScrollDownInit(int param) {
	m_scrollDown = DYNAMIC_CAST(FrButton, param);
	if (m_scrollDown) {
		m_scrollDown->SetVisible(false);
	}
}

// shopmain.cpp:3655
void CShopMain::OnScrollDownUp(void) {
	m_auxCartList->GetScrollBar()->ScrollDown(-1);
	CheckAuxCartScroll();
}

// shopmain.cpp:3661
void CShopMain::OnItemSlotFrame1Init(int param) {
	m_itemSlotFrame1 = DYNAMIC_CAST(FrArea, param);
	if (m_itemSlotFrame1) {
		m_itemSlotFrame1->SetVisible(false);
	}
}

// shopmain.cpp:3670
void CShopMain::OnItemSlotFrame2Init(int param) {
	m_itemSlotFrame2 = DYNAMIC_CAST(FrArea, param);
	if (m_itemSlotFrame2) {
		m_itemSlotFrame2->SetVisible(false);
	}
}

// shopmain.cpp:3802
void CShopMain::OnItemSlotInit(intptr_t param) {
	m_itemSlot = DYNAMIC_CAST(FrListBox, param);
	if (m_itemSlot) {
		m_itemSlot->SetVisible(false);
		m_itemSlot->SetMouseEvent(false);
	}
}

// shopmain.cpp:3863
void CShopMain::OnPowerEditInit(int param) {
	m_statEdit[0] = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:3868
void CShopMain::OnControlEditInit(int param) {
	m_statEdit[1] = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:3873
void CShopMain::OnImpactEditInit(int param) {
	m_statEdit[2] = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:3878
void CShopMain::OnSpinEditInit(int param) {
	m_statEdit[3] = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:3883
void CShopMain::OnCurveEditInit(int param) {
	m_statEdit[4] = DYNAMIC_CAST(FrStatic, param);
}

// shopmain.cpp:3917
bool CShopMain::OnGiftDlgResult(int result, FrForm *form) {
	m_giftDlg = NULL;
	return true;
}

// shopmain.cpp:3931
bool CShopMain::OnSellDlgResult(int result, FrForm *form) {
	if (result == 1) {
		FrRepayDlg *dlg = DYNAMIC_CAST(FrRepayDlg, form);
		if (dlg) {
			MsgObject msg(NULL, 1, 0, 0, 0, 0, 0);
			if (this) {
				HandleMsg(msg);
			}

			int kind;
			switch (dlg->GetTid() >> 26) {
			case 1:
				kind = 0;
				break;
			case 7:
				kind = 1;
				break;
			default:
				kind = 2;
				break;
			}

			WSendPacket packet((enumClientPacket)0x1E);
			packet.Encode1(kind);
			packet.Encode1(1);
			packet.Encode4(dlg->GetTid());
			packet.Encode4(dlg->GetGuid());
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	return true;
}

// shopmain.cpp:3961
bool CShopMain::OnReemployeeResult(int result, FrForm *form) {
	if (result == 1) {
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		for (std::map<unsigned int, sCaddieInfo>::iterator i = doc->m_caddieMap.begin(); i != doc->m_caddieMap.end(); ++i) {
		}
	}

	return true;
}

// shopmain.cpp:3987
CShopMain::CShopMain(void) {
	if (IsLocalContent((localContentType_t)0x51)) {
		m_cardImgState = 0;
		LoadCardImages();
	}

	m_cutinImgs.clear();
	InitalizeUpgradeCaddieInfo();
	BuildVisibleCaddieList();
}

// shopmain.cpp:4007
CShopMain::~CShopMain(void) {
	if (IsLocalContent((localContentType_t)0x51)) {
		DeleteCardImages();
	}

	DeleteCutinImages();

	HidePet(true);

	g_resrcmng->UnlockAllByThread(m_anotherHand.GetThreadID());
	m_anotherHand.BreakWorkingThread();
	HandleMsg(MsgObject(NULL, 2, 0, 0, 0, 0, 0));
}

// shopmain.cpp:4027
void CShopMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();

	InitShopControls();
	InitShopDialogs();

	m_selItemExpanded = false;
	m_caddieTypeId = 0x1C000000;
	m_standPetTypeId = 0;
	m_mascotTypeId = 0x40000000;
	m_clubsetTypeId = 0x0C000000;
	m_aztecTypeId = 0x14000000;
	m_charLevels = NULL;
	m_charParts = NULL;
	m_charCards = NULL;
	m_hairColor = 0;
	m_rbuttonPartTypeIds.clear();
	m_statTab = STAT_TAB_PART;
	m_shopTab = 0;
	m_subTab = (subTab_t)1;

	m_sortOptionCombo = NULL;
	m_searchGoodsButton = NULL;
}

// shopmain.cpp:4070
void CShopMain::OnLoad(void) {
	CTaskMain::OnLoad();
	LoadShopImages();
}

// shopmain.cpp:4077
void CShopMain::OnInit(void) {
	CTaskMain::OnInit();
}

// shopmain.cpp:4085
void CShopMain::OnDestroy(void) {
	CTaskMain::OnDestroy();
}

// shopmain.cpp:4091
void CShopMain::OnProcess(float dt) {
	CTaskMain::OnProcess(dt);

	if (m_anotherHand.IsWorking(0)) {
		Sleep(10);
	} else {
		m_anotherHand.Enter();

		bool flag;
		if (m_shopTab == 1 || m_shopTab == 4) {
			flag = false;
		} else {
			flag = true;
		}

		if (m_petExhibition) {
			m_petExhibition->Process(dt, flag);
		}

		m_anotherHand.Leave();
	}

	if (m_hintTime > 0.0f) {
		m_hintTime -= dt;

		if (m_hintTime < 0.1f) {
			m_hintTime = -10.0f;
			if (m_hintWnd) {
				m_hintWnd->SetVisible(false);
			}
		}
	}

	if (m_buyDlg) {
		((FrCheckoutDlg *)m_buyDlg)->Process();
	}

	if (m_shopTab == 4 || m_shopTab == 5) {
		for (int i = 0; i < 2; i++) {
			float speed = (i == 0) ? 138.0f : 228.0f;

			switch (m_sideDir[i]) {
			case 0:
				m_sidePos[i] = 0.0f;
				break;
			case 1:
				m_sidePos[i] = m_sidePos[i] - speed * dt * 0.25f;
				break;
			case 2:
				m_sidePos[i] = m_sidePos[i] + speed * dt * 0.25f;
				break;
			}
		}
	}

	if (g_input->GetDown("ESCAPE", true) == 1 && !g_pFresh->GetManager()->HasEscKeyWindow()) {
		CMessengerInfo::Instance()->Close(false);
	}

	if (!m_rbuttonPartTypeIds.empty()) {
		WSingleton<CSharedDoc>::Instance()->GetMyPartTidList();

		for (std::list<unsigned long>::iterator i = m_rbuttonPartTypeIds.begin(); i != m_rbuttonPartTypeIds.end(); ++i) {
			UnselectItemList(*i);
		}
		m_rbuttonPartTypeIds.clear();

		if (m_shopTab == 0 || m_shopTab == 4) {
			m_buyAll->Enable(!m_cartList->m_itemList.empty() || !m_auxCartList->m_itemList.empty());
			m_buyReset->Enable(!m_cartList->m_itemList.empty() || !m_auxCartList->m_itemList.empty());
		}
	}

	int tip = CIconToolTip::GetInstance()->GetCurTipIndex();
	if (tip == 0x16 || tip == 0x17) {
		OnBarHoverOff(0);
	}

	m_blinkTime += dt;
	m_blinkValue = (float)fabs(sin(m_blinkTime * 4.0f)) * 5.0f;
}

// shopmain.cpp:4199
void CShopMain::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0x6A: {
		if (m_shopTab != msg.m_param1)
			break;

		std::map<unsigned int, sCharacterInfo>::const_iterator it = WSingleton<CSharedDoc>::Instance()->m_characterMap.begin();
		for (; it != WSingleton<CSharedDoc>::Instance()->m_characterMap.end(); ++it) {
			if (m_standPetTypeId == it->second.typeId) {
				if (WSingleton<CSharedDoc>::Instance()->m_myCharacterId != it->second.id) {
					WSendPacket packet((enumClientPacket)0x20);
					packet.Encode1(5);
					packet.Encode4(it->second.id);
					packet.Send(SEND_TO_DEFAULT);
				}
				break;
			}
		}

		if (it == WSingleton<CSharedDoc>::Instance()->m_characterMap.end())
			break;

		IFF_STRUCT::sChar *character = ItemManager()->FindChar(m_standPetTypeId);
		if (!character)
			break;

		sCharacterInfo info = it->second;
		bool changed = false;

		for (int i = 0; i < character->m_numParts + character->m_numExtraParts; i++) {
			if (info.partTypeIds[i] == m_charParts[i])
				continue;

			IFF_STRUCT::sPart *part = ItemManager()->FindPart(m_charParts[i]);
			if (part) {
				unsigned char level = WSingleton<CSharedDoc>::Instance()->m_myLevel;
				unsigned char above = part->m_level >> 7;
				if (above == 0 && (part->m_level & 0x7F) > level)
					continue;
				if (above == 1 && (part->m_level & 0x7F) < level)
					continue;
			}

			if (IsUccClothes(m_charParts[i]))
				continue;

			info.partTypeIds[i] = m_charParts[i];
			info.partIds[i] = m_charPartIds[i];
			changed = true;
		}

		for (int i = character->m_numParts + character->m_numExtraParts; i < 24; i++)
			info.partTypeIds[i] = 0;

		for (int i = 0; i < 5; i++) {
			if (info.auxParts.tids[i] == m_charCards[i])
				continue;

			IFF_STRUCT::sAuxPart *auxPart = ItemManager()->FindAuxPart(m_charCards[i]);
			if (auxPart && (auxPart->m_level & 0x7F) > WSingleton<CSharedDoc>::Instance()->m_myLevel)
				continue;

			info.auxParts.tids[i] = m_charCards[i];
			changed = true;
		}

		if (info.hairColor != m_hairColor) {
			info.hairColor = m_hairColor;
		} else if (!changed) {
			break;
		}

		WSendPacket packet((enumClientPacket)0x20);
		packet.Encode1(0);
		packet.EncodeBuffer(&info, sizeof(info));
		packet.Send(SEND_TO_DEFAULT);
		break;
	}

	case 0xC4:
		if (msg.m_param1 < 6) {
			m_shopTab = msg.m_param1;
			if (msg.m_param2 < 0x24)
				m_subTab = (subTab_t)msg.m_param2;
			if (m_shopTab == 0 && m_subTab > 5)
				m_subTab = SUB_TAB_NONE;
		} else if (msg.m_param1 == 6) {
			m_shopTab = 5;
			m_subTab = (subTab_t)0x20;
		} else {
			m_shopTab = 0;
			m_subTab = SUB_TAB_NONE;
		}
		break;

	case 0xC3:
		*(int *)msg.m_param1 = m_shopTab;
		*(subTab_t *)msg.m_param2 = m_subTab;
		break;

	case 0:
		OpenLayout((const char *)msg.m_param1);
		break;

	case 0xC6:
		if (msg.m_param1 == 3) {
			m_shopTab = 0;
			m_subTab = (subTab_t)0x1A;
			ChangeMainTab((mainTab_t)0, true);
			ChangeSubTab((subTab_t)0x1A, true);

			unsigned long typeId = msg.m_param2;
			if (!typeId)
				typeId = m_standPetTypeId;

			ChangeCharacter(typeId);
		} else if (msg.m_param1 == 4) {
			if (IsLocalContent((localContentType_t)0x51)) {
				m_shopTab = 1;
				m_subTab = (subTab_t)0x1B;
				ChangeMainTab((mainTab_t)1, true);
				ChangeSubTab((subTab_t)0x1B, true);
				HidePet(true);
			}
		}
		break;

	case 0x49: {
		CTask *task = WSingleton<CTaskManager>::Instance()->GetPreservedTask();
		const sCharacterInfo *character = (const sCharacterInfo *)msg.m_param2;
		if (task && task->GetActor("AvatarChat")) {
			task->GetActor("AvatarChat") << MsgObject(0, 0x49, msg.m_param1, msg.m_param2, 0, 0, 0);
		} else {
			WSingleton<CSharedDoc>::Instance()->m_myCharacterId = character->id;
		}
		ChangeCharacter(character->typeId);
		break;
	}

	case 0x4A: {
		const sCaddieInfo *caddie = (const sCaddieInfo *)msg.m_param2;
		if (caddie->typeId)
			ChangeCaddie(caddie->typeId);
		WSingleton<CSharedDoc>::Instance()->m_myCaddieId = caddie->id;
		break;
	}

	case 0x4B: {
		const sItemInfo *clubSet = (const sItemInfo *)msg.m_param2;
		ChangeClubset(clubSet->m_typeId);
		WSingleton<CSharedDoc>::Instance()->m_myClubId = clubSet->m_serial;
		break;
	}

	case 0x4C:
		ChangeAztec(msg.m_param2);
		WSingleton<CSharedDoc>::Instance()->m_myBallId = msg.m_param2;
		break;

	case 0x4E: {
		const sMascotInfo *mascot = (const sMascotInfo *)msg.m_param2;
		if (mascot->typeId)
			ChangeMascot(mascot->typeId);
		WSingleton<CSharedDoc>::Instance()->m_myMascotId = mascot->id;
		break;
	}

	case 0xB5: {
		unsigned long id = msg.m_param1;
		unsigned long typeId = msg.m_param2;
		sCharacterInfo info;
		memset(&info, 0, sizeof(info));
		info.id = id;
		info.typeId = typeId;
		ItemManager()->GetDefCombo(typeId, info.partTypeIds);

		WSingleton<CSharedDoc>::Instance()->m_characterMap.insert(std::map<unsigned int, sCharacterInfo>::value_type(info.id, info));
		WSingleton<CSharedDoc>::Instance()->m_myCharacterId = info.id;
		break;
	}

	case 0xB6: {
		sItemInfo item;
		memset(&item, 0, sizeof(item));
		item.m_serial = msg.m_param1;
		item.m_typeId = msg.m_param2;
		if (msg.m_param4 == 6 || msg.m_param4 == 2) {
			item.m_reserved08 = msg.m_param3 * 24;
			item.m_stateBits.kind = msg.m_param4;
		}

		int slot = (item.m_typeId >> 13) & 0x1F;
		m_charPartIds[slot] = item.m_serial;

		if (IsUccClothes(item.m_typeId)) {
			strncpy(item.m_uccKey, ((const sBoughtItem *)msg.m_param5)->uccKey, 9);
			item.m_uccSeq = 1;
			WSingleton<CSharedDoc>::Instance()->m_myItemList.push_back(item);
		} else {
			WSingleton<CSharedDoc>::Instance()->AddMyPartsList(item);
		}
		break;
	}

	case 0xB7: {
		unsigned long id = msg.m_param1;
		unsigned long typeId = msg.m_param2;
		sCaddieInfo caddie;
		memset(&caddie, 0, sizeof(caddie));
		caddie.id = id;
		caddie.typeId = typeId;
		caddie.flags |= 2;
		caddie.days = 30;

		if (caddie.typeId >= 0x1C00000C && caddie.typeId <= 0x1C00000E) {
			caddie.level = (unsigned char)msg.m_param4;
			caddie.experience = msg.m_param3;
		}

		WSingleton<CSharedDoc>::Instance()->m_caddieMap.insert(std::map<unsigned int, sCaddieInfo>::value_type(caddie.id, caddie));

		m_caddieTypeId = caddie.typeId;
		BuildVisibleCaddieList();
		break;
	}

	case 0xB8: {
		unsigned int caddieId = msg.m_param1;
		unsigned long cadItemTypeId = msg.m_param2;
		unsigned long hours = msg.m_param3;
		std::map<unsigned int, sCaddieInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.find(caddieId);
		if (it == WSingleton<CSharedDoc>::Instance()->m_caddieMap.end())
			break;

		if (!ItemManager()->FindCadItem(cadItemTypeId))
			break;

		unsigned char days = (unsigned short)hours / 24;
		it->second.cadItemTypeId = cadItemTypeId;
		it->second.hours = (unsigned short)hours;
		if (it->second.days < days)
			it->second.days = days;
		break;
	}

	case 0xBA: {
		sItemInfo item;
		memset(&item, 0, sizeof(item));
		item.m_serial = msg.m_param1;
		item.m_typeId = msg.m_param2;

		WSingleton<CSharedDoc>::Instance()->m_myItemMap.insert(std::map<unsigned int, sItemInfo>::value_type(item.m_serial, item));
		break;
	}

	case 0xBB: {
		unsigned long id = msg.m_param1;
		const sBoughtItem *record = (const sBoughtItem *)msg.m_param5;
		unsigned long typeId = msg.m_param2;

		IFF_STRUCT::sBall *ball = ItemManager()->FindBall(typeId);
		if (!ball)
			break;

		std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myGearList.begin();
		for (; it != WSingleton<CSharedDoc>::Instance()->m_myGearList.end(); ++it) {
			if (it->m_typeId == typeId) {
				if (record->count > 0)
					it->m_count = record->count;
				else
					it->m_count += ball->quantity;
				break;
			}
		}

		if (it != WSingleton<CSharedDoc>::Instance()->m_myGearList.end())
			break;

		sItemInfo item;
		memset(&item, 0, sizeof(item));
		item.m_serial = id;
		item.m_typeId = typeId;
		item.m_count = record->count > 0 ? record->count : ball->quantity;

		WSingleton<CSharedDoc>::Instance()->m_myGearList.push_back(item);
		break;
	}

	case 0xBC: {
		sItemInfo item;
		memset(&item, 0, sizeof(item));
		item.m_serial = msg.m_param1;
		item.m_typeId = msg.m_param2;
		item.m_stateBits.kind = msg.m_param4;
		item.m_reserved08 = msg.m_param3 * 24;

		WSingleton<CSharedDoc>::Instance()->m_mySkinList.push_back(item);

		std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_mySkinList.begin();
		for (; it != WSingleton<CSharedDoc>::Instance()->m_mySkinList.end(); ++it) {
		}
		break;
	}

	case 0xB0:
		break;

	case 3:
		if (!m_hintWnd)
			break;

		if (!m_hintWnd->IsVisible()) {
			g_audio->PlaySfx("notice_dingdong", 0, 0, 0, 0, 0.5f, 200.0f);
			m_hintWnd->SetVisible(true);
		}

		m_hintTime = 7.0f;
		m_hintWnd->AddLine((const char *)msg.m_param1, WSingleton<CChatMsg>::Instance()->ConvertToUIColor(msg.m_param2), false);
		break;

	case 0xB9: {
		sItemInfo item;
		memset(&item, 0, sizeof(item));
		item.m_serial = msg.m_param1;
		item.m_typeId = msg.m_param2;
		int period = msg.m_param3;
		item.m_stateBits.kind = msg.m_param4;

		item.m_reserved08 = period;
		item.m_count = ((const sBoughtItem *)msg.m_param5)->count;

		ItemManager()->FindItem(item.m_typeId);

		bool found = false;
		if (item.m_stateBits.kind) {
			switch (item.m_stateBits.kind) {
			case 3:
				break;
			case 4:
				period *= 24;
				item.m_reserved08 = period;
				break;
			case 5:
				period *= 720;
				item.m_reserved08 = period;
				break;
			}
		}

		if (!item.m_stateBits.kind) {
			for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
				if (it->m_typeId == item.m_typeId) {
					it->m_reserved08 += period;
					it->m_count = item.m_count;
					found = true;
				}
			}
		}

		if (item.m_stateBits.kind && period > 0)
			Net_StartPeriodItem(item.m_serial);

		if (!found)
			WSingleton<CSharedDoc>::Instance()->m_itemInfoList.push_back(item);
	}
	// fall through: 0xC5 is the tail of 0xB9
	case 0xC5:
		SetMyItemSlot();
		break;

	// TODO(boot): 0xBD (7c444c: FindHairStyle(param2); the m_characterMap entry whose typeId & 0x3FFFFFF equals the style's byte 0x91 gets hairColor = its byte 0x90; needs names inside sHairStyle's UNKNOWN in classdefine.h)
	// TODO(boot): 0xBE (7c44c5), 0xBF (7c45f9), 0xC0 (7c4680): not transcribed
	// TODO(boot): 0xC7 (7c4430: the CTaskMain member at 0x154, its vtable slot 0x64 (1, 1)): not transcribed
	// TODO(boot): 0x1FE: not transcribed
	// TODO(boot): 0x1FF, 0x217, 0x218, 0x24F: not transcribed (0x218 at 5119)
	// TODO(boot): 0x241: FindChildForm("notify") on the desktop, then its vtable slot 0x70 (4398-4405)
	default:
		HandleMsgCommon(msg);
		break;
	}
}

// shopmain.cpp:5261
void CShopMain::OnAnotherHand(int nWork, void *pData) {
	if (nWork == 0) {
		ShowPet(*(unsigned long *)pData);
	}
}

// shopmain.cpp:5268
void CShopMain::OpenLayout(const char *layoutName) {
	WSendPacket packet((enumClientPacket)0x5C);
	packet.Send(SEND_TO_DEFAULT);

	WSendPacket packet2((enumClientPacket)0x3D);
	packet2.Send(SEND_TO_DEFAULT);

	g_pFresh->OpenLayout(layoutName, this, false);
}

// shopmain.cpp:5419
void CShopMain::ShowPet(unsigned long typeId) {
	if (typeId) {
		m_anotherHand.Enter();

		if (m_petArea == NULL) {
			m_anotherHand.Leave();
			return;
		}

		if (m_petExhibition) { delete m_petExhibition; m_petExhibition = NULL; }

		SetPetCtrl(typeId);

		m_anotherHand.Leave();
	}
}

// shopmain.cpp:5439
// 98%: cl puts this in edi and the exhibition in esi (the original swaps
// them); a temporary, a do-while(0) guard and `== true` did not move it.
void CShopMain::HidePet(bool bWait) {
	if (bWait) {
		m_anotherHand.WaitUntilWorkDone(9999);
	}

	if (m_petExhibition) { delete m_petExhibition; m_petExhibition = NULL; }
}

// shopmain.cpp:5685
bool CShopMain::HaveCharacter(unsigned long typeId) {
	const std::map<unsigned int, sCharacterInfo> &characters = WSingleton<CSharedDoc>::Instance()->m_characterMap;
	for (std::map<unsigned int, sCharacterInfo>::const_iterator i = characters.begin(); i != characters.end(); ++i) {
		if (typeId == i->second.typeId) {
			return true;
		}
	}

	return false;
}

// shopmain.cpp:6075
void CShopMain::Net_StartPeriodItem(unsigned long typeId) {
	WSendPacket packet((enumClientPacket)0x20);
	packet.Encode1(7);
	packet.Encode4(typeId);
	packet.Send(SEND_TO_DEFAULT);
}

// shopmain.cpp:6083
void CShopMain::SetLevelBar(void) {
	for (int i = 0; i < 5; i++) {
		if (m_statBar[i]) {
			m_statBar[i]->SetDestPos(0, false);
			m_statBar[i]->SetExpand(0);
		}
	}

	switch (m_subTab) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 26:
		SetLevelBar_Char();
		break;
	case SUB_TAB_CLUBSET:
		SetLevelBar_Clubset();
		break;
	case 10: {
		IFF_STRUCT::sCaddie *caddie = ItemManager()->FindCaddie(m_caddieTypeId);
		if (caddie) {
			for (int i = 0; i < 5; i++) {
				unsigned char value = (unsigned char)caddie->m_levels[i];

				if (m_statBar[i]) {
					m_statBar[i]->SetDestPos(value, false);
				}

				if (m_statEdit[i]) {
					m_statEdit[i]->SetCaption11(MakeStatusString2(value, 0, 0));
				}
			}
		}

		m_statPartKind = 2;
		break;
	}
	default:
		break;
	}
}

// shopmain.cpp:6393
void CShopMain::OpenCaddieInfo(const sCaddieInfo &info) {
	FrWnd *wnd = g_pFresh->GetManager()->GetDesktop()->FindChildByName("caddie_info");
	if (wnd == NULL) {
		FrCaddieInfoDlg *dlg = CreateForm<FrCaddieInfoDlg>(g_pFresh->GetManager(), this, "caddie_info", DYNAMIC_CAST(FrWnd, 0));
		if (dlg) {
			dlg->SetCaddieInfo(info);
			dlg->Open((FRESH_RESULT_CALLBACK)&CShopMain::OnReemployeeResult, 3);
		}
	}
}

// shopmain.cpp:6407
void CShopMain::ResetItemList(bool bKeepType1) {
	if (m_itemListBox) {
		for (FrListBox::ITEM_LIST::iterator i = m_itemListBox->m_itemList.begin(); i != m_itemListBox->m_itemList.end(); ++i) {
			FrListItem *listItem = *i;
			if (listItem->selected) {
				if (bKeepType1) {
					if ((((IFF_ITEM_COMMON *)listItem->pData)->m_typeId & 0xFC000000) == 0x04000000) {
						continue;
					}
				}

				m_itemListBox->UnselectItem(listItem);
			}
		}
	}
}

// shopmain.cpp:6827
void CShopMain::Repay(const IFF_ITEM_COMMON &item, unsigned long guid) {
	FrRepayDlg *dlg = CreateForm<FrRepayDlg>(g_pFresh->GetManager(), this, "repay", NULL);
	if (dlg) {
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		dlg->Repay(item, guid, doc->m_myPang, doc->m_myCash);
		dlg->Open((FRESH_RESULT_CALLBACK)&CShopMain::OnSellDlgResult, 0x81);
	}
}

// shopmain.cpp:6931
FrListBox::ITEM_LIST::iterator CShopMain::DeleteItemFromCartList(unsigned long typeId) {
	FrListBox::ITEM_LIST::iterator end = m_cartList->m_itemList.end();
	for (FrListBox::ITEM_LIST::iterator i = m_cartList->m_itemList.begin(); i != end; ++i) {
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)(*i)->pData;
		if (item->m_typeId == typeId) {
			return m_cartList->_DelItem(i);
		}
	}

	return end;
}

// shopmain.cpp:6944
FrListBox::ITEM_LIST::iterator CShopMain::DeleteItemFromAuxCartList(unsigned long typeId) {
	FrListBox::ITEM_LIST::iterator end = m_auxCartList->m_itemList.end();
	for (FrListBox::ITEM_LIST::iterator i = m_auxCartList->m_itemList.begin(); i != end; ++i) {
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)(*i)->pData;
		if (item->m_typeId == typeId) {
			FrListBox::ITEM_LIST::iterator next = m_auxCartList->_DelItem(i);
			CheckAuxCartScroll();
			return next;
		}
	}

	return end;
}

// shopmain.cpp:6959
bool CShopMain::SelectItemList(unsigned long typeId) {
	if (!m_itemListBox) {
		return false;
	}

	FrListItem *found = NULL;
	for (FrListBox::ITEM_LIST::iterator i = m_itemListBox->m_itemList.begin(); i != m_itemListBox->m_itemList.end(); ++i) {
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)(*i)->pData;
		if (item) {
			if (item->m_typeId == typeId) {
				found = *i;
				break;
			}
		}
	}

	if (!found) {
		return false;
	}

	SelectItemList(found);
	return true;
}

// shopmain.cpp:7628
bool CShopMain::UnselectItemFromItemList(unsigned long typeId) {
	bool unselected = false;
	if (!m_itemListBox) {
		return unselected;
	}

	for (FrListBox::ITEM_LIST::iterator i = m_itemListBox->m_itemList.begin(); i != m_itemListBox->m_itemList.end(); ++i) {
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)(*i)->pData;
		if (item) {
			if (item->m_typeId == typeId) {
				m_itemListBox->UnselectItem(*i);
				unselected = true;
			}
		}
	}

	return unselected;
}

// shopmain.cpp:8515
void CShopMain::TakeOff_AuxPartNCartList(CPartTidList &tidList, unsigned long overlapMask) {
	for (FrListBox::ITEM_LIST::iterator i = m_auxCartList->m_itemList.begin(); i != m_auxCartList->m_itemList.end();) {
		IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)(*i)->pData;

		if ((item->m_typeId & 0xFC000000) == 0x08000000) {

			unsigned long overlapped = tidList.GetOverlapped(item->m_typeId);
			if (overlapMask & overlapped) {

				UnselectItemFromItemList(item->m_typeId);
				i = DeleteItemFromAuxCartList(item->m_typeId);

				if (tidList.IsEquipped(item->m_typeId, 0)) {

					m_petExhibition->GetPetFrame()->SetPart(&tidList, item->m_typeId, 0);
				}

				RestoreShoppersOldItem(item->m_typeId, tidList);

				continue;
			}
		}

		++i;
	}
}

// shopmain.cpp:8548
void CShopMain::SetLevelBar_Char(void) {
	IFF_STRUCT::sChar *character = ItemManager()->FindChar(m_standPetTypeId);
	if (character) {

		for (int i = 0; i < 5; i++) {

			unsigned char itemLevel = ItemManager()->GetItemLevel((eLvlType)i, m_charParts, m_charCards);
			unsigned char maxValue = ItemManager()->GetCharCapacity((eLvlType)i, m_standPetTypeId, m_charParts, m_charCards, WSingleton<CSharedDoc>::Instance()->m_myLevel) + itemLevel;
			unsigned char value = ItemManager()->GetCharLevel((eLvlType)i, m_standPetTypeId, m_charLevels) + itemLevel;

			if (m_statBar[i]) {

				m_statBar[i]->SetDestPos(value, false);
				m_statBar[i]->SetExpand(maxValue - value);
			}

			if (m_statEdit[i]) {

				m_statEdit[i]->SetCaption11(MakeStatusString2(value, maxValue, 0));
			}
		}
	}

	m_statPartKind = 0;
}

// shopmain.cpp:8577
void CShopMain::SetLevelBar_Clubset(void) {
	IFF_STRUCT::sClubSet *clubSet = ItemManager()->FindClubSet(m_clubsetTypeId);
	if (clubSet) {

		for (int i = 0; i < 5; i++) {

			unsigned char value = (unsigned char)clubSet->m_baseStat[i], maxValue = (unsigned char)clubSet->m_maxStat[i];

			if (m_statBar[i]) {

				m_statBar[i]->SetDestPos(value, false);
				m_statBar[i]->SetExpand(maxValue - value);
			}

			if (m_statEdit[i]) {

				m_statEdit[i]->SetCaption11(MakeStatusString2(value, maxValue, 0));
			}
		}
	}

	m_statPartKind = 1;
}

// shopmain.cpp:9262
void CShopMain::Build_Mascot(subTab_t tab, int) {
	typedef std::map<unsigned int, IFF_STRUCT::sMascot> MascotMap;
	// Stand-in for ItemManager()->m_mascotMap (CItemManager 0xF0), private in
	// ItemManager.h for now; replace once it moves under the public maps.
#define TEMP_MASCOTS (*(MascotMap *)((char *)ItemManager() + 0xF0))
	for (MascotMap::iterator it = TEMP_MASCOTS.begin(); it != TEMP_MASCOTS.end(); ++it) {
		IFF_ITEM_COMMON &item = (IFF_ITEM_COMMON &)it->second;
		unsigned char shopFlag = item.m_flags0 >> 5;
		if (shopFlag != 0 && shopFlag != 5 && shopFlag != 6
			&& ItemManager()->IsInSalePeriod(item.m_typeId, WSingleton<CSharedDoc>::Instance()->GetServerTime())) {
			FrListItem *listItem = m_itemListBox->AddItem(&item);
			AddSellMiniButton(item, listItem);
		}
	}

#undef TEMP_MASCOTS
	m_itemListBox->SortItem(ShopEmptyCompare);
	m_itemListBox->SortItem(SpecialHitNewCompare);
}

// shopmain.cpp:9279
bool CShopMain::IsItemInCart(FrListItem *listItem) {
	IFF_ITEM_COMMON *item = (IFF_ITEM_COMMON *)listItem->pData;

	for (FrListBox::ITEM_LIST::iterator i = m_cartList->m_itemList.begin(); i != m_cartList->m_itemList.end(); ++i) {
		IFF_ITEM_COMMON *cartItem = (IFF_ITEM_COMMON *)(*i)->pData;
		if (cartItem == NULL)
			continue;

		if (item->m_typeId == cartItem->m_typeId)
			return true;
	}

	for (FrListBox::ITEM_LIST::iterator i = m_auxCartList->m_itemList.begin(); i != m_auxCartList->m_itemList.end(); ++i) {
		IFF_ITEM_COMMON *cartItem = (IFF_ITEM_COMMON *)(*i)->pData;
		if (cartItem == NULL)
			continue;

		if (item->m_typeId == cartItem->m_typeId)
			return true;
	}

	return false;
}

// shopmain.cpp:9339
void CShopMain::RestoreShoppersOldItem(unsigned long typeId, CPartTidList &tidList) {
	for (std::list<sShoppersOldItem>::iterator it = m_shoppersOldItems.begin(); it != m_shoppersOldItems.end();) {
		if (it->typeId == typeId) {
			m_petExhibition->GetPetFrame()->SetPart(&tidList, it->tid, it->id);
			it = m_shoppersOldItems.erase(it);
		} else {
			++it;
		}
	}
}

// shopmain.cpp:9515
void CShopMain::SetPetAreaRect(void) {
	switch (m_subTab) {
	case 13:
	case 14:
	case 15:
		m_petArea = m_skinPet;
		break;
	case 16:
	case 33:
		m_petArea = m_skinPet2;
		break;
	default:
		m_petArea = m_pet;
		break;
	}
}

// shopmain.cpp:9538
void CShopMain::SetMyItemSlot(void) {
	switch (m_subTab) {
	case 9:
		BuildMyItemSlot(true);
		m_itemSlotFrame1->SetVisible(true);
		m_itemSlotFrame2->SetVisible(true);
		m_itemSlot->SetVisible(true);
		break;
	case 8:
		BuildMyItemSlot(false);
		m_itemSlotFrame1->SetVisible(true);
		m_itemSlotFrame2->SetVisible(true);
		m_itemSlot->SetVisible(true);
		break;
	default:
		m_itemSlotFrame1->SetVisible(false);
		m_itemSlotFrame2->SetVisible(false);
		m_itemSlot->SetVisible(false);
		break;
	}
}

// shopmain.cpp:9562
void CShopMain::SetAutoScroll(void) {
	FrListBox *listBox = m_itemListBox;
	FrListItem *selected = listBox->GetSelected();

	int index = 0;
	if (selected) {
		FrListBox::ITEM_LIST::iterator end = listBox->m_itemList.end();
		for (FrListBox::ITEM_LIST::iterator i = listBox->m_itemList.begin(); i != end; ++i) {
			if (*i == selected) break;
			index++;
		}
	}

	listBox->ResetTopRow(); m_itemListBox->GetScrollBar()->SetCurTopRow(index / 2);
}

// shopmain.cpp:9758
void CShopMain::ChangeStatTab(statTab_t tab) {
	m_statTab = tab;
}

// shopmain.cpp:9932
void CShopMain::ChangeMascot(unsigned long typeId) {
	m_mascotTypeId = typeId;

	if (m_subTab == SUB_TAB_MASCOT) {
		BuildDisplayStand(SUB_TAB_MASCOT);
		SafeLoadPet(&m_mascotTypeId, false);
		TurnOffItemStatus();
	}
}

// shopmain.cpp:9998
void CShopMain::ChangeClubset(unsigned long typeId) {
	m_clubsetTypeId = typeId;

	if (m_subTab == SUB_TAB_CLUBSET) {
		BuildDisplayStand(SUB_TAB_CLUBSET);
		SafeLoadPet(&m_clubsetTypeId, false);
		TurnOffItemStatus();
	}
}

// shopmain.cpp:10009
void CShopMain::ChangeAztec(unsigned long typeId) {
	m_aztecTypeId = typeId;

	if (m_subTab == SUB_TAB_AZTEC) {
		BuildDisplayStand(SUB_TAB_AZTEC);
		SafeLoadPet(&m_aztecTypeId, false);
		TurnOffItemStatus();
	}
}

// shopmain.cpp:10020
void CShopMain::TurnOnItemSelect(void) {
	m_selItem->SetVisible(true);
	m_selItemPrev->SetVisible(true);
	m_selItemNext->SetVisible(true);
}

// shopmain.cpp:10027
void CShopMain::TurnOffItemSelect(void) {
	m_selItem->SetVisible(false);
	m_selItemPrev->SetVisible(false);
	m_selItemNext->SetVisible(false);
}

// shopmain.cpp:10034
const Bitmap *CShopMain::GetItemIcon(unsigned long typeId) {
	const Bitmap *bitmap = NULL;

	std::map<unsigned long, const Bitmap *>::iterator i = m_itemIcons.find(typeId);
	if (i == m_itemIcons.end()) {
		IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(typeId);
		if (item) {
			bitmap = g_pFresh->RegisterBitmap(item->m_icon);
			if (bitmap) {
				m_itemIcons.insert(std::map<unsigned long, const Bitmap *>::value_type(item->m_typeId, bitmap));
			}
		}
	} else {
		bitmap = i->second;
	}

	return bitmap;
}

// shopmain.cpp:10529
void CShopMain::DrawSelArrow(FrGraphicInterface *graphic) {
}

// shopmain.cpp:10534
bool CShopMain::IsVisibleInStock(IFF_ITEM_COMMON &item) {
	unsigned char shopFlag = item.m_flags0 >> 5;
	if (shopFlag != 0 && shopFlag != 5 && shopFlag != 6
		&& ItemManager()->IsInSalePeriod(item.m_typeId, WSingleton<CSharedDoc>::Instance()->GetServerTime())) {
		return true;
	}
	return false;
}

// shopmain.cpp:10542
void CShopMain::SafeLoadPet(unsigned long *pTypeId, bool bForce) {
	m_anotherHand.WaitUntilWorkDone(9999);

	if (!bForce) {
		if (m_petExhibition) {
			if (m_petExhibition->GetTypeId() == *pTypeId) {
				return;
			}
		}
	}

	m_anotherHand.Work(this, 0, pTypeId, true, -1);
}

// shopmain.cpp:10562
void CShopMain::ShowMainModel(CPartTidList &tidList) {
	SetLevelBar();

	m_anotherHand.Enter();
	if (m_petExhibition && !m_anotherHand.IsWorking(0)) {
		m_petExhibition->ResetCameraRecoverTime();
		m_petExhibition->ParseBoundingBoxScript();
		m_petExhibition->GetPetFrame()->Refresh(&tidList);
	}
	m_anotherHand.Leave();
}

// shopmain.cpp:10616
WOverlay *CShopMain::GetCardImg(unsigned long typeId) {
	std::map<unsigned int, WOverlay *>::iterator it = m_cardImgs.find(typeId);
	if (it == m_cardImgs.end()) {

		IFF_STRUCT::sCard *card = ItemManager()->FindCard(typeId);

		if (card == 0) {
			return 0;
		}

		char path[64];
		sprintf(path, "%s.png", card->m_model);
		WOverlay *overlay = g_resrcmng->GetOverlay(path, 0);
		if (overlay) {
			m_cardImgs.insert(std::map<unsigned int, WOverlay *>::value_type(card->m_typeId, overlay));
		}

		return overlay;
	}

	return it->second;
}

// shopmain.cpp:10753
void CShopMain::InitalizeUpgradeCaddieInfo(void) {
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000001, 0x1C000010));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000002, 0x1C000011));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000003, 0x1C000012));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000004, 0x1C000013));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000005, 0x1C000014));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000006, 0x1C000015));
	m_upgradeCaddies.insert(std::map<unsigned long, unsigned long>::value_type(0x1C000007, 0x1C000016));
}
