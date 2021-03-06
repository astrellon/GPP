#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace ui {
	class EventInterface;
}

namespace lua {
namespace ui {

	/*const char EventManager_tableName[] = "am_ui_EventManager";

	int EventManager_ctor(lua_State *lua);
	void EventManager_wrap(lua_State *lua, am::ui::EventInterface *manager);

	int EventManager_dtor(lua_State *lua);
	int EventManager_register(lua_State *lua);

	int EventManager_addEventListener(lua_State *lua);
	int EventManager_removeEventListener(lua_State *lua);
	int EventManager_hasEventListener(lua_State *lua);*/

	bool addEventListener(lua_State *lua, am::ui::EventInterface *manager);
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *manager);
	
	//am::ui::EventManager *Check_EventManager(lua_State *lua, int n);

}
}
}
