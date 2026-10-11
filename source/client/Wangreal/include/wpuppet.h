// Original: d:\build\custom\temp\code\client\wangreal\include\wpuppet.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WPUPPET_H
#define PG_CLIENT_WANGREAL_INCLUDE_WPUPPET_H

#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "wresource.h"
#include "wlist.h"
#include "wmath.h"

class Bitmap;
class WBone;
class WBlockModel;
class WBoneSet;
class WPlane;
class WScene;
class WView;
class cFile;
struct LightSet;
struct w_mesh;
struct w_pet_texture_info;
struct tagRECT;

// PDB layout, size 0x24.
struct w_motion_data {
	float s;            // 0x0
	float e;            // 0x4
	float end;          // 0x8
	int option1;        // 0xc
	float option2;      // 0x10
	char* applybone;    // 0x14
	char* name;         // 0x18
	char* nextmotion;   // 0x1c
	char ptr[1];        // 0x20
};
CHECK_SIZE(w_motion_data, 0x24);
CHECK_FIELD(w_motion_data, s, 0x0);
CHECK_FIELD(w_motion_data, e, 0x4);
CHECK_FIELD(w_motion_data, end, 0x8);
CHECK_FIELD(w_motion_data, option1, 0xc);
CHECK_FIELD(w_motion_data, option2, 0x10);
CHECK_FIELD(w_motion_data, applybone, 0x14);
CHECK_FIELD(w_motion_data, name, 0x18);
CHECK_FIELD(w_motion_data, nextmotion, 0x1c);
CHECK_FIELD(w_motion_data, ptr, 0x20);

// PDB layout, size 0x30.
struct w_bound_box_info {
	w_bound_box_info();

	Waabb aabb;             // 0x0
	WVector spherePivot;    // 0x18
	float sphereLength;     // 0x24
	char* option;           // 0x28
	char name[1];           // 0x2c
};
CHECK_SIZE(w_bound_box_info, 0x30);
CHECK_FIELD(w_bound_box_info, aabb, 0x0);
CHECK_FIELD(w_bound_box_info, spherePivot, 0x18);
CHECK_FIELD(w_bound_box_info, sphereLength, 0x24);
CHECK_FIELD(w_bound_box_info, option, 0x28);
CHECK_FIELD(w_bound_box_info, name, 0x2c);

// PDB layout, size 0x8.
struct w_bound_box {
	WBone* bone;                // 0x0
	w_bound_box_info* info;     // 0x4
};
CHECK_SIZE(w_bound_box, 0x8);
CHECK_FIELD(w_bound_box, bone, 0x0);
CHECK_FIELD(w_bound_box, info, 0x4);

// PDB layout, size 0x1c.
struct w_motion_addition {
	WList<w_motion_data*>* mdList;  // 0x0
	WBone* rootbone;                // 0x4
	float s;                        // 0x8
	float e;                        // 0xc
	float length;                   // 0x10
	float h_gap;                    // 0x14
	char name[1];                   // 0x18
};
CHECK_SIZE(w_motion_addition, 0x1c);
CHECK_FIELD(w_motion_addition, mdList, 0x0);
CHECK_FIELD(w_motion_addition, rootbone, 0x4);
CHECK_FIELD(w_motion_addition, s, 0x8);
CHECK_FIELD(w_motion_addition, e, 0xc);
CHECK_FIELD(w_motion_addition, length, 0x10);
CHECK_FIELD(w_motion_addition, h_gap, 0x14);
CHECK_FIELD(w_motion_addition, name, 0x18);

// PDB layout, size 0x68.
struct w_face_animation {
	w_mesh** mesh;          // 0x0
	int meshNum;            // 0x4
	int group;              // 0x8
	char name[32];          // 0xc
	char filename[32];      // 0x2c
	float tu;               // 0x4c
	float tv;               // 0x50
	float du;               // 0x54
	float dv;               // 0x58
	float scalex;           // 0x5c
	float scaley;           // 0x60
	int texHandle;          // 0x64
};
CHECK_SIZE(w_face_animation, 0x68);
CHECK_FIELD(w_face_animation, mesh, 0x0);
CHECK_FIELD(w_face_animation, meshNum, 0x4);
CHECK_FIELD(w_face_animation, group, 0x8);
CHECK_FIELD(w_face_animation, name, 0xc);
CHECK_FIELD(w_face_animation, filename, 0x2c);
CHECK_FIELD(w_face_animation, tu, 0x4c);
CHECK_FIELD(w_face_animation, tv, 0x50);
CHECK_FIELD(w_face_animation, du, 0x54);
CHECK_FIELD(w_face_animation, dv, 0x58);
CHECK_FIELD(w_face_animation, scalex, 0x5c);
CHECK_FIELD(w_face_animation, scaley, 0x60);
CHECK_FIELD(w_face_animation, texHandle, 0x64);

