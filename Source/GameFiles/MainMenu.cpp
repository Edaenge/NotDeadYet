#include "MainMenu.h"

MainMenu::MainMenu()
{
	this->zGame = new Game();
	this->zGame->InitGameHost(8080, 1);
	this->zGame->InitGameClient("127.0.0.1", 8080);
}

MainMenu::~MainMenu()
{
	SAFE_DELETE(this->zGame);
}

