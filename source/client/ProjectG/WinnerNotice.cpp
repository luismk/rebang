// Original: d:\build\custom\temp\code\client\projectg\winnernotice.cpp
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

#include "WinnerNotice.h"
#include "ChatMsg.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "tinyxml.h"
#include "fresh.h"
#include "frwndmanager.h"
#include "frgraphicinterface.h"
#include "wresrcmng.h"
#include "wfont.h"
#include "wview.h"
#include "CapturedBg.h"
#include "SoundManager.h"
#include "binstr.h"
#include "frwnd.h"
#include "fremoticon.h"
#include "ProjectG.h"
#include "TaskManager.h"
#include "GolfTask.h"
#include "onelineboard.h"
#include "gamedatadb.h"
#include "gdeventnotice.h"

// winnernotice.cpp:18
CWinnerNotice::CWinnerNotice(void) {
	WinnerNotice_Init();
}

// winnernotice.cpp:23
CWinnerNotice::~CWinnerNotice(void) {
	WinnerNotice_Clear();
}

// winnernotice.cpp:28
void CWinnerNotice::WinnerNotice_Init(void) {
	m_pOverlayLeft = g_resrcmng->GetOverlay("notice_l.tga", 0);
	m_pOverlayMid = g_resrcmng->GetOverlay("notice_m.tga", 0);
	m_pOverlayRight = g_resrcmng->GetOverlay("notice_r.tga", 0);

	m_pFont = CChatMsg::Instance()->GetMaskedFont();
	m_current = m_noticeList.end();
	m_posX = -5555.0f;
	m_posY = -50.0f;

	m_pEventNotice = NULL;
	m_state = 0;
}

// winnernotice.cpp:43
void CWinnerNotice::WinnerNotice_Clear(void) {
	m_current = m_noticeList.end();
	sequence_delete(m_noticeList.begin(), m_noticeList.end());

	if (g_resrcmng != NULL && m_pOverlayLeft != NULL) { g_resrcmng->Release(m_pOverlayLeft); m_pOverlayLeft = NULL; }
	if (g_resrcmng != NULL && m_pOverlayMid != NULL) { g_resrcmng->Release(m_pOverlayMid); m_pOverlayMid = NULL; }
	if (g_resrcmng != NULL && m_pOverlayRight != NULL) { g_resrcmng->Release(m_pOverlayRight); m_pOverlayRight = NULL; }
}

// winnernotice.cpp:53
void CWinnerNotice::Process(float elapsed) {
	elapsed /= WSingleton<CProjectG>::Instance()->GetGameSpeed();

	if (m_current == m_noticeList.end()) {
		m_current = m_noticeList.begin();
		if (m_current == m_noticeList.end()) {
			if (m_posY > -50.0f) {
				m_posY -= elapsed * 30.0f;
				if (m_posY < -50.0f) {
					m_posY = -50.0f;
				}
			}
			if ((__rtti_obj = AfxGetTask()) != 0 && ((IObject*)__rtti_obj)->IsKindOf(&CGolfTask::m_RTTI)) {
				WSingleton<COneLineBoard>::Instance()->FadeIn();
			}
			return;
		}
	}

	if (CCapturedBg::Instance() != NULL && CCapturedBg::Instance()->IsCaptured()) {
		FrEmoticon* emoticon = g_pFresh->GetManager()->GetEmoticon();
		if (emoticon != NULL) {
			emoticon->SetAnim(false);
		}
		return;
	}

	if (m_posY < -0.1f) {
		m_posY += elapsed * 30.0f;
		if (!(m_posY > 0.0f)) {
			return;
		}
	}

	m_posY = 0.0f;
	sWinnerNotice* notice = *m_current;
	if (m_posX < -5000.0f) {
		m_posX = g_view->GetWidth() * 0.5f + 150.0f;
	}
	m_posX -= elapsed * 30.0f;

	float center = g_view->GetWidth() * 0.5f;
	if (m_posX + notice->m_width < center - 150.0f) {
		m_posX = center + 150.0f;
		if (--notice->m_repeatCount < 1) {
			if (*m_current != NULL) {
				delete *m_current;
			}
			m_current = m_noticeList.erase(m_current);
		} else {
			++m_current;
		}
	}
}

