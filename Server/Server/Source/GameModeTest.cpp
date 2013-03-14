#include "Game.h"
#include "time.h"
#include "Physics.h"
#include "GameModeTest.h"
#include "BioActor.h"
#include "PlayerActor.h"
#include "AnimalActor.h"
#include "GhostActor.h"
#include "GameEvents.h"
#include "PlayerHumanBehavior.h"
#include "Physics.h"
#include <Packets/PhysicalConditionPacket.h>

GameModeTest::GameModeTest(Game* game, int killLimit) : GameMode(game)
{
	srand((unsigned int)time(0));
	zKillLimit = killLimit;
}

GameModeTest::~GameModeTest()
{
	this->zGame->RemoveObserver(this);
}

bool GameModeTest::Update( float dt )
{
	for(auto it = zPlayers.begin(); it != zPlayers.end(); it++)
	{
		if(zScoreBoard[(*it)] >= zKillLimit)
		{
			for(auto i = zPlayers.begin(); i != zPlayers.end(); i++)
			{
				MaloW::Debug("Kills: " + MaloW::convertNrToString((float)zScoreBoard[(*i)]));
			}
			return false;
		}
	}
	return false;
}

void GameModeTest::OnEvent( Event* e )
{
	if( BioActorTakeDamageEvent* ATD = dynamic_cast<BioActorTakeDamageEvent*>(e) )
	{
		if( PlayerActor* pActor = dynamic_cast<PlayerActor*>(ATD->zActor) )
		{
			if(pActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
			{
				//Add to scoreboard
				if( PlayerActor* dpa = dynamic_cast<PlayerActor*>(ATD->zDealer) )
					if(dpa != pActor)
						zScoreBoard[dpa->GetPlayer()]++;
					else if( dpa == pActor )
						zScoreBoard[pActor->GetPlayer()]--;

				NetworkMessageConverter NMC;
				std::string killsMsg = "";
				std::string msg = "";
				Player* player = pActor->GetPlayer();
				if (ATD->zActor == ATD->zDealer)
				{
					killsMsg = "You killed yourself";
					std::string msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}
				else if (PlayerActor* pDealer = dynamic_cast<PlayerActor*>(ATD->zDealer))
				{
					Player* dealer = pDealer->GetPlayer();
					killsMsg = "You were killed by " + dealer->GetPlayerName();
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);

					killsMsg = "You killed " + player->GetPlayerName();
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					dealer->GetClientData()->Send(msg);

				}
				else if (AnimalActor* pDealer =dynamic_cast<AnimalActor*>(ATD->zDealer))
				{
					killsMsg = "You were killed by an Animal";
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}
				else
				{
					killsMsg = "You Were killed by the environment";
					msg = NMC.Convert(MESSAGE_TYPE_SERVER_ANNOUNCEMENT, killsMsg);
					player->GetClientData()->Send(msg);
				}

				OnPlayerDeath(pActor);
			}
			else
			{
				unsigned int ID = ATD->zDealer->GetID();
				float damage = ATD->zDamage->GetTotal();

				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_ACTOR_TAKE_DAMAGE, (float)ID);
				msg += NMC.Convert(MESSAGE_TYPE_HEALTH, damage);
				Player* player = pActor->GetPlayer();
				if (player)
				{
					ClientData* cd = player->GetClientData();

					if (cd)
					{
						std::stringstream ss;
						ss << "You did " << damage << " to your target";
						cd->Send(msg);
						cd->Send(NMC.Convert(MESSAGE_TYPE_ERROR_MESSAGE, ss.str()));
					}
				}
			}
		}
		else if( AnimalActor* aActor = dynamic_cast<AnimalActor*>(ATD->zActor) )
		{
			if(aActor->GetHealth() - ATD->zDamage->GetTotal() <= 0)
			{
				this->zGame->GetActorManager()->RemoveBehavior(aActor, false);
			}
		}
	}
	else if( PlayerAddEvent* PAE = dynamic_cast<PlayerAddEvent*>(e) )
	{
		this->zScoreBoard[PAE->player] = 0;
		zPlayers.insert(PAE->player);
	}
	else if( PlayerRemoveEvent* PRE = dynamic_cast<PlayerRemoveEvent*>(e) )
	{
		this->zScoreBoard.erase(PRE->player);
		auto playerBehavior = PRE->player->GetBehavior();
		if (playerBehavior)
		{
			Actor* actor = playerBehavior->GetActor();

			this->zGame->GetActorManager()->RemoveActor(actor);
		}
		this->zPlayers.erase(PRE->player);
	}
}

void GameModeTest::OnPlayerDeath(PlayerActor* pActor)
{
	NetworkMessageConverter NMC;
	std::string msg;

	Player* player = pActor->GetPlayer();
	pActor->SetState(STATE_IDLE);
	player->GetKeys().ClearStates();
	//Remove Player Pointer From the Actor
	pActor->SetPlayer(NULL);
	pActor->GetInventory()->UnEquipAll();

	ClientData* cd = player->GetClientData();

	//Create new Player
	int maxPlayers = zPlayers.size();
	int rand = 1 + (std::rand() % (maxPlayers+1));
	Vector3 position = this->zGame->CalcPlayerSpawnPoint(rand);

	Vector3 direction = pActor->GetDir();
	std::string model = pActor->GetModel();

	if (model.length() > 4)
	{
		if (model.substr(model.length() - 4) == ".fbx")
			model = "Media/Models/temp_guy.obj";
	}

	PhysicsObject* pObj = GetPhysics()->CreatePhysicsObject(model, position);
	
	PlayerActor* newPActor = new PlayerActor(player, pObj, this->zGame);
	newPActor->SetModel(pActor->GetModel());
	newPActor->SetPosition(position, false);
	newPActor->SetDir(direction, false);
	newPActor->AddObserver(this);	
	
	Vector3 offset = this->zGame->GetOffset(pActor->GetModel());
	newPActor->SetCameraOffset(offset);
	
	PhysicalConditionPacket* PCP = new PhysicalConditionPacket();

	//Gather Actor Physical Conditions
	PCP->zBleedingLevel = newPActor->GetBleeding();
	PCP->zEnergy = pActor->GetEnergy();
	PCP->zHealth = newPActor->GetHealth();
	PCP->zHunger = newPActor->GetFullness();
	PCP->zHydration = newPActor->GetHydration();
	PCP->zStamina = newPActor->GetStamina();

	//Create New Human Behavior
	PlayerHumanBehavior* pHumanBehavior = new PlayerHumanBehavior(newPActor, this->zGame->GetWorld(), player);

	this->zGame->SetPlayerBehavior(player, pHumanBehavior);

	//Tell The Client his ID and Actor Type
	ActorManager* aManager = this->zGame->GetActorManager();
	msg = NMC.Convert(MESSAGE_TYPE_SELF_ID, (float)newPActor->GetID());
	msg += NMC.Convert(MESSAGE_TYPE_ACTOR_TYPE, 1);

	cd->Send(msg);
	cd->Send(*PCP);

	delete PCP;

	aManager->AddActor(newPActor);
}