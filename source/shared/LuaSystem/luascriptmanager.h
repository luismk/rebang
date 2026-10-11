#ifndef PG_SHARED_LUASYSTEM_LUASCRIPTMANAGER_H
#define PG_SHARED_LUASYSTEM_LUASCRIPTMANAGER_H

#include <list>
 
#include "..\..\..\defines.h"
struct lua_State;

namespace lua_system {

class lua_script;
class lua_table_helper;

class lua_script_manger {
	LAYOUT_CHECK_FRIEND;
public:
	lua_script_manger();
	~lua_script_manger();

	static lua_script_manger *GetInstance();

	lua_script *CreateScript(bool master);
	void ReleaseScript(lua_script *script);
	void ReloadAll();
	void Update(float elapsed);
	bool RunString(const char *str);
	lua_table_helper GetTable(const char *name);

	// luascriptmanager.h:30
	lua_State *GetMasterLuaState() {
		return m_masterLuaState;
	}

private:
	void DeleteScript(lua_script *script);

	std::list<lua_script *> m_scripts; // 0x00
	lua_State *m_masterLuaState;       // 0x0C
};

}

CHECK_SIZE(lua_system::lua_script_manger, 0x10);
BEGIN_LAYOUT_CHECK(lua_system::lua_script_manger)
	CHECK_FIELD(lua_system::lua_script_manger, m_scripts, 0x00);
	CHECK_FIELD(lua_system::lua_script_manger, m_masterLuaState, 0x0C);
END_LAYOUT_CHECK

#endif
