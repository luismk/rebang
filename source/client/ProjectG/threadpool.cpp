// Original: d:\build\custom\temp\code\client\projectg\threadpool.cpp
#include <windows.h>
#include <process.h>
#include <vector>
#include "ThreadPool.h"
#include "ConcurrentQueue.h"

namespace Py {
namespace Threading {

// One unit of work handed to the pool. The OVERLAPPED comes first: the IOCP
// pool posts the InvokedWork itself as the completion's OVERLAPPED.
struct InvokedWork {
	OVERLAPPED m_overlapped;            // 0x0
	ParallelWork* m_work;               // 0x14
	void (ParallelWork::*m_func)();     // 0x18
};
CHECK_SIZE(InvokedWork, 0x1C);
CHECK_FIELD(InvokedWork, m_work, 0x14);
CHECK_FIELD(InvokedWork, m_func, 0x18);

class ThreadPoolImpl {
public:
	ThreadPoolImpl();
	virtual ~ThreadPoolImpl();                              // vtable slot 3

	unsigned int GetThreadCount() const;
	unsigned int GetInvokedWorkCount() const;
	void InvokeParallelWork(ParallelWork* work, void (ParallelWork::*func)());
	int WaitForCompletionAll(unsigned long timeout);
	int SetThreadPriority(ThreadPriority::Enum priority);
	ThreadPriority::Enum GetThreadPriority() const;

	static unsigned int __stdcall ThreadProc(void* param);

protected:
	virtual void OnInvokeParallelWork(InvokedWork* work) = 0;  // vtable slot 0
	virtual unsigned int OnThreadProc(void* param) = 0;        // vtable slot 1
	virtual const char* GetThreadPoolName() const = 0;         // vtable slot 2

public:
	virtual int Initialize(int threadCount);                // vtable slot 4
	virtual void Release();                                 // vtable slot 5

protected:
	std::vector<HANDLE> m_events;               // 0x4 (one per thread, set to stop it)
	std::vector<HANDLE> m_threads;              // 0x14
	unsigned int m_threadCount;                 // 0x24
	unsigned int m_invokedWorkCount;            // 0x28 (Interlocked{In,De}crement)

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(ThreadPoolImpl, 0x2C);
BEGIN_LAYOUT_CHECK(ThreadPoolImpl)
	CHECK_FIELD(ThreadPoolImpl, m_events, 0x4);
	CHECK_FIELD(ThreadPoolImpl, m_threads, 0x14);
	CHECK_FIELD(ThreadPoolImpl, m_threadCount, 0x24);
	CHECK_FIELD(ThreadPoolImpl, m_invokedWorkCount, 0x28);
END_LAYOUT_CHECK

// The argument of ThreadProc, freed by the thread.
struct ThreadParam {
	ThreadPoolImpl* m_pool;     // 0x0
	HANDLE m_event;             // 0x4
};
CHECK_SIZE(ThreadParam, 0x8);

// threadpool.cpp:74
unsigned int __stdcall ThreadPoolImpl::ThreadProc(void* param) {
	ThreadParam* threadParam = static_cast<ThreadParam*>(param);

	ThreadPoolImpl* pool = threadParam->m_pool;
	HANDLE event = threadParam->m_event;
	delete threadParam;

	return pool->OnThreadProc(event);
}

// threadpool.cpp:99
ThreadPoolImpl::ThreadPoolImpl() : m_threadCount(0), m_invokedWorkCount(0) {}
// threadpool.cpp:100
ThreadPoolImpl::~ThreadPoolImpl() {}

// threadpool.cpp:102
unsigned int ThreadPoolImpl::GetThreadCount() const { return m_threadCount; }

// threadpool.cpp:107
unsigned int ThreadPoolImpl::GetInvokedWorkCount() const { return m_invokedWorkCount; }

// threadpool.cpp:109
void ThreadPoolImpl::InvokeParallelWork(ParallelWork* work, void (ParallelWork::*func)()) {
	InvokedWork* invokedWork = new InvokedWork();
	ZeroMemory(invokedWork, sizeof(InvokedWork));

	invokedWork->m_work = work;
	invokedWork->m_func = func;

	OnInvokeParallelWork(invokedWork);
}

// threadpool.cpp:122
int ThreadPoolImpl::WaitForCompletionAll(unsigned long timeout) {
	DWORD start = GetTickCount();
	while (m_invokedWorkCount > 0) {
		if (GetTickCount() - start > timeout)
			return FALSE;

		Sleep(0);
	}
	return TRUE;
}

// threadpool.cpp:133
int ThreadPoolImpl::SetThreadPriority(ThreadPriority::Enum priority) {
	if (m_threads.empty())
		return FALSE;

	for (std::vector<HANDLE>::iterator it = m_threads.begin(); it != m_threads.end(); ++it) {
		if (!::SetThreadPriority(*it, priority))
			return FALSE;
	}
	return TRUE;
}

// threadpool.cpp:144
ThreadPriority::Enum ThreadPoolImpl::GetThreadPriority() const {
	if (m_threads.empty())
		return ThreadPriority::Normal;

	HANDLE thread = m_threads.front();
	return static_cast<ThreadPriority::Enum>(::GetThreadPriority(thread));
}

// threadpool.cpp:153
int ThreadPoolImpl::Initialize(int threadCount) {
	if (threadCount == 0) {
		SYSTEM_INFO systemInfo;
		GetSystemInfo(&systemInfo);
		threadCount = systemInfo.dwNumberOfProcessors;
	}

	m_threadCount = threadCount;
	m_events.reserve(m_threadCount);
	m_threads.reserve(m_threadCount);

	for (unsigned int i = 0; i < m_threadCount; ++i) {
		HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
		HANDLE thread = NULL;

		if (event != NULL) {
			ThreadParam* param = new ThreadParam;
			param->m_pool = this; param->m_event = event;

			thread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, ThreadProc, param, 0, NULL));

			if (thread != NULL) {
				m_events.push_back(event);
				m_threads.push_back(thread);
				continue;
			}
		}

