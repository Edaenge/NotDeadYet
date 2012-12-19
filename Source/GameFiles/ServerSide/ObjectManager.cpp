#include "ObjectManager.h"
#include "MaloWLib/MaloW.h"
#include <fstream>
#include <string>
#include <algorithm>

static const std::string FILENAME		=	"WorldObjects.cfg";
static const std::string OBJECT_WEAPON	=	"[Object.Weapon]";
static const std::string OBJECT_FOOD	=	"[Object.Food]";

static const std::string END			=	"#end";
static const std::string TYPE			=	"TYPE";
static const std::string WEIGHT			=	"WEIGHT";
static const std::string HUNGER			=	"HUNGER";
static const std::string DAMAGE			=	"DAMAGE";
static const std::string MODEL			=	"MODEL";
static const std::string NAME			=	"NAME";
static const std::string RANGE			=	"RANGE";
static const std::string IS_RANGED		=	"IS_RANGED";


ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

bool ObjectManager::ReadObjects()
{
	std::ifstream read;
	read.open(FILENAME);

	if(!read.is_open())
		return false;

	while(!read.eof())
	{
		char line[256];
		char key[52];
		char command[126];

		read.getline(line,sizeof(line));
		TrimAndSet(line);

		sscanf_s(line, "%s", &key, sizeof(key));

		if(strcmp(key, OBJECT_WEAPON.c_str()) == 0)
		{
			WeaponData wp;
			while(!read.eof() && strcmp(key, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));
				InterpCommand(command, key, wp);
			}
			this->zWeapons.push_back(wp);
		}

		else if(strcmp(key, OBJECT_FOOD.c_str()) == 0)
		{
			FoodData fd;
			while(!read.eof() && strcmp(key, END.c_str()) != 0)
			{
				read.getline(line, sizeof(line));
				TrimAndSet(line);

				sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));
				InterpCommand(command, key, fd);
			}

			this->zFood.push_back(fd);
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

	std::string subA, subB;
	subA = str.substr(0,index);
	subB = str.substr(index+1,str.size()-1);

	subA += " = ";
	str = subA + subB;

	strcpy(ret, str.c_str());

}

bool ObjectManager::InterpCommand( const char* command, char* key, WeaponData& wp )
{
	if(strcmp(key, "") == 0)
		return false;

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		wp.zType = MaloW::convertStringToInt(key);
	}
	else if(strcmp(command, DAMAGE.c_str()) == 0)
	{
		wp.zDamage = MaloW::convertStringToFloat(key);
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		wp.zWeight = MaloW::convertStringToInt(key);
	}
	else if(strcmp(command, RANGE.c_str()) == 0)
	{
		wp.zRange = MaloW::convertStringToFloat(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		wp.zModel = key;
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		wp.zObjName = key;
	}

	return true;
}

bool ObjectManager::InterpCommand( const char* command, char* key, FoodData& fd )
{
	if(strcmp(key, "") == 0)
		return false;

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		fd.zType = MaloW::convertStringToInt(key);
	}
	else if(strcmp(command, HUNGER.c_str()) == 0)
	{
		fd.zHunger = MaloW::convertStringToFloat(key);
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		fd.zWeight = MaloW::convertStringToInt(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		fd.zModel = key;
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		fd.zObjName = key;
	}


	return true;
}

const WeaponData* ObjectManager::GetWeaponObject( const int TYPE )
{
	return SearchType(zWeapons, TYPE);
}

const FoodData* ObjectManager::GetFoodObject( const int TYPE )
{
	return SearchType(zFood, TYPE);
}

const WeaponData* ObjectManager::SearchType( std::vector<WeaponData>& weapons, const int type ) const
{
	for(auto it = weapons.begin(); it < weapons.end(); it++)
	{
		if((*it).zType == type)
			return &(*it);
	}

	return NULL;
}

const FoodData* ObjectManager::SearchType( std::vector<FoodData>& food, const int type ) const
{
	for(auto it = food.begin(); it < food.end(); it++)
	{
		if((*it).zType == type)
			return &(*it);
	}

	return NULL;
}

