// Original: d:\build\custom\temp\code\client\wangreal\source\wbone.cpp
#define WBONE_WMATH_INLINE
#define WBONE_WBONE_INLINE
#define WBONE_WUTIL_INLINE
#define WBONE_WVIEW_INLINE
#include <stdio.h>
#include <string.h>
#include <list>
#include <algorithm>

#include "../include/wbone.h"
#include "../include/wblockmodel.h"
#include "../include/wboneset.h"
#include "../include/wpetfile.h"
#include "../include/wpuppet.h"
#include "../include/wvideo.h"
#include "../include/wview.h"
#include "../include/wutil.h"
#include "../include/westpak.h"
#include "../include/wxtnlbuffer.h"

// wmath.h:86 (not yet in wmath.h, owned by its steward)
inline int WisZero(const float& f, float eps) {
	return Wabs(f) < eps;
}

WTVertex* WBone::m_vtxList = 0;
WVector* WBone::m_vecList = 0;
int WBone::m_vtx_len = 0;
int WBone::m_vtx_count = 0;

// A keyframe of a loaded .pet animation (PDB layout, size 0x18).
struct w_keyframe {
	float t;                            // 0x0
	WQuat* q;                           // 0x4
	WVector* v;                         // 0x8
	float* scale;                       // 0xc
	w_keyframe* next;                   // 0x10
	w_keyframe* prev;                   // 0x14
};
CHECK_SIZE(w_keyframe, 0x18);
CHECK_FIELD(w_keyframe, t, 0x0);
CHECK_FIELD(w_keyframe, q, 0x4);
CHECK_FIELD(w_keyframe, v, 0x8);
CHECK_FIELD(w_keyframe, scale, 0xc);
CHECK_FIELD(w_keyframe, next, 0x10);
CHECK_FIELD(w_keyframe, prev, 0x14);

// A rigid-vertex link of a loaded model (PDB layout, size 0x2c).
struct w_rigid {
	char objname[32];                   // 0x0
	w_3d_object* obj;                   // 0x20
	WBone* bone;                        // 0x24
	float blendWeight;                  // 0x28
};
CHECK_SIZE(w_rigid, 0x2c);
CHECK_FIELD(w_rigid, objname, 0x0);
CHECK_FIELD(w_rigid, obj, 0x20);
CHECK_FIELD(w_rigid, bone, 0x24);
CHECK_FIELD(w_rigid, blendWeight, 0x28);

// A vertex of a loaded model (PDB layout, size 0x3c).
struct w_vertex {
	WVector v;                          // 0x0
	float weight;                       // 0xc
	char objname[32];                   // 0x10
	w_rigid* rigidVtx;                  // 0x30
	int rigidNum;                       // 0x34
	w_3d_object* obj;                   // 0x38
};
CHECK_SIZE(w_vertex, 0x3c);
CHECK_FIELD(w_vertex, v, 0x0);
CHECK_FIELD(w_vertex, weight, 0xc);
CHECK_FIELD(w_vertex, objname, 0x10);
CHECK_FIELD(w_vertex, rigidVtx, 0x30);
CHECK_FIELD(w_vertex, rigidNum, 0x34);
CHECK_FIELD(w_vertex, obj, 0x38);

// A node of a loaded model's object tree (PDB layout, size 0xb0).
struct w_3d_object {
	w_3d_object* parent;                // 0x0
	w_3d_object* child;                 // 0x4
	w_3d_object* next;                  // 0x8
	w_vertex* gvertex;                  // 0xc
	w_faces* gfaces;                    // 0x10
	WMatrix matrix;                     // 0x14
	char name[40];                      // 0x44
	char parentName[40];                // 0x6c
	int nvertex;                        // 0x94
	int nfaces;                         // 0x98
	int first;                          // 0x9c
	int type;                           // 0xa0
	WBone* bone;                        // 0xa4
	w_3d_object* list;                  // 0xa8
	w_keyframe* keyframe;               // 0xac
};
CHECK_SIZE(w_3d_object, 0xb0);
CHECK_FIELD(w_3d_object, parent, 0x0);
CHECK_FIELD(w_3d_object, child, 0x4);
CHECK_FIELD(w_3d_object, next, 0x8);
CHECK_FIELD(w_3d_object, gvertex, 0xc);
CHECK_FIELD(w_3d_object, gfaces, 0x10);
CHECK_FIELD(w_3d_object, matrix, 0x14);
CHECK_FIELD(w_3d_object, name, 0x44);
CHECK_FIELD(w_3d_object, parentName, 0x6c);
CHECK_FIELD(w_3d_object, nvertex, 0x94);
CHECK_FIELD(w_3d_object, nfaces, 0x98);
CHECK_FIELD(w_3d_object, first, 0x9c);
CHECK_FIELD(w_3d_object, type, 0xa0);
CHECK_FIELD(w_3d_object, bone, 0xa4);
CHECK_FIELD(w_3d_object, list, 0xa8);
CHECK_FIELD(w_3d_object, keyframe, 0xac);

// Everything a cloned bone shares with its source (PDB layout, size 0xbc).
struct w_common_bone_data {
	int xid;                            // 0x0
	char name[64];                      // 0x4
	WSphere boundsphere;                // 0x44
	WMatrix basicMat;                   // 0x54
	Waabb aabb;                         // 0x84
	w_keyframe_pos* keyPos;             // 0x9c
	w_keyframe_rot* keyRot;             // 0xa0
	w_keyframe_scale* keyScale;         // 0xa4
	int keyPosNum;                      // 0xa8
	int keyRotNum;                      // 0xac
	int keyScaleNum;                    // 0xb0
	int keyFlag;                        // 0xb4
	w_mesh* mesh;                       // 0xb8
};
CHECK_SIZE(w_common_bone_data, 0xbc);
CHECK_FIELD(w_common_bone_data, xid, 0x0);
CHECK_FIELD(w_common_bone_data, name, 0x4);
CHECK_FIELD(w_common_bone_data, boundsphere, 0x44);
CHECK_FIELD(w_common_bone_data, basicMat, 0x54);
CHECK_FIELD(w_common_bone_data, aabb, 0x84);
CHECK_FIELD(w_common_bone_data, keyPos, 0x9c);
CHECK_FIELD(w_common_bone_data, keyRot, 0xa0);
CHECK_FIELD(w_common_bone_data, keyScale, 0xa4);
CHECK_FIELD(w_common_bone_data, keyPosNum, 0xa8);
CHECK_FIELD(w_common_bone_data, keyRotNum, 0xac);
CHECK_FIELD(w_common_bone_data, keyScaleNum, 0xb0);
CHECK_FIELD(w_common_bone_data, keyFlag, 0xb4);
CHECK_FIELD(w_common_bone_data, mesh, 0xb8);

// wbone.cpp:34
WBone::WBone() {
	m_parent = 0;
	m_child = 0;
	m_next = 0;
	m_rendMode = 0;
	m_scale = 1.0f;
	m_lightLine = 0.0f;
	m_keyPos = 0;
	m_keyPosNum = 0;
	m_keyRot = 0;
	m_keyRotNum = 0;
	m_keyFlag = 0;
	m_bound_sphere = WSphere(WVector::ZERO, 0.0f);
	m_mesh = 0;
	m_selfillumColor = 0xffffff;
	m_alpha = 0xff;
	m_basicQuat.Reset();
	m_basicMat.Reset();
	m_localMat.Reset();
	m_rotateMat.Reset();
	m_moveVec.Reset();
	m_matrix.Reset();
	m_light.type = 2;
	m_light.nearone = WVector::UNIT_NEG_Y;
	m_bound_aabb = Waabb(WVector::ZERO, WVector(0.0001f, 0.0001f, 0.0001f));
	m_vtxColor = 0xffffffff;
	CountVtxBuff(1);
}

// wbone.cpp:77
WBone::~WBone() {
	if (m_mesh != 0) {
		ClearMesh(m_mesh);
		if (m_mesh != 0) {
			delete m_mesh;
			m_mesh = 0;
		}
	}
	if (!m_flag.GetFlag(CLONED_ANI)) {
		if (m_keyPos != 0) {
			delete[] m_keyPos;
		}
		if (m_keyRot != 0) {
			delete[] m_keyRot;
		}
	}
	if (m_next != 0) {
		delete m_next;
		m_next = 0;
	}
	if (m_child != 0) {
		delete m_child;
		m_child = 0;
	}
	m_vtx_count--;
	if (m_vtx_count == 0) {
		if (m_vtxList != 0) {
			delete[] m_vtxList;
			m_vtxList = 0;
			m_vtx_len = 0;
		}
	}
	m_bonescale = 1.0f;
}

// wbone.cpp:107
w_mesh* WBone::CopyMesh(w_mesh* source) {
	w_mesh* mesh = new w_mesh;
	memcpy(mesh, source, sizeof(w_mesh));
	mesh->next = 0;
	mesh->drawFlag = mesh->drawFlag & 0xfffff800 | source->texHandle;
	mesh->alpha = 1.0f;
	mesh->diffuse = source->diffuse;
	mesh->vtxColorList = new unsigned long[mesh->vtxNum];
	int i;
	for (i = 0; i < source->vtxNum; ++i) {
		mesh->vtxColorList[i] = source->diffuse;
	}
	if (mesh->rigidNum > 0) {
		mesh->boneList = (WBone**)g_mem.Alloc(mesh->rigidNum * sizeof(WBone*));
		mesh->vtxColorPtrList = (unsigned long**)g_mem.Alloc(mesh->rigidNum * sizeof(unsigned long*));
		memcpy(mesh->boneList, source->boneList, mesh->rigidNum * sizeof(WBone*));
	}
	if (mesh->blendedRigidNum > 0) {
		mesh->blendedBoneList = (WBone**)g_mem.Alloc(mesh->blendedTotalNum * sizeof(WBone*));
		memcpy(mesh->blendedBoneList, source->blendedBoneList, mesh->blendedTotalNum * sizeof(WBone*));
	}
	if (mesh->group != 0) {
		mesh->uvData = new float[mesh->vtxNum][2];
		memcpy(mesh->uvData, source->uvData, mesh->vtxNum * 8);
		if (source->uvBackup != 0) {
			mesh->uvBackup = new float[mesh->vtxNum][2];
			memcpy(mesh->uvBackup, source->uvBackup, mesh->vtxNum * 8);
		}
	}
	if ((((unsigned long)m_flag) >> 16) & 1) {
		mesh->myNormalList = new WVector[mesh->vtxNum];
		memcpy(mesh->myNormalList, mesh->normList, mesh->vtxNum * 12);
		if (mesh->blendedTotalNum > 0) {
			mesh->myBlendedNormalList = new WVector[mesh->blendedTotalNum];
			memcpy(mesh->myBlendedNormalList, mesh->blendedNormalList, mesh->blendedTotalNum * 12);
		}
	}
	return mesh;
}

// wbone.cpp:174
void WBone::CopyBone(w_common_bone_data* data) {
	m_id = data->xid;
	SetName(data->name);
	m_bound_sphere = data->boundsphere;
	m_bound_aabb = data->aabb;
	m_keyPos = data->keyPos;
	m_keyPosNum = data->keyPosNum;
	m_keyRot = data->keyRot;
	m_keyRotNum = data->keyRotNum;
	m_keyFlag = data->keyFlag;
	SetBasicMatrix(data->basicMat);
	if (data->mesh != 0) {
		m_mesh = CopyMesh(data->mesh);
		if (m_mesh->rigidNum != 0 || m_mesh->blendedRigidNum != 0) {
			m_flag.Enable(HASRIGIDVTX);
		}
		w_mesh* last = m_mesh;
		for (w_mesh* source = data->mesh->next; source != 0; source = source->next) {
			last->next = CopyMesh(source);
			if (last->next->rigidNum != 0 || last->next->blendedRigidNum != 0) {
				m_flag.Enable(HASRIGIDVTX);
			}
			last = last->next;
		}
	}
	m_flag.Enable(CLONED_MESH | CLONED_ANI);
}

// wbone.cpp:217
void WBone::ClearMesh(w_mesh* mesh) {
	if (mesh->next != 0) {
		ClearMesh(mesh->next);
		if (mesh->next != 0) {
			delete mesh->next;
			mesh->next = 0;
		}
	}
	if (!m_flag.GetFlag(CLONED_MESH)) {
		if (mesh->indexList != 0) {
			delete[] mesh->indexList;
			mesh->indexList = 0;
		}
		if (mesh->normList != 0) {
			delete[] mesh->normList;
			mesh->normList = 0;
		}
		if (mesh->vecList != 0) {
			delete[] mesh->vecList;
			mesh->vecList = 0;
		}
		if (mesh->weightList != 0) {
			delete[] mesh->weightList;
			mesh->weightList = 0;
		}
		if (mesh->originalVtxColorList != 0) {
			delete[] mesh->originalVtxColorList;
			mesh->originalVtxColorList = 0;
		}
		if (mesh->uvData != 0) {
			delete[] mesh->uvData;
			mesh->uvData = 0;
		}
		if (mesh->uvBackup != 0) {
			delete[] mesh->uvBackup;
			mesh->uvBackup = 0;
		}
	} else if (mesh->group != 0) {
		if (mesh->uvData != 0) {
			delete[] mesh->uvData;
			mesh->uvData = 0;
		}
		if (mesh->uvBackup != 0) {
			delete[] mesh->uvBackup;
			mesh->uvBackup = 0;
		}
	}
	if (mesh->vtxColorList != 0) {
		delete[] mesh->vtxColorList;
		mesh->vtxColorList = 0;
	}
	if (mesh->myBlendedNormalList != 0) {
		delete[] mesh->myBlendedNormalList;
		mesh->myBlendedNormalList = 0;
	}
	if (mesh->myNormalList != 0) {
		delete[] mesh->myNormalList;
		mesh->myNormalList = 0;
	}
	if (mesh->blendedTotalNum > 0) {
		if (!m_flag.GetFlag(CLONED_MESH)) {
			g_mem.Free(mesh->blendWeightList);
			if (mesh->blendedVecList != 0) {
				delete[] mesh->blendedVecList;
				mesh->blendedVecList = 0;
			}
			if (mesh->blendedNormalList != 0) {
				delete[] mesh->blendedNormalList;
				mesh->blendedNormalList = 0;
			}
		}
		g_mem.Free(mesh->blendedBoneList);
	}
	if (mesh->rigidNum > 0) {
		g_mem.Free(mesh->boneList);
		g_mem.Free(mesh->vtxColorPtrList);
	}
}

// wbone.cpp:284
WBone* WBone::MakeClone(WBone* parent) {
	if (m_flag.GetFlag(MESHBONE)) {
		return 0;
	}

	w_common_bone_data common;
	common.xid = m_id;
	strcpy(common.name, m_name);
	common.basicMat = m_basicMat;
	common.boundsphere = m_bound_sphere;
	common.aabb = m_bound_aabb;
	common.keyPos = m_keyPos;
	common.keyPosNum = m_keyPosNum;
	common.keyRot = m_keyRot;
	common.keyRotNum = m_keyRotNum;
	common.keyFlag = m_keyFlag;
	common.mesh = m_mesh;

	WBone* clone = new WBone;
	clone->CopyBone(&common);
	clone->SetParent(parent);

	if (m_next != 0) {
		if (parent != 0) {
			m_next->MakeClone(parent);
		} else {
			clone->SetNext(m_next->MakeClone(0), true);
		}
	}
	if (m_child != 0) {
		m_child->MakeClone(clone);
	}
	return clone;
}

// wbone.cpp:324
void WBone::MergeMesh(WBone* bone, WBone* root) {
	WMatrix matrix;
	matrix = bone->m_matrix * ~m_matrix;
	if (m_mesh != 0) {
		w_mesh* last;
		for (last = m_mesh; last->next != 0; last = last->next) {
		}
		last->next = bone->m_mesh;
	} else {
		m_mesh = bone->m_mesh;
	}
	for (w_mesh* mesh = bone->m_mesh; mesh != 0; mesh = mesh->next) {
		int i;
		for (i = 0; i < mesh->vtxNum; ++i) {
			WVector& vec = mesh->vecList[i];
			vec = WVector(matrix.xx * vec.x + matrix.xz * vec.z + matrix.xy * vec.y + matrix.xm,
				matrix.yx * vec.x + matrix.yz * vec.z + matrix.yy * vec.y + matrix.ym,
				matrix.zx * vec.x + matrix.zz * vec.z + matrix.zy * vec.y + matrix.zm);
		}
		for (i = 0; i < mesh->rigidNum; ++i) {
			WBone*& rigid = mesh->boneList[i];
			rigid = root->FindBone(rigid->m_name, 0);
		}
		for (i = 0; i < mesh->blendedTotalNum; ++i) {
			WBone*& rigid = mesh->blendedBoneList[i];
			rigid = root->FindBone(rigid->m_name, 0);
		}
	}
	bone->m_mesh = 0;
}

