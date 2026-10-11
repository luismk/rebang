// Original: d:\build\custom\temp\code\client\projectg\ts_parser.h
#ifndef PG_CLIENT_PROJECTG_TS_PARSER_H
#define PG_CLIENT_PROJECTG_TS_PARSER_H

#include <stdio.h>
#include <string.h>
#include <list>

#include "..\..\defines.h"
#include "EventActor.h"

// The kind of a tutorial macro (the script commands ts_Parser::ConvertMacroType
// maps its keywords to). Enumerators not recovered yet but MACRO_WAIT.
enum eMacroType {
	MACRO_WAIT = 0, // the macro's m_param is a delay in ms
};

// A keyword of the tutorial script language (gTS_KeywordTable). Enumerators
// not recovered yet.
enum eTS_Keyword {
};

// One entry of gTS_TaskMsgTable: a task message by its script name.
struct sTS_TaskMsg {
	taskMsg_t m_msg;  // 0x00
	char m_name[0x20]; // 0x04
};
CHECK_SIZE(sTS_TaskMsg, 0x24);
CHECK_FIELD(sTS_TaskMsg, m_name, 0x04);

// One entry of gTS_KeywordTable: a keyword by its script name.
struct sTS_Keyword {
	eTS_Keyword m_keyword; // 0x00
	char m_name[0x20];     // 0x04
};
CHECK_SIZE(sTS_Keyword, 0x24);
CHECK_FIELD(sTS_Keyword, m_name, 0x04);

#define TS_TASKMSG_COUNT 9
#define TS_KEYWORD_COUNT 43

extern sTS_TaskMsg gTS_TaskMsgTable[TS_TASKMSG_COUNT];
extern sTS_Keyword gTS_KeywordTable[TS_KEYWORD_COUNT];

struct sTS_Error;

// One script of the file's PTS_SCRIPT_INFO block (ts_Parser::PTS_LoadScriptInfo);
// PTS_LoadScript looks a script up by name. Zeroed with memset on creation.
struct sTS_ScriptInfo {
	char m_name[0x20]; // 0x00
};
CHECK_SIZE(sTS_ScriptInfo, 0x20);

// One parsed script command, queued by ts_Parser::PushMacro.
struct sTS_Macro {
	// ts_parser.h:177
	sTS_Macro(void) {
		m_type = (eMacroType)-1; m_text = NULL;
		m_param = 0;
		m_count = 0;
	}

	// ts_parser.h:186
	~sTS_Macro(void) {
		if (m_text != NULL) {
			delete m_text;
			m_text = NULL;
		}
	}

	// ts_parser.h:195
	sTS_Macro &operator=(sTS_Macro &other) {
		m_type = other.m_type;

		if (m_text != NULL) {
			delete m_text;
			m_text = NULL;
		}

		if (other.m_text != NULL) {
			int length = strlen(other.m_text) + 1;
			m_text = new char[length];
			strcpy(m_text, other.m_text);
		}

		m_param = other.m_param;
		m_count = other.m_count;

		return *this;
	}

	eMacroType m_type; // 0x00
	char *m_text;      // 0x04, the command's arguments
	int m_param;       // 0x08
	int m_count;       // 0x0C
};
CHECK_SIZE(sTS_Macro, 0x10);
CHECK_FIELD(sTS_Macro, m_text, 0x04);
CHECK_FIELD(sTS_Macro, m_param, 0x08);
CHECK_FIELD(sTS_Macro, m_count, 0x0C);

// The tutorial script parser.
class ts_Parser {
	LAYOUT_CHECK_FRIEND;
public:
	enum eFileType {
		FILETYPE_NONE = 0,
		FILETYPE_BINARY = 1, // "rb"
		FILETYPE_TEXT = 2,   // "rt"
	};

	ts_Parser(void);
	ts_Parser(const char *fileName);
	virtual ~ts_Parser(void);

	bool PTS_OpenFile(const char *fileName, eFileType type);
	bool PTS_CloseFile(void);
	taskMsg_t GetTaskMessage(const char *name) const;
	eTS_Keyword GetKeyword(const char *name) const;
	void ConvertMacroType(eTS_Keyword keyword, eMacroType *type) const;
	int FindQuotationMarkBlock(const char *text, int &begin, int &end, int start, bool flag);
	bool PTS_PopMacro(sTS_Macro *macro);
	bool PTS_LoadScriptInfo(void);
	bool PTS_LoadScript(const char *fileName);

protected:
	inline bool IsOpenedFile(void) const;
	void ClearVariables(void);
	void ClearSymbolTable(void);
	void ClearStringBuffer(void);
	bool Rewind(void);
	bool NextLine(void);
	bool FindToken(const char *token);
	bool RemoveToken(void);
	const char *GetToken(void);
	const char *GetLine(void);
	int InsertSymbolTable(const char *name);
	bool PushMacro(const char *text, sTS_Macro &macro);
	sTS_Macro *CommandAnalyst(const char *text);

	bool m_opened;                           // 0x004
	char m_fileName[0x100];                  // 0x005
	eFileType m_fileType;                    // 0x108
	FILE *m_file;                            // 0x10C
	char m_line[0x800];                      // 0x110, GetLine
	char m_token[0x40];                      // 0x910, GetToken / RemoveToken
	char m_nextLine[0x40];                   // 0x950, NextLine
	short m_unknown990;                      // 0x990
	short m_unknown992;                      // 0x992
	std::list<sTS_Error *> m_errors;         // 0x994
	std::list<sTS_ScriptInfo *> m_scriptInfos; // 0x9A0
	std::list<sTS_Macro *> m_macros;         // 0x9AC
	std::list<char *> m_symbols;             // 0x9B8, InsertSymbolTable
};
CHECK_SIZE(ts_Parser, 0x9C4);
BEGIN_LAYOUT_CHECK(ts_Parser)
CHECK_FIELD(ts_Parser, m_opened, 0x004);
CHECK_FIELD(ts_Parser, m_fileName, 0x005);
CHECK_FIELD(ts_Parser, m_fileType, 0x108);
CHECK_FIELD(ts_Parser, m_file, 0x10C);
CHECK_FIELD(ts_Parser, m_line, 0x110);
CHECK_FIELD(ts_Parser, m_token, 0x910);
CHECK_FIELD(ts_Parser, m_nextLine, 0x950);
CHECK_FIELD(ts_Parser, m_unknown990, 0x990);
CHECK_FIELD(ts_Parser, m_errors, 0x994);
CHECK_FIELD(ts_Parser, m_scriptInfos, 0x9A0);
CHECK_FIELD(ts_Parser, m_macros, 0x9AC);
CHECK_FIELD(ts_Parser, m_symbols, 0x9B8);
END_LAYOUT_CHECK

#include "ts_parser.inl"

#endif // PG_CLIENT_PROJECTG_TS_PARSER_H
