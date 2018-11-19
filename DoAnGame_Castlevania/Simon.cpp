#include <algorithm>
#include "debug.h"

#include "Simon.h"
#include "Game.h"
#include "BigFire.h"
#include "Candle.h"
#include "Zombie.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Simon::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (!dynamic_cast<Candle *>(coObjects->at(i)) && !dynamic_cast<BigFire *>(coObjects->at(i))) {
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

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

	// when simon attack
	if (state == SIMON_STATE_HIT)
	{
		whip->SetPosition(x, y);
		whip->Update(dt, coObjects);
	}

	// Handle Simon go over screen camera
	float leftCorner = CGame::GetInstance()->x_cam;
	float rightCorner = leftCorner + SCREEN_WIDTH - 3*SIMON_BIG_BBOX_WIDTH;
	// Left corner
	if (x < leftCorner) 
	{
		x = 0;
	}
	// Right corner
	else if (x > rightCorner)
	{
		//x = rightCorner;
	}

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		/*
		 * Handle collision here
		 */
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Zombie *>(e->obj))
			{
				Zombie *zombie = dynamic_cast<Zombie *>(e->obj);
				if (zombie->GetState() != ZOMBIE_STATE_DIE) {
					StartUntouchable();
					//SetState(SIMON_STATE_DIE);
				}
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani, aniWhip = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
		
	// If stay idle
	if (vx == 0)
	{
		if (nx > 0)
		{
			if (state != SIMON_STATE_HIT)
				ani = SIMON_ANI_BIG_IDLE_RIGHT;
			else 
			{
				ani = SIMON_ANI_HIT_RIGHT;
				aniWhip = WHIP_RIGHT;
			}
		}

		else
		{
			if (state != SIMON_STATE_HIT)
				ani = SIMON_ANI_BIG_IDLE_LEFT;
			else 
			{
				ani = SIMON_ANI_HIT_LEFT;
				aniWhip = WHIP_LEFT;
			}
		}
	}
	else if (vx > 0)
		ani = SIMON_ANI_BIG_WALKING_RIGHT;
	else ani = SIMON_ANI_BIG_WALKING_LEFT;
	
	int alpha = 255;
	if (untouchable) alpha = 128;
	
	animations[ani]->Render(x, y , alpha);

	if (aniWhip != -1)
	{
		whip->animations[aniWhip]->Render(x , y , alpha);
	}
	RenderBoundingBox();
	whip->RenderBoundingBox();
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
	
}


