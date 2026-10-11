// Original: d:\build\custom\temp\code\client\projectg\quadtree.cpp
#define QUADTREE_WMATH_INLINE
#define QUADTREE_GOLFBALL_INLINE
#include "quadtree.h"
#include "GolfDoc.h"
#include "contentsdoc.h"
#include "wpuppet.h"
#include "wbone.h"
#include "wview.h"
#include "wvideo.h"
#include "ClientSetting.h"
#include "commonutil.h"
#include "slicestr.h"
#include "wutil.h"
#include "wind.h"
#include "SoundManager.h"
#include "SharedDoc.h"
#include <stdio.h>
#include <math.h>

extern LightSet g_lightset;

// The sound listener position (no symbol).
extern WVector data_00b7bfac;

// projectg.cpp:184 (no header declares it yet)
void __cdecl TextOutA(WView *view, float x, float y, const char *format, ...);

// golfdoc.cpp:69 (no header declares it yet)
WVector GetTeeShotPos(void);

float RayIntersectAABB(const Waabb &aabb, const WVector &pos, const WVector &moveDelta, WPlane *pReturnPlane, float radius);

// The light's direction and the ambient colour (no symbols).
extern WVector data_00aa5278;
extern unsigned long data_00aa5288;


namespace intersection {
	float IntersectRayTriangle(const WVector &orig, const WVector &dir, const WVector *tri);
	bool SameSide(const WVector &p1, const WVector &p2, const WVector &a, const WVector &b);
	float IntersectPlane(const WVector &orig, const WVector &dir, const WPlane &plane, bool bothSides);
	WVector closestPointOnTriangle(const WVector &p, const WVector &a, const WVector &b, const WVector &c);
	WVector closestPointOnLine(const WVector &a, const WVector &b, const WVector &p);
	float intersectPolygon(const WVector &pos, const WVector &center, const WVector &dir, const float &radius);
	bool PointInTriangle(const WVector &p, const WVector &a, const WVector &b, const WVector &c);
}

// quadtree.h:41
CAniObj::CAniObj() {
	m_speed = 100.0f;
	m_active = false;
}

// quadtree.h:42
CAniObj::~CAniObj() {
}

// contentsdoc.h:472
bool CVectorSlideTestContainer::IsNewTypeVectorSlide(void) {
	return m_newTypeVectorSlide;
}

// contentsdoc.h:473
int CVectorSlideTestContainer::GetLimitSpeed(void) {
	return m_limitSpeed;
}

// contentsdoc.h:474
int CVectorSlideTestContainer::GetBoundHeight(void) {
	return m_boundHeight;
}

// contentsdoc.h:475
int CVectorSlideTestContainer::GetLimitVectorSlideCount(void) {
	return m_limitVectorSlideCount;
}

// quadtree.h
CQuadTree::sColData::sColData() {
}

// quadtree.h
fragmentsCache_s::fragmentsCache_s() {
}

// quadtree.cpp:35
WVector intersection::closestPointOnLine(const WVector &a, const WVector &b, const WVector &p) {
	WVector c = p - a;
	WVector V = (b - a).Normalize();
	float d = (a - b).Magnitude();
	float t = V.x * c.x + V.y * c.y + V.z * c.z;

	if (t < 0.0f)
		return a;
	if (t > d)
		return b;

	V = V * t;
	return a + V;
}

// quadtree.cpp:51
WVector intersection::closestPointOnTriangle(const WVector &p, const WVector &a, const WVector &b, const WVector &c) {
	WVector Rab = closestPointOnLine(a, b, p);
	WVector Rbc = closestPointOnLine(b, c, p);
	WVector Rca = closestPointOnLine(c, a, p);

	float dAB = (Rab - p).Magnitude();
	float dBC = (Rbc - p).Magnitude();
	float dCA = (Rca - p).Magnitude();

	if (dAB < dBC) {
		if (dAB < dCA)
			return Rab;
		else
			return Rca;
	} else {
		if (dBC < dCA)
			return Rbc;
		else
			return Rca;
	}
}

// quadtree.cpp:77
float intersection::intersectPolygon(const WVector &pos, const WVector &center, const WVector &dir, const float &radius) {
	WVector rayDir = dir * -1.0f;
	WVector diff = pos - center;
	float a = rayDir.x * rayDir.x + rayDir.y * rayDir.y + rayDir.z * rayDir.z;
	float b = diff.x * rayDir.x + diff.y * rayDir.y + diff.z * rayDir.z;
	float c = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
	float disc = b * b - a * (c - radius * radius);

	if (disc < 0.0f)
		return 1.0f;

	float root = sqrtf(disc);
	float inv = 1.0f / a;
	float t1 = (-1.0f * b - root) * inv;
	float t2 = (root - b) * inv;

	if (t1 < 0.0f) {
		if (t2 > 0.0f && t2 < 1.0f)
			return t2;
	} else if (t1 < 1.0f) {
		return t1;
	}

	return 1.0f;
}

// quadtree.cpp:111
float intersection::IntersectRayTriangle(const WVector &orig, const WVector &dir, const WVector *tri) {
	WVector edge1, edge2, pvec, tvec, qvec;

	edge1 = tri[1] - tri[0];
	edge2 = tri[2] - tri[0];
	pvec = WCrossProduct(dir, edge2);
	float det = edge1.x * pvec.x + edge1.y * pvec.y + edge1.z * pvec.z;

	if (WAbsBySign(det) < WMATH_EPSILON)
		return 1.0f;

	det = 1.0f / det;

	tvec = orig - tri[0];

	float u = tvec.x * (pvec.x * det) + tvec.y * (pvec.y * det) + tvec.z * (pvec.z * det);
	if (u < 0.0f || u > 1.0f)
		return 1.0f;

	qvec = WCrossProduct(tvec, edge1);

	float v = dir.x * (qvec.x * det) + dir.y * (qvec.y * det) + dir.z * (qvec.z * det);
	if (v < 0.0f || v > 1.0f)
		return 1.0f;

	float t = edge2.x * (qvec.x * det) + edge2.y * (qvec.y * det) + edge2.z * (qvec.z * det);

	return Min(t, 1.0f);
}

// quadtree.cpp:146
bool intersection::SameSide(const WVector &p1, const WVector &p2, const WVector &a, const WVector &b) {
	WVector cp1, cp2;
	cp1 = WCrossProduct(b - a, p1 - a);
	cp2 = WCrossProduct(b - a, p2 - a);

	if (cp2.x * cp1.x + cp2.y * cp1.y + cp2.z * cp1.z > 0.0f
		|| WisEqual(cp1, WVector::ZERO, WMATH_EPSILON) || WisEqual(cp2, WVector::ZERO, WMATH_EPSILON))
		return true;

	return false;
}

// quadtree.cpp:159
bool intersection::PointInTriangle(const WVector &p, const WVector &a, const WVector &b, const WVector &c) {
	if (SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b))
		return true;

	return false;
}

// quadtree.cpp:167
float intersection::IntersectPlane(const WVector &orig, const WVector &dir, const WPlane &plane, bool bothSides) {
	float d1 = orig.x * plane.x + orig.y * plane.y + orig.z * plane.z + plane.dis;
	WVector end = orig + dir;
	float d2 = end.x * plane.x + end.y * plane.y + end.z * plane.z + plane.dis;

	if (!(dir.x * plane.x + dir.y * plane.y + dir.z * plane.z > 0.0f)) {
		if (bothSides && d1 < 0.0f)
			return 0.0f;

		if (d1 * d2 <= 0.0f)
			return (d1 > 0.0f ? d1 : -d1) / ((d1 > 0.0f ? d1 : -d1) + (d2 > 0.0f ? d2 : -d2));
	}

	return 1.0f;
}

// quadtree.cpp:188
void ReflectVector(const WVector &normal, WVector &v) {
	v = v - normal * (2.0f * (normal.x * v.x + normal.y * v.y + normal.z * v.z));
}

int CPVSPool::ms_InstanceIdx;

// quadtree.cpp:778
CQuadTree &CPVSPool::GetInstance(void) {
	return m_pool[ms_InstanceIdx];
}

// quadtree.cpp:784
void CPVSPool::SetInstanceIdx(int idx) {
	ms_InstanceIdx = idx;
}

// quadtree.cpp:790
void SetPVSIndex(int idx) {
	CPVSPool::SetInstanceIdx(idx);
}

// quadtree.cpp:795
CQuadTree &GetPVS(void) {
	static CPVSPool pool;
	return pool.GetInstance();
}

// quadtree.cpp:802
void CQuadTree::AddWater(Waabb aabb, int index) {
	sWater water;
	water.m_aabb = aabb;
	water.m_index = index;
	m_waterList.push_back(water);
}

// quadtree.cpp:850
void CQuadTree::ResetVariable(void) {
	m_colList.clear();

	g_colNum = 0;

	m_ballState = 0;
	m_bounceCount = 0;
	m_cupRollCount = 0;
	m_objColStop = false;
	m_applyForce = true;
}

// quadtree.cpp:866
CQuadTree::CQuadTree() : m_pPolySoup(0), m_pBaseModel(0) {
	m_pQuads = 0;
	m_slopeFactor = 1.1f;
	m_bounceFactor = 1.0f;
	m_rollFactor = 1.0f;
	m_greenSlope = 0.2f;

	g_colNum = 0;

	m_holeCupRadius = GolfBall().GetRadius() * 3.0f;

	m_baseVtxColor = WVector(255.0f, 255.0f, 255.0f);
	m_objVtxColor = WVector(255.0f, 255.0f, 255.0f);
	m_vtxColorTestMode = 0;
	m_gravity = 1.0f;
}

// quadtree.cpp:886
CQuadTree::~CQuadTree() {
	for (int i = 0; i < m_quadCols * m_quadRows; i++) {
		if (m_pQuads[i].m_pList) {
			delete[] m_pQuads[i].m_pList;
			m_pQuads[i].m_pList = 0;
		}
	}
	if (m_pQuads) {
		delete[] m_pQuads;
		m_pQuads = 0;
	}

	m_waterList.clear();
	m_colList.clear();

	for (unsigned int i = 0; i < m_aniObjs.size(); i++) {
		if (m_aniObjs[i]) {
			delete m_aniObjs[i];
			m_aniObjs[i] = 0;
		}
	}
	m_aniObjs.clear();

	if (m_pBuffer2220) {
		delete[] m_pBuffer2220;
		m_pBuffer2220 = 0;
	}
	if (m_pBuffer2224) {
		delete[] m_pBuffer2224;
		m_pBuffer2224 = 0;
	}
	if (m_pBuffer2224) {
		delete[] m_pBuffer2224;
		m_pBuffer2224 = 0;
	}
}

