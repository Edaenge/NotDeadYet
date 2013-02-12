#pragma once

#include "Projectile.h"
#include "RangedWeapon.h"
#include "Food.h"
#include "Container.h"
#include "Material.h"
#include "MeleeWeapon.h"
#include "FileReader.h"

#include <string>
#include <vector>

/****************************************************************/
/*		File That Has all Item Data for easy use				*/
/****************************************************************/

class ItemLookup : public FileReader
{
public:
	ItemLookup();
	const Projectile* GetProjectile(const unsigned int SubType);
	const RangedWeapon* GetRangedWeapon(const unsigned int SubType);
	const MeleeWeapon* GetMeleeWeapon(const unsigned int SubType);
	const Food* GetFood(const unsigned int SubType);
	const Material* GetMaterial(const unsigned int SubType);
	const Container* GetContainer(const unsigned int SubType);
	
private:
	bool ReadFromFile();
	bool InterpCommand(std::string command, std::string key, MeleeWeapon*& wp);
	bool InterpCommand(std::string command, std::string key, RangedWeapon*& wp);
	bool InterpCommand(std::string command, std::string key, Food*& fd);
	bool InterpCommand(std::string command, std::string key, Container*& ct);
	bool InterpCommand(std::string command, std::string key, Material*& ma);
	bool InterpCommand(std::string command, std::string key, Projectile*& pa);
private:
	std::vector<Item*> zItems;
};

ItemLookup* GetItemLookup();

void InitItemLookup();
void FreeItemLookup();