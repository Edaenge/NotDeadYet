/*
	Made by Jensen Christopher Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/
#pragma once

#include "stdafx.h"
#include "MaloWLib/Process.h"

using namespace MaloW;

class NewPlayerEvent : public ProcessEvent
{
private:
	int zState;
	int zClientID;
	D3DXVECTOR3 zScale;
	D3DXVECTOR3 zPosition;
	std::string zFilename;
	D3DXQUATERNION zRotation;
	
public:
	NewPlayerEvent()
	{
		this->zState	= 0;
		this->zClientID = 0;
		this->zFilename = "";
		this->zScale	= D3DXVECTOR3(1,1,1);
		this->zPosition = D3DXVECTOR3(0,0,0);
		this->zRotation = D3DXQUATERNION(0,0,0,1);
	}
	virtual ~NewPlayerEvent(){}
	void SetPlayerState(int state){this->zState	= state;}
	void SetClientID(int clientID){this->zClientID = clientID;}
	void SetPlayerScale(D3DXVECTOR3 scale){this->zScale = scale;}
	void SetFilename(std::string filename){this->zFilename = filename;}
	void SetPlayerPosition(D3DXVECTOR3 position){this->zPosition = position;}
	void SetPlayerRotation(D3DXQUATERNION rotation){this->zRotation = rotation;}

	int GetPlayerState(){return this->zState;}
	int GetClientID() {return this->zClientID;}
	D3DXVECTOR3 GetPlayerScale(){return this->zScale;}
	std::string GetFilename(){return this->zFilename;}
	D3DXVECTOR3 GetPlayerPosition(){return this->zPosition;}
	D3DXQUATERNION GetPlayerRotation(){return this->zRotation;}
};

class PlayerUpdateEvent : public ProcessEvent
{
private:
	int zState;
	int zClientID;
	bool zNewFile;
	bool zHasState;
	bool zHasPosition;
	bool zHasRotation;
	D3DXVECTOR3 zPosition;
	std::string zFilename;
	D3DXQUATERNION zRotation;
public:
	PlayerUpdateEvent()
	{
		this->zState		= 0;
		this->zClientID		= 0;
		this->zFilename		= "";
		this->zHasState		= false;
		this->zNewFile		= false;
		this->zHasPosition	= false;
		this->zHasRotation	= false;
		this->zPosition		= D3DXVECTOR3(0,0,0);
		this->zRotation		= D3DXQUATERNION(0,0,0,1);
	}
	virtual ~PlayerUpdateEvent(){}

	void SetClientID(int clientID){this->zClientID = clientID;}
	void SetFilename(std::string filename)
	{
		this->zNewFile	= true;
		this->zFilename	= filename;
	}
	void SetPlayerPosition(D3DXVECTOR3 position)
	{
		this->zHasPosition	= true;
		this->zPosition		= position;
	}
	void SetPlayerRotation(D3DXQUATERNION rotation)
	{
		this->zHasRotation	= true;
		this->zRotation		= rotation;
	}
	void SetPlayerState(int state)
	{
		this->zHasState = true;
		this->zState	= state;
	}
	bool HasState(){return this->zHasState;}
	bool HasNewFile(){return this->zNewFile;}
	int GetClientID(){return this->zClientID;}
	int GetPlayerState(){return this->zState;}
	bool HasNewRotation(){return this->zHasRotation;}
	bool HasNewPosition(){return this->zHasPosition;}
	std::string GetFilename(){return this->zFilename;}
	D3DXVECTOR3 GetPlayerPosition(){return this->zPosition;}
	D3DXQUATERNION GetPlayerRotation(){return this->zRotation;}
};
class PingEvent : public ProcessEvent
{
public:
	PingEvent(){};
	virtual ~PingEvent(){};

	std::string GetMessage(){return this->zMessage;}
	void SetMessage(std::string message){this->zMessage = message;}
private:
	std::string zMessage;


};