// quadtree.cpp:907
void CQuadTree::Reserve(short count) {
	typedef std::vector<CAniObj *> AniObjList;

	for (unsigned int i = 0; i < m_aniObjs.size(); i++) {
		if (m_aniObjs[i]) {
			delete m_aniObjs[i];
			m_aniObjs[i] = 0;
		}
	}
	m_aniObjs.~AniObjList();

	if (count > 0) {
		m_aniObjs.reserve(count);
	}
}

// quadtree.cpp:923
unsigned long CQuadTree::MakeRand(unsigned long range) {
	while (m_randSeed < range)
		m_randSeed = m_randSeed * m_randSeed + 1;

	m_randSeed = m_randSeed % range;
	return m_randSeed;
}

// quadtree.cpp:933
void CQuadTree::MakeObjStructure(short objIndex, int option, bool bRebuild) {
	WPolySoup::sModel *pModel;
	if (IsLocalContent((localContentType_t)0x56)) {
		if (option != -1)
			pModel = m_pPolySoup->m_models[option];
		else
			pModel = m_pPolySoup->m_models[objIndex];
	} else {
		pModel = m_pPolySoup->m_models[objIndex];
	}

	// A model without a puppet: drop its animated object.
	if (pModel->m_pPuppet == 0) {
		for (unsigned int i = 0; i < m_aniObjs.size(); i++) {
			if (m_aniObjs[i] && m_aniObjs[i]->m_objIndex == pModel->m_objIndex) {
				delete m_aniObjs[i];
				m_aniObjs[i] = 0;
				return;
			}
		}
		return;
	}

	if (pModel->m_subOrder != 0) {
		CAniObj *pAni = 0;

		if (bRebuild) {
			for (unsigned int i = 0; i < m_aniObjs.size(); i++) {
				if (m_aniObjs[i] && m_aniObjs[i]->m_objIndex == pModel->m_objIndex) {
					pAni = m_aniObjs[i];
					break;
				}
			}
		}

		int randRange;
		if (pModel->m_pPuppet->GetAnimationLength() > 0.0f)
			randRange = (int)(pModel->m_pPuppet->GetAnimationLength() * 50.0f);
		else
			randRange = 50;

		switch (pModel->m_subOrder) {
		case 1: {
			CAniObjMove *pMove;
			if (bRebuild)
				pMove = (CAniObjMove *)pAni;
			else
				pMove = new CAniObjMove;

			pMove->m_standTime = 0.0f;
			pMove->m_moveTime = 1.0f;
			pMove->m_startPos = pModel->m_matrix.pivot;
			pMove->m_endPos = pModel->m_matrix.pivot;

			if (pModel->m_baseName[0]) {
				char node[0x20];
				WSliceStr slice = pModel->m_baseName;
				slice.Get("*node", "%s", node);
				slice.Get("*stand", "%f", &pMove->m_standTime);
				slice.Get("*move", "%f", &pMove->m_moveTime);

				CNodeContainer *pNode = m_pPolySoup->FindNode(node);
				if (pNode) {
					pMove->m_startPos = pNode->m_points[0];
					pMove->m_endPos = pNode->m_points[1];
					pModel->m_matrix.pivot = pNode->m_points[0];
				}
			}
			pAni = pMove;
			break;
		}
		case 2: {
			CAniObjScale *pScale = (CAniObjScale *)pAni;
			if (!bRebuild)
				pScale = new CAniObjScale;

			float dy = pModel->m_aabb.max.y - pModel->m_aabb.min.y;
			float dx = pModel->m_aabb.max.x - pModel->m_aabb.min.x;
			float dz = pModel->m_aabb.max.z - pModel->m_aabb.min.z;
			pScale->m_invSize = 1.0f / Max(Max(dx, dy), dz);
			pAni = pScale;
			break;
		}
		case 3:
		case 4:
			if (!bRebuild)
				pAni = new CAniObj;
			break;
		}

		if (pAni) {
			pAni->m_rand = MakeRand(randRange);
			pAni->m_type = pModel->m_subOrder;
			pAni->m_objIndex = objIndex;
			if (!bRebuild)
				m_aniObjs.push_back(pAni);
		}
	}

	if (pModel->m_reservedByte == 3)
		return;

	// The base quads: which cells of the base grid the model's box covers.
	float lo[3] = {0.0f, 0.0f, 0.0f};
	float hi[3] = {0.0f, 0.0f, 0.0f};

	if (pModel->m_subOrder != 1 && pModel->m_subOrder != 3) {
		int minCol = (int)((pModel->m_aabb.min.x - m_quadOrigin.x) * 0.05f);
		if (minCol < 0)
			minCol = 0;
		else if (minCol > m_quadCols - 1)
			minCol = m_quadCols - 1;

		int maxCol = (int)((pModel->m_aabb.max.x - m_quadOrigin.x) * 0.05f);
		if (maxCol < 0)
			maxCol = 0;
		else if (maxCol > m_quadCols - 1)
			maxCol = m_quadCols - 1;

		int minRow = (int)((pModel->m_aabb.min.z - m_quadOrigin.z) * 0.05f);
		if (minRow < 0)
			minRow = 0;
		else if (minRow > m_quadRows - 1)
			minRow = m_quadRows - 1;

		int maxRow = (int)((pModel->m_aabb.max.z - m_quadOrigin.z) * 0.05f);
		if (maxRow < 0)
			maxRow = 0;
		else if (maxRow > m_quadRows - 1)
			maxRow = m_quadRows - 1;

		for (int row = minRow; row <= maxRow; row++) {
			for (int col = minCol; col <= maxCol; col++) {
				WPolySoup::sQuad &quad = m_pQuads[m_quadCols * row + col];

				if (quad.m_count2 == 0) {
					short *pNewList = new short[4];
					quad.m_pList2 = (int *)pNewList;    // the list holds short object indices
					pNewList[0] = objIndex;
					quad.m_count2 = 1;
					continue;
				}

				short *pList = (short *)quad.m_pList2;
				int j;
				for (j = 0; j < quad.m_count2; j++) {
					if (pList[j] == objIndex)
						break;
				}

				if (j < quad.m_count2)
					continue;

				if (quad.m_count2 % 4 == 0) {
					short *pTemp = new short[quad.m_count2];
					memcpy(pTemp, pList, quad.m_count2 * sizeof(short));

					delete[] pList;
					pList = new short[quad.m_count2 + 4];
					memcpy(pList, pTemp, quad.m_count2 * sizeof(short));
					quad.m_pList2 = (int *)pList;

					delete[] pTemp;
				}

				pList[quad.m_count2] = objIndex;
				quad.m_count2++;
			}
		}
	}

	if (pModel->m_reservedByte & 2)
		return;

	// The model's collision triangles, in world space for the static kinds.
	if (bRebuild) {
		delete[] pModel->m_pTriangles;
		if (pModel->m_pQuads)
			delete[] pModel->m_pQuads;
	}

	int faceNum = pModel->m_pPuppet->GetFaceNum();
	pModel->m_pTriangles = new WPolySoup::sTriangle[faceNum];

	bool bHasAbv = pModel->m_pPuppet->FindBoneByTail("abv") != 0;

	int triIndex = 0;
	for (w_bound_box *pBox = pModel->m_pPuppet->m_bbList.Start(); pBox; pBox = pModel->m_pPuppet->m_bbList.Next()) {
		WBone *pBone = pBox->bone;

		if (_stricmp(pBox->info->name, pBone->GetBoneName()) != 0)
			continue;

		if (bHasAbv) {
			if (!strstr(_strlwr(pBox->info->name), "abv"))
				continue;
		}

		for (w_mesh *pMesh = pBone->GetMesh(); pMesh; pMesh = pMesh->next) {
			for (int k = 0; k < pMesh->indexNum; k += 3) {
				WPolySoup::sTriangle *pTri = &pModel->m_pTriangles[triIndex];

				if (pModel->m_subOrder == 0 || pModel->m_subOrder == 4) {
					for (int j = 0; j < 3; j++)
						pTri->m_vertex[j] = pMesh->vecList[pMesh->indexList[k + j]] * pBone->GetMatrix();

					pTri->m_normal = WCrossProduct(pTri->m_vertex[2] - pTri->m_vertex[1], pTri->m_vertex[0] - pTri->m_vertex[1]);
					pTri->m_normal.Normalize();
					pTri->m_d = -pTri->m_normal.z * pTri->m_vertex[0].z + -pTri->m_normal.y * pTri->m_vertex[0].y + -pTri->m_normal.x * pTri->m_vertex[0].x;
				}

				// The bone the triangle belongs to (the unnamed word at 0x44).
				*(WBone **)((char *)pTri + 0x44) = pBone;
				pTri->m_doubleSided = (pMesh->xiDrawFlag2 >> 10) & 1;
				pTri->m_propIndex = m_pPolySoup->m_pTextures[m_pPolySoup->m_texIndexMap[pMesh->texHandle & 0x7FF]].m_param;
				pTri->m_texIndex = m_pPolySoup->m_pTextures[m_pPolySoup->m_texIndexMap[pMesh->texHandle & 0x7FF]].m_handle;

				triIndex++;
				if (triIndex >= faceNum)
					goto trianglesDone;
			}
		}
	}
trianglesDone:

	// The object quads: one quad holding every triangle, or the 8 octants.
	if (pModel->m_pPuppet->GetFaceNum() < 0x200 || pModel->m_subOrder == 2 || pModel->m_subOrder == 1 || pModel->m_subOrder == 3) {
		pModel->m_pQuads = new WPolySoup::sQuad[1];
		*((unsigned char *)pModel + 0xE4) = 1;      // the unnamed byte at 0xE4: 1 = one quad, 0 = octants
		pModel->m_pQuads[0].m_pList = new int[pModel->m_pPuppet->GetFaceNum()];
		pModel->m_pQuads[0].m_count = (short)pModel->m_pPuppet->GetFaceNum();

		for (int i = 0; i < pModel->m_pPuppet->GetFaceNum(); i++)
			pModel->m_pQuads[0].m_pList[i] = i;
		return;
	}

	pModel->m_pQuads = new WPolySoup::sQuad[8];
	*((unsigned char *)pModel + 0xE4) = 0;

	// The center of the model: the mean of its vertices in world space.
	pModel->m_basePos.x = 0.0f;
	pModel->m_basePos.y = 0.0f;
	pModel->m_basePos.z = 0.0f;

	int vertexCount = 0;
	for (w_bound_box *pBox = pModel->m_pPuppet->m_bbList.Start(); pBox; pBox = pModel->m_pPuppet->m_bbList.Next()) {
		WBone *pBone = pBox->bone;

		if (_stricmp(pBox->info->name, pBone->GetBoneName()) != 0)
			continue;

		for (w_mesh *pMesh = pBone->GetMesh(); pMesh; pMesh = pMesh->next) {
			for (int v = 0; v < pMesh->vtxNum; v++) {
				WVector pos = pMesh->vecList[v] * pBone->GetMatrix();
				vertexCount++;
				pModel->m_basePos.x += pos.x;
				pModel->m_basePos.y += pos.y;
				pModel->m_basePos.z += pos.z;
			}
		}
	}

	float invCount = 1.0f / vertexCount;
	pModel->m_basePos.x *= invCount;
	pModel->m_basePos.y *= invCount;
	pModel->m_basePos.z *= invCount;

	for (int i = 0; i < pModel->m_pPuppet->GetFaceNum(); i++) {
		WPolySoup::sTriangle *pTri = &pModel->m_pTriangles[i];

		lo[0] = Min(Min(pTri->m_vertex[0].x, pTri->m_vertex[1].x), pTri->m_vertex[2].x);
		lo[1] = Min(Min(pTri->m_vertex[0].y, pTri->m_vertex[1].y), pTri->m_vertex[2].y);
		lo[2] = Min(Min(pTri->m_vertex[0].z, pTri->m_vertex[1].z), pTri->m_vertex[2].z);

		hi[0] = Max(Max(pTri->m_vertex[0].x, pTri->m_vertex[1].x), pTri->m_vertex[2].x);
		hi[1] = Max(Max(pTri->m_vertex[0].y, pTri->m_vertex[1].y), pTri->m_vertex[2].y);
		hi[2] = Max(Max(pTri->m_vertex[0].z, pTri->m_vertex[1].z), pTri->m_vertex[2].z);

		// Which octants around the center the triangle touches.
		int octants[8] = {1, 1, 1, 1, 1, 1, 1, 1};

		const float *pCenter = &pModel->m_basePos.x;
		for (int axis = 0; axis < 3; axis++) {
			int bit = 1 << axis;
			if (hi[axis] < pCenter[axis]) {
				for (int q = 0; q < 8; q++) {
					if ((bit & q) == bit)
						octants[q] = 0;
				}
			} else if (lo[axis] > pCenter[axis]) {
				for (int q = 0; q < 8; q++) {
					if (!(q & bit))
						octants[q] = 0;
				}
			}
		}

		for (int q = 0; q < 8; q++) {
			if (octants[q] != 1)
				continue;

			WPolySoup::sQuad &quad = pModel->m_pQuads[q];

			if (quad.m_count == 0) {
				quad.m_pList = new int[0x40];
				quad.m_pList[0] = i;
				quad.m_count = 1;
				continue;
			}

			if (quad.m_count % 0x40 == 0) {
				int *pTemp = new int[quad.m_count];
				memcpy(pTemp, quad.m_pList, quad.m_count * sizeof(int));

				delete[] quad.m_pList;
				quad.m_pList = new int[quad.m_count + 0x40];
				memcpy(quad.m_pList, pTemp, quad.m_count * sizeof(int));

				delete[] pTemp;
			}

			quad.m_pList[quad.m_count] = i;
			quad.m_count++;
		}
	}
}

