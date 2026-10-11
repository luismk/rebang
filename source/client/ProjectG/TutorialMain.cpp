// Original: d:\build\custom\temp\code\client\projectg\tutorialmain.cpp
#include "TutorialMain.h"
#include "TutorialMain.inl"
#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "frviewer.h"
#include "frbutton.h"
#include "exhibition.h"
#include "ChatMsg.h"
#include "QuestInfoDlg.h"
#include "frarea.h"
#include "../client/Wangreal/include/wview.h"

#include "RankingDlg.h"
#include "user_info.h"
#include "InputManager.h"
#include "frlistbox.h"
#include "frscrollbar.h"
#include "../shared/Localize.h"
#include "SharedDoc.h"
#include "../client/Wangreal/include/wpuppet.h"
#include "mousecursor.h"
#include "TaskManager.h"

#include <math.h>
#include <windows.h>
#include <mmsystem.h>

// A quest's completion bit (in CSharedDoc::m_tutorialData) and its reward.
struct sQuestReward {
	unsigned long m_mask;   // 0x00
	unsigned long m_typeId; // 0x04
	int m_count;            // 0x08
};
CHECK_SIZE(sQuestReward, 0x0C);

static const sQuestReward s_questRewards[20] = {
	{ 0x1, 0x1A00000F, 3 }, { 0x2, 0x18000007, 3 }, { 0x4, 0x18000005, 3 },
	{ 0x8, 0x18000008, 3 }, { 0x10, 0x1A000010, 500 }, { 0x20, 0x18000004, 3 },
	{ 0x40, 0x1A000010, 500 }, { 0x80, 0, 1 }, { 0x100, 0x1A00000F, 3 },
	{ 0x200, 0x18000028, 1 }, { 0x400, 0x18000006, 1 }, { 0x800, 0x18000007, 5 },
	{ 0x1000, 0x18000000, 4 }, { 0x2000, 0x18000001, 4 }, { 0x8000, 0x18000005, 3 },
	{ 0x10000, 0x18000005, 3 }, { 0x20000, 0x18000005, 3 }, { 0x40000, 0x18000005, 3 },
	{ 0x80000, 0x18000005, 3 }, { 0x80000, 0x18000005, 3 },
};

// StartSelectedTutorial's task change per quest state (m_questState):
// the task and its parameter, then the actor and the message's parameter
// posted once the task runs.
static char s_tutorialTasks[3][16] = { "CGolfTask", "CShopTask", "CRealMyRoomTask" };
static char s_tutorialTaskParams[3][16] = { "CGolfDoc", "", "" };
static char s_tutorialMsgTargets[3][16] = { "", "Shop", "RealMyRoom" };
static char s_tutorialMsgParams[3][16] = { "", "SHOPMAIN", "RealMyRoom" };

CTutorialMain::sTutorialMap CTutorialMain::m_Tutorials[14];

const CTutorialMain::sTutorialCondition CTutorialMain::m_TutoCondition[8] = {
	{ 0, 0, 1, 1 }, { 1, 0, 2, 2 }, { 2, 0, 4, 2 }, { 3, 0, 5, 2 },
	{ 4, 7, 4, 3 }, { 5, 7, 4, 3 }, { 6, 7, 4, 3 }, { 7, 0, 1, 1 },
};

CTutorialMain::sTutorialResult CTutorialMain::m_CompleteItems[3] = {
	{ 0, 3 }, { 1, 3 }, { 2, 3 },
};

CTutorialMain::sArrowInfo CTutorialMain::m_ArrowItems[10] = {
	{ 0, 0, 0 }, { 0, 1, 1 }, { 1, 1, 2 }, { 2, 1, 3 }, { 3, 1, 4 },
	{ 6, 1, 7 }, { 7, 1, 0 }, { 1, 1, 2 }, { 2, 1, 3 }, { 3, 1, 4 },
};

// ProcessAnimateIcon's phase, in degrees.
static float s_iconAngle;

// The images LoadTutorialMainImages registers, indexed by id.
struct sTutorialImage {
	int m_id;            // 0x00
	const char *m_name;  // 0x04
};

