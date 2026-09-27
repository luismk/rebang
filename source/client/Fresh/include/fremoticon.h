#pragma once
#include <vector>
#include <map>
#include <string>
#include "wtypes.h"

class WOverlay;
class FrWndManager;

class FrEmoticon
{
public:
	virtual ~FrEmoticon();
	float GetTextWidth(const char* text);
	float PrintText(const WPoint& point, unsigned long style, const char* text,
		unsigned long color);
	void SetAnim(bool enable) { m_EnableAnim = enable; }
	void SetAnimTime(unsigned long time) { m_AnimStart = time; }

protected:
	struct sEmoticon;
	int m_Fps;
	std::vector<sEmoticon*> m_Emoticons;
	std::map<std::string, WOverlay*> m_ResMap;
	bool m_EnableAnim;
	int m_AnimStart;
	WOverlay* m_overlay;
	int m_selWidth;
	int m_selHeight;
	int m_selXNum;
	int m_selYNum;
	int m_selNum;
	FrWndManager* m_pManager;
};
