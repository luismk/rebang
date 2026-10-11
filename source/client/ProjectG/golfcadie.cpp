// Original: d:\build\custom\temp\code\client\projectg\golfcadie.cpp
#include "NetworkSystem.h"
#include "golfcadie.h"

#include "petbody.h"
#include "Shadow.h"
#include "wscene.h"
#include "../shared/globalgamedefine.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "golfball.h"
#include "ChatMsg.h"
#include "wpuppet.h"
#include "wbone.h"
#include "effect.h"
#include "fxinterpreter.h"
#include "RandomHouse.h"
#include "commonutil.h"
#include "slicestr.h"
#include "quadtree.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/westpak.h"

#include <algorithm>

extern unsigned long g_CurrentTime;
extern LightSet g_lightset;
extern WMatrix g_camera;

// 3.1415927f, a named constant of the original (its symbol is not known).
extern "C" const float data_9eee9c;

// golfcadie.cpp:30 (run-time type and factory registration of CGolfCaddie)
const WRTTI CGolfCaddie::m_RTTI("CGolfCaddie", &IActor::m_RTTI);

IObject *CGolfCaddieMakeInstance(void) {
	return new CGolfCaddie;
}

struct __impCGolfCaddie {
	__impCGolfCaddie(void) {
		ObjectFactory().AddObjectFunctor(CGolfCaddieMakeInstance, "CGolfCaddie");
		ActorFactory().AddActorClass("CGolfCaddie");
	}
};
static __impCGolfCaddie s_impCGolfCaddie;

// golfcadie.cpp:153
bool cCaddieTalkTeeShot::FindMsg(void) {
	if (GolfBall().m_groundType) {
		return false;
	}

	std::map<unsigned int, sCaddieInfo>::iterator itCaddie = CSharedDoc::Instance()->m_caddieMap.find(CSharedDoc::Instance()->m_myCaddieId);
	if (itCaddie == CSharedDoc::Instance()->m_caddieMap.end()) {
		return false;
	}

	sTalkData key;
	key.m_hole = itCaddie->second.typeId & 0x3FFFFFF;
	key.m_condition = GOLFDOC()->m_currentHole;
	key.m_par = GetCurMap();

	for (std::vector<sTalkData>::iterator it = m_talkData.begin(); it != m_talkData.end(); ++it) {
		if (*it == key) {
			m_talkMsg = it->m_msg;

			m_talkData.erase(it);
			return true;
		}
	}

	return false;
}

// golfcadie.cpp:186
CCaddieTalk::~CCaddieTalk(void) {
	std::for_each(m_talks.begin(), m_talks.end(), Delete_Object());
}

// golfcadie.cpp:190
void CCaddieTalk::Release(void) {
	std::for_each(m_talks.begin(), m_talks.end(), Delete_Object());
}

// golfcadie.cpp:195
void CCaddieTalk::Init(void) {
	m_isTalking = 0;

	ICaddieTalk *talk = new cCaddieTalkTeeShot;
	m_talks.push_back(talk);

	std::list<ICaddieTalk *>::iterator it = m_talks.begin();
	while (it != m_talks.end()) {
		(*it)->Load();
		++it;
	}
}

// golfcadie.cpp:212
const ICaddieTalk *CCaddieTalk::FindTalk(void) {
	if (m_isTalking)
		return 0;

	std::list<ICaddieTalk *>::iterator it = m_talks.begin();
	while (it != m_talks.end()) {
		if ((*it)->FindMsg()) {
			m_isTalking = 1;
			return *it;
		}
		++it;
	}

	return 0;
}

// golfcadie.cpp:231
void CGolfCaddie::OnDestroy(void) {
	GetFxInterpreter()->DetachFxAll();

	m_fxScript = -1;
	for (std::list<sCaddie *>::iterator it = m_caddies.begin(); it != m_caddies.end(); ++it) {
		if ((*it)->m_pBody) { delete (*it)->m_pBody; (*it)->m_pBody = 0; }
		if (*it) { delete *it; *it = 0; }
	}

	m_caddies.clear();

	if (m_pRing) { delete m_pRing; m_pRing = 0; }
	if (m_pCaddieTalk) { delete m_pCaddieTalk; m_pCaddieTalk = 0; }
}

// golfcadie.cpp:248
void CGolfCaddie::OnPreLoadInit(void) {
	CSceneManager::Instance()->RegisterElement((eObjType)4, 0, this, "\xc4\xb3\xb5\xf0", 0x18, 0);
}

