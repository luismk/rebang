// Original: d:\build\custom\temp\code\client\projectg\console.cpp
#include "console.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ChatMsg.h"
#include "ClientSetting.h"
#include "InputManager.h"
#include "GolfDoc.h"
#include "Packet.h"
#include "SceneManager.h"
#include "SharedDoc.h"
#include "commonutil.h"
#include "quadtree.h"
#include "TaskManager.h"
#include "tweaker.h"
#include "wfont.h"
#include "winput.h"
#include "wmath.h"
#include "woverlay.h"
#include "wtypes.h"
#include "wview.h"
#include "../shared/luasystem/LuaScriptManager.h"

// The console commands (0x59 of them): the name, the identifier
// BrowseCommand returns and the usage BadArgument prints.
extern struct {
	const char *name;                   // 0x0
	int id;                             // 0x4
	const char *usage;                  // 0x8
} command_table[];

// The caustic animation of the lakes (pool.cpp; publics ?causticSpeed@@3MA, ?causticAlpha@@3HA).
extern float causticSpeed;
extern int causticAlpha;

// The IME input device (public ?g_ime@@3PAVWInputDev@@A).
extern WInputDev *g_ime;

// The last IME composition InputProcess appended (no symbol).
static int s_lastComposition;

// The tweaker "tw using" selected, the target of the following commands.
static tweaker *s_pTweaker;

// console.cpp:333
int _POSTMSG(const void *pSender, const char *pTarget, int msg, int param1, int param2, int param3, unsigned long param4) {
	return CTaskManager::Instance()->PostMsg(NULL, pTarget, msg, param1, param2, param3, param4);
}

// console.cpp:343
CConsole::CConsole(void)
	: m_active(false), m_keyStates(NULL), m_history(32, 0), m_lines(32, 0), m_repeatDelay(5.0f) {
}

// console.cpp:347
CConsole::~CConsole(void) {
	ShutDown();
}

// console.cpp:352
void CConsole::Init(void) {
	m_bsStack[0] = 0;
	m_input[0] = 0;

	m_keyStates = new ConsoleKeyState[0x59];
	for (unsigned int i = 0; i < 0x59; i++) {
		m_keyStates[i].m_pressed = 0;
	}

	m_alpha = 0.0f;

	m_inputLine[0] = 0;
	m_inputLocked = false;
	m_lineOffset = 0;
	m_state = 1;

	m_pFont = CChatMsg::Instance()->GetMaskedFont();
}

// console.cpp:391
void CConsole::ShutDown(void) {
	if (m_keyStates) {
		delete[] m_keyStates;
		m_keyStates = NULL;
	}

	for (char *line = m_history.Start(); line != NULL; line = m_history.Next()) if (line) delete[] line; m_history.Reset();

	for (char *line = m_lines.Start(); line != NULL; line = m_lines.Next()) if (line) delete[] line; m_lines.Reset();
}

// console.cpp:400
void CConsole::Process(float elapsed) {
	// The original's body (lines 400-450) compiled to nothing: only the
	// `ret 4` remains.
}