static const sTutorialImage s_tutorialImages[64] = {
	{ 0, "tu_btn_01_n" }, { 1, "tu_btn_02_n" }, { 2, "tu_btn_03_n" }, { 3, "tu_btn_04_n" },
	{ 4, "tu_btn_05_n" }, { 5, "tu_btn_06_n" }, { 6, "tu_btn_07_n" }, { 7, "tu_btn_08_n" },
	{ 8, "tu_btn_09_n" }, { 9, "tu_btn_10_n" }, { 10, "tu_btn_11_n" }, { 11, "tu_btn_12_n" },
	{ 12, "tu_btn_13_n" }, { 13, "tu_btn_14_n" },
	{ 14, "tu_btn_01_o" }, { 15, "tu_btn_02_o" }, { 16, "tu_btn_03_o" }, { 17, "tu_btn_04_o" },
	{ 18, "tu_btn_05_o" }, { 19, "tu_btn_06_o" }, { 20, "tu_btn_07_o" }, { 21, "tu_btn_08_o" },
	{ 22, "tu_btn_09_o" }, { 23, "tu_btn_10_o" }, { 24, "tu_btn_11_o" }, { 25, "tu_btn_12_o" },
	{ 26, "tu_btn_13_o" }, { 27, "tu_btn_14_o" },
	{ 28, "tu_btn_01_d" }, { 29, "tu_btn_02_d" }, { 30, "tu_btn_03_d" }, { 31, "tu_btn_04_d" },
	{ 32, "tu_btn_05_d" }, { 33, "tu_btn_06_d" }, { 34, "tu_btn_07_d" }, { 35, "tu_btn_08_d" },
	{ 36, "tu_btn_09_d" }, { 37, "tu_btn_10_d" }, { 38, "tu_btn_11_d" }, { 39, "tu_btn_12_d" },
	{ 40, "tu_btn_13_d" }, { 41, "tu_btn_14_d" },
	{ 42, "tu_btn_lock" }, { 43, "tu_empty_first" }, { 44, "tu_empty_first_n" },
	{ 45, "tu_off_frame01" }, { 46, "tu_off_frame02" }, { 47, "tu_off_frame03" },
	{ 48, "message_lock_n" }, { 49, "tu_icon_clear" }, { 50, "tu_icon_new" },
	{ 51, "tu_arrow_n" }, { 52, "tu_arrow_o" }, { 53, "tu_icon_lock_01" }, { 54, "tu_icon_lock_02" },
	{ 55, "tu_obtain_01" }, { 56, "tu_obtain_02_nuri" }, { 57, "tu_obtain_02_hana" },
	{ 58, "tu_class_01" }, { 59, "tu_class_02" }, { 60, "tu_class_03" },
	{ 61, "tu_tip_01" }, { 62, "tu_tip_02" }, { 63, "tu_tip_03" },
};

extern unsigned long g_CurrentTime;
extern WMatrix g_camera;

// tutorialmain.cpp:174 (run-time type and factory registration of CTutorialMain)
const WRTTI CTutorialMain::m_RTTI("CTutorialMain", &CTaskMain::m_RTTI);

IObject *CTutorialMainMakeInstance(void) {
	return new CTutorialMain;
}

struct __impCTutorialMain {
	__impCTutorialMain(void) {
		ObjectFactory().AddObjectFunctor(CTutorialMainMakeInstance, "CTutorialMain");
		ActorFactory().AddActorClass("CTutorialMain");
	}
};
static __impCTutorialMain s_impCTutorialMain;

// tutorialmain.cpp:176
const sFRESH_MSGMAP *CTutorialMain::GetMessageMap(void) const {
	return &_MsgMap;
}

// tutorialmain.cpp:438
CTutorialMain::~CTutorialMain(void) {
	if (m_pet) { delete m_pet; m_pet = 0; }
}

// tutorialmain.cpp:443
void CTutorialMain::OnPreserveBack(const char *name) {
	HidePet();

	CTaskMain::OnPreserveBack(name);
}

// tutorialmain.cpp:450
void CTutorialMain::OnRestorePreserved(void) {
	CTaskMain::OnRestorePreserved();

	WSingleton<CChatMsg>::Instance()->SetActive(true, true, true);
}

