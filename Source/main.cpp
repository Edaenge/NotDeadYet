#include "stdafx.h"
#include "GraphicsEngine.h"
#include "SoundEngine\SoundEngine.h"
#include "GameFiles\MainMenu.h"


void test();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow)
{
#if defined(DEBUG) || defined(_DEBUG)
	myInitMemoryCheck();
#endif
	//_CrtSetBreakAlloc(3795052);

	MaloW::ClearDebug();
	
	GraphicsEngineParams params;
	params.LoadFromeFile("config.cfg");
	
	// Create the graphics engine
	GraphicsEngine* ge = new GraphicsEngine(params, hInstance, nCmdShow);
	gfxeng::eng = ge; // Set the global eng to our engine so that GetGraphicsEngine(); can work.
	ge->CreateSkyBox("Media/skymap.dds");
	
	test();	// tester

	// IMPLEMENT MAIN PROGRAM HERE.
	//MainMenu menu = MainMenu();

	delete ge;
	gfxeng::eng = NULL;

	#if defined(DEBUG) || defined(_DEBUG)
		myDumpMemoryLeaks();
	#endif
	
	return 0;
}




void test()
{
	// Example of GE usage
	GraphicsEngine* eng = GetGraphicsEngine();
	eng->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6f));
	eng->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));

	Terrain* terrain = eng->CreateTerrain(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(100, 1, 100), "Media/TerrainTexture.png", "Media/TerrainHeightmap.raw");
	StaticMesh* scaleShrubbery1 = eng->CreateStaticMesh("Media/Fern_02_v01.obj", D3DXVECTOR3(15, terrain->getYPositionAt(15, 15), 15));
	StaticMesh* scaleShrubbery2 = eng->CreateStaticMesh("Media/bTest.obj", D3DXVECTOR3(15, terrain->getYPositionAt(15, 15), 35));
	
	//scaleHuman->UseInvisibilityEffect(true);
	//StaticMesh* bth = eng->CreateStaticMesh("Media/bth.obj", D3DXVECTOR3(5, 20, 15));
	//scaleHuman->Scale(1.0f);
	Light* testLight = eng->CreateLight(D3DXVECTOR3(15, 50, 35));
	testLight->SetIntensity(50.0f);

	SoundEngine* seng = eng->GetSoundEngine();
	seng->SetMasterVolume(0.5f);
	//SoundSong* ss1 = seng->LoadSong("Media/Sounds/Songs/america_fuck_yeah.mp3", true);
	//ss1->Play();
	
	//eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");			// going to LoadingScreen to load the above meshes
	//eng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 0.0f, 1.0f, 1.0f, 1.0f);
	
	eng->LoadingScreen("Media/LoadingScreen/StartScreen.png", "", 0.0f, 1.0f, 1.0f, 1.0f);
	//eng->StartRendering();
	Text* text = eng->CreateText("Lol ", D3DXVECTOR2(500, 500), 1.0f, "Media/Fonts/1");
	
	bool sw = true;
	float size = 1.0f;

	bool go = true;
		
	while(eng->isRunning() && go)	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		//Sleep(1);
		float diff = eng->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.
		CursorControl cc;

		if(eng->GetKeyListener()->IsPressed('W'))
			eng->GetCamera()->moveForward(diff);
		if(eng->GetKeyListener()->IsPressed(VK_RETURN))	// For keys other than the main-chars you use the VK_ Enums, rightclick on VK_RETURN and "Go to definition" to find the list of all keys
			cc.SetVisibility(true);
		if(eng->GetKeyListener()->IsPressed('A'))
			eng->GetCamera()->moveLeft(diff);
		if(eng->GetKeyListener()->IsPressed('S'))	
			eng->GetCamera()->moveBackward(diff);
		if(eng->GetKeyListener()->IsPressed('D'))	
			eng->GetCamera()->moveRight(diff);

		if(eng->GetKeyListener()->IsPressed(VK_ESCAPE))
			go = false;

		if(eng->GetKeyListener()->IsPressed(VK_BACK))
		{
			if(sw)
			{
				//se1->Play();
				text->DeleteFromEnd(1);
			}
			sw = false;
		}
		else
			sw = true;
		
		if(eng->GetKeyListener()->IsClicked(1))
		{
			size += diff * 0.001f;
			text->SetSize(size);
			text->AppendText("LoL ");
		}
	}
}