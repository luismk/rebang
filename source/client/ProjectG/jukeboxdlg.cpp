// Original: d:\build\custom\temp\code\client\projectg\jukeboxdlg.cpp
#include <stdlib.h>
#include <vector>
#include "JukeBoxDlg.h"
#include "ObjectFactory.h"
#include "fredit.h"
#include "frbutton.h"
#include "SoundManager.h"
#include "commonutil.h"
#include "TaskManager.h"

// jukeboxdlg.cpp:20 (run-time type and factory registration of FrJukeBoxDlg)
const WRTTI FrJukeBoxDlg::m_RTTI("FrJukeBoxDlg", &FrForm::m_RTTI);

IObject* FrJukeBoxDlgMakeInstance(void) {
	return new FrJukeBoxDlg;
}

struct __sFrJukeBoxDlg {
	__sFrJukeBoxDlg(void) {
		ObjectFactory().AddObjectFunctor(FrJukeBoxDlgMakeInstance, "FrJukeBoxDlg");
	}
};
static __sFrJukeBoxDlg s_registerFrJukeBoxDlg;

// jukeboxdlg.cpp:22
const sFRESH_MSGMAP* FrJukeBoxDlg::GetMessageMap(void) const {
	return &FrJukeBoxDlg::_MsgMap;
}

sFRESH_MSGMAP FrJukeBoxDlg::_MsgMap = { &FrForm::_MsgMap, &FrJukeBoxDlg::_MsgEntries[0] };

bool FrJukeBoxDlg::m_preserve_isPlaying = false;

