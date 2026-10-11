// Original: d:\build\custom\temp\code\client\fresh\include\frtooltip.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRTOOLTIP_H
#define PG_CLIENT_FRESH_INCLUDE_FRTOOLTIP_H

#include <string>
#include <vector>

#include "..\..\defines.h"
#include "../client/Wangreal/include/wtypes.h"
#include "frtext.h"

class FrWnd;
class FrWndManager;
class WOverlay;

// The tooltip window shared by every Fresh window (FrWnd::m_pToolTip).
// Layout, access and vtable from the PDB (size 0x7c).
class FrToolTip {
	LAYOUT_CHECK_FRIEND;
	friend class FrWnd;
	friend class FrWndManager;
public:
	// The nine-piece frame drawn around the tooltip, one set per style
	// (PDB layout, size 0x14).
	class cToolTipFrame {
	public:
		enum eFrameIdx {
			LT = 0,
			MT = 1,
			RT = 2,
			LM = 3,
			MM = 4,
			RM = 5,
			LB = 6,
			MB = 7,
			RB = 8,
			FRMIDX_MAX = 9
		};
		typedef std::vector<WOverlay*> TOOLTIPFRM;

		cToolTipFrame(void);
		~cToolTipFrame(void);

		void Init(void);
		void SetStyle(unsigned long style);
		void Render(const WRect& rc, unsigned long color);

		std::vector<TOOLTIPFRM> m_Frames;   // 0x0
		unsigned long m_style;              // 0x10
	};

protected:
	FrToolTip(FrWndManager* pWndManager);
	virtual ~FrToolTip(void);

	virtual void Move(const WPoint& pt);
	virtual void SetToolTipText(const std::string& text);
	virtual void OnProcess(float delta);
	virtual void OnDisplay(void);
	virtual void SetFrameStyle(unsigned long style);

private:
	void Reset(void);
	void Prepare(void);

	float m_fAlpha;                     // 0x4
	cToolTipFrame m_Frame;              // 0x8
	WRect m_rcWnd;                      // 0x1c
	FrWndManager* m_pWndManager;        // 0x2c
	std::string m_orgText;              // 0x30
	FrTEXT m_FrText;                    // 0x4c
	unsigned long m_textColor;          // 0x78
};

CHECK_SIZE(FrToolTip::cToolTipFrame, 0x14);
BEGIN_LAYOUT_CHECK(FrToolTip)
	CHECK_FIELD(FrToolTip::cToolTipFrame, m_Frames, 0x0);
	CHECK_FIELD(FrToolTip::cToolTipFrame, m_style, 0x10);
	CHECK_FIELD(FrToolTip, m_fAlpha, 0x4);
	CHECK_FIELD(FrToolTip, m_Frame, 0x8);
	CHECK_FIELD(FrToolTip, m_rcWnd, 0x1c);
	CHECK_FIELD(FrToolTip, m_pWndManager, 0x2c);
	CHECK_FIELD(FrToolTip, m_orgText, 0x30);
	CHECK_FIELD(FrToolTip, m_FrText, 0x4c);
	CHECK_FIELD(FrToolTip, m_textColor, 0x78);
END_LAYOUT_CHECK
CHECK_SIZE(FrToolTip, 0x7c);

#endif // PG_CLIENT_FRESH_INCLUDE_FRTOOLTIP_H
