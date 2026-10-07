#pragma once

#include <map>
#include "../../shared/localize.h"

struct IContentsDataContainer
{
	IContentsDataContainer() { }
	virtual ~IContentsDataContainer() { }

	virtual bool Initialize() = 0;
	virtual bool Release() = 0;

	virtual bool Reset() { return true; }
};

class CContentsDoc : public WSingleton<CContentsDoc>
{
public:
	CContentsDoc();
	virtual ~CContentsDoc();

	bool Initialize();
	bool Release();

	int InsertContainer(localContentType_t type,
		IContentsDataContainer* pContainer, bool bForce);
	IContentsDataContainer* GetContainer(localContentType_t type);

	template <class T>
	bool GetContainer(localContentType_t type, T& pContainer)
	{
		IContentsDataContainer* p = GetContainer(type);

		if (p != NULL)
		{
			pContainer = (T)p;
			return true;
		}

		return false;
	}

protected:
	std::map<localContentType_t, IContentsDataContainer*> m_containerMap;
	int m_bInitialized;
};
