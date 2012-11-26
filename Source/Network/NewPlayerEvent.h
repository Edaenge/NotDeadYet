/*
	Made by Jensen Christopher 
	for project desperation* at Blekinge tekniska högskola.
*/
#pragma once

#include "stdafx.h"
#include "MaloWLib/Process.h"
class NewPlayerEvent : public MaloW::ProcessEvent
{
private:
	int zClientID;
	D3DXVECTOR3 zScale;
	D3DXVECTOR3 zPosition;
	std::string zFilename;
	D3DXQUATERNION zRotation;
	
public:
	NewPlayerEvent()
	{
		this->zScale	= D3DXVECTOR3(1,1,1);
		this->zFilename = "";
		this->zPosition = D3DXVECTOR3(0,0,0);
		this->zRotation = D3DXQUATERNION(0,0,0,1);
		this->zClientID = 0;
	}
	virtual ~NewPlayerEvent(){}

	void SetClientID(int clientID){this->zClientID = clientID;}
	void SetPlayerScale(D3DXVECTOR3 scale){this->zScale = scale;}
	void SetFilename(std::string filename){this->zFilename = filename;}
	void SetPlayerPosition(D3DXVECTOR3 position){this->zPosition = position;}
	void SetPlayerRotation(D3DXQUATERNION rotation){this->zRotation = rotation;}

	D3DXQUATERNION GetPlayerRotation(){return this->zRotation;}
	D3DXVECTOR3 GetPlayerPosition(){return this->zPosition;}
	D3DXVECTOR3 GetPlayerScale(){return this->zScale;}
	std::string GetFilename(){return this->zFilename;}
	int GetClientID() {return this->zClientID;}
};

class PlayerUpdateEvent : public MaloW::ProcessEvent
{
private:
	int zClientID;
	D3DXVECTOR3 zScale;
	D3DXVECTOR3 zPosition;
	D3DXQUATERNION zRotation;
	std::string zFilename;
	bool zNewFile;
public:
	PlayerUpdateEvent()
	{
		this->zScale	= D3DXVECTOR3(1,1,1);
		this->zFilename = "";
		this->zNewFile = false;
		this->zPosition = D3DXVECTOR3(0,0,0);
		this->zRotation = D3DXQUATERNION(0,0,0,1);
		this->zClientID = 0;
	}
	virtual ~PlayerUpdateEvent(){}

	void SetClientID(int clientID){this->zClientID = clientID;}
	void SetPlayerScale(D3DXVECTOR3 scale){this->zScale = scale;}
	void SetFilename(std::string filename)
	{
		this->zFilename = filename;
		this->zNewFile = true;
	}
	void SetPlayerPosition(D3DXVECTOR3 position){this->zPosition = position;}
	void SetPlayerRotation(D3DXQUATERNION rotation){this->zRotation = rotation;}

	bool HasNewFile(){return this->zNewFile;}
	int GetClientID(){return this->zClientID;}
	D3DXVECTOR3 GetPlayerScale(){return this->zScale;}
	std::string GetFilename(){return this->zFilename;}
	D3DXVECTOR3 GetPlayerPosition(){return this->zPosition;}
	D3DXQUATERNION GetPlayerRotation(){return this->zRotation;}
};