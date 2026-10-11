// Original: d:\build\custom\temp\code\client\projectg\trainingoptiondlg.h
#ifndef PG_CLIENT_PROJECTG_TRAININGOPTIONDLG_H
#define PG_CLIENT_PROJECTG_TRAININGOPTIONDLG_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "frform.h"

class FrButton;
class FrEdit;

// Training mode options (map, weather, wind); its size (0x150) is the `new`
// of FrTrainingOptionDlgMakeInstance. Only the members this unit defines so
// far are declared.
class FrTrainingOptionDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // trainingoptiondlg.h:8

	FrTrainingOptionDlg(void);   // trainingoptiondlg.cpp:48

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // trainingoptiondlg.cpp:21

	void OnOkBtnUp(void);                     // trainingoptiondlg.cpp:54
	void OnMapInit(intptr_t param);                // trainingoptiondlg.cpp:82
	void OnMapPrevBtnInit(intptr_t param);         // trainingoptiondlg.cpp:97
	void OnMapPrevBtnUp(void);                // trainingoptiondlg.cpp:107
	void OnMapNextBtnInit(intptr_t param);         // trainingoptiondlg.cpp:129
	void OnMapNextBtnUp(void);                // trainingoptiondlg.cpp:139
	void OnWeatherInit(intptr_t param);            // trainingoptiondlg.cpp:163
	void OnWeatherPrevBtnInit(intptr_t param);     // trainingoptiondlg.cpp:185
	void OnWeatherPrevBtnUp(void);            // trainingoptiondlg.cpp:205
	void OnWeatherNextBtnInit(intptr_t param);     // trainingoptiondlg.cpp:234
	void OnWeatherNextBtnUp(void);            // trainingoptiondlg.cpp:253
	void OnDirectionInit(intptr_t param);          // trainingoptiondlg.cpp:283
	void OnDirectionPrevBtnInit(intptr_t param);   // trainingoptiondlg.cpp:292
	void OnDirectionPrevBtnUp(void);          // trainingoptiondlg.cpp:311
	void OnDirectionNextBtnInit(intptr_t param);   // trainingoptiondlg.cpp:328
	void OnDirectionNextBtnUp(void);          // trainingoptiondlg.cpp:347
	void OnIntensityInit(intptr_t param);          // trainingoptiondlg.cpp:364
	void OnIntensityPrevBtnInit(intptr_t param);   // trainingoptiondlg.cpp:382
	void OnIntensityPrevBtnUp(void);          // trainingoptiondlg.cpp:401
	void OnIntensityNextBtnInit(intptr_t param);   // trainingoptiondlg.cpp:418
	void OnIntensityNextBtnUp(void);          // trainingoptiondlg.cpp:438

private:
	FrEdit* m_pMapEdit;                // 0x110, set by OnMapInit
	FrButton* m_pMapPrevBtn;           // 0x114
	FrButton* m_pMapNextBtn;           // 0x118
	FrEdit* m_pWeatherEdit;            // 0x11c, set by OnWeatherInit
	FrButton* m_pWeatherPrevBtn;       // 0x120
	FrButton* m_pWeatherNextBtn;       // 0x124
	FrEdit* m_pDirectionEdit;          // 0x128, set by OnDirectionInit
	FrButton* m_pDirectionPrevBtn;     // 0x12c
	FrButton* m_pDirectionNextBtn;     // 0x130
	FrEdit* m_pIntensityEdit;          // 0x134, set by OnIntensityInit
	FrButton* m_pIntensityPrevBtn;     // 0x138
	FrButton* m_pIntensityNextBtn;     // 0x13c
	unsigned char m_map;               // 0x140, course number (+0x80 for a special course)
	unsigned char m_weather;           // 0x141, from CSharedDoc; labelled 0 clear, 2 rain, 3 snow
	PADDING(0x142, 0x144);
	int m_direction;                   // 0x144, wind direction in degrees (0-360, step 10)
	int m_intensity;                   // 0x148, wind intensity (0-9 m)
	bool m_bFlag14c;                   // 0x14c, cleared by the constructor
	PADDING(0x14d, 0x150);
};

CHECK_SIZE(FrTrainingOptionDlg, 0x150);
BEGIN_LAYOUT_CHECK(FrTrainingOptionDlg)
	CHECK_FIELD(FrTrainingOptionDlg, m_pMapEdit, 0x110);
	CHECK_FIELD(FrTrainingOptionDlg, m_pMapPrevBtn, 0x114);
	CHECK_FIELD(FrTrainingOptionDlg, m_pWeatherEdit, 0x11c);
	CHECK_FIELD(FrTrainingOptionDlg, m_pWeatherNextBtn, 0x124);
	CHECK_FIELD(FrTrainingOptionDlg, m_pDirectionEdit, 0x128);
	CHECK_FIELD(FrTrainingOptionDlg, m_pIntensityNextBtn, 0x13c);
	CHECK_FIELD(FrTrainingOptionDlg, m_map, 0x140);
	CHECK_FIELD(FrTrainingOptionDlg, m_weather, 0x141);
	CHECK_FIELD(FrTrainingOptionDlg, m_direction, 0x144);
	CHECK_FIELD(FrTrainingOptionDlg, m_intensity, 0x148);
	CHECK_FIELD(FrTrainingOptionDlg, m_bFlag14c, 0x14c);
