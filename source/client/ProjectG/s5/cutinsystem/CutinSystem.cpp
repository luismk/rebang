// Original: d:\build\custom\temp\code\client\projectg\s5\cutinsystem\cutinsystem.cpp
#include <windows.h>
#include <atlbase.h>	// stand-in for the precompiled header: _AtlWinModule initializer
#include "CutinSystem.h"

#include <math.h>
#include <new>
#include <string.h>

#include "ObjectFactory.h"
#include "ClientSetting.h"
#include "SharedDoc.h"
#include "../client/Wangreal/include/wmath.h"
#include "../client/Wangreal/include/woverlay.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "../client/Wangreal/include/wview.h"
#include "wtemplate.h"

// clientsetting.cpp (no header declares it yet)
int WisZero(const float &f, float eps);

namespace {

// cutinsystem.cpp:17
bool IsScalingCutInStyle(eCUTINANISTLYE style) {
	return style == 3 || style == 4;
}

// cutinsystem.cpp:20
bool IsAlphaCutInStyle(eCUTINANISTLYE style) {
	return style == 5 || style == 6;
}

// cutinsystem.cpp:24
bool IsRotateCutInStyle(eCUTINANISTLYE style) {
	return style == 7;
}

const float PI_F = 3.1415927f;

// cutinsystem.cpp:29 (four full turns; built from a const variable, so a
// dynamic initializer stores it: sub_009e1db0, docs/tough_problems/010)
float s_cutinRotateAngle = PI_F * 8.0f;

// Alpha range of the fade styles (read from memory, so constants with storage).
const float ALPHA_MAX = 255.0f;
const float ALPHA_MIN = 0.0f;

} // namespace

