// Original: d:\build\custom\temp\code\client\projectg\fx.h
#ifndef PG_CLIENT_PROJECTG_FX_H
#define PG_CLIENT_PROJECTG_FX_H

#include <map>

#include "FxBox.h"
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "scenemanager.h"
#include "singleton.h"

class CFxSequence;
class CFxSpray;
class CFxKeeper;
class CPvsFxBox;
struct defines_t;
struct sFxKeeper;
class CGolfBall;
class WVector;
class WMatrix;

// Where an effect is attached: up to four offsets, each optionally carried
// by a matrix; Get averages the attached points.
struct Attacher {
	Attacher(void);
	bool Get(WVector& pos);

	float m_fLimitA;                           // 0x00, FLT_MAX by default
	float m_fLimitB;                           // 0x04, FLT_MAX by default
	const WMatrix* m_pMatrix[4];               // 0x08
	const WVector* m_pOffset[4];               // 0x18
};
CHECK_SIZE(Attacher, 0x28);
CHECK_FIELD(Attacher, m_pMatrix, 0x8);
CHECK_FIELD(Attacher, m_pOffset, 0x18);

// CFx's effect box: keeps where each sequence and spray it opened is
// attached. Its constructor and destructor are implicit (emitted in
// ground_info).
class CPvsFxBox : public CFxBox {
	LAYOUT_CHECK_FRIEND;
public:
	virtual void ProcessAttacher(float fDelta);

protected:
	virtual bool CheckGroundCollision(const WVector& from, const WVector& to, float& height, float radius);
	virtual void OnSequenceCreated(CFxSequence* pSequence);
	virtual void OnSequenceDeleted(CFxSequence* pSequence);
	virtual void OnSprayCreated(CFxSpray* pSpray);
	virtual void OnSprayDeleted(CFxSpray* pSpray);
	void UpdateLightSrc(const LightSet& light);

	std::map<CFxSequence*, Attacher> m_sequenceAttachers;     // 0x34C
	std::map<CFxSpray*, Attacher> m_sprayAttachers;           // 0x358
};
CHECK_SIZE(CPvsFxBox, 0x364);
BEGIN_LAYOUT_CHECK(CPvsFxBox)
	CHECK_FIELD(CPvsFxBox, m_sequenceAttachers, 0x34C);
	CHECK_FIELD(CPvsFxBox, m_sprayAttachers, 0x358);
END_LAYOUT_CHECK

// Base of the effects CFx runs (kept in CFx's map by m_serial). The default
// constructor is implicit (no line info).
class CFxControl {
public:
	// fx.h:20
	virtual ~CFxControl(void) {}
	virtual bool Process(float fDelta) = 0;   // vtable slot 1 (_purecall)

	static unsigned long ms_serial;

	unsigned long m_serial;                    // 0x04, CFx's map key
};
CHECK_SIZE(CFxControl, 0x8);
CHECK_FIELD(CFxControl, m_serial, 0x4);

