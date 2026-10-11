// Original: d:\build\custom\temp\code\client\projectg\indexcontrol.cpp
#include <string.h>

#include "IndexControl.h"
#include "fresh.h"
#include "frgraphicinterface.h"
#include "frwndmanager.h"

// indexcontrol.cpp:27
CIndexControl::CIndexControl(void) : m_indexRect(0.0f, 0.0f, 30.0f, 10.0f) {
	ClearVariables();
}

// indexcontrol.cpp:35
CIndexControl::~CIndexControl(void) {
	if (m_indexRects) {
		delete[] m_indexRects;
		m_indexRects = NULL;
	}
}

// indexcontrol.cpp:47
void CIndexControl::ClearVariables(void) {
	m_enabled = false;
	m_align = 1;
	m_color = 0xffff0000;
	m_selectedColor = 0xffaa1111;
	m_disabledColor = 0xffaaaaaa;
	m_totalCount = 0;
	m_totalPage = 0;
	m_curIndexCount = 0;
	m_countPerPage = 0;
	m_curPage = 0;
	m_selectedIndex = 0;
	m_indexRects = NULL;

	m_drawRect = m_indexRect;

	m_pos = WPoint(0.0f, 0.0f);
}

// indexcontrol.cpp:68
bool CIndexControl::Init(unsigned long totalCount, unsigned long countPerPage) {
	bool result = false;

	if (totalCount > 0 && countPerPage > 0) {
		m_totalCount = totalCount;
		m_countPerPage = countPerPage;
		m_totalPage = totalCount / countPerPage;

		if (totalCount % countPerPage != 0) {
			m_totalPage++;
		}

		result = BuildPage(0);

		if (result == true) {
			m_enabled = result;
		}
	}

	return result;
}

// indexcontrol.cpp:99
bool CIndexControl::BuildPage(unsigned long page) {
	bool result = false;

	if (page < m_totalPage) {
		m_curPage = page;
		if (m_totalCount - m_countPerPage * page >= m_countPerPage) {
			m_curIndexCount = m_countPerPage;
		} else {
			unsigned long rest = m_totalCount % m_countPerPage;
			if (rest != 0) {
				m_curIndexCount = rest;
			}
		}

		if (m_curIndexCount > 0) {
			if (m_indexRects) {
				delete[] m_indexRects;
				m_indexRects = NULL;
			}

			m_indexRects = new WRect[m_curIndexCount];

			if (m_indexRects != NULL) {
				memset(m_indexRects, 0, m_curIndexCount * sizeof(WRect));

				for (int i = 0; i < m_curIndexCount; i++) {
					WRect rect = m_drawRect;
					WRect& indexRect = m_indexRects[i];

					indexRect.x = i * rect.w;
					indexRect.y = rect.y;
					indexRect.w = indexRect.x + rect.w;
					indexRect.h = rect.h;
				}

				m_selectedIndex = 0;
			} else {
				return false;
			}

			result = true;
		}
	}

	return result;
}

// indexcontrol.cpp:164
void CIndexControl::Draw(WPoint& pos) {
	if (m_enabled) {
		FrGraphicInterface* pGI = g_pFresh->GetManager()->GetGDI();

		if (pGI == NULL) {
			return;
		}

		m_pos = pos;
		WPoint drawPos = pos;

		float width = m_curIndexCount * m_indexRect.w;

		switch (m_align) {
		case 1:
			drawPos.x -= width * 0.5f;
			break;
		case 2:
			drawPos.x -= width;
			break;
		}

		for (unsigned long i = 0; i < m_curIndexCount; i++) {
			WRect rect = WRect(m_indexRects[i]) + WRect(drawPos.x, drawPos.y, drawPos.x, drawPos.y);

			if (i != 0) {
				pGI->SetTextColor(m_disabledColor, 0xffffffff);
				pGI->Print(rect.TopLeft(), 1, "-");
			}

			const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();

			if (mouse.x >= rect.x && mouse.x <= rect.w && mouse.y >= rect.y && mouse.y <= rect.h) {
				m_hoverIndex = i;
			} else {
				m_hoverIndex = m_selectedIndex;
			}

			rect.x += m_indexRect.w * 0.5f;
			if (i == m_selectedIndex) {
				pGI->SetTextColor(m_color, 0xffffffff);
				pGI->SetTextStyle(1);
			} else if (i == m_hoverIndex) {
				pGI->SetTextColor(m_selectedColor, 0xffffffff);
				pGI->SetTextStyle(1);
			} else {
				pGI->SetTextColor(0xff000000, 0xffffffff);
				pGI->SetTextStyle(0);
			}

			pGI->Print(rect.TopLeft(), 1, "%d", m_countPerPage * m_curPage + i + 1);

			pGI->SetTextColor(0xff000000, 0xffffffff);
			pGI->SetTextStyle(0);
		}
	}
}

// indexcontrol.cpp:251
bool CIndexControl::Click(void) {
	bool result = false;
	WPoint pos = m_pos;

	float width = m_curIndexCount * m_indexRect.w;

	switch (m_align) {
	case 1:
		pos.x -= width * 0.5f;
		break;
	case 2:
		pos.x -= width;
		break;
	}

	for (unsigned long i = 0; i < m_curIndexCount; i++) {
		const WPoint& mouse = g_pFresh->GetManager()->GetMousePos();
		WRect rect = WRect(m_indexRects[i]) + WRect(pos.x, pos.y, pos.x, pos.y);

		if (mouse.x >= rect.x && mouse.x <= rect.w && mouse.y >= rect.y && mouse.y <= rect.h) {
			if (m_selectedIndex != i) {
				m_selectedIndex = i;
				result = true;
				break;
			}
		}
	}

	return result;
}

// indexcontrol.cpp:304
bool CIndexControl::SelectIndex(unsigned long index) {
	bool result = false;

	if (m_selectedIndex != index) {
		m_selectedIndex = index;
		result = true;
	}

	return result;
}

// indexcontrol.cpp:320
bool CIndexControl::NextPage(void) {
	bool result = false;

	if (m_totalPage > m_curPage + 1) {
		m_curPage = m_curPage + 1;
		result = true;
		BuildPage(m_curPage);
	}

	return result;
}

// indexcontrol.cpp:338
bool CIndexControl::PrevPage(void) {
	bool result = false;

	if (m_curPage > 0) {
		m_curPage = m_curPage - 1;
		result = true;
		BuildPage(m_curPage);
	}

	return result;
}

// indexcontrol.cpp:356
void CIndexControl::SetSelectedNumber(unsigned long number) {
	if (number <= 0) {
		return;
	}

	unsigned long page;
	unsigned long index;
	if (--number >= m_countPerPage) {
		page = number / m_countPerPage;
		index = number % m_countPerPage;
	} else {
		page = 0;
		index = number;
	}

	BuildPage(page);
	SelectIndex(index);
}

// indexcontrol.cpp:383
unsigned long CIndexControl::GetSelectedNumber(void) const {
	return m_curPage * m_countPerPage + m_selectedIndex + 1;
}

// indexcontrol.cpp:390
unsigned long CIndexControl::GetSelectedIndex(void) const {
	return m_selectedIndex;
}

// indexcontrol.cpp:397
unsigned long CIndexControl::GetCurIndexCount(void) const {
	return m_curIndexCount;
}

// indexcontrol.cpp:404
unsigned long CIndexControl::GetTotalPage(void) const {
	return m_totalPage;
}
