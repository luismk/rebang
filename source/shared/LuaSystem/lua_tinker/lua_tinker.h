#pragma once

extern "C"
{
#include "../Lua/src/lua.h"
#include "../Lua/src/lauxlib.h"
}

namespace lua_tinker
{
	struct table_obj
	{
		lua_State* state;
		int index;
		unsigned long unknown;
		int references;
	};

	struct table
	{
		table(const table&);
		~table();

		table_obj* object;
	};

	template <class T>
	T read(lua_State*, int);
	template <class T>
	void push(lua_State*, T);
	template <class T>
	T pop(lua_State*);
	template <>
	table pop<table>(lua_State*);

	template <class T>
	struct void2ptr
	{
		static T* invoke(void* value) { return (T*)value; }
	};

	template <class T>
	struct is_ptr
	{
		static const bool value = false;
	};
	template <class T>
	struct is_ptr<T*>
	{
		static const bool value = true;
	};

	template <class T>
	struct is_ref
	{
		static const bool value = false;
	};
	template <class T>
	struct is_ref<T&>
	{
		static const bool value = true;
	};

	template <class T>
	struct void2type;
	template <class T>
	struct void2type<T*>
	{
		static T* invoke(void* value) { return void2ptr<T>::invoke(value); }
	};

	template <class T>
	struct user2type
	{
		static T invoke(lua_State* state, int index)
		{
			if (is_ptr<T>::value && !is_ref<T>::value)
				return void2type<T>::invoke(lua_touserdata(state, index));
		}
	};

	template <class T>
	T upvalue_(lua_State* state)
	{
		return user2type<T>::invoke(state, lua_upvalueindex(1));
	}

	template <class P1 = void, class P2 = void, class P3 = void,
		class P4 = void, class P5 = void>
	struct functor;
	template <>
	struct functor<void, void, void, void, void>
	{
		template <class R>
		static int invoke(lua_State* state)
		{
			typedef R(__fastcall * Function)();
			Function fn = upvalue_<Function>(state);
			push<R>(state, fn());
			return 1;
		}
	};
	template <class P1>
	struct functor<P1, void, void, void, void>
	{
		template <class R>
		static int invoke(lua_State* state)
		{
			typedef R(__fastcall * Function)(P1);
			Function fn = upvalue_<Function>(state);
			push<R>(state, fn(read<P1>(state, 1)));
			return 1;
		}
		template <>
		static int invoke<void>(lua_State* state)
		{
			typedef void(__fastcall * Function)(P1);
			Function fn = upvalue_<Function>(state);
			fn(read<P1>(state, 1));
			return 0;
		}
	};
	template <class P1, class P2>
	struct functor<P1, P2, void, void, void>
	{
		template <class R>
		static int invoke(lua_State* state)
		{
			typedef R(__fastcall * Function)(P1, P2);
			Function fn = upvalue_<Function>(state);
			push<R>(state, fn(read<P1>(state, 1), read<P2>(state, 2)));
			return 1;
		}
	};

	template <class R>
	void push_functor(lua_State* state, R(__fastcall* fn)())
	{
		lua_pushcclosure(state, functor<>::invoke<R>, 1);
	}
	template <class R, class P1>
	void push_functor(lua_State* state, R(__fastcall* fn)(P1))
	{
		lua_pushcclosure(state, functor<P1>::invoke<R>, 1);
	}
	template <class R, class P1, class P2>
	void push_functor(lua_State* state, R(__fastcall* fn)(P1, P2))
	{
		lua_pushcclosure(state, functor<P1, P2>::invoke<R>, 1);
	}

	template <class F>
	void def(lua_State* state, const char* name, F fn)
	{
		lua_pushstring(state, name);
		lua_pushlightuserdata(state, (void*)fn);
		push_functor(state, fn);
		lua_settable(state, LUA_GLOBALSINDEX);
	}

	template <class T>
	T get(lua_State* state, const char* name)
	{
		lua_pushstring(state, name);
		lua_gettable(state, LUA_GLOBALSINDEX);
		return pop<T>(state);
	}

	bool dostring(lua_State* state, const char* text, const char* name);
}
