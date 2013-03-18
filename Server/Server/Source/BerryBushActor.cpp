#include "BerryBushActor.h"


BerryBushActor::BerryBushActor()
{
	SetPicked(false);
}

BerryBushActor::~BerryBushActor()
{

}

void BerryBushActor::SetPicked( bool pickedFlag ) 
{
	zPicked = pickedFlag;
	
	if ( zPicked )
	{
		SetModel("Media/Models/BerryBush/BerryBush_02.obj");
	}
	else
	{
		SetModel("Media/Models/BerryBush/BerryBush_01.obj");
	}
}