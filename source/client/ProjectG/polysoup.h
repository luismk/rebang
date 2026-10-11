// Original: d:\build\custom\temp\code\client\projectg\polysoup.h
#ifndef PG_CLIENT_PROJECTG_POLYSOUP_H
#define PG_CLIENT_PROJECTG_POLYSOUP_H

#include <list>
#include <map>
#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "nodecontainer.h"
#include "wmath.h"
#include "wresource.h"
#include "scenemanager.h"
#include "wscene.h"

class WPuppet;
class WBone;
class CGolfDoc;
class WxStaticPuppetGrp;
class CFxSpray;
class CFxSequence;
class cFile;
struct w_bound_box;
struct w_mesh;
struct sProperty;

// An ambient sound box of the soup (WPolySoup::m_soundBoxes; size 0x5C from
// the list node std::list<sSoundBox>::_Buynode allocates, 0x64). Played by
// CAmbientSound. The original header is not known.
struct sSoundBox {
	char m_name[0x20];                    // 0x00, the sound name (IsPlaying/Play)
	float m_elapsed;                      // 0x20, time since the last play (CAmbientSound::OnProcess)
	float m_interval;                     // 0x24, tested > 0 by CAmbientSound
	float m_range;                        // 0x28, the fade range outside m_box (GetVolume)
	Wobb m_box;                           // 0x2C, the box the sound is heard in
};
CHECK_SIZE(sSoundBox, 0x5C);
CHECK_FIELD(sSoundBox, m_name, 0x00);
CHECK_FIELD(sSoundBox, m_elapsed, 0x20);
CHECK_FIELD(sSoundBox, m_interval, 0x24);
CHECK_FIELD(sSoundBox, m_range, 0x28);
CHECK_FIELD(sSoundBox, m_box, 0x2C);

// The collision geometry of a course or of a room. Only the members this
// project's code uses so far are declared; the data members are public until
// the class's accessors are known. A WResource: the resource manager at
// 0x28 (GetResrcManager) opens the map file in ReloadBasePet.
class WPolySoup : public WResource {
public:
	// A vertex of the hole cup's rim (IsInPoly strides 0x14).
	struct sCupVtx {
		WVector m_pos;                // 0x00
		w_mesh *m_pMesh;              // 0x0C, the hole triangle it lies in (FindNewHolecupVtxs, against sHoleTri)
		int m_index;                  // 0x10
	};

	// A vertex GenNewHolecupVtx builds in a hole triangle (a 0x2C local of
	// FindNewHolecupVtxs).
	struct sHoleVtx {
		WVector m_pos;                // 0x00
		WVector m_normal;             // 0x0C (ApplyHoleMesh)
		unsigned long m_color;        // 0x18
		float m_uv[2];                // 0x1C
		float m_uvBackup[2];          // 0x24
	};

	// A triangle of a mesh the hole cup is dug into (GenHolecupTri fills it;
	// CreateNewHolecupMeshes strides 0x8C).
	struct sHoleTri {
		w_mesh *m_pMesh;              // 0x00
		int m_index;                  // 0x04, its first index in the mesh's indexList
		int m_vtx[3];                 // 0x08, its vertices
		WVector *m_pVertex[3];        // 0x14, their positions
		WVector *m_pNormal[3];        // 0x20, their normals in the mesh's normList
		float m_uv[3][2];             // 0x2C
		float m_uvBackup[3][2];       // 0x44, copied when the mesh keeps a uvBackup
		unsigned long m_color[3];     // 0x5C
		WVector m_edge[3];            // 0x68, the XZ line of each edge (normal, then -distance in z)
	};

	// An input fragment of a shadow-map pack: a triangle of a background
	// mesh with its shadow-map vertices and UVs (smAddSmapPack reads them
	// from the .sbin file, 0x10 bytes each).
	struct SmInFrag {
		unsigned char m_meshIndex;    // 0x00, its mesh in ppMeshes
		UNKNOWN(0x01, 0x02);
		unsigned short m_triIndex;    // 0x02, its triangle in the mesh's indexList
		unsigned short m_vtx[3];      // 0x04, a vertex of pVtx, or 0x8000 + a vertex of the mesh
		unsigned short m_uv[3];       // 0x0A, its UV in pUV
	};

