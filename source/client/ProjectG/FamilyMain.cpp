// Original: d:\build\custom\temp\code\client\projectg\familymain.cpp
#include "FamilyMain.h"
#include "SharedDoc.h"
#include "TreasureDlg.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frwnd.h"
#include "frarea.h"
#include "frbutton.h"
#include "frviewer.h"
#include "frlistbox.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "../shared/globalgamedefine.h"

#include "EquipDlg.h"
#include "exhibition.h"
#include "playerdlg.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "commonutil.h"
#include "mousecursor.h"
#include "UccManager.h"
#include "InputManager.h"

#include <algorithm>
#include <string>
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "frdesktop.h"
#include "RankingDlg.h"
#include "user_info.h"
#include "parttidlist.h"
#include "cardsystem.h"

// familymain.cpp:47
CFamilyMain::cFamily::cFamily(void)
	: m_exhibition(0), m_partTidList(0), m_charInfo(), m_statBar(0), m_petKind(0) {
	memset(&m_charInfo, 0, sizeof(m_charInfo));
	memset(&m_userEquip, 0, sizeof(m_userEquip));
	memset(m_unknown499, 0, sizeof(m_unknown499));
	memset(&m_caddieInfo, 0, sizeof(m_caddieInfo));
	memset(&m_itemInfo, 0, sizeof(m_itemInfo));
}

// familymain.cpp:56
CFamilyMain::cFamily::~cFamily(void) {
	if (m_exhibition) {
		delete m_exhibition;
		m_exhibition = 0;
	}
	if (m_partTidList) {
		delete m_partTidList;
		m_partTidList = 0;
	}
	if (m_statBar) {
		delete m_statBar;
		m_statBar = 0;
	}
}

// familymain.cpp:63
void CFamilyMain::cFamily::SetItemSlot(unsigned long* pItemSlot) {
	memcpy(m_userEquip.itemSlots, pItemSlot, sizeof(m_userEquip.itemSlots));
}

// familymain.cpp:91
const WPoint& CFamilyMain::cFamily::GetFramePos(void) const {
	return m_framePos;
}

// familymain.cpp:93
const sUserEquip& CFamilyMain::cFamily::GetUserEquip(void) const {
	return m_userEquip;
}

// familymain.cpp:158
void CFamilyMain::cFamily::Init(const WRect& rect, std::vector<int>& petList, unsigned char petKind) {
	m_rect = rect;
	const Bitmap* frame = g_pFresh->GetBitmap("family_info_frm");
	if (frame) {
		m_framePos.x = m_rect.x + (m_rect.w - frame->Width()) * 0.5f;
		m_framePos.y = m_rect.y + m_rect.h - frame->Height();
	}

	m_modelPos.x = m_framePos.x + 45.0f;
	m_modelPos.y = m_framePos.y + 50.0f;
	m_petKind = petKind;

	MakePet(petList);
}

// familymain.cpp:175
void CFamilyMain::cFamily::Process(float dt) {
	if (!m_exhibition)
		return;

	m_statBar->Process(dt);

	if (!FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"))) {
		// The pet turns with the mouse unless a dialog sits over it.
		m_exhibition->SetEnableRotate(true);
		if (g_pFresh->GetManager()->GetDesktop()->FindChildByName("treasuremap")
			|| CRankingInfo::Instance()->GetDlg()
			|| (CUserInfo::Instance() && CUserInfo::Instance()->GetDlg()))
			m_exhibition->SetEnableRotate(false);

		m_exhibition->Process(dt, false);
	}
}

