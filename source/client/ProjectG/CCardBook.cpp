// Original: d:\build\custom\temp\code\client\projectg\ccardbook.cpp
#include "CCardBook.h"
#include "../client/Fresh/include/fresh.h"
#include "../client/Fresh/include/frgraphicinterface.h"
#include "../client/Wangreal/include/bitmap.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/ProjectG/UccManager.h"
#include "../client/ProjectG/commonutil.h"
#include "../client/ProjectG/SharedDoc.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "../client/Fresh/include/frform.h"
#include "../client/ProjectG/CardDlg.h"
#include "../client/ProjectG/TaskManager.h"
#include "../client/ProjectG/actor.h"
#include "../client/ProjectG/Packet.h"

// ccardbook.cpp:23
CCardSlot::CCardSlot(void) {
	m_pushed = 0;
	m_rect = WRect(0.0f, 0.0f, 0.0f, 0.0f);
	m_pButton = NULL;
	m_pCardInfo = NULL;
	m_pCommonItem = NULL;
	m_pCards = NULL;
	m_pCardOverlay = NULL;
	m_pOpenCardPackDlg = NULL;
	m_pItemAttachCardDlg = NULL;
	m_pSpCardUseDlg = NULL;
}

// ccardbook.cpp:37
CCardSlot::~CCardSlot(void) {
	if (m_pCardOverlay != NULL) {
		if (g_resrcmng != NULL && m_pCardOverlay != NULL) { g_resrcmng->Release(m_pCardOverlay); m_pCardOverlay = NULL; }
	}
}

// ccardbook.cpp:45
void CCardSlot::SetCard(unsigned long typeId) {
	m_pCards = NULL;

	if (typeId != 0) {
		for (std::list<sCards>::iterator it = CSharedDoc::Instance()->m_myCardList.begin(); it != CSharedDoc::Instance()->m_myCardList.end(); ++it) {
			m_pCardInfo = ItemManager()->FindCard(typeId);
			m_pCommonItem = ItemManager()->FindCommonItem(typeId);
			m_pCards = &*it;

			if (m_pCards != NULL) {
				if (m_pCards->m_typeId == typeId) {
					break;
				}

				m_pCardInfo = NULL;
				m_pCommonItem = NULL;
				m_pCards = NULL;
			}
		}
	} else {
		m_pCardInfo = NULL;
		m_pCommonItem = NULL;
	}

	if (m_pButton != NULL && typeId != 0) {
		if (m_pCardInfo != NULL) {
			if (m_pCommonItem != NULL) {
				m_pButton->SetVisible(true);
				m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::NORMAL);
				m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::OVER);
				m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::PRESSED);
			}
		}
	} else if (m_pButton != NULL && typeId == 0) {
		m_pButton->SetVisible(false);
	}

	m_pushed = 0;
	if (m_pCardOverlay != NULL) {
		if (g_resrcmng != NULL && m_pCardOverlay != NULL) { g_resrcmng->Release(m_pCardOverlay); m_pCardOverlay = NULL; }
	}
}

// ccardbook.cpp:107
void CCardSlot::SetPosition(WRect& rect) {
	m_rect = rect;
}

// ccardbook.cpp:112
void CCardSlot::Select(unsigned char pushed) {
	m_pushed = pushed;
}

// ccardbook.cpp:118
void CCardSlot::UseCard(void) {
	if (m_pCardInfo == NULL || m_pCommonItem == NULL || m_pCards == NULL) {
		return;
	}

	switch ((m_pCards->m_typeId >> 22) & 0xF) {
	case 3:
		m_pOpenCardPackDlg = CreateForm<FrOpenCardPackDlg>(g_pFresh->GetManager(), this, "cardpack_open", NULL);
		m_pOpenCardPackDlg->m_cardId = m_pCards->m_guid;
		m_pOpenCardPackDlg->m_cardTypeId = m_pCards->m_typeId;
		m_pOpenCardPackDlg->Open((FRESH_RESULT_CALLBACK)&CCardSlot::OnCardPackDlgResult, 1);
		break;

	case 2:
		m_pSpCardUseDlg = CreateForm<FrSpCardUseDlg>(g_pFresh->GetManager(), this, "usecard", NULL);
		m_pSpCardUseDlg->SetSpCardInfo(m_pCards->m_typeId);
		m_pSpCardUseDlg->Open((FRESH_RESULT_CALLBACK)&CCardSlot::OnSpCardDlgResult, 1);
		break;

	default:
		m_pCardUseDlg = CreateForm<FrCardUseDlg>(g_pFresh->GetManager(), this, "usecard", NULL);
		m_pCardUseDlg->m_cardId = m_pCards->m_guid;
		m_pCardUseDlg->m_cardTypeId = m_pCards->m_typeId;
		m_pCardUseDlg->Open((FRESH_RESULT_CALLBACK)&CCardSlot::OnCardDlgResult, 1);
		break;
	}
}

