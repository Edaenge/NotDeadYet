#include "ActorSynchronizer.h"
struct ActorPositionEvent
{
	Actor* actor;
};
struct ActorRotationEvent
{
	Actor* actor;
};
struct ActorScaleEvent
{
	Actor* actor;
};
struct BioActorTakeDamageEvent
{
	Actor* actor;
};

void ActorSynchronizer::OnEvent( Event* e )
{
	if ( ActorPositionEvent* UPE = dynamic_cast<ActorPositionEvent*>(e) )
	{
		zUpdateSet.insert(UPE->actor);
	}
	else if ( ActorRotationEvent* UPE = dynamic_cast<ActorRotationEvent*>(e) )
	{
		zUpdateSet.insert(UPE->actor);
	}
	else if ( ActorScaleEvent* UPE = dynamic_cast<ActorScaleEvent*>(e) )
	{
		zUpdateSet.insert(UPE->actor);
	}
	else if( BioActorTakeDamageEvent* UPE = dynamic_cast<BioActorTakeDamageEvent*>(e))
	{
		zUpdateSet.insert(UPE->actor);
	}
}

bool ActorSynchronizer::SendUpdatesTo( ClientData* cd )
{
	NetworkMessageConverter* nmc = new NetworkMessageConverter();
	std::string msg;
	Vector3 pos;
	Vector4 rot;
	Vector3 scale;
	for(auto it = this->zUpdateSet.begin(); it != this->zUpdateSet.end(); it++)
	{
		pos = (*it)->GetPosition();
		rot = (*it)->GetRotation();
		scale = (*it)->GetScale();

		msg = nmc->Convert(MESSAGE_TYPE_UPDATE_PLAYER, cd->GetClient()->GetClientID());
		msg += nmc->Convert(MESSAGE_TYPE_POSITION, pos.x, pos.y, pos.z);
		msg += nmc->Convert(MESSAGE_TYPE_ROTATION, rot.x, rot.y, rot.z, rot.w);
		msg += nmc->Convert(MESSAGE_TYPE_SCALE, scale.x, scale.y, scale.z);
		cd->SendMessage(msg);
	}
}

bool ActorSynchronizer::ClearAll()
{
	this->zUpdateSet.clear();
}

