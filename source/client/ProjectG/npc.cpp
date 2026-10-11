// Original: d:\build\custom\temp\code\client\projectg\npc.cpp
#include "NetworkSystem.h"
#include "npc.h"
#include "ObjectFactory.h"
#include "wmath.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "polysoup.h"
#include "wresrcmng.h"
#include "golfball.h"
#include "quadtree.h"
#include "wpuppet.h"
#include "wview.h"
#include "commonutil.h"
#include "ClientSetting.h"
#include "wutil.h"
#include "slicestr.h"
#include "fx.h"
#include "wbone.h"
#include "wlist.h"
#include "SoundManager.h"
#include <string.h>
#include <stdio.h>
#include <string>
#include <algorithm>
#include <float.h>
#include <malloc.h>

float Random(float min, float max, int seed);  // commonutil.cpp

extern WMatrix g_camera;

// ?g_lightset@@3ULightSet@@A; its header (wscene.h) does not declare it yet.
extern LightSet g_lightset;

// One degree in radians, a file-static float of the original (read from
// memory, not folded); its name is not known.
static float data_9f0fc0 = 0.017453292f;

// npc.cpp:24 (run-time type and factory registration of CNPC)
const WRTTI CNPC::m_RTTI("CNPC", &IActor::m_RTTI);

IObject *CNPCMakeInstance(void) {
	return new CNPC;
}

struct __impCNPC {
	__impCNPC(void) {
		ObjectFactory().AddObjectFunctor(CNPCMakeInstance, "CNPC");
		ActorFactory().AddActorClass("CNPC");
	}
};
static __impCNPC s_impCNPC;

// npc.cpp:27
void CNPC::OnDestroy(void) {
	Destroy();
}

// npc.cpp:32
void CNPC::Destroy(void) {
	for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
		if (*it) { delete *it; *it = 0; }
	}
	m_creatures.clear();
}

// npc.cpp:41
void CNPC::OnInit(void) {
	Destroy();

	for (int i = 0; i < GOLFDOC()->m_pPolySoup->GetAreaNum(); i++) {
		WPolySoup::sArea *area = GOLFDOC()->m_pPolySoup->GetArea(i);

		if (area->m_type == 1)
			SetNPC(area->m_aabb, area->m_name);
	}

	for (std::vector<CNodeContainer>::iterator it = GOLFDOC()->m_pPolySoup->m_boundAreas.begin(); it != GOLFDOC()->m_pPolySoup->m_boundAreas.end(); ++it) {
		if (it->GetType() == (CNodeContainer::eType)0xC)
			SetNPC(it->GetList(), it->GetName().c_str());
	}
}

// npc.cpp:76
void CNPC::OnProcess(float dt) {
	for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
		(*it)->Process(dt);
	}
}

// npc.cpp:87
void CNPC::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 392: // is there a creature of type param1? answer through param2
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			if ((*it)->m_npcType == msg.m_param1) {
				*(int *)msg.m_param2 = 1;
				break;
			}
		}
		break;

	case 386: // switch off the active creatures
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			CCreature *creature = *it;
			if ((creature->m_flags & 1) && creature->m_npcType != 9) {
				if (creature->m_active)
					creature->m_active = false;
			}
		}
		break;

	case 387: // play the event of the creatures whose area holds the ball
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			CCreature *creature = *it;
			if (creature->m_flags & 1) {
				Waabb area = creature->m_area;
				if (area.IsInclude(GolfBall().m_pos)) {
					creature->PlayEvent();
					break;
				}
			}
		}
		break;

	case 388: // param1 = whether the ball is inside the area of a creature
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			CCreature *creature = *it;
			if (creature->m_flags & 1) {
				Waabb area = creature->m_area;
				if (area.IsInclude(GolfBall().m_pos)) {
					*(int *)msg.m_param1 = 0;
					break;
				}
			}
		}
		break;

	case 390:
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			if (!((*it)->m_flags & 1))
				(*it)->Record();
		}
		break;

	case 389:
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			(*it)->Replay();
		}
		break;

	case 394:
		for (std::list<CCreature *>::iterator it = m_creatures.begin(); it != m_creatures.end(); ++it) {
			if ((*it)->m_npcType == 9)
				(*it)->PlayEvent();
		}
		break;
	}
}

// npc.cpp:189
void CNPC::SetNPC(const std::vector<WVector> &points, const char *name) {
	if (strstr(name, "santa")) {
		if (WSingleton<WNetworkSystem>::Instance()->IsConnected(WNetworkSystem::NET_UNIT_DEFAULT)) {
			CSharedDoc *shared = WSingleton<CSharedDoc>::Instance();
			if (shared->m_roomType != 0 || shared->m_gameMode != 0)
				return;
			// m_holeRandom is indexed by the 1-based hole index here
			if (shared->m_holeRandom[GetHoleIndex(GOLFDOC()->m_currentHole) - 1] % 100 >= 3)
				return;
		}

		WSliceStr slice(name);
		float radius;
		if (!slice.Get("*r", "%f", &radius))
			radius = 32.0f;

		int i;
		for (i = 0; i < 3; i++) {
			CSanta *santa = new CSanta("NPC_Mozzi.pet");
			santa->m_unkAC = radius;
			santa->SetArea(points, (float)i * -0.05f, 0);
			santa->m_npcType = 7;
			m_creatures.push_back(santa);
		}

		CSanta *santa = new CSanta("NPC_Santa.pet");
		santa->m_unkAC = radius;
		santa->SetArea(points, (float)(i + 1) * -0.05f, 0);
		santa->m_npcType = 7;
		m_creatures.push_back(santa);
		return;
	}

	CCreature *creature;
	WSliceStr slice(name);
	float value;

	if (strstr(name, "flight")) {
		creature = new CFighter("NPC_Fighter.pet");
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 8;
	} else if (strstr(name, "ufo")) {
		creature = new CUfo("sand_house12.pet");
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0xB;
	} else if (strstr(name, "penguinwalk")) {
		creature = new ntPenguin("NPC_penguin02.pet");
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0xC;
	} else if (strstr(name, "drg")) {
		if (!slice.Get("*d", "%f", &value))
			value = 0;
		creature = new ntNpc_Dragon("NPC_dragon.pet", value);
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0xF;
	} else if (strstr(name, "ship")) {
		if (!slice.Get("*d", "%f", &value))
			value = 0;
		creature = new ntNpc_PolaShip("NPC_ship.pet", value);
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0x11;
	} else if (strstr(name, "vessel")) {
		char petName[64] = "";
		int index = 2;
		value = 0;
		if (strstr(name, "vessel4"))
			index = 4;
		if (!slice.Get("*t", "%f", &value))
			value = 1.0f;
		sprintf(petName, "gs_ship_0%d.pet", index);
		creature = new CNPC_TRSHIP(petName, value);
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0x14;
	} else if (strstr(name, "walex")) {
		char petName[64] = "";
		int index = 1;
		value = 0;
		if (strstr(name, "walex03"))
			index = 3;
		if (!slice.Get("*vel", "%f", &value))
			value = 1.0f;
		sprintf(petName, "npc_alex0%d.pet", index);
		creature = new CNPC_TORTOISE(petName, value);
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0x17;
	} else if (strstr(name, "herald")) {
		if (!slice.Get("*vel", "%f", &value))
			value = 1.0f;
		if (strstr(name, "herald01"))
			creature = new CNPC_TORTOISE("NPC_Herald01.pet", value);
		else
			creature = new CNPC_TORTOISE("NPC_Herald03.pet", value);
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0x17;
	} else if (strstr(name, "crn") || strstr(name, "gns") || strstr(name, "pnd") || strstr(name, "toy")) {
		char petName[256] = "";
		char prefix[32] = "";
		bool tortoise = false;
		value = 0;

		if (!slice.Get("*v", "%f", &value))
			value = 1.0f;
		if (strstr(name, "#"))
			tortoise = true;

		if (strstr(name, "crn"))
			strcpy(prefix, "NPC_crane");
		else if (strstr(name, "gns"))
			strcpy(prefix, "NPC_ginseng");
		else if (strstr(name, "pnd"))
			strcpy(prefix, "NPC_panda");
		else if (strstr(name, "toy"))
			strcpy(prefix, "NPC_toy");

		std::string full(name);
		std::string suffix("");
		std::string::size_type pos = full.find(strstr(name, "*") ? '*' : '#');
		if (pos == 0)
			pos = full.length();
		suffix = full.substr(7, pos - 7);
		sprintf(petName, "%s%s.pet", prefix, suffix.c_str());

		if (tortoise) {
			creature = new CNPC_TORTOISE(petName, value);
			creature->SetArea(points, 0, 0);
			creature->m_npcType = 0x17;
		} else {
			creature = new CNPC_TRSHIP(petName, value);
			creature->SetArea(points, 0, 0);
			creature->m_npcType = 0x14;
		}
	} else if (strstr(name, "npc_t")) {
		const char *petName = "";
		value = 1.0f;
		slice.Get("*v", "%f", &value);

		if (strstr(name, "npc_t0"))
			petName = "npc_insam_ani_N.pet";
		else if (strstr(name, "npc_t1"))
			petName = "npc_cat_ani02_N.pet";

		ntNpc_WizCityNode *node = new ntNpc_WizCityNode(petName, value);
		value = 1.0f;
		slice.Get("*s", "%f", &value);
		node->SetScale(value);
		creature = node;
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 0x20;
	} else {
		const char *suffix = strstr(name, "npc_");
		if (!suffix)
			return;
		creature = new CDisposable(MakeStr("%s.pet", suffix + 4));
		creature->SetArea(points, 0, 0);
		creature->m_npcType = 9;
	}

	m_creatures.push_back(creature);
}

// npc.cpp:731 (the end of SetNPC(const Waabb &, char *), shared by its cases)
void CNPC::AddCreature(CCreature *creature, const Waabb &area, unsigned long flags, int type) {
	creature->SetArea(area, flags);
	creature->m_npcType = type;
	m_creatures.push_back(creature);
}