// ccardbook.cpp:175
bool CCardSlot::OnCardPackDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		FrOpenCardPackDlg* pDlg = (FrOpenCardPackDlg*)pForm;
		if (pDlg != NULL && ((pDlg->m_cardTypeId >> 22) & 0xF) == 3) {
			CTaskManager* pTaskManager = CTaskManager::Instance();
			(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

			WSendPacket packet((enumClientPacket)0xC2);
			packet.Encode4(pDlg->m_cardTypeId);
			packet.Encode4(pDlg->m_cardId);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	m_pOpenCardPackDlg = NULL;
	return true;
}

// ccardbook.cpp:198
bool CCardSlot::OnSpCardDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		FrSpCardUseDlg* pDlg = (FrSpCardUseDlg*)pForm;
		if (pDlg != NULL) {
			CTaskManager* pTaskManager = CTaskManager::Instance();
			(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

			WSendPacket packet((enumClientPacket)0xB5);
			packet.Encode4(pDlg->GetSpCardTid());
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	m_pSpCardUseDlg = NULL;
	return true;
}

// ccardbook.cpp:217
bool CCardSlot::OnCardDlgResult(int result, FrForm* pForm) {
	FrCardUseDlg* pDlg = (FrCardUseDlg*)pForm;
	if (result == 1 && pDlg != NULL) {
		int cardKind = (pDlg->m_cardTypeId >> 22) & 0xF;
		if (cardKind == 2) {
			CTaskManager* pTaskManager = CTaskManager::Instance();
			(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));

			WSendPacket packet((enumClientPacket)0xB5);
			packet.Encode4(pDlg->m_cardTypeId);
			packet.Send(SEND_TO_DEFAULT);
		} else if (cardKind == 0 || cardKind == 1) {
			m_pItemAttachCardDlg = CreateForm<FrItemAttachCardDlg>(g_pFresh->GetManager(), this, "itemattachcarddlg", NULL);
			m_pItemAttachCardDlg->m_cardId = pDlg->m_cardId;
			m_pItemAttachCardDlg->m_cardTypeId = pDlg->m_cardTypeId;
			m_pItemAttachCardDlg->m_cardKind = (pDlg->m_cardTypeId >> 22) & 0xF;
			m_pItemAttachCardDlg->Open((FRESH_RESULT_CALLBACK)&CCardSlot::OnItemAttachCardDlgResult, 1);
		}
	}

	m_pCardUseDlg = NULL;
	return true;
}

// The card-to-item attach request (packet 0xC0), 0x3a bytes.
#pragma pack(push, 1)
struct sAttachCardRequest {
	unsigned long cardId;       // 0x0
	unsigned long cardTypeId;   // 0x4
	unsigned long itemTypeId;   // 0x8
	unsigned long itemId;       // 0xc
	int slot;                   // 0x10
	unsigned long reserved14[9];   // 0x14, never filled
	unsigned short reserved38;     // 0x38
};
#pragma pack(pop)
CHECK_SIZE(sAttachCardRequest, 0x3A);

// ccardbook.cpp:253
bool CCardSlot::OnItemAttachCardDlgResult(int result, FrForm* pForm) {
	sAttachCardRequest request;
	FrItemAttachCardDlg* pDlg = (FrItemAttachCardDlg*)pForm;
	if (pDlg == NULL) {
		return true;
	}

	if (result != 1) {
		return true;
	}

	if (pDlg->m_selectedSlot == 0xFF) {
		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xc6\xc4\xc3\xf7\xc0\xc7 \xbd\xbd\xb7\xd4\xc0\xcc \xbc\xb1\xc5\xc3\xb5\xc7\xc1\xf6 \xbe\xca\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
		return true;
	}

	IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pDlg->m_itemTypeId);
	if (pPart == NULL) {
		return true;
	}

	if (ItemManager()->IsPartAttachCard(pDlg->m_cardTypeId, pDlg->m_itemTypeId, pDlg->m_selectedSlot)) {
		request.cardId = pDlg->m_cardId;
		request.cardTypeId = pDlg->m_cardTypeId;
		request.itemTypeId = pDlg->m_itemTypeId;
		request.itemId = pDlg->m_itemId;
		request.slot = pDlg->m_selectedSlot;

		WSendPacket packet((enumClientPacket)0xC0);
		packet.EncodeBuffer(&request, sizeof(request));
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	} else {
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xbd\xbd\xb7\xd4\xb0\xfa \xc4\xab\xb5\xe5\xc0\xc7 \xc5\xb8\xc0\xd4\xc0\xcc \xb4\xde\xb6\xf3\xbc\xad \xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
	}

	return true;
}

