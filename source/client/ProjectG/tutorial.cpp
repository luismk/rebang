// Original: d:\build\custom\temp\code\client\projectg\tutorial.cpp
#include "NetworkSystem.h"
#include "tutorial.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "QuizDlg.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "polysoup.h"
#include "InputManager.h"
#include "mousecursor.h"
#include "TaskManager.h"
#include "ProjectG.h"
#include "commonutil.h"
#include "wind.h"
#include "golfball.h"
#include "CapturedBg.h"
#include "club.h"
#include "../client/Wangreal/include/wutil.h"
#include "../client/Wangreal/include/wmath.h"
#include <math.h>
#include <stdio.h>
#include "frarea.h"
#include "frbutton.h"
#include "../client/Wangreal/include/wview.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/westpak.h"

// tutorial.cpp:29 (run-time type and factory registration of CTutorial)
const WRTTI CTutorial::m_RTTI("CTutorial", &CTaskMain::m_RTTI);

IObject *CTutorialMakeInstance(void) {
	return new CTutorial;
}

struct __impCTutorial {
	__impCTutorial(void) {
		ObjectFactory().AddObjectFunctor(CTutorialMakeInstance, "CTutorial");
		ActorFactory().AddActorClass("CTutorial");
	}
};
static __impCTutorial s_impCTutorial;

// tutorial.cpp:133
AnimatedOverlay::AnimatedOverlay(void) {
	m_dst = WRect(0, 0, 1.0f, 1.0f);
	m_src = WRect(WPoint(0, 0), 1.0f, 1.0f);
	m_frameTime = 0;

	m_elapsed = 0;
	m_frame = 0;
	m_frameCount = 0;
	m_active = false;
	m_visible = false;
	m_loop = false;

	m_fadeDir = 0;
	m_alpha = 255.0f;

	m_overlay = 0;

	m_columns = 0;
}

// tutorial.cpp:156
AnimatedOverlay::~AnimatedOverlay(void) {
	for (std::vector<WOverlay *>::iterator it = m_overlays.begin(); it != m_overlays.end(); ++it) {
		if (g_resrcmng && *it) { g_resrcmng->Release(*it); *it = 0; }
	}
}

// tutorial.cpp:176
void AnimatedOverlay::Process(float dt) {
	if (!m_visible && !m_active) {
		return;
	}

	if (m_frameCount < 1) {
		return;
	}

	if (m_fadeDir == 0 && m_frameTime > 0.0f) {
		m_elapsed += dt;
		if (m_elapsed > m_frameTime) {
			if (m_frameCount == 1) {
				m_visible = !m_visible;
			}
			m_elapsed -= m_frameTime;
			m_frame++;

			if (!m_active || !m_loop) {
				if (m_frame == m_frameCount) {
					m_frame = 0;
					m_elapsed = 0;
					m_frameTime = 0;
					m_visible = m_active;
				}
			} else {
				m_frame %= m_frameCount;
			}
		}
	} else if (m_fadeDir != 0 || m_frameTime == 0.0f) {
		if (m_fadeDir != 0) {
			m_elapsed += dt * 3.0f;
			float alpha = (1 - m_fadeDir) * 128.0f + m_fadeDir * m_elapsed * 255.0f;
			m_alpha = alpha;
			if (alpha < 0.0f || alpha > 255.0f) {
				m_alpha = Between(0.0f, alpha, 255.0f);
				m_fadeDir = 0; m_elapsed = 0; m_visible = m_active;
			}
		}
	}

	m_dst.x = (m_frame % m_columns) * m_dst.w; m_dst.y = (m_frame / m_columns) * m_dst.h;
}

// tutorial.cpp:244
void AnimatedOverlay::Activate(bool active, bool fade, bool loop) {
	m_loop = loop;

	m_active = active;
	if (active) {
		m_frame = 0;
		m_elapsed = 0;
		m_alpha = 255.0f;
		m_visible = active;
	}

	if (fade) {
		m_fadeDir = active ? 1 : -1;
	}
}

// tutorial.cpp:270
void AnimatedOverlay::Render(void) {
	if (!m_visible) {
		return;
	}

	if (m_overlay) {
		m_overlay->Render(g_view, m_dst, m_src, 0, ((int)m_alpha << 24) | 0xFFFFFF, 0, 0);
	}
}

// tutorial.cpp:286
void AnimatedOverlay::SetSourceOverlay(const char *name, int columns, int rows, int mode, float w, float h) {
	if (m_overlays.size()) {
		std::vector<WOverlay *>::iterator it = m_overlays.begin();
		if (g_resrcmng && *it) { g_resrcmng->Release(*it); *it = 0; }
		m_overlays.erase(it);
	}

	WOverlay *overlay = g_resrcmng->GetOverlay(name, 0);

	if (overlay) {
		m_overlays.push_back(overlay);

		m_columns = columns;
		m_overlay = overlay;
		m_frameCount = columns * rows;

		if (mode == 1) {
			m_dst.w = w / overlay->GetWidth();
			m_dst.h = h / overlay->GetHeight();
		} else {
			m_dst.w = 1.0f / columns;
			m_dst.h = 1.0f / rows;
		}
	}
}

// tutorial.cpp:332
void AnimatedOverlay::GetNextOverlay(void) {
	int col = m_frame % m_columns, row = m_frame / m_columns;

	m_dst.x = col * m_dst.w;
	m_dst.y = row * m_dst.h;
}

// tutorial.cpp:419
CTutorial::CTutorial(void) {
	m_tipForm = 0;
	m_yesNoForm = 0;
	m_reviewForm = 0;

	m_tipCaddieForm = 0;
	m_tipCaddieForm2 = 0;
	m_tutorialInfo = 0;

	m_missionForm = 0;
	m_missionOverlay = 0;

	m_quizForm = 0;
	m_reserved270 = 0;

	m_keepMouseBlocked = false;
	m_fakeKeyDown = false;

	memset(m_missionItems, 0, sizeof(m_missionItems));

	m_priority = 4;
	AddSkipList(0x10);

	m_inputBlocked = false;
}

