// Original: d:\build\custom\temp\code\client\projectg\club.cpp
#include <math.h>
#include "club.h"
#include "wutil.h"
#include <string.h>
#include "wmath.h"
#include "golfball.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "quadtree.h"
#include "wresrcmng.h"
#include <algorithm>
#include "ChatMsg.h"
#include "Packet.h"
#include "hackingmanager.h"
#include "cardsystem.h"
#include "../shared/Localize.h"

// Declared nowhere yet: SharedDoc.h (PerformPowerBarSetting) and
// s5/Utilities.h (S5::IsUsableCalipers) are other units' headers.
void PerformPowerBarSetting(void);
namespace S5 {
	bool IsUsableCalipers(void);
}

// The accuracy each ground type adds, indexed by CGolfBall::m_groundType.
extern float ground_accuracy[];

// The club names AddClub looks a club's id up in (17), and the bar speed of
// each id; defined with the unit's data.
extern char *clubName[];
extern float bar_speed[];

// The scene camera (declared by no header in src yet).
extern WMatrix g_camera;


// club.cpp:73
CClub &GolfClub(void) {
	static CClub club;
	return club;
}

// club.cpp:79
CClub::~CClub(void) {
	for (sClub *club = m_clubList.Start(); club; club = m_clubList.Next()) { if (club) delete club; } m_clubList.Reset();
}

// club.cpp:84
void CClub::ClearClubSet(void) {
	std::list<sClubSet *>::iterator it;


	for (it = m_clubSetList.begin(); it != m_clubSetList.end(); ++it) {

		sClubSet *clubSet = *it;

		for (int i = 0; i < 4; i++) {

			if (g_resrcmng != NULL && clubSet->m_puppet[0][i] != NULL) { g_resrcmng->Release(clubSet->m_puppet[0][i], false); clubSet->m_puppet[0][i] = NULL; }
			if (g_resrcmng != NULL && clubSet->m_puppet[1][i] != NULL) { g_resrcmng->Release(clubSet->m_puppet[1][i], false); clubSet->m_puppet[1][i] = NULL; }
		}

		delete clubSet;
	}

	m_clubSetList.clear();

	m_calcRandomPower = false;
}

// club.cpp:107
void CClub::Init(void) {
	// club.cpp:110
	AddClub("1W", 230.0f, 10.0f, 236.0f, 1.61f, 0.55f, 0);
	AddClub("2W", 210.0f, 13.0f, 204.0f, 1.41f, 0.5f, 0);
	AddClub("3W", 190.0f, 16.0f, 176.0f, 1.26f, 0.45f, 0);

	AddClub("2I", 180.0f, 20.0f, 161.0f, 1.07f, 0.45f, 1);
	AddClub("3I", 170.0f, 24.0f, 149.0f, 0.95f, 0.45f, 1);
	AddClub("4I", 160.0f, 28.0f, 139.0f, 0.83f, 0.45f, 1);
	AddClub("5I", 150.0f, 32.0f, 131.0f, 0.73f, 0.45f, 1);
	AddClub("6I", 140.0f, 36.0f, 124.0f, 0.67f, 0.41f, 1);
	AddClub("7I", 130.0f, 40.0f, 118.0f, 0.61f, 0.36f, 1);
	AddClub("8I", 120.0f, 44.0f, 114.0f, 0.57f, 0.3f, 1);
	AddClub("9I", 110.0f, 48.0f, 110.0f, 0.53f, 0.25f, 1);

	AddClub("PW", 100.0f, 52.0f, 107.0f, 0.49f, 0.18f, 2);
	AddClub("SW", 80.0f, 56.0f, 93.0f, 0.42f, 0.17f, 2);

	AddClub("1PT", 20.0f, 0.0f, 30.0f, 0.0f, 0.0f, 3);
	AddClub("2PT", 10.0f, 0.0f, 21.0f, 0.0f, 0.0f, 3);

	// club.cpp:131
	m_unusedHead = 0;
	m_powerShot = 0;
	m_approachMode = 4;

	m_barAlpha = 0.0f;
	m_barPosition = 0.0f;
	m_bar = 2;
	m_itemWindow = false;
	m_prevClubIndex = 0;
	m_randomPower = 0.0f;
	m_calcRandomPower = false;
	m_pClub = m_clubList.Find("1W");
	m_clubIndex = m_pClub->m_index;
}

