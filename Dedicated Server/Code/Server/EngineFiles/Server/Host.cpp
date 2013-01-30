#include "Host.h"
#include "../../../../../Source/GameFiles/ClientServerMessages.h"
// 50 updates per sec
static const float UPDATE_DELAY = 0.020f;

Host::Host()
{
	MaloW::ClearDebug();
	Messages::ClearDebug();
#ifdef INCLUDE_MODEL_VIEWER
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	MaloW::Debug("(DEBUG): Server: Debug flag set to: _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF). ");
	MaloW::Debug("(DEBUG): Server: vld.h included.");
#endif

	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zMinClients = 0;
	this->zClients = std::vector<ClientData*>(); 

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
	this->zTimeOut = 15.0f;
	this->zPingMessageInterval = 5.0f;

	//timer = 0;
	this->zWorld = 0;
	zAnchorPlayerMap.clear();
}

Host::~Host()
{
	//Sends to all clients, the server is hutting down.
	BroadCastServerShutdown();
	
	this->Close();
	this->WaitUntillDone();

	this->zServerListener->Close();
	SAFE_DELETE(this->zServerListener);
	SAFE_DELETE(this->zActorHandler);

	for(auto x = zClients.begin(); x < zClients.end(); x++)
	{
		SAFE_DELETE(*x);
	}

	SAFE_DELETE(this->zWorld);
}
//NEEDS FIXING
void Host::Init()
{
	Vector3 position;
	float radius = 5.0f;
	int maxObjects = 12;
	int counter = 0;
	//Creates A New FoodObject With an Id And Default Values 
	FoodObject* foodObj = NULL; //new FoodObject(true);
	if(this->CreateStaticObjectActor(OBJECT_TYPE_FOOD_DEER_MEAT, &foodObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		foodObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticFoodActor(foodObj);
		
		if (Messages::FileWrite())
			Messages::Debug("Created Deer Meat Object ID: " + MaloW::convertNrToString((float)foodObj->GetID()));

		counter++;
	}
	foodObj = NULL; //new FoodObject(true);
	if(this->CreateStaticObjectActor(OBJECT_TYPE_FOOD_WOLF_MEAT, &foodObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		foodObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticFoodActor(foodObj);

		if (Messages::FileWrite())
			Messages::Debug("Created Wolf Meat Object ID: " + MaloW::convertNrToString((float)foodObj->GetID()));

		counter++;
	}
	//Creates A New WeaponObject With an Id And Default Values 
	WeaponObject* weaponObj = NULL;//new WeaponObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_BOW, &weaponObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		weaponObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);


		if (Messages::FileWrite())
			Messages::Debug("Created Bow Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL;//new WeaponObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_ROCK, &weaponObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		weaponObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);


		if (Messages::FileWrite())
			Messages::Debug("Created Rock Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL; //new WeaponObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_AXE, &weaponObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		weaponObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);
		
		if (Messages::FileWrite())
			Messages::Debug("Created Axe Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL; //new WeaponObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_POCKET_KNIFE, &weaponObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		weaponObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

		if (Messages::FileWrite())
			Messages::Debug("Created Pocket Knife Object ID: " + MaloW::convertNrToString((float)weaponObj->GetID()));

		counter++;
	}

	ContainerObject* containerObj = NULL; //new ContainerObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_CANTEEN, &containerObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		containerObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticContainerActor(containerObj);
		
		if (Messages::FileWrite())
			Messages::Debug("Created Canteen Object ID: " + MaloW::convertNrToString((float)containerObj->GetID()));

		counter++;
	}
	containerObj = NULL; //new ContainerObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_WATER_BOTTLE, &containerObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		containerObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticContainerActor(containerObj);

		if (Messages::FileWrite())
			Messages::Debug("Created Water Bottle Object ID: " + MaloW::convertNrToString((float)containerObj->GetID()));

		counter++;
	}
	StaticProjectileObject* projectileObj = NULL; //new ContainerObject(true);
	if (this->CreateStaticObjectActor(OBJECT_TYPE_PROJECTILE_ARROW, &projectileObj, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		projectileObj->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticProjectileActor(projectileObj);

		if (Messages::FileWrite())
			Messages::Debug("Created Arrow Object ID: " + MaloW::convertNrToString((float)projectileObj->GetID()));

		counter++;
	}
	MaterialObject* material = NULL;
	if (this->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_SMALL_STICK, &material, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		material->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticMaterialObject(material);

		if (Messages::FileWrite())
			Messages::Debug("Created Small Stick Object ID: " + MaloW::convertNrToString((float)material->GetID()));

		counter++;
	}
	material = NULL;
	if (this->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_MEDIUM_STICK, &material, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		material->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticMaterialObject(material);

		if (Messages::FileWrite())
			Messages::Debug("Created Medium Stick Object ID: " + MaloW::convertNrToString((float)material->GetID()));

		counter++;
	}
	material = NULL;
	if (this->CreateStaticObjectActor(OBJECT_TYPE_MATERIAL_THREAD, &material, true))
	{
		position = CalculateSpawnPoint(counter, maxObjects, radius);
		material->SetPosition(position);
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticMaterialObject(material);

		if (Messages::FileWrite())
			Messages::Debug("Created Thread Object ID: " + MaloW::convertNrToString((float)material->GetID()));

		counter++;
	}

	DeerActor* testDeer = NULL;
	if (this->CreateAnimalActor(&testDeer, true))
	{
		this->zActorHandler->AddNewAnimalActor(testDeer);
	}

	if (Messages::FileWrite())
		Messages::Debug("Created " + MaloW::convertNrToString((float)counter) + " Objects");
}

void Host::Life()
{
	if (Messages::FileWrite())
		Messages::Debug("Host Process Started");
	this->zServerListener->Start();
	
	this->Init();

	INT64 frequency;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
	
	this->zSecsPerCnt = 1.0f / (float)(frequency);

	QueryPerformanceCounter((LARGE_INTEGER*)&this->zStartime);

	static float waitTimer = 0.0f;
	
	this->zGameStarted = false;
	while(this->stayAlive)
	{
		Update();


		//Checks if ServerListener is still working
		if(!this->zServerListener->IsAlive())
		{
			MaloW::Debug("Server Listener has died.");
		}

		HandleClientNackIM();
		HandleNewConnections();
		ReadMessages();
		HandleReceivedMessages();
		if(this->zGameStarted)
		{
			waitTimer += this->zDeltaTime;
			PingClients();
			UpdateObjects();

			if(waitTimer >= UPDATE_DELAY)
			{
				waitTimer = 0.0f;

				SendPlayerActorUpdates();
				SendAnimalActorUpdates();
				SendDynamicActorUpdates();
			}
		}
		else
		{
			if (this->zClients.size() >= this->zMinClients)
			{
				this->zGameStarted = true;
				this->SendStartMessage();
			}
		}
	
		Sleep(5);
	}
}

Vector3 Host::CalculateSpawnPoint(int currentPoint, int maxPoints, float radius, Vector3 center)
{
	static const float PI = 3.14159265358979323846f;

	float slice  = 2 * PI / maxPoints;

	float angle = slice * currentPoint;

	float x = center.x + radius * cos(angle);
	float z = center.z + radius * sin(angle);

	float y = this->zWorld->CalcHeightAtWorldPos(Vector2(x, z));

	return Vector3(x, y, z);
}

int Host::InitHost(const int PORT, const unsigned int MAX_CLIENTS)
{
	int code = 0;

	if(!this->zServerListener)
		this->zServerListener = new ServerListener();
	else
	{
		SAFE_DELETE(this->zServerListener);
		this->zServerListener = new ServerListener();
	}
	
	code = this->zServerListener->InitListener(PORT);

	this->zMaxClients = MAX_CLIENTS;
	this->zPort	= PORT;
	
	if ( zWorld ) delete zWorld, zWorld=0;
	this->zWorld = new World(this, "3x3.map");
	this->zActorHandler = new ActorHandler(this->zWorld);

	return code;
}

void Host::HandleNewConnections()
{
	MaloW::ProcessEvent* pe; 
	pe = this->zServerListener->PeekEvent();

	if(!pe)
		return;

	ClientConnectedEvent* cce = dynamic_cast<ClientConnectedEvent*>(pe);
	if(!cce)
	{
		SAFE_DELETE(pe);
		return;
	}

	if (Messages::FileWrite())
		Messages::Debug("New Player Connected.");

	if((unsigned int)this->zClients.size() > zMaxClients)
	{
		std::string message;
		
		message = zMessageConverter.Convert(MESSAGE_TYPE_SERVER_FULL);

		cce->GetClientChannel()->sendData(message);
		SAFE_DELETE(pe);

		return;
	}

	if (Messages::FileWrite())
		Messages::Debug("New Player Accepted.");

	std::string message = "";
	MaloW::ClientChannel* client = cce->GetClientChannel();
	ClientData* cd = new ClientData(client);

	client->setNotifier(this);
	this->zClients.push_back(cd);

	message = this->zMessageConverter.Convert(MESSAGE_TYPE_SELF_ID, (float)client->getClientID());
	client->Start();
	SendToClient(cd, message, true);
	
	SAFE_DELETE(pe);

	//Collect player infos
	message = "";
	std::vector<std::string> temp;
	std::vector<PlayerActor *> players = this->zActorHandler->GetPlayers();

	//Gets Players Information
	for(auto it = players.begin(); it < players.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		message =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)(*it)->GetID());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)(*it)->GetState());

		temp.push_back(message);
	}
	message = "";
	std::vector<AnimalActor *> animals = this->zActorHandler->GetAnimals();

	//Gets AnimalInformation
	for(auto it = animals.begin(); it < animals.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		message =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_ANIMAL, (float)(*it)->GetID());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)(*it)->GetState());
		temp.push_back(message);
	}


	this->GetExistingObjects(temp);

	std::vector<DeadPlayerObjectActor*> deadPlayers = this->zActorHandler->GetDeadPlayers();
	std::vector<Item*> items;

	for (auto it = deadPlayers.begin(); it < deadPlayers.end(); it++)
	{
		message = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_PLAYER_OBJECT, (float)(*it)->GetID());

		Vector3 pos = (*it)->GetPosition();
		Vector4 rot = (*it)->GetRotation();
		Vector3 scale = (*it)->GetScale();

		message += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());

		items = (*it)->GetItems();
		for (auto x = items.begin(); x < items.end(); x++)
		{
			if ((*x)->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || (*x)->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
			{
				Container* container = dynamic_cast<Container*>((*x));

				if (!container)
				{
					MaloW::Debug("Failed to cast Material in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)container->GetID());

				message += container->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if ((*x)->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK || (*x)->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK ||
				(*x)->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK || (*x)->GetItemType() == ITEM_TYPE_MATERIAL_THREAD)
			{
				Material* material = dynamic_cast<Material*>((*x));

				if (!material)
				{
					MaloW::Debug("Failed to cast Material in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)material->GetID());

				message += material->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if ((*x)->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || (*x)->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
			{
				Food* food = dynamic_cast<Food*>((*x));

				if (!food)
				{
					MaloW::Debug("Failed to cast Food in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)food->GetID());

				message += food->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if ((*x)->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
			{
				Projectile* projectile = dynamic_cast<Projectile*>((*x));

				if (!projectile)
				{
					MaloW::Debug("Failed to cast Projectile in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)projectile->GetID());

				message += projectile->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if ((*x)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW || (*x)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
			{
				RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>((*x));

				if (!rWpn)
				{
					MaloW::Debug("Failed to cast Ranged Weapon in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)rWpn->GetID());

				message += rWpn->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
			else if ((*x)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE || (*x)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
			{
				MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>((*x));

				if (!mWpn)
				{
					MaloW::Debug("Failed to cast Melee Weapon in HandleNewConnection");
					continue;
				}
				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)mWpn->GetID());

				message += mWpn->ToMessageString(&this->zMessageConverter);

				message += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
			}
		}
		temp.push_back(message);

	}

	//Sends All Actors to the player
	for (auto it = temp.begin(); it < temp.end(); it++)
	{
		SendToClient(cd,*it, true);
	}

}

void Host::SendToAllClients(const std::string& message, bool sendIM /*= false*/)
{
	if(!HasClients())
		return;

	std::string im;
	for(auto it = zClients.begin(); it < zClients.end(); it++)
	{
		if(sendIM)
		{
			unsigned long m_id = (*it)->GetNextIPID();
			im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
			(*it)->SendIM((im+message), m_id);

			continue;
		}

		(*it)->SendM(message);
		
	}
}

void Host::SendToClient(int clientID, const std::string& message, bool sendIM /*= false*/)
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;
	ClientData* cd = this->zClients[pos];
	if(!sendIM)
	{
		cd->SendM(message);
		return;
	}

	unsigned long m_id = cd->GetNextIPID();
	std::string im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
	cd->SendIM((im + message), m_id);
}

void Host::SendToClient(ClientData* cd, const std::string& message, bool sendIM /*= false*/)
{
	if(cd == NULL)
		return;

	if(!sendIM)
	{
		cd->SendM(message);
		return;
	}

	unsigned long m_id = cd->GetNextIPID();
	std::string im = this->zMessageConverter.Convert(MESSAGE_TYPE_IMPORTANT_MESSAGE, (float)m_id);
	cd->SendIM((im + message), m_id);
}

void Host::SendStartMessage()
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);

	this->SendToAllClients(msg, true);
}

void Host::SendPlayerActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> playerData;
	std::string mess = "";

	//Fetch player data
	std::vector<PlayerActor*> pl = this->zActorHandler->GetPlayers();
	for (auto it_Player = pl.begin(); it_Player < pl.end(); it_Player++)
	{
		Vector3 pos = (*it_Player)->GetPosition();
		Vector4 rot = (*it_Player)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_PLAYER, (float)(*it_Player)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_FRAME_TIME, (*it_Player)->GetFrameTime());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)(*it_Player)->GetState());
		(*it_Player)->AddChangedHData(mess, &this->zMessageConverter);

		playerData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = playerData.begin(); it_Message < playerData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

void Host::SendAnimalActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> animalData;
	std::string mess = "";

	//Fetch player data
	std::vector<AnimalActor*> al = this->zActorHandler->GetAnimals();
	for (auto it_Animal = al.begin(); it_Animal < al.end(); it_Animal++)
	{
		Vector3 pos = (*it_Animal)->GetPosition();
		Vector4 rot = (*it_Animal)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_ANIMAL, (float)(*it_Animal)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)(*it_Animal)->GetState());

		animalData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = animalData.begin(); it_Message < animalData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

void Host::SendStaticActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> staticData;
	std::string mess = "";

	//Fetch Static Objects data
	std::vector<FoodObject*> stf = this->zActorHandler->GetFoods();
	for (auto it_Static = stf.begin(); it_Static < stf.end(); it_Static++)
	{
		Vector3 pos = (*it_Static)->GetPosition();
		Vector4 rot = (*it_Static)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (float)(*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it_Static)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it_Static)->GetDescription());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it_Static)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it_Static)->GetActorObjectName());

		staticData.push_back(mess);
	}

	std::vector<WeaponObject*> stw = this->zActorHandler->GetWeapons();
	for (auto it_Static = stw.begin(); it_Static < stw.end(); it_Static++)
	{
		Vector3 pos = (*it_Static)->GetPosition();
		Vector4 rot = (*it_Static)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (float)(*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it_Static)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it_Static)->GetDescription());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it_Static)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it_Static)->GetActorObjectName());

		staticData.push_back(mess);
	}

	std::vector<ContainerObject*> stc = this->zActorHandler->GetContainers();
	for (auto it_Static = stc.begin(); it_Static < stc.end(); it_Static++)
	{
		Vector3 pos = (*it_Static)->GetPosition();
		Vector4 rot = (*it_Static)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (float)(*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it_Static)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it_Static)->GetDescription());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it_Static)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it_Static)->GetActorObjectName());

		staticData.push_back(mess);
	}
	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = staticData.begin(); it_Message < staticData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

