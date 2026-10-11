// Original: d:\build\custom\temp\code\client\projectg\avatar_avatar.cpp
#define AVATAR_AVATAR_WMATH_INLINE
#define AVATAR_AVATAR_SPECIALPRIZEITEM_INLINE
#include "NetworkSystem.h"
#include <mmsystem.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "avatar_avatar.h"
#include "quadtree.h"
#include "SharedDoc.h"
#include "partsinfo.h"
#include "balloontext.h"
#include "Packet.h"
#include "wview.h"
#include "wbone.h"
#include "wpuppet.h"
#include "wutil.h"
#include "texturepart.h"
#include "../shared/Localize.h"

const char *GetStrConst(const char *pSrc, char *pDest);
float Random(float min, float max, int seed); // commonutil.cpp:1106

// Facial texture names per character, four per character type.
extern "C" const char *data_a82938[][4];
#include "wmath.h"
#include "s5/specialprize/specialprizeitem.h"
#include "avatar_main.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "woverlay.h"
#include "ClientSetting.h"
#include "commonutil.h"
#include "NetResourceManager.h"
#include "wresrcmng.h"

extern WMatrix g_camera;

extern LightSet g_lightset;

float RayIntersectAABB(const Waabb &aabb, const WVector &pos, const WVector &moveDelta, WPlane *pReturnPlane, float radius); // wmathutil.inl:46, emitted out of line here

// The original's min (a > b ? b : a, by value; wutil Min compares the
// other way); its header is unknown, as for camerawork.cpp's MinOf.
template <class T> T MinOf(T a, T b) { return a > b ? b : a; }

// A frame time this close to a whole frame counts as the previous frame.
const float kScriptFrameEpsilon = 1e-5f;

// wmath.h:98
float RadianNormalize2PI(float radian) {
	if (radian >= 6.2831855f || radian < 0.0f) {
		radian -= (float)floor((double)(radian * 0.15915494f)) * 6.2831855f;
	}
	return radian;
}

// wmath.h:106
float RadianNormalizePI(float radian) {
	radian = RadianNormalize2PI(radian);
	if (radian > 3.1415927f) {
		radian -= 6.2831855f;
	}
	return radian;
}

// wmath.h:115 (RadianNormalizePI inlined)
float RadianDelta(float a, float b) {
	float delta = RadianNormalize2PI(a - b);
	if (delta > 3.1415927f) {
		delta -= 6.2831855f;
	}
	return delta;
}

// wmath.inl:77
int operator!=(const WVector &a, const WVector &b) {
	return !WisEqual(a, b, 0.00001f);
}

// quadtree.h
trace_s::trace_s() {
}

// avatar_avatar.h:118
unsigned long ntAvatar::GetGuid() const {
	return m_guid;
}

// avatar_avatar.h:128
const WVector &ntAvatar::GetPosition() const {
	return m_position;
}

// avatar_avatar.h:151
void ntAvatar::SetHeadIcon(int icon) {
	m_headIcon = icon;
}

// avatar_avatar.cpp:86
ntAvatar::ntAvatar(CQuadTree &pvs, const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, ntAvatarChatMain *pChatMain)
	: m_boneSet(true), m_prevBoneSet(true) {
	m_pPVS = &pvs;
	strcpy(m_idName, slotInfo.m_nickname);
	strcpy(m_nickName, slotInfo.m_nickname);
	m_moveFlag = false;
	m_followName[0] = '\0';

	if (IsLocalContent((localContentType_t)0x10)) {
		if (slotInfo.m_valueCD == 1) {
			strcpy(m_slotBlockD1, slotInfo.m_blockD1);
			m_hasSlotBlockD1 = true;
		}
		else {
			memset(m_slotBlockD1, 0, sizeof(m_slotBlockD1));
			m_hasSlotBlockD1 = false;
		}
	}

	m_uid = slotInfo.m_ruleUID;
	m_guid = slotInfo.m_uid;
	m_canKick = (slotInfo.m_identity >> 2) & 1;
	m_userFlag = slotInfo.m_attributeBits.userFlag;
	m_slotFlag55 = slotInfo.m_flags55.flag0;
	m_slotFlag57 = slotInfo.m_flags57.flag0;
	m_charTypeId = slotInfo.m_characterTypeId;
	m_state = slotInfo.m_avatarState;
	m_prevState = m_state;
	m_action = slotInfo.m_avatarAction;
	m_pMotion = NULL;
	m_pPrevMotion = NULL;
	m_blendElapsed = 0.0f;
	m_blendTime = 0.0f;
	m_value6F8 = 0.0f;
	m_facialTime = 0.0f;
	m_groundNormal = WVector::UNIT_POS_Y;
	m_targetPositionChanged = false;
	m_motionSendTime = 0;
	m_hasPendingTargetAngle = false;
	m_pChatMain = pChatMain;
	m_flag3D8 = false;
	m_pPetFrame = NULL;
	m_characterInfo = charInfo;
	m_headIcon = -1;

	m_pEmblem = NULL;
	if (slotInfo.m_guildUID != 0) {
		m_pEmblem = WSingleton<NetResourceManager>::Instance()->GetEmblemByName(slotInfo.m_guildMark);
	}

	m_pBalloonText = NULL;
	m_balloonOffset[0] = Random(0.8f, 1.0f, -1);
	m_balloonOffset[1] = Random(0.8f, 1.0f, -1);
	m_balloonOffset[2] = Random(0.8f, 1.0f, -1);
	m_balloonTime = 1.0f;

	m_pendingMove.Reset();
	m_angle = m_targetAngle = RadianNormalize2PI(slotInfo.m_angle);
	SetState(m_state);
	SetAction(m_action);
	if (slotInfo.m_tradeTitle) {
		strcpy(m_tradeTitle, slotInfo.m_tradeTitle);
	}
	else {
		m_tradeTitle[0] = '\0';
	}

	m_light.type = LightSet::W_LIGHT_DIRECTIONAL;
	m_light.ambient = ModifyColor(g_lightset.ambient, 1.0f);
	m_light.nearone = g_lightset.nearone;

	m_fxBox.Init(g_resrcmng);
	m_fxBox.SetCamera(&g_camera);
	// The original sets m_fxBox's frame time (CFxBox +0x4, protected) to 33.333f here.
}

