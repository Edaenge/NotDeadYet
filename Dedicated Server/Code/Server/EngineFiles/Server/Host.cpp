#include "Host.h"
#include "../../../../../Source/GameFiles/ClientServerMessages.h"

// 30 updates per sec
static const float UPDATE_DELAY = 0.0333f;

Host::Host()
{
	MaloW::ClearDebug();
	Messages::ClearDebug();

	this->zServerListener = NULL;
	this->zMaxClients = 10;
	this->zClients = std::vector<ClientData*>(); 
	this->zActorHandler = new ActorHandler();

	this->zStartime = 0;
	this->zSecsPerCnt = 0.0f;
	this->zDeltaTime = 0.0f;
	this->zTimeOut = 15.0f;
	this->zPingMessageInterval = 5.0f;
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
}
//NEEDS FIXING
void Host::Init()
{
	int counter = 0;
	//Creates A New FoodObject With an Id And Default Values 
	FoodObject* foodObj = NULL; /*new FoodObject(true);*/
	if(this->CreateStaticObjectActor(OBJECT_TYPE_FOOD_DEER_MEAT, &foodObj, true))
	{
		foodObj->SetPosition(Vector3(5.0f, 0.0f, 5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticFoodActor(foodObj);
		
		MaloW::Debug("Created Deer Meat Object ID: " + MaloW::convertNrToString(foodObj->GetID()));

		counter++;
	}
	foodObj = NULL; /*new FoodObject(true);*/
	if(this->CreateStaticObjectActor(OBJECT_TYPE_FOOD_WOLF_MEAT, &foodObj, true))
	{
		foodObj->SetPosition(Vector3(5.0f, 1.0f, 5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticFoodActor(foodObj);

		MaloW::Debug("Created Wolf Meat Object ID: " + MaloW::convertNrToString(foodObj->GetID()));

		counter++;
	}
	//Creates A New WeaponObject With an Id And Default Values 
	WeaponObject* weaponObj = NULL;/*new WeaponObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_BOW, &weaponObj, true))
	{
		weaponObj->SetPosition(Vector3(0.0f, 0.0f, -5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);


		MaloW::Debug("Created Bow Object ID: " + MaloW::convertNrToString(weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL;/*new WeaponObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_RANGED_ROCK, &weaponObj, true))
	{
		weaponObj->SetPosition(Vector3(2.0f, 0.0f, -5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);


		MaloW::Debug("Created Rock Object ID: " + MaloW::convertNrToString(weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL; /*new WeaponObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_AXE, &weaponObj, true))
	{
		weaponObj->SetPosition(Vector3(4.0f, 0.0f, -5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);
		
		MaloW::Debug("Created Axe Object ID: " + MaloW::convertNrToString(weaponObj->GetID()));

		counter++;
	}
	weaponObj = NULL; /*new WeaponObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_WEAPON_MELEE_POCKET_KNIFE, &weaponObj, true))
	{
		weaponObj->SetPosition(Vector3(6.0f, 0.0f, -5.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticWeaponActor(weaponObj);

		MaloW::Debug("Created Pocket Knife Object ID: " + MaloW::convertNrToString(weaponObj->GetID()));

		counter++;
	}

	ContainerObject* containerObj = NULL; /*new ContainerObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_CANTEEN, &containerObj, true))
	{
		containerObj->SetPosition(Vector3(-5.0f, 0.0f, 0.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticContainerActor(containerObj);
		
		MaloW::Debug("Created Canteen Object ID: " + MaloW::convertNrToString(containerObj->GetID()));

		counter++;
	}
	containerObj = NULL; /*new ContainerObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_CONTAINER_WATER_BOTTLE, &containerObj, true))
	{
		containerObj->SetPosition(Vector3(-5.0f, 0.0f, 2.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticContainerActor(containerObj);

		MaloW::Debug("Created Water Bottle Object ID: " + MaloW::convertNrToString(containerObj->GetID()));

		counter++;
	}
	StaticProjectileObject* projectileObj = NULL; /*new ContainerObject(true);*/
	if (this->CreateStaticObjectActor(OBJECT_TYPE_PROJECTILE_ARROW, &projectileObj, true))
	{
		projectileObj->SetPosition(Vector3(0.0f, 0.0f, -4.0f));
		//Adds The Object To the Array
		this->zActorHandler->AddNewStaticProjectileActor(projectileObj);

		MaloW::Debug("Created Arrow Object ID: " + MaloW::convertNrToString(projectileObj->GetID()));

		counter++;
	}
	MaloW::Debug("Created " + MaloW::convertNrToString(counter) + " Objects");
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
	
	while(this->stayAlive)
	{
		waitTimer += Update();

		//Checks if ServerListener is still working
		if(!this->zServerListener->IsAlive())
		{
			MaloW::Debug("Server Listener has died.");
		}

		HandleNewConnections();
		ReadMessages();
		HandleRecivedMessages();
		PingClients();
		UpdateObjects();
		
		if(waitTimer >= UPDATE_DELAY)
		{
			SendPlayerActorUpdates();
			SendAnimalActorUpdates();
			SendDynamicActorUpdates();
			waitTimer = 0.0f;
		}

		Sleep(5);
	}
}

int Host::InitHost( const int PORT, const unsigned int MAX_CLIENTS )
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

	client->setNotifier(this);
	this->zClients.push_back(new ClientData(client));

	message = this->zMessageConverter.Convert(MESSAGE_TYPE_SELF_ID, client->getClientID());
	client->Start();
	client->sendData(message);
	
	SAFE_DELETE(pe);

	//Collect player infos
	message = "";
	std::vector<std::string> temp;
	std::vector<PlayerActor *> players = this->zActorHandler->GetPlayers();

	//Gets PlayerInformation
	for(auto it = players.begin(); it < players.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		message =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, (*it)->GetID());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		message += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it)->GetState());

		temp.push_back(message);
	}

	//Send the players to player
	for (auto it = temp.begin(); it < temp.end(); it++)
	{
		client->sendData(*it);
	}

	//Sends All Static Objects To the Player
	std::vector<std::string> static_Msg;
	this->GetExistingObjects(static_Msg);
	for (auto sIt = static_Msg.begin(); sIt < static_Msg.end(); sIt++)
	{
		client->sendData(*sIt);
	}
	
}