// npc.cpp:449
void CNPC::SetNPC(const Waabb &area, char *name) {
	WSliceStr slice(name);
	int type = -1;
	char petName[64];
	int num;
	float vel;
	float scale;

	petName[0] = 0;
	slice.Get("*type", "%d", &type);
	slice.Get("*pet", "%s", petName);

	switch (type) {
	case 0:
		if (slice.Get("*num", "%d", &num)) {
			for (int i = 0; i < num; i++)
				AddCreature(new CGull(petName), area, 0, type);
		} else {
			AddCreature(new CGull(petName), area, 0, type);
		}
		break;
	case 1:
		if (slice.Get("*num", "%d", &num)) {
			for (int i = 0; i < num; i++)
				AddCreature(new CButterfly(petName), area, 4, type);
		} else {
			AddCreature(new CButterfly(petName), area, 4, type);
		}
		break;
	case 2:
		if (slice.Get("*num", "%d", &num))
			AddCreature(new CDuck(petName, num + 1), area, 1, type);
		else
			AddCreature(new CDuck(petName, 0), area, 1, type);
		break;
	case 3:
		AddCreature(new CMole(petName), area, 0, type);
		break;
	case 4:
		AddCreature(new CDolphin(petName), area, 0, type);
		break;
	case 5:
		AddCreature(new CCrab(petName), area, 0, type);
		break;
	case 6:
		// the original dereferences a null creature without "*num"
		if (slice.Get("*num", "%d", &num))
			AddCreature(new CSparrow(petName, num), area, 0, type);
		break;
	case 8:
		AddCreature(new CFighter(petName), area, 0, type);
		break;
	case 9:
		AddCreature(new CDisposable(petName), area, 0, type);
		break;
	case 13:
		if (slice.Get("*num", "%d", &num)) {
			for (int i = 0; i < num; i++)
				AddCreature(new ntPenguin_Sailor(petName), area, 0, type);
		} else {
			AddCreature(new ntPenguin_Sailor(petName), area, 0, type);
		}
		break;
	case 14:
		if (slice.Get("*num", "%d", &num))
			AddCreature(new ntPenguin_Duck(petName, num + 1), area, 1, type);
		else
			AddCreature(new ntPenguin_Duck(petName, 0), area, 1, type);
		break;
	case 16:
		if (slice.Get("*num", "%d", &num)) {
			for (int i = 0; i < num; i++)
				AddCreature(new ntNpc_BlackSparrow("NPC_BTsparrow.pet"), area, 0, type);
		} else {
			AddCreature(new ntNpc_BlackSparrow("NPC_BTsparrow.pet"), area, 0, type);
		}
		break;
	case 18:
		vel = 2.0f;
		slice.Get("*vel", "%f", &vel);
		num = 1;
		slice.Get("*num", "%d", &num);
		for (int i = 0; i < num; i++)
			AddCreature(new ntNpc_PolaPenguin1(petName, vel), area, 0, type);
		break;
	case 19:
		num = 1;
		slice.Get("*num", "%d", &num);
		for (int i = 0; i < num; i++)
			AddCreature(new ntNpc_PolaPenguin2(petName), area, 0, type);
		break;
	case 21:
		num = 1;
		slice.Get("*num", "%d", &num);
		vel = 1.0f;
		slice.Get("*vel", "%f", &vel);
		for (int i = 0; i < num; i++)
			AddCreature(new CNPC_BIRD(petName, vel), area, 0, type);
		break;
	case 22:
		if (!slice.Get("num", "%d", &num))
			num = 1;
		for (int i = 0; i < num; i++)
			AddCreature(new CNPC_PARROT(petName), area, 0, type);
		break;
	case 31:
		num = 1;
		slice.Get("*num", "%d", &num);
		scale = 1.0f;
		slice.Get("*s", "%f", &scale);
		for (int i = 0; i < num; i++) {
			ntNpc_WizCityArea *creature = new ntNpc_WizCityArea(petName);
			creature->SetScale(scale);
			AddCreature(creature, area, 0, type);
		}
		break;
	default:
		break;
	}
}

// npc.h:129
CCreature::CCreature(void) {
	m_flags = 0;
	m_area.min.x = m_area.min.y = m_area.min.z = 0.0f;
	m_area.max.x = m_area.max.y = m_area.max.z = 0.0f;
	m_pPet = NULL;
	m_motionTime = 0.0f;
	m_pMotion = NULL;
	m_alpha = 0;
	m_unkAC = 0.0f;
	m_nodes.clear();
	m_recordedPath.clear();
	m_npcType = 0;
}

// npc.cpp:742
CCreature::~CCreature(void) {
	if (g_resrcmng) {
		if (m_addTex) {
			g_resrcmng->Release(m_addTex);
			m_addTex = 0;
		}
	}
	if (g_resrcmng && m_pPet) {
		g_resrcmng->Release(m_pPet, false);
		m_pPet = 0;
	}
	if (m_pBuffer) {
		delete[] m_pBuffer;
		m_pBuffer = 0;
	}

	m_nodes.clear();
	m_recordedPath.clear();
	m_alphaList.clear();
}

// npc.cpp:844
void CCreature::SetArea(const Waabb &area, unsigned long flags) {
	m_area = area;

	if (m_flags & 1)
		return;

	m_matrix = RotMat(Random(-180.0f, 180.0f, -1) * data_9f0fc0, 1);

	float *pivot = &m_matrix.pivot.x;
	const float *lo = &m_area.min.x;
	const float *hi = &m_area.max.x;
	for (int i = 0; i < 3; i++) {
		float r = Random(0, 1.0f, -1);
		pivot[i] = (1.0f - r) * hi[i] + r * lo[i];
	}

	if (flags & 1) {
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, false, 0, true);
	}
	if (flags & 2) {
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, false, 0, false);
	}

	m_pos = m_matrix.pivot;

	float cellsX = (float)ceil((m_area.max.x - m_area.min.x) / m_unk84);
	float cellsY;
	if (flags & 4)
		cellsY = (float)ceil((m_area.max.y - m_area.min.y) / m_unk84);
	else
		cellsY = 1.0f;
	float cellsZ = (float)ceil((m_area.max.z - m_area.min.z) / m_unk84);

	m_gridX = (int)cellsX;
	m_gridY = (int)cellsY;
	m_gridZ = (int)cellsZ;

	m_gridCount = m_gridY > 1 ? 4 : 2;
	m_cellX = (m_area.max.x - m_area.min.x) / cellsX;
	m_cellY = (m_area.max.y - m_area.min.y) / cellsY;
	m_cellZ = (m_area.max.z - m_area.min.z) / cellsZ;
	m_pBuffer = (unsigned char *)new int[m_gridCount];

	int index = ((int)((m_pos.y - m_area.min.y) / m_cellY) * m_gridZ
		+ (int)((m_pos.z - m_area.min.z) / m_cellZ)) * m_gridX
		+ (int)((m_pos.x - m_area.min.x) / m_cellX);
	for (int i = 0; i < m_gridCount; i++) {
		((int *)m_pBuffer)[i] = index;
	}
}

// npc.cpp:894
void CCreature::SetArea(const std::vector<WVector> &points, float radius, unsigned long flags) {
	m_nodes.clear();

	m_nodes.reserve(points.size());

	m_nodes.assign(points.begin(), points.end());

	m_matrix.za = (m_nodes[1] - m_nodes[0]).Normalize() * -1.0f;
	m_matrix.ya = WVector::UNIT_POS_Y;
	m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);

	m_matrix.pivot = points[0];

	m_pos = m_nodes[1];

	m_motionTime = radius;
}

// npc.cpp:968
void CCreature::Display(void) {
	if (!WSingleton<COption>::Instance()->vIsNpcEnabled() && GOLFDOC()->m_currentHole != 0x13) {
		return;
	}

	if (m_pPet && m_active && !(m_motionTime < 0.0f)) {
		if (m_flags & 1) {
			m_pPet->SetAlpha(m_alpha);
		}

		m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), Abs(m_motionParam) > 0.0f ? RotMat(m_motionParam, 2) * m_matrix : m_matrix);
		m_pPet->Transform(g_view, 1.0f, 0);
		m_pPet->UpdateBSphere();

		if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
			m_pPet->UpdateLightSource(&g_lightset, false, 0);
			WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, true, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), false, false);
		}
	}
}

// npc.cpp:753
void CCreature::Init(const char *name, unsigned long flags) {
	m_addTex = 0;
	m_pPet = g_resrcmng->GetPuppet(name, false, true, false);
	if (!m_pPet) {
		return;
	}

	CSceneManager::Instance()->RegisterElement((eObjType)4, m_pPet, this, name, flags, 0);

	m_motionParam = 0;
	m_recorded = false;
	m_alpha = 0xFF;
	m_replayIndex = 0;
	m_rollRate = 0.7853982f;
	SetMotion("NPC_Default", -1.0f);

	if (m_flags & 1) {
		m_active = false;
		m_alphaList.clear();

		int first, last;
		if (m_pPet->CheckFrameData((int)floor(m_pMotion->s * 30.0f), (int)ceil(m_pMotion->e * 30.0f), first, last)) {
			for (int i = first; i <= last; ++i) {
				sAlpha alpha;
				if (sscanf(m_pPet->m_frame[i].data, "*opa %f", &alpha.m_value)) {
					alpha.m_value *= 2.55f;
					alpha.m_time = m_pPet->m_frame[i].num * (1.0f / 30.0f);
					m_alphaList.push_back(alpha);
				}
			}
		}
	} else {
		m_active = true;
		m_turnRate = 1.0509f;
	}

	for (w_bound_box *box = m_pPet->m_bbList.Start(); box; box = m_pPet->m_bbList.Next()) {
		const char *option = box->info->option;
		if (!option) {
			continue;
		}
		const char *fx = strstr(option, "*fx");
		if (!fx) {
			continue;
		}

		char sequence[32] = "";
		char file[32] = "";
		char unused[32] = "";
		sscanf(fx, "%s %s %s", sequence, file, unused);

		if (!box->bone || box->bone->IsInvisible()) {
			continue;
		}
		char *dot = strrchr(file, '.');
		if (!dot) {
			continue;
		}
		const char *ext = dot + 1;
		if (strlen(ext) < 3) {
			continue;
		}
		if (stricmp(ext, "spr") && stricmp(ext, "seq")) {
			continue;
		}

		Attacher attacher;
		attacher.m_pMatrix[0] = &box->bone->GetMatrix();
		attacher.m_pOffset[0] = &box->info->spherePivot;
		WSingleton<CFx>::Instance()->Attach(attacher, file, false, 0);
	}
}

const char *GetStrConst(const char *pSrc, char *pDest);

// Nameless data: the token ParsePuppetScript cuts out of a script.
static char data_af3540[256];

// npc.cpp:995
void CCreature::DrawDebugLine(void) {
	g_view->DrawSphere(m_pPet->GetBoundSphere(), 0xFF00FF00, 20, 0);

	if (!(m_flags & 1)) {
		g_view->DrawLine(m_matrix.pivot, 0xFFFFFF00, m_matrix.pivot - m_matrix.za * 5.0f, 0xFFFFFF00, 0);
		g_view->DrawLine(m_matrix.pivot, 0xFFFF0000, m_pos, 0xFFFF0000, 0);
	}
}

// npc.cpp:1006
void CCreature::SetMotion(const char *name, float time) {
	if (m_pPet) {
		const w_motion_data *motion = m_pPet->FindMotionData(name);

		if (motion) {
			m_pMotion = motion;
			if (time >= 0.0f) {
				time += motion->s;
				m_motionTime = time > motion->e ? motion->e : time;
			} else {
				m_motionTime = Random(motion->s, motion->e, -1);
			}
		} else {
			m_motionTime = 0;
		}
	}
}

// Minimum helper of an unidentified header (also local in camerawork.cpp).
template <class T> T MinOf(T a, T b) { return a > b ? b : a; }

// npc.cpp:1028
void CCreature::ScriptProcess(const w_motion_data *motion, float from, float to) {
	if (motion == 0)
		return;

	if (!(from <= motion->e) || from == to)
		return;

	float origTo = to;
	from *= 30.0f;
	to = MinOf(motion->e, to) * 30.0f;
	int first = Ceil(from, 0.03f);
	int last = to == (float)floor(to) ? float2int(to) - 1 : float2int((float)floor(to));

	if (first <= last) {
		int start, end;
		if (m_pPet->CheckFrameData(first, last, start, end)) {
			for (int i = start; i <= end; i++)
				ParsePuppetScript(m_pPet->GetFrameData(i));
		}
	}

	// The motion looped: process the part past its end from its start.
	if (origTo > motion->e) {
		from = motion->s * 30.0f;
		to = MinOf(origTo - motion->e, motion->e) * 30.0f;
		first = Ceil(from, 0.03f);
		last = to == (float)floor(to) ? float2int(to) - 1 : float2int((float)floor(to));

		if (first <= last) {
			int start, end;
			if (m_pPet->CheckFrameData(first, last, start, end)) {
				for (int i = start; i <= end; i++)
					ParsePuppetScript(m_pPet->GetFrameData(i));
			}
		}
	}
}

