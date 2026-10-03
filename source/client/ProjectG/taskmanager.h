#pragma once

#include <memory>
#include "work.h"
#include "anotherhand.h"
#include "frcmdtarget.h"

class IActor;
class MsgObject;

class CTaskDoc;
class Fresh;
class FrForm;
class FrGaugeBar;
class WReceivedPacket;
struct sActor;

class CTask : public IObject, public CHandOwner, public FrCmdTarget
{
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI() const { return &m_RTTI; }

	struct TIP_ITEM
	{
		int id;
		std::string text;
	};

	CTask();
	virtual ~CTask();

	IActor* AddActor(const char* className, const char* name, int skipLayer,
		const std::string& option);
	IActor* GetActor(const char* name);
	void PushMsg(void* pData);
	void SetInited(bool bInited);
	IActor* GetMainActor() const { return m_pMainActor; }
	CTaskDoc* GetDocument() const { return m_pDoc; }

	virtual void ApplyScreenSize();
	virtual void Register() { }
	virtual void Init(const char* wallPaper);
	virtual void PreLoadInit();
	virtual void Reload();
	virtual void ProcessNetSyncActor(bool bForce, float step, int ballState);
	virtual void Process(float delta);
	virtual void Display();
	virtual void Destroy();
	virtual void SendGlobalMsg(MsgObject msg);
	virtual void SendMsgToMainActor(MsgObject msg);
	virtual int OnPacket(WReceivedPacket& packet) { return 0; }
	virtual void OnInitFinished() { }

	bool IsInited() { return (m_taskFlag & 2) ? true : false; }
	void SetPause(bool bPause) { m_bPause = bPause; }
	bool IsRecieveWhisper() { return m_bRecieveWhisper; }
	void SetWhisper(bool bWhisper) { m_bRecieveWhisper = bWhisper; }

protected:
	virtual void OnProcessWhileLoading(float delta);
	virtual void OnDisplayWhileLoading();
	virtual void Load();
	virtual void PreserveBack(const char* name);
	virtual void RestorePreserved();
	virtual void OnAnotherHand(int key, void* pParam);
	void SetMainActor(IActor* pActor);
	void SetProgressBar(bool bProgressBar);
	void SetLoadingBackGround(const std::string& name);
	FrForm* LoadTipText(int a, int b);
	bool OnTipDlgResult(int result, FrForm* pForm);
	bool OnOverlappedDlg(int result, FrForm* pForm);
	int OnPacketCommon(WReceivedPacket& packet);
	void SetTip(bool bTip) { m_bTip = bTip; }

protected:
	static IActor* ms_pDummyActor;
	bool m_bSyncLoad;
	WList<sActor*> m_actorList;
	IActor* m_pMainActor;
	CTaskDoc* m_pDoc;
	bool m_bLobbyScreenSize;
	std::string m_loadingBackGround;
	bool m_bProgressBar;
	bool m_bTip;
	Fresh* m_pLoadingFresh;
	FrForm* m_pTipForm;
	FrGaugeBar* m_pProgressGauge;
	CAnotherHand m_anotherHand;
	unsigned long m_taskFlag;
	bool m_bRecieveWhisper;
	bool m_bPause;
	float m_netSyncTime;
};

class CTaskManager : public WSingleton<CTaskManager>
{
public:
	CTaskManager();
	virtual ~CTaskManager();

	int PostMsg(const IActor* sender, const char* target, int message,
		int param1, int param2, int param3, unsigned long time);

	void ChangeTask(const char* taskName, const char* docName, bool bPreserve);
	CTask* GetCurrentTask() const;
	CTask* GetPreservedTask() const { return m_pPreservedTask; }

	void SetWork(CWork* pWork) { m_pWork.reset(pWork); }
	void ReleaseWork() { delete m_pWork.release(); }

protected:
	unsigned char m_unused28[0x10];
	CTask* m_pPreservedTask;
	unsigned char m_unused3c[0x30];
	std::auto_ptr<CWork> m_pWork;
	// TODO: this class definition is incomplete
};

inline CTask* AfxGetTask()
{
	if (CTaskManager::Instance() == NULL)
		return NULL;
	return CTaskManager::Instance()->GetCurrentTask();
}

inline CTask* AfxGetPreservedTask()
{
	if (CTaskManager::Instance() == NULL)
		return NULL;
	return CTaskManager::Instance()->GetPreservedTask();
}
