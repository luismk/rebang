#pragma once

#include "scenemanager.h"

#include "fxsequence.h"

struct Attacher
{
	float x;
	float y;
	WMatrix* mat[4];
	WVector* pos[4];

	Attacher();
	bool Get(WVector& out);
};

struct defines_t;

class CFx : public CRenderFuncPtr, public WSingleton<CFx>
{
public:
	CFx();
	virtual ~CFx();

	virtual void Process(float elapsed);
	virtual void Display();

	CFxSequence* OpenSequence(const char* filename, bool bDelete);
	void CloseSequence(CFxSequence* sequence, bool bDeleteChild);
	CFxSpray* OpenSpray(const char* filename);
	void CloseSpray(CFxSpray* spray);
	void* Attach(const Attacher& attacher, const char* filename, bool bMark,
		defines_t* defines);
	void SetActive(bool bActive);
	void ClearAll(bool bMarkedOnly);
	bool GetOutput(WVector& out, unsigned char& value);

protected:
	unsigned char m_pvsFxBox[0x364]; // TODO: CPvsFxBox
	std::map<int, void*> m_control;
	bool m_bActive;
};