// familymain.cpp:194
void CFamilyMain::cFamily::Render(void) {
	if (!FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"))) {
		// The pet is drawn unless its model is being rebuilt or a dialog sits over it.
		if (!m_anotherHand.IsWorking(0)
			&& !CRankingInfo::Instance()->GetDlg()
			&& !(CUserInfo::Instance() && CUserInfo::Instance()->GetDlg())) {
			m_anotherHand.Enter();
			if (m_exhibition)
				m_exhibition->Display(0.0f, 0.0f);
			m_anotherHand.Leave();
		}
	}

	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!pGDI)
		return;

	const Bitmap* frame = g_pFresh->GetBitmap("family_info_frm");
	if (frame)
		pGDI->DrawTexture(frame, WRect(m_framePos.x, m_framePos.y, (float)frame->Width(), (float)frame->Height()), 0xFFFFFFFF, 0);

	const Bitmap* charFrame = g_pFresh->GetBitmap("family_info_char");
	IFF_ITEM_COMMON* item = ItemManager()->FindCommonItem(m_charInfo.typeId);
	if (item && charFrame) {
		const Bitmap* text = g_pFresh->GetBitmap(MakeStr("text_%s", item->m_icon));
		if (text) {
			WPoint pos = m_framePos;
			pos.y -= 30.0f;
			pos.x += (m_rect.w - charFrame->Width()) * 0.5f;
			pGDI->DrawTexture(charFrame, WRect(pos.x, pos.y, (float)charFrame->Width(), (float)charFrame->Height()), 0xFFFFFFFF, 0);

			float y = (charFrame->Height() - text->Height()) * 0.5f + pos.y;
			float x = (charFrame->Width() - text->Width()) * 0.5f + pos.x;
			pGDI->DrawTexture(text, WRect(x, y, (float)text->Width(), (float)text->Height()), 0xFFFFFFFF, 0);
		}
	}

	m_statBar->Render(m_modelPos);
}

// familymain.cpp:313
void CFamilyMain::cFamily::SetStatus(void) {
	if (IsLocalContent((localContentType_t)0x51) && IsLocalContent((localContentType_t)0x64)) {
		WSingleton<CCardManager>::Instance()->CalcCardPeriodAndStatus();
		WSingleton<CCardManager>::Instance()->SetPlayerIndex(m_petKind);
	}

	if (m_statBar) {
		delete m_statBar;
		m_statBar = 0;
	}
	m_statBar = new CStatBar(9.0f, 6.0f);
	CItemManager* pItemManager = ItemManager();
	unsigned char level[5] = { 0 }, penalty[5] = { 0 }, capacity[5] = { 0 };

	// sCharacterInfo::unknown6A holds the character's five stat levels.
	for (int i = 0; i < 5; i++) {
		status_t status = pItemManager->GetLevelWithPenalty((eLvlType)i, m_charInfo.typeId,
			(const char*)&m_charInfo.unknown6A, m_charInfo.partTypeIds, m_charInfo.auxParts.tids,
			m_itemInfo.m_typeId, m_itemInfo.m_clubStatLevel, m_caddieInfo.typeId,
			WSingleton<CSharedDoc>::Instance()->m_myLevel);
		level[i] = status.m_level;
		penalty[i] = status.m_penalty;
		capacity[i] = pItemManager->GetCapacity((eLvlType)i, m_charInfo.typeId,
			(const char*)&m_charInfo.unknown6A, m_charInfo.partTypeIds,
			m_itemInfo.m_typeId, m_itemInfo.m_clubStatLevel, m_caddieInfo.typeId,
			WSingleton<CSharedDoc>::Instance()->m_myLevel);
	}

	m_statBar->SetDestTime(1.2f);
	for (int i = 0; i < 5; i++)
		m_statBar->SetRange((eLvlType)i, level[i], capacity[i], penalty[i]);
}

// familymain.cpp:365
void CFamilyMain::cFamily::PetRebuild(void) {
	if (m_exhibition) {
		delete m_exhibition;
		m_exhibition = 0;
	}
	if (m_partTidList) {
		delete m_partTidList;
		m_partTidList = 0;
	}
	if (m_statBar) {
		delete m_statBar;
		m_statBar = 0;
	}

	SetStatus();

	m_anotherHand.Enter();

	m_exhibition = new CExhibition(m_rect.x + m_rect.w * 0.5f, m_rect.y + m_rect.h * 0.5f - 70.0f, 0.35f, false);
	m_exhibition->SetArea(m_rect.w, m_rect.h);

	if (m_charInfo.typeId == 0x4000005 || m_charInfo.typeId == 0x4000009)
		m_exhibition->SetDistance(11.0f, true, false);
	else
		m_exhibition->SetDistance(13.0f, true, false);

	m_partTidList = new CPartTidList;
	if (!m_partTidList->SetTids(&m_charInfo, 0xFF) || !m_partTidList->IsComboValid())
		m_partTidList->SetDefaultTids();

	m_exhibition->SetModel(*m_partTidList, m_charInfo.auxParts.tids, 0, 3.0f, 0);
	if (WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().eventFlags & 0x10)
		m_exhibition->AttachAngelWingFx();

	m_anotherHand.Leave();
}

