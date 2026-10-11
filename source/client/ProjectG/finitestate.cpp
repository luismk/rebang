// Original: d:\build\custom\temp\code\client\projectg\finitestate.cpp
#include "FiniteState.h"

// finitestate.cpp:17
FiniteState::FiniteState(unsigned long stateID) : m_stateID(stateID) {
}

// finitestate.cpp:21
FiniteState::~FiniteState() {
	std::map<unsigned long, unsigned long>::iterator it = m_transitions.begin();

	while (it != m_transitions.end()) {
		m_transitions.erase(it++);
	}
}

// finitestate.cpp:34
void FiniteState::addTransition(unsigned long input, unsigned long output) {
	m_transitions[input] = output;
}

// finitestate.cpp:39
void FiniteState::deleteTransition(unsigned long input) {
	m_transitions.erase(input);
}

// finitestate.cpp:44
unsigned long FiniteState::outputState(unsigned long input) {
	std::map<unsigned long, unsigned long>::iterator it;

	it = m_transitions.find(input);
	if (it == m_transitions.end()) {
		return m_stateID;
	}

	return m_transitions[input];
}

// finitestate.cpp:63
unsigned long FiniteState::getCount(void) {
	return m_transitions.size();
}
