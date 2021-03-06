#pragma once

#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <base/handle.h>
using namespace am::base;

#include <vector>
#include <ostream>
using namespace std;

#include <game/dialogue.h>
#include <game/tile_type.h>
#include <game/dialogue_component.h>
#include <game/iattribute_data.h>

#include <sfx/sfx_isource.h>
using namespace am::sfx;

namespace am {
namespace util {
namespace data {
	class IData;
	class Table;
}
}
using namespace am::util;

namespace game {

	class Map;
	class LoadingState;

	class GameObject : public Layer, public IAttributeData {
	public:

		typedef vector<TileType *> PassibleTypeList;
		typedef map<string, bool> SubjectMap;

		GameObject();
		GameObject(const GameObject &rhs);
		~GameObject();

		virtual void setName(const char *name);
		virtual void setName(const string &name);
		virtual string getName() const;

		virtual void setLocation(float x, float y, bool setDraw = true);
		virtual float getLocationX() const;
		virtual float getLocationY() const;

		virtual bool setGridLocation(int x, int y, bool setDraw = true);
		virtual int getGridLocationX() const;
		virtual int getGridLocationY() const;

		virtual bool setGridLocationF(float x, float y, bool setDraw = true);
		virtual float getGridLocationXF() const;
		virtual float getGridLocationYF() const;

		virtual void setDialogueComp(DialogueComponent *comp, bool setAttached = true);
		virtual DialogueComponent *getDialogueComp() const;

		virtual bool move(float x, float y);
		virtual bool moveGrid(int x, int y);

		virtual void setFixedToGrid(bool fixed);
		virtual bool isFixedToGrid() const;

		virtual void setOnlyOnPassable(bool only);
		virtual bool isOnlyOnPassable() const;

		// The update function relates to changes in real time, the dt
		// value will be in milliseconds since the last update.
		virtual void update(float dt);
		// The onGameTick function relates to one in-game tick.
		// If game tick returns true then it is okay for the game to
		// move onto the next game object.
		// Otherwise it will wait until it is told that it is the next
		// game objects game tick.
		virtual bool onGameTick(float dt);

		virtual void setCameraOffset(float x, float y);
		virtual float getCameraOffsetX() const;
		virtual float getCameraOffsetY() const;

		virtual void setMap(Map *map);
		virtual Map *getMap() const;

		virtual void setOriginalMap(Map *map);
		virtual Map *getOriginalMap() const;

		virtual void addPassibleType(TileType *tileType);
		virtual void removePassibleType(TileType *tileType);
		virtual void removeAllPassibleTypes();
		virtual bool hasPassibleType(TileType *tileType) const;
		virtual PassibleTypeList &getPassibleTypes();
		virtual const PassibleTypeList &getPassibleTypes() const;

		virtual const char *getGameObjectTypeName() const;

		virtual bool setGameId(const char *id);
		virtual const char *getGameId() const;
		virtual void registerSelf(const char *id);

		virtual ISource *getSource(bool create = true);
		virtual void setSource(ISource *source);

		virtual float distanceTo(GameObject *obj) const;
		virtual float distanceToGrid(GameObject *obj) const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mGameId;

		bool mFixedToGrid;
		bool mOnlyOnPassable;

		float mLocationX;
		float mLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;

		PassibleTypeList mPassibleTypes;

		string mName;

		Map *mMap;
		Map *mOriginalMap;

		Handle<DialogueComponent> mDialogueComp;
		Handle<ISource> mSoundSource;

		void applyToSource();

		virtual void postRender(float dt);

	};

	typedef vector<Handle<GameObject> > ObjectList;

}
}
