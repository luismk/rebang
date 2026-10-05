#pragma once

#include <string>
#include <vector>

#include "../../shared/classdefine.h"

class CPartTidList;

struct sPart
{
	unsigned long tid;
	IFF_STRUCT::sPart* pPart;
	unsigned long uid;
	sPart()
		: tid(0), pPart(NULL), uid(0)
	{
	}
};

class CPartList
{
public:
	CPartList();
	~CPartList();

	bool Build(const CPartTidList* pTids);
	unsigned long SetPart(CPartTidList* pTids, unsigned long tid,
		unsigned long uid);
	bool SetHairClr(CPartTidList* pTids, unsigned char color);
	bool SetShirtsClr(CPartTidList* pTids, unsigned char color);
	bool IsIdentical(CPartTidList* pTids);
	bool FindPart(unsigned long tid) const;

	unsigned char GetNumTotalParts() const { return m_partNum; }

	const sPart* GetPart(unsigned char index) const
	{
		if (index >= m_partNum)
			return NULL;

		return &m_part[index];
	}

protected:
	bool SetPart(unsigned long tid, unsigned long uid);
	void EmptyList(unsigned long mask);
	bool CheckSubParts();
	bool IsEmpty(unsigned long mask, unsigned char except);

public:
	unsigned long m_tidChar;
	unsigned char m_defPartNum;
	unsigned char m_partNum;
	unsigned char m_hairColor;
	unsigned char m_shirtsColor;
	sPart m_part[24];
};

class CPetFrame
{
public:
	CPetFrame();
	~CPetFrame();

	bool Build(const CPartTidList* pTids, unsigned long* pAuxParts, bool bColor,
		bool bAniPet, int face);
	bool Refresh(CPartTidList* pTids);
	bool ResetFace(CPartTidList* pTids);
	unsigned long SetPart(CPartTidList* pTids, unsigned long tid,
		unsigned long uid);
	bool SetAuxPart(unsigned char index, unsigned long tid);
	bool FindAuxPart(unsigned long tid);
	bool SetHairClr(CPartTidList* pTids, unsigned char color);
	bool SetShirtsClr(CPartTidList* pTids, unsigned char color);
	bool SetMakeup();
	char* GetMakeupTexture();
	void AddAniPet(char* name);
	WPuppet* GetBonePet();
	std::vector<WPuppet*>& GetAniPetList();
	WPuppet* GetCurAniPet();
	w_motion_data* FindMotionData(const char* name, bool bSelect,
		WPuppet** ppPet);
	int GetMotionCountIncludeName(const char* name, const char* include);
	void CalcBVolume();
	bool HasMPet(char* name);
	void SetInvisible(bool bInvisible);
	void ChangeTexture(char* from, char* to);
	bool ChangeTexturePart(char* anim, char* tex, Bitmap** ppBitmap,
		tagRECT* pRect);
	void ChangeTexturePart(char* anim, Bitmap* pBitmap, const tagRECT& rect);

	int GetFaceTex() { return m_faceTex; }
	unsigned long GetTidChar() { return m_tidChar; }

	void ApplyBones(WBoneSet* pBoneSet, const WMatrix& mat);
	void Transform(WView* pView, float delta, int flag);
	void UpdateBound(bool bForce);
	void UpdateLightSource(LightSet* pLight, bool bUpdate, WScene* pScene);
	void SetAlpha(unsigned char alpha);
	void SetAlpha(char* name, unsigned char alpha);
	void Render(WView* pView, bool bShadow, float alpha, bool b1, bool b2,
		bool b3);

protected:
	void ReleasePets(int mask);
	void SetPartPets(unsigned long mask);
	void EndSetPart();
	void ApplyHideMask(bool bApply);
	w_face_animation* FindFaceAnim(char* name);

	unsigned long m_tidChar;
	WPuppet* m_pBonePet;
	int m_hairTex;
	int m_shirtsTex;
	int m_faceTex;
	std::vector<WPuppet*> m_aniPetList;
	WPuppet* m_pCurAniPet;
	int m_defPartNum;
	int m_partNum;
	int m_makeupIndex;
	WPuppet* m_pPartPet[24];
	int m_partTex[24][3];
	bool m_bShirtsClr;
	bool m_bMakeup;
	CPartList m_partList;
	unsigned long m_auxPart[5];

public:
	CPartList& GetPartList() { return m_partList; }
};

namespace FIXHAIR
{
	struct sFixItem
	{
		int charIndex;
		unsigned long tid;
		std::string texture;
	};

	extern sFixItem exTable[3];
}