		Release();
		return FALSE;
	}

	return TRUE;
}

// threadpool.cpp:190
void ThreadPoolImpl::Release() {
	if (!m_threads.empty()) {
		std::vector<HANDLE>::iterator it;
		for (it = m_events.begin(); it != m_events.end(); ++it) SetEvent(*it);
		if (WaitForMultipleObjects(m_threads.size(), &m_threads[0], TRUE, 10000) == WAIT_TIMEOUT) {
			for (it = m_threads.begin(); it != m_threads.end(); ++it) TerminateThread(*it, static_cast<DWORD>(-1));
		}

		for (it = m_events.begin(); it != m_events.end(); ++it) CloseHandle(*it);
		for (it = m_threads.begin(); it != m_threads.end(); ++it) CloseHandle(*it);

		m_events.clear();
		m_threads.clear();
	}
}

// Threads waiting on a shared queue.
class ConcurrentQueueThreadPool : public ThreadPoolImpl {
public:
	ConcurrentQueueThreadPool();

protected:
	virtual const char* GetThreadPoolName() const;
	virtual unsigned int OnThreadProc(void* param);
	virtual void OnInvokeParallelWork(InvokedWork* work);

	ConcurrentQueue<InvokedWork*> m_queue;      // 0x2C

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(ConcurrentQueueThreadPool, 0x5C);
BEGIN_LAYOUT_CHECK(ConcurrentQueueThreadPool)
	CHECK_FIELD(ConcurrentQueueThreadPool, m_queue, 0x2C);
END_LAYOUT_CHECK

// threadpool.cpp:246
ConcurrentQueueThreadPool::ConcurrentQueueThreadPool() {}

// threadpool.cpp:249
const char* ConcurrentQueueThreadPool::GetThreadPoolName() const { return "ConcurrentQueueThreadPool"; }
// threadpool.cpp:250
unsigned int ConcurrentQueueThreadPool::OnThreadProc(void* param) {
	HANDLE event = param;
	while (WaitForSingleObject(event, 1) != WAIT_OBJECT_0) {
		while (!m_queue.IsEmpty()) {
			InvokedWork* work = NULL;
			if (m_queue.Pop(work) && work != NULL) {
				ParallelWork* parallelWork = work->m_work;
				if (parallelWork->IsWorkingFirst())
					(parallelWork->*work->m_func)();

				InterlockedDecrement(reinterpret_cast<LONG*>(&m_invokedWorkCount)); work->m_work->DecrementLockCount();
				delete work;
			}
		}
	}

	return 0;
}

// threadpool.cpp:271
void ConcurrentQueueThreadPool::OnInvokeParallelWork(InvokedWork* work) {
	work->m_work->IncrementLockCount(); InterlockedIncrement(reinterpret_cast<LONG*>(&m_invokedWorkCount));
	m_queue.Push(work);
}

typedef HANDLE (WINAPI* CreateIoCompletionPortFunc)(HANDLE, HANDLE, ULONG_PTR, DWORD);
typedef BOOL (WINAPI* GetQueuedCompletionStatusFunc)(HANDLE, LPDWORD, PULONG_PTR, LPOVERLAPPED*, DWORD);
typedef BOOL (WINAPI* PostQueuedCompletionStatusFunc)(HANDLE, DWORD, ULONG_PTR, LPOVERLAPPED);

// Threads waiting on an I/O completion port; the port functions are looked
// up in kernel32.dll at run time.
class IocpPool : public ThreadPoolImpl {
public:
	IocpPool();