// npc.cpp:1076
void CCreature::ParsePuppetScript(const char *script) {
	char token[0x40];
	char name[0x40];
	char boneName[0x40];

	if (script) {
		const char *p = strchr(script, '*');

		while (p) {
			int len = 0;

			while (p[len]) {
				if (p[++len] == ' ')
					break;
				if (p[len] == '(') {
					len++;
					break;
				}
			}

			if (len <= 1)
				break;

			strncpy(data_af3540, p, len);
			data_af3540[len] = 0;
			p += len;

			if (strcmpi(data_af3540, "*fx(") == 0) {
				token[0] = 0;
				p = GetStrConst(p, token);

				if (strlen(token) == 0 || strlen(token) >= sizeof(token))
					return;

				if (token[0] == '@') {
					if (m_pMotion == NULL)
						return;
					if (strcmp(&token[1], m_pMotion->name) != 0)
						return;

					p = GetStrConst(p, name);
					if (strlen(name) == 0 || strlen(name) >= sizeof(name))
						return;
				} else {
					strcpy(name, token);
				}

				Attacher attacher;

				int count = 0;
				do {
					p = GetStrConst(p, boneName);

					if (strlen(boneName) == 0 || strlen(boneName) >= sizeof(boneName))
						break;

					WBone *bone = m_pPet->FindBone(boneName);
					if (bone) {
						attacher.m_pMatrix[count] = &bone->GetMatrix();
					} else {
						w_bound_box *box = m_pPet->m_bbList.Find(boneName);
						if (box) {
							if (box->bone)
								attacher.m_pMatrix[count] = &box->bone->GetMatrix();
							attacher.m_pOffset[count] = &box->info->spherePivot;
						}
					}
				} while (++count < 4);

				if (count > 0)
					WSingleton<CFx>::Instance()->Attach(attacher, name, true, 0);
			}

			if (p == NULL)
				break;
		}
	}
}

// npc.cpp:1185
void CCreature::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			int tries = 0;
			int nx, ny, nz;
			int index;
			int i;

			do {
				tries++;

				int rnd[3];
				for (i = 0; i < 3; i++) {
					rnd[i] = rand() % 3 - 1;
				}

				int cur = ((int *)m_pBuffer)[0];
				int x = cur % m_gridX;
				int y = cur / (m_gridZ * m_gridX);
				int z = cur % (m_gridZ * m_gridX) / m_gridX;

				nx = Between(0, rnd[0] + x, m_gridX - 1);
				ny = Between(0, rnd[1] + y, m_gridY - 1);
				nz = Between(0, rnd[2] + z, m_gridZ - 1);
				index = (m_gridZ * ny + nz) * m_gridX + nx;

				for (i = 0; i < m_gridCount; i++) {
					if (((int *)m_pBuffer)[i] == index)
						break;
				}
			} while (i < m_gridCount && tries < 5);

			for (i = m_gridCount - 1; i >= 1; i--) {
				((int *)m_pBuffer)[i] = ((int *)m_pBuffer)[i - 1];
			}
			((int *)m_pBuffer)[0] = index;

			m_pos.x = Random(0.0f, m_cellX, -1) + nx * m_cellX + m_area.min.x;
			if (m_gridY > 1)
				m_pos.y = Random(0.0f, m_cellY, -1) + ny * m_cellY + m_area.min.y;
			m_pos.z = Random(0.0f, m_cellZ, -1) + nz * m_cellZ + m_area.min.z;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			m_pos = m_recordedPath[m_replayIndex];
			m_replayIndex++;
		}
	}

	if (m_gridY > 1) {
		m_matrix.pivot += (m_pos - m_matrix.pivot).Normalize() * m_unk74 * dt;
	} else {
		float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

		if (Abs(angle) > data_9f0fc0) {
			if (Abs(angle) > m_rollRate) {
				if (Abs(m_motionParam) < m_unk7C)
					m_motionParam -= angle * dt;
			} else if (Abs(m_motionParam) > Abs(angle * dt)) {
				m_motionParam += (angle > 0 ? m_rollRate : -m_rollRate) * dt;
			} else {
				m_motionParam = 0;
			}

			float limit = dt * m_turnRate;
			angle = Between(-limit, angle, limit);
			m_matrix = RotMat(angle, 1) * m_matrix;
		} else if (Abs(m_motionParam) > 0) {
			if (Abs(m_motionParam) > dt * m_rollRate)
				m_motionParam += (m_motionParam > 0 ? -m_rollRate : m_rollRate) * dt;
			else
				m_motionParam = 0;
		}

		m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	}
}

// npc.cpp:1292
void CCreature::PlayEvent(void) {
	m_motionTime = 0;
	m_active = true;
	m_alpha = 0xFF;
	m_matrix = RotMat(Random(-3.1415927f, 3.1415927f, -1), 1);
	m_matrix.pivot = GetPVS().GetGroundPoint(GolfBall().m_pos + m_unkAC * m_matrix.za, false, 0, true);
}

// npc.cpp:918
void CCreature::Process(float dt) {
	if (m_pPet && m_active) {
		float t = dt + m_motionTime;
		m_motionTime = t;

		if (t > 0) {
			ScriptProcess(m_pMotion, t - dt, t);

			if (!(m_motionTime <= m_pMotion->e)) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e;
				}
			}

			if (m_flags & 1) {
				std::list<sAlpha>::iterator end = m_alphaList.end();
				std::list<sAlpha>::iterator it = m_alphaList.begin();

				while (it != end) {
					std::list<sAlpha>::iterator cur = it;
					++it;

					if (it == end) {
						m_alpha = (unsigned char)(int)cur->m_value;
						break;
					}

					if (m_motionTime <= cur->m_time || !(m_motionTime < it->m_time))
						continue;

					float k = (m_motionTime - cur->m_time) / (it->m_time - cur->m_time);
					m_alpha = (unsigned char)(int)((1.0f - k) * cur->m_value + k * it->m_value);
					break;
				}
			} else {
				CheckTarget(dt);
			}
		}
	}
}

// npc.cpp:1304
void CCreature::Record(void) {
	m_recordedPath.clear();
	m_recordedPath.reserve(10);
	m_recordedPath.push_back(m_pos);
	m_recorded = true;
	m_recordedMotionTime = m_motionTime;
	m_recordedMatrix = m_matrix;
	m_recordedMotionParam = m_motionParam;
	m_recordedActive = m_active;
}

// npc.cpp:1322
void CCreature::Replay(void) {
	if (m_flags & 1) {
		m_active = false;
		return;
	}
	m_pos = m_recordedPath[0];
	m_replayIndex = 1;
	m_motionTime = m_recordedMotionTime;
	m_recorded = false;
	m_matrix = m_recordedMatrix;
	m_motionParam = m_recordedMotionParam;
	m_active = m_recordedActive;
}

// npc.cpp:1346
CGull::CGull(const char *name) {
	m_unk74 = 10.0f;
	Init(name, 0);
	m_unk7C = 0.785398f;
	m_unk84 = 48.0f;
}

// npc.cpp:1354
CGull::~CGull(void) {
}

// npc.cpp:1362
CButterfly::CButterfly(const char *name) {
	m_unk74 = 7.0f;
	Init(name, 0);
	m_unk7C = 0;
	m_unk84 = 16.0f;
}

// npc.cpp:1370
CButterfly::~CButterfly(void) {
}

// npc.cpp:1376
CDuck::CDuck(const char *name, int count) : m_pMatrices(0) {
	m_unk74 = 2.0f;
	Init(name, 0);
	m_turnRate = 0.5235988f;
	m_unk7C = 0;
	m_unk84 = 32.0f;
	m_count = count;
	if (count > 0) {
		m_pMatrices = new WMatrix[count];
	}
}

// npc.cpp:1392
void CDuck::SetArea(const Waabb &area, unsigned long flags) {
	CCreature::SetArea(area, flags);

	if (m_count == 0)
		return;

	m_pMatrices[0] = m_matrix;

	for (int i = 1; i < m_count; i++) {
		m_pMatrices[i] = m_pMatrices[i - 1];
		m_pMatrices[i].pivot = m_pMatrices[i - 1].pivot + m_pMatrices[i - 1].za * 6.4f;
	}
}

// npc.cpp:1408
void CDuck::Process(float dt) {
	CheckTarget(dt);

	if (m_count == 0)
		return;

	m_pMatrices[0] = m_matrix;

	for (int i = 1; i < m_count; i++) {
		WVector toPrev = m_pMatrices[i - 1].pivot - m_pMatrices[i].pivot;

		if (toPrev.Magnitude() >= 3.2f) {
			float maxTurn = dt * m_turnRate * 0.7f;
			float minTurn = dt * m_turnRate * -0.7f;
			float angle;
			float delta = CalcDeltaAngle(m_pMatrices[i - 1].pivot - m_pMatrices[i].pivot, m_pMatrices[i].za * -1.0f);

			if (delta < minTurn)
				angle = minTurn;
			else if (delta > maxTurn)
				angle = maxTurn;
			else
				angle = delta;

			m_pMatrices[i] = RotMat(angle, 1) * m_pMatrices[i];
			m_pMatrices[i].pivot += m_pMatrices[i].za * -1.0f * m_unk74 * dt;
		}
	}
}

// npc.cpp:1437
void CDuck::Display(void) {
	if (m_count == 0) {
		CCreature::Display();
		return;
	}

	for (int i = 0; i < m_count; i++) {
		m_pPet->ApplyBones(0, m_pMatrices[i] * (i == 0 ? 1.0f : 0.5f));
		m_pPet->Transform(g_view, 1.0f, 0);

		m_pPet->UpdateBSphere();

		if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
			m_pPet->UpdateLightSource(&g_lightset, false, 0);
			m_pPet->Render(g_view, false, 1.0f, g_view->InFrustumSafe(m_pPet->GetBoundBox()), true, false);
		}
	}
}

// npc.cpp:1465
CMole::CMole(const char *name) {
	m_flags |= 1;
	Init(name, 0);
	m_unkAC = 6.4f;
}

// npc.cpp:1472
void CMole::PlayEvent(void) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel < 1) {
		return;
	}
	CCreature::PlayEvent();

	if (GetPVS().GetGroundType(m_matrix.pivot, true, 0) == 3) {
		return;
	}
	m_matrix.za = GolfBall().m_pos - (m_area.max + m_area.min) * 0.5f;

	m_matrix.za.y = 0; m_matrix.za.Normalize();

	m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);
	m_matrix.xa.Normalize();

	m_matrix.pivot = GetPVS().GetGroundPoint(m_unkAC * m_matrix.za + GolfBall().m_pos, false, 0, true);
}

// npc.cpp:1497
CDolphin::CDolphin(const char *name) {
	m_flags |= 1;
	Init(name, 0);
	m_unkAC = 0;
}

// npc.cpp:1504
void CDolphin::PlayEvent(void) {
	if (GolfBall().m_landed && GolfBall().m_lie == 1) {
		CCreature::PlayEvent();
	}
}

// npc.cpp:1513
CCrab::CCrab(const char *name) {
	m_flags |= 1;
	Init(name, 0);
	m_unkAC = -3.2f;
}

// npc.cpp:1520
void CCrab::PlayEvent(void) {
	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel >= 1) {
		CCreature::PlayEvent();
	}
}

// npc.cpp:1529
CSparrow::CSparrow(const char *name, int count) : m_pMatrices(0) {
	m_pPet = g_resrcmng->GetPuppet(name, false, true, false);
	m_motionParam = 0;
	m_recorded = false;
	m_alpha = 0xFF;
	m_active = false;
	m_maxCount = count;
	m_pMatrices = new WMatrix[count];
}

// npc.cpp:1542
void CSparrow::PlayEvent(void) {
	m_active = true;

	float speed = GolfBall().m_velocity.Magnitude();

	m_count = (int)(m_maxCount / GolfBall().PreData(0)->m_velocity.Magnitude() * speed);
	for (int i = 0; i < m_count; i++) {
		m_pMatrices[i] = RotMat(Random(-180.0f, 180.0f, -1) * data_9f0fc0, 1);
		m_pMatrices[i].pivot = (m_area.max + m_area.min) * 0.5f;
	}
}

