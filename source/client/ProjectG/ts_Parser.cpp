// Original: d:\build\custom\temp\code\client\projectg\ts_parser.cpp
#include "ts_parser.h"

// ts_parser.cpp:91
ts_Parser::ts_Parser(void) {
	ClearVariables();
}

// ts_parser.cpp:96
ts_Parser::ts_Parser(const char *fileName) {
	ClearVariables();
}

// ts_parser.cpp:105
ts_Parser::~ts_Parser(void) {
	PTS_CloseFile();
	ClearVariables();
}

// ts_parser.cpp:114
void ts_Parser::ClearVariables(void) {
	m_opened = false;
	memset(m_fileName, 0, sizeof(m_fileName));
	m_fileType = FILETYPE_NONE;
	m_file = NULL;

	memset(m_line, 0, sizeof(m_line));
	memset(m_token, 0, sizeof(m_token));
	memset(m_nextLine, 0, sizeof(m_nextLine));

	m_unknown990 = 0;
	m_unknown992 = 0;

	std::list<sTS_Error *>::iterator errorIt;
	for (errorIt = m_errors.begin(); errorIt != m_errors.end(); ++errorIt) {
		if (*errorIt != NULL) {
			delete *errorIt;
			*errorIt = NULL;
		}
	}
	m_errors.clear();

	std::list<sTS_ScriptInfo *>::iterator infoIt;
	for (infoIt = m_scriptInfos.begin(); infoIt != m_scriptInfos.end(); ++infoIt) {
		if (*infoIt != NULL) {
			delete *infoIt;
			*infoIt = NULL;
		}
	}
	m_scriptInfos.clear();

	std::list<sTS_Macro *>::iterator macroIt;
	for (macroIt = m_macros.begin(); macroIt != m_macros.end(); ++macroIt) {
		if (*macroIt != NULL) {
			delete *macroIt;
			*macroIt = NULL;
		}
	}
	m_macros.clear();

	ClearSymbolTable();
}

// ts_parser.cpp:176
void ts_Parser::ClearSymbolTable(void) {
	std::list<char *>::iterator it;
	for (it = m_symbols.begin(); it != m_symbols.end(); ++it) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	m_symbols.clear();
}

// ts_parser.cpp:193
bool ts_Parser::PTS_OpenFile(const char *fileName, eFileType type) {
	bool result = false;

	if (IsOpenedFile() == result) {
		if (fileName != NULL) {
			memset(m_fileName, 0, sizeof(m_fileName));
			strcpy(m_fileName, fileName);

			m_fileType = type;

			switch (m_fileType) {
			case FILETYPE_NONE:
				break;
			case FILETYPE_BINARY:
				m_file = fopen(m_fileName, "rb");
				break;
			case FILETYPE_TEXT:
				m_file = fopen(m_fileName, "rt");
				break;
			}

			if (m_file != NULL) {
				m_opened = true;
				return true;
			}
		}
	}

	return false;
}

// ts_parser.cpp:243
bool ts_Parser::PTS_CloseFile(void) {
	if (IsOpenedFile() == false) {
		if (m_file != NULL) {
			fclose(m_file);
			m_file = NULL;
			m_opened = false;
			return true;
		}
	}

	return false;
}

// ts_parser.cpp:272
taskMsg_t ts_Parser::GetTaskMessage(const char *name) const {
	if (name == NULL) {
		return (taskMsg_t)-1;
	}

	for (int i = 0; i < TS_TASKMSG_COUNT; i++) {
		if (strcmp(name, gTS_TaskMsgTable[i].m_name) == 0) {
			return gTS_TaskMsgTable[i].m_msg;
		}
	}

	return (taskMsg_t)-1;
}

// ts_parser.cpp:303
eTS_Keyword ts_Parser::GetKeyword(const char *name) const {
	if (name == NULL) {
		return (eTS_Keyword)-1;
	}

	for (int i = 0; i < TS_KEYWORD_COUNT; i++) {
		if (strcmp(name, gTS_KeywordTable[i].m_name) == 0) {
			return gTS_KeywordTable[i].m_keyword;
		}
	}

	return (eTS_Keyword)-1;
}

