// Original: d:\build\custom\temp\code\client\projectg\scratchdlg.cpp
#include "NetworkSystem.h"
#include "ScratchDlg.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "UccManager.h"
#include "ObjectFactory.h"
#include "Packet.h"
#include "../shared/Localize.h"
#include "wfont.h"
#include "wresrcmng.h"
#include "fresh.h"
#include "frbutton.h"
#include "fredit.h"
#include "frarea.h"
#include "frgraphicinterface.h"
#include "frtextbutton.h"
#include "frviewer.h"
#include "frdesktop.h"
#include <string.h>

// Title font file names ("[font_wind.jpg", ...).
extern "C" char* data_a9bea0[];

// scratchdlg.cpp:18 (run-time type and factory registration of FrScratchDlg)
const WRTTI FrScratchDlg::m_RTTI("FrScratchDlg", &FrForm::m_RTTI);

IObject* FrScratchDlgMakeInstance(void) {
	return new FrScratchDlg;
}

struct __sFrScratchDlg {
	__sFrScratchDlg(void) {
		ObjectFactory().AddObjectFunctor(FrScratchDlgMakeInstance, "FrScratchDlg");
	}
};
static __sFrScratchDlg s_registerFrScratchDlg;

// scratchdlg.cpp:20
const sFRESH_MSGMAP* FrScratchDlg::GetMessageMap(void) const {
	return &FrScratchDlg::_MsgMap;
}

// scratchdlg.cpp:112
FrScratchDlg::~FrScratchDlg(void) {
	if (IsLocalContent((localContentType_t)0x59)) {
		if (WNetworkSystem::Instance()) {
			WSendPacket packet((enumClientPacket)0xb9);
			packet.Encode1(1);
			packet.Send((eSendTo)0);
		}
	}

	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }

	OpenScratchResultDlg();
}

// scratchdlg.cpp:135
void FrScratchDlg::OnScratchDlgInit(int) {
}

// scratchdlg.cpp:140
void FrScratchDlg::OnScratchBottomInit(int param) {
	DYNAMIC_CAST(FrArea, param);
}

// scratchdlg.cpp:146
void FrScratchDlg::OnScratchBottomOwnerDraw(void) {
	DrawScratchItem();
	DrawChangeScratchCardNumber();
}

// scratchdlg.cpp:153
void FrScratchDlg::OnScratchCover01Init(int param) {
	m_pScratchCover[0] = DYNAMIC_CAST(FrArea, param);

	if (m_pScratchCover[0]) {
		m_pCoverAreaBitmap[0] = m_pScratchCover[0]->GetBgImg();

		ResetBmpScratchCover(0);
	}
}

// scratchdlg.cpp:166
void FrScratchDlg::OnScratchCover01OwnerDraw(void) {
}

// scratchdlg.cpp:171
void FrScratchDlg::OnScratchCover02Init(int param) {
	m_pScratchCover[1] = DYNAMIC_CAST(FrArea, param);

	if (m_pScratchCover[1]) {
		m_pCoverAreaBitmap[1] = m_pScratchCover[1]->GetBgImg();

		ResetBmpScratchCover(1);
	}
}

// scratchdlg.cpp:184
void FrScratchDlg::OnScratchCover02OwnerDraw(void) {
}

// scratchdlg.cpp:189
void FrScratchDlg::OnScratchCardInit(int param) {
	m_pScratchCardEdit = DYNAMIC_CAST(FrEdit, param);
}

// scratchdlg.cpp:195
bool FrScratchDlg::OnScratchCardEnterKey(int) {
	return false;
}

// scratchdlg.cpp:201
void FrScratchDlg::OnScratchNextPageBnInit(int param) {
	m_pNextPageBn = DYNAMIC_CAST(FrButton, param);

	if (m_pNextPageBn) {
		m_pNextPageBn->SetVisible(false);
	}
}

