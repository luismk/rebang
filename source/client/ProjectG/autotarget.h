#pragma once

#include <vector>

// TODO: incomplete
class CAutoTarget
{
public:
	CAutoTarget() { }
	~CAutoTarget();

	void Reset();
	float GetAngle();
	int CalcDropPos(const WVector& pos);
	WVector GetPoint(const char* club);
	WVector GetDropPos() { return m_dropPos; }

	void SetList(const std::vector<WVector>& list, const WVector& pos)
	{
		int num = list.size();

		m_pointList.reserve(num + 1);

		m_pointList.assign(list.begin(), list.end());
		m_pointList.push_back(pos);

		for (unsigned int i = 0; i < m_pointList.size(); i++)
			m_pointList[i].y = 0.0f;
	}

	const std::vector<WVector>& GetPointList() { return m_pointList; }

protected:
	float InArea(const WVector& a, const WVector& b, float radius) const;
	WVector GetNearest(unsigned char index);
	float GetDistance(const WVector& a, const WVector& b);
	WVector SolveEquation(unsigned char index, float dist);

	std::vector<WVector> m_pointList;
	WVector m_dropPos;
};
