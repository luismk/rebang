// Original: d:\build\custom\temp\code\client\projectg\parttidlist.cpp
#include "parttidlist.h"
#include "SharedDoc.h"
#include "../../shared/ItemManager.h"
#include "../../shared/classdefine.h"
#include "../../shared/globalgamedefine.h"
#include "../../shared/Localize.h"

#include <string.h>

CItemManager *CPartTidList::m_pItemManager;

// parttidlist.cpp:19
void CPartTidList::SetItemManager(CItemManager *pItemManager) {
	m_pItemManager = pItemManager;
}

// parttidlist.cpp:25
CPartTidList::CPartTidList(void)
	: m_tidChar(0), m_unk04(0), m_numTotalParts(0), m_hairClr(0), m_shirtsClr(0) {
	memset(m_partTids, 0, sizeof(m_partTids));
	memset(m_partIds, 0, sizeof(m_partIds));
}

// parttidlist.cpp:32
bool CPartTidList::SetTids(sCharacterInfo *pCharInfo, unsigned char level) {
	if (pCharInfo == NULL) {
		return false;
	}

	bool result = SetTids(pCharInfo->typeId, pCharInfo->hairColor, pCharInfo->shirtColor & 0xF,
		pCharInfo->partTypeIds, level, pCharInfo->partIds);

	if (!result) {
		m_tidChar = pCharInfo->typeId;
	}

	return result;
}

// parttidlist.cpp:48
bool CPartTidList::SetTids(unsigned long tidChar, unsigned char hairClr, unsigned char shirtsClr,
	unsigned long *partTids, unsigned char level, unsigned long *partIds) {
	IFF_STRUCT::sChar *pChar = m_pItemManager->FindChar(tidChar);

	if (pChar == NULL) {
		return false;
	}

	unsigned long charIndex = tidChar & 0x3FFFFFF;
	if (charIndex == 4) {
		partTids[18] = 0x8124400;
		partTids[19] = 0x8126400;
	} else if (charIndex == 6) {
		partTids[15] = 0x819E400;
		partTids[19] = 0x81A6400;
		partTids[20] = 0x81A8400;
	} else if (charIndex == 2) {
		partTids[18] = 0x80A4400;
	} else if (charIndex == 0) {
		partTids[18] = 0x8024400;
	} else if (charIndex == 1) {
		partTids[19] = 0x8066400;
	} else if (charIndex == 7) {
		partTids[22] = 0x81EC400;
	} else if (charIndex == 3) {
		partTids[19] = 0x80E6400;
	} else if (charIndex == 5) {
		partTids[9] = 0x8152400;
	} else if (charIndex == 8) {
		partTids[19] = 0x8226400;
	} else if (charIndex == 9) {
		if (m_pItemManager->FindPart(0x826A400) != NULL && m_pItemManager->FindPart(0x826C400) != NULL) {
			partTids[21] = 0x826A400;
			partTids[22] = 0x826C400;
		}
	}

	if (m_numTotalParts != 0) {
		memset(m_partTids, 0, m_numTotalParts * sizeof(unsigned long));
		memset(m_partIds, 0, sizeof(m_partIds));
	}

	unsigned long hideMask = 0;
	int i;
	for (i = 0; i < pChar->m_numParts + pChar->m_numExtraParts; i++) {
		if (partTids[i] != 0) {
			if (((partTids[i] >> 18) & 0xFF) != charIndex) {
				return false;
			}

			IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(partTids[i]);

			if (pPart == NULL) {
				return false;
			}

			m_partTids[i] = partTids[i];
			hideMask |= pPart->hideMask;

			if (IsLocalContent((localContentType_t)0x5F)) {
				m_partIds[i] = partIds[i];
			}
		}
	}

	for (i = 0; i < pChar->m_numParts; i++) {
		if ((hideMask & (1 << i)) == 0) {
			unsigned long tid;
			if (i == 0) {
				tid = (tidChar << 18) | hairClr;
			} else {
				tid = ((tidChar << 5) | i) << 13;
			}
			tid |= 0x8000600;
			IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(tid);
			if (pPart != NULL && (pPart->m_level & 0x7F) <= level) {
				m_partTids[i] = tid;
			} else {
				m_partTids[i] = 0;
			}

			if (IsLocalContent((localContentType_t)0x5F)) {
				m_partIds[i] = 0;
			}
		}
	}

	m_tidChar = tidChar;
	m_hairClr = hairClr;
	m_shirtsClr = shirtsClr;
	m_unk04 = pChar->m_numParts;
	m_numTotalParts = pChar->m_numParts + pChar->m_numExtraParts;

	for (i = 0; i < pChar->m_numParts + pChar->m_numExtraParts; i++) {
		if (partTids[i] != 0) {
			IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(partTids[i]);

			if ((pPart->m_level & 0x80) == 0 && (pPart->m_level & 0x7F) > level) {
				if (!SetTid(pPart->m_typeId, partIds[i], NULL, 0)) {
					ItemManager()->GetDefCombo(m_tidChar, m_partTids);
				}

				memcpy(partTids, m_partTids, m_numTotalParts * sizeof(unsigned long));
			}
		}
	}

	if (IsLocalContent((localContentType_t)0x5F)) {
		memcpy(partIds, m_partIds, sizeof(m_partIds));
	}

	return true;
}