	// The input fragments of a shadow-map case (smAddSmapPack fills them).
	struct SmInFragList {
		// polysoup.h:399
		SmInFragList() {
			m_numFrags = 0;
			m_pFrags = 0;
		}
		// polysoup.h:401
		~SmInFragList() {
			if (m_pFrags) {
				delete[] m_pFrags;
				m_pFrags = 0;
			}
		}

		int m_numFrags;               // 0x00, the fragments of m_pFrags (smAddSmapPack)
		SmInFrag *m_pFrags;           // 0x04, new[] m_numFrags (smAddSmapPack)
	};

	// A shadow-map case of the .sbin file (LoadShadowmapInfo keeps two):
	// its lists of input fragments (smLoadSmapMeshes).
	struct SmCase {
		// polysoup.h:411
		SmCase() {
			m_numLists = 0;
			m_pLists = 0;
		}
		// polysoup.h:413
		~SmCase() {
			if (m_pLists) {
				delete[] m_pLists;
				m_pLists = 0;
			}
		}

		int m_numLists;               // 0x00, read from the file (smLoadSmapMeshes)
		SmInFragList *m_pLists;       // 0x04, new[] m_numLists
	};

	// An output fragment of the .sbin file (smLoadOutFrags), sorted by
	// smCompareOutFrag on its triangle.
	struct SmOutFrag {
		unsigned short m_triIndex;    // 0x00, its triangle in the mesh's indexList
		unsigned short m_vtx[3];      // 0x02, as SmInFrag::m_vtx
	};

	// A collision triangle of the soup (0x48 bytes).
	struct sTriangle {
		WVector m_vertex[3];          // 0x00
		WVector m_normal;             // 0x24
		float m_d;                    // 0x30, the plane's distance (dot(normal, point) + m_d = 0)
		bool m_doubleSided;           // 0x34, both faces collide (GetMeshNormal, ClipDecalTriangle)
		UNKNOWN(0x35, 0x38);
		int m_texIndex;               // 0x38, its texture in m_pTextures
		int m_propIndex;              // 0x3C, its property (GetProperty)
		UNKNOWN(0x40, 0x48);
	};

	// A mesh the hole cup is dug into (AddHolecup strides 0x24).
	struct sHoleMesh {
		w_mesh *m_pMesh;              // 0x00, the mesh it extends
		std::vector<sHoleVtx> m_vtxs; // 0x04, vertices appended after the mesh's own
		std::vector<int> m_indices;   // 0x14, indices appended after the kept triangles
	};

	// A collision quad of a model (sModel::m_pQuads, deleted with delete[];
	// size 0x10 from the delete[] in DeleteDataStructure).
	struct sQuad {
		// polysoup.h:158
		sQuad() {
			m_count = 0;
			m_pList = 0;
			m_count2 = 0;
			m_pList2 = 0;
		}
		// polysoup.h:168
		~sQuad() {
			m_count = 0;
			if (m_pList) {
				delete[] m_pList;
				m_pList = 0;
			}
			m_count2 = 0;
			if (m_pList2) {
				delete[] m_pList2;
				m_pList2 = 0;
			}
		}

		short m_count;                // 0x00, the entries of m_pList (CQuadTree::GetMeshList)
		int *m_pList;                 // 0x04
		unsigned char m_count2;       // 0x08, the entries of m_pList2 (cleared with it by the destructor; no reader found yet)
		int *m_pList2;                // 0x0C, a second list deleted with delete[] (element type not known)
	};

	// A model placed in the soup. Its constructor (polysoup.h:191) is emitted
	// by realmyroomdata.cpp; the size is not known yet (at least 0x10C).
	struct sModel {
		sModel();

