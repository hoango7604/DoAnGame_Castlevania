#include "BigBat.h"
#include "define.h"

void BigBat::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + BOSSBAT_BBOX_WIDTH;
	bottom = y + BOSSBAT_BBOX_HEIGHT;
}

void BigBat::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	
	// Đổi trạng thái
	if (simon->x < x + SCREEN_WIDTH / 3 && simon->x > x - SCREEN_WIDTH / 3)
	{
		if (!isActivate)
		{
			isWait = true;
			isActivate = true;
		}
		else
		{
			// Đổi từ đang chờ sang bay
			if (isWait)
			{
				// Nếu sắp tấn công
				if (GetTickCount() - waitTime > BOSSBAT_WAITING_TIME)
				{
					destination_x = simon->x;
					destination_y = simon->y;

					// 6 hướng di chuyển
					// Qua phải
					if (destination_x > x)
					{
						vx = BIGBAT_SPEED;

						// Xuống dưới
						if (destination_y > y + 10)
						{
							vy = BIGBAT_SPEED;
						}
						// Lên trên
						else if (destination_y < y - 20)
						{
							vy = -BIGBAT_SPEED;
						}
						// Đi ngang
						else
						{
							vy = 0;
						}
					}
					// Qua trái
					else
					{
						vx = -BIGBAT_SPEED;

						// Xuống dưới
						if (destination_y > y + 10)
						{
							vy = BIGBAT_SPEED;
						}
						// Lên trên
						else if (destination_y < y - 20)
						{
							vy = -BIGBAT_SPEED;
						}
						// Đi ngang
						else
						{
							vy = 0;
						}
					}

					isWait = false;
					attackTime = GetTickCount();
				}
			}
			else
			{
				if (GetTickCount() - attackTime > BOSSBAT_ATTACK_TIME * 3)
				{
					vx = 0;
					vy = 0;

					isWait = true;
					waitTime = GetTickCount();
				}
			}

			Enemy::Update(dt, coObjects);

			this->x += dx;
			this->y += dy;
		}
	}
}

void BigBat::Render()
{
	animations[0]->Render(x, y);

	RenderBoundingBox();
}