// Original: d:\build\custom\temp\code\client\projectg\cardremove.cpp
#include "CardRemove.h"
#include "ObjectFactory.h"
#include "bitmap.h"
#include "frarea.h"
#include "frbutton.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frlistbox.h"
#include "frwndmanager.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "RealMyRoomMain.h"
#include "TaskManager.h"
#include "UccManager.h"
#include "commonutil.h"
#include "UccLibrary.h"

// cardremove.cpp:32
void DrawPartsSlot(FrGraphicInterface* pGDI, sItemInfo* pItem, FrListItem* pListItem) {
	if (pItem == NULL)
		return;


	if (pListItem == NULL)
		return;

	if (pGDI == NULL) {

		pGDI = g_pFresh->GetManager()->GetGDI();
		if (pGDI == NULL)
			return;
	}

	float charX = 220.0f;
	float caddieX = 220.0f;
	if (!((unsigned short)pItem->m_charCardSlotCount > -1 && (unsigned short)pItem->m_charCardSlotCount < 5)) {

		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->m_typeId);
		if (pPart) {

			pItem->m_charCardSlotCount = pPart->m_charCardSlots;
			if (!((unsigned short)pItem->m_charCardSlotCount > -1 && (unsigned short)pItem->m_charCardSlotCount < 5)) {
				pItem->m_charCardSlotCount = 0;
			}
		} else {
			pItem->m_charCardSlotCount = 0;
		}
	}

	for (int i = 1; i < (unsigned short)pItem->m_charCardSlotCount + 1; i++) {
		bool found = false;
		for (std::list<sSCardAvilityPeriodInfo>::iterator it = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); it != CSharedDoc::Instance()->m_cardPeriodList[0].end(); ++it) {

			if (pItem->m_typeId == it->partsTypeId && pItem->m_serial == it->partsId) {

				IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(it->typeId);
				if (pCard) {

					if (i == it->slot) {

						if (it->kind == 0) {


							found = true;
							const Bitmap* pBitmap = g_pFresh->GetBitmap(pCard->m_texture);
							if (pBitmap) {

								pGDI->DrawTexture(pBitmap, WRect(charX + pListItem->pos.x, pListItem->pos.y + 3.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
								charX += 14.0f;
							}
						}
					}
				}
			}
		}

		if (!found) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap("card_character_empty");

			if (pBitmap) {

				pGDI->DrawTexture(pBitmap, WRect(charX + pListItem->pos.x, pListItem->pos.y + 3.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
				charX += 14.0f;
			}
		}
	}

	if (!((unsigned short)pItem->m_caddieCardSlotCount > -1 && (unsigned short)pItem->m_caddieCardSlotCount < 5)) {

		IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->m_typeId);
		if (pPart) {

			pItem->m_caddieCardSlotCount = pPart->m_caddieCardSlots;
			if (!((unsigned short)pItem->m_caddieCardSlotCount > -1 && (unsigned short)pItem->m_caddieCardSlotCount < 5)) {
				pItem->m_caddieCardSlotCount = 0;
			}
		} else {
			pItem->m_caddieCardSlotCount = 0;
		}
	}

	for (int i = 1; i < (unsigned short)pItem->m_caddieCardSlotCount + 1; i++) {
		bool found = false;
		for (std::list<sSCardAvilityPeriodInfo>::iterator it = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); it != CSharedDoc::Instance()->m_cardPeriodList[0].end(); ++it) {

			if (pItem->m_typeId == it->partsTypeId && pItem->m_serial == it->partsId) {

				IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(it->typeId);
				if (pCard) {

					if (i == it->slot) {

						if (it->kind == 1) {


							found = true;
							const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr(pCard->m_texture));
							if (pBitmap) {

								pGDI->DrawTexture(pBitmap, WRect(caddieX + pListItem->pos.x, pListItem->pos.y + 15.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
								caddieX += 14.0f;
							}
						}
					}
				}
			}
		}

		if (!found) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap("card_caddie_empty");

			if (pBitmap) {

				pGDI->DrawTexture(pBitmap, WRect(caddieX + pListItem->pos.x, pListItem->pos.y + 15.0f, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);
				caddieX += 14.0f;
			}
		}
	}
}

// Type ids of the card remover items (no symbol: data at 0xA8DF28).
extern "C" unsigned long data_a8df28[4];

