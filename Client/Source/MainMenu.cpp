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

	SoundsInit();
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

	float windowWidth = (float)eng->GetEngineParameters().WindowWidth;
	float windowHeight = (float)eng->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	const char* object[] = {
		"Media/Models/Bush_01.ani",
		"Media/Models/Fern_02.ani",
		"Media/Models/Tree_01.ani",
		"Media/Models/Bigleaf_01.ani",
		"Media/Models/GrassPlant_01.ani",
		"Media/Models/WaterGrass_02.ani",
		"Media/Models/Bow_v01.obj",
		"Media/Models/temp_guy.obj",
		"Media/Models/Arrow_v01.obj",
		"Media/Models/Machete_v01.obj",
		"Media/Models/Bush_02_v02.obj",
		"Media/Models/Stone_02_v01.obj",
		"Media/Models/Stone_01_v02.obj",
		"Media/Models/ArmyRation_v01.obj", 
		"Media/Models/Veins_01_v03_r.obj",
		"Media/Models/Campfire_01_v01.obj",
		"Media/Models/Pocketknife_v02.obj",
		"Media/Models/StoneItem_01_v01.obj",
		"Media/Models/BranchesItem_01_v01.obj",};

	GetGraphics()->PreLoadResources(19, object);

	eng->StartRendering();

	Element* temp;

	//MAINMENU
	temp = new SimpleButton((50.0f / 768.0f) * dx, (600.0f / 768.0f) * windowHeight,  1.0f, "Media/Icons/MainMenu.png", 
		(100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight, new ChangeSetEvent(NOMENU), "Media/Icons/Options.png", 
		"Media/Icons/Quit.png", (50.0f / 768.0f) * dx, (600.0f / 768.0f) * windowHeight, (100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton((50.0f / 768.0f) * dx, (635.0f / 768.0f) * windowHeight, 1.0f, "Media/Icons/Options.png", 
		(100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight, new ChangeSetEvent(OPTIONS), "Media/Icons/FindServer.png", 
		"Media/Icons/Quit.png", (50.0f / 768.0f) * dx, (635.0f / 768.0f) * windowHeight, (100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton((50.0f / 768.0f) * dx, (670.0f / 768.0f) * windowHeight, 1.0f, "Media/Icons/FindServer.png", 
		(100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight, new ChangeSetEvent(FIND_SERVER), "Media/Icons/Options.png", 
		"Media/Icons/Quit.png", (50.0f / 768.0f) * dx, (670.0f / 768.0f) * windowHeight, (100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton((50.0f / 768.0f) * dx, (700.0f / 768.0f) * windowHeight, 1.0f, "Media/Icons/Quit.png", 
		(100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight, new ChangeSetEvent(QUIT), "Media/Icons/Options.png", 
		"Media/Icons/Options.png", (50.0f / 768.0f) * dx, (700.0f / 768.0f) * windowHeight, (100.0f / 768.0f) * dx, (25.0f / 768.0f) * windowHeight);
	zSets[MAINMENU].AddElement(temp);

	//FIND SERVER
	temp = new GUIPicture(offSet, (59.0f / 768.0f) * windowHeight, 1, "Media/Menu/FindServer/FindServer_BG.png", 
		(1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Back.png", 
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (539.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Quick_Refresh.png", 
		(171.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (539.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(171.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (712.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Refresh_All.png", 
		(135.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (712.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(135.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (849.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Connect.png", 
		(105.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(GETIPADRESS), "", "", 
		offSet + (849.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(105.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Internet.png", 
		(95.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (76.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(95.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (173.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Favorites.png", 
		(113.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (173.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(113.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(offSet + (288.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/History.png", 
		(89.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (288.0f / 1024.0f) * dx, (135.0f / 768.0f) * windowHeight,
		(89.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[FIND_SERVER].AddElement(temp);

	//Get IP
	float AdressX = (offSet + (512.0f / 1024.0f) * dx) - (((200.0f / 1024.0f) * dx) / 2);
	float AdressY = ((384.0f / 768.0f) * windowHeight) - (((100.0f / 768.0f) * windowHeight) / 2);
	temp = new GUIPicture(AdressX, AdressY, 1, 
		"Media/Icons/IP_Adress.png", (200.0f / 1024.0f) * dx, (100.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	//IPAddress
	temp = new TextBox(AdressX + (10.0f / 1024.0f) * dx, AdressY + (34.0f / 768.0f) * windowHeight, 1.0f, "", 
		(180.0f / 1024.0f) * dx, (40.0f / 768.0f) * windowHeight, "127.0.0.1", "IPAdress", 1.0f, 16, NR_SPECIAL);
	zSets[GETIPADRESS].AddElement(temp);

	temp = new SimpleButton(AdressX + (120.0f / 1024.0f) * dx, AdressY + (78.0f / 768.0f) * windowHeight, 1.0f, 
		"Media/Icons/buton_ok.png", (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight, 
		new ChangeSetEvent(FIND_SERVER), "", "", AdressX + (120.0f / 1024.0f) * dx
		, AdressY + (78.0f / 768.0f) * windowHeight, (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	temp = new SimpleButton(AdressX + (45.0f / 1024.0f) * dx, AdressY + (78.0f / 768.0f) * windowHeight, 1.0f, 
		"Media/Icons/buton_cancel.png", (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight, 
		new ChangeTextAndMenuEvent(FIND_SERVER, "IPAdress"), "", "", AdressX + (45.0f / 1024.0f) * dx
		, AdressY + (78.0f / 768.0f) * windowHeight, (75.0f / 1024.0f) * dx, (20.0f / 768.0f) * windowHeight);
	zSets[GETIPADRESS].AddElement(temp);

	//Options Menu

	//Graphics Column
	temp = new GUIPicture(offSet, (59.0f / 768.0f) * dx, 1.0f, "Media/Menu/Options/OptionsBG.png", 
		(1024.0f / 1024.0f) * dx, (650.0f / 768.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp); 
	
	//Option Text
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
	zSets[OPTIONS].AddElement(temp);

	// Tech stuff
	//Graphics
	//DropDown list
	temp = new DropDownList(offSet + (240.0f / 1024.0f) * dx, (295.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/DropDownBG.png", 
		(170.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, "ResolutionMenu");
	DropDownList* dropdownlist = (DropDownList*)temp;

	this->AddResolutionsToDropBox(dropdownlist);

	zSets[OPTIONS].AddElement(temp);

	//CheckBox FXAA
	bool checked = false;
	if(GetGraphics()->GetEngineParameters().FXAAQuality > 0)
		checked = true;

	temp = new CheckBox(offSet + (195.0f / 1024.0f) * dx, (355.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/CheckBoxFrame.png", 
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
		"ViewDistance", 1.0f, 3, NR, 0, 9);
	zSets[OPTIONS].AddElement(temp);

	//TextBox Shadow
	temp = new TextBox(offSet + (295.0f / 1024.0f) * dx, (412.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (32.0f / 768.0f) * windowHeight, MaloW::convertNrToString((float)GetGraphics()->GetEngineParameters().ShadowMapSettings), 
		"ShadowQuality", 1.0f, 1, NR, 0, 9);
	zSets[OPTIONS].AddElement(temp);

	//Sound tech
	//Master volume
	temp = new TextBox(offSet + (690.0f / 1024.0f) * dx, (235.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"MasterVolume", 1.0f, 2, NR);
	zSets[OPTIONS].AddElement(temp);

	//Music Volume
	temp = new TextBox(offSet + (680.0f / 1024.0f) * dx, (295.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"MusicVolume", 1.0f, 2, NR);
	zSets[OPTIONS].AddElement(temp);

	//Normal Volume
	temp = new TextBox(offSet + (695.0f / 1024.0f) * dx, (355.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Options/TextBox4032.png", 
		(40.0f / 1024.0f) * dx, (float)(32.0f / 768.0f) * windowHeight, MaloW::convertNrToString(100), 
		"NormalVolume", 1.0f, 2, NR);
	zSets[OPTIONS].AddElement(temp);


	//Buttons options menu
	//Back
	temp = new SimpleButton(offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/FindServer/Back.png", 
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
		offSet + (76.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);

	temp = new SimpleButton(offSet + (143.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/Apply.png", 
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight, new ApplyOptionsAndChangeSetEvent(MAINMENU), "", "", 
		offSet + (143.0f / 1024.0f) * dx, (638.0f / 768.0f) * windowHeight,
		(65.0f / 1024.0f) * dx, (29.0f / 650.0f) * windowHeight);
	zSets[OPTIONS].AddElement(temp);


	this->zPrimarySet = MAINMENU;
	this->zSecondarySet = NOMENU;
}

void MainMenu::StartTestRun()
{
	//zGame->InitGameClient("194.47.150.16", 11521);
	//zGame->InitGameClient("194.47.150.12", 11521);
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
			if(mousePos.x != -1 || mousePos.y != -1)
			{
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
						ChangeTextAndMenuEvent* cEvent = (ChangeTextAndMenuEvent*)retEvent;

						string temp = zSets[this->zPrimarySet].GetTextFromField(cEvent->GetTextBoxName());

					

						this->SwapMenus(NOMENU, NOMENU);

						this->EnableMouse(false);

						zGame->InitGameClient(temp, 11521);	 // Save to connect IP
						zGame->Run();

						this->EnableMouse(true);

						this->SwapMenus(MAINMENU, this->zSecondarySet);
					}
					else if(retEvent->GetEventMessage() == "ChangeResEvent")
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

					}
					else if(retEvent->GetEventMessage() == "ApplyOptionsAndChangeSetEvent")
					{
						GraphicsEngine* ge = GetGraphics();
						iGraphicsEngineParams& GEP = ge->GetEngineParameters();

						ApplyOptionsAndChangeSetEvent* cEvent = (ApplyOptionsAndChangeSetEvent*)retEvent;

						//Maximized
						bool maximized = this->zSets[this->zPrimarySet].GetCheckBox("WindowedCheckBox")->GetOn();
						GEP.Maximized = !maximized;
						if(maximized == false)
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

						//Master Volume
						tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("MasterVolume");
						float temp = MaloW::convertStringToFloat(tbTemp) / 100;
						GetSounds()->SetMasterVolume(temp);

						//Music Volume
						tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("MusicVolume");
						temp = MaloW::convertStringToFloat(tbTemp) / 100;
						GetSounds()->SetMusicVolume(temp);

						//Normal Volume
						tbTemp = this->zSets[this->zPrimarySet].GetTextFromField("NormalVolume");
						temp = MaloW::convertStringToFloat(tbTemp) / 100;
						GetSounds()->SetSoundVolume(temp);

						GEP.SaveToFile("Config.cfg");

						this->SwapMenus((SET)cEvent->GetSet(), NOMENU);
						zPrimarySet = (SET)cEvent->GetSet();
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
