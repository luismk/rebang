// Original: d:\build\custom\temp\code\client\projectg\bingoeventdlg.cpp
#include "BingoEventDlg.h"
#include "ObjectFactory.h"
#include "frgraphicinterface.h"
#include "frarea.h"
#include "frbutton.h"
#include "frviewer.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "SharedDoc.h"
#include "bitmap.h"
#include "Packet.h"
#include "TaskManager.h"

// The board numbers of each bingo kind and the bit masks of the 12 lines
// (5 rows, 5 columns, 2 diagonals); defined with the unit's data.
namespace NS_BINGO {
	extern int ns_board[][25];
	extern int ns_resultTable[];
}

// bingoeventdlg.cpp:65
CBingoPoint::CBingoPoint(int number)
	: m_pointNumber(number),
	  m_value(false),
	  m_property(0) {
}

// bingoeventdlg.cpp:70
CBingoPoint::~CBingoPoint(void) {
}

// bingoeventdlg.cpp:75
void CBingoPoint::DrawPoint(FrGraphicInterface* pGI, const Bitmap* bitmaps[], const WRect& rect, float x, float y) {
	const Bitmap* pBitmap;

	if (m_property & 1) {
		pBitmap = bitmaps[4];
	} else if (m_property & 2) {
		pBitmap = bitmaps[3];
	} else {
		pBitmap = bitmaps[2];
	}

	pGI->DrawTexture(pBitmap, rect, WRect(x, y, rect.w, rect.h), 0xFFFFFFFF, 0);
}

// bingoeventdlg.cpp:95
CBingoPoint& CBingoPoint::operator=(int number) {
	m_pointNumber = number;

	return *this;
}

// bingoeventdlg.cpp:102
CBingoPoint& CBingoPoint::operator=(bool value) {
	m_value = value;

	return *this;
}

// bingoeventdlg.cpp:119
CBingoLine::CBingoLine(int unused) {
}

// bingoeventdlg.cpp:124
CBingoLine::~CBingoLine(void) {
}

// bingoeventdlg.cpp:129
void CBingoLine::ClearOverProperty(void) {
	int count = m_points.size();
	for (int i = 0; i < count; i++) {
		m_points[i].DisableProperty(1);
	}
}

// bingoeventdlg.cpp:138
CBingoPoint* CBingoLine::FindBingoPoint(int number) {
	int count = m_points.size();
	for (int i = 0; i < count; i++) {
		if (m_points[i].GetPointNumber() == number) {
			return &m_points[i];
		}
	}

	return 0;
}

// bingoeventdlg.cpp:152
CBingoPoint& CBingoLine::GetBingoPoint(int index) {
	return m_points[index];
}

// bingoeventdlg.cpp:159
const CBingoPoint& CBingoLine::GetBingoPoint(int index) const {
	return m_points[index];
}

// bingoeventdlg.cpp:166
int CBingoLine::IsHavePointNumber(int number) {
	int count = m_points.size();

	if (count != 0) {
		for (int i = 0; i < count; i++) {
			if (m_points[i].GetPointNumber() == number) {
				return i;
			}
		}
	}

	return -1;
}

// bingoeventdlg.cpp:185
bool CBingoLine::IsCompleteLine(void) {
	int count = m_points.size();
	for (int i = 0; i < count; i++) {
		if (!m_points[i].IsProperty(2)) {
			return false;
		}
	}

	return true;
}

// bingoeventdlg.cpp:199
int CBingoLine::operator[](int index) {
	return m_points[index].GetPointNumber();
}

// bingoeventdlg.cpp:204
const int CBingoLine::operator[](int index) const {
	return m_points[index].GetPointNumber();
}

// bingoeventdlg.cpp:209
CBingoLine& CBingoLine::operator=(const CBingoLine& other) {
	m_points.clear();
	m_points.insert(m_points.begin(), other.m_points.begin(), other.m_points.end());
	m_lineValue = other.m_lineValue;

	return *this;
}

