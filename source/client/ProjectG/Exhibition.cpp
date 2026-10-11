// Original: d:\build\custom\temp\code\client\projectg\exhibition.cpp
// The original holds the out-of-line copy of WPuppet::LockBoneSetPos.
#define EXHIBITION_WPUPPET_INLINE
#include "exhibition.h"
#include "wbone.h"
#include "wpuppet.h"
#include "wresrcmng.h"
#include "partsinfo.h"
#include "wview.h"
#include "wutil.h"
#include "InputManager.h"
#include "mousecursor.h"
#include "texturepart.h"
#include "commonutil.h"
#include "fxspray.h"
#include "SharedDoc.h"
#include "RealMyRoomTask.h"
#include "../client/Fresh/include/frwnd.h"
#include <algorithm>
#include <math.h>
#include <string.h>

// Motion tables (nameless data): a row of motion indices per motion set, -1
// ends a row; the indices select a name in the name tables.
extern const int data_a08310[][12];        // character motion sets
extern const int data_a084f0[][9];         // caddie motion sets (28 rows)
extern const char* const data_a96128[];    // character motion names
extern const char* const data_a96158[];    // caddie motion names
extern const char* const data_a96180[][4]; // face textures [face type][expression]

// Constants the original reads from storage (0xa088e0..0xa088ec); their
// header is not identified yet.
const float kPi = 3.14159265f;
const float kDegree = 0.017453292f;
const float kFloatMax = 3.402823466e+38f;
const float kEpsilon = 1e-5f;

extern WMatrix g_camera;
int WisZero(const float &f, float eps);
const char* GetStrConst(const char* pSrc, char* pDest);
const char* GetCharsTillSpace(const char* pSrc, char* pDest); // commonutil.cpp
float Random(float min, float max, int seed); // commonutil.cpp

// exhibition.cpp:295
CExhibition::CExhibition(float x, float y, float screenScale, bool autoRecover)
	: m_motionDelay(0.0f), m_initialMotionLength(0.0f), m_motionIndex(-1), m_charMotionSet(-1),
	  m_caddieMotionSet(-1), m_lockDirection(false), m_zoomDragging(false), m_curDistance(20.0f),
	  m_pitchLimit(0.0f), m_frame(0.0f), m_pMotionData(NULL), m_pPetFrame(NULL), m_model(NULL),
	  m_controlFlag(0), m_facialChanged(false), m_setByCtor(true), m_enableZoom(true), m_enableRotate(true) {
	m_center.x = x;
	m_center.y = y;
	m_autoRecoverCamera = autoRecover;
	m_screenScale = screenScale;
	Init();

	m_fxBox.Init(g_resrcmng);
	m_fxBox.SetCamera(&g_camera);
	// The original also sets m_fxBox's frame time (CFxBox +0x4) to 33.333f
	// here: FxBox.h has no accessor for it yet.
	m_pFxSequence1 = NULL;
	m_pFxSequence2 = NULL;
}

// exhibition.cpp:312
CExhibition::~CExhibition(void) {
	ReleasePet();

	m_fxBox.ClearAll(false);

	for (std::list<sTexturePart*>::iterator it = m_texParts.begin(); it != m_texParts.end(); ++it) {
		delete *it;
	}
	m_texParts.clear();
}

// exhibition.cpp:326
void CExhibition::ReleasePet(void) {
	if (m_model) {
		m_model->LockBoneSetPos(0xFF);
	}
	if (m_exhibitType == 0) {
		if (m_pPetFrame) { delete m_pPetFrame; m_pPetFrame = NULL; }
		m_model = NULL;
	} else {
		if (g_resrcmng && m_model) { g_resrcmng->Release(m_model, false); m_model = NULL; }
	}
	if (g_resrcmng && m_texHandle) { g_resrcmng->Release(m_texHandle); m_texHandle = 0; }
}

// exhibition.cpp:344
void CExhibition::Init(void) {
	m_pitchSpeed = m_directionSpeed = 0.0f;
	m_defaultAngle.x = m_defaultAngle.y = 0.0f;
	m_angle.x = m_angle.y = 0.0f;

	m_defaultDistance = m_curDistance;
	m_checkedDirection = 0.0f;
	m_directionCheckTime = 0.0f;
	m_facialTime = 0.0f;
	m_distance = m_curDistance;
	m_resetByInit = 0;
	m_minDistance = 15.0f;
	m_maxDistance = 25.0f;
	m_typeId = 0;
	m_pItemSlots = NULL;
	m_controlFlag = 6;
	m_nextMotion = false;
	UpdateCamera(0.0f);

	m_cameraUpdate = 2;
	m_matrix.Reset();

	m_texHandle = 0;
	m_light.diffuse = 0x969696;
	m_light.type = LightSet::W_LIGHT_DIRECTIONAL;
}

// exhibition.cpp:378
void CExhibition::SetDistance(float distance, bool immediate, bool updateView) {
	m_distance = Between(m_minDistance, distance, m_maxDistance);

	if (immediate) {
		m_curDistance = m_distance = distance;
	}

	UpdateCamera(0.0f);

	// The view follows, except in the my-room task unless asked.
	if (g_view && (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CRealMyRoomTask::m_RTTI) : false) || updateView)) {
		g_view->SetCamera(m_camera);
		g_view->UpdateCamera();
	}
}

// exhibition.cpp:398
float CExhibition::GetDistance(void) {
	return m_distance;
}

// exhibition.cpp:403
void CExhibition::Zoom(float delta) {
	SetDistance(m_distance + delta, false, false);
	m_cameraUpdate = 1;
	m_cameraRecoverTime = 0.0f;
}

// exhibition.cpp:410
void CExhibition::UpdateCamera(float dt) {
	float step = dt * 15.0f;
	if (m_curDistance - m_distance > step) {
		m_curDistance -= step;
	} else if (m_curDistance - m_distance < dt * -15.0f) {
		m_curDistance += step;
	} else {
		m_curDistance = m_distance;
	}
	m_camera = RotMat(m_angle.x, 0) * RotMat(m_angle.y, 1);
	m_camera.pivot = m_camera.za * -m_curDistance;

	if ((m_controlFlag >> 4) & 1 && m_pPetFrame) {
		// The camera follows the pet's root bone.
		WBone* pBone = m_pPetFrame->GetBonePet()->FindBone("Bip01");
		if (pBone) {
			m_camera.pivot.x += pBone->GetMatrix().xm;
			m_camera.pivot.z += pBone->GetMatrix().zm;
		}
	}
}

// exhibition.cpp:438
const char* CExhibition::GetMotionName(void) {
	if (m_pMotionData) {
		return m_pMotionData->name;
	}
	return NULL;
}

