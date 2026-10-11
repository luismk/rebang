// Original: d:\build\custom\temp\code\shared\luasystem\lua_tinker\lua_tinker.cpp
#include "stdafx.h"
#include "lua_tinker.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// lua_tinker.cpp:23
void lua_tinker::init(lua_State *L)
{
	init_s64(L);
	init_u64(L);
}

// lua_tinker.cpp:32
static int tostring_s64(lua_State *L)
{
	char temp[64];
	sprintf(temp, "%I64d", *(__int64 *)lua_topointer(L, 1));
	lua_pushstring(L, temp);
	return 1;
}

// lua_tinker.cpp:41
static int eq_s64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(__int64)) == 0);
	return 1;
}

// lua_tinker.cpp:48
static int lt_s64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(__int64)) < 0);
	return 1;
}

// lua_tinker.cpp:55
static int le_s64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(__int64)) <= 0);
	return 1;
}

// lua_tinker.cpp:62
void lua_tinker::init_s64(lua_State *L)
{
	const char *name = "__s64";
	lua_pushstring(L, name);
	lua_newtable(L);

	lua_pushstring(L, "__name");
	lua_pushstring(L, name);
	lua_rawset(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcclosure(L, tostring_s64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__eq");
	lua_pushcclosure(L, eq_s64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__lt");
	lua_pushcclosure(L, lt_s64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__le");
	lua_pushcclosure(L, le_s64, 0);
	lua_rawset(L, -3);

	lua_settable(L, LUA_GLOBALSINDEX);
}

// lua_tinker.cpp:94
static int tostring_u64(lua_State *L)
{
	char temp[64];
	sprintf(temp, "%I64u", *(unsigned __int64 *)lua_topointer(L, 1));
	lua_pushstring(L, temp);
	return 1;
}

// lua_tinker.cpp:103
static int eq_u64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(unsigned __int64)) == 0);
	return 1;
}

// lua_tinker.cpp:110
static int lt_u64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(unsigned __int64)) < 0);
	return 1;
}

// lua_tinker.cpp:117
static int le_u64(lua_State *L)
{
	lua_pushboolean(L, memcmp(lua_topointer(L, 1), lua_topointer(L, 2), sizeof(unsigned __int64)) <= 0);
	return 1;
}

// lua_tinker.cpp:124
void lua_tinker::init_u64(lua_State *L)
{
	const char *name = "__u64";
	lua_pushstring(L, name);
	lua_newtable(L);

	lua_pushstring(L, "__name");
	lua_pushstring(L, name);
	lua_rawset(L, -3);

	lua_pushstring(L, "__tostring");
	lua_pushcclosure(L, tostring_u64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__eq");
	lua_pushcclosure(L, eq_u64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__lt");
	lua_pushcclosure(L, lt_u64, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__le");
	lua_pushcclosure(L, le_u64, 0);
	lua_rawset(L, -3);

	lua_settable(L, LUA_GLOBALSINDEX);
}

// lua_tinker.cpp:158
bool lua_tinker::dofile(lua_State *L, const char *filename)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);

	if (luaL_loadfile(L, filename) == 0) {
		lua_pcall(L, 0, 1, errfunc);
		lua_remove(L, errfunc);
		lua_pop(L, 1);

		return true;
	}
	else {
		print_error(L, "%s", lua_tostring(L, -1));
		lua_pcall(L, 0, 0, 1);
		lua_remove(L, errfunc);
		lua_pop(L, 1);

		return false;
	}
}

// lua_tinker.cpp:183
bool lua_tinker::dostring(lua_State *L, const char *buff, const char *name)
{
	return dobuffer(L, buff, strlen(buff), name);
}

// lua_tinker.cpp:189
bool lua_tinker::dobuffer(lua_State *L, const char *buff, size_t sz, const char *name)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);

	if (luaL_loadbuffer(L, buff, sz, name) == 0) {
		lua_pcall(L, 0, 1, errfunc);
		lua_remove(L, errfunc);
		lua_pop(L, 1);

		return true;
	}
	else {
		print_error(L, "%s", lua_tostring(L, -1));
		lua_pcall(L, 0, 0, 1);
		lua_remove(L, errfunc);
		lua_pop(L, 1);

		return false;
	}
}

