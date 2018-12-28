#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "GridObjects.h"
#include "Knife.h"
#include "Axe.h"
#include "HolyWater.h"
#include "Cross.h"

#include "Simon.h"
#include "Ground.h"
#include "Wall.h"
#include "define.h"
#include "BigFire.h"
#include "Candle.h"
#include "Item.h"
#include "Effect.h"
#include "Map.h"
#include "Stair.h"

#include "Enemy.h"
#include "Zombie.h"
#include "RedBat.h"
#include "Panther.h"
#include "MerMan.h"
#include "EnemyBullet.h"
#include "CheckStair.h"
#include "CheckPoint.h"
#include "UI.h"
#include "BossBat.h"
#include "Door.h"
#include "BigBat.h"
#include "Bird.h"
#include "Dracula.h"
#include "Hunchback.h"
#include "Skeleton.h"
#include "SuperDracula.h"

CGame *game;
Simon * simon;
Item *item;
Effect *whipEffect;
Map *map;
UI * ui;
CSprite *sprite;
Weapon *weapon;
Enemy *enemy;

ListGrids *listGrids;
vector<GridObjects*> currentGrids;
int lv = 1;
bool checkload = false;
// check scene lv2->lv2_1
bool checkScene = false;
bool check1 = false;
bool check = false;

//check lv2->lv2_2
bool temp = false;
//check lv34-> 33
bool temp10 = false;
//check scene lv2_1 ->boss
bool checkScene1 = false;
DWORD open_door_time, close_door_time;
bool check_open_door_time = false;
bool check_close_door_time = false;
bool check2 = false;
bool check3 = false;

bool check_enemy_lv34 = false;
bool check_enemy_lv33 = false;
bool countLoadResource2 = false;
bool countLoadResource2_1 = false ;
bool countLoadResource2_2 = false;
bool countLoadResourceboss = false;

bool countLoadResource3_5 = false;
bool countLoadResource3_4 = false;
bool countLoadResource3_3 = false;
bool countLoadResource3_2 = false;
bool countLoadResource3_1 = false;

bool isEnableKeyBoard = true;
bool isClockWeaponUsed = false;
DWORD clockWeaponCast;

bool transparent = false;

DWORD timer; // load enemy
bool count_enemy = true;
DWORD gameTime = 999000;

CSprites * sprites = CSprites::GetInstance();
CAnimations * animations = CAnimations::GetInstance();
CTextures * textures = CTextures::GetInstance();

class CSampleKeyHander : public CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
};

void GenerateWeapon()
{
	int nx = simon->nx;

	simon->SetAction(SIMON_ACTION_ATTACK);
	simon->isUseWhip = false;

	switch (simon->currentWeapon)
	{
	case ITEM_KNIFE:
		weapon = new Knife(simon, 2 * SCREEN_WIDTH / 3);

		if (nx > 0)
		{
			weapon->SetSpeed(KNIFE_SPEED, 0);
			weapon->AddAnimation(WEAPON_ANI_KNIFE_RIGHT);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-KNIFE_SPEED, 0);
			weapon->AddAnimation(WEAPON_ANI_KNIFE_LEFT);
		}

		weapon->SetPosition(simon->x, simon->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_AXE:
		weapon = new Axe(simon);

		if (nx > 0)
		{
			weapon->SetSpeed(AXE_SPEED_X, -AXE_SPEED_Y);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-AXE_SPEED_X, -AXE_SPEED_Y);
		}

		weapon->AddAnimation(WEAPON_ANI_AXE);
		weapon->SetPosition(simon->x, simon->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_HOLYWATER:
		weapon = new HolyWater(simon);

		if (nx > 0)
		{
			weapon->SetSpeed(HOLYWATER_SPEED_X, -HOLYWATER_SPEED_Y);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-HOLYWATER_SPEED_X, -HOLYWATER_SPEED_Y);
		}

		weapon->AddAnimation(WEAPON_ANI_HOLYWATER);
		weapon->AddAnimation(WEAPON_ANI_HOLYWATER_FIRE);
		weapon->SetPosition(simon->x, simon->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;

	case ITEM_CROSS:
		weapon = new Cross(simon, 2 * SCREEN_WIDTH / 5);

		if (nx > 0)
		{
			weapon->SetSpeed(CROSS_SPEED, 0);
		}
		else if (nx < 0)
		{
			weapon->SetSpeed(-CROSS_SPEED, 0);
		}

		weapon->AddAnimation(WEAPON_ANI_CROSS);
		weapon->SetPosition(simon->x, simon->y);
		weapon->firstCast = GetTickCount();
		listGrids->AddObject(weapon);
		break;
	case ITEM_CLOCK:
		isClockWeaponUsed = true;
		clockWeaponCast = GetTickCount();
		break;
	}
}

CSampleKeyHander * keyHandler;

void CSampleKeyHander::OnKeyDown(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (KeyCode == DIK_0)
	{
		lv = 99;
		checkload = false;
		simon->SetPosition(4096, 150);
	}
	if (KeyCode == DIK_1)
		simon->SetPosition(0, 100);

	if (KeyCode == DIK_2)
		simon->SetPosition(1500, 100);

	if (KeyCode == DIK_3)
		simon->SetPosition(3000, 100);

	if (KeyCode == DIK_4)
		simon->SetPosition(4000, 100);

	if (KeyCode == DIK_5)
		simon->SetPosition(5000, 100);

	if (KeyCode == DIK_6)
	{
		lv = 35;
		checkload = false;
		
	}
	if (KeyCode == DIK_Q)
	{
		simon->currentWeapon = ITEM_AXE;
		GenerateWeapon();
	}

	if (KeyCode == DIK_W)
	{
		simon->currentWeapon = ITEM_KNIFE;
		GenerateWeapon();
	}

	if (KeyCode == DIK_E)
	{
		simon->currentWeapon = ITEM_CROSS;
		GenerateWeapon();
	}

	if (KeyCode == DIK_R)
	{
		simon->currentWeapon = ITEM_HOLYWATER;
		GenerateWeapon();
	}

	if (KeyCode == DIK_T)
	{
		simon->currentWeapon = ITEM_CLOCK;
		GenerateWeapon();
	}

	if (KeyCode == DIK_Y)
	{
		simon->isRosaryUsed = true;
		simon->rosaryCast = GetTickCount();
		isEnableKeyBoard = false;
	}

	if (KeyCode == DIK_I)
	{
		simon->SetState(SIMON_STATE_DIE);
	}

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Nhay
		if (KeyCode == DIK_SPACE)
		{
			if (/*simon->isJump == false && */simon->isSit == false && simon->isAttack == false && simon->isOnStair == false)
				simon->SetAction(SIMON_ACTION_JUMP);
		}
		// Danh
		else if (KeyCode == DIK_A)
		{
			if (simon->isAttack == false)
			{
				simon->SetAction(SIMON_ACTION_ATTACK);
				simon->isUseWhip = true;
			}
		}
		// Dùng vũ khí
		else if (KeyCode == DIK_S)
		{
			if (simon->isAttack == false && simon->currentWeapon != 0)
			{
				GenerateWeapon();
			}
		}
	}
}

void CSampleKeyHander::OnKeyUp(int KeyCode)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Len xuong cau thang
		if (KeyCode == DIK_UP)
		{
			if (simon->isOnCheckStairUp)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else if (simon->isOnStair)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
		}

		// Ngoi
		if (KeyCode == DIK_DOWN)
		{
			if (simon->isOnCheckStairDown)
			{
				simon->SetState(SIMON_STATE_IDLE);
			}
			else if (simon->isOnStair)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
			else if (simon->isSit)
			{
				if (!simon->isAttack)
				{
					simon->isSit = false;
				}
				else
				{
					simon->isExitSit = true;
				}
			}
		}

		// Di bo
		else if (KeyCode == DIK_RIGHT || KeyCode == DIK_LEFT)
		{
			simon->isMoving = false;
		}
	}
}

void CSampleKeyHander::KeyState(BYTE *states)
{
	// Chet
	if (simon->GetState() == SIMON_STATE_DIE) return;

	if (isEnableKeyBoard && !simon->isHurt)
	{
		// Len xuong cau thang
		if (game->IsKeyDown(DIK_UP))
		{
			if (simon->isOnCheckStairUp && !simon->isOnStair && simon->currentCheckStairId == 0)
			{
				if (!simon->isAttack && !simon->isSit && !simon->isJump)
				{
					simon->SetState(SIMON_STATE_ONCHECKSTAIRUP);
				}
			}
			else if (simon->isOnStair && !simon->isAttack && simon->currentCheckStairId != 0)
			{
				simon->ny = -1;
				simon->SetState(SIMON_STATE_ONSTAIR);
				if (simon->isLeftToRight)
					simon->nx = 1;
				else
					simon->nx = -1;
			}
			else if (simon->isOnStair && simon->isAttack)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
		}

		// Ngoi
		if (game->IsKeyDown(DIK_DOWN))
		{
			if (simon->isOnCheckStairDown && !simon->isOnStair)
			{
				if (!simon->isAttack && !simon->isSit && !simon->isJump)
				{
					simon->SetState(SIMON_STATE_ONCHECKSTAIRDOWN);
				}
			}
			else if (simon->isOnStair && !simon->isAttack)
			{
				simon->ny = 1;
				simon->SetState(SIMON_STATE_ONSTAIR);
				if (simon->isLeftToRight)
					simon->nx = -1;
				else
					simon->nx = 1;
			}
			else if (simon->isOnStair && simon->isAttack)
			{
				simon->SetState(SIMON_STATE_ONSTAIR_IDLE);
			}
			else if (!simon->isOnCheckStairDown && !simon->isOnStair && !simon->isAttack && !simon->isJump)
				simon->SetState(SIMON_STATE_SIT);
		}

		// Di bo
		if (game->IsKeyDown(DIK_RIGHT))
		{
			if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
				simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
				simon->nx = 1.0f;
		}
		else if (game->IsKeyDown(DIK_LEFT))
		{
			if (!simon->isSit && !simon->isOnStair && !simon->isAttack)
				simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump && !simon->isOnStair && !simon->isAttack)
				simon->nx = -1.0f;
		}

		// Neu khong co gi xay ra se dung im
	}
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void LoadResourceLv1()
{
	// Đặt vị trí xuất phát cho simon
	simon->SetPosition(100, 327); // 100 327

	// Khởi tạo listGrids
	listGrids->InitList(MAX_WIDTH_LV1);

	listGrids->AddObject("Castlevania\\resource\\ResourceLv1.txt",simon,game);
	listGrids->AddObject("Castlevania\\resource\\SuperDracula.txt", 200,170, simon);
	
}

void LoadResourceLv2() {

	listGrids->AddObject("Castlevania\\resource\\ResourceLv2.txt",simon,game);

	/*for (int i = 0; i < 95; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(0 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1377 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1503 + i * 32.0f, 244);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(1854 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();

		ground->SetPosition(2782 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}

	for (int i = 0; i < 5; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(65 + i * 255, 376);
		listGrids->AddObject(candle);
	}
	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(195 + i * 257, 316);
		listGrids->AddObject(candle);
	}

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1219, 326);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1339, 193);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1445, 380);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1800, 380);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1713, 190);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1986, 198);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2601, 387);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2742, 322);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(2868, 190);
	listGrids->AddObject(candle);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(2050 + i * 125, 380);
		listGrids->AddObject(candle);
	}

	

	CheckStair *checkstair;
	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1255, 407);
	checkstair->SetId(1);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1382, 215);
	checkstair->SetId(1);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1448, 278);
	checkstair->SetId(2);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1510, 151);
	checkstair->SetId(2);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	// Top right
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1790, 151);
	checkstair->SetId(3);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	// Bottom left
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(1850, 278);
	checkstair->SetId(3);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	// Bottom right
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(2595, 407);
	checkstair->SetId(4);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	// Top left
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(2788, 153);
	checkstair->SetId(4);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	Panther *panther;
	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1403, 260);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);

	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1770, 200);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);

	panther = new Panther();
	panther->AddAnimation(605);
	panther->AddAnimation(606);
	panther->AddAnimation(607);
	panther->AddAnimation(608);
	panther->SetPosition(1950, 260);
	panther->SetState(PANTHER_STATE_WAIT);
	listGrids->AddObject(panther);*/

	
}

