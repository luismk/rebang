// Original: d:\build\custom\temp\code\client\projectg\camerawork.cpp
#define CAMERAWORK_WMATH_INLINE
#define CAMERAWORK_WPUPPET_INLINE
#include "camerawork.h"
#include "commonutil.h"
#include "SoundManager.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "wview.h"
#include "golfball.h"
#include "quadtree.h"
#include "scenemanager.h"
#include "wpuppet.h"
#include "wbone.h"
#include "wutil.h"
#include "club.h"
#include "wresrcmng.h"
#include <stdlib.h>


// Nameless data: the name ParsePuppetScript cuts out of a script, and the
// name it is compared with (a char * of another unit; size not established).
static char data_af1f60[256];
extern char *data_a7ee68;

extern WMatrix g_camera;

// Nameless .rdata floats of Wangreal's math (pi, pi/2, degrees to radians,
// radians to degrees, epsilon), read through memory rather than literals.
extern "C" const float data_9eb420[5];

// One camera of the selection tables ReplayCamera cycles through (header
// not identified; 8 bytes).
struct sCameraSelectionData {
	int m_weight;                   // 0x0, the camera is skipped when <= 0
	bool m_checkBallVisibility;     // 0x4, drive cameras: switch away when the ball is hidden
	PADDING(0x5, 0x8);
};
CHECK_SIZE(sCameraSelectionData, 0x8);
CHECK_FIELD(sCameraSelectionData, m_checkBallVisibility, 0x4);

extern sCameraSelectionData puttTable[];
extern sCameraSelectionData driveTable[];

// Minimum helper of an unidentified header (also local in PetBody.cpp).
template <class T> T MinOf(T a, T b) { return a > b ? b : a; }
template <class T> T MaxOf(T a, T b) { return a < b ? b : a; }

// camerawork.cpp:136
CCameraWork::~CCameraWork() {
	m_cameraQueue.clear();

	for (int i = 0; i < 4; i++) {
		if (g_resrcmng && m_holeInCams[i].m_pPuppet) { g_resrcmng->Release(m_holeInCams[i].m_pPuppet, false); m_holeInCams[i].m_pPuppet = 0; }
	}

	if (g_resrcmng && m_pCameraPuppet) { g_resrcmng->Release(m_pCameraPuppet, false); m_pCameraPuppet = 0; }
}

// camerawork.cpp:205
WPuppet *CCameraWork::FindHoleInCam(unsigned long id) {
	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (m_holeInCams[i].m_id == id)
			return m_holeInCams[i].m_pPuppet;
	}
	return 0;
}

// camerawork.cpp:221
void CCameraWork::ReplayCamera(bool keep) {
	if (!keep)
		m_replayCameras.clear();

	int index;

	if (GolfClub().GetType() == 3) {
		index = m_cameraIndex - 100;

		do {
			index = (index + 1) % m_puttCameraCount;
		} while (puttTable[index].m_weight <= 0);

		index += 100;
	} else {
		index = m_cameraIndex;

		do {
			index = (index + 1) % m_driveCameraCount;
		} while (driveTable[index].m_weight <= 0);
	}

	m_inputUsed = false; m_inputYaw = 0.0f; m_inputPitch = 0.0f; m_inputZoom = 0.0f;
	m_cameraIndex = index;
	m_nextCameraType = -1;
	m_stateFlag = false;
	m_keepInputOffsets = false;
	m_zoomAngle = 0.95993108f;
	m_replayPos = 0;
	SetCamera(index, true);
	CheckCameraType(true);
	InitCtrlPoints(m_cameraType);
}