// cardremove.cpp:169
bool FrCardRemove::IsCardRemoveItem(unsigned long itemTypeId) {
	bool found = false;
	for (int i = 0; i < 4; ++i) {
		if (data_a8df28[i] == itemTypeId) {
			found = true;
			break;
		}
	}
	return found;
}

// cardremove.cpp:186
FrCardRemove::FrCardRemove(void) {
	m_pDrawArea = 0;
	m_pItemList = 0;

	m_cardRemoveItemTypeId = 0;
	m_cardRemoveItemId = 0;

	m_curPartTypeId = 0;
	m_curPartUid = 0;

	m_curPartCardList.clear();
}

// cardremove.cpp:201
FrCardRemove::~FrCardRemove(void) {
}

// cardremove.cpp:206
void FrCardRemove::DrawInDlg(const char* bitmap, float x, float y) {
	if (bitmap) {

		const Bitmap* pBitmap = g_pFresh->GetBitmap(bitmap);

		if (pBitmap) {

			FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

			if (pGDI) {

				WRect rect = GetRect();

				rect.x = x + 2.0f + rect.x;
				rect.y = y + 23.0f + rect.y;
				rect.w = (float)pBitmap->Width();
				rect.h = (float)pBitmap->Height();

				pGDI->DrawTexture(pBitmap, rect, 0xFFFFFFFF, 0);
			}
		}
	}
}

// cardremove.cpp:231
void FrCardRemove::PrintInDlg(const char* text, float x, float y) {
	if (text) {

		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		if (pGDI) {

			WRect rect = GetRect();

			pGDI->PrintText(WPoint(rect.x + x, rect.y + y), 0, text, 0);
		}
	}
}

// cardremove.cpp:250
void FrCardRemove::DrawTexture(const char* bitmap, float x, float y, unsigned long diffuse) {
	if (bitmap) {

		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		if (pGDI) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap(bitmap);

			if (pBitmap) {

				pGDI->DrawTexture(pBitmap, WRect(x, y, pBitmap->Width(), pBitmap->Height()), diffuse, 0);
			}
		}
	}
}

// cardremove.cpp:269
void FrCardRemove::PrintText(const char* text, float x, float y, unsigned long color) {
	if (text) {

		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

		if (pGDI) {

			pGDI->SetTextColor(color, 0xFFFFFFFF);
			pGDI->PrintText(WPoint(x, y), 0, text, 0);
			pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
		}
	}
}

// cardremove.cpp:284
bool FrCardRemove::CheckEquiped(unsigned long itemId) {
	std::map<unsigned int, sCharacterInfo>::iterator it;
	for (it = CSharedDoc::Instance()->m_characterMap.begin(); it != CSharedDoc::Instance()->m_characterMap.end(); ++it) {

		for (int i = 0; i < 24; ++i) {

			if ((*it).second.partIds[i] != 0) {

				if ((*it).second.partIds[i] == itemId) {

					return true;
				}
			}
		}
	}

	return false;
}

// cardremove.cpp:314
void FrCardRemove::SetCardRemoveItem(unsigned long itemTypeId, unsigned long itemId) {
	m_cardRemoveItemTypeId = itemTypeId;
	m_cardRemoveItemId = itemId;
}

// cardremove.cpp:320
bool FrCardRemove::SetCurPartUid(unsigned long partUid) {
	bool changed = false;
	if (partUid != m_curPartUid) {

		m_curPartUid = partUid;
		changed = true;
	}

	return changed;
}

// cardremove.cpp:333
void FrCardRemove::BuildCurPartCardList(void) {
	m_curPartCardList.clear();

	std::list<sSCardAvilityPeriodInfo>::iterator end = CSharedDoc::Instance()->m_cardPeriodList[0].end();

	std::list<sSCardAvilityPeriodInfo>::iterator it;
	for (it = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); it != end; ++it) {

		if ((*it).partsId == m_curPartUid) {

			m_curPartCardList.push_back((*it).typeId);
		}
	}
}

// cardremove.cpp:353 (run-time type and factory registration of FrCardRemove)
const WRTTI FrCardRemove::m_RTTI("FrCardRemove", &FrForm::m_RTTI);

IObject* FrCardRemoveMakeInstance(void) {
	return new FrCardRemove;
}