void LoadResourceLv2_1()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv2_1.txt",simon,game);

	/*for (int i = 0; i < 9; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + i * 32 + 32 * 8, 246);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 16; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 9 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 12; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 440);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 3; i++)//
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 17 + i * 32.0f, 246 + 64);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 29 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 31 + i * 32.0f, 246 + 64);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 35 + 3 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}

	Wall *wall = new Wall();
	wall->SetPosition(3585, 280);
	listGrids->AddObject(wall);

	CheckStair *checkstair = new CheckStair();
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3130, 345);
	checkstair->SetId(1);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3394, 215);
	checkstair->SetId(2);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(3515, 406);
	checkstair->SetId(2);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();	
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3780, 345);
	checkstair->SetId(3);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(3839, 218);
	checkstair->SetId(4);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(3950, 406);
	checkstair->SetId(4);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3245, 193);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3389, 159);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3137, 386);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3519, 319);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3684, 155);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3776, 385);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(3905, 150);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4035, 192);
	listGrids->AddObject(candle);

	Door *door = new Door();
	door->AddAnimation(814);
	door->AddAnimation(815);
	door->AddAnimation(816);
	door->SetPosition(3070, 150);	
	listGrids->AddObject(door);

	enemy = new RedBat();
	enemy->AddAnimation(60800);
	enemy->SetPosition(3900,239);
	enemy->SetState(BAT_STATE_FLYING);
	listGrids->AddObject(enemy);

	enemy = new RedBat();
	enemy->AddAnimation(60800);
	enemy->SetPosition(4000, 250);
	enemy->SetState(BAT_STATE_FLYING);
	listGrids->AddObject(enemy);

	enemy = new RedBat();
	enemy->AddAnimation(60800);
	enemy->SetPosition(4200, 270);
	enemy->SetState(BAT_STATE_FLYING);
	listGrids->AddObject(enemy);*/
}

void LoadResourceLv2_2()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv2_2.txt",simon,game);

	/*CheckStair * checkstair;
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(1);
	checkstair->SetPosition(154, 182);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(3);
	checkstair->SetPosition(860, 248);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(192 + i * 32.0f, 216);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 14; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(0 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(512 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(640 + i * 32.0f, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(960 + i * 32.0f, 344);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(896 + i * 32.0f, 408);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 8; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(64 +128*i , 156);
		listGrids->AddObject(candle);
	}*/
	
}

void LoadResourceboss()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv99.txt",simon,game);
	/*for (int i = 0; i < 8; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 40 + i * 32.0f, 246);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 48; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 41 + i * 32.0f, 438);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 11; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 49 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 86 + i * 32.0f, 310);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2782 + 32 * 83 + i * 32.0f, 374);
		listGrids->AddObject(ground);
	}

	Candle *candle;
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4285, 194);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4418, 255);
	listGrids->AddObject(candle);

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(4362, 380);
	listGrids->AddObject(candle);

	for (int i = 0; i < 4; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(4866 + i * 127, 380);
		listGrids->AddObject(candle);
	}

	for (int i = 0; i < 2; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(5438 + i * 127, 310);
		listGrids->AddObject(candle);
	}
	CheckStair *checkstair;
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(1);
	checkstair->SetPosition(4286, 156);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(1);
	checkstair->SetPosition(4346, 279);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(2);
	checkstair->SetPosition(4673, 217);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(2);
	checkstair->SetPosition(4796, 405);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	BossBat *bossbat = new BossBat(simon, game);
	bossbat->AddAnimation(609);
	bossbat->AddAnimation(610);
	bossbat->SetPosition(5325, 125);
	bossbat->SetState(BOSSBAT_STATE_WAIT);
	listGrids->AddObject(bossbat);

	Door *door = new Door();
	door->AddAnimation(814);
	door->AddAnimation(815);
	door->AddAnimation(816);
	door->SetPosition(4080, 150);
	
	listGrids->AddObject(door);*/
}

void LoadResourceLv3_5()
{
	simon->SetPosition(2900, 270);
	listGrids->AddObject("Castlevania\\resource\\ResourceLv35.txt", simon, game);
	/*for (int i = 0; i < 18; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(64 + i * 32 , 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(704 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(832 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(960 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1216 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 12; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1345 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1792 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1984 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2176 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2304 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2431 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 12; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(2689 + i * 32, 410);
		listGrids->AddObject(ground);
	}
	CheckStair *checkstair ;
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(1);
	checkstair->SetPosition(2720, 186);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(1);
	checkstair->SetPosition(2840, 375);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	CheckPoint *checkPoint;

	checkPoint = new CheckPoint();
	checkPoint->SetType(CHECKPOINT_LEVELUP);
	checkPoint->SetPosition(91, 251);
	listGrids->AddObject(checkPoint);

	Candle *candle;
	for (int i = 0; i < 3; i++)
	{
		candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(1346 + 128 * i, 220);
		listGrids->AddObject(candle);
	}

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(186, 163);
	listGrids->AddObject(candle);

	for (int i = 0; i < 2; i++)
	{
		candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(2725 + 90 * i, 350);
		listGrids->AddObject(candle);
	}

	BigBat *bigbat;
	bigbat = new BigBat(simon);
	bigbat->AddAnimation(610);
	bigbat->SetPosition(2456, 163);
	listGrids->AddObject(bigbat);

	bigbat = new BigBat(simon);
	bigbat->AddAnimation(610);
	bigbat->SetPosition(1989, 377);
	listGrids->AddObject(bigbat);

	bigbat = new BigBat(simon);
	bigbat->AddAnimation(610);
	bigbat->SetPosition(1485, 163);
	listGrids->AddObject(bigbat);

	bigbat = new BigBat(simon);
	bigbat->AddAnimation(610);
	bigbat->SetPosition(550, 163);
	listGrids->AddObject(bigbat);*/
	
}

void LoadResourceLv3_4()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv34.txt", simon, game);

	/*for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1408 +i*32, 278);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1025 + i * 32, 344);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)//lỗi lên lthang
	{
		Ground *ground = new Ground();
		ground->SetPosition(991 , 218 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1218 + i * 32, 215);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++) 
	{
		Ground *ground = new Ground();
		ground->SetPosition(960 , 218 + i *32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(834 +i*32, 343);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(641 + i * 32, 343);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(513 + i * 32, 216);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(321 + i * 32, 407);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(449 + i * 32, 407);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(64 + i * 32, 215);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(321, 120 + i*32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(353,120 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(32, 120 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 5; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(611 + i * 32, 278 );
		listGrids->AddObject(ground);
	}

	Candle *candle;
	for (int i = 0; i < 2; i++)
	{		
		candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(324 + 120*i, 350);
		listGrids->AddObject(candle);
	}
	
	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1474, 150);
	listGrids->AddObject(candle);

	for (int i = 0; i < 2; i++)
	{
		candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(1104 + 96 * i, 286);
		listGrids->AddObject(candle);
	}

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(575, 150);
	listGrids->AddObject(candle);


	CheckStair *checkstair;

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(10);
	checkstair->SetPosition(137, 181);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(2);
	checkstair->SetPosition(226, 120);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(2);
	checkstair->SetPosition(409, 377);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(3);
	checkstair->SetPosition(583, 181);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(4);
	checkstair->SetPosition(1093, 312);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(4);
	checkstair->SetPosition(1217, 120);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(5);
	checkstair->SetPosition(1290, 181);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	Skeleton *skeleton;
	skeleton = new Skeleton();
	skeleton->AddAnimation(818);
	skeleton->AddAnimation(819);
	skeleton->SetPosition(1156, 220);
	skeleton->SetState(SKELETON_WALKING);
	listGrids->AddObject(skeleton);*/
		
}

void LoadResourceLv3_3()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv33.txt", simon, game);
	/*for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1410 + i * 32, 407);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1471 , 152 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 14; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(896 + i * 32, 216);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 5; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(993, 248 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 10; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1025 + i * 32, 408);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(704 + i * 32, 408);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(1409 + i * 32, 279);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(962 , 280 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 5; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(672 + i * 32, 216 );
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(512 + i * 32, 344);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(512 + i * 32, 376);
		listGrids->AddObject(ground);
	}
	
	for (int i = 0; i < 3; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(865 + i * 32, 408);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(0 + i * 32, 280);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 2; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(256 + i * 32, 408);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 4; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(320 , 280 + i * 32);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 5; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(736 + i * 32 ,344 );
		listGrids->AddObject(ground);
	}

	Candle * candle;
	for (int i = 0; i < 2; i++)
	{
		candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(1128 + 128 * i, 146);
		listGrids->AddObject(candle);
	}

	candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(1221, 286);
	listGrids->AddObject(candle);
	CheckStair * checkstair;

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(1);
	checkstair->SetPosition(96, 188);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(1);
	checkstair->SetPosition(219, 376);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(2);
	checkstair->SetPosition(608, 250);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(2);
	checkstair->SetPosition(664, 376);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(3);
	checkstair->SetPosition(704, 316);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(4);
	checkstair->SetPosition(1287, 376);
	checkstair->SetType(CHECKSTAIR_UP_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(5);
	checkstair->SetPosition(1312, 124);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetId(5);
	checkstair->SetPosition(1370, 248);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetId(4);
	checkstair->SetPosition(1411, 187);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1413, 313);
	checkstair->SetId(5);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(265, 313);
	checkstair->SetId(10);
	checkstair->SetType(CHECKSTAIR_DOWN_LEFT);
	listGrids->AddObject(checkstair);

	Skeleton *skeleton;
	skeleton = new Skeleton();
	skeleton->AddAnimation(818);
	skeleton->AddAnimation(819);
	skeleton->SetPosition(1173, 330);
	skeleton->SetState(SKELETON_WALKING);
	listGrids->AddObject(skeleton);

	
	skeleton = new Skeleton();
	skeleton->AddAnimation(818);
	skeleton->AddAnimation(819);
	skeleton->SetPosition(1173, 150);
	skeleton->SetState(SKELETON_WALKING);
	listGrids->AddObject(skeleton);*/
	
}

void LoadResourceLv3_2()
{
	listGrids->AddObject("Castlevania\\resource\\ResourceLv32.txt", simon, game);

	/*for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(351 + i * 32, 279);
		listGrids->AddObject(ground);
	}

	CheckStair * checkstair;
	checkstair = new CheckStair();
	checkstair->AddAnimation(803);
	checkstair->SetPosition(313, 248);
	checkstair->SetId(1);
	checkstair->SetType(CHECKSTAIR_UP_LEFT);
	listGrids->AddObject(checkstair);*/
}

