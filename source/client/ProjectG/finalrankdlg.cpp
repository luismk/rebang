// Original: d:\build\custom\temp\code\client\projectg\finalrankdlg.cpp
#include "FinalRankDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "SharedDoc.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "NetResourceManager.h"
#include "ProjectG.h"

bool FinalRankCompare(const void* a, const void* b);

// finalrankdlg.cpp:18 (run-time type and factory registration of FrFinalRankDlg)
const WRTTI FrFinalRankDlg::m_RTTI("FrFinalRankDlg", &FrForm::m_RTTI);

IObject* FrFinalRankDlgMakeInstance(void) {
	return new FrFinalRankDlg;
}

struct __sFrFinalRankDlg {
	__sFrFinalRankDlg(void) {
		ObjectFactory().AddObjectFunctor(FrFinalRankDlgMakeInstance, "FrFinalRankDlg");
	}
};
static __sFrFinalRankDlg s_registerFrFinalRankDlg;

// finalrankdlg.cpp:20
const sFRESH_MSGMAP* FrFinalRankDlg::GetMessageMap(void) const {
	return &FrFinalRankDlg::_MsgMap;
}

sFRESH_MSGMAP FrFinalRankDlg::_MsgMap = { &FrForm::_MsgMap, &FrFinalRankDlg::_MsgEntries[0] };

sFRESH_ENTRY FrFinalRankDlg::_MsgEntries[] = {
	// finalrankdlg.cpp:22
	{ "list_left", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFinalRankDlg::OnRankLeftInit },
	// finalrankdlg.cpp:23
	{ "list_left", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFinalRankDlg::OnRankLeftOwnerDraw },
	// finalrankdlg.cpp:24
	{ "list_right", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFinalRankDlg::OnRankRightInit },
	// finalrankdlg.cpp:25
	{ "list_right", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrFinalRankDlg::OnRankRightOwnerDraw },

	// finalrankdlg.cpp:27
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// finalrankdlg.cpp:32
FrFinalRankDlg::FrFinalRankDlg(void)
	: m_pListLeft(0), m_pListRight(0) {
	for (std::vector<sGameResultInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
		 it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
		if (it->m_state == 3) {
			it->m_rank = 0xFF;
			continue;
		}

		it->m_rank = 1;
		for (std::vector<sGameResultInfo>::iterator other = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
			 other != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++other) {
			if (it == other || other->m_state == 3) {
				continue;
			}

			if (it->m_score > other->m_score || (it->m_score == other->m_score && it->m_pang < other->m_pang)) {
				it->m_rank++;
			}
		}
	}
}

// finalrankdlg.cpp:56
void FrFinalRankDlg::OnRankLeftInit(intptr_t param) {
	m_pListLeft = DYNAMIC_CAST(FrListBox, param);

	for (std::vector<sGameResultInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
		 it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
		if (it->m_rank < 0x10) {
			m_pListLeft->AddItem(&*it);
		}
	}

	m_pListLeft->SortItem(FinalRankCompare);
}

// finalrankdlg.cpp:71
void FrFinalRankDlg::OnRankLeftOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {


		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();



		int oldSpace = pGDI->SetSpace(0);

		sGameResultInfo* pInfo = (sGameResultInfo*)pItem->pData;

		if (pInfo->m_uid == WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid)
			pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
		else if (pInfo->m_state == 3)
			pGDI->SetTextColor(0xFF808080, 0xFFFFFFFF);
		else
			pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);

		pGDI->SetTextStyle(0);



		if (pInfo->m_state == 3)
			pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "-");
		else
			pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "%d", pInfo->m_rank);





		float offset = 95.0f;
		if (pInfo->m_hasEmblem) {


			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(pInfo->m_emblemName);




			if (emblem) {

				pGDI->DrawTexture(emblem, WRect(pItem->pos.x + 95.0f, pItem->pos.y + 3.0f - 6.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
				offset = 122.0f;
			}
		}



		if (!WSingleton<CProjectG>::Instance()->HidePrivacy())
			g_pFresh->GetManager()->PrintText(WPoint(offset + pItem->pos.x, pItem->pos.y + 3.0f), 0, pInfo->m_nickname, 120.0f, 0xFFFFFFFF);


		pGDI->Print(WPoint(pItem->pos.x + 186.0f, pItem->pos.y + 3.0f), 2, "%d", pInfo->m_score);


		pGDI->Print(WPoint(pItem->pos.x + 230.0f, pItem->pos.y + 3.0f), 2, "%I64d", pInfo->m_pang);



		if (pInfo->m_state == 2 || GetHoleIndex(pInfo->m_hole) == 0x13)
			pGDI->Print(WPoint(pItem->pos.x + 268.0f, pItem->pos.y + 3.0f), 2, "End");
		else
			pGDI->Print(WPoint(pItem->pos.x + 268.0f, pItem->pos.y + 3.0f), 2, "%d\xC8\xA6", GetHoleIndex(pInfo->m_hole));


		pGDI->SetSpace(oldSpace);
	}
}

