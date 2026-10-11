// Original: d:\build\custom\temp\code\client\projectg\finitestatemachine.cpp
#include "FiniteStateMachine.h"
#include "FiniteState.h"

// finitestatemachine.cpp:19
FiniteStateMachine::FiniteStateMachine(void) : m_currentState(NULL) {
}

// finitestatemachine.cpp:23
FiniteStateMachine::~FiniteStateMachine() {
	std::map<unsigned long, FiniteState *>::iterator it = m_states.begin();

	while (it != m_states.end()) {
		delete (*it).second;
		m_states.erase(it++);
	}
}

// finitestatemachine.cpp:36
void FiniteStateMachine::addStateTransition(unsigned long state, unsigned long input, unsigned long output) {
	FiniteState *pState = NULL;
	std::map<unsigned long, FiniteState *>::iterator it = m_states.begin();

	while (it != m_states.end()) {
		pState = (*it).second;

		if (pState->getStateID() == state) {
			break;
		}
		++it;
	}

	if (it == m_states.end()) {
		pState = new FiniteState(state);
		m_states[pState->getStateID()] = pState;
	}

	pState->addTransition(input, output);
}

// finitestatemachine.cpp:64
void FiniteStateMachine::deleteTransition(unsigned long state, unsigned long input) {
	std::map<unsigned long, FiniteState *>::iterator it;
	FiniteState *pState = NULL;
	std::map<unsigned long, FiniteState *>::iterator iter = m_states.begin();

	while (iter != m_states.end()) {
		pState = (*iter).second;

		it = iter;
		if (pState->getStateID() == state) {
			break;
		}
		++iter;
	}

	if (iter == m_states.end()) {
		return;
	}

	pState->deleteTransition(input);
	if (pState->getCount() == 0) {
		delete pState;

		m_states.erase(it);
	}
}

// finitestatemachine.cpp:95
unsigned long FiniteStateMachine::getOutputState(unsigned long input) {
	FiniteState *pState;

	pState = m_states[m_currentState->getStateID()];

	return pState->outputState(input);
}

// finitestatemachine.cpp:111
void FiniteStateMachine::setCurrentState(unsigned long state) {
	std::map<unsigned long, FiniteState *>::iterator it;

	it = m_states.find(state);

	m_currentState = (*it).second;
}

// finitestatemachine.cpp:123
unsigned long FiniteStateMachine::getCurrentStateID(void) {
	return m_currentState->getStateID();
}

// finitestatemachine.cpp:132
void FiniteStateMachine::stateTransition(int input) {
	unsigned long output = m_currentState->outputState(input);

	m_currentState = m_states[output];
}
