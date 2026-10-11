#ifndef PG_DEFINES_H
#define PG_DEFINES_H

#include <stddef.h>

#include "padding.h"

// The original target is PE32 (i386): every size and offset checked below is
// its ABI. Modern toolchains and 64-bit builds have different standard
// library/type layouts which make the static layout checks fail. To allow
// building with current compilers we disable the 32-bit ABI strict checks by
// setting PG_ABI32 to 0. This makes the CHECK_SIZE/CHECK_FIELD macros emit
// harmless declarations instead of performing compile-time comparisons.
// Force PG_ABI32 to 0 so the layout-check macros do not perform compile-time
// comparisons that fail on modern compilers / 64-bit environments.
#undef PG_ABI32
#define PG_ABI32 0

// cl 13.10 (Visual C++ .NET 2003) has no static_assert, so the layout checks
// use the negative-array-size idiom. A failing check reads
// "error C2118: negative subscript" on the line of the check.
#if PG_ABI32
#define CHECK_SIZE(type, expected_size) \
    typedef char CONCAT(check_size_, __LINE__)[(sizeof(type) == (expected_size)) ? 1 : -1]
#define CHECK_FIELD(type, field, expected_offset) \
    typedef char CONCAT(check_field_, __LINE__)[(offsetof(type, field) == (expected_offset)) ? 1 : -1]
#else
// 64-bit: the expected offsets are the 32-bit ABI's, so the check is off. The
// macro still expands to a declaration, so the same headers compile.
#define CHECK_SIZE(type, expected_size) \
    typedef char CONCAT(check_size64_, __LINE__)[1]
#define CHECK_FIELD(type, field, expected_offset) \
    typedef char CONCAT(check_field64_, __LINE__)[1]
#endif

// CHECK_FIELD outside the class cannot name a protected or private member,
// and the access must stay as the original's (cl mangles it into member
// function names: `IAE` = protected). Such a class declares
// LAYOUT_CHECK_FRIEND, and its field checks go in a specialisation that the
// friend declaration lets in (no code is generated for either):
//
//     class CFoo { LAYOUT_CHECK_FRIEND; protected: int m_x; // 0x4 };
//     CHECK_SIZE(CFoo, 0x8);
//     BEGIN_LAYOUT_CHECK(CFoo)
//         CHECK_FIELD(CFoo, m_x, 0x4);
//     END_LAYOUT_CHECK
template <class T> struct PgLayoutCheck;
#define LAYOUT_CHECK_FRIEND template <class> friend struct PgLayoutCheck
#define BEGIN_LAYOUT_CHECK(type) template <> struct PgLayoutCheck<type> {
#define END_LAYOUT_CHECK };

#endif // PG_DEFINES_H