// quadtree.cpp:1382
unsigned char CQuadTree::GetObjQuad(const WVector &pos, const WVector &center) {
	unsigned char quad = 0;
	if (pos.x > center.x)
		quad = 1;
	if (pos.y > center.y)
		quad |= 2;
	if (pos.z > center.z)
		quad |= 4;

	return quad;
}

// quadtree.cpp:1618
void CQuadTree::MakeBaseStructure(WPolySoup::sModel *pModel, bool bRebuild) {
	ResetWater();

	m_pBaseModel = pModel;

	if (m_pQuads) {
		delete[] m_pQuads;
		m_pQuads = 0;
	}

	const Waabb &aabb = m_pBaseModel->m_aabb;

	m_lowest = 100000.0f;
	m_quadOrigin = WVector(aabb.min.x, 0.0f, aabb.min.z);
	m_quadCols = 1 - (int)((m_pBaseModel->m_aabb.max.x - m_pBaseModel->m_aabb.min.x) * -0.05f);
	m_quadRows = 1 - (int)((m_pBaseModel->m_aabb.max.z - m_pBaseModel->m_aabb.min.z) * -0.05f);

	if (bRebuild) {
		if (m_pQuads)
			delete[] m_pQuads;
		m_pQuads = 0;
	}

	m_pQuads = new WPolySoup::sQuad[m_quadCols * m_quadRows];

	for (int i = 0; i < m_pBaseModel->m_pPuppet->GetFaceNum(); i++) {
		WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[i];

		if (tri.m_doubleSided) {
			if (Abs(m_pBaseModel->m_pTriangles[i].m_normal.y) > (float)cos(0.34906585f))
				m_lowest = Min(Min(m_lowest, tri.m_vertex[0].y), Min(tri.m_vertex[1].y, tri.m_vertex[2].y));
		} else if (m_pBaseModel->m_pTriangles[i].m_normal.y > (float)cos(0.34906585f)) {
			m_lowest = Min(Min(m_lowest, tri.m_vertex[0].y), Min(tri.m_vertex[1].y, tri.m_vertex[2].y));
		}

		float minX = Min(tri.m_vertex[0].x, Min(tri.m_vertex[1].x, tri.m_vertex[2].x));
		float maxX = Max(tri.m_vertex[0].x, Max(tri.m_vertex[1].x, tri.m_vertex[2].x));
		float minZ = Min(tri.m_vertex[0].z, Min(tri.m_vertex[1].z, tri.m_vertex[2].z));
		float maxZ = Max(tri.m_vertex[0].z, Max(tri.m_vertex[1].z, tri.m_vertex[2].z));

		minX -= m_quadOrigin.x;
		maxX -= m_quadOrigin.x;
		minZ -= m_quadOrigin.z;
		maxZ -= m_quadOrigin.z;

		int minCol = (int)(minX * 0.05f);
		int maxCol = (int)(maxX * 0.05f);
		int minRow = (int)(minZ * 0.05f);
		int maxRow = (int)(maxZ * 0.05f);

		for (int row = minRow; row <= maxRow; row++) {
			for (int col = minCol; col <= maxCol; col++) {
				int index = row * m_quadCols + col;

				if (m_pQuads[index].m_count == 0) {
					m_pQuads[index].m_pList = new int[0x10];
					m_pQuads[index].m_pList[0] = i;
					m_pQuads[index].m_count = 1;
					continue;
				}

				int j;
				for (j = 0; j < m_pQuads[index].m_count; j++) {
					if (m_pQuads[index].m_pList[j] == i)
						break;
				}

				if (j < m_pQuads[index].m_count)
					continue;

				if (m_pQuads[index].m_count % 0x10 == 0) {
					int *pTemp = new int[m_pQuads[index].m_count];
					memcpy(pTemp, m_pQuads[index].m_pList, m_pQuads[index].m_count * sizeof(int));

					delete[] m_pQuads[index].m_pList;
					m_pQuads[index].m_pList = new int[m_pQuads[index].m_count + 0x10];
					memcpy(m_pQuads[index].m_pList, pTemp, m_pQuads[index].m_count * sizeof(int));

					delete[] pTemp;
				}

				m_pQuads[row * m_quadCols + col].m_pList[m_pQuads[row * m_quadCols + col].m_count] = i;
				m_pQuads[row * m_quadCols + col].m_count++;
			}
		}
	}

	m_lowest = Max(m_lowest, m_pBaseModel->m_aabb.min.y) - 1.0f;
}

// quadtree.cpp:1729
WVector CQuadTree::ColTest(const WVector &pos, bool bBase, int *pIndex, bool bAll) {
	WVector result = pos;
	bool found = false;

	result.y = -99999.0f;

	for (int i = 0; i < m_meshNum; i++) {
		WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[m_meshList[i]];

		if (!bAll && tri.m_doubleSided)
			continue;

		if (!tri.m_doubleSided && tri.m_normal.y < 0.0f)
			continue;

		float ny = tri.m_normal.y;
		if (ny < 0.0f)
			ny = -ny;
		if (ny < 0.1f)
			continue;

		WVector a = tri.m_vertex[0];
		a.y = result.y;
		WVector b = tri.m_vertex[1];
		b.y = result.y;
		WVector c = tri.m_vertex[2];
		c.y = result.y;

		if (intersection::SameSide(result, a, b, c) && intersection::SameSide(result, b, a, c) && intersection::SameSide(result, c, a, b)) {
			float y = -((result.z * tri.m_normal.z + result.x * tri.m_normal.x + tri.m_d) / tri.m_normal.y);

			if (y > result.y) {
				result.y = y;
				found = true;
				if (pIndex)
					*pIndex = m_meshList[i];
			}
		}
	}

	if (!found) {
		if (bBase) {
			if (pIndex)
				*pIndex = -1;
			return pos;
		}
		if (GolfBall().m_pos.y < m_pBaseModel->m_aabb.min.y)
			result.y = m_pBaseModel->m_aabb.min.y;
	}

	return result;
}

// quadtree.cpp:1869
WVector CQuadTree::TestCameraCollision(const WVector &target) {
	WVector camera = m_pPolySoup->GetCameraPos(target, GolfBall().m_pos);

	camera = GetGroundPoint(camera, true, 0, true);
	float limit = target.y - GOLFDOC()->GetCameraData().m_groundOffset;
	camera.y = (camera.y < limit ? camera.y : limit) + GOLFDOC()->GetCameraData().m_groundOffset;

	return camera;
}

