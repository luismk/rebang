// Original: d:\build\custom\temp\code\shared\luasystem\lua_tinker\lua_tinker.h
#ifndef PG_SHARED_LUASYSTEM_LUA_TINKER_LUA_TINKER_H
#define PG_SHARED_LUASYSTEM_LUA_TINKER_LUA_TINKER_H

extern "C" {
#include "..\Lua\src\lua.h"
#include "..\Lua\src\lualib.h"
#include "..\Lua\src\lauxlib.h"
};

#include "..\..\..\defines.h"

#include <list>
#include <algorithm>

namespace lua_tinker {
	// init LuaTinker
	void init(lua_State *L);

	void init_s64(lua_State *L);
	void init_u64(lua_State *L);

	// metatable of the classes registered to Lua
	void meta_push(lua_State *L, const char *name);

	// run a chunk of Lua code
	bool dofile(lua_State *L, const char *filename);
	bool dostring(lua_State *L, const char *buff, const char *name);
	bool dobuffer(lua_State *L, const char *buff, size_t sz, const char *name);

	// class member access through the metatable
	int meta_get(lua_State *L);
	int meta_set(lua_State *L);

	// debug
	void enum_stack(lua_State *L);
	int on_error(lua_State *L);
	void __cdecl print_error(lua_State *L, const char *fmt, ...);

	// a value that pushes itself onto the Lua stack
	struct lua_value {
		virtual void to_lua(lua_State *L) = 0;
	};

	// a registered class member variable, stored as userdata in the metatable
	struct var_base {
		virtual void get(lua_State *L) = 0;
		virtual void set(lua_State *L) = 0;
	};

	// type traits
	template<bool C, typename A, typename B> struct if_ {};
	template<typename A, typename B> struct if_<true, A, B> { typedef A type; };
	template<typename A, typename B> struct if_<false, A, B> { typedef B type; };

	template<typename A> struct is_ptr { static const bool value = false; };
	template<typename A> struct is_ptr<A *> { static const bool value = true; };

	template<typename A> struct is_ref { static const bool value = false; };
	template<typename A> struct is_ref<A &> { static const bool value = true; };

	template<typename A> struct base_type { typedef A type; };
	template<typename A> struct base_type<A *> { typedef A type; };
	template<typename A> struct base_type<A &> { typedef A type; };

	// from void * to a value, a pointer or a reference
	template<typename T>
	struct void2val {
		static T invoke(void *input) {
			return *(T *)input;
		}
	};

	template<typename T>
	struct void2ptr {
		static T *invoke(void *input) {
			return (T *)input;
		}
	};

	template<typename T>
	struct void2ref {
		static T &invoke(void *input) {
			return *(T *)input;
		}
	};

	template<typename T>
	struct void2type {
		static T invoke(void *ptr) {
			return if_<is_ptr<T>::value
				, void2ptr<typename base_type<T>::type>
				, typename if_<is_ref<T>::value
					, void2ref<typename base_type<T>::type>
					, void2val<typename base_type<T>::type>
				>::type
			>::type::invoke(ptr);
		}
	};

	// from a Lua userdata to a C++ type
	template<typename T>
	struct user2type {
		static T invoke(lua_State *L, int index) {
			return void2type<T>::invoke(lua_touserdata(L, index));
		}
	};

	// read a value from the Lua stack
	template<typename T> T read(lua_State *L, int index);

	template<> char *read(lua_State *L, int index);
	template<> const char *read(lua_State *L, int index);
	template<> char read(lua_State *L, int index);
	template<> unsigned char read(lua_State *L, int index);
	template<> int read(lua_State *L, int index);
	template<> float read(lua_State *L, int index);
	template<> double read(lua_State *L, int index);
	template<> bool read(lua_State *L, int index);
	template<> void read(lua_State *L, int index);

	// push a value onto the Lua stack
	template<typename T> void push(lua_State *L, T ret);

	template<> void push(lua_State *L, char ret);
	template<> void push(lua_State *L, int ret);
	template<> void push(lua_State *L, long ret);
	template<> void push(lua_State *L, unsigned long ret);
	template<> void push(lua_State *L, float ret);
	template<> void push(lua_State *L, double ret);
	template<> void push(lua_State *L, char *ret);
	template<> void push(lua_State *L, const char *ret);
	template<> void push(lua_State *L, bool ret);

	// pop a value from the Lua stack
	template<typename T>
	T pop(lua_State *L) {
		T t = read<T>(L, -1);
		lua_pop(L, 1);
		return t;
	}

	template<> void pop(lua_State *L);

	// the function pointer stored as upvalue 1 of a C closure
	template<typename T>
	T upvalue_(lua_State *L) {
		return user2type<T>::invoke(L, lua_upvalueindex(1));
	}

	// C closures calling a registered function
	template<typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, typename T5 = void>
	struct functor {
		template<typename RVal>
		static int invoke(lua_State *L) {
			push(L, upvalue_<RVal (*)(T1, T2, T3, T4, T5)>(L)(read<T1>(L, 1), read<T2>(L, 2), read<T3>(L, 3), read<T4>(L, 4), read<T5>(L, 5)));
			return 1;
		}
		template<>
		static int invoke<void>(lua_State *L) {
			upvalue_<void (*)(T1, T2, T3, T4, T5)>(L)(read<T1>(L, 1), read<T2>(L, 2), read<T3>(L, 3), read<T4>(L, 4), read<T5>(L, 5));
			return 0;
		}
	};

