// Original: d:\build\custom\temp\code\client\projectg\helpmsg.cpp
#include <stdio.h>
#include <string.h>

// The original holds the out-of-line copy of CMouseCursor::IsActive here.
#define HELPMSG_MOUSECURSOR_INLINE

#include "HelpMsg.h"
#include "commonutil.h"
#include "ChatMsg.h"
#include "ClientSetting.h"
#include "InputManager.h"
#include "TaskManager.h"
#include "actor.h"
#include "club.h"
#include "golfball.h"
#include "../shared/Localize.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ItemManager.h"
#include "mousecursor.h"
#include "wfont.h"
#include "woverlay.h"
#include "wresrcmng.h"
#include "wview.h"
#include "wutil.h"

// helpmsg.cpp:26
CHelpMsg::CHelpMsg(void) : m_pOverlay(0), m_helpIndex(-1), m_itemTypeId(0), m_showTime(0), m_itemChanged(false), m_pReplayItem(0) {
	float barY = WSingleton<COption>::Instance()->gGetBar_Y();

	m_pOverlay = g_resrcmng->GetOverlay("[help.jpg", 0);
	m_pFont = WSingleton<CChatMsg>::Instance()->GetMaskedFont();

	m_texRect[0] = WRect(0.0f, 0.0f, 1.0f, 0.25f);
	m_texRect[1] = WRect(0.0f, 0.25f, 1.0f, 0.5f);
	m_texRect[2] = WRect(0.0f, 0.75f, 1.0f, 0.25f);
	m_frameRect = WRect(g_view->GetWidth() - 168.0f, 0.0f, 256, 8);
	m_titleRect = WRect(g_view->GetWidth() - 168.0f, 0.0f, 256, 0);
	m_textRect = WRect(g_view->GetWidth() - 168.0f, barY - 110.0f, 256, 8);

	// Each entry: title, text, key (the /k in the title); /v in a text is
	// the item name, /c0xAARRGGBB/c a colour change, \n a line break.
	char *helpTexts[29][3] = {
		// 0: Pangya combo gauge: fill the whole combo gauge to make a power shot
		{ "\xC6\xCE\xBE\xDF \xC4\xDE\xBA\xB8\xB0\xD4\xC0\xCC\xC1\xF6", "\xC6\xCE\xBE\xDF \xC4\xDE\xBA\xB8 \xB0\xD4\xC0\xCC\xC1\xF6\xB8\xA6 \xB8\xF0\xB5\xCE \xC3\xA4\xBF\xEC\xB8\xE9 \xC6\xC4\xBF\xF6\xBC\xA6\xC0\xBB \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.\x0A/v", 0 },
		// 1: Power shot: the combo gauge is too low for a power shot
		{ "\xC6\xC4\xBF\xF6\xBC\xA6", "\xC6\xCE\xBE\xDF \xC4\xDE\xBA\xB8 \xB0\xD4\xC0\xCC\xC1\xF6\xB0\xA1 \xB8\xF0\xC0\xDA\xB6\xF3\xBC\xAD \xC6\xC4\xBF\xF6\xBC\xA6\xC0\xBB \xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 2: Player info: the players' strokes and personal information
		{ "\xC7\xC3\xB7\xB9\xC0\xCC\xBE\xEE \xC1\xA4\xBA\xB8", "\xC7\xF6\xC0\xE7 \xC2\xFC\xB0\xA1\xC0\xDA\xC0\xC7 \xC5\xB8\xBC\xF6\xB3\xAA \xB0\xB3\xC0\xCE \xC1\xA4\xBA\xB8\xB8\xA6 \xBA\xBC \xBC\xF6 \xC0\xD6\xB4\xC2 \xB0\xF7 \xC0\xD4\xB4\xCF\xB4\xD9.", 0 },
		// 3: Hole cup info: the cup's position as distance and height
		{ "\xC8\xA6\xC4\xC5\xC1\xF6\xC1\xA1 \xC1\xA4\xBA\xB8", "\xC8\xA6\xC4\xC5 \xC1\xF6\xC1\xA1\xC0\xC7 \xC0\xA7\xC4\xA1\xB8\xA6 \xB0\xC5\xB8\xAE\xBF\xCD \xB3\xF4\xC0\xCC \xC1\xC2\xC7\xA5\xB7\xCE \xBE\xCB\xB7\xC1 \xC1\xDD\xB4\xCF\xB4\xD9.\x0A/v", 0 },
		// 4: Club select /k: pick a club of the right distance with the mouse wheel
		{ "\xC5\xAC\xB7\xB4 \xBC\xB1\xC5\xC3 /k", "\xB8\xB6\xBF\xEC\xBD\xBA \xC8\xD9\xC0\xBB \xC0\xCC\xBF\xEB\xC7\xD8 \xC0\xFB\xC0\xFD\xC7\xD1 \xBA\xF1\xB0\xC5\xB8\xAE\xC0\xC7 \xC5\xAC\xB7\xB4\xC0\xBB \xBC\xB1\xC5\xC3 \xC7\xCF\xBD\xCA\xBD\xC3\xBF\xE4. /v", "(PgUp, PgDn)" },
		// 5: Chat window /k: opens and closes the chat window
		{ "\xC3\xA4\xC6\xC3 \xC3\xA2 /k", "\xC3\xA4\xC6\xC3\xC3\xA2\xC0\xBB \xBF\xAD\xB0\xED \xB4\xDD\xBD\xC0\xB4\xCF\xB4\xD9.", "(ENTER)" },
		// 6: Item window /k: opens and closes the item window
		{ "\xBE\xC6\xC0\xCC\xC5\xDB \xC3\xA2 /k", "\xBE\xC6\xC0\xCC\xC5\xDB\xC3\xA2\xC0\xBB \xBF\xAD\xB0\xED \xB4\xDD\xBD\xC0\xB4\xCF\xB4\xD9.", "(I)" },
		// 7: Item window /k: use items during the game, once per stroke
		{ "\xBE\xC6\xC0\xCC\xC5\xDB \xC3\xA2 /k", "\xB0\xD4\xC0\xD3 \xC1\xDF\xBF\xA1 \xC7\xCA\xBF\xE4\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBB \xBE\xB5 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9. \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xC7\xD1\xC5\xB8\xBC\xF6\xBF\xA1 1\xC8\xB8 \xB9\xDB\xBF\xA1 \xB8\xF8\xBE\xB2\xB4\xCF \xBD\xC5\xC1\xDF\xC8\xF7 \xBB\xE7\xBF\xEB\xC7\xCF\xBC\xBC\xBF\xE4.", "(I)" },
		// 8: Shot time limit /v: missing the time limit rings the clock and gives a penalty
		{ "\xC5\xB8\xB1\xB8 \xC1\xA6\xC7\xD1 \xBD\xC3\xB0\xE8 /v", "\xC1\xA6\xC7\xD1\xBD\xC3\xB0\xA3\xBE\xC8\xBF\xA1 \xBC\xA6\xC0\xBB \xC7\xCF\xC1\xF6 \xB8\xF8\xC7\xCF\xB8\xE9 \xBD\xC3\xB0\xE8\xB0\xA1 \xBF\xEF\xB8\xAE\xB8\xE9\xBC\xAD \xB9\xFA\xC1\xA1\xC0\xBB \xB9\xDE\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 9: Wind indicator: shows the wind's strength and direction
		{ "\xC7\xB3\xC7\xE2 \xC7\xB3\xBC\xD3 \xC7\xA5\xBD\xC3\xB1\xE2", "\xB9\xD9\xB6\xF7\xC0\xC7 \xBC\xBC\xB1\xE2\xBF\xCD \xB9\xE6\xC7\xE2\xC0\xBB \xC7\xA5\xBD\xC3\xC7\xD8 \xC1\xDD\xB4\xCF\xB4\xD9.", 0 },
		// 10: (no title) Bunker: hit back toward the nearest fairway
		{ 0, "Bunker\xBF\xA1 \xBA\xFC\xC1\xF6\xB8\xE9 \xC8\xC4\xC5\xF0 \xC7\xCF\xB4\xF5\xB6\xF3\xB5\xB5 \xC0\xCF\xB4\xDC\xC0\xBA \xB0\xA1\xB1\xEE\xBF\xEE Fairway\xB7\xCE \xB0\xF8\xC0\xBB \xC4\xA1\xBC\xBC\xBF\xE4.", 0 },
		// 11: Shot bar /k: click here to shoot
		{ "\xC5\xB8\xB1\xB8 \xB9\xD9 /k", "\xBC\xA6\xC0\xBB \xC7\xCF\xB7\xC1\xB8\xE9 \xC0\xCC\xB0\xF7\xC0\xBB \xC5\xAC\xB8\xAF \xC7\xCF\xBC\xBC\xBF\xE4.", "(SPACE)" },
		// 12: Impact point /k: drag to set the impact point for spin
		{ "\xC5\xB8\xC1\xA1 \xB0\xE1\xC1\xA4 /k", "\xB8\xB6\xBF\xEC\xBD\xBA \xB5\xE5\xB7\xA1\xB1\xD7\xB7\xCE \xB0\xF8\xC0\xC7 \xC5\xB8\xC1\xA1\xC0\xBB \xB0\xE1\xC1\xA4\xC7\xCF\xBF\xA9 \xBD\xBA\xC7\xC9\xC0\xBB \xB0\xA1\xB4\xC9\xC7\xCF\xB0\xD4 \xC7\xD5\xB4\xCF\xB4\xD9.", "(CTRL+\xB9\xE6\xC7\xE2\xC5\xB0)" },
		// 13: Ball lie and ground: power transfer by terrain (fairway, rough, bunker percentages)
		{ "\xBA\xBC\xC4\xC1\xB5\xF0\xBC\xC7\xB0\xFA \xC1\xF6\xB8\xE9 \xBB\xF3\xC8\xB2", "\xC1\xF6\xB8\xE9 \xC6\xAF\xBC\xBA\xB0\xFA \xB1\xE2\xBF\xEF\xB1\xE2\xBF\xA1 \xB5\xFB\xB6\xF3 \xB0\xF8\xC0\xC7 \xC8\xFB \xC0\xFC\xB4\xDE \xBB\xF3\xC5\xC2\xB0\xA1 \xB4\xDE\xB6\xF3 \xC1\xFD\xB4\xCF\xB4\xD9. /v\x0A/c0xFF007A39/cFairway : 90~100%\x0A/c0xFF063100/cRough : 85~95%\x0A/c0xFFD38600/cBunker : 55~85%", 0 },
		// 14: (no title) Alt makes the character transparent
		{ 0, "/c0xFFFF0000/cAlt /c0xFF000000/c\xC5\xB0\xB7\xCE \xC4\xB3\xB8\xAF\xC5\xCD\xB8\xA6 \xC5\xF5\xB8\xED\xC7\xCF\xB0\xD4 \xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 15: Emoticon group
		{ "\xC0\xCC\xB8\xF0\xC6\xBC\xC4\xDC \xB1\xD7\xB7\xEC", "/v", 0 },
		// 16: Club use limit: no further club can be used here
		{ "\xC5\xAC\xB7\xB4\xBB\xE7\xBF\xEB\xC1\xA6\xC7\xD1", "\xB4\xF5 \xC0\xCC\xBB\xF3\xC0\xC7 \xC5\xAC\xB7\xB4\xC0\xBA \xC7\xF6\xC0\xA7\xC4\xA1\xBF\xA1\xBC\xAD \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 17: Item use limit: the selected item cannot be used now
		{ "\xBE\xC6\xC0\xCC\xC5\xDB\xBB\xE7\xBF\xEB\xC1\xA6\xC7\xD1", "\xBC\xB1\xC5\xC3\xC7\xD1 \xBE\xC6\xC0\xCC\xC5\xDB\xC0\xBA \xC7\xF6\xC0\xE7 \xBB\xF3\xC8\xB2\xBF\xA1\xBC\xAD \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 18: Club use limit: no putter during a power shot
		{ "\xC5\xAC\xB7\xB4\xBB\xE7\xBF\xEB\xC1\xA6\xC7\xD1", "\xC6\xC4\xBF\xF6\xBC\xA6 \xBB\xF3\xC5\xC2\xBF\xA1\xBC\xAD\xB4\xC2 \xC6\xDB\xC5\xCD\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 19: Ball move mode: Ctrl+left click on the fairway to move the ball
		{ "\xB0\xF8 \xBF\xC5\xB1\xE2\xB1\xE2 \xB8\xF0\xB5\xE5", "\xB0\xCB\xC0\xBA \xBF\xF8\xC0\xBB \xC1\xA6\xBF\xDC\xC7\xD1 \xC6\xE4\xBE\xEE\xBF\xFE\xC0\xCC \xBA\xCE\xBA\xD0\xBF\xA1 Ctrl+\xB8\xB6\xBF\xEC\xBD\xBA \xBF\xDE\xC2\xCA \xC5\xAC\xB8\xAF\xC0\xBB \xC7\xCF\xBF\xA9 \xB0\xF8\xC0\xBB \xBF\xC5\xB1\xE6 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 20: Ball move forbidden area: the ball cannot be moved here
		{ "\xB0\xF8 \xC0\xCC\xB5\xBF \xBA\xD2\xB0\xA1 \xC1\xF6\xBF\xAA", "\xC0\xCC\xB0\xF7\xBF\xA3 \xB0\xF8\xC0\xBB \xBF\xC5\xB1\xE6 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9. \xC0\xCC\xB0\xF7\xC0\xBB \xC1\xA6\xBF\xDC\xC7\xD1 \xC6\xE4\xBE\xEE\xBF\xFE\xC0\xCC\xBF\xA1 \xB0\xF8\xC0\xBB \xBF\xC5\xB0\xDC \xC1\xD6\xBD\xCA\xBD\xC3\xBF\xE4.", 0 },
		// 21: Replay /k: press R after the shot stops or during a replay to watch it again
		{ "\xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC /k", "\xBC\xA6\xC0\xBB \xC7\xCF\xBF\xA9 \xB3\xAF\xBE\xC6\xB0\xA3 \xB0\xF8\xC0\xCC \xBF\xCF\xC0\xFC\xC8\xF7 \xB8\xD8\xC3\xE1 \xC8\xC4 \xB6\xC7\xB4\xC2 \xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC \xB5\xB5\xC1\xDF, R\xC5\xB0\xB8\xA6 \xB4\xAD\xB7\xAF \xBC\xA6\xC0\xBB \xB4\xD9\xBD\xC3 \xBA\xBC \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.\x0A/v", "(R)" },
		// 22: Unavailable: no top view during the opponent's turn in a ladder match
		{ "\xB1\xE2\xB4\xC9\xBB\xE7\xBF\xEB\xBA\xD2\xB0\xA1", "\xB7\xA1\xB4\xF5\xC0\xFC\xBF\xA1\xBC\xAD\xB4\xC2 \xBB\xF3\xB4\xEB \xC5\xCF\xC0\xCF\xB6\xA7 \xC5\xBE\xBA\xE4\xB8\xA6 \xBA\xBC \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 23: Unavailable: ball moving cannot be used now
		{ "\xB1\xE2\xB4\xC9\xBB\xE7\xBF\xEB\xBA\xD2\xB0\xA1", "\xC1\xF6\xB1\xDD\xC0\xBA \xB0\xF8 \xBF\xC5\xB1\xE2\xB1\xE2\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 24: Unavailable: pause cannot be used now
		{ "\xB1\xE2\xB4\xC9\xBB\xE7\xBF\xEB\xBA\xD2\xB0\xA1", "\xC1\xF6\xB1\xDD\xC0\xBA \xC0\xCF\xBD\xC3\xC1\xA4\xC1\xF6\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 25: Unavailable: replay cannot be used now
		{ "\xB1\xE2\xB4\xC9\xBB\xE7\xBF\xEB\xBA\xD2\xB0\xA1", "\xC1\xF6\xB1\xDD\xC0\xBA \xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC\xB8\xA6 \xBB\xE7\xBF\xEB\xC7\xD2 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0 },
		// 26: Power shot: [temporary] no power shot 2 after a stamina item
		{ "\xC6\xC4\xBF\xF6\xBC\xA6", "[\xC0\xD3\xBD\xC3\xB8\xDE\xBC\xBC\xC1\xF6] \xC3\xBC\xB7\xC2\xBA\xB8\xC1\xB6\xC1\xA6 \xBB\xE7\xBF\xEB\xC8\xC4 \xC6\xC4\xBF\xF6\xBC\xA6""2 \xBA\xD2\xB0\xA1~", 0 },
		// 27: Item use limit: auto caliper already in use
		{ "\xBE\xC6\xC0\xCC\xC5\xDB\xBB\xE7\xBF\xEB\xC1\xA6\xC7\xD1", "\xBF\xC0\xC5\xE4\xC4\xB6\xB8\xAE\xC6\xDB\xBD\xBA\xB8\xA6 \xBB\xE7\xBF\xEB\xC1\xDF\xC0\xCC\xB9\xC7\xB7\xCE \xC1\xDF\xBA\xB9 \xBB\xE7\xBF\xEB \xBA\xD2\xB0\xA1\xB4\xC9 \xC7\xD5\xB4\xCF\xB4\xD9.", 0 },
		// 28: Replay (hole) /k: press R to spend a replay tape and save this hole's replay
		{ "\xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC (\xC8\xA6) /k", "\xC5\xB0\xBA\xB8\xB5\xE5 'R'\xC0\xBB \xB4\xA9\xB8\xA3\xB8\xE9 \xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC \xC5\xD7\xC0\xD9 1\xB0\xB3\xB8\xA6 \xBC\xD2\xBA\xF1\xC7\xCF\xBF\xA9, \xC7\xF6\xC0\xE7 \xC8\xA6\xC0\xC7 \xC7\xC3\xB7\xB9\xC0\xCC \xB3\xBB\xBF\xEB\xC0\xBB \xB8\xAE\xC7\xC3\xB7\xB9\xC0\xCC\xB7\xCE \xC0\xFA\xC0\xE5\xC7\xD2 \xBC\xF6 \xC0\xD6\xBD\xC0\xB4\xCF\xB4\xD9.\x0A/v", "(R)" },
	};

	m_helpTexts = new char *[29][3];
	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 3; j++) {
			if (helpTexts[i][j]) {
				m_helpTexts[i][j] = new char[strlen(helpTexts[i][j]) + 1];
				strcpy(m_helpTexts[i][j], helpTexts[i][j]);
			} else {
				m_helpTexts[i][j] = 0;
			}
		}
	}

	memset(m_title, 0, sizeof(m_title));
	memset(m_text, 0, sizeof(m_text));
	memset(m_keyText, 0, sizeof(m_keyText));
	memset(m_itemText, 0, sizeof(m_itemText));

	CheckReplayAvailable();
}

// helpmsg.cpp:106
CHelpMsg::~CHelpMsg(void) {
	for (int i = 0; i < 29; i++) {
		for (int j = 0; j < 3; j++) {
			if (m_helpTexts[i][j]) { delete[] m_helpTexts[i][j]; m_helpTexts[i][j] = 0; }
		}
	}

	if (m_helpTexts) { delete[] m_helpTexts; m_helpTexts = 0; }
	if (g_resrcmng && m_pOverlay) { g_resrcmng->Release(m_pOverlay); m_pOverlay = 0; }
}

// helpmsg.cpp:120
void CHelpMsg::ResetUiPosition(void) {
	float barY = WSingleton<COption>::Instance()->gGetBar_Y();

	m_frameRect.x = g_view->GetWidth() - 168.0f;
	m_titleRect.x = g_view->GetWidth() - 168.0f;
	m_textRect.x = g_view->GetWidth() - 168.0f;
	m_textRect.y = barY - 110.0f;
	if (m_helpIndex != -1 || m_itemTypeId != 0) CalcWindowSize(m_title, m_text);
}

// helpmsg.cpp:133
void CHelpMsg::Process(float elapsed) {
	// The help shown for the previous cursor area while an item is selected.
	static int s_lastIndex = 29;

	char text[32];
	char distText[32];
	char countText[32];
	float x, y;
	float power;

	if (IsLocalContent((localContentType_t)0x4F) && WSingleton<CSharedDoc>::Instance()->m_playingMode == 1) {
		if (WSingleton<CSharedDoc>::Instance()->m_replayPlayType == 4 && IsMyTurn(false) && WSingleton<CSharedDoc>::Instance()->m_gameMode != 5 && WSingleton<CSharedDoc>::Instance()->m_gameMode == 0 && GOLFDOC()->m_gameState == 15 && WSingleton<CSharedDoc>::Instance()->m_roomType != 10 && WSingleton<CSharedDoc>::Instance()->m_roomInfo.m_roomMode != 14) {
			if (GetCurMap() == 7) return;
			SetHelpMsg(28, 0);
			return;
		}
	} else if (GOLFDOC()->GameMode() & 0xC0) {
		if (GOLFDOC()->m_gameState == 15 && WSingleton<CSharedDoc>::Instance()->m_gameMode == 0 && WSingleton<CSharedDoc>::Instance()->m_roomType != 10 && IsMyTurn(false) && m_pReplayItem != 0 && m_helpIndex == -1 && m_itemTypeId == 0 && (GOLFDOC()->GameMode() & 0x80) && GolfBall().m_state == 0) {
			if (IsLocalContent((localContentType_t)0x4F)) return;

			// "(%d uses left)"
			sprintf(countText, "(%d\xB9\xF8 \xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9)", m_pReplayItem->m_count);
			SetHelpMsg(21, countText);
			return;
		} else if (m_helpIndex != 21) {
			m_helpIndex = -1;
		}
	} else if (m_helpIndex == 21) {
		m_helpIndex = -1;
	}

	if (!CMouseCursor::Instance()->IsActive()) return;

	if (m_helpIndex == 0x11 || m_helpIndex == 0x10 || m_helpIndex == 1 || m_helpIndex == 0x12 || m_helpIndex == 0x14 || m_helpIndex == 0x16 || m_helpIndex == 0x17 || m_helpIndex == 0x18 || m_helpIndex == 0x1B) {
		if (m_showTime > 2.0f) {
			m_helpIndex = -1;
		} else {
			m_showTime += elapsed;
			return;
		}
	}

	m_itemText[0] = 0;
	text[0] = 0;
	int area = CMouseCursor::Instance()->GetArea();
	int index = 29;
	switch (area) {
	case 6:
		index = 6;
		break;
	case 18:
		index = 7;
		break;
	case 1:
	case 2:
		index = 4;
		// "(%.0fy club %s)"
		sprintf(m_itemText, "(%.0fy\xBF\xEB %s)", GolfClub().GetRange(0), GolfClub().GetName());
		break;
	case 7:
		index = 5;
		break;
	case 5:
		index = 11;
		break;
	case 14:
	case 15:
	case 16:
	case 17: {
		index = 0;
		float gauge;
		if (WSingleton<CSharedDoc>::Instance()->m_roomType == 1) {
			gauge = TEAM((unsigned char)(area - 14))->m_teamRate;
		} else {
			gauge = PLAYER((unsigned char)(area - 14))->m_comboGauge;
		}
		// "(%d power shots available)"
		sprintf(m_itemText, "(\xC6\xC4\xBF\xF6\xBC\xA6\xC0\xBB %d\xB9\xF8 \xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9)", float2int(gauge * 0.030303031f));
		break;
	}
	case 10:
	case 11:
	case 12:
	case 13:
		index = 2;
		break;
	case 19:
		index = 9;
		break;
	case 20: {
		unsigned long remain = GOLFDOC()->m_remainShotTime;
		unsigned long seconds = remain / 1000;
		index = 8;
		if (remain > seconds * 1000) seconds++;
		// "(now %d s)"
		sprintf(m_itemText, "(\xC7\xF6\xC0\xE7 %d\xC3\xCA)", seconds);
		break;
	}
	case 21: {
		WVector diff = GetHoleCupPos() - GolfBall().m_pos;

		float height = diff.y * 0.3125f * 0.9144f;

		diff.y = 0.0f;
		float dist = diff.Magnitude() * 0.3125f;
		float absHeight = Abs(height);
		if (absHeight < 0.1f) {
			// "(height:0.0m, "
			sprintf(text, "(\xB3\xF4\xC0\xCC:0.0m, ");
		} else if (absHeight > 10.0f) {
			// "(height:%+.0fm, "
			sprintf(text, "(\xB3\xF4\xC0\xCC:%+.0fm, ", height);
		} else {
			// "(height:%+.1fm, "
			sprintf(text, "(\xB3\xF4\xC0\xCC:%+.1fm, ", height);
		}

		if (dist < 1.0f) {
			// "distance:%.1fy)"
			sprintf(distText, "\xB0\xC5\xB8\xAE:%.1fy)", dist);
		} else {
			// "distance:%.0fy)"
			sprintf(distText, "\xB0\xC5\xB8\xAE:%.0fy)", dist);
		}
		strcat(text, distText);

		index = 3;
		break;
	}
	case 3:
		if (g_input->GetButton(LEFT_BUTTON)) break;

		AfxGetTask()->GetActor("Spin") << MsgObject(0, 0x166, (intptr_t)&x, (intptr_t)&y, 0, 0, 0);
		x -= g_input->GetMousePoint().x;
		y -= g_input->GetMousePoint().y;

		if (GolfClub().GetType() != 3 && x * x + y * y < 25.0f) {
			index = 12;
		} else {
			index = 13;
			AfxGetTask()->GetActor("Screen") << MsgObject(0, 0x1C2, (intptr_t)&power, 0, 0, 0, 0);
			// "(now %.0f%%)"
			sprintf(text, "(\xC7\xF6\xC0\xE7 %.0f%%)", power * 100.0f);
		}
		break;
	}

	if (area >= 0x33 && area <= 0x3E) index = 15;

	if (m_itemTypeId != 0) {
		if (index == 29) {
			s_lastIndex = index;
			index = 7;
		} else {
			if (index != s_lastIndex) {
				s_lastIndex = index;
				m_itemChanged = false;
			}
			if (m_itemChanged) index = 7;
		}
	}

	if (GOLFDOC()->m_dropPending) index = 19;

	if (m_helpIndex == 21) {
		index = 21;
	}
	if (index < 29) {
		if (index == 15) return;
		SetHelpMsg(index, text[0] ? text : 0);
	} else {
		m_helpIndex = -1;
	}
}

// helpmsg.cpp:368
void CHelpMsg::CalcWindowSize(char *title, char *text) {
	int pos = 0;
	if (title) {
		m_titleRect.h = 20.0f;
	} else {
		m_titleRect.h = 0;
	}

	int width;
	int length = 0;
	int type = -1;
	for (;;) {
		type = GetLine(&width, &length, 0, text + pos, type == 5 || type == 6);

		pos += length;

		if (type == 0) {
			m_titleRect.h += 11.0f;
			break;
		}

		if (type == 1 || type == 4 || type == 2 || type == 3) continue;

		if (type == 6) {
			pos++;
		}
		m_titleRect.h += 15.0f;
	}

	m_titleRect.y = m_textRect.y - m_titleRect.h;
	m_frameRect.y = m_titleRect.y - 8.0f;
}

// helpmsg.cpp:516
void CHelpMsg::Display(void) {
	if (m_helpIndex < 0) return;

	float h = 0;
	m_pOverlay->Render(g_view, m_texRect[0], m_frameRect, 0, 0xffffffff, 0, 0);
	m_pOverlay->Render(g_view, m_texRect[1], m_titleRect, 0, 0xffffffff, 0, 0);
	m_pOverlay->Render(g_view, m_texRect[2], m_textRect, 0, 0xffffffff, 0, 0);

	float y = m_frameRect.y + 8.0f;
	float x;
	if (m_title[0]) {
		char *p = strstr(m_title, "/v");
		if (p) {
			char c = *p;
			*p = 0;
			x = g_view->GetWidth() - 159.0f;
			x += m_pFont->Print(g_view, x, y, m_title, 0, 0xffff0000, 0);
			*p = c;
			m_pFont->Print(g_view, x, y, m_itemText, 0, 0xffff0000, 0);
		} else {
			p = strstr(m_title, "/k");
			if (p) {
				char c = *p;
				*p = 0;
				x = g_view->GetWidth() - 159.0f;
				x += m_pFont->Print(g_view, x, y, m_title, 0, 0xffff0000, 0);
				*p = c;
				m_pFont->Print(g_view, x, y, m_keyText, 0, 0xffff0000, 0);
			} else {
				m_pFont->Print(g_view, g_view->GetWidth() - 159.0f, y, m_title, 0, 0xffff0000, 0);
			}
		}
		y += 20.0f;
	}

	x = g_view->GetWidth() - 156.0f;
	int pos = 0;
	int start = 0;
	int length = 0;
	int type = -1;
	unsigned long color = 0xff000000;
	do {
		type = GetLine(&start, &length, &color, &m_text[pos], type == 5 || type == 6);

		start += pos;
		pos += length;

		if (type == 1) continue;

		if (type == 2) {
			h = m_pFont->Print(g_view, x, y, m_itemText, 0, 0xffff0000, 0);
			x += h;
			continue;
		}
		if (type == 3) {
			h = m_pFont->Print(g_view, x, y, m_keyText, 0, 0xffff0000, 0);
			x += h;
			continue;
		}

		if (pos > start) {
			char c = m_text[pos];
			m_text[pos] = 0;
			h = m_pFont->Print(g_view, x, y, &m_text[start], 0, color, 0);
			m_text[pos] = c;
		}

		if (type == 4) {
			x += h;
			continue;
		}

		x = g_view->GetWidth() - 156.0f;
		y += 15.0f;
		if (type == 6) {
			pos++;
			continue;
		}
	} while (type != 0);

	m_pFont->Flush(g_view);
}

// helpmsg.cpp:613
void CHelpMsg::SetItemHelpMsg(unsigned long typeId) {
	if (typeId != m_itemTypeId && typeId != 0) {
		m_itemChanged = true;
	}
	m_itemTypeId = typeId;
}

// helpmsg.cpp:621
void CHelpMsg::SetHelpMsg(int index, char *text) {
	if (GOLFDOC()->m_gameState < 15) return;

	if (index < 0) {
		m_helpIndex = -1;
		return;
	}

	if (index == 0x11 || index == 1 || index == 0x10 || index == 0x12 || index == 0x14 || index == 0x16 || index == 0x17 || index == 0x18 || index == 0x1B) {
		m_showTime = 0;
	}

	if (text) strcpy(m_itemText, text);

	if (index == 7 && m_itemTypeId != 0 && m_itemTypeId != 0x1800002D) {
		m_helpIndex = index;

		IFF_STRUCT::sItem *pItem = ItemManager()->FindItem(m_itemTypeId);
		IFF_STRUCT::sDesc *pDesc = ItemManager()->FindDesc(m_itemTypeId);

		if (pItem) {
			strncpy(m_title, pItem->m_name, 0x1F);
			strcat(m_title, " /k");
		}
		if (pDesc) {
			strncpy(m_text, pDesc->m_text, 0xFF);
		}
		strncpy(m_keyText, "(\xBD\xBA\xC6\xE4\xC0\xCC\xBD\xBA\xB9\xD9)", 0x10);

		CalcWindowSize(m_title, m_text);
	} else if (index > -1 && m_helpIndex != index) {
		m_helpIndex = index;
		if (m_helpTexts[index][0]) {
			strncpy(m_title, m_helpTexts[index][0], 0x1F);
		} else {
			m_title[0] = 0;
		}

		strcpy(m_text, m_helpTexts[m_helpIndex][1]);

		if (m_helpTexts[m_helpIndex][2]) {
			strncpy(m_keyText, m_helpTexts[m_helpIndex][2], 0xF);
		} else {
			m_keyText[0] = 0;
		}

		CalcWindowSize(m_helpTexts[m_helpIndex][0], m_helpTexts[m_helpIndex][1]);
	}
}

// helpmsg.cpp:692
void CHelpMsg::SetEmoticonHelpMsg(char **commands) {
	char *title = "\xC0\xCC\xB8\xF0\xC6\xBC\xC4\xDC \xB8\xED\xB7\xC9\xBE\xEE";

	m_helpIndex = 15;
	strcpy(m_title, title);

	m_text[0] = 0;

	strcat(m_text, "/c0xFFFF0000/c/");
	strcat(m_text, commands[0]);

	for (int i = 1; commands[i] != 0 && i <= 2; i++) {
		strcat(m_text, "/c0xFF000000/c, "); strcat(m_text, "/c0xFFFF0000/c/"); strcat(m_text, commands[i]);
	}

	CalcWindowSize(m_title, m_text);
}

// helpmsg.cpp:719
void CHelpMsg::OnChangeTurn(void) {
	m_helpIndex = -1;
	m_itemTypeId = 0;
}

// helpmsg.cpp:725
void CHelpMsg::CheckReplayAvailable(void) {
	CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();

	std::list<sItemInfo>::iterator it;
	for (it = pDoc->m_itemInfoList.begin(); it != pDoc->m_itemInfoList.end(); ++it) {
		if ((*it).m_typeId == 0x1A00004F) break;
	}

	if (it == pDoc->m_itemInfoList.end()) {
		m_pReplayItem = 0;
	} else {
		m_pReplayItem = &(*it);
	}
}
