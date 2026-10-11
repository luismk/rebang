// Original: d:\build\custom\temp\code\client\projectg\camera.h
#ifndef PG_CLIENT_PROJECTG_CAMERA_H
#define PG_CLIENT_PROJECTG_CAMERA_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "wmath.h"

// Layout not fully reconstructed yet: only the members the unit's functions
// use. Roles are read from OnInit, the Get*Acc ramps and the fixed-camera
// moves.
class CCamera : public IActor {
	LAYOUT_CHECK_FRIEND;
	// The factory function of the class registration (camera.cpp:33).
	friend IObject *CCameraMakeInstance(void);
public:
	// camera.h:11
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }
	// camera.h:9
	virtual ~CCamera(void) {}

	virtual void OnInit(void);
	virtual void OnProcess(float dt);

	// Spot-camera preset. Its implicit constructor is emitted out of line
	// (??0sSpotCam@CCamera@@QAE@XZ, `mov eax, ecx; ret`), which takes a
	// member with a user-declared constructor (WVector) and an array of
	// sSpotCam in CCamera. Members beyond the first are not known yet.
	struct sSpotCam {
		WVector m_pos; // 0x0
	};

	static const WRTTI m_RTTI;

protected:
	CCamera(void);

	virtual void HandleMsg(const MsgObject &msg);

	void LookAt(WVector &pos, WVector &at);
	float GetCameraAngle(void);
	float GetChangeRate(void);
	void GetRotateAcc(float dt);
	void GetMoveAcc(float dt);
	void GetHorizonAcc(float dt);
	float GetHeightRate(float height);
	void YawFixedCamera(float dt);
	void PitchFixedCamera(float dt);
	void Rotate(float angle);
	void RotateEnvironment(float angle);
	void ProcessForcedMove(float dt);

	int m_rotateKey;            // 0x18, rotate input of this frame (0 = none)
	float m_forcedMoveTotal;    // 0x1c, length of the forced move (ProcessForcedMove)
	float m_forcedMoveLeft;     // 0x20, what remains of it (ProcessForcedMove)
	int m_moveKey;              // 0x24, move input of this frame (0 = none)
	int m_horizonKey;           // 0x28, horizon input of this frame (0 = none)
	float m_rotateAcc;          // 0x2c, rotate ramp 0..1 (GetRotateAcc)
	float m_moveAcc;            // 0x30, move ramp 0..1 (GetMoveAcc)
	float m_horizonAcc;         // 0x34, horizon ramp 0..1 (GetHorizonAcc)
	WVector m_pos;              // 0x38, eye of the camera matrix
	WVector m_at;               // 0x44, look-at point of the camera matrix
	float m_unk50;              // 0x50, cleared by OnInit
	float m_zoom;               // 0x54, squared by GetChangeRate
	PADDING(0x58, 0x5c);
	float m_baseHeight;         // 0x5c, origin of GetHeightRate
	bool m_unk60;               // 0x60, cleared by OnInit
	PADDING(0x61, 0x64);
	float m_heightRate;         // 0x64, GetHeightRate of the camera's eye (RotateEnvironment)
	PADDING(0x68, 0x6c);
	bool m_unk6C;               // 0x6c, cleared by OnInit
	PADDING(0x6d, 0x70);
	int m_unk70;                // 0x70, cleared by OnInit
	bool m_unk74;               // 0x74, cleared by OnInit
	PADDING(0x75, 0x78);
	float m_fixedYaw;           // 0x78, fixed-camera yaw, -10..10
	float m_fixedPitch;         // 0x7c, fixed-camera pitch, -5..5
	int m_unk80;                // 0x80, cleared by OnInit
	bool m_unk84;               // 0x84, cleared by OnInit
	PADDING(0x85, 0x88);
	int m_unk88;                // 0x88, cleared by OnInit
	int m_unk8C;                // 0x8c, cleared by OnInit
	// Element count not established (0x90..0xc0 holds four).
	sSpotCam m_spotCam[1];      // 0x90
	PADDING(0x9c, 0xc0);
	int m_unkC0;                // 0xc0, cleared by OnInit
	bool m_unkC4;               // 0xc4, cleared by OnInit
	PADDING(0xc5, 0xc8);
};

CHECK_SIZE(CCamera, 0xc8);
BEGIN_LAYOUT_CHECK(CCamera)
	CHECK_FIELD(CCamera, m_rotateKey, 0x18);
	CHECK_FIELD(CCamera, m_rotateAcc, 0x2c);
	CHECK_FIELD(CCamera, m_pos, 0x38);
	CHECK_FIELD(CCamera, m_at, 0x44);
	CHECK_FIELD(CCamera, m_zoom, 0x54);
	CHECK_FIELD(CCamera, m_baseHeight, 0x5c);
	CHECK_FIELD(CCamera, m_heightRate, 0x64);
	CHECK_FIELD(CCamera, m_fixedYaw, 0x78);
	CHECK_FIELD(CCamera, m_unk8C, 0x8c);
	CHECK_FIELD(CCamera, m_spotCam, 0x90);
	CHECK_FIELD(CCamera, m_unkC0, 0xc0);
	CHECK_FIELD(CCamera, m_unkC4, 0xc4);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CAMERA_H
