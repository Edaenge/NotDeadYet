#include "Graphics.h"
#include "..\PhysicsEngine\Physics.h"
#include "SoundEngine.h"
#include "MainMenu.h"
#if defined(DEBUG) || defined(_DEBUG)
	#include <vld.h>
	#define INCLUDE_MODEL_VIEWER
#endif


void test();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) 
{
	MaloW::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): NotDeadYet: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): NotDeadYet: vld.h included.");
#endif

	if ( !GraphicsInit(hInstance) )
	{
		MaloW::Debug("Failed Initializing Graphics!");
		return 1;
	}
	/*int** memLeakTest = new int*[100];
	for(int i = 0; i < 100; i++)
	{
		memLeakTest[i] = new int(1);
	}*/
	// Tester
	//test();
	
	// IMPLEMENT MAIN PROGRAM HERE.
	MainMenu* menu = new MainMenu();
	menu->Init();
	menu->Run();
	delete menu;
	Sleep(10);

	// Free Graphics
	FreeGraphics();
	
	return 0;
}

void test()
{
	PhysicsInit();
	// Create Skybox
	GetGraphics()->CreateSkyBox("Media/skymap.dds");

	//GetGraphics()->GetCamera()->SetPosition( Vector3(0, 15, -15.6f) );
	//GetGraphics()->GetCamera()->LookAt( Vector3(30, 10, 10));
	PhysicsEngine* peng = GetPhysics();
	PhysicsObject* obj = peng->CreatePhysicsObject("Media/scale.obj", Vector3(0, 0, 0));
	PhysicsObject* obj2 = peng->CreatePhysicsObject("Media/bth.obj", Vector3(15, 0, 0));

	bool go = true;

	GetGraphics()->StartRendering();
	while(GetGraphics()->IsRunning() && go)	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		GetGraphics()->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.
		PhysicsCollisionData pcd = GetPhysics()->GetCollisionRayMesh(GetGraphics()->GetCamera()->GetPosition(), 
			GetGraphics()->GetCamera()->GetForward(), obj);

		if(pcd.collision)
			MaloW::Debug("Collision!");
	}

	GetPhysics()->DeletePhysicsObject(obj);
	GetPhysics()->DeletePhysicsObject(obj2);

	FreePhysics();
}