class CFxRunDust : public CFxControl {
public:
	CFxRunDust(CPvsFxBox& box, const WVector& pos, const WVector& dir);
	virtual ~CFxRunDust(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	const WVector& m_pos;                      // 0x0C, MakeRunDust's first vector
	const WVector& m_dir;                      // 0x10, MakeRunDust's second vector
	CFxSequence* m_pGroundSequence;            // 0x14, GroundRun.seq
	CFxSequence* m_pWaterSequence;             // 0x18, WaterRun.seq
	UNKNOWN(0x1C, 0x20);
};
CHECK_SIZE(CFxRunDust, 0x20);
CHECK_FIELD(CFxRunDust, m_pGroundSequence, 0x14);
CHECK_FIELD(CFxRunDust, m_pWaterSequence, 0x18);

class CFxCobra : public CFxControl {
public:
	CFxCobra(CPvsFxBox& box, const CGolfBall& ball);
	virtual ~CFxCobra(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	const CGolfBall& m_ball;                   // 0x0C
	CFxSequence* m_pSequences[4];              // 0x10, Cobra_BG, Cobra_BW, Cobra_BR, Cobra_C
	bool m_bFinished;                          // 0x20, false at construction
	PADDING(0x21, 0x24);
};
CHECK_SIZE(CFxCobra, 0x24);
CHECK_FIELD(CFxCobra, m_pSequences, 0x10);
CHECK_FIELD(CFxCobra, m_bFinished, 0x20);

class CFxTomahawk : public CFxControl {
public:
	CFxTomahawk(CPvsFxBox& box, const CGolfBall& ball, bool bVariant);
	virtual ~CFxTomahawk(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	const CGolfBall& m_ball;                   // 0x0C
	CFxSequence* m_pSequence;                  // 0x10, closed by the destructor
	bool m_bVariant;                           // 0x14, constructor's bool, tested by Process
	PADDING(0x15, 0x18);
	int m_state;                               // 0x18, Process's stage (0, 2, 3)
};
CHECK_SIZE(CFxTomahawk, 0x1C);
CHECK_FIELD(CFxTomahawk, m_pSequence, 0x10);
CHECK_FIELD(CFxTomahawk, m_bVariant, 0x14);
CHECK_FIELD(CFxTomahawk, m_state, 0x18);

class CFxSpike : public CFxControl {
public:
	CFxSpike(CPvsFxBox& box, const CGolfBall& ball, bool bVariant);
	virtual ~CFxSpike(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	const CGolfBall& m_ball;                   // 0x0C
	CFxSequence* m_pSequences[2];              // 0x10, Spike_C, Spike_ball
	bool m_bVariant;                           // 0x18, constructor's bool
	PADDING(0x19, 0x1C);
	int m_state;                               // 0x1C, 0 at construction
};
CHECK_SIZE(CFxSpike, 0x20);
CHECK_FIELD(CFxSpike, m_pSequences, 0x10);
CHECK_FIELD(CFxSpike, m_bVariant, 0x18);
CHECK_FIELD(CFxSpike, m_state, 0x1C);

class CFxPowerSpin : public CFxControl {
public:
	CFxPowerSpin(CPvsFxBox& box, CGolfBall& ball);
	virtual ~CFxPowerSpin(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	CGolfBall& m_ball;                         // 0x0C
	CFxSequence* m_pSequence;                  // 0x10, checked with CFxBox::FindSequence by Process
	bool m_bEnd;                               // 0x14
	PADDING(0x15, 0x18);
};
CHECK_SIZE(CFxPowerSpin, 0x18);
CHECK_FIELD(CFxPowerSpin, m_pSequence, 0x10);
CHECK_FIELD(CFxPowerSpin, m_bEnd, 0x14);

class CFxShotImpact : public CFxControl {
public:
	CFxShotImpact(CPvsFxBox& box, CGolfBall& ball);
	virtual ~CFxShotImpact(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	CGolfBall& m_ball;                         // 0x0C
	CFxSequence* m_pSequence;                  // 0x10, checked with CFxBox::FindSequence by Process
	bool m_bEnd;                               // 0x14
	PADDING(0x15, 0x18);
};
CHECK_SIZE(CFxShotImpact, 0x18);
CHECK_FIELD(CFxShotImpact, m_pSequence, 0x10);
CHECK_FIELD(CFxShotImpact, m_bEnd, 0x14);

class CFxClubImpact : public CFxControl {
public:
	CFxClubImpact(CPvsFxBox& box, CGolfBall& ball, unsigned long clubId);
	virtual ~CFxClubImpact(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	CGolfBall& m_ball;                         // 0x0C
};
CHECK_SIZE(CFxClubImpact, 0x10);

class CFxPowerShot2 : public CFxControl {
public:
	CFxPowerShot2(CPvsFxBox& box, CGolfBall& ball);
	virtual ~CFxPowerShot2(void);
	virtual bool Process(float fDelta);

