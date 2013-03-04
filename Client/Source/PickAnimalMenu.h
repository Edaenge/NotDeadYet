#pragma once

#include "GUI\GUISet.h"

enum SETPAM
{
	NOSET,
	MAINSET,
	BEAR,
	DEER,
	SPIRIT,
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

	float zSizedForWidth;
	float zSizedForHeight;

public:
	PickAnimalMenu();
	virtual ~PickAnimalMenu();

	int Run();

	void ToggleMenu();

	bool GetShow(){ return this->zShow; }
	void SetShow(bool set){ this->zShow = set; }

private:
	void Init();

	void Resize();
};