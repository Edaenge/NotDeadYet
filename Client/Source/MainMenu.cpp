#include "MainMenu.h"
#include "Safe.h"
#include "Sounds.h"

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
	zSets = 0;
	SAFE_DELETE(this->zGame);
}

void MainMenu::Init()
{
	GraphicsEngine* eng = GetGraphics();

	const iGraphicsEngineParams& GEP = GetGraphics()->GetEngineParameters();
	int windowWidth = GEP.WindowWidth;
	int windowHeight = GEP.WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	eng->StartRendering();

	Element* temp;

	//MAINMENU
	temp = new SimpleButton(50, 600, 1, "Media/Icons/MainMenu.png", 100, 25, new ChangeSetEvent(NOMENU), "Media/Icons/Options.png", "Media/Icons/Quit.png", 50, 600, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 635, 1, "Media/Icons/Options.png", 100, 25, new ChangeSetEvent(OPTIONS), "Media/Icons/FindServer.png", "Media/Icons/Quit.png", 50, 635, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 670, 1, "Media/Icons/FindServer.png", 100, 25, new ChangeSetEvent(FIND_SERVER), "Media/Icons/Options.png", "Media/Icons/Quit.png", 50, 670, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 700, 1, "Media/Icons/Quit.png", 100, 25, new ChangeSetEvent(QUIT), "Media/Icons/Options.png", "Media/Icons/Options.png", 50, 700, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	//FIND SERVER
	temp = new GUIPicture(0, (59.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/FindServer_BG.png", offSet + (1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Back.png", 
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (539.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Quick_Refresh.png", 
		(171.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (539.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(171.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (712.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Refresh_All.png", 
		(135.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (712.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(135.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (849.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Connect.png", 
		(105.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(GETIPADRESS), "", "", 
		offSet + (849.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(105.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Internet.png", 
		(95.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (76.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(95.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (173.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/Favorites.png", 
		(113.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (173.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(113.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (288.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/History.png", 
		(89.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (288.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(89.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	//OPTIONS
	temp = new GUIPicture(50, 50, 1, "Media/Icons/Options.png", windowWidth * (0.66f), windowHeight * (0.66f));
	zSets[OPTIONS].AddElement(temp);

	temp = new TextBox(100, 100, 1, "Media/Icons/MainMenu.png", 100, 50, "12", "SoundVolume", 1, 2, TEXTBOX::NR);
	zSets[OPTIONS].AddElement(temp);

	temp = new SimpleButton(70, 500, 1, "Media/Icons/Options.png", 100, 25, new ChangeSetEvent(MAINMENU), "Media/Icons/Quit.png", "Media/Icons/FindServer.png", 70, 500, 100, 25);
	zSets[OPTIONS].AddElement(temp);

	//Get IP
	float AdressX = (offSet + (512.0f / 1024.0f) * dx) - (((200.0f / 1024.0f) * dx) / 2);
	float AdressY = ((384.0f / 768.0f) * windowHeight) - (((100.0f / 768.0f) * windowHeight) / 2);
	temp = new GUIPicture(AdressX, AdressY, 1, 
		"Media/Icons/IP_Adress.png", (200.0f / 1024.0f) * dx, (100.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	//IPAddress
	temp = new TextBox(AdressX + (10.0f / 1024.0f) * dx, AdressY + (34.0f / 768.0f) * windowHeight, 1, "", 
		(180.0f / 1024.0f) * dx, (40.0f / 768.0f) * windowHeight, "127.0.0.1", "IPAdress", 1.0f, 16, NR_SPECIAL);
	zSets[GETIPADRESS].AddElement(temp);

	temp = new SimpleButton(AdressX + (120.0f / 1024.0f) * dx, AdressY + (78.0f / 768.0f) * windowHeight, 1, 
		"Media/Icons/buton_ok.png", (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight, 
		new ChangeSetEvent(FIND_SERVER), "", "", AdressX + (120.0f / 1024.0f) * dx
		, AdressY + (78.0f / 768.0f) * windowHeight, (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	temp = new SimpleButton(AdressX + (45.0f / 1024.0f) * dx, AdressY + (78.0f / 768.0f) * windowHeight, 1, 
		"Media/Icons/buton_cancel.png", (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight, 
		new ChangeTextAndMenuEvent(FIND_SERVER, "IPAdress"), "", "", AdressX + (45.0f / 1024.0f) * dx
		, AdressY + (78.0f / 768.0f) * windowHeight, (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	this->zPrimarySet = MAINMENU;
	this->zSecondarySet = NOMENU;
}

void MainMenu::StartTestRun()
{
	// zGame->InitGameClient("194.47.150.16", 11521);
	zGame->InitGameClient("127.0.0.1", 11521);	
	zGame->Run();
}

void MainMenu::Run()
{
	bool proModeOrNot = true; // CHANGE HERE!!!!!!!
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

						break;
					case OPTIONS:

						break;
					case QUIT:
						run = false;
						break;
					default:

						break;
					}
				}
				else if(retEvent->GetEventMessage() == "ChangeTextAndMenuEvent")
				{
					ChangeTextAndMenuEvent* cEvent = (ChangeTextAndMenuEvent*)retEvent;

					string temp = zSets[this->zPrimarySet].GetTextFromField(cEvent->GetTextBoxName());

					

					this->SwapMenus(NOMENU, NOMENU);

					this->EnableMouse(false);

					zGame->InitGameClient(temp, 11521);	
					zGame->Run();

					this->EnableMouse(true);

					this->SwapMenus(MAINMENU, this->zSecondarySet);
				}
			}
			else
			{
				//Returned no event
			}
		}

		Sleep(10);
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