		unsigned char m_subOrder;     // 0x00, CompareModel's third key
		unsigned char m_order;        // 0x01, CompareModel's second key
		unsigned char m_reservedByte; // 0x02, cleared by the constructor; the center box's "*nocol" option is scanned into it with %d (ProcessPostLoadPet)
		unsigned char m_alpha;        // 0x03, GetObjAlpha
		float m_windFactor;           // 0x04, GetObjWindFactor
		float m_scale;                // 0x08, the "pet_scale" define of its effects (ApplyFx)
		WPuppet *m_pPuppet;           // 0x0C
		WMatrix m_matrix;             // 0x10, pivot = position
		char m_name[0x20];            // 0x40, scene name of the puppet
		Waabb m_aabb;                 // 0x60, its world box (picked with RayIntersectAABB)
		Waabb m_sceneAabb;            // 0x78, the box registered with the scene manager (RegisterElement)
		Wobb m_obb;                   // 0x90, its oriented box (picked with IsInclude)
		WVector m_center;             // 0xC0, the center of m_aabb (Load)
		float m_radius;               // 0xCC, half the diagonal of m_aabb (Load)
		WVector m_basePos;            // 0xD0, its position before an edit (SetTargetPosition)
		sTriangle *m_pTriangles;      // 0xDC, its collision triangles
		sQuad *m_pQuads;              // 0xE0, its collision quads (new[] in Load, delete[] by Clear/DeleteDataStructure)
		UNKNOWN(0xE4, 0xE5);
		unsigned char m_reservedFlag; // 0xE5, only cleared by the constructor (no reader found)
		char m_baseName[0x20];        // 0xE6, read from the file record (Load); an addition model takes its base model's (SetAdditionModel)
		UNKNOWN(0x106, 0x108);
		int m_objIndex;               // 0x108, its object index in the quadtree (CreateDataModel)
	};

	// A texture of the soup (m_pTextures strides 0x28).
	struct sTexture {
		// polysoup.h:106
		sTexture() {
			m_handle = 0;
			m_param = 0;
		}

		char m_name[0x20];            // 0x00 (assumed)
		int m_handle;                 // 0x20, released by ClearOldTextures
		int m_param;                  // 0x24, zeroed by the constructor (no reader known yet)
	};

	// A named camera of the course (FindCamera strides 0x58).
	struct sCamera {
		char m_name[0x20];            // 0x00, FindCamera (the size is assumed)
		WMatrix m_matrix;             // 0x20, its position is the pivot (0x44)
		float m_fov;                  // 0x50
		float m_param;                // 0x54, read from the file after m_fov (Load; role not known)
	};

	struct sArea;

	// An effect placed in the background (UpdateBgFxWind strides 0x48).
	struct sBgFx {
		sBgFx() {}

		sArea *m_pArea;               // 0x00, the type-6 area that places it (Load)
		CFxSpray *m_pSpray;           // 0x04, blown by the wind at its position
		CFxSequence *m_pSequence;     // 0x08, blown by the wind at its position
		WMatrix m_matrix;             // 0x0C, rotated by the "*fx" angles, pivot = the area's center (the Attacher's matrix)
		WVector m_offset;             // 0x3C, zero (the Attacher's offset)
	};

	// A named point of the course (FindPoint strides 0x4C).
	struct sPoint {
		~sPoint();

		unsigned char m_type;         // 0x00, ChangePointType
		char m_name[0x20];            // 0x01, FindPoint
		UNKNOWN(0x21, 0x24);
		WVector m_pos;                // 0x24
		std::string m_desc;           // 0x30, the destructor's only member
	};

	// An area of the course (GetArea strides 0x5C). Its default
	// constructor is compiler-generated.
	struct sArea {
		unsigned char m_type;         // 0x00, what the area is (1: an NPC area, CNPC::OnInit; 4: a wind area, CWind::MakeWindList; 5: an ambient sound box, Load)
		char m_name[0x40];            // 0x01, FindArea (Load reads 0x40 bytes)
		UNKNOWN(0x41, 0x44);
		Waabb m_aabb;                 // 0x44, reset by the constructor
	};

	// Three counters the constructor zeroes, last first (two of them follow
	// the texture map); their use is not decompiled yet.
	struct sCounterTriple {
		sCounterTriple() { Reset(); }
		void Reset(void) { m_first = m_second = m_third = 0; }

		int m_first;                  // 0x00
		int m_second;                 // 0x04
		int m_third;                  // 0x08
	};

	// A named polygon's header in the soup file (Load reads 0x18 bytes).
	struct sBoundAreaRecord {
		char m_name[0x10];            // 0x00
		int m_numPoints;              // 0x10, the WVectors that follow
		int m_reserved;               // 0x14
	};

	// A model record of the soup file (Load reads 0xAC bytes).
	struct sModelRecord {
		unsigned char m_flags;        // 0x00, low 3 bits: sModel::m_subOrder; the others: m_order
		UNKNOWN(0x01, 0x08);
		char m_name[0x20];            // 0x08
		Waabb m_aabb;                 // 0x28
		Waabb m_sceneAabb;            // 0x40
		WMatrix m_matrix;             // 0x58
		int m_kind;                   // 0x88, 1 or 2: kept only for the course kind 1 or 2
		char m_baseName[0x20];        // 0x8C
	};

