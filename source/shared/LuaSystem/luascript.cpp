// Original: d:\build\custom\temp\code\shared\luasystem\luascript.cpp
#include <stdio.h>
#include <string.h>

#include "LuaScript.h"
#include "LuaScriptManager.h"
#include "luatablehelper.h"
#include "lua_tinker/lua_tinker.h"
#include "wresrcmng.h"
#include "westpak.h"

// westpak.cpp
void __cdecl CloseCFile(cFile *file);

namespace lua_system {

// luascript.cpp:14
lua_script::lua_script(lua_script_manger *manager, bool reserveDelete) {
	m_manager = manager;
	m_reserveDelete = reserveDelete;
	m_state = STATE_READY;
	m_elapsedTime = 0;

	lua_State *L = manager->GetMasterLuaState();
	m_luaState = lua_newthread(L);
	m_threadRef = luaL_ref(L, LUA_REGISTRYINDEX);

	lua_pushlightuserdata(L, m_luaState);
	lua_pushlightuserdata(L, this);
	lua_settable(L, LUA_GLOBALSINDEX);
}

// luascript.cpp:30
lua_script::~lua_script() {
	luaL_unref(m_manager->GetMasterLuaState(), LUA_REGISTRYINDEX, m_threadRef);
}

// luascript.cpp:37
int lua_script::Update(float elapsed) {
	m_elapsedTime += elapsed;

	switch (m_state) {
	case STATE_WAIT_TIME:
		if (m_elapsedTime >= m_waitTime) {
			ResumeScript(0);
		}
		break;
	case STATE_WAIT_FRAME:
		--m_waitFrame;
		if (m_waitFrame <= 0) {
			ResumeScript(0);
		}
		break;
	case STATE_RUNNING:
		if (m_reserveDelete && m_elapsedTime >= 5.0f) {
			m_state = STATE_DELETE;
		}
		break;
	}

	return m_state;
}

// luascript.cpp:65
void lua_script::WaitTimeScript(float time) {
	m_waitTime = time;
	m_elapsedTime = 0;
	m_state = STATE_WAIT_TIME;
}

// luascript.cpp:72
void lua_script::WaitFrameScript(int frame) {
	m_waitFrame = frame;
	m_state = STATE_WAIT_FRAME;
}

// luascript.cpp:78
bool lua_script::RunFile(const char *filename) {
	bool result = false;

	cFile *file = g_resrcmng->GetCFile(filename, 0xFFFF);
	if (file) {
		char *buffer = new char[file->Length() + 1];
		file->Read(buffer, file->Length());
		buffer[file->Length()] = 0;
		CloseCFile(file);

		int error = luaL_loadbuffer(m_luaState, buffer, strlen(buffer), filename);

		if (error == 0) {
			ResumeScript(0);
			result = true;
			m_fileName = filename;
		} else {
			FormatError();
		}

		delete[] buffer;
	}

	return result;
}

// luascript.cpp:116
bool lua_script::RunString(const char *str) {
	bool result = false;

	m_scriptString = str;

	int error = luaL_loadbuffer(m_luaState, str, strlen(str), "lua_script::RunString");

	if (error == 0) {
		ResumeScript(0);
		result = true;
	} else {
		FormatError();
	}

	return result;
}

// luascript.cpp:145
void lua_script::Reload() {
	if (m_fileName.size() > 0) {
		RunFile(m_fileName.c_str());
	}
	if (m_scriptString.size() > 0) {
		RunString(m_scriptString.c_str());
	}
}

// luascript.cpp:155
void lua_script::ResumeScript(float value) {
	m_state = STATE_RUNNING;
	m_elapsedTime = 0;
	lua_pushnumber(m_luaState, value);

	int error = lua_resume(m_luaState, 1);

	if (error != 0) {
		FormatError();
	}
}

// luascript.cpp:170
void lua_script::AbortWait() {
	ResumeScript(1.0f);
}

// luascript.cpp:177
void lua_script::FormatError() {
	const char *message = lua_tostring(m_luaState, -1);

	lua_pop(m_luaState, 1);

	if (message == NULL) {
		const char *noMessage = "(error with no message)";
		strcpy(m_errorMessage, noMessage);
	} else {
		strcpy(m_errorMessage, message);
	}
}

// luascript.cpp:204
void lua_script::OutputError(char *where) {
	printf("%s %s \n", where, m_errorMessage);
}

// luascript.cpp:209
lua_table_helper lua_script::GetTable(const char *name) {
	lua_tinker::table table = lua_tinker::get<lua_tinker::table>(m_luaState, name);

	return lua_table_helper(table);
}

}
