#pragma once

namespace am {
namespace base {

	class Tile;

	class TileInstance {
	public:
		TileInstance();
		TileInstance(Tile *tile);
		~TileInstance();
	
		Tile *getTile();
		void setTile(Tile *tile);
	
	protected:
		Tile *mTile;
	};

}
}
