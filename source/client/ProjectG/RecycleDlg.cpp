// Original: d:\build\custom\temp\code\client\projectg\recycledlg.cpp
#include "NetworkSystem.h"
#include "Packet.h"
#include "RecycleDlg.h"
#include "recycletable.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "frarea.h"
#include "frbutton.h"
#include "fredit.h"
#include "fresh.h"
#include "frlistbox.h"
#include "frtextbutton.h"
#include "frviewer.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "Localize.h"
#include "SimpleOverlay.h"
#include "exhibition.h"
#include "wresrcmng.h"
#include "wfont.h"
#include "wview.h"
#include "actor.h"
#include "TaskManager.h"
#include <string.h>
#include <string>

// recycledlg.h: no line record (empty constructor of a nested helper class)
FrRecycleDlg::MiniBtn::MiniBtn(void) {
}

// The caddie's greeting of the recycle ("magic box") dialog: the recipe
// hint, and the Valentine's Day event variant (no symbols).
// recycledlg.cpp:21
static std::string data_af75e4("\\cN\\c\xc4\xab\xb5\xf0\xbf\xa1\xc0\xc7 \xb8\xb6\xb9\xfd\xbb\xf3\xc0\xda\xb7\xce \xbf\xc0\xbd\xc5\xb0\xc9 \xc8\xaf\xbf\xb5\xc7\xd5\xb4\xcf\xb4\xd9.\n"
	"\xbe\xc6\xb7\xa1\xbf\xa1\xb4\xc2 \xc7\xd8\xb4\xe7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb8\xb8\xb5\xe9 \xbc\xf6 \xc0\xd6\xb4\xc2 \xb7\xb9\xbd\xc3\xc7\xc7\xb0\xa1 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.\n"
	"\xb7\xb9\xbd\xc3\xc7\xc7\xb8\xa6 \xb5\xfb\xb6\xf3\xbc\xad \xc7\xd8\xb4\xe7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb8\xb8\xb5\xe9\xbe\xee \xba\xb8\xbc\xbc\xbf\xe4.\\cN\\c");
// recycledlg.cpp:22
static std::string data_af7600("\\cN\\c\xc4\xab\xb5\xf0\xbf\xa1\xc0\xc7 \xb8\xb6\xb9\xfd\xbb\xf3\xc0\xda\xb7\xce \xbf\xc0\xbd\xc5\xb0\xc9 \xc8\xaf\xbf\xb5\xc7\xd5\xb4\xcf\xb4\xd9.\n"
	"\xb9\xdf\xb7\xbb\xc5\xb8\xc0\xce\xb5\xa5\xc0\xcc \xc0\xcc\xba\xa5\xc6\xae\xb0\xa1 \xc1\xf8\xc7\xe0\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9.\n"
	"\xc0\xcc\xba\xa5\xc6\xae \xb1\xe2\xb0\xa3 \xb5\xbf\xbe\xc8\xc0\xba \\c0xffff0000\\c\xb3\xb2\xbc\xba \xc0\xaf\xc0\xfa\\c0xff000000\\c\xb8\xb8 \xc1\xb6\xc7\xd5\xc0\xcc \xb0\xa1\xb4\xc9\xc7\xd5\xb4\xcf\xb4\xd9.\\cN\\c");

// recycledlg.cpp:24 (run-time type and factory registration of FrRecycleDlg)
const WRTTI FrRecycleDlg::m_RTTI("FrRecycleDlg", &FrForm::m_RTTI);

IObject* FrRecycleDlgMakeInstance(void) {
	return new FrRecycleDlg;
}

struct __sFrRecycleDlg {
	__sFrRecycleDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRecycleDlgMakeInstance, "FrRecycleDlg");
	}
};
static __sFrRecycleDlg s_registerFrRecycleDlg;

// recycledlg.cpp:26
const sFRESH_MSGMAP* FrRecycleDlg::GetMessageMap(void) const {
	return &FrRecycleDlg::_MsgMap;
}

// The title font's texture (data_a9b804, one entry).
static char* s_titleFontFile = "[font_wind.jpg";