// tutorialmain.cpp:457
void CTutorialMain::OnLoad(void) {
	CTaskMain::OnLoad();

	LoadTutorialMainImages();
}

// tutorialmain.cpp:517
void CTutorialMain::LoadTutorialMainImages(void) {
	for (int i = 0; i < 64; i++)
		m_images[i] = g_pFresh->RegisterBitmap(s_tutorialImages[i].m_name);
}

// tutorialmain.cpp:525
void CTutorialMain::EnableAllBtn(void) {
	for (int i = 0; i < 14; i++)
		m_Tutorials[i].m_enabled = true;
}

// tutorialmain.cpp:536
void CTutorialMain::SetCompleteStates(void) {
	for (int category = 0; category < m_categoryCount; category++) {
		CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
		unsigned char end = 0;
		switch ((unsigned char)category) {
		case 0: end = 8; break;
		case 1: end = 14; break;
		case 2: end = 0; break;
		}
		unsigned char start = 0;
		switch ((unsigned char)category) {
		case 0: start = 0; break;
		case 1: start = 8; break;
		case 2: start = 0; break;
		}
		for (int i = start; i < end; i++) {
			m_Tutorials[i].m_status = 0;
			switch (category) {
			case 0:
				if (doc->m_tutorialData[0] & s_questRewards[i].m_mask)
					m_Tutorials[i].m_status = 1;
				break;

			case 1:
				if (doc->m_tutorialData[0] == 0xFF) {
					if (doc->m_tutorialData[1] & s_questRewards[i].m_mask)
						m_Tutorials[i].m_status = 1;
				} else {
					m_Tutorials[i].m_status = 2;
				}
				break;

			case 2:
				if (doc->m_tutorialData[0] == 0xFF && doc->m_tutorialData[1] == 0x3F00) {
					if (doc->m_tutorialData[2] & s_questRewards[i].m_mask)
						m_Tutorials[i].m_status = 1;
				} else {
					m_Tutorials[i].m_status = 2;
				}
				break;
			}
		}
	}
}

// tutorialmain.cpp:656
void CTutorialMain::ShowPet(void) {
	if (m_pet) { delete m_pet; m_pet = 0; }

	bool loaded;
	CExhibition* pet = new CExhibition(85.0f, 245.0f, 0.35f, false);
	if (pet) {
		pet->SetArea(150.0f, 340.0f);
		pet->SetDistanceRange(9.0f, 13.0f);
		pet->SetDistance(11.0f, true, false);
		loaded = pet->SetModel("pipin", EXHIBIT_CADDIE);
		pet->SetControlFlag(0);
		pet->SetDirection(-0.45f);
		m_pet = pet;
		m_nextMotion = "\xB1\xDE\xC1\xA4\xB0\xC5";

		m_lastMotionTime = 0; m_lastIdleTime = 0; m_selected = false;
		m_lastMotionTime = m_lastIdleTime = timeGetTime();
	}

	if (!loaded) {
		if (pet)
			delete pet;
	}
}

// tutorialmain.cpp:689
void CTutorialMain::HidePet(void) {
	if (m_pet) { delete m_pet; m_pet = 0; }
}

// tutorialmain.cpp:698
void CTutorialMain::ProcessLobbyBgCamera(float dt) {
	if (WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff)
		return;

	if (m_camPaused) {
		if (g_CurrentTime - m_camPauseTime > 2000) {
			m_camDirection *= -1.0f;
			m_camPaused = false;
			m_camMoveTime = g_CurrentTime;
		}
		return;
	}

	unsigned long elapsed = g_CurrentTime - m_camMoveTime;
	if (elapsed > 80000) {
		m_camPaused = true;
		m_camPauseTime = g_CurrentTime;
		return;
	}

	float speed = 1.0f;
	if (elapsed < 2000)
		speed = elapsed * 0.0005f;
	else if (elapsed > 78000)
		speed = 1.0f - (elapsed - 78000) * 0.0005f;

	g_camera.pivot += g_camera.xa * m_camDirection * dt * 0.3f * speed;
}

