// Original: d:\build\custom\temp\code\client\projectg\cardboxdlg.cpp
#include "CardBoxDlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "TitleFontEx.h"
#include "exhibition.h"
#include "particle.h"
#include "Packet.h"
#include "actor.h"
#include "TaskManager.h"
#include "SoundManager.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"
#include "commonutil.h"
#include "SharedDoc.h"
#include "wpuppet.h"
#include "wbone.h"
#include "BongdariShopDlg.h"
#include "wtemplate.h"

#include <algorithm>

// wtemplate.h:460-465 (not yet in src/client/ProjectG/wtemplate.h): a list
// finder by the element's m_guid (same as PrivateTradeDlg.cpp's copy).
struct _FindByUid {
	unsigned long m_uid;

	_FindByUid(unsigned long uid) : m_uid(uid) {}
	template <class T>
	bool operator()(const T& item) const { return m_uid == item.m_guid; }
};

// cardboxdlg.cpp:54 (run-time type and factory registration of FrCardBoxDlg)
const WRTTI FrCardBoxDlg::m_RTTI("FrCardBoxDlg", &FrForm::m_RTTI);

IObject* FrCardBoxDlgMakeInstance(void) {
	return new FrCardBoxDlg;
}

struct __sFrCardBoxDlg {
	__sFrCardBoxDlg(void) {
		ObjectFactory().AddObjectFunctor(FrCardBoxDlgMakeInstance, "FrCardBoxDlg");
	}
};
static __sFrCardBoxDlg s_registerFrCardBoxDlg;

// cardboxdlg.cpp:56
const sFRESH_MSGMAP* FrCardBoxDlg::GetMessageMap(void) const {
	return &FrCardBoxDlg::_MsgMap;
}

sFRESH_MSGMAP FrCardBoxDlg::_MsgMap = { &FrForm::_MsgMap, &FrCardBoxDlg::_MsgEntries[0] };

sFRESH_ENTRY FrCardBoxDlg::_MsgEntries[] = {
	// cardboxdlg.cpp:58
	{ "ID_BTN_ABOUT", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardBoxDlg::OnResultBtnInit },
	// cardboxdlg.cpp:59
	{ "ID_BTN_ABOUT", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCardBoxDlg::OnResultBtnUp },
	// cardboxdlg.cpp:60
	{ "ID_BTN_CLOSE", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardBoxDlg::OnCloseBtnInit },
	// cardboxdlg.cpp:61
	{ "ID_BTN_CLOSE", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCardBoxDlg::OnCloseBtnUp },
	// cardboxdlg.cpp:62
	{ "ID_AREA_BG", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardBoxDlg::OnAreaBgInit },
	// cardboxdlg.cpp:63
	{ "ID_AREA_BG", FRCMD_OWNERDRAW, FrFuncVV, (FRESH_PMSG)&FrCardBoxDlg::OnAreaBgOwnerDraw },
	// cardboxdlg.cpp:64
	{ "ID_BTN_PACKOPEN", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardBoxDlg::OnPackOpenBtnInit },
	// cardboxdlg.cpp:65
	{ "ID_BTN_PACKOPEN", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCardBoxDlg::OnPackOpenBtnUp },
	// cardboxdlg.cpp:66
	{ "ID_NEXT_OPEN_OK", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrCardBoxDlg::OnOpenNextBtnInit },
	// cardboxdlg.cpp:67
	{ "ID_NEXT_OPEN_OK", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrCardBoxDlg::OnOpenNextBtnUp },

	// cardboxdlg.cpp:69
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// cardboxdlg.cpp:74
FrCardBoxDlg::FrCardBoxDlg(void)
	: m_pFont(0), m_pExhibition(0), m_state(0), m_pPackOpenBtn(0), m_pOpenNextBtn(0), m_pCloseBtn(0),
	  m_pResultBtn(0), m_unknown13c(0), m_packId(0), m_packOpenShake(0.0f) {
}

// cardboxdlg.cpp:79
FrCardBoxDlg::~FrCardBoxDlg(void) {
	m_map.clear();
	if (m_pParticle) { delete m_pParticle; m_pParticle = 0; }
	if (m_pFont) { delete m_pFont; m_pFont = 0; }
	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }
}