// recycledlg.cpp:78
FrRecycleDlg::FrRecycleDlg(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	m_pCloseBn = NULL; m_pLevelLowBn = NULL; m_pLevelMidBn = NULL; m_pLevelHighBn = NULL;
	m_pLevelSpecialBn = NULL; m_pLevelEventBn = NULL;
	m_pCharacterSelectList = NULL; m_pRecycleItemList = NULL;
	m_pRecycleNumber = NULL;
	m_pMixBn = NULL; m_pArrowMinimumBn = NULL; m_pArrowLeftBn = NULL; m_pArrowRightBn = NULL; m_pArrowMaximumBn = NULL;
	m_pRecycleItemName = NULL; m_pCadieMsg = NULL;
	info.filename = &s_titleFontFile;
	info.numPages = 1;
	info.fontw = 16;
	info.fonth = 16;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	m_pItemBnNormal = g_pFresh->GetManager()->GetBitmap("RECYCLEDLG", "recycle_item_bn_normal");
	m_pItemBnClick = g_pFresh->GetManager()->GetBitmap("RECYCLEDLG", "recycle_item_bn_click");

	m_pArrowUI = NULL;
	m_listLevel = -1;
	memset(m_characterTid, 0, sizeof(m_characterTid));
	m_selectedIdx = -1;
	m_characterIdx = 0;
	memset(&m_resultItem, 0, sizeof(m_resultItem));
	memset(m_needItem, 0, sizeof(m_needItem));
	memset(m_haveItem, 0, sizeof(m_haveItem));
	m_recycleNumber = 0;
	m_maxEnableRecycle = 0;

	memset(&m_mixedItem, 0, sizeof(m_mixedItem));
	m_pExhibition = NULL;
	m_arrowLeftDown = false;
	m_arrowRightDown = false;
	m_arrowHoldTime = 0;
	m_mixing = false;
	m_arrowState[0] = 0;
	m_arrowState[1] = 0;
	m_arrowBlinkTime[0] = 0;
	m_arrowBlinkTime[1] = 0;
	m_mixSendDelay = 0;
	m_mixResultTime = 0;
	m_cadieMsgTime = 0;
	m_selectedTid = 0;
	m_selectedPart = 0;
	m_partSelected = false;
	// The original also clears 0x545 (byte), 0x548 and 0x54C here: no use
	// of them is known yet, so they stay unnamed.
	m_pSelectPartBn = NULL;
	m_pSelectPartDlg = NULL;
	InitDataCharacterSelect();
	InitIndexRecycleItem();

	if (IsLocalContent((localContentType_t)0x59)) {

		WSendPacket packet((enumClientPacket)0xB9);
		packet.Encode1(2);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// recycledlg.cpp:168
FrRecycleDlg::~FrRecycleDlg(void) {
	if (IsLocalContent((localContentType_t)0x59)) {

		if (WSingleton<WNetworkSystem>::Instance()) {

			WSendPacket packet((enumClientPacket)0xB9);
			packet.Encode1(1);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	if (m_pArrowUI) { delete m_pArrowUI; m_pArrowUI = 0; }
	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// recycledlg.cpp:186
void FrRecycleDlg::OnRecycleDlgInit(int) {
}

// recycledlg.cpp:191
void FrRecycleDlg::OnAboutBnInit(int param) {
	DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:197
void FrRecycleDlg::OnAboutBnUp(void) {
	FrRecycleAboutDlg* pDlg = CreateForm<FrRecycleAboutDlg>(g_pFresh->GetManager(), this, "recycle_about", 0);

	if (pDlg) {

		pDlg->Open((FRESH_RESULT_CALLBACK)0, 3);
	}
}

// recycledlg.cpp:208
void FrRecycleDlg::OnCloseBnInit(intptr_t param) {
	m_pCloseBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:214
void FrRecycleDlg::OnCloseBnUp(void) {
	OnFreshOkay();
}

// recycledlg.cpp:220
void FrRecycleDlg::OnLevelLowBnInit(int param) {
	m_pLevelLowBn = DYNAMIC_CAST(FrButton, param);

	if (m_pLevelLowBn) {

		m_pLevelLowBn->SetStatus(FrButton::PRESSED);
	}
}

// recycledlg.cpp:231
void FrRecycleDlg::OnLevelLowBnUp(void) {


	SetListRecycleItem(m_level = 0, 0);

	if (m_pLevelLowBn) m_pLevelLowBn->SetStatus(FrButton::PRESSED);
	if (m_pLevelMidBn) m_pLevelMidBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelHighBn) m_pLevelHighBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelEventBn) m_pLevelEventBn->SetStatus(FrButton::NORMAL);
	if (m_pCharacterSelectList) m_pCharacterSelectList->SetVisible(false);
}

// recycledlg.cpp:246
void FrRecycleDlg::OnLevelMidBnInit(int param) {
	m_pLevelMidBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:252
void FrRecycleDlg::OnLevelMidBnUp(void) {


	SetListRecycleItem(m_level = 1, 0);

	if (m_pLevelLowBn) m_pLevelLowBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelMidBn) m_pLevelMidBn->SetStatus(FrButton::PRESSED);
	if (m_pLevelHighBn) m_pLevelHighBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelEventBn) m_pLevelEventBn->SetStatus(FrButton::NORMAL);
	if (m_pCharacterSelectList) m_pCharacterSelectList->SetVisible(false);
}

// recycledlg.cpp:267
void FrRecycleDlg::OnLevelHighBnInit(int param) {
	m_pLevelHighBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:273
void FrRecycleDlg::OnLevelHighBnUp(void) {


	SetListRecycleItem(m_level = 2, 0);

	if (m_pLevelLowBn) m_pLevelLowBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelMidBn) m_pLevelMidBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelHighBn) m_pLevelHighBn->SetStatus(FrButton::PRESSED);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelEventBn) m_pLevelEventBn->SetStatus(FrButton::NORMAL);
	if (m_pCharacterSelectList) m_pCharacterSelectList->SetVisible(false);
}

// recycledlg.cpp:288
void FrRecycleDlg::OnLevelSpecialBnInit(int param) {
	m_pLevelSpecialBn = DYNAMIC_CAST(FrButton, param);

	if (m_pLevelSpecialBn) {

		if (IsLocalContent((localContentType_t)0x63)) {
			m_pLevelSpecialBn->SetVisible(true);
		} else {
			m_pLevelSpecialBn->SetVisible(false);
		}
	}
}

// recycledlg.cpp:302
void FrRecycleDlg::OnLevelSpecialBnUp(void) {


	SetListRecycleItem(m_level = 3, m_characterIdx);

	if (m_pLevelLowBn) m_pLevelLowBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelMidBn) m_pLevelMidBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelHighBn) m_pLevelHighBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->SetStatus(FrButton::PRESSED);
	if (m_pLevelEventBn) m_pLevelEventBn->SetStatus(FrButton::NORMAL);
	if (m_pCharacterSelectList) m_pCharacterSelectList->SetVisible(true);
}

// recycledlg.cpp:317
void FrRecycleDlg::OnLevelEventBnInit(int param) {
	m_pLevelEventBn = DYNAMIC_CAST(FrButton, param);

	if (g_NumEventRecycleItem > 0) {

		m_pLevelEventBn->SetVisible(true);
	} else {

		m_pLevelEventBn->SetVisible(false);
	}
}

// recycledlg.cpp:336
void FrRecycleDlg::OnLevelEventBnUp(void) {









	SetListRecycleItem(m_level = 4, m_characterIdx);

	if (m_pLevelLowBn) m_pLevelLowBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelMidBn) m_pLevelMidBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelHighBn) m_pLevelHighBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->SetStatus(FrButton::NORMAL);
	if (m_pLevelEventBn) m_pLevelEventBn->SetStatus(FrButton::PRESSED);



	if (m_pCharacterSelectList) m_pCharacterSelectList->SetVisible(false);
}

// recycledlg.cpp:362
void FrRecycleDlg::OnCharacterSelectListInit(int param) {
	m_pCharacterSelectList = DYNAMIC_CAST(FrListBox, param);

	if (m_pCharacterSelectList) {

		m_pCharacterSelectList->SetVisible(false);
	}
}

// recycledlg.cpp:373
void FrRecycleDlg::OnCharacterSelectListLBnUp(void) {
	FrListItem* pItem = m_pCharacterSelectList->GetSelected();

	if (pItem) {

		m_characterIdx = pItem->idx;

		SetListRecycleItem(m_level, m_characterIdx);
	}
}

