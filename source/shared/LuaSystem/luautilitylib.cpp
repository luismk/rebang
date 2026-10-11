// Original: d:\build\custom\temp\code\shared\luasystem\luautilitylib.cpp
extern "C" { 
#include "Lua/src/lua.h"
#include "Lua/src/lauxlib.h"
}
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string>
#include "LuaUtilityLib.h"
#include "lua_tinker/lua_tinker.h"
#include "LuaScript.h"
#include "LuaScriptManager.h"
#include "wresrcmng.h"
#include "westpak.h"

// westpak.cpp
void __cdecl CloseCFile(cFile *file);

// Set by debug_line_trace_start, cleared by debug_line_trace_end.
static bool data_00b7b6ec;

static int debug_line_trace_start(lua_State *L);
static int debug_line_trace_end(lua_State *L);
static int debug_local_value(lua_State *L);
static int print_value_info(lua_State *L);
static bool is_debug_mode();
static const char *get_my_country();
static const char *get_current_dir(const char *name);
static void print_client(const char *text);
static int random(int low, int high);
static int wait_frame(lua_State *L);
static int wait_time(lua_State *L);
static lua_system::lua_script *get_lua_script(lua_State *L);

// The path get_current_dir returns.
static char current_dir_buffer[1024];

static const luaL_Reg script_lib[] = {
	{ "WaitFrame", wait_frame },
	{ "WaitSec", wait_time },
	{ NULL, NULL }
};

namespace lua_system {

// luautilitylib.cpp:45
void LuaOpenUtilityLib(lua_State *L) {
	luaL_openlib(L, "Script", script_lib, 0);

	lua_register(L, "dofile_ex", lua_dofile_for_projectG);

	lua_register(L, "debug_line_trace_start", debug_line_trace_start);
	lua_register(L, "debug_line_trace_end", debug_line_trace_end);
	lua_register(L, "debug_local_value", debug_local_value);
	lua_register(L, "print_value_info", print_value_info);

	lua_tinker::def(L, "is_debug_mode", is_debug_mode);
	lua_tinker::def(L, "get_my_country", get_my_country);
	lua_tinker::def(L, "get_current_dir", get_current_dir);
	lua_tinker::def(L, "print_client", print_client);
	lua_tinker::def(L, "random", random);
}

// luautilitylib.cpp:76
void print_debug_local_value(lua_State *L, const lua_Debug *ar) {
	if (ar->currentline <= 0) {
		return;
	}
	if (strcmp(ar->source, "?") == 0) {
		return;
	}

	char text[256];
	int index = 1;
	sprintf(text, "\t\t\t---- Debug Line : %s [%d] -----", ar->source, ar->currentline);

	const char *name;
	while ((name = lua_getlocal(L, ar, index)) != NULL) {
		if (strcmp(name, "(*temporary)") == 0) {
			break;
		}

		const char *value = lua_tostring(L, -1);

		index++;

		sprintf(text, "\t\t\t\t%s = %s", name, value);
	}
}

// luautilitylib.cpp:105
const char *lua_file_line_text_manager::get_line_text(const char *filename, int line) {
	if (filename == NULL) {
		return NULL;
	}

	std::map<std::string, std::vector<std::string> >::iterator it = m_files.find(filename);
	if (it == m_files.end()) {
		return NULL;
	}

	if (it->second.size() < (unsigned int)line) {
		return NULL;
	}

	return it->second[line - 1].c_str();
}

// luautilitylib.cpp:123
void lua_file_line_text_manager::road_lua_file(const char *filename) {
	if (filename == NULL) {
		return;
	}

	cFile *file = g_resrcmng->GetCFile(filename, 0xFFFF);
	if (file == NULL) {
		return;
	}

	char *buffer = new char[file->Length() + 1];
	file->Read(buffer, file->Length());
	buffer[file->Length()] = 0;
	CloseCFile(file);

	std::vector<std::string> lines;

	std::map<std::string, std::vector<std::string> >::iterator it = m_files.find(filename);
	if (it != m_files.end()) {
		m_files.erase(it);
	}

	const char *p = buffer;

	std::string text;

	while (*p != '\0') {
		if (*p == '\n') {
			lines.push_back(text);
			text.clear();

			++p;
			if (*p == '\0') {
				break;
			}
		}

		text += *p;

		++p;
	}

	delete[] buffer;

	if (text.size() != 0) {
		lines.push_back(text);
	}

	m_files[filename] = lines;
}

// luautilitylib.cpp:177
void lua_file_line_text_manager::road_lua_buffer(const char *filename, const char *buffer) {
	if (filename == NULL || buffer == NULL) {
		return;
	}

	std::vector<std::string> lines;

	std::map<std::string, std::vector<std::string> >::iterator it = m_files.find(filename);
	if (it != m_files.end()) {
		m_files.erase(it);
	}

	const char *p = buffer;

	std::string text;

	while (*p != '\0') {
		if (*p == '\n') {
			lines.push_back(text);
			text.clear();

			++p;
			if (*p == '\0') {
				break;
			}
		}

		text += *p;

		++p;
	}

	if (text.size() != 0) {
		lines.push_back(text);
	}

	m_files[filename] = lines;
}

// luautilitylib.cpp:222
bool lua_file_line_text_manager::is_loaded_lua_file(const char *filename) {
	if (filename == NULL) {
		return false;
	}

	std::map<std::string, std::vector<std::string> >::iterator it = m_files.find(filename);
	return it != m_files.end();
}

// luautilitylib.cpp:237
lua_file_line_text_manager *lua_file_line_text_manager::get_instance() {
	static lua_file_line_text_manager instance;
	return &instance;
}

// luautilitylib.cpp:245
int lua_dofile_for_projectG(lua_State *L) {
	const char *filename = lua_tostring(L, -1);

	lua_script *script = lua_script_manger::GetInstance()->CreateScript(true);
	script->RunFile(filename);

	return 0;
}

}

