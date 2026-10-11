// Original: d:\build\custom\temp\code\client\wangreal\source\wpuppet.cpp
#define WPUPPET_WBONE_INLINE
#include "../include/wpuppet.h"
#include "../include/wbone.h"
#include "../include/wboneset.h"
#include "../include/westpak.h"
#include "../include/wresrcmng.h"
#include "../include/wmemblock.h"
#include "../include/rectmng.h"
#include "../include/wutil.h"
#include "../include/wview.h"
#include "../include/wpetfile.h"
#include "../include/bitmap.h"

// wbone.h:367 (inline in the class body of the original wbone.h; local until wbone.h defines it)
inline char* WBone::GetBoneMotionName() {
	return m_motion_bone_name;
}

// wmathutil.cpp
void __cdecl MakePlaneEq(WPlane* plane, const Waabb& aabb, const WMatrix* pmtxWorld);

// wboneset.cpp
WBoneSet* __cdecl BlendBoneSet(WBoneSet* a, WBoneSet* b, float t);

// wpuppet.cpp:29
WMemFillBlock g_mem(0x4800, 5);

WList<WPuppet::w_tex_piece*>* WPuppet::m_tex_piece;
unsigned char WPuppet::ms_lockedBoneSetPos = 0xff;

int WPuppet::box_v[8][3] = {
	{ 0, 0, 0 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 },
	{ 0, 0, 1 }, { 1, 0, 1 }, { 0, 1, 1 }, { 1, 1, 1 }
};
int WPuppet::box_p[6][4] = {
	{ 0, 2, 3, 1 }, { 4, 5, 7, 6 }, { 0, 1, 5, 4 },
	{ 1, 3, 7, 5 }, { 3, 2, 6, 7 }, { 2, 0, 4, 6 }
};
unsigned char WPuppet::ms_boneSetPos;

// wpuppet.cpp:46
WPuppet::WPuppet() : m_phybone(8, 0), m_addedMotionList(4, 0), m_use_tex_piece(8, 0), m_bbList(8, 8), m_fanimList(4, 4), m_boneList(4, 16) {
	m_bClone = false;
	memset(m_petName, 0, sizeof(m_petName));

	m_frame = NULL;
	m_framenum = 0;

	m_storeTexList = NULL;

	m_rendMode = 0;

	m_aniLen = 0;
	m_rootbone = NULL;

	m_lightmode = 0;

	m_mdList = NULL;

	m_bound = Waabb(WVector::ZERO, WVector::ZERO);
	m_boundSphere = WSphere(WVector::ZERO, 0);

	m_BBox.bone = NULL;
	m_BBox.info = NULL;
	m_faceNum = 0;
	m_mat.Reset();
}

// wpuppet.cpp:85
WPuppet::~WPuppet() {
	for (w_face_animation* anim = m_fanimList.Start(); anim; anim = m_fanimList.Next()) {
		g_mem.Free(anim->mesh);
		delete anim;
	}

	for (w_bound_box* box = m_bbList.Start(); box; box = m_bbList.Next()) {
		if (!m_bClone) {
			if (box->info->option) {
				g_mem.Free(box->info->option);
			}
			g_mem.Free(box->info);
		}
		g_mem.Free(box);
	}

	if (!m_bClone) {
		if (m_mdList) {
			for (w_motion_data* data = m_mdList->Start(); data; data = m_mdList->Next()) {
				delete[] data;
			}
			if (m_mdList) {
				delete m_mdList;
				m_mdList = NULL;
			}
		}
		if (m_framenum > 0) {
			for (int i = 0; i < m_framenum; ++i) {
				g_mem.Free(m_frame[i].data);
			}
			g_mem.Free(m_frame);
		}
	}

	if (m_rootbone) {
		delete m_rootbone;
		m_rootbone = NULL;
	}

	if (m_storeTexList) {
		for (int handle = m_storeTexList->Start(); handle; handle = m_storeTexList->Next()) {
			GetResrcManager()->Release(handle);
		}
		delete m_storeTexList;
	}

	ClearAddMotion();
	ClearTexPiece();

	if (m_tex_piece && !m_tex_piece->Start()) {
		delete m_tex_piece;
		m_tex_piece = NULL;
	}
}

// wpuppet.cpp:177
void WPuppet::Share(w_share_pet_data* share) {
	m_bClone = true;
	strcpy(m_petName, share->petName);
	m_frame = share->frame;
	m_framenum = share->numFramedata;
	m_mdList = share->mdList;
	m_bound = share->bound;
	m_faceNum = share->faceNum;
	m_aniLen = share->length;
	m_rootbone = share->rootbone->MakeClone(0);
	m_rootbone->ResetMeshBone(m_rootbone, true);
	m_rootbone->CombineVtxColor(true);

	for (w_bound_box* box = share->bbList->Start(); box != NULL; box = share->bbList->Next()) {
		w_bound_box* copy = (w_bound_box*)g_mem.Alloc(sizeof(w_bound_box));
		copy->info = box->info;
		copy->bone = box->bone != NULL ? m_rootbone->FindBone(box->bone->GetBoneName(), 0) : NULL;
		m_bbList.AddItem(copy, copy->info->name, false);
	}

	w_bound_box* found = share->bbList->Find("bound");
	if (found == NULL) {
		found = share->bbList->Find("center");
	}
	if (found != NULL) {
		m_BBox.bone = found->bone;
		m_BBox.info = found->info;
	}
	if (m_BBox.bone != NULL) {
		m_BBox.bone = m_rootbone->FindBone(m_BBox.bone->GetBoneName(), 0);
	}

	for (w_face_animation* anim = share->fanimList->Start(); anim != NULL; anim = share->fanimList->Next()) {
		w_face_animation* copy = new w_face_animation;
		*copy = *anim;
		copy->mesh = (w_mesh**)g_mem.Alloc(anim->meshNum * sizeof(w_mesh*) + sizeof(w_mesh*));
		copy->mesh[anim->meshNum] = NULL;
		if (anim->meshNum > 0) {
			m_rootbone->GetMeshForFaceAnimation((unsigned char)copy->group, copy->mesh, 0);
		}
		m_fanimList.AddItem(copy, copy->name, false);
	}

	SetRenderMode(m_rendMode);
}

