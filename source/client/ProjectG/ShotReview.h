// Original: d:\build\custom\temp\code\client\projectg\shotreview.h
#ifndef PG_CLIENT_PROJECTG_SHOTREVIEW_H
#define PG_CLIENT_PROJECTG_SHOTREVIEW_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"
#include "actor.h"
#include "RTTI.h"
#include "scenemanager.h"
#include <list>
#include <utility>
#include <vector>

// One layer of the shot review overlay. AddItem's two parameters carry
// pointers to the layer's item data.
class IReviewHistory {
public:
	// shotreview.h:15
	virtual ~IReviewHistory() {
	}
	virtual const char* ClassName() const = 0;
	virtual void AddItem(unsigned int param1, unsigned int param2) = 0;
	virtual void Clear() = 0;
	virtual void Display() = 0;
	virtual void PopBack() = 0;
};
CHECK_SIZE(IReviewHistory, 0x4);

class CBarFont;
class WOverlay;

// The landing spots: one (start, end) pair per shot.
class SpotHistory : public IReviewHistory {
	LAYOUT_CHECK_FRIEND;
public:
	SpotHistory();
	virtual ~SpotHistory();
	// shotreview.h:74
	virtual const char* ClassName() const {
		return "SpotHistory";
	}
	virtual void AddItem(unsigned int param1, unsigned int param2);
	virtual void Clear();
	virtual void Display();
	virtual void PopBack();

private:
	std::vector<std::pair<WVector, WVector> > m_spots; // 0x4
	CBarFont *m_pBarFont;                              // 0x14
	WOverlay *m_pIndicator1;                           // 0x18, "fall_indicator1.png"
	WOverlay *m_pIndicator2;                           // 0x1C, "fall_indicator2.png"
};
CHECK_SIZE(SpotHistory, 0x20);
BEGIN_LAYOUT_CHECK(SpotHistory)
	CHECK_FIELD(SpotHistory, m_spots, 0x4);
	CHECK_FIELD(SpotHistory, m_pBarFont, 0x14);
	CHECK_FIELD(SpotHistory, m_pIndicator1, 0x18);
	CHECK_FIELD(SpotHistory, m_pIndicator2, 0x1C);
END_LAYOUT_CHECK

// The first bounces of each shot. AddItem's first parameter picks the point
// list (0 or 1), its second points to the WVector to add.
class FirstBoundHistory : public IReviewHistory {
	LAYOUT_CHECK_FRIEND;
public:
	FirstBoundHistory();
	virtual ~FirstBoundHistory();
	// shotreview.h:130
	virtual const char* ClassName() const {
		return "FirstBoundHistory";
	}
	virtual void AddItem(unsigned int param1, unsigned int param2);
	virtual void Clear();
	virtual void Display();
	virtual void PopBack();

private:
	void DrawIndicator(const WVector& pos, unsigned long color);

	std::vector<WVector> m_type0Points; // 0x4, AddItem type 0
	std::vector<WVector> m_type1Points; // 0x14, AddItem type 1
	WOverlay *m_pIndicator;             // 0x24, "{indicator.bmp"
	CBarFont *m_pBarFont;               // 0x28, the distance labels
};
CHECK_SIZE(FirstBoundHistory, 0x2C);
BEGIN_LAYOUT_CHECK(FirstBoundHistory)
	CHECK_FIELD(FirstBoundHistory, m_type0Points, 0x4);
	CHECK_FIELD(FirstBoundHistory, m_type1Points, 0x14);
	CHECK_FIELD(FirstBoundHistory, m_pIndicator, 0x24);
	CHECK_FIELD(FirstBoundHistory, m_pBarFont, 0x28);
END_LAYOUT_CHECK

// The ball's flight paths: one point list per shot.
class BallTraceHistory : public IReviewHistory {
	LAYOUT_CHECK_FRIEND;
public:
	BallTraceHistory();
	virtual ~BallTraceHistory();
	// shotreview.h:102
	virtual const char* ClassName() const {
		return "BallTraceHistory";
	}
	virtual void AddItem(unsigned int param1, unsigned int param2);
	virtual void Clear();
	virtual void Display();
	virtual void PopBack();

private:
	std::list<std::vector<WVector> > m_traces; // 0x4
};
CHECK_SIZE(BallTraceHistory, 0x10);
BEGIN_LAYOUT_CHECK(BallTraceHistory)
	CHECK_FIELD(BallTraceHistory, m_traces, 0x4);
END_LAYOUT_CHECK

// The shot review overlay: draws every review layer (landing spots, first
// bounces, flight paths) while it is shown.
class ShotReview : public IActor, public CRenderFuncPtr {
	LAYOUT_CHECK_FRIEND;
	// The factory function of the class registration (shotreview.cpp:271).
	friend IObject *ShotReviewMakeInstance(void);
public:
	// shotreview.h:27
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

	virtual void HandleMsg(const MsgObject &msg);
	virtual void Display(void);

	static const WRTTI m_RTTI;

protected:
	ShotReview(void);
	virtual ~ShotReview(void);

	virtual void OnInit(void);
	virtual void OnProcess(float dt);

private:
	std::vector<IReviewHistory*> m_histories; // 0x1C
	bool m_show;                              // 0x2C, toggled by LCONTROL+REPLAY
	bool m_enabled;                           // 0x2D, set by the constructor
};
CHECK_SIZE(ShotReview, 0x30);
BEGIN_LAYOUT_CHECK(ShotReview)
	CHECK_FIELD(ShotReview, m_histories, 0x1C);
	CHECK_FIELD(ShotReview, m_show, 0x2C);
	CHECK_FIELD(ShotReview, m_enabled, 0x2D);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_SHOTREVIEW_H
