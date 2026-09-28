#pragma once

#include <list>
#include "lua_tinker/lua_tinker.h"

namespace lua_system
{
	class lua_script;
	class lua_table_helper;

	class lua_script_manger
	{
	public:
		lua_script_manger();
		~lua_script_manger();

		static lua_script_manger* GetInstance();
		lua_script* CreateScript(bool option);
		void ReleaseScript(lua_script* value);
		void Update(float delta);
		void ReloadAll();
		bool RunString(const char* text);
		lua_table_helper GetTable(const char* name);
		lua_State* GetMasterLuaState() { return master; }

	private:
		void DeleteScript(lua_script* value);

		std::list<lua_script*> scripts;
		lua_State* master;
	};
}
