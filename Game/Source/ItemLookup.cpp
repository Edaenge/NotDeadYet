#include "ItemLookup.h"
#include "MaloW.h"
#include "Safe.h"
#include <algorithm>

namespace 
{
	static ItemLookup* itemLookup = NULL;
}

void FreeItemLookup()
{
	SAFE_DELETE(itemLookup);
}


ItemLookup::~ItemLookup()
{
	for( unsigned int i = 0; i < this->zItems.size(); i++ )
	{
		if (this->zItems.at(i))
		{
			delete this->zItems.at(i);
		}
	}
	this->zItems.clear();
}

void InitItemLookup()
{
	itemLookup = new ItemLookup();
}

ItemLookup* GetItemLookup()
{
	return itemLookup;
}

static const std::string FILENAME				=	"DefaultItemData.cfg";
static const std::string COMMENT				=	"//";
static const std::string OBJECT_RANGED_WEAPON	=	"[Object.RangedWeapon]";
static const std::string OBJECT_MELEE_WEAPON	=	"[Object.MeleeWeapon]";
static const std::string OBJECT_FOOD			=	"[Object.Food]";
static const std::string OBJECT_CONTAINER		=	"[Object.Container]";
static const std::string OBJECT_GEAR			=	"[Object.Gear]";
static const std::string OBJECT_PROJECTILE		=	"[Object.Projectile]";
static const std::string OBJECT_MATERIAL		=	"[Object.Material]";
static const std::string OBJECT_BANDAGE			=	"[Object.Bandage]";

static const std::string END					=	"#END";
static const std::string TYPE					=	"TYPE";
static const std::string SUBTYPE				=	"SUBTYPE";
static const std::string WEIGHT					=	"WEIGHT";
static const std::string PATH					=	"PATH";
static const std::string DESCRIPTION			=	"DESCRIPTION";
static const std::string MODEL					=	"MODEL";
static const std::string NAME					=	"NAME";
static const std::string IS_RANGED				=	"IS_RANGED";
static const std::string STACKS					=	"STACKS";
static const std::string STACKING				=   "STACKING";

static const std::string HUNGER					=	"HUNGER";
static const std::string DAMAGE					=	"DAMAGE";
static const std::string RANGE					=	"RANGE";
static const std::string MAX_USE				=	"MAX_USE";
static const std::string CURRENT_USE			=	"CURRENT_USE";
static const std::string SPEED					=	"SPEED";
static const std::string CRAFTING_TYPE			=	"CRAFTING_TYPE";
static const std::string STACKS_REQUIREMENT		=	"STACKS_REQUIREMENT";
ItemLookup::ItemLookup()
{
	this->ReadFromFile();
}

const Projectile* ItemLookup::GetProjectile( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_PROJECTILE && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<Projectile*>((*it));
		}
	}
	return NULL;
}

const RangedWeapon* ItemLookup::GetRangedWeapon( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_RANGED && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<RangedWeapon*>((*it));
		}
	}
	return NULL;
}

const MeleeWeapon* ItemLookup::GetMeleeWeapon( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_WEAPON_MELEE && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<MeleeWeapon*>((*it));
		}
	}
	return NULL;
}

const Food* ItemLookup::GetFood( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_FOOD && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<Food*>((*it));
		}
	}
	return NULL;
}

const Material* ItemLookup::GetMaterial( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_MATERIAL && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<Material*>((*it));
		}
	}
	return NULL;
}

const Container* ItemLookup::GetContainer( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_CONTAINER && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<Container*>((*it));
		}
	}
	return NULL;
}

const Bandage* ItemLookup::GetBandage( const unsigned int SubType )
{
	for (auto it = this->zItems.begin(); it != this->zItems.end(); it++)
	{
		if ((*it)->GetItemType() == ITEM_TYPE_BANDAGE && (*it)->GetItemSubType() == SubType)
		{
			return dynamic_cast<Bandage*>((*it));
		}
	}
	return NULL;
}

