// Original: d:\build\custom\temp\code\client\projectg\topiconmanager.h
#ifndef PG_CLIENT_PROJECTG_TOPICONMANAGER_H
#define PG_CLIENT_PROJECTG_TOPICONMANAGER_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

class CIconInterface;
class TiXmlDocument;
class TiXmlNode;

// One icon slot of the top page: its key and its instance.
struct sTopIcon {
	// topiconmanager.h:22
	sTopIcon(std::string& name, CIconInterface* icon) : m_name(name) {
		m_icon = icon;
	}
	// topiconmanager.h:27
	~sTopIcon(void) {}

	std::string m_name;                 // 0x00
	CIconInterface* m_icon;             // 0x1c
};

CHECK_SIZE(sTopIcon, 0x20);
BEGIN_LAYOUT_CHECK(sTopIcon)
	CHECK_FIELD(sTopIcon, m_name, 0x00);
	CHECK_FIELD(sTopIcon, m_icon, 0x1c);
END_LAYOUT_CHECK

// The factory pair an icon class registers under its key.
struct sInstanceFunctions {
	// topiconmanager.h:40
	sInstanceFunctions(void) {
		m_create = 0;
		m_destroy = 0;
	}
	// topiconmanager.h:45
	sInstanceFunctions(CIconInterface* (*create)(void), void (*destroy)(CIconInterface*)) {

		m_create = create; m_destroy = destroy;
	}
	// topiconmanager.h:50
	~sInstanceFunctions(void) {}

	CIconInterface* (*m_create)(void);          // 0x00
	void (*m_destroy)(CIconInterface*);         // 0x04
};

CHECK_SIZE(sInstanceFunctions, 0x8);
BEGIN_LAYOUT_CHECK(sInstanceFunctions)
	CHECK_FIELD(sInstanceFunctions, m_create, 0x00);
	CHECK_FIELD(sInstanceFunctions, m_destroy, 0x04);
END_LAYOUT_CHECK

// Manager of the lobby's top-page icons (the WSingleton<CIconManager>
// instance). The two inline counters of topiconmanager.h:77-78 are
// emitted by lobbymain.cpp and defined out of line there.
class CIconManager : public WSingleton<CIconManager> {
	LAYOUT_CHECK_FRIEND;
public:
	CIconManager(void);
	virtual ~CIconManager(void);

	bool IsIncludedIcon(std::string name);
	void InitalizeTopIcon(unsigned long slot, int param);
	void OnButtonDown(unsigned long slot);
	void RegisterInstanceFunction(std::string name, CIconInterface* (*create)(void), void (*destroy)(CIconInterface*));
	void AddTopIconFromOutSide(void);   // topiconmanager.h:77
	void ClearInformation(void);        // topiconmanager.h:78

private:
	void ParseEventIconData(TiXmlNode* node);
	void ParseWebIconData(TiXmlNode* node);
	void RegisterInstances(void);
	void Destory(void);
	void ParseTopIconData(TiXmlDocument& document);
	void LoadGameData(const char* fileName);

	std::vector<sTopIcon*> m_icons;                             // 0x28
	std::map<std::string, sInstanceFunctions> m_instanceFunctions; // 0x38
	int m_outSideIconCount;                                     // 0x44
};

CHECK_SIZE(CIconManager, 0x48);
BEGIN_LAYOUT_CHECK(CIconManager)
	CHECK_FIELD(CIconManager, m_icons, 0x28);
	CHECK_FIELD(CIconManager, m_instanceFunctions, 0x38);
	CHECK_FIELD(CIconManager, m_outSideIconCount, 0x44);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TOPICONMANAGER_H
