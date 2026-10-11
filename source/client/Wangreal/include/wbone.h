// Original: d:\build\custom\temp\code\client\wangreal\include\wbone.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WBONE_H
#define PG_CLIENT_WANGREAL_INCLUDE_WBONE_H

#include <map>
#include <vector>

#include "..\..\defines.h"
#include "wmath.h"
#include "wflag.h"
#include "wscene.h"
#include "wvideo.h"

class WBone;
class WBlockModel;
class WBoneSet;
class WPuppet;
class WScene;
class WView;
class cFile;
struct WxBatchGrp;
struct WxBatch;
struct WxBatchState;
struct _iobuf;
struct w_3d_object;
struct w_common_bone_data;
struct w_keyframe;
struct w_vertex;
struct w_keyframe_scale;
struct w_pet_texture_info;
struct w_pet_tri_point;
struct w_pet_vertex;
struct WTVertex;

// A position key of a bone's animation (PDB layout, size 0x10). Its header
// placement is not attested by line info; it sits beside WBone, which owns
// the key arrays.
struct w_keyframe_pos {
	float t;                            // 0x0
	WVector pos;                        // 0x4
};
CHECK_SIZE(w_keyframe_pos, 0x10);
CHECK_FIELD(w_keyframe_pos, t, 0x0);
CHECK_FIELD(w_keyframe_pos, pos, 0x4);

// A rotation key of a bone's animation (PDB layout, size 0x14).
struct w_keyframe_rot {
	float t;                            // 0x0
	WQuat rot;                          // 0x4
};
CHECK_SIZE(w_keyframe_rot, 0x14);
CHECK_FIELD(w_keyframe_rot, t, 0x0);
CHECK_FIELD(w_keyframe_rot, rot, 0x4);

// A triangle of a loaded model, input of WBone::SetMesh (PDB layout, size
// 0x84). Header placement not attested by line info.
struct w_faces {
	struct w_faces_info {
		w_3d_object* object;            // 0x0
		w_vertex* gvertex;              // 0x4
		WBone* bone;                    // 0x8
		WVector norm;                   // 0xc
		int vindex;                     // 0x18
		float tu;                       // 0x1c
		float tv;                       // 0x20
	};

	w_faces_info p[3];                  // 0x0
	unsigned long diffuse;              // 0x6c
	int texhandle;                      // 0x70
	int subNum;                         // 0x74
	int mapType;                        // 0x78
	int mapFlags;                       // 0x7c
	int xiMapFlags2;                    // 0x80
};
CHECK_SIZE(w_faces::w_faces_info, 0x24);
CHECK_FIELD(w_faces::w_faces_info, object, 0x0);
CHECK_FIELD(w_faces::w_faces_info, gvertex, 0x4);
CHECK_FIELD(w_faces::w_faces_info, bone, 0x8);
CHECK_FIELD(w_faces::w_faces_info, norm, 0xc);
CHECK_FIELD(w_faces::w_faces_info, vindex, 0x18);
CHECK_FIELD(w_faces::w_faces_info, tu, 0x1c);
CHECK_FIELD(w_faces::w_faces_info, tv, 0x20);
CHECK_SIZE(w_faces, 0x84);
CHECK_FIELD(w_faces, p, 0x0);
CHECK_FIELD(w_faces, diffuse, 0x6c);
CHECK_FIELD(w_faces, texhandle, 0x70);
CHECK_FIELD(w_faces, subNum, 0x74);
CHECK_FIELD(w_faces, mapType, 0x78);
CHECK_FIELD(w_faces, mapFlags, 0x7c);
CHECK_FIELD(w_faces, xiMapFlags2, 0x80);

