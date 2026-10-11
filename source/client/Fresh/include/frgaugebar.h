// Original: d:\build\custom\temp\code\client\fresh\include\frgaugebar.h
#ifndef PG_CLIENT_FRESH_INCLUDE_FRGAUGEBAR_H
#define PG_CLIENT_FRESH_INCLUDE_FRGAUGEBAR_H

#include "..\..\defines.h"
#include "frelement.h"
#include "frwnd.h"

class Bitmap;
class FrButton;

// A gauge (progress) bar sliding towards a destination position. Layout,
// access and vtable from the PDB (size 0x120, base FrWnd; Init, SetPos and
// CalcBarRect are the new virtuals of slots 23-25). The bodies written here
// are the inline ones whose COMDATs carry frgaugebar.h line marks (lines
// 19-40).
class FrGaugeBar : public FrWnd {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frgaugebar.h:19

	FrGaugeBar(void);
	virtual ~FrGaugeBar(void);

	// vtable slots 23-24
	virtual void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	virtual void SetPos(int pos);

	void SetBorder(int thickness) { m_thickness = thickness; }     // frgaugebar.h:25
	void SetRange(int lower, int upper, int origin);
	void SetExpand(int expand);
	// Never emitted as COMDATs; the bodies are inferred from their members.
	void SetExpandColor1(unsigned long color) { m_exColor[0] = color; }
	void SetExpandColor2(unsigned long color) { m_exColor[1] = color; }
	void SetDestPos(int pos, bool achillesRun);
	float GetPos(void) { return m_pos; }                           // frgaugebar.h:32
	// Never emitted as a COMDAT; the body is inferred from its member.
	float GetDestPos(void) { return m_destPos; }
	void SetColor(unsigned long color) { m_color = color; }        // frgaugebar.h:34
	void SetBarSpeed(float speed) { m_speed = speed; }             // frgaugebar.h:35
	// Never emitted as COMDATs; the bodies are inferred from their members.
	int GetRangeLower(void) { return m_lower; }
	int GetRangeUpper(void) { return m_upper; }
	virtual void MoveWindow(const WPoint& pos);
	virtual void Enable(bool enable) {                             // frgaugebar.h:40
		if (!enable) {
			m_dwStyle.Enable(0x2);
		} else {
			m_dwStyle.Disable(0x2);
		}
	}

protected:
	// vtable slot 25
	virtual void CalcBarRect(const WRect* pRect);

	virtual void OnProc(const float delta);
	virtual void OnDraw(void);

	FrGuiItem* m_pItem;              // 0xd4
	WRect m_barRect;                 // 0xd8
	unsigned long m_color;           // 0xe8
	unsigned long m_bgColor;         // 0xec
	unsigned long m_exColor[2];      // 0xf0
	int m_thickness;                 // 0xf8
	int m_lower;                     // 0xfc
	int m_upper;                     // 0x100
	int m_origin;                    // 0x104
	float m_pos;                     // 0x108
	float m_expand;                  // 0x10c
	float m_destPos;                 // 0x110
	float m_srcPos;                  // 0x114
	float m_speed;                   // 0x118
	bool m_achillesRun;              // 0x11c
};

CHECK_SIZE(FrGaugeBar, 0x120);
BEGIN_LAYOUT_CHECK(FrGaugeBar)
	CHECK_FIELD(FrGaugeBar, m_pItem, 0xd4);
	CHECK_FIELD(FrGaugeBar, m_barRect, 0xd8);
	CHECK_FIELD(FrGaugeBar, m_color, 0xe8);
	CHECK_FIELD(FrGaugeBar, m_bgColor, 0xec);
	CHECK_FIELD(FrGaugeBar, m_exColor, 0xf0);
	CHECK_FIELD(FrGaugeBar, m_thickness, 0xf8);
	CHECK_FIELD(FrGaugeBar, m_lower, 0xfc);
	CHECK_FIELD(FrGaugeBar, m_upper, 0x100);
	CHECK_FIELD(FrGaugeBar, m_origin, 0x104);
	CHECK_FIELD(FrGaugeBar, m_pos, 0x108);
	CHECK_FIELD(FrGaugeBar, m_expand, 0x10c);
	CHECK_FIELD(FrGaugeBar, m_destPos, 0x110);
	CHECK_FIELD(FrGaugeBar, m_srcPos, 0x114);
	CHECK_FIELD(FrGaugeBar, m_speed, 0x118);
	CHECK_FIELD(FrGaugeBar, m_achillesRun, 0x11c);
END_LAYOUT_CHECK

