#include "Client.h"
#include "ClientServerMessages.h"

bool Client::AddNewPlayerObject(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Player Object. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	PlayerObject* playerObject = new PlayerObject(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));

			playerObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			playerObject->SetState(state);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			playerObject->SetHealth(health);
		}
		else if(strcmp(key, M_HYDRATION.c_str()) == 0)
		{
			float hydration = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			playerObject->SetHydration(hydration);
		}
		else if(strcmp(key, M_STAMINA.c_str()) == 0)
		{
			float stamina = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			playerObject->SetStamina(stamina);
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			float hunger = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			playerObject->SetHunger(hunger);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewPlayerObject");
		}
	}
	if (!this->zCreated)
	{
		if (ID == this->zID)
		{
			if (this->zGuiManager)
				SAFE_DELETE(this->zGuiManager);

			this->zGuiManager = new GuiManager(this->zEng);
			this->zCreated = true;
			//filename = "Media/Models/Ball.obj";
			scale *= 0.1f;
			GetGraphics()->GetCamera()->SetPosition(position + Vector3(0, 1.7, 0));
		}
	}
	if (Messages::FileWrite())
		Messages::Debug("Player ID: " + MaloW::convertNrToString((float)ID) +" Added");
	
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	playerObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(playerObject);

	return true;
}

bool Client::CreateItemFromMessage(std::vector<std::string> msgArray, int& Index, Item** item, const long ID)
{
	std::string itemName = "Unknown";
	std::string itemDescription = "<UNKNOWN DESCRIPTION>";
	std::string itemIconFilePath = "none";
	int itemWeight = 0;
	int itemStackSize = 0;
	int itemType = -1;
	float weaponDamage = 0.0f;
	float weaponRange = 0.0f;
	float projectileDamage = 0.0f;
	float projectileVelocity = 0.0f;
	int craftingType = -1;
	int stacksRequired = 10000;
	float hunger = 0.0f;
	int maxUse = 0;
	int currUse = 0;

	char key[512];

	for (auto it = msgArray.begin() + Index + 1; (it < msgArray.end()) && (!strcmp(key, M_DEAD_PLAYER_ITEM_FINISHED.c_str()) == 0); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			itemDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			itemIconFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			itemWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
		}
		else if(strcmp(key, M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			itemStackSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
		}
		else if(strcmp(key, M_WEAPON_DAMAGE.c_str()) == 0)
		{
			weaponDamage = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_DAMAGE, (*it));
		}
		else if(strcmp(key, M_WEAPON_RANGE.c_str()) == 0)
		{
			weaponRange = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_RANGE, (*it));
		}
		else if(strcmp(key, M_PROJECTILE_VELOCITY.c_str()) == 0)
		{
			projectileVelocity = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_VELOCITY, (*it));
		}
		else if(strcmp(key, M_PROJECTILE_DAMAGE.c_str()) == 0)
		{
			projectileDamage = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_DAMAGE, (*it));
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
		}
		else if(strcmp(key, M_CONTAINER_MAX.c_str()) == 0)
		{
			maxUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_MAX, (*it));
		}
		else if(strcmp(key, M_CONTAINER_CURRENT.c_str()) == 0)
		{
			currUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_CURRENT, (*it));
		}
		else if(strcmp(key, M_MATERIAL_CRAFTING_TYPE.c_str()) == 0)
		{
			craftingType = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_CRAFTING_TYPE, (*it));
		}
		else if(strcmp(key, M_MATERIAL_STACKS_REQUIRED.c_str()) == 0)
		{
			stacksRequired = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_STACKS_REQUIRED, (*it));
		}
		Index++;
	}

	if (itemType == -1)
	{
		MaloW::Debug("Wrong or no Item Type sent from server in Client::AddDeadPlayerObject ItemType: " + MaloW::convertNrToString((float)itemType));
		return false;
	}
	//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
	switch (itemType)
	{
	case ITEM_TYPE_FOOD_DEER_MEAT:
		(*item) = new Food(ID, itemType, hunger);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_FOOD_WOLF_MEAT:
		(*item) = new Food(ID, itemType, hunger);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED_BOW:
		(*item) = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
		(*item)->SetItemName(itemName);
		(*item)->SetStacking(false);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_RANGED_ROCK:
		(*item) = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE_AXE:
		(*item) = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
		(*item) = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_CONTAINER_CANTEEN:
		(*item) = new Container(ID, itemType, maxUse, currUse);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_PROJECTILE_ARROW:
		(*item) = new Projectile(ID, itemType, projectileVelocity, projectileDamage);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL_SMALL_STICK:
		(*item) = new Material(ID, itemType, craftingType, stacksRequired);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL_MEDIUM_STICK:
		(*item) = new Material(ID, itemType, craftingType, stacksRequired);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL_THREAD:
		(*item) = new Material(ID, itemType, craftingType, stacksRequired);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_MATERIAL_LARGE_STICK:
		(*item) = new Material(ID, itemType, craftingType, stacksRequired);
		(*item)->SetStacking(true);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_HEAD:
		(*item) = new Gear(ID, itemType);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_CHEST:
		(*item) = new Gear(ID, itemType);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_LEGS:
		(*item) = new Gear(ID, itemType);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	case ITEM_TYPE_GEAR_BOOTS:
		(*item) = new Gear(ID, itemType);
		(*item)->SetStacking(false);
		(*item)->SetItemName(itemName);
		(*item)->SetItemWeight(itemWeight);
		(*item)->SetStackSize(itemStackSize);
		(*item)->SetIconPath(itemIconFilePath);
		(*item)->SetItemDescription(itemDescription);
		break;
	default:
		break;
	}
	return true;
}

