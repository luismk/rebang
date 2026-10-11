// Original: d:\build\custom\temp\code\client\projectg\mapselectdlg.cpp
#include "MapSelectDlg.h"
#include "ObjectFactory.h"
#include "frlistbox.h"
#include "../shared/classdefine.h"
#include "../shared/ItemManager.h"
#include "SharedDoc.h"
#include "Packet.h"
#include "TaskManager.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "commonutil.h"
#include <stdio.h>

// mapselectdlg.cpp:18 (run-time type and factory registration of FrMapSelectDlg)
const WRTTI FrMapSelectDlg::m_RTTI("FrMapSelectDlg", &FrForm::m_RTTI);

IObject* FrMapSelectDlgMakeInstance(void) {
	return new FrMapSelectDlg;
}

struct __sFrMapSelectDlg {
	__sFrMapSelectDlg(void) {
		ObjectFactory().AddObjectFunctor(FrMapSelectDlgMakeInstance, "FrMapSelectDlg");
	}
};
static __sFrMapSelectDlg s_registerFrMapSelectDlg;

// mapselectdlg.cpp:20
const sFRESH_MSGMAP* FrMapSelectDlg::GetMessageMap(void) const {
	return &FrMapSelectDlg::_MsgMap;
}

sFRESH_MSGMAP FrMapSelectDlg::_MsgMap = { &FrForm::_MsgMap, &FrMapSelectDlg::_MsgEntries[0] };

