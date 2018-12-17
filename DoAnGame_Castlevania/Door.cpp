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
	if (((CGame::GetInstance()->x_cam < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) && (CGame::GetInstance()->x_cam > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2 - 10))
		|| ((CGame::GetInstance()->x_cam < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2) && (CGame::GetInstance()->x_cam > MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2 - 10)))
	{		
		this->SetState(DOOR_STATE_ACTIVE);
	}
	
}
void Door::Render()
{
	
	if (state == DOOR_STATE_ACTIVE)
	{	
		if (check1 == false)
		{
			time = GetTickCount();
			check1 = true;
		}
		if (GetTickCount() - time > 500 )
		{
			animations[0]->Render(x, y);			
			if(GetTickCount() - time > 1500)
			state = DOOR_STATE_DEACTIVE;
		}
		
	}
	if(state == DOOR_STATE_DEACTIVE)
	{		
		animations[1]->Render(x, y);	
		if (GetTickCount() - time > 3500)
			state = DOOR_STATE_0;
	}
	if (state == DOOR_STATE_0)
	{
		animations[2]->Render(x, y);
		if (GetTickCount() - time > 4500)
			state = DOOR_STATE_01;
	}
	if (state == DOOR_STATE_01)
	{
		return;
	}

	
		
}
void Door::SetState(int state)
{
	CGameObject::SetState(state);
}