// avatar_avatar.cpp:290
ntAvatar::~ntAvatar() {
	if (m_pBalloonText) {
		WSingleton<ntBalloonTextManager>::Instance()->DeleteBalloonText(m_pBalloonText);
	}

	for (std::list<sTexturePart *>::iterator it = m_pendingTexParts.begin(); it != m_pendingTexParts.end(); ++it) {
		delete *it;
	}
	m_pendingTexParts.clear();

	m_fxBox.ClearAll(false);
}

// avatar_avatar.cpp:314
bool ntAvatar::IsTargetPositionChanged(bool changed) {
	bool wasChanged = m_targetPositionChanged;
	m_targetPositionChanged = changed;
	return wasChanged;
}

// avatar_avatar.cpp:321
bool ntAvatar::IsClickable(float x, float y) const {
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
				if ((WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->m_position - m_position).Magnitude() > 19.2f) {
					return false;
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

				// The state icon drawn above the head.
				WOverlay *pIcon = m_pChatMain->m_stateIcon01N;

				int iconWidth = pIcon->GetWidth();
				int iconHeight = pIcon->GetHeight();
				left = screenX - iconWidth / 2;
				top = screenY - iconHeight / 2 - 36;

				if (left < x && x < left + iconWidth && top < y && y < top + iconHeight) {
					return true;
				}
			}
		}
	}

	return false;
}

// avatar_avatar.cpp:398
ntAvatar *ntAvatar::GetFollowingAvatar() const {
	if (m_followName[0] != '\0') {
		return NULL;
	}

	return WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_followName);
}

// avatar_avatar.cpp:410
bool ntAvatar::GetBoneScreenCoord(const char *boneName, int *pX, int *pY) const {
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

// avatar_avatar.cpp:435
void ntAvatar::SetCharactorInfo(const sCharacterInfo &charInfo) {
	CPartTidList partTidList;

	Stop();

	m_pPetFrame = NULL; m_characterInfo = charInfo;
	m_charTypeId = charInfo.typeId;

	m_fxBox.ClearAll(false);
}

// avatar_avatar.cpp:449
// The original copies these vectors as a block (lea of the destination, then
// three dword moves), the code of an implicit WVector copy assignment; our
// wmath.inl's member-wise operator= folds the destination into the stores
// instead, so the copies are written as memcpy until wmath.h is settled.
void ntAvatar::SetPosition(const WVector &position) {
	memcpy(&m_position, &position, sizeof(WVector));
	memcpy(&m_targetPosition, &position, sizeof(WVector));
	m_targetPositionChanged = true;
}

// avatar_avatar.cpp:455
void ntAvatar::SetTargetPosition(const WVector &position) {
	memcpy(&m_targetPosition, &position, sizeof(WVector));
	m_targetPositionChanged = true;
}

// avatar_avatar.cpp:461
void ntAvatar::SetAngle(float angle) {
	float normalized = RadianNormalize2PI(angle);
	m_angle = normalized;
	m_targetAngle = normalized;
}

// avatar_avatar.cpp:468
void ntAvatar::SetTargetAngle(float angle) {
	m_targetAngle = RadianNormalize2PI(angle);
}

// avatar_avatar.cpp:474
void ntAvatar::SetPendingTargetAngle(float angle) {
	m_pendingTargetAngle = RadianNormalize2PI(angle);
	m_hasPendingTargetAngle = true;
}

// avatar_avatar.cpp:480
void ntAvatar::SetState(unsigned long state) {
	if (m_state == state) {
		return;
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSendPacket packet((enumClientPacket)0x63);
		packet.Encode1(8);
		packet.Encode4(state);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_state = state;
	if (state & 0x7E) {
		Stop();
	}
}

// avatar_avatar.cpp:503
void ntAvatar::SetAction(int action) {
	if (m_action == action) {
		return;
	}

	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		WSendPacket packet((enumClientPacket)0x63);
		packet.Encode1(5);
		packet.Encode4(action);
		packet.Send(SEND_TO_DEFAULT);
	}

	m_action = action;
}

// avatar_avatar.cpp:519
void ntAvatar::SendMove(const WVector &position, bool force) {
	if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
		&& this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		m_pendingMove += position;
		float speed = m_spItems[2].getvalue();
		if (m_pendingMove.Magnitude() >= speed * 16.5f || force) {
			WSendPacket packet((enumClientPacket)0x63);
			packet.Encode1(6);
			packet.EncodeBuffer(&m_pendingMove.x, 4);
			packet.EncodeBuffer(&m_pendingMove.z, 4);
			packet.EncodeBuffer(&m_angle, 4);
			packet.Send(SEND_TO_DEFAULT);

			m_pendingMove.Reset();
		}
	}
}