// tutorialmain.cpp:737
void CTutorialMain::OnProcess(float dt) {
	static float s_motionTime;
	static int s_idleElapsed;
	static int s_motionElapsed;

	if (m_pet) {
		m_pet->Process(dt, false);

		w_motion_data *motion = m_pet->GetMotionData();
		s_motionTime += dt;
		if (s_motionTime > motion->e - motion->s) {
			if (m_nextMotion.size() > 2 && strcmp(m_nextMotion.c_str(), motion->name) != 0) {
				m_pet->SetMotion(m_nextMotion.c_str(), false, 0.0f);
				s_motionTime = 0.0f;
				m_nextMotion = "";
			}
		}

		s_idleElapsed = timeGetTime() - m_lastIdleTime;
		if (s_idleElapsed > 16000) {
			DecideNextMotion(3);
			m_lastIdleTime = timeGetTime();
			s_idleElapsed = 0;
		} else {
			s_motionElapsed = timeGetTime() - m_lastMotionTime;
			if (s_motionElapsed > 5000) {
				DecideNextMotion(rand() % 3);
				m_lastMotionTime = timeGetTime();
			}
		}
	}

	m_iconOffset = cos(s_iconAngle * (3.14159265f / 180.0f)) * 5.0f;
	s_iconAngle += 1.0f;
	if (s_iconAngle > 360.0f)
		s_iconAngle = 0.0f;

	CTaskMain::OnProcess(dt);
}

// tutorialmain.cpp:784
void CTutorialMain::ProcessAnimateIcon(void) {
	m_iconOffset = cos(s_iconAngle * (3.14159265f / 180.0f)) * 5.0f;
	s_iconAngle += 1.0f;
	if (s_iconAngle > 360.0f)
		s_iconAngle = 0.0f;
}

// tutorialmain.cpp:798
void CTutorialMain::OnDestroy(void) {
	m_lastMotionTime = 0;
	m_lastIdleTime = 0;
	m_selected = false;
	m_tutorialList.clear();
	CTaskMain::OnDestroy();
}

// tutorialmain.cpp:914
void CTutorialMain::DisableOverBarButtons(void) {
	if (m_charViewer) {
		m_charViewer->Enable(false);
	}

	if (m_caddieViewer) {
		m_caddieViewer->Enable(false);
	}

	if (m_clubViewer) {
		m_clubViewer->Enable(false);
	}

	if (m_aztecViewer) {
		m_aztecViewer->Enable(false);
	}

	if (m_mascotViewer) {
		m_mascotViewer->Enable(false);
	}

	if (m_oneLineReqBtn) {
		m_oneLineReqBtn->Enable(false);
	}
}

// tutorialmain.cpp:953
void CTutorialMain::DisableUnderBarButtons(void) {
	if (m_barButton[15]) {
		m_barButton[15]->SetPushSound(0);
		m_barButton[15]->EnableHover(false, 0.0f);
		m_barButton[15]->Enable(false);
	}

	if (m_barButton[8]) {
		m_barButton[8]->SetPushSound(0);
		m_barButton[8]->EnableHover(false, 0.0f);
		m_barButton[8]->Enable(false);
	}

	if (m_barButton[14]) {
		m_barButton[14]->EnableHover(false, 0.0f);
		m_barButton[14]->Enable(false);
	}

	if (m_barButton[20]) {
		m_barButton[20]->EnableHover(false, 0.0f);
		m_barButton[20]->Enable(false);
	}
}

// tutorialmain.cpp:1007
void CTutorialMain::OnTutorialMainInit(int) {
	SetTimeVariableBGM();
	TurnOff3DBackground();
}

// tutorialmain.cpp:1013
void CTutorialMain::OnTutorialMainFinish(void) {
	DisableOverBarButtons();
	DisableUnderBarButtons();

	if (WSingleton<CSharedDoc>::Instance()->m_tutorialFinish) {
		this << MsgObject(0, 0xAA, 1, 0, 0, 0, 0);
	}
}

// tutorialmain.cpp:1024
void CTutorialMain::OnTutorialAreaPipin(int param) {
	m_pipinArea = DYNAMIC_CAST(FrArea, param);
}

