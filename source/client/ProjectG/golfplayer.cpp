// Original: d:\build\custom\temp\code\client\projectg\golfplayer.cpp
#define GOLFPLAYER_GOLFDOC_INLINE
#define GOLFPLAYER_PARTSINFO_INLINE
#define GOLFPLAYER_UCCMANAGER_INLINE
#include "golfplayer.h"
#include "partsinfo.h"
#include "ObjectFactory.h"
#include "NetworkSystem.h"
#include "GolfDoc.h"
#include "club.h"
#include "UccManager.h"
#include "TailEffect.h"
#include "scenemanager.h"
#include "texturepart.h"
#include "golfball.h"
#include "SharedDoc.h"
#include "headicon.h"
#include "InputManager.h"
#include "commonutil.h"
#include "quadtree.h"
#include "wpuppet.h"
#include "wbone.h"
#include "wutil.h"
#include "fx.h"
#include "parttidlist.h"
#include "SoundManager.h"
#include "Packet.h"
#include "QuitWindow.h"
#include "ItemWindow.h"
#include "../shared/Localize.h"

#include <algorithm>

const char *GetStrConst(const char *pSrc, char *pDest);

// shareddoc.inl:503, inline in the original; GolfRule.cpp holds the
// out-of-line copy.
inline unsigned char MyIndex(void) {
	CSharedDoc *doc = WSingleton<CSharedDoc>::Instance();
	if (doc->m_userInfo.IsIdentity(2)) {
		switch (doc->m_roomType) {
		case 4:
		case 5:
		case 6:
		case 9:
		case 10:
			return doc->GetIndex(doc->m_userInfo.galleryGuid);
		default:
			return doc->m_pGolfDoc->m_myPlayerIndex;
		}
	}

	return doc->GetIndex(doc->m_userInfo.guid);
}

// ?g_lightset@@3ULightSet@@A; its header (wscene.h) does not declare it yet.
extern LightSet g_lightset;
// ?convMat@@3VWMatrix@@A
extern WMatrix convMat;
// Motion names; CheckShotScript times entries 6 to 21. No symbol.
extern const char *data_00a804a8[];

// golfplayer.h
CGolfPlayer::Object::Object(void) {
}

// golfplayer.h
CGolfPlayer::sClub::sClub(void) {
}

// partsinfo.h:109
unsigned long CPetFrame::GetTidChar(void) {
	return m_tidChar;
}

// golfplayer.h:139
CGolfPlayer::sPetInfo::sPetInfo(void)
	: m_motionTime(0), m_pMotion(0), m_unk38(false), m_unk5C(0), m_unk60(0),
	  m_pTexPart(0), m_unkA8(0), m_scale(1.0f), m_unkB0(0) {
	for (int i = 2; i >= 0; --i) { m_unk08[i][0] = 1.0f; m_unk08[i][1] = m_unk08[i][2] = m_unk08[i][3] = 0.0f; }
	m_unk39 = false;
	memset(m_shotTimes, 0, sizeof(m_shotTimes));
}

// golfplayer.h:146
CGolfPlayer::sPetInfo::~sPetInfo(void) {
}

// golfplayer.h:233
int CGolfPlayer::GetAbilityPartsSize(void) {
	return m_abilityParts.size();
}

// club.h:196
bool CClub::TypeUpdated(void) {
	return (m_flags & 2) ? true : false;
}

// golfplayer.cpp:37 (run-time type and factory registration of CGolfPlayer)
const WRTTI CGolfPlayer::m_RTTI("CGolfPlayer", &IActor::m_RTTI);

IObject* CGolfPlayerMakeInstance(void) {
	return new CGolfPlayer;
}

struct __impCGolfPlayer {
	__impCGolfPlayer(void) {
		ObjectFactory().AddObjectFunctor(CGolfPlayerMakeInstance, "CGolfPlayer");
		ActorFactory().AddActorClass("CGolfPlayer");
	}
};
static __impCGolfPlayer s_impCGolfPlayer;

// golfplayer.cpp:96
CGolfPlayer::CGolfPlayer(void)
	: m_pClubPuppet(NULL), m_pPetFrames(NULL), m_pPetInfos(NULL), m_pTailEffect(NULL), m_unk15D(true),
	  m_unk2F0(false) {
	m_comboRunning = false; m_comboTime = 0;

	m_unk320 = 0;
	m_powerShotKeyTime = 0;
	m_powerShotOn = false;
	m_unk329 = false;
	m_powerShotKeyState = 0;

	m_pHeadBone = NULL;
	m_unkF8 = 0;


	m_unk330 = false;
}

