// Original: d:\build\custom\temp\code\client\projectg\frrandomboxprogress.h
#ifndef PG_CLIENT_PROJECTG_FRRANDOMBOXPROGRESS_H
#define PG_CLIENT_PROJECTG_FRRANDOMBOXPROGRESS_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrGaugeBar;
class FrStatic;

// Random box opening progress dialog: a gauge ("waitbar") filled over a random
// 4-7 seconds, then the open request for the box is sent; its size (0x128) is
// the `new` of FrRandomBoxProgressMakeInstance.
class FrRandomBoxProgress : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frrandomboxprogress.h:8

	FrRandomBoxProgress(void);
	virtual ~FrRandomBoxProgress(void);

	void SetExplane(const char* text);
	void SetBoxTid(unsigned long tid);   // frrandomboxprogress.h:37

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual bool OnInit(void);
	virtual void OnProc(const float dt);

	void OnProgressInit(intptr_t param);
	void OnCancelUp(int param);
	void OnExplaneStatic_Init(intptr_t param);

private:
	FrGaugeBar* m_pWaitBar;              // 0x110 ("waitbar")
	FrStatic* m_pExplaneStatic;          // 0x114 ("explane")
	unsigned long m_boxTid;              // 0x118 (sent with packet 0xF1)
	float m_duration;                    // 0x11c (seconds until the box opens)
	float m_elapsed;                     // 0x120
	UNKNOWN(0x124, 0x128);
};
BEGIN_LAYOUT_CHECK(FrRandomBoxProgress)
	CHECK_FIELD(FrRandomBoxProgress, m_pWaitBar, 0x110);
	CHECK_FIELD(FrRandomBoxProgress, m_pExplaneStatic, 0x114);
	CHECK_FIELD(FrRandomBoxProgress, m_boxTid, 0x118);
	CHECK_FIELD(FrRandomBoxProgress, m_duration, 0x11c);
	CHECK_FIELD(FrRandomBoxProgress, m_elapsed, 0x120);
END_LAYOUT_CHECK
CHECK_SIZE(FrRandomBoxProgress, 0x128);

#endif // PG_CLIENT_PROJECTG_FRRANDOMBOXPROGRESS_H
