#ifndef PG_SHARED_LUASYSTEM_LUAUTILITYLIB_H
#define PG_SHARED_LUASYSTEM_LUAUTILITYLIB_H

#include <map>
#include <string>
#include <vector>

#include "..\..\..\defines.h"
struct lua_State;
struct lua_Debug;

namespace lua_system {

void LuaOpenUtilityLib(lua_State *L);
int lua_dofile_for_projectG(lua_State *L);
void print_debug_local_value(lua_State *L, const lua_Debug *ar);

// The text of every loaded Lua file, split into lines, for the debug traces.
class lua_file_line_text_manager {
	LAYOUT_CHECK_FRIEND;
public:
	lua_file_line_text_manager() {}
	~lua_file_line_text_manager() {}

	static lua_file_line_text_manager *get_instance();

	const char *get_line_text(const char *filename, int line);

protected:
	bool is_loaded_lua_file(const char *filename);
	void road_lua_file(const char *filename);
	void road_lua_buffer(const char *filename, const char *buffer);

	// File name -> its lines.
	std::map<std::string, std::vector<std::string> > m_files; // 0x0
};

}

CHECK_SIZE(lua_system::lua_file_line_text_manager, 0xc);
BEGIN_LAYOUT_CHECK(lua_system::lua_file_line_text_manager)
	CHECK_FIELD(lua_system::lua_file_line_text_manager, m_files, 0x0);
END_LAYOUT_CHECK

#endif
