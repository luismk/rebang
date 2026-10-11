// Original: d:\build\custom\temp\code\client\projectg\netresourcemanager.cpp
#include "NetResourceManager.h"

#include <process.h>
#include <stdio.h>
#include <string.h>

#include "UccLibrary.h"
#include "SharedDoc.h"
#include "UccManager.h"
#include "bitmap.h"
#include "cHttp.h"
#include "commonutil.h"
#include "wresrcmng.h"
#include "GenericHTTPClient.h"
#include "Packet.h"
#include "SecurityKeyManager.h"
#include "TaskManager.h"
#include "actor.h"

extern char g_executeDirectory[];

static const char *s_guildActorName = "GuildActor";

// Queues a resource for download unless it is already queued; emblems go
// through AddToDownloadList instead. Expanded on one source line.
#define ADD_TO_DOWNLOAD_LIST(res) \
	EnterCriticalSection(&m_downloadLock); \
	switch ((res).type) { \
	case RESOURCE_TYPE_UCC_CLOTHES: \
	case RESOURCE_TYPE_1: \
	case RESOURCE_TYPE_UCC_TEMP_CLOTHES: \
		if (!IsDownloadRequested(res)) { \
			m_downloadList.push_back(res); \
		} \
		break; \
	case RESOURCE_TYPE_EMBLEM: \
		break; \
	default: \
		m_downloadList.push_back(res); \
		break; \
	} \
	LeaveCriticalSection(&m_downloadLock)

// netresourcemanager.cpp:28
void G_CreateDirectory(const char *path)
{
	std::string remain(path);

	if (remain.length() > 0) {
		std::string token = remain.substr(0, remain.find("\\"));
		remain = remain.substr(remain.find("\\") + 1);

		std::string directory;

		for (;;) {
			directory += token;
			directory += "\\";
			CreateDirectory(directory.c_str(), NULL);

			size_t pos = remain.find("\\");
			if (pos == std::string::npos) {
				directory += remain;
				CreateDirectory(directory.c_str(), NULL);
				break;
			}

			token = remain.substr(0, pos);
			remain = remain.substr(pos + 1);
		}
	}
}

// netresourcemanager.cpp:59
unsigned __stdcall CreateDownloadThreadFunc(void *param)
{
	NetResourceManager *manager = static_cast<NetResourceManager *>(param);

	manager->WorkDownloadThread();

	return 0;
}

// netresourcemanager.cpp:69
unsigned __stdcall CreateUploadThreadFunc(void *param)
{
	NetResourceManager *manager = static_cast<NetResourceManager *>(param);

	manager->WorkUploadThread();

	return 0;
}

// netresourcemanager.cpp:78
NetResourceManager::NetResourceManager(void)
{
	Init();
}

// netresourcemanager.cpp:83
NetResourceManager::~NetResourceManager(void)
{
	Close();
}

