// Original: d:\build\custom\temp\code\client\projectg\trainingoptiondlg.cpp
#include <math.h>

#include "TrainingOptionDlg.h"
#include "ObjectFactory.h"
#include "commonutil.h"
#include "frbutton.h"
#include "fredit.h"
#include "SharedDoc.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"
#include "TaskManager.h"
#include "actor.h"
#include "golfdoc.h"
#include "scenemanager.h"
#include "wind.h"

// 180 / pi: the wind direction in radians to degrees.
const float kRadToDeg = 57.29578f;

// trainingoptiondlg.cpp:19 (run-time type and factory registration of
// FrTrainingOptionDlg)
const WRTTI FrTrainingOptionDlg::m_RTTI("FrTrainingOptionDlg", &FrForm::m_RTTI);

IObject* FrTrainingOptionDlgMakeInstance(void) {
	return new FrTrainingOptionDlg;
}

struct __sFrTrainingOptionDlg {
	__sFrTrainingOptionDlg(void) {
		ObjectFactory().AddObjectFunctor(FrTrainingOptionDlgMakeInstance, "FrTrainingOptionDlg");
	}
};
static __sFrTrainingOptionDlg s_registerFrTrainingOptionDlg;

// trainingoptiondlg.cpp:21
const sFRESH_MSGMAP* FrTrainingOptionDlg::GetMessageMap(void) const {
	return &FrTrainingOptionDlg::_MsgMap;
}

sFRESH_MSGMAP FrTrainingOptionDlg::_MsgMap = { &FrForm::_MsgMap, &FrTrainingOptionDlg::_MsgEntries[0] };