// wbone.cpp:363
bool WBone::CheckRigidVtx(WBone* bone) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		int i;
		for (i = 0; i < mesh->rigidNum; i++) {
			if (mesh->boneList[i] == bone) {
				return true;
			}
		}
		for (i = 0; i < mesh->blendedTotalNum; i++) {
			if (mesh->blendedBoneList[i] == bone) {
				return true;
			}
		}
	}
	if (m_next != 0 && m_next->CheckRigidVtx(bone)) {
		return true;
	}
	if (m_child != 0 && m_child->CheckRigidVtx(bone)) {
		return true;
	}
	return false;
}

// wbone.cpp:393
void WBone::ResetMeshBone(WBone* root, bool child) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		int i;
		for (i = 0; i < mesh->rigidNum; i++) {
			WBone*& bone = mesh->boneList[i];
			if (bone != 0) {
				bone = root->FindBone(bone->m_name, 0);
			}
		}
		for (i = 0; i < mesh->blendedTotalNum; i++) {
			WBone*& bone = mesh->blendedBoneList[i];
			bone = root->FindBone(bone->m_name, 0);
		}
		if (mesh->aabbBone != 0) {
			mesh->aabbBone = root->FindBone(mesh->aabbBone->m_name, 0);
		}
	}
	if (child) {
		if (m_next != 0) {
			m_next->ResetMeshBone(root, true);
		}
		if (m_child != 0) {
			m_child->ResetMeshBone(root, true);
		}
	}
}

// wbone.cpp:452
void WBone::SetName(char* objectName) {
	strcpy(m_name, objectName);
	m_hashCode = GetHashCode(m_name);
	m_motion_bone_name = GetNameSubBip();
}

// wbone.cpp:460
void WBone::SetParent(WBone* parent) {
	if (parent != 0) {
		m_next = parent->m_child;
		m_parent = parent;
		parent->m_child = this;
	} else {
		m_parent = 0;
	}
}

// wbone.cpp:471
void WBone::ReleaseChild(WBone* bone) {
	if (m_child == bone) {
		m_child = m_child->m_next;
	} else {
		for (WBone* child = m_child; child != 0; child = child->m_next) {
			if (child->m_next == bone) {
				child->m_next = bone->m_next;
				break;
			}
		}
	}
}

// wbone.cpp:492
void WBone::SetNext(WBone* bone, bool bSetBoth) {
	if (bone != 0 && bSetBoth) {
		WBone* last;
		for (last = bone; last->m_next != 0; last = last->m_next) {
		}
		last->m_next = m_next;
	}
	m_next = bone;
}

// wbone.cpp:507
void WBone::SetBasicMatrix(const WMatrix& mat) {
	m_basicMat = mat;
	m_basicQuat = mat;
	m_localMat = mat;
	m_matrix = mat;
	m_scale = m_localMat.za.Magnitude();

	m_basicQuat.Normalize();
}

// wbone.cpp:518
void WBone::CopyBasicMatrix(WBone* root, bool recursive) {
	WBone* bone = root->FindBone(m_name, 0);
	if (bone == 0 && strstr(m_name, "Bip") != 0) {
		bone = root->FindBone(GetNameSubBip(), 0);
	}
	if (bone != 0) {
		WMatrix mat;
		mat = GetKeyRot(0.0f);
		mat.pivot = GetKeyPos(0.0f);
		bone->SetBasicMatrix(mat);
	}

	if (recursive) {
		if (m_next != 0) {
			m_next->CopyBasicMatrix(root, recursive);
		}
		if (m_child != 0) {
			m_child->CopyBasicMatrix(root, recursive);
		}
	}
}

// wbone.cpp:543
void WBone::SetMatrix(const WMatrix& mat, WView* view) {
	if (m_flag & (ROTATEBONE | MOVEBONE)) {
		if (m_flag.GetFlag(ROTATEBONE)) {
			m_matrix = m_rotateMat * mat;
		}
		if (m_flag.GetFlag(MOVEBONE)) {
			m_matrix.pivot -= m_moveVec * m_localMat;
		}
	} else {
		m_matrix = mat;
	}

	for (WBone* bone = m_child; bone != 0; bone = bone->m_next) {
		bone->SetMatrix(bone->m_localMat * m_matrix, view);
	}
}

// wbone.cpp:570
WVector WBone::GetKeyPos(float t) {
	if (m_keyPos == 0) {
		return m_basicMat.pivot;
	}
	int low = 0;
	int high = m_keyPosNum - 1;
	int mid;
	do {
		mid = (low + high) >> 1;
		if (m_keyPos[mid].t >= t) {
			high = mid;
		} else {
			low = mid + 1;
		}
	} while (low < high);
	if (m_keyPos[mid].t != t) {
		if (mid != 0 || m_keyPos[0].t < t) {
			int index = mid;
			if (m_keyPos[mid].t > t) {
				index = mid - 1;
			}
			float rate = (t - m_keyPos[index].t) / (m_keyPos[high].t - m_keyPos[index].t);
			return (m_keyPos[high].pos - m_keyPos[index].pos) * rate + m_keyPos[index].pos;
		}
	}
	return m_keyPos[mid].pos;
}

// wbone.cpp:599
WQuat WBone::GetKeyRot(float t) {
	if (m_keyRot == 0) {
		return GetBasicRot();
	}
	int low = 0;
	int high = m_keyRotNum - 1;
	int mid;
	do {
		mid = (low + high) >> 1;
		if (m_keyRot[mid].t >= t) {
			high = mid;
		} else {
			low = mid + 1;
		}
	} while (low < high);
	if (m_keyRot[mid].t != t && (mid != 0 || m_keyRot[0].t < t)) {
		int index = mid;
		if (m_keyRot[mid].t > t) {
			index = mid - 1;
		}
		float rate = (t - m_keyRot[index].t) / (m_keyRot[high].t - m_keyRot[index].t);
		return WQuaternionSlerp(m_keyRot[index].rot, m_keyRot[high].rot, rate);
	}
	return m_keyRot[mid].rot;
}

// wbone.cpp:628
void WBone::InsertBoneSetList(float t, WBoneSet* list, int flag) {
	if ((flag & 4) == 0 || strstr(m_name, "tail") == 0) {
		WQuat quat;
		WVector pos;
		float scale;
		if ((flag & 2) != 0) {
			quat = GetKeyRot(t);
			pos = GetKeyPos(t);
			list->AddBoneKey(m_motion_bone_name, &quat, &pos, 0, 3);
		} else if (m_keyFlag != 0) {
			if (m_keyRot != 0) {
				quat = GetKeyRot(t);
			}
			if (m_keyPos != 0) {
				pos = GetKeyPos(t);
			}
			list->AddBoneKey(m_motion_bone_name, &quat, &pos, &scale, m_keyFlag);
		}
	}
	if ((flag & 1) != 0) {
		for (WBone* bone = m_child; bone != 0; bone = bone->m_next) {
			bone->InsertBoneSetList(t, list, flag);
		}
	}
}

// wbone.cpp:686
WVector WBone::GetDeltaVec(float t) {
	return (m_keyPos != 0 && t > 0.0f) ? GetKeyPos(t) - m_keyPos->pos : WVector::ZERO;
}

// wbone.cpp:692
char* WBone::GetNameSubBip() {
	char* p = strstr(m_name, "Bip");
	if (p == 0) {
		p = strstr(m_name, "bip");
	}
	if (p != 0) {
		while (p[3] != '\0' && (p[3] == ' ' || p[2] != ' ')) {
			p++;
		}
		return p + 3;
	}
	return m_name;
}

// wbone.cpp:710
void WBone::ApplyBoneSetList(WBoneSet* boneSet) {
	WBoneKey* key = boneSet->FindBoneKey(m_motion_bone_name);
	if (key != 0 && key->flags != 0) {
		SetLocalMatrix((key->flags & 1) ? key->quat : m_basicQuat, (key->flags & 2) ? key->pivot : m_basicMat.pivot);
	}

	if (m_next != 0) {
		m_next->ApplyBoneSetList(boneSet);
	}
	if (m_child != 0) {
		m_child->ApplyBoneSetList(boneSet);
	}
}

// wbone.cpp:736
void WBone::SetKeyframe(w_keyframe* frame, float end, float start) {
	m_keyPosNum = 0;
	m_keyRotNum = 0;
	for (w_keyframe* key = frame; key != 0; key = key->next) {
		if (key->q != 0) {
			++m_keyRotNum;
		}
		if (key->v != 0) {
			++m_keyPosNum;
		}
		if (key->next == 0) {
			if (m_keyRotNum != 0 && (key->q == 0 || key->t < end)) {
				++m_keyRotNum;
			}
			if (m_keyPosNum != 0 && (key->v == 0 || key->t < end)) {
				++m_keyPosNum;
			}
		}
	}
	m_keyFlag = 0;
	if (m_keyRotNum != 0) {
		m_keyRot = new w_keyframe_rot[m_keyRotNum];
		m_keyFlag |= 1;
	}
	if (m_keyPosNum != 0) {
		m_keyPos = new w_keyframe_pos[m_keyPosNum];
		m_keyFlag |= 2;
	}
	int rot = 0;
	int pos = 0;
	for (w_keyframe* key = frame; key != 0; key = key->next) {
		if (key->q != 0) {
			m_keyRot[rot].t = key->t - start;
			m_keyRot[rot].rot = *key->q;
			++rot;
		}
		if (key->v != 0) {
			m_keyPos[pos].t = key->t - start;
			m_keyPos[pos].pos = *key->v;
			++pos;
		}
	}
	if (rot != 0 && rot != m_keyRotNum) {
		m_keyRot[rot].t = end - start;
		m_keyRot[rot].rot = m_keyRot[rot - 1].rot;
	}
	if (pos != 0 && pos != m_keyPosNum) {
		m_keyPos[pos].t = end - start;
		m_keyPos[pos].pos = m_keyPos[pos - 1].pos;
	}
}

// wbone.cpp:848
bool WBone::CheckSuitableForKeyframe(float t1, float t2, float (*t)[2], int n, bool poscheck) {
	for (int i = 0; i < n; i++) {
		if ((poscheck == false && t1 <= t[i][0] && t[i][0] <= t2) || (poscheck == true && t1 <= t[i][1] && t[i][1] <= t2)) {
			return true;
		}
	}
	return n == 0 ? true : false;
}

// wbone.cpp:860
bool VECEQU(const WVector& A, const WVector& B) {
	return (Abs(A.x - B.x) <= 0.0001f && Abs(A.y - B.y) <= 0.0001f && Abs(A.z - B.z) <= 0.0001f) ? true : false;
}

// wbone.cpp:866
bool QUATEQU(const WQuat& A, const WQuat& B) {
	return (Abs(A.x - B.x) <= 0.0001f && Abs(A.y - B.y) <= 0.0001f && Abs(A.z - B.z) <= 0.0001f && Abs(A.w - B.w) <= 0.0001f) ? true : false;
}

// wbone.cpp:872
int WBone::OptimizeRotKey(float (*t)[2], int n, w_keyframe_rot** buff) {
	int* table = new int[m_keyRotNum];
	int cur = 0;
	int i;
	int prev;
	WQuat q;
	for (i = 0; i < m_keyRotNum; ++i) {
		if (i > 0 && i < m_keyRotNum - 1) {
			float delta = (m_keyRot[i].t - m_keyRot[prev].t) / (m_keyRot[i + 1].t - m_keyRot[prev].t);
			q = WQuaternionSlerp(m_keyRot[prev].rot, m_keyRot[i + 1].rot, delta);
			if (QUATEQU(q, m_keyRot[i].rot)) {
				table[i] = -1;
				continue;
			}
		}
		table[i] = cur;
		prev = i;
		++cur;
	}
	if (cur < m_keyRotNum && buff != 0) {
		w_keyframe_rot* keys = new w_keyframe_rot[cur];
		for (i = 0; i < m_keyRotNum; ++i) {
			if (table[i] >= 0) {
				memcpy(&keys[table[i]], &m_keyRot[i], sizeof(w_keyframe_rot));
			}
		}
		*buff = keys;
	}
	delete[] table;
	return cur;
}

// wbone.cpp:933
int WBone::OptimizePosKey(float (*t)[2], int n, w_keyframe_pos** buff) {
	int* table = new int[m_keyPosNum];
	int i = 0;
	int cur = 0;
	int prev;
	WVector p;
	for (; i < m_keyPosNum; ++i) {
		if (i > 0 && i < m_keyPosNum - 1) {
			float delta = (m_keyPos[i].t - m_keyPos[prev].t) / (m_keyPos[i + 1].t - m_keyPos[prev].t);
			p = (m_keyPos[i + 1].pos - m_keyPos[prev].pos) * delta + m_keyPos[prev].pos;
			if (VECEQU(p, m_keyPos[i].pos)) {
				table[i] = -1;
				continue;
			}
		}
		table[i] = cur;
		prev = i;
		++cur;
	}
	if (cur < m_keyPosNum && buff != 0) {
		w_keyframe_pos* keys = new w_keyframe_pos[cur];
		for (i = 0; i < m_keyPosNum; ++i) {
			if (table[i] >= 0) {
				memcpy(&keys[table[i]], &m_keyPos[i], sizeof(w_keyframe_pos));
			}
		}
		*buff = keys;
	}
	delete[] table;
	return cur;
}

// wbone.cpp:1032
void WBone::OptimizeBoneSet(float (*range)[2], int count, int child) {
	w_keyframe_rot* rot = m_keyRot;
	w_keyframe_pos* pos = m_keyPos;
	if (m_keyRot != 0) {
		m_keyRotNum = OptimizeRotKey(range, count, &m_keyRot);
		if (rot != m_keyRot) {
			delete[] rot;
		}
	}
	if (m_keyPos != 0) {
		m_keyPosNum = OptimizePosKey(range, count, &m_keyPos);
		if (pos != m_keyPos) {
			delete[] pos;
		}
	}
	if (m_next != 0 && child != 0) {
		m_next->OptimizeBoneSet(range, count, 1);
	}
	if (m_child != 0 && child != 0) {
		m_child->OptimizeBoneSet(range, count, 1);
	}
}

// wbone.cpp:1077
void WBone::SetMesh(w_pet_vertex* vtxList, int vnum, w_pet_tri_point* triList, w_pet_texture_info** texInfo, WBone** triBoneTable, int triNum) {
	int faceNum = 0;
	int top;
	for (int bind = 0; bind < triNum; ++bind) {
		int j;
		for (j = 0; j < 3; ++j) {
			if (j != 0) {
				WBone* held = triBoneTable[bind * 3 + top];
				WBone* other = triBoneTable[bind * 3 + j];
				if (held == other) {
					continue;
				}
				if (other->FindBone(held->m_name, 0) == 0) {
					continue;
				}
			}
			top = j;
		}
		if (triBoneTable[bind * 3 + top] == this) {
			++faceNum;
		}
	}

	if (m_mesh != 0) {
		ClearMesh(m_mesh);
		if (m_mesh != 0) {
			delete m_mesh;
			m_mesh = 0;
		}
	}

	if (faceNum != 0) {
		int* faceTable = new int[faceNum];
		int* faceIndex = new int[faceNum];
		int bind = 0;
		if (triNum > 0) {
			int* write = faceTable;
			do {
				int j;
				for (j = 0; j < 3; ++j) {
					if (j != 0) {
						WBone* held = triBoneTable[bind * 3 + top];
						WBone* other = triBoneTable[bind * 3 + j];
						if (held == other) {
							continue;
						}
						if (other->FindBone(held->m_name, 0) == 0) {
							continue;
						}
					}
					top = j;
				}
				if (triBoneTable[bind * 3 + top] == this) {
					*write = bind;
					++write;
				}
				++bind;
			} while (bind < triNum);
		}
		int i = 0;
		while (i < faceNum) {
			int scan;
			for (scan = 0; scan < faceNum; ++scan) {
				if (faceTable[scan] >= 0) {
					break;
				}
			}
			w_pet_texture_info* first = texInfo[faceTable[scan]];
			int texHandle = first->texHandle;
			unsigned char group = first->group;
			int slot = 0;
			int used = 0;
			for (; slot < faceNum; ++slot) {
				int index = faceTable[slot];
				if (index < 0) {
					continue;
				}
				w_pet_texture_info* tex = texInfo[index];
				if (tex->texHandle != texHandle) {
					continue;
				}
				if (tex->group != group) {
					continue;
				}
				faceIndex[used] = index;
				++i;
				faceTable[slot] = -1;
				++used;
			}
			if (used != 0) {
				w_mesh* mesh = GetIndexedmesh(vtxList, triList, triBoneTable, texInfo, used, faceIndex);
				mesh->group = group;
				mesh->next = m_mesh;
				m_mesh = mesh;
				if (mesh->rigidNum != 0 || mesh->blendedRigidNum != 0) {
					m_flag.Enable(4);
				}
			}
		}
		delete[] faceIndex;
		delete[] faceTable;
	}

	if (m_next != 0) {
		m_next->SetMesh(vtxList, vnum, triList, texInfo, triBoneTable, triNum);
	}
	if (m_child != 0) {
		m_child->SetMesh(vtxList, vnum, triList, texInfo, triBoneTable, triNum);
	}
}

