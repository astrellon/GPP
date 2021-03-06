#pragma once

#include <map>
#include <string>
using namespace std;

//#include <util/json_value.h>
//using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileType {
	public:
		TileType(const char *name, const char *fullName = NULL);
		~TileType();

		const char *getName() const;

		void setFullName(const char *name);
		const char *getFullName() const;

		bool loadFromDef(LuaState &lua);
		
		static bool loadStandardTileTypesLua(const char *filename);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		string mName;
		string mFullName;

	};

}
}
