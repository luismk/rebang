// Original: d:\build\custom\temp\code\client\projectg\polysoup.cpp
#include "polysoup.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "ClientSetting.h"
#include "GolfBall.h"
#include "GolfDoc.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "contentsdoc.h"
#include "fx.h"
#include "fxsequence.h"
#include "fxspray.h"
#include "quadtree.h"
#include "slicestr.h"
#include "../shared/Localize.h"
#include "../shared/classdefine.h"
#include "wbone.h"
#include "westpak.h"
#include "wind.h"
#include "wlist.h"
#include "wpuppet.h"
#include "wresrcmng.h"
#include "wscene.h"
#include "wutil.h"
#include "wview.h"
#include "wxpuppet.h"

// ?g_lightset@@3ULightSet@@A; its header (wscene.h) does not declare it yet.
extern LightSet g_lightset;

// ?g_camera@@3VWMatrix@@A; no header declares it yet.
extern WMatrix g_camera;

// ?CloseCFile@@YAXPAVcFile@@@Z; no header declares it yet.
void __cdecl CloseCFile(cFile *file);

// polysoup.cpp:57
// The additional models SetAdditionModel loads with the puppet flag set
// (sub_006b8690). Emitted once SetAdditionModel, its only caller, is written.
static bool IsSpecialAdditionModel(const char *name) {
	static const char *s_names[] = {
		"un_sea.pet",
		"silv_dn.pet",
		"ice_unsea.pet",
		"panda_sea.pet",
	};

	for (int i = 0; i < sizeof(s_names) / sizeof(s_names[0]); i++) {
		if (_stricmp(name, s_names[i]) == 0) {
			return true;
		}
	}
	return false;
}

// polysoup.cpp:85
WPolySoup::WPolySoup(CGolfDoc *pDoc)
	: m_pTextures(0), m_numOldTextures(0), m_pOldTextures(0), m_pCameras(0),
	  m_pAreas(0), m_pPoints(0), m_pBgModel(0), m_bPendingOption(false), m_bUseTnL(false),
	  m_numSmapMeshes(0), m_pSmapDrawFlags(0), m_pSmapMeshes(0), m_bNoShadowmap(false),
	  m_holecupTexture(0) {
	m_pGolfDoc = pDoc;
	m_mapName[0] = 0;
	memset(m_numOtherRecords, 0, 0x28);
	m_numObjects = 0;

	m_numSoundBoxRefs = 0;
	m_defaultNear = 800.0f;
	m_defaultFar = 960.0f;

	m_models.clear();
	m_numStructuredObjects = 0;
	m_counters1.Reset();
	m_counters2.Reset();
}

// polysoup.cpp:111
// Releases everything the soup loaded: its models, cameras, points, areas,
// textures, background effects, shadow-map meshes and static puppet groups.
void WPolySoup::Clear(void) {
	memset(m_mapName, 0, sizeof(m_mapName));

	if (m_models.size() != 0) {
		for (int i = 0; i < m_numObjects; i++) {
			sModel *pModel = m_models[i];

			if (pModel->m_pPuppet) {
				g_resrcmng->Release(pModel->m_pPuppet, false);
				pModel->m_pPuppet = 0;
			}
			if (pModel->m_pQuads) {
				delete[] pModel->m_pQuads;
				pModel->m_pQuads = 0;
			}
			if (pModel->m_pTriangles) {
				delete[] pModel->m_pTriangles;
				pModel->m_pTriangles = 0;
			}
			delete pModel;
		}
	}

	if (m_pBgModel) {
		if (m_pBgModel->m_pTriangles) {
			delete[] m_pBgModel->m_pTriangles;
			m_pBgModel->m_pTriangles = 0;
		}
		if (m_pBgModel->m_pPuppet) {
			g_resrcmng->Release(m_pBgModel->m_pPuppet, true);
			m_pBgModel->m_pPuppet = 0;
		}
		delete m_pBgModel;
		m_pBgModel = 0;
	}

	BackupTextures();

	std::vector<sModel*>().swap(m_models);

	if (m_pCameras) {
		delete[] m_pCameras;
		m_pCameras = 0;
	}
	if (m_pPoints) {
		delete[] m_pPoints;
		m_pPoints = 0;
	}
	if (m_pAreas) {
		delete[] m_pAreas;
		m_pAreas = 0;
	}

	std::vector<CNodeContainer>().swap(m_boundAreas);
	m_texIndexMap.clear();

	m_lightSets.clear();
	std::vector<unsigned long>().swap(m_ulongList);
	m_soundBoxes.clear();

	memset(m_bgFx, 0, sizeof(m_bgFx));
	m_numBgFx = 0;

	for (int i = 0; i < m_numSmapMeshes; i++) {
		g_resrcmng->Release(m_pSmapDrawFlags[i]);
	}
	m_numSmapMeshes = 0;
	if (m_pSmapDrawFlags) {
		delete[] m_pSmapDrawFlags;
		m_pSmapDrawFlags = 0;
	}
	m_pSmapMeshes = 0;

	m_staticPetGrp.Clear();

	if (WSingleton<CSceneManager>::Instance()) {
		WSingleton<CSceneManager>::Instance()->ReleaseElement();
	}
	if (WSingleton<CFx>::Instance()) {
		WSingleton<CFx>::Instance()->ClearAll(false);
	}

	m_triArrays.clear();
}

// polysoup.cpp:234
void WPolySoup::BackupTextures(void) {
	ClearOldTextures();

	m_numOldTextures = m_numTextures;
	m_pOldTextures = m_pTextures;
	m_pTextures = 0;
}

// polysoup.cpp:246
void WPolySoup::ClearOldTextures(void) {
	for (int i = 0; i < m_numOldTextures; i++) {
		if (g_resrcmng && m_pOldTextures[i].m_handle) {
			g_resrcmng->Release(m_pOldTextures[i].m_handle);
			m_pOldTextures[i].m_handle = 0;
		}
	}
	if (m_pOldTextures) {
		delete[] m_pOldTextures;
		m_pOldTextures = 0;
	}
	m_numOldTextures = 0;
}

// polysoup.cpp:390
// qsort comparator of the models: by name, then by m_order, then by
// m_subOrder.
int __cdecl WPolySoup::CompareModel(const void *a, const void *b) {
	const sModel *pA = *(const sModel *const *)a;
	const sModel *pB = *(const sModel *const *)b;

	int result = _strcmpi(pA->m_name, pB->m_name);

	if (result != 0) {
		return result;
	}
	return pA->m_order < pB->m_order ? -1 : pA->m_subOrder > pB->m_subOrder;
}

// polysoup.cpp:408
// Whether a "*fx" record of the soup file places the silvia cannon sequence
// (sub_006ba430, called by Load). A record starts with its type byte (6).
static bool __stdcall IsSilviaCannonFx(const char *record) {
	if (*record != 6) {
		return false;
	}

	char name[0x40];
	WVector pos;
	WSliceStr slice = record + 1;

	slice.Get("*fx", "%s %f %f %f", name, &pos.x, &pos.y, &pos.z);

	return strcmpi(name, "silviacannon.seq") == 0;
}

