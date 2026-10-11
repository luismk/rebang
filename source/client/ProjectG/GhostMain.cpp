// Original: d:\build\custom\temp\code\client\projectg\ghostmain.cpp
#include "GhostMain.h"
#include "ghostdocument.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "exhibition.h"
#include "parttidlist.h"
#include "playerdlg.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "bitmap.h"
#include "frlistbox.h"
#include "frarea.h"
#include "frdesktop.h"
#include "Packet.h"
#include "RankingDlg.h"
#include "user_info.h"
#include "InputManager.h"
#include "ghosthandler.h"
#include "MascotInfoDlg.h"
#include "ClientSetting.h"
#include "BongdariShopDlg.h"
#include "RecycleDlg.h"
#include "EquipDlg.h"
#include "commonutil.h"

// ghostmain.cpp:50 (run-time type and factory registration of CGhostMain)
const WRTTI CGhostMain::m_RTTI("CGhostMain", &CTaskMain::m_RTTI);

IObject *CGhostMainMakeInstance(void) {
	return new CGhostMain;
}

struct __impCGhostMain {
	__impCGhostMain(void) {
		ObjectFactory().AddObjectFunctor(CGhostMainMakeInstance, "CGhostMain");
		ActorFactory().AddActorClass("CGhostMain");
	}
};
static __impCGhostMain s_impCGhostMain;

// ghostmain.cpp:51
const sFRESH_MSGMAP *CGhostMain::GetMessageMap(void) const {
	return &_MsgMap;
}

// ghostdocument.h:88
int CGhostDocument::IsInvalidate(void) const {
	return m_bInvalidate;
}

// ghostdocument.h:93
struct GHOST_USER_DATA const *CGhostDocument::GetUserData(void) const {
	return m_pUserData;
}

// ghostdocument.h:94
struct GHOST_GAME_DATA const *CGhostDocument::GetGameData(void) const {
	return m_pGameData;
}

// ghostmain.cpp:257
std::string __stdcall GetIconName(const unsigned long &typeId) {
	std::string name("w_");
	IFF_ITEM_COMMON *item = ItemManager()->FindCommonItem(typeId);
	if (item)
		name += item->m_icon;
	else
		name = "w_caddie_no";
	return name;
}

// ghostmain.cpp:296
CGhostMain::cImgBinder::sImgInfo::sImgInfo(const std::string &strImage, const WRect &rect)
	: m_strImage(strImage), m_rect(rect) {
}

// ghostmain.cpp:303
CGhostMain::cImgBinder::cImgBinder(int nReserve) {
	m_vecImage.reserve(nReserve);
}

// ghostmain.cpp:306
CGhostMain::cImgBinder::~cImgBinder(void) {}

// ghostmain.cpp:308
void CGhostMain::cImgBinder::AddImage(std::string strImage, const WPoint &pos) {
	const Bitmap *bitmap = g_pFresh->GetBitmap(strImage.c_str());
	if (bitmap)
		m_vecImage.push_back(sImgInfo(strImage, WRect(pos, WSize((float)bitmap->Width(), (float)bitmap->Height()))));
}

// ghostmain.cpp:317
WPoint CGhostMain::cImgBinder::GetPos(void) {
	WPoint pos(0.0f, 0.0f);
	if (m_vecImage.size() != 0)
		pos = WPoint(m_vecImage[0].m_rect.x, m_vecImage[0].m_rect.y);
	return pos;
}

// ghostmain.cpp:335
void CGhostMain::cImgBinder::SetPosX(float x) {
	float curX = 0.0f;
	if (m_vecImage.size() != 0)
		curX = m_vecImage[0].m_rect.x;

	for (std::vector<sImgInfo>::iterator it = m_vecImage.begin(); it != m_vecImage.end(); ++it)
		it->m_rect.x += x - curX;
}

// ghostmain.cpp:344
void CGhostMain::cImgBinder::AddPos(float dx, float dy) {
	std::vector<sImgInfo>::iterator it = m_vecImage.begin();
	for (; it != m_vecImage.end(); ++it) {
		it->m_rect.x += dx;
		it->m_rect.y += dy;
	}
}

// ghostmain.cpp:356
void CGhostMain::cImgBinder::AddPos(const WPoint &delta) {
	std::vector<sImgInfo>::iterator it = m_vecImage.begin();
	for (; it != m_vecImage.end(); ++it) {
		it->m_rect.x += delta.x;
		it->m_rect.y += delta.y;
	}
}