// bingoeventdlg.cpp:218
bool CBingoLine::operator+=(CBingoPoint& point) {
	m_points.push_back(point);
	return true;
}

// bingoeventdlg.cpp:233
CBingoBoard::CBingoBoard(void) {
	Initialize();
}

// bingoeventdlg.cpp:238
CBingoBoard::~CBingoBoard(void) {
}

// bingoeventdlg.cpp:243
CBingoLine& CBingoBoard::operator[](int index) {
	return m_lines[index];
}

// bingoeventdlg.cpp:250
bool CBingoBoard::Initialize(void) {
	if (m_lines.size() != 0) {
		m_lines.clear();
	}

	for (int i = 0; i < 5; i++) {
		CBingoLine line(0);

		for (int j = 0; j < 5; j++) {
			line += CBingoPoint(NS_BINGO::ns_board[0][i * 5 + j]);
		}

		m_lines.push_back(line);
	}

	m_values.reserve(m_lines.size() * 2 + 2);

	m_values.assign(12, 0);

	return true;
}

// bingoeventdlg.cpp:281
bool CBingoBoard::AimPoint(int number) {
	int count = m_lines.size();
	int index = -1;
	int i;

	for (i = 0; i < count; i++) {
		index = m_lines[i].IsHavePointNumber(number);
		if (index != -1) {
			break;
		}
	}

	CBingoPoint& point = m_lines[i].GetBingoPoint(index);
	if (point.IsProperty(2)) {
		return false;
	}

	point.EnableProperty(2);

	ReScanBoard();

	return true;
}

// bingoeventdlg.cpp:310
void CBingoBoard::ClearOverProperty(void) {
	int count = m_lines.size();
	for (int i = 0; i < count; i++) {
		m_lines[i].ClearOverProperty();
	}
}

// bingoeventdlg.cpp:319
CBingoPoint* CBingoBoard::FindBingoPoint(int number) {
	int count = m_lines.size();
	for (int i = 0; i < count; i++) {
		CBingoPoint* pPoint = m_lines[i].FindBingoPoint(number);
		if (pPoint != 0) {
			return pPoint;
		}
	}

	return 0;
}

// bingoeventdlg.cpp:334
// Marks the complete lines in m_values: the rows, then (as the original
// indexes them) the points (j, j) once per row, then the points (i, -i) and
// the anti-diagonal.
void CBingoBoard::ReScanBoard(void) {
	int index = 0;
	int count = m_lines.size();
	int i;
	int j;
	bool complete;

	for (i = 0; i < count; i++) {
		if (m_lines[i].IsCompleteLine()) {
			m_values[index] = 1;
		}

		index++;
	}

	complete = false;
	for (i = 0; i < count; i++) {
		for (j = 0; j < count; j++) {
			if (!m_lines[j].GetBingoPoint(j).IsProperty(2)) {
				break;
			}

			complete = true;
		}

		if (complete == true) {
			m_values[index] = 1;
		}

		index++;
		complete = false;
	}

	complete = true;
	for (i = 0; i < count; i++) {
		if (!m_lines[i].GetBingoPoint(-i).IsProperty(2)) {
			complete = false;
			break;
		}
	}

	if (complete) {
		m_values[index++] = 1;
	}

	complete = true;
	for (i = 0; i < count; i++) {
		if (!m_lines[i].GetBingoPoint(count - 1 - i).IsProperty(2)) {
			complete = false;
			break;
		}
	}

	if (complete) {
		m_values[index] = 1;
	}
}

// bingoeventdlg.cpp:419 (run-time type and factory registration of FrBingoEventDlg)
const WRTTI FrBingoEventDlg::m_RTTI("FrBingoEventDlg", &FrForm::m_RTTI);

IObject* FrBingoEventDlgMakeInstance(void) {
	return new FrBingoEventDlg;
}

struct __sFrBingoEventDlg {
	__sFrBingoEventDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBingoEventDlgMakeInstance, "FrBingoEventDlg");
	}
};
static __sFrBingoEventDlg s_registerFrBingoEventDlg;

