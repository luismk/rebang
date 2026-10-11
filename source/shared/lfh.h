// Original: d:\build\custom\temp\code\shared\lfh.h
#ifndef PG_SHARED_LFH_H
#define PG_SHARED_LFH_H

#include <windows.h>

#include "..\..\defines.h"

// Switches every process heap to the low-fragmentation heap (heap
// compatibility information 2), through Kernel32 entry points looked up at
// run time (they are missing on Windows 2000). Skipped under a debugger.
class LFHInstaller {
public:
	typedef DWORD (WINAPI* GetProcessHeapsFn)(DWORD count, PHANDLE heaps);
	typedef BOOL (WINAPI* HeapQueryInformationFn)(HANDLE heap, int infoClass, PVOID info, SIZE_T length, PSIZE_T returnLength);
	typedef BOOL (WINAPI* HeapSetInformationFn)(HANDLE heap, int infoClass, PVOID info, SIZE_T length);
	typedef BOOL (WINAPI* IsDebuggerPresentFn)(void);

	// The process heaps, as GetProcessHeaps returns them.
	class _Handles {
	public:
		// lfh.h:17
		_Handles(DWORD count, GetProcessHeapsFn getProcessHeaps) {
			m_handles = new HANDLE[count];
			getProcessHeaps(count, m_handles);
		}
		// lfh.h:21
		~_Handles(void) { delete[] m_handles; }
		// lfh.h:23
		void* operator[](unsigned int index) { return m_handles[index]; }

	private:
		HANDLE* m_handles; // 0x0
	};

	// Kernel32 and the entry points InstallLFH needs.
	class _Modules {
		LAYOUT_CHECK_FRIEND;
	public:
		// lfh.h:36
		_Modules(void) {
			m_hKernel32 = LoadLibraryA("Kernel32.dll");

			if (m_hKernel32) {
				m_pGetProcessHeaps = (GetProcessHeapsFn)GetProcAddress(m_hKernel32, "GetProcessHeaps");
				m_pHeapQueryInformation = (HeapQueryInformationFn)GetProcAddress(m_hKernel32, "HeapQueryInformation");
				m_pHeapSetInformation = (HeapSetInformationFn)GetProcAddress(m_hKernel32, "HeapSetInformation");
				m_pIsDebuggerPresent = (IsDebuggerPresentFn)GetProcAddress(m_hKernel32, "IsDebuggerPresent");
			}
		}

		// lfh.h:50
		~_Modules(void) {
			if (m_hKernel32)
				FreeLibrary(m_hKernel32);
		}

		// lfh.h:56
		bool IsValid(void) const {
			if (!m_hKernel32)
				return false;

			return m_pGetProcessHeaps && m_pHeapQueryInformation && m_pHeapSetInformation && m_pIsDebuggerPresent;
		}

		// lfh.h:64
		bool InstallLFH(void) {
			if (!IsValid())
				return false;

			if (m_pIsDebuggerPresent())
				return true;

			DWORD count = m_pGetProcessHeaps(0, NULL);

			if (!count)
				return false;

			_Handles heaps(count, m_pGetProcessHeaps);

			for (DWORD i = 0; i < count; i++) {
				ULONG info;
				SIZE_T length;
				if (!m_pHeapQueryInformation(heaps[i], 0, &info, sizeof(info), &length))
					return false;

				if (info == 1) {
					info = 2;
					if (!m_pHeapSetInformation(heaps[i], 0, &info, sizeof(info)))
						return false;
				}
			}

			return true;
		}

	private:
		HMODULE m_hKernel32;                            // 0x00
		GetProcessHeapsFn m_pGetProcessHeaps;           // 0x04
		HeapQueryInformationFn m_pHeapQueryInformation; // 0x08
		HeapSetInformationFn m_pHeapSetInformation;     // 0x0C
		IsDebuggerPresentFn m_pIsDebuggerPresent;       // 0x10
	};

	// lfh.h:123
	static bool install(void) {
		_Modules modules;

		return modules.InstallLFH();
	}
};
CHECK_SIZE(LFHInstaller::_Modules, 0x14);

#endif // PG_SHARED_LFH_H
