// Original: d:\build\custom\temp\code\client\projectg\realmyroommainui.cpp
#define REALMYROOMMAINUI_EXHIBITION_INLINE
#include "NetworkSystem.h"
#include "RealMyRoomMain.h"
#include "ClientSetting.h"
#include "RealMyRoom_Avatar.h"
#include "RealMyRoomData.h"
#include "exhibition.h"
#include "Post_SendDlg.h"
#include "Post_ReciveDlg.h"
#include "UccDrawDlg.h"
#include "ItemStorage.h"
#include "globalnetworkdefine.h"
#include "Packet.h"
#include "FriendDlg.h"
#include "FortuneDlg.h"
#include "FortuneOpenDlg.h"
#include "PangGiftDlg.h"
#include "CaddieInfoDlg.h"
#include "EnchantDlg.h"
#include "FrRandomBoxProgress.h"
#include "CCardBook.h"
#include "s5/RentalExtensionDlg.h"
#include "s5/Utilities.h"
#include "classdefine.h"
#include "frwnd.h"
#include "frcontextmenuctrl.h"
#include "frform.h"
#include "frarea.h"
#include "frbutton.h"
#include "frlistbox.h"
#include "frgaugebar.h"
#include "commonutil.h"
#include "Localize.h"
#include "ToolTip.h"
#include "frtabbutton.h"
#include "frscrollbar.h"
#include "fresh.h"
#include "SharedDoc.h"
#include "../shared/classdefine.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/woverlay.h"
#include "frgraphicinterface.h"
#include "../shared/globalgamedefine.h"
#include "frdesktop.h"
#include "CardBoxDlg.h"
#include "cardsystem.h"
#include "contentsdoc.h"
#include "frgroupbox.h"
#include "frstatic.h"
#include "frviewer.h"
#include "avatar_task.h"
#include "fredit.h"
#include "NoteDlg.h"
#include "MascotInfoDlg.h"
#include "UccManager.h"
#include "SoundManager.h"
#include "wutil.h"

// realmyroommain.h:1404
bool CRealMyRoomMain::IsItemStorageVisible(void) const {
	return m_itemStorageVisible;
}

// realmyroom_avatar.h:80
CPetFrame* CRealMyRoom_Avatar::GetPetFrame() {
	return m_pPetFrame;
}

// realmyroom_avatar.h:84
const char* CRealMyRoom_Avatar::GetIDName() const {
	return m_idName;
}

// realmyroom_avatar.h:85
const char* CRealMyRoom_Avatar::GetNickName() const {
	return m_nickName;
}

// realmyroom_avatar.h:94
const WVector& CRealMyRoom_Avatar::GetTargetPosition() const {
	return m_targetPosition;
}

// realmyroom_avatar.h:120
void CRealMyRoom_Avatar::SubState(unsigned long state) {
	SetState(m_state & ~state);
}

// fortunedlg.h:16
unsigned long FrFortuneDlg::GetTypeId(void) {
	return m_typeId;
}

// fortuneopendlg.h:99
unsigned long FrFortuneOpenDlg::GetTypeId(void) {
	return m_typeId;
}

// enchantdlg.h:24
void FrEnchantDlg::SetResultCode(unsigned char code) { m_resultCode = code; }

// panggiftdlg.h:17
sGiftInfo* FrPangGiftDlg::GetItemInfo(void) {
	return m_itemInfo;
}

// ccardbook.h:69
sCards* CCardSlot::GetCards(void) {
	return m_pCards;
}

// ccardbook.h:130
bool ICardBookPage::BoundSlotControl(unsigned char slot, FrButton* pButton) {
	bool result = false;
	if (m_pCardSlots != NULL) {
		result = m_pCardSlots[slot].BoundControl(pButton);
	}
	return result;
}

// ccardbook.h:140
CCardSlot* ICardBookPage::GetCardSlot(void) {
	return m_pCardSlots;
}

// ccardbook.h:324
CCardPackPage* CCardBook::GetCardPackPage(void) {
	return m_pCardPackPage;
}

// ccardbook.h:329
CCardPage* CCardBook::GetCardPage(void) {
	return m_pCardPage;
}

// ccardbook.h:334
void CCardBook::SetSelectedSlot(unsigned char slot) {
	m_selectedSlot = slot;
}

// ccardbook.h:350
bool CCardBook::IsShow(void) const {
	return m_show;
}

// ccardbook.h:313 (the instantiations this unit emits)
template bool CCardBook::BoundControl<FrArea*>(FrArea*& rControl, FrArea* pControl);
template bool CCardBook::BoundControl<FrListBox*>(FrListBox*& rControl, FrListBox* pControl);
template bool CCardBook::BoundControl<FrButton*>(FrButton*& rControl, FrButton* pControl);

// post_senddlg.h:35
void CPost_SendDlg::UpdateFriendList(void) {
	if (m_pFriendDlg) {
		m_pFriendDlg->UpdateFriendList();
	}
}

// post_senddlg.h:42
unsigned long CPost_SendDlg::GetSendUID(void) {
	return m_sendUID;
}

// uccdrawdlg.h:72
bool FrUccDrawDlg::IsInitialized(void) {
	return m_initialized;
}

// itemstorage.h:14
FrItemStorageNotice::FrItemStorageNotice(void) {
}

// itemstorage.h:15
FrItemStorageNotice::~FrItemStorageNotice(void) {
}

// itemstorage.h:130
FrItemStorageAccessPanel::eFormType FrItemStorageAccessPanel::GetFormType(void) const {
	return m_formType;
}

// itemstorage.h:451
bool CItemStorage::IsLock(void) const {
	return m_lock;
}

// itemstorage.h:489
unsigned long CItemStorage::GetTotalPage(void) const {
	return m_totalPage;
}

// itemstorage.h:497
unsigned long CItemStorage::GetCurPage(void) const {
	return m_curPage;
}

// itemstorage.h:501
CIndexControl* CItemStorage::GetIndexControl(void) {
	return m_indexControl;
}

// itemstorage.h:541
void CItemStorage::SetItemStorageArea(WRect rect) {
	m_itemStorageArea = rect;
}

// itemstorage.h:542
void CItemStorage::SetItemListArea(WRect rect) {
	m_itemListArea = rect;
}

// rentalextensiondlg.h:22
const sItemInfo* S5::FrRentalExtensionDlg::GetItemInfo(void) {
	return m_itemInfo;
}

// frrandomboxprogress.h:37
void FrRandomBoxProgress::SetBoxTid(unsigned long tid) { m_boxTid = tid; }

// classdefine.h:478
bool IFF_STRUCT::sItem::IsRandomBox(void) const {
	return isRandomBox;
}

// globalnetworkdefine.h:2981
unsigned long sStoredItemInfo::GetTypeID(void) const {
	return item.typeId;
}

// globalnetworkdefine.h:2984
unsigned long sStoredItemInfo::GetGuid(void) const {
	return item.guid;
}

// globalnetworkdefine.h:2991
sStoredItemInfo& sStoredItemInfo::operator=(const sStoredItemInfo& rhs) {
	slot = rhs.slot;
	item = rhs.item;

	return *this;
}

// clientsetting.h:420
void COption::mSetPCBangMascotMsg(const char* msg) {
	strncpy(m_option.m_pcBangMascotMsg, msg, sizeof(m_option.m_pcBangMascotMsg));
}

// realmyroommainui.cpp:108
void ReviseItemIconPoint(unsigned long typeId, WPoint& pt) {
	switch (typeId >> 26) {
	case 1:
	case 2:
	case 8:
	case 9:
	case 15:
	case 16:
	case 28:
		pt.x += 7.0f;
		pt.y += 2.0f;
		break;
	case 11:
		pt.x += 9.0f;
		pt.y += 12.0f;
		break;
	case 14:
		if ((typeId & 0x3C00000) == 0x800000) {
			pt.x += 7.0f;
			pt.y += 12.0f;
		} else {
			pt.x += 16.0f;
			pt.y += 27.0f;
		}
		break;
	case 31:
		pt.x += 16.0f;
		pt.y += 12.0f;
		break;
	case 18:
		pt.x += 21.0f;
		pt.y += 12.0f;
		break;
	default:
		pt.x += 16.0f;
		pt.y += 27.0f;
		break;
	}
}

// realmyroommainui.cpp:153
template <class T>
T* FindFrForm(const char* name) {
	FrWnd* pWnd = g_pFresh->GetDesktop()->FindChildForm(name);
	if (pWnd != NULL) {
		return DYNAMIC_CAST(T, pWnd);
	}

	return NULL;
}

// The instantiation OnCardUseLBDown uses (not written yet).
template FrCardBoxDlg* FindFrForm<FrCardBoxDlg>(const char* name);

// realmyroommainui.cpp:171
const sFRESH_MSGMAP* CRealMyRoomMain::GetMessageMap(void) const {
	return &CRealMyRoomMain::_MsgMap;
}

// realmyroommainui.cpp:976
void SetMainTabImg(const char* imgs[], int index) {
	imgs[0] = MakeStr("stab_%d_n", index);
	imgs[1] = MakeStr("stab_%d_d", index);
	imgs[2] = MakeStr("stab_%d_d", index);
}

// realmyroommainui.cpp:983
void SetSubTabImg(const char* imgs[], int index) {
	imgs[0] = MakeStr("stext_%d_n", index);
	imgs[1] = MakeStr("stext_%d_n", index);
	imgs[2] = MakeStr("stext_%d_n", index);
}

// realmyroommainui.cpp:991
bool EmptyCompares(const void* p1, const void* p2) {
	if (p1 != NULL) {
		if (p2 == NULL) {
			return true;
		}
	}

	return false;
}

// realmyroommainui.cpp:1258
void CRealMyRoomMain::OnAnotherHand(int nWork, void* pData) {
	if (nWork == 0) {
		ShowPet(*(unsigned long*)pData);
	}
}

// realmyroommainui.cpp:1265
void CRealMyRoomMain::OpenLayout(const char* layout) {
	WSendPacket packet(0x5C);
	packet.Send(SEND_TO_DEFAULT);

	g_pFresh->OpenLayout(layout, this, false);
}

// realmyroommainui.cpp:1273
void CRealMyRoomMain::CloseLayout(void) {
	if (m_pLayoutWnd) {
		m_pLayoutWnd->Close(true);
	}

	g_pFresh->CloseLayout();
}

// realmyroommainui.cpp:1443
void CRealMyRoomMain::HidePet(bool bWait) {
	if (bWait) {
		m_petLoader.WaitUntilWorkDone(9999);
	}

	if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = 0; }
}

// realmyroommainui.cpp:1456
void CRealMyRoomMain::OnMyRoomInit(int param) {
	m_initList.clear();

	SetConstantBGM("myroom.mp3", false, false);
	TurnOff3DBackground();
}

// realmyroommainui.cpp:1464
void CRealMyRoomMain::OnMyRoomFinish(void) {
	m_titleArea->SetBgImg("title_myroom");

	m_pPetArea = m_pPetAreaWnd[2];

	Reinitialize(false);

	if (WSingleton<CSharedDoc>::Instance()->m_tutorialFinish) {
		this << MsgObject(0, 0xAA, 1, 0, 0, 0, 0);
	}
}

// realmyroommainui.cpp:1478
void CRealMyRoomMain::Reinitialize(bool bForce) {
	if (WSingleton<CSharedDoc>::Instance()->m_myUID == WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID) {
		ChangeCharacter(WSingleton<CSharedDoc>::Instance()->m_myCharacterId);
		ChangeClubset(WSingleton<CSharedDoc>::Instance()->m_myClubId);
		ChangeAztec(WSingleton<CSharedDoc>::Instance()->m_myBallId);

		if (bForce == false) {
			ChangeCaddie(WSingleton<CSharedDoc>::Instance()->m_myCaddieId);
			ChangeMascot(WSingleton<CSharedDoc>::Instance()->m_myMascotId);
		}

		ChangeMainTab(m_mainTab, true);
		ChangeSubTab((subTab_t)m_roomMode, true);
		m_statTab = (statTab_t)(m_mainTab > 2);
		SetLevelBar();
		m_selTab = (selTab_t)0;

		for (int i = 0; i < 6; i++) {
			m_displaySkin[i] = WSingleton<CSharedDoc>::Instance()->m_equipSkin[i];
		}

		for (int i = 0; i < 6; i++) {
			if (m_displaySkin[i] != 0) {
				IFF_STRUCT::sSkin* pSkin = ItemManager()->FindSkin(m_displaySkin[i]);
				if (pSkin != 0 && (m_displaySkin[i] & 0x3C00000) == 0) {
					m_skinParamA = *((unsigned char*)pSkin + 0xB8);
					m_skinParamB = *((unsigned char*)pSkin + 0xB9);
					m_skinSlotA = 0;
					m_skinSlotB = 0;
				}
			}
		}
	}
}

// realmyroommainui.cpp:1517
void CRealMyRoomMain::ReinitializePC(ePCBangItem item) {
	if (item == PCBANG_ITEM_CADDIE)
		ChangeCaddie(CSharedDoc::Instance()->m_realEquip[0]);
	else if (item == PCBANG_ITEM_MASCOT)
		ChangeMascot(CSharedDoc::Instance()->m_realEquip[0x1A]);
}