// ccardbook.cpp:452
bool CCardSlot::BoundControl(FrButton* pButton) {
	m_pButton = pButton;

	if (pButton != NULL) {
		if (m_pCommonItem != NULL) {
			m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::NORMAL);
			m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::OVER);
			m_pButton->SetButtonImg(m_pCommonItem->m_icon, FrButton::PRESSED);
		}

		m_pButton->SetVisible(false);
		m_pButton->SetPushDelay(0.1f);
		m_pButton->UseDblClick(true);
	}

	return m_pButton ? true : false;
}

// ccardbook.cpp:487
bool CCardPackPage::Init(void) {
	// The card pack slots: the pack's type id and where the slot sits.
	struct sCardPackSlot {
		unsigned long typeId;
		WRect rect;
	};

	m_pCardSlots = new CCardSlot[8];

	sCardPackSlot slots[8] = {
		{ 0x7CC00000, WRect(439.0f, 247.0f, 0, 0) },
		{ 0x7CC00004, WRect(521.0f, 247.0f, 0, 0) },
		{ 0, WRect(603.0f, 247.0f, 0, 0) },
		{ 0, WRect(685.0f, 247.0f, 0, 0) },
		{ 0x7CC00001, WRect(439.0f, 377.0f, 0, 0) },
		{ 0x7CC00002, WRect(521.0f, 377.0f, 0, 0) },
		{ 0x7CC00003, WRect(603.0f, 377.0f, 0, 0) },
		{ 0, WRect(685.0f, 377.0f, 0, 0) },
	};

	if (m_pCardPackList == NULL) {
		m_pCardPackList = new std::vector<unsigned long>;
		if (m_pCardPackList != NULL) {
			m_pCardPackList->clear();
		}
	}

	for (int i = 0; i < 8; i++) {
		if (m_pCardPackList != NULL) {
			m_pCardPackList->push_back(slots[i].typeId);
		}

		m_pCardSlots[i].SetCard(0);

		WRect rect(slots[i].rect.x, slots[i].rect.y, 0.0f, 0.0f);
		m_pCardSlots[i].SetPosition(rect);

		m_pCardSlots[i].Select(0);
	}

	return false;
}

// ccardbook.cpp:533
void CCardPackPage::Draw(FrGraphicInterface* pGraphic) {
	for (int i = 0; i < 8; i++) {
		if (m_pCardSlots != NULL) {
			m_pCardSlots[i].Draw(pGraphic);

			sCards* pCards = m_pCardSlots[i].GetCards();
			if (pCards != NULL) {
				if (m_pFont != NULL) {
					WRect rect = m_pCardSlots[i].GetPosition();
					m_pFont->Print(g_view, rect.x + 42.0f, rect.y + 50.0f, MakeStr("%d", pCards->m_count), 0, 0xFFFFFFFF, NULL);
				}
			}
		}
	}
}

// ccardbook.cpp:558
void CCardPackPage::SetCard(unsigned char slot, unsigned long typeId) {
	if (m_pCardSlots != NULL) {
		m_pCardSlots[slot].SetCard(typeId);
	}
}

// ccardbook.cpp:566
void CCardPackPage::ShowSlotControl(bool show) {
	for (int i = 0; i < 8; i++) {
		if (m_pCardSlots != NULL) {
			if (m_pCardSlots[i].GetCardInfo() != NULL) {
				m_pCardSlots[i].Show(show);
			}
		}
	}
}

// ccardbook.cpp:580
void CCardPackPage::SelectSlot(unsigned char control, unsigned char slot) {
	if (m_pCardSlots == NULL) {
		return;
	}

	bool useCard = false;

	if (control == 2) {
		control = 1;
		useCard = true;
	}

	for (int i = 0; i < 8; i++) {
		if (i == slot) {
			if (control != 1) {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(control);
				}
			} else {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(1);
				}
			}
		} else {
			if (control != 1) {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(control);
				}
			} else {
				m_pCardSlots[i].Select(0);
			}
		}
	}

	if (useCard == true) {
		if (m_pCardSlots != NULL) {
			m_pCardSlots[slot].UseCard();
		}
	}
}

// ccardbook.cpp:650
CCardPage::CCardPage(void) {
	m_cardType = 0;

	m_pCardList1 = NULL;
	m_pCardList2 = NULL;
	m_pCardList3 = NULL;
	m_pCardList4 = NULL;

	m_pSlotBackGround = NULL;
}

// ccardbook.cpp:662
CCardPage::~CCardPage(void) {
	if (m_pCardList1 != NULL) {
		delete[] m_pCardList1;
		m_pCardList1 = NULL;
	}

	if (m_pCardList2 != NULL) {
		delete[] m_pCardList2;
		m_pCardList2 = NULL;
	}

	if (m_pCardList3 != NULL) {
		delete[] m_pCardList3;
		m_pCardList3 = NULL;
	}

	if (m_pCardList4 != NULL) {
		delete[] m_pCardList4;
		m_pCardList4 = NULL;
	}
}

