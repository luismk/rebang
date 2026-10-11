// Original: d:\build\custom\temp\code\client\projectg\tutorial.h
#ifndef PG_CLIENT_PROJECTG_TUTORIAL_H
#define PG_CLIENT_PROJECTG_TUTORIAL_H

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "TaskMain.h"
#include "RTTI.h"
#include "../client/Wangreal/include/wtypes.h"

#include <vector>
#include <deque>
#include <map>
#include <string>

class MsgObject;
class IObject;
class FrForm;
class FrButton;
class FrQuizDlg;
class WOverlay;
class WVector;

// A sprite-sheet animation drawn over the tutorial screen: m_overlay is cut
// in m_columns columns of m_dst.w x m_dst.h frames.
class AnimatedOverlay {
	LAYOUT_CHECK_FRIEND;
public:
	AnimatedOverlay(void);
	~AnimatedOverlay(void);

	bool IsActivate(void) { return m_active; } // tutorial.h:91
	void Activate(bool active, bool fade, bool loop);
	void Render(void);
	void Process(float dt);
	void SetSourceOverlay(const char *name, int columns, int rows, int mode, float w, float h);

protected:
	void GetNextOverlay(void);

public:
	WRect m_src;                       // 0x00, the second rectangle WOverlay::Render takes
	float m_frameTime;                 // 0x10, seconds per frame; 0 stops the animation (Process)
	WRect m_dst;                       // 0x14, x/y moved to the current frame by GetNextOverlay
	float m_elapsed;                   // 0x24, seconds into the frame or the fade, cleared by Activate
	int m_frame;                       // 0x28, cleared by Activate
	int m_frameCount;                  // 0x2C, columns * rows (SetSourceOverlay)
	bool m_visible;                    // 0x30, Render draws only when set
	bool m_active;                     // 0x31, IsActivate
	bool m_loop;                       // 0x32
	int m_fadeDir;                     // 0x34, +1 fading in, -1 fading out
	float m_alpha;                     // 0x38, 0..255
	WOverlay *m_overlay;               // 0x3C
	int m_columns;                     // 0x40
	std::vector<WOverlay *> m_overlays; // 0x44
};

CHECK_SIZE(AnimatedOverlay, 0x54);
BEGIN_LAYOUT_CHECK(AnimatedOverlay)
	CHECK_FIELD(AnimatedOverlay, m_src, 0x0);
	CHECK_FIELD(AnimatedOverlay, m_frameTime, 0x10);
	CHECK_FIELD(AnimatedOverlay, m_dst, 0x14);
	CHECK_FIELD(AnimatedOverlay, m_elapsed, 0x24);
	CHECK_FIELD(AnimatedOverlay, m_frame, 0x28);
	CHECK_FIELD(AnimatedOverlay, m_frameCount, 0x2C);
	CHECK_FIELD(AnimatedOverlay, m_visible, 0x30);
	CHECK_FIELD(AnimatedOverlay, m_active, 0x31);
	CHECK_FIELD(AnimatedOverlay, m_loop, 0x32);
	CHECK_FIELD(AnimatedOverlay, m_fadeDir, 0x34);
	CHECK_FIELD(AnimatedOverlay, m_alpha, 0x38);
	CHECK_FIELD(AnimatedOverlay, m_overlay, 0x3C);
	CHECK_FIELD(AnimatedOverlay, m_columns, 0x40);
	CHECK_FIELD(AnimatedOverlay, m_overlays, 0x44);
END_LAYOUT_CHECK

// The kind of a tutorial macro (the script commands ts_Parser::ConvertMacroType
// maps its keywords to). Enumerators not recovered yet but MACRO_WAIT.
enum eMacroType {
	MACRO_WAIT = 0,     // the macro's m_param is a delay in ms
	MACRO_SPECIAL = 0x1B, // taken by PopSpecial: m_param goes to CTutorial::m_specialWait
};

