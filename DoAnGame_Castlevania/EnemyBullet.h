#pragma once
#include "Enemy.h"
#include "Ground.h"

class EnemyBullet : public Enemy
{
public:

	EnemyBullet(int nx)
	{
		if (nx > 0)
			vx = ENEMY_BULLET_SPEED;
		else
			vx = -ENEMY_BULLET_SPEED;
	}

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
};

