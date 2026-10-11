// Original: d:\build\custom\temp\code\client\projectg\realmyroom_avatar.cpp
#include "NetworkSystem.h"
#include "RealMyRoom_Avatar.h"

#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "Packet.h"
#include "partsinfo.h"
#include "texturepart.h"
#include "wbone.h"
#include "wpuppet.h"
#include <math.h>
#include "../shared/Localize.h"
#include "wview.h"
#include "SharedDoc.h"
#include "RealMyRoomMain.h"
#include "quadtree.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "wutil.h"
#include "commonutil.h"
#include "ClientSetting.h"

const char *GetStrConst(const char *pSrc, char *pDest);
float RayIntersectAABB(const Waabb &aabb, const WVector &pos, const WVector &moveDelta, WPlane *pReturnPlane, float radius); // wmathutil.inl:46

// Nameless globals of the default light (0xAA5274 is probably a LightSet:
// nearone 0xAA5278, diffuse 0xAA5284).
extern WVector data_00aa5278;
extern unsigned long data_00aa5284;

// The face texture of each character type (4 facials per character).
extern "C" const char *data_a88020[][4];

// realmyroom_avatar.h:93
const WVector &CRealMyRoom_Avatar::GetPosition() const {
	return m_position;
}

// realmyroom_avatar.h:125
void CRealMyRoom_Avatar::SetHeadIcon(int headIcon) {
	m_headIcon = headIcon;
}

// realmyroom_avatar.cpp:170
CRealMyRoom_Avatar::~CRealMyRoom_Avatar() {
	std::list<sTexturePart *>::iterator it;
	for (it = m_pendingTexParts.begin(); it != m_pendingTexParts.end(); ++it) {
		delete *it;
	}
	m_pendingTexParts.clear();

	m_fxBox.ClearAll(false);
}

// realmyroom_avatar.cpp:194
bool CRealMyRoom_Avatar::IsTargetPositionChanged(bool changed) {
	bool wasChanged = m_isTargetPositionChanged;
	m_isTargetPositionChanged = changed;
	return wasChanged;
}

// realmyroom_avatar.cpp:200
void CRealMyRoom_Avatar::SetHide(bool hide) {
	m_hide = hide;

	if (m_pMain->m_pRoomFx != 0) {
		CFx::Instance()->SetActive(hide);
	}
}

// realmyroom_avatar.cpp:208
bool CRealMyRoom_Avatar::IsClickable(float x, float y) const {
	if (!g_pFresh->IsVisible()) {
		return false;
	}

	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		return false;
	}

	if (m_time < 0.1f) {
		return false;
	}

	FrGraphicInterface *pGDI = g_pFresh->GetManager()->GetGDI();
	if (m_state & 0x80) {
		WVector head;
		head = m_pPetFrame->GetBonePet()->FindBone("Bip01 HeadNub")->GetMatrix().pivot;

		WVector screen = g_view->Projection(head);

		if (!(screen.z <= 0.0f) && !(1.0f <= screen.z)) {
			int screenX = (int)floorf(screen.x);
			int screenY = (int)floorf(screen.y) - 48;

			if (m_state & 0x80) {
				if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar != NULL) {
					if ((WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar->m_position - m_position).Magnitude() > 19.2f) {
						return false;
					}
				}

				int width = (int)pGDI->GetTextExtend(m_tradeTitle) + 10;
				if (width < 100) {
					width = 100;
				}

				int left = screenX - width / 2;

				int top = screenY - 8;
				if (left < x && x < left + width && top < y && y < top + 29) {
					return true;
				}
			}
		}
	}

	return false;
}

// realmyroom_avatar.cpp:281
CRealMyRoom_Avatar *CRealMyRoom_Avatar::GetFollowingAvatar() const {
	if (m_followNickName[0] == '\0') {
		return NULL;
	}

	return WSingleton<CRealMyRoom_AvatarManager>::Instance()->FindAvatarByNickName(m_followNickName);
}