// polysoup.cpp:468
// Loads the course's soup file: header, cameras, points, areas, textures,
// bound polygons, the background model with its collision triangles, the
// placed models, and the effects of the type-6 areas.
bool WPolySoup::Load(const char *name, bool bPlaceHoleCup) {
	int tnlSupported;
	g_resrcmng->VideoReference()->Command((wVDevMessage)0x20, 1, (intptr_t)&tnlSupported);

	if (tnlSupported) {
		m_bUseTnL = true;
	}
	Clear();

	strncpy(m_mapName, name, 0x3F);
	m_mapName[0x3F] = 0;

	IFF_STRUCT::sCourse *pCourse = WSingleton<CSharedDoc>::Instance()->m_pCurCourse;
	unsigned char courseKind = pCourse == 0 ? 0 : pCourse->courseFlags >> 4;

	cFile *pFile = GetResrcManager()->GetCFile(name, -1);
	if (pFile == 0) {
		MessageBoxA(0, MakeStr("\xB9\xE8\xB0\xE6 \xC6\xC4\xC0\xCF(%s)\xC0\xBB \xC0\xD0\xBE\xEE\xBF\xC0\xC1\xF6 \xB8\xF8\xC7\xDF\xBD\xC0\xB4\xCF\xB4\xD9.", name), "\xC6\xC4\xC0\xCF \xC0\xD0\xB1\xE2 \xBD\xC7\xC6\xD0", 0);
		return false;
	}

	pFile->Read(m_numOtherRecords, 0x28);

	if (m_numOtherRecords[1] != 0x71) {
		MessageBoxA(0, "\xB9\xE8\xB0\xE6 \xC6\xC4\xC0\xCF\xC0\xC7 \xB9\xF6\xC1\xAF\xC0\xCC \xB8\xC2\xC1\xF6 \xBE\xCA\xBD\xC0\xB4\xCF\xB4\xD9.", "\xB9\xE8\xB0\xE6\xC6\xC4\xC0\xCF \xB9\xF6\xC1\xAF", 0);
		return false;
	}

	switch (courseKind) {
	case 0:
		m_numObjects = m_numOtherRecords[2];
		break;
	case 1:
		m_numObjects = m_numOtherRecords[3] + m_numOtherRecords[2];
		break;
	case 2:
		m_numObjects = m_numOtherRecords[4] + m_numOtherRecords[2];
		break;
	default:
		m_numObjects = m_numOtherRecords[4] + m_numOtherRecords[3] + m_numOtherRecords[2];
		break;
	}

	m_pTextures = new sTexture[m_numTextures];

	sModel **ppModels = new sModel *[m_numObjects];
	for (int i = 0; i < m_numObjects; i++) {
		ppModels[i] = new sModel;
	}

	m_pPoints = new sPoint[m_numPoints];
	m_pAreas = new sArea[m_numAreas];
	m_pCameras = new sCamera[m_numCameras];

	m_pBgModel = new sModel;

	WVector vec;
	for (int i = 0; i < m_numCameras; i++) {
		sCamera *pCamera = &m_pCameras[i];

		pFile->Read(pCamera->m_name, 0x20);
		WVector at;
		pFile->Read(&at, 0xC);
		WVector pos;
		pFile->Read(&pos, 0xC);
		pCamera->m_matrix = MakeCameraMatrix(pos, at);

		pFile->Read(&pCamera->m_fov, 4);
		pFile->Read(&pCamera->m_param, 4);
	}

	g_lightset.type = LightSet::W_LIGHT_DIRECTIONAL;
	g_lightset.diffuse = 0xFFFFFF;
	g_lightset.ambient = 0x808080;
	g_lightset.ambient2 = 0xFFFFFF;

	for (int i = 0; i < m_numPoints; i++) {
		sPoint *pPoint = &m_pPoints[i];

		pFile->Read(&pPoint->m_type, 1);
		pFile->Read(pPoint->m_name, 0x20);
		pFile->Read(&vec, 0xC);
		pPoint->m_pos = vec;

		if (pPoint->m_type != 0) {
			char desc[0x40];
			pFile->Read(desc, 0x40);
			pPoint->m_desc = desc;
		}

		switch (pPoint->m_type) {
		case 1:
			sscanf(pPoint->m_desc.c_str(), "%x %x %x", &g_lightset.diffuse, &g_lightset.ambient, &g_lightset.ambient2);
			g_lightset.nearone = pPoint->m_pos;
			break;
		case 3:
			GOLFDOC()->m_day = false;
			break;
		}
	}

	g_lightset.diffuse = ModifyColor(g_lightset.diffuse, GOLFDOC()->m_fogBrightness);
	g_lightset.ambient = ModifyColor(g_lightset.ambient, GOLFDOC()->m_fogBrightness);
	g_lightset.ambient2 = ModifyColor(g_lightset.ambient2, GOLFDOC()->m_fogBrightness);

	for (int i = 0; i < m_numAreas; i++) {
		sArea *pArea = &m_pAreas[i];

		pFile->Read(&pArea->m_type, 1);
		pFile->Read(pArea->m_name, 0x40);
		pFile->Read(&vec, 0xC);
		pArea->m_aabb.min = vec;
		pFile->Read(&vec, 0xC);
		pArea->m_aabb.max = vec;

		if (pArea->m_type == 5) {
			sSoundBox soundBox;
			soundBox.m_elapsed = 0.0f;
			soundBox.m_range = 60.0f;
			soundBox.m_interval = -1.0f;
			sscanf(pArea->m_name, "%s %f %f", soundBox.m_name, &soundBox.m_range, &soundBox.m_interval);
			soundBox.m_box = pArea->m_aabb;
			m_soundBoxes.push_back(soundBox);
		}

		if (strstr(pArea->m_name, "*extra")) {
			memcpy(&m_counters1, &pArea->m_aabb, sizeof(Waabb));
		}
	}

	unsigned char curMap = GetCurMap() > 0x7F && GetCurMap() != 0xFD ? GetCurMap() - 0x80 : GetCurMap();
	IFF_STRUCT::sCourse *pMapCourse = ItemManager()->FindCourse(curMap | 0x28000000);

	CGolfDoc *pGolfDoc = GOLFDOC();
	if (pGolfDoc->m_currentHole != 0x13) {
		if (pMapCourse && pMapCourse->m_propertyFile[0]) {
			m_pGolfDoc->LoadXml(pMapCourse->m_propertyFile, pGolfDoc->m_currentHole - 1, false);
		} else {
			m_pGolfDoc->LoadXml(MakeStr("%s_property.xml", pMapCourse ? pMapCourse->m_mapName : 0), pGolfDoc->m_currentHole - 1, false);
		}
	}

	for (int i = 0; i < m_numTextures; i++) {
		pFile->Read(m_pTextures[i].m_name, 0x20);
		m_pTextures[i].m_handle = GetResrcManager()->LoadTexture(m_pTextures[i].m_name, 0, 0, 0);
		m_pTextures[i].m_param = m_pGolfDoc->m_propertyInfo.GetPropertyIndex(m_pTextures[i].m_name);
		m_texIndexMap[m_pTextures[i].m_handle] = i;
	}

	m_bAltCameraBounds = false;
	for (int i = 0; i < m_numBoundAreas; i++) {
		sBoundAreaRecord areaRecord;
		pFile->Read(&areaRecord, 0x18);

		CNodeContainer node(std::string(areaRecord.m_name));

		if (strstr(areaRecord.m_name, "outline")) {
			node.SetType((CNodeContainer::eType)1);
		} else if (strstr(areaRecord.m_name, "ib")) {
			node.SetType((CNodeContainer::eType)2);
		} else if (strstr(areaRecord.m_name, "ob")) {
			node.SetType((CNodeContainer::eType)3);
		} else if (strstr(areaRecord.m_name, "auto")) {
			node.SetType((CNodeContainer::eType)4);
		} else if (strstr(areaRecord.m_name, "beach")) {
			node.SetType((CNodeContainer::eType)5);
		} else if (strstr(areaRecord.m_name, "riverex")) {
			node.SetType((CNodeContainer::eType)8);
		} else if (strstr(areaRecord.m_name, "waterfall")) {
			node.SetType((CNodeContainer::eType)9);
		} else if (strstr(areaRecord.m_name, "river")) {
			node.SetType((CNodeContainer::eType)7);
		} else if (strstr(areaRecord.m_name, "lava")) {
			node.SetType((CNodeContainer::eType)0xD);
		} else if (strstr(areaRecord.m_name, "sea")) {
			node.SetType((CNodeContainer::eType)0xA);
		} else if (strstr(areaRecord.m_name, "wave")) {
			node.SetType((CNodeContainer::eType)0xB);
		} else if (strstr(areaRecord.m_name, "npc")) {
			node.SetType((CNodeContainer::eType)0xC);
		} else if (strstr(areaRecord.m_name, "camera")) {
			m_bAltCameraBounds = true;
			node.SetType((CNodeContainer::eType)6);
		}

		for (int j = 0; j < areaRecord.m_numPoints; j++) {
			pFile->Read(&vec, 0xC);
			WVector point = vec;
			node.Add(point);
		}
		m_boundAreas.push_back(node);
	}

	if (g_lightset.type == LightSet::W_LIGHT_DIRECTIONAL) {
		g_lightset.nearone.x *= -1.0f;
		g_lightset.nearone.y *= -1.0f;
		g_lightset.nearone.z *= -1.0f;
		g_lightset.nearone.Normalize();
	}

	sModelRecord record;
	record.m_flags = 0;
	pFile->Read(&record, 0xAC);

	m_pBgModel->m_subOrder = record.m_flags & 7;
	m_pBgModel->m_order = record.m_flags & 0xF8;
	m_pBgModel->m_aabb = record.m_aabb;
	m_pBgModel->m_matrix = record.m_matrix;
	m_pBgModel->m_basePos = (m_pBgModel->m_aabb.min + m_pBgModel->m_aabb.max) * 0.5f;
	m_pBgModel->m_center = (m_pBgModel->m_aabb.min + m_pBgModel->m_aabb.max) * 0.5f;
	m_pBgModel->m_radius = (m_pBgModel->m_aabb.max - m_pBgModel->m_aabb.min).Magnitude() * 0.5f;
	m_pBgModel->m_scale = sqrtf(record.m_matrix.xx * record.m_matrix.xx + record.m_matrix.zx * record.m_matrix.zx + record.m_matrix.yx * record.m_matrix.yx);
	strncpy(m_pBgModel->m_name, record.m_name, 0x1F);

	m_pBgModel->m_pPuppet = g_resrcmng->GetPuppet(m_pBgModel->m_name, false, false, false);
	m_pBgModel->m_pPuppet->m_lightmode = 0x1000000;
	m_pBgModel->m_pPuppet->ApplyBones(0, m_pBgModel->m_matrix);
	m_pBgModel->m_pPuppet->UpdateBound(true, false);
	m_pBgModel->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);

	m_pBgModel->m_pTriangles = new sTriangle[m_pBgModel->m_pPuppet->GetFaceNum()];

	WBone *pRoot = m_pBgModel->m_pPuppet->GetRootBone();
	bool bBaseMeshTest = WSingleton<COption>::Instance()->dIsBaseMeshTestEnabled();

	int triIndex = 0;
	for (w_mesh *pMesh = pRoot->GetMesh(); pMesh; pMesh = pMesh->next) {
		for (int i = 0; i < pMesh->indexNum; i += 3) {
			for (int j = 0; j < 3; j++) {
				unsigned short v = pMesh->indexList[i + j];
				unsigned long color;
				pFile->Read(&color, 4);

				pMesh->vtxColorList[v] = ModifyColor(color, GOLFDOC()->m_fogBrightness);
				if (color >> 24) {
					pMesh->vtxColorList[v] |= 0xFF000000;
				}
				m_pBgModel->m_pTriangles[triIndex].m_vertex[j] = pMesh->vecList[v] * pRoot->GetMatrix();
			}

			sTriangle *pTri = &m_pBgModel->m_pTriangles[triIndex];
			pTri->m_normal = WCrossProduct(pTri->m_vertex[2] - pTri->m_vertex[1], pTri->m_vertex[0] - pTri->m_vertex[1]);
			pTri->m_normal.Normalize();
			pTri->m_d = -pTri->m_normal.z * pTri->m_vertex[0].z + -pTri->m_normal.y * pTri->m_vertex[0].y + -pTri->m_normal.x * pTri->m_vertex[0].x;
			pTri->m_doubleSided = (pMesh->xiDrawFlag2 >> 10) & 1;
			pTri->m_propIndex = m_pTextures[m_texIndexMap[pMesh->texHandle]].m_param;
			pTri->m_texIndex = m_pTextures[m_texIndexMap[pMesh->texHandle]].m_handle;

			CGimmickContainer *pGimmick = 0;
			IContentsDataContainer *pContainer = WSingleton<CContentsDoc>::Instance()->GetContainer((localContentType_t)0xA3);
			if (pContainer) {
				pGimmick = (CGimmickContainer *)pContainer;
			}

			w_texlist *pTexList = g_resrcmng->FindTexture(pTri->m_texIndex);
			if (pGimmick->NeedToMakeTriPtArray(pTexList->texname, 0)) {
				std::map<int, std::vector<sTriangle *> >::iterator it = m_triArrays.find(pMesh->texHandle);
				if (it == m_triArrays.end()) {
					std::vector<sTriangle *> tris;
					tris.push_back(pTri);
					m_triArrays[pMesh->texHandle] = tris;
				} else {
					it->second.push_back(pTri);
				}
			}

			if (bBaseMeshTest) {
				if (pTri->m_normal.y < 0.0f) {
					*(unsigned short *)&pMesh->vtxColorList[pMesh->indexList[i]] = 0;
					*(unsigned short *)&pMesh->vtxColorList[pMesh->indexList[i + 1]] = 0;
					*(unsigned short *)&pMesh->vtxColorList[pMesh->indexList[i + 2]] = 0;
				} else if (fabsf(pTri->m_normal.y) < 0.00001f) {
					pMesh->vtxColorList[pMesh->indexList[i]] &= 0xFF0000FF;
					pMesh->vtxColorList[pMesh->indexList[i + 1]] &= 0xFF0000FF;
					pMesh->vtxColorList[pMesh->indexList[i + 2]] &= 0xFF0000FF;
				}
			}
			triIndex++;
		}
	}

	w_bound_box *pCenter = m_pBgModel->m_pPuppet->m_bbList.Find("center");
	if (pCenter && pCenter->info->option) {
		WSliceStr slice = pCenter->info->option;
		slice.Get("*wind", "%f", &m_pBgModel->m_windFactor);
	}

	if (m_pBgModel->m_pPuppet->m_bbList.Find("npc")) {
		m_ulongList.push_back(0xFFFF);
	}

	WSingleton<CSceneManager>::Instance()->RegisterElement((eObjType)0, m_pBgModel->m_pPuppet, 0, m_pBgModel->m_name, 3, 0);

	int numInstances = 0;
	int numRecords = m_numOtherRecords[4] + m_numOtherRecords[3] + m_numOtherRecords[2];
	sModel **ppNext = ppModels;
	for (int i = 0; i < numRecords; i++) {
		pFile->Read(&record, 0xAC);

		if (record.m_kind == 1 ? courseKind == 2 : record.m_kind == 2 && courseKind == 1) {
			continue;
		}

		sModel *pModel = *ppNext++;
		pModel->m_subOrder = record.m_flags & 7;
		pModel->m_order = record.m_flags & 0xF8;
		pModel->m_aabb = record.m_aabb;
		pModel->m_sceneAabb = record.m_sceneAabb;
		pModel->m_matrix = record.m_matrix;
		pModel->m_basePos = (pModel->m_aabb.min + pModel->m_aabb.max) * 0.5f;
		pModel->m_center = (pModel->m_aabb.min + pModel->m_aabb.max) * 0.5f;
		pModel->m_radius = (pModel->m_aabb.max - pModel->m_aabb.min).Magnitude() * 0.5f;
		pModel->m_scale = sqrtf(record.m_matrix.xx * record.m_matrix.xx + record.m_matrix.zx * record.m_matrix.zx + record.m_matrix.yx * record.m_matrix.yx);
		strncpy(pModel->m_name, record.m_name, 0x1F);
		strncpy(pModel->m_baseName, record.m_baseName, 0x1F);

		pModel->m_pPuppet = g_resrcmng->GetPuppet(pModel->m_name, IsSpecialAdditionModel(pModel->m_name), pModel->m_subOrder != 0, false);
		if (pModel->m_subOrder) {
			numInstances++;
		}

		if (pModel->m_pPuppet == 0) {
			if (m_pGolfDoc->m_ownedStrings.Find(pModel->m_name) == 0) {
				char *pName = new char[strlen(pModel->m_name) + 1];
				strcpy(pName, pModel->m_name);
				m_pGolfDoc->m_ownedStrings.AddItem(pName, pName, false);
			}
			continue;
		}

		pModel->m_pPuppet->ApplyBones(0, pModel->m_matrix);
		pModel->m_pPuppet->UpdateBound(true, false);
		pModel->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);

		ProcessPostLoadPet(pModel);

		unsigned long flags = 0;
		if (pModel->m_subOrder) {
			flags = 4;
		}
		if ((pModel->m_order & 0x10) || pModel->m_pPuppet->GetBoundSphere().radius > 128.0f) {
			flags |= 2;
		}
		if (pModel->m_order & 8) {
			flags |= 1;
		}
		WSingleton<CSceneManager>::Instance()->RegisterElement((eObjType)((pModel->m_subOrder != 0) + 2), pModel->m_pPuppet, 0, pModel->m_name, flags, &pModel->m_sceneAabb);
	}

	if (m_numObjects > 1) {
		qsort(ppModels, m_numObjects, sizeof(sModel*), CompareModel);
	}
	for (int i = 0; i < m_numObjects; i++) {
		m_models.push_back(ppModels[i]);
	}
	delete[] ppModels;

	GetPVS().Reserve(numInstances);

	m_numBgFx = 0;
	for (int i = 0; i < m_numAreas; i++) {
		sArea *pArea = &m_pAreas[i];

		if (IsSilviaCannonFx((const char *)pArea)) {
			continue;
		}
		if (pArea->m_type != 6) {
			continue;
		}

		Attacher attacher;
		char fxName[0x40];
		WVector angles;
		WSliceStr slice = pArea->m_name;
		slice.Get("*fx", "%s %f %f %f", fxName, &angles.x, &angles.y, &angles.z);

		if (bPlaceHoleCup) {
			if (GetCurMap() == 4 || GetCurMap() == 5) {
				if (_stricmp(fxName, "CL_Default.spr") == 0) {
					continue;
				}
			}
		}

		m_bgFx[m_numBgFx].m_pArea = pArea;
		m_bgFx[m_numBgFx].m_matrix.Reset();
		m_bgFx[m_numBgFx].m_matrix.Rotate(angles);
		m_bgFx[m_numBgFx].m_matrix.pivot = (pArea->m_aabb.min + pArea->m_aabb.max) * 0.5f;
		m_bgFx[m_numBgFx].m_offset.Reset();
		attacher.m_pMatrix[0] = &m_bgFx[m_numBgFx].m_matrix;
		attacher.m_pOffset[0] = &m_bgFx[m_numBgFx].m_offset;
		void *pFx = CFx::Instance()->Attach(attacher, fxName, false, 0);

		if (pFx == 0) {
			continue;
		}

		char *pExt = strrchr(fxName, '.');
		if (pExt == 0 || strlen(pExt) < 4) {
			continue;
		}
		pExt++;

		if (_stricmp(pExt, "spr") == 0) {
			((CFxSpray *)pFx)->SetLight(g_lightset);
			m_bgFx[m_numBgFx].m_pSpray = (CFxSpray *)pFx;
		} else if (_stricmp(pExt, "seq") == 0) {
			((CFxSequence *)pFx)->SetLight(g_lightset);
			m_bgFx[m_numBgFx].m_pSequence = (CFxSequence *)pFx;
		} else {
			continue;
		}

		m_numBgFx++;

		if (m_numBgFx >= 0x100) {
			break;
		}
	}

	m_bNoShadowmap = false;
	if (WSingleton<COption>::Instance()->vIsShadowmapEnabled()) {
		m_bNoShadowmap = !LoadShadowmapInfo();
	}

	CFx::Instance()->m_fxBox.m_groundAreaHigh = m_pBgModel->m_aabb.max.y;
	CFx::Instance()->m_fxBox.m_groundAreaLow = m_pBgModel->m_aabb.min.y;

	CloseCFile(pFile);
	return true;
}

// polysoup.cpp:1069
// Applies the options of a placed model's bound boxes: day/night-only
// bones, the center box's wind and flags, ambient sound boxes, "abv" bones
// and the "*fx" effects attached to the boxes.
void WPolySoup::ProcessPostLoadPet(sModel *pModel) {
	for (w_bound_box *pBox = pModel->m_pPuppet->m_bbList.Start(); pBox; pBox = pModel->m_pPuppet->m_bbList.Next()) {
		char *pOption = pBox->info->option;

		if (_stricmp(pBox->info->name, "night_only") == 0) {
			if (GOLFDOC()->m_day) {
				if (pBox->bone && pBox->bone->GetChild()) {
					pBox->bone->GetChild()->SetInvisible(true, true);
				}
			}
		} else if (_stricmp(pBox->info->name, "day_only") == 0) {
			if (!GOLFDOC()->m_day) {
				if (pBox->bone && pBox->bone->GetChild()) {
					pBox->bone->GetChild()->SetInvisible(true, true);
				}
			}
		} else if (_stricmp(pBox->info->name, "center") == 0) {
			WSliceStr slice = pOption;
			slice.Get("*wind", "%f", &pModel->m_windFactor);
			slice.Get("*nocol", "%d", &pModel->m_reservedByte);

			if (pOption) {
				if (strstr(pOption, "*nofog")) {
					pModel->m_pPuppet->DisableFog();
				}
				if (strstr(pOption, "*blend")) {
					pModel->m_pPuppet->EnableAlphaBlend();
				}
			}
		} else if (_stricmp(pBox->info->name, "npc") != 0) {
			if (strstr(pBox->info->name, "sound")) {
				sSoundBox soundBox;
				soundBox.m_interval = -1.0f;
				soundBox.m_range = 60.0f;
				soundBox.m_elapsed = 0.0f;
				sscanf(pOption, "%s %f %f", soundBox.m_name, &soundBox.m_range, &soundBox.m_interval);

				soundBox.m_box = pBox->bone->GetMatrix();
				soundBox.m_box *= (pBox->info->aabb.max - pBox->info->aabb.min) * 0.5f;
				soundBox.m_box.center = pBox->info->spherePivot * pBox->bone->GetMatrix();

				m_soundBoxes.push_back(soundBox);
			} else if (strstr(_strlwr(pBox->info->name), "abv")) {
				pBox->bone->HideBone(false);
			}
		}

		if (pOption == 0) {
			continue;
		}
		char *pFx = strstr(pOption, "*fx");
		if (pFx == 0) {
			continue;
		}

		Attacher unusedAttacher;
		char key[8];
		char fxName[0x20] = "";
		char kind[0x20] = "";
		sscanf(pFx, "%s %s %s", key, fxName, kind);

		if (pBox->bone == 0 || pBox->bone->IsInvisible()) {
			continue;
		}

		char *pExt = strrchr(fxName, '.');
		if (pExt == 0 || strlen(pExt) < 4) {
			continue;
		}
		pExt++;
		if (_stricmp(pExt, "spr") != 0 && _stricmp(pExt, "seq") != 0) {
			continue;
		}

		if (_strcmpi(kind, "tree") == 0) {
			ApplyFx(pModel, pBox->bone->GetChild(), fxName);
			continue;
		}

		Attacher attacher;
		attacher.m_pMatrix[0] = &pBox->bone->GetMatrix();
		attacher.m_pOffset[0] = &pBox->info->spherePivot;

		defines_t defines;
		strcpy(defines.m_names[0], "pet_scale");
		defines.m_values[0] = pModel->m_scale;
		defines.m_count = 1;

		if (_stricmp(pExt, "spr") == 0) {
			CFx::Instance()->Attach(attacher, fxName, false, &defines);
		} else {
			CFx::Instance()->Attach(attacher, fxName, false, &defines);
		}
	}
}