// One mesh of a bone (PDB layout, size 0x110). Its header placement is not
// attested by line info; it sits beside WBone, its only owner.
struct w_mesh {
	int vtxNum;                         // 0x0
	int rigidNum;                       // 0x4
	int blendedRigidNum;                // 0x8
	int blendedTotalNum;                // 0xc
	WVector* vecList;                   // 0x10
	WVector* normList;                  // 0x14
	WVector* blendedVecList;            // 0x18
	float* blendWeightList;             // 0x1c
	WVector* blendedNormalList;         // 0x20
	unsigned long* originalVtxColorList;  // 0x24
	int maxBoneNum;                     // 0x28
	float* weightList;                  // 0x2c
	int indexNum;                       // 0x30
	unsigned short* indexList;          // 0x34
	int originalTexHandle;              // 0x38
	float originalTu;                   // 0x3c
	float originalTv;                   // 0x40
	float originalScaleu;               // 0x44
	float originalScalev;               // 0x48
	unsigned long diffuse;              // 0x4c
	float (*uvBackup)[2];               // 0x50
	float (*uvData)[2];                 // 0x54
	bool bEnv;                          // 0x58
	bool bSpec;                         // 0x59
	int group;                          // 0x5c
	int flags;                          // 0x60
	w_mesh* next;                       // 0x64
	float alpha;                        // 0x68
	WBone** boneList;                   // 0x6c
	WBone** blendedBoneList;            // 0x70
	unsigned long* vtxColorList;        // 0x74
	unsigned long** vtxColorPtrList;    // 0x78
	WVector* myNormalList;              // 0x7c
	WVector* myBlendedNormalList;       // 0x80
	int texHandle;                      // 0x84
	float tu;                           // 0x88
	float tv;                           // 0x8c
	float scaleu;                       // 0x90
	float scalev;                       // 0x94
	int clipFlag;                       // 0x98
	int drawFlag;                       // 0x9c
	WxBatchGrp* xpBatchGrp;             // 0xa0
	WxBatch* xpBatch;                   // 0xa4
	int xiBaseVtxIdx;                   // 0xa8
	int xiBaseIdxIdx;                   // 0xac
	int xiDrawFlag2;                    // 0xb0
	WBone* aabbBone;                    // 0xb4
	Waabb localAabb;                    // 0xb8
	char mpetName[64];                  // 0xd0
};
CHECK_SIZE(w_mesh, 0x110);
CHECK_FIELD(w_mesh, vtxNum, 0x0);
CHECK_FIELD(w_mesh, rigidNum, 0x4);
CHECK_FIELD(w_mesh, blendedRigidNum, 0x8);
CHECK_FIELD(w_mesh, blendedTotalNum, 0xc);
CHECK_FIELD(w_mesh, vecList, 0x10);
CHECK_FIELD(w_mesh, normList, 0x14);
CHECK_FIELD(w_mesh, blendedVecList, 0x18);
CHECK_FIELD(w_mesh, blendWeightList, 0x1c);
CHECK_FIELD(w_mesh, blendedNormalList, 0x20);
CHECK_FIELD(w_mesh, originalVtxColorList, 0x24);
CHECK_FIELD(w_mesh, maxBoneNum, 0x28);
CHECK_FIELD(w_mesh, weightList, 0x2c);
CHECK_FIELD(w_mesh, indexNum, 0x30);
CHECK_FIELD(w_mesh, indexList, 0x34);
CHECK_FIELD(w_mesh, originalTexHandle, 0x38);
CHECK_FIELD(w_mesh, originalTu, 0x3c);
CHECK_FIELD(w_mesh, originalTv, 0x40);
CHECK_FIELD(w_mesh, originalScaleu, 0x44);
CHECK_FIELD(w_mesh, originalScalev, 0x48);
CHECK_FIELD(w_mesh, diffuse, 0x4c);
CHECK_FIELD(w_mesh, uvBackup, 0x50);
CHECK_FIELD(w_mesh, uvData, 0x54);
CHECK_FIELD(w_mesh, bEnv, 0x58);
CHECK_FIELD(w_mesh, bSpec, 0x59);
CHECK_FIELD(w_mesh, group, 0x5c);
CHECK_FIELD(w_mesh, flags, 0x60);
CHECK_FIELD(w_mesh, next, 0x64);
CHECK_FIELD(w_mesh, alpha, 0x68);
CHECK_FIELD(w_mesh, boneList, 0x6c);
CHECK_FIELD(w_mesh, blendedBoneList, 0x70);
CHECK_FIELD(w_mesh, vtxColorList, 0x74);
CHECK_FIELD(w_mesh, vtxColorPtrList, 0x78);
CHECK_FIELD(w_mesh, myNormalList, 0x7c);
CHECK_FIELD(w_mesh, myBlendedNormalList, 0x80);
CHECK_FIELD(w_mesh, texHandle, 0x84);
CHECK_FIELD(w_mesh, tu, 0x88);
CHECK_FIELD(w_mesh, tv, 0x8c);
CHECK_FIELD(w_mesh, scaleu, 0x90);
CHECK_FIELD(w_mesh, scalev, 0x94);
CHECK_FIELD(w_mesh, clipFlag, 0x98);
CHECK_FIELD(w_mesh, drawFlag, 0x9c);
CHECK_FIELD(w_mesh, xpBatchGrp, 0xa0);
CHECK_FIELD(w_mesh, xpBatch, 0xa4);
CHECK_FIELD(w_mesh, xiBaseVtxIdx, 0xa8);
CHECK_FIELD(w_mesh, xiBaseIdxIdx, 0xac);
CHECK_FIELD(w_mesh, xiDrawFlag2, 0xb0);
CHECK_FIELD(w_mesh, aabbBone, 0xb4);
CHECK_FIELD(w_mesh, localAabb, 0xb8);
CHECK_FIELD(w_mesh, mpetName, 0xd0);

