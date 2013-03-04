

#pragma once

#include "Item.h"

static const enum M_BANDAGE_ITEM_SUB_TYPE
{
	ITEM_SUB_TYPE_BANDAGE_POOR,
	ITEM_SUB_TYPE_BANDAGE_GREAT 
};

class Bandage : public Item
{
public:
	Bandage();
	Bandage(const unsigned int ID, const unsigned int Type, const unsigned int SubType);
	Bandage(const unsigned int Type, const unsigned int SubType);
	Bandage(const Bandage& other);
	virtual ~Bandage();
	virtual bool Use();
	virtual std::string ToMessageString(NetworkMessageConverter* NMC);
private:

};