// realmyroommainui.cpp:2178
void CRealMyRoomMain::CloseAllDialogs(void) {
	FrWnd* pWnd = DYNAMIC_CAST(FrWnd, g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"));
	if (pWnd) {
		pWnd->Close(true);
	}

	CTaskMain::CloseAllDialogs();
}

// realmyroommainui.cpp:2189
void CRealMyRoomMain::OnMiddleUIInit(int param) {
	m_pMiddleUIWnd = DYNAMIC_CAST(FrGroupBox, param);
}

// realmyroommainui.cpp:2196
void CRealMyRoomMain::OnTabTxt1Init(int param) {
	m_pTabTxt[0] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:2203
void CRealMyRoomMain::OnTabTxt2Init(int param) {
	m_pTabTxt[1] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:2210
void CRealMyRoomMain::OnTabTxt3Init(int param) {
	m_pTabTxt[2] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:2217
void CRealMyRoomMain::OnSelectedNameInit(int param) {
	m_pSelectedName = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:2501
void CRealMyRoomMain::SetPetAreaRect(void) {
	switch (m_roomMode) {
	case 13:
	case 14:
	case 15:
		m_pPetArea = m_pPetAreaWnd[0];
		break;
	case 16:
		m_pPetArea = m_pPetAreaWnd[1];
		break;
	default:
		m_pPetArea = m_pPetAreaWnd[2];
		break;
	}
}

// realmyroommainui.cpp:2755
void CRealMyRoomMain::ChangeStatTab(statTab_t tab) {
	m_statTab = tab;
	SetLevelBar();
}

// realmyroommainui.cpp:2761
void CRealMyRoomMain::ChangeSelTab(selTab_t tab) {
	m_selTab = tab;
}

// realmyroommainui.cpp:2780
void CRealMyRoomMain::CharacterViewControl(bool bShow) {
	if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar != NULL) {
		if (bShow == true) {
			WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetState(2);
			WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetHide(false);
		} else {
			WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->ChangeCharacterInfo();
			WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetState(0);
			WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetHide(!bShow);
		}
	}
}

// realmyroommainui.cpp:3235
void CRealMyRoomMain::ChangeAztec(unsigned long typeId) {
	m_aztecTypeId = typeId;
	WSingleton<CSharedDoc>::Instance()->m_myBallId = typeId;

	if (m_roomMode == 7) {
		SafeLoadPet(&m_aztecTypeId, false);
		TurnOffItemStatus();

		m_selItemOn = false;
		m_pSelItemPrevButton->Enable(false);
		m_pSelItemNextButton->Enable(false);

		std::list<sItemInfo>& gears = WSingleton<CSharedDoc>::Instance()->m_myGearList;
		std::list<sItemInfo>::iterator it;
		for (it = gears.begin(); it != gears.end(); ++it) {
			if (it->m_typeId == typeId) {
				if (++it != gears.end()) {
					m_pSelItemNextButton->Enable(true);
				}
				break;
			}
		}

		std::list<sItemInfo>& gears2 = WSingleton<CSharedDoc>::Instance()->m_myGearList;
		std::list<sItemInfo>::reverse_iterator rit;
		for (rit = gears2.rbegin(); rit != gears2.rend(); ++rit) {
			if (rit->m_typeId == typeId) {
				if (++rit != gears2.rend()) {
					m_pSelItemPrevButton->Enable(true);
				}
				break;
			}
		}
	}
}

// realmyroommainui.cpp:3274
const Bitmap* CRealMyRoomMain::GetItemIcon(unsigned long typeId) {
	const Bitmap* pIcon = NULL;

	std::map<unsigned long, const Bitmap*>::iterator it = m_itemIconMap.find(typeId);
	if (it == m_itemIconMap.end()) {
		IFF_ITEM_COMMON* pItem = ItemManager()->FindCommonItem(typeId);
		if (pItem != NULL) {
			pIcon = g_pFresh->RegisterBitmap(pItem->m_icon);
			if (pIcon != NULL) {
				m_itemIconMap.insert(std::map<unsigned long, const Bitmap*>::value_type(pItem->m_typeId, pIcon));
			}
		}
	} else {
		pIcon = it->second;
	}

	return pIcon;
}

// realmyroommainui.cpp:3739
void CRealMyRoomMain::OnPetInit(int param) {
	m_pPetAreaWnd[2] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:3744
void CRealMyRoomMain::OnSkinPetInit(int param) {
	m_pPetAreaWnd[0] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:3749
void CRealMyRoomMain::OnSkinPet2Init(int param) {
	m_pPetAreaWnd[1] = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:3985
void CRealMyRoomMain::OnFurnitureArrangeInit(int param) {
	m_pFurnitureArrangeButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureArrangeButton) {
		m_pFurnitureArrangeButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:3994
void CRealMyRoomMain::OnFurnitureArrangeButtonUP(void) {
	if (!m_flags[3]) {
		ControlUiHideMode(4);
	} else {
		ControlUiHideMode(2);
	}
}

// realmyroommainui.cpp:4003
void CRealMyRoomMain::OnItemDescBgInit2(int param) {
	m_pItemDescBg2 = DYNAMIC_CAST(FrViewer, param);
	if (m_pItemDescBg2) {
		m_pItemDescBg2->SetVisible(false);
	}
}

// realmyroommainui.cpp:4058
void CRealMyRoomMain::OnItemDescBgInit(int param) {
	m_pItemDescBg = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:4091
void CRealMyRoomMain::OnItemDescInit2(int param) {
	m_pItemDescEdit2 = DYNAMIC_CAST(FrEdit, param);
	if (m_pItemDescEdit2) {
		m_pItemDescEdit2->SetVisible(false);
		m_itemDescEdit2Pos = WPoint(m_pItemDescEdit2->GetRect().x, m_pItemDescEdit2->GetRect().y + m_pItemDescEdit2->GetRect().h);
		m_itemDescEdit2InitPos = m_itemDescEdit2Pos;
	}
}

// realmyroommainui.cpp:4103
void CRealMyRoomMain::OnItemNameInit(int param) {
	m_pItemNameEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pItemNameEdit) {
		m_pItemNameEdit->SetVisible(false);
	}
}

// realmyroommainui.cpp:4113
void CRealMyRoomMain::OnItemDescInit(int param) {
	m_pItemDescEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pItemDescEdit) {
		m_pItemDescEdit->SetVisible(false);
		m_itemDescEditPos = WPoint(m_pItemDescEdit->GetRect().x, m_pItemDescEdit->GetRect().y + m_pItemDescEdit->GetRect().h);
	}
}

// realmyroommainui.cpp:4123
void CRealMyRoomMain::SetItemDesc(void) {
	m_pItemDescEdit->ClearLine();

	switch (m_roomMode) {
	case 6:
	case 10:
		m_itemDescPos = WPoint(0.0f, 30.0f);
		break;
	case 7:
	case 9:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 23:
	case 27:
	case 28:
	case 29:
	case 30:
	case 31:
	case 32:
		m_itemDescPos = WPoint(0.0f, 120.0f);
		break;
	default:
		m_itemDescPos = WPoint(0.0f, 0.0f);
		break;
	}
}

// realmyroommainui.cpp:4160
void CRealMyRoomMain::OnItemSlotBoxInit(int param) {
	m_pItemSlotBox = DYNAMIC_CAST(FrGroupBox, param);
	if (m_pItemSlotBox) {
		m_pItemSlotBox->SetVisible(false);

		if (IsLocalContent((localContentType_t)0x71) == true) {
			CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
			if (pItemStorage != NULL) {
				pItemStorage->AddBackGroundUI(m_pItemSlotBox, false);
			}
		}
	}
}

// realmyroommainui.cpp:4178
void CRealMyRoomMain::OnEquipCancelInit(int param) {
	m_pEquipCancelButton = DYNAMIC_CAST(FrButton, param);
	m_pEquipCancelButton->SetPushDelay(0.0f);
	m_pEquipCancelButton->EnableHover(true, 0.0f);

	if (IsLocalContent((localContentType_t)0x71) == true) {
		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
		if (pItemStorage != NULL) {
			pItemStorage->AddBackGroundUI(m_pEquipCancelButton, false);
		}
	}
}

// realmyroommainui.cpp:4256
void CRealMyRoomMain::OnEquipResetInit(int param) {
	m_pEquipResetButton = DYNAMIC_CAST(FrButton, param);
	m_pEquipResetButton->SetPushDelay(0.0f);
	m_pEquipResetButton->EnableHover(true, 0.0f);

	if (IsLocalContent((localContentType_t)0x71) == true) {
		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
		if (pItemStorage != NULL) {
			pItemStorage->AddBackGroundUI(m_pEquipResetButton, false);
		}
	}
}

// realmyroommainui.cpp:4272
void CRealMyRoomMain::OnEquipResetUp(void) {
	if (m_pItemSlotList != NULL) {
		for (FrListBox::ITEM_LIST::iterator it = m_pItemSlotList->m_itemList.begin(); it != m_pItemSlotList->m_itemList.end(); ++it) {
			FrListItem* pItem = *it;
			unsigned long* pItemId = (unsigned long*)pItem->pData;
			if (pItemId != NULL) {
				UnequipItem(pItemId);
			}
		}
	}

	CIconToolTip::GetInstance()->ClearToolTip();
}

// realmyroommainui.cpp:4288
void CRealMyRoomMain::OnItemSlotInit(intptr_t param) {
	m_pItemSlotList = DYNAMIC_CAST(FrListBox, param);
}

// realmyroommainui.cpp:4338
void CRealMyRoomMain::OnItemSlotBtnDown(void) {
}

// realmyroommainui.cpp:4345
void CRealMyRoomMain::OnItemSlotDClick(void) {
	if (IsLocalContent((localContentType_t)0x71) && m_itemStorageVisible) {
		return;
	}

	FrListItem* pItem = m_pItemSlotList->GetItemUnderCursor();
	if (pItem == 0) {
		return;
	}

	unsigned long* pItemId = (unsigned long*)pItem->pData;
	if (pItemId == 0) {
		return;
	}

	UnequipItem(pItemId);
}

// realmyroommainui.cpp:4361
void CRealMyRoomMain::OnGiftBgInit(int param) {
	m_pGiftBgViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_pGiftBgViewer) {
		m_pGiftBgViewer->SetVisible(false);
		m_pGiftBgViewer->SetMouseEvent(false);
	}
}

// realmyroommainui.cpp:4371
void CRealMyRoomMain::OnGiftDescInit(int param) {
	m_pGiftDescEdit = DYNAMIC_CAST(FrEdit, param);
	if (m_pGiftDescEdit) {
		m_pGiftDescEdit->SetVisible(false);
	}
}

// realmyroommainui.cpp:3293
void CRealMyRoomMain::DrawSelItem(FrGraphicInterface* pGI, unsigned long typeId) {
	WRect rect = m_pSelItemWnd->GetRect();

	const Bitmap* pIcon = GetItemIcon(typeId);
	if (pIcon != NULL) {
		pGI->DrawTexture(pIcon, rect, 0xFFFFFFFF, 0);
	}
}

// realmyroommainui.cpp:4380
void CRealMyRoomMain::SetGiftDescString(const char* pSender, const char* pMessage, const char* pDate, const char* pTime) {
	m_pGiftDescEdit->ClearLine();
	if (pSender == NULL) {
		m_pGiftDescEdit->AddText(MakeStr("\xB5\xB5\xC2\xF8\xC7\xD1 \xBC\xB1\xB9\xB0\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", pSender), false, true);
		return;
	}

	m_pGiftDescEdit->AddText(MakeStr("\\c0xffffffff,t\\c\xB9\xDE\xB4\xC2\xC0\xCC\\c0xffffffff,n\\c : %s\xB4\xD4\xB2\xB2\xBC\xAD \xBC\xB1\xB9\xB0\xC0\xBB \xBA\xB8\xB3\xBB\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.", pSender), false, true);

	m_pGiftDescEdit->AddText(MakeStr("\\c0xffffffff,t\\c\xB3\xBB\xBF\xEB\\c0xffffffff,n\\c : %s", pMessage), false, true);

	m_pGiftDescEdit->AddText(MakeStr("\\c0xffffffff,t\\c\xBA\xB8\xB3\xBD\xB3\xAF\xC2\xA5\\c0xffffffff,n\\c : %s", pDate), false, true);
	m_pGiftDescEdit->AddText(MakeStr("\\c0xffffffff,t\\c\xBA\xB8\xB3\xBD\xBD\xC3\xB0\xA3\\c0xffffffff,n\\c : %s", pTime), false, true);
}

// realmyroommainui.cpp:4414
void CRealMyRoomMain::OnStatFrameInit(int param) {
	m_pStatFrame = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:4422
void CRealMyRoomMain::OnStatTextInit(int param) {
	m_pStatText = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:4430
void CRealMyRoomMain::OnPowerBarInit(intptr_t param) {
	m_pPowerBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pPowerBar->SetRange(0, 50, 0);
}

// realmyroommainui.cpp:4436
void CRealMyRoomMain::OnControlBarInit(intptr_t param) {
	m_pControlBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pControlBar->SetRange(0, 30, 0);
}

// realmyroommainui.cpp:4442
void CRealMyRoomMain::OnImpactBarInit(intptr_t param) {
	m_pImpactBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pImpactBar->SetRange(0, 30, 0);
}

// realmyroommainui.cpp:4448
void CRealMyRoomMain::OnSpinBarInit(intptr_t param) {
	m_pSpinBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pSpinBar->SetRange(0, 30, 0);
}

// realmyroommainui.cpp:4454
void CRealMyRoomMain::OnCurveBarInit(intptr_t param) {
	m_pCurveBar = DYNAMIC_CAST(FrGaugeBar, param);
	m_pCurveBar->SetRange(0, 30, 0);
}

// realmyroommainui.cpp:4460
void CRealMyRoomMain::TurnOnItemSelect(void) {
	m_pItemSelectWnd->SetVisible(true);
	m_pSelItemPrevButton->SetVisible(true);
	m_pSelItemNextButton->SetVisible(true);
}

// realmyroommainui.cpp:4467
void CRealMyRoomMain::TurnOffItemSelect(void) {
	m_pItemSelectWnd->SetVisible(false);
	m_pSelItemPrevButton->SetVisible(false);
	m_pSelItemNextButton->SetVisible(false);
}

// realmyroommainui.cpp:4474
void CRealMyRoomMain::TurnOnStat(void) {
	m_pStatWnd->SetVisible(true);
}

// realmyroommainui.cpp:4479
void CRealMyRoomMain::TurnOffStat(void) {
	m_pStatWnd->SetVisible(false);
}

// realmyroommainui.cpp:4484
void CRealMyRoomMain::OnSelFrameInit(int param) {
	m_pSelFrame = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:4491
void CRealMyRoomMain::OnStatBoxInit(int param) {
	m_pStatWnd = DYNAMIC_CAST(FrGroupBox, param);
}

// realmyroommainui.cpp:4498
void CRealMyRoomMain::OnSelItemIconInit(int param) {
	m_pSelItemWnd = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:4543
void CRealMyRoomMain::OnSelItemInit(int param) {
	m_pItemSelectWnd = DYNAMIC_CAST(FrButton, param);
	if (m_pItemSelectWnd) {
		m_pItemSelectWnd->EnableBlink(true);
	}
}

// realmyroommainui.cpp:4558
void CRealMyRoomMain::OnSelItemOwnerDraw(int param) {
}

// realmyroommainui.cpp:4564
void CRealMyRoomMain::OnSelItemUp(void) {
	m_selItemOn = !m_selItemOn;
}

// realmyroommainui.cpp:4574
void CRealMyRoomMain::OnSelItemPrevInit(int param) {
	m_pSelItemPrevButton = DYNAMIC_CAST(FrButton, param);
	if (m_pSelItemPrevButton) {
		m_pSelItemPrevButton->SetPushDelay(0.1f);
	}
}

// realmyroommainui.cpp:4662
void CRealMyRoomMain::OnSelItemNextInit(int param) {
	m_pSelItemNextButton = DYNAMIC_CAST(FrButton, param);
	if (m_pSelItemNextButton) {
		m_pSelItemNextButton->SetPushDelay(0.1f);
	}
}

// realmyroommainui.cpp:4750
void CRealMyRoomMain::OnStatPartInit(int param) {
	m_pItemStatusWnd = DYNAMIC_CAST(FrViewer, param);
}

// realmyroommainui.cpp:4757
void CRealMyRoomMain::OnStatPartUp(void) {
	ChangeStatTab(STAT_TAB_PART);
}

// realmyroommainui.cpp:4787
void CRealMyRoomMain::OnStatAllInit(int param) {
	m_pStatAllViewer = DYNAMIC_CAST(FrViewer, param);
}

// realmyroommainui.cpp:4794
void CRealMyRoomMain::OnStatAllUp(void) {
	ChangeStatTab(STAT_TAB_ALL);
}

// realmyroommainui.cpp:4799
void CRealMyRoomMain::OnStatAllOwnerDraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI == NULL) {
		return;
	}

	WRect rect = m_pStatAllViewer->GetRect();
	int state = 0;
	if (m_pStatAllViewer->IsVisible()) {
		if (m_statTab == STAT_TAB_ALL) {
			state = 2;
		} else {
			const WPoint& pt = g_pFresh->GetManager()->GetMousePos();
			if (rect.IsInRect(pt)) {
				state = 1;
			}
		}

		pGI->DrawTexture(m_pStatAllBitmap[state], rect, 0xFFFFFFFF, 0);
	}
}

// realmyroommainui.cpp:4924
void CRealMyRoomMain::OnTabSubInit(int param) {
	m_pSubTab = DYNAMIC_CAST(FrTabButton, param);
}

// realmyroommainui.cpp:4932
void CRealMyRoomMain::OnTabSubButtonUp(void) {
	TButtonItem* pItem = m_pSubTab->GetSelected();
	if (pItem == NULL) {
		return;
	}

	ChangeSubTab(static_cast<subTab_t>(pItem->index), false);
}

// realmyroommainui.cpp:5301
void CRealMyRoomMain::OnCardBookInit(int param) {
	FrArea* p = DYNAMIC_CAST(FrArea, param);

	if (p != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pBookWnd, p);
	}
}

// realmyroommainui.cpp:5311
void CRealMyRoomMain::OnCardBookOwnerDraw(int param) {
	if (m_pCardBook) {
		m_pCardBook->Draw();
	}
}

// realmyroommainui.cpp:5319
void CRealMyRoomMain::SetCardBookVisible(bool bVisible) {
	if (m_pCardBook) {
		m_pCardBook->Show(bVisible);
		m_cardBookVisible = bVisible;
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(bVisible);
		m_pCardPackListBox->UnselectAllItem();
		if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
	}
}

// realmyroommainui.cpp:5335
void CRealMyRoomMain::BuildCardPackList(void) {
	if (m_pCardPackListBox) {
		m_pCardPackListBox->ClearItem();

		std::list<sCards>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myCardList.begin();
		while (it != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {
			sCards* pCards = &(*it);

			// Card packs only (item groups 3 and 4).
			if (((pCards->m_typeId >> 22) & 0xF) == 4 || ((pCards->m_typeId >> 22) & 0xF) == 3) {
				m_pCardPackListBox->AddItem(pCards);
			}
			++it;
		}

		if (WSingleton<CSharedDoc>::Instance()->m_myCardList.size() % 2) {
			m_pCardPackListBox->AddItem(NULL);
		}

		int count = 6 - m_pCardPackListBox->GetCurrentItemSize(true);
		for (int i = 0; i < count; i++) {
			m_pCardPackListBox->AddItem(NULL);
		}

		m_pCardPackListBox->UseRightButton(true);
		m_pCardPackListBox->UseDummy(true);
		if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
	}
}

// realmyroommainui.cpp:5374
void CRealMyRoomMain::OnCardPackListInit(int param) {
	m_pCardPackListBox = DYNAMIC_CAST(FrListBox, param);
	BuildCardPackList();

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(false);
	}
}

// realmyroommainui.cpp:5459
void CRealMyRoomMain::OnCardPackListDown(void) {
	if (m_pCardPackListBox && m_roomMode == 27) {
		FrListItem* pItem = m_pCardPackListBox->GetItemUnderCursor();
		if (pItem == NULL || pItem->pData == NULL) {
			return;
		}

		if (m_petLoader.IsWorking(0)) {
			m_pCardPackListBox->ToggleItem(pItem);
			return;
		}

		if (pItem->selected) {
			g_audio->PlaySfx("ui_desktop_icon_click", 0, 0, 0, 0, 0.5f, 200.0f);

			sItemInfo* pInfo = (sItemInfo*)pItem->pData;
			if (m_pCardBook) {
				m_pCardBook->m_pPageWnd->SetVisible(true);
			}

			ShowPet(pInfo->m_typeId);
			return;
		}

		m_pCardPackListBox->SelectItem(pItem, true);
	}
}

// realmyroommainui.cpp:5494
void CRealMyRoomMain::OnCardPackListRbUp(void) {
	if (m_pCardPackListBox && m_roomMode == 27) {
		FrListItem* pItem = m_pCardPackListBox->GetItemUnderCursor();
		if (pItem == NULL || pItem->pData == NULL) {
			return;
		}

		if (m_petLoader.IsWorking(0)) {
			m_pCardPackListBox->ToggleItem(pItem);
			return;
		}

		if (pItem->selected) {
			OnCardPackListDown();
		} else {
			m_pCardPackListBox->ToggleItem(pItem);
		}

		sItemInfo* pInfo = (sItemInfo*)pItem->pData;
		OpenInformation(&pInfo->m_typeId, NULL, NULL);
	}
}

// realmyroommainui.cpp:5523
void CRealMyRoomMain::OnCardBookIndexInit(int param) {
	FrListBox* p = DYNAMIC_CAST(FrListBox, param);

	if (p != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pSubWnd, p);
	}
}

// realmyroommainui.cpp:5533
void CRealMyRoomMain::OnCardBookIndexLBDown(void) {
	if (m_pCardBook) {
		const WPoint& pt = g_pFresh->GetManager()->GetMousePos();
		m_pCardBook->PageIndexBehavior(pt, 1);
	}
}

// realmyroommainui.cpp:5543
void CRealMyRoomMain::OnCardBookIndexMouseOver(void) {
	if (m_pCardBook) {
		const WPoint& pt = g_pFresh->GetManager()->GetMousePos();
		m_pCardBook->PageIndexBehavior(pt, 2);
	}
}

// realmyroommainui.cpp:5553
void CRealMyRoomMain::OnCardUseInit(int param) {
	FrButton* p = DYNAMIC_CAST(FrButton, param);

	if (p != NULL) {
		if (m_pCardBook != NULL) {
			m_pCardBook->BoundControl(m_pCardBook->m_pPageWnd, p);
		}
	}
}

// realmyroommainui.cpp:5594
bool CRealMyRoomMain::OnCardBoxDlgResult(int result, FrForm* pForm) {
	return true;
}

// realmyroommainui.cpp:5604
void CRealMyRoomMain::OnCardPropertyInit(int param) {
	FrArea* p = DYNAMIC_CAST(FrArea, param);

	if (p != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pPropertyWnd, p);
	}
}

// realmyroommainui.cpp:5614
void CRealMyRoomMain::OnTabCardPackInit(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pTabButtons[0], pButton);
		pButton->SetPushDelay(0.0f);
	}
}

// realmyroommainui.cpp:5625
void CRealMyRoomMain::OnTabCardPackLBDown(void) {
	if (m_pCardBook) {
		m_pCardBook->ButtonTabControl(0);
		m_pCardBook->ShowSlotControl(true);
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(true);
		m_pCardPackListBox->UnselectAllItem();
	}
}

// realmyroommainui.cpp:5641
void CRealMyRoomMain::OnTabCardVolume1Init(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pTabButtons[1], pButton);
		pButton->SetPushDelay(0.0f);
	}
}

// realmyroommainui.cpp:5653
void CRealMyRoomMain::OnTabCardVolume1LBDown(void) {
	if (m_pCardBook) {
		m_pCardBook->ButtonTabControl(1);
		m_pCardBook->ShowSlotControl(true);
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(false);
	}

	if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
}

// realmyroommainui.cpp:5667
void CRealMyRoomMain::OnTabCardVolume2Init(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pTabButtons[2], pButton);
		pButton->SetPushDelay(0.0f);
	}
}

