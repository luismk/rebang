#pragma once

#include "gatewayinterface.h"

namespace _gateway
{
	class FrSpecialBoxDlg;

	class CSpecialBoxHandler : public CGatewayHandler
	{
	public:
		CSpecialBoxHandler() { }
		virtual ~CSpecialBoxHandler() { }

		virtual bool Initialize();
		virtual bool Execute(const MsgObject& msg);

	protected:
		bool OnSpecialBoxCloseCallback(int result, FrForm* pForm);

	private:
		void OnReqOpenBox();
		void OnReqWarningMsg(int param);
		bool VerifyHaveKeyItem();

		FrSpecialBoxDlg* m_pSpecialBoxDlg;
	};
}