void Host::SendToAllClients(const std::string& message)
{
	if(!HasClients())
		return;

	for(auto it = zClients.begin(); it < zClients.end(); it++)
	{
		(*it)->zClient->sendData(message);
	}
}

void Host::SendToClient(int clientID, const std::string& message)
{
	int pos = SearchForClient(clientID);

	if(pos == -1)
		return;

	this->zClients[pos]->zClient->sendData(message);
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_PLAYER, (*it_Player)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_FRAME_TIME, (*it_Player)->GetFrameTime());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it_Player)->GetState());
		(*it_Player)->AddChangedHData(mess, &this->zMessageConverter);

		playerData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = playerData.begin(); it_Message < playerData.end(); it_Message++)
		{
			(*it_Client)->zClient->sendData((*it_Message));
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_ANIMAL, (*it_Animal)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, (*it_Animal)->GetState());

		animalData.push_back(mess);
	}

	//Send Data to clients
	for (auto it_Client = zClients.begin(); it_Client < zClients.end(); it_Client++)
	{
		for (auto it_Message = animalData.begin(); it_Message < animalData.end(); it_Message++)
		{
			(*it_Client)->zClient->sendData((*it_Message));
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it_Static)->GetWeight());
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it_Static)->GetWeight());
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_STATIC_OBJECT, (*it_Static)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it_Static)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it_Static)->GetWeight());
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
			(*it_Client)->zClient->sendData((*it_Message));
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_UPDATE_DYNAMIC_OBJECT, (*it_Dynamic)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);

		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it_Dynamic)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it_Dynamic)->GetWeight());
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
			(*it_Client)->zClient->sendData((*it_Message));
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

