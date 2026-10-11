// Original: d:\build\custom\temp\code\client\projectg\projectg.h
#ifndef PG_CLIENT_PROJECTG_PROJECTG_H
#define PG_CLIENT_PROJECTG_PROJECTG_H

#include <deque>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "MainFrame.h"
#include "wangrealapp.h"
#include "singleton.h"
#include "wlist.h"
#include "frcmdtarget.h"
#include "Packet.h"
#include "wavi.h"
#include "wmath.h"
#include "cCRC32.h"

class FrForm;
class WInputDev;
class WDeviceManager;
class WDevice;
class WProcManager;
class CDxDiagInfo;

extern unsigned long g_CurrentTime;

// The game application. Bases as the constructor builds them: CMainFrame at
// 0x00, CWangrealApplication at 0x2C, WSingleton<CProjectG> at 0x30 (the
// instance is registered through it), FrCmdTarget at 0x58. Only the members
// the decompiled code uses are declared; the layout is only partly
// transcribed (size 0x2A0, the constructor's allocation).
class CProjectG : public CMainFrame, public CWangrealApplication, public WSingleton<CProjectG>, public FrCmdTarget {
	LAYOUT_CHECK_FRIEND;
public:
	// windows.h types spelled out: some includers do not include it.
	CProjectG(struct HWND__* hWnd);
	virtual ~CProjectG(void);

	virtual long __stdcall WinProc(struct HWND__* hWnd, unsigned int message, unsigned int wParam, long lParam);
	virtual bool Ready(void);

	void SetWindowed(bool windowed);
	void SetMoveLoginServer(const char* id, const char* password);
	bool HidePrivacy(void);
	bool HideGUI(void);
	bool HidePI(void);
	void ResetCaptuerNum(void);
	void OnMsnPacket(WReceivedPacket& packet);
	void OnUDPPacket(WReceivedPacket& packet);
	bool OnLoginServerResult(int result, FrForm* pForm);
	bool OnGameServerResult(int result, FrForm* pForm);
	void OnPacket(WReceivedPacket& packet);
	void ReportError(int code, const char* format, ...);
	void GameGuardLog(const char* format, ...);
	void ProcessRecvPacketQueue(void);
	void FromGolfToLobbySettings(void);
	bool CheckUIHack(void);
	bool Init(void);
	virtual int MainLoop(int);

protected:
	void FPU_Check(void);
	void OpenDisconDlg(const char* message, bool gameServer);
	bool SetVideoDevice(void);
	bool SetInputDevice(void);
	bool SetAudioDevice(void);
	char* GetCurrentHdd(void);
	__int64 CalcHddFreeSpace(void);
	void ProcessPacket(WReceivedPacket& packet);
	void UploadShaderSource(void);
	void SetProc(WDevice* device);
	void LoadKeyLayout(void);
	void CheckScreenShot(void);
	void AddInputDev(WInputDev* dev);

	virtual void Process(float seconds);
	virtual void Draw(void);
	virtual void Paint(void);
	virtual unsigned long GetSystemTime(void) const { return g_CurrentTime - m_createTime; }
	virtual void Update(int time);

public:
	int m_mainFlags;           // 0x5C (passed to CMainFrame::Main by MainLoop)
	bool m_loginDlgFlag;       // 0x60 (cleared by FrLoginDlg::OnInit)
	bool m_bMoveLoginServer;   // 0x61 (SetMoveLoginServer; the login dialog prefills its edits, cleared by FrLoginDlg::OnDisconPrevSession)
	PADDING(0x62, 0x64);
	std::string m_string64;    // 0x64 (only constructed so far)
	std::string m_moveLoginId;       // 0x80 (prefilled into the login dialog's ID edit)
	std::string m_moveLoginPassword; // 0x9C (prefilled into its password edit)
	CDxDiagInfo* m_dxDiagInfo; // 0xB8 (gathering starts in the constructor)
	WList<WInputDev*> m_inputDevList; // 0xBC (AddInputDev)
	unsigned long m_mainThreadId; // 0xDC (handed to the video device, SetVideoDevice)
	unsigned int m_appFlags;   // 0xE0 (bit 1: active / AVI capture mode, bit 2: windowed, SetWindowed)
	bool m_flagE4;             // 0xE4 (cleared by the constructor)
	PADDING(0xE5, 0xE8);
	unsigned long m_createTime; // 0xE8 (g_CurrentTime at construction)
	float m_monitorFPS;        // 0xEC (the video device's monitor refresh rate, floored; SetVideoDevice)
	int m_valueF0;             // 0xF0 (4 at construction)
	WDeviceManager* m_deviceManager; // 0xF4
	WProcManager* m_procManager; // 0xF8 (the window procedures of the devices, SetProc)
	WAVIEncoder m_aviEncoder;  // 0xFC
	std::deque<WReceivedPacket*> m_recvPacketQueue; // 0x22C (packets queued by OnPacket while the task is not ready)
	unsigned int m_fpuControl; // 0x240 (FPU_Check: the control word, set to 24-bit precision)
	bool m_hidePrivacy; // 0x244
	bool m_hideGUI;     // 0x245
	bool m_hidePI;      // 0x246
	PADDING(0x247, 0x248);
	int m_captureNum;   // 0x248 (ResetCaptuerNum)
	CRC_MT m_crcMT;     // 0x24C
	WMatrix m_matrix;   // 0x270 (identity at construction)
};
CHECK_SIZE(CProjectG, 0x2A0);
BEGIN_LAYOUT_CHECK(CProjectG)
	CHECK_FIELD(CProjectG, m_mainFlags, 0x5C);
	CHECK_FIELD(CProjectG, m_loginDlgFlag, 0x60);
	CHECK_FIELD(CProjectG, m_bMoveLoginServer, 0x61);
	CHECK_FIELD(CProjectG, m_moveLoginId, 0x80);
	CHECK_FIELD(CProjectG, m_moveLoginPassword, 0x9C);
	CHECK_FIELD(CProjectG, m_inputDevList, 0xBC);
	CHECK_FIELD(CProjectG, m_mainThreadId, 0xDC);
	CHECK_FIELD(CProjectG, m_appFlags, 0xE0);
	CHECK_FIELD(CProjectG, m_monitorFPS, 0xEC);
	CHECK_FIELD(CProjectG, m_deviceManager, 0xF4);
	CHECK_FIELD(CProjectG, m_procManager, 0xF8);
	CHECK_FIELD(CProjectG, m_recvPacketQueue, 0x22C);
	CHECK_FIELD(CProjectG, m_fpuControl, 0x240);
	CHECK_FIELD(CProjectG, m_hidePrivacy, 0x244);
	CHECK_FIELD(CProjectG, m_hideGUI, 0x245);
	CHECK_FIELD(CProjectG, m_hidePI, 0x246);
	CHECK_FIELD(CProjectG, m_captureNum, 0x248);
	CHECK_FIELD(CProjectG, m_string64, 0x64);
	CHECK_FIELD(CProjectG, m_dxDiagInfo, 0xB8);
	CHECK_FIELD(CProjectG, m_flagE4, 0xE4);
	CHECK_FIELD(CProjectG, m_createTime, 0xE8);
	CHECK_FIELD(CProjectG, m_valueF0, 0xF0);
	CHECK_FIELD(CProjectG, m_aviEncoder, 0xFC);
	CHECK_FIELD(CProjectG, m_crcMT, 0x24C);
	CHECK_FIELD(CProjectG, m_matrix, 0x270);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_PROJECTG_H
