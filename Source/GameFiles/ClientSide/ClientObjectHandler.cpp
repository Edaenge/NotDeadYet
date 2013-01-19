#include "Client.h"
#include "ClientServerMessages.h"

bool Client::AddNewPlayerObject(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
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
			this->zCreated = true;
		}
	}
	if (Messages::FileWrite())
		Messages::Debug("Player ID " + MaloW::convertNrToString((float)ID) +" Added");
	
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	playerObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(playerObject);

	return true;
}

bool Client::AddNewAnimalObject(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
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
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	animalObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(animalObject);

	return true;
}

bool Client::AddNewStaticObject(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
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
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	staticObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(staticObject);

	return true;
}

bool Client::AddNewDynamicObject(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				float y = terrain->GetYPositionAt(position.x, position.z);
				if (position.y < y)
					position.y = y;
			}
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
		Messages::Debug("Dynamic Object Added ID: " + MaloW::convertNrToString((float)ID) + " " + dynamicObject->GetName());

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->SetQuaternion(rotation);
	mesh->Scale(scale);

	//Create player data
	dynamicObject->SetStaticMesh(mesh);

	this->zObjectManager->AddObject(dynamicObject);

	return true;
}

bool Client::UpdatePlayerObjects(const std::vector<std::string>& msgArray, const int ID)
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
			//iTerrain* terrain = this->zObjectManager->GetTerrain();
			//if (terrain)
			//{
			//	position.y = terrain->GetYPositionAt(position.x, position.z);
			//}
			PlayerObjectPointer->SetNextPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
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
		Vector4 quat = PlayerObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (PlayerObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(PlayerObjectPointer->GetMesh());
		}
		PlayerObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateStaticObjects(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
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

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (StaticObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(StaticObjectPointer->GetMesh());
		}
		StaticObjectPointer->SetStaticMesh(mesh);
	}
	return true;
}

bool Client::UpdateAnimalObjects(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
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

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (AnimalObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(AnimalObjectPointer->GetMesh());
		}
		AnimalObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::UpdateDynamicObjects(const std::vector<std::string>& msgArray, const int ID)
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
			iTerrain* terrain = this->zObjectManager->GetTerrain();
			if (terrain)
			{
				position.y = terrain->GetYPositionAt(position.x, position.z);
			}
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
		Vector4 quat = DynamicObjectPointer->GetRotation();

		mesh->Scale(scale);
		mesh->SetQuaternion(Vector4(quat.x, quat.y, quat.z, quat.w));
		if (DynamicObjectPointer->HasMesh())
		{
			this->zEng->DeleteMesh(DynamicObjectPointer->GetMesh());
		}
		DynamicObjectPointer->SetStaticMesh(mesh);
	}

	return true;
}

bool Client::RemovePlayerObject(const int ID)
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

bool Client::RemoveAnimalObject(const int ID)
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

bool Client::RemoveStaticObject(const int ID)
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

bool Client::RemoveDynamicObject(const int ID)
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