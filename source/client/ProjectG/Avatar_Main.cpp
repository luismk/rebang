// Original: d:\build\custom\temp\code\client\projectg\avatar_main.cpp
#define AVATAR_MAIN_SPECIALPRIZEITEM_INLINE
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include "avatar_main.h"
#include "avatar_avatar.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "TaskManager.h"
#include "SoundManager.h"
#include "JukeBoxDlg.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "quadtree.h"
#include "ClientSetting.h"
#include "fresh.h"
#include "Shadow.h"
#include "ItemManager.h"
#include "wresrcmng.h"
#include "mousecursor.h"
#include "scenemanager.h"
#include "ChatMsg.h"
#include "InputManager.h"
#include "frwndmanager.h"
#include "UccManager.h"
#include "woverlay.h"
#include "fx.h"
#include "wfont.h"
#include "wview.h"
#include "fxsequence.h"
#include "s5/specialprize/specialprizeitem.h"
#include "UccLibrary.h"
#include "bitmap.h"
#include "balloontext.h"

extern bool g_SALoadingState;
extern WMatrix g_camera;

// avatar_main.cpp:103 (run-time type and factory registration of ntAvatarChatMain)
const WRTTI ntAvatarChatMain::m_RTTI("ntAvatarChatMain", &CTaskMain::m_RTTI);

IObject *ntAvatarChatMainMakeInstance(void) {
	return new ntAvatarChatMain;
}

struct __impntAvatarChatMain {
	__impntAvatarChatMain(void) {
		ObjectFactory().AddObjectFunctor(ntAvatarChatMainMakeInstance, "ntAvatarChatMain");
		ActorFactory().AddActorClass("ntAvatarChatMain");
	}
};
static __impntAvatarChatMain s_impntAvatarChatMain;

// avatar_main.cpp:106
ntAvatarChatMain::ntAvatarChatMain(void) {
	m_priority = 4;

	m_pPVS = 0;
	m_pPVS = &::GetPVS();

	m_preservedMap = 0;
	m_preservedHole = 0;

	m_decalTime = 0.0f;
	m_cameraOffset = WVector::ZERO;
	m_cameraAngle = WVector(-0.48869219f, 0.0f, 0.0f);
	m_cameraDistance = 28.8f;
	m_cameraDistanceGoal = 28.8f;
	m_cameraClipDistance = 28.8f;
	m_cameraHeight = 4.6f;
	m_cameraHeightGoal = 4.6f;
	m_idleTime = 0.0f;
	m_idleMark = 0.0f;
	m_cameraRayHit = 1.0f;
	m_topViewMode = false;
	m_topViewHeight = 256.0f;
	m_showTargetDecal = false;

	m_glowBallTexture = g_resrcmng->LoadTexture("~GlowBall.jpg", 0, 0, 0);
	m_initFlag = false;

	new ntBalloonTextManager;
	ntBalloonTextManager::Instance()->Init();

	m_pTitleFont = g_resrcmng->GetTitleFont();

	tagWTITLEFONT info;
	memset(&info, 0, sizeof(info));

	info.fonth = 16;
	info.fontw = 16;

	char *fontName = "[font_wind.jpg";
	info.filename = &fontName;
	info.numPages = 1;
	info.texw = 128;
	info.texh = 64;
	info.pCharSet = "1234567890ym%-.?/:, ";
	m_pTitleFont->Create(&info);

	InitUI();

	m_spItemChanged = false;
	m_targetUid = 0xFFFFFFFF;
}

// avatar_main.cpp:176
ntAvatarChatMain::~ntAvatarChatMain(void) {
	g_audio->StopSfx("AmbientSnd");

	ntBalloonTextManager::Instance()->Shutdown();
	delete ntBalloonTextManager::Instance();

	if (g_resrcmng && m_glowBallTexture) { g_resrcmng->Release(m_glowBallTexture); m_glowBallTexture = 0; }

	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = 0; }

	for (int i = 0; i < 10; i++) {
		ClearTexCache(m_portraitBitmaps[i]);
		delete m_portraitBitmaps[i];
		m_portraitBitmaps[i] = 0;
	}
}

// avatar_main.cpp:198
void ntAvatarChatMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();
	new CShadowManager(g_resrcmng, COption::Instance()->vIsShadowEnabled());

	GOLFDOC()->MakePlayer(1);
}