bool Client::AddNewDeadPlayerObject(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_DEAD_PLAYER, ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Dead Player Object. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
		return false;
	}

	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";
	std::vector<Item*> items;
	
	DeadPlayerObject* deadPlayerObject = new DeadPlayerObject(ID);
	index = 0;
	char key[512];
	long id;
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));
		if(strcmp(key, M_DEAD_PLAYER_ADD_ITEM.c_str()) == 0)
		{
			id = this->zMsgHandler.ConvertStringToInt(M_DEAD_PLAYER_ADD_ITEM, (*it));
			//it++;
			//index++;
			Item* item = NULL;
			/*for(; (it < msgArray.end()) && (!strcmp(key, M_DEAD_PLAYER_ITEM_FINISHED.c_str()) == 0); it++)
			{
				sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

				if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
				{
					itemName = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
				}
				else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
				{
					itemDescription = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
				}
				else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
				{
					itemIconFilePath = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
				}
				else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
				{
					itemWeight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
				}
				else if(strcmp(key, M_ITEM_STACK_SIZE.c_str()) == 0)
				{
					itemStackSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it));
				}
				else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
				{
					itemType = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
				}
				else if(strcmp(key, M_WEAPON_DAMAGE.c_str()) == 0)
				{
					weaponDamage = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_DAMAGE, (*it));
				}
				else if(strcmp(key, M_WEAPON_RANGE.c_str()) == 0)
				{
					weaponRange = this->zMsgHandler.ConvertStringToFloat(M_WEAPON_RANGE, (*it));
				}
				else if(strcmp(key, M_PROJECTILE_VELOCITY.c_str()) == 0)
				{
					projectileVelocity = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_VELOCITY, (*it));
				}
				else if(strcmp(key, M_PROJECTILE_DAMAGE.c_str()) == 0)
				{
					projectileDamage = this->zMsgHandler.ConvertStringToFloat(M_PROJECTILE_DAMAGE, (*it));
				}
				else if(strcmp(key, M_HUNGER.c_str()) == 0)
				{
					hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
				}
				else if(strcmp(key, M_CONTAINER_MAX.c_str()) == 0)
				{
					maxUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_MAX, (*it));
				}
				else if(strcmp(key, M_CONTAINER_CURRENT.c_str()) == 0)
				{
					currUse = this->zMsgHandler.ConvertStringToInt(M_CONTAINER_CURRENT, (*it));
				}
				else if(strcmp(key, M_MATERIAL_CRAFTING_TYPE.c_str()) == 0)
				{
					craftingType = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_CRAFTING_TYPE, (*it));
				}
				else if(strcmp(key, M_MATERIAL_STACKS_REQUIRED.c_str()) == 0)
				{
					stacksRequired = this->zMsgHandler.ConvertStringToInt(M_MATERIAL_STACKS_REQUIRED, (*it));
				}
			}

			if (itemType == -1)
			{
				MaloW::Debug("Wrong or no Item Type sent from server in Client::AddDeadPlayerObject ItemType: " + MaloW::convertNrToString((float)itemType));
				return false;
			}
			//Todo add more data to item to identify type ex Bow/Axe/Pocket Knife
			switch (itemType)
			{
			case ITEM_TYPE_FOOD_DEER_MEAT:
				item = new Food(ID, itemType, hunger);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_FOOD_WOLF_MEAT:
				item = new Food(ID, itemType, hunger);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_WEAPON_RANGED_BOW:
				item = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
				item->SetItemName(itemName);
				item->SetStacking(false);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_WEAPON_RANGED_ROCK:
				item = new RangedWeapon(ID,itemType, weaponDamage, weaponRange);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_WEAPON_MELEE_AXE:
				item = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_WEAPON_MELEE_POCKET_KNIFE:
				item = new MeleeWeapon(ID, itemType, weaponDamage, weaponRange);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_CONTAINER_CANTEEN:
				item = new Container(ID, itemType, maxUse, currUse);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_PROJECTILE_ARROW:
				item = new Projectile(ID, itemType, projectileVelocity, projectileDamage);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_MATERIAL_SMALL_STICK:
				item = new Material(ID, itemType, craftingType, stacksRequired);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_MATERIAL_MEDIUM_STICK:
				item = new Material(ID, itemType, craftingType, stacksRequired);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_MATERIAL_THREAD:
				item = new Material(ID, itemType, craftingType, stacksRequired);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_MATERIAL_LARGE_STICK:
				item = new Material(ID, itemType, craftingType, stacksRequired);
				item->SetStacking(true);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_GEAR_HEAD:
				item = new Gear(ID, itemType);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_GEAR_CHEST:
				item = new Gear(ID, itemType);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_GEAR_LEGS:
				item = new Gear(ID, itemType);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			case ITEM_TYPE_GEAR_BOOTS:
				item = new Gear(ID, itemType);
				item->SetStacking(false);
				item->SetItemName(itemName);
				item->SetItemWeight(itemWeight);
				item->SetStackSize(itemStackSize);
				item->SetIconPath(itemIconFilePath);
				item->SetItemDescription(itemDescription);
				break;
			default:
				break;
			}*/
			this->CreateItemFromMessage(msgArray, index, &item, id);
			it = msgArray.begin() + (index);
			if (item)
			{
				items.push_back(item);
			}
		}
		else if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddDeadPlayerObject");
		}
		index++;
	}
		
	if (Messages::FileWrite())
		Messages::Debug("Dead Player ID: " + MaloW::convertNrToString((float)ID) +" Added");

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	deadPlayerObject->SetStaticMesh(mesh);
	deadPlayerObject->SetItems(items);
	this->zObjectManager->AddObject(deadPlayerObject);


	return true;
}