// avatar_avatar.cpp:542
void ntAvatar::Stop() {
	if (this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		if (!WisEqual(m_targetPosition, m_position, 0.00001f)) {
			SendMove(WVector(0.0f, 0.0f, 0.0f), true);

			m_targetPosition = m_position;
			m_targetPositionChanged = true;
		}

		m_moveFlag = false;
		m_followName[0] = '\0';
	}
}

// avatar_avatar.cpp:559
void ntAvatar::Follow(const char *nickName) {
	if (nickName) {
		strcpy(m_followName, nickName);
	}
	else {
		m_followName[0] = '\0';
	}
}

// avatar_avatar.cpp:571
void ntAvatar::ChangeMotion(int motion, const char *name, float blendTime) {
	const char *motionName;

	m_motionType = motion;

	if (m_pPetFrame == NULL) {
		return;
	}

	switch (motion) {
	case 0:
		motionName = "\xB1\xE2\xBA\xBB\xC0\xDA\xBC\xBC";
		break;
	case 2:
		motionName = "chat_\xBE\xC9\xB1\xE2";
		break;
	case 3:
		motionName = "chat_\xB4\xAF\xB1\xE2";
		break;
	case 1:
		motionName = "chat_\xB4\xDE\xB8\xAE\xB1\xE2";
		break;
	case 4:
		motionName = name;
		if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)
			&& this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
			unsigned long now = timeGetTime();

			if (now - m_motionSendTime <= 1000) {
				return;
			}

			m_motionSendTime = now;

			WSendPacket packet((enumClientPacket)0x63);
			packet.Encode1(7);
			packet.EncodeStr(std::string(name));
			packet.Send(SEND_TO_DEFAULT);
		}
		break;
	default:
		motionName = "\xB1\xE2\xBA\xBB\xC0\xDA\xBC\xBC";
		break;
	}

	w_motion_data *pMotion = m_pPetFrame->FindMotionData(motionName, true, NULL);
	if (pMotion == NULL) {
		return;
	}

	if (blendTime > 0.0f && m_pMotion != NULL) {
		m_prevBoneSet.CopyFrom(&m_boneSet);
		m_pPrevMotion = m_pMotion;
		m_prevMotionType = m_motionType;
		m_pMotion = pMotion;
		m_motionType = motion;
		m_blendElapsed = 0.0f;
		m_blendTime = blendTime;
	}
	else {
		m_pPrevMotion = NULL;
		m_blendElapsed = 0.0f;
		m_pMotion = pMotion;
		m_boneSet.CopyFrom(m_pPetFrame->GetCurAniPet()->GetBones(NULL, pMotion->s, 0, NULL, false));
	}

	if (m_blendElapsed == 0.0f) {
		bool invisible = motion != 4;

		switch ((m_charTypeId & 0x3FFFFFF) - 4) {
		case 0: {
			WBone *pBone = m_pPetFrame->GetBonePet()->FindBone("D_Ball");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}

			pBone = m_pPetFrame->GetBonePet()->FindBone("D_Racket");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}
			break;
		}
		case 1: {
			WBone *pBone = m_pPetFrame->GetBonePet()->FindBone("E_Cannon");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}

			pBone = m_pPetFrame->GetBonePet()->FindBone("E_Telescope");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}
			break;
		}
		case 2: {
			WBone *pBone = m_pPetFrame->GetBonePet()->FindBone("G_Bird");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}
			break;
		}
		case 3: {
			WBone *pBone = m_pPetFrame->GetBonePet()->FindBone("H_Sword01");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}

			pBone = m_pPetFrame->GetBonePet()->FindBone("H_Sword02");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}

			pBone = m_pPetFrame->GetBonePet()->FindBone("H_karen");
			if (pBone) {
				pBone->SetInvisible(false, invisible);
			}
			break;
		}
		}

		SetPendingTexPart("face", data_a82938[m_charTypeId & 0x3FFFFFF][3]);
	}

	ProcessMotion(0.0f);
}