// polysoup.cpp:1185
bool WPolySoup::IsResisteredBgSequence(const std::string &name) {
	for (int i = 0; i < m_numBgFx; i++) {
		if (m_bgFx[i].m_pSequence) {
			if (m_bgFx[i].m_pSequence->m_name == name) {
				return true;
			}
		}
	}
	return false;
}

// polysoup.cpp:1198
void WPolySoup::ApplyFx(sModel *pModel, WBone *pBone, const char *name) {
	w_bound_box *pBox = pModel->m_pPuppet->m_bbList.Find(pBone->GetBoneName());

	if (pBox) {
		Attacher attacher;
		attacher.m_pMatrix[0] = &pBox->bone->GetMatrix();
		attacher.m_pOffset[0] = &pBox->info->spherePivot;

		defines_t defines;
		strcpy(defines.m_names[0], "pet_scale");
		defines.m_values[0] = pModel->m_scale;
		defines.m_count = 1;

		if (stricmp(name, ".spr") == 0) {
			CFx::Instance()->Attach(attacher, name, false, &defines);
		} else {
			CFx::Instance()->Attach(attacher, name, false, &defines);
		}
	}

	if (pBone->GetChild()) {
		ApplyFx(pModel, pBone->GetChild(), name);
	}
	if (pBone->GetNext()) {
		ApplyFx(pModel, pBone->GetNext(), name);
	}
}

// polysoup.cpp:1226
void WPolySoup::MakeDataStructure(void) {
	GetPVS().SetPolySoup(this);

	GetPVS().MakeBaseStructure(m_pBgModel, false);

	for (short i = 0; i < m_numObjects; i++) {
		GetPVS().MakeObjStructure(i, -1, false);
	}
	m_numStructuredObjects = m_numObjects;
}

// polysoup.cpp:1238
void WPolySoup::CreateDataModel(sModel *pModel) {
	if (IsLocalContent((localContentType_t)0x56)) {
		pModel->m_objIndex = m_numStructuredObjects;

		m_models.push_back(pModel);

		GetPVS().MakeObjStructure(pModel->m_objIndex, m_numObjects, false);

		m_numStructuredObjects += 1;

		m_numObjects += 1;
	}
}

// polysoup.cpp:1254
void WPolySoup::ChangeDataStructure(sModel *pModel) {
	int index = 0;

	if (IsLocalContent((localContentType_t)0x56)) {
		for (std::vector<sModel*>::iterator it = m_models.begin(); it != m_models.end(); ++it, index++) {
			if (_stricmp((*it)->m_name, pModel->m_name) == 0) {
				*it = pModel;

				GetPVS().MakeObjStructure(pModel->m_objIndex, index, true);

				for (int i = 0; i < m_numObjects; i++) {
					GetPVS().MakeObjStructure(m_models[i]->m_objIndex, i, true);
				}
				return;
			}
		}
		CreateDataModel(pModel);
	}
}

// polysoup.cpp:1307
// Removes a model (found by name) from the quadtree and from the soup.
bool WPolySoup::DeleteDataStructure(sModel *pModel) {
	int numObjects = m_numObjects;
	int index = 0;

	if (IsLocalContent((localContentType_t)0x56)) {
		std::vector<sModel*>::iterator it;
		for (it = m_models.begin(); it != m_models.end(); ++it, ++index) {
			if (stricmp((*it)->m_name, pModel->m_name) == 0) {
				break;
			}
		}
		if (it != m_models.end()) {
			(*it)->m_pPuppet = 0;

			GetPVS().MakeBaseStructure(m_pBgModel, true);

			GetPVS().MakeObjStructure((*it)->m_objIndex, index, true);

			for (int i = 0; i < numObjects; i++) {
				GetPVS().MakeObjStructure(m_models[i]->m_objIndex, i, true);
			}

			for (it = m_models.begin(); it != m_models.end(); ++it) {
				if (stricmp((*it)->m_name, pModel->m_name) == 0) {
					break;
				}
			}
			if (it != m_models.end()) {
				if ((*it)->m_pTriangles != 0) {
					delete[] (*it)->m_pTriangles;
					(*it)->m_pTriangles = 0;
				}
				if ((*it)->m_pQuads != 0) {
					delete[] (*it)->m_pQuads;
					(*it)->m_pQuads = 0;
				}

				m_models.erase(it);

				m_numObjects--;
			}
			return true;
		}
	}
	return false;
}

// polysoup.cpp:1372
const sProperty &WPolySoup::GetProperty(int index) {
	return m_pGolfDoc->m_propertyInfo.GetProperty(index);
}

// polysoup.cpp:1377
int WPolySoup::GetPropertyIndex(const char *name) {
	return m_pGolfDoc->m_propertyInfo.GetPropertyIndex(name);
}

// polysoup.cpp:1382
void WPolySoup::EndLoad(bool bPlaceHoleCup) {
	ClearOldTextures();

	if (bPlaceHoleCup) {
		PlaceHoleCup();
	}
	xBuildTnLBuffers();
}

// polysoup.cpp:1395
void WPolySoup::ChangeFloorTexture(const char *oldName, const char *newName) {
	if (!IsLocalContent((localContentType_t)0x56)) {
		return;
	}

	if (oldName != 0 && newName != 0) {
		int oldHandle = g_resrcmng->FindTexture(oldName)->texhandle;
		int newHandle = g_resrcmng->LoadTexture(newName, 0, 0, 0);

		if (m_pBgModel->m_pPuppet->GetRootBone()->GetMesh() != 0) {
			m_pBgModel->m_pPuppet->ChangeTextureByHandle(newHandle, oldHandle, m_pBgModel->m_pPuppet);
		}
	}
}

// polysoup.cpp:1433
void WPolySoup::ChangeObjectTexture(const char *oldName, const char *newName, int index) {
	if (!IsLocalContent((localContentType_t)0x56)) {
		return;
	}

	if (oldName != 0 && newName != 0) {
		if (index == -1) {
			int oldHandle = g_resrcmng->FindTexture(oldName)->texhandle;
			int newHandle = g_resrcmng->LoadTexture(newName, 0, 0, 0);
			w_mesh *pMesh = m_models[0]->m_pPuppet->GetRootBone()->GetMesh();

			if (pMesh == 0) {
				return;
			}
			pMesh = pMesh->next;
			if (pMesh == 0) {
				return;
			}
			m_models[0]->m_pPuppet->ChangeTextureByHandle(newHandle, oldHandle, m_models[0]->m_pPuppet);
		} else {
			int modelIndex = index;

			for (int i = 0; i < m_numObjects; i++) {
				if (m_models[i]->m_objIndex == index) {
					modelIndex = i;
					break;
				}
			}

			int oldHandle = g_resrcmng->FindTexture(oldName)->texhandle;
			int newHandle = g_resrcmng->LoadTexture(newName, 0, 0, 0);
			w_mesh *pMesh = m_models[modelIndex]->m_pPuppet->GetRootBone()->GetMesh();

			if (pMesh == 0) {
				return;
			}
			pMesh = pMesh->next;
			m_models[modelIndex]->m_pPuppet->ChangeTextureByHandle(newHandle, oldHandle, m_models[modelIndex]->m_pPuppet);
		}
	}
}

// polysoup.cpp:1527
void WPolySoup::xBuildTnLBuffers(void) {
	if (m_bUseTnL) {
		m_pBgModel->m_pPuppet->xUpdateTnLBuffers(true);
		m_staticPetGrp.Build(m_models, m_numObjects);
	}
}

// polysoup.cpp:1536
// How opaque a model stays in front of the camera: 255 unless the camera is
// close to its box relative to its size (the ratio is clamped to [0.25, 1]).
unsigned char WPolySoup::OverlapCamera(int index) {
	if (m_models[index]->m_order & 8) {
		return 0xFF;
	}
	Waabb box = m_models[index]->m_aabb;

	float dx = box.max.x - box.min.x;
	float dy = box.max.y - box.min.y;
	float dz = box.max.z - box.min.z;
	float radius = sqrtf(dz * dz + dy * dy + dx * dx) * 0.5f;
	float maxRadius = radius > 96.0f ? 96.0f : radius;
	float distSq = 0.0f;

	for (int i = 0; i < 3; i++) {
		float d;

		if (g_camera.pivot.p[i] < box.min.p[i]) {
			d = g_camera.pivot.p[i] - box.min.p[i];
			distSq += d * d;
		} else if (g_camera.pivot.p[i] > box.max.p[i]) {
			d = g_camera.pivot.p[i] - box.max.p[i];
			distSq += d * d;
		}
	}

	float ratio = sqrtf(distSq) / maxRadius;
	return (unsigned char)((ratio < 0.25f ? 0.25f : (ratio > 1.0f ? 1.0f : ratio)) * 255.0f);
}

// polysoup.cpp:1579
// Whether pos (XZ) lies inside the polygon, by its winding number counted in
// quadrant steps (4 = once around).
bool WPolySoup::IsPointInPoly(const std::vector<WVector> &points, const WVector &pos) {
	int num = points.size();
	if (num < 3) {
		return false;
	}

	int winding = 0;

	for (int i = 0; i < num; i++) {
		WVector v1 = points[i];
		WVector v2 = points[(i + 1) % num];
		int q1 = v1.x > pos.x ? (v1.z > pos.z ? 1 : 4) : (v1.z > pos.z ? 2 : 3);
		int q2 = v2.x > pos.x ? (v2.z > pos.z ? 1 : 4) : (v2.z > pos.z ? 2 : 3);
		int delta = q2 - q1;

		switch (delta) {
		case 2:
		case -2:
			if (v2.x - (v1.x - v2.x) * (v2.z - pos.z) / (v1.z - v2.z) > pos.x) {
				delta = -delta;
			}
			break;
		case 3:
			delta = -1;
			break;
		case -3:
			delta = 1;
			break;
		}
		winding += delta;
	}

	return (winding > 0 ? winding : -winding) == 4;
}

// polysoup.cpp:1616
bool WPolySoup::IsOutOfBound(const WVector &pos) {
	for (std::vector<CNodeContainer>::iterator it = m_boundAreas.begin(); it != m_boundAreas.end(); ++it) {
		int type = it->m_type;

		if (type == 3 || type == 1) {
			if (!IsPointInPoly(it->m_points, pos)) {
				return true;
			}
		} else if (type == 2) {
			if (IsPointInPoly(it->m_points, pos)) {
				return true;
			}
		}
	}

	return GetPVS().IsOBArea(pos) ? true : false;
}

// polysoup.cpp:1641
WVector WPolySoup::GetCameraPos(const WVector &pos, const WVector &dir) {
	WVector result = pos;

	for (std::vector<CNodeContainer>::iterator it = m_boundAreas.begin(); it != m_boundAreas.end(); ++it) {
		int type = it->m_type;

		if (m_bAltCameraBounds ? type == 6 : type == 1) {
			if (!IsPointInPoly(it->m_points, pos)) {
				result = PosInWorld(it->m_points, pos, dir);
			}
			break;
		}
	}

	return result;
}

// polysoup.cpp:1739
// The radius of the circle through the camera's corner on the XZ plane: a
// and b are the sides from the corner, c the direction between them.
float WPolySoup::GetCameraRadius(WVector a, WVector b, WVector c) {
	a.y = 0.0f;
	b.y = 0.0f;
	c.y = 0.0f;

	float lenA = sqrtf(a.x * a.x + a.z * a.z);
	float lenB = sqrtf(b.x * b.x + b.z * b.z);
	a.Normalize();
	b.Normalize();
	c.Normalize();

	float cosA = a.x * c.x + a.z * c.z + a.y * c.y;
	if (cosA < -1.0f)
		cosA = -1.0f;
	else if (cosA > 1.0f)
		cosA = 1.0f;
	float angleA = acosf(cosA);
	float cosB = b.x * c.x + b.z * c.z + b.y * c.y;
	if (cosB < -1.0f)
		cosB = -1.0f;
	else if (cosB > 1.0f)
		cosB = 1.0f;
	float angleB = acosf(cosB);

	float sinSum = sinf(angleA + angleB);

	return sinSum * lenB * lenA / (sinf(angleA) * lenA + sinf(angleB) * lenB);
}

// polysoup.cpp:1766
bool WPolySoup::IsClockWise(const std::vector<WVector> &points, const WVector &pos) {
	WVector2D a(points[1].x - pos.x, points[1].z - pos.z), b(points[0].x - pos.x, points[0].z - pos.z);

	if (a.x * b.y - b.x * a.y > 0.0f) {
		return true;
	}
	return false;
}

// polysoup.cpp:1776
WPolySoup::sCamera *WPolySoup::FindCamera(const char *name) {
	if (name == 0 || name[0] == '\0') {
		return 0;
	}
	for (int i = 0; i < m_numCameras; i++) {
		if (_strcmpi(m_pCameras[i].m_name, name) == 0) {
			return &m_pCameras[i];
		}
	}
	return 0;
}

// polysoup.cpp:1790
WPolySoup::sPoint *WPolySoup::FindPoint(const char *name) {
	if (name == 0 || name[0] == '\0') {
		return 0;
	}
	for (int i = 0; i < m_numPoints; i++) {
		if (_strcmpi(m_pPoints[i].m_name, name) == 0) {
			return &m_pPoints[i];
		}
	}
	return 0;
}

// polysoup.cpp:1804
WPolySoup::sArea *WPolySoup::FindArea(const char *name) {
	if (name == 0 || name[0] == '\0') {
		return 0;
	}
	for (int i = 0; i < m_numAreas; i++) {
		if (strstr(m_pAreas[i].m_name, name) != 0) {
			return &m_pAreas[i];
		}
	}
	return 0;
}

// polysoup.cpp:1818
WPolySoup::sModel *WPolySoup::FindModel(const char *name) {
	if (name == 0 || name[0] == '\0') {
		return m_pBgModel;
	}
	for (int i = 0; i < m_numObjects; i++) {
		if (_strcmpi(m_models[i]->m_name, name) == 0) {
			return m_models[i];
		}
	}
	return 0;
}

// polysoup.cpp:1837
CNodeContainer *WPolySoup::FindNode(const char *name) {
	if (name && *name) {
		for (std::vector<CNodeContainer>::iterator it = m_boundAreas.begin(); it != m_boundAreas.end(); ++it) {
			if (strcmpi(it->GetName().c_str(), name) == 0) {
				return &*it;
			}
		}
	}

	return 0;
}

// polysoup.cpp:1852
WPolySoup::sArea *WPolySoup::GetAreaByPos(const WVector &pos) {
	for (int i = 0; i < m_numAreas; i++) {
		if (strstr(m_pAreas[i].m_name, "*cam") != 0) {
			if (m_pAreas[i].m_aabb.IsInclude(pos)) {
				return &m_pAreas[i];
			}
		}
	}

	return 0;
}

// polysoup.cpp:1867
WPolySoup::sCamera *WPolySoup::GetCameraByName(const char *name) {
	for (int i = 0; i < m_numCameras; i++) {
		if (_stricmp(m_pCameras[i].m_name, name) == 0) {
			return &m_pCameras[i];
		}
	}
	return 0;
}

// polysoup.cpp:1879
w_bound_box *WPolySoup::GetObjBoundBox(int index, const char *name) {
	if (index == 0xFFFF) {
		return m_pBgModel->m_pPuppet->m_bbList.Find(name);
	}
	return m_models[index]->m_pPuppet->m_bbList.Find(name);
}

// polysoup.cpp:1892
int WPolySoup::GetObjAlpha(int index) {
	if (index == 0xFFFF) {
		return 0xFF;
	}
	return m_models[index]->m_alpha;
}

// polysoup.cpp:1905
float WPolySoup::GetObjWindFactor(int index) {
	if (index == 0xFFFF) {
		return m_pBgModel->m_windFactor;
	}
	return m_models[index]->m_windFactor;
}