// console.cpp:453
void CConsole::InputProcess(float elapsed) {
	int key = 0;
	char text[4];
	char result[0x80];

	text[0] = 0;
	int ch = g_ime->GetState(3, 0);

	if (ch) {
		if (ch == 9) {
			key = ch;
		} else if (ch == 8 || ch == 13 || ch == 6 || ch == 7 || ch == 2 || ch == 3 || ch == 4 || ch == 5) {
			key = ch;
			m_input[0] = 0;
		} else if (ch & 0x80) {
			text[0] = (char)ch;
			text[1] = (char)(ch >> 8);
			text[2] = 0;

			PopFromBsStack();
			PopFromBsStack();
			m_input[0] = 0;
		} else if (ch >= 0x20 && ch <= 0x7A) {
			text[0] = (char)ch;
			text[1] = 0;
			if (ch == 0x20) {
				m_bsStack[0] = 0;
			} else {
				PopFromBsStack();
			}
			m_input[0] = 0;
		}

		if (text[0]) {
			if (m_inputLocked) {
				m_inputLine[strlen(m_inputLine) - 2] = 0;
				strcat(m_inputLine, text);
			} else {
				strcat(m_inputLine, text);
			}
			m_inputLine[0x7F] = 0;
		}

		m_inputLocked = false;
	}

	int composition = g_ime->GetState(3, -2);
	if (composition) {
		if (composition != s_lastComposition) {
			s_lastComposition = composition;
			text[0] = (char)composition;
			text[1] = (char)(composition >> 8);
			text[2] = 0;

			if (m_inputLocked) {
				m_inputLine[strlen(m_inputLine) - 2] = 0;
				strcat(m_inputLine, text);
			} else {
				strcat(m_inputLine, text);
			}
			m_inputLine[0x7F] = 0;
			m_inputLocked = true;
		}
	} else if (m_inputLocked) {
		m_inputLine[strlen(m_inputLine) - 2] = 0;
		m_inputLocked = false;
	}

	switch (key) {
	case 9: {
		char *word = m_inputLine;
		if (!m_input[0]) {
			strcpy(m_input + 1, m_inputLine);
			m_input[0] = 1;
			char *space = strstr(m_inputLine, " ");
			while (space) {
				word = space + 1;
				strcpy(m_input + 1, word);
				space = strstr(word, " ");
			}

			for (unsigned int i = 0; i < 0x59; i++) {
				m_keyStates[i].m_pressed = 0;
			}
		}

		if (word == m_inputLine) {
			if (BrowseCommand(m_input + 1, result) > -1) {
				strcpy(word, result);
			}
		} else {
			char *end = strchr(word, 0);
			if (isalnum(end[-1])) {
				while ((*end = PopFromBsStack()) != 0) {
					end++;
				}
			} else {
				char c;
				while ((c = PopFromBsStack()) != 0) {
					if (isalnum(c)) {
						*end = c;
						while ((*++end = PopFromBsStack()) != 0) {
						}
						break;
					}
				}
			}
			m_input[0] = 0;
		}
		break;
	}
	case 13:
		m_bsStack[0] = 0;
		if (strlen(m_inputLine)) {
			char *line = new char[strlen(m_inputLine) + 1];
			strcpy(line, m_inputLine);
			m_history += line;
			AddText(MakeStr(">%s", line));

			if (!TweakerCommandProc(m_inputLine)) {
				Command(strlwr(m_inputLine));
			}

			m_inputLine[0] = 0;
			int offset = m_lines.GetCount() - 19;
			m_lineOffset = offset > 0 ? offset : 0;
			m_historyPos = NULL;
			m_historyEdge = 1;
		}
		break;
	case 6:
		if (m_historyEdge >= 0 && m_history.GetCount() > 0) {
			m_historyEdge = 0;
			if (m_historyPos) {
				char *entry = m_history.Prev(m_historyPos);
				if (m_historyPos) {
					strcpy(m_inputLine, entry);
				} else {
					m_historyEdge = -1;
					m_history.Start(m_historyPos);
				}
			} else {
				char *entry = m_history.End(m_historyPos);
				strcpy(m_inputLine, entry);
			}
		}
		break;
	case 7:
		if (m_historyEdge <= 0 && m_history.GetCount() > 0) {
			m_historyEdge = 0;
			if (m_historyPos) {
				char *entry = m_history.Next(m_historyPos);
				if (m_historyPos) {
					strcpy(m_inputLine, entry);
				} else {
					m_historyEdge = 1;
					m_inputLine[0] = 0;
				}
			}
		}
		break;
	case 8: {
		int len = strlen(m_inputLine);
		if (len > 0 && !m_inputLocked) {
			if (len > 1 && m_inputLine[len - 1] < 0) {
				PushIntoBsStack(m_inputLine[len - 1]);
				PushIntoBsStack(m_inputLine[len - 2]);
				m_inputLine[len - 2] = 0;
			} else {
				if (m_inputLine[len - 1] == ' ') {
					m_bsStack[0] = 0;
				} else {
					PushIntoBsStack(m_inputLine[len - 1]);
				}
				m_inputLine[len - 1] = 0;
			}
		}
		break;
	}
	case 2:
		if (m_lines.GetCount() > 19) {
			int offset = m_lineOffset - 1;
			m_lineOffset = offset > 0 ? offset : 0;
		}
		break;
	case 3:
		if (m_lines.GetCount() > 19) {
			int offset = m_lineOffset + 1;
			int last = m_lines.GetCount() - 19;
			m_lineOffset = offset > last ? last : offset;
		}
		break;
	}
}

