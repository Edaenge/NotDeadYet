#include "Graphics.h"
#include "SoundEngine.h"
#include "MainMenu.h"
#if defined(DEBUG) || defined(_DEBUG)
	#include <vld.h>
	#define INCLUDE_MODEL_VIEWER
#endif


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
	SAFE_DELETE(menu);

	// Free Graphics
	try
	{
		FreeGraphics();
	}
	catch(...)
	{

	}

	return 0;
}