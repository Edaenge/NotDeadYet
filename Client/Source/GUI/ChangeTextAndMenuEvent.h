#pragma once
/**
* Author: Rikard Johansson
* Create Date: 04/04/2012
*
* This event will be returned when you are going into a new menu
**/

#include "GUIEvent.h"

class ChangeTextAndMenuEvent : public GUIEvent
{
private:
	int mSet;
	std::string mTextBoxName;
public:
	ChangeTextAndMenuEvent(int set, std::string textBoxName);
	virtual ~ChangeTextAndMenuEvent();

	/*! Changes the value that explane what set that is going to be active*/
	void SetSet(int set){ this->mSet = set; }
	/*! Returns the Set value*/
	int GetSet(){ return this->mSet; }

	void SetTextBoxName(std::string textBoxName){ this->mTextBoxName = textBoxName; }

	std::string GetTextBoxName(){ return this->mTextBoxName; }
};