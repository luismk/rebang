// Original: d:\build\custom\temp\code\client\projectg\beach.h
#ifndef PG_CLIENT_PROJECTG_BEACH_H
#define PG_CLIENT_PROJECTG_BEACH_H

#include "..\..\defines.h"
#include "wavelet.h"

// A wavelet breaking on a beach (beach.cpp); CSceneManager keeps them in
// m_beachList.
class CBeach : public CWavelet {
public:
	// beach.h:9
	CBeach(void) {}
	// beach.h:10
	virtual ~CBeach(void) {}
	virtual void Process(float elapsed);
	virtual void Render(void);
	virtual void ReadAttribute(const char* attribute);
};

#endif // PG_CLIENT_PROJECTG_BEACH_H
