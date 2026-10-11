// Original: d:\build\custom\temp\code\client\projectg\exception.h
#ifndef PG_CLIENT_PROJECTG_EXCEPTION_H
#define PG_CLIENT_PROJECTG_EXCEPTION_H

#include <string>

#include "..\..\defines.h"

// An exception carrying a message (size 0x8 from the throw sites in
// socket.cpp). It keeps a reference to the caller's string, not a copy.
class WException {
public:
	WException(const std::string& message) : m_message(message) {
	}
	virtual ~WException(void) {
	}
	virtual void Handle(void);

protected:
	const std::string& m_message;   // 0x4 (a reference: no CHECK_FIELD)
};

CHECK_SIZE(WException, 0x8);

// A system (Winsock) failure, thrown by WSocket.
class WSysException : public WException {
public:
	WSysException(const std::string& message) : WException(message) {
	}
	virtual ~WSysException(void) {
	}
};

CHECK_SIZE(WSysException, 0x8);

// An application failure (vtable ??_7WAppException@@6B@), thrown by
// WClientSocket on a buffer overflow.
class WAppException : public WException {
public:
	WAppException(const std::string& message) : WException(message) {
	}
	virtual ~WAppException(void) {
	}
};

CHECK_SIZE(WAppException, 0x8);

#endif // PG_CLIENT_PROJECTG_EXCEPTION_H