// realmyroommainui.cpp:5678
void CRealMyRoomMain::OnTabCardVolume2LBDown(void) {
	if (m_pCardBook) {
		m_pCardBook->ButtonTabControl(2);
		m_pCardBook->ShowSlotControl(true);
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(false);
	}

	if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
}

// realmyroommainui.cpp:5692
void CRealMyRoomMain::OnTabCardVolume3Init(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pTabButtons[3], pButton);
		pButton->SetPushDelay(0.0f);
	}
}

// realmyroommainui.cpp:5703
void CRealMyRoomMain::OnTabCardVolume3LBDown(void) {
	if (m_pCardBook) {
		m_pCardBook->ButtonTabControl(3);
		m_pCardBook->ShowSlotControl(true);
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(false);
	}

	if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
}

// realmyroommainui.cpp:5717
void CRealMyRoomMain::OnTabCardVolume4Init(int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);

	if (pButton != NULL && m_pCardBook != NULL) {
		m_pCardBook->BoundControl(m_pCardBook->m_pTabButtons[4], pButton);
		pButton->SetPushDelay(0.0f);
	}
}

// realmyroommainui.cpp:5728
void CRealMyRoomMain::OnTabCardVolume4LBDown(void) {
	if (m_pCardBook) {
		m_pCardBook->ButtonTabControl(4);
		m_pCardBook->ShowSlotControl(true);
	}

	if (m_pCardPackListBox) {
		m_pCardPackListBox->SetVisible(false);
	}

	if (m_pPetExhibition) { delete m_pPetExhibition; m_pPetExhibition = NULL; }
}

// realmyroommainui.cpp:5744
void CRealMyRoomMain::BoundCardPackPage(FrButton* pButton, unsigned char slot) {
	pButton->SetVisible(false);
	pButton->Enable(false);
}

// realmyroommainui.cpp:5749
void CRealMyRoomMain::OnCardPackInit1(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 0); }

// realmyroommainui.cpp:5750
void CRealMyRoomMain::OnCardPackInit2(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 1); }

// realmyroommainui.cpp:5751
void CRealMyRoomMain::OnCardPackInit3(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 2); }

// realmyroommainui.cpp:5752
void CRealMyRoomMain::OnCardPackInit4(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 3); }

// realmyroommainui.cpp:5753
void CRealMyRoomMain::OnCardPackInit5(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 4); }

// realmyroommainui.cpp:5754
void CRealMyRoomMain::OnCardPackInit6(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 5); }

// realmyroommainui.cpp:5755
void CRealMyRoomMain::OnCardPackInit7(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 6); }

// realmyroommainui.cpp:5756
void CRealMyRoomMain::OnCardPackInit8(int param) { BoundCardPackPage(DYNAMIC_CAST(FrButton, param), 7); }

// realmyroommainui.cpp:5760
void CRealMyRoomMain::CardPackBehavior(unsigned char slot, unsigned char button) {
	if (m_pCardBook) {
		if (!m_itemStorageVisible) {
			CCardPackPage* pPage = m_pCardBook->GetCardPackPage();

			if (pPage) {
				m_pCardBook->SetSelectedSlot(slot);
				pPage->SelectSlot(button, slot);

				if (m_pCardBook->m_pPageWnd) {
					if (pPage->IsSelectedSlot()) {
						m_pCardBook->m_pPageWnd->SetVisible(true);
					} else {
						m_pCardBook->m_pPageWnd->SetVisible(false);
					}
				}
			}
		}
	}
}

// realmyroommainui.cpp:5822
void CRealMyRoomMain::OnCardPackLBDown1(void) { CardPackBehavior(0, 1); }

// realmyroommainui.cpp:5823
void CRealMyRoomMain::OnCardPackLBDown2(void) { CardPackBehavior(1, 1); }

// realmyroommainui.cpp:5824
void CRealMyRoomMain::OnCardPackLBDown3(void) { CardPackBehavior(2, 1); }

// realmyroommainui.cpp:5825
void CRealMyRoomMain::OnCardPackLBDown4(void) { CardPackBehavior(3, 1); }

// realmyroommainui.cpp:5826
void CRealMyRoomMain::OnCardPackLBDown5(void) { CardPackBehavior(4, 1); }

// realmyroommainui.cpp:5827
void CRealMyRoomMain::OnCardPackLBDown6(void) { CardPackBehavior(5, 1); }

// realmyroommainui.cpp:5828
void CRealMyRoomMain::OnCardPackLBDown7(void) { CardPackBehavior(6, 1); }

// realmyroommainui.cpp:5829
void CRealMyRoomMain::OnCardPackLBDown8(void) { CardPackBehavior(7, 1); }

// realmyroommainui.cpp:5831
void CRealMyRoomMain::OnCardPackRBDown1(void) { CardPackBehavior(0, 2); }

// realmyroommainui.cpp:5832
void CRealMyRoomMain::OnCardPackRBDown2(void) { CardPackBehavior(1, 2); }

// realmyroommainui.cpp:5833
void CRealMyRoomMain::OnCardPackRBDown3(void) { CardPackBehavior(2, 2); }

// realmyroommainui.cpp:5834
void CRealMyRoomMain::OnCardPackRBDown4(void) { CardPackBehavior(3, 2); }

// realmyroommainui.cpp:5835
void CRealMyRoomMain::OnCardPackRBDown5(void) { CardPackBehavior(4, 2); }

// realmyroommainui.cpp:5836
void CRealMyRoomMain::OnCardPackRBDown6(void) { CardPackBehavior(5, 2); }

// realmyroommainui.cpp:5837
void CRealMyRoomMain::OnCardPackRBDown7(void) { CardPackBehavior(6, 2); }

// realmyroommainui.cpp:5838
void CRealMyRoomMain::OnCardPackRBDown8(void) { CardPackBehavior(7, 2); }

// realmyroommainui.cpp:5842
void CRealMyRoomMain::BoundCardPage(FrButton* pButton, unsigned char slot) {
	if (pButton && m_pCardBook) {
		CCardPage* pPage = m_pCardBook->GetCardPage();

		if (pPage) {
			pPage->BoundSlotControl(slot, pButton);
		}

		pButton->UseRightButton(true);
		if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true) {
			CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
			if (pItemStorage) {
				pItemStorage->AddBackGroundUI(pButton, false);
			}
		}
	}
}

// realmyroommainui.cpp:5865
void CRealMyRoomMain::OnCardInit1(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 0); }

// realmyroommainui.cpp:5866
void CRealMyRoomMain::OnCardInit2(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 1); }

// realmyroommainui.cpp:5867
void CRealMyRoomMain::OnCardInit3(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 2); }

// realmyroommainui.cpp:5868
void CRealMyRoomMain::OnCardInit4(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 3); }

// realmyroommainui.cpp:5869
void CRealMyRoomMain::OnCardInit5(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 4); }

// realmyroommainui.cpp:5870
void CRealMyRoomMain::OnCardInit6(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 5); }

// realmyroommainui.cpp:5871
void CRealMyRoomMain::OnCardInit7(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 6); }

// realmyroommainui.cpp:5872
void CRealMyRoomMain::OnCardInit8(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 7); }

// realmyroommainui.cpp:5873
void CRealMyRoomMain::OnCardInit9(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 8); }

// realmyroommainui.cpp:5874
void CRealMyRoomMain::OnCardInit10(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 9); }

// realmyroommainui.cpp:5875
void CRealMyRoomMain::OnCardInit11(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 10); }

// realmyroommainui.cpp:5876
void CRealMyRoomMain::OnCardInit12(int param) { BoundCardPage(DYNAMIC_CAST(FrButton, param), 11); }

// realmyroommainui.cpp:5879
void CRealMyRoomMain::CardBehavior(unsigned char slot, unsigned char button) {
	if (m_pCardBook) {
		if (!m_itemStorageVisible) {
			CCardPage* pPage = m_pCardBook->GetCardPage();

			if (pPage) {
				m_pCardBook->SetSelectedSlot(slot);
				pPage->SelectSlot(button, slot);

				if (m_pCardBook->m_pPageWnd) {
					if (pPage->IsSelectedSlot()) {
						if (pPage->GetCardSlot()[slot].GetCards()->m_typeId == 0x7C800026) {
							m_pCardBook->m_pPageWnd->SetVisible(false);
						} else {
							m_pCardBook->m_pPageWnd->SetVisible(true);
						}
					} else {
						m_pCardBook->m_pPageWnd->SetVisible(false);
					}
				}
			}
		}
	}
}

// realmyroommainui.cpp:5941
void CRealMyRoomMain::OnCardLBDown1(void) { CardBehavior(0, 1); }

// realmyroommainui.cpp:5942
void CRealMyRoomMain::OnCardLBDown2(void) { CardBehavior(1, 1); }

// realmyroommainui.cpp:5943
void CRealMyRoomMain::OnCardLBDown3(void) { CardBehavior(2, 1); }

// realmyroommainui.cpp:5944
void CRealMyRoomMain::OnCardLBDown4(void) { CardBehavior(3, 1); }

// realmyroommainui.cpp:5945
void CRealMyRoomMain::OnCardLBDown5(void) { CardBehavior(4, 1); }

// realmyroommainui.cpp:5946
void CRealMyRoomMain::OnCardLBDown6(void) { CardBehavior(5, 1); }

// realmyroommainui.cpp:5947
void CRealMyRoomMain::OnCardLBDown7(void) { CardBehavior(6, 1); }

// realmyroommainui.cpp:5948
void CRealMyRoomMain::OnCardLBDown8(void) { CardBehavior(7, 1); }

// realmyroommainui.cpp:5949
void CRealMyRoomMain::OnCardLBDown9(void) { CardBehavior(8, 1); }