// avatar_main.cpp:508
void ntAvatarChatMain::DoReLoad(void) {
	SendMsg(this, "Ambient", 500, 0, 0, 0, 0);

	OnLoad();

	CTaskManager *pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->Init(0);
}

// avatar_main.cpp:517
void ntAvatarChatMain::OnInit(void) {
	CTaskMain::OnInit();

	SetBGMNone();

	CalcUIOffset(false);

	if (CMouseCursor::Instance()) {
		CMouseCursor::Instance()->SetActive(true);
		CMouseCursor::Instance()->m_mode = 2;
	}

	CSceneManager::Instance()->UpdateFog(true);
	CSceneManager::Instance()->RegisterElement((eObjType)4, 0, this, "\xbe\xc6\xb9\xd9\xc5\xb8\xb8\xde\xc0\xce", 0, 0);

	if (CChatMsg::Instance()) {
		CChatMsg::Instance()->Close(true, true);
		CChatMsg::Instance()->EnableCaretMove(true);
	}

	if (!CChatMsg::Instance()->IsOpened()) {
		g_pFresh->GetManager()->SetExclusiveKey(false);
	}

	FrJukeBoxDlg::Init();

	FrJukeBoxDlg::PlayWithMap(GetCurMap());

	g_input->Reset();
}

// avatar_main.cpp:550
void ntAvatarChatMain::OnDestroy(void) {
	CTaskMain::OnDestroy();

	if (CShadowManager::Instance()) {
		delete CShadowManager::Instance();
	}

	for (int gender = 0; gender < 6; gender++) {
		if (g_resrcmng && m_genderIcons[gender]) { g_resrcmng->Release(m_genderIcons[gender]); m_genderIcons[gender] = 0; }
	}
	for (int manner = 0; manner < 2; manner++) {
		if (g_resrcmng && m_mannerIcons[manner]) { g_resrcmng->Release(m_mannerIcons[manner]); m_mannerIcons[manner] = 0; }
	}
	for (int angel = 0; angel < 2; angel++) {
		if (g_resrcmng && m_angelIcons[angel]) { g_resrcmng->Release(m_angelIcons[angel]); m_angelIcons[angel] = 0; }
	}

	if (g_resrcmng && m_diveIcon) { g_resrcmng->Release(m_diveIcon); m_diveIcon = 0; }
	if (g_resrcmng && m_stateIcon01) { g_resrcmng->Release(m_stateIcon01); m_stateIcon01 = 0; }
	if (g_resrcmng && m_stateIcon02) { g_resrcmng->Release(m_stateIcon02); m_stateIcon02 = 0; }
	if (g_resrcmng && m_stateIcon03) { g_resrcmng->Release(m_stateIcon03); m_stateIcon03 = 0; }
	if (g_resrcmng && m_stateIcon04) { g_resrcmng->Release(m_stateIcon04); m_stateIcon04 = 0; }
	if (g_resrcmng && m_stateIcon05) { g_resrcmng->Release(m_stateIcon05); m_stateIcon05 = 0; }
	if (g_resrcmng && m_stateIcon06) { g_resrcmng->Release(m_stateIcon06); m_stateIcon06 = 0; }
	if (g_resrcmng && m_stateIcon01N) { g_resrcmng->Release(m_stateIcon01N); m_stateIcon01N = 0; }
	if (g_resrcmng && m_stateIcon02N) { g_resrcmng->Release(m_stateIcon02N); m_stateIcon02N = 0; }
	if (g_resrcmng && m_stateIcon03N) { g_resrcmng->Release(m_stateIcon03N); m_stateIcon03N = 0; }
	if (g_resrcmng && m_stateIcon04N) { g_resrcmng->Release(m_stateIcon04N); m_stateIcon04N = 0; }
	if (g_resrcmng && m_emotiObject) { g_resrcmng->Release(m_emotiObject); m_emotiObject = 0; }

	for (int bar = 0; bar < 9; bar++) {
		if (g_resrcmng && m_stateBars[bar]) { g_resrcmng->Release(m_stateBars[bar]); m_stateBars[bar] = 0; }
	}

	if (g_resrcmng && m_shadowTexture) { g_resrcmng->Release(m_shadowTexture); m_shadowTexture = 0; }

	// The guids of the avatars still in the lounge, each sent message 0xE5.
	int guids[30];
	int count = 0;
	for (int slot = 0; slot < 30 && count < ntAvatarManager::Instance()->m_avatarCount; slot++) {
		if (ntAvatarManager::Instance()->m_avatars[slot]) {
			guids[count++] = ntAvatarManager::Instance()->m_avatars[slot]->m_guid;
		}
	}

	for (int msgIndex = 0; msgIndex < count; msgIndex++) {
		this << MsgObject(this, 0xE5, guids[msgIndex], 0, 0, 0, 0);
	}

	g_pFresh->GetManager()->SetExclusiveKey(true);
}