// exhibition.cpp:446
void CExhibition::Process(float dt, bool b) {
	if (!m_model) {
		return;
	}
	if (!m_model->GetRootBone()) {
		return;
	}
	if (m_alpha < 255.0f) {
		m_alpha = 255.0f; SetAlpha(255);
	}

	int button = g_input->GetButton(RIGHT_BUTTON);
	if (button == 1 && InValidArea()) {
		m_zoomDragging = true;
	} else if (button == 2) {
		m_zoomDragging = false;
	}

	button = g_input->GetButton(LEFT_BUTTON);
	if (button == 1 && InValidArea()) {
		m_lockDirection = true;
	} else if (button == 2) {
		m_lockDirection = false;
	}

	m_nextMotion = false;
	if ((m_controlFlag >> 3) & 1 && m_lockDirection && m_zoomDragging && g_input->GetButton(MIDDLE_BUTTON) == 2) {
		m_nextMotion = NextMotion();
	} else if (m_lockDirection && m_enableRotate) {
		float pitch = m_angle.x - CInputManager::Instance()->GetMouseDelta().y * 0.01f - m_defaultAngle.x;
		if ((m_controlFlag >> 2) & 1) {
			// The pitch stays within [-60, limit + 15] degrees of the default.
			if (pitch < m_pitchLimit + 0.2617994f && pitch > -1.0471976f) {
				m_angle.x -= CInputManager::Instance()->GetMouseDelta().y * 0.01f;
			}
		}
		if ((m_controlFlag >> 1) & 1) {
			m_angle.y -= CInputManager::Instance()->GetMouseDelta().x * 0.01f;
		}
		if (m_controlFlag & 6) {
			m_cameraUpdate = 1;
			m_cameraRecoverTime = 0.0f;
		}
	} else if (m_zoomDragging && m_enableZoom) {
		if (b) {
			if (m_controlFlag & 1) {
				Zoom(CInputManager::Instance()->GetMouseDelta().y * 0.1f);
				CMouseCursor::Instance()->SetCursor(6);
			}
		}
	} else if (m_autoRecoverCamera) {
		m_cameraRecoverTime += dt;
		if (m_cameraRecoverTime > 10.0f && m_cameraUpdate == 1) {
			// Back to the default angles at a speed that brings both in together.
			float pitch = m_defaultAngle.x - GetBaseAngle(m_angle.x, m_defaultAngle.x);
			float direction = m_defaultAngle.y - GetBaseAngle(m_angle.y, m_defaultAngle.y);
			if (WisZero(pitch, 1e-5f) && WisZero(direction, 1e-5f)) {
				m_pitchSpeed = m_directionSpeed = 0.0f;
				m_cameraUpdate = 2;
			} else {
				float sum = Abs(pitch) + Abs(direction);
				m_cameraUpdate = 0;
				float inv = 1.0f / sum;
				m_pitchSpeed = pitch * inv * kPi;
				m_directionSpeed = direction * inv * kPi;
			}
			m_distance = m_defaultDistance;
		}
	}

	switch (m_cameraUpdate) {
	case 0:
		m_angle.x += dt * m_pitchSpeed;
		m_angle.y += dt * m_directionSpeed;
		if ((m_pitchSpeed < 0.0f && m_angle.x < m_defaultAngle.x) || (m_pitchSpeed > 0.0f && m_angle.x > m_defaultAngle.x)
			|| (m_directionSpeed < 0.0f && m_angle.y < m_defaultAngle.y) || (m_directionSpeed > 0.0f && m_angle.y > m_defaultAngle.y)) {
			m_angle.x = m_defaultAngle.x;
			m_angle.y = m_defaultAngle.y;
			m_pitchSpeed = m_directionSpeed = 0.0f;
			m_cameraUpdate = 2;
		}
		// fall through
	case 2:
		if (m_exhibitType == 2 || m_exhibitType == 3 || m_exhibitType == 4 || m_exhibitType == 5) {
			m_matrix = m_matrix * RotMat(dt * -1.0471976f, 1);
		}
		break;
	case 1:
		m_pitchSpeed = m_directionSpeed = 0.0f;
		break;
	}

	UpdateCamera(dt);

	m_angle.x = GetBaseAngle(m_angle.x, m_defaultAngle.x);
	m_angle.y = GetBaseAngle(m_angle.y, m_defaultAngle.y);

	m_directionCheckTime += dt;
	if (m_directionCheckTime > 1.0f) {
		m_directionCheckTime -= 1.0f;
		if (!m_directionChanged && Abs(GetBaseAngle(m_checkedDirection, m_angle.y) - m_angle.y) > kDegree) {
			m_checkedDirection = m_angle.y;
			m_directionChanged = true;
		}
	}

	if (m_facialChanged) {
		m_facialTime += dt;
		if (m_facialTime > 2.0f) {
			m_facialTime -= 2.0f;
			SetPendingTexPart("face", data_a96180[m_charMotionSet][3]);
			m_facialChanged = false;
		}
	}

	m_motionDelay -= dt;
	if (m_motionDelay > 0.0f) {
		return;
	}

	if (m_pMotionData) {
		ScriptProcess(m_pMotionData, m_frame, m_frame + dt, false);
		m_frame += dt;
		if (m_frame > m_pMotionData->e) {
			SetMotion(m_pMotionData->nextmotion, true, m_frame - m_pMotionData->e);
		} else {
			Animate();
		}
	} else {
		Animate();
	}

	m_fxBox.ProcessAttacher(dt * 1000.0f);
	m_fxBox.Process(dt * 1000.0f);
}