// lua_tinker.cpp:271
static void call_stack(lua_State *L, int n)
{
	lua_Debug ar;
	if (lua_getstack(L, n, &ar) == 1) {
		lua_getinfo(L, "nSlu", &ar);

		if (n == 0) {
			lua_tinker::print_error(L, "\t<call stack>");
		}

		call_stack(L, n + 1);
	}
}

// lua_tinker.cpp:307
int lua_tinker::on_error(lua_State *L)
{
	print_error(L, "%s", lua_tostring(L, -1));

	call_stack(L, 0);

	return 0;
}

// lua_tinker.cpp:324
void lua_tinker::print_error(lua_State *L, const char *fmt, ...)
{
	char text[4096];

	va_list args;
	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);
}

// lua_tinker.cpp:368
void lua_tinker::enum_stack(lua_State *L)
{
	int top = lua_gettop(L);
	print_error(L, "Type:%d", top);
	for (int i = 1; i <= lua_gettop(L); ++i) {
		switch (lua_type(L, i)) {
		case LUA_TNIL:
			print_error(L, "\t%s", lua_typename(L, lua_type(L, i)));
			break;
		case LUA_TBOOLEAN:
			print_error(L, "\t%s\t%s", lua_typename(L, lua_type(L, i)), lua_toboolean(L, i) ? "true" : "false");
			break;
		case LUA_TLIGHTUSERDATA:
			print_error(L, "\t%s\t0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
			break;
		case LUA_TNUMBER:
			print_error(L, "\t%s\t%f", lua_typename(L, lua_type(L, i)), lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			print_error(L, "\t%s\t%s", lua_typename(L, lua_type(L, i)), lua_tostring(L, i));
			break;
		case LUA_TTABLE:
			print_error(L, "\t%s\t0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
			break;
		case LUA_TFUNCTION:
			print_error(L, "\t%s()\t0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
			break;
		case LUA_TUSERDATA:
			print_error(L, "\t%s\t0x%08p", lua_typename(L, lua_type(L, i)), lua_topointer(L, i));
			break;
		case LUA_TTHREAD:
			print_error(L, "\t%s", lua_typename(L, lua_type(L, i)));
			break;
		}
	}
}

// lua_tinker.cpp:410
template<>
char *lua_tinker::read(lua_State *L, int index)
{
	return (char *)lua_tostring(L, index);
}

// lua_tinker.cpp:416
template<>
const char *lua_tinker::read(lua_State *L, int index)
{
	return (const char *)lua_tostring(L, index);
}

// lua_tinker.cpp:422
template<>
char lua_tinker::read(lua_State *L, int index)
{
	return (char)lua_tonumber(L, index);
}

// lua_tinker.cpp:428
template<>
unsigned char lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (unsigned char)lua_tonumber(L, index);
}

// lua_tinker.cpp:440
template<>
short lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (short)lua_tonumber(L, index);
}

// lua_tinker.cpp:451
template<>
unsigned short lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (unsigned short)lua_tonumber(L, index);
}

// lua_tinker.cpp:462
template<>
long lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (long)lua_tonumber(L, index);
}

// lua_tinker.cpp:473
template<>
unsigned long lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (unsigned long)lua_tonumber(L, index);
}

// lua_tinker.cpp:484
template<>
int lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (int)lua_tonumber(L, index);
}

// lua_tinker.cpp:495
template<>
unsigned int lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (unsigned int)lua_tonumber(L, index);
}

// lua_tinker.cpp:506
template<>
float lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (float)lua_tonumber(L, index);
}

// lua_tinker.cpp:517
template<>
double lua_tinker::read(lua_State *L, int index)
{
	if (!lua_isnumber(L, index)) {
	}
	return (double)lua_tonumber(L, index);
}

// lua_tinker.cpp:527
template<>
bool lua_tinker::read(lua_State *L, int index)
{
	if (lua_isboolean(L, index))
		return lua_toboolean(L, index) != 0;
	else
		return lua_tonumber(L, index) != 0;
}