// familymain.cpp:396 (run-time type and factory registration of CFamilyMain)
const WRTTI CFamilyMain::m_RTTI("CFamilyMain", &CTaskMain::m_RTTI);

IObject* CFamilyMainMakeInstance(void) {
	return new CFamilyMain;
}

struct __impCFamilyMain {
	__impCFamilyMain(void) {
		ObjectFactory().AddObjectFunctor(CFamilyMainMakeInstance, "CFamilyMain");
		ActorFactory().AddActorClass("CFamilyMain");
	}
};
static __impCFamilyMain s_impCFamilyMain;

// familymain.cpp:397
const sFRESH_MSGMAP* CFamilyMain::GetMessageMap(void) const {
	return &_MsgMap;
}

// familymain.cpp:739
void CFamilyMain::TurnOffControls(void) {
	m_barButton[14]->Enable(false);
	m_barButton[2]->Enable(false);
	m_barButton[3]->Enable(false);
	m_barButton[4]->Enable(false);
	m_barButton[6]->Enable(false);
	m_barButton[8]->Enable(false);
	m_barButton[16]->Enable(false);
	m_barButton[17]->Enable(false);
	m_barButton[10]->Enable(false);
	m_barButton[5]->Enable(false);
	m_barButton[7]->Enable(false);

	m_charViewer->SetVisible(false);
	m_caddieViewer->SetVisible(false);
	m_aztecViewer->SetVisible(false);
	m_mascotViewer->SetVisible(false);
	m_clubViewer->SetVisible(false);
}

// familymain.cpp:766
void CFamilyMain::OnInitAreaPet1(int param) {
	m_areas[0] = DYNAMIC_CAST(FrArea, param);
}

// familymain.cpp:770
void CFamilyMain::OnInitAreaPet2(int param) {
	m_areas[1] = DYNAMIC_CAST(FrArea, param);
}

// familymain.cpp:774
void CFamilyMain::OnInitAreaPet3(int param) {
	m_areas[2] = DYNAMIC_CAST(FrArea, param);
}

// familymain.cpp:778
void CFamilyMain::OnInitAreaPet4(int param) {
	m_areas[3] = DYNAMIC_CAST(FrArea, param);
}

// familymain.cpp:782
void CFamilyMain::OnInitAreaGlobal(int param) {
	m_areas[4] = DYNAMIC_CAST(FrArea, param);
}

// familymain.cpp:787
void CFamilyMain::OnDrawAreaPet1(void) {
	if (g_pFresh->GetManager()->GetGDI() == 0)
		return;

	if (m_families[0])
		m_families[0]->Render();
}

// familymain.cpp:797
void CFamilyMain::OnDrawAreaPet2(void) {
	if (g_pFresh->GetManager()->GetGDI() == 0)
		return;

	if (m_families[1])
		m_families[1]->Render();
}

// familymain.cpp:806
void CFamilyMain::OnDrawAreaPet3(void) {
	if (g_pFresh->GetManager()->GetGDI() == 0)
		return;

	if (m_families[2])
		m_families[2]->Render();
}

// familymain.cpp:815
void CFamilyMain::OnDrawAreaPet4(void) {
	if (g_pFresh->GetManager()->GetGDI() == 0)
		return;

	if (m_families[3])
		m_families[3]->Render();
}

// familymain.cpp:824
void CFamilyMain::OnDrawAreaGlobal(void) {
}

// familymain.cpp:831
void CFamilyMain::OnInitBtnStart(int param) {
	m_buttons[0] = DYNAMIC_CAST(FrButton, param);
}

// familymain.cpp:835
void CFamilyMain::OnInitBtnSelMap(int param) {
	m_buttons[1] = DYNAMIC_CAST(FrButton, param);
}

// familymain.cpp:839
void CFamilyMain::OnInitBtnHoleType(int param) {
	m_buttons[2] = DYNAMIC_CAST(FrButton, param);
	m_buttons[2]->SetPushDelay(0.1f);
}

// familymain.cpp:844
void CFamilyMain::OnInitBtnHoleNum(int param) {
	m_buttons[3] = DYNAMIC_CAST(FrButton, param);
	m_buttons[3]->SetPushDelay(0.1f);
}

