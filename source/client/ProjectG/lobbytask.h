#pragma once

#include "taskmanager.h"

class WReceivedPacket;

class CLobbyTask : public CTask
{
	DECLARE_OBJECT(CLobbyTask)

protected:
	CLobbyTask();

public:
	virtual ~CLobbyTask();

protected:
	virtual void Init(const char* wallPaper);
	virtual void Register();
	virtual void Load();
	virtual void Process(float delta);
	virtual void Display();
	virtual int OnPacket(WReceivedPacket& packet);
};
