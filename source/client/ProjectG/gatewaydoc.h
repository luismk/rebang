// Original: d:\build\custom\temp\code\client\projectg\gatewaydoc.h
#ifndef PG_CLIENT_PROJECTG_GATEWAYDOC_H
#define PG_CLIENT_PROJECTG_GATEWAYDOC_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "icontentsdoc.h"

#include <vector>

// Enumerators not recovered yet.
enum eSecondPasswordState {
};

namespace _gateway {

class CGatewayHandler;

// The document of the second-password content. Its handler (a
// CSecondaryPassword) is created by Initialize and deleted by Release.
class CSecondPwdDoc : public IContentsDataContainer {
public:
	CSecondPwdDoc(void);
	// gatewaydoc.h:36
	virtual ~CSecondPwdDoc(void) {}

	virtual bool Initialize(void);
	virtual bool Release(void);

	// gatewaydoc.h:43
	virtual CGatewayHandler* GetGatewayHandler(void) { return m_pGatewayHandler; }

	void SetPasswordState(eSecondPasswordState state); // gatewaydoc.h:45
	eSecondPasswordState GetPasswordState(void); // gatewaydoc.h:46
	void SetRemainDay(int remainDay); // gatewaydoc.h:48
	int GetRemainDay(void); // gatewaydoc.h:49
	void IncreseInvalidInputPassword(void); // gatewaydoc.h:51
	void SetSecondPasswordConfirm(void); // gatewaydoc.h:54
	bool GetSecondPasswordConfirm(void); // gatewaydoc.h:55
	void SetCertifyFlag(void); // gatewaydoc.h:57
	bool GetCertifyFlag(void); // gatewaydoc.h:58

	void InitializeForLogout(void);

	CGatewayHandler* m_pGatewayHandler; // 0x4
	eSecondPasswordState m_ePasswordState; // 0x8
	int m_iRemainDay; // 0xC
	int m_iInvalidInputCount; // 0x10
	bool m_bSecondPasswordConfirm; // 0x14
	bool m_bCertifyFlag; // 0x15
};

CHECK_FIELD(CSecondPwdDoc, m_pGatewayHandler, 0x4);
CHECK_FIELD(CSecondPwdDoc, m_ePasswordState, 0x8);
CHECK_FIELD(CSecondPwdDoc, m_iRemainDay, 0xC);
CHECK_FIELD(CSecondPwdDoc, m_iInvalidInputCount, 0x10);
CHECK_FIELD(CSecondPwdDoc, m_bSecondPasswordConfirm, 0x14);
CHECK_FIELD(CSecondPwdDoc, m_bCertifyFlag, 0x15);
CHECK_SIZE(CSecondPwdDoc, 0x18);

// The document of the golf-item content: its handler (a CGolfItemHandler),
// the calipers count and the usable-item list.
class CGolfItemDoc : public IContentsDataContainer {
public:
	CGolfItemDoc(void);
	// gatewaydoc.h:87
	virtual ~CGolfItemDoc(void) {}

	virtual bool Initialize(void);
	virtual bool Release(void);

	// gatewaydoc.h:92
	virtual CGatewayHandler* GetGatewayHandler(void) { return m_pGatewayHandler; }

	void ResetUsableItemList(void);
	void ResetCalipersVariable(void);

	// gatewaydoc.h:98
	void SetCalipersCount(int count) { m_iCalipersCount = count; }
	// gatewaydoc.h:99
	int GetCalipersCount(void) const { return m_iCalipersCount; }
	void DeceaseCalipersCount(void);
	// gatewaydoc.h:101
	void EnableCalipers(bool bEnable) { m_usableItemList[1] = bEnable == true; }
	// gatewaydoc.h:102
	bool GetEnableCalipers(void) const { return m_usableItemList[1] == 1 ? true : false; }

	// gatewaydoc.h:104
	void EnableTimeBooster(bool bEnable) { m_usableItemList[0] = bEnable == true; }
	// gatewaydoc.h:105
	bool GetEnableTimeBooster(void) const { return m_usableItemList[0] == 1 ? true : false; }

	// gatewaydoc.h:107
	void IncreaseSpecialBoxCount(void) { ++m_iSpecialBoxCount; }
	// gatewaydoc.h:108
	void ResetSpecialBoxCount(void) { m_iSpecialBoxCount = 0; }
	// gatewaydoc.h:109
	int GetSpecialBoxCount(void) const { return m_iSpecialBoxCount; }

	// gatewaydoc.h:111
	void SetOwnSpecialBoxCount(int count) { m_iOwnSpecialBoxCount = count; }
	// gatewaydoc.h:112
	int GetOwnSpecialBoxCount(void) const { return m_iOwnSpecialBoxCount; }

	CGatewayHandler* m_pGatewayHandler; // 0x4
	int m_iCalipersCount; // 0x8
	std::vector<int> m_usableItemList; // 0xC
	int m_iSpecialBoxCount; // 0x1C
	int m_iOwnSpecialBoxCount; // 0x20
};

CHECK_FIELD(CGolfItemDoc, m_pGatewayHandler, 0x4);
CHECK_FIELD(CGolfItemDoc, m_iCalipersCount, 0x8);
CHECK_FIELD(CGolfItemDoc, m_usableItemList, 0xC);
CHECK_FIELD(CGolfItemDoc, m_iSpecialBoxCount, 0x1C);
CHECK_FIELD(CGolfItemDoc, m_iOwnSpecialBoxCount, 0x20);
CHECK_SIZE(CGolfItemDoc, 0x24);

// The document of the trade content: its handler (a CTradeHandler).
class CTradeDoc : public IContentsDataContainer {
public:
	CTradeDoc(void);
	virtual ~CTradeDoc(void);

	virtual bool Initialize(void);
	virtual bool Release(void);

	// gatewaydoc.h:140
	virtual CGatewayHandler* GetGatewayHandler(void) { return m_pGatewayHandler; }

	CGatewayHandler* m_pGatewayHandler; // 0x4
	bool m_bUnidentified; // 0x8 (cleared by the constructor; no reader decompiled yet)
};

CHECK_FIELD(CTradeDoc, m_pGatewayHandler, 0x4);
CHECK_FIELD(CTradeDoc, m_bUnidentified, 0x8);
CHECK_SIZE(CTradeDoc, 0xC);

// The document of the special-box content: its handler (a
// CSpecialBoxHandler).
class CSpecialBoxDoc : public IContentsDataContainer {
public:
	virtual bool Initialize(void);
	virtual bool Release(void);
	// gatewaydoc.h:161
	virtual CGatewayHandler* GetGatewayHandler(void) { return m_pGatewayHandler; }

	CGatewayHandler* m_pGatewayHandler; // 0x4
};

CHECK_FIELD(CSpecialBoxDoc, m_pGatewayHandler, 0x4);

}

#endif // PG_CLIENT_PROJECTG_GATEWAYDOC_H
