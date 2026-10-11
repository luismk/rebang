// Original: d:\build\custom\temp\code\client\projectg\fortuneopendlg.cpp
#include <algorithm>

#include "FortuneOpenDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "frstatic.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "../shared/classdefine.h"
#include "bitmap.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// fortuneopendlg.cpp:469 (run-time type and factory registration of FrFortuneOpenDlg)
const WRTTI FrFortuneOpenDlg::m_RTTI("FrFortuneOpenDlg", &FrForm::m_RTTI);

IObject* FrFortuneOpenDlgMakeInstance(void) {
	return new FrFortuneOpenDlg;
}

struct __sFrFortuneOpenDlg {
	__sFrFortuneOpenDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFortuneOpenDlgMakeInstance, "FrFortuneOpenDlg");
	}
};
static __sFrFortuneOpenDlg s_registerFrFortuneOpenDlg;

// fortuneopendlg.cpp:471
const sFRESH_MSGMAP* FrFortuneOpenDlg::GetMessageMap(void) const {
	return &FrFortuneOpenDlg::_MsgMap;
}

sFRESH_MSGMAP FrFortuneOpenDlg::_MsgMap = { &FrForm::_MsgMap, &FrFortuneOpenDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFortuneOpenDlg::_MsgEntries[] = {
	// fortuneopendlg.cpp:473
	{ "list", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneOpenDlg::OnItemListInit },
	{ "list", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneOpenDlg::OnItemListOwnerDraw },
	{ "total_price", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneOpenDlg::OnTotalPriceInit },
	{ "desc_items", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFortuneOpenDlg::OnDescItemsInit },

	// fortuneopendlg.cpp:478
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// fortuneopendlg.cpp:481
FrFortuneOpenDlg::FrFortuneOpenDlg(void) {
	m_pItemList = 0;
	m_pTotalPrice = 0;
	m_pDescItems = 0;
	m_typeId = 0;
}

// fortuneopendlg.cpp:490
FrFortuneOpenDlg::~FrFortuneOpenDlg(void) {
}

// fortuneopendlg.cpp:494
void FrFortuneOpenDlg::OnItemListInit(intptr_t param) {
	m_pItemList = DYNAMIC_CAST(FrListBox, param);
}

// fortuneopendlg.cpp:499
void FrFortuneOpenDlg::OnItemListOwnerDraw(intptr_t param) {
	FrListItem* pListItem = (FrListItem*)param;
	if (pListItem == 0) {
		return;
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0) {
		return;
	}

	sFortune* pFortune = (sFortune*)pListItem->pData;
	if (pFortune == 0) {
		return;
	}

	char where[64];
	const char* pPlace;
	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(pFortune->m_typeId);
	const Bitmap* pBitmap = 0;

	switch (pFortune->m_typeId >> 26) {
	case 6:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xBE\xC6\xC0\xCC\xC5\xDB\xBC\xA5 > \xB0\xD4\xC0\xD3 \xBE\xC6\xC0\xCC\xC5\xDB \xBC\xA5";
		strcpy(where, pPlace);
		break;

	case 2:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xB3\xBB\xBF\xCA\xC0\xE5 > \xC4\xB3\xB8\xAF\xC5\xCD \xB2\xD9\xB9\xCC\xB1\xE2";
		strcpy(where, pPlace);
		break;

	case 5:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xBE\xC6\xC0\xCC\xC5\xDB\xBC\xA5 > \xC0\xE5\xBA\xF1 \xBC\xA5";
		strcpy(where, pPlace);
		break;

	case 4:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xBE\xC6\xC0\xCC\xC5\xDB\xBC\xA5 > \xC0\xE5\xBA\xF1 \xBC\xA5";
		strcpy(where, pPlace);
		break;

	case 9:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS_FASHION", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xB3\xBB\xBF\xCA\xC0\xE5 > \xC4\xB3\xB8\xAF\xC5\xCD \xB2\xD9\xB9\xCC\xB1\xE2";
		strcpy(where, pPlace);
		break;

	case 13:
		pBitmap = g_pFresh->GetManager()->GetBitmap("ITEMS", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xBE\xC6\xC0\xCC\xC5\xDB\xBC\xA5 > \xB0\xD4\xC0\xD3 \xBE\xC6\xC0\xCC\xC5\xDB \xBC\xA5";
		strcpy(where, pPlace);
		break;

	case 28:
		pBitmap = g_pFresh->GetManager()->GetBitmap("AUXPART", pItem->m_icon);
		pPlace = "\xBC\xD2\xC0\xAF\xC0\xA7\xC4\xA1: \xB3\xBB\xBF\xCA\xC0\xE5 > \xC4\xB3\xB8\xAF\xC5\xCD \xB2\xD9\xB9\xCC\xB1\xE2";
		strcpy(where, pPlace);
		break;
	}

	if (pBitmap) {
		pGDI->DrawTexture(pBitmap, WRect(pListItem->pos.x + 10.0f, pListItem->pos.y + (m_pItemList->GetItemHeight() - pBitmap->Height()) * 0.5f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pGDI->SetTextStyle(1);

	if ((pFortune->m_typeId & 0xFC000000) == 0x8000000) {
		IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(((pFortune->m_typeId >> 18) & 0xFF) | 0x4000000);
		pGDI->Print(WPoint(pListItem->pos.x + 80.0f, pListItem->pos.y + 10.0f), 0, "%s(%s)", pItem->m_name, pChar ? pChar->m_name : "");
	} else {
		pGDI->Print(WPoint(pListItem->pos.x + 80.0f, pListItem->pos.y + 10.0f), 0, pItem->m_name);
	}

	pGDI->SetTextStyle(0);
	pGDI->Print(WPoint(pListItem->pos.x + 80.0f, pListItem->pos.y + 33.0f), 0, "\xB0\xB9   \xBC\xF6: %d\xB0\xB3", pFortune->m_count);
	pGDI->Print(WPoint(pListItem->pos.x + 80.0f, pListItem->pos.y + 50.0f), 0, where);
}

// fortuneopendlg.cpp:581
void FrFortuneOpenDlg::OnTotalPriceInit(intptr_t param) {
	m_pTotalPrice = DYNAMIC_CAST(FrStatic, param);
}

// fortuneopendlg.cpp:586
void FrFortuneOpenDlg::OnDescItemsInit(intptr_t param) {
	m_pDescItems = DYNAMIC_CAST(FrStatic, param);
}

// fortuneopendlg.cpp:591
void FrFortuneOpenDlg::SetItemList(unsigned long* pTypeIds, int count) {
	m_fortunes.clear();

	short quantity;
	for (int i = 0; i < count; i++) {
		std::list<sFortune>::iterator it = std::find(m_fortunes.begin(), m_fortunes.end(), pTypeIds[i]);

		switch (pTypeIds[i] >> 26) {
		case 5: {
			IFF_STRUCT::sBall* pBall = ItemManager()->FindBall(pTypeIds[i]);
			if (pBall) {
				quantity = pBall->quantity;
			}
			break;
		}

		case 6:
			if (pTypeIds[i] & 0x2000000) {
				IFF_STRUCT::sItem* pData = ItemManager()->FindItem(pTypeIds[i]);
				if (pData) {
					quantity = pData->quantity;
				}
				break;
			}
			quantity = 1;
			break;

		default:
			quantity = 1;
			break;
		}

		if (it == m_fortunes.end()) {
			sFortune fortune;
			fortune.m_typeId = pTypeIds[i];
			fortune.m_count = quantity;
			m_fortunes.push_back(fortune);
		} else {
			it->m_count += quantity;
		}
	}

	int totalPrice = 0;

	if (m_pItemList) {
		for (std::list<sFortune>::iterator it = m_fortunes.begin(); it != m_fortunes.end(); ++it) {
			sFortune* pFortune = &*it;
			IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(pFortune->m_typeId);

			if ((pFortune->m_typeId >> 26) == 2) {
				continue;
			}
			if (pItem == 0) {
				continue;
			}
			if ((pItem->m_flags0 & 1) == 0) {
				continue;
			}

			if (pItem->m_discountPrice == 0) {
				if ((pFortune->m_typeId >> 26) != 6 || (pFortune->m_typeId & 0x2000000) != 0) {
					switch (pFortune->m_typeId >> 26) {
					case 6: {
						IFF_STRUCT::sItem* pData = ItemManager()->FindItem(pFortune->m_typeId);
						if (pData) {
							totalPrice += pFortune->m_count * pItem->m_price * 10 / pData->quantity;
						}
						break;
					}

					case 5: {
						IFF_STRUCT::sBall* pBall = ItemManager()->FindBall(pFortune->m_typeId);
						if (pBall) {
							totalPrice += pFortune->m_count * pItem->m_price * 10 / pBall->quantity;
						}
						break;
					}

					default:
						totalPrice += pItem->m_price * 10;
						break;
					}
				} else {
					unsigned long price = pItem->m_price * 10 / 10;
					if (price < 1) {
						price = 1;
					}
					totalPrice += pFortune->m_count * price;
				}
			} else {
				if ((pFortune->m_typeId >> 26) != 6 || (pFortune->m_typeId & 0x2000000) != 0) {
					switch (pFortune->m_typeId >> 26) {
					case 6: {
						IFF_STRUCT::sItem* pData = ItemManager()->FindItem(pFortune->m_typeId);
						if (pData) {
							totalPrice += pFortune->m_count * pItem->m_discountPrice * 10 / pData->quantity;
						}
						break;
					}

					case 5: {
						IFF_STRUCT::sBall* pBall = ItemManager()->FindBall(pFortune->m_typeId);
						if (pBall) {
							totalPrice += pFortune->m_count * pItem->m_discountPrice * 10 / pBall->quantity;
						}
						break;
					}

					default:
						totalPrice += pItem->m_discountPrice * 10;
						break;
					}
				} else {
					unsigned long price = pItem->m_discountPrice * 10 / 10;
					if (price < 1) {
						price = 1;
					}
					totalPrice += pFortune->m_count * price;
				}
			}

			m_pItemList->AddItem(pFortune);
		}

		m_pItemList->SetKeyFocus(true);
	}

	if (m_pTotalPrice) {
		switch (m_typeId & 0x1FFFFFF) {
		case 0x19:
		case 0x1B:
		case 0x90:
			m_pTotalPrice->SetCaption(MakeStr("\xBE\xC6\xC0\xCC\xC5\xDB \xB0\xA1\xB0\xDD \xC7\xD5\xB0\xE8 : %d \xC4\xED\xC5\xB0", totalPrice / 10));
			break;
		}
	}
}

// fortuneopendlg.cpp:802
void FrFortuneOpenDlg::SetItemNewList(unsigned long* pTypeIds, int count) {
	m_fortunes.clear();

	for (int i = 0; i < count; i++) {
		std::list<sFortune>::iterator it = std::find(m_fortunes.begin(), m_fortunes.end(), pTypeIds[i]);

		short count = 1;
		switch (pTypeIds[i] >> 26) {
		case 5:
			count = 1;
			break;
		case 6:
			count = 1;
			break;
		case 0x1c:
			count = 1;
			break;
		}

		if (it == m_fortunes.end()) {
			sFortune fortune;
			fortune.m_typeId = pTypeIds[i];
			fortune.m_count = count;
			m_fortunes.push_back(fortune);
		} else {
			it->m_count++;
		}
	}

	if (m_pItemList) {
		for (std::list<sFortune>::iterator it = m_fortunes.begin(); it != m_fortunes.end(); ++it) {
			IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(it->m_typeId);
			if (pItem == 0) {
				continue;
			}

			// Both arms are the same (the price sums of SetItemList are gone); the
			// compiler merges them and keeps only the m_discountPrice compare.
			if (pItem->m_discountPrice == 0) {
				unsigned long typeId = it->m_typeId;
				unsigned long type = typeId >> 26;
				if (type != 6 || (typeId & 0x2000000) != 0) {
					switch (type) {
					case 6:
						ItemManager()->FindItem(typeId);
						break;
					case 5:
						ItemManager()->FindBall(typeId);
						break;
					}
				}
			} else {
				unsigned long typeId = it->m_typeId;
				unsigned long type = typeId >> 26;
				if (type != 6 || (typeId & 0x2000000) != 0) {
					switch (type) {
					case 6:
						ItemManager()->FindItem(typeId);
						break;
					case 5:
						ItemManager()->FindBall(typeId);
						break;
					}
				}
			}

			m_pItemList->AddItem(&*it);
		}

		m_pItemList->SetKeyFocus(true);
	}
}

// fortuneopendlg.cpp:930
void FrFortuneOpenDlg::SetTypeId(unsigned long typeId) {
	m_typeId = typeId;

	IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(typeId);

	if (m_pDescItems && pItem) {
		m_pDescItems->SetCaption(MakeStr("%s \xC0\xBB(\xB8\xA6) \xBF\xAD\xBE\xEE \xB4\xD9\xC0\xBD\xB0\xFA \xB0\xB0\xC0\xBA \xBE\xC6\xC0\xCC\xC5\xDB\xB5\xE9\xC0\xBB \xC8\xB9\xB5\xE6\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.", pItem->m_name));
	}
}
