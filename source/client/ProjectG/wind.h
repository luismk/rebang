#pragma once

// TODO: incomplete
class CWind
{
public:
	WVector GetWind(const WVector& pos);
	WVector GetGlobalWind();
	float GetGlobalIntensity();
	float GetGlobalDirection();
	void CheckSpecialWind();
};

CWind& Wind();