// quadtree.cpp:1883
int CQuadTree::GetMeshIndex(const WVector &pos, bool bAll) {
	int result = -1;
	std::vector<int> candidates;
	float best = 100000.0f;

	GetMeshList(pos);

	for (int i = 0; i < m_meshNum; i++) {
		const WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[m_meshList[i]];

		// back faces do not count unless the triangle is double sided
		if (!tri.m_doubleSided && tri.m_normal.y < 0.0f)
			continue;

		// the triangle seen from above
		WVector a = tri.m_vertex[0];
		a.y = pos.y;
		WVector b = tri.m_vertex[1];
		b.y = pos.y;
		WVector c = tri.m_vertex[2];
		c.y = pos.y;

		if (PointInTriangle(pos, a, b, c))
			candidates.push_back(m_meshList[i]);
	}

	if (candidates.size() == 1)
		return candidates[0];

	// several triangles are over the point: the plane nearest to it wins
	for (unsigned int i = 0; i < candidates.size(); i++) {
		const WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[candidates[i]];

		float dist = tri.m_normal.z * pos.z + tri.m_normal.y * pos.y + pos.x * tri.m_normal.x + tri.m_d;
		if (dist < 0.0f && tri.m_doubleSided)
			dist *= -1.0f;

		if (fabs(dist) < fabs(best)) {
			best = dist;
			result = candidates[i];
		}
	}

	return result;
}

// quadtree.cpp:1949
int CQuadTree::GetObjIndex(const WVector &pos, bool bAll) {
	int result = -1;
	std::vector<int> candidates;
	float best = 100000.0f;

	if (!IsLocalContent((localContentType_t)0x56))
		return -1;

	GetObjList(pos, pos);

	for (int i = 0; i < m_objNum; i++) {
		const WPolySoup::sTriangle *pTri = m_pPolySoup->m_models[m_objList[i]]->m_pTriangles;

		if (!pTri->m_doubleSided && pTri->m_normal.y < 0.0f)
			continue;

		WVector a = pTri->m_vertex[0];
		a.y = pos.y;
		WVector b = pTri->m_vertex[1];
		b.y = pos.y;
		WVector c = pTri->m_vertex[2];
		c.y = pos.y;

		if (PointInTriangle(pos, a, b, c))
			candidates.push_back(m_objList[i]);
	}

	if (candidates.size() == 1)
		return m_objList[0];

	for (unsigned int i = 0; i < candidates.size(); i++) {
		const WPolySoup::sTriangle *pTri = m_pPolySoup->m_models[candidates[i]]->m_pTriangles;

		float dist = pTri->m_normal.z * pos.z + pTri->m_normal.y * pos.y + pos.x * pTri->m_normal.x + pTri->m_d;
		if (dist < 0.0f && pTri->m_doubleSided)
			dist *= -1.0f;

		if (fabs(dist) < fabs(best)) {
			best = dist;
			result = candidates[i];
		}
	}

	return result;
}

// quadtree.cpp:2019
WVector CQuadTree::GetGroundPoint(WVector pos, bool bBase, int *pIndex, bool bWater) {
	GetMeshList(pos);

	int index = -1;
	pos = ColTest(pos, bBase, &index, true);

	if (bBase && index == -1) {
		pos.y = m_pBaseModel->m_aabb.min.y;
	}

	if (pIndex != 0) {
		*pIndex = index;
	}

	if (bWater) {
		for (int i = 0; i < (int)m_waterList.size(); i++) {
			sWater water = m_waterList[i];
			if (pos.x > water.m_aabb.min.x && pos.x < water.m_aabb.max.x
				&& pos.z > water.m_aabb.min.z && pos.z < water.m_aabb.max.z
				&& pos.y < water.m_aabb.max.y) {
				pos.y = water.m_aabb.max.y;
				if (pIndex != 0) {
					*pIndex = -2;
				}
				break;
			}
		}
	}

	if (pos.y < m_pBaseModel->m_aabb.min.y) {
		pos.y = m_pBaseModel->m_aabb.min.y;
		if (pIndex != 0) {
			*pIndex = Min(*pIndex, -1);
		}
	}

	return pos;
}

// quadtree.cpp:2065
float CQuadTree::GetHighestGround(WVector pos, int *pIndex) {
	GetMeshList(pos);

	float highest = -100000.0f;
	bool found = false;

	for (int i = 0; i < m_meshNum; i++) {
		int index = m_meshList[i];
		WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[index];

		float ny = tri.m_normal.y;
		if (ny < 0.0f)
			ny = -ny;
		if (ny < 0.1f)
			continue;

		if (!tri.m_doubleSided && tri.m_normal.y < 0.0f)
			continue;

		WVector a = tri.m_vertex[0];
		a.y = pos.y;
		WVector b = tri.m_vertex[1];
		b.y = pos.y;
		WVector c = tri.m_vertex[2];
		c.y = pos.y;

		if (intersection::SameSide(pos, a, b, c) && intersection::SameSide(pos, b, a, c) && intersection::SameSide(pos, c, a, b)) {
			float y = -((pos.z * tri.m_normal.z + pos.x * tri.m_normal.x + tri.m_d) / tri.m_normal.y);

			// The ground closest to the given height wins.
			if (Abs(y - pos.y) < Abs(highest - pos.y)) {
				found = true;
				if (pIndex)
					*pIndex = index;
				highest = y;
			}
		}
	}

	if (!found) {
		if (pIndex)
			*pIndex = -1;
		return pos.y;
	}

	return highest;
}

// quadtree.cpp:2118
float CQuadTree::GetLowestGround(WVector pos) {
	GetMeshList(pos);

	float lowest = 100000.0f;
	bool found = false;

	for (int i = 0; i < m_meshNum; i++) {
		WPolySoup::sTriangle &tri = m_pBaseModel->m_pTriangles[m_meshList[i]];

		float ny = tri.m_normal.y;
		if (ny < 0.0f)
			ny = -ny;
		if (ny < 0.1f)
			continue;

		if (!tri.m_doubleSided && tri.m_normal.y < 0.0f)
			continue;

		WVector a = tri.m_vertex[0];
		a.y = pos.y;
		WVector b = tri.m_vertex[1];
		b.y = pos.y;
		WVector c = tri.m_vertex[2];
		c.y = pos.y;

		if (intersection::SameSide(pos, a, b, c) && intersection::SameSide(pos, b, a, c) && intersection::SameSide(pos, c, a, b)) {
			float y = -((pos.z * tri.m_normal.z + pos.x * tri.m_normal.x + tri.m_d) / tri.m_normal.y);

			if (y < lowest) {
				lowest = y;
				found = true;
			}
		}
	}

	if (!found)
		return pos.y;

	return lowest;
}

// quadtree.cpp:2157
void CQuadTree::GetMeshList(WVector pos) {
	m_meshNum = 0;
	WVector local = pos - m_quadOrigin;
	int row = Between(0, (int)(local.z * 0.05f), m_quadRows - 1);
	int col = Between(0, (int)(local.x * 0.05f), m_quadCols - 1);

	for (int i = 0; i < m_pQuads[row * m_quadCols + col].m_count; i++) {
		int j;
		for (j = 0; j < m_meshNum; j++) {
			if (m_pQuads[row * m_quadCols + col].m_pList[i] == m_meshList[j])
				break;
		}

		if (j == m_meshNum) {
			m_meshList[m_meshNum] = m_pQuads[row * m_quadCols + col].m_pList[i];
			m_meshNum++;
		}
	}
}

// quadtree.cpp:2477
float CQuadTree::GetGroundSlope(int index) const {
	if (index < 0)
		return 0.0f;

	WVector normal = m_pBaseModel->m_pTriangles[index].m_normal;
	if (m_pBaseModel->m_pTriangles[index].m_doubleSided && normal.y < 0.0f)
		normal.y = -normal.y;

	float angle = (float)acos(normal.y < -1.0f ? -1.0f : (normal.y > 1.0f ? 1.0f : normal.y));
	return angle > 0.0f ? angle : -angle;
}

// quadtree.cpp:2495
WVector CQuadTree::GetMeshNormal(int index) {
	if (index < 0)
		return GetMeshNormal(GolfBall().m_pos);

	WVector normal = m_pBaseModel->m_pTriangles[index].m_normal;
	if (m_pBaseModel->m_pTriangles[index].m_doubleSided && normal.y < 0.0f)
		normal.x = -normal.x, normal.y = -normal.y, normal.z = -normal.z;

	return normal;
}

// quadtree.cpp:2508
WVector CQuadTree::GetMeshNormal(const WVector &pos) {
	int index = -1;
	if (IsLocalContent((localContentType_t)0x56))
		index = GetMeshIndex(pos, false);

	if (index < 0)
		return WVector::ZERO;

	WVector normal = m_pBaseModel->m_pTriangles[index].m_normal;
	if (m_pBaseModel->m_pTriangles[index].m_doubleSided && normal.y < 0.0f)
		normal.x = -normal.x, normal.y = -normal.y, normal.z = -normal.z;

	return normal;
}

// quadtree.cpp:2526
WVector CQuadTree::GetMeshObjcet(const WVector &pos) {
	int index = GetObjIndex(pos, false);

	if (index < 0)
		return WVector::ZERO;

	short objIndex = m_objList[index];
	WVector normal = m_pPolySoup->m_models[objIndex]->m_basePos;
	if (m_pPolySoup->m_models[objIndex]->m_pTriangles[0].m_doubleSided && normal.y < 0.0f)
		normal.x = -normal.x, normal.y = -normal.y, normal.z = -normal.z;

	return normal;
}

// quadtree.cpp:2543
bool CQuadTree::PointInTriangle(const WVector &p, const WVector &a, const WVector &b, const WVector &c) const {
	return intersection::SameSide(p, a, b, c) && intersection::SameSide(p, b, a, c) && intersection::SameSide(p, c, a, b) ? true : false;
}

// quadtree.cpp:2548
bool CQuadTree::SameSide(const WVector &p1, const WVector &p2, const WVector &a, const WVector &b) const {
	return intersection::SameSide(p1, p2, a, b);
}

// quadtree.cpp:2553
float CQuadTree::IntersectPlane(const WVector &orig, const WVector &dir, const WPlane &plane, bool bothSides) {
	return intersection::IntersectPlane(orig, dir, plane, bothSides);
}

// quadtree.cpp:2631
int CQuadTree::GetTextureHandle(const WVector &pos) {
	int index = GetMeshIndex(pos, false);
	if (index < 0)
		return 0;

	return m_pBaseModel->m_pTriangles[index].m_texIndex;
}

