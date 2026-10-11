// Original: d:\build\custom\temp\code\client\projectg\wind.cpp
#define WIND_WMATH_INLINE
#include "wind.h"
#include "wview.h"
#include "SharedDoc.h"
#include "scenemanager.h"
#include "weather.h"
#include "../shared/classdefine.h"
#include "GolfDoc.h"
#include "fx.h"
#include "fxsequence.h"
#include "SoundManager.h"
#include "polysoup.h"
#include "slicestr.h"
#include "cardsystem.h"
#include "s5/Utilities.h"
#include "s5/classicserver/ClassicServer.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

extern LightSet g_lightset;

// The smaller/larger of two coordinates (wind.cpp-local: their line marks are wind.cpp's).
inline float MinCoord(float a, float b) { return a > b ? b : a; }
inline float MaxCoord(float a, float b) { return a < b ? b : a; }

// wind.cpp:26
CWind& Wind(void) {
	static CWind s_wind;
	return s_wind;
}

// wind.cpp:33
CWind::CWind(void) : m_unk68(0), m_unk6C(0.0f, 0.0f, 0.0f), m_unk78(0), m_unk7C(0), m_unk80(0xFFFFFF) {
	m_unk90 = true;

	m_unk88 = 1.0f;
	m_unk8C = 9.0f;

	m_intensity = 0.0f;
	m_direction = 0.0f;

	m_specialIntensity = -1.0f;
	m_specialDirection = 0.0f;

	m_activeSpecialWind = false; m_matrix = RotMat(-0.52359879f, 0);
	m_matrix.pivot = g_view->GetWidth() * -0.265625f * RotMat(-0.52359879f, 0).za;

	m_unk68 = 2;
	m_unk78 = m_unk7C = 0xFFFFFF;

	m_unk6C = WVector(0.0f, -1.0f, 1.0f).Normalize();
	m_pFxSequence = 0;
}

// wind.cpp:57
void CWind::SetSpecialWind(bool active) {
	if (m_activeSpecialWind == active)
		return;

	m_activeSpecialWind = active;
	if (active) {
		CFx::Instance()->UpdateExtWind(m_specialWind);
		g_audio->PlaySfx("\xb7\xb9\xc0\xcc\xc1\xae\xb9\xdf\xbb\xe7", 0, 0, "\xb7\xb9\xc0\xcc\xc1\xae\xb9\xdf\xbb\xe7", 0, 0.5f, 200.0f);
	} else {
		CFx::Instance()->UpdateExtWind(m_wind);
	}
}

// wind.cpp:73
void CWind::GenerateFx(bool generate) {
	if (!generate) {
		if (m_pFxSequence) {
			CFx::Instance()->CloseSequence(m_pFxSequence, false);
			m_pFxSequence = 0;
		}
		return;
	}

	if (m_pFxSequence)
		return;

	Attacher attacher;
	attacher.m_pMatrix[0] = &m_fxMatrix;
	attacher.m_pOffset[0] = &m_fxOffset;

	m_pFxSequence = (CFxSequence*)CFx::Instance()->Attach(attacher, "SilviaCannon.seq", false, 0);
	if (m_pFxSequence)
		m_pFxSequence->SetLight(g_lightset);

	g_audio->PlaySfx("\xbf\xa1\xb3\xca\xc1\xf6\xc1\xfd\xbe\xe0", 0, 0, "\xbf\xa1\xb3\xca\xc1\xf6\xc1\xfd\xbe\xe0", 0, 0.5f, 200.0f);
}

// wind.cpp:102
// Unreferenced in the exe and without a public: an anonymous-namespace function
// (cl 13.10 emits those even when unreferenced, a `static` one it drops).
// Lines 103-111 produce no code.
namespace {
float GetZeroWindValue(void) {
	return 0.0f;
}
}