// golfplayer.cpp:113
void CGolfPlayer::OnDestroy(void) {
	GolfClub().ClearClubSet();

	std::list<sTexturePart *>::iterator it;
	for (it = m_pendingTexParts.begin(); it != m_pendingTexParts.end(); ++it) {
		if (*it) { delete *it; *it = NULL; }
	}
	m_pendingTexParts.clear();

	if (m_pTailEffect) { delete m_pTailEffect; m_pTailEffect = NULL; }

	if (m_pPetFrames) { delete[] m_pPetFrames; m_pPetFrames = NULL; }
	if (m_pPetInfos) { delete[] m_pPetInfos; m_pPetInfos = NULL; }
}

// golfplayer.cpp:227
void CGolfPlayer::OnPreLoadInit(void) {
	m_lightSet.type = LightSet::W_LIGHT_DIRECTIONAL;

	CSceneManager::Instance()->RegisterElement((eObjType)4, 0, 0, "\xc5\xac\xb7\xb4", 0x28, 0);

	CSceneManager::Instance()->RegisterElement((eObjType)6, 0, m_pTailEffect, "tail", 0, 0);
	CSceneManager::Instance()->SetVisible("tail", false);

	m_pPetInfos = new sPetInfo[4];

	for (int i = 0; i < 4; i++) {
		CSceneManager::Instance()->RegisterElement((eObjType)4, 0, 0, MakeStr("PLAYER_%d", i), 0x18, 0);
		CSceneManager::Instance()->SetVisible(MakeStr("PLAYER_%d", i), false);
	}

	m_pTailEffect = new CTailEffect("~swinglight01.jpg");
}

// golfplayer.cpp:269
void CGolfPlayer::OnLoad(void) {
	if (WSingleton<CHeadIcon>::Instance()) {
		WSingleton<CHeadIcon>::Instance()->SetLevel();
	}

	if (m_pPetFrames) { delete[] m_pPetFrames; m_pPetFrames = NULL; }
	m_pPetFrames = new CPetFrame[GOLFDOC()->GetPlayerNum()];

	for (int i = 0; i < 4; i++) {
		if (i < GOLFDOC()->GetPlayerNum()) {
			if (GOLFDOC()->GetPlayer(i)->m_state != 3) {
				CPartTidList *tidList = &GOLFDOC()->GetPartTidList(i);

				ShowAbilityParts(tidList);

				if (!m_pPetFrames[i].Build(tidList, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_characterInfo.auxParts.tids, true, true, i)) {
					tidList->SetDefaultTids();
					m_pPetFrames[i].Build(tidList, WSingleton<CSharedDoc>::Instance()->m_roomPlayers[i].m_characterInfo.auxParts.tids, true, false, i);
				}

				m_pPetFrames[i].CalcBVolume();
				SetMotion("������", 0.0f, i);

				m_pPetInfos[i].m_unk60 = 0;
				m_pPetInfos[i].m_unk39 = false;

				CSceneManager::Instance()->SetPuppet(MakeStr("PLAYER_%d", i), m_pPetFrames[i].GetBonePet());

				m_extendedEffect.RegistExtenedEffect(&m_pPetFrames[i]);
			}
		} else {
			m_pPetInfos[i].m_unk38 = false;
			CSceneManager::Instance()->SetPuppet(MakeStr("PLAYER_%d", i), 0);
		}
	}

	if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1) {
		for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			if (i == MyIndex()
				|| (GOLFDOC()->GetPlayer(MyIndex())->m_partnerIndex != 0xFF && i == GOLFDOC()->GetPlayer(MyIndex())->m_partnerIndex)
				|| !WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
				MakeStr("PLAYER_%d", i);
			}
		}
	}

	RegisterClub(-1);
	CheckShotScript();
}

// golfplayer.cpp:340
void CGolfPlayer::OnReload(void) {
	OnLoad();
}

