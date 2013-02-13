#include "Client.h"
#include <ClientServerMessages.h>

bool Client::AddActor(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	int index = this->zActorManager->SearchForActor(ID);

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
			//scale *= 2.1f;
			this->zEng->GetCamera()->SetPosition(position + Vector3(0.0f, 1.7f, 0.0f));
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

	this->zActorManager->AddActor(actor);

	return true;
}

void Client::UpdateActors(ServerFramePacket* SFP)
{
	Updates* update = NULL;
	Actor* actor = NULL;
	unsigned int ID = 0;
	Vector3 actorPosition;
	for(auto positionIterator = SFP->newPositions.begin(); positionIterator != SFP->newPositions.end(); positionIterator++)
	{
		ID = positionIterator->first;
		actorPosition = positionIterator->second;
		actor = this->zActorManager->SearchAndGetActor(ID);

		update = this->zActorManager->SearchAndGetUpdate(ID);
		if (update)
		{
			update->SetPosition(actorPosition);
		}
		else
		{
			update = new Updates(ID);
			update->SetPosition(actorPosition);
			this->zActorManager->AddUpdate(update);
		}

	}
	Vector4 actorRotation;
	for(auto rotationIterator = SFP->newRotations.begin(); rotationIterator != SFP->newRotations.end(); rotationIterator++)
	{
		ID = rotationIterator->first;
		actorRotation = rotationIterator->second;

		actor = this->zActorManager->SearchAndGetActor(ID);
		if (actor && ID != this->zID)
			actor->SetRotation(actorRotation);
	}
	Vector3 actorScale;
	for(auto scaleIterator = SFP->newScales.begin(); scaleIterator != SFP->newScales.end(); scaleIterator++)
	{
		ID = scaleIterator->first;
		actorScale = scaleIterator->second;

		actor = this->zActorManager->SearchAndGetActor(ID);
		if (actor)
			actor->SetScale(actorScale);
	}
}

bool Client::UpdateActor(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	int index = this->zActorManager->SearchForUpdate(ID);
	Updates* update = NULL;
	if (index != -1)
		update = this->zActorManager->GetUpdate(index);
	else
		update = new Updates(ID);
	
	Actor* actor = this->zActorManager->SearchAndGetActor(ID);
	if (actor)
	{
		char key[512];
		for(auto it = msgArray.begin() + 1; it < msgArray.end(); it++)
		{
			sscanf_s((*it).c_str(), "%s ", &key, sizeof(key));

			if(strcmp(key, M_POSITION.c_str()) == 0)
			{
				Vector3 position = this->zMsgHandler.ConvertStringToVector(M_POSITION, (*it));
				update->SetPosition(position);
			}
			else if(strcmp(key, M_ROTATION.c_str()) == 0)
			{
				if (ID != this->zID)
				{
					Vector4 rotation = this->zMsgHandler.ConvertStringToQuaternion(M_ROTATION, (*it));
					actor->SetRotation(rotation);
				}
			}
			else if(strcmp(key, M_STATE.c_str()) == 0)
			{
				int state = this->zMsgHandler.ConvertStringToInt(M_STATE, (*it));
				actor->SetState(state);
			}
			else if(strcmp(key, M_SCALE.c_str()) == 0)
			{
				Vector3 scale = this->zMsgHandler.ConvertStringToVector(M_SCALE, (*it));
				actor->SetScale(scale);
			}
			else
			{
				MaloW::Debug("Client: Unknown Message Was sent from server - " + (*it) + " - in UpdatePlayerObjects");
			}
		}
		if (index == -1)
			this->zActorManager->AddUpdate(update);

		return true;
	}
	return false;
}

bool Client::RemoveActor(const unsigned int ID)
{
	if (ID == -1)
		return false;

	int index = this->zActorManager->SearchForActor(ID);

	//Check if object was found in the array
	if(index == -1)
		return false;

	if(this->zID == ID)
	{
		this->CloseConnection("Unknown reason possible Kicked");
	}
	Actor* object = this->zActorManager->GetActor(index);
	if (!object)
		return false;

	iMesh* mesh = object->GetMesh();

	if(mesh)
	{
		this->zEng->DeleteMesh(mesh);
	}
	if(!this->zActorManager->RemoveActor(index))
	{
		MaloW::Debug("Failed To Remove Actor with ID: " + MaloW::convertNrToString((float)ID));
	}
	return true;
}
//
//bool Client::RemovePlayerObject(const unsigned int ID)
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
//bool Client::RemoveAnimalObject(const unsigned int ID)
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
//bool Client::RemoveStaticObject(const unsigned int ID)
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
//bool Client::RemoveDynamicObject(const unsigned int ID)
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
//
//void Client::HandleDeadPlayerMessage(const unsigned int ID)
//{
//	if (ID == -1)
//		return;
//
//	int index = this->zObjectManager->SearchForActor(ID);
//
//	//Check if object was found in the array
//	if(index == -1)
//		return;
//
//	if(this->zID == ID)
//	{
//		this->zCreated = false;
//	}
//
//	iMesh* mesh = this->zObjectManager->GetActor(index)->GetMesh();
//
//	if(mesh)
//	{
//		this->zEng->DeleteMesh(mesh);
//	}
//	if(!this->zObjectManager->RemoveActor(index))
//	{
//		MaloW::Debug("Failed To Remove Player with ID: " + MaloW::convertNrToString((float)ID));
//	}
//	if(Messages::FileWrite())
//		Messages::Debug("Player Died and was removed");
//}