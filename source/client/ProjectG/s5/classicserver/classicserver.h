#pragma once

namespace S5
{
	namespace CLASSICSRV
	{
		enum eWINDPOWERSTEP
		{
			WINDPOWERSTEP_WEAK,
			WINDPOWERSTEP_MIDDLE,
			WINDPOWERSTEP_STRONG
		};

		int IsClassicServer(unsigned long serverFlag);
		eWINDPOWERSTEP GetWindPowerStep(float windPower);
		float WindRollSpeed(float windPower);
		void WindAdditionalPitchAngleProcess(float elapsed);
		float GetWindAdditionalPitchAngle();
	}
}