// wbone.cpp:1169
void WBone::SetBoneMesh(w_pet_vertex* vtxList, int vtxNum, w_pet_tri_point* triList, w_pet_texture_info** texInfo, WBone** triBoneTable, int faceNum) {
	if (m_mesh != 0) {
		ClearMesh(m_mesh);
		if (m_mesh != 0) {
			delete m_mesh;
			m_mesh = 0;
		}
	}
	int* mark = new int[faceNum];
	int* faces = new int[faceNum];
	memset(mark, 0, faceNum * 4);
	int done = 0;
	if (faceNum > 0) {
		do {
			int i = 0;
			do {
				if (mark[i] >= 0) {
					break;
				}
				++i;
			} while (i < faceNum);
			int handle = texInfo[i]->texHandle;
			unsigned char group = texInfo[i]->group;
			int count = 0;
			for (; i < faceNum; ++i) {
				if (texInfo[i]->texHandle == handle && texInfo[i]->group == group) {
					faces[count] = i;
					mark[i] = -1;
					++count;
				}
			}
			if (count != 0) {
				done += count;
				m_flag.Enable(0x10000);
				w_mesh* mesh = GetIndexedmesh(vtxList, triList, triBoneTable, texInfo, count, faces);
				mesh->group = group;
				mesh->next = m_mesh;
				m_mesh = mesh;
				if (mesh->rigidNum != 0 || mesh->blendedRigidNum != 0) {
					m_flag.Enable(4);
				}
			}
		} while (done < faceNum);
	}
	delete[] faces;
	delete[] mark;
}

// A scratch vertex while indexing a mesh (PDB layout, size 0x38).
struct w_temp_vertex_list {
	w_temp_vertex_list();
	short index;                        // 0x0
	WVector pos;                        // 0x4
	WVector normal;                     // 0x10
	float weight;                       // 0x1c
	WBone* bone;                        // 0x20
	float tu;                           // 0x24
	float tv;                           // 0x28
	w_vertex* gvertex;                  // 0x2c
	w_pet_vertex* vertex;               // 0x30
	int kind;                           // 0x34
};
CHECK_SIZE(w_temp_vertex_list, 0x38);
CHECK_FIELD(w_temp_vertex_list, index, 0x0);
CHECK_FIELD(w_temp_vertex_list, pos, 0x4);
CHECK_FIELD(w_temp_vertex_list, normal, 0x10);
CHECK_FIELD(w_temp_vertex_list, weight, 0x1c);
CHECK_FIELD(w_temp_vertex_list, bone, 0x20);
CHECK_FIELD(w_temp_vertex_list, tu, 0x24);
CHECK_FIELD(w_temp_vertex_list, tv, 0x28);
CHECK_FIELD(w_temp_vertex_list, gvertex, 0x2c);
CHECK_FIELD(w_temp_vertex_list, vertex, 0x30);
CHECK_FIELD(w_temp_vertex_list, kind, 0x34);

// wbone.cpp:1244
w_temp_vertex_list::w_temp_vertex_list() {
}

// wbone.cpp:1244
w_mesh* WBone::GetIndexedmesh(w_pet_vertex* vtxList, w_pet_tri_point* triList, WBone** triBoneTable, w_pet_texture_info** texInfo, int faceNum, int* faceIndex) {
	short* table;
	int blendTotal = 0;
	int blendNum = 0;
	int maxBoneNum = 0;
	WVector vec, pos;
	int vtxNum;
	WMatrix invMat;
	invMat = ~m_matrix;

	w_temp_vertex_list* list = new w_temp_vertex_list[faceNum * 3];
	int l = faceNum * 3;
	int i = 0;
	vtxNum = 0;
	int rigidNum = 0;

	int j, k;
	for (; i < l; ++i) {
		w_pet_tri_point* point = &triList[faceIndex[i / 3] * 3 + i % 3];
		vec = WVector(vtxList[point->pos].x, vtxList[point->pos].y, vtxList[point->pos].z);
		for (j = 0; j < vtxNum; j++) {
			if (WisEqual(list[j].pos, vec, 1e-5f) != 0 &&
				list[j].weight == vtxList[point->pos].weight &&
				list[j].tu == point->tu && list[j].tv == point->tv &&
				fabs(list[j].normal.x - point->nx) <= 0.001f &&
				fabs(list[j].normal.y - point->ny) <= 0.001f &&
				fabs(list[j].normal.z - point->nz) <= 0.001f &&
				list[j].bone == triBoneTable[point - triList]) {
				break;
			}
		}
		list[i].index = (short)j;
		if (j == vtxNum) {
			list[j].pos = vec;
			list[j].weight = vtxList[point->pos].weight;
			list[j].tu = point->tu;
			list[j].tv = point->tv;
			list[j].normal = WVector(point->nx, point->ny, point->nz);
			list[j].bone = triBoneTable[point - triList];
			list[j].vertex = &vtxList[point->pos];
			int kind = vtxList[point->pos].list != 0 ? 2 : (list[j].bone != this ? 1 : 0);
			++vtxNum;
			list[j].kind = kind;
			if (kind > 0) {
				++rigidNum;
			}
			if (kind > 1) {
				++blendNum;
				blendTotal += list[j].vertex->listNum;
			}
		}
	}

	w_mesh* mesh = new w_mesh;
	memset(mesh, 0, sizeof(w_mesh));
	mesh->originalTexHandle = mesh->texHandle = texInfo[faceIndex[0]]->texHandle;
	mesh->originalTu = mesh->tu = texInfo[faceIndex[0]]->pu;
	mesh->originalTv = mesh->tv = texInfo[faceIndex[0]]->pv;
	mesh->originalScaleu = mesh->scaleu = texInfo[faceIndex[0]]->scalex;
	mesh->originalScalev = mesh->scalev = texInfo[faceIndex[0]]->scaley;
	mesh->diffuse = texInfo[faceIndex[0]]->diffuse;
	mesh->group = 0;
	mesh->next = 0;
	mesh->alpha = (float)(texInfo[faceIndex[0]]->alpha / 255.0f);
	mesh->flags = texInfo[faceIndex[0]]->drawFlags | (mesh->alpha < 1.0f ? 0x20400000 : 0);
	mesh->xiDrawFlag2 = texInfo[faceIndex[0]]->xulDrawFlag2;
	mesh->bEnv = texInfo[faceIndex[0]]->mapType == 1 || texInfo[faceIndex[0]]->mapType == 2 ? true : false;
	mesh->bSpec = texInfo[faceIndex[0]]->mapType == 3;
	mesh->indexNum = l;
	mesh->indexList = new unsigned short[l];

	mesh->rigidNum = rigidNum;
	mesh->vtxColorPtrList = rigidNum > 0 ? (unsigned long**)g_mem.Alloc(rigidNum * sizeof(unsigned long*)) : 0;
	mesh->boneList = mesh->rigidNum > 0 ? (WBone**)g_mem.Alloc(mesh->rigidNum * sizeof(WBone*)) : 0;
	if (mesh->boneList != 0) {
		memset(mesh->boneList, 0, mesh->rigidNum * sizeof(WBone*));
	}
	mesh->blendedRigidNum = blendNum;
	mesh->blendedTotalNum = blendTotal;
	mesh->blendWeightList = blendTotal > 0 ? (float*)g_mem.Alloc(blendTotal * sizeof(float)) : 0;
	mesh->blendedBoneList = blendTotal > 0 ? (WBone**)g_mem.Alloc(blendTotal * sizeof(WBone*)) : 0;
	mesh->blendedVecList = blendTotal > 0 ? new WVector[blendTotal] : 0;
	mesh->blendedNormalList = blendTotal > 0 ? new WVector[blendTotal] : 0;

	mesh->vtxNum = vtxNum;
	mesh->vecList = new WVector[mesh->vtxNum];
	mesh->normList = new WVector[mesh->vtxNum];
	mesh->vtxColorList = new unsigned long[mesh->vtxNum];
	mesh->originalVtxColorList = new unsigned long[mesh->vtxNum];
	mesh->myBlendedNormalList = 0;
	mesh->myNormalList = 0;
	if ((((unsigned long)m_flag) >> 16) & 1) {
		mesh->weightList = new float[mesh->vtxNum];
	} else {
		mesh->weightList = 0;
	}
	mesh->uvData = new float[mesh->vtxNum][2];
	mesh->uvBackup = texInfo[faceIndex[0]]->mapType > 1 ? new float[mesh->vtxNum][2] : 0;
	for (i = 0; i < mesh->vtxNum; ++i) {
		mesh->originalVtxColorList[i] = mesh->vtxColorList[i] = mesh->diffuse;
	}
	AllocVTX(mesh->vtxNum);

	table = new short[mesh->vtxNum];
	int n;
	for (n = 0, i = 0, j = 0, k = blendNum, l = rigidNum; i < mesh->indexNum; ++i) {
		if (list[i].index == n) {
			switch (list[n].kind) {
			case 0:
				table[n] = (short)l++;
				break;
			case 1:
				table[n] = (short)k++;
				break;
			case 2:
				table[n] = (short)j++;
				break;
			}
			++n;
		}
	}

	for (i = 0, k = 0, n = 0; i < mesh->indexNum; ++i) {
		mesh->indexList[i] = table[list[i].index];
		if (list[i].index != n) {
			continue;
		}
		if (table[n] < blendNum) {
			for (j = 0; j < list[n].vertex->listNum; ++j) {
				mesh->blendedBoneList[k] = list[n].vertex->list[j].bone;
				mesh->blendWeightList[k] = (float)list[n].vertex->list[j].blendWeight / 255.0f;
				mesh->blendedVecList[k] = list[n].pos * ~mesh->blendedBoneList[k]->GetMatrix();
				mesh->blendedNormalList[k] = RotVec(list[n].normal, ~mesh->blendedBoneList[k]->GetMatrix());
				mesh->blendedNormalList[k].Normalize();
				++k;
			}
			if (maxBoneNum < list[n].vertex->listNum) {
				maxBoneNum = list[n].vertex->listNum;
			}
		}
		if (table[n] < rigidNum) {
			mesh->boneList[table[n]] = list[n].bone;
			mesh->vecList[table[n]] = list[n].pos * ~list[n].bone->m_matrix;
			mesh->vtxColorPtrList[table[n]] = &mesh->vtxColorList[table[n]];
			mesh->normList[table[n]] = RotVec(list[n].normal, ~list[n].bone->m_matrix);
		} else {
			mesh->vecList[table[n]] = list[n].pos * invMat;
			mesh->normList[table[n]] = RotVec(list[n].normal, invMat);
		}
		if (mesh->weightList != 0) {
			mesh->weightList[table[n]] = list[n].weight;
		}
		mesh->normList[table[n]].Normalize();
		mesh->uvData[table[n]][0] = texInfo[faceIndex[i / 3]]->scalex * list[n].tu + texInfo[faceIndex[i / 3]]->pu;
		mesh->uvData[table[n]][1] = texInfo[faceIndex[i / 3]]->scaley * list[n].tv + texInfo[faceIndex[i / 3]]->pv;
		if (mesh->uvBackup != 0) {
			mesh->uvBackup[table[n]][0] = mesh->uvData[table[n]][0];
			mesh->uvBackup[table[n]][1] = mesh->uvData[table[n]][1];
		}
		++n;
	}

	if (maxBoneNum == 0 && mesh->rigidNum > 0) {
		maxBoneNum = 1;
	}
	mesh->maxBoneNum = maxBoneNum;
	CalcMeshAABB(*this, *mesh);
	delete[] table;
	delete[] list;
	return mesh;
}

// wbone.cpp:1480
void WBone::SetMesh(w_faces* facelist, int num) {
	m_mesh = 0;
	if (num != 0) {
		w_faces** list = new w_faces*[num];
		int i;
		for (i = 0; i < num; ++i) {
			list[i] = &facelist[i];
		}

		qsort(list, num, sizeof(w_faces*), SortByTextureHandle);

		int first = 0;
		int last = 0;
		while (last < num) {
			while (last < num && list[first]->texhandle == list[last]->texhandle) {
				++last;
			}
			w_mesh* mesh = GetIndexedmesh(&list[first], last - first);
			mesh->next = m_mesh;
			m_mesh = mesh;
			first = last;
		}
		delete[] list;
	}
}

// wbone.cpp:1512
int __cdecl WBone::SortByTextureHandle(const void* arg1, const void* arg2) {
	const w_faces* a = *(const w_faces* const*)arg1;
	const w_faces* b = *(const w_faces* const*)arg2;

	return a->texhandle == b->texhandle ? a->p[0].bone - b->p[0].bone : a->texhandle - b->texhandle;
}

// wbone.cpp:1522
w_mesh* WBone::GetIndexedmesh(w_faces** faces, int count) {
	int rigidNum = 0;
	int blendedRigidNum = 0;
	int blendedTotalNum = 0;
	w_temp_vertex_list* list = new w_temp_vertex_list[count * 3];
	int cornerNum = count * 3;

	int i, j, k, l, n;
	WVector vec;
	WMatrix invMat;
	for (i = 0; i < cornerNum; ++i) {
		w_faces::w_faces_info* p = &faces[i / 3]->p[i % 3];
		list[i].index = -1;
		list[i].pos = p->object->gvertex[p->vindex].v;
		list[i].normal = p->norm;
		list[i].bone = p->object->gvertex[p->vindex].obj != 0 ? p->object->gvertex[p->vindex].obj->bone : p->object->bone;
		list[i].tu = p->tu;
		list[i].tv = p->tv;
		list[i].kind = p->object->gvertex[p->vindex].rigidNum != 0 ? 2 : (list[i].bone != this);
		list[i].gvertex = &p->object->gvertex[p->vindex];
	}

	for (i = 0, n = 0; i < cornerNum; ++i) {
		for (j = 0; j <= i; ++j) {
			w_faces::w_faces_info* p = &faces[j / 3]->p[j % 3];
			if (list[i].tu == p->tu && list[i].tv == p->tv &&
				WisEqual(list[i].pos, p->object->gvertex[p->vindex].v, 1e-5f) &&
				list[i].bone == (p->object->gvertex[p->vindex].obj != 0 ? p->object->gvertex[p->vindex].obj->bone : p->object->bone) &&
				(list[i].normal - p->norm).Magnitude() <= 0.01f) {
				if (i == j) {
					list[i].index = (short)n++;
					if (list[i].kind != 0) {
						++rigidNum;
					}
					if (list[i].kind == 2) {
						blendedTotalNum += list[i].gvertex->rigidNum;
						++blendedRigidNum;
					}
				} else {
					list[i].index = list[j].index;
				}
			}
		}
	}

	w_mesh* mesh = new w_mesh;
	memset(mesh, 0, sizeof(w_mesh));
	mesh->originalTexHandle = mesh->texHandle = faces[0]->texhandle;
	mesh->originalTu = mesh->tu = 0.0f;
	mesh->originalTv = mesh->tv = 0.0f;
	mesh->originalScaleu = mesh->scaleu = 1.0f;
	mesh->originalScalev = mesh->scalev = 1.0f;
	mesh->diffuse = faces[0]->diffuse;
	mesh->group = 0;
	mesh->next = 0;
	mesh->alpha = (float)((unsigned char*)&faces[0]->diffuse)[3] / 255.0f;
	mesh->flags = (mesh->alpha < 1.0f ? 0x20400000 : 0) | faces[0]->mapFlags;
	mesh->xiDrawFlag2 = faces[0]->xiMapFlags2;
	mesh->bEnv = faces[0]->mapType == 1 || faces[0]->mapType == 2 ? true : false;
	mesh->bSpec = faces[0]->mapType == 3;
	mesh->indexNum = cornerNum;
	mesh->indexList = new unsigned short[cornerNum];

	mesh->rigidNum = rigidNum;
	mesh->vtxColorPtrList = rigidNum > 0 ? (unsigned long**)g_mem.Alloc(rigidNum * sizeof(unsigned long*)) : 0;
	mesh->boneList = mesh->rigidNum > 0 ? (WBone**)g_mem.Alloc(mesh->rigidNum * sizeof(WBone*)) : 0;
	if (mesh->rigidNum > 0) {
		memset(mesh->boneList, 0, mesh->rigidNum * sizeof(WBone*));
	}
	mesh->blendedRigidNum = blendedRigidNum;
	mesh->blendWeightList = blendedTotalNum > 0 ? (float*)g_mem.Alloc(blendedTotalNum * sizeof(float)) : 0;
	mesh->blendedBoneList = blendedTotalNum > 0 ? (WBone**)g_mem.Alloc(blendedTotalNum * sizeof(WBone*)) : 0;
	mesh->blendedVecList = blendedTotalNum > 0 ? new WVector[blendedTotalNum] : 0;
	mesh->blendedNormalList = blendedTotalNum > 0 ? new WVector[blendedTotalNum] : 0;
	mesh->blendedTotalNum = blendedTotalNum;
	mesh->vtxNum = n;
	mesh->vecList = new WVector[mesh->vtxNum];
	mesh->normList = new WVector[mesh->vtxNum];
	mesh->vtxColorList = new unsigned long[mesh->vtxNum];
	mesh->originalVtxColorList = new unsigned long[mesh->vtxNum];
	mesh->myBlendedNormalList = 0;
	mesh->myNormalList = 0;
	mesh->weightList = 0;
	mesh->uvData = new float[mesh->vtxNum][2];
	mesh->uvBackup = faces[0]->mapType > 1 ? new float[mesh->vtxNum][2] : 0;
	for (i = 0; i < mesh->vtxNum; ++i) {
		mesh->originalVtxColorList[i] = mesh->vtxColorList[i] = mesh->diffuse;
	}
	AllocVTX(mesh->vtxNum);

	short* slotList = new short[mesh->vtxNum];
	for (n = 0, i = 0, j = 0, l = blendedRigidNum, k = rigidNum; i < mesh->indexNum; ++i) {
		if (list[i].index == n) {
			switch (list[i].kind) {
			case 0:
				slotList[n] = (short)k++;
				break;
			case 1:
				slotList[n] = (short)l++;
				break;
			case 2:
				slotList[n] = (short)j++;
				break;
			}
			++n;
		}
	}

	for (i = 0, n = 0, k = 0; i < mesh->indexNum; ++i) {
		mesh->indexList[i] = slotList[list[i].index];
		if (list[i].index != n) {
			continue;
		}
		mesh->vecList[slotList[n]] = list[i].pos;
		if (list[i].gvertex->obj == 0) {
			mesh->normList[slotList[n]] = list[i].normal;
		} else {
			mesh->normList[slotList[n]] = RotVec(list[i].normal, m_matrix * ~list[i].gvertex->obj->matrix);
		}
		mesh->normList[slotList[n]].Normalize();
		if (slotList[n] < mesh->blendedRigidNum) {
			for (l = 0; l < list[i].gvertex->rigidNum; ++l, ++k) {
				WMatrix mInv;
				mInv = ~list[i].gvertex->rigidVtx[l].obj->matrix;
				mesh->blendedBoneList[k] = list[i].gvertex->rigidVtx[l].bone;
				mesh->blendWeightList[k] = list[i].gvertex->rigidVtx[l].blendWeight;
				w_3d_object* object = list[i].gvertex->obj;
				mesh->blendedVecList[k] = list[i].pos * object->matrix * mInv;
				const WMatrix& boneMatrix = m_matrix;
				mesh->blendedNormalList[k] = RotVec(list[i].normal, boneMatrix * mInv);
				mesh->blendedNormalList[k].Normalize();
			}
		}
		if (slotList[n] < rigidNum) {
			mesh->boneList[slotList[n]] = list[i].bone;
			mesh->vtxColorPtrList[slotList[n]] = &mesh->vtxColorList[slotList[n]];
		}
		mesh->uvData[slotList[n]][0] = list[i].tu;
		mesh->uvData[slotList[n]][1] = list[i].tv;
		if (mesh->uvBackup != 0) {
			mesh->uvBackup[slotList[n]][0] = list[i].tu;
			mesh->uvBackup[slotList[n]][1] = list[i].tv;
		}
		++n;
	}
	CalcMeshAABB(*this, *mesh);
	delete[] slotList;
	delete[] list;
	return mesh;
}

