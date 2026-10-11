// Original: d:\build\custom\temp\code\client\projectg\intrusion.h
#ifndef PG_CLIENT_PROJECTG_INTRUSION_H
#define PG_CLIENT_PROJECTG_INTRUSION_H

#include "..\..\defines.h"
#include "frcmdtarget.h"
#include "singleton.h"

class FrForm;
class WReceivedPacket;
struct sRoomInfo;

enum eIntrusionDenyCode {
};

enum eIntrusionPermitCode {
};

// Joining a game already in progress ("intrusion"). Layout from the
// constructor (intrusion.cpp:41) and the accessors; the size is not proven.
class CIntrusion : public FrCmdTarget, public WSingleton<CIntrusion> {
	LAYOUT_CHECK_FRIEND;
public:
	// An elapsed and a remaining time in minutes and seconds, filled by
	// GetInfoTime from two millisecond counts.
	struct sInfoTime {
		// intrusion.h:25
		sInfoTime(void) {
			m_elapsedMin = 0;
			m_elapsedSec = 0;
			m_remainMin = 0;
			m_remainSec = 0;
		}

		int m_elapsedMin;   // 0x0
		int m_elapsedSec;   // 0x4
		int m_remainMin;    // 0x8
		int m_remainSec;    // 0xc
	};

	CIntrusion(void);
	virtual ~CIntrusion(void);

	unsigned char GetCalcExpMemberSize(void);
	void FinishGame(void);
	unsigned long GetActiveColor(void);
	bool OnAccessRoom(sRoomInfo& room);
	void DoJoinRoom(unsigned short roomNumber);
	bool IsIntrusionGame(unsigned short roomNumber);
	void PacketAnalysis(WReceivedPacket& packet);

private:
	void GetInfoTime(sInfoTime* pInfo, unsigned long elapsed, unsigned long limit);
	bool OnDelayDlgResult(int result, FrForm* pForm);
	bool OnDenyDlgResult(int result, FrForm* pForm);
	void DenyAnlaysis(eIntrusionDenyCode code);
	void RivalDataUpdate(WReceivedPacket& packet);
	bool OnAlarmDlgResult(int result, FrForm* pForm);
	void DoIntrusionAccess(WReceivedPacket& packet, eIntrusionPermitCode code);
	void PlayersUpdate(int param);

	FrForm* m_pDlg;                         // 0x2c (the open alarm/delay/deny form)
	unsigned short m_roomNumber;            // 0x30
	unsigned char m_calcExpMemberSize;      // 0x32
};

CHECK_SIZE(CIntrusion::sInfoTime, 0x10);
BEGIN_LAYOUT_CHECK(CIntrusion)
	CHECK_FIELD(CIntrusion, m_pDlg, 0x2c);
	CHECK_FIELD(CIntrusion, m_roomNumber, 0x30);
	CHECK_FIELD(CIntrusion, m_calcExpMemberSize, 0x32);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_INTRUSION_H
