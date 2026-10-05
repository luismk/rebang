#pragma once

#include "frform.h"
#include <vector>
#include <map>

#include "../../shared/globalgamedefine.h"

class FrBingoDetailDlg;
class FrBingoGiftDlg;

class CBingoPoint
{
public:
	CBingoPoint(int number);
	~CBingoPoint();

	void SetPointNumber(int number) { m_pointNumber = number; }
	int GetPointNumber() const { return m_pointNumber; }

	void EnableProperty(int property) { m_property |= property; }
	void DisableProperty(int property) { m_property &= ~property; }
	bool IsProperty(int property)
	{
		return (m_property & property) ? true : false;
	}

	void DrawPoint(FrGraphicInterface* gdi, const Bitmap** const bitmap,
		const WRect& rect, float x, float y);

	CBingoPoint& operator=(int number);
	CBingoPoint& operator=(bool b);

private:
	int m_pointNumber;
	bool m_bEnable;
	int m_property;
};

class CBingoLine
{
public:
	CBingoLine(int size);
	~CBingoLine();

	CBingoLine& operator=(const CBingoLine& line);
	bool operator+=(CBingoPoint& point);
	int operator[](int index);
	const int operator[](int index) const;

	CBingoPoint& GetBingoPoint(int index);
	const CBingoPoint& GetBingoPoint(int index) const;
	CBingoPoint* FindBingoPoint(int number);
	int IsHavePointNumber(int number);
	bool IsCompleteLine();
	void ClearOverProperty();

private:
	std::vector<CBingoPoint> m_points;
	unsigned long m_reserved;
};

class CBingoBoard
{
public:
	CBingoBoard();
	~CBingoBoard();

	CBingoLine& operator[](int index);
	CBingoPoint* FindBingoPoint(int number);
	bool AimPoint(int number);
	void ClearOverProperty();

private:
	bool Initialize();
	void ReScanBoard();

	std::vector<CBingoLine> m_lines;
	std::vector<int> m_completeLine;
};

class FrBingoEventDlg : public FrForm
{
	DECLARE_OBJECT(FrBingoEventDlg)

	FrBingoEventDlg();
	virtual ~FrBingoEventDlg();

	virtual bool OnInit();
	virtual void OnProc(const float dt);

	void SetBingoData(GlobalEnum::sBingoInfo* info);
	void SetSelectedData(int number, int count, unsigned int newLine);
	void SetGiftData(std::vector<unsigned long>* gifts,
		unsigned int completeLine);

	bool OnDetailDlgResult(int result, FrForm* form);
	bool OnGiftDlgResult(int result, FrForm* form);

	struct sPoint
	{
		float x;
		float y;
		sPoint()
			: x(0), y(0)
		{
		}
		sPoint(float _x, float _y)
			: x(_x), y(_y)
		{
		}
	};

protected:
	void OnInitStartButton(int);
	void OnLButtonDownStartButton();
	void OnInitEventDetail(int);
	void OnLButtonDownEventDetail();
	void OnInitBoardNumberArea(int);
	void OnOwnerDrawBoardNumberArea(int);

private:
	void LoadBingoImage();
	void InitCharMatchMap();
	void InitNumberMatchMap();
	void DrawCompleteBar(FrGraphicInterface* gdi, const WRect& rect);
	void DrawBoardNumber(FrGraphicInterface* gdi, const WRect& rect);
	void DrawRandomNumber(FrGraphicInterface* gdi, const WRect& rect);
	void DrawTryCount(FrGraphicInterface* gdi, const WRect& rect);
	void DrawBingoLogo(FrGraphicInterface* gdi, const WRect& rect);

	static sFRESH_ENTRY _MsgEntries[0];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap() const;

private:
	FrButton* m_pStartButton;
	FrButton* m_pEventDetail;
	FrArea* m_pBoardNumberArea;
	CBingoBoard m_board;
	WPoint m_boardPos;
	WPoint m_randomNumberPos;
	WPoint m_tryCountPos;
	const Bitmap* m_pBitmap[10];
	FrBingoDetailDlg* m_pDetailDlg;
	FrBingoGiftDlg* m_pGiftDlg;
	int m_tryCount;
	unsigned long m_boardIndex;
	int m_selectedNumber;
	unsigned int m_completeLine;
	unsigned long m_aimedPoint;
	unsigned int m_newLine;
	bool m_bWaitResult;
	bool m_bReserved1;
	bool m_bReserved2;
	char m_tryCountText[3];
	std::map<char, sPoint> m_charMatchMap;
	std::map<int, sPoint> m_randomNumberMatchMap;
	std::map<int, sPoint> m_boardNumberMatchMap;
};

class FrBingoDetailDlg : public FrForm
{
	DECLARE_OBJECT(FrBingoDetailDlg)

	FrBingoDetailDlg();
	virtual ~FrBingoDetailDlg();

protected:
	void OnInitExplaneViewer(int);

private:
	static sFRESH_ENTRY _MsgEntries[0];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap() const;

private:
	FrViewer* m_pExplaneViewer;
};

class FrBingoGiftDlg : public FrForm
{
	DECLARE_OBJECT(FrBingoGiftDlg)

	FrBingoGiftDlg();
	virtual ~FrBingoGiftDlg();

	void SetGiftData(std::vector<unsigned long>* gifts);

	struct sGift
	{
		sGift(unsigned long _typeId, const char* _image)
			: typeId(_typeId), image(_image)
		{
		}

		unsigned long typeId;
		const char* image;
	};

protected:
	void OnInitGiftArea(int);
	void OnOwnerDrawGiftArea(int);

private:
	static sFRESH_ENTRY _MsgEntries[0];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap() const;

private:
	std::vector<unsigned long> m_giftList;
	FrArea* m_pGiftArea;
	float m_offsetX[4];
	const Bitmap* m_pGiftImage[7];
};
