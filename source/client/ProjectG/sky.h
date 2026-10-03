#pragma once

// TODO: incomplete
class CSky
{
public:
	CSky();
	~CSky();

	void LoadSky(const char* name, float turn);
	void RegisterCloud(char* name);
	void GenerateClouds(int num, float rate);
	void GenerateStars(int map);
	void GenerateMoons();
	void GenerateLensFlare();

	unsigned char m_unused0[0x8d8];
	unsigned long m_color;
	unsigned char m_unused8dc[0xec];
};
