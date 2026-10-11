// Original: d:\build\custom\temp\code\client\projectg\letter_effect.h
#ifndef PG_CLIENT_PROJECTG_LETTER_EFFECT_H
#define PG_CLIENT_PROJECTG_LETTER_EFFECT_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "wlist.h"
#include "wtypes.h"
#include "wresrcmng.h"
#include "woverlay.h"

class IObject;
class WTitleFont;

IObject* CLetterMakeInstance(void);

// Overlay texts ("letters") faded in over the screen, plus the replay sign
// and a score printed with the title font. Size 0x84 (the `new` of
// CLetterMakeInstance).
class CLetter : public IActor {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CLetterMakeInstance(void);
public:
	static const WRTTI m_RTTI;

	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

protected:
	CLetter(void);
public:
	// letter_effect.h:13
	virtual ~CLetter(void) {}

	virtual void OnPreLoadInit(void);
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject& msg);

	// A number printed under a text (message 0x191 copies it in), 0x10 bytes.
	struct sScore {
		bool m_show;            // 0x0
		PADDING(0x1, 0x4);
		int m_value;            // 0x4
		float m_scale;          // 0x8
		unsigned long m_color;  // 0xc
	};

	// One overlay text (0x4c bytes).
	struct sText {
		// letter_effect.h:47
		sText(void) {
			m_pOverlay = NULL;
		}

		// letter_effect.h:53
		~sText(void) {
			if (m_pOverlay != NULL) {
				g_resrcmng->Release(m_pOverlay);
				m_pOverlay = NULL;
			}
		}

		WOverlay* m_pOverlay;   // 0x0
		WRect m_rect;           // 0x4, on screen
		WRect m_uv;             // 0x14, normalized source rectangle
		float m_time;           // 0x24, display time left
		char m_name[16];        // 0x28, key in m_textList
		bool m_skippable;       // 0x38, an input ends it
		PADDING(0x39, 0x3c);
		sScore m_score;         // 0x3c
	};

protected:
	void DisplayReplay(void);
	void Print(float x, float y, const char* str, float spacing, int type, float scale, unsigned long color);
	void AddOverlay(const char* name, const char* file, WRect* rect, bool skippable);
	void SetPos(const char* name, float x, float y);

	sText* m_pCurrent;          // 0x18, text shown
	WList<sText*> m_textList;   // 0x1c
	float m_alpha;              // 0x3c, 0..255
	int m_state;                // 0x40, 0 idle, 1 fade in, 2 shown, 3 fade out
	float m_fadeTime;           // 0x44
	bool m_show;                // 0x48
	PADDING(0x49, 0x4c);
	int m_align;                // 0x4c, Print alignment: 1 center, 2 right
	PADDING(0x50, 0x70);
	const char* m_charSet;      // 0x70, characters of m_pFont, by index
	WTitleFont* m_pFont;        // 0x74
	sText* m_pReplayText;       // 0x78
	sText* m_pReplay;           // 0x7c
	float m_time;               // 0x80
};

CHECK_SIZE(CLetter::sScore, 0x10);
CHECK_SIZE(CLetter::sText, 0x4c);
CHECK_SIZE(CLetter, 0x84);
BEGIN_LAYOUT_CHECK(CLetter)
	CHECK_FIELD(CLetter::sText, m_rect, 0x4);
	CHECK_FIELD(CLetter::sText, m_uv, 0x14);
	CHECK_FIELD(CLetter::sText, m_time, 0x24);
	CHECK_FIELD(CLetter::sText, m_name, 0x28);
	CHECK_FIELD(CLetter::sText, m_skippable, 0x38);
	CHECK_FIELD(CLetter::sText, m_score, 0x3c);
	CHECK_FIELD(CLetter::sScore, m_value, 0x4);
	CHECK_FIELD(CLetter::sScore, m_color, 0xc);
	CHECK_FIELD(CLetter, m_pCurrent, 0x18);
	CHECK_FIELD(CLetter, m_textList, 0x1c);
	CHECK_FIELD(CLetter, m_alpha, 0x3c);
	CHECK_FIELD(CLetter, m_state, 0x40);
	CHECK_FIELD(CLetter, m_fadeTime, 0x44);
	CHECK_FIELD(CLetter, m_show, 0x48);
	CHECK_FIELD(CLetter, m_align, 0x4c);
	CHECK_FIELD(CLetter, m_charSet, 0x70);
	CHECK_FIELD(CLetter, m_pFont, 0x74);
	CHECK_FIELD(CLetter, m_pReplayText, 0x78);
	CHECK_FIELD(CLetter, m_pReplay, 0x7c);
	CHECK_FIELD(CLetter, m_time, 0x80);
END_LAYOUT_CHECK

#endif