// avatar_main.cpp:604
void ntAvatarChatMain::OnPreserveBack(const char *nextTask) {
	if (strcmp(nextTask, "CRealMyRoomTask") == 0) {
		ntAvatarManager::Instance()->m_pMyAvatar->SetState(ntAvatarManager::Instance()->m_pMyAvatar->m_state | 2);
	} else if (strcmp(nextTask, "CShopTask") == 0) {
		ntAvatarManager::Instance()->m_pMyAvatar->SetState(ntAvatarManager::Instance()->m_pMyAvatar->m_state | 4);
	}

	ntAvatarManager::Instance()->m_pMyAvatar->Stop();

	g_audio->StopSfx("AmbientSnd");

	FrJukeBoxDlg::Preserve();

	m_preservedMap = GetCurMap();
	m_preservedHole = GOLFDOC()->m_currentHole;

	SetPVSIndex(1);

	ResetFrWndPointers();

	CTaskMain::OnPreserveBack(nextTask);
}

// avatar_main.cpp:733
void ntAvatarChatMain::CheckUIPosition(void) {
	static float s_barY = COption::Instance()->gGetBar_Y();

	if (COption::Instance()->gGetBar_Y() != s_barY) {
		CalcUIOffset(false);
		s_barY = COption::Instance()->gGetBar_Y();

		g_pFresh->OpenLayout("AVATARCHAT_MAIN", this, false);

		if (ntAvatarManager::Instance()->m_pMyAvatar->m_state & 0x80) {
			EnableUnderBar(false);
		}
	}
}

// avatar_main.cpp:771
void ntAvatarChatMain::ToggleTopViewMode(void) {
	m_topViewMode = !m_topViewMode;
	if (m_topViewMode) {
		GOLFDOC()->GameMode() = 0x200;

		Waabb box;
		box = m_pPVS->m_pBaseModel->m_aabb;

		if (ntAvatarManager::Instance()->m_pMyAvatar) {
			m_topViewCenter = ntAvatarManager::Instance()->m_pMyAvatar->m_position;
		} else {
			m_topViewCenter.x = (box.min.x + box.max.x) * 0.5f;
			m_topViewCenter.y = box.min.y;
			m_topViewCenter.z = (box.min.z + box.max.z) * 0.5f;
		}
		m_topViewHeight = 256.0f;

		*((bool *)CSceneManager::Instance() + 0xB8) = false; // TODO: CSceneManager::m_useFog (0xB8), private in scenemanager.h; becomes CSceneManager::Instance()->m_useFog once it is public
	} else {
		GOLFDOC()->GameMode() = 0;
		*((bool *)CSceneManager::Instance() + 0xB8) = true; // TODO: CSceneManager::m_useFog (0xB8), private in scenemanager.h; becomes CSceneManager::Instance()->m_useFog once it is public
	}
}