// wind.cpp:117
void CWind::CheckSpecialWind(void) {
	WPolySoup::sArea* area = GOLFDOC()->m_pPolySoup->FindArea("silviacannon");
	if (area) {
		char name[64];
		WVector angles;
		WSliceStr desc(area->m_name);
		desc.Get("*fx", "%s %f %f %f", name, &angles.x, &angles.y, &angles.z);
		m_specialDirection = angles.y;
		// 0x4604 + hole * 4 == m_holeRandom[hole - 1]
		m_specialIntensity = (float)((&CSharedDoc::Instance()->m_holeRandom[0])[GOLFDOC()->m_currentHole - 1] & 3) + 6.0f;
		m_fxMatrix.Reset();
		m_fxMatrix.Rotate(angles);
		m_fxMatrix.pivot = (area->m_aabb.max + area->m_aabb.min) * 0.5f;
		m_fxOffset.Reset();
		m_specialWind = m_specialIntensity * RotMat(m_specialDirection, 1).xa;
	} else {
		m_specialIntensity = -1.0f;
	}

	m_activeSpecialWind = false;
	if (m_pFxSequence) {
		if (CFx::Instance()->FindSequence(m_pFxSequence) && m_pFxSequence->m_name == "SilviaCannon.seq")
			CFx::Instance()->CloseSequence(m_pFxSequence, false);

		m_pFxSequence = 0;
	}
}

// wind.cpp:380
float CWind::GetGlobalIntensity(void) {
	if (!m_unk90)
		return 0.0f;

	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000006 || PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000020) {
		if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty()))
			return m_intensity - floor(m_intensity) + 1.0f;

		return 1.0f;
	}

	if (IsLocalContent((localContentType_t)0x51) && CSharedDoc::Instance()->m_gameMode == 1) {
		float intensity = m_intensity;

		if (m_activeSpecialWind)
			intensity = m_specialIntensity;

		intensity = intensity - CCardManager::Instance()->GetWindPowerDown();
		if (intensity < 1.0f)
			intensity = 1.0f;

		return intensity;
	}

	if (m_activeSpecialWind)
		return m_specialIntensity;

	return m_intensity;
}

// wind.cpp:428
float CWind::GetGlobalDirection(void) {
	if (m_activeSpecialWind)
		return m_specialDirection;

	if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000016)
		return m_direction * -1.0f;

	return m_direction;
}

// wind.cpp:439
void CWind::SetIntensity(float intensity) {
	if (CSharedDoc::Instance()->m_gameMode == 5) {
		if (m_activeSpecialWind) {
			WPolySoup::sArea* area = GOLFDOC()->m_pPolySoup->FindArea("silviacannon");
			if (area) {
				char name[64];
				WVector angles;
				WSliceStr desc(area->m_name);
				desc.Get("*fx", "%s %f %f %f", name, &angles.x, &angles.y, &angles.z);
				m_specialDirection = angles.y;
				m_specialIntensity = intensity;
				m_fxMatrix.Reset();
				m_fxMatrix.Rotate(angles);
				m_fxMatrix.pivot = (area->m_aabb.max + area->m_aabb.min) * 0.5f;
				m_fxOffset.Reset();
				m_specialWind = m_specialIntensity * RotMat(m_specialDirection, 1).xa;
			}
			return;
		}
	}

	m_intensity = intensity;
	m_wind = m_intensity * RotMat(m_direction, 1).za;

	if (IsLocalContent((localContentType_t)0x87))
		m_globalWind = m_wind;

	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it) {
		it->m_wind.Normalize();
		it->m_wind *= it->m_flag == 1 ? m_intensity * it->m_intensity : m_intensity + it->m_intensity;
	}
}

// wind.cpp:488
void CWind::SetDirection(float direction) {
	if (CSharedDoc::Instance()->m_gameMode == 5) {
		if (m_activeSpecialWind) {
			WPolySoup::sArea* area = GOLFDOC()->m_pPolySoup->FindArea("silviacannon");
			if (area) {
				char name[64];
				WVector angles;
				WSliceStr desc(area->m_name);
				desc.Get("*fx", "%s %f %f %f", name, &angles.x, &angles.y, &angles.z);
				m_specialDirection = direction;
				m_fxMatrix.Reset();
				m_fxMatrix.Rotate(angles);
				m_fxMatrix.pivot = (area->m_aabb.max + area->m_aabb.min) * 0.5f;
				m_fxOffset.Reset();
				m_specialWind = m_specialIntensity * RotMat(angles.y, 1).xa;
			}
			return;
		}
	}

	m_direction = direction;
	m_wind = m_intensity * RotMat(m_direction, 1).za;

	if (IsLocalContent((localContentType_t)0x87))
		m_globalWind = m_wind;
}

// wind.cpp:290
void CWind::SetWind(const WVector& wind) {
	m_direction = CalcDeltaAngle(wind, WVector::UNIT_POS_Z);
	SetIntensity(wind.Magnitude());

	if (CSceneManager::Instance()->m_pWeather) {
		CSceneManager::Instance()->m_pWeather->UpdateWind();
	}
}