// wbone.cpp:1747
int WBone::GetMeshForFaceAnimation(unsigned char group, w_mesh** inMesh, int current) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->group == group) {
			if (inMesh != 0) {
				inMesh[current] = mesh;
			}
			current++;
		}
	}

	if (m_next != 0) {
		current = m_next->GetMeshForFaceAnimation(group, inMesh, current);
	}
	if (m_child != 0) {
		current = m_child->GetMeshForFaceAnimation(group, inMesh, current);
	}
	return current;
}

// The last group number handed out by GetMeshForFaceAnimation (no symbol).
static int s_faceAnimationGroup;

// wbone.cpp:1771
int WBone::GetMeshForFaceAnimation(int texHandle, w_mesh** inMesh, int current) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->originalTexHandle == texHandle || texHandle == 0) {
			if (inMesh != 0) {
				inMesh[current] = mesh;
				if (mesh->group == 0 && texHandle != 0) {
					if (current == 0) {
						s_faceAnimationGroup++;
					}
					mesh->group = s_faceAnimationGroup;
				}
			}
			current++;
		}
	}

	if (m_next != 0) {
		current = m_next->GetMeshForFaceAnimation(texHandle, inMesh, current);
	}
	if (m_child != 0) {
		current = m_child->GetMeshForFaceAnimation(texHandle, inMesh, current);
	}
	return current;
}

// wbone.cpp:1806
void WBone::CombineVtxColor(bool child) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		int i;
		for (i = 0; i < mesh->blendedRigidNum; i++) {
			mesh->vtxColorPtrList[i] = &mesh->vtxColorList[i];
		}
		for (; i < mesh->rigidNum; i++) {
			unsigned long* color = mesh->boneList[i]->FindVColor(mesh->vecList[i], mesh->normList[i]);
			if (color == 0) {
				color = &mesh->vtxColorList[i];
			}
			mesh->vtxColorPtrList[i] = color;
		}
	}
	if (child) {
		if (m_next != 0) {
			m_next->CombineVtxColor(true);
		}
		if (m_child != 0) {
			m_child->CombineVtxColor(true);
		}
	}
}

// wbone.cpp:1861
unsigned long* WBone::FindVColor(const WVector& vec, const WVector& normal) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = mesh->rigidNum; i < mesh->vtxNum; i++) {
			if (WisEqual(mesh->vecList[i], vec, 0.00001f) && WisEqual(mesh->normList[i], normal, 0.00001f)) {
				return &mesh->vtxColorList[i];
			}
		}
	}
	return 0;
}

// wbone.cpp:1882
void WBone::Transform(const WMatrix& mat, WView* view, float matscale, int flag) {
	WSphere tr_bound_sphere;
	float scale;
	if (!(flag & NOUPDATEMAT)) {
		if (m_flag.GetFlag(ROTATEBONE | MOVEBONE | SCALEBONE)) {
			if (m_flag.GetFlag(ROTATEBONE)) {
				m_matrix = m_rotateMat * m_localMat;
			} else {
				m_matrix = m_localMat;
			}
			if (m_flag.GetFlag(SCALEBONE)) {
				m_matrix.AxisScale(WVector(m_bonescale, m_bonescale, m_bonescale));
			}
			if (m_flag.GetFlag(MOVEBONE)) {
				m_matrix.pivot -= m_moveVec;
			}
			m_matrix = m_matrix * mat;
		} else {
			m_matrix = m_localMat * mat;
		}
	}
	if (view) {
		scale = matscale * m_scale;
		tr_bound_sphere.pos = Transform(m_bound_sphere.pos);
		scale *= m_bound_sphere.radius;
		tr_bound_sphere = WSphere(tr_bound_sphere.pos, scale);
		bool inside = view->InFrustum(tr_bound_sphere) == true || WisZero(tr_bound_sphere.radius, g_EPSILON);
		m_flag.Turn(VISIBLE, m_flag.GetFlag(HASRIGIDVTX) || (flag & NOCLIP) || inside);
		if (m_flag.GetFlag(HASRIGIDVTX)) {
			m_flag.Turn(CLIPPING, !(flag & NOCLIP));
		} else {
			m_flag.Turn(CLIPPING, !(flag & NOCLIP) && (!IsVisible() || !view->InFrustumSafe(tr_bound_sphere)));
		}
	}
	if (m_next) {
		m_next->Transform(mat, view, matscale, flag);
	}
	if (m_child) {
		m_child->Transform(m_matrix, view, matscale * m_scale, flag);
	}
}

// wbone.cpp:1944
void WBone::DisableFog() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		mesh->drawFlag &= ~0x8000000;
	}
}

// wbone.cpp:1952
void WBone::EnableAlphaBlend() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->drawFlag & 0x80000000) {
			mesh->drawFlag = (mesh->drawFlag & ~0x80400000) | 0x400000;
		}
	}
}

// wbone.cpp:1963
void WBone::SetRenderMode(int mode) {
	m_rendMode = mode;
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		switch (mode) {
		case 0:
			mesh->drawFlag = (((mesh->flags & 0x1800000) != 0x800000) ? 0x8000000 : 0)
				| (mesh->texHandle & 0x3ffff) | mesh->flags;
			break;
		case 1:
			mesh->drawFlag = (mesh->texHandle & 0x3ffff) | mesh->flags | 0x40000000;
			break;
		case 5:
			mesh->drawFlag = (((mesh->flags & 0x1800000) != 0x800000) ? 0x8000000 : 0)
				| (mesh->texHandle & 0x3ffff) | mesh->flags | 0x40000;
			break;
		case 2:
			mesh->drawFlag = mesh->flags;
			break;
		case 3:
			mesh->drawFlag = 0x4000000;
			break;
		case 4:
			mesh->drawFlag = (mesh->texHandle & 0x3ffff) | 0x20300000;
			break;
		}
	}
	if (m_next != 0) {
		m_next->SetRenderMode(mode);
	}
	if (m_child != 0) {
		m_child->SetRenderMode(mode);
	}
}

// wbone.cpp:2027
void WBone::SetAlpha(unsigned char alpha, int recursive) {
	if (m_alpha != alpha) {
		m_alpha = alpha;
		if (alpha == 0) {
			m_flag.Enable(1);
		}
		if (m_flag.GetFlag(1) && alpha > 0) {
			m_flag.Disable(1);
		}
		for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			unsigned long color = (unsigned long)(int)(alpha * mesh->alpha) << 24;

			if (color == 0xff000000) {
				mesh->drawFlag = (((mesh->drawFlag & 0x1800000) == 0 && (mesh->flags & 0x400000) == 0) ? (mesh->drawFlag & ~0x400000) : (mesh->drawFlag | 0x400000)) & ~0x20000000;
			} else {
				mesh->drawFlag |= 0x20400000;
			}
			for (int i = 0; i < mesh->vtxNum; i++) {
				mesh->vtxColorList[i] = (mesh->vtxColorList[i] & 0xffffff) | color;
			}
		}
	}
	if (recursive) {
		if (m_next != 0) {
			m_next->SetAlpha(alpha, 1);
		}
		if (m_child != 0) {
			m_child->SetAlpha(alpha, 1);
		}
	}
}

// wbone.cpp:2072
void WBone::RestoreVtxColor() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = 0; i < mesh->vtxNum; i++) {
			mesh->vtxColorList[i] = mesh->originalVtxColorList[i];
		}
	}
}

// wbone.cpp:2083
void WBone::SetLightLine(float line) {
	m_lightLine = line;
	if (m_next != 0) {
		m_next->SetLightLine(line);
	}
	if (m_child != 0) {
		m_child->SetLightLine(line);
	}
}

// wbone.cpp:2097
void WBone::CalcLight(WScene* scene, int lightmode) {
	if (!IsSelfIllum() && scene != 0) {
		WVector pos = m_bound_sphere.pos * m_matrix;
		CalcLight(scene->GetLightSet(pos), false, lightmode, 0);
	} else {
		// Same as CalcLight(0, false, lightmode | 0x4000000, 0); all of it is on line 2099.
		int mode = lightmode | 0x4000000;
		if ((mode & 0x4000000) || IsSelfIllum()) {
			CalcLight_SelfIllum();
		} else {
			CalcLight_Diffuse_Equals_Ambient(0, mode);
		}
	}
	if (m_next != 0) {
		m_next->CalcLight(scene, lightmode);
	}
	if (m_child != 0) {
		m_child->CalcLight(scene, lightmode);
	}
}

// wbone.cpp:2111
void WBone::SetLight(LightSet* lig) {
	m_light = *lig;
	if (m_next != 0) {
		m_next->SetLight(lig);
	}
	if (m_child != 0) {
		m_child->SetLight(lig);
	}
}

// wbone.cpp:2122
void WBone::CalcLight(LightSet* lig, bool recursive, int lightmode, WScene* scene) {
	if ((lightmode & 0x4000000) != 0 || IsSelfIllum()) {
		CalcLight_SelfIllum();
	} else if (lig != 0) {
		if (lig->type == LightSet::W_LIGHT_POINT) {
			if (lightmode == 0x1000000) {
				CalcLight_Point(lig, lightmode, scene);
			} else {
				CalcLight_Directional_Per_Bone(lig, lightmode, scene);
			}
		} else if (lig->type == LightSet::W_LIGHT_DIRECTIONAL) {
			CalcLight_Directional(lig, lightmode, scene);
		}
	} else {
		CalcLight_Diffuse_Equals_Ambient(0, lightmode);
	}
	if (recursive) {
		if (m_next != 0) {
			m_next->CalcLight(lig, recursive, lightmode, scene);
		}
		if (m_child != 0) {
			m_child->CalcLight(lig, recursive, lightmode, scene);
		}
	}
}

// wbone.cpp:2381
void WBone::CalcLight_SelfIllum() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		unsigned long calcAlpha = (unsigned long)(int)(m_alpha * mesh->alpha) << 24;
		unsigned long color = (mesh->texHandle == 0 ? Modulate(m_selfillumColor, mesh->diffuse) : m_selfillumColor) | calcAlpha;

		for (int i = 0; i < mesh->vtxNum; i++) {
			mesh->vtxColorList[i] = color;
		}
	}

	if (m_vtxColor != 0xffffffff) {
		for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if ((mesh->drawFlag & 0x1000000) == 0) {
				for (int i = 0; i < mesh->vtxNum; i++) {
					mesh->vtxColorList[i] &= m_vtxColor;
				}
			}
		}
	}
}

// wbone.cpp:2411
void WBone::CalcLight_Point(LightSet* lig, int lightmode, WScene* scene) {
	w_mesh* mesh = m_mesh;
	if (mesh == 0) {
		return;
	}

	unsigned long diffuse, ambient, calcAlpha, c;
	WVector vLocLightPos, vLightDir;
	float t;
	int i, k;
	vLocLightPos = lig->nearone * ~m_matrix;

	for (; mesh != 0; mesh = mesh->next) {
		if (mesh->texHandle == 0) {
			diffuse = Modulate(lig->diffuse, mesh->diffuse);
			ambient = Modulate(lig->ambient, mesh->diffuse);
		} else {
			diffuse = lig->diffuse;
			ambient = lig->ambient;
		}
		calcAlpha = (unsigned long)(int)(m_alpha * mesh->alpha) << 24;

		if ((mesh->xiDrawFlag2 & 0x4) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = lig->ambient2 | calcAlpha;
			}
			continue;
		}
		if ((mesh->xiDrawFlag2 & 0x10) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = calcAlpha | 0xffffff;
			}
			continue;
		}
		if ((lightmode & 0x2000000) != 0) {
			continue;
		}

		k = 0;
		for (i = 0; i < mesh->blendedRigidNum; ++i) {
			WVector v, vN;
			vN.x = vN.y = vN.z = 0.0f;
			v.x = v.y = v.z = 0.0f;
			float fTotal = 0.0f;
			for (; fTotal < 0.999f; ++k) {
				v += mesh->blendedBoneList[k]->Transform(mesh->blendedVecList[k]) * mesh->blendWeightList[k];
				vN += RotVec(mesh->blendedNormalList[k], mesh->blendedBoneList[k]->GetMatrix()) * mesh->blendWeightList[k];
				fTotal += mesh->blendWeightList[k];
			}
			vN.Normalize();
			vLightDir = (lig->nearone - v).Normalize();
			t = (float)(vLightDir * vN) * 255.0f;
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}

		for (; i < mesh->rigidNum; ++i) {
			WVector v, vN;
			v = mesh->boneList[i]->Transform(mesh->vecList[i]);
			vN = RotVec(mesh->normList[i], mesh->boneList[i]->GetMatrix());
			vN.Normalize();
			vLightDir = (lig->nearone - v).Normalize();
			t = vLightDir * vN * 255.0f;
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}

		for (; i < mesh->vtxNum; ++i) {
			vLightDir = (vLocLightPos - mesh->vecList[i]).Normalize();
			t = vLightDir * mesh->normList[i] * 255.0f;
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}
	}

	if (m_vtxColor != 0xffffffff) {
		for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if ((mesh->drawFlag & 0x1000000) != 0) {
				continue;
			}
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] &= m_vtxColor;
			}
		}
	}
}

