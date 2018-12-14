#include "Door.h"





void Door::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + DOOR_BBOX_WIDTH;
	b = y + DOOR_BBOX_HEIGHT;
}

void Door::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	if (CGame::GetInstance()->x_cam == MAX_WIDTH_LV2 - SCREEN_WIDTH / 2 || CGame::GetInstance()->x_cam == MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
		this->SetState(DOOR_STATE_ACTIVE);
	
}
void Door::Render()
{
	
	if (state == DOOR_STATE_DEACTIVE)
	{
		time = GetTickCount();
		animations[1]->Render(x, y);
		RenderBoundingBox();
		if (GetTickCount() - time > 1000)
			this->SetState(DOOR_STATE_0);
	}
	else if (state == DOOR_STATE_ACTIVE)
	{
		time = GetTickCount();
		animations[0]->Render(x, y);		
		RenderBoundingBox();
		if (GetTickCount() - time > 1000)
			this->SetState(DOOR_STATE_DEACTIVE);
	}
	else if (state == DOOR_STATE_0)
	{

	}
	

}
void Door::SetState(int state)
{
	CGameObject::SetState(state);
}
