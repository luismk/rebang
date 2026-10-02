#pragma once

#include <memory>
#include "work.h"

class IActor;

class CTaskManager : public WSingleton<CTaskManager>
{
public:
	CTaskManager();
	virtual ~CTaskManager();

	int PostMsg(const IActor* sender, const char* target, int message,
		int param1, int param2, int param3, unsigned long time);

	void SetWork(CWork* pWork) { m_pWork.reset(pWork); }
	void ReleaseWork() { delete m_pWork.release(); }

protected:
	unsigned char m_unused28[0x44];
	std::auto_ptr<CWork> m_pWork;
	// TODO: this class definition is incomplete
};
