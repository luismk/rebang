// Original: d:\build\custom\temp\code\client\projectg\rival.cpp
#include "rival.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "club.h"
#include "golfball.h"
#include "quadtree.h"
#include "autotarget.h"
#include "wind.h"

#include <math.h>

float Random(float min, float max, int seed);

// The bar power of the rival's shot at a power rate of 1 (no symbol).
extern float data_00af23f4;

template <> CRival* WSingleton<CRival>::m_pInstance = NULL;

// rival.cpp:21
CRival::CRival(void) {
	m_state = 0;
	m_lieSlope = 0;
	m_lieAngle = 0;
	m_result = 0;
}

// rival.cpp:29
CRival::~CRival(void) {
}

// rival.cpp:33
void CRival::Shot(void) {
	float rateError = m_powerRate - 1.0f;
	if ((rateError < 0.0f ? -rateError : rateError) < 0.00001f) {
		GOLFDOC()->GetBar().m_stopped = true;
	}

	GOLFDOC()->GetBar().m_power = data_00af23f4 * m_powerRate + BAR_START;
	GOLFDOC()->GetBar().m_position = Random(-10.0f, 10.0f, -1) + GOLFDOC()->GetBar().m_startPosition;

	float error = GOLFDOC()->GetBar().m_position - BAR_START;
	error = error > 0.0f ? error : -error;

	if (error < 2.0f) {
		GOLFDOC()->GetBar().m_accuracyGrade = 4;
	} else if (error < GolfClub().GetNarrowAccuracy()) {
		GOLFDOC()->GetBar().m_accuracyGrade = 3;
	} else if (error < GolfClub().GetWideAccuracy()) {
		GOLFDOC()->GetBar().m_accuracyGrade = 2;
	} else {
		GOLFDOC()->GetBar().m_accuracyGrade = 1;
	}
}

// rival.cpp:53
void CRival::SetVariable(CGolfDoc *pGolfDoc) {
	float maxPower = GetPVS().GetMaxPowerOnGround(GolfBall().m_pos);

	if (m_result != 0) {
		GolfClub().SetAccuracy();

		WVector miss = m_expectedPos - m_pos;

		GolfClub().SetClub((unsigned char)m_clubIndex);

		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_holes[GOLFDOC()->m_currentHole - 1] == 3) {
			if (m_result != 1)
				goto aim;

			int index = m_clubIndex + 2;
			if (index > 12)
				index = 12;
			m_clubIndex = index;
			GolfClub().SetClub((unsigned char)m_clubIndex);

			m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
		} else {
			switch (m_result) {
			case 1: {
				// TODO: (PLAYER(...)->byte 0x4D8 > 1 ? 3 : 2), sPlayerData 0x4D8 is PADDING in GolfDoc.h
				int index = m_clubIndex + 2;
				if (index > 12)
					index = 12;
				m_clubIndex = index;
				GolfClub().SetClub((unsigned char)m_clubIndex);

				m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
				m_pos = GolfBall().m_pos + maxPower * (m_pos - GolfBall().m_pos);
				return;
			}
			case 2:
				// TODO: if (PLAYER(...)->byte 0x4D8 == 1) goto selectClub; (sPlayerData 0x4D8)
				break;
			case 3:
				// TODO: if (PLAYER(...)->byte 0x4D8 == 1) goto aim; (sPlayerData 0x4D8)
				break;
			case 4:
				m_aimAngle = CalcDeltaAngle(m_pos - GolfBall().m_pos - miss, WVector::UNIT_POS_Z);
				m_pos = GolfBall().m_pos + WVector::UNIT_POS_Z * (GolfClub().GetRange(NULL) * 3.2f) * RotMat(m_aimAngle, 1);
				break;
			}

			// TODO: if (PLAYER(...)->byte 0x4D8 <= 1) return; (sPlayerData 0x4D8)
			int index = m_clubIndex + 3;
			if (index > 12)
				index = 12;
			m_clubIndex = index;
			GolfClub().SetClub((unsigned char)m_clubIndex);

			m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
		}

		m_pos = GolfBall().m_pos + (m_pos - GolfBall().m_pos) * maxPower;
		return;
	}

	GolfClub().ClearPointFactor();
	GolfClub().SelectBestClub();

	m_clubIndex = GolfClub().GetIndex();

	for (;;) {
		m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
		m_pos = GolfBall().m_pos + (m_pos - GolfBall().m_pos) * (GolfClub().GetType() == 3 ? 1.0f : maxPower);

		int groundResult;
		GetPVS().GetGroundPoint(m_pos, true, &groundResult, true);

		if (groundResult < 0)
			GolfClub().SetPointFactor(0);
		else if (GetPVS().GetGroundType(m_pos, true, NULL) == 4)
			GolfClub().SetPointFactor(0.8f);
		else if (GetPVS().GetGroundType(m_pos, true, NULL) == 3)
			GolfClub().SetPointFactor(0.6f);
		else
			break;

		GolfClub().SelectBestClub();

		if (GolfClub().GetIndex() == m_clubIndex)
			break;

		m_clubIndex = GolfClub().GetIndex();

		if (GolfClub().GetIndex() == 12) {
			m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
			m_pos = GolfBall().m_pos + (m_pos - GolfBall().m_pos) * (GolfClub().GetType() == 3 ? 1.0f : maxPower);

			GetPVS().GetGroundPoint(m_pos, true, &groundResult, true);

			if (groundResult < 0) {
				GolfClub().SetPointFactor(0);
				GolfClub().SelectBestClub();
				m_pos = pGolfDoc->m_pAutoTarget->GetPoint(NULL);
				m_pos = GolfBall().m_pos + (GolfClub().GetType() == 3 ? 1.0f : maxPower) * (m_pos - GolfBall().m_pos);
				m_clubIndex = GolfClub().GetIndex();
			}
			break;
		}
	}

	GolfClub().ClearPointFactor();

