// Original: d:\build\custom\temp\code\client\projectg\birthdayeventdlg.cpp
#include "birthdayeventdlg.h"
#include "ObjectFactory.h"
#include "frarea.h"
#include "frbutton.h"
#include "frviewer.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"
#include "SimpleOverlay.h"
#include "gamedatadb.h"
#include "gdbitmapbox.h"
#include "gdtextbox.h"
#include "TaskManager.h"
#include "actor.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "../shared/Localize.h"
#include "../client/Wangreal/include/wview.h"

// The boards of the event (one per row, chosen by the user's UID): each
// square holds a mission number (1-based). ns_valueTable is the bit of each
// mission in the completed-missions mask.
namespace BIRTHDAYEVENT {

int board5_5[][25] = {
	{ 13, 2, 10, 25, 1, 12, 8, 22, 5, 16, 19, 6, 15, 14, 24, 17, 7, 4, 21, 18, 9, 20, 23, 11, 3 },
	{ 7, 16, 18, 10, 21, 14, 1, 20, 2, 9, 19, 3, 6, 13, 25, 8, 5, 17, 23, 12, 15, 11, 24, 22, 4 },
	{ 12, 3, 16, 9, 1, 4, 5, 2, 21, 10, 23, 6, 18, 14, 22, 20, 24, 15, 13, 17, 7, 8, 19, 11, 25 },
	{ 6, 2, 21, 3, 17, 22, 20, 19, 8, 10, 14, 5, 13, 25, 1, 7, 23, 11, 18, 16, 24, 9, 12, 15, 4 },
	{ 20, 17, 12, 9, 10, 25, 1, 19, 14, 15, 16, 3, 4, 7, 11, 18, 13, 21, 23, 22, 6, 8, 2, 5, 24 },
	{ 23, 7, 3, 21, 11, 2, 4, 22, 24, 5, 19, 17, 10, 8, 18, 14, 15, 20, 6, 16, 12, 13, 25, 1, 9 },
	{ 23, 21, 3, 4, 16, 2, 15, 24, 11, 10, 13, 18, 17, 6, 22, 5, 9, 8, 7, 1, 25, 14, 12, 20, 19 },
	{ 1, 16, 15, 4, 24, 5, 2, 12, 20, 22, 9, 13, 8, 23, 21, 17, 11, 3, 19, 18, 7, 14, 6, 25, 10 },
	{ 5, 20, 10, 4, 7, 8, 23, 17, 24, 22, 3, 13, 12, 25, 14, 16, 9, 19, 6, 11, 18, 21, 15, 1, 2 },
	{ 25, 15, 17, 7, 18, 19, 13, 20, 22, 11, 1, 14, 21, 5, 16, 9, 4, 2, 8, 24, 10, 6, 12, 3, 23 },
};
int board4_4[][16] = {
	{ 14, 7, 5, 10, 2, 12, 9, 11, 13, 6, 8, 15, 16, 3, 4, 1 },
	{ 3, 14, 12, 1, 15, 6, 13, 11, 4, 7, 8, 2, 5, 10, 16, 9 },
	{ 16, 8, 10, 5, 2, 14, 4, 1, 3, 12, 13, 7, 15, 9, 6, 11 },
	{ 16, 1, 4, 15, 2, 14, 13, 7, 11, 12, 5, 10, 8, 6, 9, 3 },
	{ 13, 11, 6, 5, 16, 2, 4, 8, 12, 1, 7, 15, 14, 9, 3, 10 },
	{ 13, 2, 10, 3, 1, 12, 8, 4, 5, 16, 9, 6, 15, 14, 11, 7 },
	{ 6, 13, 7, 10, 8, 9, 11, 16, 15, 2, 12, 5, 14, 3, 1, 4 },
	{ 3, 9, 1, 15, 14, 11, 4, 6, 13, 16, 10, 12, 2, 7, 8, 5 },
	{ 13, 9, 2, 5, 15, 4, 11, 12, 1, 7, 16, 14, 3, 10, 8, 6 },
	{ 14, 1, 16, 3, 10, 9, 7, 6, 2, 15, 11, 5, 12, 8, 4, 13 },
};
int board3_3[][9] = {
	{ 9, 2, 7, 3, 1, 6, 8, 4, 5 },
	{ 5, 8, 3, 1, 2, 9, 7, 6, 4 },
	{ 9, 5, 7, 6, 2, 4, 8, 3, 1 },
	{ 9, 7, 6, 3, 4, 1, 5, 8, 2 },
	{ 7, 4, 5, 9, 1, 6, 3, 2, 8 },
	{ 3, 2, 7, 6, 1, 8, 4, 9, 5 },
	{ 1, 3, 2, 4, 7, 8, 6, 9, 5 },
	{ 3, 4, 1, 5, 7, 2, 9, 6, 8 },
	{ 9, 3, 5, 7, 6, 1, 8, 2, 4 },
	{ 7, 8, 9, 2, 3, 5, 6, 1, 4 },
};
int ns_valueTable[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152, 4194304, 8388608, 16777216 };

} // namespace BIRTHDAYEVENT

