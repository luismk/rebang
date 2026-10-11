// Original: d:\build\custom\temp\code\client\wangreal\include\wboneset.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WBONESET_H
#define PG_CLIENT_WANGREAL_INCLUDE_WBONESET_H

#include "..\..\defines.h"
#include "wlist.h"
#include "wmath.h"

// One bone's key of a set (PDB layout, size 0x28).
struct WBoneKey {
	char *name;          // 0x0
	WQuat quat;          // 0x4
	WVector pivot;       // 0x14
	float scale;         // 0x20
	unsigned char flags; // 0x24
};
CHECK_SIZE(WBoneKey, 0x28);
CHECK_FIELD(WBoneKey, name, 0x0);
CHECK_FIELD(WBoneKey, quat, 0x4);
CHECK_FIELD(WBoneKey, pivot, 0x14);
CHECK_FIELD(WBoneKey, scale, 0x20);
CHECK_FIELD(WBoneKey, flags, 0x24);

// A set of bone keys (PDB layout, size 0x28).
class WBoneSet {
	LAYOUT_CHECK_FRIEND;
public:
	// wboneset.h:57
	WBoneSet(bool alloc = true) : m_keyList(32, 256) {
		if (alloc)
			m_data = new WBoneKey[256];
		else
			m_data = 0;
		m_index = 0;
	}

	// wboneset.h:67
	~WBoneSet() {
		if (m_data) {
			delete[] m_data;
			m_data = 0;
		}
	}

	// wboneset.h:72
	void Clear() {
		m_keyList.Reset();
	}

	// wboneset.h:77
	WBoneKey *FindBoneKey(char *name) {
		return m_keyList.Find(name);
	}

	void AddBoneKey(char *name, WQuat *quat, WVector *pivot, float *scale, int flags);
	void CopyFrom(WBoneSet *pSrc);
	void Blend(WBoneSet *pA, WBoneSet *pB, float t);

private:
	WList<WBoneKey *> m_keyList;   // 0x0
	WBoneKey *m_data;              // 0x20
	unsigned char m_index;         // 0x24

	static WBoneKey m_preAlloc[256];
	static unsigned char m_curAlloc;
};

CHECK_SIZE(WBoneSet, 0x28);
BEGIN_LAYOUT_CHECK(WBoneSet)
	CHECK_FIELD(WBoneSet, m_keyList, 0x0);
	CHECK_FIELD(WBoneSet, m_data, 0x20);
	CHECK_FIELD(WBoneSet, m_index, 0x24);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WBONESET_H