// netresourcemanager.cpp:88
void NetResourceManager::Init(void)
{
	strcpy(m_downloadInfo[RESOURCE_TYPE_EMBLEM].directory, "emblem");
	strcpy(m_downloadInfo[RESOURCE_TYPE_EMBLEM].address, "http://qa.contents.pangya.gametree.co.kr:50006/_Files/GuildMark/");

	strcpy(m_uploadAddress[RESOURCE_TYPE_EMBLEM], "http://qa.contents.pangya.gametree.co.kr:50006/Guild/upload.asp");

	m_downloadInfo[RESOURCE_TYPE_EMBLEM].option = 0x20;

	strcpy(m_downloadInfo[RESOURCE_TYPE_UCC_CLOTHES].directory, "");
	strcpy(m_downloadInfo[RESOURCE_TYPE_UCC_CLOTHES].address, "http://qa.contents.pangya.gametree.co.kr:50006/UCC/UCC_ONE/clothes/");
	strcpy(m_uploadAddress[RESOURCE_TYPE_UCC_CLOTHES], "http://qa.contents.pangya.gametree.co.kr:50006/UCC/upload_one.asp");

	strcpy(m_downloadInfo[RESOURCE_TYPE_UCC_TEMP_CLOTHES].address, m_downloadInfo[RESOURCE_TYPE_UCC_CLOTHES].address);
	strcpy(m_uploadAddress[RESOURCE_TYPE_UCC_TEMP_CLOTHES], m_uploadAddress[RESOURCE_TYPE_UCC_CLOTHES]);

	strcpy(m_uploadAddress[RESOURCE_TYPE_5], "http://qagame.pangya.ntreev.com:50000/post/BugReport.asp");

	m_downloadList.clear();

	m_bRunning = true;
	m_bClearGarbage = false;

	InitializeCriticalSection(&m_downloadLock);
	InitializeCriticalSection(&m_uploadLock);

	unsigned downloadThreadId;
	unsigned uploadThreadId;
	m_hDownloadThread = (HANDLE)_beginthreadex(NULL, 0, CreateDownloadThreadFunc, this, 0, &downloadThreadId);
	m_hUploadThread = (HANDLE)_beginthreadex(NULL, 0, CreateUploadThreadFunc, this, 0, &uploadThreadId);

	if (m_hDownloadThread == INVALID_HANDLE_VALUE) {
		return;
	}

	if (m_hUploadThread == INVALID_HANDLE_VALUE) {
		return;
	}

	memset(m_localDirectory, 0, sizeof(m_localDirectory));
	strcpy(m_localDirectory, g_executeDirectory);
	strcat(m_localDirectory, "\\");

	char path[0x400];
	memset(path, 0, sizeof(path));
	sprintf(path, "%s\\%s", m_localDirectory, m_downloadInfo[RESOURCE_TYPE_EMBLEM].directory);
	G_CreateDirectory(path);
}

// netresourcemanager.cpp:141
void NetResourceManager::Close(void)
{
	m_bRunning = false;
	WaitForSingleObject(m_hDownloadThread, 3000);
	CloseHandle(m_hDownloadThread);

	WaitForSingleObject(m_hUploadThread, 3000);
	CloseHandle(m_hUploadThread);

	EnterCriticalSection(&m_downloadLock);
	m_downloadList.clear();
	LeaveCriticalSection(&m_downloadLock);

	EnterCriticalSection(&m_uploadLock);
	m_uploadList.clear();
	LeaveCriticalSection(&m_uploadLock);

	std::map<std::string, Bitmap *>::iterator it;
	for (it = m_bitmaps.begin(); it != m_bitmaps.end(); ++it) {
		if (it->second) {
			delete it->second;
		}
	}

	m_bitmaps.clear();

	DeleteCriticalSection(&m_downloadLock);
	DeleteCriticalSection(&m_uploadLock);
}

// netresourcemanager.cpp:175
int NetResourceManager::WorkDownloadThread(void)
{
	while (m_bRunning) {
		if (m_bClearGarbage) {
			m_bClearGarbage = false;
			ClearGarbage();
		}

		EnterCriticalSection(&m_downloadLock);

		if (m_downloadList.size() == 0) {
			LeaveCriticalSection(&m_downloadLock);
			Sleep(1000);
			continue;
		}

		sResource res = m_downloadList.front();
		m_downloadList.pop_front();

		LeaveCriticalSection(&m_downloadLock);

		if (res.type == RESOURCE_TYPE_EMBLEM) {
			char path[0x100];
			WIN32_FIND_DATA findData;

			sprintf(path, "%s%s\\%s", m_localDirectory, m_downloadInfo[res.type].directory, res.path.c_str());

			HANDLE hFind = FindFirstFile(path, &findData);

			if (hFind != INVALID_HANDLE_VALUE) {
				FindClose(hFind);
				LoadBitmap(res);
				continue;
			}
		}

		if (_RealDownLoad(res)) {
			switch (res.type) {
			case RESOURCE_TYPE_EMBLEM:
				LoadBitmap(res);
				break;

			case RESOURCE_TYPE_UCC_CLOTHES:
			case RESOURCE_TYPE_1:
				if (UccManager()->SetDownloadComplete(res.id, res.key)) {
					UccManager()->AddRefreshItem(res.id, res.key);
				}
				break;
			}
		}
	}

	return 0;
}

