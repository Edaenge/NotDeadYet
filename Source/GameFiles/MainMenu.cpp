#include "MainMenu.h"
#include "Safe.h"

MainMenu::MainMenu()
{


	zClientConnected = false;
	zHostCreated	 = false;
	this->zGame		 = new Game();

}

MainMenu::~MainMenu()
{
	SAFE_DELETE(this->zGame);
}

void MainMenu::PrintClientError(const int code )
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
	PrintToScreen(errorMessage);
}

void MainMenu::PrintHostError(const int code )
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
	PrintToScreen(errorMessage);
}

void MainMenu::PrintToScreen(const std::string msg )
{
	MaloW::Debug(msg);
}

void MainMenu::Run()
{
	int hostErrorCode = 0;
	int clientErrorCode;
	// Initializes the host and returns a code that explains what happend
	hostErrorCode    = this->zGame->InitGameHost(11521, 5);
	// Initializes the Client and returns a code that explains what happend
	clientErrorCode  = this->zGame->InitGameClient("127.0.0.1", 11521);
	//clientErrorCode  = this->zGame->InitGameClient("194.47.150.20", 10000);

	if(hostErrorCode == 0 && clientErrorCode == 0)
	{
		MaloW::Debug("Game Running");
		this->zGame->Run();
	}
	else
	{
		PrintClientError(clientErrorCode);
		PrintHostError(hostErrorCode);
	}
}