// npc.cpp:1556
void CSparrow::Display(void) {
	if (m_pPet && m_active) {
		for (int i = 0; i < m_count; i++) {
			m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), m_pMatrices[i]);
			m_pPet->Transform(g_view, 1.0f, 0);

			m_pPet->UpdateBSphere();

			if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
				m_pPet->UpdateLightSource(&g_lightset, false, 0);
				m_pPet->Render(g_view, false, 1.0f, g_view->InFrustumSafe(m_pPet->GetBoundBox()), true, false);
			}
		}
	}
}

// npc.cpp:1580
CSanta::CSanta(const char *name) {
	Init(name, 0);
	m_nodeIndex = 1;
	m_unk74 = 120.0f;
	m_unk7C = 0.785398f;
	m_turnRate = 2.0943952f;
	m_climbRate = 0;
	m_rollRate = 1.5707964f;
	m_state = 2;
}

// npc.cpp:1593
CSanta::~CSanta(void) {
}

// npc.cpp:1597
void CSanta::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				m_motionTime -= m_pMotion->e;
				if (rand() % 10 == 0 && strcmpi(m_pMotion->name, "NPC_Default") == 0)
					SetMotion("NPC_Default1", 0);
				else
					SetMotion("NPC_Default", 0);
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:1623
void CSanta::CheckTarget(float dt) {
	WVector d = m_pos - m_matrix.pivot;
	d.y = 0.0f;

	if (d.Magnitude() < (dt < 0.1f ? 0.1f : dt) * m_unk74) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			if (m_nodeIndex == m_nodes.size() - 1) {
				m_active = false;
				return;
			}

			unsigned char prev = m_nodeIndex;
			m_nodeIndex = (m_nodeIndex + 1) % m_nodes.size();
			m_pos = m_nodes[m_nodeIndex];
			m_pos += (RotMat(Random(-1.5707964f, 1.5707964f, -1), 0).za * m_unkAC) * RotMat(Random(-3.1415927f, 3.1415927f, -1), 1);

			d = m_pos - m_matrix.pivot;
			d.y = 0.0f;
			WVector seg;
			seg.x = m_nodes[m_nodeIndex].x - m_nodes[prev].x;
			seg.y = 0.0f;
			seg.z = m_nodes[m_nodeIndex].z - m_nodes[prev].z;

			float dist = d.Magnitude();
			m_unk74 = dist * 120.0f / seg.Magnitude();
			m_climbRate = (m_pos.y - m_matrix.pivot.y) / dist * m_unk74;

			float turn = CalcDeltaAngle(d, m_matrix.za * -1.0f);
			m_unk7C = Abs(turn) > 1.5707964f ? 1.5707964f : Abs(turn);
			m_state = 1;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			if (m_replayIndex < m_recordedPath.size()) {
				m_pos = m_recordedPath[m_replayIndex];
				m_replayIndex++;
			} else {
				m_active = false;
			}
		}
	}

	float angle = CalcDeltaAngle(d, m_matrix.za * -1.0f);

	if (Abs(angle) > data_9f0fc0) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	switch (m_state) {
	case 0:
		if (Abs(m_motionParam) > dt * m_rollRate) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_motionParam = 0;
			m_state = 2;
		}
		break;
	case 1:
		if (Abs(m_motionParam) < m_unk7C) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_state = 0;
		}
		break;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot.y += dt * m_climbRate;

	float dist = (g_camera.pivot - m_matrix.pivot).Magnitude();

	if (dist < 640.0f) {
		if (!g_audio->IsPlaying("Santa"))
			g_audio->PlaySfx("santaboost", 0, 1, "Santa", 0, 0.5f, 200.0f);
		g_audio->SetSFXVolumeWhilePlaying("Santa", (640.0f - dist) * 0.0015625f);
	} else {
		g_audio->StopSfx("Santa");
	}
}

// npc.cpp:1724
CFighter::CFighter(const char *name) {
	Init(name, 0);
	m_nodeIndex = 1;
	m_unk74 = 320.0f;
	m_unk7C = 0.785398f;
	m_turnRate = 2.0943952f;
	m_climbRate = 0;
	m_rollRate = 1.5707964f;
	m_state = 2;
}

// npc.cpp:1737
CFighter::~CFighter(void) {
}

// npc.cpp:1741
void CFighter::CheckTarget(float dt) {
	WVector d = m_pos - m_matrix.pivot;
	d.y = 0.0f;

	if (d.Magnitude() < (dt < 0.1f ? 0.1f : dt) * m_unk74) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			if (m_nodeIndex == m_nodes.size() - 1) {
				m_active = false;
				return;
			}

			m_nodeIndex = (m_nodeIndex + 1) % m_nodes.size();
			m_pos = m_nodes[m_nodeIndex];

			d = m_pos - m_matrix.pivot;
			d.y = 0.0f;

			m_unk74 = d.Magnitude();
			m_climbRate = (m_pos.y - m_matrix.pivot.y) / m_unk74 * m_unk74;

			float turn = CalcDeltaAngle(d, m_matrix.za * -1.0f);
			m_unk7C = Abs(turn) > 1.5707964f ? 1.5707964f : Abs(turn);
			m_state = 1;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			if (m_replayIndex < m_recordedPath.size()) {
				m_pos = m_recordedPath[m_replayIndex];
				m_replayIndex++;
			} else {
				m_active = false;
			}
		}
	}

	float angle = CalcDeltaAngle(d, m_matrix.za * -1.0f);

	if (Abs(angle) > data_9f0fc0) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	switch (m_state) {
	case 0:
		if (Abs(m_motionParam) > dt * m_rollRate) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_motionParam = 0;
			m_state = 2;
		}
		break;
	case 1:
		if (Abs(m_motionParam) < m_unk7C) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_state = 0;
		}
		break;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot.y += dt * m_climbRate;
}

// npc.cpp:1821
CUfo::CUfo(const char *name) {
	Init(name, 0);
	m_nodeIndex = 1;
	m_unk74 = 2.857143f;
	m_unk7C = 0.785398f;
	m_turnRate = 2.0943952f;
	m_climbRate = 0;
	m_rollRate = 1.5707964f;
	m_state = 2;
}

// npc.cpp:1834
CUfo::~CUfo(void) {
}

// npc.cpp:1838
void CUfo::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				m_motionTime -= m_pMotion->e;
				SetMotion("NPC_Default", 0);
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:1861
void CUfo::CheckTarget(float dt) {
	WVector d = m_pos - m_matrix.pivot;
	d.y = 0.0f;

	if (d.Magnitude() < (dt < 0.1f ? 0.1f : dt) * m_unk74) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			if (m_nodeIndex == m_nodes.size() - 1) {
				m_active = false;
				return;
			}

			unsigned char prev = m_nodeIndex;
			m_nodeIndex = (m_nodeIndex + 1) % m_nodes.size();
			m_pos = m_nodes[m_nodeIndex];
			m_pos += (m_unkAC * RotMat(Random(-1.5707964f, 1.5707964f, -1), 0).za) * RotMat(Random(-3.1415927f, 3.1415927f, -1), 1);

			d = m_pos - m_matrix.pivot;
			d.y = 0.0f;

			WVector e = m_nodes[m_nodeIndex] - m_nodes[prev];
			e.y = 0.0f;

			float len = d.Magnitude();
			m_unk74 = len * 120.0f / e.Magnitude();
			m_climbRate = (m_pos.y - m_matrix.pivot.y) / len * m_unk74;

			float turn = CalcDeltaAngle(d, m_matrix.za * -1.0f);
			m_unk7C = Abs(turn) > 1.5707964f ? 1.5707964f : Abs(turn);
			m_state = 1;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			if (m_replayIndex < m_recordedPath.size()) {
				m_pos = m_recordedPath[m_replayIndex];
				m_replayIndex++;
			} else {
				m_active = false;
			}
		}
	}

	float angle = CalcDeltaAngle(d, m_matrix.za * -1.0f);

	if (Abs(angle) > data_9f0fc0) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	switch (m_state) {
	case 0:
		if (Abs(m_motionParam) > dt * m_rollRate) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_motionParam = 0;
			m_state = 2;
		}
		break;
	case 1:
		if (Abs(m_motionParam) < m_unk7C) {
			m_motionParam += (angle > 0 ? -m_rollRate : m_rollRate) * dt;
		} else {
			m_state = 0;
		}
		break;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot.y += dt * m_climbRate;
}

// npc.cpp:1982
ntPenguin::ntPenguin(const char *name) {
	Init(name, 0);
	m_nodeIndex = 1;
	m_unk74 = 2.857143f;
	m_unk7C = 0.785398f;
	m_turnRate = 2.0943952f;
	m_climbRate = 0;
	m_rollRate = 1.5707964f;
	m_state = 2;
}

// npc.cpp:1998
ntPenguin::~ntPenguin(void) {
}

// npc.cpp:2004
void ntPenguin::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				m_motionTime -= m_pMotion->e;
				SetMotion("NPC_Default", 0);
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:2027
void ntPenguin::CheckTarget(float dt) {
	WVector d = m_pos - m_matrix.pivot;
	d.y = 0.0f;

	if (d.Magnitude() < (dt < 0.1f ? 0.1f : dt) * m_unk74) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			if (m_nodeIndex == m_nodes.size() - 1)
				m_nodeIndex = 0;

			m_nodeIndex = (m_nodeIndex + 1) % m_nodes.size();
			m_pos = m_nodes[m_nodeIndex];
			m_pos += (RotMat(Random(-1.5707964f, 1.5707964f, -1), 0).za * m_unkAC) * RotMat(Random(-3.1415927f, 3.1415927f, -1), 1);

			d = m_pos - m_matrix.pivot;
			d.y = 0.0f;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			if (m_replayIndex < m_recordedPath.size()) {
				m_pos = m_recordedPath[m_replayIndex];
				m_replayIndex++;
			} else {
				m_nodeIndex = 0;
			}
		}
	}

	float angle = CalcDeltaAngle(d, m_matrix.za * -1.0f);

	if (Abs(angle) > data_9f0fc0) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot.y += dt * m_climbRate;
	m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
}

// npc.cpp:2087
ntPenguin_Sailor::ntPenguin_Sailor(const char *name) {
	m_unk74 = 2.0f;
	Init(name, 0);
	m_unk7C = 0.785398f;
	m_unk84 = 48.0f;
	SetMotion("NPC_Act0", 0);
	m_scale = Random(0.7f, 1.0f, -1);
	m_act0Motion = true;
}

// npc.cpp:2106
ntPenguin_Sailor::~ntPenguin_Sailor(void) {
}

// npc.cpp:2112
void ntPenguin_Sailor::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;

		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);

			if (!(m_motionTime <= m_pMotion->e)) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
					switch (rand() % 10) {
					case 0:
						SetMotion("NPC_Act1", 0);
						m_act0Motion = false;
						break;
					case 1:
						SetMotion("NPC_Default", 0);
						m_act0Motion = false;
						break;
					default:
						SetMotion("NPC_Act0", 0);
						m_act0Motion = true;
						break;
					}
				}
			}

			if (m_flags & 1) {
				std::list<sAlpha>::iterator end = m_alphaList.end();
				std::list<sAlpha>::iterator it = m_alphaList.begin();

				while (it != end) {
					std::list<sAlpha>::iterator cur = it;
					++it;

					if (it == end) {
						m_alpha = (unsigned char)(int)cur->m_value;
						break;
					}

					if (m_motionTime <= cur->m_time || !(m_motionTime < it->m_time))
						continue;

					float k = (m_motionTime - cur->m_time) / (it->m_time - cur->m_time);
					m_alpha = (unsigned char)(int)((1.0f - k) * cur->m_value + k * it->m_value);
					break;
				}
			} else if (m_act0Motion) {
				CheckTarget(dt);
			}
		}
	}
}

