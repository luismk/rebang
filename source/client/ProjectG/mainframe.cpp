// Original: d:\build\custom\temp\code\client\projectg\mainframe.cpp
#include "MainFrame.h"
#include "wmath.h"

void __cdecl InitMath(void);
void __cdecl UninitMath(void);
int _GGC_CheckGameMon(void);

extern WMatrix g_camera;
extern bool g_bQuit;

// Time of the last GameGuard check (ms).
static float data_b7bb10;

// mainframe.cpp:17
CMainFrame::CMainFrame(void)
	: m_state(0), m_time(0), m_frameTime(0) {
	m_gameSpeed = 1.0f;
	m_speedScale = 1.0f;

	m_fps = 30.0f; m_frameSkip = 1; m_frameCount = m_lateFrameCount = 0;

	InitMath();

	g_camera.Reset();
}

// mainframe.cpp:33
CMainFrame::~CMainFrame(void) {
	UninitMath();
}

// mainframe.cpp:38
void CMainFrame::ResetTimer(void) {
	m_startTime = GetSystemTime();
	m_time = 0;

	m_frameTime = 0;
	m_frameCount = 0;
	m_lateFrameCount = 0;
}

// mainframe.cpp:50
void CMainFrame::FixFrameRate(bool adjust) {
	if (adjust) {
		int percent = m_lateFrameCount * 100 / m_frameCount;

		if (GetFPS() >= 15.0f && 100 - percent >= 55) {
			m_frameSkip++;
		}

		if (m_frameSkip - 1 >= 1 && percent == 100 && m_frameTime >= m_frameCount / m_fps * 0.5f) {
			m_frameSkip -= (m_frameTime >= m_frameCount / m_fps * 0.75f && m_frameSkip - 2 >= 1) ? 2 : 1;
		}
	}

	m_frameTime = 0;
	m_frameCount = 0;
	m_lateFrameCount = 0;
}

// mainframe.cpp:80
int CMainFrame::Main(int flags) {
	float now = (float)GetTime();

	if (!(flags & 4)) {
		if (m_time + 1500.0f < now) {
			m_time = now - 100.0f;
		}
	}

	while (now > m_time) {
		float step = now - m_time;

		if (step > 100.0f) {
			step = 100.0f;
		}

		m_time += step;

		Update(!(flags & 0x10) ? (int)m_time : 0);

		Process(step * 0.001f);
	}

	if (m_state == 0 && !(flags & 1)) {
		Draw();
		Paint();
	}

	if (now - data_b7bb10 > 10000.0f) {
		g_bQuit = _GGC_CheckGameMon() == 0;
		data_b7bb10 = now;
	}

	return m_state;
}

// mainframe.cpp:249
void CMainFrame::SetFPS(float fps) {
	m_fps = fps;

	m_frameSkip = 1; m_frameCount = m_lateFrameCount = 0;
}

// mainframe.cpp:256
void CMainFrame::Quit(void) {
	m_state = 1;
}

// mainframe.cpp:261
void CMainFrame::Restart(void) {
	m_state = 2;
}