bool Host::HandlePickupItem(PlayerActor* pActor, const int ObjectID)
{
	if(!HasClients())
		return false;

	//Check For FoodObject
	FoodObject* food = dynamic_cast<FoodObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_FOOD));

	if (food)
	{
		if (!pActor->PickUpObject(food))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Item " + food->GetActorObjectName());
			return false;
		}
		this->CreateItemFromObject(pActor, food);

		return true;
	}

	//Check For Weapon Object
	WeaponObject* weapon = dynamic_cast<WeaponObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_WEAPON));

	if (weapon)
	{
		if (!pActor->PickUpObject(weapon))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Item " + weapon->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, weapon);

		return true;
	}

	//Check For Container Object
	ContainerObject* container = dynamic_cast<ContainerObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_CONTAINER));

	if (container)
	{
		if (!pActor->PickUpObject(container))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Item " + container->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, container);

		return true;
	}

	//Check For Container Object
	StaticProjectileObject* projectile = dynamic_cast<StaticProjectileObject*>(this->zActorHandler->GetActor(ObjectID, ACTOR_TYPE_STATIC_OBJECT_PROJECTILE));

	if (projectile)
	{
		if (!pActor->PickUpObject(projectile))
		{
			this->SendErrorMessage(pActor->GetID(), "Failed_To_Pickup_Item " + projectile->GetActorObjectName());
			return false;
		}

		this->CreateItemFromObject(pActor, projectile);

		return true;
	}
	
	this->SendErrorMessage(pActor->GetID(), "Couldn't_Pickup_Object");

	return false;
}

