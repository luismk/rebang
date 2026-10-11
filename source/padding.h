#ifndef PG_PADDING_H
#define PG_PADDING_H

#define CONCAT_IMPL(x, y) x ## y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

// Members not transcribed yet (the comment names what they cover), and bytes
// of unknown purpose. The length is the 32-bit ABI's: a 64-bit build lays the
// type out only once the real members are transcribed (docs/PORTABILITY.md).
#define PADDING(start, end) char CONCAT(pad_, __LINE__)[(end) - (start)]
#define UNKNOWN(start, end) char CONCAT(unk_, __LINE__)[(end) - (start)]

#endif // PG_PADDING_H
