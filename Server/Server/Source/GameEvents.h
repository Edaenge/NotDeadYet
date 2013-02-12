#pragma once

#include <Observer.h>
#include <vector.h>
#include "ClientData.h"

/*Event when a player has connected.*/
class PlayerConnectedEvent : public Event
{
public:
	virtual ~PlayerConnectedEvent() {}

	ClientData* clientData;
};

/*Event when a player have been created in game.*/
class PlayerAddEvent : public Event
{
public:
	virtual ~PlayerAddEvent() {}

	Player* player;
};

/*Event when a player has disconnected.*/
class PlayerDisconnectedEvent : public Event
{
public:
	virtual ~PlayerDisconnectedEvent() {}

	ClientData* clientData;
};
/*Event when a player have been removed in game.*/
class PlayerRemoveEvent : public Event
{
public:
	virtual ~PlayerRemoveEvent() {}

	Player* player;
};
/*Event when a player has died.*/
class PlayerDiedEvent : public Event
{
public:
	virtual ~PlayerDiedEvent() {}

	ClientData* clientData;
};

/*Event when a player is ready to play.*/
class PlayerReadyEvent : public Event
{
public:
	virtual ~PlayerReadyEvent() {}

	ClientData* clientData;
};

/*Event when a player wants to pickup an item*/
class PlayerPickupObjectEvent : public Event
{
public:
	virtual ~PlayerPickupObjectEvent() {}
	
	ClientData* clientData;

	long objID;
};

class PlayerLootObjectEvent : public Event
{
public:
	virtual ~PlayerLootObjectEvent() {}

	ClientData* clientData;

	std::vector<unsigned int> actorID;
};
/*Event when a player wants to loot items from a loot able object.*/
class PlayerLootItemEvent : public Event
{
public:
	virtual ~PlayerLootItemEvent() {}

	ClientData* clientData;

	unsigned int objID;
	unsigned int itemID;
	unsigned int itemType;
	unsigned int subType;
	
};

/*Event when a player wants to drop an item.*/
class PlayerDropItemEvent : public Event
{
public:
	virtual ~PlayerDropItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
};

/*Event when a player wants to equip an item.*/
class PlayerEquipItemEvent : public Event
{
public:
	virtual ~PlayerEquipItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
};

/*Event when a player wants to unequip an item.*/
class PlayerUnEquipItemEvent : public Event
{
public:
	virtual ~PlayerUnEquipItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
	int eq_Slot;
};

/*Event when a player wants to use an item.*/
class PlayerUseItemEvent : public Event
{
public:
	virtual ~PlayerUseItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
};

/*Event when a player wants to use his equipped weapon.*/
class PlayerUseEquippedWeaponEvent : public Event
{
public:
	virtual ~PlayerUseEquippedWeaponEvent() {}

	ClientData* clientData;
	unsigned int itemID;
};

/*Event when a client sends an update to server.*/
class ClientDataEvent : public Event
{
public:
	virtual ~ClientDataEvent() {}

	ClientData* clientData;
	
	Vector3 upVector;
	Vector3 direction;
	Vector4 rotation;
};

/*Event when a client sends an information to server.
  This info is send when the client connects.
*/
class UserDataEvent : public Event
{
public:
	virtual ~UserDataEvent() {}
	
	ClientData* clientData;

	std::string playerModel;
	Vector3 upVector;
	Vector3 direction;
};

/*Client hold a key down.*/
class KeyDownEvent : public Event
{
public:
	virtual ~KeyDownEvent() {}
	
	ClientData* clientData;

	int key;
};

/*Client release a key.*/
class KeyUpEvent : public Event
{
public:
	virtual ~KeyUpEvent() {}

	ClientData* clientData;

	int key;
};