	// The named polygons of the course are CNodeContainers; the old name is
	// kept for golfbg.cpp until it uses CNodeContainer.
	typedef CNodeContainer sBoundArea;

	// The T&L puppet groups of the static models (xBuildTnLBuffers).
	class CStaticPetGrp : public CRenderFuncPtr {
		LAYOUT_CHECK_FRIEND;
	public:
		CStaticPetGrp();
		~CStaticPetGrp();
		void Clear(void);
		virtual void Display(void);
		void Build(std::vector<sModel*> models, int option);

	private:
		int m_numGroups;                      // 0x4
		WxStaticPuppetGrp **m_ppGroups;       // 0x8
	};

	bool Load(const char *name, bool bPlaceHoleCup);
	void Clear(void);
	void SetRenderMode(int mode);
	void UpdateBgFxWind(void);
	void ChangeDataStructure(sModel *pModel);
	void CreateDataModel(sModel *pModel);
	sModel *FindModel(const char *name);
	sCamera *FindCamera(const char *name);
	sCamera *GetCameraByName(const char *name);
	void MakeDataStructure(void);
	const sProperty &GetProperty(int index);
	int GetPropertyIndex(const char *name);
	sPoint *FindPoint(const char *name);
	sArea *FindArea(const char *name);
	sArea *GetAreaByPos(const WVector &pos);
	void EndLoad(bool bPlaceHoleCup);
	w_bound_box *GetObjBoundBox(int index, const char *name);
	int GetObjAlpha(int index);
	float GetObjWindFactor(int index);
	void ChangePointType(unsigned char from, unsigned char to);
	const std::vector<sTriangle*> *GetTriArray(int index) const;
	bool IsResisteredBgSequence(const std::string &name);
	void ChangeFloorTexture(const char *oldName, const char *newName);
	bool IsOutOfBound(const WVector &pos);
	WVector GetCameraPos(const WVector &pos, const WVector &dir);
	void UpdateBright(void);
	bool DeleteDataStructure(sModel *pModel);
	void ChangeObjectTexture(const char *oldName, const char *newName, int index);
	void EnableShadowmap(bool enable);
	CNodeContainer *FindNode(const char *name);
	bool LoadMapCheckData(unsigned char hole);
	void MakeStatistics(void);

	WPolySoup(CGolfDoc *pDoc);

