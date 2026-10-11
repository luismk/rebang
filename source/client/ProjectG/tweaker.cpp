// Original: d:\build\custom\temp\code\client\projectg\tweaker.cpp
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "tweaker.h"

namespace {

// tweaker.cpp:19
bool tokenize(const std::string& str, const char* delims, std::vector<std::string>& tokens) {
	std::string token;
	size_t count = strlen(delims);
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		size_t i;
		for (i = 0; i < count; ++i) {
			if (delims[i] == *it && token.length() > 0) {
				tokens.push_back(token);
				token.clear();
				if (++it == str.end())
					return true;
				break;
			}
		}
		bool isDelim = false;
		for (i = 0; i < count; ++i) {
			if (delims[i] == *it) {
				isDelim = true;
				break;
			}
		}
		if (!isDelim)
			token += *it;
	}
	if (token.length() > 0)
		tokens.push_back(token);
	return tokens.size() == 0;
}

// tweaker.cpp:62
void output_trace(const char* format, ...) {
	char buffer[512] = { 0, };
	va_list args;
	va_start(args, format);
	if (_vsnprintf(buffer, sizeof(buffer) - 1, format, args) != -1) {
		size_t len = strlen(buffer);
		buffer[len] = '\n';
		buffer[len + 1] = '\0';
	}
}

} // namespace

// tweaker.cpp:88
tweaker_cmd_target::~tweaker_cmd_target(void) {
	tweaker_database::instance()->_unregist_tweaker(this);
}

// tweaker.cpp:96
tweaker_param::tweaker_param(void) {
}

// tweaker.cpp:98
tweaker_param::tweaker_param(std::vector<std::string>& tokens) {
	if (!tokens.empty())
		m_tokens.assign(tokens.begin(), tokens.end());
}

// tweaker.cpp:104
tweaker_token tweaker_param::operator[](unsigned int index) const {
	if (m_tokens.size() > index)
		return tweaker_token(m_tokens[index]);

	output_trace("tweaker_token index overrun!");
	return tweaker_token(std::string(""));
}

// tweaker.cpp:118
tweaker::tweaker(const char* name)
	: m_name(name) {
}

// tweaker.cpp:122
void tweaker::_regist_command(const char* name, const char* cmd, tweaker_cmd_info& info) {
	if (m_name != name)
		return;

	if (m_commands.find(cmd) == m_commands.end()) {
		output_trace("succeed regist tweaker command : &%s::%s", name, cmd);
		m_commands.insert(command_map::value_type(std::string(cmd), info));
	}
}

// tweaker.cpp:133
std::string tweaker::_run_command(std::string& cmd, tweaker_param& param) {
	command_map::iterator found = m_commands.find(cmd);
	if (found == m_commands.end())
		return "unknown command";

	std::string result;
	tweaker_cmd_func func = found->second.func;
	alloc_set::iterator it = m_allocs.begin();
	for (; it != m_allocs.end(); ++it) {
		tweaker_cmd_target* target = *it;
		result = (target->*func)(param);
	}
	return result;
}

// tweaker.cpp:149
std::string tweaker::run_command(const char* line) {
	if (line == NULL)
		return "warning : parameter null";

	std::vector<std::string> tokens;
	tokenize(std::string(line), " ,", tokens);
	if (tokens.size() == 0)
		return "warning : parameter empty";

	std::string cmd = tokens[0];
	tokens.erase(tokens.begin());
	return _run_command(cmd, tweaker_param(tokens));
}

// tweaker.cpp:164
bool tweaker::is_exist_command(const std::string& line) {
	std::vector<std::string> tokens;
	tokenize(line, " ,", tokens);
	if (tokens.size() == 0)
		return false;

	command_map::iterator it = m_commands.find(tokens[0]);
	return it != m_commands.end();
}

// tweaker.cpp:175
void tweaker::get_command_list(std::vector<std::string>& list, bool withName) const {
	command_map::const_iterator it = m_commands.begin();
	for (; it != m_commands.end(); ++it) {
		std::string text;
		if (withName) {
			text = m_name;
			text += " ";
		}
		text += it->first;
		if (!it->second.help.empty()) {
			text += " (";
			text += it->second.help;
			text += ")";
		}
		list.push_back(text);
	}
}

// tweaker.cpp:199
void tweaker::_insert_alloc(tweaker_cmd_target* target) {
	if (m_allocs.find(target) == m_allocs.end()) {
		m_allocs.insert(target);
		output_trace("succeed tweaker insert alloc : 0x%p", target);
	}
}