// winnernotice.cpp:129
void CWinnerNotice::Display(void) {




	FrGraphicInterface* gdi = g_pFresh->GetManager()->GetGDI();

	if (m_pOverlayLeft && m_pOverlayMid && m_pOverlayRight) {

		float alpha = (m_posY + 50.0f) * 0.02f;
		if (WAbsBySign(alpha) < WMATH_EPSILON)
			return;



		_WRECT src = { 0.0f, 0.0f, 1.0f, 1.0f };
		_WRECT dst = { g_view->GetWidth() * 0.5f - 150.0f - 20.0f, m_posY, (float)m_pOverlayLeft->GetWidth(), (float)m_pOverlayLeft->GetHeight() };
		m_pOverlayLeft->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);

		dst.x += dst.w;
		dst.w = 300.0f - m_pOverlayRight->GetWidth() + 20.0f;
		dst.h = (float)m_pOverlayMid->GetHeight();
		m_pOverlayMid->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);

		dst.x += dst.w;
		dst.w = (float)m_pOverlayRight->GetWidth();
		dst.h = (float)m_pOverlayRight->GetHeight();
		m_pOverlayRight->Render(g_view, src, dst, 0, 0xA0FFFFFF, 0.0f, 0);

		if ((__rtti_obj = AfxGetTask()) != 0 ? ((IObject*)__rtti_obj)->IsExactKindOf(&CGolfTask::m_RTTI) : false) {
			gdi->SetTextStyle(0);

			gdi->SetTextColor(0xFFFFFFFF, 0);
		} else {
			gdi->SetTextStyle(0);
		}



		if (m_current != m_noticeList.end()) {

			sWinnerNotice* notice = *m_current;


			FrEmoticon* emoticon = g_pFresh->GetManager()->GetEmoticon();
			WRect clip(g_view->GetWidth() * 0.5f - 150.0f, 0.0f, 300.0f, 40.0f);
			emoticon->SetClippingArea(&clip);

			if (emoticon) {

				emoticon->SetAnim(true);
			}

			emoticon->SetAnimTime(notice->m_startTime);

			for (int i = 0; i < notice->m_msgCount; i++) {

				gdi->SetTextColor(0xFFFFFFFF, notice->m_msg[i].m_color);

				gdi->SetTextStyle(notice->m_msg[i].m_style);
				emoticon->PrintText(WPoint((int)m_posX + GetTextTailPoint(i), 7.0f), 0, notice->m_msg[i].m_text.c_str(), 0xFFFFFFFF);
			}


			emoticon->SetAnim(false);
			emoticon->SetClippingArea(0);
		}


		gdi->SetTextColor(0xFFFFFFFF, 0xFFFFFFFF);
		gdi->SetTextStyle(0);
	}
}

// winnernotice.cpp:199
bool CWinnerNotice::GetColornText(const char* pszText, std::string& text, unsigned long& color) {
	color = 0xFFFFFFFF;
	cTokenV token;
	if (token.Init(pszText, strlen(pszText))) {
		char* pToken;
		bool ret = token.GetTokenFullSep(&pToken, "\\c", 2);
		if (pToken == 0 || *pToken == 0) {
			if (!ret) {
				return false;
			}
		}
		if (ret) {
			token.GetTokenFullSep(&pToken, "\\c", 2);

			unsigned long value = 0xFFFFFFFF;
			cTokenV colorToken;
			bool more = colorToken.Init(pToken, strlen(pToken));
			while (more) {
				more = colorToken.GetToken(&pToken, ",", 1);
				if (pToken != 0 && *pToken != 0) {
					if (strlen(pToken) > 1) {
						sscanf(pToken, "%x", &value);
					}
				}
			}
			color = value;

			std::string str(pszText);
			text = str.substr(str.rfind("\\c") + 2);
		}
		else {
			text = pszText;
		}
		return true;
	}
	return false;
}