	// polysoup.h:336
	int GetAreaNum(void) { return m_numAreas; }
	// polysoup.h:350
	sArea *GetArea(int index) { return &m_pAreas[index]; }

protected:
	void ProcessPostLoadPet(sModel *pModel);
	void ApplyFx(sModel *pModel, WBone *pBone, const char *name);
	bool IsPointInPoly(const std::vector<WVector> &points, const WVector &pos);
	float GetCameraRadius(WVector a, WVector b, WVector c);
	bool IsClockWise(const std::vector<WVector> &points, const WVector &pos);
	WVector PosInWorld(const std::vector<WVector> &points, const WVector &pos, const WVector &dir);

private:
	static int __cdecl smCompareOutFrag(const void *a, const void *b);
	static int __cdecl CompareModel(const void *a, const void *b);
	void ClearOldTextures(void);
	unsigned char OverlapCamera(int index);
	void ReloadBasePet(void);
	bool LoadShadowmapInfo(void);
	void smLoadSmapMeshes(SmCase *pCases, cFile *pFile, w_mesh **ppMeshes, WVector *pVtx, float (*pUV)[2]);
	w_mesh *smAddSmapPack(SmInFragList *pList, cFile *pFile, int kind, int index, w_mesh **ppMeshes, WVector *pVtx, float (*pUV)[2]);
	void smLoadOutFrags(cFile *pFile, int numMeshes, w_mesh **ppMeshes, WVector *pVtx, SmCase *pCases);
	void smBuildOutFragMeshes(w_mesh *pMesh, SmOutFrag *pFrags, int numFrags, WVector *pVtx);
	void BackupTextures(void);
	void PlaceHoleCup(void);
	void xBuildTnLBuffers(void);
	bool IsInTri(WVector &pos, WVector *tri[]);
	bool IsInPoly(WVector &pos, std::vector<sCupVtx> &verts);
	void GatherHolecupTris(std::vector<sHoleTri> &tris, std::vector<sCupVtx> &cupVtxs, WVector &center);
	bool AreCrossed(WVector *a, WVector *b, WVector *c, WVector *d);
	bool GetIntersection(WVector *out, WVector *a, WVector *b, WVector *c, WVector *d, WVector *line);
	void ApplyHoleMesh(sHoleMesh &mesh, std::vector<sHoleTri> &tris);
	void DigHolecup(std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris);
	void GenHolecupTri(sHoleTri *pTri, w_mesh *pMesh, int index);
	void GenNewHolecupVtx(sHoleVtx *pVtx, sHoleTri *pTri);
	void FindNewHolecupVtxs(std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris);
	int ClipHolecup(std::vector<WVector> &clipped, sHoleTri &tri, std::vector<sCupVtx> &cupVtxs);
	void AddNewHolecupTri(std::vector<sHoleMesh> &meshes, sHoleTri &tri, WVector *pV0, WVector *pV1, WVector *pV2, int vtx0, int vtx1, int vtx2);
	void BuildNewHolecupMesh(std::vector<sHoleMesh> &meshes, sHoleTri &tri, std::vector<WVector> &clipped);
	void CreateNewHolecupMeshes(std::vector<sHoleMesh> &meshes, std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris);
	void AddHolecup(std::vector<sHoleMesh> &meshes, std::vector<sCupVtx> &cupVtxs, std::vector<sHoleTri> &tris);

public:
	std::vector<CNodeContainer> m_boundAreas; // 0x2C, the named polygons (IsOutOfBound, FindNode)
	char m_mapName[0x40];                 // 0x3C, the map file (Load: strncpy 0x3F; ReloadBasePet: GetCFile, strstr "rank")
	int m_numOtherRecords[5];             // 0x7C, the file header's first words (zeroed with the counts up to 0xA4 by the constructor): [1] the version (0x71), [2..4] the object counts of the course kinds 0, 1 and 2 (Load)
	int m_numCameras;                     // 0x90
	int m_numPoints;                      // 0x94
	int m_numAreas;                       // 0x98
	int m_numTextures;                    // 0x9C
	int m_numBoundAreas;                  // 0xA0, the named polygons in the file (Load)
	int m_numObjects;                     // 0xA4, the objects MakeDataStructure enters into the quadtree (also passed to CStaticPetGrp::Build)
	int m_numStructuredObjects;           // 0xA8, set to m_numObjects by MakeDataStructure
	sTexture *m_pTextures;                // 0xAC
	int m_numOldTextures;                 // 0xB0, BackupTextures
	sTexture *m_pOldTextures;             // 0xB4, BackupTextures
	sCamera *m_pCameras;                  // 0xB8
	sArea *m_pAreas;                      // 0xBC
	sPoint *m_pPoints;                    // 0xC0
	std::vector<sModel*> m_models;        // 0xC4
	sModel *m_pBgModel;                   // 0xD4, the model of index 0xFFFF
	std::map<int, int> m_texIndexMap;     // 0xD8, texture id -> index in m_pTextures (FindTexture)
	float m_defaultNear;                  // 0xE4, 800.0f from the constructor (role assumed)
	float m_defaultFar;                   // 0xE8, 960.0f from the constructor (role assumed)
	sCounterTriple m_counters1;           // 0xEC
	sCounterTriple m_counters2;           // 0xF8
	CGolfDoc *m_pGolfDoc;                 // 0x104
	bool m_bPendingOption;                // 0x108, cleared by the constructor (no reader decompiled yet)
	bool m_bUseTnL;                       // 0x109, xBuildTnLBuffers
	UNKNOWN(0x10A, 0x10C);
	CStaticPetGrp m_staticPetGrp;         // 0x10C
	sBgFx m_bgFx[0x100];                  // 0x118
	int m_numBgFx;                        // 0x4918
	bool m_bAltCameraBounds;              // 0x491C, GetCameraPos keeps the camera in the type-6 bounds instead of type 1
	UNKNOWN(0x491D, 0x4920);
	std::list<LightSet> m_lightSets;      // 0x4920, std::list<LightSet> _Buynode in the constructor
	std::vector<unsigned long> m_ulongList; // 0x492C, its unwind destroys a std::vector<unsigned long>
	std::vector<int> m_intList;           // 0x493C, its unwind destroys a std::vector<int>
	std::list<sSoundBox> m_soundBoxes;    // 0x494C, the ambient sound boxes (CAmbientSound; std::list<sSoundBox> _Buynode/_Tidy in the ctor/dtor)
	UNKNOWN(0x4958, 0x495C);
	int m_numSoundBoxRefs;                // 0x495C, zeroed by the constructor body (polysoup.cpp:91)
	int m_numSmapMeshes;                  // 0x4960, the meshes of m_pSmapMeshes (smLoadSmapMeshes)
	int *m_pSmapDrawFlags;                // 0x4964, each shadow-map mesh's drawFlag & 0x7FF (smLoadSmapMeshes)
	w_mesh *m_pSmapMeshes;                // 0x4968, the shadow-map meshes (smLoadSmapMeshes), a list linked by w_mesh::next
	bool m_bNoShadowmap;                  // 0x496C, set when LoadShadowmapInfo fails; EnableShadowmap then reloads nothing
	UNKNOWN(0x496D, 0x4970);
	int m_holecupTexture;                 // 0x4970, holecup.dds, loaded by DigHolecup (zeroed by the constructor)
	std::map<int, std::vector<sTriangle*> > m_triArrays;    // 0x4974, GetTriArray
};