void Host::SendDynamicActorUpdates()
{
	if(!HasClients())
		return;

	std::vector<std::string> dynamicData;
	std::string mess = "";

	//Fetch Static Objects data
	std::vector<DynamicProjectileObject*> dyp = this->zActorHandler->GetDynamicProjectiles();
	for (auto it_Dynamic = dyp.begin(); it_Dynamic < dyp.end(); it_Dynamic++)
	{
		Vector3 pos = (*it_Dynamic)->GetPosition();
		Vector4 rot = (*it_Dynamic)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_DYNAMIC_OBJECT, (float)(*it_Dynamic)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it_Dynamic)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it_Dynamic)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it_Dynamic)->GetDescription());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it_Dynamic)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it_Dynamic)->GetActorObjectName());

		dynamicData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = dynamicData.begin(); it_Message < dynamicData.end(); it_Message++)
		{
			(*it_Client)->SendM((*it_Message));
		}
	}
}

bool Host::HasClients() const
{
	return !this->zClients.empty();
}

void Host::ReadMessages()
{
	static unsigned int MAX_MESSAGES_TO_READ = 10;
	unsigned int nrOfMessages = this->GetEventQueueSize();

	//No new messsages
	if(nrOfMessages == 0)
		return;

	nrOfMessages = min(nrOfMessages, MAX_MESSAGES_TO_READ);
	MaloW::ProcessEvent* pe;

	for (unsigned int i = 0; i < nrOfMessages; i++)
	{
		pe = PeekEvent();

		if(pe)
		{
			MaloW::NetworkPacket* np = dynamic_cast<MaloW::NetworkPacket*>(pe);
			if(np)
				this->zMessages.push_back(np);
			else
				SAFE_DELETE(pe);
		}
		
	}
}

