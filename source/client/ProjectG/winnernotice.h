#pragma once

#include <string>
#include <list>

class CGDEventNotice;
class TiXmlDocument;

class CWinnerNotice : public WSingleton<CWinnerNotice>
{
public:
	CWinnerNotice();
	virtual ~CWinnerNotice();

	void Process(float elapsed);
	void Display();
	float GetTextTailPoint(int count);

	void AddWinnerNotice(int repeat);
	void SetWinnerData(int type, std::string data);
	bool GetColornText(const char* src, std::string& text,
		unsigned long& color);

	enum EventTimeFlag
	{
		EVENT_DAY,
		EVENT_TIME,
	};

protected:
	void WinnerNotice_Init();
	void WinnerNotice_Clear();

	void ParseXmlData(TiXmlDocument& doc, const char* xml);
	void GetEventNumfromXml(TiXmlDocument& doc, std::string& out, bool bNext);
	void GetDaynTimefromXml(TiXmlDocument& doc, std::string& out,
		EventTimeFlag flag, bool bNext);
	void GetItemCountfromXml(TiXmlDocument& doc, std::string& out);
	void NickNameEdit(char* name, bool bFirst, std::string& out);
	const char* GetItemNamefromXml(TiXmlDocument& doc);
	void GetUserNamefromXml(TiXmlDocument& doc, std::string& out);

public:
	struct sNoticeMsg
	{
		unsigned long color;
		unsigned long style;
		std::string msg;

		void initdata()
		{
			color = 0xffffffff;
			style = 0;
			msg = "";
		}
	};

	struct sWinnerNotice
	{
		sNoticeMsg msg[10];
		std::string extra0;
		std::string extra1;
		std::string extra2;
		std::string extra3;
		int repeat;
		float width;
		unsigned long startTime;
		int reserved0;
		int reserved1;
		int reserved2;
		int reserved3;
		int msgCount;

		void clear_data()
		{
			memset(msg, 0, sizeof(msg));

			for (int i = 0; i < 10; ++i)
				msg[i].initdata();

			extra0 = "";
			extra1 = "";
			extra2 = "";

			repeat = 1;
			width = 0;

			startTime = 0;

			reserved0 = 0;
			reserved1 = 0;
			reserved2 = 0;
			reserved3 = 0;
			msgCount = 0;
		}
	};

protected:
	WFont* m_pFont;
	WOverlay* m_pNoticeL;
	WOverlay* m_pNoticeM;
	WOverlay* m_pNoticeR;
	std::list<sWinnerNotice*> m_noticeList;
	std::list<sWinnerNotice*>::iterator m_itrCurrent;
	float m_fTextPosX;
	float m_fBgPosY;
	CGDEventNotice* m_pEventNotice;
	sWinnerNotice m_winnerNotice;
	int m_reserved;
};