// wpuppet.cpp:274
const WMatrix& WPuppet::GetCamMatrix(char* boneName) {
	static WMatrix convMat(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	static WMatrix mat;

	mat = convMat * FindBone(boneName)->m_matrix;

	return mat;
}

// wpuppet.cpp:317
float WPuppet::GetCamFOV(char* name, WBoneSet* boneSet) {
	WBoneKey* key = boneSet->FindBoneKey(name);

	if (key && (key->flags & 4)) {
		return key->scale;
	}

	return 0.0f;
}

// wpuppet.cpp:327
WPuppet* WPuppet::MakeClone(bool bNeedTnLBuf) {
	w_share_pet_data share;

	share.petName = m_petName;
	share.rootbone = m_rootbone;
	share.length = m_aniLen;
	share.faceNum = m_faceNum;
	share.numFramedata = m_framenum;
	share.frame = m_frame;
	share.bound = m_bound;

	share.bbList = &m_bbList; share.mdList = m_mdList;
	share.fanimList = &m_fanimList;

	WPuppet* clone = new WPuppet;
	clone->m_iPetType = m_iPetType;
	clone->m_rendMode = m_rendMode;
	clone->Share(&share);

	return clone;
}

// wpuppet.cpp:358
void WPuppet::SetRenderMode(int mode) {
	m_rendMode = mode;

	if (m_rootbone) {
		m_rootbone->SetRenderMode(mode);
	}
}

// wpuppet.cpp:370
void WPuppet::Transform(WView* view, float t, int flag) {
	m_rootbone->Transform(m_mat, view, t, flag);
}

// wpuppet.cpp:375
void WPuppet::UpdateLightSource(WScene* scene) {
	m_rootbone->CalcLight(scene, m_lightmode);
}

// wpuppet.cpp:382
void WPuppet::UpdateLightSource(LightSet* light, bool bFlag, WScene* scene) {
	m_rootbone->SetLight(light);
	m_rootbone->CalcLight(light, true, bFlag ? m_lightmode | 0x8000000 : m_lightmode, scene);
}

// wpuppet.cpp:388
void WPuppet::Render(WView* view, bool bTransform, float t, bool bFlag1, bool bFlag2, bool bFlag3) {
	if (bTransform) {
		m_rootbone->Transform(m_mat, view, t, (bFlag1 ? 1 : 0) | (bFlag2 ? 0 : 2));
	}
	m_rootbone->Render(view, m_rootbone->m_light);
}

// wpuppet.cpp:417
void WPuppet::RenderHierarchy(WView* view, bool bTransform, float t, float size) {
	if (bTransform) {
		m_rootbone->Transform(m_mat, view, t, 0);
	}
	m_rootbone->RenderHierarchy(view, size);
}

// wpuppet.cpp:425
void WPuppet::RenderNormals(WView* view, bool bTransform, float t, float size) {
	if (bTransform) {
		m_rootbone->Transform(m_mat, view, t, 0);
	}
	m_rootbone->RenderNormals(view, size);
}

// wpuppet.cpp:441
float WPuppet::GetMotionLength(const char* actionName) {
	if (actionName) {
		w_motion_data* md = FindMotionData(actionName);
		if (md == NULL) {
			return -1.0f;
		}
		return md->end - md->s;
	}
	return m_aniLen;
}

// wpuppet.cpp:458
WBoneSet* WPuppet::GetBones(const char* actionName, float time, int type, char* applybone, bool lockBoneSet) {
	if (actionName != NULL) {
		w_motion_data* md = FindMotionData(actionName);
		if (applybone == NULL && *md->applybone != 0) {
			applybone = md->applybone;
		}
		time = time + md->s;
		if (!(time > md->e)) {
			lockBoneSet = false;
		} else {
			float t = (time - md->e) / (md->end - md->e);
			switch (md->option1) {
			case 2:
				return BlendBoneSet(GetBones(0, time, type, applybone, false), GetBones(md->nextmotion, time - md->e, type, applybone, false), t);
			case 1:
				return BlendBoneSet(GetBones(0, md->e, type, applybone, false), GetBones(md->nextmotion, 0.0f, type, applybone, false), t);
			}
		}
	}
	if (time > m_aniLen) {
		time = m_aniLen;
	}
	WBoneSet* boneset = GetFreeBoneSetList(lockBoneSet);
	for (w_motion_addition* add = m_addedMotionList.Start(); add != NULL; add = m_addedMotionList.Next()) {
		if (add->s <= time && time <= add->e) {
			if (applybone != NULL) {
				WBone* found = add->rootbone->FindBone(applybone, 0);
				if ((type & 4) == 0) {
					found->InsertBoneSetList(time - add->s, boneset, 1);
				} else {
					for (WBone* child = found->m_child; child != NULL; child = child->m_next) {
						child->InsertBoneSetList(time - add->s, boneset, 1);
					}
				}
			} else {
				for (WBone* item = add->rootbone; item != NULL; item = item->m_next) {
					item->InsertBoneSetList(time - add->s, boneset, 1);
				}
			}
			goto keyed;
		}
	}
	if (applybone != NULL) {
		WBone* found = m_boneList.Find(applybone);
		if (found == NULL) {
			found = m_rootbone->FindBone(applybone, 0);
			if (found != NULL) {
				m_boneList.AddItem(found, found->m_name, false);
			}
		}
		if ((type & 4) == 0) {
			found->InsertBoneSetList(time, boneset, 1);
		} else {
			for (WBone* child = found->m_child; child != NULL; child = child->m_next) {
				child->InsertBoneSetList(time, boneset, 1);
			}
		}
	} else {
		for (WBone* item = m_rootbone; item != NULL; item = item->m_next) {
			item->InsertBoneSetList(time, boneset, 1);
		}
	}
keyed:
	if (type & 1) {
		WBoneKey* key = boneset->FindBoneKey(m_rootbone->GetBoneMotionName());
		if (key != NULL && (key->flags & 2) != 0) {
			key->pivot = m_rootbone->GetBasicMatrix().pivot;
		}
	}
	return boneset;
}

// wpuppet.cpp:580
w_motion_data* WPuppet::FindMotionData(const char* motionName) {
	if (m_mdList == NULL) {
		return NULL;
	}
	w_motion_data* md = m_mdList->Find(motionName);
	if (md == NULL) {
		for (w_motion_addition* add = m_addedMotionList.Start(); add != NULL && md == NULL; add = m_addedMotionList.Next()) {
			md = add->mdList->Find(motionName);
		}
	}
	return md;
}

// wpuppet.cpp:599
int WPuppet::GetMotionCountIncludeName(const char* motionName, const char* exceptName) {
	if (m_mdList == NULL) {
		return 0;
	}
	int iCount = 0;
	w_motion_data* md;
	for (md = m_mdList->Start(); md; md = m_mdList->Next()) {
		if (strstr(md->name, motionName) && !strstr(md->name, exceptName)) {
			iCount++;
		}
	}
	if (iCount == 0) {
		// The added motions' own lists are never searched: m_mdList is walked again.
		for (w_motion_addition* add = m_addedMotionList.Start(); add && md == NULL; add = m_addedMotionList.Next()) {
			for (md = m_mdList->Start(); md; md = m_mdList->Next()) {
				if (strstr(md->name, motionName) && !strstr(md->name, exceptName)) {
					iCount++;
				}
			}
		}
	}
	return iCount;
}

// wpuppet.cpp:629
char* WPuppet::GetNextMotionName(const char* actionName) {
	w_motion_data* md = FindMotionData(actionName);
	return (md->s == md->e && strcmp(md->name, md->nextmotion) == 0) ? NULL : md->nextmotion;
}

// wpuppet.cpp:640
void WPuppet::ApplyBones(WBoneSet* boneSet) {
	if (m_rootbone) {
		m_rootbone->ApplyBoneSetList(boneSet);
	}
}

// wpuppet.cpp:646
void WPuppet::ApplyBones(WBoneSet* boneSet, const WMatrix& mat) {
	m_mat = mat;
	if (boneSet && m_rootbone) {
		m_rootbone->ApplyBoneSetList(boneSet);
	}
}

// wpuppet.cpp:654
WVector WPuppet::GetDeltaVec(float t) {
	for (w_motion_addition* ma = m_addedMotionList.Start(); ma; ma = m_addedMotionList.Next()) {
		if (ma->s <= t && t <= ma->e) {
			return strcmp(ma->rootbone->GetBoneMotionName(), m_rootbone->GetBoneMotionName()) == 0 ? ma->rootbone->GetDeltaVec(t - ma->s) + WVector(0, ma->h_gap, 0) : WVector::ZERO;
		}
	}

	return m_rootbone->GetDeltaVec(t);
}

// wpuppet.cpp:667
WVector WPuppet::GetDeltaVec(char* animName, float t) {
	w_motion_data* md = FindMotionData(animName);
	return GetDeltaVec(t + md->s);
}

// wpuppet.cpp:674
WVector WPuppet::GetDeltaVec(char* actionName, float p1, float p2) {
	if (p1 == p2) {
		return WVector::ZERO;
	}

	w_motion_data* md;
	if (actionName) {
		md = FindMotionData(actionName);

		float e = md->e - md->s;
		float span = e > p2 ? p2 : e;
		if (p2 > span) {
			if (p1 < span) {
				return GetDeltaVec(actionName, p1, span) + GetDeltaVec(actionName, span, p2);
			}
			if (p1 >= span) {
				float t = (p2 - md->e) / (md->end - md->e);
				switch (md->option1) {
				case 2:
					return GetDeltaVec(0, p1 + md->s, p2 + md->s) * (1.0f - t) + GetDeltaVec(md->nextmotion, p1 - md->e, p2 - md->e) * t;
				case 1:
					return WVector::ZERO;
				}
			}
		}
	}

	return actionName ? GetDeltaVec(p2 + md->s) - GetDeltaVec(p1 + md->s) : GetDeltaVec(p2) - GetDeltaVec(p1);
}

// wpuppet.cpp:717
WBoneSet* WPuppet::GetFreeBoneSetList(bool lock) {
	static WBoneSet boneSetList[8] = { false };

	ms_boneSetPos = (ms_boneSetPos + 1) & 7;
	if (ms_boneSetPos == ms_lockedBoneSetPos) {
		ms_boneSetPos = (ms_boneSetPos + 1) & 7;
	}

	if (lock) {
		ms_lockedBoneSetPos = ms_boneSetPos;
	}

	boneSetList[ms_boneSetPos].Clear();

	return &boneSetList[ms_boneSetPos];
}

// wpuppet.cpp:740
void WPuppet::SetDefaultFaceAnimation() {
	w_face_animation* fanim = new w_face_animation;

	strcpy(fanim->name, "\xb1\xe2\xba\xbb");   // "default" (CP949)

	fanim->texHandle = 0;
	fanim->tu = 0;
	fanim->tv = 0;
	fanim->mesh = NULL;
	fanim->group = 0;
	fanim->meshNum = m_rootbone->GetMeshForFaceAnimation(0, NULL, 0);

	fanim->mesh = (w_mesh**)g_mem.Alloc((fanim->meshNum + 1) * sizeof(w_mesh*));
	m_rootbone->GetMeshForFaceAnimation(0, fanim->mesh, 0);
	fanim->mesh[fanim->meshNum] = NULL;

	m_fanimList.AddItem(fanim, fanim->name);
}

// wpuppet.cpp:766
// The names of the other face animations of groupName's group, each followed
// by a space (a new[] string the caller owns), NULL for an unknown name.
char* WPuppet::FindTexAniList(char* groupName) {
	char* aniName = NULL;

	if (!m_fanimList.Find(groupName)) {
		return NULL;
	}
	int group = m_fanimList.Find(groupName)->group;

	int i, j;
	for (i = 0, j = 0; i < 2; i++) {
		if (i == 1) {
			aniName = new char[j + i];
			memset(aniName, 0, 4);
		}
		for (w_face_animation* fanim = m_fanimList.Start(); fanim; fanim = m_fanimList.Next()) {
			if (fanim->group == group && strcmpi(fanim->name, groupName)) {
				if (i == 0) {
					j += strlen(fanim->name) + 1;
				} else {
					strcat(aniName, fanim->name);
					strcat(aniName, " ");
				}
			}
		}
	}

	return aniName;
}

// wpuppet.cpp:804
void WPuppet::AddCollisionObject(const char* boneName, WBlockModel* model) {
	WBone* bone = m_rootbone->FindBone(boneName, 0);
	bone->GetConvexArea(model);
}

// wpuppet.cpp:816
void WPuppet::SetSelfIllum(char* boneName, bool bSelfIllum) {
	if (!boneName || !_strcmpi(boneName, "Recursive")) {
		m_rootbone->SetSelfIllum(bSelfIllum, true);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->SetSelfIllum(bSelfIllum, false);
		}
	}
}

// wpuppet.cpp:831
void WPuppet::SetSelfIllumColor(char* boneName, unsigned long color) {
	if (!boneName || !_strcmpi(boneName, "Recursive")) {
		m_rootbone->SetSelfIllumColor(color, true);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->SetSelfIllumColor(color, false);
		}
	}
}

// wpuppet.cpp:845
void WPuppet::ShowBone(char* boneName) {
	if (!boneName || !strcmp(boneName, "Recursive")) {
		m_rootbone->ShowBone(true);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->ShowBone(false);
		}
	}
}

// wpuppet.cpp:858
void WPuppet::HideBone(char* boneName) {
	if (!boneName || !strcmp(boneName, "Recursive")) {
		m_rootbone->HideBone(true);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->HideBone(false);
		}
	}
}

// wpuppet.cpp:872
unsigned char WPuppet::GetAlpha(char* boneName) {
	if (boneName) {
		return FindBone(boneName)->GetAlpha();
	}
	return m_rootbone->GetAlpha();
}

// wpuppet.cpp:884
void WPuppet::SetAlpha(unsigned char alpha) {
	m_rootbone->SetAlpha(alpha, 1);
}

// wpuppet.cpp:889
void WPuppet::SetLightLine(float line) {
	m_rootbone->SetLightLine(line);
}

// wpuppet.cpp:894
void WPuppet::SetAlpha(char* boneName, unsigned char alpha) {
	if (!_strcmpi(boneName, "Recursive")) {
		m_rootbone->SetAlpha(alpha, 1);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->SetAlpha(alpha, 0);
		}
	}
}

// wpuppet.cpp:908
void WPuppet::ToggleSelfIllum(char* boneName) {
	if (boneName) {
		WBone* bone = FindBone(boneName);
		bone->ToggleSelfIllum();
	}
}

// wpuppet.cpp:917
void WPuppet::SetMirrored(bool bMirrored) {
	if (m_rootbone) {
		m_rootbone->SetMirrored(bMirrored);
	}
}

// wpuppet.cpp:926
void WPuppet::SetBoundBox(char* name, Waabb* aabb, char* boneName) {
	w_bound_box* bb = (w_bound_box*)g_mem.Alloc(sizeof(w_bound_box));
	bb->info = (w_bound_box_info*)g_mem.Alloc(strlen(name) + sizeof(w_bound_box_info));

	bb->bone = m_rootbone->FindBone(boneName, 0);
	bb->info->aabb = *aabb;

	if (bb->info->aabb.max.x - bb->info->aabb.min.x < 0.001f) {
		bb->info->aabb.max.x += 0.001f;
	}
	if (bb->info->aabb.max.y - bb->info->aabb.min.y < 0.001f) {
		bb->info->aabb.max.y += 0.001f;
	}
	if (bb->info->aabb.max.z - bb->info->aabb.min.z < 0.001f) {
		bb->info->aabb.max.z += 0.001f;
	}
	bb->info->option = NULL;
	bb->info->spherePivot = (aabb->min + aabb->max) * 0.5f;
	bb->info->sphereLength = (aabb->max - aabb->min).Magnitude() * 0.5f;
	strcpy(bb->info->name, name);
	m_bbList.AddItem(bb, bb->info->name);
}

// wpuppet.cpp:962
WBone* WPuppet::FindBone(const char* name) {
	WBone* bone = m_boneList.Find(name);
	if (bone) {
		return bone;
	}
	bone = m_rootbone->FindBone(name, 0);
	if (bone) {
		m_boneList.AddItem(bone, bone->m_name);
	}
	return bone;
}

// wpuppet.cpp:979
WBone* WPuppet::FindBone(int id) {
	return m_rootbone->FindBone(id);
}

// wpuppet.cpp:984
bool WPuppet::GetBoundBoxPlane(char* name, WPlane* plane) {
	w_bound_box* bb = m_bbList.Find(name);
	if (bb) {
		MakePlaneEq(plane, bb->info->aabb, &bb->bone->m_matrix);
		return true;
	}
	return false;
}

// wpuppet.cpp:998
bool WPuppet::GetAlignBoundBoxPlane(char* name, WPlane* plane, Waabb* aabb) {
	Waabb box;

	w_bound_box* bb = m_bbList.Find(name);

	if (bb) {
		WVector corner = WVector(bb->info->aabb.max.x, bb->info->aabb.max.y, bb->info->aabb.max.z) * bb->bone->m_matrix;
		box.min = box.max = corner;

		for (int i = 1; i < 8; i++) {
			float z = (i & 4) != 0 ? bb->info->aabb.min.z : bb->info->aabb.max.z;
			float y = (i & 2) != 0 ? bb->info->aabb.min.y : bb->info->aabb.max.y;
			float x = (i & 1) != 0 ? bb->info->aabb.min.x : bb->info->aabb.max.x;
			WVector vec;
			vec = WVector(x, y, z) * bb->bone->m_matrix;
			box += WVector(vec);
		}

		if (plane) {
			MakePlaneEq(plane, box, NULL);
		}
		if (aabb) {
			*aabb = box;
		}
		return true;
	}

	return false;
}

