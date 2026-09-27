// Minimal tool for tracing VC7.1 cl.exe/c2.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREFERRED_BASE 0x10700000
#define CURRENT_FUNCTION 0x108AB128
#define CURRENT_ENTRY 0x108839BC
#define MAX_BREAKPOINTS 64

static HANDLE process;
static DWORD base;
static DWORD breakpoint[MAX_BREAKPOINTS], address[MAX_BREAKPOINTS];
static BYTE saved[MAX_BREAKPOINTS];
static int dwords[MAX_BREAKPOINTS], count;

static DWORD read32(DWORD at)
{
	DWORD value = 0, done = 0;
	ReadProcessMemory(process, (void*)at, &value, 4, &done);
	return value;
}

static void read_string(DWORD at, char* out, int size)
{
	DWORD done = 0;
	memset(out, 0, size);
	ReadProcessMemory(process, (void*)at, out, size - 1, &done);
	out[size - 1] = 0;
}

static void poke(DWORD at, BYTE value)
{
	DWORD done, old;
	VirtualProtectEx(process, (void*)at, 1, PAGE_EXECUTE_READWRITE, &old);
	WriteProcessMemory(process, (void*)at, &value, 1, &done);
	FlushInstructionCache(process, (void*)at, 1);
	VirtualProtectEx(process, (void*)at, 1, old, &done);
}

static DWORD rebase(DWORD preferred)
{
	return base + (preferred - PREFERRED_BASE);
}

static void current_function(char* out, int size)
{
	DWORD function = read32(rebase(CURRENT_FUNCTION));
	DWORD symbol = function ? read32(function) : 0;
	DWORD entry;
	if (symbol)
	{
		read_string(read32(symbol + 4), out, size);
		return;
	}
	entry = read32(rebase(CURRENT_ENTRY));
	if (entry)
	{
		read_string(read32(read32(entry + 12) + 4), out, size);
	}
	else
	{
		strcpy(out, "-");
	}
}

static int printable(DWORD at, char* out, int size)
{
	int i;
	if (at < 0x10000)
	{
		return 0;
	}
	read_string(at, out, size);
	for (i = 0; out[i]; i++)
	{
		if (out[i] < 32 || out[i] >= 127)
		{
			return 0;
		}
	}
	return i >= 2;
}

static void dump(const char* label, DWORD at, int n)
{
	char text[48];
	int i;
	printf("  %s@%08lx:", label, at);
	for (i = 0; i < n; i++)
	{
		printf(" %08lx", read32(at + 4 * i));
	}
	printf("\n");
	for (i = 0; i < n; i++)
	{
		DWORD value = read32(at + 4 * i);
		if (printable(value, text, sizeof(text)))
		{
			printf("    [%d] -> \"%s\"\n", i, text);
		}
		else if (value >= 0x10000 &&
			printable(read32(value + 4), text, sizeof(text)))
		{
			printf("    [%d]+4 -> \"%s\"\n", i, text);
		}
	}
}

static void report(int which, CONTEXT* c)
{
	int n = dwords[which];
	printf(
		"BP %08lx eax=%08lx ecx=%08lx edx=%08lx ebx=%08lx esi=%08lx edi=%08lx ebp=%08lx"
		" ret=%08lx s=%08lx %08lx %08lx %08lx %08lx %08lx\n",
		breakpoint[which], c->Eax, c->Ecx, c->Edx, c->Ebx, c->Esi, c->Edi,
		c->Ebp, read32(c->Esp) - base + PREFERRED_BASE, read32(c->Esp + 4),
		read32(c->Esp + 8), read32(c->Esp + 12), read32(c->Esp + 16),
		read32(c->Esp + 20), read32(c->Esp + 24));
	if (n)
	{
		dump("eax", c->Eax, n);
		dump("ecx", c->Ecx, n);
		dump("edx", c->Edx, n);
		dump("esi", c->Esi, n);
		dump("arg0", read32(c->Esp + 4), n);
	}
	fflush(stdout);
}

