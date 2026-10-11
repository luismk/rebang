// Original: d:\build\custom\temp\code\client\projectg\capturedbg.h
#ifndef PG_CLIENT_PROJECTG_CAPTUREDBG_H
#define PG_CLIENT_PROJECTG_CAPTUREDBG_H

#include "..\..\defines.h"
#include "commonutil.h"

// Keeps a captured copy of the screen as the background while a modal
// screen is up (the video device renders it instead of the scene).
class CCapturedBg : public CStackableSingleton<CCapturedBg> {
public:
	CCapturedBg();
	// capturedbg.h:18
	virtual ~CCapturedBg() {}

	void Render();
	void Capture();
	void Reset();
	bool IsCaptured() { return m_CaptureMode; }

protected:
	void SetCapturedBgMode(bool on);

	bool m_CaptureMode;     // 0x28

	LAYOUT_CHECK_FRIEND;
};
CHECK_SIZE(CCapturedBg, 0x2c);
BEGIN_LAYOUT_CHECK(CCapturedBg)
	CHECK_FIELD(CCapturedBg, m_CaptureMode, 0x28);
END_LAYOUT_CHECK

#endif