// wpuppet.cpp:1030
bool WPuppet::DotContact(char* bbName, const WVector& pos) {
	w_bound_box* box = m_bbList.Find(bbName);
	WVector vt[8];
	int i;
	for (i = 0; i < 8; i++) {
		vt[i] = WVector(box_v[i][0] ? box->info->aabb.max.x : box->info->aabb.min.x, box_v[i][1] ? box->info->aabb.max.y : box->info->aabb.min.y, box_v[i][2] ? box->info->aabb.max.z : box->info->aabb.min.z) * box->bone->m_matrix;
	}

	for (i = 0; i < 6; i++) {
		if (WPlane(WCrossProduct(vt[box_p[i][0]] - vt[box_p[i][1]], vt[box_p[i][1]] - vt[box_p[i][2]]), vt[box_p[i][3]]) * pos > 0.05f) {
			return false;
		}
	}

	return true;
}

// wpuppet.cpp:1049
void WPuppet::GetBoneName(char** names, int& count) {
	m_rootbone->GetName(names, count);
}

// wpuppet.cpp:1054
WVector WPuppet::GetPivot(const char* name) {
	if (name) {
		WBone* bone = FindBone(name);
		if (bone) {
			return bone->GetPivot();
		}
	}
	return m_rootbone->GetPivot();
}

// wpuppet.cpp:1066
WMatrix WPuppet::GetRootMatrix() {
	return m_rootbone->m_localMat;
}

// wpuppet.cpp:1071
WBone* WPuppet::FindBoneByTail(const char* tail) {
	return m_rootbone ? m_rootbone->FindBoneByTail(tail) : NULL;
}

// wpuppet.cpp:1076
bool WPuppet::CheckFrameData(int frame1, int frame2, int& s, int& e) {
	int i;

	for (i = 0; i < m_framenum && frame1 > m_frame[i].num; i++);

	if (i < m_framenum && m_frame[i].num <= frame2) {
		for (s = i, e = i; e + 1 < m_framenum && m_frame[e + 1].num <= frame2; e++);
		return true;
	}

	return false;
}

// wpuppet.cpp:1091
void WPuppet::MakeTemporaryCenterBox() {
	Waabb aabb;

	if (m_bbList.Find("center") == NULL) {
		aabb = m_bound;
		SetBoundBox("center", &aabb, m_rootbone->m_name);

		m_bHaveCenter = false;
		if (m_BBox.bone) {
			m_BBox = *m_bbList.Find("center");
		}
	} else {
		m_bHaveCenter = true;
	}
}

// wpuppet.cpp:1115
void WPuppet::OptimizeBone() {
	for (WBone* bone = m_rootbone; bone; bone = bone->GetNext()) {
		ReduceBone(bone);
	}
}

// wpuppet.cpp:1133
bool WPuppet::ReduceBone(WBone* bone) {
	int bReduce = CheckStaticBone(bone) ? 1 : 0;

	for (WBone* child = bone->GetChild(); child;) {
		if (ReduceBone(child) == true) {
			child = MergeBone(bone, child);
		} else {
			child = child->GetNext();
			bReduce = false;
		}
	}
	return bReduce ? true : false;
}

// wpuppet.cpp:1167
bool WPuppet::CheckStaticBone(WBone* bone) {
	if (bone->m_keyPos || bone->m_keyRot || !bone->m_parent) {
		return false;
	}

	for (w_motion_data* md = m_mdList->Start(); md; md = m_mdList->Next()) {
		if (strcmp(md->applybone, bone->m_name) == 0) {
			return false;
		}
	}
	for (w_bound_box* bb = m_bbList.Start(); bb; bb = m_bbList.Next()) {
		if (bb->bone == bone) {
			return false;
		}
	}

	return m_rootbone->CheckRigidVtx(bone) ? false : true;
}

// wpuppet.cpp:1189
WBone* WPuppet::MergeBone(WBone* body, WBone* bone) {
	WBone* ret = bone->m_next;
	WBone* parent = bone->m_parent;
	parent->ReleaseChild(bone);

	WSphere sphere;
	sphere = body->GetBoundSphere();
	float radius = sphere.radius;
	sphere.pos = parent->Transform(sphere.pos);
	Waabb bound;
	bound = Waabb(WVector(sphere.pos.x - radius, sphere.pos.y - radius, sphere.pos.z - radius),
		WVector(radius + sphere.pos.x, radius + sphere.pos.y, radius + sphere.pos.z));

	body->MergeMesh(bone, m_rootbone);
	body->SetBoundBox(bound, true);
	bone->ReleaseLink();
	delete bone;
	return ret;
}

// wpuppet.cpp:1216
// m_bound and m_boundSphere: aabb when given, otherwise the box that holds
// the bones posed at the start and end of every motion and at time 0.
void WPuppet::CalcBound(Waabb* aabb, bool fixed) {
	WVector vMin(99999.0f, 99999.0f, 99999.0f), vMax(-99999.0f, -99999.0f, -99999.0f);

	if (!aabb) {
		WMatrix mat;
		mat.Reset();
		if (m_mdList) {
			for (w_motion_data* md = m_mdList->Start(); md; md = m_mdList->Next()) {
				ApplyBones(GetBones(NULL, md->s, fixed, NULL, false), mat);
				m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
				m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);

				ApplyBones(GetBones(NULL, md->e, fixed, NULL, false), mat);
				m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
				m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);
			}
		}

		ApplyBones(GetBones(NULL, 0.0f, fixed, NULL, false), mat);
		m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
		m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);
		m_bound = Waabb(vMin, vMax);
	} else {
		m_bound = *aabb;
	}

	m_boundSphere.pos = (m_bound.min + m_bound.max) * 0.5f;
	m_boundSphere.radius = (m_boundSphere.pos - m_bound.min).Magnitude();
}

// wpuppet.cpp:1253
void WPuppet::CalcBound(bool reset, WPuppet* puppet, bool mode, char* name, Waabb* box) {
	WVector vMin(99999.0f, 99999.0f, 99999.0f), vMax(-99999.0f, -99999.0f, -99999.0f);

	if (reset) {
		m_bound.min.x = m_bound.min.y = m_bound.min.z = 0.0f;
		m_bound.max.x = m_bound.max.y = m_bound.max.z = 0.0f;
	}
	if (box != NULL) {
		box->min = box->max = WVector::ZERO;
	}
	WMatrix mat;
	mat.Reset();
	WPuppet* src;
	WList<w_motion_data*>* list;
	if (puppet != NULL) {
		src = puppet;
		list = puppet->m_mdList;
	} else {
		src = this;
		list = m_mdList;
	}
	if (list != NULL) {
		for (w_motion_data* md = list->Start(); md != NULL; md = list->Next()) {
			if (name != NULL && strcmp(md->name, name) != 0) {
				continue;
			}
			WBoneSet* set = src->GetBones(NULL, md->s, mode, NULL, false);
			m_mat = mat;
			if (set != NULL && m_rootbone != NULL) {
				m_rootbone->ApplyBoneSetList(set);
			}
			m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
			m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);
			if (md->e != md->s) {
				set = src->GetBones(NULL, md->e, mode, NULL, false);
				m_mat = mat;
				if (set != NULL && m_rootbone != NULL) {
					m_rootbone->ApplyBoneSetList(set);
				}
				m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
				m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);
				if (name != NULL) {
					break;
				}
			}
		}
	}
	if (name == NULL) {
		WBoneSet* set = src->GetBones(NULL, 0.0f, mode, NULL, false);
		m_mat = mat;
		if (set != NULL && m_rootbone != NULL) {
			m_rootbone->ApplyBoneSetList(set);
		}
		m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
		m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, false);
	}
	if (box == NULL) {
		vMin.x = Min(m_bound.min.x, vMin.x);
		vMax.x = Max(m_bound.max.x, vMax.x);
		vMin.y = Min(m_bound.min.y, vMin.y);
		vMax.y = Max(m_bound.max.y, vMax.y);
		vMin.z = Min(m_bound.min.z, vMin.z);
		vMax.z = Max(m_bound.max.z, vMax.z);
		m_bound.min = vMin;
		m_bound.max = vMax;
		m_boundSphere.pos = (vMax + vMin) * 0.5f;
		m_boundSphere.radius = (m_boundSphere.pos - vMin).Magnitude();
	} else {
		box->min = vMin;
		box->max = vMax;
	}
}

// wpuppet.cpp:1339
Waabb WPuppet::GetTransformedBoundBox() const {
	Waabb bound;
	WVector vtx[8] = {
		WVector(m_bound.min.x, m_bound.min.y, m_bound.min.z),
		WVector(m_bound.max.x, m_bound.min.y, m_bound.min.z),
		WVector(m_bound.min.x, m_bound.max.y, m_bound.min.z),
		WVector(m_bound.min.x, m_bound.min.y, m_bound.max.z),
		WVector(m_bound.min.x, m_bound.max.y, m_bound.max.z),
		WVector(m_bound.max.x, m_bound.min.y, m_bound.max.z),
		WVector(m_bound.max.x, m_bound.max.y, m_bound.min.z),
		WVector(m_bound.max.x, m_bound.max.y, m_bound.max.z)
	};
	for (int i = 0; i < 8; ++i) {
		vtx[i] = vtx[i] * m_mat;
	}
	bound.min = WVector(3.402823466e38F, 3.402823466e38F, 3.402823466e38F);
	bound.max = WVector(-3.402823466e38F, -3.402823466e38F, -3.402823466e38F);
	for (int j = 0; j < 8; ++j) {
		if (vtx[j].x < bound.min.x) {
			bound.min.x = vtx[j].x;
		}
		if (vtx[j].y < bound.min.y) {
			bound.min.y = vtx[j].y;
		}
		if (vtx[j].z < bound.min.z) {
			bound.min.z = vtx[j].z;
		}
		if (vtx[j].x > bound.max.x) {
			bound.max.x = vtx[j].x;
		}
		if (vtx[j].y > bound.max.y) {
			bound.max.y = vtx[j].y;
		}
		if (vtx[j].z > bound.max.z) {
			bound.max.z = vtx[j].z;
		}
	}
	return bound;
}

// wpuppet.cpp:1378
void WPuppet::UpdateBound(bool bTransform, bool bVisibleMeshOnly) {
	WVector vMin(99999.0f, 99999.0f, 99999.0f), vMax(-99999.0f, -99999.0f, -99999.0f);

	if (bTransform) {
		m_rootbone->Transform(m_mat, NULL, 1.0f, 0);
	}
	m_rootbone->GetTotalOverlapBox(&vMin, &vMax, NULL, bVisibleMeshOnly);
	m_bound = Waabb(vMin, vMax);
	m_boundSphere.pos = (m_bound.max + m_bound.min) * 0.5f;
	m_boundSphere.radius = (m_bound.max - m_bound.min).Magnitude() * 0.5f;
}

// wpuppet.cpp:1393
void WPuppet::UpdateBSphere() {
	WBone* bone = m_BBox.bone;
	if (bone) {
		WMatrix* matrix = &bone->m_matrix;
		w_bound_box_info* info = m_BBox.info;
		WVector vMin;
		WVector vMax;
		vMin = info->aabb.min * *matrix;
		vMax = info->aabb.max * *matrix;
		m_boundSphere.pos = (vMin + vMax) * 0.5f;
		m_boundSphere.radius = (vMax - m_boundSphere.pos).Magnitude();
	}
}