// realmyroom_avatar.cpp:292
bool CRealMyRoom_Avatar::GetBoneScreenCoord(const char *boneName, int *pX, int *pY) const {
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		return false;
	}

	WBone *pBone = m_pPetFrame->GetBonePet()->FindBone(boneName);
	if (pBone == NULL) {
		return false;
	}

	WVector screen = g_view->Projection(pBone->GetMatrix().pivot);
	*pX = (int)floor(screen.x);
	*pY = (int)floor(screen.y);

	return true;
}

// realmyroom_avatar.cpp:318
void CRealMyRoom_Avatar::ChangeCharacterInfo() {
	m_fxBox.ClearAll(false);

	ParseBoundingBoxScript();
}

// realmyroom_avatar.cpp:337
void CRealMyRoom_Avatar::SetCharactorInfo(const sCharacterInfo &charInfo) {
	Stop();

	m_pPetFrame = NULL; m_charInfo = charInfo;
	m_charTypeId = charInfo.typeId;

	m_fxBox.ClearAll(false);
}

// realmyroom_avatar.cpp:351
void CRealMyRoom_Avatar::SetPosition(const WVector &position) {
	m_position = position;
	m_targetPosition = position; m_isTargetPositionChanged = true;
}

// realmyroom_avatar.cpp:359
void CRealMyRoom_Avatar::SetStartPosition(const WVector &position) {
	m_startPosition = position;
}

// realmyroom_avatar.cpp:364
void CRealMyRoom_Avatar::SetTargetPosition(const WVector &position) {
	m_targetPosition = position;
	m_isTargetPositionChanged = true;
}

// realmyroom_avatar.cpp:370
void CRealMyRoom_Avatar::SetAngle(float angle) {
	angle = RadianNormalize2PI(angle);
	m_angle = angle;
	m_targetAngle = angle;
}

// realmyroom_avatar.cpp:377
void CRealMyRoom_Avatar::SetTargetAngle(float angle) {
	m_targetAngle = RadianNormalize2PI(angle);
}

// realmyroom_avatar.cpp:383
void CRealMyRoom_Avatar::SetPendingTargetAngle(float angle) {
	m_pendingTargetAngle = RadianNormalize2PI(angle);
	m_hasPendingTargetAngle = true;
}