// tutorialmain.cpp:1029
void CTutorialMain::OnTutorialAreaPipinOwnerDraw(int) {
	if (!m_bongdariShop[2] && !WSingleton<CRankingInfo>::Instance()->GetDlg() && !USERINFODLG()) {
		if (m_pet)
			m_pet->Display(0.0f, 0.0f);
	}
}

// tutorialmain.cpp:1040
void CTutorialMain::OnTutorialListInit(int param) {
	m_tutorialListBox = DYNAMIC_CAST(FrListBox, param);
	if (m_tutorialListBox == 0)
		return;

	m_tutorialListBox->UseRightButton(true);
	m_tutorialListBox->UseDummy(true);
	m_tutorialListBox->SetPushSound(0);

	m_tutorialListBox->ClearItem();
	for (int i = 0; i < 3; i++) {
		m_tutorialListBox->AddItem(&m_CompleteItems[i]);
	}

	memset(m_categoryClear, 0, sizeof(m_categoryClear));
}

// tutorialmain.cpp:1309
void CTutorialMain::DrawCourseArrow(void) {
	for (int i = 0; i < 10; i++) {
		for (int j = m_ArrowItems[i].m_first; j <= m_ArrowItems[i].m_last; j++) {
			int first = GetGradeBegin_Grade(m_Tutorials[j].m_category);
			if (m_Tutorials[j].m_status == 1 || m_Tutorials[j].m_status == 0) {
				m_ArrowItems[first + i].m_show = 0;
			}
			else if (m_Tutorials[j].m_status == 2) {
				m_ArrowItems[first + i].m_show = 1;
			}
		}
	}
}

// tutorialmain.cpp:1332
int CTutorialMain::GetSelectedBtn(int type, WPoint pt) {
	return IsInArea(type, pt);
}

// tutorialmain.cpp:1339
int CTutorialMain::IsInArea(int type, WPoint pt) {
	WPoint *pos;
	unsigned char count = 0;

	switch (type) {
	case 0:
		pos = m_overBarBtnPos;
		count = 8;
		break;
	case 1:
		pos = m_underBarBtnPos;
		count = 6;
		break;
	}

	for (unsigned char i = 0; i < count; i++) {
		if (pt.x > pos[i].x && pos[i].x + 85.0f > pt.x && pt.y > pos[i].y && pos[i].y + 48.0f > pt.y)
			return i;
	}

	return -1;
}

// tutorialmain.cpp:1368
void CTutorialMain::OnTutorialListLBtnDown(void) {
	if (m_tutorialListBox == 0)
		return;

	FrListItem *item = m_tutorialListBox->GetItemUnderCursor();
	if (item == 0)
		return;

	if (!IsLocalContent((localContentType_t)0x46)) {
		if (item->idx >= 1)
			return;
	}

	float rowY[3] = { 0.0f, 0.0f, 0.0f };
	int topRow = m_tutorialListBox->GetScrollBar()->GetCurTopRow_Int();
	if (topRow == 0) {
		rowY[0] = 119.0f;
		rowY[1] = 314.0f;
	}
	else if (topRow == 1) {
		rowY[1] = 119.0f;
		rowY[2] = 314.0f;
	}

	float x = g_input->GetMousePoint().x - item->pos.x;
	float y = g_input->GetMousePoint().y - rowY[item->idx];

	int btn = IsInArea(item->idx, WPoint(x, y));
	if (btn == -1)
		return;

	switch (item->idx) {
	case 0:
		break;
	case 1:
		btn += 8;
		break;
	}

	if (m_Tutorials[btn].m_status != 2) {
		m_Tutorials[btn].m_pending = 1;
		m_pendingTutorial = btn;
	}

	OpenQuest(m_Tutorials[btn].m_category, btn);
}

// tutorialmain.cpp:1425
void CTutorialMain::OnTutorialListLBtnUp(void) {
	if (m_tutorialListBox == 0)
		return;

	FrListItem *item = m_tutorialListBox->GetItemUnderCursor();
	if (item == 0)
		return;

	int end = GetAccumulateLimit(item->idx);
	for (int i = GetGradeBegin_Grade(item->idx); i < end; i++) {
		if (m_Tutorials[i].m_pending == 1) {
			m_Tutorials[i].m_pending = 0;
			break;
		}
	}
}

