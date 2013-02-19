#include "Client.h"
#include <ClientServerMessages.h>

bool Client::AddActor(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	Actor* testActor = this->zActorManager->GetActor(ID);

	if (testActor)
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
	
	if (Messages::FileWrite())
		Messages::Debug("Actor ID: " + MaloW::convertNrToString((float)ID) +" Added");

	//Creates a StaticMesh from the given Filename
	iMesh* mesh = this->zEng->CreateStaticMesh(filename.c_str(), position);
	mesh->ResetRotation();
	mesh->SetQuaternion(rotation);
	mesh->SetScale(scale);

	//Create player data
	actor->SetStaticMesh(mesh);
	actor->SetModel(filename);

	if (!this->zCreated)
	{
		if (ID == this->zID)
		{
			if (this->zGuiManager)
				SAFE_DELETE(this->zGuiManager);

			this->zGuiManager = new GuiManager(this->zEng);
			this->zCreated = true;
			
			auto meshOffsetsIterator = this->zMeshCameraOffsets.find(filename);
			if (meshOffsetsIterator != this->zMeshCameraOffsets.end())
			{
				this->zMeshOffset = meshOffsetsIterator->second;
			}
			else
			{
				this->zMeshOffset = Vector3();
			}
			
			this->zActorManager->SetCameraOffset(this->zMeshOffset);
			this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset);
			this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);
		}
	}

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
		actor = this->zActorManager->GetActor(ID);

		update = this->zActorManager->GetUpdate(ID);
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

		actor = this->zActorManager->GetActor(ID);
		if (actor && ID != this->zID)
			actor->SetRotation(actorRotation);
	}
	Vector3 actorScale;
	for(auto scaleIterator = SFP->newScales.begin(); scaleIterator != SFP->newScales.end(); scaleIterator++)
	{
		ID = scaleIterator->first;
		actorScale = scaleIterator->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
			actor->SetScale(actorScale);
	}
	unsigned int actorState;
	for (auto stateIterator = SFP->newStates.begin(); stateIterator != SFP->newStates.end(); stateIterator++)
	{
		ID = stateIterator->first;
		actorState = stateIterator->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
		{
			if (this->zID == ID)
				this->UpdateCameraOffset(actorState);

			actor->SetState(actorState);
		}
	}
}

bool Client::RemoveActor(const unsigned int ID)
{
	if (ID == -1)
		return false;

	Actor* actor = this->zActorManager->GetActor(ID);

	//Check if object was found in the array
	if(!actor)
		return false;

	if(this->zID == ID)
	{
		this->CloseConnection("Unknown reason possible Kicked");
	}

	iMesh* mesh = actor->GetMesh();

	if(mesh)
	{
		
		this->zEng->DeleteMesh(mesh);
	}

	this->zActorManager->RemoveActor(ID);

	return true;
}