// PDB layout, size 0x3c.
struct w_share_pet_data {
	struct w_frame;

	char* petName;                          // 0x0
	WBone* rootbone;                        // 0x4
	float length;                           // 0x8
	int faceNum;                            // 0xc
	int numFramedata;                       // 0x10
	w_frame* frame;                         // 0x14
	Waabb bound;                            // 0x18
	WList<w_bound_box*>* bbList;            // 0x30
	WList<w_motion_data*>* mdList;          // 0x34
	WList<w_face_animation*>* fanimList;    // 0x38
};
CHECK_SIZE(w_share_pet_data, 0x3c);
CHECK_FIELD(w_share_pet_data, frame, 0x14);
CHECK_FIELD(w_share_pet_data, bound, 0x18);
CHECK_FIELD(w_share_pet_data, fanimList, 0x38);

// PDB layout, size 0xc.
struct w_share_pet_data::w_frame {
	int num;        // 0x0
	int check;      // 0x4
	char* data;     // 0x8
};
CHECK_SIZE(w_share_pet_data::w_frame, 0xc);
CHECK_FIELD(w_share_pet_data::w_frame, check, 0x4);
CHECK_FIELD(w_share_pet_data::w_frame, data, 0x8);
// PDB layout, size 0x1c8.
class WPuppet : public WResource {
	LAYOUT_CHECK_FRIEND;
	friend class CQuadTree;
	friend class CCreature;
public:
	struct w_tex_piece;
	struct w_pet_chunk;
	struct w_original_matrix_set;

	// Vtable after BaseObject's destructor (PDB): MakeClone, LoadPET,
	// UpdateLightSource, Render, xUpdateTnLBuffers, EndMeshBoneWork,
	// UpdateMeshBonePtr, DetachMeshBone.
	virtual ~WPuppet();
	virtual WPuppet* MakeClone(bool);
	virtual int LoadPET(char*, bool, int);
	virtual void UpdateLightSource(LightSet*, bool, WScene*);
	virtual void Render(WView*, bool, float, bool, bool, bool);
	virtual void xUpdateTnLBuffers(bool) { }           // wpuppet.h:495
	virtual void EndMeshBoneWork();
	virtual void UpdateMeshBonePtr(WPuppet*);