// wind.cpp:299
void CWind::PlusWind(const WVector& wind) {
	SetWind(m_wind + wind);
}

// wind.cpp:304
WVector CWind::GetGlobalWind(void) {
	if (!m_unk90)
		return WVector::ZERO;

	WVector wind;

	if (IsLocalContent((localContentType_t)0x87)) {
		if (WisEqual(m_globalWind, m_wind, 0.00001f)) {
			if (m_activeSpecialWind)
				wind = m_specialWind;
			else
				wind = m_wind;
		} else {
			m_wind = m_globalWind;

			if (m_activeSpecialWind)
				wind = m_specialWind;
			else
				wind = m_wind;
		}
	} else {
		if (m_activeSpecialWind)
			wind = m_specialWind;
		else
			wind = m_wind;
	}

	if (IsLocalContent((localContentType_t)0x51) && CSharedDoc::Instance()->m_gameMode == 1) {
		float intensity = wind.Magnitude();
		intensity = intensity - CCardManager::Instance()->GetWindPowerDown();
		if (intensity < 1.0f)
			intensity = 1.0f;

		wind.Normalize();
		wind *= intensity;
	}

	if (GOLFDOC()->GetPlayerNum()) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000006 || PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000020) {
			wind.Normalize();
			if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty()))
				wind *= m_intensity - floor(m_intensity) + 1.0f;
		} else if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000016) {
			wind.Negate();
		}
	}

	return wind;
}

// wind.cpp:529
void CWind::MakeWindList(void) {
	if (!OnlinePlay() && CSharedDoc::Instance()->m_playingMode != (eReplayModeType)2)
		Wind().Init(0xFF, 0xFF, 0, 1);

	m_windList.clear();

	for (int i = 0; i < GOLFDOC()->m_pPolySoup->GetAreaNum(); i++) {
		WPolySoup::sArea* area = GOLFDOC()->m_pPolySoup->GetArea(i);

		if (area->m_type == 4)
			AddWindArea(area->m_aabb, area->m_name);
	}
}

// wind.cpp:546
void CWind::AddWindArea(const Waabb& area, char* desc) {
	sWind w;
	w.m_type = m_windList.size();

	w.m_area = area;
	w.m_linked = -1;
	w.m_yaw = 0.0f;
	w.m_pitch = 0.0f;

	char level[8] = "hard";

	switch (desc[0]) {
	case '*':
		sscanf(desc + 1, "%f %f %f %s", &w.m_intensity, &w.m_yaw, &w.m_pitch, level);

		w.m_flag = 1; w.m_wind = (RotMat(w.m_pitch * 0.017453292f, 0) * RotMat(w.m_yaw * 0.017453292f, 1)).za * (m_intensity * w.m_intensity);
		break;

	case '+':
		sscanf(desc + 1, "%f %f %f %s", &w.m_intensity, &w.m_yaw, &w.m_pitch, level);

		w.m_flag = 0; w.m_wind = (RotMat(w.m_pitch * 0.017453292f, 0) * RotMat(w.m_yaw * 0.017453292f, 1)).za * (m_intensity + w.m_intensity);
		break;

	case '-':
		sscanf(desc + 1, "%f %f %f %s", &w.m_intensity, &w.m_yaw, &w.m_pitch, level);

		{ float intensity = m_intensity - w.m_intensity; w.m_flag = 2; w.m_wind = (RotMat(w.m_pitch * 0.017453292f, 0) * RotMat(w.m_yaw * 0.017453292f, 1)).za * (0.0f < intensity ? intensity : 0.0f); }
		break;

	case '=':
		sscanf(desc + 1, "%f %f %f %s", &w.m_intensity, &w.m_yaw, &w.m_pitch, level);

		w.m_flag = 3; w.m_wind = (RotMat(w.m_pitch * 0.017453292f, 0) * RotMat(w.m_yaw * 0.017453292f, 1)).za * w.m_intensity;
		break;

	default:
		sscanf(desc, "%f %f %f %s", &w.m_intensity, &w.m_yaw, &w.m_pitch, level);

		w.m_flag = 0; w.m_wind = (RotMat(w.m_pitch * 0.017453292f, 0) * RotMat(w.m_yaw * 0.017453292f, 1)).za * (m_intensity + w.m_intensity);
		break;
	}

	IFF_STRUCT::sCourse* course = CSharedDoc::Instance()->m_pCurCourse;
	switch (course ? course->courseFlags >> 4 : 0) {
	case 0:
		if (strcmpi(level, "normal") != 0)
			return;
		break;

	case 1:
		if (strcmpi(level, "hard") == 0)
			return;
		break;

	case 2:
		if (strcmpi(level, "easy") == 0)
			return;
		break;
	}

	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it) {
		int i;
		for (i = 0; i < 3; ++i) {
			if (w.m_area.min.p[i] > it->m_area.max.p[i] || w.m_area.max.p[i] < it->m_area.min.p[i])
				break;
		}
		if (i == 3) {
			w.m_linked = it->m_type;
			it->m_linked = w.m_type;
		}
	}

	m_windList.push_back(w);
}

