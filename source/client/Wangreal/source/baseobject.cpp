// Original: d:\build\custom\temp\code\client\wangreal\source\baseobject.cpp
#include "../include/baseobject.h"

int BaseObject::ms_nextId = 0;
int BaseObject::ms_count = 0;
BaseObject::ConstructionCallback BaseObject::ms_fnConstruct = 0;
BaseObject::DestructionCallback BaseObject::ms_fnDestruct = 0;

// baseobject.cpp:14
void BaseObject::SetConstructionCallback(ConstructionCallback fn) {
	ms_fnConstruct = fn;
}

// baseobject.cpp:20
void BaseObject::SetDestructionCallback(DestructionCallback fn) {
	ms_fnDestruct = fn;
}

// baseobject.cpp:26
BaseObject::BaseObject(void) {
	m_ref = 0;
	m_id = ms_nextId++;
	ms_count++;

	if (ms_fnConstruct) {
		ms_fnConstruct(this);
	}

	m_leakHint = "";
}

// baseobject.cpp:38
BaseObject::~BaseObject(void) {
	ms_count--;

	if (ms_fnDestruct) {
		ms_fnDestruct(this);
	}
}

// baseobject.cpp:47
const char* BaseObject::GetLeakHint(void) {
	return m_leakHint.c_str();
}

// baseobject.cpp:52
void BaseObject::SetLeakHint(const char* hint) {
	m_leakHint = hint;
}
