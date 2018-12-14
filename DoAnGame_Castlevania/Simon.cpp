#include "Simon.h"
#include "Weapon.h"
#include "Cross.h"

int Simon::score = 0;
int Simon::heartsAmount = 5;
/*
	Calculate potential collisions with the list of colliable objects

	coObjects: the list of colliable objects
	coEvents: list of potential collisions
*/
void Simon::CalcPotentialCollisions(
	vector<LPGAMEOBJECT> *coObjects,
	vector<LPCOLLISIONEVENT> &coEvents)
{
	bool isCollideWithCheckBox = false;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		// Simon se khong va cham voi nhung vat sau:
		if (!dynamic_cast<Candle *>(coObjects->at(i)) &&
			!dynamic_cast<BigFire *>(coObjects->at(i)) &&
			!dynamic_cast<Stair *>(coObjects->at(i)) &&
			!dynamic_cast<CheckStair *>(coObjects->at(i)) &&
			!dynamic_cast<Weapon *>(coObjects->at(i)) &&
			!dynamic_cast<Door *>(coObjects->at(i)))
		{
			LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

			if (e->t > 0 && e->t <= 1.0f)
				coEvents.push_back(e);
			else
				delete e;
		}
		// Xet simon co dang dung ngay tai check stair hay khong
		else if (dynamic_cast<CheckStair *>(coObjects->at(i)))
		{
			CheckStair *checkstair = dynamic_cast<CheckStair *>(coObjects->at(i));
			float cbl, cbr, cbt, cbb;
			checkstair->GetBoundingBox(cbl, cbt, cbr, cbb);

			// Xu ly truong hop simon cham vao CheckStairUp va chua nhan phim len
			if (x < cbr + 2 * SIMON_ONSTAIR_ERR_RANGE && x + SIMON_STAND_BBOX_WIDTH > cbl - 2 * SIMON_ONSTAIR_ERR_RANGE &&
				y + SIMON_STAND_BBOX_HEIGHT < cbb + SIMON_ONSTAIR_ERR_RANGE &&
				y + SIMON_STAND_BBOX_HEIGHT > cbb - SIMON_ONSTAIR_ERR_RANGE)
			{
				isCollideWithCheckBox = true;
				ny = -1.0f;

				if (isOnStair)
				{
					SetState(SIMON_STATE_IDLE);
					isOnStair = false;
				}
				else
				{
					int type = checkstair->GetType();

					// Bat dau xet truong hop nhan phim len
					switch (type)
					{
					case CHECKSTAIR_UP_LEFT:
						isOnCheckStairUp = true;

						if (state == SIMON_STATE_ONCHECKSTAIR)
						{
							nx = 1.0f;

							isMoving = true;

							// Truong hop simon onstair
							if (x > cbr - 2 * SIMON_ONSTAIR_ERR_RANGE &&
								x < cbr + 2 * SIMON_ONSTAIR_ERR_RANGE)
							{
								vx = 0;
								vy = 0;
								SetPosition(
									cbr - 5 * SIMON_ONSTAIR_ERR_RANGE,
									cbb - 5 * SIMON_ONSTAIR_ERR_RANGE - SIMON_STAND_BBOX_HEIGHT);
								SetState(SIMON_STATE_ONSTAIR_IDLE);

								isLeftToRight = false;
								isOnStair = true;
								isOnCheckStairUp = false;
							}
						}
						else
						{
							isOnCheckStairUp = false;
						}
						break;
					case CHECKSTAIR_UP_RIGHT:
						isOnCheckStairUp = true;

						if (state == SIMON_STATE_ONCHECKSTAIR)
						{
							nx = -1.0f;

							isMoving = true;

							// Truong hop simon onstair
							if (x + SIMON_STAND_BBOX_WIDTH > cbl - 2 * SIMON_ONSTAIR_ERR_RANGE &&
								x + SIMON_STAND_BBOX_WIDTH < cbl + 2 * SIMON_ONSTAIR_ERR_RANGE)
							{
								vx = 0;
								vy = 0;
								SetPosition(
									cbl + 5 * SIMON_ONSTAIR_ERR_RANGE - SIMON_STAND_BBOX_WIDTH,
									cbb - 5 * SIMON_ONSTAIR_ERR_RANGE - SIMON_STAND_BBOX_HEIGHT);
								SetState(SIMON_STATE_ONSTAIR_IDLE);

								isLeftToRight = true;
								isOnStair = true;
								isOnCheckStairUp = false;
							}
						}
						else
						{
							isOnCheckStairUp = false;
						}
						break;
					}
				}
			}

			// Xu ly truong hop simon cham vao CheckStairDown va chua nhan phim xuong
			if (x < cbr - 2 * SIMON_ONSTAIR_ERR_RANGE && x + SIMON_STAND_BBOX_WIDTH > cbl &&
				y < cbb + SIMON_ONSTAIR_ERR_RANGE &&
				y > cbb - 5 * SIMON_ONSTAIR_ERR_RANGE)
			{
				isCollideWithCheckBox = true;
				ny = 1.0f;

				if (isOnStair)
				{
					SetState(SIMON_STATE_IDLE);
					isOnStair = false;
				}
				else
				{
					int type = checkstair->GetType();

					// Bat dau xet truong hop nhan phim xuong
					switch (type)
					{
					case CHECKSTAIR_DOWN_LEFT:
						isOnCheckStairDown = true;

						if (state == SIMON_STATE_ONCHECKSTAIR)
						{
							isOnCheckStairDown = true;
							nx = -1.0f;

							isMoving = true;

							// Truong hop simon onstair
							if (x + SIMON_STAND_BBOX_WIDTH > cbl - 2 * SIMON_ONSTAIR_ERR_RANGE &&
								x + SIMON_STAND_BBOX_WIDTH < cbl + 2 * SIMON_ONSTAIR_ERR_RANGE)
							{
								vx = 0;
								vy = 0;
								SetPosition(
									cbl - 4 * SIMON_ONSTAIR_ERR_RANGE - SIMON_STAND_BBOX_WIDTH,
									cbb + 5 * SIMON_ONSTAIR_ERR_RANGE);
								SetState(SIMON_STATE_ONSTAIR_IDLE);

								isLeftToRight = true;
								isOnStair = true;
								isOnCheckStairDown = false;
							}
						}
						else
						{
							isOnCheckStairDown = false;
						}
						break;
					case CHECKSTAIR_DOWN_RIGHT:
						isOnCheckStairDown = true;

						if (state == SIMON_STATE_ONCHECKSTAIR)
						{
							isOnCheckStairDown = true;
							nx = 1.0f;

							isMoving = true;

							// Truong hop simon onstair
							if (x > cbr - 4 * SIMON_ONSTAIR_ERR_RANGE &&
								x < cbr + 2 * SIMON_ONSTAIR_ERR_RANGE)
							{
								vx = 0;
								vy = 0;
								SetPosition(
									cbr + 2 * SIMON_ONSTAIR_ERR_RANGE,
									cbb + 5 * SIMON_ONSTAIR_ERR_RANGE);
								SetState(SIMON_STATE_ONSTAIR_IDLE);

								isLeftToRight = false;
								isOnStair = true;
								isOnCheckStairDown = false;
							}
						}
						else
						{
							isOnCheckStairDown = false;
						}
						break;
					}
				}
			}
		}
	}

	if (!isCollideWithCheckBox && !isOnStair)
	{
		isOnCheckStairDown = false;
		isOnCheckStairUp = false;
		ny = 0;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}

