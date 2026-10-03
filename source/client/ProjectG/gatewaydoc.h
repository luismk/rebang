#pragma once

#include <vector>

#include "contentsdoc.h"
#include "gatewayinterface.h"

enum eSecondPasswordState;
class FrNoticeDlg;

namespace _gateway
{
	class FrExplanePassword;
	class FrPasswordInputDlg;
	class FrSelectCertification;
	class FrPasswordNotifyDlg;
	class CGatewayControlDoc : public IContentsDataContainer
	{
	public:
		CGatewayControlDoc();
		virtual ~CGatewayControlDoc();

		virtual CGatewayHandler* GetGatewayHandler() { return NULL; }
	};

	class CSecondPwdDoc : public CGatewayControlDoc
	{
	public:
		CSecondPwdDoc();
		virtual ~CSecondPwdDoc() { }

		virtual bool Initialize();
		virtual bool Release();

		void InitializeForLogout();

		virtual CGatewayHandler* GetGatewayHandler() { return m_pHandler; }

		void SetPasswordState(eSecondPasswordState state)
		{
			m_passwordState = state;
		}
		eSecondPasswordState GetPasswordState() { return m_passwordState; }

		void SetRemainDay(int day) { m_remainDay = day; }
		int GetRemainDay() { return m_remainDay; }

		void IncreseInvalidInputPassword() { ++m_invalidInputCount; }

		void SetSecondPasswordConfirm() { m_bSecondPasswordConfirm = true; }
		bool GetSecondPasswordConfirm() { return m_bSecondPasswordConfirm; }

		void SetCertifyFlag() { m_bCertifyFlag = true; }
		bool GetCertifyFlag() { return m_bCertifyFlag; }

	private:
		CGatewayHandler* m_pHandler;
		eSecondPasswordState m_passwordState;
		int m_remainDay;
		int m_invalidInputCount;
		bool m_bSecondPasswordConfirm;
		bool m_bCertifyFlag;
	};

	class CGolfItemDoc : public CGatewayControlDoc
	{
	public:
		CGolfItemDoc();
		virtual ~CGolfItemDoc() { }

		virtual bool Initialize();
		virtual bool Release();

		virtual CGatewayHandler* GetGatewayHandler() { return m_pHandler; }

		void DeceaseCalipersCount();
		void ResetUsableItemList();
		void ResetCalipersVariable();

		void SetCalipersCount(int count) { m_calipersCount = count; }
		int GetCalipersCount() const { return m_calipersCount; }

		void EnableCalipers(bool bEnable)
		{
			m_usableItemList[1] = (bEnable == true);
		}
		bool GetEnableCalipers() const
		{
			return m_usableItemList[1] == 1 ? true : false;
		}

		void EnableTimeBooster(bool bEnable)
		{
			m_usableItemList[0] = (bEnable == true);
		}
		bool GetEnableTimeBooster() const
		{
			return m_usableItemList[0] == 1 ? true : false;
		}

		void IncreaseSpecialBoxCount() { ++m_specialBoxCount; }
		void ResetSpecialBoxCount() { m_specialBoxCount = 0; }
		int GetSpecialBoxCount() const { return m_specialBoxCount; }

		void SetOwnSpecialBoxCount(int count) { m_ownSpecialBoxCount = count; }
		int GetOwnSpecialBoxCount() const { return m_ownSpecialBoxCount; }

	private:
		CGatewayHandler* m_pHandler;
		int m_calipersCount;
		std::vector<int> m_usableItemList;
		int m_specialBoxCount;
		int m_ownSpecialBoxCount;
	};

	class CTradeDoc : public CGatewayControlDoc
	{
	public:
		CTradeDoc();
		virtual ~CTradeDoc();

		virtual bool Initialize();
		virtual bool Release();

		void SetPackageSale(bool bPackageSale)
		{
			m_bPackageSale = bPackageSale;
		}
		bool GetPackageSale() const { return m_bPackageSale; }

		virtual CGatewayHandler* GetGatewayHandler() { return m_pHandler; }