void Host::SendErrorMessage(const long id, const std::string error_Message)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ERROR_MESSAGE, error_Message);
	this->SendToClient(id, msg);
}

std::string Host::CreateDeadPlayerObject(PlayerActor* pActor, DeadPlayerObjectActor** dpoActor)
{
	(*dpoActor) = new DeadPlayerObjectActor(true);

	std::vector<Item*> items;
	Inventory* inv = pActor->GetInventory();
	Equipment* eq = pActor->GetEquipment();

	std::string path = "Media/Models/Scale.obj";	

	Vector3 up = Vector3(0, 1, 0);//pActor->GetUpVector();
	Vector3 forward = Vector3(0, 0, 1);//pActor->GetDirection();
	forward.y = 0;
	Vector3 around = up.GetCrossProduct(forward);
	around.Normalize();
	float angle = 3.14f * 0.5f;

	PhysicsObject* pObj = pActor->GetPhysicObject();

	pObj->SetQuaternion(Vector4(0, 0, 0, 1));
	pObj->RotateAxis(around, angle);

	(*dpoActor)->SetActorModel(path);
	(*dpoActor)->SetScale(pActor->GetScale());
	
	(*dpoActor)->SetRotation(pActor->GetRotation());
	(*dpoActor)->SetPosition(pActor->GetPosition());
	
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ADD_DEAD_PLAYER_OBJECT, (float)(*dpoActor)->GetID());
	
	Vector3 pos = (*dpoActor)->GetPosition();
	Vector4 rot = (*dpoActor)->GetRotation();
	Vector3 scale = (*dpoActor)->GetScale();

	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*dpoActor)->GetActorModel());

	std::vector<Item*> temp_items = inv->GetItems();
	Weapon* weapon = eq->GetWeapon();

	if (weapon)
	{
		items.push_back(weapon);
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)weapon->GetID());

		msg += weapon->ToMessageString(&this->zMessageConverter);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
	}
	Projectile* projectile = eq->GetProjectile();

	if (projectile)
	{
		items.push_back(projectile);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)projectile->GetID());

		msg += projectile->ToMessageString(&this->zMessageConverter);

		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
	}
	for (auto it = temp_items.begin(); it < temp_items.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || (*it)->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
		{
			Container* temp = dynamic_cast<Container*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Container when Creating Dead Player items");
				continue;
			}

			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());
			msg += temp->ToMessageString(&this->zMessageConverter);
			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_MATERIAL_SMALL_STICK || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_MEDIUM_STICK ||
			(*it)->GetItemType() == ITEM_TYPE_MATERIAL_LARGE_STICK || (*it)->GetItemType() == ITEM_TYPE_MATERIAL_THREAD)
		{
			Material* temp = dynamic_cast<Material*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Material when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || (*it)->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
		{
			Food* temp = dynamic_cast<Food*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Food when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
		{
			Projectile* temp = dynamic_cast<Projectile*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Projectile when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW || (*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
		{
			RangedWeapon* temp = dynamic_cast<RangedWeapon*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Ranged Weapon when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
		else if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE || (*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
		{
			MeleeWeapon* temp = dynamic_cast<MeleeWeapon*>((*it));
			if (!temp)
			{
				MaloW::Debug("Failed to cast Melee Weapon when Creating Dead Player items");
				continue;
			}
			inv->EraseItem(temp->GetID());

			items.push_back(temp);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ADD_ITEM, (float)temp->GetID());

			msg += temp->ToMessageString(&this->zMessageConverter);

			msg += this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER_ITEM_FINISHED);
		}
	}

	(*dpoActor)->SetItems(items);

	return msg;
}

bool Host::CreateAnimalActor(DeerActor** deerAct, const bool genID)
{
	static Vector3 position;

	int size = this->zActorHandler->GetAnimals().size();
	position = CalculateSpawnPoint(size, 10, 10);

	(*deerAct) = new DeerActor(genID);
	std::string path = "Media/Models/Tree_02_v02_r.obj";	
	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(
		path, position);

	(*deerAct)->SetActorModel(path);
	(*deerAct)->SetPhysicObject(pObj);
	(*deerAct)->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	(*deerAct)->SetWorldPointer(this->zWorld);

	return true;
}

bool Host::CreateAnimalActor(WolfActor** wolfAct, const bool genID)
{
	static Vector3 position;

	int size = this->zActorHandler->GetAnimals().size();
	position = CalculateSpawnPoint(size, 10, 10);

	(*wolfAct) = new WolfActor(genID);
	std::string path = "Media/Models/Tree_02_v02_r.obj";	
	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(
		path, position);

	(*wolfAct)->SetActorModel(path);
	(*wolfAct)->SetPhysicObject(pObj);
	(*wolfAct)->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	(*wolfAct)->SetWorldPointer(this->zWorld);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, WeaponObject** weaponObj, const bool genID)
{
	//Get Default Values For a Weapon Object
	const WeaponObject* weapon = this->zActorHandler->GetObjManager()->GetWeaponObject(type);

	if (!weapon)
		return false;

	*weaponObj = new WeaponObject(weapon, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, FoodObject** foodObj, const bool genID)
{
	//Get Default Values For a Meat Object
	const FoodObject* food = this->zActorHandler->GetObjManager()->GetFoodObject(type);

	if (!food)
		return false;

	*foodObj = new FoodObject(food, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, ContainerObject** containerObj, const bool genID)
{
	//Get Default Values For a container Object
	const ContainerObject* container = this->zActorHandler->GetObjManager()->GetContainerObject(type);

	if (!container)
		return false;

	*containerObj = new ContainerObject(container, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, StaticProjectileObject** projectileObj, const bool genID)
{
	//Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zActorHandler->GetObjManager()->GetStaticProjectileObject(type);

	if (!projectile)
		return false;

	*projectileObj = new StaticProjectileObject(projectile, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, MaterialObject** materialObj, const bool genID)
{
	const MaterialObject* material = this->zActorHandler->GetObjManager()->GetMaterialObject(type);

	if (!material)
		return false;

	*materialObj = new MaterialObject(material, genID);

	return true;
}

bool Host::CreateDynamicObjectActor(const int type, DynamicProjectileObject** projectileObj, bool genID)
{
	////Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zActorHandler->GetObjManager()->GetStaticProjectileObject(type);

	if (!projectile)
		return false;
	
	string modelPath = projectile->GetActorModel();

	(*projectileObj) = new DynamicProjectileObject(genID);

	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(modelPath, Vector3(0,0,0));

	(*projectileObj)->SetPhysicObject(pObj);

	//Creates A New ProjectileObject With an Id And Default Values 
	(*projectileObj)->SetType(type);
	(*projectileObj)->SetWeight(projectile->GetWeight());
	(*projectileObj)->SetDamage(projectile->GetDamage());
	(*projectileObj)->SetIconPath(projectile->GetIconPath());
	(*projectileObj)->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	(*projectileObj)->SetActorModel(modelPath);
	(*projectileObj)->SetDescription(projectile->GetDescription());
	(*projectileObj)->SetSpeed(projectile->GetSpeed());
	(*projectileObj)->SetActorObjectName(projectile->GetActorObjectName());

	return true;
}

void Host::SendNewObjectMessage(StaticObjectActor* staticObj)
{
	std::string msg;
	Vector3 pos = staticObj->GetPosition();
	Vector3 scale = staticObj->GetScale();
	Vector4 rot = staticObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)staticObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, staticObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)staticObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)staticObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)staticObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, staticObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, staticObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, staticObj->GetIconPath());

	this->SendToAllClients(msg, true);
}

void Host::SendNewObjectMessage(DynamicObjectActor* dynamicObj)
{
	std::string msg;
	Vector3 pos = dynamicObj->GetPosition();
	Vector3 scale = dynamicObj->GetScale();
	Vector4 rot = dynamicObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_DYNAMIC_OBJECT, (float)dynamicObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, dynamicObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)dynamicObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)dynamicObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float)dynamicObj->GetStackSize());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, dynamicObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, dynamicObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, dynamicObj->GetIconPath());
	//ProjectileObject* projectileObj = dynamic_cast<ProjectileObject*>(dynamicObj);
	//if (projectileObj)
	//{
		//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, projectileObj->GetDamage());
	//}
	
	this->SendToAllClients(msg, true);
}

