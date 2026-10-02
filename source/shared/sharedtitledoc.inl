float RecalculatePercent(float Value);

bool CheckLevel(unsigned long level, eLevel limitlevel)
{
	if (level >= limitlevel)
		return true;
	return false;
}

bool CheckPangyaPercent(unsigned long pangya, unsigned long drive,
	float limitpercent)
{
	float percent = (float)pangya / (float)drive * 100;
	if (RecalculatePercent(percent) >= limitpercent)
		return true;
	return false;
}

bool CheckFairawayPercent(unsigned long dwFair, unsigned long hole,
	unsigned long matchhole, float limitpercent)
{
	float percent = (float)dwFair / ((float)hole - (float)matchhole) * 100;
	if (RecalculatePercent(percent) >= limitpercent)
		return true;
	return false;
}

bool CheckPuttinPercent(unsigned long puttin, unsigned long putt,
	float limitpercent)
{
	float percent = (float)puttin / (float)putt * 100;
	if (RecalculatePercent(percent) >= limitpercent)
		return true;
	return false;
}

bool CheckNoMannerPercent(unsigned long nomannergamecount,
	unsigned long gamecount, float limitpercent)
{
	float percent = (float)nomannergamecount / (float)gamecount * 100;
	if (RecalculatePercent(percent) < limitpercent)
		return true;
	return false;
}

bool CheckGoldMedal(sTrophyStatistics& TrohpyStatics, unsigned long limitmedal)
{
	unsigned long medal = 0;
	for (int i = 0; i < 13; i++)
	{
		medal += TrohpyStatics.Trophy[i][0];
	}

	if (medal >= limitmedal)
	{
		return true;
	}
	return false;
}

bool CheckSilverMedal(sTrophyStatistics& TrohpyStatics,
	unsigned long limitmedal)
{
	unsigned long medal = 0;
	for (int i = 0; i < 13; i++)
	{
		medal += TrohpyStatics.Trophy[i][1];
	}

	if (medal >= limitmedal)
	{
		return true;
	}
	return false;
}

bool CheckBronzeMedal(sTrophyStatistics& trohpystatics,
	unsigned long limitmedal)
{
	unsigned long medal = 0;
	for (int i = 0; i < 13; i++)
	{
		medal += trohpystatics.Trophy[i][2];
	}

	if (medal >= limitmedal)
	{
		return true;
	}
	return false;
}

bool CheckCourseMaster(sMyInfo& myinfo,
	std::map<unsigned char, sMapStatistics>& MapStatInSeason2,
	std::map<unsigned char, sMapStatistics>& MapStatInSeason3)
{
	bool bCompleteMap[20];

	for (int i = 0; i < 20; i++)
	{
		if (i == 12 || i == 17)
		{
			bCompleteMap[i] = true;
			continue;
		}

		bCompleteMap[i] = false;
		if (myinfo.mapStat[i].cBestScore != 127)
			bCompleteMap[i] = true;
	}

	std::map<unsigned char, sMapStatistics>::iterator iter =
		MapStatInSeason2.begin();
	for (; iter != MapStatInSeason2.end(); ++iter)
	{
		if (bCompleteMap[(*iter).second.bMap] == false)
		{
			if ((*iter).second.cBestScore != 127)
			{
				bCompleteMap[(*iter).second.bMap] = true;
			}
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (myinfo.classicMapStat[i].cBestScore != 127)
			bCompleteMap[i] = true;
	}

	iter = MapStatInSeason3.begin();
	for (; iter != MapStatInSeason3.end(); ++iter)
	{
		if (bCompleteMap[(*iter).second.bMap] == false)
		{
			if ((*iter).second.cBestScore != 127)
			{
				bCompleteMap[(*iter).second.bMap] = true;
			}
		}
	}

	for (int i = 0; i < 20; i++)
	{
		if (bCompleteMap[i] == false)
			return false;
	}

	return true;
}

float RecalculatePercent(float Value)
{
	float percent = floor((Value + 0.05f) * 10.0f) * 0.1f;
	return percent;
}

bool CheckPang(__int64 pang, __int64 limitpang)
{
	if (pang >= limitpang)
	{
		return true;
	}
	return false;
}

bool CheckHoleinCount(unsigned long holein, unsigned long limitholein)
{
	if (holein >= limitholein)
	{
		return true;
	}
	return false;
}
