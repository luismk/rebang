#pragma once

#include <map>

class CHeadIcon : public WSingleton<CHeadIcon>
{
public:
	CHeadIcon();
	virtual ~CHeadIcon();

	bool SetIcon(unsigned long uid, unsigned short icon);
	bool SendIcon(unsigned short icon);
	void SetPlayerPos(int index, WVector& pos);
	void SetLevel();
	void Process(float delta);
	void DisplayOverPlayer();
	void DisplayBesideBoard(float x, float y);

	void Toggle() { m_bVisible = !m_bVisible; }
	bool GetVisible() const { return m_bVisible; }
	void Show() { m_bVisible = true; }
	void Hide() { m_bVisible = false; }

private:
	void DisplayOverPlayer(unsigned char index, bool bBesideName);

	struct sPlayerHead
	{
		unsigned short icon;
		float time;
		WVector screenPos;
		char reserved[12];
		float distance;
		WOverlay* level;
	};

	bool m_bVisible;
	sPlayerHead m_head[4];
	WOverlay* m_emoticon;
	WRect m_srcRect;
	WRect m_dstRect;
	std::map<unsigned long, WOverlay*> m_itemIcon;
};