void Host::HandleUnEquipItem(PlayerActor* pActor, const int ItemID, const int Slot)
{
	if (Slot == -1)
	{
		MaloW::Debug("Error In Host::UnEquip Item Slot is -1");
		return;
	}

	Inventory* inv = pActor->GetInventory();
	Equipment* eq = pActor->GetEquipment();

	if (Slot == EQUIPMENT_SLOT_AMMO)
	{
		Projectile* projectile = eq->GetAmmo();

		if (projectile)
		{
			if (projectile->GetID() == ItemID)
			{
				eq->UnEquipAmmo();

				inv->AddItem(projectile);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Ammo ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_WEAPON)
	{
		Weapon* wpn = eq->GetWeapon();

		if (wpn)
		{
			if (wpn->GetID() == ItemID)
			{
				eq->UnEquipWeapon();

				inv->AddItem(wpn);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Weapon ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_HEAD)
	{
		Gear* head = eq->GetGear(EQUIPMENT_SLOT_HEAD);

		if (head)
		{
			if (head->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_HEAD);

				inv->AddItem(head);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Head Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_CHEST)
	{
		Gear* chest = eq->GetGear(EQUIPMENT_SLOT_CHEST);

		if (chest)
		{
			if (chest->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_CHEST);

				inv->AddItem(chest);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Chest Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_LEGS)
	{
		Gear* legs = eq->GetGear(EQUIPMENT_SLOT_LEGS);

		if (legs)
		{
			if (legs->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_LEGS);

				inv->AddItem(legs);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Legs Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

	if (Slot == EQUIPMENT_SLOT_BOOTS)
	{
		Gear* boots = eq->GetGear(EQUIPMENT_SLOT_BOOTS);

		if (boots)
		{
			if (boots->GetID() == ItemID)
			{
				eq->UnEquipGear(EQUIPMENT_SLOT_BOOTS);

				inv->AddItem(boots);

				this->SendUnEquipMessage(pActor->GetID(), ItemID, Slot);

				return;
			}
			MaloW::Debug("Item With ID doesn't exist in Boots Slot ID: " + MaloW::convertNrToString(ItemID));
		}
		MaloW::Debug("Wrong Slot type, Item is Null in slot: " + MaloW::convertNrToString(Slot));
	}

}

void Host::SendUnEquipMessage(const int PlayerID, const int ID, const int Slot)
{
	std::string message = this->zMessageConverter.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, ID);
	message += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, Slot);

	this->SendToClient(PlayerID, message);
}

void Host::SendErrorMessage(const int id, const std::string error_Message)
{
	std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ERROR_MESSAGE, error_Message);
	this->SendToClient(id, msg);
}

void Host::HandleDropItem(PlayerActor* pActor, const int ItemID)
{	
	Item* item = NULL;
	item = pActor->GetItem(ItemID);

	if (!item)
	{
		this->SendErrorMessage(pActor->GetID(), "Failed To find Item");
		return;
	}
	int item_type = item->GetItemType();

	if (item_type == ITEM_TYPE_FOOD_DEER_MEAT)
	{
		Food* item_Food = dynamic_cast<Food*>(item);
		if (!item_Food)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Food);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}
		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_MELEE_AXE)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
	{
		Weapon* item_Weapon = dynamic_cast<Weapon*>(item);
		if (!item_Weapon)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Weapon);
		pActor->DropObject(ItemID);
		return;
	}
	if (item_type == ITEM_TYPE_CONTAINER_CANTEEN)
	{
		Container* item_Container = dynamic_cast<Container*>(item);
		if (!item_Container)
		{
			MaloW::Debug("Wrong Item Type Set");
			return;
		}

		this->CreateObjectFromItem(pActor, item_Container);
		pActor->DropObject(ItemID);
		return;
	}
}

bool Host::CreateStaticObjectActor(const int type, WeaponObject** weaponObj, const bool genID /*= false*/)
{
	//Get Default Values For a Weapon Object
	const WeaponObject* weapon = this->zActorHandler->GetObjManager()->GetWeaponObject(type);

	if (!weapon)
		return false;

	//Creates A New WeaponObject With an Id And Default Values 
	//weaponObj->SetType(type);
	//weaponObj->SetRange(weapon->GetRange());
	//weaponObj->SetWeight(weapon->GetWeight());
	//weaponObj->SetDamage(weapon->GetDamage());
	//weaponObj->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	//weaponObj->SetIconPath(weapon->GetIconPath());
	//weaponObj->SetActorModel(weapon->GetActorModel());
	//weaponObj->SetDescription(weapon->GetDescription());
	//weaponObj->SetActorObjectName(weapon->GetActorObjectName());

	*weaponObj = new WeaponObject(weapon, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, FoodObject** foodObj, const bool genID /*= false*/)
{
	//Get Default Values For a Meat Object
	const FoodObject* food = this->zActorHandler->GetObjManager()->GetFoodObject(type);

	if (!food)
		return false;

	//Creates A New WeaponObject With an Id And Default Values 
	//foodObj->SetType(type);
	//foodObj->SetWeight(food->GetWeight());
	//foodObj->SetHunger(food->GetHunger());
	//foodObj->SetIconPath(food->GetIconPath());
	//foodObj->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	//foodObj->SetActorModel(food->GetActorModel());
	//foodObj->SetDescription(food->GetDescription());
	//foodObj->SetActorObjectName(food->GetActorObjectName());

	*foodObj = new FoodObject(food, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, ContainerObject** containerObj, const bool genID /*= false*/)
{
	//Get Default Values For a container Object
	const ContainerObject* container = this->zActorHandler->GetObjManager()->GetContainerObject(type);

	if (!container)
		return false;

	//Creates A New WeaponObject With an Id And Default Values 
	//containerObj->SetType(type);
	//containerObj->SetWeight(container->GetWeight());
	//containerObj->SetMaxUses(container->GetMaxUses());
	//containerObj->SetIconPath(container->GetIconPath());
	//containerObj->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	//containerObj->SetActorModel(container->GetActorModel());
	//containerObj->SetDescription(container->GetDescription());
	//containerObj->SetCurrentUses(container->GetCurrentUses());
	//containerObj->SetActorObjectName(container->GetActorObjectName());

	*containerObj = new ContainerObject(container, genID);

	return true;
}

bool Host::CreateStaticObjectActor(const int type, StaticProjectileObject** projectileObj, bool genID /*= false*/)
{
	//Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zActorHandler->GetObjManager()->GetStaticProjectileObject(type);

	if (!projectile)
		return false;

	//Creates A New ProjectileObject With an Id And Default Values 
	//projectileObj->SetType(type);
	//projectileObj->SetWeight(projectile->GetWeight());
	//projectileObj->SetDamage(projectile->GetDamage());
	//projectileObj->SetIconPath(projectile->GetIconPath());
	//projectileObj->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	//projectileObj->SetActorModel(projectile->GetActorModel());
	//projectileObj->SetDescription(projectile->GetDescription());
	//projectileObj->SetVelocity(projectile->GetVelocity());
	//projectileObj->SetActorObjectName(projectile->GetActorObjectName());

	*projectileObj = new StaticProjectileObject(projectile, genID);

	return true;
}

bool Host::CreateDynamicObjectActor(const int type, DynamicProjectileObject** projectileObj, bool genID)
{
	////Get Default Values For a Projectile Object
	const StaticProjectileObject* projectile = this->zActorHandler->GetObjManager()->GetStaticProjectileObject(type);

	if (!projectile)
		return false;

	//Creates A New ProjectileObject With an Id And Default Values 
	(*projectileObj)->SetType(type);
	(*projectileObj)->SetWeight(projectile->GetWeight());
	(*projectileObj)->SetDamage(projectile->GetDamage());
	(*projectileObj)->SetIconPath(projectile->GetIconPath());
	(*projectileObj)->SetScale(Vector3(0.05f, 0.05f, 0.05f));
	(*projectileObj)->SetActorModel(projectile->GetActorModel());
	(*projectileObj)->SetDescription(projectile->GetDescription());
	(*projectileObj)->SetVelocity(projectile->GetVelocity());
	(*projectileObj)->SetActorObjectName(projectile->GetActorObjectName());

	//*projectileObj = new DynamicProjectileObject(projectile, genID);

	return true;
}

void Host::SendNewObjectMessage(StaticObjectActor* staticObj)
{
	std::string msg;
	Vector3 pos = staticObj->GetPosition();
	Vector3 scale = staticObj->GetScale();
	Vector4 rot = staticObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, staticObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, staticObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, staticObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, staticObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, staticObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, staticObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, staticObj->GetIconPath());

	this->SendToAllClients(msg);
}

void Host::SendNewObjectMessage(DynamicObjectActor* dynamicObj)
{
	std::string msg;
	Vector3 pos = dynamicObj->GetPosition();
	Vector3 scale = dynamicObj->GetScale();
	Vector4 rot = dynamicObj->GetRotation();

	msg = this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_DYNAMIC_OBJECT, dynamicObj->GetID());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, dynamicObj->GetActorModel());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, dynamicObj->GetType());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, dynamicObj->GetWeight());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, dynamicObj->GetActorObjectName());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, dynamicObj->GetDescription());
	msg += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, dynamicObj->GetIconPath());
	//ProjectileObject* projectileObj = dynamic_cast<ProjectileObject*>(dynamicObj);
	//if (projectileObj)
	//{
		//msg += this->zMessageConverter.Convert(MESSAGE_TYPE_WEAPON_DAMAGE, projectileObj->GetDamage());
	//}
	
	this->SendToAllClients(msg);
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
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, animalObj->GetState());
	msg +=  this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, animalObj->GetActorModel());

	this->SendToAllClients(msg);
}

