/*
Made by Edänge Simon 2012-12-11
for project Not Dead Yet* at Blekinge tekniska högskola.
*/

#pragma once

#include "../Server/AnimalActor.h"
#include <vector>

class AnimalHandler
{

public:
	AnimalHandler();
	virtual ~AnimalHandler();

	virtual void Update();
	const std::string& SpawnAnimal(const Vector3& startPos, const Vector4& rot = Vector4());


private:
	int SearchAnimal(const int ID);

private:
	std::vector<AnimalActor*> zAnimals;

};