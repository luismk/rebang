// Original: d:\build\custom\temp\code\client\projectg\balloontext.h
#ifndef PG_CLIENT_PROJECTG_BALLOONTEXT_H
#define PG_CLIENT_PROJECTG_BALLOONTEXT_H

#include <list>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class WFont;
class WOverlay;
class WView;

// A chat balloon over an avatar.
class ntBalloonText {
	LAYOUT_CHECK_FRIEND;

public:
	ntBalloonText(unsigned long guid, const char *text, const float * const offset, float duration);
	~ntBalloonText(void);

	bool IsVisible(void) const { return m_elapsed < m_duration ? true : false; }   // balloontext.h:21

	void SetText(const char *text);
	void SetTransparency(float transparency);   // balloontext.h:24
	bool Process(float elapsed);
	void Render(WView *view);

private:
	const char *StripSpecialText(const char *text) const;

	unsigned long m_guid;                       // 0x00
	std::string m_text;                         // 0x04
	bool m_textChanged;                         // 0x20 set by SetText, cleared by Render
	int m_textExtent;                           // 0x24 recomputed by Render when m_textChanged
	bool m_isThink;                             // 0x28 text started with '!'
	float m_offset[3];                          // 0x2C
	float m_transparency;                       // 0x38
	float m_duration;                           // 0x3C
	float m_elapsed;                            // 0x40
};

BEGIN_LAYOUT_CHECK(ntBalloonText)
	CHECK_SIZE(ntBalloonText, 0x44);
	CHECK_FIELD(ntBalloonText, m_guid, 0x0);
	CHECK_FIELD(ntBalloonText, m_text, 0x4);
	CHECK_FIELD(ntBalloonText, m_textChanged, 0x20);
	CHECK_FIELD(ntBalloonText, m_textExtent, 0x24);
	CHECK_FIELD(ntBalloonText, m_isThink, 0x28);
	CHECK_FIELD(ntBalloonText, m_offset, 0x2C);
	CHECK_FIELD(ntBalloonText, m_transparency, 0x38);
	CHECK_FIELD(ntBalloonText, m_duration, 0x3C);
	CHECK_FIELD(ntBalloonText, m_elapsed, 0x40);
END_LAYOUT_CHECK

// Owner of every balloon (the WSingleton<ntBalloonTextManager> instance).
class ntBalloonTextManager : public WSingleton<ntBalloonTextManager> {
	LAYOUT_CHECK_FRIEND;
	friend class ntBalloonText;

public:
	void Init(void);
	void Shutdown(void);
	void DeleteAll(void);
	void Process(float elapsed);
	void Render(void);
	ntBalloonText *CreateBalloonText(unsigned long guid, const char *text, const float * const offset, float duration);
	void DeleteBalloonText(ntBalloonText *balloon);

private:
	std::list<ntBalloonText*> m_balloons;       // 0x28
	WFont *m_font;                              // 0x34
	WOverlay *m_frame[9];                       // 0x38 chat_frame_01..09 (nine-slice)
	WOverlay *m_frameCenter;                    // 0x5C chat_frame_00
	WOverlay *m_thinkFrame2;                    // 0x60 framethink_02
	WOverlay *m_thinkFrame1;                    // 0x64 framethink_01
	int m_leftWidth;                            // 0x68 m_frame[3] width
	int m_topHeight;                            // 0x6C m_frame[1] height
	int m_rightWidth;                           // 0x70 m_frame[5] width
	int m_bottomHeight;                         // 0x74 m_frame[7] height
};

BEGIN_LAYOUT_CHECK(ntBalloonTextManager)
	CHECK_FIELD(ntBalloonTextManager, m_balloons, 0x28);
	CHECK_FIELD(ntBalloonTextManager, m_font, 0x34);
	CHECK_FIELD(ntBalloonTextManager, m_frame, 0x38);
	CHECK_FIELD(ntBalloonTextManager, m_frameCenter, 0x5C);
	CHECK_FIELD(ntBalloonTextManager, m_thinkFrame2, 0x60);
	CHECK_FIELD(ntBalloonTextManager, m_thinkFrame1, 0x64);
	CHECK_FIELD(ntBalloonTextManager, m_leftWidth, 0x68);
	CHECK_FIELD(ntBalloonTextManager, m_topHeight, 0x6C);
	CHECK_FIELD(ntBalloonTextManager, m_rightWidth, 0x70);
	CHECK_FIELD(ntBalloonTextManager, m_bottomHeight, 0x74);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BALLOONTEXT_H
