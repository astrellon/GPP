#pragma once

#include <game/tile.h>
#include <game/game_object.h>
using namespace am::game;

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <vector>
using namespace std;

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class Inspector : public Panel {
	public:
		Inspector();
		~Inspector();

		void setTile(Tile *tile);
		Tile *getTile() const;

		void addGameObject(GameObject *obj);
		void addGameObjects(const ObjectList &list);
		void clearGameObjects();

		virtual TextField2 *getTextField();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		virtual void render(float dt);

	protected:

		Handle<Tile> mTile;
		
		typedef vector< Handle<GameObject> > GameObjectList;
		GameObjectList mGameObjects;

		Handle<TextField2> mInfo;

		bool mTextDirty;
		void updateText();
	};

}
}