// recycledlg.cpp:386
void FrRecycleDlg::OnCharacterSelectListOwnerDraw(int param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {

		unsigned long* pTid = (unsigned long*)pItem->pData;



		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(*pTid);

		if (pCommon) {

			const Bitmap* pBitmap = g_pFresh->GetBitmap(MakeStr("s_%s", pCommon->m_icon));


			if (pBitmap) {

				FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

				if (pGDI) {

					unsigned long color = 0xFF808080;

					if (pItem->selected) {
						color = 0xFFFFFFFF;
					} else if (pItem->underCursor) {
						color = 0xFFC0C0C0;
					}

					pGDI->DrawTexture(pBitmap, WRect(pItem->pos.x + 4.0f, pItem->pos.y + 4.0f, pBitmap->Width() * 0.8f, pBitmap->Height() * 0.8f), color, 0);
				}
			}
		}
	}
}

// recycledlg.cpp:422
void FrRecycleDlg::ReInitializeHaveItem(void) {
	m_haveItemMap.clear();
}

// recycledlg.cpp:427
void FrRecycleDlg::AddHaveItemMap(const sRecycleItem* pItem) {
	if (pItem->level <= CSharedDoc::Instance()->m_myLevel) {

		if (IsHaveItem(pItem)) {

			m_haveItemMap[pItem] = 2;
		} else {

			m_haveItemMap[pItem] = 0;
		}
	} else {

		m_haveItemMap[pItem] = 1;
	}
}

// recycledlg.cpp:447
void FrRecycleDlg::OnRecycleItemListInit(int param) {
	m_pRecycleItemList = DYNAMIC_CAST(FrListBox, param);

	if (m_pRecycleItemList) {

		m_pRecycleItemList->UseRightButton(true);
	}


	m_clockBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "clock");
	m_clockBtn.rect = WRect(50.0f, 35.0f, m_clockBtn.pBitmap->Width(), m_clockBtn.pBitmap->Height());
	m_deleteBtn.pBitmap = g_pFresh->GetManager()->GetBitmap("ICONS", "delete");
	m_deleteBtn.rect = WRect(50.0f, 35.0f, m_deleteBtn.pBitmap->Width(), m_deleteBtn.pBitmap->Height());
}

// recycledlg.cpp:463
void FrRecycleDlg::OnRecycleItemListLBnUp(void) {
	ClickRecycleItem();
}

// recycledlg.cpp:469
void FrRecycleDlg::OnRecycleItemListRBnUp(void) {
	FrListItem* pItem = m_pRecycleItemList->GetSelected();

	if (pItem) {

		sRecycleItem* pRecycleItem = (sRecycleItem*)pItem->pData;

		OpenInformation(pRecycleItem->tid);
	}

	ClickRecycleItem();
}

// recycledlg.cpp:600
void FrRecycleDlg::OnRecycleNumberInit(int param) {
	m_pRecycleNumber = DYNAMIC_CAST(FrEdit, param);

	ControlRecycleNumber(-1);
}

// recycledlg.cpp:608
bool FrRecycleDlg::OnRecycleNumberEnterKey(int) {
	return false;
}

