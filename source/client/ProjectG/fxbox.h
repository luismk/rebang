#pragma once

#include "fxsequence.h"

// TODO: incomplete
class CFxBox
{
public:
	CFxBox();
	virtual ~CFxBox();

	virtual void ProcessAttacher(float elapsed) { }

	void SetGroundHeightArea(float maxHeight, float minHeight)
	{
		m_groundHeightMax = maxHeight;
		m_groundHeightMin = minHeight;
	}

protected:
	virtual bool CheckGroundCollision(const WVector& from, const WVector& to,
		float& height, float radius);
	virtual void OnSequenceCreated(CFxSequence* sequence);
	virtual void OnSequenceDeleted(CFxSequence* sequence);
	virtual void OnSprayCreated(CFxSpray* spray);
	virtual void OnSprayDeleted(CFxSpray* spray);

	unsigned char m_unused4[0x340];
	float m_groundHeightMax;
	float m_groundHeightMin;
};
