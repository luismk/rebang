// Original: d:\build\custom\temp\code\client\wangreal\source\wboneset.cpp
#include "../include/wboneset.h"
#include "../include/wmath.h"

WBoneKey WBoneSet::m_preAlloc[256];
unsigned char WBoneSet::m_curAlloc = 0;


// wboneset.cpp:21
void WBoneSet::AddBoneKey(char *name, WQuat *quat, WVector *pivot, float *scale, int flags) {
	WBoneKey *pKey = m_keyList.Find(name);

	if (!pKey) {

		if (m_data) {


			pKey = &m_data[m_index++];

		} else {

			pKey = &m_preAlloc[m_curAlloc++];
		}


		pKey->name = name; m_keyList.AddItem(pKey, name, false);
	}

	pKey->flags = flags;

	if (flags & 1)
		pKey->quat = *quat;

	if (flags & 2)
		pKey->pivot = *pivot;

	if (flags & 4)
		pKey->scale = *scale;
}

// wboneset.cpp:53
void WBoneSet::CopyFrom(WBoneSet *a) {


	m_keyList.Reset();

	for (WBoneKey *pKey = a->m_keyList.Start(); pKey; pKey = a->m_keyList.Next())
		AddBoneKey(pKey->name, &pKey->quat, &pKey->pivot, &pKey->scale, pKey->flags);
}

// wboneset.cpp:63
static void BlendBoneKey(WBoneKey *a, WBoneKey *b, WBoneKey *c, float fraction) {
	if (a->flags & 1)
		c->quat = (b->flags & 1) ? WQuaternionSlerp(b->quat, a->quat, fraction) : a->quat;

	if (a->flags & 2)
		c->pivot = (b->flags & 2) ? (a->pivot - b->pivot) * fraction + b->pivot : a->pivot;

	if (a->flags & 4)
		c->scale = (b->flags & 4) ? b->scale = (1.0f - fraction) * b->scale + fraction * a->scale : a->scale;

	c->flags = b->flags | a->flags;
}

// wboneset.cpp:77
void WBoneSet::Blend(WBoneSet *a, WBoneSet *b, float fraction) {
	WBoneKey key;





	if (WisEqual(fraction, 0.0f, g_EPSILON)) {

		CopyFrom(a);
		return;
	}

	if (WisEqual(fraction, 1.0f, g_EPSILON)) {

		CopyFrom(b);
		return;
	}

	m_keyList.Reset();

	for (WBoneKey *pKey = a->m_keyList.Start(); pKey; pKey = a->m_keyList.Next()) {
		WBoneKey *pSrc;
		WBoneKey *pOther = b->m_keyList.Find(pKey->name);

		if (pOther) {

			BlendBoneKey(pOther, pKey, &key, fraction);
			key.name = pKey->name;


			pSrc = &key;
		} else {
			pSrc = 0;
		}


		AddBoneKey(pSrc->name, &pSrc->quat, &pSrc->pivot, &pSrc->scale, pSrc->flags);
	}
}

// wboneset.cpp:158
WBoneSet *__cdecl BlendBoneSet(WBoneSet *a, WBoneSet *b, float t) {
	static WBoneSet boneSetList[4] = { false };
	static unsigned char boneSetPos = 0;

	WBoneSet *pSet = &boneSetList[boneSetPos++ & 3];

	pSet->Clear();
	pSet->Blend(a, b, t);


	return pSet;
}