// camerawork.cpp:340
void CCameraWork::SetCaddieCamera(const WMatrix &matrix, const WVector &target) {
	m_keepInputOffsets = false;
	m_destFollowsGround = false;
	m_zoomAngle = 0.95993108f;
	m_baseMatrix = matrix;
	SetCamera(0xCA, true);

	m_baseMatrix.GetRotMatrix(&m_baseRotation);
	WVector dir;
	dir = WVector::UNIT_POS_Z * m_baseRotation;
	WVector toTarget;
	toTarget = target - GolfBall().m_pos;
	m_swingAngle = CalcDeltaAngle(toTarget, dir);
	WVector mid = toTarget * 0.5f + GolfBall().m_pos;
	m_sourceCamera = WVector(mid.x, mid.y + 10.0f, mid.z);
	m_introFrom = m_sourceCamera;
	m_introTo = matrix.pivot;
	m_baseMatrixCopy = m_baseMatrix;
}

// camerawork.cpp:475
WVector CCameraWork::GetSourceCamera(void) {
	return m_sourceCamera;
}

// camerawork.cpp:479
WVector CCameraWork::GetDestCamera(void) {
	return m_destCamera;
}

// camerawork.cpp:483
void CCameraWork::SetDestCamera(const WVector &dest) {
	m_destCamera = dest;
}

// camerawork.cpp:488
void CCameraWork::SetInstalledCamera(const WVector &source) {
	SetCamera(0x15, false);
	m_sourceCamera = source;

	m_installed = true;
}

// camerawork.cpp:497
void CCameraWork::UninstallCamera(void) {
	m_installed = false;
}

// camerawork.cpp:1475
void CCameraWork::InitVars(void) {
	m_duration = (float)GolfBall().m_eventPreDataIndex[1];
	m_phase1Time = (float)GolfBall().m_eventPreDataIndex[0];
	m_phase2Time = (float)GolfBall().m_preDataCount;
	m_processedFrame = -2;
	m_preDataFrame = 0;
	m_cameraTime = 0.0f;
	m_panDone = false;
	m_ballSoundState = 0;
	m_lastQuakeTime = 0.0f;
	m_ballSoundTime = 0.0f;
	m_transitionTime = -1.0f;
	m_firstCamera = true;
	m_riseHeight = 5.0f;
	m_inputMode = 2;

	if (m_fixedCameraIndex % 6 == 0 && GetPVS().GetObjColNum() == 0 && GolfClub().GetType() != 3 &&
		GetPVS().GetGroundType(GolfBall().PreData(GolfBall().m_preDataCount)->m_pos, true, 0) == 2)
		m_landsInWater = true;
	else
		m_landsInWater = false;

	m_stateFlag = false;

	m_holeInReached = false;
	m_flags = 1;
	m_ballStartPos = GolfBall().m_pos;

	g_audio->StopSfx("\xb0\xf8_\xb9\xd9\xb6\xf7");
}

// camerawork.cpp:1917
bool CCameraWork::CheckGround(WVector &pos, WVector &dest) {
	float y = Max(pos.y, GetPVS().GetBasePosY(pos, GOLFDOC()->GetCameraData().m_groundOffset));

	if (m_destFollowsGround)
		dest.y += y - pos.y;

	if (y > pos.y) {
		pos.y = y;
		return true;
	}

	return false;
}

// camerawork.cpp:1934
void CCameraWork::ScriptProcess(w_motion_data *motion, float from, float to) {
	if (m_pScriptPuppet == 0)
		return;

	if (!(from <= motion->e) || from == to)
		return;

	from *= 30.0f;
	to = MinOf(motion->e, to) * 30.0f;
	int first = Ceil(from, 0.03f);
	int last = to == (float)floor(to) ? float2int(to) - 1 : float2int((float)floor(to));

	if (first > last)
		return;

	int start, end;

	if (m_pScriptPuppet->CheckFrameData(first, last, start, end)) {
		for (int i = start; i <= end; i++)
			ParsePuppetScript(m_pScriptPuppet->GetFrameData(i));
	}
}

// camerawork.cpp:1960
void CCameraWork::ParsePuppetScript(char *script) {
	if (script) {
		char *p = strchr(script, '*');

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

			strncpy(data_af1f60, p, len);
			data_af1f60[len] = 0;
			p += len;
			strcmpi(data_af1f60, data_a7ee68);
			p = strchr(p, '*');
		}
	}
}

