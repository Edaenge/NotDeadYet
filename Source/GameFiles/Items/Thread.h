/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Material.h"

class Thread : public Material
{
public:
	Thread(const unsigned int id, const unsigned int itemType);
	virtual ~Thread();

	virtual bool Use();
	virtual bool IsUsable();
private:
};