// cardboxdlg.cpp:87
void FrCardBoxDlg::Init(unsigned long typeId, unsigned long packId, int count) {
	FrForm::EnableDrag(false);
	std::list<std::string> names;

	for (int i = 0; i < 5; i++)
		names.push_back(MakeStr("particle_shine_%d", i));

	m_pParticle = new CParticle2D;
	m_pParticle->Initialize(names, m_rcBg, 20, CParticle2D::STYLE_NORMAL, 20.0f, 100.0f, WVector2D(0.0f, 1.0f), WVector2D(0.0f, 1.0f));

	IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(typeId);
	if (pCard == 0 || count < 1) {


		return;
	}



	m_unknown13c = typeId;
	m_packId = packId;

	if (((typeId >> 22) & 0xF) == 4) {
		LoadCardBoxPet(typeId, count);
	} else if (((typeId >> 22) & 0xF) == 3) {
		LoadCardPackPet(typeId);
	}
}

// cardboxdlg.cpp:115
void FrCardBoxDlg::RecievedPacket(WReceivedPacket* pPacket) {
	if (!m_pExhibition)
		return;






	unsigned long result = pPacket->Decode4();
	if (result == 0) {

		if (m_pExhibition) { m_state = 3; }


















		sCards card;
		pPacket->DecodeBuffer(&card, sizeof(sCards));
		std::list<sCards>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(card.m_guid));
		if (it != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {


			if (it->m_typeId == card.m_typeId) {

				it->m_count--;
				if ((int)it->m_count < 1)
					WSingleton<CSharedDoc>::Instance()->m_myCardList.erase(it);
			}
		}

		int count = pPacket->Decode1();
		for (int i = 0; i < count; i++) {

			pPacket->DecodeBuffer(&card, sizeof(sCards));
			unsigned long num = pPacket->Decode4();



			std::map<unsigned long, unsigned int>::iterator mit = m_map.find(card.m_typeId);
			if (mit == m_map.end()) {
				m_map[card.m_typeId] = 1;
			} else {

				mit->second++;
			}



			std::list<sCards>::iterator cit = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(card.m_guid));
			if (cit != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {


				if (cit->m_typeId == card.m_typeId)
					cit->m_count += num;

			} else {

				WSingleton<CSharedDoc>::Instance()->m_myCardList.push_back(card);
			}



			IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(card.m_typeId);
			if (m_pExhibition && pCard) {





				m_pExhibition->ChangeBoneTexture(MakeStr("card_base%02d_Dummy", i + 1), "card_back-00.png", MakeStr("%s.png", pCard->m_model));
			}
		}

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x229, 0, 0, 0, 0, 0));

	} else if (result == 1) {

		FrForm::OnCancel();
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xc4\xab\xb5\xe5\xc6\xd1 \xbf\xc0\xc7\xc2\xbf\xa1 \xbd\xc7\xc6\xd0 \xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0));
	}
}

// cardboxdlg.cpp:210
bool FrCardBoxDlg::OnInit(void) {
	if (!m_pFont) {
		m_pFont = new CWindFont;
	}

	return true;
}

