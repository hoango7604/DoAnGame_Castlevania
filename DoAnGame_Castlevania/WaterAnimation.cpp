#include "WaterAnimation.h"



void WaterAnimation::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += dx;
	y += dy;
	vy += 0.0015*dt;
	
}

void WaterAnimation::Render()
{	
	//this->animations[0]->Render(x, y);
	
}
void WaterAnimation::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x ;
	bottom = y ;
}
WaterAnimation::~WaterAnimation()
{

}

