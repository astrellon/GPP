#include "unit_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>

#include <tests/test_base.h>
#include <tests/test_mouse_manager.h>
#include <tests/test_map.h>
#include <tests/test_character.h>
#include <tests/test_selector.h>
#include <tests/test_selector_value.h>
#include <tests/test_json_value.h>
#include <tests/test_lua.h>
#include <tests/test_lua_quest.h>
#include <tests/test_lua_event_listener.h>
#include <tests/test_lua_character.h>
#include <tests/test_lua_stats.h>
#include <tests/test_lua_stat_modifiers.h>
#include <tests/test_lua_item.h>
#include <tests/test_lua_inventory.h>
#include <tests/test_lua_tile_type.h>
#include <tests/test_lua_tile.h>
#include <tests/test_lua_tile_set.h>
#include <tests/test_lua_engine.h>
using namespace am::tests;

namespace am {
namespace sys {

	UnitTestSystem *UnitTestSystem::sUnitTestSystem = NULL;

	UnitTestSystem *UnitTestSystem::createUnitTestSystem(ISystem *linked, Engine *engine)
	{
		sGameSystem = sUnitTestSystem = new UnitTestSystem(linked, engine);
		return sUnitTestSystem;
	}
	UnitTestSystem *UnitTestSystem::getUnitTestSystem()
	{
		return sUnitTestSystem;
	}

	UnitTestSystem::UnitTestSystem(ISystem *linked, Engine *engine) :
		GameSystem(linked, engine)
	{

	}
	UnitTestSystem::~UnitTestSystem()
	{
	}

	void UnitTestSystem::init()
	{
		GameSystem::init();
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());

		mDebugConsole->setVisible(true);

		runSuite(TestBase);
		runSuite(TestMouseManager);
		runSuite(TestMap);
		runSuite(TestCharacter);
		runSuite(TestSelector);
		runSuite(TestSelectorValue);
		runSuite(TestJsonValue);
		runSuite(TestLua);
		runSuite(TestLuaQuest);
		runSuite(TestLuaEventListener);
		runSuite(TestLuaCharacter);
		runSuite(TestLuaStats);
		runSuite(TestLuaStatModifiers);
		runSuite(TestLuaItem);
		runSuite(TestLuaInventory);
		runSuite(TestLuaTileType);
		runSuite(TestLuaTile);
		runSuite(TestLuaTileSet);
		runSuite(TestLuaEngine);
	}
	
	void UnitTestSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		float fwidth = static_cast<float>(width);
		float fheight = static_cast<float>(height);
	}

	void UnitTestSystem::onKeyUp(int key)
	{
		GameSystem::onKeyUp(key);
	}

}
}
