// Original: d:\build\custom\temp\code\client\projectg\realmyroommain.cpp
#define REALMYROOMMAIN_ANOTHERHAND_INLINE
#include "realmyroommain.h"
#include "ToolTip.h"
#include "AnotherHand.h"
#include "ObjectFactory.h"
#include "actor.h"
#include "TaskManager.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "ClientSetting.h"
#include "Shadow.h"
#include "fresh.h"
#include "../client/Wangreal/include/wresrcmng.h"
#include "avatar_task.h"
#include "frwnd.h"
#include "wview.h"
#include "RealMyRoom_Avatar.h"
#include "RealMyRoomData.h"
#include "mousecursor.h"
#include "CCardBook.h"
#include "fx.h"
#include "../shared/Localize.h"
#include "InputManager.h"
#include "woverlay.h"
#include "frwndmanager.h"
#include "TransferableItem.h"
#include "quadtree.h"
#include "UccManager.h"
#include "exhibition.h"
#include "partsinfo.h"
#include <math.h>

extern WMatrix g_camera;

// CAnotherHand::GetThreadID (anotherhand.h:56), IsWorking, Enter and Leave
// (anotherhand.inl) are emitted here out of line by AnotherHand.h through
// REALMYROOMMAIN_ANOTHERHAND_INLINE.

// tooltip.h:98
int CIconToolTip::GetCurTipIndex(void) {
	return m_curTipIndex;
}

// realmyroommain.h:45
const WRTTI* CRealMyRoomMain::GetRTTI(void) const {
	return &m_RTTI;
}

// realmyroommain.h: no line record (empty constructor of a nested helper struct)
CRealMyRoomMain::miniBtn_t::miniBtn_t(void) {
}

// anotherhand.h: no line record (constructor of the abstract base CHandOwner,
// only sets the vtable)
CHandOwner::CHandOwner(void) {
}

// realmyroommain.cpp:129 (run-time type and factory registration of CRealMyRoomMain)
const WRTTI CRealMyRoomMain::m_RTTI("CRealMyRoomMain", &CTaskMain::m_RTTI);

IObject* CRealMyRoomMainMakeInstance(void) {
	return new CRealMyRoomMain;
}

struct __impCRealMyRoomMain {
	__impCRealMyRoomMain(void) {
		ObjectFactory().AddObjectFunctor(CRealMyRoomMainMakeInstance, "CRealMyRoomMain");
		ActorFactory().AddActorClass("CRealMyRoomMain");
	}
};
static __impCRealMyRoomMain s_impCRealMyRoomMain;

// realmyroommain.cpp:217
CRealMyRoomMain::~CRealMyRoomMain(void) {
	Net_EquipDresserParts();
	Net_EquipCharacter();
	Net_EquipCaddie();
	Net_EquipItem();
	Net_EquipGear();
	Net_EquipSkin();

	Net_EquipMascot();

	if (IsLocalContent((localContentType_t)0x51)) {
		DeleteCardImages();
		// realmyroommain.cpp:231: CSharedDoc::Instance()->SendEquipItemSlot();
		// realmyroommain.cpp:234 (else): SendEquipItemSlotByMascot();
		// SharedDoc.h declares neither yet.
	}

	HidePet(true);

	g_resrcmng->UnlockAllByThread(m_petLoader.GetThreadID());
	m_petLoader.BreakWorkingThread();
	this << MsgObject(NULL, 2, 0, 0, 0, 0, 0);

	if (m_pRoomFx) {
		CFx::Instance()->CloseSequence(m_pRoomFx, false);
	}

	if (g_resrcmng && m_decalTexture) { g_resrcmng->Release(m_decalTexture); m_decalTexture = 0; }
	if (g_resrcmng && m_pTitleFont) { g_resrcmng->Release(m_pTitleFont); m_pTitleFont = NULL; }

	CSceneManager::Instance()->ReleaseElement();

	if (m_pCardBook) {
		delete m_pCardBook;
		m_pCardBook = NULL;
	}

	DeleteCutinImages();
}

