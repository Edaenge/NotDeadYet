/*
Made by Edänge Simon Datum(30/11/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once

#include "GameFiles/ServerSide/Actor.h"
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
	void Update(float deltaTime);
	
	inline int GetID() const {return this->zID;}
	inline float GetVelocity() const {return this->zVelocity;}
	inline int GetState() const {return this->zState;}
	inline const std::string& GetPlayerModel() const {return this->zPlayerModel;}
	inline const Vector3& GetDirection() const {return this->zDir;}
	inline float GetLatency() const {return this->zLatency;}
	inline float GetFrameTime() const {return this->zFrameTime;}
	inline const Vector3& GetUpVector() const {return this->zUp;}
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
	inline void SetState(const int state){this->zState = state;}
	inline void SetVelocity(const float velocity){this->zVelocity = velocity;}
	inline void SetDirection(const Vector3& dir){this->zDir = dir;}
	inline void SetPlayerModel(const std::string& model){this->zPlayerModel = model;}
	inline void SetFrameTime(const float frameTime){this->zFrameTime = frameTime;}
	inline void SetLatency(const float latency){this->zLatency = latency;}
	inline void SetUpVector(const Vector3& up){this->zUp = up;}

private:
	int		zID;
	int		zState;
	float	zVelocity;
	float	zLatency;
	float	zFrameTime;

	std::string zPlayerModel;
	KeyStates zKeyStates;

	Vector3 zDir;
	Vector3	zUp;
};