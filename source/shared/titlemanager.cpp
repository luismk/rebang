// Original: d:\build\custom\temp\code\shared\titlemanager.cpp
#include "../shared/TitleManager.h"
#include "../client/ProjectG/Titles_Client.h"

// titlemanager.cpp:10
TitleManager::TitleManager(void) {
}

// titlemanager.cpp:13
TitleManager::~TitleManager(void) {
	std::map<eTitle, Title *>::iterator it = m_titles.begin();
	for (; it != m_titles.end(); ++it) {
		delete it->second;
	}
	m_titles.clear();
}

// titlemanager.cpp:24
void TitleManager::Register(eTitle title, Title *instance) {
	std::map<eTitle, Title *>::iterator it = m_titles.find(title);
	if (it == m_titles.end()) {
		m_titles.insert(std::map<eTitle, Title *>::value_type(title, instance));
	} else {
		delete instance;
	}
}

// titlemanager.cpp:35
Title *TitleManager::GetTitle(eTitle title) {
	std::map<eTitle, Title *>::iterator it = m_titles.find(title);
	if (it == m_titles.end()) {
		return NULL;
	}
	return it->second;
}