// golfplayer.cpp:413
void CGolfPlayer::CheckShotScript(void) {
	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (GOLFDOC()->GetPlayer(i)->m_state == 3) {
			continue;
		}
		for (int j = 0; j < 16; j++) {
			w_motion_data *pMotion = m_pPetFrames[i].FindMotionData(data_00a804a8[j + 6], true, NULL);
			if (pMotion == NULL) {
				continue;
			}
			int start = (int)ceil(pMotion->s * 30.0f);
			float endFrame = pMotion->e * 30.0f;
			int end = WAbsBySign(floor(endFrame) - endFrame) < WMATH_EPSILON ? -1 - (int)(pMotion->e * -30.0f) : (int)floor(pMotion->e * 30.0f);
			for (int frame = start; frame < end; frame++) {
				int index;
				if (m_pPetFrames[i].GetCurAniPet()->CheckFrameData(frame, frame, index, index)) {
					char *data = m_pPetFrames[i].GetCurAniPet()->GetFrameData(index);
					if (strstr(data, "*shot")) {
						m_pPetInfos[i].m_shotTimes[j] = (frame - start) * (1.0f / 30.0f);
						break;
					}
				}
			}
		}
	}
}

// golfplayer.cpp:467
void CGolfPlayer::OnInit(void) {
	m_pFootBones[0] = NULL;
	m_pFootBones[1] = NULL;
	m_unk16F = false;
	m_swingTypeName[0] = '\0';
	m_unk15C = false;

	m_unk140 = 3.14159265f;
	m_displayMatrix.Reset();
	m_lightSet.ambient = ModifyColor(g_lightset.ambient, 1.0f);
	m_lightSet.nearone = g_lightset.nearone;
}

// golfplayer.cpp:484
void CGolfPlayer::InputProcess(float dt) {
	if (g_input->Get(0x45)) {
		if (m_headAngle < 1.0471976f) {
			m_headAngle += dt * 12.566371f;
			m_pPetInfos[GOLFDOC()->m_myPlayerIndex].m_unk5C |= 0x200;
		}
	}

	if (g_input->Get(0x51)) {
		if (m_headAngle > -1.0471976f) {
			m_headAngle -= dt * 12.566371f;
			m_pPetInfos[GOLFDOC()->m_myPlayerIndex].m_unk5C |= 0x200;
		}
	}
}

// golfplayer.cpp:1016
WMatrix CGolfPlayer::GetHeadMatrix(void) {
	WMatrix mat;
	WBone *pBone = m_pHeadBone;

	if (pBone == NULL) {
		mat.Reset();
		return mat;
	}

	if (WAbsBySign(m_headAngle) < 0.00001f) {
		mat = convMat * pBone->GetMatrix();
	} else {
		mat = RotMat(m_headAngle, 1);
		mat = mat * convMat * pBone->GetMatrix();
	}

	return mat;
}

// golfplayer.cpp:1040
WMatrix CGolfPlayer::GetFootMatrix(int foot) {
	WMatrix mat;
	WBone *pBone = m_pFootBones[foot];

	if (pBone == NULL) {
		mat.Reset();
		return mat;
	}

	if (WAbsBySign(m_footAngle) < 0.00001f) {
		mat = convMat * pBone->GetMatrix();
	} else {
		mat = RotMat(m_footAngle, 0);
		mat = mat * convMat * pBone->GetMatrix();
	}

	return mat;
}

