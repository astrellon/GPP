#include "character.h"

#include "engine.h"
#include "map.h"
#include "race.h"

#include <ui/equip_event.h>

#include <sstream>
using namespace std;

#include <log/logger.h>

namespace am {
namespace game {

	Character::Character() :
		GameObject(),
		mGraphic(NULL),
		mController(NULL),
		mMoveX(0),
		mMoveY(0),
		mAge(1.0f),
		mGender(Gender::MALE),
		mRace(NULL)
	{
		setName("Character");
		mPickupReach = Engine::getEngine()->getGridXSize() * 1.5f;

		addEventListener(MOUSE_UP, this);

		mInventory = new Inventory(10, 6);
	}
	Character::~Character()
	{

	}

	void Character::setGraphic(Sprite *graphic)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		if (graphic)
		{
			addChild(graphic);
			// Aim for head-ish area.
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.3f;
		}
	}
	Sprite *Character::getGraphic()
	{
		return mGraphic.get();
	}

	void Character::update(float dt)
	{
		if (mController.get())
		{
			mController->update(this, dt);
		}
		if (mMoveX != 0 || mMoveY != 0)
		{
			Engine *engine = Engine::getEngine();
			moveGrid(mMoveX, mMoveY);
			//float dx = engine->getGridXSize() * static_cast<float>(mMoveX);
			//float dy = engine->getGridYSize() * static_cast<float>(mMoveY);

			//move(dx, dy);
		}
	}

	void Character::setController(IController *controller)
	{
		mController = controller;
	}
	IController *Character::getController()
	{
		return mController.get();
	}
	/*
	void Character::move(float x, float y)
	{
		//int gridX = static_cast<int>((mLocationX + x) * Engine::getEngine()->getGridXSizeResp());
		//int gridY = static_cast<int>((mLocationY + y) * Engine::getEngine()->getGridXSizeResp());
		setLocation(mLocationX + x, mLocationY + y);
	}
	*/
	void Character::setMoveVector(int x, int y)
	{
		mMoveX = x;
		mMoveY = y;
	}
	int Character::getMoveVectorX() const
	{
		return mMoveX;
	}
	int Character::getMoveVectorY() const
	{
		return mMoveY;
	}

	void Character::setPickupReach(float reach)
	{
		if (reach < 0.0f) {
			reach = -reach;
		}
		mPickupReach = reach;
	}
	float Character::getPickupReach() const
	{
		return mPickupReach;
	}

	float Character::getWidth()
	{
		return mGraphic->getWidth();
	}
	float Character::getHeight()
	{
		return mGraphic->getHeight();
	}

	Stats &Character::getStats()
	{
		return mStats;
	}

