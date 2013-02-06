/*
Made by Jensen Christopher Datum(01/02/13 created) 
for project Not Dead Yet at Blekinge tekniska högskola.
*/

#pragma once

#include "Actor.h"
#include "Player.h"
#include "KeyValues.h"
#include "KeyStates.h"
#include "NetworkMessageConverter.h"

class GhostActor : public Actor
{
public:
	GhostActor(Player* player);
	GhostActor(const bool genID /*= false*/, const Vector3& position);
	virtual ~GhostActor();

	Vector3 GetPosition() {return this->zPosition;}
	Vector3 GetDirection() {return this->zDirection;}
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

	void SetPosition(const Vector3& position) {this->zPosition = position;}
	void SetDirection(const Vector3& direction) {this->zDirection = direction;}
	void Update(float deltaTime);

	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
private:
	float zVelocity;
	float maxHeight;
	float zEnergy;
	Vector3 zPosition;
	Vector3 zDirection;
	KeyStates zKeyStates;
	Player* zPlayer;
};