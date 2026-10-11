// Original: d:\build\custom\temp\code\client\projectg\partsinfo.h
#ifndef PG_CLIENT_PROJECTG_PARTSINFO_H
#define PG_CLIENT_PROJECTG_PARTSINFO_H

#include "..\..\defines.h"

#include <vector>

class Bitmap;
class CPartTidList;
class WBoneSet;
class WMatrix;
class WPuppet;
class WScene;
class WView;
struct LightSet;
struct tagRECT;
struct w_face_animation;
struct w_motion_data;

namespace IFF_STRUCT {
struct sPart;
}

// One element of CPartList::m_parts: a worn part, its IFF record and its
// item id.
struct sPart {
	// partsinfo.h:16
	sPart(void) : m_tid(0), m_pIff(0), m_id(0) {}

	unsigned long m_tid;            // 0x0
	IFF_STRUCT::sPart *m_pIff;      // 0x4
	unsigned long m_id;             // 0x8, only used for local content 0x5F
};

BEGIN_LAYOUT_CHECK(sPart)
	CHECK_FIELD(sPart, m_tid, 0x0);
	CHECK_FIELD(sPart, m_pIff, 0x4);
	CHECK_FIELD(sPart, m_id, 0x8);
	CHECK_SIZE(sPart, 0xC);
END_LAYOUT_CHECK

class CPartList {
	LAYOUT_CHECK_FRIEND;
	friend class CPetFrame;  // ApplyHideMask walks m_parts
public:
	CPartList(void);                                                       // partsinfo.cpp:24
	~CPartList(void);                                                      // partsinfo.cpp:28
	bool Build(const CPartTidList *pTidList);                              // partsinfo.cpp:32
	unsigned char GetNumTotalParts(void) const;                            // partsinfo.h:41
	// partsinfo.h:53
	const sPart *GetPart(unsigned char index) const {
		if (index >= m_numTotalParts) {
			return 0;
		}
		return &m_parts[index];
	}
	bool SetHairClr(CPartTidList *pTidList, unsigned char clr);           // partsinfo.cpp:442
	bool SetShirtsClr(CPartTidList *pTidList, unsigned char clr);         // partsinfo.cpp:458
	bool IsIdentical(CPartTidList *pTidList);                              // partsinfo.cpp:474
	bool FindPart(unsigned long tid) const;                                // partsinfo.cpp:505
	unsigned long SetPart(CPartTidList *pTidList, unsigned long tid, unsigned long id); // partsinfo.cpp:73

protected:
	bool SetPart(unsigned long tid, unsigned long id);                     // partsinfo.cpp:113
	void EmptyList(unsigned long mask);                                    // partsinfo.cpp:322
	bool CheckSubParts(void);                                              // partsinfo.cpp:343
	bool IsEmpty(unsigned long mask, unsigned char except);                // partsinfo.cpp:426

private:
	unsigned long m_tidChar;        // 0x00
	unsigned char m_numDefaultParts; // 0x04, the slots that fall back to the character's default part
	unsigned char m_numTotalParts;  // 0x05
	unsigned char m_hairClr;        // 0x06
	unsigned char m_shirtsClr;      // 0x07
	sPart m_parts[24];              // 0x08
};

BEGIN_LAYOUT_CHECK(CPartList)
	CHECK_FIELD(CPartList, m_tidChar, 0x0);
	CHECK_FIELD(CPartList, m_numDefaultParts, 0x4);
	CHECK_FIELD(CPartList, m_numTotalParts, 0x5);
	CHECK_FIELD(CPartList, m_hairClr, 0x6);
	CHECK_FIELD(CPartList, m_shirtsClr, 0x7);
	CHECK_FIELD(CPartList, m_parts, 0x8);
	CHECK_SIZE(CPartList, 0x128);
END_LAYOUT_CHECK