// A node of a puppet's bone hierarchy (PDB layout, size 0x1b4).
// WxTnLBuffer and WPuppet read the private members directly (their code has
// no wbone.h accessor line marks), so they are friends.
class WBone {
	friend class WxTnLBuffer;
	friend class WPuppet;
	LAYOUT_CHECK_FRIEND;

public:
	// Spring model of a bone's skinned vertices (wbone_physics.cpp; PDB
	// layout, size 0x18).
	struct w_bone_physic_link {
		struct w_bone_physic_link_element {
			int offset;                     // 0x0
			float length;                   // 0x4
		};

		int vtxnum;                         // 0x0
		WVector* pos;                       // 0x4
		WVector* vel;                       // 0x8
		WVector** list;                     // 0xc
		w_bone_physic_link_element* link;   // 0x10
		int* linkoffset;                    // 0x14
	};
	class w_normalmerge;

	enum w_bone_flag {
		HIDEBONE = 1,
		SELFILLUM = 2,
		HASRIGIDVTX = 4,
		VISIBLE = 8,
		CLIPPING = 16,
		CLONED_MESH = 32,
		CLONED_ANI = 64,
		ROTATEBONE = 512,
		MOVEBONE = 1024,
		TRANSFORM = 4096,
		PHYSICSMODEL = 8192,
		COLORPERVERTEX = 16384,
		CHECK_SHADOW = 32768,
		MESHBONE = 65536,
		MIRRORED = 131072,
		INVISIBLE = 262144,
		SCALEBONE = 524288
	};
	enum RenderFlag {
		NOCLIP = 1,
		NOUPDATEMAT = 2
	};