// winnernotice.cpp:261
float CWinnerNotice::GetTextTailPoint(int count) {
	FrWndManager* manager = g_pFresh->GetManager();

	sWinnerNotice* notice = *m_current;
	FrGraphicInterface* gdi = manager->GetGDI();
	float tail = 0.0f;

	for (int i = 0; i < count; i++) {
		tail += gdi->GetTextExtend(notice->m_msg[i].m_text.c_str());
	}

	return tail;
}

// winnernotice.cpp:277
void CWinnerNotice::AddWinnerNotice(int repeatCount) {
	sWinnerNotice* notice = new sWinnerNotice;

	notice->clear_data();
	notice->m_msgCount = m_notice.m_msgCount;

	notice->m_repeatCount = repeatCount;
	for (int i = 0; i < m_notice.m_msgCount; i++) {
		notice->m_msg[i].m_color = m_notice.m_msg[i].m_color;
		notice->m_msg[i].m_style = m_notice.m_msg[i].m_style;
		notice->m_msg[i].m_text = m_notice.m_msg[i].m_text;
		notice->m_width += m_pFont->GetTextWidth(g_view, m_notice.m_msg[i].m_text.c_str());
	}
	notice->m_startTime = timeGetTime();

	size_t prevCount = m_noticeList.size();

	m_noticeList.push_back(notice);

	if (CCapturedBg::Instance() == NULL || !CCapturedBg::Instance()->IsCaptured()) {
		if (prevCount == 0 && m_noticeList.size() == 1 && m_posY < -0.1f) {
			g_audio->PlaySfx("goldentime_notice", NULL, 0, NULL, NULL, 0.5f, 200.0f);
		}
	}
}