// One queued tutorial macro (0x28 bytes, PushMacro).
struct sMacro {
	eMacroType m_type; // 0x00
	char m_text[0x20]; // 0x04, the command's arguments, "" for none
	int m_param;       // 0x24
};
CHECK_SIZE(sMacro, 0x28);
CHECK_FIELD(sMacro, m_text, 0x04);
CHECK_FIELD(sMacro, m_param, 0x24);

// The tutorial game task. Its size (0xD68) is the `new` of
// CTutorialMakeInstance. Only the members the transcribed functions touch
// are named.
class CTutorial : public CTaskMain {
	friend IObject *CTutorialMakeInstance(void);
	LAYOUT_CHECK_FRIEND;
public:
	static const WRTTI m_RTTI;
	virtual ~CTutorial(void);
	virtual void OnInit(void);                                  // tutorial.cpp:457
	virtual void OnDisplay(void);                               // tutorial.cpp:683
	virtual const WRTTI *GetRTTI(void) const { return &m_RTTI; } // tutorial.h:169
	virtual void OnLoad(void);                                  // tutorial.cpp:491
	virtual void OnProcess(float dt);                           // tutorial.cpp:532
	virtual void OnDestroy(void);                               // tutorial.cpp:715
	virtual void HandleMsg(const MsgObject &msg);               // tutorial.cpp:737

protected:
	CTutorial(void);

	void BlockInput(void);                                      // tutorial.cpp:1033
	void FreeInput(void);                                       // tutorial.cpp:1050
	void MissionComplate(void);
	char *GetLine(char *src, char *dst);                        // tutorial.cpp:1065
	bool OnMissionFormResult(int result, FrForm *form);         // tutorial.cpp:1934
	void PriviewOverlay(void);                                  // tutorial.cpp:1564
	bool OnTipFormResult(int result, FrForm *form);             // tutorial.cpp:1574
	bool OnTipCaddieFormResult(int result, FrForm *form);       // tutorial.cpp:1839
	bool OnTipCaddieReopenQuizResult(int result, FrForm *form); // tutorial.cpp:1869
	bool OnTipCaddieNoneResult(int result, FrForm *form);       // tutorial.cpp:1877
	bool OnQuizFormResult(int result, FrForm *form);            // tutorial.cpp:2009
	void OpenMissionForm(void);
	void PrepareMission(void);
	void OpenMissionFailedForm(void);                           // tutorial.cpp:2038
	bool OnMissionFailedFormResult(int result, FrForm *form);
	bool CheckMissionComplete(void);                            // tutorial.cpp:996
	void ReviewTutorial(void);                                  // tutorial.cpp:1133
	void SkipMission(void);                                     // tutorial.cpp:1319
	void ChanceOverlay(void);                                   // tutorial.cpp:1529
	bool OnPracticeFormResult(int result, FrForm *form);        // tutorial.cpp:1751
	bool OnSkipMissionFormResult(int result, FrForm *form);
	bool OnReviewFormResult(int result, FrForm *form);
	bool OnCompleteFormResult(int result, FrForm *form);        // tutorial.cpp:1612
	void IsPractice(void);
	void OnImageLoad(void);
	void Again_Msg(bool retry);
	void OpenQuizForm(int quiz);
	void OpenTipCaddieForm(const char *text, FRESH_RESULT_CALLBACK callback);
	void PushMacroHitPointMission(void);

private:
	bool CheckCameraTarget(void);
	bool CheckClubTarget(void);
	void InitTutorialMode(int mode);
	WVector *GetPoint(const char *name);                        // tutorial.cpp:4285
	void PopMacro(void);
	void PopSpecial(void);
	void PushMacro(eMacroType type, const char *text, int param, int count); // tutorial.cpp:4294
	void ClearMacro(void);                                      // tutorial.cpp:4865