// exhibition.cpp:656
int CExhibition::ProcessBongdariShop(float dt) {
	if (!m_model) {
		return 0;
	}
	if (m_alpha < 255.0f) {
		m_alpha = 255.0f; SetAlpha(255);
	}

	int button = g_input->GetButton(LEFT_BUTTON);
	if (button == 1 && InValidArea()) {
		m_lockDirection = true;
	} else if (button == 2) {
		m_lockDirection = false;
	}

	if (m_autoRecoverCamera) {
		m_cameraRecoverTime += dt;
		if (m_cameraRecoverTime > 10.0f && m_cameraUpdate == 1) {
			float pitch = m_defaultAngle.x - GetBaseAngle(m_angle.x, m_defaultAngle.x);
			float direction = m_defaultAngle.y - GetBaseAngle(m_angle.y, m_defaultAngle.y);
			if (WisZero(pitch, 1e-5f) && WisZero(direction, 1e-5f)) {
				m_pitchSpeed = m_directionSpeed = 0.0f;
				m_cameraUpdate = 2;
			} else {
				float sum = Abs(pitch) + Abs(direction);
				m_cameraUpdate = 0;
				float inv = 1.0f / sum;
				m_pitchSpeed = pitch * inv * kPi;
				m_directionSpeed = direction * inv * kPi;
			}
			m_distance = m_defaultDistance;
		}
	}

	switch (m_cameraUpdate) {
	case 0:
		m_angle.x += dt * m_pitchSpeed;
		m_angle.y += dt * m_directionSpeed;
		if ((m_pitchSpeed < 0.0f && m_angle.x < m_defaultAngle.x) || (m_pitchSpeed > 0.0f && m_angle.x > m_defaultAngle.x)
			|| (m_directionSpeed < 0.0f && m_angle.y < m_defaultAngle.y) || (m_directionSpeed > 0.0f && m_angle.y > m_defaultAngle.y)) {
			m_angle.x = m_defaultAngle.x;
			m_angle.y = m_defaultAngle.y;
			m_pitchSpeed = m_directionSpeed = 0.0f;
			m_cameraUpdate = 2;
		}
		break;
	case 1:
		m_pitchSpeed = m_directionSpeed = 0.0f;
		break;
	}

	UpdateCamera(dt);

	m_angle.x = GetBaseAngle(m_angle.x, m_defaultAngle.x);
	m_angle.y = GetBaseAngle(m_angle.y, m_defaultAngle.y);

	m_directionCheckTime += dt;
	if (m_directionCheckTime > 1.0f) {
		m_directionCheckTime -= 1.0f;
		if (!m_directionChanged && Abs(GetBaseAngle(m_checkedDirection, m_angle.y) - m_angle.y) > kDegree) {
			m_checkedDirection = m_angle.y;
			m_directionChanged = true;
		}
	}

	m_motionDelay -= dt;
	if (m_motionDelay > 0.0f) {
		return 0;
	}

	// 1: the motion moved on to the next one, 2: it moved on to the restart motion
	int result = 0;
	if (m_pMotionData) {
		ScriptProcess(m_pMotionData, m_frame, m_frame + dt, false);
		m_frame += dt;
		if (m_frame > m_pMotionData->e) {
			if (strcmp(m_pMotionData->name, m_pMotionData->nextmotion) != 0) {
				result = (strcmp(m_pMotionData->name, "\xBB\xF5\xB7\xCE\xBD\xC3\xC0\xDB") == 0) + 1;
			}
			SetMotion(m_pMotionData->nextmotion, true, m_frame - m_pMotionData->e);
		} else {
			Animate();
		}
	} else {
		Animate();
	}

	m_fxBox.ProcessAttacher(dt * 1000.0f);
	m_fxBox.Process(dt * 1000.0f);

	return result;
}

// exhibition.cpp:812
void CExhibition::SetDirection(float direction) {
	m_angle.y = direction;
}

// exhibition.cpp:817
void CExhibition::SetDefaultDirection(float direction) {
	m_cameraUpdate = 1; m_cameraRecoverTime = 10.0f; m_defaultAngle.y = direction;
}

// exhibition.cpp:824
bool CExhibition::IsDirectionChanged(void) {
	if (!m_lockDirection && m_directionChanged) {
		m_directionChanged = false;
		return true;
	}
	return false;
}

// exhibition.cpp:835
float CExhibition::GetBaseAngle(float angle, float baseAngle) {
	while (angle > baseAngle + 3.14159265f) {
		angle -= 6.2831853f;
	}
	while (angle < baseAngle - 3.14159265f) {
		angle += 6.2831853f;
	}
	return angle;
}

// exhibition.cpp:855
void CExhibition::Animate(void) {
	if (m_exhibitType != 0 && m_exhibitType != 1) {
		m_light.nearone = RotVec(WVector(1.0f, -1.0f, 1.0f), m_camera);
	} else {
		float x = 0.0f;
		float y = 1.0f;
		m_light.nearone = WVector(m_camera.xx * x - m_camera.xy * y + m_camera.xz,
			m_camera.yx * x - m_camera.yy * y + m_camera.yz,
			m_camera.zx * x - m_camera.zy * y + m_camera.zz);
	}
	m_light.nearone.Normalize();

	switch (m_exhibitType) {
	case 0:
		m_pPetFrame->ApplyBones(m_pPetFrame->GetCurAniPet()->GetBones(NULL, m_frame, 0, NULL, true), m_matrix);
		break;
	case 1:
	case 6:
	case 12:
		m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix);
		break;
	case 2:
	case 4:
		m_model->ApplyBones(NULL, m_matrix);
		break;
	case 3:
		m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix * RotMat(-15.0f * kDegree, 2));
		break;
	case 5:
		m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix * RotMat(-15.0f * kDegree, 2));
		break;
	case 7:
		if (IsLocalContent((localContentType_t)0x56)) {
			Waabb box;
			box = m_model->GetBoundBox();
			m_matrix.ym = (box.max.y - box.min.y) * -0.5f;

			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix);
		}
		break;
	case 8:
		if (IsLocalContent((localContentType_t)0x56)) {
			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix);
		}
		break;
	case 9:
		if (IsLocalContent((localContentType_t)0x56)) {
			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix * RotMat(-15.0f * kDegree, 2));
		}
		break;
	case 10:
		if (IsLocalContent((localContentType_t)0x56)) {
			WMatrix scale;
			scale.Reset();
			scale *= 0.7f;
			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false),
				m_matrix * RotMat(kPi / 2.0f, 0) * RotMat(-15.0f * kDegree, 2) * scale);
		}
		break;
	case 11:
		if (IsLocalContent((localContentType_t)0x56)) {
			WMatrix scale;
			scale.Reset();
			scale *= 0.85f;
			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false),
				m_matrix * RotMat(kPi / 2.0f, 0) * RotMat(-15.0f * kDegree, 2) * scale);
		}
		break;
	case 13:
		if (IsLocalContent((localContentType_t)0x56)) {
			m_matrix.xx = m_matrix.yy = 2.0f;
			m_model->ApplyBones(m_model->GetBones(NULL, m_frame, 0, NULL, false), m_matrix);
		}
		break;
	}

	if (m_model) {
		m_model->Transform(NULL, 1.0f, 0);
		m_model->LockBoneSetPos(0xFF);
	}
}

// exhibition.cpp:986
void CExhibition::ScriptProcess(const w_motion_data* data, float from, float to, bool all) {
	if ((!(from <= data->e) || from == to) && !all) {
		return;
	}

	int start, end;
	from *= 30.0f;
	if (all) {
		start = Ceil(from, 0.03f);
		end = start;
	} else {
		to = Min(data->e, to) * 30.0f;
		start = Ceil(from, 0.03f);
		end = WAbsBySign((float)floor(to) - to) < kEpsilon ? (int)to - 1 : (int)floor(to);

		if (start == (int)floor(data->s * 30.0f + 0.5f)) {
			start++;
		}

		if (start > end) {
			return;
		}
	}

	WPuppet* pPuppet;
	if (m_exhibitType == 0) {
		pPuppet = m_pPetFrame->GetCurAniPet();
	} else {
		pPuppet = m_model;
	}

	if (pPuppet) {
		int first, last;
		if (pPuppet->CheckFrameData(start, end, first, last)) {
			for (int i = first; i <= last; i++) {
				ParsePuppetScript(pPuppet->GetFrameData(i));
			}
		}
	}
}

