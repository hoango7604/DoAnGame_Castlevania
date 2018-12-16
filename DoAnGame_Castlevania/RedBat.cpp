#include "RedBat.h"

void RedBat::SetState(int state)
{
	Enemy::SetState(state);

	switch (state)
	{
	case BAT_STATE_FLYING:
		vx = -BAT_FLYING_SPEED;
		break;
	}
}

void RedBat::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}

void RedBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	// Lực kéo lên
	if (vy >= BAT_STABLE_GRAVITY)
		gravity = -BAT_FLYING_GRAVITY;

	// Lực kéo xuống
	else if (vy <= -BAT_STABLE_GRAVITY)
		gravity = BAT_FLYING_GRAVITY;

	vy += gravity * dt;

	x += dx;
	y += dy;
}

void RedBat::Render()
{
	int ani = -1;

	if (nx < 0)
		ani = BAT_ANI_FLYING_LEFT;
	else if (nx > 0)
		ani = BAT_ANI_FLYING_LEFT;

	animations[ani]->Render(x, y);

	RenderBoundingBox();
}
