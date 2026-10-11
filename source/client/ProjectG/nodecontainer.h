// Original: d:\build\custom\temp\code\client\projectg\nodecontainer.h
#ifndef PG_CLIENT_PROJECTG_NODECONTAINER_H
#define PG_CLIENT_PROJECTG_NODECONTAINER_H

#include <string>
#include <vector>

#include "..\..\defines.h"
#include "wmath.h"

// A named polygon of a course (WPolySoup keeps them; FindNode looks them up
// by name): a typed list of points. The type tells what the polygon bounds
// (1 and 3: the in-bounds area, 2: an out-of-bounds area, 4: the auto-target
// area, 6: the alternative camera bounds). The members are public until the
// remaining users go through the accessors.
class CNodeContainer {
	LAYOUT_CHECK_FRIEND;
public:
	enum eType {
	};

	// nodecontainer.h:13
	CNodeContainer(std::string name) : m_name(name), m_type((eType)0) {}
	// nodecontainer.h:14
	virtual ~CNodeContainer() { m_points.clear(); }

	// nodecontainer.h:34
	void SetType(eType type) { m_type = type; }
	// nodecontainer.h:35
	eType GetType(void) const { return m_type; }
	// nodecontainer.h:36
	std::string GetName(void) const { return m_name; }
	// nodecontainer.h:38
	void Add(WVector point) { m_points.push_back(point); }
	// nodecontainer.h:48
	const std::vector<WVector> &GetList(void) const { return m_points; }

	std::string m_name;             // 0x04
	eType m_type;                   // 0x20
	std::vector<WVector> m_points;  // 0x24, the polygon
};
CHECK_SIZE(CNodeContainer, 0x34);
BEGIN_LAYOUT_CHECK(CNodeContainer)
	CHECK_FIELD(CNodeContainer, m_name, 0x04);
	CHECK_FIELD(CNodeContainer, m_type, 0x20);
	CHECK_FIELD(CNodeContainer, m_points, 0x24);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NODECONTAINER_H