	WPuppet();
	void Share(w_share_pet_data*);
	void SetRenderMode(int);
	void SetLightLine(float);
	void SetLightMode(unsigned long);
	int GetLightMode() { return m_lightmode; }          // wpuppet.h:191
	void UpdateLightSource(WScene*);
	void Transform(WView*, float, int);
	void ApplyFaceAnimation(char*, float);
	void RenderHierarchy(WView*, bool, float, float);
	void RenderNormals(WView*, bool, float, float);
	WVector InvTransform(const WVector&);
	void ChangeTexture(char*, int);
	void ChangeTexturePart(int hDstTex, Bitmap* pBmp, const tagRECT& rc);
	bool ChangeTexturePart(int hDstTex, char* texName, Bitmap** ppBmp, tagRECT* pRc);
	void ChangeTexturePart(char* animName, Bitmap* pBmp, const tagRECT& rc);
	bool ChangeTexturePart(char* animName, char* texName, Bitmap** ppBmp, tagRECT* pRc);
	void InstantTexChange(int, int);
	void DisableFog();
	void EnableAlphaBlend();
	WBoneSet* GetBones(const char*, float, int, char*, bool);
	float GetMotionLength(const char* actionName);
	char* GetNextMotionName(const char* actionName);
	void ApplyBones(WBoneSet*);
	void ApplyBones(WBoneSet*, const WMatrix&);
	bool ChangeMesh(char*);
	bool AddMotion(const char*, char*);
	void DelAddMotion(const char*);
	void ClearAddMotion();
	void OptimizeBone();
	bool HaveCenterBoundBox();
	bool GetBoundBoxPlane(char*, WPlane*);
	bool GetAlignBoundBoxPlane(char*, WPlane*, Waabb*);
	bool DotContact(char*, const WVector&);
	w_bound_box* FindBoundBox(const char*);
	char* FindTexAniList(char*);
	w_face_animation* FindFaceAnim(char* animName);
	w_motion_data* FindMotionData(const char*);
	int GetMotionCountIncludeName(const char* motionName, const char* exceptName);
	w_motion_data* GetFirstMotionData();
	w_motion_data* GetNextMotionData();
	w_motion_addition* GetFirstAddedMotionList();
	w_motion_addition* GetNextAddedMotionList();
	void GetBoneName(char**, int&);
	WVector GetPivot(const char*);
	WMatrix GetRootMatrix();
	WVector GetDeltaVec(char*, float, float);
	WVector GetDeltaVec(char* animName, float t);
	WVector GetDeltaVec(float);
	WBone* FindBone(int id);
	WBone* FindBone(const char*);
	WBone* FindBoneByTail(const char*);
	bool CheckFrameData(int, int, int&, int&);
	float GetAnimationLength() { return m_aniLen; }   // wpuppet.h:275
	unsigned int GetFrameCount();
	char* GetFrameData(int);
	float GetFrameDataTime(int);
	bool GetFrameCheck(int);
	WMatrix& GetMatrix() { return m_mat; }              // wpuppet.h:280
	const WMatrix& GetCamMatrix(char*);
	float GetCamFOV(char*, WBoneSet*);
	WBone* GetRootBone() { return m_rootbone; }         // wpuppet.h:283
	int GetFaceNum() { return m_faceNum; }              // wpuppet.h:284
	void SetVtxColor(unsigned long);
	void SetSelfIllum(char*, bool);
	void SetSelfIllumColor(char*, unsigned long);
	void ShowBone(char*);
	void HideBone(char*);
	unsigned char GetAlpha(char*);
	void SetAlpha(char*, unsigned char);
	void SetAlpha(unsigned char);
	void ToggleSelfIllum(char*);
	void SetMirrored(bool);
	void AddCollisionObject(const char*, WBlockModel*);
	void UpdatePhysics(float, const WVector&);
	void SetPhysicsModel(const char*, int);
	void RotateBone(char*, float, int);
	void MoveBone(char*, WVector&);
	void CalcBound(bool, WPuppet*, bool, char*, Waabb*);
	void CalcBound(Waabb*, bool);
	const Waabb& GetBoundBox() { return m_bound; }                     // wpuppet.h:325
	Waabb GetTransformedBoundBox() const;
	void UpdateBound(bool, bool);
	void UpdateBSphere();
	const WSphere& GetBoundSphere() const { return m_boundSphere; }    // wpuppet.h:333
	void LockBoneSetPos(unsigned char);
	unsigned char GetBoneSetPos();
	void RenderBoundBox(const char*, WView*, unsigned long, float);
	const char* GetPuppetName() const { return m_petName; }            // wpuppet.h:345
	int GetRenderMode();
	void xBuildTransfMatPtrList();
	const std::vector<const WMatrix*>& xGetTransfMatPtrList() const { return m_xTransfMatPtrList; }  // wpuppet.h:497
	void ReplaceMeshBones(WPuppet* pOldMPet, WPuppet* pNewMPet, int hTex, int hOrgTex);
	void ChangeTextureByHandle(int, int, WPuppet*);
	WBone* FindMeshBone(WBone*);
	void ScaleBone(char*, float);
	bool CompareBasicMatrix(WPuppet*) const;
	static void AddOrginalMatrixPtrList(const std::string&, const std::vector<const WMatrix*>&);
	static bool HasOrginalMatrixList(const std::string&);
	static const std::vector<WMatrix>& GetOrginalMatrixList(const std::string&);
	static const std::vector<const WMatrix*>& GetOrginalMatrixPtrList(const std::string&);

