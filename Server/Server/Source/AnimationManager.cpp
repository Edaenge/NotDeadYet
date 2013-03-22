#include "AnimationManager.h"
#include "Weapon.h"

AnimationManager::AnimationManager()
{
	this->zAnimationFileReader[0] = AnimationFileReader("media/models/token_anims.cfg");
	this->zAnimationFileReader[2] = AnimationFileReader("media/models/deer_anims.cfg");
	this->zAnimationFileReader[3] = AnimationFileReader("media/models/bear_anims.cfg");

	this->zModelToReaderMap["media/models/token_anims.fbx"] = zAnimationFileReader[0];
	this->zModelToReaderMap["media/models/deer_anims.fbx"] = zAnimationFileReader[2];
	this->zModelToReaderMap["media/models/bear_anims.fbx"] = zAnimationFileReader[3];

	srand((unsigned int)time(0));
}

AnimationManager::~AnimationManager()
{

}

const std::string& AnimationManager::GetPlayerAnimation(const std::string model, const std::string& animation)
{
	auto it = this->zModelToReaderMap.find(model);
	if (it != this->zModelToReaderMap.end())
	{
		return it->second.GetAnimation(animation);
	}

	static const std::string empty = "";

	return empty;
}

const float AnimationManager::GetAnimationTime(const std::string model, const std::string& animation)
{
	auto it = this->zModelToReaderMap.find(model);

	if (it != this->zModelToReaderMap.end())
		return it->second.GetAnimationTime(animation);
	
	return 0.0f;
}

