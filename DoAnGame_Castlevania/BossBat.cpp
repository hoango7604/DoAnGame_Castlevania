#include "BossBat.h"
#include "define.h"
#include <math.h>

void BossBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOSSBAT_BBOX_WIDTH;	
	bottom = y + BOSSBAT_BBOX_HEIGHT;
}

void BossBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	srand(time(NULL));

	// Đổi trạng thái
	if (simon->x > game->x_cam + 3 * SCREEN_WIDTH / 5 && state == BOSSBAT_STATE_WAIT)
	{
		isWait = true;
		isAttack = true;

		destination_x = simon->x;
		destination_y = simon->y;

		waitTime = GetTickCount();
		SetState(BOSSBAT_STATE_ATTACK);
	}

	// Đổi từ đang chờ sang bay
	if (isWait)
	{
		// Nếu sắp tấn công
		if (isAttack && GetTickCount() - waitTime > BOSSBAT_WAITING_TIME)
		{
			if (destination_y < y)
				vy = -BOSSBAT_SPEED_DOWN;
			else
				vy = BOSSBAT_SPEED_DOWN;
			
			// Tính vận tốc bay ngang theo góc hợp bởi vị trí hiện tại của boss dơi và destination
			float angle = atan2(destination_x - x, destination_y - y);

			// Tính vx theo vy
			vx = vy * tan(angle);

			if (vx > 2)
			{
				vx = vx / 2;
				vy = vy / 2;
			}

			isTop = false;
			isWait = false;
		}

		// Nếu sắp quay về vị trí khác
		if (!isAttack && GetTickCount() - attackTime > BOSSBAT_ATTACK_TIME)
		{
			if (destination_y > y)
				vy = BOSSBAT_SPEED_UP;
			else
				vy = -BOSSBAT_SPEED_UP;

			// Tính vận tốc bay ngang theo góc hợp bởi vị trí hiện tại của boss dơi và destination
			float angle = atan2(destination_x - x, destination_y - y);

			// Tính vx theo vy
			vx = vy * tan(angle);

			if (vx > 1 || vx < -1)
			{
				vx = vx / 2;
				vy = vy / 2;
			}

			isBottom = false;
			isWait = false;
		}
	}
	else
	{
		if (isAttack && !isBottom)
		{
			// Đã bay đến vị trí bottom
			if (x - 10 <= destination_x && x + BOSSBAT_BBOX_WIDTH + 10 >= destination_x &&
				y - 10 <= destination_y && y + BOSSBAT_BBOX_HEIGHT + 10 >= destination_y)
			{
				vx = 0;
				vy = 0;

				// Đặt thời gian dừng
				attackTime = GetTickCount();

				// Đổi vị trí destination x y
				destination_x = rand() % (SCREEN_WIDTH - 3 * BOSSBAT_BBOX_WIDTH) + game->x_cam;
				destination_y = rand() % (SCREEN_HEIGHT / 4) + SCREEN_HEIGHT / 4 + game->y_cam;

				if (destination_x < game->x_cam) 
					destination_x = game->x_cam;
				if (destination_x > game->x_cam + SCREEN_WIDTH - 3 * BOSSBAT_BBOX_WIDTH) 
					destination_x = game->x_cam + SCREEN_WIDTH - 3 * BOSSBAT_BBOX_WIDTH;

				if (destination_y < game->y_cam)
					destination_y = game->y_cam;
				if (destination_y > game->y_cam + SCREEN_HEIGHT / 2)
					destination_y = game->y_cam + SCREEN_HEIGHT / 2;

				isAttack = false;
				isBottom = true;
				isWait = true;
			}
			// Đang trên đường bay xuống
			else
			{
				if (isHurt)
				{
					if (GetTickCount() - hurtTime > BOSSBAT_HURTING_TIME)
					{
						isHurt = false;
						isBleeding = false;
					}
				}
				else
				{
					Enemy::Update(dt, coObjects);

					this->x += dx;
					this->y += dy;
				}
			}
		}

		if (!isAttack && !isTop)
		{
			// Đã bay đến vị trí top
			if (x - 10 <= destination_x && x + BOSSBAT_BBOX_WIDTH + 10 >= destination_x &&
				y - 10 <= destination_y && y + BOSSBAT_BBOX_HEIGHT + 10 >= destination_y)
			{
				vx = 0;
				vy = 0;

				// Đặt thời gian dừng
				waitTime = GetTickCount();

				// Đổi vị trí destination x y
				destination_x = simon->x;
				destination_y = simon->y;

				isAttack = true;
				isTop = true;
				isWait = true;
			}
			// Đang trên đường bay lên
			else
			{
				if (isHurt)
				{
					if (GetTickCount() - hurtTime > BOSSBAT_HURTING_TIME)
					{
						isHurt = false;
						isBleeding = false;
					}
				}
				else
				{
					Enemy::Update(dt, coObjects);

					this->x += dx;
					this->y += dy;
				}
			}
		}
	}
}

void BossBat::Render()
{
	int ani;

	switch (state)
	{
	case BOSSBAT_STATE_WAIT:
		ani = BOSSBAT_ANI_WAIT;
		break;
	case BOSSBAT_STATE_ATTACK:
		ani = BOSSBAT_ANI_ATTACK;
		break;
	}

	animations[ani]->Render(x, y);
	
	RenderBoundingBox();
}

void BossBat::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
		case BOSSBAT_STATE_WAIT:
			vx = vy = 0;
			break;
		case BOSSBAT_STATE_ATTACK:
			break;
	}
}