// tweaker.cpp:208
void tweaker::_erase_alloc(tweaker_cmd_target* target) {
	alloc_set::iterator it = m_allocs.find(target);
	if (it != m_allocs.end())
		m_allocs.erase(it);
}

// tweaker.cpp:215
bool tweaker::_is_exist_alloc(tweaker_cmd_target* target) {
	alloc_set::iterator it = m_allocs.find(target);
	return it != m_allocs.end();
}

// tweaker.cpp:225
tweaker_database::~tweaker_database(void) {
	tweaker_map::iterator it = m_tweakers.begin();
	for (; it != m_tweakers.end(); ++it) {
		delete it->second;
	}
	m_tweakers.clear();
}

// tweaker.cpp:236
std::string tweaker_database::send_command(const char* line) {
	std::string str(line);
	std::string name;

	if (str.empty())
		return "> command string too short.";

	std::vector<std::string> tokens;
	tokenize(str, " ,", tokens);

	name = tokens[0];
	tokens.erase(tokens.begin());

	if (tokens.size() == 0)
		return "> command string too short.";
	if (name == "class" || name == "struct") {
		name += " ";
		name += tokens[0];
		tokens.erase(tokens.begin());
	}

	tweaker* tw = get_tweaker(name);
	if (tw == NULL || tokens.empty())
		return "> tweaker not found.";

	str = tokens[0];
	tokens.erase(tokens.begin());

	std::string result("> ");
	result += tw->_run_command(str, tweaker_param(tokens));
	return result;
}

// tweaker.cpp:267
bool tweaker_database::is_exist_command(const char* line) {
	std::string str(line);
	std::string name;

	if (str.empty())
		return false;

	std::vector<std::string> tokens;
	tokenize(str, " ,", tokens);

	name = tokens[0];
	tokens.erase(tokens.begin());

	if (tokens.size() == 0)
		return false;
	if (name == "class" || name == "struct") {
		name += " ";
		name += tokens[0];
		tokens.erase(tokens.begin());
	}

	tweaker* tw = get_tweaker(name);
	if (tw == NULL || tokens.empty())
		return false;

	str = tokens[0];
	return tw->is_exist_command(str);
}

// tweaker.cpp:294
bool tweaker_database::is_exist_tweaker(const tweaker* tw) {
	tweaker_map::iterator it = m_tweakers.begin();
	for (; it != m_tweakers.end(); ++it) {
		if (it->second == tw)
			return true;
	}
	return false;
}

// tweaker.cpp:306
void tweaker_database::get_tweaker_command_all(std::vector<std::string>& list) {
	list.clear();

	unsigned int count = 0;
	tweaker_map::iterator it;
	for (it = m_tweakers.begin(); it != m_tweakers.end(); ++it)
		count += it->second->_size_command();

	list.reserve(count);

	for (it = m_tweakers.begin(); it != m_tweakers.end(); ++it)
		it->second->get_command_list(list, true);
}

// tweaker.cpp:325
void tweaker_database::get_tweaker_names(std::vector<std::string>& list) {
	list.clear();

	list.reserve(m_tweakers.size());

	tweaker_map::iterator it = m_tweakers.begin();
	for (; it != m_tweakers.end(); ++it)
		list.push_back(it->first);
}

// tweaker.cpp:341
tweaker* tweaker_database::get_tweaker(std::string& name) {
	tweaker_map::iterator it = m_tweakers.find(name);
	if (it != m_tweakers.end())
		return it->second;

	return NULL;
}

// tweaker.cpp:350
tweaker* tweaker_database::_regist_tweaker(const char* name, tweaker_cmd_target* target) {
	tweaker_map::iterator it = m_tweakers.find(name);
	if (it != m_tweakers.end()) {
		it->second->_insert_alloc(target);
		return it->second;
	}

	tweaker* tw = new tweaker(name);
	tw->_insert_alloc(target);
	m_tweakers.insert(tweaker_map::value_type(std::string(name), tw));
	output_trace("succeed tweaker regist : 0x%p", target);
	return tw;
}

// tweaker.cpp:366
void tweaker_database::_unregist_tweaker(tweaker_cmd_target* target) {
	tweaker_map::iterator it = m_tweakers.begin();
	while (it != m_tweakers.end()) {
		tweaker* tw = it->second;
		if (tw->_is_exist_alloc(target)) {
			tw->_erase_alloc(target);
			if (tw->_size_alloc() == 0) {
				delete tw;
				m_tweakers.erase(it);
				output_trace("succeed tweaker unregist : 0x%p", target);
				return;
			}
		}
		++it;
	}
}