// ghostmain.cpp:368
void CGhostMain::cImgBinder::Render(unsigned char alpha) {
	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == 0)
		return;

	std::vector<sImgInfo>::iterator it = m_vecImage.begin();
	for (; it != m_vecImage.end(); ++it) {
		const Bitmap *bitmap = g_pFresh->GetBitmap(it->m_strImage.c_str());
		if (bitmap)
			gdi->DrawTexture(bitmap, it->m_rect, (alpha << 24) | 0xFFFFFF, 0);
	}
}

// ghostmain.cpp:393
CGhostMain::CGhostMain(void)
	: m_userPet(0), m_ghostPet(0), m_userParts(0), m_ghostParts(0), m_ghostAlpha(0), m_ghostVisible(false),
	  m_userStatImage(0), m_ghostStatImage(0), m_userStatAlpha(0), m_userStatStopX(0), m_ghostStatStopX(0), m_userStatSpeed(0),
	  m_ghostStatSpeed(0), m_showUserStat(false), m_statTime(0), m_statSpeedTime(0), m_selectedIcon(0),
	  m_equipAlpha(0), m_equipDlg(0), m_userStat(0), m_ghostStat(0), m_unknown404(0), m_tipImage(0) {
	Initialize();
}

// ghostmain.cpp:398
CGhostMain::~CGhostMain(void) {
}

// ghostmain.cpp:557
void CGhostMain::StatProcess(float dt) {
	m_userStat->Process(dt);
	m_ghostStat->Process(dt);
}

// ghostmain.cpp:606
void CGhostMain::HidePet(bool wait) {
	if (wait)
		m_hand.WaitUntilWorkDone(9999);

	if (m_userPet) { delete m_userPet; m_userPet = 0; }
	if (m_ghostPet) { delete m_ghostPet; m_ghostPet = 0; }
}

// ghostmain.cpp:614
void CGhostMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();
}

// ghostmain.cpp:618
void CGhostMain::OnLoad(void) {
	CTaskMain::OnLoad();
}

// ghostmain.cpp:622
void CGhostMain::OnInit(void) {
	CTaskMain::OnInit();
}

// ghostmain.cpp:627
void CGhostMain::OnDestroy(void) {
	m_areas.clear();

	if (m_userPet) { delete m_userPet; m_userPet = 0; }
	if (m_ghostPet) { delete m_ghostPet; m_ghostPet = 0; }
	if (m_userParts) { delete m_userParts; m_userParts = 0; }
	if (m_ghostParts) { delete m_ghostParts; m_ghostParts = 0; }

	if (m_tipImage) { delete m_tipImage; m_tipImage = 0; }
	if (m_userStatImage) { delete m_userStatImage; m_userStatImage = 0; }
	if (m_ghostStatImage) { delete m_ghostStatImage; m_ghostStatImage = 0; }

	if (m_userStat) { delete m_userStat; m_userStat = 0; }
	if (m_ghostStat) { delete m_ghostStat; m_ghostStat = 0; }

	CTaskMain::OnDestroy();
}

