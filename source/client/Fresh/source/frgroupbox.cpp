// Original: d:\build\custom\temp\code\client\fresh\source\frgroupbox.cpp
#include "frgroupbox.h"
#include "frelement.h"
#include "frwndmanager.h"
#include "ObjectFactory.h"

// frgroupbox.cpp:16 (run-time type and factory registration of FrGroupBox)
const WRTTI FrGroupBox::m_RTTI("FrGroupBox", &FrWnd::m_RTTI);

IObject* FrGroupBoxMakeInstance(void) {
	return new FrGroupBox;
}

struct __sFrGroupBox {
	__sFrGroupBox(void) {
		ObjectFactory().AddObjectFunctor(FrGroupBoxMakeInstance, "FrGroupBox");
	}
};
static __sFrGroupBox s_registerFrGroupBox;

// frgroupbox.cpp:21
FrGroupBox::FrGroupBox(void) {
}

// frgroupbox.cpp:25
FrGroupBox::~FrGroupBox(void) {
}

// frgroupbox.cpp:29
void FrGroupBox::Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent) {
	pWndManager->GetDocument();

	// The group box covers the gui item's rectangle.
	WRect rect((float)item.m_rect.left, (float)item.m_rect.top,
		(short)(item.m_rect.right - item.m_rect.left),
		(short)(item.m_rect.bottom - item.m_rect.top));

	Create("GroupBox", item.m_name.c_str(), pWndManager, 0x841, rect, pParent);

	// Create the child items inside the group box.
	const std::list<FrGuiItem*>* pChildList = item.GetChildList();
	if (pChildList) {
		std::list<FrGuiItem*>::const_iterator it = pChildList->begin();
		std::list<FrGuiItem*>::const_iterator end = pChildList->end();
		for (; it != end; ++it)
			pWndManager->DoCreate(**it, pWndManager, this, m_pOwner);
	}
}
