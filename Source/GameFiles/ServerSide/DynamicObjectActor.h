/*
Made by Ed�nge Simon Datum(19/12/12 created) 
for project desperation* at Blekinge tekniska h�gskola.
*/

#pragma once


class DynamicObjectActor : public Actor
{
public:
	DynamicObjectActor(){};
	virtual ~DynamicObjectActor(){};

	virtual void Update() = 0;

protected:
	
private:
};