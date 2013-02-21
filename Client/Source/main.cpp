#include "Graphics.h"
#include "Sounds.h"
#include "MainMenu.h"
#include "PlayerConfig/PlayerSettings.h"

#if defined(DEBUG) || defined(_DEBUG)
	#include <vld.h>
	#define INCLUDE_MODEL_VIEWER
#endif

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int) 
{
	MaloW::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): Client: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): Client: vld.h included.");
#endif

	if ( !GraphicsInit(hInstance) )
	{
		MaloW::Debug("Failed Initializing Graphics!");
		return 1;
	}
	if ( !SoundsInit() )
	{
		MaloW::Debug("Failed Initializing Sounds!");
		return 1;
	}
	if ( !PlayerSettingsInit() )
	{
		MaloW::Debug("Failed Initializing PlayerSettings!");
		return 1;
	}

	// IMPLEMENT MAIN PROGRAM HERE.
	MainMenu* menu = new MainMenu();
	menu->Init();
	menu->Run();
	SAFE_DELETE(menu);

	// Free Graphics
	FreeGraphics();

	// Free Sounds
	FreeSounds();	

	// Save and Free Settings
	SavePlayerSettings();
	FreePlayerSettings();

	return 0;
}