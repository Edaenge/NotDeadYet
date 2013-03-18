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
		SetModel("media/models/berrybush/berrybush_02.obj");
	}
	else
	{
		SetModel("media/models/berrybush/berrybush_01.obj");
	}
}