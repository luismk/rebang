// Original: d:\build\custom\temp\code\client\projectg\parallelwork.cpp
#include <windows.h>
#include "ParallelWork.h"

namespace Py {
namespace Threading {

// parallelwork.cpp:10
ParallelWork::ParallelWork(ParallelWorkPolicy::Enum policy)
	: m_lockCount(0)
	, m_working(1)
	, m_policy(policy) {
}

// parallelwork.cpp:14
ParallelWork::~ParallelWork() {
	if (m_policy == ParallelWorkPolicy::ClearWorkingOnDestroy)
		InterlockedExchange(&m_working, 0);

	while (m_lockCount > 0) Sleep(0);
}

// parallelwork.cpp:22
bool ParallelWork::IsWorkingFirst() const {
	return m_working != 0;
}

// parallelwork.cpp:27
long ParallelWork::IncrementLockCount() {
	return InterlockedIncrement(&m_lockCount);
}

// parallelwork.cpp:32
long ParallelWork::DecrementLockCount() {
	return InterlockedDecrement(&m_lockCount);
}

} // namespace Threading
} // namespace Py
