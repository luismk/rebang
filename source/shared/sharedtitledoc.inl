// Original: d:\build\custom\temp\code\shared\sharedtitledoc.inl
// Title conditions shared with the server; included by the units that
// define titles (Titles_Client).

#include <math.h>

// Player level thresholds (CheckLevel); the original header and names are not known.
enum eLevel {
};

float RecalculatePercent(float percent);

// sharedtitledoc.inl:8
bool CheckLevel(unsigned long level, eLevel required) {
	if (level >= (unsigned long)required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:18
bool CheckPangyaPercent(unsigned long pangya, unsigned long shots, float required) {
	float percent = (float)pangya / (float)shots;
	if (RecalculatePercent(percent * 100.0f) >= required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:28
bool CheckFairawayPercent(unsigned long fairway, unsigned long drives, unsigned long par3, float required) {
	float percent = (float)fairway / ((float)drives - (float)par3);
	if (RecalculatePercent(percent * 100.0f) >= required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:38
bool CheckPuttinPercent(unsigned long putts, unsigned long attempts, float required) {
	float percent = (float)putts / (float)attempts;

	if (RecalculatePercent(percent * 100.0f) >= required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:48
bool CheckNoMannerPercent(unsigned long noManner, unsigned long games, float required) {
	float percent = (float)noManner / (float)games;
	if (RecalculatePercent(percent * 100.0f) <= required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:58
bool CheckGoldMedal(sTrophyStatistics &trophy, unsigned long required) {
	unsigned long total = 0;
	for (int course = 0; course < 13; ++course)
	{
		total += trophy.m_medals[course][0];
	}

	if (total >= required)
	{
		return true;
	}
	return false;
}

// sharedtitledoc.inl:75
bool CheckSilverMedal(sTrophyStatistics &trophy, unsigned long required) {
	unsigned long total = 0;
	for (int course = 0; course < 13; ++course)
	{
		total += trophy.m_medals[course][1];
	}

	if (total >= required)
	{
		return true;
	}
	return false;
}

// sharedtitledoc.inl:93
bool CheckBronzeMedal(sTrophyStatistics &trophy, unsigned long required) {
	unsigned long total = 0;
	for (int course = 0; course < 13; ++course)
	{
		total += trophy.m_medals[course][2];
	}

	if (total >= required)
	{
		return true;
	}
	return false;
}

// sharedtitledoc.inl:204
float RecalculatePercent(float percent) {
	return (float)floor((percent + 0.05f) * 10.0f) * 0.1f;
}

// sharedtitledoc.inl:210
bool CheckPang(__int64 pang, __int64 required) {
	if (pang >= required) {
		return true;
	}
	return false;
}

// sharedtitledoc.inl:219
bool CheckHoleinCount(unsigned long holeIns, unsigned long required) {
	if (holeIns >= required) {
		return true;
	}
	return false;
}