// recycledlg.cpp:614
void FrRecycleDlg::OnMixBnInit(int param) {
	m_pMixBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:620
void FrRecycleDlg::OnMixBnUp(void) {
	if (m_recycleNumber > 0) {

		if (!CSharedDoc::Instance()->ConfirmInsertItem(m_selectedTid, m_recycleNumber)) {

			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
			WRect rect = pForm->GetRect();
			pForm->SetRect(WRect(rect.x, rect.y, rect.w, rect.h));
			pForm->SetTimeLimit(12.0f);
			// "You already own 20,000 of this item: it cannot be mixed."
			pForm->SetMessage("\xc7\xf6\xc0\xe7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xba\xb8\xc0\xaf\xb0\xb3\xbc\xf6\xb0\xa1 20,000\xb0\xb3\xc0\xcc\xb9\xc7\xb7\xce \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc1\xb6\xc7\xd5 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", false);
			pForm->Open((FRESH_RESULT_CALLBACK)0, 0);
			return;
		}

		m_mixing = true;
		EnableTotalControls(false);

		m_mixSendDelay = 1.5f; m_mixResultTime = 0; memset(&m_mixedItem, 0, sizeof(m_mixedItem));

		if (m_pExhibition) {

			char motion[32] = "";

			switch (rand() % 2) {
			case 0:
				strcpy(motion, "\xc8\xa6\xc0\xce");
				break;
			case 1:
				strcpy(motion, "\xc8\xa6\xc0\xce\xbf\xf8");
				break;
			}

			m_pExhibition->SetMotion(motion, false, 0);
		}
	}
}

// recycledlg.cpp:656
void FrRecycleDlg::OnArrowMinimumBnInit(int param) {
	m_pArrowMinimumBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:662
void FrRecycleDlg::OnArrowMinimumBnDown(void) {
	ControlRecycleNumber(0);
}

// recycledlg.cpp:668
void FrRecycleDlg::OnArrowLeftBnInit(int param) {
	m_pArrowLeftBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:674
void FrRecycleDlg::OnArrowLeftBnDown(void) {


	if ((m_haveItem[0].tid & 0xFC000000) == 0x08000000 && m_haveItem[0].count > 1) {

		if (!m_partSelected) {
			return;
		}

		if (!m_pSelectPartBn->IsVisible()) {

			m_pSelectPartBn->SetVisible(true);
		}
	}

	m_arrowLeftDown = true; m_arrowHoldTime = 0.0f; ControlRecycleNumber(1);
}

// recycledlg.cpp:704
void FrRecycleDlg::OnArrowLeftBnUp(void) {
	m_arrowLeftDown = false;

	if (m_pCardSlot) {

		m_pCardSlot->SetVisible(true);
	}
}

// recycledlg.cpp:715
void FrRecycleDlg::OnArrowRightBnInit(int param) {
	m_pArrowRightBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:721
void FrRecycleDlg::OnArrowRightBnDown(void) {


	if ((m_haveItem[0].tid & 0xFC000000) == 0x08000000 && m_haveItem[0].count > 1) {

		if (!m_partSelected) {
			return;
		}

		if (m_pSelectPartBn->IsVisible()) {

			m_pSelectPartBn->SetVisible(false);
		}
	}

	m_arrowRightDown = true; m_arrowHoldTime = 0.0f; ControlRecycleNumber(2);
}

// recycledlg.cpp:751
void FrRecycleDlg::OnArrowRightBnUp(void) {
	m_arrowRightDown = false;

	if (m_pCardSlot) {

		m_pCardSlot->SetVisible(false);
	}
}

// recycledlg.cpp:762
void FrRecycleDlg::OnArrowMaximumBnInit(int param) {
	m_pArrowMaximumBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:768
void FrRecycleDlg::OnArrowMaximumBnDown(void) {
	ControlRecycleNumber(3);
}

// recycledlg.cpp:774
void FrRecycleDlg::OnAreaPictureInit(int param) {
	DYNAMIC_CAST(FrArea, param);
}

// The picture area's height: nameless data of another unit (455.25).
extern float data_00a9b800;

// recycledlg.cpp:780
void FrRecycleDlg::OnAreaPictureOwnerDraw(void) {
	DrawRecycleItem();
	DrawArrow();


	if (m_pExhibition) {


		m_pExhibition->DisplayClip(WRect(GetRect().x, GetRect().y, 607.0f, data_00a9b800), 55.0f, 0.0f, 0.0f);
	}
}

// recycledlg.cpp:794
void FrRecycleDlg::OnRecycleItemNameInit(int param) {
	m_pRecycleItemName = DYNAMIC_CAST(FrEdit, param);
}

// recycledlg.cpp:800
void FrRecycleDlg::OnCadieMessageInit(int param) {
	m_pCadieMsg = DYNAMIC_CAST(FrEdit, param);

	if (m_pCadieMsg) {

		SetCadieMessage(data_af75e4.c_str());
	}
}

// recycledlg.cpp:811
bool FrRecycleDlg::OnInit(void) {
	EnableDrag(false);
	InitPositionItemData();
	SetListRecycleItem(0, 0);
	InitListCharacterSelect();
	EnableMixControls(false);

	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }

	float distance = g_view->GetWidth() * 0.03375f;
	WRect rect = GetRect();

	float aspect = g_view->GetHeight() / g_view->GetWidth();
	m_pExhibition = new CExhibition(rect.x + 438.0f, rect.y + 173.0f,
		(g_view->GetWidth() * 0.0015625f - (1.0f / aspect - 1.3333334f) * 0.8f) / g_view->GetHeight() * 99.0000076f, true);

	m_pExhibition->SetDistanceRange(distance - 5.0f, distance + 5.0f);
	m_pExhibition->SetDistance(distance, true, false);
	m_pExhibition->SetArea(164.0f, 346.0f);
	m_pExhibition->SetModel("shop_cadie", (eExhibitType)1);

	m_pArrowUI = new CSimpleUI("keys.png");
	m_pArrowUI->Add("RIGHT_S", WRect(40.0f, 0.0f, 20.0f, 20.0f), false);
	m_pArrowUI->Add("RIGHT", WRect(0.0f, 40.0f, 20.0f, 20.0f), false);
	m_pArrowUI->Add("INVALID", WRect(40.0f, 40.0f, 20.0f, 20.0f), false);

	CTaskManager* pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0xDF, 0, 0, 0, 0, 0));

	return true;
}

// recycledlg.cpp:850
void FrRecycleDlg::OnProc(const float dt) {
	if (m_arrowLeftDown) {

		m_arrowHoldTime += dt;

		if (m_arrowHoldTime > 0.2f) {
			m_arrowHoldTime = 0.0f;
			ControlRecycleNumber(1);
		}
	} else if (m_arrowRightDown) {

		m_arrowHoldTime += dt;

		if (m_arrowHoldTime > 0.2f) {
			m_arrowHoldTime = 0.0f;
			ControlRecycleNumber(2);
		}
	}

	if (m_pExhibition) {

		m_pExhibition->ProcessBongdariShop(dt);
	}

	if (m_maxEnableRecycle > 0 && m_selectedIdx >= 0) {

		if (!m_mixing) {

			m_arrowBlinkTime[0] += dt;
			m_arrowState[1] = 0;

			if (m_arrowBlinkTime[0] > 0.5f) {
				m_arrowBlinkTime[0] = 0.0f;
				m_arrowState[0] = m_arrowState[0] == 0;
			}
		} else {

			m_arrowBlinkTime[1] += dt;
			m_arrowState[0] = 1;

			if (m_arrowBlinkTime[1] > 0.5f) {
				m_arrowBlinkTime[1] = 0.0f;
				m_arrowState[1] = m_arrowState[1] == 0;
			}
		}
	} else {

		m_arrowState[0] = -1;
		m_arrowState[1] = -1;
	}

	if (m_mixSendDelay > 0.0f) {

		m_mixSendDelay -= dt;

		if (m_mixSendDelay <= 0.0f) {
			SendDataRecycleItem(m_selectedIdx, m_recycleNumber);
		}
	}

	if (m_mixResultTime > 0.0f) {

		m_mixResultTime -= dt;

		if (m_mixResultTime <= 0.0f) {
			memset(&m_mixedItem, 0, sizeof(m_mixedItem));
		}
	}

	if (m_cadieMsgTime > 0.0f) {

		m_cadieMsgTime -= dt;

		if (m_cadieMsgTime <= 0.0f) {
			const char* msg = data_af75e4.c_str();

			if (m_pCadieMsg) {
				m_pCadieMsg->ClearLine();
				m_pCadieMsg->AddText(msg, false, true);
			}
		}
	}
}

// recycledlg.cpp:951
void FrRecycleDlg::OnCancel(void) {
}

// recycledlg.cpp:958
void FrRecycleDlg::InitPositionItemData(void) {


	m_itemPos[0] = WPoint(GetRect().x + 220.0f, GetRect().y + 200.0f);

	int i;

	for (i = 0; i < 4; i++) m_itemPos[1 + i] = WPoint(GetRect().x + (float)(207 + i * 50), GetRect().y + 280.0f);




	for (i = 0; i < 4; i++) m_itemPos[5 + i] = WPoint(GetRect().x + (float)(26 + i * 63), GetRect().y + 375.0f);


	m_itemPos[9] = WPoint(GetRect().x + 427.0f, GetRect().y + 376.0f);
}

// recycledlg.cpp:978
void FrRecycleDlg::SetListRecycleItem(int level, int character) {
	m_haveItemMap.clear();


	memset(&m_resultItem, 0, sizeof(m_resultItem));
	memset(m_needItem, 0, sizeof(m_needItem));
	memset(m_haveItem, 0, sizeof(m_haveItem));

	if (m_pSelectPartBn) {

		m_pSelectPartBn->SetVisible(false);
	}

	EnableMixControls(false);


	m_maxEnableRecycle = 0; m_pRecycleItemList->ClearItem();

	m_selectedIdx = -1;
	m_listLevel = level;


	int start = 0;
	int end = 0;

	switch (level) {
	case 0:


		start = 0;
		end = g_NumLowRecycleItem;
		break;
	case 1:
		start = g_NumLowRecycleItem;
		end = start + g_NumMidRecycleItem;
		break;
	case 2:
		start = g_NumLowRecycleItem + g_NumMidRecycleItem;
		end = start + g_NumHighRecycleItem;
		break;
	case 3:
		start = g_NumLowRecycleItem + g_NumMidRecycleItem + g_NumHighRecycleItem;
		end = start + g_NumSpecialRecycleItem;
		break;
	case 4:
		start = g_NumLowRecycleItem + g_NumMidRecycleItem + g_NumHighRecycleItem + g_NumSpecialRecycleItem;
		end = start + g_NumEventRecycleItem;
		break;
	}

	if (level == 3 || level == 4) {

		for (int i = start; i < end; i++) {

			if (g_ArrayRecycleItem[i].character == character || g_ArrayRecycleItem[i].character == -1 || g_ArrayRecycleItem[i].character == -2) {

				if (g_ArrayRecycleItem[i].tid) {

					m_pRecycleItemList->AddItem(&g_ArrayRecycleItem[i]);
					AddHaveItemMap(&g_ArrayRecycleItem[i]);
				}
			}
		}
	} else {

		for (int i = start; i < end; i++) {

			m_pRecycleItemList->AddItem(&g_ArrayRecycleItem[i]);
			AddHaveItemMap(&g_ArrayRecycleItem[i]);
		}
	}
}

// recycledlg.cpp:1177
void FrRecycleDlg::InitDataCharacterSelect(void) {
	m_characterTid[0] = 0x4000000;
	m_characterTid[1] = 0x4000001;
	m_characterTid[2] = 0x4000002;
	m_characterTid[3] = 0x4000003;
	m_characterTid[4] = 0x4000004;
	m_characterTid[5] = 0x4000005;
	m_characterTid[6] = 0x4000006;
	m_characterTid[7] = 0x4000007;
	m_characterTid[8] = 0x4000008;
	m_characterTid[9] = 0x4000009;
}

// recycledlg.cpp:1188
void FrRecycleDlg::InitListCharacterSelect(void) {
	std::map<unsigned int, sCharacterInfo>::iterator it = CSharedDoc::Instance()->m_characterMap.find(CSharedDoc::Instance()->m_myCharacterId);

	if (it != CSharedDoc::Instance()->m_characterMap.end()) {

		IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(it->second.typeId);

		if (pCommon) {

			m_characterIdx = pCommon->m_typeId & 0x1FF;
		}
	}

	m_pCharacterSelectList->ClearItem();



	for (int i = 0; i < 10; i++) {

		m_pCharacterSelectList->AddItem(&m_characterTid[i]);

		if (i == m_characterIdx) {

			FrListItem* pItem = m_pCharacterSelectList->GetItem(i);

			m_pCharacterSelectList->SelectItem(pItem, true);
		}
	}
}

// recycledlg.cpp:1589
void FrRecycleDlg::DrawArrow(void) {
	if (m_pArrowUI) {


		std::string arrowName[2];
		for (int i = 0; i < 2; i++) {



			switch (m_arrowState[i]) {

			case 0: arrowName[i] = "RIGHT"; break;
			case 1: arrowName[i] = "RIGHT_S"; break;
			default: arrowName[i] = "INVALID"; break;
			}
		}

		m_pArrowUI->Render(arrowName[0].c_str(), GetRect().x + 282.0f, GetRect().y + 392.0f, 1.0f, 0);
		m_pArrowUI->Render(arrowName[1].c_str(), GetRect().x + 410.0f, GetRect().y + 392.0f, 1.0f, 0);
	}
}

// recycledlg.cpp:1611
void FrRecycleDlg::EnableTotalControls(bool enable) {
	if (m_pCloseBn) m_pCloseBn->Enable(enable);
	if (m_pLevelLowBn) m_pLevelLowBn->Enable(enable);
	if (m_pLevelMidBn) m_pLevelMidBn->Enable(enable);
	if (m_pLevelHighBn) m_pLevelHighBn->Enable(enable);
	if (m_pLevelSpecialBn) m_pLevelSpecialBn->Enable(enable);
	if (m_pLevelEventBn) m_pLevelEventBn->Enable(enable);
	if (m_pCharacterSelectList) m_pCharacterSelectList->Enable(enable);
	if (m_pRecycleItemList) m_pRecycleItemList->Enable(enable);
	if (m_pMixBn) m_pMixBn->Enable(enable);
	if (m_pArrowMinimumBn) m_pArrowMinimumBn->Enable(enable);
	if (m_pArrowLeftBn) m_pArrowLeftBn->Enable(enable);
	if (m_pArrowRightBn) m_pArrowRightBn->Enable(enable);
	if (m_pArrowMaximumBn) m_pArrowMaximumBn->Enable(enable);
}

// recycledlg.cpp:1629
void FrRecycleDlg::EnableMixControls(bool enable) {
	if (m_pMixBn) m_pMixBn->Enable(enable);
	if (m_pArrowMinimumBn) m_pArrowMinimumBn->Enable(enable);
	if (m_pArrowLeftBn) m_pArrowLeftBn->Enable(enable);
	if (m_pArrowRightBn) m_pArrowRightBn->Enable(enable);
	if (m_pArrowMaximumBn) m_pArrowMaximumBn->Enable(enable);
}

// recycledlg.cpp:1638
bool FrRecycleDlg::IsUsableParts(sItemInfo& item) {
	CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
	std::map<unsigned int, sCharacterInfo>::iterator it = pDoc->m_characterMap.begin();

	for (; it != pDoc->m_characterMap.end(); ++it) {
		if (it->second.typeId == (m_characterIdx | 0x4000000)) {
			if (pDoc->IsEquipParts(it->second, item.m_serial, item.m_typeId)) {
				return false;
			}
			break;
		}
	}

	return true;
}

// recycledlg.cpp:1941
void FrRecycleDlg::ControlRecycleNumber(int mode) {
	switch (mode) {
	case 1: m_recycleNumber--; break;
	case 2: m_recycleNumber++; break;
	case 3: m_recycleNumber = m_maxEnableRecycle; break;
	case 0: m_recycleNumber = 0; break;
	default: m_recycleNumber = 0; break;
	}

	if (m_maxEnableRecycle > 0) {

		if (m_recycleNumber >= m_maxEnableRecycle) {
			m_recycleNumber = m_maxEnableRecycle;
		} else if (m_recycleNumber < 0) {
			m_recycleNumber = 0;
		}
	} else {
		m_recycleNumber = 0;
	}

	switch (m_resultItem.tid >> 26) {
	case 5:
	case 6:
	case 0x1C:
		break;
	default:
		if (m_recycleNumber > 1) {
			m_recycleNumber = 1;
		}
		break;
	}

	if (m_resultItem.singleRecycle > 0) {

		if (m_recycleNumber > 1) {
			m_recycleNumber = 1;
		}
	}

	if (m_pRecycleNumber) {






		m_pRecycleNumber->SetLine(1, MakeStr("%d", m_recycleNumber), 0, false, 0);
	}
}

// recycledlg.cpp:1995
void FrRecycleDlg::SetMaxEnableRecycle(int) {
	int maxCount = 0x7FFFFFFF;

	m_maxEnableRecycle = 0;

	for (int i = 0; i < 4; i++) {

		if (m_needItem[i].tid == 0) break;
		if (m_needItem[i].count <= 0) break;

		if (m_haveItem[i].count >= m_needItem[i].count) {

			int count = m_haveItem[i].count / m_needItem[i].count;

			if (count < maxCount) {

				maxCount = count;
			}
		} else {




			maxCount = 0;
			break;
		}
	}

	m_maxEnableRecycle = maxCount;
}

// recycledlg.cpp:2163
void FrRecycleDlg::SendDataRecycleItem(int index, int number) {
	if (index >= 0 && number > 0) {

		char itemCount = 0;
		WSendPacket packet((enumClientPacket)0x7E);
		packet.Encode2(index);
		packet.Encode1(number);
		packet.Encode1(0);
		packet.GetLengthData();


		for (int i = 0; i < 4; i++) {

			if (m_haveItem[i].tid) {

				struct {
					unsigned long tid;      // 0x0
					unsigned long serial;   // 0x4
				} item;
				item.tid = m_haveItem[i].tid;
				item.serial = m_haveItem[i].serial;
				packet.EncodeBuffer(&item, sizeof(item));

				itemCount++;
			}
		}


		packet.EncodeData(3, &itemCount, 1);
		packet.Send(SEND_TO_DEFAULT);
	} else {

		OnFreshOkay();
	}







	unsigned long selectedPart = m_selectedPart; m_partSelected = false; if (m_haveItem[0].count < 3) {

		if (m_pSelectPartBn) {

			ClickRecycleItem();
			m_pSelectPartBn->SetVisible(false);





			for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myPartsList.begin(); it != CSharedDoc::Instance()->m_myPartsList.end(); ++it) {


				if (it->m_serial != selectedPart && it->m_typeId == m_haveItem[0].tid) {

					m_selectedPart = it->m_serial;
					return;
				}
			}
		}
	} else {

		m_selectedPart = 0;
		ClickRecycleItem();
	}
}

// The item ids RandRecycleItem accepts.
namespace {
unsigned long g_randItemList[3] = { 0x1A0000CD, 0x1A0000CE, 0x1A0000CF };
}

// recycledlg.cpp:2240
bool RandRecycleItem(unsigned long tid) {

	for (int i = 0; i < 3; i++) {

		if (tid == g_randItemList[i]) {

			return true;
		}
	}


	return false;
}

// recycledlg.cpp:2370
bool IsTunaClubset(unsigned long tid) {
	if (tid == 0x10000022 || (tid > 0x1000002E && tid <= 0x10000030)) {


		return true;
	}


	return false;
}

// recycledlg.cpp:2454
bool FrRecycleDlg::IsHaveItem(const sRecycleItem* pItem) {
	if (pItem) {


		switch (pItem->tid & 0xFC000000) {
		case 0x18000000:




			if (CheckRandomRecycleItem(pItem)) {

				return true;
			}

			if (IsHaveItem(pItem->tid)) {

				return true;
			}
			break;
		default:
			if (IsHaveItem(pItem->tid)) {

				return true;
			}
			break;
		}
	}

	return false;
}

// recycledlg.cpp:2536
bool FrRecycleDlg::IsHaveItem(unsigned long tid) {
	bool result = false;

	switch (tid >> 26) {
	case 2:
		for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myPartsList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myPartsList.end(); ++it) {
			if (it->m_typeId == tid) {
				IFF_ITEM_COMMON* pCommon = ItemManager()->FindCommonItem(tid);
				if (pCommon->m_flags0 & 1) {
					result = true;
					break;
				}
			}
		}
		break;
	case 4: {
		std::map<unsigned int, sItemInfo>::iterator end = WSingleton<CSharedDoc>::Instance()->m_myItemMap.end();
		for (std::map<unsigned int, sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myItemMap.begin(); it != end; ++it) {
			if (it->second.m_typeId == tid) {
				result = true;
				break;
			}
		}
		break;
	}
	case 7: {
		std::map<unsigned int, sCaddieInfo>::iterator end = WSingleton<CSharedDoc>::Instance()->m_caddieMap.end();
		for (std::map<unsigned int, sCaddieInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.begin(); it != end; ++it) {
			if (it->second.typeId == tid) {
				result = true;
				break;
			}
		}
		break;
	}
	case 14: {
		std::list<sItemInfo>& skins = WSingleton<CSharedDoc>::Instance()->m_mySkinList;
		for (std::list<sItemInfo>::iterator it = skins.begin(); it != skins.end(); ++it) {
			if (it->m_typeId == tid) {
				result = true;
				break;
			}
		}
		break;
	}
	}

	return result;
}