// wbone.cpp:2558
void WBone::CalcLight_Directional_Per_Bone(LightSet* lig, int lightmode, WScene* scene) {
	if (m_mesh == 0) {
		return;
	}

	float t;
	unsigned long diffuse, ambient, calcAlpha;
	WVector vWorLightDir;
	unsigned long c;
	int i, k;
	w_mesh* mesh;
	WVector vLocLightDir;
	if (m_flag.GetFlag(HASRIGIDVTX)) {
		vWorLightDir = lig->nearone - m_matrix.pivot;
		vWorLightDir = vWorLightDir.Normalize() * 255.0f;
		vLocLightDir = RotVec(vWorLightDir, ~m_matrix);
		vLocLightDir = vLocLightDir.Normalize() * 255.0f;
	} else {
		vLocLightDir = RotVec(lig->nearone - m_matrix.pivot, ~m_matrix);
		vLocLightDir.Normalize();
	}

	for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->texHandle == 0) {
			diffuse = Modulate(lig->diffuse, mesh->diffuse);
			ambient = Modulate(lig->ambient, mesh->diffuse);
		} else {
			diffuse = lig->diffuse;
			ambient = lig->ambient;
		}
		calcAlpha = (unsigned long)(int)(m_alpha * mesh->alpha) << 24;

		if ((mesh->xiDrawFlag2 & 0x4) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = lig->ambient2 | calcAlpha;
			}
			continue;
		}
		if ((mesh->xiDrawFlag2 & 0x10) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = calcAlpha | 0xffffff;
			}
			continue;
		}
		if ((lightmode & 0x2000000) != 0) {
			continue;
		}

		for (k = 0, i = 0; i < mesh->blendedRigidNum; ++i) {
			WVector vN;
			float fTotal;
			vN.x = vN.y = vN.z = 0.0f;
			for (fTotal = 0.0f; fTotal < 0.999f; ++k) {
				vN += RotVec(mesh->blendedNormalList[k], mesh->blendedBoneList[k]->m_matrix) * mesh->blendWeightList[k];
				fTotal += mesh->blendWeightList[k];
			}
			vN.Normalize();
			t = (float)(vN * vWorLightDir);
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}

		for (; i < mesh->rigidNum; ++i) {
			WVector vN;
			vN = RotVec(mesh->normList[i], mesh->boneList[i]->GetMatrix());
			vN.Normalize();
			t = (float)(vN * vWorLightDir);
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}

		for (; i < mesh->vtxNum; ++i) {
			t = (float)(vLocLightDir * mesh->normList[i]);
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}
	}

	if (m_vtxColor != 0xffffffff) {
		for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if ((mesh->drawFlag & 0x1000000) != 0) {
				continue;
			}
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] &= m_vtxColor;
			}
		}
	}
}

// wbone.cpp:2708
void WBone::CalcLight_Directional(LightSet* lig, int lightmode, WScene* scene) {
	if (m_mesh == 0) {
		return;
	}

	unsigned long diffuse;
	float t;
	WVector vWorLightDir;
	unsigned long ambient;
	WVector vLocLightDir;
	unsigned long calcAlpha;
	WVector* normList;
	WVector vN;
	float fTotal;
	w_mesh* mesh;
	int i, k;
	unsigned long c;
	if (m_flag.GetFlag(HASRIGIDVTX)) {
		vWorLightDir = lig->nearone * -255.0f;
	}

	vLocLightDir = RotVec(lig->nearone * -1.0f, ~m_matrix);
	vLocLightDir = vLocLightDir.Normalize() * 255.0f;

	for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->texHandle == 0) {
			diffuse = Modulate(lig->diffuse, mesh->diffuse);
			ambient = Modulate(lig->ambient, mesh->diffuse);
		} else {
			diffuse = lig->diffuse;
			ambient = lig->ambient;
		}
		calcAlpha = (unsigned long)(int)(m_alpha * mesh->alpha) << 24;

		if ((mesh->xiDrawFlag2 & 0x4) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = lig->ambient2 | calcAlpha;
			}
			continue;
		}
		if ((mesh->xiDrawFlag2 & 0x10) != 0) {
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] = calcAlpha | 0xffffff;
			}
			continue;
		}
		if ((lightmode & 0x2000000) != 0) {
			continue;
		}

		normList = mesh->myBlendedNormalList ? mesh->myBlendedNormalList : mesh->blendedNormalList;
		if (normList != 0) {
			for (k = 0, i = 0; i < mesh->blendedRigidNum; ++i) {
				vN.Reset();
				for (fTotal = 0.0f;
					fTotal < 0.999999f && k < mesh->blendedTotalNum; ++k) {
					WVector tmp = RotVec(normList[k], mesh->blendedBoneList[k]->GetMatrix());
					float w = mesh->blendWeightList[k];
					float tz = tmp.z * w;
					float ty = tmp.y * w;
					float tx = tmp.x * w;
					vN += WVector(tx, ty, tz);
					fTotal += mesh->blendWeightList[k];
				}
				vN.Normalize();
				t = (float)(vN.x * vWorLightDir.x + vN.z * vWorLightDir.z + vN.y * vWorLightDir.y);
				if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
					t = t * -1.0f;
				}
				c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
				if ((lightmode & 0x8000000) != 0) {
					mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
				} else {
					mesh->vtxColorList[i] = c | calcAlpha;
				}
			}
		} else {
			i = mesh->blendedRigidNum;
		}

		normList = mesh->myNormalList ? mesh->myNormalList : mesh->normList;
		if (normList == 0) {
			continue;
		}

		for (; i < mesh->rigidNum; ++i) {
			if (&mesh->vtxColorList[i] != mesh->vtxColorPtrList[i]) {
				continue;
			}
			vN = RotVec(normList[i], mesh->boneList[i]->GetMatrix());
			vN.Normalize();
			t = (float)(vN.x * vWorLightDir.x + vN.z * vWorLightDir.z + vN.y * vWorLightDir.y);
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}

		for (; i < mesh->vtxNum; ++i) {
			t = vLocLightDir * normList[i];
			if ((mesh->xiDrawFlag2 & 0x400) != 0 && t < 0.0f) {
				t = t * -1.0f;
			}
			c = t <= 0.0f ? ambient : AddDiffuse(ambient, diffuse, (unsigned char)t);
			if ((lightmode & 0x8000000) != 0) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i] & 0xffffff, c) | calcAlpha;
			} else {
				mesh->vtxColorList[i] = c | calcAlpha;
			}
		}
	}

	if (m_vtxColor != 0xffffffff) {
		for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if ((mesh->drawFlag & 0x1000000) != 0) {
				continue;
			}
			for (i = 0; i < mesh->vtxNum; ++i) {
				mesh->vtxColorList[i] &= m_vtxColor;
			}
		}
	}
}

// wbone.cpp:2866
void WBone::CalcLight_Diffuse_Equals_Ambient(LightSet* lig, int lightmode) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		unsigned long color = (lig != 0 ? (mesh->texHandle == 0 ? Modulate(lig->diffuse, mesh->diffuse) : lig->diffuse) : 0) | ((unsigned long)(int)(m_alpha * mesh->alpha) << 24);

		if (lightmode & 0x8000000) {
			for (int i = 0; i < mesh->vtxNum; i++) {
				mesh->vtxColorList[i] = AddDiffuse(mesh->vtxColorList[i], color);
			}
		} else {
			for (int i = 0; i < mesh->vtxNum; i++) {
				mesh->vtxColorList[i] = color;
			}
		}
	}

	if (m_vtxColor != 0xffffffff) {
		for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if ((mesh->drawFlag & 0x1000000) == 0) {
				for (int i = 0; i < mesh->vtxNum; i++) {
					mesh->vtxColorList[i] &= m_vtxColor;
				}
			}
		}
	}
}

// wbone.cpp:2902
void WBone::ResetLight() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = 0; i < mesh->vtxNum; i++) {
			mesh->vtxColorList[i] = mesh->diffuse;
		}
	}

	if (m_next != 0) {
		m_next->ResetLight();
	}
	if (m_child != 0) {
		m_child->ResetLight();
	}
}

// wbone.cpp:2920
void WBone::SetVtxColor(unsigned long color) {
	m_vtxColor = color;
	RestoreVtxColor();
	if (m_next != 0) {
		m_next->SetVtxColor(color);
	}
	if (m_child != 0) {
		m_child->SetVtxColor(color);
	}
}

// wbone.cpp:2932
void WBone::CalcENVCoord(WView* view, w_mesh* m) {
	int i;
	WMatrix trans;
	WVector norm, vb, va;
	if (m->uvBackup) {
		for (i = 0; i < m->vtxNum; ++i) {
			va = m->vecList[i] * m_matrix - view->GetCamera().pivot;
			vb = RotVec(m->normList[i], m_matrix);
			norm = va - vb * (2 * (va * vb) / (vb.Magnitude() * vb.Magnitude()));
			norm *= 0.5f / norm.Magnitude();
			m->uvData[i][0] = 0.5f - norm.x;
			m->uvData[i][1] = 0.5f - norm.y;
		}
	} else {
		trans = m_matrix * view->GetInvCamera();
		trans *= 0.5f / WVectorLen(trans.xa);
		for (i = 0; i < m->vtxNum; ++i) {
			norm = RotVec(m->normList[i], trans);
			m->uvData[i][0] = norm.x + 0.5f;
			m->uvData[i][1] = 0.5f - norm.y;
		}
	}
}