// console.cpp:743
void CConsole::Display(void) {
	if (WAbsBySign(m_alpha) < WMATH_EPSILON) {
		return;
	}

	WOverlay::DrawBox(g_view, WRect(0, 0, g_view->GetWidth(), 302), 0, (int)(m_alpha * 128.0f) << 24, 0.001f);

	int count = m_lines.GetCount();
	int empty = (19 - count > 0) ? 19 - count : 0;

	float y = empty * 15.0f + 5.0f;
	int alpha = (int)(m_alpha * 255.0f);
	int i = 0;
	for (char *line = m_lines.Start(); line != NULL; line = m_lines.Next(), i++) {
		if (i == m_lineOffset + 19) {
			break;
		}
		if (i >= m_lineOffset) {
			m_pFont->Print(g_view, 6.0f, y + 1.0f, line, 0, alpha << 24, NULL);
			m_pFont->Print(g_view, 5.0f, y, line, 0, (alpha << 24) | 0xFFFFFF, NULL);
			y += 15.0f;
		}
	}

	float width = m_pFont->Print(g_view, 5.0f, y, MakeStr(">%s", m_inputLine), 0, (alpha << 24) | 0xFFFFFF, NULL);
	m_pFont->Flush(g_view);

	WOverlay::DrawBox(g_view, WRect(width + 5.0f, y + 10.0f, 7, 2), 0, (alpha << 24) | 0xFFFFFF, 0.001f);
	WOverlay::DrawBox(g_view, WRect(g_view->GetWidth() - 12.0f, 5, 10, 292), 0, (alpha << 24) | 0x404040, 0.001f);

	count = m_lines.GetCount();
	if (count > 0 && count > 19) {
		WOverlay::DrawBox(g_view, WRect(g_view->GetWidth() - 10.0f, m_lineOffset * 292.0f / count + 5.0f, 6, 292.0f * 19 / count), 0, (alpha << 24) | 0x707070, 0.001f);
	}
}

// console.cpp:790
bool CConsole::PushIntoBsStack(char c) {
	int len = strlen(m_bsStack);

	if (len < 0x7F) {
		m_bsStack[len] = c;
		m_bsStack[len + 1] = 0;
		return true;
	}

	return false;
}

// console.cpp:804
char CConsole::PopFromBsStack(void) {
	int len = strlen(m_bsStack);
	if (len == 0) {
		return 0;
	}

	char c = m_bsStack[len - 1];
	m_bsStack[len - 1] = 0;
	return c;
}

// console.cpp:817
void CConsole::ClearBsStack(void) {
	m_bsStack[0] = 0;
}

// console.cpp:822
int CConsole::BrowseCommand(const char *partial, char *result) {
	unsigned int i;
	for (i = 0; i < 0x59; i++) {

		m_keyStates[i].m_code = 1;
	}


	int len = strlen(partial);


	int matched;
	for (matched = 0; matched < len; matched++) {

		bool done = true;

		for (i = 0; i < 0x59; i++) {

			if (m_keyStates[i].m_code) {

				int nameLen = strlen(command_table[i].name);

				if (nameLen > matched && partial[matched] == command_table[i].name[matched]) {





					done = false;
				} else {

					m_keyStates[i].m_code = 0;
				}
			}
		}

		if (done) {
			break;
		}
	}







	bool allBrowsed = true;
	for (i = 0; i < 0x59; i++) {

		if (m_keyStates[i].m_code) {

			if (!m_keyStates[i].m_pressed) {
				allBrowsed = false;
				break;
			}
		}
	}




	if (allBrowsed) {

		for (i = 0; i < 0x59; i++) {

			m_keyStates[i].m_pressed = 0;
		}
	}




	int best = -1;

	for (i = 0; i < 0x59; i++) {

		if (m_keyStates[i].m_code && !m_keyStates[i].m_pressed) {

			int nameLen = strlen(command_table[i].name);

			if (nameLen <= matched) {

				if (result) strcpy(result, command_table[i].name);


				return i;
			}


			if (best == -1 || command_table[best].name[matched] > command_table[i].name[matched]) {

				best = i;
			}
		}
	}


	if (best == -1) {

		*result = 0;
		return -1;
	}




	m_keyStates[best].m_pressed = 1;
	if (result) strcpy(result, command_table[best].name);

	return command_table[best].id;
}

