// Original: d:\build\custom\temp\code\client\projectg\galleryreaction.cpp
#include <stdlib.h>
#include <string.h>
#include "galleryreaction.h"
#include "ObjectFactory.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "SoundManager.h"
#include "commonutil.h"

// galleryreaction.cpp:15 (run-time type and factory registration of CGallery)
const WRTTI CGallery::m_RTTI("CGallery", &IActor::m_RTTI);

IObject *CGalleryMakeInstance(void) {
	return new CGallery;
}

struct __impCGallery {
	__impCGallery(void) {
		ObjectFactory().AddObjectFunctor(CGalleryMakeInstance, "CGallery");
		ActorFactory().AddActorClass("CGallery");
	}
};
static __impCGallery s_impCGallery;

// galleryreaction.cpp:18
CGallery::CGallery(void) {
	m_soundDelay = 0;
	m_soundName[0] = 0;
}

// galleryreaction.cpp:24
void CGallery::OnProcess(float dt) {
	if (m_soundName[0] != '\0') {
		m_soundDelay -= dt;
	}
}

// galleryreaction.cpp:32
void CGallery::SetSound(const char *name, float delay) {
	int gameState = GOLFDOC()->m_gameState;
	if (gameState >= 15 || gameState == 8) {
		if (strcmp(m_soundName, name) == 0) {
			if (m_soundDelay > 0.0f) {
				return;
			}
			m_soundDelay = delay;
		}
		else {
			if (m_soundName[0] != '\0') {
				g_audio->StopSfx(m_soundName);
			}
			m_soundDelay = delay;
			strcpy(m_soundName, name);
		}
		g_audio->PlaySfx(m_soundName, 0, 0, m_soundName, 0, 0.5f, 200.0f);
	}
}

// galleryreaction.cpp:58
void CGallery::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0xFF:
	case 0x100:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xb9\xda\xbc\xf6", 0.0f); // gallery_applause
		break;

	case 0x102:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 3.0f); // gallery_ah
		break;

	case 0x105:
		if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId == 0x14000004) {
			SetSound(MakeStr("t_Ex6-%d", rand() % 3 + 1), 0.0f);
		}
		else {
			SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		}
		break;

	case 0x106:
		if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId == 0x14000004) {
			SetSound(MakeStr("t_Ex4-%d", rand() % 3 + 1), 0.0f);
		}
		else {
			SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		}
		break;

	case 0x109:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbd\xc7\xb8\xc1", 0.0f); // gallery_disappointment
		break;

	case 0x101:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;
	case 0x103:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;
	case 0x104:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;
	case 0x107:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;
	case 0x108:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;
	case 0x10A:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbe\xc6", 0.0f); // gallery_ah
		break;

	case 0x10D:
		if (WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId == 0x14000004) {
			SetSound(MakeStr("t_Ex3-%d", rand() % 4 + 1), 0.0f);
			break;
		}
	case 0x10B:
		SetSound("\xb0\xb6\xb7\xaf\xb8\xae_\xbf\xc0", 0.0f); // gallery_oh
		break;

	default:
		break;
	}
}