// npc.cpp:2189
void ntPenguin_Sailor::Display(void) {
	if (!WSingleton<COption>::Instance()->vIsNpcEnabled() && GOLFDOC()->m_currentHole != 0x13) {
		return;
	}

	if (m_pPet && m_active && !(m_motionTime < 0.0f)) {
		if (m_flags & 1) {
			m_pPet->SetAlpha(m_alpha);
		}

		m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), (Abs(m_motionParam) > 0.0f ? RotMat(m_motionParam, 2) * m_matrix : m_matrix) * m_scale);
		m_pPet->Transform(g_view, 1.0f, 0);
		m_pPet->UpdateBSphere();

		if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
			m_pPet->UpdateLightSource(&g_lightset, false, 0);
			WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, true, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), false, false);
		}
	}
}

// npc.cpp:2217
void ntPenguin_Sailor::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			int tries = 0;
			int nx, ny, nz;
			int index;
			int i;

			do {
				tries++;

				int rnd[3];
				for (i = 0; i < 3; i++) {
					rnd[i] = rand() % 3 - 1;
				}

				int cur = ((int *)m_pBuffer)[0];
				int x = cur % m_gridX;
				int y = cur / (m_gridZ * m_gridX);
				int z = cur % (m_gridZ * m_gridX) / m_gridX;

				nx = Between(0, rnd[0] + x, m_gridX - 1);
				ny = Between(0, rnd[1] + y, m_gridY - 1);
				nz = Between(0, rnd[2] + z, m_gridZ - 1);
				index = (m_gridZ * ny + nz) * m_gridX + nx;

				for (i = 0; i < m_gridCount; i++) {
					if (((int *)m_pBuffer)[i] == index)
						break;
				}
			} while (i < m_gridCount && tries < 5);

			for (i = m_gridCount - 1; i >= 1; i--) {
				((int *)m_pBuffer)[i] = ((int *)m_pBuffer)[i - 1];
			}
			((int *)m_pBuffer)[0] = index;

			m_pos.x = Random(0.0f, m_cellX, -1) + nx * m_cellX + m_area.min.x;
			if (m_gridY > 1)
				m_pos.y = Random(0.0f, m_cellY, -1) + ny * m_cellY + m_area.min.y;
			m_pos.z = Random(0.0f, m_cellZ, -1) + nz * m_cellZ + m_area.min.z;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			m_pos = m_recordedPath[m_replayIndex];
			m_replayIndex++;
		}
	}

	if (m_gridY > 1) {
		m_matrix.pivot += (m_pos - m_matrix.pivot).Normalize() * m_unk74 * dt;
	} else {
		float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

		if (Abs(angle) > data_9f0fc0) {
			float limit = dt * m_turnRate;
			angle = Between(-limit, angle, limit);
			m_matrix = RotMat(angle, 1) * m_matrix;
		}

		m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
	}
}

// npc.cpp:2311
ntPenguin_Duck::ntPenguin_Duck(const char *name, int count) : m_pMatrices(0) {
	m_unk74 = 2.0f;
	Init(name, 0);
	m_turnRate = 0.5235988f;
	m_unk7C = 0;
	m_unk84 = 32.0f;
	m_count = count;

	if (count > 0) {
		m_pMatrices = new WMatrix[m_count];
		m_pScales = new float[m_count];

		m_pScales[0] = 1.0f;
		for (int i = 1; i < m_count; i++)
			m_pScales[i] = Random(0.6f, 0.8f, -1);
	}

	SetMotion("NPC_Act0", 0);
	m_playingAct0 = false;
}

// npc.cpp:2341
ntPenguin_Duck::~ntPenguin_Duck(void) {
	if (m_pMatrices) { delete[] m_pMatrices; m_pMatrices = 0; }
	if (m_pScales) { delete[] m_pScales; m_pScales = 0; }
}

// npc.cpp:2348
void ntPenguin_Duck::SetArea(const Waabb &area, unsigned long flags) {
	CCreature::SetArea(area, flags);

	if (m_count == 0)
		return;

	m_pMatrices[0] = m_matrix;

	for (int i = 1; i < m_count; i++) {
		m_pMatrices[i] = m_pMatrices[i - 1];
		m_pMatrices[i].pivot = m_pMatrices[i - 1].pivot + m_pMatrices[i - 1].za * 6.4f;
	}
}

// npc.cpp:2365
void ntPenguin_Duck::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;

		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);

			if (!(m_motionTime <= m_pMotion->e)) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
					SetMotion("NPC_Act0", 0);

					if (rand() % 10 != 0) {
						SetMotion("NPC_Act0", 0);
						m_playingAct0 = true;
					} else {
						SetMotion("NPC_Act1", 0);
						m_playingAct0 = false;
					}
				}
			}

			if (m_playingAct0)
				CheckTarget(dt);

			m_pos = GetPVS().GetGroundPoint(m_pos, true, 0, false);

			if (m_count == 0)
				return;

			if (!m_playingAct0)
				return;

			m_pMatrices[0] = m_matrix;

			for (int i = 1; i < m_count; i++) {
				WVector toPrev = m_pMatrices[i - 1].pivot - m_pMatrices[i].pivot;

				if (toPrev.Magnitude() >= 4.16f) {
					float angle = Between(dt * m_turnRate * -0.7f, CalcDeltaAngle(m_pMatrices[i - 1].pivot - m_pMatrices[i].pivot, m_pMatrices[i].za * -1.0f), dt * m_turnRate * 0.7f);

					m_pMatrices[i] = RotMat(angle, 1) * m_pMatrices[i];
					m_pMatrices[i].pivot += m_pMatrices[i].za * -1.0f * m_unk74 * dt;
					m_pMatrices[i].pivot = GetPVS().GetGroundPoint(m_pMatrices[i].pivot, true, 0, false);
				}
			}
		}
	}
}

// npc.cpp:2448
void ntPenguin_Duck::Display(void) {
	if (!WSingleton<COption>::Instance()->vIsNpcEnabled() && GOLFDOC()->m_currentHole != 0x13) {
		return;
	}

	if (m_pPet && m_active && !(m_motionTime < 0.0f)) {
		if (m_count == 0) {
			CCreature::Display();
			return;
		}

		for (int i = 0; i < m_count; i++) {
			m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), m_pMatrices[i] * m_pScales[i]);
			m_pPet->Transform(g_view, 1.0f, 0);
			m_pPet->UpdateBSphere();

			if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
				m_pPet->UpdateLightSource(&g_lightset, false, 0);
				WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, false, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), true, false);
			}
		}
	}
}

// npc.cpp:2482
ntNpc_Dragon::ntNpc_Dragon(const char *name, float speed) {
	Init(name, 0);
	m_hidden = true;
	m_distance = 0;
	m_speed = speed;
}

// npc.cpp:2492
ntNpc_Dragon::~ntNpc_Dragon(void) {
}

// npc.cpp:2496
void ntNpc_Dragon::Process(float dt) {
	if (m_active && m_pPet) {
		m_motionTime += dt;

		if (m_hidden) {
			if (!(m_motionTime > m_speed)) {
				return;
			}

			m_motionTime = 0;
			m_hidden = false;
			SetMotion("NPC_Act02", -1.0f);
		}

		ScriptProcess(m_pMotion, m_motionTime - dt, m_motionTime);

		if (m_motionTime > m_pMotion->e) {
			int r = rand() % 101;

			if (r < 5)
				SetMotion("NPC_Act02", 0);
			else if (r < 10)
				SetMotion("NPC_Act01", 0);
			else
				SetMotion("NPC_Default", 0);
		}

		m_distance += dt * 100.0f;

		CheckTarget(dt);
	}
}

// npc.cpp:2534
void ntNpc_Dragon::Display(void) {
	if (m_active && m_hidden != true) {
		CCreature::Display();
	}
}

// npc.cpp:2542
void ntNpc_Dragon::CheckTarget(float dt) {
	WVector next = m_spline.Evaluate(m_distance + 0.1f);
	m_matrix.pivot = m_spline.Evaluate(m_distance);

	if (WisEqual(next, m_nodes[m_nodes.size() - 1], 1e-5f)) {
		m_distance = 0;
	}

	m_matrix = MakeCameraMatrix(next, m_matrix.pivot);
}

// npc.cpp:2553
void ntNpc_Dragon::SetArea(const std::vector<WVector> &points, float radius, unsigned long flags) {
	CCreature::SetArea(points, radius, flags);

	WVector *pPoints = (WVector *)_alloca(points.size() * sizeof(WVector));
	float *pTimes = (float *)_alloca(points.size() * sizeof(float));

	pPoints[0] = points[0];
	pTimes[0] = 0;

	for (unsigned int i = 1; i < points.size(); i++) {
		pPoints[i] = points[i];
		pTimes[i] = pTimes[i - 1] + (points[i] - points[i - 1]).Magnitude();
	}

	m_spline.Init(pPoints, pTimes, points.size());
}

// npc.cpp:2575
ntNpc_BlackSparrow::ntNpc_BlackSparrow(const char *name) {
	m_unk74 = 5.5f;
	Init(name, 0);
	m_unk7C = 0.785398f;
	m_unk84 = 48.0f;
	SetMotion("NPC_Default", 0);
	m_defaultMotion = true;
}

// npc.cpp:2591
ntNpc_BlackSparrow::~ntNpc_BlackSparrow(void) {
}

// npc.cpp:2596
void ntNpc_BlackSparrow::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;

		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);

			if (!(m_motionTime <= m_pMotion->e)) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
					if (rand() % 101 < 20) {
						SetMotion("NPC_Act01", 0);
						m_defaultMotion = false;
					} else {
						SetMotion("NPC_Default", 0);
						m_defaultMotion = true;
					}
				}
			}

			if (m_flags & 1) {
				std::list<sAlpha>::iterator end = m_alphaList.end();
				std::list<sAlpha>::iterator it = m_alphaList.begin();

				while (it != end) {
					std::list<sAlpha>::iterator cur = it;
					++it;

					if (it == end) {
						m_alpha = (unsigned char)(int)cur->m_value;
						break;
					}

					if (m_motionTime <= cur->m_time || !(m_motionTime < it->m_time))
						continue;

					float k = (m_motionTime - cur->m_time) / (it->m_time - cur->m_time);
					m_alpha = (unsigned char)(int)((1.0f - k) * cur->m_value + k * it->m_value);
					break;
				}
			} else if (m_defaultMotion) {
				CheckTarget(dt);
			}
		}
	}
}

// npc.cpp:2656
void ntNpc_BlackSparrow::Display(void) {
	if (!WSingleton<COption>::Instance()->vIsNpcEnabled() && GOLFDOC()->m_currentHole != 0x13) {
		return;
	}

	if (m_pPet && m_active && !(m_motionTime < 0.0f)) {
		if (m_flags & 1) {
			m_pPet->SetAlpha(m_alpha);
		}

		m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), (Abs(m_motionParam) > 0.0f ? RotMat(m_motionParam, 2) * m_matrix : m_matrix) * 1.0f);
		m_pPet->Transform(g_view, 1.0f, 0);
		m_pPet->UpdateBSphere();

		if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
			m_pPet->UpdateLightSource(&g_lightset, false, 0);
			WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, true, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), false, false);
		}
	}
}