// console.cpp:922
int CConsole::TweakerCommandProc(const char *line) {
	if (stricmp(line, "tw cmd all") == 0) {

		std::vector<std::string> list;
		tweaker_database::instance()->get_tweaker_command_all(list);
		if (list.size() == 0) {
			// "사용 가능한 트위커 명령이 없습니다."
			AddText("\xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9\xC7\xD1 \xC6\xAE\xC0\xA7\xC4\xBF \xB8\xED\xB7\xC9\xC0\xCC \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
			return 1;
		}

		for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
			AddText(it->c_str());
		}
		return 1;
	}

	if (stricmp(line, "tw list") == 0) {

		std::vector<std::string> list;
		tweaker_database::instance()->get_tweaker_names(list);

		if (list.size() == 0) {
			// "사용 가능한 트위커가 없습니다."
			AddText("\xBB\xE7\xBF\xEB \xB0\xA1\xB4\xC9\xC7\xD1 \xC6\xAE\xC0\xA7\xC4\xBF\xB0\xA1 \xBE\xF8\xBD\xC0\xB4\xCF\xB4\xD9.");
			return 1;
		}

		for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
			AddText(it->c_str());
		}
		return 1;
	}

	if (strncmp(line, "tw using", 8) == 0) {

		char name[128] = {0};
		char command[128] = {0};
		_snscanf(line + 9, 127, "%s %s", name, command);
		std::string fullName(name);
		fullName += " ";
		fullName += command;
		s_pTweaker = tweaker_database::instance()->get_tweaker(fullName);
		if (s_pTweaker) {
			// "입력된 트위커를 시작합니다. 다음은 예약된 명령어들 입니다."
			AddText("\xC0\xD4\xB7\xC2\xB5\xC8 \xC6\xAE\xC0\xA7\xC4\xBF\xB8\xA6 \xBD\xC3\xC0\xDB\xC7\xD5\xB4\xCF\xB4\xD9. \xB4\xD9\xC0\xBD\xC0\xBA \xBF\xB9\xBE\xE0\xB5\xC8 \xB8\xED\xB7\xC9\xBE\xEE\xB5\xE9 \xC0\xD4\xB4\xCF\xB4\xD9.");
			std::vector<std::string> list;
			s_pTweaker->get_command_list(list, false);

			for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it) {
				AddText(it->c_str());
			}
		} else {
			// "트위커가 종료되었습니다.(tw list를 통해 확인하신 후 입력하세요.)"
			AddText("\xC6\xAE\xC0\xA7\xC4\xBF\xB0\xA1 \xC1\xBE\xB7\xE1\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.(tw list\xB8\xA6 \xC5\xEB\xC7\xD8 \xC8\xAE\xC0\xCE\xC7\xCF\xBD\xC5 \xC8\xC4 \xC0\xD4\xB7\xC2\xC7\xCF\xBC\xBC\xBF\xE4.)");
		}
		return 1;
	}

	if (s_pTweaker) {

		if (tweaker_database::instance()->is_exist_tweaker(s_pTweaker)) {

			if (s_pTweaker->is_exist_command(std::string(line))) {

				std::string result = s_pTweaker->run_command(line);
				AddText(result.c_str());
				return 1;
			}
		} else {
			// "이전 사용을 선언한 트위커가 더이상 유효하지 않아서 자동 종료되었습니다."
			AddText("\xC0\xCC\xC0\xFC \xBB\xE7\xBF\xEB\xC0\xBB \xBC\xB1\xBE\xF0\xC7\xD1 \xC6\xAE\xC0\xA7\xC4\xBF\xB0\xA1 \xB4\xF5\xC0\xCC\xBB\xF3 \xC0\xAF\xC8\xBF\xC7\xCF\xC1\xF6 \xBE\xCA\xBE\xC6\xBC\xAD \xC0\xDA\xB5\xBF \xC1\xBE\xB7\xE1\xB5\xC7\xBE\xFA\xBD\xC0\xB4\xCF\xB4\xD9.");
			s_pTweaker = NULL;
		}
	} else {

		if (tweaker_database::instance()->is_exist_command(line)) {

			std::string result = tweaker_database::instance()->send_command(line);

			AddText(result.c_str());
			return 1;
		}
	}

	return 0;
}