// exhibition.cpp:1392
void CExhibition::ParseScript_ShowVis(bool show, const char* script) {
	char token[0x100] = "";
	int count = 0;

	do {
		script = GetStrConst(script, token);

		if (strlen(token) == 0 || strlen(token) >= sizeof(token)) {
			break;
		}

		std::list<std::string>* pGroup = WSingleton<CSharedDoc>::Instance()->GetVisGroup(token);

		if (pGroup) {
			for (std::list<std::string>::iterator it = pGroup->begin(); it != pGroup->end(); ++it) {
				WBone* pBone = m_pPetFrame->GetBonePet()->FindBone(it->c_str());

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

// exhibition.cpp:1425
bool CExhibition::InValidArea(void) {
	WVector point = CInputManager::Instance()->GetMousePoint();

	if (m_areaSize.x > 0.0f && m_areaSize.y > 0.0f) {
		if (Abs(point.x - m_center.x) < m_areaSize.x * 0.5f && Abs(point.y - m_center.y) < m_areaSize.y * 0.5f) {
			return true;
		}
	} else if (m_areaRadius > 0.0f) {
		float dx = point.x - m_center.x;
		float dy = point.y - m_center.y;
		if (dx * dx + dy * dy < m_areaRadius * m_areaRadius) {
			return true;
		}
	}

	return false;
}

// exhibition.cpp:1448
void CExhibition::SetCenter(float x, float y) {
	m_center.x = x;
	m_center.y = y;

	m_fxBox.SetScreenCenter(x, y);

	float fov = g_view->GetFOV();
	g_view->SetFOV(g_view->GetRatio() * 2.0943952f);

	m_matrix.pivot = WVector(0.0f, -((float)tan(g_view->GetRatio() * g_view->GetFOV_Unmodified() * 0.6666667f) * m_curDistance * 2.0f
		* (m_areaSize.y / g_view->GetHeight() * m_screenScale)), 0.0f);

	g_view->SetFOV(fov);
	ParseBoundingBoxScript();
	if (m_pFxSequence1) { m_fxBox.DeleteSequence(m_pFxSequence1, true); m_pFxSequence1 = NULL; }
	if (m_pFxSequence2) { m_fxBox.DeleteSequence(m_pFxSequence2, true); m_pFxSequence2 = NULL; }
}

// exhibition.cpp:1484
const WVector2D& CExhibition::GetCenter(void) {
	return m_center;
}

// exhibition.cpp:1489
void CExhibition::SetArea(float radius) {
	m_areaSize.x = 0.0f;
	m_areaSize.y = 0.0f;
	m_areaRadius = radius;
}

// exhibition.cpp:1496
void CExhibition::SetArea(float width, float height) {
	m_areaRadius = 0.0f;
	m_areaSize.x = width;
	m_areaSize.y = height;
}

// exhibition.cpp:1503
bool CExhibition::SetMotion(const char* name, bool keepFx, float startFrame) {
	if (!keepFx) {
		m_fxBox.ClearAll(false);
		ParseBoundingBoxScript();
	}

	if (m_exhibitType == EXHIBIT_CHARACTER) {
		if (!m_pPetFrame) {
			return false;
		}
		m_pMotionData = m_pPetFrame->FindMotionData(name, true, NULL);
		m_pPetFrame->SetInvisible(false);
	} else {
		if (!m_model) {
			return false;
		}
		if (!m_model->GetRootBone()) {
			return false;
		}
		m_pMotionData = m_model->FindMotionData(name);
		m_model->GetRootBone()->SetInvisible(true, false);
	}

	if (m_pMotionData) {
		ScriptProcess(m_pMotionData, m_pMotionData->s, m_pMotionData->s, true);

		m_frame = Min(startFrame + m_pMotionData->s, m_pMotionData->e);
		UpdateCamera(0.0f);
		Animate();
		return true;
	}

	return false;
}

// exhibition.cpp:1547
const char* CExhibition::GetNextCharMotion(int motionSet) {
	m_motionIndex++;
	m_motionIndex %= 12;
	int motion = data_a08310[motionSet][m_motionIndex];
	if (motion == -1) {
		m_motionIndex = 0;
		motion = data_a08310[motionSet][0];
	}
	return data_a96128[motion];
}

// exhibition.cpp:1567
const char* CExhibition::GetNextCaddieMotion(int motionSet) {
	if (motionSet >= 28) {
		motionSet = 27;
	}
	m_motionIndex++;
	m_motionIndex %= 9;
	int motion = data_a084f0[motionSet][m_motionIndex];
	if (motion == -1) {
		m_motionIndex = 0;
		return data_a96158[data_a084f0[motionSet][0]];
	}
	return data_a96158[motion];
}

// exhibition.cpp:1599
w_motion_data* CExhibition::GetMotionData(void) {
	return m_pMotionData;
}

// exhibition.cpp:1609
bool CExhibition::NextMotion(void) {
	bool result;
	for (;;) {
		switch (m_exhibitType) {
		case 0:
			result = SetMotion(GetNextCharMotion(m_charMotionSet), false, 0.0f);
			break;
		case 1:
			result = SetMotion(GetNextCaddieMotion(m_caddieMotionSet), false, 0.0f);
			break;
		case 6:
			result = SetMotion("Default", false, 0.0f);
			break;
		case 3:
		case 5:
		case 12:
			result = SetMotion("default", false, 0.0f);
			break;
		case 2:
		case 4:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 13:
			return true;
		default:
			continue;
		}
		if (result) {
			return result;
		}
	}
}

// exhibition.cpp:1663
void CExhibition::Display(float clipNear, float clipFar) {
	bool fog = g_view->SetFogEnable(false);

	g_view->SetCamera(m_camera);

	float oldNear = g_view->GetClipNearValue();
	float oldFar = g_view->GetClipFarValue();
	float oldFov = g_view->GetFOV();

	float nearClip;
	float farClip;
	if (m_exhibitType == 2 || m_exhibitType == 3 || m_exhibitType == 5) {
		nearClip = 0.5f;
		farClip = 10.0f;
	} else {
		WSphere sphere;
		if (m_model) {
			m_model->UpdateBSphere();
			sphere = m_model->GetBoundSphere();
		} else {
			sphere.pos = WVector(0.0f, 0.0f, 0.0f);
			sphere.radius = 0.0f;
		}

		float distance = (m_camera.pivot - sphere.pos).Magnitude();
		float radius = m_matrix.xa.Magnitude() * sphere.radius;
		nearClip = 0.5f < distance - radius ? distance - radius : 0.5f;
		farClip = 100.0f > distance + radius ? distance + radius : 100.0f;
	}

	if (clipNear > 0.0f) {
		nearClip = clipNear;
	}
	if (clipFar > clipNear) {
		farClip = clipFar < nearClip ? nearClip : clipFar;
	}

	g_view->SetFOV(g_view->GetRatio() * 2.0943952f);
	g_view->SetClip(nearClip, farClip, false);
	g_view->SetScreenCenter(m_center.x, m_center.y);
	g_view->UpdateCamera();

	if (m_exhibitType == 0) {
		if (m_pPetFrame) {
			m_pPetFrame->UpdateLightSource(&m_light, false, NULL);
			m_pPetFrame->Render(g_view, true, 1.0f, false, false, true);
		}
	} else if (m_model) {
		m_model->UpdateLightSource(&m_light, false, NULL);
		m_model->Render(g_view, true, 1.0f, false, false, false);
	}

	m_fxBox.Display(g_view);

	g_view->Flush(1);
	g_view->SetClip(oldNear, oldFar, false);
	g_view->SetFOV(oldFov);
	g_view->ResetScreenCenter();
	g_view->UpdateCamera();

	g_view->SetFogEnable(fog);
}

// exhibition.cpp:1757
bool CExhibition::SetModel(CPartTidList& tidList, unsigned long* pItemSlots, const char* motionName, float maxDelay, int buildFlag) {
	m_motionIndex = -1;
	m_typeId = tidList.m_tidChar;
	m_pItemSlots = pItemSlots;

	ReleasePet();

	m_exhibitType = 0;
	m_light.ambient = 0x969696;
	m_pPetFrame = new CPetFrame;

	if (!m_pPetFrame->Build(&tidList, pItemSlots, true, false, buildFlag)) {
		if (m_pPetFrame) {
			delete m_pPetFrame;
			m_pPetFrame = NULL;
		}
		return false;
	}

	m_model = m_pPetFrame->GetBonePet();
	m_partTidList = tidList;

	WMatrix mat;
	mat.Reset();
	m_model->ApplyBones(NULL, mat);
	m_model->Transform(NULL, 1.0f, 0);
	m_model->UpdateBound(false, false);

	SetScreenPos(m_model);

	m_charMotionSet = m_pPetFrame->GetTidChar() & 0x3FFFFFF;

	if (motionName) {
		SetMotion(motionName, false, 0.0f);
	} else {
		SetMotion(GetNextCharMotion(m_charMotionSet), false, 0.0f);
	}

	m_motionDelay = Random(0.0f, maxDelay, -1);
	if (m_pMotionData) {
		m_initialMotionLength = m_pMotionData->e - m_pMotionData->s + m_motionDelay;
	} else {
		m_initialMotionLength = 0.0f;
	}

	m_alpha = 0.0f;
	m_pPetFrame->SetAlpha(0);

	if (tidList.m_tidChar == 0x4000005 || tidList.m_tidChar == 0x4000006) {
		m_pitchLimit = -0.2617994f; // -15 degrees
	} else {
		m_pitchLimit = 0.0f;
	}

	ParseBoundingBoxScript();

	m_pPetFrame->Refresh(&tidList);
	return true;
}

// exhibition.cpp:1829
// The statements marked "header:" wait for members the shared headers do not
// declare yet (classdefine.h, SharedDoc.h, exhibition.h).
bool CExhibition::SetModel(unsigned long typeId) {
	m_motionIndex = -1;
	m_typeId = typeId;
	m_pItemSlots = NULL;

	ReleasePet();

	switch (m_typeId >> 26) {
	case 8: {
		m_exhibitType = 1;
		m_light.ambient = 0x969696;

		IFF_STRUCT::sCadItem* pCadItem = ItemManager()->FindCadItem(m_typeId);
		if (pCadItem == NULL) {
			return false;
		}
		IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(((pCadItem->m_typeId >> 21) & 0x1F) | 0x1C000000);
		if (pCaddie == NULL) {
			return false;
		}

		m_model = g_resrcmng->GetPuppet(MakeStr("%s_ex.pet", pCaddie->m_model), false, true, false);

		if (m_model == NULL) {
			return false;
		}

		m_caddieMotionSet = pCaddie->m_typeId & 0x3FFFFFF;

		// header: sCadItem char[0x28] face texture 0x90, body texture 0xB8
		// if (strlen(pCadItem->m_faceTexture))
		//	SetPendingTexPart("face", pCadItem->m_faceTexture);
		// if (strlen(pCadItem->m_bodyTexture))
		//	SetPendingTexPart("body", pCadItem->m_bodyTexture);

		SetMotion(GetNextCaddieMotion(m_caddieMotionSet), false, 0.0f);
		break;
	}
	case 7: {
		m_exhibitType = 1;
		m_light.ambient = 0x969696;

		IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(m_typeId);
		if (pCaddie == NULL) {
			return false;
		}

		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pCaddie->m_model), false, true, false);

		if (m_model == NULL) {
			return false;
		}

		m_caddieMotionSet = pCaddie->m_typeId & 0x3FFFFFF;
		ParseBoundingBoxScript();

		SetMotion(GetNextCaddieMotion(m_caddieMotionSet), false, 0.0f);
		break;
	}
	case 5: {
		m_exhibitType = 2;
		m_light.ambient = 0x646464;

		IFF_STRUCT::sBall* pBall = ItemManager()->FindBall(m_typeId);
		if (pBall == NULL) {
			return false;
		}

		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pBall->m_model), false, true, false);

		if (m_model == NULL) {
			return false;
		}

		if (m_typeId == 0x14000005 || m_typeId == 0x1400001A || m_typeId == 0x14000080) {
			// header: exhibition.h void AttachFx(void) (public)
			// AttachFx();
		}
		break;
	}
	case 4: {
		m_exhibitType = 3;
		m_light.ambient = 0x646464;

		IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(m_typeId);
		if (pClubSet == NULL) {
			return false;
		}

		m_model = g_resrcmng->GetPuppet(MakeStr("%s_ui.pet", pClubSet->m_icon), false, true, false);
		if (m_model == NULL) {
			return false;
		}

		ParseBoundingBoxScript();
		SetMotion("default", true, 0.0f);
		break;
	}
	case 11: {
		m_exhibitType = 4;
		m_light.ambient = 0x646464;

		IFF_STRUCT::sMatch* pMatch = ItemManager()->FindMatch(typeId & 0xFFFFFF00);
		if (pMatch == NULL) {
			return false;
		}

		// header: sMatch m_icon as char[N][0x28] at 0x59, indexed by (typeId & 0xFF)
		// m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pMatch->m_icon[typeId & 0xFF]), false, true, false);
		if (m_model == NULL) {
			return false;
		}
		break;
	}
	case 16: {
		m_exhibitType = 6;
		m_light.ambient = 0x969696;

		IFF_STRUCT::sMascot* pMascot = ItemManager()->FindMascot(m_typeId);
		if (pMascot == NULL) {
			return false;
		}

		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pMascot->m_model), false, true, false);

		if (m_model == NULL) {
			return false;
		}

		ParseBoundingBoxScript();
		SetMotion("Default", false, 0.0f);

		// header: CSharedDoc std::map<unsigned int, sMascotInfo> at 0x1170
		// (the player's mascots); the message of the matching one goes into
		// the texture:
		// int index = 0;
		// for (it = map.begin(); it != map.end(); ++it) {
		//	if (it->second.typeId == m_typeId) {
		//		IFF_STRUCT::sMascot* p = ItemManager()->FindMascot(m_typeId);
		//		if (p != NULL && p->m_msgInTexture)
		//			m_texHandle = CMascot::ChangeMascotMsgInTex(it->second.message, p, m_model, MakeStr("ui%02d", index));
		//		break;
		//	}
		//	index++;
		// }
		break;
	}
	case 6: {
		m_exhibitType = 5;
		m_light.ambient = 0x646464;

		IFF_STRUCT::sItem* pItem = ItemManager()->FindItem(m_typeId);
		if (pItem == NULL) {
			return false;
		}

		// header: sItem char[0x28] model name 0x94
		// m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pItem->m_model), false, true, false);

		if (m_model == NULL) {
			return false;
		}

		ParseBoundingBoxScript();
		SetMotion("default", true, 0.0f);
		break;
	}
	case 31: {
		m_exhibitType = 12;
		m_light.ambient = 0xC8C8C8;

		IFF_STRUCT::sCard* pCard = ItemManager()->FindCard(m_typeId);
		if (pCard == NULL) {
			return false;
		}

		if (((m_typeId >> 22) & 0xF) == 3 || ((m_typeId >> 22) & 0xF) == 4) {
			m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pCard->m_model), false, true, false);
		}

		if (m_model == NULL) {
			return false;
		}

		ParseBoundingBoxScript();
		SetMotion("default", true, 0.0f);
		break;
	}
	case 9: {
		unsigned long kind = (m_typeId >> 21) & 0x1F;
		if (kind == 6 || kind == 5 || kind == 4 || kind == 8) {
			m_exhibitType = 5;
			m_light.ambient = 0x646464;

			IFF_STRUCT::sSetItem* pSetItem = ItemManager()->FindSetItem(m_typeId);

			if (pSetItem == NULL) {
				return false;
			}

			// header: sSetItem char m_icon[0x28] at 0x31
			// m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pSetItem->m_icon), false, true, false);

			if (m_model == NULL) {
				return false;
			}

			ParseBoundingBoxScript();
			SetMotion("default", true, 0.0f);
		}
		break;
	}
	case 18: {
		if (!IsLocalContent((localContentType_t)0x56)) {
			break;
		}

		m_light.ambient = 0x989898;
		IFF_STRUCT::sFurniture* pFurniture = ItemManager()->FindFurniture(m_typeId);
		if (pFurniture == NULL) {
			return false;
		}

		unsigned long kind = (pFurniture->m_typeId >> 11) & 0x7FF;

		if (kind == 8) {
			m_exhibitType = 11;
		} else if (kind == 9) {
			m_exhibitType = 10;
		} else if (kind == 14) {
			m_exhibitType = 13;
		// header: sFurniture short 0xBC (model kind)
		// } else if (pFurniture->m_modelKind == 5) {
		//	m_exhibitType = 8;
		// } else if (pFurniture->m_modelKind == 6) {
		//	m_exhibitType = 9;
		} else {
			m_exhibitType = 7;
		}

		if (m_exhibitType == 9 || m_exhibitType == 10 || m_exhibitType == 11) {
			// header: sFurniture char[0x28] model name 0x90
			// m_model = g_resrcmng->GetPuppet(MakeStr("%s_ui.pet", pFurniture->m_model), false, true, false);
		} else if (m_exhibitType == 13) {
			IFF_STRUCT::sFurniture* pPoster = ItemManager()->FindFurniture(m_typeId);
			if (pPoster == NULL) {
				return false;
			}

			// header: sFurniture char[0x28] model name 0x90
			// m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pPoster->m_model), false, true, false);

			if (m_model == NULL) {
				return false;
			}

			for (int i = 0; i < 3; i++) {
				if (strlen(pPoster->m_texture2[i]) && strlen(pPoster->m_texture[i])) {
					SetPendingTexPart(MakeStr("%s.jpg", pPoster->m_texture2[i]), MakeStr("%s.jpg", pPoster->m_texture[i]));
				}
			}
		} else {
			// header: sFurniture char[0x28] model name 0x90
			// m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", pFurniture->m_model), false, true, false);
		}

		if (m_model == NULL) {
			return false;
		}

		ParseBoundingBoxScript();
		break;
	}
	}

	WMatrix mat;
	mat.Reset();
	m_model->ApplyBones(NULL, mat);
	m_model->Transform(NULL, 1.0f, 0);
	m_model->UpdateBound(false, false);

	SetScreenPos(m_model);

	m_motionDelay = 0.0f;
	if (m_pMotionData) {
		m_initialMotionLength = m_pMotionData->e - m_pMotionData->s;
	} else {
		m_initialMotionLength = 0.0f;
	}

	m_alpha = 0.0f;
	m_model->SetAlpha(0);
	return true;
}

