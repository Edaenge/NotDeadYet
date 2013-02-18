#pragma once

#include "GUI\GUISet.h"

enum SETPAM
{
	NOSET,
	MAINSET,
	BEAR,
	DEER,
	PICKNOTHING
};

class PickAnimalMenu
{
private:
	GUISet*	zSets;
	int		zNrOfSets;
	int		zPrimarySet;
	GraphicsEngine* zEng;

	bool zShow;

public:
	PickAnimalMenu();
	virtual ~PickAnimalMenu();

	int Run();

	void ToggleMenu();

	bool GetShow(){ return this->zShow; }
	void SetShow(bool set){ this->zShow = set; }

private:
	void Init();
};