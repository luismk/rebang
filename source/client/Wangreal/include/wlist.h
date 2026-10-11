// Original: d:\build\custom\temp\code\client\wangreal\include\wlist.h
#ifndef PG_CLIENT_WANGREAL_INCLUDE_WLIST_H
#define PG_CLIENT_WANGREAL_INCLUDE_WLIST_H
#include "wmemblock.h"
#include <string.h>
#include "..\..\defines.h"

template <class T>
class WList {
	LAYOUT_CHECK_FRIEND;
public:
	WList(int blk_len = 8, int hash_num = 0);
	~WList();
	T Start(void *&pos) const;
	T Next(void *&pos) const;
	T End(void *&pos) const;
	T Prev(void *&pos) const;
	T Start();
	T Next();
	T End();
	T Prev();
	int GetCount() const;
	T Find(const char *name) const;
	void Reset();
	void AddItem(const T &item, const char *name = NULL, bool alloc = false);
	void Report(char *title);
	void operator+=(const T &item);
	void operator-=(const T &item);

protected:
	struct listinfo;
	void DelItem(const T &item);
	listinfo *Link(listinfo *head, listinfo *item);
	listinfo *Unlink(listinfo *head, listinfo *item);
private:
	listinfo *Alloc();
	void AddHash(int code, listinfo *item);
	void DelHash(listinfo *item);
	int HASHCODE(const void *key) const;
	listinfo *m_list;            // 0x0
	listinfo *m_surf;            // 0x4, the cursor of Start/Next/End/Prev
	listinfo *m_pre_alloc;       // 0x8, the node blocks, by their first node
	listinfo *m_idle;            // 0xC, the free nodes
	int m_blk_len;               // 0x10, nodes per block
	int m_hash_mask;             // 0x14
	int m_hashNum;               // 0x18
	listinfo **m_hash_list;      // 0x1C
};
// wlist.h:51
template <class T>
WList<T>::WList(int blk_len, int hash_num) {
	m_hash_list = NULL;
	m_list = NULL;
	m_idle = NULL;
	m_pre_alloc = NULL;
	m_blk_len = blk_len;
	m_surf = NULL;
	m_hashNum = hash_num;
	if (m_hashNum > 0) {

		int size = m_hashNum * sizeof(listinfo *);
		m_hash_list = (listinfo **)WLIST_ALLOC(size);



		m_hash_mask = m_hashNum - 1;

		for (int i = 0; i < m_hashNum; i++) {
			m_hash_list[i] = NULL;
		}

	} else {
		m_hash_list = NULL;
	}









}
// wlist.h:87
template <class T>
WList<T>::~WList() {


	while (m_pre_alloc != NULL) {
		listinfo *block = m_pre_alloc;
		m_pre_alloc = Unlink(m_pre_alloc, block);
		WLIST_FREE(block);
	}

	if (m_hash_list != NULL) {



		WLIST_FREE(m_hash_list);
		m_hash_list = NULL;
	}
}










// wlist.h:116
template <class T>
T WList<T>::Start(void *&pos) const {
	pos = m_list;
	return m_list ? m_list->item : NULL;
}
// wlist.h:122
template <class T>
T WList<T>::Next(void *&pos) const {


	if (pos != NULL) {
		listinfo *info = (listinfo *)pos;
		if (info->next == m_list) {

			pos = NULL;
		} else {


			pos = info->next;
			return info->next->item;
		}
	}
	return NULL;
}
// wlist.h:141
template <class T>
T WList<T>::End(void *&pos) const {


	pos = m_list ? m_list->prev : NULL;
	return m_list ? ((listinfo *)pos)->item : NULL;
}
// wlist.h:149
template <class T>
T WList<T>::Prev(void *&pos) const {


	if (pos != NULL) {
		listinfo *info = (listinfo *)pos;
		if (info == m_list) {

			pos = NULL;
		} else {


			pos = info->prev;
			return info->prev->item;
		}
	}
	return NULL;
}
// wlist.h:168
template <class T>
T WList<T>::Start() {
	m_surf = m_list;
	return Next();
}
// wlist.h:174
template <class T>
T WList<T>::Next() {


	if (m_surf != NULL) {

		T item = m_surf->item;
		m_surf = (m_surf->next == m_list) ? NULL : m_surf->next;
		return item;
	}

	return NULL;
}
// wlist.h:188
template <class T>
T WList<T>::End() {
	m_surf = m_list ? m_list->prev : NULL;
	return Prev();
}
// wlist.h:194
template <class T>
T WList<T>::Prev() {


	if (m_surf != NULL) {

		T item = m_surf->item;
		m_surf = (m_surf != m_list) ? m_surf->prev : NULL;
		return item;
	}

	return NULL;
}
// wlist.h:208
template <class T>
int WList<T>::GetCount() const {
	int count = 0;
	for (listinfo *info = m_list; info != NULL; count++) info = (m_list == info->next) ? NULL : info->next;
	return count;
}
// wlist.h:215
template <class T>
T WList<T>::Find(const char *name) const {


	if (name == NULL || m_hash_list == NULL) {
		return NULL;
	}
	for (listinfo *info = m_hash_list[HASHCODE(name)]; info != NULL; info = info->hash) {

		if (strcmp(info->keycode, name) == 0)
			return info->item;
	}
	return NULL;
}
// A node of the list (an optional name hashes it). Nodes come from blocks of m_blk_len: the first node of each block
// links the block into m_pre_alloc, the others go to the idle list.
template <class T>
struct WList<T>::listinfo {
	T item;                  // 0x0
	char *keycode;           // 0x4
	bool alloc;              // 0x8, keycode is a malloc'd copy
	listinfo *prev;          // 0xC
	listinfo *next;          // 0x10
	listinfo *hash;          // 0x14, next node of the same hash bucket
};