void LoadResourceLv3_1()
{	
	listGrids->AddObject("Castlevania\\resource\\ResourceLv31.txt", simon, game);

	//simon->SetPosition(50, 150);
	/*for (int i = 0; i < 29; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(0 + i * 32, 407);
		listGrids->AddObject(ground);
	}
	for (int i = 0; i < 6; i++)
	{
		Ground *ground = new Ground();
		ground->SetPosition(895 + i * 32, 344);
		listGrids->AddObject(ground);

	}
	for (int i = 0; i < 7; i++)
	{
		Candle *candle = new Candle();
		candle->AddAnimation(800);
		candle->SetPosition(62 + i*128, 281);
		listGrids->AddObject(candle);
	}
	Candle *candle = new Candle();
	candle->AddAnimation(800);
	candle->SetPosition(958, 230);
	listGrids->AddObject(candle);

	


	CheckStair * checkstair;
	checkstair = new CheckStair();
	checkstair->AddAnimation(804);
	checkstair->SetPosition(1045, 250);
	checkstair->SetId(1);
	checkstair->SetType(CHECKSTAIR_DOWN_RIGHT);
	listGrids->AddObject(checkstair);

	Dracula *dracula;
	dracula = new Dracula(simon, game);
	dracula->AddAnimation(826);
	dracula->AddAnimation(8270);
	dracula->AddAnimation(8290);
	dracula->AddAnimation(827);
	dracula->AddAnimation(829);
	dracula->AddAnimation(828);
	dracula->AddAnimation(830);
	dracula->SetPosition(500, 360);
	listGrids->AddObject(dracula);*/
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{	
	textures->Add("Castlevania\\filetxt\\textures.txt", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add("Castlevania\\filetxt\\textures1.txt", D3DCOLOR_XRGB(96, 68, 106));
	textures->Add("Castlevania\\filetxt\\textures2.txt", D3DCOLOR_XRGB(153, 43, 102));
	#pragma region ManageSprite
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	sprites->Add("Castlevania\\filetxt\\spr_Simon_right.txt", texSimon);

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);
	sprites->Add("Castlevania\\filetxt\\spr_Simon_left.txt", texSimon2);

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);
	sprites->Add("Castlevania\\filetxt\\spr_whip_right.txt", whipR);

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);
	sprites->Add("Castlevania\\filetxt\\spr_whip_left.txt", whipL);

	LPDIRECT3DTEXTURE9 axe = textures->Get(ID_TEX_AXE_ACTION);
	sprites->Add("Castlevania\\filetxt\\spr_axe_action.txt", axe);

	LPDIRECT3DTEXTURE9 holywater = textures->Get(ID_TEX_HOLY_WATER);
	sprites->Add("Castlevania\\filetxt\\spr_holy.txt", holywater);

	LPDIRECT3DTEXTURE9 holywater_action = textures->Get(ID_TEX_HOLY_WATER_ACTION);
	sprites->Add("Castlevania\\filetxt\\spr_holy_action.txt", holywater_action);

	LPDIRECT3DTEXTURE9 cross = textures->Get(ID_TEX_CROSS_ACTION);
	sprites->Add("Castlevania\\filetxt\\spr_cross.txt", cross);

	LPDIRECT3DTEXTURE9 knife = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add("Castlevania\\filetxt\\spr_knife.txt", knife);

	LPDIRECT3DTEXTURE9 clock = textures->Get(ID_TEX_UI);
	sprites->Add("Castlevania\\filetxt\\spr_clock.txt", clock);

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add("Castlevania\\filetxt\\spr_brick.txt", texMisc);

	LPDIRECT3DTEXTURE9 texMisc2 = textures->Get(ID_TEX_BRICK2);
	sprites->Add("Castlevania\\filetxt\\spr_brick2.txt", texMisc2);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add("Castlevania\\filetxt\\spr_zombie_left.txt", texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add("Castlevania\\filetxt\\spr_zombie.txt", texEnemy0);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add("Castlevania\\filetxt\\spr_panther_left.txt", texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy10 = textures->Get(ID_TEX_PANTHER_RIGHT);
	sprites->Add("Castlevania\\filetxt\\spr_panther_right.txt", texEnemy10);

	LPDIRECT3DTEXTURE9 texEnemy11 = textures->Get(ID_TEX_BAT);
	sprites->Add("Castlevania\\filetxt\\spr_bat.txt", texEnemy11);

	LPDIRECT3DTEXTURE9 texEnemy12 = textures->Get(ID_TEX_BOSS);
	sprites->Add("Castlevania\\filetxt\\spr_boss.txt", texEnemy12);

	LPDIRECT3DTEXTURE9 texEnemy13 = textures->Get(ID_TEX_MERMAN_LEFT);
	sprites->Add("Castlevania\\filetxt\\spr_merman_left.txt", texEnemy13);

	LPDIRECT3DTEXTURE9 texEnemy14 = textures->Get(ID_TEX_MERMAN_RIGHT);
	sprites->Add("Castlevania\\filetxt\\spr_merman_right.txt", texEnemy14);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add("Castlevania\\filetxt\\spr_fire.txt", texEnemy2);

	LPDIRECT3DTEXTURE9 texEnemy3 = textures->Get(ID_TEX_CANDLE);
	sprites->Add("Castlevania\\filetxt\\spr_candle.txt", texEnemy3);

	LPDIRECT3DTEXTURE9 texMisc3 = textures->Get(ID_TEX_LADDER);
	sprites->Add("Castlevania\\filetxt\\spr_ladder.txt", texMisc3);

	LPDIRECT3DTEXTURE9 texMisc4 = textures->Get(ID_TEX_LADDER_LEFT);
	sprites->Add("Castlevania\\filetxt\\spr_ladder_left.txt", texMisc4);

	LPDIRECT3DTEXTURE9 texMisc5 = textures->Get(ID_TEX_STAIR_BOT);
	sprites->Add("Castlevania\\filetxt\\spr_stair_bot.txt", texMisc5);

	LPDIRECT3DTEXTURE9 texMisc6 = textures->Get(ID_TEX_STAIR_TOP);
	sprites->Add("Castlevania\\filetxt\\spr_stair_top.txt", texMisc6);

	LPDIRECT3DTEXTURE9 texMisc7 = textures->Get(ID_TEX_UI);
	sprites->Add("Castlevania\\filetxt\\spr_ui.txt", texMisc7);

	LPDIRECT3DTEXTURE9 texMisc8 = textures->Get(ID_TEX_EFFECT1);
	sprites->Add("Castlevania\\filetxt\\spr_effect1.txt", texMisc8);

	LPDIRECT3DTEXTURE9 texMisc9 = textures->Get(ID_TEX_EFFECT2);
	sprites->Add("Castlevania\\filetxt\\spr_effect2.txt", texMisc9);

	LPDIRECT3DTEXTURE9 texMisc10 = textures->Get(ID_TEX_WHIP_VIP);
	sprites->Add("Castlevania\\filetxt\\spr_vip.txt", texMisc10);

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add("Castlevania\\filetxt\\spr_knife_action.txt", texMisc11);

	LPDIRECT3DTEXTURE9 texMisc12 = textures->Get(ID_TEX_MONEY);
	sprites->Add("Castlevania\\filetxt\\spr_money.txt", texMisc12);

	LPDIRECT3DTEXTURE9 texMisc13 = textures->Get(ID_TEX_ROSARY);
	sprites->Add("Castlevania\\filetxt\\spr_rosary.txt", texMisc13);

	LPDIRECT3DTEXTURE9 texMisc14 = textures->Get(ID_TEX_BOSSES);
	sprites->Add("Castlevania\\filetxt\\spr_bosses.txt", texMisc14);

	LPDIRECT3DTEXTURE9 texMic15 = textures->Get(ID_TEX_GATE);
	sprites->Add("Castlevania\\filetxt\\spr_door.txt", texMic15);

	LPDIRECT3DTEXTURE9 texMic16 = textures->Get(ID_TEX_BOSSES_RIGHT);
	sprites->Add("Castlevania\\filetxt\\spr_bosses_right.txt", texMic16);

	LPDIRECT3DTEXTURE9 texMic17 = textures->Get(ID_TEX_ENEMIES);
	sprites->Add("Castlevania\\filetxt\\spr_enemy_left.txt", texMic17);

	LPDIRECT3DTEXTURE9 texMic18 = textures->Get(ID_TEX_ENEMIES_RIGHT);
	sprites->Add("Castlevania\\filetxt\\spr_enemy_right.txt", texMic18);

	LPDIRECT3DTEXTURE9 texMic19 = textures->Get(ID_TEX_INVICIBILITY);
	sprites->Add("Castlevania\\filetxt\\spr_invicibility.txt", texMic19);

	LPDIRECT3DTEXTURE9 texMic21 = textures->Get(48);
	sprites->Add("Castlevania\\filetxt\\spr_rosary_action.txt", texMic21);
#pragma endregion

	animations->Add("Castlevania\\filetxt\\animation.txt");
	
	/*#pragma region Addsprite
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	sprites->Add(10001, 436, 0, 484, 64, texSimon);		// đứng im phải

	sprites->Add(10002, 260, 0, 308, 64, texSimon);		// đi phải
	sprites->Add(10003, 314, 0, 362, 64, texSimon);
	sprites->Add(11002, 378, 0, 426, 64, texSimon);
	sprites->Add(11003, 436, 0, 484, 64, texSimon);

	sprites->Add(10004, 120, 0, 168, 64, texSimon);		// đánh phải			
	sprites->Add(10005, 60, 0, 108, 64, texSimon);
	sprites->Add(10006, 0, 0, 64, 64, texSimon);

	sprites->Add(10028, 0, 67, 49, 130, texSimon);		// ngồi đánh phải			
	sprites->Add(10029, 420, 134, 469, 197, texSimon);
	sprites->Add(10030, 360, 134, 421, 197, texSimon);

	sprites->Add(10034, 120, 134, 168, 197, texSimon);		// trên thang phải đánh phải			
	sprites->Add(10035, 60, 134, 109, 197, texSimon);
	sprites->Add(10036, 0, 134, 59, 197, texSimon);

	sprites->Add(10040, 302, 134, 350, 197, texSimon);		// trên thang trái đánh phải			
	sprites->Add(10041, 242, 134, 290, 197, texSimon);
	sprites->Add(10042, 182, 134, 239, 197, texSimon);

	sprites->Add(10046, 133, 67, 165, 130, texSimon); // lên thang phải
	sprites->Add(10047, 196, 67, 228, 130, texSimon);

	sprites->Add(10052, 133, 67, 165, 130, texSimon); //xuống thang phải
	sprites->Add(10053, 318, 67, 350, 130, texSimon);

	sprites->Add(10018, 196, 0, 230, 64, texSimon); // ngồi phải

	sprites->Add(10020, 196, 0, 230, 64, texSimon); // nhảy phải
	
	sprites->Add(10054, 436, 67, 471, 130, texSimon);//bị đánh từ bên trái

	LPDIRECT3DTEXTURE9 texSimon2 = textures->Get(ID_TEX_SIMON_2);

	sprites->Add(10011, 12, 0, 44, 64, texSimon2);		// đứng im trái

	sprites->Add(10012, 196, 0, 244, 64, texSimon2);		// đi trái
	sprites->Add(10013, 136, 0, 184, 64, texSimon2);
	sprites->Add(11012, 78, 0, 126, 64, texSimon2);
	sprites->Add(11013, 12, 0, 60, 64, texSimon2);

	sprites->Add(10014, 301, 0, 360, 64, texSimon2);		// đánh trái				
	sprites->Add(10015, 361, 0, 404, 64, texSimon2);
	sprites->Add(10016, 421, 0, 464, 64, texSimon2);

	sprites->Add(10031, 420, 67, 477, 130, texSimon2);		// ngồi đánh trái				
	sprites->Add(10032, 0, 134, 44, 197, texSimon2);
	sprites->Add(10033, 60, 134, 104, 197, texSimon2);

	sprites->Add(10037, 120, 134, 177, 197, texSimon2);		// trên thang phải đánh trái			
	sprites->Add(10038, 180, 134, 225, 197, texSimon2);
	sprites->Add(10039, 240, 134, 282, 197, texSimon2);

	sprites->Add(10043, 300, 134, 361, 197, texSimon2);		// trên thang trái đánh trái			
	sprites->Add(10044, 360, 134, 405, 197, texSimon2);
	sprites->Add(10045, 420, 134, 464, 197, texSimon2);

	sprites->Add(10048, 190, 67, 224, 130, texSimon2); // xuống thang trái
	sprites->Add(10049, 129, 67, 163, 130, texSimon2);

	sprites->Add(10050, 190, 67, 224, 130, texSimon2); //lên thang trái
	sprites->Add(10051, 252, 67, 286, 130, texSimon2);

	sprites->Add(10019, 252, 0, 284, 64, texSimon2); // ngồi trái

	sprites->Add(10021, 256, 0, 286, 64, texSimon2);//nhảy trái

	sprites->Add(10055, 12, 70, 41, 130, texSimon2);//bị đánh từ bên phải

	sprites->Add(10099, 180, 237, 240, 264, texSimon);		// chết 

	LPDIRECT3DTEXTURE9 whipR = textures->Get(ID_TEX_WHIP);

	sprites->Add(10022, 570, 200, 554, 267, whipR);			//roi lv2 phải
	sprites->Add(10023, 342, 260, 312, 316, whipR);
	sprites->Add(10024, 342, 328, 458, 364, whipR); // 116

	sprites->Add(10062, 570, 200, 554, 267, whipR);			//roi lv1 phải	
	sprites->Add(10063, 342, 260, 312, 316, whipR);
	sprites->Add(10064, 112, 65, 189, 100, whipR); 
	
	sprites->Add(10056, 570, 0, 553, 67, whipR);			//roi lv0 phải	
	sprites->Add(10057, 346, 0, 312, 49, whipR);
	sprites->Add(10058, 114, 0, 188, 30, whipR);			// 74

	LPDIRECT3DTEXTURE9 whipL = textures->Get(ID_TEX_WHIP_2);

	sprites->Add(10025, 105, 70, 165, 133, whipL);			//roi lv2 trái
	sprites->Add(10026, 340, 65, 406, 117, whipL);
	sprites->Add(10027, 575, 330, 458, 364, whipL);

	sprites->Add(10065, 105, 70, 165, 133, whipL);			//roi lv1 trái	
	sprites->Add(10066, 340, 65, 406, 117, whipL);
	sprites->Add(10067, 572, 65, 530, 100, whipL); 

	sprites->Add(10059, 110, 0, 166, 66, whipL);			//roi lv0 trái
	sprites->Add(10060, 349, 0, 408, 52, whipL);
	sprites->Add(10061, 585, 0, 529, 30, whipL);

	LPDIRECT3DTEXTURE9 axe = textures->Get(ID_TEX_AXE_ACTION); // Rìu
	sprites->Add(10068, 0, 0, 30, 28, axe);
	sprites->Add(10069, 30, 0, 60, 28, axe);
	sprites->Add(10070, 60, 0, 90, 28, axe);
	sprites->Add(10071, 90, 0, 120, 28, axe);

	LPDIRECT3DTEXTURE9 holywater = textures->Get(ID_TEX_HOLY_WATER); // Nước thánh item
	sprites->Add(50072, 0, 0, 32, 32, holywater);

	LPDIRECT3DTEXTURE9 holywater_action = textures->Get(ID_TEX_HOLY_WATER_ACTION); // Nước thánh
	sprites->Add(10072, 0, 0, 32, 26, holywater_action);
	sprites->Add(10073, 32, 0, 64, 26, holywater_action);
	sprites->Add(10074, 64, 0, 96, 26, holywater_action);

	LPDIRECT3DTEXTURE9 cross = textures->Get(ID_TEX_CROSS_ACTION); // Boomerang
	sprites->Add(10075, 0, 0, 26, 28, cross);
	sprites->Add(10076, 29, 0, 56, 28, cross);
	sprites->Add(10077, 55, 1, 84, 28, cross);

	LPDIRECT3DTEXTURE9 knife = textures->Get(ID_TEX_KNIFE_ACTION); // Dao
	sprites->Add(10078, 0, 0, 32, 18, knife);					// dao trái
	sprites->Add(10079, 32, 0, 64, 18, knife);					// dao phải

	LPDIRECT3DTEXTURE9 clock = textures->Get(ID_TEX_UI); // Clock
	sprites->Add(10080, 128, 0, 160, 32, clock);

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_BRICK);
	sprites->Add(20001, 0, 0, 32, 32, texMisc);

	LPDIRECT3DTEXTURE9 texMisc2 = textures->Get(ID_TEX_BRICK2);
	sprites->Add(20002, 0, 0, 32, 32, texMisc2);

	LPDIRECT3DTEXTURE9 texEnemy = textures->Get(ID_TEX_ZOMBIE);
	sprites->Add(30001, 0, 0, 32, 64, texEnemy); // đi trái 
	sprites->Add(30002, 36, 0, 68, 64, texEnemy);

	LPDIRECT3DTEXTURE9 texEnemy0 = textures->Get(ID_TEX_ZOMBIE_RIGHT);
	sprites->Add(30003, 0, 0, 32, 64, texEnemy0); // đi Phải 
	sprites->Add(30004, 36, 0, 68, 64, texEnemy0);

	LPDIRECT3DTEXTURE9 texEnemy1 = textures->Get(ID_TEX_PANTHER);
	sprites->Add(30011, 0, 0, 64, 32, texEnemy1); // báo
	sprites->Add(30012, 64, 0, 128, 32, texEnemy1); // báo chạy trái
	sprites->Add(30013, 128, 0, 192, 32, texEnemy1);
	sprites->Add(30014, 192, 0, 256, 32, texEnemy1);

	LPDIRECT3DTEXTURE9 texEnemy10 = textures->Get(ID_TEX_PANTHER_RIGHT);	
	sprites->Add(30015, 128, 0, 192, 32, texEnemy10); //báo  chạy phải
	sprites->Add(30016, 64, 0, 128, 32, texEnemy10);
	sprites->Add(30017, 0, 0, 64, 32, texEnemy10);

	LPDIRECT3DTEXTURE9 texEnemy11 = textures->Get(ID_TEX_BAT);
	sprites->Add(30018, 0, 0, 32, 32, texEnemy11); // dơi // nằm chờ
	sprites->Add(30019, 32, 0, 64, 32, texEnemy11);
	sprites->Add(30020, 64, 0, 96, 32, texEnemy11);
	sprites->Add(30021, 96, 0, 128, 32, texEnemy11);

	LPDIRECT3DTEXTURE9 texEnemy12 = textures->Get(ID_TEX_BOSS);
	sprites->Add(30022, 27, 0, 67, 38, texEnemy12); // boss // nằm chờ
	sprites->Add(30023, 98, 0, 200, 45, texEnemy12);
	sprites->Add(30024, 213, 0, 281, 45, texEnemy12);
	
	LPDIRECT3DTEXTURE9 texEnemy13 = textures->Get(ID_TEX_MERMAN_LEFT);
	sprites->Add(30025, 0, 0, 32, 64, texEnemy13); //người cá trái
	sprites->Add(30026, 32, 0, 64, 64, texEnemy13);
	sprites->Add(30027, 64, 0, 96, 64, texEnemy13);

	LPDIRECT3DTEXTURE9 texEnemy14 = textures->Get(ID_TEX_MERMAN_RIGHT);
	sprites->Add(30028, 64, 0, 96, 64, texEnemy14); // người cá phải
	sprites->Add(30029, 32, 0, 64, 64, texEnemy14);
	sprites->Add(30030, 0, 0, 32, 64, texEnemy14);

	LPDIRECT3DTEXTURE9 texEnemy2 = textures->Get(ID_TEX_FIRE);
	sprites->Add(40011, 0, 0, 32, 64, texEnemy2);
	sprites->Add(40012, 32, 0, 64, 64, texEnemy2);

	LPDIRECT3DTEXTURE9 texEnemy3 = textures->Get(ID_TEX_CANDLE);
	sprites->Add(40013, 0, 0, 16, 32, texEnemy3);
	sprites->Add(40014, 16, 0, 32, 32, texEnemy3);

	LPDIRECT3DTEXTURE9 texMisc3 = textures->Get(ID_TEX_LADDER);
	sprites->Add(40015, 0, 0, 32, 32, texMisc3);

	LPDIRECT3DTEXTURE9 texMisc4 = textures->Get(ID_TEX_LADDER_LEFT);
	sprites->Add(40016, 0, 0, 32, 32, texMisc4);

	LPDIRECT3DTEXTURE9 texMisc5 = textures->Get(ID_TEX_STAIR_BOT);
	sprites->Add(40017, 0, 0, 32, 32, texMisc5);

	LPDIRECT3DTEXTURE9 texMisc6 = textures->Get(ID_TEX_STAIR_TOP);
	sprites->Add(40018, 0, 0, 32, 32, texMisc6);

	LPDIRECT3DTEXTURE9 texMisc7 = textures->Get(ID_TEX_UI);
	sprites->Add(40019, 0, 33, 16, 50, texMisc7);

	LPDIRECT3DTEXTURE9 texMisc8 = textures->Get(ID_TEX_EFFECT1);
	sprites->Add(40020, 0, 0, 16, 20, texMisc8);

	LPDIRECT3DTEXTURE9 texMisc9 = textures->Get(ID_TEX_EFFECT2);
	sprites->Add(40021, 0, 10, 42, 37, texMisc9);
	sprites->Add(41021, 42, 10, 84, 37, texMisc9);
	sprites->Add(42021, 84, 10, 126, 37, texMisc9);

	LPDIRECT3DTEXTURE9 texMisc10 = textures->Get(ID_TEX_WHIP_VIP);
	sprites->Add(40022, 0, 0, 32, 32, texMisc10);

	LPDIRECT3DTEXTURE9 texMisc11 = textures->Get(ID_TEX_KNIFE_ACTION);
	sprites->Add(40023, 0, 0, 32, 18, texMisc11);

	LPDIRECT3DTEXTURE9 texMisc12 = textures->Get(ID_TEX_MONEY);
	sprites->Add(40024, 0, 0, 30, 30, texMisc12);

	LPDIRECT3DTEXTURE9 texMisc13 = textures->Get(ID_TEX_ROSARY);
	sprites->Add(40025, 0, 0, 32, 32, texMisc13);
	#pragma endregion*/	
	/*LPANIMATION ani;

	#pragma region SimonAnimation

	ani = new CAnimation(100);	//đứng phải
	ani->Add(10001);
	animations->Add(400, ani);

	ani = new CAnimation(100);	//đứng trái
	ani->Add(10011);
	animations->Add(401, ani);

	ani = new CAnimation(180);	//đi phải
	ani->Add(10002);
	ani->Add(10003);
	ani->Add(11002);
	ani->Add(11003);
	animations->Add(500, ani);

	ani = new CAnimation(180);	//đi trái
	ani->Add(10012);
	ani->Add(10013);
	ani->Add(11012);
	ani->Add(11013);
	animations->Add(501, ani);

	ani = new CAnimation(150); //đánh phải
	ani->Add(10004);
	ani->Add(10005);
	ani->Add(10006);

	animations->Add(402, ani);

	ani = new CAnimation(150);//đánh trái
	ani->Add(10014);
	ani->Add(10015);
	ani->Add(10016);

	animations->Add(403, ani);

	ani = new CAnimation(100); //ngồi phải
	ani->Add(10018);
	animations->Add(404, ani);

	ani = new CAnimation(100);//ngồi trái
	ani->Add(10019);
	animations->Add(405, ani);

	ani = new CAnimation(100);//nhảy phải
	ani->Add(10020);
	animations->Add(406, ani);

	ani = new CAnimation(100);//nhảy trái
	ani->Add(10021);
	animations->Add(407, ani);

	ani = new CAnimation(150);//roi phải 
	ani->Add(10022);
	ani->Add(10023);
	ani->Add(10024);
	animations->Add(408, ani);

	ani = new CAnimation(150);//roi trái
	ani->Add(10025);
	ani->Add(10026);
	ani->Add(10027);
	animations->Add(409, ani);

	ani = new CAnimation(150);//ngồi đánh phải
	ani->Add(10028);
	ani->Add(10029);
	ani->Add(10030);
	animations->Add(410, ani);

	ani = new CAnimation(150);//ngồi đánh trái
	ani->Add(10031);
	ani->Add(10032);
	ani->Add(10033);
	animations->Add(411, ani);

	ani = new CAnimation(150);//trên thang phải đánh phải
	ani->Add(10034);
	ani->Add(10035);
	ani->Add(10036);
	animations->Add(412, ani);

	ani = new CAnimation(150);//trên thang phải đánh trái
	ani->Add(10037);
	ani->Add(10038);
	ani->Add(10039);
	animations->Add(413, ani);

	ani = new CAnimation(150);//trên thang trái đánh phải
	ani->Add(10040);
	ani->Add(10041);
	ani->Add(10042);
	animations->Add(414, ani);

	ani = new CAnimation(150);//trên thang trái đánh trái
	ani->Add(10043);
	ani->Add(10044);
	ani->Add(10045);
	animations->Add(415, ani);

	ani = new CAnimation(100);//lên thang phải
	ani->Add(10046);
	ani->Add(10047);
	animations->Add(416, ani);

	ani = new CAnimation(100);//xuống thang phải
	ani->Add(10048);
	ani->Add(10049);
	animations->Add(417, ani);

	ani = new CAnimation(100);//lên thang trái
	ani->Add(10050);
	ani->Add(10051);
	animations->Add(418, ani);

	ani = new CAnimation(100);//xuống thang trái
	ani->Add(10052);
	ani->Add(10053);
	animations->Add(419, ani);

	ani = new CAnimation(100);//đứng im bên phải thang phải
	ani->Add(10047);
	animations->Add(420, ani);

	ani = new CAnimation(100);//đứng im bên phải thang trái
	ani->Add(10049);
	animations->Add(421, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10051);
	animations->Add(422, ani);

	ani = new CAnimation(100);//đứng im bên trái thang phải
	ani->Add(10053);
	animations->Add(423, ani);

	ani = new CAnimation(100);//bị đánh từ bên trái
	ani->Add(10054);	
	animations->Add(424, ani);

	ani = new CAnimation(100);//bị đánh từ bên phải
	ani->Add(10055);	
	animations->Add(425, ani);

	ani = new CAnimation(150);//roi lv0 phải
	ani->Add(10056);
	ani->Add(10057);
	ani->Add(10058);
	animations->Add(426, ani);

	ani = new CAnimation(150);//roi lv0 trái
	ani->Add(10059);
	ani->Add(10060);
	ani->Add(10061);
	animations->Add(427, ani);

	ani = new CAnimation(150);//roi lv1 phải
	ani->Add(10062);
	ani->Add(10063);
	ani->Add(10064);
	animations->Add(428, ani);

	ani = new CAnimation(150);//roi lv1 trái
	ani->Add(10065);
	ani->Add(10066);
	ani->Add(10067);
	animations->Add(429, ani);

	ani = new CAnimation(150); // axe item
	ani->Add(10069);
	animations->Add(4430, ani);

	ani = new CAnimation(150); // axe
	ani->Add(10068);
	ani->Add(10069);
	ani->Add(10070);
	ani->Add(10071);
	animations->Add(430, ani);

	ani = new CAnimation(150); // holywater item
	ani->Add(50072);
	animations->Add(4431, ani);

	ani = new CAnimation(150); //holywater
	ani->Add(10072);
	animations->Add(431, ani);

	ani = new CAnimation(100); //holywater fire
	ani->Add(10073);
	ani->Add(10074);
	animations->Add(4310, ani);

	ani = new CAnimation(150); // cross item
	ani->Add(10077);
	animations->Add(4432, ani);

	ani = new CAnimation(150); // cross
	ani->Add(10075);
	ani->Add(10076);
	ani->Add(10077);
	animations->Add(432, ani);

	ani = new CAnimation(150); // knife item
	ani->Add(10079);
	animations->Add(4433, ani);

	ani = new CAnimation(150); // knife trái
	ani->Add(10078);
	animations->Add(4330, ani);

	ani = new CAnimation(150); // knife phải
	ani->Add(10079);
	animations->Add(4331, ani);
	
	ani = new CAnimation(150); // rosary item
	ani->Add(10090);
	animations->Add(4434, ani);

	ani = new CAnimation(150); // rosary action
	ani->Add(90010);
	animations->Add(4444, ani);
	
	ani = new CAnimation(150); // clock item
	ani->Add(10080);
	animations->Add(4435, ani);

	ani = new CAnimation(100);	// chết	
	ani->Add(10099);
	animations->Add(599, ani);

#pragma endregion

	#pragma region ObjectAnimation

	ani = new CAnimation(100);	//đất1
	ani->Add(20001);
	animations->Add(601, ani);

	ani = new CAnimation(100); //zombie đi trái
	ani->Add(30001);
	ani->Add(30002);
	animations->Add(602, ani);

	ani = new CAnimation(100); //zombie đi phải
	ani->Add(30003);
	ani->Add(30004);
	animations->Add(604, ani);

	ani = new CAnimation(100);	//đất2
	ani->Add(20002);
	animations->Add(603, ani);

	ani = new CAnimation(100); //panther nằm chờ
	ani->Add(30011);
	animations->Add(605, ani);

	ani = new CAnimation(100); //panther chạy trái
	ani->Add(30012);
	ani->Add(30013);
	ani->Add(30014);
	animations->Add(606, ani);

	ani = new CAnimation(100); //panther chạy phải
	ani->Add(30015);
	ani->Add(30016);
	ani->Add(30017);
	animations->Add(607, ani);

	ani = new CAnimation(100); //panther phóng
	ani->Add(30014);
	animations->Add(608, ani);

	ani = new CAnimation(100); //dơi bay trái
	ani->Add(30019);
	ani->Add(30020);
	ani->Add(30021);
	ani->Add(30020);
	animations->Add(60800, ani);

	ani = new CAnimation(100); //boss dơi nằm chờ
	ani->Add(30022);
	animations->Add(609, ani);

	ani = new CAnimation(100); //boosss dơi bay
	ani->Add(30023);
	ani->Add(30024);
	animations->Add(610, ani);

	ani = new CAnimation(100); //merman bay
	ani->Add(30030);	
	animations->Add(611, ani);

	ani = new CAnimation(100); //merman đi trái
	ani->Add(30026);
	ani->Add(30027);
	animations->Add(612, ani);

	ani = new CAnimation(100); //merman đánh trái
	ani->Add(30025);
	animations->Add(613, ani);

	ani = new CAnimation(100); //merman đi phải
	ani->Add(30029);
	ani->Add(30030);
	animations->Add(614, ani);

	ani = new CAnimation(100); //merman đánh phải
	ani->Add(30028);
	animations->Add(615, ani);

	ani = new CAnimation(100); //fire
	ani->Add(40011);
	ani->Add(40012);
	animations->Add(700, ani);

	ani = new CAnimation(100); //candle
	ani->Add(40013);
	ani->Add(40014);
	animations->Add(800, ani);

	ani = new CAnimation(0); //STAIR RIHGT
	ani->Add(40015);
	animations->Add(801, ani);

	ani = new CAnimation(0); //STAIR LEFT
	ani->Add(40016);
	animations->Add(802, ani);

	ani = new CAnimation(0); //STAIR BOT
	ani->Add(40017);
	animations->Add(803, ani);

	ani = new CAnimation(0); //STAIR TOP
	ani->Add(40018);
	animations->Add(804, ani);

	ani = new CAnimation(0); //heart
	ani->Add(40019);
	animations->Add(805, ani);

	ani = new CAnimation(150); //hieu ung' dau' sao
	ani->Add(40020);
	animations->Add(806, ani);

	ani = new CAnimation(50); //hieu ung toe' lua
	ani->Add(40021);
	ani->Add(41021);
	ani->Add(42021);
	animations->Add(807, ani);

	ani = new CAnimation(0); //whip item nang cap
	ani->Add(40022);
	animations->Add(808, ani);

	ani = new CAnimation(0); //money
	ani->Add(40024);
	animations->Add(810, ani);

	ani = new CAnimation(50);//boss dơi chết
	ani->Add(40025);
	ani->Add(40026);
	ani->Add(40027);
	animations->Add(811, ani);

	ani = new CAnimation(0);// đạn từ dracula, dơi, người cá
	ani->Add(40028);
	animations->Add(812, ani);

	ani = new CAnimation(100);// thưởng rớt ra sau khi boss dơi chết
	ani->Add(40029);
	ani->Add(40030);
	animations->Add(813, ani);

	ani = new CAnimation(500);// mở cửa
	ani->Add(40031);
	ani->Add(40032);
	animations->Add(814, ani);

	ani = new CAnimation(1000);//cửa đang mở
	ani->Add(40032);
	animations->Add(815, ani);

	ani = new CAnimation(500);// đóng cửa
	ani->Add(40032);
	ani->Add(40031);
	animations->Add(816, ani);

	ani = new CAnimation(0);//tóe nước người cá
	ani->Add(40033);
	animations->Add(817, ani);

	
	ani = new CAnimation(400);//enemy xương trái
	ani->Add(40034);
	ani->Add(40035);
	animations->Add(818, ani);

	ani = new CAnimation(400);//enemy xương phải
	ani->Add(40042);
	ani->Add(40043);
	animations->Add(819, ani);

	ani = new CAnimation(100);//vũ khí xương trái
	ani->Add(40036);
	ani->Add(40037);
	animations->Add(820, ani);

	ani = new CAnimation(100);//vũ khí xương phải
	ani->Add(40044);
	ani->Add(40045);
	animations->Add(821, ani);
	
	ani = new CAnimation(250);//chim bay trái
	ani->Add(40038);
	ani->Add(40039);
	animations->Add(822, ani);

	ani = new CAnimation(250);//chim bay phải
	ani->Add(40046);
	ani->Add(40047);
	animations->Add(823, ani);

	ani = new CAnimation(100);//gù trái đứng yên
	ani->Add(40041);
	animations->Add(824, ani);

	ani = new CAnimation(100);//gù trái nhảy
	ani->Add(40040);
	animations->Add(8240, ani);

	ani = new CAnimation(100);//gù phải đứng yên
	ani->Add(40049);
	animations->Add(825, ani);

	ani = new CAnimation(100);//gù phải nhảy
	ani->Add(40048);
	animations->Add(8250, ani);
	 
	ani = new CAnimation(100);//đầu của dracula lv1
	ani->Add(40054);
	animations->Add(826, ani);

	ani = new CAnimation(100);//dracula lv1 đứng im trái
	ani->Add(40050);
	animations->Add(8270, ani);

	ani = new CAnimation(100);//dracula lv1 đánh trái
	ani->Add(40051);
	animations->Add(827, ani);

	ani = new CAnimation(100);//dracula lv1 bị đánh trái (mất đầu)
	ani->Add(40053);	
	animations->Add(828, ani);

	ani = new CAnimation(100);//dracula lv1 đứng im phải
	ani->Add(40059);
	animations->Add(8290, ani);

	ani = new CAnimation(100);//dracula lv1 đánh phải
	ani->Add(40060);
	animations->Add(829, ani);

	ani = new CAnimation(100);//dracula lv1 bị đánh phải (mất đầu)
	ani->Add(40062);
	animations->Add(830, ani);
	
	
	ani = new CAnimation(100);//dracula lv2 đứng im trái 
	ani->Add(40055);
	animations->Add(831, ani);

	ani = new CAnimation(100);//dracula lv2 chuẩn bị bay trái 
	ani->Add(40056);
	animations->Add(832, ani);

	ani = new CAnimation(100);//dracula lv2 bay trái 
	ani->Add(40057);
	animations->Add(833, ani);

	ani = new CAnimation(100);//dracula lv2 trên không trái 
	ani->Add(40058);
	animations->Add(834, ani);

	ani = new CAnimation(100);//dracula lv2 đứng im phải 
	ani->Add(40064);
	animations->Add(835, ani);

	ani = new CAnimation(100);//dracula lv2 chuẩn bị bay phải 
	ani->Add(40065);
	animations->Add(836, ani);

	ani = new CAnimation(100);//dracula lv2 bay phải 
	ani->Add(40066);
	animations->Add(837, ani);

	ani = new CAnimation(100);//dracula lv2 trên không phải 
	ani->Add(40067);
	animations->Add(838, ani);
	
	ani = new CAnimation(100);
	ani->Add(40068);
	animations->Add(839, ani);
	#pragma endregion*/

	
	#pragma region simon

	simon = new Simon();
	simon->AddAnimation(400);	// đứng phải	
	simon->AddAnimation(401);	//đứng trái	
	simon->AddAnimation(500);	// đi phải
	simon->AddAnimation(501);	//đi trái
	simon->AddAnimation(402);	//đánh phải 
	simon->AddAnimation(403);	//đánh trái
	simon->AddAnimation(404);	//ngồi phải
	simon->AddAnimation(405);	//ngồi trái
	simon->AddAnimation(406);	//nhảy phải 
	simon->AddAnimation(407);	//nhảy trái
	simon->AddAnimation(410);	//ngồi đánh phải 
	simon->AddAnimation(411);	//ngồi đánh trái
	simon->AddAnimation(412);	//trên thang phải đánh phải
	simon->AddAnimation(413);	//trên thang phải đánh trái
	simon->AddAnimation(414);	//trên thang trái đánh phải
	simon->AddAnimation(415);	//trên thang trái đánh trái
	simon->AddAnimation(416);	//lên thang phải
	simon->AddAnimation(417);	//xuống thang phải
	simon->AddAnimation(418);	//lên thang trái
	simon->AddAnimation(419);	//xuống thang trái
	simon->AddAnimation(420);	//đứng im bên phải thang phải
	simon->AddAnimation(421);	//đứng im bên phải thang trái
	simon->AddAnimation(422);	//đứng im bên trái thang phải
	simon->AddAnimation(423);	//đứng im bên trái thang trái
	simon->AddAnimation(424);	//bị đánh từ bên trái
	simon->AddAnimation(425);	//bị đánh từ bên phải
	simon->AddAnimation(599);	//chết

	simon->whip->AddAnimation(426);//roi lv0 phải
	simon->whip->AddAnimation(427);//roi lv0 trái
	simon->whip->AddAnimation(428);//roi lv1 phải
	simon->whip->AddAnimation(429);//roi lv1 trái
	simon->whip->AddAnimation(408);//roi lv2 phải
	simon->whip->AddAnimation(409);//roi lv2 trái

#pragma endregion

	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	
	ui = new UI();
	ui->Initialize(d3ddv, simon);

	// Load map lv1 ra trước
	LoadResourceLv1();
	
}

void Update(DWORD dt)
{

#pragma region Resource

	/**
	 * Kiểm tra để load resource tương ứng
	 */
	float x, y;
	simon->GetPosition(x, y);
	if (!simon->isLevelUp)
		gameTime -= dt;

	if (lv == 1)
	{
		// Lên cấp
		if (simon->isLevelUp) {
			isEnableKeyBoard = false;
			simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump)
				simon->vx = SIMON_LEVELUP_SPEED;
			DWORD timer = GetTickCount();
			if (timer - simon->onCheckPointTime > LEVELUP_TIME)
			{
				listGrids->ReleaseList();

				lv = 2;
				checkload = false;
				simon->isLevelUp = false;
				simon->SetState(SIMON_STATE_IDLE);
				isEnableKeyBoard = true;
			}
		}
	}
	
	if (lv == 2)
	{
		static int count1 = 0;
		
		// Lần đầu load resource lv2
		if (countLoadResource2 == false)
		{
			game->x_cam = 0;
			listGrids->InitList(MAX_WIDTH_LV2);
			LoadResourceLv2();
			countLoadResource2 = true;
			simon->SetPosition(50, 350);
			simon->GetPosition(x, y);
			timer = GetTickCount();
		}
		else if (countLoadResource2 == true && x < MAX_WIDTH_LV2 - 2* SIMON_STAND_BBOX_WIDTH)
		{		
			if (GetTickCount() - timer > 5000)
			{
				simon->GetPosition(x, y);
				if (x < MAX_WIDTH_LV2 - SCREEN_WIDTH/2 && x >  SCREEN_WIDTH / 2 )
				{
					listGrids->AddObject("Castlevania\\resource\\Zombie.txt", x, count1, lv, simon);
					count1++;
					timer += 5000;
				}
				

			}
		}
		else if(x > MAX_WIDTH_LV2 - 2 * SIMON_STAND_BBOX_WIDTH && y<200)  //check point
		{
			listGrids->ReleaseList();

			lv = 21;
		}
	}
	if (lv == 21)
	{
		// Lần đầu load resource lv2_1
		
		if (countLoadResource2_1 == false )
		{
			listGrids->InitList(MAX_WIDTH_LV2_1);
			LoadResourceLv2_1();
			countLoadResource2_1 = true;
			
		}		
		if (x > MAX_WIDTH_LV2_1 - 2 * SIMON_STAND_BBOX_WIDTH && y < 200)
		{
			listGrids->ReleaseList();
			lv = 99;						
		}
		if (y > 420)
		{
			lv = 22;
			checkload = false;
			temp = true;
			
		}
	}
	if (lv == 22)
	{
		if (countLoadResource2_2 == false)
		{
			//game->x_cam = 0;
			listGrids->InitList(MAX_WIDTH_LV2_2);
			LoadResourceLv2_2();
			countLoadResource2_2 = true;			
			simon->SetPosition(150, 120);
			simon->GetPosition(x, y);
			timer = GetTickCount();
		}
		if (countLoadResource2_2 == true && temp == true)
		{
			
			if (x < 3600)
			{
				game->x_cam = 0;
				simon->SetPosition(150, 120);
				simon->GetPosition(x, y);

			}
			else 
			{
				game->x_cam = MAX_WIDTH_LV2_2 - SCREEN_WIDTH;
				simon->SetPosition(778, 120);
				simon->GetPosition(x, y);
			}
			
			temp = false;
		}
		if (countLoadResource2_2 == true)
		{
			if (GetTickCount() - timer > 5000)
			{				
				listGrids->AddObject("Castlevania\\resource\\MerMan.txt",game->x_cam,simon->x);
				timer = GetTickCount();
			}
		}
		if ((y < 72 && x<100) || (y<72 &&x >400))
		{
			
			lv = 21;
			checkload = false;
			if (x < 400)
			{
				simon->SetPosition(3198, 419);
				simon->GetPosition(x, y);
				game->x_cam = MAX_WIDTH_LV2;
			}
			else
			{
				simon->SetPosition(3835, 419);
				simon->GetPosition(x, y);
				game->x_cam = MAX_WIDTH_LV2_1 - SCREEN_WIDTH;
			}
		}
		
	}
	#pragma region lv99
	if (lv == 99)
	{
		static int count = 0;
		if (countLoadResourceboss == false)
		{
			listGrids->InitList(MAX_WIDTH_BOSS);
			LoadResourceboss();
			countLoadResourceboss = true;
			timer = GetTickCount();
		}
		else if (countLoadResourceboss == true)
		{
			if (GetTickCount() - timer > 5000)
			{
				simon->GetPosition(x, y);
				if (x < 5000 && x> MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2 && count_enemy == true)
				{
					listGrids->AddObject("Castlevania\\resource\\Zombie.txt", x, count, lv, simon);
					count++;
					timer += 5000;
				}
				else if (x > 5000)
				{
					count_enemy = false;
				}				
			}
		}

		
	}
#pragma endregion

	
	if (lv == 35)
	{
		if (countLoadResource3_5 == false)
		{
			listGrids->ReleaseList();
			listGrids->InitList(MAX_WIDTH_LV3_5);
			LoadResourceLv3_5();
			countLoadResource3_5 = true;			
		}
		if (simon->isLevelUp) 
		{
			isEnableKeyBoard = false;
			simon->SetState(SIMON_STATE_WALK);
			if (!simon->isJump)
				simon->vx = SIMON_LEVELUP_SPEED;
			DWORD timer = GetTickCount();
			if (timer - simon->onCheckPointTime > LEVELUP_TIME)
			{
				listGrids->ReleaseList();
				lv = 34;
				checkload = false;
				simon->SetPosition(1455, 200);
				simon->isLevelUp = false;
				simon->SetState(SIMON_STATE_IDLE);
				isEnableKeyBoard = true;
			}
		}
	}
	if (lv == 34)
	{
		static int count34 = 0;
		static int count_enemy34 = 0;
		if (countLoadResource3_4 == false )
		{
			listGrids->InitList(MAX_WIDTH_LV3_4);
			LoadResourceLv3_4();
			countLoadResource3_4 = true;
			

		}
		else if (countLoadResource3_4 == true)
		{
			if (check_enemy_lv34 == false)
			{
				timer = GetTickCount();
				check_enemy_lv34 = true;
			}
			if (GetTickCount() - timer > 5000)
			{
				simon->GetPosition(x, y);
				
				if (x < 750 && x >100)
				{
					if (count_enemy34 < 3)
					{
						listGrids->AddObject("Castlevania\\resource\\Bird_Hunchback.txt", x, count34,lv,simon);						
					}
					count_enemy34++;
					count34++;
					timer += 5000;
				}																					
			}
		}
		if (((y < 85 && x> 1340) || ( y< 85 && x>664  && x<690) || (y < 85 && x>200 && x<210)) && simon->isOnStair == true )
		{
			listGrids->ReleaseList();
			listGrids->InitList(MAX_WIDTH_LV3_3);
			LoadResourceLv3_3();
			lv = 33;
			checkload = false;

			if (x > 1340)
			{
				simon->SetPosition(1312, 423);
				simon->GetPosition(x, y);
			}
			else if (x > 664 && x < 690)
			{
				simon->SetPosition(620, 423);
				simon->GetPosition(x, y);
			}
			else if (x > 200 && x < 210)
			{
				simon->SetPosition(165, 423);
				simon->GetPosition(x, y);
			}
			//temp10 = true;
		}
		

	}
	if (lv == 33)
	{
		static int count33 = 0;
		static int count_enemy33 = 0;		
		bool prevent_load_twice ;
		if (check_enemy_lv33 == false)
		{
			timer = GetTickCount();
			check_enemy_lv33 = true;
		}
		if (GetTickCount() - timer > 8000)
		{
			simon->GetPosition(x, y);
			if (x < 750 && x >100)
			{
				if (count_enemy33 < 3)
				{
					listGrids->AddObject("Castlevania\\resource\\Bird_Hunchback.txt",x,count33,lv,simon);
				}
					/*if (count33 % 2 == 0)
					{
						Bird *bird;
						bird = new Bird(1);
						bird->AddAnimation(822);
						bird->AddAnimation(823);
						bird->SetPosition(x - SCREEN_WIDTH / 2, 140 + rand() % ( SCREEN_HEIGHT/2 - 139));
						listGrids->AddObject(bird);

						Hunchback *hunchback;
						hunchback = new Hunchback(bird);
						hunchback->AddAnimation(824);
						hunchback->AddAnimation(825);
						hunchback->AddAnimation(8240);
						hunchback->AddAnimation(8250);
						listGrids->AddObject(hunchback);

						count33++;
					}
					else if (count33 % 2 != 0)
					{
						Bird *bird;
						bird = new Bird(-1);
						bird->AddAnimation(822);
						bird->AddAnimation(823);
						bird->SetPosition(x + SCREEN_WIDTH / 2, 140 + rand() % ( SCREEN_HEIGHT / 2 - 139));
						listGrids->AddObject(bird);

						Hunchback *hunchback;
						hunchback = new Hunchback(bird);
						hunchback->AddAnimation(824);
						hunchback->AddAnimation(825);
						hunchback->AddAnimation(8240);
						hunchback->AddAnimation(8250);
						listGrids->AddObject(hunchback);
						count33++;
					}*/
				count_enemy33++;
				count33++;
				timer += 8000;
			}
		}
		if (x < 0)
		{
			listGrids->ReleaseList();			
			lv = 32;
			checkload = false;
			simon->SetPosition(500, 200);
			simon->GetPosition(x, y);			
		}
		if ((y > 430 && x< 190) || (y > 430 && x>570 && x < 600) || (y > 430 && x>1280 && x < 1300) && simon->isOnStair == true)
		{
			listGrids->ReleaseList();
			listGrids->InitList(MAX_WIDTH_LV3_4);
			LoadResourceLv3_4();
			lv = 34;
			checkload = false;
			if (x <190 && x>0)
			{
				simon->SetPosition(140, 120);
				simon->GetPosition(x, y);
			}
			else if (x > 570 && x < 600)
			{
				simon->SetPosition(600,120);
				simon->GetPosition(x, y);
			}
			else if (x > 1280 && x < 1300)
			{
				simon->SetPosition(1300,120);
				simon->GetPosition(x, y);
			}
			//temp10 = true;
			
		}
		else if (y > 480 && simon->isOnStair == false)
		{
			listGrids->ReleaseList();
			listGrids->InitList(MAX_WIDTH_LV3_4);
			LoadResourceLv3_4();
			lv = 34;
			checkload = false;
			simon->SetPosition(x, 130);
			simon->GetPosition(x, y);
		}
	}
	if (lv == 32)
	{
		if (countLoadResource3_2 == false)
		{
			listGrids->InitList(MAX_WIDTH_LV3_2);
			LoadResourceLv3_2();
			countLoadResource3_2 = true;

		}
		if (y < 90)
		{
			listGrids->ReleaseList();
			lv = 31;
			checkload = false;
			simon->SetPosition(1200, 410);
			game->x_cam = MAX_WIDTH_LV3_1 - SCREEN_WIDTH / 2;
			simon->GetPosition(x, y);
			listGrids->InitList(MAX_WIDTH_LV3_1);
			LoadResourceLv3_1();
		}
	}
	if (lv == 31)
	{
		/*if (countLoadResource3_1 == false )
		{
			listGrids->InitList(MAX_WIDTH_LV3_1);
			LoadResourceLv3_1();
			countLoadResource3_1 = true;
		}*/
		if (y >430)
		{
			listGrids->ReleaseList();
			listGrids->InitList(MAX_WIDTH_LV3_2);
			LoadResourceLv3_2();
			lv = 32;
			checkload = false;
			simon->SetPosition(249, 120);		
			simon->GetPosition(x, y);
			
		}
	}
#pragma endregion

#pragma region Update Object

	vector<LPGAMEOBJECT> objects;

	if (simon->x < 0)
		currentGrids = listGrids->GetCurrentGrids(0);
	else
		currentGrids = listGrids->GetCurrentGrids(simon->x);

	for (int i = 0; i < currentGrids.size(); i++)
	{
		vector<LPGAMEOBJECT> listObjects = currentGrids[i]->GetListObject();
		int listObjectSize = listObjects.size();
		for (int j = 0; j < listObjectSize; j++)
		{
			objects.push_back(listObjects[j]);
		}
	}

	simon->Update(lv,dt, &objects);

	if (simon->isRosaryUsed)
		isEnableKeyBoard = false;

	if (isClockWeaponUsed)
	{
		if (GetTickCount() - clockWeaponCast > CLOCK_WEAPON_FREEZE_TIME)
		{
			isClockWeaponUsed = false;
		}
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<Enemy *>(objects.at(i)))
			objects[i]->Update(dt, &objects);

		if (dynamic_cast<Enemy *>(objects.at(i)))
		{
			enemy = dynamic_cast<Enemy *>(objects.at(i));

			if (simon->isRosaryUsed)
			{
				if (!dynamic_cast<BossBat *>(objects.at(i)) && !dynamic_cast<Dracula *>(objects.at(i)) && !dynamic_cast<SuperDracula *>(objects.at(i)))
				{
					enemy->isDie = true;
					Simon::score += 100;
				}
			}

			if (isClockWeaponUsed)
			{
				enemy->isFrozen = true;
			}
			else
			{
				enemy->isFrozen = false;

				if (dynamic_cast<Panther *>(objects.at(i)))
				{
					Panther *panther = dynamic_cast<Panther *>(objects.at(i));
					if (panther->isActivate && ((panther->x < (simon->x - 2 * SCREEN_WIDTH / 3)) || (panther->x > (simon->x + 2 * SCREEN_WIDTH / 3))))
					{
						panther->isDie = true;
					}
					else if (!panther->isUse)
					{
						if ((simon->y - panther->y < PANTHER_DISTANCE_SIMON_Y && panther->x - simon->x < PANTHER_DISTANCE_SIMON_X))
						{
							panther->SetState(PANTHER_STATE_RUN);
							panther->isActivate = true;
						}

						if (panther->isActivate &&
							(panther->x >= PANTHER_JUMP_POINT_1_X - 3 && panther->x <= PANTHER_JUMP_POINT_1_X + 10) ||
							(panther->x >= PANTHER_JUMP_POINT_2_X - 3 && panther->x <= PANTHER_JUMP_POINT_2_X + 10) ||
							(panther->x >= PANTHER_JUMP_POINT_3_X - 3 && panther->x <= PANTHER_JUMP_POINT_3_X + 10))
						{
							panther->SetState(PANTHER_STATE_JUMP);
						}
					}
				}
				else if (dynamic_cast<MerMan *>(objects.at(i)))
				{
					MerMan *merman = dynamic_cast<MerMan *>(objects.at(i));

					if (merman->isOnGround)
					{
						if (merman->y > game->y_cam + SCREEN_HEIGHT / 2)
						{
							if (merman->y > game->y_cam + SCREEN_HEIGHT)
								merman->isDie = true;
							else
							{
								merman->isAttack = false;
								merman->SetState(MERMAN_STATE_WALK);
							}
						}
						else
						{
							if (merman->isAttack && !merman->didAttack)
							{
								int nx = merman->nx;
								enemy = new EnemyBullet(nx);
								enemy->AddAnimation("Castlevania\\resource\\EnemyBullet.txt");
								enemy->SetPosition(merman->x + MERMAN_BBOX_WIDTH / 2, merman->y + 10);
								listGrids->AddObject(enemy);
								merman->didAttack = true;
							}
						}
					}

					if (merman->x < game->x_cam - MERMAN_BBOX_WIDTH || merman->x > game->x_cam + SCREEN_WIDTH)
						merman->isDie = true;
				}
				else if (dynamic_cast<BigBat *>(objects.at(i)))
				{
					BigBat *bigbat = dynamic_cast<BigBat *>(objects.at(i));
					int nx = bigbat->simon->nx;
					if (bigbat->isActivate)
					{						
						if (GetTickCount() - bigbat->timer > 3000 && bigbat->timer >0)
						{
							enemy = new EnemyBullet(nx);
							enemy->AddAnimation("Castlevania\\resource\\EnemyBullet.txt");
							enemy->SetPosition(bigbat->x + BOSSBAT_BBOX_WIDTH / 2, bigbat->y);
							listGrids->AddObject(enemy);
							bigbat->timer += 3000;
						}
					}
					
				}
				else if (dynamic_cast<Skeleton *>(objects.at(i)))
				{
					Skeleton *skeleton = dynamic_cast<Skeleton *>(objects.at(i));
					int nx = skeleton->nx;
					if (GetTickCount() - skeleton->timer > 2000)
					{
						enemy = new Bone(nx);
						enemy->AddAnimation(820);
						enemy->AddAnimation(821);
						enemy->SetPosition(skeleton->x, skeleton->y - 10);
						listGrids->AddObject(enemy);
						skeleton->timer += 2000;
					}
				}
				else if (dynamic_cast<Dracula *>(objects.at(i)))
				{
					Dracula *dracula = dynamic_cast<Dracula *>(objects.at(i));

					if (!dracula->isActivate)
					{
						if (dracula->isActivating)
						{
							isEnableKeyBoard = false;
							simon->SetState(SIMON_STATE_IDLE);
						}
					}
					else
					{
						isEnableKeyBoard = true;
					}

					if (dracula->state == DRACULA_STATE_ATTACK && !dracula->didAttack)
					{
						int nx = dracula->nx;
						float root_bullet_vy;

						enemy = new EnemyBullet(nx); 
						enemy->AddAnimation("Castlevania\\resource\\EnemyBullet.txt");
						enemy->SetPosition(dracula->x + DRACULA_BBOX_WIDTH / 2, dracula->y + 55);
						if (enemy->vx > 0)
							enemy->vx += 0.002f;
						if (enemy->vx < 0)
							enemy->vx -= 0.002f;
						float angle = atan2(simon->x - enemy->x, simon->y - enemy->y);
						enemy->vy = enemy->vx / tan(angle);

						if (enemy->vy > 0.1f)
							enemy->vy = 0.1f;
						if (enemy->vy < -0.1f)
							enemy->vy = -0.1f;

						root_bullet_vy = enemy->vy;

						listGrids->AddObject(enemy);

						enemy = new EnemyBullet(nx);
						enemy->AddAnimation("Castlevania\\resource\\EnemyBullet.txt");
						enemy->vy = root_bullet_vy + 0.04f;
						enemy->SetPosition(dracula->x + DRACULA_BBOX_WIDTH / 2, dracula->y + 55);
						listGrids->AddObject(enemy);

						enemy = new EnemyBullet(nx);
						enemy->AddAnimation("Castlevania\\resource\\EnemyBullet.txt");
						enemy->vy = root_bullet_vy - 0.04f;
						enemy->SetPosition(dracula->x + DRACULA_BBOX_WIDTH / 2, dracula->y + 55);
						listGrids->AddObject(enemy);

						dracula->didAttack = true;
					}

					if (dracula->isHit && !dracula->isBleeding)
					{
						// Thêm hiệu ứng tóe lửa
						whipEffect = new Effect(GetTickCount());
						whipEffect->AddAnimation(806);
						whipEffect->SetPosition(dracula->x + DRACULA_BBOX_WIDTH / 2, dracula->y);
						objects.push_back(whipEffect);
						listGrids->AddObject(whipEffect);

						whipEffect = new Effect(GetTickCount());
						whipEffect->AddAnimation(807);
						whipEffect->SetPosition(dracula->x + DRACULA_BBOX_WIDTH / 2, dracula->y);
						objects.push_back(whipEffect);
						listGrids->AddObject(whipEffect);

						dracula->isBleeding = true;
					}
					if (dracula->isDie)
					{						
						listGrids->AddObject("Castlevania\\resource\\SuperDracula.txt",dracula->x,dracula->y,simon);
					}
				}
				else if (dynamic_cast<SuperDracula *>(objects.at(i)))
				{
					SuperDracula *superDracula = dynamic_cast<SuperDracula *>(objects.at(i));

					if (superDracula->isHitted && !superDracula->isBleeding)
					{
						for (int i = 0; i < 3; i++)
						{
							// Thêm hiệu ứng tóe lửa
							whipEffect = new Effect(GetTickCount());
							whipEffect->AddAnimation(806);
							whipEffect->SetPosition(superDracula->x + i * SUPERDRACULA_BBOX_WIDTH / 3, superDracula->y + 3 * SUPERDRACULA_BBOX_HEIGHT / 4);
							objects.push_back(whipEffect);
							listGrids->AddObject(whipEffect);

							whipEffect = new Effect(GetTickCount());
							whipEffect->AddAnimation(807);
							whipEffect->SetPosition(superDracula->x + i * SUPERDRACULA_BBOX_WIDTH / 3, superDracula->y + 3 * SUPERDRACULA_BBOX_HEIGHT / 4);
							objects.push_back(whipEffect);
							listGrids->AddObject(whipEffect);
						}

						superDracula->isBleeding = true;
					}
					
				}
				else if (dynamic_cast<BossBat *>(objects.at(i)))
				{
				BossBat *bossbat = dynamic_cast<BossBat *>(objects.at(i));

				if (bossbat->isHurt && !bossbat->isBleeding)
				{
					// Thêm hiệu ứng tóe lửa
					whipEffect = new Effect(GetTickCount());
					whipEffect->AddAnimation(806);
					whipEffect->SetPosition(bossbat->x + BOSSBAT_BBOX_WIDTH / 2, bossbat->y + BOSSBAT_BBOX_HEIGHT / 4);
					objects.push_back(whipEffect);
					listGrids->AddObject(whipEffect);

					whipEffect = new Effect(GetTickCount());
					whipEffect->AddAnimation(807);
					whipEffect->SetPosition(bossbat->x + BOSSBAT_BBOX_WIDTH / 2, bossbat->y + BOSSBAT_BBOX_HEIGHT / 4);
					objects.push_back(whipEffect);
					listGrids->AddObject(whipEffect);

					bossbat->isBleeding = true;
				}

				if (bossbat->isDie)
				{
					item = new Item();
					item->SetPosition(bossbat->x, bossbat->y);
					item->SetSpeed(0, -0.1);
					item->appearTime = GetTickCount();
					item->AddAnimation(ITEM_PRIZE);
					item->SetType(ITEM_PRIZE);
					objects.push_back(item);
					listGrids->AddObject(item);
					lv = 35;
					checkload = false;
				}
				}

				enemy->Update(dt, &objects);
			}
		}

		listGrids->UpdateObjectInGrid(objects[i]);
	}
	
