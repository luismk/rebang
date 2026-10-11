// Original: d:\build\custom\temp\code\client\projectg\fx.cpp
#include <stdio.h>
#include <string.h>

#include "fx.h"
#include "golfball.h"
#include "quadtree.h"
#include "wview.h"
#include "wresrcmng.h"
#include "fxspray.h"
#include "SharedDoc.h"
#include "club.h"
#include "ItemManager.h"
#include "SoundManager.h"
#include "TaskManager.h"
#include "actor.h"
#include <math.h>

extern WMatrix g_camera;
// The main window (?g_hwnd@@3PAUHWND__@@A); its header is not known yet.
extern HWND g_hwnd;
// ?IsEqual@@YI_NMM@Z, defined in Avatar_Main; its header is not known yet.
bool IsEqual(float a, float b);

unsigned long CFxControl::ms_serial;

// fx.cpp:27
Attacher::Attacher(void) {
	m_fLimitA = 3.402823466e+38F; // FLT_MAX (no float.h in externs)
	m_fLimitB = 3.402823466e+38F; // FLT_MAX (no float.h in externs)
	for (int i = 0; i < 4; i++) {
		m_pMatrix[i] = 0;
		m_pOffset[i] = 0;
	}
}

// fx.cpp:40
bool Attacher::Get(WVector& pos) {
	pos.Reset();

	int i;
	for (i = 0; i < sizeof(m_pMatrix) / sizeof(m_pMatrix[0]); i++) {
		if (m_pOffset[i]) {
			pos += m_pMatrix[i] ? *m_pOffset[i] * *m_pMatrix[i] : *m_pOffset[i];
		} else if (m_pMatrix[i]) {
			pos += m_pMatrix[i]->pivot;
		} else {
			break;
		}
	}

	if (i > 0) {
		pos /= (float)i;
		return true;
	}
	return false;
}

// fx.cpp:71
void CPvsFxBox::ProcessAttacher(float fDelta) {
	WVector pos;

	std::map<CFxSequence*, CFxSequence*>::iterator it = m_sequences.begin();
	for (; it != m_sequences.end(); ++it) {
		CFxSequence* pSequence = it->second;
		std::map<CFxSequence*, Attacher>::iterator found = m_sequenceAttachers.find(pSequence);
		if (found != m_sequenceAttachers.end() && found->second.Get(pos)) {
			pSequence->m_pos = pos;
		}
	}

	std::map<CFxSpray*, CFxSpray*>::iterator spray = m_sprays.begin();
	for (; spray != m_sprays.end(); ++spray) {
		CFxSpray* pSpray = spray->second;

		std::map<CFxSpray*, Attacher>::iterator found = m_sprayAttachers.find(pSpray);
		if (found != m_sprayAttachers.end()) {
			if (found->second.Get(pos)) {
				pSpray->m_pos = pos;
			}
		}
	}
}

// fx.cpp:106
bool CPvsFxBox::CheckGroundCollision(const WVector& from, const WVector& to, float& height, float radius) {
	if (!(height > m_groundAreaHigh || height < m_groundAreaLow)) {
		float fromGround = GetPVS().GetGroundPoint(from, true, 0, true).y;
		float toGround = GetPVS().GetGroundPoint(to, true, 0, true).y;
		height = toGround;

		if (to.y - toGround < 0.00001f && to.y - toGround > -0.00001f) {
			return true;
		}
		if (to.y < toGround) {
			if (IsEqual(from.y, fromGround) || from.y < fromGround) {
				float top = radius + to.y;
				return IsEqual(top, toGround) || top > toGround;
			}
		} else {
			if (IsEqual(from.y, fromGround) || from.y > fromGround) {
				float bottom = to.y - radius;
				return IsEqual(bottom, toGround) || bottom < toGround;
			}
		}
		return true;
	}
	return false;
}

// fx.cpp:135
void CPvsFxBox::OnSequenceCreated(CFxSequence* pSequence) {
	const Attacher* pAttacher = pSequence->m_pAttacher;
	if (pAttacher) {
		m_sequenceAttachers.insert(std::map<CFxSequence*, Attacher>::value_type(pSequence, *pAttacher));
		pSequence->m_pAttacher = &m_sequenceAttachers[pSequence];
		memcpy(pSequence->m_pMatrix, m_sequenceAttachers[pSequence].m_pMatrix, sizeof(pSequence->m_pMatrix));
		memcpy(pSequence->m_pOffset, m_sequenceAttachers[pSequence].m_pOffset, sizeof(pSequence->m_pOffset));
	}
}

