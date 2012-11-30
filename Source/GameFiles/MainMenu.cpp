#include "MainMenu.h"

MainMenu::MainMenu()
{
	int hostErrorCode;
	int clientErrorCode;
	
	zClientConnected = false;
	zHostCreated	 = false;
	this->zGame		 = new Game();
	// Initializes the host and returns a code that explains what happend
	hostErrorCode    = this->zGame->InitGameHost(8080, 1);
	// Initializes the Client and returns a code that explains what happend
	clientErrorCode  = this->zGame->InitGameClient("127.0.0.1", 8080);

	if(hostErrorCode == 0 && clientErrorCode == 0)
	{
		this->zGame->Run();
	}
}

MainMenu::~MainMenu()
{
	SAFE_DELETE(this->zGame);
}

void MainMenu::printClientError(const int code )
{
	std::string errorMessage = "";
	switch (code)
	{
	case 0:
		this->zClientConnected = true;
		errorMessage = "Connection successful";
		break;
	case 1:
		errorMessage = "Connection to server failed. ";
		break;
	case 2:
		errorMessage = "Failed to init Winsock library";
		break;
	case 3:
		errorMessage = "Invalid socket, failed to create socket";
		break;
	default:
		break;
	}
	printToScreen(errorMessage);
}

void MainMenu::printHostError(const int code )
{
	std::string errorMessage = "";

	switch (code)
	{
	case 0:
		this->zHostCreated = true;
		errorMessage = "Host Creation successful";
		break;
	case 1:
		errorMessage = "Failed to bind socket ";
		break;
	case 2:
		errorMessage = "Failed to init Winsock library";
		break;
	case 3:
		errorMessage = "Invalid socket, failed to create socket";
		break;
	default:
		break;
	}
	printToScreen(errorMessage);
}

void MainMenu::printToScreen(const std::string msg )
{

}