// ts_parser.cpp:332
void ts_Parser::ConvertMacroType(eTS_Keyword keyword, eMacroType *type) const {
	if (type != NULL) {
		switch (keyword) {
		case 0: *type = (eMacroType)0; break;
		case 1: *type = (eMacroType)1; break;
		case 2: *type = (eMacroType)2; break;
		case 3: *type = (eMacroType)3; break;
		case 4: *type = (eMacroType)4; break;
		case 5: *type = (eMacroType)5; break;
		case 6: *type = (eMacroType)8; break;
		case 7: *type = (eMacroType)9; break;
		case 8: *type = (eMacroType)10; break;
		case 9: *type = (eMacroType)11; break;
		case 10: *type = (eMacroType)12; break;
		case 11: *type = (eMacroType)13; break;
		case 12: *type = (eMacroType)21; break;
		case 13: *type = (eMacroType)22; break;
		case 14: *type = (eMacroType)23; break;
		case 15: *type = (eMacroType)24; break;
		case 16: *type = (eMacroType)25; break;
		case 32:
		case 33:
			break;
		default:
			*type = (eMacroType)-1;
			break;
		}
	}
}

// ts_parser.cpp:375
// The original returns the new symbol's address as an int.
int ts_Parser::InsertSymbolTable(const char *name) {
	int length = strlen(name) + 1;

	char *symbol = new char[length];
	memset(symbol, 0, length);
	strcpy(symbol, name);

	m_symbols.push_back(symbol);

	return (intptr_t)symbol;
}

// ts_parser.cpp:390
int ts_Parser::FindQuotationMarkBlock(const char *text, int &begin, int &end, int index, bool withMarks) {
	if (index > 5) {
		return 0;
	}

	int count = 0;
	int marks[10] = { 0 };
	int length = strlen(text);

	for (int i = 0; i < length; i++) {
		if (text[i] == '"') {
			marks[count++] = i;
		}
	}

	int first = marks[index * 2 - 2];
	int last = marks[index * 2 - 1];

	if (last - first > 0) {
		if (!withMarks) {
			first++;
			last--;
		}

		begin = first;
		end = last;

		return last - first + 1;
	}

	return 0;
}

// ts_parser.cpp:443
void ts_Parser::ClearStringBuffer(void) {
	memset(m_line, 0, sizeof(m_line));
	memset(m_token, 0, sizeof(m_token));
	memset(m_nextLine, 0, sizeof(m_nextLine));
}

// ts_parser.cpp:453
bool ts_Parser::Rewind(void) {
	if (!IsOpenedFile()) return false;

	bool result = true;
	fpos_t pos = 0;
	if (fsetpos(m_file, &pos) != 0) {
		result = false;
	}

	return result;
}

// ts_parser.cpp:472
bool ts_Parser::NextLine(void) {
	if (!IsOpenedFile()) return false;

	bool result = true;
	if (!feof(m_file)) {
		fgets(m_nextLine, sizeof(m_nextLine), m_file);
		if (feof(m_file)) {
			result = false;
		}
	} else {
		result = false;
	}

	return result;
}

// ts_parser.cpp:497
bool ts_Parser::FindToken(const char *token) {
	if (!IsOpenedFile()) return false;

	fpos_t startPos = 0;
	fpos_t pos = 0;
	if (fgetpos(m_file, &startPos) != 0) {
		return false;
	}

	memset(m_line, 0, sizeof(m_line));
	memset(m_token, 0, sizeof(m_token));

	while (!feof(m_file)) {
		if (fgetpos(m_file, &pos) != 0) {
			return false;
		}

		fgets(m_line, sizeof(m_line), m_file);
		sscanf(m_line, "%s", m_token);

		if (strcmp(m_token, token) == 0) {
			return fsetpos(m_file, &pos) == 0 ? true : false;
		}
	}

	fsetpos(m_file, &startPos);
	return false;
}

// ts_parser.cpp:549
bool ts_Parser::RemoveToken(void) {
	if (!IsOpenedFile()) return false;

	bool result = true;
	if (fscanf(m_file, "%s", m_token) != 0) {
		result = false;
	}

	return result;
}

// ts_parser.cpp:565
const char *ts_Parser::GetToken(void) {
	if (!IsOpenedFile()) return NULL;

	fpos_t pos;
	fscanf(m_file, "%s", m_token);

	if (fgetpos(m_file, &pos) != 0) {
		return NULL;
	}

	size_t length = strlen(m_token);

	pos -= length;
	return fsetpos(m_file, &pos) == 0 ? m_token : NULL;
}

