#include "TextBox.h"

TextBox::TextBox() : Element()
{
	this->mActiveX = 0;
	this->mActiveY = 0;

	this->mActiveWidth = 0;
	this->mActiveHeight = 0;

	this->mText = "";
	this->mName = "";

	this->mFocused = false;
	this->mPressed = false;
	this->mPressedOutside = false;
	this->mTextSize = 0;
}
TextBox::TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize, int maxNrOfChars, int allowedKeys) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;

	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mText = text;
	this->mName = name;
	
	this->mMaxNrOfChars = maxNrOfChars;
	this->mAllowedChars = allowedKeys;
	this->mFrom = 0;
	this->mTo = 9;

	this->mFocused = false;
	this->mPressed = false;
	this->mPressedOutside = false;
	this->mTextSize = textSize;
	this->mPointText = NULL;
}
TextBox::TextBox(float x, float y, float z, string textureName, float width, float height, string text, string name, float textSize, int maxNrOfChars, int allowedKeys, int from, int to) : Element(x, y, z, textureName, width, height)
{
	this->mActiveX = x;
	this->mActiveY = y;

	this->mActiveWidth = width;
	this->mActiveHeight = height;

	this->mText = text;
	this->mName = name;
	
	this->mMaxNrOfChars = maxNrOfChars;
	this->mAllowedChars = allowedKeys;
	this->mFrom = from;
	this->mTo = to;

	this->mFocused = false;
	this->mPressed = false;
	this->mPressedOutside = false;
	this->mTextSize = textSize;
}
TextBox::~TextBox()
{

}
bool TextBox::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);
	this->mPointText = ge->CreateText(this->mText.c_str(), this->GetPosition() + Vector2(3.0f, -1.0f), mTextSize, "Media/Fonts/new");

	return true;
}

 bool TextBox::RemoveFromRenderer(GraphicsEngine* ge)
{
	ge->DeleteText(this->mPointText);
	Element::RemoveFromRenderer(ge);
	this->mPointText = NULL;

	return true;
}

 void TextBox::GoingToBeFocused(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
 {
	if((mouseX < this->mActiveX || mouseX > (this->mActiveX+this->mActiveWidth)) || (mouseY < this->mActiveY || mouseY > (this->mActiveY+this->mActiveHeight)))
	{
		if(mousePressed)
		{
			if(!this->mPressedOutside)
			{
				this->mPressedOutside = true;
			}

		}
		else if(!mousePressed)
		{
			if(this->mPressedOutside)
			{
				this->mPressedOutside = false;
				this->mFocused = false;
			}

		}
				return;
		return;
	}
	else
	{
		if(mousePressed)
		{
			if(!this->mPressed)
			{
				this->mPressed = true;
			}
		}
		else if(!mousePressed)
		{
			if(this->mPressed)
			{
				this->mPressed = false;
				this->mFocused = true;
				return;
			}
		}
	}
	return;
 }

 void TextBox::CheckString(GraphicsEngine* ge)
 {
	if(strlen(this->mPointText->GetText()) < this->mMaxNrOfChars)
	{
		string pushString = "";
		const int NROFLETTERS = 27;
		const int NROFSPECIAL = 12;
		const int NROFMORESPECIALCHARS = 5;
		char keys[NROFLETTERS] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' '};
		char KEYS[NROFLETTERS] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' '};
		char specialChars[NROFSPECIAL] = {',', '.', ';', ':', '*', '?', '!', '<', '>', '-', '_'};
		int moreSpecialChars[NROFMORESPECIALCHARS] = {VK_OEM_PERIOD, VK_OEM_COMMA, VK_OEM_1, VK_OEM_2, VK_OEM_3};
		char charMoreSpecialChars[NROFMORESPECIALCHARS] = {'.' , ',', ':', '/', '?'};
		char numbers[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
		
		/*Check if a char is pressed*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == NORMALCHAR || this->mAllowedChars == NORMALCHAR_NR ||
			this->mAllowedChars == NORMALCHAR_SPECIAL)
		{
			for(int i = 0; i < NROFLETTERS; i++)
			{
				if(ge->GetKeyListener()->IsPressed(KEYS[i]))
				{
					if(!ge->GetKeyListener()->IsPressed(VK_SHIFT))
						pushString = keys[i];
					else
						pushString = KEYS[i];

					this->mPointText->AppendText(pushString.c_str());
					this->mText += pushString;
					ge->GetKeyListener()->KeyUp(KEYS[i]);
				}
			}
		}
		/*Check if a number is pressed*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == NR || this->mAllowedChars == NORMALCHAR_NR ||
				this->mAllowedChars == NR_SPECIAL)
		{
			for(int i = this->mFrom; i < this->mTo+1; i++)
			{
				if(ge->GetKeyListener()->IsPressed(numbers[i]))
				{
					pushString = numbers[i];

					this->mPointText->AppendText(pushString.c_str());
					this->mText += pushString;
					ge->GetKeyListener()->KeyUp(numbers[i]);
				}
			}
		}
		/*Check with special characters*/
		if(this->mAllowedChars == ALL || this->mAllowedChars == SPECIAL || this->mAllowedChars == NORMALCHAR_SPECIAL ||
				this->mAllowedChars == NR_SPECIAL)
		{
			for(int i = 0; i < NROFSPECIAL; i++)
			{
				if(ge->GetKeyListener()->IsPressed(specialChars[i]))
				{
					pushString = specialChars[i];

					this->mPointText->AppendText(pushString.c_str());
					this->mText += pushString;
					ge->GetKeyListener()->KeyUp(specialChars[i]);
				}
			}
			for(int i = 0; i < NROFMORESPECIALCHARS; i++)
			{
				if(ge->GetKeyListener()->IsPressed(moreSpecialChars[i]))
				{
					pushString = charMoreSpecialChars[i];

					this->mPointText->AppendText(pushString.c_str());
					this->mText += pushString;
					ge->GetKeyListener()->KeyUp(moreSpecialChars[i]);
				}
			}
		}

	}
	if(strlen(this->mPointText->GetText()) > 0)
	{
		if(ge->GetKeyListener()->IsPressed(VK_BACK))
		{
			this->mPointText->DeleteFromEnd(1);
			this->mText += mText.substr(0, mText.length()-2);
			ge->GetKeyListener()->KeyUp(VK_BACK);
		}
	}
 }

 void TextBox::Resize( float oldWindowWidth, float oldWindowHeight, float windowWidth, float windowHeight )
 {
	 Element::Resize(oldWindowWidth, oldWindowHeight, windowWidth, windowHeight);

	 float dx = ((float)windowHeight * 4.0f) / 3.0f;
	 float oldDx = ((float)oldWindowHeight * 4.0f) / 3.0f;
	 float oldOffSet = (float)(oldWindowWidth - oldDx) / 2.0f;
	 float offSet = (float)(windowWidth - dx) / 2.0f;

	 this->mActiveX = offSet + ((this->mActiveX - oldOffSet) / oldDx) * dx;
	 this->mActiveY = (this->mActiveY / oldWindowHeight) * windowHeight;

	 this->mActiveWidth = (this->mActiveWidth / oldDx) * dx;
	 this->mActiveHeight = (this->mActiveHeight / oldWindowHeight) * windowHeight;

	 if(this->mPointText)
	 {
		 this->mPointText->SetPosition(this->GetPosition());
	 }
 }
