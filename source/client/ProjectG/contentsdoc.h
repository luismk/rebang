#pragma once

#include <list>
#include <map>
#include <string>
#include <vector>
#include "../../shared/localize.h"
#include "icontentsdoc.h"

class CGiftBoxEvent : public IContentsDataContainer
{
public:
	void SetOpenedBoxTID(unsigned long tid);
	unsigned long GetOpenedBoxTID();

	virtual bool Initialize();
	virtual bool Release();

protected:
	unsigned long m_openedBoxTID;
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

class CEscapeBeginnerEvent : public IContentsDataContainer
{
public:
	void SetFlag(bool bFlag);
	bool GetFlag() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	bool m_bFlag;
};

class CPassingTitleChk : public IContentsDataContainer
{
public:
	void SetFlag(bool bFlag);
	bool GetFlag() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	bool m_bFlag;
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

class CMissionEvent : public IContentsDataContainer
{
public:
	virtual bool Initialize();
	virtual bool Release();

	void SetEventFlag(unsigned int flag);
	unsigned int GetEventFlag() const;
	void DecodeMissionInfo(WReceivedPacket& packet);

	bool IsCompleteMission(int index);
	bool IsCompleteAllDayMission();
	bool IsCompleteAllTermMission();
	bool IsCompleteCourse(int course);

	void SetDayGiftFlag(bool bFlag) { m_bDayGiftFlag = bFlag; }
	bool GetDayGiftFlag() const { return m_bDayGiftFlag; }
	void SetTermGiftFlag(bool bFlag) { m_bTermGiftFlag = bFlag; }
	bool GetTermGiftFlag() const { return m_bTermGiftFlag; }

	int GetCondition(int index) { return m_missionInfo[index].condition; }
	const int GetMapCount() const { return 15; }

private:
	int GetMissionField(int index) const;
	void ClearMissionInfo() { m_missionInfo.clear(); }

	unsigned int m_eventFlag;
	bool m_bDayGiftFlag;
	bool m_bTermGiftFlag;
	std::vector<GlobalEnum::sMissionInfo> m_missionInfo;
};

class CPeriodContents : public IContentsDataContainer
{
public:
	struct sPeriodUnit
	{
		_SYSTEMTIME startTime;
		_SYSTEMTIME endTime;
		bool bNoEndTime;
		bool bUsable;
	};

	virtual bool Initialize();
	virtual bool Release();

	void AddUsableContents(int type, const _SYSTEMTIME* pStart,
		const _SYSTEMTIME* pEnd, bool bNoEndTime);
	bool IsUsableContents(int type);
	void Set_PreSettingContentsDate();
	void CheckAllContents();
	bool CheckContent(int type);

private:
	void IsUsable(sPeriodUnit& unit);

protected:
	std::map<int, sPeriodUnit> m_contentsMap;
};

namespace _guild
{
	void ExchangeQuotation(char* str);

	struct sExchangerQuota
	{
		void operator()(GUILD_USER_LIST& info);
	};

	class CGuildInfo : public IContentsDataContainer
	{
	public:
		virtual bool Initialize();
		virtual bool Release();

		void ClearAllData();
		void ClearHistoryList();
		void ClearMemberPage();
		void ClearMemberList(int page);
		void SetMemberPage(int page, std::list<GUILD_USER_LIST>* pList);
		void ClearSearchPage();
		void ClearSearchList(int page);
		void SetSearchList(int page, std::list<GUILD_LIST>* pList);

		void SetMyGuildInfo(GUILD_INFO& info);
		void SetMyGuildUserInfo(GUILD_USER_INFO& info);
		void SetHistoryList(std::list<GUILD_HISTORY>* pList);
		void SetGuildEmblem(GUILD_USER_INFO* pInfo);
		void SetGuildName(GUILD_INFO* pInfo);
		void SetGuildIntroduce(const std::string& introduce);
		void SetGuildNotice(const std::string& notice);
		const GUILD_INFO& GetMyGuildInfo() const;
		const char* GuildMsg(int index);

		bool IsGuildMember() const;
		bool IsGuildMember(GUILD_CLASS_IDX classIdx);
		bool IsManager() const;
		bool IsManager(GUILD_CLASS_IDX classIdx);

		bool IsWaitingJoin() const
		{
			return (m_myGuildInfo.classIdx == (GUILD_CLASS_IDX)9) ? true
																  : false;
		}

		const GUILD_CLASS_IDX GetClassIndex() const
		{
			return m_myGuildInfo.classIdx;
		}
		const std::list<GUILD_HISTORY>& GetHistoryList() const
		{
			return m_historyList;
		}

		int GetGuildNumber() const { return m_myGuildInfo.guildUID; }

		int GetAllMemberCount() { return m_allMemberCount; }
		void SetAllMemberCount(int count) { m_allMemberCount = count; }

		int GetAllGuildCount() { return m_allGuildCount; }
		void SetAllGuildCount(int count) { m_allGuildCount = count; }

		int GetMaxMemberPage();
		int GetMaxSearchPage();
		const std::list<GUILD_USER_LIST>* GetMemberList(int page);
		const std::list<GUILD_LIST>* GetSearchList(int page);

	protected:
		GUILD_INFO m_myGuildInfo;
		std::list<GUILD_HISTORY> m_historyList;
		std::map<int, const char*> m_guildMsgMap;
		std::map<int, std::list<GUILD_USER_LIST> > m_memberPageMap;
		std::map<int, std::list<GUILD_LIST> > m_searchPageMap;
		int m_allMemberCount;
		int m_allGuildCount;
	};
}

class CChristmasSockEvent : public IContentsDataContainer
{
public:
	void SetFlag(bool bFlag);
	bool GetFlag() const;

	virtual bool Initialize();
	virtual bool Release();

protected:
	bool m_bFlag;
};

class CVectorSlideTestContainer : public IContentsDataContainer
{
public:
	virtual bool Initialize();
	virtual bool Release();

	void SetNewTypeVectorSlide(bool bNewType)
	{
		m_bNewTypeVectorSlide = bNewType;
	}
	void SetLimitSpeed(int speed) { m_limitSpeed = speed; }
	void SetBoundHeight(int height) { m_boundHeight = height; }
	void SetLimitVectorSlideCount(int count)
	{
		m_limitVectorSlideCount = count;
	}

	bool IsNewTypeVectorSlide() { return m_bNewTypeVectorSlide; }
	int GetLimitSpeed() { return m_limitSpeed; }
	int GetBoundHeight() { return m_boundHeight; }
	int GetLimitVectorSlideCount() { return m_limitVectorSlideCount; }

protected:
	bool m_bNewTypeVectorSlide;
	int m_limitSpeed;
	int m_boundHeight;
	int m_limitVectorSlideCount;
};

class CGimmickContainer : public IContentsDataContainer
{
public:
	virtual bool Initialize();
	virtual bool Release();

	bool LoadFromScript(const char* filename);
	void LoadFromPacket(WReceivedPacket& packet);
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
	void AddGimmickPosition(unsigned char hole,
		const std::vector<GimmickDispositionInformation>& positions);

	std::map<unsigned char, std::vector<GimmickDispositionInformation> >
		m_gimmickPositionMap;
	int m_randomSeed;
	float m_minDistBtwItems;
	std::vector<FieldItem::AcquiredItemInfo> m_acquiredItemInfoArray;

public:
	std::string m_carpetSeqName;
	std::string m_crashSeqName[3];
};