aim:
	WVector dir(m_pos.x - GolfBall().m_pos.x, 0, m_pos.z - GolfBall().m_pos.z);
	dir.Normalize();

	if (GolfClub().GetType() == 3) {
		WVector slope = GetGreenSlope() * 5.0f;

		float angle = CalcDeltaAngle(m_pos - GolfBall().m_pos - slope, WVector::UNIT_POS_Z);
		m_pos = GolfBall().m_pos + WVector::UNIT_POS_Z * RotMat(angle, 1);

		WVector toCup = GOLFDOC()->GetHoleData().m_cupPos - GolfBall().m_pos;
		toCup.y = 0;

		float rate = sqrtf(toCup.x * toCup.x + toCup.z * toCup.z) / (GolfClub().GetRange(NULL) * 3.2f);
		if (rate > 1.0f)
			rate = 1.0f;
		m_powerRate = rate;
		return;
	}

	float windScale;
	if (GolfClub().GetType() == 2) {
		if (GolfClub().GetApproachMode() > 2)
			windScale = m_powerRate * 3.0f;
		else
			windScale = 1.0f;
	} else {
		windScale = 3.0f;
	}

	m_aimAngle = CalcDeltaAngle(m_pos - GolfBall().m_pos - windScale * Wind().GetGlobalWind(), WVector::UNIT_POS_Z);
	m_pos = GolfBall().m_pos + WVector::UNIT_POS_Z * (GolfClub().GetRange(NULL) * 3.2f) * RotMat(m_aimAngle, 1);

	// TODO rival.cpp:234-342: power rate by the hole score / m_result, wind and
	// cup corrections, GetLie/GetRange, then the RayIntersectAll obstacle tests
	// (RotMat +-0.1745f) that move the aim or raise the club (second pass).
}

// rival.cpp:350
void CRival::SetResult(void) {
	m_expectedPos = GolfBall().PreData(GolfBall().m_preDataCount)->m_pos;

	if (GolfBall().m_landed || GolfBall().m_bounceCount <= 4) {
		if (GetPVS().GetObjColNum() > 0) {
			m_result = 1;
		} else if (GolfBall().m_lie == 2 || GolfBall().m_lie == 3) {
			m_result = 2;
		} else {
			m_result = GolfBall().m_lie == 1 ? 3 : 4;
		}

		m_lieSlope = GolfBall().m_lieSlope;
		m_lieAngle = GolfBall().m_lieAngle;
	} else {
		m_result = 0;
		m_lieSlope = 0;
		m_lieAngle = 0;
	}
}

// rival.cpp:378
WVector CRival::GetGreenSlope(void) {
	WVector cupPos = GOLFDOC()->GetHoleData().m_cupPos;
	WVector dir = cupPos - GolfBall().m_pos;

	dir.y = 0;
	int stepCount = (int)(sqrtf(dir.x * dir.x + dir.z * dir.z) * 0.3125f);
	dir.Normalize();

	WVector slope(0, 0, 0);
	WVector pos = GolfBall().m_pos;
	for (int i = 0; i < stepCount; i++) {
		pos += dir * 3.2f;
		slope += GetPVS().GetMeshNormal(pos);
	}
	return slope;
}