sFRESH_ENTRY FrJukeBoxDlg::_MsgEntries[] = {
	// jukeboxdlg.cpp:24
	{ "jukeboxtitle", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrJukeBoxDlg::OnTitleInit },
	// jukeboxdlg.cpp:25
	{ "jukeboxplay", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrJukeBoxDlg::OnPlayInit },
	// jukeboxdlg.cpp:26
	{ "jukeboxclose", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrJukeBoxDlg::OnCloseBtnUp },
	// jukeboxdlg.cpp:27
	{ "jukeboxplay", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrJukeBoxDlg::OnPlayBtnUp },
	// jukeboxdlg.cpp:28
	{ "jukeboxprev", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrJukeBoxDlg::OnPrevBtnUp },
	// jukeboxdlg.cpp:29
	{ "jukeboxnext", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrJukeBoxDlg::OnNextBtnUp },
	// jukeboxdlg.cpp:30
	{ "jukeboxstop", FRCMD_LBUTTONUP, FrFuncVV, (FRESH_PMSG)&FrJukeBoxDlg::OnStopBtnUp },

	// jukeboxdlg.cpp:32
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// One jukebox track: the BGM file and the title the dialog shows.
struct sJukeBoxBGM {
	const char* file;    // 0x0
	const char* title;   // 0x4
};

// The jukebox's track list (0x9f5cc0).
static const sJukeBoxBGM s_jukeBoxBGM[] = {
	{ "shiny.mp3", "shiny day" },
	{ "bunny.mp3", "bunny picnic" },
	{ "navy_blue.mp3", "navy blue memory" },
	{ "rising_sun.mp3", "rising sun" },
	{ "happy flight.mp3", "happy flight" },
	{ "crystal waver.mp3", "crystal waver" },
	{ "snowscape.mp3", "snowscape" },
	{ "winter_ride.mp3", "winter ride" },
	{ "somewhere.mp3", "somewhere" },
	{ "nowhere.mp3", "nowhere" },
	{ "volcano.mp3", "dive into volcano" },
	{ "vermilion.mp3", "vermilion sunset" },
	{ "samba3.mp3", "breeze" },
	{ "daydream.mp3", "daydream" },
	{ "frog.mp3", "frog" },
	{ "spring.mp3", "spring" },
	{ "data/Sound/season2/tea_time_piano.mp3", "tea time" },
	{ "crystal lake.mp3", "crystal lake" },
	{ "fade into white.mp3", "fade into white" },
	{ "the mystery of the lost seaway.mp3", "the mystery of the lost seaway" },
	{ "voyage the sky.mp3", "voyage the sky" },
	{ "zero fill love.mp3", "zero fill love" },
	{ "eastern_valley.mp3", "eastern valley" },
	{ "river.mp3", "river" }
};

enum {
	JUKEBOX_BGM_COUNT = 24
};

int FrJukeBoxDlg::m_curBGM;

// jukeboxdlg.cpp:70
void FrJukeBoxDlg::Init(void) {
	m_curBGM = rand() % JUKEBOX_BGM_COUNT;
}

// jukeboxdlg.cpp:76
void FrJukeBoxDlg::Preserve(void) {
	m_preserve_isPlaying = g_audio->IsPlaying(s_jukeBoxBGM[m_curBGM].file);
	g_audio->StopBGM(true);
}

// jukeboxdlg.cpp:82
void FrJukeBoxDlg::Restore(void) {
	g_audio->StopBGM(true);
	if (m_preserve_isPlaying) {
		g_audio->PlayBGM(s_jukeBoxBGM[m_curBGM].file, false, true);
	}
}

// jukeboxdlg.cpp:89
void FrJukeBoxDlg::PlayWithMap(unsigned char map) {
	std::vector<int> bgms;

	switch (map) {
	case 4:
	case 5:
		bgms.push_back(0);
		bgms.push_back(1);
		break;
	case 7:
		bgms.push_back(2);
		bgms.push_back(3);
		break;
	case 8:
		bgms.push_back(4);
		bgms.push_back(5);
		break;
	case 9:
		bgms.push_back(6);
		bgms.push_back(7);
		break;
	case 10:
		bgms.push_back(8);
		bgms.push_back(9);
		break;
	case 13:
		bgms.push_back(10);
		bgms.push_back(11);
		break;
	case 14:
		bgms.push_back(17);
		bgms.push_back(18);
		break;
	case 15:
		bgms.push_back(19);
		bgms.push_back(20);
		break;
	case 16:
		bgms.push_back(22);
		bgms.push_back(23);
		break;
	default:
		bgms.push_back(12);
		bgms.push_back(13);
		bgms.push_back(14);
		bgms.push_back(15);
		break;
	}

	if (bgms.size() != 0) {
		g_audio->StopBGM(true);
		m_curBGM = bgms[rand() % bgms.size()];
		g_audio->PlayBGM(s_jukeBoxBGM[m_curBGM].file, false, true);
	}
}

// jukeboxdlg.cpp:151
FrJukeBoxDlg::FrJukeBoxDlg(void)
	: m_pTitle(NULL)
	, m_pPlay(NULL) {
}

// jukeboxdlg.cpp:157
FrJukeBoxDlg::~FrJukeBoxDlg(void) {
}

// jukeboxdlg.cpp:162
bool FrJukeBoxDlg::OnInit(void) {
	FrForm::OnInit();

	EnableDrag(false);
	return true;
}

// jukeboxdlg.cpp:170
void FrJukeBoxDlg::OnCloseBtnUp(void) {
	Close(true);
}

// jukeboxdlg.cpp:175
void FrJukeBoxDlg::OnTitleInit(intptr_t param) {
	m_pTitle = DYNAMIC_CAST(FrEdit, param);
	if (m_pTitle) m_pTitle->SetLine(1, s_jukeBoxBGM[m_curBGM].title, 0xFFFFFFFF, false, 0);
}

// jukeboxdlg.cpp:181
void FrJukeBoxDlg::OnPlayInit(intptr_t param) {
	m_pPlay = DYNAMIC_CAST(FrButton, param);
	OnChangePlayPauseButton();
}

// jukeboxdlg.cpp:187
void FrJukeBoxDlg::OnPlayBtnUp(void) {
	bool isPlaying = g_audio->IsPlaying(s_jukeBoxBGM[m_curBGM].file);

	if (isPlaying) {
		g_audio->PauseBGM(false);
	} else {
		g_audio->PlayBGM(s_jukeBoxBGM[m_curBGM].file, false, true);
	}

	OnChangePlayPauseButton();

	AfxGetTask()->PassMsgToActor("AvatarChat", MsgObject(0, 0x69, !isPlaying, 0, 0, 0, 0));
}

// jukeboxdlg.cpp:202
void FrJukeBoxDlg::OnStopBtnUp(void) {
	g_audio->StopBGM(true);

	OnChangePlayPauseButton();

	AfxGetTask()->PassMsgToActor("AvatarChat", MsgObject(0, 0x69, 0, 0, 0, 0, 0));
}

// jukeboxdlg.cpp:211
void FrJukeBoxDlg::OnPrevBtnUp(void) {
	bool isPlaying = g_audio->IsPlaying(s_jukeBoxBGM[m_curBGM].file);

	m_curBGM--;
	if (m_curBGM < 0) {
		m_curBGM = JUKEBOX_BGM_COUNT - 1;
	}
	if (m_pTitle) {
		m_pTitle->SetLine(1, s_jukeBoxBGM[m_curBGM].title, 0xFFFFFFFF, false, 0);
	}
	if (isPlaying) {
		g_audio->StopBGM(true);
		g_audio->PlayBGM(s_jukeBoxBGM[m_curBGM].file, false, true);
	}
}

// jukeboxdlg.cpp:230
void FrJukeBoxDlg::OnNextBtnUp(void) {
	bool isPlaying = g_audio->IsPlaying(s_jukeBoxBGM[m_curBGM].file);

	m_curBGM++;
	if (m_curBGM > sizeof(s_jukeBoxBGM) / sizeof(s_jukeBoxBGM[0]) - 1) {
		m_curBGM = 0;
	}
	if (m_pTitle) {
		m_pTitle->SetLine(1, s_jukeBoxBGM[m_curBGM].title, 0xFFFFFFFF, false, 0);
	}
	if (isPlaying) {
		g_audio->StopBGM(true);
		g_audio->PlayBGM(s_jukeBoxBGM[m_curBGM].file, false, true);
	}
}

// jukeboxdlg.cpp:249
void FrJukeBoxDlg::OnChangeTitle(void) {
	if (m_pTitle) {
		m_pTitle->SetLine(1, s_jukeBoxBGM[m_curBGM].title, 0xFFFFFFFF, false, 0);
	}
}

// jukeboxdlg.cpp:257
void FrJukeBoxDlg::OnChangePlayPauseButton(void) {
	bool isPlaying = g_audio->IsPlaying(s_jukeBoxBGM[m_curBGM].file);

	if (m_pPlay) {
		const char* name;

		if (isPlaying) {
			name = "btn_pause";
		} else {
			name = "btn_play";
		}
		m_pPlay->SetButtonImg(MakeStr("%s_n", name), FrButton::NORMAL);
		m_pPlay->SetButtonImg(MakeStr("%s_o", name), FrButton::OVER);
		m_pPlay->SetButtonImg(MakeStr("%s_o", name), FrButton::PRESSED);
	}
}