struct __sFrCardRemove {
	__sFrCardRemove(void) {
		ObjectFactory().AddObjectFunctor(FrCardRemoveMakeInstance, "FrCardRemove");
	}
};
static __sFrCardRemove s_registerFrCardRemove;

// cardremove.cpp:355
const sFRESH_MSGMAP* FrCardRemove::GetMessageMap(void) const {
	return &FrCardRemove::_MsgMap;
}

sFRESH_MSGMAP FrCardRemove::_MsgMap = { &FrForm::_MsgMap, &FrCardRemove::_MsgEntries[0] };

sFRESH_ENTRY FrCardRemove::_MsgEntries[] = {
	// cardremove.cpp:357
	{ "draw_area", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnDrawAreaInit },
	// cardremove.cpp:358
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnDrawAreaOwnerDraw },
	// cardremove.cpp:359
	{ "item_list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnItemListInit },
	// cardremove.cpp:360
	{ "item_list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnItemListOwnerDraw },
	// cardremove.cpp:361
	{ "item_list", FRCMD_DBLCLICK, FrFuncVV, (FRESH_PMSG)&FrCardRemove::OnItemListDBClick },
	// cardremove.cpp:362
	{ "item_list", FRCMD_RBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemove::OnItemListRBDown },
	// cardremove.cpp:363
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnConfirmInit },
	// cardremove.cpp:364
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemove::OnConfirmLBDown },
	// cardremove.cpp:365
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemove::OnCancelInit },
	// cardremove.cpp:366
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemove::OnCancelLBDown },

	// cardremove.cpp:368
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// cardremove.cpp:372
void FrCardRemove::OnDrawAreaInit(intptr_t param) {
	m_pDrawArea = DYNAMIC_CAST(FrArea, param);
}

// cardremove.cpp:496
void FrCardRemove::OnItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);

	if (m_pItemList) {

		m_pItemList->UseDblClick(true);
		m_pItemList->UseRightButton(true);

		// The parts with cards attached.
		std::list<sItemInfo>::iterator end = CSharedDoc::Instance()->m_myPartsList.end();

		std::list<sItemInfo>::iterator it;
		for (it = CSharedDoc::Instance()->m_myPartsList.begin(); it != end; ++it) {

			sItemInfo& info = *it;
			unsigned long serial = info.m_serial;

			if (!CheckEquiped(serial)) {

				std::list<sSCardAvilityPeriodInfo>::iterator cardEnd = CSharedDoc::Instance()->m_cardPeriodList[0].end();

				std::list<sSCardAvilityPeriodInfo>::iterator card;
				for (card = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); card != cardEnd; ++card) {

					if ((*card).partsId == serial) {

						IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(info.m_typeId);
						if (pPart) {

							info.m_charCardSlotCount = pPart->m_charCardSlots;
							info.m_caddieCardSlotCount = pPart->m_caddieCardSlots;
							m_pItemList->AddItem(&info);
						}
						break;
					}
				}
			}
		}

		// The other items with cards attached.
		for (it = CSharedDoc::Instance()->m_myItemList.begin(); it != CSharedDoc::Instance()->m_myItemList.end(); ++it) {

			sItemInfo& info = *it;
			unsigned long serial = info.m_serial;

			if (!CheckEquiped(serial)) {

				std::list<sSCardAvilityPeriodInfo>::iterator cardEnd = CSharedDoc::Instance()->m_cardPeriodList[0].end();

				std::list<sSCardAvilityPeriodInfo>::iterator card;
				for (card = CSharedDoc::Instance()->m_cardPeriodList[0].begin(); card != cardEnd; ++card) {

					if ((*card).partsId == serial) {

						IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(info.m_typeId);
						if (pPart) {

							info.m_charCardSlotCount = pPart->m_charCardSlots;
							info.m_caddieCardSlotCount = pPart->m_caddieCardSlots;
							m_pItemList->AddItem(&info);
						}
						break;
					}
				}
			}
		}
	}
}

