// Original: d:\build\custom\temp\code\client\projectg\networkunit.cpp
#include "NetworkUnit.h"

// networkunit.cpp:23
NetworkUnit::NetworkUnit(void) {
	m_fsm.addStateTransition(0, 1, 2);
	m_fsm.addStateTransition(0, 2, 4);

	m_fsm.addStateTransition(1, 0, 2);
	m_fsm.addStateTransition(1, 2, 1);
	m_fsm.addStateTransition(1, 3, 1);

	m_fsm.addStateTransition(2, 0, 3);
	m_fsm.addStateTransition(2, 3, 5);

	m_fsm.addStateTransition(5, 0, 2);

	m_fsm.addStateTransition(3, 0, 0);

	m_fsm.addStateTransition(4, 2, 1);

	Init();
}

// networkunit.cpp:44
NetworkUnit::~NetworkUnit(void) {
}

// networkunit.cpp:51
bool NetworkUnit::Init(void) {
	m_fsm.setCurrentState(1);

	m_connectResult = 0; (&m_socket)->Close();
	return true;
}

// networkunit.cpp:60
void NetworkUnit::setTransition(unsigned long input) {
	if (input == 0 && m_fsm.getCurrentStateID() == 1)
		m_connectResult = 0;

	m_fsm.stateTransition(input);
}

// networkunit.cpp:70
void NetworkUnit::setForceTransition(unsigned long input) {
	m_fsm.setCurrentState(input);
}

// networkunit.cpp:76
unsigned long NetworkUnit::getState(void) {
	return m_fsm.getCurrentStateID();
}

// networkunit.cpp:83
bool NetworkUnit::Process(void) {
	unsigned long state = m_fsm.getCurrentStateID();

	switch (state) {
	case 0:
		return OnLine();
	case 1:
		return OffLine();
	case 2:
		return Connect();
	case 3:
		return Connecting();
	case 4:
		return Disconnecting();
	case 5:
		ResetConnect();
		break;
	}

	return true;
}

// networkunit.cpp:141
int NetworkUnit::IsConnected(void) {
	return m_socket.m_bConnected;
}

// networkunit.cpp:146
int NetworkUnit::IsConnectionComplete(void) {
	return m_connectResult;
}

// networkunit.cpp:151
bool NetworkUnit::ResetConnect(void) {
	return true;
}
