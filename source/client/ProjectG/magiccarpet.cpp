// Original: d:\build\custom\temp\code\client\projectg\magiccarpet.cpp
// actor.h names IActor in MsgObject before declaring it.
class IActor;

#include "magiccarpet.h"

#include <new>
#include <stdlib.h>

#include "ObjectFactory.h"
#include "ClientSetting.h"
#include "SoundManager.h"
#include "commonutil.h"

// clientsetting.h:381
float COption::aGetSfxVolume(void) const {
	return m_option.m_sfxVolume;
}

// clientsetting.h:382
float COption::aGetBgmVolume(void) const {
	return m_option.m_bgmVolume;
}

// magiccarpet.cpp:9 (run-time type and factory registration of CMagicCarpetHelper)
const WRTTI CMagicCarpetHelper::m_RTTI("CMagicCarpetHelper", &IActor::m_RTTI);

IObject* CMagicCarpetHelperMakeInstance(void) {
	return new CMagicCarpetHelper;
}

struct __impCMagicCarpetHelper {
	__impCMagicCarpetHelper(void) {
		ObjectFactory().AddObjectFunctor(CMagicCarpetHelperMakeInstance, "CMagicCarpetHelper");
		ActorFactory().AddActorClass("CMagicCarpetHelper");
	}
};
static __impCMagicCarpetHelper s_impCMagicCarpetHelper;

// magiccarpet.cpp:12
CMagicCarpetHelper::CMagicCarpetHelper(void) : m_bVolumeDown(false), m_bAutoVolume(false) {
}

// magiccarpet.cpp:17
CMagicCarpetHelper::~CMagicCarpetHelper(void) {
	g_audio->SetBGMVolume(COption::Instance()->aGetBgmVolume());
}

// magiccarpet.cpp:24
void CMagicCarpetHelper::OnInit(void) {
}

// magiccarpet.cpp:29
void CMagicCarpetHelper::OnLoad(void) {
}

// magiccarpet.cpp:34
void CMagicCarpetHelper::OnProcess(float elapsed) {
	if (m_bAutoVolume) {
		AutoVolumeAdjustProcess(elapsed);
	}
}

// magiccarpet.cpp:43
void CMagicCarpetHelper::AutoVolumeAdjustProcess(const float& elapsed) {
	if (m_bVolumeDown) {
		if (g_audio->GetBGMVolume() > COption::Instance()->aGetSfxVolume() * 0.5f) {
			float volume = g_audio->GetBGMVolume() - elapsed * 0.25f;
			if (!(volume > 0.0f)) {
				volume = 0.0f;
			}
			if (volume > COption::Instance()->aGetBgmVolume() * 0.5f) {
				g_audio->SetBGMVolume(volume);
			}
		} else {
			g_audio->SetBGMVolume(COption::Instance()->aGetSfxVolume() * 0.5f);
		}
	} else {
		if (g_audio->GetBGMVolume() < COption::Instance()->aGetBgmVolume()) {
			float volume = g_audio->GetBGMVolume() + elapsed * 0.25f;
			if (volume > 1.0f) {
				volume = 1.0f;
			}
			g_audio->SetBGMVolume(volume);
		} else {
			m_bAutoVolume = false;
			g_audio->SetBGMVolume(COption::Instance()->aGetBgmVolume());
		}
	}
}

// magiccarpet.cpp:83
void CMagicCarpetHelper::OnDisplay(void) {
}

// magiccarpet.cpp:88
void CMagicCarpetHelper::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x280:
		PlaySfxMagicCarpetBoundSound();
		break;
	case 0x27e:
		m_bAutoVolume = true;
		m_bVolumeDown = true;
		break;
	case 0x27f:
		m_bVolumeDown = false;
		break;
	}
}

// magiccarpet.cpp:116
void CMagicCarpetHelper::PlaySfxMagicCarpetBoundSound(void) {
	int first = rand() % 5 + 1;
	g_audio->PlaySfx(MakeStr("magic_melody%d", first), NULL, 0, NULL, NULL, 0.5f, 200.0f);

	int second = rand() % 5 + 1;
	if (second != first) {
		g_audio->PlaySfx(MakeStr("magic_melody%d", second), NULL, 0, NULL, NULL, 0.5f, 200.0f);

		int third = rand() % 5 + 1;
		if (third != first && third != second) {
			g_audio->PlaySfx(MakeStr("magic_melody%d", third), NULL, 0, NULL, NULL, 0.5f, 200.0f);
		}
	} else {
		g_audio->PlaySfx("magic_bounce", NULL, 0, NULL, NULL, 0.5f, 200.0f);
	}

	bool playingLong1 = g_audio->IsPlaying("magicharp_goal_long1");
	bool playingShort1 = g_audio->IsPlaying("magicharp_goal_short1");
	bool playingShort2 = g_audio->IsPlaying("magicharp_goal_short2");

	if (!playingLong1 && !playingShort1 && !playingShort2) {
		int chance = rand() % 50;
		if (chance == 1) {
			g_audio->PlaySfx("magicharp_goal_long1", NULL, 0, NULL, NULL, 0.5f, 200.0f);
		} else if (chance == 2) {
			g_audio->PlaySfx("magicharp_goal_short1", NULL, 0, NULL, NULL, 0.5f, 200.0f);
		} else if (chance == 3) {
			g_audio->PlaySfx("magicharp_goal_short2", NULL, 0, NULL, NULL, 0.5f, 200.0f);
		}
	}
}
