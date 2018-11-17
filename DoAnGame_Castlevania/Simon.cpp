#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480



void Simon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	
	else if (x < 0)
		x =  0;
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}
	whip->SetPosition(x, y);
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani,aniWhip=-1;
	 if (y != 289 && state == SIMON_STATE_JUMP)
	{

	if (nx > 0)
		ani = SIMON_ANI_JUMP_RIGHT;
	else
		ani = SIMON_ANI_JUMP_LEFT;
	}
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;	
	else
	{
		if (vx == 0)
		{
			if (nx > 0 && state != SIMON_STATE_HIT && state != SIMON_STATE_SIT && state != SIMON_STATE_JUMP)
				ani = SIMON_ANI_BIG_IDLE_RIGHT;
			else if (nx < 0 && state != SIMON_STATE_HIT && state != SIMON_STATE_SIT && state != SIMON_STATE_JUMP)
				ani = SIMON_ANI_BIG_IDLE_LEFT;
			else if (nx > 0 && state == SIMON_STATE_HIT)
			{
				ani = SIMON_ANI_HIT_RIGHT;
				aniWhip = WHIP_RIGHT;
			}
			else if (nx < 0 && state == SIMON_STATE_HIT)
			{
				ani = SIMON_ANI_HIT_LEFT;
				aniWhip = WHIP_LEFT;
			}
			else if (nx > 0 && state == SIMON_STATE_SIT)
				ani = SIMON_ANI_SIT_RIGHT;
			else if (nx < 0 && state == SIMON_STATE_SIT)
				ani = SIMON_ANI_SIT_LEFT;			

		}
		else if (vx > 0)
			ani = SIMON_ANI_BIG_WALKING_RIGHT;
		
		else ani = SIMON_ANI_BIG_WALKING_LEFT;
	}
		

	int alpha = 255;
	if (untouchable) alpha = 128;
	
	if(ani == SIMON_ANI_SIT_RIGHT || ani== SIMON_ANI_SIT_LEFT)
	animations[ani]->Render(x , y+17, alpha);
	else
	animations[ani]->Render(x, y , alpha);

	if (aniWhip != -1)
	{		
		whip->animations[aniWhip]->Render(x , y , alpha);

	}
	RenderBoundingBox();
}

void Simon::SetState(int state)
{
			
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HIT || SIMON_STATE_SIT:
		vx = 0;		
		break;
	
	}
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (level == SIMON_LEVEL_BIG)
	{
		right = x + SIMON_BIG_BBOX_WIDTH;
		bottom = y + SIMON_BIG_BBOX_HEIGHT;
	}
	else
	{
		right = x + SIMON_SMALL_BBOX_WIDTH;
		bottom = y + SIMON_SMALL_BBOX_HEIGHT;
	}
}


