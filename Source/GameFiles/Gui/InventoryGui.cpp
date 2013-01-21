#include "GameFiles/Gui/InventoryGui.h"
#include "../../MaloWLib/Safe.h"

InventoryGui::InventoryGui()
{
	zPressed = false;
}

InventoryGui::InventoryGui(float x, float y, float width, float height, std::string textureName) 
	: GuiElement(x, y, width, height, textureName)
{
	float startOffsetX = (32.0f / 1024.0f) * GetGraphics()->GetEngineParameters()->windowWidth;
	float startOffsetY = (32.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	float widthTemp = (50.0f / 1024.0f) * GetGraphics()->GetEngineParameters()->windowWidth;
	float heightTemp = (50.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	float paddingX = (13.0f / 1024.0f) * GetGraphics()->GetEngineParameters()->windowWidth;
	float paddingY = (13.0f / 1024.0f) * GetGraphics()->GetEngineParameters()->windowHeight;
	float xTemp;
	float yTemp;
	int row = 0;
	int col = 0;
	for(int i = 0; i < SLOTS; i++)
	{
		xTemp = this->zX + startOffsetX + (widthTemp + paddingX) * col;
		col++;
		yTemp = this->zY + startOffsetY + (heightTemp + paddingY) * row;

		if(col >= 7)
		{
			row++;
			col = 0;
		}
		
		zSlotPositions[i] = Vector2(xTemp, yTemp);
	}
}

InventoryGui::~InventoryGui()
{
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		SAFE_DELETE((*it));
	}
}

bool InventoryGui::AddItemToGui(Gui_Item_Data gid, GraphicsEngine* ge)
{
	int size = this->zSlotGui.size();

	if(size >=  SLOTS)
		return false;

	float width = (50.0f / 1024.0f) * GetGraphics()->GetEngineParameters()->windowWidth;
	float height = (50.0f / 768.0f) * GetGraphics()->GetEngineParameters()->windowHeight;

	InventorySlotGui* gui = new InventorySlotGui(zSlotPositions[size].x, zSlotPositions[size].y, width, height, gid.zFilePath, gid.zID);
	this->zSlotGui.push_back(gui);

	return true;
}

bool InventoryGui::RemoveItemFromGui(const int ID)
{
	int size = zSlotGui.size();
	for (auto it = this->zSlotGui.begin(); it < this->zSlotGui.end(); it++)
	{
		if ((*it)->GetID() == ID)
		{
			if(this->zSlotGui.end()-- != it)
			{
				for(auto i = this->zSlotGui.begin(); i < this->zSlotGui.end(); i++)
				{
					if((*i)->GetPosition().x == zSlotPositions[size-1].x && (*i)->GetPosition().y == zSlotPositions[size].y)
					{
						(*i)->SetPosition((*it)->GetPosition());
					}
				}
			}
			(*it)->RemoveFromRenderer(GetGraphics());
			this->zSlotGui.erase(it);

			return true;
		}
	}
	return false;
	
}

bool InventoryGui::AddToRenderer(GraphicsEngine* ge)
{
	GuiElement::AddToRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if(x == this->zSlotGui.end())
			break;

		(*x)->AddToRenderer(ge);
	}
	return true;

}

bool InventoryGui::RemoveFromRenderer(GraphicsEngine* ge)
{
	GuiElement::RemoveFromRenderer(ge);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		(*x)->RemoveFromRenderer(ge);
	}
	return true;
}

int InventoryGui::CheckCollision(float mouseX, float mouseY, bool mousePressed, GraphicsEngine* ge)
{
	Vector2 dimension = this->GetDimension();
	int counter = 0;
	bool bCollision = false;

	if (!((mouseX < this->zX || mouseX > (this->zX + dimension.x)) || (mouseY < this->zY || mouseY > (this->zY + dimension.y))))
	{
		for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end() && !bCollision; x++)
		{
			if ((*x))
			{
				bCollision = (*x)->CheckCollision(mouseX, mouseY, mousePressed, ge);
				if (bCollision)
				{
					if(mousePressed)
						return (*x)->GetID();
				}
			}
			counter++;
		}
	}
	return -1;
}

void InventoryGui::HideGui()
{
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->HideGui();
		}
	}
	GuiElement::HideGui();
}

void InventoryGui::ShowGui()
{
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->ShowGui();
		}
	}
	GuiElement::ShowGui();
}

void InventoryGui::FadeOut(float value)
{
	GuiElement::FadeOut(value);
	for (auto x = this->zSlotGui.begin(); x < this->zSlotGui.end(); x++)
	{
		if ((*x))
		{
			(*x)->FadeOut(value);
		}
	}
}

std::string InventoryGui::GetImageName( unsigned int position )
{
	std::string ret;
	this->zSlotGui[position]->GetTextureName(ret);
	return ret;
}