// avatar_main.cpp:1379
void ntAvatarChatMain::ProcessCamera(float elapsed) {
	WMatrix camera;

	if (m_spItemChanged && g_SALoadingState) {
		m_cameraDistanceGoal = ntAvatarManager::Instance()->m_pMyAvatar->GetMySpItem((eSPAVILITYTYPE)0).getvalue() * 28.800001f;
		m_spItemChanged = false;
	}

	if (m_topViewMode) {
		g_view->SetClip(8.0f, 8192.0f, false);
		g_view->SetFOV(1.5707964f);
		g_view->UpdateCamera();

		camera = RotMat(WVector(-1.5707964f, 0.0f, 0.0f));
		camera.pivot = m_pPVS->GetGroundPoint(m_topViewCenter, false, 0, false);
		camera.pivot.y += m_topViewHeight;
	} else {
		g_view->SetClip(1.0f, 4096.0f, false);
		g_view->SetFOV(1.0471976f);
		g_view->UpdateCamera();

		// Ease the distance and the height toward their goals.
		if (!(WAbsBySign(m_cameraDistance - m_cameraDistanceGoal) < 1e-5f)) {
			float step = elapsed * 46.0f;
			if (m_cameraDistanceGoal > m_cameraDistance) {
				m_cameraDistance += step;
				if (m_cameraDistance > m_cameraDistanceGoal) {
					m_cameraDistance = m_cameraDistanceGoal;
				}
			} else {
				m_cameraDistance -= step;
				if (m_cameraDistance < m_cameraDistanceGoal) {
					m_cameraDistance = m_cameraDistanceGoal;
				}
			}
		}

		camera = RotMat(WVector(m_cameraAngle.x, m_cameraAngle.y, m_cameraAngle.z));

		if (!(WAbsBySign(m_cameraHeight - m_cameraHeightGoal) < 1e-5f)) {
			float step = elapsed * 5.0f;
			if (m_cameraHeightGoal > m_cameraHeight) {
				m_cameraHeight += step;
				if (m_cameraHeight > m_cameraHeightGoal) {
					m_cameraHeight = m_cameraHeightGoal;
				}
			} else {
				m_cameraHeight -= step;
				if (m_cameraHeight < m_cameraHeightGoal) {
					m_cameraHeight = m_cameraHeightGoal;
				}
			}
		}

		WVector pos = ntAvatarManager::Instance()->m_pMyAvatar->m_position;
		pos.y += m_cameraHeight;

		// Pull the camera in when the ground or an object is between it and
		// the avatar, and let it back out slowly.
		WVector dir = camera.za * m_cameraDistance;
		dir.Negate();
		float hit = m_pPVS->RayIntersectAll(pos, dir, false);
		if (hit > m_cameraRayHit && hit * m_cameraDistance > m_cameraClipDistance) {
			m_cameraClipDistance += elapsed * 16.0f;
			if (m_cameraClipDistance >= hit * m_cameraDistance) {
				m_cameraClipDistance = hit * m_cameraDistance;
				m_cameraRayHit = hit;
			}
		} else {
			m_cameraClipDistance = hit * m_cameraDistance;
			m_cameraRayHit = hit;
		}

		dir.Normalize();
		WVector eye = pos + dir * m_cameraClipDistance;

		WVector ground = m_pPVS->GetGroundPoint(eye, false, 0, false);
		float minY = ground.y + 3.5f;
		if (minY > eye.y) {
			eye.y = minY;
		}

		camera.pivot = eye;
	}

	g_camera = camera;
}

// avatar_main.cpp:1495
void ntAvatarChatMain::PrintText(float x, float y, const char *text, float spacing, int align, float scale, unsigned long color) {
	if (text == 0) {
		return;
	}

	int len = strlen(text);
	if (len == 0) {
		return;
	}

	char *charSet = m_pTitleFont->GetFontInfo()->pCharSet;

	float oldScale = m_pTitleFont->GetScale();
	m_pTitleFont->SetScale(scale);

	float width = 0.0f;
	const char *p = text;
	switch (align) {
	case 1: {
		while (*p) {
			int c = *p++;

			char *found = strchr(charSet, c);

			width += m_pTitleFont->GetCharWidthA(g_view, found - charSet) + spacing * scale;
		}
		width -= spacing;
		width *= 0.5f;
		break;
	}
	case 2: {
		while (*p) {
			int c = *p++;

			char *found = strchr(charSet, c);

			width += m_pTitleFont->GetCharWidthA(g_view, found - charSet) + spacing * scale;
		}
		width -= spacing;
		break;
	}
	}

	x -= width;

	p = text;
	while (*p) {
		int c = *p++;

		char *found = strchr(charSet, c);

		x += m_pTitleFont->PutChar(g_view, x, y, found - charSet, 0, color) + spacing;
	}

	m_pTitleFont->SetScale(oldScale);
	m_pTitleFont->Flush(g_view);
}

