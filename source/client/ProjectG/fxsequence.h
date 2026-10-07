#pragma once

#include <map>
#include <string>
#include "fxspray.h"

// TODO: incomplete
class CFxSequence
{
public:
	void SetActive(bool bActive, bool bChild);
	WVector& Pos();
	std::map<CFxSpray*, CFxSpray*>& GetSprayList();
	void SetDelay(float delay);
	void GetBoneMatrix(WMatrix* mat);
	void SetWind(const WVector& wind);
	void SetLight(const LightSet& light);

	unsigned char m_unused0[0x20];
	std::map<CFxSpray*, CFxSpray*> m_spray;
	WMatrix* m_attachMat[4];
	unsigned char m_unused3c[0x10c8];
	bool m_bActive;
	WVector m_pos;
	WVector m_vel;
	bool m_bDelete;
	float m_time;
	float m_delay;
	unsigned char m_unused112c[0x128];
	std::string m_filename;
};

#include "fxsequence.inl"
