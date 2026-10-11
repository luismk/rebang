// Original: d:\build\custom\temp\code\client\projectg\invitedlg.h
#ifndef PG_CLIENT_PROJECTG_INVITEDLG_H
#define PG_CLIENT_PROJECTG_INVITEDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

// Room invitation dialog; its size (0x120) is the `new` of
// FrInviteDlgMakeInstance. The members are the invitation SetInvaiteInfo
// stores. The constructor and destructor are inline at invitedlg.h:23-24;
// their emitted copies (with the getters) are defined out of line in
// lobbymain.cpp.
class FrInviteDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // invitedlg.h:20

	FrInviteDlg(void) { Init(); }   // invitedlg.h:23
	virtual ~FrInviteDlg(void) {}   // invitedlg.h:24

	void SetRoomInfo(unsigned short roomIdx);
	void SetInvaiteInfo(unsigned long svrGUID, unsigned char channelIdx, unsigned short roomIdx, unsigned long fromUID, unsigned long toUID);
	unsigned long GetSvrGUID(void);     // invitedlg.h:30
	unsigned char GetChannelIdx(void);  // invitedlg.h:31
	unsigned short GetRoomIdx(void);    // invitedlg.h:32
	unsigned long GetFromUID(void);     // invitedlg.h:34

private:
	void Init(void);

	unsigned long m_svrGUID;        // 0x110
	unsigned char m_channelIdx;     // 0x114
	unsigned short m_roomIdx;       // 0x116
	unsigned long m_fromUID;        // 0x118
	unsigned long m_toUID;          // 0x11C
};

CHECK_SIZE(FrInviteDlg, 0x120);
BEGIN_LAYOUT_CHECK(FrInviteDlg)
	CHECK_FIELD(FrInviteDlg, m_svrGUID, 0x110);
	CHECK_FIELD(FrInviteDlg, m_channelIdx, 0x114);
	CHECK_FIELD(FrInviteDlg, m_roomIdx, 0x116);
	CHECK_FIELD(FrInviteDlg, m_fromUID, 0x118);
	CHECK_FIELD(FrInviteDlg, m_toUID, 0x11C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_INVITEDLG_H