	std::deque<sMacro *> m_pendingMacros; // 0x1E0, destroyed with the task
	std::deque<sMacro> m_macros;       // 0x1F4, PushMacro / ClearMacro
	int m_specialWait;                 // 0x208, the m_param of the last MACRO_SPECIAL (PopSpecial)
	char m_fakeKey[0x10];              // 0x20C, the key OnProcess feeds to SetFakeInput ("" for none)
	bool m_fakeKeyDown;                // 0x21C, the key state SetFakeInput gets (cleared after one use)
	PADDING(0x21D, 0x220);
	int m_step;                        // 0x220, the tutorial's state
	int m_tutorialIndex;               // 0x224, filled by TutorialMain's 0x14F message (IsPractice)

	int m_failCount;                   // 0x228, ChanceOverlay shows 3 - m_failCount chances
	int m_targetHits;                  // 0x22C, CheckMissionComplete completes the mission at 4
	std::map<int, std::string> m_texts; // 0x230, destroyed with the task
	int m_tipState;                    // 0x23C, 6: the tip closes the mission (MissionComplate)
	int m_tipPos[2];                   // 0x240, copied from the 0x14A message (an x/y pair)
	FrForm *m_tipForm;                 // 0x248, cleared by OnTipFormResult
	FrForm *m_yesNoForm;               // 0x24C, the "notify_yesno" form of OpenMissionFailedForm / SkipMission
	FrForm *m_reviewForm;              // 0x250, ReviewTutorial's "notify_yesno" form
	FrForm *m_tipCaddieForm;           // 0x254
	FrButton *m_tipCaddieForm2;        // 0x258, the tip form's active button, cleared with m_tipCaddieForm
	int m_dialogType;                  // 0x25C, 1 while the mission/quiz form is up (PopMacro tests 9)
	int m_dialogState;                 // 0x260, 2 when OpenMissionForm / OpenQuizForm opened a form
	FrForm *m_missionForm;             // 0x264, closed by OnQuizFormResult
	WOverlay *m_missionOverlay;        // 0x268, released by OnMissionFormResult
	FrQuizDlg *m_quizForm;             // 0x26C
	int m_reserved270;                 // 0x270, cleared by the constructor only
	unsigned long m_missionItems[10];  // 0x274, item typeids the mission hands out (PrepareMission: three 0x18000004 for mode 5)
	bool m_singleMission;              // 0x29C, 1: skipping leaves the tutorial for the lobby; 0: MissionComplate records the mode
	PADDING(0x29D, 0x2A0);
	int m_reserved2A0;                 // 0x2A0, cleared by OnInit only
	bool m_practice;                   // 0x2A4, set when the practice form is answered with FrOK
	bool m_missionCleared[0xF];        // 0x2A5, indexed by the golf mode, set by MissionComplate
	bool m_macroHold;                  // 0x2B4, cleared by MissionComplate before PopMacro
	PADDING(0x2B5, 0x2B8);
	int m_macroTimer;                  // 0x2B8, ms, -1 at OnInit; OnProcess adds the frame time (reset to 24000)
	WRect m_tipRect;                   // 0x2BC, the tip window: x, y, w, h (the PushMacro* positions are relative to it)
	float m_pulseScale;                // 0x2CC, 1.5 at OnInit, animated by OnProcess
	int m_reserved2D0;                 // 0x2D0, cleared by OnInit only
	bool m_keepMouseBlocked;           // 0x2D4, OnTipFormResult blocks the left button instead of FreeInput
	bool m_reserved2D5;                // 0x2D5, cleared by OnInit only
	PADDING(0x2D6, 0x2D8);
	float m_reserved2D8;               // 0x2D8, set to the 0x155 message's value, cleared by OnInit
	int m_tutorialInfo;                // 0x2DC, the first int of data_9f63c0[m_tutorialIndex] (IsPractice)
	bool m_quizCorrect;                // 0x2E0, the quiz's answer
	PADDING(0x2E1, 0x2E4);
	AnimatedOverlay m_tutorialOverlays[0x1A]; // 0x2E4
	AnimatedOverlay m_chanceOverlays[3];      // 0xB6C, ChanceOverlay
	AnimatedOverlay m_singleOverlay;          // 0xC68
	AnimatedOverlay m_pairOverlays[2];        // 0xCBC
	bool m_inputBlocked;               // 0xD64, BlockInput / FreeInput
	PADDING(0xD65, 0xD68);
};