using namespace BIRTHDAYEVENT;

// State of the bouncing "Click" image of FrBirthdayEventDlg::OnProc (no
// symbol): its direction and its speed.
extern "C" {
bool data_abbbf8 = true;
float data_b7cf04;
}

// birthdayeventdlg.cpp:79 (run-time type and factory registration of FrBirthdayEventDlg)
const WRTTI FrBirthdayEventDlg::m_RTTI("FrBirthdayEventDlg", &FrForm::m_RTTI);

IObject* FrBirthdayEventDlgMakeInstance(void) {
	return new FrBirthdayEventDlg;
}

struct __sFrBirthdayEventDlg {
	__sFrBirthdayEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBirthdayEventDlgMakeInstance, "FrBirthdayEventDlg");
	}
};
static __sFrBirthdayEventDlg s_registerFrBirthdayEventDlg;

// birthdayeventdlg.cpp:81
const sFRESH_MSGMAP* FrBirthdayEventDlg::GetMessageMap(void) const {
	return &FrBirthdayEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrBirthdayEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrBirthdayEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBirthdayEventDlg::_MsgEntries[] = {
	// birthdayeventdlg.cpp:83
	{ "Help", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrBirthdayEventDlg::OnLButtonDownHelpButton },
	// birthdayeventdlg.cpp:84
	{ "Gift", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventDlg::OnInitGiftButton },
	// birthdayeventdlg.cpp:85
	{ "Gift", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrBirthdayEventDlg::OnLButtonDownGiftButton },
	// birthdayeventdlg.cpp:86
	{ "board", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventDlg::OnOwnerDrawBoardArea },
	// birthdayeventdlg.cpp:87
	{ "boardNumber", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventDlg::OnInitBoardNumberArea },
	// birthdayeventdlg.cpp:88
	{ "BirthdayEvent_bg1", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventDlg::OnInitBoardBG1Area },

	// birthdayeventdlg.cpp:91
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// birthdayeventdlg.cpp:94
FrBirthdayEventDlg::FrBirthdayEventDlg(void) {
	m_boardIndex = MyUID() % 10;
	m_clickOffset = 15.0f;
}

// birthdayeventdlg.cpp:100
FrBirthdayEventDlg::~FrBirthdayEventDlg(void) {
	if (m_pGiftNumberUI) { delete m_pGiftNumberUI; m_pGiftNumberUI = 0; }
	if (m_pBoardUI) { delete m_pBoardUI; m_pBoardUI = 0; }
}

// birthdayeventdlg.cpp:107
bool FrBirthdayEventDlg::OnInit(void) {
	std::string name = m_eventName + "BirthdayEvent";
	m_pTextBox = (CGDTextBox*)CGameDataDB::Instance()->GetGameData(eGAMEDATADB_TEXTBOX, name.c_str());
	m_pBitmapBox = (CGDBitmapBox*)CGameDataDB::Instance()->GetGameData(eGAMEDATADB_BITMAPBOX, name.c_str());
	InitalizeCoordinates();
	InitalizeImages();
	return FrForm::OnInit();
}

// birthdayeventdlg.cpp:117
void FrBirthdayEventDlg::InitalizeCoordinates(void) {
	m_pBitmapBox->GetRect("BoardBG", m_boards[0].m_rect);
	m_pBitmapBox->GetRect("GiftNumber", m_giftNumberRect);
	m_pBitmapBox->GetRect("Date", m_dateRect);
	m_pBitmapBox->GetRect("Click", m_clickRect);
}

// birthdayeventdlg.cpp:127
void FrBirthdayEventDlg::InitalizeImages(void) {
	m_pBoardUI = new CSimpleUI(m_pBitmapBox->GetFileName("BoardBG"));
	m_pGiftNumberUI = new CSimpleUI(m_pBitmapBox->GetFileName("GiftNumber"));

	for (int i = 0; i < 4; ++i) {
		float y = (float)i;
		for (int j = 0; j < 4; ++j) {
			int index = i * 4 + j;
			float x = (float)j;

			WRect rect;
			rect.x = (m_boards[0].m_rect.w + 1.0f) * x;
			rect.y = (m_boards[0].m_rect.h + 1.0f) * y;
			m_boards[index].m_rect.x = rect.x + m_boards[0].m_rect.x;
			m_boards[index].m_rect.y = (m_boards[0].m_rect.h + 1.0f) * y + m_boards[0].m_rect.y;
			m_boards[index].m_rect.w = rect.w = m_boards[0].m_rect.w;
			m_boards[index].m_rect.h = rect.h = m_boards[0].m_rect.h;
			m_pBoardUI->Add(MakeStr("BoardBG_%d", index + 1), rect, true);

			m_boards[index].m_pNumberImg = g_pFresh->RegisterBitmap(MakeStr("BirthdayEvent_num2_%d", index + 1));
			m_boards[index].m_numberRect.x = (m_boards[0].m_rect.w + 1.0f) * x + 91.0f;
			m_boards[index].m_numberRect.y = (m_boards[0].m_rect.h + 1.0f) * y + 161.0f;
			m_boards[index].m_numberRect.w = 37.0f;
			m_boards[index].m_numberRect.h = 30.0f;
		}
	}

	m_pDateImg = g_pFresh->RegisterBitmap(m_pBitmapBox->GetFileName("Date"));
	m_pClickImg = g_pFresh->RegisterBitmap(m_pBitmapBox->GetFileName("Click"));

	WRect rect(0.0f, 0.0f, m_giftNumberRect.w, m_giftNumberRect.h);
	for (int i = 0; i < 10; ++i) {
		rect.x = (float)i * m_giftNumberRect.w;
		m_pGiftNumberUI->Add(MakeStr("GiftNumber_%d", i), rect, true);
	}

	m_pBoardBG1Area->SetBgImg(m_pBitmapBox->GetFileName("BoardBG1"));
}

// birthdayeventdlg.cpp:176
bool FrBirthdayEventDlg::OnKoohBirthdayEventHelpDlg(int result, FrForm* pForm) {
	if (!IsLocalContent((localContentType_t)0x8E)) {
		return false;
	}

	m_pHelpDlg = 0; m_pBoardNumberArea->SetVisible(true);

	return true;
}

// birthdayeventdlg.cpp:190
void FrBirthdayEventDlg::SetMissionData(unsigned long completed, unsigned long remainGiftCount) {
	for (int i = 0; i < 16; ++i) {
		if (ns_valueTable[board4_4[m_boardIndex][i] - 1] & completed) {
			m_missions[i].m_bCompleted = 1;
		}
	}

	SetRemainGiftCount(remainGiftCount);
}

// birthdayeventdlg.cpp:206
void FrBirthdayEventDlg::SetRemainGiftCount(unsigned long count) {
	m_remainGiftCount = count;
	if (m_pGiftButton) {
		bool bHasGift = count > 0;

		m_pGiftButton->SetVisible(bHasGift);
		m_pGiftButton->Enable(bHasGift);
	}

	m_giftCount = div(count, 10);
}

// birthdayeventdlg.cpp:219
void FrBirthdayEventDlg::SetMissionInfo(std::vector<unsigned long>& missions, std::vector<unsigned long>& values) {
	for (unsigned int i = 0; i < missions.size(); ++i) {
		m_missions[missions[i] - 1].m_value = values[i];
	}
}

// birthdayeventdlg.cpp:227
void FrBirthdayEventDlg::OnMouseMove(const WPoint& pt) {
	CheckToolTip(pt);
}

// birthdayeventdlg.cpp:232
void FrBirthdayEventDlg::OnProc(const float dt) {
	if (m_remainGiftCount > 0) {
		if (data_abbbf8) {
			if (m_clickOffset <= 15.0f) {
				m_clickOffset -= dt * (data_b7cf04 * 27.0f);
				if (data_b7cf04 < 6.0f) {
					data_b7cf04 += 1.0f;
				}
			}

			if (m_clickOffset <= 0.0) {
				m_clickOffset = 0.0f;
				data_abbbf8 = false;
				data_b7cf04 = 6.0f;
			}
		} else {
			if (m_clickOffset >= 0.0f) {
				m_clickOffset += dt * (data_b7cf04 * 27.0f);
				if (data_b7cf04 > 1.0f) {
					data_b7cf04 -= 1.0f;
				}
			}

			if (m_clickOffset >= 15.0f) {
				m_clickOffset = 15.0f;
				data_abbbf8 = true;
				data_b7cf04 = 1.0f;
			}
		}
	}
}

// birthdayeventdlg.cpp:277
void FrBirthdayEventDlg::CheckToolTip(const WPoint& pt) {
	WRect rect; rect.w = m_boards[0].m_rect.w; rect.h = m_boards[0].m_rect.h;

	for (int i = 0; i < 4; ++i) {
		float y = (float)i;
		for (int j = 0; j < 4; ++j) {
			float x = (float)j;

			rect.x = x * m_boards[0].m_rect.w + m_boards[0].m_rect.x + m_rect.x;
			rect.y = y * m_boards[0].m_rect.h + m_boards[0].m_rect.y + m_rect.y;
			if (pt.x >= rect.x && rect.x + rect.w >= pt.x && pt.y >= rect.y && rect.y + rect.h >= pt.y) {
				m_pBoardNumberArea->SetRect(rect);
				MakeToolTipText(board4_4[m_boardIndex][i * 4 + j]);
			}
		}
	}
}

// birthdayeventdlg.cpp:300
void FrBirthdayEventDlg::MakeToolTipText(int mission) {
	int index = mission - 1;

	std::string key(MakeStr("MissionText%d", mission));
	std::string text(m_pTextBox->GetText(key));
	char mapInfo[512];

	int type = 0;
	if (text.find("%d") != std::string::npos) {
		type = 1;
	} else if (text.find("%s") != std::string::npos) {
		type = 2;
	}

	switch (type) {
	case 1:
		m_pBoardNumberArea->SetToolTipText(std::string(MakeStr(text.c_str(), m_missions[index].m_value)));
		break;
	case 2:
		m_pBoardNumberArea->SetToolTipText(std::string(MakeStr(text.c_str(), MakeMapInfoText(index, mapInfo))));
		break;
	default:
		m_pBoardNumberArea->SetToolTipText(text);
		break;
	}
}

// birthdayeventdlg.cpp:381
void FrBirthdayEventDlg::SetEventName(std::string& name) {
	m_eventName = name;
}

// birthdayeventdlg.cpp:388
void FrBirthdayEventDlg::OnLButtonDownHelpButton(void) {
	m_pHelpDlg = CreateForm<FrBirthdayEventHelpDlg>(g_pFresh->GetManager(), this, "KoohBirthdayEventHelp", 0);
	if (m_pHelpDlg) {
		m_pHelpDlg->Open((FRESH_RESULT_CALLBACK)&FrBirthdayEventDlg::OnKoohBirthdayEventHelpDlg, 0x11);
		m_pHelpDlg->OpenHelpDlg(m_pBitmapBox->GetFileName("Explanation"));
	}

	m_pBoardNumberArea->SetVisible(false);
}

// birthdayeventdlg.cpp:400
void FrBirthdayEventDlg::OnInitGiftButton(intptr_t param) {
	m_pGiftButton = DYNAMIC_CAST(FrButton, param);
}

// birthdayeventdlg.cpp:407
void FrBirthdayEventDlg::OnLButtonDownGiftButton(void) {
	if (m_giftCount.quot * 10 + m_giftCount.rem <= 0) {
		CTaskManager* pTaskManager = CTaskManager::Instance();
		(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 0x23,
			(intptr_t)"\xbc\xb1\xb9\xb0 \xc1\xf6\xb1\xde \xc8\xbd\xbc\xf6\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0));
		return;
	}

	WSendPacket packet((enumClientPacket)0xF0);
	packet.Send(SEND_TO_DEFAULT);
	CTaskManager* pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// birthdayeventdlg.cpp:422
void FrBirthdayEventDlg::OnOwnerDrawBoardArea(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			int index = i * 4 + j;

			if (m_missions[index].m_bCompleted) {
				m_pBoardUI->Render(MakeStr("BoardBG_%d", index + 1), m_boards[index].m_rect.x + m_rect.x,
					m_boards[index].m_rect.y + m_rect.y, 1.0f, 0);
			} else {
				WRect rect(m_boards[index].m_numberRect.x + m_rect.x, m_boards[index].m_numberRect.y + m_rect.y,
					m_boards[index].m_numberRect.w, m_boards[index].m_numberRect.h);

				pGDI->DrawTexture(m_boards[board4_4[m_boardIndex][index] - 1].m_pNumberImg, rect, 0xFFFFFFFF, 0);
			}
		}
	}

	WRect rect(0.0f, 0.0f, m_clickRect.w, m_clickRect.h);
	if (m_remainGiftCount > 0) {
		if (m_giftCount.quot) {
			m_pGiftNumberUI->Render(MakeStr("GiftNumber_%d", m_giftCount.quot), m_giftNumberRect.x, m_giftNumberRect.y,
				1.0f, 0x22);
		}
		rect.x = m_clickRect.x + m_clickOffset + m_rect.x;
		rect.y = m_clickRect.y + m_rect.y;
		pGDI->DrawTexture(m_pClickImg, rect, 0xFFFFFFFF, 0);
	}
	m_pGiftNumberUI->Render(MakeStr("GiftNumber_%d", m_giftCount.rem), m_giftNumberRect.x + m_giftNumberRect.w,
		m_giftNumberRect.y, 1.0f, 0x22);
}

// birthdayeventdlg.cpp:471
void FrBirthdayEventDlg::OnInitBoardNumberArea(intptr_t param) {
	m_pBoardNumberArea = DYNAMIC_CAST(FrArea, param);
}

// birthdayeventdlg.cpp:477
void FrBirthdayEventDlg::OnInitBoardBG1Area(intptr_t param) {
	m_pBoardBG1Area = DYNAMIC_CAST(FrArea, param);
}

// birthdayeventdlg.cpp:482 (run-time type and factory registration of FrBirthdayEventHelpDlg)
const WRTTI FrBirthdayEventHelpDlg::m_RTTI("FrBirthdayEventHelpDlg", &FrForm::m_RTTI);

IObject* FrBirthdayEventHelpDlgMakeInstance(void) {
	return new FrBirthdayEventHelpDlg;
}

struct __sFrBirthdayEventHelpDlg {
	__sFrBirthdayEventHelpDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBirthdayEventHelpDlgMakeInstance, "FrBirthdayEventHelpDlg");
	}
};
static __sFrBirthdayEventHelpDlg s_registerFrBirthdayEventHelpDlg;

