// Not an original header: boot-time tracing of the code paths src/ has not
// transcribed yet. Without PG_BOOT_TRACE (the scored build never defines it)
// both macros expand to ((void)0) and their arguments are not evaluated, so
// the generated code is unchanged. The CMake build defines PG_BOOT_TRACE and
// links cmake/boot_todo.c, which prints "[TODO] <text>" to the console, the
// debugger and stub_calls.log.
//
//   PG_BOOT_TODO("taskmanager.cpp:13359 case 0x1d0: 110 bytes");
//       once per expansion (a static guard per site)
//   PG_BOOT_TODOF(("OnPacketCommon: unhandled packet 0x%04x", type));
//       printf-style, double parentheses (cl 13.10 has no variadic macros);
//       printed once per distinct resulting text
#ifndef PG_CLIENT_PROJECTG_PG_BOOT_TODO_H
#define PG_CLIENT_PROJECTG_PG_BOOT_TODO_H

#ifdef PG_BOOT_TRACE

#ifdef __cplusplus
extern "C" {
#endif
void __cdecl pg_boot_todo(const char *site);
void __cdecl pg_boot_todof(const char *format, ...);
#ifdef __cplusplus
}
#endif

#define PG_BOOT_TODO(text) \
	do { \
		static bool pg_boot_todo_seen = false; \
		if (!pg_boot_todo_seen) { \
			pg_boot_todo_seen = true; \
			pg_boot_todo(text); \
		} \
	} while (0)
#define PG_BOOT_TODOF(args) pg_boot_todof args

#else

#define PG_BOOT_TODO(text) ((void)0)
#define PG_BOOT_TODOF(args) ((void)0)

#endif

#endif // PG_CLIENT_PROJECTG_PG_BOOT_TODO_H
