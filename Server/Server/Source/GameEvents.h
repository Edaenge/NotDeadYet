#pragma once

#include <World/Observer.h>
#include <vector.h>
#include "ClientData.h"
#include "Actor.h"

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

	Player* player;
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
};

/*Event when a player wants to use an item.*/
class PlayerUseItemEvent : public Event
{
public:
	virtual ~PlayerUseItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
};

/*Event when a player wants to Fill an item.*/
class PlayerFillItemEvent : public Event
{
public:
	virtual ~PlayerFillItemEvent() {}

	ClientData* clientData;

	unsigned int itemID;
};

class PlayerCraftItemEvent : public Event
{
public:
	virtual ~PlayerCraftItemEvent() {}

	ClientData* clientData;

	unsigned int craftedItemType;
	unsigned int craftedItemSubType;
};

/*Event when a player wants to use his equipped weapon.*/
class PlayerUseEquippedWeaponEvent : public Event
{
public:
	virtual ~PlayerUseEquippedWeaponEvent() {}

	ClientData* clientData;
	unsigned int itemID;
};

class PlayerAnimalAttackEvent : public Event
{
public:
	virtual ~PlayerAnimalAttackEvent() {}
	ClientData* clientData;
	unsigned int mouseButton;
};

/*Event when a player swaps his equipped weapons between primary and secondary.*/
class PlayerSwapEquippedWeaponsEvent : public Event
{
public:
	virtual ~PlayerSwapEquippedWeaponsEvent() {}

	ClientData* clientdData;
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
	std::string playerName;
	Vector3 upVector;
	Vector3 direction;
};

/*The user has sent a ready event*/
class UserReadyEvent : public Event
{
public:
	virtual ~UserReadyEvent() {}

	ClientData* clientData;
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

//Temporary events
class PlayerKillEvent : public Event
{
public:
	virtual ~PlayerKillEvent() {}

	ClientData* clientData;
};
class PrintDebugDataEvent : public Event
{
public:
	virtual ~PrintDebugDataEvent() {}

	ClientData* clientData;
	int type;
};

class PlayerAnimalSwapEvent : public Event
{
public:
	virtual ~PlayerAnimalSwapEvent() {}

	ClientData* clientData;
	Actor* zActor;
	int zAnimalType;
};

class PlayerAnimalPossessEvent : public Event
{
public:
	virtual ~PlayerAnimalPossessEvent() {}

	ClientData* clientData;
	Actor* zActor;
	int zAnimalType;
};

class PlayerLeaveAnimalEvent : public Event
{
public:
	virtual ~PlayerLeaveAnimalEvent() {}

	ClientData* clientData;
};

class PlayerGhostMakesNoiseEvent : public Event
{
public:
	virtual ~PlayerGhostMakesNoiseEvent() {}

	ClientData* clientData;
	Actor* zActor;
};

class PlayerDeerEatObjectEvent : public Event
{
public:
	virtual ~PlayerDeerEatObjectEvent() {}

	ClientData* clientData;
	//ID's of all actors possibly hit by the deer
	std::vector<unsigned int> actorID;
};