// trainingoptiondlg.cpp:22
sFRESH_ENTRY FrTrainingOptionDlg::_MsgEntries[] = {
	// trainingoptiondlg.cpp:23
	{ "map", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnMapInit },
	// trainingoptiondlg.cpp:24
	{ "mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnMapPrevBtnInit },
	// trainingoptiondlg.cpp:25
	{ "mapprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnMapPrevBtnUp },
	// trainingoptiondlg.cpp:26
	{ "mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnMapNextBtnInit },
	// trainingoptiondlg.cpp:27
	{ "mapnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnMapNextBtnUp },
	// trainingoptiondlg.cpp:28
	{ "weather", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnWeatherInit },
	// trainingoptiondlg.cpp:29
	{ "weatherprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnWeatherPrevBtnInit },
	// trainingoptiondlg.cpp:30
	{ "weatherprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnWeatherPrevBtnUp },
	// trainingoptiondlg.cpp:31
	{ "weathernext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnWeatherNextBtnInit },
	// trainingoptiondlg.cpp:32
	{ "weathernext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnWeatherNextBtnUp },
	// trainingoptiondlg.cpp:33
	{ "direction", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnDirectionInit },
	// trainingoptiondlg.cpp:34
	{ "directionprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnDirectionPrevBtnInit },
	// trainingoptiondlg.cpp:35
	{ "directionprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnDirectionPrevBtnUp },
	// trainingoptiondlg.cpp:36
	{ "directionnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnDirectionNextBtnInit },
	// trainingoptiondlg.cpp:37
	{ "directionnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnDirectionNextBtnUp },
	// trainingoptiondlg.cpp:38
	{ "intensity", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnIntensityInit },
	// trainingoptiondlg.cpp:39
	{ "intensityprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnIntensityPrevBtnInit },
	// trainingoptiondlg.cpp:40
	{ "intensityprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnIntensityPrevBtnUp },
	// trainingoptiondlg.cpp:41
	{ "intensitynext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrTrainingOptionDlg::OnIntensityNextBtnInit },
	// trainingoptiondlg.cpp:42
	{ "intensitynext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnIntensityNextBtnUp },
	// trainingoptiondlg.cpp:43
	{ "ok", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrTrainingOptionDlg::OnOkBtnUp },

	// trainingoptiondlg.cpp:45
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// trainingoptiondlg.cpp:48
FrTrainingOptionDlg::FrTrainingOptionDlg(void) {
	m_bFlag14c = false;
	m_map = 0;
}

// trainingoptiondlg.cpp:54
void FrTrainingOptionDlg::OnOkBtnUp(void) {
	if (m_map != GetCurMap()) {
		SetCurMap(m_map);

		GOLFDOC()->m_currentHole = 1;

		AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0x271a, 0, 0, 0, 0, 0);
		AfxGetTask()->GetActor("GolfBg") << MsgObject(0, 0x140, 0, 0, 0, 0, 0);
		AfxGetTask()->GetActor("GolfBg") << MsgObject(0, 0x13e, 0, 0, 0, 0, 0);
		AfxGetTask()->GetActor("GolfRule") << MsgObject(0, 0x99, 0, 0, 0, 0, 0);
	}

	CSceneManager::Instance()->SetWeather(m_weather, false);

	OnFreshOkay();
}

// trainingoptiondlg.cpp:82
void FrTrainingOptionDlg::OnMapInit(intptr_t param) {
	m_pMapEdit = DYNAMIC_CAST(FrEdit, param);

	if (m_pMapEdit) {
		IFF_STRUCT::sDesc* pDesc = ItemManager()->FindDesc(0x28000000 | (m_map > 127 && m_map != 253 ? m_map - 128 : m_map));

		if (pDesc)
			m_pMapEdit->SetLine(1, pDesc->m_text, 0, false, 0);
		else
			m_pMapEdit->SetLine(1, "", 0, false, 0);
	}
}

// trainingoptiondlg.cpp:97
void FrTrainingOptionDlg::OnMapPrevBtnInit(intptr_t param) {
	m_pMapPrevBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pMapPrevBtn) {
		m_pMapPrevBtn->SetPushDelay(0.0f);
		m_pMapPrevBtn->Enable(false);
	}
}

// trainingoptiondlg.cpp:107
void FrTrainingOptionDlg::OnMapPrevBtnUp(void) {
	std::map<unsigned int, IFF_STRUCT::sCourse>& courseMap = ItemManager()->m_courseMap;
	std::map<unsigned int, IFF_STRUCT::sCourse>::const_iterator it;

	for (it = courseMap.begin(); it != courseMap.end(); ++it) {
		if (it->first == (0x28000000 | (m_map > 127 && m_map != 253 ? m_map - 128 : m_map))) {
			--it;
			m_map = (unsigned char)it->first;

			if (it == courseMap.begin())
				m_pMapPrevBtn->Enable(false);
			m_pMapNextBtn->Enable(true);
			break;
		}
	}

	OnMapInit((intptr_t)m_pMapEdit);
}

// trainingoptiondlg.cpp:129
void FrTrainingOptionDlg::OnMapNextBtnInit(intptr_t param) {
	m_pMapNextBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pMapNextBtn) {
		m_pMapNextBtn->SetPushDelay(0.0f);
		m_pMapNextBtn->Enable(false);
	}
}

// trainingoptiondlg.cpp:139
void FrTrainingOptionDlg::OnMapNextBtnUp(void) {
	std::map<unsigned int, IFF_STRUCT::sCourse>& courseMap = ItemManager()->m_courseMap;
	std::map<unsigned int, IFF_STRUCT::sCourse>::const_iterator it;

	for (it = courseMap.begin(); it != courseMap.end(); ++it) {
		if (it->first == (0x28000000 | (m_map > 127 && m_map != 253 ? m_map - 128 : m_map))) {
			++it;
			m_map = (unsigned char)it->first;

			++it;

			m_pMapPrevBtn->Enable(true);
			if (it == courseMap.end())
				m_pMapNextBtn->Enable(false);
			break;
		}
	}

	OnMapInit((intptr_t)m_pMapEdit);
}

// trainingoptiondlg.cpp:163
void FrTrainingOptionDlg::OnWeatherInit(intptr_t param) {
	m_pWeatherEdit = DYNAMIC_CAST(FrEdit, param);

	m_weather = GetWeather();
	if (m_pWeatherEdit) {
		switch (m_weather) {
		case 0:
			m_pWeatherEdit->AddLine("\xb8\xbc\xc0\xbd", 0, false);
			break;
		case 3:
			m_pWeatherEdit->AddLine("\xb4\xab", 0, false);
			break;
		case 2:
			m_pWeatherEdit->AddLine("\xba\xf1", 0, false);
			break;
		}
	}
}

// trainingoptiondlg.cpp:185
void FrTrainingOptionDlg::OnWeatherPrevBtnInit(intptr_t param) {
	m_pWeatherPrevBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pWeatherPrevBtn) {
		m_pWeatherPrevBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_weather == 0)
				m_pWeatherPrevBtn->Enable(false);
		} else {
			m_pWeatherPrevBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:205
void FrTrainingOptionDlg::OnWeatherPrevBtnUp(void) {
	m_weather -= 1;
	if (m_weather == 0) {
		if (m_pWeatherPrevBtn)
			m_pWeatherPrevBtn->Enable(false);
	}

	if (m_pWeatherNextBtn)
		m_pWeatherNextBtn->Enable(true);

	if (m_pWeatherEdit) {
		switch (m_weather) {
		case 0:
			m_pWeatherEdit->SetLine(1, "\xb8\xbc\xc0\xbd", 0, false, 0);
			break;
		case 3:
			m_pWeatherEdit->SetLine(1, "\xb4\xab", 0, false, 0);
			break;
		case 2:
			m_pWeatherEdit->SetLine(1, "\xba\xf1", 0, false, 0);
			break;
		}
	}
}

// trainingoptiondlg.cpp:234
void FrTrainingOptionDlg::OnWeatherNextBtnInit(intptr_t param) {
	m_pWeatherNextBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pWeatherNextBtn) {
		m_pWeatherNextBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_weather == 2)
				m_pWeatherNextBtn->Enable(false);
		} else {
			m_pWeatherNextBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:253
void FrTrainingOptionDlg::OnWeatherNextBtnUp(void) {
	m_weather += 1;
	if (m_weather == 2) {
		if (m_pWeatherNextBtn)
			m_pWeatherNextBtn->Enable(false);
	}

	if (m_pWeatherPrevBtn)
		m_pWeatherPrevBtn->Enable(true);

	if (m_pWeatherEdit) {
		switch (m_weather) {
		case 0:
			m_pWeatherEdit->SetLine(1, "\xb8\xbc\xc0\xbd", 0, false, 0);
			break;
		case 3:
			m_pWeatherEdit->SetLine(1, "\xb4\xab", 0, false, 0);
			break;
		case 2:
			m_pWeatherEdit->SetLine(1, "\xba\xf1", 0, false, 0);
			break;
		}
	}
}

// trainingoptiondlg.cpp:283
void FrTrainingOptionDlg::OnDirectionInit(intptr_t param) {
	m_pDirectionEdit = DYNAMIC_CAST(FrEdit, param);
	m_direction = (int)(floor(Wind().GetGlobalDirection() * kRadToDeg * 0.1f) * 10.0f);

	if (m_pDirectionEdit)
		m_pDirectionEdit->AddLine(MakeStr("%d\xb5\xb5", m_direction), 0, false);
}

// trainingoptiondlg.cpp:292
void FrTrainingOptionDlg::OnDirectionPrevBtnInit(intptr_t param) {
	m_pDirectionPrevBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pDirectionPrevBtn) {
		m_pDirectionPrevBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_direction == 0)
				m_pDirectionPrevBtn->Enable(false);
		} else {
			m_pDirectionPrevBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:311
void FrTrainingOptionDlg::OnDirectionPrevBtnUp(void) {
	m_direction -= 10;

	if (m_direction == 0) {
		if (m_pDirectionPrevBtn)
			m_pDirectionPrevBtn->Enable(false);
	}

	if (m_pDirectionNextBtn)
		m_pDirectionNextBtn->Enable(true);

	if (m_pDirectionEdit)
		m_pDirectionEdit->SetLine(1, MakeStr("%d\xb5\xb5", m_direction), 0, false, 0);
}

// trainingoptiondlg.cpp:328
void FrTrainingOptionDlg::OnDirectionNextBtnInit(intptr_t param) {
	m_pDirectionNextBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pDirectionNextBtn) {
		m_pDirectionNextBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_direction == 360)
				m_pDirectionNextBtn->Enable(false);
		} else {
			m_pDirectionNextBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:347
void FrTrainingOptionDlg::OnDirectionNextBtnUp(void) {
	m_direction += 10;

	if (m_direction == 360) {
		if (m_pDirectionNextBtn)
			m_pDirectionNextBtn->Enable(false);
	}

	if (m_pDirectionPrevBtn)
		m_pDirectionPrevBtn->Enable(true);

	if (m_pDirectionEdit)
		m_pDirectionEdit->SetLine(1, MakeStr("%d\xb5\xb5", m_direction), 0, false, 0);
}

// trainingoptiondlg.cpp:364
void FrTrainingOptionDlg::OnIntensityInit(intptr_t param) {
	m_pIntensityEdit = DYNAMIC_CAST(FrEdit, param);

	m_intensity = (int)Wind().GetGlobalIntensity();

	if (m_pIntensityEdit)
		m_pIntensityEdit->AddLine(MakeStr("%dm", m_intensity), 0, false);
}

// trainingoptiondlg.cpp:382
void FrTrainingOptionDlg::OnIntensityPrevBtnInit(intptr_t param) {
	m_pIntensityPrevBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pIntensityPrevBtn) {
		m_pIntensityPrevBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_intensity == 0)
				m_pIntensityPrevBtn->Enable(false);
		} else {
			m_pIntensityPrevBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:401
void FrTrainingOptionDlg::OnIntensityPrevBtnUp(void) {
	m_intensity -= 1;

	if (m_intensity == 0) {
		if (m_pIntensityPrevBtn)
			m_pIntensityPrevBtn->Enable(false);
	}

	if (m_pIntensityNextBtn)
		m_pIntensityNextBtn->Enable(true);

	if (m_pIntensityEdit)
		m_pIntensityEdit->SetLine(1, MakeStr("%dm", m_intensity), 0, false, 0);
}

// trainingoptiondlg.cpp:418
void FrTrainingOptionDlg::OnIntensityNextBtnInit(intptr_t param) {
	m_pIntensityNextBtn = DYNAMIC_CAST(FrButton, param);

	if (m_pIntensityNextBtn) {
		m_pIntensityNextBtn->SetPushDelay(0.0f);

		if (m_bFlag14c) {
			if (m_intensity == 9)
				m_pIntensityNextBtn->Enable(false);
		} else {
			m_pIntensityNextBtn->Enable(false);
		}
	}
}

// trainingoptiondlg.cpp:438
void FrTrainingOptionDlg::OnIntensityNextBtnUp(void) {
	m_intensity += 1;

	if (m_intensity == 9) {
		if (m_pIntensityNextBtn)
			m_pIntensityNextBtn->Enable(false);
	}

	if (m_pIntensityPrevBtn)
		m_pIntensityPrevBtn->Enable(true);

	if (m_pIntensityEdit)
		m_pIntensityEdit->SetLine(1, MakeStr("%dm", m_intensity), 0, false, 0);
}

// trainingoptiondlg.cpp:460 (run-time type and factory registration of
// FrNewTrainingOptionDlg)
const WRTTI FrNewTrainingOptionDlg::m_RTTI("FrNewTrainingOptionDlg", &FrForm::m_RTTI);

IObject* FrNewTrainingOptionDlgMakeInstance(void) {
	return new FrNewTrainingOptionDlg;
}

struct __sFrNewTrainingOptionDlg {
	__sFrNewTrainingOptionDlg(void) {
		ObjectFactory().AddObjectFunctor(FrNewTrainingOptionDlgMakeInstance, "FrNewTrainingOptionDlg");
	}
};
static __sFrNewTrainingOptionDlg s_registerFrNewTrainingOptionDlg;

// trainingoptiondlg.cpp:462
const sFRESH_MSGMAP* FrNewTrainingOptionDlg::GetMessageMap(void) const {
	return &FrNewTrainingOptionDlg::_MsgMap;
}

sFRESH_MSGMAP FrNewTrainingOptionDlg::_MsgMap = { &FrForm::_MsgMap, &FrNewTrainingOptionDlg::_MsgEntries[0] };

// trainingoptiondlg.cpp:463
sFRESH_ENTRY FrNewTrainingOptionDlg::_MsgEntries[] = {
	// trainingoptiondlg.cpp:464
	{ "training_map", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitMapButton },
	// trainingoptiondlg.cpp:465
	{ "training_map", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownMapButton },

	// trainingoptiondlg.cpp:467
	{ "training_mapprev", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitMapPrevButton },
	// trainingoptiondlg.cpp:468
	{ "training_mapprev", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownMapPrevButton },

	// trainingoptiondlg.cpp:470
	{ "training_mapnext", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitMapNextButton },
	// trainingoptiondlg.cpp:471
	{ "training_mapnext", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownMapNextButton },

	// trainingoptiondlg.cpp:473
	{ "training_fronthole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitFrontHoleButton },
	// trainingoptiondlg.cpp:474
	{ "training_fronthole", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownFrontHoleButton },

	// trainingoptiondlg.cpp:476
	{ "training_backhole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitBackHoleButton },
	// trainingoptiondlg.cpp:477
	{ "training_backhole", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownBackHoleButton },

	// trainingoptiondlg.cpp:479
	{ "training_randomhole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitRandomHoleButton },
	// trainingoptiondlg.cpp:480
	{ "training_randomhole", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownRandomHoleButton },

	// trainingoptiondlg.cpp:482
	{ "training_shufflehole", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitShuffleHoleButton },
	// trainingoptiondlg.cpp:483
	{ "training_shufflehole", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownShuffleHoleButton },

	// trainingoptiondlg.cpp:485
	{ "training_hare", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitHareButton },
	// trainingoptiondlg.cpp:486
	{ "training_hare", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownHareButton },

	// trainingoptiondlg.cpp:488
	{ "training_kumori", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitKumoriButton },
	// trainingoptiondlg.cpp:489
	{ "training_kumori", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownKumoriButton },

	// trainingoptiondlg.cpp:491
	{ "training_ame", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitAmeButton },
	// trainingoptiondlg.cpp:492
	{ "training_ame", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownAmeButton },

	// trainingoptiondlg.cpp:494
	{ "training_windedit", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitWindDirectionEdit },

	// trainingoptiondlg.cpp:496
	{ "training_play", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitPlayButton },
	// trainingoptiondlg.cpp:497
	{ "training_play", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownPlayButton },

	// trainingoptiondlg.cpp:499
	{ "training_cancel", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrNewTrainingOptionDlg::OnInitCancelButton },
	// trainingoptiondlg.cpp:500
	{ "training_cancel", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrNewTrainingOptionDlg::OnLBDownCancelButton },

	// trainingoptiondlg.cpp:502
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// trainingoptiondlg.cpp:508
FrNewTrainingOptionDlg::FrNewTrainingOptionDlg(void) {
	ClearVariables();
}

// trainingoptiondlg.cpp:516
FrNewTrainingOptionDlg::~FrNewTrainingOptionDlg(void) {
}

// trainingoptiondlg.cpp:523
void FrNewTrainingOptionDlg::ClearVariables(void) {
	m_pMapButton = 0;
	m_pMapPrevButton = 0;
	m_pMapNextButton = 0;

	m_pFrontHoleButton = 0;
	m_pBackHoleButton = 0;
	m_pRandomHoleButton = 0;
	m_pShuffleHoleButton = 0;

	m_unknown11c = 0;

	m_pHareButton = 0;
	m_pKumoriButton = 0;
	m_pAmeButton = 0;

	m_pWindDirectionEdit = 0;

	m_unknown140 = 0;

	m_pPlayButton = 0;
	m_pCancelButton = 0;

	m_bExpansionMenu = false;

	m_environment[0] = 0xffff;
	m_environment[1] = 0xffff;
	m_environment[2] = 0xffff;
	m_environment[3] = 0xffff;
	m_environment[4] = 0xffff;
}

// trainingoptiondlg.cpp:559
void FrNewTrainingOptionDlg::OnInitMapButton(intptr_t param) {
	m_pMapButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:565
void FrNewTrainingOptionDlg::OnLBDownMapButton(void) {
}

// trainingoptiondlg.cpp:573
void FrNewTrainingOptionDlg::OnInitMapPrevButton(intptr_t param) {
	m_pMapPrevButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:579
void FrNewTrainingOptionDlg::OnLBDownMapPrevButton(void) {
}

// trainingoptiondlg.cpp:587
void FrNewTrainingOptionDlg::OnInitMapNextButton(intptr_t param) {
	m_pMapNextButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:593
void FrNewTrainingOptionDlg::OnLBDownMapNextButton(void) {
}

// trainingoptiondlg.cpp:601
void FrNewTrainingOptionDlg::OnInitFrontHoleButton(intptr_t param) {
	m_pFrontHoleButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:607
void FrNewTrainingOptionDlg::OnLBDownFrontHoleButton(void) {
}

// trainingoptiondlg.cpp:615
void FrNewTrainingOptionDlg::OnInitBackHoleButton(intptr_t param) {
	m_pBackHoleButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:621
void FrNewTrainingOptionDlg::OnLBDownBackHoleButton(void) {
}

// trainingoptiondlg.cpp:629
void FrNewTrainingOptionDlg::OnInitRandomHoleButton(intptr_t param) {
	m_pRandomHoleButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:635
void FrNewTrainingOptionDlg::OnLBDownRandomHoleButton(void) {
}

// trainingoptiondlg.cpp:643
void FrNewTrainingOptionDlg::OnInitShuffleHoleButton(intptr_t param) {
	m_pShuffleHoleButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:649
void FrNewTrainingOptionDlg::OnLBDownShuffleHoleButton(void) {
}

// trainingoptiondlg.cpp:656
void FrNewTrainingOptionDlg::OnInitHareButton(intptr_t param) {
	m_pHareButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:662
void FrNewTrainingOptionDlg::OnLBDownHareButton(void) {
	m_environment[2] = 0;
}

// trainingoptiondlg.cpp:671
void FrNewTrainingOptionDlg::OnInitKumoriButton(intptr_t param) {
	m_pKumoriButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:677
void FrNewTrainingOptionDlg::OnLBDownKumoriButton(void) {
	m_environment[2] = 1;
}

// trainingoptiondlg.cpp:686
void FrNewTrainingOptionDlg::OnInitAmeButton(intptr_t param) {
	m_pAmeButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:692
void FrNewTrainingOptionDlg::OnLBDownAmeButton(void) {
	m_environment[2] = 2;
}

// trainingoptiondlg.cpp:701
void FrNewTrainingOptionDlg::OnInitWindDirectionEdit(intptr_t param) {
	m_pWindDirectionEdit = DYNAMIC_CAST(FrEdit, param);
}

// trainingoptiondlg.cpp:709
void FrNewTrainingOptionDlg::OnInitPlayButton(intptr_t param) {
	m_pPlayButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:715
void FrNewTrainingOptionDlg::OnLBDownPlayButton(void) {
	Close(FrOK, true);
}

// trainingoptiondlg.cpp:724
void FrNewTrainingOptionDlg::OnInitCancelButton(intptr_t param) {
	m_pCancelButton = DYNAMIC_CAST(FrButton, param);
}

// trainingoptiondlg.cpp:730
void FrNewTrainingOptionDlg::OnLBDownCancelButton(void) {
	Close(FrCANCEL, true);
}

// trainingoptiondlg.cpp:739
void FrNewTrainingOptionDlg::ShowExpansionMenu(bool show) {
}

// trainingoptiondlg.cpp:746
void FrNewTrainingOptionDlg::SetEnvironmentToGame(void) {
}

// trainingoptiondlg.cpp:753
void FrNewTrainingOptionDlg::GetEnvironmentFromGame(void) {
}