	template<typename T1, typename T2>
	struct functor<T1, T2> {
		template<typename RVal>
		static int invoke(lua_State *L) {
			push(L, upvalue_<RVal (*)(T1, T2)>(L)(read<T1>(L, 1), read<T2>(L, 2)));
			return 1;
		}
		template<>
		static int invoke<void>(lua_State *L) {
			upvalue_<void (*)(T1, T2)>(L)(read<T1>(L, 1), read<T2>(L, 2));
			return 0;
		}
	};

	template<typename T1>
	struct functor<T1> {
		template<typename RVal>
		static int invoke(lua_State *L) {
			push(L, upvalue_<RVal (*)(T1)>(L)(read<T1>(L, 1)));
			return 1;
		}
		template<>
		static int invoke<void>(lua_State *L) {
			upvalue_<void (*)(T1)>(L)(read<T1>(L, 1));
			return 0;
		}
	};

	template<>
	struct functor<> {
		template<typename RVal>
		static int invoke(lua_State *L) {
			push(L, upvalue_<RVal (*)()>(L)());
			return 1;
		}
		template<>
		static int invoke<void>(lua_State *L) {
			upvalue_<void (*)()>(L)();
			return 0;
		}
	};

	// push a C closure calling func
	template<typename RVal>
	void push_functor(lua_State *L, RVal (*func)()) {
		lua_pushcclosure(L, functor<>::invoke<RVal>, 1);
	}

	template<typename RVal, typename T1>
	void push_functor(lua_State *L, RVal (*func)(T1)) {
		lua_pushcclosure(L, functor<T1>::invoke<RVal>, 1);
	}

	template<typename RVal, typename T1, typename T2>
	void push_functor(lua_State *L, RVal (*func)(T1, T2)) {
		lua_pushcclosure(L, functor<T1, T2>::invoke<RVal>, 1);
	}

	// register a global function
	template<typename F>
	void def(lua_State *L, const char *name, F func) {
		lua_pushstring(L, name);
		lua_pushlightuserdata(L, (void *)func);
		push_functor(L, func);
		lua_settable(L, LUA_GLOBALSINDEX);
	}

	// A Lua table kept alive by reference counting (shared by the
	// lua_tinker::table handles).
	struct table_obj {
		table_obj(lua_State *L, int index);
		~table_obj();

		void inc_ref(void);
		void dec_ref(void);

		bool validate(void);

		// lua_tinker.h:900
		template<typename T>
		T get(const char *name) {
			if (validate()) {
				lua_pushstring(m_L, name);
				lua_gettable(m_L, m_index);
			} else {
				lua_pushnil(m_L);
			}

			return pop<T>(m_L);
		}

		lua_State *m_L;          // 0x00
		int m_index;             // 0x04
		const void *m_pointer;   // 0x08
		int m_ref;               // 0x0C
		int m_unused;            // 0x10, zeroed by the constructor, never read
	};
	CHECK_SIZE(table_obj, 0x14);
	CHECK_FIELD(table_obj, m_L, 0x00);
	CHECK_FIELD(table_obj, m_index, 0x04);
	CHECK_FIELD(table_obj, m_pointer, 0x08);
	CHECK_FIELD(table_obj, m_ref, 0x0C);
	CHECK_FIELD(table_obj, m_unused, 0x10);

	// Table object on the Lua stack
	struct table {
		table(lua_State *L);
		table(lua_State *L, int index);
		table(lua_State *L, const char *name);
		table(const table &input);
		~table();

		// lua_tinker.h:1024
		template<typename T>
		T get(const char *name) {
			return m_obj->get<T>(name);
		}

		table_obj *m_obj;   // 0x00
	};
	CHECK_SIZE(table, 0x04);
	CHECK_FIELD(table, m_obj, 0x00);

	template<> table pop(lua_State *L);
	template<> void push(lua_State *L, table ret);

	// lua_tinker.h:630
	template<typename T>
	T get(lua_State *L, const char *name) {
		lua_pushstring(L, name);
		lua_gettable(L, LUA_GLOBALSINDEX);
		return pop<T>(L);
	}
}

// The live table_obj handles, in Lua stack order: removing one shifts the
// stack index of every handle above it.
class table_obj_manager {
public:
	static table_obj_manager &GetInstance();

	// lua_tinker.h:1070
	void add_table_obj(lua_tinker::table_obj *obj) {
		m_objs.push_back(obj);
	}

	// lua_tinker.h:1074
	void delete_table_obj(lua_tinker::table_obj *obj) {
		std::list<lua_tinker::table_obj *>::iterator it =
			std::find(m_objs.begin(), m_objs.end(), obj);
		for (; it != m_objs.end(); ++it) {
			--(*it)->m_index;
		}
		m_objs.remove(obj);
	}

	std::list<lua_tinker::table_obj *> m_objs; // 0x00
};
CHECK_SIZE(table_obj_manager, 0x0C);
CHECK_FIELD(table_obj_manager, m_objs, 0x00);

#endif