// winnernotice.cpp:310
void CWinnerNotice::SetWinnerData(int type, std::string data) {
	if (m_pEventNotice == NULL) {
		m_pEventNotice = (CGDEventNotice*)CGameDataDB::Instance()->GetGameData(eGAMEDATADB_EVENTNOTICE, "WinnerEventNotice");
	}

	TiXmlDocument doc;
	ParseXmlData(doc, data.c_str());

	sWinnerNotice* notice = &m_notice;
	notice->clear_data();

	int count = 0;
	switch (type) {
	case 11:
		GetColornText(m_pEventNotice->GetText(1), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(2), notice->m_msg[2].m_text, notice->m_msg[2].m_color);

		notice->m_msg[3].m_color = 0xFFFF0000;
		notice->m_msg[3].m_style = 2;
		notice->m_msg[3].m_text = GetItemNamefromXml(doc);

		notice->m_msg[4].m_color = 0xFFFF0000;
		notice->m_msg[4].m_style = 2;
		GetItemCountfromXml(doc, notice->m_msg[4].m_text);

		GetColornText(m_pEventNotice->GetText(3), notice->m_msg[5].m_text, notice->m_msg[5].m_color);
		count = 6;
		break;
	case 12:
		GetColornText(m_pEventNotice->GetText(4), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(5), notice->m_msg[2].m_text, notice->m_msg[2].m_color);

		notice->m_msg[3].m_color = 0xFFFF0000;
		notice->m_msg[3].m_style = 2;
		notice->m_msg[3].m_text = GetItemNamefromXml(doc);

		notice->m_msg[4].m_color = 0xFFFF0000;
		notice->m_msg[4].m_style = 2;
		GetItemCountfromXml(doc, notice->m_msg[4].m_text);

		GetColornText(m_pEventNotice->GetText(6), notice->m_msg[5].m_text, notice->m_msg[5].m_color);
		count = 6;
		break;
	case 13:
		GetColornText(m_pEventNotice->GetText(7), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(8), notice->m_msg[2].m_text, notice->m_msg[2].m_color);
		count = 3;
		break;
	case 14:
		GetColornText(m_pEventNotice->GetText(9), notice->m_msg[0].m_text, notice->m_msg[0].m_color);
		GetColornText(m_pEventNotice->GetText(10), notice->m_msg[1].m_text, notice->m_msg[1].m_color);

		notice->m_msg[2].m_color = 0xFF050A9B;
		notice->m_msg[2].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[2].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(11), notice->m_msg[3].m_text, notice->m_msg[3].m_color);

		notice->m_msg[4].m_color = 0xFFFF0000;
		notice->m_msg[4].m_style = 2;
		notice->m_msg[4].m_text = GetItemNamefromXml(doc);

		notice->m_msg[5].m_color = 0xFFFF0000;
		notice->m_msg[5].m_style = 2;
		GetItemCountfromXml(doc, notice->m_msg[5].m_text);

		GetColornText(m_pEventNotice->GetText(12), notice->m_msg[6].m_text, notice->m_msg[6].m_color);

		notice->m_msg[7].m_color = 0xFFFF9BCD;
		notice->m_msg[7].m_style = 2;
		GetUserNamefromXml(doc, notice->m_msg[7].m_text);

		GetColornText(m_pEventNotice->GetText(13), notice->m_msg[8].m_text, notice->m_msg[8].m_color);
		count = 9;
		break;
	case 15:
		GetColornText(m_pEventNotice->GetText(14), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(15), notice->m_msg[2].m_text, notice->m_msg[2].m_color);

		notice->m_msg[3].m_color = 0xFF050A9B;
		notice->m_msg[3].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[3].m_text, EVENT_TIME_HOUR, true);

		GetColornText(m_pEventNotice->GetText(16), notice->m_msg[4].m_text, notice->m_msg[4].m_color);
		count = 5;
		break;
	case 16:
		GetColornText(m_pEventNotice->GetText(17), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_DAY, false);

		GetColornText(m_pEventNotice->GetText(18), notice->m_msg[2].m_text, notice->m_msg[2].m_color);

		notice->m_msg[3].m_color = 0xFF050A9B;
		notice->m_msg[3].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[3].m_text, EVENT_TIME_DAY, true);

		notice->m_msg[4].m_color = 0xFF050A9B;
		notice->m_msg[4].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[4].m_text, EVENT_TIME_HOUR, true);

		GetColornText(m_pEventNotice->GetText(19), notice->m_msg[5].m_text, notice->m_msg[5].m_color);
		count = 6;
		break;
	case 17:
		GetColornText(m_pEventNotice->GetText(20), notice->m_msg[0].m_text, notice->m_msg[0].m_color);
		count = 1;
		break;
	case 18:
		GetColornText(m_pEventNotice->GetText(21), notice->m_msg[0].m_text, notice->m_msg[0].m_color);

		notice->m_msg[1].m_color = 0xFF050A9B;
		notice->m_msg[1].m_style = 2;
		GetDaynTimefromXml(doc, notice->m_msg[1].m_text, EVENT_TIME_HOUR, false);

		GetColornText(m_pEventNotice->GetText(22), notice->m_msg[2].m_text, notice->m_msg[2].m_color);
		count = 3;
		break;
	}

	notice->m_msgCount = count + 1;
	AddWinnerNotice(1);
}

// winnernotice.cpp:505
void CWinnerNotice::ParseXmlData(TiXmlDocument& doc, const char* xml) {
	doc.Clear();
	doc.Parse(xml);
}