CHECK_FIELD(WPolySoup::sModel, m_subOrder, 0x00);
CHECK_FIELD(WPolySoup::sModel, m_order, 0x01);
CHECK_FIELD(WPolySoup::sModel, m_reservedByte, 0x02);
CHECK_FIELD(WPolySoup::sModel, m_alpha, 0x03);
CHECK_FIELD(WPolySoup::sModel, m_windFactor, 0x04);
CHECK_FIELD(WPolySoup::sModel, m_scale, 0x08);
CHECK_FIELD(WPolySoup::sModel, m_pPuppet, 0x0C);
CHECK_FIELD(WPolySoup::sModel, m_matrix, 0x10);
CHECK_FIELD(WPolySoup::sModel, m_name, 0x40);
CHECK_FIELD(WPolySoup::sModel, m_aabb, 0x60);
CHECK_FIELD(WPolySoup::sModel, m_sceneAabb, 0x78);
CHECK_FIELD(WPolySoup::sModel, m_obb, 0x90);
CHECK_FIELD(WPolySoup::sModel, m_basePos, 0xD0);
CHECK_FIELD(WPolySoup::sModel, m_pTriangles, 0xDC);
CHECK_FIELD(WPolySoup::sModel, m_pQuads, 0xE0);
CHECK_FIELD(WPolySoup::sModel, m_reservedFlag, 0xE5);
CHECK_FIELD(WPolySoup::sModel, m_baseName, 0xE6);
CHECK_FIELD(WPolySoup::sModel, m_objIndex, 0x108);
CHECK_SIZE(WPolySoup::sQuad, 0x10);
CHECK_FIELD(WPolySoup::sQuad, m_count, 0x00);
CHECK_FIELD(WPolySoup::sQuad, m_pList, 0x04);
CHECK_FIELD(WPolySoup::sQuad, m_count2, 0x08);
CHECK_FIELD(WPolySoup::sQuad, m_pList2, 0x0C);
CHECK_SIZE(WPolySoup::SmInFragList, 0x8);
CHECK_SIZE(WPolySoup::SmInFrag, 0x10);
CHECK_FIELD(WPolySoup::SmInFrag, m_meshIndex, 0x00);
CHECK_FIELD(WPolySoup::SmInFrag, m_triIndex, 0x02);
CHECK_FIELD(WPolySoup::SmInFrag, m_vtx, 0x04);
CHECK_FIELD(WPolySoup::SmInFrag, m_uv, 0x0A);
CHECK_FIELD(WPolySoup::SmInFragList, m_numFrags, 0x00);
CHECK_FIELD(WPolySoup::SmInFragList, m_pFrags, 0x04);
CHECK_SIZE(WPolySoup::SmOutFrag, 0x8);
CHECK_FIELD(WPolySoup::SmOutFrag, m_triIndex, 0x00);
CHECK_FIELD(WPolySoup::SmOutFrag, m_vtx, 0x02);
CHECK_SIZE(WPolySoup::SmCase, 0x8);
CHECK_FIELD(WPolySoup::SmCase, m_numLists, 0x00);
CHECK_FIELD(WPolySoup::SmCase, m_pLists, 0x04);
CHECK_SIZE(WPolySoup::sTriangle, 0x48);
CHECK_FIELD(WPolySoup::sTriangle, m_vertex, 0x00);
CHECK_FIELD(WPolySoup::sTriangle, m_normal, 0x24);
CHECK_FIELD(WPolySoup::sTriangle, m_d, 0x30);
CHECK_FIELD(WPolySoup::sTriangle, m_doubleSided, 0x34);
CHECK_FIELD(WPolySoup::sTriangle, m_texIndex, 0x38);
CHECK_FIELD(WPolySoup::sTriangle, m_propIndex, 0x3C);
CHECK_SIZE(WPolySoup::sHoleMesh, 0x24);
CHECK_FIELD(WPolySoup::sHoleMesh, m_pMesh, 0x00);
CHECK_FIELD(WPolySoup::sHoleMesh, m_vtxs, 0x04);
CHECK_FIELD(WPolySoup::sHoleMesh, m_indices, 0x14);
CHECK_SIZE(WPolySoup::sCupVtx, 0x14);
CHECK_FIELD(WPolySoup::sCupVtx, m_pos, 0x00);
CHECK_FIELD(WPolySoup::sCupVtx, m_pMesh, 0x0C);
CHECK_FIELD(WPolySoup::sCupVtx, m_index, 0x10);
CHECK_SIZE(WPolySoup::sHoleVtx, 0x2C);
CHECK_FIELD(WPolySoup::sHoleVtx, m_pos, 0x00);
CHECK_FIELD(WPolySoup::sHoleVtx, m_normal, 0x0C);
CHECK_FIELD(WPolySoup::sHoleVtx, m_color, 0x18);
CHECK_FIELD(WPolySoup::sHoleVtx, m_uv, 0x1C);
CHECK_FIELD(WPolySoup::sHoleVtx, m_uvBackup, 0x24);
CHECK_SIZE(WPolySoup::sHoleTri, 0x8C);
CHECK_FIELD(WPolySoup::sHoleTri, m_index, 0x04);
CHECK_FIELD(WPolySoup::sHoleTri, m_vtx, 0x08);
CHECK_FIELD(WPolySoup::sHoleTri, m_pVertex, 0x14);
CHECK_FIELD(WPolySoup::sHoleTri, m_pNormal, 0x20);
CHECK_FIELD(WPolySoup::sHoleTri, m_uv, 0x2C);
CHECK_FIELD(WPolySoup::sHoleTri, m_uvBackup, 0x44);
CHECK_FIELD(WPolySoup::sHoleTri, m_color, 0x5C);
CHECK_FIELD(WPolySoup::sHoleTri, m_edge, 0x68);
CHECK_SIZE(WPolySoup::sTexture, 0x28);
CHECK_FIELD(WPolySoup::sTexture, m_name, 0x00);
CHECK_FIELD(WPolySoup::sTexture, m_handle, 0x20);
CHECK_FIELD(WPolySoup::sTexture, m_param, 0x24);
CHECK_SIZE(WPolySoup::sCamera, 0x58);
CHECK_FIELD(WPolySoup::sCamera, m_name, 0x00);
CHECK_FIELD(WPolySoup::sCamera, m_matrix, 0x20);
CHECK_FIELD(WPolySoup::sCamera, m_fov, 0x50);
CHECK_SIZE(WPolySoup::sBgFx, 0x48);
CHECK_FIELD(WPolySoup::sBgFx, m_pSpray, 0x04);
CHECK_FIELD(WPolySoup::sBgFx, m_pSequence, 0x08);
CHECK_SIZE(WPolySoup::sPoint, 0x4C);
CHECK_FIELD(WPolySoup::sPoint, m_type, 0x00);
CHECK_FIELD(WPolySoup::sPoint, m_name, 0x01);
CHECK_FIELD(WPolySoup::sPoint, m_pos, 0x24);
CHECK_FIELD(WPolySoup::sPoint, m_desc, 0x30);
CHECK_SIZE(WPolySoup::sArea, 0x5C);
CHECK_FIELD(WPolySoup::sArea, m_type, 0x00);
CHECK_FIELD(WPolySoup::sArea, m_name, 0x01);
CHECK_FIELD(WPolySoup::sArea, m_aabb, 0x44);
CHECK_SIZE(WPolySoup::CStaticPetGrp, 0xC);
BEGIN_LAYOUT_CHECK(WPolySoup::CStaticPetGrp)
	CHECK_FIELD(WPolySoup::CStaticPetGrp, m_numGroups, 0x4);
	CHECK_FIELD(WPolySoup::CStaticPetGrp, m_ppGroups, 0x8);
