#include "Client.h"
#include <ClientServerMessages.h>

bool Client::AddActor(const std::vector<std::string>& msgArray, const unsigned int ID)
{
	if (this->zActorManager->GetActor(ID))
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
			this->zActorManager->AddActorState(actor, state);
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
				this->zMeshOffset = Vector3(0.0f, 0.5f, 0.0f);
			}

			this->zActorManager->SetCameraOffset(this->zMeshOffset);
			this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset);
			this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);

			if (this->zActorType == GHOST)
			{
				this->zPam->ToggleMenu(); // Shows the menu and sets Show to true.
				if(this->zPam->GetShow())
					zShowCursor = true;
				else
					zShowCursor = false;
			}
		}
	}

	this->zActorManager->AddActor(actor);

	return true;
}

void Client::AddActor( NewActorPacket* NAP )
{
	Actor* actor = NULL;
	unsigned int ID = 0;
	
	std::string model;
	for (auto it = NAP->actorModel.begin(); it != NAP->actorModel.end(); it++)
	{
		ID = it->first;
		model = it->second;

		if (!this->zActorManager->GetActor(ID))
		{
			actor = new Actor(ID);
			
			iMesh* mesh = NULL;
			//Creates a Mesh from the given Filename
			std::string substring = model.substr(model.length() - 4);
			if (substring == ".obj" || substring == ".ani")
				mesh = this->zEng->CreateStaticMesh(model.c_str(), Vector3());
			else if (substring == ".fbx")
				mesh = this->zEng->CreateFBXMesh(model.c_str(), Vector3());
			
			actor->SetStaticMesh(mesh);
			actor->SetModel(model);

			if (Messages::FileWrite())
				Messages::Debug("Actor ID: " + MaloW::convertNrToString((float)ID) +" Added with Model: " + model);

			this->zActorManager->AddActor(actor);

			if (ID == this->zID)
			{
				if (this->zGuiManager)
					SAFE_DELETE(this->zGuiManager);

				this->zGuiManager = new GuiManager(this->zEng);
				this->zCreated = true;

				auto meshOffsetsIterator = this->zMeshCameraOffsets.find(model);
				if (meshOffsetsIterator != this->zMeshCameraOffsets.end())
					this->zMeshOffset = meshOffsetsIterator->second;
				else
					this->zMeshOffset = Vector3(0.0f, 1.0f, 0.0f);

				this->zActorManager->SetCameraOffset(this->zMeshOffset);
				this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
				this->zEng->GetCamera()->SetPosition(mesh->GetPosition() + this->zMeshOffset);

				if (this->zActorType == GHOST)
				{
					this->zPam->ToggleMenu(); // Shows the menu and sets Show to true.
					if(this->zPam->GetShow())
						zShowCursor = true;
					else
						zShowCursor = false;
				}
			}
		}
		else
			MaloW::Debug("Cant create a new Actor. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
	}

	Vector3 position;
	for (auto it = NAP->actorPosition.begin(); it != NAP->actorPosition.end(); it++)
	{
		ID = it->first;
		position = it->second;

		actor = this->zActorManager->GetActor(ID);

		if (actor)
		{
			actor->SetPosition(position);

			if (this->zID == ID)
				this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);
		}
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}

	Vector4 rotation;
	for (auto it = NAP->actorRotation.begin(); it != NAP->actorRotation.end(); it++)
	{
		ID = it->first;
		rotation = it->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
		{
			if (actor->GetMesh())
				actor->SetRotation(rotation);
			else
				MaloW::Debug("Failed to find Mesh for Actor with ID: " + MaloW::convertNrToString((float)ID));
		}
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}

	Vector3 scale;
	for (auto it = NAP->actorScale.begin(); it != NAP->actorScale.end(); it++)
	{
		ID = it->first;
		scale = it->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
		{
			if (actor->GetMesh())
				actor->SetScale(scale);
			else
				MaloW::Debug("Failed to find Mesh for Actor with ID: " + MaloW::convertNrToString((float)ID));
		}
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}

	unsigned int state;
	for (auto it = NAP->actorState.begin(); it != NAP->actorState.end(); it++)
	{
		ID = it->first;
		state = it->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
			this->zActorManager->AddActorState(actor, state);
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}

	if (!this->zReady)
	{
		//this->zEng->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 0.0f, 1.0f, 0.2f, 0.2f);
		this->zReady = true;

		this->zServerChannel->Send(this->zMsgHandler.Convert(MESSAGE_TYPE_PLAYER_READY));
	}
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

		update = this->zActorManager->GetUpdate(ID);
		if (update)
		{
			update->SetState(actorState);
		}
		else
		{
			update = new Updates(ID);
			update->SetState(actorState);
			this->zActorManager->AddUpdate(update);
		}
		if (actor)
		{
			if (this->zID == ID)
				this->UpdateCameraOffset(actorState);
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