// parttidlist.cpp:218
bool CPartTidList::SetTid(unsigned long tid, unsigned long id, const char *texture, unsigned short slot) {
	unsigned char part = (unsigned char)((tid >> 13) & 0x1F);
	bool skipSet = false;

	if (m_partTids[part] == tid && (id == 0 || m_partIds[part] == id)) {
		int i;
		for (i = 0; i < m_numTotalParts; i++) {
			if ((m_partTids[i] & 0x600) == 0x200) {
				unsigned long baseTid = m_partTids[i] & ~0x200;
				IFF_STRUCT::sPart *pBase = ItemManager()->FindPart(baseTid);

				if (pBase != NULL) {
					if (pBase->hideMask & (1 << part)) {
						tid = baseTid;
						break;
					}
				}
			}
		}

		if (part < m_unk04 && i == m_numTotalParts) {
			tid = (((m_tidChar << 5) | part) << 13) | 0x8000400;
		} else {
			skipSet = true;
		}
	}

	IFF_STRUCT::sPart *pPart = ItemManager()->FindPart(tid);

	if (pPart == NULL) {
		return false;
	}

	unsigned long remainMask = pPart->hideMask;
	for (int i = 0; i < m_numTotalParts; i++) {
		if (m_partTids[i] != 0) {
			IFF_STRUCT::sPart *pOther = ItemManager()->FindPart(m_partTids[i]);
			unsigned long overlap = pPart->hideMask & pOther->hideMask;

			if (overlap != 0) {
				remainMask &= ~overlap;

				if (overlap != pOther->hideMask) {
					switch ((m_partTids[i] >> 9) & 3) {
					case 0:
					case 2: {
						unsigned long altTid = m_partTids[i] | 0x200;
						IFF_STRUCT::sPart *pAlt = ItemManager()->FindPart(altTid);

						if (pAlt != NULL) {
							EmptyList(pAlt->hideMask);

							for (int j = 0; j < m_numTotalParts; j++) {
								if (pAlt->hideMask & (1 << j)) {
									m_partTids[j] = altTid;
									m_partIds[j] = 0;
									break;
								}
							}
						} else {
							unsigned long freed = (pPart->hideMask ^ pOther->hideMask) & pOther->hideMask;

							for (int j = 0; j < m_numTotalParts; j++) {
								if ((freed & (0xFFFFFFFF << j)) == 0) {
									break;
								}

								if (freed & (1 << j)) {
									unsigned long defTid = (((m_tidChar << 5) | j) << 13) | 0x8000400;

									if (ItemManager()->FindPart(defTid) != NULL) {
										m_partTids[j] = defTid;
										m_partIds[j] = 0;
									}
								}
							}
						}
						break;
					}
					}
				}

				EmptyList(overlap);
			}

			if (remainMask == 0) {
				break;
			}
		}
	}

	if (!skipSet) {
		m_partTids[part] = tid;

		if (IsLocalContent((localContentType_t)0x5F) && id != 0) {
			m_partIds[part] = id;
		}
	}

	unsigned long usedMask = 0;
	for (int i = 0; i < m_numTotalParts; i++) {
		IFF_STRUCT::sPart *pUsed = ItemManager()->FindPart(m_partTids[i]);

		if (pUsed != NULL) {
			usedMask |= pUsed->hideMask;
		}
	}

	for (int j = 0; j < m_unk04; j++) {
		if (usedMask & (1 << j)) {
			continue;
		}

		unsigned long defTid = (((m_tidChar << 5) | j) << 13) | 0x8000400;
		IFF_STRUCT::sPart *pDefault = ItemManager()->FindPart(defTid);

		if (pDefault != NULL && (pDefault->hideMask & usedMask) == 0) {
			m_partTids[j] = defTid;
			m_partIds[j] = 0;
		}
	}

	return true;
}