// realmyroommain.cpp:272
void CRealMyRoomMain::OnPreLoadInit(void) {
	CTaskMain::OnPreLoadInit();

	// The shadow manager is kept across the avatar chat task.
	if (!((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
		new CShadowManager(g_resrcmng, COption::Instance()->vIsShadowEnabled());
	}

	CSharedDoc::Instance()->m_gameMode = 0;

	InitMyRoomControls();
	InitMyRoomDialogs();
	InitMyRoomVariables();

	GOLFDOC()->MakePlayer(1);
}

// realmyroommain.cpp:292
void CRealMyRoomMain::DoReLoad(void) {
	SendMsg(this, "Ambient", 500, 0, 0, 0, 0);

	OnLoad();

	CTaskManager *pTaskManager = CTaskManager::Instance();
	(!pTaskManager ? 0 : pTaskManager->GetCurrentTask())->Init(0);
}

// realmyroommain.cpp:301
void CRealMyRoomMain::OnInit(void) {
	CTaskMain::OnInit();

	SetBGMNone();

	CSceneManager::Instance()->UpdateFog(true);

	// "\xb8\xae\xbe\xf3\xb8\xb6\xc0\xcc\xb7\xeb": the real my room
	CSceneManager::Instance()->RegisterElement((eObjType)4, 0, this, "\xb8\xae\xbe\xf3\xb8\xb6\xc0\xcc\xb7\xeb", 0, 0);

	g_input->Reset();

	if (CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar) {
		m_avatarViewPos = CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar->m_position;
		m_avatarViewPos.y += 4.6f;
		m_avatarViewPos.z -= 46.0f;
	}

	m_characterMap = CSharedDoc::Instance()->m_characterMap;
	// realmyroommain.cpp:322 copies the 0x6C-byte equipment block of
	// CSharedDoc (0x628, m_myCaddieId...) into m_caddieId (0x974) as one
	// structure assignment; the structure is not declared yet.

	if (CMouseCursor::Instance() != NULL) {
		CMouseCursor::Instance()->SetActive(true);
		CMouseCursor::Instance()->m_mode = 2;
	}

	// realmyroommain.cpp:331-332: CCameraWork::Instance()->SelectCamera(200)
	// and its m_inputMode = 0; camerawork.h declares neither yet.

	g_camera.Rotate(-0.17453292f, 0);

	m_pCameras[0]->m_matrix = g_camera;
	m_pCameras[0]->m_matrix.pivot.y = g_camera.pivot.y - 5.0f;
	m_pCameras[0]->m_matrix.Rotate(0.34906584f, 1);
	m_pCameras[0]->m_matrix.pivot.x -= 5.0f;

	m_pCameras[0]->m_fov = g_view->GetFOV();
	m_savedCamera = g_camera;
	m_savedFov = g_view->GetFOV();
	m_savedCamera.pivot.y -= 5.0f;
	g_camera.pivot.y -= 7.0f;
	m_uiState = 0;
	g_view->SetFOV(0.78539819f);
}

// realmyroommain.cpp:681
void CRealMyRoomMain::OnDestroy(void) {
	CTaskMain::OnDestroy();

	// The shadow manager is kept across the avatar chat task.
	if (!((__rtti_obj = CTaskManager::Instance()->GetPreservedTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&ntAvatarChatTask::m_RTTI) : false)) {
		if (CShadowManager::Instance())
			delete CShadowManager::Instance();
	}

	int i;
	for (i = 0; i < 6; i++) {
		if (g_resrcmng && m_pGenderIcons[i]) {
			g_resrcmng->Release(m_pGenderIcons[i]);
			m_pGenderIcons[i] = 0;
		}
	}

	for (i = 0; i < 2; i++) {
		if (g_resrcmng && m_pMannerIcons[i]) {
			g_resrcmng->Release(m_pMannerIcons[i]);
			m_pMannerIcons[i] = 0;
		}
	}

	for (i = 0; i < 2; i++) {
		if (g_resrcmng && m_pAngelIcons[i]) {
			g_resrcmng->Release(m_pAngelIcons[i]);
			m_pAngelIcons[i] = 0;
		}
	}

	if (g_resrcmng && m_shadowTexture) {
		g_resrcmng->Release(m_shadowTexture);
		m_shadowTexture = 0;
	}

	// The guids of the avatars, collected before the messages are sent.
	int guids[30];
	int count = 0;
	for (i = 0; i < 30 && count < CRealMyRoom_AvatarManager::Instance()->m_avatarCount; i++) {
		if (CRealMyRoom_AvatarManager::Instance()->m_pAvatars[i])
			guids[count++] = CRealMyRoom_AvatarManager::Instance()->m_pAvatars[i]->m_guid;
	}

	for (i = 0; i < count; i++) {
		this << MsgObject(this, 0xE5, guids[i], 0, 0, 0, 0);
	}

	g_pFresh->GetManager()->SetExclusiveKey(true);
}

// realmyroommain.cpp:1814
void CRealMyRoomMain::Display(void) {
	if (CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar == NULL)
		return;

	CRealMyRoom_AvatarManager::Instance()->Render();
	CRealMyRoomData::Instance()->Render();

	CTransferableItem::Instance()->Render();

	// The pulsing decal under the avatar the player follows, or under his own target.
	if (m_decalShown) {
		WVector right, forward;
		right = WVector(4.8f, 0.0f, 0.0f);
		forward = WVector(0.0f, 0.0f, 4.8f);

		if (CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar->m_followNickName[0] != '\0') {
			CRealMyRoom_Avatar *pAvatar = CRealMyRoom_AvatarManager::Instance()->FindAvatarByNickName(CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar->m_followNickName);
			if (pAvatar)
				GetPVS().DrawDecal(m_decalTexture, pAvatar->m_position, right, forward, (cos(m_decalPulse * 10.0f) + 1.0f) * 0.05f + 1.0f, 1.0f, W_POLY_BLEND_ADD, false);
		} else {
			GetPVS().DrawDecal(m_decalTexture, CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar->m_targetPosition, right, forward, (cos(m_decalPulse * 10.0f) + 1.0f) * 0.05f + 1.0f, 1.0f, W_POLY_BLEND_ADD, false);
		}
	}

	DrawMouseCursor();
}

// realmyroommain.cpp:1868
void CRealMyRoomMain::DrawMouseCursor(void) {
	int cursor = CMouseCursor::Instance()->GetCursor();
	static int s_oldCursor = cursor;

	if (m_pRightMenuWnd && m_pRightMenuWnd->IsVisible()) {
		CMouseCursor::Instance()->SetCursor(s_oldCursor);
		return;
	}

	if (CRealMyRoomData::Instance()->GetState() == 2 && CRealMyRoomData::Instance()->m_pSelectModel) {
		if (CRealMyRoomData::Instance()->m_bDropped)
			CMouseCursor::Instance()->SetCursor(9);
		else
			CMouseCursor::Instance()->SetCursor(8);
		return;
	}

	if (CRealMyRoomData::Instance()->GetState() == 3 && CRealMyRoomData::Instance()->m_pSelectModel) {
		if (CRealMyRoomData::Instance()->m_bDropped)
			CMouseCursor::Instance()->SetCursor(11);
		else
			CMouseCursor::Instance()->SetCursor(10);
		return;
	}

	if (cursor != s_oldCursor)
		CMouseCursor::Instance()->SetCursor(s_oldCursor);

	s_oldCursor = CMouseCursor::Instance()->GetCursor();
}

// realmyroommain.cpp:1899
void CRealMyRoomMain::OnDisplay(void) {
	// The avatars' name tags and balloons.
	CRealMyRoom_AvatarManager::Instance()->RenderOverlay();

	g_view->DrawLine(m_debugLineFrom, 0xFFFFFFFF, m_debugLineTo, 0xFFFFFFFF, 0);
	g_view->DrawLine(m_debugLine2From, 0xFFFFFFFF, m_debugLine2To, 0xFFFFFFFF, 0);
}

// realmyroommain.cpp:2000
void CRealMyRoomMain::CheckUIPosition(void) {
	static float s_barY = COption::Instance()->gGetBar_Y();

	if (COption::Instance()->gGetBar_Y() != s_barY) {
		CalcUIOffset(false);
		s_barY = COption::Instance()->gGetBar_Y();

		g_pFresh->OpenLayout("REALMYROOM", this, false);
	}
}

// realmyroommain.cpp:2289
void CRealMyRoomMain::CalculationCameraMat(WMatrix& mat, WVector at, WVector dir, WVector pos, float targetFov, float curFov, float speed) {
	mat = g_camera;

	float fov = curFov;

	dir.Normalize();
	dir *= speed;

	pos += dir;

	if (!(WAbsBySign(curFov - targetFov) < 0.00001f)) {
		float step = speed * 46.0f;

		if (targetFov > curFov) {
			fov = curFov + step;
			if (fov > targetFov)
				fov = targetFov;
		} else {
			fov = curFov - step;
			if (fov < targetFov)
				fov = targetFov;
		}
	}

	g_view->SetFOV(fov);

	mat = MakeCameraMatrix(pos, at);
}

// realmyroommain.cpp:2452
void CRealMyRoomMain::ToggleTopViewMode(void) {
	if (GetFlags(1))
		return;

	m_topViewMode = !m_topViewMode;

	if (m_topViewMode) {
		GOLFDOC()->GameMode() = 0x200;

		m_cameraFov = 70.0f;
		m_cameraOffset = WVector::ZERO;
		m_cameraDisplayType = 3;
	} else {
		GOLFDOC()->GameMode() = 0;

		SetCameraDisplayType(0);
	}
}

// realmyroommain.cpp:2508
void CRealMyRoomMain::SetCameraDisplayType(int type) {
	// The camera type 5 shows next (m_pCameras[2..5]).
	static int s_cameraIndex = 0;

	if (type != 0) {
		for (;;) {
			switch (type) {
			case 1:
				g_camera = m_pCameras[0]->m_matrix;
				g_view->SetFOV(m_pCameras[0]->m_fov);
				break;

			case 2:
				g_camera = m_pCameras[2]->m_matrix;
				g_view->SetFOV(0.7330383f);

				m_eyePos = m_pCameras[3]->m_matrix.pivot;
				m_eyeFov = 0.7330383f;
				break;

			case 3:
				m_cameraFov = 70.0f;
				m_cameraOffset = WVector::ZERO;
				break;

			case 4:
				if (m_pCameras[6]) {
					g_camera = m_pCameras[6]->m_matrix;
					g_view->SetFOV(m_pCameras[0]->m_fov);
				} else {
					g_camera = m_savedCamera;
					g_view->SetFOV(m_savedFov);
				}

				m_eyePos = m_savedCamera.pivot;
				m_eyeFov = m_savedFov;
				break;

			case 5:
				if (m_pCameras[s_cameraIndex + 2])
					g_camera = m_pCameras[s_cameraIndex + 2]->m_matrix;
				else
					g_camera = m_savedCamera;

				g_view->SetFOV(70.0f);

				s_cameraIndex++;

				m_cameraFov = 70.0f;
				m_cameraOffset = WVector(0.0f, 0.0f, 5.0f);

				if (s_cameraIndex >= 4) {
					s_cameraIndex = 0;
					type = 4;
					continue;
				}

				if (m_pCameras[s_cameraIndex + 2])
					m_eyePos = m_pCameras[s_cameraIndex + 2]->m_matrix.pivot;
				else
					m_eyePos = m_savedCamera.pivot;
				break;
			}
			break;
		}
	} else {
		if (m_cameraDisplayType != 4)
			g_camera = m_savedCamera;
		g_view->SetFOV(m_savedFov);
	}

	m_cameraDisplayType = type;
}

// realmyroommain.cpp:2601
void CRealMyRoomMain::RefreshAllUccClothes(void) {
	sUccRefreshItem item;

	for (int i = 0; i < 10; i++) {
		if (!UccManager()->GetRefreshItem(item))
			break;

		// The pet exhibition shows the player's own parts.
		if (m_pPetExhibition && m_pPetExhibition->GetPetFrame()) {
			CPartTidList& tidList = CSharedDoc::Instance()->GetMyPartTidList();

			unsigned char slot = (unsigned char)((item.m_typeId >> 13) & 0x1F);
			if (tidList.m_partTids[slot] == item.m_typeId) {
				unsigned long serial = tidList.m_partIds[slot];

				sUccClothes* pClothes = UccManager()->FindClothes(serial);

				if (pClothes && strcmpi(pClothes->m_uccKey, item.m_uccKey) == 0) {
					m_pPetExhibition->GetPetFrame()->SetPart(&CSharedDoc::Instance()->GetMyPartTidList(), pClothes->m_typeId, pClothes->m_serial);
					m_pPetExhibition->GetPetFrame()->SetPart(&CSharedDoc::Instance()->GetMyPartTidList(), pClothes->m_typeId, pClothes->m_serial);
				}
			}
		}

		if (CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar) {
			CRealMyRoom_Avatar* pMyAvatar = CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar;

			unsigned char slot = (unsigned char)((item.m_typeId >> 13) & 0x1F);
			if (pMyAvatar->m_partTidList.m_partTids[slot] == item.m_typeId) {
				unsigned long serial = pMyAvatar->m_partTidList.m_partIds[slot];

				sUccClothes* pClothes = UccManager()->FindClothes(serial);

				if (pClothes && strcmpi(pClothes->m_uccKey, item.m_uccKey) == 0) {
					CRealMyRoom_AvatarManager::Instance()->m_pMyAvatar->RefreshUccClothes(pClothes->m_typeId, pClothes->m_serial);
				}
			}
		}

		for (int j = 0; j < CRealMyRoom_AvatarManager::Instance()->m_avatarCount; j++) {
			CRealMyRoom_Avatar* pAvatar = CRealMyRoom_AvatarManager::Instance()->m_pAvatars[j];

			if (pAvatar) {
				unsigned char slot = (unsigned char)((item.m_typeId >> 13) & 0x1F);
				if (pAvatar->m_partTidList.m_partTids[slot] == item.m_typeId) {
					unsigned long serial = pAvatar->m_partTidList.m_partIds[slot];

					sUccClothes* pClothes = UccManager()->FindClothes(serial);

					if (pClothes && strcmpi(pClothes->m_uccKey, item.m_uccKey) == 0) {
						pAvatar->RefreshUccClothes(pClothes->m_typeId, pClothes->m_serial);
					}
				}
			}
		}
	}
}