// realmyroom_avatar.cpp:389
void CRealMyRoom_Avatar::SetState(unsigned long state) {
	if (m_state == state) {
		return;
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
		WSendPacket packet((enumClientPacket)0x63);
		packet.Encode1(8);
		packet.Encode4(state);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_state = state; if (state & 0x7E) {
		Stop();
	}
}

// realmyroom_avatar.cpp:412
void CRealMyRoom_Avatar::SetAction(int action) {
	if (m_action == action) {
		return;
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
		WSendPacket packet((enumClientPacket)0x63);
		packet.Encode1(5);
		packet.Encode4(action);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_action = action;
}

// realmyroom_avatar.cpp:428
void CRealMyRoom_Avatar::SendMove(const WVector &delta, bool force) {
	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
		m_moveDelta += delta;

		if (m_moveDelta.Magnitude() >= 16.5f || force) {
			m_moveDelta.Reset();
		}
	}
}

// realmyroom_avatar.cpp:451
void CRealMyRoom_Avatar::Stop() {
	if (this == WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
		if (!WisEqual(m_targetPosition, m_position, 0.00001f)) {
			SendMove(WVector(0.0f, 0.0f, 0.0f), true);

			m_targetPosition = m_position;
			m_isTargetPositionChanged = true;
		}

		m_moveFlag = false;
		m_followNickName[0] = '\0';
	}
}

// realmyroom_avatar.cpp:468
void CRealMyRoom_Avatar::Follow(const char *nickName) {
	if (nickName) {
		strcpy(m_followNickName, nickName);
	}
	else {
		m_followNickName[0] = '\0';
	}
}

// realmyroom_avatar.cpp:479
void CRealMyRoom_Avatar::ChangeMotion(int motion, const char *name, float blendTime) {
	const char *motionName;

	m_motion = motion;
	if (m_pPetFrame == NULL) {
		return;
	}

	switch (motion) {
	case 0:
		motionName = "\xb1\xe2\xba\xbb\xc0\xda\xbc\xbc";
		break;
	case 2:
		motionName = "chat_\xbe\xc9\xb1\xe2";
		break;
	case 3:
		motionName = "chat_\xb4\xaf\xb1\xe2";
		break;
	case 1:
		motionName = "chat_\xb4\xde\xb8\xae\xb1\xe2";
		break;
	case 4:
		motionName = name;
		if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
			&& this == WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar) {
			unsigned long now = timeGetTime();

			if (now - m_lastMotionSendTime <= 1000) {
				return;
			}

			m_lastMotionSendTime = now;
			WSendPacket packet((enumClientPacket)0x63);
			packet.Encode1(7);
			packet.EncodeStr(name);
			packet.Send(SEND_TO_DEFAULT);
		}
		break;
	default:
		motionName = "\xb1\xe2\xba\xbb\xc0\xda\xbc\xbc";
		break;
	}

	w_motion_data *pData = m_pPetFrame->FindMotionData(motionName, true, NULL);
	if (pData == NULL) {
		return;
	}

	if (blendTime > 0.0f && m_pMotionData != NULL) {
		m_boneSet1.CopyFrom(&m_boneSet0);
		m_pPrevMotionData = m_pMotionData;
		m_prevMotion = m_motion;
		m_pMotionData = pData;
		m_motion = motion;
		m_blendTime = 0.0f;
		m_blendDuration = blendTime;
	} else {
		m_pPrevMotionData = NULL; m_blendTime = 0.0f; m_pMotionData = pData; m_boneSet0.CopyFrom(m_pPetFrame->GetCurAniPet()->GetBones(NULL, pData->s, 0, NULL, false));
	}

	if (m_blendTime == 0.0f) {
		// the props of the character are shown again by the custom motion
		bool hideProps = motion != 4;
		WBone *pBone;

		switch (m_charTypeId & 0x3FFFFFF) {
		case 4:
			pBone = m_pPetFrame->GetBonePet()->FindBone("D_Ball");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			pBone = m_pPetFrame->GetBonePet()->FindBone("D_Racket");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			break;
		case 5:
			pBone = m_pPetFrame->GetBonePet()->FindBone("E_Cannon");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			pBone = m_pPetFrame->GetBonePet()->FindBone("E_Telescope");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			break;
		case 6:
			pBone = m_pPetFrame->GetBonePet()->FindBone("G_Bird");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			break;
		case 7:
			pBone = m_pPetFrame->GetBonePet()->FindBone("H_Sword01");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			pBone = m_pPetFrame->GetBonePet()->FindBone("H_Sword02");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			pBone = m_pPetFrame->GetBonePet()->FindBone("H_karen");
			if (pBone) {
				pBone->SetInvisible(false, hideProps);
			}
			break;
		}

		if ((m_charTypeId & 0x3FFFFFF) < 10) {
			SetPendingTexPart("face", data_a88020[m_charTypeId & 0x3FFFFFF][3]);
		}
	}

	ProcessMotion(0.0f);
}