// golfcadie.cpp:317
void CGolfCaddie::OnReload(void) {
	OnLoad();
}

// golfcadie.cpp:466
CGolfCaddie::sCaddie *CGolfCaddie::FindCaddie(unsigned long typeId, unsigned long cadItemTypeId) {
	std::list<sCaddie *>::iterator it;
	it = std::find_if(m_caddies.begin(), m_caddies.end(), sFindCaddie(typeId, cadItemTypeId));

	if (it != m_caddies.end())
		return *it;

	return 0;
}

// golfcadie.cpp:478
void CGolfCaddie::OnInit(void) {
	m_pCaddie = FindCaddie(CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_caddieTypeId, CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_cadItemTypeId);

	m_unknown30 = 0;
	m_unknown44 = 0;
	m_angle = 0;
	m_unknown68 = 0;
	m_dodging = false;
	m_visible = true;
	m_unknown64 = 0;
	m_unknown60 = 0;
}

// golfcadie.cpp:504
void CGolfCaddie::LoadTalkText(sCaddie *caddie) {
	caddie->m_talks.clear();

	char fileName[32];
	fileName[0] = 0;
	sprintf(fileName, "%s.talk", caddie->m_pIffCaddie->m_model);

	cFile *file = g_resrcmng->GetCFile(fileName, -1);

	if (!file)
		return;

	int length = file->Length();

	char *buffer;
	buffer = new char[length + 1];

	file->Read(buffer, length);
	buffer[length] = 0;
	CloseCFile(file);

	char *p = buffer;
	while (p && *p && strstr(p, ":")) {
		char name[256] = "";
		char level[256] = "";
		char motion[256] = "";

		char *next = GetBehaviorToken(name, p);
		if (!next || !*next || !name[0])
			break;

		next = GetBehaviorToken(level, next);
		if (!next || !*next || !level[0])
			break;

		p = GetBehaviorEndToken(motion, next);

		sBehavior behavior;
		behavior.m_level = atoi(level);
		behavior.m_motion = motion;

		caddie->m_talks[name] = behavior;
	}

	delete[] buffer;
}

// golfcadie.cpp:559
void CGolfCaddie::LoadMotionLevel(sCaddie *caddie) {
	caddie->m_motions.clear();

	char fileName[32];
	fileName[0] = 0;
	sprintf(fileName, "%s.mot", caddie->m_pIffCaddie->m_model);

	cFile *file = g_resrcmng->GetCFile(fileName, -1);

	if (!file)
		return;

	int length = file->Length();

	char *buffer;
	buffer = new char[length + 1];

	file->Read(buffer, length);
	buffer[length] = 0;
	CloseCFile(file);

	char *p = buffer;
	while (p && *p && strstr(p, ":")) {
		char level[256] = "";
		char name[256] = "";

		char *end = GetBehaviorToken(level, p);
		if (!end || !*end || !level[0])
			break;

		p = GetBehaviorEndToken(name, end);

		sBehavior &behavior = caddie->m_motions[name];
		behavior.m_level = atoi(level);
	}

	delete[] buffer;
}

// golfcadie.cpp:606
bool CGolfCaddie::IsGoodMotion(sCaddie *caddie, const char *motion) {
	std::map<std::string, sBehavior>::iterator it = caddie->m_motions.find(motion);
	if (it == caddie->m_motions.end())
		return true;

	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (m_pCaddie->m_pInfo) {
			if (m_pCaddie->m_pInfo->level < it->second.m_level)
				return false;
		}
		else {
			if (it->second.m_level > 0)
				return false;
		}
	}

	return true;
}

// golfcadie.cpp:631
bool CGolfCaddie::ReadAttribute(sCaddie *caddie) {
	WPuppet *pet = caddie->m_pBody->GetPet();
	if (pet) {
		w_motion_data *motion = pet->FindMotionData("\xb4\xde\xb8\xae\xb1\xe2");
		if (!motion)
			return false;

		int frame = (int)(motion->s * 30.0f + 0.5f);

		int index = -1, count;
		pet->CheckFrameData(frame, frame, index, count);
		if (index > -1) {
			WSliceStr slice(pet->GetFrameData(index));

			char ringTexture[16];
			ringTexture[0] = '\0';
			caddie->m_showRing = slice.Get("*ring", "%s", ringTexture);
			if (caddie->m_showRing) {
				if (!m_pRing)
					m_pRing = new CRing(ringTexture);

				m_pRing->SetCenter(WVector(m_position.x, m_position.y + 0.3f, m_position.z));
				m_pRing->SetActive(true);
			}

			slice.Get("*stop_dist", "%f", &caddie->m_stopDist);
			caddie->m_stopDist *= 3.2f;

			slice.Get("*start_delay", "%f", &caddie->m_startDelay);

			slice.Get("*pet_speed", "%f", &caddie->m_petSpeed);

			bool noCaddieView = slice.Get("*no_caddie_view", "");

			caddie->m_petSpeed *= 3.2f; caddie->m_caddieView = !noCaddieView;
			return true;
		}
	}

	return false;
}

