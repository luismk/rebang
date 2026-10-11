// Original: d:\build\custom\temp\code\client\projectg\mascot.cpp
#include "mascot.h"
#include "ObjectFactory.h"
#include "fxinterpreter.h"
#include "SharedDoc.h"
#include "GolfDoc.h"
#include "wresrcmng.h"
#include "commonutil.h"
#include "wpuppet.h"
#include "wutil.h"
#include "wview.h"
#include "golfball.h"
#include "TaskManager.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "bitmap.h"

#include <stdio.h>
#include <string.h>

extern LightSet g_lightset;
extern WMatrix g_camera;

// mascot.cpp:21 (run-time type and factory registration of CMascot)
const WRTTI CMascot::m_RTTI("CMascot", &IActor::m_RTTI);

IObject *CMascotMakeInstance(void) {
	return new CMascot;
}

struct __impCMascot {
	__impCMascot(void) {
		ObjectFactory().AddObjectFunctor(CMascotMakeInstance, "CMascot");
		ActorFactory().AddActorClass("CMascot");
	}
};
static __impCMascot s_impCMascot;

// mascot.cpp:24
void CMascot::OnDestroy(void) {
	GetFxInterpreter()->DetachFxAll();
	m_unk7C = -1;

	for (int i = 0; i < 4; i++) {
		if (m_mascots[i].m_pBird) { delete m_mascots[i].m_pBird; m_mascots[i].m_pBird = 0; }
	}
}

// mascot.cpp:37
void CMascot::OnPreLoadInit(void) {
	for (int i = 0; i < 4; i++) {
		CSceneManager::Instance()->RegisterElement((eObjType)4, 0, this, MakeStr("mascot%02d", i), 8, 0);
	}
}

// mascot.cpp:115
int CMascot::ChangeMascotMsgInTex(const char *msg, const IFF_STRUCT::sMascot *pMascot, WPuppet *pPet, const char *suffix) {
	if (!msg || !pMascot || !pPet)
		return 0;

	if (pMascot->m_msgInTexture) {
		if (strrchr(pMascot->m_texture, '.')) {
			char texName[256];
			strcpy(texName, pMascot->m_texture);
			sprintf(strrchr(texName, '.'), "_%s%s", suffix, strrchr(pMascot->m_texture, '.'));

			int handle = g_resrcmng->LoadTexture(pMascot->m_texture, 0, 0, texName);
			w_texlist *pTex = g_resrcmng->FindTexture(pMascot->m_texture);

			if (handle && pTex) {
				Bitmap *pBitmap = g_resrcmng->LoadBitmap(pMascot->m_texture, 0, false);
				RECT rc = { 0, 0, pBitmap->Width(), pBitmap->Height() };

				FrGraphicInterface *pGDI;
				if (g_pFresh && (pGDI = g_pFresh->GetManager()->GetGDI()) != 0 && strlen(msg) != 0) {
					pGDI->SetTextStyle(1);
					pGDI->SetAlpha(1.0f);
					pGDI->SetTextColor(0xFF000000, 0xFF808080);
					WPoint pt((float)float2int(pBitmap->Width() * 0.5f), (float)float2int(pBitmap->Height() * 0.5f) - 6.0f);
					pGDI->PrintText(pt, 1, msg, pBitmap);
				}

				g_resrcmng->ChangeTexturePart(handle, pBitmap, rc);
				pPet->ChangeTextureByHandle(handle, pTex->texhandle, 0);
				delete pBitmap;

				return handle;
			}

			if (g_resrcmng && handle)
				g_resrcmng->Release(handle);
		}
	}

	return 0;
}

// mascot.cpp:174
void CMascot::OnReload(void) {
	OnLoad();
}

// mascot.cpp:180
void CMascot::OnInit(void) {
}

// mascot.cpp:202
void CMascot::OnProcess(float dt) {
	for (int i = 0; i < GOLFDOC()->GetPlayerNum(); i++) {
		if (m_mascots[i].m_pBird) {
			m_mascots[i].m_pBird->Process(dt);
		}
	}
}

// mascot.cpp:213
void CMascot::Display(void) {
}

