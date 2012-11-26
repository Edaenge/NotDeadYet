#include "FPSCamera.h"

FPSCamera::FPSCamera(HWND g_hWnd, GraphicsEngineParams params) : Camera(g_hWnd, params)
{
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);
	this->DistanceFromTarget = 6.0f;

	CursorControl cc;
	cc.SetVisibility(false);

	POINT np;
	np.x = this->params.windowWidth/2;
	np.y = this->params.windowHeight/2;
	if(ClientToScreen(this->g_hWnd, &np))
	{
		SetCursorPos(np.x, np.y);
	}
}

FPSCamera::~FPSCamera()
{

}

void FPSCamera::updateSpecific(float delta)
{
	POINT p;
	if(GetCursorPos(&p))
	{
		if(ScreenToClient(this->g_hWnd, &p))
		{
			float diffX = (this->params.windowWidth/2) - (float)p.x;
			float diffY = (this->params.windowHeight/2) - (float)p.y;
			
			this->angleX += diffX * (this->sensitivity * 0.001f);
			this->angleY += diffY * (this->sensitivity * 0.001f);
			
			if(angleY > PI/2)
				angleY = PI/2;
			if(angleY < -PI/2)
				angleY = -PI/2;

			if(angleX > 2*PI)
				angleX -= 2*PI;
			if(angleX < 0)
				angleX += 2 * PI;
			
			this->forward.x = cos(angleX);
			this->forward.y = sin(angleY);
			this->forward.z = sin(angleX);
			
			this->forward = this->NormalizeVector(this->forward);
			
			//calc new up
			//crossproduct between y-axis & forward vector
			MaloW::Vector3 yAxis = MaloW::Vector3(0, 1, 0);
			MaloW::Vector3 rightVec = yAxis.GetCrossProduct(this->forward);
			rightVec.normalize();
			//crossproduct between forward & right vector 
			//MaloW::Vector3
			MaloW::Vector3 tmpForward = MaloW::Vector3(this->forward);
			MaloW::Vector3 tmpUp = MaloW::Vector3();
			tmpUp = tmpForward.GetCrossProduct(rightVec);
			this->up.x = tmpUp.x;
			this->up.y = tmpUp.y;
			this->up.z = tmpUp.z;
			this->up = this->NormalizeVector(this->up);


			POINT np;
			np.x = this->params.windowWidth/2;
			np.y = this->params.windowHeight/2;
			if(ClientToScreen(this->g_hWnd, &np))
			{
				SetCursorPos(np.x, np.y);
			}
		}
	}
}

void FPSCamera::moveForward(float diff)
{
	this->pos += this->forward * ((float)diff/100) * this->speed;
}

void FPSCamera::moveBackward(float diff)
{
	this->pos -= this->forward * ((float)diff/100) * this->speed;
}

void FPSCamera::moveLeft(float diff)
{
	this->pos.x -= sin(this->angleX) * ((float)diff/100) * this->speed;
	this->pos.z += cos(this->angleX) * ((float)diff/100) * this->speed;
}

void FPSCamera::moveRight(float diff)
{
	this->pos.x += sin(this->angleX) * ((float)diff/100) * this->speed;
	this->pos.z -= cos(this->angleX) * ((float)diff/100) * this->speed;
}