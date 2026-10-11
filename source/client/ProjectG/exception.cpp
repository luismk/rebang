// Original: d:\build\custom\temp\code\client\projectg\exception.cpp
#include <windows.h>

#include "Exception.h"

// exception.cpp:18
void WException::Handle(void) {
	FatalAppExitA(0, m_message.c_str());
}
