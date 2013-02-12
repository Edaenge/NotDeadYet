#include "GUISet.h"
#include "Safe.h"

GUISet::GUISet()
{
	this->mNrOfElements = 0;
	this->mMaxNrOfElements = 30;
	this->mElements = new Element*[this->mMaxNrOfElements];
	for(int i = 0; i < this->mMaxNrOfElements; i++)
		this->mElements[i] = NULL;
}
GUISet::~GUISet()
{
	if(this->mElements)
	{
		for(int i = 0; i < this->mNrOfElements; i++)
		{
			if(this->mElements[i])
			{
				SAFE_DELETE(this->mElements[i]);
				//delete this->mElements[i];
			}
		}
		delete [] this->mElements;
		this->mElements = NULL;
	}
}
bool GUISet::AddElement(Element* element)
{
	this->mElements[this->mNrOfElements] = element;
	this->mNrOfElements++;

	return true;
}

bool GUISet::AddSetToRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i]->AddToRenderer(ge);
	
	return true;
}

bool GUISet::RemoveSetFromRenderer(GraphicsEngine* ge)
{
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		this->mElements[i]->RemoveFromRenderer(ge);
	}

	return true;
}

GUIEvent* GUISet::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	GUIEvent* returnEvent = NULL; // = NoEvent();
	GUIEvent* tempReturnEvent;
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) == typeid(TextBox))
		{
			TextBox* temp = (TextBox*)this->mElements[i];
			temp->GoingToBeFocused(mouseX, mouseY, mousePressed, ge);
			if(temp->GetFocused())
				temp->CheckString(ge);
		}
		if(typeid(*this->mElements[i]) == typeid(SimpleButton))
		{
			SimpleButton* temp = (SimpleButton*)this->mElements[i];
			tempReturnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
			if(tempReturnEvent != NULL)
			{
				returnEvent = tempReturnEvent;
				break;
			}
		}
		else if(typeid(*this->mElements[i]) == typeid(DropDownList))
		{
			DropDownList* temp = (DropDownList*)this->mElements[i];
			tempReturnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
			if(tempReturnEvent != NULL)
			{
				if(tempReturnEvent->GetEventMessage() != "NoEvent")
				{
					returnEvent = tempReturnEvent;
					break;
				}
			}
		}
		else if(typeid(*this->mElements[i]) == typeid(CheckBox))
		{
			CheckBox* temp = (CheckBox*)this->mElements[i];
			tempReturnEvent = temp->CheckCollision(mouseX, mouseY, mousePressed, ge);
			if(tempReturnEvent != NULL)
			{
				if(tempReturnEvent->GetEventMessage() != "NoEvent")
				{
					returnEvent = tempReturnEvent;
					break;
				}
			}
		}
	}
	
	return returnEvent;
}
GUIEvent* GUISet::UpdateAndCheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	return this->CheckCollision(mouseX, mouseY, mousePressed, ge);
}
string GUISet::GetTextFromField(string textBox)
{
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) == typeid(TextBox))
		{
			TextBox* temp = (TextBox*)this->mElements[i];
			if(temp->GetName() == textBox)
			{
				return temp->GetText();
			}
		}
	}
	return "";
}

 void GUISet::Resize(float oldWindowWidth, float oldWindowHeight, float windowWidth, float windowHeight)
 {
	 for(int i = 0; i < this->mNrOfElements; i++)
		this->mElements[i]->Resize(oldWindowWidth, oldWindowHeight, windowWidth, windowHeight);
 }
 /*
 GUIEvent* GUISet::GetEventFromDropDown(string dropDownMenu)
{
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) == typeid(DropDownList))
		{
			DropDownList* temp = (DropDownList*)this->mElements[i];
			if(temp->GetName() == dropDownMenu)
			{
				return temp->GetEventFromTop();
			}
		}
	}
	return NULL;
}*/
CheckBox* GUISet::GetCheckBox(string checkBox)
{
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) == typeid(CheckBox))
		{
			CheckBox* temp = (CheckBox*)this->mElements[i];
			if(temp->GetName() == checkBox)
			{
				return temp;
			}
		}
	}
	return NULL;
}/*
TextBox* GUISet::GetTextBox(string textBox)
{
	for(int i = 0; i < this->mNrOfElements; i++)
	{
		if(typeid(*this->mElements[i]) == typeid(TextBox))
		{
			TextBox* temp = (TextBox*)this->mElements[i];
			if(temp->GetName() == textBox)
			{
				return temp;
			}
		}
	}
	return NULL;
}*/