sFRESH_ENTRY FrMapSelectDlg::_MsgEntries[] = {
	// mapselectdlg.cpp:22
	{ "select", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMapSelectDlg::OnSelectInit },
	// mapselectdlg.cpp:23
	{ "select", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrMapSelectDlg::OnSelectBtnUp },
	// mapselectdlg.cpp:24
	{ "select", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrMapSelectDlg::OnSelectOwnerDraw },

	// mapselectdlg.cpp:26
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// mapselectdlg.cpp:30
FrMapSelectDlg::FrMapSelectDlg(void) {
	m_pSelect = 0;
	m_selectMap = 0;
	m_sendOnSelect = true;
	m_refuseCourse7F = false;
}

// mapselectdlg.cpp:38
FrMapSelectDlg::~FrMapSelectDlg(void) {
}

// mapselectdlg.cpp:42
void FrMapSelectDlg::OnSelectInit(intptr_t param) {
	m_pSelect = DYNAMIC_CAST(FrListBox, param);
	if (m_pSelect) {



		std::map<unsigned int, IFF_STRUCT::sCourse>& courseMap = ItemManager()->m_courseMap;

		int count = 0;
		FrListItem* pSelItem = 0;
		int i;



		if (IsLocalContent((localContentType_t)0x3A) && (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800)) {


			for (i = 0; i < 18; i++) {

				IFF_STRUCT::sCourse* pCourse = &courseMap[0x28000000 | s_mapSelectCourseOrder[i]];

				if ((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && (pCourse->courseFlags & 0x0F) == 3)
					continue;
				if (pCourse && (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && !WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(pCourse->m_typeId & 0x3FFFFFF))
					continue;


				FrListItem* pItem = m_pSelect->AddItem(pCourse);
				count++;

				if (!pSelItem && m_selectMap == (pCourse->m_typeId & 0x3FFFFFF))
					pSelItem = pItem;
			}

			for (i = 0; i < 18; i++) {

				IFF_STRUCT::sCourse* pCourse = &courseMap[0x28000000 | s_mapSelectCourseOrder[i]];

				if (pCourse && (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(pCourse->m_typeId & 0x3FFFFFF)) {

					if ((pCourse->courseFlags & 0x0F) != 3)
						continue;
				}



				FrListItem* pItem = m_pSelect->AddItem(pCourse);
				count++;

				if (!pSelItem && m_selectMap == (pCourse->m_typeId & 0x3FFFFFF))
					pSelItem = pItem;
			}

		} else {

			for (i = 0; i < 18; i++) {

				IFF_STRUCT::sCourse* pCourse = &courseMap[0x28000000 | s_mapSelectCourseOrder[i]];


				if (!pCourse->m_active)
					continue;



				if ((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x80) && (pCourse->courseFlags & 0x0F) == 1)
					continue;


				FrListItem* pItem = m_pSelect->AddItem(pCourse);
				count++;

				if (!pSelItem && m_selectMap == (pCourse->m_typeId & 0x3FFFFFF))
					pSelItem = pItem;
			}
		}




		int pad;
		if (count % 3 == 0)
			pad = 0;
		else
			pad = 3 - count % 3;

		for (i = 0; i < pad; i++) {

			m_pSelect->AddItem(0);
		}



		m_pSelect->SelectItem(pSelItem, true);
	}
}

// mapselectdlg.cpp:132
void FrMapSelectDlg::SetSelectMap(int map) {
	if (m_pSelect) {




		for (FrListBox::ITEM_LIST::iterator it = m_pSelect->m_itemList.begin(); it != m_pSelect->m_itemList.end(); ++it) {

			IFF_STRUCT::sCourse* pCourse = (IFF_STRUCT::sCourse*)(*it)->pData;



			if (pCourse) {


				if (map == (pCourse->m_typeId & 0x3FFFFFF)) {

					m_selectMap = map;
					m_pSelect->SelectItem(*it, true);
					break;
				}
			}
		}
	}
}

// mapselectdlg.cpp:157
void FrMapSelectDlg::OnSelectBtnUp(void) {
	FrListItem* pItem = m_pSelect->GetSelected();

	if (pItem) {

		IFF_STRUCT::sCourse* pCourse = (IFF_STRUCT::sCourse*)pItem->pData;

		if (pCourse) {


			if (((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && (pCourse->courseFlags & 0x0F) >= 3) || (m_refuseCourse7F && (pCourse->m_typeId & 0x3FFFFFF) == 0x7F)) {

				m_pSelect->ToggleItem(pItem);
				return;
			}


			if (IsLocalContent((localContentType_t)0x3A)) {

				if ((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && !WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(pCourse->m_typeId & 0x3FFFFFF)) {

					m_pSelect->ToggleItem(pItem);
					return;
				}
			}



			if (m_sendOnSelect && (!stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM") || !stricmp(g_pFresh->GetManager()->GetLayoutID(), "GAMEROOM_EXT") || !stricmp(g_pFresh->GetManager()->GetLayoutID(), "AVATARCHAT_MAIN"))) {

				WSendPacket packet((enumClientPacket)0x0A);
				packet.Encode2(0xFFFF);
				packet.Encode1(1);
				packet.Encode1(3);
				packet.Encode1((unsigned char)pCourse->m_typeId);
				packet.Send(SEND_TO_DEFAULT);

				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
			} else {

				m_selectMap = pCourse->m_typeId & 0x3FFFFFF;
			}


			Close(true);
		}
	}
}

// mapselectdlg.cpp:204
void FrMapSelectDlg::OnSelectOwnerDraw(intptr_t param) {
	FrListItem* pItem = (FrListItem*)param;

	if (pItem) {


		FrWndManager* pManager = g_pFresh->GetManager();
		FrGraphicInterface* pGDI = pManager->GetGDI();
		if (pGDI) {



			IFF_STRUCT::sCourse* pCourse = (IFF_STRUCT::sCourse*)pItem->pData;
			const Bitmap* pBitmap;
			if (pCourse)
				pBitmap = pManager->GetBitmap("ITEMS", MakeStr("mid_%s", pCourse->m_icon));
			else
				pBitmap = pManager->GetBitmap("ITEMS", "mid_map_hide");

			if (pBitmap) {




				pGDI->DrawTexture(pBitmap, WRect(0.0f, 0.0f, pBitmap->Width(), pBitmap->Height()), WRect(pItem->pos.x, pItem->pos.y, pBitmap->Width(), pBitmap->Height()), 0xFFFFFFFF, 0);


























				if (IsLocalContent((localContentType_t)0x39) && pCourse) {

					unsigned long rate = WSingleton<CSharedDoc>::Instance()->GetMapEventPangRate(pCourse->m_typeId & 0x3FFFFFF);

					char name[128] = { 0, };
					sprintf(name, "mid_pangrate_%d", rate);
					const Bitmap* pRate = g_pFresh->GetBitmap(name);

					if (pRate) {



						pGDI->DrawTexture(pRate, WRect(pItem->pos.x + 5.0f, pItem->pos.y + ((float)pBitmap->Height() - (float)pRate->Height() - 2.0f), (float)pRate->Width(), (float)pRate->Height()), 0xFFFFFFFF, 0);
					}
				}
			}






			if (pCourse && (((WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && (pCourse->courseFlags & 0x0F) >= 3) || (m_refuseCourse7F && (pCourse->m_typeId & 0x3FFFFFF) == 0x7F))) {

				const Bitmap* pLock = g_pFresh->GetManager()->GetBitmap("ITEMS", "map_lock");

				if (pLock) {




					pGDI->DrawTexture(pLock, WRect(0.0f, 0.0f, (float)pLock->Width(), (float)pLock->Height()), WRect(pItem->pos.x, pItem->pos.y, (float)pLock->Width(), (float)pLock->Height()), 0xFFFFFFFF, 0);
				}
			}



			if (IsLocalContent((localContentType_t)0x3A)) {

				if (pCourse && (WSingleton<CSharedDoc>::Instance()->m_currentChannel.flags & 0x800) && !WSingleton<CSharedDoc>::Instance()->CanUseRookieChannelMap(pCourse->m_typeId & 0x3FFFFFF)) {

					const Bitmap* pLock = g_pFresh->GetManager()->GetBitmap("ITEMS", "map_lock");

					if (pLock) {




						pGDI->DrawTexture(pLock, WRect(0.0f, 0.0f, (float)pLock->Width(), (float)pLock->Height()), WRect(pItem->pos.x, pItem->pos.y, (float)pLock->Width(), (float)pLock->Height()), 0xFFFFFFFF, 0);
					}
				}
			}

			if (pItem->selected && pItem->pData) {

				const Bitmap* pSelect = g_pFresh->GetManager()->GetBitmap("RANKING", "map_select");
				if (pSelect)
					pGDI->DrawTexture(pSelect, WRect(pItem->pos.x - 6.0f, pItem->pos.y - 6.0f, (float)pSelect->Width(), (float)pSelect->Height()), 0xFFFFFFFF, 0);
			}
		}
	}
}
