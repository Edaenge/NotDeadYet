//Made by Edänge Simon 
//for project Not Dead Yet at Blekinge tekniska högskola.
#pragma once

#include "Game.h"
#include "GUI/GUISet.h"

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
	void SwapMenus(SET primary, SET secondary);
	void EnableMouse(bool value);

	void AddResolutionsToDropBox(DropDownList* ddl);

	void UpdateOptionsMenu();

	void Resize();
private:
	GUISet*	zSets;
	int		zNrOfSets;
	SET		zPrimarySet;
	SET		zSecondarySet;

	Game* zGame;
	bool zClientConnected;
	bool zHostCreated;

	float zSizedForWidth;
	float zSizedForHeight;
};