// club.cpp:147
void CClub::AddCrypticValue(void) {
	if (HackingManager::Instance()) {

		HackingManager::Instance()->AddCrypticIntValue(&m_powerShot);


		for (sClub *club = m_clubList.Start(); club; club = m_clubList.Next()) {

			HackingManager::Instance()->AddCrypticFloatValue(&club->m_range);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_lie);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_power);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_curve);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_spin);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_barSpeed);
		}
	}
}

// club.cpp:166
void CClub::Process(float time) {
	if (GOLFDOC()->m_gameState < 1)
		return;


	if (IsLocalContent((localContentType_t)0x4F) && WSingleton<CSharedDoc>::Instance()->m_playingMode == 2)
		return;


	if (IsMyTurn(false)) {

		ClubProcess(time);
		BarProcess(time);
	}


	if (m_prevClubIndex != m_clubIndex) {

		m_flags |= 1;

		if (GetType() != GetType(m_prevClubIndex))
			m_flags |= 2;
		else
			m_flags &= ~2;

		GOLFDOC()->GetBar().m_accuracy = GetType() == 3 ? 140.0f : 105.0f;
		GOLFDOC()->GetBar().m_prevAccuracy = GOLFDOC()->GetBar().m_accuracy;

		if (IsMyTurn(false) && (GOLFDOC()->GameMode() & 0x200) && m_clubIndex < m_prevClubIndex && m_approachMode == 4)
			g_camera = GOLFDOC()->MakeTopViewCameraMatrix(true);

		m_prevClubIndex = m_clubIndex;
	} else {


		m_flags &= ~3;
	}
}

// club.cpp:290
void CClub::BarProcess(float time) {
	switch (m_bar) {
	case 0:

		if (m_flags & 1)
			m_barDelay = 0.0f;

		m_barDelay += time;
		if (m_barDelay > 0.7f || CChatMsg::Instance()->IsActive()) {




			m_bar = 1;
		}
		break;

	case 1:
		if ((m_flags & 1) && (!CChatMsg::Instance()->IsActive() || !m_itemWindow)) {
			m_bar = 3;
			m_barAlpha = 0.0f;
			m_barPosition = 456.0f;
			break;
		}

		m_barPosition -= time * 1000.0f;
		if (m_barPosition < 0.0f) {

			m_barPosition = 0.0f;
			m_bar = 2;

			if (OnlinePlay()) {

				WSendPacket packet((enumClientPacket)0x16);
				packet.Encode1(m_clubIndex);
				packet.Send(SEND_TO_DEFAULT);
			}
		}
		break;

	case 3:
		m_barAlpha += time * 765.0f;
		if (m_barAlpha > 255.0f) {

			m_barDelay = 0.0f;

			m_bar = 0;
			m_barAlpha = 255.0f;
		}
		break;

	case 2:
		if ((m_flags & 1) && !CChatMsg::Instance()->IsActive() && !m_itemWindow) {

			if (GolfBall().m_groundType == 2) {

				if (OnlinePlay()) {

					WSendPacket packet((enumClientPacket)0x16);
					packet.Encode1(m_clubIndex);
					packet.Send(SEND_TO_DEFAULT);
				}
			} else {
				m_bar = 3;
				m_barAlpha = 0.0f;
				m_barPosition = 456.0f;
			}
		}
		break;
	}
}

// club.cpp:374
void CClub::AddClub(char *name, float range, float lie, float power, float curve, float spin, unsigned char type) {

	sClub *club = m_clubList.Find(name);

	if (club) {

		club->m_range = range;
		club->m_lie = lie * 0.0174532925f;

		club->m_type = type;
		club->m_power = power;
		club->m_curve = curve;
		club->m_spin = spin;
	} else {





		club = new sClub;


		club->m_pointFactor = 1.0f;
		club->m_range = range;
		club->m_lie = lie * 0.0174532925f;
		club->m_type = type;
		club->m_power = power;
		club->m_curve = curve;
		club->m_spin = spin;
		club->m_index = (unsigned char)m_clubList.GetCount();

		strcpy(club->m_name, name);

		m_clubList.AddItem(club, club->m_name, false);

		for (unsigned char i = 0; i < 0x11; i++) {

			if (!strcmp(club->m_name, clubName[i])) {

				club->m_id = i;
				break;
			}
		}




		club->m_barSpeed = bar_speed[club->m_id];

		if (HackingManager::Instance()) {

			HackingManager::Instance()->AddCrypticFloatValue(&club->m_range);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_lie);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_power);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_curve);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_spin);
			HackingManager::Instance()->AddCrypticFloatValue(&club->m_barSpeed);
		}
	}
}