// ghostmain.cpp:646
void CGhostMain::OnProcess(float dt) {
	if (IsLocalContent((localContentType_t)0x57))
		RefreshAllUccClothes();

	OnProcTip(dt);

	if (m_userStatAlpha < 255.0f) {
		m_userStatAlpha += dt * 204.0f;
		if (m_userStatAlpha > 255.0f)
			m_userStatAlpha = 255.0f;
	}

	if (m_showUserStat) {
		m_userStat->Process(dt);
		m_ghostStat->Process(dt);
		if (m_equipAlpha < 255.0f) {
			m_equipAlpha += dt * 510.0f;
			if (m_equipAlpha > 255.0f)
				m_equipAlpha = 255.0f;
		}
	} else {
		if (m_statTime < 3.0f)
			m_statTime += dt;

		if (m_statSpeedTime < 4.0f)
			m_statSpeedTime += dt * 1.2f;

		if (m_userStatImage->GetPos().x < m_userStatStopX) {
			m_userStatImage->AddPos(m_userStatSpeed * m_statTime * m_statTime * dt, 0.0f);
			if (m_userStatImage->GetPos().x > m_userStatStopX)
				m_userStatImage->SetPosX(m_userStatStopX);
		}

		if (m_ghostStatImage->GetPos().x > m_ghostStatStopX) {
			m_ghostStatImage->AddPos(-(m_ghostStatSpeed * m_statSpeedTime * dt), 0.0f);
			if (m_ghostStatImage->GetPos().x < m_ghostStatStopX)
				m_ghostStatImage->SetPosX(m_ghostStatStopX);
		}

		if (m_userStatImage->GetPos().x >= m_userStatStopX && m_ghostStatImage->GetPos().x <= m_ghostStatStopX)
			m_showUserStat = true;
	}

	if (g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop"))
		return;

	bool equipOpen = DYNAMIC_CAST(FrEquipDlg, g_pFresh->GetManager()->GetDesktop()->FindChildByName("equip")) != 0;
	bool recycleOpen = DYNAMIC_CAST(FrRecycleDlg, g_pFresh->GetManager()->GetDesktop()->FindChildByName("recycledlg")) != 0;

	if (g_input->GetDown("ESCAPE", true) && !equipOpen && !recycleOpen) {
		OnExit();
		return;
	}

	if (!m_hand.IsWorking(0)) {
		bool rotate = !equipOpen && !recycleOpen;
		if (m_userPet) {
			m_userPet->SetEnableRotate(rotate);
			m_userPet->Process(dt, false);
		}

		if (m_ghostPet) {
			if (m_ghostAlpha < 255.0f) {
				m_ghostAlpha += dt * 127.5f;
				if (m_ghostAlpha > 255.0f)
					m_ghostAlpha = 255.0f;
			}

			if (!m_ghostVisible && m_ghostAlpha > 32.0f)
				m_ghostVisible = true;

			m_ghostPet->SetAlpha((unsigned char)m_ghostAlpha);

			if (!equipOpen && !recycleOpen)
				m_ghostPet->Process(dt, false);
		}
	}

	CTaskMain::OnProcess(dt);
}

// ghostmain.cpp:753
void CGhostMain::OnDisplay(void) {
}

// ghostmain.cpp:833
void CGhostMain::OnInitStart(int) {
}

// ghostmain.cpp:895
void CGhostMain::OnInitPlayerZone(int param) {
	m_areas[0] = DYNAMIC_CAST(FrArea, param);

	FrArea *area = m_areas[0];
	area->SetVisible(true);

	m_userPet = MakePet(area, m_userParts, 0);
}

// ghostmain.cpp:906
void CGhostMain::OnDrawPlayerZone(void) {
	FrWnd *shop = g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop");
	if (DYNAMIC_CAST(FrWnd, shop))
		return;

	if (m_hand.IsWorking(0))
		return;
	if (CRankingInfo::Instance()->GetDlg() == 0 && USERINFODLG() == 0) {
		m_hand.Enter();
		if (m_userPet)
			m_userPet->Display(0.0f, 0.0f);
		m_hand.Leave();
	}
}

// ghostmain.cpp:945
void CGhostMain::OnDownSelectCourse(void) {
}

// ghostmain.cpp:951
void CGhostMain::OnInitUserEquip(int param) {
	m_userEquip = DYNAMIC_CAST(FrListBox, param);
}

// ghostmain.cpp:957
void CGhostMain::OnDrawUserEquip(int param) {
	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();
	FrListItem *pItem = (FrListItem *)param;
	if (pItem == 0 || pGDI == 0)
		return;

	const Bitmap *icon = g_pFresh->GetBitmap(((std::string *)pItem->pData)->c_str());
	if (icon == 0)
		return;

	WRect rect(pItem->pos.x, pItem->pos.y, (float)icon->Width(), (float)icon->Height());

	pGDI->DrawTexture(icon, rect, ((unsigned char)m_equipAlpha << 24) | 0xFFFFFF, 0);

	if (rect.IsInRect(WPoint(g_input->GetMousePoint().x, g_input->GetMousePoint().y))) {
		const Bitmap *frame = g_pFresh->GetBitmap("select_frame");
		if (frame)
			pGDI->DrawTexture(frame, WRect(rect.x - 3.0f, rect.y - 3.0f, rect.w + 6.0f, rect.h + 6.0f), 0xFFFFFFFF, 0);
	}
}

// ghostmain.cpp:1063
bool CGhostMain::OnResultSubEquipBar(int result, FrForm *form) {
	if (result == 1) {
		FrSubEquipBar *bar = (FrSubEquipBar *)form;
		IFF_ITEM_COMMON *item = bar->GetResult();
		if (item != NULL) {
			unsigned long typeId = item->m_typeId;
			unsigned long itemGroup = typeId >> 26;
			if (itemGroup == 5)
				SendPacketEquipResult((PANGYA_ITEM_GROUP)itemGroup, typeId);
			else
				SendPacketEquipResult((PANGYA_ITEM_GROUP)itemGroup, bar->GetLastSelectedGuid());
		} else if (bar->GetCategory() == 7) {
			WSendPacket packet((enumClientPacket)0x0B);
			packet.Encode1(1);
			packet.Encode4(0);
			packet.Send(SEND_TO_DEFAULT);
			unsigned long none = 0;
			*(std::string *)m_selectedIcon->pData = GetIconName(none);
		} else if (bar->GetCategory() == 0x10) {
			WSendPacket packet((enumClientPacket)0x0B);
			packet.Encode1(5);
			packet.Encode4(0);
			packet.Send(SEND_TO_DEFAULT);
			unsigned long none = 0;
			*(std::string *)m_selectedIcon->pData = GetIconName(none);
		}
	}
	return true;
}

// ghostmain.cpp:1199
void CGhostMain::UserPetRebuild(void) {
	if (m_userPet) { delete m_userPet; m_userPet = 0; }
	if (m_userParts) { delete m_userParts; m_userParts = 0; }

	FrArea *area = m_areas[0];
	m_userParts = new CPartTidList;
	m_userPet = MakePet(area, m_userParts, 0);
}

// ghostmain.cpp:1209
bool CGhostMain::CallBackEquipItemResult(int result, FrForm *form) {
	if (form != 0) {
		m_equipDlg = form;
		unsigned long *equipItem = WSingleton<CSharedDoc>::Instance()->m_equipItem;
		memcpy(equipItem, ((FrEquipDlg *)form)->GetMyItemSlot(), sizeof(WSingleton<CSharedDoc>::Instance()->m_equipItem));
		// the first room player is the local one
		memcpy(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_equipItem, equipItem, sizeof(WSingleton<CSharedDoc>::Instance()->m_equipItem));
	}

	m_equipDlg = 0;
	this << MsgObject(0, 1, 0, 0, 0, 0, 0);
	WSendPacket packet((enumClientPacket)0x20);
	packet.Encode1(2);
	packet.EncodeBuffer(WSingleton<CSharedDoc>::Instance()->m_equipItem, sizeof(WSingleton<CSharedDoc>::Instance()->m_equipItem));
	packet.Send(SEND_TO_DEFAULT);

	return true;
}

// ghostmain.cpp:1231
bool CGhostMain::IsFocusToMain(void) {
	if (FrStaticCast<FrWnd>(g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop")))
		return false;

	return true;
}

// ghostmain.cpp:1241
void CGhostMain::OnInitGhostEquip(int param) {
	m_ghostEquip = DYNAMIC_CAST(FrListBox, param);
}

// ghostmain.cpp:1297
void CGhostMain::OnInitGhostZone(int param) {
	m_areas[1] = DYNAMIC_CAST(FrArea, param);

	FrArea *area = m_areas[1];
	area->SetVisible(true);
	m_ghostPet = MakePet(area, m_ghostParts, 1);
	m_ghostPet->SetAlpha(0);
}

// ghostmain.cpp:1308
void CGhostMain::OnDrawGhostZone(void) {
	if ((__rtti_obj = g_pFresh->GetManager()->GetDesktop()->FindChildByName("bongdari_shop")) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&FrBongdariShopDlg::m_RTTI)
		|| (__rtti_obj = g_pFresh->GetManager()->GetDesktop()->FindChildByName("recycledlg")) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&FrRecycleDlg::m_RTTI)
		|| (__rtti_obj = g_pFresh->GetManager()->GetDesktop()->FindChildByName("equip")) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&FrEquipDlg::m_RTTI))
		return;

	if (m_hand.IsWorking(0))
		return;
	if (CRankingInfo::Instance()->GetDlg() == 0 && USERINFODLG() == 0) {
		m_hand.Enter();
		if (m_ghostPet && m_ghostVisible) {
			m_ghostPet->Display(0.0f, 0.0f);
		}
		m_hand.Leave();
	}
}