// bingoeventdlg.cpp:421
const sFRESH_MSGMAP* FrBingoEventDlg::GetMessageMap(void) const {
	return &FrBingoEventDlg::_MsgMap;
}

sFRESH_MSGMAP FrBingoEventDlg::_MsgMap = { &FrForm::_MsgMap, &FrBingoEventDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBingoEventDlg::_MsgEntries[] = {
	// bingoeventdlg.cpp:423
	{ "start", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoEventDlg::OnInitStartButton },
	// bingoeventdlg.cpp:424
	{ "start", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrBingoEventDlg::OnLButtonDownStartButton },

	// bingoeventdlg.cpp:426
	{ "detail", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoEventDlg::OnInitEventDetail },
	// bingoeventdlg.cpp:427
	{ "detail", FRCMD_LBUTTONDOWN, FrFuncVV, (FRESH_PMSG)&FrBingoEventDlg::OnLButtonDownEventDetail },

	// bingoeventdlg.cpp:429
	{ "boardNumber", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoEventDlg::OnInitBoardNumberArea },
	// bingoeventdlg.cpp:430
	{ "boardNumber", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoEventDlg::OnOwnerDrawBoardNumberArea },

	// bingoeventdlg.cpp:432
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bingoeventdlg.cpp:435
FrBingoEventDlg::FrBingoEventDlg(void) {
	m_pStartButton = 0;
	m_pEventDetail = 0;
	m_pDetailDlg = 0;
	m_pGiftDlg = 0;
	m_tryCount = 0;
	m_bingoKind = 0;
	memset(m_bitmaps, 0, sizeof(m_bitmaps));

	m_pBoardNumberArea = 0;
	m_selectedNumber = 0;
	m_giftValue = 0;
	m_numberMask = 0;
	m_selectedValue = 0;
	m_selectedFlag = false;
	m_unusedFlag1 = false;
	m_unusedFlag2 = false;

	memset(m_tryCountText, 0, sizeof(m_tryCountText));
}

// bingoeventdlg.cpp:456
FrBingoEventDlg::~FrBingoEventDlg(void) {
}

// bingoeventdlg.cpp:461
bool FrBingoEventDlg::OnInit(void) {
	m_bingoKind = WSingleton<CSharedDoc>::Instance()->m_myUID % 10;

	LoadBingoImage();

	m_completeBarPos = sPoint(40.0f, 205.0f);
	m_randomNumberPos = sPoint(380.0f, 180.0f);
	m_tryCountPos = sPoint(380.0f, 403.0f);

	InitCharMatchMap();
	InitNumberMatchMap();

	return FrForm::OnInit();
}

// bingoeventdlg.cpp:477
void CalcOffset(int index, int columns, float& x, float& y, float width, float height, float unused) {
	int row = index / columns;
	int column = index % columns;

	x = column * width;
	y = row * height;
}

// bingoeventdlg.cpp:486
bool FrBingoEventDlg::OnDetailDlgResult(int result, FrForm* pForm) {
	m_pDetailDlg = 0;

	return true;
}

// bingoeventdlg.cpp:493
bool FrBingoEventDlg::OnGiftDlgResult(int result, FrForm* pForm) {
	m_pGiftDlg = 0;

	return true;
}

// bingoeventdlg.cpp:527
void FrBingoEventDlg::SetGiftData(std::vector<unsigned long>* gifts, unsigned int value) {
	m_giftValue = value;

	m_pGiftDlg = CreateForm<FrBingoGiftDlg>(g_pFresh->GetManager(), this, "bingo_gift", 0);
	if (m_pGiftDlg) {
		m_pGiftDlg->Open((FRESH_RESULT_CALLBACK)&FrBingoEventDlg::OnGiftDlgResult, 1);
		m_pGiftDlg->SetGiftData(gifts);
	}
}

// bingoeventdlg.cpp:539
void FrBingoEventDlg::SetSelectedData(int number, int unused, unsigned int value) {
	m_selectedNumber = number;

	CBingoPoint* pPoint = m_board.FindBingoPoint(number);

	if (pPoint) {
		pPoint->EnableProperty(1);
	}

	m_selectedValue = value;

	m_selectedFlag = false;
}

// bingoeventdlg.cpp:557
void FrBingoEventDlg::InitCharMatchMap(void) {
	char ch = '0';
	for (int i = 0; i < 10; i++) {
		int column = (unsigned int)i % 10;
		int row = (unsigned int)i / 10;
		m_charMatchMap.insert(std::pair<const char, sPoint>(ch++, sPoint(column * 20.0f, row * 32.0f)));
	}
}

// bingoeventdlg.cpp:568
void FrBingoEventDlg::InitNumberMatchMap(void) {
	int i;

	for (i = 0; i < 25; i++) {
		int column = (unsigned int)i % 6;
		int row = (unsigned int)i / 6;
		m_boardMatchMap.insert(std::pair<const int, sPoint>(i + 1, sPoint(column * 39.0f, row * 32.0f)));
	}

	for (i = 0; i < 25; i++) {
		int column = (unsigned int)i % 5;
		int row = (unsigned int)i / 5;
		m_numberMatchMap.insert(std::pair<const int, sPoint>(i + 1, sPoint(column * 48.0f, row * 37.0f)));
	}
}

// bingoeventdlg.cpp:584
void FrBingoEventDlg::LoadBingoImage(void) {
	const char* names[10] = {
		"bingo_conut_num_all", "bingo_num_all",
		"bingo_num_01", "bingo_num_02", "bingo_num_03",
		"bingo_line_1", "bingo_line_2",
		"bingo_line_3", "bingo_line_4",
		"bingo_ment"
	};

	for (int i = 0; i < 10; i++) {
		m_bitmaps[i] = g_pFresh->RegisterBitmap(names[i]);
	}
}

// bingoeventdlg.cpp:602
void FrBingoEventDlg::OnProc(const float elapsed) {
}

// bingoeventdlg.cpp:607
void FrBingoEventDlg::DrawBoardNumber(FrGraphicInterface* pGI, const WRect& rect) {
	WRect src(0.0f, 0.0f, 39.0f, 32.0f);
	float columnX = 0.0f;
	float rowY = 0.0f;
	float rowX = 0.0f;
	float slantX = 0.0f;
	float slantRowY = 0.0f;

	for (int i = 0; i < 5; i++) {
		float slantY = 0.0f;
		for (int j = 0; j < 5; j++) {
			sPoint& pt = m_boardMatchMap[m_board[i][j]];
			src.x = pt.x;
			src.y = pt.y;

			float x = slantX + rect.x + rowX + columnX + m_completeBarPos.x;
			float y = rect.y + m_completeBarPos.y + slantY + slantRowY + rowY;
			m_board[i].GetBingoPoint(j).DrawPoint(pGI, m_bitmaps, src, x, y);

			columnX = (src.w + 9.0f) * (j + 1);
			slantX += 4.0f;
			slantY -= 8.0f;
		}

		rowX = 9.0f * (i + 1);
		rowY = (src.h + 18.0f) * (i + 1);
		slantRowY += 2.0f;
		columnX = 0.0f;
		slantX = 0.0f;
	}
}

// bingoeventdlg.cpp:640
void FrBingoEventDlg::DrawRandomNumber(FrGraphicInterface* pGI, const WRect& rect) {
	WRect src(0.0f, 0.0f, 48.0f, 37.0f);
	sPoint& pt = m_numberMatchMap[m_selectedNumber];
	src.x = pt.x;
	src.y = pt.y;

	pGI->DrawTexture(m_bitmaps[1], src, WRect(rect.x + m_randomNumberPos.x, rect.y + m_randomNumberPos.y, src.w, src.h), 0xFFFFFFFF, 0);
}

// bingoeventdlg.cpp:649
void FrBingoEventDlg::DrawTryCount(FrGraphicInterface* pGI, const WRect& rect) {
	WRect src(0.0f, 0.0f, 20.0f, 32.0f);
	float offset = 0.0f;
	int length = strlen(m_tryCountText);
	float x = rect.x + m_tryCountPos.x;
	float y = rect.y + m_tryCountPos.y;

	switch (length) {
	case 1:
		x += 12.0f;
		break;
	case 2:
		x += 3.0f;
		break;
	}

	for (int i = 0; i < length; i++) {
		sPoint& pt = m_charMatchMap[m_tryCountText[i]];
		src.x = pt.x;
		src.y = pt.y;

		pGI->DrawTexture(m_bitmaps[0], src, WRect(x + offset, y, src.w, src.h), 0xFFFFFFFF, 0);

		offset += 20.0f;
	}
}

// bingoeventdlg.cpp:673
void FrBingoEventDlg::DrawCompleteBar(FrGraphicInterface* pGI, const WRect& rect) {
	const Bitmap* pBitmap;
	float width;
	float height;
	float x;
	float y;

	for (int i = 0; i < 12; i++) {
		if (i < 5) {
			pBitmap = m_bitmaps[5];
			width = 221.0f;
			height = 42.0f;
			x = i * 7.5f + 15.0f;
			y = i * 53.0f - 20.0f;
		} else if (i < 10 && i > 4) {
			pBitmap = m_bitmaps[6];
			width = 44.0f;
			height = 219.0f;
			x = (i - 5) * 54.0f + 10.0f;
			y = 15.0f - (i - 5) * 8.0f;
		} else if (i == 10) {
			pBitmap = m_bitmaps[7];
			width = 246.0f;
			x = 20.0f;
			height = 181.0f;
			y = 20.0f;
		} else {
			pBitmap = m_bitmaps[8];
			width = 181.0f;
			x = 50.0f;
			height = 246.0f;
			y = -15.0f;
		}

		if (m_giftValue & NS_BINGO::ns_resultTable[i]) {
			pGI->DrawTexture(pBitmap, WRect(m_completeBarPos.x + rect.x + x, m_completeBarPos.y + rect.y + y, width, height), 0xFFFFFFFF, 0);
		}
	}
}

// bingoeventdlg.cpp:723
void FrBingoEventDlg::DrawBingoLogo(FrGraphicInterface* pGI, const WRect& rect) {
	float x;
	float y;

	unsigned int width = m_bitmaps[9]->Width();
	unsigned int height = m_bitmaps[9]->Height();

	for (int i = 0; i < 12; i++) {
		if (i < 5) {
			x = i * 7.5f + 15.0f + width / 2;
			y = i * 53.0f - 20.0f;
		} else if (i < 10 && i > 4) {
			x = (i - 5) * 54.0f + 10.0f;
			y = 15.0f - (i - 5) * 8.0f + height / 2;
		} else if (i == 10) {
			x = width / 2 + 20.0f;
			y = height / 2 + 20.0f;
		} else {
			x = width / 2 + 50.0f;
			y = height / 2 - 15.0f;
		}

		if (m_selectedValue & NS_BINGO::ns_resultTable[i]) {
			pGI->DrawTexture(m_bitmaps[9], WRect(m_completeBarPos.x + rect.x + x, m_completeBarPos.y + rect.y + y, 67.0f, 32.0f), 0xFFFFFFFF, 0);
		}
	}
}

// bingoeventdlg.cpp:769
void FrBingoEventDlg::OnInitStartButton(intptr_t param) {
	m_pStartButton = DYNAMIC_CAST(FrButton, param);
}

// bingoeventdlg.cpp:774
void FrBingoEventDlg::OnLButtonDownStartButton(void) {
	if (m_selectedFlag == true) {
		return;
	}

	if (m_tryCount < 1) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x23, (intptr_t)"\xC3\xDF\xC3\xB7\xB1\xC7\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}

	m_board.ClearOverProperty();
	m_selectedValue = 0;

	int total = 0;
	for (int i = 0; i < 12; i++) {
		total += NS_BINGO::ns_resultTable[i];
	}

	if (m_giftValue == total) {
		AfxGetTask()->PassMsgToActor("Lobby", MsgObject(0, 0x23, (intptr_t)"\xC0\xCC\xB9\xCC \xB8\xF0\xB5\xE7 \xBC\xB1\xB9\xB0\xC0\xBB \xB4\xD9 \xB9\xDE\xC0\xB8\xBC\xCC\xBD\xC0\xB4\xCF\xB4\xD9.", 0, 0, 0, 0));
		return;
	}

	WSendPacket packet((enumClientPacket)0xE5);
	packet.Encode4(1);
	packet.Send(SEND_TO_DEFAULT);

	m_selectedFlag = true;

	AfxGetTask()->SendMsgToMainActor(MsgObject(0, 1, 0, 0, 0, 0, 0));
}

// bingoeventdlg.cpp:816
void FrBingoEventDlg::OnInitEventDetail(intptr_t param) {
	m_pEventDetail = DYNAMIC_CAST(FrButton, param);
}

// bingoeventdlg.cpp:821
void FrBingoEventDlg::OnLButtonDownEventDetail(void) {
	m_pDetailDlg = CreateForm<FrBingoDetailDlg>(g_pFresh->GetManager(), this, "bingo_detail", 0);
	if (m_pDetailDlg) {
		m_pDetailDlg->Open((FRESH_RESULT_CALLBACK)&FrBingoEventDlg::OnDetailDlgResult, 1);
	}
}

// bingoeventdlg.cpp:830
void FrBingoEventDlg::OnInitBoardNumberArea(intptr_t param) {
	m_pBoardNumberArea = DYNAMIC_CAST(FrArea, param);
}

// bingoeventdlg.cpp:835
void FrBingoEventDlg::OnOwnerDrawBoardNumberArea(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		const WRect& rect = GetRect();

		DrawCompleteBar(pGDI, rect);
		DrawBoardNumber(pGDI, rect);
		DrawRandomNumber(pGDI, rect);
		DrawTryCount(pGDI, rect);
		DrawBingoLogo(pGDI, rect);
	}
}

// bingoeventdlg.cpp:863 (run-time type and factory registration of FrBingoDetailDlg)
const WRTTI FrBingoDetailDlg::m_RTTI("FrBingoDetailDlg", &FrForm::m_RTTI);

IObject* FrBingoDetailDlgMakeInstance(void) {
	return new FrBingoDetailDlg;
}

struct __sFrBingoDetailDlg {
	__sFrBingoDetailDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBingoDetailDlgMakeInstance, "FrBingoDetailDlg");
	}
};
static __sFrBingoDetailDlg s_registerFrBingoDetailDlg;

