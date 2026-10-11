// Original: d:\build\custom\temp\code\client\projectg\ringq.h
#ifndef PG_CLIENT_PROJECTG_RINGQ_H
#define PG_CLIENT_PROJECTG_RINGQ_H

// A ring queue of T over one heap array: m_head is the oldest entry,
// m_tail the next free one, both wrapping at m_size.
template <class T>
class TRingQ {
public:
	TRingQ(void);
	~TRingQ(void);

	void Create(int size);

	int IsFull(void) const;
	int IsUndersink(unsigned int count) const;
	int IsOverflow(unsigned int count) const;
	T* GetHead(unsigned int index) const;
	T* GetTail(unsigned int index) const;
	int GetLen(void) const;
	T* StepHead(unsigned int count);
	T* StepTail(unsigned int count);

protected:
	T* m_buf;               // 0x0, new[]'d, delete[]'d by the destructor
	int m_head;             // 0x4, index of the oldest entry
	int m_tail;             // 0x8, index of the next free entry
	int m_size;             // 0xC, capacity of m_buf
};

// ringq.h:97
template <class T>
TRingQ<T>::TRingQ(void) {
	m_buf = 0;
	m_head = m_tail = m_size = 0;
}

// ringq.h:106
template <class T>
TRingQ<T>::~TRingQ(void) {
	if (m_buf) delete[] m_buf;
}

// ringq.h:112
// (Re)allocates the buffer for size entries and empties the queue; a size
// below 2 leaves it as it is. Name invented (the PDB has none).
template <class T>
inline void TRingQ<T>::Create(int size) {
	if (size < 2)
		return;
	if (m_buf) delete[] m_buf;
	m_buf = new T[size];
	m_size = size;
	m_head = m_tail = 0;
}

// ringq.h:128
template <class T>
inline int TRingQ<T>::IsFull(void) const {
	return IsOverflow(1);
}

// ringq.h:134
// More entries asked for than the queue holds.
template <class T>
inline int TRingQ<T>::IsUndersink(unsigned int count) const {
	return (int)count > GetLen();
}

// ringq.h:144
// No room left for count more entries.
template <class T>
inline int TRingQ<T>::IsOverflow(unsigned int count) const {
	if (m_head > m_tail) {
		if ((int)count < m_head - m_tail)
			return 0;
	} else {
		if (m_size > m_tail - m_head + (int)count)
			return 0;
	}
	return 1;
}

// ringq.h:170
template <class T>
inline T* TRingQ<T>::GetHead(unsigned int index) const {
	return &m_buf[(m_head + index) % m_size];
}

// ringq.h:175
// The entry index places before the tail (0: the next free one).
template <class T>
inline T* TRingQ<T>::GetTail(unsigned int index) const {
	int i = m_tail - index;
	if (i < 0)
		i = m_size - index + m_tail;

	return &m_buf[i];
}

// ringq.h:183
template <class T>
inline int TRingQ<T>::GetLen(void) const {
	if (m_tail < m_head)
		return m_size - m_head + m_tail;
	else
		return m_tail - m_head;
}

// ringq.h:191
// Drops count entries from the head; returns the new head, 0 when fewer are queued.
template <class T>
T* TRingQ<T>::StepHead(unsigned int count) {
	if (IsUndersink(count))
		return 0;

	m_head = (m_head + count) % m_size;
	return &m_buf[m_head];
}

// ringq.h:202
// Queues count entries at the tail; returns the new tail, 0 when full.
template <class T>
T* TRingQ<T>::StepTail(unsigned int count) {
	if (IsOverflow(count))
		return 0;

	m_tail = (m_tail + count) % m_size;
	return &m_buf[m_tail];
}

#endif // PG_CLIENT_PROJECTG_RINGQ_H