// polysoup.cpp:1919
void WPolySoup::UpdateBgFxWind(void) {
	CFx::Instance()->UpdateExtWind(Wind().GetGlobalWind());

	for (int i = 0; i < m_numBgFx; i++) {
		if (m_bgFx[i].m_pSpray) {
			m_bgFx[i].m_pSpray->SetWind(Wind().GetWind(m_bgFx[i].m_pSpray->m_pos));
		}
		if (m_bgFx[i].m_pSequence) {
			if (CFx::Instance()->FindSequence(m_bgFx[i].m_pSequence)) {
				m_bgFx[i].m_pSequence->SetWind(Wind().GetWind(m_bgFx[i].m_pSequence->Pos()));
			}
		}
	}
}

// polysoup.cpp:2075
void WPolySoup::SetRenderMode(int mode) {
	for (int i = 0; i < m_numObjects; i++) {
		sModel *pModel = m_models[i];

		if (pModel->m_pPuppet) {
			pModel->m_pPuppet->SetRenderMode(mode);
		}
	}
	m_pBgModel->m_pPuppet->SetRenderMode(mode);
}

// direct.h is not in the toolchain's include directory; the CRT export.
extern "C" __declspec(dllimport) int __cdecl mkdir(const char *dirname);

// polysoup.cpp:2095
// Writes the face-count distribution of the course's objects to
// statistics/<map>_<hole>_stat.txt.
void WPolySoup::MakeStatistics(void) {
	mkdir("statistics");
	FILE *fp = fopen(MakeStr("statistics/%s_%02d_stat.txt", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0, GOLFDOC()->m_currentHole), "wt");

	if (fp == 0)
		return;

	int total = 0;
	int maxFaces = 0;
	int minFaces = 1000;
	int counts[11];

	memset(counts, 0, sizeof(counts));

	int i;
	for (i = 0; i < m_numObjects; i++) {
		int faces = m_models[i]->m_pPuppet->GetFaceNum();
		int slot = faces / 100;
		if (slot > 10)
			slot = 10;
		counts[slot]++;

		total += m_models[i]->m_pPuppet->GetFaceNum();
		if (m_models[i]->m_pPuppet->GetFaceNum() >= maxFaces) maxFaces = m_models[i]->m_pPuppet->GetFaceNum();
		if (faces <= minFaces) minFaces = faces;
	}

	fprintf(fp, "*[%s_%02d]\xC0\xC7 \xBF\xC0\xBA\xEA\xC1\xA7\xC6\xAE \xB8\xDE\xBD\xAC \xBA\xD0\xC6\xF7\xB5\xB5\n\n", CSharedDoc::Instance()->m_pCurCourse ? CSharedDoc::Instance()->m_pCurCourse->m_mapName : 0, GOLFDOC()->m_currentHole);

	for (i = 0; i < 11; i++) {
		if (i == 10) {
			fprintf(fp, "%3d~    : ", 1000);
		} else {
			fprintf(fp, "%3d~%3d : ", i * 100, i * 100 + 100);
		}

		for (int j = 0; j < counts[i]; j++)
			fprintf(fp, "*");

		fprintf(fp, "(%d)\n", counts[i]);
	}

	int average = total / m_numObjects;

	int sumSquares = 0;

	for (i = 0; i < m_numObjects; i++) {
		int faces = m_models[i]->m_pPuppet->GetFaceNum();
		sumSquares += faces * faces;
	}

	int variance = (sumSquares - m_numObjects * average * average) / (m_numObjects - 1);

	fprintf(fp, "\n\xC7\xD5\xB0\xE8   : %d(%d)\n", total + m_pBgModel->m_pPuppet->GetFaceNum(), m_numObjects);
	fprintf(fp, "\xC6\xF2\xB1\xD5\xB0\xAA : %d\n", average);
	fprintf(fp, "\xC3\xD6\xBC\xD2\xB0\xAA : %d\n", minFaces);
	fprintf(fp, "\xC3\xD6\xB4\xEB\xB0\xAA : %d\n", maxFaces);
	fprintf(fp, "\xC7\xA5\xC1\xD8\xC6\xED\xC2\xF7 : %.0f\n", sqrt((double)variance));
	fprintf(fp, "\xC5\xD8\xBD\xBA\xC3\xC4\xB0\xB9\xBC\xF6 : %d\n", m_numTextures);

	fclose(fp);
}

// polysoup.cpp:2166
// Interpolates a value over a triangle from its barycentric weights
// (sub_006b8aa0). The original has no reference to it; cl drops it until a
// caller is found.
static float InterpolateTriangle(float v0, float v1, float v2, float u, float v) {
	float value = (v1 - v0) * u + (v2 - v0) * v;

	return value + v0;
}

// polysoup.cpp:2175
// The barycentric weights of pos in the triangle (a, b, c), on the XZ plane
// (sub_006ba820). The original has no reference to it; cl drops it until a
// caller is found.
static void GetBarycentric(const WVector &a, const WVector &b, const WVector &c, const WVector &pos, float &u, float &v) {
	float ax = a.x;
	float az = a.z;
	ax -= pos.x;
	az -= pos.z;
	float bx = b.x - pos.x;
	float bz = b.z - pos.z;
	float cx = c.x - pos.x;
	float cz = c.z - pos.z;

	u = (ax * cz - cx * az) / (bx * cz - cx * bz);
	v = (bx * az - bz * ax) / (bx * cz - cx * bz);
}

// polysoup.cpp:2193
// The barycentric weights of pos in the triangle (a, b, c), projected on the
// plane the triangle's normal is closest to (sub_006ba880, register
// arguments). u weighs c, v weighs b. Taking c before b reproduces the
// original's registers (ecx a, edx b, esi c, edi pos). Emitted once its callers
// smBuildOutFragMeshes and smAddSmapPack are written.
static void GetBarycentricProjected(const WVector &a, const WVector &c, const WVector &b, const WVector &pos, float &u, float &v) {
	WVector normal;
	normal = WCrossProduct(c - a, b - a);
	float px, py, cx, cy, bx, by;

	float absX = Abs(normal.x);
	float absY = Abs(normal.y);
	float absZ = Abs(normal.z);

	if (absX >= absY && absX >= absZ) {
		px = pos.z;
		py = pos.y;
		px -= a.z;
		py -= a.y;
		cx = c.z - a.z;
		cy = c.y - a.y;
		bx = b.z - a.z;
		by = b.y - a.y;
	} else if (absY >= absX && absY >= absZ) {
		px = pos.x;
		py = pos.z;
		px -= a.x;
		py -= a.z;
		cx = c.x - a.x;
		cy = c.z - a.z;
		bx = b.x - a.x;
		by = b.z - a.z;
	} else {
		px = pos.x;
		py = pos.y;
		px -= a.x;
		py -= a.y;
		cx = c.x - a.x;
		cy = c.y - a.y;
		bx = b.x - a.x;
		by = b.y - a.y;
	}

	u = (by * px - bx * py) / (by * cx - bx * cy);
	v = (cx * py - cy * px) / (by * cx - bx * cy);
}

// polysoup.cpp:2248
// Loads the shadow map of the background (<map>.sbin): the file must match
// the background's meshes (their count, vertex and index counts).
bool WPolySoup::LoadShadowmapInfo(void) {
	char name[0x40] = { 0 };

	char *pExt = strrchr(m_mapName, '.');
	if (pExt == 0 || pExt - m_mapName >= 0x3A) {
		return false;
	}
	strncpy(name, m_mapName, pExt - m_mapName);
	strcat(name, ".sbin");

	cFile *pFile = GetResrcManager()->GetCFile(name, -1);
	if (pFile == 0) {
		return false;
	}

	w_mesh *pFirst = m_pBgModel->m_pPuppet->GetRootBone()->GetMesh();
	w_mesh *pMesh;
	int numMeshes = 0;
	for (pMesh = pFirst; pMesh; pMesh = pMesh->next) {
		numMeshes++;
	}

	w_mesh **ppMeshes = new w_mesh*[numMeshes];

	int i = 0;
	for (pMesh = pFirst; pMesh; pMesh = pMesh->next, i++) {
		ppMeshes[i] = pMesh;
	}

	pFile->Read(&i, 4);

	if (i != numMeshes) {
		delete[] ppMeshes;
		CloseCFile(pFile);
		return false;
	}

	if (numMeshes == 0) {
		return true;
	}

	for (i = 0; i < numMeshes; i++) {
		int vtxNum;
		int indexNum;
		pFile->Read(&vtxNum, 4);
		pFile->Read(&indexNum, 4);

		if (vtxNum != ppMeshes[i]->vtxNum || indexNum != ppMeshes[i]->indexNum) {
			delete[] ppMeshes;
			CloseCFile(pFile);
			return false;
		}
	}

	int numUV = 0;
	int numVtx = 0;
	pFile->Read(&numVtx, 4);
	pFile->Read(&numUV, 4);

	if (numVtx == 0 || numUV == 0) {
		delete[] ppMeshes;
		CloseCFile(pFile);
		return false;
	}

	WVector *pVtx = new WVector[numVtx];
	pFile->Read(pVtx, numVtx * sizeof(WVector));

	float (*pUV)[2] = new float[numUV][2];
	pFile->Read(pUV, numUV * sizeof(float[2]));

	SmCase cases[2];

	smLoadSmapMeshes(cases, pFile, ppMeshes, pVtx, pUV);
	smLoadOutFrags(pFile, numMeshes, ppMeshes, pVtx, cases);

	if (pVtx) {
		delete[] pVtx;
	}
	if (pUV) {
		delete[] pUV;
	}

	delete[] ppMeshes;

	int maxVtx = 0;
	for (pMesh = pFirst; pMesh; pMesh = pMesh->next) {
		if (maxVtx < pMesh->vtxNum) {
			maxVtx = pMesh->vtxNum;
		}
	}

	m_pBgModel->m_pPuppet->GetRootBone()->AllocVTX(maxVtx);

	CloseCFile(pFile);

	return true;
}

// polysoup.cpp:2392
// Reads the shadow-map packs of the two cases and appends their meshes to
// the background's mesh list; the second case and the skipped blocks depend
// on the current course's kind.
void WPolySoup::smLoadSmapMeshes(SmCase *pCases, cFile *pFile, w_mesh **ppMeshes, WVector *pVtx, float (*pUV)[2]) {
	int i;
	int size;

	w_mesh *pLast = m_pBgModel->m_pPuppet->GetRootBone()->GetMesh();

	while (pLast && pLast->next) {
		pLast = pLast->next;
	}

	pFile->Read(&size, 4);

	w_mesh *pFirst = 0;
	if (size > 0) {
		pFile->Read(&pCases[0].m_numLists, 4);

		if (pCases[0].m_numLists > 0) {
			pCases[0].m_pLists = new SmInFragList[pCases[0].m_numLists];
		}

		for (i = 0; i < pCases[0].m_numLists; i++) {
			w_mesh *pNew = smAddSmapPack(&pCases[0].m_pLists[i], pFile, 0, i, ppMeshes, pVtx, pUV);
			pLast->next = pNew;
			pLast = pNew;
			if (i == 0) {
				pFirst = pNew;
			}
		}
	}

	switch (CSharedDoc::Instance()->m_pCurCourse == 0 ? (unsigned char)0 : (unsigned char)(CSharedDoc::Instance()->m_pCurCourse->courseFlags >> 4)) {
	case 0:
		pFile->Read(&size, 4);
		pFile->Seek(size, 1);
		pFile->Read(&size, 4);
		pFile->Seek(size, 1);
		break;
	case 2:
		pFile->Read(&size, 4);
		pFile->Seek(size, 1);
	case 1:
		pFile->Read(&size, 4);
		if (size > 0) {
			pFile->Read(&pCases[1].m_numLists, 4);
		}
		break;
	}

	// The original sizes the second case's lists by the first case's count.
	if (pCases[1].m_numLists > 0) {
		pCases[1].m_pLists = new SmInFragList[pCases[0].m_numLists];
	}

	for (i = 0; i < pCases[1].m_numLists; i++) {
		w_mesh *pNew = smAddSmapPack(&pCases[1].m_pLists[i], pFile, CSharedDoc::Instance()->m_pCurCourse == 0 ? (unsigned char)0 : (unsigned char)(CSharedDoc::Instance()->m_pCurCourse->courseFlags >> 4), i, ppMeshes, pVtx, pUV);
		pLast->next = pNew;
		pLast = pNew;
	}

	if ((CSharedDoc::Instance()->m_pCurCourse == 0 ? (unsigned char)0 : (unsigned char)(CSharedDoc::Instance()->m_pCurCourse->courseFlags >> 4)) == 1) {
		pFile->Read(&size, 4);
		pFile->Seek(size, 1);
	}

	m_pSmapMeshes = pFirst;

	for (m_numSmapMeshes = 0; pFirst; pFirst = pFirst->next) {
		m_numSmapMeshes++;
	}

	m_pSmapDrawFlags = new int[m_numSmapMeshes];

	i = 0;
	for (w_mesh *pMesh = m_pSmapMeshes; pMesh; pMesh = pMesh->next) {
		m_pSmapDrawFlags[i++] = pMesh->drawFlag & 0x7FF;
	}
}

// The header of a DDS file (the layout of DirectDraw's DDSURFACEDESC2, size
// 0x7C) smAddSmapPack builds in front of a shadow-map texture.
struct SmDdsHeader {
	unsigned long m_size;                 // 0x00
	unsigned long m_flags;                // 0x04
	unsigned long m_height;               // 0x08
	unsigned long m_width;                // 0x0C
	unsigned long m_linearSize;           // 0x10
	UNKNOWN(0x14, 0x18);
	unsigned long m_mipMapCount;          // 0x18
	UNKNOWN(0x1C, 0x48);
	unsigned long m_formatSize;           // 0x48, the pixel format's size
	unsigned long m_formatFlags;          // 0x4C
	unsigned long m_fourCC;               // 0x50
	UNKNOWN(0x54, 0x68);
	unsigned long m_caps;                 // 0x68
	UNKNOWN(0x6C, 0x7C);
};
CHECK_SIZE(SmDdsHeader, 0x7C);
CHECK_FIELD(SmDdsHeader, m_size, 0x00);
CHECK_FIELD(SmDdsHeader, m_flags, 0x04);
CHECK_FIELD(SmDdsHeader, m_height, 0x08);
CHECK_FIELD(SmDdsHeader, m_width, 0x0C);
CHECK_FIELD(SmDdsHeader, m_linearSize, 0x10);
CHECK_FIELD(SmDdsHeader, m_mipMapCount, 0x18);
CHECK_FIELD(SmDdsHeader, m_formatSize, 0x48);
CHECK_FIELD(SmDdsHeader, m_formatFlags, 0x4C);
CHECK_FIELD(SmDdsHeader, m_fourCC, 0x50);
CHECK_FIELD(SmDdsHeader, m_caps, 0x68);

