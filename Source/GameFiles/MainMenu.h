//Made by Edänge Simon 
//for project desperation* at Blekinge tekniska högskola.
#pragma once

#include "Game.h"

class MainMenu 
{
public:
	MainMenu();
	virtual ~MainMenu();
	void run();

private:
	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;
private:
	void printClientError(const int code);
	void printHostError(const int code);
	/*! Prints the error message to the client*/
	void printToScreen(const std::string msg);
};