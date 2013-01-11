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

	Sleep(100);

	Element* temp;

	//MAINMENU
	temp = new SimpleButton(50, 600, 1, "Media/NewGame.png", 100, 25, new ChangeSetEvent(NOMENU), "Media/Options.png", "Media/Quit.png", 50, 600, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 635, 1, "Media/Options.png", 100, 25, new ChangeSetEvent(OPTIONS), "Media/FindServers.png", "Media/Quit.png", 50, 635, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 670, 1, "Media/FindServers.png", 100, 25, new ChangeSetEvent(FIND_SERVER), "Media/Options.png", "Media/Quit.png", 50, 670, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	temp = new SimpleButton(50, 700, 1, "Media/Quit.png", 100, 25, new ChangeSetEvent(QUIT), "Media/Options.png", "Media/Options.png", 50, 700, 100, 25);
	zSets[MAINMENU].AddElement(temp);

	//FIND SERVER
	temp = new GUIPicture(50, 50, 1, "Media/NewGame.png", windowWidth * (0.66f), windowHeight * (0.66f));
	zSets[FIND_SERVER].AddElement(temp);

	temp = new SimpleButton(70, 500, 1, "Media/Options.png", 100, 25, new ChangeSetEvent(MAINMENU), "Media/Quit.png", "Media/FindServers.png", 70, 500, 100, 25);
	zSets[FIND_SERVER].AddElement(temp);

	//OPTIONS
	temp = new GUIPicture(50, 50, 1, "Media/Options.png", windowWidth * (0.66f), windowHeight * (0.66f));
	zSets[OPTIONS].AddElement(temp);

	temp = new TextBox(100, 100, 1, "Media/NewGame.png", 100, 50, "12", "SoundVolume", 1, 2, TEXTBOX::NR);
	zSets[OPTIONS].AddElement(temp);

	temp = new SimpleButton(70, 500, 1, "Media/Options.png", 100, 25, new ChangeSetEvent(MAINMENU), "Media/Quit.png", "Media/FindServers.png", 70, 500, 100, 25);
	zSets[OPTIONS].AddElement(temp);

	this->zPrimarySet = MAINMENU;
	this->zSecondarySet = NOMENU;
}
