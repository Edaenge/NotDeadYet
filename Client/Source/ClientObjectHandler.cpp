#include "Client.h"
#include <ClientServerMessages.h>

void Client::AddActor( NewActorPacket* NAP )
{
	Actor* actor = NULL;
	unsigned int ID = 0;
	
	std::string model;
	auto it_actorModel_end = NAP->actorModel.end();
	for (auto it = NAP->actorModel.begin(); it != it_actorModel_end; it++)
	{
		ID = it->first;
		model = it->second;

		if (!this->zActorManager->GetActor(ID))
		{
			iMesh* mesh = NULL;
			//Creates a Mesh from the given Filename
			if (model.length() > 4)
			{
				std::string substring = model.substr(model.length() - 4);
				if (substring == ".obj")
				{
					mesh = this->zEng->CreateStaticMesh(model.c_str(), Vector3());
				}
				else if (substring == ".fbx")
				{
					mesh = this->zEng->CreateFBXMesh(model.c_str(), Vector3());
				}
				else if (substring == ".ani")
				{
					mesh = this->zEng->CreateAnimatedMesh(model.c_str(), Vector3());
				}
			}
			
			if(mesh)
			{
				actor = new Actor(ID);

				actor->SetMesh(mesh);
				actor->SetModel(model);

				if (Messages::FileWrite())
					Messages::Debug("Actor ID: " + MaloW::convertNrToString((float)ID) +" Added with Model: " + model);

				this->zActorManager->AddActor(actor);

				if (ID == this->zID)
				{
					this->zGuiManager->ResetGui();
					this->zPlayerInventory->ClearAll();

					this->zCreated = true;

					if (!this->zReady)
					{
						//this->zEng->DeleteImage(this->zBlackImage);
						//this->zBlackImage = NULL;
						this->zEng->LoadingScreen("media/loadingScreen/loadingscreenbg.png", "media/loadingscreen/loadingscreenpb.png", 0.0f, 0.2f, 0.2f, 0.2f);
						ambientMusic->Play();
						ambientMusic->Setvolume(0.2f);

						this->zReady = true;
					}

					auto meshOffsetsIterator = this->zMeshCameraOffsets.find(model);
					if (meshOffsetsIterator != this->zMeshCameraOffsets.end())
						this->zMeshOffset = meshOffsetsIterator->second;
					else
						this->zMeshOffset = Vector3(0.0f, 1.0f, 0.0f);

					this->zActorManager->SetCameraOffset(this->zMeshOffset);
					
					auto reader = this->zModelToReaderMap.find(model);
					if (reader != this->zModelToReaderMap.end())	
					{
						std::string boneName = reader->second.GetBindingBone(BONE_CAMERA_OFFSET);
						if (boneName != "")
							this->zEng->GetCamera()->SetMesh(mesh, &boneName[0], Vector3(0.0f, 0.0f, 1.0f));
						
						else 
						{
							this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
							this->zEng->GetCamera()->SetPosition(mesh->GetPosition() + this->zMeshOffset);
						}
					}
					else
					{
						this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
						this->zEng->GetCamera()->SetPosition(mesh->GetPosition() + this->zMeshOffset);
					}
					
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
				MaloW::Debug("Failed to load mesh with Model: " + model);
		}
		else
			MaloW::Debug("Cant create a new Actor. ID: " + MaloW::convertNrToString((float)ID) + " already exists");
	}

	Vector3 position;
	auto it_actorPosition_end = NAP->actorPosition.end();
	for (auto it = NAP->actorPosition.begin(); it != it_actorPosition_end; it++)
	{
		ID = it->first;
		position = it->second;

		actor = this->zActorManager->GetActor(ID);

		if (actor)
		{
			actor->SetPosition(position);

			//if (this->zID == ID)
				//this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);
		}
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}

	Vector4 rotation;
	auto it_actorRotation_end = NAP->actorRotation.end();
	for (auto it = NAP->actorRotation.begin(); it != it_actorRotation_end; it++)
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
	auto it_actorScale_end = NAP->actorScale.end();
	for (auto it = NAP->actorScale.begin(); it != it_actorScale_end; it++)
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
	auto it_actorState_end = NAP->actorState.end();
	for (auto it = NAP->actorState.begin(); it != it_actorState_end; it++)
	{
		ID = it->first;
		state = it->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
			this->zActorManager->AddActorState(actor, state);
		else
			MaloW::Debug("Failed to find Actor with ID: " + MaloW::convertNrToString((float)ID));
	}
}

void Client::UpdateActors(ServerFramePacket* SFP)
{
	Updates* update = NULL;
	Actor* actor = NULL;
	unsigned int ID = 0;

	Vector3 actorPosition;
	auto it_position_end = SFP->newPositions.end();
	for(auto positionIterator = SFP->newPositions.begin(); positionIterator != it_position_end; positionIterator++)
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
	auto it_rotation_end = SFP->newRotations.end();
	for(auto rotationIterator = SFP->newRotations.begin(); rotationIterator != it_rotation_end; rotationIterator++)
	{
		ID = rotationIterator->first;
		actorRotation = rotationIterator->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor && ID != this->zID)
			actor->SetRotation(actorRotation);
	}

	Vector3 actorScale;
	auto it_scale_end = SFP->newScales.end();
	for(auto scaleIterator = SFP->newScales.begin(); scaleIterator != it_scale_end; scaleIterator++)
	{
		ID = scaleIterator->first;
		actorScale = scaleIterator->second;

		actor = this->zActorManager->GetActor(ID);
		if (actor)
			actor->SetScale(actorScale);
	}

	unsigned int actorState;
	auto it_state_end = SFP->newStates.end();
	for (auto stateIterator = SFP->newStates.begin(); stateIterator != it_state_end; stateIterator++)
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

	auto it_anim_Queue_end = SFP->newAnimQueue.end();

	for (auto queueIterator = SFP->newAnimQueue.begin(); queueIterator != it_anim_Queue_end; queueIterator++)
	{
		ID = queueIterator->first;
		AnimationQueue queue = queueIterator->second;

		Actor* actor = this->zActorManager->GetActor(ID);

		if (!actor)
			continue;

		const int size = queue.zAnimations.size();
		if (size != queue.zAnimationTimes.size())
			MaloW::Debug("Size Mismatch for animation Queue");

		char** queueNames = new char*[size];
		float* queueTimes = new float[size];
		for (unsigned int i = 0; i < size; i++)
		{
			queueNames[i] = &queue.zAnimations[i][0];
			queueTimes[i] = queue.zAnimationTimes[i];
		}

		iFBXMesh* fbxMesh = dynamic_cast<iFBXMesh*>(actor->GetMesh());

		if (fbxMesh)
		{
			fbxMesh->SetAnimationQueue(queueNames, queueTimes, size);
		}
	}

	auto it_mesh_end = SFP->newMesh.end();
	for (auto meshIterator = SFP->newMesh.begin(); meshIterator != it_mesh_end; meshIterator++)
	{
		ID = meshIterator->first;
		std::string modelName = meshIterator->second;

		Actor* actor = this->zActorManager->GetActor(ID);
		if (!actor)
			continue;

		iMesh* mesh = NULL;
		//Creates a Mesh from the given Filename
		if (modelName.length() > 4)
		{
			std::string substring = modelName.substr(modelName.length() - 4);
			if (substring == ".obj")
			{
				mesh = this->zEng->CreateStaticMesh(modelName.c_str(), Vector3());
			}
			else if (substring == ".fbx")
			{
				mesh = this->zEng->CreateFBXMesh(modelName.c_str(), Vector3());
			}
			else if (substring == ".ani")
			{
				mesh = this->zEng->CreateAnimatedMesh(modelName.c_str(), Vector3());
			}
		}

		if (!mesh)
		{
			MaloW::Debug("Failed to Create new Mesh with ModelName " + modelName);
			continue;
		}

		iMesh* oldMesh = actor->GetMesh();

		Vector3 position = actor->GetPosition();
		Vector4 rotation = actor->GetRotation();
		Vector3 scale = actor->GetScale();

		if (this->zID == ID)
		{
			this->zEng->GetCamera()->RemoveMesh();

			auto meshOffsetsIterator = this->zMeshCameraOffsets.find(modelName);
			if (meshOffsetsIterator != this->zMeshCameraOffsets.end())
				this->zMeshOffset = meshOffsetsIterator->second;
			else
				this->zMeshOffset = Vector3(0.0f, 1.0f, 0.0f);

			this->zActorManager->SetCameraOffset(this->zMeshOffset);

			auto reader = this->zModelToReaderMap.find(modelName);
			if (reader != this->zModelToReaderMap.end())	
			{
				std::string boneName = reader->second.GetBindingBone(BONE_CAMERA_OFFSET);
				if (boneName != "")
					this->zEng->GetCamera()->SetMesh(mesh, &boneName[0], Vector3(0.0f, 0.0f, 1.0f));

				else 
				{
					this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
					this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);
				}
			}
			else
			{
				this->zEng->GetCamera()->SetMesh(mesh, this->zMeshOffset, Vector3(0.0f, 0.0f, 1.0f));
				this->zEng->GetCamera()->SetPosition(position + this->zMeshOffset);
			}
		}
		this->zEng->DeleteMesh(oldMesh);
		
		actor->SetMesh(mesh);
		actor->SetPosition(position);
		actor->SetRotation(rotation);
		actor->SetScale(scale);
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