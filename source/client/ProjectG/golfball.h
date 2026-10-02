#pragma once

#include "encrypttypes.h"

class CGolfBall
{
public:
	enum eBallState
	{
		BALL_STOP,
		BALL_BOUNCE,
		BALL_SPIN,
		BALL_ROLL,
		BALL_HOLE
	};

	enum
	{
		MAX_EXPECTED = 1500,
		EXTRA_EXPECTED = 100
	};

	struct sExpected
	{
		WVector pos;
		WVector vel;
		float curveRot;
		float spinRot;
		char sound[64];
		unsigned char event;
		unsigned char effect;
		WVector normal;
		eBallState state;
	};

	CGolfBall();
	~CGolfBall();

	const sExpected* PreData(int index) const;

	float GetRadius() const { return m_radius; }

	WVector m_pos;
	WVector m_vel;
	eBallState m_state;
	unsigned char m_unused1c[0x48];
	int m_renderNum;
	unsigned char m_unused68[0x5c];
	float m_mass;
	float m_radius;
	sExpected m_expected[MAX_EXPECTED];
	sExpected* m_pExtra;
	unsigned char m_extraNum;
	WCrypticValue<int> m_holeIn;
};

inline CGolfBall& GolfBall()
{
	static CGolfBall me;
	return me;
}

inline CGolfBall::~CGolfBall()
{
	if (m_pExtra)
	{
		delete[] m_pExtra;
		m_pExtra = NULL;
	}
}

inline const CGolfBall::sExpected* CGolfBall::PreData(int index) const
{
	if (index < 0 || index > m_renderNum)
	{
		return NULL;
	}

	if (index < MAX_EXPECTED)
		return &m_expected[index];
	else
		return &m_pExtra[index - MAX_EXPECTED];
}