// ccardbook.cpp:690
bool CCardPage::Init(void) {
	PageSeparate(m_pCardList1, 1);
	PageSeparate(m_pCardList2, 2);
	PageSeparate(m_pCardList3, 3);
	PageSeparate(m_pCardList4, 4);

	m_pCardSlots = new CCardSlot[12];

	WRect rects[12];
	rects[0] = WRect(426.0f, 216.0f, 0.0f, 0.0f);
	rects[1] = WRect(517.0f, 216.0f, 0.0f, 0.0f);
	rects[2] = WRect(607.0f, 216.0f, 0.0f, 0.0f);
	rects[3] = WRect(698.0f, 216.0f, 0.0f, 0.0f);
	rects[4] = WRect(426.0f, 298.0f, 0.0f, 0.0f);
	rects[5] = WRect(517.0f, 298.0f, 0.0f, 0.0f);
	rects[6] = WRect(607.0f, 298.0f, 0.0f, 0.0f);
	rects[7] = WRect(698.0f, 298.0f, 0.0f, 0.0f);
	rects[8] = WRect(426.0f, 380.0f, 0.0f, 0.0f);
	rects[9] = WRect(517.0f, 380.0f, 0.0f, 0.0f);
	rects[10] = WRect(607.0f, 380.0f, 0.0f, 0.0f);
	rects[11] = WRect(698.0f, 380.0f, 0.0f, 0.0f);

	for (int i = 0; i < 12; i++) {
		m_pCardSlots[i].SetCard(0);
		m_pCardSlots[i].SetPosition(rects[i]);
		m_pCardSlots[i].Select(0);
	}

	m_pSlotBackGround = g_pFresh->GetBitmap("collection_bg");

	return false;
}

// ccardbook.cpp:728
void CCardPage::Draw(FrGraphicInterface* pGraphic) {
	unsigned long total = 0;
	unsigned long pageCount = GetPageCount(m_cardType);

	for (unsigned long page = 0; page < pageCount; page++) {
		std::vector<unsigned long>* pList = GetCardList(m_cardType);
		if (pList == NULL) {
			break;
		}

		total += pList[page].size();
	}

	for (int i = 0; i < 12; i++) {
		if (m_pCardSlots != NULL) {
			if (total > (unsigned long)(m_curPage * 12 + i)) {
				if (m_pSlotBackGround != NULL) {
					WRect rect = m_pCardSlots[i].GetPosition();
					pGraphic->DrawTexture(m_pSlotBackGround, WRect(rect.x, rect.y, m_pSlotBackGround->Width(), m_pSlotBackGround->Height()), 0xFFFFFFFF, 0);
				}

				m_pCardSlots[i].Draw(pGraphic);

				sCards* pCards = m_pCardSlots[i].GetCards();
				if (pCards != NULL) {
					if (m_pFont != NULL) {
						WRect rect = m_pCardSlots[i].GetPosition();
						m_pFont->Print(g_view, rect.x + 42.0f, rect.y + 50.0f, MakeStr("%d", pCards->m_count), 0, 0xFFFFFFFF, NULL);
					}
				}
			}
		}
	}
}

// ccardbook.cpp:839
void CCardPage::BuildCardList(void) {
	PageSeparate(m_pCardList1, 1);
	PageSeparate(m_pCardList2, 2);
	PageSeparate(m_pCardList3, 3);
	PageSeparate(m_pCardList4, 4);
}

// ccardbook.cpp:849
std::vector<unsigned long>* CCardPage::GetCardList(unsigned char type) {
	std::vector<unsigned long>* pList = NULL;

	switch (type) {
	case 1: pList = m_pCardList1; break;
	case 2: pList = m_pCardList2; break;
	case 3: pList = m_pCardList3; break;
	case 4: pList = m_pCardList4; break;
	}

	return pList;
}

// ccardbook.cpp:865
unsigned long CCardPage::GetPageCount(unsigned char type) {
	unsigned long count = 0;
	unsigned long pageCount;

	for (std::map<unsigned int, IFF_STRUCT::sCard>::const_iterator it = ItemManager()->m_cardMap.begin(); it != ItemManager()->m_cardMap.end(); it++) {
		IFF_STRUCT::sCard card = it->second;
		IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(it->first);

		if (pCard != NULL) {
			if (card.m_type == type && (card.m_typeId & 0x3C00000) < 0xC00000) {
				count++;
			}
		}
	}

	pageCount = count / 12;
	if (count % 12 != 0) {
		pageCount++;
	}

	return pageCount;
}

// ccardbook.cpp:893
void CCardPage::SetCard(unsigned char slot, unsigned long typeId) {
	if (m_pCardSlots != NULL) {
		m_pCardSlots[slot].SetCard(typeId);
	}
}