// recycledlg.cpp:2683
void FrRecycleDlg::SetCadieMessage(const char* msg) {
	if (m_pCadieMsg) {

		m_pCadieMsg->ClearLine();
		m_pCadieMsg->AddText(msg, false, true);
	}
}

// recycledlg.cpp:2693
bool FrRecycleDlg::SetEventMessage(int level) {
	bool result = false;

	if (level == 4) {
		if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().eventFlags & 2) {
			SetCadieMessage(data_af7600.c_str());
			m_cadieMsgTime = 5.0f;
			result = true;
		}
	}

	return result;
}

// recycledlg.cpp:2710 (run-time type and factory registration of FrRecycleAboutDlg)
const WRTTI FrRecycleAboutDlg::m_RTTI("FrRecycleAboutDlg", &FrForm::m_RTTI);

IObject* FrRecycleAboutDlgMakeInstance(void) {
	return new FrRecycleAboutDlg;
}

struct __sFrRecycleAboutDlg {
	__sFrRecycleAboutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrRecycleAboutDlgMakeInstance, "FrRecycleAboutDlg");
	}
};
static __sFrRecycleAboutDlg s_registerFrRecycleAboutDlg;

// recycledlg.cpp:2712
const sFRESH_MSGMAP* FrRecycleAboutDlg::GetMessageMap(void) const {
	return &FrRecycleAboutDlg::_MsgMap;
}