	bool m_bDone;                              // 0x08
	CPvsFxBox& m_box;                          // 0x0C
	CGolfBall& m_ball;                         // 0x10
};
CHECK_SIZE(CFxPowerShot2, 0x14);
CHECK_FIELD(CFxPowerShot2, m_bDone, 0x8);

class CFxBoosterWarp : public CFxControl {
public:
	CFxBoosterWarp(CPvsFxBox& box, CGolfBall& ball, unsigned char type);
	virtual ~CFxBoosterWarp(void);
	virtual bool Process(float fDelta);

	CPvsFxBox& m_box;                          // 0x08
	CGolfBall& m_ball;                         // 0x0C
	CFxSequence* m_pSequences[2];              // 0x10, closed by the destructor
	bool m_bLoaded;                            // 0x18, both sequences loaded and added
	PADDING(0x19, 0x1C);
	float m_elapsed;                           // 0x1C, seconds since the harp warp started
	unsigned char m_harpMode;                  // 0x20, 1 for the type-6 (magic harp) warp: BGM muted, timed
	PADDING(0x21, 0x24);
};
CHECK_SIZE(CFxBoosterWarp, 0x24);
CHECK_FIELD(CFxBoosterWarp, m_pSequences, 0x10);
CHECK_FIELD(CFxBoosterWarp, m_bLoaded, 0x18);
CHECK_FIELD(CFxBoosterWarp, m_elapsed, 0x1C);
CHECK_FIELD(CFxBoosterWarp, m_harpMode, 0x20);

// The effect manager singleton. Only the members this project's code uses so
// far are declared.
class CFx : public CRenderFuncPtr, public WSingleton<CFx> {
public:
	CFx(void);
	virtual ~CFx(void);
	virtual void Display(void);
	virtual void Process(float fDelta);

	void* Attach(const Attacher& attacher, const char* name, bool bOriginal, defines_t* pDefines);
	void CloseSequence(CFxSequence* pSequence, bool bImmediate);
	CFxSpray* OpenSpray(const char* name);
	void CloseSpray(CFxSpray* pSpray);
	void AddFxKeeper(sFxKeeper& keeper);
	void EraseFxKeeper(CFxKeeper* pKeeper);
	void ClearFxKeeper(void);
	void SetActive(bool bActive);
	CFxSequence* OpenSequence(const char* name, bool bOriginal);
	void ClearAll(bool bKeepActive);
	CFxControl* FindControl(int serial);
	bool RemoveRunDust(int serial);
	int MakeRunDust(const WVector& pos, const WVector& dir);
	int MakeCobra(const CGolfBall& ball);
	int MakeTomahawk(const CGolfBall& ball, bool bVariant);
	int MakePowerSpin(CGolfBall& ball);
	int MakeShotImpact(CGolfBall& ball);
	int MakeClubImpact(CGolfBall& ball, unsigned long clubId);
	int MakeSpike(const CGolfBall& ball, bool bVariant);
	int MakePowerShot2(CGolfBall& ball);
	int MakeBoosterWarp(CGolfBall& ball, unsigned char type);
	CFxSpray* GetSpray(int index);
	bool FindSequence(CFxSequence* pSequence);
	bool FindSpray(CFxSpray* pSpray);
	bool GetOutput(WVector& pos, unsigned char& alpha);
	void UpdateExtWind(WVector wind);

protected:
	void ClearControl(void);
	int AddFxControl(CFxControl* pControl);

public:

	CPvsFxBox m_fxBox;                         // 0x2C
	std::map<int, CFxControl*> m_controls;     // 0x390, by CFxControl::m_serial
	bool m_bActive;                            // 0x39C
};
CHECK_SIZE(CFx, 0x3A0);
CHECK_FIELD(CFx, m_fxBox, 0x2C);
CHECK_FIELD(CFx, m_controls, 0x390);
CHECK_FIELD(CFx, m_bActive, 0x39C);

#endif // PG_CLIENT_PROJECTG_FX_H
