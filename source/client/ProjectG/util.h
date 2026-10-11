// Original: d:\build\custom\temp\code\client\projectg\util.h
#ifndef PG_CLIENT_PROJECTG_UTIL_H
#define PG_CLIENT_PROJECTG_UTIL_H

#include <string>

#include "..\..\defines.h"
#include "thirties/gamath.h"
#include "wmath.h"

class IObject;

namespace Ut {

// A placement: uniform scale, rotation and position (GroundItem, the
// GroundItemMan placements). Size 0x20.
struct Spatial {
	// util.h:30
	Spatial(void) {
		m_scale = 1.0f; m_rotation = gaQ::ID; m_position = gaV3::ZERO;
	}

	// util.h:36
	void Reset(void) {
		m_scale = 1.0f;
		m_rotation = gaQ::ID;
		m_position = gaV3::ZERO;
	}

	// util.h:43
	// The world matrix: scale, then rotation, then translation.
	void ToMatrix(WMatrix &mat) const {
		WMatrix rot;
		reinterpret_cast<const WQuat &>(m_rotation).ConvertToRotationMatrix(rot);

		mat.Reset();
		mat *= m_scale;
		mat = mat * rot;

		mat.pivot = reinterpret_cast<const WVector &>(m_position);
	}

	float m_scale;      // 0x0
	gaQ m_rotation;     // 0x4
	gaV3 m_position;    // 0x14
};

CHECK_SIZE(Spatial, 0x20);
CHECK_FIELD(Spatial, m_rotation, 0x4);
CHECK_FIELD(Spatial, m_position, 0x14);

void MakeObjectName(std::string &name, const char *pszPrefix, const IObject &object, int id);

}

#endif // PG_CLIENT_PROJECTG_UTIL_H