// ccardbook.cpp:903
void CCardPage::ShowSlotControl(bool show) {
	for (int i = 0; i < 12; i++) {
		if (m_pCardSlots != NULL) {
			m_pCardSlots[i].Show(show);
		}
	}
}

// ccardbook.cpp:915
void CCardPage::SelectSlot(unsigned char control, unsigned char slot) {
	if (m_pCardSlots == NULL) {
		return;
	}

	bool useCard = false;

	if (control == 2) {
		control = 1;
		useCard = true;
	}

	for (int i = 0; i < 12; i++) {
		if (i == slot) {
			if (control != 1) {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(control);
				}
			} else {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(1);
				}
			}
		} else {
			if (control != 1) {
				if (m_pCardSlots[i].IsPushed() != 1) {
					m_pCardSlots[i].Select(control);
				}
			} else {
				m_pCardSlots[i].Select(0);
			}
		}
	}

	if (useCard == true) {
		if (m_pCardSlots != NULL) {
			if (m_pCardSlots[slot].GetCards()->m_typeId != 0x7C800026) {
				m_pCardSlots[slot].UseCard();
			}
		}
	}
}

// ccardbook.cpp:984
CCardBook::CCardBook(void) {
	ClearVariables();
	LoadImages();

	m_pCardPackPage = new CCardPackPage;
	m_pCardPage = new CCardPage;

	m_pCardPackPage->Init();
	m_pCardPage->Init();
}

// ccardbook.cpp:996
CCardBook::~CCardBook(void) {
	for (int i = 0; i < 2; i++) {
		if (m_pPages[i] != NULL) {
			delete m_pPages[i];
			m_pPages[i] = NULL;
		}
	}
}

// ccardbook.cpp:1012
void CCardBook::ClearVariables(void) {
	int i;

	m_show = false;
	m_showProperty = false;

	m_tab = 0;

	m_pageCount = 0;
	m_curPage = 0;
	m_hoverPage = 0;
	m_selectedSlot = 0;

	m_reservedList.clear();
	m_pCurCardList = NULL;

	m_pageIndexRects.clear();

	m_pBookWnd = NULL;
	m_pPropertyWnd = NULL;
	m_pSubWnd = NULL;

	for (i = 0; i < 5; i++) {
		m_pTabButtons[i] = NULL;
	}

	m_pPageWnd = NULL;

	for (i = 0; i < 4; i++) {
		m_pBackGround[i] = NULL;
		m_pCardPack[i] = NULL;
		m_pCollection[i] = NULL;
	}

	for (i = 0; i < 2; i++) {
		m_pPages[i] = NULL;
	}
}

// ccardbook.cpp:1054
void CCardBook::LoadImages(void) {
	m_pBackGround[0] = g_pFresh->GetBitmap("cardbook_bg1");
	m_pBackGround[1] = g_pFresh->GetBitmap("cardbook_bg2");
	m_pBackGround[2] = g_pFresh->GetBitmap("cardbook_bg3");
	m_pBackGround[3] = g_pFresh->GetBitmap("cardbook_bg4");

	m_pCardPack[0] = g_pFresh->GetBitmap("cardpack1");
	m_pCardPack[1] = g_pFresh->GetBitmap("cardpack2");
	m_pCardPack[2] = g_pFresh->GetBitmap("cardpack3");
	m_pCardPack[3] = g_pFresh->GetBitmap("cardpack4");

	m_pCollection[0] = g_pFresh->GetBitmap("collection1");
	m_pCollection[1] = g_pFresh->GetBitmap("collection2");
	m_pCollection[2] = g_pFresh->GetBitmap("collection3");
	m_pCollection[3] = g_pFresh->GetBitmap("collection4");
}

// ccardbook.cpp:1076
void CCardBook::ShowCardProperty(bool show) {
	m_showProperty = show;
}

// ccardbook.cpp:1082
void CCardBook::DrawBackGround(FrGraphicInterface* pGraphic) {
	if (m_pBookWnd != NULL) {
		if (m_pBookWnd->IsVisible()) {
			WPoint pts[4];

			pts[0] = WPoint(379.0f, 147.0f);
			pts[1] = WPoint(635.0f, 147.0f);
			pts[2] = WPoint(379.0f, 403.0f);
			pts[3] = WPoint(635.0f, 403.0f);

			for (int i = 0; i < 4; i++) {
				pGraphic->DrawTexture(m_pBackGround[i], WRect(pts[i], m_pBackGround[i]->Width(), m_pBackGround[i]->Height()), 0xFFFFFFFF, 0);
			}
		}
	}
}