// familymain.cpp:849
void CFamilyMain::OnInitBtnTimeLimit(int param) {
	m_buttons[4] = DYNAMIC_CAST(FrButton, param);
	m_buttons[4]->SetPushDelay(0.1f);
}

// familymain.cpp:855
void CFamilyMain::OnInitCreatePet3(int param) {
	m_buttons[5] = DYNAMIC_CAST(FrButton, param);
}

// familymain.cpp:859
void CFamilyMain::OnInitCreatePet4(int param) {
	m_buttons[6] = DYNAMIC_CAST(FrButton, param);
}

// familymain.cpp:863
void CFamilyMain::OnInitDeletePet3(int param) {
	m_buttons[7] = DYNAMIC_CAST(FrButton, param);
	m_buttons[7]->SetVisible(false);
}

// familymain.cpp:868
void CFamilyMain::OnInitDeletePet4(int param) {
	m_buttons[8] = DYNAMIC_CAST(FrButton, param);
	m_buttons[8]->SetVisible(false);
}

// familymain.cpp:875
void CFamilyMain::OnDownBtnSelMap(void) {
	if (m_selMapDlg == 0) {
		m_selMapDlg = CreateForm<FrTreasureCourse>(g_pFresh->GetManager(), this, "treasuremap", 0);
		if (m_selMapDlg) {
			m_selMapDlg->SetSelectMap(m_mapIndex);
			m_selMapDlg->SetMapIndexChanged(false);
			m_selMapDlg->EnableDrag(false);
			m_selMapDlg->EnableRandomBtn(false);

			m_selMapDlg->Open((FRESH_RESULT_CALLBACK)&CFamilyMain::OnResultSelMap, 3);
		}
	}
}

// familymain.cpp:892
bool CFamilyMain::OnResultSelMap(int, FrForm*) {
	if (m_selMapDlg) {
		m_mapIndex = m_selMapDlg->GetCurMapIdx();
		SetCurMap(m_mapIndex);
		m_selMapDlg = 0;
	}
	return true;
}

// familymain.cpp:903
void CFamilyMain::OnDownBtnHoleType(void) {
	m_holeType++;

	if (m_holeType > 3)
		m_holeType = 0;

	WSingleton<CSharedDoc>::Instance()->m_gameKind = m_holeType;
}

// familymain.cpp:913
void CFamilyMain::OnDownBtnHoleNum(void) {
	unsigned char holes[4] = { 3, 6, 9, 18 };

	for (int i = 0; i < 4; ++i) {
		if (holes[i] == m_holeNum) {
			++i;
			if (i >= 4)
				i = 0;

			m_holeNum = holes[i];
			break;
		}
	}

	WSingleton<CSharedDoc>::Instance()->m_holes = m_holeNum;
}

// familymain.cpp:932
void CFamilyMain::OnDownBtnTimeLimit(void) {
	unsigned long limits[6] = { 0, 30000, 40000,
		50000, 60000,
		90000 };

	for (int i = 0; i < 6; ++i) {
		if (limits[i] == m_timeLimit) {
			++i;
			if (i >= 6)
				i = 0;

			m_timeLimit = limits[i];
			break;
		}
	}

	WSingleton<CSharedDoc>::Instance()->m_shotTimeLimit = m_timeLimit;
}

// familymain.cpp:1018
void CFamilyMain::OnDownDeletePet3(void) {
	cFamily* family = m_families[2];
	delete family;
	m_families[2] = 0;

	m_buttons[7]->SetVisible(false);
	m_buttons[5]->SetVisible(true);
	m_buttons[5]->EnableHover(true, 0.0f);
	m_equipLists[2]->ClearItem();
	m_equipLists[2]->SetVisible(false);
}

// familymain.cpp:1031
void CFamilyMain::OnDownDeletePet4(void) {
	cFamily* family = m_families[3];
	delete family;
	m_families[3] = 0;

	m_buttons[8]->SetVisible(false);
	m_buttons[6]->SetVisible(true);
	m_buttons[6]->EnableHover(true, 0.0f);
	m_equipLists[3]->ClearItem();
	m_equipLists[3]->SetVisible(false);
}

// familymain.cpp:1044
void CFamilyMain::OnDrawBtnStart(void) {
}