// cardboxdlg.cpp:218
void FrCardBoxDlg::OnProc(const float dt) {
	if (!m_pExhibition)
		return;


	w_motion_data* pMotion = m_pExhibition->GetMotionData();
	if (!pMotion)
		return;









	switch (m_state) {
	case 0:


		if (stricmp(pMotion->name, "default\xb3\xa1") == 0) {


			if (m_unknown13c == 0x7D000000)
				LoadCardPackPet(0x7CC00000);










		}
		break;



	case 1:
		if (stricmp(pMotion->name, "\xc6\xd1\xb4\xd9\xbf\xee\xc1\xbe\xb7\xe1") == 0) {

			if (m_pPackOpenBtn && !m_pPackOpenBtn->IsVisible())
				m_pPackOpenBtn->SetVisible(true);


		}
		break;


	case 2:
		if (stricmp(pMotion->name, "\xc6\xd1\xc1\xa4\xc1\xf6\xbb\xf3\xc5\xc2") == 0) {
			if (m_pExhibition) { m_state = 2; m_pExhibition->SetMotion("\xc6\xd1\xb5\xb9\xb8\xae\xb1\xe2", false, 0.0f); }

		}
		break;



	case 3:
		if (stricmp(pMotion->name, "\xc6\xd1\xc1\xa4\xc1\xf6\xbb\xf3\xc5\xc2") == 0) {
			SetAnimation(4);

		}
		break;



	case 4:
		if (stricmp(pMotion->name, "\xc6\xd1\xbf\xc0\xc7\xc2\xc1\xbe\xb7\xe1") == 0) {
			if (m_pExhibition) { m_state = 5; }

		}
		break;


	case 5: {
		float distance = m_pExhibition->GetDistance();
		if (distance > 4.8f) {



			distance -= dt * 16.199999f;
			if (distance < 4.8f)
				distance = 4.8f;

			m_pExhibition->SetDistance(distance, true, true);

		} else {

			m_pExhibition->SetDistance(4.8f, true, true);
			if (m_pExhibition) { m_state = 6; }

			std::list<sCards>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(m_packId));
			if (it != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {

				if (m_pOpenNextBtn)
					m_pOpenNextBtn->SetVisible(true);
			}




			m_pCloseBtn->Enable(true);
		}

		break;
	}


	case 7:
		if (stricmp(pMotion->name, "\xc4\xab\xb5\xe5\xb9\xf6\xb8\xb2\xc1\xbe\xb7\xe1") == 0) {

			m_pExhibition->SetDistance(10.2f, true, true);










			std::list<sCards>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(m_packId));
			if (it != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {

				if ((it->m_typeId & 0x3C00000) == 0x1000000) {
					LoadCardBoxPet(it->m_typeId, it->m_count);
				} else {


					if (m_pExhibition) { m_state = 1; m_pExhibition->SetMotion("\xc6\xd1\xb4\xd9\xbf\xee\xbd\xc3\xc0\xdb", false, 0.0f); }
				}

			} else {


				FrForm::OnCancel();
			}
		}
		break;
	}







	if (m_pExhibition) {

		m_pExhibition->ProcessBongdariShop(dt);
	}

	if (m_pParticle)
		m_pParticle->Process(dt);

	if (m_pPackOpenBtn && m_pPackOpenBtn->IsVisible()) {


		static bool s_shakeBack = false;	// the pack-open button swings between 0 and 15 pixels right

		if (s_shakeBack) {
			if (SAFE_SUBVALUE(m_packOpenShake, 0.0f, dt * 15.0f) <= 0.0f)
				s_shakeBack = false;

		} else {

			if (SAFE_ADDVALUE(m_packOpenShake, 15.0f, dt * 120.0f) >= 15.0f)
				s_shakeBack = true;
		}


		WRect rc = m_rcPackOpen;
		rc.x += m_packOpenShake;
		m_pPackOpenBtn->SetRect(rc);
	}
}

// cardboxdlg.cpp:381
void FrCardBoxDlg::OnOK(void) {
}

// cardboxdlg.cpp:386
void FrCardBoxDlg::OnCancel(void) {
}

// cardboxdlg.cpp:393
void FrCardBoxDlg::OnResultBtnInit(intptr_t param) {
	m_pResultBtn = DYNAMIC_CAST(FrButton, param);

	m_pResultBtn->SetPushSound("ui_icon_click");
}

