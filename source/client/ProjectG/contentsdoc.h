#pragma once

#include <map>
#include <string>
#include <vector>
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

class CWorldTourEvent : public IContentsDataContainer
{
public:
	void SetMapFlag(unsigned long flag);
	void SetGotGift(bool bGot);
	void SetCanGift(bool bCan);
	unsigned long GetMapFlag() const;
	bool GetGotGift() const;
	bool GetCanGift() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	unsigned long m_mapFlag;
	bool m_bGotGift;
	bool m_bCanGift;
};

class CChristmasEvent : public IContentsDataContainer
{
public:
	void SetFlag(bool bFlag);
	bool GetFlag() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	bool m_bFlag;
};

class CTicketExchange : public IContentsDataContainer
{
public:
	void SetFlag(bool bFlag);
	bool GetFlag() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	bool m_bFlag;
};

class CHalloweenEvent : public IContentsDataContainer
{
public:
	void SetMapFlag(unsigned long flag);
	void SetGiftCnt(unsigned long cnt);
	void SetShowGiftDlg(bool bShow);
	void SetEventState(bool bState);
	unsigned long GetMapFlag() const;
	unsigned long GetGiftCnt() const;
	bool GetShowGiftDlg() const;
	bool GetEventState() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	unsigned long m_mapFlag;
	unsigned long m_giftCnt;
	bool m_bShowGiftDlg;
	bool m_bEventState;
};

class CGimmickContainer : public IContentsDataContainer
{
public:
	virtual bool Initialize();
	virtual bool Release();

	bool LoadFromScript(const char* filename);
	std::vector<GimmickDispositionInformation>& GetGimmickPosition(
		unsigned char hole);
	void ResetGimmikFlag(unsigned char hole,
		GimmickDispositionInformation::eFlag flag);
	GimmickDispositionInformation* FindDisposeInfo(unsigned char hole,
		GIMMICK_ITEM_TYPE type, GimmickDispositionInformation::eFlag flag);
	int GetNumDisposeInfo(unsigned char hole, GIMMICK_ITEM_TYPE type,
		GimmickDispositionInformation::eFlag flag);
	bool NeedToMakeTriPtArray(const char* texName,
		FieldItem::eDisposeTextureType* pType) const;
	void AddItemAcquired(const FieldItem::AcquiredItemInfo& info);

	int GetRandomSeed() { return m_randomSeed; }
	float GetMinDistBtwItems() const { return m_minDistBtwItems; }
	std::vector<FieldItem::AcquiredItemInfo>& GetAcquiredItemInfoArray()
	{
		return m_acquiredItemInfoArray;
	}

protected:
	std::map<unsigned char, std::vector<GimmickDispositionInformation> >
		m_gimmickPositionMap;
	int m_randomSeed;
	float m_minDistBtwItems;
	std::vector<FieldItem::AcquiredItemInfo> m_acquiredItemInfoArray;

public:
	std::string m_carpetSeqName;
	std::string m_crashSeqName[3];
};

class CMissionEvent : public IContentsDataContainer
{
public:
	virtual bool Initialize();
	virtual bool Release();
	bool IsCompleteMission(int index);
	bool IsCompleteAllDayMission();
	bool IsCompleteAllTermMission();
	bool IsCompleteCourse(int course);
	bool GetDayGiftFlag() const { return m_bDayGiftFlag; }
	bool GetTermGiftFlag() const { return m_bTermGiftFlag; }
	int GetCondition(int index) { return m_missionInfo[index].condition; }
	const int GetMapCount() const { return 15; }

private:
	unsigned int m_eventFlag;
	bool m_bDayGiftFlag;
	bool m_bTermGiftFlag;
	std::vector<GlobalEnum::sMissionInfo> m_missionInfo;
};
