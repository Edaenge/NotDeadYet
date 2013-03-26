#pragma once

#include "GUI\GUISet.h"
#include "sounds.h"
#include "AudioManager.h"

enum SETIG
{
	IGNOMENU,
	INGAMEMAIN,
	IGOPTIONS,
	IGLASTMENU,
	IGQUIT,
	IGRESUME,
	IGNOTHING,
	IGRESIZE
};

class InGameMenu
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
	InGameMenu();
	virtual ~InGameMenu();

	int Run();

	void ToggleMenu();

	bool GetShow(){ return this->zShow; }
	void SetShow(bool set){ this->zShow = set; }

	Vector2 GetScreenDim(){ return Vector2(this->zSizedForWidth, this->zSizedForHeight); }

private:
	void Init();

	void AddResolutionsToDropBox(DropDownList* ddl);

	void UpdateOptionsMenu();

	void SwapMenus(int primary);

	void Resize();
};