	bool Character::addBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return false;
		}
		string name = part->getName();
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			Item *equipped = part->getEqippedItem();
			if (equipped != NULL)
			{
				_equipItem(equipped, name.c_str());
			}
			mBodyParts[name] = part;
			return true;
		}
		stringstream ss;
		ss << "Body part '" << name << "' already on character '" << getName() << "'";
		am_log("CHAR", ss);
		return false;
	}
	bool Character::removeBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return false;
		}
		string name = part->getName();
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter != mBodyParts.end())
		{
			Item *equipped = iter->second->getEqippedItem();
			if (equipped != NULL)
			{
				//mStats.removeModifiers(equipped->getStatModifiers());
				_unequipItem(equipped, name.c_str());
			}
			mBodyParts.erase(iter);
			return true;
		}
		return false;
	}
	bool Character::hasBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return false;
		}
		string name = part->getName();
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			return false;
		}
		return true;
	}
	const BodyPart::BodyPartMap &Character::getBodyParts() const
	{
		return mBodyParts;
	}
	
	bool Character::equipItem(Item *item, const char *bodyPart)
	{
		if (item == NULL || bodyPart == NULL || bodyPart[0] == '\0')
		{
			return false;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			stringstream ss;
			ss << "Cannot equip item '" << item->getFullItemName() << "' on to '";
			ss << getName() << "' because they do not have a '" << name << "'";
			am_log("CHAR", ss);
			return false;
		}
		Item *alreadyEquipped = iter->second->getEqippedItem();
		if (alreadyEquipped == NULL)
		{
			iter->second->setEquippedItem(item);
			_equipItem(item, bodyPart);
			return true;
		}
		stringstream ss;
		ss << "'" << getName() << "' already has '" << alreadyEquipped->getFullItemName();
		ss << "' equipped on " << name;
		am_log("CHAR", ss);
		return false;
	}
	bool Character::unequipItem(const char *bodyPart)
	{
		if (bodyPart == NULL || bodyPart[0] == '\0')
		{
			return false;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			return false;
		}
		Item *equipped = iter->second->getEqippedItem();
		if (equipped != NULL)
		{
			_unequipItem(equipped, bodyPart);
		}
		iter->second->setEquippedItem(NULL);
		return true;
	}
	Item *Character::getEquipped(const char *bodyPart) const
	{
		if (bodyPart == NULL || bodyPart[0] == '\0')
		{
			return NULL;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			return NULL;
		}
		return iter->second->getEqippedItem();
	}

	Inventory *Character::getInventory()
	{
		return mInventory;
	}

	bool Character::pickupItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}
		if (item->getItemLocation() == Item::GROUND)
		{
			float dx = item->getLocationX() - getLocationX();
			float dy = item->getLocationY() - getLocationY();
			if (dx > mPickupReach || dx < -mPickupReach || dy > mPickupReach || dy < -mPickupReach)
			{
				// Too far away.
				return false;
			}
		}
		// There may not be space so this can still return false.
		return addItem(item);
	}
	bool Character::addItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->addItem(item);
	}
	bool Character::removeItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->removeItem(item);
	}
	bool Character::hasItem(const Item *item) const
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->hasItem(item);
	}
	bool Character::dropItem(Item *item, float x, float y)
	{
		if (item == NULL || mMap == NULL ||
			x < 0 || y < 0 ||
			x >= mMap->getWidth() ||
			y >= mMap->getHeight())
		{
			am_log("CHAR", "Off map");
			return false;
		}

		float dx = getLocationX() - x;
		float dy = getLocationY() - y;
		if (dx < -mPickupReach || dx > mPickupReach || dy < -mPickupReach || dy > mPickupReach)
		{
			am_log("CHAR", "Too far away");
			return false;
		}

		//if (!mMap->isValidGridLocation(gridX, gridY, item))
		if (!mMap->isValidLocation(x, y, item))
		{
			am_log("CHAR", "Invalid location for item");
			return false;
		}
		//item->setGridLocation(gridX, gridY);
		item->setLocation(x, y);
		mMap->addGameObject(item);
		item->setItemLocation(Item::GROUND);

		mInventory->removeItem(item);
		return true;
	}

	const char *Character::getGameObjectTypeName() const
	{
		return "character";
	}

	void Character::getSelector(Selector &selector) const
	{
		selector.setId(mGameId.c_str());
		selector.setAttribute("age", mAge);
		selector.setAttribute("gender", Gender::getGenderName(mGender));
		if (mRace != NULL)
		{
			selector.setAttribute("race", mRace->getRaceName());
		}
	}

	void Character::setAge(float age)
	{
		mAge = age;
	}
	float Character::getAge() const
	{
		return mAge;
	}

	void Character::setRace(Race *race)
	{
		mRace = race;
	}
	Race *Character::getRace() const
	{
		return mRace;
	}

	void Character::setGender(Gender::GenderType gender)
	{
		mGender = gender;
	}
	Gender::GenderType Character::getGender() const
	{
		return mGender;
	}

	void Character::_equipItem(Item *item, const char *bodyPartName)
	{
		item->setItemLocation(Item::INVENTORY);
		mStats.addModifiers(item->getStatModifiers());
		fireEvent<EquipEvent>(new EquipEvent("equip", this, bodyPartName, item));
	}
	void Character::_unequipItem(Item *item, const char *bodyPartName)
	{
		mStats.removeModifiers(item->getStatModifiers());
		fireEvent<EquipEvent>(new EquipEvent("unequip", this, bodyPartName, item));
	}

}
}
