#include "Dracula.h"

void Dracula::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case DRACULA_STATE_IDLE:
		restCast = GetTickCount();
		break;
	case DRACULA_STATE_ATTACK:
		restCast = GetTickCount();
		break;
	case DRACULA_STATE_INVISIBLE:
		isBleeding = false;
		isHit = false;
		lastRestTime = GetTickCount();
		break;
	case DRACULA_STATE_EXPOSE:
		break;
	}
}

void Dracula::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (isActivate && state == DRACULA_STATE_ATTACK && !isHit)
	{
		left = x;
		top = y;
		right = x + DRACULA_BBOX_WIDTH;
		bottom = y + DRACULA_BBOX_HEIGHT;
	}
	else
	{
		left = x;
		top = y;
		right = x;
		bottom = y;
	}
}

void Dracula::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActivate)
	{
		if (simon->x < 300)
		{
			Enemy::Update(dt, coObjects);

			x += dx;
			y += dy;

			if (y < DRACULA_Y)
			{
				isActivate = true;

				min_x = game->x_cam + SCREEN_WIDTH / 5;
				max_x = game->x_cam + 4 * SCREEN_WIDTH / 5;
				nx = -1;

				srand(time(NULL));

				SetState(DRACULA_STATE_IDLE);
			}
		}
	}
	else
	{
		if (GetTickCount() - restCast > DRACULA_REST_TIME)
		{
			if (state == DRACULA_STATE_IDLE)
			{
				SetState(DRACULA_STATE_ATTACK);
			}
			else if (state == DRACULA_STATE_ATTACK)
				SetState(DRACULA_STATE_INVISIBLE);
		}

		if (GetTickCount() - lastRestTime > DRACULA_INVISIBLE_TIME)
		{
			if (state == DRACULA_STATE_INVISIBLE)
			{
				x = simon->x + rand() % 100 - 50;
				y = DRACULA_Y;

				if (x > simon->x)
				{
					nx = -1;
				}
				else
				{
					nx = 1;
				}

				SetState(DRACULA_STATE_IDLE);
			}
		}
	}
}

void Dracula::Render()
{
	int ani = -1;

	if (!isActivate)
	{
		ani = DRACULA_ANI_ACTIVATE;
	}
	else
	{
		switch (state)
		{
		case DRACULA_STATE_IDLE:
			if (nx > 0)
			{
				ani = DRACULA_ANI_IDLE_RIGHT;
			}
			else
			{
				ani = DRACULA_ANI_IDLE_LEFT;
			}
			break;
		case DRACULA_STATE_ATTACK:
			if (nx > 0)
			{
				if (isHit)
				{
					ani = DRACULA_ANI_ATTACK_RIGHT_NOHEAD;
				}
				else
				{
					ani = DRACULA_ANI_ATTACK_RIGHT;
				}
			}
			else
			{
				if (isHit)
				{
					ani = DRACULA_ANI_ATTACK_LEFT_NOHEAD;
				}
				else
				{
					ani = DRACULA_ANI_ATTACK_LEFT;
				}
			}
			break;
		}
	}

	if (ani != -1)
	{
		if (nx > 0)
			animations[ani]->Render(x - 36, y);
		else
			animations[ani]->Render(x + 16, y);
	}

	//RenderBoundingBox();
}
