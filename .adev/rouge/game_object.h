#pragma once

#include "gfx/gfx_layer.h"
using namespace am::gfx;

#include "handle.h"
using namespace am::util;

#include <vector>
using namespace std;

namespace am {
namespace base {

	class Screen;

	class GameObject : public Layer {
	public:
		GameObject();
		~GameObject();

		virtual void setLocation(float x, float y, bool setDraw = true);
		virtual float getLocationX() const;
		virtual float getLocationY() const;

		virtual void setGridLocation(int x, int y, bool setDraw = true);
		virtual int getGridLocationX() const;
		virtual int getGridLocationY() const;

		virtual void setFixedToGrid(bool fixed);
		virtual bool isFixedToGrid() const;

		virtual void setOnlyOnPassable(bool only);
		virtual bool isOnlyOnPassable() const;

		virtual void update(float dt) = 0;

		virtual void setCameraOffset(float x, float y);
		virtual float getCameraOffsetX() const;
		virtual float getCameraOffsetY() const;

		virtual void setScreen(Screen *screen);
		virtual Screen *getScreen();

	protected:

		bool mFixedToGrid;
		bool mOnlyOnPassable;

		float mLocationX;
		float mLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;

		Screen *mScreen;
	};

	typedef vector<Handle<GameObject> > ObjectList;

}
}
