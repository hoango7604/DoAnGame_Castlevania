#include "Weapon.h"

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActivate)
	{
		if (GetTickCount() - firstCast > WEAPON_ACTIVATE_TIME)
		{
			float x = simon->x;
			float y = simon->y;
			if (simon->isSit)
				y += 10;

			nx = simon->nx;

			if (nx > 0)
			{
				this->max_x = x + max_width;
			}
			else if (nx < 0)
			{
				this->max_x = x - max_width;
			}

			SetPosition(x, y + 10);

			isActivate = true;
		}
	}
}

void Weapon::Render()
{
	if (isActivate)
		animations[0]->Render(x, y);
}