// familymain.cpp:1049
void CFamilyMain::OnDrawBtnSelMap(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	IFF_STRUCT::sCourse* course = ItemManager()->FindCourse(0x28000000 | m_mapIndex);
	if (course == 0)
		return;

	std::string name("mid_");
	name += course->m_icon;
	const Bitmap* icon = g_pFresh->GetBitmap(name.c_str());

	if (icon) {
		WRect rect = m_buttons[1]->GetRect();
		rect.x += (rect.w - icon->Width() + 1.0f) * 0.5f;
		rect.y += (rect.h - icon->Height() + 1.0f) * 0.5f;
		rect.w = (float)icon->Width();
		rect.h = (float)icon->Height();

		pGDI->DrawTexture(icon, rect, 0xFFFFFFFF, 0);
	}
}

// familymain.cpp:1075
void CFamilyMain::OnDrawBtnHoleType(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	const WRect& rect = m_buttons[2]->GetRect();

	pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
	pGDI->SetTextStyle(4);

	std::string holeTypes[4] = { "FRONT", "BACK", "RANDOM", "SHUFFLE" };
	pGDI->PrintText(WPoint(rect.x + 15.0f, rect.y + 2.0f), 0, MakeStr("\xc1\xf8\xc7\xe0\xb9\xe6\xbd\xc4:%s", holeTypes[m_holeType].c_str()), 0);
	pGDI->SetTextStyle(0);
}

// familymain.cpp:1090
void CFamilyMain::OnDrawBtnHoleNum(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	const WRect& rect = m_buttons[3]->GetRect();

	pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
	pGDI->SetTextStyle(4);
	pGDI->PrintText(WPoint(rect.x + 15.0f, rect.y + 2.0f), 0, MakeStr("\xc8\xa6 \xbc\xf6 : %d \xc8\xa6", m_holeNum), 0);
	pGDI->SetTextStyle(0);
}

// familymain.cpp:1103
void CFamilyMain::OnDrawBtnTimeLimit(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI == 0)
		return;

	const WRect& rect = m_buttons[4]->GetRect();

	pGDI->SetTextColor(0xFFFFFFFF, 0xFF000000);
	pGDI->SetTextStyle(4);
	unsigned long seconds = m_timeLimit / 1000;
	pGDI->PrintText(WPoint(rect.x + 15.0f, rect.y + 2.0f), 0,
		seconds == 0 ? "\xc1\xa6\xc7\xd1\xbd\xc3\xb0\xa3:\xb9\xab\xc1\xa6\xc7\xd1" : MakeStr("\xc1\xa6\xc7\xd1\xbd\xc3\xb0\xa3:%d\xc3\xca", seconds), 0);
	pGDI->SetTextStyle(0);
}

// familymain.cpp:1118
void CFamilyMain::OnInitListEquip1(int param) {
	m_equipLists[0] = DYNAMIC_CAST(FrListBox, param);
}

// familymain.cpp:1122
void CFamilyMain::OnInitListEquip2(int param) {
	m_equipLists[1] = DYNAMIC_CAST(FrListBox, param);
}

// familymain.cpp:1126
void CFamilyMain::OnInitListEquip3(int param) {
	m_equipLists[2] = DYNAMIC_CAST(FrListBox, param);
}

// familymain.cpp:1130
void CFamilyMain::OnInitListEquip4(int param) {
	m_equipLists[3] = DYNAMIC_CAST(FrListBox, param);
}

// familymain.cpp:1135
void CFamilyMain::OnDownListEquip1(int) {
	OnDownEquipListBox(0);
}

// familymain.cpp:1139
void CFamilyMain::OnDownListEquip2(int) {
	OnDownEquipListBox(1);
}

// familymain.cpp:1143
void CFamilyMain::OnDownListEquip3(int) {
	OnDownEquipListBox(2);
}

// familymain.cpp:1147
void CFamilyMain::OnDownListEquip4(int) {
	OnDownEquipListBox(3);
}

// familymain.cpp:1229
bool CFamilyMain::OnItemEquipResult(int result, FrForm* pForm) {
	if (result == 1 || result == 2) {
		cFamily*& family = m_families[m_curPet];
		family->SetItemSlot(((FrEquipDlg*)pForm)->GetMyItemSlot());

		memcpy(ms_itemslotBackup[m_curPet], ((FrEquipDlg*)pForm)->GetMyItemSlot(), sizeof(ms_itemslotBackup[0]));
	}
	return true;
}

