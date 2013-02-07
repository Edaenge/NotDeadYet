#include "ChangeTextAndMenuEvent.h"

ChangeTextAndMenuEvent::ChangeTextAndMenuEvent( int set, std::string textBoxName) : GUIEvent("ChangeTextAndMenuEvent")
{
	this->mSet = set;
	this->mTextBoxName = textBoxName;
}

ChangeTextAndMenuEvent::~ChangeTextAndMenuEvent()
{

}