// ghostmain.cpp:1336
void CGhostMain::OnInitTipZone(int param) {
	m_areas[4] = DYNAMIC_CAST(FrArea, param);

	m_tipAlpha = 0.0f;
}

// ghostmain.cpp:1344
void CGhostMain::OnDrawTipZone(void) {
	m_tipImage->Render((unsigned char)m_tipAlpha);
}

// ghostmain.cpp:1399
void CGhostMain::OnInitAreaUserStat(int param) {
	m_areas[2] = DYNAMIC_CAST(FrArea, param);
}

// ghostmain.cpp:1405
void CGhostMain::OnDrawAreaUserStat(void) {
	m_userStatImage->Render((unsigned char)m_userStatAlpha);
	if (!m_showUserStat)
		return;

	WRect rect = m_areas[2]->GetRect();
	m_userStat->Render(WPoint(rect.x + 86.0f, rect.y + 86.0f));

	FrGraphicInterface *gdi = g_pFresh->GetManager()->GetGDI();
	if (gdi == 0)
		return;

	CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
	int level = doc->m_roomPlayers[0].m_level + 1;
	const Bitmap *bitmap = g_pFresh->GetBitmap(MakeStr("level_%03d", level < 0x47 ? level : 0x47));
	if (bitmap == 0)
		return;

	gdi->DrawTexture(bitmap, WRect(rect.x + 30.0f, rect.y + 3.0f, (float)bitmap->Width(), (float)bitmap->Height()), 0xFFFFFFFF, 0);

	gdi->SetTextColor(0xFFFFFFFF, 0xFF000000);
	gdi->SetTextStyle(2);

	float x = (float)bitmap->Width() + rect.x;

	g_pFresh->GetManager()->PrintText(WPoint(x + 35.0f, rect.y + 5.0f), 0, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_name, -1.0f, 0xFFFFFFFF);
	gdi->SetTextStyle(0);
	gdi->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);
}