// netresourcemanager.cpp:267
int NetResourceManager::WorkUploadThread(void)
{
	while (m_bRunning) {
		EnterCriticalSection(&m_uploadLock);

		if (m_uploadList.size() == 0) {
			LeaveCriticalSection(&m_uploadLock);
			Sleep(1000);
			continue;
		}

		sResource res = m_uploadList.front();
		m_uploadList.pop_front();

		LeaveCriticalSection(&m_uploadLock);

		GenericHTTPClient http;

		std::string fileName;
		if (res.type != RESOURCE_TYPE_EMBLEM) {
			char currentDirectory[0x400] = { 0 };
			GetCurrentDirectory(sizeof(currentDirectory), currentDirectory);
			fileName = currentDirectory;
			fileName += "\\";
			fileName += res.path;
		} else {
			fileName = res.path;
		}

		http.InitilizePostArguments();

		if (res.type == RESOURCE_TYPE_UCC_CLOTHES || res.type == RESOURCE_TYPE_UCC_TEMP_CLOTHES) {
			sItemInfo *info = GetMyItemInfo(res.id, res.key, 0);
			if (info == NULL) {
				if (AfxGetTask()->GetActor("RealMyRoom")) {
					AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x23, (intptr_t)MakeStr("���ε尡 �����߽��ϴ�. �ٽ� �õ����ּ���.(�ڵ�:%d)", 6), 0, 0, 0, 0);
				}
				continue;
			}

			std::string key = SecurityKeyManager()->GetKey(SK_CONTENT_UCC, info->m_serial);
			http.AddPostArguments("ucctype", "clothes", FALSE);
			http.AddPostArguments("uccfile", fileName.c_str(), TRUE);
			http.AddPostArguments("userid", WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id, FALSE);
			http.AddPostArguments("type", 1);
			http.AddPostArguments("item_id", info->m_serial);
			http.AddPostArguments("key", key.c_str(), FALSE);
			http.AddPostArguments("uid", MyUID());
		} else if (res.type == RESOURCE_TYPE_EMBLEM) {
			IActor *pActor = AfxGetTask()->GetActor(s_guildActorName);
			if (pActor) {
				unsigned long *guildIdx = NULL;
				pActor << MsgObject(0, 0x263, 0x21, (intptr_t)&guildIdx, 0, 0, 0);

				http.AddPostArguments("EMBLEM_IDX", res.id);
				http.AddPostArguments("GUILD_IDX", *guildIdx);
				http.AddPostArguments("UID", MyUID());
				http.AddPostArguments("EMBLEM", res.data, FALSE);
				http.AddPostArguments("FILENAME", fileName.c_str(), TRUE);
			}
		} else if (res.type == RESOURCE_TYPE_5) {
			if (m_postArguments.find(res.data) != m_postArguments.end()) {
				std::list<sPostArgument> &arguments = m_postArguments[res.data];
				std::list<sPostArgument>::iterator it;
				for (it = arguments.begin(); it != arguments.end(); ++it) {
					http.AddPostArguments(it->name.c_str(), it->value.c_str(), it->binary);
				}

				m_postArguments.erase(res.data);
			}
		} else {
			http.AddPostArguments("arg1", res.data, FALSE);
			http.AddPostArguments("arg2", fileName.c_str(), TRUE);
		}

		BOOL requested = http.Request(Instance()->GetUploadAddress(res.type), 3, "MERONG(0.9/;p)");
		const char *response = http.QueryHTTPResponse();

		if (requested && strstr(response, "PANGYA_UPDATE_OK") == response) {
			if ((res.type >= RESOURCE_TYPE_UCC_CLOTHES && res.type <= RESOURCE_TYPE_EMBLEM) || res.type == RESOURCE_TYPE_UCC_TEMP_CLOTHES) {
				if (SetUploadStatus(res.id, res.key, res.type)) {
					EnterCriticalSection(&m_uploadLock);

					std::list<sUploadStatus>::iterator it;
					for (it = m_uploadStatusList.begin(); it != m_uploadStatusList.end(); ++it) {
						if (it->type == res.type && it->id == res.id && strcmp(it->key, res.key) == 0) {
							break;
						}
					}

					if (it != m_uploadStatusList.end()) {
						m_uploadStatusList.erase(it);
					}

					LeaveCriticalSection(&m_uploadLock);

					switch (res.type) {
					case RESOURCE_TYPE_UCC_CLOTHES:
						{
							WSendPacket packet(0xB1);
							packet.Encode1(0);
							packet.Encode4(res.id);
							packet.EncodeStr(std::string(res.key));
							packet.EncodeStr(std::string(res.data));
							packet.Send(SEND_TO_DEFAULT);
						}
						break;

					case RESOURCE_TYPE_EMBLEM:
						{
							IActor *pActor = AfxGetTask()->GetActor(s_guildActorName);
							if (pActor) {
								pActor << MsgObject(0, 0x263, 0x18, 0, 0, 0, 0);
							}

							WSendPacket packet(0x113);
							packet.Send(SEND_TO_DEFAULT);
							AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
						}
						break;

					case RESOURCE_TYPE_UCC_TEMP_CLOTHES:
						{
							WSendPacket packet(0xB1);
							packet.Encode1(3);
							packet.Encode4(res.id);
							packet.EncodeStr(std::string(res.key));
							packet.Send(SEND_TO_DEFAULT);
						}
						break;
					}
				}
			}
		} else {
			switch (res.type) {
			case RESOURCE_TYPE_UCC_CLOTHES:
			case RESOURCE_TYPE_1:
			case RESOURCE_TYPE_UCC_TEMP_CLOTHES:
				{
					char message[0x400] = { 0 };
					strncpy(message, response, sizeof(message) - 1);
					if (AfxGetTask()->GetActor("RealMyRoom")) {
						AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x213, (intptr_t)&res.id, (intptr_t)res.key, (intptr_t)message, 0, 0);

						EnterCriticalSection(&m_uploadLock);

						std::list<sResource>::iterator it;
						for (it = m_uploadList.begin(); it != m_uploadList.end(); ) {
							if (res.id == it->id && strcmp(res.key, it->key) == 0) {
								DeleteFile(it->path.c_str());
								it = m_uploadList.erase(it);
								continue;
							}

							++it;
						}

						LeaveCriticalSection(&m_uploadLock);
					}
				}
				break;

			case RESOURCE_TYPE_EMBLEM:
				{
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
					IActor *pActor = AfxGetTask()->GetActor(s_guildActorName);
					if (pActor) {
						if (strstr(response, "PANGYA_UPDATE_BLOCK")) {
							pActor << MsgObject(0, 0x263, 0x1A, 0x41F, 0, 0, 0);
						} else {
							pActor << MsgObject(0, 0x263, 0x1A, 0x41E, 0, 0, 0);
						}
					}
				}
				break;
			}
		}

		DeleteFile(fileName.c_str());
	}

	return 0;
}