void Host::SendNewObjectMessage(AnimalActor* animalObj)
{
	std::string msg;
	Vector3 pos = animalObj->GetPosition();
	Vector3 scale = animalObj->GetScale();
	Vector4 rot = animalObj->GetRotation();

	msg  = this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)animalObj->GetState());
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, animalObj->GetActorModel());

	this->SendToAllClients(msg, true);
}

void Host::HandleReceivedMessages()
{
	
	if(this->zMessages.empty())
		return;

	int c_index;
	PlayerActor* p_actor = NULL; 

	for (auto it = this->zMessages.begin(); it < this->zMessages.end(); it++)
	{
		//If null, skip and continue
		if(!(*it))
			continue;
		
		std::vector<std::string> msgArray;
		msgArray = this->zMessageConverter.SplitMessage((*it)->getMessage()); 

		//If empty, skip and continue
		if(msgArray.empty())
			continue;

		char key[128];
		sscanf_s(msgArray[0].c_str(), "%s ", &key, sizeof(key));
		
		c_index = SearchForClient((*it)->getID());
		p_actor = dynamic_cast<PlayerActor*>(this->zActorHandler->GetActor((*it)->getID(), ACTOR_TYPE_PLAYER));

		//Handles updates from client.
		if(strcmp(key, M_CLIENT_DATA.c_str()) == 0 && (p_actor))
		{
			this->HandlePlayerUpdate(p_actor, this->zClients[c_index], msgArray);
		}
		//Handles key presses from client.
		else if(strcmp(key, M_KEY_DOWN.c_str()) == 0 && (p_actor))
		{
			this->HandleKeyPress(p_actor, msgArray[0]);
		}
		//Handles key releases from client.
		else if(strcmp(key, M_KEY_UP.c_str()) == 0 && (p_actor))
		{
			this->HandleKeyRelease(p_actor, msgArray[0]);
		}
		//Handles Pings from client.
		else if(strcmp(key, M_PING.c_str()) == 0 && (c_index != -1))
		{
			this->zClients[c_index]->HandlePingMsg();
		}
		//Handles Ack Messages
		else if(strcmp(key, M_ACKNOWLEDGE_MESSAGE.c_str()) == 0 && (c_index != -1))
		{
			int m_id = this->zMessageConverter.ConvertStringToInt(M_ACKNOWLEDGE_MESSAGE, msgArray[0]);
			this->zClients[c_index]->RemoveIM(m_id);
		}
		//Handle Item usage in Inventory
		else if(strcmp(key, M_ITEM_USE.c_str()) == 0 && (c_index != -1))
		{
			int objID = this->zMessageConverter.ConvertStringToInt(M_ITEM_USE, msgArray[0]);
			this->HandleItemUse(p_actor, objID);
		}
		//Handle UnEquip Item in Equipment
		else if(strcmp(key, M_UNEQUIP_ITEM.c_str()) == 0 && (c_index != -1))
		{
			int objID = this->zMessageConverter.ConvertStringToInt(M_UNEQUIP_ITEM, msgArray[0]);
			int eq_Slot = -1;
			if (msgArray.size() > 1)
			{
				eq_Slot = this->zMessageConverter.ConvertStringToInt(M_EQUIPMENT_SLOT, msgArray[1]);
			}
			this->HandleUnEquipItem(p_actor, objID, eq_Slot);
		}
		//Handles Equipped Weapon usage
		else if(strcmp(key, M_WEAPON_USE.c_str()) == 0 && (c_index != -1))
		{
			int objID = this->zMessageConverter.ConvertStringToInt(M_WEAPON_USE, msgArray[0]);
			this->HandleWeaponUse(p_actor, objID);
		}
		//Handles Pickup Object Requests from Client
		else if(strcmp(key, M_PICKUP_ITEM.c_str()) == 0 && (p_actor))
		{
			int objID = this->zMessageConverter.ConvertStringToInt(M_PICKUP_ITEM, msgArray[0]);
			this->HandlePickupItem(p_actor, objID);
		}
		else if(strcmp(key, M_LOOT_ITEM.c_str()) == 0 && (p_actor))
		{
			long objID = this->zMessageConverter.ConvertStringToInt(M_LOOT_ITEM, msgArray[0]);
			int type = -1;
			long itemID = -1;
			if (msgArray.size() > 2)
			{
				itemID = this->zMessageConverter.ConvertStringToInt(M_ITEM_ID, msgArray[1]);
				type = this->zMessageConverter.ConvertStringToInt(M_ITEM_TYPE, msgArray[2]);

				if (this->HandleLootItem(p_actor, objID, itemID, type))
				{
					this->SendRemoveDeadPlayerItem(p_actor->GetID(), objID, itemID, type);
				}
				
			}
			else
			{
				MaloW::Debug("Msg array size is to short size: " + MaloW::convertNrToString((float)msgArray.size()) + " Expected size 3");
			}
		}
		//Handles Drop Item Requests from Client
		else if(strcmp(key, M_DROP_ITEM.c_str()) == 0 && (p_actor))
		{
			int objID = this->zMessageConverter.ConvertStringToInt(M_DROP_ITEM, msgArray[0]);
			this->HandleDropItem(p_actor, objID);
		}
		//Handles user data from client. Used when the player is new.
		else if(strcmp(key, M_USER_DATA.c_str()) == 0 && (c_index != -1))
		{
			this->CreateNewPlayer(this->zClients[c_index], msgArray);
		}
		//Handles if client disconnects.
		else if(strcmp(key, M_CONNECTION_CLOSED.c_str()) == 0)
		{
			this->KickClient((*it)->getID());
		}
		//Handles if not of the above.
		else
		{
			MaloW::Debug("Warning: The host cannot handle the message \""+(*it)->getMessage()+"\" in HandleReceivedMessages.");
		}

		SAFE_DELETE((*it));
	}
}