// recycledlg.cpp:2736
FrRecycleAboutDlg::FrRecycleAboutDlg(void) {




	for (int i = 0; i < 5; i++) m_pIndexBn[i] = 0;


	m_aboutIndex = 0;
	m_pBackwardBn = 0;
	m_pForwardBn = 0;
}

// recycledlg.cpp:2750
FrRecycleAboutDlg::~FrRecycleAboutDlg(void) {
}

// recycledlg.cpp:2754
void FrRecycleAboutDlg::OnAboutInit(int) {
}

// recycledlg.cpp:2758
void FrRecycleAboutDlg::OnAboutViewerInit(intptr_t param) {
	m_pAboutViewer = DYNAMIC_CAST(FrViewer, param);
}

// recycledlg.cpp:2764
void FrRecycleAboutDlg::OnIndex01Init(intptr_t param) {
	m_pIndexBn[0] = DYNAMIC_CAST(FrTextButton, param);
}

// recycledlg.cpp:2769
void FrRecycleAboutDlg::OnIndex01BnUp(void) {
	SetImgAboutIndex(0);
}

// recycledlg.cpp:2774
void FrRecycleAboutDlg::OnIndex02Init(intptr_t param) {
	m_pIndexBn[1] = DYNAMIC_CAST(FrTextButton, param);
}

