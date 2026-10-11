// Original: d:\build\custom\temp\code\client\projectg\finitestate.h
#ifndef PG_CLIENT_PROJECTG_FINITESTATE_H
#define PG_CLIENT_PROJECTG_FINITESTATE_H

#include <map>

#include "..\..\defines.h"

// One state of a FiniteStateMachine: its transitions, input -> output state.
// Everything is private; only the machine uses it.
class FiniteState {
	LAYOUT_CHECK_FRIEND;
	friend class FiniteStateMachine;

private:
	FiniteState(unsigned long stateID);
	virtual ~FiniteState();

	unsigned long getStateID(void) { return m_stateID; }
	unsigned long getCount(void);
	void addTransition(unsigned long input, unsigned long output);
	void deleteTransition(unsigned long input);
	unsigned long outputState(unsigned long input);

	unsigned long m_stateID;                                  // 0x4
	std::map<unsigned long, unsigned long> m_transitions;    // 0x8
};
CHECK_SIZE(FiniteState, 0x14);
BEGIN_LAYOUT_CHECK(FiniteState)
	CHECK_FIELD(FiniteState, m_stateID, 0x4);
	CHECK_FIELD(FiniteState, m_transitions, 0x8);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FINITESTATE_H