// exhibition.cpp:2265
bool CExhibition::SetModel(const char* name, eExhibitType type) {
	m_motionIndex = -1;
	m_typeId = 0;

	ReleasePet();

	switch (type) {
	case 1:
		m_exhibitType = 1;
		m_light.ambient = 0x969696;
		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", name), false, true, false);
		ParseBoundingBoxScript();

		SetMotion("\xb1\xe2\xba\xbb", false, 0.0f); // "default" (EUC-KR)
		break;
	case 12:
		m_exhibitType = 12;
		m_light.ambient = 0xFFFFFF;
		ParseBoundingBoxScript();
		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", name), false, true, false);
		break;
	case 2:
		m_exhibitType = 2;
		m_light.ambient = 0xC8C8C8;
		m_model = g_resrcmng->GetPuppet(MakeStr("%s.pet", name), false, true, false);
		break;
	}

	WMatrix mat;
	mat.Reset();
	m_model->ApplyBones(NULL, mat);
	m_model->Transform(NULL, 1.0f, 0);
	m_model->UpdateBound(false, false);

	SetScreenPos(m_model);

	m_motionDelay = 0.0f;
	if (m_pMotionData) {
		m_initialMotionLength = m_pMotionData->e - m_pMotionData->s;
	} else {
		m_initialMotionLength = 0.0f;
	}

	m_alpha = 0.0f;
	m_model->SetAlpha(0);
	return true;
}

