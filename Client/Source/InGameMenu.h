#pragma once

#include "GUI\GUISet.h"
#include "Sounds.h"

enum SETIG
{
	IGNOMENU,
	INGAMEMAIN,
	IGOPTIONS,
	IGLASTMENU,
	IGQUIT,
	IGRESUME,
	IGNOTHING
};

class InGameMenu
{
private:
	GUISet*	zSets;
	int		zNrOfSets;
	int		zPrimarySet;
	GraphicsEngine* zEng;

	bool zShow;

public:
	InGameMenu();
	virtual ~InGameMenu();

	int Run();

	void ToggleMenu();

private:
	void Init();

	void AddResolutionsToDropBox(DropDownList* ddl);

	void UpdateOptionsMenu();

	void SwapMenus(int primary);
};