// golfplayer.cpp:2726
void CGolfPlayer::SetMotion(const char *name, float time, int player) {
	if (player < 0) {
		player = GOLFDOC()->m_myPlayerIndex;
	}

	if (IsLocalContent((localContentType_t)0x6D)) {
		int count = m_pPetFrames[player].GetMotionCountIncludeName(name, data_00a804a8[22]);
		if (count > 1) {
			char motionName[0x80] = "";
			sprintf(motionName, "%s%02d", name, rand() % count + 1);

			m_pPetInfos[player].m_pMotion = m_pPetFrames[player].FindMotionData(motionName, true, NULL);

			if (m_pPetInfos[player].m_pMotion == NULL) {
				m_pPetInfos[player].m_pMotion = m_pPetFrames[player].FindMotionData(name, true, NULL);
			}
		} else {
			m_pPetInfos[player].m_pMotion = m_pPetFrames[player].FindMotionData(name, true, NULL);
		}
	} else {
		m_pPetInfos[player].m_pMotion = m_pPetFrames[player].FindMotionData(name, true, NULL);
	}

	if (m_pPetInfos[player].m_pMotion == NULL) {
		return;
	}

	m_pPetInfos[player].m_scale = 1.0f;
	m_pPetInfos[player].m_unkB0 = 0;

	if (time < 0.0f) {
		float t = time + m_pPetInfos[player].m_pMotion->e;
		m_pPetInfos[player].m_motionTime = m_pPetInfos[player].m_pMotion->s < t ? t : m_pPetInfos[player].m_pMotion->s;
	} else {
		float t = time + m_pPetInfos[player].m_pMotion->s;
		m_pPetInfos[player].m_motionTime = m_pPetInfos[player].m_pMotion->e < t ? m_pPetInfos[player].m_pMotion->e : t;
	}

	m_pPetInfos[player].m_unk5C |= 0x33;
	m_unk2F0 = ((m_pPetInfos[player].m_unk5C >> 8) & 1) != 0;
	m_pPetInfos[player].m_unk5C &= ~0x180;

	m_pPetFrames[player].SetInvisible(false);

	ScriptProcess(m_pPetInfos[player].m_pMotion, m_pPetInfos[player].m_pMotion->s, m_pPetInfos[player].m_pMotion->s, player, true, time < 0.0f);

	if ((m_pPetFrames[player].GetTidChar() & 0x3FFFFFF) == 6 && !(WAbsBySign(time) < 0.00001f) && strcmpi(name, "������") == 0) {
		WBone *pBone = m_pPetFrames[player].GetBonePet()->FindBone("G_Bird");
		if (pBone != NULL) {
			pBone->SetInvisible(false, true);
		}
	}

	m_animFx[player].~vector();
}

// golfplayer.cpp:2808
void CGolfPlayer::SetSwingType(void) {
	switch (GolfClub().GetType()) {
	case 0:
		strcpy(m_swingTypeName, "\xBF\xEC\xB5\xE5\xBC\xA6");
		break;
	case 1:
		strcpy(m_swingTypeName, "\xBE\xC6\xC0\xCC\xBE\xF0\xBC\xA6");
		break;
	case 2:
		strcpy(m_swingTypeName, "\xBB\xF7\xB5\xE5\x9F\x87\xC1\xF6\xBC\xA6");
		break;
	case 3:
		strcpy(m_swingTypeName, "\xC6\xDB\xC6\xC3\xBC\xA6");
		break;
	}

	char *motion;
	if (GolfClub().PowerShotType()) {
		if (GolfClub().PowerShotType() == 1) {
			motion = MakeStr("%s\xC6\xC4\xBF\xF6\xC1\xD8\xBA\xF1\xB3\xA1", m_swingTypeName);
		} else {
			motion = MakeStr("%s\xC6\xC4\xBF\xF6" "2\xC1\xD8\xBA\xF1\xB3\xA1", m_swingTypeName);
		}
	} else {
		motion = MakeStr("%s\xC1\xD8\xBA\xF1", m_swingTypeName);
	}

	SetMotion(motion, 0.0f, -1);

	m_ballDist = m_clubs[GOLFDOC()->m_myPlayerIndex][GolfClub().GetType()].m_ballPos.x;
	m_pPetInfos[GOLFDOC()->m_myPlayerIndex].m_unk60 = 0;
}

// golfplayer.cpp:2845
void CGolfPlayer::ScriptProcess(w_motion_data *data, float from, float to, int player, bool all, bool skip) {
	if (data == NULL) {
		return;
	}

	if ((!(from <= data->e) || from == to) && !all) {
		return;
	}

	int start, end;
	from *= 30.0f;
	if (all) {
		start = Ceil(from, 0.03f);
		end = start;
	} else {
		to = Min(data->e, to) * 30.0f;
		start = Ceil(from, 0.03f);
		end = WAbsBySign((float)floor(to) - to) < 1e-5f ? (int)to - 1 : (int)floor(to);

		if (start == (int)floor(data->s * 30.0f + 0.5f)) {
			start++;
		}

		if (start > end) {
			return;
		}
	}

	int first, last;
	if (m_pPetFrames[player].GetCurAniPet()->CheckFrameData(start, end, first, last)) {
		for (int i = first; i <= last; i++) {
			ParsePuppetScript(m_pPetFrames[player].GetCurAniPet()->GetFrameData(i), player, skip);
		}
	}
}