// wbone.cpp:3142
void RenderAABB(WView* view, const Waabb& aabb, const WMatrix& mat, float rlen) {
	static int box_v[8][3] = {
		{ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
		{ 0, 0, 1 }, { 1, 0, 1 }, { 0, 1, 1 }, { 1, 1, 1 }
	};
	static int box_p[6][4] = {
		{ 0, 2, 3, 1 }, { 4, 5, 7, 6 }, { 0, 1, 5, 4 },
		{ 3, 2, 6, 7 }, { 1, 3, 7, 5 }, { 2, 0, 4, 6 }
	};
	WVector vt[8];
	for (int i = 0; i < 8; ++i) {
		vt[i] = WVector(box_v[i][0] ? (aabb.max.x + rlen) : (aabb.min.x - rlen),
			box_v[i][1] ? (aabb.max.y + rlen) : (aabb.min.y - rlen),
			box_v[i][2] ? (aabb.max.z + rlen) : (aabb.min.z - rlen)) * mat;
	}
	for (int f = 0; f < 6; ++f) {
		for (int k = 0; k < 4; ++k) {
			view->DrawLine(vt[box_p[f][(k - 1) & 3]], 0xffff0000,
				vt[box_p[f][k]], 0xffff0000, 0);
		}
	}
}

// wbone.cpp:2982
void WBone::CalcHilightCoord(WView* view, const LightSet& light, w_mesh* mesh) {
	int i = 0;
	unsigned long type;
	if (!(type = light.type)) {
		return;
	}
	WMatrix mInv;
	mInv = ~m_matrix;
	if (type == 1) {
		return;
	}
	if (type != 2) {
		return;
	}
	{
		WVector vLocalL, vXA, vYA, vZA, vN, v, vLocalE;
		WVector* normList;
		normList = mesh->myBlendedNormalList ? mesh->myBlendedNormalList : mesh->blendedNormalList;
		vLocalE = view->GetCamera().pivot;
		vLocalL = light.nearone;
		vLocalL.Normalize();
		const WVector* direction = &light.nearone;
		const WVector* camera = &view->GetCamera().pivot;
		int k = 0;
		for (i = 0; i < mesh->blendedRigidNum; ++i) {
			float fTotal;
			vN.Reset();
			v.Reset();
			for (fTotal = 0.0f; fTotal < 0.999999f; ++k) {
				vN += RotVec(normList[k], mesh->blendedBoneList[k]->GetMatrix()) * mesh->blendWeightList[k];
				v += (mesh->blendedVecList[k] * mesh->blendedBoneList[k]->GetMatrix()) * mesh->blendWeightList[k];
				fTotal += mesh->blendWeightList[k];
			}
			vN.Normalize();
			if ((float)(vLocalL * vN) >= 0.0f) {
				mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
			} else {
				vZA = (vLocalL + (v - vLocalE).Normalize()).Normalize();
				if ((float)(vZA * vN) >= 0.0f) {
					mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
				} else {
					if (WisEqual(vZA.y, -1.0f, g_EPSILON) || WisEqual(vZA.y, 1.0f, g_EPSILON)) {
						vXA = WCrossProduct(WVector::UNIT_POS_X, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					} else {
						vXA = WCrossProduct(WVector::UNIT_POS_Y, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					}
					mesh->uvBackup[i][0] = Wabs(vXA * vN);
					mesh->uvBackup[i][1] = Wabs(vYA * vN);
				}
			}
		}
		(void)(vLocalL * vN);
		for (; i < mesh->rigidNum; ++i) {
			vLocalL = RotVec(*direction, ~mesh->boneList[i]->GetMatrix()).Normalize();
			if (vLocalL * mesh->normList[i] >= 0.0f) {
				mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
			} else {
				vLocalE = *camera * ~mesh->boneList[i]->GetMatrix();
				vZA = (vLocalL + (mesh->vecList[i] - vLocalE).Normalize()).Normalize();
				if (vZA * mesh->normList[i] >= 0.0f) {
					mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
				} else {
					if (WisEqual(vZA.y, -1.0f, g_EPSILON) || WisEqual(vZA.y, 1.0f, g_EPSILON)) {
						vXA = WCrossProduct(WVector::UNIT_POS_X, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					} else {
						vXA = WCrossProduct(WVector::UNIT_POS_Y, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					}
					mesh->uvBackup[i][0] = Wabs(vXA * mesh->normList[i]);
					mesh->uvBackup[i][1] = Wabs(vYA * mesh->normList[i]);
				}
			}
		}
		vLocalE = *camera * mInv;
		vLocalL = RotVec(*direction, mInv).Normalize();
		for (; i < mesh->vtxNum; ++i) {
			if ((float)(vLocalL * mesh->normList[i]) >= 0.0f) {
				mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
			} else {
				vZA = (vLocalL + (mesh->vecList[i] - vLocalE).Normalize()).Normalize();
				if (vZA * mesh->normList[i] >= 0.0f) {
					mesh->uvBackup[i][0] = mesh->uvBackup[i][1] = 0.75f;
				} else {
					if (WisEqual(vZA.y, -1.0f, g_EPSILON) || WisEqual(vZA.y, 1.0f, g_EPSILON)) {
						vXA = WCrossProduct(WVector::UNIT_POS_X, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					} else {
						vXA = WCrossProduct(WVector::UNIT_POS_Y, vZA).Normalize();
						vYA = WCrossProduct(vZA, vXA).Normalize();
					}
					mesh->uvBackup[i][0] = Wabs(vXA * mesh->normList[i]);
					mesh->uvBackup[i][1] = Wabs(vYA * mesh->normList[i]);
				}
			}
		}
	}
}

// wbone.cpp:3161
static void DebugTest(WView* view, const WVector& pos, unsigned long diffuse, float rlen) {
	view->DrawLine(pos - WVector(rlen, 0.0f, 0.0f), diffuse, pos + WVector(rlen, 0.0f, 0.0f), diffuse, 0);
	view->DrawLine(pos - WVector(0.0f, rlen, 0.0f), diffuse, pos + WVector(0.0f, rlen, 0.0f), diffuse, 0);
	view->DrawLine(pos - WVector(0.0f, 0.0f, rlen), diffuse, pos + WVector(0.0f, 0.0f, rlen), diffuse, 0);
}

// wbone.cpp:3187
void WBone::Render(WView* view, const LightSet& light) {
	WVector vWorLightDir, vLocLightDir, vLocEyePos;
	int i, k, n;
	float t, w;
	w_mesh* f;
	WVector vec, v;
	WTVertex* pv;
	int d0 = 0;

	if (IsVisible() && !m_flag.GetFlag(INVISIBLE) && !IsHidden()) {
		vWorLightDir = light.nearone * -1.0f;
		vLocLightDir = RotVec(vWorLightDir, ~GetMatrix());
		vLocEyePos = view->GetCamera().pivot * ~GetMatrix();

		for (f = m_mesh, n = 0; f != 0; f = f->next, ++n) {
			if (f->alpha == 0.0f || (view->IsShadowView() && (f->drawFlag & 0x300000) == 0x100000)) {
				continue;
			}
			if (view->ProcessEffect()) {
				if (f->bEnv) {
					CalcENVCoord(view, f);
				}
				if (f->bSpec) {
					CalcHilightCoord(view, light, f);
				}
			}

			i = 0;
			if (f->uvBackup != 0) {
				k = 0;
				if (f->blendedVecList != 0) {
					for (; i < f->blendedRigidNum; ++i) {
						t = 0.0f;
						w = f->blendWeightList[k];
						vec = f->blendedBoneList[k]->Transform(f->blendedVecList[k]) * w;
						t += w;
						for (++k; t < 0.999999f && k < f->blendedTotalNum; ++k) {
							WBone*& bone = f->blendedBoneList[k];
							w = f->blendWeightList[k];
							vec += bone->Transform(f->blendedVecList[k]) * w;
							t += w;
						}
						pv = GetVtxBuff(i);
						pv->tu = f->uvData[i][0];
						pv->tv = f->uvData[i][1];
						pv->lu = f->uvBackup[i][0];
						pv->lv = f->uvBackup[i][1];
						pv->diffuse = *f->vtxColorPtrList[i];
						pv->SetPosition(vec);
					}
				}
				for (; i < f->rigidNum; ++i) {
					pv = GetVtxBuff(i);
					pv->tu = f->uvData[i][0];
					pv->tv = f->uvData[i][1];
					pv->lu = f->uvBackup[i][0];
					pv->lv = f->uvBackup[i][1];
					pv->diffuse = *f->vtxColorPtrList[i];
					pv->SetPosition(f->boneList[i]->Transform(f->vecList[i]));
				}
				if (f->vtxNum - i > 0) {
					for (; i < f->vtxNum; ++i) {
						pv = GetVtxBuff(i);
						pv->diffuse = f->vtxColorList[i];
						pv->tu = f->uvData[i][0];
						pv->tv = f->uvData[i][1];
						pv->lu = f->uvBackup[i][0];
						pv->lv = f->uvBackup[i][1];
					}
					WTVertex* vertices;
					if (m_flag.GetFlag(PHYSICSMODEL)) {
						for (k = f->rigidNum; k < f->vtxNum; ++k) {
							DebugTest(view, m_physic->list[n][k], 0xffff0000, 1.5999999f);
						}
						vertices = GetVtxBuff();
						for (i = f->rigidNum; i < f->vtxNum; ++i) {
							vertices[i].SetPosition(m_physic->list[n][i]);
						}
					} else {
						vertices = GetVtxBuff();
						for (i = f->rigidNum; i < f->vtxNum; ++i) {
							vertices[i].SetPosition(f->vecList[i] * m_matrix);
						}
					}
				}
			} else {
				k = 0;
				if (f->blendedVecList != 0) {
					for (; i < f->blendedRigidNum; ++i) {
						t = 0.0f;
						w = f->blendWeightList[k];
						vec = f->blendedBoneList[k]->Transform(f->blendedVecList[k]) * w;
						t += w;
						for (++k; t < 0.999999f && k < f->blendedTotalNum; ++k) {
							WBone*& bone = f->blendedBoneList[k];
							w = f->blendWeightList[k];
							vec += bone->Transform(f->blendedVecList[k]) * w;
							t += w;
						}
						pv = GetVtxBuff(i);
						pv->tu = f->uvData[i][0];
						pv->tv = f->uvData[i][1];
						pv->diffuse = *f->vtxColorPtrList[i];
						pv->SetPosition(vec);
					}
				}
				for (; i < f->rigidNum; ++i) {
					pv = GetVtxBuff(i);
					pv->tu = f->uvData[i][0];
					pv->tv = f->uvData[i][1];
					pv->diffuse = *f->vtxColorPtrList[i];
					pv->SetPosition(f->boneList[i]->Transform(f->vecList[i]));
				}
				if (f->vtxNum - i > 0) {
					for (; i < f->vtxNum; ++i) {
						pv = GetVtxBuff(i);
						pv->diffuse = f->vtxColorList[i];
						pv->tu = f->uvData[i][0];
						pv->tv = f->uvData[i][1];
					}
					WTVertex* vertices;
					if (m_flag.GetFlag(PHYSICSMODEL)) {
						for (k = f->rigidNum; k < f->vtxNum; ++k) {
							DebugTest(view, m_physic->list[n][k], 0xffff0000, 1.5999999f);
						}
						vertices = GetVtxBuff();
						for (k = f->rigidNum; k < f->vtxNum; ++k) {
							vertices[k].SetPosition(m_physic->list[n][k]);
						}
					} else {
						vertices = GetVtxBuff();
						for (i = f->rigidNum; i < f->vtxNum; ++i) {
							vertices[i].SetPosition(f->vecList[i] * m_matrix);
						}
					}
				}
			}

			if (!view->IsShadowView() && (f->xiDrawFlag2 & 0x400) != 0) {
				if ((f->xiDrawFlag2 & 0x14) == 0) {
					WVector pos, normal;
					WVector* normList;
					float lightDot, eyeDot;
					normList = f->myBlendedNormalList ? f->myBlendedNormalList : f->blendedNormalList;
					for (i = k = 0; i < f->blendedRigidNum; ++i) {
						normal.Reset();
						pos.Reset();
						for (t = 0.0f; t < 0.999999f; ++k) {
							normal += RotVec(normList[k], f->blendedBoneList[k]->GetMatrix()) * f->blendWeightList[k];
							pos += f->blendedBoneList[k]->Transform(f->blendedVecList[k]) * f->blendWeightList[k];
							t = (float)(t + f->blendWeightList[k]);
						}
						eyeDot = (view->GetCamera().pivot - pos) * normal;
						lightDot = (float)(vWorLightDir * normal);
						if (lightDot * eyeDot < 0.0f) {
							WTVertex* vtx = GetVtxBuff(i);
							vtx->diffuse = (vtx->diffuse & 0xff000000) | light.ambient;
						}
					}
					normList = f->myNormalList ? f->myNormalList : f->normList;
					for (; i < f->rigidNum; ++i) {
						v = view->GetCamera().pivot * ~f->boneList[i]->GetMatrix();
						eyeDot = (v - f->vecList[i]) * normList[i];
						lightDot = (float)(vWorLightDir * RotVec(normList[i], f->boneList[i]->GetMatrix()));
						if (lightDot * eyeDot < 0.0f) {
							WTVertex* vtx = GetVtxBuff(i);
							vtx->diffuse = (vtx->diffuse & 0xff000000) | light.ambient;
						}
					}
					for (; i < f->vtxNum; ++i) {
						eyeDot = (vLocEyePos - f->vecList[i]) * normList[i];
						lightDot = vLocLightDir * normList[i];
						if (lightDot * eyeDot < 0.0f) {
							WTVertex* vtx = GetVtxBuff(i);
							vtx->diffuse = (vtx->diffuse & 0xff000000) | light.ambient;
						}
					}
				}
			}

			view->DrawIndexedTriangles(GetVtxBuff(), f->vtxNum, f->indexList, f->indexNum, f->drawFlag, f->xiDrawFlag2);
		}
	}

	if (m_next != 0) {
		m_next->Render(view, light);
	}
	if (m_child != 0) {
		m_child->Render(view, light);
	}
}

// wbone.cpp:3513
void WBone::RenderHierarchy(WView* view, float axisLen) {
	view->DrawLine(m_matrix.pivot, 0xffff0000, m_matrix.xa * axisLen + m_matrix.pivot, 0xffff0000, 0x300000);
	view->DrawLine(m_matrix.pivot, 0xff00ff00, m_matrix.ya * axisLen + m_matrix.pivot, 0xff00ff00, 0x300000);
	view->DrawLine(m_matrix.pivot, 0xff0000ff, m_matrix.za * axisLen + m_matrix.pivot, 0xff0000ff, 0x300000);

	if (m_child != 0) {
		view->DrawLine(m_matrix.pivot, 0xffffffff, m_child->m_matrix.pivot, 0xffff00ff, 0x300000);

		m_child->RenderHierarchy(view, axisLen);
	}

	if (m_next != 0) {
		view->DrawLine(m_matrix.pivot, 0xffffffff, m_next->m_matrix.pivot, 0xff00ffff, 0x300000);

		m_next->RenderHierarchy(view, axisLen);
	}
}

// wbone.cpp:3534
void WBone::RenderNormals(WView* view, float axisLen) {
	WVector v, n;
	float w, t;
	unsigned long c;
	int i, k;
	WVector* normList;
	for (w_mesh* m = m_mesh; m; m = m->next) {
		i = 0;
		normList = m->myBlendedNormalList ? m->myBlendedNormalList : m->blendedNormalList;
		if (m->blendedVecList && normList) {
			k = 0;
			for (; i < m->blendedRigidNum; i++) {
				c = WisEqual(normList[k], m->blendedNormalList[k], g_EPSILON) ? 0xffff0000 : 0xff00ff00;
				t = 0;
				w = m->blendWeightList[k];
				v = m->blendedBoneList[k]->Transform(m->blendedVecList[k]) * w;
				n = RotVec(normList[k], m->blendedBoneList[k]->GetMatrix()) * w;
				t += w;
				for (k++; t < 0.999999f && k < m->blendedTotalNum; k++) {
					w = m->blendWeightList[k];
					v += m->blendedBoneList[k]->Transform(m->blendedVecList[k]) * w;
					n += RotVec(normList[k], m->blendedBoneList[k]->GetMatrix()) * w;
					t += w;
				}
				n.Normalize();
				view->DrawLine(v, c, v + n * axisLen, c, 0x300000);
			}
		}
		normList = m->myNormalList ? m->myNormalList : m->normList;
		if (normList) {
			for (; i < m->rigidNum; i++) {
				c = WisEqual(normList[i], m->normList[i], g_EPSILON) ? 0xffff0000 : 0xff00ff00;
				v = m->boneList[i]->Transform(m->vecList[i]);
				n = RotVec(normList[i], m->boneList[i]->GetMatrix());
				n.Normalize();
				view->DrawLine(v, c, v + n * axisLen, c, 0x300000);
			}
			for (; i < m->vtxNum; i++) {
				c = WisEqual(normList[i], m->normList[i], g_EPSILON) ? 0xffff0000 : 0xff00ff00;
				v = m->vecList[i] * m_matrix;
				n = RotVec(normList[i], GetMatrix());
				n.Normalize();
				view->DrawLine(v, c, v + n * axisLen, c, 0x300000);
			}
		}
	}
	if (m_child) {
		m_child->RenderNormals(view, axisLen);
	}
	if (m_next) {
		m_next->RenderNormals(view, axisLen);
	}
}

// wbone.cpp:3625
void WBone::ApplyScale(float scale) {
	m_localMat.pivot *= scale;
	m_basicMat.pivot *= scale;
	m_matrix.pivot *= scale;
	if (m_keyPos != 0) {
		for (int i = 0; i < m_keyPosNum; ++i) {
			WVector& pos = m_keyPos[i].pos;
			pos *= scale;
		}
	}
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		int i;
		for (i = 0; i < mesh->blendedTotalNum; ++i) {
			WVector& vec = mesh->blendedVecList[i];
			vec *= scale;
		}
		for (i = 0; i < mesh->vtxNum; ++i) {
			WVector& vec = mesh->vecList[i];
			vec *= scale;
		}
	}
	if (m_next != 0) {
		m_next->ApplyScale(scale);
	}
	if (m_child != 0) {
		m_child->ApplyScale(scale);
	}
}

// wbone.cpp:3656
void WBone::SetBoundBox(const Waabb aabb, bool merge) {
	Waabb bound;
	if (merge) {
		bound.min = WVector(Min(m_bound_aabb.min.x, aabb.min.x),
			Min(m_bound_aabb.min.y, aabb.min.y),
			Min(m_bound_aabb.min.z, aabb.min.z));
		bound.max = WVector(Max(m_bound_aabb.max.x, aabb.max.x),
			Max(m_bound_aabb.max.y, aabb.max.y),
			Max(m_bound_aabb.max.z, aabb.max.z));
	} else {
		bound = aabb;
	}
	m_bound_aabb = bound;
	m_bound_sphere = WSphere((bound.max + bound.min) * 0.5f,
		(float)ceil((bound.max - bound.min).Magnitude() * 0.5f));
}

// wbone.cpp:3671
void WBone::FixNormal(const WMatrix& matrix) {
	WMatrix world;
	world = m_localMat * matrix;
	WMatrix inverse;
	inverse = ~world;
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = 0; i < mesh->vtxNum; i++) {
			WVector& normal = mesh->normList[i];
			normal = WVector(inverse.xx * normal.x + inverse.xz * normal.z + inverse.xy * normal.y,
				inverse.yx * normal.x + inverse.yz * normal.z + inverse.yy * normal.y,
				inverse.zx * normal.x + inverse.zz * normal.z + inverse.zy * normal.y);
		}
	}
	if (m_next != 0) {
		m_next->FixNormal(matrix);
	}
	if (m_child != 0) {
		m_child->FixNormal(world);
	}
}

// wbone.cpp:3691
WBone* WBone::FindBone(const char* name, unsigned long hashCode) {
	if (hashCode == 0) {
		hashCode = GetHashCode(name);
	}
	if (m_hashCode == hashCode && strcmp(name, m_name) == 0) {
		return this;
	}
	if (m_next != 0) {
		WBone* found = m_next->FindBone(name, hashCode);
		if (found != 0) {
			return found;
		}
	}
	if (m_child != 0) {
		WBone* found = m_child->FindBone(name, hashCode);
		if (found != 0) {
			return found;
		}
	}
	return 0;
}

// wbone.cpp:3713
WBone* WBone::FindBone(int id) {
	if (m_id == id) {
		return this;
	}
	WBone* bone;
	if (m_next != 0 && (bone = m_next->FindBone(id)) != 0) {
		return bone;
	}
	if (m_child != 0 && (bone = m_child->FindBone(id)) != 0) {
		return bone;
	}
	return 0;
}

// wbone.cpp:3729
WBone* WBone::FindBoneByTail(const char* tailName) {
	if (strstr(m_name, tailName) != 0) {
		return this;
	}
	WBone* bone;
	if (m_next != 0 && (bone = m_next->FindBoneByTail(tailName)) != 0) {
		return bone;
	}
	if (m_child != 0 && (bone = m_child->FindBoneByTail(tailName)) != 0) {
		return bone;
	}
	return 0;
}

// wbone.cpp:3746
WBone* WBone::FindFakeBodyBone(w_pet_vertex* list, int num) {
	if (strstr(m_name, "Bip") == 0 && strstr(m_name, "Jaw") == 0) {
		int i;
		for (i = 0; i < num; i++) {
			if (strcmp(list[i].bonename, m_name) == 0) {
				break;
			}
		}
		if (i == num && m_child == 0) {
			return this;
		}
	}
	WBone* bone;
	if (m_next != 0 && (bone = m_next->FindFakeBodyBone(list, num)) != 0) {
		return bone;
	}
	if (m_child != 0 && (bone = m_child->FindFakeBodyBone(list, num)) != 0) {
		return bone;
	}
	return 0;
}

// wbone.cpp:3770
int WBone::GetConvexArea(WBlockModel* manager) {
	w_mesh* mesh;
	int vtxcnt = 0;
	for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		vtxcnt += mesh->indexNum;
	}

	WVector* list = new WVector[vtxcnt];
	int i = 0;
	for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		for (int j = 0; j < mesh->indexNum; ++j) {
			list[i++] = mesh->vecList[mesh->indexList[j]] * GetMatrix();
		}
	}

	manager->AddTriangleList(list, vtxcnt);

	delete list;

	return 1;
}

// wbone.cpp:3793
void WBone::ApplySpecularMap(int texHandle) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (!mesh->bSpec) {
			mesh->bSpec = true;
			mesh->uvBackup = new float[mesh->vtxNum][2];
			mesh->texHandle |= (texHandle & 0x7f) << 11;
			mesh->drawFlag = (mesh->drawFlag & 0xfffff800) | mesh->texHandle;
		}
	}

	if (m_next != 0) {
		m_next->ApplySpecularMap(texHandle);
	}
	if (m_child != 0) {
		m_child->ApplySpecularMap(texHandle);
	}
}

// wbone.cpp:3822
int WriteID(int id, _iobuf* fp) {
	if (id >= 0xfe) {
		if (fp != 0) {
			fputc(0xfe, fp);
			fwrite(&id, 2, 1, fp);
		}
		return 3;
	}
	if (fp != 0) {
		fputc(id, fp);
	}
	return 1;
}

// wbone.cpp:3841
int WBone::SaveBoneName(_iobuf* fp, int type, char* name) {
	int written = 0;
	bool skip = name != 0 && m_mesh != 0 && stricmp(name, m_name) != 0;
	if (fp != 0) {
		if (!skip) {
			if (!((type == 0x82 || type == 0x66) && m_mesh != 0)) {
				fwrite(m_name, 1, strlen(m_name) + 1, fp);
				WriteID(m_parent != 0 ? m_parent->m_id : -1, fp);
			}
			if (type == 0xff || (type == 0x82 && m_mesh == 0) || type == 0x11b || type == 6) {
				WMatrix matrix;
				matrix = m_basicMat;
				if (type == 6) {
					for (WBone* bone = m_parent; bone != 0; bone = bone->m_parent) {
						matrix = matrix * bone->m_basicMat;
					}
					matrix.xm *= 3.125f;
					matrix.ym *= 3.125f;
					matrix.zm *= 3.125f;
				}
				fwrite(&matrix.xx, 4, 1, fp);
				fwrite(&matrix.yx, 4, 1, fp);
				fwrite(&matrix.zx, 4, 1, fp);
				fwrite(&matrix.xy, 4, 1, fp);
				fwrite(&matrix.yy, 4, 1, fp);
				fwrite(&matrix.zy, 4, 1, fp);
				fwrite(&matrix.xz, 4, 1, fp);
				fwrite(&matrix.yz, 4, 1, fp);
				fwrite(&matrix.zz, 4, 1, fp);
				fwrite(&matrix.xm, 4, 1, fp);
				fwrite(&matrix.ym, 4, 1, fp);
				fwrite(&matrix.zm, 4, 1, fp);
			}
		}
	} else {
		if (!skip) {
			if (!((type == 0x82 || type == 0x66) && m_mesh != 0)) {
				written = strlen(m_name) + WriteID(m_parent != 0 ? m_parent->GetID() : -1, 0) + 1;
			}
			if (type == 0xff || (type == 0x82 && m_mesh == 0) || type == 0x11b || type == 6) {
				written += 48;
			}
		}
	}
	if (m_child != 0) {
		written += m_child->SaveBoneName(fp, type, name);
	}
	if (m_next != 0) {
		written += m_next->SaveBoneName(fp, type, name);
	}
	return written;
}

