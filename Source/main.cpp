#include "Graphics.h"
#include "..\PhysicsEngine\Physics.h"
#include "SoundEngine.h"
#include "MainMenu.h"
#include <vld.h>

void test();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) 
{
	MaloW::ClearDebug();

	if ( !GraphicsInit(hInstance) )
	{
		MaloW::Debug("Failed Initializing Graphics!");
		return 1;
	}
	// Tester
	//test();
	
	// IMPLEMENT MAIN PROGRAM HERE.
	MainMenu *menu = new MainMenu();
	menu->Init();
	menu->Run();
	delete menu;

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