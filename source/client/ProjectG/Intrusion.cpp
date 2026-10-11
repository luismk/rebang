// Original: d:\build\custom\temp\code\client\projectg\intrusion.cpp
#include "Intrusion.h"
#include "globalgamedefine.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "frform.h"
#include "fresh.h"
#include "TaskManager.h"

// intrusion.cpp:28
unsigned char CIntrusion::GetCalcExpMemberSize(void) {
	return m_calcExpMemberSize;
}

// intrusion.cpp:34
void CIntrusion::FinishGame(void) {
	m_calcExpMemberSize = 0;
}

// intrusion.cpp:41
CIntrusion::CIntrusion(void) {
	m_pDlg = 0;
	m_roomNumber = 0;
	m_calcExpMemberSize = 0;
}

// intrusion.cpp:45
CIntrusion::~CIntrusion(void) {
}

// intrusion.cpp:54
unsigned long CIntrusion::GetActiveColor(void) {
	return 0xC3E5FFD8;
}

// intrusion.cpp:64
bool CIntrusion::OnAccessRoom(sRoomInfo& room) {
	if (room.m_state == 1) {
		if (room.m_intrusionAccess == 0) {
			room.m_intrusionAccess = 1;
			return true;
		}
	}
	return false;
}

// intrusion.cpp:637
void CIntrusion::GetInfoTime(sInfoTime* pInfo, unsigned long elapsed, unsigned long limit) {
	int elapsedSec = elapsed / 1000;
	int remainSec = limit / 1000 - elapsedSec;

	pInfo->m_elapsedMin = elapsedSec / 60;
	pInfo->m_elapsedSec = elapsedSec - pInfo->m_elapsedMin * 60;
	pInfo->m_remainMin = remainSec / 60;
	pInfo->m_remainSec = remainSec - pInfo->m_remainMin * 60;
}

// intrusion.cpp:697
bool CIntrusion::OnDelayDlgResult(int result, FrForm* pForm) {
	if (result != 1) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x3B, 0, 0, 0, 0, 0));
	}
	m_pDlg = 0;
	return true;
}

// intrusion.cpp:714
bool CIntrusion::OnDenyDlgResult(int result, FrForm* pForm) {
	AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x3B, 0, 0, 0, 0, 0));
	m_pDlg = 0;
	return true;
}

// intrusion.cpp:721
void CIntrusion::DenyAnlaysis(eIntrusionDenyCode code) {
	if (m_pDlg == NULL) {
		m_pDlg = CreateForm<FrForm>(g_pFresh->GetManager(), this, "IntrusionDeny", 0);
		m_pDlg->SetCaption("30\xC0\xCE \xB4\xEB\xC8\xB8 \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5"); // "30-player tournament: join in progress"
		m_pDlg->SetDesc(" \xA1\xD8 \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5 \xB0\xE1\xB0\xFA"); // " * join-in-progress result"

		switch (code) {
		case 1:
			// "The room is full, you cannot enter."
			m_pDlg->SetMessage("\xC0\xCE\xBF\xF8\xC0\xCC \xB0\xA1\xB5\xE6 \xC2\xF7\xBC\xAD \xB9\xE6\xBF\xA1 \xC0\xD4\xC0\xE5 \xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		case 2:
			// "You cannot enter a room whose game started more than 5 minutes ago."
			m_pDlg->SetMessage("\xB0\xD4\xC0\xD3 \xBD\xC3\xC0\xDB \xC8\xC4 5\xBA\xD0\xC0\xCC \xC1\xF6\xB3\xAD \xB9\xE6\xBF\xA1\xB4\xC2 \xC0\xD4\xC0\xE5 \xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		case 3:
			// "You cannot join a private room's game in progress."
			m_pDlg->SetMessage("\xBA\xF1\xB0\xF8\xB0\xB3 \xB9\xE6\xBF\xA1\xB4\xC2 \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5 \xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		case 4:
			// "You joined a game in progress before; you cannot enter for a while."
			m_pDlg->SetMessage("\xC0\xCC\xC0\xFC\xBF\xA1 \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5\xC7\xD1 \xB1\xE2\xB7\xCF\xC0\xCC \xC0\xD6\xBE\xEE\xBC\xAD \xB4\xE7\xBA\xD0\xB0\xA3 \xC0\xD4\xC0\xE5\xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		case 5:
			// "This room does not allow joining a game in progress."
			m_pDlg->SetMessage("\xC7\xF6\xC0\xE7 \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5\xC0\xCC \xBA\xD2\xB0\xA1\xB4\xC9\xC7\xD1 \xB9\xE6\xC0\xD4\xB4\xCF\xB4\xD9.", false);
			break;
		case 6:
			// "You played in this room before; you cannot join its game in progress."
			m_pDlg->SetMessage("\xC7\xD8\xB4\xE7 \xB9\xE6\xBF\xA1\xBC\xAD \xC7\xC3\xB7\xB9\xC0\xCC\xC7\xD1 \xB1\xE2\xB7\xCF\xC0\xCC \xC0\xD6\xBE\xEE\xBC\xAD \xB0\xD4\xC0\xD3\xC1\xDF \xC0\xD4\xC0\xE5\xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		case 7:
			// "You were kicked from this room before; you cannot enter."
			m_pDlg->SetMessage("\xC7\xD8\xB4\xE7 \xB9\xE6\xBF\xA1\xBC\xAD \xB0\xAD\xC1\xA6 \xC5\xF0\xC0\xE5\xB4\xE7\xC7\xD1 \xB1\xE2\xB7\xCF\xC0\xCC \xC0\xD6\xBE\xEE\xBC\xAD \xC0\xD4\xC0\xE5\xC7\xCF\xBD\xC7 \xBC\xF6 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", false);
			break;
		}

		m_pDlg->Open((FRESH_RESULT_CALLBACK)&CIntrusion::OnDenyDlgResult, 3);
	}
}

// intrusion.cpp:758
bool CIntrusion::IsIntrusionGame(unsigned short roomNumber) {
	bool result = false;
	std::list<sRoomInfo>& rooms = CSharedDoc::Instance()->m_roomList;
	for (std::list<sRoomInfo>::iterator it = rooms.begin(); it != rooms.end(); ++it) {
		if (it->m_roomNumber == roomNumber) {
			result = it->m_state;
			break;
		}
	}
	return result;
}

// intrusion.cpp:777
void CIntrusion::DoJoinRoom(unsigned short roomNumber) {
	m_roomNumber = roomNumber;

	WSendPacket packet(0x9A);
	packet.Encode1(0);
	packet.Encode2(m_roomNumber);
	packet.Send((eSendTo)0);
}