// scratchdlg.cpp:212
void FrScratchDlg::OnScratchNextPageBnUp(void) {
	if (!m_pNextPageBn) {
		return;
	}

	m_pNextPageBn->Enable(false);

	m_bReceivedScratchItem = false;

	if (m_scratchItem[0].m_typeId == 0) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

		if (pForm) {
			pForm->Open((FRESH_RESULT_CALLBACK)&FrScratchDlg::OnConfirmResult, 0xc0);
			pForm->SetFadeout(false);
			// "Next time..."
			pForm->SetMessage("\xB4\xD9\xC0\xBD \xB1\xE2\xC8\xB8\xBF\xA1...", false);
		}

		m_resultTextTime = 0.0f;
		m_bWaitResultText = true;
	}
	else {
		OpenScratchResultDlg();
		m_resultTextTime = 0.0f;
		m_bWaitResultText = true;
		OnLButtonUp(WPoint(0.0f, 0.0f));
	}
}

// scratchdlg.cpp:233
void FrScratchDlg::OnSerialNumberInit(int param) {
	m_pSerialNumberEdit = DYNAMIC_CAST(FrEdit, param);
}

// scratchdlg.cpp:242
bool FrScratchDlg::OnSerialNumberEnterKey(int) {
	return false;
}

// scratchdlg.cpp:248
void FrScratchDlg::OnSerialNumberBnInit(int param) {
	m_pSerialNumberBn = DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:257
void FrScratchDlg::OnSerialNumberBnUp(void) {
	if (m_pSerialNumberBn) {
		SendScratchSerialNumber();
		EnableScratchSerialNumber(false);
	}
}

// scratchdlg.cpp:267
void FrScratchDlg::OnAboutBnInit(int param) {
	DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:273
void FrScratchDlg::OnAboutBnUp(void) {
	FrScratchAboutDlg* pDlg = CreateForm<FrScratchAboutDlg>(g_pFresh->GetManager(), this, "scratch_about", 0);

	if (pDlg) {
		pDlg->Open((FRESH_RESULT_CALLBACK)0, 3);
	}
}

// scratchdlg.cpp:284
void FrScratchDlg::OnCloseBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:290
void FrScratchDlg::OnCloseBnUp(void) {
	OnFreshOkay();
}

// scratchdlg.cpp:296
bool FrScratchDlg::OnInit(void) {
	if (m_pBmpEraser && m_pScratchCover[0] && m_pScratchCover[1]) {
		m_rectEraseArea.x = (float)(int)(m_pScratchCover[0]->GetRect().x - m_pBmpEraser->Width() / 2);
		m_rectEraseArea.y = (float)(int)(m_pScratchCover[0]->GetRect().y - m_pBmpEraser->Height() / 2);
		m_rectEraseArea.w = (float)(int)(m_pCoverAreaBitmap[0]->Width() + m_pCoverAreaBitmap[1]->Width() + m_pBmpEraser->Width());
		m_rectEraseArea.h = (float)(int)(m_pCoverAreaBitmap[0]->Height() + m_pBmpEraser->Height());
	}

	InitScratchCard();
	SetBmpColsedScratchCover();

	return true;
}

// scratchdlg.cpp:316
void FrScratchDlg::OnProc(const float dt) {
	ProcessChangeScratchCardNumber(dt);
}

// scratchdlg.cpp:324
void FrScratchDlg::OnMouseMove(const WPoint& pt) {
	if (m_bLButtonDown) {
		RequestScratchItem();
		OpenScratchCover(pt);
	}
}

// scratchdlg.cpp:335
bool FrScratchDlg::OnLButtonDown(const WPoint& pt) {
	FrForm::OnLButtonDown(pt);

	if (m_pScratchCover[0] && m_pScratchCover[1]) {
		WRect rect;
		rect.x = m_pScratchCover[0]->GetRect().x + 10.0f;
		rect.y = m_pScratchCover[0]->GetRect().y + 10.0f;
		rect.w = m_bmpScratchCover[1].Width() + m_bmpScratchCover[0].Width() - 20.0f;
		rect.h = m_bmpScratchCover[0].Height() - 20.0f;

		if (rect.IsInRect(pt)) {
			m_bLButtonDown = true;
		}
	}

	return false;
}

// scratchdlg.cpp:356
bool FrScratchDlg::OnLButtonUp(const WPoint& pt) {
	FrForm::OnLButtonUp(pt);

	m_bLButtonDown = false;

	return false;
}

// scratchdlg.cpp:365
void FrScratchDlg::OnCancel(void) {
}

// scratchdlg.cpp:372
void FrScratchDlg::InitScratchCard(void) {
	m_scratchCardCount = 0;

	for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
		if (ItemManager()->GetCouponKind((*it).m_typeId) == 3) {
			m_scratchCardCount += (*it).m_clubStatLevel[0];
		}
	}

	if (m_pScratchCardEdit) {
		m_pScratchCardEdit->SetLine(1, MakeStr("%d", m_scratchCardCount), 0, false, 0);
	}
}

// scratchdlg.cpp:395
void FrScratchDlg::SetBmpColsedScratchCover(void) {
	if (m_bReceivedScratchItem) {
		return;
	}

	if (m_scratchCardCount == 0) {
		memcpy(m_bmpScratchCover[0].vram, m_pBmpClosedCover->vram, m_pBmpClosedCover->Size());

		m_pScratchCover[0]->SetBgImg(&m_bmpScratchCover[0]);
		GDI()->UpdateTextureCacheInfo(&m_bmpScratchCover[0]);
	}
	else {
		ResetBmpScratchCover(0);
	}

	ResetBmpScratchCover(1);
}

// scratchdlg.cpp:418
void FrScratchDlg::ResetBmpScratchCover(int index) {
	if (m_pCoverAreaBitmap[index]) {
		if (!m_bmpScratchCover[index].bi) {
			m_bmpScratchCover[index].Create(m_pCoverAreaBitmap[index]->Width(), m_pCoverAreaBitmap[index]->Height(), m_pCoverAreaBitmap[index]->BitsPerPixel());
		}

		memcpy(m_bmpScratchCover[index].vram, m_pCoverAreaBitmap[index]->vram, m_pCoverAreaBitmap[index]->Size());
		m_pScratchCover[index]->SetBgImg(&m_bmpScratchCover[index]);
		GDI()->UpdateTextureCacheInfo(&m_bmpScratchCover[index]);
	}
}

// scratchdlg.cpp:496
void FrScratchDlg::EnableNextPageButton(bool bEnable) {
	if (m_pNextPageBn) {
		m_pNextPageBn->SetVisible(bEnable);
		m_pNextPageBn->Enable(true);
	}
}

// scratchdlg.cpp:506
void FrScratchDlg::EnableScratchSerialNumber(bool bEnable) {
	if (m_pSerialNumberBn) {
		m_pSerialNumberBn->Enable(bEnable);
	}

	if (m_pSerialNumberEdit) {
		m_pSerialNumberEdit->ClearLine();
		m_pSerialNumberEdit->Enable(bEnable);
		m_pSerialNumberEdit->SetKeyFocus(bEnable);
		m_pSerialNumberEdit->SetViewFocus(bEnable);
	}

	m_bEnableSerialNumber = bEnable;
}

// scratchdlg.cpp:525
void FrScratchDlg::SendScratchSerialNumber(void) {
	if (m_pSerialNumberEdit) {
		const char* pSerialNumber = m_pSerialNumberEdit->GetLine(1, false);
		int length = strlen(pSerialNumber);

		if (length == 13) {
			WSendPacket packet((enumClientPacket)0x71);
			packet.Encode4(13);
			packet.EncodeBuffer(pSerialNumber, 13);
			packet.Send((eSendTo)0);

			m_bEnableSerialNumber = false;
		}
		else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

			if (pForm) {
				pForm->Open((FRESH_RESULT_CALLBACK)&FrScratchDlg::OnConfirmResult, 0xc0);
				pForm->SetFadeout(false);
				// "Serial number" (red) "is not valid." (black)
				pForm->SetMessage("\\c0xffff0000\\c\xBD\xC3\xB8\xAE\xBE\xF3 \xB9\xF8\xC8\xA3\\c0xff000000\\c\xB0\xA1 \xBF\xC3\xB9\xD9\xB8\xA3\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			}

			m_resultTextTime = 0.0f;
			m_bWaitResultText = true;
		}
	}
}

// scratchdlg.cpp:602
void FrScratchDlg::RequestScratchItem(void) {
	if (m_scratchCardCount == 0) {
		return;
	}

	if (m_bEnableRequestScratchItem) {
		WSendPacket packet((enumClientPacket)0x70);
		packet.Send((eSendTo)0);

		m_bEnableRequestScratchItem = false;
		EnableScratchSerialNumber(false);
	}
}

// scratchdlg.cpp:619
void FrScratchDlg::SetScratchItem(int count, sBonusBall* pItems) {
	if (!pItems) {
		return;
	}

	memcpy(m_scratchItem, pItems, count * sizeof(sBonusBall));

	m_scratchItemCount = count;
	m_bReceivedScratchItem = true;

	for (int i = 0; i < count; i++) {
		UpdateListMyItem(&pItems[i]);
	}

	InitScratchCard();

	SetBmpScratchItem();

	SetRectScratchItem();

	SetRectCheckScratchItemRect();
}

// scratchdlg.cpp:651
void FrScratchDlg::ClearScratchItem(void) {
	memset(m_scratchItem, 0, sizeof(m_scratchItem));
	m_scratchItemCount = 0;
}

// scratchdlg.cpp:658
void FrScratchDlg::SetBmpResultText(int result) {
	if (result >= 0 && result < 6) {
		if (result == 4) {
			OpenScratchResultDlg();
		}
		else {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);

			if (pForm) {
				pForm->Open((FRESH_RESULT_CALLBACK)&FrScratchDlg::OnConfirmResult, 0xc0);
				pForm->SetFadeout(false);

				switch (result) {
				case 0:
					// "Serial number" (red) "is not valid." (black)
					pForm->SetMessage("\\c0xffff0000\\c\xBD\xC3\xB8\xAE\xBE\xF3 \xB9\xF8\xC8\xA3\\c0xff000000\\c\xB0\xA1 \xBF\xC3\xB9\xD9\xB8\xA3\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9.", false);
					break;
				case 1:
					// "The serial number has already been used."
					pForm->SetMessage("\xC0\xCC\xB9\xCC \xBB\xE7\xBF\xEB\xC7\xD1 \\c0xffff0000\\c\xBD\xC3\xB8\xAE\xBE\xF3 \xB9\xF8\xC8\xA3\\c0xff000000\\c\xC0\xD4\xB4\xCF\xB4\xD9.", false);
					break;
				case 2:
					// "The validity period of this card's serial number has passed."
					pForm->SetMessage("\\c0xffff0000\\c\xC0\xAF\xC8\xBF \xB1\xE2\xB0\xA3\\c0xff000000\\c\xC0\xCC \xC1\xF6\xB3\xAD \xC4\xAB\xB5\xE5\xC0\xC7 \xBD\xC3\xB8\xAE\xBE\xF3 \xB9\xF8\xC8\xA3\xC0\xD4\xB4\xCF\xB4\xD9.", false);
					break;
				case 3:
					// "Up to 5 event coupons per account within the validity period; all used."
					pForm->SetMessage("\xC0\xCC\xBA\xA5\xC6\xAE\xB7\xCE \xB9\xDE\xC0\xB8\xBD\xC5 \xC4\xED\xC6\xF9\xC0\xBA \xC0\xAF\xC8\xBF\xB1\xE2\xB0\xA3 \xB3\xBB\xBF\xA1 \xC7\xD1 \xB0\xE8\xC1\xA4\xB4\xE7 \xC3\xD6\xB4\xEB 5\xB0\xB3\xB1\xEE\xC1\xF6\xB8\xB8 \xBB\xE7\xBF\xEB\xC0\xCC \xB0\xA1\xB4\xC9\xC7\xD5\xB4\xCF\xB4\xD9. \xC0\xCC\xB9\xCC \xC4\xED\xC6\xF9\xC0\xBB \xB8\xF0\xB5\xCE \xBB\xE7\xBF\xEB\xC7\xCF\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.", false);
					break;
				case 5:
					// "Next time..."
					pForm->SetMessage("\xB4\xD9\xC0\xBD \xB1\xE2\xC8\xB8\xBF\xA1...", false);
					break;
				default:
					// "Error"
					pForm->SetMessage("\xBF\xC0\xB7\xF9", false);
					break;
				}
			}
		}

		m_resultTextTime = 0.0f;
		m_bWaitResultText = true;
	}
	else {
		m_bWaitResultText = false;
	}
}

// scratchdlg.cpp:709
void FrScratchDlg::ProcessResultText(float dt) {
	if (m_serialNumberResult == 0) {
		return;
	}

	if (m_bWaitResultText) {
		m_resultTextTime += dt;

		if (m_resultTextTime > 2.0f) {
			m_resultTextTime = 0.0f;
			m_bWaitResultText = false;
			EnableScratchSerialNumber(true);
			EnableNextPageButton(false);

			if (!m_bEnableRequestScratchItem) {
				ClearDataCheckScratchItem();
				ClearScratchItem();
				SetBmpColsedScratchCover();

				m_bEnableRequestScratchItem = true;
			}
		}
	}
}

// scratchdlg.cpp:746
void FrScratchDlg::ClearDataCheckScratchItem(void) {
	memset(m_checkScratchPos, 0, sizeof(m_checkScratchPos));
	memset(m_checkScratchItem, 0, sizeof(m_checkScratchItem));
}

// scratchdlg.cpp:753
void FrScratchDlg::SetBmpScratchItem(void) {
	if (m_scratchItemCount <= 0) {
		m_pBmpScratchItem[0] = g_pFresh->GetBitmap("scratch_text_nothing");
		return;
	}

	for (int i = 0; i < m_scratchItemCount; i++) {
		if (m_scratchItem[i].m_typeId) {
			IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(m_scratchItem[i].m_typeId);

			if (pItem) {
				m_pBmpScratchItem[i] = g_pFresh->GetBitmap(pItem->m_icon);
			}

			if ((m_scratchItem[i].m_typeId & 0xFC000000) == 0x24000000) {
				m_scratchItemCount = 1;
				break;
			}
		} else {
			m_pBmpScratchItem[i] = g_pFresh->GetBitmap("scratch_text_nothing");
			break;
		}
	}
}

// scratchdlg.cpp:841
void FrScratchDlg::SetRectCheckScratchItemRect(void) {
	if (m_pBmpScratchItem[0]) {
		WPoint offset(24.0f, 24.0f);

		if (m_scratchItemCount < 2) {
			if ((m_scratchItem[0].m_typeId & 0xFC000000) != 0x18000000) {
				offset = WPoint(0.0f, 0.0f);
			}

			SetRectCheckScratchItemRectByIndex(2, offset);
		} else {
			SetRectCheckScratchItemRectByIndex(0, WPoint(24.0f, 24.0f));
			SetRectCheckScratchItemRectByIndex(1, WPoint(24.0f, 24.0f));
		}
	}
}

// scratchdlg.cpp:981
void FrScratchDlg::ProcessChangeScratchCardNumber(float dt) {
	int pos = (int)(dt * 480.0f);

	for (int i = 0; i < 2; i++) {
		m_changeCardTime[i] -= dt * 20.0f;
		m_changeCardPos[i] -= pos;
	}
}

// scratchdlg.cpp:993
void FrScratchDlg::InitChangeScratchCardNumber(int number) {
	if (number < 0 || number >= 2) {
		return;
	}

	int slot = 0;
	if (m_changeCardTime[0] > m_changeCardTime[1]) {
		slot = 1;
	}

	m_changeCardNumber[slot] = number;
	m_changeCardTime[slot] = 0;
	m_changeCardPos[slot] = 0x200;
}

// scratchdlg.cpp:1066
void FrScratchDlg::DrawChangeScratchCardNumber(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	if (pGDI) {
		for (int i = 0; i < 2; i++) {
			if (m_changeCardNumber[i] >= 0
				&& m_pBmpChangeCardNumber[m_changeCardNumber[i]]) {
				unsigned char alpha;

				if (m_changeCardPos[i] < 0) {
					alpha = 0;
					m_changeCardNumber[i] = -1;
				}
				else if (m_changeCardPos[i] > 0xff) {
					alpha = 0xff;
				}
				else {
					alpha = (unsigned char)m_changeCardPos[i];
				}

				WRect rect(m_rect.x + 348.0f, m_changeCardTime[i] + m_rect.y + 113.0f,
					m_pBmpChangeCardNumber[m_changeCardNumber[i]]->Width(), m_pBmpChangeCardNumber[m_changeCardNumber[i]]->Height());

				pGDI->DrawTexture(m_pBmpChangeCardNumber[m_changeCardNumber[i]], rect, (alpha << 24) | 0xffffff, 0);
			}
		}
	}
}

// scratchdlg.cpp:1105
void FrScratchDlg::OpenScratchResultDlg(void) {
	if (m_scratchItem[0].m_typeId) {
		FrScratchResultDlg* pDlg = CreateForm<FrScratchResultDlg>(g_pFresh->GetManager(), this, "scratchresultdlg", 0);

		if (pDlg) {
			pDlg->Open((FRESH_RESULT_CALLBACK)&FrScratchDlg::OnConfirmResult, 0xc0);
			pDlg->SetFadeout(false);
			pDlg->SetResultItem(m_scratchItem, m_scratchItemCount);
		}
	}
}

// scratchdlg.cpp:1120
bool FrScratchDlg::OnConfirmResult(int result, FrForm* pForm) {
	if (result == 1 || result == 2) {
		FrScratchDlg* pDlg = FrStaticCast<FrScratchDlg>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("scratchdlg"));

		if (!pDlg) {
			return true;
		}

		pDlg->EnableScratchSerialNumber(true);
		if (pDlg->m_pNextPageBn) { pDlg->m_pNextPageBn->SetVisible(false); pDlg->m_pNextPageBn->Enable(true); }
		pDlg->ClearDataCheckScratchItem();
		pDlg->ClearScratchItem();
		pDlg->SetBmpColsedScratchCover();
		pDlg->m_bEnableRequestScratchItem = true;
	}

	return true;
}

