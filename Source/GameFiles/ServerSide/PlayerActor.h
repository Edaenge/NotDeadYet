/*
Made by Edänge Simon Datum(30/11/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/BioActor.h"
#include "GameFiles/ServerSide/ObjectManager.h"
#include "GameFiles/ServerSide/DynamicObjectActor.h"
#include "GameFiles/KeyUtil/KeyValues.h"
#include "GameFiles/KeyUtil/KeyStates.h"
#include "Items/Inventory.h"


/*This class is used to save player information such as position and states.
  This information is sent to clients.
*/
class PlayerActor : public BioActor
{
public:
	/*! ID should be the same ID as in ClientChannel.*/
	PlayerActor(const int ID);
	/*! ID should be the same ID as in ClientChannel.*/
	PlayerActor(const int ID, const Vector3& startPos);
	/*! ID should be the same ID as in ClientChannel.*/
	PlayerActor(const int ID, const Vector3& startPos, const Vector4& startRot);

	virtual ~PlayerActor();

	/*! Updates players pos, states etc.*/
	void Update(float deltaTime);
	/*! Picks up an object and transform it to an item and adds it to the inventory.
		Genuine actors: StaticObjectActor, DynamicObjectActor.
		This function DO NOT deallocates the Actor object.
	*/
	bool PickUpObject(StaticObjectActor* object);
	bool PickUpObject(DynamicObjectActor* object);
	/*! */
	bool DropObject(const int ID);

	float GetLatency() const {return this->zLatency;}
	inline float GetFrameTime() const {return this->zFrameTime;}
	/*! Gets the current key state. This function is used
		to see which buttons are pressed right now.
		Key is an enum defined in header KeyValues.
	*/
	inline bool GetkeyState(const unsigned int key)
	{return zKeyStates.GetKeyState(key);}

	/* ! Sets key states.
		This one is used to define which buttons are being pressed.
		Key is an enum defined in header KeyValues.
	*/
	inline void SetKeyState(const unsigned int key, const bool value)
	{zKeyStates.SetKeyState(key,value);}

	inline void SetFrameTime(const float frameTime){this->zFrameTime = frameTime;}
	inline void SetLatency(const float latency){this->zLatency = latency;}
	/*! Sets the objm. This class is not responsible for deallocation.*/
	void SetObjManager(ObjectManager* objm) {this->zObjManager = objm;}
	
private:
	void InitValues();

private:
	float	zLatency;
	float	zFrameTime;

	float	zHunger;
	float	zHydration;

	KeyStates zKeyStates;

	Inventory* zInventory;

	ObjectManager* zObjManager;
};