// recycledlg.cpp:2779
void FrRecycleAboutDlg::OnIndex02BnUp(void) {
	SetImgAboutIndex(1);
}

// recycledlg.cpp:2784
void FrRecycleAboutDlg::OnIndex03Init(intptr_t param) {
	m_pIndexBn[2] = DYNAMIC_CAST(FrTextButton, param);
}

// recycledlg.cpp:2789
void FrRecycleAboutDlg::OnIndex03BnUp(void) {
	SetImgAboutIndex(2);
}

// recycledlg.cpp:2794
void FrRecycleAboutDlg::OnIndex04Init(intptr_t param) {
	m_pIndexBn[3] = DYNAMIC_CAST(FrTextButton, param);
}

// recycledlg.cpp:2799
void FrRecycleAboutDlg::OnIndex04BnUp(void) {
	SetImgAboutIndex(3);
}

// recycledlg.cpp:2804
void FrRecycleAboutDlg::OnIndex05Init(intptr_t param) {
	m_pIndexBn[4] = DYNAMIC_CAST(FrTextButton, param);
}

// recycledlg.cpp:2809
void FrRecycleAboutDlg::OnIndex05BnUp(void) {
	SetImgAboutIndex(4);
}

// recycledlg.cpp:2814
void FrRecycleAboutDlg::OnBackwardBnInit(intptr_t param) {
	m_pBackwardBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:2819
void FrRecycleAboutDlg::OnBackwardBnUp(void) {
	SetImgAboutIndex(--m_aboutIndex);
	m_pBackwardBn->SetPushDelay(0.0f);
}

// recycledlg.cpp:2825
void FrRecycleAboutDlg::OnForwardBnInit(intptr_t param) {
	m_pForwardBn = DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:2830
void FrRecycleAboutDlg::OnForwardBnUp(void) {
	SetImgAboutIndex(++m_aboutIndex);
	m_pForwardBn->SetPushDelay(0.0f);
}

// recycledlg.cpp:2836
void FrRecycleAboutDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// recycledlg.cpp:2841
void FrRecycleAboutDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// recycledlg.cpp:2846
bool FrRecycleAboutDlg::OnInit(void) {
	SetImgAboutIndex(0);

	return true;
}

// recycledlg.cpp:2854
void FrRecycleAboutDlg::SetImgAboutIndex(int index) {




	for (int i = 0; i < 5; i++) {

		m_pIndexBn[i]->Unselect(0);
	}


	if (m_pBackwardBn) {

		m_pBackwardBn->Enable(true);
	}


	if (m_pForwardBn) {

		m_pForwardBn->Enable(true);
	}



	if (index <= 0) {


		index = 0; m_pIndexBn[0]->Select(0);

		if (m_pBackwardBn) {

			m_pBackwardBn->Enable(false);
		}
	}


	else if (index >= 4) {

		index = 4; m_pIndexBn[4]->Select(0);

		if (m_pForwardBn) {

			m_pForwardBn->Enable(false);
		}
	}



	m_pIndexBn[index]->Select(0);
	m_aboutIndex = index;



	if (m_pAboutViewer) {

		m_pAboutViewer->Open(MakeStr("help_cadie_%02d.jpg", index + 1));
	}
}

// recycledlg.cpp:3045
void FrRecycleDlg::OnSelectPartInit(int param) {
	m_pSelectPartBn = DYNAMIC_CAST(FrButton, param);

	if (m_pSelectPartBn) {

		m_pSelectPartBn->SetPushDelay(0.1f);
		m_pSelectPartBn->SetVisible(false);
	}
}

// recycledlg.cpp:3057
void FrRecycleDlg::OnSelectPartLBDown(void) {
	if (m_pSelectPartDlg == 0) {

		m_pSelectPartDlg = CreateForm<FrSelectPartDlg>(g_pFresh->GetManager(), this, "select_part_dlg", 0);
		if (m_pSelectPartDlg) {




			m_selectedPart = 0; m_partSelected = false; m_pSelectPartDlg->SetPartTID(m_haveItem[0].tid);
			m_pSelectPartDlg->Open((FRESH_RESULT_CALLBACK)&FrRecycleDlg::OnSelectPartDlgResult, WPoint(68.0f, 48.0f), 0);
		}
	}
}

// recycledlg.cpp:3073
bool FrRecycleDlg::OnSelectPartDlgResult(int ret, FrForm* pForm) {


	if (ret == 1) {

		m_selectedPart = m_pSelectPartDlg->m_result;
		m_partSelected = true;
	}
	else {



		m_selectedPart = 0;
		m_partSelected = false;
	}


	m_pSelectPartDlg = 0;




	return true;
}

// recycledlg.cpp:3095
void FrRecycleDlg::OnCardSlotInit(int param) {
	m_pCardSlot = DYNAMIC_CAST(FrArea, param);
}

// recycledlg.cpp:3101
void FrRecycleDlg::OnCardSlotResultInit(int param) {
	m_pCardSlotResult = DYNAMIC_CAST(FrArea, param);
}

// recycledlg.cpp:3107
void FrRecycleDlg::OnCardSlotOwnerDraw(int) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI) {

		if (CSharedDoc::Instance()->m_myPartsList.size()) {






			sItemInfo* pItem = 0;
			for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myPartsList.begin(); it != CSharedDoc::Instance()->m_myPartsList.end(); ++it) {


				pItem = &(*it);
				if (pItem->m_serial == m_selectedPart) break;
			}



			if (pItem->m_typeId) {


				IFF_STRUCT::sPart* pPart = ItemManager()->FindPart(pItem->m_typeId);

				if (pPart) {

					pItem->m_charCardSlotCount = pPart->m_charCardSlots;
					pItem->m_caddieCardSlotCount = pPart->m_caddieCardSlots;

					if (!((unsigned short)pItem->m_charCardSlotCount > -1 && (unsigned short)pItem->m_charCardSlotCount < 5)) {

						pItem->m_charCardSlotCount = 0;
					}

					if (!((unsigned short)pItem->m_caddieCardSlotCount > -1 && (unsigned short)pItem->m_caddieCardSlotCount < 5)) {

						pItem->m_caddieCardSlotCount = 0;
					}
				} else {

					pItem->m_charCardSlotCount = 0;
					pItem->m_caddieCardSlotCount = 0;
				}





				if (m_partSelected == true) {

					if (m_pSelectPartBn && m_pSelectPartBn->IsVisible()) {

						DrawPartsSlot(pGI, pItem, m_pCardSlot);
					}
				} else {

					if (m_pSelectPartBn && !m_pSelectPartBn->IsVisible()) {

						if (m_selectedPart && (m_haveItem[0].tid & 0xFC000000) == 0x8000000 && m_haveItem[0].count > 0) {

							DrawPartsSlot(pGI, pItem, m_pCardSlot);
						}
					}
				}
			}
		}
	}
}

