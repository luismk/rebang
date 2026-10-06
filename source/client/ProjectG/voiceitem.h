#pragma once

#include <string>
#include <map>

struct sGD_VOICEITEM;

struct sVOICECLUBINSERT
{
	void operator()(const sGD_VOICEITEM& item) const;
};

class CVoiceItem : public WSingleton<CVoiceItem>
{
public:
	enum eWaveState
	{
		WAVESTATE_02 = 0x02,
		WAVESTATE_08 = 0x08,
		WAVESTATE_10 = 0x10,
		WAVESTATE_20 = 0x20,
		WAVESTATE_80 = 0x80,
		WAVESTATE_100 = 0x100,
	};

	class cVoice
	{
	public:
		unsigned long state;
		std::string name;
		unsigned char numOfSounds;
	};

	CVoiceItem();
	virtual ~CVoiceItem();

	void Initialize();
	void Destroy();
	void SetSound(unsigned long typeID, std::string name, unsigned long state,
		unsigned char numOfSounds);
	bool PlaySoundA(unsigned long typeID, eWaveState state, int index);

private:
	void SetSound(unsigned long typeID, std::string name, eWaveState state,
		unsigned char numOfSounds);
	bool ClubPlaySound(unsigned long typeID, eWaveState state, int index);
	bool AztecPlaySound(unsigned long typeID, eWaveState state);
	bool IsExistVoiceFile(char* filename);

	std::multimap<unsigned long, cVoice*> m_voiceMap;
};

class CSoundItemManager : public WSingleton<CSoundItemManager>
{
public:
	CSoundItemManager() { }
	virtual ~CSoundItemManager() { }

	void PlayCurrentSoundItem();
};