// quadtree.cpp:2640
const char *CQuadTree::GetTextureName(const WVector &pos) {
	int index = GetMeshIndex(pos, false);
	if (index < 0)
		return 0;

	return FindTexture(m_pBaseModel->m_pTriangles[index].m_texIndex & 0x7FF);
}

// quadtree.cpp:2734
void CQuadTree::PreProcess(float elapsed, bool bForce) {
	if (bForce) {
		m_frame += 1;
	} else {
		float time = elapsed + m_frameTime;
		m_frameTime = time;
		if (time < 0.02f)
			return;
		float absElapsed = elapsed;
		if (elapsed < 0.0f)
			absElapsed = -elapsed;
		if (absElapsed < 0.00001f)
			return;

		unsigned long ticks = (unsigned long)(long)(time * 50.0f);
		m_frame += ticks;
		m_frameTime = time - (float)ticks * 0.02f;
	}

	if (Wind().HasSpecialWind()) {
		unsigned long phase = (m_frame / 50) % 170;
		if (IsLocalContent((localContentType_t)0x4F)) {
			if (WSingleton<CSharedDoc>::Instance()->m_playingMode != 2)
				Wind().SetSpecialWind(phase >= 80);
		} else {
			Wind().SetSpecialWind(phase >= 80);
		}

		if (!bForce) {
			if (Wind().IsActiveSpecialWind()) {
				if (Wind().m_specialStartFrame == 0)
					Wind().m_specialStartFrame = m_frame;
			} else if (Wind().m_specialStartFrame != 0) {
				Wind().m_specialStartFrame = 0;
			}
			Wind().GenerateFx(phase >= 60);
		}
	}

	for (unsigned int i = 0; i < m_aniObjs.size(); i++) {
		CAniObj *pAni = m_aniObjs[i];
		if (!pAni)
			continue;

		WPolySoup::sModel *pModel;
		if (m_pPolySoup->m_numObjects == m_pPolySoup->m_numStructuredObjects) {
			pModel = m_pPolySoup->m_models[pAni->m_objIndex];
		} else {
			pModel = 0;
			for (int j = 0; j < m_pPolySoup->m_numObjects; j++) {
				if (m_pPolySoup->m_models[j]->m_objIndex == pAni->m_objIndex) {
					pModel = m_pPolySoup->m_models[j];
					break;
				}
			}
		}
		if (!pModel || !pModel->m_pPuppet)
			continue;

		if (!bForce && pAni->m_active) {
			char sound[8];
			sprintf(sound, "sound%d", i);
			if (g_audio->IsPlaying(sound)) {
				float dx = pModel->m_matrix.pivot.x - data_00b7bfac.x;
				float dy = pModel->m_matrix.pivot.y - data_00b7bfac.y;
				float dz = pModel->m_matrix.pivot.z - data_00b7bfac.z;
				float dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
				float volume = 1.0f - dist / (pAni->m_speed * 3.2f);
				if (!(0.0f < volume))
					volume = 0.0f;
				g_audio->SetSFXVolumeWhilePlaying(sound, volume);
			} else {
				pAni->m_active = false;
			}
		}

		if (pModel->m_subOrder == 4 && bForce)
			continue;

		pAni->m_time = (float)(unsigned long)(pAni->m_rand + m_frame) * 0.02f;
		float aniTime = pAni->m_time;

		switch (pModel->m_subOrder) {
		case 2: {
			CAniObjScale *pScale = (CAniObjScale *)pAni;
			float angle;
			if (pModel->m_windFactor > 0.0f)
				angle = Wind().GetGlobalIntensity() * pScale->m_invSize * pModel->m_windFactor * pAni->m_time * 3.1415927f;
			else
				angle = aniTime * pModel->m_windFactor * -3.1415927f;
			pScale->m_angle = angle;
			pModel->m_pPuppet->ApplyBones(0, RotMat(pScale->m_angle, 2) * pModel->m_matrix);
			pModel->m_pPuppet->Transform(0, 1.0f, 0);
			continue;
		}
		case 1: {
			CAniObjMove *pMove = (CAniObjMove *)pAni;
			float period = pMove->m_standTime + pMove->m_moveTime;
			float cycles = (float)floor(aniTime / period);
			float local = pAni->m_time - cycles * period;
			int odd = (int)floor(pAni->m_time / (pMove->m_standTime + pMove->m_moveTime)) % 2;
			const WVector &from = odd ? pMove->m_endPos : pMove->m_startPos;
			const WVector &to = odd ? pMove->m_startPos : pMove->m_endPos;
			if (local > pMove->m_moveTime) {
				pModel->m_matrix.pivot = to;
			} else {
				WVector delta = to - from;
				float f = local / pMove->m_moveTime;
				pModel->m_matrix.pivot = delta * f + from;
			}
			break;
		}
		}

		WPuppet *pPuppet = pModel->m_pPuppet;
		if (pPuppet && pPuppet->m_aniLen > 0.0f) {
			float rem = pAni->m_time - (float)floor(pAni->m_time / pPuppet->m_aniLen) * pModel->m_pPuppet->m_aniLen;
			if (!(0.0f < rem))
				rem = 0.0f;
			if (!bForce)
				ScriptProcess(i, rem - elapsed, rem);
			pModel->m_pPuppet->ApplyBones(pModel->m_pPuppet->GetBones(0, rem, 0, 0, false), pModel->m_matrix);
		} else {
			pPuppet->ApplyBones(0, pModel->m_matrix);
		}
		pModel->m_pPuppet->Transform(0, 1.0f, 0);
	}
}


// quadtree.cpp:2904
void CQuadTree::ScriptProcess(int index, float from, float to) {
	CAniObj *pAni = m_aniObjs[index];
	WPolySoup::sModel *pModel = m_pPolySoup->m_models[pAni->m_objIndex];
	WPuppet *pPuppet = pModel->m_pPuppet;

	if (from == to)
		return;

	if (0.0f > from)
		from = 0.0f;
	from *= 30.0f;
	to *= 30.0f;

	float tolerance;
	if (pPuppet->m_framenum) {
		float step = (float)pPuppet->m_frame[1].num * (1.0f / 30.0f * 1.0f) - (float)pPuppet->m_frame[0].num * (1.0f / 30.0f * 1.0f);
		tolerance = (step > 0.0f) ? step : 0.0f;
	} else {
		tolerance = 0.3f;
	}

	int first = Ceil(from, tolerance);
	int last;
	if ((float)floor(to) == to)
		last = (int)to - 1;
	else
		last = (int)floor(to);

	if (first > last)
		return;

	int lo, hi;
	if (pPuppet->CheckFrameData(first, last, lo, hi)) {
		for (int f = lo; f <= hi; f++)
			ParsePuppetScript(index, pPuppet->m_frame[f].data);
	}
}

// quadtree.cpp:3351
bool CQuadTree::IsValid(const WVector &pos) {
	return true;
}

// quadtree.cpp:3995
void CQuadTree::CalcGreenPlane(void) {
	if (!WisEqual(m_greenBox.max, m_greenBox.min, 1e-5f)) {
		WVector delta = PLAYER(GOLFDOC()->m_myPlayerIndex)->m_ballPos - m_greenCenter;

		m_greenPlane.normal = WVector::UNIT_POS_Y + WVector(delta.x / (m_greenBox.max.x - m_greenBox.min.x), 0.0f, delta.z / (m_greenBox.max.z - m_greenBox.min.z)) * m_greenSlope;
		m_greenPlane.normal.Normalize();

		m_greenPlane.d = WDotProduct(m_greenPlane.normal * -1.0f, m_greenCenter);
	}
}

// quadtree.cpp:4008
float CQuadTree::GetGreenPos(const WVector &pos) {
	if (!WisEqual(m_greenBox.max, m_greenBox.min, 1e-5f) && GetGroundType(pos, true, 0) == 2) {
		// the height of the green's plane under pos
		return (m_greenPlane.c * pos.z + m_greenPlane.a * pos.x + m_greenPlane.d) / m_greenPlane.b * -1.0f;
	}

	return pos.y;
}

// quadtree.cpp:4066
void CQuadTree::SpinProcess(float elapsed) {
	CGolfBall &ball = GolfBall();

	ball.m_rotation[1] += elapsed * ball.m_lieAngle * 62.831856f;
	if (ball.m_preDataCount == ball.m_eventPreDataIndex[6] + 50) {
		ball.m_velocity += ball.m_spinImpulse;
		ball.m_eventPreDataIndex[7] = ball.m_preDataCount;
		ball.m_state = 3;

		RollProcess(elapsed);
	}
}

// quadtree.cpp:4133
void CQuadTree::RollingBallGroundResponse(CGolfBall &ball, float t, const WVector &dir, int index, const WVector &normal) {
	if (IsLocalContent((localContentType_t)0x6B)) {
		ball.m_shotCounters[0] = ball.m_shotCounters[1] = 0;
		ball.m_pos += dir * t;
		ball.m_pos.y = GetHighestGround(ball.m_pos, &ball.m_groundArea)
			+ ball.m_radius;
		ball.m_lastEventPreDataIndex = m_pBaseModel->m_pTriangles[index].m_propIndex;

		ReflectVector(normal, ball.m_velocity);
		float coeff = GetBounceCoeff(ball.m_lastEventPreDataIndex) * GetWeatherBounceCoeff(ball.m_lastEventPreDataIndex);
		ball.m_velocity *= coeff;
	} else {
		ball.m_shotCounters[1] = 0;
		ball.m_shotCounters[0] = 0;

		WVector move = dir * t;

		ball.m_pos.x += move.x;
		ball.m_pos.z += move.z;
		ball.m_pos.y = ball.GetRadius() + ball.m_pos.y + move.y;
		ball.m_lastEventPreDataIndex = m_pBaseModel->m_pTriangles[index].m_propIndex;
		ball.m_velocity *= GetBounceCoeff(ball.m_lastEventPreDataIndex) * GetWeatherBounceCoeff(ball.m_lastEventPreDataIndex) * -1.0f;

		strcpy(ball.PreDataToWrite(ball.m_preDataCount)->m_sound, "\xc3\xe6\xb5\xb9_wood");
	}
	ball.PreDataToWrite(ball.m_preDataCount)->m_contactVector = normal;
}

// quadtree.cpp:4641
CQuadTree::sColResponse::sColResponse(const CGolfBall &ball, const WVector &point, const WVector &normal)
	: m_ball(ball), m_point(point), m_normal(normal), m_flagB4(false), m_flagB5(false) {
	m_sound[0] = '\0';
}