// ccardbook.cpp:1109
void CCardBook::DrawCard(FrGraphicInterface* pGraphic) {
	if (m_pCardPage != NULL) {
		WPoint pts[4];

		pts[0] = WPoint(412.0f, 197.0f);
		pts[1] = WPoint(668.0f, 197.0f);
		pts[2] = WPoint(412.0f, 453.0f);
		pts[3] = WPoint(668.0f, 453.0f);

		for (int i = 0; i < 4; i++) {
			pGraphic->DrawTexture(m_pCollection[i], WRect(pts[i], m_pCollection[i]->Width(), m_pCollection[i]->Height()), 0xFFFFFFFF, 0);
		}

		CCardSlot* pSlots = m_pCardPage->GetCardSlot();
		if (pSlots != NULL) {
			// The cards of the current tab, one vector per page.
			std::vector<unsigned long>* pPages = m_pCardPage->GetCardList(m_tab);
			unsigned int total = 0;

			if (pPages != NULL) {
				unsigned long pageCount = m_pCardPage->GetPageCount(m_tab);

				for (unsigned long page = 0; page < pageCount; page++) {
					total += pPages[page].size();
				}
			}

			for (int i = 0; i < 12; i++) {
				unsigned int number = m_curPage * 12 + i + 1;

				if (number > total) {
					break;
				}

				WRect rect = pSlots[i].GetPosition();
				pGraphic->SetTextColor(0xFF887966, 0xFFFFFFFF);
				pGraphic->SetTextStyle(1);
				pGraphic->Print(WPoint(rect.x - 2.0f, rect.y + 1.0f), 2, "%d", number);
				pGraphic->SetTextColor(0xFF000000, 0xFFFFFFFF);
				pGraphic->SetTextStyle(0);
			}
		}

		m_pCardPage->Draw(pGraphic);

		DrawPageIndex(pGraphic);
	}
}

// ccardbook.cpp:1173
void CCardBook::DrawCardPack(FrGraphicInterface* pGraphic) {
	if (m_pCardPackPage != NULL) {
		WPoint pts[4];

		pts[0] = WPoint(412.0f, 197.0f);
		pts[1] = WPoint(668.0f, 197.0f);
		pts[2] = WPoint(412.0f, 453.0f);
		pts[3] = WPoint(668.0f, 453.0f);

		for (int i = 0; i < 4; i++) {
			pGraphic->DrawTexture(m_pCardPack[i], WRect(pts[i], m_pCardPack[i]->Width(), m_pCardPack[i]->Height()), 0xFFFFFFFF, 0);
		}

		m_pCardPackPage->Draw(pGraphic);
	}
}

// ccardbook.cpp:1194
void CCardBook::DrawProperty(FrGraphicInterface* pGraphic) {
}

// ccardbook.cpp:1209
void CCardBook::DrawPageIndex(FrGraphicInterface* pGraphic) {
	if (!m_pageIndexRects.empty()) {
		for (unsigned int i = 0; i < m_pageIndexRects.size(); i++) {
			WRect rect = m_pageIndexRects[i];

			if (i == m_curPage) {
				pGraphic->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
				pGraphic->SetTextStyle(1);
			}
			else if (i == m_hoverPage) {
				pGraphic->SetTextColor(0xFFAA1111, 0xFFFFFFFF);
				pGraphic->SetTextStyle(1);
			}
			else {
				pGraphic->SetTextColor(0xFF000000, 0xFFFFFFFF);
				pGraphic->SetTextStyle(0);
			}

			pGraphic->Print(WPoint((rect.w - rect.x) * 0.5f + rect.x, rect.y), 1, "%d", i + 1);

			pGraphic->SetTextColor(0xFF000000, 0xFFFFFFFF);
			pGraphic->SetTextStyle(0);

			if (i + 1 < m_pageIndexRects.size()) {
				pGraphic->Print(WPoint(rect.w, rect.y), 1, "|");
			}
		}
	}
}

// ccardbook.cpp:1250
void CCardBook::Show(bool show) {
	m_show = show;

	if (m_pBookWnd != NULL) {
		m_pBookWnd->SetVisible(show);

		for (int i = 0; i < 3; i++) {
			if (m_pTabButtons[i] != NULL) {
				m_pTabButtons[i]->SetVisible(m_show);
			}
		}

		if (m_show == true) {
			ButtonTabControl(0);
		}
	}

	m_showProperty = show;
	if (m_pPropertyWnd != NULL) {
		m_pPropertyWnd->SetVisible(show);
	}

	ShowSlotControl(m_show);

	if (!show) {
		m_pPageWnd->SetVisible(false);
	}

	if (m_pSubWnd != NULL) {
		m_pSubWnd->SetVisible(show);
	}
}

// ccardbook.cpp:1293
void CCardBook::Draw(void) {
	if (m_show) {
		FrGraphicInterface* pGraphic = g_pFresh->GetManager()->GetGDI();

		if (pGraphic != NULL) {
			DrawBackGround(pGraphic);

			if (m_tab) {
				DrawCard(pGraphic);
			}
		}
	}
}

