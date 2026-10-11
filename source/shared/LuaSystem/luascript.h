#ifndef PG_SHARED_LUASYSTEM_LUASCRIPT_H
#define PG_SHARED_LUASYSTEM_LUASCRIPT_H

#include <string>
  
#include "..\..\..\defines.h"
struct lua_State;

namespace lua_system {

class lua_script_manger;
class lua_table_helper;

// A Lua coroutine run by the script manager.
class lua_script {
	LAYOUT_CHECK_FRIEND;
	friend class lua_script_manger;
public:
	enum State {
		STATE_WAIT_FRAME = 0,
		STATE_WAIT_TIME = 1,
		STATE_RUNNING = 2,
		STATE_READY = 3,
		STATE_DELETE = 4
	};

protected:
	lua_script(lua_script_manger *manager, bool reserveDelete);
	~lua_script();

	// luascript.h:43
	void ReserveDelete() {
		m_reserveDelete = true;
	}

public:
	int Update(float elapsed);
	void WaitTimeScript(float time);
	void WaitFrameScript(int frame);
	bool RunFile(const char *filename);
	bool RunString(const char *str);
	void Reload();
	void AbortWait();
	lua_table_helper GetTable(const char *name);

private:
	void ResumeScript(float value);
	void FormatError();
	void OutputError(char *where);

	lua_script_manger *m_manager; // 0x00
	lua_State *m_luaState;        // 0x04
	int m_threadRef;              // 0x08
	int m_state;                  // 0x0C
	float m_elapsedTime;          // 0x10
	float m_waitTime;             // 0x14
	int m_waitFrame;              // 0x18
	char m_errorMessage[256];     // 0x1C
	std::string m_fileName;       // 0x11C
	std::string m_scriptString;   // 0x138
	bool m_reserveDelete;         // 0x154
};

}

CHECK_SIZE(lua_system::lua_script, 0x158);
BEGIN_LAYOUT_CHECK(lua_system::lua_script)
	CHECK_FIELD(lua_system::lua_script, m_manager, 0x00);
	CHECK_FIELD(lua_system::lua_script, m_luaState, 0x04);
	CHECK_FIELD(lua_system::lua_script, m_threadRef, 0x08);
	CHECK_FIELD(lua_system::lua_script, m_state, 0x0C);
	CHECK_FIELD(lua_system::lua_script, m_elapsedTime, 0x10);
	CHECK_FIELD(lua_system::lua_script, m_waitTime, 0x14);
	CHECK_FIELD(lua_system::lua_script, m_waitFrame, 0x18);
	CHECK_FIELD(lua_system::lua_script, m_errorMessage, 0x1C);
	CHECK_FIELD(lua_system::lua_script, m_fileName, 0x11C);
	CHECK_FIELD(lua_system::lua_script, m_scriptString, 0x138);
	CHECK_FIELD(lua_system::lua_script, m_reserveDelete, 0x154);
END_LAYOUT_CHECK

#endif
