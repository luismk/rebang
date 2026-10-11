// Original: d:\build\custom\temp\code\client\projectg\threadpool.h (path assumed: no inline code of it is in the line info)
#ifndef PG_CLIENT_PROJECTG_THREADPOOL_H
#define PG_CLIENT_PROJECTG_THREADPOOL_H

#include "..\..\defines.h"
#include "ParallelWork.h"

namespace Py {
namespace Threading {

// Implementation selected by ThreadPool's constructor (names invented: the
// PDB has no enumerators).
struct ThreadPoolPolicy {
	enum Enum {
		ConcurrentQueue = 0,
		Iocp = 1,               // falls back to a one-thread ConcurrentQueue pool
		SingleThread = 2,       // a one-thread ConcurrentQueue pool
		Unimplement = 3
	};
};

// Values of SetThreadPriority (THREAD_PRIORITY_*).
struct ThreadPriority {
	enum Enum {
		Idle = -15,
		Lowest = -2,
		BelowNormal = -1,
		Normal = 0,
		AboveNormal = 1,
		Highest = 2,
		TimeCritical = 15
	};
};

// Values of SetPriorityClass (*_PRIORITY_CLASS).
struct ProcessPriority {
	enum Enum {
		Normal = 0x20,
		Idle = 0x40,
		High = 0x80,
		Realtime = 0x100,
		BelowNormal = 0x4000,
		AboveNormal = 0x8000
	};
};

class ThreadPoolImpl;

class ThreadPool {
public:
	ThreadPool(ThreadPoolPolicy::Enum policy, unsigned int threadCount);
	~ThreadPool();

	static int SetProcessPriority(ProcessPriority::Enum priority);
	static ProcessPriority::Enum GetProcessPriority();

	unsigned int GetThreadCount() const;
	unsigned int GetInvokedWorkCount() const;
	int WaitForCompletionAll(unsigned long timeout);
	int SetThreadPriority(ThreadPriority::Enum priority);
	ThreadPriority::Enum GetThreadPriority() const;

	template <class T>
	void InvokeParallelWork(T* work, void (T::*func)()) {
		InternalInvokeParallelWork(work, static_cast<void (ParallelWork::*)()>(func));
	}

private:
	void InternalInvokeParallelWork(ParallelWork* work, void (ParallelWork::*func)());

	ThreadPoolImpl* m_pool;         // 0x0

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(ThreadPool, 0x4);
BEGIN_LAYOUT_CHECK(ThreadPool)
	CHECK_FIELD(ThreadPool, m_pool, 0x0);
END_LAYOUT_CHECK

} // namespace Threading
} // namespace Py

#endif