// tutorialmain.cpp:1446
void CTutorialMain::SetQuestInformation(sTutorialMap *map) {
	m_questState = map->m_state;
	m_questCategory = map->m_category;
	m_questNextIndex = map->m_index;
	m_questIndex = map->m_quest;
}

// tutorialmain.cpp:1457
void CTutorialMain::SetQuestInformation(int category, int index) {
	m_questCategory = category;
	m_questState = 0;
	m_questNextIndex = index + 1;
	m_questIndex = index;
}

// tutorialmain.cpp:1467
void CTutorialMain::OpenQuest(int category, int index) {
	if (!m_Tutorials[index].m_enabled)
		return;

	if (m_Tutorials[index].m_status == 2) {
		m_questionForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify", 0);
		// "You can do it after finishing the previous grade."
		m_questionForm->SetMessage("\xC0\xCC\xC0\xFC \xB5\xEE\xB1\xDE\xC0\xBB \xB8\xB6\xC4\xA3 \xC8\xC4 \xC7\xCF\xBD\xC7 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", false);
		m_questionForm->Open((FRESH_RESULT_CALLBACK)0,
			WPoint((g_view->GetWidth() - m_questionForm->GetRect().w) * 0.5f, 80.0f), 3);
		m_questionForm->EnableDrag(false);

		m_Tutorials[index].m_pending = 0;
		m_pendingTutorial = 0xFF;
		return;
	}

	SetQuestInformation(category, index);
	if (IsCompleteQuest(category, index)) {
		m_questReward = false;

		m_questionForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		// "This quest is already complete, so the clear gift cannot be received.\nPlay it again anyway?"
		m_questionForm->SetMessage("\xC0\xCC\xB9\xCC \xBF\xCF\xB7\xE1\xB5\xC8 \xC4\xF9\xBD\xBA\xC6\xAE\xC0\xCC\xB9\xC7\xB7\xCE, \xC5\xAC\xB8\xAE\xBE\xEE \xB1\xE2\xB3\xE4 \xBC\xB1\xB9\xB0\xC0\xBB \xB9\xDE\xC0\xBB \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \n\xB1\xD7\xB7\xA1\xB5\xB5 \xB4\xD9\xBD\xC3 \xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
		m_questionForm->EnableDrag(false);
		m_questionForm->Open((FRESH_RESULT_CALLBACK)&CTutorialMain::OnQuestionFormResult,
			WPoint((g_view->GetWidth() - m_questionForm->GetRect().w) * 0.5f, 80.0f), 3);
		m_questionForm->MoveCursor("ok");
	}
	else {
		m_questReward = true;
		OpenTutorialInfoDlg();
	}
}

// tutorialmain.cpp:1509
void CTutorialMain::OpenQuestGift(void) {
	m_questGift = CreateForm<FrQuestGift>(g_pFresh->GetManager(), this, "quest_gift", 0);
	m_questGift->SetQuestExplane(m_questIndex);
	m_questGift->Open((FRESH_RESULT_CALLBACK)&CTutorialMain::OnGiftDlgResult,
		WPoint((g_view->GetWidth() - m_questGift->GetRect().w) * 0.5f, 80.0f), 2);
}

// tutorialmain.cpp:1517
void CTutorialMain::OpenGradeGift(unsigned char grade) {
	m_questGift = CreateForm<FrQuestGift>(g_pFresh->GetManager(), this, "quest_gift", 0);
	m_questGift->SetData(grade);
	m_questGift->Open((FRESH_RESULT_CALLBACK)0,
		WPoint((g_view->GetWidth() - m_questGift->GetRect().w + 10.0f) * 0.5f, 80.0f), 2);

	m_gradeGift = 0;
}

// tutorialmain.cpp:1526
void CTutorialMain::DecideNextMotion(unsigned char grade) {
	switch (grade) {
	case 0:
		m_nextMotion = "\xB1\xE2\xBA\xBB";
		break;
	case 1:
		m_nextMotion = "\xB1\xE2\xB4\xD9\xB8\xAE\xB1\xE2";
		break;
	case 2:
		m_nextMotion = "\xC2\xF9\xBD\xBA";
		break;
	case 3:
		m_nextMotion = "\xC0\xE7\xC3\xCB\xC7\xCF\xB1\xE2";
		break;
	case 4:
		m_nextMotion = "\xB1\xDE\xC1\xA4\xB0\xC5";
		break;
	case 5:
		m_nextMotion = "\xC8\xA6\xC0\xCE";
		break;
	case 6:
		m_nextMotion = "\xC8\xA6\xC0\xCE\xBF\xF8";
		break;
	}
}

// tutorialmain.cpp:1557
void CTutorialMain::MissionComplete(unsigned long flags) {
	for (int i = 0; i < 8; i++) {
		if (s_questRewards[i].m_mask & flags)
			MissionClear(i, flags);

		if (i == 7 && (flags & 0x80))
			m_categoryClear[0] = true;
	}
}

// tutorialmain.cpp:1569
void CTutorialMain::MissionClear(int &index, unsigned long &flags) {
	sTutorialInfo *info = FindTutorialList(index);
	if (info) {
		if (flags & s_questRewards[index].m_mask) {
			info->m_cleared = true;
			CheckEnable(index);
		}

		if (index == 7 && (flags & 0x80))
			m_categoryClear[0] = true;
	}
}

// tutorialmain.cpp:1591
void CTutorialMain::OpenTutorialInfoDlg(void) {
	if (m_tutorialInfoDlg == 0) {
		m_tutorialInfoDlg = CreateForm<FrQuestInfoDlg>(g_pFresh->GetManager(), this, "quest_info", 0);
		m_tutorialInfoDlg->SetQuestExplane(m_questIndex);
		m_tutorialInfoDlg->Open((FRESH_RESULT_CALLBACK)&CTutorialMain::OnTutorialInfoDlgResult, 3);
	}
}

// tutorialmain.cpp:1601
bool CTutorialMain::OnTutorialInfoDlgResult(int result, FrForm *form) {
	if (result == 0)
		return true;

	m_tutorialInfoDlg = 0;
	if (result == 1)
		StartSelectedTutorial();

	if (m_pendingTutorial != 0xFF) {
		m_Tutorials[m_pendingTutorial].m_pending = 0;
		m_pendingTutorial = 0xFF;
	}

	return true;
}

// tutorialmain.cpp:1628
bool CTutorialMain::OnGiftDlgResult(int result, FrForm *form) {
	if (result == 1) {
		if (m_gradeGift)
			OpenGradeGift(m_gradeGift);
	}
	return true;
}

// tutorialmain.cpp:1641
void CTutorialMain::StartSelectedTutorial(void) {
	bool valid = true;
	if (m_questState < 0 || (unsigned int)m_questState > 3)
		valid = false;

	if (m_questCategory < 0 || m_questNextIndex < 0 || m_questIndex < 0)
		valid = false;

	if (!valid) {
		HandleMsg(MsgObject(0, 0x23, (intptr_t)"\xBF\xAC\xBD\xC0\xB8\xF0\xB5\xE5 \xBC\xB3\xC1\xA4\xC0\xCC \xC0\xDF\xB8\xF8\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}

	if (m_questState > 0) {
		HandleMsg(MsgObject(0, 0x23, (intptr_t)"Not supported yet.", 0, 0, 0, 0));
		return;
	}

	if (m_pet)
		HidePet();

	switch (m_questState) {
	case 0:
		InitGolfTutorial();
		WSingleton<CMouseCursor>::Instance()->m_mode = 1;
		WSingleton<CMouseCursor>::Instance()->SetActive(false);
		break;
	}

	WSingleton<CTaskManager>::Instance()->ChangeTask(s_tutorialTasks[m_questState], s_tutorialTaskParams[m_questState], true);

	int length = strlen(s_tutorialMsgParams[m_questState]);
	if (length != 0)
		WSingleton<CTaskManager>::Instance()->PostMsg(0, s_tutorialMsgTargets[m_questState], 0, (intptr_t)s_tutorialMsgParams[m_questState], 0, 0, 0);
}

// tutorialmain.cpp:1702
void CTutorialMain::InitGolfTutorial(void) {
	// CSharedDoc 0x4D98 (inside PADDING(0x4D98, 0x4DBC) in SharedDoc.h, not
	// owned by this unit): the tutorial quest the golf task plays.
	*(int *)((char *)WSingleton<CSharedDoc>::Instance() + 0x4D98) = m_questNextIndex;
	WSingleton<CSharedDoc>::Instance()->m_gameMode = 0;

	SetCurMap(0xFD);
	SetGameType(12);

	for (unsigned char i = 0; i < 18; i++)
		WSingleton<CSharedDoc>::Instance()->m_holeOrder[i] = 0;

	SetShotTimeLimit(0);

	WSingleton<CSharedDoc>::Instance()->m_holes = 1;
	WSingleton<CSharedDoc>::Instance()->m_holeOrder[0] = m_questNextIndex < 14 ? 1 : 2;

	// SetWeather (shareddoc.inl) is only declared in SharedDoc.h, not inline,
	// and CSharedDoc::m_weather (0x476E) is protected.
	*((unsigned char *)WSingleton<CSharedDoc>::Instance() + 0x476E) = 0;

	for (std::list<sRoomPlayer>::iterator it = WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_roomPlayerList.end(); ++it)
		it->m_attribute &= ~4;

	WSingleton<CSharedDoc>::Instance()->m_gameOver = false;
}

// tutorialmain.cpp:1734
CTutorialMain::sTutorialInfo *CTutorialMain::FindTutorialList(int &id) {
	sTutorialInfo *found = 0;
	for (std::list<sTutorialInfo>::iterator it = m_tutorialList.begin(); it != m_tutorialList.end(); ++it) {
		sTutorialInfo *info = &(*it);
		if (info->m_id == id) {
			found = info;
			break;
		}
	}
	return found;
}

// tutorialmain.cpp:1752
void CTutorialMain::CheckEnable(int index) {
	int count;
	int unlock = m_TutoCondition[index].m_unlock;
	if (unlock == 0) {
		for (int i = 0; i < m_TutoCondition[index].m_count; i++) {
			int id = m_TutoCondition[index].m_start + i;
			sTutorialInfo *info = FindTutorialList(id);
			if (info == 0)
				return;

			info->m_enabled = true;
		}
	} else {
		int cleared = 0;
		count = m_TutoCondition[index].m_count;
		for (int i = 0; i < count; i++) {
			int id = m_TutoCondition[index].m_start + i;
			sTutorialInfo *info = FindTutorialList(id);
			if (info == 0)
				return;

			if (info->m_cleared == true)
				cleared++;
		}

		if (cleared == count) {
			sTutorialInfo *info = FindTutorialList(unlock);
			if (info == 0)
				return;

			info->m_enabled = true;
		}
	}
}

// tutorialmain.cpp:1800
bool CTutorialMain::IsCompleteQuest(int category, int index) {
	bool complete = false;
	if (m_Tutorials[index].m_category == category) {
		if (WSingleton<CSharedDoc>::Instance()->m_tutorialData[category] & s_questRewards[index].m_mask)
			complete = true;
	}
	return complete;
}

// tutorialmain.cpp:1815
bool CTutorialMain::IsCompleteCategory(int category) {
	int start = category;
	int end = category;
	switch (category) {
	case 0:
		start = 0;
		end = 8;
		break;
	case 1:
		start = 8;
		end = 14;
		break;
	case 2:
		start = 14;
		end = 20;
		break;
	}

	bool complete = true;
	for (int i = start; i < end; ++i) {
		if ((WSingleton<CSharedDoc>::Instance()->m_tutorialData[category] & s_questRewards[i].m_mask) == 0) {
			complete = false;
			break;
		}
	}

	return complete;
}

// tutorialmain.cpp:1848
bool CTutorialMain::OnQuestionFormResult(int result, FrForm *form) {
	if (result == 0)
		return true;

	m_questionForm = 0;
	if (result == 1)
		OpenTutorialInfoDlg();

	if (m_pendingTutorial != 0xFF) {
		m_Tutorials[m_pendingTutorial].m_pending = 0;
		m_pendingTutorial = 0xFF;
	}

	return true;
}