// fx.cpp:148
void CPvsFxBox::OnSequenceDeleted(CFxSequence* pSequence) {
	std::map<CFxSequence*, Attacher>::iterator it = m_sequenceAttachers.find(pSequence);
	if (it != m_sequenceAttachers.end()) {
		m_sequenceAttachers.erase(it);
	}
}

// fx.cpp:158
void CPvsFxBox::OnSprayCreated(CFxSpray* pSpray) {
	const Attacher* pAttacher = pSpray->m_pAttacher;
	if (pAttacher) {
		m_sprayAttachers.insert(std::map<CFxSpray*, Attacher>::value_type(pSpray, *pAttacher));
		pSpray->m_pAttacher = &m_sprayAttachers[pSpray];
		memcpy(pSpray->m_pMatrix, m_sprayAttachers[pSpray].m_pMatrix, sizeof(pSpray->m_pMatrix));
		memcpy(pSpray->m_pOffset, m_sprayAttachers[pSpray].m_pOffset, sizeof(pSpray->m_pOffset));
	}
}

// fx.cpp:171
void CPvsFxBox::OnSprayDeleted(CFxSpray* pSpray) {
	std::map<CFxSpray*, Attacher>::iterator it = m_sprayAttachers.find(pSpray);
	if (it != m_sprayAttachers.end()) {
		m_sprayAttachers.erase(it);
	}
}

// fx.cpp:181
void CPvsFxBox::UpdateLightSrc(const LightSet& light) {
	std::map<CFxSequence*, CFxSequence*>::iterator it;
	for (it = m_sequences.begin(); it != m_sequences.end(); ++it) {
		CFxSequence* pSequence = it->second;

		std::map<CFxSpray*, CFxSpray*>& sprays = pSequence->GetSprayList();
		std::map<CFxSpray*, CFxSpray*>::iterator itSpray;
		for (itSpray = sprays.begin(); itSpray != sprays.end(); ++itSpray) {
			itSpray->second->SetLight(light);
		}
	}

	std::map<CFxSpray*, CFxSpray*>::iterator itSpray;
	for (itSpray = m_sprays.begin(); itSpray != m_sprays.end(); ++itSpray) {
		itSpray->second->SetLight(light);
	}
}

// fx.cpp:208
CFx::CFx(void) {
	m_fxBox.Init(g_resrcmng);
	m_fxBox.SetCamera(&g_camera);
	m_fxBox.m_frameTime = 33.333f;
	m_bActive = true;
}

// fx.cpp:217
CFx::~CFx(void) {
	ClearControl();
}

// fx.cpp:223
void CFx::Process(float fDelta) {
	if (!m_bActive) {
		return;
	}

	m_fxBox.ProcessAttacher(fDelta);

	std::map<int, CFxControl*>::iterator it = m_controls.begin();
	while (it != m_controls.end()) {
		CFxControl* pControl = it->second;

		if (!pControl->Process(fDelta)) {
			m_controls.erase(it++);
			delete pControl;
		} else {
			++it;
		}
	}

	m_fxBox.Process(fDelta * 1000.0f);
}

// fx.cpp:281
void CFx::Display(void) {
	if (m_bActive) {
		m_fxBox.Display(g_view);
	}
}

// fx.cpp:290
CFxSequence* CFx::OpenSequence(const char* name, bool bOriginal) {
	CFxSequence* pSequence = m_fxBox.LoadSequence(name, bOriginal, 0);
	if (pSequence) {
		m_fxBox.AddSequence(pSequence);
	}

	return pSequence;
}

// fx.cpp:302
void CFx::CloseSequence(CFxSequence* pSequence, bool bImmediate) {
	m_fxBox.DeleteSequence(pSequence, bImmediate);
}

// fx.cpp:308
CFxSpray* CFx::OpenSpray(const char* name) {
	CFxSpray* pSpray = m_fxBox.LoadSpray(name, 0);
	if (pSpray) {
		m_fxBox.AddSpray(pSpray);
	}

	return pSpray;
}

// fx.cpp:320
void CFx::CloseSpray(CFxSpray* pSpray) {
	m_fxBox.DeleteSpray(pSpray);
}

