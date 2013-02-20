#include "test_lua_dialogue.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <lua/wrappers/game/lua_dialogue.h>
using namespace am::lua::game;

#include <game/dialogue.h>
#include <game/engine.h>
#include <game/game.h>
using namespace am::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaDialogue::testSimple() {
		LuaState lua;

		Game *game = new Game();
		Engine::getEngine()->setCurrentGame(game);

		int loadResult = lua.loadString("Dialogue = import(\"Dialogue\")\n"
			"diag = Dialogue.new(\"diag1\", \"Hello there\")\n"
			"Dialogue.add_dialogue(diag)\n"
			"function getId()\n"
			"	return diag:id()\n"
			"end\n"
			"function setId(id)\n"
			"	diag:id(id)\n"
			"end\n"
			"function getId()\n"
			"	return diag:id()\n"
			"end\n"
			"function setText(text)\n"
			"	diag:text(text)\n"
			"end\n"
			"function getText()\n"
			"	return diag:text()\n"
			"end\n"
			"function setTitle(title)\n"
			"	diag:title(title)\n"
			"end\n"
			"function getTitle()\n"
			"	return diag:title()\n"
			"end\n"
			"function setSubject(subject)\n"
			"	diag:subject(subject)\n"
			"end\n"
			"function getSubject()\n"
			"	return diag:subject()\n"
			"end\n"
			"function setUnlockFlag(flag)\n"
			"	diag:unlock_flag(flag)\n"
			"end\n"
			"function getUnlockFlag()\n"
			"	return diag:unlock_flag()\n"
			"end\n"
			"function setDialogueAction(action)\n"
			"	diag:dialogue_action(action)\n"
			"end\n"
			"function getDialogueAction()\n"
			"	return diag:dialogue_action()\n"
			"end\n"
			);
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		Dialogue *diag1 = game->getDialogue("diag1");
		assert(diag1 != NULL);

		assert(lua.hasGlobalFunction("getId"));
		lua_acall(lua, 0, 1);
		equalsStr("diag1", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setId"));
		lua.push("notdiag1");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getId"));
		lua_acall(lua, 0, 1);
		equalsStr("notdiag1", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setId"));
		lua.push("diag1");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getText"));
		lua_acall(lua, 0, 1);
		equalsStr("Hello there", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setText"));
		lua.push("Hi");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getText"));
		lua_acall(lua, 0, 1);
		equalsStr("Hi", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getTitle"));
		lua_acall(lua, 0, 1);
		equalsStr("diag1", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setTitle"));
		lua.push("Greetings");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getTitle"));
		lua_acall(lua, 0, 1);
		equalsStr("Greetings", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getSubject"));
		lua_acall(lua, 0, 1);
		equalsStr("diag1", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setSubject"));
		lua.push("greet");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getSubject"));
		lua_acall(lua, 0, 1);
		equalsStr("greet", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getUnlockFlag"));
		lua_acall(lua, 0, 1);
		equalsStr("none", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setUnlockFlag"));
		lua.push("locked");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getUnlockFlag"));
		lua_acall(lua, 0, 1);
		equalsStr("locked", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getDialogueAction"));
		lua_acall(lua, 0, 1);
		equalsStr("none", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setDialogueAction"));
		lua.push("shop");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getDialogueAction"));
		lua_acall(lua, 0, 1);
		equalsStr("shop", lua_tostring(lua, -1));
		lua.pop(1);
		
		return true;
	}

}
}