// camerawork.cpp:2019
bool CCameraWork::CheckState(void) {
	if (m_workTime >= m_phase2Time)
		return true;

	if (GolfBall().m_holeIn == 1) {
		WVector dist = GOLFDOC()->GetHoleData().m_cupPos; dist = GolfBall().m_pos - dist;

		if (!m_holeInReached) {
			if (sqrt(dist.x * dist.x + dist.z * dist.z) < GolfBall().GetRadius() * 3.0f && dist.y < 0.0f)
				m_holeInReached = true;
		}

		if (m_holeInReached)
			return true;
	}

	return false;
}

// camerawork.cpp:2098
void CCameraWork::StopBallSound(void) {
	m_ballSoundState = 2;
	g_audio->StopSfx("\xb0\xf8_\xb9\xd9\xb6\xf7");
}

// camerawork.cpp:2104
void CCameraWork::InterpolateBezier(WVector *out, float t, WVector *points, int count) {
	float t2 = t * t;
	float s = 1.0f - t;
	float s2 = s * s;

	if (count == 3) {
		*out = s2 * points[0] + s * t * 2.0f * points[1] + t2 * points[2];
	} else if (count == 4) {
		*out = s2 * s * points[0] + s2 * t * 3.0f * points[1] + s * t2 * 3.0f * points[2] + t2 * t * points[3];
	}
}

// camerawork.cpp:2129
void CCameraWork::CheckDriveCameraBallVisibilityAndSetBallVisibilitySafeCamera(void) {
	if (m_cameraType >= 100)
		return;

	if (!driveTable[m_cameraType].m_checkBallVisibility)
		return;

	WPolySoup::sModel *pModel = GOLFDOC()->m_pPolySoup->FindModel(0);
	WVector size = pModel->m_aabb.max - pModel->m_aabb.min;

	WVector toBall = GolfBall().m_pos - m_sourceCamera;

	if (toBall.Magnitude() <= size.Magnitude() * 2.0f && GetPVS().RayIntersectAll(m_sourceCamera, toBall, true) == 1.0f) {
		m_ballHiddenFrame = -1;
	} else if (m_ballHiddenFrame < 0) {
		m_ballHiddenFrame = m_preDataFrame;
	} else if (m_preDataFrame - m_ballHiddenFrame > 50) {
		m_installed = false;
		unsigned int i = rand() % m_safeCameras.size();

		if (m_safeCameras[i] == m_cameraType) {
			i = (i + 1) % m_safeCameras.size();

			if (m_safeCameras[i] == m_cameraType)
				return;
		}

		SetCamera(m_safeCameras[i], false);
	}
}

// camerawork.cpp:2178
void CCameraWork::EarthQuake(void) {
	if (m_cameraTime - m_lastQuakeTime < 0.1f)
		return;

	m_lastQuakeTime = m_cameraTime;

	int strength = 6 - (int)((m_cameraTime - m_quakeStartTime) * 6.0f);
	if (strength == 0)
		strength = 1;
	float half = strength * 0.05f;

	WVector shake;
	shake.x = (rand() % strength) * 0.1f - half;
	shake.y = (rand() % strength) * 0.1f - half;
	shake.z = (rand() % strength) * 0.1f - half;
	m_sourceCamera += shake * 0.1f;
}

// camerawork.cpp:2202
void CCameraWork::FollowCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);

	if (m_workTime < m_phase1Time) {
		m_destCamera = GolfBall().m_pos;
	} else if (m_workTime < m_phase2Time) {
		float s = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time);
		m_destCamera = GolfBall().m_pos + WVector(0.0f, s * m_riseHeight, 0.0f);
	}
}