// realmyroom_avatar.cpp:626
void CRealMyRoom_Avatar::ProcessMotion(float elapsed) {
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		return;
	}

	if (m_motion != 0 && m_motion != 2 && m_motion != 3 && m_motion != 1) {
		if (m_pMotionData->e - m_pMotionData->s < m_blendTime) {
			ChangeMotion(0, NULL, 0.2f);
		}
	}

	if (m_pPrevMotionData != NULL) {
		float t = m_blendTime / m_blendDuration;

		if (t > 1.0f) {
			m_pPrevMotionData = NULL;
		}
		else {
			float time = m_blendTime;
			float length = m_pMotionData->e - m_pMotionData->s;
			if (length < m_blendTime) {
				time = length;
			}

			m_boneSet0.Blend(&m_boneSet1, m_pPetFrame->GetCurAniPet()->GetBones(m_pMotionData->name, time, 0, NULL, false), t);
		}
	}
	else {
		float length = m_pMotionData->e - m_pMotionData->s;

		if (m_blendTime > length) {
			if (length > 0.01f) {
				float loops = floorf(m_blendTime / length);
				m_blendTime -= loops * length;
			}
			else {
				m_blendTime = 0.0f;
			}
		}

		m_boneSet0.CopyFrom(m_pPetFrame->GetCurAniPet()->GetBones(NULL, m_pMotionData->s + m_blendTime, 0, NULL, false));
	}

	if (m_blendTime == 0.0f) {
		ScriptProcess(m_pMotionData, m_pMotionData->s, m_pMotionData->s, true);
	}
	else {
		ScriptProcess(m_pMotionData, m_pMotionData->s + m_blendTime, m_pMotionData->s + m_blendTime + elapsed, false);
	}
}

// realmyroom_avatar.cpp:895
void CRealMyRoom_Avatar::ProcessChangeState() {
	m_prevState = m_state;
}

// realmyroom_avatar.cpp:1260
void CRealMyRoom_Avatar::Render() {
	if (!m_hide) {
		return;
	}

	if (IsLocalContent((localContentType_t)10)) {
		if (this != WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pMyAvatar && m_isAdministrator) {
			CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
			int guid = m_guid;
			std::map<unsigned long, sBriefUserInfo>::iterator it = pDoc->m_userMap.find(guid);

			if (!sBriefUserInfo((*it).second).invisible) {
				if (guid != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid) {
					return;
				}
			}
		}
	}

	WVector pos;
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		pos = GetPVS().GetGroundPoint(m_position, false, NULL, false);
	}
	else {
		pos = m_pPetFrame->GetBonePet()->FindBone("Bip01 Pelvis")->GetMatrix().pivot;
		pos = GetPVS().GetGroundPoint(pos, false, NULL, false);
	}

	if (COption::Instance()->vIsShadowEnabled()) {
		GetPVS().DrawDecal(m_pMain->m_shadowTexture, pos, WVector(2.9f, 0.0f, 0.0f), WVector(0.0f, 0.0f, 2.9f), 1.0f, m_time, (enumPolyBlend)0, false);
	}

	if (m_pPetFrame != NULL && m_pPetFrame->GetCurAniPet() != NULL) {
		if (g_view->InFrustum(m_pPetFrame->GetBonePet()->GetBoundBox())) {
			if (WSingleton<CRealMyRoom_AvatarManager>::Instance()->m_pLitAvatar == this) {
				m_light.nearone = data_00aa5278;
				m_light.diffuse = m_light.ambient = 0xFFFFFFFF;
			}
			else {
				m_light.nearone = data_00aa5278;
				m_light.ambient = 0xFFD2D2D2;
				m_light.diffuse = ModifyColor(data_00aa5284, GetPVS().GetShadowIntensity(pos));
			}

			m_pPetFrame->UpdateLightSource(&m_light, false, NULL);

			m_pPetFrame->Render(g_view, true, 1.0f, false, false, true);
		}
	}

	g_view->Flush(1);

	m_fxBox.Display(g_view);
}

// realmyroom_avatar.cpp:1598
void CRealMyRoom_Avatar::ScriptProcess(const w_motion_data *data, float from, float to, bool all) {
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

	if (m_pPetFrame) {
		WPuppet *pPuppet = m_pPetFrame->GetCurAniPet();

		if (pPuppet) {
			int first, last;
			if (pPuppet->CheckFrameData(start, end, first, last)) {
				for (int i = first; i <= last; i++) {
					ParsePuppetScript(pPuppet->GetFrameData(i));
				}
			}
		}
	}
}