// bingoeventdlg.cpp:865
const sFRESH_MSGMAP* FrBingoDetailDlg::GetMessageMap(void) const {
	return &FrBingoDetailDlg::_MsgMap;
}

sFRESH_MSGMAP FrBingoDetailDlg::_MsgMap = { &FrForm::_MsgMap, &FrBingoDetailDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBingoDetailDlg::_MsgEntries[] = {
	// bingoeventdlg.cpp:867
	{ "detailview", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoDetailDlg::OnInitExplaneViewer },

	// bingoeventdlg.cpp:869
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bingoeventdlg.cpp:872
FrBingoDetailDlg::FrBingoDetailDlg(void) {
}

// bingoeventdlg.cpp:877
FrBingoDetailDlg::~FrBingoDetailDlg(void) {
}

// bingoeventdlg.cpp:882
void FrBingoDetailDlg::OnInitExplaneViewer(intptr_t param) {
	m_pExplaneViewer = DYNAMIC_CAST(FrViewer, param);
	if (m_pExplaneViewer) {
		m_pExplaneViewer->Open("bingo_explanation.jpg");
	}
}

// bingoeventdlg.cpp:901 (run-time type and factory registration of FrBingoGiftDlg)
const WRTTI FrBingoGiftDlg::m_RTTI("FrBingoGiftDlg", &FrForm::m_RTTI);

IObject* FrBingoGiftDlgMakeInstance(void) {
	return new FrBingoGiftDlg;
}

struct __sFrBingoGiftDlg {
	__sFrBingoGiftDlg(void) {
		ObjectFactory().AddObjectFunctor(FrBingoGiftDlgMakeInstance, "FrBingoGiftDlg");
	}
};
static __sFrBingoGiftDlg s_registerFrBingoGiftDlg;

// bingoeventdlg.cpp:903
const sFRESH_MSGMAP* FrBingoGiftDlg::GetMessageMap(void) const {
	return &FrBingoGiftDlg::_MsgMap;
}

sFRESH_MSGMAP FrBingoGiftDlg::_MsgMap = { &FrForm::_MsgMap, &FrBingoGiftDlg::_MsgEntries[0] };

sFRESH_ENTRY FrBingoGiftDlg::_MsgEntries[] = {
	// bingoeventdlg.cpp:905
	{ "giftList", FRCMD_INIT, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoGiftDlg::OnInitGiftArea },
	// bingoeventdlg.cpp:906
	{ "giftList", FRCMD_OWNERDRAW, FrFuncVI, (FRESH_PMSG)(FRESH_PMSGVI)&FrBingoGiftDlg::OnOwnerDrawGiftArea },

	// bingoeventdlg.cpp:908
	{ 0, FRCMD_NONE, FrFuncNone, (FRESH_PMSG)0 }
};

// bingoeventdlg.cpp:912
FrBingoGiftDlg::FrBingoGiftDlg(void)
	: m_pGiftArea(0) {
	m_startX[0] = 84.0f;
	m_startX[1] = 52.0f;
	m_startX[2] = 21.0f;
	m_startX[3] = -5.0f;
}

// bingoeventdlg.cpp:929
FrBingoGiftDlg::~FrBingoGiftDlg(void) {
}

// bingoeventdlg.cpp:934
void FrBingoGiftDlg::SetGiftData(std::vector<unsigned long>* gifts) {
	static sGift s_gifts[] = {
		sGift(0x1A0000CB, "bingo_item_01"),
		sGift(0x1A000035, "bingo_item_02"),
		sGift(0x18000010, "bingo_item_03"),
		sGift(0x18000011, "bingo_item_04"),
		sGift(0x18000012, "bingo_item_05"),
		sGift(0x10000015, "bingo_item_06"),

		sGift(0x24410006, "bingo_item_07")
	};

	std::map<unsigned long, const char*> names;
	int i;

	for (i = 0; i < 7; i++) {
		names[s_gifts[i].id] = s_gifts[i].name;
	}

	m_gifts.assign(gifts->begin(), gifts->end());

	int count = m_gifts.size();
	for (i = 0; i < count; i++) {
		m_giftBitmaps[i] = g_pFresh->RegisterBitmap(names[m_gifts[i]]);
	}
}

// bingoeventdlg.cpp:963
void FrBingoGiftDlg::OnInitGiftArea(intptr_t param) {
	m_pGiftArea = DYNAMIC_CAST(FrArea, param);
}

// bingoeventdlg.cpp:968
void FrBingoGiftDlg::OnOwnerDrawGiftArea(int param) {
	FrGraphicInterface* pGDI = g_pFresh->GetManager()->GetGDI();
	if (pGDI) {
		float offset = 0.0f;
		const WRect& rect = m_pGiftArea->GetRect();
		int count = m_gifts.size();

		for (int i = 0; i < count; i++) {
			const Bitmap* pBitmap = m_giftBitmaps[i];
			pGDI->DrawTexture(pBitmap, WRect(offset + rect.x + m_startX[count - 1], rect.y, (float)pBitmap->Width(), (float)pBitmap->Height()), 0xFFFFFFFF, 0);

			offset += 63.0f;
		}
	}
}