// camerawork.cpp:2222
void CCameraWork::LookBackCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);

	if (m_flags & 0x20000000) {
		if (m_workTime < (float)m_workLength - 40.0f)
			m_destCamera = GolfBall().m_pos;
		else
			m_nextCameraType = 14;
	} else {
		if (m_workTime < m_phase1Time * 0.65f &&
			(m_sourceCamera - m_destCamera).Magnitude() > 10.0f)
			m_destCamera = GolfBall().m_pos;
		else
			m_nextCameraType = 2;
	}
}

// camerawork.cpp:2246
void CCameraWork::Rail_or_SideCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);
	m_zoomAngle = 0.95993108f - data_9eb420[2] * (sinf(t * data_9eb420[0]) * 25.0f);

	if (m_workTime < m_phase1Time)
		m_destCamera = GolfBall().m_pos;
	else if (m_workTime < m_phase2Time) {
		float s = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time);
		m_destCamera = GolfBall().m_pos + WVector(0.0f, s * m_riseHeight, 0.0f);
	}
}

// camerawork.cpp:2335
void CCameraWork::DropCamera(float elapsed, float time) {
	if (m_workTime < m_phase1Time)
		m_destCamera = GolfBall().m_pos;
	else if (m_workTime < m_phase2Time) {
		float t = time + m_workTime - m_phase1Time;
		InterpolateBezier(&m_destCamera, t / (m_phase2Time - m_phase1Time), m_bezierPoints, 4);
	}
}

// camerawork.cpp:2433
void CCameraWork::CircleCamera(float elapsed, float time) {
	if (m_preDataFrame < GolfBall().m_eventPreDataIndex[13]) {
		if (m_bankAngle > 0.0f) {
			m_bankAngle -= elapsed * 1.0471976f;
			if (m_bankAngle < 0.0f)
				m_bankAngle = 0.0f;
		}

		WVector dir = GolfBall().m_pos - m_sourceCamera;

		dir.y = 0.0f;
		m_introDistance = sqrt(dir.z * dir.z + dir.x * dir.x); dir.Normalize(); dir = WCrossProduct(dir, WVector::UNIT_POS_Y); m_sourceCamera += dir * (m_introDistance * data_9eb420[0]) * elapsed;
	} else {
		m_sourceCamera += GolfBall().m_velocity * 0.75f * elapsed;
		m_destCamera += GolfBall().m_velocity * 0.75f * elapsed;
		m_destCamera.y -= elapsed;
	}
}

// camerawork.cpp:2463
void CCameraWork::Chase_and_LookBackCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);
	m_zoomAngle = 0.95993108f - data_9eb420[2] * (sinf(t * data_9eb420[0]) * 25.0f);

	if (m_flags & 0x20000000) {
		if (m_workTime < (float)m_workLength - 50.0f)
			m_destCamera = GolfBall().m_pos;
		else
			m_nextCameraType = 14;
	} else {
		if (m_workTime < m_phase1Time)
			m_destCamera = GolfBall().m_pos;
		else if (m_workTime < m_phase2Time) {
			float s = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time);
			m_destCamera = GolfBall().m_pos + WVector(0.0f, s * m_riseHeight, 0.0f);
		}
	}
}

// camerawork.cpp:2492
void CCameraWork::ZoomInCamera(float elapsed, float time) {
	if (m_workTime < m_duration) {
		float t = (time + m_workTime) / m_duration;
		m_zoomAngle = 0.95993108f - data_9eb420[2] * (t * 35.0f);

		m_destCamera = GolfBall().m_pos;
		m_destCamera.y += (1.0f - t) * 2.0f;
	} else {
		m_zoomAngle = 0.95993108f; m_nextCameraType = (m_flags & 0x20000000) ? 14 : 11;
	}
}

