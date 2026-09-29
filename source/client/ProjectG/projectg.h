#pragma once

#include <string>
#include <deque>
#include "frcmdtarget.h"
#include "mainframe.h"
#include "ccrc32.h"
#include "wangrealapp.h"

class CDxDiagInfo;
class WReceivedPacket;
class WProcManager;

extern unsigned long g_CurrentTime;

class CTmpLogo
{
public:
	CTmpLogo();
	~CTmpLogo();

	WOverlay* m_pLogo;
};

class CProjectG : public CMainFrame,
				  public CWangrealApplication,
				  public BaseObject,
				  public WSingleton<CProjectG>,
				  public FrCmdTarget
{
public:
	CProjectG(HWND hWnd);
	virtual ~CProjectG();

	bool Init();
	virtual bool Ready();
	virtual int MainLoop(int flags);
	virtual long __stdcall WinProc(HWND hWnd, unsigned int msg,
		unsigned int wParam, long lParam);

	void OnPacket(WReceivedPacket& packet);
	void OnMsnPacket(WReceivedPacket& packet);
	void OnUDPPacket(WReceivedPacket& packet);
	void ProcessRecvPacketQueue();

	bool OnLoginServerResult(int result, FrForm* pForm);
	bool OnGameServerResult(int result, FrForm* pForm);
	void SetMoveLoginServer(const char* id, const char* password);
	void FromGolfToLobbySettings();
	void SetWindowed(bool bWindowed);

	bool HidePrivacy();
	bool HideGUI();
	bool HidePI();
	void ResetCaptuerNum();
	bool CheckUIHack();

	void ReportError(int code, const char* fmt, ...);
	void GameGuardLog(const char* fmt, ...);

protected:
	virtual void Process(float dt);
	virtual void Draw();
	virtual void Paint();
	virtual unsigned long GetSystemTime() const
	{
		return g_CurrentTime - m_baseTime;
	}
	virtual void Update(int time);
	bool SetVideoDevice();
	bool SetAudioDevice();
	bool SetInputDevice();
	void SetProc(WDevice* pDevice);
	void UploadShaderSource();
	void LoadKeyLayout();
	void FPU_Check();
	void CheckScreenShot();
	void OpenDisconDlg(const char* msg, bool bQuit);

	void AddInputDev(WInputDev* pDev) { m_inputList += pDev; }
	void ProcessPacket(WReceivedPacket& packet);
	char* GetCurrentHdd();
	__int64 CalcHddFreeSpace();

public:
	int m_mainFlags;
	bool m_bMsnOffline;
	bool m_bMoveLoginServer;
	std::string m_unusedString;
	std::string m_moveLoginId;
	std::string m_moveLoginPassword;
	CDxDiagInfo* m_pDxDiagInfo;

protected:
	WList<WInputDev*> m_inputList;
	unsigned long m_threadId;
	union
	{
		struct
		{
			unsigned int m_bUnused : 1;
			unsigned int m_bAviCapture : 1;
			unsigned int m_bWindowed : 1;
		};
		unsigned int m_flag;
	};
	bool m_bLostFocus;
	unsigned long m_baseTime;
	float m_refreshRate;
	int m_reserved;
	WDeviceManager* m_pDeviceManager;
	WProcManager* m_pProcManager;
	WAVIEncoder m_avi;
	std::deque<WReceivedPacket*> m_recvPacketQueue;
	unsigned int m_fpuControl;
	bool m_bHidePrivacy;
	bool m_bHideGUI;
	bool m_bHidePI;
	int m_captureNum;
	CRC_MT m_crc;
	WMatrix m_prevCamera;
};