// exhibition.cpp:2392
void CExhibition::SetScreenPos(WPuppet* pPuppet) {
	if (!pPuppet) {
		return;
	}

	WMatrix camera;
	camera.Reset();
	camera.pivot.z = -m_curDistance;

	float ratio = g_view->GetRatio();
	float fov = g_view->GetFOV();

	if (m_exhibitType == 0 || m_exhibitType == 1) {
		// Scale the model so that its height fills 3/4 of the area.
		WBone* pHead = pPuppet->FindBone("Bip01 Head");
		WBone* pFoot = pPuppet->FindBone("Bip01 Footsteps");
		WVector top, bottom;

		if (pHead && pFoot) {
			top = pHead->GetMatrix().pivot;
			bottom = pFoot->GetMatrix().pivot;
		} else {
			w_bound_box* pBox = pPuppet->m_bbList.Find("center");

			if (!pBox) {
				return;
			}

			top = WVector(0.0f, (pBox->info->aabb.max.y - pBox->info->aabb.min.y) * 0.75f, 0.0f);
			bottom = WVector::ZERO;
		}

		g_view->SetFOV(g_view->GetRatio() * 2.0943952f);
		g_view->SetCamera(camera);
		g_view->UpdateCamera();

		WVector screenTop = g_view->Projection(top);
		WVector screenBottom = g_view->Projection(bottom);

		float height = screenTop.y - screenBottom.y > 0.0f ? screenTop.y - screenBottom.y : -(screenTop.y - screenBottom.y);

		float scale;
		if (height < m_areaSize.y * 0.75f) {
			scale = (m_areaSize.y * 0.75f / height - 1.0f) * 0.5f + 1.0f;
		} else {
			scale = 1.0f - (1.0f - m_areaSize.y * 0.75f / height) * 0.5f;
		}

		scale = Between(0.5f, scale, 1.2f);

		m_matrix.Reset();
		m_matrix *= scale;
	} else if (m_exhibitType == 7 || m_exhibitType == 8 || m_exhibitType == 9) {
		g_view->SetFOV(ratio * 2.0943952f);

		m_matrix.Reset();

	} else {
		g_view->SetFOV(ratio * 2.0943952f);

		m_matrix.Reset();
	}

	m_defaultDistance = m_distance = m_curDistance;

	float height = (float)tan(g_view->GetFOV_Unmodified() * g_view->GetRatio() * 0.6666667f) * m_curDistance * 2.0f;
	m_matrix.pivot = WVector(0.0f, -(height * (m_areaSize.y / g_view->GetHeight() * m_screenScale)), 0.0f);

	g_view->SetFOV(fov);
	g_view->UpdateCamera();

	if (m_exhibitType == 0 || m_exhibitType == 1) {
		m_defaultAngle.x = 6.25319004f;
		m_defaultAngle.y = 0.33f;
	} else if (m_exhibitType == 7 || m_exhibitType == 8 || m_exhibitType == 9) {
		m_defaultAngle.x = 5.85318995f;
		m_defaultAngle.y = 0.33f;
	}

	m_angle = m_defaultAngle;
	m_checkedDirection = m_defaultAngle.y;
}