// camerawork.cpp:2530
void CCameraWork::CaddyViewCamera(float elapsed, float time) {
	WPuppet *pCaddie = CSceneManager::Instance()->GetPuppet("\xc4\xb3\xb5\xf0");

	if (pCaddie == 0)
		return;

	WVector dir = pCaddie->GetMatrix().pivot - GolfBall().m_pos;
	dir.y = MaxOf(dir.y + 10.0f, 0.0f);
	dir.Normalize();

	WBone *pHead = pCaddie->FindBoneByTail("Head");
	if (pHead)
		m_sourceCamera = WVector(pHead->GetMatrix().pivot.x, pHead->GetMatrix().pivot.y + 5.0f, pHead->GetMatrix().pivot.z);
	else
		m_sourceCamera = WVector(pCaddie->GetMatrix().pivot.x, pCaddie->GetMatrix().pivot.y + 10.0f, pCaddie->GetMatrix().pivot.z);

	m_sourceCamera += dir * 35.0f + WCrossProduct(WVector::UNIT_POS_Y, dir) * 10.0f;
	m_destCamera = GolfBall().m_pos;
}

// camerawork.cpp:2551
void CCameraWork::Drop_and_ZoomCamera(float elapsed, float time) {
	if (m_workTime < m_phase1Time) {
		float t = (time + m_workTime - m_phase0Time) / (m_phase1Time - m_phase0Time);
		m_zoomAngle = 0.26179940f - t * -0.69813168f;

		m_destCamera = GolfBall().m_pos;
	} else if (m_workTime < m_phase2Time) {
		float t = time + m_workTime - m_phase1Time;
		InterpolateBezier(&m_destCamera, t / (m_phase2Time - m_phase1Time), m_bezierPoints, 4);
	}
}

// camerawork.cpp:2586
void CCameraWork::BezierCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);

	if (m_workTime < m_phase1Time) {
		m_destCamera = GolfBall().m_pos;
		m_destCamera.y += (1.0f - t) * 2.0f;
	} else if (m_workTime < m_phase2Time) {
		float s = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time);
		m_destCamera = GolfBall().m_pos + WVector(0.0f, s * m_riseHeight, 0.0f);
	}
}

// camerawork.cpp:2607
void CCameraWork::FollowWithSwingCamera(float elapsed, float time) {
	float t = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);
	InterpolateBezier(&m_sourceCamera, t, m_bezierPoints, 4);

	if (m_workTime < m_phase1Time)
		m_swingAngle -= elapsed * 0.034906585f;

	WVector offset = GolfBall().m_pos - m_sourceCamera; m_destCamera = offset * RotMat(m_swingAngle, 0) + m_sourceCamera;
}

// camerawork.cpp:2649
void CCameraWork::FromMidPointCamera(float elapsed, float time) {
	if (m_flags & 0x20000000) {
		if (m_workTime < (float)m_workLength - 40.0f)
			m_destCamera = GolfBall().m_pos;
		else
			m_nextCameraType = 14;
	} else if (m_workTime < m_phase1Time) {
		m_destCamera = GolfBall().m_pos;
	} else if (m_phase1Time > 250.0f) {
		m_nextCameraType = 4;
	} else {
		float t = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time + 1.0f);
		m_destCamera = GolfBall().m_pos + WVector(0.0f, t * 5.0f, 0.0f);
	}
}

// camerawork.cpp:2675
void CCameraWork::FromColPointCamera(float elapsed, float time) {
	if (m_workTime < m_phase1Time)
		m_destCamera = GolfBall().m_pos;
	else {
		float t = (time + m_workTime - m_phase1Time) / (m_phase2Time - m_phase1Time);
		m_destCamera = GolfBall().m_pos + WVector(0.0f, t * 5.0f, 0.0f);
	}
}

// camerawork.cpp:2689
void CCameraWork::PangyaACamera(float elapsed, float time) {
	if (m_cameraTime > 2.0f)
		SetCamera(0x15, true);
}

// camerawork.cpp:2695
void CCameraWork::FixedAtSideCamera(float elapsed, float time) {
	m_destCamera = GolfBall().m_pos;
}

