#include "Player.h"
#include "ClientData.h"
#include "NetworkMessageConverter.h"
#include "InventoryEvents.h"
#include "PlayerActor.h"
#include "Behavior.h"

Player::Player( ClientData* client ) :
	zClient(client),
	zBehavior(0)
{
	this->zReady = false;
}

Player::~Player()
{
}

void Player::Kick()
{
	zClient->Kick();
}

void Player::OnEvent( Event* e)
{
	if (InventoryAddItemEvent* IAIE = dynamic_cast<InventoryAddItemEvent*>(e))
	{
		NetworkMessageConverter NMC;
		std::string msg = NMC.Convert(MESSAGE_TYPE_ADD_INVENTORY_ITEM);
		msg += IAIE->item->ToMessageString(&NMC);

		this->zClient->Send(msg);
	}
	else if (InventoryRemoveItemEvent* IRIE = dynamic_cast<InventoryRemoveItemEvent*>(e))
	{
		NetworkMessageConverter NMC;
		std::string msg = NMC.Convert(MESSAGE_TYPE_REMOVE_INVENTORY_ITEM, (float)IRIE->ID);

		this->zClient->Send(msg);
	}
	else if (InventoryEquipItemEvent* IEIE = dynamic_cast<InventoryEquipItemEvent*>(e))
	{
		NetworkMessageConverter NMC;

		std::string msg = NMC.Convert(MESSAGE_TYPE_EQUIP_ITEM, (float)IEIE->id);
		msg += NMC.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)IEIE->slot);
		this->zClient->Send(msg);
	}
	else if (InventoryUnEquipItemEvent* IUIE = dynamic_cast<InventoryUnEquipItemEvent*>(e))
	{
		NetworkMessageConverter NMC;

		std::string msg = NMC.Convert(MESSAGE_TYPE_UNEQUIP_ITEM, (float)IUIE->id);
		msg += NMC.Convert(MESSAGE_TYPE_EQUIPMENT_SLOT, (float)IUIE->slot);
		this->zClient->Send(msg);
	}
	else if (InventoryBindPrimaryWeapon* IBPW = dynamic_cast<InventoryBindPrimaryWeapon*>(e))
	{
		PlayerActor* pActor = dynamic_cast<PlayerActor*>(this->zBehavior->GetActor());

		if (!pActor)
			return;

		if (IBPW->type == ITEM_TYPE_WEAPON_RANGED)
		{
			if (IBPW->subType == ITEM_SUB_TYPE_BOW)
			{
				pActor->SetState(STATE_EQUIP_WEAPON);
			}
		}
		else if (IBPW->type == ITEM_TYPE_WEAPON_MELEE)
		{
			if (IBPW->subType == ITEM_SUB_TYPE_MACHETE)
			{
				pActor->SetState(STATE_EQUIP_WEAPON);
			}
			else if (IBPW->subType == ITEM_SUB_TYPE_POCKET_KNIFE)
			{
				pActor->SetState(STATE_EQUIP_WEAPON);
			}
		}
		else if (IBPW->type == ITEM_TYPE_PROJECTILE && IBPW->subType == ITEM_SUB_TYPE_ROCK)
		{
			
		}
	}
	else if (InventoryUnBindPrimaryWeapon* IUBPW = dynamic_cast<InventoryUnBindPrimaryWeapon*>(e))
	{
		NetworkMessageConverter NMC;
		std::string msg;

		Actor* actor = this->zBehavior->GetActor();

		if (PlayerActor* pActor = dynamic_cast<PlayerActor*>(actor))
		{
			Inventory* inv = pActor->GetInventory();

			Item* item = inv->GetPrimaryEquip();

			if (item)
			{
				if (item->GetItemType() == ITEM_TYPE_WEAPON_RANGED)
				{
					pActor->SetState(STATE_UNEQUIP_WEAPON);
				}
				else if (item->GetItemType() == ITEM_TYPE_WEAPON_MELEE)
				{
					if (item->GetItemSubType() == ITEM_SUB_TYPE_MACHETE)
					{
						pActor->SetState(STATE_UNEQUIP_WEAPON);
					}
					else if (item->GetItemSubType() == ITEM_SUB_TYPE_POCKET_KNIFE)
					{

					}
				}
			}
		}
	}
}