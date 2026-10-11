// Original: d:\build\custom\temp\code\client\projectg\partsinfo.cpp
#include "partsinfo.h"
#include "parttidlist.h"
#include "../../shared/classdefine.h"
#include "../../shared/Localize.h"
#include "../../shared/ItemManager.h"
#include "SharedDoc.h"
#include "../Wangreal/include/wpuppet.h"
#include "../Wangreal/include/wbone.h"
#include "../Wangreal/include/wresrcmng.h"
#include "UccLibrary.h"

#include <string.h>
#include <string>

// partsinfo.h:41
unsigned char CPartList::GetNumTotalParts(void) const {
	return m_numTotalParts;
}

// partsinfo.cpp:24
CPartList::CPartList(void)
	: m_tidChar(0), m_numDefaultParts(0), m_numTotalParts(0), m_hairClr(0), m_shirtsClr(0) {
}

// partsinfo.cpp:28
CPartList::~CPartList(void) {
}

// partsinfo.cpp:32
bool CPartList::Build(const CPartTidList *pTidList) {
	if (!pTidList->IsComboValid()) {
		ItemManager()->GetDefCombo(pTidList->m_tidChar, (unsigned long *)pTidList->m_partTids);
	}

	memset(m_parts, 0, sizeof(m_parts));

	m_tidChar = pTidList->m_tidChar;
	m_numDefaultParts = pTidList->m_unk04;
	m_numTotalParts = pTidList->m_numTotalParts;
	m_hairClr = pTidList->m_hairClr;
	m_shirtsClr = pTidList->m_shirtsClr;

	for (int i = 0; i < m_numTotalParts; i++) {
		if (pTidList->m_partTids[i] != 0) {
			IFF_STRUCT::sPart *pIff = ItemManager()->FindPart(pTidList->m_partTids[i]);
			m_parts[i].m_tid = pTidList->m_partTids[i];
			m_parts[i].m_pIff = pIff;
			if (IsLocalContent((localContentType_t)0x5F)) {
				m_parts[i].m_id = pTidList->m_partIds[i];
			}
		}
	}

	return true;
}

// partsinfo.cpp:73
unsigned long CPartList::SetPart(CPartTidList *pTidList, unsigned long tid, unsigned long id) {
	if (((tid >> 18) & 0xFF) != (m_tidChar & 0x3FFFFFF) || ((tid >> 18) & 0xFF) != (pTidList->m_tidChar & 0x3FFFFFF)) {
		return 0;
	}
	if (!IsIdentical(pTidList) || !pTidList->IsComboValid()) {
		return 0;
	}
	if (!SetPart(tid, id)) {
		return 0;
	}
	if (!CheckSubParts()) {
		return 0;
	}

	unsigned long mask = 0;
	for (int i = 0; i < m_numTotalParts; i++) {
		if (pTidList->m_partTids[i] != m_parts[i].m_tid
			|| (IsLocalContent((localContentType_t)0x57) && m_parts[i].m_tid == tid && IsUccClothes(m_parts[i].m_tid))) {
			mask |= 1 << i;
			pTidList->m_partTids[i] = m_parts[i].m_tid;
			if (IsLocalContent((localContentType_t)0x5F)) {
				pTidList->m_partIds[i] = m_parts[i].m_id;
			}
		}
	}

	return mask;
}