END_LAYOUT_CHECK
CHECK_SIZE(WPolySoup::sCounterTriple, 0xC);
CHECK_SIZE(WPolySoup::sBoundAreaRecord, 0x18);
CHECK_SIZE(WPolySoup::sModelRecord, 0xAC);
CHECK_FIELD(WPolySoup::sModelRecord, m_aabb, 0x28);
CHECK_FIELD(WPolySoup::sModelRecord, m_matrix, 0x58);
CHECK_FIELD(WPolySoup::sModelRecord, m_kind, 0x88);
CHECK_FIELD(WPolySoup::sModelRecord, m_baseName, 0x8C);
CHECK_FIELD(WPolySoup, m_boundAreas, 0x2C);
CHECK_FIELD(WPolySoup, m_numOtherRecords, 0x7C);
CHECK_FIELD(WPolySoup, m_defaultNear, 0xE4);
CHECK_FIELD(WPolySoup, m_defaultFar, 0xE8);
CHECK_FIELD(WPolySoup, m_counters1, 0xEC);
CHECK_FIELD(WPolySoup, m_counters2, 0xF8);
CHECK_FIELD(WPolySoup, m_bPendingOption, 0x108);
CHECK_FIELD(WPolySoup, m_lightSets, 0x4920);
CHECK_FIELD(WPolySoup, m_ulongList, 0x492C);
CHECK_FIELD(WPolySoup, m_intList, 0x493C);
CHECK_FIELD(WPolySoup, m_numSoundBoxRefs, 0x495C);
CHECK_FIELD(WPolySoup, m_holecupTexture, 0x4970);
CHECK_FIELD(WPolySoup, m_mapName, 0x3C);
CHECK_FIELD(WPolySoup, m_numCameras, 0x90);
CHECK_FIELD(WPolySoup, m_numPoints, 0x94);
CHECK_FIELD(WPolySoup, m_numAreas, 0x98);
CHECK_FIELD(WPolySoup, m_numTextures, 0x9C);
CHECK_FIELD(WPolySoup, m_numObjects, 0xA4);
CHECK_FIELD(WPolySoup, m_numStructuredObjects, 0xA8);
CHECK_FIELD(WPolySoup, m_pTextures, 0xAC);
CHECK_FIELD(WPolySoup, m_numOldTextures, 0xB0);
CHECK_FIELD(WPolySoup, m_pOldTextures, 0xB4);
CHECK_FIELD(WPolySoup, m_pCameras, 0xB8);
CHECK_FIELD(WPolySoup, m_pAreas, 0xBC);
CHECK_FIELD(WPolySoup, m_pPoints, 0xC0);
CHECK_FIELD(WPolySoup, m_models, 0xC4);
CHECK_FIELD(WPolySoup, m_pBgModel, 0xD4);
CHECK_FIELD(WPolySoup, m_texIndexMap, 0xD8);
CHECK_FIELD(WPolySoup, m_soundBoxes, 0x494C);
CHECK_FIELD(WPolySoup, m_pGolfDoc, 0x104);
CHECK_FIELD(WPolySoup, m_bUseTnL, 0x109);
CHECK_FIELD(WPolySoup, m_staticPetGrp, 0x10C);
CHECK_FIELD(WPolySoup, m_bgFx, 0x118);
CHECK_FIELD(WPolySoup, m_numBgFx, 0x4918);
CHECK_FIELD(WPolySoup, m_bAltCameraBounds, 0x491C);
CHECK_FIELD(WPolySoup, m_numSmapMeshes, 0x4960);
CHECK_FIELD(WPolySoup, m_pSmapDrawFlags, 0x4964);
CHECK_FIELD(WPolySoup, m_pSmapMeshes, 0x4968);
CHECK_FIELD(WPolySoup, m_bNoShadowmap, 0x496C);
CHECK_FIELD(WPolySoup, m_triArrays, 0x4974);

#endif // PG_CLIENT_PROJECTG_POLYSOUP_H
