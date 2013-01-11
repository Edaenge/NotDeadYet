#include "ObjectManager.h"
#include "../../../../../Source/MaloWLib/MaloW.h"
#include <fstream>
#include <string>
#include <algorithm>

static const std::string FILENAME			=	"WorldObjects.cfg";
static const std::string COMMENT			=	"//";
static const std::string OBJECT_WEAPON		=	"[Object.Weapon]";
static const std::string OBJECT_FOOD		=	"[Object.Food]";
static const std::string OBJECT_CONTAINER	=	"[Object.Container]";
static const std::string OBJECT_GEAR		=	"[Object.Gear]";
static const std::string OBJECT_PROJECTILE	=	"[Object.Projectile]";

static const std::string END				=	"#END";
static const std::string TYPE				=	"TYPE";
static const std::string WEIGHT				=	"WEIGHT";
static const std::string PATH				=	"PATH";
static const std::string DESCRIPTION		=	"DESCRIPTION";
static const std::string MODEL				=	"MODEL";
static const std::string NAME				=	"NAME";
static const std::string IS_RANGED			=	"IS_RANGED";
static const std::string SCALE				=	"SCALE";

static const std::string HUNGER				=	"HUNGER";
static const std::string DAMAGE				=	"DAMAGE";
static const std::string RANGE				=	"RANGE";
static const std::string MAX_USE			=	"MAX_USE";
static const std::string CURRENT_USE		=	"CURRENT_USE";
static const std::string VELOCITY			=	"VELOCITY";

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
	auto x = zWeapons.begin();
	auto i = zFood.begin();

	for(; x < zWeapons.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(; i < zFood.end(); i++)
	{
		SAFE_DELETE((*i));
	}
}

bool ObjectManager::ReadObjects()
{
	std::ifstream read;
	read.open(FILENAME);

	if(!read.is_open())
		return false;

	while(!read.eof())
	{
		char line[256] = "";
		char key[52] = "";
		char command[126] = "";

		read.getline(line,sizeof(line));

		if(strcmp(line, "") == 0)
			continue;

		sscanf_s(line, "%s", &key, sizeof(key));

		if(strcmp(key, COMMENT.c_str()) == 0)
			continue;

		if(strcmp(key, OBJECT_WEAPON.c_str()) == 0)
		{
			WeaponObject* wp = new WeaponObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, wp);
			}
			this->zWeapons.push_back(wp);
		}
		else if(strcmp(key, OBJECT_FOOD.c_str()) == 0)
		{
			FoodObject* fd = new FoodObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, fd);
			}

			this->zFood.push_back(fd);
		}
		else if(strcmp(key, OBJECT_CONTAINER.c_str()) == 0)
		{
			ContainerObject* ct = new ContainerObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, ct);
			}

			this->zContainers.push_back(ct);
		}
		else if(strcmp(key, OBJECT_PROJECTILE.c_str()) == 0)
		{
			DynamicProjectileObject* ct = new DynamicProjectileObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, ct);
			}

			this->zProjectiles.push_back(ct);
		}
	}

	return true;
}

void ObjectManager::TrimAndSet( char* ret )
{
	if(ret == NULL)
		return;

	std::string str(ret);
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());

	size_t found;
	found = str.find("=");

	if(found == std::string::npos)
		return;

	int index = (int)found;

	std::string subA;
	std::string subB;
	subA = str.substr(0,index);
	subB = str.substr(index+1,str.size()-1);

	subA += " = ";
	str = subA + subB;

	strcpy(ret, str.c_str());
}

bool ObjectManager::Replace(char* key)
{
	if (strcmp(key, "") == 0)
		return false;

	std::string str = string(key);

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (str.at(i) == '_')
		{
			str.at(i) = ' ';
		}
	}
	strcpy(key, str.c_str());
	return true;
}

