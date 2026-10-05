#pragma once

struct sRoomInfo;
class WReceivedPacket;

enum eIntrusionPermitCode
{
	INTRUSION_PERMIT_ROOM,
	INTRUSION_PERMIT_SLOTS,
	INTRUSION_PERMIT_MATCH,
	INTRUSION_PERMIT_RIVAL,
	INTRUSION_PERMIT_GAME
};
enum eIntrusionDenyCode
{
	INTRUSION_DENY_NONE,
	INTRUSION_DENY_CROWDED,
	INTRUSION_DENY_LATE,
	INTRUSION_DENY_PRIVATE,
	INTRUSION_DENY_RECENT,
	INTRUSION_DENY_CLOSED,
	INTRUSION_DENY_PLAYED,
	INTRUSION_DENY_KICKED
};

class CIntrusion : public FrCmdTarget, public WSingleton<CIntrusion>
{
public:
	struct sInfoTime
	{
		int passMin;
		int passSec;
		int remainMin;
		int remainSec;
		sInfoTime()
			: passMin(0), passSec(0), remainMin(0), remainSec(0)
		{
		}
	};

	CIntrusion();
	virtual ~CIntrusion();

	unsigned char GetCalcExpMemberSize();
	void FinishGame();
	unsigned long GetActiveColor();
	bool OnAccessRoom(sRoomInfo& room);
	void PacketAnalysis(WReceivedPacket& packet);
	bool IsIntrusionGame(unsigned short roomNumber);
	void DoJoinRoom(unsigned short roomNumber);

private:
	void DoIntrusionAccess(WReceivedPacket& packet, eIntrusionPermitCode code);
	void RivalDataUpdate(WReceivedPacket& packet);
	void PlayersUpdate(int state);
	void GetInfoTime(sInfoTime* info, unsigned long passTime,
		unsigned long remainTime);
	bool OnAlarmDlgResult(int result, FrForm* form);
	bool OnDelayDlgResult(int result, FrForm* form);
	bool OnDenyDlgResult(int result, FrForm* form);
	void DenyAnlaysis(eIntrusionDenyCode code);

	FrForm* m_pDlg;
	unsigned short m_roomNumber;
	unsigned char m_calcExpMemberSize;
};