// realmyroom_avatar.cpp:1930
void CRealMyRoom_Avatar::ParseScript_ShowVis(bool show, const char *script) {
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
				WBone *pBone = m_pPetFrame->GetBonePet()->FindBone(it->c_str());

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

// realmyroom_avatar.cpp:2145
sTexturePart *CRealMyRoom_Avatar::FindTexturePart(const char *part, const char *texture) {
	sTexturePart *pTexPart;
	std::list<sTexturePart *>::iterator it;

	it = std::find_if(m_pendingTexParts.begin(), m_pendingTexParts.end(), sFindTexPart(texture));

	if (it != m_pendingTexParts.end()) {
		pTexPart = *it;
	}
	else {
		pTexPart = new sTexturePart;

		strcpy(pTexPart->m_partName, part);
		strcpy(pTexPart->m_texName, texture);
		m_pendingTexParts.push_back(pTexPart);
	}

	return pTexPart;
}

// realmyroom_avatar.cpp:2168
void CRealMyRoom_Avatar::SetPendingTexPart(const char *part, const char *texture) {
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		return;
	}

	sTexturePart *pTexPart = FindTexturePart(part, texture);

	if (pTexPart->m_pBitmap == NULL) {
		m_pPetFrame->GetBonePet()->ChangeTexturePart(m_pPetFrame->GetFaceTex(), pTexPart->m_texName, &pTexPart->m_pBitmap, &pTexPart->m_rect);
	}
	else {
		m_pPetFrame->GetBonePet()->ChangeTexturePart(m_pPetFrame->GetFaceTex(), pTexPart->m_pBitmap, pTexPart->m_rect);
	}

	m_pPetFrame->SetMakeup();
}

// realmyroom_avatar.cpp:2193
void CRealMyRoom_Avatar::SetChatFacial(int facial) {
	SetPendingTexPart("face", data_a88020[m_charTypeId & 0x3FFFFFF][facial]);

	m_facialTime = 5.0f;
}

// realmyroom_avatar.cpp:2202
void CRealMyRoom_Avatar::SetBalloonText(const char *text) {
	m_balloonTime = m_time;
	m_balloonElapsed = 0;
}

// realmyroom_avatar.cpp:2213
void CRealMyRoom_Avatar::SetTradeTitle(const char *title) {
	if (title) {
		strcpy(m_tradeTitle, title);
	}
	else {
		m_tradeTitle[0] = '\0';
	}
}

// realmyroom_avatar.cpp:2221
void CRealMyRoom_Avatar::RefreshUccClothes(unsigned long tid, unsigned long id) {
	if (m_pPetFrame == NULL) {
		return;
	}

	if (IsLocalContent((localContentType_t)0x5F)) {
		m_pPetFrame->SetPart(&m_partTidList, tid, id);
		m_pPetFrame->SetPart(&m_partTidList, tid, id);
	}
}

// realmyroom_avatar.cpp:2241
CRealMyRoom_AvatarManager::CRealMyRoom_AvatarManager() {
	m_avatarCount = 0; for (int i = 0; i < 30; i++) m_pAvatars[i] = NULL;

	m_pMyAvatar = NULL;
	m_pLitAvatar = NULL;
	m_unknownAC = 0;
	m_isRenderEnabled = true;
	m_overlayDetail = true;
}

// realmyroom_avatar.cpp:2257
CRealMyRoom_AvatarManager::~CRealMyRoom_AvatarManager() {
	FreeAllAvatars();
}

// realmyroom_avatar.cpp:2262
void CRealMyRoom_AvatarManager::FreeAllAvatars() {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i]) {
			if (m_pAvatars[i]->m_pPetFrame) {
				delete m_pAvatars[i]->m_pPetFrame;
				m_pAvatars[i]->m_pPetFrame = NULL;
			}

			if (m_pAvatars[i]) {
				delete m_pAvatars[i];
				m_pAvatars[i] = NULL;
			}
		}
	}
}

