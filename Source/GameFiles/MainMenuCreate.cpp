#include "MainMenu.h"

void MainMenu::Init()
{
	iGraphicsEngineParams* GEP = GetGraphics()->GetEngineParameters();
	int windowWidth = GEP->windowWidth;
	int windowHeight = GEP->windowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	GraphicsEngine* eng = GetGraphics();
	eng->StartRendering();

	Element* temp;

	//MAINMENU
	temp = new SimpleButton(50, 600, 1, "Media/MainMenu.png", 100, 25, new ChangeSetEvent(NOMENU), "Media/Options.png", "Media/Quit.png", 50, 600, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 635, 1, "Media/Options.png", 100, 25, new ChangeSetEvent(OPTIONS), "Media/FindServer.png", "Media/Quit.png", 50, 635, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 670, 1, "Media/FindServer.png", 100, 25, new ChangeSetEvent(FIND_SERVER), "Media/Options.png", "Media/Quit.png", 50, 670, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 700, 1, "Media/Quit.png", 100, 25, new ChangeSetEvent(QUIT), "Media/Options.png", "Media/Options.png", 50, 700, 100, 25);
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
		(105.0f / 1024.0f) * dx,(29.0f / 650.0f) * windowHeight, new ChangeSetEvent(MAINMENU), "", "", 
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
	temp = new GUIPicture(50, 50, 1, "Media/Options.png", windowWidth * (0.66f), windowHeight * (0.66f));
	zSets[OPTIONS].AddElement(temp);

	temp = new TextBox(100, 100, 1, "Media/MainMenu.png", 100, 50, "12", "SoundVolume", 1, 2, TEXTBOX::NR);
	zSets[OPTIONS].AddElement(temp);

	temp = new SimpleButton(70, 500, 1, "Media/Options.png", 100, 25, new ChangeSetEvent(MAINMENU), "Media/Quit.png", "Media/FindServer.png", 70, 500, 100, 25);
	zSets[OPTIONS].AddElement(temp);

	this->zPrimarySet = MAINMENU;
	this->zSecondarySet = NOMENU;
}