// cardboxdlg.cpp:401
void FrCardBoxDlg::OnResultBtnUp(void) {




	std::list<sBonusBall> resultList;
	FrTotalItemDlg* pDlg = CreateForm<FrTotalItemDlg>(g_pFresh->GetManager(), this, "total_item", 0);
	if (pDlg) {

		for (std::map<unsigned long, unsigned int>::iterator it = m_map.begin(); it != m_map.end(); ++it) {





			sBonusBall ball;
			memset(&ball, 0, sizeof(ball));
			ball.m_count = it->second;
			ball.m_typeId = it->first;
			resultList.push_back(ball);
		}


		pDlg->SetListItem(&resultList);
		pDlg->Open(0, 3);
	}
}

// cardboxdlg.cpp:430
void FrCardBoxDlg::OnCloseBtnInit(intptr_t param) {
	m_pCloseBtn = DYNAMIC_CAST(FrButton, param);
}

// cardboxdlg.cpp:437
void FrCardBoxDlg::OnCloseBtnUp(void) {
	FrForm::OnCancel();
}

// cardboxdlg.cpp:443
void FrCardBoxDlg::OnAreaBgInit(intptr_t param) {
	FrArea* pArea = DYNAMIC_CAST(FrArea, param);
	if (pArea) {
		m_rcBg = pArea->GetRect();
	} else {
		m_rcBg = GetRect();
	}
}

// cardboxdlg.cpp:458
void FrCardBoxDlg::OnAreaBgOwnerDraw(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (!m_pExhibition)
		return;



	if (m_state == 0)
		SetPackVisibleInBox();


	Display2DBg();


	m_pExhibition->DisplayClip(m_rcBg, 55.0f, 0.0f, 0.0f);


	if (m_state == 6) {

		std::list<sCards>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(m_packId));
		if (it == WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {


			const Bitmap* pEmpty = g_pFresh->GetBitmap("text_cardpack_empty");
			if (!pEmpty)
				return;



			WRect rc(m_rcBg.x + (m_rcBg.w - (float)pEmpty->Width()) * 0.5f, m_rcBg.y + m_rcBg.h * 0.9f, (float)pEmpty->Width(), (float)pEmpty->Height());
			pGDI->DrawTexture(pEmpty, rc, 0xFFFFFFFF, 0);
		} else {




			const Bitmap* pNext = g_pFresh->GetBitmap("text_cardpack_open_next");
			IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(m_unknown13c);
			if (!pCard)
				return;
			const Bitmap* pIcon = g_pFresh->GetBitmap(pCard->m_icon);

			if (!pIcon || !pNext)
				return;

			WRect rc(m_rcBg);
			rc.x += 80.0f;
			rc.y += rc.h - (float)pIcon->Height() - 4.0f;
			rc.w = (float)pIcon->Width();
			rc.h = (float)pIcon->Height();


			pGDI->DrawTexture(pIcon, rc, 0xFFFFFFFF, 0);

			rc.x += 50.0f;
			rc.y += (float)pIcon->Height() * 0.6f;






			float width;
			std::list<sCards>::iterator cit = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(m_packId));
			if (cit != WSingleton<CSharedDoc>::Instance()->m_myCardList.end()) {
				width = m_pFont->Print(rc.x, rc.y, MakeStr("x %02d", (int)cit->m_count > 99 ? 99 : cit->m_count));
			} else {

				width = m_pFont->Print(rc.x, rc.y, MakeStr("x %02d", -1));
			}




			rc.x += width;

			rc.x += 10.0f; rc.y -= 4.0f; rc.w = (float)pNext->Width();
			rc.h = (float)pNext->Height();



			pGDI->DrawTexture(pNext, rc, 0xFFFFFFFF, 0);
		}
	}
}

