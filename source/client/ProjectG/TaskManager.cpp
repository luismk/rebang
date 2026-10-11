// Original: d:\build\custom\temp\code\client\projectg\taskmanager.cpp
#define TASKMANAGER_TASKMANAGER_INLINE
#include "NetworkSystem.h"
#include "TaskManager.h"
#include "fresh.h"
#include "actor.h"
#include "Packet.h"
#include "ProjectG.h"
#include "ClientSetting.h"
#include "frgaugebar.h"
#include "background.h"
#include "CapturedBg.h"
#include "hackingmanager.h"
#include "netsyncactor.h"
#include "frdesktop.h"
#include "GolfTask.h"
#include "ChatMsg.h"
#include "wview.h"
#include "SharedDoc.h"
#include "fx.h"
#include "work.h"
#include "TaskMain.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "messengerdlg.h"
#include "golfball.h"
#include "ShopTask.h"
#include "binstr.h"
#include "westpak.h"
#include "wresrcmng.h"
#include "frform.h"
#include "PangGiftDlg.h"
#include "PrivateTrade.h"
#include "ServerDlg.h"
#include "UccManager.h"
#include "UccLibrary.h"
#include "NetResourceManager.h"
#include "contentsdoc.h"
#include "MessageManager.h"
#include "commonutil.h"
#include "TutorialTask.h"
#include "s5/Utilities.h"
#include "s5/SkinnedTheme.h"
#include "logininfo.h"
#include "user_info.h"
#include "onelineboard.h"
#include "parttidlist.h"
#include "RealMyRoomData.h"
#include "avatar_task.h"
#include "LobbyTask.h"
#include "Intrusion.h"
#include <stdlib.h>
#include <mmsystem.h>
#include <string.h>
#include "pg_boot_todo.h"

// ProjectG.cpp (no header declares it yet)
void TextOut(WView *view, float x, float y, const char *format, ...);

// Queue of GameGuard CSAuth2 answers: GameGuard's callback thread pushes
// them (CTask::PushMsg), the game thread sends them to the server (Process).
class GameGuardMsgHandler {
	LAYOUT_CHECK_FRIEND;

public:
	// One CSAuth2 answer, sent as is (0x10 bytes).
	struct _GG_AUTH_DATA {
		UNKNOWN(0x0, 0x10);
	};

	GameGuardMsgHandler(void);
	~GameGuardMsgHandler(void);
	void Process(void);
	void Push(void *pMsg);

private:
	UNKNOWN(0x0, 0x10);
	std::list<_GG_AUTH_DATA> m_authList; // 0x10
	CRITICAL_SECTION m_cs;               // 0x1C
	bool m_bPushed;                      // 0x34 (answers are waiting)
};
CHECK_SIZE(GameGuardMsgHandler::_GG_AUTH_DATA, 0x10);
BEGIN_LAYOUT_CHECK(GameGuardMsgHandler)
	CHECK_FIELD(GameGuardMsgHandler, m_authList, 0x10);
	CHECK_FIELD(GameGuardMsgHandler, m_cs, 0x1C);
	CHECK_FIELD(GameGuardMsgHandler, m_bPushed, 0x34);
END_LAYOUT_CHECK

extern bool g_bQuit;

// The room record of the old room-list layout (0x7B bytes), which
// CTask::OnPacketCommon 0x45 converts into an sRoomInfo under local content
// 0x5E. The trailing five bytes are not read.
#pragma pack(push, 1)
struct sRoomInfoLegacy {
	char m_title[0x20];                  // 0x00
	unsigned char m_isPublic : 1;        // 0x20 bit 0
	unsigned char m_intrusionAccess : 1; // 0x20 bit 1
	unsigned char m_state : 1;           // 0x20 bit 2
	unsigned char m_userLimit;           // 0x21
	unsigned char m_userCount;           // 0x22
	unsigned char m_holeCount;           // 0x23
	unsigned char m_gameType;            // 0x24
	unsigned short m_roomNumber;         // 0x25
	unsigned char m_course;              // 0x27
	unsigned long m_trophy;              // 0x28
	unsigned long m_guildUID[2];         // 0x2C (with the names and marks: sRoomInfo's 0x4A-byte guild block at 0x5B)
	char m_guildName[2][21];             // 0x34
	char m_guildMark[2][12];             // 0x5E
	PADDING(0x76, 0x7B);
};
#pragma pack(pop)

CHECK_SIZE(sRoomInfoLegacy, 0x7B);
CHECK_FIELD(sRoomInfoLegacy, m_userLimit, 0x21);
CHECK_FIELD(sRoomInfoLegacy, m_userCount, 0x22);
CHECK_FIELD(sRoomInfoLegacy, m_holeCount, 0x23);
CHECK_FIELD(sRoomInfoLegacy, m_gameType, 0x24);
CHECK_FIELD(sRoomInfoLegacy, m_roomNumber, 0x25);
CHECK_FIELD(sRoomInfoLegacy, m_course, 0x27);
CHECK_FIELD(sRoomInfoLegacy, m_trophy, 0x28);
CHECK_FIELD(sRoomInfoLegacy, m_guildUID, 0x2C);
CHECK_FIELD(sRoomInfoLegacy, m_guildName, 0x34);
CHECK_FIELD(sRoomInfoLegacy, m_guildMark, 0x5E);

// Loading state shared with the tasks' loaders (only the member this unit's
// code reads so far is named).
struct sLoadParam {
	WList<sActor *> *m_pActorList; // 0x0 (the actors CTask::OnAnotherHand loads)
	int m_actorCount; // 0x4 (their number: the loading gauge's range is m_actorCount * 100)
	int m_progress;   // 0x8 (loading step, shown by the loading gauge)
	bool m_bInited;   // 0xC (the loaded task was initialised: CTaskManager::ProcessTask; until then DisplayTask shows its loading screen)
	bool m_bLoaded;   // 0xD (set by CTask::OnAnotherHand once every actor is loaded)
};
BEGIN_LAYOUT_CHECK(sLoadParam)
	CHECK_FIELD(sLoadParam, m_actorCount, 0x4);
	CHECK_FIELD(sLoadParam, m_progress, 0x8);
	CHECK_FIELD(sLoadParam, m_bInited, 0xC);
	CHECK_FIELD(sLoadParam, m_bLoaded, 0xD);
END_LAYOUT_CHECK
extern sLoadParam g_LoadParam;

// Loading step last shown by CTask::OnProcessWhileLoading.
extern "C" int data_b79bd0;
extern int g_iRecvPacketType;

static const char *s_guildActorName = "GuildActor";

// objectfactory.h:103 (an inline of objectfactory.h's class body that the
// compiler did not inline; the linker kept this object's copy. It belongs in
// ObjectFactory.h behind a TASKMANAGER_OBJECTFACTORY_INLINE macro.)
IObject* CObjectFactory::CreateObject(const char* name) {
	ObjectFunctorMap::iterator it = m_objectFunctors.find(name);
	if (it != m_objectFunctors.end()) {
		return it->second();
	}
	return 0;
}

// taskmanager.cpp:204
GameGuardMsgHandler::GameGuardMsgHandler(void)
	: m_bPushed(false) {
	InitializeCriticalSection(&m_cs);
}

// taskmanager.cpp:208
GameGuardMsgHandler::~GameGuardMsgHandler(void) {
	DeleteCriticalSection(&m_cs);
}

// taskmanager.cpp:213
void GameGuardMsgHandler::Process(void) {
	if (!m_bPushed) {
		return;
	}

	std::list<_GG_AUTH_DATA> authList;

	EnterCriticalSection(&m_cs);
	std::list<_GG_AUTH_DATA>::iterator it = m_authList.begin();
	for (; it != m_authList.end(); ++it) {
		authList.push_back(*it);
	}
	m_bPushed = false;
	m_authList.clear();
	LeaveCriticalSection(&m_cs);

	it = authList.begin();
	for (; it != authList.end(); ++it) {
		WSendPacket packet(0x85);
		packet.EncodeBuffer(&*it, sizeof(_GG_AUTH_DATA));
		packet.Send(SEND_TO_DEFAULT);
		CProjectG::Instance()->GameGuardLog("CSAuth2 responded.");
	}
}

// taskmanager.cpp:243
void GameGuardMsgHandler::Push(void *pMsg) {
	EnterCriticalSection(&m_cs);
	m_authList.push_back(*(_GG_AUTH_DATA *)pMsg);
	m_bPushed = true;
	LeaveCriticalSection(&m_cs);
}

// taskmanager.cpp:256
GameGuardMsgHandler g_gameGuardMsgHandler;

// shareddoc.h:551
void CSharedDoc::SetBsUsableTimes(short times) {
	m_userInfo.bsUsableTimes = times;
}

// shareddoc.h:553
void CSharedDoc::SetBsUsableBonusTimes(short times) {
	m_userInfo.bsUsableBonusTimes = times;
}

// shareddoc.h:555
void CSharedDoc::SetBsRemainedBonusTimes(short times) {
	m_userInfo.bsRemainedBonusTimes = times;
}

// shareddoc.h:622
void CSharedDoc::SetFlagBlock(unsigned long flagBlock) {
	m_userInfo.flagBlock = flagBlock;
}

// taskmanager.cpp:260
void RecvRSS0(int)
{
}

// taskmanager.cpp:264
const WRTTI CTask::m_RTTI("CTask", &IObject::m_RTTI);

IObject *CTaskMakeInstance(void) {
	return new CTask;
}

struct __sCTask {
	__sCTask(void) {
		ObjectFactory().AddObjectFunctor(CTaskMakeInstance, "CTask");
	}
};
static __sCTask s_registerCTask;

// taskmanager.cpp:268
IActor *CTask::ms_pDummyActor = ActorFactory().CreateActor("Dummy");

// taskmanager.cpp:506
CTask::CTask(void)
	: m_unknownC(false), m_actorList(16, 16), m_mainActor(NULL), m_pDoc(NULL), m_unknown38(true),
	  m_loadingBackGround(""), m_bProgressBar(true), m_unknown59(true), m_pLoadingFresh(NULL), m_tipDlgState(0) {
	m_stateFlags = 0;
	m_pause = false;
	m_stateFlags = 0;
	m_netSyncTime = 0;
	m_bRecieveWhisper = true;
}

// taskmanager.cpp:516
CTask::~CTask(void) {
	sActor *pActor = m_actorList.Start();
	while (pActor) {
		delete pActor->m_pActor;
		delete[] (char *)pActor;
		pActor = m_actorList.Next();
	}
	m_actorList.Reset();

	if (m_pDoc) {
		delete m_pDoc;
		m_pDoc = NULL;
	}
	if (m_pLoadingFresh) {
		delete m_pLoadingFresh;
		m_pLoadingFresh = NULL;
	}

	if (CFx::Instance()) {
		CFx::Instance()->ClearFxKeeper();
	}
}

// taskmanager.cpp:540
void CTask::SetMainActor(IActor *pActor) {
	m_mainActor = pActor;
}

// taskmanager.cpp:546
bool CTask::OnTipDlgResult(int result, FrForm *pForm) {
	m_pTipForm = 0;

	m_tipDlgState = 0;

	return true;
}

// taskmanager.cpp:560
FrForm *CTask::LoadTipText(int, int) {
	cFile *pFile = g_resrcmng->GetCFile("tip.txt", 0xFFFF);
	if (pFile == NULL) {
		return NULL;
	}

	std::list<TIP_ITEM> tipList;
	char line[0x2000];
	while (pFile->Scan("%n", line)) {
		int len = 0;
		for (int i = 0; line[i]; i++, len++) {
			if (line[i] == '\\' && line[i + 1] == 'n') {
				i++;
				line[len] = '\n';
			} else {
				line[len] = line[i];
			}
		}
		line[len] = 0;

		int lineLen = strlen(line);
		cTokenV token;
		token.Init(line, lineLen);

		char *pNumber;
		token.GetTokenWithoutFail(&pNumber, " \t", 2);
		if (token.GetPos() + 1 > lineLen) {
			CloseCFile(pFile);
			return NULL;
		}

		TIP_ITEM item;
		item.m_number = atoi(pNumber);
		item.m_text = &line[token.GetPos()];

		tipList.push_back(item);
	}
	CloseCFile(pFile);

	int index = rand() % tipList.size() + 1;
	FrForm *pForm = CreateForm<FrForm>(m_pLoadingFresh->GetManager(), this, "TIP", NULL);
	if (index && pForm) {
		std::list<TIP_ITEM>::iterator it = tipList.begin();
		while (it != tipList.end() && index > 1) {
			++it;
			index--;
		}

		pForm->SetMessage(it->m_text.c_str(), false);
		pForm->EnableDrag(false);
		pForm->Open((FRESH_RESULT_CALLBACK)&CTask::OnTipDlgResult, 8);
		return pForm;
	}

	return NULL;
}

// taskmanager.cpp:643
void CTask::SetProgressBar(bool show) {
	m_bProgressBar = show;
}

// taskmanager.cpp:648
void CTask::SetLoadingBackGround(const std::string &backGround) {
	m_loadingBackGround = backGround;
}

// taskmanager.cpp:653
IActor *CTask::AddActor(const char *className, const char *name, int layer, const std::string &str) {
	IActor *pActor = ActorFactory().CreateActor(className);
	if (pActor == NULL) {
		return NULL;
	}

	pActor->m_pDocument = m_pDoc;
	pActor->m_pTask = this;

	sActor *pEntry = (sActor *)new char[strlen(className) + sizeof(sActor)];
	pEntry->m_pActor = pActor;
	pEntry->m_name = name;
	pEntry->m_pActor->RemoveSkipList(layer);

	pEntry->m_unknown4 = false;

	m_actorList.AddItem(pEntry, pEntry->m_name, false);
	if (!str.empty()) {

		pActor->HandleMsg(MsgObject(NULL, 0x2712, (intptr_t)str.c_str(), 0, 0, 0, 0));
	}

	return pActor;
}

// taskmanager.cpp:678
IActor *CTask::GetActor(const char *name) {
	sActor *pActor = m_actorList.Find(name);

	if (pActor == NULL) {
		return NULL;
	}

	return pActor->m_pActor;
}

// taskmanager.cpp:703
void SendPacket(unsigned char value) {
	WSendPacket packet(0x48);
	packet.Encode1(value);
	packet.Send(SEND_TO_DEFAULT);
}

// taskmanager.cpp:710
void CTask::OnAnotherHand(int nWork, void *pData) {
	if (nWork == 0) {
		sLoadParam *pParam = (sLoadParam *)pData;

		sActor *pActor = pParam->m_pActorList->Start();






		while (pActor) {

			unsigned long startTime = timeGetTime();

			if ((__rtti_obj = this) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI) && OnlinePlay()) {
				switch (CSharedDoc::Instance()->m_roomType) {
				case 4:
				case 5:
				case 6:
				case 9:
				case 10:
					PG_BOOT_TODOF(("taskmanager.cpp:710 empty case %#x (of 4, 5, 6, 9, 10)", (unsigned)(CSharedDoc::Instance()->m_roomType)));
					break;
				default:
					if (pParam->m_progress % 3 == 0) {
						int value;
						if (pParam->m_progress == 23)
							value = 10;
						else
							value = pParam->m_progress / 3 + 2;
						SendPacket(value);
					}
					break;
				}
			}

			pActor->m_pActor->OnLoad();

			unsigned long endTime = timeGetTime();




			pActor = pParam->m_pActorList->Next();
			pParam->m_progress++;
		}






		pParam->m_bLoaded = true;
	}
}

// taskmanager.cpp:768
void CTask::Load(void) {
	g_LoadParam.m_pActorList = &m_actorList;
	g_LoadParam.m_actorCount = m_actorList.GetCount();

	m_pLoadingFresh = new Fresh;
	m_tipDlgState = NULL;

	if (m_loadingBackGround != "") {
		m_pLoadingFresh->Init("loading.xml", false, m_loadingBackGround.c_str());
	} else {
		m_pLoadingFresh->Init("loading.xml", false, NULL);
	}

	m_pLoadingFresh->OpenLayout("LOADING", NULL, true);

	if (m_unknown59) {
		m_pTipForm = LoadTipText(0, 0);
		if (m_pTipForm) {
			m_tipDlgState = DYNAMIC_CAST(FrGaugeBar, m_pTipForm->FindChildByName("progress"));
			if (m_tipDlgState) {
				m_tipDlgState->SetRange(0, g_LoadParam.m_actorCount * 100, 0);
				m_tipDlgState->SetDestPos(100, false);
			}
		}
	}

	if (m_unknownC) {
		OnAnotherHand(0, &g_LoadParam);
	} else {
		m_anotherHand.Work(this, 0, &g_LoadParam, false, 0);
	}
}

// taskmanager.cpp:842
void CTask::ApplyScreenSize(void) {
	if (COption::Instance()) {
		if (m_unknown38) {
			COption::Instance()->vApplyLobbyScreenSize();
		} else {
			COption::Instance()->vApplyGameScreenSize();
		}
	}
}

// taskmanager.cpp:857
void CTask::Init(const char *backGround) {
	if (m_pLoadingFresh) {
		delete m_pLoadingFresh;
		m_pLoadingFresh = NULL;
	}

	sActor *pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnInit();
		pActor = m_actorList.Next();
	}

	if (!IsInited()) {

		m_stateFlags |= 2;
		OnInitFinished();
	}

	g_pFresh->GetDesktop()->Init(g_view->GetWidth(), g_view->GetHeight());

	g_pFresh->GetDesktop()->SetWallPaper(backGround, true);

	if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&CGolfTask::m_RTTI) : false)) {

		CChatMsg::Instance()->SetActive(true, true, true);
	}
}

// taskmanager.cpp:884
void CTask::SetInited(bool inited) {
	if (inited) {
		m_stateFlags |= 2;
	} else {
		m_stateFlags &= ~2;
	}
}

// taskmanager.cpp:892
void CTask::PreLoadInit(void) {




	sActor *pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnPreLoadInit();
		pActor = m_actorList.Next();
	}
}

// taskmanager.cpp:906
void CTask::Reload(void) {
	sActor *pActor = m_actorList.Start();
	while (pActor) {

		pActor->m_pActor->OnReload();
		pActor = m_actorList.Next();
	}
}

// taskmanager.cpp:916
void CTask::Destroy(void) {

	g_pFresh->CloseLayout();

	sActor *pActor = m_actorList.Start();
	while (pActor) {


		pActor->m_pActor->OnDestroy();
		pActor = m_actorList.Next();
	}
}