void Host::HandleWeaponUse(PlayerActor* pActor, const int ItemID)
{
	Equipment* eq = pActor->GetEquipment();
	Weapon* weapon = eq->GetWeapon();

	if (!weapon)
	{
		SendErrorMessage(pActor->GetID(),"No Weapon is Equipped");
		return;
	}
	if (weapon->GetID() != ItemID)
	{
		MaloW::Debug("Server weapon isn't the same as Client Weapon");
	}

	float damage;
	float range;
	RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(weapon);

	if (rWpn)
	{
		float velocity = 5.0f;
		rWpn->UseWeapon(range, damage);

		Projectile* projectile = eq->GetAmmo();

		//Create Dynamic Object with player direction
		Vector3 direction = pActor->GetDirection();

		DynamicProjectileObject* projectileObj = new DynamicProjectileObject(true);

		int type = -1;
		if (projectile)
			type = projectile->GetItemType();
		else
			type = rWpn->GetItemType();

		if(!this->CreateDynamicObjectActor(type, &projectileObj, true))
		{
			MaloW::Debug("Failed to Create Projectile");
			SAFE_DELETE(projectileObj);
			return;
		}
		MaloW::Debug("Created projectile ID: " + MaloW::convertNrToString(projectileObj->GetID()));


		if (projectile)
		{
			damage += projectile->GetDamage();
			velocity = projectile->GetVelocity();
		}

		Vector3 position = pActor->GetPosition();
		projectileObj->SetPosition(position);
		projectileObj->SetDirection(direction);
		projectileObj->SetDamage(damage);
		projectileObj->SetVelocity(velocity);
		//Adds The Object To the Array
		this->zActorHandler->AddNewDynamicProjectileActor(projectileObj);

		SendNewObjectMessage(projectileObj);

		return;
	}

	MaloW::Debug("dynamic cast Failed in Host::WeaponUse (Ranged Weapon)");

	MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(weapon);

	if (mWpn)
	{
		mWpn->UseWeapon(range, damage);
		//Check Collision
		Vector3 direction = pActor->GetDirection();
		Vector3 position = pActor->GetPosition() + (direction * range);

		return;
	}

	MaloW::Debug("dynamic cast Failed in Host::WeaponUse (Melee Weapon)");

	return;
}