// realmyroommainui.cpp:5950
void CRealMyRoomMain::OnCardLBDown10(void) { CardBehavior(9, 1); }

// realmyroommainui.cpp:5951
void CRealMyRoomMain::OnCardLBDown11(void) { CardBehavior(10, 1); }

// realmyroommainui.cpp:5952
void CRealMyRoomMain::OnCardLBDown12(void) { CardBehavior(11, 1); }

// realmyroommainui.cpp:5954
void CRealMyRoomMain::OnCardRBDown1(void) { CardBehavior(0, 2); }

// realmyroommainui.cpp:5955
void CRealMyRoomMain::OnCardRBDown2(void) { CardBehavior(1, 2); }

// realmyroommainui.cpp:5956
void CRealMyRoomMain::OnCardRBDown3(void) { CardBehavior(2, 2); }

// realmyroommainui.cpp:5957
void CRealMyRoomMain::OnCardRBDown4(void) { CardBehavior(3, 2); }

// realmyroommainui.cpp:5958
void CRealMyRoomMain::OnCardRBDown5(void) { CardBehavior(4, 2); }

// realmyroommainui.cpp:5959
void CRealMyRoomMain::OnCardRBDown6(void) { CardBehavior(5, 2); }

// realmyroommainui.cpp:5960
void CRealMyRoomMain::OnCardRBDown7(void) { CardBehavior(6, 2); }

// realmyroommainui.cpp:5961
void CRealMyRoomMain::OnCardRBDown8(void) { CardBehavior(7, 2); }

// realmyroommainui.cpp:5962
void CRealMyRoomMain::OnCardRBDown9(void) { CardBehavior(8, 2); }

// realmyroommainui.cpp:5963
void CRealMyRoomMain::OnCardRBDown10(void) { CardBehavior(9, 2); }

// realmyroommainui.cpp:5964
void CRealMyRoomMain::OnCardRBDown11(void) { CardBehavior(10, 2); }

// realmyroommainui.cpp:5965
void CRealMyRoomMain::OnCardRBDown12(void) { CardBehavior(11, 2); }

// realmyroommainui.cpp:6046
void CRealMyRoomMain::DrawStatus2(FrGraphicInterface* pGI, FrListItem* pItem, short* const status, short* const slotStatus, float x) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {
		if (status[i] != 0 || slotStatus[i] != 0) {
			pGI->SetTextColor(0xFF0000FF, 0xFFFFFFFF);
			pGI->Print11(WPoint(x + 182.0f, offsetY + x + 36.0f) + pItem->pos, 2, MakeStatusString(i, status[i], slotStatus[i]));
			offsetY += 14.0f;
		}
	}
}

// realmyroommainui.cpp:6077
void CRealMyRoomMain::DrawStatusCard(FrGraphicInterface* pGI, FrListItem* pItem, short* const cardStatus, float x) {
	float offsetY = 0.0f;
	for (int i = 0; i < 5; i++) {
		if (cardStatus[i] != 0) {
			pGI->SetTextColor(0xFF0000FF, 0xFFFFFFFF);
			pGI->Print11(WPoint(x + 182.0f, offsetY + x + 36.0f) + pItem->pos, 2, MakeCardStatusString(i, cardStatus[i]));
			offsetY += 14.0f;
		}
	}
}

// realmyroommainui.cpp:7367
bool CRealMyRoomMain::OnCardRemoveDlgResult(int result, FrForm* pForm) {
	m_pCardRemoveDlg = NULL;
	return true;
}

// realmyroommainui.cpp:7402
void OnOpenedBoxDlg(unsigned long typeId, FRESH_RESULT_CALLBACK pfnResult, FrCmdTarget* pOwner) {
	FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), pOwner, "notify_yesno", 0);
	pForm->SetMessage("\xBC\xB1\xC5\xC3\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBF\xAD\xBE\xEE \xBA\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);

	pForm->Open(pfnResult, 1);
}

// realmyroommainui.cpp:7411
void CRealMyRoomMain::OpenNewYearMoney(sItemInfo* pItemInfo) {
	if (pItemInfo != NULL) {
		m_newYearMoneySerial = pItemInfo->m_serial;
		OnOpenedBoxDlg(pItemInfo->m_typeId, (FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnOpenNewYearMoneyDlgResult, this);
	}
}

// realmyroommainui.cpp:7420
void CRealMyRoomMain::OpenEventGiftBox(sItemInfo* pItemInfo) {
	if (pItemInfo != NULL) {
		CGiftBoxEvent* pEvent = (CGiftBoxEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x1D);
		if (pEvent == NULL) {
			return;
		}

		m_eventGiftBoxSerial = pItemInfo->m_serial;

		pEvent->SetOpenedBoxTID(pItemInfo->m_typeId);

		if (pEvent->GetOpenedBoxTID() != 0) {
			OnOpenedBoxDlg(pItemInfo->m_typeId, (FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnOpenEventGiftDlgResult, this);
		}
	}
}

// realmyroommainui.cpp:7441
void CRealMyRoomMain::OpenS4EventGiftBox(sItemInfo* pItemInfo) {
	if (pItemInfo != NULL) {
		CGiftBoxEvent* pEvent = (CGiftBoxEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x1D);
		if (pEvent == NULL) {
			return;
		}

		m_s4EventGiftBoxTypeId = pItemInfo->m_typeId;

		pEvent->SetOpenedBoxTID(pItemInfo->m_typeId);

		if (pEvent->GetOpenedBoxTID() != 0) {
			OnOpenedBoxDlg(pItemInfo->m_typeId, (FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnOpenS4EventGiftDlgResult, this);
		}
	}
}

// realmyroommainui.cpp:7913
void CRealMyRoomMain::OnItemListMouseMove(void) {
	if (m_pUccCopyHideWnd->GetItemUnderCursor() != NULL) {
		if (!(m_itemStorageVisible ? m_petLoader.IsWorking(0) : (m_petLoader.IsWorking(0) || m_roomMode == 22))) {
			if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true) {
				CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
				if (pItemStorage) {
					pItemStorage->SetDrag(sStoredItemDragDrop::MODE_PUSH);
				}
			}
		}
	}
}

// realmyroommainui.cpp:8227
void CRealMyRoomMain::OnItemListDClick(void) {
	if (!IsLocalContent(static_cast<localContentType_t>(0x71)) || !m_itemStorageVisible) {
		FrListItem* pItem = m_pUccCopyHideWnd->GetItemUnderCursor();
		if (pItem == NULL) {
			return;
		}

		if (m_roomMode != 8) {
			return;
		}

		EquipItem((sItemInfo*)pItem->pData);
		return;
	}

	if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true && m_itemStorageVisible == true) {
		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
		if (pItemStorage == NULL) {
			return;
		}

		if (pItemStorage->IsLock()) {
			return;
		}

		FrListItem* pItem = m_pUccCopyHideWnd->GetItemUnderCursor();
		if (pItem == NULL) {
			return;
		}

		pItemStorage->ReqPushStoredItemInfo(*pItem);
	}
}

// realmyroommainui.cpp:8261
void CRealMyRoomMain::OnPowerEditInit(int param) {
	m_pPowerEdit = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:8266
void CRealMyRoomMain::OnControlEditInit(int param) {
	m_pControlEdit = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:8271
void CRealMyRoomMain::OnImpactEditInit(int param) {
	m_pImpactEdit = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:8276
void CRealMyRoomMain::OnSpinEditInit(int param) {
	m_pSpinEdit = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:8281
void CRealMyRoomMain::OnCurveEditInit(int param) {
	m_pCurveEdit = DYNAMIC_CAST(FrStatic, param);
}

// realmyroommainui.cpp:8286
void CRealMyRoomMain::OnUpgradeInit(int param) {
	m_pUpgradeButton = DYNAMIC_CAST(FrButton, param);
	m_pUpgradeButton->SetVisible(false);

	m_pUpgradeButton->EnableHover(true, 0.0f);
}

// realmyroommainui.cpp:8322
void CRealMyRoomMain::OnDowngradeInit(int param) {
	m_pDowngradeButton = DYNAMIC_CAST(FrButton, param);
	m_pDowngradeButton->SetVisible(false);

	m_pDowngradeButton->EnableHover(true, 0.0f);
}

// realmyroommainui.cpp:8430
void CRealMyRoomMain::GetTotalStatus(unsigned char* const status, unsigned char* const slotStatus, unsigned char* const cardStatus) {
	if (IsLocalContent(static_cast<localContentType_t>(0x51))) {
		WSingleton<CCardManager>::Instance()->SetPlayerIndex(0xFF);
		WSingleton<CCardManager>::Instance()->CalcCardPeriodAndStatus();
	}

	for (int i = 0; i < 5; i++) {
		status_t level = ItemManager()->GetLevelWithPenalty((eLvlType)i, m_charTypeId, m_pCharLevels, m_pCharParts, m_pCharCards, m_clubTypeId, m_pClubLevels, m_caddieTypeId, CSharedDoc::Instance()->m_myLevel);
		status[i] = level.m_level;
		cardStatus[i] = level.m_penalty;

		slotStatus[i] = ItemManager()->GetCapacity((eLvlType)i, m_charTypeId, m_pCharLevels, m_pCharParts, m_clubTypeId, m_pClubLevels, m_caddieTypeId, CSharedDoc::Instance()->m_myLevel);
	}
}

// realmyroommainui.cpp:8451
void CRealMyRoomMain::GetCharacterStatus(unsigned char* const status, unsigned char* const slotStatus) {
	if (IsLocalContent(static_cast<localContentType_t>(0x51))) {
		WSingleton<CCardManager>::Instance()->SetPlayerIndex(0xFF);
		WSingleton<CCardManager>::Instance()->CalcCardPeriodAndStatus();
	}

	IFF_STRUCT::sChar* pChar = ItemManager()->FindChar(m_charTypeId);
	if (pChar == NULL) {
		return;
	}

	for (int i = 0; i < 5; i++) {
		unsigned char itemLevel = ItemManager()->GetItemLevel((eLvlType)i, m_pCharParts, m_pCharCards);
		slotStatus[i] = itemLevel + ItemManager()->GetCharCapacity((eLvlType)i, m_charTypeId, m_pCharParts, m_pCharCards, CSharedDoc::Instance()->m_myLevel);
		status[i] = itemLevel + ItemManager()->GetCharLevel((eLvlType)i, m_charTypeId, m_pCharLevels);
	}
}

// realmyroommainui.cpp:8472
void CRealMyRoomMain::GetCaddieStatus(unsigned char* const pStatus, unsigned char* const pUnused) {
	IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_caddieTypeId);
	if (pCaddie) {
		for (int i = 0; i < 5; i++) pStatus[i] += (unsigned char)pCaddie->m_levels[i];
	}
}

// realmyroommainui.cpp:8483
void CRealMyRoomMain::GetClubSetStatus(unsigned char* const status, unsigned char* const slotStatus) {
	std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.find(m_clubSerial);
	if (it != CSharedDoc::Instance()->m_myItemMap.end()) {
		for (int i = 0; i < 5; i++) {
			status[i] = (unsigned char)it->second.m_clubStatLevel[i];
		}
	}

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_clubTypeId);
	if (pClubSet) {
		for (int i = 0; i < 5; i++) {
			slotStatus[i] += (unsigned char)pClubSet->m_maxStat[i];
			status[i] += (unsigned char)pClubSet->m_baseStat[i];
		}
	}
}

// realmyroommainui.cpp:9555
bool CRealMyRoomMain::SelectItemList(unsigned long typeId) {
	if (m_pUccCopyHideWnd == NULL) {
		return false;
	}

	FrListItem* pFound = NULL;
	for (FrListBox::ITEM_LIST::iterator it = m_pUccCopyHideWnd->m_itemList.begin(); it != m_pUccCopyHideWnd->m_itemList.end(); ++it) {
		sItemInfo* pInfo = (sItemInfo*)(*it)->pData;
		if (pInfo != NULL) {
			if (pInfo->m_typeId == typeId) {
				pFound = *it;
				break;
			}
		}
	}

	if (pFound == NULL) {
		return false;
	}

	SelectItemList(pFound);
	return true;
}

// realmyroommainui.cpp:10011
bool CRealMyRoomMain::UnselectItemFromItemList(unsigned long typeId) {
	if (m_pUccCopyHideWnd == NULL) {
		return false;
	}

	if ((typeId & 0xFC000000) != 0x08000000) {
		return false;
	}

	for (FrListBox::ITEM_LIST::iterator it = m_pUccCopyHideWnd->m_itemList.begin(); it != m_pUccCopyHideWnd->m_itemList.end(); ++it) {
		sItemInfo* pInfo = (sItemInfo*)(*it)->pData;

		if (pInfo != NULL) {
			if (pInfo->m_typeId == typeId) {
				m_pUccCopyHideWnd->UnselectItem(*it);
				return true;
			}
		}
	}

	return false;
}

// realmyroommainui.cpp:10036
bool CRealMyRoomMain::UnselectItemList(unsigned long typeId) {
	if (m_pUccCopyHideWnd == NULL) {
		return false;
	}

	FrListItem* pFound = NULL;
	for (FrListBox::ITEM_LIST::iterator it = m_pUccCopyHideWnd->m_itemList.begin(); it != m_pUccCopyHideWnd->m_itemList.end(); ++it) {
		sItemInfo* pInfo = (sItemInfo*)(*it)->pData;
		if (pInfo != NULL) {
			if (pInfo->m_typeId == typeId) {
				pFound = *it;
				break;
			}
		}
	}

	if (pFound == NULL) {
		return false;
	}

	UnselectItemList(pFound);
	return true;
}

// realmyroommainui.cpp:10383
void CRealMyRoomMain::Net_EquipCharacter(void) {
	if (m_characterId != WSingleton<CSharedDoc>::Instance()->m_myCharacterId) {
		WSendPacket packet((enumClientPacket)0x20);
		packet.Encode1(5);
		packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myCharacterId);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// realmyroommainui.cpp:10467
void CRealMyRoomMain::Net_EquipCaddie(void) {
	if (m_caddieId == WSingleton<CSharedDoc>::Instance()->m_myCaddieId) {
		return;
	}

	if (WSingleton<CSharedDoc>::Instance()->m_myCaddieId != 0) {
		std::map<unsigned int, sCaddieInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.find(WSingleton<CSharedDoc>::Instance()->m_myCaddieId);
		if (it == WSingleton<CSharedDoc>::Instance()->m_caddieMap.end()) {
			return;
		}
	}

	WSendPacket packet((enumClientPacket)0x20);
	packet.Encode1(1);
	packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myCaddieId);
	packet.Send(SEND_TO_DEFAULT);
}

// realmyroommainui.cpp:10486
void CRealMyRoomMain::Net_EquipItem(void) {
	int i;
	for (i = 0; i < 10; i++) {
		if (m_equipItem[i] != WSingleton<CSharedDoc>::Instance()->m_equipItem[i]) {
			break;
		}
	}

	if (i == 10) {
		return;
	}

	WSendPacket packet(0x20);
	packet.Encode1(2);
	packet.EncodeBuffer(WSingleton<CSharedDoc>::Instance()->m_equipItem, 0x28);
	packet.Send(SEND_TO_DEFAULT);
}

// realmyroommainui.cpp:10504
void CRealMyRoomMain::Net_EquipGear(void) {
	if (m_ballId != WSingleton<CSharedDoc>::Instance()->m_myBallId || m_clubId != WSingleton<CSharedDoc>::Instance()->m_myClubId) {
		WSendPacket packet(0x20);
		packet.Encode1(3);
		packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myBallId);
		packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myClubId);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// realmyroommainui.cpp:8502
void CRealMyRoomMain::SetLevelBar(void) {
	unsigned char slotStatus[5] = {0, 0, 0, 0, 0};
	unsigned char status[5] = {0, 0, 0, 0, 0};
	unsigned char cardStatus[5] = {0, 0, 0, 0, 0};
	int i;

	FrGaugeBar** ppBar = &m_pPowerBar;
	for (i = 0; i < 5; i++) {
		if (ppBar[i]) {
			ppBar[i]->SetDestPos(0, false);
			ppBar[i]->SetExpand(0);
		}
	}

	if (m_statTab == 0) {
		switch (m_roomMode) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 21:
		case 22:
		case 26:
			GetCharacterStatus(status, slotStatus);
			m_statSource = 0;
			break;
		case 6:
			GetClubSetStatus(status, slotStatus);
			m_statSource = 1;
			break;
		case 10:
			GetCaddieStatus(status, slotStatus);
			m_statSource = 2;
			break;
		}
	} else if (m_statTab == 1) {
		GetTotalStatus(status, slotStatus, cardStatus);
	}

	FrStatic** ppEdit = &m_pPowerEdit;
	for (i = 0; i < 5; i++) {
		if (ppBar[i]) {
			ppBar[i]->SetDestPos(status[i], false);
			ppBar[i]->SetExpand(slotStatus[i] - status[i]);
		}

		if (ppEdit[i]) {
			ppEdit[i]->SetCaption11(MakeStatusString3(status[i], slotStatus[i], cardStatus[i]));
		}
	}
}

// realmyroommainui.cpp:10518
void CRealMyRoomMain::Net_EquipSkin(void) {
	int i;
	for (i = 0; i < 6; i++) {
		if (m_equipSkin[i] != WSingleton<CSharedDoc>::Instance()->m_equipSkin[i]) {
			break;
		}
	}

	if (i == 6) {
		return;
	}

	std::list<sItemInfo>& skins = WSingleton<CSharedDoc>::Instance()->m_mySkinList;
	for (std::list<sItemInfo>::iterator it = skins.begin(); it != skins.end(); ++it) {
		eEquipSkin skin;
		if (S5::UTIL::GetEquipSkinIndex(skin, it->m_typeId) && it->m_stateBits.kind == 1) {
			if (it->m_typeId == WSingleton<CSharedDoc>::Instance()->m_equipSkin[skin]) {
				it->m_stateBits.kind = 2;
			}
		}
	}

	WSendPacket packet(0x20);
	packet.Encode1(4);
	packet.EncodeBuffer(WSingleton<CSharedDoc>::Instance()->m_equipSkin, 0x18);
	packet.Send(SEND_TO_DEFAULT);
}

// realmyroommainui.cpp:10557
void CRealMyRoomMain::Net_StartPeriodItem(unsigned long serial) {
	WSendPacket packet(0x20);
	packet.Encode1(7);
	packet.Encode4(serial);
	packet.Send(SEND_TO_DEFAULT);
}

// realmyroommainui.cpp:10609
bool CRealMyRoomMain::OnEnchantDlgResult(int result, FrForm* pForm) {
	m_pEnchantDlg = NULL;
	return true;
}

// realmyroommainui.cpp:10615
bool CRealMyRoomMain::OnPangGiftDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		// The gift's serial and type id lead it like in sItemInfo
		// (sGiftInfo's members at 0x0/0x4 are not declared yet).
		sItemInfo* pInfo = (sItemInfo*)((FrPangGiftDlg*)pForm)->GetItemInfo();

		WSendPacket packet((enumClientPacket)0x24);
		packet.Encode4(pInfo->m_serial);
		packet.Encode4(pInfo->m_typeId);
		packet.Send(SEND_TO_DEFAULT);

		Sleep(50);
	}

	m_pPangGiftDlg = NULL;
	return true;
}