// avatar_main.cpp:1572
void ntAvatarChatMain::Display(void) {
	if (ntAvatarManager::Instance()->m_pMyAvatar == 0) {
		return;
	}

	ntAvatarManager::Instance()->Render();

	if (m_showTargetDecal && m_uiCycle != 2) {
		// The decal under the followed avatar, or under the own avatar's target.
		WVector normal, tangent;
		normal = WVector(4.8f, 0.0f, 0.0f);
		tangent = WVector(0.0f, 0.0f, 4.8f);

		if (ntAvatarManager::Instance()->m_pMyAvatar->m_followName[0] != 0) {
			ntAvatar *pTarget = ntAvatarManager::Instance()->FindAvatarByNickName(ntAvatarManager::Instance()->m_pMyAvatar->m_followName);
			if (pTarget == 0) {
				return;
			}
			m_pPVS->DrawDecal(m_glowBallTexture, pTarget->m_position, normal, tangent, (cosf(m_decalTime * 10.0f) + 1.0f) * 0.05f + 1.0f, 1.0f, W_POLY_BLEND_ADD, false);
		} else {
			m_pPVS->DrawDecal(m_glowBallTexture, ntAvatarManager::Instance()->m_pMyAvatar->m_targetPosition, normal, tangent, (cosf(m_decalTime * 10.0f) + 1.0f) * 0.05f + 1.0f, 1.0f, W_POLY_BLEND_ADD, false);
		}
	}
}

// avatar_main.cpp:2583
void ntAvatarChatMain::RefreshAllUccClothes(void) {
	sUccRefreshItem item;

	for (int i = 0; i < 10; i++) {
		if (!UccManager()->GetRefreshItem(item)) {
			break;
		}

		int count = 0;
		for (int j = 0; j < 30; j++) {
			if (count >= ntAvatarManager::Instance()->m_avatarCount) {
				break;
			}
			ntAvatar *pAvatar = ntAvatarManager::Instance()->m_avatars[j];

			if (pAvatar == 0) {
				continue;
			}

			count++;
			unsigned char slot = (unsigned char)((item.m_typeId >> 13) & 0x1F);
			if (pAvatar->m_partTidList.m_partTids[slot] != item.m_typeId) {
				continue;
			}
			unsigned long serial = pAvatar->m_partTidList.m_partIds[slot];

			sUccClothes *pClothes = UccManager()->FindClothes(serial);

			if (pClothes != 0 && strcmpi(pClothes->m_uccKey, item.m_uccKey) == 0) {
				pAvatar->RefreshUccClothes(pClothes->m_typeId, pClothes->m_serial);
			}
		}
	}
}

// avatar_main.cpp:2625
CQuadTree &ntAvatarChatMain::GetPVS(void) {
	return *m_pPVS;
}

// avatar_main.cpp:2631
void ntAvatarChatMain::SetLocalPVS(CQuadTree &pvs) {
	m_pPVS = &pvs;
}