// polysoup.cpp:2496
// Reads a shadow-map pack: its DXT1 texture, then its fragments, and builds
// a mesh of them; vertices equal in position, alpha and UV are shared.
w_mesh *WPolySoup::smAddSmapPack(SmInFragList *pList, cFile *pFile, int kind, int index, w_mesh **ppMeshes, WVector *pVtx, float (*pUV)[2]) {
	int width;
	int height;
	pFile->Read(&width, 4);
	pFile->Read(&height, 4);

	SmDdsHeader desc;
	memset(&desc, 0, sizeof(desc));
	desc.m_size = sizeof(desc);
	desc.m_flags = 0x80000;               // DDSD_LINEARSIZE
	desc.m_height = height;
	desc.m_width = width;
	desc.m_linearSize = height * width / 2;
	desc.m_mipMapCount = 1;
	desc.m_formatSize = 0x20;
	desc.m_formatFlags = 4;               // DDPF_FOURCC
	desc.m_fourCC = 0x31545844;           // "DXT1"
	desc.m_caps = 0x1000;                 // DDSCAPS_TEXTURE

	int size = desc.m_linearSize + 0x80;
	unsigned char *pBuf = new unsigned char[size];
	memcpy(pBuf + 4, &desc, sizeof(desc));
	pBuf[0] = 'D';
	pBuf[1] = 'D';
	pBuf[2] = 'S';
	pBuf[3] = ' ';
	pFile->Read(pBuf + 0x80, desc.m_linearSize);

	char name[0x20];
	sprintf(name, "%s-%d-%d.dds", m_pBgModel->m_name, kind, index);
	int texHandle = g_resrcmng->LoadTexture_DDS(pBuf, size, name, 0, 0);
	delete[] pBuf;

	int numFrags;
	pFile->Read(&numFrags, 4);
	SmInFrag *pFrags = new SmInFrag[numFrags];
	pList->m_numFrags = numFrags;
	pList->m_pFrags = pFrags;
	pFile->Read(pFrags, numFrags * sizeof(SmInFrag));

	w_mesh *pMesh = new w_mesh;
	memset(pMesh, 0, sizeof(w_mesh));

	pMesh->texHandle = texHandle;

	pMesh->drawFlag = (texHandle & 0x7FF) | 0x21100000;
	pMesh->diffuse = 0xFFFFFF;
	pMesh->alpha = 1.0f;

	pMesh->indexNum = numFrags * 3;
	pMesh->indexList = new unsigned short[pMesh->indexNum];
	WVector *pVerts = new WVector[pMesh->indexNum];
	float (*pUVs)[2] = new float[pMesh->indexNum][2];
	unsigned long *pColors = new unsigned long[pMesh->indexNum];
	int numVtx = 0;

	for (int i = 0; i < numFrags; i++) {
		for (int j = 0; j < 3; j++) {
			int vtx = pFrags[i].m_vtx[j];
			int uv = pFrags[i].m_uv[j];
			int k;

			if (vtx < 0x8000) {
				w_mesh *pSrc = ppMeshes[pFrags[i].m_meshIndex];
				unsigned short *pIdx = &pSrc->indexList[pFrags[i].m_triIndex * 3];
				unsigned short i0 = pIdx[0];
				unsigned short i1 = pIdx[1];
				unsigned short i2 = pIdx[2];
				float u, v;
				GetBarycentricProjected(pSrc->vecList[i0], pSrc->vecList[i1], pSrc->vecList[i2], pVtx[vtx], u, v);
				unsigned long color = ((unsigned char)InterpolateTriangle((float)(pSrc->vtxColorList[i0] >> 24), (float)(pSrc->vtxColorList[i1] >> 24), (float)(pSrc->vtxColorList[i2] >> 24), u, v) << 24) | 0xFFFFFF;

				for (k = 0; k < numVtx; k++) {
					if (WisEqual(pVerts[k].x, pVtx[vtx].x, 0.00001f) && WisEqual(pVerts[k].y, pVtx[vtx].y, 0.00001f) && WisEqual(pVerts[k].z, pVtx[vtx].z, 0.00001f) && pColors[k] == color && WisEqual(pUVs[k][0], pUV[uv][0], 0.00001f) && WisEqual(pUVs[k][1], pUV[uv][1], 0.00001f)) {
						break;
					}
				}

				if (k == numVtx) {
					pVerts[numVtx] = pVtx[vtx];
					pColors[numVtx] = color;
					pUVs[numVtx][0] = pUV[uv][0];
					pUVs[numVtx][1] = pUV[uv][1];
					pMesh->indexList[i * 3 + j] = numVtx;
					numVtx++;
				} else {
					pMesh->indexList[i * 3 + j] = k;
				}
			} else {
				w_mesh *pSrc = ppMeshes[pFrags[i].m_meshIndex];
				vtx -= 0x8000;

				for (k = 0; k < numVtx; k++) {
					if (WisEqual(pVerts[k].x, pSrc->vecList[vtx].x, 0.00001f) && WisEqual(pVerts[k].y, pSrc->vecList[vtx].y, 0.00001f) && WisEqual(pVerts[k].z, pSrc->vecList[vtx].z, 0.00001f) && pColors[k] == (pSrc->vtxColorList[vtx] | 0xFFFFFF) && WisEqual(pUVs[k][0], pUV[uv][0], 0.00001f) && WisEqual(pUVs[k][1], pUV[uv][1], 0.00001f)) {
						break;
					}
				}

				if (k == numVtx) {
					pVerts[numVtx] = pSrc->vecList[vtx];
					pUVs[numVtx][0] = pUV[uv][0];
					pUVs[numVtx][1] = pUV[uv][1];
					pColors[numVtx] = pSrc->vtxColorList[vtx] | 0xFFFFFF;
					pMesh->indexList[i * 3 + j] = numVtx;
					numVtx++;
				} else {
					pMesh->indexList[i * 3 + j] = k;
				}
			}
		}
	}

	pMesh->vtxNum = numVtx;
	pMesh->vecList = new WVector[numVtx];
	memcpy(pMesh->vecList, pVerts, numVtx * sizeof(WVector));
	pMesh->vtxColorList = new unsigned long[numVtx];
	memcpy(pMesh->vtxColorList, pColors, numVtx * sizeof(unsigned long));
	pMesh->originalVtxColorList = new unsigned long[numVtx];
	memcpy(pMesh->originalVtxColorList, pColors, numVtx * sizeof(unsigned long));
	pMesh->uvData = new float[numVtx][2];
	memcpy(pMesh->uvData, pUVs, numVtx * sizeof(float[2]));

	WBone::CalcMeshAABB(*m_pBgModel->m_pPuppet->GetRootBone(), *pMesh);

	delete[] pVerts;
	delete[] pUVs;
	delete[] pColors;

	return pMesh;
}

// polysoup.cpp:2701
// Loads the output fragments of the .sbin file: per mesh, the fragments the
// file stores for the current course case and the input fragments of the
// two cases, merged, rid of their duplicates (same vertices, any rotation),
// sorted and built into the shadow-map meshes (smBuildOutFragMeshes).
void WPolySoup::smLoadOutFrags(cFile *pFile, int numMeshes, w_mesh **ppMeshes, WVector *pVtx, SmCase *pCases) {
	int numInFrags = 0;
	int c, i, j;

	for (c = 0; c < 2; c++) {
		SmInFragList *pLists = pCases[c].m_pLists;
		if (pLists) {
			for (i = 0; i < pCases[c].m_numLists; i++) {
				for (j = 0; j < pLists[i].m_numFrags; j++) {
					if (pLists[i].m_pFrags[j].m_triIndex != 0xFFFF) {
						numInFrags++;
					}
				}
			}
		}
	}

	int numOutA;
	pFile->Read(&numOutA, 4);

	int numOutB = 0;
	int numSkipAfter = 0;
	int numSkipBefore = 0;
	switch (CSharedDoc::Instance()->m_pCurCourse == 0 ? (unsigned char)0 : (unsigned char)(CSharedDoc::Instance()->m_pCurCourse->courseFlags >> 4)) {
	case 0:
		pFile->Read(&numSkipBefore, 4);
		pFile->Read(&numSkipAfter, 4);
		break;
	case 1:
		pFile->Read(&numOutB, 4);
		pFile->Read(&numSkipAfter, 4);
		break;
	case 2:
		pFile->Read(&numSkipBefore, 4);
		pFile->Read(&numOutB, 4);
		break;
	}

	// Four arrays of numMeshes counters: the first fragment of each mesh in
	// pOut, the file's fragments A and B of each mesh, its input fragments.
	int *pFirst = new int[numMeshes * 4];
	int *pCountA = pFirst + numMeshes;
	int *pCountB = pFirst + numMeshes * 2;
	int *pCountIn = pFirst + numMeshes * 3;
	memset(pFirst, 0, numMeshes * 16);
	SmOutFrag *pOut = new SmOutFrag[numOutA + numOutB + numInFrags];

	if (numOutA > 0) {
		pFile->Read(pCountA, numMeshes * 4);
	}
	if (numSkipBefore > 0) {
		pFile->Seek(numMeshes * 4, 1);
	}
	if (numOutB > 0) {
		pFile->Read(pCountB, numMeshes * 4);
	}
	if (numSkipAfter > 0) {
		pFile->Seek(numMeshes * 4, 1);
	}

	for (c = 0; c < 2; c++) {
		if (pCases[c].m_pLists) {
			for (i = 0; i < pCases[c].m_numLists; i++) {
				SmInFragList *pLists = pCases[c].m_pLists;
				if (pLists) {
					for (j = 0; j < pLists[i].m_numFrags; j++) {
						SmInFrag *pFrag = &pLists[i].m_pFrags[j];
						if (pFrag->m_triIndex != 0xFFFF) {
							pCountIn[pFrag->m_meshIndex]++;
						}
					}
				}
			}
		}
	}

	pFirst[0] = 0;
	for (i = 1; i < numMeshes; i++) {
		pFirst[i] = pFirst[i - 1] + pCountIn[i - 1] + pCountA[i - 1] + pCountB[i - 1];
	}

	memset(pCountIn, 0, numMeshes * 4);

	for (i = 0; i < numMeshes; i++) {
		if (pCountA[i] > 0) {
			pFile->Read(&pOut[pFirst[i]], pCountA[i] * 8);
		}
	}
	if (numSkipBefore) {
		pFile->Seek(numSkipBefore * 8, 1);
	}

	if (numOutB > 0) {
		for (i = 0; i < numMeshes; i++) {
			if (pCountB[i] > 0) {
				pFile->Read(&pOut[pFirst[i] + pCountA[i]], pCountB[i] * 8);
			}
		}
	}

	for (c = 0; c < 2; c++) {
		if (pCases[c].m_pLists) {
			for (i = 0; i < pCases[c].m_numLists; i++) {
				SmInFragList *pLists = pCases[c].m_pLists;
				if (pLists) {
					for (j = 0; j < pLists[i].m_numFrags; j++) {
						SmInFrag *pFrag = &pLists[i].m_pFrags[j];
						if (pFrag->m_triIndex != 0xFFFF) {
							int mesh = pFrag->m_meshIndex;
							int *pCount = &pCountIn[mesh];
							SmOutFrag *pDst = &pOut[pCountB[mesh] + pCountA[mesh] + pFirst[mesh] + *pCount];
							pDst->m_triIndex = pFrag->m_triIndex;
							pDst->m_vtx[0] = pFrag->m_vtx[0];
							pDst->m_vtx[1] = pFrag->m_vtx[1];
							pDst->m_vtx[2] = pFrag->m_vtx[2];
							(*pCount)++;
						}
					}
				}
			}
		}
	}

	if (numSkipAfter) {
		pFile->Seek(numSkipAfter * 8, 1);
	}

	for (i = 0; i < numMeshes; i++) {
		if (pCountB[i] > 0) {
			pFile->Read(&pOut[pFirst[i] + pCountA[i]], pCountB[i] * 8);
		}
		pCountA[i] += pCountIn[i] + pCountB[i];
	}

	for (i = 0; i < numMeshes; i++) {
		int count = pCountA[i];
		if (count <= 0) {
			continue;
		}
		SmOutFrag *pFrags = &pOut[pFirst[i]];

		// The fragments with no later duplicate.
		int numUnique = 0;
		int k, r;
		for (j = 0; j < count - 1; j++) {
			for (k = j + 1; k < count; k++) {
				for (r = 0; r < 3; r++) {
					if (pFrags[j].m_vtx[0] == pFrags[k].m_vtx[r] && pFrags[j].m_vtx[1] == pFrags[k].m_vtx[(r + 1) % 3] && pFrags[j].m_vtx[2] == pFrags[k].m_vtx[(r + 2) % 3]) {
						break;
					}
				}
				if (r < 3) {
					break;
				}
			}
			if (k == count) {
				numUnique++;
			}
		}

		if (numUnique + 1 == count) {
			qsort(pFrags, count, 8, smCompareOutFrag);
			smBuildOutFragMeshes(ppMeshes[i], pFrags, count, pVtx);
		} else {
			SmOutFrag *pKept = new SmOutFrag[numUnique + 1];
			int numKept = 0;
			for (j = 0; j < pCountA[i] - 1; j++) {
				for (k = j + 1; k < pCountA[i]; k++) {
					for (r = 0; r < 3; r++) {
						if (pFrags[j].m_vtx[0] == pFrags[k].m_vtx[r] && pFrags[j].m_vtx[1] == pFrags[k].m_vtx[(r + 1) % 3] && pFrags[j].m_vtx[2] == pFrags[k].m_vtx[(r + 2) % 3]) {
							break;
						}
					}
					if (r < 3) {
						break;
					}
				}
				if (k == pCountA[i]) {
					pKept[numKept++] = pFrags[j];
				}
			}
			pKept[numKept++] = pFrags[j];
			qsort(pKept, numKept, 8, smCompareOutFrag);
			smBuildOutFragMeshes(ppMeshes[i], pKept, numKept, pVtx);
			delete[] pKept;
		}
	}

	delete[] pFirst;
	delete[] pOut;
}

// polysoup.cpp:2967
// qsort comparator of the output fragments, ascending by their leading
// 16-bit key.
int __cdecl WPolySoup::smCompareOutFrag(const void *a, const void *b) {
	unsigned short keyA = *(const unsigned short *)a;
	unsigned short keyB = *(const unsigned short *)b;

	if (keyA < keyB) {
		return -1;
	}
	if (keyA > keyB) {
		return 1;
	}
	return 0;
}