// realmyroommainui.cpp:10675
bool CRealMyRoomMain::OnMascotInfoDlgResult(int result, FrForm* pForm) {
	m_pMascotInfoDlg = NULL;
	return true;
}

// realmyroommainui.cpp:10681
bool CRealMyRoomMain::OnUpgradeDlgResult(int result, FrForm* pForm) {
	m_pUpgradeDlg = NULL;
	return true;
}

// realmyroommainui.cpp:10687
bool CRealMyRoomMain::OnNoteDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		WSendPacket packet((enumClientPacket)0x3C);
		packet.Encode2(0x111);

		packet.Encode4(0xFFFFFFFF);
		packet.EncodeStr(std::string(m_pNoteDlg->GetNote()));
		packet.Encode1(1);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_pNoteDlg = NULL;
	return true;
}

// realmyroommainui.cpp:10704
bool CRealMyRoomMain::OnFunritureSaveMsgdlgResult(int result, FrForm* pForm) {
	SetCameraDisplayType(0);
	return true;
}

// realmyroommainui.cpp:10711
bool CRealMyRoomMain::OnOfflineShopEditMsgDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		m_offlineShopEditState = 1;

		WSendPacket packet((enumClientPacket)0xC4);
		packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID);
		packet.Send(SEND_TO_DEFAULT);
	}

	return true;
}

// realmyroommainui.cpp:10779
bool CRealMyRoomMain::OnCaddieInfoDlgResult(int result, FrForm* pForm) {
	if (result == 2) {

		if (m_pCaddieInfoWnd != NULL) {

			m_pCaddieInfoWnd->Enable(true);

			if (pForm != NULL) {

				sCaddieInfo info = ((FrCaddieInfoDlg*)pForm)->GetCaddieInfo();

				std::map<unsigned int, IFF_STRUCT::sCaddie>& caddies = ItemManager()->m_caddieMap;
				for (std::map<unsigned int, IFF_STRUCT::sCaddie>::const_iterator it = caddies.begin(); it != caddies.end(); ++it) {
					if (it->second.m_typeId == info.typeId) {
						if ((it->second.m_level & 0x7F) > WSingleton<CSharedDoc>::Instance()->m_myLevel) {
							m_pCaddieInfoWnd->Enable(false);
						}
						break;
					}
				}
			}
		}
	}

	return true;
}

// realmyroommainui.cpp:10844
bool CRealMyRoomMain::OnOpenNewYearMoneyDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		if (m_newYearMoneySerial != 0) {
			this << MsgObject(0, 1, 0, 0, 0, 0, 0);

			WSendPacket packet((enumClientPacket)0x90);
			packet.Encode4(m_newYearMoneySerial);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	m_newYearMoneySerial = 0;

	return true;
}

// realmyroommainui.cpp:10864
bool CRealMyRoomMain::OnOpenEventGiftDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		if (m_eventGiftBoxSerial != 0) {
			this << MsgObject(0, 1, 0, 0, 0, 0, 0);

			WSendPacket packet((enumClientPacket)0x91);
			packet.Encode4(m_eventGiftBoxSerial);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	m_eventGiftBoxSerial = 0;

	return true;
}

// realmyroommainui.cpp:10884
bool CRealMyRoomMain::OnOpenS4EventGiftDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		if (m_s4EventGiftBoxTypeId != 0) {
			this << MsgObject(0, 1, 0, 0, 0, 0, 0);

			WSendPacket packet((enumClientPacket)0xAA);
			packet.Encode4(m_s4EventGiftBoxTypeId);
			packet.Send(SEND_TO_DEFAULT);
		}
	}

	m_s4EventGiftBoxTypeId = 0;

	return true;
}

// realmyroommainui.cpp:10565
void CRealMyRoomMain::Net_EquipMascot(void) {
	sMascotInfo* pMascot = WSingleton<CSharedDoc>::Instance()->GetMyEquippedMascotInfo();

	if (pMascot) {
		if (ItemManager()->CompareSystemTime(WSingleton<CSharedDoc>::Instance()->GetServerTime(), pMascot->endTime) <= 0 && pMascot->pcBang == 0) {
			WSingleton<CSharedDoc>::Instance()->m_myMascotId = 0;
		}
	}

	if (m_mascotId != WSingleton<CSharedDoc>::Instance()->m_myMascotId) {
		if (WSingleton<CSharedDoc>::Instance()->m_myMascotId != 0) {
			std::map<unsigned int, sMascotInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myMascotMap.find(WSingleton<CSharedDoc>::Instance()->m_myMascotId);
			if (it == WSingleton<CSharedDoc>::Instance()->m_myMascotMap.end()) {
				return;
			}
		}

		WSendPacket packet((enumClientPacket)0x20);
		packet.Encode1(8);
		packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myMascotId);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// realmyroommainui.cpp:10903
void CRealMyRoomMain::OpenCaddieInfo(const sCaddieInfo& info) {
	if (g_pFresh->GetManager()->GetDesktop()->FindChildByName("caddie_info") == NULL) {
		FrForm::ms_bHasTail = false;
		FrCaddieInfoDlg* pDlg = CreateForm<FrCaddieInfoDlg>(g_pFresh->GetManager(), this, "caddie_info", NULL);

		if (pDlg) {
			pDlg->SetCaddieInfo(info);
			pDlg->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnCaddieInfoDlgResult, 3);

			if (m_pCaddieInfoWnd) {
				m_pCaddieInfoWnd->Enable(false);
			}
		}
	}
}

// realmyroommainui.cpp:10923
void CRealMyRoomMain::OpenInformation(FrListItem* pItem) {
	if (pItem) {
		m_infoDlgPos.x = pItem->pos.x - 300.0f;
		m_infoDlgPos.y = pItem->pos.y - 450.0f;
	}

	switch (m_roomMode) {
	case 17: {
		if (IsLocalContent((localContentType_t)0x60)) {
			break;
		}

		// The gift list's rows: the type id follows the serial like in
		// sItemInfo (sGiftInfo's members at 0x0/0x4 are not declared yet).
		sItemInfo* pInfo = (sItemInfo*)pItem->pData;

		OpenInformation(&pInfo->m_typeId, (sGiftInfo*)pInfo, NULL);
		break;
	}
	default: {
		sItemInfo* pInfo = (sItemInfo*)pItem->pData;

		OpenInformation(&pInfo->m_typeId, NULL, &pInfo->m_serial);
		break;
	}
	case 10:
		OpenCaddieInfo(*(sCaddieInfo*)pItem->pData);
		break;
	case 12: {
		sMascotInfo* pInfo = (sMascotInfo*)pItem->pData;

		m_pMascotInfoDlg = CreateForm<FrMascotInfoDlg>(g_pFresh->GetManager(), this, "mascot_info", NULL);

		m_pMascotInfoDlg->SetMascotInfo(*pInfo, m_pPetExhibition ? m_pPetExhibition->GetModel() : NULL);
		m_pMascotInfoDlg->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnMascotInfoDlgResult, 1);
		break;
	}
	}
}

// realmyroommainui.cpp:11599
void CRealMyRoomMain::EquipItem(sItemInfo* pItemInfo) {
	int numSlots = 10;
	int cardSlots = 0;
	int i;

	if (IsLocalContent((localContentType_t)3)) {
		numSlots = WSingleton<CSharedDoc>::Instance()->GetNumAddItemSlotByMascot() + 8;
	}

	if (IsLocalContent((localContentType_t)0x51)) {
		WSingleton<CCardManager>::Instance()->SetPlayerIndex(0xFF);
		WSingleton<CCardManager>::Instance()->CalcCardPeriodAndStatus();
		cardSlots = WSingleton<CCardManager>::Instance()->GetCardPeriodSlot();
		if (cardSlots > 0) {
			numSlots += cardSlots;
		} else if (WSingleton<CSharedDoc>::Instance()->GetNumAddItemSlotByMascot() == 1) {
			WSingleton<CSharedDoc>::Instance()->m_equipItem[9] = 0;
		} else {
			WSingleton<CSharedDoc>::Instance()->m_equipItem[8] = 0;
			WSingleton<CSharedDoc>::Instance()->m_equipItem[9] = 0;
		}
	}

	for (i = 0; i < numSlots; i++) {
		if (WSingleton<CSharedDoc>::Instance()->m_equipItem[i] == 0) {
			break;
		}
	}

	if (i == numSlots) {
		this << MsgObject(0, 0x23, (intptr_t)"\xB4\xF5\x20\xC0\xCC\xBB\xF3\x20\xC0\xE5\xC2\xF8\xC7\xD2\x20\xBC\xF6\x20\xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9", 0, 0, 0, 0);
		return;
	}

	WSingleton<CSharedDoc>::Instance()->m_equipItem[i] = pItemInfo->m_typeId;

	int equipped = 0;
	for (int j = 0; j < 10; j++) {
		if (pItemInfo->m_typeId == WSingleton<CSharedDoc>::Instance()->m_equipItem[j]) {
			equipped++;
		}
	}

	if (pItemInfo->m_count - equipped <= 0) {
		HidePet(true);

		for (FrListBox::ITEM_LIST::iterator it = m_pUccCopyHideWnd->m_itemList.begin(); it != m_pUccCopyHideWnd->m_itemList.end(); ++it) {
			sItemInfo* pInfo = (sItemInfo*)(*it)->pData;
			if (pInfo == 0) {
				continue;
			}
			if (pItemInfo->m_typeId == pInfo->m_typeId) {
				m_pUccCopyHideWnd->_DelItem(it);
				break;
			}
		}

		m_pUccCopyHideWnd->SortItem(EmptyCompares);
	}
}

// realmyroommainui.cpp:11666
void CRealMyRoomMain::UnequipItem(unsigned long* pItemId) {
	for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
		if (it->m_typeId == *pItemId) {
			if (!IsInItemList(*pItemId)) {
				m_pUccCopyHideWnd->AddItem(&*it);
				m_pUccCopyHideWnd->SortItem(EmptyCompares);
			}
		}
	}

	*pItemId = 0;
}

// realmyroommainui.cpp:11682
bool CRealMyRoomMain::IsInItemList(unsigned long typeId) {
	for (FrListBox::ITEM_LIST::iterator it = m_pUccCopyHideWnd->m_itemList.begin(); it != m_pUccCopyHideWnd->m_itemList.end(); ++it) {
		FrListItem* pItem = *it;
		sItemInfo* pInfo = (sItemInfo*)pItem->pData;

		if (pInfo == 0) {
			continue;
		}

		if (pInfo->m_typeId == typeId) {
			return true;
		}
	}

	return false;
}

// realmyroommainui.cpp:11699
void CRealMyRoomMain::SafeLoadPet(unsigned long* pTypeId, bool bForce) {
	m_petLoader.WaitUntilWorkDone(9999);

	if (!bForce) {
		if (m_pPetExhibition != NULL) {
			if (m_pPetExhibition->GetTypeId() == *pTypeId) {
				return;
			}
		}
	}

	m_petLoader.Work(this, 0, pTypeId, true, -1);
}

// realmyroommainui.cpp:11719
void CRealMyRoomMain::TurnOnItemStatus(void) {
	if (m_pItemStatusWnd) {
		m_pItemStatusWnd->Enable(true);
	}
	m_statTab = STAT_TAB_PART;
	SetLevelBar();
}

// realmyroommainui.cpp:11727
void CRealMyRoomMain::TurnOffItemStatus(void) {
	if (m_pItemStatusWnd) {
		m_pItemStatusWnd->Enable(false);
	}
	m_statTab = STAT_TAB_ALL;
	SetLevelBar();
}

// realmyroommainui.cpp:11735
void CRealMyRoomMain::AutoScroll(void) {
	if (m_pUccCopyHideWnd && m_pUccCopyHideWnd->GetScrollBar()) {
		m_pUccCopyHideWnd->GetScrollBar()->SetCurTopRow(0);
	}
}

