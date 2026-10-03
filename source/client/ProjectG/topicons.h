#pragma once

#include "topiconinterface.h"
#include "topiconmanager.h"

class CWizcityOpenEvent : public CTopIcon
{
	DECLARE_TOPICON(CWizcityOpenEvent, "WizcityOpenEvent")

	virtual void OnInit(FrButton& button);
	virtual void OnButtonDown();
};