// cardboxdlg.cpp:556
void FrCardBoxDlg::OnPackOpenBtnInit(intptr_t param) {
	m_pPackOpenBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pPackOpenBtn) {
		m_rcPackOpen = m_pPackOpenBtn->GetRect();
		m_pPackOpenBtn->SetPushDelay(0.3f);
		m_pPackOpenBtn->SetVisible(false);
		m_pPackOpenBtn->SetPushSound("cardpack_open_click");
	}
}

// cardboxdlg.cpp:570
void FrCardBoxDlg::OnPackOpenBtnUp(void) {
	if (m_pPackOpenBtn->IsVisible() && m_pExhibition) {
		m_pCloseBtn->Enable(false);
		m_pPackOpenBtn->SetVisible(false);

		if (m_pExhibition) { m_state = 2; m_pExhibition->SetMotion("\xc6\xd1\xb5\xb9\xb8\xae\xb1\xe2", false, 0.0f); }

		WSendPacket packet((enumClientPacket)0xc2);
		packet.Encode4(m_unknown13c);
		packet.Encode4(m_packId);
		packet.Send(SEND_TO_DEFAULT);

		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
	}
}

// cardboxdlg.cpp:597
void FrCardBoxDlg::OnOpenNextBtnInit(intptr_t param) {
	m_pOpenNextBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pOpenNextBtn) {
		m_pOpenNextBtn->SetPushDelay(0.3f);
		m_pOpenNextBtn->SetVisible(false);
		m_pOpenNextBtn->SetPushSound("ui_icon_click");
	}
}

// cardboxdlg.cpp:610
void FrCardBoxDlg::OnOpenNextBtnUp(void) {
	if (m_pOpenNextBtn) {
		m_pOpenNextBtn->SetVisible(false);
	}
	if (m_pExhibition) { m_state = 7; m_pExhibition->SetMotion("\xc4\xab\xb5\xe5\xb9\xf6\xb8\xb2\xbd\xc3\xc0\xdb", false, 0.0f); }
}

// cardboxdlg.cpp:619
void FrCardBoxDlg::SetAnimation(unsigned long state) {
	if (!m_pExhibition) {
		return;
	}

	m_state = state;
	switch (state) {
	case 0:
		m_pExhibition->SetMotion("default", false, 0.0f);
		g_audio->PlaySfx("cardbox_open", 0, 0, 0, 0, 0.5f, 200.0f);
		break;
	case 1:
		m_pExhibition->SetMotion("\xc6\xd1\xb4\xd9\xbf\xee\xbd\xc3\xc0\xdb", false, 0.0f);
		break;
	case 2:
		m_pExhibition->SetMotion("\xc6\xd1\xb5\xb9\xb8\xae\xb1\xe2", false, 0.0f);
		break;
	case 4:
		m_pExhibition->SetMotion("\xc6\xd1\xbf\xc0\xc7\xc2\xbd\xc3\xc0\xdb", false, 0.0f);

		if ((m_unknown13c & 0x3c00000) == 0xc00000 && m_unknown13c != 0x7cc00000 && m_unknown13c != 0x7cc00004) {
			g_audio->PlaySfx("cardpack_open_one", 0, 0, 0, 0, 0.5f, 200.0f);
		} else {
			g_audio->PlaySfx("cardpack_open_multi", 0, 0, 0, 0, 0.5f, 200.0f);
		}
		break;
	case 7:
		m_pExhibition->SetMotion("\xc4\xab\xb5\xe5\xb9\xf6\xb8\xb2\xbd\xc3\xc0\xdb", false, 0.0f);
		break;
	}
}