bool Client::AddNewAnimalObject(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Animal Object. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	AnimalObject* animalObject = new AnimalObject(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			try
			{
				position.y = zWorld->CalcHeightAtWorldPos(position.GetXZ()) + 0.1f;
			}
			catch(...)
			{

			}
			animalObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			animalObject->SetState(state);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			animalObject->SetHealth(health);
		}
		else if(strcmp(key, M_STAMINA.c_str()) == 0)
		{
			float stamina = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
			animalObject->SetStamina(stamina);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewAnimalObject");
		}
	}

	if (Messages::FileWrite())
		Messages::Debug("Animal Added ID: " + MaloW::convertNrToString((float)ID));

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	animalObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(animalObject);

	return true;
}

bool Client::AddNewStaticObject(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Static Object. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	StaticObject* staticObject = new StaticObject(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			position.y = zWorld->CalcHeightAtWorldPos(position.GetXZ()) + 0.1f;
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			staticObject->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			staticObject->SetWeight(weight);
		}
		else if(strcmp(key, M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			int stackSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it));
			staticObject->SetStackSize(stackSize);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			staticObject->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			staticObject->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			staticObject->SetIconPath(path);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewStaticObject");
		}
	}
	
	if (Messages::FileWrite())
		Messages::Debug("Static Object Added ID: " + MaloW::convertNrToString((float)ID) + " " + filename);

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);

	mesh->SetScale(scale);

	//Create player data
	staticObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(staticObject);

	return true;
}

bool Client::AddNewDynamicObject(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Dynamic Object. ID: " + MaloW::convertNrToString((float)ID) + "already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);

	std::string filename = "";
	DynamicObject* dynamicObject = new DynamicObject(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			
			dynamicObject->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			dynamicObject->SetState(state);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			dynamicObject->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			dynamicObject->SetWeight(weight);
		}
		else if(strcmp(key, M_ITEM_STACK_SIZE.c_str()) == 0)
		{
			int stackSize = this->zMsgHandler.ConvertStringToInt(M_ITEM_STACK_SIZE, (*it));
			dynamicObject->SetStackSize(stackSize);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			dynamicObject->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			dynamicObject->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			dynamicObject->SetIconPath(path);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("C: Unknown Message Was sent from server " + (*it) + " in AddNewDynamicObject");
		}
	}

	if (Messages::FileWrite())
		Messages::Debug("Dynamic Object Added ID: " + MaloW::convertNrToString((float)ID) + " " + filename);

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);

	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	dynamicObject->SetStaticMesh(mesh);
	dynamicObject->SetRotation(rotation);

	this->zObjectManager->AddObject(dynamicObject);

	return true;
}