// camerawork.cpp:2700
void CCameraWork::FromRandomPointCamera(float elapsed, float time) {
	time = (time + m_workTime - m_phase0Time) / (m_phase2Time - m_phase0Time);

	m_destCamera = (1.0f - time) * GolfBall().PreData(0)->m_pos + time * GolfBall().m_pos + WVector(0.0f, 3.0f, 0.0f);
}

// camerawork.cpp:2708
void CCameraWork::BankFromHeadCamera(float elapsed, float time) {
	time = (time + m_workTime - m_phase0Time) / (m_duration - m_phase0Time);

	m_bankAngle = (time * 60.0f - 30.0f) * data_9eb420[2];

	WVector dir = GolfBall().m_velocity;
	dir.Normalize();

	WVector side = WCrossProduct(WVector::UNIT_POS_Y, dir);
	side.Normalize();

	m_destCamera = side * (time * 0.5f - 1.0f) + GolfBall().m_pos;
	m_sourceCamera = dir * ((1.0f - time) * 50.0f) + m_destCamera;

	if (m_workTime > m_duration)
		m_nextCameraType = 0;
}

// camerawork.cpp:2789
void CCameraWork::TomahawkACamera(float elapsed, float time) {
	if (m_cameraTime < 2.0f) {
		CGolfBall &ball = GolfBall();  // unused; the original still makes the call

		WMatrix view = MakeCameraMatrix(GolfBall().m_pos, GolfBall().m_pos - GolfBall().m_velocity);

		m_sourceCamera = WVector(-0.7f, -0.3f, m_cameraTime - 3.0f) * view;
		m_destCamera = GolfBall().PreData(m_preDataFrame - 25 > 0 ? m_preDataFrame - 25 : 0)->m_pos;
	} else {
		if (m_workTime > m_duration)
			m_nextCameraType = 4;
		else
			m_nextCameraType = 0x13;
	}
}

// camerawork.cpp:2888
void CCameraWork::TopViewCamera(float elapsed, float time) {
	m_destCamera = m_destCamera * 0.9f + GolfBall().m_pos * 0.1f;

	WVector dir = m_baseMatrix.pivot - GolfBall().PreData(GolfBall().m_preDataCount)->m_pos;
	float distance = GolfBall().m_velocity.Magnitude() * 2.0f;
	distance = distance < 100.0f ? 100.0f : (distance > 150.0f ? 150.0f : distance);

	dir.y = 0.0f; dir.Normalize();

	dir.y = 2.0f; dir.Normalize();

	WVector target = GolfBall().m_pos + dir * distance;

	m_sourceCamera = m_sourceCamera * 0.9f + target * 0.1f; InterpolateBezier(&m_sourceCamera, 0.5f, &m_sourceCamera, 3);
}

// camerawork.cpp:2945
void CCameraWork::SideCamera(float elapsed, float time) {
	WVector velocity = GolfBall().m_velocity;

	velocity.y = 0.0f;
	m_sourceCamera += velocity * elapsed * 0.8f;
	m_destCamera = GolfBall().m_pos;
	m_destCamera.y += 2.5f;
}

// camerawork.cpp:2955
void CCameraWork::BallSideCamera(float elapsed, float time) {
	WVector back = GolfBall().m_velocity;
	WVector side = WCrossProduct(WVector::UNIT_POS_Y, back);
	back = back.Normalize() * GolfBall().GetRadius() * 2.0f;
	side = side.Normalize() * GolfBall().GetRadius() * 1.2f;

	float t = MinOf((time + m_workTime) / m_phase2Time, 1.0f);

	m_destCamera = GolfBall().m_pos + side - back;
	m_sourceCamera = (m_destCamera - m_baseMatrixCopy.pivot) * t + m_baseMatrixCopy.pivot - back;
}

