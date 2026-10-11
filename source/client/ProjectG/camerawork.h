// Original: d:\build\custom\temp\code\client\projectg\camerawork.h
#ifndef PG_CLIENT_PROJECTG_CAMERAWORK_H
#define PG_CLIENT_PROJECTG_CAMERAWORK_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wmath.h"
#include <vector>

class WPuppet;
struct w_motion_data;

// The game's camera director (WSingleton<CCameraWork>, registered by the
// constructor). Layout reconstructed only as far as the decompiled
// functions use it; the full size is not established yet.
class CCameraWork : public WSingleton<CCameraWork> {
	LAYOUT_CHECK_FRIEND;
public:
	virtual ~CCameraWork();

	WVector GetSourceCamera(void);
	WVector GetDestCamera(void);
	void SetDestCamera(const WVector &dest);
	void SetInstalledCamera(const WVector &source);
	void UninstallCamera(void);
	void StopBallSound(void);
	void Quake(void);
	void EarthQuake(void);
	WMatrix GetCameraMatrix(const WVector &eye, const WVector &at, float roll);
	void Process(float elapsed, float time);
	bool CheckState(void);
	void ReplayCamera(bool keep);
	void SetCaddieCamera(const WMatrix &matrix, const WVector &target);

protected:
	// A hole-in camera puppet loaded for one player.
	struct sHoleInCam {
		unsigned long m_id;     // 0x0
		WPuppet *m_pPuppet;     // 0x4
	};

	bool CheckGround(WVector &pos, WVector &dest);
	WPuppet *FindHoleInCam(unsigned long id);
	void ParsePuppetScript(char *script);
	void SetCamera(int type, bool immediately);
	void InitVars(void);
	void ScriptProcess(w_motion_data *motion, float from, float to);
	void CheckCameraType(bool);
	void InitCtrlPoints(int type);

private:
	void PangyaACamera(float elapsed, float time);
	void BankFromHeadCamera(float elapsed, float time);
	void IntroCCamera(float elapsed, float time);
	void PuttSecondBCamera(float elapsed, float time);
	void TomahawkACamera(float elapsed, float time);
	void ToDefaultCamera(float elapsed, float time);
	void SwingReplayBCamera(float elapsed, float time);
	void ZoomOutCamera(float elapsed, float time);
	void FirstPersonCamera(float elapsed, float time);
	void StillFrameCamera(float elapsed, float time);
	void SwingReplayACamera(float elapsed, float time);
	void FixedAtSideCamera(float elapsed, float time);
	void PuttSecondACamera(float elapsed, float time);
	void FixedCamera(float elapsed, float time);
	void SideCamera(float elapsed, float time);
	void FromHoleCupCamera(float elapsed, float time);
	void FromColPointCamera(float elapsed, float time);
	void DropCamera(float elapsed, float time);
	void ZoomInCamera(float elapsed, float time);
	void Drop_and_ZoomCamera(float elapsed, float time);
	void FromRandomPointCamera(float elapsed, float time);
	void FollowCamera(float elapsed, float time);
	void BezierCamera(float elapsed, float time);
	void Rail_or_SideCamera(float elapsed, float time);
	void FollowWithSwingCamera(float elapsed, float time);
	void IntroACamera(float elapsed, float time);
	void IntroBCamera(float elapsed, float time);
	void ExtraTimeCamera(float elapsed, float time);
	void FromMidPointCamera(float elapsed, float time);
	void LookBackCamera(float elapsed, float time);
	void Chase_and_LookBackCamera(float elapsed, float time);
	void InterpolateBezier(WVector *out, float t, WVector *points, int count);
	void CircleCamera(float elapsed, float time);
	void CaddyViewCamera(float elapsed, float time);
	void TopViewCamera(float elapsed, float time);
	void BallSideCamera(float elapsed, float time);
	void FollowBallCamera(float elapsed, float time);
	void CheckDriveCameraBallVisibilityAndSetBallVisibilitySafeCamera(void);
	void IntroDCamera(float elapsed, float time);