// birthdayeventdlg.cpp:484
const sFRESH_MSGMAP* FrBirthdayEventHelpDlg::GetMessageMap(void) const {
	return &FrBirthdayEventHelpDlg::_MsgMap;
}

sFRESH_MSGMAP FrBirthdayEventHelpDlg::_MsgMap = { &FrForm::_MsgMap, &FrBirthdayEventHelpDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBirthdayEventHelpDlg::_MsgEntries[] = {
	// birthdayeventdlg.cpp:486
	{ "HelpView", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventHelpDlg::OnInitHelpView },

	// birthdayeventdlg.cpp:488
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// birthdayeventdlg.cpp:490
FrBirthdayEventHelpDlg::FrBirthdayEventHelpDlg(void) {
}

// birthdayeventdlg.cpp:493
FrBirthdayEventHelpDlg::~FrBirthdayEventHelpDlg(void) {
}

// birthdayeventdlg.cpp:498
void FrBirthdayEventHelpDlg::OnInitHelpView(intptr_t param) {
	m_pHelpView = DYNAMIC_CAST(FrViewer, param);
}

// birthdayeventdlg.cpp:504
void FrBirthdayEventHelpDlg::OpenHelpDlg(const char* file) {
	if (m_pHelpView) {
		m_pHelpView->Open(file);
	}
}

// birthdayeventdlg.cpp:516 (run-time type and factory registration of FrBirthdayEventMissionDlg)
const WRTTI FrBirthdayEventMissionDlg::m_RTTI("FrBirthdayEventMissionDlg", &FrForm::m_RTTI);

IObject* FrBirthdayEventMissionDlgMakeInstance(void) {
	return new FrBirthdayEventMissionDlg;
}

struct __sFrBirthdayEventMissionDlg {
	__sFrBirthdayEventMissionDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBirthdayEventMissionDlgMakeInstance, "FrBirthdayEventMissionDlg");
	}
};
static __sFrBirthdayEventMissionDlg s_registerFrBirthdayEventMissionDlg;