// console.cpp:1016
void CConsole::Command(const char *line) {
	for (unsigned char i = 0; i < 0x59; i++) {
		int len = strlen(command_table[i].name);
		if (strncmp(line, command_table[i].name, len) == 0 && (line[len] == ' ' || line[len] == '\0')) {
			int id = command_table[i].id;

			switch (id) {
			// console.cpp:1034
			case 66: { // "speed"
				float speed;
				sscanf(line + len + 1, "%f", &speed);

				if (speed < 1.0f) {
					BadArgument(66);
					return;
				}
				m_repeatDelay = speed;
				return;
			}
			// console.cpp:1046
			case 65: // "paint"
				// TODO console.cpp:1046-1054: sscanf "%d" (default -1) into GetPVS().m_markedTriangle (private in quadtree.h)
				return;
			// console.cpp:1055
			case 64: { // "camtarget"
				int target = -1;
				sscanf(line + len + 1, "%d", &target);

				if (target <= 0) {
					BadArgument(64);
					return;
				}
				CTaskManager::Instance()->PostMsg(0, "GolfRule", 0x9D, target, 0, 0, 0);
				return;
			}
			// console.cpp:1217
			case 63: // "plist"
				// TODO console.cpp:1217-1260: GetPartTidList@CGolfDoc, __imp__fopen, __imp__fprintf, MakeStr, AddText@CConsole, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsPart@IFF_STRUCT, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsAuxPart@IFF_STRUCT, __imp__fclose
				return;
			// console.cpp:1261
			case 62: // "transparent"
				// TODO console.cpp:1261-1267: 
				return;
			// console.cpp:1268
			case 61: // "mouse"
				// TODO console.cpp:1268-1279: __imp__sscanf, BadArgument@CConsole
				return;
			// console.cpp:1280
			case 60: { // "camvel"
				int velocity = -1;
				sscanf(line + len + 1, "%d", &velocity);

				if (velocity < 0) {
					BadArgument(60);
					return;
				}
				CTaskManager::Instance()->PostMsg(0, "GolfRule", 0x9B, velocity, 0, 0, 0);
				return;
			}
			// console.cpp:1292
			case 59: // "green"
				// TODO console.cpp:1292-1308: __imp__sscanf, BadArgument@CConsole, GetPVS, MakeStr, AddText@CConsole, CalcGreenPlane@CQuadTree
				return;
			// console.cpp:1309
			case 58: // "unbalance"
				// TODO console.cpp:1309-1329: __imp__sscanf, MakeStr, AddText@CConsole, GolfClub, SetAccuracy@CClub
				return;
			// console.cpp:1330
			case 55: // "condition"
				// TODO console.cpp:1330-1344: __imp__sscanf, BadArgument@CConsole, GetPVS
				return;
			// console.cpp:1345
			case 53: // "bounce"
				// TODO console.cpp:1345-1356: __imp__sscanf, BadArgument@CConsole, GetPVS
				return;
			// console.cpp:1357
			case 54: // "roll"
				// TODO console.cpp:1357-1368: __imp__sscanf, BadArgument@CConsole, GetPVS
				return;
			// console.cpp:1369
			case 52: // "holecup"
				// TODO console.cpp:1369-1383: __imp__sscanf, BadArgument@CConsole
				return;
			// console.cpp:1384
			case 51: { // "bright"
				float bright;
				sscanf(line + len + 1, "%f", &bright);

				if (bright < 0.0f || bright > 1.0f) {
					BadArgument(51);
					return;
				}
				GOLFDOC()->UpdateBright(bright);
				return;
			}
			// console.cpp:1396
			case 45: // "power"
				// TODO console.cpp:1396-1415: __imp__sscanf, PLAYER, MakeStr, AddText@CConsole, ?4?$WCrypticValue@H, PostMsg@CTaskManager, GolfClub, SetAccuracy@CClub, BadArgument@CConsole
				return;
			// console.cpp:1416
			case 46: // "impact"
				// TODO console.cpp:1416-1432: __imp__sscanf, PLAYER, MakeStr, AddText@CConsole, ?4?$WCrypticValue@H, GolfClub, SetAccuracy@CClub, BadArgument@CConsole
				return;
			// console.cpp:1433
			case 47: // "control"
				// TODO console.cpp:1433-1448: __imp__sscanf, PLAYER, MakeStr, AddText@CConsole, ?4?$WCrypticValue@H, BadArgument@CConsole
				return;
			// console.cpp:1449
			case 48: // "spin"
				// TODO console.cpp:1449-1466: __imp__sscanf, PLAYER, MakeStr, AddText@CConsole, ?4?$WCrypticValue@H, PostMsg@CTaskManager, BadArgument@CConsole
				return;
			// console.cpp:1467
			case 49: // "curve"
				// TODO console.cpp:1467-1482: __imp__sscanf, PLAYER, MakeStr, AddText@CConsole, BadArgument@CConsole
				return;
			// console.cpp:1483
			case 50: // "scrmode"
				COption::Instance()->vToggleFullscreen();
				return;
			// console.cpp:1487
			case 44: // "togid"
				// TODO console.cpp:1487-1491: 
				return;
			// console.cpp:1492
			case 43: // "god"
				// TODO console.cpp:1492-1499: PerformPowerBarSetting
				return;
			// console.cpp:1500
			case 42: // "debugline"
				// TODO console.cpp:1500-1507: __imp__sscanf
				return;
			// console.cpp:1508
			case 40: // "lowest"
				// TODO console.cpp:1508-1517: 
				return;
			// console.cpp:1518
			case 41: // "highest"
				// TODO console.cpp:1518-1527: 
				return;
			// console.cpp:1528
			case 38: // "key"
				g_input->IncludeKey(0);
				return;
			// console.cpp:1533
			case 37: // "ai"
				// TODO console.cpp:1533-1552: IsMyTurn, IsConnected@WNetworkSystem, MyIndex, AddText@CConsole
				return;
			// console.cpp:1553
			case 35: // "drop"
				CSceneManager::Instance()->DropTest();
				return;
			// console.cpp:1559
			case 33: // "grid"
				// TODO console.cpp:1559-1566: __imp__sscanf
				return;
			// console.cpp:1567
			case 34: // "gridshow"
				// TODO console.cpp:1567-1581: __imp__sscanf
				return;
			// console.cpp:1582
			case 32: // "trace"
				// TODO console.cpp:1582-1591: __imp__sscanf, GolfBall, BadArgument@CConsole, PreData@CGolfBall, ?4WVector
				return;
			// console.cpp:1592
			case 27: // "statistics"
				// TODO console.cpp:1592-1598: MakeStatistics@WPolySoup
				return;
			// console.cpp:1599
			case 22: // "texture"
				// TODO console.cpp:1599-1613: __imp__sscanf, FindTexture@WResourceManager
				return;
			// console.cpp:1614
			case 21: // "slope"
				// TODO console.cpp:1614-1623: __imp__sscanf, BadArgument@CConsole, GetPVS
				return;
			// console.cpp:1624
			case 18: // "cull"
				// TODO console.cpp:1624-1638: __imp__sscanf
				return;
			// console.cpp:1639
			case 17: // "world"
				// TODO console.cpp:1639-1645: FindModel@WPolySoup, MakeStr
				return;
			// console.cpp:1646
			case 16: // "wind"
				// TODO console.cpp:1646-1664: Wind, GetGlobalDirection@CWind, __imp__sscanf, BadArgument@CConsole, @floorf@4, SetIntensity@CWind, SetDirection@CWind, UpdateBgFxWind@WPolySoup
				return;
			// console.cpp:1665
			case 71: // "rm_camera"
				// TODO console.cpp:1665-1676: IsLocalContent, __imp__sscanf, _POSTMSG
				return;
			// console.cpp:1677
			case 72: // "rm_furniture"
				// TODO console.cpp:1677-1697: IsLocalContent, ?_U@YAPAXI@Z, __imp__sscanf, _POSTMSG
				return;
			// console.cpp:1698
			case 73: // "pivot"
				// TODO console.cpp:1698-1709: IsLocalContent, __imp__sscanf, _POSTMSG
				return;
			// console.cpp:1710
			case 14: // "save"
				// TODO console.cpp:1710-1716: __imp__sscanf
				return;
			// console.cpp:1717
			case 15: // "load"
				// TODO console.cpp:1717-1728: ?_U@YAPAXI@Z, __imp__sscanf, MakeStr, __imp__fopen
				return;
			// console.cpp:1729
			case 13: // "dist"
				// TODO console.cpp:1729-1748: GetPlayer@CGolfDoc, GetHoleCupPos
				return;
			// console.cpp:1749
			case 0: // "help"
				for (int k = 1; k < 0x59; k++) {
					AddText(command_table[k].usage);
				}
				return;
			// console.cpp:1755
			case 1: // "clear"
				for (char *text = m_lines.Start(); text != NULL; text = m_lines.Next()) {
					delete text;
				}
				m_lines.Reset();

				m_lineOffset = 0;
				return;
			// console.cpp:1769
			case 2: // "caddie"
				// TODO console.cpp:1769-1826: __imp__sscanf, FindCaddie@CItemManager, FindCadItem@CItemManager, _POSTMSG
				return;
			// console.cpp:1827
			case 3: // "map"
				// TODO console.cpp:1827-1875: __imp__sscanf, GetCourseNum@CItemManager, FindCourse@CItemManager, AddText@CConsole, GetCurMap, SetCurMap, GetHoleData@CGolfDoc, LoadMapCheckData@WPolySoup, PostMsg@CTaskManager
				return;
			// console.cpp:1876
			case 4: // "hole"
				// TODO console.cpp:1876-1894: __imp__sscanf, PostMsg@CTaskManager
				return;
			// console.cpp:1895
			case 5: // "weather"
				// TODO console.cpp:1895-1918: __imp__sscanf, BadArgument@CConsole, PostMsg@CTaskManager, GenerateStars@CSky, GenerateLensFlare@CSky, GenerateMoons@CSky
				return;
			// console.cpp:1919
			case 6: { // "heal"
				int amount;
				sscanf(line + len + 1, "%d", &amount);
				return;
			}
			// console.cpp:1926
			case 7: // "clubset"
				// TODO console.cpp:1926-1934: __imp__sscanf, PostMsg@CTaskManager
				return;
			// console.cpp:1935
			case 8: // "combo"
				// TODO console.cpp:1935-1948: __imp__sscanf, PLAYER, TEAM, GetComboGaugeLimit@CSharedDoc, ?$Between@M, ?4?$WCrypticValue@M
				return;
			// console.cpp:1949
			case 9: // "tag"
				// TODO console.cpp:1949-1954: 
				return;
			// console.cpp:1955
			case 10: // "render"
				// TODO console.cpp:1955-1982: __imp__sscanf, PostMsg@CTaskManager, BadArgument@CConsole
				return;
			// console.cpp:1983
			case 11: // "fill"
				// TODO console.cpp:1983-1994: __imp__sscanf
				return;
			// console.cpp:1995
			case 12: // "fov"
				// TODO console.cpp:1995-2005: __imp__sscanf, BadArgument@CConsole, SetFOV@WView
				return;
			// console.cpp:2006
			case 19: // "timeout"
				if (CSharedDoc::Instance()->m_shotTimeLimit == 0) {
					CSharedDoc::Instance()->m_shotTimeLimit = 40000;
					GOLFDOC()->m_remainShotTime = CSharedDoc::Instance()->m_shotTimeLimit;
					AddText("TIME LIMIT ON");
					return;
				}
				CSharedDoc::Instance()->m_shotTimeLimit = 0;
				AddText("TIME LIMIT OFF");
				return;
			// console.cpp:2022
			case 20: // "cameracheck"
				// TODO console.cpp:2022-2030: MakeStr
				return;
			// console.cpp:2031
			case 23: // "effectlevel"
				// TODO console.cpp:2031-2043: __imp__sscanf, vSetParticleLevel@COption, BadArgument@CConsole
				return;
			// console.cpp:2044
			case 24: // "mipfilter"
				// TODO console.cpp:2044-2068: __imp__sscanf, BadArgument@CConsole, AddText@CConsole
				return;
			// console.cpp:2069
			case 25: // "mparts"
				// TODO console.cpp:2069-2150: __imp__sscanf, BadArgument@CConsole, __imp__atoi, GetPartTidList@CGolfDoc, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsPart@IFF_STRUCT, PostMsg@CTaskManager, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsAuxPart@IFF_STRUCT, MakeStr
				return;
			// console.cpp:2151
			case 26: // "dparts"
				// TODO console.cpp:2151-2228: __imp__sscanf, BadArgument@CConsole, __imp__strtoul, GetPartTidList@CGolfDoc, MakeStr, AddText@CConsole, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsPart@IFF_STRUCT, PostMsg@CTaskManager, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@IUsAuxPart@IFF_STRUCT
				return;
			// console.cpp:2229
			case 28: // "pangya"
				// TODO console.cpp:2229-2243: __imp__sscanf, BadArgument@CConsole, MakeStr, AddText@CConsole, SetDebugPangYaArea@CPowerGauge
				return;
			// console.cpp:2244
			case 29: // "alpangya"
				// TODO console.cpp:2244-2260: __imp__sscanf, BadArgument@CConsole, MakeStr, AddText@CConsole, SetDebugAirNightLuckyPangYaArea@CPowerGauge
				return;
			// console.cpp:2261
			case 30: // "ball"
				// TODO console.cpp:2261-2278: __imp__sscanf, PostMsg@CTaskManager
				return;
			// console.cpp:2279
			case 31: // "scale"
				// TODO console.cpp:2279-2292: __imp__sscanf, BadArgument@CConsole, MakeStr, AddText@CConsole, PostMsg@CTaskManager
				return;
			// console.cpp:2293
			case 36: // "award"
				CTaskManager::Instance()->PostMsg(0, "GolfRule", 0x78, 0x20000, 0, 0, 0);
				return;
			// console.cpp:2299
			case 39: // "special"
				// TODO console.cpp:2299-2326: __imp__sscanf, PostMsg@CTaskManager, SetDebugPangYaArea@CPowerGauge, MakeStr, AddText@CConsole, PLAYER, TEAM, GetComboGaugeLimit@CSharedDoc, ?$Between@M, ?4?$WCrypticValue@M
				return;
			// console.cpp:2327
			case 67: // "listbres"
				// TODO console.cpp:2327-2350: __imp_??0?$basic_string@DU?$char_traits@D@std, ListBres@Fresh, ?0?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std, __imp_?c_str@?$basic_string@DU?$char_traits@D@std, MakeStr, AddText@CConsole, _Inc@const_iterator@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std, erase@?$_Tree@V?$_Tmap_traits@V?$basic_string@DU?$char_traits@D@std, ?3@YAXPAX@Z, __imp_??1?$basic_string@DU?$char_traits@D@std
				return;
			// console.cpp:2351
			case 68: // "lake"
				sscanf(line + len + 1, "%f %d", &causticSpeed, &causticAlpha);

				if (causticAlpha >= 0xFF) causticAlpha = 0xFF;
				return;
			// console.cpp:2359
			case 69: { // "showmethemoney"
				WSendPacket packet((enumClientPacket)0x86);
				packet.Send(SEND_TO_DEFAULT);
				return;
			}
			// console.cpp:2368
			case 70: { // "gravity"
				float gravity;
				sscanf(line + len + 1, "%f", &gravity);
				GetPVS().GravityModifier(1.0f / gravity);
				return;
			}
			// console.cpp:2376
			case 74: // "bar"
				// TODO console.cpp:2376-2413: __imp__sscanf
				return;
			// console.cpp:2414
			case 78: { // "powerbar"
				float first = 0.0f;
				float second = 0.0f;
				sscanf(line + len + 1, "%f %f", &first, &second);
				return;
			}
			// console.cpp:2429
			case 79: // "posmask"
				// TODO console.cpp:2429-2443: GetCurrentTask@CTaskManager, __imp__sscanf, ChangePartsPosmask@CItemManager, AddText@CConsole
				return;
			// console.cpp:2444
			case 80: // "texname"
				// TODO console.cpp:2444-2463: GetCurrentTask@CTaskManager, __imp__sscanf, ChangePartsTexName@CItemManager
				return;
			// console.cpp:2464
			case 81: // "petname"
				// TODO console.cpp:2464-2494: GetCurrentTask@CTaskManager, __imp__sscanf, ChangePartsPetName@CItemManager, AddText@CConsole
				return;
			// console.cpp:2495
			case 84: // "lua"
				if (lua_system::lua_script_manger::GetInstance()->RunString(line + len + 1)) {
					AddText("lua code Success!");
				}
				return;
			// console.cpp:2504
			case 83: // "luacmd"
				// TODO console.cpp:2504-2700: __imp__sscanf, GetInstance@lua_script_manger@lua_system, ReloadAll@lua_script_manger@lua_system, RunString@lua_script_manger@lua_system, AddText@CConsole, ?$GetContainer@PAVCGimmickContainer, LoadFromScript@CGimmickContainer
				return;
			// console.cpp:2701
			case 85: // "vectortype"
				// TODO console.cpp:2701-2713: ?$GetContainer@PAVCVectorSlideTestContainer, __imp__sscanf
				return;
			// console.cpp:2714
			case 86: // "vectorspeed"
				// TODO console.cpp:2714-2727: ?$GetContainer@PAVCVectorSlideTestContainer, __imp__sscanf
				return;
			// console.cpp:2728
			case 87: // "vectorheight"
				// TODO console.cpp:2728-2740: ?$GetContainer@PAVCVectorSlideTestContainer, __imp__sscanf
				return;
			// console.cpp:2741
			case 88: // "vectorcount"
				// TODO console.cpp:2741-2750: ?$GetContainer@PAVCVectorSlideTestContainer, __imp__sscanf
				return;
			}
			break;
		}
	}

	AddText("Bad Command. Type 'help' -> list command or 'tweaker' -> list tweaker command");
}

// console.cpp:2759
void CConsole::BadArgument(int command) {
	AddText(MakeStr("Bad Argument : %s", command_table[command].usage));
}

// console.cpp:2764
void CConsole::AddText(const char *text) {
	char *line = new char[strlen(text) + 1];
	strcpy(line, text);
	m_lines += line;

	m_lineOffset = m_lines.GetCount() - 19;
}
