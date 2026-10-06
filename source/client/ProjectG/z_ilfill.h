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
			ILFILLFN(10) ILFILLFN(11) ILFILLFN(12) ILFILLFN(13) \
			ILFILLFN(14) ILFILLFN(15) \
			void g() { ILFILL64 } \
		}; \
	}

#define ILFILL0 \
	namespace ILFILL0 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN(0) ILFILLFN(1) ILFILLFN(2) ILFILLFN(3) \
			ILFILLFN(4) ILFILLFN(5) ILFILLFN(6) ILFILLFN(7) \
			ILFILLFN(8) \
		}; \
	}

#define ILFILL2 \
	namespace ILFILL2 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN10(1) ILFILLFN10(2) ILFILLFN(0) ILFILLFN(1) \
			void g() { ILFILL64 ILFILL64 ILFILL64 ILFILL16 ILFILL16 ILFILL16 } \
		}; \
	}

#define ILFILL2A \
	namespace ILFILL2A \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN(0) ILFILLFN(1) void f2() { ILFILL256 ILFILL64 ILFILL16 ILFILL16 ILFILL4 ILFILL4 v += 1; v += 1; } \
		}; \
	}

#define ILFILL3 \
	namespace ILFILL3 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN(0) ILFILLFN(1) ILFILLFN(2) ILFILLFN(3) \
			void g() { ILFILL64 ILFILL16 ILFILL16 ILFILL4 ILFILL4 v += 1; v += 1; } \
		}; \
	}

#define ILFILLB1 \
	template <int N> struct TuFill { int v; void g() { ILFILL256 ILFILL16 ILFILL4 v += 1; v += 1; v += 1; } }; \
	template <int N> struct TuMid { void g() { TuFill<N>().g(); } }; \
	template <int N> struct TuPad \
	{ \
		int v; \
		void g() { ILFILL256 ILFILL16 ILFILL16 ILFILL16 v += 1; v += 1; TuMid<N>().g(); } \
	}; \
	namespace TUPAD1 \
	{ \
		struct Pad \
		{ \
			int v; \
			ILFILLFN(0) ILFILLFN(1) ILFILLFN(2) ILFILLFN(3) \
			void g() { ILFILL16 ILFILL16 v += 1; v += 1; v += 1; ILFILL64 ILFILL64 ILFILL64 ILFILL16 ILFILL4 ILFILL4 ILFILL4 v += 1; v += 1; v += 1; } \
		}; \
	}

#define ILFILLB2 \
	namespace TUPAD2 \
	{ \
		struct Pad \
		{ \
			void g() { TuPad<2>().g(); } \
		}; \
	}
#define ILFILLW1 \
	namespace TUPADW1 \
	{ \
		struct Pad \
		{ \
			int v; \
			void g() { ILFILL64 ILFILL64 ILFILL16 ILFILL16 ILFILL16 v += 1; ILFILL64 ILFILL64 ILFILL64 ILFILL16 ILFILL16 ILFILL16 ILFILL4 ILFILL4 v += 1; v += 1; } \
		}; \
	}
// clang-format on
