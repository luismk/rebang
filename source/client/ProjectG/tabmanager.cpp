// Original: d:\build\custom\temp\code\client\projectg\tabmanager.cpp
#include "TabManager.h"

#include <stdarg.h>

#include "FrWnd.h"

// tabmanager.cpp:9
CTabManager::CTabManager(void) {
}

// tabmanager.cpp:13
CTabManager::~CTabManager(void) {
}

// tabmanager.cpp:17
void __cdecl CTabManager::Add(int id, ...) {
	sTabElement* pElement = Find(id);

	if (pElement == NULL) {
		sTabElement element;
		element.m_id = id;
		m_tabs.push_back(element);
		pElement = Find(id);
	}

	va_list args;
	va_start(args, id);

	FrWnd** ppWnd = va_arg(args, FrWnd**);
	while (ppWnd != NULL) {
		pElement->m_wnds.push_back(ppWnd);
		ppWnd = va_arg(args, FrWnd**);
	}
	va_end(args);
}

// tabmanager.cpp:43
sTabElement* CTabManager::Find(int id) {
	for (unsigned int i = 0; i < m_tabs.size(); i++) {
		if (m_tabs[i].m_id == id) {
			return &m_tabs[i];
		}
	}
	return NULL;
}

// tabmanager.cpp:54
void CTabManager::Open(int id) {
	for (unsigned int i = 0; i < m_tabs.size(); i++) {
		if (m_tabs[i].m_id != id) {
			for (unsigned int j = 0; j < m_tabs[i].m_wnds.size(); j++) {
				FrWnd** ppWnd = m_tabs[i].m_wnds[j];
				if (ppWnd != NULL && *ppWnd != NULL) {
					(*ppWnd)->SetVisible(false);
				}
			}
		}
	}

	sTabElement* pElement = Find(id);
	if (pElement != NULL) {
		for (unsigned int j = 0; j < pElement->m_wnds.size(); j++) {
			FrWnd** ppWnd = pElement->m_wnds[j];
			if (ppWnd != NULL && *ppWnd != NULL) {
				(*ppWnd)->SetVisible(true);
			}
		}
	}
}
