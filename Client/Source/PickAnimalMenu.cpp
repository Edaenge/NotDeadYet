#include "PickAnimalMenu.h"

#define YOURENERGYTEXT "YOUR ENERGY: "

PickAnimalMenu::PickAnimalMenu()
{
	this->zEng			= GetGraphics();
	zNrOfSets			= 5;	
	zSets				= new GUISet[zNrOfSets];
	this->zPrimarySet	= MAINSET;

	this->zShow			= false;

	this->zEnergyText	= NULL;

	this->Init();
}

PickAnimalMenu::~PickAnimalMenu()
{
	delete [] zSets;
	zSets = 0;
}


int PickAnimalMenu::Run(int energy)
{
	if(this->zEnergyText)
		this->zEnergyText->SetText((YOURENERGYTEXT + MaloW::convertNrToString(energy) + "\200").c_str());

	this->Resize();
	int returnValue = PICKNOTHING;
	bool removeMenu = false;

	Vector2 mousePos = this->zEng->GetKeyListener()->GetMousePosition();
	bool mousePressed = this->zEng->GetKeyListener()->IsClicked(1);
	GUIEvent* guiEvent = this->zSets[this->zPrimarySet].UpdateAndCheckCollision(mousePos.x, mousePos.y, mousePressed, this->zEng);
	if(guiEvent != NULL)
	{
		if(guiEvent->GetEventMessage() == "ChangeSetEvent")
		{
			ChangeSetEvent* cse = (ChangeSetEvent*) guiEvent;

			if(cse->GetSet() == BEAR)
			{
				returnValue = BEAR;
			}
			if(cse->GetSet() == DEER)
			{
				returnValue = DEER;
			}
			
		}
	}
	else
	{
		if(this->zEng->GetKeyListener()->IsPressed('1'))
			returnValue = SPIRIT;
		else if(this->zEng->GetKeyListener()->IsPressed('2'))
			returnValue = DEER;
		else if(this->zEng->GetKeyListener()->IsPressed('3'))
			returnValue = BEAR;
	}

	return returnValue;
}

void PickAnimalMenu::ToggleMenu()
{
	if(!this->zShow)
	{
		this->zSets[this->zPrimarySet].AddSetToRenderer(this->zEng);
		if(!this->zEnergyText)
			this->zEnergyText = this->zEng->CreateText("0", this->zTextPos, 2.0f, "Media/Fonts/new");
		this->zShow = true;
	}
	else
	{
		this->zSets[this->zPrimarySet].RemoveSetFromRenderer(this->zEng);
		if(this->zEnergyText)
			this->zEng->DeleteText(this->zEnergyText);
		this->zEnergyText = NULL;
		this->zShow = false;
	}
}

void PickAnimalMenu::Init()
{
	float windowWidth = (float)this->zEng->GetEngineParameters().WindowWidth;
	float windowHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	this->zSizedForWidth = windowWidth;
	this->zSizedForHeight = windowHeight;

	this->zTextPos = Vector2(offSet + (338.0f / 1024.0f) * dx, (287.0f / 768.0f) * windowHeight);

	Element* temp = new GUIPicture(offSet + (100.0f / 1024.0f) * dx, (50.0f / 768.0f) * windowHeight, 1.0f, 
		"Media/Menu/UI/PickAnimalBG.png", (824.0f / 1024.0f) * dx, (668.0f / 768.0f) * windowHeight);
	this->zSets[MAINSET].AddElement(temp);

	float buttonSize = (150.0f / 768.0f) * windowHeight;

	temp = new SimpleButton(offSet + (220.0f / 1024.0f) * dx, (325.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/UI/PickSpirit.png", 
		buttonSize, buttonSize, new ChangeSetEvent(SPIRIT), "", "", offSet + (210.0f / 1024.0f) * dx, 
		(325.0f / 768.0f) * windowHeight, buttonSize, buttonSize);
	zSets[MAINSET].AddElement(temp);

	temp = new SimpleButton(offSet + (420.0f / 1024.0f) * dx, (325.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/UI/PickDeer.png", 
		buttonSize, buttonSize, new ChangeSetEvent(DEER), "", "", offSet + (410.0f / 1024.0f) * dx, 
		(325.0f / 768.0f) * windowHeight, buttonSize, buttonSize);
	zSets[MAINSET].AddElement(temp);

	temp = new SimpleButton(offSet + (620.0f / 1024.0f) * dx, (325.0f / 768.0f) * windowHeight, 1.0f, "Media/Menu/UI/PickBear.png", 
		buttonSize, buttonSize, new ChangeSetEvent(BEAR), "", "", offSet + (610.0f / 1024.0f) * dx, 
		(325.0f / 768.0f) * windowHeight, buttonSize, buttonSize);
	zSets[MAINSET].AddElement(temp);

	this->zPrimarySet = MAINSET;
}

void PickAnimalMenu::Resize()
{
	if(this->zSizedForWidth == this->zEng->GetEngineParameters().WindowWidth &&
		this->zSizedForHeight == this->zEng->GetEngineParameters().WindowHeight)
		return;

	float windowWidth = (float)this->zEng->GetEngineParameters().WindowWidth;
	float windowHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
	float dx = ((float)windowHeight * 4.0f) / 3.0f;
	float offSet = (float)(windowWidth - dx) / 2.0f;

	this->zSets[MAINSET].Resize(this->zSizedForWidth, this->zSizedForHeight, this->zEng->GetEngineParameters().WindowWidth, this->zEng->GetEngineParameters().WindowHeight);

	this->zTextPos = Vector2(offSet + (338.0f / 1024.0f) * dx, (287.0f / 768.0f) * windowHeight);

	this->zSizedForWidth = (float)this->zEng->GetEngineParameters().WindowWidth;
	this->zSizedForHeight = (float)this->zEng->GetEngineParameters().WindowHeight;
}
