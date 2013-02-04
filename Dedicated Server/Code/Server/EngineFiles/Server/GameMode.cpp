#include "GameMode.h"
#include "Safe.h"

GameMode::GameMode(World* worldPtr)
{
	this->zWorld = worldPtr;
	this->zGameRunning = false;
	this->zActorHandler = new ActorHandler(worldPtr);
}

GameMode::~GameMode()
{

}

void GameMode::RestartGame()
{

}

Vector3 GameMode::CalculateSpawnPoint(const int point, const int maxPoints, float radius, Vector3 center)
{
	static const float PI = 3.14159265358979323846f;

	float slice  = 2 * PI / maxPoints;

	float angle = slice * point;

	float x = center.x + radius * cos(angle);
	float z = center.z + radius * sin(angle);
	float y = 0.0f;

	if ( x >= 0.0f && y >= 0.0f && x < zWorld->GetWorldSize().x && y < zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

void GameMode::CreatePlayer(long player_ID, const std::vector<std::string> &data)
{

}

void GameMode::HandleKeyPress(long player_ID, const int key)
{
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActorHandler->GetActor(player_ID, ACTOR_TYPE_PLAYER));

	if (!pActor)
		throw exception("Player doesn't exist");	

	pActor->SetKeyState(key, true);
}

void GameMode::HandleKeyRelease(long player_ID, const int key)
{
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActorHandler->GetActor(player_ID, ACTOR_TYPE_PLAYER));

	if (!pActor)
		throw exception("Player doesn't exist");

	pActor->SetKeyState(key, false);
}