// Original: d:\build\custom\temp\code\client\projectg\fxsequence.cpp
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fxsequence.h"
#include "FxBox.h"
#include "binstr.h"
#include "westpak.h"
#include "wresrcmng.h"

void __cdecl CloseCFile(cFile* file);
#include "fxspray.h"
#include "SoundManager.h"

extern WMatrix g_camera;

// fxsequence.cpp:20
CFxSequence::CFxSequence(CFxBox& box, bool bOriginal) : m_box(box) {
	m_bAlive = true;
	m_extWind.x = m_extWind.y = m_extWind.z = 0.0f;
	m_bActive = true;

	m_pos = m_startPos = WVector(0.0f, 0.0f, 0.0f);
	m_bOriginal = bOriginal;
	m_elapsed = 0.0f;
	m_delay = 0.0f;
	m_fDistLimitA = 3.402823466e+38F;   // FLT_MAX
	m_fDistLimitB = 3.402823466e+38F;   // FLT_MAX
	m_matrix.xx = m_matrix.yy = m_matrix.zz = 1.0f; m_matrix.yx = m_matrix.zx = m_matrix.xy = m_matrix.zy = m_matrix.xz = m_matrix.yz = m_matrix.xm = m_matrix.ym = m_matrix.zm = 0.0f;

	m_pMatrix[0] = m_pMatrix[1] = m_pMatrix[2] = m_pMatrix[3] = 0;
	m_pOffset[0] = m_pOffset[1] = m_pOffset[2] = m_pOffset[3] = 0;

	Clear();
}

// fxsequence.cpp:40
CFxSequence::~CFxSequence(void) {
}

// fxsequence.cpp:45
void CFxSequence::Clear(void) {
	m_pAttacher = 0;

	m_lines.clear();
	m_curLine = m_lines.end();

	m_lastError.m_text[0] = '\0';
	m_pCurrentSpray = NULL;

	memset(&m_defines, 0, sizeof(m_defines));
}

// fxsequence.cpp:59
bool CFxSequence::LoadFile(const char* name) {
	Clear();

	std::map<std::string, CFxSequence*>::iterator it = m_box.m_originalSequences.find(name);
	if (it == m_box.m_originalSequences.end()) {
		cFile* pFile = m_box.m_pResourceManager->GetCFile(name, 0x2000);

		if (!pFile) {
			return false;
		}

		char line[0x400];
		int lineNo = 0;
		while (pFile->Scan("%n", line)) {
			AddLine(line, ++lineNo);
		}

		CloseCFile(pFile);

		CFxSequence* pOriginal = new CFxSequence(m_box, true);
		pOriginal->m_lines = m_lines;
		m_box.m_originalSequences[name] = pOriginal;
	} else {
		m_lines = it->second->m_lines;
		m_curLine = m_lines.begin();
	}

	SetLineTime();
	return true;
}

// fxsequence.cpp:100
void CFxSequence::AddLine(const char* line, int lineNo) {
	if (line[0] == '\0' || line[0] == ';') {
		return;
	}

	if (m_box.PreprocessDefines(m_defines, line)) {
		return;
	}

	char timeText[0x104];
	char text[0x104];
	char maxTimeText[0x104];

	cTokenV lineToken;
	lineToken.Init(line, strlen(line));
	lineToken.GetToken(timeText, ":", 1);
	lineToken.GetToken(text, "\r\n", 2);

	cTokenS timeToken;
	timeToken.Init(timeText, strlen(timeText));
	timeToken.GetToken(timeText, "-", 1);
	timeToken.GetToken(maxTimeText, "-", 1);

	TrimA(timeText, " \t\n\r\0");
	TrimA(maxTimeText, " \t\n\r\0");
	TrimA(text, " \t\n\r\0");

	sLine newLine;
	newLine.m_lineNo = lineNo;
	if (isalpha(timeText[0])) {
		newLine.m_time = -1;
		strcpy(newLine.m_text, timeText);
	} else if (timeText[0] == '\0') {
		newLine.m_time = -2;
		strcpy(newLine.m_text, text);
	} else {
		newLine.m_minTime = atoi(timeText);
		newLine.m_maxTime = maxTimeText[0] ? atoi(maxTimeText) : newLine.m_minTime;
		newLine.m_time = newLine.m_minTime;
		strcpy(newLine.m_text, text);
	}

	m_lines.push_back(newLine);
	if (m_curLine == m_lines.end()) {
		m_curLine = m_lines.begin();
	}
}