int main(int argc, char** argv)
{
	STARTUPINFOA startup;
	PROCESS_INFORMATION info;
	DEBUG_EVENT event;
	char command[32768], name[256];
	const char* wanted;
	DWORD stepping = 0, step_address = 0;
	int i, running = 1;
	FILE* f;

	if (argc < 4)
	{
		fprintf(stderr, "usage: c2trace CMDFILE FUNCTION ADDRESS[:N]...\n");
		return 2;
	}
	wanted = argv[2];
	for (i = 3; i < argc && count < MAX_BREAKPOINTS; i++, count++)
	{
		char* colon = strchr(argv[i], ':');
		breakpoint[count] = strtoul(argv[i], NULL, 16);
		dwords[count] = colon ? atoi(colon + 1) : 0;
	}
	f = fopen(argv[1], "rb");
	if (!f)
	{
		return 3;
	}
	command[fread(command, 1, sizeof(command) - 1, f)] = 0;
	fclose(f);

	memset(&startup, 0, sizeof(startup));
	startup.cb = sizeof(startup);
	if (!CreateProcessA(NULL, command, NULL, NULL, FALSE,
			DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &startup, &info))
	{
		return 4;
	}
	process = info.hProcess;

	while (running && WaitForDebugEvent(&event, INFINITE))
	{
		DWORD status = DBG_CONTINUE;
		if (event.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
		{
			DWORD dll = (DWORD)event.u.LoadDll.lpBaseOfDll;
			DWORD exports = read32(dll + read32(dll + 0x3c) + 0x78);
			read_string(dll + read32(dll + exports + 12), name, sizeof(name));
			if (!_stricmp(name, "c2_l.dll"))
			{
				DWORD done;
				base = dll;
				for (i = 0; i < count; i++)
				{
					address[i] = rebase(breakpoint[i]);
					ReadProcessMemory(process, (void*)address[i], &saved[i], 1,
						&done);
					poke(address[i], 0xcc);
				}
			}
			if (event.u.LoadDll.hFile)
			{
				CloseHandle(event.u.LoadDll.hFile);
			}
		}
		else if (event.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
		{
			DWORD code = event.u.Exception.ExceptionRecord.ExceptionCode;
			DWORD at =
				(DWORD)event.u.Exception.ExceptionRecord.ExceptionAddress;
			int which = -1;
			for (i = 0; i < count; i++)
			{
				if (at == address[i])
				{
					which = i;
				}
			}
			if (code == EXCEPTION_BREAKPOINT && which >= 0)
			{
				HANDLE thread =
					OpenThread(THREAD_ALL_ACCESS, FALSE, event.dwThreadId);
				CONTEXT c;
				memset(&c, 0, sizeof(c));
				c.ContextFlags = CONTEXT_FULL;
				GetThreadContext(thread, &c);
				current_function(name, sizeof(name));
				if (wanted[0] == '*' || !strncmp(name, wanted, strlen(wanted)))
				{
					report(which, &c);
				}
				// Restore original byte then single-step and restore bp
				poke(at, saved[which]);
				c.Eip = at;
				c.EFlags |= 0x100;
				SetThreadContext(thread, &c);
				CloseHandle(thread);
				stepping = event.dwThreadId;
				step_address = at;
			}
			else if (code == EXCEPTION_SINGLE_STEP &&
				stepping == event.dwThreadId)
			{
			    // Restore breakpoint after singlestepping over bp'd instruction
				poke(step_address, 0xcc);
				stepping = 0;
			}
			else if (code != EXCEPTION_BREAKPOINT)
			{
				status = DBG_EXCEPTION_NOT_HANDLED;
			}
		}
		else if (event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT)
		{
			running = 0;
		}
		ContinueDebugEvent(event.dwProcessId, event.dwThreadId, status);
	}
	return 0;
}
