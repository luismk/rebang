#pragma once

#include "taskmanager.h"

class CGolfTask : public CTask
{
public:
	DECLARE_OBJECT(CGolfTask)

	bool DetermineWhetherToAddActor_GroundItemMan();
};
