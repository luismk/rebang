// Original: d:\build\custom\temp\code\client\projectg\awarddlg.h
#ifndef PG_CLIENT_PROJECTG_AWARDDLG_H
#define PG_CLIENT_PROJECTG_AWARDDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrArea;

// Award dialog of the result screen; its size (0x140) is the `new` of
// FrAwardDlgMakeInstance. Only the members this unit defines so far are
// declared.
class FrAwardDlg : public FrForm {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // awarddlg.h:18

	FrAwardDlg(void) {}   // awarddlg.h:21

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // awarddlg.cpp:19

	void OnGoldStaticInit(int param);     // awarddlg.cpp:55
	void OnSilverStaticInit(int param);   // awarddlg.cpp:84
	void OnBronzeStaticInit(int param);   // awarddlg.cpp:128
	int GetMedalIndex(unsigned long uid); // awarddlg.cpp:186

	void OnWinner1Init(int param);     // awarddlg.cpp:395
	void OnWinner2Init(int param);     // awarddlg.cpp:441
	void OnWinner3Init(int param);     // awarddlg.cpp:485
	void OnWinner4Init(int param);     // awarddlg.cpp:529
	void OnWinner5Init(int param);     // awarddlg.cpp:573
	void OnWinner6Init(int param);     // awarddlg.cpp:617
	void OnLuckInit(int param);        // awarddlg.cpp:661
	void OnSpeederInit(int param);     // awarddlg.cpp:725
	void OnRunnerInit(int param);      // awarddlg.cpp:789
	void OnChipInInit(int param);      // awarddlg.cpp:854
	void OnLongPuttInit(int param);    // awarddlg.cpp:918
	void OnRecoveryInit(int param);    // awarddlg.cpp:983
	void OnWinner1OwnerDraw(int param);  // awarddlg.cpp:401
	void OnWinner2OwnerDraw(int param);  // awarddlg.cpp:447
	void OnWinner3OwnerDraw(int param);  // awarddlg.cpp:491
	void OnWinner4OwnerDraw(int param);  // awarddlg.cpp:535
	void OnWinner5OwnerDraw(int param);  // awarddlg.cpp:579
	void OnWinner6OwnerDraw(int param);  // awarddlg.cpp:623
	void OnLuckOwnerDraw(int param);     // awarddlg.cpp:666
	void OnSpeederOwnerDraw(int param);  // awarddlg.cpp:730
	void OnRunnerOwnerDraw(int param);   // awarddlg.cpp:794
	void OnChipInOwnerDraw(int param);   // awarddlg.cpp:859
	void OnLongPuttOwnerDraw(int param); // awarddlg.cpp:923
	void OnRecoveryOwnerDraw(int param); // awarddlg.cpp:988

private:
	FrArea* m_pWinnerArea[6];   // 0x110 (one per player slot of the result)
	FrArea* m_pLuckArea;        // 0x128
	FrArea* m_pSpeederArea;     // 0x12c
	FrArea* m_pRunnerArea;      // 0x130
	FrArea* m_pChipInArea;      // 0x134
	FrArea* m_pLongPuttArea;    // 0x138
	FrArea* m_pRecoveryArea;    // 0x13c

	LAYOUT_CHECK_FRIEND;
};

CHECK_SIZE(FrAwardDlg, 0x140);
BEGIN_LAYOUT_CHECK(FrAwardDlg)
	CHECK_FIELD(FrAwardDlg, m_pWinnerArea, 0x110);
	CHECK_FIELD(FrAwardDlg, m_pLuckArea, 0x128);
	CHECK_FIELD(FrAwardDlg, m_pSpeederArea, 0x12c);
	CHECK_FIELD(FrAwardDlg, m_pRunnerArea, 0x130);
	CHECK_FIELD(FrAwardDlg, m_pChipInArea, 0x134);
	CHECK_FIELD(FrAwardDlg, m_pLongPuttArea, 0x138);
	CHECK_FIELD(FrAwardDlg, m_pRecoveryArea, 0x13c);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_AWARDDLG_H
