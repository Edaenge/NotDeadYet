#include "Actor.h"

Actor::Actor(const unsigned int& ID)
{
	this->zMesh = 0; 
	this->zID = ID;
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
	}

	this->zMesh = NULL;

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
	this->zModel = model;
}

void Actor::SetPosition(const Vector3& pos)
{
	if (this->zMesh)
	{
		this->zMesh->SetPosition(pos);
	}
}

void Actor::SetScale(const Vector3& scale) 
{
	this->zMesh->SetScale(scale);
}

void Actor::SetRotation(const Vector4& rot)
{
	this->zMesh->ResetRotation();
	this->zMesh->SetQuaternion(rot);
}

void Actor::SetStaticMesh(iMesh* mesh) 
{ 
	this->zMesh = mesh; 
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
		return it->second;

	return NULL;
}

void Actor::RemoveSubMesh(const std::string& model) 
{
	this->zSubMeshes.erase(model);
}