// birthdayeventdlg.cpp:518
const sFRESH_MSGMAP* FrBirthdayEventMissionDlg::GetMessageMap(void) const {
	return &FrBirthdayEventMissionDlg::_MsgMap;
}

sFRESH_MSGMAP FrBirthdayEventMissionDlg::_MsgMap = { &FrForm::_MsgMap, &FrBirthdayEventMissionDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBirthdayEventMissionDlg::_MsgEntries[] = {
	// birthdayeventdlg.cpp:520
	{ "MissionNumber", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBirthdayEventMissionDlg::OnInitMissionNumberArea },

	// birthdayeventdlg.cpp:522
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// birthdayeventdlg.cpp:525
FrBirthdayEventMissionDlg::FrBirthdayEventMissionDlg(void) {
	m_completedMission = 0;
	m_fadeTime = 0.0f;
	m_fadeOut = false;

	for (int i = 0; i < 15; ++i) {
		m_pNumberImg[i] = g_pFresh->RegisterBitmap(MakeStr("BirthdayEvent_num2_%d", i + 1));
	}
}

// birthdayeventdlg.cpp:535
FrBirthdayEventMissionDlg::~FrBirthdayEventMissionDlg(void) {
}

// birthdayeventdlg.cpp:539
bool FrBirthdayEventMissionDlg::OnInit(void) {
	return FrForm::OnInit();
}

// birthdayeventdlg.cpp:544
void FrBirthdayEventMissionDlg::OnProc(const float dt) {
	if (m_fadeOut) {
		m_fadeTime += dt;
		if (m_fadeTime > 3.0f) {
			SetFadeout(true);
			m_fadeTime = 0.0f;
			m_fadeOut = false;
		}
	}
}

// birthdayeventdlg.cpp:558
void FrBirthdayEventMissionDlg::SetCompletedMissionInfo(unsigned long mission) {
	m_completedMission = mission;
	m_pMissionNumberArea->SetBgImg(m_pNumberImg[mission - 1]);
}

// birthdayeventdlg.cpp:564
void FrBirthdayEventMissionDlg::CalculatePos(unsigned long index, unsigned long count) {
	WRect rect(0.0f, 0.0f, 0.0f, 0.0f);

	GetClientRect(rect);
	rect.x = g_view->GetWidth() * 0.5f - count * rect.w * 0.5f + index * rect.w;
	SetRect(rect);
}

// birthdayeventdlg.cpp:572
void FrBirthdayEventMissionDlg::OnInitMissionNumberArea(intptr_t param) {
	m_pMissionNumberArea = DYNAMIC_CAST(FrArea, param);
}
