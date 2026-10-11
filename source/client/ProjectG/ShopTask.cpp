// Original: d:\build\custom\temp\code\client\projectg\shoptask.cpp
#include "ShopTask.h"

#include "ObjectFactory.h"
#include "actor.h"
#include "fresh.h"
#include "Packet.h"
#include "SharedDoc.h"
#include "ShopMain.h"
#include "commonutil.h"
#include "RealMyRoomData.h"
#include "../shared/Localize.h"

extern int g_iRecvPacketType;

// The class and actor names of the task's last actor (unnamed data).
extern "C" const char *data_aa7d50[2];

// shoptask.cpp:25 (run-time type and factory registration of CShopTask)
const WRTTI CShopTask::m_RTTI("CShopTask", &CTask::m_RTTI);

IObject* CShopTaskMakeInstance(void) {
	return new CShopTask;
}

struct __sCShopTask {
	__sCShopTask(void) {
		ObjectFactory().AddObjectFunctor(CShopTaskMakeInstance, "CShopTask");
	}
};
static __sCShopTask s_registerCShopTask;

// shoptask.cpp:28
CShopTask::CShopTask(void) {
	IActor::m_defaultSkipLayer = 0;
	IActor::m_pLayer = 0;
}

// shoptask.cpp:34
CShopTask::~CShopTask(void) {
}

// shoptask.cpp:42
void CShopTask::Init(const char *backGround) {
	CTask::Init("shop_background.jpg");

	SetWhisper(false);
}

// shoptask.cpp:52
void CShopTask::Register(void) {
	SetMainActor(AddActor("CShopMain", "Shop", 0, ""));

	AddActor(data_aa7d50[0], data_aa7d50[1], 0, GetRTTI()->GetName());

	if (AfxGetPreservedTask()) {
		IActor *pTutorial = AfxGetPreservedTask()->GetActor("TutorialMain");
		if (pTutorial) {
			const char *className = 0;
			const char *name = 0;

			pTutorial << MsgObject(0, 0x14E, (intptr_t)&className, (intptr_t)&name, 0, 0, 0);
			AddActor(className, name, 0, "");
		}
	}
}

// shoptask.cpp:77
void CShopTask::Load(void) {
	SetProgressBar(false);
	SetTip(false);

	if (IsLocalContent((localContentType_t)0x56) && IsLocalContent((localContentType_t)0x61)) {
		WSendPacket packet((enumClientPacket)0xAF);
		packet.Encode4(MyUID());
		packet.Encode1(0);

		packet.Send(SEND_TO_DEFAULT);

		if (IsLocalContent((localContentType_t)0x66)) {
			WSendPacket packet2((enumClientPacket)0xC4);
			packet2.Encode4(MyUID());

			packet2.Send(SEND_TO_DEFAULT);
		}
	}

	CTask::Load();
}

// shoptask.cpp:103
void CShopTask::Process(float dt) {
	g_pFresh->OnProcess(dt);
	CTask::Process(dt);
}

// shoptask.cpp:109
void CShopTask::Display(void) {
	if (!WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff)
		CTask::Display();

	g_pFresh->OnDisplay(WSingleton<CSharedDoc>::Instance()->m_3DBackgroundOff != 0);
}

