#pragma once

#include <string>
#include <vector>
#include <map>
#include "topiconinterface.h"

class TiXmlNode;
class TiXmlDocument;

typedef CIconInterface* (*CreateTopIconInstanceFunc)();
typedef void (*DestoryTopIconInstanceFunc)(CIconInterface*);

struct sTopIcon
{
	std::string name;
	CIconInterface* pInstance;

	sTopIcon(std::string& _name, CIconInterface* _pInstance)
		: name(_name)
	{
		pInstance = _pInstance;
	}

	~sTopIcon() { }
};

struct sInstanceFunctions
{
	CreateTopIconInstanceFunc pCreateInstance;
	DestoryTopIconInstanceFunc pDestoryInstance;

	sInstanceFunctions()
	{
		pCreateInstance = NULL;
		pDestoryInstance = NULL;
	}
	sInstanceFunctions(CreateTopIconInstanceFunc _pCreate,
		DestoryTopIconInstanceFunc _pDestory)
	{
		pCreateInstance = _pCreate;
		pDestoryInstance = _pDestory;
	}
	~sInstanceFunctions() { }
};

class CIconManager : public WSingleton<CIconManager>
{
public:
	CIconManager();
	virtual ~CIconManager();

	void RegisterInstanceFunction(std::string name,
		CreateTopIconInstanceFunc pCreate, DestoryTopIconInstanceFunc pDestory);
	void InitalizeTopIcon(unsigned long index, int wnd);
	void OnButtonDown(unsigned long index);
	bool IsIncludedIcon(std::string name);

private:
	void LoadGameData(const char* filename);
	void ParseTopIconData(TiXmlDocument& doc);
	void ParseEventIconData(TiXmlNode* pNode);
	void ParseWebIconData(TiXmlNode* pNode);
	void RegisterInstances();
	void Destory();

	std::vector<sTopIcon*> m_topIcons;
	std::map<std::string, sInstanceFunctions> m_instanceFunctions;
	int m_outSideIconCount;

public:
	void AddTopIconFromOutSide() { m_outSideIconCount++; }
	void ClearInformation() { m_outSideIconCount = 0; }
};

#define DECLARE_TOPICON(className, iconName) \
public: \
	className() \
	{ \
	} \
	virtual ~className() \
	{ \
	} \
	static CIconInterface* CreateTopIconInstance() \
	{ \
		return new className; \
	} \
	static void DestoryTopIconInstance(CIconInterface* pInstance) \
	{ \
		if (pInstance) \
			delete pInstance; \
	} \
	static void RegisterToTopIconDB() \
	{ \
		CIconManager::Instance()->RegisterInstanceFunction(iconName, \
			CreateTopIconInstance, DestoryTopIconInstance); \
	}
