// Original: d:\build\custom\temp\code\client\projectg\headicon.cpp
#include "headicon.h"
#include "wresrcmng.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ProjectG.h"
#include "Packet.h"
#include "commonutil.h"
#include "../shared/ItemManager.h"
#include "../shared/classdefine.h"

// shareddoc.inl:503; not declared by SharedDoc.h yet (GolfRule emits the
// copy the original calls).
unsigned char MyIndex(void);

// headicon.cpp:26
CHeadIcon::CHeadIcon(void) {
	m_visible = true;

	for (int i = 0; i < 4; i++) {
		m_heads[i].m_iconId = 0xFFFF;
		m_heads[i].m_elapsed = 0.0f;
		m_heads[i].m_levelIcon = 0;
	}

	m_emoticonObject = g_resrcmng->GetOverlay("[emoti_object.png", 0);

	m_dstRect.w = 64.0f;
	m_dstRect.h = 64.0f;
	m_srcRect.w = 0.25f;
	m_srcRect.h = 0.25f;

	m_icons.clear();

	for (unsigned char i = 0; i < 44; i++) {
		if (i == 13)
			continue;

		IFF_STRUCT::sItem *item = ItemManager()->FindItem(0x18000000 | i);
		if (item == 0)
			continue;

		WOverlay *overlay = g_resrcmng->GetOverlay(MakeStr("%s.tga", item->m_icon), 0);
		if (overlay == 0)
			continue;

		m_icons[item->m_typeId] = overlay;
	}
}

// headicon.cpp:64
CHeadIcon::~CHeadIcon(void) {
	if (g_resrcmng && m_emoticonObject) { g_resrcmng->Release(m_emoticonObject); m_emoticonObject = 0; }

	for (int i = 0; i < 4; i++) {
		if (g_resrcmng && m_heads[i].m_levelIcon) { g_resrcmng->Release(m_heads[i].m_levelIcon); m_heads[i].m_levelIcon = 0; }
	}

	for (std::map<unsigned long, WOverlay*>::iterator it = m_icons.begin(); it != m_icons.end(); ++it) {
		if (g_resrcmng && it->second) { g_resrcmng->Release(it->second); it->second = 0; }
	}
	m_icons.clear();
}

// headicon.cpp:117
void CHeadIcon::SetPlayerPos(int index, WVector &pos) {
	if (index < 0 || index >= sizeof(m_heads) / sizeof(m_heads[0]))
		return;

	WVector screen = g_view->Projection(pos);

	sPlayerHead &head = m_heads[index];
	WVector diff = pos - g_view->camera.pivot;
	head.m_screenPos.x = screen.x;
	head.m_screenPos.y = screen.y;
	head.m_screenPos.z = screen.z;
	head.m_distance = diff.Magnitude();
}

// headicon.cpp:145
bool CHeadIcon::SetIcon(unsigned long uid, unsigned short iconId) {
	unsigned char index = WSingleton<CSharedDoc>::Instance()->GetIndex(uid);
	if (index < 4) {
		sPlayerHead &head = m_heads[index];
		head.m_iconId = iconId;
		head.m_elapsed = 0.0f;
		return true;
	}

	return false;
}

// headicon.cpp:160
bool CHeadIcon::SendIcon(unsigned short iconId) {
	if (OnlinePlay()) {
		WSendPacket packet((enumClientPacket)0x18);
		packet.Encode2(iconId);
		packet.Send(SEND_TO_DEFAULT);

		return SetIcon(WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().guid, iconId);
	}

	return SetIcon(GOLFDOC()->m_myPlayerIndex, iconId);
}

// headicon.cpp:175
void CHeadIcon::Process(float elapsed) {
	int playerNum = GOLFDOC()->GetPlayerNum();
	int num = playerNum > 4 ? 4 : playerNum;

	for (int i = 0; i < num; i++) {
		if (PLAYER(i)->m_state == 3)
			continue;

		m_heads[i].m_elapsed += elapsed;
		if (m_heads[i].m_elapsed > 3.0f && m_heads[i].m_iconId != 0x1D) {
			m_heads[i].m_iconId = 0xFFFF;
			m_heads[i].m_elapsed = 0.0f;
		}
	}
}

// headicon.cpp:197
void CHeadIcon::DisplayOverPlayer(void) {
	if (!m_visible)
		return;

	if (WSingleton<CProjectG>::Instance()->HidePrivacy())
		return;

	if (GOLFDOC()->GameMode() & 0x20000) {
		for (unsigned char i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
			unsigned char roomType = WSingleton<CSharedDoc>::Instance()->m_roomType;
			if ((roomType == 0 || roomType == 3 || roomType == 7) && PLAYER(i)->m_state == 3)
				continue;

			DisplayOverPlayer(i, true);
		}
	} else if (GOLFDOC()->GameMode() & 0x1000) {
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1) {
			DisplayOverPlayer(MyIndex(), false);
			if (PLAYER(MyIndex())->m_partnerIndex != 0xFF)
				DisplayOverPlayer(PLAYER(MyIndex())->m_partnerIndex, false);
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 3) {
			DisplayOverPlayer(MyIndex(), false);
		} else if (WSingleton<CSharedDoc>::Instance()->m_roomType == 7) {
			DisplayOverPlayer(MyIndex(), false);
		} else {
			DisplayOverPlayer(GOLFDOC()->m_myPlayerIndex, false);
		}
	} else {
		DisplayOverPlayer(GOLFDOC()->m_myPlayerIndex, false);
	}
}
