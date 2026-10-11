// Original: d:\build\custom\temp\code\client\projectg\camera.cpp
#include "camera.h"
#include <math.h>
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "GolfDoc.h"

#include "wview.h"
#include "quadtree.h"

extern WMatrix g_camera;

// Input of the previous frame, compared by the Get*Acc ramps.
static int data_00af2adc;
static int data_00af2ae0;
static int data_00af2ae4;

// camera.cpp:33 (run-time type and factory registration of CCamera)
const WRTTI CCamera::m_RTTI("CCamera", &IActor::m_RTTI);

IObject *CCameraMakeInstance(void) {
	return new CCamera;
}

struct __impCCamera {
	__impCCamera(void) {
		ObjectFactory().AddObjectFunctor(CCameraMakeInstance, "CCamera");
		ActorFactory().AddActorClass("CCamera");
	}
};
static __impCCamera s_impCCamera;

// camera.cpp:56
CCamera::CCamera(void) {
	AddSkipList(0x84c82);
}

// camera.cpp:61
void CCamera::OnInit(void) {
	m_rotateAcc = 0;
	m_moveAcc = 0;
	m_horizonAcc = 0;

	m_unk50 = 0;
	m_zoom = 0;
	m_unk60 = false;
	m_unk70 = 0;
	m_unk74 = false;
	m_unk80 = 0;
	m_unk84 = false;
	m_unk6C = false;

	m_forcedMoveTotal = m_forcedMoveLeft = 0;

	m_unkC0 = 0;
	m_unkC4 = false;

	m_unk88 = 0;
	m_unk8C = 0;
}

// camera.cpp:649
void CCamera::ProcessForcedMove(float dt) {
	Rotate((m_rotateKey == 1 ? 1.0f : -1.0f) * dt * 4.1887903f);

	if (WAbsBySign(m_forcedMoveTotal) < 1e-5f) {
		dt = 1.0f;
	} else {
		dt = 1.0f - m_forcedMoveLeft / m_forcedMoveTotal;
	}
	SendMsg(this, "Spot", 0xf7, (intptr_t)&dt, 0, 0, 0);
}

// camera.cpp:729
void CCamera::LookAt(WVector &pos, WVector &at) {
	g_camera = MakeCameraMatrix(pos, at);
}

// camera.cpp:910
float CCamera::GetCameraAngle(void) {
	float angle = GOLFDOC()->GetCameraData().m_pitch - atan(m_zoom * 0.65f);
	return 0.0f > angle ? angle : 0.0f;
}

// camera.cpp:915
float CCamera::GetChangeRate(void) {
	return m_zoom * m_zoom;
}

// camera.cpp:920
void CCamera::GetRotateAcc(float dt) {
	if (data_00af2adc == m_rotateKey && m_rotateKey) {
		float acc = dt + m_rotateAcc; m_rotateAcc = acc < 0.0f ? 0.0f : (acc > 1.0f ? 1.0f : acc);
	} else {
		m_rotateAcc = 0;
	}
	data_00af2adc = m_rotateKey;
}

// camera.cpp:932
void CCamera::GetMoveAcc(float dt) {
	if (data_00af2ae0 == m_moveKey && m_moveKey) {
		float acc = dt + m_moveAcc; m_moveAcc = acc < 0.0f ? 0.0f : (acc > 1.0f ? 1.0f : acc);
	} else {
		m_moveAcc = 0;
	}
	data_00af2ae0 = m_moveKey;
}

// camera.cpp:944
void CCamera::GetHorizonAcc(float dt) {
	if (data_00af2ae4 == m_horizonKey && m_horizonKey) {
		float acc = dt + m_horizonAcc; m_horizonAcc = acc < 0.0f ? 0.0f : (acc > 1.0f ? 1.0f : acc);
	} else {
		m_horizonAcc = 0;
	}
	data_00af2ae4 = m_horizonKey;
}

// camera.cpp:956
float CCamera::GetHeightRate(float height) {
	float diff = height - m_baseHeight;
	return diff / (192.0f + diff);
}

// camera.cpp:1048
void CCamera::YawFixedCamera(float dt) {
	float yaw = dt * m_rotateAcc * 5.0f; m_fixedYaw -= yaw;
	if (m_fixedYaw > 10.0f) {
		m_fixedYaw = 10.0f;
	} else if (m_fixedYaw < -10.0f) {
		m_fixedYaw = -10.0f;
	} else {
		WVector move = g_camera.xa * yaw;
		m_at -= move; LookAt(m_pos, m_at);
	}
}

// camera.cpp:1066
void CCamera::PitchFixedCamera(float dt) {
	float pitch = dt * m_horizonAcc * 5.0f; m_fixedPitch += pitch;
	if (m_fixedPitch > 5.0f) {
		m_fixedPitch = 5.0f;
	} else if (m_fixedPitch < -5.0f) {
		m_fixedPitch = -5.0f;
	} else {
		WVector move = g_camera.ya * pitch;
		m_at += move; LookAt(m_pos, m_at);
	}
}

// camera.cpp:1084
void CCamera::RotateEnvironment(float angle) {
	GOLFDOC()->GetCameraData().m_yaw = GetBaseAngle(GOLFDOC()->GetCameraData().m_yaw + angle);
	GOLFDOC()->GetCameraData().m_matrix = RotMat(GOLFDOC()->GetCameraData().m_yaw + (IsMyTurn(true) ? 0.0f : GOLFDOC()->GetCameraData().m_yaw2), 1);

	CGolfDoc *doc = GOLFDOC();
	float dist = doc->GetCameraData().m_distance * -1.0f;
	doc->GetCameraData().m_matrix.pivot = GetPVS().TestCameraCollision(WVector(doc->GetCameraData().m_matrix.za.x * dist, doc->GetCameraData().m_height, doc->GetCameraData().m_matrix.za.z * dist) + doc->GetCameraData().m_target);

	m_heightRate = GetHeightRate(GOLFDOC()->GetCameraData().m_matrix.pivot.y);

	if (IsMyTurn(false)) {
		m_unk84 = true;
		SendMsg(this, "Player", 0x1a1, (intptr_t)&angle, 0, 0, 0);
		SendMsg(this, "Spot", 0x1a1, (intptr_t)&angle, 0, 0, 0);
		SendMsg(this, "Screen", 0x1a1, (intptr_t)&angle, 0, 0, 0);
		SendMsg(this, "Grid", 0x1a1, (intptr_t)&angle, 0, 0, 0);
		SendMsg(this, "Caddie", 0x1a1, (intptr_t)&angle, 0, 0, 0);
		SendMsg(this, "GuideLine", 0x1a1, 0, 0, 0, 0);
	}
}
