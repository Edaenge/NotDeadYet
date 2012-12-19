/*
	Made by Jensen Christopher Datum(18/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska högskola
*/

#pragma once

#include "GraphicsEngine.h"
#include <string>

class GuiElement
{
public:
	GuiElement();
	GuiElement(float x, float y, float width, float height, std::string textureName);
	virtual ~GuiElement();
	/*! Sets the Position of the GuiElement*/
	void SetPosition(const float x, const float y) {this->zX = x; this->zY = y;}
	/*! Saves the Position to the Parameters*/
	void GetPosition(float& x, float& y) {x = this->zX; y = this->zY;}
	/*! Sets the Width of the GuiElement*/
	void SetWidth(float width) {this->zWidth = width;}
	/*! Saves the Width to the Parameter*/
	void GetWidth(float& width) {width = this->zWidth;}
	/*! Sets the Height of the GuiElement*/
	void SetHeight(float height) {this->zHeight = height;}
	/*! Saves the Height to the Parameter*/
	void GetHeight(float& height) {height = this->zHeight;}
	/*! Returns the GuiElement Dimensions as a Vector2*/
	Vector2 GetDimension() {return Vector2(this->zWidth, this->zHeight);}
	/*! Sets the Texture Variable*/
	void SetTextureName(const std::string& textureName) {this->zTextureName = textureName;}
	/*! Saves the Texture Variable to the Parameter*/
	void GetTextureName(std::string& textureName) {textureName = this->zTextureName;}
	/*! Sets the Texture Variable*/
	void SetImage(iImage* image) {this->zGuiImage = image;}
	/*! Saves the Texture Variable to the Parameter*/
	iImage* GetImage() {return this->zGuiImage;}
	/*! Adds the GuiElement To the Renderer and starts Rendering*/
	virtual bool AddToRenderer(GraphicsEngine* ge);
	/*! Removes the GuiElement From the Renderer and Stops Rendering it*/
	virtual bool RemoveFromRenderer(GraphicsEngine* ge);

	virtual bool IsHidden() {return this->zHidden;}
	void HideGui();
	void ShowGui();
	void FadeOut(float value);

protected:
	float zX;
	float zY;
	float zWidth;
	bool zHidden;
	bool zPressed;
	bool zHovered;
	float zHeight;
	float zOpacity;
	iImage* zGuiImage;
	std::string zTextureName;
};