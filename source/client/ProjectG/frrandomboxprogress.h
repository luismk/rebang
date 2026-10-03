#pragma once
class FrGaugeBar;
#include "frform.h"
class FrStatic;
class FrRandomBoxProgress : public FrForm
{
public:
	DECLARE_OBJECT(FrRandomBoxProgress)

	FrRandomBoxProgress();
	virtual ~FrRandomBoxProgress();

	void SetExplane(const char* text);

protected:
	virtual bool OnInit();
	virtual void OnProc(const float dt);

	void OnCancelUp(int param);
	void OnProgressInit(int param);
	void OnExplaneStatic_Init(int param);

	FrGaugeBar* m_pProgress;
	FrStatic* m_pExplaneStatic;
	unsigned long m_boxTid;
	float m_totalTime;
	float m_elapsedTime;

	DECLARE_FRESH_MSGMAP()

public:
	void SetBoxTid(unsigned long tid) { m_boxTid = tid; }
};
