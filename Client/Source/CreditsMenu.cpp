#include "CreditsMenu.h"

CreditsMenu::CreditsMenu()
{

}

CreditsMenu::~CreditsMenu()
{

}

void CreditsMenu::PreRun()
{
	GetGraphics()->SetGrassFilePath("Media/GrassGY.png");
	
	GetGraphics()->ShowLoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 1.0f, 1.0f);

	GetGraphics()->GetCamera()->SetUpdateCamera(true);
	GetGraphics()->GetKeyListener()->SetCursorVisibility(false);


	unsigned int nrOfResources = 6;
	const char* resourceFileNames[6];
	resourceFileNames[0] = "Media/MapModels/Tree_02.ani";
	resourceFileNames[1] = "Media/MapModels/Tree_02_01.obj";
	resourceFileNames[2] = "Media/MapModels/Bush_01.ani";
	resourceFileNames[3] = "Media/MapModels/Bush_01_01.obj";
	resourceFileNames[4] = "Media/MapModels/Tree_01.ani";
	resourceFileNames[5] = "Media/MapModels/Tree_01_01.obj";
	GetGraphics()->PreLoadResources(nrOfResources, resourceFileNames);

	LoadEntList("Entities.txt");
	world = new World(this, "Media/Maps/GraveYardMap.map", true);

	wa->position = world->GetWorldCenter();
	wa->radius = 200;
	world->Update();
	while(wr->Update()) {}



	Vector3 fogCenter = Vector3(world->GetWorldCenter().x, 0, world->GetWorldCenter().y);
	GetGraphics()->SetEnclosingFogEffect(fogCenter, 40, 0.2f);

	
	GetGraphics()->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 1.0f, 1.0f, 1.0f, 1.0f);

	GetGraphics()->GetCamera()->SetPosition(Vector3(20, 0, 20));
	this->mode = 0;
}

void CreditsMenu::Run()
{
	this->PreRun();
	bool go = true;
	bool cameraPathDone = false;
	while(go)
	{
		Sleep(5);
		float diff = GetGraphics()->Update();
		world->Update();
		wr->Update();

		if(GetGraphics()->GetKeyListener()->IsPressed(VK_ESCAPE))
			go = false;


		if(this->mode == 0)
		{
			cameraPathDone = true;	////////


			if(cameraPathDone)
				this->mode++;
		}
		else if(this->mode == 1)
		{
			// Free flying:
			if(GetGraphics()->GetKeyListener()->IsPressed('W'))
				GetGraphics()->GetCamera()->MoveForward(diff * 10.0f);
			if(GetGraphics()->GetKeyListener()->IsPressed('A'))
				GetGraphics()->GetCamera()->MoveLeft(diff * 10.0f);
			if(GetGraphics()->GetKeyListener()->IsPressed('S'))	
				GetGraphics()->GetCamera()->MoveBackward(diff * 10.0f);
			if(GetGraphics()->GetKeyListener()->IsPressed('D'))	
				GetGraphics()->GetCamera()->MoveRight(diff * 10.0f);
			if(GetGraphics()->GetKeyListener()->IsPressed(VK_ADD))
				GetGraphics()->GetCamera()->SetSpeed(GetGraphics()->GetCamera()->GetSpeed() * (1.0f + diff * 0.01f));
			if(GetGraphics()->GetKeyListener()->IsPressed(VK_SUBTRACT))
				GetGraphics()->GetCamera()->SetSpeed(GetGraphics()->GetCamera()->GetSpeed() * (1.0f - diff * 0.01f));
			if(GetGraphics()->GetKeyListener()->IsPressed(VK_SPACE))
				GetGraphics()->GetCamera()->MoveUp(diff * 10.0f);
			if(GetGraphics()->GetKeyListener()->IsPressed(VK_CONTROL))
				GetGraphics()->GetCamera()->MoveDown(diff * 10.0f);
		}
	}
	this->PostRun();
}

void CreditsMenu::PostRun()
{
	delete this->world;
}