// club.cpp:431
CClub::sClub *CClub::SelectClub(unsigned char index) {
	sClub *club = m_clubList.Start();

	for (unsigned char i = 0; i < index; i++) {


		club = m_clubList.Next();
	}

	return club;
}

// club.cpp:445
void CClub::SelectClubByDist(float dist) {
	for (m_clubIndex = m_clubList.GetCount() - 1, m_pClub = m_clubList.End(); m_pClub; m_pClub = m_clubList.Prev(), m_clubIndex--) {

		if (m_clubIndex == m_approachClubIndex)
			break;


		if (PowerShotType() && m_pClub->m_type == 3)
			continue;


		if (GetRange(NULL) > dist)
			break;
	}

	if (m_pClub == NULL) {


		m_clubIndex = 0;
		m_pClub = m_clubList.Find("1W");
	}
}

// club.cpp:467
void CClub::SelectBestClub(void) {
	SelectBestClub(WVector(GOLFDOC()->GetHoleData().m_cupPos));

	if (S5::IsUsableCalipers()) {
		PerformPowerBarSetting();
	}
}

// club.cpp:652
void CClub::SetAccuracy(void) {
	m_accuracy = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_accuracy + ground_accuracy[GolfBall().m_groundType];
	float wide = m_accuracy + 10.0f;
	if (wide > 35.0f)
		wide = 35.0f;

	m_wideAccuracy = wide;

	if (IsLocalContent((localContentType_t)0x2F)) {

		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000028)
			m_accuracy = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_accuracy + ground_accuracy[1];
	}
}

// club.cpp:664
void CClub::SetApproachMode(void) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos;
	CGolfBall &ball = GolfBall();





	m_approachClubIndex = 7;
	float dx = ball.m_pos.x - cup.x;
	float dz = ball.m_pos.z - cup.z;
	float dist = (float)sqrt(dx * dx + dz * dz) * 0.3125f;

	if (dist < 10.0f) {
		m_powerPutter = false;
		m_approachMode = 0;
	} else if (dist < 15.0f) {

		m_powerPutter = false;
		m_approachMode = 1;

	} else {
		if (dist < 28.0f) {

			m_approachMode = 2;

		} else if (dist < 58.0f) {

			m_approachMode = 3;

		} else {

			m_approachMode = 4;
			m_approachClubIndex = 0;
		}
		m_powerPutter = true;
	}

	if (GetPVS().GetGroundType(GolfBall().m_pos, true, NULL) != 2)
		m_powerPutter = true;
}

// club.cpp:703
float CClub::GetNarrowAccuracy(void) {
	if (m_pClub->m_type == 3)
		return 0.0f;

	return Max((float)ceil(m_accuracy * (GolfClub().PowerShotType() ? 0.6f : 1.0f)), 2.0f);
}

// club.cpp:711
float CClub::GetWideAccuracy(void) {
	if (m_pClub->m_type == 3)
		return 0.0f;

	return Min(Max((float)ceil(m_accuracy * (GolfClub().PowerShotType() ? 0.6f : 1.0f)), 2.0f) + 10.0f, 35.0f);
}

// club.cpp:719
float CClub::GetRange(const char *name) {
	sClub *club;
	if (name)
		club = m_clubList.Find(name);
	else
		club = m_pClub;

	float range = club->m_range;

	switch (club->m_type) {
	case 0:
		// The air night club set (0x10000012) has its own wood ranges.
		if (CSharedDoc::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_clubSetTypeId == 0x10000012) {
			float luckyRange = GetAirNightLuckyRange(club->m_name);
			if (luckyRange > 0.0f) {
				range = GetPowerShotFactor() + PLAYER(GOLFDOC()->m_myPlayerIndex)->m_power + luckyRange;
			} else {
				range = club->m_range + GetPowerShotFactor() + PLAYER(GOLFDOC()->m_myPlayerIndex)->m_power;
			}
		} else {
			range = range + GetPowerShotFactor() + GOLFDOC()->GetPlayer(GOLFDOC()->m_myPlayerIndex)->m_power * 2.0f;
		}
		break;

	case 1:
		range = range + GetPowerShotFactor();
		break;

	case 2:
		switch (m_approachMode) {
		case 0:
		case 1:
		case 2:
			range = (PowerShotType() ? 30.0f : 0.0f) + 30.0f;
			break;
		case 3:
			range = (PowerShotType() ? 20.0f : 0.0f) + 60.0f;
			break;
		case 4:
			range = range + GetPowerShotFactor();
			break;
		}
		break;

	case 3:
		if (m_powerPutter) {
			range = Is("1PT", club) ? 40.0f : 30.0f;
		}
		break;
	}

	if (GetType() != 3) {
		range += CSharedDoc::Instance()->GetAuxPartProperty(GOLFDOC()->m_myPlayerIndex, 0);

		if (IsLocalContent((localContentType_t)0x70)) {
			float buff = (float)CSharedDoc::Instance()->GetBuffValue(GOLFDOC()->m_myPlayerIndex, 0x10);
			if (GolfClub().PowerShotType())
				buff += CSharedDoc::Instance()->GetBuffValue(GOLFDOC()->m_myPlayerIndex, 0x11);
			return range + buff;
		}

		CCardManager::Instance()->SetPlayerIndex(GOLFDOC()->m_myPlayerIndex);
		bool withPowerRange = false;
		if (GolfClub().PowerShotType())
			withPowerRange = true;
		return range + CCardManager::Instance()->GetTotalRange(withPowerRange);
	}

	return range;
}