void Host::HandleKeyPress(PlayerActor* pl, const std::string& key)
{
	//Hard coded for test
	int keyz = this->zMessageConverter.ConvertStringToInt(M_KEY_DOWN, key);

	switch (keyz)
	{
	case KEY_FORWARD:
		pl->SetKeyState(KEY_FORWARD, true);
		break;
	case KEY_BACKWARD:
		pl->SetKeyState(KEY_BACKWARD, true);
		break;
	case KEY_LEFT:
		pl->SetKeyState(KEY_LEFT, true);
		break;
	case KEY_RIGHT:
		pl->SetKeyState(KEY_RIGHT, true);
		break;
	case KEY_SPRINT:
		pl->SetKeyState(KEY_SPRINT, true);
		break;
	case KEY_DUCK:
		pl->SetKeyState(KEY_DUCK, true);
		break;
	case KEY_JUMP:
		pl->SetKeyState(KEY_JUMP, true);
		break;
	default:
		break;
	}

}

void Host::HandleKeyRelease(PlayerActor* pl, const std::string& key)
{
	//Hard coded for test
	int keyz = this->zMessageConverter.ConvertStringToInt(M_KEY_UP, key);

	switch (keyz)
	{
	case KEY_FORWARD:
		pl->SetKeyState(KEY_FORWARD, false);
		break;
	case KEY_BACKWARD:
		pl->SetKeyState(KEY_BACKWARD, false);
		break;
	case KEY_LEFT:
		pl->SetKeyState(KEY_LEFT, false);
		break;
	case KEY_RIGHT:
		pl->SetKeyState(KEY_RIGHT, false);
		break;
	case KEY_SPRINT:
		pl->SetKeyState(KEY_SPRINT, false);
		break;
	case KEY_DUCK:
		pl->SetKeyState(KEY_DUCK, false);
		break;
	case KEY_JUMP:
		pl->SetKeyState(KEY_JUMP, false);
		break;
	default:
		break;
	}
}

