// Original: d:\build\custom\temp\code\client\projectg\parallelwork.h (path assumed: no inline code of it is in the line info)
#ifndef PG_CLIENT_PROJECTG_PARALLELWORK_H
#define PG_CLIENT_PROJECTG_PARALLELWORK_H

#include "..\..\defines.h"

namespace Py {
namespace Threading {

struct ParallelWorkPolicy {
	// Only the value 1 is seen (~ParallelWork clears the working flag for
	// it); the enumerator names are not in the PDB.
	enum Enum {
		Default = 0,
		ClearWorkingOnDestroy = 1
	};
};

class ParallelWork {
public:
	bool IsWorkingFirst() const;
	long IncrementLockCount();
	long DecrementLockCount();

protected:
	ParallelWork(ParallelWorkPolicy::Enum policy);
	virtual ~ParallelWork();        // vtable slot 0 (??_G)

	// Both counters are volatile like the Interlocked* targets (LONG volatile*):
	// the constructor stores m_lockCount before the vptr only when both are.
	volatile long m_lockCount;                  // 0x4 (Interlocked{In,De}crement)
	volatile long m_working;                    // 0x8 (1 at construction, InterlockedExchange'd to 0)
	ParallelWorkPolicy::Enum m_policy;  // 0xC

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(ParallelWork, 0x10);
BEGIN_LAYOUT_CHECK(ParallelWork)
	CHECK_FIELD(ParallelWork, m_lockCount, 0x4);
	CHECK_FIELD(ParallelWork, m_working, 0x8);
	CHECK_FIELD(ParallelWork, m_policy, 0xC);
END_LAYOUT_CHECK

} // namespace Threading
} // namespace Py

#endif
