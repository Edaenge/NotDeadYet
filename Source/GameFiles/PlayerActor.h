/*
Made by Edänge Simon Datum(30/11/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/Actor.h"
#include "GameFiles/AnimationStates.h"
#include "GameFiles/KeyUtil/KeyValues.h"
#include "GameFiles/KeyUtil/KeyStates.h"
#include <vector>

/*This class is used to save player information such as position and states.
  This information is sent to clients.
*/
class PlayerActor : public Actor
{
public:
	PlayerActor();
	PlayerActor(const int ID);
	PlayerActor(const int ID, const Vector3& startPos);
	PlayerActor(const int ID, const Vector3& startPos, const Vector4& startRot);
	virtual ~PlayerActor();

	/*! Updates players pos, states etc.*/
	void Update();
	
	int GetID() const;
	float GetVelocity() const;
	int GetState() const;
	const std::string& GetPlayerModel() const;
	const Vector3& GetDirection() const;

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

	/*! Sets the player state.
		Enum is defined in AnimationStates.h.
	*/
	void SetState(int state);
	void SetVelocity(float velocity);
	void SetDirection(const Vector3& dir);
	void SetPlayerModel(const std::string& model);

private:
	int		zID;
	int		zState;
	float	zVelocity;

	std::string zPlayerModel;
	KeyStates zKeyStates;
	Vector3 zDir;
};