// wlist.h:252
template <class T>
void WList<T>::Reset() {


	if (m_list != NULL && m_idle != NULL) {
		// Splice the whole list into the idle list at once.



		m_list->prev->next = m_idle->next;
		m_idle->next->prev = m_list->prev;

		m_idle->next = m_list;
		m_list->prev = m_idle;

		m_list = NULL;
	} else {


		while (m_list != NULL) {

			listinfo *next = Unlink(m_list, m_list);
			m_idle = Link(m_idle, m_list);
			m_list = next;
		}
	}

	for (int i = 0; i < m_hashNum; i++)
		m_hash_list[i] = NULL;
}
// wlist.h:283
template <class T>
void WList<T>::AddItem(const T &item, const char *name, bool alloc) {
	listinfo *info = Alloc();

	info->item = item;
	info->alloc = alloc;


	if (alloc) {
		info->keycode = (char *)WLIST_ALLOC(strlen(name) + 1);
		strcpy(info->keycode, name);

	} else {

		info->keycode = (char *)name;
	}

	if (name != NULL) {
		AddHash(HASHCODE(name), info);
	}

	m_list = Link(m_list, info);
}





























// wlist.h:335
template <class T> void WList<T>::operator+=(const T &item) { AddItem(item); }
template <class T> void WList<T>::operator-=(const T &item) { DelItem(item); }











// wlist.h:350
template <class T>
void WList<T>::DelItem(const T &item) {
	listinfo *info = m_list;

	if (info != NULL) {
		do {



			if (info->item == item) {

				if (m_surf == info) {
					m_surf = (m_surf->next != m_list) ? m_surf->next : NULL;
				}

				if (info->keycode != NULL) {

					DelHash(info);
					if (info->alloc) {

						WLIST_FREE(info->keycode);
					}
				}

				m_list = Unlink(m_list, info);
				m_idle = Link(m_idle, info);

				if (m_list == NULL) {
					m_surf = NULL;
				}
				return;
			}
			info = info->next;
		} while (info != m_list);
	}
}

// wlist.h:388
template <class T>
typename WList<T>::listinfo *WList<T>::Link(listinfo *head, listinfo *item) {
	if (head == NULL) {

		item->next = item->prev = item;
		return item;
	}

	item->next = head;
	item->prev = head->prev;
	head->prev = item;
	item->prev->next = item;

	return head;
}
// wlist.h:404
template <class T>
typename WList<T>::listinfo *WList<T>::Unlink(listinfo *head, listinfo *item) {
	item->next->prev = item->prev;
	item->prev->next = item->next;

	return (item != head) ? head : ((item->next != head) ? item->next : NULL);
}






// wlist.h:418
template <class T>
typename WList<T>::listinfo *WList<T>::Alloc() {



	if (m_idle == NULL) {







		listinfo *block = (listinfo *)WLIST_ALLOC(m_blk_len * sizeof(listinfo));






		m_pre_alloc = Link(m_pre_alloc, block);

		for (int i = 1; i < m_blk_len; i++) {
			m_idle = Link(m_idle, &block[i]);
		}
	}
	listinfo *info = m_idle;

	m_idle = Unlink(m_idle, info);


	return info;
}











// wlist.h:463
template <class T>
void WList<T>::AddHash(int code, listinfo *item) {
	item->hash = m_hash_list[code];
	m_hash_list[code] = item;
}
// wlist.h:469
template <class T>
void WList<T>::DelHash(listinfo *item) {



	int code = HASHCODE(item->keycode);
	listinfo *info = m_hash_list[code];

	if (info == item) {
		m_hash_list[code] = item->hash;
	} else {

		for (; info != NULL; info = info->hash) {

			if (info->hash == item) {

				info->hash = item->hash;
				break;
			}
		}
	}


}
// wlist.h:494
template <class T>
int WList<T>::HASHCODE(const void *key) const {


	int len = strlen((const char *)key);


	int last = (len > 0) ? len - 1 : 0;

	return ((len << 6) | (((const char *)key)[0] & 0x2A) | (((const char *)key)[last] & 0x15)) & m_hash_mask;
}

CHECK_SIZE(WList<struct WBoneKey *>, 0x20);
// BEGIN_LAYOUT_CHECK cannot take a template-id ('>>' closes it).
template <> struct PgLayoutCheck<WList<struct WBoneKey *> > {
	CHECK_SIZE(WList<struct WBoneKey *>::listinfo, 0x18);
	CHECK_FIELD(WList<struct WBoneKey *>::listinfo, alloc, 0x8);
	CHECK_FIELD(WList<struct WBoneKey *>::listinfo, hash, 0x14);
	CHECK_FIELD(WList<struct WBoneKey *>, m_idle, 0xC);
	CHECK_FIELD(WList<struct WBoneKey *>, m_hash_list, 0x1C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_WANGREAL_INCLUDE_WLIST_H