AnimationQueue AnimationManager::CreatePlayerAnimationQueue( BioActor* bActor)
{
	AnimationQueue queue;

	AnimationFileReader reader;

	auto it = this->zModelToReaderMap.find(bActor->GetModel());
	if (it != this->zModelToReaderMap.end())
		reader = it->second;
	else
		return queue;

	float fRand = 0.0f;
	const unsigned int currState = bActor->GetState();
	unsigned int prevState = bActor->GetPreviousState();
	std::string animation = "";
	Item* item = NULL;
	KeyStates keystates = bActor->GetPlayer()->GetKeys();

	switch (currState)
	{
	case STATE_IDLE:
		if (prevState != STATE_EQUIP_WEAPON && prevState != STATE_UNEQUIP_WEAPON && prevState != STATE_ATTACK && 
			prevState != STATE_DRAW_BOW && prevState != STATE_USE && prevState != STATE_CRAFTING && prevState != STATE_FILLING_BOTTLE)
		{
			item = bActor->GetInventory()->GetPrimaryEquip();
			if (item == NULL)
			{
				fRand = (float)rand() / (float)RAND_MAX;

				if (fRand > 0.0f && fRand <= 0.16f)//High Chance 16%
					animation = reader.GetAnimation(IDLE_O1);
				else if (fRand > 0.16f && fRand <= 0.26f)//Medium Chance 10
					animation = reader.GetAnimation(IDLE_O2);
				else if (fRand > 0.27f && fRand <= 0.31f)//Low Chance 4%
					animation = reader.GetAnimation(IDLE_O3);
				else if (fRand > 0.31f && fRand <= 0.58f)//Very High Chance 27%
					animation = reader.GetAnimation(IDLE_O4);
				else if (fRand > 0.58f && fRand <= 0.85f)//Very High Chance 27%
					animation = reader.GetAnimation(IDLE_O5);
				else if (fRand > 0.85f && fRand <= 1.0f)//High Chance 16%
					animation = reader.GetAnimation(IDLE_O6);

				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

				bActor->SetPreviousAnimation(animation);
			}
			else
			{
				if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
				{
					if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
					{
						animation = reader.GetAnimation(MACHETE_IDLE_01);

						queue.zAnimations.push_back(animation);
						queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

						bActor->SetPreviousAnimation(animation);
					}
					else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
					{
						animation = reader.GetAnimation(MACHETE_IDLE_01);

						queue.zAnimations.push_back(animation);
						queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

						bActor->SetPreviousAnimation(animation);
					}
				}
				else if(item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
				{
					animation = reader.GetAnimation(BOW_EQUIPPED_IDLE);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));
					
					bActor->SetPreviousAnimation(animation);
				}
			}
		}
		break;
	case STATE_RUNNING:
		animation = reader.GetAnimation(SPRINT);

		if(bActor->GetPreviousAnimation() != animation)
		{
			queue.zAnimations.push_back(animation);
			queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

			bActor->SetPreviousAnimation(animation);
		}
		
		break;
	case STATE_CROUCHING:
		if(keystates.GetKeyState(KEY_FORWARD))
			animation = reader.GetAnimation(WALK_FORWARD);
		else if (keystates.GetKeyState(KEY_BACKWARD))
			animation = reader.GetAnimation(WALK_BACKWARD);
		else if(keystates.GetKeyState(KEY_LEFT))
			animation = reader.GetAnimation(WALK_LEFT);
		else if (keystates.GetKeyState(KEY_RIGHT))
			animation = reader.GetAnimation(WALK_RIGHT);
		else
			break;

		if(bActor->GetPreviousAnimation() != animation)
		{
			queue.zAnimations.push_back(animation);
			queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

			bActor->SetPreviousAnimation(animation);
		}
		break;
	case STATE_WALKING:
		if(keystates.GetKeyState(KEY_FORWARD))
			animation = reader.GetAnimation(JOG_FORWARD);
		else if (keystates.GetKeyState(KEY_BACKWARD))
			animation = reader.GetAnimation(JOG_BACKWARD);
		else if(keystates.GetKeyState(KEY_LEFT))
			animation = reader.GetAnimation(JOG_LEFT);
		else if (keystates.GetKeyState(KEY_RIGHT))
			animation = reader.GetAnimation(JOG_RIGHT);
		else
			break;

		if(bActor->GetPreviousAnimation() != animation)
		{
			queue.zAnimations.push_back(animation);
			queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

			bActor->SetPreviousAnimation(animation);
		}
		break;
	case STATE_EQUIP_WEAPON:
		item = bActor->GetInventory()->GetLastUnequipped();
		if (item)
		{
			if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
			{
				if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
				{
					animation = reader.GetAnimation(MACHETE_UNEQUIP_01);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
				else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
				{
					animation = reader.GetAnimation(PKNIFE_EQUIP);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
			}
			else if(item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
			{
				animation = reader.GetAnimation(BOW_EQUIP);

				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

				queue.zAnimations.push_back(bActor->GetPreviousAnimation());
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
			}
		}
		break;
	case STATE_UNEQUIP_WEAPON:
		item = bActor->GetInventory()->GetPrimaryEquip();
		if (item)
		{
			if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
			{
				if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
				{
					animation = reader.GetAnimation(MACHETE_UNEQUIP_01);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
				else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
				{
					animation = reader.GetAnimation(PKNIFE_UNEQUIP);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
			}
			else if(item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
			{
				animation = reader.GetAnimation(BOW_UNEQUIP);

				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

				queue.zAnimations.push_back(bActor->GetPreviousAnimation());
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
			}
		}
		break;
	case STATE_ATTACK:
		item = bActor->GetInventory()->GetPrimaryEquip();
		if (item)
		{
			if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
			{
				if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
				{
					animation = reader.GetAnimation(MACHETE_ATTACK_01);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
				else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
				{
					animation = reader.GetAnimation(PKNIFE_ATTACK);

					queue.zAnimations.push_back(animation);
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

					queue.zAnimations.push_back(bActor->GetPreviousAnimation());
					queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
				}
			}
			else if(item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
			{
				animation = reader.GetAnimation(BOW_SHOOT_DRAW_ARROW);

				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

				animation = it->second.GetAnimation(BOW_UNDRAW);
				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(it->second.GetAnimationTime(animation));

				animation = it->second.GetAnimation(BOW_EQUIPPED_IDLE);

				queue.zAnimations.push_back(animation);
				queue.zAnimationTimes.push_back(it->second.GetAnimationTime(animation));

				bActor->SetPreviousAnimation(animation);
			}
		}
		break;
	case STATE_DRAW_BOW:
		animation = reader.GetAnimation(BOW_DRAW);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		animation = it->second.GetAnimation(BOW_DRAW_IDLE);
		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(it->second.GetAnimationTime(animation));

		bActor->SetPreviousAnimation(animation);
		break;
	case STATE_CRAFTING:
		animation = reader.GetAnimation(GENERIC_CRAFT);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		queue.zAnimations.push_back(bActor->GetPreviousAnimation());
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
		break;
	case STATE_USE:
		animation = reader.GetAnimation(GENERIC_USE);
		
		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		queue.zAnimations.push_back(bActor->GetPreviousAnimation());
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
		break;
	case STATE_FILLING_BOTTLE:
		animation = reader.GetAnimation(FILL_CANTEEN);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		queue.zAnimations.push_back(bActor->GetPreviousAnimation());
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
		break;
	case STATE_DEAD:
		animation = reader.GetAnimation(DEATH);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		animation = reader.GetAnimation(DEAD);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		bActor->SetPreviousAnimation(animation);
		break;
	default:
		break;
	}

	return queue;
}

AnimationQueue AnimationManager::CreateAnimalAnimationQueue(BioActor* bActor)
{
	AnimationQueue queue;

	AnimationFileReader reader;

	auto it = this->zModelToReaderMap.find(bActor->GetModel());
	if (it != this->zModelToReaderMap.end())
		reader = it->second;
	else
		return queue;

	float fRand = 0.0f;
	const unsigned int currState = bActor->GetState();
	std::string animation = "";

	switch (currState)
	{
	case STATE_IDLE:
		fRand = (float)rand() / (float)RAND_MAX;

		if (fRand > 0.0f && fRand <= 0.5f)//Very High Chance 50%
			animation = reader.GetAnimation(IDLE_O1);
		else if (fRand > 0.5f && fRand <= 0.66f)//Medium Chance 10%
			animation = reader.GetAnimation(IDLE_O2);
		else if (fRand > 0.66f && fRand <= 0.83f)//Medium Chance 4%
			animation = reader.GetAnimation(IDLE_O3);
		else if (fRand > 0.83f && fRand <= 1.0f)//Medium Chance 27%
			animation = reader.GetAnimation(IDLE_O4);
		else 
			break;

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		bActor->SetPreviousAnimation(animation);
		break;
	case STATE_WALKING:
		animation = reader.GetAnimation(WALK_FORWARD);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));
		
		bActor->SetPreviousAnimation(animation);
		break;
	case STATE_RUNNING:
		animation = reader.GetAnimation(SPRINT);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));
		bActor->SetPreviousAnimation(animation);
		break;
	case STATE_ATTACK:
		animation = reader.GetAnimation(ANIMAL_ATTACK_01);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		queue.zAnimations.push_back(bActor->GetPreviousAnimation());
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(bActor->GetPreviousAnimation()));
		
		break;
	case STATE_DEAD:
		animation = reader.GetAnimation(DEATH);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		animation = reader.GetAnimation(DEAD);

		queue.zAnimations.push_back(animation);
		queue.zAnimationTimes.push_back(reader.GetAnimationTime(animation));

		bActor->SetPreviousAnimation(animation);
		break;
	default:
		break;
	}

	return queue;
}