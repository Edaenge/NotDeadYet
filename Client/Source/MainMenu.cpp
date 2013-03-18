#include "MainMenu.h"
#include "Safe.h"
#include "NetworkException.h"

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

	menuClick->Release();
	delete menuClick;
	menuClick = NULL;

	AudioManager::ReleaseInstance();
}

void MainMenu::Init()
{
	_sound = AudioManager::GetInstance();
	_soundLoader = AudioManager::GetInstance();
	int result = _soundLoader->LoadFEV("Media/fmod/NotDeadYet.fev");
	if(result != (FMOD_OK))
	{
		throw("Could not load .fev file.");
	}

	AudioManager* am = AudioManager::GetInstance();
	am->GetEventHandle(EVENTID_NOTDEADYET_MENU_N_BACKPACK_TOGGLE_N_CLICK, menuClick);
	menuClick->Setvolume(0.2f);

	GraphicsEngine* eng = GetGraphics();

	eng->CreateSkyBox("Media/skymap.dds");


	float windowWidth = (float)eng->GetEngineParameters().WindowWidth;
	float windowHeight = (float)eng->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	this->zSizedForWidth = windowWidth;
	this->zSizedForHeight = windowHeight;

	const char* object[] = {
		"media/mapmodels/bush_01.ani",
		"media/mapmodels/bush_02.ani",
		"media/mapmodels/bern_02.ani",
		"media/mapmodels/Tree_01.ani",
		"media/mapmodels/Tree_02.ani",
		"media/mapmodels/grassplant_01.ani",
		"media/mapmodels/watergrass_02.ani",
		"media/models/ghost.obj",
		"media/mapmodels/deer.obj",
		"media/models/bow_v01.obj",
		"media/models/temp_guy.obj",
		"media/mapmodels/target.obj",
		"media/models/arrow_v01.obj",
		"media/models/deer_temp.obj",
		"media/models/canteen_01.obj",
		"media/models/supplyaddon.obj",
		"media/mapmodels/stone_01.obj",
		"media/mapmodels/stone_02.obj",
		"media/mapmodels/stone_03.obj",
		"media/mapmodels/veins_01.obj",
		"media/models/machete_v01.obj",
		"media/models/armyration_v01.obj",
		"media/models/campfire_01_v01.obj",
		"media/models/pocketknife_v02.obj",
		"media/mapmodels/bush_misc_01.obj",
		"media/models/stoneItem_01_v01.obj",
		"media/models/branchesitem_01_v01.obj",
		"media/models/supplycrate_01_v03.obj"};

	eng->PreLoadResources(27, object);

	Element* temp;

	//MAINMENU
	#if defined(DEBUG) || defined(_DEBUG)
	temp = new SimpleButton(0, 0,  1.0f, "Media/Menu/MainMenu/DevButton.png", 
		(150.0f / 1024.0f) * dx, (25.0f / 768.0f) * windowHeight, new ChangeSetEvent(NOMENU), 
		"Media/Menu/MainMenu/DevButtonClick.png", "Media/Menu/MainMenu/DevButtonOver.png",
		0, 0, (150.0f / 1024.0f) * dx, (25.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);
	#endif

	temp = new GUIPicture(offSet + (150.0f / 1024.0f) * dx, (0.0f / 768.0f) * windowHeight, 1, "Media/Menu/MainMenu/MainMenuLogo.png", 
		(765.0f / 1024.0f) * dx, (360.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(offSet + (350.0f / 1024.0f) * dx, (350.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/MainMenu/Connect.png", 
		(300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight, new ChangeSetEvent(GETIPADRESS), "Media/Menu/MainMenu/ConnectClick.png", 
		"Media/Menu/MainMenu/ConnectOver.png", offSet + (350.0f / 1024.0f) * dx, (350.0f / 768.0f) * windowHeight, (300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(offSet + (350.0f / 1024.0f) * dx, (430.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/MainMenu/Options.png", 
		(300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight, new ChangeSetEvent(OPTIONS), "Media/Menu/MainMenu/OptionsClick.png", 
		"Media/Menu/MainMenu/OptionsOver.png", offSet + (350.0f / 1024.0f) * dx, (430.0f / 768.0f) * windowHeight, (300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(offSet + (350.0f / 1024.0f) * dx, (510.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/MainMenu/Quit.png", 
		(300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight, new ChangeSetEvent(QUIT), "Media/Menu/MainMenu/QuitClick.png", 
		"Media/Menu/MainMenu/QuitOver.png", offSet + (350.0f / 1024.0f) * dx, (510.0f / 768.0f) * windowHeight, (300.0f / 1024.0f) * dx, (60.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	//Get IP
	float AdressX = (offSet + (512.0f / 1024.0f) * dx) - (((564.0f / 1024.0f) * dx) / 2);
	float AdressY = ((384.0f / 768.0f) * windowHeight) - (((137.0f / 768.0f) * windowHeight) / 2);
	temp = new GUIPicture(AdressX, AdressY, 1, 
		"Media/Menu/ConnectMenu/Connect_BG.png", (564.0f / 1024.0f) * dx, (137.0f / 768.0f) * windowHeight);

	zSets[GETIPADRESS].AddElement(temp);

	//IPAddress
	//194.47.150.16 - Server
	temp = new TextBox(AdressX + (20.0f / 1024.0f) * dx, AdressY + (60.0f / 768.0f) * windowHeight, 1.0f, "", 
		(530.0f / 1024.0f) * dx, (40.0f / 768.0f) * windowHeight, "194.47.150.16", "IPAdress", 2.0f, 16, ALL);
	zSets[GETIPADRESS].AddElement(temp);

	temp = new SimpleButton(AdressX + (472.0f / 1024.0f) * dx, AdressY + (104.0f / 768.0f) * windowHeight, 1.0f, 
		"Media/Menu/ConnectMenu/Back.png", (67.0f / 1024.0f) * dx, (28.0f / 768.0f) * windowHeight, 
		new ChangeSetEvent(MAINMENU), "Media/Menu/ConnectMenu/BackPress.png", "Media/Menu/ConnectMenu/BackOver.png", AdressX + (472.0f / 1024.0f) * dx
		, AdressY + (104.0f / 768.0f) * windowHeight, (67.0f / 1024.0f) * dx, (28.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);
		
	temp = new SimpleButton(AdressX + (348.0f / 1024.0f) * dx, AdressY + (104.0f / 768.0f) * windowHeight, 1.0f, 
		"Media/Menu/ConnectMenu/Connect.png", (111.0f / 1024.0f) * dx, (28.0f / 768.0f) * windowHeight, 
		new ChangeTextAndMenuEvent(MAINMENU, "IPAdress"), "Media/Menu/ConnectMenu/ConnectPress.png", "Media/Menu/ConnectMenu/ConnectOver.png", AdressX + (348.0f / 1024.0f) * dx
		, AdressY + (104.0f / 768.0f) * windowHeight, (111.0f / 1024.0f) * dx, (28.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	//Options Menu

	//Graphics Column
	temp = new GUIPicture(offSet, (59.0f / 768.0f) * dx, 1.0f, "Media/Menu/Options/OptionsBG.png", 
		(1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp); 

	temp = new GUIPicture(offSet, (59.0f / 768.0f) * dx, 1.0f, "Media/Menu/Options/OptionsBGText.png", 
		(1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp); 
	
	/*//Option Text
	temp = new GUIPicture(offSet + (40.0f / 1024.0f) * dx, (130.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/OptionsText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp); 
	
	//Graphics Text
	temp = new GUIPicture(offSet + (100.0f / 1024.0f) * dx, (210.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/GraphicsText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//WindowedText
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (240.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/WindowedText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Resolution Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (300.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ResolutionText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Shadow Quality Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (420.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ShadowText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//FXAA Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (360.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/FXAAText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//View Distance Text
	temp = new GUIPicture(offSet + (120.0f / 1024.0f) * dx, (480.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/ViewdistanceText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	// Sound Column
	//Sound Text
	temp = new GUIPicture(offSet + (500.0f / 1024.0f) * dx, (210.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/SoundText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Mastervolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (240.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/MastervolumeText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Musicvolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (300.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/MusicvolumeText.png",
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Normalvolume Text
	temp = new GUIPicture(offSet + (520.0f / 1024.0f) * dx, (360.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/NormalvolumeText.png", 
		(175.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);*/

	// Tech stuff
	//Graphics
	//DropDown list
	temp = new DropDownList(offSet + (240.0f / 1024.0f) * dx, (295.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/DropDownBG.png", 
		(170.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Resolutions");
	DropDownList* dropdownlist = (DropDownList*)temp;

	this->AddResolutionsToDropBox(dropdownlist);

	zSets[OPTIONS].AddElement(temp);

	//CheckBox FXAA
	bool checked = false;
	if(GetGraphics()->GetEngineParameters().FXAAQuality > 0)
		checked = true;

	temp = new CheckBox(offSet + (180.0f / 1024.0f) * dx, (355.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/CheckBoxFrame.png", 
		(32.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Media/Menu/Options/CheckBoxCheck.png", checked, 
		new ChangeOptionEvent("FXAA", "false"), "FXAACheckBox");
	zSets[OPTIONS].AddElement(temp);

	//CheckBox Windowed
	checked = !GetGraphics()->GetEngineParameters().Maximized;

	temp = new CheckBox(offSet + (240.0f / 1024.0f) * dx, (233.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/CheckBoxFrame.png", 
		(32.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "Media/Menu/Options/CheckBoxCheck.png", checked, 
		new ChangeOptionEvent("Windowed", "false"), "WindowedCheckBox");
	zSets[OPTIONS].AddElement(temp);

	//TextBox View Distance
	temp = new TextBox(offSet + (278.0f / 1024.0f) * dx, (470.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().FarClip), 
		"ViewDistance", 2.0f, 3, NR, 0, 9);
	zSets[OPTIONS].AddElement(temp);

	//TextBox Shadow
	temp = new TextBox(offSet + (295.0f / 1024.0f) * dx, (412.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().ShadowMapSettings), 
		"ShadowQuality", 2.0f, 1, NR, 0, 9);
	zSets[OPTIONS].AddElement(temp);


	//Buttons options menu
	//Back
	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Back.png", 
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "Media/Menu/BackPress.png", "Media/Menu/BackOver.png", 
		offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	//Apply
	temp = new SimpleButton(offSet + (148.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Apply.png", 
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ApplyOptionsAndChangeSetEvent(MAINMENU), "Media/Menu/ApplyPress.png", "Media/Menu/ApplyOver.png", 
		offSet + (148.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(71.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);


	this->zPrimarySet = MAINMENU;
	this->zSecondarySet = NOMENU;
}

void MainMenu::StartTestRun()
{
	bool result = false;
	try
	{
		//result = zGame->InitGameClient("80.78.216.201", 11521); //Simon hem
		//result = zGame->InitGameClient("194.47.150.16", 11521); //server
		//result = zGame->InitGameClient("194.47.150.20", 11521); //Simon
		//result = zGame->InitGameClient("194.47.150.12", 11521); //Christopher
		result = zGame->InitGameClient("127.0.0.1", 11521); // Local
	}
	catch (NetworkException e)
	{
		GraphicsEngine* gEng = GetGraphics();
		Vector2 position = Vector2(50.0f, gEng->GetEngineParameters().WindowHeight * 0.5f);

		std::string errorMsg = "Error Code: " + MaloW::convertNrToString((float)e.errCode) + ": " + e.errString;
		iText* errorText = GetGraphics()->CreateText(errorMsg.c_str(), position, 0.7f, "Media/Fonts/new");

		Sleep(5000);

		gEng->DeleteText(errorText);
	}
	catch(...)
	{
		GraphicsEngine* gEng = GetGraphics();
		Vector2 position = Vector2(50.0f, gEng->GetEngineParameters().WindowHeight * 0.5f);

		std::string errorMsg = "Unknown Exception occurred";
		iText* errorText = GetGraphics()->CreateText(errorMsg.c_str(), position, 0.7f, "Media/Fonts/new");

		Sleep(5000);

		gEng->DeleteText(errorText);
	}
	
	if (result)
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
		GraphicsEngine* eng = GetGraphics();
		eng->StartRendering();

		//eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 0.0f, 0.0f, 0.0f, 0.0f);
		bool run = true;

		eng->GetCamera()->SetUpdateCamera(false);
		eng->GetKeyListener()->SetCursorVisibility(true);
		eng->Update();

		Vector2 mousePos;
		GUIEvent* retEvent = NULL;
		while(run)
		{
			eng->Update();
			mousePos = GetGraphics()->GetKeyListener()->GetMousePosition();
			if(mousePos.x != -1 || mousePos.y != -1)
			{
				//Try to get an event from buttons, if no event from main set try second.
				retEvent = zSets[this->zPrimarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, eng->GetKeyListener()->IsClicked(1), GetGraphics());
				if(retEvent == NULL)
					retEvent = zSets[this->zSecondarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, eng->GetKeyListener()->IsClicked(1), GetGraphics());

				if(retEvent != NULL)
				{
					menuClick->Play();
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
							
							delete this->zGame;

							this->zGame = new Game();

							this->EnableMouse(true);

							this->SwapMenus(MAINMENU, this->zSecondarySet);

							break;
						case MAINMENU:

							break;
						case FIND_SERVER:

							break;
						case OPTIONS:
							UpdateOptionsMenu();
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
						this->StartGameWithIPField();
					}
					/*else if(retEvent->GetEventMessage() == "ChangeResEvent")
					{
						if(this->zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox")->GetOn())
						{
							ChangeResEvent* cEvent = (ChangeResEvent*)retEvent;

							float windowWidth = (float)cEvent->GetWidth();
							float windowHeight = (float)cEvent->GetHeight();
							int i = NOMENU;
							while(i != LASTMENU)
							{
								zSets[i].Resize((float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight, windowWidth, windowHeight);
								i++;
							}
							GetGraphics()->ResizeGraphicsEngine((int)windowWidth, (int)windowHeight);
						}

					}*/
					else if(retEvent->GetEventMessage() == "ApplyOptionsAndChangeSetEvent")
					{
						GraphicsEngine* ge = GetGraphics();
						iGraphicsEngineParams& GEP = ge->GetEngineParameters();

						ApplyOptionsAndChangeSetEvent* cEvent = (ApplyOptionsAndChangeSetEvent*)retEvent;

						//Maximized
						bool maximized = this->zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox")->GetOn();
						GEP.Maximized = !maximized;
						if(!maximized)
						{
							RECT desktop;
							const HWND hDesktop = GetDesktopWindow();
							GetWindowRect(hDesktop, &desktop);
							float width = (float)desktop.right;
							float height = (float)desktop.bottom;

							int i = NOMENU;
							while(i != LASTMENU)
							{
								zSets[i].Resize((float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight, width, height);
								i++;
							}
							GetGraphics()->ResizeGraphicsEngine((int)width, (int)height);
						}
						else if(maximized)
						{
							float oldWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
							float oldHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;
							
							float width = oldWidth;
							float height = oldHeight;

							GUIEvent* temp = this->zSets[this->zPrimarySet].GetEventFromDropDown("Resolutions");
							if(ChangeResEvent *cre = dynamic_cast<ChangeResEvent *>(temp))
							{
								if (NULL != cre)
								{
									width = (float)cre->GetWidth();
									height = (float)cre->GetHeight();
								}
							}

							int i = NOMENU;
							while(i != LASTMENU)
							{
								zSets[i].Resize(oldWidth, oldHeight, width, height);
								i++;
							}
							GetGraphics()->ResizeGraphicsEngine((int)width, (int)height);
						}
						// Getting shadow
						std::string tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("ShadowQuality");
						ge->ChangeShadowQuality(MaloW::convertStringToInt(tbTemp));
						GEP.ShadowMapSettings = MaloW::convertStringToInt(tbTemp);
						//FXAA
						CheckBox* cbTemp = this->zSets[this->zPrimarySet].GetCheckBox("FXAACheckBox");
						if(cbTemp != NULL)
						{
							if(cbTemp->GetOn())
								GEP.FXAAQuality = 4;
							if(!cbTemp->GetOn())
								GEP.FXAAQuality = 0;
						}

						//View Distance
						tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("ViewDistance");
						GEP.FarClip = MaloW::convertStringToFloat(tbTemp);

						GEP.SaveToFile("Config.cfg");

						this->SwapMenus((SET)cEvent->GetSet(), NOMENU);
						zPrimarySet = (SET)cEvent->GetSet();
					}
				}
				else if(this->zPrimarySet == GETIPADRESS)
				{
					if(GetGraphics()->GetKeyListener()->IsPressed(VK_RETURN))
					{
						this->StartGameWithIPField();
					}
				}
				else
				{
					//Returned no event
				}
				Sleep(50);
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

void MainMenu::AddResolutionsToDropBox( DropDownList* ddl )
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	float width = (float)desktop.right;
	float height = (float)desktop.bottom;

	float dx = (float)(width / height);

	
	if(dx > 1.77f && dx < 1.78f)
	{
		ddl->AddButton("Media/Menu/Options/169small.png", new ChangeResEvent(1280, 720), "", "");
		ddl->AddButton("Media/Menu/Options/169medium.png", new ChangeResEvent(1600, 900), "", "");
		ddl->AddButton("Media/Menu/Options/169big.png", new ChangeResEvent(1920, 1080), "", "");
	}
	else if(dx > 1.59f && dx < 1.61f)
	{
		ddl->AddButton("Media/Menu/Options/1610small.png", new ChangeResEvent(1280, 800), "", "");
		ddl->AddButton("Media/Menu/Options/1610medium.png", new ChangeResEvent(1440, 900), "", "");
		ddl->AddButton("Media/Menu/Options/1610big.png", new ChangeResEvent(1680, 1050), "", "");
	}
	else
	{
		ddl->AddButton("Media/Menu/Options/43small.png", new ChangeResEvent(800, 600), "", "");
		ddl->AddButton("Media/Menu/Options/43medium.png", new ChangeResEvent(1024, 768), "", "");
		ddl->AddButton("Media/Menu/Options/43big.png", new ChangeResEvent(1600, 1200), "", "");
	}
}

void MainMenu::UpdateOptionsMenu()
{
	//Update the options menu cause values might have changed.

	//Update maximized.
	CheckBox* cbTemp = zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox");
	cbTemp->SetChecked(!GetGraphics()->GetEngineParameters().Maximized);

	//Update FXAA.
	cbTemp = zSets[this->zPrimarySet].GetCheckBox("FXAACheckBox");
	if(GetGraphics()->GetEngineParameters().FXAAQuality == 0)
		cbTemp->SetChecked(false);
	else
		cbTemp->SetChecked(true);

	//Update Shadow Quality.
	TextBox* tbTemp = this->zSets[this->zPrimarySet].GetTextBox("ShadowQuality");
	tbTemp->SetText(MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().ShadowMapSettings));

	//Update View Distance.
	tbTemp = this->zSets[this->zPrimarySet].GetTextBox("ViewDistance");
	tbTemp->SetText(MaloW::convertNrToString(GetGraphics()->GetEngineParameters().FarClip));
}

void MainMenu::Resize()
{
	if(this->zSizedForWidth == GetGraphics()->GetEngineParameters().WindowWidth &&
		this->zSizedForHeight == GetGraphics()->GetEngineParameters().WindowHeight)
		return;

	this->zSets[MAINSET].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight);
	this->zSets[FIND_SERVER].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight);
	this->zSets[OPTIONS].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight);
	this->zSets[GETIPADRESS].Resize(this->zSizedForWidth, this->zSizedForHeight, (float)GetGraphics()->GetEngineParameters().WindowWidth, (float)GetGraphics()->GetEngineParameters().WindowHeight);


	this->zSizedForWidth = (float)GetGraphics()->GetEngineParameters().WindowWidth;
	this->zSizedForHeight = (float)GetGraphics()->GetEngineParameters().WindowHeight;
}

void MainMenu::StartGameWithIPField()
{
	string temp = zSets[this->zPrimarySet].GetTextFromField("IPAdress");

	this->SwapMenus(NOMENU, NOMENU);

	this->EnableMouse(false);

	bool result = false;
	try
	{
		result = zGame->InitGameClient(temp, 11521);	 // Save to connect IP
	}
	catch (NetworkException e)
	{
		GraphicsEngine* gEng = GetGraphics();
		Vector2 position = Vector2(50.0f, gEng->GetEngineParameters().WindowHeight * 0.5f);

		std::string errorMsg = "Error Code: " + MaloW::convertNrToString((float)e.errCode) + ": " + e.errString;
		iText* errorText = GetGraphics()->CreateText(errorMsg.c_str(), position, 0.7f, "Media/Fonts/new");

		Sleep(3000);

		gEng->DeleteText(errorText);
	}
	catch(...)
	{
		GraphicsEngine* gEng = GetGraphics();
		Vector2 position = Vector2(50.0f, gEng->GetEngineParameters().WindowHeight * 0.5f);

		std::string errorMsg = "Unknown Exception occurred";
		iText* errorText = GetGraphics()->CreateText(errorMsg.c_str(), position, 0.7f, "Media/Fonts/new");

		Sleep(3000);

		gEng->DeleteText(errorText);
	}
	if (result)
	{
		this->zGame->Run();
	}

	delete this->zGame;

	this->zGame = new Game();

	this->EnableMouse(true);

	this->SwapMenus(MAINMENU, this->zSecondarySet);
}