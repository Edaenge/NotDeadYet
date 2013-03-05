#include "SoundHandler.h"
#include "GameEvents.h"
#include "PlayerActor.h"
#include "sounds.h"


SoundHandler::SoundHandler()
{

}

SoundHandler::~SoundHandler()
{
	zPlayers.clear();
}

void SoundHandler::OnEvent( Event* e )
{
	
	if(PlayerExhausted *PE = dynamic_cast<PlayerExhausted *>(e))
	{
		NetworkMessageConverter NMC;
		std::string msg = NMC.Convert(MESSAGE_TYPE_PLAY_SOUND, EVENTID_NOTDEADYET_MAN_BREATHEAFTERRUN);
		msg += NMC.Convert(MESSAGE_TYPE_POSITION, PE->zPos);

		for(auto it = zPlayers.begin(); it != zPlayers.end(); it++)
		{
			(*it)->GetClientData()->Send(msg);
		}
	}
	else if(BioActorTakeDamageEvent *BATDE = dynamic_cast<BioActorTakeDamageEvent *>(e))
	{
		
		if(BATDE->zDamage->fallingDamage == 0)
		{
			if(PlayerActor *PA = dynamic_cast<PlayerActor *>(BATDE->zActor))
			{
				
				NetworkMessageConverter NMC;
				std::string msg = NMC.Convert(MESSAGE_TYPE_PLAY_SOUND, EVENTID_NOTDEADYET_MAN_KNIFESTAB);
				if(PA->GetModel().find("female"))
					msg = NMC.Convert(MESSAGE_TYPE_PLAY_SOUND, EVENTID_NOTDEADYET_WOMAN_KNIFESTAB);
				msg += NMC.Convert(MESSAGE_TYPE_POSITION, PA->GetPosition());
				for(auto it = zPlayers.begin(); it != zPlayers.end(); it++)
				{
					(*it)->GetClientData()->Send(msg);
				}
			}
		}
	}
	else if(PlayerAddEvent *PAE = dynamic_cast<PlayerAddEvent *>(e))
	{
		zPlayers.insert(PAE->player);
	}
	else if(PlayerRemoveEvent *PAE = dynamic_cast<PlayerRemoveEvent *>(e))
	{
		zPlayers.insert(PAE->player);
	}
}