// fx.cpp:326
void CFx::ClearAll(bool bKeepActive) {
	ClearControl();
	m_fxBox.ClearAll(bKeepActive);
}

// fx.cpp:332
void CFx::AddFxKeeper(sFxKeeper& keeper) {
	m_fxBox.AddFxKeeper(keeper);
}

// fx.cpp:338
void CFx::EraseFxKeeper(CFxKeeper* pKeeper) {
	m_fxBox.EraseFxKeeper(pKeeper);
}

// fx.cpp:344
void CFx::ClearFxKeeper(void) {
	m_fxBox.ClearFxKeeper();
}

// fx.cpp:349
CFxSpray* CFx::GetSpray(int index) {
	return m_fxBox.GetSpray(index);
}

// fx.cpp:355
bool CFx::FindSequence(CFxSequence* pSequence) {
	return m_fxBox.FindSequence(pSequence);
}

// fx.cpp:361
bool CFx::FindSpray(CFxSpray* pSpray) {
	return m_fxBox.FindSpray(pSpray);
}

// fx.cpp:367
void* CFx::Attach(const Attacher& attacher, const char* name, bool bOriginal, defines_t* pDefines) {
	int len = strlen(name);
	if (len < 5) {
		return 0;
	}

	if (_strcmpi(name + len - 3, "spr") == 0) {
		CFxSpray* pSpray = m_fxBox.LoadSpray(name, pDefines);
		if (pSpray == 0) {
			return 0;
		}

		pSpray->m_bAlive = bOriginal;
		pSpray->m_pAttacher = &attacher;
		pSpray->m_fAttachLimitA = attacher.m_fLimitA;
		pSpray->m_fAttachLimitB = attacher.m_fLimitB;
		m_fxBox.AddSpray(pSpray);
		return pSpray;
	} else {
		CFxSequence* pSequence = m_fxBox.LoadSequence(name, true, pDefines);
		if (pSequence == 0) {
			return 0;
		}

		pSequence->m_bAlive = bOriginal;
		pSequence->m_pAttacher = &attacher;
		pSequence->m_fDistLimitA = attacher.m_fLimitA;
		pSequence->m_fDistLimitB = attacher.m_fLimitB;
		m_fxBox.AddSequence(pSequence);
		return pSequence;
	}
}

// fx.cpp:414
bool CFx::GetOutput(WVector& pos, unsigned char& alpha) {
	return m_fxBox.GetOutput(pos, alpha);
}

// fx.cpp:420
CFxControl* CFx::FindControl(int serial) {
	std::map<int, CFxControl*>::iterator it = m_controls.find(serial);
	if (it != m_controls.end()) {
		return it->second;
	}

	return 0;
}

// fx.cpp:430
int CFx::MakeRunDust(const WVector& pos, const WVector& dir) {
	return AddFxControl(new CFxRunDust(m_fxBox, pos, dir));
}

// fx.cpp:436
bool CFx::RemoveRunDust(int serial) {
	std::map<int, CFxControl*>::iterator it = m_controls.find(serial);
	if (it != m_controls.end()) {
		CFxControl* pControl = it->second;
		m_controls.erase(it);
		delete pControl;
		return true;
	}

	return false;
}

// fx.cpp:450
int CFx::MakeCobra(const CGolfBall& ball) {
	return AddFxControl(new CFxCobra(m_fxBox, ball));
}

// fx.cpp:455
int CFx::MakeTomahawk(const CGolfBall& ball, bool bVariant) {
	return AddFxControl(new CFxTomahawk(m_fxBox, ball, bVariant));
}

// fx.cpp:460
int CFx::MakePowerSpin(CGolfBall& ball) {
	return AddFxControl(new CFxPowerSpin(m_fxBox, ball));
}

// fx.cpp:466
int CFx::MakeShotImpact(CGolfBall& ball) {
	return AddFxControl(new CFxShotImpact(m_fxBox, ball));
}

// fx.cpp:472
int CFx::MakeClubImpact(CGolfBall& ball, unsigned long clubId) {
	return AddFxControl(new CFxClubImpact(m_fxBox, ball, clubId));
}

// fx.cpp:478
int CFx::MakeSpike(const CGolfBall& ball, bool bVariant) {
	return AddFxControl(new CFxSpike(m_fxBox, ball, bVariant));
}

