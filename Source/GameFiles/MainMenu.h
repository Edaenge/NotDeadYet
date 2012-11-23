//Made by Edänge Simon 
//for project desperation* at Blekinge tekniska högskola.
#pragma once

#include "Game.h"

class MainMenu 
{
public:
	MainMenu();
	virtual ~MainMenu();

private:
	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;
private:
	void printClientError(int code);
	void printHostError(int code);
	void printToScreen(std::string msg);
};