// lua_tinker.cpp:538
template<>
void lua_tinker::read(lua_State *L, int index)
{
	return;
}

// lua_tinker.cpp:545
template<>
__int64 lua_tinker::read(lua_State *L, int index)
{
	if (lua_isnumber(L, index))
		return (__int64)lua_tonumber(L, index);
	else
		return *(__int64 *)lua_touserdata(L, index);
}

// lua_tinker.cpp:553
template<>
unsigned __int64 lua_tinker::read(lua_State *L, int index)
{
	if (lua_isnumber(L, index))
		return (unsigned __int64)lua_tonumber(L, index);
	else
		return *(unsigned __int64 *)lua_touserdata(L, index);
}

// lua_tinker.cpp:562
template<>
lua_tinker::table lua_tinker::read(lua_State *L, int index)
{
	return table(L, index);
}

// lua_tinker.cpp:570
template<>
void lua_tinker::push(lua_State *L, char ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:577
template<>
void lua_tinker::push(lua_State *L, unsigned char ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:583
template<>
void lua_tinker::push(lua_State *L, short ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:589
template<>
void lua_tinker::push(lua_State *L, unsigned short ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:594
template<>
void lua_tinker::push(lua_State *L, long ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:600
template<>
void lua_tinker::push(lua_State *L, unsigned long ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:607
template<>
void lua_tinker::push(lua_State *L, int ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:613
template<>
void lua_tinker::push(lua_State *L, unsigned int ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:618
template<>
void lua_tinker::push(lua_State *L, float ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:624
template<>
void lua_tinker::push(lua_State *L, double ret)
{
	lua_pushnumber(L, ret);
}

// lua_tinker.cpp:630
template<>
void lua_tinker::push(lua_State *L, char *ret)
{
	lua_pushstring(L, ret);
}

// lua_tinker.cpp:636
template<>
void lua_tinker::push(lua_State *L, const char *ret)
{
	lua_pushstring(L, ret);
}

// lua_tinker.cpp:642
template<>
void lua_tinker::push(lua_State *L, bool ret)
{
	lua_pushboolean(L, ret);
}

// lua_tinker.cpp:649
template<>
void lua_tinker::push(lua_State *L, lua_value *ret)
{
	if (ret) ret->to_lua(L); else lua_pushnil(L);
}

// lua_tinker.cpp:655
template<>
void lua_tinker::push(lua_State *L, __int64 ret)
{
	*(__int64 *)lua_newuserdata(L, sizeof(__int64)) = ret;
	lua_pushstring(L, "__s64");
	lua_gettable(L, LUA_GLOBALSINDEX);
	lua_setmetatable(L, -2);
}

// lua_tinker.cpp:663
template<>
void lua_tinker::push(lua_State *L, unsigned __int64 ret)
{
	*(unsigned __int64 *)lua_newuserdata(L, sizeof(unsigned __int64)) = ret;
	lua_pushstring(L, "__u64");
	lua_gettable(L, LUA_GLOBALSINDEX);
	lua_setmetatable(L, -2);
}

// lua_tinker.cpp:672
template<>
void lua_tinker::push(lua_State *L, table ret)
{
	lua_pushvalue(L, ret.m_obj->m_index);
}

// lua_tinker.cpp:680
template<>
void lua_tinker::pop(lua_State *L)
{
	lua_pop(L, 1);
}

// lua_tinker.cpp:687
template<>
lua_tinker::table lua_tinker::pop(lua_State *L)
{
	return table(L, lua_gettop(L));
}

// lua_tinker.cpp:695
static void invoke_parent(lua_State *L)
{
	lua_pushstring(L, "__parent");
	lua_rawget(L, -2);
	if (lua_istable(L, -1)) {
		lua_pushvalue(L, 2);
		lua_rawget(L, -2);
		if (!lua_isnil(L, -1)) {
			lua_remove(L, -2);
		} else {
			lua_remove(L, -1);
			invoke_parent(L);
			lua_remove(L, -2);
		}
	}
}

// lua_tinker.cpp:717
int lua_tinker::meta_get(lua_State *L)
{
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);

	if (lua_isuserdata(L, -1)) {
		user2type<var_base *>::invoke(L, -1)->get(L);
		lua_remove(L, -2);
	} else if (lua_isnil(L, -1)) {
		lua_remove(L, -1);
		invoke_parent(L);
		if (lua_isnil(L, -1)) {
			lua_pushfstring(L, "can't find '%s' class variable. (forgot registering class variable ?)", lua_tostring(L, 2));
			lua_error(L);
		}
	}

	lua_remove(L, -2);

	return 1;
}

// lua_tinker.cpp:745
int lua_tinker::meta_set(lua_State *L)
{
	lua_getmetatable(L, 1);
	lua_pushvalue(L, 2);
	lua_rawget(L, -2);

	if (lua_isuserdata(L, -1)) {
		user2type<var_base *>::invoke(L, -1)->set(L);
	} else if (lua_isnil(L, -1)) {
		lua_pushvalue(L, 2);
		lua_pushvalue(L, 3);
		lua_rawset(L, -4);
	}
	lua_settop(L, 3);
	return 0;
}

// lua_tinker.cpp:765
void lua_tinker::meta_push(lua_State *L, const char *name)
{
	lua_pushstring(L, name);
	lua_gettable(L, LUA_GLOBALSINDEX);
}

// lua_tinker.cpp:779
lua_tinker::table_obj::table_obj(lua_State *L, int index)
	: m_L(L), m_index(index), m_ref(0), m_unused(0)
{
	if (lua_isnil(L, index)) {
		m_pointer = NULL; lua_remove(m_L, m_index);
	} else {
		m_pointer = lua_topointer(m_L, m_index);

		table_obj_manager::GetInstance().add_table_obj(this);
	}
}

// lua_tinker.cpp:796
lua_tinker::table_obj::~table_obj()
{
	if (validate()) {
		lua_remove(m_L, m_index);
	}
	table_obj_manager::GetInstance().delete_table_obj(this);
}

// lua_tinker.cpp:806
void lua_tinker::table_obj::inc_ref(void)
{
	++m_ref;
}

// lua_tinker.cpp:812
void lua_tinker::table_obj::dec_ref(void)
{
	if (--m_ref == 0) {
		delete this;
	}
}

// lua_tinker.cpp:818
bool lua_tinker::table_obj::validate()
{
	if (m_pointer != NULL) {
		if (m_pointer == lua_topointer(m_L, m_index)) {
			return true;
		}
		else {
			int top = lua_gettop(m_L);

			for (int i = 1; i <= top; ++i) {
				if (m_pointer == lua_topointer(m_L, i)) {
					m_index = i;
					return true;
				}
			}

			m_pointer = NULL;
			return false;
		}
	}
	else {
		return false;
	}
}

// lua_tinker.cpp:852
lua_tinker::table::table(lua_State *L)
{
	lua_newtable(L);

	m_obj = new table_obj(L, lua_gettop(L));

	m_obj->inc_ref();
}

// lua_tinker.cpp:861
lua_tinker::table::table(lua_State *L, const char *name)
{
	lua_pushstring(L, name);
	lua_gettable(L, LUA_GLOBALSINDEX);

	if (lua_istable(L, -1) == 0) {
		lua_pop(L, 1);

		lua_newtable(L);
		lua_pushstring(L, name);
		lua_pushvalue(L, -2);
		lua_settable(L, LUA_GLOBALSINDEX);
	}

	m_obj = new table_obj(L, lua_gettop(L));
}

// lua_tinker.cpp:879
lua_tinker::table::table(lua_State *L, int index)
{
	if (index < 0) {
		index = lua_gettop(L) + index + 1;
	}

	m_obj = new table_obj(L, index);

	m_obj->inc_ref();
}

// lua_tinker.cpp:890
lua_tinker::table::table(const table &input)
{
	m_obj = input.m_obj;

	m_obj->inc_ref();
}

// lua_tinker.cpp:898
lua_tinker::table::~table()
{
	m_obj->dec_ref();
}

// lua_tinker.cpp:904
table_obj_manager &table_obj_manager::GetInstance()
{
	static table_obj_manager instance;
	return instance;
}
