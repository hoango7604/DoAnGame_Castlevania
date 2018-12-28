#include "SuperDracula.h"
#include "define.h"

void SuperDracula::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SUPERDRACULA_BBOX_WIDTH;
	bottom = y + SUPERDRACULA_BBOX_HEIGHT;
}

void SuperDracula::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);
	
	//if (!isHitted)
	//{
		if (GetTickCount() - timer > 1500 && check_wait == false)
		{
			state = SUPERDRACULA_START_FLY;
			check_wait = true;
		}
		if (GetTickCount() - timer > 2000 && check_start_fly == false)
		{
			state = SUPERDRACULA_FLY;
			check_start_fly = true;
		}
		if (GetTickCount() - timer > 2500 && check_onsky == false)
		{
			state = SUPERDRACULA_ON_SKY;
			check_onsky = true;
		}
		if (GetTickCount() - timer > 2650)
		{
			state = SUPERDRACULA_START_FLY;
		}
		if (GetTickCount() - timer > 3150)
		{
			/*state = SUPERDRACULA_WAIT;
			timer = GetTickCount();
			check_onsky = false;
			check_start_fly = false;
			check_wait = false;
			jump += 1;*/
			if (jump % 3 !=0)
			{
				state = SUPERDRACULA_WAIT;
				timer = GetTickCount();
				check_onsky = false;
				check_start_fly = false;
				check_wait = false;
				jump += 1;
			}
			else
			{
				if (check_hit_time == false)
				{
					timer_hit = GetTickCount();
					check_hit_time = true;
				}
				if (GetTickCount() - timer_hit < 1500)
					state = SUPERDRACULA_WAIT;
				if (GetTickCount() - timer_hit > 1500)
					state = SUPERDRACULA_STATE_HIT;
				if (GetTickCount() - timer_hit > 2000)
					{
						jump += 1;
						check_hit_time = false;
					}
			}
		}
	//}
	if(isHitted)
		{
			isBleeding = false;
			isHitted = false;
		}
	/*else
	{
		if (check_hit == false)
		{
			hitTime = GetTickCount();
			check_hit = true;
		}
		if (GetTickCount() - hitTime > SUPERDRACULA_HIT_TIME)
		{
			isHitted = false;
			isBleeding = false;
			check_hit = false;
		}
	}*/
	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;


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

		//if (nx != 0) vx = 0;

		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground *>(e->obj))
			{

				if (ny != 0) vy = 0;			

			}
		}
	}
	
}

void SuperDracula::Render()
{
	switch (state)
	{
	case SUPERDRACULA_WAIT:		
		if (simon->x < x)
		{
			animations[0]->Render(x, y);
		}
		else
		{
			animations[4]->Render(x, y);
		}
		vx = 0;
		break;
	case SUPERDRACULA_START_FLY:
		if(simon->x <x)
		animations[1]->Render(x, y);
		else
		animations[5]->Render(x, y);
		break;
	case SUPERDRACULA_FLY:
		if (simon->x < x)
		{
			animations[2]->Render(x, y);
			vx = -0.08;
		}
		else
		{
			animations[6]->Render(x, y);
			vx = 0.08;
		}
		if (jump % 2 == 0)
			vy = -0.5;
		else
			vy = -0.3;
		
		break;
	case SUPERDRACULA_ON_SKY:
		if (simon->x < x)
			animations[3]->Render(x, y);
		else
			animations[7]->Render(x, y);
		vy = 0;
		break;
	case SUPERDRACULA_STATE_HIT:
		if (simon->x < x)
			animations[3]->Render(x, y -32);
		else
			animations[7]->Render(x, y-32);
	}

}

void SuperDracula::SetState(int state, int i)
{
	Enemy::SetState(state);

	switch (state)
	{
	
	}
}