void Host::HandlePlayerUpdate(PlayerActor* pl, ClientData* cd, const std::vector<std::string> &data)
{
	
	for(auto it = data.begin() + 1; it < data.end(); it++)
	{
		char key[512];
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));
		
		if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			Vector3 dir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it));
			pl->SetDirection(dir);
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			Vector3 up = this->zMessageConverter.ConvertStringToVector(M_UP, (*it));
			pl->SetUpVector(up);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rot = this->zMessageConverter.ConvertStringToQuaternion(M_ROTATION, (*it));
			pl->SetRotation(rot);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			float frameTime = this->zMessageConverter.ConvertStringToFloat(M_FRAME_TIME, (*it));
			pl->SetFrameTime(frameTime);
		}
		else
			MaloW::Debug("Unknown message in HandlePlayerUpdate.");
	}

	//Update Latency
	float latency;

	if(!cd->CalculateLatency(latency))
		return;

	pl->SetLatency(latency);
}

void Host::HandleClientNackIM()
{

	if(!HasClients())
		return;
	std::vector<int> clientsToKick;

	for(auto it = this->zClients.begin(); it < this->zClients.end(); it++)
	{
		(*it)->HandleNackIM(this->zDeltaTime);

		if((*it)->GetNrOfExceededIM() > 0)
			clientsToKick.push_back((*it)->GetClient()->getClientID());
	}

	for (auto it = clientsToKick.begin(); it < clientsToKick.end(); it++)
	{
		KickClient((*it), true, "No Ack on message.");
	}

}

int Host::SearchForClient(const int ID) const
{
	if(!HasClients())
		return -1;

	for (unsigned int i = 0; i < this->zClients.size(); i++)
	{
		if(this->zClients.at(i)->GetClient()->getClientID() == ID)
		{
			return i;
		}
	}

	return -1;
}

void Host::BroadCastServerShutdown()
{
	std::string mess = this->zMessageConverter.Convert(MESSAGE_TYPE_SERVER_SHUTDOWN);
	SendToAllClients(mess);
}

