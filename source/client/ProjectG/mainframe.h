// Original: d:\build\custom\temp\code\client\projectg\mainframe.h
#ifndef PG_CLIENT_PROJECTG_MAINFRAME_H
#define PG_CLIENT_PROJECTG_MAINFRAME_H

#include "..\..\defines.h"

// Fixed-step main loop of the client: Main() advances the simulation in
// steps of at most 100 ms, then draws one frame.
class CMainFrame {
	LAYOUT_CHECK_FRIEND;
public:
	CMainFrame(void); // mainframe.cpp:17
	virtual ~CMainFrame(void); // mainframe.cpp:33

	int Main(int flags); // mainframe.cpp:80
	void SetFPS(float fps); // mainframe.cpp:249

	// mainframe.h:37
	float GetFPS(void) const { return m_fps / m_frameSkip; }

	// mainframe.h:39
	void SetGameSpeed(float speed) { m_gameSpeed = speed; }
	// mainframe.h:40
	float GetGameSpeed(void) const { return m_gameSpeed; }

protected:
	virtual void Process(float seconds) = 0;   // vtable +0x04
	virtual void Draw(void) = 0;               // vtable +0x08
	// mainframe.h:45
	virtual void Paint(void) {}                // vtable +0x0C
	virtual unsigned long GetSystemTime(void) const = 0; // vtable +0x10
	// mainframe.h:48
	virtual void Update(int time) {}           // vtable +0x14

	void ResetTimer(void); // mainframe.cpp:38
	void Quit(void); // mainframe.cpp:256
	void Restart(void); // mainframe.cpp:261

private:
	void FixFrameRate(bool adjust); // mainframe.cpp:50

	// mainframe.h:70
	unsigned long GetTime(void) const { return GetSystemTime() - m_startTime; }

protected:
	int m_state;               // 0x04 (0 running, 1 quit, 2 restart)
	float m_fps;               // 0x08
	float m_gameSpeed;         // 0x0C
	float m_speedScale;        // 0x10
	float m_time;              // 0x14 simulated time, ms
	unsigned long m_startTime; // 0x18
	int m_frameCount;          // 0x1C
	int m_lateFrameCount;      // 0x20
	int m_frameSkip;           // 0x24
	float m_frameTime;         // 0x28
};

CHECK_SIZE(CMainFrame, 0x2C);
BEGIN_LAYOUT_CHECK(CMainFrame)
	CHECK_FIELD(CMainFrame, m_state, 0x04);
	CHECK_FIELD(CMainFrame, m_fps, 0x08);
	CHECK_FIELD(CMainFrame, m_gameSpeed, 0x0C);
	CHECK_FIELD(CMainFrame, m_speedScale, 0x10);
	CHECK_FIELD(CMainFrame, m_time, 0x14);
	CHECK_FIELD(CMainFrame, m_startTime, 0x18);
	CHECK_FIELD(CMainFrame, m_frameCount, 0x1C);
	CHECK_FIELD(CMainFrame, m_lateFrameCount, 0x20);
	CHECK_FIELD(CMainFrame, m_frameSkip, 0x24);
	CHECK_FIELD(CMainFrame, m_frameTime, 0x28);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MAINFRAME_H