// polysoup.cpp:2982
// Splits the triangles of pMesh named by the output fragments (sorted by
// triangle) into the fragments' triangles. A fragment vertex below 0x8000 is
// a new vertex at pVtx[vtx], its normal, color and UV interpolated over the
// split triangle; from 0x8000 up it is the mesh's own vertex vtx - 0x8000.
// The split triangles' indices are replaced by the fragments' ones, appended
// after the indices of the triangles that are kept.
void WPolySoup::smBuildOutFragMeshes(w_mesh *pMesh, SmOutFrag *pFrags, int numFrags, WVector *pVtx) {
	if (numFrags == 0) {
		return;
	}

	unsigned short *pOldIdx = pMesh->indexList;
	WVector *pOldVec = pMesh->vecList;
	float (*pOldUV)[2] = pMesh->uvData;
	WVector *pOldNorm = pMesh->normList;
	unsigned long *pOldColors = pMesh->vtxColorList;
	int numTris = 1;
	int i, j, k;

	for (i = 1; i < numFrags; i++) {
		if (pFrags[i].m_triIndex != pFrags[i - 1].m_triIndex) {
			numTris++;
		}
	}

	int numKeptIdx = pMesh->indexNum - numTris * 3;
	int newIndexNum = numKeptIdx + numFrags * 3;
	unsigned short *pNewIdx = new unsigned short[newIndexNum];
	WVector *pNewVec = new WVector[numFrags * 3];
	WVector *pNewNorm = new WVector[numFrags * 3];
	float (*pNewUV)[2] = new float[numFrags * 3][2];
	unsigned long *pNewColors = new unsigned long[numFrags * 3];
	int numNew = 0;

	for (i = 0; i < numFrags; i++) {
		int tri = pFrags[i].m_triIndex;
		int i0 = pOldIdx[tri * 3];
		int i1 = pOldIdx[tri * 3 + 1];
		int i2 = pOldIdx[tri * 3 + 2];
		unsigned short *pDst = &pNewIdx[numKeptIdx + i * 3];

		for (j = 0; j < 3; j++) {
			int vtx = pFrags[i].m_vtx[j];

			if (vtx < 0x8000) {
				WVector *pPos = &pVtx[vtx];
				float u, v;
				GetBarycentricProjected(pOldVec[i0], pOldVec[i1], pOldVec[i2], *pPos, u, v);

				WVector normal;
				normal.x = InterpolateTriangle(pOldNorm[i0].x, pOldNorm[i1].x, pOldNorm[i2].x, u, v);
				normal.y = InterpolateTriangle(pOldNorm[i0].y, pOldNorm[i1].y, pOldNorm[i2].y, u, v);
				normal.z = InterpolateTriangle(pOldNorm[i0].z, pOldNorm[i1].z, pOldNorm[i2].z, u, v);

				unsigned long c0 = pOldColors[i0];
				unsigned long c1 = pOldColors[i1];
				unsigned long c2 = pOldColors[i2];
				unsigned long color = (unsigned char)(int)InterpolateTriangle((float)(c0 >> 24 & 0xFF), (float)(c1 >> 24 & 0xFF), (float)(c2 >> 24 & 0xFF), u, v);
				color = color << 8 | (unsigned char)(int)InterpolateTriangle((float)(c0 >> 16 & 0xFF), (float)(c1 >> 16 & 0xFF), (float)(c2 >> 16 & 0xFF), u, v);
				color = color << 8 | (unsigned char)(int)InterpolateTriangle((float)(c0 >> 8 & 0xFF), (float)(c1 >> 8 & 0xFF), (float)(c2 >> 8 & 0xFF), u, v);
				color = color << 8 | (unsigned char)(int)InterpolateTriangle((float)(c0 & 0xFF), (float)(c1 & 0xFF), (float)(c2 & 0xFF), u, v);

				float uv[2];
				uv[0] = InterpolateTriangle(pOldUV[i0][0], pOldUV[i1][0], pOldUV[i2][0], u, v);
				uv[1] = InterpolateTriangle(pOldUV[i0][1], pOldUV[i1][1], pOldUV[i2][1], u, v);

				for (k = 0; k < numNew; k++) {
					if (WisEqual(pNewVec[k].x, pPos->x, 0.00001f) && WisEqual(pNewVec[k].y, pPos->y, 0.00001f) && WisEqual(pNewVec[k].z, pPos->z, 0.00001f) && WisEqual(pNewNorm[k].x, normal.x, 0.00001f) && WisEqual(pNewNorm[k].y, normal.y, 0.00001f) && WisEqual(pNewNorm[k].z, normal.z, 0.00001f) && WisEqual(pNewUV[k][0], uv[0], 0.00001f) && WisEqual(pNewUV[k][1], uv[1], 0.00001f)) {
						break;
					}
				}

				if (k == numNew) {
					pNewVec[numNew] = *pPos;
					pNewNorm[numNew] = normal;
					pNewUV[numNew][0] = uv[0];
					pNewUV[numNew][1] = uv[1];
					pNewColors[numNew] = color;
					*pDst = (unsigned short)(pMesh->vtxNum + numNew);
					numNew++;
				} else {
					*pDst = (unsigned short)(pMesh->vtxNum + k);
				}
			} else {
				*pDst = (unsigned short)(vtx - 0x8000);
			}

			pDst++;
		}
	}

	// The indices of the triangles before, between and after the split ones.
	int numIdx = 0;

	if (pFrags[0].m_triIndex > 0) {
		memcpy(pNewIdx, pOldIdx, pFrags[0].m_triIndex * 3 * sizeof(unsigned short));
		numIdx = pFrags[0].m_triIndex * 3;
	}

	for (i = 1; i < numFrags; i++) {
		int prev = pFrags[i - 1].m_triIndex * 3;
		int cur = pFrags[i].m_triIndex * 3;

		if (cur - prev > 3) {
			memcpy(&pNewIdx[numIdx], &pOldIdx[prev + 3], (cur - prev - 3) * sizeof(unsigned short));
			numIdx += cur - prev - 3;
		}
	}

	int last = pFrags[numFrags - 1].m_triIndex * 3;
	int rest = pMesh->indexNum - last;

	if (rest > 3) {
		memcpy(&pNewIdx[numIdx], &pOldIdx[last + 3], (rest - 3) * sizeof(unsigned short));
	}

	WVector *pVec = new WVector[pMesh->vtxNum + numNew];
	memcpy(pVec, pMesh->vecList, pMesh->vtxNum * sizeof(WVector));
	memcpy(&pVec[pMesh->vtxNum], pNewVec, numNew * sizeof(WVector));

	WVector *pNorm = new WVector[pMesh->vtxNum + numNew];
	memcpy(pNorm, pMesh->normList, pMesh->vtxNum * sizeof(WVector));
	memcpy(&pNorm[pMesh->vtxNum], pNewNorm, numNew * sizeof(WVector));

	unsigned long *pColors = new unsigned long[pMesh->vtxNum + numNew];
	memcpy(pColors, pMesh->vtxColorList, pMesh->vtxNum * sizeof(unsigned long));
	memcpy(&pColors[pMesh->vtxNum], pNewColors, numNew * sizeof(unsigned long));

	unsigned long *pOrigColors = new unsigned long[pMesh->vtxNum + numNew];
	memcpy(pOrigColors, pColors, (pMesh->vtxNum + numNew) * sizeof(unsigned long));

	float (*pUV)[2] = new float[pMesh->vtxNum + numNew][2];
	memcpy(pUV, pMesh->uvData, pMesh->vtxNum * sizeof(float[2]));
	memcpy(&pUV[pMesh->vtxNum], pNewUV, numNew * sizeof(float[2]));

	delete[] pNewVec;
	delete[] pNewNorm;
	delete[] pNewUV;
	delete[] pNewColors;

	delete[] pMesh->vecList;
	delete[] pMesh->normList;
	delete[] pMesh->vtxColorList;
	delete[] pMesh->originalVtxColorList;
	delete[] pMesh->uvData;
	delete[] pMesh->indexList;

	pMesh->vtxNum += numNew;
	pMesh->indexNum = newIndexNum;
	pMesh->vecList = pVec;
	pMesh->normList = pNorm;
	pMesh->vtxColorList = pColors;
	pMesh->originalVtxColorList = pOrigColors;
	pMesh->uvData = pUV;
	pMesh->indexList = pNewIdx;
}

// polysoup.cpp:3204
void WPolySoup::EnableShadowmap(bool enable) {
	w_mesh *pMesh;

	if (enable) {
		if (m_pSmapMeshes) {
			for (pMesh = m_pSmapMeshes; pMesh; pMesh = pMesh->next) {
				if ((pMesh->drawFlag & 0x21100000) == 0x21100000) {
					pMesh->alpha = 1.0f;
				}
			}
		} else if (!m_bNoShadowmap) {
			ReloadBasePet();
		}
	} else {
		for (pMesh = m_pSmapMeshes; pMesh; pMesh = pMesh->next) {
			if ((pMesh->drawFlag & 0x21100000) == 0x21100000) {
				pMesh->alpha = 0.0f;
			}
		}
	}
}

// polysoup.cpp:3234
// Reloads the background puppet and its vertex colours from the map file,
// then the shadow map and the hole cup.
void WPolySoup::ReloadBasePet(void) {
	if (g_resrcmng == 0) {
		return;
	}

	if (m_pBgModel->m_pPuppet) {
		g_resrcmng->Release(m_pBgModel->m_pPuppet, true);
		m_pBgModel->m_pPuppet = 0;
	}

	m_pBgModel->m_pPuppet = g_resrcmng->GetPuppet(m_pBgModel->m_name, false, false, false);

	m_pBgModel->m_pPuppet->m_lightmode = 0x1000000;
	m_pBgModel->m_pPuppet->ApplyBones(0, m_pBgModel->m_matrix);
	m_pBgModel->m_pPuppet->UpdateBound(true, false);
	m_pBgModel->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);

	cFile *pFile = GetResrcManager()->GetCFile(m_mapName, -1);

	int offset = m_numTextures * 0x20 + m_numCameras * 0x40 + m_numAreas * 0x59 + m_numPoints * 0x2D + 0x28;

	for (int i = 0; i < m_numPoints; i++) {
		if (m_pPoints[i].m_type) {
			offset += 0x40;
		}
	}

	for (std::vector<CNodeContainer>::iterator it = m_boundAreas.begin(); it != m_boundAreas.end(); ++it) {
		offset += it->GetList().size() * 12 + 0x18;
	}

	pFile->Seek(offset + 0xAC, 1);

	for (w_mesh *pMesh = m_pBgModel->m_pPuppet->GetRootBone()->GetMesh(); pMesh; pMesh = pMesh->next) {
		for (int i = 0; i < pMesh->indexNum; i += 3) {
			for (int j = 0; j < 3; j++) {
				pFile->Read(&pMesh->vtxColorList[pMesh->indexList[i + j]], 4);
			}
		}
	}

	CloseCFile(pFile);

	m_bNoShadowmap = !LoadShadowmapInfo();

	if (strstr(m_mapName, "rank") == 0) {
		PlaceHoleCup();
	}
	m_pBgModel->m_pPuppet->xUpdateTnLBuffers(true);

	CSceneManager::Instance()->SetPuppet(m_pBgModel->m_name, m_pBgModel->m_pPuppet);
}

// polysoup.cpp:3341
// Digs the hole cup into the background: a 32-gon of the cup's radius around
// the current hole's cup position (taken into the root bone's space) is cut
// out of the meshes it lies on.
void WPolySoup::PlaceHoleCup(void) {
	int i;
	for (i = 0; i < m_numPoints; i++) {
		if (strstr(m_pPoints[i].m_name, "³¡Á¡")) {
			break;
		}
	}
	if (i == m_numPoints) {
		return;
	}

	std::vector<sCupVtx> cupVtxs;
	std::vector<sHoleTri> tris;
	std::vector<sHoleMesh> meshes;

	WMatrix inv = ~m_pBgModel->m_pPuppet->GetRootBone()->GetMatrix();
	WVector center = GOLFDOC()->GetHoleData().m_cupPos * inv;

	float radius = GolfBall().m_radius * 3.0f;

	if (IsLocalContent((localContentType_t)0x3A)) {
		CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
		if ((pDoc->m_currentChannel.flags & 0x800) && pDoc->m_roomType != 0x0B && pDoc->m_roomType != 0x0C) {
			radius = GolfBall().m_radius * 6.0f;
		}
	}

	for (int n = 0; n < 0x21; n++) {
		float angle = (float)n * 0.19634955f;
		sCupVtx vtx;
		vtx.m_pos.x = cosf(angle) * radius + center.x;
		vtx.m_pos.z = center.z - sinf(angle) * radius;
		cupVtxs.push_back(vtx);
	}

	GatherHolecupTris(tris, cupVtxs, center);

	if (tris.size() == 0) {
		return;
	}

	CreateNewHolecupMeshes(meshes, cupVtxs, tris);
	AddHolecup(meshes, cupVtxs, tris);

	WBone *pBone = m_pBgModel->m_pPuppet->GetRootBone();
	int maxVtx = 0;
	for (w_mesh *pMesh = pBone->GetMesh(); pMesh; pMesh = pMesh->next) {
		if (maxVtx < pMesh->vtxNum) {
			maxVtx = pMesh->vtxNum;
		}
	}
	pBone->AllocVTX(maxVtx);
}

// polysoup.cpp:3557
// Whether pos lies inside the triangle tri on the XZ plane: pos must be on
// the same side of every edge as the triangle's own winding.
bool WPolySoup::IsInTri(WVector &pos, WVector *tri[]) {
	float x1 = tri[1]->x - tri[0]->x;
	float z1 = tri[1]->z - tri[0]->z;
	float len = 1.0f / sqrtf(z1 * z1 + x1 * x1);
	x1 *= len;
	z1 *= len;
	float x2 = tri[2]->x - tri[0]->x;
	float z2 = tri[2]->z - tri[0]->z;

	float px = pos.x;
	float pz = pos.z;
	len = 1.0f / sqrtf(z2 * z2 + x2 * x2);
	x2 *= len;
	z2 *= len;

	float winding = x1 * z2 - x2 * z1;

	if (Abs(winding) < 0.001f)
		return false;

	for (int i = 0; i < 3; i++) {
		float ex = tri[(i + 1) % 3]->x - tri[i]->x;
		float ez = tri[(i + 1) % 3]->z - tri[i]->z;
		len = 1.0f / sqrtf(ez * ez + ex * ex);
		ex *= len;
		ez *= len;
		float dx = px - tri[i]->x;
		float dz = pz - tri[i]->z;
		len = 1.0f / sqrtf(dz * dz + dx * dx);
		dx *= len;
		dz *= len;

		float side = ex * dz - dx * ez;

		if (side * winding < 0.0f)
			return false;
	}

	return true;
}

// polysoup.cpp:3600
// Whether pos lies inside the closed polygon verts (its last vertex repeats
// the first) on the XZ plane: pos must be on the same side of every edge as
// the polygon's own winding.
bool WPolySoup::IsInPoly(WVector &pos, std::vector<sCupVtx> &verts) {
	if (verts.size() < 4)
		return false;

	float x1 = verts[1].m_pos.x - verts[0].m_pos.x;
	float z1 = verts[1].m_pos.z - verts[0].m_pos.z;
	float len = 1.0f / sqrtf(z1 * z1 + x1 * x1);
	x1 *= len;
	z1 *= len;
	float x2 = verts[2].m_pos.x - verts[0].m_pos.x;
	float z2 = verts[2].m_pos.z - verts[0].m_pos.z;
	len = 1.0f / sqrtf(z2 * z2 + x2 * x2);
	x2 *= len;
	z2 *= len;

	float winding = x1 * z2 - x2 * z1;

	for (int i = 0; i < (int)verts.size() - 1; i++) {
		float ex = verts[i + 1].m_pos.x - verts[i].m_pos.x;
		float ez = verts[i + 1].m_pos.z - verts[i].m_pos.z;
		len = 1.0f / sqrtf(ez * ez + ex * ex);
		ex *= len;
		ez *= len;
		float dx = pos.x - verts[i].m_pos.x;
		float dz = pos.z - verts[i].m_pos.z;
		len = 1.0f / sqrtf(dz * dz + dx * dx);
		dx *= len;
		dz *= len;

		float side = ex * dz - dx * ez;

		if (side * winding <= 0.0f)
			return false;
	}

	return true;
}

// polysoup.cpp:3653
// Whether the segments (a, b) and (c, d) cross on the XZ plane: each
// segment's ends lie on opposite sides of the other.
bool WPolySoup::AreCrossed(WVector *a, WVector *b, WVector *c, WVector *d) {
	float x1 = b->x - a->x;
	float z1 = b->z - a->z;
	float len = 1.0f / sqrtf(z1 * z1 + x1 * x1);
	x1 *= len;
	z1 *= len;
	float x2 = c->x - a->x;
	float z2 = c->z - a->z;
	len = 1.0f / sqrtf(z2 * z2 + x2 * x2);
	x2 *= len;
	z2 *= len;
	float side1 = x1 * z2 - x2 * z1;

	float x3 = d->x - a->x;
	float z3 = d->z - a->z;
	len = 1.0f / sqrtf(z3 * z3 + x3 * x3);
	x3 *= len;
	z3 *= len;

	float side2 = x1 * z3 - x3 * z1;

	if (side2 * side1 > 0.0f)
		return false;

	x1 = d->x - c->x;
	z1 = d->z - c->z;
	len = 1.0f / sqrtf(z1 * z1 + x1 * x1);
	x1 *= len;
	z1 *= len;

	x2 = a->x - c->x;
	z2 = a->z - c->z;
	len = 1.0f / sqrtf(z2 * z2 + x2 * x2);
	x2 *= len;
	z2 *= len;

	side1 = x1 * z2 - x2 * z1;

	x3 = b->x - c->x;
	z3 = b->z - c->z;
	len = 1.0f / sqrtf(z3 * z3 + x3 * x3);
	x3 *= len;
	z3 *= len;

	side2 = x1 * z3 - x3 * z1;

	if (side2 * side1 > 0.0f)
		return false;

	return true;
}

