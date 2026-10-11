// Original: d:\build\custom\temp\code\client\projectg\bubble.cpp
#include "bubble.h"

#include <new>

#include "ObjectFactory.h"
#include "talkbox.h"
#include <string>
#include "wview.h"
#include "SharedDoc.h"
#include "golfdoc.h"
#include "golfball.h"
#include "CapturedBg.h"
#include "ChatMsg.h"
#include "ItemManager.h"
#include "classdefine.h"

extern WMatrix g_camera;

// talkbox.h:15
bool CTalkBox::IsActive(void) {
	return m_remainTime > 0.0f ? true : false;
}

// talkbox.h:19
void CTalkBox::SetTransparency(float alpha) {
	m_transparency = alpha;
}

// bubble.cpp:3
CBubble::sBubble::sBubble(void) {
}

// bubble.cpp:20
IObject* CBubbleMakeInstance(void) {
	return new CBubble;
}

// bubble.cpp:20 (run-time type and factory registration of CBubble)
const WRTTI CBubble::m_RTTI("CBubble", &IActor::m_RTTI);

struct __impCBubble {
	__impCBubble(void) {
		ObjectFactory().AddObjectFunctor(CBubbleMakeInstance, "CBubble");
		ActorFactory().AddActorClass("CBubble");
	}
};
static __impCBubble s_impCBubble;

// bubble.cpp:26
CBubble::CBubble(void) {
	m_priority = 4;
	m_pBubbles = NULL;
	m_bEnable = true;
	AddSkipList(0x854C2);
}

// bubble.cpp:36
void CBubble::OnDestroy(void) {
	if (m_pBubbles) {
		for (int i = 0; i < 2; i++) {
			if (m_pBubbles[i].m_pTalkBox) { delete m_pBubbles[i].m_pTalkBox; m_pBubbles[i].m_pTalkBox = NULL; }
		}
		if (m_pBubbles) { delete[] m_pBubbles; m_pBubbles = NULL; }
	}
}

// bubble.cpp:47
void CBubble::OnLoad(void) {
	m_pBubbles = new sBubble[2];

	for (int i = 0; i < 2; i++) {
		m_pBubbles[i].m_pTalkBox = new CTalkBox;
		m_pBubbles[i].m_bShow = false;
	}
}

// bubble.cpp:58
void CBubble::OnProcess(float elapsed) {
	if (!m_bEnable) {
		return;
	}

	if (WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		return;
	}

	for (int i = 0; i < 2; i++) {
		if (m_pBubbles[i].m_pTalkBox->IsActive()) {
			float fade = ((m_pBubbles[i].m_pos - g_camera.pivot).Magnitude() - 64.0f) * (1.0f / 256.0f);
			if (fade < 0.0f) {
				fade = 0.0f;
			} else if (fade > 1.0f) {
				fade = 1.0f;
			}
			m_pBubbles[i].m_pTalkBox->SetTransparency(1.0f - fade);
			m_pBubbles[i].m_pTalkBox->Process(elapsed);
		}
	}

	if (!m_pBubbles[0].m_pTalkBox->IsActive() && m_pBubbles[0].m_bShow) {
		m_pBubbles[0].m_bShow = false;
		SendMsg(this, "Caddie", 0x1DD, 0, 0, 0, 0);
	}
}

// bubble.cpp:82
void CBubble::OnDisplay(void) {
	if (!m_bEnable) {
		return;
	}

	if ((GOLFDOC()->GameMode() & 0x200) || WSingleton<CCapturedBg>::Instance()->IsCaptured()) {
		return;
	}

	if (m_pBubbles[1].m_pTalkBox->IsActive()) {
		WVector pos = g_view->Projection(m_pBubbles[1].m_pos);
		if (0.0f < pos.z && pos.z < 1.0f) {
			m_pBubbles[1].m_pTalkBox->Render(g_view, pos.x, pos.y, 1);
		}
	}

	if (m_pBubbles[0].m_pTalkBox->IsActive()) {
		WVector pos = g_view->Projection(m_pBubbles[0].m_pos);
		if (0.0f < pos.z && pos.z < 1.0f) {
			float offsetX;
			float offsetY;
			int align;
			if (GolfBall().m_groundType == 2) {
				offsetX = -20.0f;
				offsetY = -10.0f;
				align = 2;
			} else {
				offsetX = 0.0f;
				offsetY = 0.0f;
				align = 1;
			}

			m_pBubbles[0].m_pTalkBox->Render(g_view, pos.x + offsetX, pos.y + offsetY, align);
		}
	}
}

// bubble.cpp:140
void CBubble::HandleMsg(const MsgObject& msg) {
	switch (msg.m_msg) {
	case 0x13: {
		std::string text;
		IFF_STRUCT::sCaddie* pCaddie = ItemManager()->FindCaddie(WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_caddieTypeId);
		if (pCaddie) {
			text = pCaddie->m_name;
			text += " : ";
			text += (const char*)msg.m_param1;
			WSingleton<CChatMsg>::Instance()->AddChatMsg(text, 0xFF00FF00, true, false);
		} else {
			text = (const char*)msg.m_param1;
		}

		m_pBubbles[0].m_pTalkBox->SetText(text.c_str(), 3.0f);
		m_pBubbles[0].m_pos = *(const WVector*)msg.m_param2;
		m_pBubbles[0].m_bShow = true;
		break;
	}
	case 0x1F5:
		m_pBubbles[1].m_pTalkBox->SetText((const char*)msg.m_param1, 3.0f);
		m_pBubbles[1].m_pos = *(const WVector*)msg.m_param2;
		m_pBubbles[1].m_bShow = true;
		break;
	case 0x163:
		m_bEnable = true;
		break;
	case 0x164:
		m_bEnable = false;
		break;
	}
}

// actor.h:120
void IActor::OnInit(void) {
}