void Simon::Update(int lv,DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = CGame::GetInstance()->GetBackBuffer();
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Has completed attack animation
	if (isAttack == true && GetTickCount() - attackTime >= SIMON_TIMER_ATTACK)
	{
		isAttack = false;
		if (isExitSit)
		{
			isSit = false;
			isExitSit = false;
		}
		// Check collision between whip and game objects here
		if (isUseWhip)
			whip->Update(dt, coObjects);
	}

	// Simple fall down
	if (!isOnStair)
		vy += SIMON_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE || state != SIMON_STATE_HURT)
		CalcPotentialCollisions(coObjects, coEvents);

	// Set idle state
	if (!isOnStair && !isSit && !isMoving && !isJump && !isAttack && !isHurt)
	{
		SetState(SIMON_STATE_IDLE);
	}

	// reset untouchable timer if untouchable time has passed
	if (isUntouchable && GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		isUntouchable = false;
	}

	// when simon attack with whip
	if (isAttack && isUseWhip)
	{
		if (nx > 0)
		{
			// Whip position equal to simon position
			whip->SetPosition(x, y);
		}
		else
		{
			// Whip position equal to simon position + simon width - whip width
			float wl, wr, wt, wb;
			whip->GetBoundingBox(wl, wt, wr, wb);
			whip->SetPosition(x + SIMON_STAND_BBOX_WIDTH - (wr - wl), y);
		}
	}

	// Handle Simon go over screen camera
	
	if (lv == 1 || lv == 22 || lv == 2)
	{
		if (x < 0)
			x = 0;
		if (lv == 2 && x> MAX_WIDTH_LV2 -2*SIMON_STAND_BBOX_WIDTH)
		{
			x = MAX_WIDTH_LV2 - 2*SIMON_STAND_BBOX_WIDTH;
		}
		
		else if (lv == 22 && x > MAX_WIDTH_LV2_2 - SIMON_STAND_BBOX_WIDTH)
		{
			x = MAX_WIDTH_LV2_2 - SIMON_STAND_BBOX_WIDTH;
		}
	}
	else if (lv == 21)
	{
		if (x < MAX_WIDTH_LV2 && CGame::GetInstance()->x_cam == MAX_WIDTH_LV2)
			x = MAX_WIDTH_LV2;
		if (x > MAX_WIDTH_LV2_1 - SIMON_STAND_BBOX_WIDTH && CGame::GetInstance()->x_cam == MAX_WIDTH_LV2_1 - SCREEN_WIDTH)
			x = MAX_WIDTH_LV2_1 - SIMON_STAND_BBOX_WIDTH;
	}
	else if (lv == 99)
	{
		if (x < MAX_WIDTH_LV2_1 && CGame::GetInstance()->x_cam == MAX_WIDTH_LV2_1)
			x = MAX_WIDTH_LV2_1;
		if (x > MAX_WIDTH_BOSS - SIMON_STAND_BBOX_WIDTH)
			x = MAX_WIDTH_BOSS - SIMON_STAND_BBOX_WIDTH;
	}
	
	

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

		/*
		 * Handle collision here
		 */
		bool willHurt = false;
		bool willBlock = false;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if ((dynamic_cast<Zombie *>(e->obj) || dynamic_cast<Panther *>(e->obj)) && !isUntouchable)
			{
				// Reset frame cho hành động attack
				if (isAttack)
				{
					if (this->nx > 0)
					{
						if (isSit)
							animations[SIMON_ANI_SIT_ATTACK_RIGHT]->ResetFrame();
						else
							animations[SIMON_ANI_ATTACK_RIGHT]->ResetFrame();

						if (whip->level == 0)
							whip->animations[WHIP_RIGHT]->ResetFrame();
						else if (whip->level == 1)
							whip->animations[WHIP_RIGHT_1]->ResetFrame();
						else
							whip->animations[WHIP_RIGHT_2]->ResetFrame();
					}
					else if (this->nx < 0)
					{
						if (isSit)
							animations[SIMON_ANI_SIT_ATTACK_LEFT]->ResetFrame();
						else
							animations[SIMON_ANI_ATTACK_LEFT]->ResetFrame();

						if (whip->level == 0)
							whip->animations[WHIP_LEFT]->ResetFrame();
						else if (whip->level == 1)
							whip->animations[WHIP_LEFT_1]->ResetFrame();
						else
						whip->animations[WHIP_LEFT_2]->ResetFrame();
					}
				}

				// Đặt hướng hurt
       			if (e->nx > 0)
					this->nx = -1;
				else if (e->nx < 0)
					this->nx = 1;

				SetState(SIMON_STATE_HURT);
				willHurt = true;
				preHP -= 1;
				StartUntouchable();
			}
			else if (dynamic_cast<Ground *>(e->obj) && !willHurt && !isOnStair)
			{
				// Da cham dat
				// Khong va cham theo phuong ngang
				if (e->ny < 0)
				{
					if (isJump)
						isJump = false;

					if (isHurt)
						isHurt = false;

					willBlock = true;

					// Xét va chạm cứng
					if (ny != 0) vy = 0;
				}
			}
			else if (dynamic_cast<CheckPoint *>(e->obj))
			{
				CheckPoint *checkpoint = dynamic_cast<CheckPoint *>(e->obj);
				int type = checkpoint->GetType();

				switch (type)
				{
				case CHECKPOINT_BONUS:
					isBonus = true;
					checkpoint->SetType(CHECKPOINT_BONUS);
					break;
				case CHECKPOINT_LEVELUP:
					if (!isLevelUp)
					{
						x -= nx * 1.0f;
						isLevelUp = true;
						onCheckPointTime = GetTickCount();
					}
					break;
				}
			}
			else if (dynamic_cast<Item *>(e->obj))
			{
				Item *item = dynamic_cast<Item *>(e->obj);
				item->SetEaten();

				if (e->ny > 0)
					dy = 0;
				else
					willBlock = true;

				int type = item->GetType();
				switch (type)
				{
				case ITEM_HEART:
					IncHeart(5);
					break;
				case ITEM_WHIPITEM:
					whip->UpLevel();
					break;
				case ITEM_KNIFE:
					SetCurrentWeapon(ITEM_KNIFE);
					break;
				case ITEM_AXE:
					SetCurrentWeapon(ITEM_AXE);
					break;
				case ITEM_HOLYWATER:
					SetCurrentWeapon(ITEM_HOLYWATER);
					break;
				case ITEM_CROSS:
					SetCurrentWeapon(ITEM_CROSS);
					break;
				case ITEM_CLOCK:
					SetCurrentWeapon(ITEM_CLOCK);
					break;
				case ITEM_MONEY:
					IncScore(1000);
					break;
				/*case ITEM_ROSARY:
					d3ddv->ColorFill(bb, NULL, (255, 0, 0));
					break;*/
				}
			}
		}

		// block 
		if (!isOnStair && willBlock)
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}
		else
		{
			x += dx;
			y += dy;
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	int ani = -1, aniWhip = -1;

	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else
	{
		if (isHurt)
		{
			if (nx > 0)
				ani = SIMON_ANI_HURT_RIGHT;
			else if (nx < 0)
				ani = SIMON_ANI_HURT_LEFT;
		}
		else if (isAttack)
		{
			if (nx > 0)
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_HITUP_RLADDER;
					else if (ny > 0)
						ani = SIMON_ANI_HITDOWN_RLADDER;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_RIGHT;
				}

				if (isUseWhip)
				{
					if (whip->level == 0)
						aniWhip = WHIP_RIGHT;
					else if (whip->level == 1)
						aniWhip = WHIP_RIGHT_1;
					else
						aniWhip = WHIP_RIGHT_2;
				}
			}
			else
			{
				if (isSit)
				{
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				}
				else if (isOnStair)
				{
					if (ny < 0)
						ani = SIMON_ANI_HITUP_LLADDER;
					else if (ny > 0)
						ani = SIMON_ANI_HITDOWN_LLADDER;
				}
				else
				{
					ani = SIMON_ANI_ATTACK_LEFT;
				}

				if (isUseWhip)
				{
					if (whip->level == 0)
						aniWhip = WHIP_LEFT;
					else if (whip->level == 1)
						aniWhip = WHIP_LEFT_1;
					else
						aniWhip = WHIP_LEFT_2;
				}
			}
		}
		else if (isJump)
		{
			if (nx > 0)
				ani = SIMON_ANI_JUMP_RIGHT;
			else
				ani = SIMON_ANI_JUMP_LEFT;
		}
		// Right direction
		else if (nx > 0)
		{
			switch (state)
			{
			case SIMON_STATE_ONCHECKSTAIR:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_ONSTAIR:
				if (ny < 0)
				{
					ani = SIMON_ANI_WALKUP_RLADDER;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_WALKDOWN_RLADDER;
				}
				break;
			case SIMON_STATE_ONSTAIR_IDLE:
				if (ny < 0)
				{
					ani = SIMON_ANI_IDLE_WALKUP_RLADDER;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_IDLE_WALKDOWN_RLADDER;
				}
				break;
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_RIGHT;
				else
					ani = SIMON_ANI_SIT_RIGHT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_RIGHT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_RIGHT;
				break;
			}
		}
		// Left direction
		else if (nx < 0)
		{
			switch (state)
			{
			case SIMON_STATE_ONCHECKSTAIR:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_ONSTAIR:
				if (ny < 0)
				{
					ani = SIMON_ANI_WALKUP_LLADDER;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_WALKDOWN_LLADDER;
				}
				break;
			case SIMON_STATE_ONSTAIR_IDLE:
				if (ny < 0)
				{
					ani = SIMON_ANI_IDLE_WALKUP_LLADDER;
				}
				else if (ny > 0)
				{
					ani = SIMON_ANI_IDLE_WALKDOWN_LLADDER;
				}
				break;
			case SIMON_STATE_SIT:
				if (isAttack)
					ani = SIMON_ANI_SIT_ATTACK_LEFT;
				else
					ani = SIMON_ANI_SIT_LEFT;
				break;
			case SIMON_STATE_WALK:
				ani = SIMON_ANI_WALKING_LEFT;
				break;
			case SIMON_STATE_IDLE:
				ani = SIMON_ANI_IDLE_LEFT;
				break;
			}
		}
	}

	int alpha = 255;

	if (ani != -1)
	{
		switch (ani)
		{
		case SIMON_ANI_ATTACK_RIGHT:
		case SIMON_ANI_HITDOWN_RLADDER:
		case SIMON_ANI_SIT_ATTACK_RIGHT:
			animations[ani]->Render(x - 16, y, alpha);
			break;
		case SIMON_ANI_HITUP_RLADDER:
			animations[ani]->Render(x - 18, y, alpha);
			break;
		case SIMON_ANI_ATTACK_LEFT:
		case SIMON_ANI_SIT_ATTACK_LEFT:
		case SIMON_ANI_HITUP_LLADDER:
			animations[ani]->Render(x - 12, y, alpha);
			break;
		case SIMON_ANI_HITDOWN_LLADDER:
			animations[ani]->Render(x - 10, y, alpha);
			break;
		default:
			animations[ani]->Render(x, y, alpha);
			break;
		}
	}

	if (aniWhip != -1)
	{
		if (!isSit)
			whip->animations[aniWhip]->Render(x, y, alpha);
		else
			whip->animations[aniWhip]->Render(x, y + (SIMON_STAND_BBOX_HEIGHT - SIMON_SIT_BBOX_HEIGHT), alpha);
	}

	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_DIE:
		isDead = true;
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HURT:
		isHurt = true;

		vy = -SIMON_DIE_DEFLECT_SPEED;
		if (nx > 0)
		{
			
			vx = -SIMON_DIE_DEFLECT_SPEED;
		}
		else if (nx < 0)
		{
			vx = SIMON_DIE_DEFLECT_SPEED;
		}

		isAttack = false;
		isJump = false;
		isMoving = false;
		isOnStair = false;
		isOnCheckStairDown = false;
		isOnCheckStairUp = false;
		isSit = false;
		isExitSit = false;

		break;
	case SIMON_STATE_ONCHECKSTAIR:
		if (nx < 0)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		else
		{
			vx = SIMON_WALKING_SPEED;
		}
		vy = 0;
		break;
	case SIMON_STATE_ONSTAIR:
		if (!isAttack)
		{
			isMoving = true;
			if (nx < 0)
			{
				if (ny < 0)
				{
					vx = -SIMON_CLIMBING_SPEED_X;
					vy = -SIMON_CLIMBING_SPEED_Y;
				}
				else
				{
					vx = -SIMON_CLIMBING_SPEED_X;
					vy = SIMON_CLIMBING_SPEED_Y;
				}
			}
			else
			{
				if (ny < 0)
				{
					vx = SIMON_CLIMBING_SPEED_X;
					vy = -SIMON_CLIMBING_SPEED_Y;
				}
				else
				{
					vx = SIMON_CLIMBING_SPEED_X;
					vy = SIMON_CLIMBING_SPEED_Y;
				}
			}
		}
		break;
	case SIMON_STATE_ONSTAIR_IDLE:
		isMoving = false;
		vx = 0;
		vy = 0;
		break;
	case SIMON_STATE_SIT:
		isSit = true;
		vx = 0;
		break;
	case SIMON_STATE_WALK:
		isMoving = true;
		if (nx == 1.0f)
		{
			vx = SIMON_WALKING_SPEED;
		}
		else if (nx == -1.0f)
		{
			vx = -SIMON_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_IDLE:
		vx = 0;
		isMoving = false;
		break;
	}
}

void Simon::SetAction(int action)
{
	switch (action)
	{
	case SIMON_ACTION_ATTACK:
		// Ngat tat ca trang thai (tru ngoi)
		// Neu dang nhay toi thi de nguyen van toc, neu khong thi dung lai va danh
		isAttack = true;
		if (!isJump)
			vx = 0;
		isJump = false;
		isMoving = false;
		attackTime = GetTickCount();
		break;
		// Don gian la cho nhay, khong ngat bat ki trang thai nao
		// Them van toc nhay
	case SIMON_ACTION_JUMP:
		isJump = true;
		vy = -SIMON_JUMP_SPEED_Y;
		break;
	}
}

void Simon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + SIMON_STAND_BBOX_WIDTH;
	bottom = y + SIMON_STAND_BBOX_HEIGHT;
}


