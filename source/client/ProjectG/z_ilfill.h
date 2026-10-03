#pragma once

// HACK! This file contains filler to balance IL offsets between the PCH and TU.
// Ideally this file will disappear as we zero in on the actual contents of the
// PCH and TUs, but it will take a while.

// clang-format off
#define ILFILL4 v += 1; v += 1; v += 1; v += 1;
#define ILFILL16 ILFILL4 ILFILL4 ILFILL4 ILFILL4
#define ILFILL64 ILFILL16 ILFILL16 ILFILL16 ILFILL16
#define ILFILL256 ILFILL64 ILFILL64 ILFILL64 ILFILL64

#define ILFILLFN(n) void f##n() { ILFILL256 }
#define ILFILLFN10(n) \
	ILFILLFN(n##0) ILFILLFN(n##1) ILFILLFN(n##2) ILFILLFN(n##3) \
	ILFILLFN(n##4) ILFILLFN(n##5) ILFILLFN(n##6) ILFILLFN(n##7) \
	ILFILLFN(n##8) ILFILLFN(n##9)

#define ILFILL1 \
	namespace ILFILL1 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN10(1) ILFILLFN10(2) ILFILLFN10(3) ILFILLFN10(4) \
			ILFILLFN10(5) ILFILLFN10(6) ILFILLFN10(7) ILFILLFN10(8) \
			ILFILLFN10(9) ILFILLFN(0) ILFILLFN(1) ILFILLFN(2) \
			ILFILLFN(3) \
			void g() { ILFILL64 ILFILL4 ILFILL4 ILFILL4 v += 1; v += 1; } \
		}; \
	}

#define ILFILL2 \
	namespace ILFILL2 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN10(1) ILFILLFN10(2) ILFILLFN(0) ILFILLFN(1) \
			ILFILLFN(2) \
			void g() { ILFILL64 } \
		}; \
	}

#define ILFILL3 \
	namespace ILFILL3 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN(0) ILFILLFN(1) ILFILLFN(2) ILFILLFN(3) \
			ILFILLFN(4) ILFILLFN(5) \
			void g() { ILFILL16 ILFILL16 } \
		}; \
	}
// clang-format on
