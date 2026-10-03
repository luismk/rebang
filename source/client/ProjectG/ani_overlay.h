#pragma once

class WOverlay;

class CAniOverlay
{
public:
	CAniOverlay(const char* filename, float width, float height);
	~CAniOverlay();

	void Render(float x, float y, int frame, int mode, int color, float scale);

private:
	WOverlay* m_pOverlay;
	float m_width;
	float m_height;
	int m_frameNum;
	WRect m_clipRect;
	WRect m_rect;
	int m_cols;
	int m_rows;
};

class CEffectOverlay
{
public:
	CEffectOverlay();
	~CEffectOverlay();

	void Process(float delta);
	void SetActive();
	void Display();

private:
	CAniOverlay* m_pAniOverlay;
	int m_frame;
	float m_frameTime;
	bool m_bActive;
};