// avatar_avatar.cpp:715
void ntAvatar::ProcessMotion(float elapsed) {
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		return;
	}

	if (m_motionType != 0 && m_motionType != 2 && m_motionType != 3 && m_motionType != 1) {
		if (m_pMotion->e - m_pMotion->s < m_blendElapsed) {
			ChangeMotion(0, NULL, 0.2f);
		}
	}

	if (m_pPrevMotion != NULL) {
		float t = m_blendElapsed / m_blendTime;

		if (t > 1.0f) {
			m_pPrevMotion = NULL;
		}
		else {
			float time = m_blendElapsed;
			float length = m_pMotion->e - m_pMotion->s;
			if (length < m_blendElapsed) {
				time = length;
			}

			m_boneSet.Blend(&m_prevBoneSet, m_pPetFrame->GetCurAniPet()->GetBones(m_pMotion->name, time, 0, NULL, false), t);
		}
	}
	else {
		float length = m_pMotion->e - m_pMotion->s;

		if (m_blendElapsed > length) {
			if (length > 0.01f) {
				float loops = floorf(m_blendElapsed / length);
				m_blendElapsed -= loops * length;
			}
			else {
				m_blendElapsed = 0.0f;
			}
		}

		m_boneSet.CopyFrom(m_pPetFrame->GetCurAniPet()->GetBones(NULL, m_pMotion->s + m_blendElapsed, 0, NULL, false));
	}

	if (m_blendElapsed == 0.0f) {
		ScriptProcess(m_pMotion, m_pMotion->s, m_pMotion->s, true);
	}
	else {
		ScriptProcess(m_pMotion, m_pMotion->s + m_blendElapsed, m_pMotion->s + m_blendElapsed + elapsed, false);
	}
}

// avatar_avatar.cpp:836
CQuadTree &ntAvatar::GetPVS() {
	return *m_pPVS;
}

// avatar_avatar.cpp:842
void ntAvatar::SetLocalPVS(CQuadTree &pvs) {
	m_pPVS = &pvs;
}

// avatar_avatar.cpp:851
// Removes from a velocity its part along a plane's normal (scaled by
// overbounce), the Quake way of sliding along a wall.
static void ClipVelocity(const WVector &in, const WVector &normal, WVector &out, float overbounce) {
	float backoff = WDotProduct(in, normal) * overbounce;
	out = in - normal * backoff;
}

// avatar_avatar.cpp:980
// The rotation that turns the world's up axis onto a ground normal.
static void MakeUpRotation(WMatrix &mat, const WVector &up) {
	if (WisEqual(up, WVector::UNIT_POS_Y, 1e-5f)) {
		mat.Reset();
	}
	else {
		WVector axis = WCrossProduct(up, WVector::UNIT_POS_Y);
		axis.Normalize();

		float cosAngle = WDotProduct(up, WVector::UNIT_POS_Y);
		mat = RotMat(axis, (float)acos(cosAngle));
	}
}

// avatar_avatar.cpp:1000
void ntAvatar::ProcessChangeState() {
	m_prevState = m_state;
}

