/*
	Made by Jensen Christopher Datum(02/01/13 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GameFiles/Items/Item.h"

class Container : public Item
{
public:
	Container(const unsigned int id, const unsigned int weight, const std::string& name, 
		const unsigned int itemType, const std::string& itemDescription, const float maxSize, const float currSize);
	virtual ~Container();

	void SetMaxSize(const float size) {this->zMaxSize = size;}
	void SetCurrentSize(const float size) {this->zCurrentSize = size;}

	inline float GetCurrentSize() {return this->zCurrentSize;}
	inline float GetMaxSize() {return this->zMaxSize;}

	virtual void Use();
private:
	float zMaxSize;
	float zCurrentSize;
};