// ccardbook.cpp:1316
void CCardBook::ReBuildCurrentPage(void) {
	ICardBookPage* pPage;

	if (!m_tab) {
		BuildCardPackTab();
		pPage = m_pCardPackPage;
	}
	else {
		BuildCardPage(m_curPage);
		pPage = m_pCardPage;
	}

	if (pPage != NULL && m_pPageWnd != NULL) {
		m_pPageWnd->SetVisible(false);
	}
}

// ccardbook.cpp:1338
void CCardBook::BuildCardPackTab(void) {
	CCardPackPage* pPage = m_pCardPackPage;
	if (pPage != NULL) {
		std::vector<unsigned long>* pList = pPage->GetCardPackList();
		if (pList != NULL) {
			int slot = 0;
			for (std::vector<unsigned long>::iterator it = pList->begin(); it != pList->end(); ++it) {
				unsigned long typeId = *it;

				std::list<sCards>::iterator card;
				for (card = CSharedDoc::Instance()->m_myCardList.begin(); card != CSharedDoc::Instance()->m_myCardList.end(); ++card) {
					if (card->m_typeId == typeId) {
						pPage->SetCard(slot, card->m_typeId);
						break;
					}

					pPage->SetCard(slot, 0);
				}

				slot++;
				if (slot == 8) {
					break;
				}
			}
		}

		if (m_pPageWnd != NULL) {
			m_pPageWnd->SetVisible(false);
		}
	}
}

// ccardbook.cpp:1386
void CCardBook::BuildCardTab(unsigned long tab) {
	if (tab != 0) {
		CCardPage* pCardPage = m_pCardPage;

		m_pageIndexRects.clear();

		if (pCardPage != NULL) {
			pCardPage->SetCardType((unsigned char)tab);
			m_pCurCardList = pCardPage->GetCardList((unsigned char)tab);

			if (m_pCurCardList != NULL) {
				unsigned long pageCount = pCardPage->GetPageCount((unsigned char)tab);

				m_pageCount = (unsigned char)pageCount;
				m_curPage = 0;
				BuildCardPage(0);

				float totalWidth = pageCount * 20.0f;
				float startX = 586.0f - totalWidth * 0.5f;

				for (unsigned long i = 0; i < pageCount; i++) {
					WRect rect;
					rect = WRect(startX + i * 20.0f, 492.0f, startX + i * 20.0f + 20.0f, 502.0f);

					m_pageIndexRects.push_back(rect);
				}
			}
			else {
				BuildCardPage(255);
			}

			if (m_pPageWnd != NULL) {
				m_pPageWnd->SetVisible(false);
			}
		}
	}
}

// ccardbook.cpp:1444
void CCardBook::BuildCardPage(unsigned long page) {
	if (page != 0xFF) {
		if (m_pCurCardList == NULL) {
			return;
		}

		CCardPage* pPage = m_pCardPage;
		if (pPage->GetPageCount(m_tab) - 1 < page) {
			return;
		}

		m_curPage = (unsigned char)page;
		if (pPage != NULL) {
			int slot = 0;
			pPage->SetCurPage(m_curPage);

			for (int i = 0; i < 12; i++) {
				pPage->SetCard(i, 0);
			}

			for (std::vector<unsigned long>::iterator it = m_pCurCardList[page].begin(); it != m_pCurCardList[page].end(); ++it) {
				unsigned long typeId = *it;

				std::list<sCards>::iterator card;
				for (card = CSharedDoc::Instance()->m_myCardList.begin(); card != CSharedDoc::Instance()->m_myCardList.end(); ++card) {
					if (card->m_typeId == typeId) {
						pPage->SetCard(slot, card->m_typeId);
						break;
					}

					pPage->SetCard(slot, 0);
				}

				slot++;
			}
		}
	} else {
		CCardPage* pPage = m_pCardPage;
		if (pPage != NULL) {
			for (int i = 0; i < 12; i++) {
				pPage->SetCard(i, 0);
			}
		}
	}

	if (m_pPageWnd != NULL) {
		m_pPageWnd->SetVisible(false);
	}
}

// ccardbook.cpp:1518
void CCardBook::ShowSlotControl(bool show) {
	CCardPackPage* pCardPackPage = m_pCardPackPage;
	CCardPage* pCardPage = m_pCardPage;

	if (show == true) {
		if (!m_tab) {
			if (pCardPackPage != NULL) {
				pCardPackPage->ShowSlotControl(true);
			}
			if (pCardPage != NULL) {
				pCardPage->ShowSlotControl(false);
			}
		}
		else {
			if (pCardPackPage != NULL) {
				pCardPackPage->ShowSlotControl(false);
			}
			if (pCardPage != NULL) {
				pCardPage->ShowSlotControl(true);
			}
		}
	}
	else {
		if (pCardPackPage != NULL) {
			pCardPackPage->ShowSlotControl(false);
		}
		if (pCardPage != NULL) {
			pCardPage->ShowSlotControl(false);
		}
	}
}

