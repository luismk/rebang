#pragma once

#include "lua_tinker/lua_tinker.h"

namespace lua_system
{
	class lua_table_helper
	{
	public:
		lua_table_helper(lua_tinker::table value)
			: table(value), state(value.object->state)
		{
		}
		lua_table_helper(const lua_table_helper& value)
			: table(value.table), state(value.state)
		{
		}
		~lua_table_helper() { }

		lua_tinker::table table;
		lua_State* state;
	};
}