// npc.cpp:2683
void ntNpc_BlackSparrow::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			int tries = 0;
			int nx, ny, nz;
			int index;
			int i;

			do {
				tries++;

				int rnd[3];
				for (i = 0; i < 3; i++) {
					rnd[i] = rand() % 3 - 1;
				}

				int cur = ((int *)m_pBuffer)[0];
				int x = cur % m_gridX;
				int y = cur / (m_gridZ * m_gridX);
				int z = cur % (m_gridZ * m_gridX) / m_gridX;

				nx = Between(0, rnd[0] + x, m_gridX - 1);
				ny = Between(0, rnd[1] + y, m_gridY - 1);
				nz = Between(0, rnd[2] + z, m_gridZ - 1);
				index = (m_gridZ * ny + nz) * m_gridX + nx;

				for (i = 0; i < m_gridCount; i++) {
					if (((int *)m_pBuffer)[i] == index)
						break;
				}
			} while (i < m_gridCount && tries < 5);

			for (i = m_gridCount - 1; i >= 1; i--) {
				((int *)m_pBuffer)[i] = ((int *)m_pBuffer)[i - 1];
			}
			((int *)m_pBuffer)[0] = index;

			m_pos.x = Random(0.0f, m_cellX, -1) + nx * m_cellX + m_area.min.x;
			if (m_gridY > 1)
				m_pos.y = Random(0.0f, m_cellY, -1) + ny * m_cellY + m_area.min.y;
			m_pos.z = Random(0.0f, m_cellZ, -1) + nz * m_cellZ + m_area.min.z;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			m_pos = m_recordedPath[m_replayIndex];
			m_replayIndex++;
		}
	}

	if (m_gridY > 1) {
		m_matrix.pivot += (m_pos - m_matrix.pivot).Normalize() * m_unk74 * dt;
	} else {
		float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

		if (Abs(angle) > data_9f0fc0) {
			float limit = dt * m_turnRate;
			angle = Between(-limit, angle, limit);
			m_matrix = RotMat(angle, 1) * m_matrix;
		}

		m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
	}
}

// npc.cpp:2769
ntNpc_PolaShip::ntNpc_PolaShip(const char *name, float speed) {
	Init(name, 0);
	m_hidden = true;
	m_distance = 0;
	m_speed = speed;
}

// npc.cpp:2779
ntNpc_PolaShip::~ntNpc_PolaShip(void) {
}

// npc.cpp:2783
void ntNpc_PolaShip::Process(float dt) {
	if (m_active && m_pPet) {
		m_motionTime += dt;

		if (m_hidden) {
			if (!(m_motionTime > m_speed)) {
				return;
			}

			m_motionTime = 0;
			m_hidden = false;
			SetMotion("NPC_Act02", -1.0f);
		}

		ScriptProcess(m_pMotion, m_motionTime - dt, m_motionTime);

		if (m_motionTime > m_pMotion->e) {
			m_motionTime -= m_pMotion->e - m_pMotion->s;
		}

		m_distance += dt * 10.0f;

		CheckTarget(dt);
	}
}

// npc.cpp:2814
void ntNpc_PolaShip::Display(void) {
	if (m_active && m_hidden != true) {
		CCreature::Display();
	}
}

// npc.cpp:2822
void ntNpc_PolaShip::CheckTarget(float dt) {
	WVector next = m_spline.Evaluate(m_distance + 0.1f);
	m_matrix.pivot = m_spline.Evaluate(m_distance);

	if (WisEqual(next, m_nodes[m_nodes.size() - 1], 1e-5f)) {
		m_distance = 0;
	}

	next.y = m_matrix.pivot.y;

	m_matrix = MakeCameraMatrix(next, m_matrix.pivot);
}

// npc.cpp:2836
void ntNpc_PolaShip::SetArea(const std::vector<WVector> &points, float radius, unsigned long flags) {
	CCreature::SetArea(points, radius, flags);

	WVector *pPoints = (WVector *)_alloca(points.size() * sizeof(WVector));
	float *pTimes = (float *)_alloca(points.size() * sizeof(float));

	pPoints[0] = points[0];
	pTimes[0] = 0;

	for (unsigned int i = 1; i < points.size(); i++) {
		pPoints[i] = points[i];
		pTimes[i] = pTimes[i - 1] + (points[i] - points[i - 1]).Magnitude();
	}

	m_spline.Init(pPoints, pTimes, points.size());
}

// npc.cpp:2858
ntNpc_PolaPenguin1::ntNpc_PolaPenguin1(const char *name, float speed) {
	m_unk74 = speed;
	Init(name, 0);
}

// npc.cpp:2865
ntNpc_PolaPenguin1::~ntNpc_PolaPenguin1(void) {
}

// npc.cpp:2869
void ntNpc_PolaPenguin1::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
				}
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:2897
void ntNpc_PolaPenguin1::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		FindTarget();
	}

	float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

	if (Abs(angle) > 0.017453292f) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
}

// npc.cpp:2912
void ntNpc_PolaPenguin1::FindTarget(void) {
	for (int i = 0; i < 3; i++) {
		float t = Random(0.0f, 1.0f, -1);
		m_pos.p[i] = m_area.min.p[i] * t + m_area.max.p[i] * (1.0f - t);
	}
	m_pos = GetPVS().GetGroundPoint(m_pos, false, 0, false);
}

// npc.cpp:2931
ntNpc_PolaPenguin2::ntNpc_PolaPenguin2(const char *name) {
	m_unk74 = 2.0f;
	Init(name, 0);
	m_unk120 = true;
	SetMotion("NPC_Act0", 0);
}

// npc.cpp:2941
ntNpc_PolaPenguin2::~ntNpc_PolaPenguin2(void) {
}

// npc.cpp:2946
void ntNpc_PolaPenguin2::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
					if (m_unk120 == true && IsSlidable() == true) {
						SetMotion("NPC_Default", 0);
						m_unk120 = false;
					} else {
						SetMotion("NPC_Act0", 0);
						m_unk120 = true;
					}
				}
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:2985
void ntNpc_PolaPenguin2::Display(void) {
	if (!WSingleton<COption>::Instance()->vIsNpcEnabled() && GOLFDOC()->m_currentHole != 0x13) {
		return;
	}

	if (m_pPet && m_active && !(m_motionTime < 0.0f)) {
		if (m_flags & 1) {
			m_pPet->SetAlpha(m_alpha);
		}

		WVector delta = m_pPet->GetDeltaVec(m_pMotion->name, m_motionTime);
		delta = RotVec(delta, m_matrix);

		m_matrix.pivot -= delta;

		m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), m_matrix);
		m_pPet->Transform(g_view, 1.0f, 0);
		m_pPet->UpdateBSphere();

		m_matrix.pivot += delta;

		if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
			m_pPet->UpdateLightSource(&g_lightset, false, 0);
			WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, true, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), false, false);
		}
	}
}

// npc.cpp:3030
void ntNpc_PolaPenguin2::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		FindTarget();
	}

	if (m_unk120) {
		float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

		if (Abs(angle) > 0.017453292f) {
			float limit = dt * m_turnRate;
			angle = Between(-limit, angle, limit);
			m_matrix = RotMat(angle, 1) * m_matrix;
		}

		m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
		return;
	} else {
		WVector delta = m_pPet->GetDeltaVec(m_pMotion->name, m_motionTime - dt, m_motionTime);
		delta = RotVec(delta, m_matrix);

		m_matrix.pivot += delta;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
	}
}

// npc.cpp:3056
void ntNpc_PolaPenguin2::FindTarget(void) {
	for (int i = 0; i < 3; i++) {
		float r = Random(0, 1.0f, -1);
		(&m_pos.x)[i] = (1.0f - r) * (&m_area.max.x)[i] + r * (&m_area.min.x)[i];
	}

	while (m_area.IsInclude(m_pos)) {
		m_pos += (m_pos - m_matrix.pivot).Normalize() * 3.2f;
	}

	m_pos = GetPVS().GetGroundPoint(m_pos, true, 0, false);
}

// npc.cpp:3072
bool ntNpc_PolaPenguin2::IsSlidable(void) {
	float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

	if (Abs(angle) > 0.017453292f)
		return false;

	w_motion_data *motion = m_pPet->FindMotionData("NPC_Default");

	if (motion) {
		WVector delta = m_pos - m_matrix.pivot;
		WVector move = m_pPet->GetDeltaVec("NPC_Default", motion->e);

		if (delta.Magnitude() < move.Magnitude())
			return false;

		return true;
	}

	return false;
}

// npc.cpp:3104
CNPC_BIRD::CNPC_BIRD(const char *name, float speed) {
	m_unk74 = speed;
	Init(name, 0);
}

// npc.cpp:3109
CNPC_BIRD::~CNPC_BIRD(void) {
}

// npc.cpp:3114
void CNPC_BIRD::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					m_motionTime -= m_pMotion->e - m_pMotion->s;
				}
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:3141
void CNPC_BIRD::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		FindTarget();
	}

	float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

	if (Abs(angle) > 0.017453292f) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
}

// npc.cpp:3156
void CNPC_BIRD::FindTarget(void) {
	for (int i = 0; i < 3; i++) {
		float t = Random(0.0f, 1.0f, -1);
		m_pos.p[i] = m_area.min.p[i] * t + m_area.max.p[i] * (1.0f - t);
	}
	m_pos = GetPVS().GetGroundPoint(m_pos, false, 0, false);
}

// npc.cpp:3170
CNPC_PARROT::CNPC_PARROT(const char *name) {
	m_unk74 = 10.0f;
	Init(name, 0);
	m_unk7C = 0.785398f;
	m_unk84 = 48.0f;
}

// npc.cpp:3178
CNPC_PARROT::~CNPC_PARROT(void) {
}

// npc.cpp:3182
CNPC_TORTOISE::CNPC_TORTOISE(const char *name, float speed) {
	Init(name, 0);
	m_nodeIndex = 1;
	m_unk74 = speed * 2.0f;
	m_unk7C = 0.785398f;
	m_turnRate = 2.0943952f;
	m_climbRate = 0;
	m_rollRate = 1.5707964f;
	m_state = 2;
}

// npc.cpp:3195
CNPC_TORTOISE::~CNPC_TORTOISE(void) {
}

// npc.cpp:3200
void CNPC_TORTOISE::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (!(time < 0.0f)) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				m_motionTime -= m_pMotion->e;
				SetMotion("NPC_Default", 0);
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:3222
void CNPC_TORTOISE::CheckTarget(float dt) {
	WVector d = m_pos - m_matrix.pivot;
	d.y = 0.0f;

	if (d.Magnitude() < (dt < 0.1f ? 0.1f : dt) * m_unk74) {
		if (m_replayIndex >= m_recordedPath.size() || m_recorded) {
			if (m_nodeIndex == m_nodes.size() - 1)
				m_nodeIndex = 0;

			m_nodeIndex = (m_nodeIndex + 1) % m_nodes.size();
			m_pos = m_nodes[m_nodeIndex];
			m_pos += (RotMat(Random(-1.5707964f, 1.5707964f, -1), 0).za * m_unkAC) * RotMat(Random(-3.1415927f, 3.1415927f, -1), 1);

			d = m_pos - m_matrix.pivot;
			d.y = 0.0f;

			if (m_recorded)
				m_recordedPath.push_back(m_pos);
		} else {
			if (m_replayIndex < m_recordedPath.size()) {
				m_pos = m_recordedPath[m_replayIndex];
				m_replayIndex++;
			} else {
				m_nodeIndex = 0;
			}
		}
	}

	float angle = CalcDeltaAngle(d, m_matrix.za * -1.0f);

	if (Abs(angle) > data_9f0fc0) {
		float limit = dt * m_turnRate;
		angle = Between(-limit, angle, limit);
		m_matrix = RotMat(angle, 1) * m_matrix;
	}

	m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
	m_matrix.pivot.y += dt * m_climbRate;
	m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
}

// npc.cpp:3281
CNPC_TRSHIP::CNPC_TRSHIP(const char *name, float speed) {
	Init(name, 0);
	m_distance = 0;

	if (Between(-1.0f, speed, 1.0f) == speed)
		m_speed = 1.0f;
	else
		m_speed = speed;

	m_hidden = true;
}

// npc.cpp:3298
CNPC_TRSHIP::~CNPC_TRSHIP(void) {
}

