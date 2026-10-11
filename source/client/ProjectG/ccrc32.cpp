// Original: d:\build\custom\temp\code\client\projectg\ccrc32.cpp
#include "cCRC32.h"

#include <algorithm>
#include <commctrl.h>
#include <fcntl.h>
#include <io.h>
#include <mbstring.h>
#include <process.h>
#include <share.h>
#include <stdio.h>
#include <string.h>

// Sent to the parent of the progress window when a CRC is done
// (wParam = thread handle, lParam = CRC).
#define WM_CRC_DONE 0xADB1
// Sent to CRC_MT's parent window when a file's CRC differs (lParam = task).
#define WM_CRC_MISMATCH 0xADB2

// CRC_32 reads files in blocks of this size.
#define CRC_BLOCK_SIZE 0x19000

unsigned int __stdcall ThreadCRCCheckProc(void *param);

// Parameter block of CRC32ThreadProc, allocated by CalcCRC (new 0x118).
struct sCRC_THREADPARAM {
	CRC_32 *m_crc32;                         // 0x0
	char m_fileName[0x104];                  // 0x4, empty for a memory buffer
	unsigned char *m_buffer;                 // 0x108, copy of the buffer, 0 for a file
	unsigned int m_size;                     // 0x10C
	HWND m_hWnd;                             // 0x110, notified with the result
	HANDLE m_hThread;                        // 0x114
};

CHECK_SIZE(sCRC_THREADPARAM, 0x118);
BEGIN_LAYOUT_CHECK(sCRC_THREADPARAM)
	CHECK_FIELD(sCRC_THREADPARAM, m_crc32, 0x0);
	CHECK_FIELD(sCRC_THREADPARAM, m_fileName, 0x4);
	CHECK_FIELD(sCRC_THREADPARAM, m_buffer, 0x108);
	CHECK_FIELD(sCRC_THREADPARAM, m_size, 0x10C);
	CHECK_FIELD(sCRC_THREADPARAM, m_hWnd, 0x110);
	CHECK_FIELD(sCRC_THREADPARAM, m_hThread, 0x114);
END_LAYOUT_CHECK

// ccrc32.cpp:45
unsigned long __stdcall CRC_32::CRC32ThreadProc(void *param) {
	sCRC_THREADPARAM *p = (sCRC_THREADPARAM *)param;
	unsigned long crc = 0xFFFFFFFF;
	unsigned char buffer[CRC_BLOCK_SIZE];
	HWND hWnd = NULL;

	if (IsWindow(p->m_hWnd)) {
		hWnd = p->m_hWnd;
		PostMessage(hWnd, PBM_SETPOS, 0, 0);
		PostMessage(hWnd, PBM_SETRANGE32, 0, 100);
	}

	if (p->m_buffer) {
		for (unsigned int offset = 0; offset < p->m_size; offset += CRC_BLOCK_SIZE) {
			p->m_crc32->Calculate(p->m_buffer + offset, p->m_size - offset > CRC_BLOCK_SIZE ? CRC_BLOCK_SIZE : p->m_size - offset, crc, 0);
			if (IsWindow(hWnd)) {
				WPARAM percent = offset > p->m_size ? 100 : (WPARAM)((double)offset / p->m_size * 100);
				PostMessage(hWnd, PBM_SETPOS, percent, 0);
			}
		}
	} else if (p->m_fileName != NULL) {
		__int64 done = 0;
		int fd = _sopen(p->m_fileName, _O_BINARY | _O_SEQUENTIAL, _SH_DENYWR);
		if (fd != -1) {
			_lseeki64(fd, 0, SEEK_SET);
			__int64 total = _lseeki64(fd, 0, SEEK_END);
			_lseeki64(fd, 0, SEEK_SET);

			unsigned int read = CRC_BLOCK_SIZE;
			while (read == CRC_BLOCK_SIZE) {
				read = _read(fd, buffer, CRC_BLOCK_SIZE);
				if (read) {
					p->m_crc32->Calculate(buffer, read, crc, 0);
					if (IsWindow(hWnd)) {
						done += read;
						PostMessage(hWnd, PBM_SETPOS, (WPARAM)((double)done / total * 100), 0);
					}
				}
			}
			close(fd);
		}
	}

	unsigned long result = ~crc;

	if (IsWindow(hWnd)) {
		PostMessage(GetParent(hWnd), WM_CRC_DONE, (WPARAM)p->m_hThread, result);
	}

	delete p->m_buffer;
	delete p;

	return result;
}

