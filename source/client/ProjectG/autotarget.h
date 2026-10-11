// Original: d:\build\custom\temp\code\client\projectg\autotarget.h
#ifndef PG_CLIENT_PROJECTG_AUTOTARGET_H
#define PG_CLIENT_PROJECTG_AUTOTARGET_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"

#include <vector>

// Aiming helper of a golf round: the list of target points of the hole and
// the drop position computed from them. Only the members the functions
// decompiled so far use are declared; the full size is not known yet.
class CAutoTarget {
	LAYOUT_CHECK_FRIEND;
public:
	// autotarget.h:19
	CAutoTarget() {
	}
	~CAutoTarget();

	void Reset(void);
	int CalcDropPos(const WVector &pos);
	WVector GetPoint(const char *clubName);
	float GetAngle(void);

	// autotarget.h:26
	WVector GetDropPos(void) { return m_dropPos; }

	// autotarget.h:29
	void SetList(const std::vector<WVector> &list, const WVector &pos) {
		int count = list.size();

		m_pointList.reserve(count + 1);

		m_pointList.assign(list.begin(), list.end());
		m_pointList.push_back(pos);

		for (unsigned int i = 0; i < m_pointList.size(); ++i)
			m_pointList[i].y = 0.0f;
	}

	// autotarget.h:45
	const std::vector<WVector> &GetPointList(void) { return m_pointList; }

protected:
	float InArea(const WVector &a, const WVector &b, float radius) const;
	WVector GetNearest(unsigned char index);
	float GetDistance(const WVector &a, const WVector &b);
	WVector SolveEquation(unsigned char index, float radius);

private:
	std::vector<WVector> m_pointList;  // 0x0
	WVector m_dropPos;                 // 0x10
};
BEGIN_LAYOUT_CHECK(CAutoTarget)
	CHECK_FIELD(CAutoTarget, m_pointList, 0x0);
	CHECK_FIELD(CAutoTarget, m_dropPos, 0x10);
END_LAYOUT_CHECK

#endif