// golfcadie.cpp:683
char *CGolfCaddie::GetBehaviorToken(char *token, char *src) {
	while (*src == ' ' || *src == '\n') src++;

	while (*src) {
		if (*src == ' ') {
			src++;
			continue;
		}

		if (*src & 0x80)
			*token++ = *src++;

		*token++ = *src++;
		if (*src == ':') {
			*token = '\0';
			return src + 1;
		}
	}

	return 0;
}

// golfcadie.cpp:710
char *CGolfCaddie::GetBehaviorEndToken(char *token, char *src) {
	while (*src == ' ') src++;

	*token++ = *src++;
	while (*src != '\r' && *src != '\0') *token++ = *src++;
	*token = '\0';
	if (*src != '\0')
		src++;

	return src;
}

// golfcadie.cpp:1190
void CGolfCaddie::SetBubble(unsigned char type, bool chatOnly, unsigned char param) {
	sBehavior *behavior = SetTalkToBubble(type, chatOnly, param);
	if (!behavior)
		return;

	m_bubbleTalkType = type;
	WVector pos = m_pCaddie->m_pBody->GetPet()->FindBone("Bip01 HeadNub")->GetMatrix().pivot;
	SendMsg(this, "Bubble", 0x13, (intptr_t)behavior->m_motion.c_str(), (intptr_t)&pos, 0, 0);
}

// golfcadie.cpp:1235
void CGolfCaddie::SetBubble(unsigned char type, const char *msg) {
	if (m_pCaddie && !m_talked[type]) {
		m_talked[type] = true;

		std::map<std::string, sBehavior>::iterator it = m_pCaddie->m_talks.find(talkTable[type].m_name);
		if (it == m_pCaddie->m_talks.end())
			return;

		if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
			if (m_pCaddie->m_pInfo) {
				if (m_pCaddie->m_pInfo->level < it->second.m_level)
					return;
			}
			else if (it->second.m_level > 0)
				return;
		}

		m_bubbleTalkType = type;
		WVector pos = m_pCaddie->m_pBody->GetPet()->FindBone("Bip01 HeadNub")->GetMatrix().pivot;
		SendMsg(this, "Bubble", 0x13, (intptr_t)msg, (intptr_t)&pos, 0, 0);
	}
}

// golfcadie.cpp:1268
CGolfCaddie::sBehavior *CGolfCaddie::SetTalkToBubble(unsigned char type, bool chatOnly, unsigned char param) {
	if (!m_pCaddie || m_talked[type])
		return 0;

	if (!chatOnly && m_bubbleTalkType)
		return 0;

	if (GOLFDOC()->m_gameState != 15)
		return 0;

	m_talked[type] = true;
	std::map<std::string, sBehavior>::iterator it = m_pCaddie->m_talks.find(MakeStr("%s%d", talkTable[type].m_name, !param ? CRandomHouse::Instance()->GetRandom() % 5 + 1 : param));
	if (it == m_pCaddie->m_talks.end())
		return 0;

	sBehavior *behavior = &it->second;
	if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
		if (m_pCaddie->m_pInfo) {
			if (m_pCaddie->m_pInfo->level < behavior->m_level)
				return 0;
		}
		else {
			if (behavior->m_level > 0)
				return 0;
		}
	}

	return behavior;
}

// golfcadie.cpp:1308
void CGolfCaddie::SetCaddieTalk_ChatOnly(unsigned char type) {
	sBehavior *behavior = SetTalkToBubble(type, true, 0);
	if (!behavior)
		return;

	std::string msg;
	IFF_STRUCT::sCaddie *iffCaddie = ItemManager()->FindCaddie(CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_caddieTypeId);
	if (iffCaddie) {
		msg = iffCaddie->m_name;
		msg += " : ";
		msg += behavior->m_motion;
		CChatMsg::Instance()->AddChatMsg(msg, 0xff00ff00, true, false);
	}
}

