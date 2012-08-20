#pragma once

#include "event.h"
#include "mouse_event.h"
#include "keyboard_event.h"
#include "data_event.h"
#include "inventory_event.h"
#include "equip_event.h"

namespace am {
namespace ui {

	class IEventListener {
	public:
		virtual void onEvent(Event *e) {}
		virtual void onEvent(MouseEvent *e) {}
		virtual void onEvent(KeyboardEvent *e) {}
		virtual void onEvent(DataEvent *e) {}
		virtual void onEvent(InventoryEvent *e) {}
		virtual void onEvent(EquipEvent *e) {}
	};

}
}