// wpuppet.cpp:1408
void WPuppet::SetVtxColor(unsigned long color) {
	m_rootbone->SetVtxColor(color);
}

// wpuppet.cpp:1415
void WPuppet::UpdatePhysics(float t, const WVector& v) {
	for (WBone* bone = m_phybone.Start(); bone; bone = m_phybone.Next()) {
		bone->UpdatePhysicsModel(t, v);
	}
}

// wpuppet.cpp:1422
void WPuppet::SetPhysicsModel(const char* boneName, int model) {
	WBone* bone = FindBone(boneName);
	bone->SetPhysicsModel(model);
	m_phybone += bone;
}

// wpuppet.cpp:1434
void WPuppet::RotateBone(char* boneName, float angle, int axis) {
	WBone* bone = FindBone(boneName[0] == '@' ? boneName + 1 : boneName);
	if (bone) {
		bone->Rotate(angle, axis, boneName[0] != '@');
	}
}

// wpuppet.cpp:1442
void WPuppet::MoveBone(char* boneName, WVector& v) {
	if (!_strcmpi(boneName, "Recursive")) {
		m_rootbone->Move(v, true);
	} else {
		WBone* bone = FindBone(boneName);
		if (bone) {
			bone->Move(v, false);
		}
	}
}

// wpuppet.cpp:1457
void WPuppet::RenderBoundBox(const char* name, WView* view, unsigned long color, float size) {
	static unsigned long box_color[6] = { 0xffffff00, 0xff00ff00, 0xffffff00, 0xffff0000, 0xff0000ff, 0xffffff00 };
	static WVector _vMinInvalid(99999.0f, 99999.0f, 99999.0f);
	static WVector _vMaxInvalid(-99999.0f, -99999.0f, -99999.0f);
	static WVector _vMinZero(-0.01f, -0.01f, -0.01f);
	static WVector _vMaxZero(0.01f, 0.01f, 0.01f);

	w_bound_box* box = m_bbList.Find(name);
	if (box == NULL) {
		return;
	}
	WVector v[8];
	WVector a, b, normal, center;
	float dot_product;
	int i;
	if (WisEqual(box->info->aabb.min, _vMinInvalid, 0.00001f) != 0 && WisEqual(box->info->aabb.max, _vMaxInvalid, 0.00001f) != 0) {
		for (i = 0; i < 8; ++i) {
			v[i] = box->bone->Transform(WVector(
				box_v[i][0] ? (float)(_vMaxZero.x + size) : (float)(_vMinZero.x - size),
				box_v[i][1] ? (float)(_vMaxZero.y + size) : (float)(_vMinZero.y - size),
				box_v[i][2] ? (float)(_vMaxZero.z + size) : (float)(_vMinZero.z - size)));
		}
	} else {
		for (i = 0; i < 8; ++i) {
			WVector p(box_v[i][0] ? (float)(size + box->info->aabb.max.x) : (float)(box->info->aabb.min.x - size),
				box_v[i][1] ? (float)(size + box->info->aabb.max.y) : (float)(box->info->aabb.min.y - size),
				box_v[i][2] ? (float)(size + box->info->aabb.max.z) : (float)(box->info->aabb.min.z - size));
			v[i] = p * box->bone->m_matrix;
		}
	}
	for (int f = 0; f < 6; ++f) {
		center = WVector::ZERO;
		if (WisEqual(box->info->aabb.min, _vMinInvalid, 0.00001f) != 0 && WisEqual(box->info->aabb.max, _vMaxInvalid, 0.00001f) != 0) {
			for (i = 0; i < 4; ++i) {
				center += v[box_p[f][i]];
			}
		} else {
			for (i = 0; i < 4; ++i) {
				view->DrawLine(v[box_p[f][(i + 1) % 4]], color, v[box_p[f][i]], color, 0);
				center += v[box_p[f][i]];
			}
		}
		a = v[box_p[f][0]] - v[box_p[f][1]];
		b = v[box_p[f][1]] - v[box_p[f][2]];
		a.Normalize();
		b.Normalize();
		dot_product = a * b;
		if (dot_product > -0.999f && dot_product < 0.999f) {
			normal = WCrossProduct(a, b);
			normal.Normalize();
			center *= 0.25f;
			view->DrawLine(center, box_color[f], center + normal * (15.0f * g_CM_TO_WU), box_color[f], 0);
			view->DrawLine(center + normal * (15.0f * g_CM_TO_WU), box_color[f], center + normal * (10.0f * g_CM_TO_WU) + a * (3.0f * g_CM_TO_WU), box_color[f], 0);
			view->DrawLine(center + normal * (15.0f * g_CM_TO_WU), box_color[f], center + normal * (10.0f * g_CM_TO_WU) - a * (3.0f * g_CM_TO_WU), box_color[f], 0);
		}
	}
}

// wpuppet.cpp:1538
void WPuppet::ChangeTexture(char* animName, int texHandle) {
	w_face_animation* fanim = m_fanimList.Find(animName);
	if (fanim == NULL) {
		return;
	}

	w_mesh** meshes = fanim->mesh;
	for (int i = 0; meshes[i]; i++) {
		float su = fanim->scalex / meshes[i]->scaleu;
		float sv = fanim->scaley / meshes[i]->scalev;
		float du = fanim->tu - su * meshes[i]->tu;
		float dv = fanim->tv - sv * meshes[i]->tv;
		meshes[i]->texHandle = texHandle;
		int tex = meshes[i]->texHandle;
		meshes[i]->drawFlag ^= (tex ^ meshes[i]->drawFlag) & 0x7ff;
		meshes[i]->tu = fanim->tu;
		meshes[i]->tv = fanim->tv;
		meshes[i]->scaleu = fanim->scalex;
		meshes[i]->scalev = fanim->scaley;
		if (WisEqual(du, 0.0f, 0.00001f) && WisEqual(dv, 0.0f, 0.00001f) && WisEqual(su, 1.0f, 0.00001f) && WisEqual(sv, 1.0f, 0.00001f)) {
			continue;
		}
		for (int j = 0; j < meshes[i]->vtxNum; j++) {
			meshes[i]->uvData[j][0] = su * meshes[i]->uvData[j][0] + du;
			meshes[i]->uvData[j][1] = sv * meshes[i]->uvData[j][1] + dv;
		}
	}
}

// wpuppet.cpp:1577
bool WPuppet::ChangeTexturePart(char* animName, char* texName, Bitmap** ppBmp, tagRECT* pRc) {
	w_face_animation* anim = m_fanimList.Find(animName);
	if (anim == NULL) {
		return false;
	}
	return GetResrcManager()->ChangeTexturePart(anim->filename, texName, ppBmp, pRc);
}

// wpuppet.cpp:1593
void WPuppet::ChangeTexturePart(char* animName, Bitmap* pBmp, const tagRECT& rc) {
	w_face_animation* anim = m_fanimList.Find(animName);
	if (anim) {
		GetResrcManager()->ChangeTexturePart(anim->filename, pBmp, rc);
	}
}

// wpuppet.cpp:1609
bool WPuppet::ChangeTexturePart(int hDstTex, char* texName, Bitmap** ppBmp, tagRECT* pRc) {
	return GetResrcManager()->ChangeTexturePart(hDstTex, texName, ppBmp, pRc);
}

// wpuppet.cpp:1614
void WPuppet::ChangeTexturePart(int hDstTex, Bitmap* pBmp, const tagRECT& rc) {
	GetResrcManager()->ChangeTexturePart(hDstTex, pBmp, rc);
}

// wpuppet.cpp:1619
void WPuppet::ApplyFaceAnimation(char* animName, float time) {
	float su, sv, du, dv;
	if (animName) {
		w_face_animation* fanim = m_fanimList.Find(animName);
		if (!fanim) {
			return;
		}

		w_mesh** mesh = fanim->mesh;
		for (int i = 0; mesh[i]; i++) {
			su = fanim->scalex / mesh[i]->scaleu;
			sv = fanim->scaley / mesh[i]->scalev;
			du = fanim->tu - su * mesh[i]->tu;
			dv = fanim->tv - sv * mesh[i]->tv;
			mesh[i]->texHandle = fanim->texHandle;
			int tex = mesh[i]->texHandle;
			mesh[i]->drawFlag ^= (tex ^ mesh[i]->drawFlag) & 0x7ff;
			mesh[i]->tu = fanim->tu;
			mesh[i]->tv = fanim->tv;
			mesh[i]->scaleu = fanim->scalex;
			mesh[i]->scalev = fanim->scaley;
			if (WisEqual(du, 0.0f, 0.00001f) && WisEqual(dv, 0.0f, 0.00001f) && WisEqual(su, 1.0f, 0.00001f) && WisEqual(sv, 1.0f, 0.00001f)) {
				continue;
			}

			for (int v = 0; v < mesh[i]->vtxNum; v++) {
				mesh[i]->uvData[v][0] = su * mesh[i]->uvData[v][0] + du;
				mesh[i]->uvData[v][1] = sv * mesh[i]->uvData[v][1] + dv;
			}
		}
	} else {
		w_face_animation* fanim = m_fanimList.Find("±âº»");   // "default" (CP949)
		if (!fanim) {
			return;
		}

		w_mesh** mesh = fanim->mesh;
		for (int i = 0; mesh[i]; i++) {
			su = mesh[i]->originalTu / mesh[i]->scaleu;
			sv = mesh[i]->originalTv / mesh[i]->scalev;
			du = mesh[i]->originalTu - su * mesh[i]->tu;
			dv = mesh[i]->originalTv - sv * mesh[i]->tv;
			mesh[i]->texHandle = mesh[i]->originalTexHandle;
			int tex = mesh[i]->texHandle;
			mesh[i]->drawFlag ^= (tex ^ mesh[i]->drawFlag) & 0x7ff;
			mesh[i]->tu = mesh[i]->originalTu;
			mesh[i]->tv = mesh[i]->originalTv;
			mesh[i]->scaleu = mesh[i]->originalScaleu;
			mesh[i]->scalev = mesh[i]->originalScalev;
			if (WisEqual(du, 0.0f, 0.00001f) && WisEqual(dv, 0.0f, 0.00001f) && WisEqual(su, 1.0f, 0.00001f) && WisEqual(sv, 1.0f, 0.00001f)) {
				continue;
			}

			for (int v = 0; v < mesh[i]->vtxNum; v++) {
				mesh[i]->uvData[v][0] = su * mesh[i]->uvData[v][0] + du;
				mesh[i]->uvData[v][1] = sv * mesh[i]->uvData[v][1] + dv;
			}
		}
	}
}

// wpuppet.cpp:1693
int WPuppet::SeekChunkFromPET(cFile* fp, char* str) {
	w_pet_chunk chunk;
	for (int pos = 0; pos < fp->Length(); pos += chunk.length + 8) {
		fp->Seek(pos, 0);
		fp->Read(&chunk, 8);
		if (memcmp(str, chunk.id, 4) == 0) {
			fp->Seek(pos + 8, 0);
			return 1;
		}
	}
	return 0;
}

// wpuppet.cpp:1715
int ReadID(cFile* fp) {
	int id = fp->GetByte();
	if (id == 0xff) {
		return -1;
	}
	if (id == 0xfe) {
		fp->Read(&id, 2);
	}
	return id;
}