// golfcadie.cpp:1353
void CGolfCaddie::Display(void) {
	if (!ShouldDisplay())
		return;

	m_pCaddie->m_pBody->Display();

	if (m_pRing && m_pCaddie->m_showRing)
		m_pRing->Display();
}

// golfcadie.cpp:1364
void CGolfCaddie::DisplayShadow(void) {
	if (!ShouldDisplay())
		return;

	CShadowManager::Instance()->RegisterCaster_New("caddie", 0x10, "sun", g_lightset);

	m_pCaddie->m_pBody->DisplayShadow("caddie");
}

// golfcadie.cpp:1374
void CGolfCaddie::ResetTalk(void) {
	m_bubbleTalkType = 0;
	for (int i = 0; i < 36; i++)
		m_talked[i] = false;

	m_talkFlag = false;
	m_talkResetTime = g_CurrentTime;
}

// golfcadie.cpp:1809
void CGolfCaddie::SearchGround(void) {
	for (int i = 0; i < 4; i++) {
		m_angle = GOLFDOC()->GetCameraData().m_yaw + (i & 1 ? 1.0f : -1.0f) * 0.6981317f + (i & 2 ? data_9eee9c : 0.0f);
		m_position = GetPVS().GetGroundPoint(GolfBall().m_pos + RotMat(m_angle, 1).za * 22.4f, true, 0, false);

		if (GetPVS().GetMeshIndex(m_position, true) < 0)
			continue;

		if (!(GetPVS().GetMeshNormal(m_position).y < cos(0.3490658476948738))) {
			if (!GetPVS().OverlappedWithObject(m_position, 1.6f))
				break;
		}
	}
}

// golfcadie.cpp:1838
bool CGolfCaddie::ShouldDisplay(void) {
	if (!m_visible || !m_pCaddie)
		return false;

	float dist = (g_camera.pivot - m_position).Magnitude();
	if (dist > 640.0f)
		return false;

	return true;
}

// golfcadie.cpp:1849
void CGolfCaddie::Dodge(void) {
	if (!m_pCaddie)
		return;

	WVector delta = m_position - GolfBall().PreData(0)->m_pos;
	float angle = CalcDeltaAngle(delta, GolfBall().PreData(0)->m_velocity);

	switch (GetPVS().CollideBetweenBallandCaddie()) {
	case 2:
	case 3:
		if (angle > 0.0f) {
			if (IsGoodMotion(m_pCaddie, "\xbf\xde\xc2\xca\xc0\xb8\xb7\xce\xc7\xc7\xc7\xcf\xb1\xe2")) {
				m_dodging = true;
				m_pCaddie->m_pBody->SetMotion("\xbf\xde\xc2\xca\xc0\xb8\xb7\xce\xc7\xc7\xc7\xcf\xb1\xe2", false, 0.0f, false);
			}
		} else {
			if (IsGoodMotion(m_pCaddie, "\xbf\xc0\xb8\xa5\xc2\xca\xc0\xb8\xb7\xce\xc7\xc7\xc7\xcf\xb1\xe2")) {
				m_dodging = true;
				m_pCaddie->m_pBody->SetMotion("\xbf\xc0\xb8\xa5\xc2\xca\xc0\xb8\xb7\xce\xc7\xc7\xc7\xcf\xb1\xe2", false, 0.0f, false);
			}
		}
		break;
	case 1:
		if (IsGoodMotion(m_pCaddie, "\xb3\xf4\xc0\xba\xb0\xf8\xc7\xc7\xc7\xcf\xb1\xe2")) {
			m_dodging = true;
			m_pCaddie->m_pBody->SetMotion("\xb3\xf4\xc0\xba\xb0\xf8\xc7\xc7\xc7\xcf\xb1\xe2", false, 0.0f, false);
		}
		break;
	case 0:
		m_dodging = false;
		LookUp(&GolfBall().m_pos, true);
		break;
	}
}

// golfcadie.cpp:1955
void CGolfCaddie::LookUp(WVector *target, bool immediate) {
	if (m_pCaddie)
		m_pCaddie->m_pBody->LookUp(target, immediate);
}

// golfcadie.cpp:1961
void CGolfCaddie::DontLookUp(void) {
	if (m_pCaddie)
		m_pCaddie->m_pBody->LookUp(0, true);
}

// golfcadie.cpp:1967
void CGolfCaddie::UseSpecialAbility(void) {
}