// tutorial.cpp:449
CTutorial::~CTutorial(void) {
	if (m_inputBlocked) {
		FreeInput();
	}
}

// tutorial.cpp:457
void CTutorial::OnInit(void) {
	if (GOLFDOC()->m_gameState == 15) {
		return;
	}

	memset(m_missionCleared, 0, sizeof(m_missionCleared));

	Wind().CheckSpecialWind();

	m_tutorialIndex = -1;
	m_macroTimer = -1;
	m_singleMission = false;
	m_reserved2A0 = 0;
	m_yesNoForm = 0;
	m_failCount = 0;
	m_targetHits = 0;
	m_macroHold = false;
	m_pulseScale = 1.5f;
	m_reserved2D0 = 0;

	m_reserved2D5 = false;
	m_reserved2D8 = 0;

	m_practice = false;

	m_tipRect.x = (g_view->GetWidth() - 370.0f) * 0.5f;
	m_tipRect.y = 50.0f;
	m_tipRect.w = 370.0f;
	m_tipRect.h = 125.0f;

	IsPractice();
}

// tutorial.cpp:491
void CTutorial::OnLoad(void) {
	if (GOLFDOC()->m_gameState == 15) {
		return;
	}

	cFile *file = g_resrcmng->GetCFile("tutorial_text.txt", -1);
	if (file) {
		int size = file->Length();
		char *buffer = new char[size + 1];
		memset(buffer, 0, size + 1);
		file->Read(buffer, size);

		char *cursor = buffer;
		if (cursor) {
			do {
				int length = (int)strlen(cursor);
				if (!length) {
					break;
				}

				int key;
				char line[0x400];
				sscanf(cursor, "%d", &key);
				cursor = GetLine(cursor, line);
				m_texts[key] = line;
			} while (cursor);
		}

		CloseCFile(file);
		delete[] buffer;
	}

	CGolfDoc *doc = GOLFDOC();
	doc->m_target.m_cameraTarget = g_resrcmng->GetPuppet("camera_target.pet", false, true, false);

	OnImageLoad();
}

// tutorial.cpp:532
void CTutorial::OnProcess(float dt) {
	if (GOLFDOC()->m_gameState == 15) {
		return;
	}

	if (m_fakeKey[0]) {
		g_input->SetFakeInput(m_fakeKey, m_fakeKeyDown);
		m_fakeKeyDown = false;
	}

	if (m_specialWait >= 0) {
		m_specialWait -= (int)(dt * 1000.0f);
		if (m_specialWait < 0) {
			if (m_tipCaddieForm && !m_tipCaddieForm2) {
				m_tipCaddieForm->Close(true);
			} else {
				PopMacro();
			}
		}
	}

	if (m_step == 1) {
		if (!m_missionForm && !WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
			OpenMissionForm();
		}

		if (m_step == 1 && m_macroHold) {
			m_macroTimer += (int)(dt * -1000.0f);
			if (m_macroTimer < 0) {
				m_macroTimer = 24000;
				if (GOLFDOC()->m_gameMode & 0x208) {
					Again_Msg(false);
				}
			} else if (m_macroTimer < 15000) {
				Again_Msg(true);
			}
		}
	}

	for (int i = 0; i < 0x1A; i++) {
		m_tutorialOverlays[i].Process(dt);
	}

	m_singleOverlay.Process(dt);
	if (m_singleOverlay.IsActivate()) {
		if (m_singleOverlay.m_src.y > 20.0f) {
			m_singleOverlay.m_src.y -= 2.0f;
		}
		if (m_pulseScale > 1.0f) {
			m_pulseScale -= 0.004f;
		}
		m_singleOverlay.m_src.w = m_pulseScale * 262.0f;
		m_singleOverlay.m_src.h = m_pulseScale * 44.0f;
		m_singleOverlay.m_src.x = (g_view->GetWidth() - m_singleOverlay.m_src.w) * 0.5f;
	}

	for (int i = 0; i < 3; i++) {
		m_chanceOverlays[i].Process(dt);
	}

	switch (m_step) {
	case 1:
		if (GOLFDOC()->m_gameState != 12 && CheckMissionComplete()) {
			MissionComplate();
		}
		break;
	case 4:
		m_step = 5;
		BlockInput();
		OpenMissionForm();
		OpenQuizForm(m_reserved270);
		break;
	case 6:
		if (IsLocalContent((localContentType_t)0x38)) {
			if (m_missionForm) {
				m_missionForm->Close(true);
			}
			if (m_quizCorrect) {
				MissionComplate();
			} else {
				HandleMsg(MsgObject(0, 0x154, 0, 0, 0, 0, 0));
			}
		}
		break;
	}
}

// tutorial.cpp:683
void CTutorial::OnDisplay(void) {
	if (!IsLocalContent((localContentType_t)0x38)) {
		return;
	}

	for (int i = 0; i < 0x1A; i++) {
		m_tutorialOverlays[i].Render();
	}

	m_singleOverlay.Render();

	ChanceOverlay();

	for (int i = 0; i < 3; i++) {
		m_chanceOverlays[i].Render();
	}

	if (WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		return;
	}

	if (m_missionOverlay) {
		m_missionOverlay->Render(g_view, WRect(0, 0, 1.0f, 1.0f), WRect(g_view->GetWidth() - m_missionOverlay->GetWidth() - 10.0f, 40.0f, m_missionOverlay->GetWidth(), m_missionOverlay->GetHeight()), 0, 0xFFFFFFFF, 0, 0);
	}
}