// camerawork.cpp:2971
void CCameraWork::FollowBallCamera(float elapsed, float time) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos; WVector toBall = GolfBall().m_pos - cup;
	toBall.y = 0.0f;
	float cupDistance2 = toBall.x * toBall.x + toBall.z * toBall.z;

	WVector velocity = GolfBall().m_velocity;

	velocity.y = 0.0f;

	if (sqrt(velocity.z * velocity.z + velocity.x * velocity.x) > 1.0f && sqrt(cupDistance2) >= GolfBall().GetRadius() * 3.0f) { m_destCamera = GetPVS().GetGroundPoint(GolfBall().m_pos, false, 0, false); m_destCamera.y += GolfBall().GetRadius(); m_sourceCamera = m_destCamera - velocity.Normalize() * 3.2f; m_sourceCamera.y += 1.0f; }
}

// camerawork.cpp:2990
void CCameraWork::FixedCamera(float elapsed, float time) {
	if (!g_view->InFrustumSafe(WSphere(GolfBall().m_pos, GolfBall().GetRadius())))
		SetCamera(m_fixedCameraIndex % 2 + 0x6d, true);
}

// camerawork.cpp:2997
void CCameraWork::PuttSecondACamera(float elapsed, float time) {
	m_sourceCamera.y -= elapsed * 9.0f;
	m_destCamera = GolfBall().m_pos;
}

// camerawork.cpp:3003
void CCameraWork::PuttSecondBCamera(float elapsed, float time) {
	if ((g_camera.pivot - WVector(GOLFDOC()->GetHoleData().m_cupPos)).Magnitude() < 16.0f) {
		m_pCameraProc = 0;
		return;
	}

	WVector dir = GolfBall().PreData(0)->m_pos - WVector(GOLFDOC()->GetHoleData().m_cupPos);
	dir.y = 0.0f;
	dir.Normalize();
	dir *= elapsed * 20.0f;

	m_sourceCamera += dir;
	m_destCamera = GolfBall().m_pos;

	m_zoomAngle += elapsed * 0.17453292f;
}

// camerawork.cpp:3024
void CCameraWork::FromHoleCupCamera(float elapsed, float time) {
	WVector cup = GOLFDOC()->GetHoleData().m_cupPos; m_sourceCamera = WVector(cup.x, cup.y - 2.0f, cup.z);
	m_destCamera = WVector(m_sourceCamera.x, m_sourceCamera.y + 1.0f, m_sourceCamera.z + 0.2f);
}

// camerawork.cpp:3032
void CCameraWork::IntroACamera(float elapsed, float time) {
	m_swingAngle -= elapsed * 0.52359879f;
	m_introDistance -= elapsed * 11.0f;
	m_introHeight -= elapsed * 2.0f;

	WMatrix rot = RotMat(m_swingAngle + GOLFDOC()->GetCameraData().m_yaw, 1);
	m_sourceCamera = GolfBall().m_pos - rot.za * m_introDistance;
	m_sourceCamera.y += m_introHeight;

	m_destCamera = rot.za + m_sourceCamera;

	if (m_swingAngle < 0.0f)
		m_nextCameraType = 0xcc;
}

// camerawork.cpp:3048
void CCameraWork::IntroBCamera(float elapsed, float time) {
	float t = m_cameraTime * 0.2f;
	m_sourceCamera = (m_introTo - m_introFrom) * t + m_introFrom;
	m_sourceCamera.y += m_introHeight;

	m_destCamera = GolfBall().m_pos;
	float x = t * 2.0f - 1.0f; m_destCamera.y += m_introHeight - (m_introHeight - 3.0f) * (x > 0.0f ? x : -x);

	if (m_cameraTime > 5.0f)
		m_nextCameraType = 0xcd;
}

// camerawork.cpp:3061
void CCameraWork::ZoomOutCamera(float elapsed, float time) {
	m_zoomAngle += elapsed * 0.95993108f;

	if (m_zoomAngle > 0.95993108f)
		m_zoomAngle = 0.95993108f;
}

