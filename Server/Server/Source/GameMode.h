#pragma once

#include <Observer.h>

class GameMode : public Observer
{
protected:
	GameMode() {};
	virtual ~GameMode() {};
public:

	bool Update(float) { return true; };
	virtual void OnEvent(Event* e) = 0;
};