bool Host::CheckCollision(Vector3 position)
{
	return false;
}

void Host::HandleItemUse(PlayerActor* pActor, const int ItemID )
{
	Inventory* inv = pActor->GetInventory();

	Item* item = inv->SearchAndGetItem(ItemID);

	if (!item)
	{
		this->SendErrorMessage(pActor->GetID(), "Item Doesn't Exist");
		return;
	}
	if (item->GetItemType() == ITEM_TYPE_FOOD_DEER_MEAT || item->GetItemType() == ITEM_TYPE_FOOD_WOLF_MEAT)
	{
		Food* food = dynamic_cast<Food*>(item);

		if (!food)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Food)");
			return;
		}

		if (!food->Use())
		{
			this->SendErrorMessage(pActor->GetID(), "Stack is Empty");
			return;
		}
		pActor->EatFood(food->GetHunger());

		std::string msg;

		msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, food->GetID());
		this->SendToClient(pActor->GetID(), msg);
		if (food->GetStackSize() <= 0)
		{
			pActor->GetInventory()->RemoveItem(food);
		}
		

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_CONTAINER_CANTEEN || item->GetItemType() == ITEM_TYPE_CONTAINER_WATER_BOTTLE)
	{
		Container* container = dynamic_cast<Container*>(item);

		if (!container)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Container)");
			return;
		}

		if (!container->Use())
		{
			this->SendErrorMessage(pActor->GetID(), "Stack is Empty");
			return;
		}
		float hydration = (container->GetRemainingUses() + 1) * 15.0f;
		pActor->Drink(hydration);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_USE, container->GetID());
		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_BOW)
	{
		Equipment* eq = pActor->GetEquipment();

		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);

		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Bow)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + rWpn->GetItemName());
		eq->EquipWeapon(rWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, rWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED_ROCK)
	{
		Equipment* eq = pActor->GetEquipment();

		RangedWeapon* rWpn = dynamic_cast<RangedWeapon*>(item);

		if (!rWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Rock)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + rWpn->GetItemName());
		eq->EquipWeapon(rWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, rWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_AXE)
	{
		Equipment* eq = pActor->GetEquipment();

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);

		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Axe)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + mWpn->GetItemName());
		eq->EquipWeapon(mWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, mWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE)
	{
		Equipment* eq = pActor->GetEquipment();

		MeleeWeapon* mWpn = dynamic_cast<MeleeWeapon*>(item);

		if (!mWpn)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Pocket Knife)");
			return;
		}

		inv->EquipItem(ItemID);

		Weapon* oldWeapon = eq->GetWeapon();

		if (oldWeapon)
		{
			inv->AddItem(oldWeapon);
		}
		MaloW::Debug("Weapon Equipped" + mWpn->GetItemName());
		eq->EquipWeapon(mWpn);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, mWpn->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_WEAPON);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
	if (item->GetItemType() == ITEM_TYPE_PROJECTILE_ARROW)
	{
		Equipment* eq = pActor->GetEquipment();

		Projectile* arrow = dynamic_cast<Projectile*>(item);

		if (!arrow)
		{
			MaloW::Debug("dynamic cast Failed in Host::UseItem (Arrow)");
			return;
		}

		inv->EquipItem(ItemID);

		Projectile* oldProjectile = eq->GetAmmo();

		
		if (oldProjectile)
		{
			inv->AddItem(oldProjectile);
		}
		MaloW::Debug("Weapon Equipped" + arrow->GetItemName());
		eq->EquipAmmo(arrow);

		std::string msg = this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIP_ITEM, arrow->GetID());
		msg += this->zMessageConverter.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, EQUIPMENT_SLOT_AMMO);

		this->SendToClient(pActor->GetID(), msg);

		return;
	}
}