#pragma endregion

#pragma region Remove Object
	/**
	 * Không remove trực tiếp trong mảng khi đang duyệt, thêm vào danh sách các object sẽ bị remove và sau đó mới bắt đầu remove
	 */
	vector<LPGAMEOBJECT> listRemoveObjects;

	// Thêm các object sẽ bị remove vào listRemoveObjects và cập nhật thêm các object mới vào objects và listGrids
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<Enemy *>(objects.at(i)))
		{
			enemy = dynamic_cast<Enemy *>(objects.at(i));

			if (enemy->isDie)
			{
				float object_x, object_y, object_right, object_bottom;
				enemy->GetBoundingBox(object_x, object_y, object_right, object_bottom);

				if (dynamic_cast<Dracula *>(objects.at(i)) || dynamic_cast<BossBat *>(objects.at(i)) || dynamic_cast<SuperDracula *>(objects.at(i)))
				{
					if (dynamic_cast<Dracula *>(objects.at(i)))
					{
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 4; j++)
							{
								// Thêm hiệu ứng tóe lửa
								whipEffect = new Effect(GetTickCount());
								whipEffect->AddAnimation(806);
								whipEffect->SetPosition(object_x + i * DRACULA_BBOX_WIDTH, object_y + j * DRACULA_BBOX_HEIGHT);
								objects.push_back(whipEffect);
								listGrids->AddObject(whipEffect);

								whipEffect = new Effect(GetTickCount());
								whipEffect->AddAnimation(807);
								whipEffect->SetPosition(object_x + i * DRACULA_BBOX_WIDTH, object_y + j * DRACULA_BBOX_HEIGHT);
								objects.push_back(whipEffect);
								listGrids->AddObject(whipEffect);
							}
						}
					}
					else
					{
						for (int i = 0; i < 3; i++)
						{
							for (int j = 0; j < 4; j++)
							{
								// Thêm hiệu ứng tóe lửa
								whipEffect = new Effect(GetTickCount());
								whipEffect->AddAnimation(806);
								whipEffect->SetPosition(object_x + i * (object_right - object_x) / 3, object_y + j * (object_bottom - object_y) / 4);
								objects.push_back(whipEffect);
								listGrids->AddObject(whipEffect);

								whipEffect = new Effect(GetTickCount());
								whipEffect->AddAnimation(807);
								whipEffect->SetPosition(object_x + i * (object_right - object_x) / 3, object_y + j * (object_bottom - object_y) / 4);
								objects.push_back(whipEffect);
								listGrids->AddObject(whipEffect);
							}
						}
					}

					item = new Item();
					item->SetPosition(object_x, object_y);
					item->SetSpeed(0, -0.1);
					item->appearTime = GetTickCount();
					item->AddAnimation(ITEM_PRIZE);
					item->SetType(ITEM_PRIZE);
					objects.push_back(item);
					listGrids->AddObject(item);
				}
				else
				{
					// Thêm hiệu ứng tóe lửa
					whipEffect = new Effect(GetTickCount());
					whipEffect->AddAnimation(806);
					whipEffect->SetPosition(object_x, object_y + (object_bottom - object_y) / 4);
					objects.push_back(whipEffect);
					listGrids->AddObject(whipEffect);

					whipEffect = new Effect(GetTickCount());
					whipEffect->AddAnimation(807);
					whipEffect->SetPosition(object_x, object_y + (object_bottom - object_y) / 4);
					objects.push_back(whipEffect);
					listGrids->AddObject(whipEffect);
				}

				listRemoveObjects.push_back(enemy);
			}
		}
		else if (dynamic_cast<BigFire *>(objects.at(i)) || dynamic_cast<Candle *>(objects.at(i)))
		{
			bool isHitted = false;
			float object_x, object_y, object_right, object_bottom;

			if (dynamic_cast<BigFire *>(objects.at(i)))
			{
				BigFire *bigfire = dynamic_cast<BigFire *>(objects.at(i));
				bigfire->GetBoundingBox(object_x, object_y, object_right, object_bottom);

				isHitted = bigfire->isHitted;
				if (isHitted)
					listRemoveObjects.push_back(bigfire);
			}
			else if (dynamic_cast<Candle *>(objects.at(i)))
			{
				Candle *candle = dynamic_cast<Candle *>(objects.at(i));
				candle->GetBoundingBox(object_x, object_y, object_right, object_bottom);

				isHitted = candle->isHitted;
				if (isHitted)
					listRemoveObjects.push_back(candle);
			}

			if (isHitted)
			{
				item = new Item();
				item->SetPosition(object_x, object_y);
				item->SetSpeed(0, -0.1);
				item->appearTime = GetTickCount();
				objects.push_back(item);
				listGrids->AddObject(item);

				// Whip item
				if (simon->whip->level < 2)
				{
					item->AddAnimation(ITEM_WHIPITEM);
					item->SetType(ITEM_WHIPITEM);
				}
				else
				{
					/**
					 * Random ra item: (do hiện tại chỉ có 3 món này)
					 * 90% heart
					 * 4% money
					 * 1% knife
					 * 1% axe
					 * 1% holy water
					 * 1% cross
					 * 1% rosary
					 * 1% clock
					 */

					srand(time(NULL));
					int random_portion = rand() % 100;

					// Heart
					/*if (random_portion < 10) // 90
					{
						item->AddAnimation(ITEM_HEART);
						item->SetType(ITEM_HEART);
						
					}
					// Money
					else if (random_portion >= 10 && random_portion < 20) // 90 94
					{
						item->AddAnimation(ITEM_HOLYWATER);
						item->SetType(ITEM_HOLYWATER);
					}
					// Knife
					else if(random_portion >= 20 && random_portion < 30) // 94 95
					{
						item->AddAnimation(ITEM_KNIFE);
						item->SetType(ITEM_KNIFE);
					}
					// Axe
					else if(random_portion >= 30 && random_portion < 40)
					{
						item->AddAnimation(ITEM_AXE);
						item->SetType(ITEM_AXE);
					}
					// Holy water
					else if(random_portion >= 40 && random_portion < 50)
					{
						item->AddAnimation(ITEM_HOLYWATER);
						item->SetType(ITEM_HOLYWATER);
					}
					// Cross
					else if(random_portion >= 50 && random_portion < 70)
					{
						item->AddAnimation(ITEM_CROSS);
						item->SetType(ITEM_CROSS);
					}
					// Rosary
					else if(random_portion >= 70 && random_portion < 80)
					{
						item->AddAnimation(ITEM_ROSARY);
						item->SetType(ITEM_ROSARY);
					}
					// Clock
					else if(random_portion >= 80 && random_portion < 100)
					{
						item->AddAnimation(ITEM_CLOCK);
						item->SetType(ITEM_CLOCK);
					}*/
					if (random_portion >= 0 && random_portion <= 100)
					{
						
						item->AddAnimation(ITEM_INVICIBILITY);
						item->SetType(ITEM_INVICIBILITY);
					}
				}

				// Thêm hiệu ứng tóe lửa
				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(806);
				whipEffect->SetPosition(object_x, object_y + (object_bottom - object_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);

				whipEffect = new Effect(GetTickCount());
				whipEffect->AddAnimation(807);
				whipEffect->SetPosition(object_x, object_y + (object_bottom - object_y) / 4);
				objects.push_back(whipEffect);
				listGrids->AddObject(whipEffect);
			}
		}
		else if (dynamic_cast<Item *>(objects.at(i)))
		{
			Item *item = dynamic_cast<Item *>(objects.at(i));

			if (item->GetEaten() || GetTickCount() - item->appearTime > ITEM_LIVE_TIME)
			{
				listRemoveObjects.push_back(item);
			}
		}
		else if (dynamic_cast<Weapon *>(objects.at(i)))
		{
			Weapon *weapon = dynamic_cast<Weapon *>(objects.at(i));

			if (weapon->isExposed)
			{
				listRemoveObjects.push_back(weapon);
			}
		}
		else if (dynamic_cast<Effect *>(objects.at(i)))
		{
			Effect *effect = dynamic_cast<Effect *>(objects.at(i));

			if (effect->GetExposed())
			{
				listRemoveObjects.push_back(effect);
			}
		}
	}

	// Remove lần lượt từng object từ listRemoveObjects trong listGrids
	for (int i = 0; i < listRemoveObjects.size(); i++)
	{
		listGrids->RemoveObject(listRemoveObjects[i]);
		delete listRemoveObjects[i];
	}