// ccrc32.cpp:132
CRC_32::CRC_32(void) {
	unsigned long polynomial = 0xEDB88320;

	for (int i = 0; i <= 0xFF; i++) {
		m_table[i] = Reflect(i, 8) << 24;
		for (int j = 0; j < 8; j++) m_table[i] = (m_table[i] << 1) ^ (m_table[i] & (1 << 31) ? polynomial : 0);
		m_table[i] = Reflect(m_table[i], 32);
	}
}

// ccrc32.cpp:168
unsigned long CRC_32::Reflect(unsigned long ref, char ch) {
	unsigned long value = 0;

	// Swap bit 0 for bit (ch - 1), bit 1 for bit (ch - 2), ...
	for (int i = 1; i < (ch + 1); i++) {
		if (ref & 1)
			value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

// ccrc32.cpp:202
void CRC_32::Calculate(unsigned char *const buffer, unsigned int size, unsigned long &crc, unsigned long sleepTime) {
	unsigned char *p = buffer;
	int count = 128;

	while (size--) {
		crc = (crc >> 8) ^ m_table[(crc & 0xFF) ^ *p++];
		if (sleepTime) {
			if (--count == 0) {
				count = 128;
				Sleep(sleepTime);
			}
		}
	}
}

// ccrc32.cpp:249
unsigned long CRC_32::CalcCRC(void *buffer, unsigned int size, HWND hWnd) {
	if (buffer == NULL || size == 0)
		return 0;

	if (!IsWindow(hWnd)) {
		unsigned long crc = 0xFFFFFFFF;
		Calculate((unsigned char *)buffer, size, crc, 0);
		return ~crc;
	}

	sCRC_THREADPARAM *param = new sCRC_THREADPARAM;
	DWORD threadId;
	HANDLE hThread = CreateThread(NULL, 0, CRC32ThreadProc, param, CREATE_SUSPENDED, &threadId);
	if (hThread) {
		param->m_crc32 = this;
		param->m_fileName[0] = 0;
		param->m_buffer = new unsigned char[size];
		memcpy(param->m_buffer, buffer, size);
		param->m_size = size;
		param->m_hWnd = hWnd;
		param->m_hThread = hThread;
		ResumeThread(hThread);
	} else {
		delete param;
	}
	return (unsigned long)hThread;
}

// ccrc32.cpp:306
unsigned long CRC_32::CalcCRC(const char *fileName, HWND hWnd) {
	DWORD attributes = GetFileAttributesA(fileName);
	if (attributes == 0xFFFFFFFF || (attributes & FILE_ATTRIBUTE_DIRECTORY))
		return 0;

	sCRC_THREADPARAM *param = new sCRC_THREADPARAM;
	param->m_crc32 = this;
	_mbsnbcpy((unsigned char *)param->m_fileName, (const unsigned char *)fileName, 0x104);
	param->m_buffer = NULL;
	param->m_size = 0;
	param->m_hWnd = hWnd;
	param->m_hThread = NULL;

	if (!IsWindow(hWnd))
		return CRC32ThreadProc(param);

	DWORD threadId;
	HANDLE hThread = CreateThread(NULL, 0, CRC32ThreadProc, param, CREATE_SUSPENDED, &threadId);
	if (hThread) {
		param->m_hThread = hThread;
		ResumeThread(hThread);
	} else {
		delete param;
	}
	return (unsigned long)hThread;
}

namespace {

// ccrc32.cpp:345
struct _delete {
	template <typename T>
	void operator()(const T *p) const {
		delete p;
	}
};

} // namespace

// ccrc32.cpp:355
CRC_MT::CRC_MT(void)
	: m_hParentWnd(0)
	, m_hThread(0)
	, m_paused(0)
	, m_terminate(0)
	, m_sleepTime(0) {
	m_tasks.reserve(100);
}

// ccrc32.cpp:366
CRC_MT::~CRC_MT(void) {
	Terminate();
	Reset();
}

// ccrc32.cpp:372
void CRC_MT::GetWork(std::vector<sCRC_CHKTASK *> &work) {
	work.assign(m_tasks.begin(), m_tasks.end());
}

// ccrc32.cpp:377
int CRC_MT::AddWork(const char *fileName, unsigned long crc) {
	if (m_hThread)
		return 0;

	sCRC_CHKTASK *task = new sCRC_CHKTASK;
	_mbsnbcpy((unsigned char *)task->m_fileName, (const unsigned char *)fileName, 0x104);
	task->m_crc = crc;
	m_tasks.push_back(task);

	return 1;
}

// ccrc32.cpp:391
void CRC_MT::Reset(void) {
	m_hThread = 0;
	std::for_each(m_tasks.begin(), m_tasks.end(), _delete());
	m_tasks.~vector();
}

// ccrc32.cpp:398
void CRC_MT::Terminate(void) {
	m_terminate = 1;
	if (m_paused) {
		Resume();
	}

	if (m_hThread) {
		WaitForSingleObject(m_hThread, INFINITE);
		m_hThread = 0;
	}
}

// ccrc32.cpp:413
unsigned int __stdcall ThreadCRCCheckProc(void *param) {
	CRC_MT *pThis = (CRC_MT *)param;
	std::vector<CRC_MT::sCRC_CHKTASK *> work;

	pThis->GetWork(work);
	DWORD startTick = GetTickCount();

	CRC_32 crc32;
	std::vector<CRC_MT::sCRC_CHKTASK *>::iterator it;
	for (it = work.begin(); it != work.end(); ++it) {
		CRC_MT::sCRC_CHKTASK *task = *it;
		unsigned long crc = 0xFFFFFFFF;
		unsigned char buffer[CRC_BLOCK_SIZE];

		int fd = _sopen(task->m_fileName, _O_BINARY | _O_SEQUENTIAL, _SH_DENYWR);
		if (fd != -1) {
			_lseeki64(fd, 0, SEEK_SET);
			_lseeki64(fd, 0, SEEK_END);
			_lseeki64(fd, 0, SEEK_SET);

			unsigned int read = CRC_BLOCK_SIZE;
			while (read == CRC_BLOCK_SIZE) {
				read = _read(fd, buffer, CRC_BLOCK_SIZE);
				if (read)
					crc32.Calculate(buffer, read, crc, 0);

				if (pThis->IsTerminate()) {
					close(fd);
					return 0;
				}
			}
			close(fd);
		}

		if (task->m_crc != ~crc) {
			if (IsWindow(pThis->GetParentWnd())) {
				PostMessage(pThis->GetParentWnd(), WM_CRC_MISMATCH, 0, (LPARAM)task);
				return 0;
			}
		}
	}

	if (IsWindow(pThis->GetParentWnd())) {
		PostMessage(pThis->GetParentWnd(), WM_CRC_DONE, GetTickCount() - startTick, 0);
	}

	pThis->Reset();
	return 0;
}

// ccrc32.cpp:483
int CRC_MT::Begin(HWND hParentWnd) {
	if (m_hThread)
		return 0;

	m_hParentWnd = hParentWnd;
	m_hThread = (HANDLE)_beginthreadex(0, 0, ThreadCRCCheckProc, this, 0, 0);
	return m_hThread != 0;
}

// ccrc32.cpp:500
void CRC_MT::Pause(void) {
	if (m_hThread && !m_paused) {
		SuspendThread(m_hThread);
		m_paused = 1;
	}
}

// ccrc32.cpp:513
void CRC_MT::Resume(void) {
	if (m_hThread && m_paused) {
		ResumeThread(m_hThread);
		m_paused = 0;
	}
}
