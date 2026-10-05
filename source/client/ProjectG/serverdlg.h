#pragma once

#include "frform.h"
#include <map>
#include <string>

#include "../../shared/globalgamedefine.h"

class FrLogoutDlg;
class FrAutoConnect;
class cProgressBar;

enum gsLoginState_t
{
};

class FrServerDlg : public FrForm
{
	DECLARE_OBJECT(FrServerDlg)

	FrServerDlg();
	virtual ~FrServerDlg();

	virtual bool OnInit();
	virtual void OnProc(const float delta);
	virtual void OnCancel();

	void SetState(gsLoginState_t state);
	void SetLoginMessage(const char* msg);
	void SetLoginMessageAlert(const char* msg);
	void AddLoginReplyCount(int count);
	void BuildServerList();
	void BuildChannelList();
	void ConnectToServer(unsigned long serverUID);
	void FailedToConnectServer();
	void OnNotify(const char* msg, unsigned long style);
	void ClickDisableList(bool bEnable)
	{
		if (m_pServerList)
			m_pServerList->Enable(bEnable);
	}
	void BeginAutoLogin();
	void FinishAutoLogin();
	bool OnResultAutoRecon(int result, FrForm* pForm);
	bool CloseByEnterChannel(bool bRet);

protected:
	void OnServerListInit(int param);
	void OnServerListBtnDown();
	void OnServerListOwnerDraw(int param);
	void OnChannelListInit(int param);
	void OnChannelListBtnDown();
	void OnChannelListOwnerDraw(int param);
	void OnConnectMessageInit(int param);
	void OnConnectCancleInit(int param);
	void OnConnectCancleUp();
	void OnMessageInit(int param);
	void OnCurServerInit(int param);
	void OnCloseBtnInit(int param);
	void OnCloseLBtnUp();

	void ConnectToServer();
	void TurnOnChannels();
	void TurnOffChannels();
	void OnWaitNotify(const char* msg, unsigned long style);
	void OpenQuitDlg();

	bool OnLoginDlgResult(int result, FrForm* pForm);
	bool OnNotifyResult(int result, FrForm* pForm);
	bool OnWaitNotifyResult(int result, FrForm* pForm);
	bool OnQuitDlgResult(int result, FrForm* pForm);

private:
	void ClearServerList();
	bool IsNotDrawServerList(FrListItem* pItem);
	bool IsDisableEnterChannel(unsigned long flag, unsigned char level);
	void DrawServerButton(const WPoint& pt, const sGameServerInfo* pInfo,
		bool bSelected);
	void DrawChannelButton(const WPoint& pt, const sChannelInfo* pInfo,
		bool bSelected);

protected:
	int m_unknown110;
	FrListBox* m_pServerList;
	FrListBox* m_pChannelList;
	FrStatic* m_pMessage;
	FrStatic* m_pConnectMessage;
	FrStatic* m_pCurServer;
	FrButton* m_pCloseBtn;
	FrForm* m_pLoginDlg;
	gsLoginState_t m_state;
	float m_connectWaitTime;
	float m_reconnectDelay;
	bool m_bUnknown13c;
	bool m_bFirstLogin;
	bool m_bAlertMessage;
	bool m_bConnecting;
	float m_alertBlinkTime;
	sGameServerInfo m_curServer;
	const Bitmap* m_pImgServerIcon[4];
	const Bitmap* m_pImgConnection;
	const Bitmap* m_pImgNew;
	tagWTITLEFONT m_fontInfo;
	WTitleFont* m_pFont;
	FrForm* m_pNotifyDlg;
	FrForm* m_pWaitDlg;
	FrLogoutDlg* m_pQuitDlg;
	cProgressBar* m_pProgressBar;
	FrAutoConnect* m_pAutoConnect;
	int m_loginReplyCount;
	FrButton* m_pConnectCancle;
	sGameServerInfo m_serverList[9];
	std::map<unsigned int, std::string> m_serverIconMap;

	DECLARE_FRESH_MSGMAP()
};

class cProgressBar
{
public:
	struct sProgressImg
	{
		WRect rect;
		const Bitmap* pImg;
	};

	cProgressBar()
		: m_min(0), m_max(0), m_pos(0), m_marginX(0), m_marginY(0)
	{
	}
	~cProgressBar() { }

	void Initialize(const WRect& rect, float marginX, float marginY)
	{
		m_rect = rect;
		m_marginX = marginX;
		m_marginY = marginY;
	}
	void SetImg(const char* left, const char* middle, const char* right,
		const char* bar);
	void SetRange(int min, int max)
	{
		m_min = min;
		m_max = max;
	}
	void SetPos(int pos)
	{
		m_pos = pos < m_min ? m_min : pos;
		m_pos = pos > m_max ? m_max : pos;
	}
	void Display();

private:
	sProgressImg m_left;
	sProgressImg m_middle;
	sProgressImg m_right;
	sProgressImg m_bar;
	WRect m_rect;
	int m_min;
	int m_max;
	int m_pos;
	float m_marginX;
	float m_marginY;
};

class FrAutoConnect : public FrForm
{
	DECLARE_OBJECT(FrAutoConnect)

	FrAutoConnect();
	virtual ~FrAutoConnect();

	virtual bool OnInit();
	virtual void OnProc(const float delta);

	void Retry();

protected:
	void OnDrawArea();
	void OnInitStaticMsg(int param);
	void OnInitBt1(int param);
	void OnInitBt2(int param);

protected:
	cProgressBar* m_pProgressBar;
	FrStatic* m_pStaticMsg;
	int m_state;
	float m_elapsed;
	float m_retryTime;

	DECLARE_FRESH_MSGMAP()
};