// fxsequence.cpp:160
const CFxSequence::sLine& CFxSequence::GetLastError(void) {
	return m_lastError;
}

// fxsequence.cpp:167
bool CFxSequence::ParseLine(const char* line, bool& bDone) {
	bDone = false;

	if (line[0] == '\0' || line[0] == ';') {
		return true;
	}

	char args[16][0x104];
	char cmd[0x104];
	cTokenV token;

	token.Init(line, strlen(line));

	token.GetToken(cmd, "=", 1);
	TrimA(cmd, " \t\n\r\0");

	for (int i = 0; i < 16; i++) {
		token.GetToken(args[i], ",", 1);
		TrimA(args[i], " \t()%");

		m_box.ProcessDefines(m_defines, args[i]);
	}

	if (cmd[0] == '\0') {
		return true;
	}

	if (strcmpi(cmd, "spray") == 0) {
		if (m_bActive) {
			CFxSpray* pSpray = m_box.LoadSpray(args[0], NULL);
			if (pSpray == NULL) {
				bDone = true;
				return false;
			}

			pSpray->m_param.m_bSurvivesSequence = strcmpi(args[7], "strongbond") == 0;
			pSpray->m_pParent = this;

			pSpray->m_fAttachLimitA = m_fDistLimitA;
			pSpray->m_fAttachLimitB = m_fDistLimitB;
			pSpray->m_bAlive = m_bAlive;
			pSpray->SetWind(m_extWind);

			pSpray->m_pos = WVector((float)atof(args[1]), (float)atof(args[2]), (float)atof(args[3]));
			pSpray->m_spawnDir = RotVec(WVector((float)atof(args[4]), (float)atof(args[5]), (float)atof(args[6])), m_matrix);

			pSpray->m_bActive = m_bActive;
			m_sprays[pSpray] = pSpray;
			m_box.AddSpray(pSpray);

			m_pCurrentSpray = pSpray;
		}
	} else if (strcmpi(cmd, "add_spawn") == 0) {
		if (m_pCurrentSpray != NULL) {
			CFxSpray::sSpawn spawn;

			strcpy(spawn.m_name, args[0]);
			spawn.m_bFollow = strcmpi(args[1], "attached") == 0;

			if (strcmpi(args[2], "interval") == 0) {
				spawn.m_type = 0;
				spawn.m_fParam = m_box.ParseNumber(args[3]);
				spawn.m_intervalCount = (int)m_box.ParseNumber(args[4]);
			} else if (strcmpi(args[2], "age") == 0) {
				spawn.m_type = 1;
				spawn.m_fParam = m_box.ParseNumber(args[3]);
			} else if (strcmpi(args[2], "ondeath") == 0) {
				spawn.m_type = 2;
			} else if (strcmpi(args[2], "onground") == 0) {
				spawn.m_type = 3;
			}

			m_pCurrentSpray->m_param.m_spawns.push_back(spawn);
		}
	} else if (strcmpi(cmd, "add_revolution") == 0) {
		if (m_pCurrentSpray != NULL) {
			CFxSpray::sRevol revol;

			sscanf(args[0], "%f", &revol.center.x);
			sscanf(args[1], "%f", &revol.center.y);
			sscanf(args[2], "%f", &revol.center.z);
			sscanf(args[3], "%f", &revol.axis.x);
			sscanf(args[4], "%f", &revol.axis.y);
			sscanf(args[5], "%f", &revol.axis.z);

			revol.t0 = (int)m_box.ParseNumber(args[6]);
			revol.speed0 = m_box.ParseNumber(args[7]);
			revol.t1 = (int)m_box.ParseNumber(args[8]);
			revol.speed1 = m_box.ParseNumber(args[9]);

			m_pCurrentSpray->m_param.m_revols.push_back(revol);
		}
	} else if (strcmpi(cmd, "add_rotation") == 0) {
		if (m_pCurrentSpray != NULL) {
			CFxSpray::sNozzleRot rot;

			sscanf(args[0], "%f", &rot.axis.x);
			sscanf(args[1], "%f", &rot.axis.y);
			sscanf(args[2], "%f", &rot.axis.z);
			rot.t0 = (int)m_box.ParseNumber(args[3]);
			rot.a0 = m_box.ParseNumber(args[4]);
			rot.t1 = (int)m_box.ParseNumber(args[5]);
			rot.a1 = m_box.ParseNumber(args[6]);

			m_pCurrentSpray->m_param.m_axisRots.push_back(rot);
		}
	} else if (strcmpi(cmd, "add_nrotation") == 0) {
		if (m_pCurrentSpray != NULL) {
			CFxSpray::sNozzleRot rot;

			sscanf(args[0], "%f", &rot.axis.x);
			sscanf(args[1], "%f", &rot.axis.y);
			sscanf(args[2], "%f", &rot.axis.z);
			rot.t0 = (int)m_box.ParseNumber(args[3]);
			rot.a0 = m_box.ParseNumber(args[4]);
			rot.t1 = (int)m_box.ParseNumber(args[5]);
			rot.a1 = m_box.ParseNumber(args[6]);

			m_pCurrentSpray->m_param.m_nozzleRots.push_back(rot);
		}
	} else if (strcmpi(cmd, "add_prevolution") == 0) {
		if (m_pCurrentSpray != NULL) {
			CFxSpray::sRevol revol;

			sscanf(args[0], "%f", &revol.center.x);
			sscanf(args[1], "%f", &revol.center.y);
			sscanf(args[2], "%f", &revol.center.z);
			sscanf(args[3], "%f", &revol.axis.x);
			sscanf(args[4], "%f", &revol.axis.y);
			sscanf(args[5], "%f", &revol.axis.z);

			revol.t0 = (int)m_box.ParseNumber(args[6]);
			revol.speed0 = m_box.ParseNumber(args[7]);
			revol.t1 = (int)m_box.ParseNumber(args[8]);
			revol.speed1 = m_box.ParseNumber(args[9]);

			m_pCurrentSpray->m_param.m_grandRevols.push_back(revol);
		}
	} else if (stricmp(cmd, "AttachedToParent") == 0) {
		if (m_pCurrentSpray != NULL) {
			m_pCurrentSpray->m_param.m_bTransformPos = false;

			if (stricmp(args[0], "on") == 0) {
				m_pCurrentSpray->m_param.m_bTransformPos = true;
			}

			int x = 0, y = 0, z = 0;
			sscanf(args[1], "%d", &x);
			sscanf(args[2], "%d", &y);
			sscanf(args[3], "%d", &z);

			m_pCurrentSpray->m_param.m_flags = 0;

			if (x) m_pCurrentSpray->m_param.m_flags |= 1;
			if (y) m_pCurrentSpray->m_param.m_flags |= 2;
			if (z) m_pCurrentSpray->m_param.m_flags |= 4;
		}
	} else if (strcmpi(cmd, "sound") == 0) {
		float range = -1.0f;
		char name[0x104];
		strcpy(name, args[0]);

		char* ext = strstr(name, ".wav");
		if (ext) {
			*ext = '\0';
		}

		sscanf(args[1], "%f", &range);

		if (range < 0.0f) {
			g_audio->PlaySfx(name, NULL, 0, NULL, NULL, 0.5f, 200.0f);
		} else {
			WVector pos = m_pos - g_camera.pivot;
			g_audio->PlaySfx(name, &pos, 5, NULL, NULL, 0.0f, range);
		}
	} else if (strcmpi(cmd, "goto") == 0) {
		for (std::list<sLine>::iterator it = m_lines.begin(); it != m_lines.end(); ++it) {
			if (strcmpi(it->m_text, args[0]) == 0) {
				SetLineTime();

				m_curLine = ++it;
				float time = (float)m_curLine->m_minTime;

				float diff = m_elapsed - time;
				if (diff < m_box.m_frameTime * 2.0f && diff > m_box.m_frameTime * -2.0f) {
					return false;
				}
				m_elapsed = time;
				break;
			}
		}
	} else if (strcmpi(cmd, "stop") == 0) {
		m_curLine = m_lines.end();
	} else {
		bDone = true;
		return false;
	}

	return true;
}