void Host::HandleRecivedMessages()
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
			this->HandlePingMsg(this->zClients[c_index]);
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
			MaloW::Debug("Warning: The host cannot handle the message \""+(*it)->getMessage()+"\" in HandleRecivedMessages.");
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

void Host::HandlePingMsg(ClientData* cd)
{
	//Hard coded
	if(cd->zTotalPingTime > 10.0f)
		cd->ResetPingCounter();

	cd->zTotalPingTime += cd->zCurrentPingTime;
	cd->zNrOfPings++;

	cd->zPinged = false;
	cd->zCurrentPingTime = 0.0f;
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
	if( cd->zNrOfPings == 0)
		return;

	float latency = cd->zTotalPingTime / cd->zNrOfPings;
	pl->SetLatency(latency);
}

int Host::SearchForClient(const int ID) const
{
	if(!HasClients())
		return -1;

	for (unsigned int i = 0; i < this->zClients.size(); i++)
	{
		if(this->zClients.at(i)->zClient->getClientID() == ID)
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

	for(unsigned int i = 0; i < (unsigned int)zClients.size(); i++)
	{
		cd = zClients.at(i);

		//If client has not been pinged.
		if(!cd->zPinged)
		{
			//If it was x sec ago we sent a ping, don't send a ping.
			if(cd->zCurrentPingTime < zPingMessageInterval)
				cd->IncPingTime(zDeltaTime);

			//else send ping.
			else
			{
				cd->zCurrentPingTime = 0.0f;
				cd->zClient->sendData(this->zMessageConverter.Convert(MESSAGE_TYPE_PING));
				cd->zPinged = true;
			}
		}
		//If he have sent a ping.
		else
		{
			//If we sent a ping x sec ago, drop the client.
			if(cd->zCurrentPingTime > zTimeOut)
			{
				KickClient(cd->zClient->getClientID());
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
	this->zActorHandler->UpdateObjects(zDeltaTime);

	std::vector<DynamicProjectileObject*> dynamicProjectileObj = this->zActorHandler->GetDynamicProjectiles();
	std::vector<DynamicProjectileObject*> toBeRemoved;
	for (auto it_proj = dynamicProjectileObj.begin(); it_proj < dynamicProjectileObj.end(); it_proj++)
	{
		if ( !((*it_proj)->IsMoving()) )
		{
			HandleConversion((*it_proj));
			toBeRemoved.push_back((*it_proj));
		}
	}

	for (auto it_proj = toBeRemoved.begin(); it_proj < toBeRemoved.end(); it_proj++)
	{
		if ( !((*it_proj)->IsMoving()) )
		{
			if(!this->zActorHandler->RemoveDynamicProjectileActor((*it_proj)->GetID()))
				MaloW::Debug("Failed to Remove Object in Host::UpdateObjects");
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

		temp_c->zClient->sendData(mess);
	}

	//create a remove player message.
	mess = this->zMessageConverter.Convert(MESSAGE_TYPE_REMOVE_PLAYER, ID);

	//remove the player

	this->zClients.erase(zClients.begin() + index);
	this->zActorHandler->RemovePlayerActor(ID);

	SAFE_DELETE(temp_c);

	removed = true;
	MaloW::Debug("Client"+MaloW::convertNrToString(ID)+" removed from server.");
	

	//Notify clients
	this->SendToAllClients(mess);

	return removed;
}

bool Host::IsAlive() const
{
	return this->stayAlive;
}

void Host::CreateNewPlayer(ClientData* cd, const std::vector<std::string> &data )
{
	std::string mess;
	
	PlayerActor* pi = new PlayerActor(cd->zClient->getClientID());

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
	pi->SetPosition(Vector3(pi->GetID()*15,0,1)); 
	//Add new player to the list
	this->zActorHandler->AddNewPlayer(pi);

	//Gather New player information
	Vector3 pos = pi->GetPosition();
	Vector3 scale = pi->GetScale();
	Vector4 rot = pi->GetRotation();

	mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_PLAYER, pi->GetID());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, pi->GetActorModel());
	mess += this->zMessageConverter.Convert(MESSAGE_TYPE_STATE, pi->GetState());

	//Send new player to players
	for (unsigned int i = 0; i < (unsigned int)this->zClients.size(); i++)
	{
		this->zClients[i]->zClient->sendData(mess);
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

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);

	}
	std::vector<WeaponObject *> static_Weapon = this->zActorHandler->GetWeapons();
	//Gets Static Weapon Objects Data
	for(auto it = static_Weapon.begin(); it < static_Weapon.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}

	std::vector<ContainerObject *> static_Container = this->zActorHandler->GetContainers();
	//Gets Static Weapon Objects Data
	for(auto it = static_Container.begin(); it < static_Container.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}

	std::vector<StaticProjectileObject *> static_Projectile = this->zActorHandler->GetStaticProjectiles();
	//Gets Static Weapon Objects Data
	for(auto it = static_Projectile.begin(); it < static_Projectile.end(); it++)
	{
		Vector3 pos = (*it)->GetPosition();
		Vector3 scale = (*it)->GetScale();
		Vector4 rot = (*it)->GetRotation();

		mess =  this->zMessageConverter.Convert(MESSAGE_TYPE_NEW_STATIC_OBJECT, (*it)->GetID());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_MESH_MODEL, (*it)->GetActorModel());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_TYPE, (*it)->GetType());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_NAME, (*it)->GetActorObjectName());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_WEIGHT, (*it)->GetWeight());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_ICON_PATH, (*it)->GetIconPath());
		mess += this->zMessageConverter.Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, (*it)->GetDescription());

		static_Objects.push_back(mess);
	}
}

int Host::GetNrOfPlayers() const
{
	return this->zClients.size();
}