// fx.cpp:484
int CFx::MakePowerShot2(CGolfBall& ball) {
	return AddFxControl(new CFxPowerShot2(m_fxBox, ball));
}

// fx.cpp:490
int CFx::MakeBoosterWarp(CGolfBall& ball, unsigned char type) {
	return AddFxControl(new CFxBoosterWarp(m_fxBox, ball, type));
}

// fx.cpp:503
int CFx::AddFxControl(CFxControl* pControl) {
	m_controls.insert(std::map<int, CFxControl*>::value_type(pControl->m_serial, pControl));
	return pControl->m_serial;
}

// fx.cpp:510
void CFx::ClearControl(void) {
	for (std::map<int, CFxControl*>::iterator it = m_controls.begin(); it != m_controls.end(); ++it) {
		delete it->second;
	}

	m_controls.clear();
}

// fx.cpp:521
void CFx::SetActive(bool bActive) {
	m_bActive = bActive;
}

// fx.cpp:527
void CFx::UpdateExtWind(WVector wind) {
	std::map<CFxSpray*, CFxSpray*>::iterator it;
	for (it = m_fxBox.GetSprayList().begin(); it != m_fxBox.GetSprayList().end(); ++it) {
		it->second->SetWind(wind);
	}

	std::map<CFxSequence*, CFxSequence*>& sequences = m_fxBox.GetSequenceList();
	for (std::map<CFxSequence*, CFxSequence*>::iterator seq = sequences.begin(); seq != sequences.end(); ++seq) {
		seq->second->m_extWind = wind;
	}
}

// fx.cpp:553
CFxRunDust::CFxRunDust(CPvsFxBox& box, const WVector& pos, const WVector& dir) : m_box(box), m_pos(pos), m_dir(dir) {
	m_pGroundSequence = m_box.LoadSequence("GroundRun.seq", false, 0);
	m_pWaterSequence = m_box.LoadSequence("WaterRun.seq", false, 0);

	if (m_pGroundSequence && m_pWaterSequence) {
		m_box.AddSequence(m_pGroundSequence);
		m_box.AddSequence(m_pWaterSequence);
	}

	m_serial = ms_serial++;
}

// fx.cpp:571
CFxRunDust::~CFxRunDust(void) {
	m_box.DeleteSequence(m_pGroundSequence, false);
	m_box.DeleteSequence(m_pWaterSequence, false);

	delete m_pGroundSequence;
	delete m_pWaterSequence;
}

// fx.cpp:581
bool CFxRunDust::Process(float fDelta) {
	WVector pos = m_pos;
	pos.y = GetPVS().GetGroundPoint(m_pos, true, 0, true).y;

	if (m_dir.Magnitude() < 0.5f || pos.y + 1.2f < m_pos.y) {
		m_pGroundSequence->SetActive(false, false);
		m_pWaterSequence->SetActive(false, false);
	} else {
		CFxSequence* pSequence;
		if (GetPVS().IsOverWater(pos)) {
			m_pGroundSequence->SetActive(false, false);
			m_pWaterSequence->SetActive(true, false);

			pSequence = m_pWaterSequence;
		} else {
			m_pGroundSequence->SetActive(true, false);
			m_pWaterSequence->SetActive(false, false);

			pSequence = m_pGroundSequence;
		}

		pSequence->Pos() = pos;
	}

	return true;
}

// fx.cpp:623
CFxCobra::CFxCobra(CPvsFxBox& box, const CGolfBall& ball) : m_box(box), m_ball(ball) {
	m_bFinished = false;

	m_pSequences[0] = m_box.LoadSequence("Cobra_BG.seq", false, 0);

	m_pSequences[1] = m_box.LoadSequence("Cobra_BW.seq", false, 0);
	m_pSequences[2] = m_box.LoadSequence("Cobra_BR.seq", false, 0);
	m_pSequences[3] = m_box.LoadSequence("Cobra_C.seq", false, 0);

	if (m_pSequences[0] && m_pSequences[1] && m_pSequences[2] && m_pSequences[3]) {
		m_box.AddSequence(m_pSequences[0]);
		m_box.AddSequence(m_pSequences[1]);
		m_box.AddSequence(m_pSequences[2]);
		m_box.AddSequence(m_pSequences[3]);
	}

	m_serial = ms_serial++;
}

