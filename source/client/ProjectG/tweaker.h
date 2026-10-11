// Original: d:\build\custom\temp\code\client\projectg\tweaker.h
#ifndef PG_CLIENT_PROJECTG_TWEAKER_H
#define PG_CLIENT_PROJECTG_TWEAKER_H

#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "..\..\defines.h"

class tweaker;
class tweaker_database;

// One token of a tweaker command line (a copy of its text).
class tweaker_token {
public:
	tweaker_token(const std::string& token) : m_token(token) {}   // tweaker.h:37

	const char* to_cstr(void) const { return m_token.c_str(); }   // tweaker.h:39
	int to_int(void) const { return atoi(m_token.c_str()); }   // tweaker.h:41
	float to_float(void) const { return (float)atof(m_token.c_str()); }   // tweaker.h:42

private:
	std::string m_token;   // 0x00
};

CHECK_SIZE(tweaker_token, 0x1C);

// Parameter block passed to a tweaker command handler: the command line's
// tokens after the command name.
class tweaker_param {
public:
	tweaker_param(void);
	tweaker_param(std::vector<std::string>& tokens);

	tweaker_token operator[](unsigned int index) const;

private:
	std::vector<std::string> m_tokens;   // 0x00
};

CHECK_SIZE(tweaker_param, 0x10);

// Base of the objects that receive tweaker (debug console) commands. Only its
// vtable pointer is seen (sizeof 4); its vtable holds the destructor alone.
class tweaker_cmd_target {
public:
	tweaker_cmd_target(void) {}

protected:
	virtual ~tweaker_cmd_target(void);
};

CHECK_SIZE(tweaker_cmd_target, 0x4);

// Command handler of a tweaker_cmd_target (single inheritance: 4 bytes).
typedef std::string (tweaker_cmd_target::*tweaker_cmd_func)(const tweaker_param& param);

// The commands of one named tweaker and the targets they are run on.
class tweaker {
	friend class tweaker_database;

public:
	struct tweaker_cmd_info {
		tweaker_cmd_func func;   // 0x00
		std::string help;   // 0x04
	};

	typedef std::set<tweaker_cmd_target*> alloc_set;
	typedef std::map<std::string, tweaker_cmd_info> command_map;

	std::string run_command(const char* line);
	bool is_exist_command(const std::string& line);
	void get_command_list(std::vector<std::string>& list, bool withHelp) const;

private:
	tweaker(const char* name);

	void _regist_command(const char* name, const char* cmd, tweaker_cmd_info& info);
	std::string _run_command(std::string& cmd, tweaker_param& param);
	void _insert_alloc(tweaker_cmd_target* target);
	void _erase_alloc(tweaker_cmd_target* target);
	bool _is_exist_alloc(tweaker_cmd_target* target);

	unsigned int _size_command(void) const { return m_commands.size(); }   // tweaker.h:99
	unsigned int _size_alloc(void) const { return m_allocs.size(); }   // tweaker.h:100

	alloc_set m_allocs;   // 0x00
	command_map m_commands;   // 0x0C
	std::string m_name;   // 0x18
};

CHECK_SIZE(tweaker, 0x34);

// The named tweakers, a function-local singleton.
class tweaker_database {
	friend class tweaker_cmd_target;

public:
	typedef std::map<std::string, tweaker*> tweaker_map;

	tweaker_database(void) {}

	static tweaker_database* instance(void) {   // tweaker.h:121
		static tweaker_database _inst;
		return &_inst;
	}

	tweaker* get_tweaker(std::string& name);
	bool is_exist_tweaker(const tweaker* tw);
	bool is_exist_command(const char* line);
	void get_tweaker_command_all(std::vector<std::string>& list);
	void get_tweaker_names(std::vector<std::string>& list);
	std::string send_command(const char* line);

private:
	~tweaker_database(void);

	tweaker* _regist_tweaker(const char* name, tweaker_cmd_target* target);
	void _unregist_tweaker(tweaker_cmd_target* target);

	tweaker_map m_tweakers;   // 0x00
};

CHECK_SIZE(tweaker_database, 0xC);

#endif // PG_CLIENT_PROJECTG_TWEAKER_H
