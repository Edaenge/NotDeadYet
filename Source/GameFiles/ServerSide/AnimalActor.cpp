#include "GameFiles/ServerSide/AnimalActor.h"

AnimalActor::AnimalActor(const int ID) : BioActor()
{

}

AnimalActor::AnimalActor( const int ID, const Vector3& startPos ) : BioActor(startPos)
{

}

AnimalActor::AnimalActor( const int ID, const Vector3& startPos, const Vector4& rot ) : BioActor(startPos, rot)
{

}

AnimalActor::~AnimalActor()
{

}

void AnimalActor::Update()
{

}