// polysoup.cpp:3709
void WPolySoup::GenHolecupTri(sHoleTri *pTri, w_mesh *pMesh, int index) {
	pTri->m_pMesh = pMesh;
	pTri->m_index = index;

	for (int i = 0; i < 3; i++) {
		pTri->m_vtx[i] = pMesh->indexList[index + i];

		pTri->m_pNormal[i] = &pMesh->normList[pTri->m_vtx[i]];
		pTri->m_color[i] = pMesh->vtxColorList[pTri->m_vtx[i]];
		pTri->m_uv[i][0] = pMesh->uvData[pTri->m_vtx[i]][0];
		pTri->m_uv[i][1] = pMesh->uvData[pTri->m_vtx[i]][1];
		if (pMesh->uvBackup) {
			pTri->m_uvBackup[i][0] = pMesh->uvBackup[pTri->m_vtx[i]][0];
			pTri->m_uvBackup[i][1] = pMesh->uvBackup[pTri->m_vtx[i]][1];
		}

		const WVector *v0 = pTri->m_pVertex[i];
		const WVector *v1 = pTri->m_pVertex[(i + 1) % 3];
		pTri->m_edge[i].z = 0.0f;
		pTri->m_edge[i].x = v0->z - v1->z;
		pTri->m_edge[i].y = v1->x - v0->x;
		pTri->m_edge[i].Normalize();
		pTri->m_edge[i].z = (v0->z * pTri->m_edge[i].y + v0->x * pTri->m_edge[i].x) * -1.0f;
	}
}

// polysoup.cpp:3739
// The intersection of the segment (a, b) with the XZ line
// line->x * x + line->y * z + line->z = 0, stored in out; true only when it
// also lies strictly between c and d.
bool WPolySoup::GetIntersection(WVector *out, WVector *a, WVector *b, WVector *c, WVector *d, WVector *line) {
	float distA = a->x * line->x + a->z * line->y + line->z;

	if (WAbsBySign(distA) < 0.001f)
		return false;

	float distB = b->x * line->x + b->z * line->y + line->z;

	if (WAbsBySign(distB) < 0.001f)
		return false;

	if (distB * distA > 0.0f)
		return false;

	out->x = (b->x - a->x) * distA / (distA - distB) + a->x;
	out->z = (b->z - a->z) * distA / (distA - distB) + a->z;

	float dx = WAbsBySign(d->x - c->x);
	float dz = WAbsBySign(d->z - c->z);

	if (dx > dz) {
		if ((out->x - c->x) * (d->x - out->x) > 0.0f)
			return true;
	} else if ((out->z - c->z) * (d->z - out->z) > 0.0f) {
		return true;
	}

	return false;
}

// polysoup.cpp:3780
void WPolySoup::CreateNewHolecupMeshes(std::vector<sHoleMesh> &meshes, std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris) {
	std::vector<WVector> clipped;

	for (std::vector<sHoleTri>::iterator it = tris.begin(); it != tris.end(); ) {
		if (ClipHolecup(clipped, *it, cupVtxs) >= 3) {
			BuildNewHolecupMesh(meshes, *it, clipped);
			++it;
		} else {
			it = tris.erase(it);
		}
	}
}

// polysoup.cpp:3830
// Clips the cup polygon (cupVtxs) against the three XZ edge lines of tri
// (Sutherland-Hodgman, keeping the side where a line is <= 0) into clipped,
// closing the polygon by repeating its first vertex. Returns the vertex count.
int WPolySoup::ClipHolecup(std::vector<WVector> &clipped, sHoleTri &tri, std::vector<sCupVtx> &cupVtxs) {
	std::vector<WVector> work;

	std::vector<WVector>().swap(clipped);
	int count = (int)cupVtxs.size();

	for (int i = 0; i < count; i++)
		work.push_back(cupVtxs[i].m_pos);

	std::vector<WVector> *pIn = &work;
	std::vector<WVector> *pOut = &clipped;

	for (int edge = 0; edge < 3; edge++) {
		WVector &line = tri.m_edge[edge];

		WVector *prev = &(*pIn)[0];
		float dist = line.x * prev->x + line.y * prev->z + line.z;
		int prevSide;
		if (dist > 0.001f) {
			prevSide = 1;
		} else {
			prevSide = dist < -0.001f ? -1 : 0;
			pOut->push_back(*prev);
		}

		for (int j = 1; j < count; j++) {
			WVector *cur = &(*pIn)[j];
			dist = line.x * cur->x + line.y * cur->z + line.z;
			int side;
			if (dist > 0.001f)
				side = 1;
			else if (dist < -0.001f)
				side = -1;
			else
				side = 0;

			if (prevSide == 0) {
				if (side < 0)
					pOut->push_back(*cur);
			} else if (side == 0) {
				pOut->push_back(*cur);
			} else {
				if (side * prevSide < 0) {
					WVector cross;
					GetIntersection(&cross, cur - 1, cur, tri.m_pVertex[edge], tri.m_pVertex[(edge + 1) % 3], &line);
					pOut->push_back(cross);
				}
				if (side < 0)
					pOut->push_back(*cur);
			}
			prevSide = side;
		}

		if (pOut->size() < 3)
			return (int)pOut->size();

		WVector &first = pOut->front();
		WVector &last = pOut->back();
		if (!WisEqual(first.x, last.x, g_EPSILON) || !WisEqual(first.y, last.y, g_EPSILON) || !WisEqual(first.z, last.z, g_EPSILON))
			pOut->push_back(first);

		std::vector<WVector> *tmp = pIn;
		pIn = pOut;
		pOut = tmp;
		if (edge < 2)
			std::vector<WVector>().swap(*pOut);

		count = (int)pIn->size();
	}

	return (int)clipped.size();
}

// polysoup.cpp:3919
void WPolySoup::BuildNewHolecupMesh(std::vector<sHoleMesh> &meshes, sHoleTri &tri, std::vector<WVector> &clipped) {
	std::vector<WVector> edges;
	int numClipped = (int)clipped.size();
	int i;

	// The XZ line of each edge of the clipped polygon: (a, b, c), a*x + b*z + c.
	edges.reserve(numClipped);
	for (i = 0; i < numClipped; i++) {
		int next = i + 1;
		if (next >= numClipped)
			next++;
		next %= numClipped;

		WVector line;
		line.x = clipped[i].z - clipped[next].z;
		line.y = clipped[next].x - clipped[i].x;
		line.z = 0.0f;
		line.Normalize();
		line.z = (line.x * clipped[i].x + line.y * clipped[i].z) * -1.0f;
		edges.push_back(line);
	}

	bool used[3] = { false, false, false };
	int firstInside = -1;
	bool started = false;
	int prevCount = 0;
	int prevInside[3];

	for (i = 0; i < numClipped; i++) {
		WVector &edge = edges[i];
		int inside[3];
		int count = 0;
		int k;

		for (k = 0; k < 3; k++) {
			if (tri.m_pVertex[k]->x * edge.x + tri.m_pVertex[k]->z * edge.y + edge.z > 0.001f)
				inside[count++] = k;
		}

		if (count == 2) {
			if (inside[0] == 0 && inside[1] == 2) {
				inside[1] = 0;
				inside[0] = 2;
			}
			if (i == 0)
				firstInside = inside[0];
		}

		switch (count) {
		case 0:
			started = true;
			break;

		case 1: {
			int cur = inside[0];
			int other = -1;
			if (prevCount == 1)
				other = prevInside[0];
			else if (prevCount == 2)
				other = prevInside[1];

			if (other != -1 && cur != other && !used[other]) {
				used[other] = true;
				AddNewHolecupTri(meshes, tri, tri.m_pVertex[other], tri.m_pVertex[cur], &clipped[i],
					tri.m_vtx[other], tri.m_vtx[cur], -1);
			}

			if (i < numClipped - 1) {
				AddNewHolecupTri(meshes, tri, tri.m_pVertex[cur], &clipped[i + 1], &clipped[i],
					tri.m_vtx[cur], -1, -1);
			}
			started = true;
			break;
		}

		case 2: {
			if (prevCount == 1 && inside[0] != prevInside[0] && !used[prevInside[0]]) {
				int other = prevInside[0];
				used[other] = true;
				started = true;
				AddNewHolecupTri(meshes, tri, tri.m_pVertex[other], tri.m_pVertex[inside[0]], &clipped[i],
					tri.m_vtx[other], tri.m_vtx[inside[0]], -1);
			}

			if (i < numClipped - 1) {
				int cur = inside[0];
				if (!used[cur]) {
					used[cur] = true;
					AddNewHolecupTri(meshes, tri, tri.m_pVertex[cur], tri.m_pVertex[inside[1]], &clipped[i],
						tri.m_vtx[cur], tri.m_vtx[inside[1]], -1);
					AddNewHolecupTri(meshes, tri, tri.m_pVertex[inside[1]], &clipped[i + 1], &clipped[i],
						tri.m_vtx[inside[1]], -1, -1);
				} else if (cur == firstInside && started) {
					AddNewHolecupTri(meshes, tri, tri.m_pVertex[cur], &clipped[i + 1], &clipped[i],
						tri.m_vtx[cur], -1, -1);
				} else {
					AddNewHolecupTri(meshes, tri, tri.m_pVertex[inside[1]], &clipped[i + 1], &clipped[i],
						tri.m_vtx[inside[1]], -1, -1);
				}
			}
			break;
		}
		}

		prevCount = count;
		for (k = 0; k < count; k++)
			prevInside[k] = inside[k];
	}
}

// polysoup.cpp:4170
void WPolySoup::GenNewHolecupVtx(sHoleVtx *pVtx, sHoleTri *pTri) {
	// The barycentric coordinates of pVtx->m_pos in the triangle, on the XZ plane.
	float dx = pVtx->m_pos.x - pTri->m_pVertex[0]->x;
	float dz = pVtx->m_pos.z - pTri->m_pVertex[0]->z;
	float e1x = pTri->m_pVertex[1]->x - pTri->m_pVertex[0]->x;
	float e1z = pTri->m_pVertex[1]->z - pTri->m_pVertex[0]->z;
	float e2x = pTri->m_pVertex[2]->x - pTri->m_pVertex[0]->x;
	float e2z = pTri->m_pVertex[2]->z - pTri->m_pVertex[0]->z;
	float det = e2z * e1x - e2x * e1z;
	float u = (e2z * dx - e2x * dz) / det;
	float v = (dz * e1x - e1z * dx) / det;

	pVtx->m_pos.y = (pTri->m_pVertex[1]->y - pTri->m_pVertex[0]->y) * u
		+ (pTri->m_pVertex[2]->y - pTri->m_pVertex[0]->y) * v + pTri->m_pVertex[0]->y;

	if (pTri->m_pMesh->normList) {
		pVtx->m_normal.x = (pTri->m_pNormal[1]->x - pTri->m_pNormal[0]->x) * u
			+ (pTri->m_pNormal[2]->x - pTri->m_pNormal[0]->x) * v + pTri->m_pNormal[0]->x;
		pVtx->m_normal.y = (pTri->m_pNormal[1]->y - pTri->m_pNormal[0]->y) * u
			+ (pTri->m_pNormal[2]->y - pTri->m_pNormal[0]->y) * v + pTri->m_pNormal[0]->y;
		pVtx->m_normal.z = (pTri->m_pNormal[1]->z - pTri->m_pNormal[0]->z) * u
			+ (pTri->m_pNormal[2]->z - pTri->m_pNormal[0]->z) * v + pTri->m_pNormal[0]->z;
	}

	// Each byte of the colour, interpolated alone.
	float alpha = (float)(unsigned char)(pTri->m_color[0] >> 24);
	float red = (float)(unsigned char)(pTri->m_color[0] >> 16);
	float green = (float)(unsigned char)(pTri->m_color[0] >> 8);
	float blue = (float)(pTri->m_color[0] & 0xFF);
	pVtx->m_color =
		((unsigned long)(unsigned char)(int)(((unsigned char)(pTri->m_color[1] >> 24) - alpha) * u
			+ ((unsigned char)(pTri->m_color[2] >> 24) - alpha) * v + alpha) << 24)
		| ((unsigned long)(unsigned char)(int)(((unsigned char)(pTri->m_color[1] >> 16) - red) * u
			+ ((unsigned char)(pTri->m_color[2] >> 16) - red) * v + red) << 16)
		| ((unsigned long)(unsigned char)(int)(((unsigned char)(pTri->m_color[1] >> 8) - green) * u
			+ ((unsigned char)(pTri->m_color[2] >> 8) - green) * v + green) << 8)
		| (unsigned char)(int)(((float)(pTri->m_color[1] & 0xFF) - blue) * u
			+ ((float)(pTri->m_color[2] & 0xFF) - blue) * v + blue);

	pVtx->m_uv[0] = (pTri->m_uv[1][0] - pTri->m_uv[0][0]) * u
		+ (pTri->m_uv[2][0] - pTri->m_uv[0][0]) * v + pTri->m_uv[0][0];
	pVtx->m_uv[1] = (pTri->m_uv[1][1] - pTri->m_uv[0][1]) * u
		+ (pTri->m_uv[2][1] - pTri->m_uv[0][1]) * v + pTri->m_uv[0][1];

	if (pTri->m_pMesh->uvBackup) {
		pVtx->m_uvBackup[0] = (pTri->m_uvBackup[1][0] - pTri->m_uvBackup[0][0]) * u
			+ (pTri->m_uvBackup[2][0] - pTri->m_uvBackup[0][0]) * v + pTri->m_uvBackup[0][0];
		pVtx->m_uvBackup[1] = (pTri->m_uvBackup[1][1] - pTri->m_uvBackup[0][1]) * u
			+ (pTri->m_uvBackup[2][1] - pTri->m_uvBackup[0][1]) * v + pTri->m_uvBackup[0][1];
	}
}

// polysoup.cpp:4215
void WPolySoup::AddHolecup(std::vector<sHoleMesh> &meshes, std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris) {
	DigHolecup(cupVtxs, tris);

	for (std::vector<sHoleMesh>::iterator it = meshes.begin(); it != meshes.end(); ++it) {
		ApplyHoleMesh(*it, tris);
	}
}

