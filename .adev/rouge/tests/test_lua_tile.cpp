#include "test_lua_tile.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/tile.h>
#include <game/tile_type.h>
using namespace am::game;

#include <lua/wrappers/game/lua_tile_type.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaTile::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("Tile = import(\"Tile\")\n"
			"tile = Tile.new(\"testTile\")\n"
			"function getName()\n"
			"	return tile:get_name()\n"
			"end\n"
			"function setName(name)\n"
			"	return tile:set_name(name)\n"
			"end\n"
			"function getFullName()\n"
			"	return tile:get_full_name()\n"
			"end\n"
			"function setFullName(name)\n"
			"	tile:set_full_name(name)\n"
			"end\n"
			"function getDescription()\n"
			"	return tile:get_description()\n"
			"end\n"
			"function setDescription(desc)\n"
			"	tile:set_description(desc)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		equalsStr("testTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setName"));
		lua.push("newTestTile");
		lua.call(1, 0);

		assert(lua.hasGlobalFunction("getName"));
		lua.call(0, 1);
		equalsStr("newTestTile", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Full Name");
		lua.call(1, 0);
		
		assert(lua.hasGlobalFunction("getFullName"));
		lua.call(0, 1);
		equalsStr("New Full Name", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setDescription"));
		lua.push("New Description");
		lua.call(1, 0);

		assert(lua.hasGlobalFunction("getDescription"));
		lua.call(0, 1);
		equalsStr("New Description", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

	bool TestLuaTile::testTileTypes() {
		LuaState lua;
		
		assert(lua.loadString("Tile = import(\"Tile\")\n"
			"TileType = import(\"TileType\")\n"
			"tile = Tile.new(\"testTile2\")\n"
			"function addTileType(type)\n"
			"	tile:add_tile_type(type)\n"
			"end\n"
			"function removeTileType(type)\n"
			"	tile:remove_tile_type(type)\n"
			"end\n"
			"function removeAllTileTypes()\n"
			"	tile:remove_all_tile_types()\n"
			"end\n"
			"function hasTileType(type)\n"
			"	return tile:has_tile_type(type)\n"
			"end\n"
			));

		assert(lua.hasGlobalFunction("hasTileType"));

		TileType *type = new TileType("superType", "Super Type");
		TileType::addTileType(type);

		assert(lua.hasGlobalFunction("hasTileType"));
		TileType_wrap(lua, type);
		lua.call(1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua.call(1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTileType"));
		TileType_wrap(lua, type);
		lua.call(1, 0);
		
		assert(lua.hasGlobalFunction("hasTileType"));
		TileType_wrap(lua, type);
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeTileType"));
		lua.push("superType");
		lua.call(1, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua.call(1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("addTileType"));
		TileType_wrap(lua, type);
		lua.call(1, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua.call(1, 1);
		assert(lua_toboolean(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("removeAllTileTypes"));
		lua.call(0, 0);

		assert(lua.hasGlobalFunction("hasTileType"));
		lua.push("superType");
		lua.call(1, 1);
		assert(!lua_toboolean(lua, -1));
		lua.pop(1);

		return true;
	}

}
}