	WBone();
	~WBone();
	WBone* MakeClone(WBone*);
	w_mesh* CopyMesh(w_mesh*);
	void ResetMeshBone(WBone*, bool);
	void CopyBone(w_common_bone_data*);
	void SetName(char*);
	void SetParent(WBone*);
	void SetNext(WBone*, bool);
	void ReleaseChild(WBone*);
	void ReleaseLink() {
		m_child = 0;
		m_next = 0;
	}
	void SetLocalMatrix(const WQuat& quat, const WVector& pivot) {    // wbone.h:226
		m_localMat = quat;
		m_localMat.pivot = pivot;
	}
	void SetLocalMatrix(const WMatrix&);
	void SetBasicMatrix(const WMatrix&);
	void CopyBasicMatrix(WBone*, bool);
	void SetMatrix(const WMatrix&, WView*);
	void InsertBoneSetList(float, WBoneSet*, int);
	void ApplyBoneSetList(WBoneSet*);
	void OptimizeBoneSet(float (*)[2], int, int);
	void SetKeyframe(w_keyframe*, float, float);
	void ReplaceKeyframe(WBone*, float);
	void SetMesh(w_pet_vertex*, int, w_pet_tri_point*, w_pet_texture_info**, WBone**, int);
	void SetMesh(w_faces*, int);
	void SetBoneMesh(w_pet_vertex*, int, w_pet_tri_point*, w_pet_texture_info**, WBone**, int);
	int GetMeshForFaceAnimation(unsigned char, w_mesh**, int);
	int GetMeshForFaceAnimation(int, w_mesh**, int);
	w_mesh* GetIndexedmesh(w_pet_vertex*, w_pet_tri_point*, WBone**, w_pet_texture_info**, int, int*);
	w_mesh* GetIndexedmesh(w_faces**, int);
	w_mesh* GetMesh() { return m_mesh; }                    // wbone.h:257
	void CombineVtxColor(bool);
	unsigned long* FindVColor(const WVector&, const WVector&);
	WVector Transform(const WVector& v) { return v * m_matrix; }   // wbone.h:272
	void Transform(const WMatrix&, WView*, float, int);
	void SetRenderMode(int);
	void SetAlpha(unsigned char, int);
	unsigned char GetAlpha() { return m_alpha; }            // wbone.h:282
	void RestoreVtxColor();
	void SetLightLine(float);
	void SetLight(LightSet*);
	void DisableFog();
	void EnableAlphaBlend();
	const LightSet& GetLight() { return m_light; }          // wbone.h:288
	void CalcLight(LightSet*, bool, int, WScene*);
	void CalcLight(WScene*, int);
	void ResetLight();
protected:
	void CalcLight_SelfIllum();
	void CalcLight_Point(LightSet*, int, WScene*);
	void CalcLight_Directional_Per_Bone(LightSet*, int, WScene*);
	void CalcLight_Directional(LightSet*, int, WScene*);
	void CalcLight_Diffuse_Equals_Ambient(LightSet*, int);
public:
	void CalcENVCoord(WView*, w_mesh*);
	void CalcHilightCoord(WView*, const LightSet&, w_mesh*);
	void SetLightMode(unsigned long);
	void Render(WView*, const LightSet&);
	void RenderHierarchy(WView*, float);
	void RenderNormals(WView*, float);
	void ApplyScale(float);
	void SetBoundBox(const Waabb, bool);
	void FixNormal(const WMatrix&);
	void MergeMesh(WBone*, WBone*);
	bool CheckRigidVtx(WBone*);
	WVector GetDeltaVec(float);
	WVector GetKeyPos(float);
	WQuat GetKeyRot(float);
	WSphere GetBoundSphere() { return m_bound_sphere; }
	Waabb GetBoundBox();
	bool IsVisible() { return m_flag.GetFlag(VISIBLE); }    // wbone.h:329
	bool HasRigidVtx();
	bool IsHidden() { return m_flag.GetFlag(HIDEBONE); }    // wbone.h:331
	bool IsSelfIllum() { return m_flag.GetFlag(SELFILLUM); }    // wbone.h:332
	void SetRigidVtxFlag();
	void SetInvisible(bool, bool);
	bool IsInvisible() { return m_flag.GetFlag(INVISIBLE); }   // wbone.h:335
	bool HasAnimation();                                    // wbone.h:340
	WBone* FindBone(int id);
	WBone* FindBone(const char* name, unsigned long hashCode);
	WBone* FindBoneByTail(const char*);
	WBone* FindFakeBodyBone(w_pet_vertex*, int);
	int GetConvexArea(WBlockModel*);
	void ApplySpecularMap(int);
	void SetIDNumber(int);
	int GetID() { return m_id; }                            // wbone.h:354
	int SaveBoneName(_iobuf*, int, char*);
	int SaveAnimationKey(_iobuf*, int, float (*)[2], int);
	int SetID(int, bool, char*);
	void CountBones(int&);
	float LoadAnimationKey(cFile*);
	void GetName(char**, int&);
	char* GetBoneName() { return m_name; }                  // wbone.h:366
	char* GetBoneMotionName();
	void GetBoneInfo(int*);
	int GetOverlapBox(float*, WBone*);
	void GetTotalOverlapBox(WVector*, WVector*, WMatrix*, bool);
	void Rotate(float, int, bool);
	void Move(WVector&, bool);
	WVector& GetPivot() { return m_basicMat.pivot; }        // wbone.h:377
	WMatrix& GetLocalMatrix() { return m_localMat; }        // wbone.h:378
	WMatrix& GetBasicMatrix() { return m_basicMat; }        // wbone.h:379
	WQuat& GetBasicRot();                                   // wbone.h:380
	WMatrix& GetMatrix() { return m_matrix; }               // wbone.h:381
	void HideBone(bool);
	void ShowBone(bool);
	void SetSelfIllum(bool, bool);
	void SetSelfIllumColor(unsigned long, bool);
	unsigned long GetSelfIllumColor();
	void ToggleSelfIllum() { m_flag.Turn(SELFILLUM, !IsSelfIllum()); }    // wbone.h:389
	WBone* GetChild() const { return m_child; }             // wbone.h:391
	WBone* GetNext() const { return m_next; }               // wbone.h:392
	WBone* GetParent() const { return m_parent; }           // wbone.h:393
	WBone* GetRoot();
	void SetPhysicsModel(int);
	void ResetPhysicsModel();
	void UpdatePhysicsModel(float, const WVector&);
protected:
	void ReleasePhysicsModel();
	int FindLink(int, w_mesh*, w_bone_physic_link::w_bone_physic_link_element*, int);
private:
	bool CheckSuitableForKeyframe(float, float, float (*)[2], int, bool);
	void ClearMesh(w_mesh*);
	int OptimizeRotKey(float (*)[2], int, w_keyframe_rot**);
	int OptimizeScaleKey(float (*)[2], int, w_keyframe_scale**);
	int OptimizePosKey(float (*)[2], int, w_keyframe_pos**);
	static int __cdecl SortByTextureHandle(const void*, const void*);
	char* GetNameSubBip();
	void CountVtxBuff(int);
	WTVertex* GetVtxBuff(int);
	WTVertex* GetVtxBuff();
public:
	void AllocVTX(int);
	void SetVtxColor(unsigned long);
	bool CompareBasicMatrix(WBone*);
	void xBuildTransfMatPtrList(std::vector<const WMatrix*>&) const;
	void xSetAlpha(unsigned char, int);
	void xRender(WView*, WxBatchState*, bool, int, int);
	WBone* AttachMeshBone(WBone*, int, int);
	void ChangeTextureByHandle(int, int, bool, bool);
	bool ChangeTexture(const char*, int);
	bool ChangeTexture(const char*, int, int);
	void ResetMyNormalList();
	void GatherNormalMergeInfo(w_normalmerge&) const;
	void SetMirrored(bool);
	void SetScale(float);
	float GetBoneScale();
	static void CalcMeshAABB(WBone&, w_mesh&);
	static bool IsMeshVisible(WBone*, w_mesh*);
	bool CheckMeshBoneAllVertexRigid() const;

protected:
	w_bone_physic_link* m_physic;       // 0x0
	w_mesh* m_mesh;                     // 0x4

private:
	char m_name[64];                    // 0x8
	unsigned long m_hashCode;           // 0x48
	char* m_motion_bone_name;           // 0x4c
	WQuat m_basicQuat;                  // 0x50
	WMatrix m_basicMat;                 // 0x60
	WSphere m_bound_sphere;             // 0x90
	Waabb m_bound_aabb;                 // 0xa0
	w_keyframe_pos* m_keyPos;           // 0xb8
	int m_keyPosNum;                    // 0xbc
	w_keyframe_rot* m_keyRot;           // 0xc0
	int m_keyRotNum;                    // 0xc4
	int m_keyFlag;                      // 0xc8
	WBone* m_parent;                    // 0xcc
	WBone* m_child;                     // 0xd0
	WBone* m_next;                      // 0xd4
	float m_lightLine;                  // 0xd8
	int m_rendMode;                     // 0xdc
	int m_id;                           // 0xe0
	unsigned long m_selfillumColor;     // 0xe4
	unsigned char m_alpha;              // 0xe8
	float m_scale;                      // 0xec
	WFlags m_flag;                      // 0xf0
	WMatrix m_localMat;                 // 0xf4
	WMatrix m_matrix;                   // 0x124
	WMatrix m_rotateMat;                // 0x154
	WVector m_moveVec;                  // 0x184
	LightSet m_light;                   // 0x190
	unsigned long m_vtxColor;           // 0x1ac
	static WTVertex* m_vtxList;
	static WVector* m_vecList;
	static int m_vtx_len;
	static int m_vtx_count;
	float m_bonescale;                  // 0x1b0
};
CHECK_SIZE(WBone, 0x1b4);
BEGIN_LAYOUT_CHECK(WBone)
	CHECK_FIELD(WBone, m_physic, 0x0);
	CHECK_FIELD(WBone, m_mesh, 0x4);
	CHECK_FIELD(WBone, m_name, 0x8);
	CHECK_FIELD(WBone, m_hashCode, 0x48);
	CHECK_FIELD(WBone, m_motion_bone_name, 0x4c);
	CHECK_FIELD(WBone, m_basicQuat, 0x50);
	CHECK_FIELD(WBone, m_basicMat, 0x60);
	CHECK_FIELD(WBone, m_bound_sphere, 0x90);
	CHECK_FIELD(WBone, m_bound_aabb, 0xa0);
	CHECK_FIELD(WBone, m_keyPos, 0xb8);
	CHECK_FIELD(WBone, m_keyPosNum, 0xbc);
	CHECK_FIELD(WBone, m_keyRot, 0xc0);
	CHECK_FIELD(WBone, m_keyRotNum, 0xc4);
	CHECK_FIELD(WBone, m_keyFlag, 0xc8);
	CHECK_FIELD(WBone, m_parent, 0xcc);
	CHECK_FIELD(WBone, m_child, 0xd0);
	CHECK_FIELD(WBone, m_next, 0xd4);
	CHECK_FIELD(WBone, m_lightLine, 0xd8);
	CHECK_FIELD(WBone, m_rendMode, 0xdc);
	CHECK_FIELD(WBone, m_id, 0xe0);
	CHECK_FIELD(WBone, m_selfillumColor, 0xe4);
	CHECK_FIELD(WBone, m_alpha, 0xe8);
	CHECK_FIELD(WBone, m_scale, 0xec);
	CHECK_FIELD(WBone, m_flag, 0xf0);
	CHECK_FIELD(WBone, m_localMat, 0xf4);
	CHECK_FIELD(WBone, m_matrix, 0x124);
	CHECK_FIELD(WBone, m_rotateMat, 0x154);
	CHECK_FIELD(WBone, m_moveVec, 0x184);
	CHECK_FIELD(WBone, m_light, 0x190);
	CHECK_FIELD(WBone, m_vtxColor, 0x1ac);
	CHECK_FIELD(WBone, m_bonescale, 0x1b0);
