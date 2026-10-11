// Original: d:\build\custom\temp\code\client\projectg\shufflebonus.cpp
#include <windows.h>
#include <string>

#include "ShuffleBonus.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ChatMsg.h"
#include "../shared/Localize.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wview.h"

// shufflebonus.cpp:9
CShuffleBonus::CShuffleBonus(void) {
}

// shufflebonus.cpp:13
CShuffleBonus::~CShuffleBonus(void) {
}

// shufflebonus.cpp:22
void CShuffleBonus::Render(WOverlay* pOverlay, float alpha) {
	if (pOverlay == 0) {
		return;
	}

	if (_Validity(eVaild_FirstHole)) {
		WRECT texRect = { 0.0f, 0.0f, 1.0f, 1.0f };
		WRECT screenRect = { g_view->GetWidth() * 0.5f - 127.0f, 160.0f, 264.0f, 36.0f };
		pOverlay->Render(g_view, texRect, screenRect, 0, ((int)alpha << 24) | 0xFFFFFF, 0.0f, 0);
	}
}

// shufflebonus.cpp:40
void CShuffleBonus::ShowNoticeMsg(void) {
	if (_Validity(eVaild_FirstHole)) {
		// "(Event) Shuffle Bonus pays 10% extra Pang on game completion."
		WSingleton<CChatMsg>::Instance()->AddChatMsg(std::string("(\276\313\270\262) \260\324\300\323 \277\317\301\326\275\303 Shuffle Bonus\267\316 \306\316\300\273 10% \303\337\260\241 \301\366\261\336 \271\336\275\300\264\317\264\331."), 0xFFFF7878, true, false);
	}
}

// shufflebonus.cpp:55
bool CShuffleBonus::_Validity(eVaildType type) {
	if (!OnlinePlay()) {
		return false;
	}

	bool result = false;
	if (type == eVaild_FirstHole) {
		CSharedDoc* pDoc = WSingleton<CSharedDoc>::Instance();
		if (pDoc->m_holeOrder[0] != GOLFDOC()->m_currentHole) {
			return false;
		}
	}

	if (IsLocalContent((localContentType_t)0x11)) {
		switch (WSingleton<CSharedDoc>::Instance()->m_roomType) {
		case 0:
		case 1:
		case 4:
		case 5:
		case 6:
			if (WSingleton<CSharedDoc>::Instance()->m_gameKind == 3) {
				result = true;
			}
			break;
		}
	}
	return result;
}