// finalrankdlg.cpp:138
void FrFinalRankDlg::OnRankRightInit(intptr_t param) {
	m_pListRight = DYNAMIC_CAST(FrListBox, param);

	for (std::vector<sGameResultInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_gameResultList.begin();
		 it != WSingleton<CSharedDoc>::Instance()->m_gameResultList.end(); ++it) {
		if (it->m_rank > 0xF) {
			m_pListRight->AddItem(&*it);
		}
	}

	m_pListRight->SortItem(FinalRankCompare);
}

// finalrankdlg.cpp:153
void FrFinalRankDlg::OnRankRightOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {


		FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();



		int oldSpace = pGDI->SetSpace(0);

		sGameResultInfo* pInfo = (sGameResultInfo*)pItem->pData;

		if (pInfo->m_uid == WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid)
			pGDI->SetTextColor(0xFFFF0000, 0xFFFFFFFF);
		else if (pInfo->m_state == 3)
			pGDI->SetTextColor(0xFF808080, 0xFFFFFFFF);
		else
			pGDI->SetTextColor(0xFF000000, 0xFFFFFFFF);

		pGDI->SetTextStyle(0);



		if (pInfo->m_state == 3)
			pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "-");
		else
			pGDI->Print(WPoint(pItem->pos.x + 21.0f, pItem->pos.y + 3.0f), 1, "%d", pInfo->m_rank);





		float offset = 95.0f;
		if (pInfo->m_hasEmblem) {


			const Bitmap* emblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(pInfo->m_emblemName);




			if (emblem) {

				pGDI->DrawTexture(emblem, WRect(pItem->pos.x + 95.0f, pItem->pos.y + 3.0f - 6.0f, emblem->Width(), emblem->Height()), 0xFFFFFFFF, 0);
				offset = 122.0f;
			}
		}



		if (!WSingleton<CProjectG>::Instance()->HidePrivacy())
			g_pFresh->GetManager()->PrintText(WPoint(offset + pItem->pos.x, pItem->pos.y + 3.0f), 0, pInfo->m_nickname, 120.0f, 0xFFFFFFFF);


		pGDI->Print(WPoint(pItem->pos.x + 186.0f, pItem->pos.y + 3.0f), 2, "%d", pInfo->m_score);


		pGDI->Print(WPoint(pItem->pos.x + 230.0f, pItem->pos.y + 3.0f), 2, "%I64d", pInfo->m_pang);



		if (pInfo->m_state == 2 || GetHoleIndex(pInfo->m_hole) == 0x13)
			pGDI->Print(WPoint(pItem->pos.x + 268.0f, pItem->pos.y + 3.0f), 2, "End");
		else
			pGDI->Print(WPoint(pItem->pos.x + 268.0f, pItem->pos.y + 3.0f), 2, "%d\xC8\xA6", GetHoleIndex(pInfo->m_hole));


		pGDI->SetSpace(oldSpace);
	}
}

// finalrankdlg.cpp:219
bool FinalRankCompare(const void* a, const void* b) {
	const sGameResultInfo* pA = (const sGameResultInfo*)a;
	const sGameResultInfo* pB = (const sGameResultInfo*)b;

	if (pA->m_sortGroup > pB->m_sortGroup) {
		return true;
	} else if (pA->m_sortGroup < pB->m_sortGroup) {
		return false;
	}

	if (pA->m_score < pB->m_score) {
		return true;
	} else if (pA->m_score > pB->m_score) {
		return false;
	}

	if (pA->m_pang > pB->m_pang) {
		return true;
	} else if (pA->m_pang < pB->m_pang) {
		return false;
	}

	return pA->m_value13F < pB->m_value13F;
}
