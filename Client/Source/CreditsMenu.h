#pragma once

#include "Graphics.h"
#include <MaloWFileDebug.h>
#include "World\EntityList.h"
#include "World\Observer.h"
#include "World\World.h"
#include "World\WorldRenderer.h"
#include "CamRecording.h"

class CreditsMenu : public Observer
{
private:
	World* world;
	WorldAnchor* wa;
	WorldRenderer* wr;
	int mode;
	CamRecording camRec;

public:
	CreditsMenu();
	virtual ~CreditsMenu();

	void PreRun();
	void Run();
	void PostRun();

protected:
	virtual void OnEvent(Event* e) 
	{
		if ( WorldDeletedEvent* wde = dynamic_cast<WorldDeletedEvent*>(e))
		{
			delete this->wr;
			this->wr = NULL;
		}
		if ( WorldLoadedEvent* WLE = dynamic_cast<WorldLoadedEvent*>(e) )
		{
			// Create Anchor
			wa = WLE->world->CreateAnchor();
			this->wr = new WorldRenderer(WLE->world, GetGraphics());
			this->wr->SetGrassTexturePath("Media/GrassGY.png");
		}
	}
};