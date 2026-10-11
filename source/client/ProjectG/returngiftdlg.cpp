// Original: d:\build\custom\temp\code\client\projectg\returngiftdlg.cpp
#include "ReturnGiftDlg.h"
#include "ObjectFactory.h"

// returngiftdlg.cpp:23 (run-time type and factory registration of FrReturnGiftDlg)
const WRTTI FrReturnGiftDlg::m_RTTI("FrReturnGiftDlg", &FrForm::m_RTTI);

IObject* FrReturnGiftDlgMakeInstance(void) {
	return new FrReturnGiftDlg;
}

struct __sFrReturnGiftDlg {
	__sFrReturnGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrReturnGiftDlgMakeInstance, "FrReturnGiftDlg");
	}
};
static __sFrReturnGiftDlg s_registerFrReturnGiftDlg;

// returngiftdlg.cpp:27
FrReturnGiftDlg::FrReturnGiftDlg(void) {
}