// winnernotice.cpp:511
void CWinnerNotice::GetEventNumfromXml(TiXmlDocument& doc, std::string& out, bool next) {
	char buf[50] = { 0, };
	TiXmlElement* element;
	if (next) {
		element = doc.FirstChildElement("PARAMS")->FirstChildElement("NEXTTIMES");
	} else {
		element = doc.FirstChildElement("PARAMS")->FirstChildElement("TIMES");
	}

	const char* times = NULL;
	if (element != NULL) {
		times = element->FirstChild()->Value();
	}

	sprintf(buf, "%s \xc2\xf7\xbc\xf6 \xc0\xcc\xba\xa5\xc6\xae", times ? times : "0");
	out = buf;
}

// winnernotice.cpp:535
void CWinnerNotice::GetDaynTimefromXml(TiXmlDocument& doc, std::string& out, EventTimeFlag flag, bool next) {
	char buf[100] = { 0, };
	TiXmlElement* element;
	if (next) {
		element = doc.FirstChildElement("PARAMS")->FirstChildElement("EVENTNEXTTIME");
	} else {
		element = doc.FirstChildElement("PARAMS")->FirstChildElement("EVENTSTARTTIME");
	}

	if (element == NULL) {
		out = "0";
		return;
	}

	const char* text = element->FirstChild()->Value();
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;

	sscanf(text, "%d-%d-%d %d:%d:%d.%d", &year, &month, &day, &hour, &minute, &second, &second);

	switch (flag) {
	case EVENT_TIME_DAY:
		sprintf(buf, "%d \xbf\xf9 %d \xc0\xcf", month, day);
		break;

	case EVENT_TIME_HOUR:
		sprintf(buf, "%02d\xbd\xc3%02d\xba\xd0", hour, minute);
		break;
	}

	out = buf;
}

// winnernotice.cpp:579
const char* CWinnerNotice::GetItemNamefromXml(TiXmlDocument& doc) {
	TiXmlElement* element = doc.FirstChildElement("PARAMS")->FirstChildElement("TYPEID");
	if (element == NULL) {
		return "";
	}
	int typeId = atoi(element->FirstChild()->Value());
	return ItemManager()->GetItemName(typeId);
}

// winnernotice.cpp:590
void CWinnerNotice::GetItemCountfromXml(TiXmlDocument& doc, std::string& out) {
	TiXmlElement* qty = doc.FirstChildElement("PARAMS")->FirstChildElement("QTY");
	TiXmlElement* typeId = doc.FirstChildElement("PARAMS")->FirstChildElement("TYPEID");

	if (typeId == NULL || qty == NULL) {
		out = "";
		return;
	}

	char buf[100] = { 0, };

	if (atoi(typeId->FirstChild()->Value()) == 0x1A000010) {
		sprintf(buf, "(%d \xc6\xce)", atoi(qty->FirstChild()->Value()));
		out = buf;
	} else if (atoi(qty->FirstChild()->Value()) > 1) {
		sprintf(buf, "(%d)", atoi(qty->FirstChild()->Value()));
		out = buf;
	} else {
		out = "";
	}
}

// winnernotice.cpp:625
void CWinnerNotice::GetUserNamefromXml(TiXmlDocument& doc, std::string& out) {
	std::string name;
	TiXmlElement* element = doc.FirstChildElement("PARAMS")->FirstChildElement("NICKNAME");

	if (element == NULL) {
		out = "";
		return;
	}

	do {
		char* nick = (char*)element->FirstChild()->Value();

		NickNameEdit(nick, true, name);

		out += "(" + name + ")";
	} while ((element = element->NextSiblingElement()) != NULL);
}

// winnernotice.cpp:652
void CWinnerNotice::NickNameEdit(char* name, bool cut, std::string& out) {
	std::string result;
	std::string mask("***");
	char buf[50] = { 0, };

	result = "";

	if (cut) {
		mbscut(buf, name, strlen(name) - 2);
		result = buf;
		result += mask;
		out = result;
	} else {
		result = name + 2;
		out = mask + result;
	}
}
