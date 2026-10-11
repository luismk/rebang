// Original: d:\build\custom\temp\code\client\projectg\netresourcemanager.h
#ifndef PG_CLIENT_PROJECTG_NETRESOURCEMANAGER_H
#define PG_CLIENT_PROJECTG_NETRESOURCEMANAGER_H

#include <windows.h>

#include <list>
#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"
#include "RSSManager.h"

class Bitmap;

// Kind of a network resource; it also indexes the per-type upload address
// table (NetResourceManager::GetUploadAddress). Values from the code that
// fills sResource::type; the enumerator names are not in the PDB.
enum eResourceType {
	RESOURCE_TYPE_UCC_CLOTHES = 0,      // DownloadUccClothes
	RESOURCE_TYPE_1 = 1,
	RESOURCE_TYPE_EMBLEM = 2,           // GetEmblemByName
	RESOURCE_TYPE_3 = 3,
	RESOURCE_TYPE_UCC_TEMP_CLOTHES = 4, // DownloadUccTempClothes, UploadUccTempClothes
	RESOURCE_TYPE_5 = 5,
	RESOURCE_TYPE_COUNT = 6             // Upload rejects types >= this
};

// One resource to download or upload. Layout from the constructor
// (netresourcemanager.h:57) and the implicit copy constructor.
struct sResource {
	// netresourcemanager.h:57
	sResource(void)
	{
		id = 0;
		slot = 0;
		memset(data, 0, sizeof(data));
		memset(key, 0, sizeof(key));
	}

	eResourceType type;     // 0x0
	std::string path;       // 0x4
	unsigned long id;       // 0x20 (type id)
	char key[9];            // 0x24 (UCC key)
	unsigned short slot;    // 0x2e
	char data[64];          // 0x30
};
CHECK_SIZE(sResource, 0x70);
BEGIN_LAYOUT_CHECK(sResource)
	CHECK_FIELD(sResource, type, 0x0);
	CHECK_FIELD(sResource, path, 0x4);
	CHECK_FIELD(sResource, id, 0x20);
	CHECK_FIELD(sResource, key, 0x24);
	CHECK_FIELD(sResource, slot, 0x2e);
	CHECK_FIELD(sResource, data, 0x30);
END_LAYOUT_CHECK

// Upload result of one resource (SetUploadStatus, IsFailedToUpload).
// Layout from the constructor (netresourcemanager.h:73).
struct sUploadStatus {
	// netresourcemanager.h:73
	sUploadStatus(void)
	{
		id = 0;
		type = (eResourceType)0;
		failCount = 0;
		memset(key, 0, sizeof(key));
	}

	unsigned long id;           // 0x0
	char key[9];                // 0x4
	eResourceType type;         // 0x10
	unsigned char failCount;    // 0x14 (SetUploadStatus increments it)
};
CHECK_SIZE(sUploadStatus, 0x18);
BEGIN_LAYOUT_CHECK(sUploadStatus)
	CHECK_FIELD(sUploadStatus, id, 0x0);
	CHECK_FIELD(sUploadStatus, key, 0x4);
	CHECK_FIELD(sUploadStatus, type, 0x10);
	CHECK_FIELD(sUploadStatus, failCount, 0x14);
END_LAYOUT_CHECK

// Where one resource type is downloaded from and saved to (Init, Download).
struct sDownloadInfo {
	char directory[0x400];  // 0x0 (local save directory, "emblem", "./ntsaved/")
	char address[0x400];    // 0x400 (download URL)
	int option;             // 0x800 (0x20 for emblems, 0 after Download)
};
CHECK_SIZE(sDownloadInfo, 0x804);
BEGIN_LAYOUT_CHECK(sDownloadInfo)
	CHECK_FIELD(sDownloadInfo, directory, 0x0);
	CHECK_FIELD(sDownloadInfo, address, 0x400);
	CHECK_FIELD(sDownloadInfo, option, 0x800);
END_LAYOUT_CHECK

unsigned __stdcall CreateDownloadThreadFunc(void *param);
unsigned __stdcall CreateUploadThreadFunc(void *param);

// The network resource (UCC texture, guild emblem) downloader and uploader
// singleton. Layout from the constructor, the destructor and Init
// (netresourcemanager.cpp:78-135).
class NetResourceManager : public WSingleton<NetResourceManager> {
	LAYOUT_CHECK_FRIEND;

public:
	// A name/value pair of an HTTP POST (implicit constructor/destructor).
	struct sPostArgument {
		std::string name;   // 0x0
		std::string value;  // 0x1c
		bool binary;        // 0x38 (AddPostArguments' bBinary in WorkUploadThread)
	};

	NetResourceManager(void);
	virtual ~NetResourceManager(void);

	void Init(void);
	void Close(void);

