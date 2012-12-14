#include "GameFiles/ServerSide/AnimalActor.h"

AnimalActor::AnimalActor() : BioActor()
{

}

AnimalActor::AnimalActor( const Vector3& startPos ) : BioActor(startPos)
{

}

AnimalActor::AnimalActor( const Vector3& startPos, const Vector4& rot ) : BioActor(startPos, rot)
{

}

AnimalActor::~AnimalActor()
{

}

void AnimalActor::Update(float deltaTime)
{

}