IObject *CTutorialMakeInstance(void);

CHECK_SIZE(CTutorial, 0xD68);
BEGIN_LAYOUT_CHECK(CTutorial)
	CHECK_FIELD(CTutorial, m_pendingMacros, 0x1E0);
	CHECK_FIELD(CTutorial, m_macros, 0x1F4);
	CHECK_FIELD(CTutorial, m_specialWait, 0x208);
	CHECK_FIELD(CTutorial, m_step, 0x220);
	CHECK_FIELD(CTutorial, m_tutorialIndex, 0x224);
	CHECK_FIELD(CTutorial, m_tutorialInfo, 0x2DC);
	CHECK_FIELD(CTutorial, m_dialogType, 0x25C);
	CHECK_FIELD(CTutorial, m_dialogState, 0x260);
	CHECK_FIELD(CTutorial, m_reserved2A0, 0x2A0);
	CHECK_FIELD(CTutorial, m_fakeKey, 0x20C);
	CHECK_FIELD(CTutorial, m_fakeKeyDown, 0x21C);
	CHECK_FIELD(CTutorial, m_reserved270, 0x270);
	CHECK_FIELD(CTutorial, m_missionItems, 0x274);
	CHECK_FIELD(CTutorial, m_macroTimer, 0x2B8);
	CHECK_FIELD(CTutorial, m_tipRect, 0x2BC);
	CHECK_FIELD(CTutorial, m_pulseScale, 0x2CC);
	CHECK_FIELD(CTutorial, m_reserved2D0, 0x2D0);
	CHECK_FIELD(CTutorial, m_reserved2D5, 0x2D5);
	CHECK_FIELD(CTutorial, m_tipPos, 0x240);
	CHECK_FIELD(CTutorial, m_reserved2D8, 0x2D8);
	CHECK_FIELD(CTutorial, m_singleMission, 0x29C);
	CHECK_FIELD(CTutorial, m_failCount, 0x228);
	CHECK_FIELD(CTutorial, m_targetHits, 0x22C);
	CHECK_FIELD(CTutorial, m_texts, 0x230);
	CHECK_FIELD(CTutorial, m_tipState, 0x23C);
	CHECK_FIELD(CTutorial, m_tipForm, 0x248);
	CHECK_FIELD(CTutorial, m_yesNoForm, 0x24C);
	CHECK_FIELD(CTutorial, m_reviewForm, 0x250);
	CHECK_FIELD(CTutorial, m_tipCaddieForm, 0x254);
	CHECK_FIELD(CTutorial, m_tipCaddieForm2, 0x258);
	CHECK_FIELD(CTutorial, m_missionForm, 0x264);
	CHECK_FIELD(CTutorial, m_missionOverlay, 0x268);
	CHECK_FIELD(CTutorial, m_quizForm, 0x26C);
	CHECK_FIELD(CTutorial, m_practice, 0x2A4);
	CHECK_FIELD(CTutorial, m_missionCleared, 0x2A5);
	CHECK_FIELD(CTutorial, m_macroHold, 0x2B4);
	CHECK_FIELD(CTutorial, m_keepMouseBlocked, 0x2D4);
	CHECK_FIELD(CTutorial, m_quizCorrect, 0x2E0);
	CHECK_FIELD(CTutorial, m_tutorialOverlays, 0x2E4);
	CHECK_FIELD(CTutorial, m_chanceOverlays, 0xB6C);
	CHECK_FIELD(CTutorial, m_singleOverlay, 0xC68);
	CHECK_FIELD(CTutorial, m_pairOverlays, 0xCBC);
	CHECK_FIELD(CTutorial, m_inputBlocked, 0xD64);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_TUTORIAL_H
