#pragma once

#include <string>
#include <map>
using namespace std;

#include "handle.h"
#include "imanaged.h"
#include "json_value.h"
using namespace am::util;

#include "tile.h"

namespace am {
namespace base {

	class TileSet : public IManaged {
	public:
		TileSet();
		TileSet(const char *name);
		~TileSet();

		void setName(const char *name);
		string getName();

		void addTile(Tile *tile);
		void removeTile(Tile *tile);
		void removeTile(const char *tileName);
		bool hasTile(Tile *tile);
		bool hasTile(const char *tileName);
		Tile *getTile(const char *tileName);

		void loadDef(JsonValue value);

	protected:

		typedef map<string, Handle<Tile> > TileMap;
		TileMap mTiles;
		
		string mName;
	};

}
}