// ts_parser.cpp:593
const char *ts_Parser::GetLine(void) {
	if (!IsOpenedFile() ||
		feof(m_file)) return NULL;

	fpos_t pos;
	if (fgetpos(m_file, &pos) != 0) return NULL;

	fgets(m_line, sizeof(m_line), m_file);

	return fsetpos(m_file, &pos) == 0 ? m_line : NULL;
}

// ts_parser.cpp:611
bool ts_Parser::PTS_LoadScriptInfo(void) {
	if (!IsOpenedFile()) return false;

	Rewind();

	ClearStringBuffer();

	std::list<sTS_ScriptInfo *>::iterator it;
	for (it = m_scriptInfos.begin(); it != m_scriptInfos.end(); ++it) {
		if (*it != NULL) {
			delete *it;
			*it = NULL;
		}
	}
	m_scriptInfos.clear();

	ClearSymbolTable();

	if (FindToken("PTS_SCRIPT_INFO")) {
		RemoveToken();
		if (strcmp(GetToken(), "{") == 0) {
			RemoveToken();
			while (strcmp(GetToken(), "}") != 0) {
				sTS_ScriptInfo *info = new sTS_ScriptInfo;

				if (info != NULL) {
					memset(info, 0, sizeof(sTS_ScriptInfo));

					strcpy(info->m_name, GetToken());
					RemoveToken();

					m_scriptInfos.push_back(info);
				}
			}
		}
	}

	int count = m_scriptInfos.size();
	return count > 0 ? true : false;
}

// ts_parser.cpp:680
bool ts_Parser::PTS_LoadScript(const char *fileName) {
	if (!IsOpenedFile()) return false;

	bool found = false;
	std::list<sTS_ScriptInfo *>::iterator it;
	for (it = m_scriptInfos.begin(); it != m_scriptInfos.end(); ++it) {
		if (*it != NULL) {
			if (strcmp((*it)->m_name, fileName) == 0) {
				found = true;
				break;
			}
		}
	}

	if (!found) {
		return false;
	}

	std::list<sTS_Macro *>::iterator macroIt;
	for (macroIt = m_macros.begin(); macroIt != m_macros.end(); ++macroIt) {
		if (*macroIt != NULL) {
			delete *macroIt;
			*macroIt = NULL;
		}
	}
	m_macros.clear();

	ClearStringBuffer();

	Rewind();

	ClearSymbolTable();

	if (FindToken("#BEGIN_SCRIPT_DECLARE")) {
		if (FindToken(fileName)) {
			RemoveToken();
			if (strcmp(GetToken(), "{") == 0) {
				RemoveToken();
				while (strcmp(GetToken(), "}") != 0) {
					sTS_Macro *macro = CommandAnalyst(GetLine());
					if (macro != NULL) {
						m_macros.push_back(macro);
					}
					NextLine();
				}
			}
		}
	}

	int count = m_macros.size();
	return count > 0;
}

// ts_parser.cpp:771
bool ts_Parser::PTS_PopMacro(sTS_Macro *macro) {
	bool result = m_macros.size() > 0;

	if (macro == NULL) {
		return false;
	}

	if (result == true) {
		std::list<sTS_Macro *>::iterator it = m_macros.begin();

		if (it == m_macros.end() || *it == NULL) {
			return false;
		}

		*macro = **it;
		delete *it;
		*it = NULL;
		m_macros.erase(it);
	}

	return result;
}

// ts_parser.cpp:820
sTS_Macro *ts_Parser::CommandAnalyst(const char *text) {
	if (text == NULL) return NULL;

	char keyword[0x40] = "";

	sTS_Macro *macro = new sTS_Macro;
	if (macro != NULL) {
		int length = strlen(text) + 1;
		char *buffer = new char[length];
		memset(buffer, 0, length);
		strcpy(buffer, text);

		sscanf(buffer, "%s", keyword);

		bool pushed = false;
		switch (GetKeyword(keyword)) {
		case 0x11:
			pushed = PushMacro(buffer, *macro);
			break;
		}

		if (!pushed) {
			delete macro;
			macro = NULL;
		}

		if (buffer != NULL) {
			delete buffer;
		}
	}

	return macro;
}