// parttidlist.cpp:415
void CPartTidList::EmptyList(unsigned long mask) {
	for (int i = 0; i < m_numTotalParts; i++) {
		if ((mask & (0xFFFFFFFF << i)) == 0) {
			break;
		}

		if (mask & (1 << i)) {
			m_partTids[i] = 0;
			if (IsLocalContent((localContentType_t)0x5F)) {
				m_partIds[i] = 0;
			}
		}
	}
}

// parttidlist.cpp:436
void CPartTidList::SetDefaultTids(void) {
	memset(m_partTids, 0, sizeof(m_partTids));
	memset(m_partIds, 0, sizeof(m_partIds));

	m_pItemManager->GetDefCombo(m_tidChar, m_partTids);
}

// parttidlist.cpp:446
bool CPartTidList::IsComboValid(void) const {
	unsigned long usedMask = 0;

	for (int i = 0; i < m_numTotalParts; i++) {
		if (m_partTids[i] == 0) {
			continue;
		}

		if (((m_partTids[i] >> 18) & 0xFF) != (m_tidChar & 0x3FFFFFF)) {
			return false;
		}

		IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(m_partTids[i]);

		if (pPart == NULL) {
			return false;
		}

		if (usedMask & pPart->hideMask) {
			return false;
		}

		usedMask |= pPart->hideMask;
	}

	unsigned long required = ~(0xFFFFFFFF << m_unk04);
	return (usedMask & required) == required ? true : false;
}

// parttidlist.cpp:509
bool CPartTidList::IsEquipped(unsigned long tid, unsigned long id) const {
	for (int i = 0; i < m_numTotalParts; i++) {
		if (id != 0 && m_partIds[i] != 0) {
			if (m_partTids[i] == tid && id == m_partIds[i]) {
				return true;
			}
		} else {
			if (m_partTids[i] == tid) {
				return true;
			}
		}
	}

	return false;
}

// parttidlist.cpp:533
unsigned long CPartTidList::GetOverlapped(unsigned long tid) const {
	if (((tid >> 18) & 0xFF) != (m_tidChar & 0x3FFFFFF)) {
		return 0;
	}

	IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(tid);

	unsigned long overlapped = 0;
	for (int i = 0; i < m_numTotalParts; i++) {
		if (m_partTids[i] == 0) {
			continue;
		}

		if (((m_partTids[i] >> 18) & 0xFF) != (m_tidChar & 0x3FFFFFF)) {
			return 0;
		}

		IFF_STRUCT::sPart *pOther = m_pItemManager->FindPart(m_partTids[i]);

		if (pOther == NULL) {
			return 0;
		}

		if (pPart->hideMask & pOther->hideMask) {
			overlapped |= 1 << i;
		}
	}

	return overlapped;
}

// parttidlist.cpp:571
bool CPartTidList::CanSetPart(unsigned long tid) const {
	if (((tid >> 18) & 0xFF) != (m_tidChar & 0x3FFFFFF)) {
		return false;
	}

	IFF_STRUCT::sPart *pPart = m_pItemManager->FindPart(tid);

	for (int i = 0; i < m_unk04; i++) {
		if (m_partTids[i] == 0) {
			continue;
		}

		IFF_STRUCT::sPart *pOther = m_pItemManager->FindPart(m_partTids[i]);

		if (pOther == NULL) {
			return false;
		}

		if ((pOther->hideMask & pPart->hideMask) == 0) {
			continue;
		}

		unsigned long remain = (pOther->hideMask ^ pPart->hideMask) & pOther->hideMask;

		for (int j = 0; j < m_unk04; j++) {
			if ((remain & (0xFFFFFFFF << j)) == 0) {
				break;
			}

			if (remain & (1 << i)) {
				unsigned long defTid = (((m_tidChar << 5) | i) << 13) | 0x8000400;
				IFF_STRUCT::sPart *pDefault = ItemManager()->FindPart(defTid);

				if (pDefault != NULL && (pDefault->hideMask & pPart->hideMask)) {
					return false;
				}
			}
		}
	}

	return true;
}