// partsinfo.cpp:113
bool CPartList::SetPart(unsigned long tid, unsigned long id) {
	unsigned char slot = (unsigned char)((tid >> 13) & 0x1F);
	bool unequip = false;
	if (m_parts[slot].m_tid == tid && (id == 0 || m_parts[slot].m_id == 0 || m_parts[slot].m_id == id)) {
		if (slot < m_numDefaultParts) {
			tid = ((m_tidChar << 5 | slot) << 13) | 0x8000400;
			id = 0;
		} else {
			unequip = true;
		}
	}

	IFF_STRUCT::sPart *pIff = ItemManager()->FindPart(tid);
	if (pIff == 0) {
		return false;
	}

	unsigned long hideMask = pIff->hideMask;
	for (int i = 0; i < m_numTotalParts; i++) {
		if (m_parts[i].m_tid != 0) {
			unsigned long overlap = m_parts[i].m_pIff->hideMask & pIff->hideMask;
			if (overlap != 0) {
				hideMask &= ~overlap;
				if (overlap != m_parts[i].m_pIff->hideMask) {
					switch ((m_parts[i].m_tid >> 9) & 3) {
					case 0:
					case 2: {
						unsigned long subTid = m_parts[i].m_tid | 0x200;
						IFF_STRUCT::sPart *pSub = ItemManager()->FindPart(subTid);
						if (pSub != 0) {
							EmptyList(pSub->hideMask);
							for (int j = 0; j < m_numTotalParts; j++) {
								if (pSub->hideMask & (1 << j)) {
									m_parts[j].m_tid = subTid;
									m_parts[j].m_pIff = pSub;
									if (IsLocalContent((localContentType_t)0x5F)) {
										m_parts[j].m_id = 0;
									}
									break;
								}
							}
						} else {
							unsigned long freed = (pIff->hideMask ^ m_parts[i].m_pIff->hideMask) & m_parts[i].m_pIff->hideMask;
							for (int j = 0; j < m_numTotalParts; j++) {
								if ((freed & (0xFFFFFFFF << j)) == 0) {
									break;
								}
								if (freed & (1 << j)) {
									unsigned long defTid = ((m_tidChar << 5 | j) << 13) | 0x8000400;
									IFF_STRUCT::sPart *pDef = ItemManager()->FindPart(defTid);
									if (pDef != 0) {
										if (pIff->hideMask & pDef->hideMask) {
											return false;
										}
										EmptyList(pDef->hideMask);
										m_parts[j].m_tid = defTid;
									} else {
										m_parts[j].m_tid = 0;
									}
									m_parts[j].m_pIff = pDef;
									if (IsLocalContent((localContentType_t)0x5F)) {
										m_parts[j].m_id = 0;
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
			if (hideMask == 0) {
				break;
			}
		}
	}

	if (!unequip) {
		m_parts[slot].m_tid = tid;
		m_parts[slot].m_pIff = pIff;
		if (IsLocalContent((localContentType_t)0x5F)) {
			m_parts[slot].m_id = id;
		}
	}

	unsigned long usedMask = 0;
	for (int k = 0; k < m_numTotalParts; k++) {
		if (m_parts[k].m_pIff != 0) {
			usedMask |= m_parts[k].m_pIff->hideMask;
		}
	}

	if (!unequip) {
		unsigned long subTid = ((m_tidChar << 5 | slot) << 13) | 0x8000600;
		IFF_STRUCT::sPart *pSub = ItemManager()->FindPart(subTid);
		if (pSub != 0 && (usedMask & pSub->hideMask) == 0) {
			int j;
			for (j = 0; j < m_numDefaultParts; j++) {
				if (pSub->hideMask & (1 << j)) {
					break;
				}
			}
			if (m_parts[j].m_tid == 0) {
				m_parts[j].m_tid = subTid;
				m_parts[j].m_pIff = pSub;
				if (IsLocalContent((localContentType_t)0x5F)) {
					m_parts[j].m_id = 0;
				}
			}
		}
	}

	for (int k = 0; k < m_numDefaultParts; k++) {
		if ((usedMask & (1 << k)) == 0) {
			unsigned long defTid = ((m_tidChar << 5 | k) << 13) | 0x8000400;
			IFF_STRUCT::sPart *pDef = ItemManager()->FindPart(defTid);
			if (pDef != 0 && (pDef->hideMask & usedMask) == 0) {
				m_parts[k].m_tid = defTid;
				m_parts[k].m_pIff = pDef;
				if (IsLocalContent((localContentType_t)0x5F)) {
					m_parts[k].m_id = 0;
				}
			}
		}
	}

	return true;
}

// partsinfo.cpp:322
void CPartList::EmptyList(unsigned long mask) {
	for (int i = 0; i < m_numTotalParts; i++) {
		if ((mask & (0xFFFFFFFF << i)) == 0) {
			break;
		}
		if (mask & (1 << i)) {
			m_parts[i].m_tid = 0;
			m_parts[i].m_pIff = 0;
			if (IsLocalContent((localContentType_t)0x5F)) {
				m_parts[i].m_id = 0;
			}
		}
	}
}

// partsinfo.cpp:426
bool CPartList::IsEmpty(unsigned long mask, unsigned char except) {
	for (int i = 0; i < m_numTotalParts; i++) {
		if (m_parts[i].m_pIff != 0 && except != i && (m_parts[i].m_pIff->hideMask & mask)) {
			return false;
		}
	}
	return true;
}

// partsinfo.cpp:442
bool CPartList::SetHairClr(CPartTidList *pTidList, unsigned char clr) {
	if (m_tidChar != pTidList->m_tidChar) {
		return false;
	}
	if (pTidList->m_hairClr == clr) {
		return false;
	}
	pTidList->m_hairClr = clr;
	m_hairClr = clr;
	return true;
}

// partsinfo.cpp:458
bool CPartList::SetShirtsClr(CPartTidList *pTidList, unsigned char clr) {
	if (m_tidChar != pTidList->m_tidChar) {
		return false;
	}
	if (pTidList->m_shirtsClr == clr) {
		return false;
	}
	pTidList->m_shirtsClr = clr;
	m_shirtsClr = clr;
	return true;
}

// partsinfo.cpp:474
bool CPartList::IsIdentical(CPartTidList *pTidList) {
	if (m_tidChar != pTidList->m_tidChar) {
		return false;
	}
	if (m_numDefaultParts != pTidList->m_unk04 || m_numTotalParts != pTidList->m_numTotalParts) {
		return false;
	}
	int i;
	for (i = 0; i < m_numTotalParts; i++) {
		if (m_parts[i].m_tid != pTidList->m_partTids[i]) {
			return false;
		}
	}
	if (IsLocalContent((localContentType_t)0x5F)) {
		for (i = 0; i < m_numTotalParts; i++) {
			if (m_parts[i].m_id != 0 && pTidList->m_partIds[i] != 0) {
				if (m_parts[i].m_id != pTidList->m_partIds[i]) {
					return false;
				}
			}
		}
	}
	return true;
}

// partsinfo.cpp:505
bool CPartList::FindPart(unsigned long tid) const {
	for (unsigned char i = 0; i < m_numTotalParts; i++) {
		if (GetPart(i)->m_tid == tid) {
			return true;
		}
	}
	return false;
}

// partsinfo.cpp:521
CPetFrame::CPetFrame(void)
	: m_tidChar(0), m_pBonePet(0), m_hairTex(0), m_shirtsTex(0), m_faceTex(0), m_pCurAniPet(0),
	  m_unk28(0), m_numPets(0), m_makeupIndex(1), m_unk1B4(true), m_hasMakeup(false) {
	memset(m_pets, 0, sizeof(m_pets));
	memset(m_petTex, 0, sizeof(m_petTex));
	memset(m_auxParts, 0, sizeof(m_auxParts));
}

// partsinfo.cpp:528
CPetFrame::~CPetFrame(void) {
	ReleasePets(7);
}

// partsinfo.cpp:533
void CPetFrame::ReleasePets(int mask) {
	if ((mask & 1) && g_resrcmng != 0 && m_pBonePet != 0) {
		g_resrcmng->Release(m_pBonePet, false);
		m_pBonePet = 0;
	}

	if (mask & 2) {
		for (std::vector<WPuppet *>::iterator it = m_aniPetList.begin(); it != m_aniPetList.end(); ++it) {
			if (*it != 0) {
				g_resrcmng->Release(*it, false);
			}
		}
		m_aniPetList.clear();
	}

	if (mask & 4) {
		for (int i = 0; i < m_numPets; i++) {
			if (g_resrcmng != 0 && m_pets[i] != 0) {
				g_resrcmng->Release(m_pets[i], false);
				m_pets[i] = 0;
			}
			for (int j = 0; j < 3; j++) {
				if (g_resrcmng != 0 && m_petTex[i][j] != 0) {
					g_resrcmng->Release(m_petTex[i][j]);
					m_petTex[i][j] = 0;
				}
			}
		}

		if (m_numPets != 0) {
			memset(m_pets, 0, sizeof(m_pets));
			memset(m_petTex, 0, sizeof(m_petTex));
			m_numPets = 0; m_unk28 = 0;
		}

		if (g_resrcmng != 0 && m_faceTex != 0) {
			g_resrcmng->Release(m_faceTex);
			m_faceTex = 0;
		}
		if (g_resrcmng != 0 && m_hairTex != 0) {
			g_resrcmng->Release(m_hairTex);
			m_hairTex = 0;
		}
		if (g_resrcmng != 0 && m_shirtsTex != 0) {
			g_resrcmng->Release(m_shirtsTex);
			m_shirtsTex = 0;
		}
	}
}

// partsinfo.cpp:1026
char *CPetFrame::GetMakeupTexture(void) {
	return m_partList.GetPart(m_makeupIndex)->m_pIff->texture[0];
}

// partsinfo.cpp:1032
WPuppet *CPetFrame::GetBonePet(void) {
	return m_pBonePet;
}

// partsinfo.cpp:1047
bool CPetFrame::Refresh(CPartTidList *pTidList) {
	if (!ResetFace(pTidList)) {
		return false;
	}
	if (!SetMakeup()) {
		return false;
	}
	return true;
}

// partsinfo.cpp:1074
bool CPetFrame::ResetFace(CPartTidList *pTidList) {
	IFF_STRUCT::sChar *pChar = ItemManager()->FindChar(m_tidChar);
	if (pChar == 0) {
		return false;
	}

	g_resrcmng->ChangeTexturePart(m_faceTex, pChar->m_faceTexture, 0, 0);

	SetHairClr(pTidList, pTidList->m_hairClr);

	return true;
}

// partsinfo.cpp:1094
void CPetFrame::AddAniPet(char *name) {
	for (std::vector<WPuppet *>::iterator it = m_aniPetList.begin(); it != m_aniPetList.end(); ++it) {
		WPuppet *pPet = *it;
		if (_stricmp(pPet->GetPuppetName(), name) == 0) {
			return;
		}
	}

	WPuppet *pPet = g_resrcmng->GetPuppet(name, false, true, false);
	if (pPet == 0) {
		return;
	}

	m_aniPetList.push_back(pPet);
}

// partsinfo.cpp:1116
std::vector<WPuppet *> &CPetFrame::GetAniPetList(void) {
	return m_aniPetList;
}

// partsinfo.cpp:1121
w_motion_data *CPetFrame::FindMotionData(const char *name, bool setCur, WPuppet **ppPet) {
	for (std::vector<WPuppet *>::iterator it = m_aniPetList.begin(); it != m_aniPetList.end(); ++it) {
		w_motion_data *pData = (*it)->FindMotionData(name);
		if (pData != 0) {
			if (ppPet != 0) {
				*ppPet = *it;
			}
			if (setCur) {
				m_pCurAniPet = *it;
			}
			return pData;
		}
	}
	return 0;
}

// partsinfo.cpp:1145
int CPetFrame::GetMotionCountIncludeName(const char *motionName, const char *exceptName) {
	int count = 0;
	for (std::vector<WPuppet *>::iterator it = m_aniPetList.begin(); it != m_aniPetList.end(); ++it) {
		count = (*it)->GetMotionCountIncludeName(motionName, exceptName);
		if (count > 0) {
			break;
		}
	}
	return count;
}

// partsinfo.cpp:1166
WPuppet *CPetFrame::GetCurAniPet(void) {
	return m_pCurAniPet;
}

// partsinfo.cpp:1171
void CPetFrame::ApplyBones(WBoneSet *pBoneSet, const WMatrix &mat) {
	m_pBonePet->ApplyBones(pBoneSet, mat);
}

// partsinfo.cpp:1176
void CPetFrame::Transform(WView *pView, float time, int flag) {
	m_pBonePet->Transform(pView, time, flag);
}

// partsinfo.cpp:1181
void CPetFrame::UpdateBound(bool b) {
	m_pBonePet->UpdateBound(b, false);
}

// partsinfo.cpp:1186
void CPetFrame::UpdateLightSource(LightSet *pLightSet, bool b, WScene *pScene) {
	m_pBonePet->UpdateLightSource(pLightSet, b, pScene);
}

// partsinfo.cpp:1191
void CPetFrame::SetAlpha(unsigned char alpha) {
	m_pBonePet->SetAlpha(alpha);
}

// partsinfo.cpp:1196
void CPetFrame::SetAlpha(char *name, unsigned char alpha) {
	m_pBonePet->SetAlpha(name, alpha);
}

// partsinfo.cpp:1201
void CPetFrame::Render(WView *pView, bool b, float f, bool b1, bool b2, bool b3) {
	m_pBonePet->Render(pView, b, f, b1, b2, b3);
}

// partsinfo.cpp:1206
bool CPetFrame::HasMPet(char *name) {
	for (int i = 0; i < m_numPets; i++) {
		if (m_pets[i] != 0 && _stricmp(m_pets[i]->GetPuppetName(), name) == 0) {
			return true;
		}
	}
	return false;
}

// partsinfo.cpp:1219
void CPetFrame::SetInvisible(bool invisible) {
	if (m_pBonePet != 0) {
		m_pBonePet->GetRootBone()->SetInvisible(true, invisible);
		if (!invisible) {
			ApplyHideMask(true);
		}
	}
}

// partsinfo.cpp:1230
unsigned long CPetFrame::SetPart(CPartTidList *pTidList, unsigned long tid, unsigned long id) {
	ApplyHideMask(false);
	unsigned long mask = m_partList.SetPart(pTidList, tid, id);
	if (mask != 0) {
		SetPartPets(mask);
		SetHairClr(0, pTidList->m_hairClr);
		SetShirtsClr(0, pTidList->m_shirtsClr);
		m_unk1B4 = true; m_pBonePet->EndMeshBoneWork(); ApplyHideMask(true);
		return mask;
	}
	return 0;
}

// partsinfo.cpp:1251
bool CPetFrame::SetAuxPart(unsigned char index, unsigned long tid) {
	if (index >= 5) {
		return false;
	}
	m_auxParts[index] = tid;
	return true;
}

// partsinfo.cpp:1262
bool CPetFrame::FindAuxPart(unsigned long tid) {
	for (int i = 0; i < 5; i++) {
		if (m_auxParts[i] == tid) {
			return true;
		}
	}
	return false;
}

namespace FIXHAIR {

// A hair style a character keeps whatever hair part it wears.
struct sFixItem {
	int m_charType;          // 0x0
	unsigned long m_tid;     // 0x4, the hair part's type id
	std::string m_texture;   // 0x8
};

// partsinfo.cpp:1315
sFixItem exTable[] = {
	{ 0, 0x8006031, "m_hair_02-01.jpg" },
	{ 0, 0x8006032, "m_hair_02-01.jpg" },
	{ 5, 0x8168009, "2#e_hair_07-07.jpg" },
};

} // namespace FIXHAIR

// partsinfo.cpp:1323
int IsFixedHairStyle(unsigned long tid) {
	for (int i = 0; i < 3; i++) {
		if (FIXHAIR::exTable[i].m_tid == tid) {
			return i;
		}
	}
	return -1;
}

// partsinfo.cpp:1602
bool CPetFrame::SetMakeup(void) {
	if (m_hasMakeup) {
		if (!m_pBonePet->ChangeTexturePart(m_faceTex, GetMakeupTexture(), 0, 0)) {
			return false;
		}
	}
	return true;
}

// partsinfo.cpp:1615
void CPetFrame::CalcBVolume(void) {
	if (m_pBonePet == 0) {
		return;
	}
	std::vector<WPuppet *>::iterator it = m_aniPetList.begin();
	if (it == m_aniPetList.end()) {
		m_pBonePet->CalcBound(0, true);
		return;
	}
	m_pBonePet->CalcBound(true, *it, true, 0, 0);
	for (++it; it != m_aniPetList.end(); ++it) {
		m_pBonePet->CalcBound(false, *it, true, 0, 0);
	}
}

// partsinfo.cpp:1636
void CPetFrame::EndSetPart(void) {
	m_pBonePet->EndMeshBoneWork();
	ApplyHideMask(true);
}

// partsinfo.cpp:1643
void CPetFrame::ApplyHideMask(bool b) {
	for (int i = 0; i < m_numPets; i++) {
		if (m_partList.m_parts[i].m_tid == 0 || m_partList.m_parts[i].m_pIff == 0) {
			continue;
		}
		IFF_STRUCT::sPart *pIff = m_partList.m_parts[i].m_pIff;
		if (pIff->meshHideMask == 0) {
			continue;
		}
		for (int j = 0; j < m_numPets; j++) {
			if ((pIff->meshHideMask & (1 << j)) == 0 || i == j) {
				continue;
			}
			if (m_partList.m_parts[j].m_tid == 0 || m_partList.m_parts[j].m_pIff == 0 || m_pets[j] == 0) {
				continue;
			}
			WBone *pMesh = m_pets[j]->FindMeshBone(0);
			while (pMesh != 0) {
				WBone *pBone = m_pBonePet->FindBone(pMesh->GetBoneName());
				if (pBone != 0) {
					pBone->SetInvisible(false, b);
				}
				pMesh = m_pets[j]->FindMeshBone(pMesh);
			}
		}
	}
}

// partsinfo.cpp:1703
bool CPetFrame::ChangeTexturePart(char *animName, char *texName, Bitmap **ppBmp, tagRECT *pRc) {
	w_face_animation *pAnim = FindFaceAnim(animName);
	if (pAnim == 0) {
		return false;
	}
	return g_resrcmng->ChangeTexturePart(pAnim->filename, texName, ppBmp, pRc);
}

// partsinfo.cpp:1729
void CPetFrame::ChangeTexturePart(char *animName, Bitmap *pBmp, const tagRECT &rc) {
	w_face_animation *pAnim = FindFaceAnim(animName);
	if (pAnim != 0) {
		g_resrcmng->ChangeTexturePart(pAnim->filename, pBmp, rc);
	}
}

// partsinfo.cpp:1743
void CPetFrame::ChangeTexture(char *srcName, char *dstName) {
	int hSrc = g_resrcmng->LoadTexture(srcName, 0, 0, 0);
	int hDst = g_resrcmng->LoadTexture(dstName, 0, 0, 0);
	if (hSrc != 0 && hDst != 0) {
		m_pBonePet->ChangeTextureByHandle(hDst, hSrc, 0);
	}
}

// partsinfo.cpp:1766
w_face_animation *CPetFrame::FindFaceAnim(char *animName) {
	w_face_animation *pAnim = 0;
	for (int i = 0; i < m_numPets; i++) {
		if (m_pets[i] != 0) {
			pAnim = m_pets[i]->FindFaceAnim(animName);
			if (pAnim != 0) {
				break;
			}
		}
	}
	return pAnim;
}