#pragma endregion

#pragma region Camera
	if (lv == 1)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			
		}
		else if (x > MAX_WIDTH_LV1 - SCREEN_WIDTH / 2)
		{
			game->x_cam = MAX_WIDTH_LV1 - SCREEN_WIDTH;
			
		}
		else
		{
			game->x_cam = 0;
			
		}
	}
	else if (lv == 2)
	{
		if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;
			
		}
		else if (x > MAX_WIDTH_LV2 - SCREEN_WIDTH / 2) {
			game->x_cam = MAX_WIDTH_LV2 - SCREEN_WIDTH;
			
		}
		else if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
			
		}
	}
	else if (lv == 21)
	{
		// chuyen scene
		
		if (game->x_cam < MAX_WIDTH_LV2 - SCREEN_WIDTH/2)
		{
			game->x_cam += SIMON_WALKING_SPEED * dt;
			simon->SetState(SIMON_STATE_IDLE);
			isEnableKeyBoard = false;
			
		}
		else 
		{
			if (check_open_door_time == false)
			{
				open_door_time = GetTickCount();
				check_open_door_time = true;
			}
			
			if (GetTickCount() - open_door_time > 1800)
			{
				if (x < 3200)
				{
					if (check1 == false)
						simon->SetState(SIMON_STATE_WALK);
				}
				else if (x > 3200 && x<3210)
				{
					check1 = true;
					check = true;
					simon->SetState(SIMON_STATE_IDLE);
					if (check_close_door_time == false)
					{
						close_door_time = GetTickCount();
						check_close_door_time = true;
					}
					
				}
			}
		}
				
		if (game->x_cam <= MAX_WIDTH_LV2 && check == true)
		{				
			if (GetTickCount() - close_door_time > 1800)
			game->x_cam += SIMON_WALKING_SPEED * dt;

		}
		else if (game->x_cam > MAX_WIDTH_LV2)
		{
			checkScene = true;
			check_close_door_time = false;
			check_open_door_time = false;
			
		}
		
		//
		//trả camera về simon
		if (checkScene == true)
		{
			if (x > MAX_WIDTH_LV2 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
			{
				game->x_cam = x - SCREEN_WIDTH / 2;
				
			}
			else if (x > MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2) 
			{
				game->x_cam = MAX_WIDTH_LV2_1 - SCREEN_WIDTH;
				
			}
			else if (x < MAX_WIDTH_LV2 + SCREEN_WIDTH / 2)
			{
				game->x_cam = MAX_WIDTH_LV2;
				
			}
			isEnableKeyBoard = true;
		}
		
	}
	else if (lv == 22)
	{
		if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
		}
		else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;

		}
		else if (x > MAX_WIDTH_LV2_2 - SCREEN_WIDTH / 2)
			game->x_cam = MAX_WIDTH_LV2_2 - SCREEN_WIDTH;
	}
	else if (lv == 99)
	{
		if (game->x_cam < MAX_WIDTH_LV2_1 - SCREEN_WIDTH / 2)
		{
			game->x_cam += SIMON_WALKING_SPEED * dt;
			simon->SetState(SIMON_STATE_IDLE);
			isEnableKeyBoard = false;
			
		}
		else 
		{
			if (check_open_door_time == false)
			{
				open_door_time = GetTickCount();
				check_open_door_time = true;
			}

			if (GetTickCount() - open_door_time > 1800)
			{
				if (x < MAX_WIDTH_LV2_1 + 100)
				{
					if (check3 == false)
						simon->SetState(SIMON_STATE_WALK);
				}
				else if (x > MAX_WIDTH_LV2_1 + 100 && x<MAX_WIDTH_LV2_1 +105)
				{
					simon->SetState(SIMON_STATE_IDLE);
					checkScene1 = true;
					check3 = true;
					if (check_close_door_time == false)
					{
						close_door_time = GetTickCount();
						check_close_door_time = true;
					}
				}
			}
		}
		
		if (checkScene1 == true)
		{		
			if (game->x_cam < MAX_WIDTH_LV2_1)
			{
				if (GetTickCount() - close_door_time > 1800)
					game->x_cam += SIMON_WALKING_SPEED * dt;
			}
			else 
			{
				if (CGame::start_fight_boss == false)
				{
					if (x > MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2 && x < MAX_WIDTH_BOSS - SCREEN_WIDTH / 2)
					{
						game->x_cam = x - SCREEN_WIDTH / 2;
					}
					else if (x > MAX_WIDTH_BOSS - SCREEN_WIDTH / 2) {
						game->x_cam = MAX_WIDTH_BOSS - SCREEN_WIDTH;
						CGame::start_fight_boss = true;
					}
					else if (x < MAX_WIDTH_LV2_1 + SCREEN_WIDTH / 2)
					{
						game->x_cam = MAX_WIDTH_LV2_1;
					}
					isEnableKeyBoard = true;
				}
				else
					game->x_cam = MAX_WIDTH_BOSS - SCREEN_WIDTH;
			}
		}
	}
	else if (lv == 35)
	{
		if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
		}
		else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV3_5 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;

		}
		else if (x > MAX_WIDTH_LV3_5 - SCREEN_WIDTH / 2)
			game->x_cam = MAX_WIDTH_LV3_5 - SCREEN_WIDTH;
	}
	else if (lv == 34)
	{
		if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
		}
		else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV3_4 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;

		}
		else if (x > MAX_WIDTH_LV3_4 - SCREEN_WIDTH / 2)
			game->x_cam = MAX_WIDTH_LV3_4 - SCREEN_WIDTH;
	}
	else if (lv == 33)
	{
		if (x < SCREEN_WIDTH / 2)
		{
			game->x_cam = 0;
		}
		else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV3_3 - SCREEN_WIDTH / 2)
		{
			game->x_cam = x - SCREEN_WIDTH / 2;

		}
		else if (x > MAX_WIDTH_LV3_3 - SCREEN_WIDTH / 2)
			game->x_cam = MAX_WIDTH_LV3_3 - SCREEN_WIDTH;
	}
	else if (lv == 32)
	{
		game->x_cam = 0;
		
	}
	else if (lv == 31)
	{
		if (CGame::GetInstance()->start_fight_boss == false)
		{
			if (x < SCREEN_WIDTH / 2)
			{
				game->x_cam = 0;
				CGame::GetInstance()->start_fight_boss = true;
			}
			else if (x > SCREEN_WIDTH / 2 && x < MAX_WIDTH_LV3_1 - SCREEN_WIDTH / 2)
			{
				game->x_cam = x - SCREEN_WIDTH / 2;

			}
			else if (x > MAX_WIDTH_LV3_1 - SCREEN_WIDTH / 2)
				game->x_cam = MAX_WIDTH_LV3_1 - SCREEN_WIDTH;
		}
		else
			game->x_cam = 0;
	}
