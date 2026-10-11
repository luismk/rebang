// Original: d:\build\custom\temp\code\client\fresh\include\frgroupbox.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRGROUPBOX_H
#define PG_CLIENT_FRESH_INCLUDE_FRGROUPBOX_H

#include "..\..\defines.h"
#include "frwnd.h"

class FrGuiItem;

// A window grouping child windows. Layout and methods from the PDB (size
// 0xd4, base FrWnd, no members or virtual slots of its own).
class FrGroupBox : public FrWnd {
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frgroupbox.h:23

	FrGroupBox(void);
	virtual ~FrGroupBox(void);

	void Init(FrGuiItem& item, FrWndManager* pWndManager, FrWnd* pParent);
};

CHECK_SIZE(FrGroupBox, 0xd4);

#endif // PG_CLIENT_FRESH_INCLUDE_FRGROUPBOX_H