// scratchdlg.cpp:1311 (run-time type and factory registration of
// FrScratchResultDlg)
const WRTTI FrScratchResultDlg::m_RTTI("FrScratchResultDlg", &FrForm::m_RTTI);

IObject* FrScratchResultDlgMakeInstance(void) {
	return new FrScratchResultDlg;
}

struct __sFrScratchResultDlg {
	__sFrScratchResultDlg(void) {
		ObjectFactory().AddObjectFunctor(FrScratchResultDlgMakeInstance, "FrScratchResultDlg");
	}
};
static __sFrScratchResultDlg s_registerFrScratchResultDlg;

// scratchdlg.cpp:1313
const sFRESH_MSGMAP* FrScratchResultDlg::GetMessageMap(void) const {
	return &FrScratchResultDlg::_MsgMap;
}

sFRESH_MSGMAP FrScratchResultDlg::_MsgMap = { &FrForm::_MsgMap, &FrScratchResultDlg::_MsgEntries[0] };

sFRESH_ENTRY FrScratchResultDlg::_MsgEntries[] = {
	// scratchdlg.cpp:1315
	{ "scratchresultdlg", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScratchResultDlg::OnScratchResultDlgInit },
	// scratchdlg.cpp:1316
	{ "resultitem_icon", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScratchResultDlg::OnResultItemIconInit },
	// scratchdlg.cpp:1317
	{ "resultitem_icon", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrScratchResultDlg::OnResultItemIconOwnerDraw },
	// scratchdlg.cpp:1318
	{ "resultitem_name", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScratchResultDlg::OnResultItemNameInit },
	// scratchdlg.cpp:1319
	{ "resultitem_ability", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrScratchResultDlg::OnResultItemAbilityInit },

	// scratchdlg.cpp:1321
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// scratchdlg.cpp:1324
FrScratchResultDlg::FrScratchResultDlg(void) {
	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	m_pResultItemIcon = 0;
	m_pResultItemName = 0;
	m_pResultItemAbility = 0;

	info.filename = data_a9bea0;
	info.fontw = 16;
	info.fonth = 16;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:";
	m_pTitleFont = g_resrcmng->GetTitleFont();
	m_pTitleFont->Create(&info);

	memset(m_resultItem, 0, sizeof(m_resultItem));
	m_resultItemCount = 0;
	m_bShowItemDetail = false;
	m_pBmpResultItem[0] = 0;
	m_pBmpResultItem[1] = 0;
}

// scratchdlg.cpp:1353
FrScratchResultDlg::~FrScratchResultDlg(void) {
	if (m_pTitleFont) {
		m_pTitleFont->Erase();
	}

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }
}

