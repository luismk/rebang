// Original: d:\build\custom\temp\code\shared\coord.h
#ifndef PG_SHARED_COORD_H
#define PG_SHARED_COORD_H

#include "..\..\defines.h"

// Coordinate and rectangle templates shared by client and server (PDB layouts;
// the original keeps the inline bodies in coord.inl, here they follow the
// classes with their coord.inl line). Only the members other code uses so far
// are written: the PDB also lists _Rectangle's tl/br _Coordinates aliasing
// left/top and right/bottom, and the other methods of both templates.

// coord.h
template <class T>
struct _Coordinates {
	T x;    // 0x0
	T y;    // sizeof(T)

	// coord.inl:7
	_Coordinates(void) {}
};

template <class T>
struct _Rectangle {
	// coord.h:77
	void Reset(void) { left = top = right = bottom = 0; }
	T Height(void) const;
	T Width(void) const;

	_Rectangle(const _Rectangle<T>& rhs);
	_Rectangle(void);

	_Rectangle<T>& operator=(const _Rectangle<T>& rhs);

	// coord.h: the fields alias tl/br (PDB), as in the original header.
	union {
		struct {
			_Coordinates<T> tl;    // 0x0
			_Coordinates<T> br;    // sizeof(_Coordinates<T>)
		};
		struct {
			T left;      // 0x0
			T top;       // sizeof(T)
			T right;     // 2 * sizeof(T)
			T bottom;    // 3 * sizeof(T)
		};
	};
};

struct _RectangleSHORT : public _Rectangle<short> {
	// coord.h:160
	_RectangleSHORT(void) {}
};

// coord.inl:236
template <class T>
inline _Rectangle<T>::_Rectangle(void) {
}

// coord.inl:242
template <class T>
inline _Rectangle<T>::_Rectangle(const _Rectangle<T>& rhs) {
	left = rhs.left;
	top = rhs.top;
	right = rhs.right;
	bottom = rhs.bottom;
}

// coord.inl:310
template <class T>
inline T _Rectangle<T>::Height(void) const {
	return bottom - top;
}

template <class T>
inline T _Rectangle<T>::Width(void) const {
	return right - left;
}

// coord.inl:412
template <class T>
inline _Rectangle<T>& _Rectangle<T>::operator=(const _Rectangle<T>& rhs) {
	left = rhs.left;
	top = rhs.top;
	right = rhs.right;
	bottom = rhs.bottom;
	return *this;
}

CHECK_SIZE(_Coordinates<short>, 0x4);
CHECK_SIZE(_Rectangle<short>, 0x8);
CHECK_FIELD(_Rectangle<short>, left, 0x0);
CHECK_FIELD(_Rectangle<short>, top, 0x2);
CHECK_FIELD(_Rectangle<short>, right, 0x4);
CHECK_FIELD(_Rectangle<short>, bottom, 0x6);
CHECK_SIZE(_RectangleSHORT, 0x8);

#endif // PG_SHARED_COORD_H
