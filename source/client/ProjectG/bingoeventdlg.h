// Original: d:\build\custom\temp\code\client\projectg\bingoeventdlg.h
#ifndef PG_CLIENT_PROJECTG_BINGOEVENTDLG_H
#define PG_CLIENT_PROJECTG_BINGOEVENTDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"
#include <vector>
#include <map>

class Bitmap;
class FrArea;
class FrBingoGiftDlg;
class FrButton;
class FrGraphicInterface;
class FrViewer;
class WRect;

// One cell of the bingo board: its number, a bool set by operator=(bool) and
// a bit set of properties (bit 0 and bit 1 pick the cell's bitmap in
// DrawPoint). The destructor is not virtual (??_G is QAE).
class CBingoPoint {
	LAYOUT_CHECK_FRIEND;

public:
	CBingoPoint(int number);
	~CBingoPoint(void);

	CBingoPoint& operator=(int number);
	CBingoPoint& operator=(bool value);

	void SetPointNumber(int number) { m_pointNumber = number; }    // bingoeventdlg.h:32
	int GetPointNumber(void) const { return m_pointNumber; }       // bingoeventdlg.h:33

	void EnableProperty(int property) { m_property |= property; }     // bingoeventdlg.h:35
	void DisableProperty(int property) { m_property &= ~property; }   // bingoeventdlg.h:36
	bool IsProperty(int property) { return (m_property & property) ? true : false; }   // bingoeventdlg.h:37

	void DrawPoint(FrGraphicInterface* pGI, const Bitmap* bitmaps[], const WRect& rect, float x, float y);

private:
	int m_pointNumber;   // 0x0
	bool m_value;        // 0x4 (operator=(bool))
	int m_property;      // 0x8 (EnableProperty/DisableProperty bits)
};

CHECK_SIZE(CBingoPoint, 0xC);
BEGIN_LAYOUT_CHECK(CBingoPoint)
	CHECK_FIELD(CBingoPoint, m_pointNumber, 0x0);
	CHECK_FIELD(CBingoPoint, m_value, 0x4);
	CHECK_FIELD(CBingoPoint, m_property, 0x8);
END_LAYOUT_CHECK

// One line (row, column or diagonal) of the bingo board: its points and a
// value at 0x10 the implicit copy constructor and operator= copy but the int
// constructor leaves alone. The destructor is not virtual (??_G is QAE).
class CBingoLine {
	LAYOUT_CHECK_FRIEND;

public:
	CBingoLine(int unused);
	~CBingoLine(void);

	CBingoLine& operator=(const CBingoLine& other);
	bool operator+=(CBingoPoint& point);

	int operator[](int index);
	const int operator[](int index) const;

	CBingoPoint& GetBingoPoint(int index);
	const CBingoPoint& GetBingoPoint(int index) const;

	CBingoPoint* FindBingoPoint(int number);
	int IsHavePointNumber(int number);
	bool IsCompleteLine(void);
	void ClearOverProperty(void);

private:
	std::vector<CBingoPoint> m_points;   // 0x0
	int m_lineValue;                     // 0x10 (copied by operator=)
};

CHECK_SIZE(CBingoLine, 0x14);
BEGIN_LAYOUT_CHECK(CBingoLine)
	CHECK_FIELD(CBingoLine, m_points, 0x0);
	CHECK_FIELD(CBingoLine, m_lineValue, 0x10);
END_LAYOUT_CHECK

// The bingo board: its lines (CBingoLine is 0x14 bytes, operator[]) and a
// second vector of trivially destructible elements at 0x10 (~CBingoBoard
// frees it without destroying elements).
class CBingoBoard {
	LAYOUT_CHECK_FRIEND;

public:
	CBingoBoard(void);
	~CBingoBoard(void);

	CBingoLine& operator[](int index);

	CBingoPoint* FindBingoPoint(int number);
	void ClearOverProperty(void);
	bool AimPoint(int number);

private:
	bool Initialize(void);
	void ReScanBoard(void);

	std::vector<CBingoLine> m_lines;   // 0x0
	std::vector<int> m_values;         // 0x10 (element type not yet known; trivially destructible)
};

CHECK_SIZE(CBingoBoard, 0x20);
BEGIN_LAYOUT_CHECK(CBingoBoard)
	CHECK_FIELD(CBingoBoard, m_lines, 0x0);
	CHECK_FIELD(CBingoBoard, m_values, 0x10);