// realmyroom_avatar.cpp:2284
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::FindAvatarByIDName(const char *idName) const {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			if (strcmp(m_pAvatars[i]->m_idName, idName) == 0) {
				return m_pAvatars[i];
			}
		}
	}

	return NULL;
}

// realmyroom_avatar.cpp:2300
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::FindAvatarByNickName(const char *nickName) const {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			if (strcmp(m_pAvatars[i]->m_nickName, nickName) == 0) {
				return m_pAvatars[i];
			}
		}
	}

	return NULL;
}

// realmyroom_avatar.cpp:2316
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::FindAvatarByGuid(int guid) const {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			if (m_pAvatars[i]->m_guid == guid) {
				return m_pAvatars[i];
			}
		}
	}

	return NULL;
}

// realmyroom_avatar.cpp:2332
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::CreateAvatar(const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, CRealMyRoomMain *pMain) {
	int slot;

	for (slot = 0; slot < 30; slot++) {
		if (m_pAvatars[slot] == 0) {
			break;
		}
	}

	if (slot == 30) {
		// every slot is taken
		return 0;
	}

	m_avatarCount++;

	CRealMyRoom_Avatar *pAvatar = new CRealMyRoom_Avatar(slotInfo, charInfo, pMain);
	WVector position(slotInfo.m_x, 0.0f, slotInfo.m_z);
	position = GetPVS().GetGroundPoint(position, false, 0, false);
	pAvatar->SetPosition(position);
	pAvatar->SetAngle(slotInfo.m_angle);

	m_pAvatars[slot] = pAvatar;
	return pAvatar;
}

// realmyroom_avatar.cpp:2366
bool CRealMyRoom_AvatarManager::DeleteAvatar(const char *idName) {
	int i;
	for (i = 0; i < 30; i++) {
		if (m_pAvatars[i]) {
			if (strcmp(m_pAvatars[i]->m_idName, idName) == 0) {
				break;
			}
		}
	}

	if (i == 30) {
		return false;
	}

	if (m_pAvatars[i]) {
		delete m_pAvatars[i];
		m_pAvatars[i] = NULL;
	}
	m_avatarCount--;

	return true;
}

// realmyroom_avatar.cpp:2391
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::AvatarByRayIntersect(const WVector &pos, const WVector &dir) const {
	Waabb aabb;
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i]) {
			if (IsLocalContent((localContentType_t)10)) {
				CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
				if (m_pAvatars[i]->m_guid != pDoc->GetPangyaUserInfo().guid) {
					std::map<unsigned long, sBriefUserInfo>::iterator it;
					it = pDoc->m_userMap.find(m_pAvatars[i]->m_guid);

					if (!(pDoc->GetPangyaUserInfo().identity & 4)) {
						if (sBriefUserInfo((*it).second).identityBits.administrator) {
							if (!sBriefUserInfo((*it).second).invisible)
								break;
						}
					}
				}
			}

			aabb.min = WVector(-1.0f, 0.0f, -1.0f);
			aabb.min += m_pAvatars[i]->m_position;
			aabb.max = WVector(1.0f, 8.0f, 1.0f);
			aabb.max += m_pAvatars[i]->m_position;

			if (RayIntersectAABB(aabb, pos, dir, NULL, 0.0f) < 1.0f) {
				return m_pAvatars[i];
			}
		}
	}

	return NULL;
}

// realmyroom_avatar.cpp:2430
CRealMyRoom_Avatar *CRealMyRoom_AvatarManager::CheckClick(float x, float y) const {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] && m_pAvatars[i]->IsClickable(x, y)) {
			return m_pAvatars[i];
		}
	}

	return NULL;
}

// realmyroom_avatar.cpp:2443
void CRealMyRoom_AvatarManager::Process(float elapsed) {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			m_pAvatars[i]->Process(elapsed);
		}
	}
}