END_LAYOUT_CHECK

CHECK_SIZE(WBone::w_bone_physic_link::w_bone_physic_link_element, 0x8);
CHECK_FIELD(WBone::w_bone_physic_link::w_bone_physic_link_element, offset, 0x0);
CHECK_FIELD(WBone::w_bone_physic_link::w_bone_physic_link_element, length, 0x4);
CHECK_SIZE(WBone::w_bone_physic_link, 0x18);
CHECK_FIELD(WBone::w_bone_physic_link, vtxnum, 0x0);
CHECK_FIELD(WBone::w_bone_physic_link, pos, 0x4);
CHECK_FIELD(WBone::w_bone_physic_link, vel, 0x8);
CHECK_FIELD(WBone::w_bone_physic_link, list, 0xc);
CHECK_FIELD(WBone::w_bone_physic_link, link, 0x10);
CHECK_FIELD(WBone::w_bone_physic_link, linkoffset, 0x14);

// Gathers the normals of coincident vertices to merge them (PDB layout,
// size 0xc).
class WBone::w_normalmerge {
	LAYOUT_CHECK_FRIEND;
public:
	struct N {
		WVector m_n;                        // 0x0
		std::vector<int> m_idxList;         // 0xc
		N(const WVector& n);                    // wbone.h:154
	};
	typedef std::vector<N> NLIST;
	typedef std::map<w_mesh*, NLIST> MLIST;
	struct P {
		WVector m_p;                        // 0x0
		MLIST m_meshList;                   // 0xc
		P(const WVector& p);                    // wbone.h:168
	};
	typedef std::vector<P> PLIST;
	typedef std::map<float, PLIST> WLIST;