// wbone.cpp:3920
int WBone::SaveAnimationKey(_iobuf* fp, int id, float (*range)[2], int count) {
	int written = 0;
	w_keyframe_pos* posKeys = 0;
	w_keyframe_rot* rotKeys = 0;
	int posNum, rotNum, i;
	float zero;
	if (m_keyPosNum != 0 || m_keyRotNum != 0) {
		if (m_keyPosNum > 0) {
			posNum = OptimizePosKey(range, count, &posKeys);
			if (m_keyPosNum == posNum) {
				posKeys = m_keyPos;
			}
		} else {
			posNum = 0;
		}
		if (m_keyRotNum > 0) {
			rotNum = OptimizeRotKey(range, count, &rotKeys);
			if (m_keyRotNum == rotNum) {
				rotKeys = m_keyRot;
			}
		} else {
			rotNum = 0;
		}
		if (fp != 0) {
			WriteID(m_id, fp);
			fwrite(&posNum, 4, 1, fp);
			for (i = 0; i < posNum; ++i) {
				fwrite(&posKeys[i].t, 4, 1, fp);
				fwrite(&posKeys[i].pos.x, 4, 1, fp);
				fwrite(&posKeys[i].pos.y, 4, 1, fp);
				fwrite(&posKeys[i].pos.z, 4, 1, fp);
			}
			fwrite(&rotNum, 4, 1, fp);
			w_keyframe_rot* rot = rotKeys;
			for (i = 0; i < rotNum; ++i) {
				fwrite(&rot[i].t, 4, 1, fp);
				fwrite(&rot[i].rot.x, 4, 1, fp);
				fwrite(&rot[i].rot.y, 4, 1, fp);
				fwrite(&rot[i].rot.z, 4, 1, fp);
				fwrite(&rot[i].rot.w, 4, 1, fp);
			}
			zero = 0.0f;
			fwrite(&zero, 4, 1, fp);
		}
		written = WriteID(m_id, 0) + posNum * 16 + rotNum * 20 + 12;
		if (m_keyPosNum > 0 && m_keyPos != posKeys) {
			delete[] posKeys;
		}
		if (m_keyRotNum > 0 && m_keyRot != rotKeys) {
			delete[] rotKeys;
		}
	}
	if (m_child != 0) {
		written += m_child->SaveAnimationKey(fp, id, range, count);
	}
	if (m_next != 0) {
		written += m_next->SaveAnimationKey(fp, id, range, count);
	}
	return written;
}

// wbone.cpp:4032
int WBone::SetID(int i, bool bCntMeshBone, char* meshboneName) {
	if (bCntMeshBone || m_mesh == 0) {
		if (meshboneName == 0 || m_mesh == 0 || stricmp(meshboneName, m_name) == 0) {
			m_id = i;
			i++;
		}
	}

	if (m_child != 0) {
		i = m_child->SetID(i, bCntMeshBone, meshboneName);
	}
	if (m_next != 0) {
		i = m_next->SetID(i, bCntMeshBone, meshboneName);
	}
	return i;
}

// wbone.cpp:4051
void WBone::CountBones(int& nBones) {
	nBones++;
	if (m_child != 0) {
		m_child->CountBones(nBones);
	}
	if (m_next != 0) {
		m_next->CountBones(nBones);
	}
}

// wbone.cpp:4062
float WBone::LoadAnimationKey(cFile* fp) {
	float t = 0.0f;
	int i;
	float n;
	WQuat rot;
	fp->Read(&m_keyPosNum, 4);
	if (m_keyPosNum != 0) {
		m_keyPos = new w_keyframe_pos[m_keyPosNum];
		m_keyFlag |= 2;
		for (i = 0; i < m_keyPosNum; ++i) {
			fp->Read(&m_keyPos[i].t, 4);
			fp->Read(&m_keyPos[i].pos.x, 4);
			fp->Read(&m_keyPos[i].pos.y, 4);
			fp->Read(&m_keyPos[i].pos.z, 4);
			if (t < m_keyPos[i].t) {
				t = m_keyPos[i].t;
			}
		}
	}
	fp->Read(&m_keyRotNum, 4);
	if (m_keyRotNum != 0) {
		m_keyFlag |= 1;
		m_keyRot = new w_keyframe_rot[m_keyRotNum];
		for (i = 0; i < m_keyRotNum; ++i) {
			fp->Read(&m_keyRot[i].t, 4);
			fp->Read(&rot.x, 4);
			fp->Read(&rot.y, 4);
			fp->Read(&rot.z, 4);
			fp->Read(&rot.w, 4);
			rot.Normalize();
			m_keyRot[i].rot = rot;
			if (t < m_keyRot[i].t) {
				t = m_keyRot[i].t;
			}
		}
	}
#ifdef PG_7XX
	// The 7xx .pet ANIM record adds a third key block after the rotation keys
	// (WBone has no scale key array, so it is skipped). The matching build
	// compiles the original line below.
	{
		int pgPos3Num = 0;
		fp->Read(&pgPos3Num, 4);
		for (int pgI = 0; pgI < pgPos3Num; ++pgI)
			fp->Seek(16, 1);
	}
#else
	fp->Read(&n, 4);
#endif
	return t;
}

// wbone.cpp:4174
void WBone::GetName(char** ppName, int& num) {
	if (m_parent != 0) {
		memcpy(*ppName, m_parent->m_name, strlen(m_parent->m_name));
	}
	*ppName += 30;
	memcpy(*ppName, m_name, strlen(m_name));
	*ppName += 30;
	num++;

	if (m_next != 0) {
		m_next->GetName(ppName, num);
	}
	if (m_child != 0) {
		m_child->GetName(ppName, num);
	}
}

// wbone.cpp:4193
void WBone::GetBoneInfo(int* piInfo) {
	w_mesh* mesh = m_mesh;
	while (mesh != 0) {
		piInfo[1] += mesh->vtxNum;
		piInfo[2] += mesh->indexNum / 3;
		mesh = mesh->next;
	}
	piInfo[0]++;
	if (m_next != 0) {
		m_next->GetBoneInfo(piInfo);
	}
	if (m_child != 0) {
		m_child->GetBoneInfo(piInfo);
	}
}

// wbone.cpp:4213
int WBone::GetOverlapBox(float* box, WBone* bone) {
	for (w_mesh* mesh = bone->m_mesh; mesh != 0; mesh = mesh->next) {
		for (int i = 0; i < mesh->indexNum; i += 3) {
			for (int k = 0; k < 3; ++k) {
				int index = mesh->indexList[i + k];
				WVector point;
				if (bone != this) {
					if (mesh->indexList[i] >= mesh->rigidNum) {
						continue;
					}
					if (mesh->boneList[index] != this) {
						continue;
					}
				}
				if (mesh->indexList[i] < mesh->rigidNum) {
					point = mesh->vecList[index] * mesh->boneList[index]->GetMatrix() * ~GetMatrix();
				} else {
					point = mesh->vecList[index];
				}
				box[0] = box[0] < point.x ? box[0] : point.x;
				box[1] = box[1] > point.x ? box[1] : point.x;
				box[2] = box[2] < point.y ? box[2] : point.y;
				box[3] = box[3] > point.y ? box[3] : point.y;
				box[4] = box[4] < point.z ? box[4] : point.z;
				box[5] = box[5] > point.z ? box[5] : point.z;
			}
		}
	}
	if (bone->m_next != 0) {
		GetOverlapBox(box, bone->m_next);
	}
	if (bone->m_child != 0) {
		GetOverlapBox(box, bone->m_child);
	}
	return 1;
}

// wbone.cpp:4256
void WBone::GetTotalOverlapBox(WVector* pvMin, WVector* pvMax, WMatrix* pmInvLocal, bool bVisibleMeshOnly) {
	WVector v;
	int i;
	w_mesh* mesh;

	if (pmInvLocal) {
		for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if (bVisibleMeshOnly && !IsMeshVisible(this, mesh)) {
				continue;
			}
			for (i = 0; i < mesh->vtxNum; ++i) {
				if (i < mesh->rigidNum) {
					v = mesh->boneList[i]->Transform(mesh->vecList[i]);
				} else {
					v = Transform(mesh->vecList[i]);
				}
				v = v * *pmInvLocal;
				pvMin->x = pvMin->x < v.x ? pvMin->x : v.x;
				pvMax->x = pvMax->x > v.x ? pvMax->x : v.x;
				pvMin->y = pvMin->y < v.y ? pvMin->y : v.y;
				pvMax->y = pvMax->y > v.y ? pvMax->y : v.y;
				pvMin->z = pvMin->z < v.z ? pvMin->z : v.z;
				pvMax->z = pvMax->z > v.z ? pvMax->z : v.z;
			}
		}
	} else {
		for (mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if (bVisibleMeshOnly && !IsMeshVisible(this, mesh)) {
				continue;
			}
			for (i = 0; i < mesh->vtxNum; ++i) {
				if (i < mesh->rigidNum) {
					v = mesh->boneList[i]->Transform(mesh->vecList[i]);
				} else {
					v = Transform(mesh->vecList[i]);
				}
				pvMin->x = pvMin->x < v.x ? pvMin->x : v.x;
				pvMax->x = pvMax->x > v.x ? pvMax->x : v.x;
				pvMin->y = pvMin->y < v.y ? pvMin->y : v.y;
				pvMax->y = pvMax->y > v.y ? pvMax->y : v.y;
				pvMin->z = pvMin->z < v.z ? pvMin->z : v.z;
				pvMax->z = pvMax->z > v.z ? pvMax->z : v.z;
			}
		}
	}

	if (m_next != 0) {
		m_next->GetTotalOverlapBox(pvMin, pvMax, pmInvLocal, bVisibleMeshOnly);
	}
	if (m_child != 0) {
		m_child->GetTotalOverlapBox(pvMin, pvMax, pmInvLocal, bVisibleMeshOnly);
	}
}

// wbone.cpp:4321
void WBone::Rotate(float angle, int axis, bool reset) {
	if (reset) {
		m_rotateMat.Reset();
	}
	m_rotateMat.Rotate(angle, axis);
	m_flag.Turn(ROTATEBONE, !reset || !(WAbsBySign(angle) < 1e-5f));
}

// wbone.cpp:4330
void WBone::Move(WVector& vec, bool recursive) {
	m_moveVec = vec;
	m_flag.Enable(MOVEBONE);
	if (recursive) {
		if (m_next != 0) {
			m_next->Move(vec, recursive);
		}
		if (m_child != 0) {
			m_child->Move(vec, recursive);
		}
	}
}

// wbone.cpp:4345
void WBone::SetSelfIllum(bool illum, bool recursive) {
	if (illum == true) {
		m_flag.Enable(SELFILLUM);
	} else {
		m_flag.Disable(SELFILLUM);
	}
	if (recursive) {
		if (m_next != 0) {
			m_next->SetSelfIllum(illum, recursive);
		}
		if (m_child != 0) {
			m_child->SetSelfIllum(illum, recursive);
		}
	}
}

// wbone.cpp:4359
void WBone::SetSelfIllumColor(unsigned long color, bool recursive) {
	m_selfillumColor = color & 0xffffff;
	if (recursive) {
		if (m_next != 0) {
			m_next->SetSelfIllumColor(color, recursive);
		}
		if (m_child != 0) {
			m_child->SetSelfIllumColor(color, recursive);
		}
	}
}

// wbone.cpp:4373
void WBone::SetInvisible(bool recursive, bool invisible) {
	if (invisible) {
		m_flag.Enable(INVISIBLE);
	} else {
		m_flag.Disable(INVISIBLE);
	}
	if (recursive) {
		if (m_next != 0) {
			m_next->SetInvisible(recursive, invisible);
		}
		if (m_child != 0) {
			m_child->SetInvisible(recursive, invisible);
		}
	}
}

// wbone.cpp:4389
void WBone::HideBone(bool recursive) {
	m_flag.Enable(HIDEBONE);
	if (recursive) {
		if (m_next != 0) {
			m_next->HideBone(recursive);
		}
		if (m_child != 0) {
			m_child->HideBone(recursive);
		}
	}
}

// wbone.cpp:4403
void WBone::ShowBone(bool recursive) {
	m_flag.Disable(HIDEBONE);
	if (recursive) {
		if (m_next != 0) {
			m_next->ShowBone(recursive);
		}
		if (m_child != 0) {
			m_child->ShowBone(recursive);
		}
	}
}

// wbone.cpp:4421
void WBone::AllocVTX(int len) {
	if (m_vtx_len < len) {
		m_vtx_len = len;
		if (m_vtxList != 0) {
			delete[] m_vtxList;
		}
		m_vtxList = new WTVertex[m_vtx_len];
	}
}

// wbone.cpp:4435
void WBone::CountVtxBuff(int inc) {
	m_vtx_count += inc;
	if (m_vtx_count == 0 && m_vtxList != 0) {
		delete[] m_vtxList;
		m_vtxList = 0;
		m_vtx_len = 0;
	}
}

// wbone.cpp:4447
void WBone::SetRigidVtxFlag() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->rigidNum != 0 || mesh->blendedRigidNum != 0) {
			m_flag.Enable(HASRIGIDVTX);
			break;
		}
	}

	if (m_next != 0) {
		m_next->SetRigidVtxFlag();
	}
	if (m_child != 0) {
		m_child->SetRigidVtxFlag();
	}
}

// wbone.cpp:4470
bool WBone::CompareBasicMatrix(WBone* otherRootbone) {
	if (!m_flag.GetFlag(MESHBONE)) {
		WBone* bone = otherRootbone->FindBone(m_name, 0);
		if (bone == 0) {
			return false;
		}
		if (!(m_basicMat == bone->m_basicMat)) {
			return false;
		}
	}

	if (m_child != 0) {
		if (!m_child->CompareBasicMatrix(otherRootbone)) {
			return false;
		}
	}
	if (m_next != 0) {
		if (!m_next->CompareBasicMatrix(otherRootbone)) {
			return false;
		}
	}
	return true;
}

// wbone.cpp:4498
void WBone::xSetAlpha(unsigned char ucAlpha, int iRecursive) {
	m_alpha = ucAlpha;
	if (ucAlpha == 0) {
		m_flag.Enable(1);
	}
	if (m_flag.GetFlag(1) && ucAlpha > 0) {
		m_flag.Disable(1);
	}
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		unsigned long value = (unsigned long)(int)(ucAlpha * mesh->alpha);

		if ((value & 0xff) == 0xff) {
			mesh->drawFlag = (((mesh->drawFlag & 0x1800000) == 0 && (mesh->flags & 0x400000) == 0) ? (mesh->drawFlag & ~0x400000) : (mesh->drawFlag | 0x400000)) & ~0x20000000;
		} else {
			mesh->drawFlag |= 0x20400000;
		}
	}
	if (iRecursive) {
		if (m_next != 0) {
			m_next->xSetAlpha(ucAlpha, 1);
		}
		if (m_child != 0) {
			m_child->xSetAlpha(ucAlpha, 1);
		}
	}
}

// wbone.cpp:4532
void WBone::xRender(WView* pView, WxBatchState* pBState, bool bNeedTransfMat, int hInstTexA, int hInstTexB) {
	if (IsVisible() && !IsInvisible() && !IsHidden()) {
		for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if (mesh->xpBatch == 0) {
				continue;
			}
			pBState->xiBaseVtxIdx = mesh->xpBatch->xiBaseVtxIdx;
			pBState->xiBaseIdxIdx = mesh->xpBatch->xiBaseIdxIdx;
			pBState->xnVtxs = mesh->vtxNum;
			pBState->xnIdxs = mesh->indexNum;
			if (bNeedTransfMat) {
				pBState->xmW = m_matrix;
				pBState->xnmTransfs = 0;
			}
			int drawFlag = mesh->drawFlag;
			if ((drawFlag & 0x7ff) == hInstTexA && hInstTexA != hInstTexB) {
				pBState->xiFlag0 = drawFlag ^ ((drawFlag ^ hInstTexB) & 0x7ff);
			} else {
				pBState->xiFlag0 = drawFlag;
			}
			int drawFlag2 = mesh->xiDrawFlag2;
			pBState->xiFlag1 = drawFlag2;
			if ((drawFlag2 & 4) != 0) {
				pBState->xdwDiffuse = ((unsigned long)m_alpha << 24) | pView->xGetLight()->ambient2;
			} else {
				pBState->xdwDiffuse = ((unsigned long)m_alpha << 24) | 0xffffff;
			}
			pView->xDrawIndexedTriangles(*pBState);
		}
	}
	if (m_next != 0) {
		m_next->xRender(pView, pBState, bNeedTransfMat, hInstTexA, hInstTexB);
	}
	if (m_child != 0) {
		m_child->xRender(pView, pBState, bNeedTransfMat, hInstTexA, hInstTexB);
	}
}

