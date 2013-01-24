#include "MainMenu.h"
#include "Safe.h"

MainMenu::MainMenu()
{
	zNrOfSets			= 10;	
	zSets				= new GUISet[zNrOfSets];
	zClientConnected	= false;
	zHostCreated		= false;
	this->zGame			= new Game();

	this->zPrimarySet	= MAINMENU;
	this->zSecondarySet = NOMENU;
}

MainMenu::~MainMenu()
{
	delete [] zSets;

	SAFE_DELETE(this->zGame);
}

void MainMenu::Run()
{
	bool proModeOrNot = false; // CHANGE HERE!!!!!!!
	if(!proModeOrNot)
	{
		this->StartTestRun();
	}
	else
	{
		zSets[zPrimarySet].AddSetToRenderer(GetGraphics());

		bool run = true;
		GraphicsEngine* eng = GetGraphics();

		eng->GetCamera()->SetUpdateCamera(false);
		eng->GetKeyListener()->SetCursorVisibility(true);
		eng->Update();

		Vector2 mousePos;
		GUIEvent* retEvent = NULL;
		bool mousePressed = false;
		while(run)
		{
			eng->Update();
			mousePos = GetGraphics()->GetKeyListener()->GetMousePosition();

			//Try to get an event from buttons, if no event from main set try second.
			retEvent = zSets[this->zPrimarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, eng->GetKeyListener()->IsClicked(1), GetGraphics());
			if(retEvent == NULL)
				retEvent = zSets[this->zSecondarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, eng->GetKeyListener()->IsClicked(1), GetGraphics());

			if(retEvent != NULL)
			{
				if(retEvent->GetEventMessage() == "ChangeSetEvent")
				{
					ChangeSetEvent* setEvent = (ChangeSetEvent*)retEvent;
					if(this->zPrimarySet == FIND_SERVER || this->zPrimarySet == OPTIONS)
					{
						zSets[MAINMENU].RemoveSetFromRenderer(eng);
					}

					this->SwapMenus((SET)setEvent->GetSet(), this->zSecondarySet); // THIS IS ALWAYS DONE IN THIS FUNCTION!
					zPrimarySet = (SET)setEvent->GetSet(); // THIS IS ALWAYS DONE IN THIS FUNCTION!

					//Special Menu Things Are Done Below.
					switch(setEvent->GetSet())
					{
					case NOMENU:
						this->SwapMenus((SET)setEvent->GetSet(), this->zSecondarySet);

						this->EnableMouse(false);

						this->StartTestRun();

						this->EnableMouse(true);

						this->SwapMenus(MAINMENU, this->zSecondarySet);

						break;
					case MAINMENU:

						break;
					case FIND_SERVER:
						zSets[MAINMENU].AddSetToRenderer(eng);

						break;
					case OPTIONS:
						zSets[MAINMENU].AddSetToRenderer(eng);

						break;
					case QUIT:
						run = false;
						break;
					default:

						break;
					}
				}
			}
			else
			{
				//Returned no event
			}
		}
	}

	/*
	int hostErrorCode = 0;
	int clientErrorCode;
	// Initializes the Client and returns a code that explains what happend
	//clientErrorCode  = this->zGame->InitGameClient("127.0.0.1", 11521);
	clientErrorCode  = this->zGame->InitGameClient("194.47.150.20", 11521);

	if(clientErrorCode == 0)
	{
		MaloW::Debug("Game Running");
		this->zGame->Run();
	}
	else
	{
		PrintClientError(clientErrorCode);
		PrintHostError(hostErrorCode);
	}*/

}

void MainMenu::StartTestRun()
{
	int clientErrorCode;
	// Initializes the Client and returns a code that explains what happend
	clientErrorCode  = this->zGame->InitGameClient("127.0.0.1", 11521);	
	//clientErrorCode  = this->zGame->InitGameClient("194.47.150.16", 11521);
	if(clientErrorCode == 0)
	{
		MaloW::Debug("Game Running");
		this->zGame->Run();
	}
	else
	{
		PrintClientError(clientErrorCode);
	}
}

void MainMenu::PrintClientError(const int code)
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

void MainMenu::PrintToScreen(const std::string msg)
{
	MaloW::Debug(msg);
}

void MainMenu::SwapMenus(SET primary, SET secondary)
{
	GraphicsEngine* eng = GetGraphics();
	if(primary == NOMENU)
	{
		zSets[this->zPrimarySet].RemoveSetFromRenderer(eng);
		this->zPrimarySet = primary;
	}
	else if(primary != -1)
	{
		zSets[this->zPrimarySet].RemoveSetFromRenderer(eng);
		this->zPrimarySet = primary;
		zSets[this->zPrimarySet].AddSetToRenderer(eng);
	}

	if(secondary == NOMENU)
	{
		zSets[this->zSecondarySet].RemoveSetFromRenderer(eng);
		this->zSecondarySet = secondary;
	}
	else if(secondary != -1)
	{
		zSets[this->zSecondarySet].RemoveSetFromRenderer(eng);
		this->zSecondarySet = secondary;
		zSets[this->zSecondarySet].AddSetToRenderer(eng);
	}
}

void MainMenu::EnableMouse(bool value)
{
	if(value)
	{
		GetGraphics()->GetCamera()->SetUpdateCamera(false);
		GetGraphics()->GetKeyListener()->SetCursorVisibility(true);
	}
	else
	{
		GetGraphics()->GetCamera()->SetUpdateCamera(true);
		GetGraphics()->GetKeyListener()->SetCursorVisibility(false);
	}
	
}