// netresourcemanager.cpp:564
bool NetResourceManager::_RealDownLoad(const sResource &res)
{
	bool result = false;
	cHttp http;

	switch (res.type) {
	case RESOURCE_TYPE_3:
		{
			std::string address = m_downloadInfo[RESOURCE_TYPE_3].address;
			address += res.data[0];
			address += "/";
			address += res.data;
			address += "/";

			CreateDirectory(m_downloadInfo[res.type].directory, NULL);

			unsigned long size = 0;
			unsigned char *data = http.DownLoad<unsigned char>(res.path.c_str(), address.c_str(), size);
			if (data != NULL) {
				delete[] data;
				result = true;
			}
		}
		break;

	case RESOURCE_TYPE_EMBLEM:
		{
			char path[0x100];
			memset(path, 0, sizeof(path));
			sprintf(path, "%s\\%s", m_downloadInfo[res.type].directory, res.path.c_str());

			if (http.DownLoad(path, res.path.c_str(), m_localDirectory, m_downloadInfo[res.type].address)) {
				result = true;
			}
		}
		break;

	case RESOURCE_TYPE_UCC_CLOTHES:
	case RESOURCE_TYPE_1:
		{
			std::string address = m_downloadInfo[res.type].address;
			address += res.key[0];
			address += "/";

			unsigned long size = 0;
			unsigned char *data = http.DownLoad<unsigned char>(res.path.c_str(), address.c_str(), size);

			int width = 0;
			int height = 0;
			IFF_STRUCT::sPart *part = ItemManager()->FindPart(res.id);
			if (part != NULL) {
				Bitmap *bitmap = g_resrcmng->LoadBitmap(part->texture[0], 0, false);
				if (bitmap != NULL) {
					width = bitmap->Width();
					height = bitmap->Height();
					delete bitmap;
				}
			}

			Bitmap front;
			Bitmap back;
			front.Create(width, height, 24);
			back.Create(width, height, 24);
			Bitmap *icon = new Bitmap(0x40, 0x54, 0x20);

			sUccHeader header;

			if (!UnzipUccClothes(data, size, &front, &back, icon, &header)) {
				delete icon;
				delete[] data;
				return false;
			}

			delete[] data;

			if (header.m_flags & 1) {
				LoadWarningClothes(res.id, &front, &back, icon);
			}

			std::string textureF = GenerateTextureName_F(res.id, res.key);
			std::string textureB = GenerateTextureName_B(res.id, res.key);
			g_resrcmng->UploadTexture(textureF.c_str(), &front, 0, NULL);
			g_resrcmng->UploadTexture(textureB.c_str(), &back, 0, NULL);

			std::string iconName = GenerateIconFilename(res.id, res.key);
			std::map<std::string, Bitmap *>::iterator it = UccManager()->m_iconMap.find(iconName);
			if (it == UccManager()->m_iconMap.end()) {
				bool found = false;

				if (res.type == RESOURCE_TYPE_UCC_CLOTHES) {
					sUccClothes *clothes = UccManager()->FindClothes(res.id, res.key);
					if (clothes != NULL) {
						clothes->m_pIcon = icon;
						found = true;
					}
				} else {
					sUccAztec *aztec = UccManager()->FindAztec(res.id, res.key);
					if (aztec != NULL) {
						aztec->m_pIcon = icon;
						found = true;
					}
				}

				if (!found) {
					ClearTexCache(icon);
					delete icon;
				} else {
					UccManager()->m_iconMap.insert(std::pair<const std::string, Bitmap *>(iconName, icon));
				}
			} else {
				delete icon;
			}

			result = true;
		}
		break;

	case RESOURCE_TYPE_UCC_TEMP_CLOTHES:
		{
			std::string address = m_downloadInfo[RESOURCE_TYPE_UCC_TEMP_CLOTHES].address;
			address += res.key[0];
			address += "/";

			unsigned long size = 0;
			unsigned char *data = http.DownLoad<unsigned char>(res.path.c_str(), address.c_str(), size);

			int width = 0;
			int height = 0;
			IFF_STRUCT::sPart *part = ItemManager()->FindPart(res.id);
			if (part != NULL) {
				Bitmap *bitmap = g_resrcmng->LoadBitmap(part->texture[0], 0, false);
				if (bitmap != NULL) {
					width = bitmap->Width();
					height = bitmap->Height();
					delete bitmap;
				}
			}

			Bitmap front;
			Bitmap back;
			front.Create(width, height, 32);
			back.Create(width, height, 32);

			if (!UnzipUccClothes(data, size, &front, &back, NULL, NULL)) {
				delete[] data;
				return false;
			}

			delete[] data;

			UccManager()->SetUccTempClothes(res.id, res.key, &front, &back);

			result = true;
		}
		break;
	}

	return result;
}