#pragma endregion

#pragma region UI

	if (lv == 1)
		ui->Update(gameTime / 1000, 1, simon);
	else
		ui->Update(gameTime / 1000, 2, simon);

#pragma endregion
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
		CTextures * textures = CTextures::GetInstance();			
		float x, y;
		simon->GetPosition(x, y);

		LPDIRECT3DTEXTURE9 tileset = textures->Get(ID_TEX_TILESET);
		LPDIRECT3DTEXTURE9 tileset1 = textures->Get(ID_TEX_TILESET_2);
		LPDIRECT3DTEXTURE9 tileset2 = textures->Get(ID_TEX_TILESET_3);
		LPDIRECT3DTEXTURE9 tileset31 = textures->Get(ID_TEX_TILESET31);
		LPDIRECT3DTEXTURE9 tileset32 = textures->Get(ID_TEX_TILESET32);
		LPDIRECT3DTEXTURE9 tileset33 = textures->Get(ID_TEX_TILESET33);
		LPDIRECT3DTEXTURE9 tileset34 = textures->Get(ID_TEX_TILESET34);
		LPDIRECT3DTEXTURE9 tileset35 = textures->Get(ID_TEX_TILESET35);
		
		if (lv==1 && checkload == false)
		{
			map = new	Map ( tileset, 32, 32); 
			map->LoadMatrixMap("Castlevania\\Mapstate.txt");
			checkload = true;
			/*map = new	Map(tileset35, 32, 32);
			map->LoadMatrixMap("Castlevania\\map35_bg.txt");*/
			
		}
		else if( (lv == 2 || lv == 21 || lv==99) && checkload == false ) {									
			map = new	Map (tileset1, 32, 32); 
			map->LoadMatrixMap("Castlevania\\Mapstate2.txt");			
			checkload = true;
		}
		else if (lv == 22 && checkload == false)
		{
			map = new	Map(tileset2, 32, 32);
			map->LoadMatrixMap("Castlevania\\Mapstate2_1.txt");
			checkload = true;
		}
		else if (lv == 31 && checkload == false)
		{
			map = new	Map(tileset31, 32, 32);
			map->LoadMatrixMap("Castlevania\\map31_bg.txt");
			checkload = true;
		}
		else if (lv == 32 && checkload == false)
		{
			map = new	Map(tileset32, 32, 32);
			map->LoadMatrixMap("Castlevania\\map32_bg.txt");
			checkload = true;
		}
		else if (lv == 33 && checkload == false)
		{
			map = new	Map(tileset33, 32, 32);
			map->LoadMatrixMap("Castlevania\\map33_bg.txt");
			checkload = true;
		}
		else if (lv == 34 && checkload == false)
		{
			map = new	Map(tileset34, 32, 32);
			map->LoadMatrixMap("Castlevania\\map34_bg.txt");
			checkload = true;
		}
		else if (lv == 35 && checkload == false)
		{
			map = new	Map(tileset35, 32, 32);
			map->LoadMatrixMap("Castlevania\\map35_bg.txt");
			checkload = true;
		}
		map->Draw(game->x_cam, game->y_cam);

		for (int i = 0; i < currentGrids.size(); i++)
		{
			vector<LPGAMEOBJECT> listObject = currentGrids[i]->GetListObject();
			int listObjectSize = listObject.size();

			for (int j = 0; j < listObjectSize; j++)
			{
				listObject[j]->Render();
			}
		}

		if (simon->isRosaryUsed)
		{
			if (transparent)
			{
				animations->Get(4444)->Render(game->x_cam, game->y_cam, 100);
			}
			transparent = !transparent;

			if (GetTickCount() - simon->rosaryCast > ROSARY_TIME)
			{
				simon->isRosaryUsed = false;
				isEnableKeyBoard = true;
			}
		}

		simon->Render();
		ui->Render(game->x_cam, game->y_cam,simon, CGame::bossheath, lv);
		
		/*RECT newRect;
		SetRect(&newRect, 0, 0, 30, 30);
		D3DXVECTOR3 p(0, 0, 0);
		spriteHandler->Draw(textures->Get(ID_TEX_AXE), &newRect, NULL, &p, D3DCOLOR_ARGB(255, 255, 255, 255));*/
		
		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	keyHandler = new CSampleKeyHander();
	game->InitKeyboard(keyHandler);
	
	listGrids = ListGrids::GetInstance();

	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}