	float m_inputYaw;               // 0x28, InputProcess's free-look offsets (keys); SetCamera and
	float m_inputZoom;              // 0x2c   ReplayCamera reset them
	float m_inputPitch;             // 0x30
	bool m_inputUsed;               // 0x34, set by InputProcess
	PADDING(0x35, 0x38);
	void (CCameraWork::*m_pCameraProc)(float elapsed, float time);  // 0x38, the current camera work (Process calls it)
	WVector m_sourceCamera;         // 0x3c, camera position
	PADDING(0x48, 0x60);
	WVector m_destCamera;           // 0x60, camera target
	int m_driveCameraCount;         // 0x6c, entries of driveTable
	int m_puttCameraCount;          // 0x70, entries of puttTable
	PADDING(0x74, 0x7c);
	std::vector<int> m_safeCameras; // 0x7c, camera types the ball-visibility check switches to
	int m_cameraType;               // 0x8c, the camera SetCamera installed (InitCtrlPoints' argument)
	int m_cameraIndex;              // 0x90, ReplayCamera's position in drive/puttTable (+100 for putts)
	int m_nextCameraType;           // 0x94, the SetCamera type ZoomInCamera picks when done
	unsigned long m_flags;          // 0x98, bit 31 = quaking
	float m_duration;               // 0x9c, ZoomInCamera's length
	float m_phase1Time;             // 0xa0, Drop/FromColPoint follow the ball until then
	float m_phase2Time;             // 0xa4, end of the Bezier phase
	float m_phase0Time;             // 0xa8, start of the first phase
	float m_workTime;               // 0xac, time into the current camera work
	int m_workLength;               // 0xb0, the camera work's length (the ball's flight)
	int m_processedFrame;           // 0xb4, the frame Process handled last (-2: none)
	int m_preDataFrame;             // 0xb8, the ball's PreData frame TomahawkACamera looks back from
	int m_ballHiddenFrame;          // 0xbc, m_preDataFrame when the ball got hidden (-1: visible)
	WMatrix m_baseMatrix;           // 0xc0, SetCaddieCamera's matrix; SetCamera starts from its pivot
	WMatrix m_baseMatrixCopy;       // 0xf0, copy of m_baseMatrix taken by SetCaddieCamera
	WVector m_bezierPoints[4];      // 0x120, InterpolateBezier's control points
	PADDING(0x150, 0x18c);
	float m_riseHeight;             // 0x18c, FollowCamera lifts its target by up to this
	float m_cameraTime;             // 0x190, time spent in the current camera
	float m_lastQuakeTime;          // 0x194, EarthQuake's last shake (-1 restarts it)
	float m_quakeStartTime;         // 0x198
	float m_ballSoundTime;          // 0x19c, ProcessBallSound's timer
	bool m_stateFlag;               // 0x1a0, role unknown: cleared by InitVars and ReplayCamera
	bool m_holeInReached;           // 0x1a1, CheckState: the holed ball reached the cup
	bool m_panDone;                 // 0x1a2, set by PanCamera
	PADDING(0x1a3, 0x1a4);
	int m_ballSoundState;           // 0x1a4
	float m_swingAngle;             // 0x1a8, FollowWithSwingCamera's RotMat angle
	WMatrix m_baseRotation;         // 0x1ac, rotation part of m_baseMatrix (SetCaddieCamera)
	bool m_keepInputOffsets;        // 0x1dc, SetCamera keeps the free-look offsets while set
	PADDING(0x1dd, 0x1df);
	bool m_destFollowsGround;       // 0x1df, CheckGround lifts the target with the camera
	PADDING(0x1e0, 0x1ec);
	sHoleInCam m_holeInCams[4];     // 0x1ec, one per player
	WPuppet *m_pCameraPuppet;       // 0x20c, LoadCamera's camera puppet
	WPuppet *m_pScriptPuppet;       // 0x210, ScriptProcess runs its frame scripts
	PADDING(0x214, 0x218);
	WVector m_ballStartPos;         // 0x218, the ball position InitVars took (JustLookingCamera)
	WQuat m_fromRotation;           // 0x224, ToDefaultCamera/SwingReplayBCamera slerp from it
	WQuat m_toRotation;             // 0x234, to it
	WVector m_introFrom;            // 0x244, IntroBCamera's start position
	WVector m_introTo;              // 0x250, IntroBCamera's end position
	float m_introDistance;          // 0x25c, IntroACamera's distance from the ball
	float m_introHeight;            // 0x260, raised over the intro positions
	float m_transitionTime;         // 0x264, ToDefaultCamera/SwingReplayBCamera's length
	float m_zoomAngle;              // 0x268, radians
	bool m_firstCamera;             // 0x26c, set by InitVars, cleared by SetCamera, tested by Process
	bool m_landsInWater;            // 0x26d, InitVars: the shot ends on ground type 2 (no object hit)
	PADDING(0x26e, 0x270);
	std::vector<int> m_cameraQueue; // 0x270, role and element type not established (freed by the destructor)
	PADDING(0x280, 0x284);
	std::vector<int> m_replayCameras;   // 0x284, camera types SetCamera records and replays
	int m_replayPos;                // 0x294, next entry of m_replayCameras (-1: recording)
	int m_fixedCameraIndex;         // 0x298, its parity picks FixedCamera's fallback (0x6d/0x6e)
	bool m_installed;               // 0x29c, SetInstalledCamera's fixed source
	PADDING(0x29d, 0x2a0);
	float m_bankAngle;              // 0x2a0, BankFromHeadCamera's roll, radians
	PADDING(0x2a4, 0x2ac);
	int m_inputMode;                // 0x2ac, InputProcess's key mode (0, 1, 2)
	PADDING(0x2b0, 0x2b4);
};