	w_normalmerge(WPuppet&);
	void AddVertex(w_mesh*, int);
private:
	void MergeNormal();
	void NormalizeMergedNormals();
	void ApplyToBlendedNormalList();
	static N* Find(NLIST&, const WVector&);
	static P* Find(PLIST&, const WVector&);
	static void SetMyNormalsEqualToFirstNormal(w_mesh*, const std::vector<int>&);

	WLIST m_wlist;                          // 0x0
};
CHECK_SIZE(WBone::w_normalmerge::N, 0x1c);
CHECK_FIELD(WBone::w_normalmerge::N, m_n, 0x0);
CHECK_FIELD(WBone::w_normalmerge::N, m_idxList, 0xc);
CHECK_SIZE(WBone::w_normalmerge::P, 0x18);
CHECK_FIELD(WBone::w_normalmerge::P, m_p, 0x0);
CHECK_FIELD(WBone::w_normalmerge::P, m_meshList, 0xc);
CHECK_SIZE(WBone::w_normalmerge, 0xc);
BEGIN_LAYOUT_CHECK(WBone::w_normalmerge)
	CHECK_FIELD(WBone::w_normalmerge, m_wlist, 0x0);
END_LAYOUT_CHECK

// wbone.cpp defines WBONE_WBONE_INLINE empty before its includes: the
// original holds the out-of-line copies of these inlines in wbone's object;
// every other unit inlines them.
#ifndef WBONE_WBONE_INLINE
#define WBONE_WBONE_INLINE inline
#endif

