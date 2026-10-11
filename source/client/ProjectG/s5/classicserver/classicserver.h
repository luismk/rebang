// Original: d:\build\custom\temp\code\client\projectg\s5\classicserver\classicserver.h
#ifndef PG_CLIENT_PROJECTG_S5_CLASSICSERVER_CLASSICSERVER_H
#define PG_CLIENT_PROJECTG_S5_CLASSICSERVER_CLASSICSERVER_H

namespace S5 {

namespace CLASSICSRV {

// Wind strength band of the classic server's wind (GetWindPowerStep).
enum eWINDPOWERSTEP {
	WINDPOWERSTEP_WEAK = 0,   // power < 4
	WINDPOWERSTEP_MEDIUM = 1, // 4 <= power < 7
	WINDPOWERSTEP_STRONG = 2  // power >= 7
};

int IsClassicServer(unsigned long property);
eWINDPOWERSTEP GetWindPowerStep(float power);
float WindRollSpeed(float power);
void WindAdditionalPitchAngleProcess(float power);
float GetWindAdditionalPitchAngle(void);

} // namespace CLASSICSRV

} // namespace S5

#endif // PG_CLIENT_PROJECTG_S5_CLASSICSERVER_CLASSICSERVER_H
