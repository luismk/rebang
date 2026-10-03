#pragma once

#include <map>
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
};

#include "fxsequence.inl"
