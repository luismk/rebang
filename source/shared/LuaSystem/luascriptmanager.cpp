// Original: d:\build\custom\temp\code\shared\luasystem\luascriptmanager.cpp
#include <algorithm>

#include "LuaScriptManager.h"
#include "LuaScript.h"
#include "LuaUtilityLib.h"
#include "luatablehelper.h"
#include "lua_tinker/lua_tinker.h"
#include "wresrcmng.h"
#include "westpak.h"

// westpak.cpp
void __cdecl CloseCFile(cFile *file);

namespace lua_system {

// luascriptmanager.cpp:14
lua_script_manger *lua_script_manger::GetInstance() {
	static lua_script_manger instance;
	return &instance;
}

// luascriptmanager.cpp:20
lua_script_manger::lua_script_manger() {
	m_masterLuaState = luaL_newstate();

	if (m_masterLuaState == NULL) {
		return;
	}

	luaL_openlibs(m_masterLuaState);

	LuaOpenUtilityLib(m_masterLuaState);

	cFile *file = g_resrcmng->GetCFile("root.lua", 0xFFFF);
	if (file) {
		char *buffer = new char[file->Length() + 1];
		file->Read(buffer, file->Length());
		buffer[file->Length()] = 0;
		CloseCFile(file);

		lua_tinker::dostring(m_masterLuaState, buffer, "root.lua");

		delete[] buffer;
	}
}

// luascriptmanager.cpp:53
lua_script_manger::~lua_script_manger() {
	std::list<lua_script *>::iterator it;
	for (it = m_scripts.begin(); it != m_scripts.end(); ++it) {
		if (*it) {
			delete *it;
		}
		*it = NULL;
	}

	m_scripts.clear();

	lua_close(m_masterLuaState);
}

// luascriptmanager.cpp:69
lua_script *lua_script_manger::CreateScript(bool master) {
	lua_script *script = new lua_script(this, master);

	if (script == NULL) {
		return NULL;
	}

	m_scripts.push_back(script);

	return script;
}

// luascriptmanager.cpp:85
void lua_script_manger::ReleaseScript(lua_script *script) {
	script->ReserveDelete();
}

// luascriptmanager.cpp:90
void lua_script_manger::DeleteScript(lua_script *script) {
	std::list<lua_script *>::iterator it = std::find(m_scripts.begin(), m_scripts.end(), script);

	if (it != m_scripts.end()) {
		if (*it) {
			delete *it;
		}
		*it = NULL;
		m_scripts.erase(it);
	}
}

// luascriptmanager.cpp:104
void lua_script_manger::ReloadAll() {
	std::list<lua_script *>::iterator it = m_scripts.begin();
	for (; it != m_scripts.end(); ++it) {
		(*it)->Reload();
	}
}

// luascriptmanager.cpp:111
void lua_script_manger::Update(float elapsed) {
	std::list<lua_script *>::iterator it = m_scripts.begin();

	for (; it != m_scripts.end(); ++it) {
		if ((*it)->Update(elapsed) == lua_script::STATE_DELETE) {
			std::list<lua_script *>::iterator next = it;
			++next;

			DeleteScript(*it);
			it = next;
		}
	}
}

// luascriptmanager.cpp:127
bool lua_script_manger::RunString(const char *str) {
	return lua_tinker::dostring(m_masterLuaState, str, 0);
}

// luascriptmanager.cpp:132
lua_table_helper lua_script_manger::GetTable(const char *name) {
	lua_tinker::table table = lua_tinker::get<lua_tinker::table>(m_masterLuaState, name);

	return lua_table_helper(table);
}

}