// avatar_avatar.cpp:1019
void ntAvatar::Process(float elapsed) {
	if (m_flag3D8) {
		m_partTidList = m_burden.m_partTidList;

		m_flag3D8 = false; m_pPetFrame = m_burden.m_pPetFrame;
		m_pPetFrame->CalcBVolume();
		m_pPetFrame->UpdateLightSource(&g_lightset, false, NULL);

		switch (m_action) {
		case 0:
		default:
			ChangeMotion(0, NULL, 0.0f);
			break;
		case 1:
			ChangeMotion(2, NULL, 0.0f);
			break;
		case 2:
			ChangeMotion(3, NULL, 0.0f);
			break;
		}
	}

	if (WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar == this && (WSingleton<CSharedDoc>::Instance()->m_userInfo.flagBlock & 8)) {
		m_headIcon = 3;
	}
	else {
		m_headIcon = -1;
	}

	m_prevState = m_state; ProcessMotion(elapsed);

	if (m_followName[0] != '\0') {
		ntAvatar *pTarget = WSingleton<ntAvatarManager>::Instance()->FindAvatarByNickName(m_followName);
		if (pTarget) {
			WVector delta = pTarget->m_position - m_position;
			float dist = delta.Magnitude();
			if (dist > 64.0f) {
				m_followName[0] = '\0';
				Stop();
			}
			else if (dist > 12.8f && dist > 0.0f) {
				delta.Normalize();
				delta *= dist - 12.8f;
				SetTargetPosition(m_position + delta);
				SetAction(0);
			}
		}
		else {
			Stop();
			m_moveFlag = false;
			m_followName[0] = '\0';
		}
	}

	WVector move(0.0f, 0.0f, 0.0f); WVector dir = m_targetPosition - m_position;
	if (Wabs(dir.x) >= kMoveEpsilon || Wabs(dir.z) >= kMoveEpsilon) {
		dir.y = 0.0f; dir.Normalize();

		float speed = m_spItems[2].getvalue();
		WVector velocity = dir * 16.5f * speed * elapsed;

		WVector toTarget(m_targetPosition.x - m_position.x, 0.0f, m_targetPosition.z - m_position.z);
		if (velocity.Magnitude() > toTarget.Magnitude()) {
			velocity = toTarget;
		}

		WVector newPos;
		if (this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
			Waabb box(WVector(-1.0f, -40.0f, -1.0f), WVector(1.0f, 8.0f, 1.0f));
			SlideMove(velocity, m_position, m_pPVS, box, newPos);
		}
		else {
			newPos = m_position + velocity;
		}

		move = newPos - m_position;
		move.y = 0.0f;

		if (move.Magnitude() > 0.01f) {
			int hit;
			m_pPVS->GetGroundPoint(newPos, true, &hit, true);
			if (hit >= 0) {
				m_position = newPos;
			}
			else if (this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
				move = WVector(0.0f, 0.0f, 0.0f);
			}
		}
		else {
			move = WVector(0.0f, 0.0f, 0.0f);
		}
	}

	SendMove(move, false);

	float moveSq = move.x * move.x + move.z * move.z;
	if (sqrtf(moveSq) > 0.01f) {
		if (m_motionType != 1) {
			ChangeMotion(1, NULL, 0.2f);
		}
	}
	else {
		if (this == WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
			if (m_pendingMove.x != 0.0f || m_pendingMove.z != 0.0f) {
				SendMove(WVector::ZERO, true);
			}
			m_targetPosition = m_position;
		}

		if (m_motionType != 4) {
			switch (m_action) {
			case 0:
				if (m_motionType != 0) {
					ChangeMotion(0, NULL, 0.2f);
				}
				break;
			case 1:
				if (m_motionType != 2) {
					ChangeMotion(2, NULL, 0.2f);
				}
				break;
			case 2:
				if (m_motionType != 3) {
					ChangeMotion(3, NULL, 0.3f);
				}
				break;
			}
		}
	}

	if (moveSq > 0.001f) {
		m_targetAngle = (float)atan2(dir.z, dir.x) + 1.5707964f;
	}
	else if (this != WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar) {
		if (m_hasPendingTargetAngle) {
			m_targetAngle = RadianNormalize2PI(m_pendingTargetAngle);
		}
	}

	float turn = RadianNormalize2PI(m_targetAngle - m_angle); if (turn > 3.1415927f) turn -= 6.2831855f;
	if (turn > 0.0f && m_targetAngle < m_angle) {
		m_angle -= 6.2831855f;
	}
	else if (turn < 0.0f && m_targetAngle > m_angle) {
		m_angle += 6.2831855f;
	}

	if (fabs(turn) > 0.001f) {
		m_angle += elapsed * turn * 10.0f;
		if ((turn > 0.0f && m_angle > m_targetAngle) || (turn < 0.0f && m_angle < m_targetAngle)) {
			m_angle = m_targetAngle;
		}
	}

	m_position = m_pPVS->GetGroundPoint(m_position, false, NULL, false);

	WVector toCamera = g_view->camera.pivot - m_position;

	float cameraDist = sqrtf(toCamera.x * toCamera.x + toCamera.z * toCamera.z);
	if (cameraDist > 192.0f) {
		m_time = 0.0f;
	}
	else if (cameraDist > 160.0f) {
		m_time = 1.0f - (cameraDist - 160.0f) * 0.03125f;
	}
	else {
		bool faded = false;
		if (IsLocalContent((localContentType_t)10)) {
			CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
			int guid = m_guid;
			const sBriefUserInfo &info = (*pDoc->m_userMap.find(guid)).second;

			if (info.identityBits.administrator) {
				if (!info.invisible) {
					faded = true;
				}
			}
		}
		if (faded) {
			m_time = 0.1f;
		}
		else {
			m_time = 1.0f;
		}
	}

	WVector up;
	if (m_motionType == 3) {
		up = m_pPVS->GetMeshNormal(m_position);
	}
	else {
		up = WVector::UNIT_POS_Y;
	}

	WVector axis = WCrossProduct(m_groundNormal, up);
	axis.Normalize();

	if (Wabs(axis.SquareMagnitude()) >= kMoveEpsilon) {
		if (acos(WDotProduct(up, m_groundNormal)) < elapsed * 6.2831855f) {
			m_groundNormal = up;
		}
		else {
			WMatrix rot = RotMat(axis, elapsed * -6.2831855f);
			m_groundNormal = RotVec(m_groundNormal, rot);
		}
	}

	WMatrix mat; MakeUpRotation(mat, m_groundNormal);

	mat = RotMat(m_angle, 1) * mat;

	float scale = m_spItems[0].getvalue(); mat.pivot = m_position;
	WVector axisScale(scale, scale, scale); mat.AxisScale(axisScale);

	if (m_pPetFrame) {
		m_pPetFrame->ApplyBones(&m_boneSet, mat);
		m_pPetFrame->Transform(g_view, 1.0f, 0);
		m_pPetFrame->UpdateBound(true);
		m_pPetFrame->SetAlpha((unsigned char)(m_time * 255.0f));
		m_pPetFrame->GetBonePet()->ScaleBone("Bip01 Head", m_spItems[1].getvalue());
	}

	if (m_facialTime > 0.0f) {
		m_facialTime -= elapsed;
		if (m_facialTime < 0.0f) {
			m_facialTime = 0.0f;
			SetPendingTexPart("face", data_a82938[m_charTypeId & 0x3FFFFFF][3]);
		}
	}

	m_balloonElapsed += elapsed;
	if (m_balloonElapsed > 5.0f) {
		WSingleton<ntBalloonTextManager>::Instance()->DeleteBalloonText(m_pBalloonText);
		m_pBalloonText = NULL;
	}

	m_fxBox.ProcessAttacher(elapsed * 1000.0f);
	m_fxBox.Process(elapsed * 1000.0f);

	m_blendElapsed += elapsed;
	m_value6F8 += elapsed;
}