void Host::PingClients()
{
	if(!HasClients())
		return;

	ClientData* cd; 
	MaloW::ClientChannel* ch;

	for(unsigned int i = 0; i < (unsigned int)zClients.size(); i++)
	{
		cd = zClients.at(i);
		ch = cd->GetClient();

		//If client has not been pinged.
		if(!cd->HasBeenPinged())
		{
			//If it was x sec ago we sent a ping, don't send a ping.
			if(cd->GetCurrentPingTime() < zPingMessageInterval)
				cd->IncPingTime(zDeltaTime);

			//else send ping.
			else
			{
				cd->SetCurrentPingTime(0.0f);
				ch->sendData(this->zMessageConverter.Convert(MESSAGE_TYPE_PING));
				cd->SetPinged(true);
			}
		}
		//If he have sent a ping.
		else
		{
			//If we sent a ping x sec ago, drop the client.
			if(cd->GetCurrentPingTime() > zTimeOut)
			{
				KickClient(ch->getClientID());
			}
			else
				cd->IncPingTime(zDeltaTime);

		}
	}
}

float Host::Update()
{
	//Update Timer
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - this->zStartime);

	this->zDeltaTime = timeDifference * this->zSecsPerCnt;

	this->zStartime = currentTime;

	return this->zDeltaTime;
}

void Host::UpdateObjects()
{
	//Update the objects in the world.
	this->zActorHandler->UpdateObjects(zDeltaTime);

	std::vector<CollisionEvent> ce;
	//CheckCollisions is not complete.
	this->zActorHandler->CheckCollisions();
	ce = this->zActorHandler->CheckProjectileCollisions();

	//Iterate projectiles and converts them to static if they have stopped
	std::vector<DynamicProjectileObject*> dynamicProjectileObj = this->zActorHandler->GetDynamicProjectiles();

	for (auto it_proj = dynamicProjectileObj.begin(); it_proj < dynamicProjectileObj.end(); it_proj++)
	{
		if (!(*it_proj)->IsMoving())
		{
			HandleConversion((*it_proj));

			if(!this->zActorHandler->RemoveDynamicProjectileActor((*it_proj)->GetID()))
				MaloW::Debug("Failed to Remove Object in Host::UpdateObjects");
		}
	}

	//Iterate players and see if they are dead.
	std::vector<PlayerActor*> pActors = this->zActorHandler->GetPlayers();
	Vector3 position;	for (auto it_player = pActors.begin(); it_player < pActors.end(); it_player++)
	{
		if (!(*it_player)->IsAlive())
		{
			OnPlayerDeath((*it_player)->GetID());
		}
		else
		{
			//position = (*it_player)->GetPosition();
			//if (this->zWorld->IsBlockingAt(position.GetXZ()))
			//{
			//	(*it_player)->RewindPosition();
			//}
		}
	}

}

bool Host::KickClient(const int ID, bool sendAMessage, std::string reason)
{
	int index = SearchForClient(ID);

	ClientData* temp_c = zClients.at(index);

	std::string mess;
	bool removed = false;

	if(index == -1)
	{
		MaloW::Debug("Can't find client to kick in KickClient in Host.cpp.");
		return false;
	}

	if(sendAMessage)
	{
		mess = this->zMessageConverter.Convert(MESSAGE_TYPE_KICKED, reason);
		temp_c->SendM(mess);
	}
	//Create Dead Player Object
	
	OnPlayerRemove(ID);

	//Create a remove player message.
	mess = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_PLAYER, (float)ID);

	//Remove the player

	this->zClients.erase(zClients.begin() + index);
	this->zActorHandler->RemovePlayerActor(ID);

	SAFE_DELETE(temp_c);

	removed = true;
	MaloW::Debug("Client"+MaloW::convertNrToString((float)ID)+" removed from server.");
	
	
	//Notify clients
	this->SendToAllClients(mess, true);

	return removed;
}

void Host::OnPlayerRemove(unsigned int ID)
{
	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActorHandler->GetActor(ID, ACTOR_TYPE_PLAYER));

	if (!pActor)
		return;

	DeadPlayerObjectActor* dpoActor = NULL;

	std::string msg = this->CreateDeadPlayerObject(pActor, &dpoActor);

	this->zActorHandler->AddNewDeadPlayer(dpoActor);

	this->SendToAllClients(msg, true);
}

void Host::OnPlayerDeath(unsigned int ID)
{
	OnPlayerRemove(ID);

	PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zActorHandler->GetActor(ID, ACTOR_TYPE_PLAYER));
	if (pActor)
	{
		std::string	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_DEAD_PLAYER, ID);
		SendToAllClients(msg, true);

		RespawnPlayer(pActor);

		this->zActorHandler->RemovePlayerActor(pActor->GetID());
	}
	else
		MaloW::Debug("Cant find Player in Host::OnPlayerDeath");

	//KickClient(ID);
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data )
{
	std::string mess;
	
	PlayerActor* pi = new PlayerActor(cd->GetClient()->getClientID());

	for (auto it_m = data.begin() + 1; it_m < data.end(); it_m++)
	{
		char key[512];
		sscanf_s((*it_m).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			std::string uModel = this->zMessageConverter.ConvertStringToSubstring(M_MESH_MODEL, (*it_m));
			if(uModel != "")
				pi->SetActorModel(uModel);
		}
		else if(strcmp(key, M_DIRECTION.c_str()) == 0)
		{
			Vector3 uDir = this->zMessageConverter.ConvertStringToVector(M_DIRECTION, (*it_m));
			pi->SetDirection(uDir);
		}
		else if(strcmp(key, M_UP.c_str()) == 0)
		{
			Vector3 uUp = this->zMessageConverter.ConvertStringToVector(M_UP, (*it_m));
			pi->SetUpVector(uUp);
		}
	}

	//Debug Pos
	int currentPoint = this->zActorHandler->GetPlayers().size() % this->zMaxClients;
	Vector3 position = CalculateSpawnPoint(currentPoint, this->zMaxClients, 20);
	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(pi->GetActorModel(), position);
	pi->SetPhysicObject(pObj);
	pi->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	if(!pObj)
		MaloW::Debug("Error in function AddNewPlayer in ActorHandler: PhysicObj is null.");

	/*End debug*/	//Add new player to the list
	this->zActorHandler->AddNewPlayer(pi);

	pi->AddObserver(this);
	zAnchorPlayerMap[pi] = this->zWorld->CreateAnchor();

	//Gather New player information
	Vector3 pos = pi->GetPosition();
	Vector3 scale = pi->GetScale();
	Vector4 rot = pi->GetRotation();

	mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)pi->GetID());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, pi->GetActorModel());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)pi->GetState());

	//Send new player to players
	SendToAllClients(mess, true);
	if (this->zGameStarted)
	{
		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);
		this->SendToClient(pi->GetID(), msg, true);
	}

}