// quadtree.cpp:4653
CQuadTree::sColResponse::GolfBallClone::GolfBallClone(const CGolfBall &ball)
	: m_pos(ball.m_pos), m_velocity(ball.m_velocity), m_lastContactId(ball.m_lastContactId),
	  m_lastContactIndex(ball.m_lastContactIndex), m_state(ball.m_state), m_pBall(&ball) {
}

// quadtree.cpp:4658
void CQuadTree::CommitResponse(CGolfBall &ball, const sColResponse &response, int index) {
	ball.m_pos = response.m_ball.m_pos;
	ball.m_velocity = response.m_ball.m_velocity;
	ball.m_lastContactId = response.m_ball.m_lastContactId;
	ball.m_lastContactIndex = response.m_ball.m_lastContactIndex;
	ball.m_state = response.m_ball.m_state;

	CGolfBall::sExpected *pData = ball.PreDataToWrite(index);
	pData->m_contactVector = response.m_contactVector;
	strcpy(pData->m_sound, response.m_sound);

	m_colData.m_point = response.m_point;
	m_colData.m_normal = response.m_normal;
	if (response.m_flagB5) {
		m_colList.push_back(m_colData);
	}
}

// quadtree.cpp:4678
void CQuadTree::Display(void) {
	WTVertex vtx[3];
	WTVertex *vl[4] = { &vtx[0], &vtx[1], &vtx[2], 0 };

	if (m_markedTriangle >= 0) {
		vtx[0].SetPosition(m_pBaseModel->m_pTriangles[m_markedTriangle].m_vertex[0]);
		vtx[0].diffuse = 0xFFFFFFFF;
		vtx[1].SetPosition(m_pBaseModel->m_pTriangles[m_markedTriangle].m_vertex[1]);
		vtx[1].diffuse = 0xFFFFFFFF;
		vtx[2].SetPosition(m_pBaseModel->m_pTriangles[m_markedTriangle].m_vertex[2]);
		vtx[2].diffuse = 0xFFFFFFFF;

		g_view->DrawPolygonFan(vl, 0x4300000, 0x400, false);
	}

	if (COption::Instance()->dIsVtxClrTestEnabled()) {
		TextOutA(g_view, 470.0f, 60.0f, m_vtxColorTestMode == 0 ? "<<BASE COLOR MODE>>" : "<<OBJ COLOR MODE>>");
		TextOutA(g_view, 470.0f, 72.0f, MakeStr("Base : %.0f %.0f %.0f", m_baseVtxColor.x, m_baseVtxColor.y, m_baseVtxColor.z));
		TextOutA(g_view, 470.0f, 84.0f, MakeStr("Obj  : %.0f %.0f %.0f", m_objVtxColor.x, m_objVtxColor.y, m_objVtxColor.z));
	}
}

// quadtree.cpp:4782
WVector CQuadTree::closestPointOnTriangle(const WVector &a, const WVector &b, const WVector &c, const WVector &p) const {
	return intersection::closestPointOnTriangle(a, b, c, p);
}

// quadtree.cpp:4787
WVector CQuadTree::closestPointOnLine(const WVector &a, const WVector &b, const WVector &p) const {
	return intersection::closestPointOnLine(a, b, p);
}

// quadtree.cpp:4792
float CQuadTree::intersectPolygon(const WVector &pos, const WVector &center, const WVector &dir) const {
	return intersection::intersectPolygon(pos, center, dir, GolfBall().GetRadius());
}

// quadtree.cpp:5127
float CQuadTree::GetWeatherRollCoeff(int property) const {
	if (GetWeather() >= 2 && GetWeather() <= 3) {
		switch (m_pPolySoup->GetProperty(property).m_weatherBounceType) {
		case 0:
			return 1.3f;
		case 3:
			return 1.5f;
		case 2:
			return 1.13f;
		case 7:
		case 8:
			break;
		default:
			return 1.8f;
		}
	}
	return 1.0f;
}

// quadtree.cpp:5164
float CQuadTree::GetWeatherBounceCoeff(int property) const {
	if (GetWeather() >= 2 && GetWeather() <= 3) {
		switch (m_pPolySoup->GetProperty(property).m_weatherBounceType) {
		case 0:
		case 1:
		case 4:
		case 5:
		case 6:
			return 0.8f;
		case 3:
			return 0.7f;
		case 2:
			return 0.9f;
		case 7:
		case 8:
			break;
		default:
			return 0.8f;
		}
	}
	return 1.0f;
}

// quadtree.cpp:5201
void CQuadTree::SetCondition(void) {
	switch (GetWeather()) {
	case 0:
	case 1:
		m_condition = 0.0f;
		break;
	case 2:
	case 3:
		switch (GolfBall().m_groundType) {
		case 0:
		case 1:
		case 3:
			m_condition = -2.0f;
			break;
		case 2:
			m_condition = 0.0f;
			break;
		case 4:
			m_condition = -3.0f;
			break;
		default:
			m_condition = -3.0f;
			break;
		}
		break;
	}
}

// quadtree.cpp:5237
float CQuadTree::GetRollCoeff(int property) const {
	return m_pPolySoup->GetProperty(property).m_rollCoeff;
}

// quadtree.cpp:5242
float CQuadTree::GetBounceCoeff(int property) const {
	return m_pPolySoup->GetProperty(property).m_bounceCoeff;
}

// quadtree.cpp:5247
unsigned char CQuadTree::GetGroundType(WVector pos, bool bWater, CGolfBall::eWater *pWater) {
	if (bWater && !GOLFDOC()->m_approachMode) {
		WVector tee = GOLFDOC()->GetHoleData().m_teePos;
		if (Wabs(pos.x - tee.x) < g_CM_TO_WU && Wabs(pos.z - GetTeeShotPos().z) < g_CM_TO_WU)
			return 0;
	}

	GetMeshList(pos);

	int index;
	pos = ColTest(pos, true, &index, true);
	if (index < 0)
		return 10;

	bool inWater = false;
	for (unsigned int i = 0; i < m_waterList.size(); i++) {
		sWater &water = m_waterList[i];

		if (pos.x > water.m_aabb.min.x && pos.x < water.m_aabb.max.x
			&& pos.z > water.m_aabb.min.z && pos.z < water.m_aabb.max.z
			&& pos.y < water.m_aabb.max.y) {
			if (pWater != 0)
				*pWater = (CGolfBall::eWater)water.m_index;

			inWater = true;
		}
	}

	if (inWater)
		return 10;

	return m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[index].m_propIndex).m_weatherBounceType;
}

// quadtree.cpp:5285
bool CQuadTree::IsOBArea(const WVector &pos) {
	int index = GetMeshIndex(pos, false);
	if (index < 0)
		return true;
	return m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[index].m_propIndex).m_outOfBounds;
}

// quadtree.cpp:5294
bool CQuadTree::IsPenetrable(int property) const {
	return m_pPolySoup->GetProperty(property).m_penetrable;
}

// quadtree.cpp:5299
float CQuadTree::GetMaxPowerOnGround(void) const {
	if (IsLocalContent((localContentType_t)0x2F)) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000028)
			return (m_pPolySoup->GetProperty(1)
				.m_maxPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
	}

	return (m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[GolfBall().m_groundArea].m_propIndex)
		.m_maxPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
}

// quadtree.cpp:5310
float CQuadTree::GetMinPowerOnGround(void) const {
	if (IsLocalContent((localContentType_t)0x2F)) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000028)
			return (m_pPolySoup->GetProperty(1)
				.m_minPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
	}

	return (m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[GolfBall().m_groundArea].m_propIndex)
		.m_minPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
}

// quadtree.cpp:5321
float CQuadTree::GetMaxPowerOnGround(const WVector &pos) {
	if (IsLocalContent((localContentType_t)0x2F)) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000028)
			return (m_pPolySoup->GetProperty(1)
				.m_maxPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
	}

	return (m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[GetMeshIndex(pos, false)].m_propIndex)
		.m_maxPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
}

