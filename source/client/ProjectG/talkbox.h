#pragma once

#include <list>

class WFont;
class WOverlay;

class CTalkBox
{
public:
	CTalkBox();
	~CTalkBox();

	void SetText(const char* text, float time);
	bool IsActive() { return m_time > 0.0f ? true : false; }
	void Process(float elapsed);
	void Render(WView* view, float x, float y, int align);

	void SetTransparency(float alpha) { m_transparency = alpha; }

private:
	struct sLine
	{
		char text[32];
		float width;
		unsigned long color;
		int newLine;
	};

	WFont* m_pFont;
	WOverlay* m_pOverlay;
	std::list<sLine> m_lines;
	int m_lineNum;
	float m_transparency;
	float m_time;
	float m_x;
	float m_y;
	char m_text[512];
	unsigned char m_maxLineLen;
};
