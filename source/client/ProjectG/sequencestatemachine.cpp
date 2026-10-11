// Original: d:\build\custom\temp\code\client\projectg\sequencestatemachine.cpp
#include "SequenceStateMachine.h"

#include <algorithm>

// sequencestatemachine.cpp:6
void CStateUnit::OnPrintLog(char const* pszLog) {
}

// sequencestatemachine.cpp:17
CSequenceStateMachine::CSequenceStateMachine(void const* pUser, char const* pszName)
	: m_lastOrder(0), m_pUser(pUser), m_name(pszName) {
	m_current = m_list.end();
	m_unknown34 = 0;
	m_unknown38 = 0;
}

// sequencestatemachine.cpp:28
CSequenceStateMachine::~CSequenceStateMachine(void) {
	Clear();
}

// sequencestatemachine.cpp:37
void CSequenceStateMachine::PrintLog(CStateUnit const* pState, char const* pszLog) {
}

// sequencestatemachine.cpp:45
void CSequenceStateMachine::PrintLog(char const*, ...) {
}

// sequencestatemachine.cpp:58
bool CSequenceStateMachine::Process(float fDelta) {
	CStateUnit* pState = GetCurrentState();
	if (pState) {
		pState->OnProcess(fDelta);
		if (pState->OnFinished()) {
			++m_current;
			PrintLog("%s -> %s", pState->GetClassNameA(), GetCurrentState() ? GetCurrentState()->GetClassNameA() : "");
		}
		return true;
	}
	return false;
}

// sequencestatemachine.cpp:76
void CSequenceStateMachine::Render(void) {
	CStateUnit* pState = GetCurrentState();
	if (pState && !pState->OnFinished())
		pState->OnRender();
}

// sequencestatemachine.cpp:84
void CSequenceStateMachine::Clear(void) {
	std::list<CStateUnit*>::iterator it = m_list.begin();
	for (; it != m_list.end(); ++it) {
		delete *it;
	}
	m_list.clear();
}

// sequencestatemachine.cpp:95
void CSequenceStateMachine::Reset(void) {
	m_current = m_list.begin();
}

// sequencestatemachine.cpp:112
void CSequenceStateMachine::Insert(int order, CStateUnit* pState) {
	pState->m_order = order;
	std::list<CStateUnit*>::iterator it = std::upper_bound(m_list.begin(), m_list.end(), *pState);
	pState->SetOwner(this);
	m_list.insert(it, pState);
	PrintLog("insert %s", pState->GetClassNameA());
	if (m_list.size() == 1)
		m_current = m_list.begin();
}

// sequencestatemachine.cpp:137
void CSequenceStateMachine::BackInsert(CStateUnit* pState) {
	pState->m_order = ++m_lastOrder;
	pState->SetOwner(this);
	m_list.push_back(pState);
	PrintLog("insert %s", pState->GetClassNameA());
	if (m_list.size() == 1)
		m_current = m_list.begin();
}

// sequencestatemachine.cpp:150
CStateUnit* CSequenceStateMachine::GetCurrentState(void) {
	if (m_current != m_list.end())
		return *m_current;
	return NULL;
}

// sequencestatemachine.cpp:158
CStateUnit* CSequenceStateMachine::GetPreviousState(void) {
	CStateUnit* pState = NULL;
	if (m_current != m_list.begin()) {
		pState = *--m_current;
		++m_current;
	}
	return pState;
}

// sequencestatemachine.cpp:169
CStateUnit* CSequenceStateMachine::GetNextState(void) {
	CStateUnit* pState = NULL;
	if (m_current != m_list.end()) {
		if (++m_current != m_list.end()) {
			pState = *m_current;
			--m_current;
		}
	}
	return pState;
}

// sequencestatemachine.cpp:183
bool CSequenceStateMachine::SkipCurrentState(void) {
	if (GetCurrentState()) {
		CStateUnit* pState = GetCurrentState();
		++m_current;
		PrintLog("%s -> %s", pState->GetClassNameA(), GetCurrentState() ? GetCurrentState()->GetClassNameA() : "");
	}
	return m_current != m_list.end();
}
