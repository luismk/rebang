// Original: d:\build\custom\temp\code\client\projectg\uccmanager.cpp
#include "UccManager.h"

#include "NetResourceManager.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "UccLibrary.h"
#include "bitmap.h"
#include "wresrcmng.h"

// uccmanager.cpp:11
CUccManager *UccManager(void) {
	static CUccManager s_uccManager;
	return &s_uccManager;
}

// uccmanager.cpp:17
CUccManager::CUccManager(void) {
	InitializeCriticalSection(&m_refreshLock);
}

// uccmanager.cpp:22
CUccManager::~CUccManager(void) {
	DeleteCriticalSection(&m_refreshLock);

	m_clothesList.clear();
	m_aztecList.clear();

	m_refreshList.clear();
	ClearTempClothes();
}

// uccmanager.cpp:34
void CUccManager::AddClothes(unsigned char slot, unsigned long serial, unsigned long typeId, const char *uccKey, bool send, bool request) {
	sUccClothes clothes;
	clothes.m_serial = serial; clothes.m_typeId = typeId;
	strncpy(clothes.m_uccKey, uccKey, 9);

	AddClothes(slot, clothes, send, request);
}

// uccmanager.cpp:44
void CUccManager::AddClothes(unsigned char slot, sUccClothes &clothes, bool send, bool request) {
	sUccClothes *found = FindClothes(clothes.m_typeId, clothes.m_uccKey);
	if (found == NULL) {
		m_clothesList.push_back(clothes);

		if (clothes.m_downloadCount == 0) {
			RequestClothes(clothes.m_typeId, clothes.m_uccKey, request);
		}
	} else {
		if (clothes.m_serial != 0xFFFFFFFF) {
			if (FindClothes(clothes.m_serial) == NULL) {
				sUccClothes copy = *found;
				copy.m_typeId = clothes.m_typeId;
				copy.m_serial = clothes.m_serial;
				m_clothesList.push_back(copy);

				AddRefreshItem(copy.m_typeId, copy.m_uccKey);
			}
		}
	}

	if (send) {
		WSendPacket packet((enumClientPacket)0xB1);
		packet.Encode1(1);
		packet.Encode4(clothes.m_serial);
		packet.Encode1(slot);
		packet.Send((eSendTo)0);
	}
}

// uccmanager.cpp:86
void CUccManager::AddAztec(sUccAztec &aztec, bool request) {
	if (FindAztec(aztec.m_typeId, aztec.m_uccKey) == NULL) {
		m_aztecList.push_back(aztec);

		if (aztec.m_downloadCount == 0) {
			RequestAztec(aztec.m_typeId, aztec.m_uccKey, request);
		}
	}
}

// uccmanager.cpp:99
sUccClothes *CUccManager::FindClothes(unsigned long typeId, const char *uccKey) {
	std::list<sUccClothes>::iterator it;

	for (it = m_clothesList.begin(); it != m_clothesList.end(); ++it) {
		sUccClothes *clothes = &(*it);
		if (clothes->m_typeId == typeId && strcmp(clothes->m_uccKey, uccKey) == 0) {
			return clothes;
		}
	}

	for (it = m_clothesList.begin(); it != m_clothesList.end(); ++it) {
		sUccClothes *clothes = &(*it);
		if (strcmp(clothes->m_uccKey, uccKey) == 0 && IsSameClothes(typeId, clothes->m_typeId)) {
			return clothes;
		}
	}

	return NULL;
}

// uccmanager.cpp:128
sUccAztec *CUccManager::FindAztec(unsigned long typeId, const char *uccKey) {
	for (std::list<sUccAztec>::iterator it = m_aztecList.begin(); it != m_aztecList.end(); ++it) {
		sUccAztec *aztec = &(*it);
		if (aztec->m_typeId == typeId && strcmp(aztec->m_uccKey, uccKey) == 0) {
			return aztec;
		}
	}

	return NULL;
}

// uccmanager.cpp:143
sUccClothes *CUccManager::FindClothes(unsigned long serial) {
	sItemInfo *info = GetMyItemInfo(serial);

	if (info) {
		std::list<sItemInfo>::iterator it;

		for (it = WSingleton<CSharedDoc>::Instance()->m_myItemList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myItemList.end(); ++it) {
			if ((*it).m_typeId == info->m_typeId && strcmp((*it).m_uccKey, info->m_uccKey) == 0) {
				sUccClothes *clothes = _GetClothes((*it).m_serial);
				if (clothes) {
					return clothes;
				}
			}
		}

		for (it = WSingleton<CSharedDoc>::Instance()->m_myItemList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myItemList.end(); ++it) {
			if (strcmp((*it).m_uccKey, info->m_uccKey) == 0 && IsSameClothes(info->m_typeId, (*it).m_typeId)) {
				sUccClothes *clothes = _GetClothes((*it).m_serial);
				if (clothes) {
					return clothes;
				}
			}
		}
	} else {
		return _GetClothes(serial);
	}

	return NULL;
}

