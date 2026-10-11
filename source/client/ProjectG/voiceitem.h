// Original: d:\build\custom\temp\code\client\projectg\voiceitem.h
#ifndef PG_CLIENT_PROJECTG_VOICEITEM_H
#define PG_CLIENT_PROJECTG_VOICEITEM_H

#include <windows.h>
#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "singleton.h"

struct sGD_VOICEITEM;

// Functor of CVoiceItem::Initialize: registers one "VoiceClub" row.
struct sVOICECLUBINSERT {
	void operator()(const sGD_VOICEITEM& item) const;
};

// The voice-item sound table: voice files per item type id. Size 0x34.
class CVoiceItem : public WSingleton<CVoiceItem> {
	LAYOUT_CHECK_FRIEND;
	friend class CSoundItemManager;
public:
	// Bit flags of the wave a voice plays (tested with &).
	enum eWaveState {
	};

	// One voice entry. Size 0x24.
	class cVoice {
	public:
		unsigned long m_waveState;    // 0x0, eWaveState bits
		std::string m_fileName;       // 0x4
		unsigned char m_numOfSounds;  // 0x20
	};

	CVoiceItem(void);
	virtual ~CVoiceItem(void);

	void Initialize(void);
	void Destroy(void);
	// PlaySound is PlaySoundA through <windows.h>, as in the original.
	bool PlaySound(unsigned long dwTypeID, eWaveState eState, int nIndex);
	void SetSound(unsigned long dwTypeID, std::string strName, unsigned long dwState, unsigned char byNumOfSounds);

private:
	bool IsExistVoiceFile(char* szName);
	bool ClubPlaySound(unsigned long dwTypeID, eWaveState eState, int nIndex);
	bool AztecPlaySound(unsigned long dwTypeID, eWaveState eState);
	void SetSound(unsigned long dwTypeID, std::string strName, eWaveState eState, unsigned char byNumOfSounds);

	std::map<unsigned long, cVoice*> m_mapVoice;  // 0x28
};
CHECK_SIZE(CVoiceItem::cVoice, 0x24);
CHECK_FIELD(CVoiceItem::cVoice, m_fileName, 0x4);
CHECK_FIELD(CVoiceItem::cVoice, m_numOfSounds, 0x20);
CHECK_SIZE(CVoiceItem, 0x34);
BEGIN_LAYOUT_CHECK(CVoiceItem)
	CHECK_FIELD(CVoiceItem, m_mapVoice, 0x28);
END_LAYOUT_CHECK

// Plays the voice of the local player's equipped ball (and of a second
// equipped item) through CVoiceItem. ProjectG.cpp emits the constructor and
// destructor (voiceitem.h:62/63).
class CSoundItemManager : public WSingleton<CSoundItemManager> {
public:
	CSoundItemManager(void) {}          // voiceitem.h:62
	virtual ~CSoundItemManager(void) {} // voiceitem.h:63

	void PlayCurrentSoundItem(void);
};

// voiceitem.cpp:817
bool WearWig(unsigned char byIndex);

#endif
