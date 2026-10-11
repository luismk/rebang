// Original: d:\build\custom\temp\code\client\projectg\finitestatemachine.h
#ifndef PG_CLIENT_PROJECTG_FINITESTATEMACHINE_H
#define PG_CLIENT_PROJECTG_FINITESTATEMACHINE_H

#include <map>

#include "..\..\defines.h"

class FiniteState;

class FiniteStateMachine {
	LAYOUT_CHECK_FRIEND;

public:
	FiniteStateMachine(void);
	virtual ~FiniteStateMachine();

	unsigned long getCurrentStateID(void);
	void setCurrentState(unsigned long state);
	void addStateTransition(unsigned long state, unsigned long input, unsigned long output);
	void deleteTransition(unsigned long state, unsigned long input);
	unsigned long getOutputState(unsigned long input);
	void stateTransition(int input);

private:
	std::map<unsigned long, FiniteState *> m_states;    // 0x4
	FiniteState *m_currentState;                         // 0x10
};
CHECK_SIZE(FiniteStateMachine, 0x14);
BEGIN_LAYOUT_CHECK(FiniteStateMachine)
	CHECK_FIELD(FiniteStateMachine, m_states, 0x4);
	CHECK_FIELD(FiniteStateMachine, m_currentState, 0x10);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FINITESTATEMACHINE_H
