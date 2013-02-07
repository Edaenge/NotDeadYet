#include "Client.h"
#include <ClientServerMessages.h>

bool Client::AddActor(const std::vector<std::string>& msgArray, const long ID)
{
	int index = this->zObjectManager->SearchForActor(ID);

	if (index != -1)
	{
		MaloW::Debug("Cant create a new Player Object. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
		return false;
	}
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(0.05f, 0.05f, 0.05f);
	Vector4 rotation = Vector4(0, 0, 0, 0);
	std::string filename = "";

	Actor* actor = new Actor(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			actor->SetState(state);
		}
		else if(strcmp(key, M_SCALE.c_str()) == 0)
		{
			scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
		}
		//else if(strcmp(key, M_HEALTH.c_str()) == 0)
		//{
		//	float health = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
		//	playerObject->SetHealth(health);
		//}
		//else if(strcmp(key, M_HYDRATION.c_str()) == 0)
		//{
		//	float hydration = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
		//	playerObject->SetHydration(hydration);
		//}
		//else if(strcmp(key, M_STAMINA.c_str()) == 0)
		//{
		//	float stamina = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
		//	playerObject->SetStamina(stamina);
		//}
		//else if(strcmp(key, M_HUNGER.c_str()) == 0)
		//{
		//	float hunger = this->zMsgHandler.ConvertStringToInt(M_HEALTH, (*it));
		//	playerObject->SetHunger(hunger);
		//}
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
			this->zEng->GetCamera()->SetPosition(position + Vector3(0, 1.7, 0));
		}
	}
	if (Messages::FileWrite())
		Messages::Debug("Actor ID: " + MaloW::convertNrToString((float)ID) +" Added");
	
	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	actor->SetStaticMesh(mesh);

	this->zObjectManager->AddActor(actor);

	return true;
}

bool Client::UpdateActor(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	Updates update = Updates(ID);

	char key[512];
	for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
	{
		sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

		if(strcmp(key, M_POSITION.c_str()) == 0)
		{
			Vector3 position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
			update.SetPosition(position);
		}
		else if(strcmp(key, M_ROTATION.c_str()) == 0)
		{
			Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
			update.SetRotation(rotation);
		}
		else if(strcmp(key, M_STATE.c_str()) == 0)
		{
			int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
			update.SetState(state);
		}
		else
		{
			MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdatePlayerObjects");
		}
	}

	this->zObjectManager->AddUpdate(update);
	return true;
}

bool Client::RemoveActor(const long ID)
{
	if (ID == -1)
		return false;

	int index = this->zObjectManager->SearchForActor(ID);

	//Check if object was found in the array
	if(index == -1)
		return false;

	if(this->zID == ID)
	{
		this->CloseConnection("Unknown reason possible Kicked");
	}
	Actor* object = this->zObjectManager->GetActor(index);
	if (!object)
		return false;

	iMesh* mesh = object->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveActor(index))
	{
		MaloW::Debug("Failed To Remove Actor with ID: " + MaloW::convertNrToString((float)ID));
	}
	return true;
}
//
//bool Client::RemovePlayerObject(const long ID)
//{
//	if (ID == -1)
//		return false;
//
//	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_PLAYER, ID);
//
//	//Check if object was found in the array
//	if(index == -1)
//		return false;
//
//	if(this->zID == ID)
//	{
//		this->CloseConnection("Unknown reason possible Kicked");
//	}
//
//	iMesh* mesh = this->zObjectManager->GetPlayerObject(index)->GetMesh();
//
//	if(mesh)
//	{
//		this->zEng->DeleteMesh(mesh);
//	}
//	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_PLAYER, index))
//	{
//		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
//	}
//	return true;
//}
//
//bool Client::RemoveAnimalObject(const long ID)
//{
//	if (ID == -1)
//		return false;
//
//	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_ANIMAL, ID);
//
//	//Check if object was found in the array
//	if(index == -1)
//		return false;
//
//	iMesh* mesh = this->zObjectManager->GetAnimalObject(index)->GetMesh();
//
//	if(mesh)
//	{
//		this->zEng->DeleteMesh(mesh);
//	}
//	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_ANIMAL, index))
//	{
//		MaloW::Debug("Failed To Remove Animal with ID: " + MaloW::convertNrToString((float)ID));
//	}
//
//	return true;
//}
//
//bool Client::RemoveStaticObject(const long ID)
//{
//	if (ID == -1)
//		return false;
//
//	int index = this->zObjectManager->SearchForObject(OBJECT_TYPE_STATIC_OBJECT, ID);
//
//	//Check if object was found in the array
//	if(index == -1)
//		return false;
//
//	iMesh* mesh = this->zObjectManager->GetStaticObject(index)->GetMesh();
//
//	if(mesh)
//	{
//		this->zEng->DeleteMesh(mesh);
//	}
//	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_STATIC_OBJECT, index))
//	{
//		MaloW::Debug("Failed To Remove Static Object with ID: " + MaloW::convertNrToString((float)ID));
//		return false;
//	}
//	if(Messages::FileWrite())
//		Messages::Debug("Removed Static Object, Number of Objects remaining = " + MaloW::convertNrToString((float)this->zObjectManager->GetStaticObjects().size()));
//
//	return true;
//}
//
//bool Client::RemoveDynamicObject(const long ID)
//{
//	if (ID == -1)
//		return false;
//
//	int pos = this->zObjectManager->SearchForObject(OBJECT_TYPE_DYNAMIC_OBJECT, ID);
//
//	//Check if client was found in the array
//	if(pos == -1)
//		return false;
//
//	iMesh* mesh = this->zObjectManager->GetDynamicObject(pos)->GetMesh();
//
//	if(mesh)
//	{
//		this->zEng->DeleteMesh(mesh);
//	}
//	if(!this->zObjectManager->RemoveObject(OBJECT_TYPE_DYNAMIC_OBJECT, pos))
//	{
//		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
//	}
//
//	return true;
//}

void Client::HandleDeadPlayerMessage(const int ID)
{
	if (ID == -1)
		return;

	int index = this->zObjectManager->SearchForActor(ID);

	//Check if object was found in the array
	if(index == -1)
		return;

	if(this->zID == ID)
	{
		this->zCreated = false;
	}

	iMesh* mesh = this->zObjectManager->GetActor(index)->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zObjectManager->RemoveActor(index))
	{
		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
	}
	if(Messages::FileWrite())
		Messages::Debug("Player Died and was removed");
}