// cardremove.cpp:576
void FrCardRemove::OnItemListOwnerDraw(intptr_t param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {


		FrListItem* pListItem = (FrListItem*)param;
		if (pListItem) {

			sItemInfo* pItem = (sItemInfo*)pListItem->pData;
			if (pItem) {

				float x = pListItem->pos.x;
				float y = pListItem->pos.y + 2.0f;




				bool expired = (pItem->m_stateBits.kind == 6 && pItem->m_stateBits.expired) ? true : false;

				unsigned long diffuse = expired ? 0x80808080 : 0xFFFFFFFF;
				if (!pListItem->selected && !pListItem->underCursor) {

					DrawTexture("button_n", x, y, diffuse);
				} else {

					DrawTexture("button_o", x, y, diffuse);
				}




				IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(pItem->m_typeId);

				if (pCommon) {


					unsigned long setTypeId = ((pItem->m_typeId >> 18) & 0xFF) | 0x4000000;
					IFF_ITEM_COMMON* pSet = ItemManager()->FindCommonItem(setTypeId);

					if (pSet) {

						const Bitmap* pSetIcon = g_pFresh->GetBitmap(MakeStr("s_%s", pSet->m_icon));

						if (pSetIcon) {




							pGDI->DrawTexture(pSetIcon, WRect(x + 5.0f, y + 3.0f, pSetIcon->Width() * 0.01f * 90.0f, pSetIcon->Height() * 0.01f * 90.0f), 0xFFFFFFFF, 0);
						}
					}

					const Bitmap* pIcon = g_pFresh->GetBitmap(pCommon->m_icon);

					if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pItem->m_serial)) {

						pIcon = GetUccItemIcon(pItem->m_serial);
					}

					if (pIcon) {




						pGDI->DrawTexture(pIcon, WRect(x + 30.0f, y + 1.0f, pIcon->Width() * 0.01f * 34.0f, pIcon->Height() * 0.01f * 34.0f), 0xFFFFFFFF, 0);
					}



					if (pListItem->selected == true) {

						pGDI->SetTextStyle(1);
						pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
					} else {

						pGDI->SetTextStyle(0);
						pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
					}



					if (!pListItem->selected && pListItem->underCursor) {

						pGDI->SetTextStyle(1);
					}


					if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(pItem->m_serial)) {

						pGDI->Print(WPoint(x + 61.0f, y + 10.0f), 0, GetUccItemName(pItem->m_serial));
					} else {

						pGDI->Print(WPoint(x + 61.0f, y + 10.0f), 0, pCommon->m_name);
					}



					if (pItem->m_stateBits.kind == 6) {

						const Bitmap* pClock = g_pFresh->GetBitmap("clock");
						if (pClock)
							pGDI->DrawTexture(pClock, WRect(pListItem->pos.x + 198.0f, pListItem->pos.y + 6.0f, pClock->Width(), pClock->Height()), 0xFFFFFFFF, 0);
					}



					DrawPartsSlot(pGDI, pItem, pListItem);


					pGDI->SetTextStyle(0);
					pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);
				}
			}
		}
	}
}

// cardremove.cpp:698
void FrCardRemove::OnItemListDBClick(void) {
}

// cardremove.cpp:704
void FrCardRemove::OnItemListRBDown(void) {
	FrListItem* pItem = m_pItemList->GetItemUnderCursor();
	if (pItem) {

		if (!pItem->selected) {

			m_pItemList->ToggleItem(pItem);
		}

		CRealMyRoomMain* pMain = (CRealMyRoomMain*)AfxGetTask()->GetActor("RealMyRoom");
		if (pMain) {

			pMain->OpenInformation(pItem);
		}
	}
}

// cardremove.cpp:720
void FrCardRemove::OnConfirmInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton) {

		pButton->SetPushDelay(0.1f);
	}
}

// cardremove.cpp:730
void FrCardRemove::OnConfirmLBDown(void) {
	if (m_curPartUid) {

		FrCardRemoveConfirm* pForm = CreateForm<FrCardRemoveConfirm>(g_pFresh->GetManager(), this, "card_remove_confirm", 0);
		if (pForm) {
			// "Use the card remover"
			pForm->SetCaption("\xC4\xAB\xB5\xE5\xC3\xDF\xC3\xE2\xBE\xD7 \xBB\xE7\xBF\xEB\xC8\xAE\xC0\xCE");
			pForm->SetPart(m_curPartTypeId, m_curPartUid);
			pForm->Open((FRESH_RESULT_CALLBACK)&FrCardRemove::OnUseCardRemoveConfirm, 0);
		}
	} else {

		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		pForm->SetTimeLimit(5.0f);
		// "First choose the clothes to remove the card from."
		pForm->SetMessage("\xB8\xD5\xC0\xFA \xC4\xAB\xB5\xE5\xB8\xA6 \xBA\xD0\xB8\xAE\xC7\xD2 \xC0\xC7\xBB\xF3\xC0\xBB \xBC\xB1\xC5\xC3\xC7\xD8\xC1\xD6\xBC\xBC\xBF\xE4.", false);
		pForm->Open(0, 0);
	}
}

