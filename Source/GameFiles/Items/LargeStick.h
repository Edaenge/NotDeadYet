/*
	Made by Jensen Christopher Datum(15/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "Material.h"

class LargeStick : public Material
{
public:
	LargeStick(const unsigned int id, const unsigned int itemType);
	virtual ~LargeStick();

	virtual bool Use();
	virtual bool IsUsable();

private:
};