	void RequestGuildRSS(unsigned long guildIdx, void (__fastcall *callBack)(int));
	void RequestGuildRSS_JP(unsigned long guildIdx, void (__fastcall *callBack)(int), const char *param1, const char *param2);

	// netresourcemanager.h:131
	bool IsRSSDownLoadComplete(void)
	{
		return m_rssManager.IsComplete();
	}

	// netresourcemanager.h:137
	void ResetGuildRSS(void)
	{
		m_rssManager.ResetVars();
	}

	// netresourcemanager.h:142
	void SetClearGarbage(void)
	{
		m_bClearGarbage = true;
	}

	// netresourcemanager.h:143
	char *GetUploadAddress(eResourceType type)
	{
		return m_uploadAddress[type];
	}

	bool IsDownloadRequested(eResourceType type, unsigned long id, const char *key);
	bool IsFailedToUpload(unsigned long id, const char *key, eResourceType type);
	void UploadUccClothes(unsigned long typeId, const char *uccKey, const char *data);
	void UploadUccTempClothes(unsigned long typeId, const char *uccKey);
	bool SetUploadStatus(unsigned long id, const char *key, eResourceType type);
	bool Upload(const sResource &res, const char *address);
	const Bitmap *GetEmblemByName(const char *name);
	void DownloadUccClothes(unsigned long typeId, const char *uccKey, unsigned short slot, bool force);
	void DownloadUccTempClothes(unsigned long typeId, const char *uccKey, unsigned short slot, bool force);
	void Download(const sResource &res, const char *address);
	void UploadBugReportData(const char *fileName, int mainType, int subType, bool attach);

protected:
	friend unsigned __stdcall CreateDownloadThreadFunc(void *param);
	friend unsigned __stdcall CreateUploadThreadFunc(void *param);

	bool IsDownloadRequested(const sResource &res);
	bool IsUploadRequested(const sResource &res);
	bool _RealDownLoad(const sResource &res);
	void AddToUploadList(const sResource &res);
	bool LoadBitmap(const sResource &res);
	const Bitmap *AddToDownloadList(const sResource &res, int request, bool first);
	void ClearGarbage(void);
	int WorkDownloadThread(void);
	int WorkUploadThread(void);

	RSSManager m_rssManager;                    // 0x28
	UNKNOWN(0x70, 0x74);
	std::list<sResource> m_downloadList;        // 0x74
	std::list<sResource> m_uploadList;          // 0x80
	HANDLE m_hDownloadThread;                   // 0x8c
	HANDLE m_hUploadThread;                     // 0x90
	CRITICAL_SECTION m_downloadLock;            // 0x94
	CRITICAL_SECTION m_uploadLock;              // 0xac
	bool m_bRunning;                            // 0xc4 (Init sets it)
	char m_localDirectory[0x200];               // 0xc5 (g_executeDirectory + "\\...")
	std::map<std::string, Bitmap *> m_bitmaps;  // 0x2c8
	sDownloadInfo m_downloadInfo[RESOURCE_TYPE_COUNT];  // 0x2d4
	char m_uploadAddress[RESOURCE_TYPE_COUNT][0x400];   // 0x32ec
	std::list<sUploadStatus> m_uploadStatusList;        // 0x4aec
	bool m_bClearGarbage;                               // 0x4af8 (SetClearGarbage)
	std::map<std::string, std::list<sPostArgument> > m_postArguments;  // 0x4afc
};
CHECK_SIZE(NetResourceManager, 0x4b08);
BEGIN_LAYOUT_CHECK(NetResourceManager)
	CHECK_FIELD(NetResourceManager, m_rssManager, 0x28);
	CHECK_FIELD(NetResourceManager, m_downloadList, 0x74);
	CHECK_FIELD(NetResourceManager, m_uploadList, 0x80);
	CHECK_FIELD(NetResourceManager, m_hDownloadThread, 0x8c);
	CHECK_FIELD(NetResourceManager, m_hUploadThread, 0x90);
	CHECK_FIELD(NetResourceManager, m_downloadLock, 0x94);
	CHECK_FIELD(NetResourceManager, m_uploadLock, 0xac);
	CHECK_FIELD(NetResourceManager, m_bRunning, 0xc4);
	CHECK_FIELD(NetResourceManager, m_localDirectory, 0xc5);
	CHECK_FIELD(NetResourceManager, m_bitmaps, 0x2c8);
	CHECK_FIELD(NetResourceManager, m_downloadInfo, 0x2d4);
	CHECK_FIELD(NetResourceManager, m_uploadAddress, 0x32ec);
	CHECK_FIELD(NetResourceManager, m_uploadStatusList, 0x4aec);
	CHECK_FIELD(NetResourceManager, m_bClearGarbage, 0x4af8);
	CHECK_FIELD(NetResourceManager, m_postArguments, 0x4afc);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_NETRESOURCEMANAGER_H
