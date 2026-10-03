#pragma once

class CLensFlare
{
public:
	CLensFlare();
	~CLensFlare();

	void Init(const WVector& sunDir);
	void SetSunDirection(const WVector& sunDir);
	void Process(float delta);
	void Render(WView* view);

protected:
	struct w_flare_set
	{
		int overlay;
		float pos;
		float scale;
		unsigned long color;
	};

	static w_flare_set ms_lenzTable[7];

	WVector m_sunDir;
	float m_angle;
	float m_alpha;
	WOverlay* m_overlay[2];
	W3dSpr* m_spr[4];
	_WSIZE m_size[4];
};
