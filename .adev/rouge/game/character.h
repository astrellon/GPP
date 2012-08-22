#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include "icontroller.h"
#include "game_object.h"
#include "body_part.h"

#include "stats.h"

namespace am {
namespace game {

	class Character : public IEventListener, public GameObject {
	public:
		Character();
		~Character();

		void setGraphic(Sprite *graphic);
		Sprite *getGraphic();

		virtual void update(float dt);

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setMoveVector(int x, int y);
		virtual int getMoveVectorX() const;
		virtual int getMoveVectorY() const;

		virtual void setPickupReach(float reach);
		virtual float getPickupReach() const;

		virtual float getWidth();
		virtual float getHeight();

		virtual Stats &getStats();

		virtual bool addBodyPart(BodyPart *part);
		virtual bool removeBodyPart(BodyPart *part);
		virtual bool hasBodyPart(BodyPart *part);
		virtual const BodyPart::BodyPartMap &getBodyParts() const;

		virtual bool equipItem(Item *item, const char *bodyPart);
		virtual bool unequipItem(const char *bodyPart);
		virtual Item *getEquipped(const char *bodyPart) const;
		
		virtual Inventory *getInventory();

		virtual bool pickupItem(Item *item);
		virtual bool addItem(Item *item);
		virtual bool removeItem(Item *item);
		virtual bool hasItem(const Item *item) const;
		virtual bool dropItem(Item *item, float x, float y);

		virtual const char *getGameObjectTypeName() const;

	protected:

		Handle<IController> mController;

		int mMoveX;
		int mMoveY;

		float mPickupReach;

		BodyPart::BodyPartMap mBodyParts;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		Handle<Inventory> mInventory;

		Stats mStats;

		void _equipItem(Item *item, const char *bodyPartName);
		void _unequipItem(Item *item, const char *bodyPartName);
	};

}
}