// netresourcemanager.cpp:822
bool NetResourceManager::LoadBitmap(const sResource &res)
{
	EnterCriticalSection(&m_downloadLock);
	if (res.type == RESOURCE_TYPE_EMBLEM) {
		char path[0x100] = { 0 };

		sprintf(path, "%s%s\\%s", m_localDirectory, m_downloadInfo[res.type].directory, res.path.c_str());
		Bitmap *bitmap = g_resrcmng->LoadBitmap(path, 0, true);

		if (bitmap == NULL) {
			DeleteFile(path);

			LeaveCriticalSection(&m_downloadLock);
			return false;
		}

		m_bitmaps[res.path] = bitmap;
	}

	LeaveCriticalSection(&m_downloadLock);
	return true;
}

// netresourcemanager.cpp:864
const Bitmap *NetResourceManager::AddToDownloadList(const sResource &res, int request, bool first)
{
	const Bitmap *bitmap = NULL;
	EnterCriticalSection(&m_downloadLock);

	switch (res.type) {
	case RESOURCE_TYPE_EMBLEM: {
		if (request == 0) {
			LeaveCriticalSection(&m_downloadLock);
			return NULL;
		}

		std::map<std::string, Bitmap *>::iterator it = m_bitmaps.find(res.path);

		if (it == m_bitmaps.end() || it->second == NULL) {
			if (!IsDownloadRequested(res)) {
				if (first) {
					m_downloadList.push_front(res);
				} else {
					m_downloadList.push_back(res);
				}
				m_bitmaps[res.path] = NULL;
			}
			bitmap = NULL;
		} else {
			bitmap = it->second;
		}
		break;
	}

	case RESOURCE_TYPE_UCC_CLOTHES:
	case RESOURCE_TYPE_1:
	case RESOURCE_TYPE_UCC_TEMP_CLOTHES:
		if (!IsDownloadRequested(res)) {
			if (first) {
				m_downloadList.push_front(res);
			} else {
				m_downloadList.push_back(res);
			}
		}
		break;

	default:
		if (first) {
			m_downloadList.insert(m_downloadList.begin(), res);
		} else {
			m_downloadList.insert(m_downloadList.end(), res);
		}
		break;
	}

	LeaveCriticalSection(&m_downloadLock);
	return bitmap;
}