// npc.cpp:3303
void CNPC_TRSHIP::Process(float dt) {
	if (m_active && m_pPet) {
		m_motionTime += dt;

		if (m_hidden) {
			if (!(m_motionTime > 0.0f)) {
				return;
			}

			m_motionTime = 0;
			m_hidden = false;
			SetMotion("NPC_default", -1.0f);
		}

		ScriptProcess(m_pMotion, m_motionTime - dt, m_motionTime);

		if (m_motionTime > m_pMotion->e) {
			m_motionTime -= m_pMotion->e - m_pMotion->s;
		}

		m_distance += dt * 10.0f;

		CheckTarget(dt);
	}
}

// npc.cpp:3334
void CNPC_TRSHIP::Display(void) {
	if (m_active && m_hidden != true) {
		CCreature::Display();
	}
}

// npc.cpp:3344
void CNPC_TRSHIP::CheckTarget(float dt) {
	WVector next = m_spline.Evaluate(m_distance + 0.1f);
	m_matrix.pivot = m_spline.Evaluate(m_distance);

	if (WisEqual(next, m_nodes[m_nodes.size() - 1], 1e-5f)) {
		m_distance = 0;
	}

	next.y = m_matrix.pivot.y;

	m_matrix = MakeCameraMatrix(next, m_matrix.pivot);
}

// npc.cpp:3358
void CNPC_TRSHIP::SetArea(const std::vector<WVector> &points, float radius, unsigned long flags) {
	CCreature::SetArea(points, radius, flags);

	WVector *pPoints = (WVector *)_alloca(points.size() * sizeof(WVector));
	float *pTimes = (float *)_alloca(points.size() * sizeof(float));

	pPoints[0] = points[0];
	pTimes[0] = 0;

	for (unsigned int i = 1; i < points.size(); i++) {
		pPoints[i] = points[i];

		float length = (points[i] - points[i - 1]).Magnitude();
		pTimes[i] = pTimes[i - 1] + (length != 0.0f ? length : 1.0f) / m_speed;
	}

	m_spline.Init(pPoints, pTimes, points.size());
}

// npc.cpp:3462
ntNpc_AreaWalker::ntNpc_AreaWalker(const char *name) {
	m_walking = true;
	m_unk74 = 2.0f;
	Init(name, 0);
}

// npc.cpp:3469
ntNpc_AreaWalker::~ntNpc_AreaWalker(void) {
}

// npc.cpp:3473
void ntNpc_AreaWalker::Process(float dt) {
	if (m_pPet && m_active) {
		float time = dt + m_motionTime;
		m_motionTime = time;
		if (time >= 0.0f) {
			ScriptProcess(m_pMotion, time - dt, time);
			if (m_motionTime > m_pMotion->e) {
				if (m_flags & 1) {
					m_active = false;
					m_motionTime = m_pMotion->s;
				} else {
					SelectNextMotion();
				}
			}
			CheckTarget(dt);
		}
	}
}

// npc.cpp:3501
void ntNpc_AreaWalker::CheckTarget(float dt) {
	if ((m_matrix.pivot - m_pos).Magnitude() < 1.0f) {
		FindTarget();
	}

	if (m_walking) {
		float angle = CalcDeltaAngle(m_pos - m_matrix.pivot, m_matrix.za * -1.0f);

		if (Abs(angle) > 0.017453292f) {
			float limit = dt * m_turnRate;
			angle = Between(-limit, angle, limit);
			m_matrix = RotMat(angle, 1) * m_matrix;
		}

		m_matrix.pivot += m_matrix.za * -1.0f * m_unk74 * dt;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
	} else {
		WVector delta = m_pPet->GetDeltaVec(m_pMotion->name, m_motionTime - dt, m_motionTime);
		delta = RotVec(delta, m_matrix);

		m_matrix.pivot += delta;
		m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);
	}
}

// npc.cpp:3527
void ntNpc_AreaWalker::FindTarget(void) {
	for (int i = 0; i < 3; i++) {
		float r = Random(0, 1.0f, -1);
		(&m_pos.x)[i] = (1.0f - r) * (&m_area.max.x)[i] + r * (&m_area.min.x)[i];
	}

	while (m_area.IsInclude(m_pos)) {
		m_pos += (m_pos - m_matrix.pivot).Normalize() * 3.2f;
	}

	m_pos = GetPVS().GetGroundPoint(m_pos, true, 0, false);
}

// npc.cpp:3543
void ntNpc_AreaWalker::AddWalkingMotion(const char *name) {
	std::vector<std::string>::iterator it = std::find(m_walkingMotions.begin(), m_walkingMotions.end(), name);
	if (it == m_walkingMotions.end()) {
		m_walkingMotions.push_back(name);
	}
}

// npc.cpp:3550
void ntNpc_AreaWalker::SelectNextMotion(void) {
	std::map<std::string, std::list<MotionMap> >::iterator map = m_motionMaps.find(m_pMotion->name);

	if (map == m_motionMaps.end())
		return;

	int roll = rand() % 100;
	std::list<MotionMap> &motions = map->second;

	for (std::list<MotionMap>::iterator it = motions.begin(); it != motions.end(); ++it) {
		if (it->m_threshold > roll) {
			const char *name = it->m_motion.c_str();

			if (name) {
				SetMotion(name, 0);
				m_walking = std::find(m_walkingMotions.begin(), m_walkingMotions.end(), m_pMotion->name) != m_walkingMotions.end();
			}
			return;
		}
	}
}

// npc.cpp:3589
ntNpc_WizCityArea::ntNpc_WizCityArea(const char *name) : ntNpc_AreaWalker(name) {
	ConstructNextMotion();
	AddWalkingMotion("NPC_Default");
	int choice = rand() % 2;
	if (choice < 1) {
		SetMotion("NPC_Default", -1.0f);
	} else {
		m_walking = false;
		SetMotion("NPC_Ato", -1.0f);
	}
}

// npc.cpp:3604
void ntNpc_WizCityArea::Display(void) {
	WMatrix matrix = m_matrix; m_matrix *= m_scale;
	CCreature::Display();
	m_matrix = matrix;
}

// npc.cpp:3612
void ntNpc_WizCityArea::ConstructNextMotion(void) {
	MotionMap map;
	std::list<MotionMap> motions;
	map.m_motion = "NPC_Ato";

	map.m_threshold = 100;
	motions.push_back(map);
	m_motionMaps.insert(std::make_pair("NPC_Default", motions));

	map.m_motion = "NPC_Default";
	map.m_threshold = 100;
	motions.clear();
	motions.push_back(map);
	m_motionMaps.insert(std::make_pair("NPC_Ato", motions));
}

// npc.cpp:3632
ntNpc_NodeWalker::ntNpc_NodeWalker(const char *name, float speed) {
	m_unk74 = speed;
	m_splineLength = 0;
	m_distance = 0;
	Init(name, 0);
}

// npc.cpp:3641
ntNpc_NodeWalker::~ntNpc_NodeWalker(void) {
}

// npc.cpp:3645
void ntNpc_NodeWalker::Process(float dt) {
	m_distance += dt * m_unk74;
	CCreature::Process(dt);
}

// npc.cpp:3651
void ntNpc_NodeWalker::SetArea(const std::vector<WVector> &points, float radius, unsigned long flags) {
	CCreature::SetArea(points, radius, flags);

	m_nodes.push_back(points[0]);
	m_nodes.push_back(points[1]);

	MakeSpline(m_nodes, m_splineLength, m_spline);
}

// npc.cpp:3664
void ntNpc_NodeWalker::CheckTarget(float dt) {
	WVector next = m_spline.Evaluate(m_distance + 0.1f);
	m_matrix.pivot = m_spline.Evaluate(m_distance);

	if (m_distance > m_splineLength) {
		m_distance = 0;
	}

	next = GetPVS().GetGroundPoint(next, true, 0, false);
	m_matrix.pivot = GetPVS().GetGroundPoint(m_matrix.pivot, true, 0, false);

	if (next.y < m_matrix.pivot.y)
		next.y = m_matrix.pivot.y;

	m_matrix = MakeCameraMatrix(next, m_matrix.pivot);
}

// npc.cpp:3684
void ntNpc_NodeWalker::MakeSpline(const std::vector<WVector> &points, float &length, ntCatmullRom &spline) {
	int count = points.size();
	WVector *pPoints = (WVector *)_alloca(count * sizeof(WVector));
	float *pTimes = (float *)_alloca(count * sizeof(float));

	pPoints[0] = points[0];
	pTimes[0] = 0;

	for (int i = 1; i < count; i++) {
		pPoints[i] = points[i];
		pTimes[i] = pTimes[i - 1] + (points[i] - points[i - 1]).Magnitude();
	}

	length = pTimes[count - 1];

	spline.Init(pPoints, pTimes, count);
}

// npc.cpp:3711
ntNpc_WizCityNode::ntNpc_WizCityNode(const char *name, float speed) : ntNpc_NodeWalker(name, speed) {
	m_scale = 1.0f;
}

// npc.cpp:3716
void ntNpc_WizCityNode::Display(void) {
	WMatrix matrix = m_matrix; m_matrix *= m_scale;
	CCreature::Display();
	m_matrix = matrix;
}

// npc.cpp:3729
CDisposable::CDisposable(const char *name) {
	m_flags |= 1;
	Init(name, 0);
}

// npc.cpp:3735
void CDisposable::PlayEvent(void) {
	CCreature::PlayEvent();

	m_matrix.za = m_nodes[0] - m_nodes[1];
	m_matrix.za.y = 0;
	m_matrix.za.Normalize();
	m_matrix.ya = WVector::UNIT_POS_Y;
	m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);
	m_matrix.pivot = GetPVS().GetGroundPoint(m_nodes[0], true, 0, true);
}

// npc.cpp:3752
COnGameNet::COnGameNet(const char *name, unsigned char playerIndex) : m_hTexture(0) {
	Init(name, 0);
	m_unk74 = 0.5f;
	m_playerIndex = playerIndex;
	m_hTexture = g_resrcmng->LoadTexture("camera.jpg", 0, 0, MakeStr("camera_%d.jpg", playerIndex));
	w_texlist *texture = g_resrcmng->FindTexture("camera.jpg");

	if (texture) {
		m_pPet->ChangeTextureByHandle(m_hTexture, texture->texhandle, 0);
	}
}

// npc.cpp:3768
COnGameNet::~COnGameNet(void) {
	if (g_resrcmng && m_hTexture) { g_resrcmng->Release(m_hTexture); m_hTexture = 0; }
}

// npc.cpp:3773
void COnGameNet::Process(float dt) {
	if (GOLFDOC()->m_myPlayerIndex != m_playerIndex) {
		m_active = false;
		return;
	}

	m_active = true;
	CCreature::Process(dt);
}

// npc.cpp:3785
void COnGameNet::CheckTarget(float dt) {
	if (GolfBall().m_preDataIndex >= 0 && GOLFDOC()->GameMode() != 0x1000) return;

	(!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask())->GetActor("Player") << MsgObject(0, 0x124, m_playerIndex, (intptr_t)&m_playerMatrix, 0, 0, 0);

	WVector dir = m_playerMatrix.za;

	dir.y = 0.0f; dir.Normalize();

	if (GolfBall().m_groundType == 2)
		m_pos = m_playerMatrix.pivot - dir * 2.56f;
	else
		m_pos = m_playerMatrix.pivot - dir * 3.84f;
	m_pos.y = GolfBall().m_pos.y + 3.84f;

	dir = m_pos - m_matrix.pivot;
	float dist = dir.Magnitude();
	float t = dist * 0.15625f;
	if (t > 1.0f) t = 1.0f;

	m_matrix.za = dir * t + (GolfBall().m_pos - m_matrix.pivot) * (1.0f - t);

	m_matrix.za.y = 0.0f; m_matrix.za.Normalize();
	m_matrix.za *= -1.0f;
	m_matrix.ya = WVector::UNIT_POS_Y;
	m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);

	WVector pivot = m_matrix.pivot;
	m_matrix.pivot = WVector::ZERO;

	m_matrix = RotMat(-0.5235988f, 1) * m_matrix;

	m_matrix.pivot = pivot;
	if (dist < 32.0000005) {
		if (dist > 0.5)
			m_matrix.pivot += dir * (dist * m_unk74) * dt;
	} else {
		m_matrix.pivot = m_pos;
	}
}

