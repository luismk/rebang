// Original: d:\build\custom\temp\code\client\projectg\headicon.h
#ifndef PG_CLIENT_PROJECTG_HEADICON_H
#define PG_CLIENT_PROJECTG_HEADICON_H

#include <map>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "wmath.h"
#include "../client/Wangreal/include/wtypes.h"

class WOverlay;

// The icons drawn over the players' heads. The constructor (headicon.cpp:26)
// sets m_visible, resets the four player slots and fills the std::map at
// 0xF0 with the emoticon overlays by type id.
class CHeadIcon : public WSingleton<CHeadIcon> {
public:
	// One player's head icon (0x28 bytes).
	struct sPlayerHead {
		unsigned short m_iconId;    // 0x0, 0xFFFF = none
		PADDING(0x2, 0x4);
		float m_elapsed;            // 0x4
		WVector m_screenPos;        // 0x8
		PADDING(0x14, 0x20);
		float m_distance;           // 0x20, from the camera (SetPlayerPos)
		WOverlay* m_levelIcon;      // 0x24, level or ladder badge (SetLevel)
	};

	CHeadIcon(void);
	virtual ~CHeadIcon(void);

	void SetPlayerPos(int index, WVector &pos);
	void SetLevel(void);
	void Process(float elapsed);
	void DisplayBesideBoard(float x, float y);
	bool SetIcon(unsigned long uid, unsigned short iconId);
	bool SendIcon(unsigned short iconId);
	void DisplayOverPlayer(void);

	bool GetVisible(void) const;    // headicon.h:30
	void Show(void);                // headicon.h:31
	void Hide(void);                // headicon.h:32

private:
	void DisplayOverPlayer(unsigned char index, bool team);

protected:
	bool m_visible;                 // 0x28
	PADDING(0x29, 0x2C);
	sPlayerHead m_heads[4];         // 0x2C
	WOverlay* m_emoticonObject;     // 0xCC
	_WRECT m_srcRect;               // 0xD0, the emoticon's cell in emoti_object (DisplayBesideBoard)
	_WRECT m_dstRect;               // 0xE0, where it is drawn
	std::map<unsigned long, WOverlay*> m_icons; // 0xF0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CHeadIcon::sPlayerHead, 0x28);
CHECK_FIELD(CHeadIcon::sPlayerHead, m_elapsed, 0x4);
CHECK_FIELD(CHeadIcon::sPlayerHead, m_screenPos, 0x8);
CHECK_FIELD(CHeadIcon::sPlayerHead, m_distance, 0x20);
CHECK_FIELD(CHeadIcon::sPlayerHead, m_levelIcon, 0x24);
BEGIN_LAYOUT_CHECK(CHeadIcon)
	CHECK_FIELD(CHeadIcon, m_visible, 0x28);
	CHECK_FIELD(CHeadIcon, m_heads, 0x2C);
	CHECK_FIELD(CHeadIcon, m_emoticonObject, 0xCC);
	CHECK_FIELD(CHeadIcon, m_srcRect, 0xD0);
	CHECK_FIELD(CHeadIcon, m_dstRect, 0xE0);
	CHECK_FIELD(CHeadIcon, m_icons, 0xF0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_HEADICON_H