// cardboxdlg.cpp:652
void FrCardBoxDlg::LoadCardBoxPet(unsigned long typeId, int packCount) {
	IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(typeId);
	if (pCard == 0)
		return;





	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }

	m_pExhibition = new CExhibition(m_rcBg.x + m_rcBg.w * 0.5f, m_rcBg.y + m_rcBg.h * 0.5f, 0.0f, true);

	m_pExhibition->SetDistance(10.8f, true, false);
	m_pExhibition->SetArea(m_rcBg.w, m_rcBg.h);

	if (!m_pExhibition->SetModel(MakeStr("%s_open", pCard->m_model), (eExhibitType)12))	// type 12 is not named in exhibition.h yet
		return;




	if (packCount < 10) {

		WBone* pRoot = m_pExhibition->GetModel()->GetRootBone();
		if (pRoot == 0)
			return;






		WBone* pMain = pRoot->FindBone("Main_Dummy01", 0);
		WBone* pPack = pRoot->FindBone(MakeStr("card_pack%02d_Dummy", 10 - packCount), 0);
		if (pMain && pPack) {

			WVector move = pMain->GetLocalMatrix().pivot - pPack->GetLocalMatrix().pivot;












			move.y = pMain->GetMatrix().ym;
			pMain->Move(move, false);
		}
	}



	if (m_pExhibition) { m_state = 0; m_pExhibition->SetMotion("default", false, 0.0f); g_audio->PlaySfx("cardbox_open", 0, 0, 0, 0, 0.5f, 200.0f); }
}

// cardboxdlg.cpp:709
void FrCardBoxDlg::LoadCardPackPet(unsigned long typeId) {
	IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(typeId);
	if (pCard == 0)
		return;





	if (m_pExhibition) { delete m_pExhibition; m_pExhibition = 0; }

	m_pExhibition = new CExhibition(m_rcBg.x + m_rcBg.w * 0.5f, m_rcBg.y + m_rcBg.h * 0.5f, -0.01f, true);

	m_pExhibition->SetDistance(10.2f, true, false);
	m_pExhibition->SetArea(m_rcBg.w, m_rcBg.h);
	if (!m_pExhibition->SetModel(MakeStr("%s_open", pCard->m_model), (eExhibitType)12))	// type 12 is not named in exhibition.h yet
		return;




	if (m_pExhibition) { m_state = 1; m_pExhibition->SetMotion("\xc6\xd1\xb4\xd9\xbf\xee\xbd\xc3\xc0\xdb", false, 0.0f); }
}

// cardboxdlg.cpp:732
void FrCardBoxDlg::SetPackVisibleInBox(void) {





	std::list<sCards>::iterator it = std::find_if(WSingleton<CSharedDoc>::Instance()->m_myCardList.begin(), WSingleton<CSharedDoc>::Instance()->m_myCardList.end(), _FindByUid(m_packId));
	if (it == WSingleton<CSharedDoc>::Instance()->m_myCardList.end())
		return;
	int count = it->m_count;







	if (count >= 10)
		return;



	WBone* pRoot = m_pExhibition->GetModel()->GetRootBone();
	for (int i = 0; i < 10 - count; i++) {

		WBone* pPack = pRoot->FindBone(MakeStr("card_pack%02d_Dummy", i + 1), 0);
		if (pPack)
			pPack->SetInvisible(false, true);
	}
}

// Draws the named Fresh bitmap at (x, y) in its own size, when it exists
// (one source line per bitmap in the original).
#define DRAW_BG_BITMAP(pGDI, name, x, y) { const Bitmap* pBitmap = g_pFresh->GetBitmap(name); if (pBitmap) (pGDI)->DrawTexture(pBitmap, WRect((x), (y), (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0); }

// cardboxdlg.cpp:763
void FrCardBoxDlg::Display2DBg(void) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();


	float x = m_rcBg.x;
	float y = m_rcBg.y;






	DRAW_BG_BITMAP(pGDI, "bg_cardcenter_0", x, y - 28.0f);
	DRAW_BG_BITMAP(pGDI, "bg_cardcenter_1", x + 256.0f, y - 28.0f);
	DRAW_BG_BITMAP(pGDI, "bg_cardcenter_2", x, y + 256.0f - 28.0f);
	DRAW_BG_BITMAP(pGDI, "bg_cardcenter_3", x + 256.0f, y + 256.0f - 28.0f);


	if (m_pParticle)
		m_pParticle->Display();
}