// ghostmain.cpp:1435
void CGhostMain::OnInitAreaGhostStat(int param) {
	m_areas[3] = DYNAMIC_CAST(FrArea, param);
}

// ghostmain.cpp:1477
void CGhostMain::OnInitGhostMain(int) {
	SetTimeVariableBGM();
	TurnOff3DBackground();
}

// ghostmain.cpp:1485
void CGhostMain::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0:
		if (g_pFresh == 0)
			return;

		g_pFresh->OpenLayout((const char *)msg.m_param1, this, true);
		switch (msg.m_param2) {
		case 1:
			CGhostHandler::Instance()->OpenNotifyDlg(5);
			break;
		}
		break;

	case 73:
		SetUserEquip((PANGYA_ITEM_GROUP)1, ((const sCharacterInfo *)msg.m_param2)->id);
		break;

	case 74: {
		const sCaddieInfo *caddie = (const sCaddieInfo *)msg.m_param2;
		SetUserEquip((PANGYA_ITEM_GROUP)7, caddie->id);
		break;
	}

	case 75: {
		const sItemInfo *clubSet = (const sItemInfo *)msg.m_param2;
		SetUserEquip((PANGYA_ITEM_GROUP)4, clubSet->m_serial);
		break;
	}

	case 76:
		SetUserEquip((PANGYA_ITEM_GROUP)5, msg.m_param2);
		break;

	case 78: {
		const sMascotInfo *mascot = (const sMascotInfo *)msg.m_param2;
		if (mascot->pcBang)
			COption::Instance()->mSetPCBangMascotMsg(mascot->message);
		SetUserEquip((PANGYA_ITEM_GROUP)16, mascot->id);
		break;
	}

	default:
		HandleMsgCommon(msg);
		break;
	}
}

// ghostmain.cpp:1546
void CGhostMain::SendPacketEquipResult(PANGYA_ITEM_GROUP group, unsigned long typeId) {
	WSendPacket packet((enumClientPacket)0x0B);
	unsigned char kind = 0;
	switch (group) {
	case 1: kind = 4; break;
	case 7: kind = 1; break;
	case 4: kind = 3; break;
	case 5: kind = 2; break;
	case 16: kind = 5; break;
	}
	packet.Encode1(kind);
	packet.Encode4(typeId);
	packet.Send(SEND_TO_DEFAULT);
}
