#pragma once

#include "frcmdtarget.h"

class FrRankingDlg;

class CRankingInfo : public WSingleton<CRankingInfo>, public FrCmdTarget
{
public:
	CRankingInfo();

	bool Open();
	void Close();
	void Process(float delta);
	void OnNotify(const char* msg, unsigned long type);

	FrRankingDlg* GetDlg() { return m_pDlg; }

protected:
	bool OnRankingDlgResult(int result, FrForm* pForm);

	FrRankingDlg* m_pDlg;
};

inline CRankingInfo* RANKING()
{
	return CRankingInfo::Instance();
}

inline FrRankingDlg* RANKINGDLG()
{
	return RANKING()->GetDlg();
}