// wpuppet.cpp:1730
bool WPuppet::ChangeMesh(char* filename) {
	char directory[256];

	cFile* fp = GetResrcManager()->GetCFile(filename, 0xffff);

	if (fp) {
		GetDirectory(filename, directory);

		w_pet_texture_info* texInfo = LoadPET_Texture(fp, false, *directory ? directory : NULL, NULL);

		WBone* root = LoadPET_Bone(fp, 0xff);

		root->Transform(m_mat, NULL, 1.0f, 0);

		LoadPET_Mesh(fp, root, texInfo, false);
		LoadPET_FaceAnim(fp, texInfo, root, directory);

		if (texInfo) {
			delete[] texInfo;
		}
		m_rootbone->CombineVtxColor(true);

		CloseCFile(fp);

		delete root;

		SetRenderMode(m_rendMode);
		SetDefaultFaceAnimation();

		return true;
	}

	return false;
}

// wpuppet.cpp:1775
bool WPuppet::AddMotion(const char* petname, char* actname) {
	char motion[64];

	cFile* fp = GetResrcManager()->GetCFile(petname, 0xffff);
	if (fp == NULL) {
		return false;
	}

	if (actname != NULL) {
		strcpy(motion, actname);
	} else {
		strcpy(motion, strchr(petname, '/') != NULL ? strchr(petname, '/') + 1 : petname);
	}

	WMatrix mat;
	mat.Reset();

	bool mtn = strlen(petname) > 3 && strcmpi(petname + strlen(petname) - 3, "mtn") == 0;

	w_motion_addition* add = (w_motion_addition*)new char[strlen(motion) + sizeof(w_motion_addition)];
	strcpy(add->name, motion);
	add->rootbone = LoadPET_Bone(fp, mtn);
	add->rootbone->Transform(mat, NULL, 1.0f, 0);
	add->length = LoadPET_Animation(fp, add->rootbone);
	add->s = m_aniLen + 1.0f;
	add->e = add->s + add->length;
	add->mdList = mtn == false ? LoadPET_Motion(fp, add->s) : NULL;
	add->h_gap = add->rootbone->GetKeyPos(0.0f).y - m_rootbone->GetKeyPos(0.0f).y;
	CloseCFile(fp);

	if (add->mdList == NULL) {
		add->mdList = new WList<w_motion_data*>(2, 2);
		w_motion_data* defmd = (w_motion_data*)new char[strlen(motion) + strlen(motion) + strlen(add->rootbone->m_name) + sizeof(w_motion_data) + 3];
		defmd->s = add->s;
		defmd->end = defmd->e = add->length + add->s;
		defmd->option1 = 1;
		defmd->option2 = 0.0f;
		defmd->name = defmd->ptr;
		defmd->nextmotion = defmd->name + strlen(motion) + 1;
		defmd->applybone = defmd->nextmotion + strlen(motion) + 1;
		strcpy(defmd->name, motion);
		defmd->nextmotion[0] = 0;
		strcpy(defmd->applybone, add->rootbone->m_name);
		add->mdList->AddItem(defmd, defmd->name, false);
	}

	m_addedMotionList += add;
	m_aniLen = add->e;
	MergeRootMotion(add->mdList, add->rootbone);
	return false;
}

// wpuppet.cpp:1843
void WPuppet::DelAddMotion(const char* name) {
	w_motion_addition* ma;

	for (ma = m_addedMotionList.Start(); ma; ma = m_addedMotionList.Next()) {
		if (strcmp(ma->name, name) == 0) {
			break;
		}
	}
	if (ma) {
		m_aniLen = ma->s;

		for (w_motion_addition* other = m_addedMotionList.Start(); other; other = m_addedMotionList.Next()) {
			if (other->s >= ma->e) {
				other->s -= ma->length;
				other->e -= ma->length;

				if (m_aniLen < other->e) {
					m_aniLen = other->e;
				}
			}
		}

		m_addedMotionList -= ma;

		for (w_motion_data* md = ma->mdList->Start(); md; md = ma->mdList->Next()) {
			delete[] md;
		}
		delete ma->mdList;
		delete ma->rootbone;
		delete[] ma;
	}
}

// wpuppet.cpp:1876
void WPuppet::ClearAddMotion() {
	for (w_motion_addition* ma = m_addedMotionList.Start(); ma; ma = m_addedMotionList.Next()) {
		if (m_aniLen > ma->s) {
			m_aniLen = ma->s;
		}
		for (w_motion_data* md = ma->mdList->Start(); md; md = ma->mdList->Next()) {
			delete[] md;
		}
		delete ma->mdList;
		delete ma->rootbone;
		delete[] ma;
	}

	m_addedMotionList.Reset();
}

// wpuppet.cpp:1899
void WPuppet::SetTexPiece(char* texname, int texHandle, float pu, float pv, float scalex, float scaley) {
	char* name = texname;

	if (strchr(name, '/')) {
		name = strchr(name, '/') + 1;
	}
	if (!m_tex_piece) {
		m_tex_piece = new WList<w_tex_piece*>(32, 8);
	}

	w_tex_piece* piece = (w_tex_piece*)g_mem.Alloc(strlen(name) + sizeof(w_tex_piece));

	piece->pu = pu;
	piece->pv = pv;
	piece->count = 1;
	piece->scalex = scalex;
	piece->scaley = scaley;
	piece->texHandle = texHandle;
	strcpy(piece->name, name);

	m_tex_piece->AddItem(piece, piece->name);
	m_use_tex_piece += piece;
}

// wpuppet.cpp:1929
WPuppet::w_tex_piece* WPuppet::GetTexPiece(char* texname) {
	char* name = strchr(texname, '/') ? strchr(texname, '/') + 1 : texname;
	w_tex_piece* piece = m_tex_piece ? m_tex_piece->Find(name) : NULL;
	if (piece) {
		piece->count++;
		m_use_tex_piece.AddItem(piece);
		return piece;
	}
	return NULL;
}

// wpuppet.cpp:1948
// Drops this puppet's references to the shared texture pieces; a piece no
// puppet uses any more is freed, and its texture released unless another
// piece still shows it.
void WPuppet::ClearTexPiece() {
	for (w_tex_piece* tex = m_use_tex_piece.Start(); tex; tex = m_use_tex_piece.Next()) {
		tex->count--;

		if (tex->count == 0) {
			w_tex_piece* gtex;
			if (m_tex_piece) {
				*m_tex_piece -= tex;

				for (gtex = m_tex_piece->Start(); gtex; gtex = m_tex_piece->Next()) {
					if (gtex->texHandle == tex->texHandle) {
						break;
					}
				}
			}
			if (!gtex) {
				GetResrcManager()->Release(tex->texHandle);
			}

			g_mem.Free(tex);
		}
	}

	m_use_tex_piece.Reset();
}

// wpuppet.cpp:1988
void WPuppet::LoadPET(cFile* fp, bool alphatest, char* directory, int iPetType) {
	w_pet_texture_info* texinfo = NULL;
	int texNum = 0;

	m_iPetType = iPetType;
	if ((iPetType & 1) != 0) {
		texinfo = LoadPET_Texture(fp, alphatest, directory, &texNum);
	}

	WMatrix mat;
	mat.Reset();
	m_rootbone = LoadPET_Bone(fp, iPetType);
	m_rootbone->Transform(mat, NULL, 1.0f, 0);

	if ((iPetType & 4) != 0) {
		m_aniLen = LoadPET_Animation(fp, m_rootbone);
	}
	if ((iPetType & 8) != 0) {
		LoadPET_Mesh(fp, m_rootbone, texinfo, iPetType == 0x11b);
	}
	if ((iPetType & 0x10) != 0) {
		LoadPET_FaceAnim(fp, texinfo, m_rootbone, directory);
	}
	if ((iPetType & 0x20) != 0) {
		LoadPET_Frame(fp);
	}
	if ((iPetType & 0x40) != 0) {
		m_mdList = LoadPET_Motion(fp, 0.0f);
	}
	if ((iPetType & 0x80) != 0) {
		LoadPET_Collision(fp);
	}

	for (int i = 0; i < texNum; ++i) {
		if (texinfo[i].mapType == 3) {
			m_rendMode = 5;
			m_rootbone->ApplySpecularMap(texinfo[i].texHandle);
			break;
		}
	}
	if (texinfo != NULL) {
		delete[] texinfo;
	}
}

// wpuppet.cpp:2035
float WPuppet::LoadPET_Animation(cFile* fp, WBone* root) {
	float len = 0;

	if (!SeekChunkFromPET(fp, "ANIM")) {
		return 0;
	}
	int id;
	while ((id = ReadID(fp)) >= 0) {
		len = root->FindBone(id)->LoadAnimationKey(fp);
	}
	return len;
}

// wpuppet.cpp:2054
WList<w_motion_data*>* WPuppet::LoadPET_Motion(cFile* fp, float scale) {
	char text[4][512];
	int length;
	float option2;
	WList<w_motion_data*>* list;
	int i;
	int count;
	int startFrame;
	float e;
	float s;
	int endFrame;

	if (SeekChunkFromPET(fp, "MOTI") == 0) {
		return NULL;
	}
	list = new WList<w_motion_data*>(8, 8);
	fp->Read(&count, 4);
	for (i = 0; i < count; ++i) {
		fp->Read(&length, 4);
		fp->Read(text[0], length);
		text[0][length] = 0;
		fp->Read(&startFrame, 4);
		fp->Read(&endFrame, 4);
		s = (float)startFrame / 30.0f;
		e = (float)endFrame / 30.0f;
		fp->Read(&length, 4);
		fp->Read(text[1], length);
		text[1][length] = 0;
		fp->Read(&length, 4);
		fp->Read(text[2], length);
		text[2][length] = 0;
		int option1 = strcmpi(text[2], "blending") != 0;
		fp->Read(&option2, 4);
		fp->Read(&length, 4);
		fp->Read(text[3], length);
		text[3][length] = 0;
		w_motion_data* data = (w_motion_data*)new char[(int)strlen(text[0]) + (int)strlen(text[1]) + (int)strlen(text[3]) + sizeof(w_motion_data) + 3];
		data->s = s;
		data->end = e;
		data->e = e;
		if (option1 == 2) {
			data->e = data->e - option2;
		}
		if (option2 == 1.0f) {
			data->end = option2 + data->end;
		}
		data->option1 = option1;
		data->option2 = option2;
		data->name = data->ptr;
		data->nextmotion = data->name + (int)strlen(text[0]) + 1;
		data->applybone = data->nextmotion + (int)strlen(text[1]) + 1;
		strcpy(data->name, text[0]);
		strcpy(data->nextmotion, text[1]);
		strcpy(data->applybone, text[3]);
		list->AddItem(data, data->name, false);
	}
	return list;
}

// wpuppet.cpp:2133
void WPuppet::LoadPET_Collision(cFile* fp) {
	char name[4][512];
	Waabb aabb;
	aabb.min.z = 0.0f;
	aabb.min.y = 0.0f;
	aabb.min.x = 0.0f;
	aabb.max.z = 0.0f;
	aabb.max.y = 0.0f;
	aabb.max.x = 0.0f;
	if (SeekChunkFromPET(fp, "COLL") == 0) {
		return;
	}
	int count;
	fp->Read(&count, 4);
	for (int i = 0; i < count; ++i) {
		int length;
		float value[6];
		int head[2];
		fp->Read(&head[0], 4);
		fp->Read(&head[1], 4);
		for (int n = 0; n <= 3; ++n) {
			memset(name[n], 0, 512);
			fp->Read(&length, 4);
			fp->Read(name[n], length);
			name[n][length] = 0;
		}
		for (int k = 0; k < 6; ++k) {
			fp->Read(&value[k], 4);
		}
		aabb.min.y = value[1];
		aabb.min.z = value[2];
		aabb.min.x = value[0];
		aabb.max.y = value[4];
		aabb.max.z = value[5];
		aabb.max.x = value[3];
		SetBoundBox(name[0], &aabb, name[1]);
		if (strlen(name[3]) > 1 || strlen(name[2]) > 1) {
			w_bound_box* box = m_bbList.Find(name[0]);
			box->info->option = (char*)g_mem.Alloc((int)strlen(name[3]) + (int)strlen(name[2]) + 2);
			sprintf(box->info->option, "%s %s", name[2], name[3]);
		}
		if (m_rootbone->FindBone(name[0], 0)) {
			m_rootbone->FindBone(name[1], 0)->SetBoundBox(aabb, false);
		}
	}
	w_bound_box* box = m_bbList.Find("bound");
	if (box) {
		m_BBox.bone = box->bone;
		m_BBox.info = box->info;
		return;
	}
	box = m_bbList.Find("center");
	if (box) {
		m_BBox.bone = box->bone;
		m_BBox.info = box->info;
	}
}

