/*
Made by Edänge Simon Datum(19/12/12 created) 
for project desperation* at Blekinge tekniska högskola.
*/

#pragma once


class DynamicObjectActor : public Actor
{
public:
	DynamicObjectActor(){};
	virtual ~DynamicObjectActor(){};

	virtual void Update() = 0;

	int GetWeight() const {return this->zWeight;}
	int GetType() const {return this->zType;}
	std::string GetDescription() const {return this->zDescription;}
	std::string GetIconPath() const {return this->zIconPath;}

	void SetIconPath(const std::string& path) {this->zIconPath = path;}
	void SetDescription(const std::string& description) {this->zDescription = description;}
	void SetWeight(const int weight) {this->zWeight = weight;}
	void SetType(const int TYPE) {this->zType = TYPE;}

protected:
	int zWeight;
	int zType;
	std::string zDescription;
	std::string zIconPath;
	
private:
};