// Original: d:\build\custom\temp\code\client\projectg\capturedbg.cpp
#include "CapturedBg.h"
#include "ProjectG.h"
#include "wresrcmng.h"
#include "wvideo.h"

// capturedbg.cpp:27
CCapturedBg::CCapturedBg() : m_CaptureMode(false) {
}

// capturedbg.cpp:34
void CCapturedBg::Render() {
}

// capturedbg.cpp:48
void CCapturedBg::Reset() {
	if (m_CaptureMode) {
		SetCapturedBgMode(false);
	}
}

// capturedbg.cpp:54
void CCapturedBg::Capture() {
	SetCapturedBgMode(true);
}

// capturedbg.cpp:59
void CCapturedBg::SetCapturedBgMode(bool on) {
	if (on == true) {
		if (m_CaptureMode) {
			return;
		}

		if (g_resrcmng->VideoReference()->Command(W_VDEV_CAPTURED_BG, 1, 0) == 1) {
			m_CaptureMode = true;
		}

		WSingleton<CProjectG>::Instance()->m_mainFlags |= 2;
		WSingleton<CProjectG>::Instance()->SetFPS(g_resrcmng->VideoReference()->GetMonitorSupportFPS());
	} else {
		g_resrcmng->VideoReference()->Command(W_VDEV_CAPTURED_BG, 0, 0);

		m_CaptureMode = false;

		WSingleton<CProjectG>::Instance()->m_mainFlags &= ~2;
	}
}