// taskmanager.cpp:930
void CTask::OnProcessWhileLoading(float dt) {
	if (m_pLoadingFresh) {
		m_pLoadingFresh->OnProcess(dt);

		if (data_b79bd0 != g_LoadParam.m_progress) {
			data_b79bd0 = g_LoadParam.m_progress;
			if (m_bProgressBar && m_tipDlgState) {
				m_tipDlgState->SetPos(g_LoadParam.m_progress * 100 - 100);
				m_tipDlgState->SetDestPos(g_LoadParam.m_progress * 100, false);
			}
		}
	}
}

// taskmanager.cpp:952
void CTask::PushMsg(void *pMsg) {
	g_gameGuardMsgHandler.Push(pMsg);
}

// taskmanager.cpp:990
void CTask::PreserveBack(const char *layoutName) {


	for (sActor *pActor = m_actorList.Start(); pActor; pActor = m_actorList.Next())
		pActor->m_pActor->OnPreserveBack(layoutName);
}

// taskmanager.cpp:998
void CTask::RestorePreserved(void) {


	for (sActor *pActor = m_actorList.Start(); pActor; pActor = m_actorList.Next())
		pActor->m_pActor->OnRestorePreserved();
}

// taskmanager.cpp:1006
void CTask::ProcessNetSyncActor(bool active, float elapsed, int frame) {




	sActor *pActor = m_actorList.Start();
	while (pActor) {
		if (!pActor->m_pActor->Skipped()) {


			NetSyncActor *pNetSyncActor = (NetSyncActor *)pActor->m_pActor->DynamicCast(&NetSyncActor::m_RTTI);

			if (pNetSyncActor) {

				pNetSyncActor->NSA_Step(active, elapsed, frame);
			}
		}

		pActor = m_actorList.Next();
	}
}

// taskmanager.cpp:1029
void CTask::Process(float dt) {
	g_gameGuardMsgHandler.Process();

	m_mainActor->OnProcess(dt);

	if (m_pause) {
		return;
	}

	sActor *pActor = m_actorList.Start();
	while (pActor) {
		if (pActor->m_pActor != m_mainActor && !pActor->m_pActor->Skipped()) {
			pActor->m_pActor->OnProcess(dt);
		}
		if (HackingManager::Instance()) {
			HackingManager::Instance()->CheckCrypticValue();
		}
		pActor = m_actorList.Next();
	}

	bool bReset = false;
	if (GolfBall().m_preDataIndex == 0) {
		bReset = true;
	}

	if (dt > 0.0f) {
		m_netSyncTime += dt;
		if (bReset) {
			m_netSyncTime = 0.022f;
		}

		while (m_netSyncTime > 0.02f) {
			if (GolfBall().m_preDataIndex == -1 || GolfBall().m_preDataIndex == GolfBall().m_preDataCount) {
				ProcessNetSyncActor(false, 0.02f, GolfBall().m_preDataIndex);
			} else if (GolfBall().m_preDataIndex == 0) {
				ProcessNetSyncActor(true, 0.02f, GolfBall().m_preDataIndex);
			} else {
				GolfBall();
			}
			m_netSyncTime -= 0.02f;
		}
	}

	if (CSceneManager::Instance() && !CCapturedBg::Instance()->IsCaptured()
		&& !((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&CShopTask::m_RTTI) : false)) {
		CSceneManager::Instance()->Process(dt);
	}
}

// taskmanager.cpp:1136
void CTask::OnDisplayWhileLoading(void) {
	if (m_pLoadingFresh) {
		m_pLoadingFresh->OnDisplay(true);
	}
}

// taskmanager.cpp:1142
void CTask::Display(void) {
	for (int priority = 0; priority <= 5; priority++) {
		void *pos;
		sActor *pActor = m_actorList.Start(pos);
		while (pActor) {
			if (pActor->m_pActor && pActor->m_pActor->GetDisplayPriority() == priority && !pActor->m_pActor->Skipped()) {
				pActor->m_pActor->OnDisplay();
			}
			pActor = m_actorList.Next(pos);
		}

		if (priority == 2) {
			if (!(CCapturedBg::Instance() && CCapturedBg::Instance()->IsCaptured())) {
				if (CSceneManager::Instance()) {
					CSceneManager::Instance()->Prerender(g_view);
					CSceneManager::Instance()->Render(g_view, 0xffffffff);
				}

				if (!(m_stateFlags & 1)) {
					g_view->Clear(0, 2);
				}
			}
		} else if (priority == 4 || priority == 1) {
			g_view->Flush(0);
		}
	}
}

// taskmanager.cpp:1219
void CTask::SendGlobalMsg(MsgObject msg) {
	IActor *pMainActor = m_mainActor;
	if (pMainActor) {
		pMainActor->HandleMsg(msg);
	}

	void *pos;
	sActor *pActor = m_actorList.Start(pos);
	while (pActor) {
		if (pActor->m_pActor != m_mainActor) {
			if (pActor->m_pActor) {
				pActor->m_pActor->HandleMsg(msg);
			}
		}
		pActor = m_actorList.Next(pos);
	}
}

// taskmanager.cpp:1237
void CTask::SendMsgToMainActor(MsgObject msg) {
	IActor *pMainActor = m_mainActor;
	if (pMainActor) {
		pMainActor->HandleMsg(msg);
	}
}