// recycledlg.cpp:3181 (run-time type and factory registration of FrSelectPartDlg)
const WRTTI FrSelectPartDlg::m_RTTI("FrSelectPartDlg", &FrForm::m_RTTI);

IObject* FrSelectPartDlgMakeInstance(void) {
	return new FrSelectPartDlg;
}

struct __sFrSelectPartDlg {
	__sFrSelectPartDlg(void) {
		ObjectFactory().AddObjectFunctor(FrSelectPartDlgMakeInstance, "FrSelectPartDlg");
	}
};
static __sFrSelectPartDlg s_registerFrSelectPartDlg;

// recycledlg.cpp:3183
const sFRESH_MSGMAP* FrSelectPartDlg::GetMessageMap(void) const {
	return &FrSelectPartDlg::_MsgMap;
}

sFRESH_MSGMAP FrSelectPartDlg::_MsgMap = { &FrForm::_MsgMap, &FrSelectPartDlg::_MsgEntries[0] };

sFRESH_ENTRY FrSelectPartDlg::_MsgEntries[] = {
	// recycledlg.cpp:3185
	{ "list_box", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectPartDlg::OnInitListBox },
	{ "list_box", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrSelectPartDlg::OnLBUpListBox },
	{ "list_box", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrSelectPartDlg::OnOwnerDrawListBox },

	// recycledlg.cpp:3189
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// recycledlg.cpp:3192
FrSelectPartDlg::FrSelectPartDlg(void) {
	m_pListBox = 0;
	m_partTid = 0;
	m_result = 0;
}

// recycledlg.cpp:3199
FrSelectPartDlg::~FrSelectPartDlg(void) {
	m_partTid = 0;
}

// recycledlg.cpp:3204
void FrSelectPartDlg::SetPartTID(unsigned long tid) {
	m_partTid = tid;

	if (m_pListBox) {

		m_pListBox->ClearItem();




		for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myPartsList.begin(); it != CSharedDoc::Instance()->m_myPartsList.end(); ++it) {

			sItemInfo* pItem = &(*it);

			if (pItem->m_typeId == m_partTid) {

				// A part a character wears is not offered (the search leaves
				// both loops at once: the original's jump to the next part).
				for (std::map<unsigned int, sCharacterInfo>::iterator ci = CSharedDoc::Instance()->m_characterMap.begin(); ci != CSharedDoc::Instance()->m_characterMap.end(); ++ci) {



					for (int i = 0; i < 24; i++) {



						if (pItem->m_serial == ci->second.partIds[i]) goto nextPart;
					}
				}













				m_pListBox->AddItem(pItem);
			}
nextPart:;
		}
	}
}

// recycledlg.cpp:3256
void FrSelectPartDlg::OnInitListBox(intptr_t param) {
	m_pListBox = DYNAMIC_CAST(FrListBox, param);
}

// recycledlg.cpp:3262
void FrSelectPartDlg::OnLBUpListBox(void) {
}

// The text of one status line of a part: the status name with the part's
// bonus and the card slot's bonus ("%s+%d/%s+%d").
// recycledlg.cpp:3383
static const char* MakeStatusString(int value, int index, int slotValue) {
	const char* names[5] = {
		"\xc6\xc4\xbf\xf6",           // power
		"\xc4\xc1\xc6\xae\xb7\xd1",   // control
		"\xc1\xa4\xc8\xae\xb5\xb5",   // accuracy
		"\xbd\xba\xc7\xc9",           // spin
		"\xc4\xbf\xba\xea"            // curve
	};

	if (value != 0) {
		if (slotValue != 0) {
			return MakeStr("%s+%d/%s+%d", names[index], value, "\xbd\xbd\xb7\xd4", slotValue);
		} else {
			return MakeStr("%s+%d", names[index], value);
		}
	} else if (slotValue != 0) {
		return MakeStr("%s%s+%d", names[index], "\xbd\xbd\xb7\xd4", slotValue);
	}

	return "";
}

// recycledlg.cpp:3404
void FrSelectPartDlg::DrawStatus2(FrGraphicInterface* pGI, FrListItem* pItem, short* const status, short* const slotStatus, float x) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {

		if (status[i] != 0 || slotStatus[i] != 0) {


			pGI->SetTextColor(0xFF0000FF, 0xFFFFFFFF);
			pGI->Print11(WPoint(x + 182.0f, offsetY + x + 36.0f) + pItem->pos, 2, MakeStatusString(status[i], i, slotStatus[i]));




			offsetY += 14.0f;
		}
	}
}
