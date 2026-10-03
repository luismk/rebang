#pragma once

#include <list>
#include "frform.h"

struct sSCardAvilityPeriodInfo;
class FrListBox;

class FrSPCardBuffDlg : public FrForm
{
public:
	DECLARE_OBJECT(FrSPCardBuffDlg)

	FrSPCardBuffDlg();
	virtual ~FrSPCardBuffDlg();

	virtual bool OnInit();
	virtual bool Close(eFormRet ret, bool bSound);
	void SetBuffItem(const std::list<sSCardAvilityPeriodInfo>& cards,
		bool bClear);
	const WPoint& GetPosition() { return m_Pos; }
	int GetSpecialCardNum();
	static void SetPosition(const WPoint& pos) { m_Pos = pos; }
	static void SetSpecialBuffStartTick(unsigned long tick)
	{
		m_SpecialBuffStartTick = tick;
	}

protected:
	virtual void OnProc(const float delta);

	void OnSPCardListInit(int param);
	void OnSPCardListOwnerDraw(int param);
	int GetRemainedSec(const sSCardAvilityPeriodInfo* info);

	FrListBox* m_pSPCardList;

	static WPoint m_Pos;
	static unsigned long m_SpecialBuffStartTick;

	DECLARE_FRESH_MSGMAP()
};