// wpuppet.cpp:2202
void WPuppet::LoadPET_Frame(cFile* fp) {
	char name[1024];
	char text[4][1024];
	int count;
	int check;
	if (SeekChunkFromPET(fp, "FRAM") == 0) {
		return;
	}
	fp->Read(&count, 4);
	if (count <= 0) {
		return;
	}
	if (count > 10000) {
		return;
	}
	m_framenum = count;
	m_frame = (w_share_pet_data::w_frame*)g_mem.Alloc(m_framenum * sizeof(w_share_pet_data::w_frame));
	for (int i = 0; i < count; ++i) {
		int length;
		int num;
		int head[2];
		fp->Read(&head[0], 4);
		num = head[0];
		char* line = text[0];
		for (int k = 0; k < 3; ++k) {
			memset(line, 0, 1024);
			fp->Read(&length, 4);
			fp->Read(line, length);
			line += 1024;
		}
		memset(name, 0, 1024);
		memcpy(name, text[0], strlen(text[0]) < 1024 ? strlen(text[0]) : 1024);
		if (count <= 1 && strcmp(name, "") == 0) {
			return;
		}
		fp->Read(&check, 4);
		if (check == 0) {
			check = -1;
		}
		m_frame[i].num = num;
		m_frame[i].check = check;
		m_frame[i].data = (char*)g_mem.Alloc((int)strlen(name) + 1);
		strcpy(m_frame[i].data, name);
		char* found = strstr(m_frame[i].data, "msg");
		while (found != NULL) {
			int c = 0;
			if (*found != 0) {
				do {
					if (found[c] == ')') {
						found[c] = ' ';
						break;
					}
					++c;
				} while (found[c] != 0);
			}
			found = strstr(found + 3, "msg");
		}
	}
}

// wpuppet.cpp:2289
WBone* WPuppet::LoadPET_Bone(cFile* fp, int version) {
	char name[256];
	WBone* root;
	WBone** bones;
	int count;
	WMatrix mat;
	WMatrix* mats;

	root = NULL;
	if (SeekChunkFromPET(fp, "BONE") == 0) {
		return NULL;
	}
	mat.Reset();
	count = fp->GetByte();
	if (count == 0) {
		fp->Read(&count, 2);
	}
	bones = new WBone*[count];
	mats = new WMatrix[count];
	for (int i = 0; i < count; ++i) {
		name[0] = (char)fp->GetByte();
		if (name[0]) {
			for (char* p = name; *p; *++p = (char)fp->GetByte()) {
			}
		}
		bones[i] = new WBone;
		bones[i]->SetName(name);
		bones[i]->SetIDNumber(i);
		int parent = ReadID(fp);
		if (root != NULL) {
			if (parent != -1) {
				bones[i]->SetParent(root->FindBone(parent));
			} else {
				root->SetNext(bones[i], true);
			}
		} else {
			root = bones[i];
		}
		if (version == 0xff || version == 0x82 || version == 0x11b || version == 6) {
			fp->Read(&mat.xx, 4);
			fp->Read(&mat.yx, 4);
			fp->Read(&mat.zx, 4);
			fp->Read(&mat.xy, 4);
			fp->Read(&mat.yy, 4);
			fp->Read(&mat.zy, 4);
			fp->Read(&mat.xz, 4);
			fp->Read(&mat.yz, 4);
			fp->Read(&mat.zz, 4);
			fp->Read(&mat.xm, 4);
			fp->Read(&mat.ym, 4);
			fp->Read(&mat.zm, 4);
			WVectorLen(mat.xa);
			if (version == 6) {
				mat.xm = mat.xm * g_CM_TO_WU;
				mat.ym = mat.ym * g_CM_TO_WU;
				mat.zm = mat.zm * g_CM_TO_WU;
				mats[i] = mat;
				if (parent >= 0) {
					mat = mats[i] * ~mats[parent];
				}
			}
		}
		bones[i]->SetBasicMatrix(mat);
	}
	delete[] mats;
	delete[] bones;
	for (WBone* previous = root; previous->m_next != NULL; previous = previous->m_next) {
		if (strstr(previous->m_next->GetBoneName(), "Bip") != NULL ||
			strstr(previous->m_next->GetBoneName(), "Bone") != NULL ||
			strstr(previous->m_next->GetBoneName(), "Dummy") != NULL) {
			WBone* next = previous->m_next;
			previous->SetNext(NULL, true);
			next->SetNext(root, true);
			root = next;
			break;
		}
	}
	return root;
}

// wpuppet.cpp:2396
w_pet_texture_info* WPuppet::LoadPET_Texture(cFile* fp, bool alphaTest, char* directory, int* textureCount) {
	int i, j;
	int count = 0;
	if (!SeekChunkFromPET(fp, "TEXT")) {
		return NULL;
	}
	int texNum;
	fp->Read(&texNum, sizeof(texNum));
	if (textureCount) {
		*textureCount = texNum;
	}
	w_pet_texture* texture = new w_pet_texture[texNum];
	fp->Read(texture, texNum * sizeof(w_pet_texture));
	w_pet_texture_info* info = new w_pet_texture_info[texNum];
	Bitmap** bitmap = new Bitmap*[texNum];
	if (!m_storeTexList) {
		m_storeTexList = new WList<int>;
	}
	char filename[260];
	for (i = 0; i < texNum; ++i) {
		strcpy(info[i].filename, texture[i].filename);
		info[i].group = texture[i].group;
		info[i].bBind = false;
		info[i].mapType = 0;
		info[i].alpha = (unsigned char)(texture[i].diffuse >> 24);
		info[i].diffuse = texture[i].diffuse;
		info[i].xulDrawFlag2 = 0;
		info[i].drawFlags = 0;
		info[i].texHandle = 0;
		info[i].pu = info[i].pv = 0.0f;
		info[i].scalex = info[i].scaley = 1.0f;
		bitmap[i] = NULL;
		if (!strcmp(texture[i].filename, "null")) {
			continue;
		}
		if (strstr(texture[i].filename, "2#")) {
			info[i].xulDrawFlag2 = 0x400;
		}
		for (j = 0; j < 3; ++j) {
			switch (texture[i].filename[j]) {
			case '+':
				if (alphaTest) {
					info[i].drawFlags |= 0x80000000;
				} else {
					info[i].drawFlags |= 0x400000;
				}
				info[i].xulDrawFlag2 |= 0x10;
				break;
			case '[':
				if (alphaTest) {
					info[i].drawFlags |= 0x80000000;
				} else {
					info[i].drawFlags |= 0x400000;
				}
				break;
			case ']':
				info[i].drawFlags |= 0x80000000;
				info[i].xulDrawFlag2 |= 4;
				break;
			case '{':
				info[i].drawFlags |= 0x80000000;
				break;
			case '~':
				info[i].drawFlags |= 0x20800000;
				if (alphaTest) {
					info[i].drawFlags |= 0x80000000;
				} else {
					info[i].drawFlags |= 0x400000;
				}
				if (!texture[i].flag) {
					info[i].bBind = true;
				}
				break;
			case '}':
				info[i].drawFlags |= 0x21800000;
				if (alphaTest) {
					info[i].drawFlags |= 0x80000000;
				} else {
					info[i].drawFlags |= 0x400000;
				}
				break;
			case '@':
				info[i].mapType = 1;
				break;
			case '!':
				info[i].mapType = 3;
				break;
			default:
				if (j == 0 && !texture[i].flag) {
					info[i].bBind = true;
				}
				break;
			}
		}
		strcpy(filename, texture[i].filename);
		if (strstr(filename, ".dds") || info[i].mapType == 3) {
			info[i].bBind = false;
		}
		if (texNum > 1 && info[i].bBind == true && !GetResrcManager()->CheckTexture(filename)) {
			w_tex_piece* piece = GetTexPiece(texture[i].filename);
			if (piece) {
				info[i].pu = piece->pu;
				info[i].pv = piece->pv;
				info[i].scalex = piece->scalex;
				info[i].scaley = piece->scaley;
				info[i].texHandle = piece->texHandle;
				info[i].bBind = false;
				bitmap[i] = NULL;
			} else {
				bitmap[i] = GetResrcManager()->LoadBitmap(filename, 1, false);
				if (!bitmap[i]) {
					bitmap[i] = new Bitmap(32, 32, 8);
				}
				if (bitmap[i]->bi->bmiHeader.biWidth > 256 || bitmap[i]->bi->bmiHeader.biHeight > 256 ||
					(bitmap[i]->bi->bmiHeader.biWidth == 256 && bitmap[i]->bi->bmiHeader.biHeight == 256)) {
					if (strcmp(texture[i].filename, "null")) {
						bitmap[i]->Update();
						info[i].texHandle = GetResrcManager()->UploadTexture(filename, bitmap[i], 0, 0);
						delete bitmap[i];
						*m_storeTexList += info[i].texHandle;
						bitmap[i] = NULL;
					}
				} else {
					++count;
				}
			}
		} else {
			bitmap[i] = NULL;
			info[i].texHandle = GetResrcManager()->LoadTexture(filename, 0, 0, 0);
			*m_storeTexList += info[i].texHandle;
		}
	}
	WRectManager page[8];
	Bitmap surface;
	int* bindIndex = new int[texNum];
	static int fixed_wh[8][2] = {
		{ 256, 256 }, { 256, 128 }, { 128, 128 }, { 128, 64 }, { 64, 128 }, { 64, 64 }, { 32, 64 }, { 32, 32 }
	};
	while (count > 0) {
		for (i = 0; i < 8; ++i) {
			page[i].Clear(fixed_wh[i][0], fixed_wh[i][1]);
		}
		i = 0;
		int max = 8;
		int level;
		int x, y;
		for (; i < texNum; ++i) {
			if (bitmap[i] && !page[0].Alloc(bitmap[i]->bi->bmiHeader.biWidth, bitmap[i]->bi->bmiHeader.biHeight, x, y)) {
				bindIndex[i] = 1;
				level = 0;
				for (j = 1; j < max; ++j) {
					if (page[j].Alloc(bitmap[i]->bi->bmiHeader.biWidth, bitmap[i]->bi->bmiHeader.biHeight, x, y)) {
						max = level + 1;
						break;
					}
					++level;
				}
			} else {
				bindIndex[i] = 0;
			}
		}
		page[level].Clear(fixed_wh[level][0], fixed_wh[level][1]);
		surface.Create(fixed_wh[level][0], fixed_wh[level][1], 24);
		int handle = GetResrcManager()->UploadTexture(0, &surface, 0x10000000, 0);
		for (i = 0; i < texNum; ++i) {
			if (bindIndex[i]) {
				bitmap[i]->Update();
				page[level].Alloc(bitmap[i]->bi->bmiHeader.biWidth, bitmap[i]->bi->bmiHeader.biHeight, x, y);
				info[i].pu = (float)x / (float)surface.bi->bmiHeader.biWidth;
				info[i].pv = (float)y / (float)surface.bi->bmiHeader.biHeight;
				info[i].scalex = (float)bitmap[i]->bi->bmiHeader.biWidth / (float)surface.bi->bmiHeader.biWidth;
				info[i].scaley = (float)bitmap[i]->bi->bmiHeader.biHeight / (float)surface.bi->bmiHeader.biHeight;
				info[i].texHandle = handle;
				bitmap[i]->Update();
				for (j = 0; j < (int)(bitmap[i]->bi->bmiHeader.biWidth * bitmap[i]->bi->bmiHeader.biHeight); ++j) {
					unsigned char r, g, b;
					int px = j % bitmap[i]->bi->bmiHeader.biWidth;
					int py = j / bitmap[i]->bi->bmiHeader.biWidth;
					bitmap[i]->GetPixel(px, py, r, g, b);
					surface.vram[(y + py) * surface.pitch + (x + px) * 3] = b;
					surface.vram[(y + py) * surface.pitch + (x + px) * 3 + 1] = g;
					surface.vram[(y + py) * surface.pitch + (x + px) * 3 + 2] = r;
				}
				--count;
				delete bitmap[i];
				bitmap[i] = NULL;
				SetTexPiece(texture[i].filename, info[i].texHandle, info[i].pu, info[i].pv, info[i].scalex, info[i].scaley);
			}
		}
		GetResrcManager()->FixTexture(handle, &surface, 0, 0);
	}
	delete[] bindIndex;
	delete[] bitmap;
	delete[] texture;
	return info;
}