bool ObjectManager::InterpCommand( char* command, char* key, WeaponObject* wp )
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		wp->SetType(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, DAMAGE.c_str()) == 0)
	{
		wp->SetDamage(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		wp->SetWeight(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, PATH.c_str()) == 0)
	{
		wp->SetIconPath(key);
	}
	else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	{
		wp->SetDescription(key);
	}
	else if(strcmp(command, RANGE.c_str()) == 0)
	{
		wp->SetRange(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		wp->SetActorModel(key);
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		wp->SetActorObjectName(key);
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char x[52];
		char y[52];
		char z[52];
		unsigned int index = 0;

		while(key[index] != '\0')
		{
			if(key[index] == ',')
				key[index] = ' ';

			index++;
		}

		sscanf_s(key, "%s %s %s", &x, sizeof(x), &y, sizeof(y), &z, sizeof(z));
		Vector3 vec(MaloW::convertStringToFloat(x), MaloW::convertStringToFloat(y), MaloW::convertStringToFloat(z));

		wp->SetScale(vec);
	}

	return true;
}

bool ObjectManager::InterpCommand( char* command, char* key, FoodObject* fd )
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		fd->SetType(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, HUNGER.c_str()) == 0)
	{
		fd->SetHunger(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		fd->SetWeight(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, PATH.c_str()) == 0)
	{
		fd->SetIconPath(key);
	}
	else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	{
		fd->SetDescription(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		fd->SetActorModel(key);
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		fd->SetActorObjectName(key);
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char x[52];
		char y[52];
		char z[52];

		unsigned int index = 0;
		while(command[index] != '\0')
		{
			if(command[index] == ',')
				command[index] = ' ';

			index++;
		}

		sscanf_s(key, "%s %s %s", &x, sizeof(x), &y, sizeof(y), &z, sizeof(z));
		Vector3 vec(MaloW::convertStringToFloat(x), MaloW::convertStringToFloat(y), MaloW::convertStringToFloat(z));

		fd->SetScale(vec);
	}

	return true;
}

bool ObjectManager::InterpCommand( char* command, char* key, ContainerObject* ct )
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		ct->SetType(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, MAX_USE.c_str()) == 0)
	{
		ct->SetMaxUses(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, CURRENT_USE.c_str()) == 0)
	{
		ct->SetCurrentUses(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		ct->SetWeight(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, PATH.c_str()) == 0)
	{
		ct->SetIconPath(key);
	}
	else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	{
		ct->SetDescription(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		ct->SetActorModel(key);
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		ct->SetActorObjectName(key);
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char x[52];
		char y[52];
		char z[52];

		unsigned int index = 0;
		while(command[index] != '\0')
		{
			if(command[index] == ',')
				command[index] = ' ';

			index++;
		}

		sscanf_s(key, "%s %s %s", &x, sizeof(x), &y, sizeof(y), &z, sizeof(z));
		Vector3 vec(MaloW::convertStringToFloat(x), MaloW::convertStringToFloat(y), MaloW::convertStringToFloat(z));

		ct->SetScale(vec);
	}

	return true;
}

bool ObjectManager::InterpCommand( char* command, char* key, DynamicProjectileObject* pt )
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		pt->SetType(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, VELOCITY.c_str()) == 0)
	{
		pt->SetVelocity(MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		pt->SetWeight(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, PATH.c_str()) == 0)
	{
		pt->SetIconPath(key);
	}
	else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	{
		pt->SetDescription(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		pt->SetActorModel(key);
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		pt->SetActorObjectName(key);
	}

	return true;
}

const WeaponObject* ObjectManager::GetWeaponObject( const int type )
{
	return SearchType(this->zWeapons, type);
}

const FoodObject* ObjectManager::GetFoodObject( const int type )
{
	return SearchType(this->zFood, type);
}

const ContainerObject* ObjectManager::GetContainerObject( const int type )
{
	return SearchType(this->zContainers, type);
}

const DynamicProjectileObject* ObjectManager::GetProjectileObject( const int type )
{
	return SearchType(this->zProjectiles, type);
}

const WeaponObject* ObjectManager::SearchType( std::vector<WeaponObject*>& weapons, const int type ) const
{
	for(auto it = weapons.begin(); it < weapons.end(); it++)
	{
		if((*it)->GetType() == type)
			return (*it);
	}

	return NULL;
}

const FoodObject* ObjectManager::SearchType( std::vector<FoodObject*>& food, const int type ) const
{
	for(auto it = food.begin(); it < food.end(); it++)
	{
		if((*it)->GetType() == type)
			return (*it);
	}

	return NULL;
}

const ContainerObject* ObjectManager::SearchType( std::vector<ContainerObject*>& containers, const int type ) const
{
	for(auto it = containers.begin(); it < containers.end(); it++)
	{
		if((*it)->GetType() == type)
			return (*it);
	}

	return NULL;
}

const DynamicProjectileObject* ObjectManager::SearchType(std::vector<DynamicProjectileObject*>& projectiles, const int type) const
{
	for(auto it = projectiles.begin(); it < projectiles.end(); it++)
	{
		if((*it)->GetType() == type)
			return (*it);
	}

	return NULL;
}