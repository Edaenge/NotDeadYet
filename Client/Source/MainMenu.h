//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.
#pragma once

#include "Game.h"
#include "GUI/GUISet.h"

#define BGSCREENSHOTS 9
#define PAUSEBETWEENIMAGES 8
#define OPACITYDIVIDERSPEED 0.001

enum SET{
	NOMENU,
	MAINMENU,
	FIND_SERVER,
	OPTIONS,
	GETIPADRESS,
	QUIT,
	LASTMENU
};

class MainMenu 
{
public:
	MainMenu();
	virtual ~MainMenu();

	void Init();
	void Run();

private:
	void PrintClientError(const int code);
	/*! Prints the error message to the client*/
	void PrintToScreen(const std::string msg);

	void StartTestRun();
	void StartGameWithIPField();
	void SwapMenus(SET primary, SET secondary);
	void EnableMouse(bool value);

	void AddResolutionsToDropBox(DropDownList* ddl);

	void UpdateOptionsMenu();
	void UpdateBackground(float dt);

	void Resize();
private:
	GUISet*	zSets;
	int		zNrOfSets;
	SET		zPrimarySet;
	SET		zSecondarySet;

	IAudioManager* _sound;
	IAudioLoader* _soundLoader;
	IEventHandle* menuClick;
	IEventHandle* menuSound;

	iImage* zBGScreens[BGSCREENSHOTS];
	float zPause;
	int zCurrentImage;
	int zNextImage;

	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;

	float zSizedForWidth;
	float zSizedForHeight;
};