END_LAYOUT_CHECK

// The bingo event dialog; its size (0x1C8) is the `new` of
// FrBingoEventDlgMakeInstance.
class FrBingoEventDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bingoeventdlg.h:101

	FrBingoEventDlg(void);
	virtual ~FrBingoEventDlg(void);

	struct sPoint {
		sPoint(void) : x(0), y(0) {}                   // bingoeventdlg.h:120
		sPoint(float px, float py) : x(px), y(py) {}   // bingoeventdlg.h:121

		float x;   // 0x0
		float y;   // 0x4
	};

	virtual bool OnInit(void);
	virtual void OnProc(const float elapsed);

	void SetGiftData(std::vector<unsigned long>* gifts, unsigned int value);
	void SetSelectedData(int number, int unused, unsigned int value);

	bool OnDetailDlgResult(int result, FrForm* pForm);
	bool OnGiftDlgResult(int result, FrForm* pForm);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitStartButton(intptr_t param);
	void OnLButtonDownStartButton(void);
	void OnInitEventDetail(intptr_t param);
	void OnLButtonDownEventDetail(void);
	void OnInitBoardNumberArea(intptr_t param);
	void OnOwnerDrawBoardNumberArea(int param);

private:
	void LoadBingoImage(void);
	void InitCharMatchMap(void);
	void InitNumberMatchMap(void);
	void DrawCompleteBar(FrGraphicInterface* pGI, const WRect& rect);
	void DrawBingoLogo(FrGraphicInterface* pGI, const WRect& rect);
	void DrawBoardNumber(FrGraphicInterface* pGI, const WRect& rect);
	void DrawRandomNumber(FrGraphicInterface* pGI, const WRect& rect);
	void DrawTryCount(FrGraphicInterface* pGI, const WRect& rect);

	FrButton* m_pStartButton;                 // 0x110
	FrButton* m_pEventDetail;                 // 0x114
	FrArea* m_pBoardNumberArea;               // 0x118
	CBingoBoard m_board;                      // 0x11C
	sPoint m_completeBarPos;                  // 0x13C (40, 205)
	sPoint m_randomNumberPos;                 // 0x144 (380, 180), DrawRandomNumber
	sPoint m_tryCountPos;                     // 0x14C (380, 403)
	const Bitmap* m_bitmaps[10];              // 0x154 (LoadBingoImage)
	FrForm* m_pDetailDlg;                     // 0x17C (cleared by OnDetailDlgResult)
	FrBingoGiftDlg* m_pGiftDlg;               // 0x180 (cleared by OnGiftDlgResult)
	int m_tryCount;                           // 0x184 (sBingoInfo +0x4, DrawTryCount)
	int m_bingoKind;                          // 0x188 (local player's UID % 10, OnInit)
	int m_selectedNumber;                     // 0x18C (SetSelectedData)
	unsigned int m_giftValue;                 // 0x190 (SetGiftData; bits of NS_BINGO::ns_resultTable, DrawCompleteBar)
	unsigned int m_numberMask;                // 0x194 (sBingoInfo +0x8, one bit per drawn number)
	unsigned int m_selectedValue;             // 0x198 (SetSelectedData)
	bool m_selectedFlag;                      // 0x19C (cleared by SetSelectedData)
	bool m_unusedFlag1;                       // 0x19D (only cleared by the constructor)
	bool m_unusedFlag2;                       // 0x19E (only cleared by the constructor)
	char m_tryCountText[3];                   // 0x19F (_itoa of m_tryCount, at most 99)
	PADDING(0x1A2, 0x1A4);
	std::map<char, sPoint> m_charMatchMap;    // 0x1A4 (InitCharMatchMap)
	std::map<int, sPoint> m_numberMatchMap;   // 0x1B0 (5 columns of 48x37, DrawRandomNumber)
	std::map<int, sPoint> m_boardMatchMap;    // 0x1BC (6 columns of 39x32, InitNumberMatchMap)
};