// scratchdlg.cpp:1364
void FrScratchResultDlg::OnScratchResultDlgInit(int) {
}

// scratchdlg.cpp:1369
void FrScratchResultDlg::OnResultItemIconInit(intptr_t param) {
	m_pResultItemIcon = DYNAMIC_CAST(FrArea, param);
}

// scratchdlg.cpp:1468
void FrScratchResultDlg::OnResultItemNameInit(intptr_t param) {
	m_pResultItemName = DYNAMIC_CAST(FrEdit, param);
}

// scratchdlg.cpp:1474
void FrScratchResultDlg::OnResultItemAbilityInit(intptr_t param) {
	m_pResultItemAbility = DYNAMIC_CAST(FrEdit, param);
}

// scratchdlg.cpp:1480
bool FrScratchResultDlg::OnInit(void) {
	return true;
}

// scratchdlg.cpp:1486
void FrScratchResultDlg::OnProc(const float) {
}

// scratchdlg.cpp:1636 (run-time type and factory registration of
// FrScratchAboutDlg)
const WRTTI FrScratchAboutDlg::m_RTTI("FrScratchAboutDlg", &FrForm::m_RTTI);

IObject* FrScratchAboutDlgMakeInstance(void) {
	return new FrScratchAboutDlg;
}

struct __sFrScratchAboutDlg {
	__sFrScratchAboutDlg(void) {
		ObjectFactory().AddObjectFunctor(FrScratchAboutDlgMakeInstance, "FrScratchAboutDlg");
	}
};
static __sFrScratchAboutDlg s_registerFrScratchAboutDlg;

