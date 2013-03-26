#include "CreditsMenu.h"

CreditsMenu::CreditsMenu()
{
	//this->camRec = CamRecording();
}

CreditsMenu::~CreditsMenu()
{
	
}

void CreditsMenu::PreRun()
{
	this->camRec.Init(GetGraphics()->GetCamera());
	//this->camRec.Open("Media/camPathCredits");

	GetGraphics()->SetGrassFilePath("Media/GrassGY.png");
	
	GetGraphics()->ShowLoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 1.0f, 1.0f);
	GetGraphics()->ChangeSkyBox("Media/StarMap.dds");
	this->previousFarClip = GetGraphics()->GetEngineParameters().FarClip;
	GetGraphics()->GetEngineParameters().FarClip = 300;
	GetGraphics()->GetCamera()->SetUpdateCamera(true);
	GetGraphics()->GetKeyListener()->SetCursorVisibility(false);
	GetGraphics()->SetFogColor(Vector3(0.1f, 0.1f, 0.1f));

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
	GetGraphics()->SetEnclosingFogEffect(fogCenter, 35, 0.3f);


	// Create lights
	Vector3 LightColor = Vector3(0.1f, 0.095f, 0.0f);
	float yoffset = 5.0f;
	float intensity = 13.0f;
	l1 = GetGraphics()->CreateLight(Vector3(51, 3.25 + yoffset, 39.8));
	l1->SetLookAt(l1->GetPosition() - Vector3(0, 1, 0));
	l1->SetUp(Vector3(1, 0, 0));
	l1->SetIntensity(intensity);
	l1->SetColor(LightColor);

	l2 = GetGraphics()->CreateLight(Vector3(40.3, 3.2 + yoffset, 43));
	l2->SetLookAt(l2->GetPosition() - Vector3(0, 1, 0));
	l2->SetUp(Vector3(1, 0, 0));
	l2->SetIntensity(intensity);
	l2->SetColor(LightColor);
	
	l3 = GetGraphics()->CreateLight(Vector3(41.3, 3.2 + yoffset, 61));
	l3->SetLookAt(l3->GetPosition() - Vector3(0, 1, 0));
	l3->SetUp(Vector3(1, 0, 0));
	l3->SetIntensity(intensity);
	l3->SetColor(LightColor);

	l4 = GetGraphics()->CreateLight(Vector3(55.6, 3.3 + yoffset, 57.3));
	l4->SetLookAt(l4->GetPosition() - Vector3(0, 1, 0));
	l4->SetUp(Vector3(1, 0, 0));
	l4->SetIntensity(intensity);
	l4->SetColor(LightColor);

	selfLight = GetGraphics()->CreateLight(Vector3(0, 0, 0));
	selfLight->SetIntensity(4.0f);
	selfLight->SetColor(Vector3(0, 0, 0));


	GetGraphics()->SetSunLightDisabled();
	
	GetGraphics()->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 1.0f, 1.0f, 1.0f, 1.0f);

	GetGraphics()->GetCamera()->SetPosition(Vector3(20, 0, 20));
	this->mode = 0;
}

void CreditsMenu::Run()
{
	this->PreRun();
	bool go = true;
	bool cameraPathDone = false;
	//this->camRec.Play(); //Start playing record
	while(go)
	{
		Sleep(5);
		float diff = GetGraphics()->Update();
		world->Update();
		wr->Update();
		selfLight->SetPosition(GetGraphics()->GetCamera()->GetPosition());

		if(GetGraphics()->GetKeyListener()->IsPressed(VK_ESCAPE))
			go = false;


		if(this->mode == 0)
		{
			//cameraPathDone = camRec.Play();	////////
			//this->camRec.Update(diff);

			//if(!this->camRec.IsPlaying())
			//{
			//	this->mode++;
			//}
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

			if(GetGraphics()->GetKeyListener()->IsPressed('R'))	
				this->camRec.Record(true);
			if(GetGraphics()->GetKeyListener()->IsPressed('E'))	
				this->camRec.Record(false);
			if(GetGraphics()->GetKeyListener()->IsPressed('V'))	
				this->camRec.Save("creditsCamPath");
			if(GetGraphics()->GetKeyListener()->IsPressed('B'))	
				this->camRec.Open("creditsCamPath");
			if(GetGraphics()->GetKeyListener()->IsPressed('N'))	
				this->camRec.Play();

			this->camRec.Update(diff);
		}
	}
	this->PostRun();
}

void CreditsMenu::PostRun()
{
	delete this->world;
	GetGraphics()->GetEngineParameters().FarClip = this->previousFarClip;
	GetGraphics()->GetCamera()->SetUpdateCamera(false);
	GetGraphics()->GetKeyListener()->SetCursorVisibility(true);
	GetGraphics()->SetGrassFilePath("Media/Grass.png");
	GetGraphics()->ChangeSkyBox("Media/skymap.dds"); 
	GetGraphics()->SetFogColor(Vector3(0.45f, 0.45f, 0.45f));

	GetGraphics()->DeleteLight(l1);
	GetGraphics()->DeleteLight(l2);
	GetGraphics()->DeleteLight(l3);
	GetGraphics()->DeleteLight(l4);
	GetGraphics()->DeleteLight(selfLight);
}