END_LAYOUT_CHECK

// The newer training options dialog; its size (0x158) is the `new` of
// FrNewTrainingOptionDlgMakeInstance.
class FrNewTrainingOptionDlg : public FrForm {
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }   // trainingoptiondlg.h:67

	FrNewTrainingOptionDlg(void);    // trainingoptiondlg.cpp:508
	virtual ~FrNewTrainingOptionDlg(void);   // trainingoptiondlg.cpp:516

private:
	static sFRESH_ENTRY _MsgEntries[];

protected:
	static sFRESH_MSGMAP _MsgMap;
	virtual const sFRESH_MSGMAP* GetMessageMap(void) const;   // trainingoptiondlg.cpp:462

	void ClearVariables(void);                // trainingoptiondlg.cpp:523
	void OnInitMapButton(intptr_t param);          // trainingoptiondlg.cpp:559
	void OnInitMapPrevButton(intptr_t param);      // trainingoptiondlg.cpp:573
	void OnInitMapNextButton(intptr_t param);      // trainingoptiondlg.cpp:587
	void OnInitFrontHoleButton(intptr_t param);    // trainingoptiondlg.cpp:601
	void OnInitBackHoleButton(intptr_t param);     // trainingoptiondlg.cpp:615
	void OnInitRandomHoleButton(intptr_t param);   // trainingoptiondlg.cpp:629
	void OnInitShuffleHoleButton(intptr_t param);  // trainingoptiondlg.cpp:643
	void OnInitHareButton(intptr_t param);         // trainingoptiondlg.cpp:656
	void OnInitKumoriButton(intptr_t param);       // trainingoptiondlg.cpp:671
	void OnInitAmeButton(intptr_t param);          // trainingoptiondlg.cpp:686
	void OnInitWindDirectionEdit(intptr_t param);  // trainingoptiondlg.cpp:701
	void OnInitPlayButton(intptr_t param);         // trainingoptiondlg.cpp:709
	void OnInitCancelButton(intptr_t param);       // trainingoptiondlg.cpp:724
	void OnLBDownMapButton(void);             // trainingoptiondlg.cpp:565
	void OnLBDownMapPrevButton(void);         // trainingoptiondlg.cpp:579
	void OnLBDownMapNextButton(void);         // trainingoptiondlg.cpp:593
	void OnLBDownFrontHoleButton(void);       // trainingoptiondlg.cpp:607
	void OnLBDownBackHoleButton(void);        // trainingoptiondlg.cpp:621
	void OnLBDownRandomHoleButton(void);      // trainingoptiondlg.cpp:635
	void OnLBDownShuffleHoleButton(void);     // trainingoptiondlg.cpp:649
	void OnLBDownHareButton(void);            // trainingoptiondlg.cpp:662
	void OnLBDownKumoriButton(void);          // trainingoptiondlg.cpp:677
	void OnLBDownAmeButton(void);             // trainingoptiondlg.cpp:692
	void OnLBDownPlayButton(void);            // trainingoptiondlg.cpp:715
	void OnLBDownCancelButton(void);          // trainingoptiondlg.cpp:730
	void ShowExpansionMenu(bool show);        // trainingoptiondlg.cpp:739
	void SetEnvironmentToGame(void);          // trainingoptiondlg.cpp:746
	void GetEnvironmentFromGame(void);        // trainingoptiondlg.cpp:753

private:
	FrButton* m_pMapButton;            // 0x110
	FrButton* m_pMapPrevButton;        // 0x114
	FrButton* m_pMapNextButton;        // 0x118
	int m_unknown11c;                  // 0x11c, cleared by ClearVariables
	FrButton* m_pFrontHoleButton;      // 0x120
	FrButton* m_pBackHoleButton;       // 0x124
	FrButton* m_pRandomHoleButton;     // 0x128
	FrButton* m_pShuffleHoleButton;    // 0x12c
	FrButton* m_pHareButton;           // 0x130, clear weather
	FrButton* m_pKumoriButton;         // 0x134, cloudy weather
	FrButton* m_pAmeButton;            // 0x138, rainy weather
	FrEdit* m_pWindDirectionEdit;      // 0x13c
	int m_unknown140;                  // 0x140, cleared by ClearVariables
	FrButton* m_pPlayButton;           // 0x144
	FrButton* m_pCancelButton;         // 0x148
	bool m_bExpansionMenu;             // 0x14c
	// Environment chosen in the dialog, 0xffff = unset; [2] is the weather
	// (0 clear, 1 cloudy, 2 rain).
	unsigned short m_environment[5];   // 0x14e
};

CHECK_SIZE(FrNewTrainingOptionDlg, 0x158);
BEGIN_LAYOUT_CHECK(FrNewTrainingOptionDlg)
	CHECK_FIELD(FrNewTrainingOptionDlg, m_pMapButton, 0x110);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_unknown11c, 0x11c);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_pFrontHoleButton, 0x120);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_pWindDirectionEdit, 0x13c);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_unknown140, 0x140);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_pCancelButton, 0x148);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_bExpansionMenu, 0x14c);
	CHECK_FIELD(FrNewTrainingOptionDlg, m_environment, 0x14e);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TRAININGOPTIONDLG_H
