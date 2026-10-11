// Original: d:\build\custom\temp\code\client\projectg\topiconmanager.cpp
#include "TopIconManager.h"
#include "TopIconInterface.h"
#include "TopIcons.h"
#include "WebIcons.h"
#include "tinyxml.h"

// topiconmanager.cpp:10
CIconManager::CIconManager(void) {
	RegisterInstances();
	m_outSideIconCount = 0;
	LoadGameData("TopIcons.xml");
}

// topiconmanager.cpp:19
CIconManager::~CIconManager(void) {
	m_outSideIconCount = 0;
	Destory();
}

// topiconmanager.cpp:27
void CIconManager::LoadGameData(const char* fileName) {
	TiXmlDocument document;

	if (document.LoadFileEx(fileName)) {

		ParseTopIconData(document);
	}
}

// topiconmanager.cpp:40
void CIconManager::ParseTopIconData(TiXmlDocument& document) {
	TiXmlNode* root = document.FirstChild("TopIcons");



	std::string type("");

	TiXmlNode* node = root->FirstChild("Icon");

	while (node) {

		type = node->ToElement()->Attribute("Type");
		if (type != "") {

			if (type == "EVENTICON") {

				ParseEventIconData(node);

			} else if (type == "WEBICON") {

				ParseWebIconData(node);
			}
		}

		node = node->NextSibling("Icon");
	}
}

// topiconmanager.cpp:70
void CIconManager::ParseEventIconData(TiXmlNode* node) {
	std::string name("");

	name = node->ToElement()->Attribute("Name");
	if (name == "") {


		return;
	}

	if (m_instanceFunctions.find(name) != m_instanceFunctions.end()) {

		sTopIcon* icon = new sTopIcon(name, m_instanceFunctions[name].m_create());
		m_icons.push_back(icon);
	}
}

// topiconmanager.cpp:87
void CIconManager::ParseWebIconData(TiXmlNode* node) {
	std::string normalImage, overImage, selectedImage, qaUrl, url, name("");



	name = node->ToElement()->Attribute("Name");


	if (m_instanceFunctions.find(name) == m_instanceFunctions.end())
		return;
	CIconInterface* icon = m_instanceFunctions[name].m_create();


	normalImage = node->ToElement()->Attribute("Normal");



	overImage = node->ToElement()->Attribute("Over");



	selectedImage = node->ToElement()->Attribute("Selected");



	qaUrl = node->ToElement()->Attribute("QAUrl");



	url = node->ToElement()->Attribute("Url");


	icon->SetInformation(normalImage, overImage, selectedImage, qaUrl, url);
	sTopIcon* topIcon = new sTopIcon(name, icon);
	m_icons.push_back(topIcon);
}

// topiconmanager.cpp:125
void CIconManager::RegisterInstances(void) {


	CWizcityOpenEvent::RegisterToTopIconDB();


	CWebIcons_WebEvent1::RegisterToTopIconDB();
	CWebIcons_WebEvent2::RegisterToTopIconDB();
}

// topiconmanager.cpp:139
void CIconManager::RegisterInstanceFunction(std::string name, CIconInterface* (*create)(void), void (*destroy)(CIconInterface*)) {
	m_instanceFunctions.insert(std::map<std::string, sInstanceFunctions>::value_type(name, sInstanceFunctions(create, destroy)));
}

// topiconmanager.cpp:145
void CIconManager::Destory(void) {
	std::vector<sTopIcon*>::iterator it = m_icons.begin();
	for (; it != m_icons.end(); ++it) {

		if (*it) {

			if ((*it)->m_icon)
				m_instanceFunctions[(*it)->m_name].m_destroy((*it)->m_icon);
			delete *it;
		}
	}
	m_icons.clear();
}

// topiconmanager.cpp:160
void CIconManager::InitalizeTopIcon(unsigned long slot, int param) {
	FrButton* pButton = DYNAMIC_CAST(FrButton, param);
	if (!pButton)
		return;

	if (slot >= m_icons.size()) {

		pButton->SetVisible(false);
		pButton->Enable(false);
		return;
	}

	CIconInterface* icon = m_icons[slot]->m_icon;

	if (!icon) {
		pButton->SetVisible(false);
		pButton->Enable(false);
		return;
	}

	WRect rect;
	pButton->GetClientRect(rect);
	rect.x = (float)((m_outSideIconCount + slot) * 80);
	pButton->SetClientRect(rect);

	icon->SetIconImage(*pButton);
	icon->OnInit(*pButton);
}

// topiconmanager.cpp:191
void CIconManager::OnButtonDown(unsigned long slot) {


	m_icons[slot]->m_icon->OnButtonDown();
}

// topiconmanager.cpp:199
bool CIconManager::IsIncludedIcon(std::string name) {
	std::vector<sTopIcon*>::iterator it = m_icons.begin();
	for (; it != m_icons.end(); ++it) {

		if (*it) {

			if (strcmp((*it)->m_name.c_str(), name.c_str()) == 0)
				return true;
		}
	}
	return false;
}