// exhibition.cpp:2489
void CExhibition::SetAlpha(unsigned char alpha) {
	if (m_exhibitType == 0) {
		if (m_pPetFrame) {
			m_pPetFrame->SetAlpha(alpha);
		}
	} else {
		if (m_model) {
			m_model->SetAlpha(alpha);
		}
	}
}

// exhibition.cpp:2503
// Opens the effects the model's bounding boxes name in their option script:
// "*fx <name>.spr|.seq", or "*fx(\"<name>\") \"&<part id>\"..." for a
// character, where the effect needs one of the listed parts worn.
void CExhibition::ParseBoundingBoxScript(void) {
	WPuppet* pPuppet = m_model;
	if (pPuppet == NULL) {
		return;
	}

	char token[] = "*fx";

	unsigned int i;
	for (i = 0; i < m_sprays.size(); i++)
		m_fxBox.DeleteSpray(m_sprays[i]);
	for (i = 0; i < m_sequences.size(); i++)
		m_fxBox.DeleteSequence(m_sequences[i], true);

	m_sprays.clear();
	m_sequences.clear();

	for (w_bound_box* pBox = pPuppet->m_bbList.Start(); pBox; pBox = pPuppet->m_bbList.Next()) {
		if (pBox->info->option == NULL || pBox->bone == NULL || pBox->bone->IsInvisible()) {
			continue;
		}

		const char* script = pBox->info->option;
		while (true) {
			const char* found = strstr(script, token);
			if (found == NULL)
				break;

			unsigned int pos = strlen(token);
			if (found[pos] == '(')
				pos++;

			char name[64];
			if (pos > strlen(token))
				script = GetStrConst(found + pos, name);
			else
				script = GetCharsTillSpace(found + pos, name);

			int len = strlen(name);

			if (len == 0 || len >= 64)
				continue;

			char* ext = strrchr(name, '.');
			if (ext == NULL || strlen(ext) < 4)
				continue;

			if (m_exhibitType == EXHIBIT_CHARACTER) {
				char part[64];
				script = GetStrConst(script, part);

				len = strlen(part);
				if (len == 0 || len >= 64)
					continue;

				bool partRequired = false;

				while (part[0] == '&') {
					partRequired = true;
					unsigned long tid = atoi(part + 1);

					if (m_pPetFrame->m_partList.FindPart(tid)) {
						partRequired = false;
						break;
					}

					if (m_pPetFrame->FindAuxPart(tid)) {
						partRequired = false;
						break;
					}

					script = GetStrConst(script, part);

					len = strlen(part);
					if (len == 0 || len >= 64)
						break;
				}

				if (partRequired)
					continue;
			}

			Attacher attacher;

			attacher.m_fLimitA = m_center.x; attacher.m_fLimitB = m_center.y;
			attacher.m_pMatrix[0] = &pBox->bone->GetMatrix();
			attacher.m_pOffset[0] = &pBox->info->spherePivot;

			if (stricmp(ext + 1, "spr") == 0) {
				CFxSpray* pSpray = m_fxBox.LoadSpray(name, NULL);

				if (pSpray == NULL)
					continue;

				pSpray->m_pAttacher = &attacher;
				pSpray->m_fAttachLimitA = attacher.m_fLimitA;
				pSpray->m_fAttachLimitB = attacher.m_fLimitB;
				memcpy(pSpray->m_pMatrix, attacher.m_pMatrix, sizeof(pSpray->m_pMatrix));
				memcpy(pSpray->m_pOffset, attacher.m_pOffset, sizeof(pSpray->m_pOffset));
				m_fxBox.AddSpray(pSpray);
				m_sprays.push_back(pSpray);

			} else if (stricmp(ext + 1, "seq") == 0) {
				CFxSequence* pSequence = m_fxBox.LoadSequence(name, true, NULL);

				if (pSequence == NULL)
					continue;

				pSequence->m_pAttacher = &attacher;
				pSequence->m_fDistLimitA = attacher.m_fLimitA;
				pSequence->m_fDistLimitB = attacher.m_fLimitB;
				m_fxBox.AddSequence(pSequence);
				m_sequences.push_back(pSequence);
			}
		}
	}
}