	private:
		CGatewayHandler* m_pHandler;
		bool m_bPackageSale;
	};

	class CSpecialBoxDoc : public CGatewayControlDoc
	{
	public:
		virtual ~CSpecialBoxDoc() { }

		virtual bool Initialize();
		virtual bool Release();

		virtual CGatewayHandler* GetGatewayHandler() { return m_pHandler; }

	private:
		CGatewayHandler* m_pHandler;
	};

	class CSecondaryPassword : public CGatewayHandler
	{
	public:
		CSecondaryPassword();
		virtual ~CSecondaryPassword();

		virtual bool Initialize();
		virtual bool Execute(const MsgObject& msg);

		bool OnPasswordExplaneFormResult(int result, FrForm* pForm);
		bool OnInputPasswordFormResult(int result, FrForm* pForm);
		bool OnSelectCertifyFormResult(int result, FrForm* pForm);
		bool OnSystemMsgFormResult(int result, FrForm* pForm);

	private:
		void OnClosePasswordDlg();
		void OnCloseSelectCertifyDlg();
		void OnWaitGameServerList();
		void OnRequestStoreCertifyFlag(int param);
		void OnOpenSelfCertifyWebPage();
		void OnNotifyPasswordInfo(int param);
		void OnRequestAvailableContents(int param);
		void OnNotifyCannotUseContents();
		void OnNotifySecondPasswordConfirm();
		void OnRequestQualifyLogin(int param);
		void OnRequestRegistPassword(int param);
		void OnOpenPasswordExplaneDlg();
		void OnOpenPasswordCreateDlg();
		void OnOpenPasswordLoginDlg();
		void OnOpenSelectCertifyDlg();
		void OnRequestLoginPassword(int param);
		void OnOpenSystemMsg(int msg);
		void OnOpenSystemMsg(int msg, const char* text);
		void OnResultRegistPassword(int param);
		void OnRequestCheckCertifyFlag();
		void OnResultLoginPassword(int param);
		void OnRequestCheckPasswordValid(int param);
		void OnNotifySystemMsg(int param);

		FrExplanePassword* m_pExplanePasswordDlg;
		FrPasswordInputDlg* m_pPasswordInputDlg;
		FrSelectCertification* m_pSelectCertifyDlg;
		FrPasswordNotifyDlg* m_pPasswordNotifyDlg;
	};

	class CGolfItemHandler : public CGatewayHandler
	{
	public:
		CGolfItemHandler();
		virtual ~CGolfItemHandler();

		virtual bool Initialize();
		virtual bool Execute(const MsgObject& msg);

	protected:
		bool OnCloseWizCityNoticeResult(int result, FrForm* pForm);

	private:
		int CalcRemainMoreGetSpecialBoxCount(int count);
		void OnRequestResetCalipers();
		void OnGetCountCalipers(int param);
		void OnSetCountCalipers(int param);
		void OnDecreaseCalipers();
		void OnIncSpecialBox();
		void OnGetSpecialBox(int param);
		void OnResetSpecialBox();
		void OnCheckSomeMoreGetSpecialBox();
		void OnRequestWizCityNoticeDlg();
		void OnGetFlagCalipers(int param);
		void OnSetFlagCalipersEnable();
		void OnSetFlagCalipersDisable();
		void OnGetEnableTimeBooster(int param);
		void OnSetEnableItemTID(int param);

		FrNoticeDlg* m_pWizCityNoticeDlg;
	};

	class CTradeHandler : public CGatewayHandler
	{
	public:
		CTradeHandler();
		virtual ~CTradeHandler();

		virtual bool Initialize();
		virtual bool Execute(const MsgObject& msg);

	private:
		void OnSetEnablePackageSale();
		void OnSetDisablePackageSale();
		void OnGetPackageSale(int param);
		void OnNotifyTradeErrorCode(int param);
		void OnUpdateTradeMinusItem(int param);
		void OnUpdateTradeSellItem(int param);
		void OnUpdateTradeBuyItem(int param);
	};
}