bool Client::UpdatePlayerObjects(const std::vector<std::string>& msgArray, const long ID)
{
	//Get ID and Position Depending on type
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, ID);

	//Check if object was found
	if(index == -1)
		return false;

	PlayerObject* PlayerObjectPointer = this->zObjectManager->GetPlayerObject(index);

	if (!PlayerObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));

			PlayerObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			if(ID != this->zID)
				PlayerObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			PlayerObjectPointer->SetState(state);
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, (*it));
			PlayerObjectPointer->SetHealth(health);
		}
		else if(strcmp(key, M_HUNGER.c_str()) == 0)
		{
			float hunger = this->zMsgHandler.ConvertStringToFloat(M_HUNGER, (*it));
			PlayerObjectPointer->SetHunger(hunger);
		}
		else if(strcmp(key, M_HYDRATION.c_str()) == 0)
		{
			float hydration = this->zMsgHandler.ConvertStringToFloat(M_HYDRATION, (*it));
			PlayerObjectPointer->SetHydration(hydration);
		}
		else if(strcmp(key, M_STAMINA.c_str()) == 0)
		{
			float stamina = this->zMsgHandler.ConvertStringToFloat(M_STAMINA, (*it));
			PlayerObjectPointer->SetStamina(stamina);
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdatePlayerObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), PlayerObjectPointer->GetPosition());
		float scale = PlayerObjectPointer->GetScale().y;

		mesh->Scale(scale);

		if (PlayerObjectPointer->HasMesh())
		{
			iMesh* m = PlayerObjectPointer->GetMesh();
			this->zEng->DeleteMesh(m);
		}
		PlayerObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateStaticObjects(const std::vector<std::string>& msgArray, const long ID)
{
	//Get ID and Position Depending on type
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, ID);

	//Check if object was found
	if(index == -1)
		return false;

	StaticObject* StaticObjectPointer = this->zObjectManager->GetStaticObject(index);
	if (!StaticObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			position.y = zWorld->CalcHeightAtWorldPos(Vector2(position.x, position.z)) + 0.1f;
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			StaticObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			StaticObjectPointer->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			StaticObjectPointer->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			StaticObjectPointer->SetIconPath(path);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			StaticObjectPointer->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			StaticObjectPointer->SetWeight(weight);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateStaticObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), StaticObjectPointer->GetPosition());
		float scale = StaticObjectPointer->GetScale().y;
		Vector4 quat = StaticObjectPointer->GetRotation();

		mesh->ResetRotation();
		mesh->SetQuaternion(quat);
		mesh->SetScale(scale);

		if (StaticObjectPointer->HasMesh())
		{
			iMesh* m = StaticObjectPointer->GetMesh();
			this->zEng->DeleteMesh(m);
		}
		StaticObjectPointer->SetStaticMesh(mesh);
	}
	return true;
}