// luautilitylib.cpp:259
static int wait_frame(lua_State *L) {
	lua_system::lua_script *script = get_lua_script(L);

	int frame = (int)luaL_checknumber(L, 1);

	script->WaitFrameScript(frame);

	return lua_yield(L, 1);
}

// luautilitylib.cpp:272
static int wait_time(lua_State *L) {
	lua_system::lua_script *script = get_lua_script(L);

	float time = (float)luaL_checknumber(L, 1);

	script->WaitTimeScript(time);

	return lua_yield(L, 1);
}

// luautilitylib.cpp:287
static lua_system::lua_script *get_lua_script(lua_State *L) {
	lua_pushlightuserdata(L, L);
	lua_gettable(L, LUA_GLOBALSINDEX);
	return (lua_system::lua_script *)lua_touserdata(L, -1);
}

// luautilitylib.cpp:294
static bool is_debug_mode() {
	return false;
}

// luautilitylib.cpp:303
static const char *get_my_country() {
	return "KOREA";
}

// luautilitylib.cpp:317
static const char *get_current_dir(const char *name) {
	memset(current_dir_buffer, 0, sizeof(current_dir_buffer));
	char current_dir[1024] = { 0 };
	GetCurrentDirectoryA(sizeof(current_dir), current_dir);
	std::string path(current_dir);

	if (name != NULL && strcmp(name, "?") != 0) {
		path += "\\";
		path += name;
	}

	strncpy(current_dir_buffer, path.c_str(), sizeof(current_dir_buffer) - 1);
	return current_dir_buffer;
}

// luautilitylib.cpp:339
static void print_client(const char *text) {
}

// luautilitylib.cpp:383
static int debug_line_trace_start(lua_State *L) {
	return 0;
}

// luautilitylib.cpp:392
static int debug_line_trace_end(lua_State *L) {
	data_00b7b6ec = false;

	return 0;
}

// luautilitylib.cpp:399
static int debug_local_value(lua_State *L) {
	return 0;
}

// luautilitylib.cpp:408
static int print_value_info(lua_State *L) {
	return 0;
}

// luautilitylib.cpp:451
static int random(int low, int high) {
	if (low == high) {
		return high;
	}

	srand((unsigned int)time(NULL));

	if (low > high) {
		int swap = high;
		high = low;
		low = swap;
	}

	return rand() % (high - low) + low;
}