// polysoup.cpp:4227
void WPolySoup::ApplyHoleMesh(sHoleMesh &mesh, std::vector<sHoleTri> &tris) {
	w_mesh *pMesh = mesh.m_pMesh;
	int vtxNum = pMesh->vtxNum + mesh.m_vtxs.size();

	WVector *pVecList = new WVector[vtxNum];
	WVector *pNormList;
	if (pMesh->normList)
		pNormList = new WVector[vtxNum];
	else
		pNormList = 0;
	unsigned long *pVtxColors = new unsigned long[vtxNum];
	unsigned long *pOrigColors = new unsigned long[vtxNum];
	float (*pUV)[2] = new float[vtxNum][2];
	float (*pUVBackup)[2] = pMesh->uvBackup ? new float[vtxNum][2] : 0;

	int removed = 0;
	for (std::vector<sHoleTri>::iterator tri = tris.begin(); tri != tris.end(); ++tri) {
		if (tri->m_pMesh == pMesh)
			removed++;
	}

	int indexNum = mesh.m_indices.size() - removed * 3 + pMesh->indexNum;
	unsigned short *pIndexList = new unsigned short[indexNum];

	memcpy(pVecList, pMesh->vecList, pMesh->vtxNum * sizeof(WVector));

	if (pNormList)
		memcpy(pNormList, pMesh->normList, pMesh->vtxNum * sizeof(WVector));

	memcpy(pVtxColors, pMesh->vtxColorList, pMesh->vtxNum * sizeof(unsigned long));
	memcpy(pOrigColors, pMesh->originalVtxColorList, pMesh->vtxNum * sizeof(unsigned long));
	memcpy(pUV, pMesh->uvData, pMesh->vtxNum * sizeof(float[2]));
	if (pMesh->uvBackup)
		memcpy(pUVBackup, pMesh->uvBackup, pMesh->vtxNum * sizeof(float[2]));

	for (int i = pMesh->vtxNum; i < vtxNum; i++) {
		sHoleVtx &vtx = mesh.m_vtxs[i - pMesh->vtxNum];

		pVecList[i] = vtx.m_pos;

		if (pNormList)
			pNormList[i] = vtx.m_normal;

		pVtxColors[i] = vtx.m_color;
		pOrigColors[i] = vtx.m_color;
		pUV[i][0] = vtx.m_uv[0];
		pUV[i][1] = vtx.m_uv[1];

		if (pMesh->uvBackup) {
			pUVBackup[i][0] = vtx.m_uvBackup[0];
			pUVBackup[i][1] = vtx.m_uvBackup[1];
		}
	}

	std::vector<sHoleTri>::iterator it;
	for (it = tris.begin(); it != tris.end(); ++it) {
		if (it->m_pMesh == pMesh)
			break;
	}

	if (it != tris.end()) {
		// keep the indices before the first removed triangle
		if (it->m_index > 0)
			memcpy(pIndexList, pMesh->indexList, it->m_index * sizeof(unsigned short));

		int prev = it->m_index;
		int dst = prev;

		// and those between the removed triangles (sorted by index)
		for (++it; it != tris.end(); ++it) {
			if (it->m_pMesh != pMesh)
				break;

			int gap = it->m_index - prev;
			if (gap > 3) {
				memcpy(&pIndexList[dst], &pMesh->indexList[prev + 3], (gap - 3) * sizeof(unsigned short));
				dst += it->m_index - prev - 3;
			}
			prev = it->m_index;
		}

		int gap = pMesh->indexNum - prev;
		if (gap > 3) {
			memcpy(&pIndexList[dst], &pMesh->indexList[prev + 3], (gap - 3) * sizeof(unsigned short));
			dst += pMesh->indexNum - prev - 3;
		}

		// then the hole mesh's own triangles
		for (int k = 0; dst < indexNum; dst++, k++)
			pIndexList[dst] = (unsigned short)mesh.m_indices[k];
	}

	delete[] pMesh->vecList;
	delete[] pMesh->normList;
	delete[] pMesh->vtxColorList;
	delete[] pMesh->originalVtxColorList;
	delete[] pMesh->uvData;
	if (pMesh->uvBackup)
		delete[] pMesh->uvBackup;
	delete[] pMesh->indexList;

	pMesh->vtxNum = vtxNum;
	pMesh->vecList = pVecList;
	pMesh->normList = pNormList;
	pMesh->vtxColorList = pVtxColors;
	pMesh->originalVtxColorList = pOrigColors;
	pMesh->indexNum = indexNum;
	pMesh->uvData = pUV;
	pMesh->uvBackup = pUVBackup;
	pMesh->indexList = pIndexList;
}

// polysoup.cpp:4353
// Appends to the background's root bone a mesh for the inside of the hole
// cup: a wall strip down from the rim (cupVtxs) to one unit below it, and a
// floor quad, textured with holecup.dds and lit by g_lightset.
void WPolySoup::DigHolecup(std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris) {
	FindNewHolecupVtxs(cupVtxs, tris);

	w_mesh *pMesh = m_pBgModel->m_pPuppet->GetRootBone()->GetMesh();

	WVector up(0.0f, 1.0f, 0.0f);
	if (!pMesh)
		return;

	while (pMesh->next)
		pMesh = pMesh->next;

	w_mesh *pCup = new w_mesh;
	memset(pCup, 0, sizeof(w_mesh));
	pMesh->next = pCup;

	if (!m_holecupTexture)
		m_holecupTexture = g_resrcmng->LoadTexture("holecup.dds", 0, 0, 0);
	pCup->texHandle = m_holecupTexture;
	pCup->drawFlag = (m_holecupTexture & 0x7FF) | 0x8000000;
	pCup->diffuse = 0xFFFFFF;
	pCup->alpha = 1.0f;

	int numCupVtxs = cupVtxs.size();
	pCup->vtxNum = numCupVtxs * 2 + 4;
	pCup->indexNum = numCupVtxs * 6;
	pCup->vecList = new WVector[pCup->vtxNum];
	pCup->normList = new WVector[pCup->vtxNum];
	pCup->vtxColorList = new unsigned long[pCup->vtxNum];
	pCup->originalVtxColorList = new unsigned long[pCup->vtxNum];
	pCup->uvData = new float[pCup->vtxNum][2];
	pCup->uvBackup = 0;
	pCup->indexList = new unsigned short[pCup->indexNum];

	float radius = GolfBall().m_radius * 3.0f;

	if (IsLocalContent((localContentType_t)0x3A)) {
		CSharedDoc *pDoc = WSingleton<CSharedDoc>::Instance();
		if (pDoc->m_roomType != 0x0B && pDoc->m_roomType != 0x0C && (pDoc->m_currentChannel.flags & 0x800)) {
			radius = GolfBall().m_radius * 6.0f;
		}
	}

	WMatrix inv = ~m_pBgModel->m_pPuppet->GetRootBone()->GetMatrix();
	WVector center = GOLFDOC()->GetHoleData().m_cupPos * inv;

	float bottom = cupVtxs.begin()->m_pos.y - 1.0f;

	// The light direction in the bone's space (rotation only), reversed.
	const WVector &light = g_lightset.nearone;
	WVector toLight;
	toLight.x = (light.z * inv.za.x + light.y * inv.ya.x + light.x * inv.xa.x) * -1.0f;
	toLight.y = (light.z * inv.za.y + light.y * inv.ya.y + light.x * inv.xa.y) * -1.0f;
	toLight.z = (light.z * inv.za.z + light.y * inv.ya.z + light.x * inv.xa.z) * -1.0f;
	toLight.Normalize();

	int i;
	for (i = 0; i < numCupVtxs; i++) {
		WVector &rim = cupVtxs[i].m_pos;
		WVector dir;
		dir.x = center.x - rim.x;
		dir.y = 0.0f;
		dir.z = center.z - rim.z;
		dir.Normalize();

		pCup->vecList[i * 2 + 1] = rim;
		pCup->vecList[i * 2] = pCup->vecList[i * 2 + 1];
		pCup->vecList[i * 2 + 1].y = bottom;
		pCup->normList[i * 2 + 1] = dir;
		pCup->normList[i * 2] = dir;

		float angle;
		if (i == numCupVtxs - 1) {
			angle = 6.2831855f;
		} else {
			float cosine = -dir.x;
			if (cosine < -1.0f)
				cosine = -1.0f;
			else if (cosine > 1.0f)
				cosine = 1.0f;
			angle = acosf(cosine);
			if (dir.z < 0.0f)
				angle = 6.2831855f - angle;
		}
		float u = angle * 0.47746483f;

		pCup->uvData[i * 2][0] = u;
		pCup->uvData[i * 2][1] = 0.015625f;
		pCup->uvData[i * 2 + 1][0] = u;
		pCup->uvData[i * 2 + 1][1] = 0.90625f;

		float lit = (toLight.z * dir.z + toLight.x * dir.x + toLight.y * dir.y) * 255.0f;
		unsigned long color;
		if (lit > 0.0f)
			color = AddDiffuse(g_lightset.ambient, g_lightset.diffuse, (unsigned char)lit);
		else
			color = g_lightset.ambient;
		color |= 0xFF000000;

		pCup->originalVtxColorList[i * 2 + 1] = color;
		pCup->originalVtxColorList[i * 2] = color;
		pCup->vtxColorList[i * 2 + 1] = color;
		pCup->vtxColorList[i * 2] = color;
	}

	// The floor quad, a square of 2 * radius around the cup's centre.
	static const float corners[4][2] = {
		{ -1.0f, 1.0f }, { 1.0f, 1.0f }, { -1.0f, -1.0f }, { 1.0f, -1.0f }
	};
	int floor = numCupVtxs * 2;
	unsigned long floorColor = g_lightset.ambient | 0xFF000000;
	for (int k = 0; k < 4; k++) {
		WVector &pos = pCup->vecList[floor + k];
		pos.x = center.x + corners[k][0] * radius;
		pos.y = center.y + radius * 0.0f;
		pos.z = center.z + corners[k][1] * radius;
		pos.y = bottom;
		pCup->normList[floor + k] = up;
		pCup->uvData[floor + k][0] = 0.0f;
		pCup->uvData[floor + k][1] = 0.96875f;
		pCup->originalVtxColorList[floor + k] = floorColor;
		pCup->vtxColorList[floor + k] = floorColor;
	}

	// The wall's quads, then the floor's in the last slot.
	for (i = 0; i < numCupVtxs - 1; i++) {
		int base = i * 2;
		pCup->indexList[i * 6 + 0] = base;
		pCup->indexList[i * 6 + 1] = base + 2;
		pCup->indexList[i * 6 + 2] = base + 1;
		pCup->indexList[i * 6 + 3] = base + 1;
		pCup->indexList[i * 6 + 4] = base + 2;
		pCup->indexList[i * 6 + 5] = base + 3;
	}
	pCup->indexList[i * 6 + 0] = floor;
	pCup->indexList[i * 6 + 1] = floor + 1;
	pCup->indexList[i * 6 + 2] = floor + 2;
	pCup->indexList[i * 6 + 3] = floor + 2;
	pCup->indexList[i * 6 + 4] = floor + 1;
	pCup->indexList[i * 6 + 5] = floor + 3;

	WBone::CalcMeshAABB(*m_pBgModel->m_pPuppet->GetRootBone(), *pCup);
}

// polysoup.cpp:4506
void WPolySoup::FindNewHolecupVtxs(std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris) {
	for (std::vector<sHoleTri>::iterator tri = tris.begin(); tri != tris.end(); ++tri) {
		std::vector<sCupVtx>::iterator prev = cupVtxs.begin();

		bool prevIn = IsInTri(prev->m_pos, tri->m_pVertex);

		for (std::vector<sCupVtx>::iterator cur = prev + 1; cur != cupVtxs.end(); ++cur) {
			bool curIn = IsInTri(cur->m_pos, tri->m_pVertex);

			if (prevIn != curIn) {
				for (int i = 0; i < 3; i++) {
					sCupVtx vtx;

					if (GetIntersection(&vtx.m_pos, &prev->m_pos, &cur->m_pos, tri->m_pVertex[i], tri->m_pVertex[(i + 1) % 3], &tri->m_edge[i])) {
						vtx.m_pMesh = tri->m_pMesh;
						vtx.m_index = tri->m_index;
						cur = cupVtxs.insert(cur, vtx);
						++cur;
						break;
					}
				}
			}

			prev = cur;
			prevIn = curIn;
		}
	}

	for (std::vector<sCupVtx>::iterator it = cupVtxs.begin(); it != cupVtxs.end(); ++it) {
		sHoleVtx holeVtx;

		holeVtx.m_pos = it->m_pos;

		for (std::vector<sHoleTri>::iterator tri = tris.begin(); tri != tris.end(); ++tri) {
			if (tri->m_pMesh == it->m_pMesh && tri->m_index == it->m_index) {
				GenNewHolecupVtx(&holeVtx, &*tri);
				it->m_pos.y = holeVtx.m_pos.y;
				break;
			}
		}
	}
}

// polysoup.cpp:4585
WPolySoup::CStaticPetGrp::CStaticPetGrp() {
	m_numGroups = 0;
	m_ppGroups = 0;
}

// polysoup.cpp:4589
WPolySoup::CStaticPetGrp::~CStaticPetGrp() {
	Clear();
}

// polysoup.cpp:4597
// Groups the consecutive top-level models sharing a puppet name into one
// static puppet group each, then registers the groups with the scene manager.
void WPolySoup::CStaticPetGrp::Build(std::vector<sModel*> models, int option) {
	Clear();

	const char *prevName = "";
	int numGroups = 0;
	m_numGroups = 0;
	for (int i = 0; i < option; i++) {
		if (models[i]->m_pPuppet != 0 && models[i]->m_subOrder == 0 && _strcmpi(models[i]->m_name, prevName) != 0) {
			prevName = models[i]->m_name;
			numGroups++;
		}
	}

	if (numGroups == 0) {
		return;
	}

	m_ppGroups = new WxStaticPuppetGrp*[numGroups];
	memset(m_ppGroups, 0, numGroups * sizeof(WxStaticPuppetGrp*));

	prevName = "";
	int group = 0;
	int index = 0;
	while (group < numGroups) {
		int count = 1;
		sModel **ppModel = &models[index];
		if ((*ppModel)->m_pPuppet != 0 && (*ppModel)->m_subOrder == 0 && _strcmpi((*ppModel)->m_name, prevName) != 0) {
			prevName = (*ppModel)->m_name;
			for (int j = index + 1; j < option; j++) {
				if ((*ppModel)->m_subOrder != 0 || _strcmpi(models[j]->m_name, prevName) != 0) {
					break;
				}
				count++;
			}

			WPuppet **ppPuppets = new WPuppet*[count];
			for (int k = 0; k < count; k++) {
				ppPuppets[k] = ppModel[k]->m_pPuppet;
			}

			m_ppGroups[group] = g_resrcmng->xGetStaticPuppetGrp(count, (*ppModel)->m_name, ppPuppets);
			index += count;
			group++;
			delete[] ppPuppets;
		} else {
			index++;
		}
	}

	WSingleton<CSceneManager>::Instance()->RegisterElement((eObjType)2, 0, this, "StaticPetGrp", 2, 0);
	m_numGroups = numGroups;
}

// polysoup.cpp:4718
void WPolySoup::CStaticPetGrp::Clear(void) {
	for (int i = 0; i < m_numGroups; i++) {
		g_resrcmng->Release(m_ppGroups[i]);
	}
	if (m_ppGroups) {
		delete[] m_ppGroups;
		m_ppGroups = 0;
	}
	m_numGroups = 0;
}

// polysoup.cpp:4729
void WPolySoup::CStaticPetGrp::Display(void) {
	g_view->xSetLight(0, g_lightset);

	for (int i = 0; i < m_numGroups; i++) {
		if (m_ppGroups[i]) {
			m_ppGroups[i]->xUpdateCullFlag(g_view);
			m_ppGroups[i]->xRender(g_view);
		}
	}
}

// polysoup.cpp:4745
void WPolySoup::UpdateBright(void) {
	if (m_pBgModel->m_pPuppet != 0) {
		m_pBgModel->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);
	}

	for (int i = 0; i < m_numObjects; i++) {
		if (m_models[i]->m_pPuppet != 0) {
			m_models[i]->m_pPuppet->UpdateLightSource(&g_lightset, false, 0);
		}
	}
}

// polysoup.cpp:4762
void WPolySoup::ChangePointType(unsigned char from, unsigned char to) {
	sPoint *pPoint = m_pPoints;
	unsigned int num = m_numPoints;

	for (unsigned int i = 0; i < num; i++, pPoint++) {
		if (pPoint && pPoint->m_type == from) {
			pPoint->m_type = to;
			return;
		}
	}
}

// polysoup.cpp:4778
const std::vector<WPolySoup::sTriangle*> *WPolySoup::GetTriArray(int index) const {
	std::map<int, std::vector<sTriangle*> >::const_iterator it = m_triArrays.find(index);
	if (it == m_triArrays.end()) {
		return 0;
	}
	return &it->second;
}

// polysoup.h (no line marks: out-of-line, destroys m_desc only)
WPolySoup::sPoint::~sPoint() {
}
