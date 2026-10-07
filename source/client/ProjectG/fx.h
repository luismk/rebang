#pragma once

#include "scenemanager.h"

#include "fxbox.h"

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

// TODO: incomplete
class CPvsFxBox : public CFxBox
{
public:
	virtual void ProcessAttacher(float elapsed);

protected:
	virtual bool CheckGroundCollision(const WVector& from, const WVector& to,
		float& height, float radius);
	virtual void OnSequenceCreated(CFxSequence* sequence);
	virtual void OnSequenceDeleted(CFxSequence* sequence);
	virtual void OnSprayCreated(CFxSpray* spray);
	virtual void OnSprayDeleted(CFxSpray* spray);

	std::map<CFxSequence*, Attacher> m_seqAttacher;
	std::map<CFxSpray*, Attacher> m_sprayAttacher;
};

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
	void UpdateExtWind(WVector wind);
	bool FindSequence(CFxSequence* sequence);
	bool GetOutput(WVector& out, unsigned char& value);
	void SetPvsFxBoxGroundHeight(float maxHeight, float minHeight)
	{
		m_pvsFxBox.SetGroundHeightArea(maxHeight, minHeight);
	}

protected:
	CPvsFxBox m_pvsFxBox;
	std::map<int, void*> m_control;
	bool m_bActive;
};
