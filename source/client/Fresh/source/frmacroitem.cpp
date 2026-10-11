// Original: d:\build\custom\temp\code\client\fresh\source\frmacroitem.cpp
#include "frmacroitem.h"
#include "frelement.h"
#include "frwndmanager.h"

// frmacroitem.cpp:16
FrMacroItem::FrMacroItem(void) {
	m_pOwner = 0;
}

// frmacroitem.cpp:21
void FrMacroItem::SetOwner(FrCmdTarget* pOwner) {
	m_pOwner = pOwner;
}

// frmacroitem.cpp:26
void FrMacroItem::Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent) {
	FrElementDoc* pDoc = pManager->GetDocument();

	// The macro item named by the gui item's resource.
	FrElementMacroItem* pMacro = pDoc->GetMacroItem(item.m_resource);
	if (pMacro == 0) {
		return;
	}

	// Create every gui item of the macro under the parent window.
	std::list<FrGuiItem*>::iterator itEnd = pMacro->m_guiList.end();
	std::list<FrGuiItem*>::iterator it = pMacro->m_guiList.begin();

	for (; it != itEnd; ++it) {
		pManager->DoCreate(**it, pManager, pParent, m_pOwner);
	}
}
