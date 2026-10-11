// Original: d:\build\custom\temp\code\client\projectg\obmarker.h
#ifndef PG_CLIENT_PROJECTG_OBMARKER_H
#define PG_CLIENT_PROJECTG_OBMARKER_H

#include <vector>

#include "..\..\defines.h"
#include "wmath.h"

class W3dSpr;

// The course's out-of-bounds markers: one sprite drawn at every ground point
// of the list.
class CMarker {
	LAYOUT_CHECK_FRIEND;
public:
	CMarker(void);
	virtual ~CMarker(void);

	void Reset(void);
	void AddList(const std::vector<WVector>& list);
	void Display(void);
	void GetReady(void);

private:
	std::vector<WVector> m_list;    // 0x4, ground points of the markers
	W3dSpr* m_pSpr;                 // 0x14, marker sprite (GetReady)
};
CHECK_SIZE(CMarker, 0x18);
BEGIN_LAYOUT_CHECK(CMarker)
	CHECK_FIELD(CMarker, m_list, 0x4);
	CHECK_FIELD(CMarker, m_pSpr, 0x14);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_OBMARKER_H
