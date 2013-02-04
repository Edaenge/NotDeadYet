#include "GameModeFFA.h"


GameModeFFA::GameModeFFA(World* worldPtr) : GameMode(worldPtr)
{

}

GameModeFFA::~GameModeFFA()
{

}

void GameModeFFA::Update(float dt)
{
	this->zActorHandler->UpdateObjects(dt);
}

void GameModeFFA::WinningCheck()
{
	throw std::exception("The method or operation is not implemented.");
}

bool GameModeFFA::InitiGameObjects()
{
	this->InitStaticObjects();
	this->InitAnimals();

	return true;
}

void GameModeFFA::InitAnimals()
{
	Vector3 position = this->CalculateSpawnPoint(0, 4, 10.0f);
	this->zActorHandler->CreateDeerAnimal(true, position);
}

void GameModeFFA::InitStaticObjects()
{
	int maxClients = 32;

	Vector3 position;
	float radius = 3.5f;
	int numberOfObjects = 12;
	int counter = 0;
	int total = 0;


}