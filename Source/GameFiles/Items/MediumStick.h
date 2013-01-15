/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Material.h"

class MediumStick : public Material
{
public:
	MediumStick(const unsigned int id, const unsigned int itemType);
	virtual ~MediumStick();

	virtual bool Use();

private:
};