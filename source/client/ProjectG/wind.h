#pragma once

// TODO: incomplete
class CWind
{
public:
	WVector GetWind(const WVector& pos);
	WVector GetGlobalWind();
	float GetGlobalIntensity();
	void CheckSpecialWind();
};

CWind& Wind();
