// Original: d:\build\custom\temp\code\client\projectg\invitedlg.cpp
#include "InviteDlg.h"
#include "ObjectFactory.h"

// invitedlg.cpp:14 (run-time type and factory registration of FrInviteDlg)
const WRTTI FrInviteDlg::m_RTTI("FrInviteDlg", &FrForm::m_RTTI);

IObject* FrInviteDlgMakeInstance(void) {
	return new FrInviteDlg;
}

struct __sFrInviteDlg {
	__sFrInviteDlg(void) {
		ObjectFactory().AddObjectFunctor(FrInviteDlgMakeInstance, "FrInviteDlg");
	}
};
static __sFrInviteDlg s_registerFrInviteDlg;

// invitedlg.cpp:17
void FrInviteDlg::Init(void) {
	m_svrGUID = 0xFFFFFFFF;
	m_channelIdx = 0xFF;
	m_roomIdx = 0xFFFF;

	m_fromUID = 0xFFFFFFFF;
	m_toUID = 0xFFFFFFFF;
}

// invitedlg.cpp:27
void FrInviteDlg::SetRoomInfo(unsigned short roomIdx) {
	m_roomIdx = roomIdx;
}

// invitedlg.cpp:32
void FrInviteDlg::SetInvaiteInfo(unsigned long svrGUID, unsigned char channelIdx, unsigned short roomIdx, unsigned long fromUID, unsigned long toUID) {
	m_svrGUID = svrGUID;
	m_channelIdx = channelIdx;
	m_roomIdx = roomIdx;
	m_fromUID = fromUID;
	m_toUID = toUID;
}
