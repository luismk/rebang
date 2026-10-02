#pragma once

#include <list>

struct sSCardAvilityPeriodInfo;
class FrSPCardBuffDlg;

class CCardManager : public WSingleton<CCardManager>
{
public:
	CCardManager();
	virtual ~CCardManager();

	void Init();
	void CalcCardPeriodAndStatus();

	void SetCharacterCardAvility(const sSCardAvilityPeriodInfo& info, int slot);
	int SetSpecialCardAvility(const sSCardAvilityPeriodInfo& info, int slot);
	void SetCaddieCardAvility(const sSCardAvilityPeriodInfo& info, int slot);

	void SetPlayerIndex(unsigned char index);
	void SetOffLinePlayerIndex(unsigned char index);

	void SetWindColor(unsigned char color) { m_windColor = color; }
	unsigned char GetWindColor() { return m_windColor; }

	int GetTotalRange(bool bPowerRange);
	float GetTotalPangyaZone();
	int GetPowerRangeDown();

	int GetCardPeriodStatus(int index);
	int GetCardStatusSlot(int index);
	float GetCardPeriodComboGauge();
	int GetCardPeriodSlot();
	float GetCardPeriodPangRate();

	int GetCardSuccesssProbUp();
	float GetCardRangeUp();
	__int64 GetCardBounceBonusUp();
	float GetCardPowerRangeUp();
	int GetCardComboGaugeUp();
	float GetCardPangyaZoneUp();
	unsigned char GetWindPowerDown();

	float GetSpecialCardPangyaZoneUp();
	float GetSpecialClearBonusProb(unsigned char course);
	float GetCaddieCardPangYaZoneUpWind1();

	void UpdateCardBuffDlg();
	void InitCardBuffDlgPosition();
	void ResetSpecialBuffTick();

	void SetCardBuffDlg(FrSPCardBuffDlg* pDlg) { m_pCardBuffDlg = pDlg; }
	void SetSPCardAlarm(bool bAlarm) { m_bSPCardAlarm = bAlarm; }
	bool ShowSPCardAlarm() { return m_bSPCardAlarm; }

protected:
	virtual void OnProc(float delta);

public:
	std::list<unsigned long>& GetActiveCardList() { return m_activeCardList; }

private:
	struct sCaddieCardAvility
	{
		int effect;
		int value;
		int type;
	};

public:
	unsigned char m_playerSlot[5];
	unsigned char m_charInfo[5][0x1bc];

private:
	int m_periodStatus[5][5];
	int m_periodSlot[5];
	float m_periodComboGauge[5];
	float m_periodPangRate[5];
	float m_periodPangyaZone[5];
	float m_specialClearBonusProb[5];
	unsigned char m_specialClearBonusCourse;
	int m_cardStatus[5][5];
	int m_powerRangeDown[5];
	unsigned char m_windColor;
	sCaddieCardAvility m_caddieCard[5][13];
	unsigned long m_unknown;
	unsigned char m_playerIndex;
	std::list<unsigned long> m_activeCardList;
	FrSPCardBuffDlg* m_pCardBuffDlg;
	bool m_bSPCardAlarm;
};