// wpuppet.cpp:2753
void WPuppet::LoadPET_FaceAnim(cFile* fp, w_pet_texture_info* texture, WBone* bone, char* name) {
	w_pet_face_animation record;
	int i;
	w_face_animation* anim;
	int count;

	if (SeekChunkFromPET(fp, "FANM") == 0) {
		return;
	}
	for (anim = m_fanimList.Start(); anim != NULL; anim = m_fanimList.Next()) {
		g_mem.Free(anim->mesh);
		delete anim;
	}
	m_fanimList.Reset();
	fp->Read(&count, 4);
	for (i = 0; i < count; ++i) {
		fp->Read(&record, 65);
		int n = 0;
		while (strcmpi(texture[n].filename, record.fName) != 0) {
			++n;
		}
		anim = new w_face_animation;
		strcpy(anim->name, record.animName);
		strcpy(anim->filename, record.fName);
		anim->texHandle = texture[n].texHandle;
		anim->tu = texture[n].pu;
		anim->tv = texture[n].pv;
		anim->scalex = texture[n].scalex;
		anim->scaley = texture[n].scaley;
		anim->mesh = NULL;
		anim->group = record.group;
		anim->meshNum = m_rootbone->GetMeshForFaceAnimation((unsigned char)anim->group, NULL, 0);
		anim->mesh = (w_mesh**)g_mem.Alloc(anim->meshNum * sizeof(w_mesh*) + sizeof(w_mesh*));
		m_rootbone->GetMeshForFaceAnimation((unsigned char)anim->group, anim->mesh, 0);
		anim->mesh[anim->meshNum] = NULL;
		m_fanimList.AddItem(anim, anim->name, false);
	}
}

// wpuppet.cpp:2811
void WPuppet::LoadPET_Mesh(cFile* fp, WBone* root, w_pet_texture_info* texture, bool meshPet) {
	WVector position, normal, va, vb, vc;
	short temp[16][2];
	int i, j, k, m;
	int vertexCount, triangleCount, skinCount, end;
	w_skin_info* skins;
	w_pet_vertex* vertices;
	w_pet_tri_point* triangles;
	w_pet_texture_info** textures;
	WBone** bones;
	WBone* fake;
	if (!SeekChunkFromPET(fp, "MESH")) {
		return;
	}
	skins = NULL;
	if (meshPet) {
		skinCount = fp->GetByte();
		skins = new w_skin_info[skinCount];
		fp->Read(skins, skinCount * sizeof(w_skin_info));
	}
	fp->Read(&vertexCount, sizeof(vertexCount));
	vertices = new w_pet_vertex[vertexCount];
	for (i = 0; i < vertexCount; ++i) {
		fp->Read(&position, sizeof(position));
		if (meshPet) {
			fp->Read(&vertices[i].weight, sizeof(float));
		} else {
			vertices[i].weight = 1.0f;
		}
		k = 0;
		j = 0;
		do {
			temp[j][0] = fp->GetByte();
			temp[j][1] = ReadID(fp);
			k += temp[j][0];
			++j;
		} while (k < 255);
		normal = root->FindBone(temp[0][1])->Transform(position);
		vertices[i].x = normal.x;
		vertices[i].y = normal.y;
		vertices[i].z = normal.z;
		vertices[i].blendWeight = (unsigned char)temp[0][0];
		vertices[i].bonename = root->FindBone(temp[0][1])->GetBoneName();
		vertices[i].list = NULL;
		if (j > 1) {
			vertices[i].list = new w_pet_vertex::w_blended[j];
			vertices[i].listNum = j;
			for (m = 0; m < j; ++m) {
				WBone* blended = root->FindBone(temp[m][1]);
				vertices[i].list[m].blendWeight = (unsigned char)temp[m][0];
				vertices[i].list[m].bone = m_rootbone->FindBone(blended->GetBoneName(), 0);
			}
		} else {
			fp->GetByte();
			fp->GetByte();
		}
	}
	fp->Read(&triangleCount, sizeof(triangleCount));
	triangles = new w_pet_tri_point[triangleCount * 3];
	textures = new w_pet_texture_info*[triangleCount];
#ifdef PG_7XX
	// From version 0xFFFE0100 the triangle corner carries a UV-count byte and
	// that many (u,v) pairs (the 7xx clients; see pangya-glmodels read_MESH).
	// The matching build reads the original 0x18-byte corner.
	if (m_version >= 0xFFFE0100) {
		for (i = 0; i < triangleCount * 3; ++i) {
			fp->Read(&triangles[i].pos, 4);
			fp->Read(&triangles[i].nx, 4);
			fp->Read(&triangles[i].ny, 4);
			fp->Read(&triangles[i].nz, 4);
			unsigned char uvCount = (unsigned char)fp->GetByte();
			for (int q = 0; q < uvCount; ++q) {
				float u = 0.0f, v = 0.0f;
				fp->Read(&u, 4);
				fp->Read(&v, 4);
				if (q == 0) {
					triangles[i].tu = u;
					triangles[i].tv = v;
				}
			}
		}
	} else {
		fp->Read(triangles, triangleCount * 3 * sizeof(w_pet_tri_point));
	}
#else
	fp->Read(triangles, triangleCount * 3 * sizeof(w_pet_tri_point));
#endif
	for (i = 0; i < triangleCount; ++i) {
		textures[i] = &texture[fp->GetByte()];
	}
	m_faceNum = triangleCount;
	if (!meshPet) {
		fake = root->FindFakeBodyBone(vertices, vertexCount);
		for (i = 0; i < triangleCount * 3; ++i) {
			if (Wabs(triangles[i].nx) < g_EPSILON &&
				Wabs(triangles[i].ny) < g_EPSILON &&
				Wabs(triangles[i].nz) < g_EPSILON) {
				int j = i / 3 * 3;
				WBone* bone = root->FindBone(vertices[triangles[j].pos].bonename, 0);
				va = bone->Transform(WVector(vertices[triangles[j].pos].x, vertices[triangles[j].pos].y, vertices[triangles[j].pos].z));
				vb = bone->Transform(WVector(vertices[triangles[j + 1].pos].x, vertices[triangles[j + 1].pos].y, vertices[triangles[j + 1].pos].z));
				vc = bone->Transform(WVector(vertices[triangles[j + 2].pos].x, vertices[triangles[j + 2].pos].y, vertices[triangles[j + 2].pos].z));
				normal = WCrossProduct(va - vb, vc - vb);
			} else {
				WBone* bone = root->FindBone(vertices[triangles[i].pos].bonename, 0);
				if (fake && (strstr(vertices[triangles[i].pos].bonename, "Bip") || strstr(vertices[triangles[i].pos].bonename, "Bone") || strstr(vertices[triangles[i].pos].bonename, "Jaw"))) {
					normal = RotVec(WVector(triangles[i].nx, triangles[i].ny, triangles[i].nz), fake->GetMatrix());
				} else {
					normal = RotVec(WVector(triangles[i].nx, triangles[i].ny, triangles[i].nz), bone->GetMatrix());
				}
			}
			normal.Normalize();
			triangles[i].nx = normal.x;
			triangles[i].ny = normal.y;
			triangles[i].nz = normal.z;
		}
		bones = new WBone*[triangleCount * 3];
		for (i = 0; i < triangleCount * 3; ++i) {
			bones[i] = m_rootbone->FindBone(vertices[triangles[i].pos].bonename, 0);
			if (!bones[i]) {
				bones[i] = m_rootbone->FindBone(root->FindBone(vertices[triangles[i].pos].bonename, 0)->GetParent()->GetBoneName(), 0);
			}
		}
		m_rootbone->SetMesh(vertices, vertexCount, triangles, textures, bones, triangleCount);
	} else {
		for (i = 0; i < skinCount; ++i) {
			end = (skins[i].iSTriIdx + skins[i].nTris) * 3;
			for (int j = skins[i].iSTriIdx * 3; j < end; ++j) {
				if (Wabs(triangles[j].nx) < g_EPSILON &&
					Wabs(triangles[j].ny) < g_EPSILON &&
					Wabs(triangles[j].nz) < g_EPSILON) {
					int k = j / 3 * 3;
					w_pet_vertex* a = &vertices[triangles[k].pos + skins[i].iSVtxIdx];
					w_pet_vertex* b = &vertices[triangles[k + 1].pos + skins[i].iSVtxIdx];
					w_pet_vertex* c = &vertices[triangles[k + 2].pos + skins[i].iSVtxIdx];
					WBone* bone = root->FindBone(a->bonename, 0);
					va = bone->Transform(WVector(a->x, a->y, a->z));
					vb = bone->Transform(WVector(b->x, b->y, b->z));
					vc = bone->Transform(WVector(c->x, c->y, c->z));
					normal = WCrossProduct(va - vb, vc - vb);
					normal.Normalize();
				} else {
					normal = WVector(triangles[j].nx, triangles[j].ny, triangles[j].nz);
				}
				triangles[j].nx = normal.x;
				triangles[j].ny = normal.y;
				triangles[j].nz = normal.z;
			}
		}
		bones = new WBone*[triangleCount * 3];
		for (i = 0; i < skinCount; ++i) {
			end = (skins[i].iSTriIdx + skins[i].nTris) * 3;
			int j = skins[i].iSTriIdx * 3;
			if (j < end) {
				do {
					w_pet_vertex* vertex = &vertices[triangles[j].pos + skins[i].iSVtxIdx];
					bones[j] = m_rootbone->FindBone(vertex->bonename, 0);
					if (!bones[j]) {
						bones[j] = m_rootbone->FindBone(root->FindBone(vertex->bonename, 0)->GetParent()->GetBoneName(), 0);
					}
					++j;
				} while (j < end);
			}
		}
		for (i = 0; i < skinCount; ++i) {
			WBone* bone = m_rootbone->FindBone(skins[i].ucBoneID);
			bone->SetBoneMesh(vertices + skins[i].iSVtxIdx, skins[i].nVtxs, triangles + skins[i].iSTriIdx * 3, textures + skins[i].iSTriIdx, bones + skins[i].iSTriIdx * 3, skins[i].nTris);
			for (w_mesh* mesh = bone->m_mesh; mesh; mesh = mesh->next) {
				strcpy(mesh->mpetName, m_petName);
			}
		}
	}
	for (i = 0; i < vertexCount; ++i) {
		if (vertices[i].list) {
			delete[] vertices[i].list;
		}
	}
	delete[] bones;
	delete[] textures;
	delete[] triangles;
	delete[] vertices;
	if (meshPet) {
		delete[] skins;
	}
}

