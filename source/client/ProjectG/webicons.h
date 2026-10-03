#pragma once

#include "topiconinterface.h"
#include "topiconmanager.h"

class CWebIcons_WebEvent1 : public CWebIcon
{
	DECLARE_TOPICON(CWebIcons_WebEvent1, "WebEvent1")

	virtual void OnInit(FrButton& button);
	virtual void OnButtonDown();
};

class CWebIcons_WebEvent2 : public CWebIcon
{
	DECLARE_TOPICON(CWebIcons_WebEvent2, "WebEvent2")

	virtual void OnInit(FrButton& button);
	virtual void OnButtonDown();
};
