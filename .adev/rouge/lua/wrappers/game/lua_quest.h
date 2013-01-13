#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class Quest;
}

namespace lua {
namespace game {

	int Quest_ctor(lua_State *lua);
	int Quest_dtor(lua_State *lua);
	int Quest_eq(lua_State *lua);
	int Quest_register(lua_State *lua);

	int Quest_start_quest(lua_State *lua);
	int Quest_finish_quest(lua_State *lua);

	int Quest_set_complete(lua_State *lua);
	int Quest_is_completed(lua_State *lua);

	int Quest_set_title(lua_State *lua);
	int Quest_get_title(lua_State *lua);

	int Quest_set_description(lua_State *lua);
	int Quest_get_description(lua_State *lua);

	int Quest_set_active_text(lua_State *lua);
	int Quest_get_active_text(lua_State *lua);

	int Quest_add_event_listener(lua_State *lua);
	int Quest_remove_event_listener(lua_State *lua);
	int Quest_has_event_listener(lua_State *lua);

	int Quest_add_quest(lua_State *lua);
	int Quest_remove_quest(lua_State *lua);
	int Quest_get_quest(lua_State *lua);

}
}
}