// netresourcemanager.cpp:938
void NetResourceManager::AddToUploadList(const sResource &res)
{
	if (IsUploadRequested(res)) {
		return;
	}

	EnterCriticalSection(&m_uploadLock);
	m_uploadList.push_back(res);
	LeaveCriticalSection(&m_uploadLock);
}

// netresourcemanager.cpp:961
const Bitmap *NetResourceManager::GetEmblemByName(const char *name)
{
	if (strlen(name) == 0) {
		return NULL;
	}

	char emblemName[13];
	memset(emblemName, 0, sizeof(emblemName));
	strncpy(emblemName, name, 12);

	sResource res;
	res.path = MakeStr("%s.png", emblemName);
	res.type = RESOURCE_TYPE_EMBLEM;
	return AddToDownloadList(res, 1, false);
}

// netresourcemanager.cpp:982
void NetResourceManager::DownloadUccClothes(unsigned long typeId, const char *uccKey, unsigned short slot, bool force)
{
	sResource res;
	res.path = GenerateTextureName_F(typeId, uccKey);

	res.type = RESOURCE_TYPE_UCC_CLOTHES;
	res.id = typeId;
	res.slot = slot;
	strncpy(res.key, uccKey, sizeof(res.key));

	ADD_TO_DOWNLOAD_LIST(res);
}

// netresourcemanager.cpp:994
void NetResourceManager::DownloadUccTempClothes(unsigned long typeId, const char *uccKey, unsigned short slot, bool force)
{
	sResource res;
	res.path = GenerateTextureName_F(typeId, uccKey);

	res.type = RESOURCE_TYPE_UCC_TEMP_CLOTHES;
	res.id = typeId;
	res.slot = slot;
	strncpy(res.key, uccKey, sizeof(res.key));

	ADD_TO_DOWNLOAD_LIST(res);
}

// netresourcemanager.cpp:1006
void NetResourceManager::Download(const sResource &res, const char *address)
{
	if (address != NULL) {
		m_downloadInfo[res.type].option = 0;
		strcpy(m_downloadInfo[res.type].directory, "./ntsaved/");
		strcpy(m_downloadInfo[res.type].address, address);
	}
	ADD_TO_DOWNLOAD_LIST(res);
}