// wbone.h:154
WBONE_WBONE_INLINE WBone::w_normalmerge::N::N(const WVector& n)
	: m_n(n) {
}

// wbone.h:168
WBONE_WBONE_INLINE WBone::w_normalmerge::P::P(const WVector& p)
	: m_p(p) {
}

// wbone.h:380
WBONE_WBONE_INLINE WQuat& WBone::GetBasicRot() {
	return m_basicQuat;
}

// wbone.h:523
WBONE_WBONE_INLINE WTVertex* WBone::GetVtxBuff() {
	return m_vtxList;
}

// wbone.h:524
WBONE_WBONE_INLINE WTVertex* WBone::GetVtxBuff(int index) {
	return m_vtxList + index;
}

// wpuppet.cpp defines WPUPPET_WBONE_INLINE empty before its includes: the
// original holds the out-of-line copies of these inlines in wpuppet's object;
// every other unit inlines them.
#ifndef WPUPPET_WBONE_INLINE
#define WPUPPET_WBONE_INLINE inline
#endif

// wbone.h:340
WPUPPET_WBONE_INLINE bool WBone::HasAnimation() {
	return (m_keyPos != 0 || m_keyRot != 0) ? true : false;
}

// wbone.h:353
WPUPPET_WBONE_INLINE void WBone::SetIDNumber(int id) {
	m_id = id;
}

#endif // PG_CLIENT_WANGREAL_INCLUDE_WBONE_H
