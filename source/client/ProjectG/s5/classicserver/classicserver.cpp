// Original: d:\build\custom\temp\code\client\projectg\s5\classicserver\classicserver.cpp
#include "s5/classicserver/ClassicServer.h"
#include "SharedDoc.h"
#include <stdlib.h>

namespace {

float l_LIMITANGLE = 10.0f;
float l_WINDANGLE;

} // namespace

// Swing direction of the additional pitch angle: true while it climbs
// towards a positive limit. No public symbol in the PDB.
static bool data_ac05ac = true;

namespace S5 {

namespace CLASSICSRV {

// classicserver.cpp:26
int IsClassicServer(unsigned long property) {
	return (OnlinePlay() && (property & 0x80)) ? TRUE : FALSE;
}

// classicserver.cpp:38
eWINDPOWERSTEP GetWindPowerStep(float power) {
	if (power < 4.0f) {
		return WINDPOWERSTEP_WEAK;
	} else if (power >= 4.0f && power < 7.0f) {
		return WINDPOWERSTEP_MEDIUM;
	}
	return WINDPOWERSTEP_STRONG;
}

// classicserver.cpp:52
float WindRollSpeed(float power) {
	if (power < 4.0f) {
		return 1.4137167f;
	} else if (power >= 4.0f && power < 7.0f) {
		return 3.2986722f;
	} else if (power >= 7.0f) {
		return 7.0685835f;
	}
	return 0.0f;
}

// classicserver.cpp:66
void WindAdditionalPitchAngleProcess(float power) {
	if (data_ac05ac) {
		if (l_LIMITANGLE <= 0.0f) {
			l_LIMITANGLE = (float)(rand() % 12 + 3);
		}
		l_WINDANGLE += l_LIMITANGLE * power * 1.3f;
		if (l_WINDANGLE > l_LIMITANGLE) {
			data_ac05ac = false;
		}
	} else {
		if (l_LIMITANGLE >= 0.0f) {
			l_LIMITANGLE = -(float)(rand() % 12 + 3);
		}
		l_WINDANGLE += l_LIMITANGLE * power * 1.3f;
		if (l_WINDANGLE < l_LIMITANGLE) {
			data_ac05ac = true;
		}
	}
}

// classicserver.cpp:93
float GetWindAdditionalPitchAngle(void) {
	return l_WINDANGLE;
}

} // namespace CLASSICSRV

} // namespace S5
