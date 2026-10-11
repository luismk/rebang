// Original: d:\build\custom\temp\code\client\projectg\s5\cutinsystem\cutinsystem.h
#ifndef PG_CLIENT_PROJECTG_S5_CUTINSYSTEM_CUTINSYSTEM_H
#define PG_CLIENT_PROJECTG_S5_CUTINSYSTEM_CUTINSYSTEM_H

#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "RTTI.h"
#include "tweaker.h"
#include "../Wangreal/include/wtypes.h"
#include "../shared/globalgamedefine.h"

class IObject;
class WOverlay;

namespace S5 {

IObject* CCutinMakeInstance(void);

// The cut-in actor: plays the overlay animations shown over the game screen
// (IActor at 0x0, tweaker_cmd_target at 0x18, 0x220 bytes, from the
// constructor and CCutinMakeInstance).
class CCutin : public IActor, public tweaker_cmd_target {
	LAYOUT_CHECK_FRIEND;
	friend IObject* CCutinMakeInstance(void);
public:
	static const WRTTI m_RTTI;

	// cutinsystem.h:31
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }
	// cutinsystem.h:33
	virtual ~CCutin(void) {}

	virtual void OnInit(void);
	virtual void OnProcess(float elapsed);
	virtual void OnDisplay(void);
	virtual void OnDestroy(void);
	virtual void HandleMsg(const MsgObject &msg);

protected:
	CCutin(void);

	// cutinsystem.h:60
	void SetGameState(unsigned long state) { m_gameState |= state; }
	// cutinsystem.h:66
	void SetCondition(unsigned long condition) { m_condition |= condition; }
	// cutinsystem.h:67
	int GetCondition(void) { return m_condition; }

	void OutDraw_Setting(void);
	bool IsGameState(unsigned long state);
	bool ISCondition(unsigned long condition);
	void RectScaling(WRect &rc, float scale);
	bool ProcessWorkInFinish(float elapsed);
	void ClearAllData(bool bAll);
	bool ProcessOverlay(float elapsed, WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc);
	bool ProcessOverlay_Out(float elapsed, WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc);
	void DisplayOverlay(WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc);
	void ScreenRate(WRect &rc, WOverlay *pOverlay);
	bool ProcessWorkIn(float elapsed);
	bool ProcessWorkOut(float elapsed);
	void SetOverlayInfo(WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc);
	bool CheckGameState(void);
	std::string TwAnimateTest(const tweaker_param &param);
	bool PrepareCutIn(void);

	int m_1c;                           // 0x1c, cleared by the constructor
	CUTIN_ANI_STYPEINFO m_inInfo;       // 0x20
	WOverlay* m_d8;                     // 0xd8, overlay 0 (rect m_rc0)
	WOverlay* m_dc;                     // 0xdc, overlay 1 (rect m_rc1)
	WOverlay* m_e0;                     // 0xe0, overlay 2 (rect m_rc2)
	WOverlay* m_e4;                     // 0xe4, overlay 3 (rect m_rc3)
	float m_alpha;                      // 0xe8, 0..255
	float m_angle;                      // 0xec, rotation of the rotate style
	float m_scale;                      // 0xf0
	WRect m_rc0;                        // 0xf4
	WRect m_rc1;                        // 0x104
	WRect m_rc2;                        // 0x114
	WRect m_rc3;                        // 0x124
	float m_134;                        // 0x134, 0.1 at construction
	unsigned long m_gameState;          // 0x138, bit set (SetGameState)
	UNKNOWN(0x13c, 0x140);
	unsigned long m_condition;          // 0x140, bit set (SetCondition)
	CUTIN_ANI_STYPEINFO m_outInfo;      // 0x144
	std::string m_1fc;                  // 0x1fc
	int m_218;                          // 0x218
	bool m_21c;                         // 0x21c
};

CHECK_SIZE(CCutin, 0x220);
BEGIN_LAYOUT_CHECK(CCutin)
	CHECK_FIELD(CCutin, m_1c, 0x1c);
	CHECK_FIELD(CCutin, m_inInfo, 0x20);
	CHECK_FIELD(CCutin, m_d8, 0xd8);
	CHECK_FIELD(CCutin, m_e4, 0xe4);
	CHECK_FIELD(CCutin, m_alpha, 0xe8);
	CHECK_FIELD(CCutin, m_angle, 0xec);
	CHECK_FIELD(CCutin, m_scale, 0xf0);
	CHECK_FIELD(CCutin, m_rc0, 0xf4);
	CHECK_FIELD(CCutin, m_rc3, 0x124);
	CHECK_FIELD(CCutin, m_134, 0x134);
	CHECK_FIELD(CCutin, m_gameState, 0x138);
	CHECK_FIELD(CCutin, m_condition, 0x140);
	CHECK_FIELD(CCutin, m_outInfo, 0x144);
	CHECK_FIELD(CCutin, m_1fc, 0x1fc);
	CHECK_FIELD(CCutin, m_218, 0x218);
	CHECK_FIELD(CCutin, m_21c, 0x21c);
END_LAYOUT_CHECK

} // namespace S5

#endif // PG_CLIENT_PROJECTG_S5_CUTINSYSTEM_CUTINSYSTEM_H