bool ItemLookup::ReadFromFile()
{
	std::ifstream read;
	read.open(FILENAME);
	
	if(!read.is_open())
		return false;
	
	while(!read.eof())
	{
		std::string line;
		char key[52] = "";
		char command[126] = "";
	
		std::getline(read, line);
	
		if(line.empty())
			continue;
	
		sscanf_s(line.c_str(), "%s", &key, sizeof(key));
	
		if(strcmp(key, COMMENT.c_str()) == 0)
			continue;
	
		if(strcmp(key, OBJECT_RANGED_WEAPON.c_str()) == 0)
		{
			RangedWeapon* rWpn = new RangedWeapon();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);
	
				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));
	
				if(command != END)
					InterpCommand(command, key, rWpn);
			}
			this->zItems.push_back(rWpn);
		}
		else if(strcmp(key, OBJECT_MELEE_WEAPON.c_str()) == 0)
		{
			MeleeWeapon* mWpn = new MeleeWeapon();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(command != END)
					InterpCommand(command, key, mWpn);
			}
			this->zItems.push_back(mWpn);
		}
		else if(strcmp(key, OBJECT_FOOD.c_str()) == 0)
		{
			Food* fd = new Food();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);
	
				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));
	
				if(command != END)
					InterpCommand(command, key, fd);
			}
	
			this->zItems.push_back(fd);
		}
		else if(strcmp(key, OBJECT_CONTAINER.c_str()) == 0)
		{
			Container* ct = new Container();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);
	
				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));
	
				if(command != END)
					InterpCommand(command, key, ct);
			}
	
			this->zItems.push_back(ct);
		}
		else if(strcmp(key, OBJECT_PROJECTILE.c_str()) == 0)
		{
			Projectile* pt = new Projectile();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(command != END)
					InterpCommand(command, key, pt);
			}

			this->zItems.push_back(pt);
		}
		else if(strcmp(key, OBJECT_MATERIAL.c_str()) == 0)
		{
			Material* ma = new Material();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);
	
				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));
	
				if(command != END)
					InterpCommand(command, key, ma);
			}
	
			this->zItems.push_back(ma);
		}
		else if(strcmp(key, OBJECT_BANDAGE.c_str()) == 0)
		{
			Bandage* ba = new Bandage();
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);
	
				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));
	
				if(command != END)
					InterpCommand(command, key, ba);
			}
	
			this->zItems.push_back(ba);
		}
	}
	read.close();
	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, MeleeWeapon*& wp)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);
	

	if(command == TYPE)
	{
		wp->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		wp->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		wp->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		wp->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		wp->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		wp->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		wp->SetItemName(key);
	}
	else if(command == STACKS)
	{
		wp->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			wp->SetStacking(true);
		else
			wp->SetStacking(false);
	}
	else if(command == RANGE)
	{
		wp->SetRange(MaloW::convertStringToFloat(key));
	}
	else if(command == DAMAGE)
	{
		wp->SetDamage(MaloW::convertStringToFloat(key));
	}
	

	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, RangedWeapon*& wp)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);


	if(command == TYPE)
	{
		wp->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		wp->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		wp->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		wp->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		wp->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		wp->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		wp->SetItemName(key);
	}
	else if(command == STACKS)
	{
		wp->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			wp->SetStacking(true);
		else
			wp->SetStacking(false);
	}
	else if(command == RANGE)
	{
		wp->SetRange(MaloW::convertStringToFloat(key));
	}
	else if(command == DAMAGE)
	{
		wp->SetDamage(MaloW::convertStringToFloat(key));
	}


	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, Food*& fd)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == TYPE)
	{
		fd->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		fd->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		fd->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		fd->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		fd->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		fd->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		fd->SetItemName(key);
	}
	else if(command == STACKS)
	{
		fd->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			fd->SetStacking(true);
		else
			fd->SetStacking(false);
	}
	else if(command == HUNGER)
	{
		fd->SetHunger(MaloW::convertStringToFloat(key));
	}

	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, Container*& ct)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == TYPE)
	{
		ct->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		ct->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		ct->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		ct->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ct->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		ct->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ct->SetItemName(key);
	}
	else if(command == STACKS)
	{
		ct->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			ct->SetStacking(true);
		else
			ct->SetStacking(false);
	}
	else if(command == MAX_USE)
	{
		ct->SetMaxUses((int)MaloW::convertStringToFloat(key));
	}
	else if(command == CURRENT_USE)
	{
		ct->SetRemainingUses((int)MaloW::convertStringToFloat(key));
	}

	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, Material*& ma)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == TYPE)
	{
		ma->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		ma->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		ma->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		ma->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ma->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		ma->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ma->SetItemName(key);
	}
	else if(command == STACKS)
	{
		ma->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			ma->SetStacking(true);
		else
			ma->SetStacking(false);
	}
	else if(command == CRAFTING_TYPE)
	{
		ma->SetCraftingType((int)MaloW::convertStringToFloat(key));
	}
	else if(command == STACKS_REQUIREMENT)
	{
		ma->SetRequiredStackToCraft((int)MaloW::convertStringToFloat(key));
	}

	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, Projectile*& pa)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == TYPE)
	{
		pa->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		pa->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		pa->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		pa->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		pa->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		pa->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		pa->SetItemName(key);
	}
	else if(command == STACKS)
	{
		pa->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			pa->SetStacking(true);
		else
			pa->SetStacking(false);
	}
	else if(command == SPEED)
	{
		pa->SetVelocity(MaloW::convertStringToFloat(key));
	}
	else if(command == DAMAGE)
	{
		pa->SetDamage(MaloW::convertStringToFloat(key));
	}

	return true;
}

bool ItemLookup::InterpCommand(std::string command, std::string key, Bandage*& ba)
{
	if(key.empty())
		return false;

	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	if(command == TYPE)
	{
		ba->SetItemType(MaloW::convertStringToInt(key));
	}
	else if(command == SUBTYPE)
	{
		ba->SetItemSubType(MaloW::convertStringToInt(key));
	}
	else if(command == WEIGHT)
	{
		ba->SetItemWeight(MaloW::convertStringToInt(key));
	}
	else if(command == PATH)
	{
		ba->SetIconPath(key);
	}
	else if(command == DESCRIPTION)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ba->SetItemDescription(key);
	}
	else if(command == MODEL)
	{
		ba->SetModel(key);
	}
	else if(command == NAME)
	{
		std::replace(key.begin(), key.end(), '_', ' ');
		ba->SetItemName(key);
	}
	else if(command == STACKS)
	{
		ba->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if(command == STACKING)
	{
		if(MaloW::convertStringToInt(key))
			ba->SetStacking(true);
		else
			ba->SetStacking(false);
	}

	return true;
}