// avatar_avatar.cpp:1364
void ntAvatar::Render() {
	if (IsLocalContent((localContentType_t)10)) {
		if (this != WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar && m_canKick) {
			CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
			int guid = m_guid;
			const sBriefUserInfo &info = (*pDoc->m_userMap.find(guid)).second;

			if (!sBriefUserInfo(info).invisible) {
				if (guid != WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid) {
					return;
				}
			}
		}
	}

	WVector pos;
	if (m_pPetFrame == NULL || m_pPetFrame->GetCurAniPet() == NULL) {
		pos = m_pPVS->GetGroundPoint(m_position, false, NULL, false);
	}
	else {
		pos = m_pPetFrame->GetBonePet()->FindBone("Bip01 Pelvis")->GetMatrix().pivot;
		pos = m_pPVS->GetGroundPoint(pos, false, NULL, false);
	}

	if (COption::Instance()->vIsShadowEnabled()) {
		WVector xAxis = WVector(2.9f, 0.0f, 0.0f) * m_spItems[0].getvalue();
		WVector zAxis = WVector(0.0f, 0.0f, 2.9f) * m_spItems[0].getvalue();

		m_pPVS->DrawDecal(m_pChatMain->m_shadowTexture, pos, xAxis, zAxis, 1.0f, m_time, (enumPolyBlend)0, false);
	}

	if (m_pPetFrame != NULL && m_pPetFrame->GetCurAniPet() != NULL) {
		if (g_view->InFrustum(m_pPetFrame->GetBonePet()->GetBoundBox())) {
			if (WSingleton<ntAvatarManager>::Instance()->m_pLitAvatar == this) {
				m_light.nearone = g_lightset.nearone;
				m_light.diffuse = m_light.ambient = 0xFFFFFFFF;
			}
			else {
				m_light.nearone = g_lightset.nearone;
				m_light.ambient = ModifyColor(g_lightset.ambient, 1.0f);
				m_light.diffuse = ModifyColor(g_lightset.diffuse, m_pPVS->GetShadowIntensity(pos));
			}

			m_pPetFrame->UpdateLightSource(&m_light, false, NULL);

			m_pPetFrame->Render(g_view, true, 1.0f, false, false, false);
		}
	}

	g_view->Flush(1);

	m_fxBox.Display(g_view);
}

// avatar_avatar.cpp:1814
void ntAvatar::RefreshUccClothes(unsigned long tid, unsigned long id) {
	if (m_pPetFrame == NULL) {
		return;
	}

	if (IsLocalContent((localContentType_t)0x5F)) {
		m_pPetFrame->SetPart(&m_partTidList, tid, id);
		m_pPetFrame->SetPart(&m_partTidList, tid, id);

		m_pPetFrame->GetBonePet()->xUpdateTnLBuffers(true);
	}
}