// scratchdlg.cpp:1638
const sFRESH_MSGMAP* FrScratchAboutDlg::GetMessageMap(void) const {
	return &FrScratchAboutDlg::_MsgMap;
}

// scratchdlg.cpp:1661
FrScratchAboutDlg::FrScratchAboutDlg(void) {
	for (int i = 0; i < 5; i++) {
		m_pIndexBn[i] = 0;
	}

	m_curIndex = 0;
	m_pBackwardBn = 0;
	m_pForwardBn = 0;
}

// scratchdlg.cpp:1675
FrScratchAboutDlg::~FrScratchAboutDlg(void) {
}

// scratchdlg.cpp:1679
void FrScratchAboutDlg::OnAboutViewerInit(intptr_t param) {
	m_pAboutViewer = DYNAMIC_CAST(FrViewer, param);
}

// scratchdlg.cpp:1685
void FrScratchAboutDlg::OnIndex01Init(intptr_t param) {
	m_pIndexBn[0] = DYNAMIC_CAST(FrTextButton, param);
}

// scratchdlg.cpp:1690
void FrScratchAboutDlg::OnIndex01BnUp(void) {
	SetImgAboutIndex(0);
}

// scratchdlg.cpp:1695
void FrScratchAboutDlg::OnIndex02Init(intptr_t param) {
	m_pIndexBn[1] = DYNAMIC_CAST(FrTextButton, param);
}