// avatar_main.cpp:2637
void ntAvatarChatMain::StartSpEffect(ntAvatar *pAvatar, eSPAVILITYTYPE type) {
	CFxSequence *pSeq;
	bool bOn = pAvatar->GetMySpItem(type).getflag();

	switch (type) {
	case 0:
		if (bOn) {
			pSeq = CFx::Instance()->OpenSequence("change_up-big.seq", true);
		} else if (pAvatar->GetMySpItem((eSPAVILITYTYPE)1).getflag()) {
			pSeq = CFx::Instance()->OpenSequence("change_up-middle.seq", true);
		} else {
			pSeq = CFx::Instance()->OpenSequence("change_up-small.seq", true);
		}
		break;

	case 1:
		if (bOn) {
			if (pAvatar->GetMySpItem((eSPAVILITYTYPE)0).getflag()) {
				pSeq = CFx::Instance()->OpenSequence("change_up-big.seq", true);
			} else {
				pSeq = CFx::Instance()->OpenSequence("change_up-middle.seq", true);
			}
		} else {
			if (pAvatar->GetMySpItem((eSPAVILITYTYPE)0).getflag()) {
				pSeq = CFx::Instance()->OpenSequence("change_up-big.seq", true);
			} else {
				pSeq = CFx::Instance()->OpenSequence("change_up-small.seq", true);
			}
		}
		break;

	case 2:
		if (pAvatar->GetMySpItem((eSPAVILITYTYPE)0).getflag()) {
			pSeq = CFx::Instance()->OpenSequence("change_up-big.seq", true);
		} else if (pAvatar->GetMySpItem((eSPAVILITYTYPE)1).getflag()) {
			pSeq = CFx::Instance()->OpenSequence("change_up-middle.seq", true);
		} else {
			pSeq = CFx::Instance()->OpenSequence("change_up-small.seq", true);
		}
		break;

	case 3: {
		float height = pAvatar->GetMySpItem((eSPAVILITYTYPE)0).getvalue() * 10.0f;
		pSeq = CFx::Instance()->OpenSequence("BFX_B2.seq", true);
		if (pSeq) {
			pSeq->Pos() = WVector(pAvatar->m_position.x + 1.0f, pAvatar->m_position.y + height, pAvatar->m_position.z + 1.0f);
		}
		pSeq = CFx::Instance()->OpenSequence("BFX_B3.seq", true);
		if (pSeq) {
			pSeq->Pos() = WVector(pAvatar->m_position.x + 1.0f, pAvatar->m_position.y + height, pAvatar->m_position.z + 1.0f);
		}
		g_audio->PlaySfx("Ball_B", 0, 0, 0, 0, 0.5f, 200.0f);
		return;
	}

	default:
		return;
	}

	if (pSeq) {
		pSeq->Pos() = WVector(pAvatar->m_position.x, pAvatar->m_position.y + 5.0f, pAvatar->m_position.z);
	}
}

// avatar_main.cpp:2771
bool ntAvatarChatMain::SpCommand(const char *command) {
	bool handled = false;
	static unsigned long s_lastTime = timeGetTime() - 1000;
	int len = strlen(command + 1);

	if (len == 0 || command[0] != '/') {
		return false;
	}

	// The chat commands (giant, big head, light speed, sparkle), indexed by
	// eSPAVILITYTYPE.
	static const char s_commands[5][64] = {
		"\xb0\xc5\xc0\xce",
		"\xbf\xd5\xb8\xd3\xb8\xae",
		"\xb1\xa4\xbc\xd3",
		"\xb9\xdd\xc2\xa6\xc0\xcc",
		""
	};

	for (int i = 0; i < 5; i++) {
		if (strcmp(command + 1, s_commands[i]) == 0) {
			handled = true;

			if (i == 3) {
				if (timeGetTime() - s_lastTime >= 1000) {
					s_lastTime = timeGetTime();
				} else {
					CChatMsg::Instance()->AddChatMsg(std::string("\xc1\xf6\xb1\xdd\xc0\xba \xbb\xe7\xbf\xeb\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9"), 0xFFFF7878, true, false);
					return true;
				}
			}

			if (CanUseSPItem((eSPAVILITYTYPE)i)) {
				ntAvatarManager::Instance()->m_pMyAvatar->UseSPItem((eSPAVILITYTYPE)i);
			} else {
				CChatMsg::Instance()->AddChatMsg(std::string("\xc7\xd8\xb4\xe7 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xc2\xf8\xbf\xeb\xc7\xcf\xc1\xf6 \xbe\xca\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9"), 0xFFFF7878, true, false);
			}
			break;
		}
	}

	return handled;
}

// avatar_main.cpp:2818
bool ntAvatarChatMain::CanUseSPItem(eSPAVILITYTYPE type) {
	const sCharacterInfo &info = ntAvatarManager::Instance()->m_pMyAvatar->GetCharacterInfo();
	for (int i = 0; i < 24; i++) {
		if (info.partTypeIds[i] != 0) {
			IFF_STRUCT::sSpecialPrizeItem *pItem = ItemManager()->FindSPItem(info.partTypeIds[i], type);
			if (pItem != 0) {
				return true;
			}
		}
	}

	for (int j = 0; j < 5; j++) {
		if (info.auxParts.tids[j] != 0) {
			IFF_STRUCT::sSpecialPrizeItem *pItem = ItemManager()->FindSPItem(info.auxParts.tids[j], type);
			if (pItem != 0) {
				return true;
			}
		}
	}

	return false;
}