// cardremove.cpp:751
bool FrCardRemove::OnUseCardRemoveConfirm(int result, FrForm* pForm) {
	if (result == FrOK) {

		if (IsCardRemoveItem(m_cardRemoveItemTypeId)) {

			WSendPacket packet((enumClientPacket)0xE2);
			packet.Encode4(m_cardRemoveItemTypeId);
			packet.Encode4(m_cardRemoveItemId);
			packet.Encode4(m_curPartTypeId);
			packet.Encode4(m_curPartUid);
			packet.Send(SEND_TO_DEFAULT);
		}

		Close(true);
	}

	return true;
}

// cardremove.cpp:771
void FrCardRemove::OnCancelInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton) {

		pButton->SetPushDelay(0.1f);
	}
}

// cardremove.cpp:781
void FrCardRemove::OnCancelLBDown(void) {
	Close(true);
}

// cardremove.cpp:792
FrCardRemoveConfirm::FrCardRemoveConfirm(void) : m_partTypeId(0), m_partId(0) {
}

// cardremove.cpp:796
FrCardRemoveConfirm::~FrCardRemoveConfirm(void) {
}

// cardremove.cpp:801 (run-time type and factory registration of FrCardRemoveConfirm)
const WRTTI FrCardRemoveConfirm::m_RTTI("FrCardRemoveConfirm", &FrForm::m_RTTI);

IObject* FrCardRemoveConfirmMakeInstance(void) {
	return new FrCardRemoveConfirm;
}

struct __sFrCardRemoveConfirm {
	__sFrCardRemoveConfirm(void) {
		ObjectFactory().AddObjectFunctor(FrCardRemoveConfirmMakeInstance, "FrCardRemoveConfirm");
	}
};
static __sFrCardRemoveConfirm s_registerFrCardRemoveConfirm;

// cardremove.cpp:803
const sFRESH_MSGMAP* FrCardRemoveConfirm::GetMessageMap(void) const {
	return &FrCardRemoveConfirm::_MsgMap;
}

sFRESH_MSGMAP FrCardRemoveConfirm::_MsgMap = { &FrForm::_MsgMap, &FrCardRemoveConfirm::_MsgEntries[0] };

sFRESH_ENTRY FrCardRemoveConfirm::_MsgEntries[] = {
	// cardremove.cpp:805
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemoveConfirm::OnDrawAreaOwnerDraw },
	// cardremove.cpp:806
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemoveConfirm::OnConfirmInit },
	// cardremove.cpp:807
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemoveConfirm::OnConfirmLBDown },
	// cardremove.cpp:808
	{ "btn_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemoveConfirm::OnCancelInit },
	// cardremove.cpp:809
	{ "btn_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemoveConfirm::OnCancelLBDown },

	// cardremove.cpp:811
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// cardremove.cpp:815
void FrCardRemoveConfirm::OnDrawAreaOwnerDraw(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {

		WRect rect = GetRect();
		WPoint pos(rect.x + rect.w * 0.5f, rect.y + 55.0f);
		// "Do you want to use the card remover?"
		pGDI->PrintText(pos, 1, "\xC4\xAB\xB5\xE5\xC3\xDF\xC3\xE2\xBE\xD7\xC0\xBB \xBB\xE7\xBF\xEB\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", 0);
	}
}

// cardremove.cpp:826
void FrCardRemoveConfirm::OnConfirmInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);


	if (pButton) {

		pButton->SetPushDelay(0.1f);
	}
}

// cardremove.cpp:837
void FrCardRemoveConfirm::OnConfirmLBDown(void) {
	if (m_partTypeId && m_partId) {

		Close(FrOK, true);
	} else {

		Close(true);
	}
}

// cardremove.cpp:849
void FrCardRemoveConfirm::OnCancelInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);


	if (pButton) {

		pButton->SetPushDelay(0.1f);
	}
}

// cardremove.cpp:860
void FrCardRemoveConfirm::OnCancelLBDown(void) {
	Close(true);
}

