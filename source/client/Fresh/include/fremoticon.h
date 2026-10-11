// Original: d:\build\custom\temp\code\client\fresh\include\fremoticon.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FREMOTICON_H
#define PG_CLIENT_FRESH_INCLUDE_FREMOTICON_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"

class FrWndManager;
class WOverlay;

// The emoticon set of the Fresh UI: icons read from emoticon.xml, drawn from
// one overlay sheet, and printed inline in text. Layout, access and methods
// from the PDB (size 0x48).
class FrEmoticon {
	LAYOUT_CHECK_FRIEND;
public:
	struct sEmoticon {
		std::string Name;          // 0x00
		std::string Alias[2];      // 0x1c
		WOverlay* Overlay;         // 0x54
		std::vector<int> Frames;   // 0x58
	};
	typedef std::vector<sEmoticon*> EMOTICONLIST;
	typedef EMOTICONLIST::iterator EMOTICONLIST_itr;
	typedef std::map<std::string, WOverlay*> RESMAP;
	typedef RESMAP::iterator RESMAP_itr;

	FrEmoticon(FrWndManager* pManager);
	virtual ~FrEmoticon(void);

	bool Init(void);
	bool Draw(int icon, const WRect& rect, unsigned long diffuse, bool flip);
	bool Draw(int icon, float x, float y, unsigned long diffuse, bool flip);
	float PrintText(const WPoint& pos, unsigned long align, const char* text, unsigned long emoDiffuse);
	float PrintText11(const WPoint& pos, unsigned long align, const char* text, unsigned long emoDiffuse);
	float GetTextWidth(const char* text);
	float GetTextWidth11(const char* text);
	int GetIconIndex(const char* key);
	const char* GetIconName(int index);
	int GetIconNum(void);
	void GetAlias(std::string (*buffer)[2], int index);
	int GetWidth(void) { return m_selWidth; }                     // fremoticon.h:29
	int GetHeight(void) { return m_selHeight; }                   // fremoticon.h:30
	void SetClippingArea(WRect* clip);
	void SetAnim(bool enable) { m_EnableAnim = enable; }          // fremoticon.h:33
	void SetAnimTime(unsigned long time) { m_AnimStart = time; }  // fremoticon.h:34

protected:
	void ParseAnimFrame(const std::string& Frames, std::vector<int>& Vector);
	bool LoadXml(const char* fileName);

	int m_Fps;                   // 0x04
	EMOTICONLIST m_Emoticons;    // 0x08
	RESMAP m_ResMap;             // 0x18
	bool m_EnableAnim;           // 0x24
	int m_AnimStart;             // 0x28
	WOverlay* m_overlay;         // 0x2c
	int m_selWidth;              // 0x30
	int m_selHeight;             // 0x34
	int m_selXNum;               // 0x38
	int m_selYNum;               // 0x3c
	int m_selNum;                // 0x40
	FrWndManager* m_pManager;    // 0x44
};
CHECK_SIZE(FrEmoticon::sEmoticon, 0x68);
CHECK_FIELD(FrEmoticon::sEmoticon, Alias, 0x1c);
CHECK_FIELD(FrEmoticon::sEmoticon, Overlay, 0x54);
CHECK_FIELD(FrEmoticon::sEmoticon, Frames, 0x58);
CHECK_SIZE(FrEmoticon, 0x48);
BEGIN_LAYOUT_CHECK(FrEmoticon)
	CHECK_FIELD(FrEmoticon, m_Fps, 0x4);
	CHECK_FIELD(FrEmoticon, m_Emoticons, 0x8);
	CHECK_FIELD(FrEmoticon, m_ResMap, 0x18);
	CHECK_FIELD(FrEmoticon, m_EnableAnim, 0x24);
	CHECK_FIELD(FrEmoticon, m_AnimStart, 0x28);
	CHECK_FIELD(FrEmoticon, m_overlay, 0x2c);
	CHECK_FIELD(FrEmoticon, m_selWidth, 0x30);
	CHECK_FIELD(FrEmoticon, m_selHeight, 0x34);
	CHECK_FIELD(FrEmoticon, m_selXNum, 0x38);
	CHECK_FIELD(FrEmoticon, m_selYNum, 0x3c);
	CHECK_FIELD(FrEmoticon, m_selNum, 0x40);
	CHECK_FIELD(FrEmoticon, m_pManager, 0x44);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FREMOTICON_H
