#pragma once

struct sChatCmd
{
	char name[20];
};

extern sChatCmd ChatCmd[];

class ChatManager
{
public:
	ChatManager();
	virtual ~ChatManager();

	bool Load();
	void ParseString(const char* str);
	void CheckPenaltyTime();
	void SendChatMessage(const char* nick, const char* msg, bool bWhisper);
	void SendChatMessage2(const char* nick, const char* msg, bool bWhisper);
	bool Filtering(char* str);
	int GetFaceIndex(const char* str);
	const char* GetMotionName(const char* str, unsigned long uid);
	void FilteringHack(const char* str, bool bChat);
	const char* FilteringNick(char* nick);
	int FilteringGuildString(const char* str, int minLen, int maxLen,
		bool bOption);
	const char* FilteringEmoticon(unsigned short* str);
	int CheckChatCommand(const char* str, int type);
	int SaveChatHistory(int from, int to);
	void Display(const char* str, int type);

private:
	bool LoadChatFilter();
	bool LoadNickFilter();
	void RecordTournament();
	const char* GetAlignedString(const char* str, int len, int width);
	void RecordMyChatHistory(const char* str);

	struct sNickFilter
	{
		char type;
		char word[30];
	};

	char (*m_chatFilter)[30];
	sNickFilter* m_nickFilter;
	int m_chatFilterNum;
	int m_nickFilterNum;
	unsigned long m_lastChatTime;
	int m_swearCount;
	int m_floodCount;
	bool m_bSwearPenalty;
	bool m_bFloodPenalty;
	bool m_bUnused;
	unsigned char m_reserved[0x15];
};
