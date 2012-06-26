#include "engine.h"

#include "game.h"
#include "tile.h"
#include "json_value.h"
using namespace am::util;

#include <sstream>

#include "logger.h"

namespace am {
namespace base {

	Engine *Engine::sMainEngine = NULL;

	Engine::Engine() :
		mCurrentGame(NULL),
		mGridXSize(32.0f),
		mGridYSize(32.0f),
		mUsingTileSetDirty(true)
	{
	}
	Engine::~Engine() 
	{
		//deregisterTiles();
	}

	void Engine::init()
	{
		mTopLevelTileSet = new TileSet();
		addTileSet(mTopLevelTileSet.get());
	}
	void Engine::deinit()
	{

	}
	void Engine::update(float dt)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->update(dt);
		}
	}

	void Engine::setCurrentGame(Game *game)
	{
		mCurrentGame = game;
	}
	Game *Engine::getCurrentGame()
	{
		return mCurrentGame.get();
	}

	void Engine::usingTileSet(const char *tileSetName)
	{
		if (tileSetName)
		{
			string tileSetNameStr = tileSetName;
			if (mUsingTileSetNames.find(tileSetNameStr) == mUsingTileSetNames.end())
			{
				mUsingTileSetNames[tileSetNameStr] = 1;
				mUsingTileSetDirty = true;
			}
		}
	}
	void Engine::clearUsingTileSet()
	{
		mUsingTileSet.clear();
		mUsingTileSetNames.clear();
		mUsingTileSetDirty = false;
	}
	void Engine::checkUsingTileSet()
	{
		if (mUsingTileSetDirty)
		{
			UsingTileSet::iterator iter;
			TileSetMap::iterator tileIter;
			for (iter = mUsingTileSetNames.begin(); iter != mUsingTileSetNames.end(); ++iter)
			{
				tileIter = mTileSets.find(iter->first);
				if (tileIter != mTileSets.end())
				{
					mUsingTileSet[tileIter->first] = tileIter->second;
				}
			}
			mUsingTileSetNames.clear();
			mUsingTileSetDirty = false;
		}
	}

	Tile *Engine::getTile(const char *name)
	{
		string nameStr = name;
		string tileSetStr;
		int tileSetIndex = nameStr.find("/");
		bool useSpecificTileSet = false;
		if (tileSetIndex >= 0)
		{
			useSpecificTileSet = true;
			tileSetStr = nameStr.substr(0, tileSetIndex);
			tileSetIndex++;
			nameStr = nameStr.substr(tileSetIndex, nameStr.size() - tileSetIndex);
		}

		if (useSpecificTileSet)
		{
			Handle<TileSet> tileSet = mTopLevelTileSet;
			if (tileSetStr.size() > 0)
			{
				TileSetMap::iterator iter = mTileSets.find(tileSetStr);
				if (iter != mTileSets.end())
				{
					tileSet = iter->second;
				}
			}
			if (tileSet.get())
			{
				return tileSet->getTile(nameStr.c_str());
			}
			return NULL;
		}
		else
		{
			checkUsingTileSet();
			TileSetMap::iterator iter;
			Tile *foundTile = NULL;
			for (iter = mUsingTileSet.begin(); iter != mUsingTileSet.end(); ++iter)
			{
				foundTile = iter->second->getTile(nameStr.c_str());
				if (foundTile != NULL)
				{
					break;
				}
			}
			if (foundTile == NULL)
			{
				foundTile = mTopLevelTileSet->getTile(nameStr.c_str());
			}
			return foundTile;
		}

	}

	TileSet *Engine::getTileSet(const char *tileSetName)
	{
		TileSetMap::iterator iter = mTileSets.find(string(tileSetName));
		if (iter == mTileSets.end())
		{
			return NULL;
		}
		return iter->second.get();
	}
	void Engine::addTileSet(TileSet *tileSet)
	{
		if (tileSet)
		{
			mTileSets[tileSet->getName()] = tileSet;
		}
	}
	TileSetMap &Engine::getTileSets()
	{
		return mTileSets;
	}
	TileSet *Engine::getTopLevelTileSet()
	{
		return mTopLevelTileSet.get();
	}

	void Engine::setMainEngine(Engine *engine)
	{
		sMainEngine = engine;
	}
	Engine *Engine::getMainEngine()
	{
		return sMainEngine;
	}

	void Engine::setGridXSize(float size)
	{
		if (size > -0.0001f && size < 0.0f)
		{
			size = -0.0001f;
		}
		if (size < 0.0001f && size >= 0.0f)
		{
			size = 0.0001f;
		}
		mGridXSize = size;
	}
	float Engine::getGridXSize() const
	{
		return mGridXSize;
	}
	void Engine::setGridYSize(float size)
	{
		if (size > -0.0001f && size < 0.0f)
		{
			size = -0.0001f;
		}
		if (size < 0.0001f && size >= 0.0f)
		{
			size = 0.0001f;
		}
		mGridYSize = size;
	}
	float Engine::getGridYSize() const
	{
		return mGridYSize;
	}

}
}