// camerawork.cpp:3069
void CCameraWork::IntroCCamera(float elapsed, float time) {
	m_swingAngle -= elapsed * 0.52359879f;
	m_introDistance += elapsed * 2.0f;

	WPuppet *pPuppet = CSceneManager::Instance()->GetPuppet(MakeStr("PLAYER_%d", GOLFDOC()->m_myPlayerIndex));
	WBone *pHead = pPuppet->FindBone("Bip01 Head");
	float t = m_cameraTime * 0.2f;

	m_destCamera = (1.0f - t) * pHead->GetMatrix().pivot + (GolfBall().m_pos + WVector(0.0f, 5.0f, 0.0f)) * t;

	WMatrix rot = RotMat(GOLFDOC()->GetCameraData().m_yaw + m_swingAngle, 1);
	m_sourceCamera = m_destCamera - rot.za * m_introDistance;

	if (t > 1.0f)
		m_nextCameraType = 0xcc;
}

// camerawork.cpp:3087
void CCameraWork::IntroDCamera(float elapsed, float time) {
	if (m_cameraTime < 3.0f) {
		WVector move = elapsed * GOLFDOC()->GetCameraData().m_matrix.xa;
		m_sourceCamera += move;
		m_destCamera += move;
	} else if (m_cameraTime > 4.0f) {
		m_sourceCamera = GolfBall().m_pos - RotMat(GOLFDOC()->GetCameraData().m_yaw, 1).za * 25.0f;
		m_sourceCamera.y += 5.0f;
		m_destCamera = m_sourceCamera + RotMat(GOLFDOC()->GetCameraData().m_yaw, 1).za;
		m_baseMatrixCopy = GetCameraMatrix(m_sourceCamera, m_destCamera, 0.0f);

		m_nextCameraType = 0xCC;
	}
}

// camerawork.cpp:3106
void CCameraWork::ToDefaultCamera(float elapsed, float time) {
	if (!(m_cameraTime > m_transitionTime)) {
		m_sourceCamera = (m_introTo - m_introFrom) * m_cameraTime + m_introFrom;

		WMatrix rot;
		rot = WQuaternionSlerp(m_fromRotation, m_toRotation, m_cameraTime);

		m_destCamera = rot.za + m_sourceCamera;
	}
}

// camerawork.cpp:3119
void CCameraWork::FirstPersonCamera(float elapsed, float time) {
}

// camerawork.cpp:3123
void CCameraWork::StillFrameCamera(float elapsed, float time) {
}

// camerawork.cpp:3140
void CCameraWork::SwingReplayACamera(float elapsed, float time) {
}

// camerawork.cpp:3145
void CCameraWork::SwingReplayBCamera(float elapsed, float time) {
	if (!(m_cameraTime > m_transitionTime)) {
		float t = m_cameraTime / m_transitionTime;

		m_sourceCamera = (m_introTo - m_introFrom) * t + m_introFrom;

		WMatrix rot;
		rot = WQuaternionSlerp(m_fromRotation, m_toRotation, t);

		m_destCamera = rot.za + m_sourceCamera;
	}
}

// camerawork.cpp:3160
void CCameraWork::ExtraTimeCamera(float elapsed, float time) {
	m_swingAngle += elapsed * 0.08726646f;

	WMatrix rot;
	rot.Reset();
	rot.Rotate(m_swingAngle, 1);

	m_sourceCamera = rot.za * m_introDistance + m_destCamera;
	m_sourceCamera.y += m_introDistance * 0.2f;
}

// camerawork.cpp:3172
void CCameraWork::Quake(void) {
	m_flags |= 0x80000000; m_quakeStartTime = m_cameraTime;
	m_lastQuakeTime = -1.0f;
}

// camerawork.cpp:3179
WMatrix CCameraWork::GetCameraMatrix(const WVector &eye, const WVector &at, float roll) {
	if (WAbsBySign(roll) < WMATH_EPSILON)
		return MakeCameraMatrix(eye, at);
	else
		return RotMat(roll, 2) * MakeCameraMatrix(eye, at);
}
