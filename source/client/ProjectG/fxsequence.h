// Original: d:\build\custom\temp\code\client\projectg\fxsequence.h
#ifndef PG_CLIENT_PROJECTG_FXSEQUENCE_H
#define PG_CLIENT_PROJECTG_FXSEQUENCE_H

#include <list>
#include <map>
#include <string>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wmath.h"

class CFxBox;
class CFxSpray;
struct Attacher;
struct LightSet;

// Named numbers of an effect script (ProcessDefines replaces a token naming
// one by its value). Layout from CFxBox::ProcessDefines (fxbox.cpp:622).
struct defines_t {
	char m_names[16][0x104];                   // 0x0
	float m_values[16];                        // 0x1040
	int m_count;                               // 0x1080
};
CHECK_SIZE(defines_t, 0x1084);
CHECK_FIELD(defines_t, m_values, 0x1040);
CHECK_FIELD(defines_t, m_count, 0x1080);

// A running effect sequence: the timed script lines of a .fx file, run by
// Process, and the sprays they opened. Layout from the constructor
// (fxsequence.cpp:20); members whose use is not decompiled yet are named
// after what the constructor and Clear do to them.
class CFxSequence {
	LAYOUT_CHECK_FRIEND;
	friend class CFxBox;
	friend class CPvsFxBox;
	friend class CFx;
	friend class CSky;
	friend class CFxSpray;
	friend class WPolySoup;
	friend class CRealMyRoom_Avatar;
	friend class CExhibition;
	friend class CWind;
public:
	// One script line: when it runs (a random time between m_minTime and
	// m_maxTime, or -1/-2 for untimed lines) and its text.
	struct sLine {
		int m_lineNo;                   // 0x0, line number in the .fx file
		unsigned int m_minTime;         // 0x4
		unsigned int m_maxTime;         // 0x8
		unsigned int m_time;            // 0xC, -1 and -2 mark untimed lines
		char m_text[0x104];             // 0x10
	};

	CFxSequence(CFxBox& box, bool bOriginal);
	virtual ~CFxSequence(void);

	void Clear(void);
	void AddLine(const char* line, int lineNo);
	bool LoadFile(const char* name);
	const sLine& GetLastError(void);
	void SetLight(const LightSet& light);
	void SetWind(const WVector& wind);
	void SetDistLimit(float limitA, float limitB);
	void GetBoneMatrix(WMatrix* pMatrix);
	void SetActive(bool bActive, bool bChildren);   // fxsequence.inl:7
	WVector& Pos(void);                             // fxsequence.inl:26
	std::map<CFxSpray*, CFxSpray*>& GetSprayList(void);   // fxsequence.inl:40
	void SetDelay(float delay);                     // fxsequence.inl:63

protected:
	void SetLineTime(void);
	bool ParseLine(const char* line, bool& bDone);
	bool Process(float fDelta, bool& bDone);

private:
	void DeleteChildSpray(CFxSpray* pSpray);

protected:
	bool m_bAlive;                  // 0x4, true from the constructor
	WVector m_extWind;              // 0x8, CFx::UpdateExtWind; zeroed by the constructor
	const Attacher* m_pAttacher;    // 0x14, where CFx::Attach attached it; zeroed by Clear
	float m_fDistLimitA;            // 0x18, FLT_MAX by default
	float m_fDistLimitB;            // 0x1C, FLT_MAX by default
	std::map<CFxSpray*, CFxSpray*> m_sprays;   // 0x20, GetSprayList
	const WMatrix* m_pMatrix[4];    // 0x2C, attachment matrices, zeroed by the constructor
	const WVector* m_pOffset[4];    // 0x3C, attachment offsets, zeroed by the constructor
	defines_t m_defines;            // 0x4C, cleared by Clear
	WMatrix m_matrix;               // 0x10D0, identity by default
	CFxBox& m_box;                  // 0x1100
	bool m_bActive;                 // 0x1104
	WVector m_pos;                  // 0x1108, Pos
	WVector m_startPos;             // 0x1114
	bool m_bOriginal;               // 0x1120
	float m_elapsed;                // 0x1124, zeroed by the constructor
	float m_delay;                  // 0x1128, seconds before the sequence starts
	sLine m_lastError;              // 0x112C, GetLastError
	CFxSpray* m_pCurrentSpray;      // 0x1240, reset when that spray is deleted
	std::list<sLine> m_lines;       // 0x1244
	std::list<sLine>::iterator m_curLine;   // 0x1250
	std::string m_name;             // 0x1254
};
CHECK_SIZE(CFxSequence::sLine, 0x114);
CHECK_SIZE(CFxSequence, 0x1270);

BEGIN_LAYOUT_CHECK(CFxSequence)
	CHECK_FIELD(CFxSequence, m_bAlive, 0x4);
	CHECK_FIELD(CFxSequence, m_extWind, 0x8);
	CHECK_FIELD(CFxSequence, m_pAttacher, 0x14);
	CHECK_FIELD(CFxSequence, m_fDistLimitA, 0x18);
	CHECK_FIELD(CFxSequence, m_fDistLimitB, 0x1C);
	CHECK_FIELD(CFxSequence, m_sprays, 0x20);
	CHECK_FIELD(CFxSequence, m_pMatrix, 0x2C);
	CHECK_FIELD(CFxSequence, m_pOffset, 0x3C);
	CHECK_FIELD(CFxSequence, m_defines, 0x4C);
	CHECK_FIELD(CFxSequence, m_matrix, 0x10D0);
	CHECK_FIELD(CFxSequence, m_bActive, 0x1104);
	CHECK_FIELD(CFxSequence, m_pos, 0x1108);
	CHECK_FIELD(CFxSequence, m_startPos, 0x1114);
	CHECK_FIELD(CFxSequence, m_bOriginal, 0x1120);
	CHECK_FIELD(CFxSequence, m_elapsed, 0x1124);
	CHECK_FIELD(CFxSequence, m_delay, 0x1128);
	CHECK_FIELD(CFxSequence, m_lastError, 0x112C);
	CHECK_FIELD(CFxSequence, m_pCurrentSpray, 0x1240);
	CHECK_FIELD(CFxSequence, m_lines, 0x1244);
	CHECK_FIELD(CFxSequence, m_curLine, 0x1250);
	CHECK_FIELD(CFxSequence, m_name, 0x1254);
END_LAYOUT_CHECK

#include "fxsequence.inl"

#endif // PG_CLIENT_PROJECTG_FXSEQUENCE_H
