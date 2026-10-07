#pragma once

struct defines_t
{
	char name[16][260];
	float value[16];
	int num;
};

// TODO: incomplete
class CFxSpray
{
public:
	WVector& Pos() { return m_pos; }
	void SetWind(const WVector& wind);
	void SetLight(const LightSet& light);

	unsigned char m_unused0[0x110c];
	bool m_bActive;
	unsigned char m_unused110d[0x2b];
	WVector m_pos;
};
