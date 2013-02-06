#include "ObjectManager.h"
#include "MaloW.h"

static const std::string FILENAME			=	"WorldObjects.cfg";
static const std::string COMMENT			=	"//";
static const std::string OBJECT_WEAPON		=	"[Object.Weapon]";
static const std::string OBJECT_FOOD		=	"[Object.Food]";
static const std::string OBJECT_CONTAINER	=	"[Object.Container]";
static const std::string OBJECT_GEAR		=	"[Object.Gear]";
static const std::string OBJECT_PROJECTILE	=	"[Object.Projectile]";
static const std::string OBJECT_MATERIAL	=	"[Object.Material]";

static const std::string END				=	"#END";
static const std::string TYPE				=	"TYPE";
static const std::string WEIGHT				=	"WEIGHT";
static const std::string PATH				=	"PATH";
static const std::string DESCRIPTION		=	"DESCRIPTION";
static const std::string MODEL				=	"MODEL";
static const std::string NAME				=	"NAME";
static const std::string IS_RANGED			=	"IS_RANGED";
static const std::string SCALE				=	"SCALE";
static const std::string STACKS				=	"STACKS";

static const std::string HUNGER				=	"HUNGER";
static const std::string DAMAGE				=	"DAMAGE";
static const std::string RANGE				=	"RANGE";
static const std::string MAX_USE			=	"MAX_USE";
static const std::string CURRENT_USE		=	"CURRENT_USE";
static const std::string SPEED				=	"SPEED";
static const std::string CRAFTING_TYPE		=	"CRAFTING_TYPE";
static const std::string STACKS_REQUIREMENT =	"STACKS_REQUIREMENT";


ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

	for(auto x = zWeapons.begin(); x < zWeapons.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto i = zFood.begin(); i < zFood.end(); i++)
	{
		SAFE_DELETE((*i));
	}

	for(auto x = zContainers.begin(); x < zContainers.end(); x++)
	{
		SAFE_DELETE((*x));
	}

	for(auto i = zStaticProjectiles.begin(); i < zStaticProjectiles.end(); i++)
	{
		SAFE_DELETE((*i));
	}

	for(auto i = zMaterials.begin(); i < zMaterials.end(); i++)
	{
		SAFE_DELETE((*i));
	}
}

bool ObjectManager::ReadFromFile()
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

		if(strcmp(key, OBJECT_WEAPON.c_str()) == 0)
		{
			WeaponObject* wp = new WeaponObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

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
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

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
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, ct);
			}

			this->zContainers.push_back(ct);
		}
		else if(strcmp(key, OBJECT_PROJECTILE.c_str()) == 0)
		{
			//ProjectileActor* ct = new ProjectileActor(false);
			//while(!read.eof() && strcmp(command, END.c_str()) != 0)
			//{
			//	read.getline(line, sizeof(line));
			//	TrimAndSet(line);

			//	sscanf_s(line, "%s = %s" , &command, sizeof(command), &key, sizeof(key));

			//	if(strcmp(command,END.c_str()) != 0)
			//		InterpCommand(command, key, ct);
			//}

			//this->zStaticProjectiles.push_back(ct);
		}
		else if(strcmp(key, OBJECT_MATERIAL.c_str()) == 0)
		{
			MaterialObject* ma = new MaterialObject(false);
			while(!read.eof() && strcmp(command, END.c_str()) != 0)
			{
				std::getline(read, line);
				TrimAndSet(line);

				sscanf_s(line.c_str(), "%s = %s" , &command, sizeof(command), &key, sizeof(key));

				if(strcmp(command,END.c_str()) != 0)
					InterpCommand(command, key, ma);
			}

			this->zMaterials.push_back(ma);
		}
	}
	read.close();
	return true;
}

bool ObjectManager::InterpCommand(char* command, char* key, WeaponObject* wp)
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
	else if(strcmp(command, STACKS.c_str()) == 0)
	{
		wp->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char a[10];
		char b[10];
		char c[10];

		sscanf_s(key, "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

		Vector3 scale((float)atof(a), (float)atof(b), (float)atof(c));
		wp->SetScale(scale);
	}

	return true;
}

bool ObjectManager::InterpCommand(char* command, char* key, FoodObject* fd)
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
	else if(strcmp(command, STACKS.c_str()) == 0)
	{
		fd->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char a[10];
		char b[10];
		char c[10];

		sscanf_s(key, "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

		Vector3 scale((float)atof(a), (float)atof(b), (float)atof(c));
		fd->SetScale(scale);
	}

	return true;
}

