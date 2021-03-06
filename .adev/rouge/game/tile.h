#pragma once

#include <string>
using namespace std;

#include <gfx/gfx_asset.h>
using namespace am::gfx;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

//#include <util/json_value.h>
//using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileSet;
	class TileType;
	
	class Tile : public IManaged {
	public:

		typedef vector<TileType *> TileTypeList;

		Tile(const char *name);
		Tile(const char *name, const char *fullName);
		~Tile();

		string getName() const;
		void setName(const char *name);
		bool setNameWithCheck(const char *name);

		string getFullName() const;
		void setFullName(const char *name);

		string getDescription() const;
		void setDescription(const char *description);

		Asset *getGraphicAsset();
		void setGraphicAsset(Asset *asset);

		void setTileSet(TileSet *tileSet);
		TileSet *getTileSet() const;

		void addTileType(TileType *tileType);
		void removeTileType(TileType *tileType);
		void removeAllTileTypes();
		bool hasTileType(TileType *tileType) const;
		TileTypeList &getTileTypes();
		const TileTypeList &getTileTypes() const;

		//void loadDef(JsonValue value);
		void loadDef(LuaState &lua);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
	
	protected:
	
		string mName;
		string mFullName;
		TileSet *mTileSet;
		Handle<Asset> mGraphic;
		string mDescription;

		TileTypeList mTileTypes;

	};

}
}