// scratchdlg.cpp:1700
void FrScratchAboutDlg::OnIndex02BnUp(void) {
	SetImgAboutIndex(1);
}

// scratchdlg.cpp:1705
void FrScratchAboutDlg::OnIndex03Init(intptr_t param) {
	m_pIndexBn[2] = DYNAMIC_CAST(FrTextButton, param);
}

// scratchdlg.cpp:1710
void FrScratchAboutDlg::OnIndex03BnUp(void) {
	SetImgAboutIndex(2);
}

// scratchdlg.cpp:1715
void FrScratchAboutDlg::OnIndex04Init(intptr_t param) {
	m_pIndexBn[3] = DYNAMIC_CAST(FrTextButton, param);
}

// scratchdlg.cpp:1720
void FrScratchAboutDlg::OnIndex04BnUp(void) {
	SetImgAboutIndex(3);
}

// scratchdlg.cpp:1725
void FrScratchAboutDlg::OnIndex05Init(intptr_t param) {
	m_pIndexBn[4] = DYNAMIC_CAST(FrTextButton, param);
}

// scratchdlg.cpp:1730
void FrScratchAboutDlg::OnIndex05BnUp(void) {
	SetImgAboutIndex(4);
}

// scratchdlg.cpp:1735
void FrScratchAboutDlg::OnBackwardBnInit(intptr_t param) {
	m_pBackwardBn = DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:1740
void FrScratchAboutDlg::OnBackwardBnUp(void) {
	SetImgAboutIndex(--m_curIndex);
	m_pBackwardBn->SetPushDelay(0.0f);
}

// scratchdlg.cpp:1746
void FrScratchAboutDlg::OnForwardBnInit(intptr_t param) {
	m_pForwardBn = DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:1751
void FrScratchAboutDlg::OnForwardBnUp(void) {
	SetImgAboutIndex(++m_curIndex);
	m_pForwardBn->SetPushDelay(0.0f);
}

// scratchdlg.cpp:1757
void FrScratchAboutDlg::OnOkBnInit(intptr_t param) {
	DYNAMIC_CAST(FrButton, param);
}

// scratchdlg.cpp:1762
void FrScratchAboutDlg::OnOkBnUp(void) {
	OnFreshOkay();
}

// scratchdlg.cpp:1767
bool FrScratchAboutDlg::OnInit(void) {
	SetImgAboutIndex(0);

	return true;
}

// scratchdlg.cpp:1775
void FrScratchAboutDlg::SetImgAboutIndex(int index) {
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
		index = 0;
		m_pIndexBn[0]->Select(0);

		if (m_pBackwardBn) {
			m_pBackwardBn->Enable(false);
		}
	}
	else if (index >= 4) {
		index = 4;
		m_pIndexBn[4]->Select(0);

		if (m_pForwardBn) {
			m_pForwardBn->Enable(false);
		}
	}

	m_pIndexBn[index]->Select(0);

	m_curIndex = index;
	if (m_pAboutViewer) {
		m_pAboutViewer->Open(MakeStr("help_scratch_%02d.jpg", index + 1));
	}
}