// realmyroommainui.cpp:11894
bool CRealMyRoomMain::OnFortuneDlgResult(int result, FrForm* pForm) {
	if (result == 1) {
		this << MsgObject(0, 1, 0, 0, 0, 0, 0);

		WSendPacket packet((enumClientPacket)0x59);
		packet.Encode4(m_pFortuneDlg->GetTypeId());
		packet.Send(SEND_TO_DEFAULT);
	}

	m_pFortuneDlg = NULL;

	return true;
}

// realmyroommainui.cpp:14164
bool CRealMyRoomMain::OnReplayDelOpenDlgResult(int result, FrForm* pForm) {
	m_pReplayDelDlg = NULL;

	if (result == 1) {
		BuildDisplayStand(static_cast<subTab_t>(0x17));
	}

	return true;
}

// realmyroommainui.cpp:14179
void CRealMyRoomMain::OnFurniturePupInit(int param) {
	m_pFurniturePageUpButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurniturePageUpButton) {
		m_pFurniturePageUpButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14188
void CRealMyRoomMain::OnFurniturePupButtonUp(void) {
	m_furniturePage--;

	m_furniturePage = m_furniturePage < 0 ? 0 : (m_furniturePage > m_furniturePageCount - 1 ? m_furniturePageCount - 1 : m_furniturePage);

	m_pFurnitureListNumWnd->ClearLine();
	m_pFurniturePageCountWnd->ClearLine();
	m_pFurnitureListNumWnd->AddText(MakeStr("%3d", m_furniturePage + 1), false, true);
	m_pFurniturePageCountWnd->AddText(MakeStr("%3d", m_furniturePageCount), false, true);

	BuildDisplayStandFurniture();
}

// realmyroommainui.cpp:14205
void CRealMyRoomMain::OnFurniturePdownInit(int param) {
	m_pFurniturePageDownButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurniturePageDownButton) {
		m_pFurniturePageDownButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14214
void CRealMyRoomMain::OnFurniturePdownButtonUp(void) {
	m_furniturePage++;

	m_furniturePage = m_furniturePage < 0 ? 0 : (m_furniturePage > m_furniturePageCount - 1 ? m_furniturePageCount - 1 : m_furniturePage);

	m_pFurnitureListNumWnd->ClearLine();
	m_pFurniturePageCountWnd->ClearLine();
	m_pFurnitureListNumWnd->AddText(MakeStr("%3d", m_furniturePage + 1), false, true);
	m_pFurniturePageCountWnd->AddText(MakeStr("%3d", m_furniturePageCount), false, true);

	BuildDisplayStandFurniture();
}

// realmyroommainui.cpp:14230
void CRealMyRoomMain::OnFurnitureListNumInit(int param) {
	m_pFurnitureListNumWnd = DYNAMIC_CAST(FrEdit, param);
	if (m_pFurnitureListNumWnd) {
		m_pFurnitureListNumWnd->SetCharLimit(2, false);
		m_pFurnitureListNumWnd->SetVisible(false);
	}
}

// realmyroommainui.cpp:14243
void CRealMyRoomMain::OnFurnitureListNumBtnUp(void) {
	if (m_pFurnitureListNumWnd->IsVisible())
	{
		m_pFurnitureListNumWnd->SetKeyEvent(true);
	}
}

// realmyroommainui.cpp:14252
bool CRealMyRoomMain::OnFurnitureListEnterKey(int param) {
	OnFurniture_BtnListNumBtnUp();

	return false;
}

// realmyroommainui.cpp:14259
void CRealMyRoomMain::OnFurnitureListMaxNumInit(int param) {
	m_pFurniturePageCountWnd = DYNAMIC_CAST(FrEdit, param);
	if (m_pFurniturePageCountWnd) {
		m_pFurniturePageCountWnd->SetVisible(false);
	}
}

// realmyroommainui.cpp:14268
void CRealMyRoomMain::OnFurniture_BtnListNumInit(int param) {
	m_pFurnitureListNumButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureListNumButton) {
		m_pFurnitureListNumButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14278
void CRealMyRoomMain::OnFurniture_BtnListNumBtnUp(void) {
	m_furniturePage = atoi(m_pFurnitureListNumWnd->GetLine(1, false)) - 1;
	m_furniturePage = Between(0, m_furniturePage, m_furniturePageCount - 1);

	m_pFurnitureListNumWnd->ClearLine();
	m_pFurniturePageCountWnd->ClearLine();
	m_pFurnitureListNumWnd->AddText(MakeStr("%3d", m_furniturePage + 1), false, true);
	m_pFurniturePageCountWnd->AddText(MakeStr("%3d", m_furniturePageCount), false, true);

	if (m_pFurnitureListNumWnd->IsVisible()) {
		m_pFurnitureListNumWnd->SetKeyEvent(true);
	}

	BuildDisplayStandFurniture();
}

// realmyroommainui.cpp:14319
void CRealMyRoomMain::OnFurnitureResetInit(int param) {
	m_pFurnitureResetButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureResetButton) {
		m_pFurnitureResetButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14328
void CRealMyRoomMain::OnFurnitureResetButtonUp(void) {
	WSingleton<CRealMyRoomData>::Instance()->ResetList(false);

	for (std::list<sFurniture_List>::iterator it = WSingleton<CSharedDoc>::Instance()->m_furnitureList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_furnitureList.end(); ++it) {
		it->m_flags &= ~1;
	}

	BuildDisplayStandFurniture();
}

// realmyroommainui.cpp:14340
void CRealMyRoomMain::OnFurnitureBackInit(int param) {
	m_pFurnitureBackButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureBackButton) {
		m_pFurnitureBackButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14364
void CRealMyRoomMain::OnFurnitureSaveInit(int param) {
	m_pFurnitureSaveButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureSaveButton) {
		m_pFurnitureSaveButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14405
void CRealMyRoomMain::OnFurnitureOutInit(int param) {
	m_pFurnitureOutButton = DYNAMIC_CAST(FrButton, param);
	if (m_pFurnitureOutButton) {
		m_pFurnitureOutButton->SetVisible(false);
	}
}

// realmyroommainui.cpp:14422
void CRealMyRoomMain::OnFurnitureBackGroundInit1(int param) {
	m_pFurnitureBackGround[0] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[0]) {
		m_pFurnitureBackGround[0]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14430
void CRealMyRoomMain::OnFurnitureBackGroundInit2(int param) {
	m_pFurnitureBackGround[1] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[1]) {
		m_pFurnitureBackGround[1]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14438
void CRealMyRoomMain::OnFurnitureBackGroundInit3(int param) {
	m_pFurnitureBackGround[2] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[2]) {
		m_pFurnitureBackGround[2]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14446
void CRealMyRoomMain::OnFurnitureBackGroundInit4(int param) {
	m_pFurnitureBackGround[3] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[3]) {
		m_pFurnitureBackGround[3]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14454
void CRealMyRoomMain::OnFurnitureBackGroundInit5(int param) {
	m_pFurnitureBackGround[4] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[4]) {
		m_pFurnitureBackGround[4]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14462
void CRealMyRoomMain::OnFurnitureBackGroundInit6(int param) {
	m_pFurnitureBackGround[5] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[5]) {
		m_pFurnitureBackGround[5]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14470
void CRealMyRoomMain::OnFurnitureBackGroundInit7(int param) {
	m_pFurnitureBackGround[6] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[6]) {
		m_pFurnitureBackGround[6]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14478
void CRealMyRoomMain::OnFurnitureBackGroundInit8(int param) {
	m_pFurnitureBackGround[7] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureBackGround[7]) {
		m_pFurnitureBackGround[7]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14486
void CRealMyRoomMain::OnFurnitureScrollInit1(int param) {
	m_pFurnitureScroll[0] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureScroll[0]) {
		m_pFurnitureScroll[0]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14494
void CRealMyRoomMain::OnFurnitureScrollInit2(int param) {
	m_pFurnitureScroll[1] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureScroll[1]) {
		m_pFurnitureScroll[1]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14502
void CRealMyRoomMain::OnFurnitureScrollInit3(int param) {
	m_pFurnitureScroll[2] = DYNAMIC_CAST(FrArea, param);
	if (m_pFurnitureScroll[2]) {
		m_pFurnitureScroll[2]->SetVisible(false);
	}
}

// realmyroommainui.cpp:14709
void CRealMyRoomMain::OnFurnitureTabMainButtonUp(void) {
	TButtonItem* pItem = m_pFurnitureMainTab->GetSelected();
	if (pItem == NULL) {
		return;
	}

	ChangeFMainTab(static_cast<eFurniture_Tab>(pItem->index), false);
}

// realmyroommainui.cpp:14722
void CRealMyRoomMain::ChangeFMainTab(eFurniture_Tab tab, bool bForce) {
	if (m_furnitureMainTab != tab)
		m_furniturePage = 0;

	m_furnitureMainTab = tab;

	if (tab == 2 || tab == 3 || tab == 1)
		m_pFurnitureList->SetMultiSelect(false);
	else
		m_pFurnitureList->SetMultiSelect(true);

	BuildDisplayStandFurniture();
}

// realmyroommainui.cpp:14739
void CRealMyRoomMain::OnFurnitureListInit(int param) {
	m_pFurnitureList = DYNAMIC_CAST(FrListBox, param);

	if (m_pFurnitureList != NULL) {
		m_pFurnitureList->SetVisible(false);
		m_pFurnitureList->UseDummy(true);
		m_pFurnitureList->SetMultiSelect(true);
	}
}

// realmyroommainui.cpp:14981
void CRealMyRoomMain::OnBtnMenuInit(int param) {
	m_pBtnMenu = DYNAMIC_CAST(FrButton, param);
}

// realmyroommainui.cpp:14988
void CRealMyRoomMain::OOnBtnMenuButtonUP(void) {
	m_pMenuList->SetVisible(true);
}

// realmyroommainui.cpp:15007
void CRealMyRoomMain::SetListMenu(void) {
	if (m_pMenuList) {
		m_pMenuList->ClearItem();
	}

	int menu[13];
	menu[0] = 5;
	menu[1] = 6;
	menu[2] = 7;
	menu[3] = 4;
	int count = 4;
	if (IsLocalContent(static_cast<localContentType_t>(0x66)) && WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x80000)) {
		menu[4] = 3;
		count = 5;
	}

	if (WSingleton<CSharedDoc>::Instance()->IsControlServerService(0x40000)) {
		menu[count++] = 1;
	}

	if (!IsLocalContent(static_cast<localContentType_t>(0x71))) {
		menu[count++] = 11;
	}

	switch (m_uiState) {
	case 0:
	case 1:
		menu[count++] = 9;
		menu[count++] = 10;
		break;
	case 2:
		menu[count++] = 9;
		menu[count++] = 10;
	case 4:
		menu[count++] = 11;
		break;
	}

	std::list<sFurniture_List>::iterator it;
	for (it = WSingleton<CSharedDoc>::Instance()->m_furnitureList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_furnitureList.end(); ++it) {
		if ((it->m_typeId & 0x3FF800) == 0x2800 && (it->m_flags & 1)) {
			break;
		}
	}

	if (it == WSingleton<CSharedDoc>::Instance()->m_furnitureList.end()) {
		menu[count++] = 3;
	}

	if (m_pMenuList) {
		for (int i = 0; i < 13; i++) {
			if (i == 12) {
				int pad = 2 - m_pMenuList->GetCurrentItemSize(true) % 3;
				for (; pad > 0; pad--) {
					m_pMenuList->AddItem(NULL);
				}

				m_pMenuList->AddItem((void*)13);
				break;
			}

			int j;
			for (j = 0; j < count; j++) {
				if (menu[j] == i) {
					break;
				}
			}

			if (j == count) {
				m_pMenuList->AddItem((void*)(i + 1));
			}
		}
	}
}

// realmyroommainui.cpp:15160
void CRealMyRoomMain::OnBtn_MenuListBtnDown(void) {
	if (m_pMenuList == NULL) {
		return;
	}

	FrListItem* pItem = m_pMenuList->GetItemUnderCursor();
	if (pItem == NULL) {
		return;
	}

	// The item data is the menu number + 1, stored as the pointer value.
	size_t data = (size_t)pItem->pData;
	if (data == 0) {
		return;
	}

	unsigned char menu = (unsigned char)data - 1;

	SelectMenu(menu);
}

// realmyroommainui.cpp:15178
void CRealMyRoomMain::OnBtn_MenuListRBtnDown(void) {
}

// realmyroommainui.cpp:15182
void CRealMyRoomMain::OnBtn_MenuListMouseMove(void) {
}

// realmyroommainui.cpp:15186
void CRealMyRoomMain::OnBtn_MenuListDClick(void) {
}

// realmyroommainui.cpp:15445
void CRealMyRoomMain::SetUIState(unsigned long state) {
	if (m_uiState & state)
		m_uiState &= ~state;
	else
		m_uiState |= state;

	CRealMyRoomData::Instance()->m_pSelectModel = NULL;

	ChangeFlags(state);
}

// realmyroommainui.cpp:15458
void CRealMyRoomMain::ChangeFlags(unsigned long flag) {
	memset(m_flags, 0, sizeof(m_flags));

	switch (flag) {
	case 1:
		m_flags[0] = !m_flags[0];
		break;
	case 2:
		m_flags[1] = !m_flags[1];
		break;
	case 4:
		m_flags[2] = !m_flags[2];
		m_flags[0] = m_flags[2];
		m_flags[1] = m_flags[2];
		break;
	case 8:
		m_flags[3] = !m_flags[3];
		break;
	}
}

// realmyroommainui.cpp:15497
bool CRealMyRoomMain::GetFlags(unsigned long flag) {
	switch (flag) {
	case 1: return m_flags[0];
	case 2: return m_flags[1];
	case 4: return m_flags[2];
	case 8: return m_flags[3];
	}

	return false;
}

// realmyroommainui.cpp:15518
void CRealMyRoomMain::UIDisplayControl(bool bShow) {
	SetOverAndUnderBarVisible(bShow);

	if (bShow) {
		SetCameraDisplayType(1);
	} else {
		SetCameraDisplayType(4);
	}
}

// realmyroommainui.cpp:15589
void CRealMyRoomMain::PostDlgCreate(void) {
	if (m_pPostDlg == NULL) {
		m_pPostDlg = CreateForm<CPost_ReciveDlg>(g_pFresh->GetManager(), this, "postbox", NULL);

		FrForm::ms_bHasTail = false;
		m_pPostDlg->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnPostDlgResult, 0x81);
		m_pPostDlg->SetFadeout(false);
	} else {
		m_pPostDlg->OnInit();
	}
}

// realmyroommainui.cpp:15605
void CRealMyRoomMain::ControlUiHideMode(int mode) {
	for (;;) {
		if (mode != m_uiState && mode != 5) {
			m_prevUiState = m_uiState;
		}

		m_pRoomFx->SetActive(false, true);

		switch (mode) {
		case 1:
			SetUIState(1);

			ShowOverAndUnderBar(true);
			for (int i = 0; i < 3; i++) {
				if (m_barFrame[i] && m_barFrame[i]->IsVisible()) {
					m_barFrame[i]->SetElementFadeOut(false);
				}
			}
			SetCameraDisplayType(1);

			m_bHideMiddleUI = true;
			if (m_pMiddleUIWnd) {
				m_pMiddleUIWnd->SetElementFadeOut(false);
			}
			ChangeMainTab(m_mainTab, true);

			FurnitureDisplayControl(false);
			if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetState(2);
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetHide(false);
			}
			break;
		case 2:
			SetUIState(2);

			ShowOverAndUnderBar(true);
			for (int i = 0; i < 3; i++) {
				if (m_barFrame[i] && m_barFrame[i]->IsVisible()) {
					m_barFrame[i]->SetElementFadeOut(false);
				}
			}

			m_bHideMiddleUI = false;
			if (m_pMiddleUIWnd && m_pMiddleUIWnd->IsVisible()) {
				m_pMiddleUIWnd->SetElementFadeOut(true);
			}
			FurnitureDisplayControl(false);

			SetCameraDisplayType(4);
			if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->ChangeCharacterInfo();
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetState(0);
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetHide(true);
			}
			break;
		case 4:
			m_pRoomFx->SetActive(true, true);

			SetUIState(8);
			for (int i = 0; i < 3; i++) {
				if (m_barFrame[i] && m_barFrame[i]->IsVisible()) {
					m_barFrame[i]->SetElementFadeOut(true);
				}
			}
			ShowOverAndUnderBar(false);

			m_bHideMiddleUI = false;
			if (m_pMiddleUIWnd && m_pMiddleUIWnd->IsVisible()) {
				m_pMiddleUIWnd->SetElementFadeOut(true);
			}
			FurnitureDisplayControl(true);

			SetCameraDisplayType(4);
			if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->ChangeCharacterInfo();
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetState(0);
				WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->SetHide(true);
			}
			break;
		case 5:
			if (m_prevUiState == 1) {
				mode = (m_mainTab != 6) + 1;
			} else {
				mode = 2;
			}
			continue;
		}
		break;
	}

	m_uiState = mode;
	SetListMenu();
}

// realmyroommainui.cpp:15722
void CRealMyRoomMain::SetOverAndUnderBarVisible(bool bVisible) {
	bool bFadeOut = !bVisible;

	if (bVisible) {
		ShowOverAndUnderBar(true);
	}

	for (int i = 0; i < 3; i++) {
		if (m_barFrame[i] && m_barFrame[i]->IsVisible()) {
			m_barFrame[i]->SetElementFadeOut(bFadeOut);
		}
	}

	if (bFadeOut) {
		ShowOverAndUnderBar(false);
	}
}

// realmyroommainui.cpp:15747
void CRealMyRoomMain::SetMiddleUIControl(bool bShow) {
	if (bShow) {
		SetCameraDisplayType(1);
	} else {
		SetCameraDisplayType(4);
	}
}

// realmyroommainui.cpp:15779
void CRealMyRoomMain::SetMiddleUIVisible(bool bVisible) {
	bool bFadeOut = !bVisible;

	if (bVisible) {
		if (m_pMiddleUIWnd != NULL) {
			m_pMiddleUIWnd->SetElementFadeOut(bFadeOut);
		}

		ChangeMainTab(m_mainTab, true);
	} else {
		if (m_pMiddleUIWnd != NULL && m_pMiddleUIWnd->IsVisible()) {
			m_pMiddleUIWnd->SetElementFadeOut(bFadeOut);
		}
	}
}

// realmyroommainui.cpp:15881
void CRealMyRoomMain::Furniture_ShowUI_Left(bool bShow) {
	if (m_pFurnitureLeftWnd[0]) m_pFurnitureLeftWnd[0]->Enable(bShow);
	if (m_pFurnitureLeftWnd[1]) m_pFurnitureLeftWnd[1]->Enable(bShow);
	if (m_pFurnitureLeftWnd[2]) m_pFurnitureLeftWnd[2]->Enable(bShow);
	if (m_pFurnitureLeftWnd[3]) m_pFurnitureLeftWnd[3]->Enable(bShow);
	if (m_pFurnitureLeftWnd[4]) m_pFurnitureLeftWnd[4]->Enable(bShow);
}

// realmyroommainui.cpp:15978
void CRealMyRoomMain::OnEffectViewInit(int param) {
	m_pEffectViewWnd = DYNAMIC_CAST(FrArea, param);
}

// realmyroommainui.cpp:15983
void CRealMyRoomMain::OnEffectViewOnwnerDraw(int param) {
	FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();
	if (pGI == 0) {
		return;
	}

	pGI->Box(m_pEffectViewWnd->GetRect(), 0x18000000, 0, 0.0f);
}

// realmyroommainui.cpp:16026
void CRealMyRoomMain::OnRightMenuInit(int param) {
	m_pRightMenuWnd = DYNAMIC_CAST(FrWnd, param);
	if (m_pRightMenuWnd) {
		m_pRightMenuWnd->SetVisible(false);
		m_pRightMenuWnd->SetFixed(true);
	}
}

// realmyroommainui.cpp:16105
void CRealMyRoomMain::OnRightMenuRBtnDown(void) {
	if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true) {
		if (m_itemStorageVisible == true) {
			return;
		}
	}

	m_pRightMenuWnd->SetVisible(false);
}

// realmyroommainui.cpp:16119
void CRealMyRoomMain::UpdateRightMenu(void) {
	if (WSingleton<CRealMyRoomData>::Instance()->m_pSelectModel != NULL) {
		int kind = (WSingleton<CRealMyRoomData>::Instance()->m_pSelectModel->m_pFurniture->m_typeId >> 11) & 0x7FF;
		if (kind >= 7 && kind <= 9) {
			static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetVisible(false);
			return;
		}

		if (WSingleton<CSharedDoc>::Instance()->m_myUID == WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID
			|| !WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
			if (m_flags[3]) {
				static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetMenuList("\xB0\xA1\xB1\xB8\xBB\xE7\xBF\xEB@rmr_i_use|\xB0\xA1\xB1\xB8\xC0\xCC\xB5\xBF@rmr_i_move|separator|\xB0\xA1\xB1\xB8\xC8\xB8\xC0\xFC@rmr_i_rotate|separator|\xB0\xA1\xB1\xB8\xC1\xA6\xB0\xC5@rmr_i_delete");
			} else {
				IFF_STRUCT::sFurniture* pFurniture = ItemManager()->FindFurniture(WSingleton<CRealMyRoomData>::Instance()->m_pSelectModel->m_pFurniture->m_typeId);
				if (pFurniture->m_rentalFlag != 0) {
					static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetMenuList("\xB0\xA1\xB1\xB8\xBB\xE7\xBF\xEB@rmr_i_use|\xB0\xA1\xB1\xB8\xBC\xB1\xC5\xC3@rmr_i_select");
				} else {
					static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetVisible(false);
				}
			}
		} else if (kind == 5 || kind == 0xD) {
			unsigned long typeId = WSingleton<CRealMyRoomData>::Instance()->m_pSelectModel->m_pFurniture->m_typeId;
			if (typeId == 0x48006820 || typeId == 0x48006821) {
				static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetVisible(false);
			} else {
				static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetMenuList("\xB0\xA1\xB1\xB8\xBB\xE7\xBF\xEB@rmr_i_use|\xB0\xA1\xB1\xB8\xBC\xB1\xC5\xC3@rmr_i_select");
			}
		} else {
			static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->SetVisible(false);
		}

		if (IsLocalContent(static_cast<localContentType_t>(0x76)) && kind == 0xD) {
			if (IsLocalContent(static_cast<localContentType_t>(0x76))) {
				unsigned long typeId = WSingleton<CRealMyRoomData>::Instance()->m_pSelectModel->m_pFurniture->m_typeId;
				if (typeId != 0x48006820 && typeId != 0x48006821) {
					static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->UnableMenuItem("\xB0\xA1\xB1\xB8\xBC\xB1\xC5\xC3");
					return;
				}
			} else {
				return;
			}
		}

		static_cast<FrContextMenuCtrl*>(m_pRightMenuWnd)->UnableMenuItem("\xB0\xA1\xB1\xB8\xBB\xE7\xBF\xEB");
	} else {
		m_pRightMenuWnd->SetVisible(false);
	}
}

// realmyroommainui.cpp:16202
void CRealMyRoomMain::PopupRightMenu(int x, int y) {
	if (WSingleton<CSharedDoc>::Instance()->m_myUID == WSingleton<CSharedDoc>::Instance()->m_myRoomOwnerUID
		|| !WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		m_pRightMenuWnd->MoveWindow(WPoint((float)x, (float)y));

		m_pRightMenuWnd->SetVisible(true);
		m_pRightMenuWnd->SetTopmost(true);
	} else if (g_pFresh->IsCurrentLayout("REALMYROOMVISITOR")) {
		m_pRightMenuWnd->MoveWindow(WPoint((float)x, (float)y));

		m_pRightMenuWnd->SetVisible(true);
		m_pRightMenuWnd->SetTopmost(true);
	}

	UpdateRightMenu();
}

// realmyroommainui.cpp:16426
void CRealMyRoomMain::OnUnderBar_BackUp(void) {
	if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true) {
		if (m_itemStorageVisible == true) {
			SetItemStorageVisible(false);
		} else {
			CTaskMain::OnUnderBar_BackUp();
		}
	} else {
		CTaskMain::OnUnderBar_BackUp();
	}
}

// realmyroommainui.cpp:16575
void CRealMyRoomMain::OnUnderBar_ItemStorageUp(void) {
	if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true) {
		SelectMenu(11);
	}

	CIconToolTip::GetInstance()->ClearToolTip();
}

// realmyroommainui.cpp:16592
void CRealMyRoomMain::OnUnderBar_MyRoomUp(void) {
	if (g_pFresh->IsCurrentLayout("REALMYROOMVISITOR")) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		pForm->SetMessage("\xB4\xD9\xB8\xA5\xC0\xAF\xC0\xFA\xC0\xC7 \xB9\xE6\xC0\xBB \xB9\xE6\xB9\xAE \xC1\xDF\xC0\xCE \xBB\xF3\xC5\xC2\xBF\xA1\xBC\xAD\xB4\xC2 \xB8\xB6\xC0\xCC\xB7\xEB\xC0\xB8\xB7\xCE \xB0\xA5 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		pForm->Open(0, 3);
	} else {
		CTaskMain::OnUnderBar_MyRoomUp();
	}
}

// realmyroommainui.cpp:16603
void CRealMyRoomMain::OnUnderBar_GiftUp(void) {
	if (g_pFresh->IsCurrentLayout("REALMYROOMVISITOR")) {
		FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", NULL);
		pForm->SetMessage("\xB4\xD9\xB8\xA5\xC0\xAF\xC0\xFA\xC0\xC7 \xB9\xE6\xC0\xBB \xB9\xE6\xB9\xAE \xC1\xDF\xC0\xCE \xBB\xF3\xC5\xC2\xBF\xA1\xBC\xAD\xB4\xC2 \xBC\xB1\xB9\xB0\xC7\xD4\xC0\xB8\xB7\xCE \xB0\xA5 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		pForm->Open(0, 3);
	} else {
		SelectMenu(1);
	}
}

// realmyroommainui.cpp:16617
bool CRealMyRoomMain::OnPostDlgResult(int result, FrForm* pForm) {
	if (pForm != NULL) {
		FrForm::ms_bHasTail = true;
	}

	SetListMenu();
	m_pPostDlg = NULL;
	return true;
}

// realmyroommainui.cpp:16717
bool CRealMyRoomMain::OnPostSendDlgResult(int result, FrForm* pForm) {
	if (pForm) {
		FrForm::ms_bHasTail = true;
	}

	if (result == 1) {
		if (m_pPostSendDlg) {
			WSendPacket packet((enumClientPacket)0xBB);
			packet.Encode4(WSingleton<CSharedDoc>::Instance()->m_myUID);
			packet.Encode4(m_pPostSendDlg->m_sendUID);

			packet.Encode1(m_pPostSendDlg->GetSendListSize());
			packet.Encode8(m_pPostSendDlg->GetSendServiceCharge());
			packet.EncodeStr(std::string(""));
			packet.EncodeStr(m_pPostSendDlg->GetLetterMsg());

			sMailIncludeItem item;
			std::list<sMailIncludeItem>* pList = m_pPostSendDlg->GetSendList();

			for (std::list<sMailIncludeItem>::iterator it = pList->begin(); it != pList->end(); ++it) {
				item = *it;
				packet.EncodeBuffer(&item, 0x35);
			}

			packet.Send(SEND_TO_DEFAULT);

			m_pPostSendDlg = NULL;
		}
	} else {
		m_pPostDlg = NULL;
		m_pPostSendDlg = NULL;

		m_pPostDlg = CreateForm<CPost_ReciveDlg>(g_pFresh->GetManager(), this, "postbox", NULL);
		FrForm::ms_bHasTail = false;
		m_pPostDlg->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnPostDlgResult, 0x81);
		m_pPostDlg->SetFadeout(false);
	}

	return true;
}

// realmyroommainui.cpp:16771
bool CRealMyRoomMain::OnPostSendDlg2Result(int result, FrForm* pForm) {
	m_pPostSendDlg = NULL;

	if (result == 1) {
		m_pPostSendDlg = CreateForm<CPost_SendDlg>(g_pFresh->GetManager(), this, "write_post", NULL);
		FrForm::ms_bHasTail = false;
		m_pPostSendDlg->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnPostSendDlgResult, 0x81);
		m_pPostSendDlg->SetFadeout(false);
	}

	return true;
}

// realmyroommainui.cpp:16827
bool CRealMyRoomMain::OnAuthDlgFaileResult(int result, FrForm* pForm) {
	if (result == 1) {
		SelectMenu(8);
	}

	return true;
}

// realmyroommainui.cpp:17011
void CRealMyRoomMain::DeleteCardImages(void) {
	for (std::map<unsigned int, WOverlay*>::iterator it = m_cardImgMap.begin(); it != m_cardImgMap.end(); ++it) {
		if (g_resrcmng && it->second) { g_resrcmng->Release(it->second); it->second = 0; }
	}
}

// realmyroommainui.cpp:17028
WOverlay* CRealMyRoomMain::GetCardImg(unsigned long typeId) {
	std::map<unsigned int, WOverlay*>::iterator it = m_cardImgMap.find(typeId);
	IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(typeId);

	if (it == m_cardImgMap.end()) {

		pCard = ItemManager()->FindCard(typeId);

		if (pCard != NULL) {

			char path[64];
			sprintf(path, "%s.png", pCard->m_model);
			WOverlay* overlay = g_resrcmng->GetOverlay(path, 0);
			if (overlay) {
				m_cardImgMap.insert(std::map<unsigned int, WOverlay*>::value_type(pCard->m_typeId, overlay));
			}
			return overlay;
		}
	} else {
		return it->second;
	}

	return NULL;
}

// realmyroommainui.cpp:17055
void CRealMyRoomMain::DrawCard(void) {
	if (m_cardTypeId != 0) {
		IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(m_cardTypeId);

		if (pCard != NULL) {
			WOverlay* pImg = GetCardImg(m_cardTypeId);
			if (pImg != NULL) {
				WRECT screenRect = { 92.0f, 117.0f, 248.0f, 499.0f };
				WRECT texRect = { 0.0f, 0.0f, 1.0f, 1.0f };
				pImg->Render(g_view, texRect, screenRect, 0, 0xFFFFFFFF, 0.0f, 0);
			}
		}
	}
}

// realmyroommainui.cpp:17327
bool CRealMyRoomMain::OnResultDlg_Report(int result, FrForm* pForm) {
	m_pReportDlg = NULL;

	return true;
}

// realmyroommainui.cpp:17335
bool CRealMyRoomMain::OnUccDrawDlgResult(int result, FrForm* pForm) {
	m_pUccDrawDlg = NULL;
	unsigned long tidChar = WSingleton<CSharedDoc>::Instance()->GetMyPartTidList().m_tidChar;
	m_pUccCopyHideWnd->SetVisible(true);
	if (g_view != NULL) {
		ShowPet(tidChar);
	}

	return true;
}

// realmyroommainui.cpp:17347
bool CRealMyRoomMain::OnUccCopyDlgResult(int result, FrForm* pForm) {
	m_pUccCopyDlg = NULL;
	m_pUccCopyHideWnd->SetVisible(true);
	return true;
}

// realmyroommainui.cpp:17355
void CRealMyRoomMain::OpenEventXmasGiftBox(sItemInfo* pItemInfo) {
	if (pItemInfo != NULL) {
		CGiftBoxEvent* pEvent = (CGiftBoxEvent*)WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0x24);
		if (pEvent == NULL) {
			return;
		}

		m_xmasGiftBoxSerial = pItemInfo->m_serial;

		pEvent->SetOpenedBoxTID(pItemInfo->m_typeId);

		if (pEvent->GetOpenedBoxTID() != 0) {
			FrForm* pForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
			pForm->SetMessage("\xC5\xA9\xB8\xAE\xBD\xBA\xB8\xB6\xBD\xBA \xBC\xB1\xB9\xB0 \xBB\xF3\xC0\xDA\xB8\xA6 \xBF\xAD\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
			pForm->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnOpenEventXmasGiftDlgResult, 1);
		}
	}
}

// realmyroommainui.cpp:17379
bool CRealMyRoomMain::OnOpenEventXmasGiftDlgResult(int result, FrForm* pForm) {
	return true;
}

// realmyroommainui.cpp:17400
bool CheckRandomBoxTypeID(unsigned long typeId) {
	IFF_STRUCT::sItem* pItem = ItemManager()->FindItem(typeId);
	if (pItem != NULL) {
		if (pItem->IsRandomBox()) {
			return true;
		}
	}

	return false;
}

// realmyroommainui.cpp:17499
bool CRealMyRoomMain::OnOpenNewRandomBoxDlgResult(int result, FrForm* pForm) {
	if (result == 1) {

		if (m_s4EventGiftBoxTypeId != 0) {

			FrRandomBoxProgress* pDlg = CreateForm<FrRandomBoxProgress>(g_pFresh->GetManager(), this, "newRandomBox", 0);
			if (pDlg == NULL) {
				return false;
			}

			pDlg->SetBoxTid(m_s4EventGiftBoxTypeId);

			const char* pName = ItemManager()->GetItemName(m_s4EventGiftBoxTypeId);
			if (pName != NULL) {

				pDlg->SetExplane(MakeStr("%\xBC\xB1\xC5\xC3\xC7\xCF\xBD\xC5 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBF\xA9\xB4\xC2 \xC1\xDF\xC0\xD4\xB4\xCF\xB4\xD9.", pName));
			}
			pDlg->SetFixed(true);
			pDlg->Open(0, 1);
		}
	}

	m_s4EventGiftBoxTypeId = 0;

	return true;
}

// realmyroommainui.cpp:17555
void CRealMyRoomMain::OpenItemStorageNotice(void) {
	if (m_pItemStorageNotice == NULL) {
		m_pItemStorageNotice = CreateForm<FrItemStorageNotice>(g_pFresh->GetManager(), this, "item_storage_notice", NULL);
	}

	if (m_pItemStorageNotice != NULL) {
		m_pItemStorageNotice->SetCaption("\xBE\xCB\xB8\xB2");
		m_pItemStorageNotice->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnResultItemStorageNotice, 9);
	}
}

// realmyroommainui.cpp:17570
bool CRealMyRoomMain::OnResultItemStorageNotice(int result, FrForm* pForm) {
	OpenItemStorageAccessPanel(0);
	m_pItemStorageNotice = NULL;
	return true;
}

// realmyroommainui.cpp:17579
void CRealMyRoomMain::OpenItemStorageAccessPanel(unsigned char type) {
	if (m_pItemStorageAccessPanel == NULL) {
		m_pItemStorageAccessPanel = CreateForm<FrItemStorageAccessPanel>(g_pFresh->GetManager(), this, "item_storage_access_panel", NULL);
	}

	if (m_pItemStorageAccessPanel != NULL) {
		WRect rect = m_pItemStorageAccessPanel->GetRect();

		rect.x = 100.0f;
		rect.y = 75.0f;
		m_pItemStorageAccessPanel->SetRect(rect);

		m_pItemStorageAccessPanel->SetFormType((FrItemStorageAccessPanel::eFormType)type);
		m_pItemStorageAccessPanel->Open((FRESH_RESULT_CALLBACK)&CRealMyRoomMain::OnResultItemStorageAccessPanel, 9);
	}
}

// realmyroommainui.cpp:17599
bool CRealMyRoomMain::OnResultItemStorageAccessPanel(int result, FrForm* pForm) {
	bool bClose = true;

	if (pForm) {
		FrItemStorageAccessPanel* pPanel = (FrItemStorageAccessPanel*)pForm;

		if (result == 1) {
			pPanel->SendPasswordToServer();
		} else if (pPanel->GetFormType() == 0) {
			bClose = false;
		}
	}

	m_pItemStorageAccessPanel = 0;

	return bClose;
}

// realmyroommainui.cpp:17728
void CRealMyRoomMain::OnOwnerDrawItemStorageBGI(int param) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		pItemStorage->DrawBackgroundImage();
	}
}

// realmyroommainui.cpp:17740
void CRealMyRoomMain::OnInitItemStoragePang(int param) {
	m_pItemStoragePang = DYNAMIC_CAST(FrArea, param);
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage) {
		pItemStorage->AddFrontUI(m_pItemStoragePang);
	}
}

// realmyroommainui.cpp:17754
void CRealMyRoomMain::OnOwnerDrawItemStoragePang(int param) {
}

// realmyroommainui.cpp:17760
void CRealMyRoomMain::OnInitItemStoragePangMinus(int param) {
	m_pItemStoragePangMinus = DYNAMIC_CAST(FrButton, param);
	if (m_pItemStoragePangMinus) {
		m_pItemStoragePangMinus->SetPushDelay(0.1f);

		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
		if (pItemStorage != NULL) {
			pItemStorage->AddFrontUI(m_pItemStoragePangMinus);
		}
	}
}

// realmyroommainui.cpp:17777
void CRealMyRoomMain::OnLBDownItemStoragePangMinus(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			pItemStorage->OpenPangBank(FrItemStoragePangBank::TYPE_WITHDRAW);
		}
	}
}

