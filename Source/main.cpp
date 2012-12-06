#include "Graphics.h"
#include "SoundEngine.h"
#include "MainMenu.h"
//#include <vld.h>

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
	menu->run();
	delete menu;

	// Free Graphics
	FreeGraphics();
	
	return 0;
}


void test()
{
	// Create Skybox
	GetGraphics()->CreateSkyBox("Media/skymap.dds");

	GetGraphics()->GetCamera()->SetPosition( Vector3(0, 15, -15.6f) );
	GetGraphics()->GetCamera()->LookAt( Vector3(30, 10, 10));

	bool go = true;

	GetGraphics()->StartRendering();
	while(GetGraphics()->IsRunning() && go)	// Returns true as long as ESC hasnt been pressed, if it's pressed the game engine will shut down itself (to be changed)
	{
		GetGraphics()->Update();	// Updates camera etc, does NOT render the frame, another process is doing that, so diff should be very low.
	}
}