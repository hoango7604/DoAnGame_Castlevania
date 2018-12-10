#include "Item.h"

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	CalcPotentialCollisions(coObjects, coEvents);

	vy += SIMON_GRAVITY * dt;

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
	}
}

void Item::Render()
{
	animations[0]->Render(x, y);
}

void Item::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;

	switch (type)
	{
	case ITEM_HEART:
		r = x + HEART_BBOX_WIDTH;
		b = y + HEART_BBOX_HEIGHT;
		break;
	case ITEM_WHIPITEM:
		r = x + WHIPITEM_BBOX_WIDTH;
		b = y + WHIPITEM_BBOX_HEIGHT;
		break;
	case ITEM_MONEY:
		r = x + MONEY_BBOX_WIDTH;
		b = y + MONEY_BBOX_HEIGHT;
		break;
	case ITEM_KNIFE:
		r = x + KNIFE_BBOX_WIDTH;
		b = y + KNIFE_BBOX_HEIGHT;
		break;
	case ITEM_AXE:
		r = x + AXE_BBOX_WIDTH;
		b = y + AXE_BBOX_HEIGHT;
		break;
	case ITEM_HOLYWATER:
		r = x + HOLYFIRE_BBOX_WIDTH;
		b = y + HOLYFIRE_BBOX_HEIGHT;
		break;
	case ITEM_CROSS:
		r = x + CROSS_BBOX_WIDTH;
		b = y + CROSS_BBOX_HEIGHT;
		break;
	case ITEM_ROSARY:
		r = x + ROSARY_BBOX_WIDTH;
		b = y + ROSARY_BBOX_HEIGHT;
		break;
	case ITEM_CLOCK:
		r = x + CLOCK_BBOX_WIDTH;
		b = y + CLOCK_BBOX_HEIGHT;
		break;
	}
}

