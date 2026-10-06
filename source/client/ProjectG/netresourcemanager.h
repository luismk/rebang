#pragma once

#include <string>
#include <list>
#include <map>
#include "rssmanager.h"

enum eResourceType
{
	RESOURCE_UCC_CLOTHES,
	RESOURCE_UCC_AZTEC,
	RESOURCE_GUILD_EMBLEM,
	RESOURCE_GHOST,
	RESOURCE_UCC_TEMP_CLOTHES,
	RESOURCE_BUG_REPORT,
	RESOURCE_NUM
};

struct sResource
{
	eResourceType type;
	std::string filename;
	unsigned long id;
	char key[9];
	unsigned short flag;
	char arg[64];

	sResource()
	{
		id = 0;
		flag = 0;
		memset(arg, 0, sizeof(arg));
		memset(key, 0, sizeof(key));
	}
};

struct sUploadStatus
{
	unsigned long id;
	char key[9];
	eResourceType type;
	unsigned char count;

	sUploadStatus()
	{
		id = 0;
		type = RESOURCE_UCC_CLOTHES;
		count = 0;
		memset(key, 0, sizeof(key));
	}
};

class NetResourceManager : public WSingleton<NetResourceManager>
{
	friend unsigned int __stdcall CreateDownloadThreadFunc(void* param);
	friend unsigned int __stdcall CreateUploadThreadFunc(void* param);

public:
	struct sPostArgument
	{
		std::string name;
		std::string value;
		bool bBinary;
	};

	NetResourceManager();
	virtual ~NetResourceManager();

	void Init();
	void Close();

	const Bitmap* GetEmblemByName(const char* name);
	void DownloadUccClothes(unsigned long id, const char* key,
		unsigned short flag, bool bFront);
	void DownloadUccTempClothes(unsigned long id, const char* key,
		unsigned short flag, bool bFront);
	void Download(const sResource& res, const char* url);
	bool IsDownloadRequested(eResourceType type, unsigned long id,
		const char* key);
	void UploadUccClothes(unsigned long id, const char* key, const char* arg);
	void UploadUccTempClothes(unsigned long id, const char* key);
	bool SetUploadStatus(unsigned long id, const char* key, eResourceType type);
	bool IsFailedToUpload(unsigned long id, const char* key,
		eResourceType type);
	bool Upload(const sResource& res, const char* url);
	void RequestGuildRSS(unsigned long guildId,
		void(__fastcall* callback)(int));
	void RequestGuildRSS_JP(unsigned long guildId,
		void(__fastcall* callback)(int), const char* arg1, const char* arg2);
	void UploadBugReportData(const char* filename, int mainType, int subType,
		bool bAttachFile);

	bool IsRSSDownLoadComplete() { return m_rss.IsComplete(); }

	void ResetGuildRSS() { m_rss.ResetVars(); }

	void SetClearGarbage() { m_bClearGarbage = true; }
	char* GetUploadAddress(eResourceType type) { return m_uploadAddress[type]; }

protected:
	int WorkDownloadThread();
	int WorkUploadThread();
	bool _RealDownLoad(const sResource& res);
	bool LoadBitmapA(const sResource& res);
	const Bitmap* AddToDownloadList(const sResource& res, int bLoad,
		bool bFront);
	void AddToUploadList(const sResource& res);
	bool IsDownloadRequested(const sResource& res);
	bool IsUploadRequested(const sResource& res);
	void ClearGarbage();

private:
	struct sDownloadInfo
	{
		char dir[1024];
		char url[1024];
		int reserved;
	};

	RSSManager m_rss;
	std::list<sResource> m_downloadList;
	std::list<sResource> m_uploadList;
	void* m_hDownloadThread;
	void* m_hUploadThread;
	_RTL_CRITICAL_SECTION m_csDownload;
	_RTL_CRITICAL_SECTION m_csUpload;
	bool m_bRunning;
	char m_rootDir[512];
	std::map<std::string, Bitmap*> m_emblem;
	sDownloadInfo m_download[RESOURCE_NUM];
	char m_uploadAddress[RESOURCE_NUM][1024];
	std::list<sUploadStatus> m_uploadStatus;
	bool m_bClearGarbage;
	std::map<std::string, std::list<sPostArgument> > m_postArgument;
};

inline NetResourceManager* NetResManager()
{
	return NetResourceManager::Instance();
}