	virtual int Initialize(int threadCount);
	virtual void Release();

protected:
	virtual const char* GetThreadPoolName() const;
	virtual unsigned int OnThreadProc(void* param);
	virtual void OnInvokeParallelWork(InvokedWork* work);

	HANDLE m_completionPort;                                            // 0x2C
	HMODULE m_kernel32;                                                 // 0x30
	CreateIoCompletionPortFunc m_createIoCompletionPort;                // 0x34
	GetQueuedCompletionStatusFunc m_getQueuedCompletionStatus;          // 0x38
	PostQueuedCompletionStatusFunc m_postQueuedCompletionStatus;        // 0x3C

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(IocpPool, 0x40);
BEGIN_LAYOUT_CHECK(IocpPool)
	CHECK_FIELD(IocpPool, m_completionPort, 0x2C);
	CHECK_FIELD(IocpPool, m_kernel32, 0x30);
	CHECK_FIELD(IocpPool, m_createIoCompletionPort, 0x34);
	CHECK_FIELD(IocpPool, m_getQueuedCompletionStatus, 0x38);
	CHECK_FIELD(IocpPool, m_postQueuedCompletionStatus, 0x3C);
END_LAYOUT_CHECK

// threadpool.cpp:304
IocpPool::IocpPool() {}

// threadpool.cpp:306
int IocpPool::Initialize(int threadCount) {
	m_kernel32 = LoadLibrary("kernel32.dll");
	if (m_kernel32 == NULL)
		return FALSE;

	m_createIoCompletionPort = reinterpret_cast<CreateIoCompletionPortFunc>(GetProcAddress(m_kernel32, "CreateIoCompletionPort"));
	m_getQueuedCompletionStatus = reinterpret_cast<GetQueuedCompletionStatusFunc>(GetProcAddress(m_kernel32, "GetQueuedCompletionStatus"));
	m_postQueuedCompletionStatus = reinterpret_cast<PostQueuedCompletionStatusFunc>(GetProcAddress(m_kernel32, "PostQueuedCompletionStatus"));

	if (m_createIoCompletionPort == NULL || m_getQueuedCompletionStatus == NULL || m_postQueuedCompletionStatus == NULL)
		return FALSE;

	if (!ThreadPoolImpl::Initialize(threadCount))
		return FALSE;
	m_completionPort = m_createIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_threadCount);
	return TRUE;
}

// threadpool.cpp:325
void IocpPool::Release() {
	if (m_completionPort != NULL) {
		// A completion key of 1 stops a thread.
		for (unsigned int i = 0; i < m_threadCount; ++i)
			m_postQueuedCompletionStatus(m_completionPort, 0, 1, NULL);

		ThreadPoolImpl::Release();

		CloseHandle(m_completionPort);
		FreeLibrary(m_kernel32);

		m_completionPort = NULL;
		m_kernel32 = NULL;
	}
}

// threadpool.cpp:345
const char* IocpPool::GetThreadPoolName() const { return "IocpPool"; }
// threadpool.cpp:346
unsigned int IocpPool::OnThreadProc(void* param) {
	for (;;) {
		DWORD bytes = 0;
		ULONG_PTR key = 0;
		LPOVERLAPPED overlapped;

		if (!m_getQueuedCompletionStatus(m_completionPort, &bytes, &key, &overlapped, INFINITE)) {
			if (GetLastError() == ERROR_INVALID_HANDLE)
				break;
		}

		if (key == 1)
			break;

		if (key == 0 && overlapped != NULL) {
			InvokedWork* work = reinterpret_cast<InvokedWork*>(overlapped);
			ParallelWork* parallelWork = work->m_work;
			if (parallelWork->IsWorkingFirst())
				(parallelWork->*work->m_func)();

			InterlockedDecrement(reinterpret_cast<LONG*>(&m_invokedWorkCount)); work->m_work->DecrementLockCount();
			delete work;
		}
	}
	return 0;
}

// threadpool.cpp:379
void IocpPool::OnInvokeParallelWork(InvokedWork* work) {
	work->m_work->IncrementLockCount(); InterlockedIncrement(reinterpret_cast<LONG*>(&m_invokedWorkCount));
	m_postQueuedCompletionStatus(m_completionPort, sizeof(InvokedWork), 0, &work->m_overlapped);
}

// A pool without threads: the work runs on the caller's thread.
class UnimplementPool : public ThreadPoolImpl {
public:
	virtual int Initialize(int threadCount);
	virtual void Release();

protected:
	virtual const char* GetThreadPoolName() const;
	virtual unsigned int OnThreadProc(void* param);
	virtual void OnInvokeParallelWork(InvokedWork* work);
};
CHECK_SIZE(UnimplementPool, 0x2C);

// threadpool.cpp:407
int UnimplementPool::Initialize(int threadCount) {
	return TRUE;
}
// threadpool.cpp:410
void UnimplementPool::Release() {}

// threadpool.cpp:413
const char* UnimplementPool::GetThreadPoolName() const { return "UnimplementPool"; }
// threadpool.cpp:414
unsigned int UnimplementPool::OnThreadProc(void* param) { return 0; }

// threadpool.cpp:416
void UnimplementPool::OnInvokeParallelWork(InvokedWork* work) {
	(work->m_work->*work->m_func)();
	delete work;
}

// threadpool.cpp:432
int ThreadPool::SetProcessPriority(ProcessPriority::Enum priority) {
	return SetPriorityClass(GetCurrentProcess(), priority);
}

// threadpool.cpp:436
ProcessPriority::Enum ThreadPool::GetProcessPriority() {
	return static_cast<ProcessPriority::Enum>(GetPriorityClass(GetCurrentProcess()));
}

// threadpool.cpp:442
ThreadPool::ThreadPool(ThreadPoolPolicy::Enum policy, unsigned int threadCount) : m_pool(NULL) {
	switch (policy) {
	case ThreadPoolPolicy::ConcurrentQueue:
		m_pool = new ConcurrentQueueThreadPool;
		m_pool->Initialize(threadCount);
		break;

	case ThreadPoolPolicy::Iocp:
		m_pool = new IocpPool;
		if (m_pool->Initialize(threadCount))
			break;

		delete m_pool;
		m_pool = new ConcurrentQueueThreadPool;
		m_pool->Initialize(1);
		break;

	case ThreadPoolPolicy::SingleThread:
		m_pool = new ConcurrentQueueThreadPool;
		m_pool->Initialize(1);
		break;

	case ThreadPoolPolicy::Unimplement:
		m_pool = new UnimplementPool;
		m_pool->Initialize(0);
		break;
	}
}

// threadpool.cpp:481
ThreadPool::~ThreadPool() {
	m_pool->Release();
	delete m_pool;
}

// threadpool.cpp:486
unsigned int ThreadPool::GetThreadCount() const {
	return m_pool->GetThreadCount();
}

// threadpool.cpp:490
unsigned int ThreadPool::GetInvokedWorkCount() const {
	return m_pool->GetInvokedWorkCount();
}

// threadpool.cpp:494
void ThreadPool::InternalInvokeParallelWork(ParallelWork* work, void (ParallelWork::*func)()) {
	m_pool->InvokeParallelWork(work, func);
}

// threadpool.cpp:498
int ThreadPool::WaitForCompletionAll(unsigned long timeout) {
	return m_pool->WaitForCompletionAll(timeout);
}

// threadpool.cpp:502
int ThreadPool::SetThreadPriority(ThreadPriority::Enum priority) {
	return m_pool->SetThreadPriority(priority);
}

// threadpool.cpp:506
ThreadPriority::Enum ThreadPool::GetThreadPriority() const {
	return m_pool->GetThreadPriority();
}

} // namespace Threading
} // namespace Py