	int m_lightmode;                                    // 0x2c
	WList<WBone*> m_phybone;                            // 0x30
protected:
	virtual void DetachMeshBone(WPuppet*);
	void FindMeshBone(WBone**, WBone**, const char*);
	void SetDefaultFaceAnimation();
	void SetBoundBox(char*, Waabb*, char*);
	WBoneSet* GetFreeBoneSetList(bool);
	void AttachMeshBone(WPuppet*, int, int);
	void MergeNormal();
private:
	void LoadPET(cFile*, bool, char*, int);
	void FreeBone(WBone*);
	void FixNormal();
	void MakeTemporaryCenterBox();
	w_pet_texture_info* LoadPET_Texture(cFile*, bool, char*, int*);
	void LoadPET_Mesh(cFile*, WBone*, w_pet_texture_info*, bool);
	WBone* LoadPET_Bone(cFile*, int);
	float LoadPET_Animation(cFile*, WBone*);
	void LoadPET_FaceAnim(cFile*, w_pet_texture_info*, WBone*, char*);
	void LoadPET_Frame(cFile*);
	WList<w_motion_data*>* LoadPET_Motion(cFile*, float);
	void LoadPET_Collision(cFile*);
	void MergeRootMotion(WList<w_motion_data*>*, WBone*);
	bool CheckStaticBone(WBone*);
	WBone* MergeBone(WBone*, WBone*);
	bool ReduceBone(WBone*);
	void GetDirectory(char* filename, char* directory);
	void GetName(char* filename, char* name);
	void SetTexPiece(char*, int, float, float, float, float);
	w_tex_piece* GetTexPiece(char* texname);
	void ClearTexPiece();
	static int SeekChunkFromPET(cFile* fp, char* str);

	static WList<w_tex_piece*>* m_tex_piece;
	static unsigned char ms_lockedBoneSetPos;
	static unsigned char ms_boneSetPos;
	static int box_v[8][3];
	static int box_p[6][4];
protected:

	WMatrix m_mat;                                      // 0x50
private:
	WList<w_motion_addition*> m_addedMotionList;        // 0x80
protected:
	int m_rendMode;                                     // 0xa0
	bool m_bClone;                                      // 0xa4
private:
	bool m_bHaveCenter;                                 // 0xa5
protected:
	w_share_pet_data::w_frame* m_frame;                 // 0xa8
	int m_framenum;                                     // 0xac
	char m_petName[64];                                 // 0xb0
	int m_faceNum;                                      // 0xf0
	Waabb m_bound;                                      // 0xf4
	WSphere m_boundSphere;                              // 0x10c
	w_bound_box m_BBox;                                 // 0x11c
private:
	WList<int>* m_storeTexList;                         // 0x124
	WList<w_tex_piece*> m_use_tex_piece;                // 0x128
public:
	WList<w_bound_box*> m_bbList;                       // 0x148
protected:
	WList<w_motion_data*>* m_mdList;                    // 0x168
	WList<w_face_animation*> m_fanimList;               // 0x16c
	WList<WBone*> m_boneList;                           // 0x18c
	float m_aniLen;                                     // 0x1ac
	int m_iPetType;                                     // 0x1b0
	WBone* m_rootbone;                                  // 0x1b4
	std::vector<const WMatrix*> m_xTransfMatPtrList;    // 0x1b8

	static std::map<std::string, w_original_matrix_set> ms_xOrginalMatrixList;
#ifdef PG_7XX
	// The 7xx .pet VERS value (0xFFFE0000 when absent). The matching build
	// (pre-7xx) has no versioned loader and omits this member.
	unsigned int m_version;
#endif
};
CHECK_SIZE(WPuppet, 0x1c8);

// PDB layout, size 0x1c.
struct WPuppet::w_tex_piece {
	float pu;           // 0x0
	float pv;           // 0x4
	float scalex;       // 0x8
	float scaley;       // 0xc
	int texHandle;      // 0x10
	int count;          // 0x14
	char name[1];       // 0x18
};
CHECK_SIZE(WPuppet::w_tex_piece, 0x1c);
CHECK_FIELD(WPuppet::w_tex_piece, count, 0x14);
CHECK_FIELD(WPuppet::w_tex_piece, name, 0x18);

// PDB layout, size 0x20.
struct WPuppet::w_original_matrix_set {
	std::vector<const WMatrix*> matPtrList;     // 0x0
	std::vector<WMatrix> matList;               // 0x10

	w_original_matrix_set() {
	}
	// wpuppet.h:461
	w_original_matrix_set(const w_original_matrix_set& other) {
		if (other.matPtrList.size() > 0) {
			matPtrList.resize(other.matPtrList.size());
			matList = other.matList;
			for (int i = 0; i < (int)other.matPtrList.size(); ++i) {
				matPtrList[i] = &matList[i];
			}
		}
	}
};
CHECK_SIZE(WPuppet::w_original_matrix_set, 0x20);
CHECK_FIELD(WPuppet::w_original_matrix_set, matPtrList, 0x0);
CHECK_FIELD(WPuppet::w_original_matrix_set, matList, 0x10);