// realmyroommainui.cpp:17791
void CRealMyRoomMain::OnInitItemStoragePangPlus(int param) {
	m_pItemStoragePangPlus = DYNAMIC_CAST(FrButton, param);
	if (m_pItemStoragePangPlus) {
		m_pItemStoragePangPlus->SetPushDelay(0.1f);

		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
		if (pItemStorage != NULL) {
			pItemStorage->AddFrontUI(m_pItemStoragePangPlus);
		}
	}
}

// realmyroommainui.cpp:17808
void CRealMyRoomMain::OnLBDownItemStoragePangPlus(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			pItemStorage->OpenPangBank(FrItemStoragePangBank::TYPE_DEPOSIT);
		}
	}
}

// realmyroommainui.cpp:17843
void CRealMyRoomMain::OnOwnerDrawItemStorageList(int param) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		const FrListItem* pItem = (const FrListItem*)param;

		if (pItem != NULL) {
			pItemStorage->DrawStoredItemInfo(m_pItemStorageList, *pItem);
		}
	}
}

// realmyroommainui.cpp:17904
void CRealMyRoomMain::OnRBDownItemStorageList(void) {
	if (IsLocalContent((localContentType_t)0x71) == true && m_itemStorageVisible == true) {

		if (m_pItemStorageList != NULL) {

			CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
			if (pItemStorage != NULL) {

				if (!pItemStorage->IsLock()) {

					FrListItem* pItem = m_pItemStorageList->GetItemUnderCursor();

					if (pItem != NULL) {

						sStoredItemInfo* pInfo = (sStoredItemInfo*)pItem->pData;

						if (pInfo != NULL) {

							unsigned long typeId = pInfo->item.typeId;
							unsigned long serial = pInfo->item.guid;

							OpenInformation(&typeId, 0, &serial);
						}

						m_pItemStorageList->SelectItem(pItem, true);
					}
				}
			}
		}
	}
}