// quadtree.cpp:5333
float CQuadTree::GetMinPowerOnGround(const WVector &pos) {
	if (IsLocalContent((localContentType_t)0x2F)) {
		if (PLAYER(GOLFDOC()->m_myPlayerIndex)->m_shotItemTypeId == 0x18000028)
			return (m_pPolySoup->GetProperty(1)
				.m_minPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
	}

	return (m_pPolySoup->GetProperty(m_pBaseModel->m_pTriangles[GetMeshIndex(pos, false)].m_propIndex)
		.m_minPowerOnGround[PLAYER(GOLFDOC()->m_myPlayerIndex)->m_powerShotLevel] + m_condition) * 0.01f;
}

// quadtree.cpp:5344
const char *CQuadTree::GetBounceSound(int property) const {
	return m_pPolySoup->GetProperty(property).m_name;
}

// quadtree.cpp:5349
const char *CQuadTree::GetRollSound(int property) const {
	return m_pPolySoup->GetProperty(property).m_desc;
}

// quadtree.cpp:5354
const char *CQuadTree::GetStepSound(int property) const {
	return m_pPolySoup->GetProperty(property).m_stepSound;
}

// quadtree.cpp:5359
const char *CQuadTree::GetStepSound(const WVector &pos) {
	return GetStepSound(m_pBaseModel->m_pTriangles[GetMeshIndex(pos, false)].m_propIndex);
}

// quadtree.cpp:5364
const char *CQuadTree::FindTexture(int index) {
	int texIndex = m_pPolySoup->m_texIndexMap[index];
	return m_pPolySoup->m_pTextures[texIndex].m_name;
}

// quadtree.cpp:5389
void CQuadTree::ResetWater(void) {
	m_waterList.clear();
	m_waterList.reserve(32);
}

// quadtree.cpp:5401
void CQuadTree::HoleIn(void) {
	CGolfBall &ball = GolfBall();
	CGolfBall::sExpected *pData = ball.PreDataToWrite(ball.m_preDataCount);

	ball.m_velocity.x = ball.m_velocity.y = ball.m_velocity.z = 0.0f;
	ball.m_state = 0;
	ball.SetHoleIn(1);
	// sExpected 0x60/0x61 are not named in golfball.h yet (header request pending)
	reinterpret_cast<unsigned char*>(pData)[0x60] = 3;
	reinterpret_cast<unsigned char*>(pData)[0x61] = 0;

	unsigned long ballId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[GOLFDOC()->m_myPlayerIndex].m_ballId;
	if (ballId == 0x14000005 || ballId == 0x14000080) {
		strcpy(pData->m_sound, "wb_holein");
	} else {
		strcpy(pData->m_sound, "\xb0\xf8_\xc8\xa6\xc0\xce");
	}
}

// quadtree.cpp:5421
WVector CQuadTree::GetCollisionPoint(const WVector &dir) {
	WVector point = GolfBall().m_pos - dir * GolfBall().GetRadius();
	return point;
}

// quadtree.cpp:5429
bool CQuadTree::OverlappedWithObject(WVector pos, float radius) {
	for (unsigned char i = 0; i < m_objNum; i++) {
		WPolySoup::sModel *pModel = m_pPolySoup->m_models[m_objList[i]];

		// "*nocol" objects do not collide.
		if (pModel->m_reservedByte & 1)
			continue;

		int count = 0;
		w_bound_box *pBox = pModel->m_pPuppet->m_bbList.Find("char0");

		if (pBox) {
			// The oriented boxes "char0", "char1", ... of the object, grown by the radius.
			while (pBox) {
				WMatrix m = pBox->bone->GetMatrix();
				const Waabb &aabb = pBox->info->aabb;

				float sx = WVector::ONE.x * radius;
				float sy = WVector::ONE.y * radius;
				float sz = WVector::ONE.z * radius;
				float hx = (aabb.max.x - aabb.min.x) * 0.5f + sx;
				float hy = (aabb.max.y - aabb.min.y) * 0.5f + sy;
				float hz = (aabb.max.z - aabb.min.z) * 0.5f + sz;

				m.xa *= hx;
				m.ya *= hy;
				m.za *= hz;

				Wobb obb;
				obb.center = pBox->info->spherePivot * pBox->bone->GetMatrix();
				obb.extend[0] = m.xa;
				obb.extend[1] = m.ya;
				obb.extend[2] = m.za;

				if (obb.IsInclude(pos))
					return true;

				count++;
				pBox = m_pPolySoup->m_models[m_objList[i]]->m_pPuppet->m_bbList.Find(MakeStr("char%d", count));
			}
		} else {
			if (pos.y > pModel->m_sceneAabb.max.y)
				continue;
			if (pModel->m_sceneAabb.min.y - 10.0f > pos.y)
				continue;

			float minX = pModel->m_sceneAabb.min.x - radius;
			float minZ = pModel->m_sceneAabb.min.z - radius;
			float maxX = pModel->m_sceneAabb.max.x + radius;
			float maxZ = pModel->m_sceneAabb.max.z + radius;

			if (pos.x > minX && pos.x < maxX && pos.z > minZ && pos.z < maxZ)
				return true;
		}
	}

	return false;
}

// quadtree.cpp:5539
void CQuadTree::SetBaseVtxColor(unsigned long color) {
	m_baseVtxColor.x = (float)((color >> 16) & 0xFF);
	m_baseVtxColor.y = (float)((color >> 8) & 0xFF);
	m_baseVtxColor.z = (float)(color & 0xFF);

	if (m_pBaseModel != 0) {
		// the base model's puppet takes the colour
		if (m_pBaseModel->m_pPuppet != 0) {
			m_pBaseModel->m_pPuppet->SetVtxColor(color);
			m_pBaseModel->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);
		}
	}
}

// quadtree.cpp:5555
void CQuadTree::SetObjVtxColor(unsigned long color) {
	m_objVtxColor.x = (float)((color >> 16) & 0xFF);
	m_objVtxColor.y = (float)((color >> 8) & 0xFF);
	m_objVtxColor.z = (float)(color & 0xFF);

	if (m_pPolySoup != 0) {
		for (int i = 0; i < m_pPolySoup->m_numObjects; i++) {
			// every object's puppet takes the colour
			if (m_pPolySoup->m_models[i]->m_pPuppet != 0) {
				m_pPolySoup->m_models[i]->m_pPuppet->SetVtxColor(color);
				m_pPolySoup->m_models[i]->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);
			}
		}
	}
}

// quadtree.cpp:5666
bool CQuadTree::SimpleColTest(const WVector &pos, const WVector &dir) {
	GetObjList(pos, dir);

	for (unsigned char i = 0; i < m_objNum; i++) {
		WPolySoup::sModel *pModel = m_pPolySoup->m_models[m_objList[i]];
		if (RayIntersectAABB(pModel->m_aabb, pos, dir, 0, 0) < 1.0f)
			return true;
	}

	return false;
}

// quadtree.cpp:5876
void CQuadTree::ClipDecalTriangle(const WPolySoup::sTriangle *pTri, WVector normal) {
	if (!pTri->m_doubleSided && WDotProduct(normal, pTri->m_normal) < 0.01f)
		return;

	WVector verts[4];   // room for the vertex a clip adds
	memcpy(verts, pTri->m_vertex, sizeof(pTri->m_vertex));
	ClipDecalPolygon(verts, 3, 0);
}

// A decal's clipped fragments, kept in a ring of 32 by DecalFragments.
struct sDecalFragments {
	bool m_used;                    // 0x0
	WVector m_box[4];               // 0x4, DecalFragments' first argument
	WVector m_normal;               // 0x34
	int m_numVertices;              // 0x40
	WVector m_vertices[1024];       // 0x44
	int m_numIndices;               // 0x3044
	unsigned short m_indices[1536]; // 0x3048
};
CHECK_SIZE(sDecalFragments, 0x3C48);
CHECK_FIELD(sDecalFragments, m_normal, 0x34);
CHECK_FIELD(sDecalFragments, m_numVertices, 0x40);
CHECK_FIELD(sDecalFragments, m_numIndices, 0x3044);
CHECK_FIELD(sDecalFragments, m_indices, 0x3048);

static int data_00af80b4;
static sDecalFragments data_00af80b8[32];

// quadtree.cpp:5947
static void SaveDecalFragments(int numVertices, const WVector *box, const WVector &normal, const WVector *vertices, int numIndices, const unsigned short *indices) {
	sDecalFragments *pFrag = &data_00af80b8[data_00af80b4++];
	if (data_00af80b4 == 32)
		data_00af80b4 = 0;

	pFrag->m_used = true;

	pFrag->m_box[0] = box[0];
	pFrag->m_box[1] = box[1];
	pFrag->m_box[2] = box[2];
	pFrag->m_box[3] = box[3];
	pFrag->m_normal = normal;

	pFrag->m_numVertices = numVertices; memcpy(pFrag->m_vertices, vertices, numVertices * sizeof(WVector));
	pFrag->m_numIndices = numIndices;
	memcpy(pFrag->m_indices, indices, numIndices * sizeof(unsigned short));
}

// quadtree.cpp:6121
void CQuadTree::DrawDecal(int texture, const WVector &pos, const WVector &normal, const WVector &tangent, float size, float alpha, enumPolyBlend blend, bool bAll) {
	static WVector vertices[1024];
	static unsigned short indices[1536];
	WTVertex tvs[1024];
	int numVerts;
	int numIndices;

	// the decal's frame: the unit normal, tangent and their cross product
	WVector n = normal;
	n.Normalize();
	WVector t = tangent;
	t.Normalize();
	WVector b(t.z * n.y - n.z * t.y, n.z * t.x - t.z * n.x, t.y * n.x - n.y * t.x);
	b.Normalize();

	// the box the decal covers
	WVector box[4];
	box[0] = pos - normal - tangent;
	box[1] = normal + pos - tangent;
	box[2] = normal + pos + tangent;
	box[3] = pos - normal + tangent;

	DecalFragments(box, b, vertices, &numVerts, 0x400, indices, &numIndices, 0x600, bAll);

	float lenN = (float)sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
	float lenT = (float)sqrt(tangent.x * tangent.x + tangent.y * tangent.y + tangent.z * tangent.z);
	float half = (lenT + lenN) * 0.5f;
	float scaleU = size / (lenN + lenN);
	float scaleV = size / (lenT + lenT);

	for (int i = 0; i < numVerts; i++) {
		tvs[i].x = vertices[i].x;
		tvs[i].y = vertices[i].y;
		tvs[i].z = vertices[i].z;

		float dx = vertices[i].x - pos.x;
		float dy = vertices[i].y - pos.y;
		float dz = vertices[i].z - pos.z;
		tvs[i].tu = (dz * n.z + dy * n.y + dx * n.x) * scaleU + 0.5f;
		tvs[i].tv = 0.5f - (dz * t.z + dy * t.y + dx * t.x) * scaleV;

		// fades out with the height above or below the decal's plane
		float fade = (float)fabs(pos.y - vertices[i].y) < half ? (float)fabs(pos.y - vertices[i].y) : half;
		tvs[i].diffuse = ((unsigned long)(unsigned char)(long)((half - fade) * (1.0f / half) * alpha * 255.0f) << 24) | 0xFFFFFF;
	}

	g_view->DrawIndexedTriangles(tvs, numVerts, indices, numIndices, (texture & 0x7FF) | blend | 0x22200000, 0x400);
}

// quadtree.cpp:6552
int CQuadTree::GetObjColNum(void) {
	int num = 0;
	for (std::vector<sColData>::iterator it = m_colList.begin(); it != m_colList.end(); ++it) {
		if ((*it).m_objIndex >= 0)
			num++;
	}
	return num;
}

// quadtree.cpp:6566
bool CQuadTree::IsFallenIntoWater(WVector pos, Waabb *pBox, bool bTest) {
	bool fallen = false;
	for (unsigned int i = 0; i < m_waterList.size(); i++) {
		sWater &water = m_waterList[i];

		// inside the water's box horizontally and below its surface
		if (pos.x > water.m_aabb.min.x && pos.x < water.m_aabb.max.x
			&& pos.z > water.m_aabb.min.z && pos.z < water.m_aabb.max.z
			&& pos.y < water.m_aabb.max.y) {
			if (pBox != 0)
				*pBox = water.m_aabb;

			if (!bTest)
				GolfBall().m_lie = water.m_index;

			fallen = true;
		}
	}

	return fallen;
}