// netresourcemanager.cpp:1017
bool NetResourceManager::IsDownloadRequested(const sResource &res)
{
	std::list<sResource>::iterator it;
	for (it = m_downloadList.begin(); it != m_downloadList.end(); ++it) {
		if (it->type == res.type && it->path == res.path) {
			return true;
		}
	}

	return false;
}

// netresourcemanager.cpp:1035
bool NetResourceManager::IsDownloadRequested(eResourceType type, unsigned long id, const char *key)
{
	sResource res;
	res.type = type;
	res.id = id;
	strncpy(res.key, key, sizeof(res.key));

	return IsDownloadRequested(res);
}

// netresourcemanager.cpp:1045
bool NetResourceManager::IsUploadRequested(const sResource &res)
{
	std::list<sResource>::iterator it;
	for (it = m_uploadList.begin(); it != m_uploadList.end(); ++it) {
		if (it->type == res.type && it->path == res.path) {
			return true;
		}
	}

	return false;
}

// netresourcemanager.cpp:1063
void NetResourceManager::UploadUccClothes(unsigned long typeId, const char *uccKey, const char *data)
{
	sResource res;
	res.path = GenerateTextureFullPath(0, typeId, uccKey);

	res.type = RESOURCE_TYPE_UCC_CLOTHES;
	res.id = typeId;
	strcpy(res.data, data);
	strncpy(res.key, uccKey, sizeof(res.key));

	AddToUploadList(res);
}

// netresourcemanager.cpp:1075
void NetResourceManager::UploadUccTempClothes(unsigned long typeId, const char *uccKey)
{
	sResource res;
	res.path = GenerateTextureFullPath(0, typeId, uccKey);

	res.type = RESOURCE_TYPE_UCC_TEMP_CLOTHES;
	res.id = typeId;
	strncpy(res.key, uccKey, sizeof(res.key));

	AddToUploadList(res);
}

// netresourcemanager.cpp:1086
bool NetResourceManager::SetUploadStatus(unsigned long id, const char *key, eResourceType type)
{
	int count;
	bool found = false;

	EnterCriticalSection(&m_uploadLock);

	std::list<sUploadStatus>::iterator it;
	for (it = m_uploadStatusList.begin(); it != m_uploadStatusList.end(); ++it) {
		if (it->id == id && strcmp(it->key, key) == 0 && it->type == type) {
			found = true;
			break;
		}
	}

	if (!found) {
		sUploadStatus status;
		status.type = type;
		status.failCount = 1;
		status.id = id;
		strncpy(status.key, key, sizeof(status.key));

		m_uploadStatusList.push_back(status);

		count = 1;
	} else {
		++it->failCount;

		count = it->failCount;
	}

	LeaveCriticalSection(&m_uploadLock);

	if (type == RESOURCE_TYPE_UCC_CLOTHES || type == RESOURCE_TYPE_UCC_TEMP_CLOTHES) {
		if (count == 1) {
			return true;
		}
		return false;
	} else if (type == RESOURCE_TYPE_1 || type == RESOURCE_TYPE_EMBLEM) {
		if (count == 1) {
			return true;
		}
		return false;
	}

	return true;
}

// netresourcemanager.cpp:1171
void NetResourceManager::ClearGarbage(void)
{
	std::list<sResource> keepList;

	if (IsLocalContent((localContentType_t)0x5F)) {
		std::map<unsigned long, sRoomSlot>::iterator it;
		for (it = WSingleton<CSharedDoc>::Instance()->m_roomSlotMap.begin(); it != WSingleton<CSharedDoc>::Instance()->m_roomSlotMap.end(); ++it) {
			sRoomSlot *slot = &it->second;
			if (slot == NULL) {
				continue;
			}

			for (int i = 0; i < 24; ++i) {
				if (!IsUccClothes(slot->m_partTypeIds[i])) {
					continue;
				}

				sItemInfo *info = GetMyItemInfo(slot->m_partItemIds[i]);
				if (info == NULL || strlen(info->m_uccKey) == 0) {
					continue;
				}

				sResource res;
				res.id = slot->m_partTypeIds[i];
				strncpy(res.key, info->m_uccKey, sizeof(res.key));
				keepList.push_back(res);
			}
		}
	}

	EnterCriticalSection(&m_downloadLock);

	std::list<sResource>::iterator it;
	for (it = m_downloadList.begin(); it != m_downloadList.end(); ) {
		bool keep = false;

		std::list<sResource>::iterator keepIt;
		for (keepIt = keepList.begin(); keepIt != keepList.end(); ++keepIt) {
			if (it->id == keepIt->id && strcmp(it->key, keepIt->key) == 0) {
				keep = true;
				break;
			}
		}

		if (!keep) {
			it = m_downloadList.erase(it);
			continue;
		}

		++it;
	}

	LeaveCriticalSection(&m_downloadLock);
}

