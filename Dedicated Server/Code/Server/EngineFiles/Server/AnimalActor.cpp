#include "AnimalActor.h"


AnimalActor::AnimalActor( bool genID /*= true*/ ) : BioActor()
{
	if(genID)
		this->GenerateID();

}

AnimalActor::AnimalActor( const Vector3& startPos, bool genID /*= true*/ ) : BioActor(startPos)
{
	if(genID)
		this->GenerateID();
}

AnimalActor::AnimalActor( const Vector3& startPos, const Vector4& rot, bool genID /*= true*/ ) : BioActor(startPos, rot)
{
	if(genID)
		this->GenerateID();
}

AnimalActor::~AnimalActor()
{

}

void AnimalActor::Update( float deltaTime )
{

}

