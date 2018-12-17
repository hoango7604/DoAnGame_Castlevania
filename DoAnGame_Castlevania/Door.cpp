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
		|| ((CGame::GetInstance()->x_cam < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)&&(CGame::GetInstance()->x_cam > MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2 -10)))
		this->SetState(DOOR_STATE_ACTIVE);
	
}
void Door::Render()
{
	
	if (state == DOOR_STATE_ACTIVE)
	{		
		
		animations[0]->Render(x, y);
		if (check == 0)
		{
			time = GetTickCount();
			check++;
		}
		if( GetTickCount() - time > 500)
			animations[1]->Render(x, y);				
	}
	if (check != 0)
	{
		if (GetTickCount() - time > 3000)
			state = DOOR_STATE_0;
		if (state == DOOR_STATE_0)
		{
			animations[2]->Render(x, y);
			time = GetTickCount();
			if (GetTickCount() - time > 1000)
				state == DOOR_STATE_01;
		}
		if (state == DOOR_STATE_01)
		{

		}
	}
	
	

	

}
void Door::SetState(int state)
{
	CGameObject::SetState(state);
}