// fx.cpp:647
CFxCobra::~CFxCobra(void) {
	for (int i = 0; i < 4; i++) {
		m_box.DeleteSequence(m_pSequences[i], false);
		delete m_pSequences[i];
		m_pSequences[i] = 0;
	}
}

// fx.cpp:658
bool CFxCobra::Process(float fDelta) {
	if (!(m_ball.m_flags & 0x20) || !m_pSequences[0] || !m_pSequences[1] || !m_pSequences[2] || !m_pSequences[3]) {
		return false;
	}

	WVector pos = m_ball.m_pos;
	pos.y = GetPVS().GetGroundPoint(m_ball.m_pos, true, 0, true).y;

	if (m_bFinished) {
		if (m_ball.m_velocity.y < 0.0f) {
			return false;
		}

		m_pSequences[3]->Pos() = m_ball.m_pos;

		if (pos.y + 22.4f <= m_ball.m_pos.y || m_ball.m_velocity.Magnitude() < 0.5f) {
			m_pSequences[0]->SetActive(false, false);
			m_pSequences[1]->SetActive(false, false);
			m_pSequences[2]->SetActive(false, false);
		} else if (GetPVS().IsOverWater(pos)) {
			m_pSequences[0]->SetActive(false, false);
			m_pSequences[1]->SetActive(true, false);
			m_pSequences[2]->SetActive(true, false);

			m_pSequences[1]->Pos() = pos;
			m_pSequences[2]->Pos() = pos;
		} else {
			m_pSequences[0]->SetActive(true, false);
			m_pSequences[1]->SetActive(false, false);
			m_pSequences[2]->SetActive(false, false);

			m_pSequences[0]->Pos() = pos;
		}
	} else if (m_ball.m_velocity.Magnitude() > 1.0f) {
		m_bFinished = true;
		CFxSequence* pSequence = m_box.LoadSequence("Cobra_A.seq", true, 0);
		m_box.AddSequence(pSequence);
		pSequence->Pos() = m_ball.m_pos;
	}

	return true;
}

// fx.cpp:734
CFxTomahawk::CFxTomahawk(CPvsFxBox& box, const CGolfBall& ball, bool bVariant) : m_box(box), m_ball(ball) {
	m_state = 0;
	m_pSequence = 0;
	m_serial = ms_serial++;
	m_bVariant = bVariant;
}

// fx.cpp:744
CFxTomahawk::~CFxTomahawk(void) {
	if (m_pSequence) {
		m_box.DeleteSequence(m_pSequence, false);
		delete m_pSequence;
		m_pSequence = 0;
	}
}

// fx.cpp:755
bool CFxTomahawk::Process(float fDelta) {
	WVector ground = GetPVS().GetGroundPoint(m_ball.m_pos, true, 0, true);

	if (m_ball.m_eventPreDataIndex[10] > 0 && m_ball.m_preDataIndex >= m_ball.m_eventPreDataIndex[10]) {
		m_state = 3;
	}
	switch (m_state) {
	case 0:
		if (m_ball.m_velocity.Magnitude() > 1.0f) {
			m_state = 1;
			CFxSequence* pSequence = m_box.LoadSequence("Tomahawk_A.seq", true, 0);
			if (!pSequence) {
				return false;
			}
			m_box.AddSequence(pSequence);
			pSequence->Pos() = m_ball.m_pos;

			m_pSequence = m_box.LoadSequence("Tomahawk_B.seq", false, 0);
			if (!m_pSequence) {
				return false;
			}
			m_box.AddSequence(m_pSequence);
		}
		break;
	case 1:
		if (m_ball.m_velocity.y < 0.6f) {
			m_state = 2;
			if (m_pSequence) {
				m_box.DeleteSequence(m_pSequence, false);
				delete m_pSequence;
				m_pSequence = 0;
			}

			m_pSequence = m_box.LoadSequence("Tomahawk_C.seq", false, 0);
			if (!m_pSequence) {
				return false;
			}
			m_box.AddSequence(m_pSequence);
		}
		break;
	case 3:
		if (m_pSequence) {
			m_box.DeleteSequence(m_pSequence, false);
			delete m_pSequence;
			m_pSequence = 0;
		}

		CFxSequence* pSequence = m_box.LoadSequence("Tomahawk_D.seq", true, 0);
		if (pSequence) {
			m_box.AddSequence(pSequence);
			pSequence->Pos() = m_ball.m_pos;

			g_audio->PlaySfx("\xb0\xf8_\xc5\xe4\xb8\xb6\xc8\xa3\xc5\xa9", 0, 0, 0, 0, 0.5f, 200.0f);

			if (m_bVariant) {
				AfxGetTask()->GetActor("Trajectory") << MsgObject(0, 0x15F, 0, 0, 0, 0, 0);
			}
		}
		return false;
	}

	if (m_pSequence) {
		m_pSequence->Pos() = m_ball.m_pos;
	}
	return true;
}