// cardremove.cpp:867 (run-time type and factory registration of FrCardRemoveResult)
const WRTTI FrCardRemoveResult::m_RTTI("FrCardRemoveResult", &FrForm::m_RTTI);

IObject* FrCardRemoveResultMakeInstance(void) {
	return new FrCardRemoveResult;
}

struct __sFrCardRemoveResult {
	__sFrCardRemoveResult(void) {
		ObjectFactory().AddObjectFunctor(FrCardRemoveResultMakeInstance, "FrCardRemoveResult");
	}
};
static __sFrCardRemoveResult s_registerFrCardRemoveResult;

// cardremove.cpp:869
const sFRESH_MSGMAP* FrCardRemoveResult::GetMessageMap(void) const {
	return &FrCardRemoveResult::_MsgMap;
}

sFRESH_MSGMAP FrCardRemoveResult::_MsgMap = { &FrForm::_MsgMap, &FrCardRemoveResult::_MsgEntries[0] };

sFRESH_ENTRY FrCardRemoveResult::_MsgEntries[] = {
	// cardremove.cpp:871
	{ "draw_area", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemoveResult::OnDrawAreaOwnerDraw },
	// cardremove.cpp:872
	{ "btn_confirm", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardRemoveResult::OnConfirmInit },
	// cardremove.cpp:873
	{ "btn_confirm", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrCardRemoveResult::OnConfirmLBDown },

	// cardremove.cpp:875
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// cardremove.cpp:887
FrCardRemoveResult::FrCardRemoveResult(void) : m_result((eRESULT)0) {

	m_pCardBitmap = 0; m_pCoverMask = 0; m_cardX = 0; m_cardY = 0; m_cardWidth = 0; m_cardHeight = 0; m_maskX = 0; m_maskY = 0; m_maskWidth = 0; m_maskOffset = 0; m_state = 0; m_removerTypeId = 0;
}

// cardremove.cpp:892
FrCardRemoveResult::~FrCardRemoveResult(void) {
}

// cardremove.cpp:896
bool FrCardRemoveResult::OnInit(void) {
	IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(m_removerTypeId);

	if (pCommon == 0)
		return true;

	m_pCardBitmap = g_pFresh->GetBitmap(MakeStr("%s_for_resultdlg", pCommon->m_icon));
	m_pCoverMask = g_pFresh->GetBitmap("card_remover_covermask");

	if (m_pCardBitmap) {

		m_cardWidth = (float)m_pCardBitmap->Width();
		m_cardHeight = (float)m_pCardBitmap->Height();
	}

	if (m_pCoverMask) {

		m_maskWidth = (float)m_pCoverMask->Width();
		m_maskOffset = 0;
	}

	return true;
}

// cardremove.cpp:931
void FrCardRemoveResult::OnProc(float dt) {
	if (m_result == 0 || m_result == 3) {

		FrWnd* pConfirm = FindChildByName("btn_confirm");
		if (pConfirm && !pConfirm->IsVisible()) {
			pConfirm->SetVisible(true);
		}
		return;
	}

	switch (m_state) {
	case 0: {
		m_state = 1;

		WRect rect = GetRect();
		SetRect(WRect(rect.x, rect.y, rect.w, rect.h + 50.0f));
		break;
	}
	case 1: {
		m_cardX = (m_rect.w - m_cardWidth) * 0.5f + m_rect.x;
		m_cardY = m_rect.y + 30.0f;

		m_maskX = m_cardX;
		m_maskY = m_cardY + 115.0f;
		m_maskOffset += dt * 20.0f;


		if (m_maskOffset > 40.0f) {

			m_maskOffset = 40.0f;
			m_state = 2;

			WRect rect(m_rect);
			SetRect(WRect(rect.x, rect.y, rect.w, rect.h - 50.0f));
		}
		break;
	}
	case 2: {
		FrWnd* pConfirm = FindChildByName("btn_confirm");
		if (pConfirm && !pConfirm->IsVisible()) {
			pConfirm->SetVisible(true);
		}
		break;
	}
	}
}

// cardremove.cpp:1075
void FrCardRemoveResult::OnConfirmInit(intptr_t param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);


	if (pButton) {

		pButton->SetPushDelay(0.1f);
		pButton->SetVisible(false);
	}
}

// cardremove.cpp:1087
void FrCardRemoveResult::OnConfirmLBDown(void) {
	Close(true);
}