// tutorial.cpp:715
void CTutorial::OnDestroy(void) {
	if (CSharedDoc::Instance()->m_roomType == 0xB || CSharedDoc::Instance()->m_roomType == 0xC) {
		SetCurMap(0);
		CSharedDoc::Instance()->m_roomType = 0;
		strcpy(CSharedDoc::Instance()->m_roomName, CSharedDoc::Instance()->m_gameTypeNames[0].name);
	}

	g_input->FreeKey();
	g_input->FreeMouse();

	ClearMacro();
	m_texts.clear();

	if (g_resrcmng) {
		if (GOLFDOC()->m_target.m_cameraTarget) {
			g_resrcmng->Release(GOLFDOC()->m_target.m_cameraTarget, false);
			GOLFDOC()->m_target.m_cameraTarget = 0;
		}
	}

	if (g_resrcmng && m_missionOverlay) {
		g_resrcmng->Release(m_missionOverlay);
		m_missionOverlay = 0;
	}
}

// tutorial.cpp:737
void CTutorial::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0x148:
		PopMacro();
		break;
	case 0x149:
		if (m_step != 1) {
			PopMacro();
			break;
		}
		if (GOLFDOC()->m_gameState == 12) {
			CheckMissionComplete();
		} else {
			MissionComplate();
		}
		break;
	case 0x14A: {
		const int *pos = (const int *)msg.m_param1;
		m_tipPos[0] = pos[0];
		m_tipPos[1] = pos[1];
		break;
	}
	case 0x14B:
		if (!m_yesNoForm && !m_missionCleared[GOLFDOC()->m_gameState]) {
			m_singleMission = true;
		}
		SkipMission();
		break;
	case 0x14C:
		ReviewTutorial();
		break;
	case 0x152:
		*(unsigned long **)msg.m_param1 = m_missionItems;
		break;
	case 0x154:
		if (!IsLocalContent((localContentType_t)0x38)) {
			break;
		}

		if (m_reviewForm) {
			m_reviewForm->Close(FrCANCEL, true);
		}

		BlockInput();

		g_input->BlockKey("\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9");

		if (m_step == 1 || m_step == 6) {
			m_dialogType = 4;
			m_dialogState = 2;
			m_step = 7;

			switch (GOLFDOC()->m_gameState) {
			case 2:
				m_dialogType = 1;
				OpenTipCaddieForm(m_texts[0x133].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 3:
				OpenTipCaddieForm(m_texts[0x260].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 4:
				if (m_failCount >= 2) {
					m_failCount++;
					OpenTipCaddieForm(m_texts[0x199].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
					OpenMissionFailedForm();
					return;
				}
				m_failCount++;
				m_step = 1;
				FreeInput();
				break;
			case 5:
				OpenTipCaddieForm(m_texts[0x32A].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 6:
				OpenTipCaddieForm(m_texts[0x2C6].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 7:
				OpenTipCaddieForm(m_texts[0x1FE].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 8:
				SendMsg(this, "Letter", 0x18C, 0, 0, 0, 0);
				OpenTipCaddieForm(m_texts[0x38B].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 9:
				OpenTipCaddieForm(m_texts[0x453].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 10:
				OpenTipCaddieForm(m_texts[0x4B8].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 11:
				OpenTipCaddieForm(m_texts[0x3F2].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 12:
				ClearMacro();
				PushMacroHitPointMission();
				m_targetHits = 0;
				SendMsg(this, "GolfRule", 0x89, (intptr_t)"\xBD\xC3\xC0\xDB\xC1\xA1", (intptr_t)"\xB3\xAB\xC7\xCF\xC1\xF6\xC1\xA1\x33", 0, 0);
				PopMacro();
				OpenTipCaddieForm(m_texts[0x51D].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 13:
				OpenTipCaddieForm(m_texts[0x585].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			case 14:
				OpenTipCaddieForm(m_texts[0x5E9].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
				OpenMissionFailedForm();
				return;
			default:
				return;
			}
		}

		SendMsg(this, "Screen", 0x1BA, 1, 0, 0, 0);
		break;
	case 0x155:
		m_reserved2D8 = (float)msg.m_param1;
		if ((float)msg.m_param1 < (float)GOLFDOC()->m_value15C) {
			GOLFDOC()->m_bar.m_accuracy = (float)GOLFDOC()->m_value15C;
			SendMsg(0, "GolfRule", 0xA2, 0, 0, 0, 0);
		}
		break;
	case 0x156:
		PopSpecial();
		break;
	case 0x20F:
		switch (msg.m_param1) {
		case 0:
			m_keepMouseBlocked = true;
			g_input->BlockMouse(LEFT_BUTTON);
			BlockInput();
			break;
		case 1:
			m_keepMouseBlocked = false;
			g_input->FreeMouse(LEFT_BUTTON);
			break;
		case 2: {
			const float *value = (const float *)msg.m_param2;
			GOLFDOC()->m_bar.m_accuracy = *value;
			if (msg.m_param3 == 1) {
				GOLFDOC()->m_bar.m_state = 1;
			} else if (msg.m_param3 == 2) {
				GOLFDOC()->m_bar.m_prevAccuracy = *value;
			}
			break;
		}
		}
		break;
	}
}

// tutorial.cpp:996
bool CTutorial::CheckMissionComplete(void) {
	switch (GOLFDOC()->m_gameState) {
	case 0:
		return CheckCameraTarget();
	case 1:
		return CheckClubTarget();
	case 12:
		m_targetHits++;

		if (m_targetHits == 4) {
			MissionComplate();
		}
		BlockInput();
		WSingleton<CMouseCursor>::Instance()->SetActive(true);

		if (m_tipForm) {
			m_tipForm->Close(FrCANCEL, true);
		}
		PopMacro();
		return true;
	}

	return false;
}

// tutorial.cpp:1033
void CTutorial::BlockInput(void) {
	g_input->BlockKey();

	g_input->FreeKey("ESCAPE");
	g_input->FreeKey("ENTER");
	g_input->FreeKey("PADENTER");
	g_input->FreeKey("FUNC_11");

	WSingleton<CMouseCursor>::Instance()->m_inputEnable = false;

	g_input->BlockMouse(MIDDLE_BUTTON);
	g_input->BlockMouse(RIGHT_BUTTON);

	m_inputBlocked = true;
}

// tutorial.cpp:1050
void CTutorial::FreeInput(void) {
	g_input->FreeKey();

	g_input->FreeMouse();
	g_input->FreeMouse(LEFT_BUTTON);
	g_input->FreeMouse(MIDDLE_BUTTON);
	g_input->FreeMouse(RIGHT_BUTTON);

	WSingleton<CMouseCursor>::Instance()->m_inputEnable = true;

	m_inputBlocked = false;
}

// tutorial.cpp:1065
char *CTutorial::GetLine(char *src, char *dst) {
	while (*src != '"' && *src != 0)
		src++;

	if (*src == 0)
		return 0;

	for (src++; *src != 0 && *src != '"';) {
		if (*src == '\\' && src[1] == 'n') {
			*dst++ = '\n';
			src += 2;
		} else {
			*dst++ = *src++;
		}
	}

	if (*src == 0)
		return 0;

	src++;
	*dst = 0;
	return src;
}

// A 12-byte entry per tutorial (data at 0x9F63C0); only its first int is read.
struct sTutorialTableEntry {
	int m_info;        // 0x0
	int m_reserved[2]; // 0x4
};
extern "C" const sTutorialTableEntry data_9f63c0[];

// tutorial.cpp:1097
void CTutorial::IsPractice(void) {
	CTask *pTask = AfxGetPreservedTask();
	if (pTask != 0) {
		IActor *pActor = pTask->GetActor("TutorialMain");
		if (pActor != 0) {
			int value1;
			int value2;
			int mode;
			int value4;
			pActor << MsgObject(0, 0x14F, (intptr_t)&value1, (intptr_t)&value2, (intptr_t)&mode, (intptr_t)&m_tutorialIndex, (intptr_t)&value4);
			GOLFDOC()->m_gameState = mode;

			m_tutorialInfo = data_9f63c0[m_tutorialIndex].m_info;
		}
	}

	m_yesNoForm = 0;
	WSingleton<CMouseCursor>::Instance()->Store();

	for (int i = 0; i < 0x1A; i++) {
		m_tutorialOverlays[i].Activate(false, true, true);
	}

	BlockInput();

	SendMsg(this, "Screen", 0x1BC, 2, 0, 0, 0);
	InitTutorialMode(GOLFDOC()->m_gameState);
}

// tutorial.cpp:1133
void CTutorial::ReviewTutorial(void) {
	if (m_reviewForm != 0) {
		return;
	}

	BlockInput();

	WSingleton<CMouseCursor>::Instance()->SetActive(true);
	if (m_tipForm) {
		m_tipForm->Close(FrCANCEL, true);
	}
	m_step = 3;

	m_reviewForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
	m_reviewForm->SetMessage("\xBC\xB3\xB8\xED\xC0\xBB \xB4\xD9\xBD\xC3 \xB5\xE9\xC0\xB8\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	m_reviewForm->EnableDrag(false);
	m_reviewForm->Open((FRESH_RESULT_CALLBACK)&CTutorial::OnReviewFormResult,
		WPoint((g_view->GetWidth() - m_reviewForm->GetRect().w) * 0.5f, 80.0f), 2);
	m_reviewForm->MoveCursor("ok");
}

// tutorial.cpp:1238
void CTutorial::MissionComplate(void) {
	if (m_yesNoForm != 0) {
		return;
	}

	if (m_missionForm) {
		m_missionForm->Close(FrNONE, true);
	}

	if (!m_singleMission) {
		m_missionCleared[GOLFDOC()->m_gameState] = true;
	}

	BlockInput();

	WSingleton<CMouseCursor>::Instance()->SetActive(true);
	if (m_tipForm) {
		m_tipForm->Close(FrCANCEL, true);
	}
	m_step = 3;

	m_singleOverlay.m_src.w = 393.0f;
	m_singleOverlay.m_src.h = 66.0f;
	m_singleOverlay.m_src.x = (g_view->GetWidth() - 393.0f) * 0.5f;
	m_singleOverlay.m_src.y = (g_view->GetHeight() - 66.0f) * 0.5f;

	m_singleOverlay.m_frameTime = 0;

	m_singleOverlay.Activate(true, true, true);
	m_macroHold = false;
	PopMacro();
}

// tutorial.cpp:1277
void CTutorial::PrepareMission(void) {
	m_failCount = 0;

	if (GOLFDOC()->m_gameState == 3) {
		GOLFDOC()->GetPlayer(0)->m_comboGauge = 33.0f;
	} else if (GOLFDOC()->m_gameState == 5) {
		m_missionItems[0] = 0x18000004;
		m_missionItems[1] = 0x18000004;
		m_missionItems[2] = 0x18000004;
		GOLFDOC()->GetPlayer(0)->m_numItems = 3;
	} else if (GOLFDOC()->m_gameState == 8) {
		GOLFDOC()->GetPlayer(0)->m_ghostAward = 0;
		GOLFDOC()->GetPlayer(0)->m_powerShotLevel = 0;
		sPlayerData *player = GOLFDOC()->GetPlayer(0);
		player->m_holes[GOLFDOC()->m_currentHole - 1] = 1;
		SendMsg(this, "GolfRule", 0x89, (intptr_t)"\xbd\xc3\xc0\xdb\xc1\xa1", 0, 0, 0);
	}

	switch (GOLFDOC()->m_gameState) {
	case 2:
	case 3:
	case 5:
	case 11:
	case 13:
	case 14:
		SendMsg(this, "GolfRule", 0x89, (intptr_t)"\xbd\xc3\xc0\xdb\xc1\xa1", (intptr_t)"\xb3\xab\xc7\xcf\xc1\xf6\xc1\xa1" "3", 0, 0);
		break;
	}
}

// tutorial.cpp:1319
void CTutorial::SkipMission(void) {
	if (m_yesNoForm != 0) {
		return;
	}

	BlockInput();

	WSingleton<CMouseCursor>::Instance()->SetActive(true);
	if (m_tipForm) {
		m_tipForm->Close(FrCANCEL, true);
	}
	m_step = 3;

	m_yesNoForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
	m_yesNoForm->SetMessage("\xC7\xF6\xC0\xE7 \xC1\xF8\xC7\xE0\xC1\xDF\xC0\xCE \xC4\xF9\xBD\xBA\xC6\xAE\xB8\xA6 \xC6\xF7\xB1\xE2\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?", false);
	m_yesNoForm->EnableDrag(false);
	m_yesNoForm->Open((FRESH_RESULT_CALLBACK)&CTutorial::OnSkipMissionFormResult,
		WPoint((g_view->GetWidth() - m_yesNoForm->GetRect().w) * 0.5f, 80.0f), 2);
	m_yesNoForm->MoveCursor("ok");
}

// tutorial.cpp:1345
void CTutorial::OnImageLoad(void) {
	char name[260];

	m_tutorialOverlays[0].SetSourceOverlay("tuto_point.tga", 4, 1, 0, 1.0f, 1.0f);
	m_singleOverlay.SetSourceOverlay("text_questclear.png", 1, 1, 0, 1.0f, 1.0f);
	m_chanceOverlays[0].SetSourceOverlay("tutorial_icon_chance.png", 1, 1, 0, 1.0f, 1.0f);
	m_chanceOverlays[1].SetSourceOverlay("tutorial_icon_chance.png", 1, 1, 0, 1.0f, 1.0f);
	m_chanceOverlays[2].SetSourceOverlay("tutorial_icon_chance.png", 1, 1, 0, 1.0f, 1.0f);

	for (int i = 1; i < 26; i++) {
		if (i < 20) {
			sprintf(name, "tutorial_keyanmaion%02d.png", i);
		} else {
			sprintf(name, "tutorial_ground%02d.png", i - 20);
		}

		switch (i) {
		case 1:
			m_tutorialOverlays[1].SetSourceOverlay(name, 3, 1, 1, 128.0f, 88.0f);
			break;
		case 3:
			m_tutorialOverlays[3].SetSourceOverlay(name, 2, 1, 1, 128.0f, 88.0f);
			break;
		case 4:
			m_tutorialOverlays[4].SetSourceOverlay(name, 3, 1, 1, 136.0f, 88.0f);
			break;
		case 5:
			m_tutorialOverlays[5].SetSourceOverlay(name, 2, 1, 1, 136.0f, 88.0f);
			break;
		case 12:
			m_tutorialOverlays[12].SetSourceOverlay(name, 2, 4, 1, 136.0f, 105.0f);
			break;
		case 13:
		case 14:
			m_tutorialOverlays[i].SetSourceOverlay(name, 2, 1, 1, 136.0f, 105.0f);
			break;
		case 15:
			m_tutorialOverlays[15].SetSourceOverlay(name, 2, 2, 1, 136.0f, 105.0f);
			break;
		case 20:
		case 21:
		case 22:
			m_tutorialOverlays[i].SetSourceOverlay(name, 1, 1, 0, 1.0f, 1.0f);
			break;
		case 23:
			m_tutorialOverlays[23].SetSourceOverlay(name, 2, 1, 1, 128.0f, 88.0f);
			break;
		case 24:
			sprintf(name, "tu_icon_click.tga");
			m_tutorialOverlays[24].SetSourceOverlay(name, 1, 1, 0, 1.0f, 1.0f);
			break;
		case 25:
			sprintf(name, "tu_arrow_click.tga");
			m_tutorialOverlays[25].SetSourceOverlay(name, 1, 1, 0, 1.0f, 1.0f);
			break;
		default:
			m_tutorialOverlays[i].SetSourceOverlay(name, 2, 1, 1, 128.0f, 88.0f);
			break;
		}
	}

	m_tutorialOverlays[23].SetSourceOverlay("tutorial_preview.png", 1, 1, 0, 1.0f, 1.0f);
}

// tutorial.cpp:1475
void CTutorial::Again_Msg(bool retry) {
	if (m_step == 1) {
		if (retry) {
			if (m_tipCaddieForm) {
				m_tipCaddieForm->Close(true);
			}
			return;
		}

		m_dialogType = 1;
		m_dialogState = 0;

		int textKey = 5;
		switch (GOLFDOC()->m_gameState) {
		case 1: textKey = 0xD5; break;
		case 2: textKey = 0x134; break;
		case 3: textKey = 0x19A; break;
		case 4: textKey = 0x25F; break;
		case 5: textKey = 0x329; break;
		case 9: textKey = 0x453; break;
		case 10: textKey = 0x4B9; break;
		case 11: textKey = 0x3F3; break;
		case 12: textKey = 0x51D; break;
		case 13: textKey = 0x586; break;
		case 14: textKey = 0x5EA; break;
		}

		if (m_tipCaddieForm == 0) {
			OpenTipCaddieForm(m_texts[textKey].c_str(), (FRESH_RESULT_CALLBACK)&CTutorial::OnTipCaddieNoneResult);
		}

		if (m_tipCaddieForm) {
			m_tipCaddieForm->EnableFlag(2);
		}

		if (m_tipCaddieForm2) {
			m_tipCaddieForm2->Enable(true);
		}

		WSingleton<CMouseCursor>::Instance()->ForceMove(g_view->GetWidth() * 0.5f, g_view->GetHeight() * 0.5f, -1.0f);
	}
}

// tutorial.cpp:1529
void CTutorial::ChanceOverlay(void) {
	if (m_step == 1 && GOLFDOC()->m_gameState == 4) {
		for (int i = 0; i < 3; i++) {
			if (i < 3 - m_failCount) {
				AnimatedOverlay &chance = m_chanceOverlays[i];
				chance.m_src.x = g_view->GetWidth() - (i + 1) * 62.0f - 20.0f;
				chance.m_src.y = 160.0f;
				chance.m_src.w = 58.0f;
				chance.m_src.h = 53.0f;
				chance.Activate(true, true, true);
			} else {
				m_chanceOverlays[i].Activate(false, true, true);
			}
		}
	} else {
		for (int i = 0; i < 3; i++)
			m_chanceOverlays[i].Activate(false, true, true);
	}
}

// tutorial.cpp:1564
void CTutorial::PriviewOverlay(void) {
	PushMacro((eMacroType)0x13, MakeStr("%d %.1f %.1f", 0x17, (g_view->GetWidth() - 128.0f) * 0.5f, g_view->GetHeight() * 0.25f), 0, 1);
	PushMacro((eMacroType)0x10, MakeStr("%d %.1f %.1f %.1f", 0x17, 128.0, 64.0, 0.0), 0, 1);
	PushMacro(MACRO_WAIT, 0, 2000, 1);
	PushMacro((eMacroType)0x12, MakeStr("%d", 0x17), 500, 1);
}

// tutorial.cpp:1574
bool CTutorial::OnTipFormResult(int result, FrForm *form) {
	m_tipForm = 0;
	if (result == FrCANCEL) {
		return true;
	}

	if (m_keepMouseBlocked) {
		g_input->BlockMouse(LEFT_BUTTON);
	} else {
		FreeInput();
	}

	WSingleton<CMouseCursor>::Instance()->Store();

	if (m_tipState == 6) {
		m_tipState = 0;
		MissionComplate();
	} else {
		PopMacro();
	}

	return true;
}

extern bool g_bQuit;

// tutorial.cpp:1612
bool CTutorial::OnCompleteFormResult(int result, FrForm *form) {
	m_yesNoForm = 0;
	WSingleton<CMouseCursor>::Instance()->Store();

	if (!IsLocalContent((localContentType_t)0x38)) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 11 || GOLFDOC()->m_gameState == 7) {
			if (WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)0)) {
				CProjectG::Instance()->FromGolfToLobbySettings();

				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", false);
				CTaskManager::Instance()->PostMsg(0, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			} else {
				g_bQuit = true;
			}
		} else {
			if (m_singleMission && result == 1) {
				m_reserved2A0 += result;
			}
			m_singleMission = false;
		}

		SendMsg(this, "Screen", 0x1BC, result == 1 ? 2 : 1, 0, 0, 0);
		InitTutorialMode(GOLFDOC()->m_gameState + (result == 1));
	} else {
		if (result != 0) {
			if (!WSingleton<WNetworkSystem>::Instance()->IsConnected((WNetworkSystem::eNetUnit)0)) {
				g_bQuit = true;
			}
		}
	}

	return true;
}

// tutorial.cpp:1670
bool CTutorial::OnReviewFormResult(int result, FrForm *form) {
	m_reviewForm = 0;

	WSingleton<CMouseCursor>::Instance()->Store();

	if (result == 1) {
		SendMsg(this, "Screen", 0x208, 0, 0, 0, 0);
		SendMsg(this, "Screen", 0x1BC, 1, 0, 0, 0);
		InitTutorialMode(GOLFDOC()->m_gameState);
	} else if (result == 2) {
		if (GOLFDOC()->m_gameState == 6 || GOLFDOC()->m_gameState == 7) {
			m_step = 4;
		} else {
			FreeInput();

			m_step = 1;

			switch (GOLFDOC()->m_gameState) {
			case 2:
			case 3:
			case 4:
			case 5:
			case 8:
			case 10:
			case 11:
			case 13:
			case 14:
				break;
			default:
				OpenMissionForm();
				SendMsg(this, "Screen", 0x1BC, 2, 0, 0, 0);
				SendMsg(this, "Screen", 0x1BA, 1, 0, 0, 0);
				break;
			}
		}
	}

	return true;
}

// tutorial.cpp:1715
bool CTutorial::OnSkipMissionFormResult(int result, FrForm *form) {
	m_yesNoForm = 0;

	WSingleton<CMouseCursor>::Instance()->Store();

	if (m_singleMission == true && result == 1) {
		if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
			CProjectG::Instance()->FromGolfToLobbySettings();
			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(0, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);
		}
		return true;
	}

	if (GOLFDOC()->m_gameState == 6 || GOLFDOC()->m_gameState == 7) {
		m_step = 4;
		return true;
	}

	m_step = 1;
	FreeInput();
	OpenMissionForm();
	SendMsg(this, "Screen", 0x1BC, 2, 0, 0, 0);
	SendMsg(this, "Screen", 0x1BA, 1, 0, 0, 0);
	return true;
}

// tutorial.cpp:1751
bool CTutorial::OnPracticeFormResult(int result, FrForm *form) {
	m_yesNoForm = 0;
	WSingleton<CMouseCursor>::Instance()->Store();

	if (result == FrOK) {
		m_practice = true;
	}

	for (int i = 0; i < 0x1A; i++) {
		m_tutorialOverlays[i].Activate(false, true, true);
	}

	BlockInput();

	InitTutorialMode(GOLFDOC()->m_gameState);

	return true;
}

// Names of the tip form's three buttons ("ok", "yes", "keep"), indexed by m_dialogState (no symbol).
extern "C" const char data_9f64b0[][8];
// Pipin's face images of the tip form, indexed by m_dialogType (no symbol).
extern "C" const char data_9f64c8[][32];

// tutorial.cpp:1777
void CTutorial::OpenTipCaddieForm(const char *text, FRESH_RESULT_CALLBACK callback) {
	if (m_reviewForm) {
		m_reviewForm->Close(FrCANCEL, true);
	}

	if (m_tipCaddieForm == 0 && g_view != 0) {
		std::string message(text);
		if (message.size() <= 75) {
			m_tipCaddieForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "tip_imageBox", 0);
		} else if (message.size() <= 100) {
			m_tipCaddieForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "tip_MimageBox", 0);
		} else {
			m_tipCaddieForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "tip_BimageBox", 0);
		}

		m_tipCaddieForm->SetMessage(text, false);
		m_tipCaddieForm->EnableDrag(false);

		FrArea *face = DYNAMIC_CAST(FrArea, m_tipCaddieForm->FindChildByName("pipin_face"));
		if (face) {
			face->SetBgImg(data_9f64c8[m_dialogType]);
		}

		for (unsigned int i = 0; i < 3; i++) {
			void *child = m_tipCaddieForm->FindChildByName(data_9f64b0[i]);
			__rtti_obj = child;
			FrButton *button = (FrButton *)(child != 0 ? (((IObject *)child)->IsKindOf(&FrButton::m_RTTI) ? child : 0) : 0);
			if (button) {
				bool selected = (i == (unsigned int)m_dialogState);
				if (selected) {
					m_tipCaddieForm2 = button;
				}
				button->SetVisible(selected);
				button->Enable(false);
			}
		}

		m_tipRect.x = (g_view->GetWidth() - m_tipCaddieForm->GetRect().w) * 0.5f;
		m_tipRect.y = 50.0f;
		m_tipRect.w = m_tipCaddieForm->GetRect().w;
		m_tipRect.h = m_tipCaddieForm->GetRect().h;
		m_tipCaddieForm->SetKeyEvent(true);
		m_tipCaddieForm->Open(callback,
			WPoint((g_view->GetWidth() - m_tipCaddieForm->GetRect().w) * 0.5f, 50.0f), 4);
		m_tipCaddieForm->MoveCursor(data_9f64b0[m_dialogState]);
	}
}

// tutorial.cpp:1839
bool CTutorial::OnTipCaddieFormResult(int result, FrForm *form) {
	m_tipCaddieForm = 0;
	m_tipCaddieForm2 = 0;
	if (result == FrCANCEL) {
		return true;
	}
	if (result == FrNONE) {
		return true;
	}

	WSingleton<CMouseCursor>::Instance()->Store();

	if (m_tipState == 6) {
		m_tipState = 0;
		MissionComplate();
	} else {
		PopMacro();
	}

	return true;
}

// tutorial.cpp:1869
bool CTutorial::OnTipCaddieReopenQuizResult(int result, FrForm *form) {
	m_tipCaddieForm = 0;
	m_tipCaddieForm2 = 0;
	m_step = 4;
	return true;
}

// tutorial.cpp:1877
bool CTutorial::OnTipCaddieNoneResult(int result, FrForm *form) {
	m_tipCaddieForm = 0;
	m_tipCaddieForm2 = 0;

	return true;
}

// tutorial.cpp:1934
bool CTutorial::OnMissionFormResult(int result, FrForm *form) {
	if (m_tipCaddieForm) {
		m_tipCaddieForm->Close(true);
	}

	if (m_quizForm) {
		m_quizForm->Close(true);
	}

	if (g_resrcmng && m_missionOverlay) { g_resrcmng->Release(m_missionOverlay); m_missionOverlay = 0; }

	m_missionForm = 0;
	if (result == 1) {
		this << MsgObject(0, 0x14C, 0, 0, 0, 0, 0);
	} else if (result == 2) {
		this << MsgObject(0, 0x14B, 0, 0, 0, 0, 0);
	}

	return true;
}

// tutorial.cpp:1971
void CTutorial::OpenQuizForm(int quiz) {
	if (m_quizForm == 0) {
		if (quiz == 1) {
			m_quizForm = CreateForm<FrQuizDlg>(g_pFresh->GetManager(), this, "tuto_quiz_ground", 0);
		} else {
			m_quizForm = CreateForm<FrQuizDlg>(g_pFresh->GetManager(), this, "tuto_quiz_score", 0);
		}

		m_quizForm->SetupQuiz(quiz);
		m_dialogType = 1;
		m_dialogState = 2;
		m_quizForm->SetKeyEvent(true);
		m_quizForm->SetFixed(true);
		m_quizForm->EnableDrag(false);
		m_quizForm->Open((FRESH_RESULT_CALLBACK)&CTutorial::OnQuizFormResult,
			WPoint((g_view->GetWidth() - m_quizForm->GetRect().w) * 0.5f,
				(g_view->GetHeight() - m_quizForm->GetRect().h) * 0.5f + 10.0f), 3);
		m_quizForm->MoveCursor("cases");
		m_step = 5;
		m_quizCorrect = false;
	}
}

// tutorial.cpp:2009
bool CTutorial::OnQuizFormResult(int result, FrForm *form) {
	if (m_tipCaddieForm) {
		m_tipCaddieForm->Close(true);
	}

	if (m_quizForm) {
		m_quizCorrect = m_quizForm->IsCorrectAnswer();
	}

	m_quizForm = 0;
	m_step = 6;
	if (result == 2) {
		m_missionForm->Close(FrCANCEL, true);
	}

	return true;
}

// tutorial.cpp:2038
void CTutorial::OpenMissionFailedForm(void) {
	if (m_yesNoForm == 0) {
		m_yesNoForm = CreateForm<FrForm>(g_pFresh->GetManager(), this, "notify_yesno", 0);
		m_yesNoForm->SetMessage("\xB4\xD9\xBD\xC3 \xB5\xB5\xC0\xFC\xC7\xCF\xBD\xC3\xB0\xDA\xBD\xC0\xB4\xCF\xB1\xEE?\n\n(\xBE\xC6\xB4\xCF\xBF\xC0\xB8\xA6 \xB4\xA9\xB8\xA3\xB8\xE9 \xB9\xCC\xBC\xC7\xC0\xBB \xC6\xF7\xB1\xE2\xC7\xCF\xB0\xED\n\xB9\xDB\xC0\xB8\xB7\xCE \xB3\xAA\xB0\xA9\xB4\xCF\xB4\xD9)", false);
		m_yesNoForm->EnableDrag(false);
		m_yesNoForm->Open((FRESH_RESULT_CALLBACK)&CTutorial::OnMissionFailedFormResult,
			WPoint((g_view->GetWidth() - m_yesNoForm->GetRect().w) * 0.5f, 180.0f), 2);
		m_yesNoForm->MoveCursor("ok");
	}
}

// tutorial.cpp:2050
bool CTutorial::OnMissionFailedFormResult(int result, FrForm *form) {
	m_yesNoForm = 0;
	WSingleton<CMouseCursor>::Instance()->Store();

	if (m_tipCaddieForm) {
		m_tipCaddieForm->Close(true);
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (result == 1) {
			m_step = result;

			if (GOLFDOC()->m_gameState == 7 || GOLFDOC()->m_gameState == 6) {
				m_step = 4;
				return true;
			}

			PrepareMission();
			OpenMissionForm();
			SendMsg(this, "Screen", 0x1BC, 2, 0, 0, 0);
		} else if (result == 2) {
			m_macroHold = false;

			CProjectG::Instance()->FromGolfToLobbySettings();

			CTask *pTask = AfxGetPreservedTask();
			if (pTask != 0) {
				IActor *pActor = pTask->GetActor("TutorialMain");
				if (pActor != 0) {
					pActor << MsgObject(0, 0x150, m_tutorialIndex, m_tutorialInfo, 0, 0, 0);
				}
			}

			CTaskManager::Instance()->ChangeTask("CTutorialTask", "", false);
			CTaskManager::Instance()->PostMsg(0, "TutorialMain", 0, (intptr_t)"TUTORIAL_MAIN", 0, 0, 0);

			FreeInput();
			return true;
		} else {
			return true;
		}
	}

	SendMsg(this, "Screen", 0x1BA, 1, 0, 0, 0);
	FreeInput();

	return true;
}

// tutorial.cpp:3409
void CTutorial::PushMacroHitPointMission(void) {
	PushMacro((eMacroType)22, MakeStr("Screen %d %d", 465, 1), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Screen %d %d", 520, 1), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d %d %d", 357, 2, 0, -20), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 2), 0, 1);
	PushMacro((eMacroType)8, 0, 20000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 1), 300, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d %d %d", 357, 2, -20, 0), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 0), 1000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 2), 0, 1);
	PushMacro((eMacroType)8, 0, 20000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 1), 300, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d %d %d", 357, 2, 0, 20), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 0), 1000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 2), 0, 1);
	PushMacro((eMacroType)8, 0, 20000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 1), 300, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d %d %d", 357, 2, 20, 0), 0, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 0), 1000, 1);
	PushMacro((eMacroType)22, MakeStr("Spin %d %d", 361, 2), 0, 1);
	PushMacro((eMacroType)8, 0, 20000, 1);
	PushMacro(MACRO_WAIT, 0, 2000, 1);
	PushMacro((eMacroType)14, "2 ok", 0, 1);
	PushMacro((eMacroType)9, "1308", -1, 1);
	PushMacro((eMacroType)15, 0, -1, 1);
	PushMacro((eMacroType)26, 0, -1, 1);
}

// The view matrix of the running camera (no symbol; za and pivot are read).
extern "C" WMatrix data_b7bf88;
// The smallest view-axis distance CheckCameraTarget measures (1e-5, no symbol).
extern "C" const float data_9f67e4;

// tutorial.cpp:4249
bool CTutorial::CheckCameraTarget(void) {
	WVector diff = GOLFDOC()->m_target.m_pos - data_b7bf88.pivot;
	diff.y += 6.0f;
	float dist = data_b7bf88.za.x * diff.x + data_b7bf88.za.y * diff.y + data_b7bf88.za.z * diff.z;

	if (WAbsBySign(dist) < data_9f67e4) {
		return false;
	}

	if (dist > 96.0f && dist < 288.0f && Abs(acos(Between(-1.0f, dist / diff.Magnitude(), 1.0f))) < 0.05236f) {
		m_tipState++;
		PopMacro();
	}

	return false;
}

// tutorial.cpp:4272
bool CTutorial::CheckClubTarget(void) {
	WVector reach = GOLFDOC()->m_cameraData.m_matrix.za * (GolfClub().GetRange(0) * 3.2f);
	WVector diff = GOLFDOC()->m_target.m_pos - (GolfBall().m_pos + reach);
	diff.y = 0.0f;

	if (diff.Magnitude() < 16.0f) {
		return true;
	}
	return false;
}

// tutorial.cpp:4285
WVector *CTutorial::GetPoint(const char *name) {
	return &GOLFDOC()->m_pPolySoup->FindPoint(name)->m_pos;
}

// tutorial.cpp:4294
void CTutorial::PushMacro(eMacroType type, const char *text, int param, int count) {
	sMacro macro;
	macro.m_type = type;
	macro.m_param = param;
	if (text == 0) {
		macro.m_text[0] = 0;
	} else {
		strcpy(macro.m_text, text);
	}

	for (int i = 0; i < count; i++) {
		m_macros.push_back(macro);
	}
}

// tutorial.cpp:4314
void CTutorial::PopSpecial(void) {
	if (!m_macros.empty()) {
		if (m_macros.front().m_type == MACRO_SPECIAL) {
			m_step = 7;
			m_specialWait = m_macros.front().m_param;

			m_macros.pop_front();

			if (!m_macros.empty()) {
				if (m_specialWait == 0) {
					PopMacro();
				}
			}
		}
	}
}

// tutorial.cpp:4865
void CTutorial::ClearMacro(void) {
	while (!m_macros.empty()) {
		m_macros.pop_front();
	}
}
