// Original: d:\build\custom\temp\code\client\projectg\initdelegate.cpp
#include "InitDelegate.h"

CInitDelegate::DelegateFunction CInitDelegate::m_InitFunctions[CInitDelegate::MAX_FUNCTIONS];
CInitDelegate::DelegateFunction CInitDelegate::m_ReleaseFunctions[CInitDelegate::MAX_FUNCTIONS];
int CInitDelegate::m_numInitFunctions = 0;
int CInitDelegate::m_numReleaseFunctions = 0;

// initdelegate.cpp:19
void CInitDelegate::AddInitFunction(DelegateFunction pFunction) {
	if (m_numInitFunctions < MAX_FUNCTIONS) {
		m_InitFunctions[m_numInitFunctions++] = pFunction;
	}
}

// initdelegate.cpp:31
void CInitDelegate::AddReleaseFunction(DelegateFunction pFunction) {
	if (m_numReleaseFunctions < MAX_FUNCTIONS) {
		m_ReleaseFunctions[m_numReleaseFunctions++] = pFunction;
	}
}

// initdelegate.cpp:43
void CInitDelegate::ExecuteInitFunctions() {
	for (int i = 0; i < m_numInitFunctions; i++) {
		m_InitFunctions[i]();
	}
}

// initdelegate.cpp:51
void CInitDelegate::ExecuteReleaseFunctions() {
	for (int i = 0; i < m_numReleaseFunctions; i++) {
		m_ReleaseFunctions[i]();
	}
}
