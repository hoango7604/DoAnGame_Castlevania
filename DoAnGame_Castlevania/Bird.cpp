#include "Bird.h"
#include <ctime>
#include <math.h>

void Bird::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BIRD_BBOX_WIDTH;
	bottom = y + BIRD_BBOX_HEIGHT;
}

void Bird::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	if (!isRest)
	{
		x += dx;

		if (GetTickCount() - lastRestTime > BIRD_FLYING_TIME)
		{
			isRest = true;
			restCat = GetTickCount();

			if (!isDropHunchBack)
			{
				srand(time(NULL));
				int portion = rand() % 3;

				if (portion > 1)
					isDropHunchBack = true;
			}
		}
	}

	if (isRest && GetTickCount() - restCat > BIRD_REST_TIME)
	{
		isRest = false;
		lastRestTime = GetTickCount();
	}
}

void Bird::Render()
{
	int ani = -1;

	if (nx > 0)
	{
		ani = BIRD_ANI_FLYING_RIGHT;
	}
	if (nx <0)
	{
		ani = BIRD_ANI_FLYING_LEFT;
	}

	animations[ani]->Render(x, y);

	//RenderBoundingBox();
}

