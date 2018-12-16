#include "EnemyBullet.h"

void EnemyBullet::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + ENEMY_BULLET_BBOX_WIDTH;
	bottom = y + ENEMY_BULLET_BBOX_HEIGHT;
}

void EnemyBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	Enemy::Update(dt, coObjects);

	x += dx;
	y += dy;
}

void EnemyBullet::Render()
{
	animations[0]->Render(x, y);

	RenderBoundingBox();
}