// avatar_avatar.cpp:1843
void ntAvatar::ScriptProcess(const w_motion_data *data, float from, float to, bool all) {
	if ((!(from <= data->e) || from == to) && !all) {
		return;
	}

	int start, end;
	from *= 30.0f;
	if (all) {
		start = Ceil(from, 0.03f);
		end = start;
	} else {
		to = MinOf(data->e, to) * 30.0f;
		start = Ceil(from, 0.03f);
		end = WAbsBySign((float)floor(to) - to) < kScriptFrameEpsilon ? (int)to - 1 : (int)floor(to);

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

// avatar_avatar.cpp:2175
void ntAvatar::ParseScript_ShowVis(bool show, const char *script) {
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

// avatar_avatar.cpp:2218
sTexturePart *ntAvatar::FindTexturePart(const char *part, const char *texture) {
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

// avatar_avatar.cpp:2241
void ntAvatar::SetPendingTexPart(const char *part, const char *texture) {
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

// avatar_avatar.cpp:2266
void ntAvatar::SetChatFacial(int facial) {
	SetPendingTexPart("face", data_a82938[m_charTypeId & 0x3FFFFFF][facial]);

	m_facialTime = 5.0f;
}

// avatar_avatar.cpp:2275
void ntAvatar::SetBalloonText(const char *text) {
	if (m_pBalloonText == NULL) {
		m_pBalloonText = WSingleton<ntBalloonTextManager>::Instance()->CreateBalloonText(m_guid, text, m_balloonOffset, 5.0f);
	}
	else {
		m_pBalloonText->SetText(text);
	}

	m_balloonTime = m_time;
	m_balloonElapsed = 0;
}

// avatar_avatar.cpp:2286
void ntAvatar::SetTradeTitle(const char *title) {
	if (title) {
		strcpy(m_tradeTitle, title);
	}
	else {
		m_tradeTitle[0] = '\0';
	}
}

// avatar_avatar.cpp:2294
SpecialPrizeItem &ntAvatar::GetMySpItem(eSPAVILITYTYPE type) {
	return m_spItems[type];
}

// avatar_avatar.cpp:2299
void ntAvatar::UseSPItem(eSPAVILITYTYPE type) {
	WSendPacket packet(0xC);
	packet.Encode1(6);
	packet.Encode4(WSingleton<ntAvatarManager>::Instance()->m_pMyAvatar->m_guid);
	packet.Encode4(type);
	packet.Send(SEND_TO_DEFAULT);
}

// avatar_avatar.cpp:2310
// The original also zeroes the dwords at 0xA8 and 0xAC, which nothing else uses.
ntAvatarManager::ntAvatarManager() {
	m_pPVS = NULL;
	m_pPVS = &::GetPVS();

	m_avatarCount = 0;
	for (int i = 0; i < 30; i++) {
		m_avatars[i] = NULL;
	}

	m_pMyAvatar = NULL;
	m_pLitAvatar = NULL;
	m_pHighlightAvatar = NULL;
	m_active = true;
	m_overlayDetail = true;
}

// avatar_avatar.cpp:2331
ntAvatarManager::~ntAvatarManager() {
	FreeAllAvatars();
}

// avatar_avatar.cpp:2336
void ntAvatarManager::FreeAllAvatars() {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (m_avatars[i]->m_pPetFrame) {
				delete m_avatars[i]->m_pPetFrame;
				m_avatars[i]->m_pPetFrame = NULL;
			}

			if (m_avatars[i]) {
				delete m_avatars[i];
				m_avatars[i] = NULL;
			}
		}
	}
}

// avatar_avatar.cpp:2358
ntAvatar *ntAvatarManager::FindAvatarByIDName(const char *idName) const {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (strcmp(m_avatars[i]->m_idName, idName) == 0) {
				return m_avatars[i];
			}
		}
	}

	return NULL;
}

// avatar_avatar.cpp:2374
ntAvatar *ntAvatarManager::FindAvatarByNickName(const char *nickName) const {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (strcmp(m_avatars[i]->m_nickName, nickName) == 0) {
				return m_avatars[i];
			}
		}
	}

	return NULL;
}

// avatar_avatar.cpp:2390
ntAvatar *ntAvatarManager::FindAvatarByGuid(int guid) const {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (m_avatars[i]->m_guid == guid) {
				return m_avatars[i];
			}
		}
	}

	return NULL;
}

// avatar_avatar.cpp:2406
ntAvatar *ntAvatarManager::CreateAvatar(const sSlotInfo &slotInfo, const sCharacterInfo &charInfo, ntAvatarChatMain *pChatMain) {
	int i;
	for (i = 0; i < 30; i++) {
		if (!m_avatars[i])
			break;
	}

	if (i == 30) {
		return NULL;
	}

	m_avatarCount++;

	ntAvatar *pAvatar = new ntAvatar(*m_pPVS, slotInfo, charInfo, pChatMain);

	WVector pos(slotInfo.m_x, 0.0f, slotInfo.m_z);
	pos = m_pPVS->GetGroundPoint(pos, false, NULL, false);
	pAvatar->SetPosition(pos);
	pAvatar->SetAngle(slotInfo.m_angle);

	m_avatars[i] = pAvatar;
	return pAvatar;
}

// avatar_avatar.cpp:2484
bool ntAvatarManager::DeleteAvatar(const char *idName) {
	int i;
	for (i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (strcmp(m_avatars[i]->m_idName, idName) == 0) {
				break;
			}
		}
	}

	if (i == 30) {
		return false;
	}

	if (m_avatars[i]) {
		delete m_avatars[i];
		m_avatars[i] = NULL;
	}
	m_avatarCount--;

	return true;
}

// avatar_avatar.cpp:2509
ntAvatar *ntAvatarManager::AvatarByRayIntersect(const WVector &pos, const WVector &dir) const {
	Waabb aabb;
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			if (IsLocalContent((localContentType_t)10)) {
				if (m_avatars[i]->m_guid != MyGuid(false)) {
					CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
					std::map<unsigned long, sBriefUserInfo>::iterator it = pDoc->m_userMap.find(m_avatars[i]->m_guid);

					if (!IsAdministrator(0xFFFFFFFF)) {
						if (sBriefUserInfo((*it).second).identityBits.administrator) {
							if (!sBriefUserInfo((*it).second).invisible)
								break;
						}
					}
				}
			}

			aabb.min = WVector(-1.0f, 0.0f, -1.0f);
			aabb.max = WVector(1.0f, 8.0f, 1.0f);
			aabb.min += m_avatars[i]->m_position; aabb.max += m_avatars[i]->m_position;

			if (RayIntersectAABB(aabb, pos, dir, NULL, 0.0f) < 1.0f) {
				return m_avatars[i];
			}
		}
	}

	return NULL;
}