// wind.cpp:626
bool CWind::IsInclude(const Waabb& aabb, const WVector& pos) {
	return aabb.IsInclude(pos);
}

// wind.cpp:631
WVector CWind::GetWind(const WVector& pos) {
	if (!m_unk90)
		return WVector::ZERO;

	WVector wind;

	if (IsLocalContent((localContentType_t)0x87)) {
		if (!WisEqual(m_globalWind, m_wind, 0.00001f))
			m_wind = m_globalWind;

		if (m_activeSpecialWind)
			wind = m_specialWind;
		else
			wind = m_wind;
	} else {
		if (m_activeSpecialWind)
			wind = m_specialWind;
		else
			wind = m_wind;
	}

	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it) {
		if (it->m_area.min.x > pos.x || pos.x > it->m_area.max.x
			|| it->m_area.min.y > pos.y || pos.y > it->m_area.max.y
			|| it->m_area.min.z > pos.z || pos.z > it->m_area.max.z)
			continue;

		if (it->m_linked >= 0) {
			sWind* other = &m_windList[it->m_linked];
			if (other->m_area.IsInclude(pos)) {
				WVector c1 = (it->m_area.min + it->m_area.max) * 0.5f - pos;
				WVector c2 = (other->m_area.min + other->m_area.max) * 0.5f - pos;
				WVector s1 = it->m_area.max - it->m_area.min;
				WVector s2 = other->m_area.max - other->m_area.min;

				float w1 = 1.0f - (c1.Magnitude() * 2.0f) / s1.Magnitude();
				float w2 = 1.0f - (c2.Magnitude() * 2.0f) / s2.Magnitude();
				float inv = 1.0f / (w1 + w2);

				wind = other->m_wind * (inv * w2) + it->m_wind * (inv * w1);
				break;
			}
		}

		wind = it->m_wind;
		break;
	}

	if (IsLocalContent((localContentType_t)0x51) && CSharedDoc::Instance()->m_gameMode == 1) {
		float intensity = wind.Magnitude();
		intensity = intensity - CCardManager::Instance()->GetWindPowerDown();
		if (intensity < 1.0f)
			intensity = 1.0f;

		wind.Normalize();
		wind *= intensity;
	}

	if (GOLFDOC()->GetPlayerNum()) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000006 || PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000020) {
			wind.Normalize();
			if (S5::CLASSICSRV::IsClassicServer(S5::UTIL::GetServerProperty()))
				wind *= m_intensity - floor(m_intensity) + 1.0f;
		} else if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000016) {
			wind.Negate();
		}
	}

	return wind;
}

// wind.cpp:731
bool CWind::IsInTurbulence(const WVector& pos) {
	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it) {
		if (it->m_area.IsInclude(pos))
			return true;
	}
	return false;
}

// wind.cpp:743
bool CWind::IsInTurbulence(const WVector& pos, const WVector& dir) {
	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it) {
		if (MinCoord(pos.x, pos.x + dir.x) > it->m_area.max.x || MaxCoord(pos.x, pos.x + dir.x) < it->m_area.min.x
			|| MinCoord(pos.z, dir.z + pos.z) > it->m_area.max.z || MaxCoord(pos.z, dir.z + pos.z) < it->m_area.min.z)
			continue;
		return true;
	}
	return false;
}

// wind.cpp:797
void CWind::DrawWindArea(WView* view) {
	for (std::vector<sWind>::iterator it = m_windList.begin(); it != m_windList.end(); ++it)
		view->DrawAABB(it->m_area, 0xFFFF0000, false, 0);
}

// wind.cpp:804
bool CWind::IsGlobalWind_Compare(void) {
	return WisEqual(m_wind, m_globalWind, 0.00001f) ? true : false;
}