// A gauge bar with left/right step buttons. Layout, access and vtable from
// the PDB (size 0x234, base FrGaugeBar).
class FrGaugeBarEx : public FrGaugeBar {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frgaugebar.h:68

	FrGaugeBarEx(void);
	virtual ~FrGaugeBarEx(void);

	virtual void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	virtual void SetPos(int pos);
	void SetSteps(int steps);
	int GetSteps(void);
	virtual void Enable(bool enable);
	// Never emitted as COMDATs; the bodies are inferred from their members.
	const FrButton* GetLButton(void) { return m_pLButton; }
	const FrButton* GetRButton(void) { return m_pRButton; }
	virtual void MoveWindow(const WPoint& pos);

protected:
	virtual void OnDraw(void);
	virtual void CalcBarRect(const WRect* pRect);

	void OnLLButtonDown(void);
	void OnRLButtonDown(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	unsigned long m_borderColor;     // 0x120
	unsigned long m_color2;          // 0x124
	unsigned long m_colorP;          // 0x128
	WRect m_gaugeRect;               // 0x12c
	float m_fPos;                    // 0x13c
	int m_steps;                     // 0x140
	FrGuiItem m_LButtonInfo;         // 0x144
	FrButton* m_pLButton;            // 0x1b8
	FrGuiItem m_RButtonInfo;         // 0x1bc
	FrButton* m_pRButton;            // 0x230
};

CHECK_SIZE(FrGaugeBarEx, 0x234);
BEGIN_LAYOUT_CHECK(FrGaugeBarEx)
	CHECK_FIELD(FrGaugeBarEx, m_borderColor, 0x120);
	CHECK_FIELD(FrGaugeBarEx, m_color2, 0x124);
	CHECK_FIELD(FrGaugeBarEx, m_colorP, 0x128);
	CHECK_FIELD(FrGaugeBarEx, m_gaugeRect, 0x12c);
	CHECK_FIELD(FrGaugeBarEx, m_fPos, 0x13c);
	CHECK_FIELD(FrGaugeBarEx, m_steps, 0x140);
	CHECK_FIELD(FrGaugeBarEx, m_LButtonInfo, 0x144);
	CHECK_FIELD(FrGaugeBarEx, m_pLButton, 0x1b8);
	CHECK_FIELD(FrGaugeBarEx, m_RButtonInfo, 0x1bc);
	CHECK_FIELD(FrGaugeBarEx, m_pRButton, 0x230);
END_LAYOUT_CHECK

// A gauge bar drawn from a skin bitmap and an optional mask. Layout, access
// and vtable from the PDB (size 0x14c, base FrGaugeBar).
class FrGaugeBarImage : public FrGaugeBar {
	LAYOUT_CHECK_FRIEND;
public:
	enum eGauge {
		GAUGE_NORMAL = 0,
		GAUGE_MASK = 1,
		MAX_COUNT = 2
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // frgaugebar.h:111

	FrGaugeBarImage(void);
	virtual ~FrGaugeBarImage(void);

	virtual void Init(FrGuiItem& item, FrWndManager* pManager, FrWnd* pParent);
	virtual void MoveWindow(const WPoint& pos);
	const Bitmap* GetGaugeSkin(int type);
	void ResetGauge(void);
	int GetPixelPerHeight(void);
	float GetComplete(void);

protected:
	virtual void OnDraw(void);
	virtual void OnProc(const float delta);
	virtual void CalcBarRect(const WRect* pRect);

	const Bitmap* m_pBitImage[2];    // 0x120
	Bitmap* m_BitMask;               // 0x128
	WRect m_gaugeRect;               // 0x12c
	float m_start;                   // 0x13c
	unsigned int m_iPixelCounter;    // 0x140
	bool m_bOwnerDraw;               // 0x144
	float m_complete;                // 0x148
};

CHECK_SIZE(FrGaugeBarImage, 0x14c);
BEGIN_LAYOUT_CHECK(FrGaugeBarImage)
	CHECK_FIELD(FrGaugeBarImage, m_pBitImage, 0x120);
	CHECK_FIELD(FrGaugeBarImage, m_BitMask, 0x128);
	CHECK_FIELD(FrGaugeBarImage, m_gaugeRect, 0x12c);
	CHECK_FIELD(FrGaugeBarImage, m_start, 0x13c);
	CHECK_FIELD(FrGaugeBarImage, m_iPixelCounter, 0x140);
	CHECK_FIELD(FrGaugeBarImage, m_bOwnerDraw, 0x144);
	CHECK_FIELD(FrGaugeBarImage, m_complete, 0x148);
END_LAYOUT_CHECK

#endif // PG_CLIENT_FRESH_INCLUDE_FRGAUGEBAR_H
