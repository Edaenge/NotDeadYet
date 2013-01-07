//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.
#pragma once

#include "Game.h"

class MainMenu 
{
public:
	MainMenu();
	virtual ~MainMenu();
	void Run();

private:
	void PrintClientError(const int code);
	void PrintHostError(const int code);
	/*! Prints the error message to the client*/
	void PrintToScreen(const std::string msg);

private:
	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;
};