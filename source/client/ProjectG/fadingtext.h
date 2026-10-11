// Original: d:\build\custom\temp\code\client\projectg\fadingtext.h
#ifndef PG_CLIENT_PROJECTG_FADINGTEXT_H
#define PG_CLIENT_PROJECTG_FADINGTEXT_H

#include "..\..\defines.h"

class FrWnd;

// How the text's alpha evolves (values from CFadingText::Print/Process).
enum eFadeType {
	eFADE_OUT = -1,     // starts opaque, fades out, ends after the duration
	eFADE_IN = 1,       // starts transparent, fades in and stays
	eFADE_IN_OUT = 2,   // fades in, then out, ends after the duration
};

class CFadingText {
public:
	CFadingText();
	~CFadingText();

	void Print(const char* text, float duration, FrWnd* pWnd, float x, float y, eFadeType type,
		unsigned long color);
	void Process(void);

	char m_text[0x200];         // 0x0
	float m_duration;           // 0x200
	FrWnd* m_pWnd;              // 0x204
	float m_x;                  // 0x208
	float m_y;                  // 0x20c
	unsigned long m_color;      // 0x210
	eFadeType m_type;           // 0x214
	float m_elapsed;            // 0x218
	unsigned char m_alpha;      // 0x21c
	bool m_active;              // 0x21d
	int m_direction;            // 0x220
};
CHECK_SIZE(CFadingText, 0x224);
CHECK_FIELD(CFadingText, m_duration, 0x200);
CHECK_FIELD(CFadingText, m_pWnd, 0x204);
CHECK_FIELD(CFadingText, m_x, 0x208);
CHECK_FIELD(CFadingText, m_y, 0x20c);
CHECK_FIELD(CFadingText, m_color, 0x210);
CHECK_FIELD(CFadingText, m_type, 0x214);
CHECK_FIELD(CFadingText, m_elapsed, 0x218);
CHECK_FIELD(CFadingText, m_alpha, 0x21c);
CHECK_FIELD(CFadingText, m_active, 0x21d);
CHECK_FIELD(CFadingText, m_direction, 0x220);

#endif