// uccmanager.cpp:189
sUccClothes *CUccManager::_GetClothes(unsigned long serial) {
	for (std::list<sUccClothes>::iterator it = m_clothesList.begin(); it != m_clothesList.end(); ++it) {
		sUccClothes *entry = &(*it);
		if (entry->m_serial == serial) {
			return entry;
		}
	}

	return NULL;
}

// uccmanager.cpp:205
sUccAztec *CUccManager::FindAztec(unsigned long serial) {
	sItemInfo *info = GetMyItemInfo(serial);
	if (info == NULL) {
		return _GetAztec(serial);
	}

	for (std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_myAuxItemList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_myAuxItemList.end(); ++it) {
		if ((*it).m_typeId == info->m_typeId && strcmp((*it).m_uccKey, info->m_uccKey) == 0) {
			sUccAztec *aztec = _GetAztec((*it).m_serial);
			if (aztec) {
				return aztec;
			}
		}
	}

	return NULL;
}

// uccmanager.cpp:229
sUccAztec *CUccManager::_GetAztec(unsigned long serial) {
	for (std::list<sUccAztec>::iterator it = m_aztecList.begin(); it != m_aztecList.end(); ++it) {
		sUccAztec *entry = &(*it);
		if (entry->m_serial == serial) {
			return entry;
		}
	}

	return NULL;
}

// uccmanager.cpp:246
bool CUccManager::IsDownloadComplete(unsigned long typeId, const char *uccKey) {
	sUccClothes *clothes = FindClothes(typeId, uccKey);
	sUccAztec *aztec = FindAztec(typeId, uccKey);

	if (clothes) {
		return IsDownloadComplete(clothes);
	}

	if (aztec) {
		return IsDownloadComplete(aztec);
	}

	return false;
}

// uccmanager.cpp:264
bool CUccManager::SetDownloadComplete(unsigned long typeId, const char *uccKey) {
	sUccClothes *clothes = FindClothes(typeId, uccKey);
	sUccAztec *aztec = FindAztec(typeId, uccKey);

	if (clothes) {
		return SetDownloadComplete(clothes);
	}

	if (aztec) {
		return SetDownloadComplete(aztec);
	}

	return false;
}

// uccmanager.cpp:282
bool CUccManager::IsDownloadComplete(sUccClothes *clothes) {
	if (clothes && clothes->m_downloadCount == 1) {
		return true;
	}

	return false;
}

// uccmanager.cpp:294
bool CUccManager::IsDownloadComplete(sUccAztec *aztec) {
	if (aztec && aztec->m_downloadCount == 1) {
		return true;
	}

	return false;
}

// uccmanager.cpp:306
bool CUccManager::SetDownloadComplete(sUccClothes *clothes) {
	if (clothes) {
		clothes->m_downloadCount++;
		if (clothes->m_downloadCount == 1) {
			for (std::list<sUccClothes>::iterator it = m_clothesList.begin(); it != m_clothesList.end(); ++it) {
				if (IsSameClothes(clothes->m_typeId, (*it).m_typeId) && strcmpi(clothes->m_uccKey, (*it).m_uccKey) == 0) {
					(*it).m_downloadCount = clothes->m_downloadCount;
					(*it).m_pIcon = clothes->m_pIcon;
					strncpy((*it).m_uccName, clothes->m_uccName, 0x29);
				}
			}

			return true;
		}
	}

	return false;
}

// uccmanager.cpp:331
bool CUccManager::SetDownloadComplete(sUccAztec *aztec) {
	if (aztec) {
		aztec->m_downloadCount++;
		return aztec->m_downloadCount == 1 ? true : false;
	}

	return false;
}

// uccmanager.cpp:343
void CUccManager::RequestClothes(unsigned long typeId, const char *uccKey, bool force) {
	NetResourceManager::Instance()->DownloadUccClothes(typeId, uccKey, force, false);
}

// uccmanager.cpp:348
void CUccManager::RequestAztec(unsigned long typeId, const char *uccKey, bool force) {
}

// uccmanager.cpp:352
void CUccManager::Clear(void) {
	ClearTexture();
	m_clothesList.clear();
	m_aztecList.clear();
	ClearIcon();
	m_refreshList.clear();
	m_displayClothesList.clear();
	ClearTempClothes();
}

// uccmanager.cpp:363
void CUccManager::ClearTexture(void) {
	std::list<sUccClothes>::iterator it = m_clothesList.begin();
	while (it != m_clothesList.end()) {
		for (int i = 0; i < 2; i++) {
			std::string name;
			if (i == 0) {
				name = GenerateTextureName_F((*it).m_typeId, (*it).m_uccKey);
			} else if (i == 1) {
				name = GenerateTextureName_B((*it).m_typeId, (*it).m_uccKey);
			}

			w_texlist *texture = g_resrcmng->FindTexture(name.c_str());
			if (texture) {
				g_resrcmng->Release(texture->texhandle);
			}
		}

		it = m_clothesList.erase(it);
	}
}