CHECK_SIZE(FrBingoEventDlg::sPoint, 0x8);
CHECK_SIZE(FrBingoEventDlg, 0x1C8);
BEGIN_LAYOUT_CHECK(FrBingoEventDlg)
	CHECK_FIELD(FrBingoEventDlg, m_pStartButton, 0x110);
	CHECK_FIELD(FrBingoEventDlg, m_pEventDetail, 0x114);
	CHECK_FIELD(FrBingoEventDlg, m_pBoardNumberArea, 0x118);
	CHECK_FIELD(FrBingoEventDlg, m_board, 0x11C);
	CHECK_FIELD(FrBingoEventDlg, m_completeBarPos, 0x13C);
	CHECK_FIELD(FrBingoEventDlg, m_randomNumberPos, 0x144);
	CHECK_FIELD(FrBingoEventDlg, m_tryCountPos, 0x14C);
	CHECK_FIELD(FrBingoEventDlg, m_bitmaps, 0x154);
	CHECK_FIELD(FrBingoEventDlg, m_bingoKind, 0x188);
	CHECK_FIELD(FrBingoEventDlg, m_selectedNumber, 0x18C);
	CHECK_FIELD(FrBingoEventDlg, m_selectedValue, 0x198);
	CHECK_FIELD(FrBingoEventDlg, m_selectedFlag, 0x19C);
	CHECK_FIELD(FrBingoEventDlg, m_charMatchMap, 0x1A4);
	CHECK_FIELD(FrBingoEventDlg, m_numberMatchMap, 0x1B0);
	CHECK_FIELD(FrBingoEventDlg, m_boardMatchMap, 0x1BC);
	CHECK_FIELD(FrBingoEventDlg, m_giftValue, 0x190);
	CHECK_FIELD(FrBingoEventDlg, m_tryCount, 0x184);
	CHECK_FIELD(FrBingoEventDlg, m_numberMask, 0x194);
	CHECK_FIELD(FrBingoEventDlg, m_unusedFlag1, 0x19D);
	CHECK_FIELD(FrBingoEventDlg, m_unusedFlag2, 0x19E);
	CHECK_FIELD(FrBingoEventDlg, m_tryCountText, 0x19F);
	CHECK_FIELD(FrBingoEventDlg, m_pDetailDlg, 0x17C);
	CHECK_FIELD(FrBingoEventDlg, m_pGiftDlg, 0x180);
END_LAYOUT_CHECK

// The bingo detail (explanation) dialog; its size (0x118) is the `new` of
// FrBingoDetailDlgMakeInstance.
class FrBingoDetailDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bingoeventdlg.h:206

	FrBingoDetailDlg(void);
	virtual ~FrBingoDetailDlg(void);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitExplaneViewer(intptr_t param);

private:
	FrViewer* m_pExplaneViewer;   // 0x110
	UNKNOWN(0x114, 0x118);
};

CHECK_SIZE(FrBingoDetailDlg, 0x118);
BEGIN_LAYOUT_CHECK(FrBingoDetailDlg)
	CHECK_FIELD(FrBingoDetailDlg, m_pExplaneViewer, 0x110);
END_LAYOUT_CHECK

// The bingo gift dialog; its size (0x150) is the `new` of
// FrBingoGiftDlgMakeInstance.
class FrBingoGiftDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;

public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // bingoeventdlg.h:234

	struct sGift {
		sGift(unsigned long giftId, const char* giftName) : id(giftId), name(giftName) {}   // bingoeventdlg.h:243

		unsigned long id;   // 0x0
		const char* name;   // 0x4
	};

	FrBingoGiftDlg(void);
	virtual ~FrBingoGiftDlg(void);

	void SetGiftData(std::vector<unsigned long>* gifts);

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;

	void OnInitGiftArea(intptr_t param);
	void OnOwnerDrawGiftArea(int param);

private:
	std::vector<unsigned long> m_gifts;   // 0x110 (gift item ids, SetGiftData)
	FrArea* m_pGiftArea;          // 0x120
	float m_startX[4];            // 0x124 (first gift's x by gift count - 1: 84, 52, 21, -5)
	const Bitmap* m_giftBitmaps[7];   // 0x134 (one per gift, SetGiftData)
};

CHECK_SIZE(FrBingoGiftDlg::sGift, 0x8);
CHECK_SIZE(FrBingoGiftDlg, 0x150);
BEGIN_LAYOUT_CHECK(FrBingoGiftDlg)
	CHECK_FIELD(FrBingoGiftDlg, m_gifts, 0x110);
	CHECK_FIELD(FrBingoGiftDlg, m_pGiftArea, 0x120);
	CHECK_FIELD(FrBingoGiftDlg, m_startX, 0x124);
	CHECK_FIELD(FrBingoGiftDlg, m_giftBitmaps, 0x134);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_BINGOEVENTDLG_H
