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
		SetModel("Media/Models/Cube_02.obj");
	}
	else
	{
		SetModel("Media/Models/Cube_01.obj");
	}
}