// fxsequence.cpp:438
void CFxSequence::SetLineTime(void) {
	for (std::list<sLine>::iterator it = m_lines.begin(); it != m_lines.end(); ++it) {
		if (it->m_time == -1 || it->m_time == -2) {
			continue;
		}

		if (it->m_maxTime != it->m_minTime) {
			it->m_time = rand() % (it->m_maxTime - it->m_minTime) + it->m_minTime;
		}

		std::list<sLine>::iterator pos = it;
		while (pos != m_lines.begin()) {
			std::list<sLine>::iterator prev = pos;
			--prev;
			if (prev->m_time == -1 || prev->m_time == -2) {
				break;
			}
			--pos;
		}

		for (; pos != it; ++pos) {
			if (pos->m_time > it->m_time) {
				m_lines.insert(pos, *it);
				it = m_lines.erase(it);
				--it;
				break;
			}
		}
	}
}

// fxsequence.cpp:483
bool CFxSequence::Process(float fDelta, bool& bDone) {
	bDone = false;

	if (!m_bActive) {
		if (m_delay > 0.0f) {
			m_delay -= fDelta;

			if (m_delay < 0.0f) {
				m_bActive = true;
			} else {
				return true;
			}
		} else {
			return true;
		}
	}

	while (true) {
		if (m_curLine == m_lines.end()) {
			m_elapsed += fDelta;
			return false;
		}

		sLine& line = *m_curLine;

		if (line.m_time == -1) {
			++m_curLine;
		} else if (line.m_time != -2 && line.m_time > (unsigned int)(int)(m_elapsed + 0.5f)) {
			break;
		} else {
			++m_curLine;

			if (line.m_text[0] != '\0') {
				if (!ParseLine(line.m_text, bDone)) {
					--m_curLine;
					break;
				}

				if (bDone) {
					m_lastError = line;
					break;
				}
			}
		}
	}

	m_elapsed += fDelta;
	return true;
}

// fxsequence.cpp:552
void CFxSequence::DeleteChildSpray(CFxSpray* pSpray) {
	std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.find(pSpray);
	if (it != m_sprays.end()) {
		if (it->second == m_pCurrentSpray) {
			m_pCurrentSpray = NULL;
		}

		m_sprays.erase(it);
	}
}

// fxsequence.cpp:565
void CFxSequence::SetLight(const LightSet& light) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		it->second->SetLight(light);
	}
}

// fxsequence.cpp:572
void CFxSequence::SetWind(const WVector& wind) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		it->second->SetWind(wind);
	}
}

// fxsequence.cpp:579
void CFxSequence::SetDistLimit(float limitA, float limitB) {
	for (std::map<CFxSpray*, CFxSpray*>::iterator it = m_sprays.begin(); it != m_sprays.end(); ++it) {
		it->second->SetDistLimit(limitA, limitB);
	}
}