// golfplayer.cpp:2887
void CGolfPlayer::CloseBoundingBoxFx(int slot, unsigned long fxId) {
	if (fxId == 0 || slot > 4) {
		return;
	}

	std::vector<sCondBBSpray>::iterator spray;
	for (spray = m_condBBSprays[slot].begin(); spray != m_condBBSprays[slot].end(); ) {
		if (spray->m_fxId == fxId) {
			CFx::Instance()->CloseSpray(spray->m_pSpray);
			m_condBBSprays[slot].erase(spray);
		} else {
			++spray;
		}
	}

	std::vector<sCondBBSequence>::iterator sequence;
	for (sequence = m_condBBSequences[slot].begin(); sequence != m_condBBSequences[slot].end(); ) {
		if (sequence->m_fxId == fxId) {
			CFx::Instance()->CloseSequence(sequence->m_pSequence, true);
			m_condBBSequences[slot].erase(sequence);
		} else {
			++sequence;
		}
	}
}

// golfplayer.cpp:3474
void CGolfPlayer::ParseScript_ShowVis(int player, bool show, const char *script) {
	char token[0x100] = "";
	int count = 0;

	do {
		script = GetStrConst(script, token);

		if (strlen(token) == 0 || strlen(token) >= sizeof(token)) {
			break;
		}

		std::list<std::string> *pGroup = WSingleton<CSharedDoc>::Instance()->GetVisGroup(token);

		if (pGroup) {
			for (std::list<std::string>::iterator it = pGroup->begin(); it != pGroup->end(); ++it) {
				WBone *pBone = m_pPetFrames[player].GetBonePet()->FindBone(it->c_str());

				if (pBone) {
					pBone->SetInvisible(false, !show);
				}
			}
		}

		if (script == NULL || *script == ')') {
			break;
		}
	} while (++count < 64);
}

// golfplayer.cpp:3752
void CGolfPlayer::SetPendingTexPart(int player, char *texName) {
	std::list<sTexturePart *>::iterator it;

	it = std::find_if(m_pendingTexParts.begin(), m_pendingTexParts.end(), sFindTexPart(texName));

	if (it != m_pendingTexParts.end()) {
		if (*it != m_pPetInfos[player].m_pTexPart) {
			m_pPetInfos[player].m_pTexPart = *it;
		}
		return;
	}

	sTexturePart *pTexPart = new sTexturePart;

	strcpy(pTexPart->m_texName, texName);
	m_pendingTexParts.push_back(pTexPart);

	m_pPetInfos[player].m_pTexPart = pTexPart;
}

// golfplayer.cpp:3925
unsigned char CGolfPlayer::HasStandGround(const WVector &pos) {
	float angle = 0.0f; unsigned char count = 0;

	for (int i = 0; i < 8; ++i, angle += 3.14159265f / 4.0f) {
		// Original: pos - WVector(0, radius, 0), with the binary operator- of
		// wmath.inl:44 (not transcribed yet).
		WVector v(pos.x, pos.y, pos.z); v -= WVector(0.0f, GolfBall().GetRadius(), 0.0f);
		if (SearchGround(v, angle)) {
			++count;
		}
	}
	return count;
}

// golfplayer.cpp:3939
void CGolfPlayer::SetLight(void) {
	WBone *pHead = m_pPetFrames[GOLFDOC()->m_myPlayerIndex].GetBonePet()->FindBoneByTail("Head");
	m_lightSet.diffuse = ModifyColor(g_lightset.diffuse, GetPVS().GetShadowIntensity(pHead->GetMatrix().pivot));
}

// golfplayer.cpp:3945
void CGolfPlayer::SetLight(int index) {
	if (index < 0 || index >= 4) {
		return;
	}

	WBone *pHead = m_pPetFrames[index].GetBonePet()->FindBoneByTail("Head");

	if (pHead) {
		m_lightSet.diffuse = ModifyColor(g_lightset.diffuse, GetPVS().GetShadowIntensity(pHead->GetMatrix().pivot));
	}
}

// golfplayer.cpp:3958
void CGolfPlayer::ResetComboState(void) {
	m_comboRunning = false;
	m_comboTime = 0;
}

// golfplayer.cpp:3965
void CGolfPlayer::StartComboState(void) {
	m_comboRunning = true;
	m_comboTime = 0;
}

// golfplayer.cpp:3972
void CGolfPlayer::EndComboState(void) {
	m_comboCount = (int)(m_comboTime * (1.0f / 33.0f));
	m_comboRunning = false;
	m_comboTime = 0;
}