class CPetFrame {
	LAYOUT_CHECK_FRIEND;
	friend class CExhibition;  // ParseBoundingBoxScript reads m_partList
public:
	CPetFrame(void);          // partsinfo.cpp:521
	~CPetFrame(void);         // partsinfo.cpp:528
	int GetFaceTex(void);     // partsinfo.h:108
	unsigned long GetTidChar(void);     // partsinfo.h:109
	void AddAniPet(char *name);                                            // partsinfo.cpp:1094
	char *GetMakeupTexture(void);                                          // partsinfo.cpp:1026
	WPuppet *GetBonePet(void);                                             // partsinfo.cpp:1032
	std::vector<WPuppet *> &GetAniPetList(void);                          // partsinfo.cpp:1116
	w_motion_data *FindMotionData(const char *name, bool setCur, WPuppet **ppPet); // partsinfo.cpp:1121
	int GetMotionCountIncludeName(const char *motionName, const char *exceptName); // partsinfo.cpp:1145
	WPuppet *GetCurAniPet(void);                                           // partsinfo.cpp:1166
	void ApplyBones(WBoneSet *pBoneSet, const WMatrix &mat);               // partsinfo.cpp:1171
	void Transform(WView *pView, float time, int flag);                    // partsinfo.cpp:1176
	void UpdateBound(bool b);                                              // partsinfo.cpp:1181
	void UpdateLightSource(LightSet *pLightSet, bool b, WScene *pScene);   // partsinfo.cpp:1186
	void SetAlpha(unsigned char alpha);                                    // partsinfo.cpp:1191
	void SetAlpha(char *name, unsigned char alpha);                        // partsinfo.cpp:1196
	void Render(WView *pView, bool b, float f, bool b1, bool b2, bool b3); // partsinfo.cpp:1201
	bool HasMPet(char *name);                                              // partsinfo.cpp:1206
	void SetInvisible(bool invisible);                                     // partsinfo.cpp:1219
	bool Build(const CPartTidList *pTidList, unsigned long *pAuxParts, bool, bool, int); // partsinfo.cpp:594
	unsigned long SetPart(CPartTidList *pTidList, unsigned long tid, unsigned long id); // partsinfo.cpp:1230
	bool SetAuxPart(unsigned char index, unsigned long tid);               // partsinfo.cpp:1251
	bool FindAuxPart(unsigned long tid);                                   // partsinfo.cpp:1262
	bool Refresh(CPartTidList *pTidList);                                  // partsinfo.cpp:1047
	bool ResetFace(CPartTidList *pTidList);                                // partsinfo.cpp:1074
	bool SetMakeup(void);                                                  // partsinfo.cpp:1602
	void CalcBVolume(void);                                                // partsinfo.cpp:1615
	bool ChangeTexturePart(char *animName, char *texName, Bitmap **ppBmp, tagRECT *pRc); // partsinfo.cpp:1703
	void ChangeTexturePart(char *animName, Bitmap *pBmp, const tagRECT &rc);            // partsinfo.cpp:1729
	void ChangeTexture(char *srcName, char *dstName);                      // partsinfo.cpp:1743
	bool SetHairClr(CPartTidList *pTidList, unsigned char clr);
	bool SetShirtsClr(CPartTidList *pTidList, unsigned char clr);

protected:
	void ReleasePets(int mask);                                            // partsinfo.cpp:533
	void ApplyHideMask(bool b);
	void SetPartPets(unsigned long mask);
	void EndSetPart(void);                                                 // partsinfo.cpp:1636
	w_face_animation *FindFaceAnim(char *animName);                        // partsinfo.cpp:1766

private:
	unsigned long m_tidChar;     // 0x00
	WPuppet *m_pBonePet;         // 0x04
	int m_hairTex;               // 0x08, the recoloured hair texture (SetHairClr)
	int m_shirtsTex;             // 0x0C, the recoloured shirts texture (SetShirtsClr)
	int m_faceTex;               // 0x10
	std::vector<WPuppet *> m_aniPetList; // 0x14
	WPuppet *m_pCurAniPet;       // 0x24
	unsigned long m_unk28;       // 0x28
	int m_numPets;               // 0x2C
	int m_makeupIndex;           // 0x30, the part carrying the makeup texture
	WPuppet *m_pets[24];         // 0x34
	int m_petTex[24][3];         // 0x94, texture handles of each pet, released by ReleasePets
	bool m_unk1B4;               // 0x1B4
	bool m_hasMakeup;            // 0x1B5
	CPartList m_partList;        // 0x1B8
	unsigned long m_auxParts[5]; // 0x2E0
};

BEGIN_LAYOUT_CHECK(CPetFrame)
	CHECK_FIELD(CPetFrame, m_tidChar, 0x0);
	CHECK_FIELD(CPetFrame, m_pBonePet, 0x4);
	CHECK_FIELD(CPetFrame, m_faceTex, 0x10);
	CHECK_FIELD(CPetFrame, m_aniPetList, 0x14);
	CHECK_FIELD(CPetFrame, m_pCurAniPet, 0x24);
	CHECK_FIELD(CPetFrame, m_numPets, 0x2C);
	CHECK_FIELD(CPetFrame, m_makeupIndex, 0x30);
	CHECK_FIELD(CPetFrame, m_pets, 0x34);
	CHECK_FIELD(CPetFrame, m_hasMakeup, 0x1B5);
	CHECK_FIELD(CPetFrame, m_partList, 0x1B8);
	CHECK_FIELD(CPetFrame, m_auxParts, 0x2E0);
	CHECK_SIZE(CPetFrame, 0x2F4);
END_LAYOUT_CHECK

// golfplayer.obj emits CPetFrame::GetFaceTex out of line: golfplayer.cpp
// defines GOLFPLAYER_PARTSINFO_INLINE empty before its includes and gets the
// out-of-line copy from here; every other unit inlines it.
#ifndef GOLFPLAYER_PARTSINFO_INLINE
#define GOLFPLAYER_PARTSINFO_INLINE inline
#endif

#ifdef GOLFPLAYER_PARTSINFO_INLINE
// partsinfo.h:108
GOLFPLAYER_PARTSINFO_INLINE int CPetFrame::GetFaceTex(void) {
	return m_faceTex;
}
#endif

#endif // PG_CLIENT_PROJECTG_PARTSINFO_H