// taskmanager.cpp:1246
int CTask::OnPacketCommon(WReceivedPacket &packet) {
	packet.Reset();
	unsigned short type = packet.Decode2();
	g_iRecvPacketType = type;

	switch (g_iRecvPacketType) {
	case 0x1d3:
		// taskmanager.cpp:1259
		{
			unsigned long sendType = packet.Decode4();
			unsigned long value = packet.Decode4();
			// taskmanager.cpp:1262
			WSendPacket send(sendType);
			send.Encode4(CSharedDoc::Instance()->m_userInfo.guildUID);
			send.Encode4(value);
			send.Send(SEND_TO_MSN);
		}
		break;
	case 0x187:
		// taskmanager.cpp:1280
		{
			IActor *pActor = AfxGetTask()->GetActor("PrivateTrade");
			if (!pActor) {
				break;
			}
			// taskmanager.cpp:1283
			CPrivateTrade *pTrade = DYNAMIC_CAST(CPrivateTrade, pActor);
			pTrade->OnPacket(&packet);
		}
		break;
	case 0x19c:
		// taskmanager.cpp:1290
		PG_BOOT_TODO("taskmanager.cpp:1290 case 0x19c: 614 bytes; calls FrWnd::FindChildForm, CreateForm<FrForm>"); // TODO(boot)
		break;
	case 0x11b:
		{
			// taskmanager.cpp:1361
			unsigned char type = packet.Decode1();
			// taskmanager.cpp:1362
			unsigned char value = packet.Decode1();
			// taskmanager.cpp:1364
			if (type == 3) {
				// taskmanager.cpp:1366
				packet.DecodeBuffer(CSharedDoc::Instance()->m_tutorialData, 0xC);
			}
			// taskmanager.cpp:1369
			if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&CTutorialTask::m_RTTI) : false) {
				// taskmanager.cpp:1371
				AfxPostMsg(0, "TutorialMain", 0x151, type, value, 0, 0);
			}
		}
		break;
	case 0x112:
		// taskmanager.cpp:1380
		PG_BOOT_TODO("taskmanager.cpp:1380 case 0x112: 99 bytes; calls CreateForm<FrForm>"); // TODO(boot)
		break;
	case 0xf0:
		{
			// taskmanager.cpp:1405
			unsigned char value = packet.Decode1();
			// taskmanager.cpp:1406
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x6B, value, 0, 0, 0, 0));
		}
		// taskmanager.cpp:1408
		break;
	case 0x75:
		// taskmanager.cpp:1414
		PG_BOOT_TODO("taskmanager.cpp:1414 case 0x75: 455 bytes; calls GalleryMode, CSharedDoc::ClearRivalVars, CItemManager::FindCaddie"); // TODO(boot)
		break;
	case 0x74:
		// taskmanager.cpp:1469
		PG_BOOT_TODO("taskmanager.cpp:1469 case 0x74: 1322 bytes; calls GalleryMode, IsMassGame, IsLocalContent, CSharedDoc::SetServerTime"); // TODO(boot)
		break;
	case 0x50:
		// taskmanager.cpp:1632
		PG_BOOT_TODO("taskmanager.cpp:1632 case 0x50: 801 bytes; calls FrWnd::FindChildByName, FrBongdariShopDlg::Clear, CUserInfo::Clear, IsLocalContent, CTHunter::SetTreasurePoint, CIconToolTip::ClearToolTip"); // TODO(boot)
		break;
	case 0x3e:
		// taskmanager.cpp:1742
		PG_BOOT_TODO("taskmanager.cpp:1742 case 0x3e: 1661 bytes; calls ChatManager::FilteringHack"); // TODO(boot)
		break;
	case 0x3f:
		// taskmanager.cpp:1875
		PG_BOOT_TODO("taskmanager.cpp:1875 case 0x3f: 89 bytes; calls CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0x40:
		// taskmanager.cpp:1885
		PG_BOOT_TODO("taskmanager.cpp:1885 case 0x40: 92 bytes; calls CNoticeBoard::AddNotice"); // TODO(boot)
		break;
	case 0x1d5:
		// taskmanager.cpp:1896
		PG_BOOT_TODO("taskmanager.cpp:1896 case 0x1d5: 137 bytes; calls CWinnerNotice::SetWinnerData"); // TODO(boot)
		break;
	case 0x41:
		// taskmanager.cpp:1938
		PG_BOOT_TODO("taskmanager.cpp:1938 case 0x41: 239 bytes; calls CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0x42:
		// taskmanager.cpp:1958
		{
			unsigned char result = packet.Decode1();
			// taskmanager.cpp:1960
			switch (result) {
			case 0xd3:
				// taskmanager.cpp:1964
				{
					FrWnd *pForm = g_pFresh->GetManager()->GetDesktop()->FindChildForm("server");
					// taskmanager.cpp:1965
					if (!pForm || !pForm->IsKindOf(&FrServerDlg::m_RTTI)) {
						break;
					}
					// taskmanager.cpp:1968
					if (packet.Decode1() == 0) {
						// taskmanager.cpp:1971
						((FrServerDlg *)pForm)->FinishAutoLogin();
					} else {
						// taskmanager.cpp:1975
						((FrServerDlg *)pForm)->BeginAutoLogin();
					}
				}
				break;
			case 0xd2:
				// taskmanager.cpp:1982
				{
					unsigned long count = packet.Decode4();
					// taskmanager.cpp:1985
					FrWnd *pForm = g_pFresh->GetManager()->GetDesktop()->FindChildForm("server");
					// taskmanager.cpp:1986
					if (!pForm || !pForm->IsKindOf(&FrServerDlg::m_RTTI)) {
						break;
					}
					// taskmanager.cpp:1989
					((FrServerDlg *)pForm)->AddLoginReplyCount(count);
				}
				break;
			case 0:
				// taskmanager.cpp:1996
				{
					FrWnd *pForm = g_pFresh->GetManager()->GetDesktop()->FindChildForm("server");
					// taskmanager.cpp:1997
					if (pForm && pForm->IsKindOf(&FrServerDlg::m_RTTI)) {
						// taskmanager.cpp:2000
						((FrServerDlg *)pForm)->AddLoginReplyCount(0x15);
					}

					// taskmanager.cpp:2004
					UccManager()->BuildDisplayClothesSet();
					// taskmanager.cpp:2007
					CSharedDoc::Instance()->ClearInventory();

					// taskmanager.cpp:2012
					if (((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsKindOf(&CShopTask::m_RTTI) : false) == true) {
						// taskmanager.cpp:2014
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xC5, 0, 0, 0, 0, 0));
					}

					// taskmanager.cpp:2017
					CSharedDoc::Instance()->m_latestVersion = packet.DecodeStr();
					// taskmanager.cpp:2021
					CSharedDoc::Instance()->m_string4D4C = packet.DecodeStr();

					// taskmanager.cpp:2025
					sUserInfoExt userInfoExt;
					// taskmanager.cpp:2026
					sUserInfo userInfo;
					// taskmanager.cpp:2027
					if (IsLocalContent((localContentType_t)0x70)) {
						// taskmanager.cpp:2029
						packet.DecodeBuffer(&userInfoExt, sizeof(sUserInfoExt));
						// taskmanager.cpp:2030
						userInfo = userInfoExt;
						// taskmanager.cpp:2031
						memcpy(CSharedDoc::Instance()->m_userInfoExtBlock, userInfoExt.m_extBlock, sizeof(userInfoExt.m_extBlock));
					} else {
						// taskmanager.cpp:2035
						packet.DecodeBuffer(&userInfo, sizeof(sUserInfo));
					}

					// taskmanager.cpp:2038
					RequestUccInfo(userInfo);

					// taskmanager.cpp:2041
					_SYSTEMTIME serverTime;
					packet.DecodeBuffer(&serverTime, sizeof(serverTime));
					// taskmanager.cpp:2042
					CSharedDoc::Instance()->SetServerTime(serverTime);

					// taskmanager.cpp:2044
					unsigned char firstLogin = packet.Decode1();
					// taskmanager.cpp:2045
					packet.Decode1();

					// taskmanager.cpp:2049
					CSharedDoc::Instance()->SetBsUsableTimes(packet.Decode2());
					// taskmanager.cpp:2050
					CSharedDoc::Instance()->SetBsUsableBonusTimes(packet.Decode2());
					// taskmanager.cpp:2051
					CSharedDoc::Instance()->SetBsRemainedBonusTimes(packet.Decode2());
					// taskmanager.cpp:2053
					if (IsLocalContent((localContentType_t)8)) {
						// taskmanager.cpp:2054
						CSharedDoc::Instance()->SetFlagBlock(packet.Decode4());
					}

					// taskmanager.cpp:2056
					CSharedDoc::Instance()->m_controlServerService = packet.Decode4();
					// taskmanager.cpp:2057
					CSharedDoc::Instance()->m_value5060 = packet.Decode4();
					// taskmanager.cpp:2061
					unsigned long serverProperty = packet.Decode4();
					// taskmanager.cpp:2069
					CSharedDoc::Instance()->m_serverProperty = serverProperty;

					// taskmanager.cpp:2073
					GUILD_USER_INFO guildUserInfo;
					// taskmanager.cpp:2074
					packet.DecodeBuffer(&guildUserInfo, sizeof(GUILD_USER_INFO));
					// taskmanager.cpp:2075
					IActor *pGuildActor = AfxGetTask()->GetActor(s_guildActorName);
					// taskmanager.cpp:2077
					pGuildActor << MsgObject(0, 0x263, 6, (intptr_t)&guildUserInfo, 0, 0, 0);

					// taskmanager.cpp:2082
					S5::UTIL::SetServerProperty(serverProperty);
					// taskmanager.cpp:2083
					S5::THEME::SetSkinnedTheme(serverProperty);

					// The local player's copy of the user information.
					// taskmanager.cpp:2090
					CSharedDoc::Instance()->m_mySlot = userInfo.m_slot;
					// taskmanager.cpp:2091 (sPangYaUserInfo, 0x10D bytes)
					memcpy(&CSharedDoc::Instance()->m_userInfo, userInfo.m_nickname, 0x10D);
					// taskmanager.cpp:2092 (sPangYaUserStatistics, 0xEB bytes)
					memcpy(&CSharedDoc::Instance()->m_myStatShots, &userInfo.m_statShots, 0xEB);
					// taskmanager.cpp:2093
					CSharedDoc::Instance()->m_myTrophy = userInfo.m_trophy;
					// taskmanager.cpp:2094 (the 0x6C-byte equipment block)
					memcpy(&CSharedDoc::Instance()->m_myCaddieId, &userInfo.m_equipHead, 0x6C);
					// taskmanager.cpp:2098
					memcpy(CSharedDoc::Instance()->m_myMapStatistics, userInfo.m_mapStatistics, sizeof(userInfo.m_mapStatistics));
					// taskmanager.cpp:2099
					memcpy(CSharedDoc::Instance()->m_myMapStatistics2, userInfo.m_mapStatistics2, sizeof(userInfo.m_mapStatistics2));

					// taskmanager.cpp:2102
					NetResourceManager::Instance()->ResetGuildRSS();
					// taskmanager.cpp:2105
					if (userInfo.m_guildUID) {
						// taskmanager.cpp:2113
						NetResourceManager::Instance()->RequestGuildRSS(userInfo.m_guildUID, RecvRSS0);
					}

					// taskmanager.cpp:2117
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 5, 0, 0, 0, 0));
					// taskmanager.cpp:2118
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x42, 0, 0, 0, 0, 0));
					// taskmanager.cpp:2120
					if (firstLogin == 1) {
						// taskmanager.cpp:2121
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x43, 1, 0, 0, 0, 0));
					}

					// taskmanager.cpp:2126
					if (!COption::Instance()->gIsWhisperAllowed()) {
						// taskmanager.cpp:2128
						WSendPacket send((enumClientPacket)0x55);
						// taskmanager.cpp:2129
						send.Encode1(0);
						// taskmanager.cpp:2130
						send.Send(SEND_TO_DEFAULT);
					}

					// taskmanager.cpp:2133
					if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)
						&& CMessengerInfo::Instance()->m_serverListRequest == 4) {
						// taskmanager.cpp:2135
						sUserPosition position;
						// taskmanager.cpp:2136
						position.m_userValue = CSharedDoc::Instance()->m_value84;
						// taskmanager.cpp:2137
						wsprintfA(position.m_channelName, "NONE");
						position.m_roomNumber = 0xFFFF;
						position.m_value2 = 0xFFFFFFFF;
						// taskmanager.cpp:2149
						WSendPacket send(0x23);
						// taskmanager.cpp:2153
						send.EncodeBuffer(&position, sizeof(sUserPosition));
						// taskmanager.cpp:2154
						send.Send(SEND_TO_MSN);
					}

					// taskmanager.cpp:2181
					CSharedDoc::Instance()->SetPeriodDoc();
					// taskmanager.cpp:2182
					CSharedDoc::Instance()->GetPeriodDoc()->Set_PreSettingContentsDate();
				}
				break;
			case 2:
				// taskmanager.cpp:2189
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 6, 0, 0, 0, 0));
				break;
			case 4:
				// taskmanager.cpp:2195
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xB, 0, 0, 0, 0));
				break;
			case 5:
				// taskmanager.cpp:2201
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 8, 0, 0, 0, 0));
				break;
			case 6:
				// taskmanager.cpp:2207
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 7, 0, 0, 0, 0));
				break;
			case 7:
				// taskmanager.cpp:2213
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 9, 0, 0, 0, 0));
				break;
			case 8:
				// taskmanager.cpp:2219
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xA, 0, 0, 0, 0));
				break;
			case 0xb:
				// taskmanager.cpp:2225
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xC, 0, 0, 0, 0));
				// taskmanager.cpp:2226
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xbc\xad\xb9\xf6 \xb9\xf6\xc0\xfc\xc0\xcc \xb4\xd9\xb8\xa8\xb4\xcf\xb4\xd9. \xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				break;
			case 0xe:
				// taskmanager.cpp:2232
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xE, 0, 0, 0, 0));
				break;
			case 0xd6:
				// taskmanager.cpp:2238
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xF, 0, 0, 0, 0));
				break;
			case 0xd7:
				// taskmanager.cpp:2244
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0x10, 0, 0, 0, 0));
				break;
			case 0x12:
				// taskmanager.cpp:2250
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0x15, 0, 0, 0, 0));
				break;
			case 0xc9:
				// taskmanager.cpp:2255
				GetActor("Lobby") << MsgObject(0, 0x1E, 0x14, 0, 0, 0, 0);
				// taskmanager.cpp:2256
				GetActor("Lobby") << MsgObject(0, 0x25, (intptr_t)"\xb0\xd4\xc0\xd3\xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
				break;
			case 0x10:
				// taskmanager.cpp:2271
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0x11, 0, 0, 0, 0));
				break;
			case 0x13:
				// taskmanager.cpp:2275
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0x12, 0, 0, 0, 0));
				break;
			case 0xdb:
				// taskmanager.cpp:2279
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0x13, 0, 0, 0, 0));
				break;
			case 3:
			case 0xe0:
				// taskmanager.cpp:2309
				{
					unsigned long blockedTime = packet.Decode4();
					// taskmanager.cpp:2310
					if (blockedTime == 0) {
						break;
					}
					// taskmanager.cpp:2320
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xD, 0, 0, 0, 0));
					// taskmanager.cpp:2321
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)MakeStr("%s %d", _systemmsg::CMessageManager::Instance()->GetMsg((_systemmsg::eMsgIdentifier)0x76), blockedTime), 0, 0, 0, 0));
				}
				break;
			default:
				// taskmanager.cpp:2329
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1F, 0xD, 0, 0, 0, 0));
				break;
			}
		}
		break;
	case 0x43:
		// taskmanager.cpp:2340
		PG_BOOT_TODO("taskmanager.cpp:2340 case 0x43: 595 bytes; calls MyGuid, CSharedDoc::FindUserInfoTimeGUID"); // TODO(boot)
		break;
	case 0x49:
		// taskmanager.cpp:2394
		// Another player's character, caddie, club set, ... while the
		// avatar chat task waits as the preserved task: the message goes to it.
		switch (packet.Decode1()) {
		case 1:
			// taskmanager.cpp:2400
			{
				unsigned long guid = packet.Decode4();
				// taskmanager.cpp:2402
				sCharacterInfo character;
				// taskmanager.cpp:2403
				packet.DecodeBuffer(&character, sizeof(sCharacterInfo));
				// taskmanager.cpp:2405
				if (IsLocalContent((localContentType_t)0x57)) {
					// taskmanager.cpp:2407
					RequestUccInfo(character);
				}
				// taskmanager.cpp:2413
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2414
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x49, guid, (intptr_t)&character, 0, 0, 0));
				} else {
					// taskmanager.cpp:2416
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x49, guid, (intptr_t)&character, 0, 0, 0));
				}
			}
			break;
		case 2:
			// taskmanager.cpp:2422
			{
				unsigned long guid = packet.Decode4();
				sCaddieInfo caddie;
				// taskmanager.cpp:2425
				packet.DecodeBuffer(&caddie, sizeof(sCaddieInfo));
				// taskmanager.cpp:2427
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2428
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x4A, guid, (intptr_t)&caddie, 0, 0, 0));
				} else {
					// taskmanager.cpp:2430
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x4A, guid, (intptr_t)&caddie, 0, 0, 0));
				}
			}
			break;
		case 3:
			// taskmanager.cpp:2436
			{
				unsigned long guid = packet.Decode4();
				// The club set's record (0x12 bytes, not typed yet; sUserInfoExt 0xB41).
				unsigned char clubSet[0x12];
				// taskmanager.cpp:2439
				packet.DecodeBuffer(clubSet, sizeof(clubSet));
				// taskmanager.cpp:2441
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2442
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x4B, guid, (intptr_t)clubSet, 0, 0, 0));
				} else {
					// taskmanager.cpp:2444
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x4B, guid, (intptr_t)clubSet, 0, 0, 0));
				}
			}
			break;
		case 4:
			// taskmanager.cpp:2450
			{
				unsigned long guid = packet.Decode4();
				// taskmanager.cpp:2451
				unsigned long value = packet.Decode4();
				// taskmanager.cpp:2453
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2454
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x4C, guid, value, 0, 0, 0));
				} else {
					// taskmanager.cpp:2456
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x4C, guid, value, 0, 0, 0));
				}
			}
			break;
		case 5:
			// taskmanager.cpp:2462
			{
				unsigned long guid = packet.Decode4();
				sMascotInfo mascot;
				// taskmanager.cpp:2465
				packet.DecodeBuffer(&mascot, sizeof(sMascotInfo));
				// taskmanager.cpp:2467
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2468
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x4E, guid, (intptr_t)&mascot, 0, 0, 0));
				} else {
					// taskmanager.cpp:2470
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x4E, guid, (intptr_t)&mascot, 0, 0, 0));
				}
			}
			break;
		case 6:
			// taskmanager.cpp:2475
			{
				unsigned long guid = packet.Decode4();
				// taskmanager.cpp:2476
				unsigned long value = packet.Decode4();
				// taskmanager.cpp:2478
				float rate = 0;
				packet.DecodeBuffer(&rate, sizeof(rate));
				// taskmanager.cpp:2479
				if (((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false) && guid != MyGuid(false)) {
					// taskmanager.cpp:2480
					AfxGetPreservedTask()->SendMsgToMainActor(MsgObject(0, 0x24E, guid, value, (int)rate, 0, 0));
				} else {
					// taskmanager.cpp:2482
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x24E, guid, value, (int)rate, 0, 0));
				}
			}
			break;
		}
		break;
	case 0x4a:
	case 0x7c:
		// taskmanager.cpp:2494
		// Back to the lobby (from a room, or from the avatar chat).
		{
			// taskmanager.cpp:2495
			if (packet.Decode2() != 0xFFFF) {
				// taskmanager.cpp:2497
				AfxPostMsg(0, "AvatarChat", 8, 0, 0, 0, 0);
				// taskmanager.cpp:2498
				AfxPostMsg(0, "AvatarChat", 2, 0, 0, 0, 0);
				break;
			}
			// taskmanager.cpp:2503
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
			// taskmanager.cpp:2505
			CSharedDoc::Instance()->m_mySlot = 0xFFFF;
			// taskmanager.cpp:2506
			CSharedDoc::Instance()->m_userInfo.galleryGuid = 0xFFFFFFFF;
			// taskmanager.cpp:2508
			CProjectG::Instance()->FromGolfToLobbySettings();
			// taskmanager.cpp:2510
			if (!((__rtti_obj = AfxGetTask()) != 0 ? ((IObject *)__rtti_obj)->IsExactKindOf(&CLobbyTask::m_RTTI) : false)) {
				// taskmanager.cpp:2511
				CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", 0);
			}
			// taskmanager.cpp:2514
			if (CSharedDoc::Instance()->m_lobbyStateFlag) {
				// taskmanager.cpp:2516
				AfxPostMsg(0, "Lobby", 2, 0, 0, 0, 0);
				// taskmanager.cpp:2517
				AfxPostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
			} else {
				// taskmanager.cpp:2521
				AfxPostMsg(0, "Lobby", 2, 0, 0, 0, 0);
				// taskmanager.cpp:2522
				AfxPostMsg(0, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
			}
			// taskmanager.cpp:2527
			if (WSingleton<CSceneManager>::IsInstantiated()) {
				// taskmanager.cpp:2528
				CSceneManager::Instance()->SetWeather(0, false);
			}
			// The messenger learns the player's new place.
			// taskmanager.cpp:2530
			if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && CMessengerInfo::Instance()->m_serverListRequest == 4) {
				// taskmanager.cpp:2532
				CSharedDoc::Instance()->m_roomInfo.m_roomNumber = 0xFFFF;
				// taskmanager.cpp:2533
				CSharedDoc::Instance()->m_roomInfo.m_gameType = 0xFF;
				// taskmanager.cpp:2535
				sUserPosition position;
				// taskmanager.cpp:2536
				position.m_userValue = CSharedDoc::Instance()->m_value84;
				// taskmanager.cpp:2537
				position.m_channelId = CSharedDoc::Instance()->m_currentChannel.id;
				// taskmanager.cpp:2538
				wsprintfA(position.m_channelName, "%s", CSharedDoc::Instance()->m_currentChannel.name);
				position.m_roomNumber = 0xFFFF;
				position.m_value2 = 0xFFFFFFFF;
				// taskmanager.cpp:2550
				WSendPacket send(0x23);
				// taskmanager.cpp:2554
				send.EncodeBuffer(&position, sizeof(sUserPosition));
				// taskmanager.cpp:2555
				send.Send(SEND_TO_MSN);
				// taskmanager.cpp:2557
				if (CMessengerInfo::Instance()->m_myStatus != MSN::STATUS_BUSY) {
					// taskmanager.cpp:2558
					CMessengerInfo::Instance()->SetMyStatus(MSN::STATUS_ONLINE);
				}
			}
			// taskmanager.cpp:2562
			CIntrusion::Instance()->FinishGame();
		}
		break;
	case 0x45:
		// taskmanager.cpp:2569
		{
			unsigned char count = packet.Decode1();
			// taskmanager.cpp:2570
			unsigned char kind = packet.Decode1();
			// taskmanager.cpp:2571
			unsigned short lobby = packet.Decode2();
			// taskmanager.cpp:2573
			sRoomInfo room;
			// taskmanager.cpp:2576
			if (lobby != 0xFFFF)
				break;
			// taskmanager.cpp:2580
			std::list<sRoomInfo> &roomList = CSharedDoc::Instance()->m_roomList;
			switch (kind) {
			case 0:
				// The whole list again.
				// taskmanager.cpp:2584
				roomList.clear();
				// taskmanager.cpp:2586
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 6, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2588
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2590
					if (IsLocalContent((localContentType_t)0x5E)) {
						sRoomInfoLegacy legacy;
						// taskmanager.cpp:2592
						memset(&room, 0, sizeof(sRoomInfo));
						// taskmanager.cpp:2593
						packet.DecodeBuffer(&legacy, sizeof(sRoomInfoLegacy));
						// taskmanager.cpp:2596
						room.m_isPublic = legacy.m_isPublic;
						// taskmanager.cpp:2597
						room.m_intrusionAccess = legacy.m_intrusionAccess;
						// taskmanager.cpp:2598
						room.m_state = legacy.m_state;
						// taskmanager.cpp:2601
						room.m_userCount = legacy.m_userCount;
						// taskmanager.cpp:2602
						room.m_userLimit = legacy.m_userLimit;
						room.m_roomNumber = legacy.m_roomNumber;
						// taskmanager.cpp:2607
						room.m_course = legacy.m_course;
						// taskmanager.cpp:2608
						room.m_gameType = legacy.m_gameType;
						// taskmanager.cpp:2609
						room.m_holeCount = legacy.m_holeCount;
						// taskmanager.cpp:2614
						strcpy(room.m_title, legacy.m_title);
						// taskmanager.cpp:2616
						memcpy(room.m_guildUID, legacy.m_guildUID, 0x4A);
					// taskmanager.cpp:2619
					} else {
						// taskmanager.cpp:2621
						packet.DecodeBuffer(&room, sizeof(sRoomInfo));
					}
					// taskmanager.cpp:2625
					CSharedDoc::Instance()->m_chatManager.FilteringHack(room.m_title, false);
					// taskmanager.cpp:2627
					if (IsLocalContent((localContentType_t)0x2E))
						// taskmanager.cpp:2628
						CIntrusion::Instance()->OnAccessRoom(room);
					// taskmanager.cpp:2630
					roomList.push_back(room);
					// taskmanager.cpp:2632
					std::list<sRoomInfo>::iterator last = --roomList.end();
					// taskmanager.cpp:2633
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 4, (intptr_t)&*last, 0, 0, 0, 0));
					// taskmanager.cpp:2635
					if (room.m_roomNumber == CSharedDoc::Instance()->m_roomInfo.m_roomNumber) {
						// taskmanager.cpp:2637
						CSharedDoc::Instance()->m_roomInfo = room;
						// taskmanager.cpp:2638
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x10, 0, 0, 0, 0, 0));
					}
				}
				// taskmanager.cpp:2641
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xD, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2643
				break;
			case 1:
				// Rooms added or changed.
				// taskmanager.cpp:2647
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2649
					if (IsLocalContent((localContentType_t)0x5E)) {
						sRoomInfoLegacy legacy;
						// taskmanager.cpp:2651
						memset(&room, 0, sizeof(sRoomInfo));
						// taskmanager.cpp:2652
						packet.DecodeBuffer(&legacy, sizeof(sRoomInfoLegacy));
						// taskmanager.cpp:2655
						room.m_isPublic = legacy.m_isPublic;
						// taskmanager.cpp:2656
						room.m_intrusionAccess = legacy.m_intrusionAccess;
						// taskmanager.cpp:2657
						room.m_state = legacy.m_state;
						// taskmanager.cpp:2660
						room.m_userCount = legacy.m_userCount;
						// taskmanager.cpp:2661
						room.m_userLimit = legacy.m_userLimit;
						room.m_roomNumber = legacy.m_roomNumber;
						// taskmanager.cpp:2666
						room.m_course = legacy.m_course;
						// taskmanager.cpp:2667
						room.m_gameType = legacy.m_gameType;
						// taskmanager.cpp:2668
						room.m_holeCount = legacy.m_holeCount;
						// taskmanager.cpp:2672
						room.m_trophy = legacy.m_trophy;
						// taskmanager.cpp:2673
						strcpy(room.m_title, legacy.m_title);
						// taskmanager.cpp:2675
						memcpy(room.m_guildUID, legacy.m_guildUID, 0x4A);
					// taskmanager.cpp:2678
					} else {
						// taskmanager.cpp:2680
						packet.DecodeBuffer(&room, sizeof(sRoomInfo));
					}
					// taskmanager.cpp:2684
					CSharedDoc::Instance()->m_chatManager.FilteringHack(room.m_title, false);
					// taskmanager.cpp:2686
					if (IsLocalContent((localContentType_t)0x2E))
						// taskmanager.cpp:2687
						CIntrusion::Instance()->OnAccessRoom(room);
					// taskmanager.cpp:2690
					std::list<sRoomInfo>::iterator it = std::find(roomList.begin(), roomList.end(), room.m_roomNumber);
					// taskmanager.cpp:2692
					if (it == roomList.end()) {
						// taskmanager.cpp:2694
						roomList.push_back(room);
						// taskmanager.cpp:2695
						std::list<sRoomInfo>::iterator last = --roomList.end();
						// taskmanager.cpp:2696
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 4, (intptr_t)&*last, 0, 0, 0, 0));
					// taskmanager.cpp:2698
					} else {
						// taskmanager.cpp:2700
						*it = room;
					}
					// taskmanager.cpp:2703
					if (room.m_roomNumber == CSharedDoc::Instance()->m_roomInfo.m_roomNumber) {
						// taskmanager.cpp:2705
						CSharedDoc::Instance()->m_roomInfo = room;
						// taskmanager.cpp:2706
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x10, 0, 0, 0, 0, 0));
					}
				}
				// taskmanager.cpp:2710
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xD, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2712
				break;
			case 2:
				// Rooms removed.
				// taskmanager.cpp:2716
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2718
					packet.DecodeBuffer(&room, sizeof(sRoomInfo));
					// taskmanager.cpp:2721
					CSharedDoc::Instance()->m_chatManager.FilteringHack(room.m_title, false);
					// taskmanager.cpp:2723
					if (IsLocalContent((localContentType_t)0x2E))
						// taskmanager.cpp:2724
						CIntrusion::Instance()->OnAccessRoom(room);
					// taskmanager.cpp:2727
					std::list<sRoomInfo>::iterator it = std::find(roomList.begin(), roomList.end(), room.m_roomNumber);
					// taskmanager.cpp:2728
					if (it != roomList.end()) {
						// taskmanager.cpp:2730
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 5, (intptr_t)&*it, sizeof(sRoomInfo), 0, 0, 0));
						// taskmanager.cpp:2731
						roomList.erase(it);
					}
				}
				break;
			case 3:
				// Rooms updated in place.
				// taskmanager.cpp:2739
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2741
					packet.DecodeBuffer(&room, sizeof(sRoomInfo));
					// taskmanager.cpp:2744
					CSharedDoc::Instance()->m_chatManager.FilteringHack(room.m_title, false);
					// taskmanager.cpp:2746
					if (IsLocalContent((localContentType_t)0x2E))
						// taskmanager.cpp:2747
						CIntrusion::Instance()->OnAccessRoom(room);
					// taskmanager.cpp:2765
					std::list<sRoomInfo>::iterator it = std::find(roomList.begin(), roomList.end(), room.m_roomNumber);
					// taskmanager.cpp:2766
					if (it != roomList.end())
						// taskmanager.cpp:2768
						*it = room;
					// taskmanager.cpp:2771
					if (room.m_roomNumber == CSharedDoc::Instance()->m_roomInfo.m_roomNumber) {
						// taskmanager.cpp:2773
						CSharedDoc::Instance()->m_roomInfo = room;
						// taskmanager.cpp:2774
						AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x10, 0, 0, 0, 0, 0));
					}
				}
				// taskmanager.cpp:2777
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xD, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2783
				break;
			}
		}
		break;
	case 0x44:
		// taskmanager.cpp:2789
		{
			sBriefUserInfo info;
			unsigned char kind = packet.Decode1();
			// taskmanager.cpp:2790
			unsigned char count = packet.Decode1();
			// taskmanager.cpp:2793
			switch (kind) {
			case 4:
				// The whole lobby again: drop the old list, then add as case 5.
				// taskmanager.cpp:2796
				CSharedDoc::Instance()->m_userMap.clear();
				// taskmanager.cpp:2797
				GetActor("Lobby") << MsgObject(0, 0xB, 0, 0, 0, 0, 0);
			case 5:
				// taskmanager.cpp:2801
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2803
					packet.DecodeBuffer(&info, sizeof(sBriefUserInfo));
					// taskmanager.cpp:2805
					if (CSharedDoc::Instance()->m_userMap.find(info.uid) == CSharedDoc::Instance()->m_userMap.end()) {
						// taskmanager.cpp:2807
						CSharedDoc::Instance()->m_userMap.insert(std::map<unsigned long, sBriefUserInfo>::value_type(info.uid, info));
						// taskmanager.cpp:2808
						std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(info.uid);
						// taskmanager.cpp:2809
						GetActor("Lobby") << MsgObject(0, 0xA, (intptr_t)&it->second, 0, 0, 0, 0);
					}
				}
				break;
			case 1:
				// taskmanager.cpp:2818
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2820
					packet.DecodeBuffer(&info, sizeof(sBriefUserInfo));
					// taskmanager.cpp:2822
					std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(info.uid);
					// taskmanager.cpp:2823
					if (it == CSharedDoc::Instance()->m_userMap.end()) {
						// taskmanager.cpp:2825
						CSharedDoc::Instance()->m_userMap.insert(std::map<unsigned long, sBriefUserInfo>::value_type(info.uid, info));
						// taskmanager.cpp:2826
						it = CSharedDoc::Instance()->m_userMap.find(info.uid);
						// taskmanager.cpp:2827
						GetActor("Lobby") << MsgObject(0, 0xA, (intptr_t)&it->second, 0, 0, 0, 0);
					} else {
						// taskmanager.cpp:2831
						it->second = info;
						// taskmanager.cpp:2833
						if (info.identityBits.administrator) {
							// taskmanager.cpp:2836
							if (info.invisible) {
								// taskmanager.cpp:2839
								if (info.lobbyFlags & 4) {
									// taskmanager.cpp:2841
									GetActor("Lobby") << MsgObject(0, 0xA, (intptr_t)&it->second, 0, 0, 0, 0);
								}
							}
						}
					}
				}
				break;
			case 2:
				// taskmanager.cpp:2852
				for (int i = 0; i < count; i++) {
					// taskmanager.cpp:2854
					packet.DecodeBuffer(&info, sizeof(sBriefUserInfo));
					// taskmanager.cpp:2856
					std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(info.uid);
					// taskmanager.cpp:2857
					if (it != CSharedDoc::Instance()->m_userMap.end()) {
						// taskmanager.cpp:2859
						if (info.identityBits.administrator) {
							// taskmanager.cpp:2861
							it->second = info;
						}
						// taskmanager.cpp:2863
						GetActor("Lobby") << MsgObject(0, 0xC, (intptr_t)&it->second, sizeof(sBriefUserInfo), 0, 0, 0);
						// taskmanager.cpp:2864
						if (info.identityBits.administrator) {
							// taskmanager.cpp:2866
							if (!(info.lobbyFlags & 4)) {
								// taskmanager.cpp:2868
								CSharedDoc::Instance()->m_userMap.erase(it);
							}
						} else {
							// taskmanager.cpp:2873
							CSharedDoc::Instance()->m_userMap.erase(it);
						}
					}
				}
				break;
			case 3:
				// taskmanager.cpp:2882
				{
					packet.DecodeBuffer(&info, sizeof(sBriefUserInfo));
					// taskmanager.cpp:2884
					std::map<unsigned long, sBriefUserInfo>::iterator it = CSharedDoc::Instance()->m_userMap.find(info.uid);
					// taskmanager.cpp:2885
					if (it == CSharedDoc::Instance()->m_userMap.end()) {
						break;
					}
					// taskmanager.cpp:2887
					it->second = info;
					// taskmanager.cpp:2888
					CSharedDoc *pDoc = CSharedDoc::Instance();
					if (info.roomNumber != pDoc->m_mySlot || info.roomNumber == 0xFFFF) {
						break;
					}
					// A player of the local player's room: refresh his room entry.
					// taskmanager.cpp:2890
					for (std::list<sRoomPlayer>::iterator player = pDoc->m_roomPlayerList.begin(); player != pDoc->m_roomPlayerList.end(); ++player) {
						// taskmanager.cpp:2892
						if (player->m_uid == info.uid) {
							// taskmanager.cpp:2894
							player->m_identity = info.identity;
							// taskmanager.cpp:2895
							player->m_titleTypeId = info.titleTypeId;
							break;
						}
					}
				}
				break;
			}
		}
		break;
	case 0x4b:
		// taskmanager.cpp:2909
		{
			if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_LOGIN)) {
				// taskmanager.cpp:2914
				WSendPacket send(3);
				send.Encode4(CSharedDoc::Instance()->m_value84);
				send.Send(SEND_TO_LOGIN);
			}
			// taskmanager.cpp:2921
			unsigned char count = packet.Decode1();
			CSharedDoc::Instance()->m_channelList.clear();
			for (int i = 0; i < count; i++) {
				sChannelInfo channel;
				memset(&channel, 0, sizeof(channel));
				packet.DecodeBuffer(&channel, sizeof(channel));
				CSharedDoc::Instance()->m_channelList.push_back(channel);
			}
			// taskmanager.cpp:2935
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1B, 0, 0, 0, 0, 0));
		}
		break;
	case 0x4c:
		// taskmanager.cpp:2942
		{
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
			// taskmanager.cpp:2944
			unsigned char result = packet.Decode1();
			// taskmanager.cpp:2945
			switch (result) {
			case 1:
				// taskmanager.cpp:2949
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1C, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2950
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x20, 0, 0, 0, 0, 0));
				// taskmanager.cpp:2951
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 3, (intptr_t)MakeStr("[%s]\xbf\xa1 \xc0\xd4\xc0\xe5\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", CSharedDoc::Instance()->m_currentChannel.name), 0xFFFF7878, 0, 0, 0));
				// taskmanager.cpp:2953
				if (CSharedDoc::Instance()->m_currentChannel.flags & 0x4000) {
					// taskmanager.cpp:2954
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 3, (intptr_t)"[EVENT]\xc6\xce\xb9\xe8\xc6\xb2 5\xb0\xe6\xb1\xe2\xb8\xa6 \xc7\xcf\xbd\xc7 \xb6\xa7\xb8\xb6\xb4\xd9 1000\xc6\xce\xc0\xbb \xb5\xe5\xb8\xb3\xb4\xcf\xb4\xd9.", 0xFFFF7878, 0, 0, 0));
				}
				// taskmanager.cpp:2956
				if (IsLocalContent((localContentType_t)0x28)) {
					// taskmanager.cpp:2957
					CMatchHistory::Instance()->LoginRequest(MyId());
				}
				break;
			case 2:
				// taskmanager.cpp:2963
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x22, (intptr_t)"\xc3\xa4\xb3\xce\xc0\xc7 \xc1\xa4\xbf\xf8\xbc\xf6\xb0\xa1 \xc3\xca\xb0\xfa\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:2964
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x39, 0, 0, 0, 0, 0));
				break;
			case 3:
				// taskmanager.cpp:2970
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x22, (intptr_t)"\xc3\xa4\xb3\xce\xc0\xbb \xc3\xa3\xc0\xbb\xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:2971
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x39, 0, 0, 0, 0, 0));
				break;
			case 4:
				// taskmanager.cpp:2977
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x22, (intptr_t)"\xc3\xa4\xb3\xce \xc1\xa4\xba\xb8\xb0\xa1 \xc0\xdf\xb8\xf8 \xc0\xfc\xb4\xde\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:2978
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x39, 0, 0, 0, 0, 0));
				break;
			}
		}
		break;
	case 0x4d:
		// taskmanager.cpp:2992
		CSharedDoc::Instance()->ClearCurrentChannel(false);
		// taskmanager.cpp:2994
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		// taskmanager.cpp:2995
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEC, 0, 0, 0, 0, 0));
		// taskmanager.cpp:2996
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x3A, 0, 0, 0, 0, 0));
		// taskmanager.cpp:2998
		CSharedDoc::Instance()->m_roomList.clear();
		// taskmanager.cpp:3000
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 6, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3001
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEB, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3003
		COneLineBoard::Instance()->Clear();
		break;
	case 0xf3:
		// taskmanager.cpp:3011
		CSharedDoc::Instance()->m_lobbyStateFlag = true;
		// taskmanager.cpp:3012
		CSharedDoc::Instance()->m_lobbyStateFlag2 = false;
		// taskmanager.cpp:3014
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3016
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEB, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3017
		if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI))) {
			// taskmanager.cpp:3018
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", 0);
			// taskmanager.cpp:3022
			AfxPostMsg(0, "Lobby", 0, (intptr_t)"ROOMLIST", 0, 0, 0);
		} else {
			// taskmanager.cpp:3030
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0, (intptr_t)"ROOMLIST", 0, 0, 0, 0));
		}
		break;
	case 0xf4:
		// taskmanager.cpp:3039
		CSharedDoc::Instance()->m_lobbyStateFlag = false;
		// taskmanager.cpp:3040
		CSharedDoc::Instance()->m_lobbyStateFlag2 = false;
		// taskmanager.cpp:3042
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3043
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEB, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3044
		if (!((__rtti_obj = AfxGetTask()) != 0 && ((IObject *)__rtti_obj)->IsKindOf(&CLobbyTask::m_RTTI))) {
			// taskmanager.cpp:3045
			CTaskManager::Instance()->ChangeTask("CLobbyTask", "CGolfDoc", 0);
		}
		// taskmanager.cpp:3047
		AfxPostMsg(0, "Lobby", 0, (intptr_t)"TOPPAGE", 0, 0, 0);
		break;
	case 0xf5:
		// taskmanager.cpp:3056
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
		// taskmanager.cpp:3058
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xEB, 0, 0, 0, 0, 0));
		break;
	case 0x106:
		// taskmanager.cpp:3064
		PG_BOOT_TODO("taskmanager.cpp:3064 case 0x106: 572 bytes; calls WNetworkSystem::IsConnected"); // TODO(boot)
		break;
	case 0x4e:
		// taskmanager.cpp:3124
		{
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));
			// taskmanager.cpp:3126
			unsigned long result = packet.Decode4();
			// taskmanager.cpp:3128
			switch (result) {
			case 0:
				// taskmanager.cpp:3132
				{
					std::string nickname;
					// taskmanager.cpp:3135
					packet.DecodeStr(nickname);
					// taskmanager.cpp:3136
					__int64 cookie = packet.Decode8();
					// taskmanager.cpp:3138
					if (nickname.length() >= 4 && nickname.length() <= 16) {
						// taskmanager.cpp:3139
						strcpy(CSharedDoc::Instance()->m_userInfo.nickname, nickname.c_str());
					}
					// taskmanager.cpp:3146
					if (!IsLocalContent((localContentType_t)0x82)) {
						// taskmanager.cpp:3159
						if (!IsPcBang()) {
							// taskmanager.cpp:3161
							// The nickname-change ticket (0x1A000003) pays for the change; without one, cookies do.
							std::list<sItemInfo>::iterator it = std::find(CSharedDoc::Instance()->m_itemInfoList.begin(), CSharedDoc::Instance()->m_itemInfoList.end(), 0x1A000003);
							// taskmanager.cpp:3163
							if (it == CSharedDoc::Instance()->m_itemInfoList.end()) {
								// taskmanager.cpp:3166
								CSharedDoc::Instance()->m_myBonusCash = cookie;
								// taskmanager.cpp:3167
								CSharedDoc::Instance()->RefreshAllCookie();
							}
							// taskmanager.cpp:3172
							else {
								// taskmanager.cpp:3174
								it->m_count--;
								// taskmanager.cpp:3175
								if (it->m_count <= 0) {
									// taskmanager.cpp:3176
									CSharedDoc::Instance()->m_itemInfoList.erase(it);
								}
							}
						}
					}
					// taskmanager.cpp:3185
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xb4\xeb\xc8\xad\xb8\xed\xc0\xcc \xba\xaf\xb0\xe6 \xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9. \xc1\xf6\xb1\xdd\xba\xce\xc5\xcd 1\xc1\xd6\xc0\xcf \xc8\xc4 \xc0\xe7 \xba\xaf\xb0\xe6\xc0\xcc \xb0\xa1\xb4\xc9\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
					// taskmanager.cpp:3187
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0xE7, (intptr_t)nickname.c_str(), 0, 0, 0, 0));
					// taskmanager.cpp:3190
					if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN) && CMessengerInfo::Instance()->m_serverListRequest == 4) {
						// taskmanager.cpp:3192
						WSendPacket send(0x22);
						// taskmanager.cpp:3193
						send.EncodeStr(nickname);
						// taskmanager.cpp:3194
						send.Send(SEND_TO_MSN);
						// taskmanager.cpp:3195
					}
					// taskmanager.cpp:3196
				}
				// taskmanager.cpp:3197
				break;
			case 2:
				// taskmanager.cpp:3200
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xc0\xcc\xb9\xcc \xbb\xe7\xbf\xeb\xb5\xc7\xb0\xed \xc0\xd6\xb4\xc2 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3202
				break;
			case 3:
				// taskmanager.cpp:3205
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\xb4\xeb\xc8\xad\xb8\xed\xc0\xba \\c0xffff0000\\c\xbf\xb5\xb9\xae 4 - 16 \xb1\xdb\xc0\xda\\c0xff000000\\c \xb6\xc7\xb4\xc2 \\c0xffff0000\\c\xc7\xd1\xb1\xdb 2 - 8\xb1\xdb\xc0\xda\\c0xff000000\\c\xc0\xc7 \xb1\xe6\xc0\xcc\xb8\xa6 \xb0\xa1\xc1\xae\xbe\xdf \xc7\xcf\xb8\xe7, \\c0xffff0000\\c\xb0\xf8\xb9\xe9\xb9\xae\xc0\xda\\c0xff000000\\c\xb8\xa6 \xc6\xf7\xc7\xd4\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3207
				break;
			case 4:
				// taskmanager.cpp:3210
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xc4\xed\xc5\xb0\\c0xff000000\\c\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.\n\xbb\xf3\xc1\xa1\xbf\xa1\xbc\xad \xc4\xed\xc5\xb0\xb8\xa6 \xc3\xe6\xc0\xfc\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0));
				// taskmanager.cpp:3212
				break;
			case 5:
				// taskmanager.cpp:3216
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xbb\xe7\xbf\xeb\xc0\xcc \xb1\xdd\xc1\xf6\xb5\xc8 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3218
				break;
			case 1:
			case 6:
				// taskmanager.cpp:3222
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xba\xaf\xb0\xe6 \xbf\xe4\xc3\xbb\xc0\xcc \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3224
				break;
			case 7:
				// taskmanager.cpp:3227
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xb4\xeb\xc8\xad\xb8\xed \xba\xaf\xb0\xe6\xc0\xcc \xc0\xcf\xbd\xc3\xc0\xfb\xc0\xb8\xb7\xce \xc1\xdf\xb4\xdc\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3229
				break;
			case 9:
				// taskmanager.cpp:3242
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xc0\xcc\xc0\xfc \xb4\xeb\xc8\xad\xb8\xed \xba\xaf\xb0\xe6 \xc8\xc4 1\xc1\xd6\xc0\xcf\xc0\xcc \xc1\xf6\xb3\xaa\xc1\xf6 \xbe\xca\xbe\xd2\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3244
				break;
			case 11:
				// taskmanager.cpp:3249
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xb4\xeb\xc8\xad\xb8\xed \xb8\xed\xc2\xfb\xc0\xbb \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				// taskmanager.cpp:3251
				break;
			default:
				// taskmanager.cpp:3255
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)"\\c0xffff0000\\c\xc0\xcc\xb9\xcc \xbb\xe7\xbf\xeb\xb5\xc7\xb0\xed \xc0\xd6\xb4\xc2 \xb4\xeb\xc8\xad\xb8\xed\xc0\xd4\xb4\xcf\xb4\xd9.", 0, 0, 0, 0));
				break;
			}
			// taskmanager.cpp:3260
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x40, result, 0, 0, 0, 0));
		}
		break;
	case 0x4f:
		// taskmanager.cpp:3266
		PG_BOOT_TODO("taskmanager.cpp:3266 case 0x4f: 155 bytes"); // TODO(boot)
		break;
	case 0x6e:
		// taskmanager.cpp:3290
		{
			sCharacterInfo character;
			// taskmanager.cpp:3292
			CPartTidList tidList;
			// taskmanager.cpp:3295
			int total = packet.Decode2();
			// taskmanager.cpp:3296
			int count = packet.Decode2();
			// taskmanager.cpp:3298
			for (int i = 0; i < count; i++) {
				// taskmanager.cpp:3300
				packet.DecodeBuffer(&character, sizeof(sCharacterInfo));
				// taskmanager.cpp:3305
				tidList.SetTids(&character, CSharedDoc::Instance()->m_myLevel);
				// taskmanager.cpp:3307
				CSharedDoc::Instance()->AddChar(&character);
			}
			// taskmanager.cpp:3309
			if (total == count) {
				// taskmanager.cpp:3311
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x2C, 1, 0, 0, 0, 0));
			}
		}
		break;
	case 0x6f:
		// taskmanager.cpp:3324
		{
			int total = packet.Decode2();
			// taskmanager.cpp:3325
			int count = packet.Decode2();
			// taskmanager.cpp:3327
			for (int i = 0; i < count; i++) {
				sCaddieInfo caddie;
				// taskmanager.cpp:3329
				packet.DecodeBuffer(&caddie, sizeof(sCaddieInfo));
				// taskmanager.cpp:3330
				CSharedDoc::Instance()->AddCaddie(&caddie);
				// taskmanager.cpp:3331
				if (!caddie.checkEquip) {
					continue;
				}
				// taskmanager.cpp:3333
				CSharedDoc *pDoc = CSharedDoc::Instance();
				if (pDoc->m_myCaddieId) {
					// taskmanager.cpp:3336
					std::map<unsigned int, sCaddieInfo>::iterator it = pDoc->m_caddieMap.find(pDoc->m_myCaddieId);
					// taskmanager.cpp:3339
					if (it == pDoc->m_caddieMap.end() && pDoc->m_caddieMap.size() > 0) {
						// taskmanager.cpp:3345
						// The equipped caddie is gone: equip the first one.
						pDoc->m_myCaddieId = pDoc->m_caddieMap.begin()->first;
					}
				}
				// taskmanager.cpp:3355
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x68, 1, 0, 0, 0, 0));
			}
			// taskmanager.cpp:3359
			if (total == count) {
				// taskmanager.cpp:3361
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x2C, 2, 0, 0, 0, 0));
			}
		}
		break;
	case 0xdf:
		// taskmanager.cpp:3372
		{
			unsigned char count = packet.Decode1();
			// taskmanager.cpp:3374
			for (int i = 0; i < count; i++) {
				sMascotInfo mascot;
				// taskmanager.cpp:3376
				packet.DecodeBuffer(&mascot, sizeof(sMascotInfo));
				// taskmanager.cpp:3378
				CSharedDoc::Instance()->AddMascot(&mascot);
				// taskmanager.cpp:3379
				if (!mascot.pcBang) {
					continue;
				}
				// A PC-bang mascot shows the message of the options.
				// taskmanager.cpp:3382
				strcpy(mascot.message, COption::Instance()->GetOption()->m_pcBangMascotMsg);
				// taskmanager.cpp:3383
				CSharedDoc::Instance()->AddMascot(&mascot);
				// taskmanager.cpp:3386
				WSendPacket send((enumClientPacket)0x97);
				// taskmanager.cpp:3387
				send.Encode1(2);
				// taskmanager.cpp:3388
				send.Encode4(mascot.id);
				// taskmanager.cpp:3389
				send.EncodeStr(std::string(mascot.message));
				// taskmanager.cpp:3390
				send.Send(SEND_TO_DEFAULT);
				// taskmanager.cpp:3392
				CSharedDoc *pDoc = CSharedDoc::Instance();
				if (pDoc->m_myMascotId) {
					// taskmanager.cpp:3396
					std::map<unsigned int, sMascotInfo>::iterator it = pDoc->m_myMascotMap.find(pDoc->m_myMascotId);
					// taskmanager.cpp:3398
					if (it == pDoc->m_myMascotMap.end()) {
						// taskmanager.cpp:3400
						pDoc->m_myMascotId = 0;
					}
				}
				// taskmanager.cpp:3406
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x68, 2, 0, 0, 0, 0));
			}
			// taskmanager.cpp:3410
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x2C, 0x10, 0, 0, 0, 0));
		}
		break;
	case 0x70:
		// taskmanager.cpp:3418
		{
			sUserEquip equip;
			packet.DecodeBuffer(&equip, sizeof(equip));
			CSharedDoc::Instance()->SetCurEquipInfo(&equip);
		}
		break;
	case 0x71:
		// taskmanager.cpp:3428
		{
			sItemInfo item;
			// taskmanager.cpp:3431
			int total = packet.Decode2();
			// taskmanager.cpp:3432
			int count = packet.Decode2();
			// taskmanager.cpp:3434
			for (int i = 0; i < count; i++) {
				// taskmanager.cpp:3436
				packet.DecodeBuffer(&item, sizeof(sItemInfo));
				// An item the lists already hold is not added again.
				bool isNew = true;
				// taskmanager.cpp:3441
				switch (item.m_typeId >> 26) {
				case 2:
					// taskmanager.cpp:3445
					if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(item.m_typeId)) {
						// taskmanager.cpp:3448
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemList.begin(); it != CSharedDoc::Instance()->m_myItemList.end(); ++it) {
							// taskmanager.cpp:3450
							if (it->m_typeId == item.m_typeId && strcmp(it->m_uccKey, item.m_uccKey) == 0) {
								it->m_uccSeq = item.m_uccSeq;
								// taskmanager.cpp:3453
								isNew = false;
								break;
							}
						}
					}
					// taskmanager.cpp:3461
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myPartsList.begin(); it != CSharedDoc::Instance()->m_myPartsList.end(); ++it) {
							// taskmanager.cpp:3463
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 4:
					// taskmanager.cpp:3477
					{
						for (std::map<unsigned int, sItemInfo>::iterator it = CSharedDoc::Instance()->m_myItemMap.begin(); it != CSharedDoc::Instance()->m_myItemMap.end(); ++it) {
							// taskmanager.cpp:3479
							if (it->second.m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 5:
					// taskmanager.cpp:3493
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myGearList.begin(); it != CSharedDoc::Instance()->m_myGearList.end(); ++it) {
							// taskmanager.cpp:3495
							if (it->m_serial == item.m_serial) {
								// taskmanager.cpp:3498
								isNew = false;
								break;
							}
						}
					}
					// taskmanager.cpp:3502
					if (item.m_count > 19000) {
						// taskmanager.cpp:3504
						if (item.m_count > 20000) {
							// taskmanager.cpp:3506
							CSharedDoc::Instance()->m_value5064 = 2;
						} else if (CSharedDoc::Instance()->m_value5064 != 2) {
							// taskmanager.cpp:3512
							CSharedDoc::Instance()->m_value5064 = 1;
						}
					}
					break;
				case 6:
					// taskmanager.cpp:3523
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_itemInfoList.begin(); it != CSharedDoc::Instance()->m_itemInfoList.end(); ++it) {
							// taskmanager.cpp:3525
							if (it->m_serial == item.m_serial) {
								// taskmanager.cpp:3528
								isNew = false;
								break;
							}
						}
					}
					// taskmanager.cpp:3532
					if (item.m_count > 19000) {
						// taskmanager.cpp:3534
						if (item.m_count > 20000) {
							// taskmanager.cpp:3536
							CSharedDoc::Instance()->m_value5064 = 2;
						} else if (CSharedDoc::Instance()->m_value5064 != 2) {
							// taskmanager.cpp:3542
							CSharedDoc::Instance()->m_value5064 = 1;
						}
					}
					break;
				case 8:
					// taskmanager.cpp:3553
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myExtraItemList.begin(); it != CSharedDoc::Instance()->m_myExtraItemList.end(); ++it) {
							// taskmanager.cpp:3555
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 9:
					// taskmanager.cpp:3569
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myExtraItemList2.begin(); it != CSharedDoc::Instance()->m_myExtraItemList2.end(); ++it) {
							// taskmanager.cpp:3571
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 0xe:
					// taskmanager.cpp:3585
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_mySkinList.begin(); it != CSharedDoc::Instance()->m_mySkinList.end(); ++it) {
							// taskmanager.cpp:3587
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 0x1c:
					// taskmanager.cpp:3601
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myExtraItemList3.begin(); it != CSharedDoc::Instance()->m_myExtraItemList3.end(); ++it) {
							// taskmanager.cpp:3603
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 0x1d:
					// taskmanager.cpp:3617
					{
						for (std::list<sItemInfo>::iterator it = CSharedDoc::Instance()->m_myExtraItemList4.begin(); it != CSharedDoc::Instance()->m_myExtraItemList4.end(); ++it) {
							// taskmanager.cpp:3619
							if (it->m_serial == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				case 0x12:
					// taskmanager.cpp:3630
					if (IsLocalContent((localContentType_t)0x56)) {
						// taskmanager.cpp:3634
						for (std::list<sFurniture_List>::iterator it = CSharedDoc::Instance()->m_furnitureList.begin(); it != CSharedDoc::Instance()->m_furnitureList.end(); ++it) {
							// taskmanager.cpp:3636
							if (it->m_id == item.m_serial) {
								isNew = false;
								break;
							}
						}
					}
					break;
				}

				// taskmanager.cpp:3650
				if (!isNew) {
					continue;
				}
				// taskmanager.cpp:3652
				switch (item.m_typeId >> 26) {
				case 2:
					// taskmanager.cpp:3656
					if (IsLocalContent((localContentType_t)0x57) && IsUccClothes(item.m_typeId)) {
						// taskmanager.cpp:3658
						if (item.m_uccStatus & 1) {
							// taskmanager.cpp:3660
							sUccClothes clothes;
							// taskmanager.cpp:3661
							clothes.m_serial = item.m_serial;
							// taskmanager.cpp:3662
							clothes.m_typeId = item.m_typeId;
							// taskmanager.cpp:3663
							strncpy(clothes.m_uccKey, item.m_uccKey, 9);
							// taskmanager.cpp:3664
							strncpy(clothes.m_uccName, item.m_uccName, 0x29);
							// taskmanager.cpp:3665
							clothes.m_period[0] = item.m_reserved18[0]; clothes.m_period[1] = item.m_reserved18[1]; clothes.m_period[2] = item.m_reserved18[2]; clothes.m_period[3] = item.m_reserved18[3];
							clothes.m_downloadCount = 0;
							// taskmanager.cpp:3667
							UccManager()->AddClothes(1, clothes, true, false);
						} else if (item.m_uccStatus & 2) {
							// taskmanager.cpp:3671
							NetResourceManager::Instance()->DownloadUccTempClothes(item.m_typeId, item.m_uccKey, item.m_uccSeq, false);
						}
						// taskmanager.cpp:3674
						CSharedDoc::Instance()->m_myItemList.push_back(item);
					} else {
						// taskmanager.cpp:3678
						CSharedDoc::Instance()->AddMyPartsList(item);
					}
					break;
				case 4:
					// taskmanager.cpp:3683
					CSharedDoc::Instance()->m_myItemMap.insert(std::map<unsigned int, sItemInfo>::value_type(item.m_serial, item));
					break;
				case 5:
					// taskmanager.cpp:3686
					CSharedDoc::Instance()->m_myGearList.push_back(item);
					break;
				case 6:
					// taskmanager.cpp:3689
					CSharedDoc::Instance()->m_itemInfoList.push_back(item);
					break;
				case 8:
					// taskmanager.cpp:3692
					CSharedDoc::Instance()->m_myExtraItemList.push_back(item);
					break;
				case 9:
					// taskmanager.cpp:3695
					CSharedDoc::Instance()->m_myExtraItemList2.push_back(item);
					break;
				case 0xe:
					// taskmanager.cpp:3698
					CSharedDoc::Instance()->m_mySkinList.push_back(item);
					break;
				case 0x1c:
					// taskmanager.cpp:3701
					CSharedDoc::Instance()->m_myExtraItemList3.push_back(item);
					break;
				case 0x1d:
					// taskmanager.cpp:3704
					CSharedDoc::Instance()->m_myExtraItemList4.push_back(item);
					break;
				}
			}
			// taskmanager.cpp:3713
			if (total == count) {
				// taskmanager.cpp:3715
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x2C, 4, 0, 0, 0, 0));
			}
			// taskmanager.cpp:3717
			if (IsLocalContent((localContentType_t)0x17)) {
				// taskmanager.cpp:3718
				CSharedDoc::Instance()->SetInitHaveHalloweenItem();
			}
			// taskmanager.cpp:3721
			if (IsLocalContent((localContentType_t)0x57) && AfxGetTask()->GetActor("RealMyRoom")) {
				// taskmanager.cpp:3725
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0xE1, 0, 0, 0, 0, 0);
			}
		}
		break;
	case 0x17d:
		// taskmanager.cpp:3735
		PG_BOOT_TODO("taskmanager.cpp:3735 case 0x17d: 120 bytes; calls CSharedDoc::ClearCardStack, CSharedDoc::AddCardStack"); // TODO(boot)
		break;
	case 0x12d:
		// taskmanager.cpp:3750
		PG_BOOT_TODO("taskmanager.cpp:3750 case 0x12d: 27 bytes"); // TODO(boot)
		break;
	case 0x130:
		// taskmanager.cpp:3760
		PG_BOOT_TODO("taskmanager.cpp:3760 case 0x130: 440 bytes"); // TODO(boot)
		break;
	case 0x12e:
		// taskmanager.cpp:3812
		PG_BOOT_TODO("taskmanager.cpp:3812 case 0x12e: 41 bytes"); // TODO(boot)
		break;
	case 0x12f:
		// taskmanager.cpp:3819
		PG_BOOT_TODO("taskmanager.cpp:3819 case 0x12f: 299 bytes; calls ItemManager, CItemManager::FindCard"); // TODO(boot)
		break;
	case 0x158:
		// taskmanager.cpp:4003
		PG_BOOT_TODO("taskmanager.cpp:4003 case 0x158: 1215 bytes; calls ItemManager, CItemManager::FindCard, IsLocalContent"); // TODO(boot)
		break;
	case 0x131:
		// taskmanager.cpp:4129
		PG_BOOT_TODO("taskmanager.cpp:4129 case 0x131: 227 bytes"); // TODO(boot)
		break;
	case 0x15a:
		// taskmanager.cpp:4150
		PG_BOOT_TODO("taskmanager.cpp:4150 case 0x15a: 73 bytes"); // TODO(boot)
		break;
	case 0x159:
		// taskmanager.cpp:4162
		PG_BOOT_TODO("taskmanager.cpp:4162 case 0x159: 226 bytes"); // TODO(boot)
		break;
	case 0x17f:
		// taskmanager.cpp:4184
		PG_BOOT_TODO("taskmanager.cpp:4184 case 0x17f: 222 bytes"); // TODO(boot)
		break;
	case 0x13a:
		// taskmanager.cpp:4208
		PG_BOOT_TODO("taskmanager.cpp:4208 case 0x13a: 428 bytes; calls CCardManager::GetCardPeriodSlot, CSharedDoc::GetNumAddItemSlotByMascot, CCardManager::UpdateCardBuffDlg"); // TODO(boot)
		break;
	case 0x14c:
		// taskmanager.cpp:4260
		PG_BOOT_TODO("taskmanager.cpp:4260 case 0x14c: 760 bytes; calls FrWnd::FindChildForm, FrCardBoxDlg::RecievedPacket"); // TODO(boot)
		break;
	case 0x73:
		// taskmanager.cpp:4362
		PG_BOOT_TODO("taskmanager.cpp:4362 case 0x73: 31 bytes"); // TODO(boot)
		break;
	case 0x13d:
		// taskmanager.cpp:4369
		PG_BOOT_TODO("taskmanager.cpp:4369 case 0x13d: 550 bytes; calls IsLocalContent, ChatManager::FilteringHack, CProjectG::FromGolfToLobbySettings, CTaskManager::ChangeTask, AfxPostMsg, SetActive"); // TODO(boot)
		break;
	case 0x78:
		// taskmanager.cpp:4416
		PG_BOOT_TODO("taskmanager.cpp:4416 case 0x78: 534 bytes; calls ChatManager::FilteringHack, ItemManager, CItemManager::FindCommonItem"); // TODO(boot)
		break;
	case 0x85:
		// taskmanager.cpp:4495
		PG_BOOT_TODO("taskmanager.cpp:4495 case 0x85: 102 bytes; calls CSharedDoc::AddCaddie"); // TODO(boot)
		break;
	case 0x9d:
		// taskmanager.cpp:4511
		{
			CSharedDoc::Instance()->m_gameServerList.clear();
			// taskmanager.cpp:4512
			unsigned char serverCount = packet.Decode1();
			sGameServerInfo server;
			// taskmanager.cpp:4516
			for (int i = 0; i < serverCount; i++) {
				// taskmanager.cpp:4518
				memset(&server, 0, sizeof(server));
				// taskmanager.cpp:4519
				packet.DecodeBuffer(&server, sizeof(server));
				// taskmanager.cpp:4521
				if (server.flags & 0x1000) {
					continue;
				}
				// taskmanager.cpp:4525
				CSharedDoc::Instance()->m_gameServerList.push_back(server);
			}
			// taskmanager.cpp:4529
			CSharedDoc::Instance()->m_channelList.clear();
			// taskmanager.cpp:4530
			unsigned char channelCount = packet.Decode1();
			sChannelInfo channel;
			// taskmanager.cpp:4533
			for (int j = 0; j < channelCount; j++) {
				// taskmanager.cpp:4534
				packet.DecodeBuffer(&channel, sizeof(channel));
				// taskmanager.cpp:4535
				CSharedDoc::Instance()->m_channelList.push_back(channel);
			}
			// taskmanager.cpp:4538
			if (IsLocalContent((localContentType_t)0x53) || IsLocalContent((localContentType_t)0x54) ||
				IsLocalContent((localContentType_t)0x59)) {
				// taskmanager.cpp:4540
				if (CSharedDoc::Instance()->m_nowGoWithMode == 1) {
					// taskmanager.cpp:4542
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x216, 0, 0, 0, 0, 0));
				}
				// taskmanager.cpp:4544
				else if (CSharedDoc::Instance()->m_nowGoWithMode == 2) {
					// taskmanager.cpp:4546
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x216, 0, 0, 0, 0, 0));
				}
				// taskmanager.cpp:4548
				else if (CSharedDoc::Instance()->m_nowGoWithMode == 3) {
					// taskmanager.cpp:4550
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x216, 0, 0, 0, 0, 0));
				}
				// taskmanager.cpp:4552
				else {
					// taskmanager.cpp:4554
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x19, 0, 0, 0, 0, 0));
					// taskmanager.cpp:4555
					AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1B, 0, 0, 0, 0, 0));
				}
			} else {
				// taskmanager.cpp:4560
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x19, 0, 0, 0, 0, 0));
				// taskmanager.cpp:4561
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x1B, 0, 0, 0, 0, 0));
			}
		}
		break;
	case 0xfa:
		// taskmanager.cpp:4570
		{
			CMessengerInfo::Instance()->m_serverList.clear();
			// taskmanager.cpp:4571
			MSNServerInfo server;
			unsigned char count = packet.Decode1();
			for (int i = 0; i < count; i++) {
				// taskmanager.cpp:4578
				// Only the sGameServerInfo part comes from the packet.
				memset(&server, 0, sizeof(sGameServerInfo));
				packet.DecodeBuffer(&server, sizeof(sGameServerInfo));
				CMessengerInfo::Instance()->m_serverList.push_back(server);
			}
			// taskmanager.cpp:4585
			CMessengerInfo::Instance()->SetServerListWaitingState(false);
		}
		break;
	case 0x9f:
		// taskmanager.cpp:4591
		PG_BOOT_TODO("taskmanager.cpp:4591 case 0x9f: 250 bytes"); // TODO(boot)
		break;
	case 0xa0:
		// taskmanager.cpp:4616
		PG_BOOT_TODO("taskmanager.cpp:4616 case 0xa0: 116 bytes; calls WNetworkSystem::Init, WNetworkSystem::SendMessageA"); // TODO(boot)
		break;
	case 0xab:
		// taskmanager.cpp:4629
		PG_BOOT_TODO("taskmanager.cpp:4629 case 0xab: 303 bytes"); // TODO(boot)
		break;
	case 0xc4:
		// taskmanager.cpp:4651
		PG_BOOT_TODO("taskmanager.cpp:4651 case 0xc4: 95 bytes"); // TODO(boot)
		break;
	case 0xb7:
		// taskmanager.cpp:4672
		PG_BOOT_TODO("taskmanager.cpp:4672 case 0xb7: 111 bytes"); // TODO(boot)
		break;
	case 0x82:
		// taskmanager.cpp:4684
		PG_BOOT_TODO("taskmanager.cpp:4684 case 0x82: 718 bytes; calls MyNick, COption::gIsWhisperAllowed"); // TODO(boot)
		break;
	case 0xb9:
		// taskmanager.cpp:4745
		PG_BOOT_TODO("taskmanager.cpp:4745 case 0xb9: 120 bytes; calls CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0x87:
		// taskmanager.cpp:4766
		PG_BOOT_TODO("taskmanager.cpp:4766 case 0x87: 366 bytes; calls CUserInfo::IsVisible, CUserInfo::Close, CSharedDoc::FindUserInfoTimeUID, CUserInfo::IsRecvedData, MyUID, MyGuid"); // TODO(boot)
		break;
	case 0x8b:
		// taskmanager.cpp:4834
		PG_BOOT_TODO("taskmanager.cpp:4834 case 0x8b: 37 bytes"); // TODO(boot)
		break;
	case 0x91:
		// taskmanager.cpp:4841
		PG_BOOT_TODO("taskmanager.cpp:4841 case 0x91: 404 bytes"); // TODO(boot)
		break;
	case 0x9b:
		// taskmanager.cpp:4891
		PG_BOOT_TODO("taskmanager.cpp:4891 case 0x9b: 98 bytes; calls GalleryMode, IsLocalContent, RequestUccInfo"); // TODO(boot)
		break;
	case 0xa5:
		// taskmanager.cpp:4910
		PG_BOOT_TODO("taskmanager.cpp:4910 case 0xa5: 1827 bytes; calls YIPAVIActor::6"); // TODO(boot)
		break;
	case 0xa7:
		// taskmanager.cpp:5173
		PG_BOOT_TODO("taskmanager.cpp:5173 case 0xa7: 262 bytes; calls CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0xc6:
		// taskmanager.cpp:5207
		PG_BOOT_TODO("taskmanager.cpp:5207 case 0xc6: 350 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0xa8:
		// taskmanager.cpp:5265
		PG_BOOT_TODO("taskmanager.cpp:5265 case 0xa8: 4281 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x93:
		// taskmanager.cpp:5788
		PG_BOOT_TODO("taskmanager.cpp:5788 case 0x93: 136 bytes; calls CMessengerInfo::Notify1, WNetworkSystem::IsConnected"); // TODO(boot)
		break;
	case 0xd3:
		// taskmanager.cpp:6037
		PG_BOOT_TODO("taskmanager.cpp:6037 case 0xd3: 47 bytes"); // TODO(boot)
		break;
	case 0xd4:
		// taskmanager.cpp:6048
		PG_BOOT_TODO("taskmanager.cpp:6048 case 0xd4: 435 bytes; calls FrWnd::FindChildByName, FrBongdariShopDlg::EnableExitButton, CSharedDoc::RefreshAllCookie, FrBongdariShopDlg::InitBongdariShop, FrBongdariShopDlg::SetBonusBall, FrBongdariShopDlg::SetBongdariShop"); // TODO(boot)
		break;
	case 0xf9:
		// taskmanager.cpp:6135
		PG_BOOT_TODO("taskmanager.cpp:6135 case 0xf9: 133 bytes; calls CSharedDoc::SetBsUsableTimes, CSharedDoc::SetBsUsableBonusTimes, FrWnd::FindChildByName, FrBongdariShopDlg::EnableExitButton, FrBongdariShopDlg::ShowDiscountUsableTimes, FrBongdariShopDlg::UpdateUsableTimes"); // TODO(boot)
		break;
	case 0x2d:
		// taskmanager.cpp:6325
		// taskmanager.cpp:6326
		switch (packet.Decode1()) {
		case 2:
			// taskmanager.cpp:6331
			CMessengerInfo::Instance()->Notify1("\xb8\xde\xbd\xc5\xc0\xfa\xbc\xad\xb9\xf6\xbf\xa1 \xc1\xa2\xbc\xd3\xc7\xcf\xb1\xe2 \xc0\xa7\xc7\xd1 \xb7\xce\xb1\xd7\xc0\xce\xc1\xa4\xba\xb8\xb0\xa1 \xc0\xdf\xb8\xf8\xb5\xc7\xbe\xee \xb7\xce\xb1\xd7\xc0\xce\xc7\xcf\xc1\xf6 \xb8\xf8\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.");
			// taskmanager.cpp:6332
			if (WNetworkSystem::Instance()->IsConnected(WNetworkSystem::NET_UNIT_MSN)) {
				// taskmanager.cpp:6333
				WNetworkSystem::Instance()->ForceShutDown(WNetworkSystem::NET_UNIT_MSN);
			}
			break;
		case 0:
			{
				// taskmanager.cpp:6346
				unsigned long uid = packet.Decode4();
				// taskmanager.cpp:6347
				if (uid == MyUID()) {
					// taskmanager.cpp:6353
					CMessengerInfo::Instance()->SetComplete(CONNECT_STATUS_ACCEPTED);
					// taskmanager.cpp:6357
					sUserPosition position;
					// taskmanager.cpp:6358
					position.m_userValue = CSharedDoc::Instance()->m_value84;
					// taskmanager.cpp:6360
					if (CSharedDoc::Instance()->m_currentChannel.id == 0xFF) {
						// taskmanager.cpp:6361
						position.m_channelId = 0xFF;
					} else {
						// taskmanager.cpp:6363
						position.m_channelId = CSharedDoc::Instance()->m_currentChannel.id;
					}
					// taskmanager.cpp:6365
					wsprintfA(position.m_channelName, "%s", CSharedDoc::Instance()->m_currentChannel.name);
					// taskmanager.cpp:6366
					if (CSharedDoc::Instance()->m_roomInfo.m_roomNumber == 0xFFFF) {
						// taskmanager.cpp:6367
						position.m_roomNumber = 0xFFFF;
					} else {
						// taskmanager.cpp:6369
						position.m_roomNumber = CSharedDoc::Instance()->m_roomInfo.m_roomNumber;
					}
					// taskmanager.cpp:6371
					if (CSharedDoc::Instance()->m_roomInfo.m_gameType == 0xFFFFFFFF) {
						// taskmanager.cpp:6372
						position.m_value2 = 0xFFFFFFFF;
					} else {
						// taskmanager.cpp:6374
						position.m_value2 = CSharedDoc::Instance()->m_roomInfo.m_gameType;
					}
					// taskmanager.cpp:6376
					WSendPacket send(0x23);
					// taskmanager.cpp:6377
					send.EncodeBuffer(&position, sizeof(sUserPosition));
					// taskmanager.cpp:6378
					send.Send(SEND_TO_MSN);
					// taskmanager.cpp:6381
					WSendPacket request(0x14);
					// taskmanager.cpp:6387
					request.Send(SEND_TO_MSN);
					// taskmanager.cpp:6388
					CMessengerInfo::Instance()->SetComplete(CONNECT_STATUS_POSITION_SENT);
				}
			}
			break;
		}
		break;
	case 0x31:
		// taskmanager.cpp:6397
		if (IsLocalContent((localContentType_t)0x60)) {
			// taskmanager.cpp:6399
			CSharedDoc::Instance()->m_mailList.clear();
			// taskmanager.cpp:6400
			WSendPacket send((enumClientPacket)0xE6);
			// taskmanager.cpp:6401
			send.Send(SEND_TO_DEFAULT);
		} else {
			// taskmanager.cpp:6405
			CSharedDoc::Instance()->m_giftList.clear();
			// taskmanager.cpp:6406
			WSendPacket send((enumClientPacket)0x93);
			// taskmanager.cpp:6407
			send.Send(SEND_TO_DEFAULT);
		}
		break;
	case 0x36:
		{
			// taskmanager.cpp:6414
			WSendPacket send((enumClientPacket)0xE6);
			// taskmanager.cpp:6415
			send.Send(SEND_TO_DEFAULT);
		}
		break;
	case 0x38:
		{
			// taskmanager.cpp:6422
			int count = packet.Decode4();
			// taskmanager.cpp:6424
			for (int i = 0; i < count; i++) {
				// taskmanager.cpp:6426
				GUILD_USER_MSN_LIST member;
				// taskmanager.cpp:6427
				packet.DecodeBuffer(&member, sizeof(GUILD_USER_MSN_LIST));
				// taskmanager.cpp:6429
				sFriend buddy;
				// taskmanager.cpp:6430
				buddy.value2D = member.value4;
				// taskmanager.cpp:6431
				buddy.uid = member.uid;
				// taskmanager.cpp:6432
				buddy.listFlags |= 2;
				// taskmanager.cpp:6433
				buddy.flags = (unsigned char)((buddy.flags & ~1) | (member.sex & 1));
				// taskmanager.cpp:6434
				strcpy(buddy.nick, member.nick);
				// taskmanager.cpp:6435
				buddy.flags = (unsigned char)((buddy.flags & ~2) | ((member.online & 1) << 1));
				// taskmanager.cpp:6437
				BuddyMGR->AddBuddy(&buddy);
			}
		}
		break;
	case 0x39:
		{
			// taskmanager.cpp:6444
			GUILD_USER_MSN_LIST member;
			// taskmanager.cpp:6445
			packet.DecodeBuffer(&member, sizeof(GUILD_USER_MSN_LIST));
			// taskmanager.cpp:6447
			sFriend buddy;
			// taskmanager.cpp:6451
			buddy.value2D = member.value4;
			buddy.uid = member.uid;
			buddy.listFlags |= 2;
			buddy.flags = (unsigned char)((buddy.flags & ~1) | (member.sex & 1));
			// taskmanager.cpp:6452
			strcpy(buddy.nick, member.nick);
			// taskmanager.cpp:6453
			buddy.flags = (unsigned char)((buddy.flags & ~2) | ((member.online & 1) << 1));
			// taskmanager.cpp:6455
			BuddyMGR->AddBuddy(&buddy);
			// taskmanager.cpp:6459
			IActor *pGuildActor = GetActor(s_guildActorName);
			// taskmanager.cpp:6460
			if (pGuildActor) {
				// taskmanager.cpp:6463
				std::map<unsigned long, sBriefUserInfo>::iterator it;
				if (CSharedDoc::Instance()->FindUserInfoUID(member.uid, it) == 1) {
					// taskmanager.cpp:6465
					sBriefUserInfo &userInfo = it->second;
					// taskmanager.cpp:6469
					GUILD_INFO *pInfo = 0;
					pGuildActor << MsgObject(0, 0x263, 0x21, (intptr_t)&pInfo, 0, 0, 0);
					// taskmanager.cpp:6470
					strncpy(userInfo.guildMark, pInfo->emblem, 0xB);
				}
			}
			// taskmanager.cpp:6475
			AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)MakeStr("%s ���� ��忡 �����Ͽ����ϴ�.", member.nick), 0, 0, 0, 0));
		}
		break;
	case 0x3a:
		{
			// taskmanager.cpp:6481
			unsigned long uid = packet.Decode4();
			// taskmanager.cpp:6487
			BuddyMGR->DeleteBuddy(uid);
			// taskmanager.cpp:6491
			std::map<unsigned long, sBriefUserInfo>::iterator it;
			if (CSharedDoc::Instance()->FindUserInfoUID(uid, it) == 1) {
				// taskmanager.cpp:6493
				sBriefUserInfo &userInfo = it->second;
				// taskmanager.cpp:6494
				strcpy(userInfo.guildMark, "guildmark");
				// taskmanager.cpp:6495
				AfxGetTask()->SendMsgToMainActor(MsgObject(0, 0x23, (intptr_t)MakeStr("%s ���� ��带 Ż���Ͽ����ϴ�.", userInfo.nick), 0, 0, 0, 0));
			}
		}
		break;
	case 0x3b:
		// taskmanager.cpp:6505
		{
			GUILD_INFO guildInfo;
			// taskmanager.cpp:6506
			packet.DecodeBuffer(&guildInfo, sizeof(GUILD_INFO));
			// taskmanager.cpp:6508
			IActor *pGuildActor = GetActor(s_guildActorName);
			// taskmanager.cpp:6509
			if (pGuildActor) {
				// taskmanager.cpp:6514
				pGuildActor << MsgObject(0, 0x263, 0x16, (intptr_t)&guildInfo, 0, 0, 0);
			}
		}
		break;
	case 0x3c:
		{
			// taskmanager.cpp:6520
			GUILD_INFO guildInfo;
			// taskmanager.cpp:6521
			packet.DecodeBuffer(&guildInfo, sizeof(GUILD_INFO));
			// taskmanager.cpp:6522
			IActor *pGuildActor = GetActor(s_guildActorName);
			// taskmanager.cpp:6523
			if (pGuildActor) {
				// taskmanager.cpp:6528
				pGuildActor << MsgObject(0, 0x263, 0x17, (intptr_t)&guildInfo, 0, 0, 0);
			}
		}
		break;
	case 0x32:
		{
			// taskmanager.cpp:6537
			std::string nick;
			// taskmanager.cpp:6539
			unsigned long uid = packet.Decode4();
			// taskmanager.cpp:6540
			nick = packet.DecodeStr();
			// taskmanager.cpp:6542
			sFriend *pBuddy = BuddyMGR->GetBuddy(uid);
			// taskmanager.cpp:6543
			if (pBuddy) {
				// taskmanager.cpp:6545
				strcpy(pBuddy->nick, nick.c_str());
			}
		}
		// taskmanager.cpp:6547
		break;
	case 0x37:
		{
			// taskmanager.cpp:6552
			std::string msg("");
			// taskmanager.cpp:6553
			std::string unknownStr = packet.DecodeStr(); // decoded, never read
			// taskmanager.cpp:6554
			std::string nick = packet.DecodeStr();
			// taskmanager.cpp:6555
			std::string text = packet.DecodeStr();
			// taskmanager.cpp:6567
			std::list<std::string>::iterator it = std::find(CSharedDoc::Instance()->m_ignoreList.begin(), CSharedDoc::Instance()->m_ignoreList.end(), nick);
			// taskmanager.cpp:6568
			if (it != CSharedDoc::Instance()->m_ignoreList.end()) {
				// taskmanager.cpp:6569
				break;
			}
			// taskmanager.cpp:6572
			const char *chatLine;
			GetActor("Lobby") << MsgObject(0, 0x3F, (intptr_t)&chatLine, 0, 0, 0, 0);
			// taskmanager.cpp:6576
			msg = "���:" + nick + "> ";
			// taskmanager.cpp:6579
			CSharedDoc::Instance()->m_chatManager.FilteringHack(text.c_str(), false);
			// taskmanager.cpp:6584
			CChatMsg::Instance()->AddChatMsg(msg, 0xFF5BFFB0, false, true);
		}
		// taskmanager.cpp:6586
		break;
	case 0x2e:
		// taskmanager.cpp:6591
		PG_BOOT_TODO("taskmanager.cpp:6591 case 0x2e: 4026 bytes; calls CMessengerInfo::SetOfflineUserNick, CMessengerInfo::Notify1"); // TODO(boot)
		break;
	case 0x98:
		// taskmanager.cpp:7792
		PG_BOOT_TODO("taskmanager.cpp:7792 case 0x98: 159 bytes; calls CSharedDoc::SetIdentity, IsPcBang, CSharedDoc::AccumulateIdentity, MyIndex"); // TODO(boot)
		break;
	case 0xaf:
		// taskmanager.cpp:7810
		PG_BOOT_TODO("taskmanager.cpp:7810 case 0xaf: 283 bytes; calls UsItemInfo::0?$list, CMessengerInfo::SetMyStatus"); // TODO(boot)
		break;
	case 0xb2:
		// taskmanager.cpp:7847
		PG_BOOT_TODO("taskmanager.cpp:7847 case 0xb2: 156 bytes"); // TODO(boot)
		break;
	case 0x16f:
		// taskmanager.cpp:7881
		PG_BOOT_TODO("taskmanager.cpp:7881 case 0x16f: 77 bytes"); // TODO(boot)
		break;
	case 0xd6:
		// taskmanager.cpp:7907
		PG_BOOT_TODO("taskmanager.cpp:7907 case 0xd6: 80 bytes"); // TODO(boot)
		break;
	case 0xb8:
		// taskmanager.cpp:7922
		{
			_SYSTEMTIME time;
			packet.DecodeBuffer(&time, sizeof(time));
			CSharedDoc::Instance()->SetServerTime(time);
		}
		break;
	case 0xc7:
		// taskmanager.cpp:7948
		PG_BOOT_TODO("taskmanager.cpp:7948 case 0xc7: 141 bytes; calls ChatManager::FilteringHack, COneLineBoard::AddOnelineMsg"); // TODO(boot)
		break;
	case 0xc9:
		// taskmanager.cpp:7961
		PG_BOOT_TODO("taskmanager.cpp:7961 case 0xc9: 11 bytes"); // TODO(boot)
		break;
	case 0x94:
		// taskmanager.cpp:7965
		CSharedDoc::Instance()->m_myBonusCash = packet.Decode8();
		// taskmanager.cpp:7966
		CSharedDoc::Instance()->RefreshAllCookie();
		break;
	case 0xc8:
		// taskmanager.cpp:7983
		PG_BOOT_TODO("taskmanager.cpp:7983 case 0xc8: 45 bytes"); // TODO(boot)
		break;
	case 0xcc:
		// taskmanager.cpp:7992
		PG_BOOT_TODO("taskmanager.cpp:7992 case 0xcc: 124 bytes"); // TODO(boot)
		break;
	case 0xf8:
		// taskmanager.cpp:8030
		PG_BOOT_TODO("taskmanager.cpp:8030 case 0xf8: 271 bytes; calls MyGuid"); // TODO(boot)
		break;
	case 0x1ae:
		// taskmanager.cpp:8089
		PG_BOOT_TODO("taskmanager.cpp:8089 case 0x1ae: 109 bytes; calls YAPAXI::2, CTaskManager::SetWork, CTaskManager::ReleaseWork"); // TODO(boot)
		break;
	case 0xd5:
		// taskmanager.cpp:8107
		PG_BOOT_TODO("taskmanager.cpp:8107 case 0xd5: 17 bytes"); // TODO(boot)
		break;
	case 0xcf:
		// taskmanager.cpp:8116
		PG_BOOT_TODO("taskmanager.cpp:8116 case 0xcf: 130 bytes"); // TODO(boot)
		break;
	case 0xd0:
		// taskmanager.cpp:8128
		PG_BOOT_TODO("taskmanager.cpp:8128 case 0xd0: 108 bytes"); // TODO(boot)
		break;
	case 0xd8:
		// taskmanager.cpp:8139
		PG_BOOT_TODO("taskmanager.cpp:8139 case 0xd8: 77 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0xdb:
		// taskmanager.cpp:8150
		PG_BOOT_TODO("taskmanager.cpp:8150 case 0xdb: 340 bytes; calls FrWnd::FindChildByName, FrScratchDlg::EnableNextPageButton, FrScratchDlg::SetScratchItem, FrScratchDlg::InitChangeScratchCardNumber, AfxPostMsg"); // TODO(boot)
		break;
	case 0xdc:
		// taskmanager.cpp:8203
		PG_BOOT_TODO("taskmanager.cpp:8203 case 0xdc: 303 bytes; calls FrWnd::FindChildByName, FrScratchDlg::EnableScratchSerialNumber, FrScratchDlg::ResponseScratchSerialNumber, FrScratchDlg::InitChangeScratchCardNumber"); // TODO(boot)
		break;
	case 0xdd:
		// taskmanager.cpp:8261
		PG_BOOT_TODO("taskmanager.cpp:8261 case 0xdd: 244 bytes"); // TODO(boot)
		break;
	case 0xde:
		// taskmanager.cpp:8295
		PG_BOOT_TODO("taskmanager.cpp:8295 case 0xde: 69 bytes"); // TODO(boot)
		break;
	case 0x16d:
		// taskmanager.cpp:8315
		PG_BOOT_TODO("taskmanager.cpp:8315 case 0x16d: 31 bytes"); // TODO(boot)
		break;
	case 0x164:
		// taskmanager.cpp:8328
		PG_BOOT_TODO("taskmanager.cpp:8328 case 0x164: 341 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x16c:
		// taskmanager.cpp:8380
		PG_BOOT_TODO("taskmanager.cpp:8380 case 0x16c: 585 bytes; calls IsLocalContent, MyUID"); // TODO(boot)
		break;
	case 0x167:
		// taskmanager.cpp:8451
		PG_BOOT_TODO("taskmanager.cpp:8451 case 0x167: 708 bytes; calls CSharedDoc::SetStallItem, CSharedDoc::ChageListStallitem, MyUID, FrWnd::FindChildByName"); // TODO(boot)
		break;
	case 0x161:
		// taskmanager.cpp:8566
		PG_BOOT_TODO("taskmanager.cpp:8566 case 0x161: 1110 bytes; calls IsLocalContent, MyUID, MyNick"); // TODO(boot)
		break;
	case 0x165:
	case 0x166:
		// taskmanager.cpp:8698
		PG_BOOT_TODO("taskmanager.cpp:8698 case 0x165/0x166: 391 bytes; calls CTaskMain::CreateMaketDlg, FrWnd::FindChildByName, CSharedDoc::ClearMaketList, CSharedDoc::AddMaketListItem, COfflineTradeDlg::SetListItem, COfflineTradeDlg::SetPageData"); // TODO(boot)
		break;
	case 0x162:
	case 0x163:
		// taskmanager.cpp:8780
		PG_BOOT_TODO("taskmanager.cpp:8780 case 0x162/0x163: 1122 bytes; calls IsLocalContent, MyUID, FrWnd::FindChildByName"); // TODO(boot)
		break;
	case 0x169:
		// taskmanager.cpp:8902
		PG_BOOT_TODO("taskmanager.cpp:8902 case 0x169: 754 bytes; calls IsLocalContent, CSharedDoc::GetServerTime, CSharedDoc::SetStallItem, CSharedDoc::ChageListStallitem"); // TODO(boot)
		break;
	case 0xe1:
		// taskmanager.cpp:9020
		PG_BOOT_TODO("taskmanager.cpp:9020 case 0xe1: 235 bytes; calls MyNick, CSharedDoc::SetStateTrade"); // TODO(boot)
		break;
	case 0xe2:
		// taskmanager.cpp:9040
		PG_BOOT_TODO("taskmanager.cpp:9040 case 0xe2: 481 bytes; calls MyUID, CSharedDoc::InitTradeData, FrWnd::FindChildByName"); // TODO(boot)
		break;
	case 0xe3:
		// taskmanager.cpp:9116
		PG_BOOT_TODO("taskmanager.cpp:9116 case 0xe3: 601 bytes; calls MyNick, CSharedDoc::SetStateTrade, FrWnd::FindChildByName"); // TODO(boot)
		break;
	case 0xe4:
		// taskmanager.cpp:9191
		PG_BOOT_TODO("taskmanager.cpp:9191 case 0xe4: 642 bytes; calls CSharedDoc::InitTradeData, CSharedDoc::SetStateTrade, CSharedDoc::SetTradeUID, CSharedDoc::SetTradeTitle, CSharedDoc::AddListTradeItem"); // TODO(boot)
		break;
	case 0xe5:
		// taskmanager.cpp:9275
		PG_BOOT_TODO("taskmanager.cpp:9275 case 0xe5: 49 bytes; calls CSharedDoc::InitTradeData, SetTradeResultCode"); // TODO(boot)
		break;
	case 0xe6:
		// taskmanager.cpp:9288
		PG_BOOT_TODO("taskmanager.cpp:9288 case 0xe6: 530 bytes; calls AfxGetPreservedTask, MyUID, CSharedDoc::SetTradeTitle"); // TODO(boot)
		break;
	case 0xe7:
		// taskmanager.cpp:9347
		PG_BOOT_TODO("taskmanager.cpp:9347 case 0xe7: 125 bytes; calls CSharedDoc::SetCountVisitor, SetTradeResultCode, FrWnd::FindChildByName, FrTradeDlg::UpdateCountVisitor"); // TODO(boot)
		break;
	case 0xe8:
		// taskmanager.cpp:9370
		PG_BOOT_TODO("taskmanager.cpp:9370 case 0xe8: 58 bytes; calls CSharedDoc::SetTradeIncome, SetTradeResultCode"); // TODO(boot)
		break;
	case 0xe9:
		// taskmanager.cpp:9388
		PG_BOOT_TODO("taskmanager.cpp:9388 case 0xe9: 599 bytes; calls CSharedDoc::SetStateTrade, CSharedDoc::ClearListTradeItem, CSharedDoc::SetTradeUID, CSharedDoc::AddListTradeItem"); // TODO(boot)
		break;
	case 0x1d7:
		// taskmanager.cpp:9463
		PG_BOOT_TODO("taskmanager.cpp:9463 case 0x1d7: 305 bytes; calls YIPAVIActor::6"); // TODO(boot)
		break;
	case 0x1d6:
		// taskmanager.cpp:9491
		PG_BOOT_TODO("taskmanager.cpp:9491 case 0x1d6: 320 bytes; calls YIPAVIActor::6"); // TODO(boot)
		break;
	case 0x1d8:
		// taskmanager.cpp:9520
		PG_BOOT_TODO("taskmanager.cpp:9520 case 0x1d8: 308 bytes; calls YIPAVIActor::6"); // TODO(boot)
		break;
	case 0xea:
		// taskmanager.cpp:9550
		PG_BOOT_TODO("taskmanager.cpp:9550 case 0xea: 3436 bytes; calls IsLocalContent, IsUccClothes"); // TODO(boot)
		break;
	case 0xeb:
		// taskmanager.cpp:9957
		PG_BOOT_TODO("taskmanager.cpp:9957 case 0xeb: 967 bytes; calls CSharedDoc::SetStateTrade, FrWnd::FindChildByName, CChatMsg::AddChatMsg, FrTradeDlg::SetListTradeItem, CSharedDoc::MinusListTradeItem"); // TODO(boot)
		break;
	case 0xec:
		// taskmanager.cpp:10074
		PG_BOOT_TODO("taskmanager.cpp:10074 case 0xec: 188 bytes; calls SetTradeResultCode, CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0xed:
		// taskmanager.cpp:10087
		PG_BOOT_TODO("taskmanager.cpp:10087 case 0xed: 417 bytes; calls UsRecycleItem::0?$list, FrWnd::FindChildByName, FrRecycleDlg::SetResultRecycleItem"); // TODO(boot)
		break;
	case 0x69:
		// taskmanager.cpp:10205
		PG_BOOT_TODO("taskmanager.cpp:10205 case 0x69: 964 bytes; calls IUsCharacterInfo::A?$map, CSharedDoc::BuildMyPartTidList, MyGuid, CSharedDoc::FindUserInfoTimeGUID"); // TODO(boot)
		break;
	case 0x15f:
		// taskmanager.cpp:10331
		PG_BOOT_TODO("taskmanager.cpp:10331 case 0x15f: 98 bytes"); // TODO(boot)
		break;
	case 0xef:
		// taskmanager.cpp:10342
		PG_BOOT_TODO("taskmanager.cpp:10342 case 0xef: 895 bytes; calls IsLocalContent, CSharedDoc::GetDirectMoveRoomInfo"); // TODO(boot)
		break;
	case 0xf6:
		// taskmanager.cpp:10471
		PG_BOOT_TODO("taskmanager.cpp:10471 case 0xf6: 176 bytes; calls MakeStr, CChatMsg::AddChatMsg, CNoticeBoard::AddNotice"); // TODO(boot)
		break;
	case 0x1f5:
		// taskmanager.cpp:10525
		PG_BOOT_TODO("taskmanager.cpp:10525 case 0x1f5: 782 bytes; calls FrRankingDlg::SetRankTypes, FrRankingDlg::SetBottomMsg, FrRankingDlg::ClearRankList, FrRankingDlg::UpdateMyInfo"); // TODO(boot)
		break;
	case 0x1f6:
		// taskmanager.cpp:10600
		PG_BOOT_TODO("taskmanager.cpp:10600 case 0x1f6: 172 bytes; calls CRankingInfo::OnNotify, FrRankingDlg::SetBottomMsg, IsLocalContent, RequestUccInfo, FrRankingDlg::UpdateUserInfo"); // TODO(boot)
		break;
	case 0x1f7:
		// taskmanager.cpp:10630
		PG_BOOT_TODO("taskmanager.cpp:10630 case 0x1f7: 28 bytes; calls CRankingInfo::OnNotify"); // TODO(boot)
		break;
	case 0x1f8:
		// taskmanager.cpp:10637
		PG_BOOT_TODO("taskmanager.cpp:10637 case 0x1f8: 502 bytes; calls CRankingInfo::OnNotify, FrRankingDlg::SetRankTypes"); // TODO(boot)
		break;
	case 0xfc:
		// taskmanager.cpp:10689
		PG_BOOT_TODO("taskmanager.cpp:10689 case 0xfc: 37 bytes; calls FrPointEventDlg::ResultMessage"); // TODO(boot)
		break;
	case 0xfb:
		// taskmanager.cpp:10701
		PG_BOOT_TODO("taskmanager.cpp:10701 case 0xfb: 29 bytes"); // TODO(boot)
		break;
	case 0xfd:
		// taskmanager.cpp:10708
		PG_BOOT_TODO("taskmanager.cpp:10708 case 0xfd: 139 bytes; calls CCommodityStatusManager::UpdateTotalCount"); // TODO(boot)
		break;
	case 0xfe:
		// taskmanager.cpp:10723
		PG_BOOT_TODO("taskmanager.cpp:10723 case 0xfe: 124 bytes; calls CCommodityStatusManager::UpdateStatus, CCommodityStatusManager::UpdateMyCount"); // TODO(boot)
		break;
	case 0x10b:
		// taskmanager.cpp:10740
		PG_BOOT_TODO("taskmanager.cpp:10740 case 0x10b: 670 bytes; calls PAVCGiftBoxEvent::GetContainer, CGiftBoxEvent::GetOpenedBoxTID, CTask::_RemoveWareHouseItem, CTask::_ReFreshMyRoomGift"); // TODO(boot)
		break;
	case 0x107:
		// taskmanager.cpp:10812
		PG_BOOT_TODO("taskmanager.cpp:10812 case 0x107: 368 bytes; calls CTask::_RemoveWareHouseItem, ItemManager, CItemManager::FindCommonItem"); // TODO(boot)
		break;
	case 0xf7:
		// taskmanager.cpp:10850
		PG_BOOT_TODO("taskmanager.cpp:10850 case 0xf7: 65 bytes"); // TODO(boot)
		break;
	case 0xce:
		// taskmanager.cpp:10859
		PG_BOOT_TODO("taskmanager.cpp:10859 case 0xce: 280 bytes; calls CSharedDoc::RefreshItemListFromGuidList"); // TODO(boot)
		break;
	case 0xc3:
		// taskmanager.cpp:10892
		PG_BOOT_TODO("taskmanager.cpp:10892 case 0xc3: 726 bytes; calls ItemManager, CItemManager::FindCommonItem, MakeStr"); // TODO(boot)
		break;
	case 0x15e:
		// taskmanager.cpp:10968
		PG_BOOT_TODO("taskmanager.cpp:10968 case 0x15e: 166 bytes"); // TODO(boot)
		break;
	case 0x108:
		// taskmanager.cpp:10993
		PG_BOOT_TODO("taskmanager.cpp:10993 case 0x108: 329 bytes; calls ChatManager::FilteringHack, ItemManager, CItemManager::FindCommonItem"); // TODO(boot)
		break;
	case 0x109:
		// taskmanager.cpp:11036
		PG_BOOT_TODO("taskmanager.cpp:11036 case 0x109: 288 bytes; calls CSharedDoc::SetBsUsableBonusTimes, CSharedDoc::SetBsRemainedBonusTimes, YIPAVIActor::6"); // TODO(boot)
		break;
	case 0x10e:
		// taskmanager.cpp:11068
		PG_BOOT_TODO("taskmanager.cpp:11068 case 0x10e: 1220 bytes; calls IsAdministrator, PAUsPrize::0?$list, YAPAXI::2, YAPAXI::_U"); // TODO(boot)
		break;
	case 0x10c:
		// taskmanager.cpp:11206
		{
			sUserMatchHistory history[5];
			// taskmanager.cpp:11207
			packet.DecodeBuffer(history, sizeof(history));
			// taskmanager.cpp:11208
			CMatchHistory::Instance()->SetMatchHistory(history);
		}
		break;
	case 0x10f:
		// taskmanager.cpp:11214
		PG_BOOT_TODO("taskmanager.cpp:11214 case 0x10f: 102 bytes; calls CSharedDoc::SetFlagBlock, CreateForm<FrForm>"); // TODO(boot)
		break;
	case 0x10d:
		// taskmanager.cpp:11234
		PG_BOOT_TODO("taskmanager.cpp:11234 case 0x10d: 285 bytes"); // TODO(boot)
		break;
	case 0x111:
		// taskmanager.cpp:11257
		PG_BOOT_TODO("taskmanager.cpp:11257 case 0x111: 97 bytes; calls IsLocalContent, CIntrusion::PacketAnalysis"); // TODO(boot)
		break;
	case 0x10a:
		// taskmanager.cpp:11270
		PG_BOOT_TODO("taskmanager.cpp:11270 case 0x10a: 22 bytes"); // TODO(boot)
		break;
	case 0x110:
		// taskmanager.cpp:11279
		PG_BOOT_TODO("taskmanager.cpp:11279 case 0x110: 71 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x118:
		// taskmanager.cpp:11300
		PG_BOOT_TODO("taskmanager.cpp:11300 case 0x118: 139 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x119:
		// taskmanager.cpp:11472
		PG_BOOT_TODO("taskmanager.cpp:11472 case 0x119: 195 bytes; calls PAVCWorldTourEvent::GetContainer, IsLocalContent, CWorldTourEvent::SetMapFlag, CWorldTourEvent::SetGotGift, CWorldTourEvent::SetCanGift, CWorldTourEvent::GetMapFlag"); // TODO(boot)
		break;
	case 0x11a:
		// taskmanager.cpp:11511
		PG_BOOT_TODO("taskmanager.cpp:11511 case 0x11a: 197 bytes; calls PAVCWorldTourEvent::GetContainer, CWorldTourEvent::SetMapFlag, CWorldTourEvent::SetGotGift, CWorldTourEvent::SetCanGift, MyUID"); // TODO(boot)
		break;
	case 0x11f:
		// taskmanager.cpp:11570
		PG_BOOT_TODO("taskmanager.cpp:11570 case 0x11f: 84 bytes; calls PAVCHalloweenEvent::GetContainer, CHalloweenEvent::SetMapFlag, CHalloweenEvent::SetGiftCnt"); // TODO(boot)
		break;
	case 0x120:
		// taskmanager.cpp:11582
		PG_BOOT_TODO("taskmanager.cpp:11582 case 0x120: 210 bytes; calls IsLocalContent, PAVCHalloweenEvent::GetContainer, CHalloweenEvent::SetGiftCnt, CHalloweenEvent::SetShowGiftDlg, MyNick"); // TODO(boot)
		break;
	case 0x122:
		// taskmanager.cpp:11617
		PG_BOOT_TODO("taskmanager.cpp:11617 case 0x122: 1376 bytes; calls CTHunter::SetUsedTikiReport, IsLocalContent, CreateForm<FrImageNoticeDlg>, FrImageNoticeDlg::SetDlgSize, FrImageNoticeDlg::SetBackGroundImage, FrImageNoticeDlg::SetOkButton"); // TODO(boot)
		break;
	case 0x129:
		// taskmanager.cpp:11863
		PG_BOOT_TODO("taskmanager.cpp:11863 case 0x129: 188 bytes; calls IsLocalContent, CTHunter::SetTHunterGauge"); // TODO(boot)
		break;
	case 0x12a:
		// taskmanager.cpp:11898
		PG_BOOT_TODO("taskmanager.cpp:11898 case 0x12a: 129 bytes; calls IsLocalContent, GetGameType, CTHunter::SetTreasurePoint, YIPAVIActor::6"); // TODO(boot)
		break;
	case 0x12b:
		// taskmanager.cpp:11925
		PG_BOOT_TODO("taskmanager.cpp:11925 case 0x12b: 207 bytes; calls UsTreasureHunt::0?$vector, CTHunter::ReceiveRepository, CTHunter::SetReceiveGiftPacket, CSharedDoc::SetAppGiftDownloadState"); // TODO(boot)
		break;
	case 0x12c:
		// taskmanager.cpp:11954
		PG_BOOT_TODO("taskmanager.cpp:11954 case 0x12c: 180 bytes; calls IsLocalContent, UsTreasureGift::0?$vector, CTHunter::ReceiveAllGift, CTHunter::UpdateItemList, UsTreasureGift::1?$vector"); // TODO(boot)
		break;
	case 0x138:
		// taskmanager.cpp:11975
		PG_BOOT_TODO("taskmanager.cpp:11975 case 0x138: 73 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x126:
		// taskmanager.cpp:11994
		PG_BOOT_TODO("taskmanager.cpp:11994 case 0x126: 2156 bytes"); // TODO(boot)
		break;
	case 0x13c:
		// taskmanager.cpp:12194
		PG_BOOT_TODO("taskmanager.cpp:12194 case 0x13c: 89 bytes; calls CMatchingSystem::SetDisplayIntrest"); // TODO(boot)
		break;
	case 0x14f:
		// taskmanager.cpp:12214
		PG_BOOT_TODO("taskmanager.cpp:12214 case 0x14f: 198 bytes; calls CSharedDoc::FindUserInfoTimeUID, CSharedDoc::InsertUserInfoTime"); // TODO(boot)
		break;
	case 0x150:
		// taskmanager.cpp:12244
		PG_BOOT_TODO("taskmanager.cpp:12244 case 0x150: 309 bytes; calls CSharedDoc::FindUserInfoTimeUID, CSharedDoc::InsertUserInfoTime"); // TODO(boot)
		break;
	case 0x14e:
		// taskmanager.cpp:12294
		PG_BOOT_TODO("taskmanager.cpp:12294 case 0x14e: 83 bytes; calls CSharedDoc::FindUserInfoTimeUID"); // TODO(boot)
		break;
	case 0x155:
		// taskmanager.cpp:12309
		PG_BOOT_TODO("taskmanager.cpp:12309 case 0x155: 189 bytes; calls CSharedDoc::FindUserInfoTimeUID, MyUID"); // TODO(boot)
		break;
	case 0x153:
		// taskmanager.cpp:12344
		PG_BOOT_TODO("taskmanager.cpp:12344 case 0x153: 239 bytes; calls CSharedDoc::FindUserInfoTimeUID, YAXPAX::_V, YAPAXI::_U"); // TODO(boot)
		break;
	case 0x151:
		// taskmanager.cpp:12399
		PG_BOOT_TODO("taskmanager.cpp:12399 case 0x151: 169 bytes; calls CSharedDoc::FindUserInfoTimeUID"); // TODO(boot)
		break;
	case 0x152:
		// taskmanager.cpp:12425
		PG_BOOT_TODO("taskmanager.cpp:12425 case 0x152: 301 bytes; calls CSharedDoc::FindUserInfoTimeUID, YAXPAX::_V, YAPAXI::_U"); // TODO(boot)
		break;
	case 0x154:
		// taskmanager.cpp:12480
		PG_BOOT_TODO("taskmanager.cpp:12480 case 0x154: 241 bytes; calls CSharedDoc::FindUserInfoTimeUID, YGXPAXIHP6EPAX0::_H"); // TODO(boot)
		break;
	case 0x156:
		// taskmanager.cpp:12511
		PG_BOOT_TODO("taskmanager.cpp:12511 case 0x156: 151 bytes; calls CSharedDoc::FindUserInfoTimeUID, CSharedDoc::SetUserInfoMod, IsLocalContent, RequestUccInfo"); // TODO(boot)
		break;
	case 0x14b:
		// taskmanager.cpp:12541
		PG_BOOT_TODO("taskmanager.cpp:12541 case 0x14b: 318 bytes; calls SecurityKeyManager, CSecurityKeyManager::ResCheckKey, CSecurityKeyManager::ResCreateKey"); // TODO(boot)
		break;
	case 0x15b:
		// taskmanager.cpp:12569
		PG_BOOT_TODO("taskmanager.cpp:12569 case 0x15b: 1185 bytes; calls CChatMsg::AddChatMsg"); // TODO(boot)
		break;
	case 0x123:
		// taskmanager.cpp:12699
		PG_BOOT_TODO("taskmanager.cpp:12699 case 0x123: 323 bytes; calls IsLocalContent, AfxPostMsg"); // TODO(boot)
		break;
	case 0x186:
		// taskmanager.cpp:12738
		PG_BOOT_TODO("taskmanager.cpp:12738 case 0x186: 85 bytes; calls CTask::_RemoveWareHouseItem, WReceivedPacket::Reset, CItemBuff::ProcessPacket"); // TODO(boot)
		break;
	case 0x188:
		// taskmanager.cpp:12756
		PG_BOOT_TODO("taskmanager.cpp:12756 case 0x188: 121 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x189:
		// taskmanager.cpp:12770
		PG_BOOT_TODO("taskmanager.cpp:12770 case 0x189: 119 bytes; calls IsLocalContent, CTask::_RemoveWareHouseItem, CreateForm<FrForm>"); // TODO(boot)
		break;
	case 0x18d:
		// taskmanager.cpp:12794
		PG_BOOT_TODO("taskmanager.cpp:12794 case 0x18d: 422 bytes; calls CreateForm<FrCardRemoveResult>, FrCardRemove::IsCardRemoveItem, FrCardRemoveResult::SetRemover, CTask::_RemoveWareHouseItem, KV?$allocator::0?$list, UTIL::ErasePartsCardAll"); // TODO(boot)
		break;
	case 0x18b:
		// taskmanager.cpp:12863
		PG_BOOT_TODO("taskmanager.cpp:12863 case 0x18b: 20 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x198:
		// taskmanager.cpp:12893
		PG_BOOT_TODO("taskmanager.cpp:12893 case 0x198: 35 bytes; calls SetMyOutsideCash, CSharedDoc::RefreshAllCookie"); // TODO(boot)
		break;
	case 0x19a:
		// taskmanager.cpp:12902
		PG_BOOT_TODO("taskmanager.cpp:12902 case 0x19a: 146 bytes; calls CSharedDoc::GetMyPastMapStat, insert"); // TODO(boot)
		break;
	case 0x1a1:
		// taskmanager.cpp:13048
		PG_BOOT_TODO("taskmanager.cpp:13048 case 0x1a1: 205 bytes; calls IsLocalContent"); // TODO(boot)
		break;
	case 0x1cf:
		// taskmanager.cpp:13350
		PG_BOOT_TODO("taskmanager.cpp:13350 case 0x1cf: 32 bytes"); // TODO(boot)
		break;
	case 0x1d0:
		// taskmanager.cpp:13359
		PG_BOOT_TODO("taskmanager.cpp:13359 case 0x1d0: 110 bytes; calls YIPAVIActor::6"); // TODO(boot)
		break;
	case 0x30:
	case 0x60:
	case 0xff:
	case 0x11e:
	case 0x13b:
	case 0x19e:
		PG_BOOT_TODOF(("taskmanager.cpp:13359 empty case %#x (of 0x30, 0x60, 0xff, 0x11e, 0x13b, 0x19e)", (unsigned)(g_iRecvPacketType)));
		break;
	default:
		// taskmanager.cpp:13946
		return 0;
	}
	// taskmanager.cpp:13954
	return 1;
}

// taskmanager.cpp:13964
int CTask::_RemoveWareHouseItem(unsigned long typeId, IActor *pActor, int count) {
	unsigned long serial = 0;
	std::list<sItemInfo>::iterator it;

	if (typeId == 0x1A000042 && IsLocalContent((localContentType_t)0x14)) {
		AfxGetTask()->PassMsgToActor("RealMyRoom", MsgObject(0, 0x204, (intptr_t)&serial, 0, 0, 0, 0));
		for (it = CSharedDoc::Instance()->m_itemInfoList.begin(); it != CSharedDoc::Instance()->m_itemInfoList.end(); ++it) {
			if (it->m_serial == serial) {
				break;
			}
		}
	} else {
		for (it = CSharedDoc::Instance()->m_itemInfoList.begin(); it != CSharedDoc::Instance()->m_itemInfoList.end(); ++it) {
			if (it->m_typeId == typeId) {
				break;
			}
		}
	}

	if (it == CSharedDoc::Instance()->m_itemInfoList.end()) {
		return 0;
	}

	int removed;
	for (removed = 0; removed < count; removed++) {
		it->m_count--;
		if (it->m_count <= 0) {
			CSharedDoc::Instance()->m_itemInfoList.erase(it);
			break;
		}
	}

	if (pActor) {
		pActor->HandleMsg(MsgObject(0, 0xDD, 0, 0, 0, 0, 0));
	}

	return removed;
}

// taskmanager.cpp:14002
int CTask::_RemoveWareHouseItem(unsigned long typeId, const char *actorName, int count) {
	return _RemoveWareHouseItem(typeId, AfxGetTask()->GetActor(actorName), count);
}

// taskmanager.cpp:14013
void CTask::_ReFreshMyRoomGift(bool bSend, std::string name) {
	if (bSend) {
		WSendPacket packet(0x21);
		packet.EncodeStr(name);
		packet.Encode4(MyUID());
		packet.Send(SEND_TO_MSN);
	} else {
		CSharedDoc::Instance()->m_giftList.clear();
		WSendPacket packet((enumClientPacket)0x93);
		packet.Send(SEND_TO_DEFAULT);
	}
}

// taskmanager.cpp:14039
bool CTask::OnOverlappedDlg(int result, FrForm *pForm) {
	g_bQuit = true;
	return true;
}

// taskmanager.cpp:14051
CTaskManager::CTaskManager(void)
	: m_bReservedTask(false), m_reservedPreserve(false), m_reservedTaskName(NULL), m_reservedParam(NULL),
	  m_currentTask(NULL), m_preservedTask(NULL), m_pBackGround(NULL), m_pWork(NULL) {
	m_curHistory = m_history.begin();

	m_naviState = 0;
}

// taskmanager.cpp:14057
CTaskManager::~CTaskManager(void) {
	ClearTask();

	if (m_preservedTask) {

		m_preservedTask->Destroy();
		delete m_preservedTask;
		m_preservedTask = NULL;
	}

	if (m_pBackGround) {
		delete m_pBackGround;
		m_pBackGround = NULL;
	}
	delete m_pWork;
}

// taskmanager.cpp:14071
void CTaskManager::CreateTask(const char *taskName, const char *param, bool preserve) {
	if (preserve) {
		if (m_preservedTask == NULL) {
			m_preservedTask = m_currentTask;
			m_preservedTask->PreserveBack(taskName);
			m_currentTask = NULL;
		}
		ClearTask();
	} else {
		ClearTask();

		if (m_preservedTask) {
			if (stricmp(m_preservedTask->GetRTTI()->GetName(), taskName) == 0) {
				m_currentTask = m_preservedTask;
				m_currentTask->RestorePreserved();

				m_preservedTask = NULL;
				m_bReservedTask = false;
				m_reservedTaskName = NULL;
				m_reservedParam = NULL;
				if (m_pBackGround) {
					m_pBackGround->ResetScreenSize();
				}
				return;
			}

			m_preservedTask->Destroy();
			delete m_preservedTask;
			m_preservedTask = NULL;
		}
	}

	m_currentTask = (CTask *)ObjectFactory().CreateObject(taskName);

	m_currentTask->ApplyScreenSize();

	if (m_pBackGround) {
		m_pBackGround->ResetScreenSize();
	}

	if (param && *param) {
		// The type test casts the created object itself, not __rtti_obj.
		IObject *pDoc = ObjectFactory().CreateObject(param);
		m_currentTask->m_pDoc = DYNAMIC_CAST(CTaskDoc, pDoc);
		if (m_currentTask->m_pDoc) {
			m_currentTask->m_pDoc->m_pTask = m_currentTask;
		}
	} else {
		m_currentTask->m_pDoc = NULL;
	}

	m_currentTask->Register();
	m_currentTask->PreLoadInit();

	g_LoadParam.m_progress = 0;
	g_LoadParam.m_bLoaded = false;
	g_LoadParam.m_bInited = false;
	m_reservedPreserve = preserve;

	m_bReservedTask = false;
	m_reservedTaskName = NULL;
	m_reservedParam = NULL;
	m_currentTask->Load();
}

// taskmanager.cpp:14162
void CTaskManager::ChangeTask(const char *taskName, const char *param, bool preserve) {
	CMessengerInfo::Instance()->CloseWindow();

	m_reservedTaskName = taskName;
	m_reservedParam = param;

	m_bReservedTask = true;
	m_reservedPreserve = preserve;

	g_audio->m_fadeOutTime = 1.0f;

	if (m_pBackGround) { delete m_pBackGround; m_pBackGround = NULL; }
	m_pBackGround = new CBackGround(NULL, true);
	m_backGroundAlpha = 1.0f;
}

// taskmanager.cpp:14180
void CTaskManager::PreProcess(void) {
	if (m_bReservedTask) {
		CreateTask(m_reservedTaskName, m_reservedParam, m_reservedPreserve);
	}
}

// taskmanager.cpp:14187
void CTaskManager::ProcessTask(float dt) {
	if (m_bReservedTask) {
		return;
	}

	if (m_pBackGround) {
		float step = (m_currentTask->m_unknownC ? 0.8f : 1.0f) * dt;
		if (step > 0.04f) {
			step = 0.04f;
		}

		m_backGroundAlpha -= step;
		if (m_backGroundAlpha < 0.0f) {
			m_backGroundAlpha = 0.0f;
			delete m_pBackGround;
			m_pBackGround = NULL;
		}
	}

	if (!g_LoadParam.m_bLoaded) {
		m_currentTask->OnProcessWhileLoading(dt);

		Sleep(30);
		return;
	}

	if (!g_LoadParam.m_bInited) {
		m_currentTask->Init(NULL);
		g_LoadParam.m_bInited = true;
	}

	while (!m_postMsgQueue.empty()) {
		sPostMsg &postMsg = m_postMsgQueue.front();
		m_currentTask->PassMsgToActor(postMsg.m_target.c_str(), postMsg.m_msg);
		m_postMsgQueue.pop();
	}

	m_currentTask->Process(dt);
}

// taskmanager.cpp:14240
void CTaskManager::DisplayTask(void) const {
	if (!g_LoadParam.m_bInited) {
		m_currentTask->OnDisplayWhileLoading();
	} else {
		if (!m_bReservedTask) {
			m_currentTask->Display();
		}
	}

	if (m_pBackGround) {
		m_pBackGround->Draw(((int)(m_backGroundAlpha * 255.0f) << 24) | 0xFFFFFF);
	}
}

// taskmanager.cpp:14257
void CTaskManager::ClearTask(void) {
	if (CCapturedBg::Instance()) {
		CCapturedBg::Instance()->Reset();
	}

	if (m_currentTask) {
		m_currentTask->Destroy();
		delete m_currentTask;
		m_currentTask = NULL;
	}

	if (HackingManager::Instance()) {
		HackingManager::Instance()->ClearCrypticList();
	}
}

// taskmanager.cpp:14280
CTask *CTaskManager::GetCurrentTask(void) const {
	return m_currentTask;
}

// taskmanager.cpp:14287
int CTaskManager::PostMsg(const IActor *pSender, const char *pTarget, int msg, intptr_t param1, intptr_t param2, intptr_t param3, unsigned long param4) {
	sPostMsg postMsg;

	postMsg.m_msg.m_pSender = pSender;
	postMsg.m_msg.m_msg = msg;
	postMsg.m_msg.m_param1 = param1;
	postMsg.m_msg.m_param2 = param2;
	postMsg.m_msg.m_param3 = param3;
	postMsg.m_msg.m_reserved = param4;
	postMsg.m_target = pTarget;

	m_postMsgQueue.push(postMsg);
	return 0;
}

// taskmanager.cpp:14305
CTaskDoc::CTaskDoc(void) {
}

// taskmanager.cpp:14309
CTaskDoc::~CTaskDoc(void) {
}

// taskmanager.cpp:14313
CTask *CTaskDoc::GetTask(void) const {
	return m_pTask;
}

// taskmanager.cpp:14324
void CTaskManager::NaviOpenLayout(const char *taskName, const char *docName, const char *actorName, const char *layoutName) {
	switch (m_naviState) {
	case 1:
		--m_curHistory;
		break;
	case 0: {
		std::list<history_t>::iterator it = m_curHistory;
		if (++it != m_history.end()) {
			if (stricmp((*it).m_layoutName, layoutName) == 0) {
				++m_curHistory;
				break;
			}
			m_history.erase(++m_curHistory, m_history.end());
			NaviAddHistory(taskName, docName, actorName, layoutName);
		} else {
			NaviAddHistory(taskName, docName, actorName, layoutName);
		}
		break;
	}
	}

	CTask *task = AfxGetTask();
	((CTaskMain *)task->m_mainActor)->CheckNaviButtons();

	m_naviState = 0;
}

// taskmanager.cpp:14353
void CTaskManager::NaviAddHistory(const char *taskName, const char *docName, const char *actorName, const char *layoutName) {
	history_t history;
	history.m_taskName = taskName;

	history.m_docName = docName;
	history.m_actorName = actorName;
	history.m_layoutName = layoutName;

	std::list<history_t>::iterator it = m_history.end();
	m_history.insert(it, history);
	m_curHistory = --it;
}

// taskmanager.cpp:14368
void CTaskManager::NaviClearHistory(void) {
	m_history.clear();
}

// taskmanager.cpp:14373
void CTaskManager::NaviGoForward(void) {
	std::list<history_t>::iterator it = m_curHistory;

	if (it != m_history.end()) {
		if (++it != m_history.end()) {
			history_t history = *it;

			sActor *actor = AfxGetTask()->m_actorList.Find(history.m_actorName);
			if (!actor || !actor->m_pActor) {
				CTaskManager::Instance()->ChangeTask(history.m_taskName, history.m_docName, false);
			}

			CTaskManager::Instance()->PostMsg(NULL, history.m_actorName, 0, (intptr_t)history.m_layoutName, 1, 0, 0);
			m_naviState = 0;
		}
	}
}

// taskmanager.cpp:14393
void CTaskManager::NaviGoBackward(void) {
	std::list<history_t>::iterator it = m_curHistory;

	if (it != m_history.begin()) {
		history_t history = *--it;

		sActor *actor = AfxGetTask()->m_actorList.Find(history.m_actorName);
		if (!actor || !actor->m_pActor) {
			CTaskManager::Instance()->ChangeTask(history.m_taskName, history.m_docName, false);
		}

		CTaskManager::Instance()->PostMsg(NULL, history.m_actorName, 0, (intptr_t)history.m_layoutName, 1, 0, 0);
		m_naviState = 1;
	}
}

// taskmanager.cpp:14411
bool CTaskManager::IsNaviForwardEnable(void) {
	std::list<history_t>::iterator it = m_curHistory;

	return ++it != m_history.end() ? true : false;
}

// taskmanager.cpp:14421
bool CTaskManager::IsNaviBackwardEnable(void) {
	return m_curHistory != m_history.begin() ? true : false;
}

// taskmanager.cpp:14429
bool CTaskManager::IsNaviForwardSameHistory(const char *layoutName) {
	std::list<history_t>::iterator it = m_curHistory;

	return stricmp((*++it).m_layoutName, layoutName) == 0 ? true : false;
}

// taskmanager.cpp:14439
void CTaskManager::NaviDisplayDebugInfo(void) {
	float y = 200.0f;
	int index = 0;

	for (std::list<history_t>::iterator it = m_history.begin(); it != m_history.end(); ++it) {
		history_t history = *it;
		const char *mark;
		if (it == m_curHistory) {
			mark = "*";
		} else {
			mark = "";
		}

		TextOut(g_view, 5.0f, y, "%d: (%s)%s%s", index, history.m_actorName, history.m_layoutName, mark);
		y += 15.0f;
		index++;
	}
}

class CNoticeBoard;
class CMatchingSystem;
class CEventThread;
class CWinnerNotice;

// Instantiates the singleton accessors this unit emits (the linker kept this
// unit's copies; the callers are in other units).
template CNoticeBoard* WSingleton<CNoticeBoard>::Instance();
template CMatchingSystem* WSingleton<CMatchingSystem>::Instance();
template CEventThread* WSingleton<CEventThread>::Instance();
template CWinnerNotice* WSingleton<CWinnerNotice>::Instance();