// club.cpp:827
float CClub::GetLie(const char *name) {
	sClub *club;
	if (name)
		club = m_clubList.Find(name);
	else
		club = m_pClub;

	if (m_approachMode == 4)
		return club->m_lie;

	return club->m_lie + GolfBall().m_lieAngle * (3.14159265f / 45.0f);
}

// club.cpp:950
void CClub::ClearPointFactor(void) {
	for (sClub *club = m_clubList.Start(); club; club = m_clubList.Next())
		club->m_pointFactor = 1.0f;
}

// club.cpp:957
void CClub::SetPointFactor(float factor) {
	m_pClub->m_pointFactor = factor;
}

// club.cpp:962
unsigned char CClub::GetType(unsigned char index) {
	for (sClub *club = m_clubList.Start(); club; club = m_clubList.Next()) {
		if (club->m_index == index)
			return club->m_type;
	}

	return 0;
}

// club.cpp:977
void CClub::SetClub(const char *name) {
	m_pClub = m_clubList.Find(name);
	m_clubIndex = m_pClub->m_index;
}

// club.cpp:983
void CClub::SetClub(unsigned char index) {
	m_pClub = SelectClub(index);
	m_clubIndex = m_pClub->m_index;
}

// club.cpp:1076
sClubSet *CClub::FindClubSet(unsigned long typeId) {


	std::list<sClubSet *>::iterator it = std::find_if(m_clubSetList.begin(), m_clubSetList.end(), sFindClubSet(typeId));

	if (it != m_clubSetList.end())
		return *it;

	return NULL;
}

// club.cpp:1088
float CClub::GetPowerShotFactor(void) {
	float factor[4] = {
		0.0f,
		10.0f,
		20.0f,
		15.0f,
	};

	return factor[m_powerShot];
}

// club.cpp:1098
float CClub::GetAirNightLuckyRange(char *name) {
	float range = 0.0f;

	if (!stricmp(name, "1W"))
		return 216.0f;
	else if (!stricmp(name, "2W"))
		return 206.0f;
	else if (!stricmp(name, "3W"))
		return 196.0f;

	return range;
}

// club.cpp:1115
float CClub::GetAirNightLuckyPower(char *name) {
	float power = 0.0f;

	if (!stricmp(name, "1W"))
		return 216.0f;
	else if (!stricmp(name, "2W"))
		return 194.0f;
	else if (!stricmp(name, "3W"))
		return 176.0f;

	return power;
}

// club.cpp:1129
const char *CClub::GetClubString(eClub club) {
	switch (club) {
	case WOOD_1: return "WOOD_1";
	case WOOD_2: return "WOOD_2";
	case WOOD_3: return "WOOD_3";
	case IRON_2: return "IRON_2";
	case IRON_3: return "IRON_3";
	case IRON_4: return "IRON_4";
	case IRON_5: return "IRON_5";
	case IRON_6: return "IRON_6";
	case IRON_7: return "IRON_7";
	case IRON_8: return "IRON_8";
	case IRON_9: return "IRON_9";
	case WEDGE_P: return "WEDGE_P";
	case WEDGE_S: return "WEDGE_S";
	case PUTTER_1: return "PUTTER_1";
	case PUTTER_2: return "PUTTER_2";
	}

	return "NOT DEFINED";
}