// shoptask.cpp:117
// FIRST PASS: the dispatch and most cases are written; the remaining cases
// carry the line range and calls of the original.
int CShopTask::OnPacket(WReceivedPacket &packet) {
	g_iRecvPacketType = packet.Decode2();

	switch (g_iRecvPacketType) {
	case 0xB6: {
		packet.Decode4();

		unsigned char count = packet.Decode1();
		count = count < 0x80 ? count : 0x80;

		unsigned long uids[0x80];
		for (int i = 0; i < count; i++)
			uids[i] = packet.Decode4();

		PassMsgToActor("Shop", MsgObject(0, 0xD5, (intptr_t)uids, count, 0, 0, 0));
		break;
	}

	case 0x66:
		GetActor("Shop") << MsgObject(0, 2, 0, 0, 0, 0, 0);
		GetActor("Shop") << MsgObject(0, 0xC0, 1, 0, 0, 0, 0);

		switch (packet.Decode4()) {
		// shoptask.cpp:160
		case 0x2F:
			{
				unsigned long typeId = packet.Decode4();
				unsigned long serial = packet.Decode4();
				unsigned long count = packet.Decode4();
				unsigned long bonus = packet.Decode4();

				std::list<sItemInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_itemInfoList.begin();
				for (; it != WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end(); ++it) {
					if (it->m_serial == serial && it->m_typeId == typeId) {
						it->m_count = (short)count;
						break;
					}
				}

				if (it == WSingleton<CSharedDoc>::Instance()->m_itemInfoList.end()) {
					// TODO (180-186): `sItemInfo item;` zeroed by `rep stosd` (0x2A
					// dwords: memset or an inline sItemInfo ctor?), m_serial = serial,
					// m_typeId = typeId, flag byte 0x28 |= 2 (no member in sItemInfo),
					// m_count = count, m_itemInfoList.push_back(item).
				}

				// "Purchase completed.\nYou received bonus %s x%d."
				GetActor("Shop") << MsgObject(0, 0x241, (intptr_t)MakeStr("\xc1\xa4\xbb\xf3\xc0\xfb\xc0\xb8\xb7\xce \xb1\xb8\xb8\xc5\xb0\xa1 \xbf\xcf\xb7\xe1\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.\n\xba\xb8\xb3\xca\xbd\xba %s %d\xb0\xb3\xb8\xa6 \xb9\xde\xc0\xb8\xbc\xcc\xbd\xc0\xb4\xcf\xb4\xd9.", ItemManager()->GetItemName(typeId), bonus), 0, 0, 0, 0);
				break;
			}

		// shoptask.cpp:199
		case 0x0:
			{
				WSingleton<CSharedDoc>::Instance()->m_myPang = packet.Decode8();

				// TODO (201): WSingleton<CSharedDoc>::Instance()->m_myBonusCash = packet.Decode8();
				// (m_myBonusCash 0x1298 is protected in SharedDoc.h: needs a friend or setter)
				WSingleton<CSharedDoc>::Instance()->RefreshAllCookie();

				// "Purchase completed."
				GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc1\xa4\xbb\xf3\xc0\xfb\xc0\xb8\xb7\xce \xb1\xb8\xb8\xc5\xb0\xa1 \xbf\xcf\xb7\xe1\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);

				if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CShopTask::m_RTTI)) {
					CShopMain *pShop = (CShopMain*)AfxGetTask()->GetActor("Shop");
					if (pShop) {
						if (!pShop->IsActiveRecycleDlg())
							pShop << MsgObject(0, 0x6A, 0, 0, 0, 0, 0);
					}
				}
				break;
			}

		// shoptask.cpp:229
		case 0x2:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xba\xb8\xc0\xaf\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xc6\xce\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:233
		case 0x3:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xdf\xb8\xf8\xb5\xc8 \xbb\xf3\xc7\xb0 \xc4\xda\xb5\xe5\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:237
		case 0x1:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb1\xb8\xb8\xc5\xb0\xa1 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:245
		case 0x4:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xcc\xb9\xcc \xb1\xb8\xb8\xc5\xc7\xd1 \xb9\xb0\xb0\xc7\xc0\xd4\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:248
		case 0x28:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xcc\xb9\xcc \xbf\xec\xc6\xed\xc7\xd4\xbf\xa1 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:251
		case 0x29:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc1\xf6\xb1\xdd \xc6\xc7\xb8\xc5\xc1\xdf\xc0\xce \xbb\xf3\xc7\xb0\xc0\xd4\xb4\xcf\xb4\xd9. \xc6\xc7\xb8\xc5 \xbf\xcf\xb7\xe1\xb5\xc8 \xb5\xda\xbf\xa1 \xb1\xb8\xc0\xd4\xc7\xcf\xbd\xc7 \xbc\xf6 \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:254
		case 0x2B:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xcc\xb9\xcc \xb5\xb9\xc7\xc7\xb4\xcf\xc0\xc7 \xba\xf1\xb9\xd0\xb1\xdd\xb0\xed\xbf\xa1 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:260
		case 0xB:
			{
				_SYSTEMTIME serverTime;
				packet.DecodeBuffer(&serverTime, 0x10);
				WSingleton<CSharedDoc>::Instance()->SetServerTime(serverTime);

				GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc6\xc7\xb8\xc5\xb1\xe2\xb0\xa3\xc0\xbb \xc8\xae\xc0\xce\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
				break;
			}

		// shoptask.cpp:269
		case 0x9:
			{
				_SYSTEMTIME serverTime;
				packet.DecodeBuffer(&serverTime, 0x10);
				WSingleton<CSharedDoc>::Instance()->SetServerTime(serverTime);

				GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xb1\xe2\xb0\xa3\xc0\xbb \xc8\xae\xc0\xce\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
				break;
			}

		// shoptask.cpp:276
		case 0x10:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb1\xe2\xb0\xa3\xc1\xa6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xbb\xe7\xbf\xeb\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:280
		case 0x11:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb0\xb9\xbc\xf6\xc1\xa6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xbb\xe7\xbf\xeb\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:284
		case 0x15:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xd1\xb9\xf8\xbf\xa1 \xb3\xca\xb9\xab \xb8\xb9\xc0\xba \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb1\xb8\xc0\xd4\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:288
		case 0x17:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xba\xb8\xc0\xaf\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xc4\xed\xc5\xb0\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:292
		case 0x18:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc4\xed\xc5\xb0\xb8\xa6 \xbe\xf7\xb5\xa5\xc0\xcc\xc6\xae\xc7\xcf\xb4\xc2\xb5\xa5 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:295
		case 0x12:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb4\xf5 \xc0\xcc\xbb\xf3 \xb1\xb8\xc0\xd4\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:298
		case 0x23:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xc1\xa1\xc0\xcc \xc8\xa5\xc0\xe2\xc7\xcf\xbf\xa9 \xc1\xf6\xb1\xdd\xc0\xba \xb1\xb8\xc0\xd4\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9. \xc0\xe1\xbd\xc3 \xc8\xc4 \xc0\xcc\xbf\xeb\xc7\xd8\xc1\xd6\xbd\xc3\xb1\xe2 \xb9\xd9\xb6\xf8\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:301
		case 0x13:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc6\xc7\xb8\xc5\xc7\xcf\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc \xbe\xc6\xb4\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:304
		case 0x2E:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc3\xa4\xb3\xce\xb8\xb5\xbc\xad\xba\xf1\xbd\xba\xb0\xa1 \xc1\xbe\xb7\xe1\xb5\xc7\xbe\xee \xb4\xf5\xc0\xcc\xbb\xf3 \xb1\xb8\xb8\xc5\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:307
		case 0x22:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xcc \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb4\xf5 \xc0\xcc\xbb\xf3 \xba\xb8\xc0\xaf\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:311
		case 0x2C:
		case 0x2D:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xf6\xc0\xe7 \xb7\xb9\xba\xa7\xb7\xce\xb4\xc2 \xb1\xb8\xc0\xd4\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:314
		case 0x35:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xf6\xc0\xe7 \xb8\xf0\xb5\xe7 \xc4\xb3\xbd\xac\xbf\xa1 \xb4\xeb\xc7\xd1 \xc1\xb6\xc8\xb8 \xb9\xd7 \xbb\xe7\xbf\xeb\xc0\xcc \xc1\xa4\xc1\xf6\xb5\xc7\xbe\xee \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;
		}
		break;

	case 0x68:
		GetActor("Shop") << MsgObject(0, 2, 0, 0, 0, 0, 0);
		GetActor("Shop") << MsgObject(0, 0xC0, 1, 0, 0, 0, 0);

		switch (packet.Decode4()) {
		case 0x0:
			// TODO (334-381), left whole (a partial body scores lower: the missing
			// WSendPacket locals drop the EH frame): m_myPang = Decode8();
			// m_myBonusCash = Decode8() (protected); RefreshAllCookie();
			// GetActor("Shop") << MsgObject(0, 0x23, "\xbc\xb1\xb9\xb0\xc7\xcf\xb1\xe2\xb0\xa1 \xbf\xcf\xb7\xe1\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9");
			// if (IsLocalContent(0x60)) { if (WNetworkSystem IsConnected(1))
			// { WSendPacket p(0x27); Encode4(m_myUID); Encode4(CSharedDoc+0x4DB8); Send(1); } }
			// else if (IsConnected(1)) { WSendPacket p(0x21); Encode4(m_myUID);
			// Encode4(CSharedDoc+0x4DB8); Send(1); CSharedDoc std::string 0x4D9C = "";
			// CSharedDoc+0x4DB8 = -1; } (no members for 0x4D9C / 0x4DB8 in SharedDoc.h)
			break;

		// shoptask.cpp:384
		case 0x2:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb1\xdd\xbe\xd7\xc0\xcc \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:388
		case 0x3:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xdf\xb8\xf8\xb5\xc8 \xbb\xf3\xc7\xb0 \xc4\xda\xb5\xe5\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:392
		case 0x1:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc7\xcf\xb1\xe2\xb0\xa1 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:400
		case 0x4:
		case 0x24:
		case 0x25:
		case 0x26:
		case 0x27:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xcc \xc0\xcc\xb9\xcc \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:403
		case 0x28:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xcc \xc0\xcc\xb9\xcc \xbf\xec\xc6\xed\xc7\xd4\xbf\xa1 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:406
		case 0x29:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xcc \xc7\xf6\xc0\xe7 \xc6\xc7\xb8\xc5\xc1\xdf\xc0\xce \xbb\xf3\xc7\xb0\xc0\xd4\xb4\xcf\xb4\xd9. \xc6\xc7\xb8\xc5 \xbf\xcf\xb7\xe1\xb5\xc8 \xb5\xda\xbf\xa1 \xbc\xb1\xb9\xb0\xc7\xcf\xb1\xe2\xb0\xa1 \xb0\xa1\xb4\xc9\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:414
		case 0x2B:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xcc\xb9\xcc \xb5\xb9\xc7\xc7\xb4\xcf\xc0\xc7 \xba\xf1\xb9\xd0\xb1\xdd\xb0\xed\xbf\xa1 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:419
		case 0x8:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc7\xcf\xb1\xe2\xb8\xa6 \xc7\xd2 \xbc\xf6 \xbe\xf8\xb4\xc2 \xb0\xe8\xc1\xa4\xc0\xd4\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:423
		case 0x7:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc1\xd6\xb9\xce\xb9\xf8\xc8\xa3\xb0\xa1 \xc0\xdf\xb8\xf8 \xc0\xd4\xb7\xc2\xb5\xc7\xbe\xfa\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:429
		case 0xB:
			{
				_SYSTEMTIME serverTime;
				packet.DecodeBuffer(&serverTime, 0x10);
				WSingleton<CSharedDoc>::Instance()->SetServerTime(serverTime);

				GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc6\xc7\xb8\xc5\xb1\xe2\xb0\xa3\xc0\xbb \xc8\xae\xc0\xce\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
				break;
			}

		// shoptask.cpp:438
		case 0x9:
			{
				_SYSTEMTIME serverTime;
				packet.DecodeBuffer(&serverTime, 0x10);
				WSingleton<CSharedDoc>::Instance()->SetServerTime(serverTime);

				GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xc6\xc0\xcc\xc5\xdb\xc0\xc7 \xb1\xe2\xb0\xa3\xc0\xbb \xc8\xae\xc0\xce\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4.", 0, 0, 0, 0);
				break;
			}

		// shoptask.cpp:445
		case 0xC:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xbf\xa1\xb0\xd4 \xc7\xd8\xb4\xe7 \xc4\xb3\xb5\xf0\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:449
		case 0xE:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xbf\xa1\xb0\xd4 \xc7\xd8\xb4\xe7 \xc4\xb3\xb8\xaf\xc5\xcd\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:453
		case 0x10:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xcc \xb1\xe2\xb0\xa3\xc1\xa6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xbb\xe7\xbf\xeb\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:457
		case 0x11:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xcc \xb0\xb9\xbc\xf6\xc1\xa6 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xbb\xe7\xbf\xeb\xc1\xdf\xc0\xd4\xb4\xcf\xb4\xd9", 0, 0, 0, 0);
			break;

		// shoptask.cpp:461
		case 0x12:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xc6\xc0\xcc\xc5\xdb \xba\xb8\xc0\xaf \xc7\xd1\xb5\xb5\xb8\xa6 \xc3\xca\xb0\xfa\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:465
		case 0x15:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xd1\xb9\xf8\xbf\xa1 \xb3\xca\xb9\xab \xb8\xb9\xc0\xba \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xbb \xb1\xb8\xc0\xd4\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:469
		case 0x17:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xba\xb8\xc0\xaf\xc7\xcf\xb0\xed \xc0\xd6\xb4\xc2 \xc4\xed\xc5\xb0\xb0\xa1 \xba\xce\xc1\xb7\xc7\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:473
		case 0x18:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc4\xed\xc5\xb0\xc1\xa4\xba\xb8\xb8\xa6 \xbe\xf7\xb5\xa5\xc0\xcc\xc6\xae\xc7\xcf\xc1\xf6 \xb8\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:477
		case 0x1C:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb1\xb8\xc0\xd4\xc7\xcf\xbd\xc5 \xbc\xb1\xb9\xb0\xc0\xbb \xc0\xfa\xc0\xe5\xc7\xcf\xc1\xf6 \xb8\xf8\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:480
		case 0x1E:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xc7 \xc1\xa4\xba\xb8\xb8\xa6 \xc1\xb6\xc8\xb8\xc7\xcf\xb4\xc2\xb5\xa5 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:483
		case 0x1F:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xb1\xb8\xc0\xd4\xc7\xcf\xb7\xc1\xb0\xed \xc7\xcf\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:486
		case 0x20:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xc5\xc3\xc7\xcf\xbd\xc5 \xc4\xed\xc6\xf9\xc0\xbb \xbb\xe7\xbf\xeb\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9..", 0, 0, 0, 0);
			break;

		// shoptask.cpp:489
		case 0x21:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbb\xf3\xb4\xeb\xb9\xe6\xc0\xc7 \xbc\xb1\xb9\xb0\xc7\xd4\xc0\xcc \xb0\xa1\xb5\xe6 \xc2\xf7 \xb4\xf5 \xc0\xcc\xbb\xf3 \xbc\xb1\xb9\xb0\xc0\xbb \xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:492
		case 0x13:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc6\xc7\xb8\xc5\xc7\xcf\xb4\xc2 \xbe\xc6\xc0\xcc\xc5\xdb\xc0\xcc \xbe\xc6\xb4\xd5\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:495
		case 0x19:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc4\xed\xc5\xb0\xb8\xa6 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xb4\xd9\xb4\xc2 \xc1\xa4\xba\xb8\xb8\xa6 \xc3\xa3\xc0\xbb \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:498
		case 0x2E:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc3\xa4\xb3\xce\xb8\xb5\xbc\xad\xba\xf1\xbd\xba\xb0\xa1 \xc1\xbe\xb7\xe1\xb5\xc7\xbe\xee \xb4\xf5\xc0\xcc\xbb\xf3 \xb1\xb8\xb8\xc5\xc7\xcf\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9..", 0, 0, 0, 0);
			break;

		// shoptask.cpp:501
		case 0x22:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc0\xbb \xc7\xcf\xb7\xc1\xb4\xc2 \xc0\xaf\xc0\xfa\xb4\xc2 \xc0\xcc \xbc\xb1\xb9\xb0\xc0\xbb \xb9\xde\xc0\xb8\xbd\xc7 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:504
		case 0x2C:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc0\xbb \xb9\xde\xb4\xc2 \xc0\xaf\xc0\xfa\xc0\xc7 \xb7\xb9\xba\xa7\xc0\xcc \xb3\xb7\xbe\xc6 \xbc\xb1\xb9\xb0\xc7\xd2\xbc\xf6\xb0\xa1 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:507
		case 0x2D:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc0\xbb \xb9\xde\xb4\xc2 \xc0\xaf\xc0\xfa\xc0\xc7 \xb7\xb9\xba\xa7\xc0\xcc \xb3\xf4\xbe\xc6 \xbc\xb1\xb9\xb0 \xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:510
		case 0x34:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc0\xda\xb1\xe2 \xc0\xda\xbd\xc5\xbf\xa1\xb0\xd4\xb4\xc2 \xbc\xb1\xb9\xb0\xc0\xbb \xba\xb8\xb3\xbe \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:513
		case 0x33:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbc\xb1\xb9\xb0\xc7\xd2 \xc0\xaf\xc0\xfa\xc0\xc7 \xc1\xa4\xba\xb8\xb8\xa6 \xc3\xa3\xc1\xf6 \xb8\xf8\xc7\xdf\xbd\xc0\xb4\xcf\xb4\xd9. \xb4\xd9\xbd\xc3 \xbd\xc3\xb5\xb5\xc7\xd8 \xc1\xd6\xbc\xbc\xbf\xe4", 0, 0, 0, 0);
			break;

		// shoptask.cpp:516
		case 0x35:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xf6\xc0\xe7 \xb8\xf0\xb5\xe7 \xc4\xb3\xbd\xac\xbf\xa1 \xb4\xeb\xc7\xd1 \xc1\xb6\xc8\xb8 \xb9\xd7 \xbb\xe7\xbf\xeb\xc0\xcc \xc1\xa4\xc1\xf6\xb5\xc7\xbe\xee \xc0\xd6\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;
		}
		break;

	case 0xB4:
		GetActor("Shop") << MsgObject(0, 0xC1, packet.Decode4(), 0, 0, 0, 0);
		break;

	case 0xAE:
		break;

	case 0x105:
		if (packet.Decode1() == 0)
			PassMsgToActor("Shop", MsgObject(0, 0x1FF, 0, 0, 0, 0, 0));
		break;

	case 0x125:
		if (IsLocalContent((localContentType_t)0x56)) {
			unsigned long result = packet.Decode4();
			if (result == 1) {
				int count = packet.Decode2();
				for (int i = 0; i < count; i++) {
					sFurniture_List furniture;
					memset(&furniture, 0, sizeof(furniture));
					packet.DecodeBuffer(&furniture, 0x1B);

					std::list<sFurniture_List>::iterator it;
					for (it = WSingleton<CSharedDoc>::Instance()->m_furnitureList.begin(); it != WSingleton<CSharedDoc>::Instance()->m_furnitureList.end(); ++it) {
						if (it->m_id == furniture.m_id)
							break;
					}

					if (it == WSingleton<CSharedDoc>::Instance()->m_furnitureList.end())
						WSingleton<CSharedDoc>::Instance()->AddFurniture(furniture);
				}
			} else {
				AfxGetTask()->GetActor("RealMyRoom") << MsgObject(0, 0x228, 0x125, result, 0, 0, 0);
			}
		}
		break;

	case 0x199:
		AfxGetTask()->SendMsgToMainActor(MsgObject(0, 2, 0, 0, 0, 0, 0));

		switch (packet.Decode4()) {
		// shoptask.cpp:596
		case 0x0: {
			unsigned long caddieId = packet.Decode4();
			unsigned long equipped = packet.Decode4();
			sCaddieInfo caddie;
			packet.DecodeBuffer(&caddie, 0x19);

			std::map<unsigned int, sCaddieInfo>::iterator it = WSingleton<CSharedDoc>::Instance()->m_caddieMap.find(caddieId);
			if (it != WSingleton<CSharedDoc>::Instance()->m_caddieMap.end())
				WSingleton<CSharedDoc>::Instance()->m_caddieMap.erase(it);

			WSingleton<CSharedDoc>::Instance()->AddCaddie(&caddie);
			WSingleton<CSharedDoc>::Instance()->m_myCaddieId = equipped;
			// "Upgrade succeeded."
			AfxGetTask()->GetActor("Shop") << MsgObject(0, 0x24F, (intptr_t)"\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbc\xba\xb0\xf8\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", caddie.typeId, 0, 0, 0);
			break;
		}

		// shoptask.cpp:613
		case 0x1:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xc7\xd8\xb4\xe7 \xc4\xb3\xb5\xf0\xb8\xa6 \xb0\xa1\xc1\xf6\xb0\xed \xc0\xd6\xc1\xf6 \xbe\xca\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:618
		case 0x2:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xf7\xb1\xd7\xb7\xb9\xc0\xcc\xb5\xe5\xb8\xa6 \xbd\xc7\xc6\xd0\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;

		// shoptask.cpp:623
		case 0x3:
			GetActor("Shop") << MsgObject(0, 0x23, (intptr_t)"\xbe\xcb\xbc\xf6\xbe\xf8\xb4\xc2 \xbf\xc0\xb7\xf9\xb0\xa1 \xb9\xdf\xbb\xfd\xc7\xcf\xbf\xb4\xbd\xc0\xb4\xcf\xb4\xd9.", 0, 0, 0, 0);
			break;
		}
		// falls through: the original passes the packet on afterwards

	default:
		return OnPacketCommon(packet);
	}

	return 1;
}