// fx.cpp:851
CFxPowerSpin::CFxPowerSpin(CPvsFxBox& box, CGolfBall& ball) : m_box(box), m_ball(ball) {
	m_bEnd = false;

	char name[32];
	switch (GetPVS().GetGroundType(m_ball.m_pos, true, 0)) {
	case 3:
	case 6:
		sprintf(name, "PowerSpin_%s_S.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		break;

	case 5:
		sprintf(name, "PowerSpin_%s_P.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		break;

	case 9:
		sprintf(name, "PowerSpin_%s_W.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		break;

	default:
		if (!_stricmp(CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0, "pola")) {
			sprintf(name, "PowerSpin_icee_G.seq");
		} else {
			sprintf(name, "PowerSpin_%s_G.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		}
		break;
	}

	m_pSequence = m_box.LoadSequence(name, true, 0);

	if (!m_pSequence) {
		m_pSequence = m_box.LoadSequence("PowerSpin_Blue_G.seq", true, 0);
	}

	if (m_pSequence) {
		m_box.AddSequence(m_pSequence);
	}

	m_serial = ms_serial++;
}

// fx.cpp:901
CFxPowerSpin::~CFxPowerSpin(void) {
}

// fx.cpp:906
bool CFxPowerSpin::Process(float fDelta) {
	if (!m_box.FindSequence(m_pSequence)) {
		return false;
	}

	if (!m_bEnd) {
		std::map<CFxSpray*, CFxSpray*>& sprays = m_pSequence->GetSprayList();
		if (sprays.size() != 0) {
			WVector dir = m_ball.PreData(m_ball.m_preDataIndex)->m_pos - m_ball.PreData(0)->m_pos;
			dir.y = 0.0f;
			dir = (m_ball.m_flags & 1) ? dir.Normalize() * -1.0f : dir.Normalize();

			float angle = (float)acos(dir.x * 0.0f + dir.y * 0.0f - dir.z * 1.0f);
			if (dir.x < 0.0f) {
				angle = 6.2831855f - angle;
			}
			WMatrix mat = RotMat(WVector(0.0f, 1.0f, 0.0f), angle);

			for (std::map<CFxSpray*, CFxSpray*>::const_iterator it = sprays.begin(); it != sprays.end(); ++it) {
				CFxSpray* pSpray = it->second;
				pSpray->InitialMaxVel() = pSpray->InitialMaxVel() * mat;
				pSpray->InitialMinVel() = pSpray->InitialMinVel() * mat;
			}

			m_bEnd = true;
		}
	} else {
		std::map<CFxSpray*, CFxSpray*>& sprays = m_pSequence->GetSprayList();
		for (std::map<CFxSpray*, CFxSpray*>::const_iterator it = sprays.begin(); it != sprays.end(); ++it) {
			it->second->m_pos = m_ball.m_pos;
		}
	}

	return true;
}

// fx.cpp:956
CFxShotImpact::CFxShotImpact(CPvsFxBox& box, CGolfBall& ball) : m_box(box), m_ball(ball) {
	m_bEnd = false;

	char name[32];
	switch (GetPVS().GetGroundType(m_ball.m_pos, false, 0)) {
	case 3:
		sprintf(name, "ShotImpact_%s_S.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		m_pSequence = m_box.LoadSequence(name, true, 0);
		break;

	case 5:
	case 7:
	case 8:
		m_pSequence = m_box.LoadSequence("ShotImpact_P.seq", true, 0);
		break;

	case 4:
		sprintf(name, "ShotImpact_%s_G.seq", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0);
		m_pSequence = m_box.LoadSequence(name, true, 0);
		break;

	case 0:
		m_pSequence = m_box.LoadSequence("ShotImpact_T.seq", true, 0);
		break;

	case 9:
		m_pSequence = m_box.LoadSequence("ShotImpact_W.seq", true, 0);
		break;

	default:
		m_pSequence = 0;
		break;
	}

	if (m_pSequence) {
		m_box.AddSequence(m_pSequence);

		m_pSequence->Pos() = m_ball.PreData(0)->m_pos;
	}

	m_serial = ms_serial++;
}

// fx.cpp:1019
CFxShotImpact::~CFxShotImpact(void) {
}

// fx.cpp:1024
bool CFxShotImpact::Process(float fDelta) {
	if (!m_box.FindSequence(m_pSequence)) {
		return false;
	}

	if (!m_bEnd) {
		std::map<CFxSpray*, CFxSpray*>& sprays = m_pSequence->GetSprayList();
		if (sprays.size() != 0) {
			WVector dir = m_ball.PreData(0)->m_velocity;
			dir.y = 0.0f;
			dir.Normalize();

			float angle = (float)acos(dir.x * 0.0f + dir.y * 0.0f - dir.z * 1.0f);
			if (dir.x < 0.0f) {
				angle = 6.2831855f - angle;
			}
			WMatrix mat = RotMat(WVector(0.0f, 1.0f, 0.0f), angle);

			for (std::map<CFxSpray*, CFxSpray*>::const_iterator it = sprays.begin(); it != sprays.end(); ++it) {
				CFxSpray* pSpray = it->second;
				pSpray->InitialMaxVel() = pSpray->InitialMaxVel() * mat;
				pSpray->InitialMinVel() = pSpray->InitialMinVel() * mat;
			}

			m_bEnd = true;
		}
	}

	return true;
}

// fx.cpp:1062
// Opens the impact effect of the club set clubId for the club type in hand
// ("<icon>_<w|i|s|p>_impact.seq", else the ".spr" of that name) at the ball.
CFxClubImpact::CFxClubImpact(CPvsFxBox& box, CGolfBall& ball, unsigned long clubId)
	: m_box(box), m_ball(ball) {
	const char* typeNames[4] = { "w", "i", "s", "p" };
	char name[256];

	IFF_STRUCT::sClubSet* pClubSet = ItemManager()->FindClubSet(clubId);

	sprintf(name, "%s_%s_impact.seq", pClubSet->m_icon, typeNames[GolfClub().GetType()]);

	CFxSequence* pSequence = m_box.LoadSequence(name, true, 0);

	if (pSequence) {
		m_box.AddSequence(pSequence);
		pSequence->Pos() = m_ball.PreData(0)->m_pos;
	} else {
		sprintf(name, "%s_%s_impact.spr", pClubSet->m_icon, typeNames[GolfClub().GetType()]);

		CFxSpray* pSpray = m_box.LoadSpray(name, 0);

		if (pSpray) {
			m_box.AddSpray(pSpray);
			pSpray->m_pos = m_ball.PreData(0)->m_pos;
		}
	}
}

// fx.cpp:1095
CFxClubImpact::~CFxClubImpact(void) {
}

// fx.cpp:1100
bool CFxClubImpact::Process(float fDelta) {
	return false;
}

// fx.cpp:1110
CFxSpike::CFxSpike(CPvsFxBox& box, const CGolfBall& ball, bool bVariant) : m_box(box), m_ball(ball) {
	m_pSequences[0] = m_box.LoadSequence("Spike_C.seq", false, 0);
	m_pSequences[1] = m_box.LoadSequence("Spike_ball.seq", false, 0);

	m_state = 0;
	m_serial = ms_serial++;
	m_bVariant = bVariant;
}

// fx.cpp:1122
CFxSpike::~CFxSpike(void) {
	for (int i = 0; i < 2; i++) {
		if (m_pSequences[i]) {
			m_box.DeleteSequence(m_pSequences[i], false);
			delete m_pSequences[i];
		}
	}
}

// fx.cpp:1135
bool CFxSpike::Process(float fDelta) {
	WVector ground = GetPVS().GetGroundPoint(m_ball.m_pos, true, 0, true);

	if (m_ball.m_eventPreDataIndex[14] > 0 && m_ball.m_preDataIndex >= m_ball.m_eventPreDataIndex[14]) {
		m_state = 4;
	}
	switch (m_state) {
	case 0: {
		m_box.AddSequence(m_pSequences[0]);
		m_pSequences[0]->Pos() = m_ball.m_pos;

		CFxSequence* pSequence = m_box.LoadSequence("Spike_A.seq", true, 0);
		if (!pSequence) {
			return false;
		}
		m_box.AddSequence(pSequence);
		pSequence->Pos() = m_ball.m_pos;

		m_state = 1;
		break;
	}
	case 1:
		if (m_ball.m_eventPreDataIndex[1] < m_ball.m_preDataIndex) {
			CFxSequence* pSequence = m_box.LoadSequence("spike_shot-01.seq", true, 0);
			if (!pSequence) {
				return false;
			}
			m_box.AddSequence(pSequence);
			pSequence->Pos() = m_ball.m_pos;

			g_audio->PlaySfx("spike_ball_duration", 0, 0, 0, 0, 0.5f, 200.0f);

			m_state = 2;
		} else {
			m_pSequences[0]->Pos() = m_ball.m_pos;
		}

		if (!(m_ball.m_flags & 0x40)) {
			return false;
		}
		break;
	case 2:
		if (m_ball.m_eventPreDataIndex[1] + 60 < m_ball.m_preDataIndex) {
			CFxSequence* pSequence = m_box.LoadSequence("Spike_shot.seq", true, 0);
			m_box.AddSequence(pSequence);
			pSequence->Pos() = m_ball.m_pos;

			m_box.AddSequence(m_pSequences[1]);
			m_pSequences[1]->Pos() = m_ball.m_pos;

			m_pSequences[0]->SetActive(false, false);

			g_audio->PlaySfx("spike_beam", 0, 0, 0, 0, 0.5f, 200.0f);

			m_state = 3;
		}
		break;
	case 3:
		m_pSequences[1]->Pos() = m_ball.m_pos;
		break;
	case 4: {
		CFxSequence* pSequence = m_box.LoadSequence("Tomahawk_D.seq", true, 0);
		if (!pSequence) {
			return false;
		}
		m_box.AddSequence(pSequence);
		pSequence->Pos() = m_ball.m_pos;

		m_pSequences[1]->SetActive(false, false);

		g_audio->PlaySfx("\xb0\xf8_\xc5\xe4\xb8\xb6\xc8\xa3\xc5\xa9", 0, 0, 0, 0, 0.5f, 200.0f);

		if (m_bVariant) {
			AfxGetTask()->GetActor("Trajectory") << MsgObject(0, 0x15F, 0, 0, 0, 0, 0);
		}
		return false;
	}
	}

	return true;
}

// fx.cpp:1241
CFxPowerShot2::CFxPowerShot2(CPvsFxBox& box, CGolfBall& ball) : m_box(box), m_ball(ball) {
	m_serial = ms_serial++;

	m_bDone = false;
}

// fx.cpp:1249
CFxPowerShot2::~CFxPowerShot2(void) {
}

// fx.cpp:1254
bool CFxPowerShot2::Process(float fDelta) {
	if (!m_bDone) {
		CFxSequence* pSequence = m_box.LoadSequence("woodwave2.seq", true, 0);

		if (pSequence) {
			m_box.AddSequence(pSequence);

			WVector pos = m_ball.PreData(0)->m_pos;
			pos.y += 1.6f;

			pSequence->Pos() = pos;
		} else {
			MessageBoxA(g_hwnd, "Couldn't load 'woodwave2.seq'.", "Error", 0);
		}

		m_bDone = true;
	}
	return true;
}

// fx.cpp:1430
CFxBoosterWarp::~CFxBoosterWarp(void) {
	for (int i = 0; i < 2; i++) {
		if (m_pSequences[i]) {
			m_box.DeleteSequence(m_pSequences[i], false);
			delete m_pSequences[i];
			m_pSequences[i] = 0;
		}
	}
}

// fx.cpp:1444
bool CFxBoosterWarp::Process(float fDelta) {
	if (m_harpMode == 1) {
		m_elapsed += fDelta;
	}

	if (2.0f < m_elapsed) {
		m_pSequences[0]->SetActive(false, false);
		m_pSequences[1]->SetActive(false, false);
		return false;
	}

	if (m_bLoaded) {
		if (m_ball.m_velocity.Magnitude() < 0.5f) {
			m_pSequences[0]->SetActive(false, false);
			m_pSequences[1]->SetActive(false, false);
		} else {
			m_pSequences[0]->Pos() = m_ball.m_pos;
		}
	}

	return true;
}
