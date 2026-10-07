#pragma once

#include <map>
#include "taskmanager.h"

class Fresh;
class WReceivedPacket;

class CGolfTask : public CTask
{
public:
	DECLARE_OBJECT(CGolfTask)

protected:
	CGolfTask();

public:
	virtual ~CGolfTask() { }

	virtual void Register();
	virtual void Init(const char* wallPaper);
	virtual void Load();
	virtual void Process(float delta);
	virtual void Display();
	virtual void Destroy();
	virtual int OnPacket(WReceivedPacket& packet);
	virtual void OnInitFinished();
	virtual void OnDisplayWhileLoading();

	bool DetermineWhetherToAddActor_GroundItemMan();

protected:
	void UpdateLoadingInfo(unsigned long uid, unsigned char percent);
	void DrawLoadingInfo(Fresh* pFresh);

	std::map<unsigned long, unsigned char> m_loadingInfo;
	float m_returnToLobbyTime;
};