namespace S5 {

// cutinsystem.cpp:59 (run-time type and factory registration of CCutin)
const WRTTI CCutin::m_RTTI("CCutin", &IActor::m_RTTI);

IObject* CCutinMakeInstance(void) {
	return new CCutin;
}

struct __impCCutin {
	__impCCutin(void) {
		ObjectFactory().AddObjectFunctor(CCutinMakeInstance, "CCutin");
		ActorFactory().AddActorClass("CCutin");
	}
};
static __impCCutin s_impCCutin;

// cutinsystem.cpp:77
CCutin::CCutin(void)
	: m_1c(0)
	, m_d8(0)
	, m_dc(0)
	, m_e0(0)
	, m_e4(0)
	, m_alpha(255.0f)
	, m_angle(0)
	, m_scale(1.0f)
	, m_134(0.1f)
	, m_gameState(0)
	, m_condition(0)
	, m_218(0)
	, m_21c(false) {
	m_priority = 4;
	ClearAllData(true);
}

// cutinsystem.cpp:84
void CCutin::OnInit(void) {
}

// cutinsystem.cpp:93
void CCutin::OnDestroy(void) {
	if (g_resrcmng && m_d8) { g_resrcmng->Release(m_d8); m_d8 = 0; }
	if (g_resrcmng && m_dc) { g_resrcmng->Release(m_dc); m_dc = 0; }
	if (g_resrcmng && m_e0) { g_resrcmng->Release(m_e0); m_e0 = 0; }
	if (g_resrcmng && m_e4) { g_resrcmng->Release(m_e4); m_e4 = 0; }
}

// cutinsystem.cpp:199
void CCutin::OnDisplay(void) {
	if (m_1c > 2 && m_d8 != NULL) {
		if (m_1c >= 6) {
			DisplayOverlay(m_dc, m_inInfo.m_style, m_rc1);
			DisplayOverlay(m_e0, m_inInfo.m_style, m_rc2);
			DisplayOverlay(m_d8, m_inInfo.m_style, m_rc0);
			DisplayOverlay(m_e4, m_inInfo.m_style, m_rc3);
		} else {
			DisplayOverlay(m_dc, m_inInfo.m_style1, m_rc1);
			DisplayOverlay(m_e0, m_inInfo.m_style2, m_rc2);
			DisplayOverlay(m_d8, m_inInfo.m_style0, m_rc0);
			DisplayOverlay(m_e4, m_inInfo.m_style3, m_rc3);
		}
	}
}

// cutinsystem.cpp:299
bool CCutin::PrepareCutIn(void) {
	if (m_1c > 1) {
		if (m_d8 != NULL) {
			SetOverlayInfo(m_d8, m_inInfo.m_style0, m_rc0);
			SetOverlayInfo(m_dc, m_inInfo.m_style1, m_rc1);
			SetOverlayInfo(m_e0, m_inInfo.m_style2, m_rc2);
			SetOverlayInfo(m_e4, m_inInfo.m_style3, m_rc3);
			return true;
		}
	}
	return false;
}

// cutinsystem.cpp:324
bool CCutin::ProcessWorkIn(float elapsed) {
	bool done1 = ProcessOverlay(elapsed, m_dc, m_inInfo.m_style1, m_rc1);
	bool done2 = ProcessOverlay(elapsed, m_e0, m_inInfo.m_style2, m_rc2);
	bool done0 = ProcessOverlay(elapsed, m_d8, m_inInfo.m_style0, m_rc0);
	bool done3 = ProcessOverlay(elapsed, m_e4, m_inInfo.m_style3, m_rc3);

	if (done1 == true && done2 == true && done0 == true && done3 == true) {
		return true;
	}
	return false;
}

// cutinsystem.cpp:371
bool CCutin::ProcessWorkInFinish(float elapsed) {
	if (m_134 > 0.0f) { m_134 -= elapsed; if (m_134 < 0.0f) m_134 = 0.0f; }
	if (WAbsBySign(m_134) < WMATH_EPSILON) {
		m_134 = 0.1f;
		if (m_inInfo.m_style == 7) {
			m_angle = s_cutinRotateAngle;
		}
		return true;
	}
	return false;
}

// cutinsystem.cpp:386
bool CCutin::ProcessWorkOut(float elapsed) {
	bool done1 = ProcessOverlay_Out(elapsed, m_dc, m_inInfo.m_style1, m_rc1);
	bool done2 = ProcessOverlay_Out(elapsed, m_e0, m_inInfo.m_style2, m_rc2);
	bool done0 = ProcessOverlay_Out(elapsed, m_d8, m_inInfo.m_style0, m_rc0);
	bool done3 = ProcessOverlay_Out(elapsed, m_e4, m_inInfo.m_style3, m_rc3);

	if (done1 == true && done2 == true && done0 == true && done3 == true) {
		return true;
	}
	return false;
}

// cutinsystem.cpp:437
void CCutin::ClearAllData(bool bAll) {
	if (!bAll) {
		m_1c = 0;
		m_alpha = 255.0f;
		m_angle = 0;
		m_scale = 1.0f;
		m_21c = false;
		m_134 = 0.1f;
		m_218 = 0;
		m_condition = 0;
	}

	m_21c = false;
	m_inInfo.Reset();
	m_outInfo.Reset();
	m_1fc.clear();

	memset(&m_rc0, 0, sizeof(WRect));
	memset(&m_rc1, 0, sizeof(WRect));
	memset(&m_rc2, 0, sizeof(WRect));
	memset(&m_rc3, 0, sizeof(WRect));

	if (g_resrcmng && m_d8) { g_resrcmng->Release(m_d8); m_d8 = 0; }
	if (g_resrcmng && m_dc) { g_resrcmng->Release(m_dc); m_dc = 0; }
	if (g_resrcmng && m_e0) { g_resrcmng->Release(m_e0); m_e0 = 0; }
	if (g_resrcmng && m_e4) { g_resrcmng->Release(m_e4); m_e4 = 0; }
}

// cutinsystem.cpp:476
void CCutin::SetOverlayInfo(WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc) {
	if (pOverlay != NULL) {
		rc.w = (float)pOverlay->GetWidth();
		rc.h = (float)pOverlay->GetHeight();

		switch (m_inInfo.m_posType) {
		case 0:
		case 4:
			rc.x = 0;
			rc.y = 0;
			break;
		case 1:
			rc.x = (g_view->GetWidth() - rc.w) * 0.5f;
			rc.y = (g_view->GetHeight() - rc.h) * 0.5f;
			break;
		case 5:
			rc.x = g_view->GetWidth() - rc.w;
			rc.y = g_view->GetHeight() - rc.h;
			break;
		}

		ScreenRate(rc, pOverlay);

		switch (style) {
		case 2:
			rc.x = g_view->GetWidth();
			break;
		case 1:
			rc.x = -rc.w;
			break;
		case 3:
			m_scale = 1.0f;
			break;
		case 4:
			m_scale = 0;
			break;
		case 5:
			m_alpha = 0;
			break;
		case 6:
			m_alpha = 255.0f;
			break;
		}
	}
}

// cutinsystem.cpp:542
bool CCutin::ProcessOverlay(float elapsed, WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc) {
	float speed;

	if (pOverlay == NULL) {
		return true;
	}

	switch (style) {
	case 0:
		SAFE_SUBVALUE(m_134, 0.0f, elapsed);
		return WisZero(m_134, 1e-5f) ? true : false;
	case 1:
		if (rc.x < -100.0f) {
			speed = 3.0f;
		} else {
			speed = 0.08f;
		}
		if (SAFE_ADDVALUE(rc.x, 0.0f, speed * elapsed * rc.w) < 0.0f) {
			return false;
		}
		break;
	case 2:
		if (rc.x < 100.0f) {
			speed = 0.08f;
		} else {
			speed = 3.0f;
		}
		if (SAFE_SUBVALUE(rc.x, 0.0f, speed * elapsed * rc.w) > 0.0f) {
			return false;
		}
		break;
	case 3:
		if (SAFE_SUBVALUE(m_scale, 0.0f, elapsed) > 0.0f) {
			return false;
		}
		break;
	case 4:
		if (SAFE_ADDVALUE(m_scale, 1.0f, elapsed) < 1.0f) {
			return false;
		}
		break;
	case 7:
		if (SAFE_ADDVALUE(m_angle, s_cutinRotateAngle, s_cutinRotateAngle * elapsed) < s_cutinRotateAngle) {
			return false;
		}
		break;
	case 5:
		if (SAFE_ADDVALUE(m_alpha, ALPHA_MAX, elapsed * ALPHA_MAX) < ALPHA_MAX) {
			return false;
		}
		break;
	case 6:
		if (SAFE_SUBVALUE(m_alpha, ALPHA_MIN, elapsed * ALPHA_MAX) > 0.0f) {
			return false;
		}
		break;
	}

	return true;
}

// cutinsystem.cpp:695
bool CCutin::ProcessOverlay_Out(float elapsed, WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc) {
	float speed;

	if (pOverlay == NULL) {
		return true;
	}

	if (m_inInfo.m_style == 9 || m_inInfo.m_style == 8) {
		if (m_inInfo.m_style == 8) {
			if (style == 1) {
				style = (eCUTINANISTLYE)2;
			} else if (style == 2) {
				style = (eCUTINANISTLYE)1;
			}
		}
	}

	switch (style) {
	case 0:
		SAFE_SUBVALUE(m_134, 0.0f, elapsed);
		return WisZero(m_134, 1e-5f) ? true : false;
	case 1: {
		float limit = -rc.w;
		if (rc.x > limit * 0.9f) {
			speed = 0.08f;
		} else {
			speed = 3.0f;
		}
		if (SAFE_SUBVALUE(rc.x, limit, speed * rc.w * elapsed) > limit) {
			return false;
		}
		break;
	}
	case 2:
		if (SAFE_ADDVALUE(rc.x, g_view->GetWidth(), elapsed * rc.w) < g_view->GetWidth()) {
			return false;
		}
		break;
	case 3:
		if (SAFE_SUBVALUE(m_scale, 0.0f, elapsed) > 0.0f) {
			return false;
		}
		break;
	case 4:
		if (SAFE_ADDVALUE(m_scale, 1.0f, elapsed) < 1.0f) {
			return false;
		}
		break;
	case 7:
		if (SAFE_SUBVALUE(m_angle, 0.0f, s_cutinRotateAngle * elapsed) > 0.0f) {
			return false;
		}
		break;
	case 5:
		if (SAFE_ADDVALUE(m_alpha, ALPHA_MAX, elapsed * ALPHA_MAX) < ALPHA_MAX) {
			return false;
		}
		break;
	case 6:
		if (SAFE_SUBVALUE(m_alpha, ALPHA_MIN, elapsed * ALPHA_MAX) > 0.0f) {
			return false;
		}
		break;
	}

	return true;
}

// cutinsystem.cpp:841
void CCutin::DisplayOverlay(WOverlay *pOverlay, eCUTINANISTLYE style, WRect &rc) {
	float alpha = 255.0f;
	float angle = 0.0f;
	WRect rect = rc;

	if (pOverlay == NULL) {
		return;
	}

	if (IsAlphaCutInStyle(style)) {
		alpha = m_alpha;
	}

	if (IsRotateCutInStyle(style)) {
		angle = m_angle;
	} else if (IsScalingCutInStyle(style)) {
		RectScaling(rect, m_scale);
	}

	pOverlay->Render(g_view, WRect(0.0f, 0.0f, 1.0f, 1.0f), rect, 0, ((unsigned char)alpha << 24) | 0x00ffffff, angle, 0);
}

// cutinsystem.cpp:868
void CCutin::OutDraw_Setting(void) {
	switch (m_inInfo.m_style) {
	case 3:
		m_scale = 1.0f;
		break;
	case 4:
		m_scale = 0.0f;
		break;
	case 5:
		m_alpha = 0.0f;
		break;
	case 6:
		m_alpha = 255.0f;
		break;
	}
}

// cutinsystem.cpp:892
bool CCutin::CheckGameState(void) {
	if (IsGameState(4)) {
		if (WSingleton<CSharedDoc>::Instance()->m_gameMode == 0) {
			if (m_21c) {
				if (ISCondition(7)) {
					if (IsGameState(8) && IsGameState(0x10)) {
						return true;
					}
				}
			} else {
				if (ISCondition(8)) {
					return IsGameState(0x20);
				} else {
					if (IsGameState(8) && IsGameState(0x10)) {
						return true;
					}
				}
			}
		} else {
			if (m_21c) {
				if (ISCondition(7)) {
					return IsGameState(0x10);
				}
			} else {
				if (ISCondition(8)) {
					return IsGameState(0x20);
				} else {
					return IsGameState(0x10);
				}
			}
		}
	}
	return false;
}

// cutinsystem.cpp:979
bool CCutin::IsGameState(unsigned long state) {
	if (m_gameState & state)
		return true;

	return false;
}

// cutinsystem.cpp:987
bool CCutin::ISCondition(unsigned long condition) {
	if (m_condition & condition)
		return true;

	return false;
}

// cutinsystem.cpp:995
void CCutin::RectScaling(WRect &rc, float scale) {
	float cx = rc.x + rc.w * 0.5f;
	float cy = rc.y + rc.h * 0.5f;

	rc.w = scale * rc.w;
	rc.h = scale * rc.h;
	rc.x = cx - rc.w * 0.5f;
	rc.y = cy - rc.h * 0.5f;
}

// cutinsystem.cpp:1008
void CCutin::ScreenRate(WRect &rc, WOverlay *pOverlay) {
	if (pOverlay == NULL) {
		return;
	}

	float cx = rc.x + rc.w * 0.5f;
	float cy = rc.y + rc.h * 0.5f;
	float rateX = g_view->GetWidth() * (1.0f / 1024.0f);
	float rateY = g_view->GetHeight() * (1.0f / 768.0f);

	if (WSingleton<COption>::Instance()->vIsWideMode() && pOverlay == m_d8) {
		rateX = rateY;
	}

	rc.w = rateX * rc.w;
	rc.h = rateY * rc.h;
	rc.x = cx - rc.w * 0.5f;
	rc.y = cy - rc.h * 0.5f;

	if (pOverlay->GetWidth() > pOverlay->GetHeight()) {
		rc.x = 0.0f;
	} else {
		rc.y = 0.0f;
	}
}

} // namespace S5
