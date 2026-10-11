// Original: d:\build\custom\temp\code\client\projectg\tooltip.h
#ifndef PG_CLIENT_PROJECTG_TOOLTIP_H
#define PG_CLIENT_PROJECTG_TOOLTIP_H

#include "..\..\defines.h"

class Bitmap;

// Icon bar button; only the enumerators the transcribed code uses are
// named (CTaskMain::CheckNaviButtons).
enum eBarButton {
	BAR_BUTTON_NAVI_BACKWARD = 0,
	BAR_BUTTON_NAVI_FORWARD = 1
};

// Singleton showing the tool tip of the icon bar buttons.
class CIconToolTip {
	LAYOUT_CHECK_FRIEND;
public:
	// tooltip.h:89, inline in the original header (TaskMain emitted the
	// copy the linker kept). Defined in TaskMain.cpp: its local static's
	// guard shifts the _$E<n> numbering of every includer, whose
	// symbol_mappings.json pairs would all need remapping.
	static CIconToolTip *GetInstance(void);

	void SetToolTip(eBarButton button);
	void ClearToolTip(void);
	bool IsCurToolTip(eBarButton button);
	void Render(void);

	int GetCurTipIndex(void);          // tooltip.h:98

private:
	CIconToolTip(void);
	~CIconToolTip(void);

	int m_curTipIndex;              // 0x0, -1 when no tip is shown
	const Bitmap *m_curTipBitmap;   // 0x4
	const Bitmap *m_tipBitmaps[72]; // 0x8, one per bar button
};

CHECK_SIZE(CIconToolTip, 0x128);

BEGIN_LAYOUT_CHECK(CIconToolTip)
	CHECK_FIELD(CIconToolTip, m_curTipIndex, 0x0);
	CHECK_FIELD(CIconToolTip, m_curTipBitmap, 0x4);
	CHECK_FIELD(CIconToolTip, m_tipBitmaps, 0x8);
END_LAYOUT_CHECK

// TaskMain.cpp defines TASKMAIN_TOOLTIP_INLINE empty before its first
// include, so its object emits the out-of-line copy of TOOLTIP, as the
// original's did.
#ifndef TASKMAIN_TOOLTIP_INLINE
#define TASKMAIN_TOOLTIP_INLINE inline
#endif

// tooltip.h:111
TASKMAIN_TOOLTIP_INLINE CIconToolTip *TOOLTIP(void) {
	return CIconToolTip::GetInstance();
}

#endif // PG_CLIENT_PROJECTG_TOOLTIP_H