bool Client::UpdateAnimalObjects(const std::vector<std::string>& msgArray, const long ID)
{
	//Get ID and Position Depending on type
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, ID);
	//Check if object was found
	if(index == -1)
		return false;	

	AnimalObject* AnimalObjectPointer = this->zObjectManager->GetAnimalObject(index);

	if (!AnimalObjectPointer)
		return false;

	Vector3 position;
	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			position.y = zWorld->CalcHeightAtWorldPos(position.GetXZ()) + 0.1f;
			AnimalObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			AnimalObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			AnimalObjectPointer->SetState(state);
		}
		else if(strcmp(key, M_HEALTH.c_str()) == 0)
		{
			float health = this->zMsgHandler.ConvertStringToFloat(M_HEALTH, (*it));
			AnimalObjectPointer->SetHealth(health);
			if (Messages::FileWrite())
				Messages::Debug("Animal Health is: " + MaloW::convertNrToString(health));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateAnimalObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), AnimalObjectPointer->GetPosition());
		float scale = AnimalObjectPointer->GetScale().y;
		Vector4 quat = AnimalObjectPointer->GetRotation();

		mesh->ResetRotation();
		mesh->SetQuaternion(quat);
		mesh->Scale(scale);

		if (AnimalObjectPointer->HasMesh())
		{
			iMesh* mesh = AnimalObjectPointer->GetMesh();
			this->zEng->DeleteMesh(mesh);
		}
		AnimalObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateDynamicObjects(const std::vector<std::string>& msgArray, const long ID)
{
	//Get ID and Position Depending on type
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, ID);

	//Check if object was found
	if(index == -1)
		return false;

	DynamicObject* DynamicObjectPointer = this->zObjectManager->GetDynamicObject(index);

	if (DynamicObjectPointer == NULL)
		return false;

	Vector3 position;

	std::string filename;
	float serverTime = 0.0f;

	bool bFile = false;

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));

			DynamicObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			DynamicObjectPointer->SetRotation(rotation);
		}
		else if(strcmp(key, M_ITEM_NAME.c_str()) == 0)
		{
			std::string name = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_NAME, (*it));
			DynamicObjectPointer->SetName(name);
		}
		else if(strcmp(key, M_ITEM_DESCRIPTION.c_str()) == 0)
		{
			std::string description = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_DESCRIPTION, (*it));
			DynamicObjectPointer->SetDescription(description);
		}
		else if(strcmp(key, M_ITEM_ICON_PATH.c_str()) == 0)
		{
			std::string path = this->zMsgHandler.ConvertStringToSubstring(M_ITEM_ICON_PATH, (*it));
			DynamicObjectPointer->SetIconPath(path);
		}
		else if(strcmp(key, M_ITEM_TYPE.c_str()) == 0)
		{
			int type = this->zMsgHandler.ConvertStringToInt(M_ITEM_TYPE, (*it));
			DynamicObjectPointer->SetType(type);
		}
		else if(strcmp(key, M_ITEM_WEIGHT.c_str()) == 0)
		{
			int weight = this->zMsgHandler.ConvertStringToInt(M_ITEM_WEIGHT, (*it));
			DynamicObjectPointer->SetWeight(weight);
		}
		else if(strcmp(key, M_FRAME_TIME.c_str()) == 0)
		{
			serverTime = this->zMsgHandler.ConvertStringToFloat(M_FRAME_TIME, (*it));
		}
		else if(strcmp(key, M_MESH_MODEL.c_str()) == 0)
		{
			bFile = true;
			filename = this->zMsgHandler.ConvertStringToSubstring(M_MESH_MODEL, (*it));
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdateDynamicObjects");
		}
	}

	if (bFile)
	{
		//Create a new Mesh with the current values
		iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), DynamicObjectPointer->GetPosition());
		float scale = DynamicObjectPointer->GetScale().y;
		Vector4 quad = DynamicObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(quad);

		if (DynamicObjectPointer->HasMesh())
		{
			iMesh* m = DynamicObjectPointer->GetMesh();
			this->zEng->DeleteMesh(m);
		}
		DynamicObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::RemovePlayerObject(const long ID)
{
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, ID);

	//Check if object was found in the array
	if(index == -1)
		return false;

	if(this->zID == ID)
	{
		this->CloseConnection("Unknown reason possible Kicked");
	}

	iMesh* mesh = this->zObjectManager->GetPlayerObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_PLAYER, index))
	{
		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
	}
	return true;
}

bool Client::RemoveAnimalObject(const long ID)
{
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, ID);

	//Check if object was found in the array
	if(index == -1)
		return false;

	iMesh* mesh = this->zObjectManager->GetAnimalObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_ANIMAL, index))
	{
		MaloW::Debug("Failed To Remove Animal with ID: " + MaloW::convertNrToString((float)ID));
	}

	return true;
}

bool Client::RemoveStaticObject(const long ID)
{
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, ID);

	//Check if object was found in the array
	if(index == -1)
		return false;

	iMesh* mesh = this->zObjectManager->GetStaticObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_STATIC_OBJECT, index))
	{
		MaloW::Debug("Failed To Remove Static Object with ID: " + MaloW::convertNrToString((float)ID));
		return false;
	}
	if(Messages::FileWrite())
		Messages::Debug("Removed Static Object, Number of Objects remaining = " + MaloW::convertNrToString((float)this->zObjectManager->GetStaticObjects().size()));

	return true;
}

bool Client::RemoveDynamicObject(const long ID)
{
	if (ID == -1)
		return false;

	int pos = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, ID);

	//Check if client was found in the array
	if(pos == -1)
		return false;

	iMesh* mesh = this->zObjectManager->GetDynamicObject(pos)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_DYNAMIC_OBJECT, pos))
	{
		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
	}

	return true;
}

void Client::HandleDeadPlayerMessage(const int ID)
{
	if (ID == -1)
		return;

	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, ID);

	//Check if object was found in the array
	if(index == -1)
		return;

	if(this->zID == ID)
	{
		this->zCreated = false;
	}

	iMesh* mesh = this->zObjectManager->GetPlayerObject(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_PLAYER, index))
	{
		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
	}
	if(Messages::FileWrite())
		Messages::Debug("Player Died and was removed");
}