// golfplayer.cpp:4119
void CGolfPlayer::Cancel_PowerShot(void) {
	if (GolfClub().PowerShotType()) {
		g_audio->StopSfx("powershot_ready");

		g_audio->PlaySfx("powershot_cancel", 0, 0, 0, 0, 0.5f, 200.0f);

		int screenParam = 0;

		GolfClub().SetPowerShot(0);
		SendMsg(this, "Spot", 0xF5, 0, 0, 0, 0);
		SendMsg(this, "Screen", 0x1AA, (intptr_t)&screenParam, 0, 0, 0);

		SetSwingType();

		if (OnlinePlay()) {
			WSendPacket packet((enumClientPacket)0x15);
			packet.Encode1(0);
			packet.Send(SEND_TO_DEFAULT);
		}
	}
}

// golfplayer.cpp:4153
void CGolfPlayer::PowerShotInput(void) {
	if (IsMyTurn(false) && !WSingleton<CQuitWindow>::Instance()->IsActive() && CItemWindow::IsClosed()) {
		if (m_powerShotKeyState == 0) {
			if (g_input->GetUp("LALT") || g_input->GetUp("RALT")) {
				if (m_powerShotOn) {
					Cancel_PowerShot();
					m_powerShotOn = false;
				} else if (IsLocalContent((localContentType_t)0x31) && PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000027) {
					m_powerShotKeyState = 2;
					m_powerShotKeyTime = timeGetTime();
				} else {
					m_powerShotKeyState = 1;
					m_powerShotKeyTime = timeGetTime();
				}
			}
		} else if (m_powerShotKeyState == 1) {
			if (g_input->GetDown("LALT", true) || g_input->GetDown("RALT", true)) {
				if (timeGetTime() < m_powerShotKeyTime + 500) {
					m_powerShotKeyState = 2;
				} else if (m_powerShotOn) {
					Cancel_PowerShot();
					m_powerShotOn = false;
					m_powerShotKeyState = 0;
				}
			}
		} else if (m_powerShotKeyState == 2) {
			if (g_input->GetUp("LALT") || g_input->GetUp("RALT")) {
				m_comboCount = 2;
				Shot_PowerShot();
				m_powerShotOn = true;
				m_powerShotKeyState = 0;
			}
		}
	}

	if (m_powerShotKeyState == 1 && !m_powerShotOn && timeGetTime() > m_powerShotKeyTime + 500) {
		m_comboCount = 1;
		Shot_PowerShot();
		m_powerShotOn = true;
		m_powerShotKeyState = 0;
	}
}

// golfplayer.cpp:4256
bool CGolfPlayer::IsAnimFxRunning(int slot, unsigned long fxId) {
	if (fxId == 0 || slot > 4) {
		return false;
	}

	std::vector<unsigned long>::iterator it;
	for (it = m_animFx[slot].begin(); it != m_animFx[slot].end(); ++it) {
		if (*it == fxId) {
			return true;
		}
	}
	return false;
}

// golfplayer.cpp:4273
void CGolfPlayer::RefreshAllUccClothes(void) {
	sUccRefreshItem item;

	for (int i = 0; i < 10; i++) {
		if (!UccManager()->GetRefreshItem(item)) {
			return;
		}

		for (int player = 0; player < GOLFDOC()->GetPlayerNum(); player++) {
			CPartTidList &tidList = GOLFDOC()->GetPartTidList(player);

			unsigned char part = (unsigned char)((item.m_typeId >> 13) & 0x1F);
			if (tidList.m_partTids[part] == item.m_typeId) {
				unsigned long serial = tidList.m_partIds[part];

				sUccClothes *pClothes = UccManager()->FindClothes(serial);
				if (pClothes && !_strcmpi(pClothes->m_uccKey, item.m_uccKey)) {
					m_pPetFrames[player].SetPart(&tidList, pClothes->m_typeId, pClothes->m_serial);
					m_pPetFrames[player].SetPart(&tidList, pClothes->m_typeId, pClothes->m_serial);
					m_pPetFrames[player].GetBonePet()->xUpdateTnLBuffers(true);
				}
			}
		}
	}
}

// golfplayer.cpp:4308
void CGolfPlayer::AddAbilityParts(unsigned long tid, unsigned long value) {
	m_abilityParts.push_back(std::pair<unsigned long, unsigned long>(tid, value));
}
