#include "GUIPicture.h"
GUIPicture::GUIPicture() : Element()
{

}
GUIPicture::GUIPicture(float x, float y, float z, string textureName, float width, float height) : Element(x ,y, z, textureName, width, height)
{

}
GUIPicture::~GUIPicture()
{

}

bool GUIPicture::AddToRenderer(GraphicsEngine* ge)
{
	Element::AddToRenderer(ge);

	return true;
}
bool GUIPicture::RemoveFromRenderer(GraphicsEngine* ge)
{
	Element::RemoveFromRenderer(ge);

	return true;
}

void GUIPicture::Resize( int oldWindowWidth, int oldWindowHeight, int windowWidth, int windowHeight )
{
	Element::Resize( (float)oldWindowWidth, (float)oldWindowHeight, (float)windowWidth, (float)windowHeight);
}
