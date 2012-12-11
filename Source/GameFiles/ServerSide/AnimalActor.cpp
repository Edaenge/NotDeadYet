#include "GameFiles/ServerSide/AnimalActor.h"

AnimalActor::AnimalActor(const int ID) : BioActor(ID)
{

}

AnimalActor::AnimalActor( const int ID, const Vector3& startPos ) : BioActor(ID, startPos)
{

}

AnimalActor::AnimalActor( const int ID, const Vector3& startPos, const Vector4& rot ) : BioActor(ID, startPos, rot)
{

}

AnimalActor::~AnimalActor()
{

}

void AnimalActor::Update()
{

}