// netresourcemanager.cpp:1231
bool NetResourceManager::IsFailedToUpload(unsigned long id, const char *key, eResourceType type)
{
	bool failed = false;
	bool notRequested = true;

	if (type == RESOURCE_TYPE_UCC_CLOTHES) {
		failed = UccManager()->FindClothes(id, key) == NULL;
	} else if (type == RESOURCE_TYPE_1) {
		failed = UccManager()->FindAztec(id, key) == NULL;
	} else if (type == RESOURCE_TYPE_UCC_TEMP_CLOTHES) {
		Bitmap front;
		Bitmap back;
		failed = !UccManager()->GetUccTempClothes(id, key, front, back);
	}

	EnterCriticalSection(&m_uploadLock);

	std::list<sResource>::iterator it;
	for (it = m_uploadList.begin(); it != m_uploadList.end(); ++it) {
		if (it->type == type && it->id == id && strcmp(it->key, key) == 0) {
			notRequested = false;
		}
	}

	LeaveCriticalSection(&m_uploadLock);

	if (failed && notRequested) {
		return true;
	}
	return false;
}

// netresourcemanager.cpp:1280
bool NetResourceManager::Upload(const sResource &res, const char *address)
{
	if (res.type < RESOURCE_TYPE_COUNT) {
		if (address != NULL && strlen(address) < sizeof(m_uploadAddress[0])) {
			strcpy(m_uploadAddress[res.type], address);
		}
		AddToUploadList(res);
		return true;
	}

	return false;
}

// netresourcemanager.cpp:1297
void NetResourceManager::RequestGuildRSS(unsigned long guildIdx, void (__fastcall *callBack)(int))
{
	m_rssManager.GetRSSFile(guildIdx, callBack);
}

// netresourcemanager.cpp:1302
void NetResourceManager::RequestGuildRSS_JP(unsigned long guildIdx, void (__fastcall *callBack)(int), const char *param1, const char *param2)
{
	m_rssManager.GetRSSFile_JP(guildIdx, callBack, param1, param2);
}

// netresourcemanager.cpp:1311
void NetResourceManager::UploadBugReportData(const char *fileName, int mainType, int subType, bool attach)
{
	sPostArgument argument;
	std::list<sPostArgument> arguments;
	char buffer[32];

	argument.name = "bugReportFile";
	argument.value = fileName;
	arguments.push_back(argument);

	argument.name = "bugReportMainType";
	sprintf(buffer, "%d", mainType);
	argument.value = buffer;
	arguments.push_back(argument);

	argument.name = "bugReportSubType";
	sprintf(buffer, "%d", mainType);
	argument.value = buffer;
	arguments.push_back(argument);

	argument.name = "uid";
	sprintf(buffer, "%d", MyUID());
	argument.value = buffer;
	arguments.push_back(argument);

	argument.name = "userID";
	argument.value = WSingleton<CSharedDoc>::Instance()->GetPangyaUserInfo().id;
	arguments.push_back(argument);

	argument.name = "packetVer";
	sprintf(buffer, "%d", 2011100699);
	argument.value = buffer;
	arguments.push_back(argument);

	argument.name = "clientVer";
	argument.value = "645.00";
	arguments.push_back(argument);

	m_postArguments[fileName] = arguments;

	sResource res;
	res.type = RESOURCE_TYPE_5;
	strcpy(res.data, fileName);

	if (attach) {
		res.path = fileName;
	}

	Upload(res, "http://qagame.pangya.ntreev.com:50000/post/BugReport.asp");
}
