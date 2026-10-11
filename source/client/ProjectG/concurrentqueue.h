// Original: d:\build\custom\temp\code\client\projectg\concurrentqueue.h
#ifndef PG_CLIENT_PROJECTG_CONCURRENTQUEUE_H
#define PG_CLIENT_PROJECTG_CONCURRENTQUEUE_H

#include <deque>
#include "ParallelLock.h"

namespace Py {
namespace Threading {

// A queue guarded by a critical section. The original member is a
// std::queue<T, Container>; the toolchain copy in externs has no <queue>, so
// the container is used directly with the calls std::queue makes on it.
template <class T, class Container = std::deque<T> >
class ConcurrentQueue {
public:
	ConcurrentQueue() {}
	~ConcurrentQueue() {}

	void Push(const T& value) {
		SingleLock lock(&m_lock);
		m_queue.push_back(value);
	}
	bool Pop(T& value) {
		SingleLock lock(&m_lock);
		if (!m_queue.empty()) {
			T front = m_queue.front();
			m_queue.pop_front();
			value = front;
			return true;
		}
		return false;
	}
	bool IsEmpty() const { return m_queue.empty(); }

private:
	Container m_queue;                  // 0x0
	CriticalSection m_lock;             // 0x14
};

} // namespace Threading
} // namespace Py

#endif
