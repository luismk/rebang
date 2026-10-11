// Original: d:\build\custom\temp\code\client\fresh\include\frmacroitem.h
// (no inline code of this header carries line marks; the name follows the
// unit client/Fresh/source/FrMacroItem)
#ifndef PG_CLIENT_FRESH_INCLUDE_FRMACROITEM_H
#define PG_CLIENT_FRESH_INCLUDE_FRMACROITEM_H

#include "..\..\defines.h"

class FrCmdTarget;
class FrGuiItem;
class FrWnd;
class FrWndManager;

// Creates the windows of a macro item (a named group of gui items of the
// document) for its owner. Layout, access and methods from the PDB (size 0x4).
class FrMacroItem {
	LAYOUT_CHECK_FRIEND;
public:
	FrMacroItem(void);

	void SetOwner(FrCmdTarget* pOwner);
	void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);

protected:
	FrCmdTarget* m_pOwner;    // 0x0
};

CHECK_SIZE(FrMacroItem, 0x4);
BEGIN_LAYOUT_CHECK(FrMacroItem)
	CHECK_FIELD(FrMacroItem, m_pOwner, 0x0);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRMACROITEM_H