// PDB layout, size 0x8.
struct WPuppet::w_pet_chunk {
	char id[4];             // 0x0
	// A 32-bit field of the .pet record; `unsigned long` is 8 bytes on LP64
	// and the 8-byte chunk read would leave it unset (docs/PORTABILITY.md).
	unsigned int length;    // 0x4
};
CHECK_SIZE(WPuppet::w_pet_chunk, 0x8);
CHECK_FIELD(WPuppet::w_pet_chunk, length, 0x4);
BEGIN_LAYOUT_CHECK(WPuppet)
	CHECK_FIELD(WPuppet, m_lightmode, 0x2c);
	CHECK_FIELD(WPuppet, m_phybone, 0x30);
	CHECK_FIELD(WPuppet, m_mat, 0x50);
	CHECK_FIELD(WPuppet, m_addedMotionList, 0x80);
	CHECK_FIELD(WPuppet, m_rendMode, 0xa0);
	CHECK_FIELD(WPuppet, m_bClone, 0xa4);
	CHECK_FIELD(WPuppet, m_bHaveCenter, 0xa5);
	CHECK_FIELD(WPuppet, m_frame, 0xa8);
	CHECK_FIELD(WPuppet, m_framenum, 0xac);
	CHECK_FIELD(WPuppet, m_petName, 0xb0);
	CHECK_FIELD(WPuppet, m_faceNum, 0xf0);
	CHECK_FIELD(WPuppet, m_bound, 0xf4);
	CHECK_FIELD(WPuppet, m_boundSphere, 0x10c);
	CHECK_FIELD(WPuppet, m_BBox, 0x11c);
	CHECK_FIELD(WPuppet, m_storeTexList, 0x124);
	CHECK_FIELD(WPuppet, m_use_tex_piece, 0x128);
	CHECK_FIELD(WPuppet, m_bbList, 0x148);
	CHECK_FIELD(WPuppet, m_mdList, 0x168);
	CHECK_FIELD(WPuppet, m_fanimList, 0x16c);
	CHECK_FIELD(WPuppet, m_boneList, 0x18c);
	CHECK_FIELD(WPuppet, m_aniLen, 0x1ac);
	CHECK_FIELD(WPuppet, m_iPetType, 0x1b0);
	CHECK_FIELD(WPuppet, m_rootbone, 0x1b4);
	CHECK_FIELD(WPuppet, m_xTransfMatPtrList, 0x1b8);
END_LAYOUT_CHECK

// Exhibition.cpp defines EXHIBITION_WPUPPET_INLINE empty before its includes:
// the original holds the out-of-line copy of this inline in Exhibition's
// object; every other unit inlines it.
#ifndef EXHIBITION_WPUPPET_INLINE
#define EXHIBITION_WPUPPET_INLINE inline
#endif

// wpuppet.h:335
EXHIBITION_WPUPPET_INLINE void WPuppet::LockBoneSetPos(unsigned char n) {
	ms_lockedBoneSetPos = n;
}

// camerawork.cpp defines CAMERAWORK_WPUPPET_INLINE empty before its includes:
// the original holds the out-of-line copy of this inline in camerawork's
// object; every other unit inlines it.
#ifndef CAMERAWORK_WPUPPET_INLINE
#define CAMERAWORK_WPUPPET_INLINE inline
#endif

// wpuppet.h:277
CAMERAWORK_WPUPPET_INLINE char* WPuppet::GetFrameData(int index) {
	return m_frame[index].data;
}

// Petal.cpp defines PETAL_WPUPPET_INLINE empty before its includes: the
// original holds the out-of-line copy of this inline in Petal's object;
// every other unit inlines it.
#ifndef PETAL_WPUPPET_INLINE
#define PETAL_WPUPPET_INLINE inline
#endif

// wpuppet.h:260
PETAL_WPUPPET_INLINE w_motion_data* WPuppet::GetFirstMotionData() {
	return m_mdList->Start();
}

#endif // PG_CLIENT_WANGREAL_INCLUDE_WPUPPET_H