CHECK_SIZE(CCameraWork, 0x2b4);
BEGIN_LAYOUT_CHECK(CCameraWork)
	CHECK_FIELD(CCameraWork, m_inputYaw, 0x28);
	CHECK_FIELD(CCameraWork, m_inputZoom, 0x2c);
	CHECK_FIELD(CCameraWork, m_inputPitch, 0x30);
	CHECK_FIELD(CCameraWork, m_inputUsed, 0x34);
	CHECK_FIELD(CCameraWork, m_pCameraProc, 0x38);
	CHECK_FIELD(CCameraWork, m_sourceCamera, 0x3c);
	CHECK_FIELD(CCameraWork, m_destCamera, 0x60);
	CHECK_FIELD(CCameraWork, m_driveCameraCount, 0x6c);
	CHECK_FIELD(CCameraWork, m_puttCameraCount, 0x70);
	CHECK_FIELD(CCameraWork, m_safeCameras, 0x7c);
	CHECK_FIELD(CCameraWork, m_cameraType, 0x8c);
	CHECK_FIELD(CCameraWork, m_cameraIndex, 0x90);
	CHECK_FIELD(CCameraWork, m_nextCameraType, 0x94);
	CHECK_FIELD(CCameraWork, m_flags, 0x98);
	CHECK_FIELD(CCameraWork, m_duration, 0x9c);
	CHECK_FIELD(CCameraWork, m_phase1Time, 0xa0);
	CHECK_FIELD(CCameraWork, m_phase2Time, 0xa4);
	CHECK_FIELD(CCameraWork, m_phase0Time, 0xa8);
	CHECK_FIELD(CCameraWork, m_workTime, 0xac);
	CHECK_FIELD(CCameraWork, m_workLength, 0xb0);
	CHECK_FIELD(CCameraWork, m_processedFrame, 0xb4);
	CHECK_FIELD(CCameraWork, m_preDataFrame, 0xb8);
	CHECK_FIELD(CCameraWork, m_ballHiddenFrame, 0xbc);
	CHECK_FIELD(CCameraWork, m_baseMatrix, 0xc0);
	CHECK_FIELD(CCameraWork, m_baseMatrixCopy, 0xf0);
	CHECK_FIELD(CCameraWork, m_bezierPoints, 0x120);
	CHECK_FIELD(CCameraWork, m_riseHeight, 0x18c);
	CHECK_FIELD(CCameraWork, m_cameraTime, 0x190);
	CHECK_FIELD(CCameraWork, m_lastQuakeTime, 0x194);
	CHECK_FIELD(CCameraWork, m_quakeStartTime, 0x198);
	CHECK_FIELD(CCameraWork, m_ballSoundTime, 0x19c);
	CHECK_FIELD(CCameraWork, m_stateFlag, 0x1a0);
	CHECK_FIELD(CCameraWork, m_panDone, 0x1a2);
	CHECK_FIELD(CCameraWork, m_holeInReached, 0x1a1);
	CHECK_FIELD(CCameraWork, m_ballSoundState, 0x1a4);
	CHECK_FIELD(CCameraWork, m_swingAngle, 0x1a8);
	CHECK_FIELD(CCameraWork, m_baseRotation, 0x1ac);
	CHECK_FIELD(CCameraWork, m_keepInputOffsets, 0x1dc);
	CHECK_FIELD(CCameraWork, m_destFollowsGround, 0x1df);
	CHECK_FIELD(CCameraWork, m_holeInCams, 0x1ec);
	CHECK_FIELD(CCameraWork, m_pCameraPuppet, 0x20c);
	CHECK_FIELD(CCameraWork, m_pScriptPuppet, 0x210);
	CHECK_FIELD(CCameraWork, m_ballStartPos, 0x218);
	CHECK_FIELD(CCameraWork, m_fromRotation, 0x224);
	CHECK_FIELD(CCameraWork, m_toRotation, 0x234);
	CHECK_FIELD(CCameraWork, m_introFrom, 0x244);
	CHECK_FIELD(CCameraWork, m_introTo, 0x250);
	CHECK_FIELD(CCameraWork, m_introDistance, 0x25c);
	CHECK_FIELD(CCameraWork, m_introHeight, 0x260);
	CHECK_FIELD(CCameraWork, m_transitionTime, 0x264);
	CHECK_FIELD(CCameraWork, m_zoomAngle, 0x268);
	CHECK_FIELD(CCameraWork, m_firstCamera, 0x26c);
	CHECK_FIELD(CCameraWork, m_landsInWater, 0x26d);
	CHECK_FIELD(CCameraWork, m_cameraQueue, 0x270);
	CHECK_FIELD(CCameraWork, m_replayCameras, 0x284);
	CHECK_FIELD(CCameraWork, m_replayPos, 0x294);
	CHECK_FIELD(CCameraWork, m_fixedCameraIndex, 0x298);
	CHECK_FIELD(CCameraWork, m_installed, 0x29c);
	CHECK_FIELD(CCameraWork, m_bankAngle, 0x2a0);
	CHECK_FIELD(CCameraWork, m_inputMode, 0x2ac);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_CAMERAWORK_H