// familymain.cpp:1286
void CFamilyMain::OnDrawListEquip1(int item) {
	OnDrawEquipListBox(reinterpret_cast<FrListItem*>(item));
}

// familymain.cpp:1290
void CFamilyMain::OnDrawListEquip2(int item) {
	OnDrawEquipListBox(reinterpret_cast<FrListItem*>(item));
}

// familymain.cpp:1294
void CFamilyMain::OnDrawListEquip3(int item) {
	OnDrawEquipListBox(reinterpret_cast<FrListItem*>(item));
}

// familymain.cpp:1298
void CFamilyMain::OnDrawListEquip4(int item) {
	OnDrawEquipListBox(reinterpret_cast<FrListItem*>(item));
}

// familymain.cpp:1303
void CFamilyMain::OnDrawEquipListBox(FrListItem* pItem) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pItem == 0 || pGDI == 0)
		return;

	const Bitmap* bar;
	IFF_ITEM_COMMON* item = (IFF_ITEM_COMMON*)pItem->pData;
	if (item) {
		bar = g_pFresh->GetBitmap(MakeStr("bar_%s", item->m_icon));
	} else {
		bar = g_pFresh->GetBitmap("bar_no");
	}

	if (bar == 0)
		return;

	WRect rect(pItem->pos.x + 5.0f, pItem->pos.y + 5.0f, bar->Width() + 0.5f, bar->Height() + 0.5f);
	if (pItem->idx != 5) {
		pGDI->DrawTexture(bar, rect, 0xFFFFFFFF, 0);
	} else {
		const Bitmap* button = g_pFresh->GetBitmap("gameroom_equip_item_bn");
		if (button)
			pGDI->DrawTexture(button, rect, 0xFFFFFFFF, 0);
	}

	if (rect.IsInRect(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y))) {
		const Bitmap* frame = g_pFresh->GetBitmap("select_frame");
		if (frame)
			pGDI->DrawTexture(frame, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);
	}
}

// familymain.cpp:1471
CFamilyMain::CFamilyMain(void)
	: m_selMapDlg(0), m_unknown23C(0), m_unknown248(false) {
	m_curPet = 0;
	WSingleton<CSharedDoc>::Instance()->m_gameMode = 1;
	g_pFresh->GetManager()->SetExclusiveKey(true);
}

// familymain.cpp:1478
CFamilyMain::~CFamilyMain(void) {
	g_pFresh->GetManager()->SetExclusiveKey(false);
	this << MsgObject(0, 2, 0, 0, 0, 0, 0);
}

// familymain.cpp:1484
void CFamilyMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();

	WSingleton<CMouseCursor>::Instance()->SetAutoHide(false);
}

// familymain.cpp:1491
void CFamilyMain::OnLoad(void) {
	CTaskMain::OnLoad();
}

// familymain.cpp:1496
void CFamilyMain::OnInit(void) {
	CTaskMain::OnInit();
}

// familymain.cpp:1501
void CFamilyMain::OnDestroy(void) {
	std::for_each(m_families.begin(), m_families.end(), Delete_Object());
	m_families.clear();
	m_areas.clear();
	m_buttons.clear();

	CTaskMain::OnDestroy();
}

// familymain.cpp:1513
void CFamilyMain::OnProcess(float dt) {
	for (std::vector<cFamily*>::iterator it = m_families.begin(); it != m_families.end(); ++it) {
		if (*it)
			(*it)->Process(dt);
	}

	CTaskMain::OnProcess(dt);
}

// familymain.cpp:1528
void CFamilyMain::OnDisplay(void) {
}

// familymain.cpp:1532
void CFamilyMain::ReFresh(bool) {
}

// familymain.cpp:1537
void CFamilyMain::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0: {
		if (g_pFresh == 0)
			return;

		const char* layout = (const char*)msg.m_param1;
		if (WSingleton<CUserInfo>::Instance())
			WSingleton<CUserInfo>::Instance()->Close();
		g_pFresh->OpenLayout(layout, this, true);
		break;
	}

	case 1:
	case 2:
	case 0xE2:
	case 0xE3:
		break;

	default:
		HandleMsgCommon(msg);
		break;
	}
}
