#include "Actor.h"


Actor::Actor(const unsigned int& ID) :
	zMesh(0),
	zID(ID)
{
}

Actor::~Actor()
{
	GraphicsEngine* graphics = GetGraphics();

	if (this->zMesh)
	{
		if ( iFBXMesh* fbxMesh = dynamic_cast<iFBXMesh*>(this->zMesh) )
		{
			graphics->DeleteFBXMesh( fbxMesh );
		}
		else
		{
			graphics->DeleteMesh(this->zMesh);
		}

		this->zMesh = NULL;
	}

	

	for (auto it = this->zSubMeshes.begin(); it != this->zSubMeshes.end(); it++)
	{
		if( it->second )
		{
			if( iFBXMesh* fbxMesh = dynamic_cast<iFBXMesh*>(this->zMesh) )
			{
				graphics->DeleteFBXMesh(fbxMesh);
			}
			else
			{
				graphics->DeleteMesh(it->second);
			}
		}

		it->second = NULL;
	}
}

void Actor::SetModel(const std::string& model) 
{
	if ( zModel != model )
	{
		this->zModel = model;

		// Notify Observers
		ActorChangedModelEvent ACME;
		ACME.zActor = this;
		NotifyObservers(&ACME);
	}
}

void Actor::SetPosition(const Vector3& pos, const bool& updateMesh)
{
	if ( zPosition != pos )
	{
		// Update Position
		zPosition = pos;

		// Update Mesh
		if ( updateMesh && this->zMesh )
		{
			this->zMesh->SetPosition(pos);
		}

		// Notify Movement
		ActorMovedEvent AME;
		AME.zActor = this;
		NotifyObservers(&AME);
	}
}

void Actor::SetScale(const Vector3& scale) 
{
	this->zMesh->SetScale(scale);
}

void Actor::SetRotation(const Vector4& rot)
{
	if (
		zRotation.x != rot.x || 
		zRotation.y != rot.y || 
		zRotation.z != rot.z || 
		zRotation.w != rot.w
		)
	{
		zRotation = rot;

		this->zMesh->ResetRotation();
		this->zMesh->SetQuaternion(rot);
	}
}

void Actor::SetMesh(iMesh* mesh) 
{ 
	if ( zMesh != mesh )
	{
		this->zMesh = mesh;

		if ( zMesh ) 
		{
			zMesh->SetPosition(zPosition);
			zMesh->ResetRotation();
			zMesh->SetQuaternion(zRotation);
		}
	}
}

void Actor::SetID(const int clientID) 
{ 
	this->zID = clientID; 
}

void Actor::AddSubMesh(const std::string& model, iMesh* mesh)
{
	this->zSubMeshes[model] = mesh;
}

iMesh* Actor::GetSubMesh(const std::string& model)
{
	auto it = this->zSubMeshes.find(model);
	if (it != this->zSubMeshes.end())
	{
		return it->second;
	}

	return NULL;
}

void Actor::RemoveSubMesh(const std::string& model) 
{
	this->zSubMeshes.erase(model);
}