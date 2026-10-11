// Original: d:\build\custom\temp\code\client\projectg\formbar.h
#ifndef PG_CLIENT_PROJECTG_FORMBAR_H
#define PG_CLIENT_PROJECTG_FORMBAR_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include <list>

class Bitmap;
class FrArea;
class FrButton;
class FrEdit;

// A form with a gauge bar (unit client/ProjectG/FormBar), optionally with
// OK/NO buttons, a message edit and an auto-close timer. Size 0x190, the
// `new` of FrFormBarMakeInstance.
class FrFormBar : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	// An extra bitmap drawn over the bar (Attach).
	struct sATTACHRES {
		const Bitmap* pBitmap;   // 0x0
		WRect rect;              // 0x4
	};

	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // formbar.h:24

	FrFormBar(void);
	virtual ~FrFormBar(void);

	virtual bool Open(FRESH_RESULT_CALLBACK pfnResult, unsigned long flag);
	virtual void SetDesc(const char* desc);

	void Attach(const WRect& rect, const char* name);
	void SetFixedWnd(void);
	void MoveBar(const WPoint& pt);
	WPoint GetRelativeBarPos(void);
	void SetFrameImg(const char* name);
	void SetGaugeImg(const char* name);
	void SetRange(int min, int max);
	void SetPos(float pos);
	void SetGaugeSize(const WSize& size);
	void SetGaugeFrameSize(const WSize& size);
	void SetGaugePivot(const WPoint& pivot);
	void EnableButton(bool enable);
	void SetAutoCloseTimer(float time);
	const WRect& GetGaugeRect(void);   // formbar.h:41

private:
	static sFRESH_ENTRY _MsgEntries[];

	void Init(void);
	void MakeDefault(void);
	void ReCalcRects(void);

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	virtual void OnProc(const float dt);

	void OnInitEdMsg(intptr_t);
	void OnInitBtnOK(intptr_t);
	void OnInitBtnNO(intptr_t);
	void OnInitGaugeArea(intptr_t);
	void OnDrawGaugeArea(void);
	void OnDownBtnOK(void);
	void OnDownBtnNO(void);

private:
	std::list<sATTACHRES> m_attachList;   // 0x110
	FrArea* m_pGaugeArea;                 // 0x11C
	const Bitmap* m_pGaugeImg;            // 0x120
	const Bitmap* m_pFrameImg;            // 0x124
	WRect m_frameRect;                    // 0x128
	WRect m_gaugeRect;                    // 0x138
	WRect m_defaultRect;                  // 0x148 (the form's rect when opened)
	float m_min;                          // 0x158
	float m_max;                          // 0x15C
	float m_pos;                          // 0x160
	WPoint m_pivot;                       // 0x164 (gauge inset inside the frame)
	WPoint m_barPos;                      // 0x16C
	WSize m_frameSize;                    // 0x174
	FrButton* m_pBtnOK;                   // 0x17C
	FrButton* m_pBtnNO;                   // 0x180
	FrEdit* m_pEdMsg;                     // 0x184
	float m_autoCloseTime;                // 0x188
	float m_autoCloseTimer;               // 0x18C
};

CHECK_SIZE(FrFormBar, 0x190);
BEGIN_LAYOUT_CHECK(FrFormBar)
	CHECK_FIELD(FrFormBar, m_attachList, 0x110);
	CHECK_FIELD(FrFormBar, m_pGaugeArea, 0x11C);
	CHECK_FIELD(FrFormBar, m_pGaugeImg, 0x120);
	CHECK_FIELD(FrFormBar, m_pFrameImg, 0x124);
	CHECK_FIELD(FrFormBar, m_frameRect, 0x128);
	CHECK_FIELD(FrFormBar, m_gaugeRect, 0x138);
	CHECK_FIELD(FrFormBar, m_defaultRect, 0x148);
	CHECK_FIELD(FrFormBar, m_min, 0x158);
	CHECK_FIELD(FrFormBar, m_max, 0x15C);
	CHECK_FIELD(FrFormBar, m_pos, 0x160);
	CHECK_FIELD(FrFormBar, m_pivot, 0x164);
	CHECK_FIELD(FrFormBar, m_barPos, 0x16C);
	CHECK_FIELD(FrFormBar, m_frameSize, 0x174);
	CHECK_FIELD(FrFormBar, m_pBtnOK, 0x17C);
	CHECK_FIELD(FrFormBar, m_pBtnNO, 0x180);
	CHECK_FIELD(FrFormBar, m_pEdMsg, 0x184);
	CHECK_FIELD(FrFormBar, m_autoCloseTime, 0x188);
	CHECK_FIELD(FrFormBar, m_autoCloseTimer, 0x18C);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_FORMBAR_H
