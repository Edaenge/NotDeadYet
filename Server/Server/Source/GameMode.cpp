#include "GameMode.h"
#include "Safe.h"

GameMode::GameMode(World* worldPtr)
{
	this->zWorld = worldPtr;
	this->zGameRunning = false;
	this->zActorHandler = new ActorHandler(worldPtr);
	this->zObjCreator = new ObjectCreator(this->zActorHandler->GetPhysicEnginePtr());
}

GameMode::~GameMode()
{

	SAFE_DELETE(zActorHandler);
	SAFE_DELETE(zObjCreator);
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

	if ( x >= 0.0f && z >= 0.0f && x < zWorld->GetWorldSize().x && z < zWorld->GetWorldSize().y )
	{
		y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));
	}

	return Vector3(x, y, z);
}

void GameMode::CreatePlayer( long player_ID, const std::vector<std::string> &data, NetworkMessageConverter nmc )
{
	PlayerActor* pActor = new PlayerActor(player_ID);

	for (auto it_Player = data.begin(); it_Player < data.end(); it_Player++)
	{
		char key[512];

		if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			std::string uModel = nmc.ConvertStringToSubstring(M_MESH_MODEL, (*it_Player));
			if(uModel != "")
				pActor->SetActorModel(uModel);
		}
		else if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			Vector3 uDir = nmc.ConvertStringToVector(M_DIRECTION, (*it_Player));
			pActor->SetDirection(uDir);
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			Vector3 uUp = nmc.ConvertStringToVector(M_UP, (*it_Player));
			pActor->SetUpVector(uUp);
		}
	}

	int currentPoint = this->zActorHandler->GetPlayers().size() & 32;
	Vector3 position = this->CalculateSpawnPoint(currentPoint, 32, 20);
	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(pActor->GetActorModel(), position);

	pActor->SetPhysicObject(pObj);
	pActor->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	this->zActorHandler->AddNewPlayer(pActor);
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