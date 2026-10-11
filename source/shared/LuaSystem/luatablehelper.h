// Original: d:\build\custom\temp\code\shared\luasystem\luatablehelper.h
#ifndef PG_SHARED_LUASYSTEM_LUATABLEHELPER_H
#define PG_SHARED_LUASYSTEM_LUATABLEHELPER_H

#include "lua_tinker/lua_tinker.h"

#include "..\..\..\defines.h"
namespace lua_system {

// A Lua table handle with the state it lives in.
class lua_table_helper {
	LAYOUT_CHECK_FRIEND;
public:
	// luatablehelper.h:25
	lua_table_helper(lua_tinker::table table) : m_table(table), m_L(table.m_obj->m_L) {}

	// luatablehelper.h:28
	lua_table_helper(const lua_table_helper &other) : m_table(other.m_table), m_L(other.m_L) {}
	// luatablehelper.h:29
	~lua_table_helper() {}

	// luatablehelper.h:50
	template<typename T>
	T GetValueByName(const char *name) {
		return m_table.get<T>(name);
	}

private:
	lua_tinker::table m_table; // 0x00
	lua_State *m_L;            // 0x04
};

}

CHECK_SIZE(lua_system::lua_table_helper, 0x08);
BEGIN_LAYOUT_CHECK(lua_system::lua_table_helper)
	CHECK_FIELD(lua_system::lua_table_helper, m_table, 0x00);
	CHECK_FIELD(lua_system::lua_table_helper, m_L, 0x04);
END_LAYOUT_CHECK

#endif