void Host::RespawnPlayer(PlayerActor* pActor)
{
	std::string model = pActor->GetActorModel();
	PlayerActor* new_Player = new PlayerActor(pActor->GetID());

	int currentPoint = this->zActorHandler->GetPlayers().size() % this->zMaxClients;
	Vector3 position = CalculateSpawnPoint(currentPoint, this->zMaxClients, 20);
	PhysicsObject* pObj = this->zActorHandler->GetPhysicEnginePtr()->CreatePhysicsObject(model, position);
	new_Player->SetPhysicObject(pObj);
	new_Player->SetActorModel(model);
	new_Player->SetScale(Vector3(0.05f, 0.05f, 0.05f));

	if(!pObj)
		MaloW::Debug("Error in function AddNewPlayer in ActorHandler: PhysicObj is null.");

	/*End debug*/	//Add new player to the list
	this->zActorHandler->AddNewPlayer(new_Player);

	new_Player->AddObserver(this);
	zAnchorPlayerMap[new_Player] = this->zWorld->CreateAnchor();

	//Gather New player information
	Vector3 pos = new_Player->GetPosition();
	Vector3 scale = new_Player->GetScale();
	Vector4 rot = new_Player->GetRotation();

	std::string mess;

	mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (float)new_Player->GetID());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, new_Player->GetActorModel());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (float)new_Player->GetState());

	//Send new player to players
	SendToAllClients(mess, true);
	if (this->zGameStarted)
	{
		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_START_GAME);
		this->SendToClient(new_Player->GetID(), msg, true);
	}
}

void Host::GetExistingObjects(std::vector<std::string>& static_Objects)
{
	std::string mess;
	std::vector<FoodObject*> static_Food = this->zActorHandler->GetFoods();
	//Gets Static Food Objects Data
	for(auto it = static_Food.begin(); it < static_Food.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);

	}
	std::vector<WeaponObject*> static_Weapon = this->zActorHandler->GetWeapons();
	//Gets Static Weapon Objects Data
	for(auto it = static_Weapon.begin(); it < static_Weapon.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}

	std::vector<ContainerObject*> static_Container = this->zActorHandler->GetContainers();
	//Gets Static Container Objects Data
	for(auto it = static_Container.begin(); it < static_Container.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}

	std::vector<StaticProjectileObject*> static_Projectile = this->zActorHandler->GetStaticProjectiles();
	//Gets Static Projectile Objects Data
	for(auto it = static_Projectile.begin(); it < static_Projectile.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}

	std::vector<MaterialObject*> static_Material = this->zActorHandler->GetMaterials();
	//Gets Static Weapon Objects Data
	for(auto it = static_Material.begin(); it < static_Material.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (float)(*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (float)(*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float)(*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}
}

int Host::GetNrOfPlayers() const
{
	return this->zClients.size();
}

void Host::onEvent( Event* e )
{
	if ( WorldDeletedEvent* WDE = dynamic_cast<WorldDeletedEvent*>(e) )
	{
		if ( zWorld ) zWorld = 0;
	}
	else if ( EntityRemovedEvent *ERE = dynamic_cast<EntityRemovedEvent*>(e) )
	{
		//zTargetedEntities.erase(ERE->entity);
	}
	else if( PlayerUpdatedEvent* PUE = dynamic_cast<PlayerUpdatedEvent*>(e) )
	{
		Vector3 playerTempPos = PUE->playerActor->GetPosition();
		
		if(playerTempPos.x >= 0 && playerTempPos.z >= 0)
		{
			Vector3 oldPos = PUE->prevPos; 
			float yPos = this->zWorld->CalcHeightAtWorldPos(Vector2(playerTempPos.x, playerTempPos.z));
			Vector3 dir = playerTempPos - oldPos;
			Vector3 groundNormal = this->zWorld->CalcHeightAtWorldPos(Vector2(playerTempPos.x, playerTempPos.z));

			playerTempPos.y -= (9.82f * this->zDeltaTime);			
			if(playerTempPos.y < yPos)
				playerTempPos.y = yPos;

			//dir.y = groundNormal.y;
			//dir.y = yPos;
			dir.Normalize();
			Vector3 tempGround = groundNormal;
			tempGround.y = 0;
			tempGround.Normalize();
			float dot = dir.GetDotProduct(tempGround);
			if( groundNormal.y <= 0.5f )
			{
				Vector3 newPlayerTempPos = playerTempPos + (tempGround * (zDeltaTime * 4));

				float yPosNew = this->zWorld->CalcHeightAtWorldPos(Vector2(newPlayerTempPos.x, newPlayerTempPos.z));
				newPlayerTempPos.y += -9.82f * zDeltaTime;
				if(newPlayerTempPos.y < yPosNew)
					newPlayerTempPos.y = yPosNew;
				PUE->validMove = false;
				PUE->prevPos = newPlayerTempPos;
			}
			else if(dot > 0.2)
			{
				PUE->validMove = true;
//				playerTempPos.y += -1.82 * zDeltaTime;
				playerTempPos.y += -9.82f * zDeltaTime;
				if(playerTempPos.y < yPos)
					playerTempPos.y = yPos;

				PUE->playerActor->SetPosition(playerTempPos);
				this->zAnchorPlayerMap[PUE->playerActor]->position = Vector2(playerTempPos.x, playerTempPos.z);
			}			
			else if(groundNormal.y > 0.7f)
			{
				playerTempPos.y = yPos;
				PUE->playerActor->SetPosition(playerTempPos);
				this->zAnchorPlayerMap[PUE->playerActor]->position = Vector2(playerTempPos.x, playerTempPos.z);
				PUE->validMove = true;
			}
			

		}
		else
		{
			PUE->validMove = true;
		}
	}
}