// ccardbook.cpp:1562
void CCardBook::UseCard(void) {
	ICardBookPage* pPage;

	if (!m_tab) {
		pPage = m_pCardPackPage;
	}
	else {
		pPage = m_pCardPage;
	}

	if (pPage != NULL) {
		pPage->SelectSlot(2, m_selectedSlot);
	}
}

// ccardbook.cpp:1583
void CCardBook::ButtonTabControl(unsigned char tab) {
	m_tab = tab;

	if (tab != 0) {
		BuildCardTab(tab);
	}
	else {
		BuildCardPackTab();
	}

	for (int i = 0; i < 3; i++) {
		if (m_pTabButtons[i] == NULL) {
			break;
		}

		if (i == 0) {
			if (tab == 0) {
				m_pTabButtons[0]->SetButtonImg("tab_pack_d", FrButton::NORMAL);
				m_pTabButtons[0]->SetButtonImg("tab_pack_d", FrButton::OVER);
			}
			else {
				m_pTabButtons[0]->SetButtonImg("tab_pack_n", FrButton::NORMAL);
				m_pTabButtons[0]->SetButtonImg("tab_pack_o", FrButton::OVER);
			}
		}
		else if (i == tab) {
			m_pTabButtons[i]->SetButtonImg(MakeStr("tab_vol%d_d", i), FrButton::NORMAL);
			m_pTabButtons[i]->SetButtonImg(MakeStr("tab_vol%d_d", i), FrButton::OVER);
		}
		else {
			m_pTabButtons[i]->SetButtonImg(MakeStr("tab_vol%d_n", i), FrButton::NORMAL);
			m_pTabButtons[i]->SetButtonImg(MakeStr("tab_vol%d_o", i), FrButton::OVER);
		}
	}
}

// ccardbook.cpp:1629
void CCardBook::CardSlotControl(unsigned char page, unsigned char slot, unsigned char control) {
	if (m_pPages[page] != NULL) {
		m_pPages[page]->SelectSlot(control, slot);
	}
}

// ccardbook.cpp:1638
void CCardBook::PageIndexBehavior(const WPoint& pt, unsigned char type) {
	unsigned char index = 0;

	if (!m_pageIndexRects.empty()) {
		for (std::vector<WRect>::iterator it = m_pageIndexRects.begin(); it != m_pageIndexRects.end(); ++it) {
			if (it->x <= pt.x && it->y <= pt.y && it->w >= pt.x && it->h >= pt.y) {
				switch (type) {
				case 1:
					m_curPage = index;
					BuildCardPage(index);
					return;
				case 2:
					break;
				default:
					return;
				}

				break;
			}

			index++;
		}

		m_hoverPage = index;
	}
}

// ccardbook.cpp:1684
void CCardBook::DrawCardPackHelp(unsigned long typeId) {
	FrGraphicInterface* pGraphic = g_pFresh->GetManager()->GetGDI();

	// The two halves of the help frame.
	const Bitmap* pBitmap = g_pFresh->GetBitmap("help_1");
	if (pBitmap != NULL) {
		pGraphic->DrawTexture(pBitmap, WRect(12.0f, 439.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pBitmap = g_pFresh->GetBitmap("help_2");
	if (pBitmap != NULL) {
		pGraphic->DrawTexture(pBitmap, WRect(268.0f, 439.0f, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);
	}

	pGraphic->SetTextColor(0xFF887966, 0xFFFFFFFF);
	IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(typeId);
	IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(typeId);

	if (pCard != NULL) {
		pGraphic->Print(WPoint(120.0f, 458.0f), 0, pCard->m_name);
	}

	if (pDesc != NULL) {
		// The description, wrapped into three lines of 39 bytes (a
		// double-byte character is never split).
		unsigned int length = strlen(pDesc->m_text);
		unsigned int col = 0;
		unsigned int line = 0;

		char text[0x200] = { 0 };
		strcpy(text, pDesc->m_text);

		char lines[3][0x80] = { 0 };
		for (unsigned int i = 0; i < length; i++) {
			lines[line][col++] = text[i];
			if (col > 38 && length > i + 1) {
				if ((unsigned char)text[i] >= 0x81 && (unsigned char)text[i] <= 0xFE) {
					lines[line][col++] = text[++i];
				}

				lines[line][col] = 0;
				line++;
				col = 0;
			}

			if (line >= 3) {
				break;
			}
		}

		pGraphic->Print(WPoint(120.0f, 474.0f), 0, lines[0]);
		pGraphic->Print(WPoint(72.0f, 486.0f), 0, lines[1]);
		pGraphic->Print(WPoint(72.0f, 498.0f), 0, lines[2]);
	}

	pGraphic->SetTextColor(0xFF000000, 0xFFFFFFFF);
}