// realmyroommainui.cpp:17940
void CRealMyRoomMain::OnDBClickItemStorageList(void) {
	if (IsLocalContent(static_cast<localContentType_t>(0x71)) == true && m_itemStorageVisible == true) {
		if (m_pItemStorageList != NULL) {
			CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
			if (pItemStorage != NULL) {
				if (!pItemStorage->IsLock()) {
					FrListItem* pItem = m_pItemStorageList->GetItemUnderCursor();

					if (pItem != NULL) {
						m_pItemStorageList->SelectItem(pItem, true);

						CItemStorage* pStorage = WSingleton<CItemStorage>::Instance();

						if (pStorage != NULL) {
							pStorage->ReqPopStoredItemInfo(*pItem);
						}
					}
				}
			}
		}
	}
}

// realmyroommainui.cpp:17972
void CRealMyRoomMain::OnMouseMoveItemStorageList(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage != NULL) {
		pItemStorage->SetDrag(static_cast<sStoredItemDragDrop::MODE>(2));
	}
}

// realmyroommainui.cpp:17982
void CRealMyRoomMain::OnInitItemStorageIndexMsg(int param) {
	m_pItemStorageIndexMsg = DYNAMIC_CAST(FrListBox, param);
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage) {
		pItemStorage->AddFrontUI(m_pItemStorageIndexMsg);
	}
}

// realmyroommainui.cpp:17996
void CRealMyRoomMain::OnOwnerDrawItemStorageIndexMsg(int param) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		WPoint pt(165.0f, 386.0f);
		pItemStorage->IndexControlDraw(pt);
	}
}

// realmyroommainui.cpp:18008
void CRealMyRoomMain::OnLBDownItemStorageIndexMsg(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			pItemStorage->IndexControlClick();
		}
	}
}

// realmyroommainui.cpp:18022
void CRealMyRoomMain::OnInitItemStoragePageNext(int param) {
	m_pItemStoragePageNext = DYNAMIC_CAST(FrButton, param);
	if (m_pItemStoragePageNext) {
		m_pItemStoragePageNext->SetPushDelay(0.1f);
		m_pItemStoragePageNext->SetVisible(false);
	}
}

// realmyroommainui.cpp:18034
void CRealMyRoomMain::OnLBDownItemStoragePageNext(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			pItemStorage->IndexControlNext();
		}
	}
}

// realmyroommainui.cpp:18049
void CRealMyRoomMain::OnInitItemStoragePagePrev(int param) {
	m_pItemStoragePagePrev = DYNAMIC_CAST(FrButton, param);
	if (m_pItemStoragePagePrev) {
		m_pItemStoragePagePrev->SetPushDelay(0.1f);
		m_pItemStoragePagePrev->SetVisible(false);
	}
}

// realmyroommainui.cpp:18061
void CRealMyRoomMain::OnLBDownItemStoragePagePrev(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			pItemStorage->IndexControlPrev();
		}
	}
}

// realmyroommainui.cpp:18076
void CRealMyRoomMain::OnInitItemStorageClose(int param) {
	m_pItemStorageClose = DYNAMIC_CAST(FrButton, param);
	if (m_pItemStorageClose) {
		m_pItemStorageClose->SetPushDelay(0.1f);
	}

	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage != NULL) {
		pItemStorage->AddFrontUI(m_pItemStorageClose);
	}
}

// realmyroommainui.cpp:18094
void CRealMyRoomMain::OnLBDownItemStorageClose(void) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

	if (pItemStorage != NULL) {
		if (!pItemStorage->IsLock()) {
			SetItemStorageVisible(false);
		}
	}

	if ((__rtti_obj = AfxGetPreservedTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&ntAvatarChatTask::m_RTTI)) {
		m_barButton[7]->Enable(false);
		m_barButton[8]->Enable(false);
	}
}

// realmyroommainui.cpp:18116
void CRealMyRoomMain::OnInitItemStorageDragDrop(int param) {
	m_pItemStorageDragDrop = DYNAMIC_CAST(FrArea, param);
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage) {
		pItemStorage->AddFrontUI(m_pItemStorageDragDrop);
	}
}

// realmyroommainui.cpp:18127
void CRealMyRoomMain::OnOwnerDrawItemStorageDragDrop(int param) {
	CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();
	if (pItemStorage != NULL) {
		pItemStorage->DragDropItemDraw();
	}
}

// realmyroommainui.cpp:18137
void CRealMyRoomMain::SetItemStorageVisible(bool visible) {
	if (IsLocalContent((localContentType_t)0x71) == true) {
		m_itemStorageVisible = visible;

		CItemStorage* pItemStorage = WSingleton<CItemStorage>::Instance();

		if (pItemStorage != NULL) {
			if (m_itemStorageVisible == true) {
				pItemStorage->SetVisibleFrontUI(true);
				pItemStorage->SetEnableBackGroundUI(false);
				pItemStorage->SetVisibleBackGroundUI(false);

				pItemStorage->InitIndexControl(pItemStorage->GetTotalPage(), 5);

				if (m_pItemStorageList != NULL) {
					pItemStorage->MakeStoredItemInfo(m_pItemStorageList);

					if (m_pItemStoragePageNext != NULL && m_pItemStoragePagePrev != NULL) {
						if (pItemStorage->NeedPageIndexer() == true) {
							m_pItemStoragePageNext->SetVisible(true);
							m_pItemStoragePagePrev->SetVisible(true);
						} else {
							m_pItemStoragePageNext->SetVisible(false);
							m_pItemStoragePagePrev->SetVisible(false);
						}
					}
				}
			} else {
				pItemStorage->SetVisibleFrontUI(false);
				pItemStorage->SetEnableBackGroundUI(true);
				pItemStorage->SetVisibleBackGroundUI(true);

				if (m_pItemStorageList != NULL) {
					m_pItemStorageList->ClearItem();
				}

				pItemStorage->ClearStoredItemInfo();

				if (m_pItemStoragePageNext != NULL && m_pItemStoragePagePrev != NULL) {
					m_pItemStoragePageNext->SetVisible(false);
					m_pItemStoragePagePrev->SetVisible(false);
				}
			}
		}
	}
}

// realmyroommainui.cpp:18214
void CRealMyRoomMain::DrawCutinFrame(void) {
	if (m_roomMode == 0x21) {
		FrListItem* pSelected = m_pUccCopyHideWnd->GetSelected();
		if (pSelected != NULL) {
			sItemInfo* pInfo = (sItemInfo*)pSelected->pData;

			if (pInfo != NULL) {
				WOverlay* pCutin = GetCutinImg(pInfo->m_typeId);
				if (pCutin != NULL) {
					WRECT screenRect = { 72.0f, 67.0f, (float)pCutin->GetWidth(), (float)pCutin->GetHeight() };
					WRECT texRect = { 0.0f, 0.0f, 1.0f, 1.0f };
					pCutin->Render(g_view, texRect, screenRect, 0, 0xFFFFFFFF, 0.0f, 0);
				}
			}
		}
	}
}

// realmyroommainui.cpp:18261
WOverlay* CRealMyRoomMain::GetCutinImg(unsigned long typeId) {
	WOverlay* overlay;
	std::map<unsigned int, WOverlay*>::iterator it = m_cutinImgMap.find(typeId);
	if (it == m_cutinImgMap.end()) {

		IFF_STRUCT::sSkin* pSkin = ItemManager()->FindSkin(typeId);

		if (pSkin) {
			char path[64];
			sprintf(path, "%s.jpg", pSkin->m_cutinImage);
			overlay = g_resrcmng->GetOverlay(path, 0);
			if (overlay) {
				m_cutinImgMap.insert(std::map<unsigned int, WOverlay*>::value_type(pSkin->m_typeId, overlay));
			}
		} else {
			return NULL;
		}
	} else {
		overlay = it->second;
	}

	return overlay;
}

// realmyroommainui.cpp:18301
void CRealMyRoomMain::DeleteCutinImages(void) {
	for (std::map<unsigned int, WOverlay*>::iterator it = m_cutinImgMap.begin(); it != m_cutinImgMap.end(); ++it) {
		if (g_resrcmng && it->second) { g_resrcmng->Release(it->second); it->second = 0; }
	}
}