// uccmanager.cpp:389
void CUccManager::ClearIcon(void) {
	for (std::map<std::string, Bitmap *>::iterator it = m_iconMap.begin(); it != m_iconMap.end(); ++it) {
		Bitmap *icon = (*it).second;
		ClearTexCache(icon); delete icon;
	}

	m_iconMap.clear();
}

// uccmanager.cpp:400
void CUccManager::ClearTempClothes(void) {
	for (std::list<sUccTempClothes>::iterator it = m_tempClothesList.begin(); it != m_tempClothesList.end(); ++it) {
		ClearTexCache((*it).m_pFront); delete (*it).m_pFront; (*it).m_pFront = NULL;
		ClearTexCache((*it).m_pBack); delete (*it).m_pBack; (*it).m_pBack = NULL;
	}

	m_tempClothesList.clear();
}

// uccmanager.cpp:415
void CUccManager::AddRefreshItem(unsigned long typeId, const char *uccKey) {
	sUccRefreshItem item;
	item.m_typeId = typeId;
	strncpy(item.m_uccKey, uccKey, 9);

	EnterCriticalSection(&m_refreshLock);
	m_refreshList.push_back(item);
	LeaveCriticalSection(&m_refreshLock);

	IFF_STRUCT::sPart *part = ItemManager()->FindPart(typeId);

	for (std::map<unsigned int, IFF_STRUCT::sPart>::iterator it = ItemManager()->m_partMap.begin(); it != ItemManager()->m_partMap.end(); ++it) {
		IFF_STRUCT::sPart *other = &(*it).second;
		if (typeId != other->m_typeId && IsSameClothes(part, other)) {
			sUccRefreshItem sameItem;
			sameItem.m_typeId = other->m_typeId;
			strncpy(sameItem.m_uccKey, uccKey, 9);

			EnterCriticalSection(&m_refreshLock);
			m_refreshList.push_back(sameItem);
			LeaveCriticalSection(&m_refreshLock);
		}
	}
}

// uccmanager.cpp:448
bool CUccManager::GetRefreshItem(sUccRefreshItem &item) {
	EnterCriticalSection(&m_refreshLock);

	int count = m_refreshList.size();

	if (count > 0) {
		item = m_refreshList.front();
		m_refreshList.pop_front();
	}

	LeaveCriticalSection(&m_refreshLock);

	return count > 0;
}

// uccmanager.cpp:467
void CUccManager::SetUccTempClothes(unsigned long typeId, const char *uccKey, Bitmap *front, Bitmap *back) {
	for (std::list<sUccTempClothes>::iterator it = m_tempClothesList.begin(); it != m_tempClothesList.end(); ++it) {
		if ((*it).m_typeId == typeId && strcmpi((*it).m_uccKey, uccKey) == 0) {
			*(*it).m_pFront = *front;
			*(*it).m_pBack = *back;
			return;
		}
	}

	sUccTempClothes clothes;
	clothes.m_typeId = typeId;
	strncpy(clothes.m_uccKey, uccKey, 9);
	*clothes.m_pFront = *front;
	*clothes.m_pBack = *back;

	m_tempClothesList.push_back(clothes);
}

// uccmanager.cpp:494
bool CUccManager::GetUccTempClothes(unsigned long typeId, const char *uccKey, Bitmap &front, Bitmap &back) {
	for (std::list<sUccTempClothes>::iterator it = m_tempClothesList.begin(); it != m_tempClothesList.end(); ++it) {
		if ((*it).m_typeId == typeId && strcmpi((*it).m_uccKey, uccKey) == 0) {
			front = *(*it).m_pFront;
			back = *(*it).m_pBack;
			return true;
		}
	}

	return false;
}

// uccmanager.cpp:512
void CUccManager::BuildDisplayClothesSet(void) {
}

// uccmanager.cpp:565
bool CUccManager::IsDisplayUccClothes(const IFF_ITEM_COMMON *item) {
	return strcmpi(item->m_icon, "UCCDISPLAY") == 0;
}

// uccmanager.cpp:570
unsigned long CUccManager::GetWinnerCharacterTypeId(void) {
	unsigned long typeId = 0;
	int bestRank = 999999;

	for (std::list<sDisplayUccClothes>::iterator it = m_displayClothesList.begin(); it != m_displayClothesList.end(); ++it) {
		if ((*it).m_rank < bestRank) {
			typeId = (*it).m_typeId;
			bestRank = (*it).m_rank;
		}
	}

	return 0x4000000 | ((typeId >> 18) & 0xFF);
}