// avatar_avatar.cpp:2548
ntAvatar *ntAvatarManager::CheckClick(float x, float y) const {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i] && m_avatars[i]->IsClickable(x, y)) {
			return m_avatars[i];
		}
	}

	return NULL;
}

// avatar_avatar.cpp:2561
void ntAvatarManager::Process(float elapsed) {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			m_avatars[i]->Process(elapsed);
		}
	}
}

// avatar_avatar.cpp:2572
void ntAvatarManager::Render() {
	if (!m_active) {
		return;
	}

	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			m_avatars[i]->Render();
		}
	}
}

// avatar_avatar.cpp:2586
void ntAvatarManager::ResetAvatarPositionAngle() {
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			m_avatars[i]->m_position = m_avatars[i]->m_targetPosition;
			m_avatars[i]->m_angle = m_avatars[i]->m_targetAngle;
		}
	}
}

// avatar_avatar.cpp:2601
CQuadTree &ntAvatarManager::GetPVS() {
	return *m_pPVS;
}

// avatar_avatar.cpp:2607
void ntAvatarManager::SetLocalPVS(CQuadTree &pvs) {
	m_pPVS = &pvs;
}

// avatar_avatar.cpp:2613
// qsort order of RenderOverlay: the avatar farther from the camera first.
static int __cdecl CompareCameraDistance(const void *a, const void *b) {
	WVector delta = g_view->GetCamera().pivot;
	WVector deltaA;
	deltaA = delta - (*(ntAvatar *const *)a)->m_position;
	delta = delta - (*(ntAvatar *const *)b)->m_position;

	float distA = deltaA.x * deltaA.x + deltaA.y * deltaA.y + deltaA.z * deltaA.z;
	float distB = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;

	if (distA > distB) {
		return -1;
	}
	if (distA < distB) {
		return 1;
	}
	return 0;
}

// avatar_avatar.cpp:2634
void ntAvatarManager::RenderOverlay() {
	if (!m_active) {
		return;
	}

	ntAvatar *avatars[30];
	int count = 0;
	for (int i = 0; i < 30; i++) {
		if (m_avatars[i]) {
			avatars[count++] = m_avatars[i];
		}
	}

	qsort(avatars, count, sizeof(ntAvatar *), CompareCameraDistance);

	for (int j = 0; j < count; j++) {
		if (avatars[j]) {
			avatars[j]->RenderOverlay(m_overlayDetail);
		}
	}
}

// avatar_avatar.cpp:2659
ntAvatarBurden::ntAvatarBurden(unsigned long first, unsigned long key, const sCharacterInfo &charInfo, CPetFrame *pPetFrame) : m_charInfo() {
	m_first = first;
	m_key = key;

	m_charInfo = charInfo;
	m_pPetFrame = pPetFrame;
}

// avatar_avatar.cpp:2667
ntAvatarBurden::~ntAvatarBurden() {
}

// avatar_avatar.cpp:2672
bool ntAvatarBurden::Load() {
	if (!m_partTidList.SetTids(&m_charInfo, 0xFF)) {
		m_partTidList.SetDefaultTids();
	}
	if (!m_partTidList.IsComboValid()) {
		m_partTidList.SetDefaultTids();
	}

	if (m_pPetFrame) { delete m_pPetFrame; m_pPetFrame = NULL; }

	m_pPetFrame = new CPetFrame;
	if (!m_pPetFrame->Build(&m_partTidList, m_charInfo.auxParts.tids, true, true, m_key)) {
		delete m_pPetFrame;
		m_pPetFrame = NULL;
		return false;
	}

	m_pPetFrame->SetHairClr(&m_partTidList, m_charInfo.hairColor);
	m_pPetFrame->SetShirtsClr(&m_partTidList, m_charInfo.shirtColor & 0xF);

	return true;
}

// balloontext.h:24
void ntBalloonText::SetTransparency(float transparency) {
	m_transparency = transparency;
}

// specialprizeitem.h:9
SpecialPrizeItem::SpecialPrizeItem() : m_flag(false), m_value(1.0f) {
}

// specialprizeitem.h:10
SpecialPrizeItem::~SpecialPrizeItem() {
}

// specialprizeitem.h:16
void SpecialPrizeItem::InitalizeItem() {
	m_flag = false;
	m_value = 1.0f;
}

// shareddoc.inl:549
int IsAdministrator(unsigned long identity) {
	if (identity == 0xFFFFFFFF)
		identity = WSingleton<CSharedDoc>::Instance()->m_userInfo.identity;

	return (identity & 4) == 4;
}