// realmyroom_avatar.cpp:2454
void CRealMyRoom_AvatarManager::Render() {
	if (!m_isRenderEnabled) {
		return;
	}

	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			m_pAvatars[i]->Render();
		}
	}
}

// realmyroom_avatar.cpp:2468
void CRealMyRoom_AvatarManager::ResetAvatarPositionAngle() {
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i] != NULL) {
			m_pAvatars[i]->m_position = m_pAvatars[i]->m_targetPosition;
			m_pAvatars[i]->m_angle = m_pAvatars[i]->m_targetAngle;
		}
	}
}

// realmyroom_avatar.cpp:2482
// qsort order of RenderOverlay: the avatar farther from the camera first.
static int __cdecl CompareCameraDistance(const void *a, const void *b) {
	WVector eye = g_view->GetCamera().pivot;

	const WVector &posA = (*(CRealMyRoom_Avatar *const *)a)->m_position;
	WVector deltaA(eye.x - posA.x, eye.y - posA.y, eye.z - posA.z);
	const WVector &posB = (*(CRealMyRoom_Avatar *const *)b)->m_position;
	WVector deltaB(eye.x - posB.x, eye.y - posB.y, eye.z - posB.z);

	float distA = deltaA.z * deltaA.z + deltaA.y * deltaA.y + deltaA.x * deltaA.x;
	float distB = deltaB.z * deltaB.z + deltaB.y * deltaB.y + deltaB.x * deltaB.x;

	if (distA > distB) {
		return -1;
	}
	if (distA < distB) {
		return 1;
	}
	return 0;
}

// realmyroom_avatar.cpp:2503
void CRealMyRoom_AvatarManager::RenderOverlay() {
	if (!m_isRenderEnabled) {
		return;
	}

	CRealMyRoom_Avatar *avatars[30];
	int count = 0;
	for (int i = 0; i < 30; i++) {
		if (m_pAvatars[i]) {
			avatars[count++] = m_pAvatars[i];
		}
	}

	qsort(avatars, count, sizeof(CRealMyRoom_Avatar *), CompareCameraDistance);

	for (int j = 0; j < count; j++) {
		if (avatars[j]) {
			avatars[j]->RenderOverlay(m_overlayDetail);
		}
	}
}

// realmyroom_avatar.cpp:2528
CRealMyRoom_AvatarBurden::CRealMyRoom_AvatarBurden() : m_charInfo(), m_pPetFrame(NULL) {
}

// realmyroom_avatar.cpp:2533
CRealMyRoom_AvatarBurden::CRealMyRoom_AvatarBurden(unsigned long guid, const sCharacterInfo &charInfo, CPetFrame *pPetFrame) : m_charInfo() {
	m_guid = guid;

	m_charInfo = charInfo; m_pPetFrame = pPetFrame;
}

// realmyroom_avatar.cpp:2540
CRealMyRoom_AvatarBurden::~CRealMyRoom_AvatarBurden() {
}

// realmyroom_avatar.cpp:2545
bool CRealMyRoom_AvatarBurden::LoadAvatarBurden() {
	if (!m_partTidList.SetTids(&m_charInfo, 0xFF)) {
		m_partTidList.SetDefaultTids();
	}
	if (!m_partTidList.IsComboValid()) {
		m_partTidList.SetDefaultTids();
	}

	if (m_pPetFrame) { delete m_pPetFrame; m_pPetFrame = NULL; }

	m_pPetFrame = new CPetFrame;

	if (!m_pPetFrame->Build(&m_partTidList, m_charInfo.auxParts.tids, true, false, m_guid)) {
		delete m_pPetFrame;
		m_pPetFrame = NULL;
		return false;
	}

	m_pPetFrame->SetHairClr(&m_partTidList, m_charInfo.hairColor);
	m_pPetFrame->SetShirtsClr(&m_partTidList, m_charInfo.shirtColor & 0xF);

	return true;
}