// quadtree.cpp:6591
bool CQuadTree::IsOverWater(WVector pos) {
	GetMeshList(pos);

	for (unsigned int i = 0; i < m_waterList.size(); i++) {
		sWater &water = m_waterList[i];

		pos.y = (water.m_aabb.min.y + water.m_aabb.max.y) * 0.5f;

		// the ground under the water's middle is below its surface
		if (water.m_aabb.IsInclude(pos) && water.m_aabb.max.y > ColTest(pos, true, 0, true).y)
			return true;
	}

	return false;
}

// quadtree.cpp:6608
float CQuadTree::GetShadowIntensity(const WVector &pos) {
	if (!WSingleton<COption>::Instance()->vIsShadowmapEnabled() || !GOLFDOC()->m_day)
		return 1.0f;

	WVector lightDir = data_00aa5278;
	lightDir.y = 0.0f;
	lightDir.Normalize();
	lightDir *= -320.0f;

	GetObjList(pos, lightDir);

	unsigned char ambient = (unsigned char)(data_00aa5288 >> 17);
	float result = 1.0f;

	for (int i = 0; i < m_objNum; i++) {
		WPolySoup::sModel *pModel;
		if (m_pPolySoup->m_numObjects == m_pPolySoup->m_numStructuredObjects) {
			pModel = m_pPolySoup->m_models[m_objList[i]];
		} else {
			pModel = 0;
			for (int j = 0; j < m_pPolySoup->m_numObjects; j++) {
				if (m_pPolySoup->m_models[j]->m_objIndex == m_objList[i])
					pModel = m_pPolySoup->m_models[j];
			}
		}

		if (!pModel)
			continue;

		// only tall objects cast a shadow
		if (pModel->m_aabb.max.y - pModel->m_aabb.min.y < 16.0f)
			continue;

		// inside the object's box, in front of the light
		if (pModel->m_aabb.IsInclude(pos)) {
			WVector d = pos - pModel->m_basePos;
			if (d.x * lightDir.x + d.y * lightDir.y + d.z * lightDir.z < 0.0f)
				return (float)ambient;
		}

		WVector ext(pModel->m_aabb.max.x - pModel->m_basePos.x, pModel->m_aabb.max.y - pModel->m_basePos.y, pModel->m_aabb.max.z - pModel->m_basePos.z);
		float hx = ext.x * 0.75f;
		float hz = ext.z * 0.75f;
		float hy = ext.y * 0.5f;

		Waabb box;
		box.min.x = pModel->m_basePos.x - hx;
		box.min.y = pModel->m_basePos.y - ext.y;
		box.min.z = pModel->m_basePos.z - hz;
		box.max.x = hx + pModel->m_basePos.x;
		box.max.y = hy + pModel->m_basePos.y;
		box.max.z = hz + pModel->m_basePos.z;

		WVector ray(data_00aa5278.x * -320.0f, data_00aa5278.y * -320.0f, data_00aa5278.z * -320.0f);
		float t = RayIntersectAABBSolid(box, pos, ray, 0, 0.0f);
		if (t < result)
			result = t;
	}

	return result;
}

// quadtree.cpp:6678
// Marches the ray in steps of 1.6 until it falls below the ground, then
// returns the point the nearest triangle of the last step is hit at.
WVector CQuadTree::GetRayIntersection(WVector from, WVector dir) {
	dir.Normalize();
	dir *= 1.6f;

	float t = 1.0f;

	for (int step = 0; step < 1500; step++) {
		WVector next = from + dir;

		GetMeshList(next);
		int index = -1;
		WVector ground = ColTest(next, true, &index, true);
		if (index == -1) {
			ground.y = m_pBaseModel->m_aabb.min.y;
		}
		if (ground.y < m_pBaseModel->m_aabb.min.y) {
			ground.y = m_pBaseModel->m_aabb.min.y;
		}

		if (ground.y > next.y) {
			GetMeshList(from, dir);

			for (int i = 0; i < m_meshNum; i++) {
				float hit = intersection::IntersectRayTriangle(from, dir, m_pBaseModel->m_pTriangles[m_meshList[i]].m_vertex);
				if (hit <= t) {
					t = hit;
				}
			}

			if (t < 1.0f) {
				break;
			}
		}

		from = next;
	}

	WVector point = from + dir * t;
	GetMeshList(point);
	int dummy;
	WVector ground = ColTest(point, false, &dummy, true);
	if (ground.y < m_pBaseModel->m_aabb.min.y) {
		ground.y = m_pBaseModel->m_aabb.min.y;
	}

	return ground;
}

// quadtree.cpp:6939
float CQuadTree::IntersectRayTriangle(const WVector &orig, const WVector &dir, const WVector *tri) {
	return intersection::IntersectRayTriangle(orig, dir, tri);
}

// quadtree.cpp:6996
int CQuadTree::GetBounceCount(void) {
	int count = m_bounceCount;
	int lastFrame = -1;
	for (std::vector<sColData>::iterator it = m_colList.begin(); it != m_colList.end(); ++it) {
		if ((*it).m_frame - lastFrame > 5) {
			count++;
			lastFrame = (*it).m_frame;
		}
	}
	return count;
}

// quadtree.cpp:7018
float CQuadTree::GetBasePosY(WVector pos, float height) {
	for (unsigned char i = 0; i < m_waterList.size(); i++) {
		sWater &water = m_waterList[i];
		if (pos.x > water.m_aabb.min.x && pos.x < water.m_aabb.max.x
			&& pos.z > water.m_aabb.min.z && pos.z < water.m_aabb.max.z
			&& pos.y < water.m_aabb.max.y) {
			pos.y = water.m_aabb.max.y;
			break;
		}
	}

	if (m_lowestGroundMode && GolfBall().m_preDataCount > 0) {
		return GetLowestGround(pos) + height;
	}

	float t[2];
	t[0] = CheckCollision(pos, WVector(0.0f, 100.0f, 0.0f), 0, true);
	WVector top(pos.x, pos.y + 100.0f, pos.z);
	t[1] = CheckCollision(top, WVector(0.0f, -100.0f, 0.0f), 0, true);
	float fromTop = 1.0f - t[1];

	int hit = (t[0] < fromTop) ? 0 : 1;
	float y;
	if (t[hit] < 1.0f && hit != 0) {
		y = fromTop * 100.0f + pos.y + height;
	} else {
		float down = CheckCollision(pos, WVector(0.0f, -100.0f, 0.0f), 0, true);
		if (down < 1.0f) {
			y = pos.y - down * 100.0f + height;
		} else {
			y = pos.y;
		}
	}

	height += m_lowest;
	return Max(y, height);
}

// quadtree.cpp:7080
float CQuadTree::GetGravity(void) {
	return m_gravity * 34.295296f;
}

// quadtree.cpp:7085
void CQuadTree::GravityModifier(float gravity) {
	m_gravity = gravity;
}

// quadtree.cpp:7237
bool CQuadTree::TraceAABB_BackGround(const WVector &start, const WVector &end, const Waabb &box, trace_s *pTrace, WPolySoup::sModel *pModel) {
	if (!IsLocalContent((localContentType_t)0x56)) {
		return false;
	}

	if (WisEqual(start, end, 0.00001f)) {
		pTrace->m_endPos = end;
		return false;
	}

	pTrace->m_endPos = WVector(0.0f, 0.0f, 0.0f);
	pTrace->m_fraction = 0.0f;
	pTrace->m_normal = WVector(0.0f, 0.0f, 0.0f);
	pTrace->m_planeDist = 0.0f;
	pTrace->m_flag = false;

	WVector delta = end - start;

	// The base model's box, grown by one unit on every side.
	Waabb world = m_pBaseModel->m_aabb;
	world.min.x -= 1.0f;
	world.min.y -= 1.0f;
	world.min.z -= 1.0f;
	world.max += WVector::ONE;

	// The box moved along the horizontal part of the path.
	Waabb moved = box;
	moved.min.x += delta.x;
	moved.min.z += delta.z;
	moved.max.x += delta.x;
	moved.max.z += delta.z;

	bool inside = world.IsInclude(moved);
	pTrace->m_fraction = 1.0f;
	pTrace->m_flag = false;

	return inside;
}

// quadtree.cpp:7314
int CQuadTree::CheckCamera(void) {
	int result = -1;
	int index = 0;

	if (!IsLocalContent((localContentType_t)0x56))
		return -1;

	WPuppet *pPuppet = m_pPolySoup->m_models[0]->m_pPuppet;
	if (pPuppet != 0) {
		w_bound_box *pCenter = pPuppet->m_bbList.Find("center");

		if (pCenter != 0) {
			w_bound_box *pBox = pPuppet->m_bbList.Find("camera0");
			while (pBox != 0) {
				const WVector &vMin = pBox->info->aabb.min;
				WVector corners[4] = {
					vMin, vMin, vMin, vMin
				};
				const WVector &vMax = pBox->info->aabb.max;
				corners[0].y = vMax.y; corners[1].x = vMax.x; corners[1].y = vMax.y;
				corners[2].x = vMax.x;

				bool visible;
				for (int i = 0; i < 4; i++) {
					visible = g_view->InFrustum(corners[i]);
					if (visible) {
						result = index;
						break;
					}
				}
				if (visible)
					break;

				index++;
				pBox = m_pPolySoup->m_models[0]->m_pPuppet->m_bbList.Find(MakeStr("camera%d", index));
			}
		}
	}

	return result;
}

// quadtree.cpp:7374
void CQuadTree::SetAvatarPosition(const WVector &pos, float size) {
	if (IsLocalContent((localContentType_t)0x56)) {
		m_avatarBox.min.Reset(); m_avatarBox.max.Reset();

		m_avatarBox.min = pos;
		m_avatarBox.max = pos;

		for (int i = 0; i < 3; i++) {
			m_avatarBox.min.p[i] -= size;
			m_avatarBox.max.p[i] += size;
		}

		m_avatarBox.min.y = -2.0f;
		m_avatarBox.max.y = 2.0f;
	}
}

// quadtree.cpp:7808
bool CQuadTree::CheckPickRoom(WVector pos) {
	if (!IsLocalContent((localContentType_t)0x56))
		return false;

	Waabb box;

	if (m_pPolySoup->m_models[0]->m_pPuppet == 0)
		return false;

	w_bound_box *pBound = m_pPolySoup->m_models[0]->m_pPuppet->m_bbList.Find("center_window");
	if (pBound == 0)
		return false;

	box = pBound->info->aabb + m_pPolySoup->m_models[0]->m_pPuppet->GetMatrix().pivot;

	return box.IsInclude(pos);
}