// mascot.cpp:217
void CMascot::HandleMsg(const MsgObject &msg) {
	switch (msg.m_msg) {
	case 0x164:
		if (m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird) {
			static_cast<CCameraBird *>(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird)->SetActive(false);
		}
		break;
	case 0x163:
		if (m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird) {
			static_cast<CCameraBird *>(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird)->SetActive(true);
		}
		break;
	case 0x4E:
		if ((unsigned long)msg.m_param1 == MyGuid(true)) {
			WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_mascot = *(const sMascotInfo *)msg.m_param2;
			WSingleton<CSharedDoc>::Instance()->m_myMascotId = WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_mascot.id;
		}

		m_mascots[GOLFDOC()->m_myPlayerIndex].m_pPlayer = &WSingleton<CSharedDoc>::Instance()->m_roomPlayers[0].m_mascot;

		const IFF_STRUCT::sMascot *pIff = ItemManager()->FindMascot(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pPlayer->typeId);
		if (pIff) {
			CCameraBird *pBird = static_cast<CCameraBird *>(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird);
			if (pBird) {
				pBird->ChangePet(MakeStr("%s.pet", pIff->m_model));

				int handle = ChangeMascotMsgInTex(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pPlayer->message, pIff, pBird->GetPet(), MakeStr("_golf%02d", GOLFDOC()->m_myPlayerIndex));
				static_cast<CCameraBird *>(m_mascots[GOLFDOC()->m_myPlayerIndex].m_pBird)->SetAddTex(handle);
			}
		}
		break;
	}
}

// mascot.cpp:263
CCameraBird::CCameraBird(const char *name, unsigned char playerIndex) {
	Init(name, 8);
	m_playerIndex = playerIndex;
	m_unk74 = 0.5f;
}

// mascot.cpp:271
void CCameraBird::Process(float dt) {
	if (PLAYER(m_playerIndex)->m_state != 3 && GOLFDOC()->m_myPlayerIndex == m_playerIndex && !(GOLFDOC()->GameMode() & 0x22001))
		CCreature::Process(dt);
}

// mascot.cpp:292
void CCameraBird::Display(void) {
	if (!m_pPet || !m_active || m_motionTime < 0.0f) return;

	if (PLAYER(m_playerIndex)->m_state == 3 || GOLFDOC()->m_myPlayerIndex != m_playerIndex || GOLFDOC()->GameMode() & 0x22001) return;

	m_pPet->ApplyBones(m_pPet->GetBones(0, m_motionTime, 0, 0, false), Abs(m_motionParam) > 0.0f ? RotMat(m_motionParam, 2) * m_matrix : m_matrix);
	m_pPet->Transform(g_view, 1.0f, 0);
	m_pPet->UpdateBSphere();

	if (g_view->InFrustum(m_pPet->GetBoundSphere())) {
		m_pPet->UpdateLightSource(&g_lightset, false, 0);
		WPuppet *pet = m_pPet; WView *view = g_view; pet->Render(view, true, 1.0f, view->InFrustumSafe(pet->GetBoundBox()), false, false);
	}
}

// mascot.cpp:314
void CCameraBird::CheckTarget(float dt) {
	if (GolfBall().m_preDataIndex >= 0 && GOLFDOC()->GameMode() != 0x1000) return;

	(!CTaskManager::Instance() ? 0 : CTaskManager::Instance()->GetCurrentTask())->GetActor("Player") << MsgObject(0, 0x124, m_playerIndex, (intptr_t)&m_playerMatrix, 0, 0, 0);

	WVector dir = m_playerMatrix.za;

	dir.y = 0.0f; dir.Normalize();

	m_pos = m_playerMatrix.pivot - dir * 3.8400002f;
	m_pos.y = GolfBall().m_pos.y + 3.8400002f;

	dir = m_pos - m_matrix.pivot;
	float dist = dir.Magnitude();
	float t = dist * 0.15625f;
	if (t > 1.0f) t = 1.0f;

	m_matrix.za = dir * t + (GolfBall().m_pos - m_matrix.pivot) * (1.0f - t);

	m_matrix.za.y = 0.0f; m_matrix.za.Normalize();
	m_matrix.za *= -1.0f;
	m_matrix.ya = WVector::UNIT_POS_Y;
	m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);

	WVector pivot = m_matrix.pivot;
	m_matrix.pivot = WVector::ZERO;

	m_matrix = RotMat(-0.95993108f, 1) * m_matrix;

	m_matrix.pivot = pivot;
	if (dist < 32.0000005) {

		if (dist > 0.5)
			m_matrix.pivot += dir * (dist * m_unk74) * dt;
	} else {


		m_matrix.za = g_camera.za * -1.0f;
		m_matrix.xa = WCrossProduct(m_matrix.ya, m_matrix.za);
		m_matrix.pivot = m_pos;
	}
}

// mascot.cpp:361
void CCameraBird::ChangePet(const char *name) {
	if (g_resrcmng && m_pPet) { g_resrcmng->Release(m_pPet, false); m_pPet = 0; }

	m_pPet = g_resrcmng->GetPuppet(name, false, true, false);

	CSceneManager::Instance()->SetPuppet(name, m_pPet);
}