// wbone.cpp:4586
void WBone::ReplaceKeyframe(WBone* pSrc, float fAniLength) {
	WMatrix mSrc, mDst;
	mDst = m_localMat;
	mSrc = pSrc->m_localMat;

	m_keyFlag = 0;
	if (m_keyRot != 0) {
		delete[] m_keyRot;
		m_keyRot = 0;
	}
	m_keyRotNum = pSrc->m_keyRotNum;
	if (m_keyRotNum != 0) {
		m_keyRot = new w_keyframe_rot[m_keyRotNum];
		memcpy(m_keyRot, pSrc->m_keyRot, m_keyRotNum * sizeof(w_keyframe_rot));
		m_keyFlag |= 1;
		m_flag.Disable(0x40);
	} else {
		WQuat qSrc, qDst;
		qSrc = mSrc;
		qDst = mDst;
		if (!WisZero(Wabs(qSrc.x - qDst.x), g_EPSILON) ||
			!WisZero(Wabs(qSrc.y - qDst.y), g_EPSILON) ||
			!WisZero(Wabs(qSrc.z - qDst.z), g_EPSILON) ||
			!WisZero(Wabs(qSrc.w - qDst.w), g_EPSILON)) {
			m_keyRotNum = 2;
			m_keyRot = new w_keyframe_rot[2];
			m_keyRot[0].t = 0;
			m_keyRot[0].rot = qSrc;
			m_keyRot[1].t = fAniLength;
			m_keyRot[1].rot = qSrc;
			m_keyFlag |= 1;
			m_flag.Disable(0x40);
		}
	}

	if (m_keyPos != 0) {
		delete[] m_keyPos;
		m_keyPos = 0;
	}
	m_keyPosNum = pSrc->m_keyPosNum;
	if (m_keyPosNum != 0) {
		m_keyPos = new w_keyframe_pos[m_keyPosNum];
		memcpy(m_keyPos, pSrc->m_keyPos, m_keyPosNum * sizeof(w_keyframe_pos));
	} else {
		WVector vSrc = mSrc.pivot;
		WVector vDst = mDst.pivot;
		if (WisEqual(vSrc, vDst, 1e-5f)) {
			return;
		}
		m_keyPosNum = 2;
		m_keyPos = new w_keyframe_pos[2];
		m_keyPos[0].t = 0;
		m_keyPos[0].pos = vSrc;
		m_keyPos[1].t = fAniLength;
		m_keyPos[1].pos = vSrc;
	}
	m_keyFlag |= 2;
	m_flag.Disable(0x40);
}

// wbone.cpp:4674
WBone* WBone::AttachMeshBone(WBone* pSrc, int hTex, int hOrgTex) {
	m_next = new WBone;
	m_next->SetName(pSrc->m_name);
	m_next->m_id = 0;
	m_next->SetBasicMatrix(pSrc->m_basicMat);

	w_mesh* mesh = pSrc->m_mesh;
	w_mesh** tail = &m_next->m_mesh;
	while (mesh != 0) {
		if (!m_next->m_flag.GetFlag(MESHBONE | CLONED_MESH)) {
			m_next->m_flag.Enable(MESHBONE | CLONED_MESH);
		}
		*tail = m_next->CopyMesh(mesh);
		if (*tail != 0) {
			if (hTex != hOrgTex && ((*tail)->drawFlag & 0x7ff) == hOrgTex) {
				(*tail)->drawFlag = ((*tail)->drawFlag & ~0x7ff) | hTex;
			}
			if (!m_next->m_flag.GetFlag(HASRIGIDVTX) && ((*tail)->rigidNum != 0 || (*tail)->blendedRigidNum != 0)) {
				m_next->m_flag.Enable(HASRIGIDVTX);
			}
		}
		mesh = mesh->next;
		if (*tail != 0) {
			tail = &(*tail)->next;
		}
	}
	return m_next;
}

// wbone.cpp:4711
void WBone::ChangeTextureByHandle(int hTex, int hOrgTex, bool bNext, bool bChild) {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->originalTexHandle == hOrgTex) {
			mesh->drawFlag = (mesh->drawFlag & 0xfffff800) | hTex;
		}
	}

	if (bNext && m_next != 0) {
		m_next->ChangeTextureByHandle(hTex, hOrgTex, bNext, bChild);
	}
	if (bChild && m_child != 0) {
		m_child->ChangeTextureByHandle(hTex, hOrgTex, bNext, bChild);
	}
}

// wbone.cpp:4728
bool WBone::ChangeTexture(const char* boneName, int hOrgTex, int hDstTex) {
	if (boneName == 0 || hOrgTex == 0 || hDstTex == 0) {
		return false;
	}
	unsigned long hash = GetHashCode(boneName);
	if (m_hashCode == hash) {
		if (strcmp(boneName, m_name) == 0) {
			for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
				if (mesh->originalTexHandle == hOrgTex) {
					mesh->drawFlag = (mesh->drawFlag & 0xfffff800) | hDstTex;
				}
			}
			return true;
		}
	}

	if (m_next != 0) {
		if (m_next->ChangeTexture(boneName, hOrgTex, hDstTex)) {
			return true;
		}
	}
	if (m_child != 0) {
		if (m_child->ChangeTexture(boneName, hOrgTex, hDstTex)) {
			return true;
		}
	}
	return false;
}

// wbone.cpp:4763
bool WBone::ChangeTexture(const char* boneName, int hTex) {
	if (boneName == 0 || hTex == 0) {
		return false;
	}
	unsigned long hash = GetHashCode(boneName);
	if (m_hashCode == hash) {
		if (strcmp(boneName, m_name) == 0) {
			for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
				if (mesh->originalTexHandle != 0) {
					mesh->drawFlag = (mesh->drawFlag & 0xfffff800) | hTex;
				}
			}
			return true;
		}
	}

	if (m_next != 0) {
		if (m_next->ChangeTexture(boneName, hTex)) {
			return true;
		}
	}
	if (m_child != 0) {
		if (m_child->ChangeTexture(boneName, hTex)) {
			return true;
		}
	}
	return false;
}

// wbone.cpp:4832
void WBone::ResetMyNormalList() {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->myNormalList != 0) {
			memcpy(mesh->myNormalList, mesh->normList, mesh->vtxNum * sizeof(WVector));
		}
		if (mesh->myBlendedNormalList != 0) {
			memcpy(mesh->myBlendedNormalList, mesh->blendedNormalList, mesh->blendedTotalNum * sizeof(WVector));
		}
	}
}

// wbone.cpp:4848
void WBone::GatherNormalMergeInfo(w_normalmerge& info) const {
	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		if (mesh->weightList != 0) {
			for (int i = 0; i < mesh->vtxNum; i++) {
				if (mesh->weightList[i] != 1.0f) {
					info.AddVertex(mesh, i);
				}
			}
		}
	}
}

// wbone.cpp:4870
void WBone::xBuildTransfMatPtrList(std::vector<const WMatrix*>& matPtrList) const {
	if (!m_flag.GetFlag(MESHBONE)) {
		matPtrList[m_id] = &m_matrix;
	}
	if (m_child != 0) {
		m_child->xBuildTransfMatPtrList(matPtrList);
	}
	if (m_next != 0) {
		m_next->xBuildTransfMatPtrList(matPtrList);
	}
}

// wbone.cpp:4886
void WBone::SetMirrored(bool bMirrored) {
	if (bMirrored) {
		m_flag.Enable(MIRRORED);
	} else {
		m_flag.Disable(MIRRORED);
	}

	for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
		mesh->xiDrawFlag2 &= ~0xc00;

		int drawFlag = mesh->xiDrawFlag2;
		if (bMirrored) {
			drawFlag |= 0x800;
		}

		mesh->xiDrawFlag2 = drawFlag;
	}

	if (m_next != 0) {
		m_next->SetMirrored(bMirrored);
	}
	if (m_child != 0) {
		m_child->SetMirrored(bMirrored);
	}
}

// wbone.cpp:4915
void WBone::SetScale(float scale) {
	m_bonescale = scale;
	m_flag.Enable(SCALEBONE);
}

// wbone.cpp:4921
void WBone::CalcMeshAABB(WBone& bone, w_mesh& mesh) {
	int i;
	if (mesh.vtxNum <= 0) {
		return;
	}
	if (mesh.rigidNum == 0) {
		mesh.aabbBone = &bone;
	} else {
		std::map<WBone*, float> boneInfluenceList;
		for (i = 0; i < mesh.rigidNum; ++i) {
			std::map<WBone*, float>::iterator found;
			found = boneInfluenceList.find(mesh.boneList[i]);
			if (found == boneInfluenceList.end()) {
				boneInfluenceList.insert(std::pair<WBone* const, float>(mesh.boneList[i], 1.0f));
			} else {
				(*found).second = (*found).second + 1.0f;
			}
		}
		if (i < mesh.vtxNum) {
			boneInfluenceList.insert(std::pair<WBone* const, float>(&bone, 1.0f));
			for (; i < mesh.vtxNum; ++i) {
				boneInfluenceList[&bone] += 1.0f;
			}
		}
		if (boneInfluenceList.size() > 0) {
			std::map<WBone*, float>::const_iterator boneItr;
			std::map<WBone*, float>::const_iterator scan;
			boneItr = boneInfluenceList.begin();
			scan = boneItr;
			for (scan++; scan != boneInfluenceList.end(); scan++) {
				if ((*scan).second > (*boneItr).second) {
					boneItr = scan;
				}
			}
			mesh.aabbBone = (*boneItr).first;
		}
	}

	WMatrix inverse = ~mesh.aabbBone->m_matrix;
	WBone* prevBone = 0;
	WMatrix m;
	WVector v;
	mesh.localAabb.Clear();

	i = 0;
	for (; i < mesh.rigidNum; ++i) {
		if (mesh.boneList[i] != mesh.aabbBone) {
			if (mesh.boneList[i] != prevBone) {
				prevBone = mesh.boneList[i];
				m = mesh.boneList[i]->GetMatrix() * inverse;
			}
			v = mesh.vecList[i] * m;
		} else {
			v = mesh.vecList[i];
		}
		mesh.localAabb.AddPoint(v);
	}
	if (&bone != mesh.aabbBone) {
		m = bone.m_matrix * inverse;
	}
	for (; i < mesh.vtxNum; ++i) {
		if (&bone != mesh.aabbBone) {
			v = mesh.vecList[i] * m;
		} else {
			v = mesh.vecList[i];
		}
		mesh.localAabb.AddPoint(v);
	}
}

// wbone.cpp:5011
bool WBone::IsMeshVisible(WBone* pBone, w_mesh* pMesh) {
	return pBone->IsVisible() && !pBone->IsInvisible() && !pBone->IsHidden() && pMesh->alpha > 0.0f;
}

// wbone.cpp:5019
bool WBone::CheckMeshBoneAllVertexRigid() const {
	if (m_flag.GetFlag(MESHBONE)) {
		for (w_mesh* mesh = m_mesh; mesh != 0; mesh = mesh->next) {
			if (mesh->rigidNum < mesh->vtxNum) {
				return false;
			}
		}
	}

	if (m_next != 0) {
		if (!m_next->CheckMeshBoneAllVertexRigid()) {
			return false;
		}
	}
	if (m_child != 0) {
		if (!m_child->CheckMeshBoneAllVertexRigid()) {
			return false;
		}
	}
	return true;
}

// wbone.cpp:5047
WBone::w_normalmerge::w_normalmerge(WPuppet& pet) {
	WBone* bone;
	for (bone = pet.FindMeshBone(0); bone != 0; bone = pet.FindMeshBone(bone)) {
		bone->ResetMyNormalList();
	}

	for (bone = pet.FindMeshBone(0); bone != 0; bone = pet.FindMeshBone(bone)) {
		bone->GatherNormalMergeInfo(*this);
	}

	MergeNormal();
}

// wbone.cpp:5063
void WBone::w_normalmerge::AddVertex(w_mesh* m, int idx) {
	PLIST& plist = m_wlist[m->weightList[idx]];
	P* p = Find(plist, m->vecList[idx]);
	if (p == 0) {
		plist.push_back(P(m->vecList[idx]));
		p = &*plist.rbegin();
	}

	NLIST& nlist = p->m_meshList[m];
	N* n = Find(nlist, m->normList[idx]);
	if (n == 0) {
		nlist.push_back(N(m->normList[idx]));
		n = &*nlist.rbegin();
	}

	n->m_idxList.push_back(idx);
}

// wbone.cpp:5162
void WBone::w_normalmerge::MergeNormal() {
	WLIST::const_iterator w;
	for (w = m_wlist.begin(); w != m_wlist.end(); w++) {
		PLIST::const_iterator p;
		for (p = (*w).second.begin(); p != (*w).second.end(); p++) {
			MLIST::const_iterator m;
			for (m = (*p).m_meshList.begin(); m != (*p).m_meshList.end(); m++) {
				w_mesh* mesh = (*m).first;
				if (mesh->myNormalList == 0) {
					continue;
				}
				NLIST::const_iterator n;
				for (n = (*m).second.begin(); n != (*m).second.end(); n++) {
					int index = *(*n).m_idxList.begin();
					bool merged = false;
					MLIST::const_iterator other = m;
					for (other++; other != (*p).m_meshList.end(); other++) {
						w_mesh* mesh2 = (*other).first;
						if (mesh2->myNormalList == 0) {
							continue;
						}
						if (stricmp(mesh->mpetName, mesh2->mpetName) == 0) {
							continue;
						}
						NLIST::const_iterator n2;
						for (n2 = (*other).second.begin(); n2 != (*other).second.end(); n2++) {
							merged = true;
							int index2 = *(*n2).m_idxList.begin();
							mesh->myNormalList[index] += mesh2->normList[index2];
							mesh2->myNormalList[index2] += (*n).m_n;
							SetMyNormalsEqualToFirstNormal(mesh2, (*n2).m_idxList);
						}
					}
					if (merged) {
						SetMyNormalsEqualToFirstNormal(mesh, (*n).m_idxList);
					}
				}
			}
		}
	}
	NormalizeMergedNormals();
	ApplyToBlendedNormalList();
}

// wbone.cpp:5259
void WBone::w_normalmerge::NormalizeMergedNormals() {
	WLIST::const_iterator w;
	PLIST::const_iterator p;
	MLIST::const_iterator m;
	NLIST::const_iterator n;
	for (w = m_wlist.begin(); w != m_wlist.end(); w++) {
		for (p = (*w).second.begin(); p != (*w).second.end(); p++) {
			for (m = (*p).m_meshList.begin(); m != (*p).m_meshList.end(); m++) {
				w_mesh* mesh = (*m).first;
				if ((*m).first->myNormalList) {
					for (n = (*m).second.begin(); n != (*m).second.end(); n++) {
						for (std::vector<int>::const_iterator i = (*n).m_idxList.begin(); i != (*n).m_idxList.end(); i++) {
							if (WisEqual(mesh->myNormalList[*i], mesh->normList[*i], 1e-05f) == 0) {
								mesh->myNormalList[*i].Normalize();
							}
						}
					}
				}
			}
		}
	}
}

// wbone.cpp:5308
void WBone::w_normalmerge::ApplyToBlendedNormalList() {
	w_mesh* mesh;
	int slot, i;
	float weight;
	std::list<w_mesh*> meshList;
	WLIST::const_iterator w;
	for (w = m_wlist.begin(); w != m_wlist.end(); w++) {
		PLIST::const_iterator p;
		for (p = (*w).second.begin(); p != (*w).second.end(); p++) {
			MLIST::const_iterator m;
			for (m = (*p).m_meshList.begin(); m != (*p).m_meshList.end(); m++) {
				mesh = (*m).first;
				if (std::find(meshList.begin(), meshList.end(), mesh) == meshList.end()) {
					meshList.push_back(mesh);
				}
			}
		}
	}
	std::list<w_mesh*>::const_iterator it;
	for (it = meshList.begin(); it != meshList.end(); it++) {
		mesh = *it;
		const std::vector<WMatrix>& matrixList = WPuppet::GetOrginalMatrixList(mesh->mpetName);
		slot = 0;
		for (i = 0; i < mesh->blendedRigidNum; ++i) {
			if (mesh->weightList[i] == 1.0f || WisEqual(mesh->myNormalList[i], mesh->normList[i], 1e-05f) != 0) {
				for (weight = 0.0f; weight < 0.999f; ++slot) {
					weight += mesh->blendWeightList[slot];
				}
			} else {
				for (weight = 0.0f; weight < 0.999f; ++slot) {
					int rigid = mesh->boneList[i]->GetID();
					int blended = mesh->blendedBoneList[slot]->GetID();
					if (rigid >= 0 && rigid < (int)matrixList.size() && blended >= 0 && blended < (int)matrixList.size()) {
						const WMatrix* matrices = &matrixList[0];
						mesh->myBlendedNormalList[slot] = RotVec(mesh->myNormalList[i], matrices[rigid] * ~matrixList[blended]);
					}
					weight += mesh->blendWeightList[slot];
				}
			}
		}
	}
}

// wbone.cpp:5377
WBone::w_normalmerge::P* WBone::w_normalmerge::Find(PLIST& plist, const WVector& p) {
	for (PLIST::iterator it = plist.begin(); it != plist.end(); ++it) {
		if (WisEqual(it->m_p, p, 0.001f)) {
			return &*it;
		}
	}
	return 0;
}

// wbone.cpp:5390
WBone::w_normalmerge::N* WBone::w_normalmerge::Find(NLIST& nlist, const WVector& n) {
	for (NLIST::iterator it = nlist.begin(); it != nlist.end(); ++it) {
		if (WisEqual(it->m_n, n, 0.001f)) {
			return &*it;
		}
	}
	return 0;
}

// wbone.cpp:5403
void WBone::w_normalmerge::SetMyNormalsEqualToFirstNormal(w_mesh* m, const std::vector<int>& idxList) {
	if (idxList.size() > 1) {
		int first = idxList[0];
		for (std::vector<int>::const_iterator it = idxList.begin(); it != idxList.end(); ++it) {
			m->myNormalList[*it] = m->myNormalList[first];
		}
	}
}