// exhibition.cpp:2676
sTexturePart* CExhibition::FindTexturePart(char* partName, char* texName) {
	sTexturePart* pTexPart;
	std::list<sTexturePart*>::iterator it;

	it = std::find_if(m_texParts.begin(), m_texParts.end(), sFindTexPart(texName));

	if (it != m_texParts.end()) {
		pTexPart = *it;
	} else {
		pTexPart = new sTexturePart;

		strcpy(pTexPart->m_partName, partName);
		strcpy(pTexPart->m_texName, texName);
		m_texParts.push_back(pTexPart);
	}

	return pTexPart;
}

// exhibition.cpp:2711
void CExhibition::SetPendingTexPart(char* partName, const char* texName) {
	sTexturePart* pTexPart = FindTexturePart(partName, (char*)texName);

	if (!pTexPart) {
		return;
	}

	if (m_exhibitType == EXHIBIT_CHARACTER) {
		if (!m_pPetFrame) {
			return;
		}
		if (pTexPart->m_pBitmap == NULL) {
			m_pPetFrame->GetBonePet()->ChangeTexturePart(m_pPetFrame->GetFaceTex(), pTexPart->m_texName, &pTexPart->m_pBitmap, &pTexPart->m_rect);
		} else {
			m_pPetFrame->GetBonePet()->ChangeTexturePart(m_pPetFrame->GetFaceTex(), pTexPart->m_pBitmap, pTexPart->m_rect);
		}
		m_pPetFrame->SetMakeup();
	} else if (m_exhibitType == 13) {
		// Swaps a whole texture of the model by handle.
		if (!m_model) {
			return;
		}
		int partHandle = g_resrcmng->LoadTexture(partName, 0, 0, 0);
		if (!partHandle) {
			return;
		}
		int texHandle = g_resrcmng->LoadTexture(texName, 0, 0, 0);
		if (!texHandle) {
			return;
		}
		m_model->ChangeTextureByHandle(texHandle, partHandle, m_model);
	} else {
		if (!m_model) {
			return;
		}
		if (pTexPart->m_pBitmap == NULL) {
			m_model->ChangeTexturePart(pTexPart->m_partName, pTexPart->m_texName, &pTexPart->m_pBitmap, &pTexPart->m_rect);
		} else {
			m_model->ChangeTexturePart(pTexPart->m_partName, pTexPart->m_pBitmap, pTexPart->m_rect);
		}
	}
}

// exhibition.cpp:2783
void CExhibition::SetChatFacial(int expression, unsigned long faceType) {
	faceType &= 0x3FFFFFF;
	if (expression >= 0 && expression < 4 && faceType < 10) {
		m_facialChanged = true;
		SetPendingTexPart("face", data_a96180[faceType][expression]);
	}
}

// exhibition.cpp:2857
bool CExhibition::ChangeBoneTexture(const char* boneName, const char* texName, const char* fileName) {
	w_texlist* texList = g_resrcmng->FindTexture(texName);
	if (!texList) {
		return false;
	}
	if (m_model && m_model->GetRootBone()) {
		return m_model->GetRootBone()->ChangeTexture(boneName, texList->texhandle, g_resrcmng->LoadTexture(fileName, 0x80000000, 0, NULL));
	}
	return false;
}

// exhibition.cpp:2872
bool CExhibition::ChangeBoneTexture(const char* boneName, const char* fileName) {
	if (m_model && m_model->GetRootBone()) {
		return m_model->GetRootBone()->ChangeTexture(boneName, g_resrcmng->LoadTexture(fileName, 0x80000000, 0, NULL));
	}
	return false;
}

// exhibition.cpp:2892
void CExhibition::DisplayClip(const WRect& rect, float fov, float clipNear, float clipFar) {
	bool fog = g_view->SetFogEnable(false);

	float oldNear = g_view->GetClipNearValue();
	float oldFar = g_view->GetClipFarValue();
	float oldFov = g_view->GetFOV();

	m_model->UpdateBSphere();

	const WSphere& sphere = m_model->GetBoundSphere();
	float distance = (m_camera.pivot - sphere.pos).Magnitude();
	float radius = m_matrix.xa.Magnitude() * sphere.radius;
	float nearClip = 0.5f < distance - radius ? distance - radius : 0.5f;
	float farClip = 100.0f > distance + radius ? distance + radius : 100.0f;

	if (clipNear > 0.0f) {
		nearClip = clipNear;
	}
	if (clipFar > clipNear) {
		farClip = clipFar < nearClip ? nearClip : clipFar;
	}

	g_view->SetClip(nearClip, farClip, false);
	g_view->SetCamera(m_camera);
	g_view->SetClippingArea(rect);
	g_view->SetFOV(fov * kDegree / g_view->GetWidth() * g_view->GetHeight() * (4.0f / 3.0f));
	g_view->SetScreenCenter(m_center.x, m_center.y);
	g_view->UpdateCamera();

	m_model->UpdateLightSource(&m_light, false, NULL);
	m_model->Render(g_view, true, 1.0f, false, false, false);
	g_view->Flush(1);

	g_view->SetCamera(g_camera);
	g_view->SetClip(oldNear, oldFar, false);
	g_view->SetFOV(oldFov);
	g_view->SetClippingArea(WRect(0.0f, 0.0f, g_view->GetWidth(), g_view->GetHeight()));
	g_view->ResetScreenCenter();
	g_view->UpdateCamera();

	g_view->SetFogEnable(fog);
}

// exhibition.cpp:2957
void CExhibition::MoveModel(const WPoint& point, const WRect* pRect, float fov) {
	float width = pRect ? pRect->w : g_view->GetWidth();
	float height = pRect ? pRect->h : g_view->GetHeight();

	// The area center and the point, unprojected at the camera distance.
	WVector from((m_center.x - width * 0.5f) * (tan(fov * kDegree * 0.5f) * m_curDistance / (width * 0.5f)),
		(m_center.y - height * 0.5f) * (tan(fov * kDegree * 0.5f) * m_curDistance / (width * 0.5f)) * -1.0f,
		m_curDistance);
	WVector to((point.x - width * 0.5f) * (tan(fov * kDegree * 0.5f) * m_curDistance / (width * 0.5f)),
		(point.y - height * 0.5f) * (tan(fov * kDegree * 0.5f) * m_curDistance / (width * 0.5f)) * -1.0f,
		m_curDistance);
	WVector move = from - to;

	if (m_model) {
		m_model->MoveBone(m_model->GetRootBone()->GetBoneName(), move);
	}
}