// npc.cpp:3836
ntCatmullRom::ntCatmullRom(void) : m_pPoints(0), m_pTimes(0), m_pLengths(0), m_totalLength(0), m_count(0) {
}

// npc.cpp:3840
ntCatmullRom::~ntCatmullRom(void) {
	Clean();
}

// npc.cpp:3845
bool ntCatmullRom::Init(const WVector *points, const float *times, int count) {
	if (m_count)
		return false;

	if (count < 4 || !points || !times)
		return false;

	m_pPoints = new WVector[count];
	m_pTimes = new float[count];
	m_count = count;
	for (int i = 0; i < count; i++) {
		m_pPoints[i] = points[i];
		m_pTimes[i] = times[i];
	}

	int segments = count - 1;
	m_pLengths = new float[segments];
	m_totalLength = 0;
	for (int i = 0; i < segments; i++) {
		m_pLengths[i] = SegmentArcLength(i, 0.0f, 1.0f);
		m_totalLength += m_pLengths[i];
	}

	return true;
}

// npc.cpp:3883
void ntCatmullRom::Clean(void) {
	if (m_pPoints) { delete[] m_pPoints; m_pPoints = 0; }
	if (m_pTimes) { delete[] m_pTimes; m_pTimes = 0; }
	if (m_pLengths) { delete[] m_pLengths; m_pLengths = 0; }

	m_totalLength = 0;
	m_count = 0;
}

// npc.cpp:3894
WVector ntCatmullRom::Evaluate(float t) {
	if (m_count < 2)
		return WVector::ZERO;

	const float *times = m_pTimes;
	if (t <= times[0])
		return m_pPoints[0];
	if (!(t < times[m_count - 1]))
		return m_pPoints[m_count - 1];

	// Segment i holds t: times[i] < t <= times[i + 1].
	int last = m_count - 1;
	int i = 0;
	while (i < last && t > times[i + 1])
		i++;

	float u = (t - times[i]) / (times[i + 1] - times[i]);
	const WVector *p = m_pPoints;
	WVector r;

	if (i == 0) {
		// Quadratic through the first three points.
		float h = u * 0.5f;
		r.x = ((p[0].x - 2 * p[1].x + p[2].x) * u + (4 * p[1].x - 3 * p[0].x - p[2].x)) * h + p[0].x;
		r.y = ((p[0].y - 2 * p[1].y + p[2].y) * u + (4 * p[1].y - 3 * p[0].y - p[2].y)) * h + p[0].y;
		r.z = ((p[0].z - 2 * p[1].z + p[2].z) * u + (4 * p[1].z - 3 * p[0].z - p[2].z)) * h + p[0].z;
	} else if (i >= m_count - 2) {
		// Quadratic through the last three points.
		const WVector &a = p[i - 1];
		const WVector &b = p[i];
		const WVector &c = p[i + 1];
		float h = u * 0.5f;
		r.x = ((a.x - 2 * b.x + c.x) * u + (c.x - a.x)) * h + b.x;
		r.y = ((a.y - 2 * b.y + c.y) * u + (c.y - a.y)) * h + b.y;
		r.z = ((a.z - 2 * b.z + c.z) * u + (c.z - a.z)) * h + b.z;
	} else {
		// Catmull-Rom cubic between p[i] and p[i + 1].
		const WVector &a = p[i - 1];
		const WVector &b = p[i];
		const WVector &c = p[i + 1];
		const WVector &d = p[i + 2];
		float h = u * 0.5f;
		r.x = (((3 * b.x - a.x - 3 * c.x + d.x) * u + (2 * a.x - 5 * b.x + 4 * c.x - d.x)) * u + (c.x - a.x)) * h + b.x;
		r.y = (((3 * b.y - a.y - 3 * c.y + d.y) * u + (2 * a.y - 5 * b.y + 4 * c.y - d.y)) * u + (c.y - a.y)) * h + b.y;
		r.z = (((3 * b.z - a.z - 3 * c.z + d.z) * u + (2 * a.z - 5 * b.z + 4 * c.z - d.z)) * u + (c.z - a.z)) * h + b.z;
	}
	return r;
}

// npc.cpp:3961
WVector ntCatmullRom::Velocity(float t) {
	if (m_count < 2)
		return WVector::ZERO;

	const float *times = m_pTimes;
	if (t <= times[0])
		t = 0.0f;
	else if (!(t < times[m_count - 1]))
		t = times[m_count - 1];

	int last = m_count - 1;
	int i = 0;
	while (i < last && t > times[i + 1])
		i++;

	float dt = times[i + 1] - times[i];
	float u = (t - times[i]) / dt;
	const WVector *p = m_pPoints;
	WVector v;

	if (i == 0) {
		v.x = (p[0].x - 2 * p[1].x + p[2].x) * u + (4 * p[1].x - 3 * p[0].x - p[2].x) * 0.5f;
		v.y = (p[0].y - 2 * p[1].y + p[2].y) * u + (4 * p[1].y - 3 * p[0].y - p[2].y) * 0.5f;
		v.z = (p[0].z - 2 * p[1].z + p[2].z) * u + (4 * p[1].z - 3 * p[0].z - p[2].z) * 0.5f;
	} else if (i >= m_count - 2) {
		const WVector &a = p[i - 1];
		const WVector &b = p[i];
		const WVector &c = p[i + 1];
		v.x = (a.x - 2 * b.x + c.x) * u + (c.x - a.x) * 0.5f;
		v.y = (a.y - 2 * b.y + c.y) * u + (c.y - a.y) * 0.5f;
		v.z = (a.z - 2 * b.z + c.z) * u + (c.z - a.z) * 0.5f;
	} else {
		const WVector &a = p[i - 1];
		const WVector &b = p[i];
		const WVector &c = p[i + 1];
		const WVector &d = p[i + 2];
		float h = u * 1.5f;
		v.x = ((3 * b.x - a.x - 3 * c.x + d.x) * h + (2 * a.x - 5 * b.x + 4 * c.x - d.x)) * u + (c.x - a.x) * 0.5f;
		v.y = ((3 * b.y - a.y - 3 * c.y + d.y) * h + (2 * a.y - 5 * b.y + 4 * c.y - d.y)) * u + (c.y - a.y) * 0.5f;
		v.z = ((3 * b.z - a.z - 3 * c.z + d.z) * h + (2 * a.z - 5 * b.z + 4 * c.z - d.z)) * u + (c.z - a.z) * 0.5f;
	}

	// Per unit of t instead of per unit of the segment parameter.
	v.x /= dt;
	v.y /= dt;
	v.z /= dt;
	return v;
}

// npc.cpp:4088
float ntCatmullRom::FindParameterByDistance(float t, float distance) {
	int count = m_count;
	const float &end = m_pTimes[count - 1];
	if (ArcLength(t, end) < distance)
		return end;

	float range = m_pTimes[count - 1] - m_pTimes[0];

	float u = t + range / m_totalLength * distance;

	for (int i = 0; i < 32; i++) {
		float diff = ArcLength(t, u) - distance;
		if (WAbsBySign(diff) < 0.001f)
			return u;

		u -= diff / Velocity(u).Magnitude();
	}

	return FLT_MAX;
}

// npc.cpp:4118
float ntCatmullRom::ArcLength(float t0, float t1) {
	if (!(t1 > t0))
		return 0.0f;

	const float *times = m_pTimes;
	if (t0 <= times[0])
		t0 = times[0];
	if (t1 >= times[m_count - 1])
		t1 = times[m_count - 1];

	int last = m_count - 1;
	int i0 = 0;
	while (i0 < last && t0 > times[i0 + 1])
		i0++;
	float u0 = (t0 - times[i0]) / (times[i0 + 1] - times[i0]);

	int i1 = 0;
	while (i1 < last && t1 > times[i1 + 1])
		i1++;
	float u1 = (t1 - times[i1]) / (times[i1 + 1] - times[i1]);

	if (i0 == i1)
		return SegmentArcLength(i0, u0, u1);

	float sum = SegmentArcLength(i0, u0, 1.0f);
	for (int i = i0 + 1; i < i1; i++)
		sum += m_pLengths[i];
	return SegmentArcLength(i1, 0.0f, u1) + sum;
}

// 5-point Gauss-Legendre rule (.rdata 0x9F0FD4 and 0x9F0FEC).
static const float s_gaussWeights[5] = { 0.5688889f, 0.47862867f, 0.47862867f, 0.23692688f, 0.23692688f };
static const float s_gaussNodes[5] = { 0.0f, 0.53846931f, -0.53846931f, 0.90617985f, -0.90617985f };

// npc.cpp:4176
float ntCatmullRom::SegmentArcLength(int segment, float t0, float t1) {
	if (!(t1 > t0))
		return 0.0f;
	if (t0 < 0.0f)
		t0 = 0.0f;
	if (t1 > 1.0f)
		t1 = 1.0f;

	// Polynomial coefficients of the derivative (same pieces as Evaluate).
	const WVector *p = m_pPoints;
	WVector a, b, c;
	if (segment == 0) {
		a.x = p[0].x - 2 * p[1].x + p[2].x;
		a.y = p[0].y - 2 * p[1].y + p[2].y;
		a.z = p[0].z - 2 * p[1].z + p[2].z;
		b.x = 4 * p[1].x - 3 * p[0].x - p[2].x;
		b.y = 4 * p[1].y - 3 * p[0].y - p[2].y;
		b.z = 4 * p[1].z - 3 * p[0].z - p[2].z;
	} else if (segment >= m_count - 2) {
		segment = m_count - 2;
		const WVector &pa = p[segment - 1];
		const WVector &pb = p[segment];
		const WVector &pc = p[segment + 1];
		a.x = pa.x - 2 * pb.x + pc.x;
		a.y = pa.y - 2 * pb.y + pc.y;
		a.z = pa.z - 2 * pb.z + pc.z;
		b.x = pc.x - pa.x;
		b.y = pc.y - pa.y;
		b.z = pc.z - pa.z;
	} else {
		const WVector &pa = p[segment - 1];
		const WVector &pb = p[segment];
		const WVector &pc = p[segment + 1];
		const WVector &pd = p[segment + 2];
		a.x = 3 * pb.x - pa.x - 3 * pc.x + pd.x;
		a.y = 3 * pb.y - pa.y - 3 * pc.y + pd.y;
		a.z = 3 * pb.z - pa.z - 3 * pc.z + pd.z;
		b.x = 2 * pa.x - 5 * pb.x + 4 * pc.x - pd.x;
		b.y = 2 * pa.y - 5 * pb.y + 4 * pc.y - pd.y;
		b.z = 2 * pa.z - 5 * pb.z + 4 * pc.z - pd.z;
		c.x = pc.x - pa.x;
		c.y = pc.y - pa.y;
		c.z = pc.z - pa.z;
	}

	float range = t1 - t0;
	float sum = 0.0f;
	for (int k = 0; k < 5; k++) {
		float x = (range * s_gaussNodes[k] + t0 + t1) * 0.5f;
		WVector v;
		if (segment == 0 || segment >= m_count - 2) {
			v.x = a.x * x + b.x * 0.5f;
			v.y = a.y * x + b.y * 0.5f;
			v.z = a.z * x + b.z * 0.5f;
		} else {
			float h = x * 1.5f;
			v.x = (a.x * h + b.x) * x + c.x * 0.5f;
			v.y = (a.y * h + b.y) * x + c.y * 0.5f;
			v.z = (a.z * h + b.z) * x + c.z * 0.5f;
		}
		sum += s_gaussWeights[k] * sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	return sum * range * 0.5f;
}