// wpuppet.cpp:3088
void WPuppet::GetDirectory(char* filename, char* directory) {
	strcpy(directory, filename);
	char* p = strrchr(directory, '/');
	if (p) {
		*p = 0;
	} else {
		*directory = 0;
	}
}

// wpuppet.cpp:3096
void WPuppet::GetName(char* filename, char* name) {
	char* p = strrchr(filename, '/');
	strcpy(name, p ? p + 1 : filename);
}

// wpuppet.cpp:3103
void WPuppet::MergeRootMotion(WList<w_motion_data*>* mdList, WBone* rootbone) {
	for (w_motion_data* md = mdList->Start(); md; md = mdList->Next()) {
		for (WBone* bone = rootbone; bone && md->applybone[0]; bone = bone->m_next) {
			if (!strcmp(bone->m_name, md->applybone))
				md->applybone[0] = 0;
		}
	}
}

// wpuppet.cpp:3113
// Loads the puppet from its ".pet" file; 1 when the file is missing, or when
// a type 0x11B puppet has a bone whose vertices are not all rigid.
int WPuppet::LoadPET(char* filename, bool alphatest, int iPetType) {
	char directory[256];
	char name[256];
	char buf[260];

	cFile* fp = GetResrcManager()->GetCFile(filename, -1);
	if (fp == NULL) {
		return 1;
	}

	GetDirectory(filename, directory);
	char* p = strrchr(filename, '/');
	strcpy(name, p ? p + 1 : filename);

	strcpy(m_petName, name);

	LoadPET(fp, alphatest, directory[0] ? directory : NULL, iPetType);
	CloseCFile(fp);
	if (m_mdList) {
		MergeRootMotion(m_mdList, m_rootbone);
	}
	m_rootbone->CombineVtxColor(true);
	SetRenderMode(m_rendMode);
	SetDefaultFaceAnimation();

	MakeTemporaryCenterBox();

	WMatrix defMat;
	defMat.Reset();
	m_mat = defMat;
	m_rootbone->Transform(m_mat, NULL, 1.0f, 0);

	if (m_iPetType == 0xFF || m_iPetType == 0x82) {
		xBuildTransfMatPtrList();
	}

	sprintf(buf, "WPuppet:%s", filename);
	SetLeakHint(buf);

	if (iPetType == 0x11B && m_rootbone->CheckMeshBoneAllVertexRigid() == false) {
		return 1;
	}
	return 0;
}

// wpuppet.cpp:3173
void WPuppet::FixNormal() {
	WMatrix mat;
	mat.Reset();
	m_rootbone->FixNormal(mat);
}

// wpuppet.cpp:3226
void WPuppet::xBuildTransfMatPtrList() {
	m_xTransfMatPtrList.clear();

	int xnmTransfs = 0; m_rootbone->CountBones(xnmTransfs);

	if (xnmTransfs <= 0) {
		return;
	}

	m_xTransfMatPtrList.resize(xnmTransfs, NULL);

	for (int i = 0; i < xnmTransfs; i++) {
		m_xTransfMatPtrList[i] = NULL;
	}
	m_rootbone->xBuildTransfMatPtrList(m_xTransfMatPtrList);

	AddOrginalMatrixPtrList(m_petName, m_xTransfMatPtrList);
}

// wpuppet.cpp:3249
void WPuppet::AttachMeshBone(WPuppet* pMPet, int hTex, int hOrgTex) {
	WBone* tail;
	for (tail = m_rootbone; tail->m_next != NULL; tail = tail->m_next) {
	}

	WBone* bone = NULL;
	while ((bone = pMPet->FindMeshBone(bone)) != NULL) {
		tail = tail->AttachMeshBone(bone, hTex, hOrgTex);
		tail->ResetMeshBone(m_rootbone, false);
		tail->CombineVtxColor(false);
	}

	if (!HasOrginalMatrixList(pMPet->m_petName)) {
		std::vector<const WMatrix*> matPtrList;
		int xnmTransfs = 0;
		pMPet->m_rootbone->CountBones(xnmTransfs);
		if (xnmTransfs > 0) {
			std::vector<const WMatrix*> newMatPtrList;
			matPtrList.resize(xnmTransfs);
			int i;
			for (i = 0; i < xnmTransfs; ++i) {
				matPtrList[i] = NULL;
			}
			pMPet->m_rootbone->xBuildTransfMatPtrList(matPtrList);
			newMatPtrList.resize(m_xTransfMatPtrList.size());
			for (i = 0; i < (int)m_xTransfMatPtrList.size(); ++i) {
				newMatPtrList[i] = NULL;
			}
			for (i = 0; i < xnmTransfs; ++i) {
				if (matPtrList[i] == NULL) {
					continue;
				}
				WBone* found = pMPet->m_rootbone->FindBone(i);
				WBone* mapped = m_boneList.Find(found->m_name);
				if (mapped == NULL) {
					mapped = m_rootbone->FindBone(found->m_name, 0);
					if (mapped == NULL) {
						continue;
					}
					m_boneList.AddItem(mapped, mapped->m_name, false);
				}
				int id = mapped->GetID();
				if (id < 0) {
					continue;
				}
				if (id < (int)newMatPtrList.size()) {
					newMatPtrList[id] = matPtrList[i];
				}
			}
			AddOrginalMatrixPtrList(pMPet->m_petName, newMatPtrList);
		}
	}
}

// wpuppet.cpp:3323
void WPuppet::DetachMeshBone(WPuppet* pet) {
	WBone* meshBone = NULL;
	while ((meshBone = pet->FindMeshBone(meshBone)) != NULL) {
		WBone* bone;
		WBone* prev;
		FindMeshBone(&bone, &prev, meshBone->m_name);
		if (bone) {
			if (prev) {
				prev->SetNext(bone->m_next, false);
			} else {
				m_rootbone = bone->m_next;
			}
			bone->SetNext(NULL, true);
			delete bone;
		}
	}
}

// wpuppet.cpp:3351
void WPuppet::ReplaceMeshBones(WPuppet* pOldMPet, WPuppet* pNewMPet, int hTex, int hOrgTex) {
	if (pOldMPet) {
		DetachMeshBone(pOldMPet);
	}
	if (pNewMPet) {
		AttachMeshBone(pNewMPet, hTex, hOrgTex);
	}
}

// wpuppet.cpp:3360
void WPuppet::ChangeTextureByHandle(int hTex, int hOrgTex, WPuppet* pMPet) {
	if (pMPet == NULL) {
		m_rootbone->ChangeTextureByHandle(hTex, hOrgTex, true, true);
		return;
	}

	WBone* meshBone = NULL;
	while ((meshBone = pMPet->FindMeshBone(meshBone)) != NULL) {
		WBone* root = m_rootbone;
		WBone* bone = NULL;
		if (!stricmp(root->m_name, meshBone->m_name)) {
			bone = root;
		} else {
			while (root->m_next) {
				if (!stricmp(root->m_next->m_name, meshBone->m_name)) {
					bone = root->m_next;
					break;
				}
				root = root->m_next;
			}
		}
		if (bone) {
			bone->ChangeTextureByHandle(hTex, hOrgTex, false, false);
		}
	}
}

// wpuppet.cpp:3387
void WPuppet::EndMeshBoneWork() {
	MergeNormal();
}

// wpuppet.cpp:3392
void WPuppet::MergeNormal() {
	WBone::w_normalmerge merge(*this);
}

// wpuppet.cpp:3397
void WPuppet::FindMeshBone(WBone** ppBone, WBone** ppPrev, const char* name) {
	if (ppBone) {
		*ppBone = NULL;
	}
	if (ppPrev) {
		*ppPrev = NULL;
	}

	WBone* bone = m_rootbone;
	if (!stricmp(bone->m_name, name)) {
		*ppBone = bone;
		return;
	}

	while (bone->m_next) {
		if (!stricmp(bone->m_next->m_name, name)) {
			*ppBone = bone->m_next;
			if (ppPrev) {
				*ppPrev = bone;
			}
			return;
		}
		bone = bone->m_next;
	}
}

// wpuppet.cpp:3433
WBone* WPuppet::FindMeshBone(WBone* prev) {
	WBone* bone;

	if (!prev) {
		for (bone = m_rootbone; bone; bone = bone->GetNext()) {
			if (bone->m_mesh) {
				break;
			}
		}
	} else {
		for (bone = m_rootbone; bone; bone = bone->GetNext()) {
			if (bone == prev) {
				break;
			}
		}

		if (bone) {
			for (bone = bone->GetNext(); bone; bone = bone->GetNext()) {
				if (bone->m_mesh) {
					break;
				}
			}
		}
	}

	return bone;
}

// wpuppet.cpp:3463
void WPuppet::UpdateMeshBonePtr(WPuppet* pet) {
	for (WBone* bone = m_rootbone; bone; bone = bone->GetNext()) {
		if (bone->m_mesh) {
			bone->ResetMeshBone(pet->m_rootbone, false);
		}
	}
}

// wpuppet.cpp:3475
w_face_animation* WPuppet::FindFaceAnim(char* animName) {
	return m_fanimList.Find(animName);
}

// wpuppet.cpp:3480
void WPuppet::DisableFog() {
	for (WBone* bone = m_rootbone; bone; bone = bone->GetNext()) {
		bone->DisableFog();
	}
}

// wpuppet.cpp:3488
void WPuppet::EnableAlphaBlend() {
}
// wpuppet.cpp:3492
void WPuppet::ScaleBone(char* boneName, float scale) {
	WBone* bone = FindBone(boneName);
	if (bone) {
		bone->SetScale(scale);
	}
}

// wpuppet.cpp:3498
bool WPuppet::CompareBasicMatrix(WPuppet* pet) const {
	if (!m_rootbone || !pet->m_rootbone) {
		return false;
	}

	return m_rootbone->CompareBasicMatrix(pet->m_rootbone);
}

// wpuppet.cpp:3505
std::map<std::string, WPuppet::w_original_matrix_set> WPuppet::ms_xOrginalMatrixList;

// wpuppet.cpp:3508
void WPuppet::AddOrginalMatrixPtrList(const std::string& name, const std::vector<const WMatrix*>& matPtrList) {
	std::map<std::string, w_original_matrix_set>::iterator it = ms_xOrginalMatrixList.find(name);

	if (it != ms_xOrginalMatrixList.end()) {
		return;
	}

	w_original_matrix_set& set = ms_xOrginalMatrixList[name];

	set.matPtrList.resize(matPtrList.size());
	set.matList.resize(matPtrList.size());

	for (int i = 0; i < (int)matPtrList.size(); i++) {
		set.matPtrList[i] = &set.matList[i];

		if (matPtrList[i]) {
			set.matList[i] = *matPtrList[i];
		} else {
			set.matList[i].Reset();
		}
	}
}

// wpuppet.cpp:3534
bool WPuppet::HasOrginalMatrixList(const std::string& name) {
	std::map<std::string, w_original_matrix_set>::iterator it = ms_xOrginalMatrixList.find(name);

	return it != ms_xOrginalMatrixList.end() ? true : false;
}

// wpuppet.cpp:3541
const std::vector<WMatrix>& WPuppet::GetOrginalMatrixList(const std::string& name) {
	std::map<std::string, w_original_matrix_set>::iterator it = ms_xOrginalMatrixList.find(name);

	return it->second.matList;
}

// wpuppet.cpp:3549
const std::vector<const WMatrix*>& WPuppet::GetOrginalMatrixPtrList(const std::string& name) {
	std::map<std::string, w_original_matrix_set>::iterator it = ms_xOrginalMatrixList.find(name);

	return it->second.matPtrList;
}