bool ObjectManager::InterpCommand(char* command, char* key, ContainerObject* ct)
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
		ct->SetMaxUses((int)MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, CURRENT_USE.c_str()) == 0)
	{
		ct->SetCurrentUses((int)MaloW::convertStringToFloat(key));
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
	else if(strcmp(command, STACKS.c_str()) == 0)
	{
		ct->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char a[10];
		char b[10];
		char c[10];

		sscanf_s(key, "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

		Vector3 scale((float)atof(a), (float)atof(b), (float)atof(c));
		ct->SetScale(scale);
	}

	return true;
}

bool ObjectManager::InterpCommand(char* command, char* key, MaterialObject* ma)
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	if(strcmp(command, TYPE.c_str()) == 0)
	{
		ma->SetType(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, CRAFTING_TYPE.c_str()) == 0)
	{
		ma->SetCraftingType((int)MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, STACKS_REQUIREMENT.c_str()) == 0)
	{
		ma->SetRequiredStackToCraft((int)MaloW::convertStringToFloat(key));
	}
	else if(strcmp(command, WEIGHT.c_str()) == 0)
	{
		ma->SetWeight(MaloW::convertStringToInt(key));
	}
	else if(strcmp(command, PATH.c_str()) == 0)
	{
		ma->SetIconPath(key);
	}
	else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	{
		ma->SetDescription(key);
	}
	else if(strcmp(command, MODEL.c_str()) == 0)
	{
		ma->SetActorModel(key);
	}
	else if(strcmp(command, NAME.c_str()) == 0)
	{
		ma->SetActorObjectName(key);
	}
	else if(strcmp(command, STACKS.c_str()) == 0)
	{
		ma->SetStackSize(MaloW::convertStringToInt(key));
	}
	else if (strcmp(command, SCALE.c_str()) == 0)
	{
		char a[10];
		char b[10];
		char c[10];

		sscanf_s(key, "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

		Vector3 scale((float)atof(a), (float)atof(b), (float)atof(c));
		ma->SetScale(scale);
	}

	return true;
}

bool ObjectManager::InterpCommand(char* command, char* key, ProjectileActor* pa)
{
	if(strcmp(key, "") == 0)
		return false;

	string CC(command);
	std::transform(CC.begin(), CC.end(), CC.begin(), ::toupper);
	strcpy(command, CC.c_str());

	//if(strcmp(command, TYPE.c_str()) == 0)
	//{
	//	pa->SetType(MaloW::convertStringToInt(key));
	//}
	//else if(strcmp(command, SPEED.c_str()) == 0)
	//{
	//	pa->SetSpeed(MaloW::convertStringToFloat(key));
	//}
	//else if(strcmp(command, DAMAGE.c_str()) == 0)
	//{
	//	pa->SetDamage(MaloW::convertStringToFloat(key));
	//}
	//else if(strcmp(command, WEIGHT.c_str()) == 0)
	//{
	//	pa->SetWeight(MaloW::convertStringToInt(key));
	//}
	//else if(strcmp(command, PATH.c_str()) == 0)
	//{
	//	pa->SetIconPath(key);
	//}
	//else if(strcmp(command, DESCRIPTION.c_str()) == 0)
	//{
	//	pa->SetDescription(key);
	//}
	//else if(strcmp(command, MODEL.c_str()) == 0)
	//{
	//	pa->SetActorModel(key);
	//}
	//else if(strcmp(command, NAME.c_str()) == 0)
	//{
	//	pa->SetActorObjectName(key);
	//}
	//else if(strcmp(command, STACKS.c_str()) == 0)
	//{
	//	pa->SetStackSize(MaloW::convertStringToInt(key));
	//}
	//else if(strcmp(command, SCALE.c_str()) == 0)
	//{
	//	char a[10];
	//	char b[10];
	//	char c[10];

	//	sscanf_s(key, "%[^','], %[^','], %[^','], %s", a, sizeof(a), b, sizeof(b), c, sizeof(c));

	//	Vector3 scale((float)atof(a), (float)atof(b), (float)atof(c));
	//	pa->SetScale(scale);
	//}

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

//const ProjectileActor* ObjectManager::GetProjectileObject( const int type )
//{
//	return SearchType(this->zStaticProjectiles, type);
//}

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

//const ProjectileActor* ObjectManager::SearchType(std::vector<ProjectileActor*>& projectiles, const int type) const
//{
//	for(auto it = projectiles.begin(); it < projectiles.end(); it++)
//	{
//		if((*it)->GetType() == type)
//			return (*it);
//	}
//
//	return NULL;
//}

const MaterialObject* ObjectManager::SearchType(std::vector<MaterialObject*>& materials, const int type) const
{
	for(auto it = materials.begin(); it < materials.end(); it++)
	{
		if((*it)->GetType() == type)
			return (*it);
	}

	return NULL;
}

const MaterialObject* ObjectManager::GetMaterialObject(const int type)
{
	return SearchType(this->zMaterials, type);
}