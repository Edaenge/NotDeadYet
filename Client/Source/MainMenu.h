//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.
#pragma once

#include "Game.h"
#include "GUI/GUISet.h"

static const unsigned int BGSCREENSHOTS = 8;
static const unsigned int PAUSEBETWEENIMAGES = 8;
static const float OPACITYDIVIDERSPEED = 0.001f;

enum SET
{
	NOMENU,
	MAINMENU,
	FIND_SERVER,
	OPTIONS,
	GETIPADRESS,
	QUIT,
	CREDITS,
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

	//void StartTestRun();
	void StartGameWithIPField();
	void SwapMenus(SET primary, SET secondary);
	void EnableMouse(bool value);

	void AddResolutionsToDropBox(DropDownList* ddl);

	void UpdateOptionsMenu();
	void UpdateBackground(float dt);

	void Resize();

	void ResizeWallpapers();
private:
	GUISet*	zSets;
	int		zNrOfSets;
	SET		zPrimarySet;
	SET		zSecondarySet;

	IAudioManager* _sound;
	IAudioLoader* _soundLoader;
	IEventHandle* menuClick;
	IEventHandle* menuSound;

	iImage* zBGScreens[BGSCREENSHOTS+1];
	float zPause;
	int zCurrentImage;
	int zNextImage;

	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;

	float zSizedForWidth;
	float zSizedForHeight;
};