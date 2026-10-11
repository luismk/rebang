// Original: d:\build\custom\temp\code\client\wangreal\include\wpetfile.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WPETFILE_H
#define PG_CLIENT_WANGREAL_INCLUDE_WPETFILE_H

#include "..\..\defines.h"

class WBone;

// A vertex of a .pet model file (PDB layout, size 0x20).
struct w_pet_vertex {
	// A bone the vertex is blended to (PDB layout, size 0x8).
	struct w_blended {
		unsigned char blendWeight;      // 0x0
		WBone* bone;                    // 0x4
	};

	float x;                            // 0x0
	float y;                            // 0x4
	float z;                            // 0x8
	float weight;                       // 0xc
	unsigned char blendWeight;          // 0x10
	char* bonename;                     // 0x14
	w_blended* list;                    // 0x18
	int listNum;                        // 0x1c
};
CHECK_SIZE(w_pet_vertex::w_blended, 0x8);
CHECK_FIELD(w_pet_vertex::w_blended, blendWeight, 0x0);
CHECK_FIELD(w_pet_vertex::w_blended, bone, 0x4);
CHECK_SIZE(w_pet_vertex, 0x20);
CHECK_FIELD(w_pet_vertex, x, 0x0);
CHECK_FIELD(w_pet_vertex, y, 0x4);
CHECK_FIELD(w_pet_vertex, z, 0x8);
CHECK_FIELD(w_pet_vertex, weight, 0xc);
CHECK_FIELD(w_pet_vertex, blendWeight, 0x10);
CHECK_FIELD(w_pet_vertex, bonename, 0x14);
CHECK_FIELD(w_pet_vertex, list, 0x18);
CHECK_FIELD(w_pet_vertex, listNum, 0x1c);

// A triangle corner of a .pet model file (PDB layout, size 0x18).
struct w_pet_tri_point {
	int pos;                            // 0x0
	float nx;                           // 0x4
	float ny;                           // 0x8
	float nz;                           // 0xc
	float tu;                           // 0x10
	float tv;                           // 0x14
};
CHECK_SIZE(w_pet_tri_point, 0x18);
CHECK_FIELD(w_pet_tri_point, pos, 0x0);
CHECK_FIELD(w_pet_tri_point, nx, 0x4);
CHECK_FIELD(w_pet_tri_point, ny, 0x8);
CHECK_FIELD(w_pet_tri_point, nz, 0xc);
CHECK_FIELD(w_pet_tri_point, tu, 0x10);
CHECK_FIELD(w_pet_tri_point, tv, 0x14);

// A skin of a mesh .pet (PDB layout, size 0x14).
struct w_skin_info {
	unsigned char ucBoneID;     // 0x0
	int iSVtxIdx;               // 0x4
	int nVtxs;                  // 0x8
	int iSTriIdx;               // 0xC
	int nTris;                  // 0x10
};
CHECK_SIZE(w_skin_info, 0x14);
CHECK_FIELD(w_skin_info, ucBoneID, 0x0);
CHECK_FIELD(w_skin_info, iSVtxIdx, 0x4);
CHECK_FIELD(w_skin_info, nVtxs, 0x8);
CHECK_FIELD(w_skin_info, iSTriIdx, 0xC);
CHECK_FIELD(w_skin_info, nTris, 0x10);

// A raw texture record of a .pet model file (PDB layout, size 0x2c).
struct w_pet_texture {
	char filename[32];          // 0x0
	char flag;                  // 0x20
	unsigned char group;        // 0x21
	unsigned int diffuse;       // 0x24 (a 32-bit file field; `unsigned long`
	                            // is 8 bytes on LP64 and would grow the record)
	int handle;                 // 0x28
};
CHECK_SIZE(w_pet_texture, 0x2c);
CHECK_FIELD(w_pet_texture, filename, 0x0);
CHECK_FIELD(w_pet_texture, flag, 0x20);
CHECK_FIELD(w_pet_texture, group, 0x21);
CHECK_FIELD(w_pet_texture, diffuse, 0x24);
CHECK_FIELD(w_pet_texture, handle, 0x28);

// A face animation record of a .pet model file (PDB layout, size 0x41).
struct w_pet_face_animation {
	unsigned char group;        // 0x0
	char animName[32];          // 0x1
	char fName[32];             // 0x21
};
CHECK_SIZE(w_pet_face_animation, 0x41);
CHECK_FIELD(w_pet_face_animation, group, 0x0);
CHECK_FIELD(w_pet_face_animation, animName, 0x1);
CHECK_FIELD(w_pet_face_animation, fName, 0x21);

// A texture slot of a .pet model file (PDB layout, size 0x70).
struct w_pet_texture_info {
	char filename[64];                  // 0x0
	unsigned char alpha;                // 0x40
	unsigned int diffuse;               // 0x44 (32-bit file fields; see above)
	unsigned int drawFlags;             // 0x48
	unsigned int xulDrawFlag2;          // 0x4c
	int texHandle;                      // 0x50
	int mapType;                        // 0x54
	bool bBind;                         // 0x58
	float pu;                           // 0x5c
	float pv;                           // 0x60
	float scalex;                       // 0x64
	float scaley;                       // 0x68
	unsigned char group;                // 0x6c
};
CHECK_SIZE(w_pet_texture_info, 0x70);
CHECK_FIELD(w_pet_texture_info, filename, 0x0);
CHECK_FIELD(w_pet_texture_info, alpha, 0x40);
CHECK_FIELD(w_pet_texture_info, diffuse, 0x44);
CHECK_FIELD(w_pet_texture_info, drawFlags, 0x48);
CHECK_FIELD(w_pet_texture_info, xulDrawFlag2, 0x4c);
CHECK_FIELD(w_pet_texture_info, texHandle, 0x50);
CHECK_FIELD(w_pet_texture_info, mapType, 0x54);
CHECK_FIELD(w_pet_texture_info, bBind, 0x58);
CHECK_FIELD(w_pet_texture_info, pu, 0x5c);
CHECK_FIELD(w_pet_texture_info, pv, 0x60);
CHECK_FIELD(w_pet_texture_info, scalex, 0x64);
CHECK_FIELD(w_pet_texture_info, scaley, 0x68);
CHECK_FIELD(w_pet_texture_info, group, 0x6c);

#endif // PG_CLIENT_WANGREAL_INCLUDE_WPETFILE_H
